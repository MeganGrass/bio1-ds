/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\def\SCENARIO.H
 */

#ifndef SCENARIO_H
#define SCENARIO_H

// Script flag banks (Ck, Set and Sce_Flg_chg: the bank byte selects the flag array). RE1 numbering, which differs from bio2;
// bank 5 is the System_flg bits (SYS_* in FLAG.H), bank 6 the Stop_flg bits (STOP_* in FLAG.H)
#define FLAG_SCENARIO   0    // Scenario_flg
#define FLAG_SCENARIO2  1    // Scenario_flg2
#define FLAG_KEY        2    // Key_flg
#define FLAG_ENEMY      3    // Enemy_flg
#define FLAG_ROOM_WK    4    // GLOBAL.Room_flg (the room work bits, ROOM_* in FLAG.H; FLAG_ROOM is the SaveData visited bits)
#define FLAG_SYSTEM     5    // System_flg
#define FLAG_STOP       6    // Stop_flg
#define FLAG_ITEM       7    // Item_flg
#define FLAG_ROOM       8    // SaveData.Room_flg (rooms visited)
#define FLAG_USE        9    // Use_flg, cleared every frame

// Set operators (Set: the operator byte). RE1 numbering, which differs from bio2
#define SET_SET         0    // set the bit
#define SET_CLEAR       1    // clear the bit
#define SET_TOGGLE      2    // invert the bit

// Comparison operators (Ck_byte, Cmp, Item_cnt_ck, Obj_prm_ck: the operator byte). The variable is the left operand
#define CMP_EQ          0    // equal
#define CMP_GT          1    // greater than
#define CMP_GE          2    // greater than or equal
#define CMP_LT          3    // less than
#define CMP_LE          4    // less than or equal
#define CMP_NE          5    // not equal

// Aot Id (SCE_AOT.Id): the kind of trigger area; the values index Sce_at_jump_tbl in SCE_AT.C. RE1 numbering, which differs
// from bio2
#define SCE_NOTHING     0    // no event
#define SCE_DOOR        1    // door
#define SCE_MESSAGE     2    // message display
#define SCE_LOCK_CK     3    // lock check (needs the matching equipped key, else a message)
#define SCE_ITEM        4    // item
#define SCE_STAIRS_X    5    // stairs zone along X
#define SCE_STAIRS_Z    6    // stairs zone along Z
#define SCE_FLAG_CHG    7    // flag change
#define SCE_ITEMBOX     8    // item box
#define SCE_EVENT       9    // scripted event
#define SCE_NORMAL      10   // generic event
#define SCE_WATER       11   // water
#define SCE_LADDER_SET  12   // ladder zone
#define SCE_EVT_FLG_SET 13   // event flag set (requests the event file)
#define SCE_HIKIDASHI   14   // drawer
#define SCE_PICKUP      15   // item pickup (clears the area and the object model)
#define SCE_SAVE        16   // save point
#define SCE_SLOPE_SET   17   // slope zone

// Aot Type (SCE_AOT.Type, the Att argument of Sce_at_check): which actors and approach directions trigger the area (same values
// as bio2)
#define SCE_PLAYER      0x01 // triggered by the player
#define SCE_04          0x04 // triggered by object models
#define SCE_UNDER       0x40 // triggers from underneath: tested at the entity position instead of the front probe

// Sound bank of Snd_se_play (the Edt_no argument): the bank selects the VAB the effect number indexes. RE1 numbering, which
// differs from bio2
#define SE_BANK_CORE    0    // core bank
#define SE_BANK_ARMS    1    // weapon bank
#define SE_BANK_ROOM    2    // room bank
#define SE_BANK_ENEMY   3    // enemy bank
#define SE_BANK_BGM     4    // BGM bank

// BGM sequence of the open SEP (Snd_bgm_play, Snd_bgm_stop, Snd_bgm_vol_set: the sequence index; the bits 0x08, 0x10 and 0x20
// of the BGM state select them in this order)
#define BGM_MAIN        0    // main sequence
#define BGM_SUB0        1    // first sub sequence
#define BGM_SUB1        2    // second sub sequence

// OBJ_MODEL_WORK.Be_flg bits
#define OM_ENABLED      0x01 // the object model is live; every object loop skips it otherwise
#define OM_NO_COLLISION 0x02 // Oma_ck_oba skips the object

#endif      // SCENARIO_H
