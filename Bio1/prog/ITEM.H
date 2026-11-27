/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ITEM.H
 */

#ifndef ITEM_H
#define ITEM_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

typedef VOID (*ITEM_FUNC)(VOID);    // item viewer action handler

/*
    per-frame update of the item viewer: state 0 sets up the model's objects and coordinate chain, state 1
    zooms in with a light fade-in (when it ends, mode 0 enters state 2 and modes 3, 4 and 6 show a message and
    enter state 3 or 4), state 2 runs the current viewer action, states 3 and 4 wait for the message reply,
    state 5 zooms out with a fade-out; draws the model and, in mode 0 with viewer action 0, the item name;
    returns 1 once the viewer has closed
 */
INT Item_view_main(VOID);

/*
    viewer action 0: confirm tries to examine the item (else shows its description), cancel starts closing the
    viewer, the zoom buttons move the model nearer or farther within limits, the shoulder buttons and the
    direction pad set the spin inputs; applies the spin and draws the direction arrows
 */
VOID Item_act_disp(VOID);

/*
    viewer action 1: spin the two parts of the item in opposite directions with a growing step (at most 0x20)
    until the second part passes angle 0x300, then show the item's examine message
 */
VOID Item_act_spin(VOID);

/*
    viewer action 2: wait for the message window to close, then return to viewer action 0; items 0x3F and 0x40
    whose second part was turned start viewer action 3 instead, item 0x3E sets flag 0xD in that case
 */
VOID Item_act_wait(VOID);

/*
    viewer action 3: zoom the model out with a light fade-out, then replace the item in its Item_work slot with
    the item whose id is 0x1B lower (count 1), upload an icon image for the slot, reload the model and restart
    the viewer
 */
VOID Item_act_medal(VOID);

/*
    wrap the rotation angles to 0-0xFFF, build the rotation matrix of the model and of each of its parts and
    queue every part into the current frame's ordering table
 */
VOID Item_model_disp(VOID);

/*
    apply the spin inputs to the model's rotation matrix and recompute the model's rotation angles from the result
 */
VOID Item_rot_set(VOID);

/*
    queue the four direction arrow sprites; an arrow uses a different image while its direction is not held
 */
VOID Item_arrow_disp(VOID);

/*
    examine the current item: an item with an examine pose needs the model's rotation to match it, then shows
    the examine message (items 0x3E not yet turned, 0x3F and 0x40 start viewer action 1 instead); items without
    an examine entry only react for items 5 and 7-9 (message 0xEF when the slot count is nonzero) and item 0x13
    (message 0xF0 when flag 0x13 or 0 is set); returns 1 when handled, 0 when nothing happened
 */
UCHAR Item_exam_ck(VOID);

/*
    test angle against the next angle window of the examine table (tolerance, target angle) and advance past it;
    a zero tolerance accepts any angle; returns pRet when the angle is inside the window, else 0
 */
INT Item_ang_ck(SHORT angle, UCHAR pRet);

extern ITEM_FUNC Item_act_tbl[4];                 // viewer action handlers, by View_act

extern UCHAR Item_arrow_x_tbl[8];                         // x position of each direction arrow (USHORT: up, right, down, left)
extern UCHAR Item_arrow_y_tbl[8];                         // y position of each direction arrow (USHORT: up, right, down, left)
extern USHORT Item_model_tbl[76];                    // file number of each item model, by model id - 1

extern UCHAR Item_mix_tbl0[0x14];                 // combine rule lists for Mix_idx 0-3
extern UCHAR Item_mix_tbl1[0x2C];                 // combine rule lists for Mix_idx 4-7
extern UCHAR Item_mix_tbl2[0x28];                 // combine rule lists for Mix_idx 8-11
extern UCHAR Item_mix_tbl3[0x3C];                 // combine rule lists for Mix_idx 12-15
extern UCHAR Item_mix_tbl4[0x58];                 // combine rule lists for Mix_idx 16-19
extern UCHAR Item_mix_tbl5[0x74];                 // combine rule lists for Mix_idx 20-23
extern UCHAR Item_mix_tbl6[0x10];                 // combine rule lists for Mix_idx 24-27
extern UCHAR Item_mix_tbl7[0x2C];                 // combine rule lists for Mix_idx 28-31
extern UCHAR Item_mix_tbl8[0x18];                 // combine rule lists for Mix_idx 32-35
extern UCHAR Item_mix_tbl9[0x4];                  // combine rule lists for Mix_idx 36-37
extern UCHAR* Item_mix_tbl[38];                    // combine rule list of each Mix_idx

