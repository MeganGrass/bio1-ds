/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SUB_PL.C
 */

#include "TYPES.H"
#include <RAND.H>
#include "SUB_PL.H"
#include "CD.H"
#include "CUT.H"
#include "EFF1.H"
#include "ENEMY.H"
#include "GUIDE_CK.H"
#include "JOINT.H"
#include "OBA.H"
#include "PLAYER.H"
#include "PL_WEP.H"
#include "SCA.H"
#include "SCE1.H"
#include "SCE_AT.H"
#include "SND.H"
#include "FLAG.H"
#include "MONITOR.H"
#include "EM_SUB.H"

VOID Spl_Auto(VOID)
{
#ifdef __INTELLISENSE__  // __asm__ label is GNU only
    extern INT (*Spl_walk_tbl_ptr[])();
#else
    extern INT (*Spl_walk_tbl_ptr[])() __asm__("Spl_walk_tbl");
#endif
    ENEMY_WORK* pEm3;
    ENEMY_WORK* pEm;
    SUBPLAY_WORK* pEm2;

    if (EM_CUR->Routine_1 == 0) // pEm re-typed in place; plain MAIN.pEm changes scheduling
    {
        Spl_auto_set();
    }
    Em_sikai_proc();
    Spl_walk_chg();
    pEm = MAIN.pEm;
    MAIN_PEM(SUBPLAY_WORK)->zone = Guide_check((SHORT)pEm->Pos_x, (SHORT)pEm->Pos_z);  // pEm re-typed in place as the player split work (see MAIN_PEM); the SHORT casts narrow the positions
    Spl_walk_tbl_ptr[MAIN.pEm->Routine_2]();
    pEm2 = MAIN_PEM(VOID);
    MAIN_PEM(SUBPLAY_WORK)->Seq_end = Joint_move(pEm2->Mot_flg & 1, pEm2->pKan_t_ptr, pEm2->pSeq_t_ptr, 0x1000 / (pEm2->Hokan_flg + 1));
    Spl_foot_se_set();
    Oba_set_ofs(MAIN.pEm);
    Oba_ck_hit((ENEMY_WORK*)&MAIN.Player, MAIN.pEm);
    Oba_ck_em();
    pEm3 = MAIN.pEm;
    Sca_ck_hit(POS_VEC(pEm3), pEm3->pAt_tbl->At_w);
}

VOID Spl_walk00(VOID)
{
    INT Side_flg;
    UCHAR state;
    SUBPLAY_WORK* pEm;
    SUBPLAY_WORK* pEm2;
    SUBPLAY_WORK* pEm3;

    pEm = MAIN_PEM(VOID);
    state = pEm->Routine_3;
    switch (state)
    {
    case 0:
        pEm->Routine_3 = 1U;
        MAIN.pEm->Move_no = 0;
        MAIN.pEm->Move_cnt = 0;
        MAIN.pEm->Move_wait = 0;
        MAIN.pEm->Hokan_flg = 7;
        MAIN_PEM(SUBPLAY_WORK)->Idle_timer = (rand() & 0x38) + 0x40;
        Spl_neck_init();
        return;

    case 1:
        pEm->Idle_timer--;
        pEm2 = MAIN_PEM(VOID);
        Side_flg = 0;
        if (pEm2->Idle_timer == 0)
        {
            pEm2->Routine_3 = 2U;
            MAIN.pEm->Move_no = 5;
            MAIN.pEm->Move_cnt = 0;
            MAIN.pEm->Move_wait = 0;
            MAIN.pEm->Hokan_flg = 7;
            Spl_neck_init(0);
        case 2:
            pEm3 = MAIN_PEM(VOID);
            if (pEm3->Seq_end & 1)
            {
                pEm3->Routine_3 = 3U;
                MAIN.pEm->Move_no = 6;
                MAIN.pEm->Move_cnt = 0;
                MAIN.pEm->Move_wait = 0;
                MAIN.pEm->Hokan_flg = 7;
            case 3:
                Side_flg = G.pEm->Id;
                Side_flg = Side_flg & 1;
                Side_flg = Side_flg ^ 1;
                goto block_13;
            }
        }
        else
        {
        block_13:
            Spl_neck_rnd_set(Side_flg);
        }
        return;
    }
}

VOID Spl_walk01(VOID)
{
    UINT Hani_lo;
    ENEMY_WORK* pEm2;
    SUBPLAY_WORK* pEm3;
    ENEMY_WORK* pEm;

    Spl_dir_set();
    pEm = EM_CUR;
    if (pEm->Routine_3 == 0)
    {
        pEm->Routine_3 = 1U;
        pEm2 = MAIN.pEm;
        if (pEm2->Move_no != 7)
        {
            pEm2->Move_no = 7U;
            MAIN.pEm->Move_cnt = 0;
            MAIN.pEm->Move_wait = 0;
            MAIN.pEm->Hokan_flg = 7;
        }
    }
    pEm3 = MAIN_PEM(VOID);
    Hani_lo = pEm3->Cdir_y - 0x180;
    if (pEm3->heading - Hani_lo < 0x301U) // width or sign conversion
    {
        Spl_dir_turn(0x30);
        Walk_spd_set(0x5D);
        Spl_add_xz(MAIN.pEm->Spd_cur);
    }
    else
    {
        Spl_dir_turn(0x28);
    }
    Spl_neck_rnd_set(0);
}

