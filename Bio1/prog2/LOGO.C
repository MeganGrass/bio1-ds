/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog2\LOGO.C
 */

#include "LOGO.H"
#include "FILECODE.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "MONITOR.H"

static LOGO_WORK Logo_wk; // movie playback state

static VOID* Logo_vlc_buf0; // address of the first VLC decode buffer
static VOID* Logo_vlc_buf1; // address of the second VLC decode buffer

static VOID* Logo_ring_buf;  // address of the stream ring buffer
static VOID* Logo_slice_buf; // address of the slice (decoded macroblock) buffer

VOID main(VOID)
{
    INT Cbuf;    // index of the active draw buffer
    INT Rsv;         // unused local: reserves stack space
    INT Rsv2;         // unused local: reserves stack space
    INT Eom;       // nonzero once the movie has ended
    INT frame;         // frame number of the decoded frame; not read
    INT Rsv3;         // unused local: reserves stack space
    RECT rect;

    FUNC(SND_TITLE_INIT)();  // Snd_title_init

    Logo_slice_buf = LOGO_SLICE_BUF;
    Logo_ring_buf = LOGO_RING_BUF;
    Logo_vlc_buf0 = LOGO_VLC_BUF0;
    Logo_vlc_buf1 = LOGO_VLC_BUF1;

    Logo_wk.Movie_x = 0;
    Logo_wk.skipped = 0;

    FUNC(GS_INIT_GRAPH)(0x140U, 0xF0, 0, 0, 1);  // GsInitGraph

    FUNC(SET_MOVIE_VOLUME2)(0x40U);  // Set_movie_volume2

    FUNC(CD_SET_STEREO)(((G.SaveData.Key_idx >> 7) ^ 1) & 0xFF);  // Cd_set_stereo

    FUNC(SS_SET_SERIAL_ATTR)(0U, 0, 1);  // SsSetSerialAttr

    Eom = FUNC(OPEN_MOVIE)(CAPCOM_STR, 1, 0x92, Logo_ring_buf, 0x18, Logo_vlc_buf0, Logo_vlc_buf1, Logo_slice_buf, 1);  // Open_movie

    while (Eom == 0)
    {
        Cbuf = FUNC(GS_GET_ACTIVE_BUFF)();  // GsGetActiveBuff
        Eom = FUNC(PLAY_MOVIE)(Logo_wk.Movie_x, Cbuf != 0 ? 0xF0 : 0);  // Play_movie
        FUNC(STR_DECODE_WAIT)();  // Str_decode_wait
        frame = FUNC(STR_FRM_GET)();  // Str_frm_get

        if (FUNC(PAD_RAW_GET)(0U) & 0xDF0)  // Pad_raw_get
        {
            Logo_wk.skipped = 1;
            break;
        }
        FUNC(VSYNC)(0U);  // VSync
        FUNC(GS_SWAP_DISP_BUFF)();  // GsSwapDispBuff
    }

    rect.y = 0;
    rect.x = 0;
    rect.w = 0x1E0;
    rect.h = 0x1E0;
    FUNC(CLEAR_IMAGE)(&rect, 0, 0, 0);  // ClearImage

    FUNC(CLOSE_MOVIE)();  // Close_movie
    FUNC(SS_SET_SERIAL_ATTR)(0U, 0, 0);  // SsSetSerialAttr

    G.System_flg &= ~SYS_BG_MODE_MASK;

    FUNC(SND_SYS_END)();  // Snd_sys_end

    FUNC(TASK_LCHAIN)(TITLE_EXE);  // Task_lchain
}
