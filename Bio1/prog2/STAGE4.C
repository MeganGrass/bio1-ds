/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog2\STAGE4.C
 */

#include "STAGE4.H"
#include "EM00.H"
#include "EM03.H"
#include "EM07.H"
#include "EM08.H"
#include "EM0B.H"
#include "EM0E.H"
#include "EM0F.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID main(VOID)
{
    Monitor.Em_move_tbl = enemies_update_functions_tbl;
    G.Pl_move[0] = Em00_Pl_catch;
    G.Pl_move[7] = Pl_mv_crawl;
    G.Pl_move[27] = Em08_Pl_dm;
    G.Pl_move[46] = Pl_anim_limb_bounce;
    G.Pl_move[30] = Pl_poison_die;
    G.Pl_move[49] = Pl_devour_die;
    G.Pl_move[34] = Pl_Em0f_hold;
    Monitor.pBgm_room_tbl = (UCHAR*)Bgm_room_tbl;
    Monitor.pXa_sector = (XA_SECTOR_ENT*)Xa_sector04;

    if (!(G.Status_flg & STAT_ARRANGE))  // flag clear: standard checksum table, set: alternate table
    {
        Monitor.pBs_check_sum = (UCHAR*)Bs_check_sum4;
    }
    else
    {
        Monitor.pBs_check_sum = (UCHAR*)Bs_check_sum_tbl2;
    }

    FUNC(TASK_EXIT)();  // Task_exit
}

// XA voice clip table: USHORT entries, each the start offset of a clip in units of 16 sectors; bit 15 ends a channel's list
UINT Xa_sector04[43] =
{
    0x00F68000,   // 0 //
    0x000F0000,   // 1 //
    0x80700030,   // 2 //
    0x000000BF,   // 3 //
    0x00B28074,   // 4 //
    0x00260000,   // 5 //
    0x00380030,   // 6 //
    0x0097808D,   // 7 //
    0x00640000,   // 8 //
    0x00AC807E,   // 9 //
    0x00450000,   // 10 //
    0x006F005D,   // 11 //
    0x80A00091,   // 12 //
    0x000000DF,   // 13 //
    0x003C0021,   // 14 //
    0x00AB8094,   // 15 //
    0x003B0000,   // 16 //
    0x00950076,   // 17 //
    0x00CC80BA,   // 18 //
    0x000A0000,   // 19 //
    0x80670024,   // 20 //
    0x000000D4,   // 21 //
    0x0012000A,   // 22 //
    0x00380025,   // 23 //
    0x00A48051,   // 24 //
    0x80590000,   // 25 //
    0x000000BA,   // 26 //
    0x006F0065,   // 27 //
    0x0085007A,   // 28 //
    0x00D6809D,   // 29 //
    0x005A0000,   // 30 //
    0x00A98080,   // 31 //
    0x00380000,   // 32 //
    0x00780058,   // 33 //
    0x00CE8087,   // 34 //
    0x00290000,   // 35 //
    0x004B0040,   // 36 //
    0x00BB8095,   // 37 //
    0x001F0000,   // 38 //
    0x005F002F,   // 39 //
    0x00970072,   // 40 //
    0x00CC00B8,   // 41 //
    0x011080FA,   // 42 //
};

// expected background image checksum, one byte per camera cut, 8 bytes per room
UINT Bs_check_sum4[36] =
{
    0xE4DD3572,   // 0 //
    0x0000A55F,   // 1 //
    0xCA964ACA,   // 2 //
    0x1D587FCE,   // 3 //
    0x0000F29A,   // 4 //
    0x00000000,   // 5 //
    0x009373B0,   // 6 //
    0x00000000,   // 7 //
    0xB4F75073,   // 8 //
    0x6D5B9852,   // 9 //
    0xA9FDC208,   // 10 //
    0xA3E3DBD6,   // 11 //
    0xB3B361D5,   // 12 //
    0xF356E9CD,   // 13 //
    0x0000B1CC,   // 14 //
    0x00000000,   // 15 //
    0x287CAF59,   // 16 //
    0x3876AEBB,   // 17 //
    0x00CDD64C,   // 18 //
    0x00000000,   // 19 //
    0xC8A167A6,   // 20 //
    0x0071AF4E,   // 21 //
    0x000015A7,   // 22 //
    0x00000000,   // 23 //
    0x51D28B50,   // 24 //
    0x309F8043,   // 25 //
    0x2743C703,   // 26 //
    0x3D279AC6,   // 27 //
    0x72A8D2B5,   // 28 //
    0x00000206,   // 29 //
    0x6DB19AD8,   // 30 //
    0x000000F1,   // 31 //
    0xCD0DEC01,   // 32 //
    0x00000000,   // 33 //
    0x36F68A63,   // 34 //
    0x0000BE14,   // 35 //
};

