/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\JOINT.C
 */

#include "JOINT.H"
#include "CUT.H"
#include "ENEMY.H"
#include "GAME.H"
#include "MAIN.H"
#include "ST_SUB03.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "SDK.H"
#include "EM_SUB.H"

USHORT Mirror_min;    // lower bound of the mirror range along the mirror plane
USHORT Mirror_max;    // upper bound of the mirror range along the mirror plane
USHORT Mirror_pos;    // mirror plane position (x or z coordinate)

VOID Pl_model_set(VOID)
{
    INT Load_bak;

    Load_bak = FREE_WORK_INT;
    MAIN.Player.pTmd = PL_EMD_BUF;
    FREE_WORK_INT = (INT)PL_EMD_BUF;
    Emd_load(EM_CUR, G.Player.Id & 3);
    PartsWork_set(MAIN.pEm, sizeof(PARTS_W));
    FREE_WORK_INT = Load_bak;
    Em_tmd_link(G.Player.pTmd);
    G.Player.Parts_num += 1;
    PartsWork_link(PL_PAC_BUF);
    VIEW(SCHAR, G.Player.Parts_num) -= 1;  // SCHAR* view of the field (a plain access changes the code)
    Init_Parts_work();
}

VOID Em_tmd_link(VOID* tmd)
{
    ENEMY_WORK* pEm;

    GsMapModelingData((ULONG*)((UCHAR*)tmd + 4));
    GsLinkObject5((ULONG)((UCHAR*)tmd + 0xC), (GsDOBJ5*)&EM_CUR->Attribute, 0);  // reinterprets the memory as GsDOBJ5
    pEm = MAIN.pEm;
    pEm->pCoord2 = (GsCOORDINATE2*)&pEm->Coord_flg;  // the embedded GsCOORDINATE2 starts at Coord_flg
    pEm->Attribute = 0x100;
    pEm->Hokan_flg = 0;
}

