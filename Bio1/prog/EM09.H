/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM09.H
 */

#ifndef EM09_H
#define EM09_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include "SCRATCHPAD.H"

typedef struct EM09_FREE EM09_FREE;
typedef struct EM09_STATE_TBL EM09_STATE_TBL;
typedef struct EM09_ARC_TBL EM09_ARC_TBL;

// private work area of a chimera, stored in the Free area of its ENEMY_WORK
struct EM09_FREE
{
    SHORT turn_step;            // turn rate per tick of the turn actions (0x20 walking, 0x40 running)
    SHORT Sikai_flg;            // bit 0: the path update reports the player in view; the waypoint then follows the player
    SHORT Pl_hit;               // nonzero while the chimera touches the player
    SHORT attack_cooldown;      // ticks before the next attack may start
    SHORT Kage_stop;            // 1 once a death action has faded the shadow out; stops the per-frame shadow resize
    SHORT room_hit;             // nonzero while the chimera touches the room collision
    SHORT wall_stuck_frames;    // consecutive ticks spent in contact with the room collision
    SHORT Run_flg;              // 1 while running (animation 2, faster move speed), 0 while walking
    SHORT Catch_timer;          // grab hold Time_cnt (starts at 0x5A); lowered by the player's button presses and mirrored into the player's Timer0
    SHORT Atck_flg;             // set when placed with behavior 3 (the behavior is then randomised to 0 or 1); enables claw attacks
};

struct EM09_STATE_TBL
{
    VOID (*state[4])(VOID);  // state handlers by entity state
    UCHAR seed[8];           // first hit reaction (Routine_2) by hit type (Damage_no & 7) - 1
};

struct EM09_ARC_TBL
{
    SHORT Pos_y_tbl[33];    // height above the floor along the swoop and drop arc, by animation frame
    SHORT Spd_tbl[33];      // forward speed along the swoop and drop arc, by animation frame
};

// access a field of the chimera work area of the current enemy
#define EM09_EF(field) ENEMY_FREE_FIELD(EM09_FREE, field)

/*
    state 0 handler: set state 1 and clear the work fields; behavior 3 marks the chimera Atck_flg and is replaced by a random 0 or 1;
    behaviors 0 and 1 start on the floor, 2 and above start upside down on the ceiling (behavior 2); set up the shadow, random health
    (0x50 to 0x7A, even), start animation for the behavior and the collision volume
 */
VOID Em09_Init(VOID);

/*
    per-frame entry point: on update ticks run the state handler, the entity and room collision checks and store the results; then test
    the position against the camera switch zones and, inside one, size the shadow from the height of joint 3 (unless locked) and place it
 */
VOID Em09_Main(VOID);

/*
    state 1 handler: unless behavior flag 0x80 is set run the decision routine (sub-state 0) and the current action; rebuild the top
    status flag bits with the visual range test 0xFA0 (and 0x1388 on the ceiling); count the attack cooldown down and the frames
    spent in contact with the room collision
 */
VOID Em09_Move(VOID);

/*
    state 2 handler (hit reaction): on sub-state 0 choose the reaction from the hit type (seed table), plus 1 when the player is outside the facing range; reactions 0 to 3
    run the stagger with animation 0xD, 0xC or 0xF (6 for behavior 1 and above), 4 and 5 run the knock down with animation 0xB
 */
VOID Em09_Damage(VOID);

/*
    state 3 handler (death): on sub-state 0 choose the variant (0, or 1 when the player is outside the facing range; 4 on the ceiling); variants 0 and 1 dissolve with
    animation 0xB or 0xC, variant 4 drops and dissolves
 */
VOID Em09_Die(VOID);

/*
    run the action selected by Routine_2; first refresh the path update and copy its in-view result (when 0 or 1) to Sikai_flg bit 0
 */
VOID Em09_behavior_dispatch(VOID);

/*
    store the distance to the player (sum of the absolute x and z differences) in scratchpad word 0, then run the decision routine selected by Type
 */
VOID Em09_Move_br(VOID);

/*
    decision routine for behavior 0: walk (action 1) while the player is in view; with the player ahead within 0x5DC and unhit swoop
    (action 7), or claw (0xC) when Atck_flg or the facing test passes, toggle (5) while Atck_flg on cooldown; short toggle (0xB)
    when not in view and close or stuck; hold (0xD) when the player's hit state has bit 7 set
 */
VOID Em09_Move_br00(VOID);

/*
    decision routine for behavior 1: walk (run when the player is over 0x1388 away) while the player is in view; with the player ahead
    within 0x5DC and unhit half the time spit (action 8), or claw (0xC) when the facing test passes, toggle (5) while Atck_flg on
    cooldown; turn (action 2) when not in view and close or stuck; hold (0xD) when the player's hit state has bit 7 set
 */
VOID Em09_Move_br01(VOID);

/*
    decision routine for behavior 2 (ceiling): walk (action 9) while the player is not in view; turn (action 0xA) when in view or
    stuck; swipe (action 3) with the player within 0x7D0, unhit and the cooldown over, or drop (6) when the facing test passes; drop
    when the player is over 0x3A98 away and lined up; hold (0xD) when the player's hit state has bit 7 set
 */
VOID Em09_Move_br02(VOID);

/*
    run the idle routine selected by Type
 */
VOID Em09_mv00(VOID);

/*
    idle on the floor: play the animation of the behavior and hold it
 */
VOID Em09_mv00_floor(VOID);

/*
    idle on the ceiling: play animation 1 for a random 0x50 to 0x8F ticks (shorter by 0x50 when the player is in view), then return
    to the decision routine
 */
