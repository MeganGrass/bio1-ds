/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\GUIDE_CK.C
 */

#include "GUIDE_CK.H"
#include "SCA.H"

SCHAR Root_ck(SHORT x, SHORT z, SHORT* pOut_x, SHORT* pOut_z)
{
    SHORT T_x;
    SHORT T_z;
    INT P_area_no;
    INT T_area_no;
    INT Next_zone;
    SCHAR step;
    INT Rsv;    // unused local: reserves stack space
    INT res;
    SCHAR diff;
    SCHAR From_area;
    SCHAR To_area;
    SCHAR dir;
    SCHAR nArea;
    VOID* pB;

    T_x = x;
    T_z = z;
    From_area = Guide_check((SHORT)G.pEm->Pos_x, (SHORT)G.pEm->Pos_z);  // sign-extends the halfword
    if (z != 0)
    {
        To_area = Guide_check(x, z);
    }
    else
    {
        To_area = T_x;
        pB = (To_area & 0xFF) * sizeof(BLOCK_REC) + (UCHAR*)G.pRoom->pBlock;
        T_x = (((BLOCK_REC*)((UCHAR*)pB + 2))->Min_x + ((BLOCK_REC*)((UCHAR*)pB + 2))->Max_x) >> 1;  // reinterprets the memory as BLOCK_REC
        T_z = (((BLOCK_REC*)((UCHAR*)pB + 2))->Min_z + ((BLOCK_REC*)((UCHAR*)pB + 2))->Max_z) >> 1;  // reinterprets the memory as BLOCK_REC
    }
    P_area_no = From_area & 0xFF;
    T_area_no = To_area & 0xFF;
    if (P_area_no == T_area_no)
    {
        *pOut_x = T_x;
        *pOut_z = T_z;
        return From_area | 0x10;
    }
    diff = To_area - From_area;
    nArea = *(UCHAR*)G.pRoom->pBlock;  // byte view of the same memory
    dir = diff;
    if (diff < 0)
    {
        dir = nArea + diff;
    }
    step = -1;
    if (dir > nArea >> 1)  // sign-extends the byte
    {
        step = 1;
    }
    dir = step;
    if (P_area_no == 0)
    {
        dir = ~dir + 1;
    }
    *ScratchPad->Root_path = From_area;
    *ScratchPad->Root_best = From_area;
    *ScratchPad->Poxz = G.pEm->Pos_x;
    ScratchPad->Poxz[1] = G.pEm->Pos_z;
    if ((dir << 0x18) > 0)
    {
        ScratchPad->Root_path[1] = 0xFF;
        res = Root_ck_cw(P_area_no, T_area_no, T_x, T_z) << 0x18;
    }
    else
    {
        ScratchPad->Root_path[1] = nArea;
        res = Root_ck_ccw(P_area_no, T_area_no, T_x, T_z) << 0x18;
    }
    Next_zone = res >> 0x18;
    if (Next_zone + 1 == 0)
    {
        return -1;
    }
    Root_edge_set(From_area & 0xFF, Next_zone & 0xFFFF);
    *pOut_x = ScratchWord[0];
    *pOut_z = ScratchWord[1];
    return Next_zone;
}

