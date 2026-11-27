/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM0C.H
 */

#ifndef EM0C_H
#define EM0C_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

typedef struct EM0C_SCALE_DATA EM0C_SCALE_DATA;
typedef struct EM0C_TRAIL_DATA EM0C_TRAIL_DATA;
typedef struct EM0C_CLUT_DATA EM0C_CLUT_DATA;
typedef struct EM0C_FREE EM0C_FREE;

struct EM0C_SCALE_DATA
{
    SHORT a;               // scale of the claw ghost across x and z (0x1000 = unscaled), pulsed between 0xBB8 and 0x1770
    SHORT b;                // scale of the claw ghost along y (claw length), pulsed with a, raised by rise and eased back by rush
};

struct EM0C_TRAIL_DATA
{
    USHORT timer;           // trail lifetime in update ticks; bit 15 requests the trail to be primed on the next draw
    SCHAR segments;         // number of trail records currently in use (0 to 8)
};

struct EM0C_CLUT_DATA
{
    USHORT x;               // x offset advanced by 0x10 on each run of the unused CLUT routine; never read
    USHORT y;               // y offset advanced by 0x10 on each run of the unused CLUT routine; never read
};

// per-entity state of the Tyrant, overlaid on ENEMY_WORK::Free
struct EM0C_FREE
{
    UCHAR room_hit;         // result of the room collision test, kept for the wander-turn steering
    UCHAR uv_scroll_step;   // index (0 to 29) into the body texture scroll tables
    UCHAR hit_mask;         // attack hit bits: 1=claw swipe, 2=claw slash or rush, 4=claw thrust, 8=backhand or charge, 0x80=Atck_flg
    UCHAR look_at_timer;    // head look-at Time_cnt: 0 looks at the player, 1 to 3 look-at off, 4 or more a fixed point
    ENEMY_WORK* heart;      // heart sub-entity, drawn by Em0c_draw_heart
    INT saved_state;        // state word stored at the end of each update tick and restored after a hit reaction
    UCHAR Dummy178;         // set to 3 at start-up, never read
    UCHAR flags;            // bit 0: shadow enabled, bit 1: look-at enabled, bit 2: attack already connected, bit 3: destroyed
    UCHAR pad0E[2];         // padding: keeps the following fields at their layout offsets
    USHORT Dir_ctrl_flg;    // direction flags from the entity collision check, passed to the wander-turn routine
    UCHAR Wander_rno;       // wander-turn steering state
    UCHAR attack_cooldown;  // ticks until the next attack may be chosen
    UCHAR rocket_flag;      // bit 7 set once the player's Type is 0xA; never read
    UCHAR special_timer;    // Time_cnt to the special move (rise) of id 0x10; reloaded to 0xD2 at start-up, after a hit and after attacks
    UCHAR close_frames;     // update ticks spent close to the player
    UCHAR frenzy_timer;     // ticks of frenzy left: extra steering and an Atck_flg attack choice
};

// the Free[] work area of the current entity (G.pEm) as this unit's EM0C_FREE
#define EM0C_FREE_P ((EM0C_FREE*)ENEMY_FREE(G.pEm))

/*
    initialise the entity: clear its collision and hit state, set up the shadow, health (600 and the erupt
    behavior for id 0x10, 220 otherwise), collision volume, heart sub-entity, work fields, start state and
    animation and head look-at limits; every id except 0xC also builds the two claw ghost copies and the claw
    trail
 */
VOID Em0c_Init(VOID);

/*
    advance the claw trail for this tick: build the claw joint matrices and push a trail record; on update ticks
    shorten the trail once its timer drops below the segment count and count the timer down
 */
VOID Em0c_trail_move(VOID);

/*
    update and draw the two claw ghost copies (not for id 0xC): on update ticks copy the claw transform into
    ghost 1, pulse the ghost scale and store the scaled transform in ghost 0, then queue both ghost models when
    the entity passes the camera switch zone test
 */
VOID Em0c_draw_claw_ghosts(VOID);

/*
    per-frame entry point: once initialised, switch to the scripted state when behavior flag 0x40 is set and do
    nothing more while behavior flag 0x80 is set; otherwise run the state handler, collision checks, body
    texture scroll and head look-at on update ticks, then draw the heart, claw ghosts, claw trail and shadow; id
    0x10 sets game flag 0x1F and holds health at 0xC8 once it drops below 0xC9
 */
