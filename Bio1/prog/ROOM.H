/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ROOM.H
 */

#ifndef ROOM_H
#define ROOM_H

#include "TYPES.H"

typedef VOID (*ROOM_FUNC)(VOID);

/*
    return the row of the room tables for the current stage; with status flag 0x20000 set the
    stage/room pair is looked up in a list and rows 7..13 are returned
 */
UCHAR Rdt_stage_get(VOID);

/*
    load and set up the current stage/room: reset the script flags, script Task and collision
    flag, switch the player character when the requested one differs, load the room file, set up
    object and item model slots and enemy entities, run the room init script and select the
    first camera cut
 */
VOID Set_room(VOID);

/*
    read the room file of the current stage/room from the CD at the load pointer, convert its
    offsets (cameras, object and item models, scripts) to addresses and set the ambient light
 */
VOID Rdt_adr_set(VOID);

extern UCHAR* pInit_scd_top;                     // room init script bytecode address
extern INT* Sce_event_adr;           // room event script address table
extern USHORT Rdt_tbl[][32];                // CD file number of each room file, by stage row and room
extern UCHAR Rdt_same_tbl[][32];            // per room: nonzero when the file is the same in both game modes

#endif      // ROOM_H
