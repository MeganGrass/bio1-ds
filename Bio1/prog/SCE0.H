/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SCE0.H
 */

#ifndef SCE0_H
#define SCE0_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBSND.H>
#include "SCRATCHPAD.H"

struct SCE_BGM_RAMP_REC
{
    USHORT head; // Opcode in the low byte, sequence number in the high byte
    USHORT Ramp; // low byte target volume, high byte Ramp time
};

struct SCE_CUT_AUTO_REC
{
    UCHAR Dummy[1];    // Opcode
    UCHAR OnOff;         // cut lock value, 0 unlocks
};

struct SCE_CUT_SET_REC
{
    UCHAR Dummy[1];    // Opcode
    UCHAR Cut_no;          // camera cut to select
};

struct SCE_CUT_REPL_REC
{
    UCHAR Dummy[1];    // Opcode
    UCHAR zone;         // camera switch zone index
    UCHAR Fcut;         // camera cut in which the zone is active
    UCHAR Tcut;         // camera cut switched to inside the zone
};

struct SCE_EM_SET_REC
{
    UCHAR Com[1];    // Opcode
    UCHAR Id;           // enemy type id
    UCHAR Type;     // initial behavior flags
    UCHAR Em_set_flg;       // death flag number, 0xFF for none
    UCHAR No_restore;   // nonzero: use the script values instead of restoring the saved state
    UCHAR Sca_num;      // collision hit records claimed from the pool
    USHORT Cdir_x;      // initial rotation about x
    USHORT Cdir_y;       // initial angle
    USHORT Cdir_z;      // initial rotation about z
    USHORT Pos_x;           // initial x position
    SHORT Pos_y;            // initial y position
    USHORT Pos_z;           // initial z position
    UCHAR slot;         // enemy slot in the low nibble
    UCHAR anim_id;      // initial animation id
    UCHAR anim_frame;   // initial animation frame
    UCHAR Se_set_no;    // sound set, stored in the high nibble of the slot info
};

struct SCE_EM_POSY_REC
{
    USHORT head; // Opcode in the low byte, target in the high byte (0 player, else enemy slot)
    USHORT y;    // floor y to store
};

struct SCE_EVT_EXEC_REC
{
    UCHAR Dummy[2];    // padding (Opcode and unused byte)
    USHORT event;       // low byte event slot, high byte script id
};

struct SCE_ITEM_AOT_REC
{
    UCHAR Dummy[1];    // Opcode
    UCHAR Aot;         // room action table slot, bit 7 selects the alternate attribute
    USHORT X;           // pickup trigger box x
    USHORT Z;           // pickup trigger box z
    USHORT W;           // pickup trigger box width
    USHORT D;           // pickup trigger box depth
    UCHAR iItem;      // item id
    UCHAR nItem;   // item count
    UCHAR Om_no;        // item model slot
    UCHAR parent;       // parent coordinate system, 0xFF for none
    SHORT pos_x;        // model x position
    SHORT pos_y;        // model y position
    SHORT pos_z;        // model z position
    USHORT angle;       // model angle
    UCHAR Item_flg;         // pickup flag number, set once the item has been taken
    UCHAR Type;      // room action entry flags
    USHORT flags;       // model flags: bit 15 starts the sparkle effect, bits 8-10 select it, bits 4-7 raise it
};

struct SCE_ITEM_CHG_REC
{
    USHORT head;   // Opcode in the low byte, mode in the high byte (0 read count, 1 write count, 2 carried count)
    USHORT target; // low byte room action slot, high byte state byte index
};

struct SCE_ITEM_OP_REC
{
    UCHAR Dummy[1];    // Opcode
    UCHAR Item_no;      // item id to test
};

struct SCE_LIGHT_PRM_REC
{
    SHORT x;       // light position x
    SHORT y;       // light position y
    SHORT z;       // light position z
    UCHAR r;       // light red
    UCHAR g;       // light green
    UCHAR b;       // light blue
    UCHAR type;    // light type
    USHORT radius; // light range
};

struct SCE_MIRROR_REC
{
    UCHAR Opcode;     // Opcode
    UCHAR Mirror_flg;       // mirror mode flags, or-ed into the game flags
    USHORT Mirror_min; // mirror reflection range start
    USHORT Mirror_max; // mirror reflection range end
    USHORT Mirror_pos;     // mirror plane position
};

