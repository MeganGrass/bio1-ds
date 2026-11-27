/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SCE2.H
 */

#ifndef SCE2_H
#define SCE2_H

#include "TYPES.H"

typedef VOID (*SCE2_FUNC)(VOID);

/*
    clear the state, substate, step and substep bytes of the screen record and the
    previous-frame open flag
 */
VOID Sce_scr_clr(VOID);

/*
    per-frame entry: while scenario flag 0x20 is set, run the passcode panel, the computer lab
    or the lab slides, chosen by system flags 0x1D / 0x1E / 0x1F; clear the state, substate,
    step and substep bytes on the frame the screen opens and remember the open flag for the
    next frame
 */
VOID Pc_event_ck(VOID);

/*
    run the current passcode panel state handler, then draw the grid and refresh the room sprites
 */
VOID Sce_panel_disp(VOID);

/*
    state 0 of the passcode panel: save the camera cut and switch to cut 5, clear bits of the
    room update flags, pick the initial tile layout from the scenario flags and player id, then
    show the prompt message
 */
VOID Sce_panel_init(VOID);

/*
    state 1 of the passcode panel: run the handler of the current substate
 */
VOID Sce_panel_main(VOID);

/*
    substate 0: wait for the prompt message to close; a non-zero answer leaves the screen,
    otherwise input starts
 */
VOID Sce_panel_wait_mess(VOID);

/*
    substate 1: move the 3x3 cursor from the direction buttons; the action button selects
    toggle, the cancel button leaves the screen
 */
VOID Sce_panel_pad(VOID);

/*
    substate 2: flip the tile under the cursor and its four neighbours; when all nine tiles
    are lit, start the solved animation, otherwise return to input
 */
VOID Sce_panel_tile_chg(VOID);

/*
    substate 3: run the handler of the current solved-animation step
 */
VOID Sce_panel_anm_main(VOID);

/*
    animation step 0: count down the start delay, then begin switching tiles off
 */
VOID Sce_panel_anm_start(VOID);

/*
    animation step 1: when the delay expires, switch off the next tile of the sequence and load
    the next delay; a zero delay ends the sequence
 */
VOID Sce_panel_anm_step(VOID);

/*
    animation step 2: count down the end delay, then advance to the finish state, play the
    solved sound and set scenario flag 0x21
 */
VOID Sce_panel_anm_end(VOID);

/*
    state 2 of the passcode panel: set the room update flag bits cleared at the start and
    restore the saved camera cut, deactivate the panel and clear scenario flag 0x20
 */
VOID Sce_panel_end(VOID);

/*
    draw the panel frame, the nine numbered tiles and the cursor highlight while the panel
    is active
 */
VOID Sce_panel_draw(VOID);

/*
    set each of the nine room sprites active or inactive from the lit state of its tile
 */
VOID Sce_panel_spr_set(VOID);

struct PANEL_WORK
{
    UCHAR Rno;      // screen state, index into the state routine table
    UCHAR Sub_rno;  // sub-state: index into the update table of the state
    UCHAR Anm_rno;  // animation state: index into the animation routine table (Sce_panel_anm_tbl)
    UCHAR Boot_rno; // boot and typing sequence state (Com_boot_tbl index)
    INT Cursor_x;   // selection cursor column
    INT Cursor_y;   // selection cursor row
    INT Bak_pos_y;  // player height saved while the screen is shown
    USHORT Timer;   // frames left in the current step; the lab slides also use it as a scale value
    SHORT Cnt;     // step counter: animation table position, frames left or text length
};

struct PANEL_SLIDE_WORK  // the lab slides read the same record as four bytes behind the cursor column: Timer and Cnt split into two bytes each
{
    UCHAR Rno;        // screen state (as PANEL_WORK)
    UCHAR Sub_rno;    // slide step: 0 wait, 1 scroll in, 2 text, 3 scroll out
    UCHAR Anm_rno;    // unused by the slides
    UCHAR Boot_rno;   // unused by the slides
    INT Scroll_x;     // left edge of the slide (the Cursor_x of PANEL_WORK)
    INT Scroll_y;     // top edge of the slide (the Cursor_y of PANEL_WORK)
    INT Bak_pos_y;    // unused by the slides
    UCHAR Slide_no;   // slide shown (low byte of Timer): the picture is Slide_no / 2, the half Slide_no & 1
    UCHAR Slide_pass; // 0 first pass over the slides, 1 second pass (high byte of Timer)
    UCHAR Mess_no;    // message shown with the slide (low byte of Cnt)
    UCHAR Wait_cnt;   // frames to wait before the slide scrolls in (high byte of Cnt)
};

extern PANEL_WORK Sce_panel_work;    // state record shared by the passcode panel, computer lab and lab slides
extern USHORT Sce_cut_save;         // camera cut saved when a screen opens, restored when it closes
extern UCHAR Sce_panel_init_tbl00[12];                 // initial tile layout (row-major, 1=lit), default
extern UCHAR Sce_panel_init_tbl02[12];               // initial tile layout when scenario flag 0x1F is set
extern UCHAR Sce_panel_init_tbl01[12];               // initial tile layout when scenario flag 0x1E is set and player id is 1
extern UCHAR Sce_panel_init_tbl03[12];               // initial tile layout when scenario flag 0x1F is set and player id is 1
extern UCHAR Sce_panel_seq_tbl[20];                  // solved animation: (tile index, delay in frames) pairs, zero delay ends it
extern SCE2_FUNC Sce_panel_rno_tbl[3];                    // panel state handlers: init, update, finish
extern SCE2_FUNC Sce_panel_upd_tbl[4];                   // update substate handlers: wait message, input, toggle, animation
extern SCE2_FUNC Sce_panel_anm_tbl[3];                     // solved animation step handlers: start, step, finish

#endif
