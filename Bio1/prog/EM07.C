/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM07.C
 */

#include "EM07.H"
#include "SCRATCHPAD.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Em07_Init(VOID)
{
    EM_ROUTINE_HALF0 = 0x101;
    EM_ROUTINE_HALF2 = 0;
    G.pEm->Coord_flg = 0;
    G.pEm->Timer0 = 0;
    G.pEm->Move_no = 0;
    G.pEm->Timer2 = 0;
    G.pEm->Damage_no = 0;
    G.pEm->Be_flg = EM_BE_ENABLED;
    EM07_EF(Pl_hit) = 0;
    EM07_EF(Chase_cnt) = 0;
    EM07_EF(timer) = 0x5A;
    FUNC(INIT_PARTS_WORK)();
    G.pEm->pAt_tbl = pEm07_sca;
    G.pEm->Scale = 0;
    if (G.pEm->Pos_y == 0)
    {
        G.pEm->Pos_y = -0xFA0;
    }
    EM07_EF(Big_flg) = 0U;
    if ((G.pEm->Type >> 1) & 1)
    {
        G.pEm->Scale = 0x2000;
        G.pEm->Type = G.pEm->Type - 2;
        EM07_EF(Big_flg) = 1U;
    }
    if (G.pEm->Type == 0x10)
    {
        G.pEm->Timer0 = 0x320 - (Api_rand() & 9) * 0x1E;
    }
    ScratchWord[7] = 0x606060;
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vx = 0;
    FUNC(KAGE_WORK_INIT)(&ScratchPad->Sv, &G.pEm->Kage_work, EM07_EF(Big_flg) * 0x64 + 0x64, EM07_EF(Big_flg) * 0x64 + 0x64);
    Api_rand();
    G.pEm->Life = (0xA + (Api_rand() & 7) + (Api_rand() & 7) + (Api_rand() & 7) + EM07_EF(Big_flg) * 0xA) * 2;
}

VOID Em07_Main(VOID)
{
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        Em07_routine_0[G.pEm->Routine_0]();
        if (G.pEm->Pos_y != 0 && G.pEm->Routine_2 != 0 && G.pEm->Routine_2 != 6)
        {
            FUNC(OBA_SET_OFS)(G.pEm);
            EM07_EF(Pl_hit) = FUNC(OBA_CK_HIT)(&MAIN.Player.Be_flg, G.pEm);
            FUNC(OBA_CK_EM)();
            EM07_EF(room_hit) = FUNC(SCA_CK_HIT)(&G.pEm->Pos_x, G.pEm->pAt_tbl->At_w);
        }
    }
    G.pEm->Zone_in_flg = FUNC(HIT_CK_POINT4)(&G.pEm->Pos_x, G.Cccut);
    if (G.pEm->Zone_in_flg != 0)
    {
        FUNC(MUL_KAGE)(&G.pEm->Pos_x, &G.pEm->Kage_work, 0U, G.pEm->Cdir_y);
    }
}

VOID Em07_Move(VOID)
{
    PARTS_W* pP_ptr;

    pP_ptr = G.pEm->pSin_parts_ptr;
    if (G.Player.Life < 0 && G.pEm->Routine_2 != 4)
    {
        EM07_EF(Bob_spd) = 0x23;
        EM07_EF(Climb_flg) = 1;
        G.pEm->Spd_cur = EM07_EF(player_distance) / 100 + 0xC8;
        G.pEm->Routine_1 = 0U;
        EM_ROUTINE_UHALF2 = 4;
    }
    if (EM07_EF(room_hit) != 0)
    {
        G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x80);
    }
    EM07_EF(player_distance) = (__builtin_abs(G.Player.Pos_x - G.pEm->Pos_x)) + (__builtin_abs(G.Player.Pos_z - G.pEm->Pos_z));
    switch (G.pEm->Routine_1)
    {
    case 0:
        EM07_EF(Chase_cnt)++;
        G.pEm->Spd_cur = G.pEm->Spd_cur + 2;
        if ((SHORT)EM07_EF(Chase_cnt) >= 0x1D)  // sign-extends the halfword
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x100) != 0)
            {
                G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x80);
                G.pEm->Spd_cur = G.pEm->Spd_cur - 8;
            }
        }
        if (G.pEm->Spd_cur < 0x1E)
        {
            G.pEm->Spd_cur = EM07_EF(player_distance) / 100 + 0xC8;
            G.pEm->Cdir_y = G.pEm->Cdir_y - FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200);
        }
        if (G.pEm->Spd_cur >= 0x191)
        {
            G.pEm->Spd_cur = 0x190U;
        }

    case 1:
        Em07_mv_call();
    }
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    G.pEm->Be_flg = G.pEm->Be_flg | 0x40;

    if (G.pEm->Pos_y < -0xFA0)
    {
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        FUNC(EM_CK_ALERT)(0xFA0);
    }
    if (G.pEm->Pos_y >= -0x3E7)
    {
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        FUNC(EM_CK_HANI)(0xFA0);
    }
    ScratchWord[3] = (pP_ptr->Workm.t[1] >> 4) + 0x12C + EM07_EF(Big_flg) * 0x64;
    if (ScratchWord[3] < 0)
    {
        ScratchWord[3] = EM07_EF(Big_flg) * 0x64 + 0x64;
    }
    FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, ScratchWord[3], ScratchWord[3]);
}

