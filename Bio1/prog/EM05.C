/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM05.C
 */

#include "EM05.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Em05_Init(VOID)
{
    EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);  // state 1, Key_rno 0, Routine_2 0, Act_sts 0
    G.pEm->Coord_flg = 0;
    G.pEm->Dummy0c1 = 0;
    G.pEm->Timer0 = 0;
    G.pEm->Move_no = 2;
    G.pEm->Timer2 = 0;
    G.pEm->Damage_no = 0;
    G.pEm->Sca_info |= 4;
    if (G.pEm->Type & 0x10)
    {
        EM05_EF(vert_speed) = 0xC8;

        G.pEm->Spd_cur = ((Api_rand() & 1) << 4) + 0x32;  // rand
        G.pEm->Cdir_z = 0x100;
        EM_ROUTINE_WORD = ROUTINES(0, 5, 1, 1);  // state 1, Key_rno 1, Routine_2 5, Act_sts 0
        G.pEm->Move_no = 0;
    }
    if (G.pEm->Type & 2)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 8);  // state 8, Key_rno 0, Routine_2 0, Act_sts 0
        G.pEm->Move_no = 0;
    }
    EM05_EF(wall_hit_frames) = 0;
    EM05_EF(Unk_17e) = 0x64;
    EM05_EF(swerve_latch) = 0;
    FUNC(INIT_PARTS_WORK)();  // Init_Parts_work
    FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
    ScratchWord[7] = 0x808080;
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vx = 0;
    FUNC(KAGE_WORK_INIT)(&ScratchPad->Sv, &G.pEm->Kage_work, 0xC8, 0xC8);  // Kage_work_init
    Api_rand();  // rand
    G.pEm->Life = ((Api_rand() & 7) + 5 + (Api_rand() & 7) + (Api_rand() & 7)) * 2;  // rand
    EM05_EF(Lim_ofs) = 0;
    if (G.pEm->Type & 1)
    {
        G.pEm->Life = 0;
        EM05_EF(Lim_ofs) = -0x190;
    }
    G.pEm->pAt_tbl = pEm05_sca;
    G.pEm->Scale = 0x14CC;
}

VOID Em05_Main(VOID)
{
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        Em05_routine_0[G.pEm->Routine_0]();
        FUNC(OBA_SET_OFS)(G.pEm);  // Oba_set_ofs
        EM05_EF(Pl_hit) = FUNC(OBA_CK_HIT)(&MAIN.Player.Be_flg, G.pEm);  // Oba_ck_hit
        FUNC(OBA_CK_EM)();  // Oba_ck_em
        EM05_EF(room_hit) = FUNC(SCA_CK_HIT)(&G.pEm->Pos_x, G.pEm->pAt_tbl->At_w);  // Sca_ck_hit
        G.pEm->Ground = ScratchHalf[14];
    }
    G.pEm->Zone_in_flg = FUNC(HIT_CK_POINT4)(&G.pEm->Pos_x, G.Cccut);  // Hit_ck_point4
    if (G.pEm->Pos_y > 0)
    {
        EM05_EF(vert_speed) = 0xC8;

        G.pEm->Pos_y = -0x64;
        EM_ROUTINE_WORD = ROUTINES(0, 7, 1, 1);  // state 1, Key_rno 1, Routine_2 7, Act_sts 0
    }
    if (G.pEm->Pos_y < -0x7530)
    {
        EM05_EF(vert_speed) = 0xC8;

        G.pEm->Pos_y = -0x7148;
        EM_ROUTINE_WORD = ROUTINES(0, 0xA, 1, 1);  // state 1, Key_rno 1, Routine_2 0xA, Act_sts 0
    }
    ScratchWord[3] = (G.pEm->Pos_y >> 4) + 0x190;
    if (ScratchWord[3] < 0)
    {
        ScratchWord[3] = 0x32;
    }
    FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, ScratchWord[3], ScratchWord[3]);  // Kage_half_set

    if (G.pEm->Zone_in_flg != 0 && G.pEm->Ground >= -0x63 && EM_ROUTINE_UHALF0 != 1)
    {
        FUNC(MUL_KAGE)(&G.pEm->Pos_x, &G.pEm->Kage_work, 0U, G.pEm->Cdir_y);  // MulKage
    }
}

