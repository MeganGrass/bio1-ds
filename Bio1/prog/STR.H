/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\STR.H
 */

#ifndef STR_H
#define STR_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBCD.H>

typedef struct STR_SLICE_WORK STR_SLICE_WORK;
typedef struct STR_GETLOC_BUF STR_GETLOC_BUF;
typedef struct CD_AUDIO_DATA CD_AUDIO_DATA;

struct STR_SLICE_WORK
{
    RECT Rect;      // VRAM rectangle receiving the current slice
    UINT Dummy8;    // padding after the rectangle
    INT Size;       // size of one decoded slice, in words
    UINT Count;     // number of the slice being decoded
    UINT Loop;      // slices per frame
};

struct STR_GETLOC_BUF
{
    UCHAR result[5];    // track, index and relative time bytes of the position reply
    CdlLOC position;    // absolute disc position of the reply
    UCHAR Dummy9[11];   // spare reply space
};

struct CD_AUDIO_DATA
{
    INT volume;     // CD audio volume, 0 to 0x80
    INT stereo;     // nonzero = stereo mix, 0 = mono mix
    INT Skip_flg;   // nonzero = frames are read but not decoded
};

extern INT StCdIntrFlag; // CD stream interrupt pending flag (SDK variable)

/*
    open a movie by CD file name: reset the MDEC, set up the ring buffer and stream, start the read
    and VLC-decode the first frame into the first VLC buffer; returns 0 on success, 1 when the file
    is not found, the read cannot start or no frame arrives within 601 VSyncs
 */
INT Str_open(SCHAR* fname, UINT first_frame, INT Frm_end, UINT* pRing_buf, UINT Ring_len, UINT* pRl_buf0, UINT* pRl_buf1, VOID* pSlice_buf, UINT flags);

/*
    open a movie by CD file index: set up the ring buffer and stream, start the read at the file's
    start sector, check the drive position, start the audio fade-in and VLC-decode the first frame;
    returns 0 on success, 1 when the drive is not near the start sector or no frame arrives within 301 VSyncs
 */
INT Open_movie(INT file_id, UINT first_frame, INT Frm_end, ULONG* pRing_buf, UINT Ring_len, ULONG* pRl_buf0, ULONG* pRl_buf1, VOID* pSlice_buf, UINT flags);

/*
    decode and display one movie frame at X position Dst_x_flg & 0x3FF (bit 0x400 sets and bit
    0x800 clears the decode-skip flag), Y position dst_y: wait up to 180 VSyncs for the next frame,
    start the MDEC decode of the frame prepared earlier, swap the VLC buffers and VLC-decode the new frame;
    returns 1 at the end frame, on a frame number going backwards or on timeout, else 0
 */
INT Play_movie(INT Dst_x_flg, SHORT dst_y);

/*
    stop the movie: fade out the audio, clear the MDEC and CD callbacks, release the ring buffer
    and wait for the drive to finish, restoring the drive mode on an error status
 */
VOID Close_movie(VOID);

/*
    wait until the slice decode in progress has finished
 */
VOID Str_decode_wait(VOID);

/*
    return nonzero once the current frame number has reached the end frame
 */
INT Str_end_ck(VOID);

/*
    return the number of the frame most recently handed to the MDEC by Play_movie
 */
INT Str_frm_get(VOID);

/*
    fade out the audio, take the start sector from the CD file entry and issue the set-filter, seek
    and read commands until the read starts (restoring the drive mode on an error status), then
    fade the audio in; returns 0
 */
INT Str_cd_start();

/*
    MDEC output callback: service a pending CD stream interrupt, upload the decoded slice to VRAM
    and start decoding the next slice, or clear the busy flag after the last slice of the frame
 */
VOID Mdec_callback(VOID);

/*
    restart the stream Frame * 10 sectors after the start of the movie file: clear the ring
    buffer and repeat the pause, seek and read commands until the read starts
 */
VOID Fwd_movie(INT Frame);

/*
    Ramp the CD audio mixer from 0 up to the current volume in steps of 1, with the channels
    straight for stereo or mixed at half level for mono
 */
VOID Movie_xa_start(VOID);

/*
    Ramp the CD audio mixer from the current volume down towards 0 in steps of 2, with the channels
    straight for stereo or mixed at half level for mono
 */
VOID Movie_xa_stop(VOID);

/*
    set the CD audio volume and apply it to the mixer according to the stereo flag
 */
VOID Set_movie_volume2(UINT Volume);

/*
    set the stereo flag and apply the current volume to the CD mixer: channels straight for
    stereo, mixed at half level for mono
 */
VOID Cd_set_stereo(INT mode);

/*
    send the CD set-mode command with mode 0x90; returns 0 when the reply reports an error
    status, else waits 3 VSyncs, seeks to the start of file 0, pauses and returns 1
 */
INT Cd_recover_90(VOID);

/*
    send the CD set-mode command with mode 0xA8; returns 0 when the reply reports an error
    status, else waits 3 VSyncs, seeks to the start of file 0, pauses and returns 1
 */
INT Cd_recover_a8(VOID);

/*
    seek the drive to the start of file 0 and pause it
 */
VOID Cd_seek_pause(VOID);

/*
    decode one still image: reset the MDEC, VLC-decode the pBs, feed it to the MDEC and
    read the decoded pixels (16-bit, or 24-bit when Rgb24_flg) into pOut_buf, waiting for completion
 */
VOID Mdec_img_decode(ULONG* pBs, ULONG* pRl_buf, ULONG* pOut_buf, INT w, INT h, INT Rgb24_flg);

/*
    decode one still image strip by strip (16 or 24 pixels wide) through pSlice_buf and upload each
    strip to the VRAM rectangle dst_rect
 */
VOID Mdec_vram_decode(RECT* dst_rect, UINT* pBs, UINT* pRl_buf, UINT* pSlice_buf, INT Rgb24_flg);

/*
    upload an already decoded image from memory to the VRAM rectangle src, strip by strip (16 or
    24 pixels wide)
 */
VOID Mdec_img_upload(RECT* src, UINT* buf, INT Rgb24_flg);

/*
    set the two frame buffers of the PC-file stream: the second starts Sector_num / 2 sectors (2048
    bytes each) after the first
 */
VOID Str_pc_buf_set(UCHAR* buf, UINT Sector_num);

/*
    open the PC-side movie file, detect whether its sectors carry sub-headers, read frames until
    the start frame is reached and call the frame callback; returns 0, or -1 when the file cannot
    be opened
 */
INT Str_pc_open(CHAR* name);

/*
    reset the PC-file stream state: buffer index, first and last frame, callbacks and end-of-file flag
 */
VOID Str_pc_init(INT unused, INT first_frame, INT Frm_end, VOID (*pOpen_cb)(), INT (*pEnd_cb)());

/*
    close the PC-file stream
 */
VOID Str_pc_close(VOID);

/*
    switch to the other frame buffer and read the next frame into it, then call the frame callback
    and, at the end frame or end of file, the end callback; returns 0
 */
INT Str_pc_frm_next(VOID);

/*
    return the current frame buffer and its sector header; returns 0
 */
INT Str_pc_frm_get(UCHAR** pFrm_buf, VOID** header);

/*
    read the sectors of one frame from the PC file into buf (2016 bytes per sector, skipping
    sub-headers when present) up to the last sector of the frame; sets the end-of-file flag when
    the file ends or the end frame is read
 */
VOID Str_pc_frm_read(UCHAR* buf);

extern CD_AUDIO_DATA Cd_audio; // CD audio volume, stereo flag and decode-skip flag
extern INT Str_unused_ring;        // unused

#endif
