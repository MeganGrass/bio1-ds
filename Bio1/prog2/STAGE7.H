/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog2\STAGE7.H
 */

#ifndef STAGE7_H
#define STAGE7_H

#include "TYPES.H"

/*
    overlay entry point: install the stage 7 enemy update table, player animation handlers,
    BGM, voice and background checksum tables into the scheduler workspace, then end the task
 */
VOID main(VOID);

extern UINT Xa_sector07[55];       // XA voice clip table, USHORT entries
extern UINT Bs_check_sum7[58];        // expected background image checksums
extern UINT Bs_check_sum_tbl2[58];    // expected background image checksums, alternate set
extern VOID (*enemies_update_functions_tbl[48])(VOID); // enemy update function by enemy id (0 = none)
extern UINT Bgm_room_tbl[32];                // BGM id table, 4 bytes per room

#endif
