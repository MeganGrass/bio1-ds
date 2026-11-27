/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM0A.C
 */

#include "EM0A.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Em0a_Init(VOID)
{
    EM_ROUTINE_HALF0 = 1;
    EM_ROUTINE_HALF2 = 0;
    G.pEm->Coord_flg = 0;
    G.pEm->Dummy0c1 = 0;
    G.pEm->Timer0 = 0;
    G.pEm->Damage_no = 0;
    G.pEm->Be_flg = EM_BE_ENABLED;
    FUNC(INIT_PARTS_WORK)();  // Init_Parts_work
    ScratchWord[7] = 0x202020;
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vx = 0;
    FUNC(KAGE_WORK_INIT)(&ScratchPad->Sv, &G.pEm->Kage_work, 0, 0);  // Kage_work_init
    Api_rand();  // rand
    G.pEm->Life = (5 + (Api_rand() & 7) + (Api_rand() & 7) + (Api_rand() & 7)) * 2;  // rand
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Move_no = 0;
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);  // Joint_move
    G.pEm->pAt_tbl = pEm0a_sca;
    G.pEm->Scale = 0x3000;
    EM0A_FREE_P->wall_stuck_frames = 0;
    EM0A_FREE_P->Hit_en = 1;
    if (G.pEm->Type != 0)
    {
        G.pEm->Timer0 = 0xF0 - (Api_rand() & 6) * 0xA;  // rand
        G.pEm->Cdir_x = 0;
        G.pEm->Cdir_z = 0x800;
        if (G.pEm->Pos_y == 0)
        {
            G.pEm->Pos_y = -0x1FB8;
        }
    }
    if (G.pEm->Type == 3)
    {
        G.pEm->Cdir_z = 0;
        G.pEm->Pos_y = 0;
        EM0A_FREE_P->Hit_en = 0;
        G.pEm->Damage_no = 1;
    }
    return;
}

VOID Em0a_Main(VOID)
{
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        Em0a_routine_0[G.pEm->Routine_0]();
        if (EM0A_FREE_P->Hit_en != 0)
        {
            FUNC(OBA_SET_OFS)(G.pEm);  // Oba_set_ofs
            EM0A_FREE_P->Pl_hit = FUNC(OBA_CK_HIT)(&MAIN.Player.Be_flg, G.pEm);  // Oba_ck_hit
            FUNC(OBA_CK_EM)();  // Oba_ck_em
            EM0A_FREE_P->room_hit = FUNC(SCA_CK_HIT)(&G.pEm->Pos_x, G.pEm->pAt_tbl->At_w);  // Sca_ck_hit
        }
    }
    G.pEm->Zone_in_flg = FUNC(HIT_CK_POINT4)(&G.pEm->Pos_x, G.Cccut);  // Hit_ck_point4

    if (G.pEm->Type != 0)
    {
        G.pEm->Zone_in_flg = 0U;
    }

    if (G.pEm->Zone_in_flg != 0)
    {
        FUNC(MUL_KAGE)(&G.pEm->Pos_x, &G.pEm->Kage_work, 0U, G.pEm->Cdir_y);  // MulKage
    }
}

VOID Em0a_Move(VOID)
{
    PARTS_W* pP_ptr;

    pP_ptr = G.pEm->pSin_parts_ptr;
    ScratchWord[6] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x100);  // Goto00_ck
    ScratchWord[5] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200);  // Goto00_ck
    switch (G.pEm->Routine_1)
    {
    case 0:
        Em0a_brain();

    case 1:
        Em0a_mv_call();
    }
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    if (G.pEm->Pos_y == 0 && G.Player.Pos_y == 0)
    {
        FUNC(EM_CK_HANI)(0x1388);  // Em_ck_hani
    }
    if (G.pEm->Pos_y >= -0xF9F)
    {
        ScratchWord[3] = (pP_ptr->Workm.t[1] >> 4) + 0x1F4;
        if (ScratchWord[3] < 0)
        {
            ScratchWord[3] = 0x12C;
        }
        FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, ScratchWord[3] + 0x1F4, ScratchWord[3]);  // Kage_half_set
    }
    if (EM0A_FREE_P->room_hit != 0)  // truncates to an unsigned halfword
    {
        EM0A_FREE_P->wall_stuck_frames++;
        return;
    }
    EM0A_FREE_P->wall_stuck_frames = 0;
}

VOID Em0a_Damage(VOID)
{
    INT unused;  // unused local
    PARTS_W* pP_ptr;

    pP_ptr = G.pEm->pSin_parts_ptr;
    switch (G.pEm->Routine_2)
    {
    case 0:
        Em0a_dm00();
        break;
    }
    ScratchWord[3] = (pP_ptr->Workm.t[1] >> 4) + 0x190;
    if (ScratchWord[3] < 0)
    {
        ScratchWord[3] = 0xC8;
    }
    FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, ScratchWord[3] + 0x190, ScratchWord[3]);  // Kage_half_set
}