struct SCE_OBJ_FLG_REC
{
    UCHAR Dummy[1];    // Opcode
    UCHAR index;        // item model slot
    UCHAR value;        // flags to store
};

struct SCE_OBJ_SET_REC
{
    UCHAR Dummy[1];    // Opcode
    UCHAR Om_no;           // object model slot in bits 0-5, bit 7 fades the model palette
    UCHAR Be_flg;        // model flags, bit 4 selects the alternate attribute
    UCHAR Super;       // parent coordinate system: 0xFF none, 0xFE fixed, below 0x80 object model, above that enemy
    USHORT Pos_x;           // position x
    USHORT Pos_y;           // position y
    USHORT Pos_z;           // position z
    USHORT Cdir_y;       // angle
    USHORT Probe1_x;    // collision probe point 1 x
    USHORT Probe1_z;    // collision probe point 1 z
    USHORT Probe2_x;    // collision probe point 2 x
    USHORT Probe2_z;    // collision probe point 2 z
    USHORT radius;      // collision radius
    USHORT At_h;        // collision half height
    USHORT half_x;      // collision box offset x
    USHORT half_z;      // collision box offset z
};

struct SCE_PL_DIR_REC
{
    UCHAR Opcode;       // Opcode
    UCHAR Dummy1[1];    // padding
    USHORT Min_dir;     // start of the accepted angle range
    USHORT Max_dir;     // end of the accepted angle range
};

struct SCE_PL_DIST_REC
{
    UCHAR Dummy[2];    // padding (Opcode and unused byte)
    SHORT target;       // low byte target kind (0 enemy, 1 object model, 2 item model), high byte slot
    USHORT distance;    // maximum distance
};

struct SCE_PL_POS_REC
{
    UCHAR Dummy[2];    // padding (Opcode and unused byte)
    SHORT Cdir_x;       // rotation about x
    SHORT Cdir_y;        // angle
    SHORT Cdir_z;       // rotation about z
    SHORT Pos_x;            // position x
    SHORT Pos_y;            // position y
    SHORT Pos_z;            // position z
};

struct SCE_AOT_RESET_REC
{
    UCHAR Dummy[1];    // Opcode
    UCHAR Aot;         // room action table slot
    UCHAR Id;         // new entry type
    UCHAR Type;        // new entry flags
    USHORT Data0;       // new entry argument 0
    USHORT Data1;       // new entry argument 1
    USHORT Data2;       // new entry argument 2
};

struct SCE_AOT_SET_REC
{
    UCHAR Dummy[1];    // Opcode
    UCHAR Aot;         // room action table slot
    USHORT X;           // trigger box x
    USHORT Z;           // trigger box z
    USHORT W;           // trigger box width
    USHORT D;           // trigger box depth
    UCHAR Id;         // entry type
    UCHAR Type;        // entry flags
    USHORT Data0;       // entry argument 0
    USHORT Data1;       // entry argument 1
    USHORT Data2;       // entry argument 2
};

struct SCE_PSP_SET_REC
{
    UCHAR Dummy[2];    // padding (Opcode and unused byte)
    USHORT sprite;      // low byte sprite group, high byte 0 activates and nonzero deactivates
};

struct SCE_SE_ON_REC
{
    SHORT head;  // Opcode in the low byte, sound bank in the high byte
    SHORT sound; // low byte sound id, high byte signed parameter
    SHORT where; // low byte source kind: 0 position from the script, 1 player, otherwise enemy
    SHORT vx;    // source position x
    SHORT z;    // source position z
};

struct SCE_SAVE_REC
{
    USHORT head;  // Opcode in the low byte, state word index in the high byte
    USHORT Set_val; // value to store
};

struct SCE_CMP_REC
{
    UCHAR Opcode;       // Opcode
    UCHAR Dummy[1];    // padding
    UCHAR Flag;        // state word index
    UCHAR Operator;      // comparison kind (equal, greater, greater or equal, less, less or equal, not equal)
    USHORT Cmp_val;       // value to compare with
};

struct SCE_VIB_REC
{
    USHORT head;     // Opcode in the low byte, strength in the high byte where used
    USHORT Time; // vibration duration
    USHORT Attr;    // delay before the vibration starts
};

struct SCE_VIB_FADE_REC
{
    USHORT head;     // Opcode in the low byte, nonzero high byte stops the current vibration first
    USHORT strength; // low byte start strength, high byte end strength
    USHORT Time; // fade duration
    USHORT Attr;    // delay before the fade starts
};

