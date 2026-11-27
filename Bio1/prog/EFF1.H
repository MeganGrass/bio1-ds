/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EFF1.H
 */

#ifndef EFF1_H
#define EFF1_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

struct MSEQUENCE
{
    UCHAR Routine0;     // behavior id run before the movement update
    UCHAR Routine1;     // behavior id run after the movement update
    UCHAR Dummy02[14];  // padding
    USHORT Speed_x;     // x velocity of the step
    USHORT Speed_y;     // y velocity of the step
    USHORT Speed_z;     // z velocity of the step
    USHORT DirY;        // rotation added to the DirY
};

struct SSEQUENCE
{
    UCHAR uv_index; // index into the sprite uv list
    UCHAR Time;     // frames the step is shown
    UCHAR w;        // sprite width
    UCHAR h;        // sprite height
};

struct ESP_DATA_HDR
{
    USHORT uv_count;    // number of uv entries; not read
    USHORT Frm_num;     // number of 4-byte animation steps that precede the uv list
    USHORT Clut;        // palette position of the sprite
    USHORT Tpage;       // texture page of the sprite
};

typedef struct MSEQ_REC MSEQ_REC;
typedef struct ESP_STEP_REC ESP_STEP_REC;
typedef struct ESP_SLOT_WORK ESP_SLOT_WORK;
typedef struct ESP_CASING_WORK ESP_CASING_WORK;
typedef struct ESP_ACTOR_WORK ESP_ACTOR_WORK;

typedef VOID (*ESP_FUNC)(VOID);

struct MSEQ_REC
{
    UCHAR b[0x18]; // record bytes, copied over the start of the effect
};

struct ESP_STEP_REC
{
    UCHAR Routine0;     // behavior run before the movement update, 0 when the slot is free
    UCHAR Routine1;     // behavior run after the movement update
    UCHAR Mat_mode;     // 1=follow the parent matrix, 2=keep its own copy of the matrix
    UCHAR Mag;          // sprite scale factor; multiplied with the camera fov and divided by Otz4 in Esp_pos
    UCHAR Free0;        // behavior parameter 0
    UCHAR Free1;        // behavior parameter 1
    UCHAR Free2;        // behavior parameter 2
    UCHAR Free3;        // behavior parameter 3
    USHORT Add_x;       // acceleration added to the velocity each frame, x
    SHORT Add_y;       // acceleration, y
    USHORT Add_z;       // acceleration, z
    USHORT Be_flg;    // 1 animate, 2 update position, 4 force y 0, 8 force y to floor, 0x8000 hidden, bits 6-7 blend mode
    USHORT Speed_x;     // velocity added to the displacement each frame, x
    USHORT Speed_y;     // velocity, y
    USHORT Speed_z;     // velocity, z
    USHORT DirY;        // rotation about the y axis applied to the displacement
};

struct ESP_SLOT_WORK
{
    ESP_STEP_REC h;             // behaviors, parameters, acceleration, flags, velocity and DirY
    SHORT Mov_x;                // displacement from the local offset, x
    SHORT Mov_y;                // displacement, y
    SHORT Mov_z;                // displacement, z
    UCHAR Anm_time;             // frames left before the sprite advances to the next frame
    UCHAR Anm_cnt;              // index of the current sprite frame
    SHORT Pos_x;                // world position, x
    SHORT Pos_y;                // world position, y
    SHORT Pos_z;                // world position, z
    UCHAR Type;             // effect type the entry was created with
    UCHAR Id;                   // effect id within the type, also selects the palette row
    SHORT Hot_x;                // offset from the parent matrix, x
    SHORT Hot_y;                // offset from the parent matrix, y
    SHORT Hot_z;                // offset from the parent matrix, z
    SHORT Otz4;                 // projected depth times 4
    SHORT M00;                  // matrix element row 0 column 0
    SHORT M01;                  // matrix element row 0 column 1
    SHORT m02;                  // matrix element row 0 column 2
    SHORT m10;                  // matrix element row 1 column 0
    SHORT M11;                  // matrix element row 1 column 1
    SHORT m12;                  // matrix element row 1 column 2
    SHORT M20;                  // matrix element row 2 column 0
    SHORT M21;                  // matrix element row 2 column 1
    SHORT M22;                  // matrix element row 2 column 2
    SHORT Dummy42;              // matrix padding
    INT trans_x;                // matrix translation, x
    INT Trans_y;                // matrix translation, y
    INT Trans_z;                // matrix translation, z
    UINT Otz;                    // projected depth of the position
    VECTOR spawn;               // position the effect was created at
    MATRIX* pMat;          // matrix the effect is placed relative to
    UCHAR* pEsp_hdr;            // sprite set information: counts, palette and texture page
    UCHAR* pAnmseq;             // start of the sprite frame list
    UCHAR* pAnm_cur;            // current sprite frame
    UCHAR* pSprg;               // start of the texture coordinate list
    UCHAR* pSprg_cur;           // texture coordinates of the current frame
    ESP_STEP_REC* pMoveseq_top; // first animation step record of the effect
    ESP_STEP_REC* pMoveseq;     // current animation step record
};