INT Joint_move(Late_flg, Kan_t_ptr, Seq_t_ptr, Hokan_step)
UCHAR Late_flg;  // narrow parameter, passed as int
SHORT* Kan_t_ptr;  // parameter
UCHAR* Seq_t_ptr;  // parameter
SHORT Hokan_step;  // narrow parameter, passed as int
{
    ENEMY_WORK* pEm;
    ANM_REC* hdr;
    UCHAR* frm;
    SHORT* data;
    PARTS_W* pP_ptr;
    INT half;
    UCHAR t;
    UCHAR be;
    UCHAR n;

    pEm = G.pEm;
    if (pEm->Move_wait >= 2)
    {
        pEm->Move_wait--;
        return 0;
    }
    data = (SHORT*)((INT*)Kan_t_ptr + Kan_t_ptr[1] / 4);  // halfword view of the same memory
    half = Kan_t_ptr[3] / 2;
    ScratchWord[0] = half;
    hdr = (ANM_REC*)(Seq_t_ptr + pEm->Move_no * 4);  // reinterprets the memory as ANM_REC
    frm = Seq_t_ptr + (hdr->Frm_ofs & 0xFFFC);
    frm += pEm->Move_cnt * 4;
    if (Late_flg)
    {
        frm += (hdr->Frm_num - pEm->Move_cnt * 2) * 4 - 4;
    }
    pP_ptr = pEm->pSin_parts_ptr;

    data += *(USHORT*)frm * half;  // halfword view of the same memory

    t = pEm->Hokan_flg;
    pP_ptr->Coord.t[0] = *data;
    n = pEm->Parts_num;
    if (t != 0)
    {
        data++;
        pP_ptr->Coord.t[1] = Hokan_step * pP_ptr->Coord.t[1] * t / 4096 +
                                 (*data * Hokan_step * (0x1000 / Hokan_step - t)) / 4096;

        data++;
        pP_ptr->Coord.t[2] = *data;
        data += 4;
        while (n-- != 0)
        {
            if (pP_ptr->Be_flg & 0x10)
            {
                data += 3;
            }
            else
            {
                ScratchPad->Sv.vx = *data++;
                ScratchPad->Sv.vy = *data++;

                ScratchPad->Sv.vz = *data;

                if (t == 0x1000 / Hokan_step - 1)
                {
                    ScratchWord[7] = 2;
                    do
                    {
                        INT i = ScratchWord[7];
                        INT s = ScratchSvU[i];
                        USHORT* r = (USHORT*)&((PARTS_W*)(i * 2 + (INT)pP_ptr))->Cdir;  // halfword view of the same memory
                        SHORT a = *r;
                        USHORT d = s - a + 0x800;
                        INT v;

                        if (d > 0x1000)  // truncates to an unsigned halfword
                        {
                            v = a;  // sign-extends the halfword
                            *r = d & 0x8000 ? v - 0x1000 : v + 0x1000;
                        }
                    } while (--ScratchWord[7] != -1);
                }
                LoadAverageShort12(&pP_ptr->Cdir, &ScratchPad->Sv, Hokan_step * t,
                    Hokan_step * (0x1000 / Hokan_step - t), &pP_ptr->Cdir);
                data++;
                RotMatrix(&pP_ptr->Cdir, &pP_ptr->Coord);
            }
            pP_ptr++;
        }
        G.pEm->Hokan_flg--;
    }
    else
    {
        data++;
        pP_ptr->Coord.t[1] = *data;
        data++;
        pP_ptr->Coord.t[2] = *data;
        data += 4;
        while (n-- != 0)
        {
            if (pP_ptr->Be_flg & 0x10)
            {
                data += 3;
                pP_ptr++;
            }
            else
            {
                pP_ptr->Cdir.vx = *data++;
                pP_ptr->Cdir.vy = *data++;
                pP_ptr->Cdir.vz = *data++;
                RotMatrix(&pP_ptr->Cdir, &pP_ptr->Coord);
                pP_ptr++;
            }
        }
    }

    EM_CUR->Move_wait = frm[2];

    be = MAIN.pEm->Move_cnt + 1;
    MAIN.pEm->Move_cnt = be;
    if (hdr->Frm_num - 1 < be)
    {
        MAIN.pEm->Move_cnt = 0;
        return 1;
    }
    return 0;
}

UINT* PartsWork_link(UINT* Packet_top)
{
    INT unused[6];
    PARTS_W* pP_ptr;
    GsDOBJ5* obj;
    UINT handle;
    UCHAR parts_num;
    UCHAR i;

    pP_ptr = EM_NOW->pSin_parts_ptr;
    parts_num = EM_NOW->Parts_num;
    handle = (UINT)EM_NOW->pTmd;
    for (i = 0; i < parts_num; i++)
    {
        obj = (GsDOBJ5*)&pP_ptr->Attribute;  // the embedded GsDOBJ5 starts at Attribute
        GsLinkObject5((ULONG)handle, obj, i);
        pP_ptr->Parts_no = i | (i & 1);
        pP_ptr->Be_flg = 3;
        obj->coord2 = (GsCOORDINATE2*)&pP_ptr->Coord_flg;  // the embedded GsCOORDINATE2 starts at Coord_flg
        pP_ptr->Coord_flg = 1;
        obj->id = 0;
        obj->packet = 0;
        pP_ptr->Fall_cnt = 0;
        obj->attribute = 0x100;
        Packet_top = (UINT*)GsPresetObject(obj, (ULONG*)Packet_top);  // word view of the same memory
        pP_ptr++;
    }
    return Packet_top;
}