VOID Spl_walk02(VOID)
{
    SUBPLAY_WORK* pEm;
    SUBPLAY_WORK* pEm2;

    Spl_dir_set();
    pEm = MAIN_PEM(VOID);
    if (pEm->heading + 0x180 - pEm->Cdir_y < 0x301U) // width or sign conversion
    {
        if (pEm->Move_no != 8)
        {
            pEm->Move_no = 8U;
            MAIN.pEm->Move_cnt = 0;
            MAIN.pEm->Move_wait = 0;
            MAIN.pEm->Hokan_flg = 7;
        }
        Spl_dir_turn(0x60);
        Spl_add_xz(0xD2);
    }
    else
    {
        if (pEm->Move_no != 7)
        {
            pEm->Move_no = 7U;
            MAIN.pEm->Move_cnt = 0;
            MAIN.pEm->Move_wait = 0;
            MAIN.pEm->Hokan_flg = 7;
        }
        Spl_dir_turn(0x30);
        pEm2 = MAIN_PEM(VOID);
        if (pEm2->heading + 0x200 - pEm2->Cdir_y < 0x401U) // width or sign conversion
        {
            Walk_spd_set(0x5D);
            Spl_add_xz(MAIN.pEm->Spd_cur);
        }
    }
    Spl_neck_pos_set(G.pEm->Dest_x, G.pEm->Dest_z);
}

VOID Spl_walk03(VOID)
{
    SHORT Dir_pl;
    SUBPLAY_WORK* pEm;
    SUBPLAY_WORK* pEm2;
    INT Hani_lo;

    Dir_pl = Get_dir_tgt(G_PLAYER->Pos_x, G.Player.Pos_z);
    pEm = MAIN_PEM(VOID);
    pEm->heading = Dir_pl;
    Hani_lo = pEm->Cdir_y - 0x200;
    if (Dir_pl - Hani_lo < 0x401U) // width or sign conversion
    {
        if (pEm->Move_no != 3)
        {
            pEm->Move_no = 3U;
            MAIN.pEm->Move_cnt = 0;
            MAIN.pEm->Move_wait = 0;
            MAIN.pEm->Hokan_flg = 7;
        }
        Spl_dir_turn(0x30);
        Spl_add_xz(-0x3C);
        Spl_neck_pos_set(G.Player.Pos_x, G.Player.Pos_z);
        return;
    }
    if (pEm->Move_no != 7)
    {
        pEm->Move_no = 7U;
        MAIN.pEm->Move_cnt = 0;
        MAIN.pEm->Move_wait = 0;
        MAIN.pEm->Hokan_flg = 7;
    }
    pEm2 = MAIN_PEM(VOID);
    pEm2->heading = (pEm2->heading + 0x800) & 0xFFF;
    Spl_dir_turn(0x30);
    Walk_spd_set(0x5D);
    Spl_add_xz(MAIN.pEm->Spd_cur);
    Spl_neck_init();
}

VOID Spl_auto_set(VOID)
{
    G.pEm->Routine_1 = 1;
    Spl_neck_init();
}

VOID Spl_walk_chg(VOID)
{
    UINT dist;
    INT dx;
    INT dz;
    USHORT Far_dist;
    USHORT Near_dist;
    UCHAR R_no_2;
    UCHAR beh2;
    ENEMY_WORK* pEm;
    ENEMY_WORK* pEm2;

    pEm = EM_CUR;
    dx = pEm->Pos_x - G.Player.Pos_x;
    dz = pEm->Pos_z - G.Player.Pos_z;
    dist = SquareRoot0(dx * dx + dz * dz);
    pEm2 = MAIN.pEm;
    R_no_2 = pEm2->Routine_2;
    Near_dist = Spl_def_tbl.Swap_dist[R_no_2].Near_dist;
    if (Near_dist != 0 && dist < Near_dist)
    {
        (ROUTINE_HALF2(pEm2)) = Spl_def_tbl.Swap_next[R_no_2].Near_next; // pointer re-typed; type-only conversion
        return;
    }
    beh2 = G.pEm->Routine_2;
    Far_dist = Spl_def_tbl.Swap_dist[beh2].Far_dist;
    if (Far_dist != 0 && dist >= Far_dist)
    {
        {
            INT Tmp = Spl_def_tbl.Swap_next[beh2].Far_next;
            EM_ROUTINE_HALF2 = Tmp;
        }
    }
}

VOID Spl_dir_set(VOID)
{
    SUBPLAY_WORK* pEm;
    UCHAR r;
    INT x;
    INT z;
    SHORT t;

    MAIN_PEM(SUBPLAY_WORK)->Next_zone = Root_ck(G_PLAYER->Pos_x, G.Player.Pos_z, &EM_CUR->Dest_x,
        &MAIN.pEm->Dest_z);
    if (MAIN_PEM(SUBPLAY_WORK)->Next_zone & 0x10)
    {
        MAIN_PEM(SUBPLAY_WORK)->heading = Get_dir_tgt(G.Player.Pos_x, G.Player.Pos_z);
        return;
    }
    r = Root_edge_set(MAIN_PEM(SUBPLAY_WORK)->zone, MAIN_PEM(SUBPLAY_WORK)->Next_zone);
    if (r)
    {
        z = ScratchWord[1];
        x = (G.Player.Pos_x - MAIN.pEm->Pos_x) * (z - MAIN.pEm->Pos_z);
        x = x / (G.Player.Pos_z - MAIN.pEm->Pos_z);
        x += MAIN.pEm->Pos_x;
    }
    else
    {
        x = ScratchWord[0];
        z = (G.Player.Pos_z - MAIN.pEm->Pos_z) * (x - MAIN.pEm->Pos_x);
        z = z / (G.Player.Pos_x - MAIN.pEm->Pos_x);
        z += MAIN.pEm->Pos_z;
    }
    if (Spl_hani_ck(r, x, z) && !Sca_ck_point(x, z))
    {
        t = Get_dir_tgt(x, z); // callee declared with another prototype; cast gives the call type
        pEm = (SUBPLAY_WORK*)MAIN.pEm;
        pEm->heading = t;
        pEm->Dest_x = x;
        pEm->Dest_z = z;
        return;
    }
    *(USHORT*)&MAIN_PEM(SUBPLAY_WORK)->heading = Root_edge_dir(EM_CUR->Pos_x, MAIN.pEm->Pos_z,  // USHORT* view of the same memory
        MAIN_PEM(SUBPLAY_WORK)->Next_zone, MAIN_PEM(SUBPLAY_WORK)->zone);
    pEm = (SUBPLAY_WORK*)MAIN.pEm;
    pEm->Dest_x = ScratchWord[0];
    pEm->Dest_z = ScratchWord[1]; // plain ScratchWord field changes scheduling
}

