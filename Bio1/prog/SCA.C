/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SCA.C
 */

#include "SCA.H"
#include "MAIN.H"
#include "FLAG.H"

USHORT* pSca_last_grp;            // start of the boundary group last tested by Sca_ck_line
SCA_RTN_FN Sca_rtn_tbl[6];   // response callback per boundary shape type

VOID Sca_rtn_init(VOID)
{
    INT* pHdr;
    SCA_BOX_REC* pT_xz;
    INT n;
    INT t;
    INT c;

    pHdr = G.pRoom->pSca;
    pT_xz = (SCA_BOX_REC*)((SCA_HEAD*)pHdr + 1);  // reinterprets the memory as SCA_BOX_REC
    pHdr++;
    n = *pHdr;
    *pHdr++ = (INT)pT_xz;  // real width or sign conversion
    ScratchWord[0] = 0;
    do
    {
        t = *pHdr;
        *pHdr++ = (INT)(pT_xz + n);  // real width or sign conversion
        c = ScratchWord[0] + 1;
        ScratchWord[0] = c;
        n += t;
    } while (c < 4);
    Sca_rtn_tbl[1] = (SCA_RTN_FN)Sca_hit_box;
    Sca_rtn_tbl[3] = &Sca_hit_circle;
    Sca_rtn_tbl[4] = (SCA_RTN_FN)Sca_hit_flg;
    Sca_rtn_tbl[5] = (SCA_RTN_FN)Sca_hit_box;
}

INT Sca_ck_type(SVECTOR* ofs, SCA_BOX_REC* box, USHORT radius)
{
    if (Sca_ck_out(ofs, box->Max_x + radius, box->Max_z + radius, box->Min_x - radius, box->Min_z - radius) == 0)
    {
        return box->Id;
    }
    return -1;
}

SHORT Sca_ck_flg(SVECTOR* ofs, SCA_BOX_REC* box, USHORT radius)
{
    if (Sca_ck_out(ofs, box->Max_x + radius, box->Max_z + radius, box->Min_x - radius, box->Min_z - radius) == 0)
    {
        return box->Type & 0xFF00;
    }
    return -1;
}

UINT Sca_get_area(VECTOR* pos, SVECTOR* ofs, INT Sx, INT Sz)
{
    return (((pos->vz + ofs->vz - Sz) & 0x80000000) | (((pos->vx + ofs->vx - Sx) & 0x80000000) >> 1)) >> 0x1E;
}

INT Sca_ck_out(SVECTOR* ofs, INT x_hi, INT z_hi, INT x_lo, INT z_lo)
{
    INT y;
    INT x;
    UINT r;

    y = ScratchPad->Vec.vz + ofs->vz;
    r = ((z_hi - y) & 0x80000000) | (((y - z_lo) & 0x80000000) >> 1);
    x = ScratchPad->Vec.vx + ofs->vx;
    r |= ((x_hi - x) & 0x80000000) >> 2;
    return r | (((x - x_lo) & 0x80000000) >> 3);
}

