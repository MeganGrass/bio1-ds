/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM0B.C
 */

#include "EM0B.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Em0b_Main(VOID)
{
    PARTS_W* pP_ptr;
    VECTOR probe;

    if (G.pEm->Routine_0 != 0 && (G.pEm->Type & 0x80))
    {
        return;
    }
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        Em0b_routine_0[G.pEm->Routine_0]();
        FUNC(OBA_SET_OFS)(G.pEm);
        FUNC(OBA_CK_HIT)(&MAIN.Player.Be_flg, G.pEm);
        FUNC(OBA_CK_EM)();
        probe = MATRIX_POS(Monitor.pId_mat);
        probe.vx = 0x320;
        FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, &ScratchPad->Mat);
        FUNC(APPLY_MATRIX_LV)(&ScratchPad->Mat, &probe, &probe);
        G.pEm->Pos_x = G.pEm->Pos_x + probe.vx;
        G.pEm->Pos_z = G.pEm->Pos_z + probe.vz;
        EM0B_FREE_P->Dir_ctrl_flg |= FUNC(SCA_CK_HIT)(&G.pEm->Pos_x, 0x258);  // reinterprets the memory as ENEMY_WORK
        EM0B_FREE_P->Wander_dir = ScratchWord[6];
        G.pEm->Pos_x = G.pEm->Pos_x - probe.vx;
        G.pEm->Pos_z = G.pEm->Pos_z - probe.vz;
    }
    G.pEm->Zone_in_flg = FUNC(HIT_CK_POINT4)(&G.pEm->Pos_x, G.Cccut);
    pP_ptr = G.pEm->pSin_parts_ptr;
    if (G.pEm->Zone_in_flg != 0)
    {
        FUNC(MUL_KAGE)(&G.pEm->Pos_x, &G.pEm->Kage_work, 0, G.pEm->Cdir_y);  // reinterprets the memory as ENEMY_WORK
    }
}

VOID Em0b_Init(VOID)
{
    EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
    G.pEm->Coord_flg = 0;
    G.pEm->Dummy0c1 = 0;
    G.pEm->Timer0 = 0xF;
    G.pEm->Damage_no = 0;
    FUNC(INIT_PARTS_WORK)();
    ScratchWord[7] = 0x404040;
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vx = 0;
    if (G.pEm->Type & 2)
    {
        FUNC(KAGE_WORK_INIT)(&ScratchPad->Sv, &G.pEm->Kage_work, 0x3E8, 0x96);
        G.pEm->Scale = 0x800;
        G.pEm->pAt_tbl = Em0b_sca_small;  // reinterprets the memory as AT_TBL
    }
    else
    {
        FUNC(KAGE_WORK_INIT)(&ScratchPad->Sv, &G.pEm->Kage_work, 0x7D0, 0x12C);
        G.pEm->pAt_tbl = Em0b_sca_big;  // reinterprets the memory as AT_TBL
    }
    Api_rand();
    G.pEm->Life = 0x28 + (Api_rand() & 7) + (Api_rand() & 7) + (Api_rand() & 7);
    G.pEm->Move_no = 0;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0x3F;
    if (G.pEm->Type & 1)
    {
        G.pEm->Move_no = 2;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0;
        EM_ROUTINE_WORD = ROUTINES(0, 1, 1, 1);
    }
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
    EM0B_FREE_P->effect_timer = 7;
    EM0B_FREE_P->Unk_173 = 5;
    EM0B_FREE_P->Dir_ctrl_flg = 0;
    EM0B_FREE_P->Wander_rno = 0;
    ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
    ScratchPad->Vec.vx = 0x3E8;
    FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, &ScratchPad->Mat);
    FUNC(APPLY_MATRIX_LV)(&ScratchPad->Mat, &ScratchPad->Vec, &ScratchPad->Vec);
    G.pEm->Old_x = G.pEm->Old_x + ScratchPad->Vec.vx;
    G.pEm->Old_z = G.pEm->Old_z + ScratchPad->Vec.vz;
    if (G.pEm->Type == 2)
    {
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
    }
}

VOID Em0b_Move(VOID)
{
    FUNC(ROOT_CK)(G.Player.Pos_x, G.Player.Pos_z, &G.pEm->Dest_x, &G.pEm->Dest_z);
    ScratchWord[0] = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;

    if (!(G.pEm->Type & 1) && ScratchWord[0] < 0x2710)
    {
        G.pEm->Be_flg = G.pEm->Be_flg | 0x40;
    }
    FUNC(EM_CK_HANI)(0xFA0);
    if (G.pEm->Routine_1 == 0)
    {
        Em0b_Atck_set();
    }
    Em0b_Move_call();
}

VOID Em0b_Atck_set(VOID)
{
    ScratchPad->Vec.vx = G.pEm->Dest_x;
    ScratchPad->Vec.vz = G.pEm->Dest_z;
    FUNC(EM_WANDER_TURN)(EM0B_FREE_P->Wander_dir, &EM0B_FREE_P->Dir_ctrl_flg, &EM0B_FREE_P->Wander_rno, 0x20, 0x3C);
    if (G.Player.Damage_no == 0 && !(G.pEm->Type & 2) && FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x300) == 0)
    {
        if (G.Player.Life < 0x1A)
        {
            if (ScratchWord[0] < 0xDAC)
            {
                G.pEm->Routine_1 = 1;
                EM_ROUTINE_HALF2 = 4;
            }
            return;
        }
        if (ScratchWord[0] < 0xBB8)
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_HALF2 = 5;
        }
    }
}

