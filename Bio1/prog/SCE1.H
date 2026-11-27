/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SCE1.H
 */

#ifndef SCE1_H
#define SCE1_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBSND.H>

/*
    set bit x of the bit array p; bit 0 is the most significant bit of the first word
    Declared without a parameter list: a caller passes a third argument the function ignores
 */
INT Flg_on();

/*
    clear bit x of the bit array p
    Declared without a parameter list, declared INT: a caller passes the address as an integer and uses the call as an int expression
 */
INT Flg_off();

/*
    test bit x of the bit array p; returns non-zero when it is set
 */
INT Flg_ck(VOID* p, UINT x);

/*
    run a script: each block starts with a 16-bit byte length and its commands are executed
    through the script jump table until one returns zero; pending entries of the branch stack
    are then resumed before moving on to the next block, until a zero length ends the script
 */
VOID Sce_scheduler(UCHAR* script);

#endif
