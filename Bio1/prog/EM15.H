/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM15.H
 */

#ifndef EM15_H
#define EM15_H

#include "TYPES.H"

typedef struct EM15_FREE EM15_FREE;

struct EM15_FREE
{
    INT vel_x;          // x velocity per tick, 16.16 fixed point
    INT vel_y;          // y velocity per tick, 16.16 fixed point (used by the drop command)
    INT vel_z;          // z velocity per tick, 16.16 fixed point
    INT pos_x;          // x position, 16.16 fixed point (the entity x is this value >> 16)
    INT pos_y;          // y position, 16.16 fixed point (the entity y is this value >> 16)
    INT pos_z;          // z position, 16.16 fixed point (the entity z is this value >> 16)
    SHORT Slide_cnt;    // animation frames during which the typing slide keeps moving the arm
};

// access a field of the left arm work area of the current enemy
#define EM15_EF(field) ENEMY_FREE_FIELD(EM15_FREE, field)

/*
    per-frame entry point of the left computer arm: run the state handler (0 init, 1 command runner), clear the collision matrix
    head and test the arm position against the camera switch zones unless behavior flag 0x80 is set
 */
VOID Em15_Main(VOID);

/*
    state 0 handler: advance to state 1, set health 1 and the status flags, start animation 0, remember the start position as
    the home position and convert the position to 16.16 fixed point
 */
VOID Em15_Init(VOID);

/*
    state 1 handler: while behavior flag 0x80 or 0x20 (command finished) is set only reset the sub-state; otherwise flag 0x40 restarts the command at
    sub-state 0 and is cleared; run the command selected by Type & 0xF
 */
VOID Em15_Move(VOID);

/*
    command 0, rest: return to the home position over 8 ticks while playing animation 0
 */
VOID Em15_mv00(VOID);

/*
    command 1: run the step selected by sub-state (start the reach, then move)
 */
VOID Em15_mv01(VOID);

/*
    command 1, start: begin a reach with animation 1 to an offset of (-0x32, -0x32) from the home position
 */
VOID Em15_mv01_set(VOID);

/*
    reach move step: advance the position by the velocity and play the animation; when it ends set the command finished flag
    (0x20) and reset the sub-state; play the reach sound on frames 3 and 0xD for Jill, frame 8 for Chris
 */
VOID Em15_mv_move(VOID);

/*
    command 2: run the step selected by sub-state (start the reach, then move)
 */
VOID Em15_mv02(VOID);

/*
    command 2, start: begin a reach to an offset of (0x32, 0) from the home position with animation 1 (2 for Jill)
 */
VOID Em15_mv02_set(VOID);

/*
    command 3: run the step selected by sub-state (start the reach, then move)
 */
VOID Em15_mv03(VOID);

/*
    command 3, start: begin a reach to an offset of (-0x14, 0x32) from the home position with animation 2 (3 for Jill)
 */
VOID Em15_mv03_set(VOID);

/*
    command 4, login typing: run the step selected by sub-state (start, type, settle)
 */
VOID Em15_mv04(VOID);

/*
    command 4, start: play animation 4 and slide in z to before the home position: Jill by 0xC8 over 10 ticks, Chris by 0x12C
    over 0x12 ticks
 */
VOID Em15_mv04_set(VOID);

/*
    command 4, typing: advance the position while the animation frame is below the slide length and play the animation; when it
    ends set the command finished flag (0x20); Jill plays the key sound on frame 0xD
 */
VOID Em15_mv04_key(VOID);

/*
    command 4, settle: advance the position until the ticks are over, then set the command finished flag (0x20)
 */
VOID Em15_mv04_end(VOID);

/*
    command 5: run the step selected by sub-state (begin, then the player specific steps)
 */
VOID Em15_mv05(VOID);

/*
    command 5, start: clear Routine_2
 */
VOID Em15_mv05_set(VOID);

/*
    command 5: run the step selected by Routine_2 from the Chris or the Jill step table
 */
VOID Em15_mv05_run(VOID);

/*
    command 5, Chris: slide to 0x50 before the home position in z over 4 ticks
 */
VOID Em15_mv05_slide(VOID);

