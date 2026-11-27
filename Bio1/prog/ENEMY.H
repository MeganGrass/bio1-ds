/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ENEMY.H
 */

#ifndef ENEMY_H
#define ENEMY_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

struct CORPSE_SAVE_WORK
{
    UCHAR status_flags;     // saved enemy status flags
    UCHAR behavior_flags;   // saved enemy behavior flags
    UCHAR Room_no;          // room the enemy was saved in
    UCHAR Slot_info;        // enemy slot of the saved record
    INT state;              // saved enemy state
    SHORT pos_x;            // saved x position
    SHORT pos_y;            // saved y position
    SHORT pos_z;            // saved z position
    UCHAR pad0E[2];         // padding
    USHORT angle;           // saved facing angle
    UCHAR Dummy12[2];       // padding
    UCHAR rooms_left;       // room changes left before the record is dropped, 0 marks a free record
    UCHAR pad15[7];         // padding
};

typedef VOID (*ENEMY_FUNC)(VOID);

#define EM_SAVE_CUR (*&MAIN.Em_save_cur)   // saved entity entry being examined (*& form kept: a plain access changes the code)

/*
    load the model file of enemy model id for the current player character into the load buffer,
    map its textures and model and set the model and animation pointers of pEm
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
VOID Emd_load(ENEMY_WORK* pEm, INT id);

/*
    load the weapon model of the current entity's behavior number and attach it to joint 14
    Declared without a parameter list: a caller passes an argument the function ignores
 */
VOID Em_wep_load();

/*
    load the animation and model files of a player weapon into buf and attach the model to the joint
    joint_index, or restore the stored weapon part when the weapon number is 0
 */
VOID Pl_weapon_ch(INT weapon, INT joint_index, VOID* buf, UINT* anim_object);

/*
    run the scripted action handler of the current entity, run it again when flag 2 is set, then
    when flag 4 is set pin the entity to the foot joint chosen by flag 8; handlers signal
    completion by setting the script flag numbered by Set_flg_no
 */
VOID Spl_Scenario(VOID);

/*
    set the move speed of the current entity, lowered during the walk and run animation frames
 */
VOID Walk_spd_set(USHORT speed);

/*
    scripted action 0: play animation 0 with no end
 */
VOID Spl_sce00(VOID);

/*
    scripted action 1: stand still and play the animation to its end, then turn by the script
    timer and signal completion
 */
VOID Spl_sce01(VOID);

/*
    scripted action 2: turn towards the target position, then walk to it with animation 7 and
    signal completion
 */
VOID Spl_sce02(VOID);

/*
    scripted action 3: turn towards the target position, then move to it with animation 8, slow
    down and signal completion
 */
VOID Spl_sce03(VOID);

/*
    scripted action 4: move backwards to the target position with animation 3 and signal
    completion
 */
VOID Spl_sce04(VOID);

/*
    scripted action 5: move backwards to the target position with animation 2 and signal
    completion
 */
VOID Spl_sce05(VOID);

/*
    scripted action 6: turn towards the target position with animation 7 until facing it, then
    clear the behavior and signal completion
 */
VOID Spl_sce06(VOID);

/*
    scripted action 7: play the animation to its end, turning by the script timer, then signal
    completion
 */
VOID Spl_sce07(VOID);

/*
    scripted action 8: play the weapon animation chosen by the behavior number, spawning the muzzle
    flash, shell and smoke effects at their set frames, then signal completion; states 4 and 5
    loop an animation with a repeating effect and sound
 */
VOID Spl_sce08(VOID);

/*
    scripted action 9: play the animation to its end, then clear the behavior and signal completion
 */
VOID Spl_sce09(VOID);

/*
    scripted action 10: stand still, play the animation to its end, then signal completion
 */
VOID Spl_sce0a(VOID);

/*
    set the texture page number in every primitive of the model object
 */
VOID Tmd_tpage_set(TMD_MEM* obj, INT tpage);

/*
    run the update handler of every active entity and queue the mirror reflection of those in view
 */
VOID Em_main(VOID);

/*
    store the live entities in the saved entity table, age the saved entries when the room has
    changed and free every entity slot
 */
VOID Em_save_set(VOID);

/*
    restore the state and position of the saved entry of slot for the current room into the current
    entity, returning 1 when found
 */
INT Em_save_get(INT slot, VOID* pScd);

extern UCHAR Emd_file_tbl[276];         // CD file number of each enemy model as 16-bit values, by model id for each player character
extern UINT Emd_clut_mask;            // bit mask by model id: models that register a CLUT fade when loaded
extern USHORT Wep_model_tbl[5][7]; // CD file number of the weapon model, by model group then behavior number
extern USHORT Wep_model_pad;    // padding: keeps the following data at its linked position
extern USHORT Wep_anim_file[52];      // CD file number of the weapon animation, by player character then weapon number
extern ENEMY_FUNC Spl_sce[11];                  // scripted action handlers, by action behavior

#endif      // ENEMY_H
