/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\OBA.C
 */

#include "TYPES.H"
#include "OBA.H"

INT Oba_ck_hit(ENEMY_WORK* pEm, ENEMY_WORK* pTa)
{
    AT_WORK* pHit_em;
    AT_WORK* pHit_ta;
    AT_TBL* pSca_em;
    AT_TBL* pSca_ta;
    INT Dx;
    INT Dz;
    INT d;
    INT r;

    INT Ay;

    INT Dy;
    INT h;
    INT hosei_x;
    INT hosei_z;
    INT hit;

    SHORT By;

    hit = 0;
    if (pTa->Routine_0 == 4 || ((pEm->Be_flg | pTa->Be_flg) & 2))
    {
        return 0;
    }
    pSca_em = pEm->pAt_tbl;
    pHit_em = pEm->pSca_ofs;
    for (;;)
    {
        pHit_ta = pTa->pSca_ofs;
        pSca_ta = pTa->pAt_tbl;
        for (;;)
        {
            Dx = pTa->Pos_x + pHit_ta->Off_x - (pEm->Pos_x + pHit_em->Off_x);
            Dz = pTa->Pos_z + pHit_ta->Off_z - (pEm->Pos_z + pHit_em->Off_z);

            d = pSca_em->At_w + pSca_ta->At_w;
            r = SquareRoot0(Dx * Dx + Dz * Dz) + 1;
            d -= r;
            if (d > 0)
            {
                By = pHit_ta->Off_y;
                Dy = pTa->Pos_y + By - (Ay = pEm->Pos_y + pHit_em->Off_y);

                h = pSca_ta->H + pSca_em->H;
                if (-h < Dy && Dy < h)
                {
                    hosei_x = Dx * d / r;
                    hosei_z = Dz * d / r;

                    Dy = pTa->Old_y + By - Ay;
                    if (!(-h < Dy && Dy < h))
                    {
                        if (((pTa->Old_x) < pEm->Pos_x && pEm->Pos_x < pTa->Pos_x) ||
                            (pEm->Pos_x < (pTa->Old_x) && pTa->Pos_x < pEm->Pos_x))
                        {
                            hosei_x = -(Dx * d) / r;
                            if (hosei_x > 0)
                            {
                                hosei_x = pSca_em->At_w * 2 - hosei_x;
                            }
                            else
                            {
                                hosei_x = -(pSca_em->At_w * 2) - hosei_x;
                            }
                        }
                        if (((pTa->Old_z) < pEm->Pos_z && pEm->Pos_z < pTa->Pos_z) ||
                            (pEm->Pos_z < (pTa->Old_z) && pTa->Pos_z < pEm->Pos_z))
                        {
                            hosei_z = -(Dz * d) / r;
                            if (hosei_z > 0)
                            {
                                hosei_z = pSca_em->At_w * 2 - hosei_z;
                            }
                            else
                            {
                                hosei_z = -(pSca_em->At_w * 2) - hosei_z;
                            }
                        }
                    }
                    hit = 1;
                    pTa->Pos_x += hosei_x;
                    pTa->Pos_z += hosei_z;
                }
            }

            pHit_ta++;

            if (pSca_ta->Flg < 0)  // sign-extends the halfword
            {
                break;
            }

            pSca_ta++;
        }

        pHit_em++;

        if (pSca_em->Flg < 0)  // sign-extends the halfword
        {
            break;
        }

        pSca_em++;
    }
    return hit;
}

VOID Oba_set_ofs(ENEMY_WORK* pEm)
{
    AT_TBL* pSca;
    AT_WORK* pHit;

    pSca = pEm->pAt_tbl;
    pHit = pEm->pSca_ofs;

    ScratchPad->Sv.vy = pEm->Cdir_y;
    for (;;)
    {
        ScratchPad->Sv.vz = 0;
        ScratchSv[0] = 0;
        RotMatrix(&ScratchPad->Sv, &ScratchPad->Mat);
        ScratchPad->Sv.vx = pSca->Off_x;
        ScratchPad->Sv.vz = pSca->Off_z;
        ApplyMatrix(&ScratchPad->Mat, &ScratchPad->Sv, &ScratchPad->Vec);
        pHit->Off_x = ScratchPad->Vec.vx;
        pHit->Off_y = pSca->Off_y;
        pHit->Off_z = ScratchPad->Vec.vz;
        if (pSca->Flg < 0)  // sign-extends the halfword
        {
            break;
        }
        pHit++;
        pSca++;
    }
}