/*
    command 5, Chris: advance the position until the ticks are over, then start animation 5 and move 0x64 before the home position in x
    over 4 ticks
 */
VOID Em15_mv05_wait(VOID);

/*
    command 5 play: advance the position while ticks remain and play the animation; when it ends set the command finished flag (0x20)
 */
VOID Em15_mv05_play(VOID);

/*
    command 5, Jill: start animation 5
 */
VOID Em15_mv05_jill(VOID);

/*
    command 6, lever: run the step selected by sub-state (reach, move, pull)
 */
VOID Em15_mv06(VOID);

/*
    command 6, start: begin a reach with animation 6 and move to (-0x28, -0x28) from the home position over 4 ticks
 */
VOID Em15_mv06_set(VOID);

/*
    lever move: advance the position until the 4 ticks are over, then start animation 6 and play the animation
 */
VOID Em15_lever_move(VOID);

/*
    lever pull: play the animation with a sound on frame 9; when it ends set the command finished flag (0x20)
 */
VOID Em15_lever_pull(VOID);

/*
    command 7: run the step selected by sub-state (hold, drop, fall, loop)
 */
VOID Em15_mv07(VOID);

/*
    command 7, start: set a 0xA0 tick hold and start animation 0
 */
VOID Em15_mv07_set(VOID);

/*
    command 7, drop: play the animation until the hold ends, then set up a 0xC tick fall to (-0x64, -0x190) from the home position
    in x and z with an upward y velocity
 */
VOID Em15_mv07_drop(VOID);

/*
    command 7, fall: advance the position with gravity on y, clamped to the home height, then settle on animation 2 from frame 0xE
    for 0x5A ticks
 */
VOID Em15_mv07_fall(VOID);

/*
    command 7, loop: play animation 2 up to frame 0xA (0xE for Chris) and restart it each time the tick counter runs out
    (10 ticks for Jill, a random 10 to 0x29 for Chris)
 */
VOID Em15_mv07_loop(VOID);

/*
    command 8, lever: run the step selected by sub-state (reach, move, pull)
 */
VOID Em15_mv08(VOID);

/*
    command 8, start: begin a reach with animation 6 and move to (-0x28, -0x8C) from the home position over 4 ticks
 */
VOID Em15_mv08_set(VOID);

/*
    start a reach: choose the animation (2 becomes 3 after animation 1, and 3 becomes 2 after animation 3), restart it with blend 7
    and set the velocity that reaches the home position plus the offset in 15 ticks
 */
VOID Em15_reach_set(INT Ofs_x, INT off_z, UCHAR anim);

/*
    set the velocity that moves from one x/z position to another in the given number of ticks and restart the fixed point position
    from the entity position
 */
VOID Em15_spd_set(INT Src_x, INT Src_z, INT Dst_x, INT Dst_z, SHORT frames);

/*
    advance the fixed point position by the velocity and store its integer part in the entity position
 */
VOID Em15_pos_add(VOID);

extern VOID (*Em15_routine_0[2])(VOID);          // left arm state handlers by entity state
extern VOID (*Em15_mv_tbl[9])(VOID);            // left arm commands by Type & 0xF
extern VOID (*Em15_mv01_tbl[2])(VOID);    // left arm command 1 steps by sub-state
extern VOID (*Em15_mv02_tbl[2])(VOID);    // left arm command 2 steps by sub-state
extern VOID (*Em15_mv03_tbl[2])(VOID);    // left arm command 3 steps by sub-state
extern VOID (*Em15_mv04_tbl[3])(VOID);      // left arm command 4 steps by sub-state
extern VOID (*Em15_mv05_tbl[2])(VOID);           // left arm command 5 steps by sub-state
extern VOID (*Em15_mv05_chris_tbl[3])(VOID);     // left arm command 5 Chris steps by Routine_2
extern VOID (*Em15_mv05_jill_tbl[2])(VOID);      // left arm command 5 Jill steps by Routine_2
extern VOID (*Em15_mv06_tbl[3])(VOID);           // left arm command 6 steps by sub-state
extern VOID (*Em15_mv07_tbl[4])(VOID);           // left arm command 7 steps by sub-state
extern VOID (*Em15_mv08_tbl[3])(VOID);     // left arm command 8 steps by sub-state

#endif
