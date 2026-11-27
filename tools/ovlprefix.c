/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  tools\ovlprefix.c
 */

/*
    Gives the global names of one overlay's PsyQ (LNK) objects a program prefix, so that all 13 programs can be
    linked by one PSYLINK run (makefile.link). The overlays are separate programs that load at the same address:
    the stage overlays define the same global names (zombie, hunter, crow, ... code), and the SDK startup object
    NONE2.OBJ they all link defines __SN_ENTRY_POINT, __main and stup0..2 and calls main. PSYLINK has one global
    name space per run, so every overlay's names must be told apart. The game source keeps its names; only these
    link inputs are renamed, and a name is never part of the executable.

    usage: ovlprefix NAME OUTDIR IN.obj... [--plain IN.obj...]

    Every global name that an object of the list defines (XDEF and COMMON records) becomes NAME_<name> in all
    objects of the list, both where it is defined and where it is referenced. A name the list only references
    (g_FixedWorkC3000 and the other names the link script defines) is left alone. The objects after --plain
    (the SDK startup object) also get their section names prefixed, .text -> NAME.text, the form ASPSX -sNAME
    gives the project's own objects. Each result is written to OUTDIR under the input's file name.

    A malformed or unsupported object is reported on stderr and gives exit code 1.
*/
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef unsigned char u8;

#define GROUP_ID 20951 /* an id no record of NONE2.OBJ uses (its sections are 20952..20957) */

typedef struct
{
    u8* d;
    size_t n;
} BYTES;

typedef struct
{
    u8* d;
    size_t n, cap;
} BUF;

static void die(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "ovlprefix: ");
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
    exit(1);
}

static void buf_add(BUF* b, const void* p, size_t n)
{
    if (b->n + n > b->cap)
    {
        b->cap = (b->n + n) * 2 + 64;
        b->d = realloc(b->d, b->cap);
        if (!b->d)
        {
            die("out of memory");
        }
    }
    memcpy(b->d + b->n, p, n);
    b->n += n;
}

static BYTES read_file(const char* path)
{
    FILE* f = fopen(path, "rb");
    BYTES r = {NULL, 0};
    long n;
    if (!f)
    {
        die("%s: cannot read", path);
    }
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (n < 0 || !(r.d = malloc((size_t)n + 1)) || fread(r.d, 1, (size_t)n, f) != (size_t)n)
    {
        die("%s: cannot read", path);
    }
    fclose(f);
    r.n = (size_t)n;
    return r;
}

/* the byte length of the expression at d[p..], which must lie inside len */
static size_t expr_len(const u8* d, size_t p, size_t len, int depth)
{
    if (p >= len || depth > 64)
    {
        die("truncated or too deeply nested expression");
    }
    u8 op = d[p];
    if (op == 0x00)
    {
        return 5;
    }
    if (op == 0x02 || op == 0x04 || op == 0x06 || op == 0x08 || op == 0x0A || op == 0x0C || op == 0x16)
    {
        return 3;
    }
    size_t a = expr_len(d, p + 1, len, depth + 1);
    size_t b = expr_len(d, p + 1 + a, len, depth + 1);
    return 1 + a + b;
}

/*
    The total length of the record at d[p] (opcode included) and, for a record that carries a name, the offset of the
    name's length byte from the start of the record (0 when it has none). The layouts are those of sdklibbuild.c's
    xdefs_of, plus the group record 0x14 that ASPSX -sNAME writes.
*/
static size_t rec_info(const u8* d, size_t p, size_t len, size_t* name_at)
{
    size_t n, tot;
    *name_at = 0;
    if (p >= len)
    {
        die("truncated object");
    }
    switch (d[p])
    {
    case 0x00:
        return 1;
    case 0x02:
        if (p + 3 > len)
        {
            die("truncated object");
        }
        tot = 3 + ((size_t)d[p + 1] | ((size_t)d[p + 2] << 8));
        break;
    case 0x04:
        tot = 4;
        break;
    case 0x06:
        tot = 3;
        break;
    case 0x08:
        tot = 5;
        break;
    case 0x0A:
        tot = 4 + expr_len(d, p + 4, len, 0);
        break;
    case 0x0C:
    case 0x30:
        *name_at = 9;
        goto named;
    case 0x0E:
        *name_at = 3;
        goto named;
    case 0x10:
        *name_at = 6;
        goto named;
    case 0x12:
        *name_at = 7;
        goto named;
    case 0x14:
        *name_at = 4;
        goto named;
    case 0x1C:
        *name_at = 3;
    named:
        if (p + *name_at >= len)
        {
            die("truncated object");
        }
        n = d[p + *name_at];
        tot = *name_at + 1 + n;
        break;
    case 0x2E:
        tot = 2;
        break;
    case 0x2C:
        tot = 4;
        break;
    case 0x32:
        tot = 1;
        break;
    default:
        die("unknown LNK record 0x%02X", d[p]);
        return 0;
    }
    if (p + tot > len)
    {
        die("record runs past the end of the object");
    }
    return tot;
}

/* ---- the set of names defined by the overlay ---- */
static char** names;
static size_t nnames, capnames;

static int in_set(const char* s, size_t n)
{
    for (size_t i = 0; i < nnames; i++)
    {
        if (strlen(names[i]) == n && memcmp(names[i], s, n) == 0)
        {
            return 1;
        }
    }
    return 0;
}