VOID Em07_mv_call(VOID)
{
    ScratchWord[7] = FUNC(EM_SIKAI_PROC)();
    if ((ScratchWord[7] >> 1) == 0)
    {
        EM07_EF(Sikai_flg) = ((SHORT)EM07_EF(Sikai_flg)) & ~1;  // sign-extends the halfword
        EM07_EF(Sikai_flg) = EM07_EF(Sikai_flg) | (ScratchWord[7] & 1);
    }
    ScratchWord[6] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x100);
    ScratchWord[5] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200);
    Em07_mv_tbl[G.pEm->Routine_2]();
}

VOID Em07_mv00(VOID)
{
    G.pEm->Damage_no = 1;
    G.pEm->Move_no = 0;
    Em07_mot_set();
    if (G.pEm->Type & 0xF0)
    {
        if (G.pEm->Type >= 0x20U)
        {
            G.pEm->Damage_no = 0;
            G.pEm->Routine_1 = 0;
            EM_ROUTINE_HALF2 = 2;
            EM07_EF(Bob_spd) = 0x23;
            EM07_EF(Climb_flg) = 1;
            G.pEm->Spd_cur = EM07_EF(player_distance) / 100 + 0xC8;
            return;
        }
        if (G.pEm->Type & 1)
        {
            EM_ROUTINE_HALF2 = 6;
            G.pEm->Spd_cur = 0x1E;
            G.pEm->Timer0 = 0U;
            G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
            return;
        }
        if ((G.pEm->Timer0-- != 0) && EM07_EF(player_distance) >= 0xFA0U)
        {
        }
        else
        {
            G.pEm->Type = 0x11U;
        }
        return;
    }
    if ((G.pEm->Type == 0 && EM07_EF(player_distance) < 0x1388U) || (G.pEm->Type != 0 && (--EM07_EF(timer) != -1)))
    {
        G.pEm->Damage_no = 0;
        EM_ROUTINE_HALF2 = 1;
        EM07_EF(timer) = 0x5AU;
        G.pEm->Type = G.pEm->Type | 0x20;
        G.pEm->Spd_cur = 0x32;
    }
}

VOID Em07_mv01(VOID)
{
    G.pEm->Move_no = 1;
    G.pEm->Spd_cur = G.pEm->Spd_cur + 1;
    Em07_mot_set();
    G.pEm->Cdir_y = G.pEm->Cdir_y + 0x55;
    FUNC(ADD_SPEED_XZ)(0x800 - G.pEm->Move_cnt * 0x55);
    if (G.pEm->Move_cnt == 0x18)
    {
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = 2;
        EM07_EF(Bob_spd) = 0x23;
        EM07_EF(Climb_flg) = 1;
    }
}