VOID Em0b_Move_call(VOID)
{
    Em0b_Move_type[G.pEm->Routine_2]();
}

VOID Em0b_Move_type00(VOID)
{
    PARTS_W* pP_ptr;

    Em0b_swim_tbl[G.pEm->Routine_3]();
    EM0B_FREE_P->Swim_ctr = EM0B_FREE_P->Swim_ctr + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
    if (EM0B_FREE_P->Swim_ctr == 3)
    {
        FUNC(SND_SE_ENEM)(0, 0);
        EM0B_FREE_P->Swim_ctr = 0;
    }
    FUNC(ADD_SPEED_XZ)(0);
    if (!(G.pEm->Type & 2))
    {
        if (--EM0B_FREE_P->effect_timer == 0)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchPad->Vec.vx = 0x1F4;
            ScratchPad->Vec.vy = 0;
            ScratchPad->Vec.vz = 0;

            FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr[6].Workm, &ScratchPad->Vec, 0);

            if (!(Api_rand() & 1))
            {
                ScratchPad->Vec.vx = 0x190;

                FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);
            }
            ScratchPad->Vec.vx = -0x64;
            ScratchPad->Vec.vy = -0x190;
            ScratchPad->Vec.vz = 0;

            FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr[12].Workm, &ScratchPad->Vec, 0);

            if (!(Api_rand() & 3))
            {
                ScratchPad->Vec.vy = 0x1F4;

                FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr[3].Workm, &ScratchPad->Vec, 0);
            }
            EM0B_FREE_P->effect_timer = 4;
        }
    }
}

VOID Em0b_Swim_begin(VOID)
{
    G.pEm->Move_no = 0;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0x3F;
    G.pEm->Routine_3++;

    G.pEm->Spd_cur = 0x78;

    EM0B_FREE_P->Swim_ctr = 3;
}

VOID Em0b_Swim_far(VOID)
{
    if (ScratchWord[0] >= 0x2711 && G.Player.Routine_2 == 0xD)
    {
        G.pEm->Routine_3++;
        G.pEm->Spd_cur = 0x82;
    }
}

VOID Em0b_Swim_near(VOID)
{
    if (ScratchWord[0] >= 0x1770 && G.Player.Routine_2 == 0xD)
    {
        return;
    }
    G.pEm->Routine_3--;
    G.pEm->Spd_cur = 0x64;
}

VOID Em0b_Move_type01(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3++;
        G.pEm->Spd_cur = 0x32;
        G.pEm->Move_no = (FUNC(RND)() & 1) + 2;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;
        FUNC(SND_SE_ENEM)(4U, 0);

    case 1:
        if (G.pEm->Move_no == 2 && G.pEm->Move_cnt == 0x19)
        {
            FUNC(SND_SE_ENEM)(5U, 0);
        }
        if (FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40) != 0)
        {
            G.pEm->Routine_3--;
        }
        FUNC(ADD_SPEED_XZ)(Api_rand() & 0xFFF);
        G.pEm->Cdir_y = G.pEm->Cdir_y - 0x400;
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 4);  // address passed as an integer
        G.pEm->Cdir_y = G.pEm->Cdir_y + 0x400;
    }
}

VOID Em0b_Move_type02(VOID)
{
    Em0b_retreat_tbl[G.pEm->Routine_3]();
}

VOID Em0b_Retreat_begin(VOID)
{
    G.pEm->Routine_3++;
    G.pEm->Spd_cur = 0x96;
    G.pEm->Move_no = 6;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0x3F;
}

VOID Em0b_Retreat_run(VOID)
{
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
    if (G.pEm->Move_cnt == 0x17)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
    }
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em0b_Move_type03(VOID)
{
    Em0b_circle_tbl[G.pEm->Routine_3]();
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em0b_Circle_begin(VOID)
{
    G.pEm->Move_no = 0;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0x3F;
    G.pEm->Routine_3++;
    G.pEm->Spd_cur = 0x64;
    G.pEm->Timer0 = (Api_rand() & 0x1F) + 0x5A;
    EM0B_FREE_P->Unk_175 = 0x20;
}

VOID Em0b_Circle_left(VOID)
{
    FUNC(EM_WANDER_TURN)(EM0B_FREE_P->Wander_dir, &EM0B_FREE_P->Dir_ctrl_flg, &EM0B_FREE_P->Wander_rno, 0x20, 0x3C);
    if (--G.pEm->Timer0 == 0)
    {
        G.pEm->Routine_3++;
        G.pEm->Timer0 = (Api_rand() & 0x1F) + 0x1E;
    }
}

VOID Em0b_Circle_right(VOID)
{
    FUNC(EM_WANDER_TURN)(EM0B_FREE_P->Wander_dir, &EM0B_FREE_P->Dir_ctrl_flg, &EM0B_FREE_P->Wander_rno, -0x20, 0x3C);
    if (--G.pEm->Timer0 == 0)
    {
        G.pEm->Routine_3--;
        G.pEm->Timer0 = (Api_rand() & 0x1F) + 0x5A;
    }
}

VOID Em0b_Move_type04(VOID)
{
    INT old_x;
    INT old_z;

    Em0b_devour_tbl[G.pEm->Routine_3]();
    old_x = G.pEm->Pos_x;
    old_z = G.pEm->Pos_z;
    FUNC(SCA_CK_HIT)(&G.pEm->Pos_x, 0x1F4);
    ScratchWord[0] = G.pEm->Pos_x - old_x;
    ScratchWord[1] = G.pEm->Pos_z - old_z;
    G.pEm->Root_x = G.pEm->Root_x + ScratchWord[0];
    G.pEm->Root_z = G.pEm->Root_z + ScratchWord[1];
    G.Player.Root_x += ScratchWord[0];
    G.Player.Root_z += ScratchWord[1];
    G.pEm->Old_x = G.pEm->Old_x + ScratchWord[0];
    G.pEm->Old_z = G.pEm->Old_z + ScratchWord[1];
    G.Player.Old_x += ScratchWord[0];
    G.Player.Old_z += ScratchWord[1];
}

VOID Em0b_Devour_seize(VOID)
{
    FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x100);  // address passed as an integer
    G.pEm->Move_no = 6;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0x3F;
    G.pEm->Routine_3++;
    G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
    G.pEm->Spd_cur = 0x96;
    FUNC(SND_SE_ENEM)(2, 0);
    G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
    G.Player.Be_flg |= EM_BE_NO_HIT | EM_BE_NO_WALL;
    PL_ROUTINE_WORD = ROUTINES(0, 0, 0xB, 7);
    G.Player.Cdir_y = G.pEm->Cdir_y;
}

