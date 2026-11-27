/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog2\LOGO.H
 */

#ifndef LOGO_H
#define LOGO_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

typedef struct LOGO_WORK LOGO_WORK;

struct LOGO_WORK
{
    USHORT Movie_x; // VRAM x the movie frames are drawn at
    SCHAR skipped;  // 1 when a button press ended the movie early
};

/*
    overlay entry point: play the company logo movie until it ends or a button is pressed,
    clear the screen, stop the sound and chain to the next program
 */
VOID main(VOID);

#endif
