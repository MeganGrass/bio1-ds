/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM0D.H
 */

#ifndef EM0D_H
#define EM0D_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include "SCRATCHPAD.H"

typedef struct EM0D_FREE EM0D_FREE;
typedef struct EM0D_BITE_TBL EM0D_BITE_TBL;
typedef struct EM0D_WAY_TBL EM0D_WAY_TBL;
typedef struct EM0D_DUST_TBL EM0D_DUST_TBL;

struct EM0D_FREE                         // yawn state kept in the Free[] area of its enemy work (offset 0x16C)
{
    SCHAR Sway_add;         // step added to Move_spd by Em0d_sway_add, flips sign at the limits, doubles until it reaches 64
    UCHAR Dummy1[1];        // padding
    UCHAR form;             // body form 0 or 1: selects the animation set and attack ranges, toggled by the rear action
    SCHAR Bite_cnt;         // bites since the last rear, 5 triggers a rear
    SHORT Swallow_scale;   // scale of the bulge drawn on the neck pSin_parts_ptr while a swallow is in progress, 0 = none
    SHORT Move_spd;         // sway offset passed to Add_speedXZ, oscillates between the limits of Em0d_sway_add
    SHORT Ground_y;         // ground height read from joint 7 at init
    SHORT Die_scale;        // body scale while shrinking away in the death sequence
    INT saved_state;        // state word saved by Em0d_Move, restored by Em0d_Damage
    SCHAR Bite_dir;         // turn direction while a bite is active: 1 on the strike, -1 after a hit, 0 = none
    UCHAR Bite_wait;        // frames of bite recovery, bit 7 flags the recovery
    UCHAR Way_idx;          // current index into the entry, flee or reposition path
    UCHAR Pl_low_flg;       // 1 when the player health is low in form 0
    USHORT Die_cnt;         // frames of the death effects played so far
    UCHAR wall_frames;      // frames spent in contact with the room collision, 0x97 triggers a reposition
    UCHAR Wall_wait;        // frames until wall_frames clears after the last wall contact
    UCHAR Dust_timer;       // frames of dust effects left
    UCHAR Swallow_wait;     // frames until another swallow is allowed
};

// the Free[] work area of the current entity (G.pEm) as this unit's EM0D_FREE
#define EM0D_FREE_P ((EM0D_FREE*)ENEMY_FREE(G.pEm))

struct EM0D_BITE_TBL                   // animation frames of a bite that can hurt the player, per form
{
    UCHAR first_frame;  // first animation frame of the window
    UCHAR Frm_num;      // length of the window in frames
};

struct EM0D_WAY_TBL                        // ground position on a path
{
    USHORT x;                           // x position
    USHORT z;                           // z position
};

struct EM0D_DUST_TBL                   // dust effect offset
{
    SHORT x;                            // x offset from joint 1
    SHORT z;                            // z offset from joint 1, sign randomised
};

/*
    state 0: initialise the yawn, set shadow, health and start animation, place the pSin_parts_ptr, set up the body segment entities and pick the start action
 */
VOID Em0d_Init(VOID);

/*
    per-frame update: run the state routine, copy segment positions from the pSin_parts_ptr, pass segment hits on to the yawn health, keep the joint chain on the segment, scale the swallow bulge and draw the shadows
 */
VOID Em0d_Main(VOID);

/*
    state 1: set alert and visual ranges, pick a new behavior when none is running, run the current action and save the state word
 */
VOID Em0d_Move(VOID);

/*
    state 2: damaged, restore the saved state when a bite or recovery is in progress, otherwise run the Damage reaction
 */
VOID Em0d_Damage(VOID);

/*
    state 3: death, run the death sequence
 */
VOID Em0d_Die(VOID);

/*
    state 4: wait until the wait flag is cleared, then start state 1
 */
VOID Em0d_Wait(VOID);

/*
    call the action routine of the current Routine_2
 */
VOID Em0d_Move_act(VOID);

/*
    measure the distance to the player and call the action selector for the behavior flags
 */
VOID Em0d_Move_br(VOID);

/*
    action selector: face the player and pick bite, swallow, rear, flee or reposition from distance, form, health and wall contact
 */
