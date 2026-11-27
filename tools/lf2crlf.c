/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  tools\lf2crlf.c
 */

/*
    Copies a text file, converting every bare LF to CRLF (an LF that already follows a CR is left alone).

    ASPSX 2.56 rejects a bare LF line ("Illegal character (10)") when it assembles a text file, and the
    project's own sources are LF, UTF-8, no BOM. This tool bridges the two without storing CRLF in any
    source file: the makefile writes the CRLF copy under the build's output directory and ASPSX reads
    that copy, never the checked-in file.

    usage: lf2crlf IN OUT         LF -> CRLF
           lf2crlf --lf IN OUT    CRLF -> LF (the PsyQ headers are CRLF; GCC 2.6.3's preprocessor needs LF)
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    int to_lf = argc == 4 && strcmp(argv[1], "--lf") == 0;
    if (to_lf)
    {
        argv++;
        argc--;
    }
    if (argc != 3)
    {
        fprintf(stderr, "usage: lf2crlf [--lf] IN OUT\n");
        return 1;
    }
    FILE* in = fopen(argv[1], "rb");
    if (!in)
    {
        fprintf(stderr, "lf2crlf: cannot open %s\n", argv[1]);
        return 1;
    }
    FILE* out = fopen(argv[2], "wb");
    if (!out)
    {
        fprintf(stderr, "lf2crlf: cannot write %s\n", argv[2]);
        fclose(in);
        return 1;
    }
    int c, prev = 0;
    while ((c = fgetc(in)) != EOF)
    {
        if (to_lf)
        {
            if (c == '\r')
            {
                prev = c;
                continue;
            }
        }
        else if (c == '\n' && prev != '\r')
        {
            fputc('\r', out);
        }
        fputc(c, out);
        prev = c;
    }
    int read_error = ferror(in);
    fclose(in);
    if (read_error)
    {
        fprintf(stderr, "lf2crlf: read error on %s\n", argv[1]);
        fclose(out);
        return 1;
    }
    int write_error = ferror(out);
    if (fclose(out) != 0 || write_error)
    {
        fprintf(stderr, "lf2crlf: write error on %s\n", argv[2]);
        return 1;
    }
    return 0;
}
