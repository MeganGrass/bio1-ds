/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\CUT.C
 */

#include "CUT.H"
#include "CD.H"
#include "GAME.H"
#include "PAD.H"
#include "ROOM.H"
#include "SCE0.H"
#include "STR.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "PSP.H"

INT Cut_set(VOID)
{
    if (VIEW(UCHAR, MAIN.Tpage_clut_bak) != 0)  // UCHAR* view of the field (a plain access changes the code)
    {
        G.Vram_cur.Tpage_clut = MAIN.Tpage_clut_bak;
        Psp_set();
        if (!(G.System_flg & SYS_DOOR_PROCESS))
        {
            Set_view();
            Bg_draw();
            Psp_mask_set();
        }
    }
}

VOID Set_view(VOID)
{
    INT Rdt;
    ROOM_WORK* pRoom;
    Rdt = MAIN_PROOM_INT;
    pRoom = (ROOM_WORK*)(Rdt + (CUT_NO_CUR) * 0x2C);  // reinterprets the memory as ROOM_WORK
    GsSetProjection(pRoom->Cut[0].ViewR);
    GsSetRefView2((GsRVIEW2*)((INT)MAIN.pRoom + MAIN.SaveData.Cut_no * 0x2C + 0x9C));  // signed conversion (unsigned operand)
    if (1)
    {
    }
}

VOID Bg_draw(VOID)
{
    RECT rect;

    UCHAR Act[2];

    UCHAR st;
    USHORT* pTbl;

    USHORT old;
    UINT v;

    UCHAR* pMtr;

    rect.x = 0;
    rect.y = 0;
    rect.w = 0x140;
    rect.h = 0xF0;
    if (GsGetActiveBuff() != 0)
    {
        rect.y = 0xF0;
    }

    STATUS_FLG_CUR |= STAT_BG_DRAW;
    pMtr = Act_mtr_lv;
    Act[0] = *pMtr;
    Act[1] = Act_mtr_lv[1];
    *pMtr = 0;
    Act_mtr_lv[1] = 0;

    st = NOPROTO(INT, Rdt_stage_get)();  // passes 1 argument, Rdt_stage_get declares 0
    if ((UCHAR)(st - 5) < 2 || (UCHAR)(st - 12) < 2)  // truncates to a byte
    {
        st -= 5;
    }
    old = floc[Rdt_tbl[st][ROOM_NO_CUR]].sector;
    floc[Rdt_tbl[st][MAIN.SaveData.Room_no]].size = 0x8000;
    v = floc[Rdt_tbl[st][MAIN.SaveData.Room_no]].sector;

    v += CUT_NO_CUR * 16;

    floc[Rdt_tbl[st][MAIN.SaveData.Room_no]].sec_high += v >> 16;
    pTbl = Rdt_tbl[st];

    CHECK_SUM_FLG_CUR = 1;

    floc[pTbl[MAIN.SaveData.Room_no]].sector += MAIN.SaveData.Cut_no * 16;

    do
    {
        Cd_read_wait(pTbl[G.SaveData.Room_no], IMG_TRANS_BUF);
    } while (Cd_bg_sum_ck() == 0);

    floc[Rdt_tbl[st][ROOM_NO_CUR]].sector = old;
    floc[Rdt_tbl[st][MAIN.SaveData.Room_no]].sec_high -= v >> 16;

    G.Check_sum_flg = 0;
    Mdec_vram_decode(&rect, IMG_TRANS_BUF, MDEC_DEC_BUF, LOAD_BUF, 0);
    StoreImage(&rect, (ULONG*)LOAD_BUF);  // word view of the same memory
    DrawSync(0);
    rect.y = -(rect.y != 0xF0) & 0xF0;
    MoveImage(&rect, 0, -(rect.y != 0xF0) & 0xF0);
    DrawSync(0);

    Act_mtr_lv[0] = Act[0];
    Act_mtr_lv[1] = Act[1];
    STATUS_FLG_CUR &= ~STAT_BG_DRAW;
}

VOID Cut_check(INT Flg)
{
    INT flags;
    VCUT* cp;
    INT busy;
    cp = (VCUT*)G.Cccut + 1;  // reinterprets the memory as VCUT
    if (!(G.System_flg & SYS_CAM_LOCK))
    {
        if (((VCUT*)CCCUT_NOW)[1].Fcut == G.SaveData.Cut_no)  // VOID** view of the field (a plain access changes the code)
        {
            INT* pSystem_flg = &MAIN.System_flg;
        zone_loop:
            if (Hit_ck_point4(&PL_POS_VEC, cp) != 0)
            {
                flags = *pSystem_flg;
                busy = flags & SYS_CUT_HOLD;
                G.SaveData.Cut_no = cp->Tcut;
                if (busy)
                {
                    *pSystem_flg = flags | SYS_CUT_PENDING;
                    return;
                }
                Cut_change();
                return;
            }

            cp++;
            if (cp->Fcut != G.SaveData.Cut_no)
            {
                goto skip_tail;
            }
            goto zone_loop;
        }
    skip_tail:
        if (Flg != 0)
        {
            Cut_set();
        }
    }
}

VOID Cut_change(VOID)
{
    VOID** ppVc;
    VCUT* cp;
    VCUT* pCcut;

    cp = G.pRoom->pVcut;
    VIEW_VIA_CHAR(VOID*, G.Cccut) = cp;  // VOID* view of the field (a plain access changes the code)
    ppVc = (VOID**)VCUT_SENTINEL;  // untyped pointer, the callee takes the address as is
    if (cp->Fcut != G.SaveData.Cut_no)
    {
        ppVc = &G.Cccut;
        do
        {
            pCcut = CCCUT_CUR;
            MAIN.Cccut = pCcut + 1;
        } while (pCcut[1].Fcut != G_BASE->SaveData.Cut_no);
    }
    Cut_set();
}

INT Hit_ck_point4(VECTOR* pPos, VCUT* pVc)
{
    INT x0;
    INT y0;
    INT Ax;
    INT Ay;
    INT px;
    INT py;

    INT Bx;
    INT by;

    INT cx;
    INT cy;

    x0 = pVc->X0;
    y0 = pVc->Z0;
    py = pPos->vz - y0;
    Ax = pVc->X1 - x0;
    px = pPos->vx - x0;
    Ay = pVc->Z1 - y0;
    Bx = pVc->X3 - x0;
    by = pVc->Z3 - y0;
    if (Ax * py > Ay * px)
    {
        return 0;
    }
    if (Bx * py < by * px)
    {
        return 0;
    }
    cy = pVc->Z2 - y0;
    cx = pVc->X2 - x0;
    py -= cy;
    Ax -= cx;
    px -= cx;
    Ay -= cy;
    Bx -= cx;
    by -= cy;
    if (Ax * py < Ay * px || Bx * py > by * px)
    {
        return 0;
    }
    return 1;
}