SCHAR Root_ck_cw(INT src, UCHAR dst, SHORT x, SHORT z)
{
    UCHAR* path;
    UCHAR* best;
    SHORT* pts;
    INT* pz;
    UINT sum;
    UINT min;
    SCHAR i;
    SCHAR j;
    USHORT mask;
    INT pRet;

    path = ScratchPad->Root_path;
    best = ScratchPad->Root_best;
    pts = ScratchPad->Poxz;
    sum = 0;
    min = -1;
    i = 0;
    pz = &ScratchWord[1];
    do
    {
        mask = ((BLOCK_REC*)((UCHAR*)G.pRoom->pBlock + path[i] * sizeof(BLOCK_REC) + 2))->Abut;  // reinterprets the memory as BLOCK_REC
        if ((mask >> dst) & 1)
        {
            SHORT* q;
            path[i + 1] = dst;
            Root_edge_set(path[i], dst);
            q = (SHORT*)(i * 4 + (INT)pts);  // halfword view of the same memory
            sum += SquareRoot0((q[0] - ScratchWord[0]) * (q[0] - ScratchWord[0]) + (q[1] - *pz) * (q[1] - *pz));
            sum += SquareRoot0((ScratchWord[0] - x) * (ScratchWord[0] - x) + (*pz - z) * (*pz - z));
            if (sum < min)
            {
                min = sum;
                j = i + 1;
                do
                {
                    best[j] = path[j];
                } while (--j != 0);
            }
            if (i != 0)
            {
                SHORT* pQ2;
                SHORT* r2;
                sum -= SquareRoot0((ScratchWord[0] - x) * (ScratchWord[0] - x) + (*pz - z) * (*pz - z));
                pQ2 = (SHORT*)(i * 4 + (INT)pts);  // halfword view of the same memory
                sum -= SquareRoot0((pQ2[0] - ScratchWord[0]) * (pQ2[0] - ScratchWord[0]) + (pQ2[1] - *pz) * (pQ2[1] - *pz));
                r2 = (SHORT*)((i - 1) * 4 + (INT)pts);  // halfword view of the same memory
                sum -= SquareRoot0((r2[0] - pQ2[0]) * (r2[0] - pQ2[0]) + (r2[1] - pQ2[1]) * (r2[1] - pQ2[1]));
            }
        }
        else if (mask & (-1 << (path[i + 1] + 1)))
        {
            SHORT* pQ3;
            i++;
            do
            {
                path[i]++;
            } while (!((mask >> path[i]) & 1));
            for (j = i - 2; j >= 0; j--)
            {
                if (path[i] == path[j])
                {
                    goto next;
                }
            }
            Root_edge_set(path[i], path[i - 1]);
            pQ3 = (SHORT*)((i - 1) * 4 + (INT)pts);  // halfword view of the same memory
            sum += SquareRoot0((pQ3[0] - ScratchWord[0]) * (pQ3[0] - ScratchWord[0]) + (pQ3[1] - *pz) * (pQ3[1] - *pz));
            if (sum >= min)
            {
                sum -= SquareRoot0((pQ3[0] - ScratchWord[0]) * (pQ3[0] - ScratchWord[0]) + (pQ3[1] - *pz) * (pQ3[1] - *pz));
            }
            else
            {
                pts[i * 2] = ScratchWord[0];
                pts[i * 2 + 1] = *pz;
                path[i + 1] = 0xFF;
                continue;
            }
        }
        else if (i != 0)
        {
            SHORT* pQ4;
            SHORT* r4;
            r4 = (SHORT*)((i - 1) * 4 + (INT)pts);  // halfword view of the same memory
            pQ4 = (SHORT*)(i * 4 + (INT)pts);  // halfword view of the same memory
            sum -= SquareRoot0((r4[0] - pQ4[0]) * (r4[0] - pQ4[0]) + (r4[1] - pQ4[1]) * (r4[1] - pQ4[1]));
        }
    next:
        i--;
    } while (i >= 0);
    if (min + 1 != 0)
    {
        return best[1];
    }
    return -1;
}

SCHAR Root_ck_ccw(INT src, UCHAR dst, SHORT x, SHORT z)
{
    UCHAR* path = ScratchPad->Root_path;
    UCHAR* best = ScratchPad->Root_best;
    AREA_PT_REC* pos = (AREA_PT_REC*)ScratchPad->Poxz; // SHORT array Poxz read as AREA_PT_REC records
    UINT dist = 0;
    UINT Min_dist = -1;
    SCHAR depth = 0;
    UCHAR first = *(UCHAR*)G.pRoom->pBlock;  // first byte of the block table
    SCHAR d;
    SCHAR i;
    USHORT mask;
    UCHAR* e;
    UCHAR* n;
    UCHAR* c;

    do
    {
        d = depth;
        e = &path[d];
        mask = ((BLOCK_REC*)((UCHAR*)G.pRoom->pBlock + *e * sizeof(BLOCK_REC) + 2))->Abut;  // reinterprets the memory as BLOCK_REC
        if ((mask >> dst) & 1)
        {
            e[1] = dst;
            Root_edge_set(e[0], dst);
            {
                INT dx = pos[d].x - ScratchWord[0];
                INT dz = pos[d].z - ScratchWord[1];
                dist += SquareRoot0(dx * dx + dz * dz);
            }
            {
                INT dx = ScratchWord[0] - x;
                INT dz = ScratchWord[1] - z;
                dist += SquareRoot0(dx * dx + dz * dz);
            }
            if (dist < Min_dist)
            {
                Min_dist = dist;
                i = depth + 1;
                do
                {
                    best[i] = path[i];
                } while (--i != 0);
            }
            if (depth != 0)
            {
                {
                    INT dx = ScratchWord[0] - x;
                    INT dz = ScratchWord[1] - z;
                    dist -= SquareRoot0(dx * dx + dz * dz);
                }
                {
                    INT dx = pos[depth].x - ScratchWord[0];
                    INT dz = pos[depth].z - ScratchWord[1];
                    dist -= SquareRoot0(dx * dx + dz * dz);
                }
                {
                    INT prev = depth - 1;
                    INT dx = pos[prev].x - pos[depth].x;
                    INT dz = pos[prev].z - pos[depth].z;
                    dist -= SquareRoot0(dx * dx + dz * dz);
                }
            }
        }
        else if (mask & ((1 << e[1]) - 1))
        {
            depth++;
            c = &path[depth];
            do
            {
                (*c)--;
            } while (!((mask >> *c) & 1));
            for (i = depth - 2; i >= 0; i--)
            {
                if (path[depth] == path[i])
                {
                    goto next;
                }
            }
            n = &path[depth];
            Root_edge_set(n[0], n[-1]);
            {
                INT prev = depth - 1;
                INT dx = pos[prev].x - ScratchWord[0];
                INT dz = pos[prev].z - ScratchWord[1];
                dist += SquareRoot0(dx * dx + dz * dz);
            }
            if (dist >= Min_dist)
            {
                INT prev = depth - 1;
                INT dx = pos[prev].x - ScratchWord[0];
                INT dz = pos[prev].z - ScratchWord[1];
                dist -= SquareRoot0(dx * dx + dz * dz);
            }
            else
            {
                pos[depth].x = ScratchWord[0];
                pos[depth].z = ScratchWord[1];
                n[1] = first;
                continue;
            }
        }
        else if (d != 0)
        {
            INT prev = d - 1;
            INT dx = pos[prev].x - pos[d].x;
            INT dz = pos[prev].z - pos[d].z;
            dist -= SquareRoot0(dx * dx + dz * dz);
        }
    next:
        depth--;
    } while (depth >= 0);
    if (Min_dist + 1 != 0)
    {
        return best[1];
    }
    return -1;
}

