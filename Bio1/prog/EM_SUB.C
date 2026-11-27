/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM_SUB.C
 */

#include "TYPES.H"
#include <RAND.H>
#include "EM_SUB.H"
#include "CUT.H"
#include "EFF1.H"
#include "GUIDE_CK.H"
#include "JOINT.H"
#include "MAIN.H"
#include "OBA.H"
#include "PAD.H"
#include "PLAYER.H"
#include "SCA.H"
#include "SND.H"
#include "FLAG.H"
#include "SDK.H"

SHORT Em_fall_move(SHORT speed, INT Fall_spd, SHORT gravity, SHORT floor_y)
{
    SVECTOR sv;
    MATRIX m;
    ENEMY_WORK** ppEm;
    ENEMY_WORK* pEm;
    SHORT t;

    ppEm = &MAIN.pEm;
    sv.vz = 0;
    sv.vx = 0;
    sv.vy = (*ppEm)->Cdir_y;
    RotMatrix(&sv, &m);
    sv.vx = speed;
    sv.vz = 0;
    sv.vy = 0;
    ApplyMatrixSV(&m, &sv, &sv);
    pEm = *ppEm;
    pEm->Pos_x += sv.vx;
    pEm->Pos_z += sv.vz;
    t = Fall_spd + gravity * pEm->Timer2;
    pEm->Pos_y -= t;
    if (floor_y < pEm->Pos_y)
    {
        pEm->Pos_y = floor_y;
        return t;
    }
    pEm->Timer2++;
    return 0;
}

INT Parts_down(PARTS_W* pP_ptr, SHORT gravity, SHORT Ground, UCHAR ref_index)
{
    SVECTOR Sv;
    USHORT d;
    INT t;

    d = pP_ptr[ref_index].Workm.t[1];
    d = d - pP_ptr->Workm.t[1] + 200;
    if (pP_ptr->Workm.t[1] >= Ground && (pP_ptr->Fall_sts & 0x80))
    {
        pP_ptr->Fall_sts |= 1;
        return 0;
    }
    if (d >= 0x191 || !(pP_ptr->Fall_sts & 0x80))
    {
        Sv.vy = 0x80 - (pP_ptr->Fall_sts & 0x80) * 2;
        Sv.vx = (0x20 - (pP_ptr->Fall_sts & 0x80)) >> 1;
        Sv.vz = 0;
        RotMatrix(&Sv, &ScratchPad->Mat);
        MulMatrix(&pP_ptr->Workm, &ScratchPad->Mat);
    }
    ScratchPad->Mat = GsIDMATRIX;
    RotMatrixY(EM_CUR->Cdir_y, &ScratchPad->Mat);
    ApplyMatrixSV(&ScratchPad->Mat, &pP_ptr->Cdir, &pP_ptr->Cdir);
    pP_ptr->Workm.t[0] += pP_ptr->Cdir.vx;
    pP_ptr->Workm.t[2] += pP_ptr->Cdir.vz;
    pP_ptr->Cdir.vy += gravity * pP_ptr->Fall_cnt;
    pP_ptr->Workm.t[1] -= pP_ptr->Cdir.vy;
    if (pP_ptr->Workm.t[1] >= Ground)
    {
        pP_ptr->Workm.t[1] = -0x15E;
        if (pP_ptr->Fall_sts & 0x80)
        {
            pP_ptr->Workm.t[1] = Ground;
        }
        pP_ptr->Fall_cnt = 3;
        pP_ptr->Fall_sts = 0x80;
        t = MAIN.pEm->Id;
        ScratchWord[0] = t;
        if (t - 2 < 0 || t == 0x11)
        {
            pP_ptr->Be_flg &= 0xFC;
            pP_ptr[1].Be_flg &= 0xFC;
        }
    }
    pP_ptr->Fall_cnt++;
    return 0;
}

INT Em_wander_turn(UINT speed, UCHAR* pTurn_flg, UCHAR* counter, SHORT turn, UCHAR limit)
{
    INT unused[2];  // unused local: reserves stack space
    ENEMY_WORK* pEm;
    ENEMY_WORK* pEm2;
    INT* pTmp;
    INT pRet;
    INT d;
    USHORT ang;
    USHORT cur;
    UCHAR f;
    SHORT a;

    f = *pTurn_flg;
    a = turn;
    if (f & 0x80)
    {
        pEm = G.pEm;
        pEm->Cdir_y += turn - turn * ((f >> 5) & 2);
        if (pEm->Spd_cur * 3 / 4 < speed)
        {
            if (--*counter == 0)
            {
                *pTurn_flg = 0;
                *counter = 0;
            }
        }
        return 1;
    }
    if (speed < G.pEm->Spd_cur * 2 / 3)
    {
        if (limit < ++*counter)
        {
            *pTurn_flg |= 0x80;

            *pTurn_flg |= G.SaveData.Sce_random & 0x40;

            *counter = limit / 6;
        }
    }
    else
    {
        *pTurn_flg = 0;
        *counter = 0;
    }
    pRet = Get_dir_tgt(EM_CUR->Dest_x, MAIN.pEm->Dest_z);
    pTmp = &ScratchWord[7];
    *pTmp = pRet;
    if (a >> 15)
    {
        a = a * -1;
        *pTmp = (pRet + 0x800) & 0xFFF;
    }
    pEm2 = MAIN.pEm;
    ang = *pTmp;
    cur = pEm2->Cdir_y;
    d = (USHORT)(a + (ang - cur)) & 0xFFF;  // truncates to an unsigned halfword
    if (d < a * 2)
    {
        pEm2->Cdir_y = ang;
    }
    else
    {
        pEm2->Cdir_y = cur - a;
        if ((UINT)d < 0x801)  // unsigned conversion (signed operand)
        {
            pEm2->Cdir_y += a << 1;
        }
    }
    return 0;
}

