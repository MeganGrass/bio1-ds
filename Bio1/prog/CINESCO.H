/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\CINESCO.H
 */

#ifndef CINESCO_H
#define CINESCO_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

struct CINESCO_FADE_REC
{
    UCHAR x0;     // fade entry identifier
    UCHAR active; // set to apply the deltas on the next update, cleared once applied
    UCHAR row;    // CLUT row index in the captured buffer
    SCHAR dr;     // delta added to the red channel
    SCHAR dg;     // delta added to the green channel
    SCHAR db;     // delta added to the blue channel
    USHORT start; // first colour index of the faded range
    USHORT count; // number of colours in the faded range
};

/*
    clear the captured buffer pointers and the row count, and reset all 4 fade entries to no change
    over a full row
 */
VOID Cinesco_fade_clr(VOID);

/*
    when entries are registered, copy the CLUT rows from VRAM into two buffers at the load pointer (an
    untouched source copy and a working copy) and advance the load pointer past them; Row_end less 10 is
    the row count
 */
VOID Cinesco_fade_capture(INT Row_end);

/*
    register the next free fade entry for CLUT row row under identifier id; returns the entry index,
    or -1 when all 4 entries are in use
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
INT Cinesco_fade_add(UINT row, INT id);

/*
    for every active entry, rebuild its colour range in the working copy from the source copy with the
    entry's red, green and blue deltas applied to the 5-bit channels (clamped, black kept), then
    upload the working copy to VRAM
 */
VOID Cinesco_fade_main(VOID);

/*
    add the deltas to the entry with identifier id (each sum clamped to -31..31), set its colour range
    and mark it active
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
VOID Cinesco_fade_sabun_add(INT dr, INT dg, INT db, INT start, INT count, INT id);

/*
    set the deltas and colour range of the entry with identifier id and mark it active
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
VOID Cinesco_fade_set(INT dr, INT dg, INT db, INT start, INT count, INT id);

/*
    zero the deltas of the entry with identifier id, set its colour range and mark it active, restoring
    the source colours
 */
VOID Cinesco_fade_off(UCHAR id, SHORT start, SHORT count);

extern USHORT Cinesco_fade_rows;                 // number of CLUT rows held in each captured buffer
extern CINESCO_FADE_REC Cinesco_fade_tbl[4]; // fade entries: identifier, row, colour deltas and range
extern UINT* pCinesco_fade_src;                    // source copy of the CLUT rows captured from VRAM
extern UINT* pCinesco_fade_dst;                    // working copy of the CLUT rows, uploaded to VRAM after a fade
extern USHORT Cinesco_fade_num;                   // number of fade entries registered

#endif      // CINESCO_H