INT Sca_ck_hit(VECTOR* pPos, USHORT R)
{
    SVECTOR sv = { 0, 0, 0, 0 };
    ENEMY_WORK* pEm;
    UINT* pSca_data;
    UINT* pT_xz_end;
    SCA_BOX_REC* pT_xz;
    USHORT* h;
    SHORT r;
    SHORT Hit_flg;
    SHORT res;
    USHORT lo;
    USHORT hi;
    UINT b;
    INT* flag;
    INT dx;
    INT dz;

    Hit_flg = 0;
    res = 0;
    EM_CUR->Sca_info &= 0xF7;
    if (MAIN.pEm->Be_flg & EM_BE_NO_WALL)
    {
        return 0;
    }
    r = Sca_get_area(pPos, &sv, VIEW(SHORT*, ROOM_CUR->pSca)[0], VIEW(SHORT*, MAIN.pRoom->pSca)[1]);  // ROOM_WORK** view of the field (a plain access changes the code)
    {
        INT ofs = r * 4 + 4;
        pSca_data = (UINT*)((UCHAR*)MAIN.pRoom->pSca + ofs);
    }

    ScratchPad->Sv.vx = ScratchPad->Sv.vy = ScratchPad->Sv.vz = 0;
    ScratchPad->Vec = *pPos;
    ScratchPad->tmp1C = 0;
    pT_xz_end = pSca_data + 1;
    pT_xz = (SCA_BOX_REC*)*pSca_data;  // reinterprets the memory as SCA_BOX_REC
    if ((UINT)pT_xz < *pT_xz_end)  // unsigned conversion (signed operand)
    {
        h = &pT_xz->Type;
        do
        {
            if ((MAIN.pEm->Sca_info & 4) && !(*h & 0x100))
            {
                lo = *h & 0xFF;
                hi = h[-1];
                h += 6;
                pT_xz++;
                res = ((SHORT)(((hi & 0x7F00) >> 8) * 1000 + lo * 100) * -(hi & 0x8000) >> 15) | 1;  // sign-extends the halfword
            }
            else
            {
                if ((UCHAR)h[-1] != 5 || !(MAIN.pEm->Sca_info & 0x10))  // truncates to a byte
                {
                    r = Sca_ck_type(&ScratchPad->Sv, pT_xz, R);
                    if (r != -1)
                    {
                        pEm = MAIN.pEm;
                        Sca_rtn_tbl[r](pT_xz, &pEm->Pos_x, &pEm->Old_x);
                        Hit_flg |= (*h & 0x300) >> 8;
                    }
                }
                h += 6;
                pT_xz++;
            }
        } while ((UINT)pT_xz < *pT_xz_end);  // unsigned conversion (signed operand)
    }
    if (Hit_flg == 0)
    {
        dx = EM_CUR->Pos_x - MAIN.pEm->Old_x;
        dz = MAIN.pEm->Pos_z - MAIN.pEm->Old_z;
        ScratchWord[6] = SquareRoot0(dx * dx + dz * dz);
        EM_CUR->Old_x = MAIN.pEm->Pos_x;
        MAIN.pEm->Old_y = MAIN.pEm->Pos_y;
        MAIN.pEm->Old_z = MAIN.pEm->Pos_z;

        if (res != 0)
        {
            ScratchPad->tmp1C = res;
            return 3;
        }
        return 0;
    }
    ScratchPad->Vec = MAIN_EM_POS_VEC;
    ScratchPad->tmp1C = 0;
    Hit_flg = 0;
    pT_xz = (SCA_BOX_REC*)pT_xz_end[-1];  // reinterprets the memory as SCA_BOX_REC
    while ((UINT)pT_xz < *pT_xz_end)  // unsigned conversion (signed operand)
    {
        if (pT_xz->Id == 4)
        {
            goto next;
        }
        if (pT_xz->Id == 5 && (MAIN.pEm->Sca_info & 0x10))
        {
            pT_xz++;
            continue;
        }
        if ((MAIN.pEm->Sca_info & 4) && !(pT_xz->Type & 0x100))
        {
            pT_xz++;
            continue;
        }
        r = Sca_ck_type(&ScratchPad->Sv, pT_xz, R);
        if (r & 0x8000)
        {
            goto next;
        }
        {
            INT type = pT_xz->Type & 0x300;
            b = (UINT)type >> 8;  // unsigned conversion (signed operand)
        }
        Hit_flg |= b;
        if (r == 3 && b != 0)
        {
            ScratchPad->tmp1C = 1;
        }
    next:
        pT_xz++;
    }
    flag = &ScratchPad->tmp1C;
    if (*flag != 0)
    {
        dx = EM_CUR->Pos_x - MAIN.pEm->Old_x;
        dz = MAIN.pEm->Pos_z - MAIN.pEm->Old_z;
        ScratchWord[6] = SquareRoot0(dx * dx + dz * dz);
        EM_CUR->Old_x = MAIN.pEm->Pos_x;
        MAIN.pEm->Old_y = MAIN.pEm->Pos_y;
        MAIN.pEm->Old_z = MAIN.pEm->Pos_z;

        if (res != 0)
        {
            *flag = res;
            return 3;
        }
        return 0;
    }
    if (Hit_flg == 0)
    {
        dx = EM_CUR->Pos_x - MAIN.pEm->Old_x;
        dz = MAIN.pEm->Pos_z - MAIN.pEm->Old_z;
        ScratchWord[6] = SquareRoot0(dx * dx + dz * dz);
        EM_CUR->Old_x = MAIN.pEm->Pos_x;
        MAIN.pEm->Old_y = MAIN.pEm->Pos_y;
        MAIN.pEm->Old_z = MAIN.pEm->Pos_z;

        if (res != 0)
        {
            *flag = res;
            return 3;
        }
        return 1;
    }
    ScratchWord[6] = 0;
    EM_CUR->Pos_x = MAIN.pEm->Old_x;
    MAIN.pEm->Old_y = MAIN.pEm->Pos_y;
    MAIN.pEm->Pos_z = MAIN.pEm->Old_z;

    if (res != 0)
    {
        *flag = res;
        return 3;
    }
    return 2;
}

