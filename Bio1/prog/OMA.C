/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\OMA.C
 */

#include "TYPES.H"
#include "OMA.H"
#include "CUT.H"
#include "GAME.H"
#include "SCE0.H"
#include "SCE_AT.H"
#include "SCENARIO.H"
#include "FLAG.H"

INT Oma_ck_oba(ENEMY_WORK* pEm, OBJ_MODEL_WORK* pOm, INT Flg)
{
    AT_WORK* ofs;
    AT_TBL* box;
    AT_TBL* m;
    AT_TBL* s;
    INT x1;
    INT z1;
    INT Dx;
    INT Dy;
    INT Dz;
    INT Wx;
    INT Wy;
    INT Wz;
    INT Ax;
    INT Az;
    INT v;
    USHORT h;
    INT n;

    n = 0;
    if ((pEm->Be_flg & EM_BE_NO_OBJ) || (pOm->Be_flg & OM_NO_COLLISION))
    {
        return 0;
    }
    ofs = pEm->pSca_ofs;
    box = pEm->pAt_tbl;
    for (;;)
    {
        x1 = pOm->Coord.t[0];
        z1 = pOm->Coord.t[2];
        Dx = x1 - (pEm->Pos_x + ofs->Off_x);
        Dy = pOm->Coord.t[1] - (pEm->Pos_y + ofs->Off_y);
        Dz = z1 - (pEm->Pos_z + ofs->Off_z);
        m = pEm->pAt_tbl;
        s = pOm->pAt_tbl;
        h = m->At_w;
        Wx = h + s->Off_x;
        Wy = m->H + s->Off_y;
        Wz = h + s->Off_z;
        if (Dx + Wx <= (UINT)(Wx * 2) && Dy + Wy <= (UINT)(Wy * 2) && Dz + Wz <= (UINT)(Wz * 2))  // unsigned conversion (signed operand)
        {
            Ax = __builtin_abs(Dx * Wz);
            Az = __builtin_abs(Dz * Wx);
            if (Az < Ax)
            {
                if (Flg != 0)
                {
                    v = pEm->Pos_x + m->Off_x;
                    if (Dx < 0)
                    {
                        pOm->Coord.t[0] = v - Wx;
                    }
                    else
                    {
                        pOm->Coord.t[0] = v + Wx;
                    }
                    n++;
                }
                else if (Dx < 0)
                {
                    pEm->Pos_x = x1 + Wx;
                }
                else
                {
                    pEm->Pos_x = x1 - Wx;
                }
            }
            else if (Flg != 0)
            {
                v = m->Off_z;

                do
                {
                    v += pEm->Pos_z;
                } while (0);

                if (Dz < 0)
                {
                    v -= Wz;
                }
                else
                {
                    v += Wz;
                }

                pOm->Coord.t[2] = v;

                n++;
            }
            else
            {
                v = z1 - Wz;
                if (Dz < 0)
                {
                    v = z1 + Wz;
                }
                pEm->Pos_z = v;
            }
        }
        if (box->Flg < 0)  // sign-extends the halfword
        {
            break;
        }
        ofs++;
        box++;
    }
    return n;
}

INT Oma_ck_om(OBJ_MODEL_WORK* pOm, OBJ_MODEL_WORK* pOm2)
{
    INT x0;
    INT z0;
    INT Dx;
    INT Dz;
    INT Wx;
    INT Wz;
    INT m1;
    INT m2;
    AT_TBL* pa;
    AT_TBL* pb;

    if ((pOm->Be_flg | pOm2->Be_flg) & 8)
    {
        return 0;
    }
    Dx = pOm2->Coord.t[0] - pOm->Coord.t[0];
    Dz = pOm2->Coord.t[2] - pOm->Coord.t[2];
    x0 = pOm->Coord.t[0];
    z0 = pOm->Coord.t[2];
    pa = pOm->pAt_tbl;
    pb = pOm2->pAt_tbl;
    Wx = pa->Off_x + pb->Off_x;
    Wz = pa->Off_z + pb->Off_z;
    if (Dx + Wx > (UINT)(Wx * 2))  // unsigned conversion (signed operand)
    {
        return 0;
    }
    if (Dz + Wz > (UINT)(Wz * 2))  // unsigned conversion (signed operand)
    {
        return 0;
    }
    m1 = __builtin_abs(Dx * Wz);
    m2 = __builtin_abs(Dz * Wx);
    if (m2 < m1)
    {
        if (Dx < 0)
        {
            pOm2->Coord.t[0] = x0 - (Wx + 1);
        }
        else
        {
            pOm2->Coord.t[0] = x0 + (Wx + 1);
        }
    }
    else
    {
        if (Dz < 0)
        {
            pOm2->Coord.t[2] = z0 - (Wz + 1);
        }
        else
        {
            pOm2->Coord.t[2] = z0 + (Wz + 1);
        }
    }
    return 1;
}

