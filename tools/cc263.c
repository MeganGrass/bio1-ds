/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  tools\cc263.c
 */

/*
    Compiles one game source into a PsyQ LNK object with the GCC 2.6.3 PlayStation compiler.

    Pipeline: preprocess (tools\gcc-2.6.3-psx\cpp.exe) -> cc1 (tools\gcc-2.6.3-psx\cc1.exe, target
    mips-sony-psx) -> CRLF -> ASPSX 2.56 (PSY-Q 4.0, location from tools\sdkroot.ini, key PSYQ_4_0).
    The retail USA executables are reproduced byte for byte by this compiler (PsyQ's own GCC 2.7.2 does not).

    Intermediates go to the stage folder, one set of files per unit (SRC stem), so many cc263 runs can
    work in parallel as long as each uses its own stage folder or its own stems:

        STAGEDIR\STEM.i      the preprocessed source (cpp's stdout, byte for byte)
        STAGEDIR\STEM.s      the compiler's assembly output; LF converted to CRLF when it has no CRLF yet
        STAGEDIR\STEM.obj    ASPSX's object, copied to the -o path when everything succeeded

    The absolute path of SRC (backslash form, from _fullpath) is what cpp sees and what ends up in the
    object's file name record, so the same source gives the same object from any current directory.

    ASPSX is an old DOS-style tool that splits its arguments at spaces and fails on long paths, so it
    runs with the stage folder as its current directory and gets only bare file names. -G<A> is the
    assembler's small-data threshold (0: no .sbss split, every uninitialised static lands in one .bss
    in declaration order, as in retail).

    Relative -I, -w and -o paths are taken from the current directory (-I folders are passed to cpp as
    absolute paths); tools\ is found from the executable's own path (the compiler lives in
    tools\gcc-2.6.3-psx). A step that runs longer than 15 minutes is killed and reported as failed.

    usage: cc263 [-O0|-O1|-O2] [-G<n>] [-A<n>] [-s<NAME>] [-D<NAME>]... [-I<dir>]... -w<STAGEDIR> SRC.c -o<OUT.obj>
      -O<n>   compiler optimisation level (default -O2)
      -G<n>   compiler small-data threshold (default 0)
      -A<n>   assembler (ASPSX) small-data threshold (default 0)
      -s<NAME> the object's sections are named NAME.text, NAME.data, ... in a group NAME (ASPSX -sNAME, what bio2-ds
              gives its overlays), so that PSYLINK can put the objects of each overlay into their own group
      -D<NAME>[=<VALUE>] a preprocessor definition (in order, before the include directories)
      -I<dir> an include directory for the preprocessor (in order, before the directory of SRC)
      -w<dir> stage folder for the intermediate files (created when missing)
      -o<obj> the object to write (its folder is created when missing)

    On a failure the tool prints "cc263: cpp:", "cc263: cc1:" or "cc263: aspsx:" and the first 400
    characters of that step's output, and exits 1.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include "sdkroot.h"

#define MAXINC 64
#define MAXDEF 16
#define MSG_CHARS 400
#define TIMEOUT_MS 900000   /* a step that runs longer than 15 minutes is killed and reported as failed */
#define PATH_BUF (2 * MAX_PATH + 16)

/* creates dir and its missing parents; returns 1 when the folder exists afterwards */
static int make_dirs(const char* dir)
{
    char tmp[MAX_PATH];
    size_t n = strlen(dir);
    if (n == 0 || n >= sizeof tmp)
    {
        return 0;
    }
    memcpy(tmp, dir, n + 1);
    for (size_t i = 1; i < n; i++)
    {
        if ((tmp[i] == '\\' || tmp[i] == '/') && tmp[i - 1] != ':')
        {
            char c = tmp[i];
            tmp[i] = 0;
            CreateDirectoryA(tmp, NULL);
            tmp[i] = c;
        }
    }
    CreateDirectoryA(tmp, NULL);
    DWORD a = GetFileAttributesA(tmp);
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY);
}

static void fail(const char* step, const char* text, size_t len)
{
    if (len > MSG_CHARS)
    {
        len = MSG_CHARS;
    }
    fprintf(stderr, "cc263: %s%s%.*s\n", step, step[0] ? ": " : "", (int)len, text);
    exit(1);
}

static void die(const char* msg, const char* arg)
{
    fprintf(stderr, "cc263: %s%s%s\n", msg, arg[0] ? " " : "", arg);
    exit(1);
}

