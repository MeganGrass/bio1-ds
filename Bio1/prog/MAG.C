/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\MAG.C
 */

#include "TYPES.H"

#include "MAG.H"
#include "EFF1.H"
#include "GAME.H"
#include "SCE0.H"
#include "FLAG.H"
#include "MONITOR.H"
#include "SDK.H"
#include "SND.H"

VOID Mag_anm_clr()
{
    PARTS_W* pP_ptr;

    pP_ptr = G.Player.pSin_parts_ptr;
    pP_ptr += 15;
    pP_ptr->Free2 = 0;
    pP_ptr->Free3 = 0;
}

VOID Mag_down(VOID)
{
    MATRIX tmp;
    MATRIX* transform;
    MATRIX* world;
    INT vel;
    TMD_MEM* tmd;
    PARTS_W* pP_ptr;

    pP_ptr = G.Player.pSin_parts_ptr;

    pP_ptr += 15;
    if ((G.Player.Zone_in_flg & 0x7F) && pP_ptr->Free2 != 0)
    {
        pP_ptr->Workm.t[1] += pP_ptr->Free0;
        vel = (USHORT)pP_ptr->Free0; // width or sign conversion
        pP_ptr->Free1 += vel;
        Mag_mv_tbl[pP_ptr->Free3](pP_ptr, vel);
        Set_light(&PL_POS_VEC);
        transform = &pP_ptr->Coord;
        RotMatrix(&pP_ptr->Cdir, transform);
        world = &pP_ptr->Workm;
        GsMulCoord0(world, transform, &tmp);
        CompMatrix(Monitor.pWs_mat, &tmp, &ScratchPad->Mat);
        MulMatrix0(Monitor.pLight_mtx, world, &tmp);
        ScratchPtr[14] = pP_ptr->pTmd;
        SetLightMatrix(&tmp);
        GsSetLsMatrix(&ScratchPad->Mat);
        GsPrstTG3L(ScratchTmd[14]->prim_top, ScratchTmd[14]->ver_top, ScratchTmd[14]->nor_top, (UCHAR*)pP_ptr->pPacket + CBUF_ID_CUR * sizeof(POLY_GT3) + 8, ScratchTmd[14]->n_poly, 4, &MAIN.Ot1[MAIN.Cbuf_id]); // MAIN field re-typed in place; plain field changes scheduling
    }
}

VOID Mag_mv00(PARTS_W* pP_ptr)
{
    PARTS_W* pHand;

    pP_ptr->Free3 += 1;
    pHand = G.Player.pSin_parts_ptr;
    pHand += 14;
    pP_ptr->Workm = pHand->Workm;
    pP_ptr->Coord.t[0] = 0;
    pP_ptr->Coord.t[1] = 0;
    pP_ptr->Coord.t[2] = 0;
    pP_ptr->Cdir = pHand->Cdir;
    pP_ptr->Cdir.vz = 0x400;
    pP_ptr->Cdir.vx = 0;
    pP_ptr->Cdir.vy = 0;
    pP_ptr->Free0 = 0x50;
    pP_ptr->Free4 = 0;
    pP_ptr->Free1 = pP_ptr->Coord.t[1] + pP_ptr->Workm.t[1];

    pP_ptr->Free5 = G.Player.Pos_y;
}

VOID Mag_mv01(PARTS_W* pP_ptr)
{
    pP_ptr->Free0 = (USHORT)pP_ptr->Free0 + 0x14 - pP_ptr->Free4 * 0x14; // width or sign conversion
    pP_ptr->Cdir.vx = (USHORT)pP_ptr->Cdir.vx + 0x10 + pP_ptr->Free4 * 0x10; // width or sign conversion
    if (pP_ptr->Free4 == 0 && (G.Status_flg & STAT_WATER) && Omodel_tbl[0]->Coord.t[1] < pP_ptr->Free1)
    {
        ScratchPad->Vec = MATRIX_POS(MON_ID_MAT);

        Esp_call(0x17, 8, 0, Monitor.pId_mat, MATRIX_POS_PTR(&pP_ptr->Workm), 0);

        pP_ptr->Free4 = 1;
        return;
    }

    if (pP_ptr->Free1 > pP_ptr->Free5)
    {
        pP_ptr->Free0 = -0x50;
        pP_ptr->Free3 += 1;

        pP_ptr->Workm.t[1] = -pP_ptr->Coord.t[1] + pP_ptr->Free5;
        pP_ptr->Free1 = pP_ptr->Free5;

        ScratchPad->Vec.vx = pP_ptr->Coord.t[0] + pP_ptr->Workm.t[0];

        ScratchPad->Vec.vy = pP_ptr->Coord.t[1] + pP_ptr->Workm.t[1];

        ScratchPad->Vec.vz = pP_ptr->Coord.t[2] + pP_ptr->Workm.t[2];
        Snd_se_stad(2, 0x15, 0, &ScratchPad->Vec);
    }
}

VOID Mag_mv02(PARTS_W* pP_ptr)
{
    pP_ptr->Workm.t[0] += 0x14;
    pP_ptr->Workm.t[2] += 0xA;
    pP_ptr->Cdir.vy = (USHORT)pP_ptr->Cdir.vy + 0x80 - pP_ptr->Free4 * 0x40; // width or sign conversion
    pP_ptr->Cdir.vx = (USHORT)pP_ptr->Cdir.vx + 0x100 - pP_ptr->Free4 * 0x80; // width or sign conversion
    pP_ptr->Free0 = (USHORT)pP_ptr->Free0 + 0xF + pP_ptr->Free4 * 0xF; // width or sign conversion

    if (pP_ptr->Free5 < pP_ptr->Free1)
    {
        pP_ptr->Free2 = 0;
    }
}

VOID Mag_set(VOID)
{
    PARTS_W* pBase = G.Player.pSin_parts_ptr;

    PARTS_W* pP_ptr;

    pBase += 15;
    pP_ptr = pBase;
    if (pP_ptr->Free2 == 0)
    {
        pP_ptr->Free2 = 1;
        pP_ptr->Free3 = 0;
    }
}

// magazine state handlers, indexed by the magazine state (joint Free3); the last entry is never called
VOID (*Mag_mv_tbl[4])() =
{
    Mag_mv00,    // 0 //
    Mag_mv01,     // 1 //
    Mag_mv02,   // 2 //
    (VOID (*)())1,                // 3 //
};