VOID Em05_Move(VOID)
{
    if (G.pEm->Type & 0x80)
    {
        return;
    }
    if (G.pEm->Type & 0x10)
    {
        if (EM05_EF(Pl_hit) != 0 && G.Player.Damage_no == 0 && G.pEm->Routine_2 != 9)
        {
            if (G.pEm->Type & 1)
            {
                if (-G.pEm->Pos_y - 0x9C4 >= 0x12DU)
                {
                    G.pEm->Pos_y = -0x9C4;
                }
                EM_ROUTINE_WORD = ROUTINES(0, 9, 1, 1);  // state 1, Key_rno 1, Routine_2 9, Act_sts 0
                return;
            }
            if (G.pEm->Spd_cur >= 0x12D)
            {
                if (FUNC(CDIR_CK)(&MAIN.Player) == 0)  // Cdir_ck
                {
                    EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 2);  // state 2, Key_rno 0, Routine_2 0, Act_sts 0
                    G.Player.Damage_no = 1;
                    G.Player.Routine_2 = 0x64;

                    FUNC(PAD_VIB_SET0)(5, 0);  // Pad_vib_set0
                    FUNC(PAD_VIB_SET1)(2, 0xC8, 0);  // Pad_vib_set1

                    return;
                }
            }
        }
        if (EM05_EF(room_hit) != 0)
        {
            ++EM05_EF(wall_hit_frames);
            if (EM05_EF(wall_hit_frames) >= 6 && (G.pEm->Type & 1))
            {
                G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x40);  // Goto00_ck
            }
            if (EM05_EF(wall_hit_frames) >= 0x1F && G.Player.Damage_no == 0)
            {
                EM05_EF(wall_hit_frames) = 0;
                if (G.pEm->Spd_cur >= 0x12D && G.Player.Damage_no == 0 && !(G.pEm->Type & 1))
                {
                    EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 2);  // state 2, Key_rno 0, Routine_2 0, Act_sts 0
                    return;
                }
                ScratchPad->Vec.vx = G.Player.Pos_x;
                ScratchPad->Vec.vy = G.Player.Pos_y;
                ScratchPad->Vec.vz = G.Player.Pos_z;
                ScratchWord[7] = 4;
                G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(EM_WALL_TURN)(0x40, (UCHAR)EM05_EF(room_hit), &EM05_FREE_P->swerve_angle, &EM05_FREE_P->swerve_latch);  // Em_wall_turn
            }
        }
        if (G.Player.Damage_no != 0)
        {
            G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x40);  // Goto00_ck
            if (G.Player.Routine_3 >= 4)
            {
                G.pEm->Cdir_y += FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x80) == 0 ? 1 : 0;  // Goto00_ck
            }
            if (G.Player.Life < 0 && G.Player.Routine_2 == 0xC8 && G.pEm->Routine_2 == 9)
            {
                EM_ROUTINE_WORD = ROUTINES(0, 6, 1, 1);  // state 1, Key_rno 1, Routine_2 6, Act_sts 0
            }
        }
    }
    EM05_EF(player_dist) = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);
    switch (G.pEm->Routine_1)
    {
    case 0:
        if (PL_ROUTINE_WORD == ((0) << 24 | (0x14) << 16 | (3) << 8 | (1)) && !(G.pEm->Type & 0x10))  // state 1, Key_rno 3, Routine_2 0x14, Act_sts 0
        {
            G.pEm->Cdir_y = G.pEm->Cdir_y + (Api_rand() & 1) * 0x40;  // rand
            EM_ROUTINE_WORD = ROUTINES(0, 0xD, 1, 1);  // state 1, Key_rno 1, Routine_2 0xD, Act_sts 0
        }

    case 1:
        Em05_mv_main();
    }
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    FUNC(EM_CK_ALERT)(0x1388);  // Em_ck_alert

    if (G.pEm->Pos_y >= -0x1193)
    {
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        G.pEm->Be_flg = G.pEm->Be_flg | 0x40;
        if (G.pEm->Pos_y >= -0x7CF)
        {
            FUNC(EM_CK_HANI)(0x7D0);  // Em_ck_hani
        }
    }

    if (G.pEm->Pos_y >= -0x5DB)
    {
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        FUNC(EM_CK_HANI)(0x1388);  // Em_ck_hani
    }
}

