/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM00.C
 */

#include "EM00.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Em00_Init(VOID)
{
    PARTS_W* pP_ptr;
    EM00_HIT_TBL Hit_tbl;
    {
        register EM00_HIT_TBL* p = &Hit_tbl;    // unused register local: shapes the stack layout of the table copy
    }

    Hit_tbl = Em00_hit_tbl;

    EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
    G.pEm->Coord_flg = 0;
    G.pEm->Dummy0c1 = 0;
    G.pEm->Timer0 = 0;
    G.pEm->Timer2 = 0;
    G.pEm->Damage_no = 0;
    G.pEm->pAt_tbl = Em00_sca_tbl[0];
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vx = 0;
    G.pEm->pKage_work = G.pFree_work;
    FUNC(KAGE_WORK_SET)(2);

    ScratchWord[7] = 0xFFFF50;

    FUNC(KAGE_WORK_INIT)(&ScratchPad->Sv, G.pEm->pKage_work, 0x190, 0x190);
    FUNC(INIT_PARTS_WORK)();
    ScratchWord[7] = 0x808080;
    FUNC(KAGE_WORK_INIT)(&ScratchPad->Sv, &G.pEm->Kage_work, 0x2BC, 0x384);

    if (G.Status_flg & STAT_DEMO_MODE)
    {
        G.pEm->Life = Em00_life[G.SaveData.Sce_random & 0xF] - G.SaveData.Sce_random % 22;
    }
    else
    {
        G.pEm->Life = Em00_life[Api_rand() & 0xF] - ((UINT)Api_rand()) % 22;  // unsigned conversion (signed operand)
    }

    if ((G.pEm->Type & 0xF) == 5)
    {
        G.pEm->Routine_3 = 2;
    }
    if ((G.pEm->Type & 0xF) == 0xB)
    {
        G.pEm->Be_flg |= EM_BE_NO_WALL;
        G.pEm->pAt_tbl = Em00_sca_tbl[2];

        FUNC(OBA_SET_OFS)(G.pEm);
    }

    if (G.pEm->Id == 1)
    {
        G.pEm->pAt_tbl = Em00_sca_tbl[1];
    }

    EM00_FREE_P->Walk_flg = 0;
    EM00_FREE_P->Dummy176 = 0;
    EM00_FREE_P->Head_flg = 0;
    EM00_FREE_P->Dir_ctrl_flg = 0;
    EM00_FREE_P->Wander_rno = 0;
    EM00_FREE_P->hit_count = 0;

    EM00_FREE_P->Hit_lim = Hit_tbl.b[G.SaveData.Sce_random & 0x1F];
    EM00_FREE_P->Stagger_timer = Em00_stagger_tbl[G.SaveData.Sce_random & 0x1F];

    EM00_FREE_P->Dummy178 = 0;
    EM00_FREE_P->down_flags = 0;

    EM00_FREE_P->Move_spd = 0x2D;

    G.pEm->Step_sw = 0;
    if ((G.pEm->Type & 0xF) == 0xC)
    {
        G.pEm->Step_sw = 1;
        G.pEm->Type &= 0xF0;
    }
    if ((G.pEm->Type & 0xF) == 0xD)
    {
        G.pEm->Step_sw = 1;
        G.pEm->Type &= 0xF1;
    }
    if ((G.pEm->Type & 0xF) == 0xE)
    {
        G.pEm->Step_sw = 1;
        G.pEm->Type &= 0xF0;
        G.pEm->Life += 0x64;
    }

    EM00_FREE_P->turn_speed = 0x18;
    EM00_FREE_P->push_back_count = 0;
    EM00_FREE_P->Push_timer = 0;

    G.pEm->Hokan_flg = 0;

    G.pEm->Move_no = Em00_move_no_tbl[G.pEm->Type & 0xF];

    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);

    if (G.pEm->Type & 2)
    {
        G.pEm->Pos_y = 1;
    }

    if (G.pEm->Type == 6)
    {
        G.pEm->Be_flg |= EM_BE_NO_HIT | EM_BE_NO_OBJ;
        G.pEm->Life = -1;
        EM_ROUTINE_WORD = ROUTINES(0, 4, 1, 3);
    }
    if (G.pEm->Type == 0xA)
    {
        pP_ptr = G.pEm->pSin_parts_ptr;
        G.pEm->Be_flg |= EM_BE_NO_WALL;
        pP_ptr[9].Be_flg &= 0xFE;
        pP_ptr[10].Be_flg &= 0xFE;
        pP_ptr[11].Be_flg &= 0xFE;
        pP_ptr[12].Be_flg &= 0xFE;
        pP_ptr[13].Be_flg &= 0xFE;
        pP_ptr[14].Be_flg &= 0xFE;
    }

    if (G.pEm->Type & 0x40)
    {
        G.pEm->Routine_0 = 8;
    }
}

// hit counts that knock the zombie down, indexed by a random value 0 to 31
const EM00_HIT_TBL Em00_hit_tbl =
{
    {
        3,   // 0 //
        2,   // 1 //
        3,   // 2 //
        3,   // 3 //
        2,   // 4 //
        3,   // 5 //
        4,   // 6 //
        3,   // 7 //
        2,   // 8 //
        3,   // 9 //
        3,   // 10 //
        4,   // 11 //
        3,   // 12 //
        2,   // 13 //
        3,   // 14 //
        3,   // 15 //
        3,   // 16 //
        2,   // 17 //
        3,   // 18 //
        3,   // 19 //
        3,   // 20 //
        3,   // 21 //
        2,   // 22 //
        3,   // 23 //
        3,   // 24 //
        2,   // 25 //
        3,   // 26 //
        4,   // 27 //
        3,   // 28 //
        4,   // 29 //
        3,   // 30 //
        2,   // 31 //
    },
};

VOID Em00_Main(VOID)
{
    PARTS_W* pP_ptr;
    SVECTOR Probe_neg = { -600, 0, 0, 0 };
    SVECTOR Probe_pos = { 600, 0, 0, 0 };

    pP_ptr = G.pEm->pSin_parts_ptr;
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        Em00_routine_0[G.pEm->Routine_0]();

        EM00_FREE_P->saved_state = EM_ROUTINE_WORD;

        if (EM00_FREE_P->Push_timer != 0)
        {
            EM00_FREE_P->Push_timer--;
        }
        if (G.pEm->Routine_0 != 5)
        {
            if ((G.pEm->Type & 0xF) != 0xB)
            {
                FUNC(OBA_SET_OFS)(G.pEm);
                FUNC(OBA_CK_HIT)(&MAIN.Player, G.pEm);  // address passed as an integer
                FUNC(OBA_CK_EM)();
            }

            G.pEm->Sca_info &= 0xF7;
            if (G.pEm->Type & 2)
            {
                EM00_FREE_P->Dir_ctrl_flg |= FUNC(SCA_CK_HIT)(&G.pEm->Pos_x, G.pEm->pAt_tbl->At_w);

                EM00_FREE_P->Wander_dir = ScratchHalf[12];
                EM00_FREE_P->Dir_ctrl_flg |= FUNC(SCA_CK_POINT2)(&Probe_neg, &Probe_pos);
            }
            else
            {
                EM00_FREE_P->Dir_ctrl_flg |= FUNC(SCA_CK_HIT)(&G.pEm->Pos_x, G.pEm->pAt_tbl->At_w);

                EM00_FREE_P->Wander_dir = ScratchHalf[12];
            }
        }
        if (EM00_FREE_P->Head_flg != 0)
        {
            Em00_Head_bound(&pP_ptr[2], 6);
        }
    }
    G.pEm->Zone_in_flg = FUNC(HIT_CK_POINT4)(&G.pEm->Pos_x, G.Cccut);
    if (G.pEm->Zone_in_flg != 0)
    {
        FUNC(MUL_KAGE)(&G.pEm->Pos_x, &G.pEm->Kage_work, 0, G.pEm->Cdir_y);
    }
    pP_ptr = G.pEm->pSin_parts_ptr;
    if ((pP_ptr[4].Be_flg & 4) && pP_ptr[4].Workm.t[1] == -100)
    {
        if (FUNC(HIT_CK_POINT4)(pP_ptr[4].Workm.t, G.Cccut) != 0)
        {
            FUNC(MUL_KAGE)(pP_ptr[4].Workm.t, G.pEm->pKage_work, 0, pP_ptr[4].Cdir.vy);
        }
    }
}

VOID Em00_Move(VOID)
{
    if (G.pEm->Type & 0x80)
    {
        return;
    }
    Em00_Move_type[(G.pEm->Type & 0xF)]();
    G.pEm->Be_flg &= EM_BE_STATE_MASK;
    if (!(G.pEm->Type & 2) && !(EM00_FREE_P->hit_count & 0x80))
    {
        G.pEm->Be_flg |= 0x40;

        FUNC(EM_CK_ALERT)(0xBB8);
    }
    FUNC(EM_CK_HANI)(0x1194);

    ScratchWord[0] = G.Player.Pos_y - G.pEm->Pos_y;

    if (ScratchWord[0] >= -0x64 && ScratchWord[0] < 0x65)
    {
    }
    else
    {
        G.pEm->Be_flg &= EM_BE_STATE_MASK;
        if (ScratchWord[0] < 0)
        {
            G.pEm->Be_flg |= 0x20;
        }
        else
        {
            G.pEm->Be_flg |= 0x80;
        }
    }

    if (G.pEm->Type != 7 && G.pEm->Type != 5)
    {
    }
    else
    {
        G.pEm->Be_flg &= EM_BE_STATE_MASK;
        FUNC(EM_CK_HANI)(0xFA0);
    }

    if (EM00_FREE_P->down_flags & 4)
    {
        G.pEm->Be_flg |= 0x40;
    }
}

VOID Em00_Move_chase(VOID)
{
    VOID* Rsv_init = G.pEm->pSin_parts_ptr;    // unused local: reserves stack space
    INT unused;    // unused local: reserves stack space

    if (!(EM00_FREE_P->down_flags & 4) && !(EM00_FREE_P->hit_count & 0x80))
    {
        if (FUNC(L_POS_CK_XZ)(0x2BC, 0x5DC, &MAIN.Player.Pos_x) != 0)  // address passed as an integer
        {
            if (FUNC(SIKAI_CK)(&MAIN.Player.Pos_x) == 0 && G.Player.Damage_no == 0)
            {
                G.pEm->Cdir_y = FUNC(GET_DIR_TGT)(G.Player.Pos_x, G.Player.Pos_z);
                EM_ROUTINE_HALF0 = 5;
                G.pEm->Routine_3 = 0;
                Em00_Atck_main();
                return;
            }
        }
    }
    if (G.pEm->Routine_1 == 0)
    {
        Em00_Mv_run();
    }
    Em00_Act_run();
}

