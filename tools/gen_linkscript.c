/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  tools\gen_linkscript.c
 */

/*
    Resolves the @NAME@ placeholders of a PSYLINK link script template. PSYLINK reads its script directly and
    has no variable substitution of its own, so the link scripts are kept as templates (no SDK path written
    in them) and this tool writes the real one, with the PSY-Q install roots from tools\sdkroot.ini, just
    before the link.

    A placeholder is an @, a name made of letters, digits and underscores, and a closing @. Each one is replaced
    by, in this order:
      1. the value of a KEY=VALUE argument whose KEY is NAME (the makefile uses this for the per-build paths,
         e.g. @OUT@);
      2. the tools\sdkroot.ini value of the key NAME (e.g. @PSYQ_3_7@ -> C:\Framework\sdk\psx\PSY-Q_3_7).
    A placeholder that neither gives is an error naming it (and line number), and nothing is written. An @ that
    does not begin a placeholder (no closing @, or a character other than a name character in between) is left
    as it is. The rest of the template is copied byte for byte: line endings are not touched.

    PSYLINK cannot take a path with a space, so an ini value containing one is an error (a KEY=VALUE argument
    is the caller's own choice and is not checked). The ini is read from the folder of this executable, so the
    tool works from any current directory. OUT's folder is created if missing; relative TEMPLATE and OUT
    are relative to the current directory.

    usage: gen_linkscript TEMPLATE OUT [KEY=VALUE ...]
    Exit code 0 on success, 1 on any error.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sdkroot.h"

typedef struct
{
    char* data;
    size_t len, cap;
} OUTBUF;

static void out_add(OUTBUF* o, const char* s, size_t n)
{
    if (o->len + n + 1 > o->cap)
    {
        size_t nc = (o->len + n + 1) * 2;
        char* nd = realloc(o->data, nc);
        if (!nd)
        {
            fprintf(stderr, "gen_linkscript: out of memory\n");
            exit(1);
        }
        o->data = nd;
        o->cap = nc;
    }
    memcpy(o->data + o->len, s, n);
    o->len += n;
}

static int is_name_char(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_';
}

/* creates the folder of file (and its missing parents) */
static void make_parent_dirs(const char* file)
{
    char tmp[MAX_PATH];
    size_t n = strlen(file);
    if (n >= sizeof tmp)
    {
        fprintf(stderr, "gen_linkscript: path too long: %s\n", file);
        exit(1);
    }
    memcpy(tmp, file, n + 1);
    for (size_t i = 1; i < n; i++)
    {
        if (tmp[i] == '\\' || tmp[i] == '/')
        {
            char c = tmp[i];
            tmp[i] = 0;
            if (!(i == 2 && tmp[1] == ':'))
            {
                CreateDirectoryA(tmp, NULL);
            }
            tmp[i] = c;
        }
    }
}

/* the value of name: a KEY=VALUE argument first, else the sdkroot.ini key; NULL when neither has it */
static const char* lookup(const char* name, size_t nlen, int argc, char** argv, char* inival, size_t inisz, int* from_ini)
{
    *from_ini = 0;
    for (int i = 3; i < argc; i++)
    {
        const char* eq = strchr(argv[i], '=');
        if (eq && (size_t)(eq - argv[i]) == nlen && memcmp(argv[i], name, nlen) == 0)
        {
            return eq + 1;
        }
    }
    char key[128];
    if (nlen >= sizeof key)
    {
        return NULL;
    }
    memcpy(key, name, nlen);
    key[nlen] = 0;
    if (sdkroot_get_optional(key, inival, inisz))
    {
        *from_ini = 1;
        return inival;
    }
    return NULL;
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: gen_linkscript TEMPLATE OUT [KEY=VALUE ...]\n");
        return 1;
    }
    for (int i = 3; i < argc; i++)
    {
        const char* eq = strchr(argv[i], '=');
        if (!eq || eq == argv[i])
        {
            fprintf(stderr, "gen_linkscript: argument '%s' is not KEY=VALUE\n", argv[i]);
            return 1;
        }
    }

    FILE* f = fopen(argv[1], "rb");
    if (!f)
    {
        fprintf(stderr, "gen_linkscript: cannot open %s\n", argv[1]);
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long tn = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* t = malloc((size_t)tn + 1);
    if (!t || tn < 0 || (tn && fread(t, 1, (size_t)tn, f) != (size_t)tn))
    {
        fprintf(stderr, "gen_linkscript: cannot read %s\n", argv[1]);
        return 1;
    }
    fclose(f);
    size_t tlen = (size_t)tn;

    OUTBUF o = {NULL, 0, 0};
    int errors = 0;
    unsigned line = 1;
    size_t i = 0;
    while (i < tlen)
    {
        if (t[i] == '@')
        {
            size_t j = i + 1;
            while (j < tlen && is_name_char(t[j]))
            {
                j++;
            }
            if (j > i + 1 && j < tlen && t[j] == '@')
            {
                char inival[1024];
                int from_ini;
                const char* v = lookup(t + i + 1, j - i - 1, argc, argv, inival, sizeof inival, &from_ini);
                if (!v)
                {
                    fprintf(stderr, "gen_linkscript: %s(%u): placeholder @%.*s@ is neither a KEY=VALUE argument nor a tools\\sdkroot.ini key\n",
                            argv[1], line, (int)(j - i - 1), t + i + 1);
                    errors++;
                }
                else if (from_ini && strchr(v, ' '))
                {
                    fprintf(stderr, "gen_linkscript: %s(%u): sdkroot.ini value of %.*s contains a space (%s); PSYLINK cannot take spaces in a path, move the SDK to a path without one\n",
                            argv[1], line, (int)(j - i - 1), t + i + 1, v);
                    errors++;
                }
                else
                {
                    out_add(&o, v, strlen(v));
                }
                i = j + 1;
                continue;
            }
        }
        if (t[i] == '\n')
        {
            line++;
        }
        out_add(&o, t + i, 1);
        i++;
    }
    if (errors)
    {
        return 1;
    }

    make_parent_dirs(argv[2]);
    f = fopen(argv[2], "wb");
    if (!f)
    {
        fprintf(stderr, "gen_linkscript: cannot write %s\n", argv[2]);
        return 1;
    }
    if (o.len && fwrite(o.data, 1, o.len, f) != o.len)
    {
        fprintf(stderr, "gen_linkscript: short write %s\n", argv[2]);
        fclose(f);
        return 1;
    }
    if (fclose(f) != 0)
    {
        fprintf(stderr, "gen_linkscript: cannot close %s\n", argv[2]);
        return 1;
    }
    return 0;
}
