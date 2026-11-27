/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM0F.H
 */

#ifndef EM0F_H
#define EM0F_H

#include "TYPES.H"

typedef struct EM0F_FREE EM0F_FREE;

// private work area of the plant monster, stored in the Free area of its ENEMY_WORK
struct EM0F_FREE
{
    INT player_dist;        // distance to the player (sum of the x and z differences)
    SHORT attack_cooldown;  // frames before the next attack may start
    SHORT Mot_end;          // nonzero when the animation finished this frame
    SHORT vine_segment;     // vine joint being shown or hidden
    SHORT timer;            // general Time_cnt (squeeze Damage period, fade steps)
    SHORT Fade_timer;       // Time_cnt of the palette fade step
    SHORT Idle_cnt;         // frames until the next idle animation
    USHORT First_flg;       // 1 until the plant has attacked, which widens its strike range
    USHORT Dir_bak;         // facing saved while turning toward the player; reused as a fade counter
    USHORT Side_no;         // side the player stands on for the ambush; reused as a fade counter
    SHORT Kage_flg;         // bit 0: the shadow is drawn
    INT saved_state;        // state bytes of the last frame, restored after a hit
    USHORT hit_count;       // hits taken; the fourth raises an event flag
};

// the Free[] work area of the current entity (G.pEm) as this unit's EM0F_FREE
#define EM0F_FREE_P ((EM0F_FREE*)ENEMY_FREE(G.pEm))

// access a field of the plant work area of the current enemy
#define EM0F_EF(field) ENEMY_FREE_FIELD(EM0F_FREE, field)

/*
    update the plant for one frame: run the state handler, copy the position of joint 11 into the
    collision data, test the camera switch zones and place the shadow
 */
VOID Em0f_Main(VOID);

/*
    state 0: set up a new plant with health, collision info, parts and shadow; plants flagged 2 start
    with every vine segment hidden
 */
VOID Em0f_Init(VOID);

/*
    state 1: update the visibility flags and player distance, select a variant when the sub-state is 0,
    then run the sub-state handler; behavior bit 0x40 switches to the death state
 */
VOID Em0f_Move(VOID);

/*
    pick the sub-state from the behavior through the start table (0x80 enters watch)
 */
VOID Em0f_Type_sel(VOID);

/*
    enter watch: set the watch sub-state, clear status flag 8 and mark the plant First_flg
 */
VOID Em0f_watch_set(VOID);

/*
    enter hidden: hide every vine segment and turn the shadow off
 */
VOID Em0f_hidden_set(VOID);

/*
    enter grab: start the grab animation
 */
VOID Em0f_catch_set(VOID);

/*
    enter strike: start the swing animation chosen by the facing and set status flag 2
 */
VOID Em0f_strike_set(VOID);

/*
    enter ambush: hide the vine segments, turn toward the player and seize the player at once
 */
VOID Em0f_ambush_set(VOID);

/*
    enter dead: random resting pose with health -1; behavior 7 also starts a palette fade
 */
VOID Em0f_dead_set(VOID);

/*
    enter sway: pick a sway animation, avoiding the same side three times in a row
 */
VOID Em0f_sway_set(VOID);

/*
    watch sub-state: fidget, run the range check of the behavior and step the animation
 */
VOID Em0f_Move_watch(VOID);

/*
    watch check 0: switch to the grab behavior when the player is within reach
 */
VOID Em0f_catch_ck(VOID);

/*
    watch check 1: switch to the strike behavior when the player is in range and ahead, after the cooldown
 */
VOID Em0f_strike_ck(VOID);

/*
    watch check 2: track the side the player is on and switch to the ambush behavior when in range
 */
VOID Em0f_ambush_ck(VOID);

/*
    grab sub-state: step the animation and run the grab step handler
 */
VOID Em0f_Move_catch(VOID);

/*
    grab step 0: idle until the player is in reach, then start the grab animation
 */
VOID Em0f_Catch_wait(VOID);

/*
    seize the player: put the player into the held state facing the plant and start vibration
 */
VOID Em0f_Catch_seize(VOID);

/*
    grab step 2: hold the player until the animation ends, then start the squeeze with its sound
 */
VOID Em0f_Catch_hold(VOID);