SHORT Root_edge_dir(x, z, dst, src)
SHORT x;  // narrow parameter, passed as int
SHORT z;  // narrow parameter, passed as int
SCHAR dst;  // narrow parameter, passed as int
SCHAR src;  // narrow parameter, passed as int
{
    USHORT* pBlk_a;
    USHORT* pBlk_b;
    USHORT* pCoord_a;
    USHORT* pCoord_b;
    INT tbl;
    SCHAR Area0;
    SHORT Edge_x;
    SHORT Edge_z;
    SHORT lo;
    SHORT hi;

    if (src < 0)
    {
        Area0 = Guide_check(x, z);
    }
    else
    {
        Area0 = src & 0xF;
    }
    tbl = (INT)G.pRoom->pBlock;
    pBlk_a = (USHORT*)(Area0 * sizeof(BLOCK_REC) + tbl);  // halfword view of the same memory
    pCoord_a = pBlk_a + 1;
    pBlk_b = (USHORT*)(dst * sizeof(BLOCK_REC) + tbl);  // halfword view of the same memory
    pCoord_b = pBlk_b + 1;
    if (pBlk_a[1] == pBlk_b[3] || pBlk_a[3] == pBlk_b[1])
    {
        Edge_x = pBlk_a[1] == pBlk_b[3] ? pBlk_a[1] : pBlk_b[1];
        lo = pCoord_a[1] > pCoord_b[1] ? pCoord_a[1] : pCoord_b[1];
        hi = pCoord_a[3] < pCoord_b[3] ? pCoord_a[3] : pCoord_b[3];
        Edge_z = Root_edge_lim(z, Edge_x, lo, hi);
    }
    else
    {
        Edge_z = pBlk_a[2] == pBlk_b[4] ? pBlk_a[2] : pBlk_b[2];
        lo = pCoord_a[0] > pCoord_b[0] ? pCoord_a[0] : pCoord_b[0];
        hi = pCoord_a[2] < pCoord_b[2] ? pCoord_a[2] : pCoord_b[2];
        Edge_x = Root_edge_lim(x, Edge_z | 0x8000, lo, hi);
    }
    ScratchWord[0] = Edge_x;
    ScratchWord[1] = Edge_z;
    return Direction_ck(x, z, Edge_x, Edge_z);
}

INT Root_edge_lim(SHORT v, USHORT edge, SHORT start, SHORT end)
{
    SHORT lo;
    SHORT hi;
    SHORT t;
    INT d;

    lo = start;
    hi = end;
    t = edge & 0x7FFF;
    d = ((end - start) >> 3) + 0x280;
    if (edge & 0x8000)
    {
        if (Sca_ck_point(start, t))
        {
            lo = start + d;
        }
        if (Sca_ck_point(end, t))
        {
            hi = end - d;
        }
    }
    else
    {
        if (Sca_ck_point(t, start))
        {
            lo = start + d;
        }
        if (Sca_ck_point(t, end))
        {
            hi = end - d;
        }
    }
    if (lo < hi)
    {
        if (v < lo)
        {
            return lo;
        }
        if (hi < v)
        {
            return hi;
        }
        return v;
    }
    return (lo + hi) >> 1;
}

