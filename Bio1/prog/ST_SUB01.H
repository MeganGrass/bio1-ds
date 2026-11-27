/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ST_SUB01.H
 */

#ifndef ST_SUB01_H
#define ST_SUB01_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

typedef VOID (*ST_SUB01_FUNC)(VOID);    // menu handler

/*
    draw the health status label sprite (0 danger, 1 caution, 2 and 3 fine, 4 poison); the
    animated labels pick their frame from the scroll position of the EKG, returned in frame
 */
VOID Ecg_label_disp(UCHAR status, UCHAR* frame);

/*
    draw the EKG health monitor: scroll the primary and secondary waveforms, derive the health
    status and line colour from the player health and poison flags, draw the label and the line
    segments, then fade the display out in the closing states
 */
VOID St_disp_ECG(VOID);

/*
    return the index of the first step threshold above the current EKG scroll position
 */
INT Ecg_step_get(UCHAR* steps);

/*
    draw one gradient line segment of the fading EKG trail, lowering each colour channel by its
    step per pixel, and carry the end colour over as the start colour of the next segment
 */
VOID Ecg_seg_disp(INT dr, INT dg, INT db);

/*
    status menu input state machine: initialise the Sel_no, move the cursor over the tab and
    item cells and open the selected entry, then run the tab handler or the item submenu
 */
VOID St_main00(VOID);

/*
    draw the cursor sprite on the current cell: the blinking item frame for an item slot or the
    highlight for a tab button
 */
VOID St_disp_cursol0(VOID);

/*
    set the selected item id from the slot under the cursor, or to 0 when the slot is empty
 */
VOID St_sel_item_set(VOID);

extern VOID (*St_menu0_proc_tbl[4])(VOID);    // tab handlers: map, file, radio, exit

#endif      // ST_SUB01_H
