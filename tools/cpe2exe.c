/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  tools\cpe2exe.c
 */

/*
    Converts a PSYLINK 2.52 CPE file into a PS-X EXE (what PsyQ's 16-bit CPE2X.EXE does; that program does not
    run on a 64-bit Windows). Used by the makefile for SLUS_007.47 and the 12 PROG2 overlay executables.

    usage: cpe2exe IN.CPE OUT.EXE [--area J|A|E] [--end HEX]
           cpe2exe --bin IN.BIN OUT.EXE --group NAME --pc-symbol NAME --map FILE [--bss NAME] [--area J|A|E]
      --area  region string of the header: J Japan, A North America (default), E Europe
      --end   ignore load data at or above this address (hex)
      --bin   IN is not a CPE but the raw image of one PSYLINK group (its file("...") output, which the one-run link of
              makefile.link writes for each overlay). The link map FILE gives the load address (the object address of
              the group --group) and the start address (the address of the symbol --pc-symbol). --bss NAME: the image ends
              where the map's sections NAME.sbss and NAME.bss begin (the first one listed), since the executable does
              not store the uninitialised data (the startup code clears it at run time)

    CPE layout as produced by PSYLINK 2.52 (little endian):
      "CPE" 0x01                               magic + version
      then chunks, each a u8 id followed by its body, ended by id 0x00:
        0x01 LOAD    u32 address, u32 size, size bytes          one per object section, relocations already applied
        0x03 SETREG  u16 register, u32 value                    register id = 4*N for rN, 0x90 = pc
        0x08 UNIT    u8 unit number
    PS-X EXE written (the layout CPE2X 1.5 writes):
      0x000 "PS-X EXE"   0x010 pc0   0x014 gp0 (0)   0x018 t_addr   0x01C t_size   0x020..0x02F d/b address+size (0)
      0x030 s_addr 0x801FFFF0   0x034 s_size 0   0x04C "Sony Computer Entertainment Inc. for <region> area"
      0x800 payload: the image from t_addr (the lowest load address), gaps zero filled, t_size rounded up to 0x800

    A malformed CPE (bad magic, truncated chunk, unknown chunk id, no LOAD chunk) is reported on stderr and gives
    exit code 1; no output file is written then.
*/
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long long u64;

#define HDR_SIZE 0x800u
#define REG_PC 0x90u

typedef struct
{
    u32 addr;
    u32 size;
    const u8* data;
} LOAD;

static void die(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputc('\n', stderr);
    exit(1);
}

static u32 rd16(const u8* p)
{
    return (u32)p[0] | ((u32)p[1] << 8);
}

static u32 rd32(const u8* p)
{
    return (u32)p[0] | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24);
}

static void put32(u8* p, u32 v)
{
    p[0] = (u8)v;
    p[1] = (u8)(v >> 8);
    p[2] = (u8)(v >> 16);
    p[3] = (u8)(v >> 24);
}

static u8* read_file(const char* path, size_t* len)
{
    FILE* f = fopen(path, "rb");
    long n;
    u8* buf;
    if (!f)
    {
        die("%s: cannot read", path);
    }
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (n < 0)
    {
        die("%s: cannot read", path);
    }
    buf = (u8*)malloc((size_t)n + 1);
    if (!buf || fread(buf, 1, (size_t)n, f) != (size_t)n)
    {
        die("%s: cannot read", path);
    }
    fclose(f);
    *len = (size_t)n;
    return buf;
}

/* the address that a PSYLINK map gives the symbol name (the lines "  80105534  name" of its alphabetical list) */
static u32 map_symbol(const char* map, const char* name)
{
    size_t len, i = 0, n = strlen(name);
    u8* d = read_file(map, &len);
    while (i < len)
    {
        size_t e = i;
        while (e < len && d[e] != '\n')
        {
            e++;
        }
        /* optional blanks, 8 hex digits, blanks, the name, optional blanks */
        size_t p = i;
        while (p < e && d[p] == ' ')
        {
            p++;
        }
        if (e - p >= 8 + 1 + n)
        {
            char* stop;
            char hex[9];
            memcpy(hex, d + p, 8);
            hex[8] = 0;
            unsigned long v = strtoul(hex, &stop, 16);
            size_t q = p + 8;
            if (*stop == 0 && q < e && d[q] == ' ')
            {
                while (q < e && d[q] == ' ')
                {
                    q++;
                }
                size_t r = e;
                while (r > q && (d[r - 1] == '\r' || d[r - 1] == ' '))
                {
                    r--;
                }
                if (r - q == n && memcmp(d + q, name, n) == 0)
                {
                    free(d);
                    return (u32)v;
                }
            }
        }
        i = e + 1;
    }
    die("%s: no symbol %s", map, name);
    return 0;
}