UCHAR Sca_ck_point2(SVECTOR* pSv0, SVECTOR* pSv1)
{
    SCA_BOX_REC** tbl[2];
    SHORT hit[2] = { 0, 0 };
    VECTOR v[2];
    VECTOR save;
    SVECTOR sv[2];
    INT Rsv[2];    // unused local: reserves stack space
    SCA_BOX_REC* pT_xz;
    SCA_BOX_REC** ppT_xz;
    SCA_BOX_REC** ppEnd;
    SHORT r;
    INT c;
    UCHAR i;

    if (EM_CUR->Be_flg & EM_BE_NO_WALL)
    {
        return 0;
    }

    ScratchPad->Mat = GsIDMATRIX;
    RotMatrixY(MAIN.pEm->Cdir_y, &ScratchPad->Mat);
    i = 1;
    ApplyMatrix(&ScratchPad->Mat, pSv0, &v[0]);
    ApplyMatrix(&ScratchPad->Mat, pSv1, &v[1]);
    ScratchPad->Mat = GsIDMATRIX;
    RotMatrixY(MAIN.pEm->Old_dir, &ScratchPad->Mat);
    ApplyMatrixSV(&ScratchPad->Mat, pSv0, &sv[0]);
    ApplyMatrixSV(&ScratchPad->Mat, pSv1, &sv[1]);
    sv[0].vx += MAIN.pEm->Old_x;
    sv[0].vz += MAIN.pEm->Old_z;
    sv[1].vx += MAIN.pEm->Old_x;
    sv[1].vz += MAIN.pEm->Old_z;
    save = v[1];
    do
    {
        v[i].vx += G_PEM_BYTE(ENEMY_WORK)->Pos_x;  // G_PEM_BYTE: a plain access changes the code
        v[i].vz += G.pEm->Pos_z;
        ScratchPad->Sv.vx = ScratchPad->Sv.vy = ScratchPad->Sv.vz = 0;
        ScratchWord[0] = Sca_get_area(&v[i], &ScratchPad->Sv,
            VIEW(SHORT*, ROOM_CUR->pSca)[0],
            VIEW(SHORT*, MAIN.pRoom->pSca)[1]);  // ROOM_WORK** view of the field (a plain access changes the code)
        tbl[i] = (VOID*)(ScratchWord[0] * 4 + (UCHAR*)ROOM_CUR->pSca + 8);

        ScratchPad->Vec = v[i];
        pT_xz = tbl[i][-1];
        while (pT_xz < tbl[i][0])
        {
            c = pT_xz->Id;
            if (c == 4 || (UINT)(c - 4) < 2)  // unsigned conversion (signed operand)
            {
                pT_xz++;
                continue;
            }
            r = Sca_ck_type(&ScratchPad->Sv, pT_xz, G.pEm->pAt_tbl->At_w);
            ScratchWord[0] = r;
            if (r != -1)
            {
                Sca_rtn_tbl[r](pT_xz, &v[i], &sv[i]);
                hit[i] |= (pT_xz->Type & 0x300) >> 8;
            }
            pT_xz++;
        }
    } while (i-- != 0);
    if (hit[0] == 0)
    {
        G_PEM_BYTE(ENEMY_WORK)->Old_x = EM_NOW->Pos_x;  // G_PEM_BYTE: a plain access changes the code
        EM_NOW->Old_y = EM_NOW->Pos_y;
        EM_NOW->Old_z = EM_NOW->Pos_z;
        EM_NOW->Old_dir = EM_NOW->Cdir_y;
        return hit[1];
    }
    save.vx += G.pEm->Pos_x;
    save.vz += G.pEm->Pos_z;
    do
    {
        ppEnd = tbl[1];
    } while (0);
    ScratchPad->Vec = v[1];
    pT_xz = ppEnd[-1];
    ppT_xz = ppEnd;
    goto test;
loop:
    c = pT_xz->Id;
    if (c != 4 && (UINT)(c - 4) >= 2)  // unsigned conversion (signed operand)
    {
        if (!(Sca_ck_type(&ScratchPad->Sv, pT_xz, G.pEm->pAt_tbl->At_w) & 0x8000))
        {
            hit[1] |= (pT_xz->Type & 0x300) >> 8;
        }
    }
    pT_xz++;
    ppT_xz = tbl[1];
test:
    if (pT_xz < *ppT_xz)
    {
        goto loop;
    }
    if (hit[1] != 0)
    {
        G_PEM_BYTE(ENEMY_WORK)->Pos_x = EM_NOW->Old_x;  // G_PEM_BYTE: a plain access changes the code
        EM_NOW->Old_y = EM_NOW->Pos_y;
        EM_NOW->Pos_z = EM_NOW->Old_z;
        EM_NOW->Cdir_y = EM_NOW->Old_dir;
        return 0x80;
    }
    G_PEM_BYTE(ENEMY_WORK)->Old_x = EM_NOW->Pos_x;  // G_PEM_BYTE: a plain access changes the code
    EM_NOW->Old_y = EM_NOW->Pos_y;
    EM_NOW->Old_z = EM_NOW->Pos_z;
    EM_NOW->Old_dir = EM_NOW->Cdir_y;
    return 1;
}

