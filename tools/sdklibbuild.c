/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  tools\sdklibbuild.c
 */

/*
    Composes the two custom PSY-Q libraries the USA retail executables are linked with, without python.

    LIBPAD.LIB: PsyQ 4.2's LIBPAD with
      * the members PADMAIN and PADIF replaced, at the position of the stock member (so PSYLINK takes the
        members in the same order), by the objects given on the command line (the later 4.2.1 patch
        revisions, kept as assembly source in Bio1\lib and assembled by the makefile with ASPSX -G0);
        the exported-name list of each replaced entry is recomputed from the new object;
      * WAITRC2 taken from PsyQ 4.3's LIBPAD (setRC2wait / chkRC2wait differ from 4.2's) and put at the END of
        the archive, as PSYLIB /u does.
      PADPORTD stays the stock 4.2 member.

    LIBGS.LIB: PsyQ 3.7's LIBGS with the members GS_131, GS_133 and GS_134 replaced in place by the PsyQ 4.0
      versions, which do not define the commons GsLSMATRIX, GsWSMATRIX and GsWSMATRIX_ORG (retail has no
      storage for them).

    The SDK locations come from tools\sdkroot.ini (PSYQ_3_7, PSYQ_4_0, PSYQ_4_2, PSYQ_4_3); nothing else is
    hardcoded. PsyQ 4.2's archive is LIB\LIBPAD.LIB, 3.7's is LIB\LIBGS.LIB, the 4.0 and 4.3 ones are
    PSX\LIB\LIBGS.LIB and PSX\LIB\LIBPAD.LIB.

    PSY-Q LIB\x01 archive format:
      4 bytes  : "LIB\x01"
      per entry, repeated to the tail:
        8 bytes  : name, space-padded ASCII
        4 bytes  : date (LE u32, copied through unchanged, never recomputed)
        4 bytes  : objoff (LE u32), offset from the start of this entry to its LNK object
        4 bytes  : size (LE u32), the entry's total length (name field to the end of the object)
        then the length-prefixed exported (XDEF) names, a 0 byte, and the LNK\x02 object
      then the tail (a single 0 byte in the stock archives).
    The exported names of a replaced member are found by walking the object's records (opcode 0x0C, XDEF). An
    object that does not start with the LNK\x02 signature, or is cut short, is an error (never a silent
    truncation).

    The tool also re-packs each untouched stock archive and warns if the result is not byte-identical to the
    file it was read from (that would mean the entry parse above does not hold for the archive).

    usage: sdklibbuild OUTDIR PADMAIN.OBJ PADIF.OBJ
    Writes OUTDIR\LIBPAD.LIB and OUTDIR\LIBGS.LIB (OUTDIR is created if missing; relative arguments are relative
    to the current directory) and prints one summary line per library. Exit code 0 on success, 1 on any error;
    a failed round-trip check is only a warning.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include "sdkroot.h"

typedef uint8_t u8;
typedef uint32_t u32;

typedef struct
{
    const u8* data;
    size_t len;
} BUF;

typedef struct
{
    char name[16];      /* trimmed member name */
    u32 date;
    const u8* syms;     /* length-prefixed exported names, without the terminating 0 */
    size_t symlen;
    const u8* obj;
    size_t objlen;
} ENTRY;

typedef struct
{
    ENTRY* e;
    size_t n, cap;
    BUF tail;
} LIB;

