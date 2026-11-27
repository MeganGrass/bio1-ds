/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM07.H
 */

#ifndef EM07_H
#define EM07_H

#include "TYPES.H"

typedef struct EM07_FREE EM07_FREE;

// private work area of the wasp, stored in the Free area of its ENEMY_WORK
struct EM07_FREE
{
    USHORT Chase_cnt;       // frames spent chasing the player in sub-state 0
    USHORT Sikai_flg;       // bit 0: the last path check toward the player succeeded
    SHORT Pl_hit;           // nonzero when the wasp overlaps the player
    UCHAR pad06[4];         // padding: never accessed
    USHORT Bob_spd;         // vertical speed of the hover bobbing
    USHORT player_distance; // distance to the player (sum of the x and z differences)
    USHORT room_hit;        // nonzero when the wasp touches room collision
    USHORT Climb_flg;       // 1 while the hover bobbing rises, 0 while it descends
    SHORT timer;            // general Time_cnt (nest wait, Damage fade, death fade and respawn delay)
    SHORT Seq_end;          // nonzero when the animation finished this frame
    UCHAR Big_flg;          // 1 for the large wasp (Type bit 1 at start-up)
    UCHAR pad17[1];         // padding: never accessed
    UCHAR Respawn_cnt;      // respawns since the last reset; the eleventh one starts with behavior 0x12
};

// the Free[] work area of the current entity (G.pEm) as this unit's EM07_FREE
#define EM07_FREE_P ((EM07_FREE*)ENEMY_FREE(G.pEm))

// access a field of the wasp work area of the current enemy
#define EM07_EF(field) ENEMY_FREE_FIELD(EM07_FREE, field)

/*
    state 0: enter state 1 with sub-state 1, set up status flags, collision info, a random health (higher for
    the large wasp), the shadow and the start height; Type bit 1 makes the wasp large and
    behavior 0x10 sets a random wait before it appears
 */
VOID Em07_Init(VOID);

/*
    per-frame enemy update: when Stop_flg bit 2 is set, run the state handler and, while airborne
    outside the nest and emerge behaviors, test the wasp against the player, other enemies and the room;
    test the camera switch zones and place the shadow
 */
VOID Em07_Main(VOID);

/*
    state 1: switch to the victory behavior when the player is dead, turn toward the player when touching
    room collision, update the player distance, adjust speed and heading in sub-state 0 and run the behavior
    handler; run the range checks by height and size the shadow from the height of the body
 */
VOID Em07_Move(VOID);

/*
    store the path check result and the facing checks of the player in scratchpad, then run the behavior
    handler selected by Routine_2
 */
VOID Em07_mv_call(VOID);

/*
    behavior 0, nest: stay in the nest until the player is near (behavior flags 0) or the timer check passes,
    then take off; flags with a high nibble go to hover (0x20 and up), to emerge (bit 0) or wait out a
    Time_cnt first
 */
VOID Em07_mv00(VOID);

/*
    behavior 1, takeoff: turn and accelerate along the takeoff animation, then switch to hover at its end
 */
VOID Em07_mv01(VOID);

/*
    behavior 2, hover: flap the wings and bob up and down between two Pos_y_tbl; when the player is close,
    in front and reachable, switch to sting (or to grab for the small wasp when it faces the player)
 */
VOID Em07_mv02(VOID);

/*
    behavior 3, sting: turn toward the player and play the sting animation; at its end Damage the player
    when within reach (the large wasp can kill) and return to takeoff
 */
VOID Em07_mv03(VOID);

/*
    behavior 4, victory: bob above the dead player, flapping and turning along the path
 */
VOID Em07_mv04(VOID);

/*
    behavior 5, grab: forwards to Em07_mv05_main
 */
VOID Em07_mv05(VOID);

/*
    behavior 6, emerge: rise out of the nest with status flag 2 set, then switch to hover
 */
VOID Em07_mv06(VOID);

/*
    start and step the current animation by Act_sts, storing in Seq_end whether it ended
 */
VOID Em07_mot_set(VOID);

/*
    grab the player: face the player and play the grab animation, then poison the player on a random roll,
    take 15 health and enter state 3
 */
VOID Em07_mv05_main(VOID);

/*
    state 2: knock the wasp down to the floor, then tint the shadow red and grow it back from nothing; once
    down, wait for the player to come near while moving and then enter state 3
 */
VOID Em07_Damage(VOID);

/*
    state 3: death sequence with effects on the pSin_parts_ptr and the death event flag; a lightly hit small wasp
    instead respawns at a fixed position after a delay
 */
VOID Em07_Die(VOID);

/*
    player animation handler for slot 7: play the grabbed animation of the player, then return the
    player to the idle state and clear its hit state
 */
VOID Pl_mv_crawl(VOID);

extern AT_TBL Em07_sca0;         // collision info of the wasp, an AT_TBL record
extern AT_TBL Em07_sca1;         // alternate collision info, an AT_TBL record; unused
extern AT_TBL* pEm07_sca;       // collision info installed on every wasp
extern AT_TBL* Em07_sca1_adr;         // address of Em07_sca1; unused
extern VOID (*Em07_routine_0[4])(VOID);      // state handlers, by entity state
extern VOID (*Em07_mv_tbl[7])(VOID);   // behavior handlers, by Routine_2

#endif