USHORT Sca_ck_pos(VECTOR* pos, SVECTOR* ofs)
{
    SHORT* pSplit;
    SCA_BOX_REC** ppT_xz;
    SCA_BOX_REC* pT_xz;
    SCA_BOX_REC** ppEnd;
    SHORT r;
    SHORT res;
    INT bits;
    INT Acc;

    pSplit = ROOM_CUR->pSca;
    r = Sca_get_area(pos, ofs, pSplit[0], pSplit[1]);
    Acc = 0;
    ppT_xz = &((SCA_BOX_REC**)MAIN.pRoom->pSca)[r + 1];  // reinterprets the memory as SCA_BOX_REC
    ScratchPad->Vec = *pos;
    pT_xz = ppT_xz[0];
    ppEnd = &ppT_xz[1];
    for (; pT_xz < *ppEnd; pT_xz++)
    {
        res = Sca_ck_flg(ofs, pT_xz, 0);
        if (res != -1)
        {
            bits = res & 0x300;
            Acc |= bits;
            if (bits == 0x300)
            {
                return 1;
            }
        }
    }
    return Acc;
}

INT Sca_ck_point_r(VECTOR* pos, SVECTOR* ofs, USHORT radius)
{
    SCA_BOX_REC** ppT_xz;
    SCA_BOX_REC** ppEnd;
    SCA_BOX_REC* pT_xz;
    SHORT r;
    USHORT flags;

    ppT_xz = (SCA_BOX_REC**)((UCHAR*)MAIN.pRoom->pSca + 4) + (SHORT)Sca_get_area(pos, ofs, VIEW(SHORT*, ROOM_CUR->pSca)[0], VIEW(SHORT*, MAIN.pRoom->pSca)[1]);  // UCHAR** view of the field (a plain access changes the code)
    flags = 0;
    ScratchPad->Vec = *pos;
    ppEnd = ppT_xz + 1;
    for (pT_xz = *ppT_xz; pT_xz < *ppEnd; pT_xz++)
    {
        r = Sca_ck_flg(ofs, pT_xz, radius);
        if (r != -1)
        {
            flags |= r & 0x300;
            if ((r & 0x300) == 0x300)
            {
                return 1;
            }
        }
    }
    return flags;
}