VOID Em0b_Devour_carry(VOID)
{
    PARTS_W* pP_ptr;

    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
    pP_ptr = G.pEm->pSin_parts_ptr;
    ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
    ScratchPad->Vec.vx = 0xC8;
    if (G.pEm->Move_cnt == 0xF)
    {
        if (FUNC(L_POS_CK_XZM)(&pP_ptr[1].Workm, &ScratchPad->Vec, 0x3E8, &MAIN.Player.Pos_x) == 0)
        {
            PL_ROUTINE_WORD = ROUTINES(1, 0, 0, 1);
            G.Player.Move_cnt = 0;
            G.Player.Move_wait = 0;
            G.Player.Move_no = 0;
            G.Player.Hokan_flg = 0;
            G.Player.Timer0 = 0x64;
            G.Player.Damage_no = 0;
            G.Player.Spd_cur = 0;
            VIEW(SHORT, G.Player.Cdir_y) -= 0x800;  // signed 16-bit view of the USHORT
            G.pEm->Routine_3 = 5U;
            G.Player.Be_flg &= 0xF9;
            G.Player.Damage_no = 0;
            return;
        }
        G.Player.Pos_x = G.pEm->Pos_x;
        G.Player.Pos_z = G.pEm->Pos_z;
        G.pEm->Routine_3++;

        FUNC(PAD_VIB_SET1)(5, 0xFA, 0);
        FUNC(PAD_VIB_FADE_SET)(0xA, 0xFA, 0, 5);
        FUNC(PAD_VIB_SET0)(5, 0);
        FUNC(PAD_VIB_SET0)(8, 0xA);
        FUNC(PAD_VIB_SET0)(0xA, 0x15);
        FUNC(PAD_VIB_SET0)(0xA, 0x23);
        FUNC(PAD_VIB_SET1)(0xA, 0xFA, 0xF);
        FUNC(PAD_VIB_SET1)(0xA, 0xFA, 0x1A);
        FUNC(PAD_VIB_SET1)(0xA, 0xC8, 0x28);
        FUNC(PAD_VIB_FADE_SET)(0x2D, 0xC8, 0, 0x32);
    }
    if (G.pEm->Move_cnt < 3U)
    {
        ScratchPad->Vec.vx = (Api_rand() & 0x3FF) - 0x12C;
        ScratchPad->Vec.vy = -0x5DC;
        ScratchPad->Vec.vz = (Api_rand() & 0x3FF) - 0x2EE;

        FUNC(ESP_CALL)(0x17, 0xA, 0, G.pEm->M, &ScratchPad->Vec, 0);

        ScratchPad->Vec.vx = (Api_rand() & 0x3FF) - 0x5AA;
        ScratchPad->Vec.vy = -0x5DC;
        ScratchPad->Vec.vz = (Api_rand() & 0x3FF) - 0x2EE;

        FUNC(ESP_CALL)(0x17, 9, 0, G.pEm->M, &ScratchPad->Vec, 0);
    }
    if (G.pEm->Move_cnt >= 4U && G.pEm->Move_cnt < 5U)
    {
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchPad->Vec.vy = 0x1F4;

        FUNC(ESP_CALL)(0x17, 0xD, 0, &pP_ptr[12].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(0x17, 0xD, 0, &pP_ptr[10].Workm, &ScratchPad->Vec, 0);
    }
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em0b_Devour_bite(VOID)
{
    PARTS_W* pP_ptr;

    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
    if (G.pEm->Move_cnt == 0x13)
    {
        G.pEm->Routine_3++;
        pP_ptr = G.pEm->pSin_parts_ptr;
        FUNC(MUL_LMATRIX)(&pP_ptr[1].Workm, &MAIN.Player.M, &Em0b_catch_m);  // address passed as an integer
        G.Player.Routine_3++;
        G.Player.Sce_flg |= 0x40;
        pP_ptr = G.Player.pSin_parts_ptr;
        pP_ptr[1].Be_flg = pP_ptr[1].Be_flg & 0xFE;
        pP_ptr->Be_flg = pP_ptr->Be_flg & 0xFE;
        pP_ptr[9].Be_flg = pP_ptr[9].Be_flg & 0xFE;
        pP_ptr[12].Be_flg = pP_ptr[12].Be_flg & 0xFE;
        pP_ptr[10].Be_flg = pP_ptr[10].Be_flg & 0xFE;
        pP_ptr[13].Be_flg = pP_ptr[13].Be_flg & 0xFE;
        pP_ptr[11].Be_flg = pP_ptr[11].Be_flg & 0xFE;
        pP_ptr[14].Be_flg = pP_ptr[14].Be_flg & 0xFE;
        pP_ptr[2].Be_flg = pP_ptr[2].Be_flg & 0xFE;
    }
}

VOID Em0b_Devour_swallow(VOID)
{
    PARTS_W* pP_ptr;

    G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);

    G.Player.Life = -1;

    Em0b_catch_m.t[0] -= 0x41;
    Em0b_catch_m.t[1] += 0xD;
    pP_ptr = G.Player.pSin_parts_ptr;
    if (G.pEm->Move_cnt == 0x29)
    {
        pP_ptr[3].Be_flg = pP_ptr[3].Be_flg & 0xFE;
        pP_ptr[6].Be_flg = pP_ptr[6].Be_flg & 0xFE;
    }
    if (G.pEm->Move_cnt == 0x3B)
    {
        pP_ptr[4].Be_flg = pP_ptr[4].Be_flg & 0xFE;
        pP_ptr[7].Be_flg = pP_ptr[7].Be_flg & 0xFE;
        pP_ptr[5].Be_flg = pP_ptr[5].Be_flg & 0xFE;
        pP_ptr[8].Be_flg = pP_ptr[8].Be_flg & 0xFE;
    }
    if (G.pEm->Move_cnt < 0x38U)
    {
        if (G.pEm->Move_cnt >= 3U && G.pEm->Move_cnt < 5U)
        {
            ScratchPad->Vec.vx = (Api_rand() & 0x3FF) - 0x12C;
            ScratchPad->Vec.vy = -0x5DC;
            ScratchPad->Vec.vz = (Api_rand() & 0x3FF) - 0x2EE;

            FUNC(ESP_CALL)(0x17U, 0xA, 0, G.pEm->M, &ScratchPad->Vec, 0);

            ScratchPad->Vec.vx = (Api_rand() & 0x3FF) - 0x5AA;
            ScratchPad->Vec.vy = -0x5DC;
            ScratchPad->Vec.vz = (Api_rand() & 0x3FF) - 0x2EE;

            FUNC(ESP_CALL)(0x17U, 9, 0, G.pEm->M, &ScratchPad->Vec, 0);
        }
        if (!(G.pEm->Move_cnt & 7) && G.pEm->Move_cnt >= 8U)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0xC8;
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) - 0x100;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) - 0x100;

            FUNC(ESP_CALL)(0x17U, 0xA, 0, &pP_ptr[5].Workm, &ScratchPad->Vec, 0);

            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

            FUNC(ESP_CALL)(0x17U, 9, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);
        }
        if (!(G.pEm->Move_cnt & 7) && G.pEm->Move_cnt >= 0xBU)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0xC8;
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) - 0x100;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) - 0x100;

            FUNC(ESP_CALL)(0x17U, 0xA, 0, &pP_ptr[12].Workm, &ScratchPad->Vec, 0);
        }
    }
    else if (!(G.pEm->Move_cnt & 3))
    {
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchPad->Vec.vy = -0x64;

        FUNC(ESP_CALL)(0x17U, 0xD, 0, &pP_ptr[12].Workm, &ScratchPad->Vec, 0);

        ScratchPad->Vec.vy = 0;

        FUNC(ESP_CALL)(0x17U, 8, 0, &pP_ptr[6].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(0x17U, 8, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);
    }
    if (!((G.pEm->Move_cnt % 5) & 0xFF) && G.pEm->Move_cnt >= 2U)
    {
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

        FUNC(ESP_CALL)(0U, 0, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);
    }
}