UCHAR Spl_hani_ck(UCHAR mode, SHORT x, SHORT z)
{
    UCHAR* pTbl;
    BLOCK_REC* pBlk_a;
    BLOCK_REC* pBlk_b;
    USHORT pos;
    USHORT lo;
    USHORT hi;
    INT d;
    SHORT o;

    pTbl = G.pRoom->pBlock;

    pBlk_a = (BLOCK_REC*)(((SUBPLAY_WORK*)G.pEm)->zone * sizeof(BLOCK_REC) + (INT)pTbl + 2);  // reinterprets the memory as BLOCK_REC
    pBlk_b = (BLOCK_REC*)(((SUBPLAY_WORK*)G.pEm)->Next_zone * sizeof(BLOCK_REC) + (INT)pTbl + 2);  // reinterprets the memory as BLOCK_REC

    if (mode)
    {
        pos = x;
        if (pBlk_a->Min_x > pBlk_b->Min_x)
        {
            lo = pBlk_a->Min_x;
        }
        else
        {
            lo = pBlk_b->Min_x;
        }
        if (pBlk_a->Max_x < pBlk_b->Max_x)
        {
            hi = pBlk_a->Max_x;
        }
        else
        {
            hi = pBlk_b->Max_x;
        }
    }
    else
    {
        pos = z;
        if (pBlk_a->Min_z > pBlk_b->Min_z)
        {
            lo = pBlk_a->Min_z;
        }
        else
        {
            lo = pBlk_b->Min_z;
        }
        if (pBlk_a->Max_z < pBlk_b->Max_z)
        {
            hi = pBlk_a->Max_z;
        }
        else
        {
            hi = pBlk_b->Max_z;
        }
    }
    d = ((hi - lo) >> 3) + 0x280;
    if (mode)
    {
        o = z;
        if (Sca_ck_point(lo, o))
        {
            lo += d;
        }
        if (Sca_ck_point(hi, o))
        {
            hi -= d;
        }
    }
    else
    {
        o = x;
        if (Sca_ck_point(o, lo))
        {
            lo += d;
        }
        if (Sca_ck_point(o, hi))
        {
            hi -= d;
        }
    }
    if (lo > hi)
    {
        return 0;
    }
    return (pos >= lo) & (pos < hi);
}

VOID Spl_dir_turn(INT base_step)
{
    SHORT target;
    SHORT cur;
    SHORT step;
    ENEMY_WORK** ppEm;

    target = ((SUBPLAY_WORK*)G.pEm)->heading;  // reinterprets the memory as SUBPLAY_WORK
    cur = G.pEm->Cdir_y;
    step = base_step + (G.pEm->Id & 1) * 8;
    if ((UINT)(target + step - cur) <= step * 2) // width or sign conversion
    {
        G.pEm->Cdir_y = target;
        return;
    }
    if ((target - cur) & 0x800)
    {
        G.pEm->Cdir_y = cur - step;
    }
    else
    {
        G.pEm->Cdir_y = step + cur;
    }
    ppEm = &G.pEm;
    (*ppEm)->Cdir_y &= 0xFFF;
}

VOID Spl_add_xz(SHORT dist)
{
    ENEMY_WORK* pEm;

    ScratchSv[0] = dist; // plain Sv field changes scheduling
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0;
    Rot_vector(EM_CUR->Cdir_y, &ScratchPad->Sv, &ScratchPad->Sv); // pEm re-typed in place; plain MAIN.pEm changes scheduling
    pEm = MAIN.pEm;
    pEm->Pos_x += ScratchPad->Sv.vx;
    pEm->Pos_z += ScratchPad->Sv.vz;
}

VOID Spl_neck_init(VOID)
{
    ENEMY_WORK* pEm2;
    SUBPLAY_WORK* pEm;

    MAIN_PEM(SUBPLAY_WORK)->Neck_timer = ((rand() & 0x18) + 0x30) >> 1;
    pEm = (SUBPLAY_WORK*)MAIN.pEm;
    VIEW(INT, pEm->Neck_rnd0) = 0;  // INT* view of the field (a plain access changes the code)
    pEm->Neck_flg = 0x10;
    pEm2 = MAIN.pEm;
    pEm2->Neck_point_x = 0;
    pEm2->Neck_point_y = 0;
    pEm2->Neck_point_z = 0;
    pEm2->Neck_spd_y = 0xC0;
    MAIN.pEm->Neck_spd_x = 0x40;
}

