/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SCE_AT.H
 */

#ifndef SCE_AT_H
#define SCE_AT_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

typedef INT (*SCE_AT_FUNC)();       // room action handler

/*
    rotate the vector p around the Y axis by dir and store the result in q
 */
VOID Rot_vector(SHORT dir, SVECTOR* p, SVECTOR* q);

/*
    test whether the point lies inside the XZ box of a room action record; returns 1 when inside
 */
INT Hani_ck(VECTOR* p, SEARCH_AOT* cp);

/*
    door entry handler: while the door is locked, check the key (item or special flag), unlock it or
    show the locked message; once unlocked, start the transition through the door record; returns 0
 */
INT Sce_Door(SCE_AOT* pAot);

/*
    message entry handler: show the message with the id in data0 and the wait argument in data1; returns 0
 */
INT Sce_Message(SCE_AOT* pAot);

/*
    key use entry handler: unless the equipped item is the item of the record, remember the entry and
    show the item required message; returns 0
 */
INT Sce_lock_ck(SCE_AOT* pAot);

/*
    item pickup entry handler: remember the entry and request the pickup menu when data0 is 0;
    returns 1 for other entries
 */
INT Sce_Item(SCE_AOT* pAot);

/*
    stairs entry handler for stairs running along X: pick the stair direction of the current entity
    from its facing and the half of the zone it stands in, reset its sub state and mark it as inside
    a switch zone; returns 0
 */
INT Sce_stairs_x(SCE_AOT* pAot);

/*
    stairs entry handler for stairs running along Z: pick the stair direction of the current entity
    from its facing and the half of the zone it stands in, reset its sub state and mark it as inside
    a switch zone; returns 0
 */
INT Sce_stairs_z(SCE_AOT* pAot);

/*
    flag change entry handler: clear or set (data2 non-zero) bit data1 of the script flag array
    selected by data0; returns 0
 */
INT Sce_Flg_chg(USHORT* pAot, UINT* pFlg);

/*
    item box entry handler: while the player has no hit reaction, game flag bits 0x7F00 are clear
    and update flag 0x40 is set, start the item box opening sequence (clear update flags 0x45,
    play the open sound, remember the entry); returns 0
 */
INT Sce_Itembox(SCE_AOT* pAot);

/*
    event entry handler: start event script data1 in task slot data0; returns 0
 */
INT Sce_Event(SCE_AOT* pAot);

/*
    entry handler that does nothing; returns 0
 */
INT Sce_Normal(VOID);

/*
    water entry handler: raise the in-water frame flag and, while the player moves, start a water
    effect at the next of six offsets around the player; returns 0
 */
INT Sce_Water(VOID);

/*
    ladder entry handler: flag the player as inside a switch zone, store the ladder anchor position
    in the player base position, toggle data0 and request status flag 0x10; returns 0
 */
INT Sce_ladder_set(SCE_AOT* pAot);

/*
    room event entry handler: remember the entry and request status flag 0x100 when data0 is 0;
    returns 1 for other entries
 */
INT Sce_evt_flg_set(SCE_AOT* pAot);

/*
    drawer entry handler: while the player has no hit reaction, game flag bits 0x7F00 are clear and
    update flag 0x40 is set, show the locked or missing item message, start the lock check when the
    lock item is carried, or open an unlocked drawer by showing its item model and switching to its
    cut; returns 0
 */
INT Sce_Hikidashi(SCE_AOT* pAot);

/*
    drawer sequence state machine: state 1-2 ask to use the lock item, state 3 reads the answer and
    unlocks the drawer, state 35 switches the cut and the states above 5 count down, state 5 restores
    the saved cut, state 4 applies it and hides the drawer item model
 */
INT Hikidashi_entrance(VOID);

/*
    key item pickup handler: deactivate the entry, hide its item model, clear its pickup flag, record
    the picked item and mark the map area of the item as known; returns 0
 */
INT Sce_pickup(SCE_AOT* pAot);

/*
    typewriter entry handler: while game flag bits 0x7F00 are clear and update flag 0x40 is set,
    start the save sequence when the ink ribbon is carried (remembering its slot in data0), else
    show the message that it is missing; returns 0
 */
INT Sce_Save(SCE_AOT* pAot);

/*
    slope entry handler: set the player height and floor height from the distance of the player into the
    zone along the direction in data0, with data1 as distance per step and data2 as height per step;
    returns 0
 */
INT Sce_slope_set(SCE_AOT* pAot, INT dist_in);

/*
    test every room action entry whose flags share a bit with Att and lack flag 0x80 against the
    point in front of the player (or the position of the entity for entries with flag 0x40) and run
    the handler of each entry hit; returns the result of the last handler run
 */
INT Sce_at_check(ENEMY_WORK* pEm, INT Att);

/*
    test the room action entries with flag 1 and flag 0x80 set against the point in front of the player
    (or the player position for entries with flag 0x40) and run the handler of the first entry hit;
    returns its result, or 0 when nothing was hit
 */
INT Check_action_object(VOID);

/*
    item box opening state machine: swing the lid object open with a speed that rises and then falls,
    then request the item box menu and reset the lid
 */
VOID Item_box_entrance(VOID);

/*
    typewriter save state machine: ask whether to save, on yes fade out, write the save to the memory
    card with the updated play time and fade back in
 */
INT Save_entrance(VOID);

/*
    consume the item used on a door entry once its message has ended
 */
UINT Key_lost(VOID);

extern PSP Room_psp[64];          // sprite records of the current camera's room sprites
extern SCE_AOT* pAot_data;    // room action entry of the active pickup, item box, save or drawer sequence
extern ENEMY_WORK* pPush_om;             // object model being pushed by the player
extern OBJ_MODEL_WORK* Oma_climb_om;   // room object model the player is Climb_flg
extern INT Water_idx;                 // current index into the water effect offset tables
extern INT Water_x_tbl[6];                  // water effect X offsets
extern INT Water_z_tbl[6];                  // water effect Z offsets
extern SCE_AT_FUNC Sce_at_jump_tbl[18];           // room action handler table, indexed by entry type

struct IN_DOOR_WORK
{
    SHORT X;            // trigger box x
    SHORT Z;            // trigger box z
    SHORT W;            // trigger box width
    SHORT D;            // trigger box depth
    UCHAR Anm_id;       // door animation id, copied to script variable 0 (Door_anim_id); the texture index byte follows it
    UCHAR Snd_bank;     // door sound bank
    UCHAR Dtex_type;    // door texture index, selects the door file in Dtex_file
    UCHAR Anm_type;     // door animation type in bits 0-4, bit 5 selects Vloop 2 (DOOR.C), bit 6 no door sound, bit 7 stay in the room
    UCHAR Key_id;       // door lock: bit 7 locked, bit 6 unusable for player id 3, low 6 bits Key_flg bit number
    UCHAR Next_room;    // destination stage (bits 5-7) and room (bits 0-4)
    USHORT Next_pos_x;   // player x after the transition (unsigned: read with an unsigned halfword load)
    SHORT Next_pos_y;   // player y after the transition
    USHORT Next_pos_z;   // player z after the transition (unsigned: read with an unsigned halfword load)
    SHORT Next_cdir_y;  // player facing after the transition
    UCHAR Key_type;     // item id of the key that opens the door, 0xFE and 0xFF are special locks
};

#endif      // SCE_AT_H