VOID Em00_Move_push(VOID)
{
    VOID* Rsv_init = G.pEm->pSin_parts_ptr;    // unused local: reserves stack space
    INT unused;    // unused local: reserves stack space
    if (FUNC(L_POS_CK_XZ)(0x200, 0x898, &MAIN.Player.Pos_x) != 0)  // address passed as an integer
    {
        if (FUNC(SIKAI_CK)(&MAIN.Player.Pos_x) == 0 && G.Player.Damage_no == 0)
        {
            G.pEm->Cdir_y = FUNC(GET_DIR_TGT)(G.Player.Pos_x, G.Player.Pos_z);
            EM_ROUTINE_HALF0 = 5;
            G.pEm->Routine_3 = 0;
            Em00_Atck_main();
            return;
        }
    }
    if (G.pEm->Routine_1 == 0)
    {
        Em00_Mv_run();
    }
    Em00_Push_run();
}

VOID Em00_Move_rand(VOID)
{
    PARTS_W* pP_ptr;

    if (!(EM00_FREE_P->down_flags & 4) && !(EM00_FREE_P->hit_count & 0x80) && !(G.pEm->Sca_info & 8))
    {
        pP_ptr = G.pEm->pSin_parts_ptr;

        if (FUNC(L_POS_CK_XZ)(0x2BC, 0x5DC, &MAIN.Player.Pos_x) != 0)  // address passed as an integer
        {
            if (FUNC(SIKAI_CK)(&MAIN.Player.Pos_x) == 0 && G.Player.Damage_no == 0)
            {
                G.pEm->Cdir_y = FUNC(GET_DIR_TGT)(G.Player.Pos_x, G.Player.Pos_z);
                EM_ROUTINE_HALF0 = 5;
                G.pEm->Routine_3 = 0;
                Em00_Atck_main();
                return;
            }
        }
    }
    if (G.pEm->Routine_1 == 0)
    {
        Em00_Mv_run();
    }
    Em00_Act_run();
}

VOID Em00_Damage(VOID)
{
    UCHAR Mv_type = 0;
    EM00_HIT_TBL Hit_tbl;
    {
        register EM00_HIT_TBL* p = &Hit_tbl;    // unused register local: shapes the stack layout of the table copy
    }

    Hit_tbl = Em00_hit_tbl;

    if (G.pEm->Routine_1 == 0)
    {
        Mv_type = G.pEm->Type & 0xF;

        if (EM00_FREE_P->down_flags & 4)
        {
            EM_ROUTINE_WORD = EM00_FREE_P->saved_state;
            G.pEm->Damage_no = 0U;
            return;
        }

        if (EM00_FREE_P->hit_count & 0x80)
        {
            EM_ROUTINE_WORD = ROUTINES(3, 7, 1, 1);
            G.pEm->Hokan_flg = 0;
            G.pEm->Damage_no = 1U;
            if (!(EM00_FREE_P->down_flags & 1))
            {
                FUNC(SND_SE_ENEM)(9, 0);
            }
            return;
        }
        if ((G.pEm->Damage_no & 0x78) == 8)
        {
            if ((++EM00_FREE_P->hit_count >= EM00_FREE_P->Hit_lim) && !(G.pEm->Type & 2))
            {
                EM00_FREE_P->hit_count = -0x80;

                EM00_FREE_P->Hit_lim = Hit_tbl.b[G.SaveData.Sce_random & 0x1F];
            }
        }
        if ((G.pEm->Damage_no & 0x78) == 0x10 && G.pEm->Routine_3 == 0)
        {
            if ((--EM00_FREE_P->Stagger_timer == 0) && !(G.pEm->Type & 2))
            {
                EM00_FREE_P->hit_count = -0x80;
            }
        }
        G.pEm->Routine_2 = Em00_dm_sel_tbl[((G.pEm->Type & 2) >> 1) & 0xFF][(G.pEm->Damage_no & 7) - 1];
        if (((G.pEm->Damage_no & 7) - 1) & 2)
        {
            G.pEm->Routine_2 += (((UINT)FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400)) >> 10) & 1;  // address passed as an integer
        }

        if (Mv_type == 7 || Mv_type == 5)
        {
            G.pEm->Routine_2 = 7U;

            if (G.pEm->Move_no == 0xD)
            {
                G.pEm->Routine_3 = 1U;
            }
        }
        G.pEm->Routine_1 = 1U;
    }
    Em00_dm_rtn_tbl[G.pEm->Routine_2]();
}

VOID Em00_Die(VOID)
{
    PARTS_W* pP_ptr;

    if (G.pEm->Routine_1 == 0)
    {
        G.pEm->Routine_1 = 1U;
        G.pEm->Routine_2 = 0U;

        if (!(G.pEm->Type & 2) && !(EM00_FREE_P->hit_count & 0x80) && G.pEm->Type != 5)
        {
        }
        else
        {
            G.pEm->Routine_2 = 2U;
        }
        if ((G.pEm->Damage_no & 7) == 4)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400) == 0)  // address passed as an integer
            {
                G.pEm->Routine_2 = 1U;
            }
        }
        if (G.pEm->Routine_2 == 1)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;

            if ((pP_ptr[2].Be_flg & 0x40) && (G.SaveData.Sce_random & 1))
            {
                G.pEm->Routine_2 = 3U;
            }
        }

        EM00_FREE_P->down_flags &= 0xFB;

        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg, G.pEm->Em_set_flg);  // address passed as an integer
    }
    switch (G.pEm->Routine_2)
    {
    case 0:
        G.pEm->Move_no = 8;
        G.pEm->Spd_cur = 0x14U;
        if (G.pEm->Move_wait == 1)
        {
            if (G.pEm->Move_cnt == 8)
            {
                FUNC(SND_SE_ENEM)(1, 0U);
            }
            if (G.pEm->Move_cnt == 0x1A)
            {
                FUNC(SND_SE_ENEM)(0, 0U);
            }
        }
        Em00_Die_mot();
        FUNC(ADD_SPEED_XZ)(0);
        return;
    case 1:
        G.pEm->Move_no = 0xA;
        G.pEm->Spd_cur = 0x14U;
        if (G.pEm->Move_cnt == 0x12 && G.pEm->Move_wait == 1)
        {
            FUNC(SND_SE_ENEM)(0, 0U);
        }
        if (G.pEm->Move_cnt < 5U)
        {
            G.pEm->Spd_cur += 0x5A;
        }
        Em00_Die_mot();
        FUNC(ADD_SPEED_XZ)(0x800);
        return;
    case 2:
        G.pEm->Move_no = 0x10;
        Em00_Die_mot();
        return;
    case 3:
        Em00_Die_walk();
        return;
    }
}

VOID Em00_Dead(VOID)
{
}

VOID Em00_Atck(VOID)
{
    Em00_Atck_main();
}

VOID Em00_Act_run(VOID)
{
    Em00_act_tbl[G.pEm->Routine_2]();
}

VOID Em00_Push_run(VOID)
{
    if (G.pEm->Routine_2 == 0)
    {
        Em00_Push_idle();
        return;
    }
    Em00_Push_stagger();
}

VOID Em00_Mv_run(VOID)
{
    if (EM00_FREE_P->Walk_flg != 0)
    {
        FUNC(ROOT_CK)(G.Player.Pos_x, G.Player.Pos_z, &G.pEm->Dest_x, &G.pEm->Dest_z);
    }
    ScratchWord[0] = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);
    Em00_mv_tbl[(G.pEm->Type & 0xF)]();
}

VOID Em00_Mv_walk1(VOID)
{
    ScratchPad->Vec.vx = G.pEm->Dest_x;
    ScratchPad->Vec.vz = G.pEm->Dest_z;
    ScratchPad->Vec.vy = 0;
    ScratchWord[7] = 6;
    ScratchWord[4] = G.pEm->Routine_2;
    if (EM00_FREE_P->Walk_flg == 0)
    {
        ScratchWord[5] = FUNC(EM_SIKAI_PROC)();
        if ((ScratchWord[5] >> 1) == 0)
        {
            EM00_FREE_P->Walk_flg = EM00_FREE_P->Walk_flg & ~1;
            EM00_FREE_P->Walk_flg = EM00_FREE_P->Walk_flg | (ScratchWord[5] & 1);
        }
    }
    if (EM00_FREE_P->hit_count & 0x80)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 7, 1, 1);
        return;
    }
    if (G.pEm->Routine_2 != 0 && G.pEm->Routine_2 != 1)
    {
        ScratchWord[7] = 7;
        FUNC(EM_WANDER_TURN)(EM00_FREE_P->Wander_dir, &EM00_FREE_P->Dir_ctrl_flg, &EM00_FREE_P->Wander_rno, EM00_FREE_P->turn_speed, 0x3C);
    }
    if (ScratchWord[0] < 0xBB8)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400) != 0)  // address passed as an integer
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_UHALF2 = 3;
            return;
        }
    }
    if (EM00_FREE_P->Walk_flg != 0 && G.Player.Damage_no == 0)
    {
        if (ScratchWord[4] != 2)
        {
            G.pEm->Routine_3 = 0;
            G.pEm->Hokan_flg = 3;
        }
        G.pEm->Routine_2 = 2U;
        G.pEm->Routine_1 = 0;
    }
    if (!(G.Player.Damage_no & 0x80))
    {
        return;
    }
    if (--EM00_FREE_P->Walk_wait != 0)
    {
        return;
    }
    if (ScratchWord[5] & 1)
    {
        if (ScratchWord[6] != 2)
        {
            G.pEm->Routine_3 = 0;
            G.pEm->Hokan_flg = 3;
        }
        G.pEm->Routine_2 = 2U;
        G.pEm->Routine_1 = 0;
    }
    if (ScratchWord[0] < 0x4B0)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x2C8) == 0)  // address passed as an integer
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_UHALF2 = 4;
        }
    }
}

VOID Em00_Pl_dist_ck(VOID)
{
    G.pEm->Timer0++;
    ScratchPad->Vec.vx = G.pEm->Dest_x;
    ScratchPad->Vec.vz = G.pEm->Dest_z;
    ScratchPad->Vec.vy = 0;
    G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&ScratchPad->Vec, 4U);
    if ((FUNC(EM_SIKAI_PROC)() & 1) && ScratchWord[0] < 0x2328)
    {
        G.pEm->Type--;
        if (G.pEm->Type == 7)
        {
            G.pEm->Type = 4U;
        }
        return;
    }
    if (G.pEm->Routine_2 == 0 && ScratchWord[0] >= 0xBB8)
    {
        return;
    }
    G.pEm->Type--;
    if (G.pEm->Type == 7)
    {
        G.pEm->Type = 4U;
    }
}