VOID Spl_neck_rnd_set(INT Side_flg)
{
    INT mask;
    UINT r;
    INT t;
    INT v;
    UCHAR a;
    UCHAR b;
    UCHAR c;
    UCHAR d;

    EM_CUR->Neck_flg = 0x33; // pEm re-typed in place; plain MAIN.pEm changes scheduling
    MAIN_PEM(SUBPLAY_WORK)->Neck_timer--;
    mask = (((Side_flg & 1) ^ 1) + 1) * 4 - 1;
    if (MAIN_PEM(SUBPLAY_WORK)->Neck_timer == 0)
    {
        MAIN_PEM(SUBPLAY_WORK)->Neck_timer = (rand() & 0x18) + 0x30;
        r = rand();
        t = r & mask;
        a = t;
        r &= 0xFFFF;
        b = (r >> 4) & 7;
        c = (r >> 8) & 0x18;
        d = (r >> 12) & 8;

        if (MAIN_PEM(SUBPLAY_WORK)->Neck_rnd0 == a || ((a | MAIN_PEM(SUBPLAY_WORK)->Neck_rnd0) & 3) == 0)
        {
            a = mask & (t + 1);
        }
        MAIN_PEM(SUBPLAY_WORK)->Neck_rnd0 = a;
        v = (a & 3) * 0x60;
        EM_CUR->Neck_point_y = v; // pEm re-typed in place; plain MAIN.pEm changes scheduling
        if (!(a & 4))
        {
            MAIN.pEm->Neck_point_y = -v;
        }
        EM_CUR->Neck_point_y &= 0xFFF; // pEm re-typed in place; plain MAIN.pEm changes scheduling

        if (MAIN_PEM(SUBPLAY_WORK)->Neck_rnd1 == b || ((b | MAIN_PEM(SUBPLAY_WORK)->Neck_rnd1) & 3) == 0)
        {
            b = (b + 1) & 7;
        }
        MAIN_PEM(SUBPLAY_WORK)->Neck_rnd1 = b;
        v = (b & 3) * 0xA0;
        EM_CUR->Neck_point_x = v; // pEm re-typed in place; plain MAIN.pEm changes scheduling
        if (b & 4)
        {
            MAIN.pEm->Neck_point_x = -v;
        }
        EM_CUR->Neck_point_x &= 0xFFF; // pEm re-typed in place; plain MAIN.pEm changes scheduling
        if (MAIN_PEM(SUBPLAY_WORK)->Neck_rnd2 == c)
        {
            c = (c + 8) & 0x18;
        }
        MAIN_PEM(SUBPLAY_WORK)->Neck_rnd2 = c;
        MAIN.pEm->Neck_spd_x = c + 0x14;
        if (MAIN_PEM(SUBPLAY_WORK)->Neck_rnd3 == d)
        {
            d = (d + 8) & 0x18;
        }
        MAIN_PEM(SUBPLAY_WORK)->Neck_rnd3 = d;
        MAIN.pEm->Neck_spd_y = d + 0x28;
    }
}

VOID Spl_neck_pos_set(SHORT x, SHORT z)
{
    SUBPLAY_WORK* pEm;
    ENEMY_WORK* pEm2;

    MAIN_PEM(SUBPLAY_WORK)->Neck_timer = ((rand() & 0x18) + 0x30) >> 1;
    pEm = (SUBPLAY_WORK*)MAIN.pEm;
    VIEW(INT, pEm->Neck_rnd0) = 0;  // INT* view of the field (a plain access changes the code)
    pEm->Neck_flg = 0x11;
    pEm2 = MAIN.pEm;
    pEm2->Neck_point_x = x;
    pEm2->Neck_point_y = 0;
    pEm2->Neck_point_z = z;
    pEm2->Neck_spd_y = 0xC0;
    MAIN.pEm->Neck_spd_x = 0x40;
}

VOID Spl_foot_se_set(VOID)
{
    SCHAR v;

    switch (G.pEm->Move_no)
    {
    case 3:
    case 7:
        if (G.pEm->Move_cnt == 8)
        {
            v = (G.pEm->Id & 1) * 2;
            Snd_se_walk(0, v);
        }
        else if (G.pEm->Move_cnt == 0x16)
        {
            v = (G.pEm->Id & 1) * 2 - 4;
            Snd_se_walk(0, v);
        }
        break;
    case 8:
        if (G.pEm->Move_cnt == 0)
        {
            v = (G.pEm->Id & 1) * 2;
            Snd_se_walk(1, v);
        }
        else if (G.pEm->Move_cnt == 0xA)
        {
            v = (G.pEm->Id & 1) * 2 - 4;
            Snd_se_walk(1, v);
        }
        break;
    }
}

VOID Spl(VOID)
{
#ifdef __INTELLISENSE__  // __asm__ label is GNU only
    extern INT (*Spl_routine_0_ptr[])();
#else
    extern INT (*Spl_routine_0_ptr[])() __asm__("Spl_routine_0");
#endif
    ENEMY_WORK* pEm2;
    ENEMY_WORK* pEm;

    if (G.Stop_flg & STOP_SUB_PLAYER)
    {
        Spl_routine_0_ptr[G.pEm->Routine_0]();
        Rot_neck();
    }
    pEm = EM_CUR;
    pEm->Coord_flg = 0;
    MAIN.pEm->Zone_in_flg = Hit_ck_point4(POS_VEC(pEm), CCCUT_CUR); // G field re-typed in place; plain field changes scheduling
    pEm2 = MAIN.pEm;
    if (pEm2->Zone_in_flg != 0 && !(pEm2->Sce_flg & 0x40))
    {
        NOPROTO(INT, MulKage)(&pEm2->Pos_x, &pEm2->Kage_work, pEm2->Pos_y, pEm2->Cdir_y);  // narrow parameter of MulKage would convert the argument
    }
}

