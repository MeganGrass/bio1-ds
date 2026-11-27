/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\MAG.H
 */

#ifndef MAG_H
#define MAG_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

typedef VOID (*MAG_FUNC)(VOID); // handler taking no arguments

/*
    deactivate the ejected magazine: clear its active flag and state index on player joint 15
 */
VOID Mag_anm_clr();

/*
    per-frame update and draw of the ejected magazine (player joint 15, Free2 = active flag,
    Free3 = state index): while active and the player has a camera-switch zone, apply the
    vertical speed, run the handler of the current state, then build the matrices and queue the
    primitives in the ordering table
 */
VOID Mag_down(VOID);

/*
    state 0: copy the world matrix and rotation of player joint 14 (the hand) to the magazine,
    set the starting rotation and speed, record its height and the player's ground height,
    advance to state 1
 */
VOID Mag_mv00(PARTS_W* pP_ptr);

/*
    state 1: fall and spin the magazine; spawn an effect once when it first passes the object
    model height, and on reaching the ground height snap to it, set the rebound speed, play a
    sound and advance to state 2
 */
VOID Mag_mv01(PARTS_W* pP_ptr);

/*
    state 2: rebound arc: Dir_add sideways and spin the magazine while it rises and falls back,
    deactivate it once its height passes the ground height again
 */
VOID Mag_mv02(PARTS_W* pP_ptr);

/*
    start the ejected magazine: when inactive, activate it on player joint 15 at state 0
 */
VOID Mag_set(VOID);

extern VOID (*Mag_mv_tbl[4])(); // magazine state handlers (spawn, fall, bounce), indexed by state; the last entry is never called

#endif
