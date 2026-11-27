/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog2\ENDING.H
 */

#ifndef ENDING_H
#define ENDING_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

struct ENDING_PIC_REC
{
    USHORT Show_frm;    // video frame at which the picture appears
    USHORT Move_frm;    // video frame at which the cover boxes start moving

    SHORT Box_x;        // cover box start x, screen pixels
    SHORT Box_y;        // cover box start y, screen pixels
    USHORT Box_spd_x;   // cover box x speed per frame
    USHORT Box_spd_y;   // cover box y speed per frame
};

struct END_SPR_WORK
{
    USHORT active;      // 1 while the slot is in use
    USHORT type;        // draw type: 0=text strip 1=background 2=caption 3=clear time 5=save count 6=logo zoom 7=picture cover boxes
    USHORT Ot_pri;       // ordering table priority (unsigned: passed with an unsigned halfword load)
    USHORT Vram_x;      // source x of the strip image in video memory
    USHORT Vram_y;      // source y of the strip image in video memory
    UCHAR pad0A[2];     // padding
    INT x;              // screen x, 16.16 fixed point, left edge at 0
    INT y;              // screen y, 16.16 fixed point, top edge at 0

    USHORT Box_spd_x;   // cover box x speed per frame (type 7)
    USHORT Box_spd_y;   // cover box y speed per frame (type 7)
    SHORT Fade_spd;     // fade level change per frame
    SHORT Fade_val;     // Kido, -0x8000 = black to 0 = full; colour is Fade_val >> 8
    UCHAR pad1C[4];     // padding
    INT Tex_flg;        // sprite attribute word: pixel mode, blend and hide bits
    USHORT Scr_x;       // sprite screen x, centre origin
    SHORT Scr_y;        // sprite screen y, centre origin
    SHORT Tex_w;        // sprite width in pixels
    SHORT Tex_h;        // sprite height in pixels
    SHORT tex_page;     // texture page id
    SCHAR Tex_u;        // sprite texture u
    UCHAR Tex_v;        // sprite texture v
    SHORT clut_x;       // palette x in video memory
    SHORT clut_y;       // palette y in video memory
    SCHAR Col_r;        // red modulation, from Fade_val
    SCHAR Col_g;        // green modulation, from Fade_val
    SCHAR Col_b;        // blue modulation, from Fade_val
    UCHAR Dummy37[1];   // padding
    SHORT Pivot_x;      // sprite rotation pivot x
    SHORT Pivot_y;      // sprite rotation pivot y
    SHORT Scale_x;      // horizontal scale, 0x1000 = 1.0
    SHORT Scale_y;      // vertical scale, 0x1000 = 1.0
    INT rotate;         // sprite rotation
};

struct ENDING_TEXT_HEAD
{
    USHORT size;    // block size in bytes
    USHORT Run_ofs; // offset of the run length list
    USHORT Val_ofs; // offset of the run value bytes, 0xFFFF = raw image data
};

struct CAPTION_TBL
{
    USHORT Start_frame;    // video frame at which the caption fades in
    USHORT Hide_frm;       // video frame at which the caption fades out
    USHORT Pos_x;          // caption screen x
};

typedef struct ENDING_BG_REC ENDING_BG_REC;
typedef struct ENDING_DEF_REC ENDING_DEF_REC;
typedef struct ENDING_SPR_REC ENDING_SPR_REC;

struct ENDING_BG_REC
{
    USHORT Show_frm;    // movie frame at which the cue appears
    USHORT Hide_frm;    // movie frame at which the cue disappears
    SHORT x;            // horizontal screen position
    SHORT y;            // vertical screen position
};

struct ENDING_DEF_REC
{
    UCHAR a;                // 1 = play a second movie after the credits
    UCHAR b;                // credits scroll mode: start step is b ^ 1, 0 also loads the extra background
    UCHAR c;                // 1 = show the result screen and award the rocket launcher when it is missing
    UCHAR d;                // nonzero = alternate graphics set and epilogue sprite layout
};

struct ENDING_SPR_REC
{
    USHORT type;    // sprite slot type
    SHORT x;        // horizontal screen position
    SHORT y;        // vertical screen position
    UCHAR Tex_v;    // texture row the sprite starts at
    UCHAR Tex_h;    // sprite height in pixels
};

/*
    ending program entry: load the ending movie and graphics, run the credits, the epilogue and the
    result screen, reset the player data for the next game with the rewards earned, write the save
    data and chain to the next program
 */
