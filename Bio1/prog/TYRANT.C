/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\TYRANT.C
 */

#include "TYRANT.H"
#include "SCRATCHPAD.H"
#include "MONITOR.H"
#include "SDK.H"

#define Prim_idx ScratchWord[0] // trail primitive index, counted down in the scratchpad word

VOID Em0c_trail_alloc(UCHAR n, EM0C_DR_TBL* pPrims, UINT col)
{
    EM0C_DR_TBL* pCur;

    pCur = pPrims;
    Prim_idx = n;
    if (--Prim_idx != -1)
    {
        do
        {
            SetPolyF4(&pCur[Prim_idx].q[0].p);
            SetSemiTrans(&pCur[Prim_idx].q[0].p, 1);
            SetDrawMode(&pCur[Prim_idx].q[0].Dr_mode, 0, 0,
                (GetGraphType() == 1 || GetGraphType() == 2) ? 0x200 : 0x80, 0);
            pCur[Prim_idx].q[0].p.r0 = col + Prim_idx * 8;
            pCur[Prim_idx].q[0].p.g0 = ((col & 0xFF00) >> 8) + Prim_idx * 16;
            {
                INT red = (col >> 16) & 0xFF;
                pCur[Prim_idx].q[0].p.b0 = red + Prim_idx * 16;
            }
            MargePrim(&pCur[Prim_idx].q[0].Dr_mode, &pCur[Prim_idx].q[0].p);

            SetPolyF4(&pCur[Prim_idx].q[1].p);
            SetSemiTrans(&pCur[Prim_idx].q[1].p, 1);
            SetDrawMode(&pCur[Prim_idx].q[1].Dr_mode, 0, 0,
                (GetGraphType() == 1 || GetGraphType() == 2) ? 0x200 : 0x80, 0);
            pCur[Prim_idx].q[1].p.r0 = col + Prim_idx * 8;
            pCur[Prim_idx].q[1].p.g0 = ((col & 0xFF00) >> 8) + Prim_idx * 16;
            {
                INT red = (col >> 16) & 0xFF;
                pCur[Prim_idx].q[1].p.b0 = red + Prim_idx * 16;
            }
            MargePrim(&pCur[Prim_idx].q[1].Dr_mode, &pCur[Prim_idx].q[1].p);

            SetPolyF4(&pCur[Prim_idx].q[2].p);
            SetSemiTrans(&pCur[Prim_idx].q[2].p, 1);
            SetDrawMode(&pCur[Prim_idx].q[2].Dr_mode, 0, 0,
                (GetGraphType() == 1 || GetGraphType() == 2) ? 0x200 : 0x80, 0);
            pCur[Prim_idx].q[2].p.r0 = col + Prim_idx * 8;
            pCur[Prim_idx].q[2].p.g0 = ((col & 0xFF00) >> 8) + Prim_idx * 16;
            {
                INT red = (col >> 16) & 0xFF;
                pCur[Prim_idx].q[2].p.b0 = red + Prim_idx * 16;
            }
            MargePrim(&pCur[Prim_idx].q[2].Dr_mode, &pCur[Prim_idx].q[2].p);

            SetPolyF4(&pCur[Prim_idx].q[3].p);
            SetSemiTrans(&pCur[Prim_idx].q[3].p, 1);
            SetDrawMode(&pCur[Prim_idx].q[3].Dr_mode, 0, 0,
                (GetGraphType() == 1 || GetGraphType() == 2) ? 0x200 : 0x80, 0);
            pCur[Prim_idx].q[3].p.r0 = col + Prim_idx * 8;
            pCur[Prim_idx].q[3].p.g0 = ((col & 0xFF00) >> 8) + Prim_idx * 16;
            {
                INT red = (col >> 16) & 0xFF;
                pCur[Prim_idx].q[3].p.b0 = red + Prim_idx * 16;
            }
            MargePrim(&pCur[Prim_idx].q[3].Dr_mode, &pCur[Prim_idx].q[3].p);
        } while (--Prim_idx != -1);
    }
    *(UCHAR**)&FREE_WORK_CUR += n << 8;
}

#undef Prim_idx

#define Seg_idx ScratchWord[0] // trail segment index, counted down in the scratchpad word

