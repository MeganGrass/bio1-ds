/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM06.H
 */

#ifndef EM06_H
#define EM06_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include "SCRATCHPAD.H"

typedef struct EM06_STATE_TBL EM06_STATE_TBL;
typedef struct EM06_INTRO_REC EM06_INTRO_REC;
typedef struct EM06_FREE EM06_FREE;
typedef struct EM06_REACH_REC EM06_REACH_REC;

// state handler table of the Hunter followed by the attack choice seed bytes
struct EM06_STATE_TBL
{
    VOID (*state[9])(VOID); // state handlers by entity state (0=init, 1=think, 2=act, 3=death, 4=empty, 8=scripted)
    UCHAR seed[8];          // Routine_2 base by (Damage_no & 7) - 1: attack kind chosen after a hit
};

// entry of Em06_intro_move_tbl
struct EM06_INTRO_REC
{
    SHORT x;                // x displacement per tick of the intro jump
    SHORT y;                // z displacement per tick of the intro jump
};

// work data of the Hunter, overlaid on ENEMY_WORK.Free
struct EM06_FREE
{
    UCHAR Dummy0[2];        // padding: keeps the following fields at their layout offsets
    SHORT Sikai_flg;        // latched result bit of the path update; non-zero starts the decapitation cooldown and enables chasing
    SHORT airborne;         // non-zero while leaping or dodging through the air
    UCHAR pad06[2];         // padding: keeps the following fields at their layout offsets
    ENEMY_WORK* npc_target; // other enemy entity followed by the variant 0 and 1 behavior
    SHORT leap_target_x;    // x of the decapitation leap target
    SHORT leap_target_z;    // z of the decapitation leap target
    USHORT decap_claw_joint; // joint of the claw used by the decapitation leap (6 or 9); 0 when none (unsigned: read with an unsigned halfword load)
    USHORT Dir_ctrl_flg;    // direction flags from the entity collision check, passed to the wander-turn routine
    UCHAR room_hit;         // result of the room collision test, kept for the wander-turn steering
    UCHAR pad15[1];         // padding: keeps the following fields at their layout offsets
    UCHAR decap_chance;     // non-zero when a decapitation attack may be started this tick
    SCHAR hit_window_timer; // ticks left of the window in which repeated swipes are counted
    UCHAR hit_window_count; // swipes started inside the current window
    SCHAR backstep_dir;     // backstep turn direction: 1=left, 2=straight, 3=right
    SCHAR Wander_rno;       // wander-turn steering state
    SCHAR approach_frames;  // ticks spent in the approach behavior
    SCHAR rsv_188;          // cleared at start-up and while deciding on a decapitation; never read
    SCHAR decap_cooldown;   // ticks until the next decapitation attack may be chosen
    UCHAR leap_followup;    // bit 0: dodge after the leap attack lands
};

// the Free[] work area of the current entity (G.pEm) as this unit's EM06_FREE
#define EM06_FREE_P ((EM06_FREE*)ENEMY_FREE(G.pEm))

// attack reach record of the dodge swipe, indexed from 2 down to 0
struct EM06_REACH_REC
{
    UCHAR first_frame;  // first animation frame of the hit window
    UCHAR pad1[1];      // padding: keeps the following fields at their layout offsets
    SHORT Frm_num;      // number of frames of the hit window
    SHORT joint;        // index of the claw joint tested
    SHORT Off_x;        // x offset of the test point from the joint
    SHORT radius;       // radius of the test sphere around the point
};

/*
    state 0: initialise the Hunter: set health from the random health table, collision volume (late table for
    Type bit 5 or stage 5 onwards), shadow, pSin_parts_ptr and idle animation, pick the npc target for the
    variant 0 and 1 behavior, clear the work fields and start in the intro action when Type bit 3 is set;
    lowers the player health gates when status flag 0x20000 is set
 */
VOID Em06_Init(VOID);

