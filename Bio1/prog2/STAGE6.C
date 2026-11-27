/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog2\STAGE6.C
 */

#include "STAGE6.H"
#include "EM00.H"
#include "EM03.H"
#include "EM05.H"
#include "EM06.H"
#include "EM0F.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID main(VOID)
{
    Monitor.Em_move_tbl = enemies_update_functions_tbl;
    G.Pl_move[0] = Em00_Pl_catch;
    G.Pl_move[24] = Pl_atck_multi;
    G.Pl_move[44] = Pldie_Em06;
    Monitor.pBgm_room_tbl = (UCHAR*)Bgm_room_tbl;
    Monitor.pXa_sector = (XA_SECTOR_ENT*)Xa_sector06;

    if (!(G.Status_flg & STAT_ARRANGE))  // flag clear: standard checksum table, set: alternate table
    {
        Monitor.pBs_check_sum = (UCHAR*)Bs_check_sum6;
    }
    else
    {
        Monitor.pBs_check_sum = (UCHAR*)Bs_check_sum_tbl2;
    }

    FUNC(TASK_EXIT)();  // Task_exit
}

// XA voice clip table: USHORT entries, each the start offset of a clip in units of 16 sectors; bit 15 ends a channel's list
UINT Xa_sector06[75] =
{
    0x00160000,   // 0 //
    0x00600050,   // 1 //
    0x00C100A7,   // 2 //
    0x00FF00DB,   // 3 //
    0x814A0122,   // 4 //
    0x0000015E,   // 5 //
    0x005D0026,   // 6 //
    0x810B00CA,   // 7 //
    0x00000155,   // 8 //
    0x00430030,   // 9 //
    0x00C0009E,   // 10 //
    0x010C00DB,   // 11 //
    0x015D813A,   // 12 //
    0x006A0000,   // 13 //
    0x00BE0086,   // 14 //
    0x011680E2,   // 15 //
    0x008E0000,   // 16 //
    0x00C100AC,   // 17 //
    0x010300E2,   // 18 //
    0x015A813D,   // 19 //
    0x002C0000,   // 20 //
    0x00620041,   // 21 //
    0x00AD0082,   // 22 //
    0x00FA00C0,   // 23 //
    0x81430115,   // 24 //
    0x00000166,   // 25 //
    0x002C0013,   // 26 //
    0x0062004F,   // 27 //
    0x00B20088,   // 28 //
    0x00E500D4,   // 29 //
    0x813C00F0,   // 30 //
    0x00000181,   // 31 //
    0x8066004C,   // 32 //
    0x800000B2,   // 33 //
    0x0000018B,   // 34 //
    0x00660029,   // 35 //
    0x00BF009D,   // 36 //
    0x00FC00EE,   // 37 //
    0x0116010B,   // 38 //
    0x81580138,   // 39 //
    0x00000164,   // 40 //
    0x002B0011,   // 41 //
    0x00620044,   // 42 //
    0x0083006A,   // 43 //
    0x009B008D,   // 44 //
    0x00CE00BD,   // 45 //
    0x00F200DF,   // 46 //
    0x012A0102,   // 47 //
    0x0169814B,   // 48 //
    0x000B0000,   // 49 //
    0x0027001A,   // 50 //
    0x0067003C,   // 51 //
    0x009F0093,   // 52 //
    0x00B600AA,   // 53 //
    0x010000F1,   // 54 //
    0x0129010B,   // 55 //
    0x0169813F,   // 56 //
    0x00200000,   // 57 //
    0x00A70081,   // 58 //
    0x00D600C6,   // 59 //
    0x81330106,   // 60 //
    0x00000158,   // 61 //
    0x00350021,   // 62 //
    0x00A10063,   // 63 //
    0x012400CA,   // 64 //
    0x01578141,   // 65 //
    0x00320000,   // 66 //
    0x008D0040,   // 67 //
    0x00B7009F,   // 68 //
    0x00F800D2,   // 69 //
    0x015B8121,   // 70 //
    0x00230000,   // 71 //
    0x007C002C,   // 72 //
    0x016B0092,   // 73 //
    0x026A824C,   // 74 //
};

// expected background image checksum, one byte per camera cut, 8 bytes per room
UINT Bs_check_sum6[58] =
{
    0xC16E3051,   // 0 //
    0x0000D570,   // 1 //
    0xCD01B8A3,   // 2 //
    0x0000A00D,   // 3 //
    0xF2B57762,   // 4 //
    0x00000000,   // 5 //
    0x372EF760,   // 6 //
    0x0036E27C,   // 7 //
    0x016E96ED,   // 8 //
    0xB94B4A96,   // 9 //
    0x2AC8E4FC,   // 10 //
    0x007B950C,   // 11 //
    0x173A291D,   // 12 //
    0x03695E20,   // 13 //
    0x21ED35ED,   // 14 //
    0xFE968E5C,   // 15 //
    0x384D10D4,   // 16 //
    0x00000000,   // 17 //
    0x8C5679B4,   // 18 //
    0x45856D4C,   // 19 //
    0xEFED0E83,   // 20 //
    0x0000005A,   // 21 //
    0xA92704EA,   // 22 //
    0x00000000,   // 23 //
    0x3231227B,   // 24 //
    0x0000C506,   // 25 //
    0x000003E2,   // 26 //
    0x00000000,   // 27 //
    0x82EEB5EF,   // 28 //
    0x00000000,   // 29 //
    0xEF919957,   // 30 //
    0x4FE1E7C8,   // 31 //
    0xC18A68F1,   // 32 //
    0x00000069,   // 33 //
    0x4BA47830,   // 34 //
    0x0000002E,   // 35 //
    0x0000F42E,   // 36 //
    0x00000000,   // 37 //
    0xF4E22E92,   // 38 //
    0x00000000,   // 39 //
    0xB9FC656F,   // 40 //
    0x00000000,   // 41 //
    0x391E7610,   // 42 //
    0x00000045,   // 43 //
    0x002D029C,   // 44 //
    0x00000000,   // 45 //
    0x618F1B84,   // 46 //
    0xE90EA7FC,   // 47 //
    0x0000A440,   // 48 //
    0x00000000,   // 49 //
    0x0FC63DC9,   // 50 //
    0x00000000,   // 51 //
    0xFE4D49D6,   // 52 //
    0x0000006A,   // 53 //
    0x00977651,   // 54 //
    0x00000000,   // 55 //
    0x00A416CB,   // 56 //
    0x00000000,   // 57 //
};

