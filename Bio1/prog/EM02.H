/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM02.H
 */

#ifndef EM02_H
#define EM02_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include "SCRATCHPAD.H"

typedef struct EM02_FREE EM02_FREE;
typedef struct EM02_WALK_REC EM02_WALK_REC;

// private work area of a Cerberus (dog), stored in the Free area of its ENEMY_WORK
struct EM02_FREE
{
    INT player_dist;            // distance to the player (sum of the x and z differences)
    USHORT turn_step;           // turn rate per frame used when steering
    SHORT turn_delta;           // signed turn applied per frame while turning or strafing
    SHORT Leap_spd_y;           // initial vertical speed of a jump or knock-back
    SHORT ballistic_gravity;    // vertical acceleration of a jump or knock-back; holds the cached turn while chasing
    USHORT Ck_flg;              // obstacle probe results: bit 0 blocked ahead now, bits 1-3 older frames, bit 4 stuck
    UCHAR Path_ck;              // result flags of the path update for the current frame
    UCHAR Dummy0f[1];           // padding: keeps the following fields at their layout offsets
    USHORT Sence_cnt;           // awareness or contact counter (stalking, chase blocking, leap result)
    SHORT Step_cnt;             // frame counter within the current step of a behaviour
    SHORT Yoko_ofs;             // lateral steering or head swivel offset; counts frames in some behaviours
    SHORT Blood_cnt;            // blood effects still to spawn
    SCHAR Sence_flg;            // nonzero once the dog has noticed the player; bit 7 marks a staggered hit
    UCHAR Dummy19[1];           // padding: keeps the following fields at their layout offsets
    USHORT Move_flg;            // behaviour flags: bit 0 variant, bit 1 stalking, bit 2 Atck_flg
    USHORT Ai_flg;              // AI flags: bit 0 steer retarget, bit 1 player ahead, bits 2-3 skid request, bit 4 wall follow done, bit 5 recently hit, bit 7 restart animation
    SHORT Tgt_timer;            // frames before the chase target is refreshed
};

// the Free[] work area of the current entity (G.pEm) as this unit's EM02_FREE
#define EM02_FREE_P ((EM02_FREE*)ENEMY_FREE(G.pEm))

struct EM02_WALK_REC
{
    UCHAR anim_id;      // walk animation id
    UCHAR pad1;         // padding: keeps the following field at its layout offset
    USHORT Move_spd;    // walk speed
};

// access a field of the Cerberus work area of the current enemy
#define EM02_EF(field) ENEMY_FREE_FIELD(EM02_FREE, field)

/*
    update the dog for one frame: run the state handler, update object and enemy collision,
    track the obstacle probe history, test the camera switch zones and place the shadow
 */
VOID Em02_Main(VOID);

/*
    state 0: start a new dog with random health, clear its work area, initialise the parts and
    shadow, and begin airborne or on the floor depending on the placed behaviour
 */
VOID Em02_Init(VOID);

/*
    state 1: refresh the player distance and path result, pick a behaviour when the sub-state is 0,
    then run the handler of the current sub-state
 */
VOID Em02_Move(VOID);

/*
    pick the next sub-state from the behaviour id through the select table; behaviour 0x80 is script controlled
 */
VOID Em02_Move_set(VOID);

/*
    enter the script-controlled sub-state
 */
VOID Em02_mv08_set(VOID);

/*
    enter patrol (behaviours 0-3): walk animation and speed from the walk parameters;
    behaviour 3 enters stalk instead
 */
VOID Em02_mv01_set(VOID);

/*
    enter chase: reset the counters, pick a turn rate and take the player position as the waypoint
 */
VOID Em02_mv02_set(VOID);

/*
    enter leap (behaviours 5-7): set the jump speed, gravity and animation for a leap at the
    player or a scripted entrance jump
 */
VOID Em02_mv03_set(VOID);

/*
    enter the bite sub-state
 */
VOID Em02_mv07_set(VOID);

/*
    enter strafe: pick a random side when there is no turn direction yet
 */
VOID Em02_mv04_set(VOID);

/*
    enter alert: idle animation with a short timer
 */
VOID Em02_mv05_set(VOID);

/*
    enter stalk: creeping animation at a random frame, no movement, flag as stalking unless already flagged Atck_flg
 */
VOID Em02_mv09_set(VOID);

/*
    chase sub-state: run the chase handler, consider attacking, step the animation and play footsteps
 */
VOID Em02_mv02(VOID);

/*
    chase handler 0: speed up and steer toward the waypoint; detour or switch to wall following when blocked
 */
VOID Em02_mv02_run(VOID);

/*
    chase handler 1: follow the wall around an obstacle until the way is clear
 */
VOID Em02_mv02_turn(VOID);

/*
    chase handler 2: brake while turning toward the waypoint, then resume running
 */
VOID Em20_Mv_run20(VOID);

/*
    leap sub-state: run the leap handler and spawn blood
 */
VOID Em02_mv03(VOID);

/*
    leap handler 0: crouch until the timer ends, then launch with a bark
 */
