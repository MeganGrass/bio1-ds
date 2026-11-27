/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog2\STAGE3.C
 */

#include "STAGE3.H"
#include "EM02.H"
#include "EM04.H"
#include "EM06.H"
#include "EM0A.H"
#include "EM0C.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID main(VOID)
{
    Monitor.Em_move_tbl = enemies_update_functions_tbl;
    G.Pl_move[40] = Em02_pl_rec;
    G.Pl_move[44] = Pldie_Em06;
    G.Pl_move[31] = Pldamege_Em0c;
    G.Pl_move[50] = Pldie_Em0c;
    Monitor.pBgm_room_tbl = (UCHAR*)Bgm_room_tbl;
    Monitor.pXa_sector = (XA_SECTOR_ENT*)Xa_sector03;

    if (!(G.Status_flg & STAT_ARRANGE))  // flag clear: standard checksum table, set: alternate table
    {
        Monitor.pBs_check_sum = (UCHAR*)Bs_check_sum3;
    }
    else
    {
        Monitor.pBs_check_sum = (UCHAR*)Bs_check_sum_tbl2;
    }

    FUNC(TASK_EXIT)();  // Task_exit
}

// XA voice clip table: USHORT entries, each the start offset of a clip in units of 16 sectors; bit 15 ends a channel's list
UINT Xa_sector03[33] =
{
    0x00170000,   // 0 //
    0x8075004D,   // 1 //
    0x00000097,   // 2 //
    0x00600038,   // 3 //
    0x00CA8090,   // 4 //
    0x00360000,   // 5 //
    0x00C78073,   // 6 //
    0x00170000,   // 7 //
    0x005A0032,   // 8 //
    0x80B00093,   // 9 //
    0x000000C4,   // 10 //
    0x00420025,   // 11 //
    0x00BC806D,   // 12 //
    0x00190000,   // 13 //
    0x00BB8084,   // 14 //
    0x00310000,   // 15 //
    0x00B7808A,   // 16 //
    0x005F0000,   // 17 //
    0x00E9808F,   // 18 //
    0x00520000,   // 19 //
    0x00C1805D,   // 20 //
    0x00100000,   // 21 //
    0x807D0053,   // 22 //
    0x000000CF,   // 23 //
    0x00770013,   // 24 //
    0x00D0808D,   // 25 //
    0x00330000,   // 26 //
    0x80B0006A,   // 27 //
    0x800000C5,   // 28 //
    0x80000050,   // 29 //
    0x0000026C,   // 30 //
    0x00738011,   // 31 //
    0x00108000,   // 32 //
};

// expected background image checksum, one byte per camera cut, 8 bytes per room
UINT Bs_check_sum3[36] =
{
    0x01681E06,   // 0 //
    0x0085D076,   // 1 //
    0xF41C04FB,   // 2 //
    0xDD3F95FE,   // 3 //
    0x2EDBF62A,   // 4 //
    0xB3AD8350,   // 5 //
    0x87EC433A,   // 6 //
    0x5FDC3DEE,   // 7 //
    0xF119F513,   // 8 //
    0x0000215F,   // 9 //
    0x0908AD3C,   // 10 //
    0x6874B6EE,   // 11 //
    0x98DD71C1,   // 12 //
    0x00000015,   // 13 //
    0xE630B729,   // 14 //
    0x00282C2A,   // 15 //
    0xF2389290,   // 16 //
    0x00005B55,   // 17 //
    0x58D6CEB0,   // 18 //
    0xF439FB12,   // 19 //
    0x9AD17420,   // 20 //
    0x40DA1EE4,   // 21 //
    0x6BA7F6FE,   // 22 //
    0xAA72CE5F,   // 23 //
    0xFF73A293,   // 24 //
    0x00000000,   // 25 //
    0x73C85A28,   // 26 //
    0x00000059,   // 27 //
    0x49B34F57,   // 28 //
    0x00000000,   // 29 //
    0xB27ABF0E,   // 30 //
    0xE9E552AE,   // 31 //
    0x0000009F,   // 32 //
    0x00000000,   // 33 //
    0x00A2E1D7,   // 34 //
    0x00000000,   // 35 //
};