VOID Em0d_br_type00(VOID);

/*
    action selector for script controlled yawns: face the player and pick bite, swallow or rear from distance and form
 */
VOID Em0d_br_type01(VOID);

/*
    action 0: stand still with the idle animation for a random time, then move
 */
VOID Em0d_mv00(VOID);

/*
    action 1: advance toward the player with the walking animation, stepping and spawning dust
 */
VOID Em0d_mv01(VOID);

/*
    action 2: bite at the player, hurt the player inside the bite window, then recover and turn away
 */
VOID Em0d_mv02(VOID);

/*
    action 3: rear up, toggle the form and sway with the rear animation
 */
VOID Em0d_mv03(VOID);

/*
    action 4: swallow the player, carry the player in the mouth, play the swallow sequence and kill the player
 */
VOID Em0d_mv04(VOID);

/*
    action 5: entry, follow the entry path with dust effects and emerge once three waypoints have been reached
 */
VOID Em0d_mv05(VOID);

/*
    action 6: emerge from the ground with the effects, then settle and release the body segments
 */
VOID Em0d_mv06(VOID);

/*
    action 7: flee along the flee path, flagging the segment entities one by one, until the path ends
 */
VOID Em0d_mv07(VOID);

/*
    action 8: walk to the nearest point of the reposition path, then return to state 1
 */
VOID Em0d_mv08(VOID);

/*
    Damage reaction: play the hit animation, then turn away from the player and switch to form 1
 */
VOID Em0d_dm00(VOID);

/*
    advance the sway offset: reverse the step at the limits, otherwise double it until it reaches 64
 */
VOID Em0d_sway_add(VOID);

/*
    death sequence: fall animation, flag the death event, effects, then shrink and fade the body away
 */
VOID Em0d_die00(VOID);

/*
    player animation: call the routine for the current Routine_2 of the player
 */
VOID Em0d_pldie(VOID);

/*
    player animation: grabbed by the yawn, play the grab animation, then follow the yawn mouth joint
 */
VOID Em0d_pldie00(VOID);

/*
    pull a joint toward the end of its parent along the chain, turning at most Max_turn per call
 */
VOID Em0d_chain_follow(PARTS_W* pParent, PARTS_W* pP_ptr, SHORT Max_turn);

/*
    turn the parent joint toward the child joint, limited to 0x30 per call
 */
VOID Em0d_chain_align(PARTS_W* pParent, PARTS_W* pP_ptr);

/*
    advance the animation one frame with blending, move the pSin_parts_ptr and the root, return 1 when the animation ends
 */
UCHAR Em0d_anm_step(UCHAR reverse, SHORT Hokan_step);

/*
    set the joint matrices from the current animation frame without blending
 */
VOID Em0d_joint_set(VOID);

/*
    after movement: check player and wall collision, then follow the joint chain from the head
 */
VOID Em0d_Move_ck(SHORT Max_turn);

/*
    spawn a dust effect at one of the offsets of Em0d_dust_tbl
 */
VOID Em0d_dust_set(INT Ofs_idx);

extern AT_TBL* pEm0d_sca;                   // collision info pointer read by Em0d_Init
extern MATRIX Em0d_catch_m;                  // transform of the captured player while swallowed
extern SHORT Em0d_flee_timer;                       // frames spent fleeing
extern VOID (*Em0d_routine_0[5])(VOID);                  // state routine by state number
extern VOID (*Em0d_mv_tbl[9])(VOID);                 // action routine by Routine_2
extern VOID (*Em0d_br_tbl[7])(VOID);               // action selector by behavior flags (0 = none)
extern EM0D_BITE_TBL Em0d_bite_tbl[2];          // hurtful bite frames by form
extern EM0D_WAY_TBL Em0d_entry_tbl[6];                // waypoints of the entry path
extern EM0D_WAY_TBL Em0d_flee_tbl[9];                 // waypoints of the flee path
extern EM0D_WAY_TBL Em0d_repos_tbl[4];           // waypoints of the reposition path
extern PL_FUNC Pldie_Em0d_tbl[1];        // player animation routine by Routine_2
extern EM0D_DUST_TBL Em0d_dust_tbl[3];          // dust effect offsets

#endif