/*
    per-frame entry point: switch to the scripted state when Type bit 6 is set, count the hit window
    timer down and on update ticks run the state handler, entity collision checks and room collision, then place
    the shadow while the entity is inside a camera switch zone
 */
VOID Em06_Main(VOID);

/*
    state 1 handler: refresh status flags (alert range, player above or below), count the decapitation cooldown
    down, pick the next behavior with the variant AI when Key_rno is 0, then run the behavior update
 */
VOID Em06_Move(VOID);

/*
    state 2 handler: on sub-state 0 count the swipe window and choose the attack from the hit state and the player
    direction (leap attack while airborne, flurry when the player is above, pounce for the intro variant), then
    run the attack selected by Routine_2
 */
VOID Em06_Damage(VOID);

/*
    state 3 handler (dying): on sub-state 0 choose the death action from the hit state, airborne state and player
    height, set the death event flag and run the death action
 */
VOID Em06_Die(VOID);

/*
    death action: fall down (animation 0x11), flatten the shadow and recentre the entity on its hip joint
    once the blend has finished; plays the fall sounds on frames 0x24, 0x32 and 0x4B
 */
VOID Em06_Die_fall(VOID);

/*
    death action: thrash on the ground (animation 7) with a fast first three frames and the thrash sounds on frames 0xD and 0x17
 */
VOID Em06_Die_thrash(VOID);

/*
    death action: lie down (animation 0xF) and recentre the entity on its hip joint
 */
VOID Em06_Die_settle(VOID);

/*
    death action: collapse in place (animation 6) with the common death sequence
 */
VOID Em06_Die_collapse(VOID);

/*
    state 4 handler: empty
 */
VOID Em06_Dead(VOID);

/*
    run the path update, latch its result in Sikai_flg (starting the decapitation cooldown), refresh the
    waypoint towards the player and run the behavior selected by Routine_2
 */
VOID Em06_Move_main(VOID);

/*
    measure the distance to the player and run the variant AI selected by Type & 0xF (the coward
    check first when bit 4 is set)
 */
VOID Em06_variant_dispatch(VOID);

/*
    variant 0 AI (follows the npc target): fall back to variant 2 when Type is 0 or the target died; react to
    the target's hits, a blocked path or the player, and choose among approach, scream, attack, dodge and the
    decapitation leap
 */
VOID Em06_Move_type00(VOID);

/*
    variant 1 AI: restart the ledge jump behavior (action 8) whenever it is not already running
 */
VOID Em06_Move_type01(VOID);

/*
    variant 2 AI (hunts the player): update the decapitation chance from the player health and distance and start the
    decapitation leap, or approach, scream, attack or dodge depending on the distance and the player state
 */
VOID Em06_Move_type02(VOID);

/*
    variant 4 to 6 AI: clear the player-height status bits and, when the player is within 0xFA0, start the ledge
    jump behavior (action 8) and step Type back by 4
 */
VOID Em06_Move_type04(VOID);

/*
    coward check: clear Type bit 4 when the player is close (0xFA0) or the path flag is set and the player is within 0x2328
 */
VOID Em06_coward_ai(VOID);

/*
    idle behavior: run the idle action selected by Type & 0xF
 */
VOID Em06_Move_idle(VOID);

/*
    idle action: stand still (animation 0x15) for 0x1E ticks, then walk (action 1) unless Type bit 4 holds the idle
 */
VOID Em06_idle_stand(VOID);

/*
    idle action: walk in place (animation 1)
 */
VOID Em06_idle_walk(VOID);

/*
    behavior 1: run (animation 0x10) towards the waypoint with wander-turn steering, footstep sounds on frames 1 and 0x20 and
    a slow turn towards the player when closer than 0x7D0
 */
VOID Em06_Move_chase(VOID);

/*
    behavior 3: empty
 */
VOID Em06_Move_nop(VOID);

/*
    behavior 4: run the attack step selected by Act_sts, then recentre the entity on its hip joint
 */
VOID Em06_Move_atck(VOID);