// expected background image checksums used when status flag 0x20000 is set, same layout as Bs_check_sum3
UINT Bs_check_sum_tbl2[36] =
{
    0x5FE8DFAD,   // 0 //
    0x00000716,   // 1 //
    0xF41C04FB,   // 2 //
    0xDD3F95FE,   // 3 //
    0x2EDBF62A,   // 4 //
    0xB3AD8350,   // 5 //
    0x87EC433A,   // 6 //
    0x5FDC3DEE,   // 7 //
    0xF119F513,   // 8 //
    0x0000215F,   // 9 //
    0x0908AD3C,   // 10 //
    0x6874B6EE,   // 11 //
    0x98DD71C1,   // 12 //
    0x00000015,   // 13 //
    0xE630B729,   // 14 //
    0x00282C2A,   // 15 //
    0xF2389290,   // 16 //
    0x00005B55,   // 17 //
    0x58D6CEB0,   // 18 //
    0xF439FB12,   // 19 //
    0x9AD17420,   // 20 //
    0x40DA1EE4,   // 21 //
    0x6BA7F6FE,   // 22 //
    0xAA72CE5F,   // 23 //
    0xFF73A293,   // 24 //
    0x00000000,   // 25 //
    0x73C85A28,   // 26 //
    0x00000059,   // 27 //
    0x49B34F57,   // 28 //
    0x00000000,   // 29 //
    0xB27ABF0E,   // 30 //
    0xE9E552AE,   // 31 //
    0x0000009F,   // 32 //
    0x00000000,   // 33 //
    0x00A2E1D7,   // 34 //
    0x00000000,   // 35 //
};

// enemy update function by enemy id (0 = none)
VOID (*enemies_update_functions_tbl[48])(VOID) =
{
    0x00000000,                 // 0 //
    0x00000000,                 // 1 //
    Em02_Main,      // 2 //
    0x00000000,                 // 3 //
    Em04_Main,   // 4 //
    0x00000000,                 // 5 //
    Em06_Main,        // 6 //
    0x00000000,                 // 7 //
    0x00000000,                 // 8 //
    0x00000000,                 // 9 //
    Em0a_Main,         // 10 //
    0x00000000,                 // 11 //
    Em0c_Main,        // 12 //
    0x00000000,                 // 13 //
    0x00000000,                 // 14 //
    0x00000000,                 // 15 //
    Em0c_Main,        // 16 //
    0x00000000,                 // 17 //
    0x00000000,                 // 18 //
    0x00000000,                 // 19 //
    0x00000000,                 // 20 //
    0x00000000,                 // 21 //
    0x00000000,                 // 22 //
    0x00000000,                 // 23 //
    0x00000000,                 // 24 //
    0x00000000,                 // 25 //
    0x00000000,                 // 26 //
    0x00000000,                 // 27 //
    0x00000000,                 // 28 //
    0x00000000,                 // 29 //
    0x00000000,                 // 30 //
    0x00000000,                 // 31 //
    0x00000000,                 // 32 //
    0x00000000,                 // 33 //
    0x00000000,                 // 34 //
    0x00000000,                 // 35 //
    0x00000000,                 // 36 //
    0x00000000,                 // 37 //
    0x00000000,                 // 38 //
    0x00000000,                 // 39 //
    0x00000000,                 // 40 //
    0x00000000,                 // 41 //
    0x00000000,                 // 42 //
    0x00000000,                 // 43 //
    0x00000000,                 // 44 //
    0x00000000,                 // 45 //
    0x00000000,                 // 46 //
    0x00000000,                 // 47 //
};

// BGM id per room, 4 bytes per room (0xFF = none)
UINT Bgm_room_tbl[32] =
{
    0xFFFFFF24,   // 0 //
    0xFFFFFF24,   // 1 //
    0xFFFFFF24,   // 2 //
    0xFFFFFF25,   // 3 //
    0xFFFFFF24,   // 4 //
    0xFFFFFF24,   // 5 //
    0xFFFFFF0C,   // 6 //
    0xFFFFFF26,   // 7 //
    0xFFFFFF20,   // 8 //
    0xFFFFFF21,   // 9 //
    0xFFFFFFFF,   // 10 //
    0xFFFFFF22,   // 11 //
    0xFFFFFF23,   // 12 //
    0xFFFFFF21,   // 13 //
    0xFFFFFF0F,   // 14 //
    0xFFFFFF21,   // 15 //
    0xFFFFFF37,   // 16 //
    0xFFFFFFFF,   // 17 //
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