/*
    clear the type of every room action table entry and reset the table end pointer to the first entry
 */
VOID Sce_aot_init(VOID);

/*
    script op 0x00: reset the branch nesting count, which ends the script run
 */
INT Evt_end(VOID);

/*
    script op 0x01: push the address of the else/endif part, given as a byte offset from the
    operand, on the branch stack and count one more nesting level
 */
INT Ifel_ck(VOID);

/*
    script op 0x02: pop the branch stack, drop one nesting level and skip forward by the byte offset in the operand
 */
INT Else_ck(VOID);

/*
    script op 0x03: pop the branch stack, drop one nesting level and skip the operand
 */
INT Endif(VOID);

/*
    script op 0x04: test one bit of a script flag array and compare it with the expected value;
    array 10 tests the high bit of the player hit state instead
 */
INT Ck(VOID);

/*
    script op 0x05: set, clear or toggle one bit of a script flag array
 */
INT Set(VOID);

/*
    script op 0x06: compare a state byte register with a value (equal, greater, greater or equal,
    less, less or equal, not equal)
 */
INT Ck_byte(VOID);

/*
    script op 0x07: compare a state word register with a value, using the same comparison kinds as op 0x06
 */
INT Cmp(VOID);

/*
    script op 0x08: store a value in a state byte register
 */
INT Set_byte(VOID);

/*
    script op 0x31: store a value in a state word register
 */
INT Save(VOID);

/*
    script op 0x0E: skip the operand
 */
INT Nop(VOID);

/*
    script op 0x09: remember the current cut, select the cut given by the script, point the camera
    switch zone cursor at it, apply it and lock the cut
 */
INT Cut_chg(VOID);

/*
    script op 0x0A: restore the remembered cut, point the camera switch zone cursor at it, apply it and unlock the cut
 */
INT Cut_old(VOID);

/*
    script op 0x0B: show a message with the id and wait argument given by the script
 */
INT Message_on(VOID);

/*
    script op 0x37: store a value in the room BGM table at the given row and column
 */
INT Sce_bgmtbl_set(VOID);

/*
    script op 0x2E: does nothing; does not advance the script pointer
 */
INT Nop_2E(VOID);

/*
    script op 0x15: start a SEP sequence and mark it in the BGM state mask
 */
INT Sce_bgm_play(VOID);

/*
    script op 0x4A: shift the BGM state mask back down and restart the sequences marked in it
    at their stored volumes; ignored while no BGM state is requested
 */
INT Sce_bgm_restore(VOID);

/*
    script op 0x4B: mute and stop every playing SEP sequence, key off all voices and shift the
    BGM state mask up; ignored while no BGM state is requested
 */
INT Sce_bgm_stop_all(VOID);

/*
    script op 0x2F: set the left and right volume of a SEP sequence and remember it in the sequence volume table
 */
INT Sce_snd_vol_set(VOID);

/*
    script op 0x16: mute and stop a SEP sequence that is marked playing, clear its BGM state bit and
    reset its volume
 */
INT Sce_bgm_stop(VOID);

/*
    script op 0x26: does nothing; does not advance the script pointer
 */
INT Nop_26(VOID);

/*
    script op 0x27: start a BGM fade with the step given by the script
 */
INT Sce_snd_fade_set(VOID);

/*
    script op 0x43: start a crescendo on a SEP sequence that is marked playing
 */
INT Sce_bgm_vol_ramp(VOID);

/*
    script op 0x17: play a sound effect; the source is a position from the script, the player, an
    enemy or none (plain effect), selected by the low byte of the third word
 */
INT Se_on(VOID);

/*
    script op 0x1D: test whether the equipped item has the given id
 */
INT Equip_Item_ck(VOID);

/*
    script op 0x11: test whether the last picked up item has the given id
 */
INT Sce_picked_item_ck(VOID);

/*
    script op 0x10: test whether the last used item has the given id
 */
INT Sce_used_item_ck(VOID);

/*
    script op 0x19: set the flags of an item model slot
 */
INT Obj_model_flg_set(VOID);

/*
    script op 0x1A: test whether the item with the given id is carried
 */
INT Keep_Item_ck(VOID);

/*
    script op 0x12: rewrite the type, flags and data words of a room action table entry
 */
INT Aot_reset(VOID);