// expected background image checksums used when status flag 0x20000 is set, same layout as Bs_check_sum4
UINT Bs_check_sum_tbl2[36] =
{
    0xE01B06A7,   // 0 //
    0x00CCC0E3,   // 1 //
    0xCA964ACA,   // 2 //
    0x1D587FCE,   // 3 //
    0x0000F29A,   // 4 //
    0x00000000,   // 5 //
    0x009373B0,   // 6 //
    0x00000000,   // 7 //
    0xE9378D0C,   // 8 //
    0x002B7004,   // 9 //
    0xA9FDC208,   // 10 //
    0xA3E3DBD6,   // 11 //
    0xB3B361D5,   // 12 //
    0xF356E9CD,   // 13 //
    0x0000B1CC,   // 14 //
    0x00000000,   // 15 //
    0x287CAF59,   // 16 //
    0x3876AEBB,   // 17 //
    0x00CDD64C,   // 18 //
    0x00000000,   // 19 //
    0xC8A167A6,   // 20 //
    0x0071AF4E,   // 21 //
    0x000015A7,   // 22 //
    0x00000000,   // 23 //
    0x51D28B50,   // 24 //
    0x309F8043,   // 25 //
    0x2743C703,   // 26 //
    0x3D279AC6,   // 27 //
    0x72A8D2B5,   // 28 //
    0x00000206,   // 29 //
    0x6DB19AD8,   // 30 //
    0x000000F1,   // 31 //
    0xCD0DEC01,   // 32 //
    0x00000000,   // 33 //
    0x36F68A63,   // 34 //
    0x0000BE14,   // 35 //
};

// enemy update function by enemy id (0 = none)
VOID (*enemies_update_functions_tbl[48])(VOID) =
{
    Em00_Main,          // 0 //
    Em00_Main,          // 1 //
    0x00000000,                   // 2 //
    Em03_Main,     // 3 //
    0x00000000,                   // 4 //
    0x00000000,                   // 5 //
    0x00000000,                   // 6 //
    Em07_Main,            // 7 //
    Em08_Main,         // 8 //
    0x00000000,                   // 9 //
    0x00000000,                   // 10 //
    Em0b_Main,         // 11 //
    0x00000000,                   // 12 //
    0x00000000,                   // 13 //
    Em0e_Main,   // 14 //
    Em0f_Main,   // 15 //
    0x00000000,                   // 16 //
    0x00000000,                   // 17 //
    0x00000000,                   // 18 //
    0x00000000,                   // 19 //
    0x00000000,                   // 20 //
    0x00000000,                   // 21 //
    0x00000000,                   // 22 //
    0x00000000,                   // 23 //
    0x00000000,                   // 24 //
    0x00000000,                   // 25 //
    0x00000000,                   // 26 //
    0x00000000,                   // 27 //
    0x00000000,                   // 28 //
    0x00000000,                   // 29 //
    0x00000000,                   // 30 //
    0x00000000,                   // 31 //
    0x00000000,                   // 32 //
    0x00000000,                   // 33 //
    0x00000000,                   // 34 //
    0x00000000,                   // 35 //
    0x00000000,                   // 36 //
    0x00000000,                   // 37 //
    0x00000000,                   // 38 //
    0x00000000,                   // 39 //
    0x00000000,                   // 40 //
    0x00000000,                   // 41 //
    0x00000000,                   // 42 //
    0x00000000,                   // 43 //
    0x00000000,                   // 44 //
    0x00000000,                   // 45 //
    0x00000000,                   // 46 //
    0x00000000,                   // 47 //
};

// BGM id per room, 4 bytes per room (0xFF = none)
UINT Bgm_room_tbl[32] =
{
    0xFFFFFF2B,   // 0 //
    0xFFFFFF0C,   // 1 //
    0xFFFFFF0C,   // 2 //
    0xFFFFFF0F,   // 3 //
    0xFFFFFF2B,   // 4 //
    0xFFFF2C2D,   // 5 //
    0xFFFF2E0C,   // 6 //
    0xFFFFFF0C,   // 7 //
    0xFFFFFF2B,   // 8 //
    0xFFFFFF1F,   // 9 //
    0xFFFFFF0C,   // 10 //
    0xFFFFFF0C,   // 11 //
    0xFFFF2827,   // 12 //
    0xFFFFFF29,   // 13 //
    0xFFFFFF29,   // 14 //
    0xFFFFFF0C,   // 15 //
    0xFFFFFF29,   // 16 //
    0xFFFFFF29,   // 17 //
    0xFFFFFFFF,   // 18 //
    0xFFFFFFFF,   // 19 //
    0xFFFFFFFF,   // 20 //
    0xFFFFFFFF,   // 21 //
    0xFFFFFFFF,   // 22 //
    0xFFFFFFFF,   // 23 //
    0xFFFFFFFF,   // 24 //
    0xFFFFFFFF,   // 25 //
    0xFFFFFFFF,   // 26 //
    0xFFFFFFFF,   // 27 //
    0xFFFFFFFF,   // 28 //
    0xFFFFFFFF,   // 29 //
    0xFFFFFFFF,   // 30 //
    0xFFFFFFFF,   // 31 //
};