/*
    attack step 0: start the swipe animation 5
 */
VOID Em06_atk_start(VOID);

/*
    attack step 1: play the swipe and test the claw against the player on frames 4 to 0xB; a hit damages the player by 0xD,
    plays sound and vibration and moves to the lunge step; a swipe that ends without a hit returns to the think state
 */
VOID Em06_atk_swing(VOID);

/*
    attack step 2: lunge forward at speed 0xC8, spawn a hit effect on the claw joint and tint it
 */
VOID Em06_atk_lunge(VOID);

/*
    attack step 3: finish the animation and push the player along the entity's speed for 4 ticks
 */
VOID Em06_atk_recover(VOID);

/*
    attack step 4: return to the think state, choosing the chase or the dodge behavior from a random roll (always chase for behavior variant 7)
 */
VOID Em06_atk_finish(VOID);

/*
    behavior 5 (decapitation leap): turn towards the leap target during the first 0x17 frames and step forward until
    frame 0x1F, running the sub-step selected by Act_sts
 */
VOID Em06_Move_decap(VOID);

/*
    leap step 0: start the leap animation for the chosen claw joint with the leap sound
 */
VOID Em06_decap_leap_start(VOID);

/*
    leap step 1: on frames 7 to 0xE test the claw against the player; a hit grabs the player into the decapitation
    state with blood effect, tint, sound and vibration, otherwise continue the animation
 */
VOID Em06_decap_strike(VOID);

/*
    leap step 2: return to the think state and clear the claw joint
 */
VOID Em06_decap_leap_end(VOID);

/*
    leap step 3: carry the held player on the claw joint while the animation plays
 */
VOID Em06_decap_leap_track(VOID);

/*
    leap step 4: switch to the hold behavior (action 7)
 */
VOID Em06_decap_leap_to_hold(VOID);

/*
    behavior 6 (dodge): run the dodge step selected by Act_sts and move along the facing
 */
VOID Em06_Move_dodge(VOID);

/*
    dodge step 0: start the jump animation 0x13 and mark the entity airborne
 */
VOID Em06_dodge_start(VOID);

/*
    dodge step 1: play the animation until frame bit 2 is set, then raise the jump speed to 0x82 with a sound
 */
VOID Em06_dodge_hop(VOID);

/*
    dodge step 2: set the high status bit from frame 9, test the claw against the player (Damage 0x14 with blood effect,
    sound and vibration) and fly with Em_fall_move until landing
 */
VOID Em06_dodge_swipe(VOID);

/*
    dodge step 3: finish the animation then return to the approach behavior
 */
VOID Em06_dodge_end(VOID);

/*
    dodge step 4: land, with the high status bits set from frame 9; return to the approach behavior on touching the ground
 */
VOID Em06_dodge_land(VOID);

/*
    behavior 7 (hold the player): play the grab animation, carry the player on the claw joint, then the release
    animation for 5 ticks and return to the think state
 */
VOID Em06_Move_catch(VOID);

/*
    behavior 2: walk fast towards the player with wander-turn steering and footstep sounds, then switch to behavior 1 after
    0x5A ticks
 */
VOID Em06_Move_yori(VOID);

/*
    behavior 8 (ledge jump): jump (animation 0) with a ballistic arc, land on the next animation and return to the chase
    behavior; script-driven entities raise the script animation flag instead
 */
VOID Em06_Move_jump(VOID);

/*
    behavior 9 (scream): play animation 0x17 with a scream on frame 8, then return to idle
 */
VOID Em06_Move_ikaku(VOID);

/*
    behavior 10 (backstep): back away (animation 0x14) while turning sideways, then return to the think state
 */
VOID Em06_Move_back(VOID);

/*
    behavior 11 (intro): play the intro jump animation of the variant, moving the entity by the jump displacement of the
    room's jump kind from Em06_intro_move_tbl and turning it when finished
 */
VOID Em06_Move_tojo(VOID);