VOID Em05_Damage(VOID)
{
    switch (G.pEm->Routine_2)
    {
    case 0:
        G.pEm->Move_no = 6;
        Em05_seq_hold();
        G.pEm->Spd_cur = 0xC8;
        G.pEm->Pos_y = G.pEm->Pos_y + 0xC8;
        if (EM05_EF(Pl_hit) != 0)
        {
            FUNC(ADD_SPEED_XZ)(0x800);  // Add_speedXZ
        }
        if (G.pEm->Move_cnt == 1)
        {
            FUNC(SND_SE_ENEM)(2, 0);  // Snd_se_enem
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

            FUNC(ESP_CALL)(0x1C, 0, G.pEm->Cdir_y, G.pEm->M, &ScratchPad->Vec, 0);  // Esp_call
            FUNC(ESP_CALL)(0x1C, 0, G.pEm->Cdir_y + 0x400, G.pEm->M, &ScratchPad->Vec, 0);  // Esp_call
        }
        if (G.pEm->Pos_y >= -0x1C1)
        {
            EM_ROUTINE_UHALF2 = 1;
            G.pEm->Pos_y = -0x1C2;
        }
        return;
    case 1:
        G.pEm->Move_no = 9;
        Em05_seq_step();
        if (G.pEm->Routine_3 == 0)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);  // state 1, Key_rno 1, Routine_2 3, Act_sts 0
            G.pEm->Damage_no = 0;

            G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
            FUNC(EM_CK_HANI)(0x1388);  // Em_ck_hani
        }

        return;
    }
}

VOID Em05_Die(VOID)
{
    PARTS_W* pP_ptr;

    if (G.pEm->Ground < -0x7D0)
    {
        G.pEm->Ground = 0x64;
    }
    switch (G.pEm->Routine_2)
    {
    case 0:
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);  // Flg_on
        G.pEm->Move_no = 6;
        if (!(G.pEm->Damage_no & 1))
        {
            G.pEm->Spd_cur = 0xC8;
            FUNC(ADD_SPEED_XZ)(FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400U) == 0 ? 0x800 : 0);  // Add_speedXZ, Goto00_ck
        }
        Em05_seq_hold();
        G.pEm->Pos_y = G.pEm->Pos_y + 0xC8;
        G.pEm->Cdir_z = G.pEm->Cdir_z + 0x80;
        if (G.pEm->Move_cnt == 1)
        {
            FUNC(SND_SE_ENEM)(0, 0U);  // Snd_se_enem
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

            FUNC(ESP_CALL)(0x1C, 0U, G.pEm->Cdir_y, G.pEm->M, &ScratchPad->Vec, 0);  // Esp_call
            FUNC(ESP_CALL)(0x1C, 0U, G.pEm->Cdir_y + 0x400, G.pEm->M, &ScratchPad->Vec, 0);  // Esp_call
        }
        if (G.pEm->Pos_y >= -0x1C1)
        {
            if (G.pEm->Ground == 0x64)
            {
                EM_ROUTINE_UHALF2 = 2;
                break;
            }
            EM_ROUTINE_UHALF2 = 1;
            G.pEm->Pos_y = -0x1C2;
            G.pEm->Cdir_z = 0U;

            FUNC(KAGE_WORK_COLOR_SET)(&G.pEm->Kage_work, 1U, 2, 0xFFFF50);  // Kage_work_color_set

            FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, -0x64U, -0x64);  // Kage_work_size_add
        }
        break;
    case 1:
        G.pEm->Move_no = 0xA;
        Em05_seq_step();
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, 6U, 6);  // Kage_work_size_add
        if (G.pEm->Routine_3 == 0)
        {
            EM_ROUTINE_HALF2 = 4;
        }
        return;
    case 2:
        pP_ptr = G.pEm->pSin_parts_ptr;

        FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, 0U, 0);  // Kage_half_set

        ScratchWord[7] = G.pEm->Parts_num;
        while ((ScratchWord[7] = ScratchWord[7] - 1) != -1)
        {
            FUNC(JOINT_ATCK_SET)(pP_ptr + ScratchWord[7], 0x1EU, 0x1E, 3);  // Joint_atck_set
        }

        FUNC(SND_SE_ENEM)(0, 0U);  // Snd_se_enem
        EM_ROUTINE_HALF2 = 4;

    case 4:

        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_OBJ;
        return;
    }
}

VOID Em05_mv_main(VOID)
{
    ScratchWord[7] = FUNC(EM_SIKAI_PROC)();  // Em_sikai_proc
    if ((ScratchWord[7] >> 1) == 0)
    {
        EM05_EF(Sikai_flg) = EM05_EF(Sikai_flg) & ~1;
        EM05_EF(Sikai_flg) = EM05_EF(Sikai_flg) | (ScratchWord[7] & 1);
    }
    ScratchWord[6] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x100);  // Goto00_ck
    ScratchWord[5] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200);  // Goto00_ck
    Em05_mv_tbl[G.pEm->Routine_2]();
}

VOID Em05_mv00(VOID)
{
    Em05_mv00_mot();
}

