/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\def\TYPES.H
 */

#ifndef TYPES_H
#define TYPES_H

#define TRUE 1
#define FALSE 0

#define ON 1
#define OFF 0

#define ENABLED 1
#define DISABLED 0

/*
    Named constants. The pad button bits stay here (same values as bio2-ds). The status/system flag bits are in FLAG.H,
    the script flag banks, operators, trigger ids and OM_* bits in SCENARIO.H, the fixed RAM addresses in MEM_MAP.H; those
    three are constants-only headers that a unit includes after its other includes.
 */

// G.Padd* -- raw PS1 digital-pad button bits (same values as bio2 DEF\CONTROLLER.H; 0x200/0x400 are L3/R3, absent on this pad)
#define PAD_L2        0x0001
#define PAD_R2        0x0002
#define PAD_L1        0x0004
#define PAD_R1        0x0008
#define PAD_TRIANGLE  0x0010
#define PAD_CIRCLE    0x0020
#define PAD_X         0x0040
#define PAD_SQUARE    0x0080
#define PAD_SELECT    0x0100
#define PAD_START     0x0800
#define PAD_UP        0x1000
#define PAD_RIGHT     0x2000
#define PAD_DOWN      0x4000
#define PAD_LEFT      0x8000

// G.SaveData.Key / Key_trg -- the key-config mapped buttons (names from bio2 DEF\CONTROLLER.H; bit n = Key_tbl[n] pad mask)
#define KEY_UP        0x0001
#define KEY_RIGHT     0x0002
#define KEY_DOWN      0x0004
#define KEY_LEFT      0x0008
#define KEY_INSPECT   0x0080
#define KEY_AIM       0x0100
#define KEY_RUN       0x0200

// forward declarations
typedef struct KAGE_WORK KAGE_WORK;
typedef struct AT_WORK AT_WORK;
typedef struct AT_TBL AT_TBL;
typedef struct PARTS_W PARTS_W;
typedef struct ENEMY_WORK ENEMY_WORK;
typedef struct PLAYER_WORK PLAYER_WORK;
typedef struct ESP_WORK ESP_WORK;
typedef struct ECG_SEG_REC ECG_SEG_REC;
typedef struct STATUS STATUS;
typedef struct RCUT RCUT;
typedef struct LIGHT_CUT_DATA LIGHT_CUT_DATA;
typedef struct ROOM_WORK ROOM_WORK;
typedef struct COM_ROW_REC COM_ROW_REC;
typedef struct DOOR_WORK DOOR_WORK;
typedef struct DOOR_VIEW_REC DOOR_VIEW_REC;
typedef struct SCREEN_SHAKE_WORK SCREEN_SHAKE_WORK;
typedef struct SCE_TASK SCE_TASK;
typedef struct ESP_SPR_TBL ESP_SPR_TBL;
typedef struct STAGE_RES_WORK STAGE_RES_WORK;
typedef struct AT_ESP_REC AT_ESP_REC;
typedef struct PSP PSP;
typedef struct TASK TASK;
typedef struct MONITOR MONITOR;
typedef struct ITEM_DATA ITEM_DATA;
typedef struct ITEM_INFO_TBL ITEM_INFO_TBL;
typedef struct PL_ESP_REC PL_ESP_REC;
typedef struct AIM_LIM_REC AIM_LIM_REC;
typedef struct AIM_DATA AIM_DATA;
typedef union DOOR_MENU_WORK DOOR_MENU_WORK;
typedef struct PDEMO PDEMO;
typedef struct PICKUP_WORK PICKUP_WORK;
typedef struct KEY_CFG_WORK KEY_CFG_WORK;
typedef struct ITEM_WORK ITEM_WORK;
typedef struct EDT_TABLE_WORK EDT_TABLE_WORK;
typedef struct OPT_WORK OPT_WORK;
typedef struct SAVE_DATA SAVE_DATA;
typedef struct SCENARIO SCENARIO;
typedef struct GLOBAL GLOBAL;
typedef struct _CARD _CARD;
typedef struct CFILE_INFO CFILE_INFO;
typedef struct ANM_REC ANM_REC;
typedef struct ANM_FRAME_REC ANM_FRAME_REC;
typedef struct BLOCK_REC BLOCK_REC;
typedef struct SCA_HEAD SCA_HEAD;
typedef struct VCUT VCUT;
typedef struct fpos fpos;
typedef struct COM_POINT_REC COM_POINT_REC;
typedef struct COM_QUAD_REC COM_QUAD_REC;
typedef struct COM_WND_ROW_REC COM_WND_ROW_REC;
typedef struct COM_WND_WORK COM_WND_WORK;
typedef struct COM_TEXT_REC COM_TEXT_REC;
typedef struct COM_CURSOR_WORK COM_CURSOR_WORK;
typedef struct CINESCO_FADE_REC CINESCO_FADE_REC;
typedef struct PL_DIE_REC PL_DIE_REC;
typedef struct DOOR_OP_REC DOOR_OP_REC;
typedef struct DOOR_OPB_REC DOOR_OPB_REC;
typedef struct DOOR_VTX_REC DOOR_VTX_REC;
typedef struct MSEQUENCE MSEQUENCE;
typedef struct SSEQUENCE SSEQUENCE;
typedef struct KO_TBL KO_TBL;
typedef struct KAN_HEAD KAN_HEAD;
typedef struct CORPSE_SAVE_WORK CORPSE_SAVE_WORK;
typedef struct ESP_DATA_HDR ESP_DATA_HDR;
typedef struct FILE_WORK FILE_WORK;
typedef struct MAP_SCR_WORK MAP_SCR_WORK;
typedef struct OBJ_MODEL_WORK OBJ_MODEL_WORK;
typedef struct MODEL_LINK_INFO MODEL_LINK_INFO;
typedef struct SEARCH_AOT SEARCH_AOT;
typedef struct SCE_AOT SCE_AOT;
typedef struct SCA_DATA SCA_DATA;
typedef struct PSP_SPR_REC PSP_SPR_REC;
typedef struct SAVE_POS_REC SAVE_POS_REC;
typedef struct SCA_BOX_REC SCA_BOX_REC;
typedef struct IN_DOOR_WORK IN_DOOR_WORK;
typedef struct EVT_CMD_REC EVT_CMD_REC;
typedef struct EVT_EXEC_REC EVT_EXEC_REC;
typedef struct SCE_BGM_RAMP_REC SCE_BGM_RAMP_REC;
typedef struct SCE_CUT_AUTO_REC SCE_CUT_AUTO_REC;
typedef struct SCE_CUT_SET_REC SCE_CUT_SET_REC;
typedef struct SCE_CUT_REPL_REC SCE_CUT_REPL_REC;
typedef struct SCE_EM_SET_REC SCE_EM_SET_REC;
typedef struct SCE_EM_POSY_REC SCE_EM_POSY_REC;
typedef struct SCE_EVT_EXEC_REC SCE_EVT_EXEC_REC;
typedef struct SCE_ITEM_AOT_REC SCE_ITEM_AOT_REC;
typedef struct SCE_ITEM_CHG_REC SCE_ITEM_CHG_REC;
typedef struct SCE_ITEM_OP_REC SCE_ITEM_OP_REC;
typedef struct SCE_LIGHT_PRM_REC SCE_LIGHT_PRM_REC;
typedef struct SCE_MIRROR_REC SCE_MIRROR_REC;
typedef struct SCE_OBJ_FLG_REC SCE_OBJ_FLG_REC;
typedef struct SCE_OBJ_SET_REC SCE_OBJ_SET_REC;
typedef struct SCE_PL_DIR_REC SCE_PL_DIR_REC;
typedef struct SCE_PL_DIST_REC SCE_PL_DIST_REC;
typedef struct SCE_PL_POS_REC SCE_PL_POS_REC;
typedef struct SCE_AOT_RESET_REC SCE_AOT_RESET_REC;
typedef struct SCE_AOT_SET_REC SCE_AOT_SET_REC;
typedef struct SCE_PSP_SET_REC SCE_PSP_SET_REC;
typedef struct SCE_SE_ON_REC SCE_SE_ON_REC;
typedef struct SCE_SAVE_REC SCE_SAVE_REC;
typedef struct SCE_CMP_REC SCE_CMP_REC;
typedef struct SCE_VIB_REC SCE_VIB_REC;
typedef struct SCE_VIB_FADE_REC SCE_VIB_FADE_REC;
typedef struct SP_WORK SP_WORK;
typedef struct TMD_MEM TMD_MEM;
typedef struct TMD_HEAD TMD_HEAD;
typedef struct VIB_REQ VIB_REQ;
typedef struct COM_LOGO_WORK COM_LOGO_WORK;
typedef struct COM_TYPER_WORK COM_TYPER_WORK;
typedef struct DOOR_CMD_WORK DOOR_CMD_WORK;
typedef struct ENDING_PIC_REC ENDING_PIC_REC;
typedef struct END_SPR_WORK END_SPR_WORK;
typedef struct ENDING_TEXT_HEAD ENDING_TEXT_HEAD;
typedef struct CAPTION_TBL CAPTION_TBL;
typedef struct PANEL_WORK PANEL_WORK;
typedef struct PANEL_SLIDE_WORK PANEL_SLIDE_WORK;
typedef struct SUBPLAY_WORK SUBPLAY_WORK;
typedef struct SCRATCHPAD_BUF SCRATCHPAD_BUF;
typedef struct TITLE_TIMER_REC TITLE_TIMER_REC;
typedef struct WEB_WORK WEB_WORK;

