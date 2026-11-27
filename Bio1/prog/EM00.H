/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM00.H
 */

#ifndef EM00_H
#define EM00_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include "SCRATCHPAD.H"

typedef struct EM00_FREE EM00_FREE;
typedef struct EM00_HIT_TBL EM00_HIT_TBL;

struct EM00_FREE
{
    UCHAR Catch_dir;        // grab direction 0 to 3 chosen when the grab starts (bit 1 set for behavior flag 2); indexes the grab tables
    UCHAR Dir_ctrl_flg;     // direction flags from the room collision check at the entity position, passed to the wander-turn routine (bit 7 reverses the pushback turn)
    UCHAR Dummy02[2];       // padding: keeps the following fields at their layout offsets
    SHORT angle_turn_delta; // turn step towards the waypoint used by the pushback stagger action
    SHORT Walk_flg;         // bit 0 set from the sight check on the player; while nonzero the waypoint follows the player and the zombie switches to the chase walk
    SHORT Head_flg;         // 1 once the head has been kicked off; enables the head bounce physics
    SCHAR Dummy176;         // cleared at start-up, never read
    SCHAR Catch_timer;      // grab hold Time_cnt (0x69, 0x1E after a recent grab); drops by 1 each tick plus the strength of the player's button presses, the grab ends below 0
    SCHAR Dummy178;         // cleared at start-up, never read
    SCHAR Wander_rno;       // wander-turn steering state
    USHORT Wander_dir;      // angle to the room collision hit, passed to the wander-turn routine
    SCHAR hit_count;        // hits taken; bit 7 set while knocked down
    SCHAR Hit_lim;          // hit count that knocks the zombie down, drawn from the hit threshold table
    UCHAR down_flags;       // bit 0: headless, bit 2: down (ignores hits and does not attack)
    UCHAR push_back_count;  // updates of the short push back run with hit bit 0 set; the second one slows the zombie
    UCHAR Move_spd;         // walking speed of the chase walk
    UCHAR turn_speed;       // turn rate used by the wander-turn steering
    UCHAR Push_timer;       // ticks until the next hit-reaction sound may play
    UCHAR pad17[1];         // padding: keeps the following fields at their layout offsets
    INT saved_state;        // state word stored after each state handler call, restored when a hit arrives while down
    UCHAR Stagger_timer;    // hits left before the stagger knocks the zombie down, drawn from the stagger table
    SCHAR Walk_wait;        // Time_cnt (0x3C) before the walk decision is retried once the player is dead
};

// the Free[] work area of the current entity (G.pEm) as this unit's EM00_FREE
#define EM00_FREE_P ((EM00_FREE*)ENEMY_FREE(G.pEm))

struct EM00_HIT_TBL
{
    UCHAR b[32];    // hit counts that knock the zombie down, indexed by a random value 0 to 31
};

/*
    initialise the entity: set up both shadows, health (health table entry less a random remainder, plus 100 for behavior 0xE),
    collision volume, work fields, start state 1 and the animation for its behavior; behaviors 0xC to 0xE enable the double step,
    behavior 6 starts dead, behavior 0xA hides pSin_parts_ptr 9 to 14 and behavior flag 0x40 selects the scripted state
 */
VOID Em00_Init(VOID);

/*
    per-frame entry point: on update ticks run the state handler, save the state word, count the push cooldown down and (outside
    state 5) run the entity collision checks (not for behavior 0xB) and the room collision checks; step the detached head; then test
    the body position and the position of joint 4 (while its flag 4 is set and its height is -100) against the camera switch zones
    and draw the matching shadows
 */
VOID Em00_Main(VOID);

/*
    state 1 handler: return at once when behavior flag 0x80 is set; otherwise run the behavior handler selected by Type &
    0xF and rebuild the top three status flag bits: 0x40 with the 0xBB8 alert range test unless behavior flag 2 is set or the zombie
    is knocked down, then the 0x1194 visual range test; when the y distance to the player exceeds 0x64 they are replaced by 0x20
    (player y below the zombie y) or 0x80; for behaviors 5 and 7 they are replaced by the 0xFA0 visual range test; 0x40 is set again
    while down
 */
