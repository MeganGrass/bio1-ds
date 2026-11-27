/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ZENTAI.C
 */

#include "TYPES.H"
#include "ZENTAI.H"

VOID Init_global(VOID)
{
    INT* p;

    p = &MAIN.System_flg;
    do
    {
        *p = 0;
        p += 1;
    } while (p <= (INT*)&MAIN.Dummy17[4]);  // walk the block as INT words
    G.SaveData.Mess_flg = 0;
}

VOID Init_snd_fade(VOID)
{
    G.Vloop = 2;
    G.Fade_rtn = 0;

    G.SaveData.Fade_kido = -1;

    G.SaveData.Fade1_kido = -1;
}

VOID Game_work_clr(VOID)
{
    INT* p;

    p = (INT*)&MAIN.Def_item;  // word view of the same memory
    do
    {
        *p = 0;
        p += 1;
    } while (p < (INT*)&MAIN.SaveData);  // walk the block as INT words
}

VOID Sce_work_clr(VOID)
{
    G.SaveData.Get_id = 0;
    G.SaveData.Use_id = 0;
    G.Room_flg[1] = 0;
    VIEW(INT, G.SaveData.Front_aot_no) = 0;  // INT* view of the field (a plain access changes the code)
}

VOID Memclr_dw(INT* p, INT* end)
{
    do
    {
        *p++ = 0;
    } while (p < end);
}

#include "FILE_LOC.H"      // position of this include fixes the data order within the object

VOID* pFixed_work = &G;  // pointer to the fixed work area