VOID Spl_Init(VOID)
{
#ifdef __INTELLISENSE__  // __asm__ label is GNU only
    extern INT (*Spl_routine_0_ptr[])();
#else
    extern INT (*Spl_routine_0_ptr[])() __asm__("Spl_routine_0");
#endif
    ENEMY_WORK* pEm;

    pEm = EM_CUR;
    ROUTINE_WORD(pEm) = 1;  // INT* view of the field (a plain access changes the code)
    pEm->Cdir_x = 0;
    pEm->Cdir_z = 0;
    pEm->Life = -1;
    pEm->Scale = 0;
    Init_Parts_work();
    Spl_routine_0_ptr[MAIN.pEm->Id - 22]();
    Oba_set_ofs(MAIN.pEm);
}

VOID Spl_in20(VOID)
{
    ENEMY_WORK* pEm;

    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = -0x50;
    ScratchWord[7] = 0x808080;
    Kage_work_init(&ScratchPad->Sv, &EM_CUR->Kage_work, 0x200, 0x280); // pEm re-typed in place; plain MAIN.pEm changes scheduling
    MAIN.pEm->Hokan_flg = 0;
    pEm = MAIN.pEm;
    Joint_move(0, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x400);
    MAIN.pEm->pAt_tbl = &Spl_def_tbl.sca_info[0];
    Em_wep_load();
}

VOID Spl_in21(VOID)
{
    ENEMY_WORK* pEm;

    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = -0x50;
    ScratchWord[7] = 0x808080;
    Kage_work_init(&ScratchPad->Sv, &EM_CUR->Kage_work, 0x200, 0x280); // pEm re-typed in place; plain MAIN.pEm changes scheduling
    MAIN.pEm->Hokan_flg = 0;
    pEm = MAIN.pEm;
    Joint_move(0, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x400);
    MAIN.pEm->pAt_tbl = &Spl_def_tbl.sca_info[1];
    Em_wep_load();
}

VOID Spl_in22(VOID)
{
    ENEMY_WORK* pEm;

    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = -0x50;
    ScratchWord[7] = 0x808080;
    Kage_work_init(&ScratchPad->Sv, &EM_CUR->Kage_work, 0x200, 0x280); // pEm re-typed in place; plain MAIN.pEm changes scheduling
    MAIN.pEm->Hokan_flg = 0;
    pEm = MAIN.pEm;
    Joint_move(0, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x400);
    MAIN.pEm->pAt_tbl = &Spl_def_tbl.sca_info[2];
    Em_wep_load();
}

VOID Spl_in23(VOID)
{
    PARTS_W* pP_ptr;
    ENEMY_WORK* pEm;
    ENEMY_WORK* pEm2;

    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = -0x50;
    ScratchWord[7] = 0x808080;
    Kage_work_init(&ScratchPad->Sv, &EM_CUR->Kage_work, 0x200, 0x280); // pEm re-typed in place; plain MAIN.pEm changes scheduling
    MAIN.pEm->Hokan_flg = 0;
    if (Flg_ck(&MAIN.SaveData.Scenario_flg2, 0xC0) != 0) // address passed as UINT
    {
        pEm = MAIN.pEm;
        pP_ptr = pEm->pSin_parts_ptr;
        pEm->Move_wait = 0;
        MAIN.pEm->Move_no = 0x33;
        MAIN.pEm->Move_cnt = 0x3D;
        Kage_work_color_set(&MAIN.pEm->Kage_work, 1, 2, 0xFFFF70);
        Kage_work_size_add(&MAIN.pEm->Kage_work, 0x1E0, 0x1E0);
        pP_ptr[1].Be_flg &= 0xFE;
        Model_color_plus(pP_ptr, 0x30, 0x80820, 0x606060);
        Model_color_plus(pP_ptr + 9, 0x30, 0x80820, 0x606060);
        Model_color_plus(pP_ptr + 12, 0x30, 0x80820, 0x606060);
    }
    pEm2 = MAIN.pEm;
    Joint_move(0, pEm2->pKan_t_ptr, pEm2->pSeq_t_ptr, 0x400);
    MAIN.pEm->pAt_tbl = &Spl_def_tbl.sca_info[3];
    Em_wep_load();
}

VOID Spl_in24(VOID)
{
    ENEMY_WORK* pEm2;
    ENEMY_WORK* pEm;
    PARTS_W* pP_ptr;

    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = -0x50;
    ScratchWord[7] = 0x808080;
    Kage_work_init(&ScratchPad->Sv, &EM_CUR->Kage_work, 0x200, 0x280); // pEm re-typed in place; plain MAIN.pEm changes scheduling
    MAIN.pEm->Hokan_flg = 0;

    if (Flg_ck(&MAIN.SaveData.Scenario_flg, 0x37) != 0) // address passed as UINT
    {
        MAIN.pEm->Move_no = 0x30;
        MAIN.pEm->Move_cnt = 0x6D;
        Kage_work_color_set(&MAIN.pEm->Kage_work, 1, 2, 0xFFFF70);
        Kage_work_size_add(&MAIN.pEm->Kage_work, 0x5DC, 0x5DC);
    }
    pEm = MAIN.pEm;
    Joint_move(0, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x400);
    pEm2 = MAIN.pEm;
    pP_ptr = pEm2->pSin_parts_ptr;

    pP_ptr[1].Free5 = 0x10;

    pP_ptr[1].Free3 = 0;
    pP_ptr[1].Free4 = 0;

    pEm2->pAt_tbl = &Spl_def_tbl.sca_info[4];
    if (STAGE_ROOM == 0x1104) // G field re-typed in place; plain field changes scheduling
    {
        pEm2->Be_flg |= EM_BE_NO_HIT;
    }

    Em_wep_load(pEm2);  // passes 1 argument, Em_wep_load declares 0
}

