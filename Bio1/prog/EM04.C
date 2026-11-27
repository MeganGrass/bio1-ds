/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM04.C
 */

#include "EM04.H"
#include "SCRATCHPAD.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Em04_Init(VOID)
{
    EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
    G.pEm->Coord_flg = 0;
    G.pEm->Timer0 = 0;
    G.pEm->Damage_no = 0;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0;
    G.pEm->Move_no = 0;
    FUNC(INIT_PARTS_WORK)();  // Init_Parts_work
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vx = 0;
    ScratchWord[7] = 0x404040;
    FUNC(KAGE_WORK_INIT)(&ScratchPad->Sv, &G.pEm->Kage_work, 0x9C4, 0x9C4);  // Kage_work_init
    Api_rand();  // rand
    G.pEm->Life = 0xCC;
    G.pEm->pAt_tbl = pEm04_sca;
    G.pEm->Scale = 0x1B33;
    EM04_EF(attack_cooldown) = 0;
    EM04_EF(Die_mv) = 0;
    EM04_EF(wall_stuck_frames) = 0;
    EM04_EF(Unk_17a) = 0;
    EM04_EF(Wound_cnt) = 0;
    EM04_EF(Pick_timer) = 0x2D;
}

VOID Em04_Main(VOID)
{
    USHORT unused;  // unused local
    PARTS_W* pP_ptr;

    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        Em04_routine_0[G.pEm->Routine_0]();
        FUNC(OBA_SET_OFS)(G.pEm);  // Oba_set_ofs
        EM04_EF(Pl_hit) = FUNC(OBA_CK_HIT)(&MAIN.Player.Be_flg, G.pEm);  // Oba_ck_hit
        FUNC(OBA_CK_EM)();  // Oba_ck_em
        EM04_EF(room_hit) = FUNC(SCA_CK_HIT)(&G.pEm->Pos_x, G.pEm->pAt_tbl->At_w);  // Sca_ck_hit
    }
    G.pEm->Zone_in_flg = FUNC(HIT_CK_POINT4)(&G.pEm->Pos_x, G.Cccut);  // Hit_ck_point4
    pP_ptr = G.pEm->pSin_parts_ptr;
    if (G.pEm->Zone_in_flg != 0)
    {
        FUNC(MUL_KAGE)(&G.pEm->Pos_x, &G.pEm->Kage_work, 0U, G.pEm->Cdir_y);  // MulKage
    }
}

VOID Em04_Move(VOID)
{
    switch (G.pEm->Routine_1)
    {
    case 0:
        Em04_Move_pick();

    case 1:
        Em04_Move_proc();
    }
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    G.pEm->Be_flg = G.pEm->Be_flg | 0x40;
    FUNC(EM_CK_HANI)(0xFA0);  // Em_ck_hani
    if (EM04_EF(attack_cooldown) != 0)
    {
        EM04_EF(attack_cooldown)--;
    }
    if (EM04_EF(room_hit) != 0)
    {
        EM04_EF(wall_stuck_frames)++;
        return;
    }
    EM04_EF(wall_stuck_frames) = 0U;
}

VOID Em04_Damage(VOID)
{
    if (G.pEm->Routine_2 != 0)
    {
        return;
    }
    G.pEm->Move_no = 1;
    Em04_dm00();
    return;
}

VOID Em04_Die(VOID)
{
    if (G.pEm->Routine_1 == 0)
    {
        G.pEm->Routine_2 = G.pEm->Damage_no >> 3;
        G.pEm->Routine_1 = 1;
        if (EM04_EF(Die_mv) != 0)
        {
            G.pEm->Routine_3 = 3;
            G.pEm->Routine_2 = EM04_EF(Die_mv);
        }
    }
    switch (G.pEm->Routine_2)
    {
    case 1:
    case 2:
        EM04_EF(Die_mv) = 1;
        Em04_Die_mv01();
        break;
    case 6:
    case 8:
    case 9:
        EM04_EF(Die_mv) = 6;
        Em04_Die_mv06();
        break;
    default: LABEL_END
        G.pEm->Move_no = 1;
        Em04_Die_mv00();
    }
}

VOID Em04_Move_proc(VOID)
{
    ScratchWord[7] = FUNC(EM_SIKAI_PROC)();  // Em_sikai_proc
    if ((ScratchWord[7] >> 1) == 0)
    {
        EM04_EF(Sikai_flg) = EM04_EF(Sikai_flg) & ~1;
        EM04_EF(Sikai_flg) = (EM04_EF(Sikai_flg)) | (ScratchWord[7] & 1);
    }
    switch (G.pEm->Routine_2)
    {
    case 0:
        Em04_mv00();
        return;
    case 1:
        G.pEm->Move_no = 3;
        G.pEm->Cdir_y = G.pEm->Cdir_y + (EM04_EF(Sikai_flg) & 1) * FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x10);  // Goto00_ck
        EM04_EF(Unk_17a) = 6;
        Em04_mv01();
        return;
    case 2:
        Em04_mv02();
        return;
    case 3:
        G.pEm->Move_no = 2;
        EM04_EF(turn_step) = 0x80;
        Em04_mv03();
        return;
    case 10:
        Em04_mv0a();
        return;
    case 12:
        Em04_mv0c();
        return;
    case 13:
        Em04_mv0d();
        break;
    }
}