VOID Em00_Mv_slow(VOID)
{
    ScratchWord[2] = __builtin_abs(G.pEm->Dest_x - G.pEm->Pos_x) + __builtin_abs(G.pEm->Dest_z - G.pEm->Pos_z);
    ScratchWord[1] = G.pEm->Routine_2;

    if (EM00_FREE_P->Walk_flg == 0)
    {
        ScratchWord[5] = FUNC(EM_SIKAI_PROC)();
        if ((ScratchWord[5] >> 1) == 0)
        {
            EM00_FREE_P->Walk_flg = EM00_FREE_P->Walk_flg & ~1;
            EM00_FREE_P->Walk_flg = EM00_FREE_P->Walk_flg | (ScratchWord[5] & 1);
        }

        return;
    }

    if (ScratchWord[1] != 1)
    {
        G.pEm->Routine_3 = 0;
        G.pEm->Hokan_flg = 3;
    }
    G.pEm->Routine_2 = 1U;
    G.pEm->Routine_1 = 0;
}

VOID Em00_Mv_idle(VOID)
{
    if (G.pEm->Routine_2 != 0)
    {
        EM_ROUTINE_UHALF2 = 0;
    }
    FUNC(EM_SIKAI_PROC)();
}

VOID Em00_Mv_walk2(VOID)
{
    ScratchPad->Vec.vx = G.pEm->Dest_x;
    ScratchPad->Vec.vz = G.pEm->Dest_z;
    ScratchPad->Vec.vy = 0;
    ScratchWord[7] = 6;
    ScratchWord[6] = G.pEm->Routine_2;
    if (EM00_FREE_P->Walk_flg == 0)
    {
        ScratchWord[5] = FUNC(EM_SIKAI_PROC)();
        if ((ScratchWord[5] >> 1) == 0)
        {
            EM00_FREE_P->Walk_flg = EM00_FREE_P->Walk_flg & ~1;
            EM00_FREE_P->Walk_flg = EM00_FREE_P->Walk_flg | (ScratchWord[5] & 1);
        }
    }
    if (G.pEm->Routine_2 != 0 && G.pEm->Routine_2 != 1)
    {
        ScratchWord[7] = 7;
        FUNC(EM_WANDER_TURN)(EM00_FREE_P->Wander_dir, &EM00_FREE_P->Dir_ctrl_flg, &EM00_FREE_P->Wander_rno, EM00_FREE_P->turn_speed, 0x3C);
    }
    if (EM00_FREE_P->hit_count & 0x80)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 7, 1, 1);
        return;
    }
    if (ScratchWord[0] < 0xBB8)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400) != 0)  // address passed as an integer
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_UHALF2 = 3;
            return;
        }
    }
    if ((G.pEm->Sca_info & 8) && ScratchWord[0] < 0x7D0)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200) == 0)  // address passed as an integer
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_UHALF2 = 6;

            if (G.Player.Damage_no != 0)
            {
                G.pEm->Routine_3 = 2;
            }

            return;
        }
    }
    if (EM00_FREE_P->Walk_flg != 0 && G.Player.Damage_no == 0)
    {
        if (ScratchWord[6] != 2)
        {
            G.pEm->Routine_3 = 0;
            G.pEm->Hokan_flg = 3;
        }
        G.pEm->Routine_2 = 2U;
        G.pEm->Routine_1 = 0;
    }
    if (!(G.Player.Damage_no & 0x80))
    {
        return;
    }
    if (--EM00_FREE_P->Walk_wait != 0)
    {
        return;
    }
    if (ScratchWord[5] & 1)
    {
        if (ScratchWord[6] != 2)
        {
            G.pEm->Routine_3 = 0;
            G.pEm->Hokan_flg = 3;
        }
        G.pEm->Routine_2 = 2U;
        G.pEm->Routine_1 = 0;
    }
    if (!(G.pEm->Sca_info & 8) && ScratchWord[0] < 0x4B0)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x2C8) == 0)  // address passed as an integer
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_UHALF2 = 4;
        }
    }
}

VOID Em00_Act_idle(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0;

        G.pEm->Timer0 = (G.SaveData.Sce_random & 0x7F) + 0x32;

        G.pEm->Hokan_flg = 3;

    case 1:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        if (--G.pEm->Timer0 == 0)
        {
            EM_ROUTINE_HALF2 = 1;
        }
        return;
    case 2:
        G.pEm->Routine_3 = 3U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0;
        G.pEm->Hokan_flg = 3;

        G.pEm->Timer0 = (G.SaveData.Sce_random & 0xF) + 8;

    case 3:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 4U;

            G.pEm->Timer0 = (G.SaveData.Sce_random & 0xF) + 8;
        }
        G.pEm->Cdir_y += 0xC;
        return;
    case 4:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 5U;

            G.pEm->Timer0 = (G.SaveData.Sce_random & 0xF) + 4;
        }
        G.pEm->Cdir_y -= 0x18;
        return;
    case 5:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 6U;

            G.pEm->Timer0 = (G.SaveData.Sce_random & 0xF) + 8;
        }
        G.pEm->Cdir_y -= 0x20;
        return;
    case 6:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 7U;

            G.pEm->Timer0 = (G.SaveData.Sce_random & 0xF) + 4;
        }
        G.pEm->Cdir_y += 0x18;
        return;
    case 7:
        EM_ROUTINE_HALF2 = 0;
    }
}

VOID Em00_Act_slow(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Spd_cur = 0x14;
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0U;
        G.pEm->Move_no = 1;
        G.pEm->Hokan_flg = 3;

        G.pEm->Timer0 = (G.SaveData.Sce_random & 0x7F) + 0x12C;

        ScratchPad->Mat = *Monitor.pId_mat;
        ScratchPad->Sv.vx = 0x1388;
        ScratchPad->Sv.vz = 0;
        ScratchPad->Sv.vy = 0;
        FUNC(ROT_MATRIX_Y)(G.pEm->Cdir_y + 8, &ScratchPad->Mat);
        FUNC(APPLY_MATRIX_SV)(&ScratchPad->Mat, &ScratchPad->Sv, &ScratchPad->Sv);
        G.pEm->Dest_x = ScratchPad->Sv.vx + G.pEm->Pos_x;
        G.pEm->Dest_z = ScratchPad->Sv.vz + G.pEm->Pos_z;
    }
    if (G.pEm->Move_wait == 1)
    {
        if (G.pEm->Move_cnt == 0xD)
        {
            FUNC(SND_SE_ENEM)(1, 0);
        }
        if (G.pEm->Move_cnt == 0x15)
        {
            FUNC(SND_SE_ENEM)(2, 0);
        }
    }
    FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400U);
    if (--G.pEm->Timer0 == 0)
    {
        EM_ROUTINE_HALF2 = 0x200;
    }
    ScratchWord[7] = 7;
    FUNC(EM_WANDER_TURN)(EM00_FREE_P->Wander_dir, &EM00_FREE_P->Dir_ctrl_flg, &EM00_FREE_P->Wander_rno, EM00_FREE_P->turn_speed, 0x3C);
    if (G.pEm->Move_cnt >= 0x10U)
    {
        FUNC(ADD_SPEED_XZ)(0U);
    }
}

VOID Em00_Act_chase(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Spd_cur = 0;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0U;
        G.pEm->Move_no = 2;
        G.pEm->Hokan_flg = 3;

    case 1:
        G.pEm->Routine_3 += FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        break;
    case 2:
        G.pEm->Spd_cur = EM00_FREE_P->Move_spd;
        G.pEm->Routine_3 = 3U;
        G.pEm->Move_wait = 0U;
        G.pEm->Move_no = 3;
        G.pEm->Hokan_flg = 3;

        G.pEm->Timer0 = (G.SaveData.Sce_random & 0x7F) + 0xA;

        G.pEm->Turn_timer = 0U;

    case 3:
        if (G.pEm->Move_wait == 1)
        {
            if (G.pEm->Move_cnt == 8)
            {
                FUNC(SND_SE_ENEM)(1, 0);
            }
            if (G.pEm->Move_cnt == 0x1D)
            {
                FUNC(SND_SE_ENEM)(1, 0);
            }
        }
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Turn_timer = (G.SaveData.Sce_random & 0xF) + 0x14;
            G.pEm->Timer0 = G.pEm->Turn_timer + (G.SaveData.Sce_random & 0x7F);
        }
        FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);

        if (G.pEm->Step_sw != 0)
        {
            FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        }

        G.pEm->Spd_cur = 0x2D;

        if (EM00_FREE_P->Move_spd >= 0x2EU)
        {
            G.pEm->Spd_cur = EM00_FREE_P->Move_spd;
        }

        if (G.pEm->Turn_timer != 0)
        {
            ScratchWord[7] = 7;
            FUNC(EM_WANDER_TURN)(EM00_FREE_P->Wander_dir, &EM00_FREE_P->Dir_ctrl_flg, &EM00_FREE_P->Wander_rno, -(EM00_FREE_P->turn_speed * 2), 0x3C);
            G.pEm->Spd_cur = 0xA;
            G.pEm->Turn_timer--;
        }
        ScratchWord[0] = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);
        if (G.Player.Damage_no != 0 && ScratchWord[0] < 0x320)
        {
            G.pEm->Routine_1 = 1;
            G.pEm->Routine_3++;
            G.pEm->Timer0 = 0x3CU;
            G.pEm->Move_cnt = 0U;
            G.pEm->Move_wait = 0U;
            G.pEm->Move_no = 0;
            G.pEm->Hokan_flg = 3;
            G.pEm->Spd_cur = 0;
        }
        break;
    case 4:
        FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);

        if (G.pEm->Step_sw != 0)
        {
            FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        }

        if (--G.pEm->Timer0 == 0)
        {
            G.pEm->Routine_1 = 0;
            G.pEm->Routine_3 = 0U;
        }
    }
    FUNC(ADD_SPEED_XZ)(0U);

    if (G.pEm->Step_sw != 0)
    {
        FUNC(ADD_SPEED_XZ)(0U);
    }
}