VOID Init_Parts_work(VOID)
{
    ENEMY_WORK* pEm;
    SHORT* data_ptr;
    PARTS_W* pP_ptr;
    UCHAR parts_num;
    UCHAR i;

    pEm = EM_CUR;
    data_ptr = pEm->pKan_t_ptr;
    parts_num = pEm->Parts_num;
    pEm->Neck_no = 1;
    pP_ptr = MAIN.pEm->pSin_parts_ptr;
    data_ptr += 4;
    for (i = 0; i < parts_num; i++)
    {
        pP_ptr->Coord = GsIDMATRIX;
        pP_ptr->Coord.t[0] = *data_ptr++;
        pP_ptr->Coord.t[1] = *data_ptr++;
        pP_ptr->Coord.t[2] = *data_ptr++;
        pP_ptr->Cdir.vx = 0;
        pP_ptr->Cdir.vy = 0;
        pP_ptr->Cdir.vz = 0;
        pP_ptr->Free3 = 0;
        pP_ptr->Free4 = 0;
        pP_ptr->Free5 = 0;
        pP_ptr++;
    }
}

VOID Parts_pos_set(INT part)
{
    ENEMY_WORK* pEm;
    SHORT* pData;
    PARTS_W* pP_ptr;
    INT i;

    pEm = G.pEm;
    i = part & 0xFF;
    pData = pEm->pKan_t_ptr;
    pP_ptr = pEm->pSin_parts_ptr;
    pP_ptr += i;
    pData += i * 3 + 4;
    pP_ptr->Be_flg = (pP_ptr->Be_flg & 0xEB) | 2;
    pP_ptr->Coord.t[0] = *pData;
    pData++;
    pP_ptr->Coord.t[1] = *pData;
    pP_ptr->Coord.t[2] = pData[1];
}

VOID get_null_pos(ENEMY_WORK* pEm, KAN_HEAD* Kan, UCHAR* Seq, UCHAR R_flg)
{
    USHORT* pData;
    USHORT half;
    ENEMY_WORK* pEm_cur;
    ANM_REC* pHdr;
    UCHAR t;

    pData = (USHORT*)((INT*)Kan + Kan->Bone_ofs / 4);  // halfword view of the same memory
    half = Kan->frame_size / 2;
    Kan = (KAN_HEAD*)Seq;  // reinterprets the memory as KAN_HEAD
    pEm_cur = EM_CUR;
    pHdr = BYTE_OFS(ANM_REC, Kan, pEm->Move_no * 4);
    ScratchWord[7] = pEm_cur->Move_cnt;
    if (pEm_cur->Move_wait >= 2)
    {
        t = pEm_cur->Move_cnt - 1;
        pEm_cur->Move_cnt = t;
        if (t == 0xFF)
        {
            MAIN.pEm->Move_cnt = *(UCHAR*)pHdr - 1;  // byte view of the same memory
        }
    }
    Kan = BYTE_OFS(KAN_HEAD, Kan, pHdr->Frm_ofs & 0xFFFC);
    if (R_flg)
    {
        Kan = BYTE_OFS(KAN_HEAD, Kan, (pHdr->Frm_num - pEm->Move_cnt) * 4 - 4);
    }
    else
    {
        Kan = BYTE_OFS(KAN_HEAD, Kan, pEm->Move_cnt * 4);
    }
    pData += half * *(USHORT*)Kan;  // halfword view of the same memory
    pData += 3;
    ScratchPad->Sv.vx = *pData++;
    ScratchPad->Sv.vy = *pData;
    ScratchPad->Sv.vz = pData[1];
    G.pEm->Move_cnt = ScratchWord[7];
}

VOID PartsWork_set(ENEMY_WORK* pEm, INT Parts_size)
{
    INT work_top;

    pEm->Parts_num = ((KAN_HEAD*)pEm->pKan_t_ptr)->Parts_cnt;  // reinterprets the memory as KAN_HEAD

    work_top = FREE_WORK_INT;
    pEm->pSin_parts_ptr = (PARTS_W*)work_top;  // reinterprets the memory as PARTS_W

    FREE_WORK_INT = ((UINT)(Parts_size & 0xFFFF) >> 2) * pEm->Parts_num * 4 + work_top;  // the load pointer is advanced as an integer address (see FREE_WORK_INT)
}