VOID Em04_Move_pick(VOID)
{
    SHORT saved_state;

    ScratchWord[0] = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);

    saved_state = G.pEm->Routine_3;

    if (EM04_EF(Pick_timer) != 0)
    {
        if (ScratchWord[0] >= 0x7D1)
        {
            G.pEm->Routine_1 = 0;
            G.pEm->Routine_2 = 1;
            G.pEm->Routine_3 = saved_state;
        }

        EM04_EF(Pick_timer)--;
        return;
    }

    if (EM04_EF(wall_stuck_frames) >= 0x15 || (EM04_EF(Sikai_flg) & 1) == 0)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = (Api_rand() & 1) + 2;  // rand
        EM04_EF(wall_stuck_frames) = 0;
    }
    else
    {
        if (ScratchWord[0] < 0x1388)
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_HALF2 = 0xC;
        }
        if (EM04_EF(attack_cooldown) == 0)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200) == 0)  // Goto00_ck
            {
                G.pEm->Routine_1 = 1;
                EM_ROUTINE_HALF2 = 0xD;
            }
        }
        if (ScratchWord[0] >= 0x2329)
        {
            G.pEm->Routine_1 = 0;
            G.pEm->Routine_2 = 1;
            G.pEm->Routine_3 = saved_state;
        }
    }
}

VOID Em04_mv00(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0;
        G.pEm->Hokan_flg = 3;

    case 1:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    }
}

VOID Em04_mv01(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Timer0 = (Api_rand() & 0x1F) * G.pEm->Routine_2;  // rand
        if (!((G.pEm->Routine_2 >> 1) & 0xFF))
        {
            G.pEm->Timer0 = G.pEm->Timer0 + 0x50;
        }
        G.pEm->pAt_tbl = pEm04_sca_walk;

    case 1:
        if (Em04_step_se_tbl[G.pEm->Move_cnt] != 0)
        {
            FUNC(SND_SE_ENEM)(Em04_step_se_tbl[G.pEm->Move_cnt], 0);  // Snd_se_enem
        }
        FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        if (G.pEm->Move_cnt != 0 && G.pEm->Move_cnt < 9U)
        {
            Em04_Foot_spd_set(1, G.pEm->Scale);
        }
        else
        {
            Em04_Foot_spd_set(0, G.pEm->Scale);
        }
        if (G.pEm->Spd_cur >= 0x51)
        {
            G.pEm->Spd_cur = 0x50;
        }
        FUNC(ADD_SPEED_XZ)(0U);  // Add_speedXZ
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_1 = 0;
            EM_ROUTINE_UHALF2 = 0;
            G.pEm->pAt_tbl = pEm04_sca;
            return;
        }
    }
}

VOID Em04_mv03(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Damage_no = 0;
        G.pEm->Routine_3 = 1U;
        G.pEm->Timer0 = (Api_rand() & 0x1F) + 0x50;  // rand
        G.pEm->pAt_tbl = pEm04_sca_walk;

    case 1:
        ScratchWord[7] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, EM04_EF(turn_step));  // Goto00_ck
        if (!(G.pEm->Timer0-- != 0) || ScratchWord[7] == 0)
        {
            G.pEm->Routine_3 = 2U;
        }
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        G.pEm->Cdir_y = G.pEm->Cdir_y + ScratchWord[7];
        return;
    case 2:
        G.pEm->Damage_no = 0;
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = 0;
        G.pEm->pAt_tbl = pEm04_sca;
        G.pEm->Dest_x = G.Player.Pos_x;
        G.pEm->Dest_z = G.Player.Pos_z;
        return;
    }
}

VOID Em04_mv02(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 3;
        G.pEm->Timer0 = Api_rand() & 0x3F;  // rand
        G.pEm->Hokan_flg = 3;
        EM04_EF(turn_step) = 0x18U;
        if ((G.pEm->Timer0 & 1) != 0)
        {
            EM04_EF(turn_step) = -0x18;
        }
        G.pEm->pAt_tbl = pEm04_sca_walk;

    case 1:
        G.pEm->Cdir_y = G.pEm->Cdir_y + (EM04_EF(turn_step));
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        if ((EM04_EF(Sikai_flg) & 1) != 0)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, EM04_EF(turn_step)) == 0 && EM04_EF(attack_cooldown) == 0)  // Goto00_ck
            {
                G.pEm->Routine_1 = 1;
                EM_ROUTINE_HALF2 = 0xD;
                return;
            }
        }
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_1 = 0;
            EM_ROUTINE_HALF2 = 0;
            G.pEm->pAt_tbl = pEm04_sca;
        }
    }
}