VOID Em00_Atck_main(VOID)
{
    extern struct
    {
        UCHAR a;
        UCHAR b;
        UCHAR c;
    } zombie_attack_anim_tbl_view[] ASM_NAME("Em00_attack_anim_tbl");

    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Routine_3 = 1U;
        G.pEm->Hokan_flg = 3;

        G.pEm->Be_flg |= EM_BE_NO_HIT | EM_BE_NO_OBJ;

        FUNC(SND_SE_ENEM)(4, 0U);
        EM00_FREE_P->Catch_dir = FUNC(CDIR_CK)(&MAIN.Player.Be_flg) + (G.pEm->Type & 2);
        G.pEm->Move_no = zombie_attack_anim_tbl_view[EM00_FREE_P->Catch_dir].a;
        G.Player.Move_no = zombie_attack_anim_tbl_view[EM00_FREE_P->Catch_dir].b;

        G.Player.Timer0 = Em00_atck_ofs_tbl[EM00_FREE_P->Catch_dir];

        G.pEm->Damage_no = 1;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        FUNC(SET_BASE_POS)(&MAIN.Player.Be_flg);
        G.Player.Damage_no = 1;
        PL_ROUTINE_WORD = ROUTINES(0, 0, 0, 5);
        G.Player.Cdir_y = G.pEm->Cdir_y;

        FUNC(PAD_VIB_SET0)(0xF, 0U);
        FUNC(PAD_VIB_SET1)(6, 0xFAU, 0);

    case 1:
        FUNC(NULL_POS_SET)(G.pEm, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr);
        G.pEm->Routine_3 += FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        break;
    case 2:
        G.pEm->Routine_3 = 3U;
        G.pEm->Move_no++;
        G.pEm->Move_wait = 0;

        G.pEm->Timer0 = 0U;
        EM00_FREE_P->Catch_timer = 0x69;
        if (G.Player.Turn_timer != 0)
        {
            EM00_FREE_P->Catch_timer = 0x1E;
        }

    case 3:

        if ((G.pEm->Timer0++ % 19) == 0)
        {
            FUNC(PAD_VIB_SET1)(6, 0xFAU, 0);
            FUNC(PAD_VIB_SET0)(0xC, 0U);

            G.Player.Life -= Em00_atck_dm_tbl[G.pEm->Type & 0xF];

            FUNC(SND_SE_ENEM)(3, 0U);
            pP_ptr = G.pEm->pSin_parts_ptr;

            FUNC(ESP_CALL)(0, 0U, (SHORT)G.Player.Cdir_y + 0x800, Monitor.pId_mat, pP_ptr[2].Workm.t, 0);  // sign-extends the halfword

            if (G.Player.Life < 0 && (EM00_FREE_P->Catch_dir & 2))
            {
                G.Player.Life = 1;
            }
        }
        FUNC(NULL_POS_SET)(G.pEm, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr);
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);

        EM00_FREE_P->Catch_timer = EM00_FREE_P->Catch_timer - (Em00_Catch_pad() + 1);

        if (EM00_FREE_P->Catch_timer < 0)
        {
            G.pEm->Routine_3 = 4U;
            G.Player.Routine_3 = 3;
        }
        if (G.Player.Life < 0)
        {
            EM00_FREE_P->Walk_wait = 0x3C;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            pP_ptr = G.pEm->pSin_parts_ptr;

            FUNC(ESP_CALL)(0, 0U, 0x200, &pP_ptr[2].Workm, &ScratchPad->Vec, 0);

            EM_ROUTINE_WORD = ROUTINES(0, 5, 1, 1);
            G.pEm->Routine_2 = G.pEm->Routine_2 - ((G.pEm->Type & 2) >> 1) * 5;
            G.Player.Routine_0 = 1;
            G.Player.Routine_2 = 0xC8;
            Em00_pl_blood_tbl[EM00_FREE_P->Catch_dir](G.Player.pSin_parts_ptr);
        }
        break;
    case 4:
        G.pEm->Routine_3 = 5U;
        G.pEm->Move_no++;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;

        if (G.pEm->Type & 2)
        {
            if ((G.Player.Id & 1) && EM00_FREE_P->Catch_dir == 2)
            {
                G.pEm->Routine_3 = 8U;
                goto head_kicked_off;
            }
            G.pEm->Routine_3 = 6U;
            goto bite_hold;
        }

        FUNC(PAD_VIB_SET1)(3, 0xFAU, 0);

    case 5:
        FUNC(NULL_POS_SET)(G.pEm, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr);
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 6, 1, 2);

            G.pEm->Be_flg &= 0xF5;

            G.pEm->Damage_no = 1;
        }
        break;
    case 6:
    bite_hold:
        FUNC(NULL_POS_SET)(G.pEm, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr);
        G.pEm->Routine_3 += FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);

        if (G.pEm->Move_cnt == zombie_attack_anim_tbl_view[EM00_FREE_P->Catch_dir].c)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            FUNC(JOINT_ATCK_SET)(&pP_ptr[2], 0x1EU, 0, 3);
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

            FUNC(ESP_CALL)(3, 0U, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0);

            ScratchPad->Vec.vx += 0x1F4;

            FUNC(ESP_CALL)(4, 0U, 0x800, G.pEm->M, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(4, 1U, 0x5E8, G.pEm->M, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(4, 2U, 0x9F4, G.pEm->M, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(4, 4U, 0xB84, G.pEm->M, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(4, 2U, 0xDB8, G.pEm->M, &ScratchPad->Vec, 0);

            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[3], 0x30U, 0x80820, 0x606060);
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[6], 0x30U, 0x80820, 0x606060);
            FUNC(SND_SE_ENEM)(6, 0U);
        }
        break;
    case 7:

        FUNC(KAGE_WORK_COLOR_SET)(&G.pEm->Kage_work, 1U, 2, 0xFFFF50);

        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);
        G.pEm->Timer2 = 0x46;
        EM_ROUTINE_WORD = ROUTINES(3, 2, 1, 3);

        G.pEm->Be_flg |= EM_BE_NO_HIT | EM_BE_NO_WALL | EM_BE_NO_OBJ;

        G.pEm->Life = -1;
        G.pEm->Damage_no = 1;
        break;
    case 8:
    head_kicked_off:
        FUNC(NULL_POS_SET)(G.pEm, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr);
        G.pEm->Routine_3 += FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        pP_ptr = G.pEm->pSin_parts_ptr;

        if (G.pEm->Move_cnt == zombie_attack_anim_tbl_view[EM00_FREE_P->Catch_dir].c)
        {
            pP_ptr[2].Be_flg |= 0x88;
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[2], 0x30U, 0x80820, 0x606060);
            pP_ptr[2].Cdir.vx = -0x12C;
            pP_ptr[2].Cdir.vy = 0xFA;
            pP_ptr[2].Fall_cnt = 0;
            pP_ptr[2].Fall_sts = 0;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

            FUNC(ESP_CALL)(0, 0U, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0);
        }
        if (pP_ptr[2].Be_flg & 0x40)
        {
            EM00_FREE_P->Head_flg = 1;
        }
        if (G.Player.Move_cnt == 6)
        {
            FUNC(SND_SE_ENEM)(7, 0U);
        }
        break;
    case 9:

        FUNC(KAGE_WORK_COLOR_SET)(&G.pEm->Kage_work, 1U, 2, 0xFFFF50);

        G.pEm->Timer2 = 0x46;
        EM_ROUTINE_WORD = ROUTINES(3, 2, 1, 3);
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);

        G.pEm->Be_flg |= EM_BE_NO_HIT | EM_BE_NO_WALL | EM_BE_NO_OBJ;

        G.pEm->Life = -1;
        G.pEm->Damage_no = 1;
    }
}

VOID Em00_Pl_blood0(PARTS_W* pP_ptr)
{
    ScratchPtr[5] = G.pEm;
    G.pEm = PL_EM;
    FUNC(MODEL_COLOR_PLUS)(&pP_ptr[12], 0x30, 0x80820, 0x606060);
    FUNC(MODEL_COLOR_PLUS)(&pP_ptr[1], 0x30, 0x80820, 0x606060);
    G.pEm = ScratchPtr[5];
}

VOID Em00_Pl_blood1(PARTS_W* pP_ptr)
{
    ScratchPtr[5] = G.pEm;
    G.pEm = PL_EM;
    FUNC(MODEL_COLOR_PLUS)(&pP_ptr[12], 0x30, 0x80820, 0x606060);
    FUNC(MODEL_COLOR_PLUS)(&pP_ptr[1], 0x30, 0x80820, 0x606060);
    G.pEm = ScratchPtr[5];
}

VOID Em00_Pl_blood2(PARTS_W* pP_ptr)
{
    ScratchPtr[5] = G.pEm;
    G.pEm = PL_EM;
    FUNC(MODEL_COLOR_PLUS)(&pP_ptr[4], 0x30, 0x80820, 0x606060);
    FUNC(MODEL_COLOR_PLUS)(&pP_ptr[5], 0x30, 0x80820, 0x606060);
    G.pEm = ScratchPtr[5];
}

VOID Em00_Pl_blood3(PARTS_W* pP_ptr)
{
    ScratchPtr[5] = G.pEm;
    G.pEm = PL_EM;
    FUNC(MODEL_COLOR_PLUS)(&pP_ptr[8], 0x30, 0x80820, 0x606060);
    FUNC(MODEL_COLOR_PLUS)(&pP_ptr[9], 0x30, 0x80820, 0x606060);
    G.pEm = ScratchPtr[5];
}

VOID Em00_Act_face(VOID)
{
    UCHAR Rsv_init = 169;    // unused local: reserves stack space

    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Damage_no = 0;
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400) != 0)  // address passed as an integer
        {
        }
        G.pEm->Move_no = 3;
        G.pEm->Routine_3 = 1U;

        G.pEm->Timer0 = (G.SaveData.Sce_random & 0x3F) + 0x1E;
    }
    ScratchWord[7] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x54);
    if ((G.pEm->Timer0-- == 0) || ScratchWord[7] == 0)
    {
        EM_ROUTINE_WORD = ROUTINES(2, 2, 0, 1);
        G.pEm->Dest_x = G.Player.Pos_x;
        G.pEm->Dest_z = G.Player.Pos_z;
    }
    else
    {
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        G.pEm->Cdir_y = G.pEm->Cdir_y + ScratchWord[7];
    }
}

VOID Em00_Push_idle(VOID)
{
    G.pEm->Move_no = 9;
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
    }
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
}