/*
    script op 0x13: rewrite the type and flags of a room action table entry
 */
INT Aot_arm(VOID);

/*
    script op 0x14: start a script event task
 */
INT Evt_exec(VOID);

/*
    script op 0x44: deactivate a script event task
 */
INT Evt_kill(VOID);

/*
    script op 0x0F: configure the mirror (mode flags, range and plane) and copy the player model
    and weapon into the mirror
 */
INT Mirror_set(VOID);

/*
    script op 0x0D: define a room action table entry from the script record and extend the table end pointer
 */
INT Aot_set(VOID);

/*
    script op 0x0C: define a door entry of the room action table from the script record and
    extend the table end pointer
 */
INT Door_aot_set(VOID);

/*
    script op 0x18: define an item pickup entry of the room action table and set up its item model:
    entry type follows the item id range while the pickup flag is set, the model is linked from the
    room item models and an effect is started when the record asks for one
 */
INT Item_aot_set(VOID);

/*
    script op 0x1B: create an enemy from the script record unless its death flag is set; restores
    the saved enemy state or initialises position, angle and animation, then claims a collision hit record
 */
INT Sce_em_set(VOID);

/*
    script op 0x29: request movie playback of the given movie id
 */
INT Movie_on(VOID);

/*
    script op 0x1E: start a voice clip and set the voice-playing status flag
 */
INT Xa_on(VOID);

/*
    script op 0x1F: set up an object model slot of the room from the script record: link the model
    (loading its texture and TMD when new), choose the parent coordinate, then set attribute,
    angle, collision box, probe points and position
 */
INT Obj_model_set(VOID);

/*
    script op 0x25: activate or deactivate every room sprite of a group
 */
INT Sce_psp_set(VOID);

/*
    script op 0x1C: set the room fade parameters (level byte, step and colour channel mask);
    a non-zero step starts the fade at 0 when positive and at its maximum otherwise
 */
INT Sce_fade_set(VOID);

/*
    script op 0x4F: start vibration with the duration and delay given by the script
 */
INT Vib_set0(VOID);

/*
    script op 0x50: start vibration with the duration, strength and delay given by the script
 */
INT Vib_set1(VOID);

/*
    script op 0x51: start a vibration fade from one strength to another, stopping the current
    vibration first when requested
 */
INT Vib_fade_set(VOID);

/*
    script op 0x52: stop all vibration
 */
INT Vib_stop(VOID);

/*
    script op 0x20: set the rotation and position of the player and clear bits 0x0C of its script entity flags
 */
INT Sce_pl_pos_set(VOID);

/*
    script op 0x21: set the rotation and position of an enemy and clear bits 0x0C of its script entity flags
 */
INT Sce_em_pos_set(VOID);

/*
    script op 0x47: set the rotation and position of an object model
 */
INT Obj_trans_set(VOID);

/*
    script op 0x22: compare the carried count of an item with a value; ammunition ids also add
    the counts of the weapons that use it
 */
INT Item_cnt_ck(VOID);

/*
    script op 0x23: lock or unlock the camera cut
 */
INT Cut_auto(VOID);

/*
    script op 0x24: run the room action handler given by the script on a room action table entry
 */
INT Aot_on(VOID);

/*
    script op 0x28: change a property of an enemy: behavior flags, health and hit state, action
    behavior, status flags, angle, blend counter, action reset, joint visibility or action state
 */
INT Sce_em_prm_set(VOID);

/*
    script op 0x33: change a property of the player: unequip, hit state, status flags, action,
    angle, blend counter, action reset, collision flags, joint visibility or the script entity flag
 */
INT Sce_pl_prm_set(VOID);

/*
    script op 0x2A: start an effect sprite at the position from the script, attached to the
    identity matrix, the player, an object model or an enemy
 */
INT Sce_espr_on(VOID);

/*
    script op 0x3D: start an effect sprite like op 0x2A and remember its id and parent matrix as the bullet effect
 */
INT Sce_espr_on2(VOID);

/*
    script op 0x3E: stop the remembered bullet effect
 */
INT Sce_espr_kill(VOID);

/*
    script op 0x42: stop the effects of the given id and group
 */
INT Sce_espr_kill2(VOID);

/*
    script op 0x48: deactivate every effect slot
 */
INT Sce_espr_clr(VOID);

/*
    script op 0x4E: set, clear or toggle bits of the draw flags of every active effect
 */
INT Sce_espr_control(VOID);

