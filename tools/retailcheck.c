/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  tools\retailcheck.c
 */

/*
    Checks the 13 linked PSX USA executables against the retail game:

        OBJ\PSX\USA\SLUS_007.47
        OBJ\PSX\USA\PROG2\{LOGO,PROLOGUE,TITLE,SELECT,ENDING,STAGE1,STAGE2,STAGE3,STAGE4,STAGE5,STAGE6,STAGE7}.EXE

    Each file is checked
      1. against the SHA-256 of the retail file pinned in Bio1\config\psx-usa\retail_sha256.txt (always possible;
         sha256sum style: "<64 hex digits>  <relative path>", paths with forward slashes as listed above);
      2. against the retail file itself, read only, when tools\sdkroot.ini has RETAIL_USA and that folder exists
         (SLUS_007.47 and PSXUSA\PROG2\<NAME>.EXE there), with the number of differing bytes.
    A file passes when it exists, is not empty, starts with the 8 bytes "PS-X EXE" and has the pinned hash (and equals the
    retail file when that comparison is possible). A missing (MISSING), unreadable (UNREADABLE), empty, mistyped or different file is reported by name.
    The exit code is 0 only when all 13 pass, else 1. The SHA-256 comes from the Windows BCrypt API.

    usage: retailcheck [--quiet]
      --quiet        print only problems and the summary
    hidden overrides (for tests):
      --obj DIR      check the files below DIR instead of OBJ\PSX\USA
      --pins FILE    read the pinned hashes from FILE
      --retail DIR   compare with the retail files in DIR instead of RETAIL_USA from sdkroot.ini

    The tool finds the project from its own location (tools\ is below the project root), so the current directory does
    not matter.
*/
#include "sdkroot.h"
#include <ctype.h>
#include <bcrypt.h>

#pragma comment(lib, "bcrypt.lib")

#define NFILES 13

typedef struct
{
    const char* ours;   /* path below the output folder, and the key of the pinned hash list */
    const char* theirs; /* path below the retail folder */
} FILEDEF;

static const FILEDEF FILES[NFILES] = {
    {"SLUS_007.47", "SLUS_007.47"},
    {"PROG2/LOGO.EXE", "PSXUSA/PROG2/LOGO.EXE"},
    {"PROG2/PROLOGUE.EXE", "PSXUSA/PROG2/PROLOGUE.EXE"},
    {"PROG2/TITLE.EXE", "PSXUSA/PROG2/TITLE.EXE"},
    {"PROG2/SELECT.EXE", "PSXUSA/PROG2/SELECT.EXE"},
    {"PROG2/ENDING.EXE", "PSXUSA/PROG2/ENDING.EXE"},
    {"PROG2/STAGE1.EXE", "PSXUSA/PROG2/STAGE1.EXE"},
    {"PROG2/STAGE2.EXE", "PSXUSA/PROG2/STAGE2.EXE"},
    {"PROG2/STAGE3.EXE", "PSXUSA/PROG2/STAGE3.EXE"},
    {"PROG2/STAGE4.EXE", "PSXUSA/PROG2/STAGE4.EXE"},
    {"PROG2/STAGE5.EXE", "PSXUSA/PROG2/STAGE5.EXE"},
    {"PROG2/STAGE6.EXE", "PSXUSA/PROG2/STAGE6.EXE"},
    {"PROG2/STAGE7.EXE", "PSXUSA/PROG2/STAGE7.EXE"},
};

typedef struct
{
    char hash[65];
    char path[128];
} PIN;

static PIN pins[64];
static int npins;

static int is_dir(const char* path)
{
    DWORD a = GetFileAttributesA(path);
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

/* whole file into a malloc'd buffer (never NULL for a readable file, even an empty one); NULL when absent or unreadable */
static unsigned char* read_file(const char* path, size_t* len)
{
    FILE* f;
    long n;
    unsigned char* buf;
    DWORD attr = GetFileAttributesA(path);
    if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY))
    {
        return NULL;
    }
    f = fopen(path, "rb");
    if (!f)
    {
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (n < 0)
    {
        fclose(f);
        return NULL;
    }
    buf = (unsigned char*)malloc((size_t)n + 1);
    if (!buf || fread(buf, 1, (size_t)n, f) != (size_t)n)
    {
        free(buf);
        fclose(f);
        return NULL;
    }
    fclose(f);
    *len = (size_t)n;
    return buf;
}

static int sha256_hex(const unsigned char* data, size_t len, char* hex)
{
    unsigned char digest[32];
    int i;
    if (BCryptHash(BCRYPT_SHA256_ALG_HANDLE, NULL, 0, (PUCHAR)data, (ULONG)len, digest, sizeof digest) != 0)
    {
        return 0;
    }
    for (i = 0; i < 32; i++)
    {
        sprintf(hex + 2 * i, "%02x", digest[i]);
    }
    return 1;
}