VOID Em00_Act_eat(VOID)
{
    PARTS_W* pPl_parts;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3++;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0U;
        G.pEm->Timer0 = 0x3CU;
        G.pEm->Hokan_flg = 3;
        return;
    case 1:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        if (--G.pEm->Timer0 == 0)
        {
            G.pEm->Routine_3++;
        }
        return;
    case 2:
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Routine_3++;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 0xBU;

    case 3:
        G.pEm->Routine_3 += FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        G.pEm->Cdir_y += FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x20);  // address passed as an integer
        return;
    case 4:
        G.pEm->Routine_3++;
        G.pEm->Move_no++;
        G.pEm->Move_wait = 0;
        G.pEm->Timer0 = 0x78U;

    case 5:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        if (!(G.pEm->Move_cnt & 7))
        {
            ScratchPad->Vec.vx = 0x320;
            ScratchPad->Vec.vy = -0x12C;
            ScratchPad->Vec.vz = 0;

            FUNC(ESP_CALL)(0, 0, 0, G.pEm->M, &ScratchPad->Vec, 0);

            FUNC(SND_SE_ENEM)(3, 0);
        }
        if (G.pEm->Move_cnt == 0xE)
        {
            pPl_parts = G.Player.pSin_parts_ptr;
            FUNC(MODEL_COLOR_PLUS)(pPl_parts, 0x30, 0x80820, 0x606060);
            ScratchPad->Vec.vx = 0x64;
            ScratchPad->Vec.vy = -0x2BC;

            ScratchPad->Vec.vz = 0;

            FUNC(ESP_CALL)(0, 0, 0, &pPl_parts->Workm, &ScratchPad->Vec, 0);

            FUNC(MODEL_COLOR_PLUS)(&pPl_parts[2], 0x30, 0x80820, 0x606060);

            FUNC(ESP_CALL)(0, 0, 0, &pPl_parts[2].Workm, &ScratchPad->Vec, 0);
        }
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3++;
            G.pEm->Move_no++;
            G.pEm->Move_cnt = 0U;
            G.pEm->Move_wait = 0;
            G.pEm->Hokan_flg = 3;
        }
        return;
    case 6:
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
        }
    }
}

VOID Em00_Act_turn(VOID)
{
    extern struct
    {
        SHORT a;
        SHORT b;
        SHORT c;
    } zombie_attack_data_tbl_view[] ASM_NAME("zombie_attack_data_tbl");

    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0U;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 3;

        G.pEm->Spd_cur = 0x2D;

        FUNC(ADD_SPEED_XZ)(zombie_attack_data_tbl_view[EM00_FREE_P->Catch_dir].a);
        G.pEm->Timer0 = zombie_attack_data_tbl_view[EM00_FREE_P->Catch_dir].b;
    }
    if (G.pEm->Move_wait == 1)
    {
        if (G.pEm->Move_cnt == 8)
        {
            FUNC(SND_SE_ENEM)(1, 0);
        }
        if (G.pEm->Move_cnt == 0x1D)
        {
            FUNC(SND_SE_ENEM)(1, 0);
        }
    }
    G.pEm->Cdir_y -= zombie_attack_data_tbl_view[EM00_FREE_P->Catch_dir].c;
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
    if (G.pEm->Timer0-- == 0)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 4;
    }
    G.pEm->Pos_x += G.pEm->Spd_x;
    G.pEm->Pos_y += G.pEm->Spd_y;
    G.pEm->Pos_z += G.pEm->Spd_z;
}

VOID Em00_Push_stagger(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0U;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 0xE;

    case 1:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        if (G.pEm->Move_cnt == 5)
        {
            G.pEm->Routine_3 = 2U;
            G.pEm->Hokan_flg = 3;
            G.pEm->Spd_cur = 0x14;
            G.pEm->Timer0 = 1U;
        }
        break;
    case 2:
        if ((G.pEm->Move_cnt == 7 || G.pEm->Move_cnt == 0x18) && G.pEm->Move_wait == 1)
        {
            FUNC(SND_SE_ENEM)(2, 0);
        }
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        Em00_Leg_spd(1);

        if ((G.pEm->pSin_parts_ptr[4].Be_flg & 4) && G.pEm->Move_cnt >= 0x12)
        {
            return;
        }

        ScratchPad->Vec.vx = G.pEm->Dest_x;
        ScratchPad->Vec.vz = G.pEm->Dest_z;

        ScratchPad->Vec.vy = 0;
        EM00_FREE_P->angle_turn_delta = FUNC(GOTO00_CK)(&ScratchPad->Vec, 8);
        if (EM00_FREE_P->Dir_ctrl_flg & 0x80)
        {
            EM00_FREE_P->angle_turn_delta *= -1;
            G.pEm->Routine_3 = 3U;

            G.pEm->Timer0 = (G.SaveData.Sce_random & 0x1F) + 0x1E;
        }
        G.pEm->Cdir_y += EM00_FREE_P->angle_turn_delta;

        FUNC(ADD_SPEED_XZ)(0);
        break;
    case 3:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        Em00_Leg_spd(1);
        G.pEm->Cdir_y += EM00_FREE_P->angle_turn_delta;
        FUNC(ADD_SPEED_XZ)(0);
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 3U;
        }
    }
    if (G.pEm->Sca_info & 8)
    {
        G.pEm->Type++;
    }
}

VOID Em00_Act_vomit(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3++;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 5;
        ScratchPad->Vec.vx = 0x1F4;
        ScratchPad->Vec.vz = 0;
        ScratchPad->Vec.vy = -0x9C4;

        FUNC(ESP_CALL)(0x20, 0, 0, G.pEm->M, &ScratchPad->Vec, 0);

        FUNC(SND_SE_ENEM)(7, 0);

    case 1:

        G.pEm->Routine_3 += FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);

        return;
    case 2:

        G.pEm->Routine_3++;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0;

        G.pEm->Timer0 = (G.SaveData.Sce_random & 0x1F) + 0x14;

        G.pEm->Hokan_flg = 3;

    case 3:

        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        if (--G.pEm->Timer0 == 0)
        {
            G.pEm->Routine_1 = 0;

            EM_ROUTINE_HALF2 = 0;
        }
    }
}

VOID Em00_Move_eat(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3++;

        G.pEm->Move_cnt = G.SaveData.Sce_random & 0x1F;

        G.pEm->Move_wait = 0;

        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 0x1E;

        G.pEm->Timer0 = (G.SaveData.Sce_random & 0xF) + 0x2D;

    case 1:
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)
        {
            if (G.SaveData.Sce_random & 0x01000001)
            {
                G.pEm->Move_no = 0x1D;
            }
            else
            {
                G.pEm->Move_no = 0x1E;
            }
        }
        if (G.pEm->Move_cnt == 0x19)
        {
            ScratchPad->Vec.vx = 0x320;
            ScratchPad->Vec.vy = -0x12C;
            ScratchPad->Vec.vz = 0;

            FUNC(ESP_CALL)(0, 0, 0, G.pEm->M, &ScratchPad->Vec, 0);

            FUNC(SND_SE_ENEM)(3, 0);
        }
        ScratchWord[0] = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);
        if (ScratchWord[0] < 0xBB8)
        {
            G.pEm->Routine_3++;
        }
        break;
    case 2:
        G.pEm->Routine_3++;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0xD;

    case 3:
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)
        {
            G.pEm->Type = 0;

            if (STAGE_ROOM == ((5) << 8 | (4)))
            {
                G.pEm->Type = 4;
            }

            EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);

            G.pEm->Pos_y = 0;
        }
    }
}

VOID Em00_Act_fall(VOID)
{
    UCHAR Rsv_init = 222;    // unused local: reserves stack space
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0U;
        G.pEm->Move_no = 8;
        G.pEm->Hokan_flg = 7;

        G.pEm->Spd_cur = 0x14;
        FUNC(SND_SE_ENEM)(5, 0);

        G.pEm->Pos_y = 1;

    case 1:

        EM00_FREE_P->down_flags &= 0xFB;
        G.pEm->Damage_no = 1;
        EM00_FREE_P->hit_count |= ~0x7F;
        if (G.pEm->Move_cnt < 4U)
        {
            EM00_FREE_P->hit_count &= 0x7F;
            EM00_FREE_P->down_flags |= 4;
            G.pEm->Damage_no = 0;
        }

        if (G.pEm->Move_wait == 1)
        {
            if (G.pEm->Move_cnt == 8)
            {
                FUNC(SND_SE_ENEM)(1, 0);
            }
            if (G.pEm->Move_cnt == 0x1A)
            {
                FUNC(SND_SE_ENEM)(0, 0);
            }
        }
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)
        {
            G.pEm->Damage_no = 0;

            G.pEm->Timer0 = Em00_down_time_tbl[G.SaveData.Sce_random & 0xF] * 0x1E;

            G.pEm->Routine_3 = 2U;
        }
        FUNC(ADD_SPEED_XZ)(0);
        return;
    case 2:
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 3U;
            G.pEm->Hokan_flg = 0;
            G.pEm->Damage_no = 1;
            FUNC(SND_SE_ENEM)(5, 0);
        }
        return;
    case 3:

        EM00_FREE_P->down_flags &= 0xFB;
        G.pEm->Damage_no = 1;
        EM00_FREE_P->hit_count |= ~0x7F;
        if (G.pEm->Move_cnt >= 0x1BU)
        {
            EM00_FREE_P->hit_count &= 0x7F;
            EM00_FREE_P->down_flags |= 4;
            G.pEm->Damage_no = 0;
        }

        if (FUNC(JOINT_MOVE)(1, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)
        {
            if (EM00_FREE_P->Stagger_timer != 0)
            {
                EM00_FREE_P->hit_count = 0;
            }
            else
            {
                EM00_FREE_P->Stagger_timer = Em00_stagger_tbl[G.SaveData.Sce_random & 0x1F];

                EM00_FREE_P->hit_count &= 0x7F;
            }

            G.pEm->Damage_no = 0;
            EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);

            EM00_FREE_P->down_flags &= 0xFB;
            G.pEm->Pos_y = 0;
        }
        FUNC(ADD_SPEED_XZ)(0x800);
    }
}

VOID Em00_Die_walk(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        EM00_FREE_P->down_flags |= 1;

        G.pEm->Spd_cur = 0x2D;

        G.pEm->Routine_3++;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0U;
        G.pEm->Move_no = 3;
        G.pEm->Hokan_flg = 3;

    case 1:
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x20);  // address passed as an integer
        if (G.pEm->Move_wait == 1)
        {
            if (G.pEm->Move_cnt == 8)
            {
                FUNC(SND_SE_ENEM)(1, 0);
            }
            if (G.pEm->Move_cnt == 0x1D)
            {
                FUNC(SND_SE_ENEM)(1, 0);
            }
        }
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 0, 1, 3);
        }
    }
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em00_Move_lunge(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 0x1D;
        G.pEm->Timer0 = (G.SaveData.Sce_random & 0xF) + 0x2D;

    case 1:
        if (G.pEm->Move_cnt == 0x14)
        {
            FUNC(SND_SE_ENEM)(4, 0);
        }
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)
        {
            G.pEm->Spd_cur = 0x1E7;
            FUNC(ADD_SPEED_XZ)(0);
            G.pEm->Spd_cur = 0x3DF;
            FUNC(ADD_SPEED_XZ)(0x400);
            G.pEm->Cdir_y += 0x400;
            G.pEm->Type = 0;
            EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);
            G.pEm->pAt_tbl = Em00_sca_tbl[0];
            G.pEm->Be_flg &= ~EM_BE_NO_WALL;
        }
    }
}