VOID Em0a_Die(VOID)
{
    INT unused;  // unused local
    switch (G.pEm->Routine_2)
    {
    case 0:
        Em0a_die00();
        return;
    case 3:
        Em0a_die01();
    }
}

VOID Em0a_mv_call(VOID)
{
    ScratchWord[7] = FUNC(EM_SIKAI_PROC)();  // Em_sikai_proc
    if ((ScratchWord[7] >> 1) == 0)
    {
        EM0A_FREE_P->Sikai_flg = EM0A_FREE_P->Sikai_flg & ~1;  // sign-extends the halfword
        EM0A_FREE_P->Sikai_flg = EM0A_FREE_P->Sikai_flg | (ScratchWord[7] & 1);
    }
    Em0a_mv_tbl[G.pEm->Routine_2]();
}

VOID Em0a_brain(VOID)
{
    if (G.Player.Pos_y == 0)
    {
        EM0A_FREE_P->player_distance = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);
    }
    else
    {
        EM0A_FREE_P->player_distance = 0x1387;
    }
    Em0a_brain_tbl[G.pEm->Type]();
}

VOID Em0a_brain00(VOID)
{
    if (EM0A_FREE_P->player_distance < 0x2710U && (EM0A_FREE_P->Sikai_flg & 1) == 0)  // sign-extends the halfword
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 2;
    }
    if ((EM0A_FREE_P->player_distance < 0x7D0U && ScratchWord[6] == 0) || EM0A_FREE_P->Pl_hit != 0)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 3;
        EM0A_FREE_P->Pl_hit = 0;
    }
    else
    {
        if (EM0A_FREE_P->wall_stuck_frames >= 0x3DU)
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_HALF2 = 2;
            EM0A_FREE_P->wall_stuck_frames = 0;
        }
        if (G.Player.Life < 0)
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_HALF2 = 4;
        }
    }
}

VOID Em0a_brain01(VOID)
{
    EM_ROUTINE_HALF2 = 5;
    if (EM0A_FREE_P->player_distance < 0xBB8U)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 1;
    }
}

VOID Em0a_brain02(VOID)
{
    EM_ROUTINE_HALF2 = 5;
    if ((G.Player.Routine_2 == 0x14 && G.Player.Routine_3 == 0 && EM0A_FREE_P->player_distance < 0x1388U) || (G.pEm->Timer0-- == 0))
    {
        G.pEm->Pos_x = G.Player.Pos_x + (Api_rand() & 1) * 0x64;  // rand
        G.pEm->Pos_z = G.Player.Pos_z + (Api_rand() & 1) * 0x64;  // rand
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 1;
    }
}

VOID Em0a_brain03(VOID)
{
    EM_ROUTINE_HALF2 = 5;
    if (EM0A_FREE_P->player_distance < 0x1194U)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 6;
    }
}

VOID Em0a_mv00(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Spd_cur = 0x32 - (Api_rand() & 0xF);  // rand
        G.pEm->Timer0 = (Api_rand() & 0xF) + 0x10;  // rand

    case 1:
        if (EM0A_FREE_P->player_distance < 0x1F40U)
        {
            G.pEm->Cdir_y = G.pEm->Cdir_y + (EM0A_FREE_P->Sikai_flg & 1) * FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, G.pEm->Timer0);  // Goto00_ck
        }
        else
        {
            FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x40);  // Goto00
        }
        G.pEm->Cdir_y = G.pEm->Cdir_y + (((Api_rand()) & 0xF) + (Api_rand() & 0xF));  // rand
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
    }
}

VOID Em0a_mv01(VOID)
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
        if (G.pEm->Pos_y < -0x1FB8)
        {
            G.pEm->Pos_y = -0x1FB8;
        }

        G.pEm->Type = 0;

    case 1:
        G.pEm->Pos_y = G.pEm->Pos_y + (G.pEm->Timer0 * G.pEm->Timer0++ << 3);
        G.pEm->Cdir_z = G.pEm->Cdir_z - 0x55;
        if (G.pEm->Cdir_z & 0x80000000)
        {
            G.pEm->Cdir_z = 0U;
        }
        if (!(G.pEm->Pos_y & 0x80000000))
        {
            G.pEm->Pos_y = 0;
            G.pEm->Cdir_z = 0U;
            G.pEm->Routine_3 = 2U;
        }
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        break;
    case 2:
        FUNC(SND_SE_ENEM)(2, 0);  // Snd_se_enem

        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = 0;
    }
}