VOID Em04_mv_lunge(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 8;

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        G.pEm->Spd_cur = 0x64;
        FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
        return;
    case 2:
        G.pEm->Routine_3 = 3;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 0xB;
        G.pEm->Timer0 = (Api_rand() & 0x1F) + 0x14;  // rand

    case 3:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        ScratchWord[7] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x40);  // Goto00_ck
        G.pEm->Cdir_y = G.pEm->Cdir_y + ScratchWord[7];
        if (ScratchWord[7] == 0)
        {
            G.pEm->Routine_3 = 4;
        }
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 6;
        }
        return;
    case 4:
        G.pEm->Routine_3 = 5;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 0xB;
        G.pEm->Timer0 = (Api_rand() & 0x1F) + 0xA;  // rand

    case 5:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x10);  // Goto00_ck
        G.pEm->Spd_cur = 0x12C;
        FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
        if ((G.pEm->Timer0-- != 0) && FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200) == 0)  // Goto00_ck
        {
        }
        else
        {
            G.pEm->Routine_3 = 6;
        }
        ScratchWord[0] = (__builtin_abs(G.Player.Pos_x - G.pEm->Pos_x)) + (__builtin_abs(G.Player.Pos_z - G.pEm->Pos_z));
        if (EM04_EF(Pl_hit) != 0 && G.Player.Damage_no == 0)
        {
            ScratchWord[7] = FUNC(CDIR_CK)(&MAIN.Player.Be_flg);  // Cdir_ck
            G.pEm->Routine_3 = 6;

            G.Player.Life -= 0x14;

            if (G.Player.Life > 0)
            {
                G.Player.Damage_no = ScratchWord[7] + 1;
                G.Player.Routine_2 = ScratchWord[7] + 0x66;
            }

            FUNC(PAD_VIB_SET0)(8, 0);  // Pad_vib_set0
            FUNC(PAD_VIB_SET1)(0xA, 0xFA, 0);  // Pad_vib_set1
        }
        return;
    case 6:
        G.pEm->Routine_3 = 7;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 9;

    case 7:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        G.pEm->Spd_cur = 0x64;
        FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
        return;
    case 8:
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = 0;
        EM04_EF(attack_cooldown) = 0x1E;
    }
}

VOID Em04_mv_spit(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0xC;
        ScratchPad->Vec.vx = 0x3E8;
        ScratchPad->Vec.vy = -0x258;
        ScratchPad->Vec.vz = 0;

        FUNC(ESP_CALL)(0x1E, 0, 0, G.pEm->M, &ScratchPad->Vec, 0);  // Esp_call

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    case 2:
        EM_ROUTINE_HALF0 = 1;
        EM_ROUTINE_HALF2 = 0xB;
        EM04_EF(attack_cooldown) = 0x1E;
    }
}

VOID Em04_mv_drop(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 0;
        G.pEm->Timer0 = 0;

    case 1:
        G.pEm->Pos_y = G.pEm->Pos_y + ((G.pEm->Timer0 * G.pEm->Timer0++) << 3);
        if (!(G.pEm->Pos_y & 0x80000000))
        {
            G.pEm->Pos_y = 0;
            G.pEm->Routine_3 = 2U;
        }
        G.pEm->Cdir_x = G.pEm->Cdir_x - 0x100;  // sign-extends the halfword
        if (G.pEm->Cdir_x & 0x80000000)  // sign-extends the halfword
        {
            G.pEm->Cdir_x = 0U;
            G.pEm->Type = G.pEm->Type & 0xFD;
        }
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    case 2:
        G.pEm->Routine_3 = 3U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 7;

    case 3:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    case 4:
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = 0;

    default: LABEL_END
    }
}

VOID Em04_mv_nop(VOID)
{
}

VOID Em04_mv0a(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_no = 6;
        EM04_EF(turn_step) = 0x32C - (Api_rand() & 1) * 0x658;  // rand

        EM04_EF(turn_step) += (Api_rand() & 0xFFF) * !(EM04_EF(Sikai_flg) & 1);  // rand

        G.pEm->Spd_cur = (Api_rand() & 0x3F) + 0xFA;  // rand
        FUNC(ADD_SPEED_XZ)(EM04_EF(turn_step));  // Add_speedXZ

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x40);  // Goto00_ck
        G.pEm->Pos_x = G.pEm->Pos_x + G.pEm->Spd_x;
        G.pEm->Pos_y = G.pEm->Pos_y + G.pEm->Spd_y;
        G.pEm->Pos_z = G.pEm->Pos_z + G.pEm->Spd_z;
        break;
    case 2:
        G.pEm->Damage_no = 0;
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = ((EM04_EF(Sikai_flg)) & 1) * 4;
        EM04_EF(wall_stuck_frames) = 0;
    }
}