VOID Em00_Dm_short(VOID)
{
    PARTS_W* pP_ptr;

    if (!(G.pEm->Type & 0x40))
    {
        G.pEm->Move_no = G.pEm->Routine_2 == 0 ? 4 : 5;
    }
    G.pEm->Spd_cur = 0xF;
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1U;
        G.pEm->Spd_cur = 0;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        if (G.pEm->Type & 0x40)
        {
            ScratchPad->Vec.vx = 0x64;
            ScratchPad->Vec.vy = -0xA3C;
            ScratchPad->Vec.vz = 0;

            FUNC(ESP_CALL)(0, 0, 0, G.pEm->M, &ScratchPad->Vec, 0);
        }
        pP_ptr = G.pEm->pSin_parts_ptr;

        if ((G.pEm->Damage_no & 7) == 3 && !(pP_ptr[4].Be_flg & 4) && ((0x40 >> (G.SaveData.Sce_random & 7)) & 1))
        {
            pP_ptr[4].Be_flg |= 0xC;
            ScratchPad->Vec.vz = 0;
            ScratchPad->Vec.vy = 0;
            ScratchPad->Vec.vx = 0;

            FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[4].Workm, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(0, 0, 0, 0, pP_ptr[4].Workm.t, 0);

            pP_ptr[5].Be_flg |= 0x10;
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[4], 0x30, 0x80820, 0x606060);
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[3], 0x30, 0x80820, 0x606060);
        }
        if (EM00_FREE_P->Push_timer == 0)
        {
            FUNC(SND_SE_ENEM)(9, 0);
            EM00_FREE_P->Push_timer = 0x96;
        }
    }
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)
    {
        G.pEm->Routine_3++;
        if (!(G.pEm->Type & 0x40))
        {
            if ((G.pEm->Type & 0xF) == 5)
            {
                G.pEm->Type = 0U;
            }
            EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);
        }
        G.pEm->Dest_x = G.Player.Pos_x;
        G.pEm->Dest_z = G.Player.Pos_z;
        G.pEm->Damage_no = 0U;
    }
    if (G.pEm->Damage_no & 1)
    {
        if ((++EM00_FREE_P->push_back_count & 0xFF) == 2)
        {
            EM00_FREE_P->Move_spd = 0x14;
            EM00_FREE_P->turn_speed = 0xE;
        }
    }
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em00_Dm_stagger(VOID)
{
    USHORT Push_dir;

    if (G.pEm->Routine_2 == 2)
    {
        G.pEm->Move_no = 7;
        Push_dir = 0x800;
    }
    else
    {
        G.pEm->Move_no = 6;
        Push_dir = 0;
    }
    G.pEm->Spd_cur = 0xFU;
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 6;
        if (EM00_FREE_P->Push_timer == 0)
        {
            FUNC(SND_SE_ENEM)(9, 0);
            EM00_FREE_P->Push_timer = 0x96;
        }
    }
    if (FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)
    {
        if ((G.pEm->Type & 0xF) == 5)
        {
            G.pEm->Type = 0U;
        }
        EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);
        G.pEm->Dest_x = G.Player.Pos_x;
        G.pEm->Dest_z = G.Player.Pos_z;
        G.pEm->Damage_no = 0;
    }
    else
    {
        if (G.pEm->Move_cnt < 5U)
        {
            G.pEm->Spd_cur += 0x5A;
        }
        if (G.pEm->Move_cnt == 0x14)
        {
            G.pEm->Damage_no = 0;
        }
    }
    FUNC(ADD_SPEED_XZ)(Push_dir);
}

VOID Em00_Dm_leg(VOID)
{
    PARTS_W* pP_ptr;
    SVECTOR Probe_neg = { -800, 0, 0 };
    SVECTOR Probe_pos = { 800, 0, 0 };
    VECTOR saved_pos;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Timer2 = 0x46U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0U;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 8;
        G.pEm->Spd_cur = 0x14;
        pP_ptr = G.pEm->pSin_parts_ptr;
        FUNC(JOINT_ATCK_SET)(&pP_ptr[10], 0x14, 5, 3);
        FUNC(JOINT_ATCK_SET)(&pP_ptr[11], 0x14, 5, 3);
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

        FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[9].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[10].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[13].Workm, &ScratchPad->Vec, 0);

        FUNC(SND_SE_ENEM)(6, 0);
        if (EM00_FREE_P->Push_timer == 0)
        {
            FUNC(SND_SE_ENEM)(9, 0);
            EM00_FREE_P->Push_timer = 0x96;
        }
        if (G.pEm->Life < 0)
        {
            G.pEm->Life = 1;
        }

        G.pEm->Pos_y = 1;

    case 1:
        G.pEm->Routine_3 += FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        if (G.pEm->Move_wait == 1)
        {
            if (G.pEm->Move_cnt == 8)
            {
                FUNC(SND_SE_ENEM)(1, 0);
            }
            if (G.pEm->Move_cnt == 0x1A)
            {
                FUNC(SND_SE_ENEM)(0, 0);
            }
        }
        break;
    case 2:
        saved_pos = EM_POS_VEC;

        G.pEm->Be_flg |= EM_BE_NO_HIT | EM_BE_NO_WALL | EM_BE_NO_OBJ;

        ScratchWord[0] = FUNC(SCA_CK_POINT2)(&Probe_neg, &Probe_pos);
        EM_POS_VEC = saved_pos;

        if (G.pEm->Type != 4 && STAGE_ROOM != ((2) << 8 | (1)) && ScratchWord[0] == 0 && G.pEm->Life > 0)
        {
            G.pEm->Type = (!(G.SaveData.Sce_random & 3)) ? 3 : 2;

            EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
            G.pEm->Damage_no = 0;
            G.pEm->Dest_x = G.Player.Pos_x;
            G.pEm->Dest_z = G.Player.Pos_z;

            G.pEm->Be_flg &= 0xF1;

            return;
        }
        G.pEm->Life = -1;

        FUNC(KAGE_WORK_COLOR_SET)(&G.pEm->Kage_work, 1, 2, 0xFFFF50);

        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, -0x64, -0x64);
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);
        G.pEm->Routine_3 = 3U;

        G.pEm->Be_flg |= EM_BE_NO_HIT | EM_BE_NO_WALL | EM_BE_NO_OBJ;

        G.pEm->Spd_cur = 0;
    case 3:
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, 6, 6);
        if (--G.pEm->Timer2 == 0)
        {
            G.pEm->Routine_3 = 4U;
        }
        break;
    case 4:;
    }
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em00_Dm_drop(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0xF;
        if (EM00_FREE_P->Push_timer == 0)
        {
            FUNC(SND_SE_ENEM)(9, 0);
            EM00_FREE_P->Push_timer = 0x96;
        }
    }
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 1, 0, 1);
        G.pEm->Dest_x = G.Player.Pos_x;
        G.pEm->Dest_z = G.Player.Pos_z;
        G.pEm->Damage_no = 0;

        if (!(G.pEm->Sca_info & 8) && !(G.pEm->Type & 0x20) && (G.pEm->Type & 0xF) == 3)
        {
            G.pEm->Type = 2U;
        }
    }
}

VOID Em00_Dm_long(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0xCU;
        G.pEm->Move_wait = 1;
        G.pEm->Damage_no = 1;
        G.pEm->Move_no = 7;
        G.pEm->Spd_cur = 0x1E;

    case 1:
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)
        {
            G.pEm->Routine_3 = 2U;

            G.pEm->Timer0 = (G.SaveData.Sce_random + 0x10) & 7;

            G.pEm->Spd_cur = 0xF;
        }
        else if (G.pEm->Move_cnt == 0x14)
        {
            G.pEm->Damage_no = 0;
        }
        break;
    case 2:
        if (G.pEm->Timer0-- == 0)
        {
            EM_ROUTINE_WORD = ROUTINES(2, 2, 0, 1);
            G.pEm->Dest_x = G.Player.Pos_x;
            G.pEm->Dest_z = G.Player.Pos_z;
            G.pEm->Damage_no = 0;
        }
    }
    FUNC(ADD_SPEED_XZ)(0x800);
}

VOID Em00_Dm_standup(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3++;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0xD;

    case 1:
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);
            G.pEm->Type = 0;
            if (STAGE_ROOM == ((5) << 8 | (4)))
            {
                G.pEm->Type = 4;
            }
            G.pEm->Damage_no = 0;
        }
    }
}

VOID Em00_Die_mot(VOID)
{
    SVECTOR Probe_neg = { -800, 0, 0 };
    SVECTOR Probe_pos = { 800, 0, 0 };
    VECTOR saved_pos;

    USHORT Dir_bak;

    PARTS_W* pP_ptr;

    Dir_bak = 0;

    pP_ptr = G.pEm->pSin_parts_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Timer2 = 0x46U;
        G.pEm->Hokan_flg = 3;
        G.pEm->Damage_no = 1;
        if (!(EM00_FREE_P->down_flags & 1) && !(pP_ptr[2].Be_flg & 0xCC))
        {
            FUNC(SND_SE_ENEM)(5, 0);
        }

    case 1:
        G.pEm->Routine_3 += FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        return;
    case 2:

        Dir_bak = G.pEm->Cdir_y;

        saved_pos = EM_POS_VEC;
        ScratchWord[0] = FUNC(SCA_CK_POINT2)(&Probe_neg, &Probe_pos);
        EM_POS_VEC = saved_pos;

        G.pEm->Cdir_y = Dir_bak;

        pP_ptr = G.pEm->pSin_parts_ptr;

        if (!(G.pEm->Type & 0x40) && G.pEm->Type != 4 && !(pP_ptr[2].Be_flg & 0xCC) && STAGE_ROOM != ((2) << 8 | (1)) && G.pEm->Move_no == 8 && ScratchWord[0] == 0 && !(G.SaveData.Sce_random & 3) && !(G.System_flg & SYS_DIM))
        {
            G.pEm->Type = (!(G.SaveData.Sce_random & 3)) ? 3 : 2;

            G.pEm->Life = 1;
            EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
            G.pEm->Damage_no = 0;

            G.pEm->Pos_y = 1;
            EM00_FREE_P->down_flags &= 0xFB;
            G.pEm->Be_flg &= EM_BE_STATE_MASK;

            return;
        }
        G.pEm->Life = -1;

        FUNC(KAGE_WORK_COLOR_SET)(&G.pEm->Kage_work, 1, 2, 0xFFFF50);

        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, -0x64, -0x64);
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);
        G.pEm->Routine_3 = 3U;

        G.pEm->Be_flg |= EM_BE_NO_HIT | EM_BE_NO_WALL | EM_BE_NO_OBJ;

    case 3:
        G.pEm->Spd_cur = 0;
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, 6, 6);
        if (--G.pEm->Timer2 == 0)
        {
            G.pEm->Routine_3 = 4U;
        }
        return;
    case 4:
        G.pEm->Spd_cur = 0;
        if (G.pEm->Type & 0x40)
        {
            FUNC(FLG_ON)(&MAIN.Room_flg[0], G.pEm->Set_flg_no);
            EM_ROUTINE_HALF2 = 0;
        }
    }
}