VOID Em07_mv02(VOID)
{
    PARTS_W* pP_ptr;

    pP_ptr = G.pEm->pSin_parts_ptr;
    pP_ptr[1].Be_flg = 0;
    pP_ptr[2].Be_flg = 0;
    pP_ptr[3].Be_flg = 0;
    pP_ptr[4].Be_flg = 0;
    G.pEm->Move_no = 2;
    Em07_mot_set();
    if (!((G.pEm->Move_cnt % 7) & 0xFF))
    {
        pP_ptr[1].Be_flg = 3;
        pP_ptr[2].Be_flg = 3;
        pP_ptr[3].Be_flg = 3;
        pP_ptr[4].Be_flg = 3;
        if (EM07_EF(player_distance) < 0x3E8U)
        {
            FUNC(SND_SE_ENEM)(0U, 0);
        }
    }
    FUNC(ADD_SPEED_XZ)(0U);
    EM07_EF(Bob_spd) = EM07_EF(Bob_spd) + 1;
    if ((SHORT)EM07_EF(Bob_spd) >= 0x97)  // sign-extends the halfword
    {
        EM07_EF(Bob_spd) = 0x23U;
    }
    if (G.pEm->Pos_y < (UINT)(-0xDAC - (Api_rand() & 1) * 0x1F4))  // unsigned conversion (signed operand)
    {
        EM07_EF(Climb_flg) = 0U;
    }
    if (G.pEm->Pos_y > (UINT)(-0x5DC - (Api_rand() & 1) * 0x1F4))  // unsigned conversion (signed operand)
    {
        EM07_EF(Climb_flg) = 1U;
    }
    if (EM07_EF(Climb_flg) != 0)
    {
        G.pEm->Pos_y = G.pEm->Pos_y - (SHORT)EM07_EF(Bob_spd);  // sign-extends the halfword
    }
    else
    {
        G.pEm->Pos_y = G.pEm->Pos_y + (SHORT)EM07_EF(Bob_spd);  // sign-extends the halfword
    }
    if (EM07_EF(Pl_hit) != 0 && EM07_EF(player_distance) < 0x3E8U && FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x100) == 0 && -G.pEm->Pos_y - 0x8FC < 0x258U && (EM07_EF(Chase_cnt) = 0, pP_ptr[1].Be_flg = 3, pP_ptr[2].Be_flg = 3, pP_ptr[3].Be_flg = 3, pP_ptr[4].Be_flg = 3, G.pEm->Routine_1 = 0, EM_ROUTINE_HALF2 = 2, EM07_EF(Bob_spd) = 0x23U, EM07_EF(Climb_flg) = 1U, G.pEm->Spd_cur = EM07_EF(player_distance) / 100 + 0xC8, G.Player.Damage_no == 0) && (((SHORT)EM07_EF(Sikai_flg)) & 1) != 0 && (G.pEm->Routine_1 = 1, EM_ROUTINE_HALF2 = 3, FUNC(CDIR_CK)(&MAIN.Player) == 0) && FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0xC0) == 0)  // pointer type conversion (no code); the statement list is the chase start transition
    {
        if (EM07_EF(Big_flg) != 0)
        {
            return;
        }
        EM_ROUTINE_HALF2 = 5;
        G.pEm->Cdir_y = FUNC(GET_DIR_TGT)(G.Player.Pos_x, G.Player.Pos_z);
        G.Player.Damage_no = 1;
        G.pEm->Damage_no = 1;
        return;
    }
}

VOID Em07_mv03(VOID)
{
    G.pEm->Move_no = 3;
    G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x30);  // address passed as an integer
    Em07_mot_set();
    if (EM07_EF(Seq_end) != 0)
    {
        if (G.Player.Damage_no == 0 && EM07_EF(player_distance) < 0x5DCU)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x100) == 0 && (((SHORT)EM07_EF(Sikai_flg)) & 1) != 0)  // address passed as an integer
            {
                G.Player.Damage_no = 1;
                G.Player.Routine_2 = 0x64;

                FUNC(PAD_VIB_SET0)(3, 0);

                G.Player.Life -= (EM07_EF(Big_flg) + 1) * 0xA;

                if (G.Player.Life < 0)
                {
                    G.Player.Life = 1;
                    if (EM07_EF(Big_flg) != 0)
                    {
                        PL_ROUTINE_HALF0 = 3;
                        PL_ROUTINE_HALF2 = 0xC8;
                        G.Player.Life = -1;
                        return;
                    }
                }
            }
        }
        G.pEm->Spd_cur = 0x64;
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 1;
    }
}