VOID Spl_in25(VOID)
{
    ENEMY_WORK* pEm;

    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0;
    ScratchWord[7] = 0xFFFF50;
    Kage_work_init(&ScratchPad->Sv, &EM_CUR->Kage_work, 0x1F4, 0x2BC); // pEm re-typed in place; plain MAIN.pEm changes scheduling
    MAIN.pEm->Move_no = 0;
    MAIN.pEm->Move_cnt = 0;
    MAIN.pEm->Move_wait = 0;
    MAIN.pEm->Hokan_flg = 0;
    pEm = MAIN.pEm;
    Joint_move(0, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x400);
    MAIN.pEm->pAt_tbl = &Spl_def_tbl.sca_info[5];
}

VOID Spl_in26(VOID)
{
    ENEMY_WORK* pEm;

    ScratchPad->Sv.vx = -0x258;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0xC8;
    ScratchWord[7] = 0xFFFF50;
    Kage_work_init(&ScratchPad->Sv, &EM_CUR->Kage_work, 0x2BC, 0x2BC); // pEm re-typed in place; plain MAIN.pEm changes scheduling
    MAIN.pEm->Move_no = 0;
    MAIN.pEm->Move_cnt = 0;
    MAIN.pEm->Move_wait = 0;
    MAIN.pEm->Hokan_flg = 0;
    pEm = MAIN.pEm;
    Joint_move(0, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x400);
    MAIN.pEm->pAt_tbl = &Spl_def_tbl.sca_info[6];
}

VOID Spl_in27(VOID)
{
    ENEMY_WORK* pEm;

    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0;
    ScratchWord[7] = 0x606060;
    Kage_work_init(&ScratchPad->Sv, &EM_CUR->Kage_work, 0x1F4, 0x2BC); // pEm re-typed in place; plain MAIN.pEm changes scheduling
    MAIN.pEm->Hokan_flg = 0;
    pEm = MAIN.pEm;
    Joint_move(0, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x400);
    MAIN.pEm->pAt_tbl = &Spl_def_tbl.sca_info[7];
}

VOID Spl_in28(VOID)
{
    ENEMY_WORK* pEm;

    ScratchPad->Sv.vx = -0x258;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0xC8;
    ScratchWord[7] = 0x404040;
    Kage_work_init(&ScratchPad->Sv, &EM_CUR->Kage_work, 0x2BC, 0x2BC); // pEm re-typed in place; plain MAIN.pEm changes scheduling
    MAIN.pEm->Hokan_flg = 0;
    pEm = MAIN.pEm;
    Joint_move(0, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x400);
    MAIN.pEm->pAt_tbl = &Spl_def_tbl.sca_info[8];
}

VOID Spl_in29(VOID)
{
    ENEMY_WORK* pEm;

    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0;
    ScratchWord[7] = 0x606060;
    Kage_work_init(&ScratchPad->Sv, &EM_CUR->Kage_work, 0x1F4, 0x2BC); // pEm re-typed in place; plain MAIN.pEm changes scheduling
    MAIN.pEm->Move_no = 0;
    MAIN.pEm->Move_cnt = 0;
    MAIN.pEm->Move_wait = 0;
    MAIN.pEm->Hokan_flg = 0;
    pEm = MAIN.pEm;
    Joint_move(0, pEm->pKan_t_ptr, pEm->pSeq_t_ptr, 0x400);
    MAIN.pEm->pAt_tbl = &Spl_def_tbl.sca_info[9];
}

VOID Spl_Sp_Move(VOID)
{
#ifdef __INTELLISENSE__  // __asm__ label is GNU only
    extern INT (*Spl_sp_tbl_ptr[])();
#else
    extern INT (*Spl_sp_tbl_ptr[])() __asm__("Spl_sp_tbl");
#endif
    Spl_sp_tbl_ptr[G.pEm->Routine_2]();
}

VOID Spl_sp00(VOID)
{
#ifdef __INTELLISENSE__  // __asm__ label is GNU only
    extern INT (*Spl_sp_tbl_ptr[])();
#else
    extern INT (*Spl_sp_tbl_ptr[])() __asm__("Spl_sp_tbl");
#endif
    Spl_sp_tbl_ptr[G.pEm->Id - 28]();
}

VOID Spl_sp_nop(VOID)
{
}

VOID Spl_sp_mot(VOID)
{
    UCHAR state;
    ENEMY_WORK* pEm;

    pEm = EM_CUR;
    state = pEm->Routine_3;
    switch (state)
    {
    case 0:
        pEm->Routine_3 = 1U;
        MAIN.pEm->Move_no = 0;
        MAIN.pEm->Move_cnt = 0;
        MAIN.pEm->Move_wait = 0;
        MAIN.pEm->Hokan_flg = 0;

    case 1:
        Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        return;
    }
}

