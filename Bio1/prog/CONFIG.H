/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\CONFIG.H
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

struct KEY_CFG_WORK
{
    UCHAR state;    // key config screen state
    UCHAR preset;   // selected preset
    UCHAR Slot_cur; // button slot cursor
    UCHAR List_cur; // button list cursor
    UCHAR pad4[3];  // padding
    UCHAR rep;      // auto-repeat counter
    USHORT prev;    // previous pad word
};

typedef VOID (*CNF_FUNC)(VOID); // handler taking no arguments

/*
    key configuration screen update for one frame, returns 1 when the screen closes and 0 while
    it stays open: prepare the scratch sprite, enter the controller type Sel_no on the first
    frame, close on the exit input or let Cnf_key_padd handle the pad, then draw the frame
    and the layout
 */
INT Cnf_key_main(VOID);

/*
    draw the highlight of the current cursor (button slot or action row, by Sub_mode), the
    highlight of the selected controller type tab, the shading of the tabs of the types other
    than the active one and the header sprites
 */
VOID Cnf_key_frame_disp(VOID);

/*
    draw the controller layout: the 12 button icons, the buttons assigned to the 8 action
    rows with their labels, and the name of the action bound to each button slot
 */
VOID Cnf_key_layout_disp(VOID);

/*
    pad input of the key configuration screen, returns 0 when the screen should close
    Sub_mode 0: pick the controller type (5 entries, the last one leaves), confirming the custom
    type while it is active enters mode 1, any other entry selects that type
    Sub_mode 1: move the cursor over the button slots and action rows, confirm enters mode 2 or 3,
    the reset input restores the default bindings (with a held button pair it first toggles the
    8-entry action list)
    Sub_mode 2: pick the action to bind to the chosen button slot from the action list
    Sub_mode 3: pick the button to bind to the chosen action row from the button list
 */
UCHAR Cnf_key_padd(VOID);

/*
    draw the action list popup: the action names, colored by whether the action already has a
    button bound, the highlight bar on the selected entry and the popup background
 */
VOID Cnf_key_act_disp(VOID);

/*
    draw the button list popup: the 12 button icons (marked when already bound), the Sel_no
    box and the popup background
 */
VOID Cnf_key_btn_disp(VOID);

/*
    map the direction bits in the high byte of a pad code to a direction index 1..8, or 0 when
    it matches none
 */
UCHAR Cnf_key_idx_get(USHORT code);

/*
    auto-repeat filter for menu input: return the buttons of mask newly pressed this frame,
    plus the held ones each time the repeat timer runs out; the high nibble of rates is the
    delay before the first repeat and the low nibble the repeat interval in frames
 */
INT Pad_rep_get(UCHAR* timer, USHORT* pHeld_old, UCHAR rates, INT mask);

struct OPT_WORK
{
    UCHAR Opt_rno;      // options screen routine number
    UCHAR Key_rno;      // key config screen routine number
    UCHAR Sub_mode;     // key config mode: 0 preset list, 1 and above key edit steps
    UCHAR Sel_no;       // selected entry of the options / key config list
    UCHAR Key_cur;      // key config button cursor
    UCHAR Key_sel;      // key config list selection
    UCHAR Dummy06[3];   // padding
    UCHAR Repeat_timer; // pad auto-repeat timer
    USHORT Pad_rep_old; // previous pad word of the auto-repeat
    UCHAR Pad_fade_rno; // controller screen fade state
    UCHAR Pad_img_sts;  // controller screen state
    UCHAR Dummy0e[2];   // padding
    TILE Tile[2];       // fade rectangles, one per display buffer
    DR_MODE Dr_mode[2]; // draw mode packets, one per display buffer
    SHORT Pad_fade_lv;  // controller screen fade level
};

extern OPT_WORK Cnf_work; // options and key configuration screen work area
extern SHORT Cnf_typex_tbl[];            // x position of each controller type tab (entry 4 is the exit box)
extern SHORT Cnf_slotx_tbl[];               // x position of each button slot
extern SHORT Cnf_sloty_tbl[];               // y position of each button slot row, by slot / 2
extern UCHAR Cnf_move_tbl[][8];       // cursor target when moving in each of the 8 directions, by slot - 4
extern UCHAR Cnf_dir_tbl[];         // direction codes (high byte of the pad bits) of the 8 directions
extern USHORT Cnf_bit_tbl[];           // pad button bit of each button slot
extern UCHAR Cnf_act_tbl0[];         // binding index of each row of the 7-entry action list (0xFE = unassign, two nibbles = two bindings)
extern UCHAR Cnf_act_tbl1[];         // same as Cnf_act_tbl0 for the 8-entry list
extern UCHAR Cnf_slot_tbl[];         // binding index of each of the 8 action rows
extern UCHAR* Cnf_name_tbl0[];       // name text of the 7-entry action list: entry 0 is the unassign row, entry 1 + n the name of row n
extern UCHAR* Cnf_name_tbl1[];       // name text of the 8-entry action list, indexed like Cnf_name_tbl0
extern UCHAR Cnf_namex_tbl0[];        // x offset of each name of Cnf_name_tbl0
extern UCHAR Cnf_namex_tbl1[];        // x offset of each name of Cnf_name_tbl1
extern UCHAR* Cnf_label_tbl[];       // label text of each of the 8 action rows
extern UCHAR Cnf_labelx_tbl[];        // x position of each action row label
extern USHORT Cnf_btn_tbl[];        // pad button bit shown in each cell of the button list
extern UCHAR Cnf_name_dat0[20];   // message text: names of action list entries 0, 1 and 2
extern UCHAR Cnf_name_dat1[36];   // message text: names of action list entries 3 to 7
extern UCHAR Cnf_name_dat2[16];   // message text: names of the two split action entries of the 8-entry list
extern UCHAR Cnf_label_dat0[76];  // message text: labels of action rows 0 to 6
extern UCHAR Cnf_label_dat1[12];  // message text: label of action row 7

#endif