/*
    act 0, 1 and 3 (swipe): swipe at the player (animation 3 or 4) and then choose the next move from the distance and
    random bits; script-driven entities spawn effects on the first tick and raise the script animation flag at the end
 */
VOID Em06_act_swipe(VOID);

/*
    act 4 (leap attack): jump (animation 7), land (0x12), crouch (8) for a random time and optionally lash out
    again (0xF), moving the entity along its speed; ends in the think state
 */
VOID Em06_act_leapattack(VOID);

/*
    act 2 and 8 (pounce): run the pounce animation 7 with a fast first three frames, steering the entity in a half or
    quarter turn during the intro variants 8 and 9; when it ends continue in the leap attack at its step 6, script-driven
    entities raise the script animation flag instead
 */
VOID Em06_act_pounce(VOID);

/*
    act 5 (flurry): claw animation 6 with effects on joint 3; when it ends restart it and continue in the leap attack
    at its step 6, script-driven entities raise the script animation flag instead
 */
VOID Em06_act_flurry(VOID);

/*
    act 6 (hold the player): play the grab animation 0x16 with the player held, then return to the think state
 */
VOID Em06_act_holdplayer(VOID);

/*
    common death sequence: set health to -1, play the animation, fade the shadow, set the death event flag and count
    down before ending
 */
VOID Em06_Die_main(VOID);

/*
    death action: leap and land (animations 0x11 and 0x12) with sounds, then hand over to the fall death action (action 0) at its shadow-fade step
 */
VOID Em06_Die_pounce(VOID);

/*
    player animation: the decapitation death: play the grab animation with blood effects, tints, sounds and
    vibration, then fade out and end in the dead state
 */
VOID Pldie_Em06(VOID);

/*
    carry the player on the claw joint: rebuild the joint transforms of the entity up to the claw joint, copy the
    claw position to the player's first joint and offset it by 0xC8 along the claw direction
 */
VOID Em06_Catch_set(UCHAR claw_joint);

/*
    recentre the entity on a joint: rebuild the joint transforms and subtract the joint's displacement
    from the entity position; Flg selects the joint set (0 or 1)
 */
VOID Em06_recenter_on_joint(UCHAR Flg);

/*
    state 8 handler (scripted): leave the scripted state when Type bit 6 is cleared, otherwise point the
    victim at the first enemy slot and run the scripted behavior selected by Routine_2
 */
VOID Em06_Scenario(VOID);

/*
    scripted behavior 0: stand idle (animation 0x15)
 */
VOID Em06_Sce_idle(VOID);

/*
    scripted behavior 2: walk (animation 0x10) to the position in Root_x and Root_z with footstep sounds;
    on arrival raise the script animation flag and finish
 */
VOID Em06_Sce_walk(VOID);

/*
    scripted behavior 3: run (animation 2) to the position in Root_x and Root_z with footstep sounds;
    on arrival raise the script animation flag and finish
 */
VOID Em06_Sce_run(VOID);

/*
    scripted behavior 0xA: run the scripted dodge step selected by Act_sts and move along the facing
 */
VOID Em06_Sce_dodge(VOID);

/*
    scripted dodge step 3: play the animation then raise the script animation flag and finish
 */
VOID Em06_Sce_dodge30(VOID);

/*
    scripted behavior 0xB: run the scripted decapitation leap step selected by Act_sts, turning to the leap target
    for 0x17 frames and stepping forward until frame 0x1F
 */
VOID Em06_Sce_decap(VOID);

/*
    scripted leap step 2: raise the script animation flag and finish
 */
VOID Em06_Sce_decap20(VOID);

/*
    scripted behavior 0xD: run the scripted swipe step selected by Act_sts, then recentre the entity on its hip joint
 */
VOID Em06_Sce_swipe(VOID);

/*
    scripted swipe step 2: tint the claw joint, raise the script animation flag and finish
 */
VOID Em06_Sce_swipe20(VOID);

