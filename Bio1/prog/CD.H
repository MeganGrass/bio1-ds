/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\CD.H
 */

#ifndef CD_H
#define CD_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBCD.H>

typedef VOID (*XA_FUNC)(VOID);

extern USHORT Xa_no[];  // CD file index of the voice (XA) file, by stage id

/*
    wait for the CD-ROM library to initialise, clear the checksum bypass, set the XA volume and idle the XA state
 */
VOID Cd_init(VOID);

/*
    script voice command: cmd 1 starts voice clip number clip, cmd 2 fades the XA volume out and stops playback
 */
VOID Xa_cmd(INT cmd, INT clip);

/*
    empty function
 */
VOID Cd_nop(VOID);

/*
    start an asynchronous read of file number file into dst: wait for the drive to idle, set the drive mode, seek
    to the file's start sector and issue the read; returns 0 when started, or 1 after resetting the library when the drive was found stopped
 */
INT Cd_file_read(INT file, UINT* dst);

/*
    return the start sector of file number file and store its byte size through pSize when not null;
    returns 0 for a file number beyond the table
 */
INT Cd_floc_get(UINT file, INT* pSize);

/*
    XOR the first byte of every 512 bytes of the file just read and compare with the checksum in the file
    table; prints an error and returns 0 on mismatch, 1 when it matches or the check is bypassed
 */
INT Cd_sum_ck(VOID);

/*
    XOR the first byte of every 2048 bytes of the 32KB room background just read and compare with the
    stage's background checksum table for the current room and cut; prints an error and returns 0 on mismatch
 */
INT Cd_bg_sum_ck(VOID);

/*
    start XA voice clip number clip of the current stage: walk the stage's clip offset table to find the
    channel and the start and end sectors, then start the stream
 */
VOID Xa_play(UINT clip);

/*
    start XA streaming at the stored start position: set the drive mode and XA filter, then seek and read,
    making up to 5 attempts; enters state 1 when started and state 0 when out of retries
 */
VOID Xa_start(VOID);

/*
    set the XA volume level (low 8 bits of vol)
 */
VOID Xa_set_vol(INT vol);

/*
    set the CD audio mix to volume vol: stereo routes left to left and right to right at full volume,
    mono mixes all four paths at half volume
 */
VOID Set_volume(UCHAR vol);

/*
    select mono output and apply the current XA volume
 */
VOID Cd_stereo_off(VOID);

/*
    select stereo output and apply the current XA volume
 */
VOID Cd_stereo_on(VOID);

/*
    run the handler of the current XA state (stop, init or play) from the state table
 */
VOID Xa_control(VOID);

/*
    state 0: clear the voice-playing game flag
 */
VOID Xa_control_stop(VOID);

/*
    state 1: wait for the drive to start the XA stream; on a drive error, a bad status or a timeout of
    181 frames fade the volume out and stop, once playing fade the volume in and enter state 2
 */
VOID Xa_control_init(VOID);

/*
    state 2: while playing, poll the disc position and status; fade out and stop when the position
    passes the end sector (restarting the stream when far beyond it), when the drive stops reading,
    or after 2251 frames
 */
VOID Xa_control_play(VOID);

/*
    wait for the drive to be idle with no shell-open or seek error, then issue command com with
    parameter param, retrying up to 80 times; returns 1 when accepted, 0 otherwise
 */
INT Cd_ctrl_retry(UCHAR com, UCHAR* param, UCHAR* res);

extern UCHAR Cd_status_res[8];       // result bytes of the last drive status query
extern UINT Xa_start_retry;          // number of attempts made to start the XA stream
extern INT Cd_file_size;          // byte size of the file being read
extern UCHAR Cd_xa_filter;             // XA filter file number for the set-filter command
extern SCHAR Xa_filter_chan;        // XA filter channel number, follows the file number
extern UINT Cd_xa_frame_cnt;    // frames since the XA stream was started or last checked
extern INT Cd_sector;         // current sector number
extern UINT Xa_end_sector;                 // last sector of the XA clip being played
extern INT Xa_start_sector;              // first sector of the XA clip being played
extern INT Xa_st_no;                  // XA state: 0=stopped, 1=starting, 2=playing; index into xa_control_tbl
extern INT Cd_file_no;            // file number of the file being read
extern INT Cd_stereo_mode;        // 1=stereo mix, 0=mono mix
extern INT Cd_read_busy;       // set to 1 when a file read is started
extern UCHAR* pCd_read_buf;        // destination buffer of the file being read
extern INT Xa_vol_info;                 // current XA volume level
extern CdlLOC Xa_start_loc;         // disc position at which the XA clip starts
extern CdlLOC Cd_read_loc;             // disc position at which the file read starts
extern CdlATV Cdvol;                        // CD audio mix levels passed to CdMix
extern XA_FUNC xa_control_tbl[3];              // XA state handlers: stop, init, play

#endif      // CD_H
