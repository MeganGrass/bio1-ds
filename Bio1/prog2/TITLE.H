/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog2\TITLE.H
 */

#ifndef TITLE_H
#define TITLE_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

struct TITLE_TIMER_REC
{
    SHORT timer;      // screen Time_cnt, idle timeout and transitions
    SHORT Hold_timer; // mode-select hold Time_cnt
};

typedef struct TITLE_WORK TITLE_WORK;

struct TITLE_WORK
{
    TITLE_TIMER_REC Timer_wk;   // screen Time_cnt (timer) and mode-select hold Time_cnt (Hold_timer)
    UCHAR Dummy4[4];            // padding: keeps the following fields at their layout offsets

    USHORT Spr_clut_tbl[8];     // CLUT row of each title sprite, by sprite number
    UCHAR unused_18[8];         // padding: keeps the following fields at their layout offsets
    USHORT Spr_tpage_tbl[8];    // texture page of each title sprite, by sprite number

    UCHAR Cursor_no;            // selected entry: 0=attract demo, 1=new game, 2=load game, 3=other task, 4..7=mode submenu
    SCHAR Dummy31;              // cleared at start-up; never read
    UCHAR running;              // 1 while the title screen loop runs, 0 once an exit was requested
    UCHAR page;                 // 0=press-start page, 1=menu page
    UCHAR Sub_rtn0;             // state within the current page
};

/*
    overlay entry point: set up the title screen, run Title_main once per frame until the
    screen exits, then chain to the program selected by Cursor_no
 */
VOID main(VOID);

/*
    reset the display, load the title graphics, upload the title sprites to VRAM and
    record their texture page and CLUT row; start the screen running with Cursor_no 1
 */
VOID Title_init(VOID);

/*
    per-frame title state machine: page 0 fades in the press-start sprite, moves to page 1
    on a button press and falls back to the attract demo on timeout; page 1 runs the menu
    and the mode submenu with fades, Sel_no sounds, input handling and timeout
 */
VOID Title_main(VOID);

/*
    empty function
 */
VOID Title_nop(VOID);

/*
    queue the sprite number option at the given Kido into the current frame's
    ordering table; a Kido other than 0x80 draws it semi-transparent
 */
VOID Title_sp_set(UCHAR bright, UCHAR option);

/*
    queue the mode-menu sprite (sprite 1) at the given Kido, with the CLUT row
    offset by option to highlight that entry
 */
VOID Title_mode_set(UCHAR bright, UCHAR option);

/*
    stop the title loop and set the mode field in the top two bits of the game flags to 1
 */
VOID Title_exit(VOID);

extern TITLE_WORK Ti; // title screen work area

#endif