INT Sca_ck_line(VECTOR* dir, INT group)
{
    VECTOR v1;
    VECTOR v2;
    SCA_DATA** ppEnd;
    SCA_DATA* pT_xz;
    USHORT a, b, c, d;
    INT Ax, Az;
    INT px, pz;

    px = G.pEm->Pos_x / 18;
    pz = G.pEm->Pos_z / 18;
    Ax = dir->vx / 18;
    ppEnd = &((SCA_DATA**)((UCHAR*)G.pRoom->pSca + 8))[group & 0x1F];  // reinterprets the memory as SCA_DATA
    Az = dir->vz / 18;
    pT_xz = ppEnd[-1];
    pSca_last_grp = (USHORT*)pT_xz;
    for (; pT_xz < *ppEnd; pT_xz++)
    {
        a = pT_xz->Max_x / 18;
        b = pT_xz->Max_z / 18;
        c = pT_xz->Min_x / 18;
        d = pT_xz->Min_z / 18;
        if ((pT_xz->Type &= 0x300) != 0x300 && G.pEm->Id != 13 && G.pEm->Id != 18)
        {
            continue;
        }
        if (pT_xz->Id == 4 || pT_xz->Id == 5)
        {
            continue;
        }
        {
            INT z0, z1, x0, x1;
            x0 = c;
            x1 = a;
            z0 = b;
            z1 = d;
            ScratchPad->Vec.vy = 0;
            v1.vy = 0;
            ScratchPad->Vec.vx = x0 - x1;
            ScratchPad->Vec.vz = z0 - z1;
            v1.vx = px + Ax - x1;
            v1.vz = pz + Az - z1;
            v2.vx = px - x1;
            v2.vz = pz - z1;
            v2.vy = 0;
            OuterProduct0(&ScratchPad->Vec, &v1, &v1);
            OuterProduct0(&ScratchPad->Vec, &v2, &v2);
            if ((v1.vy ^ v2.vy) & 0x80000000)
            {
                v1.vx = x0 - px;
                v1.vz = z0 - pz;
                v1.vy = 0;
                OuterProduct0(dir, &v1, &v1);
                v2.vx = x1 - px;
                v2.vz = z1 - pz;
                v2.vy = 0;
                VectorNormal(&v2, &v2);
                OuterProduct0(dir, &v2, &v2);
                if ((v1.vy ^ v2.vy) & 0x80000000)
                {
                    return 1;
                }
            }
        }
        {
            INT z0, z1, x0, x1;
            x0 = a;
            x1 = c;
            z0 = b;
            z1 = d;
            ScratchPad->Vec.vy = 0;
            v1.vy = 0;
            ScratchPad->Vec.vx = x0 - x1;
            ScratchPad->Vec.vz = z0 - z1;
            v1.vx = px + Ax - x1;
            v1.vz = pz + Az - z1;
            v2.vx = px - x1;
            v2.vz = pz - z1;
            v2.vy = 0;
            OuterProduct0(&ScratchPad->Vec, &v1, &v1);
            OuterProduct0(&ScratchPad->Vec, &v2, &v2);
            if ((v1.vy ^ v2.vy) & 0x80000000)
            {
                v1.vx = x0 - px;
                v1.vz = z0 - pz;
                v1.vy = 0;
                OuterProduct0(dir, &v1, &v1);
                v2.vx = x1 - px;
                v2.vz = z1 - pz;
                v2.vy = 0;
                OuterProduct0(dir, &v2, &v2);
                if ((v1.vy ^ v2.vy) & 0x80000000)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

VOID Sca_hit_box(SHORT* pScd, VECTOR* pos, SVECTOR* old)
{
    SHORT Dx;
    SHORT Dz;
    SHORT t;
    UCHAR side;

    Dx = pScd[0] + EM_CUR->pAt_tbl->At_w - pos->vx + 0x12;
    ScratchWord[0] = Dx;
    t = pScd[2] - MAIN.pEm->pAt_tbl->At_w - pos->vx - 0x12;
    ScratchWord[1] = t;
    if (-t < Dx)
    {
        Dx = t;
        ScratchWord[2] = t;
    }
    Dz = pScd[1] + MAIN.pEm->pAt_tbl->At_w - pos->vz + 0x12;
    ScratchWord[3] = Dz;
    t = pScd[3] - MAIN.pEm->pAt_tbl->At_w - pos->vz - 0x12;
    ScratchWord[4] = t;
    if (-t < Dz)
    {
        Dz = t;
        ScratchWord[5] = t;
    }
    side = (((Dx ^ (pos->vx - old->vx)) & 0x8000) >> 15) | (((Dz ^ (pos->vz - old->vz)) & 0x8000) >> 14);
    if (pos->vx == old->vx && pos->vz == old->vz)
    {
        ScratchWord[7] = 0;
    }
    switch (side)
    {
    case 0:
        pos->vx = old->vx;
        pos->vz = old->vz;
        break;
    case 1:
        if ((USHORT)(Dx + 400) < 801)  // truncates to an unsigned halfword
        {
            ((ENEMY_WORK*)G_PEM(UCHAR))->Pos_x += Dx;  // pEm viewed as a byte pointer and re-typed in place (see G_PEM)
            EM_BE_FLG &= 0xEF;
            break;
        }
        goto both;
    case 2:
        if ((USHORT)(Dz + 400) < 801)  // truncates to an unsigned halfword
        {
            ((ENEMY_WORK*)G_PEM(UCHAR))->Pos_z += Dz;  // pEm viewed as a byte pointer and re-typed in place (see G_PEM)
            EM_BE_FLG |= 0x10;
            break;
        }
        goto both;
    case 3:
    both:
        if ((Dx < 0 ? -Dx : Dx) < (Dz < 0 ? -Dz : Dz))
        {
            ((ENEMY_WORK*)G_PEM(UCHAR))->Pos_x += Dx;  // pEm viewed as a byte pointer and re-typed in place (see G_PEM)
            EM_BE_FLG &= 0xEF;
        }
        else
        {
            ((ENEMY_WORK*)G_PEM(UCHAR))->Pos_z += Dz;  // pEm viewed as a byte pointer and re-typed in place (see G_PEM)
            EM_BE_FLG |= 0x10;
        }
        break;
    }
}

INT Sca_hit_circle(SCA_BOX_REC* pT_xz, VECTOR* pos)
{
    INT New_z;
    INT Abs_x;
    AT_TBL* pSca;
    INT Old_z;
    INT Abs_z;
    INT Half;
    INT Dx;
    INT Dz;
    INT root;
    INT New_x;
    INT mx;
    INT Dist;
    INT mz;
    USHORT w2;
    USHORT R;
    ENEMY_WORK* pEm;

    w2 = pT_xz->Min_x;
    pSca = (AT_TBL*)EM_CUR;  // reinterprets the memory as AT_TBL
    pSca = ((ENEMY_WORK*)pSca)->pAt_tbl;  // reinterprets the memory as ENEMY_WORK
    R = pSca->At_w;
    Half = (pT_xz->Max_x - w2 + R * 2) / 2;
    Dz = pos->vz - (pT_xz->Min_z - R + Half);
    Dx = pos->vx - (w2 - R + Half);
    root = SquareRoot0(Dz * Dz + Dx * Dx);
    Dist = root >= 0 ? root : -root;
    Half -= Dist;
    if (Half > 0)
    {
        mx = Half * Dx / Dist;
        mz = Half * Dz / Dist;
        pEm = MAIN.pEm;
        New_x = mx + pEm->Pos_x;
        Old_z = pEm->Pos_z;
        Abs_x = mx < 0 ? -mx : mx;
        pEm->Pos_x = New_x;
        New_z = mz + Old_z;
        Abs_z = mz < 0 ? -mz : mz;
        pEm->Pos_z = New_z;
        if (Abs_x < Abs_z)
        {
            pEm->Be_flg |= EM_BE_SLIDE_AXIS;
        }
        else
        {
            pEm->Be_flg &= ~EM_BE_SLIDE_AXIS;
        }
        return 1;
    }
    return 0;
}

VOID Sca_hit_flg(VOID)
{
    G.pEm->Sca_info = G.pEm->Sca_info | 8;
}