VOID Em04_mv0c(VOID)
{
    PARTS_W* pP_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 8;

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    case 2:
        G.pEm->Routine_3 = 3;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 0xB;
        G.pEm->Timer0 = (Api_rand() & 0x1F) + 0x14;  // rand

    case 3:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        ScratchWord[7] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x40);  // Goto00_ck
        G.pEm->Cdir_y = G.pEm->Cdir_y + ScratchWord[7];
        if (ScratchWord[7] == 0)
        {
            G.pEm->Routine_3 = 4;
        }
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 4;
        }
        return;
    case 4:
        G.pEm->Routine_3 = 5;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 9;

    case 5:
        if (G.pEm->Move_cnt == 0xC && G.Player.Damage_no == 0)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchWord[1] = FUNC(L_POS_CK_XZM)(&pP_ptr[1].Workm, &ScratchPad->Vec, 0x514, &MAIN.Player.Pos_x);  // L_pos_ckXZM
            ScratchWord[2] = FUNC(L_POS_CK_XZM)(&pP_ptr[2].Workm, &ScratchPad->Vec, 0x514, &MAIN.Player.Pos_x);  // L_pos_ckXZM
            if (ScratchWord[1] == 0 && ScratchWord[2] == 0)
            {
            }
            else
            {
                ScratchWord[7] = FUNC(CDIR_CK)(&MAIN.Player.Be_flg);  // Cdir_ck
                FUNC(SND_SE_ENEM)(4, 0);  // Snd_se_enem

                G.Player.Life -= 0x14;

                if (G.Player.Life < 0)
                {
                    G.Player.Life = 1;
                }
                if (G.Player.Life >= 0)
                {
                    G.Player.Damage_no = ScratchWord[7] + 1;
                    G.Player.Routine_2 = ScratchWord[7] + 0x66;
                }
            }
        }
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    case 6:
        FUNC(SND_SE_ENEM)(3, 0);  // Snd_se_enem
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = 0;
        if (G.Player.Life < 0)
        {
            EM_ROUTINE_HALF0 = 0x101;
            EM_ROUTINE_HALF2 = 0xC;
        }

    default: LABEL_END
    }
}

VOID Em04_mv0d(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0xC;
        G.pEm->Timer0 = 0;
        ScratchPad->Vec.vx = 0x3E8;
        ScratchPad->Vec.vy = -0x258;
        ScratchPad->Vec.vz = 0;
        ScratchWord[3] = 0x100 - (Api_rand() & 1) * 0x80 * (Api_rand() & 1);  // rand

        FUNC(ESP_CALL)(0x1E, 0, -ScratchWord[3], G.pEm->M, &ScratchPad->Vec, 0);  // Esp_call
        FUNC(ESP_CALL)(0x1E, 0, 0, G.pEm->M, &ScratchPad->Vec, 0);  // Esp_call
        FUNC(ESP_CALL)(0x1E, 0, ScratchWord[3], G.pEm->M, &ScratchPad->Vec, 0);  // Esp_call

        FUNC(SND_SE_ENEM)(7, 0);  // Snd_se_enem

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    case 2:
        EM_ROUTINE_HALF0 = 0x101;
        EM_ROUTINE_HALF2 = 0xC;
        EM04_EF(attack_cooldown) = 0x14;
    }
}

