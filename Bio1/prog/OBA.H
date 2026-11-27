/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\OBA.H
 */

#ifndef OBA_H
#define OBA_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

/*
    push entity pTa out of entity pEm when a collision shape of one overlaps a shape of the other
    in the XZ plane and in height; returns 1 when pTa was pushed, 0 otherwise (also when pTa is in
    state 4 or either entity has status flag 2)
 */
INT Oba_ck_hit(ENEMY_WORK* pEm, ENEMY_WORK* pTa);

/*
    rotate the offset of every collision shape of the entity by its angle and store the result in
    the entity's collision hit records
 */
VOID Oba_set_ofs(ENEMY_WORK* pEm);

#endif      // OBA_H