VOID Em02_mv03_crouch(VOID);

/*
    leap handler 1: follow the jump arc, bite or grab the player on contact, start landing once falling
 */
VOID Em02_mv03_air(VOID);

/*
    leap handler 2: fall until the floor is reached, then play the landing
 */
VOID Em02_mv03_land(VOID);

/*
    leap handler 3: slow down after landing, then return to chase or strafe
 */
VOID Em02_mv03_rec(VOID);

/*
    leap handler 4: tumble away after a bite, then return to chase
 */
VOID Em02_mv03_tumble(VOID);

/*
    strafe sub-state: turn sideways, run the strafe handler and play footsteps
 */
VOID Em02_mv04(VOID);

/*
    strafe handler 0: hold the sideways run for the timer, then reverse direction or return to chase
 */
VOID Em02_mv04_hold(VOID);

/*
    strafe handler 1: speed up while turning, then back off
 */
VOID Em02_mv04_acc(VOID);

/*
    strafe handler 2: spring back, then return to chase
 */
VOID Em02_mv04_spring(VOID);

/*
    alert sub-state: keep the animation frame fixed; wait for the player, then track the player with the head
 */
VOID Em02_mv05(VOID);

/*
    alert handler 0: wait until the player is near, then start the tracking animation
 */
VOID Em02_mv05_wait(VOID);

/*
    alert handler 1: track the player with the head until the timer ends, then pick the stalk behaviour
 */
VOID Em02_mv05_track(VOID);

/*
    maul sub-state: begin the kill animation, then hold it
 */
VOID Em02_mv06(VOID);

/*
    start the maul: pin the player facing the dog, put the player into its death state and start vibration
 */
VOID Em02_mv06_begin(VOID);

/*
    play the maul animation: hold the player to the dog, with sounds, vibration and blood at set
    frames; the player dies at frame 0x2A
 */
VOID Em02_mv06_hold(VOID);

/*
    bite sub-state: step the animation, then wait or track
 */
VOID Em02_mv07(VOID);

/*
    bite handler 0: wait until the player is near, then start the bite animation
 */
VOID Em02_mv07_wait(VOID);

/*
    bite handler 1: track the player with the head until frame 0x21, then return to chase
 */
VOID Em02_mv07_track(VOID);

/*
    script-controlled sub-state: only advance the animation
 */
VOID Em02_mv08(VOID);

/*
    patrol sub-state: switch to chase when the player is near, otherwise walk, run the patrol handler
    and play footsteps
 */
VOID Em02_mv01(VOID);

/*
    patrol handler 0: reset the counters
 */
VOID Em02_mv01_begin(VOID);

/*
    patrol handler 1: wait out the walk timer, then pick a turn direction (away from walls, else random)
 */
VOID Em02_mv01_pick(VOID);

/*
    patrol handler 2: turn until the turn amount is used up, then walk again
 */
VOID Em02_mv01_turn(VOID);

/*
    stalk sub-state: creep, circle and watch the player; build up awareness from distance and
    player speed, then bark and switch to chase
 */
VOID Em02_mv09(VOID);

/*
    stalk handler 0: pick a circling direction and duration
 */
VOID Em02_mv09_begin(VOID);

/*
    stalk handler 1: circle for the picked duration
 */
VOID Em02_mv09_arc(VOID);

/*
    stalk handler 2: face the player again once the timer ends or an obstacle is hit
 */
VOID Em02_mv09_face(VOID);

/*
    stalk handler 3: follow the wall until the way is clear
 */
VOID Em02_mv09_wall(VOID);

/*
    run one step of the wall-following state machine for at most Max_cnt frames; returns 0, 1 or 2
    for a negative, no or positive turn
 */
INT Em02_wall_step(SHORT Max_cnt, SHORT Turn_rate);

/*
    wall-follow step 0: pick the turn direction toward the waypoint and advance
 */
SHORT Em02_wall00(SHORT Turn_rate);

/*
    wall-follow step 1: wait until the way ahead is blocked, returning the turn once it is
 */
SHORT Em02_wall01(SHORT Turn_rate);

/*
    wall-follow step 2: turn along the obstacle until the way is clear, then pick the side to go round
 */
SHORT Em02_wall02(SHORT Turn_rate);

/*
    wall-follow step 3: start going round the picked side when it is free or the way is blocked
 */
SHORT Em02_wall03(SHORT Turn_rate);

/*
    wall-follow step 4: turn round the obstacle for a distance, then go back to step 3
 */
SHORT Em02_wall04(SHORT Turn_rate);

/*
    state 2: dispatch the Damage reaction by sub-state
 */
VOID Em02_Damage(VOID);

/*
    Damage handler 0: pick the reaction from the behaviour (flinch, knock-back flight or skid) and start it
 */
VOID Em02_dm00(VOID);

/*
    Damage handler 1: flinch, then get up
 */
VOID Em02_dm01(VOID);

/*
    end of the flinch animation: start getting up
 */
VOID Em02_dm01_end(VOID);

