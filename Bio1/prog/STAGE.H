/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\STAGE.H
 */

#ifndef STAGE_H
#define STAGE_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

struct STAGE_RES_WORK
{
    UCHAR Pl_emd[0xCC00];       // player model file
    UCHAR Pl_pac[0xDC00];       // polygon packets of the player model
    UCHAR Wep_mot[0x7A00];      // weapon animation data
    UCHAR Wep_pac[0x1A00];      // polygon packets of the weapon model
    UCHAR Core_esp[0x1800];     // core effect data
    UCHAR Vh1[0x1000];          // header of sound bank 1
    UCHAR Vh0[0x1000];          // header of sound bank 0
    UCHAR Vh3[0x1000];          // header of sound bank 3
    UCHAR Bgm_vh_sep[0x7C00];   // background music header and sequence data
};

/*
    start a new stage: reset the previous room, select the stage's row of the room BGM
    configuration, fade out an open BGM, load the stage overlay, mute the current room's BGM
    entry in attract mode, upload a 64x56 image from the load area to VRAM and enter the
    current room
 */
VOID Set_stage(VOID);

/*
    load and start the overlay of the current stage as task 2, set the load pointer past it and
    let the task run for a frame, then fill the enemy update table: Em13_Main for entry
    0x13 and the character handler Spl for entries 0x17 to 0x2F
 */
VOID Stage_ov_load(VOID);

extern UCHAR Stage_ov_tbl[0x118];    // overlay file codes of the stages (16-bit entries indexed by stage id), then data not read here

#endif      // STAGE_H