VOID Em00_Pl_catch(VOID)
{
    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Routine_3 = 1;
        G.Player.Hokan_flg = 0;
        G.Player.Pos_x = G.pEm->Root_x;
        G.Player.Pos_z = G.pEm->Root_z;
        FUNC(SND_SE_STAD)(3, 0, 0, &MAIN.Player.Pos_x);  // address passed as an integer
        G.Player.Be_flg |= EM_BE_NO_HIT;

    case 1:
        FUNC(NULL_POS_SET)(&MAIN.Player, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr);  // address passed as an integer
        if (FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400) != 0)
        {
            G.Player.Move_no++;
            G.Player.Routine_3 = 2;
            G.Player.Move_wait = 0;
        }
        return;
    case 2:
        FUNC(NULL_POS_SET)(&MAIN.Player, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr);  // address passed as an integer
        FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
        return;
    case 3:
        G.Player.Move_no++;
        G.Player.Routine_3 = 4;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;

    case 4:
        FUNC(NULL_POS_SET)(&MAIN.Player, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr);  // address passed as an integer
        if (FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400) != 0)
        {
            PL_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
            G.Player.Be_flg &= ~EM_BE_NO_HIT;
            if ((UCHAR)G.Player.Move_no != 2 && (UCHAR)G.Player.Move_no != 8)  // truncates to a byte
            {
            }
            else
            {
                G.Player.Cdir_y += 0x800;
            }
            G.Player.Damage_no = 0;
            if (G.Player.Turn_timer == 0)
            {
                G.Player.Turn_timer = 0x96;
            }
        }

    default: LABEL_END
    }
}

VOID Em00_Leg_spd(UCHAR Flg)
{
    PARTS_W* pP_ptr;
    UCHAR n;
    pP_ptr = G.pEm->pSin_parts_ptr;
    FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);
    FUNC(GS_MUL_COORD0)(G.pEm->M, &pP_ptr->Coord, &ScratchPad->Mat);
    FUNC(GS_MUL_COORD3)(&ScratchPad->Mat, &pP_ptr[1].Coord);
    pP_ptr = &pP_ptr[Flg * 3 + 5];
    n = 3;
    while (n != 0)
    {
        FUNC(GS_MUL_COORD3)(&ScratchPad->Mat, &(pP_ptr - n)[1].Coord);
        n--;
    }
    ScratchPad->Mat.t[0] -= pP_ptr->Workm.t[0];
    ScratchPad->Mat.t[1] = 0;
    ScratchPad->Mat.t[2] -= pP_ptr->Workm.t[2];
    FUNC(SQUARE0)(&ScratchPad->Mat.t, &ScratchPad->Vec);
    G.pEm->Spd_cur = FUNC(SQUARE_ROOT0)(ScratchPad->Vec.vx + ScratchPad->Vec.vz);
}

VOID Em00_Head_bound(PARTS_W* pP_ptr, SHORT gravity)
{
    SVECTOR vel;
    VECTOR Old_pos;
    VOID* pMirror;

    if (G.System_flg & SYS_MIRROR_MATRIX)
    {
        pMirror = &G.pEm->pM_char_ptr[2];
    }
    if (pP_ptr->Workm.t[1] >= -0x64 && (pP_ptr->Fall_sts & 0x1F) >= 6U)
    {
        pP_ptr->Be_flg &= 0xFC;
        return;
    }
    vel.vy = (6 - (pP_ptr->Fall_sts & 0x1F)) * 0x10;
    vel.vx = (6 - (pP_ptr->Fall_sts & 0x1F)) * 8;
    vel.vz = 0x40;
    FUNC(ROT_MATRIX)(&vel, &ScratchPad->Mat);
    FUNC(MUL_MATRIX)(&pP_ptr->Workm, &ScratchPad->Mat);
    ScratchPad->Mat = *Monitor.pId_mat;
    FUNC(ROT_MATRIX_Y)(G.pEm->Cdir_y, &ScratchPad->Mat);
    FUNC(APPLY_MATRIX_SV)(&ScratchPad->Mat, &pP_ptr->Cdir, &vel);
    Old_pos = MATRIX_POS(&pP_ptr->Workm);
    pP_ptr->Workm.t[0] = pP_ptr->Workm.t[0] + (vel.vx - vel.vx * KEEP_VALUE(((pP_ptr->Fall_sts & 0x40) >> 6) * 2));  // KEEP_VALUE keeps the operand a separate value; the plain form and a cached local change the code
    pP_ptr->Workm.t[2] = pP_ptr->Workm.t[2] + (vel.vz - vel.vz * KEEP_VALUE(((pP_ptr->Fall_sts & 0x80) >> 7) * 2));  // KEEP_VALUE keeps the operand a separate value; the plain form and a cached local change the code
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vx = 0;
    if (FUNC(SCA_CK_POS)(pP_ptr->Workm.t, &ScratchPad->Sv) != 0)
    {
        pP_ptr->Workm.t[0] = Old_pos.vx;
        pP_ptr->Workm.t[2] = Old_pos.vz;
        pP_ptr->Fall_sts ^= 0x40;
        pP_ptr->Workm.t[0] = pP_ptr->Workm.t[0] + (vel.vx - vel.vx * KEEP_VALUE(((pP_ptr->Fall_sts & 0x40) >> 6) * 2));  // KEEP_VALUE keeps the operand a separate value; the plain form and a cached local change the code
        pP_ptr->Workm.t[2] = pP_ptr->Workm.t[2] + (vel.vz - vel.vz * KEEP_VALUE(((pP_ptr->Fall_sts & 0x80) >> 7) * 2));  // KEEP_VALUE keeps the operand a separate value; the plain form and a cached local change the code
        if (FUNC(SCA_CK_POS)(pP_ptr->Workm.t, &ScratchPad->Sv) != 0)
        {
            pP_ptr->Fall_sts ^= 0xC0;
        }
        pP_ptr->Workm.t[0] = Old_pos.vx;
        pP_ptr->Workm.t[2] = Old_pos.vz;
        pP_ptr->Cdir.vx >>= 1;
        ScratchPad->Vec.vz = 0;
        ScratchPad->Vec.vy = 0;
        ScratchPad->Vec.vx = 0;

        FUNC(ESP_CALL)(0, 0, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);
    }
    pP_ptr->Cdir.vy -= gravity * pP_ptr->Fall_cnt;
    pP_ptr->Workm.t[1] -= pP_ptr->Cdir.vy;
    if (pP_ptr->Workm.t[1] >= -0x64)
    {
        pP_ptr->Workm.t[1] = -0x64;
        pP_ptr->Fall_sts++;
        pP_ptr->Cdir.vy *= -1;
        pP_ptr->Cdir.vy >>= 2;
        pP_ptr->Fall_cnt = 0U;
        pP_ptr->Cdir.vx += 0x28;
        ScratchPad->Vec.vz = 0;
        ScratchPad->Vec.vy = 0;
        ScratchPad->Vec.vx = 0;

        FUNC(ESP_CALL)(0, 0, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);

        FUNC(SND_SE_ENEM)(8, 0);
    }
    if (pP_ptr->Cdir.vx > 0)
    {
        pP_ptr->Cdir.vx = 0;
    }
    pP_ptr->Fall_cnt++;
}

UCHAR Em00_Catch_pad(VOID)
{
    UCHAR power;

    power = 0;
    if (G.Padd1_trg & (PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT))
    {
        power = 3;
    }
    if (G.Padd1_trg & (PAD_TRIANGLE | PAD_CIRCLE | PAD_X | PAD_SQUARE))
    {
        power += 2;
    }
    return power;
}

VOID Em00_Scenario(VOID)
{
    Em00_sce_act_tbl[G.pEm->Routine_2]();
}

VOID Em00_Sce_nop(VOID)
{
}

VOID Em00_Sce_walk(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3++;
        G.pEm->Spd_cur = 0;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 2;
        G.pEm->Hokan_flg = 3;
        FUNC(SND_SE_ENEM)(4, 0U);

    case 1:
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400U) != 0)
        {
            G.pEm->Routine_3++;
            G.pEm->Spd_cur = 0x23;
            G.pEm->Move_cnt = 0U;
            G.pEm->Move_wait = 0;
            G.pEm->Move_no = 3;
            G.pEm->Hokan_flg = 3;
        }
        break;
    case 2:
        ScratchPad->Vec.vx = G.pEm->Root_x;
        ScratchPad->Vec.vz = G.pEm->Root_z;
        ScratchPad->Vec.vy = 0;
        FUNC(GOTO00)(&ScratchPad->Vec, 0x40U);
        if (G.pEm->Move_cnt == 8)
        {
            FUNC(SND_SE_ENEM)(1, 0U);
        }
        if (G.pEm->Move_cnt == 0x1D)
        {
            FUNC(SND_SE_ENEM)(1, 0U);
        }
        FUNC(JOINT_MOVE)(G.pEm->Sce_flg & 1, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400U);
        if (FUNC(SQUARE_ROOT0)((G.pEm->Pos_x - G.pEm->Root_x) * (G.pEm->Pos_x - G.pEm->Root_x) + (G.pEm->Pos_z - G.pEm->Root_z) * (G.pEm->Pos_z - G.pEm->Root_z)) < 0x96U)
        {
            G.pEm->Routine_3++;
        }
        break;
    case 3:
        FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);  // address passed as an integer
        if (G.pEm->Sca_info & 0x80)
        {
            G.pEm->Routine_3 = 2U;
        }
        else
        {
            EM_ROUTINE_HALF2 = 0;
            break;
        }
    }
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em00_Sce_headshot(VOID)
{
    PARTS_W* pP_ptr;

    G.pEm->Move_no = 8;
    G.pEm->Spd_cur = 0x14;
    if (G.pEm->Routine_3 == 0)
    {
        ScratchPad->Vec.vx = 0x64;
        ScratchPad->Vec.vy = -0xA3C;
        ScratchPad->Vec.vz = 0;
        pP_ptr = G.pEm->pSin_parts_ptr;
        FUNC(JOINT_ATCK_SET)(&pP_ptr[2], 0x1E, 2, 3);

        FUNC(ESP_CALL)(3, 0, (SHORT)G.Player.Cdir_y - G.pEm->Cdir_y + 0x800, G.pEm->M, &ScratchPad->Vec, 0);  // sign-extends the halfword

        ScratchPad->Vec.vx = 0;
        ScratchPad->Vec.vy = -0x258;
        ScratchPad->Vec.vz = 0;

        FUNC(ESP_CALL)(0, 3, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(4, 0, (SHORT)G.Player.Cdir_y - G.pEm->Cdir_y + 0x600, G.pEm->M, &ScratchPad->Vec, 0);  // sign-extends the halfword
        FUNC(ESP_CALL)(4, 2, (SHORT)G.Player.Cdir_y - G.pEm->Cdir_y + 0x700, G.pEm->M, &ScratchPad->Vec, 0);  // sign-extends the halfword
        FUNC(ESP_CALL)(4, 3, (SHORT)G.Player.Cdir_y - G.pEm->Cdir_y + 0x900, G.pEm->M, &ScratchPad->Vec, 0);  // sign-extends the halfword

        FUNC(SND_SE_ENEM)(6, 0);
    }
    if (G.pEm->Move_cnt == 8)
    {
        FUNC(SND_SE_ENEM)(1, 0);
    }
    if (G.pEm->Move_cnt == 0x1A)
    {
        FUNC(SND_SE_ENEM)(0, 0);
    }
    Em00_Die_mot();
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em00_Sce_dying(VOID)
{
    G.pEm->Move_no = 0xA;
    G.pEm->Spd_cur = 0x14U;
    if (G.pEm->Move_cnt == 0x12)
    {
        FUNC(SND_SE_ENEM)(2, 0);
    }
    if (G.pEm->Move_cnt < 5U)
    {
        G.pEm->Spd_cur += 0x5A;
    }
    Em00_Die_mot();
    FUNC(ADD_SPEED_XZ)(0x800);
}

VOID Em00_Sce_vomit(VOID)
{
    G.pEm->Move_no = 4;
    Em00_Dm_short();
    if (G.pEm->Routine_3 & 2)
    {
        FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);  // address passed as an integer
        EM_ROUTINE_HALF2 = 0;
    }
}

VOID Em00_Sce_vomit2(VOID)
{
    G.pEm->Move_no = 5;
    Em00_Dm_short();
    if (G.pEm->Routine_3 & 2)
    {
        FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);  // address passed as an integer
        EM_ROUTINE_HALF2 = 0;
    }
}

