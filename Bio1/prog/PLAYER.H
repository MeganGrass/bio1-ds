/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\PLAYER.H
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

/*
    per-frame player update: make the player the current entity, run the state routine and the neck
    rotation when update flag 1 is set, update the collision boxes and run the entity and scene collision
    checks (skipped in state 5 and behaviour 0x11), refresh the camera zone flags, queue the shadow
    inside the zones, run the mirror transform when the mirror is in view and call Mag_down
 */
VOID Player_move(VOID);

/*
    state 0 routine: clear the held buttons, enter state 1, set up the shadow quad, reset the animation
    and run its first step, centre the collision box and clear the hit and turn state
 */
VOID Pl_init(VOID);

/*
    initialise the player entity for a stage: reset its fields, set the VRAM cursors, load the player
    model, set the coordinate system and joint animation data, restore the equipped weapon from the
    Item_work and load its model, place the player at the saved position and pick the collision box by id
 */
VOID Player_set(VOID);

/*
    reset the player entity fields: Timer_wk, animation, status flags, state 0, speeds, rotation, identity
    matrix and scale
 */
VOID Pl_work_init(VOID);

/*
    state 1 routine: enter the dying state when health is negative, the Damage state when a hit is
    registered, apply the periodic health loss of collision flags 0x62, count down the turn timer and
    run the sub-state routine from Pl_mv
 */
VOID Pl_move(VOID);

/*
    state 2 routine: run the hit reaction while the sub-state is 0
 */
VOID Pl_damage(VOID);

/*
    state 3 routine: run the death routine
 */
VOID Pl_die(VOID);

/*
    state 4 routine: clear the entity update flags
 */
VOID Pl_dead(VOID);

/*
    run the routine of the first group of the Pl_move table (19 entries) selected by the
    sub-state; at action state 0 first raise update flag 0x40 when game flag 0x40 or collision flag
    0x80 is set, and clear collision flag 0x80
 */
VOID Pl_Em_damage(VOID);

/*
    run the routine of the second group of the Pl_move table (19 entries) selected by the
    sub-state; at action state 0 first raise update flag 0x40 when game flag 0x40 or collision flag
    0x80 is set, and clear collision flag 0x80
 */
VOID Pl_Em_die(VOID);

/*
    run the routine of the third group of the Pl_move table (19 entries) selected by the
    sub-state
 */
VOID Pl_state_anim_window2(VOID);

/*
    sub-state 0 routine: turn the pad input into a behaviour, then run the behaviour dispatch
 */
VOID Pl_mv_sub0(VOID);

/*
    run the movement routine selected by Routine_2 (standing, walking, turning, backing, running,
    Climb_flg, pushing), applying the per-frame turn of the turning variants
 */
VOID Pl_mv_sub2(VOID);

/*
    sub-state 1 routine: run the Pl_act_tbl entry selected by Routine_2
 */
VOID Pl_mv_sub1(VOID);

/*
    weapon behaviours: run the aim, raise and hold, aim change, holster, reload, fire click or lock-on
    turn routine selected by Routine_2 (0x12 to 0x1A), then the footstep; checks the hand when
    Type is 6
 */
VOID pl_mv05(VOID);

/*
    sub-state 4 routine: run the Pl_act_tbl entry selected by Routine_2
 */
VOID Pl_mv_sub4(VOID);

/*
    hit reaction: at action state 0 raise update flag 0x40 when game flag 0x40 or collision flag 0x80
    is set and clear collision flag 0x80; run pl_dm00 for behaviours 0x64 and 0x65, pl_dm01 with a
    backward slide for 0x66 or a forward slide for 0x67 (from animation frame 4), and pl_dm02 for 0x68
 */
VOID Pl_hit_react_common(VOID);

/*
    turn the pad state into a behaviour: the action button starts a climb, an action object or the
    switch-zone action; the weapon button readies the equipped weapon; the direction bits select
    walking forward, backing, turning on the spot or turning while moving
 */
VOID pl_mv00_pad(VOID);

/*
    standing behaviour: idle animation 0 with a 100-frame timer; when the timer ends and update flag
    0x100 is set play the weapon animation set, then hold animation 1
 */
VOID pl_mv00(VOID);

/*
    walking behaviour: animation 2 at speed 0x5D, slowed on the step frames, with footsteps and vibration
    pulses; the run button switches to running and releasing forward returns to standing; status flag 1
    halves the speed and advances the animation on alternate frames
 */
VOID pl_mv01(VOID);

/*
    turning-on-the-spot behaviour: play animation 2 with footsteps until the turn buttons are released,
    then return to standing
 */
VOID pl_mv04(VOID);

/*
    turn the pad direction bits into the turn-on-the-spot behaviours only (4 for right, 5 for left)
 */
VOID Pl_pad_turn(VOID);

/*
    empty function
 */
VOID Pl_nop09(VOID);

/*
    action pose behaviour: play animation 4 from the weapon animation set, then set game flag 0x100 for
    a room action of type 0xD or 0x800 otherwise, raise update flag 0x40, clear collision flag 0x80 and
    return to standing
 */
VOID pl_mv06(VOID);

/*
    backing behaviour: play animation 3, or animation 2 when an active enemy passes the proximity test,
    with footsteps and vibration pulses, move backward and return to standing when the backward button
    is released
 */
VOID pl_mv03(VOID);

