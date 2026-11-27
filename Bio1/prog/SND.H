/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SND.H
 */

#ifndef SND_H
#define SND_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBCD.H>
#include <LIBSND.H>
#include "SCRATCHPAD.H"

typedef UCHAR SND_VB_DATA[0x10000]; // staging buffer for a sound bank body

typedef struct
{
    SHORT left;  // left channel volume
    SHORT right; // right channel volume
} SND_VOL_DATA; // volume pair with signed channels

struct EDT_TABLE_WORK
{
    SCHAR id_check; // reserved, not accessed
    SCHAR prog;     // sound bank program number
    SCHAR tone;     // sound bank tone number
    SCHAR monopoly; // reserved, not accessed
};

/*
    reset the sound state (key-on mask, BGM state, VAB and SEP ids, sequence volumes),
    initialise the sound library with its sequence table, tick mode and reverb, then start it
 */
VOID Snd_sys_init(VOID);

/*
    start the sound library and set the master volume, reverb volume and CD audio input
 */
VOID Snd_sys_start(VOID);

/*
    close the open SEP and VABs, turn the reverb off and shut the sound library down
 */
VOID Snd_sys_init2(VOID);

/*
    restart the sound system by running its initialisation again
 */
VOID Snd_title_init(VOID);

/*
    close the enemy sound bank (VAB 3) when it is open
 */
VOID Snd_vab3_close(VOID);

/*
    end and quit the sound library
 */
VOID Snd_sys_end(VOID);

/*
    select stereo (low byte non-zero) or mono output and set the CD audio mix to match
 */
VOID Snd_sys_stereo(UINT sorm);

/*
    load the core sound bank (VAB 0) of the given type: read its header and body files,
    open the header, transfer the body to the SPU and wait for the transfer to finish
 */
VOID Snd_sys_core_set(UCHAR core_type_no);

/*
    load the weapon sound bank (VAB 1) of the given arms type, reading its body into pVb_buf
 */
VOID Snd_sys_arms_set(INT arms_type_no, UCHAR* pVb_buf);

/*
    load the sound bank of the current room (VAB 2) from the header and body in the room data,
    when the room has one
 */
VOID Snd_sys_room_set(VOID);

/*
    load the enemy sound bank (VAB 3) of the given enemy type, reading its body into the load
    buffer
 */
VOID Snd_sys_enem_set(INT enem_type_no);

/*
    load the BGM bank file that the stage BGM table gives for the current room and Bgm_cnf: close the
    open BGM VAB, copy the header and sequence data of the file to the BGM work area, open the VAB
    (VAB 4), transfer the body and open the sequence set
 */
VOID Snd_bgm_main(INT Bgm_cnf);

/*
    close the open SEP, open the sequence set at pSep with the BGM VAB, set the volume of
    each of its three sequences and start them
 */
VOID Snd_bgm_sep_open(UCHAR Bgm_cnf, VOID* pSep);

/*
    restart the BGM sequences 0, 1 and 2 selected by bits 0x08, 0x10 and 0x20 of the target
    BGM state and record them in the current BGM state
 */
VOID Snd_bgm_play_ck(VOID);

/*
    select the BGM of the current room from the room BGM configuration: stop it when the room has
    none; otherwise fade out and load a new bank when its file differs from the previous room or
    when bits 6 and 7 of the configuration force a reload, else start or stop single sequences
    to reach the target state; the target state becomes the current state
 */
VOID Snd_bgm_set(VOID);

/*
    start a BGM sequence of the open SEP
 */
VOID Snd_bgm_play(SHORT seq);

/*
    stop a BGM sequence of the open SEP when a BGM is selected
 */
VOID Snd_bgm_stop(SHORT seq);

/*
    set the left and right volume of a BGM sequence of the open SEP
 */
VOID Snd_bgm_vol_set(SHORT seq, SHORT Vol_l, SHORT Vol_r);

/*
    play sound effect SENo of sound bank Edt_no (0 core, 1 weapon, 2 room, 3 enemy, 4 BGM) panned
    and attenuated for the source position pVec; effects beyond the size of the bank are ignored
 */
