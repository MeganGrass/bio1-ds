/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\PL_WEP.H
 */

#ifndef PL_WEP_H
#define PL_WEP_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

struct PL_DIE_REC
{
    USHORT speed; // initial move speed of the death fall
    USHORT push;  // forward push applied every frame of the fall
};

typedef struct PL_VIB_REC PL_VIB_REC;
typedef struct PL_HIT_REC PL_HIT_REC;
typedef VOID (*PL_WEP_FN)(VOID);

struct PL_VIB_REC
{
    SHORT duration;     // pad vibration time of a shot
    USHORT pad;         // not read by any code, kept as data
    SHORT ramp_from;    // fade-out start strength
    SHORT ramp_to;      // fade-out end strength
    SHORT Fade_len;     // fade-out length in frames
};

struct PL_HIT_REC
{
    INT hit_type;   // hit kind passed to Weapon_at_ck
    UCHAR frame;    // animation frame of the hit
    UCHAR Se_no1;   // first sound effect number
    UCHAR Se_no2;   // second sound effect number
    UCHAR flags;    // padding
};

/*
    gun aim entry state: play the weapon raise animation while turning with left/right, lock the
    aim onto the auto-aim target when enabled, switch to the aim-change behavior when an aim
    button is pressed and continue with behavior 0x13 when the animation ends
 */
VOID Pl_gun_aim(VOID);

/*
    gun raise state: blend into the aimed pose and play the raise animation for the aim level
    (level, up, down) selected by Aim_flg
 */
VOID Pl_gun_raise(VOID);

/*
    gun hold state: read the aim up/down buttons and switch the aim animation, start a shot when
    ammo is loaded (vibration included), play the empty click, start a reload or lock-on turn,
    turn with left/right and holster when the hold button is released
 */
VOID Pl_gun_hold(VOID);

/*
    gun fire state: run the fire sub-state handler Pl_fire_tbl[Act_sts]
 */
VOID Pl_gun_fire(VOID);

/*
    begin a shot: select the fire animation for the aim level, spawn the camera flare effect,
    play the foot sound and run the first shot frame
 */
VOID Pl_fire_start(VOID);

/*
    per-frame shot handling: advance the fire animation and, at the table frames, spend one
    round and spawn the muzzle flash, hit, shell and second muzzle effects with their sounds;
    return to the hold behavior after the end frame
 */
VOID Pl_fire_shoot(VOID);

/*
    end of a shot sequence: return to the hold behavior and play the empty click when no ammo
    is left
 */
VOID Pl_fire_end_click(VOID);

/*
    begin raising the weapon again after a shot: select the raise animation for the aim level
    and run its first frame
 */
VOID Pl_fire_reraise_start(VOID);

/*
    advance the weapon animation of the re-raise state
 */
VOID Pl_fire_reraise();

/*
    begin sustained fire: reset the animation state and run the first frame
 */
VOID Pl_fire_hold_start(VOID);

/*
    sustained fire: while the fire button is held spend one round per frame with sound,
    vibration and muzzle effects, switch the aim animation on up/down changes and turn with
    left/right; return to the hold behavior when ammo or the button runs out
 */
VOID Pl_fire_hold(VOID);

/*
    empty fire sub-state
 */
VOID Pl_fire_nop(VOID);

/*
    begin the weapon reverse animation: reset the animation state, play the sound and run its
    first frame
 */
VOID Pl_fire_reverse_start(VOID);

/*
    advance the weapon reverse animation; an aim button press returns to state 5
 */
VOID Pl_fire_reverse(VOID);

/*
    set fire sub-state 5
 */
VOID Pl_fire_loop(VOID);

/*
    aim change state: play the aim up/down transition animation, forward or reversed, and return
    to the hold behavior when it ends or an aim button is pressed
 */
VOID Pl_gun_aim_chg(VOID);

/*
    holster state: turn with left/right, play the holster animation and clear the behavior when
    it ends
 */
VOID Pl_gun_holster(VOID);

/*
    reload state: play the reload animation and run the weapon's reload effect handler each
    frame; return to the hold behavior when it ends
 */
VOID Pl_gun_reload(VOID);

/*
    reload effects of the first weapon: start the ejected magazine once and, later in the
    animation, load the ammo with its sound
 */
VOID Pl_reload_beretta(VOID);

/*
    reload effects of the second weapon: shell loading sounds at three animation frames and the
    ammo transfer at the first
 */
VOID Pl_reload_shotgun(VOID);

/*
    reload effects of the third and fourth weapons: spawn an effect at the hand joint, then load
    the ammo with its sound
 */
VOID Pl_reload_colt(VOID);

/*
    reload effects of the remaining weapons: load the ammo with its sound at one animation point
 */
VOID Pl_reload_def(VOID);

/*
    lock-on turn state: blend into the aimed pose and turn the player toward the aim target,
    returning to the hold behavior once facing it
 */
VOID Pl_gun_lock(VOID);

/*
    empty-fire state: play the click sound and return to the hold behavior after 15 ticks
 */
