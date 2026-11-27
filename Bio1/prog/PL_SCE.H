/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\PL_SCE.H
 */

#ifndef PL_SCE_H
#define PL_SCE_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>

typedef VOID (*PL_SCE_FUNC)(VOID);

/*
    state routine of the player under script control: run the routine selected by Routine_2
    (twice when script flag 2 is set) and handle the footstep when script flag 4 is set
 */
VOID Pl_Scenario(VOID);

/*
    scripted animation 0: state 0 starts animation 0 and state 1 plays the body animation; states 2
    to 5 reload the player weapon model and play the body, weapon, action or grab animation set; state 6 idles
 */
VOID Pl_sce00(VOID);

/*
    scripted animation 1: start the animation selected by Move_no (grab set from 0x3E, action
    set from 0x10, otherwise the body or weapon set from the chain table), blending in over 8 frames
    unless script flag 0x20 is set, play it (on alternate frames while script flag 0x80 is set),
    then add the scripted turn to the angle and raise the flag given by Set_flg_no; restarts when script
    flag 0x10 is set
 */
VOID Pl_sce01(VOID);

/*
    scripted walk: turn toward the target direction, then walk (animation 2) to the target position
    with footstep sounds until within 150 units, raise the flag given by Set_flg_no and return
    the entity to state 1 unless collision flag 0x80 is set
 */
VOID Pl_sce02(VOID);

/*
    scripted run: turn toward the target direction, run (animation 3) to the target position until
    within 250 units, slow down for 4 frames, return the entity to state 1 and raise the flag given
    by Set_flg_no; with collision flag 0x80 set only the flag is raised, without slowing down
 */
VOID Pl_sce03(VOID);

/*
    scripted backward walk: walk backward (animation 3, footsteps) toward the target position until
    within 100 units, raise the flag given by Set_flg_no and return the entity to state 1 unless
    collision flag 0x80 is set
 */
VOID Pl_sce04(VOID);

/*
    scripted slow backward step: step backward (animation 2, footsteps) toward the target position
    until within 100 units, raise the flag given by Set_flg_no and return the entity to state 1
    unless collision flag 0x80 is set
 */
VOID Pl_sce05(VOID);

/*
    scripted turn in place: face the target direction while playing the weapon animation, then
    return the entity to state 1 and raise the flag given by Set_flg_no
 */
VOID Pl_sce06(VOID);

/*
    scripted weapon animation: play the weapon animation set once (animation id minus 5), adding the
    scripted turn to the angle, then raise the flag given by Set_flg_no
 */
VOID Pl_sce07(VOID);

/*
    scripted weapon animation without turn: play the weapon animation set once (animation id minus
    5) with the late flag clear, then raise the flag given by Set_flg_no
 */
VOID Pl_sce08(VOID);

/*
    scripted weapon animation: play the weapon animation set once (animation id minus 5) with the
    late flag set, then return the entity to state 1 and raise the flag given by Set_flg_no
 */
VOID Pl_sce09(VOID);

extern PL_SCE_FUNC Pl_sce[10]; // player behaviour routines of the script-controlled state, by Routine_2

struct ECG_SEG_REC
{
    SCHAR Start_x;  // segment start x
    SCHAR End_x;    // segment end x
    SCHAR Start_y;  // segment start y
    SCHAR Slope;    // y change per x step along the segment
};

extern UCHAR Ecg_wave_tbl[0x384];         // EKG waveform segments (x start, x end, y start, slope), 4 bytes each
extern ECG_SEG_REC* Ecg_line_tbl[20];        // EKG waveform segment tables
extern UCHAR Ecg_rgb_tbl[4][3];           // EKG line colour (RGB) by health status
extern UCHAR Ecg_caution_step_tbl[0x8];     // trace position thresholds of the caution label animation
extern UCHAR Ecg_danger_step_tbl[0xC];      // trace position thresholds of the danger label animation
extern UCHAR Ecg_caution_frm_tbl[0x8];    // caution label frame shown at each step
extern UCHAR Ecg_danger_frm_tbl[0xC];     // danger label frame shown at each step

#endif