typedef char CHAR;
typedef signed char SCHAR;
typedef unsigned char UCHAR;
typedef short SHORT;
typedef unsigned short USHORT;
typedef int INT;
typedef unsigned int UINT;
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;
typedef long int LONG;
typedef unsigned long int ULONG;

typedef void VOID;

typedef void (*CODE_PTR)();

// call through an unprototyped function pointer: no prototype argument conversions
#define NOPROTO(Ret, Name) ((Ret (*)())(Name))
// the same through a pointer with the given parameter list: FN_CAST(INT, (SCHAR, INT), Name)
#define FN_CAST(Ret, Args, Name) ((Ret (*)Args)(Name))

typedef UINT BOOL;

#include <SYS/TYPES.H>
#include <KERNEL.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

#include "MEM_MAP.H"

#define MATRIX_POS_PTR(pMat) ((VECTOR*)(pMat)->t)   // translation of a matrix viewed as a VECTOR (its 4th word is the one after the matrix)
#define MATRIX_POS(pMat) (*MATRIX_POS_PTR(pMat))    // the same, as a value

struct ESP_WORK
{
    UCHAR Routine0;             // behaviour routine run before drawing; 0 = free slot
    UCHAR Routine1;             // behaviour routine run after the movement update
    UCHAR Mat_mode;             // matrix mode: 1 = follow the parent matrix, 2 = keep its own copy
    UCHAR Mag;                  // sprite scale factor; multiplied with the camera fov and divided by Otz4 in Esp_pos
    UCHAR Free0;                // behaviour parameter byte, copied from the phase record
    UCHAR Free1;                // behaviour parameter byte, copied from the phase record
    UCHAR Free2;                // behaviour counter byte, copied from the phase record
    UCHAR Free3;                // behaviour counter byte, copied from the phase record
    SHORT Add_x;               // acceleration x, added to Speed_x each frame
    SHORT Add_y;               // acceleration y, added to Speed_y each frame
    USHORT Add_z;               // acceleration z, added to Speed_z each frame
    USHORT Be_flg;            // draw control bits: 1 animate, 2 update position, 4 force y 0, 8 force y to floor, 0x8000 hidden, bits 6-7 blend mode
    SHORT Speed_x;             // velocity x, added to Mov_x each frame
    SHORT Speed_y;              // velocity y, added to Mov_y each frame
    USHORT Speed_z;             // velocity z, added to Mov_z each frame
    SHORT DirY;                 // rotation about the y axis applied to the displacement; also passed to spawned effects
    SHORT Mov_x;                // displacement x, rotated by DirY and added to Hot_x
    SHORT Mov_y;                // displacement y
    SHORT Mov_z;                // displacement z
    UCHAR Anm_time;             // ticks left on the current sprite frame
    UCHAR Anm_cnt;              // uv index of the current sprite frame
    SHORT Pos_x;                // world position x
    SHORT Pos_y;                // world position y
    SHORT Pos_z;                // world position z
    UCHAR Type;             // effect type given to Esp_call; selects the sprite and move tables
    UCHAR Id;                   // effect id within the type, given to Esp_call; bits 3 and up select the palette row
    SHORT Hot_x;                // offset x from the parent matrix (copy of the Esp_call vector)
    SHORT Hot_y;                // offset y from the parent matrix
    SHORT Hot_z;                // offset z from the parent matrix
    SHORT Otz4;                 // projected depth times 4; divisor of the sprite scale
    MATRIX Mat;                 // matrix the position is transformed with (copy of the parent matrix when Mat_mode is 1)
    UINT Otz;                    // projected depth; >> 4 is the ordering table index
    INT Org_x;                  // spawn vector x passed to Esp_call (VECTOR with Org_y, Org_z)
    INT Org_y;                  // spawn vector y
    INT Org_z;                  // spawn vector z
    INT Dummy60;                // padding word of the spawn vector
    MATRIX* pMat;            // matrix of the parent the effect follows
    ESP_DATA_HDR* pEsp_hdr;    // sprite set header of the effect: counts, palette and texture page
    SSEQUENCE* pAnmseq;         // start of the sprite frame list of the effect
    SSEQUENCE* pAnm_cur;        // current sprite frame
    VOID* pSprg;                  // start of the texture coordinate list of the effect
    VOID* pSprg_cur;              // texture coordinates of the current frame
    VOID* pMoveseq_top;           // first behaviour step record of the effect
    VOID* pMoveseq;               // current behaviour step record
};

struct KAGE_WORK
{
    SHORT Off_x;       // shadow offset from the entity position, x (first SVECTOR copied by Kage_work_init)
    USHORT Off_y;       // shadow offset y (MulKage does not read it; EM06 adjusts it, Kage_work_init copies the whole SVECTOR)
    SHORT Off_z;       // shadow offset from the entity position, z
    USHORT Dummy06;     // padding (SVECTOR pad word)
    POLY_FT4 Prim[2];   // the two shadow quad primitives, one per display buffer
    SVECTOR Vtx[4];     // shadow quad corner offsets on the floor plane (x/z of four SVECTORs)
};

struct PLAYER_WORK
{
    UCHAR Be_flg;                   // slot state bits: 0x01 slot in use, 0x02 hit checks off, 0x04 wall collision off (Sca_ck), 0x08 object collision off (Oma), 0x10 last wall slide axis, 0xE0 attack class bits tested by ATTACK.C (player in range / alert)
    UCHAR Id;                       // entity type id
    UCHAR Type;                     // variant selector set by the room script; player: equipped weapon number (weapon = Type - 2); enemies: bits 0xC0 exclude the entity from auto-aim
    UCHAR Zone_in_flg;              // camera-switch zone result for the position; bit 7 skips the rotation refresh
    AT_TBL* pAt_tbl;                // collision shape record (radius, half height, centre offset) of the entity
    AT_WORK* pSca_ofs;              // collision centre offset record (x, y, z)
    UINT Attribute;                 // model object attribute flags (start of the embedded GsDOBJ5)
    GsCOORDINATE2* pCoord2;         // coordinate system of the model (points at Coord_flg)
    VOID* pTmd;                       // model (TMD) data of the entity
    VOID* pPacket;                    // polygon packet buffer of the model
    INT Coord_flg;                  // coordinate flag word, start of the embedded GsCOORDINATE2
    UCHAR M[0x34 - 0x20];           // rotation matrix of the entity (GsCOORDINATE2 matrix, rotation part; translation is Pos_x/y/z)
    INT Pos_x;                      // world position x
    INT Pos_y;                      // world position y
    INT Pos_z;                      // world position z
    INT Workm;                      // first word of the world matrix (workm) of the coordinate block
    UCHAR Dummy044[0x6C - 0x44];    // rest of the embedded coordinate block (world matrix and links), not accessed by name
    SHORT Old_x;                   // x position at the start of the frame (previous-frame position, movement delta base)
    SHORT Old_y;                   // y position at the start of the frame
    USHORT Old_z;                   // z position at the start of the frame
    USHORT Cdir_x;                  // rotation about x (model pitch)
    USHORT Cdir_y;                  // facing angle (rotation about y)
    USHORT Cdir_z;                  // rotation about z (roll)
    SHORT Spd_x;                    // movement velocity x
    SHORT Spd_y;                    // movement velocity y
    SHORT Spd_z;                    // movement velocity z
    SHORT Old_dir;                 // facing angle (Cdir_y) of the previous frame; restored when a move is blocked (SCA.C); some enemies reuse it as a small counter
    SHORT Aspd_x;                   // rotation speed added to Cdir_x by the script move command
    SHORT Aspd_y;                   // rotation speed added to Cdir_y by the script move command
    UCHAR Routine_0;                // main routine number (Routine_0..3 are often written as one 32-bit word)
    UCHAR Routine_1;                // sub-routine number within Routine_0
    UCHAR Routine_2;                // action routine number (index into the player action table); script-driven when Routine_0 = 8
    UCHAR Routine_3;                // step counter within the current action routine (advanced by Joint_move results)
    SHORT Life;                     // hit points; negative when dead
    UCHAR Damage_no;                // hit reaction code written by the attack code, 0 = none: bits 0-2 reaction number (1-based, selects the damage routine), bits 3-6 hit class of the weapon, 0x80 set by the player fall routine
    UCHAR Dummy08b;                 // padding, never accessed
    UCHAR Hokan_flg;                // animation blend step counter (interpolation flag)
    UCHAR Parts_num;                // number of PARTS_W entries in pSin_parts_ptr
    SHORT Ground;                   // floor height at the position
    VOID* pKan_t_ptr;               // joint (Kan) animation table of the loaded model file
    VOID* pSeq_t_ptr;               // animation sequence (Seq) table of the loaded model file
    PARTS_W* pSin_parts_ptr;        // joint work array, one PARTS_W per joint
    VOID* pParts_tmd;                 // model (TMD) address of the weapon joint, saved by Wep_joint_set and restored to the joint by ENEMY.C
    VOID* pParts_packet;              // polygon packet address of the weapon joint, saved by Wep_joint_set and restored to the joint by ENEMY.C
    UCHAR Dummy0a4[0xAC - 0xA4];    // padding
    PARTS_W* pM_char_ptr;         // joint array drawn for the mirror reflection
    VOID* pM_option_tmd;                // address of the weapon model data used for the mirror reflection
    VOID* pM_option_packet;                // address of the weapon packet buffer used for the mirror reflection
    ENEMY_WORK* pTgt;               // entity being targeted (enemies reuse it as a group or target link)
    UCHAR Timer2;                   // general frame counter (fall and death motion, flight time of thrown objects)
    SCHAR Move_no;                  // animation sequence number
    UCHAR Move_cnt;                 // frame number within the animation sequence
    UCHAR Move_wait;                // frames the current animation frame is still held; loaded from the frame data and counted down by Joint_move
    UCHAR Dummy0c0;                 // padding, cleared on spawn
    SCHAR Dummy0c1;                 // padding, cleared on spawn
    SHORT Spd_cur;                  // current forward speed per frame
    SHORT Timer0;                   // general countdown / frame counter of the current routine step
    USHORT Root_x;                  // root anchor position x; set by a script action or by a grabbing enemy, motion offset = Pos_x - Root_x
    USHORT Root_z;                  // root anchor position z; same role as Root_x
    USHORT Scale;                   // model scale factor, 0 = unscaled
    INT Neck_point_x;               // neck look-at target x (an angle when Neck_flg 0x20 is set); EM14 and EM15 reuse it as their home position x
    INT Neck_point_y;               // neck look-at target y; EM15 reuses it as the ceiling y
    INT Neck_point_z;               // neck look-at target z; EM14 and EM15 reuse it as their home position z, PL_WEP uses it as a sweep counter
    UCHAR Neck_flg;                 // neck turn control bits; 0x10 enabled, low nibble mode, 0x20 target is an angle, 0x80 tracks pTgt
    UCHAR Neck_spd_y;               // neck turn step per frame around y (heading), low byte of the look-at script operand, default 0xC0
    UCHAR Neck_spd_x;               // neck turn step per frame around x (pitch), high byte of the look-at script operand, default 0x40
    UCHAR Set_flg_no;               // system flag number raised (Flg_on) when the scripted action finishes
    UCHAR Sca_info;                 // collision and condition bits: 0x02 poisoned, 0x20 and 0x40 higher poison levels (0x40 lethal), 0x08 wall contact this frame, 0x04 and 0x10 SCA check modes, 0x80 scripted action lock; saved as Pl_sca_info
    UCHAR Neck_no;                  // joint that turns towards the look-at target
    USHORT Sce_free0;               // script scratch word: turn amount added to Cdir_y, move speed, or a timer, set by the script move commands
    USHORT Sce_flg;                 // script control bits: 0x01 play the animation reversed (Joint_move arg), 0x02/0x10/0x20/0x80 script move modes (ENEMY.C, PL_SCE.C), 0x40 suppresses the zone camera-switch check (PLAYER.C)
    SHORT Turn_timer;               // frames until the next turn
    KAGE_WORK Kage_work;            // shadow quad work (embedded KAGE_WORK)
    VOID* pSub0_kan_t_ptr;            // joint table of the weapon (sub-motion 0) model
    VOID* pSub0_seq_t_ptr;            // sequence table of the weapon (sub-motion 0) model
    VOID* pRoom_kan_t_ptr;            // joint table of the room-specific motions (room data pEmr)
    VOID* pRoom_seq_t_ptr;            // sequence table of the room-specific motions (room data pEdd)
    VOID* pSub1_kan_t_ptr;            // joint table of the grab and knock-down motions loaded with the enemy file
    VOID* pSub1_seq_t_ptr;            // sequence table of the grab and knock-down motions
    UCHAR Poison_timer;             // frames of poison remaining
    UCHAR Max_life;                 // maximum hit points
    UCHAR Aim_flg;                  // weapon aiming state bits
    UCHAR Dummy177;                 // padding
    USHORT Aim_sts;                 // aiming state bits, set from the auto-aim target check
    UCHAR Dummy17a[6];              // padding
};