VOID Em0a_mv02(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = (Api_rand() & 1) + 1;  // rand
        G.pEm->Timer0 = Api_rand() & 0x3F;  // rand
        G.pEm->Hokan_flg = 3;

    case 1:
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)  // Joint_move
        {
            G.pEm->Cdir_y = G.pEm->Cdir_y + 0x800;
            G.pEm->Routine_1 = 0;
            EM_ROUTINE_HALF2 = 0;
        }
        return;
    }
}

VOID Em0a_mv03(VOID)
{
    PARTS_W* pP_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Timer2 = 0;
        G.pEm->Move_no = 3;
        G.pEm->Hokan_flg = 3;
        G.pEm->Routine_3 = 1U;
        G.pEm->Spd_cur = 0x32;
        FUNC(SND_SE_ENEM)(0, 0);  // Snd_se_enem

    case 1:
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchWord[4] = FUNC(L_POS_CK_XZM)(&pP_ptr[1].Workm, &ScratchPad->Vec, 0x384, &MAIN.Player.Pos_x);  // L_pos_ckXZM
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        if (G.pEm->Move_cnt == 0xB && ScratchWord[5] == 0 && ScratchWord[4] != 0 && G.Player.Damage_no == 0 && (EM0A_FREE_P->Sikai_flg & 1) != 0)
        {
            FUNC(SND_SE_ENEM)(1, 0);  // Snd_se_enem

            FUNC(PAD_VIB_SET0)(5, 0);  // Pad_vib_set0

            if (((Api_rand()) & 1) * (Api_rand() & 1) != 0)  // rand
            {
                G.Player.Sca_info |= 2;
                G.Player.Poison_timer = 0x96;
            }
            G.Player.Life -= 6;
            G.Player.Damage_no = 1;
            G.Player.Routine_2 = 0x64;
            FUNC(SND_SE_STAD)(3, 0, 0, &MAIN.Player.Pos_x);  // Snd_se_stad
            ScratchPad->Vec.vx = 0;
            ScratchPad->Vec.vy = -0x208;
            ScratchPad->Vec.vz = 0;

            FUNC(ESP_CALL)(0, 1, (SHORT)G.Player.Cdir_y, &MAIN.Player.M, &ScratchPad->Vec, 0);  // Esp_call
        }
        FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
        break;
    case 2:
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 2;
        pP_ptr;  // no-op expression statement
    }
}

VOID Em0a_mv04(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Timer2 = 0;
        G.pEm->Move_no = 3;
        G.pEm->Hokan_flg = 3;
        G.pEm->Routine_3 = 1U;
        G.pEm->Spd_cur = 0x32;
        FUNC(SND_SE_ENEM)(0, 0);  // Snd_se_enem

    case 1:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
    }
}

VOID Em0a_mv05(VOID)
{
    return;
}

VOID Em0a_mv06(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Spd_cur = 0x1E;
        G.pEm->Timer0 = 0x3CU;

        G.pEm->Type = 0;

    case 1:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Damage_no = 0;
            EM0A_FREE_P->Hit_en = 1;
            G.pEm->Routine_1 = 0;
            EM_ROUTINE_HALF2 = 0;
        }
    }
}

VOID Em0a_dm00(VOID)
{
    PARTS_W* pP_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_no = 3;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Timer2 = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Routine_3 = 1;
        G.pEm->Timer0 = (Api_rand() & 0x1F) + 0x10;  // rand
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchWord[7] = 4;
        while (--ScratchWord[7] != -1)
        {
            (pP_ptr + 11 - ScratchWord[7])->Be_flg = 0;
        }

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    case 2:
        if (G.pEm->Timer0-- == 0)
        {
            EM_ROUTINE_HALF0 = 1;
            EM_ROUTINE_HALF2 = 0;
            G.pEm->Damage_no = 0;
        }
        return;
    }
}

VOID Em0a_die00(VOID)
{
    PARTS_W* pP_ptr;

    pP_ptr = G.pEm->pSin_parts_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_no = 4;
        G.pEm->Move_cnt = 0x1E;
        G.pEm->Move_wait = 0U;
        G.pEm->Timer2 = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Routine_3 = 1U;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_OBJ;
        G.pEm->Spd_cur = 0x28;
        G.pEm->Timer0 = (Api_rand() & 0xF) + 0x10;  // rand
        if (((G.pEm->Damage_no >> 3) & 0xFF) == 7)
        {
            ScratchWord[7] = 0xB;
            while ((ScratchWord[7] = ScratchWord[7] - 1) != -1)
            {
                FUNC(JOINT_ATCK_SET)(&pP_ptr[ScratchWord[7]], 8U, 5, 3);  // Joint_atck_set
            }
            FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, 0U, 0);  // Kage_half_set
            G.pEm->Routine_2 = 4;
            FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg, G.pEm->Em_set_flg);  // Flg_on
            return;
        }
        if (((G.pEm->Damage_no >> 3) & 0xFF) >= 2U)
        {
            FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, 0U, 0);  // Kage_half_set
            G.pEm->Routine_2 = 3;
            return;
        }
    case 1:
        if (FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)  // Joint_move
        {
            FUNC(KAGE_WORK_COLOR_SET)(&G.pEm->Kage_work, 1U, 2, 0xFFFF50);  // Kage_work_color_set

            FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, 0x14U, 0xA);  // Kage_half_set
            G.pEm->Routine_3 = 2U;
        }
        if (G.pEm->Move_wait < 0x23U)
        {
            FUNC(ADD_SPEED_XZ)(0U);  // Add_speedXZ
        }
        return;
    case 2:
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, 0x16U, 0xB);  // Kage_work_size_add
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_2 = 4;
            FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg, G.pEm->Em_set_flg);  // Flg_on
        }
        return;
    }
}