VOID Em05_mv01(VOID)
{
    G.pEm->Move_no = 1;
    G.pEm->Spd_cur = ((Api_rand() & 1) << 4) + 0x32;  // rand
    Em05_seq_step();
    FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
    G.pEm->Pos_y = G.pEm->Pos_y - 0x1E;
    if (G.pEm->Move_cnt >= 9U)
    {
        EM05_EF(vert_speed) = G.pEm->Move_cnt + 0x34;
        G.pEm->Pos_y = G.pEm->Pos_y + EM05_EF(vert_speed);
    }
    G.pEm->Cdir_z = G.pEm->Cdir_z + 0x15;
    if (G.pEm->Routine_1 != 0 && G.pEm->Pos_y < -0x5DB)
    {
        return;
    }
    G.pEm->Cdir_z = 0x100U;
    EM_ROUTINE_WORD = ROUTINES(0, 5, 1, 1);  // state 1, Key_rno 1, Routine_2 5, Act_sts 0
    if (G.pEm->Pos_y >= -0x31F)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 7, 1, 1);  // state 1, Key_rno 1, Routine_2 7, Act_sts 0
    }
}

VOID Em05_mv02(VOID)
{
    G.pEm->Move_no = 4;
    Em05_seq_step();
    G.pEm->Spd_cur = 0xC8 - G.pEm->Move_cnt * 6;
    if (G.pEm->Spd_cur > 0)
    {
        FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
    }
    G.pEm->Pos_y = G.pEm->Pos_y + -EM05_EF(Down_lim) / 40;
    G.pEm->Cdir_z = G.pEm->Cdir_z - 6;
    if (G.pEm->Routine_1 != 0 && G.pEm->Pos_y < -0x1C1)
    {
        return;
    }
    G.pEm->Spd_cur = 0;
    G.pEm->Cdir_z = 0U;
    G.pEm->Pos_y = -0x1C2;
    EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);  // state 1, Key_rno 1, Routine_2 3, Act_sts 0
    if (ScratchWord[5] == 0)
    {
        EM_ROUTINE_HALF2 = 4;
    }
    if (G.Player.Life < 0)
    {
        EM_ROUTINE_HALF2 = 0;
    }
}

VOID Em05_mv03(VOID)
{
    G.pEm->Move_no = 3;
    EM05_EF(bank_turn_rate) = (((Api_rand()) & 1) << 5) + 0x80 - ((((UINT)Api_rand()) & 1) << 6);  // rand
    Em05_seq_turn();
    if (G.pEm->Routine_1 == 0)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);  // state 1, Key_rno 1, Routine_2 3, Act_sts 0
        if (ScratchWord[5] == 0)
        {
            EM_ROUTINE_HALF2 = 4;
        }
        if (G.Player.Life < 0)
        {
            EM_ROUTINE_HALF2 = 0;
        }
    }
}

VOID Em05_mv04(VOID)
{
    G.pEm->Move_no = 7;
    Em05_seq_step();
    G.pEm->Cdir_z = G.pEm->Cdir_z - 0x19;
    if (G.pEm->Move_cnt >= 9U)
    {
        EM05_EF(vert_speed) = (((EM05_EF(player_dist) / 100) & 0xFFFF) + 0xC8) * 0xA28 / EM05_EF(player_dist);
        if (EM05_EF(vert_speed) >= 0x1F5)
        {
            EM05_EF(vert_speed) = 0xC8;
        }
        G.pEm->Spd_cur = EM05_EF(player_dist) / 100 + 0xC8;
        G.pEm->Cdir_z = -0x100;
        EM_ROUTINE_WORD = ROUTINES(0, 6, 1, 1);  // state 1, Key_rno 1, Routine_2 6, Act_sts 0
        FUNC(SND_SE_ENEM)(4, 0);  // Snd_se_enem
    }
}

VOID Em05_mv05(VOID)
{
    G.pEm->Move_no = 0;

    G.pEm->Cdir_y = G.pEm->Cdir_y + (EM05_EF(Sikai_flg) & 1) * FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x10) * 2;  // Goto00_ck

    if (EM05_EF(player_dist) < 0x7D0U && !((G.pEm->Move_cnt % 9) & 0xFF))
    {
        FUNC(SND_SE_ENEM)(5, 0);  // Snd_se_enem
    }
    Em05_seq_hold();
    FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
    EM05_EF(vert_speed) = EM05_EF(vert_speed) + 1;
    if (EM05_EF(vert_speed) >= 0x1F5)
    {
        EM05_EF(vert_speed) = 0xC8U;
    }
    G.pEm->Pos_y = G.pEm->Pos_y + EM05_EF(vert_speed);
    EM05_EF(Down_lim) = EM05_EF(Lim_ofs) * 2 - ((((UINT)Api_rand()) & 1) * 0x3E8 + 0x5DC);  // rand
    if (G.pEm->Pos_y <= EM05_EF(Down_lim) && G.pEm->Pos_y < -0x5DB)
    {
    }
    else
    {
        EM_ROUTINE_WORD = ROUTINES(0, 7, 1, 1);  // state 1, Key_rno 1, Routine_2 7, Act_sts 0
        if (ScratchWord[5] != 0 && !(G.pEm->Type & 1))
        {
            EM_ROUTINE_WORD = ROUTINES(0, 2, 1, 1);  // state 1, Key_rno 1, Routine_2 2, Act_sts 0
        }
    }
    if (EM05_EF(player_dist) < 0x4B0U)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x100) == 0 && -G.pEm->Pos_y - 0x9C4 < 0x12CU)  // Goto00_ck
        {
            EM_ROUTINE_WORD = ROUTINES(0, 9, 1, 1);  // state 1, Key_rno 1, Routine_2 9, Act_sts 0
        }
    }
}