VOID Em0b_Devour_finish(VOID)
{
    G.pEm->Be_flg = G.pEm->Be_flg & ~EM_BE_NO_HIT;
    EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
}

VOID Em0b_Devour_spit(VOID)
{
    PARTS_W* pP_ptr;

    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
    if (G.pEm->Move_cnt == 0x17)
    {
        G.pEm->Routine_3++;
        G.pEm->Timer0 = 0x1E;
        G.pEm->Be_flg = G.pEm->Be_flg & ~EM_BE_NO_HIT;
        G.pEm->Move_no = 0;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
    }
    if (G.pEm->Move_cnt >= 3U && G.pEm->Move_cnt < 5U)
    {
        ScratchPad->Vec.vx = (Api_rand() & 0x3FF) - 0x12C;
        ScratchPad->Vec.vy = -0x5DC;
        ScratchPad->Vec.vz = (Api_rand() & 0x3FF) - 0x2EE;

        FUNC(ESP_CALL)(0x17, 0xA, 0, G.pEm->M, &ScratchPad->Vec, 0);

        ScratchPad->Vec.vx = (Api_rand() & 0x3FF) - 0x5AA;
        ScratchPad->Vec.vy = -0x5DC;
        ScratchPad->Vec.vz = (Api_rand() & 0x3FF) - 0x2EE;

        FUNC(ESP_CALL)(0x17, 9, 0, G.pEm->M, &ScratchPad->Vec, 0);
    }
    if (G.pEm->Move_cnt >= 5U && !(G.pEm->Move_cnt & 3))
    {
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchPad->Vec.vy = 0xC8;

        FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);

        ScratchPad->Vec.vx = 0x320 - (Api_rand() & 0x3FF);

        FUNC(ESP_CALL)(0x17, 0xD, 0, &pP_ptr[12].Workm, &ScratchPad->Vec, 0);
    }
    if (!(G.pEm->Move_cnt & 7))
    {
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchPad->Vec.vy = -0x64;

        FUNC(ESP_CALL)(0x17, 0xD, 0, &pP_ptr[12].Workm, &ScratchPad->Vec, 0);

        ScratchPad->Vec.vy = 0;

        FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr[6].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);

        ScratchPad->Vec.vx = 0;
        ScratchPad->Vec.vy = -0x5DC;
        ScratchPad->Vec.vz = 0;

        FUNC(ESP_CALL)(0x17, 0xD, 0, &MAIN.Player.M[0], &ScratchPad->Vec, 0);
    }
}