VOID Snd_se_stad(UCHAR Edt_no, UCHAR SENo, INT flg, VOID* pVec);

/*
    play sound effect SENo of sound bank Edt_no (0 core, 1 weapon, 2 room, 3 enemy, 4 BGM) at the
    volume stored in its tone; effects beyond the size of the bank are ignored
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
VOID Snd_se_play(INT Edt_no, INT SENo, INT Pan);
#define VAG_ATR(pTone) ((VagAtr*)(pTone))  // the tone record of a VAB header, which the code steps through as a byte pointer

/*
    play a footstep effect of the room bank: choose the entry from the step type and the floor
    material under the current entity (a fixed entry in the special floor case), panned for its
    position
 */
VOID Snd_se_walk(UCHAR no, INT Pan);

/*
    play enemy sound effect SENo (0 to 9) of the room bank, shifted by ten per sound group of the
    current entity, panned for its position
 */
VOID Snd_se_enem(UCHAR SENo, INT Pan);

/*
    read the key status of the SPU voices and clear the key-on mask when the first voice is off
 */
VOID Snd_voice_ck(VOID);

/*
    key off the voices in the key-on mask and clear the mask
 */
VOID Snd_key_off_all(VOID);

/*
    compute the left and right volume of a sound effect at pVec from its distance and angle to
    the camera of the current cut: pan by angle, attenuate by distance, minimum 30
 */
VOID Snd_se_3D(VECTOR* pVec);

/*
    start a BGM fade: keep the current master volume and set the fade step and mode
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
INT Snd_bgm_fade_ON(INT time, INT mode);

/*
    per-frame BGM fade: step the master volume down to silence, then count the end of the fade,
    stopping the sequences, keying off all voices and closing the SEP and BGM VAB, and restore
    the full volume
 */
VOID Snd_bgm_fade(VOID);

/*
    when a SEP is open, fade out every playing BGM sequence, stop them, key off all voices and
    close the SEP and the BGM VAB
 */
VOID Snd_bgm_fade_all(VOID);

/*
    read the header and body files of arms type 10 into the preload buffer, behind a 4-byte header
    size; returns the size of the stored data
 */
INT Snd_bank1_load(VOID);

/*
    open the weapon sound bank (VAB 1) from the preloaded header and body in memory and transfer
    the body to the SPU
 */
VOID Snd_vab1_load(VOID);

extern UCHAR Snd_unused_vg_set[4];          // unused
extern USHORT Snd_unused_rev2;                // unused
extern CHAR Seq_table[516];    // sequence table storage given to the sound library
extern SndVolume Main_vol;                  // master volume of the BGM fade, saved when it starts and stepped while it runs
extern UCHAR* Snd_pre_adr;     // preloaded sound bank 1 data, advanced past each part while the bank is opened
extern SND_VOL_DATA Pan_vol;            // left and right volume of the sound effect being started
extern INT Snd_unused_err;           // unused
extern UCHAR* Snd_vh_core;           // VAB header of the core sound bank (VAB 0)
extern UCHAR* Snd_vab1_head;           // VAB header of the weapon sound bank (VAB 1)
extern INT* Snd_vh_room;              // VAB header of the room sound bank (VAB 2)
extern UCHAR* Snd_vab3_head;           // VAB header of the enemy sound bank (VAB 3)
extern UCHAR* Bgm_vh;       // VAB header of the BGM bank (VAB 4)
extern UINT Snd_keyon_mask;              // voice key-on mask of the sound system
extern SCHAR Spu_key_stat;                  // start of the SPU voice key status buffer
extern USHORT Bgm_bank_file[58];              // BGM bank file codes
extern USHORT Core_edh[16];                    // core sound bank header file codes, by core type
extern USHORT Core_vbd[16];                    // core sound bank body file codes, by core type
extern USHORT Arms_edh[16];                    // weapon sound bank header file codes, by arms type
extern USHORT Arms_vbd[16];                    // weapon sound bank body file codes, by arms type
extern USHORT Enem_edh[8];           // enemy sound bank header file codes, by enemy type
extern USHORT Enem_vbd[8];            // enemy sound bank body file codes, by enemy type

#endif      // SND_H