INT Goto00_ck(target, Add_dir)
VECTOR* target;  // parameter
SHORT Add_dir;  // narrow parameter, passed as int
{
    LONG new_var;
    SHORT Dir;
    INT Sa;

    Dir = (Add_dir + (Get_dir_tgt(target->vx, target->vz) - G.pEm->Cdir_y)) & 0xFFF;
    Sa = Dir;
    new_var = Sa;
    Dir = 0;
    if (Sa >= Add_dir * 2)
    {
        if (new_var >= 0x801U)
        {
            return (SHORT)(Add_dir * -1);
        }
        Dir = Add_dir;
        return (SHORT)Add_dir;
    }
    return (SHORT)Dir;
}

VOID Goto00(target, Add_dir)
VECTOR* target;  // parameter
SHORT Add_dir;  // narrow parameter, passed as int
{
    INT unused[2];  // unused local: reserves stack space
    INT r;
    INT* pTmp;
    ENEMY_WORK* pEm;
    USHORT Dir;
    USHORT cur;
    USHORT New_dir;
    INT d;

    r = Get_dir_tgt(target->vx, target->vz);
    pTmp = &ScratchWord[7];
    *pTmp = r;

    if (Add_dir >> 15)
    {
        Add_dir = Add_dir * -1;
        *pTmp = (r + 0x800) & 0xFFF;
    }
    pEm = G.pEm;
    Dir = *pTmp;
    cur = pEm->Cdir_y;
    d = (USHORT)(Add_dir + (Dir - cur)) & 0xFFF;  // truncates to an unsigned halfword
    if (d < Add_dir * 2)
    {
        pEm->Cdir_y = Dir;
    }
    else
    {
        New_dir = cur - Add_dir;
        pEm->Cdir_y = New_dir;
        if ((UINT)d < 0x801)  // unsigned conversion (signed operand)
        {
            pEm->Cdir_y = New_dir + (Add_dir << 1);
        }
    }
}

SHORT Em_wall_turn(INT Max_turn, UCHAR room_hit, SHORT* swerve_angle, UCHAR* swerve_latch)
{
    VECTOR v;
    INT r;
    ENEMY_WORK* e;
    SHORT ang;
    INT q;
    SHORT cur;
    SHORT h;
    INT d;
    INT x;
    USHORT diff;
    SHORT Step_dir;

    v = ScratchPad->Vec;
    r = Get_dir_tgt(v.vx, v.vz);
    Step_dir = Max_turn;
    if (room_hit == 0)
    {
        if (!(*swerve_latch & 0x80))
        {
            *swerve_latch |= 0x80;
        }
    }
    else if (!(*swerve_latch & 0x80))
    {
        if (*swerve_angle == 0)
        {
            *swerve_angle = Get_dir_tgt(v.vx, EM_CUR->Pos_z);
            if (!(MAIN.pEm->Be_flg & EM_BE_SLIDE_AXIS))
            {
                *swerve_angle = Get_dir_tgt(MAIN.pEm->Pos_x, v.vz);
            }
            h = *swerve_angle;
            diff = (Step_dir + (h - MAIN.pEm->Cdir_y)) & 0xFFF;
            *swerve_angle = Step_dir * -1;
            if (diff < 0x801)
            {
                *swerve_angle = Step_dir;
            }
        }

        *swerve_latch = ScratchWord[7];

        e = G.pEm;
        ang = e->Cdir_y;
        do
        {
            q = (ang & 0xFFF) / 1024;
            cur = *swerve_angle;
            d = (ang + cur) & 0xFFF;
        } while (0);
        if (q != d / 1024)
        {
            ScratchWord[2] = cur - d % 1024;
            cur = *swerve_angle;
            if (cur & 0x8000)
            {
                x = e->Cdir_y & 0xFFF;
                ScratchWord[2] = cur + (SHORT)(x % 1024);  // sign-extends the halfword
            }
            return ScratchWord[2];
        }
        return cur;
    }
    if (*swerve_latch & 0x7F)
    {
        (*swerve_latch)--;
        h = *swerve_angle;
        diff = (Step_dir + (h - G.pEm->Cdir_y)) & 0xFFF;
        *swerve_angle = Step_dir * -1;
        if (diff < 0x801)
        {
            *swerve_angle = Step_dir;
        }
        return *swerve_angle;
    }
    *swerve_angle = 0;
    *swerve_latch = 0;
    d = (USHORT)(Step_dir + (r - G.pEm->Cdir_y)) & 0xFFF;  // truncates to an unsigned halfword
    if (Step_dir * 2 >= d)
    {
        return 0;
    }
    if ((UINT)d < 0x801)  // unsigned conversion (signed operand)
    {
        return Step_dir;
    }
    return Step_dir * -1;
}