VOID Em07_mv04(VOID)
{
    PARTS_W* pP_ptr;

    pP_ptr = G.pEm->pSin_parts_ptr;
    pP_ptr[1].Be_flg = 0;
    pP_ptr[2].Be_flg = 0;
    pP_ptr[3].Be_flg = 0;
    pP_ptr[4].Be_flg = 0;
    G.pEm->Move_no = 2;

    G.pEm->Cdir_y += (VIEW(SHORT, EM07_FREE_P->Sikai_flg) & 1) * FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x10U) * 0x2;  // SHORT* view of the field (a plain access changes the code)

    Em07_mot_set();
    if (!((G.pEm->Move_cnt % 7) & 0xFF))
    {
        pP_ptr[1].Be_flg = 3;
        pP_ptr[2].Be_flg = 3;
        pP_ptr[3].Be_flg = 3;
        pP_ptr[4].Be_flg = 3;
    }
    EM07_EF(Bob_spd) = EM07_EF(Bob_spd) + 1;
    if ((SHORT)EM07_EF(Bob_spd) >= 0x97)  // sign-extends the halfword
    {
        EM07_EF(Bob_spd) = 0x23U;
    }
    if (G.pEm->Pos_y < (UINT)(-0xDAC - (Api_rand() & 1) * 0x1F4))  // unsigned conversion (signed operand)
    {
        EM07_EF(Climb_flg) = 0U;
        EM07_EF(Bob_spd) = 0x23U;
    }
    if (G.pEm->Pos_y > (UINT)(-0x5DC - (Api_rand() & 1) * 0x1F4))  // unsigned conversion (signed operand)
    {
        EM07_EF(Climb_flg) = 1U;
        EM07_EF(Bob_spd) = 0x23U;
    }
    if (EM07_EF(Climb_flg) != 0)
    {
        G.pEm->Pos_y = G.pEm->Pos_y - (SHORT)EM07_EF(Bob_spd);  // sign-extends the halfword
    }
    else
    {
        G.pEm->Pos_y = G.pEm->Pos_y + (SHORT)EM07_EF(Bob_spd);  // sign-extends the halfword
    }
    if (EM07_EF(player_distance) < 0x7D0U)
    {
        FUNC(ADD_SPEED_XZ)(0x400U);
        return;
    }
    FUNC(ADD_SPEED_XZ)(0U);
}

VOID Em07_mv05(VOID)
{
    Em07_mv05_main();
}

VOID Em07_mv06(VOID)
{
    G.pEm->Move_no = 2;
    G.pEm->Spd_cur = G.pEm->Spd_cur + 1;
    Em07_mot_set();
    G.pEm->Pos_y = G.pEm->Pos_y + 0x14;
    if (G.pEm->Pos_y >= -0xAEF)
    {
        G.pEm->Pos_y = G.pEm->Pos_y + 0x14;
        FUNC(ADD_SPEED_XZ)(0);
        G.pEm->Timer0++;
        if (G.pEm->Timer0 >= 0x15)
        {
            G.pEm->Pos_y = G.pEm->Pos_y - 0x3C;
        }
    }
    if (G.pEm->Timer0 >= 0x1A)
    {
        G.pEm->Be_flg = G.pEm->Be_flg & ~EM_BE_NO_HIT;
        G.pEm->Damage_no = 0;
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = 2;
        EM07_EF(Bob_spd) = 0x23;
        EM07_EF(Climb_flg) = 1;
    }
}

VOID Em07_mot_set(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Timer2 = 0;
        G.pEm->Hokan_flg = 3;

    case 1:
        EM07_EF(Seq_end) = FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        return;
    }
}