VOID Em00_Move(VOID);

/*
    behavior 0, 1, 6 and 8 handler: start the grab attack (state 5) when the zombie is neither down nor knocked down, the player
    lies within 0x5DC and within 0x2BC either side of the facing, the line to the player is clear of room collision and the player
    is not already hit; otherwise run the movement decision (sub-state 0) and the current action
 */
VOID Em00_Move_chase(VOID);

/*
    behavior 2, 3 and 0xA handler: start the grab attack on the same test with distance 0x898 and 0x200 either side of the facing
    and no knock-down checks; otherwise run the movement decision (sub-state 0) and the pushback action
 */
VOID Em00_Move_push(VOID);

/*
    behavior 4 handler: same as the chase handler, with the additional condition that collision flag 8 is clear for the attack
 */
VOID Em00_Move_rand(VOID);

/*
    state 2 handler (hit reaction): on sub-state 0 restore the saved state while down; resume the fall down action at its stand-up
    step when knocked down (hit_count bit 7); otherwise count hits towards the knock-down threshold and choose the reaction from the
    hit type and the behavior flag 2 row of the reaction table; then run the selected reaction
 */
VOID Em00_Damage(VOID);

/*
    state 3 handler (death): on sub-state 0 choose the death variant (0 default, 1 hit type 4 while facing the player, 2 behavior
    flag 2, knocked down or behavior 5, 3 headless walk instead of 1 one time in two when the head joint is detached) and raise the
    death event; then play the variant: animation 8, 0xA or 0x10 followed by the shared death animation, or the headless walk
 */
VOID Em00_Die(VOID);

/*
    state 4 handler: empty
 */
VOID Em00_Dead(VOID);

/*
    state 5 handler: run the grab attack
 */
VOID Em00_Atck(VOID);

/*
    run the action routine selected by Routine_2 from the action table
 */
VOID Em00_Act_run(VOID);

/*
    pushback action: idle routine when Routine_2 is 0, the stagger routine otherwise
 */
VOID Em00_Push_run(VOID);

/*
    movement decision: refresh the waypoint towards the player while Walk_flg is set, store the distance to the player (sum of the
    absolute x and z differences) in scratchpad word 0, then run the movement decision selected by Type & 0xF
 */
VOID Em00_Mv_run(VOID);

/*
    movement decision for behavior 0: run the sight check into Walk_flg while it is clear, go to the fall down action when knocked
    down, steer with wander turns, turn to face the player when close, switch to the chase walk once Walk_flg is set and the
    player is free, and bend down to eat when the player is dead, close and faced
 */
VOID Em00_Mv_walk1(VOID);

/*
    movement decision for behavior 1 and 5 to 8: count a tick, turn by up to 4 towards the waypoint and step Type down by
    one (a result of 7 becomes 4) when the sight check holds and the player is within 0x2328, or when the zombie is not idling or
    the player is within 0xBB8
 */
VOID Em00_Pl_dist_ck(VOID);

/*
    movement decision for behavior 2: measure the distance to the waypoint; run the sight check while Walk_flg is clear and, once
    it is set, switch to the slow walk action
 */
VOID Em00_Mv_slow(VOID);

/*
    movement decision for behavior 3 and 0xA: force the idle action and advance the sight check without using its result
 */
VOID Em00_Mv_idle(VOID);

/*
    movement decision for behavior 4: as behavior 0, and additionally vomit when blocked by a collision (flag 8) within 0x7D0 of the
    faced player; bend down to eat only when not blocked
 */
VOID Em00_Mv_walk2(VOID);

/*
    action 0, idle: stand (animation 0) for 0x32 to 0xB1 ticks, then switch to the slow walk; from action state 2 turn left and
    right in short steps before returning to the start
 */