VOID Em05_mv06(VOID)
{
    G.pEm->Move_no = 5;

    G.pEm->Cdir_y = G.pEm->Cdir_y + (EM05_EF(Sikai_flg) & 1) * FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x10) * 2;  // Goto00_ck

    if (EM05_EF(player_dist) < 0x7D0U && !((G.pEm->Move_cnt % 9) & 0xFF))
    {
        FUNC(SND_SE_ENEM)(5, 0);  // Snd_se_enem
    }
    Em05_seq_hold();
    FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
    EM05_EF(vert_speed) = EM05_EF(vert_speed) + 1;
    if (EM05_EF(vert_speed) >= 0x1F5)
    {
        EM05_EF(vert_speed) = 0xC8U;
    }
    G.pEm->Pos_y = G.pEm->Pos_y - EM05_EF(vert_speed);
    EM05_EF(Up_lim) = -((Api_rand() & 1) * 0x3E8 + 0x1194) + EM05_EF(Lim_ofs);  // rand
    if (G.pEm->Pos_y < EM05_EF(Up_lim))
    {
        EM_ROUTINE_WORD = ROUTINES(0, 0xA, 1, 1);  // state 1, Key_rno 1, Routine_2 0xA, Act_sts 0
    }
    if (EM05_EF(player_dist) < 0x4B0U)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x100) == 0 && -G.pEm->Pos_y - 0x9C4 < 0x12CU)  // Goto00_ck
        {
            EM_ROUTINE_WORD = ROUTINES(0, 9, 1, 1);  // state 1, Key_rno 1, Routine_2 9, Act_sts 0
        }
    }
}

VOID Em05_mv07(VOID)
{
    G.pEm->Move_no = 7;
    Em05_seq_step();
    G.pEm->Spd_cur = EM05_EF(player_dist) / 100 + 0xC8 - (Api_rand() & 1) * 0x32;  // rand
    FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
    G.pEm->Cdir_z = G.pEm->Cdir_z - 0x19;

    if (G.pEm->Move_cnt < 0x13U && !(G.Player.Damage_no & 1))
    {
        return;
    }

    if ((G.Player.Damage_no & 1) && G.Player.Routine_3 == 4)
    {
        G.pEm->Pos_y = G.pEm->Pos_y - 0x32;
        G.pEm->Cdir_y = G.pEm->Cdir_y + (Api_rand() & 1) * 0x800;  // rand
    }
    G.pEm->Cdir_z = -0x100;
    EM_ROUTINE_WORD = ROUTINES(0, 6, 1, 1);  // state 1, Key_rno 1, Routine_2 6, Act_sts 0

    if (G.pEm->Type & 1)
    {
        EM05_EF(vert_speed) = (((EM05_EF(player_dist) / 100) & 0xFFFF) + 0xC8) * 0xA28 / EM05_EF(player_dist);
        if (EM05_EF(vert_speed) >= 0x1F5)
        {
            EM05_EF(vert_speed) = 0xC8;
        }
    }
}

VOID Em05_mv0a(VOID)
{
    G.pEm->Move_no = 5;
    Em05_seq_step();
    G.pEm->Pos_y = G.pEm->Pos_y - 0x19;
    if (G.pEm->Cdir_z > 0)
    {
        G.pEm->Pos_y = G.pEm->Pos_y + 0x32;
    }
    FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
    G.pEm->Cdir_z = G.pEm->Cdir_z + 0x19;
    if (G.pEm->Routine_1 == 0)
    {
        G.pEm->Cdir_z = 0x100;
        EM_ROUTINE_WORD = ROUTINES(0, 5, 1, 1);  // state 1, Key_rno 1, Routine_2 5, Act_sts 0
    }
}