VOID Em04_dm00(VOID)
{
    PARTS_W* pP_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Spd_cur = 0;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        FUNC(SND_SE_ENEM)(4, 0);  // Snd_se_enem
        if (!(G.pEm->Damage_no & 1))
        {
            ScratchWord[7] = Api_rand() & 7;  // rand
            ScratchWord[6] = 8;
            ScratchWord[5] = 0;
            while ((ScratchWord[6] = ScratchWord[6] - 1) != -1)
            {
                if (Em04_wound_kan_tbl[ScratchWord[7]] == Em04_wound_kan[ScratchWord[6]])
                {
                    ScratchWord[5] = 1;
                }
            }
            if (ScratchWord[5] == 0)
            {
                pP_ptr = G.pEm->pSin_parts_ptr;
                Em04_wound_kan[EM04_EF(Wound_cnt)] = *(Em04_wound_kan_tbl + ScratchWord[7]);
                FUNC(JOINT_ATCK_SET)(&pP_ptr[Em04_wound_kan_tbl[ScratchWord[7]]], 0x1E, 0x14, 3);  // Joint_atck_set
                FUNC(JOINT_ATCK_SET)(&pP_ptr[Em04_wound_kan_tbl[ScratchWord[7]]] + 1, 0x1E, 0x14, 3);  // Joint_atck_set
                ScratchPad->Vec.vz = 0;
                ScratchPad->Vec.vy = 0;
                ScratchPad->Vec.vx = 0;

                FUNC(ESP_CALL)(0, 8, 0, &pP_ptr[Em04_wound_kan_tbl[ScratchWord[7]]].Workm, &ScratchPad->Vec, 0);  // Esp_call
                FUNC(ESP_CALL)(0, 8, 0, &(&pP_ptr[Em04_wound_kan_tbl[ScratchWord[7]]])[1].Workm, &ScratchPad->Vec, 0);  // Esp_call

                FUNC(SND_SE_ENEM)(5, 0);  // Snd_se_enem
                EM04_EF(Wound_cnt)++;
            }
        }

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    case 2:
        EM_ROUTINE_HALF0 = 0x101;
        EM_ROUTINE_HALF2 = 3;
        G.pEm->Dest_x = G.Player.Pos_x;
        G.pEm->Dest_z = G.Player.Pos_z;
        if ((EM04_EF(Sikai_flg) & 1) != 0)
        {
            EM_ROUTINE_HALF0 = 0x101;
            EM_ROUTINE_HALF2 = 0xA;
        }
        EM04_EF(attack_cooldown) = 0;
        return;
    }
}

VOID Em04_Die_mv00(VOID)
{
    PARTS_W* pP_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Spd_cur = 0;
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0;
        pP_ptr = G.pEm->pSin_parts_ptr;
        FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, 0, 0);  // Kage_half_set
        FUNC(JOINT_ATCK_SET)(pP_ptr, 0x1E, 0x14, 3);  // Joint_atck_set
        FUNC(JOINT_ATCK_SET)(&pP_ptr[19], 0x1E, 0x14, 3);  // Joint_atck_set
        ScratchPad->Vec.vz = 0;
        ScratchPad->Vec.vy = 0;
        ScratchPad->Vec.vx = 0;

        FUNC(ESP_CALL)(0, 8, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);  // Esp_call
        FUNC(ESP_CALL)(0, 8, 0, &pP_ptr[19].Workm, &ScratchPad->Vec, 0);  // Esp_call

        FUNC(SND_SE_ENEM)(5, 0);  // Snd_se_enem
        ScratchWord[7] = 9;
        while ((ScratchWord[7] = ScratchWord[7] - 1) != 0)
        {
            if ((&pP_ptr[ScratchWord[7] * 2])[1].Be_flg == 0 || ((&pP_ptr[ScratchWord[7] * 2])[1].Be_flg & 0x20))
            {
                continue;
            }
            (&pP_ptr[ScratchWord[7] * 2])[2].Be_flg = (&pP_ptr[ScratchWord[7] * 2])[2].Be_flg | 0xC;
            (&pP_ptr[ScratchWord[7] * 2])[3].Be_flg = (&pP_ptr[ScratchWord[7] * 2])[3].Be_flg | 0x10;
        }

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    case 2:
        FUNC(KAGE_WORK_COLOR_SET)(&G.pEm->Kage_work, 1, 2, 0xDF809F);  // Kage_work_color_set
        FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, 0x1F4, 0x1F4);  // Kage_half_set
        G.pEm->Routine_3 = 3U;

        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT | EM_BE_NO_OBJ;

        G.pEm->Spd_cur = 0;
        G.pEm->Timer0 = 0x5AU;

        G.pEm->pAt_tbl = pEm04_sca_dead;

        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);  // Flg_on

    case 3:
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, 9, 9);  // Kage_work_size_add

        if (--G.pEm->Timer0 == 0)
        {
            FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);  // Flg_on
            G.pEm->Routine_3 = 4U;
            FUNC(SET_ENEMY_WORK)(0x33, 0xF2, G.pEm->Parts_num, &G.pEm->pTgt);  // Set_enemy_work
        }
        return;
    case 4:
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);  // Flg_on
        FUNC(EM_WEB_PROC)(0x33);  // Em_web_proc
        break;
    }
}