VOID Em0b_Devour_recover(VOID)
{
    PARTS_W* pP_ptr;

    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);
    if (--G.pEm->Timer0 == 0)
    {
        EM_ROUTINE_WORD = ROUTINES(1, 0, 0, 1);
        G.pEm->Spd_cur = 0x78;
    }
    FUNC(ADD_SPEED_XZ)(0);
    if (--EM0B_FREE_P->effect_timer == 0)
    {
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchPad->Vec.vy = -0x64;

        FUNC(ESP_CALL)(0x17, 0xD, 0, &pP_ptr[12].Workm, &ScratchPad->Vec, 0);

        ScratchPad->Vec.vy = 0;

        FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr[6].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);

        EM0B_FREE_P->effect_timer = 0xB;
    }
}

VOID Em0b_Move_type05(VOID)
{
    Em0b_bite_tbl[G.pEm->Routine_3]();
}

VOID Em0b_Bite_begin(VOID)
{
    G.pEm->Routine_3++;
    G.pEm->Spd_cur = 0x64;
    G.pEm->Move_no = 7;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0x1F;
    G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
}

VOID Em0b_Bite_close(VOID)
{
    PARTS_W* pP_ptr;

    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80) != 0)
    {
        G.pEm->Routine_3 = 5U;
        G.pEm->Be_flg = G.pEm->Be_flg & ~EM_BE_NO_HIT;
        G.pEm->Timer0 = 0x1E;
        G.pEm->Move_no = 0;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        return;
    }
    if (G.pEm->Move_cnt < 0x1EU)
    {
        if (!(G.pEm->Move_cnt & 1))
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchPad->Vec.vx = 0x1F4;
            ScratchPad->Vec.vy = 0xC8;
            ScratchPad->Vec.vz = 0;

            FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);
        }
    }
    else if (!(G.pEm->Move_cnt & 1))
    {
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchPad->Vec.vy = -0x64;

        FUNC(ESP_CALL)(0x17, 0xD, 0, &pP_ptr[12].Workm, &ScratchPad->Vec, 0);

        ScratchPad->Vec.vy = 0;

        FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr[6].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);
    }
    FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x20);  // address passed as an integer
    pP_ptr = G.pEm->pSin_parts_ptr;
    ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
    ScratchPad->Vec.vx = 0xC8;
    if (FUNC(L_POS_CK_XZM)(&pP_ptr[1].Workm, &ScratchPad->Vec, 0x12C, &MAIN.Player.Pos_x) != 0)
    {
        G.pEm->Routine_3++;
        G.pEm->Move_no = 8;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        G.pEm->Root_x = G.pEm->Pos_x;
        G.pEm->Root_z = G.pEm->Pos_z;
        G.Player.Root_x = G.pEm->Pos_x;
        G.Player.Root_z = G.pEm->Pos_z;
        G.Player.Cdir_y = G.pEm->Cdir_y;
        PL_ROUTINE_WORD = ROUTINES(0, 0, 0xB, 6);
        G.Player.Be_flg |= EM_BE_NO_HIT;

        FUNC(PAD_VIB_FADE_SET)(0xA, 0, 0xC8, 0);
    }
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em0b_Bite_seize(VOID)
{
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80) != 0)
    {
        G.pEm->Routine_3++;
        FUNC(SND_SE_ENEM)(1, 0);
        G.pEm->Move_no = 9;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        G.Player.Move_no = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 3;
        EM0B_FREE_P->Catch_timer = 0x5A;

        FUNC(PAD_VIB_FADE_SET)(0xA, 0, 0xC8, 0);
        FUNC(PAD_VIB_SET0)(6, 5);
    }
    if (!(G.pEm->Move_cnt & 3))
    {
        ScratchPad->Vec.vx = 0;
        ScratchPad->Vec.vy = -0x5DC;
        ScratchPad->Vec.vz = 0;

        FUNC(ESP_CALL)(0x17, 0xD, 0, &MAIN.Player.M, &ScratchPad->Vec, 0);
    }
}

