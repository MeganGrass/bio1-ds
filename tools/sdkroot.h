/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  tools\sdkroot.h
 */

/*
    Shared helper: reads the locations of the PSY-Q SDK revisions (and the optional retail folder) from
    tools\sdkroot.ini, next to the executable, so a user can point the build at wherever the SDK is
    installed without editing a source file or the makefile. The build needs several PSY-Q releases side
    by side (specific library members from each, to match the retail executables byte for byte), so each
    release is its own ini key. tools\sdkroot.ini is the only place these paths live: nothing else in the
    project hardcodes one. A missing ini or key is a hard error, not a silent default.

    Paths must not contain spaces: ASPSX and PSYLINK split their arguments at spaces.
*/
#ifndef SDKROOT_H
#define SDKROOT_H
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* directory of this executable (tools\) */
static void sdkroot_exe_dir(char* out, size_t outsz)
{
    char path[MAX_PATH];
    DWORD n = GetModuleFileNameA(NULL, path, sizeof path);
    if (n == 0 || n >= sizeof path)
    {
        strncpy(out, ".", outsz);
        out[outsz - 1] = 0;
        return;
    }
    char* slash = strrchr(path, '\\');
    if (slash)
    {
        *slash = 0;
    }
    else
    {
        strcpy(path, ".");
    }
    strncpy(out, path, outsz - 1);
    out[outsz - 1] = 0;
}

/* looks key up in ini_path; returns 1 and fills out when found with a non-empty value, else 0 (ini missing: 0 as well) */
static int sdkroot_ini_lookup(const char* ini_path, const char* key, char* out, size_t outsz)
{
    FILE* f = fopen(ini_path, "rb");
    if (!f)
    {
        return 0;
    }
    size_t keylen = strlen(key);
    char line[1024];
    while (fgets(line, sizeof line, f))
    {
        char* h = strchr(line, '#');
        if (h)
        {
            *h = 0;
        }
        char* s = line;
        while (*s == ' ' || *s == '\t')
        {
            s++;
        }
        if (strncmp(s, key, keylen) == 0 && s[keylen] == '=')
        {
            char* val = s + keylen + 1;
            size_t vl = strlen(val);
            while (vl > 0 && (val[vl - 1] == '\n' || val[vl - 1] == '\r' || val[vl - 1] == ' ' || val[vl - 1] == '\t'))
            {
                val[--vl] = 0;
            }
            if (vl > 0)
            {
                strncpy(out, val, outsz - 1);
                out[outsz - 1] = 0;
                fclose(f);
                return 1;
            }
            break;
        }
    }
    fclose(f);
    return 0;
}

/* value of key in tools\sdkroot.ini, or exits with a message naming the key to add */
static void sdkroot_get(const char* key, char* out, size_t outsz)
{
    char dir[MAX_PATH];
    char ini[MAX_PATH + 32];
    sdkroot_exe_dir(dir, sizeof dir);
    snprintf(ini, sizeof ini, "%s\\sdkroot.ini", dir);
    if (!sdkroot_ini_lookup(ini, key, out, outsz))
    {
        fprintf(stderr, "sdkroot: %s has no %s= line (or the file is missing). Add one, e.g.\n  %s=C:\\path\\to\\%s\n", ini, key, key, key);
        exit(1);
    }
}

/* like sdkroot_get, but returns 0 (and leaves out empty) when the key is absent */
static int sdkroot_get_optional(const char* key, char* out, size_t outsz)
{
    char dir[MAX_PATH];
    char ini[MAX_PATH + 32];
    sdkroot_exe_dir(dir, sizeof dir);
    snprintf(ini, sizeof ini, "%s\\sdkroot.ini", dir);
    out[0] = 0;
    return sdkroot_ini_lookup(ini, key, out, outsz);
}

#endif