VOID Em0c_trail_push(EM0C_TRAIL_REC* pRec0, MATRIX* pMat0, MATRIX* pMat1, SVECTOR* pVec0, SVECTOR* pVec1, UCHAR idx, UCHAR segments)
{
    EM0C_TRAIL_REC* pBase = pRec0;
    SVECTOR Sv0;
    SVECTOR Sv1;
    SVECTOR Sv2;
    SVECTOR Sv3;
    LONG p;
    LONG flag;
    EM0C_TRAIL_REC* pRec1;
    EM0C_TRAIL_REC* pRec2;
    EM0C_TRAIL_REC* pRec3;
    EM0C_TRAIL_REC* pRec4;
    EM0C_TRAIL_REC* pRec5;
    EM0C_TRAIL_REC* pRec6;
    UINT otz;
    INT k;

    if (segments == 0)
    {
        pRec1 = (EM0C_TRAIL_REC*)((idx << 8) + (INT)pBase); // width or sign conversion
        pRec1->A = *pMat0;
        pRec1->B = *pMat1;
        pRec1->M.m[0][0] = pMat0->m[0][0] + (pMat1->m[0][0] - pMat0->m[0][0]) / 2;
        pRec1->M.m[0][1] = pMat0->m[0][1] + (pMat1->m[0][1] - pMat0->m[0][1]) / 2;
        pRec1->M.m[0][2] = pMat0->m[0][2] + (pMat1->m[0][2] - pMat0->m[0][2]) / 2;
        pRec1->M.m[1][0] = pMat0->m[1][0] + (pMat1->m[1][0] - pMat0->m[1][0]) / 2;
        pRec1->M.m[1][1] = pMat0->m[1][1] + (pMat1->m[1][1] - pMat0->m[1][1]) / 2;
        pRec1->M.m[1][2] = pMat0->m[1][2] + (pMat1->m[1][2] - pMat0->m[1][2]) / 2;
        pRec1->M.m[2][0] = pMat0->m[2][0] + (pMat1->m[2][0] - pMat0->m[2][0]) / 2;
        pRec1->M.m[2][1] = pMat0->m[2][1] + (pMat1->m[2][1] - pMat0->m[2][1]) / 2;
        pRec1->M.m[2][2] = pMat0->m[2][2] + (pMat1->m[2][2] - pMat0->m[2][2]) / 2;
        pRec1->M.t[0] = pMat0->t[0] + (pMat1->t[0] - pMat0->t[0]) / 2;
        pRec1->M.t[1] = pMat0->t[1] + (pMat1->t[1] - pMat0->t[1]) / 2;
        pRec1->M.t[2] = pMat0->t[2] + (pMat1->t[2] - pMat0->t[2]) / 2;
        pRec1->C = *pVec0;
        pRec1->D = *pVec1;
        return;
    }
    Seg_idx = segments - 1;
    do
    {
        pBase[Seg_idx + 1].A = pBase[Seg_idx].A;
        pBase[Seg_idx + 1].B = pBase[Seg_idx].B;
        pBase[Seg_idx + 1].M = pBase[Seg_idx].M;
    } while (--Seg_idx != -1);
    pBase->A = *pMat0;
    pBase->B = *pMat1;
    pBase->M.m[0][0] = pMat0->m[0][0] + (pMat1->m[0][0] - pMat0->m[0][0]) / 2;
    pBase->M.m[0][1] = pMat0->m[0][1] + (pMat1->m[0][1] - pMat0->m[0][1]) / 2;
    pBase->M.m[0][2] = pMat0->m[0][2] + (pMat1->m[0][2] - pMat0->m[0][2]) / 2;
    pBase->M.m[1][0] = pMat0->m[1][0] + (pMat1->m[1][0] - pMat0->m[1][0]) / 2;
    pBase->M.m[1][1] = pMat0->m[1][1] + (pMat1->m[1][1] - pMat0->m[1][1]) / 2;
    pBase->M.m[1][2] = pMat0->m[1][2] + (pMat1->m[1][2] - pMat0->m[1][2]) / 2;
    pBase->M.m[2][0] = pMat0->m[2][0] + (pMat1->m[2][0] - pMat0->m[2][0]) / 2;
    pBase->M.m[2][1] = pMat0->m[2][1] + (pMat1->m[2][1] - pMat0->m[2][1]) / 2;
    pBase->M.m[2][2] = pMat0->m[2][2] + (pMat1->m[2][2] - pMat0->m[2][2]) / 2;
    pBase->M.t[0] = pMat0->t[0] + (pMat1->t[0] - pMat0->t[0]) / 2;
    pBase->M.t[1] = pMat0->t[1] + (pMat1->t[1] - pMat0->t[1]) / 2;
    pBase->M.t[2] = pMat0->t[2] + (pMat1->t[2] - pMat0->t[2]) / 2;
    k = segments - 1;
    Seg_idx = k;
    pRec2 = &pBase[k];
    ApplyMatrixSV(&pRec2[1].B, &pRec2[1].C, &Sv2);
    Sv2.vx += pBase[Seg_idx + 1].B.t[0];
    Sv2.vy += pBase[Seg_idx + 1].B.t[1];
    Sv2.vz += pBase[Seg_idx + 1].B.t[2];
    ApplyMatrixSV(&(&pBase[Seg_idx])[1].A, &(&pBase[Seg_idx])[1].D, &Sv3);
    Sv3.vx += pBase[Seg_idx + 1].B.t[0];
    Sv3.vy += pBase[Seg_idx + 1].B.t[1];
    Sv3.vz += pBase[Seg_idx + 1].B.t[2];
    do
    {
        pBase[Seg_idx].C.vy += 50; // plain scratch word changes scheduling
        pBase[Seg_idx].D.vy += 100;
        ApplyMatrixSV(&pBase[Seg_idx].M, &pBase[Seg_idx].C, &Sv0);
        Sv0.vx += pBase[Seg_idx].M.t[0];
        Sv0.vy += pBase[Seg_idx].M.t[1];
        Sv0.vz += pBase[Seg_idx].M.t[2];
        ApplyMatrixSV(&pBase[Seg_idx].M, &pBase[Seg_idx].D, &Sv1);
        Sv1.vx += pBase[Seg_idx].M.t[0]; // plain scratch word changes scheduling
        Sv1.vy += pBase[Seg_idx].M.t[1];
        Sv1.vz += pBase[Seg_idx].M.t[2];
        pBase[Seg_idx].C.vy -= 50;
        pBase[Seg_idx].D.vy -= 100;
        SetRotMatrix(MON_WS_MAT);
        SetTransMatrix(Monitor.pWs_mat);
        pRec3 = &pBase[Seg_idx];
        otz = RotAverage4(&Sv0, &Sv1, &Sv2, &Sv3,
            (LONG*)&pRec3->a[CBUF_ID_CUR].p.x0, (LONG*)&pRec3->a[MAIN.Cbuf_id].p.x1,  // LONG* view of the same memory
            (LONG*)&pRec3->a[MAIN.Cbuf_id].p.x2, (LONG*)&pRec3->a[MAIN.Cbuf_id].p.x3,  // word view of the same memory
            &p, &flag);
        if (otz >= 0xFF0)
        {
            otz = 0xFF0;
        }
        pRec4 = &pBase[Seg_idx];
        AddPrim(MAIN.Ot1[MAIN.Cbuf_id].org + (otz >> 4), &pRec4->a[MAIN.Cbuf_id]);
        pRec5 = &pBase[Seg_idx];
        ApplyMatrixSV(&pRec5->B, &pRec5->C, &Sv2);
        Sv2.vx += pBase[Seg_idx].B.t[0];
        Sv2.vy += pBase[Seg_idx].B.t[1];
        Sv2.vz += pBase[Seg_idx].B.t[2];
        ApplyMatrixSV(&pBase[Seg_idx].B, &pBase[Seg_idx].D, &Sv3);
        Sv3.vx += pBase[Seg_idx].B.t[0];
        Sv3.vy += pBase[Seg_idx].B.t[1];
        Sv3.vz += pBase[Seg_idx].B.t[2];
        SetRotMatrix(Monitor.pWs_mat); // plain Monitor.pWs_mat changes scheduling
        SetTransMatrix(Monitor.pWs_mat);
        pRec6 = &pBase[Seg_idx];
        otz = RotAverage4(&Sv0, &Sv1, &Sv2, &Sv3,
            (LONG*)&pRec6->b[MAIN.Cbuf_id].p.x0, (LONG*)&pRec6->b[MAIN.Cbuf_id].p.x1,  // word view of the same memory
            (LONG*)&pRec6->b[MAIN.Cbuf_id].p.x2, (LONG*)&pRec6->b[MAIN.Cbuf_id].p.x3,  // word view of the same memory
            &p, &flag);
        if (otz >= 0xFF0)
        {
            otz = 0xFF0;
        }
        pRec4 = &pBase[Seg_idx];
        AddPrim(MAIN.Ot1[MAIN.Cbuf_id].org + (otz >> 4), &pRec4->b[MAIN.Cbuf_id]);
    } while (--Seg_idx != -1);
}

#undef Seg_idx