VOID Spl_sp01(VOID)
{
    ENEMY_WORK* pEm;
    ENEMY_WORK* pEm1;
    ENEMY_WORK* pEm2;
    ENEMY_WORK* pEm3;
    VECTOR save;

    pEm = EM_CUR;
    switch (pEm->Routine_3)
    {
    case 0:
        pEm->Routine_3 = 1;
        MAIN.pEm->Move_cnt = 0;
        MAIN.pEm->Move_wait = 0;
        MAIN.pEm->Move_no = 0x35;
        MAIN.pEm->Hokan_flg = 0;
        MAIN.pEm->Spd_cur = 1000;
    case 1:
        pEm1 = EM_CUR;
        pEm1->Spd_cur -= pEm1->Move_cnt * 15;
        Joint_move(0, pEm1->pKan_t_ptr, pEm1->pSeq_t_ptr, 0x400);
        Add_speedXZ(0x800);
        save = VIEW(VECTOR, EM_CUR->Pos_x); // pEm re-typed in place; plain MAIN.pEm changes scheduling
        pEm2 = EM_CUR;
        ScratchWord[0] = Sca_ck_hit(POS_VEC(pEm2), pEm2->pAt_tbl->At_w);
        VIEW(VECTOR, EM_CUR->Pos_x) = save; // pEm re-typed in place; plain MAIN.pEm changes scheduling
        if (ScratchWord[0] != 0)
        {
            EM_CUR->Routine_3++; // pEm re-typed in place; plain MAIN.pEm changes scheduling
            Xa_cmd(1, 0xA9);
            MAIN.System_flg |= SYS_XA_VOICE;
        }
        break;
    case 2:
        pEm->Routine_3++;
        MAIN.pEm->Move_cnt = 0;
        MAIN.pEm->Move_wait = 0;
        MAIN.pEm->Move_no = 0x36;
        MAIN.pEm->Hokan_flg = 3;
        Snd_se_stad(2, 0x1C, 0, &MAIN.pEm->Pos_x);
    case 3:
        pEm3 = EM_CUR;
        MAIN.pEm->Routine_3 += Joint_move(0, pEm3->pKan_t_ptr, pEm3->pSeq_t_ptr, 0x400);
        Foot_set_pl(0);
        break;
    }
}

VOID Spl_sp02(VOID)
{
    ENEMY_WORK* pEm;
    PARTS_W* pP_ptr;
    ENEMY_WORK* pEm2;

    switch (G.pEm->Routine_3)
    {
    case 0:
        EM_CUR->Routine_3++; // pEm re-typed in place; plain MAIN.pEm changes scheduling
        MAIN.pEm->Move_cnt = 0;
        MAIN.pEm->Move_wait = 0;
        MAIN.pEm->Move_no = 0x33;

        MAIN.pEm->Timer2 = 0xB4;
        MAIN.pEm->Hokan_flg = 3;
        pP_ptr = MAIN.pEm->pSin_parts_ptr;
        pP_ptr[1].Be_flg |= 8;
        *&ScratchPad->Vec = MATRIX_POS(MON_ID_MAT);  // *& form kept: the plain member store changes the scheduling

        Esp_call(0, 3, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);
        Esp_call(0, 3, 0, Monitor.pId_mat, MATRIX_POS_PTR(&pP_ptr[1].Workm), 0);

    case 1:
        if (EM_CUR->Move_cnt < 10) // pEm re-typed in place; plain MAIN.pEm changes scheduling
        {
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = -0x898;

            Esp_call(0, 0, 0, MATRIX_OF(MAIN.pEm), &ScratchPad->Vec, 0);
        }
        if (MAIN.pEm->Move_cnt == 3)
        {
            pP_ptr = MAIN.pEm->pSin_parts_ptr;
            Model_color_plus(pP_ptr, 0x30, 0x80820, 0x606060);
            Model_color_plus(pP_ptr + 9, 0x30, 0x80820, 0x606060);
            Model_color_plus(pP_ptr + 12, 0x30, 0x80820, 0x606060);
        }
        if (MAIN.pEm->Move_cnt == 0x2A)
        {
            Snd_se_stad(2, 0x2F, 0, &MAIN.pEm->Pos_x);
        }
        MAIN.pEm->Routine_3 += Joint_move(0, MAIN.pEm->pKan_t_ptr, MAIN.pEm->pSeq_t_ptr, 0x400);
        Foot_set_pl(0);
        break;
    case 2:
        ScratchSv[0] = -0x384; // plain Sv field changes scheduling
        ScratchPad->Sv.vy = 0;
        ScratchPad->Sv.vz = 0;
        ScratchPad->Mat = *Monitor.pId_mat;
        RotMatrixY(EM_CUR->Cdir_y, &ScratchPad->Mat); // pEm re-typed in place; plain MAIN.pEm changes scheduling
        ApplyMatrixSV(&ScratchPad->Mat, &ScratchPad->Sv, &ScratchPad->Sv);
        pEm = MAIN.pEm;
        pEm->Kage_work.Off_x += ScratchSvU[0];
        pEm->Kage_work.Off_y += ScratchPad->Sv.vy;
        pEm->Kage_work.Off_z += ScratchPad->Sv.vz;
        Kage_work_color_set(&pEm->Kage_work, 1, 2, 0xFFFF50); // pointer re-typed; type-only conversion
        Kage_work_size_add(&MAIN.pEm->Kage_work, -0xC8, -0xC8);
        MAIN.pEm->Routine_3++;
        MAIN.pEm->Timer0 = 0x1E;
        break;
    case 3:
        Kage_work_size_add(&EM_CUR->Kage_work, 0x10, 0x10); // pEm re-typed in place; plain MAIN.pEm changes scheduling
        pEm2 = MAIN.pEm;
        if (--pEm2->Timer0 == 0)
        {
            pEm2->Routine_3++;
        }
        break;
    case 4:
        break;
    }
}