VOID Om_obj_trans(OBJ_MODEL_WORK* pOm)
{
    MATRIX M;

    Set_light(MATRIX_POS_PTR(&pOm->Coord));
    GsGetLs(pOm->pCoord2, &M);
    MulMatrix0(&GsLIGHTWSMATRIX, &pOm->Coord, &ScratchPad->Mat);
    SetLightMatrix(&ScratchPad->Mat);
    GsSetLsMatrix(&M);
    if (Hit_ck_point4(MATRIX_POS_PTR(&pOm->Workm), G.Cccut) != 0)
    {
        if (pOm->Be_flg & 0x80)
        {
            GsSortObject5((GsDOBJ5*)(&pOm->Attribute), MAIN.Ot2 + G.Cbuf_id, 0xA, ScratchUlong);  // the embedded GsDOBJ5 starts at Attribute
        }
        else
        {
            GsSortObject5((GsDOBJ5*)(&pOm->Attribute), MAIN.Ot1 + G.Cbuf_id, 4, ScratchUlong);  // the embedded GsDOBJ5 starts at Attribute
        }
    }
}

INT Oma_pl_updown_ck(VOID)
{
    INT t;
    INT n;
    OBJ_MODEL_WORK* pOm;
    INT d;
    INT r;

    t = MAIN.System_flg;
    if (t & SYS_CLIMB)
    {
        r = 0;
        if ((USHORT)(__builtin_abs((SHORT)G.Player.Cdir_y - (SHORT)Oma_climb_om->Cdir_y) - 300) >= 0xDA9)  // truncates to an unsigned halfword
        {
            MAIN.System_flg = t & ~SYS_CLIMB;
            G.Player.Zone_in_flg |= 0x10;
            goto dir;
        }
        return r;
    }
    n = Omodel_num;
    if (n != 0)
    {
        do
        {
            n--;
            pOm = Omodel_tbl[n];
            if ((pOm->Be_flg & 0x40) && Oma_ck_front(pOm) != 0)
            {
                d = (((SHORT)G.Player.Cdir_y + 0x800) & 0xFFF) - (SHORT)pOm->Cdir_y;  // sign-extends the halfword
                if (d < 0)
                {
                    d = -d;
                }
                if ((USHORT)(d - 300) >= 0xDA9)  // truncates to an unsigned halfword
                {
                    Oma_climb_om = pOm;
                    SYSTEM_FLG_CUR |= SYS_CLIMB;
                    MAIN.Player.Zone_in_flg &= 0xEF;
                    goto dir;
                }
            }
        } while (n != 0);
    }
    return 0;
dir:
    if (((SHORT)G.Player.Cdir_y + 0x200) & 0x800)  // sign-extends the halfword
    {
        G.Player.Timer0 = 1;
    }
    else
    {
        G.Player.Timer0 = -1;
    }
    return 1;
}

INT Oma_ck_front(OBJ_MODEL_WORK* pOm)
{
    SHORT* p;
    SHORT X;
    SHORT Z;
    INT Ax;
    INT Az;
    UINT dx;
    UINT dz;
    INT W;
    INT c;

    p = &ScratchPad->Sv.vx;
    *p = 0x1D6;
    ScratchPad->Sv.vz = 0;
    Rot_vector(G.Player.Cdir_y, &ScratchPad->Sv, &ScratchPad->Sv);
    X = *p + G.Player.Pos_x;
    *p = X;
    Z = ScratchPad->Sv.vz + G.Player.Pos_z;
    ScratchPad->Sv.vz = Z;
    dx = X - pOm->Coord.t[0];
    dz = Z - pOm->Coord.t[2];
    W = pOm->Sca_ofs_x;
    dx += W;
    W *= 2;
    c = W < dx;
    dx = pOm->Sca_ofs_z;
    if (c)
    {
        return 0;
    }
    dz += dx;
    if (dx * 2 < dz)
    {
        return 0;
    }
    Ax = __builtin_abs(X);
    Az = __builtin_abs(Z);
    if (Az < Ax)
    {
        ScratchPad->Sv.vz = 0;
    }
    else
    {
        *p = 0;
    }
    return 1;
}
