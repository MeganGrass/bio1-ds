/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM0A.H
 */

#ifndef EM0A_H
#define EM0A_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include "SCRATCHPAD.H"

typedef struct EM0A_FREE EM0A_FREE;

struct EM0A_FREE
{
    SHORT Sikai_flg;           // bit 0: the player is in sight (result of the sight check)
    SHORT Pl_hit;               // nonzero while the entity touches the player (collision test result)
    USHORT player_distance;     // x-z distance to the player (0x1387 while the player is off the ground)
    USHORT room_hit;             // nonzero while the entity is blocked by room collision (unsigned: read with an unsigned halfword load)
    USHORT Hit_en;               // 1: the entity collides with the player and the room, 0: hidden entity without collision (unsigned: read with an unsigned halfword load)
    SHORT wall_stuck_frames;    // update ticks spent blocked by room collision
};

// the Free[] work area of the current entity (G.pEm) as this unit's EM0A_FREE
#define EM0A_FREE_P ((EM0A_FREE*)ENEMY_FREE(G.pEm))

/*
    initialise the entity: set state 1 and clear its action and hit fields, set up the shadow, set a random health
    (2 * (5 + three rolls of 0 to 7)) and scale 0x3000; with a nonzero behavior flag the entity starts on the ceiling
    (upside down, 0xF0 minus a random wait), and flag 3 starts hidden with collision off
 */
VOID Em0a_Init(VOID);

/*
    per-frame entry point: on update ticks run the state handler and, while collision is enabled, test the entity against
    the player, other enemies and the room; then test the camera switch zones (never for a ceiling variant) and update the shadow
 */
VOID Em0a_Main(VOID);

/*
    state 1 handler: sample the facing check toward the player (0x100 and 0x200 tolerances), run the decision step (sub-state 0,
    falling through to the behavior) or the behavior alone, test the visual range, size the shadow from the body height
    and count the ticks spent blocked by room collision
 */
VOID Em0a_Move(VOID);

/*
    state 2 handler: run the flinch reaction and size the shadow from the body height
 */
VOID Em0a_Damage(VOID);

/*
    state 3 handler: run the death behavior selected by Routine_2 (0 writhe, 3 retreat)
 */
VOID Em0a_Die(VOID);

/*
    refresh the sight check (bit 0 of Sikai_flg tracks whether the player is seen) and run the behavior selected by Routine_2
 */
VOID Em0a_mv_call(VOID);

/*
    measure the x-z distance to the player (0x1387 when the player is off the ground), then run the decision function
    selected by Type
 */
VOID Em0a_brain(VOID);

/*
    decision for the ground adder (behavior flag 0): turn away when the player is near but unseen, bite when the player is close
    and ahead or touching, turn away after 0x3D blocked ticks, and start the victory behavior once the player is dead
 */
VOID Em0a_brain00(VOID);

/*
    decision for the ceiling adder (behavior flag 1): hang idle, and drop when the player is within 0xBB8
 */
VOID Em0a_brain01(VOID);

/*
    decision for the ambush ceiling adder (behavior flag 2): hang idle, and drop next to the player when the player is in action
    behavior 0x14 within 0x1388 or the wait runs out
 */
VOID Em0a_brain02(VOID);

/*
    decision for the hidden adder (behavior flag 3): stay idle, and emerge when the player is within 0x1194
 */
VOID Em0a_brain03(VOID);

/*
    behavior 0: crawl towards the player with random speed and turn jitter, steering by the facing check when within 0x1F40
 */
VOID Em0a_mv00(VOID);

/*
    behavior 1: fall from the ceiling with growing speed while the roll angle unwinds, land on the floor and return to the decision step
 */
VOID Em0a_mv01(VOID);

/*
    behavior 2: play a random turn animation, then turn the entity around by 0x800 and return to the decision step
 */
VOID Em0a_mv02(VOID);

/*
    behavior 3: bite (animation 3); on frame 0xB, when the player is in sight, ahead and unhurt and within reach of the head,
    deal 6 Damage, poison the player with a 1 in 4 chance and start the player's hit reaction
 */
VOID Em0a_mv03(VOID);

/*
    behavior 4: play the bite animation in a loop after the player has died
 */
VOID Em0a_mv04(VOID);

/*
    behavior 5: empty
 */
VOID Em0a_mv05(VOID);

/*
    behavior 6: clear the behavior flag and move out of hiding for 0x3C ticks (animation 0, speed 0x1E), then enable collision
    and return to the decision step
 */
VOID Em0a_mv06(VOID);

/*
    flinch after a hit (animation 3): clear the flags of four pSin_parts_ptr, wait a random time and return to state 1
 */
VOID Em0a_dm00(VOID);

/*
    death behavior 0: writhe (animation 4) from frame 0x1E; hit class 7 (Damage_no >> 3) bursts every joint and ends at once,
    class 2 or more switches to the retreat, otherwise play the animation, shrink the shadow, raise the death event flag and end
 */
VOID Em0a_die00(VOID);

/*
    death behavior 3: shrink while the animation plays, hide the pSin_parts_ptr and move the entity far above the floor, then after a random
    wait restore the pSin_parts_ptr, set behavior flag 2 (1 in room 1) and restart the initialisation
 */
VOID Em0a_die01(VOID);

extern AT_TBL* pEm0a_sca;             // collision volume of the adder (see Em0a_sca0)
extern VOID (*Em0a_routine_0[4])(VOID);       // state handlers by entity state (0=init, 1=run, 2=damaged, 3=death)
extern VOID (*Em0a_mv_tbl[7])(VOID);    // behaviors by Routine_2
extern VOID (*Em0a_brain_tbl[7])(VOID);      // decision functions by Type (0=ground, 1=ceiling, 2=ambush ceiling, 3=hidden)

#endif