VOID Em05_mv08(VOID)
{
    G.pEm->Move_no = 5;
    FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x40);  // Goto00
    Em05_seq_hold();
    FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
    if (G.pEm->Move_cnt >= 9U)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 6, 1, 1);  // state 1, Key_rno 1, Routine_2 6, Act_sts 0
    }
}

VOID Em05_mv09(VOID)
{
    G.pEm->Move_no = 8;
    FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x30);  // Goto00
    Em05_seq_step();

    if ((G.Player.Damage_no & 1) && G.Player.Routine_3 >= 4U)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 6, 1, 1);  // state 1, Key_rno 1, Routine_2 6, Act_sts 0
        G.pEm->Spd_cur = EM05_EF(player_dist) / 100 + 0xC8 - (Api_rand() & 1) * 0x32;  // rand
        G.pEm->Cdir_z = -0x100;
        G.pEm->Cdir_y = G.pEm->Cdir_y + (Api_rand() & 1) * 0x800;  // rand
        G.pEm->Pos_y = G.pEm->Pos_y - 0x64;
    }
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Cdir_z = 0x100;
        EM_ROUTINE_WORD = ROUTINES(0, 7, 1, 1);  // state 1, Key_rno 1, Routine_2 7, Act_sts 0
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

        FUNC(ESP_CALL)(0x1C, 0, G.pEm->Cdir_y, G.pEm->M, &ScratchPad->Vec, 0);  // Esp_call

        if (EM05_EF(player_dist) < 0x5DCU)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x100) == 0 && (EM05_EF(Sikai_flg) & 1) != 0)  // Goto00_ck
            {
                EM_ROUTINE_HALF2 = 9;
                if (G.Player.Damage_no == 0)
                {
                    G.Player.Life -= 0x10;

                    G.Player.Damage_no = 1;
                    PL_ROUTINE_WORD = ROUTINES(0, 0, 5, 6);  // state 6, Key_rno 5, Routine_2 0, Act_sts 0
                    FUNC(SND_SE_STAD)(3, 0, 0, &MAIN.Player.Pos_x);  // Snd_se_stad
                    ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
                    ScratchPad->Vec.vy = -0x9D8;

                    FUNC(ESP_CALL)(0, 1, (SHORT)G.Player.Cdir_y, &MAIN.Player.M[0], &ScratchPad->Vec, 0);  // Esp_call

                    G.pEm->Damage_no = 1;
                    G.pEm->Cdir_z = 0;
                    EM_ROUTINE_HALF2 = 0xB;
                }
            }
        }
    }
}

VOID Em05_mv0b(VOID)
{
    G.pEm->Move_no = 8;
    FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x80);  // Goto00
    G.pEm->Spd_cur = 0x32;
    Em05_seq_grab();
    FUNC(ADD_SPEED_XZ)((Api_rand() & 1) * 0x800 + 0x400);  // Add_speedXZ, rand
}

VOID Em05_mv0c(VOID)
{
    G.pEm->Move_no = 0;
    FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x80);  // Goto00
    Em05_seq_hold();
    G.pEm->Spd_cur = 0xC8;
    FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
    EM05_EF(vert_speed) = EM05_EF(vert_speed) + 1;
    G.pEm->Pos_y = G.pEm->Pos_y + EM05_EF(vert_speed);
    if (G.pEm->Pos_y >= -0xBB7)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 2, 1, 1);  // state 1, Key_rno 1, Routine_2 2, Act_sts 0
    }
}

VOID Em05_mv0d(VOID)
{
    Em05_mv0d_mot();
}

VOID Em05_seq_hold(VOID)
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
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    }
}

VOID Em05_seq_step(VOID)
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
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    case 2:
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = 0;
    }
}

VOID Em05_seq_turn(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Timer2 = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Routine_3 = 1U;
        G.pEm->Timer0 = (Api_rand() & 0x1F) * 2 + 0x50;  // rand

    case 1:
        ScratchWord[7] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, EM05_EF(bank_turn_rate));  // Goto00_ck
        if ((G.pEm->Timer0-- == 0) || ScratchWord[7] == 0)
        {
            G.pEm->Routine_3 = 2U;
        }
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        G.pEm->Cdir_y = G.pEm->Cdir_y + ScratchWord[7];
        return;
    case 2:
        EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);  // state 1, Key_rno 0, Routine_2 0, Act_sts 0
        G.pEm->Dest_x = G.Player.Pos_x;
        G.pEm->Dest_z = G.Player.Pos_z;
        return;
    }
}