VOID Em0a_die01(VOID)
{
    PARTS_W* pP_ptr;

    pP_ptr = G.pEm->pSin_parts_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_no = 4;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Timer2 = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Routine_3 = 1;
        G.pEm->Timer0 = (Api_rand() & 0x1F) + 0x10;  // rand
        ScratchPad->Vec.vz = 0;
        ScratchPad->Vec.vy = 0;
        ScratchPad->Vec.vx = 0;

        FUNC(ESP_CALL)(0, 7, (SHORT)G.Player.Cdir_y - G.pEm->Cdir_y + 0x800, &pP_ptr[0].Workm, &ScratchPad->Vec, 0);  // Esp_call
        FUNC(ESP_CALL)(0, 7, (SHORT)G.Player.Cdir_y - G.pEm->Cdir_y + 0x800, &pP_ptr[5].Workm, &ScratchPad->Vec, 0);  // Esp_call

    case 1:
        G.pEm->Scale -= 0xF5;
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        break;
    case 2:
        ScratchWord[7] = 0xB;
        while ((ScratchWord[7] = ScratchWord[7] - 1) != -1)
        {
            pP_ptr[ScratchWord[7]].Be_flg = 0;
        }
        G.pEm->Routine_3 = 3;
        G.pEm->Timer0 = (Api_rand() & 0x1F) + 0xD2;  // rand
        break;
    case 3:
        G.pEm->Pos_y = -0x4E20;
        G.pEm->Routine_3 = 4;

    case 4:
        if (!(G.pEm->Timer0--))  // tests the counter before it is decremented; signed conversion (unsigned operand)
        {
            ScratchWord[7] = 0xB;
            while ((ScratchWord[7] = ScratchWord[7] - 1) != -1)
            {
                pP_ptr[ScratchWord[7]].Be_flg = 3;
            }
            G.pEm->Type = 2;
            if (G.SaveData.Room_no == 1)
            {
                G.pEm->Type = 1;
            }
            EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 0);  // clears state, Key_rno, Routine_2 and Act_sts: restarts the initialisation
        }
        break;
    }
}

static AT_TBL Em0a_sca0 = { 0x8000, 0, 0, 0, 0, 0xC8 };  // collision volume record of the adder
static AT_TBL Em0a_sca1 = { 0x8001, 0, 0, 0, 0, 0 };  // second collision volume record, only referenced by Em0a_sca1_adr
AT_TBL* pEm0a_sca = &Em0a_sca0;                    // collision volume assigned in Em0a_Init
static AT_TBL* Em0a_sca1_adr = &Em0a_sca1;                      // address of Em0a_sca1, never read
// state handlers by entity state: 0=init, 1=run, 2=damaged, 3=death
VOID (*Em0a_routine_0[4])(VOID) =
{
    Em0a_Init,      // 0 //
    Em0a_Move,       // 1 //
    Em0a_Damage,   // 2 //
    Em0a_Die,     // 3 //
};
// behaviors by Routine_2
VOID (*Em0a_mv_tbl[7])(VOID) =
{
    Em0a_mv00,       // 0 //
    Em0a_mv01,        // 1 //
    Em0a_mv02,   // 2 //
    Em0a_mv03,        // 3 //
    Em0a_mv04,     // 4 //
    Em0a_mv05,        // 5 //
    Em0a_mv06,      // 6 //
};
// decision functions by Type: 0=ground, 1=ceiling near, 2=ceiling ambush, 3=hidden
VOID (*Em0a_brain_tbl[7])(VOID) =
{
    Em0a_brain00,           // 0 //
    Em0a_brain01,     // 1 //
    Em0a_brain02,   // 2 //
    Em0a_brain03,           // 3 //
    0,                             // 4 //
    0,                             // 5 //
    0,                             // 6 //
};