VOID Em09_mv00_ceil(VOID);

/*
    action 1, walk: while the player is in view find the route to it and turn towards the waypoint; run the walk timer, move
    forward and play footstep sounds on animations 2, 3 and 9 at fixed frame intervals
 */
VOID Em09_mv01(VOID);

/*
    walk timer: choose a duration (random 0 to 0x1F times the action, plus 0x50 for action 0 or 1), play the animation until it runs
    out, then return to the decision routine
 */
VOID Em09_mv01_mot(VOID);

/*
    action 2, turn: turn towards the player by turn_step per tick for 0x50 to 0x6F ticks or until lined up; then return to the decision
    routine, running when the player is in view, with the player position as waypoint
 */
VOID Em09_mv02(VOID);

/*
    action 3, swipe: play animation 8; on frames 7 to 0xC, when joint 6 or 10 is within 0x320 of the player in view and unhit, hit the
    player for 0x14 with effects, sound and vibration; at the end return to the decision routine with a 0x3C tick cooldown
 */
VOID Em09_mv03(VOID);

/*
    action 4, grab: lock the player into the grab (animation 5, player state 6) and hold; on frame 0x22 bite for 0x1E Damage with
    effects and vibration; button presses lower the struggle timer; at the end release and swoop (7), toggle (5) with a 0x5A tick
    cooldown when Atck_flg
 */
VOID Em09_mv04(VOID);

/*
    action 5, toggle: from behavior 0 play animation 4 and switch to behavior 1, then play animation 7 with an effect; once the attack
    cooldown is over return to state 1
 */
VOID Em09_mv05(VOID);

/*
    action 6, drop: fall from the ceiling along the arc with animation 0x10, turning towards the player; on frame 0x19 with the player
    in range start the grab (action 4); at the end land on the floor as behavior 1
 */
VOID Em09_mv06(VOID);

/*
    action 7, swoop: jump from the floor to the ceiling along the arc with animation 0x10 moving forward; turn upside down as behavior 2;
    at the end set a 0x5A tick cooldown and animation 1 on the ceiling
 */
VOID Em09_mv07(VOID);

/*
    action 8, spit: play animation 0xE; on frames 0xA to 0xC with the player in range and in view hit it for 0x14 with effects and a
    sound; at the end set a 0x28 tick cooldown when Atck_flg, switch to behavior 0 and return to state 1
 */
VOID Em09_mv08(VOID);

/*
    action 9, ceiling walk: walk (animation 3, or running animation 2) with the heading mirrored by 0x800, turning towards the player,
    with a footstep sound on animation 2
 */
VOID Em09_mv09(VOID);

/*
    action 0xA, ceiling turn: as the turn action with the heading mirrored by 0x800
 */
VOID Em09_mv0a(VOID);

/*
    action 0xB, short toggle: play animation 4, switch between behavior 0 and 1 and return to state 1
 */
VOID Em09_mv0b(VOID);

/*
    action 0xC, claw: play animation 0xA; on frame 0xA grab the player (action 4) when in range, in view, unhit and facing, otherwise
    hit it for 0xA when Atck_flg and touching; at the end switch to behavior 1 and return to state 1
 */
VOID Em09_mv0c(VOID);

/*
    action 0xD (entered when the player's hit state has bit 7 set): play the animation of the behavior from the death animation table
    with a sound and hold it
 */
VOID Em09_mv0d(VOID);

/*
    hit reaction 0 to 3, stagger: play the animation set by the dispatcher with a sound, effect and the acid burst at low health;
    reactions 0 and 1 also move (backwards, forwards); at the end return to state 1 and half the time swoop (action 7), claw (0xC) when
    Atck_flg
 */
VOID Em09_dm00(VOID);

/*
    hit reaction 4 and 5, knock down: arc up with animation 0x10, fall to the floor with a sound, lie for a time that grows with the
    Damage taken (acid burst at low health), then turn and get up (animation 0x11), returning to state 1 and half the time swooping
 */
VOID Em09_dm01(VOID);

/*
    death variant 0 and 1: play the dying animation with an effect, sound, acid burst and the death event flag; then lock the shadow,
    fade it and shrink it, and raise the death event flag again when done
 */
VOID Em09_dm02(VOID);

/*
    death variant 4 (ceiling): fall along the arc with the acid burst and a sound, land, then lock the shadow, fade it and shrink it and
    raise the death event flag
 */
VOID Em09_dm03(VOID);

/*
    player animation while grabbed by a chimera: follow the grab, struggle (button presses lower the timer), recoil and get up; when
    the health runs out fade and shrink the shadow
 */
VOID Em09_pldm00(VOID);

/*
    spawn the acid effects: two bursts at joint 1 and five sprays fanned out around the direction to the player
 */
VOID Em09_Esp_set(VOID);

extern AT_TBL* pEm09_sca;                          // collision volume of the chimera
extern EM09_STATE_TBL Em09_state_tbl;               // state handlers by entity state and first hit reaction by hit type
extern VOID (*Em09_mv_tbl[14])(VOID);                // actions by Routine_2
extern VOID (*Em09_Move_br_tbl[3])(VOID);                    // decision routines by Type
extern VOID (*Em09_mv00_tbl[3])(VOID);             // idle routines by Type
extern EM09_ARC_TBL Em09_arc_tbl;                  // height and speed along the swoop and drop arc by animation frame
extern SCHAR Em09_mot_tbl[4];                       // animation of the hold action by Type

#endif
