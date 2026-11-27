/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM05.H
 */

#ifndef EM05_H
#define EM05_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include "SCRATCHPAD.H"

typedef struct EM05_FREE EM05_FREE;

struct EM05_FREE                         // crow state kept in the Free[] area of its enemy work (offset 0x16C)
{
    SHORT bank_turn_rate;   // turn step per frame while banking
    SHORT Sikai_flg;        // bit 0: pathfinding flag, scales steering toward the player and gates the peck hit
    SHORT Pl_hit;           // non-zero while the crow overlaps the player
    SHORT Up_lim;           // height (Pos_y) at which the climb ends
    SHORT Down_lim;         // height (Pos_y) at which the descent ends, also scales the dive speed
    SHORT vert_speed;       // vertical speed added to Pos_y each frame
    USHORT player_dist;     // horizontal distance to the player (|dx| + |dz|)
    USHORT room_hit;        // room collision result, non-zero while touching a wall
    USHORT wall_hit_frames; // frames spent touching a wall
    SHORT Unk_17e;          // set to 100 at init, not read by the unit
    UCHAR pad14[2];         // padding
    SHORT swerve_angle;     // obstacle Yoko_ofs angle, state of the obstacle avoidance
    SCHAR swerve_latch;     // obstacle Yoko_ofs latch, state of the obstacle avoidance
    UCHAR Dummy19[1];       // padding
    SCHAR Catch_cnt;        // grab hold gauge: starts at 100, drops each frame, the grab breaks below 0
    UCHAR Dummy1b[3];       // padding
    SHORT Lim_ofs;          // offset added to the descent and climb limits
};

// the Free[] work area of the current entity (G.pEm) as this unit's EM05_FREE
#define EM05_FREE_P ((EM05_FREE*)ENEMY_FREE(G.pEm))

// crow work field access: EM05_FREE overlay on the Free[] area of the current entity
#define EM05_EF(f) ENEMY_FREE_FIELD(EM05_FREE, f)

/*
    state 0: initialise the crow, pick its start behavior from the behavior flags, set the shadow, health and collision info
 */
VOID Em05_Init(VOID);

/*
    per-frame update: run the state routine, check overlap, room collision and camera zone, wrap the altitude, size and draw the shadow
 */
VOID Em05_Main(VOID);

/*
    state 1: normal flight, strike the player on contact, Yoko_ofs around walls, run the behavior routine and set the alert range by altitude
 */
VOID Em05_Move(VOID);

/*
    state 2: flutter down to ground height after striking the player, then play the recovery animation and resume flight
 */
VOID Em05_Damage(VOID);

/*
    state 3: death, set the kill flag, fall to the ground while the shadow shrinks, then burst the pSin_parts_ptr and set the final status flags
 */
VOID Em05_Die(VOID);

/*
    refresh the path flag, sample the facing toward the player and call the routine of the current behavior
 */
VOID Em05_mv_main(VOID);

/*
    behavior 0: forward to Em05_mv00_mot
 */
VOID Em05_mv00(VOID);

/*
    behavior 1: flap upward after take-off, then descend, or dive attack when already low
 */
VOID Em05_mv01(VOID);

/*
    behavior 2: dive toward the ground, then bank turn, strike run when the player is ahead, or perch when the player is dead
 */
VOID Em05_mv02(VOID);

/*
    behavior 3: bank turn with a random rate, then resume like the end of a dive
 */
VOID Em05_mv03(VOID);

/*
    behavior 4: wind-up run at the player, derive the speeds from the player distance at frame 9 and continue with ascend
 */
VOID Em05_mv04(VOID);

/*
    behavior 5: descending flight toward the descent limit, then dive attack, dive or peck depending on player position
 */
VOID Em05_mv05(VOID);

/*
    behavior 6: ascending flight toward the climb limit, then level out, or peck when the player is close below
 */
VOID Em05_mv06(VOID);

/*
    behavior 7: dive attack, forward speed from the player distance, then pitch up and continue with ascend from animation frame 19 or on a player hit
 */
VOID Em05_mv07(VOID);

/*
    behavior 10: level out of a dive, pitching back to flight, then descend
 */
VOID Em05_mv0a(VOID);

/*
    behavior 8: turn toward the player, then continue with ascend from animation frame 9
 */
VOID Em05_mv08(VOID);

/*
    behavior 9: peck, turn toward the player and on completion hurt the player and grab, or return to dive attack
 */
VOID Em05_mv09(VOID);

/*
    behavior 11: turn toward the player and move slowly while the grab animation runs
 */
VOID Em05_mv0b(VOID);

/*
    behavior 12: turn toward the player and sink at a growing vertical speed, then dive (entered when the player dies in the grab)
 */
VOID Em05_mv0c(VOID);

/*
    behavior 13: forward to Em05_mv0d_mot
 */
VOID Em05_mv0d(VOID);

/*
    animation: reset the animation counters on the first call, then keep playing the animation without a finish
 */
VOID Em05_seq_hold(VOID);

/*
    animation: play the animation once, then clear the sub state and reset the behavior to 0
 */
VOID Em05_seq_step(VOID);

/*
    animation: turn toward the player at the bank turn rate for a random time, then restart the run state and store the player position as waypoint
 */
VOID Em05_seq_turn(VOID);

/*
    animation: grab the player, drain player health every 20 frames, release when the hold gauge runs out or the player breaks away
 */
VOID Em05_seq_grab(VOID);

/*
    behavior 0: perched idle, wait a random time, play a random idle animation, then restart the state
 */
VOID Em05_mv00_mot(VOID);

/*
    behavior 13: take off, wait a random short time, then start the wingbeat with a flap sound and effects
 */
VOID Em05_mv0d_mot(VOID);

/*
    player animation: three-part multi-hit attack when grabbed by a crow, then restore the player state
 */
VOID Pl_atck_multi(VOID);

/*
    states 4 to 8: script-driven crow, fly to the base position or restart init as an active flier
 */
VOID Em05_Scenario(VOID);

/*
    fly toward the entity base position under script control, clear the behavior once within range
 */
VOID Em05_sce00(VOID);

extern AT_TBL Em05_sca;                // crow collision info
extern AT_TBL* pEm05_sca;           // pointer to the collision info assigned at init
extern VOID (*Em05_routine_0[9])(VOID);          // state routine by state number
extern VOID (*Em05_mv_tbl[14])(VOID);      // behavior routine by Routine_2 number

#endif