typedef VOID (*PL_FUNC)(VOID); // player animation routine

typedef ULONG PKT_BUF[0x960]; // polygon packet buffer

struct ENEMY_WORK
{
    UCHAR Be_flg;                   // slot state bits: 0x01 slot in use, 0x02 hit checks off, 0x04 wall collision off (Sca_ck), 0x08 object collision off (Oma), 0x10 last wall slide axis, 0xE0 attack class bits tested by ATTACK.C (player in range / alert)
    UCHAR Id;                       // entity type id, selects the model and behaviour routines
    UCHAR Type;                     // variant selector from the room script (low bits pick the variant, high bits are per-enemy flags); weapon index for the player
    UCHAR Zone_in_flg;              // zone/state flag bits: low 7 bits = camera-switch zone point test result (Hit_ck_point4, 0 = outside, skips shadow/drawing), bits 4-6 raised by object and attack events, bit 7 skips the rotation refresh in JOINT.C
    AT_TBL* pAt_tbl;                // collision volume table of the entity (AT_TBL array, last record has bit 15 set in Flg)
    AT_WORK* pSca_ofs;              // per-record collision centre offsets (AT_WORK array, parallel to pAt_tbl), filled by Oba_set_ofs
    UINT Attribute;                 // model object attribute flags (start of the embedded GsDOBJ5)
    GsCOORDINATE2* pCoord2;         // coordinate system of the model object (points at Coord_flg)
    VOID* pTmd;                       // model (TMD) data of the entity
    VOID* pPacket;                    // polygon packet buffer of the model
    INT Coord_flg;                  // coordinate flag word, start of the embedded GsCOORDINATE2 (cleared at init)
    UCHAR M[0x34 - 0x20];           // rotation part of the embedded GsCOORDINATE2 matrix (0x14 bytes; Pos_x/y/z that follow are its translation)
    INT Pos_x;                      // world position x
    INT Pos_y;                      // world position y
    INT Pos_z;                      // world position z
    INT Workm;                      // first word of the world matrix (Workm) of the embedded coordinate system; rest in Dummy044
    UCHAR Dummy044[0x6C - 0x44];    // rest of the embedded coordinate system (world matrix remainder and links)
    SHORT Old_x;                   // x position at the end of the previous frame
    SHORT Old_y;                   // y position at the end of the previous frame
    SHORT Old_z;                    // z position at the end of the previous frame
    SHORT Cdir_x;                  // rotation about x (model pitch)
    SHORT Cdir_y;                   // facing angle (rotation about y, 12-bit)
    SHORT Cdir_z;                   // rotation about z (roll)
    SHORT Spd_x;                    // movement step x per frame
    SHORT Spd_y;                    // movement step y per frame
    SHORT Spd_z;                    // movement step z per frame
    SHORT Old_dir;                 // facing angle (Cdir_y) of the previous frame; some enemies reuse it as a scratch value
    USHORT Aspd_x;                  // rotation speed added to Cdir_x by the script move command
    USHORT Aspd_y;                  // rotation speed added to Cdir_y by the script move command
    UCHAR Routine_0;                // main routine number (index into the per-enemy routine table)
    UCHAR Routine_1;                // step within the main routine
    UCHAR Routine_2;                // damage/action routine selector (index into the per-enemy routine table)
    UCHAR Routine_3;                // step within the selected damage/action routine
    SHORT Life;                     // hit points; negative when dead
    UCHAR Damage_no;                // hit reaction code written by the attack code, 0 = none: bits 0-2 reaction number (1-based, selects the damage routine), bits 3-6 hit class of the weapon, 0x80 set by the player fall routine
    UCHAR Dummy08b;                 // padding
    UCHAR Hokan_flg;                // pose blend steps left after an animation change
    UCHAR Parts_num;                // number of joints in the pSin_parts_ptr array
    SHORT Ground;                   // ground height used for the shadow and landing
    VOID* pKan_t_ptr;               // joint animation data (Kan table) of the model
    VOID* pSeq_t_ptr;               // animation sequence table (Seq table) of the model
    PARTS_W* pSin_parts_ptr;        // joint work array, one PARTS_W per joint
    VOID* pParts_tmd;                 // model (TMD) address of the weapon joint, saved by Wep_joint_set and restored to the joint by ENEMY.C
    VOID* pParts_packet;              // polygon packet address of the weapon joint, saved by Wep_joint_set and restored to the joint by ENEMY.C
    UCHAR Dummy0a4[0xAC - 0xA4];    // padding
    PARTS_W* pM_char_ptr;         // joint work array used for the mirror reflection
    VOID* pM_option_tmd;                // address of the weapon model data used for the mirror reflection
    VOID* pM_option_packet;                // address of the weapon packet buffer used for the mirror reflection
    ENEMY_WORK* pTgt;               // pointer to the entity tracked or aimed at (head-turn target, player aim target); EM08 reuses it for the first web of a group, EM0D casts it to PARTS_W*
    UCHAR Timer2;                   // general frame counter; each enemy uses it differently (death delay, jump frames)
    UCHAR Move_no;                  // animation sequence index
    UCHAR Move_cnt;                 // frame index within the animation sequence
    UCHAR Move_wait;                // frames left before the next animation frame is read
    UCHAR Dummy0c0;                 // padding
    SCHAR Dummy0c1;                 // unused, cleared at spawn
    SHORT Spd_cur;                  // current forward speed per frame (Walk_spd_set, scaled per animation frame)
    SHORT Timer0;                   // general countdown frame counter of the current action step
    USHORT Root_x;                  // x of the animation root anchor (position minus the root offset)
    USHORT Root_z;                  // z of the animation root anchor
    USHORT Scale;                   // model scale factor, 0 = unscaled
    INT Neck_point_x;               // neck look-at target x (an angle when Neck_flg 0x20 is set); EM14 and EM15 reuse it as their home position x
    INT Neck_point_y;               // neck look-at target y; EM15 reuses it as the ceiling y
    INT Neck_point_z;               // neck look-at target z; EM14 and EM15 reuse it as their home position z, PL_WEP uses it as a sweep counter
    UCHAR Neck_flg;                 // neck turn control bits: 4 enabled, 0 yaw follows target, 5 target is an angle, 6 sweep, 7 track the aim target
    UCHAR Neck_spd_y;               // neck turn step per frame around y (heading), low byte of the look-at script operand, default 0xC0
    UCHAR Neck_spd_x;               // neck turn step per frame around x (pitch), high byte of the look-at script operand, default 0x40
    UCHAR Set_flg_no;               // system flag number raised when the scripted action finishes
    UCHAR Sca_info;                 // collision and condition bits: 0x02 poisoned, 0x20 and 0x40 higher poison levels (0x40 lethal), 0x08 wall contact this frame, 0x04 and 0x10 SCA check modes, 0x80 scripted action lock; saved as Pl_sca_info
    UCHAR Neck_no;                  // index of the joint turned by the neck turn
    USHORT Sce_free0;               // scene-event scratch word set by script move commands (movement speed or turn amount per frame)
    USHORT Sce_flg;                 // scripted-event control bits: 0 play animation reversed, 1/4/5/7 script move modes, 6 hide shadow
    SHORT Turn_timer;               // frames left before the next turn is allowed
    KAGE_WORK Kage_work;            // ground shadow work (embedded KAGE_WORK)
    VOID* pKage_work;                 // pointer to an extra KAGE_WORK allocated from the load pool (EM00 head shadow)
    UCHAR Dummy160;                 // padding
    SCHAR Slot_info;                // enemy slot in the low nibble, sound set in bits 4-6, bit 7 = not saved when leaving the room
    UCHAR Dummy162;                 // padding
    UCHAR Em_set_flg;               // event flag number raised when killed, 0xFF = none
    UCHAR Sikai_cnt;                // line-of-sight check counter in the low 5 bits, bit 5 = out of sight
    UCHAR Dummy165;                 // padding
    SHORT Dest_x;                  // x of the last known target position to walk toward
    SHORT Dest_z;                  // z of the last known target position to walk toward
    UCHAR Dummy16a;                 // padding
    UCHAR Step_sw;                  // 1 = run the animation step twice per frame (zombie variants 0xC-0xE)
    USHORT Free[16];                // enemy-specific work words, layout differs per enemy type (overlay _FREE structs)
};

