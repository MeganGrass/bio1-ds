/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM03.H
 */

#ifndef EM03_H
#define EM03_H

#include "TYPES.H"

typedef struct EM03_FREE EM03_FREE;

// private work area of the giant spider, stored in the Free area of its ENEMY_WORK
struct EM03_FREE
{
    SHORT turn_step;            // turn per tick of the strafe and the circle heading; the approach actions store the turn limit here
    USHORT Sikai_flg;           // bit 0: taken from the pathfinding update result when it is 0 or 1; selects the pick conditions and the next action row
    SHORT Pl_hit;               // result of the body against player hit test of the current tick; nonzero lets the lunge bite
    SHORT attack_cooldown;      // ticks until the next lunge or spit may be picked (0xF after each attack)
    SHORT death_started;        // 1 once the death state has started; a repeat entry resumes the flip death at step 3
    SHORT room_hit;             // result of the room collision test of the current tick
    SHORT wall_stuck_frames;    // consecutive ticks with a room collision; 0x1F or more makes the spider circle away
    SHORT Wound_cnt;            // wounds shown so far, the next free slot of Em03_wound_use
};

// access a field of the web spinner work area of the current enemy
#define EM03_EF(field) ENEMY_FREE_FIELD(EM03_FREE, field)

/*
    initialise the entity: clear the state fields, start state 1 with the health taken from the health table, put it on the
    floor or hang it from the ceiling by Type (flag 4 starts dead in state 3), set up the parts, idle animation,
    shadow and collision volume and clear the work fields
 */
VOID Em03_Init(VOID);

/*
    per-frame entry point: on update ticks run the state handler and (outside state 5) the entity, player and room collision
    checks; outside update ticks keep running the death handler while a dead spider is at action step 6; then test the
    position against the camera switch zones and draw the shadow unless the spider hangs from the ceiling
 */
VOID Em03_Main(VOID);

/*
    state 1 handler (nothing while Type bit 7 is set): pick a behavior when the sub-state is 0, run the current one,
    set status flag 0x40 and run the visual range test; a ceiling spider drops when the player comes within 0xBB8 or starts
    action 0x14; count the attack cooldown down and the wall stuck counter up or clear it
 */
VOID Em03_Move(VOID);

/*
    state 2 handler (hit reaction): play the hit action (animation 1) once the action behavior is 0
 */
VOID Em03_Damage(VOID);

/*
    state 3 handler (death): on sub-state 0 take the death variant from the hit kind (Damage_no >> 3), resuming the flip death
    at step 3 when the death was already started; then play the flip death for hit kinds 1, 2, 6, 8 and 9 and the burst
    death otherwise
 */
VOID Em03_Die(VOID);

/*
    state 4 handler: empty
 */
VOID Em03_Dead(VOID);

/*
    state 5 handler: empty
 */
VOID Em03_Nop5(VOID);

/*
    run the action selected by Routine_2 (0 idle, 1 walk, 2 strafe, 3 and 5 approach, 4 and 11 walk, 6 lunge, 7 spit,
    8 drop, 9 none, 10 circle); the walking actions turn towards the waypoint, add a speed bonus and move; runs the pathfinding
    update and takes path flag bit 0 from its result when that is 0 or 1
 */
VOID Em03_Mv_proc(VOID);

/*
    store the distance to the player (sum of the absolute x and z differences) in scratchpad word 0, then run the pick routine for
    Type
 */
VOID Em03_Pick(VOID);

/*
    pick routine for floor spiders of variant 0: approach a player within 0x1770 when path flag bit 0 is clear; once the cooldown is over
    lunge at random when path flag bit 0 is set, or lunge or spit at a player in front within 0x1B58; circle at random when the player
    starts action 0x14 or when stuck at a wall; walk slowly when player Damage_no bit 7 is set
 */
VOID Em03_Pick_floor0(VOID);

/*
    pick routine for floor spiders of variant 1: approach a player that is not in front when the player starts action 0x14 or
    is within 0x1F40; once the cooldown is over lunge or spit within 0x1D4C when path flag bit 0 is set or spit at a player in front
    within 0x1770; circle at random when the player starts action 0x14 or when stuck at a wall
 */
VOID Em03_Pick_floor1(VOID);

/*
    pick routine for ceiling spiders: drop when the player is within 0xBB8 or starts action 0x14, strafe when the player is
    beyond 0x1389 and also walk slowly when the player is in front
 */
VOID Em03_Pick_ceiling(VOID);