VOID Pl_gun_click(VOID);

/*
    knife aim entry state: play the knife raise animation with auto-aim lock-on, turn with
    left/right and continue with the knife hold behavior when it ends
 */
VOID Pl_knife_aim(VOID);

/*
    knife hold state: select the hold animation for the aim level, start a swing on the fire
    button, turn with left/right and holster when the hold button is released
 */
VOID Pl_knife_hold(VOID);

/*
    knife swing state: play the swing animation with its sounds and effects, apply the hit
    check inside the swing frame window and return to the hold behavior when it ends
 */
VOID Pl_knife_swing(VOID);

/*
    knife holster state: turn with left/right and play the holster animation
 */
VOID Pl_knife_holster(VOID);

/*
    knife lock-on turn state: blend through the hold animations and turn the player toward the
    aim target, returning to the hold behavior once facing it
 */
VOID Pl_knife_turn(VOID);

/*
    player death state: play the death animation with the push-back movement, sound and pad
    vibration, then resize the shadow and finish in state 4
 */
VOID pl_die00(VOID);

/*
    store the animation slot and object of the joint joint_idx in the current entity's weapon
    part animation fields
 */
VOID Wep_joint_set(INT joint_idx);

/*
    rebuild the current entity's joint matrices and shift the entity by the displacement of the
    foot joint R_no so the foot stays in place
 */
VOID Foot_set_pl(UCHAR R_no);

/*
    update the head look-at rotation of the current entity: step the DirY and pitch of the look-at
    joint toward the target angle within the allowed limits
 */
VOID Rot_neck(VOID);

/*
    apply the look-at DirY and pitch rotation of the current entity to its look-at joint matrix
 */
VOID Pl_neck(VOID);

/*
    rebuild the current entity's hand joint matrices and set the aim up/down bit of Aim_flg
    from the hand height against the pitch bounds
 */
VOID Hand_ck(VOID);

/*
    return the ammo count of the equipped weapon slot; an empty slot is refilled to 4 or 6
    rounds when the matching game flag is set; 0 when no weapon with ammo is equipped
 */
UCHAR Tama_ck_i(VOID);

/*
    return nonzero when the Item_work holds ammo for the equipped weapon
 */
UCHAR Tama_reload_ck(VOID);

/*
    set the volley bit (0x80) in the equipped slot count; return 1 when it was clear and 0
    when it was already set
 */
INT Tama_once_ck(VOID);

/*
    reload the equipped weapon from the largest matching ammo stack: fill up to the weapon
    capacity, remove an emptied stack and sort the items
 */
VOID Tama_reload(VOID);

/*
    when the gun points toward the camera, spawn the muzzle flare effect(s) in front of the camera;
    return 1 when spawned, 0 otherwise
 */
INT Pl_view_esp_set();

struct PL_ESP_REC
{
    UCHAR frame;    // animation frame at which the effect is spawned
    UCHAR fx_type;  // effect type passed to Esp_call
    UCHAR Esp_dt;   // effect variant passed to Esp_call
    UCHAR flags;    // padding
    SHORT Ofs_x;    // effect offset X from the hand joint
    SHORT off_y;    // effect offset Y from the hand joint
    SHORT off_z;    // effect offset Z from the hand joint
};

struct AIM_LIM_REC
{
    USHORT min;         // lower limit of the aim height window, relative to the player height
    USHORT max;         // upper limit of the aim height window
    USHORT Alt_min;     // lower limit used while the player behavior flags are below 6 and not 3
    USHORT Alt_max;     // upper limit used while the player behavior flags are below 6 and not 3
};

struct AIM_DATA
{
    AIM_LIM_REC pitch[2];   // aim height window per player character
    UCHAR Anm_chain[16][2]; // per script animation id: action state minus 1, then animation id
};

extern UCHAR Aim_auto_en;                      // nonzero while auto-aim is enabled
extern PL_VIB_REC Pl_vib_tbl[9];                        // pad vibration of a shot, by weapon
extern PL_WEP_FN Pl_fire_tbl[11];              // fire sub-state handlers, by action state
extern UCHAR Pl_fire_end_tbl[0xC];             // animation frame after which the shot ends, by weapon
extern PL_HIT_REC Pl_fire_hit_tbl[9];               // hit frame, kind and sounds of a shot, by weapon
extern PL_ESP_REC Pl_flash_tbl[12];        // muzzle flash effect, by weapon
extern PL_ESP_REC Pl_shell_tbl[9];               // ejected shell effect, by weapon
extern PL_ESP_REC Pl_flash2_tbl[12];       // second muzzle effect, by weapon
extern UCHAR Pl_flash2_pad[8];           // data following the second muzzle effect table, not read by any code
extern UCHAR reload_fx_tbl[0x8];                    // data before Pl_reload_tbl; the reload dispatch indexes pointers from here
extern PL_WEP_FN Pl_reload_tbl[8];                     // reload effect handlers, by weapon
extern AIM_DATA Aim_data;          // hand height bounds for the aim level and scripted animation chain

#endif      // PL_WEP_H