/*
    running behaviour: the action button starts a climb, an action object or the switch-zone action;
    direction bits turn the player; run (animation 3, speed 0xD2) with footsteps and vibration pulses;
    releasing the run button falls back to walking and releasing forward slows down over 4 frames
    and returns to standing
 */
VOID pl_mv02(VOID);

/*
    empty function
 */
VOID Pl_nop(VOID);

/*
    zone climb behaviour: walk to the target position, align to the nearest axis, play the climb
    animation (0x33, or 0x35 going up) with step sounds and vibration, shift the player to the far
    side, walk 15 frames and return to state 1, clearing game flag 0x10
 */
VOID pl_mv07(VOID);

/*
    choose the aim target: the nearest active, living enemy of a normal type (id below 0x13) inside the
    camera zones, behavior flags 0xC0 clear and not hidden behind a wall; weapon 1 takes the nearest,
    other weapons the nearest with pos_y of 0 or less, else the nearest with positive pos_y; returns 1
    when found, else points pTgt at the first enemy slot and returns 0
 */
INT Em_ck(VOID);

/*
    switch the aim target to the next enemy after the current one (cycling over the 30 slots) that is
    active, living, of a normal type, inside the camera zones, behavior flags 0xC0 clear and not
    hidden behind a wall; returns 1 when found, else 0
 */
INT pl_mv05_sub(VOID);

/*
    test whether the segment from the player along dir crosses a wall segment of the room collision
    data (types other than 4 and 5 with mask 0x300 set); returns 1 when blocked
 */
UCHAR Pl_aim_wall_ck(VECTOR* dir);

/*
    initialise a shadow Billboard: fill its textured quad (colour from the scratchpad, semi-transparent),
    set its four Vtx from the half sizes Half_x and Half_z, store the position and copy the quad to the
    second primitive buffer
 */
VOID Kage_work_init(SVECTOR* pPos, KAGE_WORK* pK, SHORT Half_x, SHORT Half_z);

/*
    set the semi-transparency flag and the RGB colour of both primitive buffers of a shadow Billboard
 */
VOID Kage_work_color_set(KAGE_WORK* pK, INT Trans, INT Abr, INT Col);

/*
    grow the Vtx of a shadow Billboard by dx and dz (negative values shrink it)
 */
VOID Kage_work_size_add(KAGE_WORK* pK, INT dx, INT dz);

/*
    set the Vtx of a shadow Billboard to a rectangle of half width Half_x and half depth Half_z
 */
VOID Kage_half_set(KAGE_WORK* pK, SHORT Half_x, INT Half_z);

/*
    set the Vtx of a shadow Billboard to a rectangle with the given extent on each side
 */
VOID Kage_rect_set(KAGE_WORK* pK, SHORT x_pos, SHORT x_neg, SHORT z_pos, INT z_neg);

/*
    draw a shadow Billboard: build its matrix from the position, the ground height and the direction,
    transform the Vtx and queue the quad in the ordering table, by depth when status flag 8 is set
    and at a fixed depth otherwise
 */
VOID MulKage(VECTOR* pPos, KAGE_WORK* pK, SHORT Grand, SHORT Dir);

/*
    advance the load pointer past nKage shadow work records
 */
VOID Kage_work_set(INT nKage);

/*
    Damage reaction 0: play animation 1 with a sound, then return to state 1 and clear the hit state
 */
VOID pl_dm00(VOID);

/*
    Damage reaction 1: play the grab animation set (animation Damage_no - 1) with a sound, then return
    to state 1 and clear the hit state
 */
VOID pl_dm01(VOID);

/*
    Damage reaction 2: play the grab animation set (animation Damage_no - 1) with another sound, then
    return to state 1 and clear the hit state
 */
VOID pl_dm02(VOID);

/*
    pushing behaviour: play the push animation (0x30, then 0x31) from the action animation set, moving
    for the first 16 frames, with a sound and vibration on frame 1; when game flag 0x40 clears, play the
    release and return to state 1
 */
VOID pl_mv0a(VOID);

/*
    object climb behaviour: align the angle, play the climb animation (0x33, or 0x35 going up) with step
    sounds while the shadow follows the first joint, then move the player by the climbed distance (smaller in
    rooms 5 and 0xE) and return to state 1
 */
VOID pl_mv08(VOID);

extern USHORT Pl_turn_walk;    // turn speed per frame while walking or backing, set by Pad_set
extern SHORT Pl_turn_run;     // turn speed per frame while running, set by Pad_set
extern UCHAR Pl_sca_tbl[0x24]; // player collision box records, 12 bytes each; the first and third are chosen by the player id through Pl_sca_ptr
extern AT_TBL* Pl_sca_ptr[5];      // pointers to the collision box records (the player id picks entry 0 or 2), then two constant words
extern PL_FUNC Pl_routine[5];                // player state routines, by state (0 init, 1 move, 2 Damage, 3 die, 4 dead)
extern PL_FUNC Pl_act_tbl[4];       // behavior routines run by Pl_mv_sub1 and Pl_mv_sub4, by Routine_2; the table goes on in Pl_mv
extern PL_FUNC Pl_mv[19];                      // player sub-state routines, by Key_rno; also the entries of the behavior table from Routine_2 4
extern UCHAR Ladder_frm_tbl[];          // animation frames of the climb step sounds, in playing order

#endif