VOID Em0c_Main(VOID);

/*
    state 1 handler: clear status flag bits 5 to 7 and set bit 6, run the two range checks against the player
    (0xFA0), then run the think step (sub-state 0, falling through to the act step) or the act step alone
 */
VOID Em0c_Move(VOID);

/*
    state 2 handler (hit reaction): on sub-state 0 restore the saved state word, set the look-at timer to 8 when
    it is 0, reload a running special timer and spawn a hit effect on the entity
 */
VOID Em0c_Damage(VOID);

/*
    state 3 handler (forced behavior): on sub-state 0 enter state 3 (behavior 1 for id 0x10), then run the
    behavior selected from Em0c_die
 */
VOID Em0c_Die(VOID);

/*
    state 4 handler: empty
 */
VOID Em0c_Dead(VOID);

/*
    act step: refresh the waypoint towards the player and run the current attack or movement behavior from
    Em0c_mv
 */
VOID Em0c_act(VOID);

/*
    think step: select the pause behavior when the player is dead, set hit_mask bit 7 when health is below 0x3C,
    count the update ticks the player stays within 0xFA0 (frenzy for 0x3C ticks and hit_mask bit 7 from the
    0x79th), then run the decision routine selected by id >> 2
 */
VOID Em0c_think(VOID);

/*
    decision routine for id 0xC: count down the attack cooldown, then choose claw slash or claw swipe by
    distance and facing (the swipe becomes an impale at low player health); frenzy makes the slash Atck_flg
    (hit_mask bit 7) and skips the swipe
 */
VOID Em0c_think_lab(VOID);

/*
    decision routine for id 0x10: count down the attack cooldown and special timer, then choose charge, claw
    slash, claw swipe, claw thrust or impale by distance and facing; rise when the special timer expires; impale
    also when game flag 0x1E is set and the player is close
 */
VOID Em0c_think_roof(VOID);

/*
    behavior 0: hold the idle animation for 0x3C ticks, then switch to the walk behavior
 */
VOID Em0c_Move_pause(VOID);

/*
    behavior 1: walk towards the waypoint with wander-turn steering (a second steering step while frenzied),
    moving by leg root motion with a speed boost and a footstep sound on frames 6 and 0x1F
 */
VOID Em0c_Move_walk(VOID);

/*
    behavior 2: empty
 */
VOID Em0c_Move_nop(VOID);

/*
    behavior 3, claw swipe (animation 5): advance towards the player with a speed that decays with the frame
    number, test the claw on frames 5 to 8 and apply 0x12 Damage on frame 8, starting the player's hit reaction;
    returns to walking afterwards
 */
VOID Em0c_Move_swipe(VOID);

/*
    behavior 4, claw slash (animation 4): test the claw on frames 7 to 11 and apply 0x12 Damage on frame 12 (id
    0x10 starts the player's fall reaction when the player survives); when the Atck_flg hit bit is set and the
    slash connected, chain into the backhand on frame 0x11
 */
VOID Em0c_Move_slash(VOID);

/*
    behavior 5, claw thrust (animation 7): test the claw on frames 8 to 15 and apply 0x14 Damage on frame 15,
    with leg root motion; turns the entity around when finished
 */
VOID Em0c_Move_thrust(VOID);

/*
    behavior 6, backhand (animation 3): test the claw on frames 4 to 7 and apply 0x12 Damage on frame 7; when
    connected after an Atck_flg slash or rush, chain into the claw swipe on frame 0xC (or the impale when the
    player's health is below 0x10)
 */
VOID Em0c_Move_backhand(VOID);

/*
    behavior 7, impale (animation 5): grab the player into the impale reaction, spawn effects on set frames,
    hold and move the player with the entity and release after the animation
 */
VOID Em0c_Move_impale(VOID);

/*
    behavior 8, charge (animation 2): run towards the player until close or blocked, then lunge (animation 9),
    testing the claw on frames 0xF to 0x13 and applying 0x1E Damage on frame 0x13, with effects around the claws
 */
VOID Em0c_Move_charge(VOID);

/*
    behavior 9, erupt (animation 6): spawn debris effects while counting down, then move the entity along the
    animation, lower it to the floor, enable the shadow and look-at and switch to the walk behavior
 */