/*
    get up after a flinch and return to the behaviour state
 */
VOID Em02_dm01_up(VOID);

/*
    knock-back flight: follow the arc until the floor is reached, then advance to the next sub-state
 */
VOID Em02_dm02(VOID);

/*
    Damage handler 3: slide while braking through the animation steps, then get up
 */
VOID Em02_dm03(VOID);

/*
    Damage handler 4: get up and return to the behaviour state
 */
VOID Em02_dm04(VOID);

/*
    state 3: dispatch the death sequence by sub-state
 */
VOID Em02_Die(VOID);

/*
    death step 0: set the death event flag and start the death animation
 */
VOID Em02_die00(VOID);

/*
    death step 2: slide while the animation plays; at its end recolour and shrink the shadow
 */
VOID Em02_die02(VOID);

/*
    death step 3: grow the shadow each frame until the timer ends
 */
VOID Em02_die03(VOID);

/*
    state 4: empty handler
 */
VOID Em02_Dead(VOID);

/*
    sub-state 0: empty handler
 */
VOID Em02_mv00(VOID);

/*
    player animation handler for slot 40: begin and run the player's recovery animation
 */
VOID Em02_pl_rec(VOID);

/*
    start the player's recovery animation with a sound
 */
VOID Em02_pl_rec_set(VOID);

/*
    run the player's recovery animation and turn the player around when it ends
 */
VOID Em02_pl_rec_run(VOID);

/*
    bite the player: when the player is weak and facing the dog, grab the player for the kill (result 2);
    otherwise bite for 16 Damage with a hit reaction, vibration and blood
 */
UCHAR Em02_pl_bite(VOID);

/*
    when the player is near, not hit and straight ahead, leap (behaviour 5), or with low player health
    and a random chance strafe (behaviour 9); otherwise watch the player's side
 */
VOID Em02_at_ck(VOID);

/*
    spawn one blood effect at a joint while Blood_cnt is positive
 */
VOID Set_blood(ENEMY_WORK* pEm, UCHAR joint_idx, UCHAR fx_type);

/*
    probe the scene collision one body offset ahead along the facing; returns nonzero when blocked
 */
INT Em02_sca_ck(VOID);

/*
    probe ahead after turning by Dir_ofs with the speed scaled by Spd_mul, restoring the position afterwards
 */
SCHAR Em02_turn_ck(SHORT Dir_ofs, USHORT Spd_mul);

/*
    probe both turn directions: 1 when only the positive turn is blocked, -1 when only the negative
    one is, 0 when neither, Both_val when both
 */
SCHAR Em02_turn2_ck(SHORT Spd_mul, SCHAR Both_val);

/*
    raise the movement speed by step up to limit
 */
VOID Rate_speed_up(USHORT step, SHORT limit);

/*
    lower the movement speed by step down to limit
 */
VOID Rate_speed_down(USHORT step, SHORT limit);

/*
    pick the run animation from the sign of the turn
 */
VOID Set_run_motion(SHORT turn);

/*
    add the rotation to the first three spine pSin_parts_ptr, scaled by 1, 1.25 and 1.5, rebuild their
    matrices and return the last joint
 */
VOID* Em02_back_rot(USHORT rx, USHORT ry, USHORT rz);

/*
    turn the head toward the player by swivelling the spine; returns nonzero when the swivel is at its limit
 */
SCHAR Em02_neck_rot(VOID);

/*
    probe a turn toward the player at Spd_mul times the speed
 */
SCHAR Em02_pl_turn_ck(SHORT Spd_mul);

extern AT_TBL Em02_sca;                         // collision info of the dog
extern UCHAR Em02_life[16];                    // base health, indexed by a random value
extern INT Em02_bark_flg;                             // set once a dog has barked
extern VOID (*Em02_routine_0[5])(VOID);                   // state handlers
extern VOID (*Em02_mv[10])(VOID);               // behaviour handlers by sub-state
extern VOID (*Em02_mv_set_tbl[13])(VOID);        // behaviour entry functions by behaviour id
extern EM02_WALK_REC Em02_walk_tbl[2];        // walk animation and speed of the two patrol gaits
extern VOID (*Em02_mv02_tbl[3])(VOID);                   // chase handlers
extern VOID (*Em02_mv03_tbl[5])(VOID);                    // leap handlers
extern VOID (*Em02_mv04_tbl[3])(VOID);                  // strafe handlers
extern VOID (*Em02_mv01_tbl[3])(VOID);                  // patrol handlers
extern VOID (*Em02_mv09_tbl[4])(VOID);                   // stalk handlers
extern SHORT (*Em02_wall_tbl[5])(SHORT);             // wall-following steps
extern VOID (*Em02_dm[5])(VOID);                 // Damage reaction handlers
extern SHORT Em02_dm_dir_tbl[8];                // random knock-back turn offsets
extern VOID (*Em02_die_tbl[4])(VOID);                     // death sequence steps
extern UCHAR Em02_run_mot_tbl[4];                  // run animation by turn direction: negative, none, positive

#endif
