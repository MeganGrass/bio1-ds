/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM0B.H
 */

#ifndef EM0B_H
#define EM0B_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include "SCRATCHPAD.H"

typedef struct EM0B_FREE EM0B_FREE;

// private work area of the shark, stored in the Free area of its ENEMY_WORK
struct EM0B_FREE
{
    SHORT Catch_timer;  // Time_cnt of the bite hold; button mashing drains it faster, below zero the shark lets go
    USHORT Wander_dir;  // angle result of the room collision probe ahead of the shark, fed to the wander turn
    SCHAR Wander_rno;   // state of the wander turn that steers around obstacles
    UCHAR effect_timer; // frames until the next splash or blood effect
    UCHAR Swim_ctr;     // swim animation loops finished; the third plays the swim sound
    SCHAR Unk_173;      // set to 5 at start-up; never read
    UCHAR Dir_ctrl_flg; // direction flags set by the room collision probe, used by the wander turn
    SCHAR Unk_175;      // set to 0x20 when circling starts; never read
};

// the Free[] work area of the current entity (G.pEm) as this unit's EM0B_FREE
#define EM0B_FREE_P ((EM0B_FREE*)ENEMY_FREE(G.pEm))

/*
    per-frame enemy update: a dormant shark (Type bit 7 set after state 0) does nothing; otherwise
    when Stop_flg bit 2 is set, run the state handler and the three collision calls against the player,
    probe room collision 0x320 units ahead for the wander turn, then test the camera switch zones and place
    the shadow
 */
VOID Em0b_Main(VOID);

/*
    state 0: enter state 1, reset counters and set up the shadow, collision info and random health of the
    large or the small shark (Type bit 1); start swimming, advance the saved position ahead of
    the shark; Type bit 0 starts directly in sub-state 1 with behavior 1
 */
VOID Em0b_Init(VOID);

/*
    state 1: find the next waypoint toward the player, measure the distance to the player, update the
    status and visibility flags, choose an attack while Key_rno is 0 and run the behavior
 */
VOID Em0b_Move(VOID);

/*
    steer along the wander turn; when the player is not hit and the large shark faces it within 0x300, start
    the devour behavior if the player health is low and it is near, else the bite behavior if it is near
 */
VOID Em0b_Atck_set(VOID);

/*
    run the behavior handler selected by Routine_2
 */
VOID Em0b_Move_call(VOID);

/*
    behavior 0, swim: run the swim step, step the swim animation (with its sound), move ahead and, for the
    large shark, spawn splash effects around the body every few frames
 */
VOID Em0b_Move_type00(VOID);

/*
    swim step 0: start the swim animation and set the swim speed
 */
VOID Em0b_Swim_begin(VOID);

/*
    swim step 1: speed up when the player is far away and in action behavior 0xD
 */
VOID Em0b_Swim_far(VOID);

/*
    swim step 2: return to the normal swim speed once the player is near or leaves action behavior 0xD
 */
VOID Em0b_Swim_near(VOID);

/*
    behavior 1, lunge: start one of two lunge animations with sounds, step it while moving ahead with random
    speed and turn toward the player; back to the start step when the animation ends
 */
VOID Em0b_Move_type01(VOID);

/*
    behavior 2, retreat: run the retreat step
 */
VOID Em0b_Move_type02(VOID);

/*
    retreat step 0: start the retreat animation at a high speed
 */
VOID Em0b_Retreat_begin(VOID);

/*
    retreat step 1: play the animation moving ahead; at frame 0x17 return to state 1 with behavior 0
 */
VOID Em0b_Retreat_run(VOID);

/*
    behavior 3, circle: run the circle step, step the animation and move ahead
 */
VOID Em0b_Move_type03(VOID);

/*
    circle step 0: start the swim animation and a random Time_cnt
 */
VOID Em0b_Circle_begin(VOID);

/*
    circle step 1: wander-turn one way until the Time_cnt ends, then switch to the other way
 */
VOID Em0b_Circle_left(VOID);

/*
    circle step 2: wander-turn the other way until the Time_cnt ends, then switch back
 */
VOID Em0b_Circle_right(VOID);

/*
    behavior 4, devour: run the devour step, then keep the shark and the player out of room collision by
    moving both saved positions and base positions by the correction
 */
VOID Em0b_Move_type04(VOID);

/*
    devour step 0: turn toward the player, start the seize animation, hide the player and put the player
    into the held state facing the shark
 */
VOID Em0b_Devour_seize(VOID);

/*
    devour step 1: play the animation with blood effects; at frame 0xF advance with the player carried, or,
    when the position check at the mouth returns 0, release the player and go to the spit step
 */
VOID Em0b_Devour_carry(VOID);

/*
    devour step 2: play the animation; at frame 0x13 capture the player into Em0b_catch_m and hide
    its body parts
 */
VOID Em0b_Devour_bite(VOID);

/*
    devour step 3: play the animation with effects, set player health to -1, move the captured player into
    the mouth and hide the remaining body parts at set frames
 */
VOID Em0b_Devour_swallow(VOID);

/*
    devour step 4: clear the status flag 2 and return to state 1 with behavior 0
 */
VOID Em0b_Devour_finish(VOID);