INT Dir_pos_ck(INT Hani_dir, VECTOR* pP)
{
    VECTOR v;
    INT Dir;

    Dir = (Hani_dir + (Get_dir_tgt(pP->vx, pP->vz) - (USHORT)G.pEm->Cdir_y)) & 0xFFF;  // truncates to an unsigned halfword
    if (((Hani_dir << 0x10) >> 0xF) < Dir)
    {
        return 1;
    }
    v.vx = pP->vx - G.pEm->Pos_x;
    v.vz = pP->vz - G.pEm->Pos_z;
    v.vy = 0;
    return Sca_ck_line(&v, G.pEm->Sikai_cnt) & 0xFF;
}

INT L_pos_ckXZ(SHORT ang, SHORT Ll, VECTOR* pP)
{
    MATRIX m;
    VECTOR d;
    VECTOR r1;
    VECTOR r2;
    VECTOR v = { 2000, 0, 0 };
    INT x;
    INT z;

    d.vx = pP->vx - EM_CUR->Pos_x;
    x = __builtin_abs(d.vx);
    d.vz = pP->vz - MAIN.pEm->Pos_z;
    d.vy = 0;
    z = __builtin_abs(d.vz);
    if (Ll >= x + z)
    {
        m = GsIDMATRIX;
        RotMatrixY(MAIN.pEm->Cdir_y - ang, &m);
        ApplyMatrixLV(&m, &v, &r1);
        RotMatrixY(ang * 2, &m);
        ApplyMatrixLV(&m, &v, &r2);
        OuterProduct0(&r1, &d, &r1);
        OuterProduct0(&r2, &d, &r2);
        return (r1.vy & 0x80000000) < (r2.vy & 0x80000000);
    }
    return 0;
}

UCHAR Oba_ck_em(VOID)
{
    INT unused[4];  // unused local: reserves stack space
    UCHAR r;
    UCHAR n;
    ENEMY_WORK* pEm2;
    UCHAR* flag;
    INT dx;
    INT dz;
    INT d;

    r = 0;
    n = G.Enemy_num;
    pEm2 = MAIN.Enemy;
    while (n != 0)
    {
        if (pEm2->Be_flg != 0)
        {
            n--;
            if (G.pEm != pEm2)
            {
                r |= Oba_ck_hit(pEm2, G.pEm);
            }
        }
        pEm2++;
    }
    flag = &MAIN.Player.Be_flg;
    dx = G_PLAYER->Pos_x - G.Player.Old_x;
    d = __builtin_abs(dx);
    dz = G.Player.Pos_z - (SHORT)G.Player.Old_z;  // sign-extends the halfword
    d += __builtin_abs(dz);
    ScratchWord[0] = d;
    if (d >= 0x1C3 && !(*flag & 2) && (G.Enemy[0].Id == 0xD || G.Enemy[0].Id == 0x12))
    {
        ScratchWord[0] = dx >> 4;
        ScratchWord[1] = dz >> 4;
        G.Player.Pos_x = G.Player.Old_x + ScratchWord[0];
        G.Player.Pos_z = (SHORT)G.Player.Old_z + ScratchWord[1];  // sign-extends the halfword
    }
    return r;
}

VOID Set_base_pos(ENEMY_WORK* pPm)
{
    ENEMY_WORK* pEm;
    ENEMY_WORK* e2;

    pEm = EM_CUR;
    get_null_pos(pEm, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0);
    ScratchPad->Mat = GsIDMATRIX;
    RotMatrixY(MAIN.pEm->Cdir_y, &ScratchPad->Mat);
    ApplyMatrixSV(&ScratchPad->Mat, &ScratchPad->Sv, &ScratchPad->Sv);
    e2 = MAIN.pEm;
    e2->Root_x = e2->Pos_x - ScratchSvU[0];
    e2->Root_z = e2->Pos_z - ScratchSvU[2];
    pPm->Root_x = e2->Root_x;
    pPm->Root_z = e2->Root_z;
}

VOID Null_pos_set(ENEMY_WORK* pEm, KAN_HEAD* Kan, UCHAR* Seq)
{
    get_null_pos(pEm, Kan, Seq, 0);
    ScratchPad->Mat = GsIDMATRIX;
    RotMatrixY(pEm->Cdir_y, &ScratchPad->Mat);
    ApplyMatrixSV(&ScratchPad->Mat, &ScratchPad->Sv, &ScratchPad->Sv);
    pEm->Pos_x = pEm->Root_x + ScratchPad->Sv.vx;
    pEm->Pos_z = pEm->Root_z + ScratchPad->Sv.vz;
}

INT Gacha_ch(VOID)
{
    return G.Padd1_trg & (PAD_TRIANGLE | PAD_CIRCLE | PAD_X | PAD_SQUARE | PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT);
}