VOID Em0c_Move_erupt(VOID);

/*
    behavior 0xA, rush (animation 2): run towards the player until close, blocked or timed out, then lunge
    (animation 0xB), testing the claw on frames 9 to 16 and applying 0x1E Damage on frame 17; when the
    Atck_flg hit bit is set and the lunge connected, chain into the backhand on frame 0x11
 */
VOID Em0c_Move_rush(VOID);

/*
    behavior 0xB, rise (animation 0xC): lengthen the claw scale from frame 0x29, move by root motion and
    continue with the rush behavior when the animation ends
 */
VOID Em0c_Move_rise(VOID);

/*
    forced behavior 0: play the restrained animation (8) with a sound on frames 0x19 and 0x32, set health to -1
    and raise the entity's death event
 */
VOID Em0c_Die_restrain(VOID);

/*
    forced behavior 1: raise the death event, hand control to the script (behavior flag 0x40) and reset to the
    scripted idle state
 */
VOID Em0c_Die_sce(VOID);

/*
    player animation while grabbed by the Tyrant's impale (state 7, sub-state 0xC): play animation 2 with hit
    effects and vibration on frames 8, 0x5C and from 0x60, and set health to -1 at the end
 */
VOID Pldie_Em0c(VOID);

/*
    dispatch the player's reaction to a Tyrant attack through Pldamage_Em0c_tbl by Routine_2
 */
VOID Pldamege_Em0c(VOID);

/*
    player reaction 0 (backhand or charge): knock the player back with hit sounds, effects and vibration, then
    return to the idle state
 */
VOID Em0c_pldm00(VOID);

/*
    player reaction 1 (claw swipe, a claw slash by an id other than 0x10, or a lethal claw slash, thrust or
    rush): knock the player back, slightly weaker than reaction 0, then return to the idle state
 */
VOID Em0c_pldm01(VOID);

/*
    player reaction 2 (claw slash by id 0x10, claw thrust or rush, when the player survives): slide the player
    back with a room collision check, then fall, lie and get up; a wall collision branches into a second fall
    animation
 */
VOID Em0c_pldm02(VOID);

/*
    compute the root motion of the current pose: build the entity matrix (applying its scale), concatenate the
    three leg joint matrices selected by Flg (0 or 1), set Spd_cur from the horizontal distance and,
    when apply is nonzero, subtract the offset from the entity position
 */
VOID Em0c_root_motion(UCHAR Flg, UCHAR apply);

/*
    draw the heart sub-entity when the Tyrant passes the camera switch zone test: make it the current entity, on
    update ticks pulse its scale from the heartbeat table (while alive), light it and queue its model, then
    restore the current entity
 */
VOID Em0c_draw_heart(VOID);

/*
    add du and dv to the texture coordinates of the body model primitives listed in Em0c_uv_scroll_poly_tbl,
    in both display buffers
 */
VOID Em0c_scroll_body_uvs(SHORT du, SHORT dv);

/*
    unused: read the model's CLUT rows back from VRAM, set the semi-transparency bit in each entry and write the
    rows out again
 */
VOID Em0c_clut_stp_set(VOID);

/*
    state 8 handler (scripted control): point the victim at the first enemy slot, then run the behavior from
    Em0c_sce_mv while behavior flag 0x40 is set; otherwise return to state 1 with look-at enabled
 */
VOID Em0c_Scenario(VOID);

/*
    scripted behavior 0: play the idle animation, clearing the claw trail state on entry
 */
VOID Em0c_Sce_idle(VOID);

/*
    scripted behavior 2: walk towards the position in Root_x and Root_z with footstep sounds; on
    arrival raise the script animation flag, then finish unless collision flag 0x80 is set (it then keeps
    walking)
 */
VOID Em0c_Sce_walk(VOID);

/*
    scripted behavior 0xA: break out through the glass (animation 6) with glass debris, sounds and vibration on
    frames 0x1E, 0x55 and 0x59, enable the shadow on frame 0x91, then raise the script animation flag
 */
VOID Em0c_Sce_glass(VOID);

/*
    scripted behavior 0xB: float in the pod, bobbing between Pos_y_tbl -0xC8 and -0x15E every 5 ticks until game
    flag 0x1F is set, then sink to the floor and raise the script animation flag
 */
VOID Em0c_Sce_pod(VOID);