/* the whole file in a malloc'd buffer (NUL terminated, *len bytes before the NUL), or NULL */
static char* read_file(const char* path, size_t* len)
{
    FILE* f = fopen(path, "rb");
    if (!f)
    {
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buf = size < 0 ? NULL : malloc((size_t)size + 1);
    if (!buf)
    {
        fclose(f);
        return NULL;
    }
    size_t got = fread(buf, 1, (size_t)size, f);
    fclose(f);
    buf[got] = 0;
    *len = got;
    return buf;
}

static int file_exists(const char* path)
{
    return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
}

/* appends one command-line argument, quoted, to cmd (embedded quotes and backslashes before a quote are escaped) */
static void add_arg(char* cmd, size_t cmdsz, const char* arg)
{
    size_t n = strlen(cmd);
    if (n + 2 * strlen(arg) + 4 >= cmdsz)
    {
        die("command line too long:", arg);
    }
    if (n)
    {
        cmd[n++] = ' ';
    }
    cmd[n++] = '"';
    for (const char* p = arg;; p++)
    {
        size_t slashes = 0;
        while (*p == '\\')
        {
            slashes++;
            p++;
        }
        if (*p == 0 || *p == '"')
        {
            slashes *= 2;                       /* backslashes in front of a quote (or of the closing quote) double */
        }
        while (slashes--)
        {
            cmd[n++] = '\\';
        }
        if (*p == 0)
        {
            break;
        }
        if (*p == '"')
        {
            cmd[n++] = '\\';
        }
        cmd[n++] = *p;
    }
    cmd[n++] = '"';
    cmd[n] = 0;
}

/* copies the folder part of path (up to the last backslash) into dir; a drive root keeps its backslash ("C:\") */
static void dir_of(const char* path, char* dir, size_t dirsz)
{
    strncpy(dir, path, dirsz - 1);
    dir[dirsz - 1] = 0;
    char* slash = strrchr(dir, '\\');
    if (slash)
    {
        if (slash == dir + 2 && dir[1] == ':')
        {
            slash[1] = 0;
        }
        else
        {
            *slash = 0;
        }
    }
}

/*
    Runs exe with command line cmd (argument 0 included) in directory cwd (NULL: current). The child's
    stdout goes to out_path (a file; NULL: the log) and its stderr to log_path, so nothing can fill a
    pipe and block; with out_path NULL, stdout and stderr share log_path. Returns the exit code, -1
    when the process could not be started, or -2 when it was killed after TIMEOUT_MS.
*/
static int run(const char* exe, char* cmd, const char* cwd, const char* out_path, const char* log_path)
{
    SECURITY_ATTRIBUTES sa = {sizeof sa, NULL, TRUE};
    HANDLE nul = CreateFileA("NUL", GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa, OPEN_EXISTING, 0, NULL);
    HANDLE log = CreateFileA(log_path, GENERIC_WRITE, FILE_SHARE_READ, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE out = log;
    if (out_path)
    {
        out = CreateFileA(out_path, GENERIC_WRITE, FILE_SHARE_READ, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    }
    if (nul == INVALID_HANDLE_VALUE || log == INVALID_HANDLE_VALUE || out == INVALID_HANDLE_VALUE)
    {
        die("cannot create the output files for", exe);
    }
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    memset(&si, 0, sizeof si);
    si.cb = sizeof si;
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = nul;
    si.hStdOutput = out;
    si.hStdError = log;
    DWORD code = (DWORD)-1;
    if (CreateProcessA(exe, cmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, cwd, &si, &pi))
    {
        if (WaitForSingleObject(pi.hProcess, TIMEOUT_MS) == WAIT_TIMEOUT)
        {
            TerminateProcess(pi.hProcess, 1);
            WaitForSingleObject(pi.hProcess, INFINITE);
            code = (DWORD)-2;
        }
        else
        {
            GetExitCodeProcess(pi.hProcess, &code);
        }
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }
    CloseHandle(nul);
    if (out != log)
    {
        CloseHandle(out);
    }
    CloseHandle(log);
    return (int)code;
}

/* reports the log of a failed step */
static void fail_log(const char* step, const char* log_path, int code)
{
    size_t len = 0;
    char* text = read_file(log_path, &len);
    if (code == -2)
    {
        fail(step, "timed out", 9);
    }
    if (code == -1 && (!text || len == 0))
    {
        fail(step, "cannot start the program", 24);
    }
    fail(step, text ? text : "", len);
}

int main(int argc, char** argv)
{
    const char* opt = "-O2";
    int gcc_g = 0, asm_g = 0;
    const char* sect_name = NULL;
    const char* inc[MAXINC];
    int ninc = 0;
    const char* def[MAXDEF];
    int ndef = 0;
    const char* stage_arg = NULL;
    const char* src_arg = NULL;
    const char* out_arg = NULL;

    for (int i = 1; i < argc; i++)
    {
        const char* a = argv[i];
        if (a[0] == '-' && a[1] == 'O' && a[2] >= '0' && a[2] <= '9' && a[3] == 0)
        {
            opt = a;
        }
        else if (a[0] == '-' && a[1] == 'G' && a[2])
        {
            gcc_g = atoi(a + 2);
        }
        else if (a[0] == '-' && a[1] == 'A' && a[2])
        {
            asm_g = atoi(a + 2);
        }
        else if (a[0] == '-' && a[1] == 's' && a[2])
        {
            sect_name = a + 2;
        }
        else if (a[0] == '-' && a[1] == 'D' && a[2])
        {
            if (ndef >= MAXDEF)
            {
                die("too many -D definitions", "");
            }
            def[ndef++] = a;
        }
        else if (a[0] == '-' && a[1] == 'I')
        {
            const char* d = a + 2;
            if (!*d && i + 1 < argc)
            {
                d = argv[++i];
            }
            if (ninc >= MAXINC)
            {
                die("too many -I directories", "");
            }
            inc[ninc++] = d;
        }
        else if (a[0] == '-' && a[1] == 'w')
        {
            stage_arg = a[2] ? a + 2 : (i + 1 < argc ? argv[++i] : NULL);
        }
        else if (a[0] == '-' && a[1] == 'o')
        {
            out_arg = a[2] ? a + 2 : (i + 1 < argc ? argv[++i] : NULL);
        }
        else if (a[0] != '-' && !src_arg)
        {
            src_arg = a;
        }
        else
        {
            die("unknown option", a);
        }
    }
    if (!src_arg || !stage_arg || !out_arg)
    {
        fprintf(stderr, "usage: cc263 [-O0|-O1|-O2] [-G<n>] [-A<n>] [-s<NAME>] [-D<NAME>]... [-I<dir>]... -w<STAGEDIR> SRC.c -o<OUT.obj>\n");
        return 1;
    }

    char tools[MAX_PATH], psyq[MAX_PATH];
    sdkroot_exe_dir(tools, sizeof tools);
    sdkroot_get("PSYQ_4_0", psyq, sizeof psyq);
    char cpp[MAX_PATH + 40], cc1[MAX_PATH + 40], aspsx[MAX_PATH + 40];
    snprintf(cpp, sizeof cpp, "%s\\gcc-2.6.3-psx\\cpp.exe", tools);
    snprintf(cc1, sizeof cc1, "%s\\gcc-2.6.3-psx\\cc1.exe", tools);
    snprintf(aspsx, sizeof aspsx, "%s\\PSYQ\\ASPSX.EXE", psyq);
    if (!file_exists(cpp) || !file_exists(cc1))
    {
        die("compiler not found in", tools);
    }
    if (!file_exists(aspsx))
    {
        die("ASPSX not found:", aspsx);
    }

    char src[MAX_PATH], srcdir[MAX_PATH], stage[MAX_PATH], out[MAX_PATH];
    if (!_fullpath(src, src_arg, sizeof src) || !_fullpath(stage, stage_arg, sizeof stage) || !_fullpath(out, out_arg, sizeof out))
    {
        die("bad path", "");
    }
    if (!file_exists(src))
    {
        die("no such source:", src);
    }
    dir_of(src, srcdir, sizeof srcdir);
    char stem[MAX_PATH];
    const char* slash = strrchr(src, '\\');
    strncpy(stem, slash ? slash + 1 : src, sizeof stem - 1);
    stem[sizeof stem - 1] = 0;
    char* dot = strrchr(stem, '.');
    if (dot)
    {
        *dot = 0;
    }
    if (stem[0] == 0 || strchr(stem, ' '))
    {
        die("ASPSX cannot take a source name that is empty or has spaces:", src);
    }

    if (!make_dirs(stage))
    {
        die("cannot create the stage folder", stage);
    }

    /* stage and stem are each shorter than MAX_PATH, so these cannot truncate */
    char f_i[PATH_BUF], f_s[PATH_BUF], f_o[PATH_BUF], f_log[PATH_BUF];
    snprintf(f_i, sizeof f_i, "%s\\%s.i", stage, stem);
    snprintf(f_s, sizeof f_s, "%s\\%s.s", stage, stem);
    snprintf(f_o, sizeof f_o, "%s\\%s.obj", stage, stem);
    snprintf(f_log, sizeof f_log, "%s\\%s.log", stage, stem);
    DeleteFileA(f_i);
    DeleteFileA(f_s);
    DeleteFileA(f_o);
    DeleteFileA(f_log);

    char cmd[8192];
    int code;

    /* (a) preprocess: stdout is the .i, stderr the log */
    cmd[0] = 0;
    add_arg(cmd, sizeof cmd, cpp);
    add_arg(cmd, sizeof cmd, "-undef");
    add_arg(cmd, sizeof cmd, "-nostdinc");
    for (int i = 0; i < ndef; i++)
    {
        add_arg(cmd, sizeof cmd, def[i]);
    }
    for (int i = 0; i < ninc; i++)
    {
        char full[MAX_PATH];
        if (!_fullpath(full, inc[i], sizeof full))
        {
            die("bad include folder", inc[i]);
        }
        add_arg(cmd, sizeof cmd, "-I");
        add_arg(cmd, sizeof cmd, full);
    }
    add_arg(cmd, sizeof cmd, "-I");
    add_arg(cmd, sizeof cmd, srcdir);
    add_arg(cmd, sizeof cmd, src);
    code = run(cpp, cmd, NULL, f_i, f_log);
    if (code != 0)
    {
        fail_log("cpp", f_log, code);
    }

    /* (b) compile */
    char gopt[32];
    snprintf(gopt, sizeof gopt, "-G%d", gcc_g);
    cmd[0] = 0;
    add_arg(cmd, sizeof cmd, cc1);
    add_arg(cmd, sizeof cmd, "-quiet");
    add_arg(cmd, sizeof cmd, opt);
    add_arg(cmd, sizeof cmd, gopt);
    add_arg(cmd, sizeof cmd, "-o");
    add_arg(cmd, sizeof cmd, f_s);
    add_arg(cmd, sizeof cmd, f_i);
    code = run(cc1, cmd, NULL, NULL, f_log);
    if (code != 0 || !file_exists(f_s))
    {
        fail_log("cc1", f_log, code);
    }

    /* (c) ASPSX rejects bare LF ("Illegal character (10)"); our cc1 writes LF, so convert unless the file has CRLF */
    size_t len = 0;
    char* text = read_file(f_s, &len);
    if (!text)
    {
        die("cannot read", f_s);
    }
    int has_crlf = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (text[i] == '\n')
        {
            if (i > 0 && text[i - 1] == '\r')
            {
                has_crlf = 1;
            }
        }
    }
    if (!has_crlf)
    {
        FILE* fs = fopen(f_s, "wb");
        if (!fs)
        {
            die("cannot write", f_s);
        }
        size_t at = 0;
        for (size_t i = 0; i < len; i++)
        {
            if (text[i] == '\n')
            {
                fwrite(text + at, 1, i - at, fs);
                fwrite("\r\n", 1, 2, fs);
                at = i + 1;
            }
        }
        fwrite(text + at, 1, len - at, fs);
        int write_error = ferror(fs);
        if (fclose(fs) != 0 || write_error)
        {
            die("write error on", f_s);
        }
    }
    free(text);

    /* (d) assemble, in the stage folder with bare names */
    char name_s[MAX_PATH + 8], name_o[MAX_PATH + 8], aopt[32];
    snprintf(name_s, sizeof name_s, "%s.s", stem);
    snprintf(name_o, sizeof name_o, "%s.obj", stem);
    snprintf(aopt, sizeof aopt, "-G%d", asm_g);
    cmd[0] = 0;
    add_arg(cmd, sizeof cmd, aspsx);
    strcat(cmd, " -q ");
    strcat(cmd, aopt);
    if (sect_name)
    {
        if (strlen(cmd) + strlen(sect_name) + 8 >= sizeof cmd)
        {
            die("section name too long:", sect_name);
        }
        strcat(cmd, " -s");
        strcat(cmd, sect_name);
    }
    strcat(cmd, " ");
    strcat(cmd, name_s);
    strcat(cmd, " -o ");
    strcat(cmd, name_o);
    code = run(aspsx, cmd, stage, NULL, f_log);
    if (!file_exists(f_o))
    {
        fail_log("aspsx", f_log, code);
    }

    /* (e) deliver */
    dir_of(out, srcdir, sizeof srcdir);
    if (!make_dirs(srcdir))
    {
        die("cannot create the folder of", out);
    }
    if (!CopyFileA(f_o, out, FALSE))
    {
        die("cannot write", out);
    }
    DeleteFileA(f_log);
    return 0;
}