struct STATUS
{
    UCHAR St_type;              // screen entry type, chosen at screen start and switched on by the screen task (0-6)
    UCHAR Main_rtn;             // top-level screen state routine (switch in the status task)
    UCHAR Act_res;              // result of the last item action, selects the message
    UCHAR St_flg;               // screen control bits (0x01/0x40 leave, 0x80 item used, 0x02/0x04/0x08 action phases)
    UCHAR* pFrm_rec;            // frame record table of the screen element being drawn
    UCHAR Blink_timer;          // cursor blink / repeat counter; cleared on every cursor move
    UCHAR Chr_flg;              // character bits; bit 0 = player id bit, bit 1 = extra Item_work slots
    UCHAR nSlot;                // Item_work slot count of the character
    UCHAR Item_id;              // item selected or shown
    SCHAR Wep_flg;              // dual use byte: 1 = selected item is a weapon (ST_SUB01/02, Item_id < 11); in the item box scroll code it is the scroll step (+-1, +-3, 0 idle)
    UCHAR Res_id;               // id of the resource currently loaded (File_tex_load/file_moji_load compare it); 0 = none; the item box also reuses it as the 0-0xF scroll offset counter (INVENTORY.C)
    SCHAR Model_sts;            // item model load state, 0 = none
    UCHAR Radio_flg;            // radio available flag
    UCHAR Sub_rtn0;             // sub state of the screen state
    UCHAR Sub_sts;              // state of the item sub menu
    UCHAR Act_sts;              // state of the item move and combine action
    UCHAR Icursol_main;         // cursor position on the main item list, encodes row and column
    UCHAR Ibox_sel;             // selected item box slot (0-0x2F)
    UCHAR Icursol_sub;          // cursor position the moved item was picked from
    UCHAR Sub_wait;             // item box repeat countdown (reload 0xF); the next byte Sub_timer doubles as the held-pad mask there; ST_SUB03.C sets it to 0xFF at move start
    UCHAR Sub_timer;            // sub menu timer (bit 7 flag, low bits count); in the item box the held pad mask
    UCHAR Move_anm[3];          // item move animation counters
    UCHAR Dummy01b[3];          // padding
    UCHAR Ecg_rtn;              // health display animation state
    UCHAR Ecg_type;             // health tier derived from health and Max_life
    UCHAR Ecg_col[3];           // ECG trace color rgb
    UCHAR Ecg2_col[3];          // second ECG trace color rgb
    SHORT Ecg_x0;               // primary ECG trace start x; Ecg_x1 follows as the end x
    UCHAR Ecg_x1;               // primary ECG trace end x, low byte (read as a SHORT with Ecg_x1h)
    UCHAR Ecg_x1h;              // primary ECG trace end x, high byte
    SHORT Ecg2_x0;              // secondary ECG trace start x
    SHORT Ecg2_x1;              // secondary ECG trace end x
    UCHAR Dummy02e[2];          // padding
    ECG_SEG_REC* pEcg_wave;     // segment table of the primary ECG trace
    ECG_SEG_REC* pEcg2_wave;    // segment table of the secondary ECG trace
    UCHAR Face_anm;             // face portrait animation frame, 0xFF = none
    UCHAR View_rno;             // item viewer state
    UCHAR View_act;             // item viewer action state
    UCHAR nView_obj;            // number of model objects in the viewer
    SHORT Spin_x;               // viewer spin input x
    SHORT Spin_y;               // viewer spin input y
    SHORT Spin_z;               // viewer spin input z
    UCHAR Dummy042[2];          // padding
    SVECTOR Obj_rot[2];         // rotation of each viewer model object
    SVECTOR Rot;                // rotation of the viewed item
    GsDOBJ5 Obj[2];             // model objects of the viewer
    GsCOORDINATE2 Coord[3];     // coordinate systems of the viewer objects
    UCHAR Timer;                // viewer fade timer (counts 0x40 down)
};

struct ITEM_DATA
{
    UCHAR Item_max;         // maximum count stackable in one slot
    UCHAR model_id;         // icon image and model number (1-based)
    SCHAR Mix_idx;          // index into Item_mix_tbl / Item_img_tbl; -128 = not combinable
    UCHAR Item_attribute;   // index into Item_exam_type_tbl / Item_exam_mess_tbl; 0x80 = no examine entry
};

struct ITEM_INFO_TBL
{
    ITEM_DATA items[64]; // per-item records, indexed by item id - 1
    UCHAR Heal_tbl[52]; // 13 further 4-byte item records (ids 65-77), read as raw bytes by the item-use code
};

extern UCHAR Item_img_tbl[0x28];                // icon image number (1-based, 0 = none) of an item, by Mix_idx
extern ITEM_INFO_TBL Item_data_tbl;                      // per-item records (max count, icon, combine and examine index), by item id - 1
extern UCHAR Item_use_tbl[0x18];                 // first item id of each use-type group (first 10 bytes); later bytes are further per-item data
extern UCHAR Item_exam_mess_tbl[0x10];               // message id shown for each examine entry
extern UCHAR Item_exam_ang_tbl[0x30];                 // examine angle windows: 12-byte sets of three (tolerance, target angle) SHORT pairs for x, y, z
extern UCHAR Item_exam_type_tbl[0x10];                  // per examine entry: high nibble = pose count, low nibble = first window set
extern UCHAR Item_mess_dat[0x6A8];          // encoded item description strings
extern UCHAR* Item_mess_tbl[77];                   // description string of each item, by item id - 1

extern GsF_LIGHT Item_light[3];             // flat lights of the item viewer, faded with the zoom
extern STATUS St;                  // work area of the status menu and the item viewer

#endif
