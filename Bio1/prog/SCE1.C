/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SCE1.C
 */

#include "TYPES.H"
#include "SCE1.H"
#include "SCE0.H"

INT Flg_on(VOID* p, UINT x)
{
    ((UINT*)p)[x >> 5] |= 0x80000000U >> (x & 0x1F);  // word view of the same memory
}

INT Flg_off(VOID* p, UINT x)
{
    ((UINT*)p)[x >> 5] &= ~(0x80000000U >> (x & 0x1F));  // word view of the same memory
}

INT Flg_ck(VOID* p, UINT x)
{
    return ((INT*)p)[x >> 5] & (0x80000000U >> (x & 0x1F));  // word view of the same memory
}

VOID Sce_scheduler(UCHAR* pCScd)
{
    UCHAR* pBlock_top;
    USHORT width;

    pBlock_top = pCScd;
    G.Scenario.ppSce_sp = (UCHAR**)&MAIN.Scenario.Ifel_stk;  // address passed as an integer
    G.Scenario.Ifel_ctr = 0;
    G.Scenario.pSce_data = pBlock_top;
    while ((width = *(USHORT*)pBlock_top) != 0)  // halfword view of the same memory
    {
        SCE_DATA_CUR += 2;
    loop:
        while (Sce_jmp_tbl[*G.Scenario.pSce_data]() != 0)
        {
        }
        if (VIEW(UCHAR, MAIN.Scenario.Ifel_ctr) != 0)  // UCHAR* view of the field (a plain access changes the code)
        {
            SCE_SP_WORD -= 4;
            G.Scenario.pSce_data = *MAIN.Scenario.ppSce_sp;
            MAIN.Scenario.Ifel_ctr--;
            goto loop;
        }
        pBlock_top += width;
        G.Scenario.pSce_data = pBlock_top;
    }
}