// enemy-specific work area (ENEMY_WORK.Free, offset 0x16C) of the enemy p; cast it to the unit's EMxx_FREE struct
#define ENEMY_FREE(p) ((UCHAR*)((ENEMY_WORK*)(p))->Free)

// byte offset of a member in a struct
#define OFFSET_OF(T, member) ((INT)&((T*)0)->member)
#define BYTE_OFS(T, base, ofs) ((T*)((UCHAR*)(base) + (ofs)))      // pointer to a T that lies ofs bytes after base (a sum on a typed pointer would step in units of its own type)

// a member of the current enemy's work area (G.pEm) given as its unit's EMxx_FREE struct
#define ENEMY_FREE_FIELD(T, field) (*(__typeof__(((T*)0)->field)*)(ENEMY_FREE(G.pEm) + OFFSET_OF(T, field)))

struct _CARD
{
    UCHAR magic[2];     // card file signature
    UCHAR type;         // icon display flag
    UCHAR blockEntry;   // number of card blocks used
    CHAR title[64];     // save title text
    UCHAR reserve[28];  // reserved, zero
    UCHAR clut[32];     // icon palette
    UCHAR icon[3][128]; // icon frames
};

struct ITEM_WORK
{
    UCHAR Id;   // item id, 0 = empty slot
    UCHAR Num;  // item quantity
};

struct SCE_TASK
{
    UCHAR Routine0;     // opcode handler set: 0 general commands, 1 animation setup (Evt_anm_set), 2 speed setup (Evt_spd_set), 3 move setup (Evt_move_set)
    UCHAR Dummy01;      // padding, cleared together with Routine0 by Event_init and Evt commands 0x80/0x8B
    UCHAR Status;       // 0 = task idle, 1 = task running (set by Event_init, cleared by opcode 0xFF)
    SCHAR Sub_ctr;      // nesting depth of the script calls and loops, -1 = empty
    ENEMY_WORK* pWork;  // entity (ENEMY_WORK) the script drives
    UCHAR* Data;        // script program counter
    UCHAR* pRet[4];     // per-level script position: loop start (opcodes 0xFA/0xFB) or resume position of a call block (0xFC/0xFD)
    UCHAR* Call_ptr[4]; // per-level pointer to the command a call block (0xFC/0xFD) runs each frame
    SHORT Loop_ctr[4];  // loop counter per nesting level
};

struct ESP_SPR_TBL
{
    INT Tpage_cur;      // texture page cursor: page of the next effect sprite upload (0x18 at start)
    USHORT Tex_x;       // texture x cursor of the next upload (advances by 0x40 per page column)
    USHORT Tex_y;       // texture y cursor of the next upload (advances by the image height)
    USHORT Clut_x;      // palette column cursor of the next upload
    USHORT Clut_y;      // palette row cursor of the next upload (starts at 4)
    UCHAR Espid[16];    // effect sprite set ids, 0xFF = end
    INT pEspdt[50];     // sprite data pointer per effect id
    INT pEspmv[50];     // behaviour (move) data pointer per effect id
};

struct SAVE_DATA
{
    _CARD Card_Head;            // memory card file header
    UCHAR Stage_no;             // current stage number
    UCHAR Room_no;              // current room number
    UCHAR Cut_no;               // current camera cut number
    UCHAR Room_no_old;          // previous room number
    UCHAR Cut_bak;              // camera cut saved while a screen is open; restored afterwards
    UCHAR Mess_flg;             // message window flags; bit 7 = window active, bit 0 = chosen answer
    UCHAR Sel_id;               // item selected by the player
    UCHAR Item_num;             // number of Item_work slots in use
    UCHAR Fade1_hrate;          // semi-transparency rate of the second fade box (Fade_box.attribute bits 28-29)
    UCHAR Pl_chg_no;            // player model number to load on a character change
    UCHAR Em_flg_bak;           // behavior flags of the enemy slot read by the script command
    UCHAR Tama_esp;             // effect id of the bullet effect started by the script
    UCHAR Unk_20c[3];           // three bytes initialised to 7, 0xF0, 0xF0 at game start; never read by name
    UCHAR Dummy20f;             // padding
    UCHAR Front_aot_no;         // index of the room action entry in range ahead of the player
    UCHAR Em_aot_no;            // index of the room action entry in range at the enemy position
    UCHAR Use_id;               // item used on the room action
    UCHAR Get_id;               // item picked up
    SHORT Fade_kido;            // main screen fade brightness, negative = no fade running
    SHORT Fade1_kido;           // second fade box brightness, negative = off
    SHORT Fade1_add;            // second fade box brightness change per frame
    USHORT Sce_random;           // random value refreshed every frame by rand() (unsigned: read with an unsigned halfword load)
    USHORT Count_down_timer;     // elapsed-second counter, +1 every 30 frames while not paused (System_flg SYS_GAME_OVER); a value of 0x7FFE or more (0x7FFF forces it at once) wraps to 0; the countdown display shows 180 minus it (unsigned: read with an unsigned halfword load)
    SHORT Pl_life;              // player health mirror, copied from the player every frame and restored on load
    USHORT Key;                 // mapped (key-config) buttons held this frame
    USHORT Key_trg;             // mapped buttons newly pressed this frame
    UINT Game_play_time;        // play time in frames
    UCHAR Save_cnt;             // number of saves
    UCHAR Equip_no;             // equipped Item_work slot, 1-based
    UCHAR Equip_bak;            // equipped slot of the other character, swapped on a character change
    UCHAR Pl_id;                // player character id
    SHORT Pl_pos_x;             // saved player position x
    SHORT Pl_pos_z;             // saved player position z
    SHORT Pl_cdir_y;            // saved player facing angle
    UCHAR Pl_sca_info;          // saved player collision bits
    UCHAR Dc_mode;              // Director's Cut mode chosen on the title screen: 0 normal, 1-3 mode variants (status bits 0x40000, 0x20000, 0x20000|0x10000)
    UCHAR Scenario_flg2[32];    // scenario flag bits, set 2 (script flag type 1)
    UCHAR Key_flg[8];           // door lock flag bits (script flag type 2)
    UCHAR Enemy_flg[32];        // killed enemy flag bits (script flag type 3)
    UCHAR Item_flg[32];         // room item taken flag bits (script flag type 7)
    UCHAR Item_c_flg[4];        // item examined flag bits
    UCHAR Scenario_flg[16];     // scenario progress flag bits (script flag type 0)
    UCHAR Room_flg[20];         // room visited flag bits (script flag type 8)
    ITEM_WORK Item_boxwork[48]; // item box slots
    ITEM_WORK Item_work[6];     // held item slots of the current character
    ITEM_WORK Item_work2[6];    // held item slots of the other character, swapped in on a character change
    UCHAR Bgm_tbl[224];         // background music id table, 32 rooms per stage
    // key configuration
    USHORT Key_tbl[16];         // button masks of the active key config, one per key action
    UCHAR Key_idx;              // key config byte: bits 0-1 preset index, bit 6 option, bit 7 stereo off
    UCHAR Dummy21[3];           // padding
};