INT Cdir_ck2(VECTOR* pT)
{
    VECTOR d;
    VECTOR v = { 5000, 0, 0 };
    ENEMY_WORK* pEm;

    pEm = EM_CUR;
    d.vx = pT->vx - pEm->Pos_x;
    d.vz = pT->vz - pEm->Pos_z;
    ScratchPad->Mat = GsIDMATRIX;
    d.vy = 0;
    RotMatrixY(MAIN.pEm->Cdir_y + 0x400, &ScratchPad->Mat);
    ApplyMatrixLV(&ScratchPad->Mat, &v, &v);
    OuterProduct0(&d, &v, &v);
    return (UINT)v.vy >> 31;  // unsigned conversion (signed operand)
}

INT Cdir_ck(ENEMY_WORK* pT)
{
    return ((pT->Cdir_y - G.pEm->Cdir_y + 0x400) & 0xFFF) < 0x800;
}

VOID Joint_atck_set(PARTS_W* pP_ptr, SCHAR duration, SHORT Fall_spd, SHORT stride)
{
    UCHAR r;
    UCHAR g;
    UCHAR b;

    PARTS_W* pM_ptr;
    ENEMY_WORK* pEm;

    if (!(pP_ptr->Fall_cnt & 0x80))
    {
        r = 0x60;
        g = 0x28;
        b = 0x28;
        if (EM_CUR->Id - 3 < 2U)
        {
            r = 0x18;
            g = 0x30;
            b = 0x18;
        }
        Parts_col_set(pP_ptr, r, g, b);

        pP_ptr->Fall_sts = duration;
        pP_ptr->Free0 = Fall_spd;
        pP_ptr->Free1 = stride;
        if (SYSTEM_FLG_NOW & SYS_MIRROR_MATRIX)
        {
            pEm = MAIN.pEm;

            pM_ptr = (VOID*)((UCHAR*)pP_ptr - (UCHAR*)pEm->pSin_parts_ptr + (UCHAR*)pEm->pM_char_ptr);  // untyped pointer, the callee takes the address as is
            ScratchPtr[6] = pM_ptr;

            Parts_col_set(pM_ptr, r, g, b);

            pM_ptr->Fall_sts = duration;
            pM_ptr->Free0 = Fall_spd;
            pM_ptr->Free1 = stride;
        }
    }
}

VOID Parts_col_set(PARTS_W* pP_ptr, UCHAR r, UCHAR g, UCHAR b)
{
    POLY_GT3* pPoly;
    USHORT i;
    UCHAR flags;
    UCHAR* pPkt;

    flags = pP_ptr->Be_flg;
    if (flags & 1)
    {
        i = 0;
        pP_ptr->Be_flg = (flags & 0xFE) | 0x28;
        pPkt = pP_ptr->pPacket;
        ScratchWord[0] = ((TMD_MEM*)pP_ptr->pTmd)->n_poly * 2;  // reinterprets the memory as TMD_MEM
        for (pPoly = (POLY_GT3*)(pPkt + 8); i < ScratchWord[0]; pPoly++, i++)  // reinterprets the memory as POLY_GT3
        {
            SetPolyGT3(pPoly);
            pPoly->r2 = r;
            pPoly->r1 = r;
            pPoly->r0 = r;
            pPoly->g2 = g;
            pPoly->g1 = g;
            pPoly->g0 = g;
            pPoly->b2 = b;
            pPoly->b1 = b;
            pPoly->b0 = b;
        }
    }
}

VOID Model_color_plus(PARTS_W* pMw, UINT Col0, UINT Pluscolor_1, UINT Pluscolor_2)
{
    PARTS_W* pM_ptr;
    GT3_COL_REC* pPrim;
    USHORT i;
    TMD_MEM* pTmd;

    pMw->Be_flg |= 0x80;
    pPrim = pMw->pPacket;  // reinterprets the memory as GT3_COL_REC
    pTmd = (TMD_MEM*)pMw->pTmd;  // reinterprets the memory as TMD_MEM
    ScratchWord[0] = pTmd->n_poly * 2;
    for (i = 0; i < ScratchWord[0]; i++)
    {
        {
            UINT t = pPrim->c0 & 0xFF000000;
            pPrim->c0 = (Col0 & 0xFFFFFF) | t;
        }
        {
            UINT t = pPrim->c1 & 0xFF000000;
            pPrim->c1 = (Pluscolor_1 & 0xFFFFFF) | t;
        }
        {
            UINT t = pPrim->c2 & 0xFF000000;
            pPrim->c2 = (Pluscolor_2 & 0xFFFFFF) | t;
        }

        pPrim++;
    }
    if (G.System_flg & SYS_MIRROR_MATRIX)
    {
        do
        {
            pM_ptr = (PARTS_W*)((UCHAR*)pMw - (UCHAR*)G.pEm->pSin_parts_ptr + (UCHAR*)G.pEm->pM_char_ptr);  // reinterprets the memory as PARTS_W
        } while (0);
        ScratchPtr[6] = pM_ptr;
        pM_ptr->Be_flg |= 0x80;
        pPrim = pM_ptr->pPacket;  // reinterprets the memory as GT3_COL_REC
        pTmd = (TMD_MEM*)pM_ptr->pTmd;  // reinterprets the memory as TMD_MEM
        ScratchWord[0] = pTmd->n_poly * 2;
        for (i = 0; i < ScratchWord[0]; i++)
        {
            {
                UINT t = pPrim->c0 & 0xFF000000;
                pPrim->c0 = (Col0 & 0xFFFFFF) | t;
            }
            {
                UINT t = pPrim->c1 & 0xFF000000;
                pPrim->c1 = (Pluscolor_1 & 0xFFFFFF) | t;
            }
            {
                UINT t = pPrim->c2 & 0xFF000000;
                pPrim->c2 = (Pluscolor_2 & 0xFFFFFF) | t;
            }

            pPrim++;
        }
    }
}

