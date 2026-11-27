/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\OMA.H
 */

#ifndef OMA_H
#define OMA_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

/*
    resolve the overlap between an entity's collision boxes and a room object model along the
    dominant axis: with Flg set the object is pushed out and the push count is returned, otherwise
    the entity is pushed out; nothing happens when the entity (flag 8) or the object (flag 2) is disabled
 */
INT Oma_ck_oba(ENEMY_WORK* pEm, OBJ_MODEL_WORK* pOm, INT Flg);

/*
    push the second room object model out of the first along the dominant axis of their overlap;
    returns 1 when they overlapped, 0 when apart or when either object has flag 8
 */
INT Oma_ck_om(OBJ_MODEL_WORK* pOm, OBJ_MODEL_WORK* pOm2);

/*
    set up the light and local-to-screen matrices of a room object model and queue it into the
    ordering table of the active buffer (background table for flag 0x80, main table otherwise);
    skipped when its position fails the camera switch-zone test
 */
VOID Om_obj_trans(OBJ_MODEL_WORK* pOm);

/*
    start or end a climb of a room object model (flag 0x40) in front of the player: select the
    object when the player faces it within 300 angle units, toggle game flag 0x80 and bit 0x10
    of the player's zone flags, store the climb direction (1 or -1) in Timer0;
    returns 1 when the climb state changed, 0 otherwise
 */
INT Oma_pl_updown_ck(VOID);

/*
    test whether the point 470 units in front of the player lies inside the collision box of a room
    object model; returns 1 when inside, 0 otherwise. The point is left in the scratchpad vector, with
    the axis of smaller absolute value cleared when inside
 */
INT Oma_ck_front(OBJ_MODEL_WORK* pOm);

#endif