VOID Em0b_Bite_shake(VOID)
{
    PARTS_W* pP_ptr;

    EM0B_FREE_P->Catch_timer -= FUNC(GACHA_CH)() != 0 ? 8 : 1;
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80) != 0)
    {
        G.Player.Life -= 0x1E;

        if (!(G.pEm->Move_cnt & 3))
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = -0x64;

            FUNC(ESP_CALL)(0x17, 0xA, 0, &pP_ptr[12].Workm, &ScratchPad->Vec, 0);

            ScratchPad->Vec.vy = 0;

            FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr[6].Workm, &ScratchPad->Vec, 0);

            pP_ptr = G.Player.pSin_parts_ptr;

            FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr[11].Workm, &ScratchPad->Vec, 0);
        }
        if (G.Player.Life < 0)
        {
            G.Player.Life = 1;
        }
    }
    if (!(G.pEm->Move_cnt & 0xF))
    {
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchPad->Vec.vy = -0x64;

        FUNC(ESP_CALL)(0x17, 0xD, 0, &pP_ptr[12].Workm, &ScratchPad->Vec, 0);

        pP_ptr = G.Player.pSin_parts_ptr;
        ScratchPad->Vec.vy = 0;
        ScratchPad->Vec.vx = 0xC8;

        FUNC(ESP_CALL)(0x17, 0xD, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);

        ScratchPad->Vec.vx = 0;
        ScratchPad->Vec.vy = -0x5DC;
        ScratchPad->Vec.vz = 0;

        FUNC(ESP_CALL)(0x17, 0xD, 0, &MAIN.Player.M[0], &ScratchPad->Vec, 0);

        FUNC(PAD_VIB_FADE_SET)(0xF, 0x82, 0xB4, 0);
        FUNC(PAD_VIB_SET0)(4, 0);
        FUNC(PAD_VIB_SET0)(4, 0xA);
    }
    if (--EM0B_FREE_P->effect_timer == 0)
    {
        ScratchPad->Vec.vx = 0x258;
        ScratchPad->Vec.vy = -0xC8;
        ScratchPad->Vec.vz = -0x514;

        FUNC(ESP_CALL)(0x17, 0xD, 0, G.pEm->M, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(0, 0, 0, G.pEm->M, &ScratchPad->Vec, 0);

        EM0B_FREE_P->effect_timer = 0xB;
    }
    if (EM0B_FREE_P->Catch_timer < 0)
    {
        G.pEm->Routine_3++;
        G.pEm->Spd_cur = 0x64;
        G.pEm->Move_no = 0xA;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        G.Player.Move_no = 2;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 0;
        FUNC(SND_SE_ENEM)(1, 0);

        FUNC(PAD_VIB_SET1)(0x1E, 0x50, 0);
    }
}

VOID Em0b_Bite_release(VOID)
{
    PARTS_W* pP_ptr;

    FUNC(GOTO00)(&MAIN.Player.Pos_x, -0x10);  // address passed as an integer
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80) != 0)
    {
        G.pEm->Routine_3++;
        G.pEm->Timer0 = 0x1E;
        G.pEm->Be_flg = G.pEm->Be_flg & ~EM_BE_NO_HIT;
        G.pEm->Move_no = 0;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        PL_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
        G.Player.Damage_no = 0;
        G.Player.Be_flg &= ~EM_BE_NO_HIT;
    }
    if (!(G.pEm->Move_cnt & 7))
    {
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchPad->Vec.vy = -0x64;

        FUNC(ESP_CALL)(0x17, 0xD, 0, &pP_ptr[12].Workm, &ScratchPad->Vec, 0);

        ScratchPad->Vec.vy = 0;

        FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr[6].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);

        ScratchPad->Vec.vx = 0;
        ScratchPad->Vec.vy = -0x5DC;
        ScratchPad->Vec.vz = 0;

        FUNC(ESP_CALL)(0x17, 0xD, 0, &MAIN.Player.M, &ScratchPad->Vec, 0);
    }
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em0b_Bite_recover(VOID)
{
    PARTS_W* pP_ptr;

    FUNC(GOTO00)(&MAIN.Player.Pos_x, -0x20);  // address passed as an integer
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);
    if (--G.pEm->Timer0 == 0)
    {
        EM_ROUTINE_WORD = ROUTINES(1, 0, 0, 1);
        G.pEm->Spd_cur = 0x78;
    }
    FUNC(ADD_SPEED_XZ)(0);
    if (--EM0B_FREE_P->effect_timer == 0)
    {
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchPad->Vec.vy = -0x64;

        FUNC(ESP_CALL)(0x17, 0xD, 0, &pP_ptr[12].Workm, &ScratchPad->Vec, 0);

        ScratchPad->Vec.vy = 0;

        FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr[6].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);

        EM0B_FREE_P->effect_timer = 0xF;
    }
}

VOID Em0b_Die(VOID)
{
    if (G.pEm->Routine_1 == 0)
    {
        G.pEm->Routine_1++;
        EM_ROUTINE_HALF2 = 0;
    }
    Em0b_die_kind_tbl[(G.pEm->Type & 1)]();
}

VOID Em0b_Die_normal(VOID)
{
    PARTS_W* pP_ptr;

    Em0b_die_tbl[G.pEm->Routine_2]();
    if (!(G.pEm->Type & 2))
    {
        if (--EM0B_FREE_P->effect_timer == 0)
        {
            EM0B_FREE_P->effect_timer = 7;
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = -0x64;

            FUNC(ESP_CALL)(0x17, 0xD, 0, &pP_ptr[12].Workm, &ScratchPad->Vec, 0);

            ScratchPad->Vec.vy = 0;

            FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr[6].Workm, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);
        }
    }
}