VOID Joint_path_anim_step(PARTS_W* pP_ptr)
{
    MATRIX m;
    MATRIX lm;
    LONG p;
    LONG flag;
    TMD_MEM* pTmd;
    TMD_GT3_REC* pFace;
    GT3_BUF* pPrim;
    USHORT* v;
    INT a;
    INT b;
    UINT z;
    UCHAR t;
    UCHAR* base;

    pTmd = (TMD_MEM*)pP_ptr->pTmd;  // reinterprets the memory as TMD_MEM
    base = pP_ptr->pPacket;
    pFace = (TMD_GT3_REC*)pTmd->prim_top;  // reinterprets the memory as TMD_GT3_REC
    lm = GsIDMATRIX;
    ScratchWord[1] = pTmd->n_poly;
    t = pP_ptr->Fall_cnt;

    ScratchWord[0] = t;
    ScratchWord[2] = pP_ptr->Fall_sts - t;

    pPrim = (GT3_BUF*)(G.Cbuf_id * sizeof(POLY_GT3) + (INT)base + 8);  // reinterprets the memory as GT3_BUF

    ScratchWord[3] = pP_ptr->Free1;

    pP_ptr->Workm.t[1] += pP_ptr->Free0 * t;

    if (pP_ptr->Workm.t[1] > 10000)
    {
        pP_ptr->Workm.t[1] = 10000;
    }
    if (Hit_ck_point4(MATRIX_POS_PTR(&pP_ptr->Workm), G.Cccut) != 0)
    {
        GsMulCoord0(&GsWSMATRIX, &pP_ptr->Workm, &m);
        {
            INT last = ScratchWord[1] - 1;
            pPrim += last;
        }
        pFace += ScratchWord[1] - 1;
        while (--ScratchWord[1] != -1)
        {
            v = (USHORT*)(pFace->a * sizeof(SVECTOR) + pTmd->ver_top);  // halfword view of the same memory
            if (ScratchWord[1] % ScratchWord[3] == 0)
            {
                a = ScratchWord[0];
                b = ScratchWord[2];

                lm.t[0] = ((SHORT)v[0] >> 1) * a + ((SHORT)v[0] >> 4) * b;  // sign-extends the halfword
                lm.t[1] = ((SHORT)v[1] >> 1) * a + ((SHORT)v[1] >> 4) * b;  // sign-extends the halfword
                lm.t[2] = ((SHORT)v[2] >> 1) * a + ((SHORT)v[2] >> 4) * b;  // sign-extends the halfword

                GsMulCoord0(&m, &lm, &ScratchPad->Mat);
                SetTransMatrix(&ScratchPad->Mat);
                SetRotMatrix(&ScratchPad->Mat);
            }
            z = RotAverage3((SVECTOR*)v, (SVECTOR*)(pFace->b * sizeof(SVECTOR) + pTmd->ver_top),  // reinterprets the memory as SVECTOR
                (SVECTOR*)(pFace->c * sizeof(SVECTOR) + pTmd->ver_top),  // reinterprets the memory as SVECTOR
                (LONG*)&pPrim->p.x0, (LONG*)&pPrim->p.x1, (LONG*)&pPrim->p.x2, &p, &flag);  // word view of the same memory
            if (z >= 0xFF0)
            {
                z = 0xFF0;
            }
            AddPrim(MAIN.Ot1[G.Cbuf_id].org + (z >> 4), pPrim--);
            pFace--;
        }
    }
    if (pP_ptr->Fall_sts < pP_ptr->Fall_cnt++)
    {
        pP_ptr->Fall_cnt = 0x80;
        pP_ptr->Be_flg &= 0xDE;
    }
}

VOID Om_vtx_rnd(OBJ_MODEL_WORK* obj)
{
    TMD_MEM* t;
    LONG n;
    SVECTOR* base;

    t = obj->pTmd;
    n = t->n_ver;
    base = *(SVECTOR**)t;  // reinterprets the memory as SVECTOR
    while (n-- != 0)
    {
        if (!(rand() & 0xF))
        {
            base->vy += 0x1E;
        }
        base++;
    }
}

VOID Parts_uv_scrl(PARTS_W* pP_ptr)
{
    LONG n;
    INT i;
    POLY_GT3* pPoly;
    INT j;

    i = 0;
    n = ((TMD_MEM*)pP_ptr->pTmd)->n_poly;  // reinterprets the memory as TMD_MEM
    pPoly = (POLY_GT3*)pP_ptr->pPacket + G.Cbuf_id;  // the packet buffer is read as POLY_GT3 records
    j = 0;
    while (n-- != 0)
    {
        if (i == Scrl_prim_tbl[j])
        {
            j++;
            ((UCHAR*)pPoly)[0x14] += 2;  // byte view of the same memory
            ((UCHAR*)pPoly)[0x20] += 2;  // byte view of the same memory
            ((UCHAR*)pPoly)[0x2C] += 2;  // byte view of the same memory
        }
        i++;
        pPoly += 2;
    }
}