VOID Em07_mv05_main(VOID)
{
    INT unused;    // unused local: reserves stack space

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Routine_3 = 1U;
        G.pEm->Hokan_flg = 3;
        G.pEm->Cdir_y = G.Player.Cdir_y;
        G.pEm->Pos_y = 0;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
        G.pEm->Move_no = (G.Player.Id & 1) + 4;
        G.pEm->Root_x = G.Player.Pos_x;
        G.pEm->Root_z = G.Player.Pos_z;
        G.Player.Root_x = G.Player.Pos_x;
        G.Player.Root_z = G.Player.Pos_z;
        G.Player.Damage_no = 1;
        PL_ROUTINE_HALF0 = 0x705;
        PL_ROUTINE_HALF2 = 0;

    case 1:
        FUNC(NULL_POS_SET)(G.pEm, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr);
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        G.pEm->Timer2++;
        if (G.pEm->Move_cnt == 4)
        {
            FUNC(SND_SE_STAD)(3, 0, 0, &MAIN.Player.Pos_x);
        }
        if (G.pEm->Move_cnt == (G.Player.Id & 1) * 2 + 0x22)
        {
            G.pEm->Routine_3 = 2U;
        }
        return;
    case 2:
        G.pEm->Life = -0x2C;
        if (Api_rand() & 1)
        {
            G.Player.Sca_info |= 2;
            G.Player.Poison_timer = 0x96;
        }

        G.Player.Life -= 0xF;

        if (G.Player.Life < 0)
        {
            G.Player.Life = 1;
        }
        EM_ROUTINE_HALF0 = 3;
        EM_ROUTINE_HALF2 = 0;
        return;
    }
}

VOID Em07_Damage(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Spd_cur = 0;
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Timer0 = 0x1E;
        G.pEm->Hokan_flg = 3;
        G.pEm->Damage_no = 1;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_OBJ;
        FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, 0, 0);
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec.vz = 0;
        ScratchPad->Vec.vy = 0;
        ScratchPad->Vec.vx = 0;

        FUNC(ESP_CALL)(0, 0x11, 0, &pP_ptr[0].Workm, &ScratchPad->Vec, 0);

        if (EM07_EF(Big_flg) != 0)
        {
            ScratchWord[7] = 4;
            while ((ScratchWord[7] = ScratchWord[7] - 1) != -1)
            {
                FUNC(JOINT_ATCK_SET)(&pP_ptr[ScratchWord[7] + 1], 0x1E, 0xA, 3);
            }
            FUNC(SND_SE_ENEM)(2, 0);
        }

    case 1:
        G.pEm->Pos_y = G.pEm->Pos_y + 0xC8;
        if (G.pEm->Pos_y >= -0x63)
        {
            G.pEm->Pos_y = 0;
            G.pEm->Routine_3 = 2U;
            EM07_EF(timer) = 0x1EU;
            FUNC(KAGE_WORK_COLOR_SET)(&G.pEm->Kage_work, 1, 2, 0xFF3030);
        }
        return;
    case 2:
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, 8, 8);
        if (--EM07_EF(timer) == -1)
        {
            G.pEm->Routine_3 = 3U;

            G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;

            G.pEm->Damage_no = 0;
            if (EM07_EF(Big_flg) != 0)
            {
                EM07_EF(timer) = 0x14U;
            }
        }
        return;
    case 3:
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        FUNC(EM_CK_HANI)(0xFA0);
        EM07_EF(player_distance) = (__builtin_abs(G.Player.Pos_x - G.pEm->Pos_x)) + (__builtin_abs(G.Player.Pos_z - G.pEm->Pos_z));
        if (EM07_EF(player_distance) < 0x190U && G.Player.Spd_cur != 0)
        {
            G.pEm->Damage_no = 1;

            G.pEm->Life = -4;
            FUNC(SND_SE_ENEM)(2, 0);
            EM_ROUTINE_HALF0 = 3;
            EM_ROUTINE_HALF2 = 0;

            FUNC(PAD_VIB_SET0)(1, 0);
            FUNC(PAD_VIB_SET1)(4, 0x5A, 0);
        }
    }
}

