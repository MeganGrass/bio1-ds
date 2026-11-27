/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM08.H
 */

#ifndef EM08_H
#define EM08_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

typedef struct EM08_FREE EM08_FREE;
typedef struct EM08_BODY_FREE EM08_BODY_FREE;

// work data of the plant entity, overlaid on ENEMY_WORK.Free
struct EM08_FREE
{
    SCHAR Yaw_add;              // idle: DirY angle step per frame; suspended: change of Yaw_swing per swing
    SCHAR Tilt_add;             // idle: tilt angle step per frame; suspended: change of Tilt_swing per swing
    USHORT Yaw_swing;           // suspended: DirY swing amplitude; death: DirY angle to stop at
    USHORT Tilt_swing;          // suspended: tilt swing amplitude; death: tilt angle to stop at
    SHORT Rot_spd;              // rotation speed of the current action (sweep, head strike, recoil, hold)
    SCHAR Sweep_dir;            // sweep direction: index into the sweep table, 0 or 1
    SCHAR Catch_parts;          // joint that holds the player; recoil reuses it as the swing step
    SCHAR Dummy176;             // cleared at start-up; never read
    SCHAR Eff_timer;            // frames left until the next hit effect while script-driven
    ENEMY_WORK* roots;          // roots entity, drawn together with the body
    UINT player_distance;       // distance to the player on the ground plane
    SHORT ambient_effect_timer; // frames left until the next ambient effect
    SCHAR Wilt_timer;           // death: frames of wilting before the fall
    SCHAR Bounce_cnt;           // death: floor contacts left before the plant stays down
};

// the Free[] work area of the current entity (G.pEm) as this unit's EM08_FREE
#define EM08_FREE_P ((EM08_FREE*)ENEMY_FREE(G.pEm))

// work data of the body entity, overlaid on ENEMY_WORK.Free
struct EM08_BODY_FREE
{
    INT Scale_x;                    // x scale of the body model (0x1000 = unit scale)
    INT Scale_y;                    // y scale of the body model (0x1000 = unit scale)
    INT Scale_z;                    // z scale of the body model (0x1000 = unit scale)
};

// the Free[] work area of body entity p as its EM08_BODY_FREE
#define EM08_BODY_OF(p) ((EM08_BODY_FREE*)(p)->Free)

/*
    initialise the plant: reset state, shadow and health and select the vine collision table; unless
    Type bit 0 is set (vine) also allocate the body and roots entities with Set_enemy_work, switch to
    the main collision table and make the body the aim target of every enemy slot; then apply the Type
    variants (0x40 script-driven, 4 inert with the body already fallen, 6 starting in the suspend action,
    5 enlarged)
 */
VOID Em08_Init(VOID);

/*
    per-frame enemy update: when Stop_flg bit 2 is set, run the state handler from Em08_routine_0
    (forcing the script-driven state for Type 0x40) and copy the head joint offset into the collision
    data; draw the body and roots unless a vine; for Type 4 clear Zone_in_flg and stop,
    otherwise resize and place the shadow under the plant
 */
VOID Em08_Main(VOID);

/*
    state 1 handler; forwards to Em08_Move_main
 */
VOID Em08_Move(VOID);

/*
    state 1: refresh the status flags and the distance to the player; enter the damaged state when the body
    was hit, otherwise pick a new action while Key_rno is 0; run the current action and count down the
    ambient effect timer, spawning ambient effects around the player
 */
VOID Em08_Move_main(VOID);

/*
    state 2: react to Damage and keep the ambient effect timer and its effects running as in state 1
 */
VOID Em08_Damage(VOID);

/*
    enter the dying state when the body is dead; on a registered hit select the recoil or flinch action and
    spawn impact effects; a vine hit at low health also enters the dying state and counts down the body
    counter, and at 1 every other live enemy slot is set to the wither action
 */
VOID Em08_Dm_sel(VOID);

/*
    empty function
 */
VOID Em08_Nop(VOID);

/*
    state 3 (dying): count down the body counter and wither the other enemies at 1; for the main plant mark
    the body and roots dead; run the death behavior when Routine_2 is 0
 */
VOID Em08_Die(VOID);

/*
    state 4: empty
 */
VOID Em08_Dead(VOID);

/*
    run the current action from Em08_act_tbl; enter the dying state instead when the body is dead
 */
VOID Em08_Act_run(VOID);