/*
    script op 0x2B: start an attack animation on the player
 */
INT Sce_atk_anim_set(VOID);

/*
    script op 0x2C: remove a carried item and compact the item list; returns 0 when it is not carried
 */
INT Sce_Item_lost(VOID);

/*
    script op 0x2D: run a room action entry, then set menu request bit 0x400 and toggle bit 0x800;
    ends the script run
 */
INT Sce_Item_get(VOID);

/*
    script op 0x30: rewrite the bounds (and optionally the floor bits) of a collision boundary of the room
 */
INT Sca_id_set(VOID);

/*
    script op 0x32: rewrite the texture page word of every primitive of an object model
 */
INT Obj_clut_set(VOID);

/*
    script op 0x34: add a delta to, set or clear a CLUT fade
 */
INT Col_chg_set(VOID);

/*
    script op 0x35: set the flags of an object model or an item model slot
 */
INT Obj_flg_set(VOID);

/*
    script op 0x36: compare the Push_cnt counter of an object model with a value, using the
    comparison kinds of op 0x06
 */
INT Obj_prm_ck(VOID);

/*
    script op 0x38: test the held pad buttons against a mask, or test that none of the masked buttons is held
 */
INT Sce_key_ck(VOID);

/*
    script op 0x39: copy the behavior flags of an enemy to the last enemy flags byte
 */
INT Sce_em_flg_get(VOID);

/*
    script op 0x3A: rewrite the source and target camera of a camera switch zone
 */
INT Cut_replace(VOID);

/*
    script op 0x3B: set the rotation of an item model or object model if it is enabled
 */
INT Obj_rot_set(VOID);

/*
    script op 0x3C: test whether the player is within a distance of an enemy, object model or item model
 */
INT Sce_pl_dist_ck(VECTOR* pos_in);

/*
    script op 0x3F: test whether the player angle lies within the given angle range
 */
INT Dir_ck(VOID);

/*
    script op 0x40: overwrite the view point, reference point and field of view of a room cut
 */
INT Sce_light_prm_set(VOID);

/*
    script op 0x41: set the floor height of the player or of an enemy
 */
INT Sce_em_posy_set(VOID);

/*
    script op 0x45: add a value to the floor height of the player
 */
INT Sce_pl_posy_add(VOID);

/*
    script op 0x46: replace the three room lights and the ambient colour of the room
 */
INT Sce_room_light_set(VOID);

/*
    deactivate the room sprites of every group set in the sprite hide mask, then clear the mask
 */
INT Psp_mask_set(VOID);

/*
    script op 0x49: add a room sprite group to the sprite hide mask, or clear the mask for group 0xFF
 */
INT Sce_psp_hide(VOID);

/*
    script op 0x4C: copy the item count of a room action entry to a state byte register, or the reverse,
    or copy the carried count of its item to both
 */
INT Sce_Item_chg(VOID);

/*
    script op 0x4D: raise the colour of the 14 pSin_parts_ptr of the player model
 */
INT Sce_pl_parts_col(VOID);

struct MODEL_LINK_INFO
{
    VOID* Tmd_adr;  // TMD (mesh) address of the room model, made absolute on room load; 0 = none
    VOID* Tim_adr;  // TIM (texture) address of the room model, made absolute on room load; 0 = none
};

extern LONG Bgm_status_mask;               // BGM state mask: bit (3 + sequence) per playing sequence, shifted up by 8 while stopped
extern USHORT Sprite_hide_mask;  // mask of room sprite groups to hide
extern OBJ_MODEL_WORK* pOm_tbl[16];   // item model slots of the current room
extern OBJ_MODEL_WORK* Omodel_tbl[16];  // object model slot buffer of the room
extern MODEL_LINK_INFO* pItem_model_last;    // model link record of the last item model set up
extern MODEL_LINK_INFO* pOmodel_last; // model link record of the last object model set up
extern UCHAR Omodel_num;                 // number of object models set up in the room
extern SCE_AOT* pAot_last;             // last used entry of the room action table
extern SndVolume Seqvol[3];                   // left/right volume of each SEP sequence set by the script
extern UCHAR Imodel_num;               // number of item models set up in the room
extern SCE_AOT Sce_aot_tbl[24];    // room action table of the current room
extern INT (*Sce_jmp_tbl[83])(); // script Opcode handler table, indexed by Opcode

#endif      // SCE0_H