UCHAR Sikai_ck(VECTOR* pTa)
{
    VECTOR v;
    SHORT* header;
    UCHAR r;

    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vx = 0;
    header = G.pRoom->pSca;
    r = Sca_get_area(pTa, &ScratchPad->Sv, header[0], header[1]);
    v.vx = pTa->vx - G.pEm->Pos_x;
    v.vy = pTa->vy - G.pEm->Pos_y;
    v.vz = pTa->vz - G.pEm->Pos_z;
    return ScratchWord[1] = Sca_ck_line(&v, r);
}

INT Em_line_ck(VECTOR* target)
{
    VECTOR delta;
    UCHAR i;
    UCHAR r;

    delta.vx = target->vx - G.pEm->Pos_x;
    r = 0;
    delta.vy = target->vy - G.pEm->Pos_y;
    i = 3;
    delta.vz = target->vz - G.pEm->Pos_z;
    do
    {
        r |= Sca_ck_line(&delta, i);
    } while (i--);
    return r;
}

VOID Add_speedXZ(muki)
SHORT muki;  // narrow parameter, passed as int
{
    ENEMY_WORK* pEm;

    ScratchSvU[0] = EM_CUR->Spd_cur;
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Mat = GsIDMATRIX;
    RotMatrixY(MAIN.pEm->Cdir_y + muki, &ScratchPad->Mat);
    ApplyMatrixSV(&ScratchPad->Mat, &ScratchPad->Sv, (SVECTOR*)&MAIN.pEm->Spd_x);  // reinterprets the memory as SVECTOR
    pEm = MAIN.pEm;
    pEm->Pos_x += pEm->Spd_x;
    pEm->Pos_y += pEm->Spd_y;
    pEm->Pos_z += pEm->Spd_z;
}

VOID Set_enemy_work(UCHAR n, INT unused_arg, UCHAR idx, ENEMY_WORK** out)
{
    ENEMY_WORK* pEm;

    ScratchPtr[0] = (UCHAR*)EM_CUR->pSin_parts_ptr->pTmd + idx * sizeof(TMD_MEM);
    *out = FREE_WORK_CUR;
    VIEW(ENEMY_WORK*, MAIN.pFree_work) += n;  // ENEMY_WORK** view of the field (a plain access changes the code)
    pEm = *out;
    ScratchPad->Mat = *(MATRIX*)MAIN.pEm->M;  // reinterprets the memory as MATRIX
    do
    {
        *pEm = *G.pEm;
        *(MATRIX*)pEm->M = ScratchPad->Mat;  // reinterprets the memory as MATRIX
        pEm->pTmd = ScratchPtr[0];
        pEm->Cdir_y += n << 8;
        pEm->pPacket = (VOID*)FREE_WORK_INT;
        GsLinkObject5((ULONG)pEm->pTmd, (GsDOBJ5*)&pEm->Attribute, 0);  // the embedded GsDOBJ5 starts at Attribute
        MAIN.pFree_work = GsPresetObject((GsDOBJ5*)&pEm->Attribute, pEm->pPacket);  // the embedded GsDOBJ5 starts at Attribute
        pEm->Routine_0 = 0;
        pEm->Routine_3 = (rand() & 3) == 0;
        pEm++;
    } while (--n);
}