/*
    store the distance to the player in the scratchpad and call the action selector chosen by
    Type & 7
 */
VOID Em08_Sel_run(VOID);

/*
    choose the next action from the distance to the player: turn toward the player when near, start a sweep
    or head strike when closer and facing it, or a random sweep, head strike or reach when closer still
 */
VOID Em08_Sel_def(VOID);

/*
    empty function
 */
VOID Em08_Act_nop(VOID);

/*
    action selector for Type 4: empty
 */
VOID Em08_Sel_nop(VOID);

/*
    action selector for Type 5: as Em08_Sel_def, with only a sweep or a head strike as the
    random choice
 */
VOID Em08_Sel_type5(VOID);

/*
    action 0, idle: sway the head with random DirY and tilt steps while playing an idle animation, and pick a
    new random idle animation when the head sinks too low or the timer runs out
 */
VOID Em08_Act_idle(VOID);

/*
    action 1, sweep: wind up, swing the head around the plant with growing speed, hit the player through
    Em08_Sweep_hit, then slow down and return to state 1
 */
VOID Em08_Act_sweep(VOID);

/*
    test the swinging head against the player; a hit within range, facing the player at speed, starts the
    knockdown or throw reaction of the player and deducts 0x19 health (a negative result becomes 1)
 */
VOID Em08_Sweep_hit(VOID);

/*
    action 2, head strike: raise the head and lunge at the player, turning toward it; a hit by joint 15 puts
    the player into its hit state and deducts 0xF health; then lower the head and return to state 1
 */
VOID Em08_Act_spit(VOID);

/*
    action 3, reach: sway toward the player; when the head is close and the player stands on the ground and
    is not already hit, grab it (grabbed reaction), pull it to the head and hand over to action 4; otherwise
    give up after a timeout and return to state 1
 */
VOID Em08_Act_move(VOID);

/*
    action 4, hold: keep the grabbed player attached to the grab joint through the capture matrix while
    lifting and lowering it, then drop it with the thrown reaction and 0x28 health deducted; switch to action
    8 when the player has less than 0x28 health
 */
VOID Em08_Act_hold(VOID);

/*
    action 5, alternate grab: turn toward the player, pull it to joint 15 and keep it attached through the
    capture matrix
 */
VOID Em08_Act_grab(VOID);

/*
    action 6, suspend: keep the player attached to joint 15 and swing it, with swing amplitudes that grow and
    shrink on both axes, triggering a vibration every 20 frames
 */
VOID Em08_Act_suspend(VOID);

/*
    action 7, release: put the held player down at a fixed position with the thrown reaction, then return to
    state 8
 */
VOID Em08_Act_release(VOID);

/*
    action 8, kill: shake the head with the held player attached, spawn effects, put the player into its
    death state and set its health to -1
 */
VOID Em08_Act_kill(VOID);

/*
    action 9, wither: shrink the plant while the body and roots shrivel, wait, grow again, then clear the
    scale (0) and return to state 1
 */
VOID Em08_Act_wither(VOID);

/*
    recoil from a hit: play a random flinch animation while swaying the head, or retreat with a sound; return
    to state 1 (state 8 when script-driven) when done and re-enable hits
 */
VOID Em08_Act_recoil(VOID);

/*
    flinch: play the flinch animation forward then backward, tilting the head while hit; when script-driven
    also spawn hit effects on random pSin_parts_ptr; return to state 1 afterward (state 8 when script-driven)
 */
VOID Em08_Act_flinch(VOID);

/*
    death: wilt the plant while shrinking, drop it to the floor and bounce it with effects on the pSin_parts_ptr,
    then shrink it away; a vine also clears its enter-switch-zone flag
 */
VOID Em08_Act_death(VOID);

/*
    player animation 46, used when the plant kills the player: effects, sounds and vibration on pSin_parts_ptr 0 to
    2, then swing pSin_parts_ptr 0 and 2 with accumulating velocity until they stop, with a burst of effects at the
    end; finish by playing animation 0
 */
VOID Pl_anim_limb_bounce(VOID);

/*
    player animation 27, the plant reactions: call the handler chosen by Routine_2 from
    Em08_pl_dm_tbl
 */
VOID Em08_Pl_dm(VOID);

/*
    player reaction 0, knocked down: slide with decaying speed while the knockdown, rise and stand animations
    play, with dust effects; return to state 1 when finished
 */
VOID Em08_Pl_down(VOID);