/* "1234567" -> "1,234,567" */
static const char* thousands(size_t v, char* out)
{
    char plain[32];
    int n, i, o = 0;
    sprintf(plain, "%zu", v);
    n = (int)strlen(plain);
    for (i = 0; i < n; i++)
    {
        if (i > 0 && (n - i) % 3 == 0)
        {
            out[o++] = ',';
        }
        out[o++] = plain[i];
    }
    out[o] = 0;
    return out;
}

static size_t differing_bytes(const unsigned char* x, size_t nx, const unsigned char* y, size_t ny)
{
    size_t n = nx < ny ? nx : ny, i, diff = 0;
    for (i = 0; i < n; i++)
    {
        diff += x[i] != y[i];
    }
    return diff + (nx > ny ? nx - ny : ny - nx);
}

/* out = s; returns 0 (out unusable) when it does not fit */
static int copy_path(char* out, size_t outsz, const char* s)
{
    size_t n = strlen(s);
    if (n >= outsz)
    {
        return 0;
    }
    memcpy(out, s, n + 1);
    return 1;
}

/* out = a + "\" + b; returns 0 (out unusable) when it does not fit */
static int join_path(char* out, size_t outsz, const char* a, const char* b)
{
    int n = snprintf(out, outsz, "%s\\%s", a, b);
    return n >= 0 && (size_t)n < outsz;
}

static int is_hex64(const char* s)
{
    int i;
    for (i = 0; i < 64; i++)
    {
        if (!((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'f') || (s[i] >= 'A' && s[i] <= 'F')))
        {
            return 0;
        }
    }
    return 1;
}

/* reads the sha256sum style pin list; returns 0 (message printed) when the file is missing or has no valid line */
static int load_pins(const char* path)
{
    FILE* f = fopen(path, "rb");
    char line[512];
    int lineno = 0;
    if (!f)
    {
        fprintf(stderr, "retailcheck: cannot read the pinned hashes %s\n", path);
        return 0;
    }
    while (fgets(line, sizeof line, f))
    {
        char* s = line;
        size_t n, k;
        lineno++;
        if (lineno == 1 && memcmp(line, "\xEF\xBB\xBF", 3) == 0)
        {
            s += 3; /* UTF-8 byte order mark */
        }
        n = strlen(line);
        while (n > 0 && (line[n - 1] == '\n' || line[n - 1] == '\r' || line[n - 1] == ' ' || line[n - 1] == '\t'))
        {
            line[--n] = 0;
        }
        while (*s == ' ' || *s == '\t')
        {
            s++;
        }
        if (*s == 0 || *s == '#')
        {
            continue;
        }
        if (!is_hex64(s) || (s[64] != ' ' && s[64] != '\t'))
        {
            fprintf(stderr, "retailcheck: %s line %d: expected \"<64 hex digits>  <path>\"\n", path, lineno);
            fclose(f);
            return 0;
        }
        if (npins == (int)(sizeof pins / sizeof pins[0]))
        {
            fprintf(stderr, "retailcheck: %s: too many lines\n", path);
            fclose(f);
            return 0;
        }
        memcpy(pins[npins].hash, s, 64);
        pins[npins].hash[64] = 0;
        for (k = 0; k < 64; k++)
        {
            pins[npins].hash[k] = (char)tolower((unsigned char)pins[npins].hash[k]);
        }
        s += 64;
        while (*s == ' ' || *s == '\t' || *s == '*')
        {
            s++;
        }
        strncpy(pins[npins].path, s, sizeof pins[npins].path - 1);
        pins[npins].path[sizeof pins[npins].path - 1] = 0;
        for (k = 0; pins[npins].path[k]; k++)
        {
            if (pins[npins].path[k] == '\\')
            {
                pins[npins].path[k] = '/';
            }
        }
        npins++;
    }
    fclose(f);
    if (npins == 0)
    {
        fprintf(stderr, "retailcheck: %s holds no pinned hashes\n", path);
        return 0;
    }
    return 1;
}

static const char* pinned_hash(const char* path)
{
    int i;
    for (i = 0; i < npins; i++)
    {
        if (strcmp(pins[i].path, path) == 0)
        {
            return pins[i].hash;
        }
    }
    return NULL;
}

