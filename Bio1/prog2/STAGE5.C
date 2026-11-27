/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog2\STAGE5.C
 */

#include "STAGE5.H"
#include "EM00.H"
#include "EM09.H"
#include "EM0C.H"
#include "EM14.H"
#include "EM15.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID main(VOID)
{
    Monitor.Em_move_tbl = enemies_update_functions_tbl;
    G.Pl_move[0] = Em00_Pl_catch;
    G.Pl_move[28] = Em09_pldm00;
    G.Pl_move[31] = Pldamege_Em0c;
    G.Pl_move[50] = Pldie_Em0c;
    Monitor.pBgm_room_tbl = (UCHAR*)Bgm_room_tbl;
    Monitor.pXa_sector = (XA_SECTOR_ENT*)Xa_sector05;

    if (!(G.Status_flg & STAT_ARRANGE))  // flag clear: standard checksum table, set: alternate table
    {
        Monitor.pBs_check_sum = (UCHAR*)Bs_check_sum5;
    }
    else
    {
        Monitor.pBs_check_sum = (UCHAR*)Bs_check_sum_tbl2;
    }

    FUNC(TASK_EXIT)();  // Task_exit
}

// XA voice clip table: USHORT entries, each the start offset of a clip in units of 16 sectors; bit 15 ends a channel's list
UINT Xa_sector05[102] =
{
    0x00080000,   // 0 //
    0x00670043,   // 1 //
    0x00A6009C,   // 2 //
    0x00C200B4,   // 3 //
    0x011200EC,   // 4 //
    0x0157013B,   // 5 //
    0x01740162,   // 6 //
    0x01D301A4,   // 7 //
    0x01FC01F1,   // 8 //
    0x020D8204,   // 9 //
    0x00610000,   // 10 //
    0x816100BF,   // 11 //
    0x000001C8,   // 12 //
    0x00C90077,   // 13 //
    0x81830137,   // 14 //
    0x00000227,   // 15 //
    0x00610029,   // 16 //
    0x009E006D,   // 17 //
    0x01650100,   // 18 //
    0x01C801A0,   // 19 //
    0x01E901D3,   // 20 //
    0x021781F3,   // 21 //
    0x00140000,   // 22 //
    0x00DF0025,   // 23 //
    0x0129010A,   // 24 //
    0x01680157,   // 25 //
    0x819D017F,   // 26 //
    0x000001F2,   // 27 //
    0x00960052,   // 28 //
    0x00BA00B0,   // 29 //
    0x00ED00E4,   // 30 //
    0x014D0131,   // 31 //
    0x018C0176,   // 32 //
    0x01C801AC,   // 33 //
    0x81ED01D6,   // 34 //
    0x00000211,   // 35 //
    0x007B0022,   // 36 //
    0x00F500E8,   // 37 //
    0x01170106,   // 38 //
    0x015D0127,   // 39 //
    0x01B10193,   // 40 //
    0x023081FB,   // 41 //
    0x00220000,   // 42 //
    0x0052002E,   // 43 //
    0x00C9006B,   // 44 //
    0x00FF00F4,   // 45 //
    0x01AC812A,   // 46 //
    0x01110000,   // 47 //
    0x01450125,   // 48 //
    0x01630152,   // 49 //
    0x01800174,   // 50 //
    0x01A60190,   // 51 //
    0x020981F0,   // 52 //
    0x00370000,   // 53 //
    0x00B70050,   // 54 //
    0x011A00EC,   // 55 //
    0x01B20198,   // 56 //
    0x81E301BB,   // 57 //
    0x00000218,   // 58 //
    0x0097007B,   // 59 //
    0x010000EB,   // 60 //
    0x017F0164,   // 61 //
    0x01B601A4,   // 62 //
    0x81CF01C0,   // 63 //
    0x0000023D,   // 64 //
    0x00610028,   // 65 //
    0x00AD0068,   // 66 //
    0x01250101,   // 67 //
    0x01730141,   // 68 //
    0x019D018E,   // 69 //
    0x01D901B1,   // 70 //
    0x01FC81E7,   // 71 //
    0x00700000,   // 72 //
    0x00D100A2,   // 73 //
    0x0176015C,   // 74 //
    0x81D801C5,   // 75 //
    0x00000228,   // 76 //
    0x0044001E,   // 77 //
    0x007B0051,   // 78 //
    0x00A0008A,   // 79 //
    0x00ED00DB,   // 80 //
    0x015B00FB,   // 81 //
    0x01AF0186,   // 82 //
    0x01E701E0,   // 83 //
    0x020E81FD,   // 84 //
    0x00220000,   // 85 //
    0x00810047,   // 86 //
    0x00C6009A,   // 87 //
    0x010C00EF,   // 88 //
    0x014B0123,   // 89 //
    0x017D0163,   // 90 //
    0x81D7018E,   // 91 //
    0x000001EA,   // 92 //
    0x006F005E,   // 93 //
    0x011000EB,   // 94 //
    0x01690149,   // 95 //
    0x01AF0197,   // 96 //
    0x020501DE,   // 97 //
    0x026B0263,   // 98 //
    0x02C30275,   // 99 //
    0x82D302C9,   // 100 //
    0x000002F9,   // 101 //
};