VOID Em05_seq_grab(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Timer2 = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Routine_3 = 1U;
        EM05_EF(Catch_cnt) = 0x64;

        FUNC(PAD_VIB_SET0)(8, 0);  // Pad_vib_set0
        FUNC(PAD_VIB_SET1)(0xA, 0x96, 4);  // Pad_vib_set1

    case 1:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        if (G.Player.Life < 0)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 0xC, 1, 1);  // state 1, Key_rno 1, Routine_2 0xC, Act_sts 0
            PL_ROUTINE_HALF0 = 3;
            PL_ROUTINE_HALF2 = 0xC8;
            return;
        }

        EM05_EF(Catch_cnt) -= FUNC(GACHA_CH)() != 0 ? 9 : 1;  // Gacha_ch

        if ((EM05_EF(Catch_cnt) < 0 && G.Player.Routine_3 == 3) || (EM05_EF(player_dist) >= 0x5DDU && FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x100) != 0))  // Goto00_ck
        {
            G.pEm->Routine_3 = 2U;
            G.Player.Routine_3 = 4;
            return;
        }
        if (!((G.Player.Move_cnt % 20) & 0xFF))
        {
            ScratchPad->Vec.vx = 0x64;
            ScratchPad->Vec.vy = -0x974;
            ScratchPad->Vec.vz = 0;

            FUNC(ESP_CALL)(0, 1, 0x200, &MAIN.Player.M[0], &ScratchPad->Vec, 0);  // Esp_call

            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

            FUNC(ESP_CALL)(0x1C, 0, G.pEm->Cdir_y, G.pEm->M, &ScratchPad->Vec, 0);  // Esp_call

            G.Player.Life = G.Player.Life - 4;

            FUNC(SND_SE_ENEM)(3, 0);  // Snd_se_enem

            FUNC(PAD_VIB_SET0)(3, 0);  // Pad_vib_set0
            FUNC(PAD_VIB_SET1)(0xF, 0x96, 4);  // Pad_vib_set1
            FUNC(PAD_VIB_SET0)(3, 0xF);  // Pad_vib_set0
        }
        return;
    case 2:
        G.pEm->Spd_cur = EM05_EF(player_dist) / 100 + 0xC8 - (Api_rand() & 1) * 0x32;  // rand
        G.pEm->Cdir_z = -0x100;
        G.pEm->Cdir_y = G.pEm->Cdir_y + (Api_rand() & 1) * 0x800;  // rand
        G.pEm->Pos_y = G.pEm->Pos_y - 0x64;
        EM_ROUTINE_WORD = ROUTINES(0, 6, 1, 1);  // state 1, Key_rno 1, Routine_2 6, Act_sts 0
        G.pEm->Damage_no = 0;
    }
}

VOID Em05_mv00_mot(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Timer2 = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Routine_3 = 1;
        G.pEm->Timer0 = (Api_rand() & 0x1F) * 2 + 0x28;  // rand

    case 1:
        if ((G.pEm->Timer0-- != 0) && G.Player.Life >= 0)
        {
        }
        else
        {
            G.pEm->Routine_3 = 2;
        }
        return;
    case 2:
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Timer2 = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Routine_3 = 3;
        G.pEm->Move_no = 0xB;
        if (Api_rand() & 1)  // rand
        {
            G.pEm->Move_no = 0xC;
            if (Api_rand() & 1)  // rand
            {
                G.pEm->Move_no = 2;
                FUNC(SND_SE_ENEM)(1, 0);  // Snd_se_enem
            }
        }

    case 3:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    case 4:
        EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);  // state 1, Key_rno 0, Routine_2 0, Act_sts 0

    default:;
    }
}

VOID Em05_mv0d_mot(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Timer2 = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Routine_3 = 1U;
        G.pEm->Timer0 = ((Api_rand()) & 1) * 3 + (Api_rand() & 1) * 8;  // rand
        G.pEm->Routine_1 = 1;
        G.pEm->Type = G.pEm->Type | 0x10;

    case 1:
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 2U;
        }
        break;
    case 2:
        EM_ROUTINE_WORD = ROUTINES(0, 1, 1, 1);  // state 1, Key_rno 1, Routine_2 1, Act_sts 0
        FUNC(SND_SE_ENEM)(4, 0);  // Snd_se_enem
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

        FUNC(ESP_CALL)(0x1C, 0, G.pEm->Cdir_y, G.pEm->M, &ScratchPad->Vec, 0);  // Esp_call
        FUNC(ESP_CALL)(0x1C, 0, G.pEm->Cdir_y + 0x400, G.pEm->M, &ScratchPad->Vec, 0);  // Esp_call
    }
}