int main(int argc, char** argv)
{
    char tools[MAX_PATH], root[MAX_PATH], obj[MAX_PATH], pinfile[MAX_PATH], retail[MAX_PATH];
    char how[MAX_PATH + 128];
    const char* obj_arg = NULL;
    const char* pins_arg = NULL;
    const char* retail_arg = NULL;
    int quiet = 0, have_retail = 0, ok = 0, a, i;

    for (a = 1; a < argc; a++)
    {
        if (strcmp(argv[a], "--quiet") == 0)
        {
            quiet = 1;
        }
        else if (strcmp(argv[a], "--obj") == 0 && a + 1 < argc)
        {
            obj_arg = argv[++a];
        }
        else if (strcmp(argv[a], "--pins") == 0 && a + 1 < argc)
        {
            pins_arg = argv[++a];
        }
        else if (strcmp(argv[a], "--retail") == 0 && a + 1 < argc)
        {
            retail_arg = argv[++a];
        }
        else
        {
            fprintf(stderr, "usage: retailcheck [--quiet]\n");
            return 2;
        }
    }

    sdkroot_exe_dir(tools, sizeof tools);
    if (!join_path(root, sizeof root, tools, "..") ||
        !(obj_arg ? copy_path(obj, sizeof obj, obj_arg) : join_path(obj, sizeof obj, root, "OBJ\\PSX\\USA")) ||
        !(pins_arg ? copy_path(pinfile, sizeof pinfile, pins_arg)
                   : join_path(pinfile, sizeof pinfile, root, "Bio1\\config\\psx-usa\\retail_sha256.txt")))
    {
        fprintf(stderr, "retailcheck: path too long\n");
        return 1;
    }
    if (!load_pins(pinfile))
    {
        return 1;
    }

    /* the retail folder is optional: RETAIL_USA in sdkroot.ini (or --retail); without it only the hashes are checked */
    retail[0] = 0;
    if (retail_arg)
    {
        if (!copy_path(retail, sizeof retail, retail_arg))
        {
            fprintf(stderr, "retailcheck: path too long\n");
            return 1;
        }
    }
    else
    {
        sdkroot_get_optional("RETAIL_USA", retail, sizeof retail);
    }
    have_retail = retail[0] && is_dir(retail);
    if (have_retail)
    {
        snprintf(how, sizeof how, "against the pinned hashes and the retail folder");
    }
    else if (retail[0])
    {
        snprintf(how, sizeof how, "against the pinned hashes only (retail folder not found: %s); NOT compared byte by byte", retail);
    }
    else
    {
        snprintf(how, sizeof how, "against the pinned hashes only (retail folder not found: RETAIL_USA is not set in sdkroot.ini); NOT compared byte by byte");
    }

    for (i = 0; i < NFILES; i++)
    {
        char path[MAX_PATH], hex[65], problems[1024], num[32];
        size_t len = 0;
        unsigned char* x;
        const char* pin;
        problems[0] = 0;

        if (!join_path(path, sizeof path, obj, FILES[i].ours))
        {
            printf("  UNREADABLE %s (path too long)\n", FILES[i].ours);
            continue;
        }
        x = read_file(path, &len);
        if (!x)
        {
            DWORD attr = GetFileAttributesA(path);
            if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY))
            {
                printf("  MISSING   %s\n", FILES[i].ours);
            }
            else
            {
                printf("  UNREADABLE %s (cannot open or read it)\n", FILES[i].ours);
            }
            continue;
        }
        if (!sha256_hex(x, len, hex))
        {
            strcpy(hex, "(sha256 failed)");
        }
        pin = pinned_hash(FILES[i].ours);
        if (len == 0)
        {
            strcat(problems, "empty file");
        }
        else if (len < 8 || memcmp(x, "PS-X EXE", 8) != 0)
        {
            strcat(problems, "not a PS-X EXE");
        }
        if (!pin)
        {
            strcat(problems, problems[0] ? "; " : "");
            strcat(problems, "no pinned hash for this file");
        }
        else if (strcmp(hex, pin) != 0)
        {
            strcat(problems, problems[0] ? "; " : "");
            strcat(problems, "hash differs from the pinned retail hash");
        }
        if (have_retail)
        {
            char rpath[MAX_PATH], msg[MAX_PATH + 64];
            size_t rlen = 0;
            unsigned char* y;
            y = join_path(rpath, sizeof rpath, retail, FILES[i].theirs) ? read_file(rpath, &rlen) : NULL;
            msg[0] = 0;
            if (!y)
            {
                snprintf(msg, sizeof msg, "retail file not found (%s)", FILES[i].theirs);
            }
            else if (len != rlen || memcmp(x, y, len) != 0)
            {
                size_t diff = differing_bytes(x, len, y, rlen);
                snprintf(msg, sizeof msg, "%s %s from the retail file", thousands(diff, num), diff == 1 ? "byte differs" : "bytes differ");
            }
            if (msg[0])
            {
                strcat(problems, problems[0] ? "; " : "");
                strcat(problems, msg);
            }
            free(y);
        }
        if (problems[0])
        {
            printf("  DIFFERENT %-20s %8zu bytes  sha256 %s  %s\n", FILES[i].ours, len, hex, problems);
        }
        else
        {
            ok++;
            if (!quiet)
            {
                printf("  identical %-20s %8zu bytes  sha256 %s\n", FILES[i].ours, len, hex);
            }
        }
        free(x);
    }
    printf("retail check: %d of %d files identical to the retail executables, %s\n", ok, NFILES, how);
    return ok == NFILES ? 0 : 1;
}