/*
    grab step 3, also ambush step 1: squeeze the player, damaging every 12 frames; struggling shortens the hold
 */
VOID Em0f_Catch_squeeze(VOID);

/*
    grab step 4: return to watch with a cooldown and restore the facing once the animation ends
 */
VOID Em0f_Catch_end(VOID);

/*
    grab step 5: repeat the lash animation; when the repeats run out, restart the grab wait if the player is close, else reselect the grab behavior
 */
VOID Em0f_Catch_lash(VOID);

/*
    strike sub-state: run the strike step handler
 */
VOID Em0f_Move_strike(VOID);

/*
    strike step 0: play the swing; at frame 6 hit the player when in front
 */
VOID Em0f_Strike_swing(VOID);

/*
    strike step 1: recover, then strike again when in range or go back to watch when far
 */
VOID Em0f_Strike_recover(VOID);

/*
    ambush sub-state: step the animation and run the ambush step handler
 */
VOID Em0f_Move_ambush(VOID);

/*
    ambush step 0: extend the vine segments in stages, then hold the player
 */
VOID Em0f_Ambush_extend(VOID);

/*
    ambush step 2: start the retract animation
 */
VOID Em0f_Ambush_retract_set(VOID);

/*
    ambush step 3: hide the vine segments in stages, then return to watch
 */
VOID Em0f_Ambush_retract(VOID);

/*
    dead sub-state: keep health at -1 and play the resting animation for the remaining ticks
 */
VOID Em0f_Move_dead(VOID);

/*
    sway sub-state: loop the sway animation; behavior 0x29 also steps its palette fades
 */
VOID Em0f_Move_sway(VOID);

/*
    state 2: count the hit (the fourth raises an event flag), restore the saved state and re-run the state check
 */
VOID Em0f_Damage(VOID);

/*
    state 3: run the death step by sub-state
 */
VOID Em0f_Die(VOID);

/*
    death step 0: start writhing for a random time
 */
VOID Em0f_Die_begin(VOID);

/*
    death step 1: sway for the random time, then play the fall animation and start the fade
 */
VOID Em0f_Die_writhe(VOID);

/*
    death step 2: fade out with random animation skips, raise the death event flag at the end
 */
VOID Em0f_Die_fade(VOID);

/*
    state 4: empty handler
 */
VOID Em0f_Dead(VOID);

/*
    sub-state 0: empty handler
 */
VOID Em0f_Move_nop(VOID);

/*
    player animation handler for slot 34: run the hold step by action state
 */
VOID Pl_Em0f_hold(VOID);

/*
    player hold step 0: start the held animation
 */
VOID Pl_Em0f_hold_start(VOID);

/*
    player hold step 1: play the squeezed animation, looping its tail, with vibration
 */
VOID Pl_Em0f_hold_squeezed(VOID);

/*
    player hold step 2: play the release animation, then return the player to the idle state
 */
VOID Pl_Em0f_hold_release(VOID);

/*
    pick a random idle animation after the fidget timer runs out
 */
VOID Em0f_Idle_set(VOID);

/*
    empty function
 */
VOID Em0f_Nop5(VOID);

extern AT_TBL Em0f_sca;                   // collision info of the plant
extern INT Em0f_side_old;                     // sway sides chosen so far, newest in bit 0
extern VOID (*Em0f_routine_0[5])(VOID);             // state handlers
extern VOID (*Em0f_Move_type[7])(VOID);              // sub-state handlers
extern VOID (*Em0f_set_tbl[10])(VOID);            // sub-state entry functions, indexed by behavior
extern VOID (*Em0f_watch_tbl[3])(VOID);             // watch range checks
extern VOID (*Em0f_catch_tbl[6])(VOID);         // grab steps
extern VOID (*Em0f_strike_tbl[2])(VOID);       // strike steps
extern VOID (*Em0f_ambush_tbl[4])(VOID);       // ambush steps
extern SHORT Em0f_extend_tbl[4];             // vine joint index each extend stage runs down to
extern SHORT Em0f_retract_tbl[12];           // vine joint index each retract stage runs up to
extern VOID (*Em0f_die_tbl[3])(VOID);               // death steps
extern VOID (*Pl_Em0f_hold_tbl[3])(VOID);               // player hold steps

#endif