VOID Em_web_proc(INT count)
{
    ENEMY_WORK* save;
    WEB_WORK* p1;
    ENEMY_WORK* p2;
    WEB_WORK* p3;
    WEB_WORK* p4;
    WEB_WORK* p5;
    ENEMY_WORK* pEm6;
    WEB_WORK* pWeb7;
    ENEMY_WORK* pEm8;
    ENEMY_WORK* pEm9;
    WEB_WORK* p10;
    WEB_WORK* p11;
    WEB_WORK* p12;
    ENEMY_WORK* p13;
    WEB_WORK* p14;
    ENEMY_WORK* p15;
    ENEMY_WORK* p16;
    ENEMY_WORK* p17;
    TMD_MEM* pTmd;
    INT r;
    SHORT t;
    UCHAR k;
    USHORT c;

    save = EM_CUR;
    MAIN_PEM(UCHAR) = (UCHAR*)save->pTgt;  // pEm is set to the saved target and read as bytes (see MAIN_PEM)
    do
    {
        EM_CUR->Zone_in_flg = Hit_ck_point4(POS_VEC(EM_CUR), G.Cccut);
        p1 = (WEB_WORK*)EM_CUR;  // reinterprets the memory as WEB_WORK
        if (p1->Be_flg == 0)
        {
            if (p1->Zone_in_flg != 0)
            {
                MulKage(POS_VEC(p1), &p1->Kage_work, 0, p1->Cdir_y);
            }
            EM_CUR++;
            continue;
        }
        else
        {
            if (MAIN.Stop_flg & STOP_ENEMY_MOVE)
            {
                ScratchWord[0] = __builtin_abs(MAIN.Player.Pos_x - p1->Pos_x) + __builtin_abs(MAIN.Player.Pos_z - p1->Pos_z);
                if (p1->Se_wait != 0)
                {
                    p1->Se_wait--;
                }
                if (ScratchWord[0] < 0x190 && MAIN.Player.Spd_cur != 0)
                {
                    p2 = EM_CUR;
                    if (p2->Routine_0 != 5)
                    {
                        VECTOR* vec = &ScratchPad->Vec;

                        p2->Be_flg = 0;
                        ScratchWord[7] = 0xFF55DF;
                        ScratchPad->Sv.vz = 0;
                        ScratchPad->Sv.vy = 0;
                        ScratchPad->Sv.vx = 0;
                        Kage_work_init(&ScratchPad->Sv, &EM_CUR->Kage_work, 300, 300);
                        ScratchPad->Vec.vz = 0;
                        ScratchPad->Vec.vy = 0;
                        vec->vx = 0;

                        Esp_call(0, 10, 0, MATRIX_OF(EM_CUR), vec, 0);

                        if (((WEB_WORK*)EM_CUR)->Se_wait == 0)  // reinterprets the memory as WEB_WORK
                        {
                            Snd_se_enem(6, 0);
                            ((WEB_WORK*)EM_CUR)->Se_wait = 0x2D;  // reinterprets the memory as WEB_WORK

                            Pad_vib_set0(1, 0);
                            Pad_vib_set1(4, 0x5A, 0);
                        }
                        EM_CUR->Routine_0 = 6;
                    }
                }
                switch (G.pEm->Routine_0)
                {
                case 0:
                    EM_CUR->Timer0 = rand() & 0x3F;
                    ((WEB_WORK*)EM_CUR)->Dir_add = rand() & 0x3F;  // reinterprets the memory as WEB_WORK
                    r = rand() & 1;
                    p3 = (WEB_WORK*)EM_CUR;  // reinterprets the memory as WEB_WORK
                    p3->Routine_0 = 1;
                    p3->Dir_add = -p3->Dir_add * r;
                    r = rand();
                    p4 = (WEB_WORK*)EM_CUR;  // reinterprets the memory as WEB_WORK
                    p4->Spd_cur = (r & 0x1F) + 100;
                    p4->swing = 0x80;
                case 1:
                    t = Goto00_ck(&PL_POS_VEC, 0x3E);
                    ScratchWord[7] = t;
                    if (G.Player.Spd_cur != 0 && G.pEm->Routine_3 != 0)
                    {
                        ScratchWord[7] = -t;
                    }
                    p5 = (WEB_WORK*)G.pEm;  // reinterprets the memory as WEB_WORK

                    p5->Cdir_y += ScratchHalf[14] + p5->Dir_add;

                    p5->Cdir_y += p5->swing;
                    p5->swing = -p5->swing;
                    c = p5->Timer0;
                    p5->Timer0 = c - 1;
                    if (c == 0)
                    {
                        k = 2;
                        if (ScratchWord[7] == 0)
                        {
                            k = 4;
                        }
                        p5->Routine_0 = k;
                    }
                    break;
                case 2:
                    r = rand();
                    pEm6 = EM_CUR;
                    pEm6->Timer0 = r & 0x1F;
                    pEm6->Routine_0 = 3;
                    pWeb7 = (WEB_WORK*)EM_CUR;  // reinterprets the memory as WEB_WORK
                    pWeb7->Spd_cur = 0;
                    pWeb7->swing = 0;
                case 3:
                    pEm8 = G.pEm;
                    c = pEm8->Timer0;
                    {
                        INT left = c - 1;
                        pEm8->Timer0 = left;
                    }
                    if (c == 0)
                    {
                        pEm8->Routine_0 = 0;
                    }
                    break;
                case 4:
                    r = rand();
                    pEm9 = EM_CUR;
                    pEm9->Spd_cur = (r & 0x1F) + 0x8C;
                    pEm9->Routine_0 = 5;
                    EM_CUR->Timer2 = 0;
                    r = rand();
                    p10 = (WEB_WORK*)EM_CUR;  // reinterprets the memory as WEB_WORK
                    p10->Dir_add = (r & 0x7F) + 100;
                    p10->Hit_done = 0;
                    p10->swing = 0;
                case 5:
                    p11 = (WEB_WORK*)EM_CUR;  // reinterprets the memory as WEB_WORK
                    if (Em_fall_move(p11->Spd_cur, (SHORT)p11->Dir_add, -30, 0) != 0)  // sign-extends the halfword
                    {
                        EM_CUR->Routine_0 = 2;
                    }
                    if (ScratchWord[0] < 0x320)
                    {
                        p12 = (WEB_WORK*)EM_CUR;  // reinterprets the memory as WEB_WORK
                        if (p12->Hit_done == 0)
                        {
                            VECTOR* vec = &ScratchPad->Vec;

                            p12->Hit_done = 1;
                            p12->Cdir_y += 0x400;
                            ScratchPad->Vec.vz = 0;
                            ScratchPad->Vec.vy = 0;
                            vec->vx = 0;

                            Esp_call(0, 0, 0, MATRIX_OF(p12), vec, 0);

                            EM_CUR->Spd_cur -= 0x3C;

                            G_PLAYER->Life -= 3;

                            Pad_vib_set0(3, 0);

                            if (G.Player.Life < 0)
                            {
                                G.Player.Life = 1;
                            }
                            G.Player.Routine_2 = 100;
                            G.Player.Damage_no = 1;
                        }
                    }
                    break;
                }
                p13 = EM_CUR;
                RotMatrix((SVECTOR*)&p13->Cdir_x, (MATRIX*)&p13->M);  // reinterprets the memory as SVECTOR
                p14 = (WEB_WORK*)EM_CUR;  // reinterprets the memory as WEB_WORK
                if (p14->Routine_0 < 6)
                {
                    Add_speedXZ(p14->swing);
                }
            }
            p15 = EM_CUR;
            Sca_ck_hit(POS_VEC(p15), p15->pAt_tbl->At_w);
            p16 = EM_CUR;
            p16->Old_x = p16->Pos_x;
            p16->Old_y = p16->Pos_y;
            p16->Old_z = p16->Pos_z;
            GsMulCoord0(&GsWSMATRIX, (MATRIX*)&p16->M, &ScratchPad->Mat);  // reinterprets the memory as MATRIX
            ScratchPtr[14] = EM_CUR->pTmd;
            GsSetLsMatrix(&ScratchPad->Mat);
            p17 = EM_CUR;
            if (p17->Zone_in_flg != 0)
            {
                pTmd = ScratchPtr[14];
                GsPrstTG3NL(pTmd->prim_top, pTmd->ver_top, pTmd->nor_top,
                    (UCHAR*)p17->pPacket + CBUF_ID_CUR * sizeof(POLY_GT3) + 8, pTmd->n_poly, 4,
                    &MAIN.Ot1[MAIN.Cbuf_id]);
            }
        }
        EM_CUR++;
    } while (--count & 0xFF);
    G.pEm = save;
}

