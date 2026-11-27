/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM0E.H
 */

#ifndef EM0E_H
#define EM0E_H

#include "TYPES.H"

typedef struct EM0E_FREE EM0E_FREE;

// work data of the roots entity, overlaid on ENEMY_WORK.Free
struct EM0E_FREE
{
    SHORT Shake_spd;    // vertical shake speed; flips between +Shake_pow and -Shake_pow every frame
    USHORT Shake_pow;   // magnitude of the vertical shake; grows while rising, shrinks while sinking
    SHORT Fade_cnt;     // palette fade steps left while shrinking
    SHORT Dummy172;     // cleared at start-up; never read
    INT Base_pos_x;     // spawn x position, restored every frame for the non-zero behaviors
    INT Base_pos_z;     // spawn z position, restored every frame for the non-zero behaviors
    SHORT Snd_timer;    // frames until the next groan sound
    USHORT Sink_cnt;    // counter raised every fourth frame while sinking
};

// access a field of the roots work area of the current enemy
#define EM0E_EF(field) ENEMY_FREE_FIELD(EM0E_FREE, field)

/*
    per-frame enemy update: when Stop_flg bit 2 is set, run the state handler and the three collision
    calls against the player; restore the spawn position for the non-zero behaviors, test the camera
    switch zones and place the shadow
 */
VOID Em0e_Main(VOID);

/*
    state 0: set state 1, health 1, collision info, status flags and animation, scale and hit state, store the
    spawn position and groan timer in the work area and set up the shadow
 */
VOID Em0e_Init(VOID);

/*
    state 1: set the zone bits (0xE0) of the status flags, run the movement of its behavior (0 is the
    rise-and-sink sequence, 0x80 does nothing, any other starts a one-shot shrink) and step the animation
 */
VOID Em0e_Move(VOID);

/*
    behavior 0 movement: run the sub-state phase, shake the roots vertically and play the groan sound at
    random intervals
 */
VOID Em0e_mv00(VOID);

/*
    phase 0: start the sequence with a sound, a short step timer and six palette fade steps, set status flag 2
    and set health to -1
 */
VOID Em0e_phase_start(VOID);

/*
    phase 1: raise the shake amplitude until it reaches its maximum
 */
VOID Em0e_phase_rise(VOID);

/*
    phase 2: shrink the roots and the shadow while stepping the palette fade, until the scale drops to 0x9C4
 */
VOID Em0e_phase_shrink(VOID);

/*
    phase 3: lower the shake amplitude to zero, then raise the death event flag
 */
VOID Em0e_phase_sink(VOID);

/*
    other behaviors movement: on the first call set the shrunken scale and shadow size, set health to -1 and
    apply a one-shot palette fade
 */
VOID Em0e_mv01(VOID);

/*
    state 2: return to state 1 with sub-state 0 and clear the hit state
 */
VOID Em0e_Damage(VOID);

/*
    state 3: empty
 */
VOID Em0e_Nop3(VOID);

/*
    state 4: empty
 */
VOID Em0e_Dead(VOID);

/*
    state 5: empty
 */
VOID Em0e_Nop5(VOID);

extern AT_TBL Em0e_sca_info;                 // collision info of the roots, an AT_TBL record
extern VOID (*Em0e_routine_0[6])(VOID);           // state handlers, by entity state
extern VOID (*Em0e_phase_tbl[4])(VOID);           // sub-state handlers of the behavior 0 movement

#endif