VOID Mirror_view(pV0, pV1, flg, mxz)
GsRVIEW2* pV0;  // parameter
GsRVIEW2* pV1;  // parameter
UCHAR flg;  // narrow parameter, passed as int
USHORT mxz;  // narrow parameter, passed as int
{
    *pV1 = *pV0;
    if (flg)
    {
        pV1->vpx = pV0->vpx + (mxz - pV0->vpx) * 2;
        pV1->vrx = pV0->vrx + (mxz - pV0->vrx) * 2;
    }
    else
    {
        pV1->vpz = pV0->vpz + (mxz - pV0->vpz) * 2;
        pV1->vrz = pV0->vrz + (mxz - pV0->vrz) * 2;
    }
}

INT Mirror_in_view(VOID* pV0, INT flg, VOID* pos)
{
    INT view_along;
    INT ck;
    INT Dv;
    INT Dp;
    INT across_ofs;
    INT D5;
    INT qv;

    ck = (flg * 8) & 0x3F8;
    across_ofs = ck - 8;
    Dp = *(INT*)((INT)pos - across_ofs) - Mirror_pos;  // word view of the same memory
    Dv = *(INT*)((INT)pV0 - across_ofs) - Mirror_pos;  // word view of the same memory
    if ((Dp ^ Dv) >= 0)
    {
        qv = *(INT*)(ck + (INT)pos);  // word view of the same memory
        view_along = *(INT*)(ck + (INT)pV0);  // word view of the same memory
        D5 = (qv - view_along) * Dv / (Dp + Dv) + view_along;
        ScratchWord[5] = D5;
        return (UINT)(D5 - Mirror_min) < Mirror_max - Mirror_min;  // unsigned conversion (signed operand)
    }
    return 0;
}

VOID Mirror_trans(VOID)
{
    GsRVIEW2 M_cut;
    ENEMY_WORK* pEm;
    INT f;
    INT w;
    INT* save;
    ENEMY_WORK** ppEm;
    INT* pv;
    UCHAR* pi;

    Parts_work_cp();
    save = &ScratchWord[6];
    ppEm = &MAIN.pEm;
    pv = (INT*)&MAIN.pRoom;  // word view of the same memory
    pi = &MAIN.SaveData.Cut_no;
    pEm = *ppEm;
    *save = (INT)pEm->pSin_parts_ptr;  // real width or sign conversion
    pEm->pSin_parts_ptr = pEm->pM_char_ptr;
    f = SYSTEM_FLG_NOW & SYS_MIRROR_SIDE;
    Mirror_view((GsRVIEW2*)(*pi * 0x2C + *pv + 0x9C), &M_cut, f != 0, Mirror_pos);
    GsSetRefView2(&M_cut);
    ScratchPad->Mat = GsIDMATRIX;
    w = ScratchPad->Mat.m[0][0];
    ScratchPad->Mat.m[0][0] = -w;
    GsMulCoord2(&ScratchPad->Mat, &GsWSMATRIX);
    Joint_trans2(*ppEm);
    GsSetRefView2((GsRVIEW2*)(*pi * 0x2C + *pv + 0x9C));  // the camera record is addressed as an integer and read as a GsRVIEW2
    (*ppEm)->pSin_parts_ptr = (PARTS_W*)*save;  // reinterprets the memory as PARTS_W
}