VOID Pl_atck_multi(VOID)
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

    case 1:
        G.Player.Routine_3 += FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);  // Joint_move
        break;
    case 2:

        G.Player.Move_no = 1;

        G.Player.Routine_3 = 3;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;

    case 3:
        FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);  // Joint_move
        break;
    case 4:

        G.Player.Move_no = 2;

        G.Player.Routine_3 = 5;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;

    case 5:
        if (FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400) != 0)  // Joint_move
        {
            PL_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);  // state 1, Key_rno 0, Routine_2 0, Act_sts 0
            G.Player.Damage_no = 0;
        }
        break;
    }
    if (G.Player.Move_cnt > 0xA - (G.Player.Id & 1) * 4)
    {
        FUNC(FOOT_SET_PL)(1);  // Foot_set_pl
        return;
    }
    FUNC(FOOT_SET_PL)(0);  // Foot_set_pl
}

VOID Em05_Scenario(VOID)
{
    if (G.pEm->Type & 0x80)
    {
        return;
    }
    switch (G.pEm->Routine_2)
    {
    case 2:
    case 0:
        Em05_sce00();
        break;
    case 1:
        EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 0);  // state 0, Key_rno 0, Routine_2 0, Act_sts 0
        G.pEm->Type = 0x11U;
        break;
    }
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
}

VOID Em05_sce00(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_no = 5;
        G.pEm->Spd_cur = 0xC8U;
        G.pEm->Routine_3++;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        FUNC(SND_SE_ENEM)(4, 0);  // Snd_se_enem

    case 1:
        if (!((G.pEm->Move_cnt % 9) & 0xFF))
        {
            if (Api_rand() & 1)  // rand
            {
                if (Api_rand() & 1)  // rand
                {
                    FUNC(SND_SE_ENEM)(5, 0);  // Snd_se_enem
                }
            }
        }
        ScratchPad->Vec.vx = G.pEm->Root_x;
        ScratchPad->Vec.vz = G.pEm->Root_z;
        ScratchPad->Vec.vy = 0;
        FUNC(GOTO00)(&ScratchPad->Vec, 0x40);  // Goto00
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200U);  // Joint_move
        G.pEm->Spd_cur = G.pEm->Spd_cur + 1;
        if (G.pEm->Spd_cur >= 0x191)
        {
            G.pEm->Spd_cur = 0xC8U;
        }
        FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ

        if (G.pEm->Pos_y >= -0x5DB)
        {
            G.pEm->Pos_y = G.pEm->Pos_y - 0x1F4;
        }

        if (FUNC(SQUARE_ROOT0)((G.pEm->Pos_x - G.pEm->Root_x) * (G.pEm->Pos_x - G.pEm->Root_x) + (G.pEm->Pos_z - G.pEm->Root_z) * (G.pEm->Pos_z - G.pEm->Root_z)) < 0x96U)  // SquareRoot0
        {
            G.pEm->Routine_3++;
        }
        break;
    case 2:
        EM_ROUTINE_HALF2 = 0;
        (G.pEm->Move_cnt / 9);
    }
}

// collision info of the crow: flags 0x8000, no offset, half height 0xB4, radius 0xC8
AT_TBL Em05_sca =
{
    0x8000,
    0,
    0,
    0,
    0x00B4,
    0x00C8,
};

AT_TBL* pEm05_sca = &Em05_sca;  // collision info pointer read by Em05_Init

// state routine by state number: 0 init, 1 run, 2 recoil, 3 death, 4-8 script driven
VOID (*Em05_routine_0[9])(VOID) =
{
    Em05_Init,     // 0 //
    Em05_Move,      // 1 //
    Em05_Damage,   // 2 //
    Em05_Die,    // 3 //
    Em05_Scenario,      // 4 //
    Em05_Scenario,      // 5 //
    Em05_Scenario,      // 6 //
    Em05_Scenario,      // 7 //
    Em05_Scenario,      // 8 //
};

// behavior routine by Routine_2 number
VOID (*Em05_mv_tbl[14])(VOID) =
{
    Em05_mv00,       // 0 //
    Em05_mv01,          // 1 //
    Em05_mv02,              // 2 //
    Em05_mv03,         // 3 //
    Em05_mv04,        // 4 //
    Em05_mv05,           // 5 //
    Em05_mv06,            // 6 //
    Em05_mv07,       // 7 //
    Em05_mv08,          // 8 //
    Em05_mv09,              // 9 //
    Em05_mv0a,         // 10 //
    Em05_mv0b,              // 11 //
    Em05_mv0c,   // 12 //
    Em05_mv0d,     // 13 //
};