VOID Em00_Act_idle(VOID);

/*
    action 1, slow walk (animation 1): on entry choose a waypoint 0x1388 ahead and a duration of 0x12C to 0x1AB ticks; walk with
    footstep sounds and wander-turn steering, moving forward from frame 0x10; when the duration ends return to idle in its
    look-around state
 */
VOID Em00_Act_slow(VOID);

/*
    action 2, chase walk: start-up animation (2), then the walk animation (3) at Move_spd with footstep sounds, steering away for
    0x14 to 0x23 ticks now and then; stop for 0x3C ticks when the player is hit within 0x320; move forward each tick (twice with the
    double step)
 */
VOID Em00_Act_chase(VOID);

/*
    state 5 handler (grab attack), by Act_sts: 0 choose the grab direction and both animations and lock the player into the
    grab; 1 to 3 hold the player and bite every 0x13 ticks (Damage from the Damage table) until the struggle timer runs out or the
    player dies (the zombie then switches to action 5 to line up with the body); 4 and 5 release and recoil; 6 to 9 (behavior
    flag 2) play the bite hold and death variants that end with the zombie dying
 */
VOID Em00_Atck_main(VOID);

/*
    bite reaction for grab direction 0: tint pSin_parts_ptr 12 and 1 of the player with blood, running with the player as the current entity
 */
VOID Em00_Pl_blood0(PARTS_W* pP_ptr);

/*
    bite reaction for grab direction 1: tint pSin_parts_ptr 12 and 1 of the player with blood, running with the player as the current entity
 */
VOID Em00_Pl_blood1(PARTS_W* pP_ptr);

/*
    bite reaction for grab direction 2: tint pSin_parts_ptr 4 and 5 of the player with blood, running with the player as the current entity
 */
VOID Em00_Pl_blood2(PARTS_W* pP_ptr);

/*
    bite reaction for grab direction 3: tint pSin_parts_ptr 8 and 9 of the player with blood, running with the player as the current entity
 */
VOID Em00_Pl_blood3(PARTS_W* pP_ptr);

/*
    action 3, face the player: play animation 3 for 0x1E to 0x5D ticks turning towards the player each tick (up to 0x54); when the
    turn is complete or the ticks run out store the player position as the waypoint and continue with the chase walk
 */
VOID Em00_Act_face(VOID);

/*
    pushback action 0: hold animation 9
 */
VOID Em00_Push_idle(VOID);

/*
    action 4, bend down and eat the dead player: wait 0x3C ticks, turn towards the player and bend down (animation 0xB), eat
    (animation 0xC) for 0x78 ticks with effects and sounds every eighth frame and blood on the player at frame 0xE, then stand
    up (animation 0xD) and return to state 1
 */
VOID Em00_Act_eat(VOID);

/*
    action 5, line up with the dead player: play animation 3 at speed 0x2D and turn by the per-direction angle step for the
    per-direction number of ticks while moving, then switch to action 4
 */
VOID Em00_Act_turn(VOID);

/*
    pushback action 1: play animation 0xE, then walk towards the waypoint at the speed of the leg motion with footstep sounds,
    turning by the angle delta; when direction flag 0x80 is set the delta is reversed and the walk continues in action state 3 (its
    0x1E to 0x3D tick count never changes the state); a collision (flag 8) advances Type
 */
VOID Em00_Push_stagger(VOID);

/*
    action 6, vomit: play animation 5 with a spray effect and sound, then stand for 0x14 to 0x33 ticks and return to idle
 */
VOID Em00_Act_vomit(VOID);

/*
    behavior 5 and 7 handler: eat crouched (animations 0x1E and 0x1D, with effects and a sound on frame 0x19) until the player
    comes within 0xBB8, then stand up (animation 0xD) and continue with behavior 0 (4 in stage id 4 room 5) facing the player
 */
VOID Em00_Move_eat(VOID);

