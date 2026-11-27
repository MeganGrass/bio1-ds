/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\MOVIE.C
 */

#include "MOVIE.H"
#include "CUT.H"
#include "PAD.H"
#include "STR.H"
#include <LIBETC.H>
#include <LIBSND.H>
#include "FILECODE.H"
#include "MEM_MAP.H"

static CAPTION_REC* pCaption_tbl;                   // current movie subtitle entry table
static USHORT Caption_start;                        // first frame of the current subtitle
static USHORT Caption_end;                          // last frame of the current subtitle
static USHORT Caption_x;                                 // subtitle screen x position
static USHORT Caption_y;                                 // subtitle screen y position
static INT Caption_w;                                // subtitle width
static INT Caption_h;                               // subtitle height
static UCHAR* pCaption_blk;                          // current subtitle text block
static UCHAR* pCaption_top;                          // base of the loaded subtitle data
static UCHAR* pCaption_buf;                           // work buffer the subtitle scanlines are unpacked into
static USHORT Caption_done_flg;                              // 1 when the subtitles of the movie are finished
static INT Movie_task_pad[1];                         // padding: keeps the following data at its linked position (the 4-byte bss of monitor.Obj is padded to 8 by the linker)
static USHORT Movie_xa_id;                               // id of the movie whose XA audio was stopped to skip frames, 0 when none

VOID Movie_play(UINT Movie_no, INT Work_top)
{
    extern MOVIE_TBL Movie_tbl[];

    RECT Rect;
    INT unused[2];
    INT done;
    INT mode;
    UINT Frame;
    INT buf;
    INT small;

    Set_movie_volume2(0x80);

    Cd_set_stereo((G.SaveData.Key_idx >> 7) ^ 1);
    {
        INT is_small = Movie_no - 2 < 13;
        small = is_small;
    }
    SsSetSerialAttr(0, 0, 1);  // call through a cast: the declaration has no parameters

    mode = 0;

retry:
    if (Open_movie(Movie_tbl[Movie_no].Filecode, 2, Movie_tbl[Movie_no].N_frame, (ULONG*)Work_top, Movie_tbl[Movie_no].ring_sectors,
            (ULONG*)(Work_top + (Movie_tbl[Movie_no].ring_sectors << 11)),
            (ULONG*)(Work_top + (Movie_tbl[Movie_no].ring_sectors << 11) + (Movie_tbl[Movie_no].Rl_size >> 2) * 4),
            (VOID*)((ULONG*)(Work_top + (Movie_tbl[Movie_no].ring_sectors << 11) + (Movie_tbl[Movie_no].Rl_size >> 2) * 4) + (Movie_tbl[Movie_no].Rl_size >> 2)),
            Movie_tbl[Movie_no].flags) == 1)
    {
        Close_movie();
        goto retry;
    }
    if (small)
    {
        GsInitGraph2(0x100, 0xF0, 0, 0, 1);
    }

    do
    {
        done = Play_movie(mode, -(GsGetActiveBuff() != 0) & 0xF0);

        Str_decode_wait();
        VSync(0);
        Frame = Str_frm_get();

        if (Frame - 0x97E < 10 && (G.SaveData.Pl_id & 3) == 1 && Movie_no == 1)
        {
            mode = 0x400;

            Movie_xa_stop();
            Movie_xa_id = Movie_no;

            Fwd_movie(0xA15);
        }

        if (Frame >= 0xA16 && (G.SaveData.Pl_id & 3) == 1 && Movie_no == 1 && Movie_xa_id == Movie_no)
        {
            Movie_xa_start();
            Movie_xa_id = 0;
        }

        if (Frame >= 0xA1E && (G.SaveData.Pl_id & 3) == 1 && Movie_no == 1 && (mode & 0x400))
        {
            mode = 0x800;
        }

        VSync(0);

        GsSwapDispBuff();

        Pad_set();

        if (Movie_no < 14)
        {
            if (G.Padd1_trg & Movie_tbl[Movie_no].Cancel)
            {
                done = 1;
            }
        }
        else if ((G.Padd1_trg & Movie_tbl[Movie_no].Cancel) && Frame >= 0x4C)
        {
            done = 1;
        }
    } while (done == 0);

    if (Movie_no == 0)
    {
        Close_movie();

        VSync(0);
        buf = GsGetActiveBuff();

        GsInitGraph2(0x140, 0xF0, 0, 0, 0);

        GsInit3D();
        VSync(0);
        Rect.x = 0;
        if (buf != 0)
        {
            Rect.y = 0xF0;
        }
        else
        {
            Rect.y = 0;
        }
        Rect.w = 0x140;
        Rect.h = 0xF0;
        LoadImage(&Rect, (LOAD_BUF));

        DrawSync(0);
        VSync(0);
        GsSwapDispBuff();
        buf = GsGetActiveBuff();
        Rect.x = 0;
        if (buf != 0)
        {
            Rect.y = 0xF0;
        }
        else
        {
            Rect.y = 0;
        }
        Rect.w = 0x140;
        Rect.h = 0xF0;
        LoadImage(&Rect, (LOAD_BUF));
        DrawSync(0);
    }
    else if (Movie_no - 2 < 12)
    {
        Close_movie();

        VSync(0);
        Bg_draw();
        buf = GsGetActiveBuff();
        GsInitGraph2(0x140, 0xF0, 0, 0, 0);
        GsInit3D();
        VSync(0);
        Rect.x = 0;
        if (buf != 0)
        {
            Rect.y = 0xF0;
        }
        else
        {
            Rect.y = 0;
        }
        Rect.w = 0x140;
        Rect.h = 0xF0;

        if (Movie_no != 2)
        {
            ClearImage(&Rect, 0, 0, 0);
        }
        else
        {
            ClearImage(&Rect, 0xFF, 0xFF, 0xFF);
        }

        DrawSync(0);
        VSync(0);
        GsSwapDispBuff();
        buf = GsGetActiveBuff();
        Rect.x = 0;
        if (buf != 0)
        {
            Rect.y = 0xF0;
        }
        else
        {
            Rect.y = 0;
        }
        Rect.w = 0x140;
        Rect.h = 0xF0;

        if (Movie_no != 2)
        {
            ClearImage(&Rect, 0, 0, 0);
        }
        else
        {
            ClearImage(&Rect, 0xFF, 0xFF, 0xFF);
        }

        DrawSync(0);

        if (Movie_no != 0)
        {
            Set_view();
        }
    }
    else
    {
        buf = GsGetActiveBuff();
        VSync(0);
        Rect.x = 0;
        if (buf != 0)
        {
            Rect.y = 0xF0;
        }
        else
        {
            Rect.y = 0;
        }
        Rect.w = 0x1E0;
        Rect.h = 0xF0;
        ClearImage(&Rect, 0, 0, 0);
        DrawSync(0);
        VSync(0);
        GsSwapDispBuff();
        buf = GsGetActiveBuff();
        Rect.x = 0;
        if (buf != 0)
        {
            Rect.y = 0xF0;
        }
        else
        {
            Rect.y = 0;
        }
        Rect.w = 0x1E0;
        Rect.h = 0xF0;
        ClearImage(&Rect, 0, 0, 0);
        DrawSync(0);

        Close_movie();
    }
}