VOID main(VOID);

/*
    reset the scene state: video sync mode, text block cursor (back to the start of the work buffer)
    and strip index
 */
VOID Ending_reset(VOID);

/*
    set up the full-screen background as two sprite slots, 256 and 64 pixels wide, read from video
    memory
 */
VOID Ending_bg_set(VOID);

/*
    select the first inactive sprite slot as the current slot, or none when all are in use
 */
VOID Ending_spr_get(VOID);

/*
    deactivate every sprite slot
 */
VOID Ending_spr_clr(VOID);

/*
    set up the current slot as a credits text strip sourced from the next text block and upload that
    block to video memory
 */
VOID Ending_strip_set(VOID);

/*
    advance to the next text block and expand its run-length data into the small buffer; a block with
    byte offset 0xFFFF holds raw image data and is used as it is
 */
VOID Ending_blk_set(VOID);

/*
    empty function
 */
VOID Ending_nop(VOID);

/*
    step through the picture cue list by movie frame: place the two cover boxes at a cue's show
    frame and start them sliding at its move frame, until the 0xFFFF end marker
 */
VOID Ending_pic_main(VOID);

/*
    step through the caption page list by movie frame: upload each page's text strips, fade the text
    in at the page's show frame and out at its hide frame, until the 0xFFFF end marker
 */
VOID Ending_cap_main(VOID);

/*
    set up the caption text slot and clear its area in video memory
 */
VOID Ending_cap_set(VOID);

/*
    draw every active sprite slot: update screen position and fade, derive the colour and queue the
    slot by type (credits strip, background, caption, play time, save count, logo, cover boxes)
 */
VOID Ending_spr_draw(VOID);

/*
    choose the ending number 1 to 7 from event flags 0xC0 and 0x4B and the player character
 */
VOID Ending_no_set(VOID);

/*
    epilogue state machine: fade in the background and picture sprites in turn, hold, fade them out,
    run the clear time check and finish the step
 */
VOID Ending_epi_main(VOID);

/*
    set up the five epilogue sprite slots from Ending_spr_tbl, the epilogue camera and room
    data, and upload the player picture or clear its area
 */
VOID Ending_epi_set(VOID);

/*
    set the under-time flag and point the room collision data at the empty block when the play time
    is 3 hours or less, otherwise clear the flag
 */
VOID Ending_time_set(VOID);

/*
    logo zoom state machine: fade the logo slot in, shrink its scale as the depth grows, fade it out and
    finish the step after 300 frames
 */
VOID Ending_zoom_main(VOID);

/*
    result screen state machine: fade in, wait for a button or 270 frames, fade out; each frame also
    draws the sprites and the spinning model
 */
VOID Ending_res_main(VOID);

/*
    build the result screen: load and upload the player picture, set up the two label slots, the
    model coordinates, camera, light and model object
 */
VOID Ending_res_set(VOID);

/*
    draw the text in the scratch buffer as sprites, one 8 by 14 character cell each
 */
VOID Ending_text_draw(VOID);

extern UINT Ending_cap_tbl00[32];                       // caption page timing and position list, set 0
extern UINT Ending_cap_tbl01[32];                     // caption page timing and position list, set 1
extern CAPTION_TBL* Ending_cap_tbl[2];            // caption page list per text set
extern UINT Ending_bg_tbl00[28];                  // background cue list, set 0 (not referenced by code)
extern UINT Ending_bg_tbl01[28];                  // background cue list, set 1 (not referenced by code)
extern ENDING_BG_REC* Ending_bg_tbl[2];               // background cue list per text set (not referenced by code)
extern UINT Ending_pic_tbl00[6];                     // picture cue list, set 0
extern UINT Ending_pic_tbl01[6];                     // picture cue list, set 1
extern UINT Ending_pic_tbl02[3];                     // empty picture cue list
extern ENDING_PIC_REC* Ending_pic_tbl[2];     // picture cue list per text set
extern ENDING_DEF_REC Ending_def_tbl[7];                // per-ending options, indexed by ending number - 1
extern ENDING_SPR_REC Ending_spr_tbl[12]; // sprite slot layout for the epilogue and result screens
extern INT Ending_cam_tbl[5];                      // camera switch zone data for the epilogue room
extern UINT Ending_sca_tbl[6];                    // empty collision block for the epilogue room

#endif