/*
    idle action: run the idle routine for Type
 */
VOID Em03_mv00(VOID);

/*
    idle routine: stand (animation 0) for a random time, shorter when path flag bit 0 is set, then take the next action from the pick
    table by path flag bit 0 and a random value 0 to 7
 */
VOID Em03_mv00_pick(VOID);

/*
    idle routine: stand (animation 0) and keep animating
 */
VOID Em03_mv00_hold(VOID);

/*
    walk action (Routine_2 1, 4 and 11): play the walk animation for a random time with the step sounds, set the walking
    speed from the leg pose and return to sub-state 0 with the idle action when the time is over
 */
VOID Em03_Walk_move(VOID);

/*
    approach action (Routine_2 3 and 5): turn towards the player up to the stored turn limit for a random time, then
    go idle (path flag bit 0 set: walk) and take the player position as the waypoint
 */
VOID Em03_Yori_move(VOID);

/*
    strafe action: turn on the spot to a random side (animation 3), switch to the fast walk when facing the player when
    path flag bit 0 is set and end after a random time
 */
VOID Em03_mv02(VOID);

/*
    lunge action: wind up (animation 8) while creeping forward, then run at the player (animation 0xB) and bite when touching
    it (Damage 0x12 with hit reaction, sound and vibration), recover (animation 9) and start the attack cooldown
 */
VOID Em03_mv06(VOID);

/*
    spit action: start the spit animation (0xC) with the spit effect and sound, wait for the animation to end, return to the
    run state and start the attack cooldown
 */
VOID Em03_mv07(VOID);

/*
    drop action: fall from the ceiling with growing speed while turning upright, land on the floor (animation 7 with sound),
    clear the ceiling flag in Type and finish
 */
VOID Em03_mv08(VOID);

/*
    empty action (Routine_2 9)
 */
VOID Em03_mv09(VOID);

/*
    circle action: scuttle sideways (animation 6) along a heading to a random side at a random speed, turning towards the
    player, then reset the wall stuck counter
 */
VOID Em03_mv0a(VOID);

/*
    hit reaction: stop and play the hit sound; for a hit without bit 0 pick a random wound joint and, when it shows no wound yet,
    record it, set the wound effect on the joint with blood and sound; at the end return to the run state with the approach
    action (circle when path flag bit 0 is set) and the player position as the waypoint
 */
VOID Em03_Dm_rtn(VOID);

/*
    death action for the burst hit kinds: burst the body parts with effects, raise the death event, tint and shrink the
    shadow, switch to the dead collision volume and, after 0x5A ticks, spawn the follow-up death entities
 */
VOID Em03_Die_burst(VOID);

/*
    death action for the flip hit kinds: flip onto the back (animation 4 then 5), tint the shadow and keep the body down; a
    hit with Damage_no bit 1 set bursts the body, one with bit 0 set starts the long fade variant that ends with the
    follow-up death entities
 */
VOID Em03_Die_flip(VOID);

/*
    compute the walking speed from the pose: build the entity matrix (scaled when scale is nonzero), concatenate the leg joint
    matrices of the group selected by Flg (0 or 1) and set Spd_cur from the horizontal distance of the foot; the
    speed is 0 while the leg joint is hidden
 */
VOID Em03_Leg_spd_set(UCHAR Flg, INT scale);

extern AT_TBL* pEm03_sca;                 // collision volume of the living spider
extern AT_TBL* pEm03_sca_dead;            // collision volume of the dead spider
extern UCHAR Em03_life[16];                 // start health, indexed by a random nibble
extern VOID (*Em03_routine_0[6])(VOID);                // state handlers by entity state (0=init, 1=run, 2=damaged, 3=death, 4 and 5=empty)
extern VOID (*Em03_pick_tbl[4])(VOID);                 // behavior pick routines by Type (0 to 3)
extern UCHAR Em03_mv_next_tbl[4][8];        // next action by path flag bit 0 (row) and a random value 0 to 7; rows 2 and 3 are never indexed
extern VOID (*Em03_mv00_tbl[4])(VOID);                 // idle routines by Type (0 to 3)
extern UCHAR Em03_step_se_tbl[16];                // step sound by animation frame (0 for none)
extern UCHAR Em03_wound_tbl[8];                // joint that shows a wound, indexed by a random value 0 to 7
extern UCHAR Em03_wound_use[8];               // pSin_parts_ptr that already show a wound (0 for a free slot)

#endif