// expected background image checksums used when status flag 0x20000 is set, same layout as Bs_check_sum6
UINT Bs_check_sum_tbl2[58] =
{
    0xC16E3051,   // 0 //
    0x0000D570,   // 1 //
    0x910BF3B6,   // 2 //
    0x0000BC11,   // 3 //
    0xF2B57762,   // 4 //
    0x00000000,   // 5 //
    0x372EF760,   // 6 //
    0x0036E27C,   // 7 //
    0x016E96ED,   // 8 //
    0xB94B4A96,   // 9 //
    0x2AC8E4FC,   // 10 //
    0x007B950C,   // 11 //
    0x173A291D,   // 12 //
    0x03695E20,   // 13 //
    0x7DF59DF7,   // 14 //
    0x007E322D,   // 15 //
    0x384D10D4,   // 16 //
    0x00000000,   // 17 //
    0x8C5679B4,   // 18 //
    0x45856D4C,   // 19 //
    0xEFED0E83,   // 20 //
    0x0000005A,   // 21 //
    0xABD2D88D,   // 22 //
    0x00000000,   // 23 //
    0x3231227B,   // 24 //
    0x0000C506,   // 25 //
    0x000003E2,   // 26 //
    0x00000000,   // 27 //
    0x82EEB5EF,   // 28 //
    0x00000000,   // 29 //
    0xEF919957,   // 30 //
    0x4FE1E7C8,   // 31 //
    0xC18A68F1,   // 32 //
    0x00000069,   // 33 //
    0x4BA47830,   // 34 //
    0x0000002E,   // 35 //
    0x0061809A,   // 36 //
    0x00000000,   // 37 //
    0xBE2EC743,   // 38 //
    0x0000C72F,   // 39 //
    0xB9FC656F,   // 40 //
    0x00000000,   // 41 //
    0x391E7610,   // 42 //
    0x00000045,   // 43 //
    0x002D029C,   // 44 //
    0x00000000,   // 45 //
    0x618F1B84,   // 46 //
    0xE90EA7FC,   // 47 //
    0x0000A440,   // 48 //
    0x00000000,   // 49 //
    0x0FC63DC9,   // 50 //
    0x00000000,   // 51 //
    0x0C3B3D2B,   // 52 //
    0x000039C8,   // 53 //
    0x00977651,   // 54 //
    0x00000000,   // 55 //
    0xB06874C0,   // 56 //
    0x00000000,   // 57 //
};

// enemy update function by enemy id (0 = none)
VOID (*enemies_update_functions_tbl[48])(VOID) =
{
    Em00_Main,          // 0 //
    Em00_Main,          // 1 //
    0x00000000,                   // 2 //
    Em03_Main,     // 3 //
    0x00000000,                   // 4 //
    Em05_Main,            // 5 //
    Em06_Main,          // 6 //
    0x00000000,                   // 7 //
    0x00000000,                   // 8 //
    0x00000000,                   // 9 //
    0x00000000,                   // 10 //
    0x00000000,                   // 11 //
    0x00000000,                   // 12 //
    0x00000000,                   // 13 //
    0x00000000,                   // 14 //
    Em0f_Main,   // 15 //
    0x00000000,                   // 16 //
    Em00_Main,          // 17 //
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
    0xFFFF0A0F,   // 0 //
    0xFFFF1B13,   // 1 //
    0xFFFFFFFF,   // 2 //
    0xFFFFFF18,   // 3 //
    0xFFFFFF18,   // 4 //
    0xFFFFFF18,   // 5 //
    0xFFFFFF18,   // 6 //
    0xFFFFFF18,   // 7 //
    0xFFFFFF18,   // 8 //
    0xFFFFFF02,   // 9 //
    0xFFFFFF02,   // 10 //
    0xFFFF0218,   // 11 //
    0xFFFFFF1E,   // 12 //
    0xFFFFFF1C,   // 13 //
    0xFFFFFF18,   // 14 //
    0xFFFFFFFF,   // 15 //
    0xFFFFFFFF,   // 16 //
    0xFFFFFFFF,   // 17 //
    0xFFFFFFFF,   // 18 //
    0xFFFFFFFF,   // 19 //
    0xFFFFFF02,   // 20 //
    0xFFFFFF03,   // 21 //
    0xFFFFFFFF,   // 22 //
    0xFFFF0505,   // 23 //
    0xFFFFFFFF,   // 24 //
    0xFFFFFF02,   // 25 //
    0xFFFFFF02,   // 26 //
    0xFFFFFF02,   // 27 //
    0xFFFFFFFF,   // 28 //
    0xFFFFFFFF,   // 29 //
    0xFFFFFFFF,   // 30 //
    0xFFFFFFFF,   // 31 //
};