INT Guide_check(INT target_x, INT target_z)
{
    UCHAR* atari_ptr;
    BLOCK_REC* pBlk;
    UCHAR atari_ctr;
    UINT d;
    UINT atari_size;

    atari_ptr = G.pRoom->pBlock;
    atari_ctr = atari_ptr[0];

    pBlk = (BLOCK_REC*)(atari_ctr * sizeof(BLOCK_REC) + (INT)atari_ptr + 2);  // reinterprets the memory as BLOCK_REC
    do
    {
        pBlk--;
        atari_ctr--;
        atari_size = pBlk->Max_x - pBlk->Min_x;
        d = target_x - pBlk->Min_x;
        if ((d & 0xFFFF) < (atari_size & 0xFFFF))
        {
            atari_size = pBlk->Max_z - pBlk->Min_z;
            d = target_z - pBlk->Min_z;
            if ((d & 0xFFFF) < (atari_size & 0xFFFF))
            {
                ScratchWord[0] = pBlk->Dir_bit;
                ScratchWord[1] = pBlk->Abut;
                return atari_ctr;
            }
        }
    } while (atari_ctr != 0);
    return 0xFF;
}

INT Root_edge_set(USHORT Area0, USHORT Area1)
{
    AREA_REC* base;
    AREA_REC* pBlk_a;
    AREA_REC* pBlk_b;
    USHORT* pa;
    USHORT* pb;
    UCHAR pRet;
    USHORT v;
    USHORT Max_val;
    USHORT Min_val;

    pRet = 0;
    base = G.pRoom->pBlock;
    pBlk_a = (AREA_REC*)(Area0 * sizeof(AREA_REC) + (INT)base);  // reinterprets the memory as AREA_REC
    pa = &pBlk_a->x0;
    pBlk_b = (AREA_REC*)(Area1 * sizeof(AREA_REC) + (INT)base);  // reinterprets the memory as AREA_REC
    pb = &pBlk_b->x0;
    if ((v = pBlk_a->x0) == pBlk_b->x1 || (v = pBlk_b->x0) == pBlk_a->x1)
    {
        ScratchWord[0] = v;
    }
    else
    {
        pRet = 1;
        if ((v = pBlk_a->z0) == pBlk_b->z1)
        {
            ScratchWord[1] = v;
        }
        else if ((v = pBlk_b->z0) == pBlk_a->z1)
        {
            ScratchWord[1] = v;
        }
    }
    if (pRet)
    {
        if (pa[0] > pb[0])
        {
            Max_val = pa[0];
        }
        else
        {
            Max_val = pb[0];
        }
        if (pa[2] < pb[2])
        {
            Min_val = pa[2];
        }
        else
        {
            Min_val = pb[2];
        }
        ScratchWord[0] = (Max_val + Min_val) >> 1;
    }
    else
    {
        if (pa[1] > pb[1])
        {
            Max_val = pa[1];
        }
        else
        {
            Max_val = pb[1];
        }
        if (pa[3] < pb[3])
        {
            Min_val = pa[3];
        }
        else
        {
            Min_val = pb[3];
        }
        ScratchWord[1] = (Max_val + Min_val) >> 1;
    }
    return pRet;
}

USHORT Sca_ck_point(SHORT x, SHORT z)
{
    ScratchPad->Vec.vx = x;
    ScratchPad->Vec.vy = 0;
    ScratchPad->Vec.vz = z;
    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0;
    return Sca_ck_pos(&ScratchPad->Vec, &ScratchPad->Sv) & 0xFFFF;
}

INT Direction_ck(INT Hontai_x, INT Hontai_z, INT Target_x, INT Target_z)
{
    SHORT XX = Target_x - Hontai_x;
    SHORT ZZ = Target_z - Hontai_z;

    if (XX != 0)
    {
        return ((XX < 0 ? 0x800 : 0x1000) - catan((ZZ << 12) / XX)) & 0xFFF;
    }
    return ZZ > 0 ? 0xC00 : 0x400;
}

INT Get_dir_tgt(x, z)
SHORT x;  // narrow parameter, passed as int
SHORT z;  // narrow parameter, passed as int
{
    return (SHORT)Direction_ck((SHORT)G.pEm->Pos_x, (SHORT)G.pEm->Pos_z, x, z);  // sign-extends the halfword
}

INT Floor_check(check_x, check_z)
SHORT check_x;  // narrow parameter, passed as int
SHORT check_z;  // narrow parameter, passed as int
{
    FLOOR_REC* zone;

    zone = (FLOOR_REC*)((UCHAR*)G.pRoom->pFloor + 2);
    while (1)
    {
        if ((UINT)(check_x - zone->x) < zone->w && (UINT)(check_z - zone->y) < zone->h)  // unsigned conversion (signed operand)
        {
            return zone->id;
        }
        zone++;
    }
}
