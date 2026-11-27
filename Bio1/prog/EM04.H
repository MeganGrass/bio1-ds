/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM04.H
 */

#ifndef EM04_H
#define EM04_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>

typedef struct EM04_FREE EM04_FREE;

// private work area of the black tiger, stored in the Free area of its ENEMY_WORK
struct EM04_FREE
{
    SHORT turn_step;            // turn limit per tick of the approach and strafe; signed heading offset of the circle
    SHORT Sikai_flg;            // bit 0: the player is in sight (from the path update result)
    SHORT Pl_hit;               // result of the body versus player hit test of the current tick
    SHORT attack_cooldown;      // ticks until the next spit may be picked
    SHORT Die_mv;               // death action chosen once (1 writhe, 6 flip), reused when the death state is entered again
    SHORT room_hit;             // result of the room collision test of the current tick
    SHORT wall_stuck_frames;    // consecutive ticks with a room collision; 0x15 or more makes the tiger strafe or approach
    SHORT Unk_17a;              // set to 6 by the walk action and never read
    SHORT Wound_cnt;            // wounds shown so far, the next free slot of Em04_wound_kan
    SHORT Pick_timer;           // ticks left in which the tiger only walks towards a distant player (0x2D after init)
};

// access a field of the black tiger work area of the current enemy
#define EM04_EF(field) ENEMY_FREE_FIELD(EM04_FREE, field)

/*
    initialise the entity: start state 1 and clear the animation and hit fields, set up the parts and shadow, set health 0xCC,
    scale 0x1B33 and the collision volume, clear the work fields and start the behavior pick cooldown at 0x2D ticks
 */
VOID Em04_Init(VOID);

/*
    per-frame entry point: on update ticks run the state handler, test the entity against the player, other enemies and the room;
    then test the camera switch zones and update the shadow
 */
VOID Em04_Main(VOID);

/*
    state 1 handler: pick a behavior when the sub-state is 0, run the current one, set status flag 0x40, run the visual range
    test, count the attack cooldown down and count the ticks spent blocked by room collision
 */
VOID Em04_Move(VOID);

/*
    state 2 handler (hit reaction): play the hit reaction (animation 1) once the action behavior is 0
 */
VOID Em04_Damage(VOID);

/*
    state 3 handler (death): on sub-state 0 take the death action from the hit kind (Damage_no >> 3), or the remembered one, then
    play the writhe death for hit kinds 1 and 2, the flip death for 6, 8 and 9 and the burst death otherwise
 */
VOID Em04_Die(VOID);

/*
    refresh bit 0 of Sikai_flg from the path update result (when it is 0 or 1), then run the action selected by Routine_2
    (0 idle, 1 walk, 2 strafe, 3 approach, 10 circle, 12 bite, 13 spit)
 */
VOID Em04_Move_proc(VOID);

/*
    store the distance to the player (sum of the x and z differences) in scratchpad word 0, then pick the next action: walk while
    the cooldown runs and the player is far, strafe or approach when blocked or the player is out of sight, bite when
    the player is within 0x1388, spit when the cooldown is over and the player is ahead, walk when the player is beyond 0x2328
 */
VOID Em04_Move_pick(VOID);

/*
    idle action (Routine_2 0): play the idle animation (0)
 */
VOID Em04_mv00(VOID);

/*
    walk action (Routine_2 1): play the walk animation for a random time with the step sounds, set the walking speed from
    the leg pose (at most 0x50), move and return to the pick step when the time is over
 */
VOID Em04_mv01(VOID);

/*
    approach action (Routine_2 3): turn towards the player up to the stored turn limit for a random time or until facing
    it, then return to the pick step and take the player position as the waypoint
 */
VOID Em04_mv03(VOID);

/*
    strafe action (Routine_2 2): turn on the spot to a random side (animation 3), spit when the player is in sight, in
    front and the cooldown is over, and end after a random time
 */
VOID Em04_mv02(VOID);

/*
    unused lunge action: rear up (animation 8), track the player, then run at it (animation 0xB, speed 0x12C) and bite when touching
    it (Damage 0x14 with hit reaction and vibration), recover (animation 9) and start the attack cooldown
 */
VOID Em04_mv_lunge(VOID);

/*
    unused single spit action: play the spit animation (0xC) with one spit effect, then return to state 1 with action
    behavior 0xB and start the attack cooldown
 */
VOID Em04_mv_spit(VOID);

/*
    unused drop action: fall from the ceiling with growing speed while the roll angle unwinds, land on the floor (animation 7)
    and return to the pick step
 */
VOID Em04_mv_drop(VOID);

/*
    empty action
 */
VOID Em04_mv_nop(VOID);

/*
    circle action (Routine_2 10): scuttle sideways (animation 6) along a heading to a random side at a random speed,
    turning towards the player, then reset the wall stuck counter
 */
VOID Em04_mv0a(VOID);

/*
    bite action (Routine_2 12): rear up (animation 8), track the player, then bite (animation 9) and hurt the player
    for 0x14 health on frame 0xC when one of the first two pSin_parts_ptr reaches it; end with the pick step, or bite again when the player is dead
 */
VOID Em04_mv0c(VOID);

/*
    spit action (Routine_2 13): start the spit animation (0xC) with three spit effects spread to both sides and a sound,
    wait for the animation to end, return to state 1 with action behavior 0xC (bite) and start the attack cooldown of 0x14 ticks
 */
VOID Em04_mv0d(VOID);

/*
    hit reaction: stop and play the hit sound; for a hit without bit 0 pick a random wound joint and, when it shows no wound yet,
    record it, set the wound effects on the joint with blood and sound; at the end return to state 1 with the approach
    action (circle when the player is in sight) and the player position as the waypoint
 */
VOID Em04_dm00(VOID);

/*
    death action for the burst hit kinds: burst the body parts with effects, raise the death event, tint and shrink the
    shadow, switch to the dead collision volume and, after 0x5A ticks, spawn the follow-up death entities
 */
VOID Em04_Die_mv00(VOID);

/*
    death action for the flip hit kinds: flip onto the back (animation 4 then 5), tint the shadow and keep the body down; a
    hit with Damage_no bit 1 set bursts the body, one with bit 0 set starts the long fade variant that ends with the
    follow-up death entities
 */
VOID Em04_Die_mv06(VOID);

/*
    death action for hit kinds 1 and 2: writhe on the ground (animation 1 then 7) with burst effects, raise the death event,
    skid in random directions while the follow-up death entities are updated, then play animation 4 and end
 */
VOID Em04_Die_mv01(VOID);

/*
    compute the walking speed from the pose: build the entity matrix (scaled when scale is nonzero), concatenate the leg joint
    matrices of the group selected by Flg (0 or 1) and set Spd_cur from the horizontal distance of the foot; the
    speed is 0 while the leg joint is hidden
 */
VOID Em04_Foot_spd_set(UCHAR Flg, INT scale);

extern AT_TBL* pEm04_sca;                 // collision volume of the living tiger
extern AT_TBL* pEm04_sca_walk;            // collision volume used while walking, approaching and strafing
extern AT_TBL* pEm04_sca_dead;            // collision volume of the dead tiger
extern VOID (*Em04_routine_0[4])(VOID);           // state handlers by entity state (0=init, 1=run, 2=damaged, 3=death)
extern UCHAR Em04_step_se_tbl[16];                // step sound id by animation frame (0 for none)
extern UCHAR Em04_wound_kan_tbl[8];                // joint that shows a wound, indexed by a random value 0 to 7
extern UCHAR Em04_wound_kan[8];               // pSin_parts_ptr that already show a wound (0 for a free slot)

#endif