struct ESP_CASING_WORK
{
    UCHAR Routine0;         // behavior run before the movement update, 0 when the slot is free
    UCHAR Routine1;         // behavior run after the movement update
    UCHAR Mat_mode;         // 1=follow the parent matrix, 2=keep its own copy of the matrix
    UCHAR Mag;              // sprite scale factor; multiplied with the camera fov and divided by Otz4 in Esp_pos
    UCHAR Free0;            // behavior parameter 0
    UCHAR Free1;            // behavior parameter 1, number of bounce records to choose from
    UCHAR Free2;            // behavior parameter 2, set to mirror the x velocity
    UCHAR Free3;            // behavior parameter 3, weapon index the casing came from
    USHORT Add_x;           // acceleration, x
    SHORT Add_y;           // acceleration, y
    USHORT Add_z;           // acceleration, z
    USHORT Be_flg;        // 1 animate, 2 update position, 4 force y 0, 8 force y to floor, 0x8000 hidden, bits 6-7 blend mode
    SHORT Speed_x;          // velocity, x
    SHORT Speed_y;          // velocity, y
    SHORT Speed_z;          // velocity, z
    SHORT DirY;             // rotation about the y axis
    SHORT Mov_x;            // displacement, x
    SHORT Mov_y;            // displacement, y
    SHORT Mov_z;            // displacement, z
    SHORT Anm_time;         // frame delay and frame index
    SHORT Pos_x;            // world position, x
    SHORT Pos_y;            // world position, y
    SHORT Pos_z;            // world position, z
    SHORT Type;         // effect type and effect id
    SHORT Hot_x;            // offset from the parent matrix, x
    SHORT Hot_y;            // offset from the parent matrix, y
    UCHAR pad2C[0x1C];      // padding: fields not used by the casing behavior
    INT Trans_y;            // matrix translation, y
    UCHAR Dummy4c[0x34];    // padding: fields not used by the casing behavior
    MSEQUENCE* pMoveseq;    // current animation step record
};

struct ESP_ACTOR_WORK
{
    UCHAR Routine0;         // behavior run before the movement update, 0 when the slot is free
    UCHAR Routine1;         // behavior run after the movement update
    UCHAR Mat_mode;         // 1=follow the parent matrix, 2=keep its own copy of the matrix
    UCHAR Mag;              // sprite scale factor; multiplied with the camera fov and divided by Otz4 in Esp_pos
    UCHAR pad4[4];          // padding: behavior parameters, not used here
    USHORT Add_x;           // acceleration added to the velocity each frame, x
    SHORT Add_y;           // acceleration, y
    USHORT Add_z;           // acceleration, z
    USHORT Be_flg;        // 1 animate, 2 update position, 4 force y 0, 8 force y to floor, 0x8000 hidden, bits 6-7 blend mode
    USHORT Speed_x;         // velocity added to the displacement each frame, x
    USHORT Speed_y;         // velocity, y
    USHORT Speed_z;         // velocity, z
    SHORT DirY;             // rotation about the y axis applied to the displacement
    USHORT Mov_x;           // displacement from the local offset, x
    USHORT Mov_y;           // displacement, y
    USHORT Mov_z;           // displacement, z
    USHORT Dummy1e;         // padding: frame delay and frame index, not used here
    USHORT Pos_x;           // world position, x
    USHORT Pos_y;           // world position, y
    USHORT Pos_z;           // world position, z
    UCHAR Dummy26;          // padding: effect type, not used here
    UCHAR Id;               // effect id within the type, also selects the palette row
    USHORT Hot_x;           // offset from the parent matrix, x
    USHORT Hot_y;           // offset from the parent matrix, y
    USHORT Hot_z;           // offset from the parent matrix, z
    USHORT Otz4;            // projected depth times 4
    MATRIX matrix;          // matrix the position is transformed with
    UINT Otz;                // projected depth of the position
    UCHAR Dummy54[0x10];    // padding: spawn position, not used here
    MATRIX* pMat;      // matrix the effect is placed relative to
    USHORT* pEsp_hdr;       // sprite set information: counts, palette and texture page
    UCHAR Dummy6c[4];       // padding: frame list, not used here
    UCHAR* pAnm_cur;        // current sprite frame
    UCHAR Dummy74[4];       // padding: texture coordinate list, not used here
    UCHAR* pSprg_cur;       // texture coordinates of the current frame
    UCHAR Dummy7c[8];       // padding: animation step pointers, not used here
};