VOID Em04_Die_mv06(VOID)
{
    PARTS_W* pP_ptr;

    if (G.pEm->Routine_3 == 1 || G.pEm->Routine_3 == 3)
    {
        if (G.pEm->Damage_no & 2)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, 0, 0);  // Kage_half_set
            FUNC(JOINT_ATCK_SET)(pP_ptr, 0x1E, 0x1E, 3);  // Joint_atck_set
            FUNC(JOINT_ATCK_SET)(&pP_ptr[19], 0x1E, 0x1E, 3);  // Joint_atck_set
            ScratchPad->Vec.vz = 0;
            ScratchPad->Vec.vy = 0;
            ScratchPad->Vec.vx = 0;

            FUNC(ESP_CALL)(0, 8, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);  // Esp_call
            FUNC(ESP_CALL)(0, 8, 0, &pP_ptr[19].Workm, &ScratchPad->Vec, 0);  // Esp_call

            FUNC(SND_SE_ENEM)(5, 0);  // Snd_se_enem
            ScratchWord[7] = 9;
            while ((ScratchWord[7] = ScratchWord[7] - 1) != 0)
            {
                if ((&pP_ptr[ScratchWord[7] * 2])[1].Be_flg == 0 || ((&pP_ptr[ScratchWord[7] * 2])[1].Be_flg & 0x20))
                {
                    continue;
                }
                (&pP_ptr[ScratchWord[7] * 2])[1].Be_flg = (&pP_ptr[ScratchWord[7] * 2])[1].Be_flg | 0xC;
                (&pP_ptr[ScratchWord[7] * 2])[2].Be_flg = (&pP_ptr[ScratchWord[7] * 2])[2].Be_flg | 0x10;
            }
            G.pEm->Routine_2 = 3;
            G.pEm->Routine_3 = 2U;
            return;
        }
        if (G.pEm->Damage_no & 1)
        {
            G.pEm->Routine_3 = 4U;
        }
    }
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Spd_cur = 0;
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Damage_no = 0U;
        G.pEm->Move_no = 4;
        FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, 0x708, 0x708);  // Kage_half_set

    case 1:
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        if (G.pEm->Move_cnt >= 0xEU)
        {
            G.pEm->Be_flg = G.pEm->Be_flg | 0x20;
        }
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    case 2:
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT | EM_BE_NO_OBJ;

        G.pEm->Move_no = 5;
        G.pEm->Routine_3 = 3U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Damage_no = 0U;
        G.pEm->Timer0 = 0xB4U;
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);  // Flg_on

    case 3:
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        G.pEm->Be_flg = G.pEm->Be_flg | 0x20;
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 4U;
            G.pEm->Damage_no = 0x11U;
        }
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    case 4:
        G.pEm->Move_no = 5;
        G.pEm->Routine_3 = 5U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 1;
        FUNC(SND_SE_ENEM)(5, 0);  // Snd_se_enem
        FUNC(KAGE_WORK_COLOR_SET)(&G.pEm->Kage_work, 1, 2, 0xDF809F);  // Kage_work_color_set
        FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, 0x7D0, 0x7D0);  // Kage_half_set
        G.pEm->Timer0 = 0x64U;
        G.pEm->Spd_cur = 0;

        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT | EM_BE_NO_OBJ;
        G.pEm->pAt_tbl = pEm04_sca_dead;

    case 5:
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, 4, 4);  // Kage_work_size_add
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move

        if (--G.pEm->Timer0 == 0)
        {
            G.pEm->Routine_3 = 6U;
            FUNC(SET_ENEMY_WORK)(((G.pEm->Damage_no >> 3) & 0xFF) * 5 + 1, 0xF2, G.pEm->Parts_num, &G.pEm->pTgt);  // Set_enemy_work
        }
        return;
    case 6:
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);  // Flg_on
        FUNC(EM_WEB_PROC)(((G.pEm->Damage_no >> 3) & 0xFF) * 5 + 1);  // Em_web_proc
        break;
    }
}

