/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\PSP.C
 */

#include "PSP.H"
#include "SCE_AT.H"
#include "SUB.H"
#include "FLAG.H"

VOID Psp_set(VOID)
{
    UCHAR* pGrp_top;
    USHORT* pGrp;
    PSP_SPR_REC* pSpr;
    USHORT count;
    UINT i;
    UINT j;
    USHORT f;
    USHORT t;

    count = 0;
    pGrp_top = G.pRoom->Cut[G.SaveData.Cut_no].pSp;
    pGrp = (USHORT*)(pGrp_top + 4);  // halfword view of the same memory
    pSpr = BYTE_OFS(PSP_SPR_REC, pGrp, *(UINT*)pGrp_top * 8);
    for (i = 0; i < *(UINT*)G.pRoom->Cut[G.SaveData.Cut_no].pSp; pGrp += 4, i++)  // word view of the same memory
    {
        for (j = 0; j < pGrp[0]; j++)
        {
            Room_psp[count].be_flg = 1;
            Room_psp[count].id = i + 1;
            Room_psp[count].cx = (pGrp[1] & 0x3F) << 4;
            Room_psp[count].cy = CCLUT_UBYTE_NOW + 0x1E0;
            Room_psp[count].u = pSpr->u;
            Room_psp[count].v = pSpr->v;
            Room_psp[count].x = pSpr->x + pGrp[2] - 0xA0;
            Room_psp[count].y = pSpr->y + pGrp[3] - 0x78;
            Room_psp[count].otz = pSpr->depth;
            f = pSpr->attr;
            Room_psp[count].tpage = MAIN.Vram_cur.b.Ctpage + (f & 0x1F);
            if (!(f & 0xF000))
            {
                Room_psp[count].w = pSpr->w;
                Room_psp[count].h = pSpr->h;
                pSpr++;
            }
            else
            {
                pSpr = BYTE_OFS(PSP_SPR_REC, pSpr, 8);
                t = (f >> 12) * 8;
                Room_psp[count].w = t;
                Room_psp[count].h = t;
            }
            Room_psp[count].attribute |= 0x40;
            if (!(f & 0xC00))
            {
                Room_psp[count].attribute |= 0x08000000;
            }
            Room_psp[count].attribute |= ((f >> 7) & 3) << 24;
            Room_psp[count].attribute |= ((f >> 5) & 3) << 28;
            count++;
        }
    }
    *MAIN_PROOM(UCHAR) = count;
    if (count)
    {
        Mapping_tim(MAIN.pRoom->Cut[G.SaveData.Cut_no].pSp_tim);
    }
}

VOID Psp_trans(VOID)
{
    INT unused[2];    // unused local: reserves stack space

    UINT z;

    UINT i;

    if (!(G.Status_flg & STAT_PSP_HIDE))
    {
        i = 0;
        if (G.pRoom->nSprite != 0)
        {
            do
            {
                if (Room_psp[i].be_flg != 0)
                {
                    z = Room_psp[i].otz >> 2;
                    GsSortFastSprite((GsSPRITE*)&Room_psp[i], &MAIN.Ot1[G.Cbuf_id], z < 0x400 ? z : 0x3FF);  // the room sprite record is read as a GsSPRITE
                }
                i++;
            } while (i < G.pRoom->nSprite);
        }
    }
}

INT Psp_off(psp_id)
UCHAR psp_id;  // narrow parameter, passed as int
{
    INT unused[2];    // unused local: reserves stack space
    UINT i;

    i = 0;
    if (G.pRoom->nSprite != 0)
    {
        do
        {
            if (Room_psp[i].id == psp_id)
            {
                Room_psp[i].be_flg = 0;
            }
            i++;
        } while (i < *MAIN_PROOM(UCHAR));
    }
}

VOID Psp_on(UCHAR psp_id)
{
    INT unused[2];    // unused local: reserves stack space
    UINT i;

    i = 0;
    if (G.pRoom->nSprite != 0)
    {
        do
        {
            if (Room_psp[i].id == psp_id)
            {
                Room_psp[i].be_flg = 1;
            }
            i++;
        } while (i < *MAIN_PROOM(UCHAR));
    }
}