VOID Spl_sp03(VOID)
{
    ENEMY_WORK* pEm;
    ENEMY_WORK* pEm2;
    ENEMY_WORK* pEm3;
    ENEMY_WORK* pEm4;
    ENEMY_WORK* pEm5;
    PARTS_W* pP_ptr;

    pEm = EM_CUR;
    switch (pEm->Routine_3)
    {
    case 0:
        pEm->Routine_3 = 1;
        MAIN.pEm->Move_cnt = 0;
        MAIN.pEm->Move_wait = 0;
        MAIN.pEm->Move_no = 0x30;
        MAIN.pEm->Hokan_flg = 0;
        pEm2 = MAIN.pEm;
        pEm2->Timer0 = 0xFA;
        pEm2->Damage_no = 0x80;
        MAIN.pEm->Be_flg |= EM_BE_NO_HIT | EM_BE_NO_WALL;
        MAIN.pEm->Cdir_y = G.Enemy[1].Cdir_y;
    case 1:
        if (EM_CUR->Move_cnt == 8) // pEm re-typed in place; plain MAIN.pEm changes scheduling
        {
            pP_ptr = MAIN.pEm->pSin_parts_ptr;
            Model_color_plus(pP_ptr, 0x30, 0x80820, 0x606060);
            Model_color_plus(pP_ptr + 1, 0x30, 0x80820, 0x606060);
            Model_color_plus(pP_ptr + 2, 0x30, 0x80820, 0x606060);
            Model_color_plus(pP_ptr + 9, 0x30, 0x80820, 0x606060);
            Model_color_plus(pP_ptr + 12, 0x30, 0x80820, 0x606060);
        }
        if (MAIN.pEm->Move_cnt < 9)
        {
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = -0x5DC;

            Esp_call(0, 0, 0, MATRIX_OF(MAIN.pEm), &ScratchPad->Vec, 0);
        }
        if (MAIN.pEm->Move_cnt >= 0x60)
        {
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

            Esp_call(0, 0, 0, MATRIX_OF(MAIN.pEm), &ScratchPad->Vec, 0);
        }
        pEm4 = MAIN.pEm;
        Null_pos_set(pEm4, pEm4->pKan_t_ptr, pEm4->pSeq_t_ptr);
        pEm5 = MAIN.pEm;
        if (Joint_move(0, pEm5->pKan_t_ptr, pEm5->pSeq_t_ptr, 0x400) != 0)
        {
            MAIN.pEm->Routine_3++;
            Kage_work_color_set(&MAIN.pEm->Kage_work, 1, 2, 0xFFFF50);
            Kage_work_size_add(&MAIN.pEm->Kage_work, -0x64, -0x64);
        }
        break;
    case 2:
        pEm->Be_flg &= ~EM_BE_NO_HIT;
        pEm2 = MAIN.pEm;
        pEm2->Life = -1;
        Kage_work_size_add(&pEm2->Kage_work, 6, 6);
        pEm3 = MAIN.pEm;
        if (--pEm3->Timer0 == 0)
        {
            pEm3->Routine_3++;
        }
        break;
    }
}

// walk behaviour routines, by Routine_2
SPL_FUNC Spl_walk_tbl[4] =
{
    Spl_walk00,   // 0 //
    Spl_walk01,   // 1 //
    Spl_walk02,   // 2 //
    Spl_walk03,   // 3 //
};

// walk behaviour swap distances and targets, and the collision boxes of the NPC characters
SPL_DEF_TBL Spl_def_tbl =
{
    { { 0x0708, 0x1194 }, { 0x0AF0, 0x1964 }, { 0x1194, 0x0000 }, { 0x0000, 0x09C4 } },
    { { 3, 1 }, { 0, 2 }, { 1, 2 }, { 3, 0 } },
    { { 0x8000, 0, -1530, 0, 1530, 422 }, { 0x8000, 0, -1530, 0, 1530, 372 }, { 0x8000, 0, -1530, 0, 1530, 422 }, { 0x8000, 0, -1530, 0, 1530, 372 }, { 0x8000, 0, -1530, 0, 1530, 422 }, { 0x8000, 0, -1530, 0, 1530, 422 }, { 0x8000, 600, -180, -200, 180, 500 }, { 0x8000, 0, -1530, 0, 1530, 422 }, { 0x8000, 600, -180, 0, 180, 500 }, { 0x8000, 0, -1530, 0, 1530, 422 } },
};

// NPC state routines by state (0 init, 1 special move, 8 scenario, 9 auto walk); from entry 10 the character init routines, by id - 22
SPL_FUNC Spl_routine_0[25] =
{
    Spl_Init,                       // 0 //
    Spl_Sp_Move,                    // 1 //
    0,                                    // 2 //
    0,                                    // 3 //
    0,                                    // 4 //
    0,                                    // 5 //
    0,                                    // 6 //
    0,                                    // 7 //
    Spl_Scenario,                         // 8 //
    Spl_Auto,                       // 9 //
    Spl_in20,            // 10 //
    Spl_in21,             // 11 //
    Spl_in22,            // 12 //
    Spl_in23,          // 13 //
    Spl_in24,           // 14 //
    Spl_in25,   // 15 //
    Spl_in26,    // 16 //
    Spl_in27,          // 17 //
    Spl_in28,           // 18 //
    Spl_in29,               // 19 //
    Spl_in22,            // 20 //
    Spl_in22,            // 21 //
    Spl_in23,          // 22 //
    Spl_in22,            // 23 //
    Spl_in24,           // 24 //
};

// idle behaviour routines, by Routine_2; Spl_sp00 selects an entry by id - 28
SPL_FUNC Spl_sp_tbl[19] =
{
    Spl_sp00,     // 0 //
    Spl_sp01,     // 1 //
    Spl_sp02,     // 2 //
    Spl_sp03,     // 3 //
    Spl_sp_nop,         // 4 //
    Spl_sp_nop,         // 5 //
    Spl_sp_nop,         // 6 //
    Spl_sp_nop,         // 7 //
    Spl_sp_nop,         // 8 //
    Spl_sp_mot,   // 9 //
    Spl_sp_mot,   // 10 //
    Spl_sp_nop,         // 11 //
    Spl_sp_nop,         // 12 //
    Spl_sp_mot,   // 13 //
    Spl_sp_nop,         // 14 //
    Spl_sp_nop,         // 15 //
    Spl_sp_nop,         // 16 //
    Spl_sp_nop,         // 17 //
    Spl_sp_nop,         // 18 //
};