struct SCENARIO
{
    SCHAR Vab_id[5];            // sound bank (VAB) ids, -1 = not loaded
    SCHAR Bgm_fade_add;         // volume step added to Main_vol each frame of a BGM fade (the time argument of Snd_bgm_fade_ON)
    UCHAR nEsp_free;            // count of free effect slots (0x40 on reset)
    SCHAR Sep_no;               // sequence access number of the playing SEP, -1 = none
    UCHAR Bgm_tgt_sts;          // requested background music state, 0xFF = none
    UCHAR Bgm_sts_old;          // background music state before the request
    UCHAR Dummy0a[6];           // padding (6 bytes), not accessed
    USHORT Proc_flg_bak;        // saved Stop_flg across pause and door transition, 0xFFFF = none
    SCHAR Ifel_ctr;             // depth of the if/else branch stack (Ifel_stk), 0 = none
    UCHAR Movie_id;             // movie to play
    VOID* pMovie_buf;           // movie stream buffer
    UCHAR* pSce_data;           // script instruction pointer
    UCHAR** ppSce_sp;           // next free slot of Ifel_stk
    UCHAR* Ifel_stk[16];        // if/else branch target stack (16 entries), depth in Ifel_ctr
    SCE_TASK* pSce_cur;         // script task being run
    SCE_TASK Sce_task[8];       // script event tasks, SCE_TASK each
    TASK* pTask_cur;            // task being run by the scheduler
    INT Main_gp;                // global pointer of the main program
};

struct GLOBAL
{
    INT System_flg;             // system state bits, script flag type 5: SYS_* bits (FLAG.H)
    INT Status_flg;            // scene status and system mode bits: water, quake, difficulty mode, demo playback, countdown shown and others (STAT_* bits, FLAG.H)
    UINT Room_flg[2];           // room flag bit vector (script flag type 4): ROOM_* indices, word 0 is cleared per room, word 1 every frame, see FLAG.H
    UCHAR Cbuf_id;              // display buffer index being drawn, 0 or 1
    UCHAR Clr_r;                // screen clear color red
    UCHAR Clr_g;                // screen clear color green
    UCHAR Clr_b;                // screen clear color blue
    UCHAR Curtain_kido;         // screen dim level
    UCHAR Reset_cnt;            // frames the soft reset buttons were held
    UCHAR Demo_idx;             // index of the next demo to play, wraps at 3; accessed as USHORT
    UCHAR Dummy17[5];           // padding cleared on reset
    UCHAR Vloop;                // vertical sync wait count per frame
    UCHAR Dummy1d[3];           // padding
    SCHAR Def_item;             // empty item slot, returned by Search_item_id when the item is not held (id)
    SCHAR Def_item_num;         // empty item slot, quantity
    UCHAR Enemy_num;            // number of enemy slots in use
    UCHAR Dummy23;              // padding
    ESP_WORK Esp[64];           // effect work slots, ESP_WORK each
    PLAYER_WORK Player;         // player work
    ENEMY_WORK Enemy[30];       // enemy work array
    UCHAR Em_save_buf[0x348];   // saved enemy table kept across rooms, CORPSE_SAVE_WORK entries
    SCHAR Die_rno;              // player death sequence state
    SCHAR Evt_use;              // set when an item was used on an event
    UCHAR Typewriter_sts;       // typewriter menu state
    UCHAR Ibox_sts;             // item box menu state
    UCHAR Desk_sts;             // desk examine state
    UCHAR Check_sum_flg;        // 1 skips the checksum test of disc reads
    UCHAR Dummy06;              // padding, not accessed
    UCHAR Menu_req;             // menu open state: 1 requested, 2 and 3 follow-up steps, 0 idle
    SCHAR Disp_y;               // y origin of the display buffer
    UCHAR Mask_r;               // red mask of the second fade box
    UCHAR Mask_g;               // green mask of the second fade box
    UCHAR Mask_b;               // blue mask of the second fade box
    SAVE_DATA SaveData;         // save data block: card file header and the saved game state
    GsOT Ot0[2];                // near layer ordering tables, one per display buffer
    GsOT Ot1[2];                // main layer ordering tables, one per display buffer
    GsOT Ot2[2];                // far layer ordering tables, one per display buffer
    GsOT_TAG Ot1_tag[2][1024];  // tag storage of Ot1
    GsOT_TAG Ot2_tag[2][16];    // tag storage of Ot2
    GsOT_TAG Ot0_tag[2][16];    // tag storage of Ot0
    PL_FUNC Pl_move[57];      // player animation routines
    IN_DOOR_WORK* pSce_aot_on;  // door record the player is inside, set by Sce_Door
    USHORT Stop_flg;            // process enable bits (STOP_* in FLAG.H, script flag type 6): a set bit lets that part of the game run
    USHORT Stop_keep;           // Stop_flg value saved while a message is open
    union
    {
        USHORT Tpage_clut; // texture page and palette cursors as one word
        struct
        {
            UCHAR Ctpage; // next free texture page
            SCHAR Cclut;  // next free palette row
        } b;              // byte view of the cursors
    } Vram_cur;                 // texture page and palette cursors
    USHORT Tpage_clut_bak;      // Vram_cur saved after the room load, restored by Cut_set, low byte 0 = nothing saved
    UCHAR Dummy0c;              // padding
    UCHAR Em_id_old;            // id of the enemy whose model was loaded last, 0xFF = none; equal ids share the model buffers
    SHORT Fade_add;             // screen fade step per frame
    VOID* Cccut;                // current camera cut switch record (VCUT), advanced as the cut changes; also the polygon for zone point tests
    ROOM_WORK* pRoom;           // room data loaded from the RDT file
    ENEMY_WORK* pEm;            // enemy (or player) work being updated
    CORPSE_SAVE_WORK* Em_save_cur;            // current entry of the saved enemy table Em_save_buf, advanced as entries are written
    VOID* pFree_work;           // next free address of the load area, advanced by every load
    UINT* pFree_work_head;      // start of the load area as left by the stage load
    UCHAR Dummy28[4];           // padding
    PKT_BUF Pkt_buf[2];         // polygon packet buffers, one per display buffer
    SCENARIO Scenario;          // sound bank, script interpreter state, event tasks, scheduler task and gp
    INT Bg_img_mode;            // background image mode, 1 = alternate buffer
    VOID* pBg_buf_a;            // background image buffer a
    VOID* pBg_buf_b;            // background image buffer b
    UINT Dummy14;               // not accessed
    UINT Padd;                  // raw pad word of this frame
    UINT Padd_trg;              // buttons newly pressed this frame; holds the previous raw word until it is computed
    USHORT Padd1;               // pad 1 buttons held this frame, low 16 bits of Padd
    USHORT Padd2;               // pad 2 buttons held this frame, high 16 bits of Padd
    USHORT Padd1_trg;           // pad 1 buttons newly pressed, low 16 bits of Padd_trg
    USHORT Padd2_trg;           // pad 2 buttons newly pressed, high 16 bits of Padd_trg
    USHORT Key_old;             // mapped buttons (Key) held on the previous frame
    UCHAR Mess_r_no0;           // message window routine number
    UCHAR Mess_type;            // 0x80 = fast text
    USHORT Stop_data;           // Stop_flg bits stopped while the message is shown
    USHORT Pos_y_def;           // message window y position
    UCHAR* Mess_top;            // start of the message text
    UCHAR* Mess_end;            // read position in the message text
    UCHAR* Mess_svp;            // return position after a text jump
    UCHAR Dummy3c;              // not accessed
    UCHAR Mess_wait;            // frames per character
    UCHAR Mess_time;            // frames left until the next character
    UCHAR Mcol_init;            // text color in use
    UCHAR Mcol_next;            // text color set by the script, applied at the next character
    SCHAR Fade_rtn;             // background music fade state; 0 idle, negative counts a fade out
    UCHAR Esp_idx;              // effect slot being processed
    UCHAR Fade_hrate;           // semi-transparency rate of the main fade box (Fade_box.attribute bits 28-29)
    UCHAR* Bgm_tbl_adr;            // address of the current stage rows of the music table (Bgm_tbl)
    VOID* pTama_m;              // parent matrix of the bullet effect
    ESP_SPR_TBL Esp_spr;        // effect sprite table
};

extern GLOBAL G; // fixed work area shared by all programs
extern SHORT Scr_ofs[];       // screen origin offset in pixels: [0] = x, [1] = y
extern UCHAR Door_anim_id[2]; // script variable pair: [0] id of the door animation being played, [1] door texture index
#define MAIN (*(GLOBAL*)0x800C3000) // the same object addressed as a constant, where the code shares the base address register
#define G_BASE ((GLOBAL*)(VOID*)&G) // G as a pointer: GCC 2.6.3 keeps the address of G in a base register and indexes it, where G.member would fold the offset into the symbol
#define G_PLAYER (&G_BASE->Player)  // the player work through the G_BASE pointer form (see G_BASE)