VOID Em0b_Die_turn(VOID)
{
    PARTS_W* pP_ptr;

    G.pEm->Routine_2++;
    G.pEm->Spd_cur = 0x50;
    G.pEm->Timer0 = 0x1E;
    if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400) != 0)  // address passed as an integer
    {
        G.pEm->Routine_2 = 2U;
        G.pEm->Spd_cur = 0x78;
        return;
    }
    G.pEm->Move_no = 1;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 7;
    FUNC(SND_SE_ENEM)(2, 0);
    EM0B_FREE_P->effect_timer = 7;
    pP_ptr = G.pEm->pSin_parts_ptr;
    ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
    ScratchPad->Vec.vy = -0x64;

    FUNC(ESP_CALL)(0x17, 0xD, 0, &pP_ptr[12].Workm, &ScratchPad->Vec, 0x1E);

    ScratchPad->Vec.vy = 0;

    FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0x1E);
    FUNC(ESP_CALL)(0x17, 8, 0, &pP_ptr[9].Workm, &ScratchPad->Vec, 0x1E);

    Em0b_Die_thrash();
}

VOID Em0b_Die_thrash(VOID)
{
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)
    {
        G.pEm->Routine_2++;
        G.pEm->Spd_cur = 0x78;
        G.pEm->Move_no = 0;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;
        G.pEm->Cdir_y = (G.pEm->Cdir_y + 0x800) & 0xFFF;
    }
    FUNC(ADD_SPEED_XZ)(0x800);
}

VOID Em0b_Die_sink(VOID)
{
    ScratchPad->Vec.vx = G.pEm->Dest_x;
    ScratchPad->Vec.vz = G.pEm->Dest_z;
    FUNC(GOTO00)(&MAIN.Player.Pos_x, -0x20);  // address passed as an integer
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
    FUNC(ADD_SPEED_XZ)(0);
    if (--G.pEm->Timer0 == 0)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
        G.pEm->Hokan_flg = 0x3F;
        G.pEm->Damage_no = 0;
    }
}

VOID Em0b_Die_boss(VOID)
{
    Em0b_die_boss_tbl[G.pEm->Routine_2]();
}

VOID Em0b_Die_boss_begin(VOID)
{
    PARTS_W* pP_ptr;

    FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg, G.pEm->Em_set_flg);  // address passed as an integer

    pP_ptr = G.pEm->pSin_parts_ptr;
    G.pEm->Routine_2++;
    G.pEm->Spd_cur = 0x96;
    ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
    ScratchPad->Vec.vx = -0x64;

    FUNC(ESP_CALL)(0, 0U, 3, &pP_ptr[7].Workm, &ScratchPad->Vec, 0);

    G.pEm->Timer0 = 0x2D;
    G.pEm->Move_no = 2;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0x3F;
    FUNC(SND_SE_ENEM)(4, 0U);
    Em0b_Die_boss_charge();
}

VOID Em0b_Die_boss_charge(VOID)
{
    SHORT turning;

    FUNC(GOTO00)(&MAIN.Player.Pos_x, -8);  // address passed as an integer
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40) != 0)
    {
        G.pEm->Routine_2++;
        G.pEm->Move_no = 3;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;
        G.pEm->Spd_cur = 0x64;
    }
    if (G.pEm->Move_cnt >= 0xDU && G.pEm->Move_cnt < 0x19U)
    {
        G.pEm->Cdir_y = G.pEm->Cdir_y + 0x400;
        turning = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400);  // address passed as an integer
        G.pEm->Cdir_y = G.pEm->Cdir_y - 0x400;
        if (turning != 0)
        {
            FUNC(ADD_SPEED_XZ)(0x400);
            return;
        }
        FUNC(ADD_SPEED_XZ)(0xC00);
    }
}

VOID Em0b_Die_boss_charge2(VOID)
{
    SHORT turning;

    FUNC(GOTO00)(&MAIN.Player.Pos_x, -8);  // address passed as an integer
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40) != 0)
    {
        G.pEm->Routine_2++;
        G.pEm->Move_no = 4;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;
        G.pEm->Spd_cur = 0;
        G.pEm->Timer0 = 0x2D;
    }
    G.pEm->Cdir_y = G.pEm->Cdir_y + 0x400;
    turning = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400);  // address passed as an integer
    G.pEm->Cdir_y = G.pEm->Cdir_y - 0x400;
    if (turning != 0)
    {
        FUNC(ADD_SPEED_XZ)(0x400);
        return;
    }
    FUNC(ADD_SPEED_XZ)(0xC00);
}

VOID Em0b_Die_boss_beach(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        if (G.pEm->Move_cnt == 0)
        {
            FUNC(SND_SE_ENEM)(4, 0);
        }
        if ((--G.pEm->Timer0 < 0) && G.pEm->Move_cnt == 8)
        {
            G.pEm->Routine_3++;
            G.pEm->Move_no = 5;
            G.pEm->Move_cnt = 0U;
            G.pEm->Move_wait = 0;
            G.pEm->Hokan_flg = 0x3F;
            G.pEm->Timer0 = 0x46U;
        }
        break;
    case 1:
        G.pEm->Routine_2++;
        G.pEm->Routine_3 = 0U;
        G.pEm->Move_no = 5;

        FUNC(KAGE_WORK_COLOR_SET)(&G.pEm->Kage_work, 1, 2, 0xFFFF50);

        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, -0x64, -0x64);
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
        G.pEm->Timer0 = 0x5AU;
        break;
    }
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
}

VOID Em0b_Die_boss_fade(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Life = -1;

        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, 6, 6);
        if (--G.pEm->Timer0 == 0)
        {
            G.pEm->Routine_3++;
        }
    }
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
}