/*
    player reaction 1, grabbed: play the struggle animations with a vibration at the start of each, and burst
    effects when the grab ends
 */
VOID Em08_Pl_catch(VOID);

/*
    player reaction 2, thrown: play the thrown animation with effects and vibration, then slide with decaying
    speed along the heading stored in the capture matrix
 */
VOID Em08_Pl_throw(VOID);

/*
    draw the body and roots: for each make it the current entity, run its state handler, build its matrices
    and scale and queue its polygons (the body also lights and casts a shadow), then restore the entity
 */
VOID Em08_Disp_copy(VOID);

/*
    body state 0, idle: pulse the body scale from Em08_pulse_tbl and sway the body in DirY and tilt
 */
VOID Em08_Body_idle(VOID);

/*
    body state 1, shrivel: shrink the body scale with a jitter (Act_sts 0 and 1), pause (2) or grow it
    back (3 and 4), resizing the shadow and spawning effects
 */
VOID Em08_Body_shrivel(VOID);

/*
    body state 2, fall: drop the body to the floor with growing speed, then squash it with a burst of
    effects and sounds and jitter the scale for a while
 */
VOID Em08_Body_fall(VOID);

/*
    roots state 0, idle: wrap the pulse counter
 */
VOID Em08_Roots_idle(VOID);

/*
    roots state 1, pulse: shrink the roots scale with a jitter (Act_sts 0 and 1), pause (2) or grow it
    back (3 and 4)
 */
VOID Em08_Roots_pulse(VOID);

/*
    roots state 2, rise: move the roots with growing speed until the limit and shrink the scale toward its
    target
 */
VOID Em08_Roots_rise(VOID);

/*
    advance the animation of the current entity: read the pose for Move_no and Move_cnt (played
    backward when reverse is set), blend into it using Hokan_step and update the joint transforms; returns 1
    when the animation reached its last frame and wrapped, otherwise 0
 */
UCHAR Em08_Joint_move(UCHAR reverse, VOID* pKan_t_ptr, VOID* pSeq_t_ptr, SHORT Hokan_step);

/*
    state 8 (script-driven): run the action from Em08_sce_act_tbl, spawn hit effects while the effect
    timer runs, and return to state 1 unless Type has bit 0x40
 */
VOID Em08_Scenario(VOID);

/*
    script action 2: turn toward the player and set the event flag of Set_flg_no once facing it, then idle
 */
VOID Em08_Sce_face(VOID);

/*
    script action 13: start the hit effect timer on entry, then recoil
 */
VOID Em08_Sce_recoil(VOID);

/*
    script action 14: count down the body counter (skipping 1); for the main plant mark the body and roots
    dead; then run the death behavior
 */
VOID Em08_Sce_death(VOID);

extern AT_TBL Em08_sca_vine[2];                // collision table of the vine: two AT_TBL records
extern AT_TBL Em08_sca_main[2];                // collision table of the main plant: two AT_TBL records
extern AT_TBL* pEm08_sca_vine;           // collision table of the vine
extern AT_TBL* pEm08_sca_main;           // collision table of the main plant
extern MATRIX Em08_catch_mat;               // transform of the held player relative to its grab joint; t[0] doubles as the player heading
extern VOID (*Em08_routine_0[])();               // state handlers, by entity state
extern SHORT Em08_esp_ofs_x[];                    // x offsets of the ambient effects
extern SHORT Em08_esp_ofs_z[];                    // z offsets of the ambient effects
extern VOID (*Em08_act_tbl[])();              // action handlers, by Routine_2
extern VOID (*Em08_sel_tbl[])();       // action selectors, by Type & 7
extern UCHAR Em08_idle_mot_tbl[];               // animation ids picked at random for idle
extern MATRIX Em08_hold_mat;            // capture matrix preset used by the suspend action when the player id bit 0 is clear
extern VOID (*Em08_pl_dm_tbl[])();          // player reaction handlers, by Routine_2
extern UCHAR Em08_pulse_tbl[];                   // scale pulse curve, indexed by 0x59 - Timer0 (the first 90 entries are used)
extern VOID (*Em08_body_tbl[])();          // body state handlers, by body state
extern VOID (*Em08_roots_tbl[])();   // roots state handlers, by roots state
extern VOID (*Em08_sce_act_tbl[])();          // script-driven action handlers, by Routine_2

#endif      // EM08_H