static void die(const char* fmt, ...)
{
    va_list ap;
    fprintf(stderr, "sdklibbuild: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    exit(1);
}

static void* xmalloc(size_t n)
{
    void* p = malloc(n ? n : 1);
    if (!p)
    {
        die("out of memory");
    }
    return p;
}

static BUF read_file(const char* path)
{
    FILE* f = fopen(path, "rb");
    if (!f)
    {
        die("cannot open %s", path);
    }
    fseek(f, 0, SEEK_END);
    long n = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (n < 0)
    {
        die("cannot size %s", path);
    }
    u8* b = xmalloc((size_t)n);
    if (n && fread(b, 1, (size_t)n, f) != (size_t)n)
    {
        die("short read %s", path);
    }
    fclose(f);
    BUF r = {b, (size_t)n};
    return r;
}

static void write_file(const char* path, const u8* data, size_t len)
{
    FILE* f = fopen(path, "wb");
    if (!f)
    {
        die("cannot write %s", path);
    }
    if (len && fwrite(data, 1, len, f) != len)
    {
        die("short write %s", path);
    }
    if (fclose(f) != 0)
    {
        die("cannot close %s", path);
    }
}

/* creates dir and every missing parent */
static void make_dirs(const char* dir)
{
    char tmp[MAX_PATH];
    size_t n = strlen(dir);
    if (n == 0 || n >= sizeof tmp)
    {
        die("bad output directory '%s'", dir);
    }
    memcpy(tmp, dir, n + 1);
    for (size_t i = 1; i <= n; i++)
    {
        if (tmp[i] == '\\' || tmp[i] == '/' || tmp[i] == 0)
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
    DWORD a = GetFileAttributesA(dir);
    if (a == INVALID_FILE_ATTRIBUTES || !(a & FILE_ATTRIBUTE_DIRECTORY))
    {
        die("cannot create directory %s", dir);
    }
}

static u32 get_u32(const u8* p)
{
    return (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24);
}

static void put_u32(u8* p, u32 v)
{
    p[0] = (u8)v;
    p[1] = (u8)(v >> 8);
    p[2] = (u8)(v >> 16);
    p[3] = (u8)(v >> 24);
}

/* ---------------- LIB\x01 archive ---------------- */

static void lib_add(LIB* lib, const ENTRY* e)
{
    if (lib->n == lib->cap)
    {
        lib->cap = lib->cap ? lib->cap * 2 : 64;
        ENTRY* ne = xmalloc(lib->cap * sizeof(ENTRY));
        if (lib->n)
        {
            memcpy(ne, lib->e, lib->n * sizeof(ENTRY));
        }
        free(lib->e);
        lib->e = ne;
    }
    lib->e[lib->n++] = *e;
}

static void lib_read(LIB* lib, BUF d, const char* path)
{
    memset(lib, 0, sizeof *lib);
    if (d.len < 4 || memcmp(d.data, "LIB\x01", 4) != 0)
    {
        die("%s is not a PsyQ library", path);
    }
    size_t p = 4;
    while (p + 20 < d.len)
    {
        ENTRY e;
        memset(&e, 0, sizeof e);
        memcpy(e.name, d.data + p, 8);
        size_t nl = 8;
        while (nl > 0 && (e.name[nl - 1] == ' ' || e.name[nl - 1] == 0))
        {
            nl--;
        }
        e.name[nl] = 0;
        size_t lead = 0;
        while (lead < nl && e.name[lead] == ' ')
        {
            lead++;
        }
        memmove(e.name, e.name + lead, nl - lead + 1);
        e.date = get_u32(d.data + p + 8);
        u32 off = get_u32(d.data + p + 12);
        u32 size = get_u32(d.data + p + 16);
        size_t q = p + 20;
        while (q < d.len && d.data[q])
        {
            q += 1 + (size_t)d.data[q];
        }
        if (q >= d.len)
        {
            die("%s: entry %s: name list runs past the end of the file", path, e.name);
        }
        if (q + 1 != p + off)
        {
            die("%s: entry %s: unexpected object offset", path, e.name);
        }
        if (size < off || p + size > d.len)
        {
            die("%s: entry %s: bad entry size", path, e.name);
        }
        e.syms = d.data + p + 20;
        e.symlen = q - (p + 20);
        e.obj = d.data + p + off;
        e.objlen = size - off;
        lib_add(lib, &e);
        p += size;
    }
    lib->tail.data = d.data + p;
    lib->tail.len = d.len - p;
}

/* serialises the archive; the caller owns the buffer */
static BUF lib_write(const LIB* lib)
{
    size_t total = 4 + lib->tail.len;
    for (size_t i = 0; i < lib->n; i++)
    {
        total += 20 + lib->e[i].symlen + 1 + lib->e[i].objlen;
    }
    u8* out = xmalloc(total);
    u8* p = out;
    memcpy(p, "LIB\x01", 4);
    p += 4;
    for (size_t i = 0; i < lib->n; i++)
    {
        const ENTRY* e = &lib->e[i];
        u32 off = (u32)(20 + e->symlen + 1);
        memset(p, ' ', 8);
        memcpy(p, e->name, strlen(e->name) > 8 ? 8 : strlen(e->name));
        put_u32(p + 8, e->date);
        put_u32(p + 12, off);
        put_u32(p + 16, off + (u32)e->objlen);
        p += 20;
        memcpy(p, e->syms, e->symlen);
        p += e->symlen;
        *p++ = 0;
        memcpy(p, e->obj, e->objlen);
        p += e->objlen;
    }
    memcpy(p, lib->tail.data, lib->tail.len);
    p += lib->tail.len;
    BUF r = {out, (size_t)(p - out)};
    return r;
}

/* the last entry called name (a name seen twice resolves to the later one), or NULL */
static ENTRY* lib_find(const LIB* lib, const char* name)
{
    ENTRY* hit = NULL;
    for (size_t i = 0; i < lib->n; i++)
    {
        if (strcmp(lib->e[i].name, name) == 0)
        {
            hit = &lib->e[i];
        }
    }
    return hit;
}

/* re-packs the stock archive and compares with the bytes it was read from: 1 when identical */
static int lib_roundtrip(const LIB* lib, BUF orig)
{
    BUF b = lib_write(lib);
    int same = b.len == orig.len && memcmp(b.data, orig.data, b.len) == 0;
    free((void*)b.data);
    return same;
}

/* ---------------- LNK\x02 object: exported names ---------------- */

/* the byte length of the expression at d[p..], which must lie inside len */
static size_t expr_len(const u8* d, size_t p, size_t len, int depth)
{
    if (p >= len || depth > 64)
    {
        die("truncated or too deeply nested expression in an object");
    }
    u8 op = d[p];
    if (op == 0x00)
    {
        return 5;
    }
    /* psx_sdklib.py's skip_expr takes 02 04 06 08 0A as 2-byte operands; the LNK format's operands are 02 04 0C 16 */
    if (op == 0x02 || op == 0x04 || op == 0x06 || op == 0x08 || op == 0x0A || op == 0x0C || op == 0x16)
    {
        return 3;
    }
    size_t a = expr_len(d, p + 1, len, depth + 1);
    size_t b = expr_len(d, p + 1 + a, len, depth + 1);
    return 1 + a + b;
}

/* a record byte that must exist */
static u8 need(const u8* d, size_t p, size_t len)
{
    if (p >= len)
    {
        die("truncated record in an object");
    }
    return d[p];
}

/*
    The XDEF records (opcode 0x0C) of an LNK object, as the length-prefixed name list an archive entry carries
    (without the terminating 0). Records are skipped by their layout, the same walk psx_sdklib.py's xdefs_of does.
*/
static BUF xdefs_of(const u8* obj, size_t len)
{
    if (len < 4 || memcmp(obj, "LNK\x02", 4) != 0)
    {
        die("not an LNK object (missing the LNK\\x02 signature; did the assembler fail?)");
    }
    u8* out = xmalloc(len + 1);
    size_t o = 0;
    size_t p = 4;
    int ended = 0;
    while (p < len)
    {
        u8 t = obj[p++];
        if (t == 0x00)
        {
            ended = 1;
            break;
        }
        switch (t)
        {
        case 0x02:
            need(obj, p + 1, len);
            p += 2 + ((size_t)obj[p] | ((size_t)obj[p + 1] << 8));
            break;
        case 0x04:
            p += 3;
            break;
        case 0x06:
            p += 2;
            break;
        case 0x08:
            p += 4;
            break;
        case 0x0A:
            p += 3;
            p += expr_len(obj, p, len, 0);
            break;
        case 0x0C:
        {
            size_t n = need(obj, p + 8, len);
            need(obj, p + 8 + n, len);
            out[o++] = (u8)n;
            memcpy(out + o, obj + p + 9, n);
            o += n;
            p += 9 + n;
            break;
        }
        case 0x0E:
            p += 3 + need(obj, p + 2, len);
            break;
        case 0x10:
            p += 6 + need(obj, p + 5, len);
            break;
        case 0x12:
            p += 7 + need(obj, p + 6, len);
            break;
        case 0x1C:
            p += 3 + need(obj, p + 2, len);
            break;
        case 0x2E:
            p += 1;
            break;
        case 0x2C:
            p += 3;
            break;
        case 0x30:
            p += 9 + need(obj, p + 8, len);
            break;
        case 0x32:
            break;
        default:
            die("unknown LNK record 0x%02X", t);
        }
    }
    if (!ended || p > len)
    {
        die("the object is truncated (no end record, or the last record runs past its end)");
    }
    BUF r = {out, o};
    return r;
}

/* ---------------- the two libraries ---------------- */

static char* sdk_path(const char* key, const char* suffix)
{
    char root[MAX_PATH];
    sdkroot_get(key, root, sizeof root);
    size_t n = strlen(root) + strlen(suffix) + 1;
    char* p = xmalloc(n);
    snprintf(p, n, "%s%s", root, suffix);
    return p;
}

static void print_members(const LIB* lib)
{
    for (size_t i = 0; i < lib->n; i++)
    {
        printf("%s%s", i ? " " : "", lib->e[i].name);
    }
}

static void replace_member(LIB* lib, const char* name, const u8* obj, size_t objlen)
{
    ENTRY* e = lib_find(lib, name);
    if (!e)
    {
        die("member %s not found in the stock archive", name);
    }
    BUF xd = xdefs_of(obj, objlen);
    e->obj = obj;
    e->objlen = objlen;
    e->syms = xd.data;
    e->symlen = xd.len;
}

static void build_libpad(const char* outdir, const char* padmain, const char* padif)
{
    char* p42 = sdk_path("PSYQ_4_2", "\\LIB\\LIBPAD.LIB");
    char* p43 = sdk_path("PSYQ_4_3", "\\PSX\\LIB\\LIBPAD.LIB");
    BUF d42 = read_file(p42);
    BUF d43 = read_file(p43);
    LIB l42, l43, out;
    lib_read(&l42, d42, p42);
    lib_read(&l43, d43, p43);
    int ok = lib_roundtrip(&l42, d42);

    BUF m = read_file(padmain);
    BUF i = read_file(padif);
    replace_member(&l42, "PADMAIN", m.data, m.len);
    replace_member(&l42, "PADIF", i.data, i.len);

    ENTRY* w = lib_find(&l43, "WAITRC2");
    if (!w)
    {
        die("member WAITRC2 not found in %s", p43);
    }
    ENTRY waitrc2 = *w;

    memset(&out, 0, sizeof out);
    out.tail = l42.tail;
    for (size_t k = 0; k < l42.n; k++)
    {
        if (strcmp(l42.e[k].name, "WAITRC2") != 0)
        {
            lib_add(&out, &l42.e[k]);
        }
    }
    lib_add(&out, &waitrc2);

    BUF b = lib_write(&out);
    char path[MAX_PATH];
    snprintf(path, sizeof path, "%s\\LIBPAD.LIB", outdir);
    write_file(path, b.data, b.len);
    printf("%s (%zu bytes), members: ", path, b.len);
    print_members(&out);
    printf("  (round-trip of the stock archive: %s)\n", ok ? "identical" : "DIFFERS");
    if (!ok)
    {
        fprintf(stderr, "sdklibbuild: warning: re-packing the stock %s does not reproduce it byte for byte\n", p42);
    }
    free(p42);
    free(p43);
}

static void build_libgs(const char* outdir)
{
    char* p37 = sdk_path("PSYQ_3_7", "\\LIB\\LIBGS.LIB");
    char* p40 = sdk_path("PSYQ_4_0", "\\PSX\\LIB\\LIBGS.LIB");
    BUF d37 = read_file(p37);
    BUF d40 = read_file(p40);
    LIB l37, l40;
    lib_read(&l37, d37, p37);
    lib_read(&l40, d40, p40);
    int ok = lib_roundtrip(&l37, d37);

    static const char* const names[] = {"GS_131", "GS_133", "GS_134"};
    for (size_t k = 0; k < sizeof names / sizeof names[0]; k++)
    {
        ENTRY* n = lib_find(&l40, names[k]);
        if (!n)
        {
            die("member %s not found in %s", names[k], p40);
        }
        for (size_t j = 0; j < l37.n; j++)
        {
            if (strcmp(l37.e[j].name, names[k]) == 0)
            {
                l37.e[j].obj = n->obj;
                l37.e[j].objlen = n->objlen;
                l37.e[j].syms = n->syms;
                l37.e[j].symlen = n->symlen;
            }
        }
    }

    BUF b = lib_write(&l37);
    char path[MAX_PATH];
    snprintf(path, sizeof path, "%s\\LIBGS.LIB", outdir);
    write_file(path, b.data, b.len);
    printf("%s (%zu bytes), %zu members  (round-trip of the stock archive: %s)\n", path, b.len, l37.n, ok ? "identical" : "DIFFERS");
    if (!ok)
    {
        fprintf(stderr, "sdklibbuild: warning: re-packing the stock %s does not reproduce it byte for byte\n", p37);
    }
    free(p37);
    free(p40);
}

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "usage: sdklibbuild OUTDIR PADMAIN.OBJ PADIF.OBJ\n");
        return 1;
    }
    size_t n = strlen(argv[1]);
    char outdir[MAX_PATH];
    if (n == 0 || n + 16 >= sizeof outdir)     /* room for "\LIBPAD.LIB" and the terminator */
    {
        die("bad output directory");
    }
    memcpy(outdir, argv[1], n + 1);
    while (n > 1 && (outdir[n - 1] == '\\' || outdir[n - 1] == '/') && outdir[n - 2] != ':')
    {
        outdir[--n] = 0;
    }
    make_dirs(outdir);
    build_libpad(outdir, argv[2], argv[3]);
    build_libgs(outdir);
    return 0;
}