VOID mirror_TMD_GT3(GsDOBJ5* dobj)
{
    struct
    {
        UCHAR b[0x1C];
    }* pB32;
    struct
    {
        UCHAR b[0x1C];
    }* pA32;
    INT unused;
    INT Last_ofs;
    INT tmp32;
    USHORT* pB16;
    USHORT* pA16;
    USHORT tmp16;
    USHORT n;

    TMD_MEM* tmd_top;

    tmd_top = (VOID*)dobj->tmd;  // untyped pointer, the callee takes the address as is
    pA32 = (VOID*)tmd_top->prim_top;  // untyped pointer, the callee takes the address as is
    n = tmd_top->n_poly;
    Last_ofs = (n - 1) * 0x1C;
    pA16 = (USHORT*)pA32;  // halfword view of the same memory
    pA32 = BYTE_OFS(VOID, pA32, Last_ofs + 0x14);  // untyped pointer, the callee takes the address as is
    pA16 = BYTE_OFS(USHORT, pA16, Last_ofs + 8);
    pB32 = (VOID*)pA32;  // untyped pointer, the callee takes the address as is
    pB32 = BYTE_OFS(VOID, pB32, 4);  // untyped pointer, the callee takes the address as is
    pB16 = pA16 + 2;
    do
    {
        tmp16 = *pA16;

        n--;

        *pA16 = *pB16;
        *pB16 = tmp16;
        pB16 -= 14;
        tmp32 = *(INT*)pA32;  // word view of the same memory
        pA16 -= 14;
        *(INT*)pA32 = *(INT*)pB32;  // INT* view of the same memory
        *(INT*)pB32 = tmp32;  // INT* view of the same memory
        pB32--;
        pA32--;
    } while (n & 0xFFFF);
}

VOID Mirror_model_cp(VOID)
{
    ENEMY_WORK* pEm;
    UCHAR parts_num;
    UCHAR n;
    PARTS_W* pM_ptr;
    INT byte;
    PARTS_W* pP_ptr;
    ULONG* pTmd_src;
    UINT width;
    ULONG* pTmd_dst;
    INT i;
    GsDOBJ5* pDobj;

    pEm = G.pEm;
    parts_num = pEm->Parts_num;
    pM_ptr = pEm->pM_char_ptr = FREE_WORK_CUR;
    n = parts_num;
    byte = n * sizeof(PARTS_W);

    FREE_WORK_INT = byte + (INT)pM_ptr;  // the load pointer is advanced as an integer address (see FREE_WORK_INT)

    pP_ptr = pEm->pSin_parts_ptr;
    pTmd_src = (ULONG*)pP_ptr->pTmd;  // word view of the same memory
    width = *pTmd_src - (INT)pTmd_src;  // signed conversion (unsigned operand)

    Memcpy((UCHAR*)pM_ptr + byte, pTmd_src, width);  // byte view of the same memory

    Memcpy(pM_ptr, pP_ptr, byte);

    pTmd_dst = MAIN.pFree_work;
    ((GsDOBJ5*)&pM_ptr->Attribute)->tmd = pTmd_dst;  // the embedded GsDOBJ5 starts at Attribute

    FREE_WORK_INT = (width >> 2) * 4 + (INT)pTmd_dst;  // the load pointer is advanced as an integer address (see FREE_WORK_INT)

    width = (INT)((GsDOBJ5*)&pM_ptr->Attribute)->tmd - (INT)pP_ptr->pTmd;  // signed conversion (unsigned operand)
    ScratchPtr[8] = pTmd_dst;
    i = 0;
    if (n != 0)
    {
        do
        {
            pDobj = (GsDOBJ5*)&pM_ptr->Attribute;  // the embedded GsDOBJ5 starts at Attribute
            GsLinkObject5(ScratchWord[8], pDobj, i & 0xFF);
            i++;
            ((GsDOBJ5*)&pM_ptr->Attribute)->coord2 = (GsCOORDINATE2*)&pM_ptr->Coord_flg;  // the embedded GsDOBJ5 starts at Attribute, its GsCOORDINATE2 at Coord_flg
            ((TMD_MEM*)(((GsDOBJ5*)&pM_ptr->Attribute)->tmd))->prim_top += width;  // reinterprets the memory as TMD_MEM
            mirror_TMD_GT3(pDobj);
            pM_ptr++;
            MAIN.pFree_work = GsPresetObject(pDobj, FREE_WORK_CUR);
        } while ((UINT)(i & 0xFF) < parts_num);  // unsigned conversion (signed operand)
    }
}