#define ESP_POOL ((ESP_WORK*)&MAIN.Esp)              // effect pool: 64 entries
#define ESP_IDX (*&MAIN.Esp_idx)      // index of the effect being processed
#define pESP (&ESP_POOL[ESP_IDX])           // the effect being processed
#define ESP_FREE_CNT (*&MAIN.Scenario.nEsp_free)        // number of free effect entries
#define ESP_SLOTS ((ESP_SLOT_WORK*)&MAIN.Esp)            // effect pool as ESP_SLOT_WORK entries
#define ESP_CASINGS ((ESP_CASING_WORK*)&MAIN.Esp)         // effect pool as ESP_CASING_WORK entries
#define ESP_ACTORS ((ESP_ACTOR_WORK*)&MAIN.Esp)           // effect pool as ESP_ACTOR_WORK entries

/*
    reset the effect system for a room: mark all 64 entries free and clear them, clear the
    sprite set tables, register the room's effect data and upload its textures
 */
VOID Esp_init_R(VOID);

/*
    reset the effect system for the common effects: mark all entries free, clear the sprite set
    tables, read the common effect data and texture files and upload the textures
 */
VOID Esp_init_C(VOID);

/*
    register the sprite sets listed in ptr8 (ending at 0xFF, at most 8) from the offsets in
    ptr32 relative to addr, recording their ids from index type; returns the number of sets
 */
UCHAR Esp_data_set0(UCHAR* ptr8, INT* ptr32, INT addr, UCHAR type);

/*
    upload the textures and palettes of the sprite sets from index start to VRAM, continuing
    from the saved upload cursor when start is 8 and restarting it otherwise, and patch each
    set's palette, texture page and v coordinates; the cursor is saved when start is 0
 */
VOID Esp_data_set1(INT start);

/*
    call Esp_call with the same arguments
 */
SCHAR Esp_call_api(UCHAR tp, UCHAR id, INT dir_y, MATRIX* pmat, VECTOR* svec, UCHAR scale);

/*
    create the effect of type tp and id at position svec relative to matrix pmat (identity when
    0) with DirY dir_y and scale (0 keeps the animation scale); returns the entry number of the
    last effect created, or -1 when none is free
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
INT Esp_call(INT id, INT tp, INT dir_y, MATRIX* pmat, VECTOR* svec, INT scale);

/*
    update and draw every effect from the last entry down, drawing the mirror reflection as well
    when the mirror game flag is set
 */
VOID Esp_move(VOID);

/*
    update one effect: run its pre behavior, compute the world position from displacement, DirY
    and parent matrix, project it, integrate acceleration and velocity, run the post behavior,
    animate the sprite and queue it in the ordering table
 */
VOID Esp_pos(VOID);

/*
    queue the mirrored sprite of the effect: project its position through the mirrored view and
    sort it into the ordering table when it is inside the mirror
 */
VOID Esp_mirror_disp(VOID);

/*
    when the frame delay is zero step to the next sprite frame (jumping back on the loop marker,
    ending the effect on the end marker), then count the delay down
 */
VOID Esp_anm_move(VOID);

/*
    clear r words starting at p
 */
VOID Memclr(UINT* p, INT r);

/*
    end every effect matching all the criteria enabled in mask: bit 0 effect type equals id, bit
    1 depth group equals tp, bit 2 Free2 equals c, bit 3 parent matrix equals pmat
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
INT Esp_kill(INT mask, INT id, INT tp, INT c, MATRIX* pmat);

extern UCHAR* Eff_tim_adr[8];    // texture images of the loaded sprite sets
extern ESP_FUNC Esp_tblj[64];                          // effect behaviors, by behavior number

#endif      // EFF1_H