/*
    action 7, fall down: fall (animation 8) with a sound, lie for a random 2 to 9 units of 0x1E ticks, then stand up and return to
    state 1 facing the player; the knocked-down bit of hit_count, the down flag and the hit state are updated each tick so that hits
    are ignored while falling and standing up
 */
VOID Em00_Act_fall(VOID);

/*
    death variant 3, headless walk: walk on (animation 3) turning towards the player with footstep sounds until the animation
    ends, then fall dead (death variant 0)
 */
VOID Em00_Die_walk(VOID);

/*
    behavior 0xB handler: play animation 0x1D (sound on frame 0x14), then lunge forward and sideways, turn by 0x400, continue
    with behavior 0 facing the player and restore the default collision volume
 */
VOID Em00_Move_lunge(VOID);

/*
    hit reaction 0 and 1, short push back: play animation 4 (5 for reaction 1) at a slow speed with a sound; on hit type 3 there is
    a 1 in 8 chance that joint 4 is blown off; at the end return to state 1; the second update with hit bit 0 set slows the zombie
    down
 */
VOID Em00_Dm_short(VOID);

/*
    hit reaction 2 and 3, push and stagger: play animation 6 (7 and the reversed move direction for reaction 2) with a speed boost
    over the first 5 frames; at the end return to state 1 and face the player
 */
VOID Em00_Dm_stagger(VOID);

/*
    hit reaction 5, leg explosion: blow off the leg pSin_parts_ptr with effects and sounds, then fall (health below 0 is raised to 1); a
    zombie that is not behavior 4, is not in stage id 1 room 2, passes the room collision probe and has health left gets up again as
    behavior 3 (1 time in 4) or 2, otherwise health becomes -1, the death event is raised and the shadow shrinks
 */
VOID Em00_Dm_leg(VOID);

/*
    hit reaction 4, push and drop: play animation 0xF with a sound; at the end continue with the slow walk (state 1) and turn
    behavior 3 into behavior 2
 */
VOID Em00_Dm_drop(VOID);

/*
    hit reaction 6, long push back: play animation 7 from frame 0xC moving away from the hit, wait 0 to 7 ticks, then continue
    with the chase walk towards the player position
 */
VOID Em00_Dm_long(VOID);

/*
    hit reaction 7: play animation 0xD, then return to state 1 facing the player with behavior 0 (4 in stage id 4 room 5)
 */
VOID Em00_Dm_standup(VOID);

/*
    shared death animation by Act_sts: 0 start (death timer 0x46, sound unless headless), 1 play, 2 run the room collision
    probe and, one time in four after a forward fall (animation 8), get up again as behavior 3 with 1 health, otherwise set health
    to -1, shrink the shadow and raise the death event, 3 grow the shadow by 6 per tick for 0x46 ticks, 4 raise the script flag when
    behavior flag 0x40 is set
 */
VOID Em00_Die_mot(VOID);

/*
    player animation while grabbed by a zombie (state 5): lock to the zombie's grab position and play the grab animation, hold the
    next animation while Act_sts is 2, play the release animation once Act_sts becomes 3, then return the player to the
    idle state with the grab recovery timer set to 0x96 unless it is already running
 */
VOID Em00_Pl_catch(VOID);

/*
    compute the walking speed from the pose: build the entity matrix, concatenate pSin_parts_ptr 0 and 1 and the three joint matrices of the
    chain selected by Flg (pSin_parts_ptr 3 to 5 or 6 to 8) and set Spd_cur from the horizontal distance of the chain end to its
    world position
 */
VOID Em00_Leg_spd(UCHAR Flg);

/*
    advance the detached head joint by one tick: move it along its rotated velocity (reversing on a wall hit), apply gravity from
    the fall tick counter and bounce off the floor level with a quarter of the speed, an effect and a sound; switched off after
    the sixth bounce
 */
VOID Em00_Head_bound(PARTS_W* pP_ptr, SHORT gravity);