VOID Init_mirror_weapon(VOID)
{
    INT Tmd_adr;
    INT Pkt_adr;

    G_PEM_BYTE(ENEMY_WORK)->pM_option_tmd = (VOID*)(Tmd_adr = FREE_WORK_INT);  // G_PEM_BYTE; the mirror TMD address is an integer
    Pkt_adr = Tmd_adr + 0x7A00;
    FREE_WORK_INT = Pkt_adr;
    EM_NOW->pM_option_packet = (VOID*)Pkt_adr;
    FREE_WORK_INT = Pkt_adr + 0x1A00;
}

INT Mirror_weapon_cp(INT Weapon_num)
{
    GsDOBJ5* dobj;
    INT* tmd;
    INT prim_top;
    INT ofs;
    TMD_MEM* pTmd_new;
    PARTS_W* pM_ptr;
    PARTS_W* pP_ptr;
    ENEMY_WORK* pEm;
    ENEMY_WORK* pEm2;

    pEm = EM_CUR;
    pM_ptr = pEm->pM_char_ptr;
    pP_ptr = pEm->pSin_parts_ptr;
    ofs = (UCHAR)Weapon_num;  // truncates to a byte
    pP_ptr += ofs;
    tmd = (INT*)pP_ptr->pTmd;  // word view of the same memory
    pM_ptr += ofs;

    Memcpy(pEm->pM_option_tmd, tmd, *tmd - (INT)tmd);  // size of the model data in bytes

    pEm2 = MAIN.pEm;
    dobj = (GsDOBJ5*)&pM_ptr->Attribute;  // the embedded GsDOBJ5 starts at Attribute
    pM_ptr->pTmd = pEm2->pM_option_tmd;
    pM_ptr->pPacket = pEm2->pM_option_packet;
    GsLinkObject5((ULONG)pEm2->pM_option_tmd, dobj, 0);

    pTmd_new = (TMD_MEM*)pM_ptr->pTmd;  // reinterprets the memory as TMD_MEM
    pTmd_new->prim_top += (UINT)pTmd_new - (UINT)pP_ptr->pTmd;  // unsigned conversion (signed operand)

    mirror_TMD_GT3(dobj);
    GsPresetObject(dobj, MAIN.pEm->pM_option_packet);
}

VOID Parts_work_cp(VOID)
{
    PARTS_W* pP_ptr;
    PARTS_W* pM_ptr;
    UCHAR p_num;
    INT t;
    VOID* a;

    pP_ptr = EM_NOW->pSin_parts_ptr;
    p_num = EM_NOW->Parts_num;
    pM_ptr = EM_NOW->pM_char_ptr;
    pP_ptr += p_num - 1;
    pM_ptr += p_num - 1;
    do
    {
        pM_ptr->Be_flg = pP_ptr->Be_flg;
        pM_ptr->Workm = pP_ptr->Workm;
        if (pM_ptr->Be_flg & 0x40)
        {
            a = MAIN.SaveData.Cut_no * 0x2C + (UCHAR*)MAIN.pRoom + 0x9C;
            t = G.System_flg & SYS_MIRROR_SIDE;
            if ((UCHAR)Mirror_in_view(a, t != 0, pM_ptr->Workm.t))  // truncates to a byte
            {
                pM_ptr->Be_flg |= 1;
            }
            else
            {
                pM_ptr->Be_flg &= 0xFE;
            }

            if (!(pP_ptr->Be_flg & 1))
            {
                pM_ptr->Be_flg &= 0xFE;
            }
        }
        pP_ptr--;
        pM_ptr--;
    } while (--p_num);
}