/*
    devour step 5: play the animation with blood effects until frame 0x17, then start a Time_cnt and the
    swim animation
 */
VOID Em0b_Devour_spit(VOID);

/*
    devour step 6: play the animation during the Time_cnt with periodic effects, then return to state 1
    with behavior 0
 */
VOID Em0b_Devour_recover(VOID);

/*
    behavior 5, bite: run the bite step
 */
VOID Em0b_Move_type05(VOID);

/*
    bite step 0: start the bite animation and set status flag 2
 */
VOID Em0b_Bite_begin(VOID);

/*
    bite step 1: play the bite animation moving toward the player; when the player is within the mouth reach
    start the hold on the player; when the animation ends first go to the recover step
 */
VOID Em0b_Bite_close(VOID);

/*
    bite step 2: play the animation until it ends with effects on the player, then start the shake with a
    90 frame struggle timer
 */
VOID Em0b_Bite_seize(VOID);

/*
    bite step 3: shake the player, damaging it each time the animation ends and draining the struggle timer
    faster on button mashing; when it runs out let go
 */
VOID Em0b_Bite_shake(VOID);

/*
    bite step 4: push the player away until the animation ends, then return the player to the idle state
 */
VOID Em0b_Bite_release(VOID);

/*
    bite step 5: back off from the player during the Time_cnt with periodic effects, then return to state 1
    with behavior 0
 */
VOID Em0b_Bite_recover(VOID);

/*
    state 2: start the death sub-state on the first call and run the death handler for the kind of shark
    (Type bit 0)
 */
VOID Em0b_Die(VOID);

/*
    death of a normal shark: run the death step, with periodic splash effects for the large shark
 */
VOID Em0b_Die_normal(VOID);

/*
    death step 0: advance the step; when the facing check toward the player fails skip to the sink step,
    else start the death animation with its sound and effects and run the thrash step
 */
VOID Em0b_Die_turn(VOID);

/*
    death step 1: play the death animation while drifting; when it ends turn the shark around and advance
    to the sink step
 */
VOID Em0b_Die_thrash(VOID);

/*
    death step 2: swim away from the player during the Time_cnt, then return to state 1
 */
VOID Em0b_Die_sink(VOID);

/*
    death of the boss shark: run the boss death step
 */
VOID Em0b_Die_boss(VOID);

/*
    boss death step 0: raise the death event flag, start the charge animation and run the charge step
 */
VOID Em0b_Die_boss_begin(VOID);

/*
    boss death step 1: turn away from the player and play the animation, adding forward speed during frames
    0xD to 0x18; when it ends start the next animation
 */
VOID Em0b_Die_boss_charge(VOID);

/*
    boss death step 2: turn away from the player and charge forward while playing the animation; when it
    ends start the Time_cnt and the next animation
 */
VOID Em0b_Die_boss_charge2(VOID);

/*
    boss death step 3: play the animation; once the Time_cnt has run out at frame 8 start animation 5, then
    recolour and shrink the shadow, set status flag 2, start a 90 frame Time_cnt and advance
 */
VOID Em0b_Die_boss_beach(VOID);

/*
    boss death step 4: keep health at -1 and grow the shadow until the Time_cnt ends
 */
VOID Em0b_Die_boss_fade(VOID);

/*
    state 3: empty
 */
VOID Em0b_Nop3(VOID);

/*
    state 4: empty
 */
VOID Em0b_Dead(VOID);

/*
    state 5: empty
 */
VOID Em0b_Nop5(VOID);

/*
    player animation handler for slot 30: play the poison death animation with a sound, setting the hit state
 */
VOID Pl_poison_die(VOID);

/*
    player animation handler for slot 49: play the death animation, holding frame 0xD; in action state 2
    place the player from the capture matrix relative to the first enemy
 */
VOID Pl_devour_die(VOID);

extern AT_TBL Em0b_sca_big[2];                  // collision table of the large shark, two AT_TBL records
extern AT_TBL Em0b_sca_small[2];                // collision table of the small shark, two AT_TBL records
extern MATRIX Em0b_catch_m;                // transform of the held player relative to the mouth joint
extern VOID (*Em0b_routine_0[6])(VOID);                // state handlers, by entity state
extern VOID (*Em0b_Move_type[6])(VOID);             // behavior handlers, by Routine_2
extern VOID (*Em0b_swim_tbl[3])(VOID);             // swim steps, by Act_sts
extern VOID (*Em0b_retreat_tbl[2])(VOID);          // retreat steps, by Act_sts
extern VOID (*Em0b_circle_tbl[3])(VOID);           // circle steps, by Act_sts
extern VOID (*Em0b_devour_tbl[7])(VOID);           // devour steps, by Act_sts
extern VOID (*Em0b_bite_tbl[6])(VOID);             // bite steps, by Act_sts
extern VOID (*Em0b_die_kind_tbl[2])(VOID);           // death handlers, by Type bit 0
extern VOID (*Em0b_die_tbl[3])(VOID);            // death steps of the normal shark, by Routine_2
extern VOID (*Em0b_die_boss_tbl[5])(VOID);       // death steps of the boss shark, by Routine_2

#endif