AT_TBL Em00_sca00 = { 0x8001, 0, -1530, 0, 0x5FA, 0x1A6 };  // collision volume 0: default
AT_TBL Em00_sca01 = { 0x8001, 0, -1530, 0, 0x5FA, 0x142 };  // collision volume 1: entity id 1
AT_TBL Em00_sca02 = { 0x8001, 200, -1530, 0, 0x5FA, 0x3E8 };  // collision volume 2: behavior 0xB

// collision volumes by index
AT_TBL* Em00_sca_tbl[3] =
{
    &Em00_sca00,   // 0 //
    &Em00_sca01,   // 1 //
    &Em00_sca02,   // 2 //
};

// base health, indexed by a random nibble
UCHAR Em00_life[16] =
{
    0x3B,   // 0 //
    0x3B,   // 1 //
    0x4F,   // 2 //
    0x3B,   // 3 //
    0x3B,   // 4 //
    0x27,   // 5 //
    0x3B,   // 6 //
    0x4F,   // 7 //
    0x4F,   // 8 //
    0x63,   // 9 //
    0x3B,   // 10 //
    0x4F,   // 11 //
    0x3B,   // 12 //
    0x3B,   // 13 //
    0x4F,   // 14 //
    0x3B,   // 15 //
};

// start animation id by behavior (Type & 0xF)
UCHAR Em00_move_no_tbl[12] =
{
    0x00,   // 0 //
    0x00,   // 1 //
    0x09,   // 2 //
    0x09,   // 3 //
    0x00,   // 4 //
    0x0C,   // 5 //
    0x09,   // 6 //
    0x1D,   // 7 //
    0x00,   // 8 //
    0x00,   // 9 //
    0x09,   // 10 //
    0x1D,   // 11 //
};

// initial stagger timer, indexed by a random value 0 to 31
UCHAR Em00_stagger_tbl[32] =
{
    0x04,   // 0 //
    0x03,   // 1 //
    0x05,   // 2 //
    0x03,   // 3 //
    0x04,   // 4 //
    0x04,   // 5 //
    0x03,   // 6 //
    0x04,   // 7 //
    0x03,   // 8 //
    0x05,   // 9 //
    0x04,   // 10 //
    0x04,   // 11 //
    0x05,   // 12 //
    0x03,   // 13 //
    0x04,   // 14 //
    0x05,   // 15 //
    0x04,   // 16 //
    0x03,   // 17 //
    0x03,   // 18 //
    0x04,   // 19 //
    0x04,   // 20 //
    0x03,   // 21 //
    0x04,   // 22 //
    0x04,   // 23 //
    0x05,   // 24 //
    0x03,   // 25 //
    0x05,   // 26 //
    0x03,   // 27 //
    0x04,   // 28 //
    0x03,   // 29 //
    0x03,   // 30 //
    0x04,   // 31 //
};

// state handlers by entity state
VOID (*Em00_routine_0[9])(VOID) =
{
    Em00_Init,                  // 0 //
    Em00_Move,           // 1 //
    Em00_Damage,               // 2 //
    Em00_Die,                   // 3 //
    Em00_Dead,            // 4 //
    Em00_Atck,                // 5 //
    0,                            // 6 //
    0,                            // 7 //
    Em00_Scenario,   // 8 //
};

// behavior handlers by Type & 0xF
VOID (*Em00_Move_type[12])(VOID) =
{
    Em00_Move_chase,   // 0 //
    Em00_Move_chase,   // 1 //
    Em00_Move_push,    // 2 //
    Em00_Move_push,    // 3 //
    Em00_Move_rand,   // 4 //
    Em00_Move_eat,         // 5 //
    Em00_Move_chase,   // 6 //
    Em00_Move_eat,         // 7 //
    Em00_Move_chase,   // 8 //
    0,                     // 9 //
    Em00_Move_push,    // 10 //
    Em00_Move_lunge,      // 11 //
};

// hit reaction by behavior flag 2 and hit type (Damage_no & 7) - 1
UCHAR Em00_dm_sel_tbl[2][6] =
{
    { 0x00, 0x05, 0x00, 0x02, 0x00, 0x00 },   // 0 //
    { 0x04, 0x04, 0x00, 0x00, 0x00, 0x00 },   // 1 //
};

// hit reaction routines by reaction number
VOID (*Em00_dm_rtn_tbl[8])(VOID) =
{
    Em00_Dm_short,        // 0 //
    Em00_Dm_short,        // 1 //
    Em00_Dm_stagger,       // 2 //
    Em00_Dm_stagger,       // 3 //
    Em00_Dm_drop,          // 4 //
    Em00_Dm_leg,   // 5 //
    Em00_Dm_long,         // 6 //
    Em00_Dm_standup,   // 7 //
};

// action routines by Routine_2
VOID (*Em00_act_tbl[8])(VOID) =
{
    Em00_Act_idle,           // 0 //
    Em00_Act_slow,      // 1 //
    Em00_Act_chase,     // 2 //
    Em00_Act_face,    // 3 //
    Em00_Act_eat,      // 4 //
    Em00_Act_turn,   // 5 //
    Em00_Act_vomit,       // 6 //
    Em00_Act_fall,       // 7 //
};

// movement decisions by Type & 0xF
VOID (*Em00_mv_tbl[11])(VOID) =
{
    Em00_Mv_walk1,                   // 0 //
    Em00_Pl_dist_ck,   // 1 //
    Em00_Mv_slow,           // 2 //
    Em00_Mv_idle,                  // 3 //
    Em00_Mv_walk2,                   // 4 //
    Em00_Pl_dist_ck,   // 5 //
    Em00_Pl_dist_ck,   // 6 //
    Em00_Pl_dist_ck,   // 7 //
    Em00_Pl_dist_ck,   // 8 //
    0,                              // 9 //
    Em00_Mv_idle,                  // 10 //
};

UINT Em00_attack_anim_tbl[3] = { 0x14000011, 0x06170003, 0x0E091A06 };  // per grab direction: zombie animation, player animation, effect frame

// per grab direction: initial Timer0 of the player
USHORT Em00_atck_ofs_tbl[4] =
{
    0x7FFF,   // 0 //
    0x0000,   // 1 //
    0x7FFF,   // 2 //
    0x0000,   // 3 //
};

// bite reactions by grab direction
VOID (*Em00_pl_blood_tbl[4])(PARTS_W*) =
{
    Em00_Pl_blood0,   // 0 //
    Em00_Pl_blood1,   // 1 //
    Em00_Pl_blood2,   // 2 //
    Em00_Pl_blood3,   // 3 //
};

// Damage per bite by behavior (Type & 0xF)
UCHAR Em00_atck_dm_tbl[12] =
{
    0x0C,   // 0 //
    0x0C,   // 1 //
    0x09,   // 2 //
    0x09,   // 3 //
    0x0C,   // 4 //
    0x0C,   // 5 //
    0x0C,   // 6 //
    0x0C,   // 7 //
    0x0C,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
};

// per grab direction: angle offset of the step, tick count and angle step of the line-up turn (3 shorts each)
UINT zombie_attack_data_tbl[6] =
{
    0x00200018,   // 0 //
    0x00180020,   // 1 //
    0x000C0055,   // 2 //
    0x00290CC8,   // 3 //
    0x0000FFE0,   // 4 //
    0x090C0A0A,   // 5 //
};

// lying time in units of 0x1E ticks, indexed by a random nibble
UCHAR Em00_down_time_tbl[16] =
{
    2,   // 0 //
    4,   // 1 //
    4,   // 2 //
    4,   // 3 //
    4,   // 4 //
    4,   // 5 //
    4,   // 6 //
    6,   // 7 //
    6,   // 8 //
    6,   // 9 //
    6,   // 10 //
    6,   // 11 //
    6,   // 12 //
    9,   // 13 //
    9,   // 14 //
    9,   // 15 //
};

// scripted actions by Routine_2
VOID (*Em00_sce_act_tbl[20])(VOID) =
{
    Em00_Sce_nop,      // 0 //
    0,                          // 1 //
    Em00_Sce_walk,   // 2 //
    0,                          // 3 //
    0,                          // 4 //
    0,                          // 5 //
    0,                          // 6 //
    0,                          // 7 //
    0,                          // 8 //
    0,                          // 9 //
    Em00_Sce_headshot,            // 10 //
    Em00_Sce_dying,           // 11 //
    Em00_Sce_vomit,        // 12 //
    Em00_Sce_vomit2,       // 13 //
    0,                          // 14 //
    0,                          // 15 //
    0,                          // 16 //
    0,                          // 17 //
    0,                          // 18 //
    0,                          // 19 //
};