/*
    scripted behavior 0xC: run the scripted bite step selected by Act_sts, turning to the target for 0x17 frames
    and stepping forward until frame 0x1F
 */
VOID Em06_Sce_bite(VOID);

/*
    scripted bite step 0: start the lunge animation 0xC
 */
VOID Em06_Sce_bite00(VOID);

/*
    scripted bite step 1: on frames 7 to 0xE, unless the victim is already hit, mark the victim as hit and reset its
    state, spawn a blood effect, tint and sound on the claw joint and move to the bite end step
 */
VOID Em06_Sce_bite10(VOID);

/*
    scripted bite step 2: finish the animation, then raise the script animation flag and switch to scripted
    behavior 7; carry the victim on the claw joint
 */
VOID Em06_Sce_bite20(VOID);

/*
    scripted behavior 7 (hold the victim): play two animation steps in turn while carrying the victim on the claw joint
 */
VOID Em06_Sce_catch(VOID);

/*
    carry the victim entity on the claw joint: rebuild the joint transforms of the entity up to joint 6 and place
    the victim's first joint at the claw position offset by 0xC8
 */
VOID Em06_Sce_catch_set(SCHAR claw_joint);

/*
    scripted behavior 0x16: look around for a random 0x1E to 0x5D ticks (turning towards the player when it is out of
    sight), then raise the script animation flag and store the player position as the waypoint
 */
VOID Em06_Sce_stalk(VOID);

extern AT_TBL* pEm06_sca;                // collision volume of the Hunter
extern AT_TBL* Em06_sca_late;               // collision volume used from stage 5 onwards or with Type bit 5
extern UCHAR Em06_life[];                  // starting health, picked by a random index 0 to 15
extern UCHAR Em06_decap_chance_mid[];            // decapitation chance flags by random index while health is below 0x4F
extern UCHAR Em06_decap_chance_low[];            // decapitation chance flags by random index while health is below 0x28
extern UCHAR Em06_pl_life_lim[4];          // player health below which decapitation may start, by player id & 1
extern EM06_STATE_TBL Em06_routine_0;      // state handlers of the Hunter and attack choice seed bytes
extern EM06_INTRO_REC Em06_intro_move_tbl[6];   // intro tables: 0=animation ids, 1=facing change, 2 to 5=jump displacement by kind
extern UCHAR Em06_jump_kind;                // intro jump kind of the room: 2 to 5, selects the jump displacement
extern ENEMY_WORK* pEm06_victim;              // victim entity of the scripted bite
extern VOID (*Em06_dm[9])(VOID);            // act handlers by Routine_2 in state 2
extern VOID (*Em06_die[7])(VOID);          // death actions by Routine_2 in state 3
extern VOID (*Em06_mv[12])(VOID);      // behaviors by Routine_2 in state 1
extern VOID (*Em06_Move_type[16])(VOID);       // variant AI handlers by Type & 0xF
extern VOID (*Em06_idle_variant_tbl[8])(VOID);   // idle actions by Type & 0xF
extern VOID (*Em06_attack_sub_tbl[5])(VOID);     // attack steps by Act_sts
extern VOID (*Em06_decap_leap_sub_tbl[5])(VOID); // decapitation leap steps by Act_sts
extern EM06_REACH_REC Em06_swipe_reach_tbl[3]; // dodge swipe reach records
extern VOID (*Em06_dodge_sub_tbl[5])(VOID);      // dodge steps by Act_sts
extern VOID (*Em06_sce_mv[24])(VOID);     // scripted behaviors by Routine_2
extern VOID (*Em06_sce_dodge_mv[4])(VOID);      // scripted dodge steps by Act_sts
extern VOID (*Em06_sce_decap_mv[3])(VOID); // scripted decapitation leap steps by Act_sts
extern VOID (*Em06_sce_swipe_mv[3])(VOID);      // scripted swipe steps by Act_sts
extern VOID (*Em06_sce_bite_mv[3])(VOID);     // scripted bite steps by Act_sts

#endif