VOID Em07_Die(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Spd_cur = 0;
        G.pEm->Routine_3 = 4U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Timer0 = 0x1E;
        G.pEm->Hokan_flg = 3;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_OBJ;
        FUNC(SND_SE_ENEM)(1, 0U);
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec.vz = 0;
        ScratchPad->Vec.vy = 0;
        ScratchPad->Vec.vx = 0;

        FUNC(ESP_CALL)(0, 0x11U, 0, &pP_ptr[0].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(0, 0x11U, 0, &pP_ptr[9].Workm, &ScratchPad->Vec, 0);

        if (EM07_EF(Big_flg) != 0 || ((G.pEm->Damage_no >> 3) & 0xFF) >= 7U)
        {
            ScratchWord[7] = 0xA;
            while ((ScratchWord[7] = ScratchWord[7] - 1) != -1)
            {
                FUNC(JOINT_ATCK_SET)(&pP_ptr[ScratchWord[7]], 0x1EU, 0xA, 3);
            }
            FUNC(SND_SE_ENEM)(2, 0U);
        }
        if (EM07_EF(timer) == 0x14)
        {
            G.pEm->Routine_3 = 1U;
            return;
        }
        FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, 0U, 0);
        if (EM07_EF(Big_flg) != 0 || ((G.pEm->Damage_no >> 3) & 0xFF) >= 7U)
        {
            G.pEm->Routine_3 = 4U;
            FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);
            return;
        }
        ScratchPad->Vec.vz = 0;
        ScratchPad->Vec.vy = 0;
        ScratchPad->Vec.vx = 0;

        FUNC(ESP_CALL)(0, 0x11U, 0, &pP_ptr[7].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(0, 0x11U, 0, &pP_ptr[8].Workm, &ScratchPad->Vec, 0);

        G.pEm->Routine_3 = 2U;
        EM07_EF(timer) = 0x14;
        return;
    case 1:
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, 8U, 8);
        if (--EM07_EF(timer) == -1)
        {
            G.pEm->Routine_3 = 4U;
            FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);
        }
        return;
    case 2:
        G.pEm->Scale = 1;
        if (--EM07_EF(timer) == -1)
        {
            G.pEm->Type = 0x10;
            if (EM07_EF(Respawn_cnt) >= 0xBU)
            {
                EM07_EF(Respawn_cnt) = 0U;
            }
            if (EM07_EF(Respawn_cnt) == 0xA)
            {
                G.pEm->Type = 0x12;
            }
            EM07_EF(Respawn_cnt) = EM07_EF(Respawn_cnt) + 1;
            G.pEm->Cdir_y = 0x400;
            G.pEm->Pos_x = 0x4650 - (Api_rand() & 1) * 0x64;
            G.pEm->Pos_y = -0xFA0 - (Api_rand() & 1) * 0x64;
            G.pEm->Pos_z = 0x5974 - (Api_rand() & 1) * 0x64;
            EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 0);
        }
        return;
    }
}

VOID Pl_mv_crawl(VOID)
{
    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 0;
        G.Player.Routine_3 = 1;
        G.Player.Hokan_flg = 3;
        G.Player.Spd_cur = 0;

        FUNC(PAD_VIB_SET0)(3, 0);

    case 1:
        FUNC(NULL_POS_SET)(&MAIN.Player, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr);  // address passed as an integer
        if (FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400) != 0)
        {
            PL_ROUTINE_HALF0 = 1;
            PL_ROUTINE_HALF2 = 0;
            G.Player.Damage_no = 0;
        }
        return;
    }
}

AT_TBL Em07_sca0 = { 0x8000, 0, 0, 0, 0, 0 };  // flags 0x8000, no offset or size

AT_TBL Em07_sca1 = { 0x8001, 0, 8000, 0, 0, 0 };  // flags 0x8001, y offset 8000; unused

AT_TBL* pEm07_sca = &Em07_sca0;  // collision info installed on every wasp

AT_TBL* Em07_sca1_adr = &Em07_sca1;  // address of Em07_sca1; unused

// state handlers, indexed by the enemy state
VOID (*Em07_routine_0[4])(VOID) =
{
    Em07_Init,      // 0 //
    Em07_Move,       // 1 //
    Em07_Damage,   // 2 //
    Em07_Die,     // 3 //
};

// behavior handlers, indexed by Routine_2
VOID (*Em07_mv_tbl[7])(VOID) =
{
    Em07_mv00,         // 0 //
    Em07_mv01,      // 1 //
    Em07_mv02,        // 2 //
    Em07_mv03,        // 3 //
    Em07_mv04,      // 4 //
    Em07_mv05,   // 5 //
    Em07_mv06,       // 6 //
};
