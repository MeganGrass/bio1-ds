/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog2\STAGE2.C
 */

#include "STAGE2.H"
#include "EM00.H"
#include "EM05.H"
#include "EM0D.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID main(VOID)
{
    Monitor.Em_move_tbl = enemies_update_functions_tbl;
    G.Pl_move[0] = Em00_Pl_catch;
    G.Pl_move[24] = Pl_atck_multi;
    G.Pl_move[51] = Em0d_pldie;
    Monitor.pBgm_room_tbl = (UCHAR*)Bgm_room_tbl;
    Monitor.pXa_sector = (XA_SECTOR_ENT*)Xa_sector02;

    if (!(G.Status_flg & STAT_ARRANGE))  // flag clear: standard checksum table, set: alternate table
    {
        Monitor.pBs_check_sum = (UCHAR*)Bs_check_sum2;
    }
    else
    {
        Monitor.pBs_check_sum = (UCHAR*)Bs_check_sum_tbl2;
    }

    FUNC(TASK_EXIT)();  // Task_exit
}

// XA voice clip table: USHORT entries, each the start offset of a clip in units of 16 sectors; bit 15 ends a channel's list
UINT Xa_sector02[55] =
{
    0x000A0000,   // 0 //
    0x00560027,   // 1 //
    0x80B4008C,   // 2 //
    0x000000C5,   // 3 //
    0x005F0047,   // 4 //
    0x00A4006A,   // 5 //
    0x80D200B3,   // 6 //
    0x000000DE,   // 7 //
    0x0057002B,   // 8 //
    0x00860062,   // 9 //
    0x80B20096,   // 10 //
    0x000000DA,   // 11 //
    0x00560025,   // 12 //
    0x008F0067,   // 13 //
    0x00D580C8,   // 14 //
    0x00270000,   // 15 //
    0x00400036,   // 16 //
    0x0062005A,   // 17 //
    0x80B70085,   // 18 //
    0x000000D2,   // 19 //
    0x00EF8016,   // 20 //
    0x00E18000,   // 21 //
    0x000C0000,   // 22 //
    0x008D0047,   // 23 //
    0x00DA80A0,   // 24 //
    0x002E0000,   // 25 //
    0x00680058,   // 26 //
    0x80960076,   // 27 //
    0x000000B0,   // 28 //
    0x00640037,   // 29 //
    0x80B3008A,   // 30 //
    0x00000101,   // 31 //
    0x005D001F,   // 32 //
    0x00850075,   // 33 //
    0x00AD8095,   // 34 //
    0x00420000,   // 35 //
    0x009A0066,   // 36 //
    0x00D680C5,   // 37 //
    0x00240000,   // 38 //
    0x004D0041,   // 39 //
    0x80BB007F,   // 40 //
    0x000000CF,   // 41 //
    0x003E002E,   // 42 //
    0x00710058,   // 43 //
    0x00B90096,   // 44 //
    0x00EE80BF,   // 45 //
    0x00070000,   // 46 //
    0x00390026,   // 47 //
    0x007E006C,   // 48 //
    0x80B800A9,   // 49 //
    0x000000EF,   // 50 //
    0x00400026,   // 51 //
    0x006F0056,   // 52 //
    0x00A70092,   // 53 //
    0x010180E3,   // 54 //
};

// expected background image checksum, one byte per camera cut, 8 bytes per room
UINT Bs_check_sum2[58] =
{
    0x6CF553EE,   // 0 //
    0x00000066,   // 1 //
    0x0D0B4318,   // 2 //
    0x091C79FB,   // 3 //
    0x16505CC1,   // 4 //
    0x000029A3,   // 5 //
    0x6B5F5407,   // 6 //
    0x00000084,   // 7 //
    0x0E632517,   // 8 //
    0x00000047,   // 9 //
    0xE6ED755B,   // 10 //
    0x00000000,   // 11 //
    0x002629D4,   // 12 //
    0xDE0F4295,   // 13 //
    0x851DE91E,   // 14 //
    0x00A610A0,   // 15 //
    0x0000AAE2,   // 16 //
    0x00000000,   // 17 //
    0x00C932AA,   // 18 //
    0x00000000,   // 19 //
    0x462F6C73,   // 20 //
    0x00003E13,   // 21 //
    0x00BCFC7E,   // 22 //
    0x00000000,   // 23 //
    0xA9A4940F,   // 24 //
    0x2A6C1773,   // 25 //
    0xD2D09325,   // 26 //
    0x00007F5C,   // 27 //
    0xF97FDD14,   // 28 //
    0x00000079,   // 29 //
    0xBE996C5F,   // 30 //
    0x0000F899,   // 31 //
    0x4487709C,   // 32 //
    0x00D481A1,   // 33 //
    0x001B9DF9,   // 34 //
    0x00000000,   // 35 //
    0x38F39066,   // 36 //
    0x000000D4,   // 37 //
    0x985B16C3,   // 38 //
    0x003D1306,   // 39 //
    0x49B7568A,   // 40 //
    0x000000A5,   // 41 //
    0xB2E69728,   // 42 //
    0x79D26797,   // 43 //
    0xB45BA4C0,   // 44 //
    0x00BD44A7,   // 45 //
    0x437E6C79,   // 46 //
    0x5B16353E,   // 47 //
    0x00D3C308,   // 48 //
    0x00000000,   // 49 //
    0x0037105B,   // 50 //
    0x00000000,   // 51 //
    0xEFD4F491,   // 52 //
    0x0000ABDF,   // 53 //
    0x58A0B4A9,   // 54 //
    0x00006654,   // 55 //
    0x643A8855,   // 56 //
    0x00000019,   // 57 //
};

