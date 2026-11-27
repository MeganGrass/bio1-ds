/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog2\STAGE3.H
 */

#ifndef STAGE3_H
#define STAGE3_H

#include "TYPES.H"

/*
    overlay entry point: install the stage 3 enemy update table, player animation handlers,
    BGM, voice and background checksum tables into the scheduler workspace, then end the task
 */
VOID main(VOID);

extern UINT Xa_sector03[33];       // XA voice clip table, USHORT entries
extern UINT Bs_check_sum3[36];        // expected background image checksums
extern UINT Bs_check_sum_tbl2[36];    // expected background image checksums, alternate set
extern VOID (*enemies_update_functions_tbl[48])(VOID); // enemy update function by enemy id (0 = none)
extern UINT Bgm_room_tbl[32];                // BGM id table, 4 bytes per room

#endif