/*
    scripted behavior 0xC: impale the victim entity (animation 5), spawning blood effects and sounds on frames 6
    and 0x5C and moving the victim with the entity; finish by raising game flag 0x1E
 */
VOID Em0c_Sce_impale(VOID);

/*
    scripted behavior 0xD (rocket death): mark the entity destroyed, spawn explosion effects and launch five
    limb pSin_parts_ptr as bouncing fragments, then advance the effects, limbs and the falling heart each tick and set
    health to -1 when finished
 */
VOID Em0c_Sce_rocket(VOID);

/*
    start a bouncing limb fragment on joint pP_ptr: take the vertical speed from pVel->vy and the horizontal
    speed from pVel->vx and pVel->vz, and set the gravity, bounce count and rotation
 */
VOID Em0c_limb_launch(PARTS_W* pP_ptr, SVECTOR* pVel, VOID* pRot, USHORT gravity, UCHAR bounces);

/*
    advance a limb fragment while bounces remain: add its velocity and gravity to the joint position, bounce
    with reversed, halved vertical speed at floor level (using up a bounce), and rebuild the joint transform
    from its rotation
 */
VOID Em0c_Limb_move(PARTS_W* pP_ptr);

/*
    scripted behavior 0xE: play animation 0xA with vibration, then raise the script animation flag
 */
VOID Em0c_Sce_anim(VOID);

/*
    scripted behavior 0xF: turn towards the player until aligned or a random tick counter (0x28 to 0x67) runs
    out, then raise the script animation flag and store the player position as the waypoint; sound on frames 3
    and 0x24
 */
VOID Em0c_Sce_face(VOID);

/*
    scripted behavior 0x10: claw strike (animation 4) at the victim entity, setting the victim's state with hit
    effects and sounds on frame 0xB, then raise game flag 0x1E
 */
VOID Em0c_Sce_strike(VOID);

extern AT_TBL* pEm0c_sca;                         // collision volume record assigned to the Tyrant in Em0c_Init
extern PARTS_W* pEm0c_ghost;                     // two claw ghost joint copies built in Em0c_Init
extern EM0C_SCALE_DATA Em0c_ghost_scale;                  // current claw ghost scale
extern SCHAR Em0c_ghost_add;                         // signed per-tick change of the claw ghost scale; reverses at the ends of its range
extern VOID* pEm0c_trail;                            // trail record block built in Em0c_Init
extern EM0C_TRAIL_DATA Em0c_trail_timer;                 // trail lifetime and segment count
extern SVECTOR Em0c_trail_near;                           // near edge point of the trail, relative to the claw joint
extern SVECTOR Em0c_trail_far;                            // far edge point of the trail, relative to the claw joint
extern VOID (*Em0c_routine_0[9])(VOID);                   // state handlers by entity state (0=init, 1=think/act, 2=hit, 3=forced, 4=empty, 8=scripted)
extern SCHAR Em0c_uv_scroll_u_tbl[];                      // per-step u offset of the body texture scroll
extern SHORT Em0c_uv_scroll_v_tbl[];                      // per-step v offset of the body texture scroll
extern VOID (*Em0c_die[2])(VOID);                // forced behaviors by Routine_2 (0=restrained, 1=yield to script)
extern VOID (*Em0c_mv[9])(VOID);         // state-1 behaviors by Routine_2 (values 9 to 0xB index into the think table that follows)
extern VOID (*Em0c_mv_think[10])(VOID);         // behaviors 9 to 0xB (entries 0 to 2: erupt, rush, rise, continuing the state-1 table), then the decision routines by id >> 2 (3=id 0xC, 4=id 0x10)
extern VOID (*Pldamage_Em0c_tbl[3])(VOID);              // player reactions to a Tyrant attack by Routine_2
extern SCHAR Em0c_heartbeat_tbl[];                        // per-tick heart scale change over one heartbeat
extern USHORT Em0c_uv_scroll_poly_tbl[];                  // indices of the body primitives scrolled by Em0c_scroll_body_uvs
extern EM0C_CLUT_DATA Em0c_clut_ofs;                    // offset advanced by the unused CLUT routine; never read
extern ENEMY_WORK* pEm0c_victim;                       // victim entity of the scripted impale and strike
extern VOID (*Em0c_sce_mv[17])(VOID);           // scripted behaviors by Routine_2

#endif