VOID Em0b_Nop3(VOID)
{
}

VOID Em0b_Dead(VOID)
{
}

VOID Em0b_Nop5(VOID)
{
}

VOID Pl_poison_die(VOID)
{
    if (G.Player.Routine_3 == 0)
    {
        G.Player.Routine_3++;
        G.Player.Move_no = 0;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 3;
        G.Player.Spd_cur = 0;
        G.Player.Damage_no = 1;
        FUNC(SND_SE_STAD)(3, 0, 0, &MAIN.Player.Pos_x);  // address passed as an integer
    }
    FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
}

VOID Pl_devour_die(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Routine_3++;
        G.Player.Move_no = 3;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 0;
        G.Player.Damage_no = 1;

    case 1:
        FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
        if (G.Player.Move_cnt == 0xE)
        {
            G.Player.Move_cnt--;
        }
        return;
    case 2:
        pP_ptr = G.Enemy[0].pSin_parts_ptr;  // reinterprets the memory as PARTS_W
        G.Player.Zone_in_flg |= 0x80;
        FUNC(ROT_MATRIX)(&MAIN.Enemy[0].Cdir_x, MAIN.Enemy[0].M);
        FUNC(GS_MUL_COORD0)(MAIN.Enemy[0].M, &pP_ptr->Coord, &ScratchPad->Mat.m[0][0]);
        FUNC(GS_MUL_COORD0)(&ScratchPad->Mat.m[0][0], &Em0b_catch_m, &MAIN.Player.M);
        return;
    }
}

// collision table of the large shark: two AT_TBL records
AT_TBL Em0b_sca_big[2] =
{
    { 0, 500, -800, 0, 0x320, 0x258 },   // 0 //
    { 0x8000, -500, -800, 0, 0x320, 0x258 },   // 1 //
};

// collision table of the small shark: two AT_TBL records
AT_TBL Em0b_sca_small[2] =
{
    { 0, 250, -400, 0, 0x190, 0x12C },   // 0 //
    { 0x8000, -250, -400, 0, 0x190, 0x258 },   // 1 //
};

MATRIX Em0b_catch_m = { 0 };  // held player transform relative to the mouth joint; t[0] and t[1] move while swallowing

// state handlers, indexed by the enemy state
VOID (*Em0b_routine_0[6])(VOID) =
{
    Em0b_Init,    // 0 //
    Em0b_Move,     // 1 //
    Em0b_Die,   // 2 //
    Em0b_Nop3,    // 3 //
    Em0b_Dead,    // 4 //
    Em0b_Nop5,    // 5 //
};

// behavior handlers, indexed by Routine_2
VOID (*Em0b_Move_type[6])(VOID) =
{
    Em0b_Move_type00,      // 0 //
    Em0b_Move_type01,     // 1 //
    Em0b_Move_type02,   // 2 //
    Em0b_Move_type03,    // 3 //
    Em0b_Move_type04,    // 4 //
    Em0b_Move_type05,      // 5 //
};

// swim steps, indexed by Act_sts
VOID (*Em0b_swim_tbl[3])(VOID) =
{
    Em0b_Swim_begin,   // 0 //
    Em0b_Swim_far,     // 1 //
    Em0b_Swim_near,    // 2 //
};

VOID (*Em0b_retreat_tbl[2])(VOID) = { Em0b_Retreat_begin, Em0b_Retreat_run };  // retreat steps, indexed by Act_sts

// circle steps, indexed by Act_sts
VOID (*Em0b_circle_tbl[3])(VOID) =
{
    Em0b_Circle_begin,   // 0 //
    Em0b_Circle_left,    // 1 //
    Em0b_Circle_right,   // 2 //
};

// devour steps, indexed by Act_sts
VOID (*Em0b_devour_tbl[7])(VOID) =
{
    Em0b_Devour_seize,     // 0 //
    Em0b_Devour_carry,     // 1 //
    Em0b_Devour_bite,      // 2 //
    Em0b_Devour_swallow,   // 3 //
    Em0b_Devour_finish,    // 4 //
    Em0b_Devour_spit,      // 5 //
    Em0b_Devour_recover,   // 6 //
};

// bite steps, indexed by Act_sts
VOID (*Em0b_bite_tbl[6])(VOID) =
{
    Em0b_Bite_begin,     // 0 //
    Em0b_Bite_close,     // 1 //
    Em0b_Bite_seize,     // 2 //
    Em0b_Bite_shake,     // 3 //
    Em0b_Bite_release,   // 4 //
    Em0b_Bite_recover,   // 5 //
};

VOID (*Em0b_die_kind_tbl[2])(VOID) = { Em0b_Die_normal, Em0b_Die_boss };  // death handlers, indexed by Type bit 0

// death steps of the normal shark, indexed by Routine_2
VOID (*Em0b_die_tbl[3])(VOID) =
{
    Em0b_Die_turn,     // 0 //
    Em0b_Die_thrash,   // 1 //
    Em0b_Die_sink,     // 2 //
};

// death steps of the boss shark, indexed by Routine_2
VOID (*Em0b_die_boss_tbl[5])(VOID) =
{
    Em0b_Die_boss_begin,     // 0 //
    Em0b_Die_boss_charge,    // 1 //
    Em0b_Die_boss_charge2,   // 2 //
    Em0b_Die_boss_beach,     // 3 //
    Em0b_Die_boss_fade,      // 4 //
};