VOID Em04_Die_mv01(VOID)
{
    PARTS_W* pP_ptr;

    if (G.pEm->Routine_3 == 1 || G.pEm->Routine_3 == 3)
    {
        if (G.pEm->Damage_no & 2)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, 0, 0);  // Kage_half_set
            FUNC(JOINT_ATCK_SET)(pP_ptr, 0x1E, 0x1E, 3);  // Joint_atck_set
            FUNC(JOINT_ATCK_SET)(&pP_ptr[19], 0x1E, 0x1E, 3);  // Joint_atck_set
            ScratchPad->Vec.vz = 0;
            ScratchPad->Vec.vy = 0;
            ScratchPad->Vec.vx = 0;

            FUNC(ESP_CALL)(0, 8, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);  // Esp_call
            FUNC(ESP_CALL)(0, 8, 0, &pP_ptr[19].Workm, &ScratchPad->Vec, 0);  // Esp_call

            FUNC(SND_SE_ENEM)(5, 0);  // Snd_se_enem
            ScratchWord[7] = 9;

            while (--ScratchWord[7] != 0)
            {
                if ((&pP_ptr[ScratchWord[7] * 2])[1].Be_flg == 0 || ((&pP_ptr[ScratchWord[7] * 2])[1].Be_flg & 0x20))
                {
                    continue;
                }
                (&pP_ptr[ScratchWord[7] * 2])[1].Be_flg = (&pP_ptr[ScratchWord[7] * 2])[1].Be_flg | 0xC;
                (&pP_ptr[ScratchWord[7] * 2])[2].Be_flg = (&pP_ptr[ScratchWord[7] * 2])[2].Be_flg | 0x10;
            }
            G.pEm->Routine_2 = 3;
            G.pEm->Routine_3 = 2U;
            return;
        }
        if (G.pEm->Damage_no & 1)
        {
            G.pEm->Routine_3 = 4U;
        }
    }
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Spd_cur = 0U;
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Damage_no = 0U;
        G.pEm->Move_no = 1;
        pP_ptr = G.pEm->pSin_parts_ptr;
        FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, 0x5DC, 0x5DC);  // Kage_half_set
        if (EM04_EF(Wound_cnt) < 6)
        {
            FUNC(JOINT_ATCK_SET)(&pP_ptr[19], 0x1E, 0x1E, 3);  // Joint_atck_set
        }
        ScratchPad->Vec.vz = 0;
        ScratchPad->Vec.vy = 0;
        ScratchPad->Vec.vx = 0;

        FUNC(ESP_CALL)(0, 8, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);  // Esp_call
        FUNC(ESP_CALL)(0, 8, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);  // Esp_call
        FUNC(ESP_CALL)(0, 0xB, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);  // Esp_call
        FUNC(ESP_CALL)(0, 0xB, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0);  // Esp_call

        FUNC(SND_SE_ENEM)(5, 0);  // Snd_se_enem

    case 1:
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    case 2:
        G.pEm->Move_no = 7;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Timer0 = 0xD2U;

        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT | EM_BE_NO_OBJ;

        G.pEm->Damage_no = 0U;
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);  // Flg_on
        G.pEm->Routine_3 = 3U;

    case 3:
        if ((G.pEm->Timer0-- & 1) != 0)
        {
            FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        }
        if (G.pEm->Move_cnt == 0)
        {
            G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
            G.pEm->Be_flg = G.pEm->Be_flg | 0x20;
            ScratchPad->Vec.vz = 0;
            ScratchPad->Vec.vy = 0;
            ScratchPad->Vec.vx = 0;
            pP_ptr = G.pEm->pSin_parts_ptr;

            FUNC(ESP_CALL)(0, 0xB, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);  // Esp_call
            FUNC(ESP_CALL)(0, 0xB, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0);  // Esp_call
        }
        if (G.pEm->Timer0 == 0)
        {
            G.pEm->Routine_3 = 4U;
            G.pEm->Damage_no = 0x11U;
        }
        return;
    case 4:
        G.pEm->Move_no = 1;
        G.pEm->Routine_3 = 5U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 1;
        FUNC(SND_SE_ENEM)(5, 0);  // Snd_se_enem
        G.pEm->Timer0 = 0x64U;
        G.pEm->Spd_cur = 0x78U;

        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT | EM_BE_NO_OBJ;
        G.pEm->pAt_tbl = pEm04_sca_dead;

        FUNC(SET_ENEMY_WORK)(((G.pEm->Damage_no >> 3) & 0xFF) * 5 + 1, 0xF2, G.pEm->Parts_num, &G.pEm->pTgt);  // Set_enemy_work

    case 5:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        FUNC(EM_WEB_PROC)(((G.pEm->Damage_no >> 3) & 0xFF) * 5 + 1);  // Em_web_proc
        --G.pEm->Spd_cur;
        ScratchWord[7] = Api_rand() & 7;  // rand
        // the statement expression keeps the factor 0x64 a multiply instead of shifts
        FUNC(ADD_SPEED_XZ)(Em04_wound_kan_tbl[ScratchWord[7]] * (((Api_rand() & 1) + 1) * KEEP_VALUE(0x64)));  // Add_speedXZ, rand; KEEP_VALUE keeps the constant out of GCC 2.6.3's folding; the plain form and a local change the code

        if (--G.pEm->Timer0 == 0)
        {
            G.pEm->Routine_3 = 6U;
        }
        return;
    case 6:
        G.pEm->Spd_cur = 0U;
        G.pEm->Routine_3 = 7U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 4;

    case 7:
        FUNC(EM_WEB_PROC)(((G.pEm->Damage_no >> 3) & 0xFF) * 5 + 1);  // Em_web_proc
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)  // Joint_move
        {
            G.pEm->Routine_3 = 8U;
            G.pEm->Timer0 = 0x3CU;
        }
        return;
    case 8:
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);  // Flg_on
        FUNC(EM_WEB_PROC)(((G.pEm->Damage_no >> 3) & 0xFF) * 5 + 1);  // Em_web_proc
        break;
    }
}