// Typed views of fields that the code reads or writes with another type. A plain member access, or a value cast, makes GCC 2.6.3 schedule
// or reload differently, so the field is accessed as an lvalue of the other type (same address, same width as the original code).
#define FREE_WORK_INT (*(INT*)&MAIN.pFree_work)                 // the load pointer pFree_work as an integer address
#define FREE_WORK_UINT (*(UINT*)&MAIN.pFree_work)               // the same, unsigned
#define SCE_DATA(T) (*(T**)&MAIN.Scenario.pSce_data)            // the script instruction pointer as a pointer to the record or word type T
#define SCE_DATA_NEXT(pRec, ofs) (MAIN.Scenario.pSce_data = (UCHAR*)(pRec) + (ofs)) // the script instruction pointer after the command record pRec: byte offset ofs from its start
#define VIEW(T, lv) (*(T*)&(lv))                                        // the lvalue lv re-typed in place as a T: same address and width, accessed as another type (a plain access changes how GCC 2.6.3 schedules or reloads it)
#define VIEW_VIA_CHAR(T, lv) (*(T*)(CHAR*)&(lv))                        // like VIEW, through a CHAR pointer: GCC 2.6.3 then neither reuses an earlier load of lv nor keeps this one in a register (plain VIEW changes the code)
#ifdef __INTELLISENSE__   // IntelliSense (C mode) knows no GNU statement expressions, asm labels or labels before "}"; GCC 2.6.3 never defines it
#define KEEP_VALUE(x) (x)
#define LABEL_END ;
#define ASM_NAME(n)
#else
#define KEEP_VALUE(x) ({ x; })                                          // x as a separate value: GCC 2.6.3 does not fold it into the surrounding expression (the plain form and a cached local change the code)
#define LABEL_END
#define ASM_NAME(n) __asm__(n)
#endif
// Routine_0..Routine_3 of a work record (enemy, player or script task work) are four byte members that the code sets and compares as one
// word or as two halfwords (Routine_0 in the low byte). The macros access them as the wider type, so the code is one load or store; the
// members written one by one compile to different code. Variants: WORD, HALF0 (Routine_0, Routine_1), HALF2 (Routine_2, Routine_3),
// UHALF0 and UHALF2 (unsigned halfwords).
#define ROUTINE_WORD(pWork) (*(INT*)&(pWork)->Routine_0)
#define ROUTINE_HALF0(pWork) (*(SHORT*)&(pWork)->Routine_0)
#define ROUTINE_HALF2(pWork) (*(SHORT*)&(pWork)->Routine_2)
#define ROUTINE_UHALF0(pWork) (*(USHORT*)&(pWork)->Routine_0)
#define ROUTINE_UHALF2(pWork) (*(USHORT*)&(pWork)->Routine_2)
// Routine_3..Routine_0 packed into the one word that is stored over Routine_0 (Routine_0 the low byte); the value folds to a constant
#define ROUTINES(r3, r2, r1, r0) ((r3) << 24 | (r2) << 16 | (r1) << 8 | (r0))
// the same for the work record that the global pointers select
#define EM_ROUTINE_WORD ROUTINE_WORD(G.pEm)                     // the current enemy
#define EM_ROUTINE_HALF0 ROUTINE_HALF0(G.pEm)
#define EM_ROUTINE_HALF2 ROUTINE_HALF2(G.pEm)
#define EM_ROUTINE_UHALF0 ROUTINE_UHALF0(G.pEm)
#define EM_ROUTINE_UHALF2 ROUTINE_UHALF2(G.pEm)
#define MAIN_EM_ROUTINE_WORD ROUTINE_WORD(MAIN.pEm)             // the current enemy, through the MAIN alias
#define EM_TGT_ROUTINE_WORD ROUTINE_WORD(G.pEm->pTgt)           // the target of the current enemy
#define ENEMY_ROUTINE_WORD(n) ROUTINE_WORD(&G.Enemy[n])         // enemy slot n
#define PL_ROUTINE_WORD ROUTINE_WORD(&G.Player)                 // the player
#define PL_ROUTINE_HALF0 ROUTINE_HALF0(&G.Player)
#define PL_ROUTINE_HALF2 ROUTINE_HALF2(&G.Player)
#define PL_ROUTINE_UHALF2 ROUTINE_UHALF2(&G.Player)
#define MAIN_PL_ROUTINE_HALF2 ROUTINE_HALF2(&MAIN.Player)       // the player, through the MAIN alias
#define SCE_CUR_ROUTINE_UHALF0 ROUTINE_UHALF0(MAIN.Scenario.pSce_cur->pWork) // the work of the running script task
#define SCE_CUR_ROUTINE_UHALF2 ROUTINE_UHALF2(MAIN.Scenario.pSce_cur->pWork)
#define MAIN_PEM(T) (*(T**)&MAIN.pEm)                           // the current work pointer pEm re-typed in place as a pointer to T
#define G_PEM_BYTE(T) (*(T**)(CHAR*)&G.pEm)                     // the current work pointer pEm re-typed in place as a pointer to T, written through a byte pointer (GCC 2.6.3 generates different code for G_PEM)
#define G_PEM(T) (*(T**)&G.pEm)                                 // the current work pointer pEm (through G) re-typed in place as a pointer to T
#define EM_BE_FLG (*(*(UCHAR**)&G.pEm))                         // Be_flg (first byte) of the current enemy, written through a byte pointer view of pEm
#define MAIN_PEM_INT (*(INT*)&MAIN.pEm)                         // the current work pointer pEm as an integer address
#define MAIN_PROOM_INT (*(INT*)&MAIN.pRoom)                     // the room data pointer pRoom as an integer address
#define MAIN_PROOM(T) (*(T**)&MAIN.pRoom)                       // the room data pointer pRoom re-typed in place as a pointer to T
#define STAGE_ROOM (*(USHORT*)&G.SaveData.Stage_no)             // Stage_no (low byte) and Room_no (high byte) as one halfword, compared as (room << 8 | stage)
#define PL_MOVE_WORD (*(INT*)&G.Player.Timer2)                  // Timer2 (low byte), Move_no, Move_cnt and Move_wait of the player as one word
#define PL_MOVE_NO_UBYTE (*(UCHAR*)&G.Player.Move_no)           // Move_no of the player as an unsigned byte (the member is SCHAR)
#define POS_VEC(p) ((VECTOR*)&(p)->Pos_x)                         // the position words Pos_x, Pos_y, Pos_z of an entity record as a VECTOR
#define MATRIX_OF(p) ((MATRIX*)(p)->M)                          // the matrix of an entity record: its M bytes, followed by Pos_x, Pos_y, Pos_z as the translation
#define PL_POS_VEC (*(VECTOR*)&MAIN.Player.Pos_x)               // Pos_x, Pos_y, Pos_z and the word after them of the player as a VECTOR, through the MAIN alias
#define EM_POS_VEC (*(VECTOR*)&G.pEm->Pos_x)                    // the same four words of the current enemy
#define MAIN_EM_POS_VEC (*(VECTOR*)&MAIN.pEm->Pos_x)            // the same, through the MAIN alias
#define PL_EM ((ENEMY_WORK*)&MAIN.Player.Be_flg)                // the player work read as an ENEMY_WORK (the two records share the layout of their common fields), through the MAIN alias
#define SCE_DATA_WORD (*(INT*)&MAIN.Scenario.pSce_data)         // the script instruction pointer pSce_data as an integer address
#define SCE_SP_WORD (*(INT*)&MAIN.Scenario.ppSce_sp)            // the script stack pointer ppSce_sp as an integer address
#define PL_M_HALF(i) (*(SHORT*)&G.Player.M[i])                  // halfword at byte offset i of the rotation matrix M of the player (M is a byte array)
#define PL_OLD_Z_SHORT (*(SHORT*)&G.Player.Old_z)               // Old_z of the player as a signed halfword (the member is USHORT)
#define DEMO_IDX_HALF (*(USHORT*)&MAIN.Demo_idx)                // Demo_idx as an unsigned halfword (the member is a byte)
#define CCLUT_UBYTE_CUR (*(UCHAR*)&MAIN.Vram_cur.b.Cclut)       // next free palette row as an unsigned byte (the member is SCHAR)
#define CCLUT_UBYTE_NOW (*(UCHAR*)&G.Vram_cur.b.Cclut)          // the same, through G