VOID Joint_pos_trans(INT scale)
{
    KO_TBL* pKo_tbl;
    KAN_HEAD* pHdr;
    UCHAR* pChild;
    PARTS_W* pP_ptr;
    ENEMY_WORK* pEm;
    UCHAR v;
    UCHAR n;

    v = EM_NOW->Id;
    if ((v != 0xD && v != 0x12) || EM_NOW->Type != 1)
    {
        pEm = EM_CUR;
        pKo_tbl = pEm->pKan_t_ptr;
        pP_ptr = pEm->pSin_parts_ptr;
        pHdr = (KAN_HEAD*)pKo_tbl;  // reinterprets the memory as KAN_HEAD

        pKo_tbl = (KO_TBL*)((pHdr->Ko_ofs & 0xFFFC) + (UINT)pHdr);  // reinterprets the memory as KO_TBL

        pChild = (UCHAR*)pKo_tbl + pKo_tbl->ko_t_off;  // byte view of the same memory
        if (!(pEm->Zone_in_flg & 0x80))
        {
            RotMatrix((SVECTOR*)&pEm->Cdir_x, (MATRIX*)pEm->M);  // reinterprets the memory as SVECTOR
        }
        if (scale != 0)
        {
            ScratchPad->Vec.vz = scale;
            ScratchPad->Vec.vy = scale;
            ScratchPad->Vec.vx = scale;
            ScaleMatrix((MATRIX*)MAIN.pEm->M, &ScratchPad->Vec);  // reinterprets the memory as MATRIX
        }
        if ((VIEW(INT, pP_ptr->Be_flg) & 0x48) == 8)  // INT* view of the field (a plain access changes the code)
        {
            GsMulCoord0((MATRIX*)MAIN.pEm->M, &pP_ptr->Coord, &pP_ptr->Workm);  // reinterprets the memory as MATRIX
            pP_ptr->Fall_cnt = 0;
            pP_ptr->Be_flg = (pP_ptr->Be_flg & 0xF5) | 0x50;
        }
        if (pP_ptr->Be_flg & 2)
        {
            GsMulCoord0((MATRIX*)MAIN.pEm->M, &pP_ptr->Coord, &pP_ptr->Workm);  // reinterprets the memory as MATRIX
        }
        n = pKo_tbl->ko_num;
        while (n--)
        {
            ko_joint_trans3(&pP_ptr->Workm, (UCHAR*)pKo_tbl, *pChild, pP_ptr);  // byte view of the same memory
            pChild++;
        }
    }
}

VOID ko_joint_trans3(MATRIX* wm, UCHAR* arm, UCHAR idx, PARTS_W* pO)
{
    KO_TBL* entry;
    PARTS_W* pP_ptr;
    UCHAR* list;
    UCHAR n;

    pP_ptr = idx + G.pEm->pSin_parts_ptr;
    entry = idx + (KO_TBL*)arm;  // reinterprets the memory as KO_TBL
    list = arm + entry->ko_t_off;
    if ((VIEW(INT, pP_ptr->Be_flg) & 0x48) == 8)  // INT* view of the field (a plain access changes the code)
    {
        GsMulCoord0(wm, &pP_ptr->Coord, &pP_ptr->Workm);
        pP_ptr->Fall_cnt = 0;
        pP_ptr->Be_flg = (pP_ptr->Be_flg & 0xF5) | 0x50;
    }
    if (pP_ptr->Be_flg & 2)
    {
        GsMulCoord0(wm, &pP_ptr->Coord, &pP_ptr->Workm);
    }
    n = entry->ko_num;
    while (n--)
    {
        ko_joint_trans3(&pP_ptr->Workm, arm, *list, pP_ptr);
        list++;
    }
}