/*
    The object address of the first line of the section table of a PSYLINK map whose group is name (by_group) or whose
    section is name (not by_group); a line is "Start Stop Length Obj Group Section". 0 when there is none (die when must).
*/
static u32 map_line(const char* map, const char* name, int by_group, int must)
{
    size_t len, i = 0;
    u8* d = read_file(map, &len);
    while (i < len)
    {
        size_t e = i;
        while (e < len && d[e] != '\n')
        {
            e++;
        }
        char line[512], grp[128], sec[128];
        unsigned start, stop, size, obj;
        size_t n = e - i < sizeof line - 1 ? e - i : sizeof line - 1;
        memcpy(line, d + i, n);
        line[n] = 0;
        if (sscanf(line, " %8x %8x %8x %8x %127s %127s", &start, &stop, &size, &obj, grp, sec) == 6 && strcmp(by_group ? grp : sec, name) == 0)
        {
            free(d);
            return (u32)obj;
        }
        i = e + 1;
    }
    free(d);
    if (must)
    {
        die("%s: no %s %s", map, by_group ? "section of group" : "section", name);
    }
    return 0;
}

int main(int argc, char** argv)
{
    int bin = 0;
    const char* bss = NULL;
    const char* group = NULL;
    const char* pc_symbol = NULL;
    const char* map_file = NULL;
    const char* in = NULL;
    const char* out = NULL;
    const char* region = "North America";
    u64 end = 0;
    int have_end = 0;
    size_t len;
    u8* d;
    LOAD* loads = NULL;
    size_t nloads = 0, cap = 0, pos, i;
    u32 pc0 = 0;
    u64 lo, hi, size;
    u8* file;
    u8* image;
    char text[96];
    FILE* f;
    int a;

    for (a = 1; a < argc; a++)
    {
        if (strcmp(argv[a], "--area") == 0 && a + 1 < argc)
        {
            const char* v = argv[++a];
            if (strcmp(v, "J") == 0)
            {
                region = "Japan";
            }
            else if (strcmp(v, "A") == 0)
            {
                region = "North America";
            }
            else if (strcmp(v, "E") == 0)
            {
                region = "Europe";
            }
            else
            {
                die("--area must be J, A or E");
            }
        }
        else if (strcmp(argv[a], "--end") == 0 && a + 1 < argc)
        {
            char* stop;
            end = strtoull(argv[++a], &stop, 16);
            if (stop == argv[a] || *stop != 0)
            {
                die("--end needs a hex address, not \"%s\"", argv[a]);
            }
            have_end = 1;
        }
        else if (strcmp(argv[a], "--bin") == 0)
        {
            bin = 1;
        }
        else if (strcmp(argv[a], "--group") == 0 && a + 1 < argc)
        {
            group = argv[++a];
        }
        else if (strcmp(argv[a], "--bss") == 0 && a + 1 < argc)
        {
            bss = argv[++a];
        }
        else if (strcmp(argv[a], "--pc-symbol") == 0 && a + 1 < argc)
        {
            pc_symbol = argv[++a];
        }
        else if (strcmp(argv[a], "--map") == 0 && a + 1 < argc)
        {
            map_file = argv[++a];
        }
        else if (argv[a][0] == '-' && argv[a][1] == '-')
        {
            die("unknown option %s", argv[a]);
        }
        else if (!in)
        {
            in = argv[a];
        }
        else if (!out)
        {
            out = argv[a];
        }
        else
        {
            die("too many arguments");
        }
    }
    if (!in || !out)
    {
        die("usage: cpe2exe IN.CPE OUT.EXE [--area J|A|E] [--end HEX]");
    }

    d = read_file(in, &len);
    if (bin)
    {
        if (!group || !pc_symbol || !map_file)
        {
            die("--bin needs --group, --pc-symbol and --map");
        }
        u64 bin_addr = map_line(map_file, group, 1, 1);
        if (len > 0xFFFFFFFFull - bin_addr)
        {
            die("%s: image does not fit below 4 GiB at %llx", in, bin_addr);
        }
        if (bss)
        {
            char sec[160];
            u32 cut = 0, a1, a2;
            snprintf(sec, sizeof sec, "%s.sbss", bss);
            a1 = map_line(map_file, sec, 0, 0);
            snprintf(sec, sizeof sec, "%s.bss", bss);
            a2 = map_line(map_file, sec, 0, 0);
            cut = a1 && (!a2 || a1 < a2) ? a1 : a2;
            if (cut && cut >= bin_addr && cut - bin_addr < len)
            {
                len = cut - bin_addr;
            }
        }
        loads = (LOAD*)malloc(sizeof(LOAD));
        if (!loads)
        {
            die("out of memory");
        }
        loads[0].addr = (u32)bin_addr;
        loads[0].size = (u32)len;
        loads[0].data = d;
        nloads = 1;
        pc0 = map_symbol(map_file, pc_symbol);
        pos = len; /* no CPE chunks to read */
    }
    else
    {
        if (len < 3 || memcmp(d, "CPE", 3) != 0)
        {
            die("%s: not a CPE file", in);
        }
        pos = 4;
    }
    while (pos < len)
    {
        u8 chunk = d[pos++];
        if (chunk == 0x00)
        {
            break;
        }
        if (chunk == 0x01)
        {
            u32 addr, sz;
            if (pos + 8 > len)
            {
                die("%s: truncated LOAD chunk header", in);
            }
            addr = rd32(d + pos);
            sz = rd32(d + pos + 4);
            pos += 8;
            if ((u64)pos + sz > len)
            {
                die("%s: truncated LOAD chunk", in);
            }
            if (nloads == cap)
            {
                cap = cap ? cap * 2 : 256;
                loads = (LOAD*)realloc(loads, cap * sizeof(LOAD));
                if (!loads)
                {
                    die("out of memory");
                }
            }
            loads[nloads].addr = addr;
            loads[nloads].size = sz;
            loads[nloads].data = d + pos;
            nloads++;
            pos += sz;
        }
        else if (chunk == 0x03)
        {
            if (pos + 6 > len)
            {
                die("%s: truncated SETREG chunk", in);
            }
            if (rd16(d + pos) == REG_PC)
            {
                pc0 = rd32(d + pos + 2);
            }
            pos += 6;
        }
        else if (chunk == 0x08)
        {
            pos += 1;
        }
        else
        {
            die("unknown CPE chunk id 0x%02x at 0x%x", (unsigned)chunk, (unsigned)(pos - 1));
        }
    }
    if (nloads == 0)
    {
        die("%s: CPE has no LOAD chunks", in);
    }

    lo = loads[0].addr;
    hi = (u64)loads[0].addr + loads[0].size;
    for (i = 1; i < nloads; i++)
    {
        u64 e = (u64)loads[i].addr + loads[i].size;
        if (loads[i].addr < lo)
        {
            lo = loads[i].addr;
        }
        if (e > hi)
        {
            hi = e;
        }
    }
    if (have_end && end < hi)
    {
        hi = end;
    }
    if (hi < lo)
    {
        die("--end %llx is below the lowest load address %08x", end, (unsigned)lo);
    }
    size = (hi - lo + 0x7FF) & ~(u64)0x7FF;
    if (size > 0xFFFFFFFFull - HDR_SIZE)
    {
        die("image too large");
    }

    file = (u8*)calloc((size_t)(HDR_SIZE + size), 1);
    if (!file)
    {
        die("out of memory");
    }
    image = file + HDR_SIZE;
    for (i = 0; i < nloads; i++)
    {
        u64 n = loads[i].size;
        if (loads[i].addr >= lo + size)
        {
            continue;
        }
        if (loads[i].addr + n > lo + size)
        {
            n = lo + size - loads[i].addr;
        }
        memcpy(image + (loads[i].addr - lo), loads[i].data, (size_t)n);
    }

    memcpy(file, "PS-X EXE", 8);
    put32(file + 0x10, pc0);
    put32(file + 0x18, (u32)lo);
    put32(file + 0x1C, (u32)size);
    put32(file + 0x30, 0x801FFFF0u);
    snprintf(text, sizeof text, "Sony Computer Entertainment Inc. for %s area", region);
    memcpy(file + 0x4C, text, strlen(text));

    f = fopen(out, "wb");
    if (!f)
    {
        die("%s: cannot write", out);
    }
    if (fwrite(file, 1, (size_t)(HDR_SIZE + size), f) != (size_t)(HDR_SIZE + size) || fclose(f) != 0)
    {
        remove(out); /* no half written executable left behind */
        die("%s: write error", out);
    }
    printf("%s: pc0 %08X  t_addr %08X  t_size %06X  (%u chunks, %u bytes)\n", out, (unsigned)pc0, (unsigned)lo, (unsigned)size,
           (unsigned)nloads, (unsigned)(HDR_SIZE + size));
    return 0;
}