// The same field read through its own address (*&X): GCC 2.6.3 keeps the access separate from the struct member (a plain member access
// changes scheduling and register allocation). Names ending in _CUR are spelled through the MAIN alias, names ending in _NOW through G.
#define EM_CUR (*&MAIN.pEm)                                     // the current enemy or player work pointer
#define EM_NOW (*&G.pEm)                                        // the same, through G
#define SCE_CUR (*&MAIN.Scenario.pSce_cur)                      // the script task being run
#define SCE_NOW (*&G.Scenario.pSce_cur)                         // the same, through G
#define SCE_DATA_CUR (*&MAIN.Scenario.pSce_data)                // the script instruction pointer
#define SCE_DATA_NOW (*&G.Scenario.pSce_data)                   // the same, through G
#define ROOM_CUR (*&MAIN.pRoom)                                 // the room data pointer
#define ROOM_NOW (*&G.pRoom)                                    // the same, through G
#define FREE_WORK_CUR (*&MAIN.pFree_work)                       // the next free address of the load area
#define STOP_FLG_CUR (*&MAIN.Stop_flg)                          // the Stop_flg word
#define STATUS_FLG_CUR (*&MAIN.Status_flg)                      // the Status_flg word
#define STATUS_FLG_NOW (*&G.Status_flg)                         // the same, through G
#define CBUF_ID_CUR (*&MAIN.Cbuf_id)                            // the display buffer index being drawn
#define SEP_NO_CUR (*&MAIN.Scenario.Sep_no)                     // the sequence access number of the playing SEP
#define VAB_ID_CUR(n) (*&MAIN.Scenario.Vab_id[n])               // sound bank id n
#define CUT_NO_CUR (*&MAIN.SaveData.Cut_no)                     // the current camera cut number
#define CUT_NO_NOW (*&G.SaveData.Cut_no)                        // the same, through G
#define KEY_IDX_CUR (*&MAIN.SaveData.Key_idx)                   // the key config byte
#define PL_BE_FLG_CUR (*&MAIN.Player.Be_flg)                    // the state bits of the player
#define SEL_ID_CUR (*&MAIN.SaveData.Sel_id)                     // the item selected by the player
#define EQUIP_NO_CUR (*&MAIN.SaveData.Equip_no)                 // the equipped Item_work slot
#define ITEM_NUM_CUR (*&MAIN.SaveData.Item_num)                 // the number of Item_work slots in use
#define ROOM_NO_CUR (*&MAIN.SaveData.Room_no)                   // the current room number
#define MESS_FLG_CUR (*&MAIN.SaveData.Mess_flg)                 // the message window flags
#define KEY_TRG_CUR (*&MAIN.SaveData.Key_trg)                   // the mapped buttons newly pressed this frame
#define CCCUT_CUR (*&MAIN.Cccut)                                // the current camera cut switch record
#define KEY_TBL_CUR (*&MAIN.SaveData.Key_tbl)                   // the active key config table as an array; the plain Key_tbl[] access changes the code
#define ITEM_WORK_TBL (*&MAIN.SaveData.Item_work)               // the held item slots as an array; the plain Item_work[] access and a pointer cast both change the code
#define KEY_CUR (*&MAIN.SaveData.Key)                                   // the mapped (key-config) buttons held this frame
#define KEY_NOW (*&G.SaveData.Key)                                      // the same, through G
#define FADE_KIDO_CUR (*&MAIN.SaveData.Fade_kido)                       // the main screen fade brightness
#define MESS_TIME_CUR (*&MAIN.Mess_time)                                // frames left until the next message character
#define MESS_WAIT_CUR (*&MAIN.Mess_wait)                                // frames per message character
#define MESS_END_CUR (*&MAIN.Mess_end)                                  // the read position in the message text
#define MCOL_INIT_CUR (*&MAIN.Mcol_init)                                // the text color in use
#define PL_CHG_NO_CUR (*&MAIN.SaveData.Pl_chg_no)                       // the player model number to load on a character change
#define PADD1_CUR (*&MAIN.Padd1)                                        // the pad 1 buttons held this frame
#define PADD1_NOW (*&G.Padd1)                                           // the same, through G
#define PADD_CUR (*&MAIN.Padd)                                          // the raw pad word of this frame
#define PADD_TRG_CUR (*&MAIN.Padd_trg)                                  // the buttons newly pressed this frame
#define PADD1_TRG_CUR (*&MAIN.Padd1_trg)                                // the pad 1 buttons newly pressed this frame
#define KEY_OLD_CUR (*&MAIN.Key_old)                                    // the mapped buttons held on the previous frame
#define MON_ID_MAT (*&Monitor.pId_mat)                                  // the identity matrix of the monitor block (read through the address of its pointer)
#define MON_WS_MAT (*&Monitor.pWs_mat)                                  // the world-screen matrix of the monitor block
#define MON_BGM_ROOM_TBL (*&Monitor.pBgm_room_tbl)                      // the room background music table of the stage
#define CTPAGE_CUR (*&MAIN.Vram_cur.b.Ctpage)                           // the next free texture page
#define CTPAGE_NOW (*&G.Vram_cur.b.Ctpage)                              // the same, through G
#define CCLUT_CUR (*&MAIN.Vram_cur.b.Cclut)                             // the next free palette row
#define TPAGE_CLUT_CUR (*&MAIN.Vram_cur.Tpage_clut)                     // the texture page and palette cursors as one word
#define SYSTEM_FLG_CUR (*&MAIN.System_flg)                              // the system state bits
#define SYSTEM_FLG_NOW (*&G.System_flg)                                 // the same, through G
#define IFEL_CTR_CUR (*&MAIN.Scenario.Ifel_ctr)                         // the depth of the script if/else branch stack
#define SCE_SP_CUR (*&MAIN.Scenario.ppSce_sp)                           // the script stack pointer
#define BGM_TGT_STS_CUR (*&MAIN.Scenario.Bgm_tgt_sts)                   // the requested background music state
#define BGM_STS_OLD_CUR (*&MAIN.Scenario.Bgm_sts_old)                   // the background music state before the request
#define DC_MODE_CUR (*&MAIN.SaveData.Dc_mode)                           // the Director's Cut mode chosen on the title screen
#define ENEMY_NUM_CUR (*&MAIN.Enemy_num)                                // the number of enemy slots in use
#define CHECK_SUM_FLG_CUR (*&MAIN.Check_sum_flg)                        // 1 skips the checksum test of disc reads
#define KEY_IDX_NOW (*&G.SaveData.Key_idx)                              // the key config byte, through G
#define STAGE_NO_CUR (*&MAIN.SaveData.Stage_no)                         // the current stage number
#define STAGE_NO_NOW (*&G.SaveData.Stage_no)                            // the same, through G
#define SCE_RANDOM_CUR (*&MAIN.SaveData.Sce_random)                     // the random value refreshed every frame
#define SAVE_CNT_CUR (*&MAIN.SaveData.Save_cnt)                         // the number of saves
#define GAME_PLAY_TIME_CUR (*&MAIN.SaveData.Game_play_time)             // the play time in frames
#define COUNT_DOWN_TIMER_CUR (*&MAIN.SaveData.Count_down_timer)         // the elapsed-second counter of the countdown
#define RESET_CNT_CUR (*&MAIN.Reset_cnt)                                // the frames the soft reset buttons were held
#define OT0_CUR (*&MAIN.Ot0)                                            // the near layer ordering tables, one per display buffer
#define IBOX_STS_CUR (*&MAIN.Ibox_sts)                                  // the item box menu state
#define DESK_STS_CUR (*&MAIN.Desk_sts)                                  // the desk examine state
#define TYPEWRITER_STS_CUR (*&MAIN.Typewriter_sts)                      // the typewriter menu state
#define ESPID_CUR (*&MAIN.Esp_spr.Espid)                                // the effect sprite set ids, 0xFF = end
#define EM_ID_OLD_CUR (*&MAIN.Em_id_old)                                // the id of the enemy whose model was loaded last
#define CURTAIN_KIDO_CUR (*&MAIN.Curtain_kido)                          // the screen dim level
#define SCE_AOT_ON_CUR (*&MAIN.pSce_aot_on)                             // the door record the player is inside
#define PL_LIFE_CUR (*&MAIN.SaveData.Pl_life)                           // the player health mirror
#define ITEM_BOXWORK_CUR (*&MAIN.SaveData.Item_boxwork)                 // the item box slots
#define GET_ID_CUR (*&MAIN.SaveData.Get_id)                             // the item picked up
#define FADE1_KIDO_CUR (*&MAIN.SaveData.Fade1_kido)                     // the second fade box brightness
#define EQUIP_BAK_CUR (*&MAIN.SaveData.Equip_bak)                       // the equipped slot of the other character
#define PL_DAMAGE_NO_CUR (*&MAIN.Player.Damage_no)                      // the hit reaction code of the player
#define MENU_REQ_CUR (*&MAIN.Menu_req)                                  // the menu open state
#define MASK_R_CUR (*&MAIN.Mask_r)                                      // the red mask of the second fade box
#define MASK_G_CUR (*&MAIN.Mask_g)                                      // the green mask of the second fade box
#define MASK_B_CUR (*&MAIN.Mask_b)                                      // the blue mask of the second fade box
#define FADE_RTN_CUR (*&MAIN.Fade_rtn)                                  // the background music fade state
#define FADE_ADD_CUR (*&MAIN.Fade_add)                                  // the screen fade step per frame
#define EVT_USE_CUR (*&MAIN.Evt_use)                                    // set when an item was used on an event
#define BGM_TBL_ADR_CUR (*&MAIN.Bgm_tbl_adr)                            // the address of the current stage rows of the music table
#define VLOOP_CUR (*&MAIN.Vloop)                                        // the vertical sync wait count per frame
#define KEY_TRG_NOW (*&G.SaveData.Key_trg)                              // the same as KEY_TRG_CUR, through G
#define CCCUT_NOW (*&G.Cccut)                                           // the same as CCCUT_CUR, through G

struct fpos
{
    UINT size;      // byte size of the file
    USHORT sector;  // low 16 bits of the start sector
    UCHAR sec_high; // high 8 bits of the start sector
    UCHAR sum;      // file checksum
};

extern fpos floc[]; // CD file table: size, start sector and checksum of every file

struct AT_WORK
{
    SHORT Off_x;    // collision centre offset from the entity position, x (rotated by the facing angle)
    SHORT Off_y;    // collision centre offset from the entity position, y
    SHORT Off_z;    // collision centre offset from the entity position, z (rotated by the facing angle)
};

struct AT_TBL
{
    SHORT Flg;     // record flags; bit 15 set = last record of the array (0x8000 = single record)
    SHORT Off_x;    // collision centre offset x
    SHORT Off_y;    // collision centre offset y
    SHORT Off_z;    // collision centre offset z
    USHORT H;       // half height of the collision volume (vertical overlap test)
    USHORT At_w;    // collision half width (circle radius on the xz plane)
};

struct PARTS_W
{
    UCHAR Be_flg;   // joint flag bits: 0 part present, bits 0-1 cleared when a detached part lands, 4 keeps the rotation from being taken from the animation
    UCHAR Parts_no; // joint number
    UCHAR Fall_cnt; // frame counter of the detached-part fall; bit 7 = finished
    UCHAR Fall_sts; // detached-part state; bit 7 = landed, bits 6/7 flip bounce direction, also the duration limit in the fly-off effect
    SVECTOR Cdir;   // joint rotation angles (SVECTOR); detached parts reuse it as a speed vector
    INT Attribute;  // model object attribute flags (start of the embedded GsDOBJ5)
    VOID* pCoord2;  // coordinate system of the model object (points at Coord_flg)
    VOID* pTmd;       // model (TMD) data of the joint
    VOID* pPacket;  // polygon packet buffer of the joint
    INT Obj_id;     // model object id (GsDOBJ5.id)
    INT Coord_flg;  // coordinate flag word, start of the embedded GsCOORDINATE2 (set to 1 at init)
    MATRIX Coord;   // local transform matrix of the joint (GsCOORDINATE2.coord)
    MATRIX Workm;   // world matrix of the joint (GsCOORDINATE2.workm)
    VOID* pParam;     // coordinate parameter pointer (GsCOORDINATE2.param)
    VOID* pSuper;     // parent coordinate system (GsCOORDINATE2.super)
    VOID* pKo;        // child coordinate system (GsCOORDINATE2.sub); ko = child
    SHORT Free0;    // scratch word 0, meaning set by the user: fall speed (Parts_down, Em0c bounce), counter (EM08), magazine y
    SHORT Free1;    // scratch word 1: stride/horizontal speed (Parts_down, Em0c), magazine y position
    SHORT Free2;    // scratch word 2: horizontal speed z (Em0c bounce)
    SHORT Free3;    // scratch word 3: gravity (Em0c), magazine state (MAG.C), neck/aim angle (PL_WEP.C)
    SHORT Free4;    // scratch word 4: bounce count (Em0c), magazine flag (MAG.C)
    SHORT Free5;    // scratch word 5: magazine floor y (MAG.C), cleared at init
};

