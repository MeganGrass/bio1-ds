/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM14.H
 */

#ifndef EM14_H
#define EM14_H

#include "TYPES.H"

typedef struct EM14_FREE EM14_FREE;

struct EM14_FREE
{
    INT vel_x;    // x velocity per tick, 16.16 fixed point
    INT vel_z;    // z velocity per tick, 16.16 fixed point
    INT pos_x;    // x position, 16.16 fixed point (the entity x is this value >> 16)
    INT pos_z;    // z position, 16.16 fixed point (the entity z is this value >> 16)
};

// access a field of the right arm work area of the current enemy
#define EM14_EF(field) ENEMY_FREE_FIELD(EM14_FREE, field)

/*
    per-frame entry point of the right computer arm: run the state handler (0 init, 1 command runner), clear the collision
    matrix head and test the arm position against the camera switch zones unless behavior flag 0x80 is set
 */
VOID Em14_Main(VOID);

/*
    state 0 handler: advance to state 1, set health 1 and the status flags, start animation 0, remember the start position as
    the home position and convert the position to 16.16 fixed point
 */
VOID Em14_Init(VOID);

/*
    state 1 handler: while behavior flag 0x80 or 0x20 (command finished) is set only reset the sub-state; otherwise flag 0x40 restarts the command at
    sub-state 0 and is cleared; run the command selected by Type & 0xF
 */
VOID Em14_Move(VOID);

/*
    command 0, rest: return to the home position over 8 ticks (skipped when within 0xC8 of it) while playing animation 0
 */
VOID Em14_mv00(VOID);

/*
    command 1: run the step selected by sub-state (start the reach, then move)
 */
VOID Em14_mv01(VOID);

/*
    command 1, start: begin a reach with animation 1 to an offset of (-0x28, 0x28) from the home position
 */
VOID Em14_mv01_set(VOID);

/*
    reach move step: advance the position by the velocity and play the animation; when it ends set the command finished flag
    (0x20) and reset the sub-state; play the reach sound on frames 3 and 0xD for Jill, frame 8 for Chris
 */
VOID Em14_mv_move(VOID);

/*
    command 2: run the step selected by sub-state (start the reach, then move)
 */
VOID Em14_mv02(VOID);

/*
    command 2, start: begin a reach to an offset of (0x28, 0) from the home position with animation 1 (2 for Jill)
 */
VOID Em14_mv02_set(VOID);

/*
    command 3: run the step selected by sub-state (start the reach, then move)
 */
VOID Em14_mv03(VOID);

/*
    command 3, start: begin a reach to an offset of (-0x14, -0x28) from the home position with animation 2 (3 for Jill)
 */
VOID Em14_mv03_set(VOID);

/*
    command 4, login typing: run the step selected by sub-state (start the typing, then type)
 */
VOID Em14_mv04(VOID);

/*
    command 4, start: play animation 4; Chris slides the arm to 0xC8 past the home position in z over 0x12 ticks; start the voice
    command (0xB8 for Chris, 0xB9 for Jill) and raise the voice playing game flag 0x20000
 */
VOID Em14_mv04_set(VOID);

/*
    command 4, typing: Jill runs the single typing step; Chris runs the typing step selected by Routine_2 (press, then wait
    for the voice)
 */
VOID Em14_mv04_key(VOID);

/*
    Chris typing, key press: move the arm during the first 0x12 frames with the key sound on frames 0x15 and 0x22; when the
    animation ends advance to the voice wait with 4 ticks
 */
VOID Em14_mv04_press(VOID);

/*
    Chris typing, wait for the voice: once the voice playing game flag 0x20000 is clear set the command finished flag (0x20) and
    reset the sub-state
 */
VOID Em14_mv04_wait(VOID);

/*
    Jill typing: play the typing animation, then wait for the voice playing game flag 0x20000 to clear and set the command
    finished flag (0x20)
 */
VOID Em14_mv04_jill(VOID);

/*
    command 5, lift (Chris only): run the step selected by sub-state (lift, wait, withdraw, settle)
 */
VOID Em14_mv05(VOID);

/*
    lift start: move to 0x50 past the home position in z over 4 ticks
 */
VOID Em14_mv05_set(VOID);

/*
    lift wait: advance the position; when the 4 ticks are over start animation 5
 */
VOID Em14_mv05_wait(VOID);

/*
    lift withdraw: play the animation (Chris plays a sound on frame 0xF); when it ends return to the home position over 4 ticks with animation 0
 */
VOID Em14_mv05_back(VOID);

/*
    lift settle: advance the position and play the animation until the 4 ticks are over, then set the command finished flag (0x20)
 */
VOID Em14_mv05_end(VOID);

/*
    command 6: run the step selected by sub-state (start the reach, then move)
 */
VOID Em14_mv06(VOID);

/*
    command 6, start: begin a reach to an offset of (-0x14, 0xA0) from the home position with animation 2 (3 for Jill)
 */
VOID Em14_mv06_set(VOID);

/*
    command 7, hold: run the step selected by sub-state (hold, wait, move, end)
 */
VOID Em14_mv07(VOID);

/*
    hold start: set a 0x96 tick hold and start animation 0
 */
VOID Em14_mv07_set(VOID);

/*
    hold wait: play the animation until the hold ends, then start animation 4 and move to the position 0x64 before the home position
    in x and 0x46 (0x8C for Jill) past it in z over 0x18 ticks
 */
VOID Em14_mv07_wait(VOID);

/*
    hold move: play animation 4 (Jill is limited to frame 5, Chris held on frame 0x28) and advance the position until the ticks are over
 */
VOID Em14_mv07_move(VOID);

/*
    command 7, final hold step: do nothing
 */
VOID Em14_mv07_end(VOID);

/*
    start a reach: choose the animation (2 becomes 3 after animation 1, and 3 becomes 2 after animation 3), restart it with blend 7
    and set the velocity that reaches the home position plus the offset in 15 ticks
 */
VOID Em14_reach_set(INT Ofs_x, INT off_z, UCHAR anim);

/*
    set the velocity that moves from one x/z position to another in the given number of ticks, in 16.16 fixed point
 */
VOID Em14_spd_set(INT Src_x, INT Src_z, INT Dst_x, INT Dst_z, USHORT frames);

/*
    advance the fixed point position by the velocity and store its integer part in the entity position
 */
VOID Em14_pos_add(VOID);

extern VOID (*Em14_routine_0[2])(VOID);            // right arm state handlers by entity state
extern VOID (*Em14_mv_tbl[8])(VOID);              // right arm commands by Type & 0xF
extern VOID (*Em14_mv01_tbl[2])(VOID);      // right arm command 1 steps by sub-state
extern VOID (*Em14_mv02_tbl[2])(VOID);      // right arm command 2 steps by sub-state
extern VOID (*Em14_mv03_tbl[2])(VOID);      // right arm command 3 steps by sub-state
extern VOID (*Em14_mv04_tbl[2])(VOID);        // right arm command 4 steps by sub-state
extern VOID (*Em14_mv04_key_tbl[2])(VOID);      // right arm Chris typing steps by Routine_2
extern VOID (*Em14_mv05_tbl[4])(VOID);         // right arm command 5 steps by sub-state
extern VOID (*Em14_mv06_tbl[2])(VOID);    // right arm command 6 steps by sub-state
extern VOID (*Em14_mv07_tbl[4])(VOID);         // right arm command 7 steps by sub-state

#endif