/*
    return the struggle strength from the new button presses: 3 for a direction button plus 2 for a face button
 */
UCHAR Em00_Catch_pad(VOID);

/*
    state 8 handler (scripted control): run the scripted action selected by Routine_2 from the script action table
 */
VOID Em00_Scenario(VOID);

/*
    scripted action 0: empty
 */
VOID Em00_Sce_nop(VOID);

/*
    scripted action 2: start-up animation (2), then walk (animation 3) towards the position in Root_x and Root_z with
    footstep sounds until within 0x96, then raise the script flag and finish (the walk resumes instead while collision flag 0x80 is
    set)
 */
VOID Em00_Sce_walk(VOID);

/*
    scripted action 0xA, head shot: on entry blow off the head (joint 2) with effects and a sound, then run the death animation
    with footstep sounds on frames 8 and 0x1A
 */
VOID Em00_Sce_headshot(VOID);

/*
    scripted action 0xB, dying: play animation 0xA with a sound on frame 0x12 and a speed boost over the first 5 frames, then run
    the death animation moving back
 */
VOID Em00_Sce_dying(VOID);

/*
    scripted action 0xC: play animation 4 through the short push back routine; raise the script flag and finish once Act_sts has bit 1
 */
VOID Em00_Sce_vomit(VOID);

/*
    scripted action 0xD: as action 0xC with animation 5
 */
VOID Em00_Sce_vomit2(VOID);

extern const EM00_HIT_TBL Em00_hit_tbl;  // hit counts that knock the zombie down, indexed by a random value 0 to 31
extern AT_TBL Em00_sca00;                                // collision volume 0 (flags 0x8001, half height 0x5FA, radius 0x1A6): default
extern AT_TBL Em00_sca01;                                // collision volume 1 (radius 0x142): entity id 1
extern AT_TBL Em00_sca02;                                // collision volume 2 (offset x 0xC8, radius 0x3E8): behavior 0xB
extern AT_TBL* Em00_sca_tbl[3];                               // collision volumes by index
extern UCHAR Em00_life[];                                // base health, indexed by a random nibble
extern UCHAR Em00_move_no_tbl[];                               // start animation id by behavior (Type & 0xF)
extern UCHAR Em00_stagger_tbl[];                               // initial stagger timer (3 to 5), indexed by a random value 0 to 31
extern VOID (*Em00_routine_0[9])(VOID);                             // state handlers by entity state (0=init, 1=think, 2=hit, 3=die, 4=empty, 5=attack, 8=scripted)
extern VOID (*Em00_Move_type[12])(VOID);                         // behavior handlers by Type & 0xF
extern UCHAR Em00_dm_sel_tbl[][6];                      // hit reaction by behavior flag 2 and hit type (Damage_no & 7) - 1
extern VOID (*Em00_dm_rtn_tbl[8])(VOID);                   // hit reaction routines by reaction number
extern VOID (*Em00_act_tbl[8])(VOID);                            // action routines by Routine_2
extern VOID (*Em00_mv_tbl[11])(VOID);                    // movement decisions by Type & 0xF
extern UINT Em00_attack_anim_tbl[3];                           // per grab direction: zombie animation, player animation and effect frame (3 bytes each)
extern USHORT Em00_atck_ofs_tbl[];                           // per grab direction: initial Timer0 of the player
extern VOID (*Em00_pl_blood_tbl[4])(PARTS_W*);                   // bite reactions by grab direction
extern UCHAR Em00_atck_dm_tbl[];                                // Damage per bite by behavior (Type & 0xF)
extern UINT zombie_attack_data_tbl[6];                           // per grab direction: angle offset of the step, tick count and angle step of the line-up turn (3 shorts each)
extern UCHAR Em00_down_time_tbl[];                        // lying time in units of 0x1E ticks, indexed by a random nibble
extern VOID (*Em00_sce_act_tbl[20])(VOID);                       // scripted actions by Routine_2

#endif