// movie table by movie id: disc file, last frame, buffer sizes, flags, skip buttons
MOVIE_TBL Movie_tbl[23] =
{
    { OJ_STR, 180, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },    // 0 //
    { PJ_STR, 3153, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },   // 1 //
    { DMF_STR, 95, 29, 0x00FF, 0x0001, 0x0000E000, 0x0000, 0x0000 },     // 2 //
    { DM3_STR, 190, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },    // 3 //
    { DM4_STR, 195, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },    // 4 //
    { DM1_STR, 95, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },     // 5 //
    { DM6_STR, 215, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },    // 6 //
    { DM7_STR, 91, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },     // 7 //
    { DM8_STR, 150, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },    // 8 //
    { DM2_STR, 95, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },     // 9 //
    { 0, 0, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },        // 10 //
    { DMB_STR, 299, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },    // 11 //
    { DMC_STR, 297, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },    // 12 //
    { DMD_STR, 185, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },    // 13 //
    { DME_STR, 150, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },    // 14 //
    { ED1_STR, 733, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },    // 15 //
    { ED2_STR, 683, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },    // 16 //
    { ED3_STR, 595, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },    // 17 //
    { ED4_STR, 766, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },    // 18 //
    { ED5_STR, 848, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },    // 19 //
    { ED6_STR, 708, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },    // 20 //
    { ED7_STR, 707, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },    // 21 //
    { ED8_STR, 185, 29, 0x00FF, 0x0001, 0x0000E000, 0x09F0, 0x0000 },    // 22 //
};

// subtitle entries of the opening movie
CAPTION_REC Caption_open_tbl[18] =
{
    { 125, 60, 0, 1, 0, 205 },     // 0 //
    { 195, 65, 1, 2, 0, 187 },     // 1 //
    { 260, 97, 3, 2, 0, 187 },     // 2 //
    { 435, 59, 5, 2, 0, 187 },     // 3 //
    { 495, 105, 7, 2, 0, 187 },    // 4 //
    { 615, 40, 9, 1, 0, 196 },     // 5 //
    { 655, 30, 10, 1, 0, 196 },    // 6 //
    { 720, 30, 11, 1, 0, 196 },    // 7 //
    { 982, 53, 12, 2, 0, 187 },    // 8 //
    { 1035, 60, 14, 1, 0, 196 },   // 9 //
    { 1105, 60, 15, 1, 0, 196 },   // 10 //
    { 1342, 30, 16, 1, 0, 196 },   // 11 //
    { 2195, 33, 18, 1, 0, 196 },   // 12 //
    { 2475, 27, 19, 1, 0, 196 },   // 13 //
    { 2670, 55, 20, 1, 0, 196 },   // 14 //
    { 2725, 65, 21, 1, 0, 196 },   // 15 //
    { 2790, 30, 22, 1, 0, 196 },   // 16 //
    { -1, 0, 0, 0, 0, 0 },         // 17 //
};

// subtitle entries of ending A
CAPTION_REC Caption_end_a_tbl[6] =
{
    { 334, 44, 0, 1, 0, 196 },   // 0 //
    { 379, 44, 1, 1, 0, 196 },   // 1 //
    { 424, 44, 2, 1, 0, 196 },   // 2 //
    { 469, 44, 3, 1, 0, 196 },   // 3 //
    { 514, 44, 4, 1, 0, 196 },   // 4 //
    { -1, 0, 0, 0, 0, 0 },       // 5 //
};

// subtitle entries of ending B
CAPTION_REC Caption_end_b_tbl[8] =
{
    { 372, 30, 0, 1, 0, 196 },   // 0 //
    { 408, 44, 1, 1, 0, 196 },   // 1 //
    { 453, 29, 2, 1, 0, 196 },   // 2 //
    { 483, 45, 3, 1, 0, 196 },   // 3 //
    { 555, 30, 4, 1, 0, 196 },   // 4 //
    { 570, 44, 5, 1, 0, 196 },   // 5 //
    { 615, 45, 6, 1, 0, 196 },   // 6 //
    { -1, 0, 0, 0, 0, 0 },       // 7 //
};
