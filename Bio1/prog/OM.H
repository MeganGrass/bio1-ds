/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\OM.H
 */

#ifndef OM_H
#define OM_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

/*
    rebuild the rotation matrix of every enabled object and item model of the room and queue it for rendering
 */
VOID Om_trans(VOID);

/*
    per-frame update of the movable object models of the room: resolve collisions with enemies, the
    player and other objects, start a push when the player presses against an object, run the room
    action checks for the object and update the push status flag 0x40
 */
INT Om_move(VOID);

#endif      // OM_H
