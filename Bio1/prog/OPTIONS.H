/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\OPTIONS.H
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

/*
    options screen task: set up the menu (sound is reinitialised when opened from the title, the game
    task is suspended when opened from the game), then run the menu every frame with the states
    0=fade in, 1=main menu, 2=key configuration, 3=stereo/mono, 4=leave, 5=vibration on/off
 */
VOID Config(VOID);

/*
    start leaving the options screen: enter state 4 and fade the screen out
 */
VOID Config_exit(VOID);

/*
    draw the menu panel and the four entry boxes, the selected entry in blue; the selected box is
    skipped while the stereo/mono or vibration page is open
 */
VOID Opt_box_draw(VOID);

/*
    track the connected controller type through a fade between the digital and analog controller
    pictures (reloading the picture file when it changes) and queue the fade overlay primitives;
    pType holds the type shown, 0xFF for none
 */
VOID Opt_pad_img_proc(UCHAR* pType);

extern INT Opt_from_title;       // 1 when the options screen was opened from the title, 0 from the game
extern USHORT Opt_box_tbl[4];         // x position of each entry box in the options menu

#endif      // OPTIONS_H