static void add_name(const u8* s, size_t n)
{
    if (in_set((const char*)s, n))
    {
        return;
    }
    if (nnames == capnames)
    {
        capnames = capnames ? capnames * 2 : 256;
        names = realloc(names, capnames * sizeof *names);
        if (!names)
        {
            die("out of memory");
        }
    }
    names[nnames] = malloc(n + 1);
    memcpy(names[nnames], s, n);
    names[nnames][n] = 0;
    nnames++;
}

static void collect(BYTES o, const char* path)
{
    if (o.n < 4 || memcmp(o.d, "LNK\x02", 4) != 0)
    {
        die("%s: not an LNK object", path);
    }
    size_t p = 4;
    for (;;)
    {
        size_t at, len = rec_info(o.d, p, o.n, &at);
        if (o.d[p] == 0x00)
        {
            break;
        }
        if (o.d[p] == 0x0C || o.d[p] == 0x30)
        {
            add_name(o.d + p + at + 1, o.d[p + at]);
        }
        p += len;
    }
}

static void emit_named(BUF* out, const u8* rec, size_t at, const char* prefix)
{
    size_t pl = strlen(prefix), n = rec[at];
    if (n + pl > 255)
    {
        die("name too long after prefixing: %.*s", (int)n, rec + at + 1);
    }
    buf_add(out, rec, at);
    u8 l = (u8)(n + pl);
    buf_add(out, &l, 1);
    buf_add(out, prefix, pl);
    buf_add(out, rec + at + 1, n);
}

static void rewrite(BYTES o, const char* path, const char* outdir, const char* sym_prefix, const char* sec_prefix, int plain)
{
    BUF out = {NULL, 0, 0};
    size_t p = 4;
    int grouped = 0;
    buf_add(&out, o.d, 4);
    for (;;)
    {
        size_t at, len = rec_info(o.d, p, o.n, &at);
        u8 op = o.d[p];
        if ((op == 0x0C || op == 0x30 || op == 0x0E) && in_set((const char*)o.d + p + at + 1, o.d[p + at]))
        {
            emit_named(&out, o.d + p, at, sym_prefix);
        }
        else if (op == 0x10 && plain)
        {
            /* the form ASPSX -sNAME gives: one group record named NAME (before the first section) and sections that belong to it */
            if (!grouped)
            {
                size_t gl = strlen(sec_prefix);
                u8 g[5] = {0x14, (u8)(GROUP_ID & 0xFF), (u8)(GROUP_ID >> 8), 0x00, (u8)gl};
                buf_add(&out, g, sizeof g);
                buf_add(&out, sec_prefix, gl);
                grouped = 1;
            }
            u8 rec[300]; /* a section record is at most 7 + 255 bytes */
            memcpy(rec, o.d + p, len);
            rec[3] = (u8)(GROUP_ID & 0xFF);
            rec[4] = (u8)(GROUP_ID >> 8);
            emit_named(&out, rec, at, sec_prefix);
        }
        else
        {
            buf_add(&out, o.d + p, len);
        }
        p += len;
        if (op == 0x00)
        {
            break;
        }
    }
    const char* base = strrchr(path, '\\');
    const char* base2 = strrchr(path, '/');
    if (base2 > base)
    {
        base = base2;
    }
    base = base ? base + 1 : path;
    char dst[MAX_PATH];
    if (snprintf(dst, sizeof dst, "%s\\%s", outdir, base) >= (int)sizeof dst)
    {
        die("path too long: %s", outdir);
    }
    for (size_t i = 1; dst[i]; i++)
    {
        if ((dst[i] == '\\' || dst[i] == '/') && dst[i - 1] != ':')
        {
            char c = dst[i];
            dst[i] = 0;
            CreateDirectoryA(dst, NULL);
            dst[i] = c;
        }
    }
    FILE* f = fopen(dst, "wb");
    if (!f || fwrite(out.d, 1, out.n, f) != out.n || fclose(f) != 0)
    {
        die("%s: cannot write", dst);
    }
}

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        fprintf(stderr, "usage: ovlprefix NAME OUTDIR IN.obj... [--plain IN.obj...]\n");
        return 1;
    }
    char sym[130], sec[130];
    snprintf(sym, sizeof sym, "%s_", argv[1]);
    snprintf(sec, sizeof sec, "%s", argv[1]); /* the section names already start with a dot */
    int nin = argc - 3;
    BYTES* objs = calloc((size_t)nin, sizeof *objs);
    int* plain = calloc((size_t)nin, sizeof *plain);
    int is_plain = 0, k = 0;
    for (int a = 3; a < argc; a++)
    {
        if (strcmp(argv[a], "--plain") == 0)
        {
            is_plain = 1;
            continue;
        }
        objs[k] = read_file(argv[a]);
        plain[k] = is_plain;
        collect(objs[k], argv[a]);
        argv[3 + k] = argv[a];
        k++;
    }
    for (int i = 0; i < k; i++)
    {
        rewrite(objs[i], argv[3 + i], argv[2], sym, sec, plain[i]);
    }
    printf("ovlprefix: %s: %d objects, %u global names prefixed\n", argv[1], k, (unsigned)nnames);
    return 0;
}