INT Em_sikai_proc(VOID)
{
    INT Out_flg;
    UCHAR Sikai2;
    UCHAR Next_sikai;
    UCHAR Sikai;
    ENEMY_WORK* e;
    ENEMY_WORK* e2;
    ENEMY_WORK* e3;
    ENEMY_WORK* e4;

    e = EM_CUR;
    Sikai = e->Sikai_cnt;
    if ((Sikai & 0x1F) < 4U)
    {
        Out_flg = Dir_pos_ck(0x5E8, &PL_POS_VEC);
        e2 = MAIN.pEm;
        e2->Sikai_cnt |= Out_flg << 5;
        e3 = MAIN.pEm;
        Sikai2 = e3->Sikai_cnt;
        if ((Sikai2 & 0x1F) == 3)
        {
            if (!(Sikai2 & 0x20))
            {
                e3->Dest_x = G.Player.Pos_x;
                e3->Dest_z = G.Player.Pos_z;
                e3->Sikai_cnt++;
                e3 = MAIN.pEm;
                e3->Sikai_cnt &= 0xDF;
                return 1;
            }
            e3->Sikai_cnt = Sikai2 + 1;
            e3 = MAIN.pEm;
            e3->Sikai_cnt &= 0xDF;
            return 0;
        }
        e3->Sikai_cnt = Sikai2 + 1;
        return 2;
    }
    Next_sikai = Sikai + 1;
    e->Sikai_cnt = Next_sikai;
    if ((Next_sikai & 0x1F) >= 0x10U)
    {
        e4 = MAIN.pEm;
        e4->Sikai_cnt &= 0xC0;
    }
    return 2;
}

INT L_pos_ckXZM(MATRIX* pM, VECTOR* pP, INT Ll, VECTOR* pSv)
{
    MATRIX m;
    USHORT d;

    ScratchPad->Mat = GsIDMATRIX;
    ScratchPad->Mat.t[0] = pP->vx;
    ScratchPad->Mat.t[1] = pP->vy;
    ScratchPad->Mat.t[2] = pP->vz;
    GsMulCoord0(pM, &ScratchPad->Mat, &m);
    d = m.t[0] - Ll;
    d = pSv->vx - d;
    if (d <= (SHORT)Ll * 2)  // sign-extends the halfword
    {
        d = m.t[2] - Ll;
        d = pSv->vz - d;
        return d <= (SHORT)Ll * 2;  // sign-extends the halfword
    }
    return 0;
}

UCHAR Rnd(VOID)
{
    USHORT cur;
    INT new_var;
    UINT shifted;

    cur = SCE_RANDOM_CUR; // unsigned view of the SHORT: plain loads and stores change codegen
    shifted = MAIN.SaveData.Sce_random >> 1;
    MAIN.SaveData.Sce_random = shifted;
    new_var = 8;
    if (((UINT)(cur & 0x202) >> new_var) == (cur & 2))  // unsigned conversion (signed operand)
    {
        MAIN.SaveData.Sce_random = shifted | 0x8000;
    }
    return cur = MAIN.SaveData.Sce_random;
}

// indices of the primitives whose texture coordinates are scrolled
USHORT Scrl_prim_tbl[8] =
{
    0x2A,   // 0 //
    0x60,   // 1 //
    0x61,   // 2 //
    0x6B,   // 3 //
    0x6C,   // 4 //
    0x6E,   // 5 //
    0x74,   // 6 //
    0x0,    // 7 //
};