// expected background image checksum, one byte per camera cut, 8 bytes per room
UINT Bs_check_sum5[44] =
{
    0x3DE682CE,   // 0 //
    0x007A5218,   // 1 //
    0x40F0A340,   // 2 //
    0xC47EFB45,   // 3 //
    0x6C0C7BBF,   // 4 //
    0x00000000,   // 5 //
    0x1B18CBD1,   // 6 //
    0x00003C3E,   // 7 //
    0xD4619757,   // 8 //
    0x003B46FB,   // 9 //
    0xC8F2B702,   // 10 //
    0x393F9BA1,   // 11 //
    0x0CF9198F,   // 12 //
    0x00007E4C,   // 13 //
    0xCD62AAC1,   // 14 //
    0x08825E55,   // 15 //
    0xC996408D,   // 16 //
    0x00000039,   // 17 //
    0xA4AAEA18,   // 18 //
    0x00003198,   // 19 //
    0x2362B44B,   // 20 //
    0xAC8B2D57,   // 21 //
    0x5692D9CC,   // 22 //
    0x00000000,   // 23 //
    0xD2F57E2A,   // 24 //
    0x6BFF317C,   // 25 //
    0x000000BC,   // 26 //
    0x00000000,   // 27 //
    0x0000E616,   // 28 //
    0x00000000,   // 29 //
    0xC8CDF9BC,   // 30 //
    0xFF77052F,   // 31 //
    0xE7ED8AF5,   // 32 //
    0x48E270BE,   // 33 //
    0x792455C2,   // 34 //
    0x0000EC70,   // 35 //
    0xE5F04C56,   // 36 //
    0x007A524B,   // 37 //
    0x6B420FA3,   // 38 //
    0x2824EBF5,   // 39 //
    0xCD470547,   // 40 //
    0x00E20E84,   // 41 //
    0x0000000B,   // 42 //
    0x00000000,   // 43 //
};