VOID Joint_trans2(ENEMY_WORK* pEm)
{
    MATRIX M;
    MATRIX tmp;
    MATRIX unused;
    PARTS_W* pP_ptr;
    UCHAR n;
    UCHAR c;
    UCHAR* pWorld;
    TMD_MEM* pTmd;
    INT Pkt_base;

    if ((EM_NOW->Id == 0xD || EM_NOW->Id == 0x12) && EM_NOW->Type == 1)
    {
        return;
    }
    ScratchWord[7] = 0;
    if (!(pEm->Zone_in_flg & 0x7F))
    {
        ScratchWord[7] = 1;
    }
    n = pEm->Parts_num - 1;
    pP_ptr = pEm->pSin_parts_ptr;
    pP_ptr += n;
    Set_light((VECTOR*)&pEm->Pos_x);  // reinterprets the memory as VECTOR
    do
    {
        c = pP_ptr->Be_flg;
        if (pEm->Id == 0xD || pEm->Id == 0x12)
        {
            Set_light(MATRIX_POS_PTR(&pP_ptr->Workm));
        }
        if (c & 4)
        {
            ScratchPad->Sv.vy = 0x1E;
            ScratchPad->Sv.vx = 0;
            ScratchPad->Sv.vz = 0;
            pP_ptr->Cdir.vx = -0x14;
            pP_ptr->Cdir.vz = 0;
            pP_ptr->Cdir.vy = 0xC8;
            Parts_down(pP_ptr, -0x23, -0x64, 1);
        }
        if (!(c & 1))
        {
            if (c & 0x20)
            {
                Joint_path_anim_step(pP_ptr);
            }
            goto next;
        }
        pWorld = (UCHAR*)&pP_ptr->Workm;  // byte view of the same memory
        tmp = GsWSMATRIX;
        GsMulCoord0(&GsWSMATRIX, (MATRIX*)pWorld, &M);  // reinterprets the memory as MATRIX
        MulMatrix0(&GsLIGHTWSMATRIX, (MATRIX*)pWorld, &ScratchPad->Mat);  // reinterprets the memory as MATRIX
        if (ScratchWord[7] != 0)
        {
            if (c & 0x74)
            {
                goto check;
            }
            goto next;
        }
        if (c & 0x74)
        {
        check:
            if (Hit_ck_point4(MATRIX_POS_PTR(&pP_ptr->Workm), G.Cccut) == 0)
            {
            next:
                pP_ptr--;
                continue;
            }
        }
        ScratchPtr[14] = pP_ptr->pTmd;
        SetLightMatrix(&ScratchPad->Mat);
        GsSetLsMatrix(&M);
        if (c & 0x80)
        {
            TMD_MEM* pTmd2 = ScratchPtr[14];
            GsPrstTG3NL((pTmd2->prim_top), pTmd2->ver_top, (pTmd2->nor_top),
                (UCHAR*)pP_ptr->pPacket + CBUF_ID_CUR * sizeof(POLY_GT3) + 8, pTmd2->n_poly, 4,
                &MAIN.Ot1[MAIN.Cbuf_id]);
            pP_ptr--;
        }
        else
        {
            pTmd = ScratchPtr[14];
            GsPrstTG3L((pTmd->prim_top), pTmd->ver_top, (pTmd->nor_top),
                (UCHAR*)pP_ptr->pPacket + CBUF_ID_CUR * sizeof(POLY_GT3) + 8, pTmd->n_poly, 4,
                &MAIN.Ot1[MAIN.Cbuf_id]);
            pP_ptr--;
        }
    } while (n-- != 0);
}

VOID MulLMatrix(MATRIX* lwm, MATRIX* klwm, MATRIX* nlm)
{
    MATRIX tmp;
    VECTOR vec;

    TransposeMatrix(lwm, &tmp);
    MulMatrix0(&tmp, klwm, nlm);
    vec.vx = klwm->t[0] - lwm->t[0];
    vec.vy = klwm->t[1] - lwm->t[1];
    vec.vz = klwm->t[2] - lwm->t[2];
    ApplyMatrixLV(&tmp, &vec, &vec);
    nlm->t[0] = vec.vx;
    nlm->t[1] = vec.vy;
    nlm->t[2] = vec.vz;
}