struct RCUT
{
    VOID* pSp;        // room sprite group table of the camera cut, file offset relocated to a pointer at load
    VOID* pSp_tim;    // texture image (TIM) of the background sprites of the cut
    GsRVIEW2 View;  // camera view (GsRVIEW2): eye position vpx/vpy/vpz and look-at vrx/vry/vrz
    INT ViewR;      // projection distance of the camera, passed to GsSetProjection
};

struct LIGHT_CUT_DATA
{
    GsF_LIGHT Flat; // flat light direction and color handed to GsSetFlatLight
    USHORT Mode;    // light mode, 0 = point light with distance falloff, else fixed direction
    USHORT L;       // falloff distance of the point light
};

struct ROOM_WORK
{
    UCHAR nSprite;              // number of room sprites
    UCHAR nCut;                 // number of camera cuts
    UCHAR nOmodel;              // number of object models
    UCHAR nItem;                // number of item models
    UCHAR Dummy04[2];           // padding
    USHORT Ambient_r;           // room ambient light, red
    USHORT Ambient_g;           // room ambient light, green
    USHORT Ambient_b;           // room ambient light, blue
    LIGHT_CUT_DATA Light[3];    // three light records of the room, LIGHT_CUT_DATA each
    VCUT* pVcut;                // camera switch zone table
    VOID* pSca;                   // collision data of the room, SCA_HEAD followed by the wall boxes
    MODEL_LINK_INFO* pOmodel;   // object model link records
    MODEL_LINK_INFO* pImodel;   // item model link records
    VOID* pBlock;                 // block records of the room, zone count byte then BLOCK_REC entries
    VOID* pFloor;                 // footstep sound zones
    VOID* pScdx;                  // init script
    VOID* pScd;                  // main script
    VOID* pScd1;                  // event script table
    VOID* pEmr;                   // action animation header
    VOID* pEdd;                   // action animation frame data
    VOID* pMessage;               // room message table
    VOID* pRaw;                   // raw item icon images
    VOID* pEsp_hed;               // effect index table
    VOID* pEsp_end;               // effect data
    VOID* pEsp_tim;               // effect sprite textures
    VOID* pEdt0;                  // sound attribute table
    VOID* pVh0;                   // sound bank header
    VOID* pVb0;                   // sound bank body
    RCUT Cut[1];                // camera cut records, one per camera
};

struct TMD_MEM
{
    UCHAR* ver_top;     // vertex table
    INT n_ver;          // number of vertices
    UCHAR* nor_top;     // normal table
    INT n_nor;          // number of normals
    UCHAR* prim_top;    // primitive table
    INT n_poly;         // number of primitives
    INT scale;          // object scale
};

struct OBJ_MODEL_WORK
{
    UCHAR Be_flg;           // state bits: 0x01 live, 0x02 no collision, 0x04 skip probe test, 0x08 no object-object collision, 0x20 not pushable, 0x40 climbable, 0x80 drawn in background layer
    UCHAR Id;               // object model id; item models store their item slot number
    UCHAR Dummy02[2];       // padding
    AT_TBL* pAt_tbl;        // collision box table (AT_TBL); object models point it at their own Sca_flg block
    AT_WORK* pSca_ofs;      // collision offset record slot (entity layout); not used by object models
    UINT Attribute;         // draw object attribute bits, start of the embedded GsDOBJ5
    GsCOORDINATE2* pCoord2; // coordinate system of the model (points at Coord_flg)
    TMD_MEM* pTmd;          // TMD object data (TMD_MEM) the model draws from; 0 = no model
    VOID* pPacket;            // GPU packet buffer of the model
    UINT Coord_flg;         // coordinate flag word (GsCOORDINATE2.flg); the block starting here is the model's coordinate system
    MATRIX Coord;           // local matrix (GsCOORDINATE2.coord); its translation .t is the world position
    MATRIX Workm;           // working matrix (GsCOORDINATE2.workm); its translation is used for the visibility test
    VOID* pParam;             // coordinate parameter pointer (GsCOORDINATE2.param); not read
    GsCOORDINATE2* pSuper;  // parent coordinate system (GsCOORDINATE2.super), 0 for none
    UCHAR Dummy68[4];       // padding (GsCOORDINATE2.sub slot, never accessed)
    SHORT Old_x;           // position x last synced with coord.t[0] (Om_move re-syncs it when the model moves)
    SHORT Old_y;           // position y as placed by the script (not re-synced)
    USHORT Old_z;           // position z last synced with coord.t[2] (Om_move re-syncs it when the model moves)
    USHORT Cdir_x;          // rotation about x, fed to RotMatrix
    USHORT Cdir_y;          // rotation about y (heading), fed to RotMatrix
    SHORT Cdir_z;          // rotation about z, fed to RotMatrix
    SHORT Spd_x;            // movement speed x; not used by object models
    SHORT Spd_y;            // movement speed y; not used by object models
    SHORT Spd_z;            // movement speed z; not used by object models
    SHORT Old_dir;         // facing angle (Cdir_y) of the previous frame; set once at setup for object models
    USHORT Aspd_x;          // script rotation speed about x
    USHORT Aspd_y;          // script rotation speed about y
    USHORT Aspd_z;          // script rotation speed about z, added to Cdir_z by the script move command
    USHORT Push_cnt;        // object models: frames the action button has been held facing the model (push starts at 9); item models: effect slot of the item sparkle
    SHORT Sca_flg;          // collision table flags (first word of the inline AT_TBL block); 0x8000 = last entry
    SHORT Sca_ofs_x;        // collision box half width x (AT_TBL Off_x)
    SHORT Sca_ofs_y;        // collision box half height y (AT_TBL Off_y; Oma_ck_oba uses it as the y extent)
    SHORT Sca_ofs_z;        // collision box half width z (AT_TBL Off_z)
    USHORT Sca_half_h;      // collision half height (AT_TBL H)
    USHORT Sca_r;           // collision radius (AT_TBL At_w)
    SVECTOR Probe1;         // probe segment start (x,z) tested against the room collision before a push
    SVECTOR Probe2;         // probe segment end (x,z) tested against the room collision before a push
};

typedef UCHAR ROOM_BG_BUF[0x8000]; // room background image buffer

struct SEARCH_AOT
{
    USHORT X;           // trigger box x
    USHORT Z;           // trigger box z
    USHORT W;           // trigger box width
    USHORT D;           // trigger box depth
    UCHAR Item_id;      // item id of the pickup
    UCHAR Item_num;     // item count of the pickup
    UCHAR Model_no;     // item model slot, index into the object model table pOm_tbl
    UCHAR Super_no;     // object model index of the parent coordinate system, 0xFF = none
    UCHAR Key_id;       // door lock: bit 7 locked, bit 6 unusable for player id 3, low 6 bits Key_flg bit; item records reuse bytes 0xC-0xD as the model x position
    UCHAR Next_room;    // destination stage (bits 5-7) and room (bits 0-4) of a door
    SHORT Pos_y;        // item model position y
    SHORT Pos_z;        // item model position z
    USHORT Cdir_y;      // item model angle
    UCHAR Item_flg_no;  // flag number set once the item has been taken
    UCHAR Aot_flg;      // action flags of the entry, copied to SCE_AOT.Type by Item_aot_set; not read through this record
    UCHAR Key_type;     // item id of the key that opens the door; item records reuse this byte as the low byte of the model flags
    UCHAR Flg_hi;       // high byte of the model flags word of item records, not read through this record
};

struct SCE_AOT
{
    UCHAR Id;           // entry id, index into Sce_at_jump_tbl, 0 = unused
    UCHAR Type;         // entry attribute flags: bits tested against the attack mask, bit 6 uses the enemy position, bit 7 held
    USHORT Data0;       // entry argument 0 (meaning depends on the Id)
    USHORT Data1;       // entry argument 1
    USHORT Data2;       // entry argument 2
    SEARCH_AOT* pRec;   // record of the entry inside the room script (SEARCH_AOT or IN_DOOR_WORK)
};

struct PSP
{
    UINT attribute; // sprite attribute flags (GsSPRITE.attribute)
    SHORT x;        // screen position x
    SHORT y;        // screen position y
    USHORT w;       // sprite width
    USHORT h;       // sprite height
    USHORT tpage;   // texture page
    UCHAR u;        // texture u
    UCHAR v;        // texture v
    SHORT cx;       // palette x
    SHORT cy;       // palette y
    UCHAR r;        // color red
    UCHAR g;        // color green
    UCHAR b;        // color blue
    UCHAR Dummy17;  // padding
    SHORT mx;       // pivot x
    SHORT my;       // pivot y
    SHORT scalex;   // scale x
    SHORT scaley;   // scale y
    UCHAR be_flg;   // 1 while the sprite is drawn (bit 0)
    UCHAR id;       // sprite group id, 1-based
    USHORT otz;     // ordering table depth; >> 2 is the ordering table index
};

#endif      // TYPES_H