// expected background image checksums used when status flag 0x20000 is set, same layout as Bs_check_sum5
UINT Bs_check_sum_tbl2[44] =
{
    0x3DE682CE,   // 0 //
    0x007A5218,   // 1 //
    0x40F0A340,   // 2 //
    0xC47EFB45,   // 3 //
    0x6C0C7BBF,   // 4 //
    0x00000000,   // 5 //
    0x1B18CBD1,   // 6 //
    0x00003C3E,   // 7 //
    0xD4619757,   // 8 //
    0x003B46FB,   // 9 //
    0xA33DEFDF,   // 10 //
    0x0EE1C5A4,   // 11 //
    0x0CF9198F,   // 12 //
    0x00007E4C,   // 13 //
    0xDB0322B6,   // 14 //
    0x0018C4DD,   // 15 //
    0xC996408D,   // 16 //
    0x00000039,   // 17 //
    0xA4AAEA18,   // 18 //
    0x00003198,   // 19 //
    0x2362B44B,   // 20 //
    0xAC8B2D57,   // 21 //
    0x5692D9CC,   // 22 //
    0x00000000,   // 23 //
    0xD2F57E2A,   // 24 //
    0x6BFF317C,   // 25 //
    0x000000BC,   // 26 //
    0x00000000,   // 27 //
    0x0000E616,   // 28 //
    0x00000000,   // 29 //
    0xC8CDF9BC,   // 30 //
    0xFF77052F,   // 31 //
    0xE7ED8AF5,   // 32 //
    0x48E270BE,   // 33 //
    0x7A9EC3CC,   // 34 //
    0x0000CB2B,   // 35 //
    0xE5F04C56,   // 36 //
    0x007A524B,   // 37 //
    0x6B420FA3,   // 38 //
    0x2824EBF5,   // 39 //
    0xCD470547,   // 40 //
    0x00E20E84,   // 41 //
    0x0000000B,   // 42 //
    0x00000000,   // 43 //
};

// enemy update function by enemy id (0 = none)
VOID (*enemies_update_functions_tbl[48])(VOID) =
{
    Em00_Main,               // 0 //
    Em00_Main,               // 1 //
    0x00000000,                        // 2 //
    0x00000000,                        // 3 //
    0x00000000,                        // 4 //
    0x00000000,                        // 5 //
    0x00000000,                        // 6 //
    0x00000000,                        // 7 //
    0x00000000,                        // 8 //
    Em09_Main,              // 9 //
    0x00000000,                        // 10 //
    0x00000000,                        // 11 //
    Em0c_Main,               // 12 //
    0x00000000,                        // 13 //
    0x00000000,                        // 14 //
    0x00000000,                        // 15 //
    0x00000000,                        // 16 //
    Em00_Main,               // 17 //
    0x00000000,                        // 18 //
    0x00000000,                        // 19 //
    Em14_Main,   // 20 //
    Em15_Main,    // 21 //
    0x00000000,                        // 22 //
    0x00000000,                        // 23 //
    0x00000000,                        // 24 //
    0x00000000,                        // 25 //
    0x00000000,                        // 26 //
    0x00000000,                        // 27 //
    0x00000000,                        // 28 //
    0x00000000,                        // 29 //
    0x00000000,                        // 30 //
    0x00000000,                        // 31 //
    0x00000000,                        // 32 //
    0x00000000,                        // 33 //
    0x00000000,                        // 34 //
    0x00000000,                        // 35 //
    0x00000000,                        // 36 //
    0x00000000,                        // 37 //
    0x00000000,                        // 38 //
    0x00000000,                        // 39 //
    0x00000000,                        // 40 //
    0x00000000,                        // 41 //
    0x00000000,                        // 42 //
    0x00000000,                        // 43 //
    0x00000000,                        // 44 //
    0x00000000,                        // 45 //
    0x00000000,                        // 46 //
    0x00000000,                        // 47 //
};

// BGM id per room, 4 bytes per room (0xFF = none)
UINT Bgm_room_tbl[32] =
{
    0xFFFF3536,   // 0 //
    0xFFFFFF35,   // 1 //
    0xFF263536,   // 2 //
    0xFFFF3536,   // 3 //
    0xFFFF3536,   // 4 //
    0xFFFF352F,   // 5 //
    0xFFFF352F,   // 6 //
    0xFFFF352F,   // 7 //
    0xFFFF352F,   // 8 //
    0xFFFF352F,   // 9 //
    0xFFFF352F,   // 10 //
    0xFFFF3530,   // 11 //
    0xFFFF352F,   // 12 //
    0xFFFFFF35,   // 13 //
    0xFFFF350F,   // 14 //
    0xFFFF3531,   // 15 //
    0xFFFF3531,   // 16 //
    0xFFFF3531,   // 17 //
    0xFFFF3530,   // 18 //
    0xFFFF3534,   // 19 //
    0xFF353332,   // 20 //
    0xFFFFFF35,   // 21 //
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
