/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ATTACK.H
 */

#ifndef ATTACK_H
#define ATTACK_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include "SCRATCHPAD.H"

struct AT_ESP_REC
{
    SHORT Eff_x;        // hit effect position x
    SHORT Eff_y;        // hit effect position y
    SHORT Eff_z;        // hit effect position z
    SHORT Damage;       // Damage dealt
    UCHAR fx_type;      // hit effect type
    UCHAR Esp_dt;       // hit effect variant
    UCHAR hit_state;    // hit reaction state set on the target
    UCHAR pad;          // padding
};

/*
    test the effect point (x, z) of the given radius against the player and, in stage 2 room 12,
    against the enemy in slot 1; a hit takes health (more after a strike by enemy types 3 and 4,
    which can also poison), sets the hit and state fields and vibrates the pad. returns 1 on a hit
 */
INT Pos_pl_at_ck(INT radius, SHORT x, SHORT z);

/*
    test whether entity pEm lies closer to the point (x, z) than the radius held in scratchpad word 0
 */
INT Pl_dist_ck(SHORT x, SHORT z, ENEMY_WORK* pEm);

/*
    test whether entity pEm lies closer to the point (x, z) than the radius held in scratchpad word 0
    plus 500
 */
INT Em_dist_ck(SHORT x, SHORT z, ENEMY_WORK* pEm);

/*
    measure the distance from the current entity to the player and set status bit 0x20 of the
    entity when it is under range; returns the distance
 */
UINT Em_ck_hani(UINT range);

/*
    measure the distance from the current entity to the player and set status bit 0x80 of the
    entity when it is under range; returns the distance
 */
UINT Em_ck_alert(UINT range);

/*
    resolve a weapon attack of the given Type (1 to 10, one more than the table index): find the
    nearest active enemy in reach that passes the hit test of the type, apply the Damage and hit
    effect from Wep_at_tbl, run the post-hit function of the type and put the enemy in its
    damaged or dead state. returns 1 on a hit
 */
INT Weapon_at_ck(INT Type);

/*
    play the knife hit sound, run the hit enemy's reaction function and, unless the effect type is
    1, spawn the hit effect at the player's joint 14 with a per-character offset
 */
VOID Wep_set_knife(ENEMY_WORK* pEm);

/*
    run the hit enemy's reaction function
 */
VOID Wep_set_rtn(ENEMY_WORK* pEm);

/*
    weaken a long-range hit of hit type 2 (distance of 9001 or more) by stepping the hit state back
    and returning 10 health, then run the hit enemy's reaction function
 */
VOID Wep_set_shotgun(ENEMY_WORK* pEm);

/*
    spawn blood effects at the hit enemy unless the effect type is 1; a killing hit of effect type
    14 adds splatter to both sides and brightens the colour of every enemy joint
 */
VOID Wep_set_blood(ENEMY_WORK* pEm);

/*
    spawn blood effects at the hit enemy unless the effect type is 1; a killing hit of effect type
    0 adds attack effects to the last five pSin_parts_ptr or to the pSin_parts_ptr listed in Em_kan_tbl
 */
VOID Wep_set_blood2(ENEMY_WORK* pEm);

/*
    spawn spark effects at the hit enemy unless the effect type is 1; a killing hit of effect type
    9 adds sparks to both sides and brightens the colour of every enemy joint
 */
VOID Wep_set_sparks(ENEMY_WORK* pEm);

/*
    spawn blood effects at the hit enemy like Wep_set_blood (killing splatter unless the
    effect type is 2), then run Wep_set_blood2
 */
VOID Wep_set_blood3(ENEMY_WORK* pEm);

/*
    reaction of enemy ids 0 to 2 and 17: a strong hit of some types kills the enemy outright (health
    -300, death event flag, joint effect and sound), adjusts health for some modes and the second
    character, then spawns the hit effect
 */
VOID Em_dm_zombie(ENEMY_WORK* pEm);

/*
    spawn the hit effect recorded in the scratchpad at the hit enemy
 */
VOID Em_dm_basic(ENEMY_WORK* pEm);

/*
    reaction of enemy id 6: raise the effect height when the player flag 0x20 is set, spawn an extra
    effect of type 3, then spawn the hit effect
 */
VOID Em_dm_head(ENEMY_WORK* pEm);

/*
    reaction of enemy id 8: for every hit type except 0 spawn effect 0x18 at the enemy; a hit from 7001
    or more away has an even chance of restoring the health it took
 */
VOID Em_dm_blood(ENEMY_WORK* pEm);

/*
    empty reaction
 */
VOID Em_dm_none(ENEMY_WORK* pEm);

/*
    hit test for the gun hit types: rotate a near and a far pair of side points by the player's angle
    (the far pair reaches out to range) and test enemy pEm against both cones in turn. returns 1 on a hit
 */
INT Wep_hit_ck_gun(INT range, ENEMY_WORK* pEm);

/*
    test enemy pEm against the cone bounded by the lines pSide_a and pSide_b; a hit nearer than the best
    so far is kept in scratchpad word 0. returns 1 on such a hit
 */
INT Wep_hit_ck_cone(SVECTOR* pSide_a, SVECTOR* pSide_b, ENEMY_WORK* pEm);

/*
    hit test for the knife: compare the distance from the player's joint 14 (offset per character) to enemy
    pEm with its radius plus range, adjusted per enemy id; a hit nearer than the best so far is kept in
    scratchpad word 0. returns 1 on such a hit
 */
INT Wep_hit_ck_knife(INT range, ENEMY_WORK* pEm);

/*
    hit test for the projectile hit types: compare the distance from the scratchpad point to enemy pEm
    with its radius plus r (smaller for enemy id 4); a hit nearer than the best so far is kept in
    scratchpad word 0. returns 1 on such a hit
 */
INT Wep_hit_ck_tama(INT r, ENEMY_WORK* pEm);

/*
    debug display: test the player's facing line against each boundary flagged 0x100 in collision
    list 3 of the room data and print left or right, the intercept and "Atari!" on a hit
 */
VOID Sca_hit_print(VOID);

/*
    return the x offset on the line held in the scratchpad vector at z offset dz; a zero z component
    counts as 1
 */
INT Line_get_x(SHORT dz);

/*
    return the z offset on the line held in the scratchpad vector at x offset dx; a zero x component
    counts as 1
 */
INT Line_get_z(SHORT dx);

/*
    return a * d - b * c, the cross product of the 2D vectors (a, b) and (c, d)
 */
INT Cross2d(INT a, INT b, INT c, INT d);

/*
    return a * d + b * c, the dot product of the 2D vectors (a, b) and (d, c)
 */
INT Dot2d_swap(INT a, INT b, INT c, INT d);

extern INT (*Wep_hit_ck_tbl[10])(INT, ENEMY_WORK*);     // hit test per weapon hit type
extern VOID (*Wep_set_tbl[10])(ENEMY_WORK*);           // post-hit effect function per weapon hit type
extern VOID (*Em_dm_rtn_tbl[20])(ENEMY_WORK*);                    // hit reaction function per enemy id
extern UCHAR Em_kan_tbl[20][6];                       // joint indices per enemy id that get attack effects on a killing hit
extern INT Wep_range_tbl[2][10];                             // hit reach per character and weapon hit type
extern AT_ESP_REC Wep_at_tbl[3][20][10];                    // hit record per Damage table, enemy id and weapon hit type
extern UCHAR Wep_crit_tbl[0x10];                  // handgun critical hit flags
extern UCHAR Build_stamp[0x24];                    // build stamp text stored behind the table

#endif
