/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog2\PROLOGUE.H
 */

#ifndef PROLOGUE_H
#define PROLOGUE_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

/*
    overlay entry point: clear both display buffers, re-initialise the graphics, request
    playback of the prologue movie (movie 1) and a graphics reset from the main loop, then
    stop the sound and chain to game start
 */
VOID main(VOID);

#endif