// expected background image checksums used when status flag 0x20000 is set, same layout as Bs_check_sum2
UINT Bs_check_sum_tbl2[58] =
{
    0x6CF553EE,   // 0 //
    0x00000066,   // 1 //
    0x571CEF3A,   // 2 //
    0x1DE210A7,   // 3 //
    0x7289F790,   // 4 //
    0x0000005D,   // 5 //
    0x03E10267,   // 6 //
    0x0000F661,   // 7 //
    0x3CB16386,   // 8 //
    0x000091C2,   // 9 //
    0xE6ED755B,   // 10 //
    0x00000000,   // 11 //
    0x002629D4,   // 12 //
    0xDE0F4295,   // 13 //
    0x851DE91E,   // 14 //
    0x00A610A0,   // 15 //
    0x0000AAE2,   // 16 //
    0x00000000,   // 17 //
    0x00C932AA,   // 18 //
    0x00000000,   // 19 //
    0x462F6C73,   // 20 //
    0x00003E13,   // 21 //
    0x00BCFC7E,   // 22 //
    0x00000000,   // 23 //
    0xA9A4940F,   // 24 //
    0x2A6C1773,   // 25 //
    0xD2D09325,   // 26 //
    0x00007F5C,   // 27 //
    0xF97FDD14,   // 28 //
    0x00000079,   // 29 //
    0xBE996C5F,   // 30 //
    0x0000F899,   // 31 //
    0x4487709C,   // 32 //
    0x00D481A1,   // 33 //
    0x001B9DF9,   // 34 //
    0x00000000,   // 35 //
    0x9F5D7B81,   // 36 //
    0xB4C25F2E,   // 37 //
    0x985B16C3,   // 38 //
    0x003D1306,   // 39 //
    0x49B7568A,   // 40 //
    0x000000A5,   // 41 //
    0xB2E69728,   // 42 //
    0x79D26797,   // 43 //
    0xB45BA4C0,   // 44 //
    0x00BD44A7,   // 45 //
    0x437E6C79,   // 46 //
    0x5B16353E,   // 47 //
    0xB6C6AF30,   // 48 //
    0x00000000,   // 49 //
    0x0037105B,   // 50 //
    0x00000000,   // 51 //
    0xEFD4F491,   // 52 //
    0x0000ABDF,   // 53 //
    0x58A0B4A9,   // 54 //
    0x00006654,   // 55 //
    0x643A8855,   // 56 //
    0x00000019,   // 57 //
};

// enemy update function by enemy id (0 = none)
VOID (*enemies_update_functions_tbl[48])(VOID) =
{
    Em00_Main,   // 0 //
    Em00_Main,   // 1 //
    0x00000000,            // 2 //
    0x00000000,            // 3 //
    0x00000000,            // 4 //
    Em05_Main,     // 5 //
    0x00000000,            // 6 //
    0x00000000,            // 7 //
    0x00000000,            // 8 //
    0x00000000,            // 9 //
    0x00000000,            // 10 //
    0x00000000,            // 11 //
    0x00000000,            // 12 //
    Em0d_Main,     // 13 //
    0x00000000,            // 14 //
    0x00000000,            // 15 //
    0x00000000,            // 16 //
    Em00_Main,   // 17 //
    Em0d_Main,     // 18 //
    0x00000000,            // 19 //
    0x00000000,            // 20 //
    0x00000000,            // 21 //
    Em00_Main,   // 22 //
    0x00000000,            // 23 //
    0x00000000,            // 24 //
    0x00000000,            // 25 //
    0x00000000,            // 26 //
    0x00000000,            // 27 //
    0x00000000,            // 28 //
    0x00000000,            // 29 //
    0x00000000,            // 30 //
    0x00000000,            // 31 //
    0x00000000,            // 32 //
    0x00000000,            // 33 //
    0x00000000,            // 34 //
    0x00000000,            // 35 //
    0x00000000,            // 36 //
    0x00000000,            // 37 //
    0x00000000,            // 38 //
    0x00000000,            // 39 //
    0x00000000,            // 40 //
    0x00000000,            // 41 //
    0x00000000,            // 42 //
    0x00000000,            // 43 //
    0x00000000,            // 44 //
    0x00000000,            // 45 //
    0x00000000,            // 46 //
    0x00000000,            // 47 //
};

// BGM id per room, 4 bytes per room (0xFF = none)
UINT Bgm_room_tbl[32] =
{
    0xFFFFFF37,   // 0 //
    0xFFFFFF07,   // 1 //
    0xFFFFFF07,   // 2 //
    0xFFFFFF09,   // 3 //
    0xFFFFFF07,   // 4 //
    0xFFFFFF1F,   // 5 //
    0xFFFFFFFF,   // 6 //
    0xFFFFFFFF,   // 7 //
    0xFFFFFF07,   // 8 //
    0xFFFFFF07,   // 9 //
    0xFFFF0711,   // 10 //
    0xFFFFFF04,   // 11 //
    0xFFFFFF0C,   // 12 //
    0x07171610,   // 13 //
    0xFF0F1514,   // 14 //
    0xFFFFFFFF,   // 15 //
    0xFFFFFF06,   // 16 //
    0xFFFFFF08,   // 17 //
    0xFFFFFF08,   // 18 //
    0xFFFFFF07,   // 19 //
    0xFFFFFF07,   // 20 //
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