VOID Em04_Foot_spd_set(UCHAR Flg, INT scale)
{
    PARTS_W* pP_ptr;
    UCHAR i;
    pP_ptr = G.pEm->pSin_parts_ptr;
    FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);  // RotMatrix
    FUNC(COMP_MATRIX)(G.pEm->M, &pP_ptr->Coord, &ScratchPad->Mat);  // CompMatrix
    if (scale != 0)
    {
        ScratchPad->Vec.vx = ScratchPad->Vec.vy = ScratchPad->Vec.vz = scale;
        FUNC(SCALE_MATRIX)(&ScratchPad->Mat, &ScratchPad->Vec);  // ScaleMatrix
    }
    pP_ptr = &pP_ptr[Flg * 8 + 4];
    if (!(pP_ptr->Be_flg & 1))
    {
        G.pEm->Spd_cur = 0;
        return;
    }
    i = 2;
    while (i != 0)
    {
        FUNC(COMP_MATRIX)(&ScratchPad->Mat, &(i + pP_ptr)[-1].Coord, &ScratchPad->Mat);  // CompMatrix
        i--;
    }
    ScratchPad->Mat.t[0] = ScratchPad->Mat.t[0] - pP_ptr->Workm.t[0];
    ScratchPad->Mat.t[1] = 0;
    ScratchPad->Mat.t[2] = ScratchPad->Mat.t[2] - pP_ptr->Workm.t[2];
    FUNC(SQUARE0)(&ScratchPad->Mat.t, &ScratchPad->Vec);  // Square0
    G.pEm->Spd_cur = FUNC(SQUARE_ROOT0)(ScratchPad->Vec.vx + ScratchPad->Vec.vz);  // SquareRoot0
}

static AT_TBL Em04_sca_tbl00 = { 0x8000, -500, -180, 0, 0xB4, 0x9C4 };  // collision volume record: flags 0x8000, offset -500 / -180 / 0, half height 180, radius 2500
static AT_TBL Em04_sca_tbl01 = { 0x8000, 500, -180, 0, 0xB4, 0x7D0 };  // collision volume record: flags 0x8000, offset 500 / -180 / 0, half height 180, radius 2000
// two collision volume records for the walking tiger: offsets 0 / -180 / 1000 and 0 / -180 / -1000, half height 180, radius 2000
static AT_TBL Em04_sca_tbl02[2] =
{
    { 0, 0, -180, 1000, 0xB4, 0x7D0 },   // 0 //
    { 0x8000, 0, -180, -1000, 0xB4, 0x7D0 },   // 1 //
};

static AT_TBL Em04_sca_tbl03 = { 0x8000, 0, 0, 0, 0, 0x64 };  // collision volume record of the dead tiger: flags 0x8000, no offset, half height 0, radius 100

AT_TBL* pEm04_sca = &Em04_sca_tbl00;  // collision volume of the living tiger
static AT_TBL* Em04_sca_adr01 = &Em04_sca_tbl01;  // address of the second collision volume record; never read
AT_TBL* pEm04_sca_walk = Em04_sca_tbl02;  // collision volume used while walking, approaching and strafing

AT_TBL* pEm04_sca_dead = &Em04_sca_tbl03;  // collision volume of the dead tiger

// state handlers, indexed by the entity state
VOID (*Em04_routine_0[4])(VOID) =
{
    Em04_Init,      // 0 //
    Em04_Move,       // 1 //
    Em04_Damage,   // 2 //
    Em04_Die,     // 3 //
};
// step sound id by animation frame, 0 for none
UCHAR Em04_step_se_tbl[16] =
{
    0x00,   // 0 //
    0x02,   // 1 //
    0x00,   // 2 //
    0x00,   // 3 //
    0x00,   // 4 //
    0x00,   // 5 //
    0x00,   // 6 //
    0x01,   // 7 //
    0x00,   // 8 //
    0x00,   // 9 //
    0x01,   // 10 //
    0x00,   // 11 //
    0x00,   // 12 //
    0x00,   // 13 //
    0x01,   // 14 //
    0x00,   // 15 //
};
// joint that shows a wound, indexed by a random value 0 to 7
UCHAR Em04_wound_kan_tbl[8] =
{
    0x05,   // 0 //
    0x07,   // 1 //
    0x07,   // 2 //
    0x09,   // 3 //
    0x0B,   // 4 //
    0x0B,   // 5 //
    0x0F,   // 6 //
    0x11,   // 7 //
};
UCHAR Em04_wound_kan[8] = { 0 };  // pSin_parts_ptr that already show a wound, 0 for a free slot
