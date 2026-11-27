/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM13.H
 */

#ifndef EM13_H
#define EM13_H

#include "TYPES.H"

typedef VOID (*EM13_FUNC)(VOID);    // enemy state handler

/*
    run the state handler of the current web and clear its coordinate flag word, then test its
    position against the camera switch zones and update its shadow when the test passes
 */
VOID Em13_Main(VOID);

/*
    state 0: enter the idle state, reset parts, shadow and animation work, set the health
    and the collision record chosen by the player id
 */
VOID Em13_Init(VOID);

/*
    state 1: clear the status flags above the low five bits and set flag 0x40
 */
VOID Em13_Move(VOID);

/*
    state 2: return to idle; on a hit, hide one more joint (counted from the last) for every
    10 points of health below 50, then clear the hit state
 */
VOID Em13_Damage(VOID);

/*
    state 3: hide every joint, set the death event flag and enter the gone state
 */
VOID Em13_Die(VOID);

/*
    state 4: final state, does nothing
 */
VOID Em13_Dead(VOID);

extern AT_TBL Em13_sca00;    // collision record used when the player id is even
extern AT_TBL Em13_sca01;    // collision record used when the player id is odd
extern AT_TBL* Em13_sca_tbl[];     // collision records indexed by player id & 1
extern EM13_FUNC Em13_routine_0[5];      // state handlers of the web, indexed by state

#endif      // EM13_H
