/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM09.C
 */

#include "EM09.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Em09_Init(VOID)
{
    EM_ROUTINE_HALF0 = 1;
    EM_ROUTINE_HALF2 = 0;
    G.pEm->Coord_flg = 0;
    G.pEm->Dummy0c1 = 0;
    G.pEm->Timer0 = 0;
    G.pEm->Timer2 = 0;
    G.pEm->Damage_no = 0;
    EM09_EF(Atck_flg) = 0;
    if (G.pEm->Type == 3)
    {
        EM09_EF(Atck_flg) = 1;
        G.pEm->Type = Api_rand() & 1;  // rand
    }
    if (G.pEm->Type < 2U)
    {
        G.pEm->Cdir_x = 0;
        G.pEm->Cdir_z = 0;
        G.pEm->Pos_y = 0;
    }
    if (G.pEm->Type >= 2U)
    {
        G.pEm->Cdir_x = 0;
        G.pEm->Cdir_z = 0x800;
        G.pEm->Pos_y = -0x17E8;
        G.pEm->Type = 2U;
    }
    FUNC(INIT_PARTS_WORK)();  // Init_Parts_work
    ScratchWord[7] = 0x808080;
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vx = 0;
    FUNC(KAGE_WORK_INIT)(&ScratchPad->Sv, &G.pEm->Kage_work, 0, 0);  // Kage_work_init
    Api_rand();  // rand
    G.pEm->Life = (0x28 + (Api_rand() & 7) + (Api_rand() & 7) + (Api_rand() & 7)) * 2;  // rand
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Move_no = G.pEm->Type;
    FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);  // Joint_move
    G.pEm->pAt_tbl = pEm09_sca;
    G.pEm->Scale = 0;
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    EM09_EF(attack_cooldown) = 0;
    EM09_EF(Kage_stop) = 0;
    EM09_EF(wall_stuck_frames) = 0;
    EM09_EF(Run_flg) = 0;
}

VOID Em09_Main(VOID)
{
    PARTS_W* pP_ptr;
    USHORT unused;

    pP_ptr = G.pEm->pSin_parts_ptr;
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        Em09_state_tbl.state[G.pEm->Routine_0]();
        FUNC(OBA_SET_OFS)(G.pEm);  // Oba_set_ofs
        EM09_EF(Pl_hit) = FUNC(OBA_CK_HIT)(&MAIN.Player.Be_flg, G.pEm);  // Oba_ck_hit
        FUNC(OBA_CK_EM)();  // Oba_ck_em
        G.pEm->Sca_info = G.pEm->Sca_info & 0xF7;
        EM09_EF(room_hit) = FUNC(SCA_CK_HIT)(&G.pEm->Pos_x, G.pEm->pAt_tbl->At_w);  // Sca_ck_hit
    }
    G.pEm->Zone_in_flg = FUNC(HIT_CK_POINT4)(&G.pEm->Pos_x, G.Cccut);  // Hit_ck_point4
    if (G.pEm->Zone_in_flg != 0)
    {
        if (EM09_EF(Kage_stop) == 0)
        {
            ScratchWord[3] = (pP_ptr[3].Workm.t[1] >> 4) + 0x320;
            if (-pP_ptr[3].Workm.t[1] - 0x898 < 0x7D0U)
            {
                ScratchWord[3] = 0x258;
            }
            if (G.pEm->Pos_y < -0x17D4)
            {
                ScratchWord[3] = ScratchWord[3] - 0xC8;
            }
            FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, ScratchWord[3] + 0x32, ScratchWord[3] + 0xC8);  // Kage_half_set
        }
        FUNC(MUL_KAGE)(&G.pEm->Pos_x, &G.pEm->Kage_work, 0U, G.pEm->Cdir_y);  // MulKage
    }
}

VOID Em09_Move(VOID)
{
    if (G.pEm->Type & 0x80)
    {
        return;
    }
    switch (G.pEm->Routine_1)
    {
    case 0:
        Em09_Move_br();

    case 1:
        Em09_behavior_dispatch();
    }
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    G.pEm->Be_flg = G.pEm->Be_flg | 0x40;
    FUNC(EM_CK_HANI)(0xFA0);  // Em_ck_hani
    if (G.pEm->Type != 0 && G.pEm->Routine_2 == 0)
    {
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        FUNC(EM_CK_HANI)(0xFA0);  // Em_ck_hani
    }
    if (G.pEm->Type & 2)
    {
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        FUNC(EM_CK_ALERT)(0x1388);  // Em_ck_alert
    }
    if (EM09_EF(attack_cooldown) != 0)
    {
        EM09_EF(attack_cooldown)--;
    }
    if (EM09_EF(room_hit) != 0)
    {
        EM09_EF(wall_stuck_frames)++;
        return;
    }
    EM09_EF(wall_stuck_frames) = 0U;
}

VOID Em09_Damage(VOID)
{
    if (G.pEm->Routine_1 == 0)
    {
        G.pEm->Routine_2 = Em09_state_tbl.seed[(G.pEm->Damage_no & 7) - 1];
        G.pEm->Routine_2 += (((UINT)FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400)) >> 10) & 1;  // Goto00_ck
        G.pEm->Routine_1 = 1;
        G.pEm->Routine_3 = 0;
    }
    switch (G.pEm->Routine_2)
    {
    case 0:
        G.pEm->Move_no = 0xD;
        Em09_dm00();
        return;
    case 1:
        G.pEm->Move_no = 0xC;
        Em09_dm00();
        return;
    case 2:
    case 3:
        G.pEm->Move_no = 0xF;
        if (G.pEm->Type != 0)
        {
            G.pEm->Move_no = 6;
        }
        Em09_dm00();
        return;
    case 4:
    case 5:
        G.pEm->Move_no = 0xB;
        Em09_dm01();
        break;
    }
}

VOID Em09_Die(VOID)
{
    if (G.pEm->Routine_1 == 0)
    {
        G.pEm->Routine_2 = (((UINT)FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400)) >> 10) & 1;  // Goto00_ck
        G.pEm->Routine_1 = 1U;
        G.pEm->Routine_3 = 0;
        if (G.pEm->Type & 2)
        {
            G.pEm->Routine_2 = 4U;
        }
    }
    switch (G.pEm->Routine_2)
    {
    case 0:
        G.pEm->Move_no = 0xB;
        Em09_dm02();
        return;
    case 1:
        G.pEm->Move_no = 0xC;
        Em09_dm02();
        return;
    case 4:
        G.pEm->Move_no = 0xB;
        Em09_dm03();
        return;
    }
}

VOID Em09_behavior_dispatch(VOID)
{
    ScratchWord[7] = FUNC(EM_SIKAI_PROC)();  // Em_sikai_proc
    if ((ScratchWord[7] >> 1) == 0)
    {
        EM09_EF(Sikai_flg) = EM09_EF(Sikai_flg) & ~1;
        EM09_EF(Sikai_flg) = (EM09_EF(Sikai_flg)) | (ScratchWord[7] & 1);
    }
    Em09_mv_tbl[G.pEm->Routine_2]();
}

VOID Em09_Move_br(VOID)
{
    ScratchWord[0] = (__builtin_abs(G.Player.Pos_x - G.pEm->Pos_x)) + (__builtin_abs(G.Player.Pos_z - G.pEm->Pos_z));
    Em09_Move_br_tbl[G.pEm->Type]();
}

VOID Em09_Move_br00(VOID)
{
    USHORT Old_rtn2;
    SHORT Old_rtn3;

    Old_rtn2 = G.pEm->Routine_2;
    Old_rtn3 = G.pEm->Routine_3;
    if ((EM09_EF(Sikai_flg) & 1) != 0)
    {
        G.pEm->Routine_1 = 0;
        G.pEm->Routine_2 = 1U;
        if (Old_rtn2 != 1)
        {
            G.pEm->Move_no = 9;
            G.pEm->Spd_cur = 0xB4;
            G.pEm->Routine_3 = 0U;
        }
    }
    if (FUNC(L_POS_CK_XZ)(0x200, 0x5DC, &MAIN.Player.Pos_x) != 0)  // L_pos_ckXZ
    {
        if (FUNC(SIKAI_CK)(&MAIN.Player.Pos_x) == 0 && G.Player.Damage_no == 0)  // Sikai_ck
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_UHALF2 = 7;
            if (EM09_EF(Atck_flg) != 0)
            {
                EM_ROUTINE_UHALF2 = 0xC;
                if (EM09_EF(attack_cooldown) != 0)
                {
                    G.pEm->Routine_2 = 5U;
                    G.pEm->Routine_3 = Old_rtn3;
                    if (Old_rtn2 != 5)
                    {
                        G.pEm->Routine_3 = 0U;
                    }
                }
                return;
            }
            if (FUNC(CDIR_CK)(&MAIN.Player) != 0)  // Cdir_ck
            {
                EM_ROUTINE_UHALF2 = 0xC;
            }
            return;
        }
    }
    if (((EM09_EF(Sikai_flg) & 1) == 0 && ScratchWord[0] < 0x1770) || EM09_EF(wall_stuck_frames) >= 0x15)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_UHALF2 = 0xB;
        EM09_EF(wall_stuck_frames) = 0;
    }
    if (G.Player.Routine_2 == 0x14 && ScratchWord[0] >= 0x7D1)
    {
        if (FUNC(CDIR_CK)(&MAIN.Player) == 0 && G.Player.Routine_3 == 0)  // Cdir_ck
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_UHALF2 = (Api_rand() & 1) * 4 + 7;  // rand
            if (EM09_EF(Atck_flg) != 0)
            {
                EM_ROUTINE_UHALF2 = 0xB;
            }
        }
    }
    if (G.Player.Damage_no & 0x80)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_UHALF2 = 0xD;
    }
}

VOID Em09_Move_br01(VOID)
{
    USHORT Old_rtn2;
    SHORT Old_rtn3;

    Old_rtn2 = G.pEm->Routine_2;
    Old_rtn3 = G.pEm->Routine_3;
    if ((EM09_EF(Sikai_flg) & 1) != 0)
    {
        G.pEm->Routine_1 = 0;
        G.pEm->Routine_2 = 1U;
        if (Old_rtn2 != 1)
        {
            G.pEm->Routine_3 = 0U;
            if (ScratchWord[0] >= 0x1389)
            {
                EM09_EF(Run_flg) = 1;
            }
            G.pEm->Move_no = 3;
            G.pEm->Spd_cur = 0x64;
            if (EM09_EF(Run_flg) != 0)
            {
                G.pEm->Move_no = 2;
                G.pEm->Spd_cur = 0xC8;
            }
        }
    }
    if (FUNC(L_POS_CK_XZ)(0x200, 0x5DC, &MAIN.Player.Pos_x) != 0)  // L_pos_ckXZ
    {
        if (FUNC(SIKAI_CK)(&MAIN.Player.Pos_x) == 0 && G.Player.Damage_no == 0)  // Sikai_ck
        {
            if (Api_rand() & 1)  // rand
            {
                G.pEm->Routine_1 = 1;
                EM_ROUTINE_UHALF2 = 8;
                if (FUNC(CDIR_CK)(&MAIN.Player) != 0)  // Cdir_ck
                {
                    EM_ROUTINE_UHALF2 = 0xC;
                }
                if (EM09_EF(Atck_flg) != 0)
                {
                    if (EM09_EF(attack_cooldown) != 0)
                    {
                        G.pEm->Routine_2 = 5U;
                        G.pEm->Routine_3 = Old_rtn3;
                        if (Old_rtn2 != 5)
                        {
                            G.pEm->Routine_3 = 0U;
                        }
                    }
                    else if (EM09_EF(Pl_hit) != 0)
                    {
                        EM_ROUTINE_UHALF2 = 8;
                    }
                }
                EM09_EF(Run_flg) = 0;
                return;
            }
        }
    }
    if (((EM09_EF(Sikai_flg) & 1) == 0 && ScratchWord[0] < 0x1F40) || EM09_EF(wall_stuck_frames) >= 0x15)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_UHALF2 = 2;
        G.pEm->Move_no = 3;
        EM09_EF(turn_step) = 0x20;
        if (ScratchWord[0] >= 0xFA0 && EM09_EF(Run_flg) == 0)
        {
        }
        else
        {
            G.pEm->Move_no = 2;
            EM09_EF(turn_step) = 0x40;
        }
        EM09_EF(wall_stuck_frames) = 0;
        EM09_EF(Run_flg) = 0;
    }
    if ((EM09_EF(Sikai_flg) & 1) != 0)
    {
        if (FUNC(CDIR_CK)(&MAIN.Player) == 0 && ScratchWord[0] >= 0x1771)  // Cdir_ck
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x40) == 0 && G.Player.Routine_2 == 0x13)  // Goto00_ck
            {
                G.pEm->Routine_1 = 1;
                EM_ROUTINE_UHALF2 = 0xB;
                EM09_EF(Run_flg) = 0;
            }
        }
    }
    if (G.Player.Damage_no & 0x80)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_UHALF2 = 0xD;
    }
}

VOID Em09_Move_br02(VOID)
{
    USHORT Old_rtn2;

    Old_rtn2 = G.pEm->Routine_2;
    G.pEm->Cdir_y = G.pEm->Cdir_y + 0x800;
    if ((EM09_EF(Sikai_flg) & 1) == 0)
    {
        G.pEm->Routine_1 = 0;
        G.pEm->Routine_2 = 9U;
        if (Old_rtn2 != 9)
        {
            G.pEm->Routine_3 = 0;
            if (ScratchWord[0] >= 0xBB9)
            {
                EM09_EF(Run_flg) = 1;
            }
        }
    }
    if ((EM09_EF(Sikai_flg) & 1) == 0 && EM09_EF(wall_stuck_frames) < 0x1F)
    {
    }
    else
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_UHALF2 = 0xA;
        G.pEm->Move_no = 3;
        EM09_EF(turn_step) = 0x20;
        if (ScratchWord[0] >= 0xFA0 && EM09_EF(Run_flg) == 0)
        {
        }
        else
        {
            G.pEm->Move_no = 2;
            EM09_EF(turn_step) = 0x40;
        }
        EM09_EF(wall_stuck_frames) = 0;
        EM09_EF(Run_flg) = 0;
    }
    if (EM09_EF(attack_cooldown) == 0 && ScratchWord[0] < 0x7D0 && G.Player.Damage_no == 0)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_UHALF2 = 3;
        if (FUNC(CDIR_CK)(&MAIN.Player) != 0)  // Cdir_ck
        {
            EM_ROUTINE_UHALF2 = 6;
        }
        EM09_EF(Run_flg) = 0;
    }
    if (ScratchWord[0] >= 0x3A99)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200) == 0)  // Goto00_ck
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_UHALF2 = 6;
            EM09_EF(Run_flg) = 0;
        }
    }
    if (G.Player.Damage_no & 0x80)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_UHALF2 = 0xD;
    }
    G.pEm->Cdir_y = G.pEm->Cdir_y - 0x800;
}

VOID Em09_mv00(VOID)
{
    Em09_mv00_tbl[G.pEm->Type]();
}

VOID Em09_mv00_floor(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = G.pEm->Type;
        G.pEm->Hokan_flg = 3;

    case 1:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    }
}

VOID Em09_mv00_ceil(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 1;
        G.pEm->Timer0 = (USHORT)((Api_rand() & 0x3F) + 0x50 - ((EM09_EF(Sikai_flg)) & 1) * 0x50);  // rand
        G.pEm->Hokan_flg = 3;

    case 1:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_1 = 0;
            EM_ROUTINE_HALF2 = 0;
        }
        return;
    }
}

VOID Em09_mv01(VOID)
{
    if ((EM09_EF(Sikai_flg) & 1) != 0)
    {
        FUNC(ROOT_CK)(G.Player.Pos_x, G.Player.Pos_z, &G.pEm->Dest_x, &G.pEm->Dest_z);  // Root_ck
        ScratchPad->Vec.vx = G.pEm->Dest_x;
        ScratchPad->Vec.vz = G.pEm->Dest_z;
        ScratchPad->Vec.vy = 0;
        FUNC(GOTO00)(&ScratchPad->Vec, 0x40);  // Goto00
    }
    else
    {
        G.pEm->Cdir_y = G.pEm->Cdir_y + ((EM09_EF(Sikai_flg)) & 1) * FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x10);  // Goto00_ck
    }
    Em09_mv01_mot();
    FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
    ScratchWord[3] = 0;
    if (!((G.pEm->Move_cnt % 6) & 0xFF) && G.pEm->Move_no == 2)
    {
        FUNC(SND_SE_ENEM)(ScratchWord[3], 0);  // Snd_se_enem
    }
    if (!((G.pEm->Move_cnt % 10) & 0xFF) && G.pEm->Move_no == 3)
    {
        FUNC(SND_SE_ENEM)(ScratchWord[3], 0);  // Snd_se_enem
    }
    if (!((G.pEm->Move_cnt % 15) & 0xFF) && G.pEm->Move_no == 9)
    {
        FUNC(SND_SE_ENEM)(ScratchWord[3], 0);  // Snd_se_enem
    }
}

VOID Em09_mv01_mot(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Timer0 = (Api_rand() & 0x1F) * G.pEm->Routine_2;  // rand
        if (!((G.pEm->Routine_2 >> 1) & 0xFF))
        {
            G.pEm->Timer0 = G.pEm->Timer0 + 0x50;
        }

    case 1:
        FUNC(JOINT_MOVE)(G.Player.Damage_no, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_1 = 0;
            EM_ROUTINE_UHALF2 = 0;
        }
    }
}

VOID Em09_mv02(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Routine_3 = 1U;
        G.pEm->Timer0 = (Api_rand() & 0x1F) + 0x50;  // rand

    case 1:

        ScratchWord[7] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, EM09_EF(turn_step));  // Goto00_ck
        if (!(G.pEm->Timer0-- != 0) || ScratchWord[7] == 0)
        {
            G.pEm->Routine_3 = 2U;
        }
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        G.pEm->Cdir_y = G.pEm->Cdir_y + ScratchWord[7];
        return;
    case 2:
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = 0;
        if ((EM09_EF(Sikai_flg) & 1) != 0)
        {
            EM09_EF(Run_flg) = 1;
        }
        G.pEm->Dest_x = G.Player.Pos_x;
        G.pEm->Dest_z = G.Player.Pos_z;
        return;
    }
}

VOID Em09_mv03(VOID)
{
    PARTS_W* pP_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 8;
        G.pEm->Hokan_flg = 3;
        FUNC(SND_SE_ENEM)(3, 0U);  // Snd_se_enem

    case 1:
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchPad->Vec.vx = 0x64;
        ScratchWord[1] = FUNC(L_POS_CK_XZM)(&pP_ptr[6].Workm, &ScratchPad->Vec, 0x320, &MAIN.Player.Pos_x);  // L_pos_ckXZM
        ScratchWord[2] = FUNC(L_POS_CK_XZM)(&pP_ptr[10].Workm, &ScratchPad->Vec, 0x320, &MAIN.Player.Pos_x);  // L_pos_ckXZM
        if (FUNC(SIKAI_CK)(&MAIN.Player.Pos_x) == 0 && G.Player.Damage_no == 0)  // Sikai_ck
        {
            if (ScratchWord[1] == 0 && ScratchWord[2] == 0)
            {
            }
            else if (((G.pEm->Move_cnt + 0xF9) & 0xFF) < 6U)
            {
                ScratchPad->Vec.vx = 0;
                ScratchPad->Vec.vy = -0x9D8;
                ScratchPad->Vec.vz = 0;

                FUNC(ESP_CALL)(0, 0U, 0x200, &MAIN.Player.M, &ScratchPad->Vec, 0);  // Esp_call

                G.Player.Life -= 0x14;

                ScratchWord[2] = FUNC(CDIR_CK)(&MAIN.Player);  // Cdir_ck
                G.Player.Damage_no = ScratchWord[2] + 1;
                G.Player.Routine_2 = ScratchWord[2] + 0x66;
                FUNC(SND_SE_STAD)(3, G.Player.Damage_no, 0, &MAIN.Player.Pos_x);  // Snd_se_stad
                FUNC(SND_SE_ENEM)(5, 0U);  // Snd_se_enem

                FUNC(PAD_VIB_SET0)(8, 0U);  // Pad_vib_set0
                FUNC(PAD_VIB_SET1)(5, 0xFAU, 4);  // Pad_vib_set1

                ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
                if (ScratchWord[1] != 0)
                {
                    ScratchPad->Vec.vx = 0x64;

                    FUNC(ESP_CALL)(0, 0U, 0, &pP_ptr[6].Workm, &ScratchPad->Vec, 0);  // Esp_call

                    ScratchPad->Vec.vx = 0x12C;

                    FUNC(ESP_CALL)(0, 0U, 0, &pP_ptr[6].Workm, &ScratchPad->Vec, 0);  // Esp_call

                    FUNC(MODEL_COLOR_PLUS)(&pP_ptr[6], 0U, 0x40, 0x90);  // Model_color_plus
                }
                if (ScratchWord[2] != 0)
                {
                    ScratchPad->Vec.vx = 0x64;

                    FUNC(ESP_CALL)(0, 0U, 0, &pP_ptr[10].Workm, &ScratchPad->Vec, 0);  // Esp_call

                    ScratchPad->Vec.vx = 0x12C;

                    FUNC(ESP_CALL)(0, 0U, 0, &pP_ptr[10].Workm, &ScratchPad->Vec, 0);  // Esp_call

                    FUNC(MODEL_COLOR_PLUS)(&pP_ptr[10], 0x90U, 0x50, 0x10);  // Model_color_plus
                }
            }
        }
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)  // Joint_move
        {
            G.pEm->Routine_3 = 2U;
        }
        return;
    case 2:
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = 0;
        EM09_EF(attack_cooldown) = 0x3C;
    }
}

VOID Em09_mv04(VOID)
{
    PARTS_W* pP_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_cnt = 0xAU;
        G.pEm->Move_wait = 0;
        G.pEm->Routine_3 = 1U;
        G.pEm->Hokan_flg = 0;
        G.pEm->Cdir_y = G.Player.Cdir_y;
        G.pEm->Pos_y = 0;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
        EM09_EF(Catch_timer) = 0x5A;
        G.pEm->Move_no = 5;
        G.pEm->Root_x = G.Player.Pos_x;
        G.pEm->Root_z = G.Player.Pos_z;
        G.Player.Root_x = G.Player.Pos_x;
        G.Player.Root_z = G.Player.Pos_z;
        G.pEm->Damage_no = 1;
        G.Player.Damage_no = 1;
        PL_ROUTINE_HALF0 = 0x906;
        PL_ROUTINE_HALF2 = 0;

        FUNC(PAD_VIB_SET1)(0x14, 0xB4, 0);  // Pad_vib_set1

    case 1:
        FUNC(NULL_POS_SET)(G.pEm, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr);  // Null_pos_set
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)  // Joint_move
        {
            G.pEm->Routine_3 = 2U;
        }
        G.pEm->Timer2++;
        if (G.pEm->Move_cnt == 0x22)
        {
            ScratchPad->Vec.vx = 0x320;
            ScratchPad->Vec.vy = -0x71C;
            ScratchPad->Vec.vz = 0;

            FUNC(ESP_CALL)(0, 0, 0x200, &MAIN.Player.M, &ScratchPad->Vec, 0);  // Esp_call

            FUNC(SND_SE_STAD)(3, 1, 0, &MAIN.Player.Pos_x);  // Snd_se_stad
            FUNC(SND_SE_ENEM)(6, 0);  // Snd_se_enem
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

            FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[6].Workm, &ScratchPad->Vec, 0);  // Esp_call

            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[6], 0xA0, 0x50, 0x10);  // Model_color_plus

            G.Player.Life -= 0x1E;

            FUNC(PAD_VIB_SET0)(0xA, 0);  // Pad_vib_set0
            FUNC(PAD_VIB_FADE_SET)(0xF, 0xFA, 0, 8);  // Pad_vib_fade_set
        }
        EM09_EF(Catch_timer) -= FUNC(GACHA_CH)() != 0 ? 3 : 0;  // Gacha_ch
        G.Player.Timer0 = EM09_EF(Catch_timer);
        if (EM09_EF(Catch_timer) < 0)
        {
            EM09_EF(Catch_timer) = 0;
            G.Player.Timer0 = 0;
            return;
        }
        return;
    case 2:
        G.pEm->Damage_no = 0;
        G.pEm->Type = 1;
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 7;
        if (EM09_EF(Atck_flg) != 0)
        {
            EM_ROUTINE_HALF2 = 5;
            G.pEm->Be_flg = G.pEm->Be_flg & ~EM_BE_NO_HIT;
            EM09_EF(attack_cooldown) = 0x5A;
        }
    }
}

VOID Em09_mv05(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        if (G.pEm->Type != 0)
        {
            G.pEm->Routine_3 = 3U;
            return;
        }
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 4;
        FUNC(SND_SE_ENEM)(0U, 0);  // Snd_se_enem
    case 1:
        if (FUNC(JOINT_MOVE)(G.pEm->Type, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)  // Joint_move
        {
            G.pEm->Routine_3 = 2U;
        }
        return;
    case 2:
        FUNC(SND_SE_ENEM)(0U, 0);  // Snd_se_enem
        G.pEm->Type = (G.pEm->Type + 1) % 2;
        G.pEm->Routine_3 = 3U;

    case 3:
        G.pEm->Routine_3 = 4U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 7;
        ScratchPad->Vec.vx = 0xC8;
        ScratchPad->Vec.vy = -0x654;
        ScratchPad->Vec.vz = 0;

        FUNC(ESP_CALL)(0U, 0x1F, 0, G.pEm->M, &ScratchPad->Vec, 0);  // Esp_call

        EM09_EF(Run_flg) = 0;

    case 4:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    case 5:
        G.pEm->Routine_3 = 0U;
        if (EM09_EF(attack_cooldown) == 0)
        {
            EM_ROUTINE_HALF0 = 1;
            EM_ROUTINE_HALF2 = 0;
        }

    default:;
    }
}

VOID Em09_mv06(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0;
        G.pEm->Move_no = 0x10;
        G.pEm->Timer0 = 0;
        G.pEm->Damage_no = 1;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;

    case 1:
        G.pEm->Pos_y = -Em09_arc_tbl.Pos_y_tbl[0x20 - G.pEm->Move_cnt];
        FUNC(JOINT_MOVE)(1, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        G.pEm->Routine_3 = 2U;
        return;
    case 2:
        G.pEm->Cdir_z = 0;
        G.pEm->Routine_3 = 3U;
        G.pEm->Hokan_flg = 0;
        G.pEm->Type = 1;
        FUNC(SND_SE_ENEM)(3, 0);  // Snd_se_enem

    case 3:
        G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x20);  // Goto00_ck
        if (G.pEm->Move_cnt == 0x19)
        {
            if (FUNC(L_POS_CK_XZ)(0x400, 0x7D0, &MAIN.Player.Pos_x) != 0)  // L_pos_ckXZ
            {
                if (FUNC(SIKAI_CK)(&MAIN.Player.Pos_x) == 0 && G.Player.Damage_no == 0)  // Sikai_ck
                {
                    if (FUNC(CDIR_CK)(&MAIN.Player) != 0)  // Cdir_ck
                    {
                        EM_ROUTINE_HALF2 = 4;
                        G.pEm->Cdir_y = FUNC(GET_DIR_TGT)(G.Player.Pos_x, G.Player.Pos_z);  // Get_dir_tgt
                        G.Player.Damage_no = 1;
                        G.pEm->Damage_no = 0;
                        return;
                    }
                }
            }
        }
        G.pEm->Pos_y = -Em09_arc_tbl.Pos_y_tbl[0x20 - G.pEm->Move_cnt];
        G.pEm->Spd_cur = Em09_arc_tbl.Spd_tbl[0x20 - G.pEm->Move_cnt];
        FUNC(ADD_SPEED_XZ)(0x800);  // Add_speedXZ
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(1, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        if (G.pEm->Move_cnt == 0x1F)
        {
            FUNC(SND_SE_ENEM)(4, 0);  // Snd_se_enem
        }
        return;
    case 4:
        G.pEm->Damage_no = 0;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 3;
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);  // Joint_move
        G.pEm->Be_flg = G.pEm->Be_flg & ~EM_BE_NO_HIT;
        G.pEm->Pos_y = 0;
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = 0;
    }
}

VOID Em09_mv07(VOID)
{
    PARTS_W* pP_ptr;

    pP_ptr = G.pEm->pSin_parts_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0;
        G.pEm->Move_no = 0x10;
        G.pEm->Timer0 = 0;
        G.pEm->Damage_no = 1;
        FUNC(SND_SE_ENEM)(1, 0);  // Snd_se_enem

    case 1:
        G.pEm->Pos_y = -Em09_arc_tbl.Pos_y_tbl[G.pEm->Move_cnt];
        G.pEm->Spd_cur = Em09_arc_tbl.Spd_tbl[G.pEm->Move_cnt];
        FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        if (G.pEm->Move_cnt == 0x20)
        {
            G.pEm->Routine_3 = 2U;
        }
        return;
    case 2:
        G.pEm->Pos_y = -Em09_arc_tbl.Pos_y_tbl[G.pEm->Move_cnt];
        G.pEm->Cdir_z = 0x800;
        G.pEm->Routine_3 = 3U;
        G.pEm->Hokan_flg = 0;
        G.pEm->Type = 2;
        FUNC(SND_SE_ENEM)(2, 0);  // Snd_se_enem

    case 3:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    case 4:
        G.pEm->Damage_no = 0;
        EM09_EF(attack_cooldown) = 0x5A;
        G.pEm->Be_flg = G.pEm->Be_flg & ~EM_BE_NO_HIT;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 1;
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);  // Joint_move
        G.pEm->Pos_y = -0x17E8;
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = 0;

    default:;
    }
}

VOID Em09_mv08(VOID)
{
    PARTS_W* pP_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0xE;
        G.pEm->Hokan_flg = 3;
        FUNC(SND_SE_ENEM)(3, 0);  // Snd_se_enem

    case 1:
        pP_ptr = G.pEm->pSin_parts_ptr;
        if (FUNC(L_POS_CK_XZ)(0x200, 0x7D0, &MAIN.Player.Pos_x) != 0)  // L_pos_ckXZ
        {
            if (FUNC(SIKAI_CK)(&MAIN.Player.Pos_x) == 0 && G.Player.Damage_no == 0 && ((G.pEm->Move_cnt + 0xF6) & 0xFF) < 3U)  // Sikai_ck
            {
                ScratchPad->Vec.vx = 0;
                ScratchPad->Vec.vy = -0x9D8;
                ScratchPad->Vec.vz = 0;

                FUNC(ESP_CALL)(0, 0, 0x200, &MAIN.Player.M, &ScratchPad->Vec, 0);  // Esp_call

                G.Player.Life -= 0x14;

                ScratchWord[2] = FUNC(CDIR_CK)(&MAIN.Player);  // Cdir_ck
                G.Player.Damage_no = ScratchWord[2] + 1;
                G.Player.Routine_2 = ScratchWord[2] + 0x66;
                FUNC(SND_SE_STAD)(3, 1, 0, &MAIN.Player.Pos_x);  // Snd_se_stad
                FUNC(SND_SE_ENEM)(6, 0);  // Snd_se_enem
                ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

                FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[6].Workm, &ScratchPad->Vec, 0);  // Esp_call

                FUNC(MODEL_COLOR_PLUS)(&pP_ptr[6], 0x10, 0x20, 0x60);  // Model_color_plus

                FUNC(PAD_VIB_SET0)(8, 0);  // Pad_vib_set0
                FUNC(PAD_VIB_SET1)(5, 0xFA, 4);  // Pad_vib_set1
            }
        }
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)  // Joint_move
        {
            G.pEm->Routine_3 = 2U;
        }
        return;
    case 2:
        if (EM09_EF(Atck_flg) != 0)
        {
            EM09_EF(attack_cooldown) = 0x28;
        }
        G.pEm->Type = 0;
        EM_ROUTINE_HALF0 = 1;
        EM_ROUTINE_HALF2 = 0;
    }
}

VOID Em09_mv09(VOID)
{
    G.pEm->Move_no = 3U;
    G.pEm->Spd_cur = 0x64;
    if (EM09_EF(Run_flg) != 0)
    {
        G.pEm->Move_no = 2U;
        G.pEm->Spd_cur = 0xC8;
    }
    G.pEm->Cdir_y = G.pEm->Cdir_y + 0x800;
    G.pEm->Cdir_y = G.pEm->Cdir_y + (EM09_EF(Sikai_flg) & 1) * FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x10);  // Goto00_ck
    G.pEm->Cdir_y = G.pEm->Cdir_y - 0x800;
    Em09_mv01_mot();
    FUNC(ADD_SPEED_XZ)(0x800);  // Add_speedXZ
    ScratchWord[3] = 2;
    if (G.pEm->Move_cnt == 0 && G.pEm->Move_no == 2)
    {
        FUNC(SND_SE_ENEM)(ScratchWord[3], 0);  // Snd_se_enem
    }
}

VOID Em09_mv0a(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Routine_3 = 1U;
        G.pEm->Timer0 = 0x50U;

    case 1:
        G.pEm->Cdir_y = G.pEm->Cdir_y + 0x800;
        ScratchWord[7] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, EM09_EF(turn_step));  // Goto00_ck
        if (!(G.pEm->Timer0-- != 0) || ScratchWord[7] == 0)
        {
            G.pEm->Routine_3 = 2U;
        }
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        G.pEm->Cdir_y = G.pEm->Cdir_y + ScratchWord[7];
        G.pEm->Cdir_y = G.pEm->Cdir_y - 0x800;
        return;
    case 2:
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = 0;
        G.pEm->Dest_x = G.Player.Pos_x;
        G.pEm->Dest_z = G.Player.Pos_z;
        return;
    }
}

VOID Em09_mv0b(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 4;
        G.pEm->Hokan_flg = 3;
        FUNC(SND_SE_ENEM)(0U, 0);  // Snd_se_enem

    case 1:
        if (FUNC(JOINT_MOVE)(G.pEm->Type, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)  // Joint_move
        {
            G.pEm->Routine_3 = 2U;
        }
        return;
    case 2:
        FUNC(SND_SE_ENEM)(0U, 0);  // Snd_se_enem
        G.pEm->Type = (G.pEm->Type + 1) % 2;
        EM_ROUTINE_HALF0 = 1;
        EM_ROUTINE_HALF2 = 0;
    }
}

VOID Em09_mv0c(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0xA;
        G.pEm->Hokan_flg = 3;
        FUNC(SND_SE_ENEM)(1, 0);  // Snd_se_enem

    case 1:
        G.pEm->Cdir_y = G.pEm->Cdir_y + (EM09_EF(Sikai_flg) & 1) * FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x20);  // Goto00_ck
        if (G.pEm->Move_cnt == 0xA && FUNC(L_POS_CK_XZ)(0x200, 0x7D0, &MAIN.Player.Pos_x) != 0 && FUNC(SIKAI_CK)(&MAIN.Player.Pos_x) == 0 && G.Player.Damage_no == 0 && FUNC(CDIR_CK)(&MAIN.Player) != 0 && !((G.pEm->Sca_info = G.pEm->Sca_info & 8) & 0xFF))  // L_pos_ckXZ, Sikai_ck, Cdir_ck
        {
            EM_ROUTINE_HALF2 = 4;
            G.pEm->Cdir_y = FUNC(GET_DIR_TGT)(G.Player.Pos_x, G.Player.Pos_z);  // Get_dir_tgt
            G.Player.Damage_no = 1;
            G.pEm->Damage_no = 1;
            return;
        }
        else if (EM09_EF(Atck_flg) != 0 && EM09_EF(Pl_hit) != 0 && G.pEm->Move_cnt == 0xA && G.Player.Damage_no == 0)
        {
            G.Player.Life -= 0xA;

            ScratchWord[2] = FUNC(CDIR_CK)(&MAIN.Player);  // Cdir_ck
            G.Player.Damage_no = ScratchWord[2] + 1;
            G.Player.Routine_2 = ScratchWord[2] + 0x66;
            FUNC(SND_SE_STAD)(3, 0, 0, &MAIN.Player.Pos_x);  // Snd_se_stad

            FUNC(PAD_VIB_SET0)(3, 0);  // Pad_vib_set0
            FUNC(PAD_VIB_SET1)(5, 0xB4, 0);  // Pad_vib_set1
        }
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)  // Joint_move
        {
            G.pEm->Routine_3 = 2U;
        }
        FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
        if (G.pEm->Move_cnt == 0x17)
        {
            FUNC(SND_SE_ENEM)(4, 0);  // Snd_se_enem
        }
        return;
    case 2:
        G.pEm->Type = 1;
        EM_ROUTINE_HALF0 = 1;
        EM_ROUTINE_HALF2 = 0;
    }
}

VOID Em09_mv0d(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = Em09_mot_tbl[G.pEm->Type];
        FUNC(SND_SE_ENEM)(8U, 0);  // Snd_se_enem

    case 1:
        FUNC(JOINT_MOVE)(G.Player.Damage_no, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
    }
}

VOID Em09_dm00(VOID)
{
    PARTS_W* pP_ptr;

    pP_ptr = G.pEm->pSin_parts_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Spd_cur = 0xA0;
        FUNC(SND_SE_ENEM)(7, 0);  // Snd_se_enem
        ScratchPad->Vec.vz = 0;
        ScratchPad->Vec.vy = 0;
        ScratchPad->Vec.vx = 0;

        FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[3].Workm, &ScratchPad->Vec, 0);  // Esp_call

        if (G.pEm->Life < 0x14 && !(G.pEm->Damage_no & 1))
        {
            Em09_Esp_set();
        }

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        if (G.pEm->Routine_2 < 2U)
        {
            if (G.pEm->Move_cnt >= 0xAU)
            {
                G.pEm->Spd_cur = 0x3C;
            }
            FUNC(ADD_SPEED_XZ)((1 - G.pEm->Routine_2) * 0x800);  // Add_speedXZ
        }
        if (G.pEm->Move_cnt == 0xA)
        {
            ScratchPad->Vec.vz = 0;
            ScratchPad->Vec.vy = 0;
            ScratchPad->Vec.vx = 0;

            FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[3].Workm, &ScratchPad->Vec, 0);  // Esp_call
        }
        return;
    case 2:
        G.pEm->Damage_no = 0U;
        EM_ROUTINE_HALF0 = 1;
        EM_ROUTINE_HALF2 = 0;
        if (Api_rand() & 1)  // rand
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_HALF2 = 7;
            if (EM09_EF(Atck_flg) != 0)
            {
                EM_ROUTINE_HALF2 = 0xC;
            }
        }
        return;
    }
}

VOID Em09_dm01(VOID)
{
    PARTS_W* pP_ptr;

    pP_ptr = G.pEm->pSin_parts_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0;
        G.pEm->Move_no = 0x10;
        G.pEm->Timer0 = 0;

    case 1:
        G.pEm->Pos_y = -Em09_arc_tbl.Pos_y_tbl[0x20 - G.pEm->Move_cnt];
        FUNC(JOINT_MOVE)(1, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        G.pEm->Routine_3 = 2;
        return;
    case 2:
        G.pEm->Pos_y += 0x3E8;
        G.pEm->Cdir_z = 0;
        G.pEm->Routine_3 = 3;
        G.pEm->Hokan_flg = 0;
        G.pEm->Type = 1;
        G.pEm->Timer0 = ((0x82 - G.pEm->Life) >> 2) + 0x14;
        ScratchPad->Vec.vz = 0;
        ScratchPad->Vec.vy = 0;
        ScratchPad->Vec.vx = 0;

        FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[3].Workm, &ScratchPad->Vec, 0);  // Esp_call

        if (G.pEm->Life < 0x14 && !(G.pEm->Damage_no & 1))
        {
            Em09_Esp_set();
        }

    case 3:
        G.pEm->Pos_y += 0x64 + G.pEm->Move_cnt * 0x32;
        if (G.pEm->Pos_y > 0)
        {
            G.pEm->Pos_y = 0;
            if (G.pEm->Damage_no >= 2U)
            {
                FUNC(SND_SE_ENEM)(4, 0);  // Snd_se_enem
                G.pEm->Damage_no = 1;
            }
        }
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)  // Joint_move
        {
            G.pEm->Routine_3 = 4;
        }
        return;
    case 4:
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 5;
        }
        return;
    case 5:
        G.pEm->Routine_3 = 6;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0;
        G.pEm->Timer0 = 0;

    case 6:
        G.pEm->Move_no = 0x11;
        if (G.pEm->Move_cnt == 0x16)
        {
            G.pEm->Cdir_y += 0x800;
        }
        G.pEm->Routine_3 += FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        return;
    case 7:
        G.pEm->Damage_no = 0;
        EM_ROUTINE_HALF0 = 1;
        EM_ROUTINE_HALF2 = 0;
        if (Api_rand() & 1)  // rand
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_HALF2 = 7;
        }

    default:;
    }
}

VOID Em09_dm02(VOID)
{
    PARTS_W* pP_ptr;

    pP_ptr = G.pEm->pSin_parts_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Spd_cur = 0xA0;
        G.pEm->Timer0 = 0x3CU;
        ScratchPad->Vec.vz = 0;
        ScratchPad->Vec.vy = 0;
        ScratchPad->Vec.vx = 0;

        FUNC(ESP_CALL)(0, 0U, 0, &pP_ptr[3].Workm, &ScratchPad->Vec, 0);  // Esp_call

        FUNC(SND_SE_ENEM)(8, 0U);  // Snd_se_enem

        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);  // Flg_on

        Em09_Esp_set();

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        if (G.pEm->Move_cnt >= 0xAU)
        {
            G.pEm->Spd_cur = 0x3C;
        }
        FUNC(ADD_SPEED_XZ)((1 - G.pEm->Routine_2) * 0x800);  // Add_speedXZ
        if (G.pEm->Move_cnt == 0xF && G.pEm->Routine_2 != 0)
        {
            G.pEm->Routine_3 = 2U;
        }
        return;
    case 2:
        G.pEm->Routine_3 = 3U;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_OBJ;
        EM09_EF(Kage_stop) = 1;

        FUNC(KAGE_WORK_COLOR_SET)(&G.pEm->Kage_work, 1U, 2, 0xFFFF50);  // Kage_work_color_set

        FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, 0U, 0);  // Kage_half_set

    case 3:
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, 0x16U, 0x16);  // Kage_work_size_add
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 4U;
            FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);  // Flg_on
        }
        return;
    }
}

VOID Em09_dm03(VOID)
{
    PARTS_W* pP_ptr;

    pP_ptr = G.pEm->pSin_parts_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0;
        G.pEm->Move_no = 0x10;
        G.pEm->Timer0 = 0x3CU;
        FUNC(SND_SE_ENEM)(8, 0);  // Snd_se_enem
        Em09_Esp_set();

    case 1:
        G.pEm->Pos_y = -Em09_arc_tbl.Pos_y_tbl[0x20 - G.pEm->Move_cnt];
        FUNC(JOINT_MOVE)(1, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
        G.pEm->Routine_3 = 2U;
        return;
    case 2:
        G.pEm->Pos_y = G.pEm->Pos_y + 0x3E8;
        G.pEm->Cdir_z = 0;
        G.pEm->Routine_3 = 3U;
        G.pEm->Hokan_flg = 0;
        G.pEm->Type = 1;
        ScratchPad->Vec.vz = 0;
        ScratchPad->Vec.vy = 0;
        ScratchPad->Vec.vx = 0;

        FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[3].Workm, &ScratchPad->Vec, 0);  // Esp_call

    case 3:
        G.pEm->Pos_y += 0x64 + G.pEm->Move_cnt * 0x32;
        if (G.pEm->Pos_y > 0)
        {
            G.pEm->Pos_y = 0;
            if (G.pEm->Damage_no >= 2U)
            {
                FUNC(SND_SE_ENEM)(4, 0);  // Snd_se_enem
                G.pEm->Damage_no = 1U;
            }
        }
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)  // Joint_move
        {
            G.pEm->Routine_3 = 4U;
            G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
            G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_OBJ;
            EM09_EF(Kage_stop) = 1;

            FUNC(KAGE_WORK_COLOR_SET)(&G.pEm->Kage_work, 1, 2, 0xFFFF50);  // Kage_work_color_set

            FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, 0, 0);  // Kage_half_set
        }
        return;
    case 4:
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, 0x18, 0x18);  // Kage_work_size_add
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 5U;
            FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);  // Flg_on
        }
        break;
    }
}

VOID Em09_pldm00(VOID)
{
    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Routine_3 = 1;
        G.Player.Move_no = 2;
        G.Player.Hokan_flg = 3;
        G.Player.Spd_cur = 0xC8;

        FUNC(PAD_VIB_SET1)(8, 0xFA, 0x1B);  // Pad_vib_set1
        FUNC(PAD_VIB_FADE_SET)(0x28, 0xC8, 0, 0x2A);  // Pad_vib_fade_set

    case 1:
        if (G.Player.Move_cnt < 0x24U)
        {
            FUNC(NULL_POS_SET)(&MAIN.Player.Be_flg, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr);  // Null_pos_set
        }
        else
        {
            FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
        }
        if (FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400) != 0)  // Joint_move
        {
            G.Player.Routine_3 = 2;
        }
        return;
    case 2:
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Routine_3 = 3;
        G.Player.Move_no = 3;
        G.Player.Hokan_flg = 3;
        G.Player.Damage_no = 2;

    case 3:
        if (G.Player.Damage_no >= 2U && G.Player.Move_cnt >= 4U)
        {
            FUNC(SND_SE_STAD)(3, 2, 0, &MAIN.Player.Pos_x);  // Snd_se_stad
            FUNC(SND_SE_WALK)(2, 0);  // Snd_se_walk
            G.Player.Damage_no = 1;
        }
        if (G.Player.Life < 0 && G.Player.Move_cnt >= 0xBU)
        {
            FUNC(SND_SE_WALK)(2, 0);  // Snd_se_walk
            G.Player.Routine_3 = 8;
            return;
        }
        G.Player.Timer0 -= FUNC(GACHA_CH)() != 0 ? 3 : 0;  // Gacha_ch
        if (G.Player.Timer0 < 0)
        {
            if (FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400) != 0)  // Joint_move
            {
                G.Player.Routine_3 = 4;
                return;
            }
        }
        if (FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400) != 0)  // Joint_move
        {
            G.Player.Routine_3 = 4;
        }
        return;
    case 4:
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Routine_3 = 5;
        G.Player.Move_no = 5;
        G.Player.Hokan_flg = 3;

    case 5:
        G.Player.Timer0 -= FUNC(GACHA_CH)() != 0 ? 3 : 0;  // Gacha_ch
        if (G.Player.Timer0 < 0)
        {
            if (FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400) != 0)  // Joint_move
            {
                G.Player.Routine_3 = 6;
                return;
            }
        }
        if (FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400) != 0)  // Joint_move
        {
            G.Player.Routine_3 = 6;
        }
        return;
    case 6:
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Routine_3 = 7;
        G.Player.Move_no = 4;
        G.Player.Hokan_flg = 3;

    case 7:
        if (FUNC(JOINT_MOVE)(1, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400) != 0)  // Joint_move
        {
            if (G.Player.Life < 0)
            {
                G.Player.Routine_3 = 8;
                return;
            }
            PL_ROUTINE_HALF0 = 1;
            PL_ROUTINE_HALF2 = 0;
            G.Player.Damage_no = 0;
        }
        break;
    case 8:
        FUNC(SND_SE_STAD)(3, 3, 0, &MAIN.Player.Pos_x);  // Snd_se_stad
        G.Player.Routine_3 = 9;
        G.Player.Timer0 = 0x5A;

        FUNC(KAGE_WORK_COLOR_SET)(&MAIN.Player.Kage_work.Off_x, 1, 2, 0xFFFF50);  // Kage_work_color_set

        FUNC(KAGE_HALF_SET)(&MAIN.Player.Kage_work.Off_x, 0, 0);  // Kage_half_set

        FUNC(PAD_VIB_FADE_SET)(0xA, 0x64, 0);  // Pad_vib_fade_set
        FUNC(PAD_VIB_FADE_SET)(0x5A, 0x64, 0, 0xA);  // Pad_vib_fade_set

    case 9:
        FUNC(KAGE_WORK_SIZE_ADD)(&MAIN.Player.Kage_work.Off_x, 0x14, 0x14);  // Kage_work_size_add
        if (G.Player.Timer0-- == 0)
        {
            G.Player.Routine_3 = 0xA;
        }
        break;
    }
}

VOID Em09_Esp_set(VOID)
{
    UCHAR i;
    PARTS_W* pP_ptr;

    pP_ptr = G.pEm->pSin_parts_ptr;
    ScratchPad->Vec.vz = 0;
    ScratchPad->Vec.vy = 0;
    ScratchPad->Vec.vx = 0;

    FUNC(ESP_CALL)(0, 0U, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);  // Esp_call
    FUNC(ESP_CALL)(3, 0U, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);  // Esp_call

    ScratchPad->Vec.vx = 0x64;
    ScratchPad->Vec.vy = -0x5DC;
    ScratchPad->Vec.vz = 0;
    i = 5;
    while (i--)
    {
        FUNC(ESP_CALL)(0x1D, i, (SHORT)G.Player.Cdir_y - G.pEm->Cdir_y + 0x600 + (i - 1) * 0x100, G.pEm->M, &ScratchPad->Vec, 0);  // Esp_call
    }
}

static AT_TBL Em09_sca_data0 = { 0x8000, 0, -180, 0, 0xB4, 0x1F4 };    // collision volume 0 (flags 0x8000, offset y -0xB4, half height 0xB4, radius 0x1F4)
static AT_TBL Em09_sca_data1 = { 0x8001, 0, 5000, 0, 0, 0 };    // collision volume 1 (flags 0x8001, offset y 0x1388, no extent); only referenced by pEm09_sca1
AT_TBL* pEm09_sca = &Em09_sca_data0;    // collision volume of the chimera
static AT_TBL* pEm09_sca1 = &Em09_sca_data1;    // pointer to collision volume 1; never read
// state handlers by entity state, first hit reaction by hit type
EM09_STATE_TBL Em09_state_tbl =
{
    {
        Em09_Init,        // 0 //
        Em09_Move,         // 1 //
        Em09_Damage,      // 2 //
        Em09_Die,    // 3 //
    },
    {
        0x02,   // 0 //
        0x00,   // 1 //
        0x02,   // 2 //
        0x00,   // 3 //
        0x04,   // 4 //
        0x04,   // 5 //
        0x00,   // 6 //
        0x00,   // 7 //
    },
};
// actions by Routine_2
VOID (*Em09_mv_tbl[14])(VOID) =
{
    Em09_mv00,           // 0 //
    Em09_mv01,           // 1 //
    Em09_mv02,           // 2 //
    Em09_mv03,          // 3 //
    Em09_mv04,       // 4 //
    Em09_mv05,         // 5 //
    Em09_mv06,           // 6 //
    Em09_mv07,          // 7 //
    Em09_mv08,           // 8 //
    Em09_mv09,           // 9 //
    Em09_mv0a,      // 10 //
    Em09_mv0b,   // 11 //
    Em09_mv0c,           // 12 //
    Em09_mv0d,                // 13 //
};
VOID (*Em09_Move_br_tbl[3])(VOID) = { Em09_Move_br00, Em09_Move_br01, Em09_Move_br02 };    // decision routines by Type
// idle routines by Type
VOID (*Em09_mv00_tbl[3])(VOID) =
{
    Em09_mv00_floor,     // 0 //
    Em09_mv00_floor,     // 1 //
    Em09_mv00_ceil,   // 2 //
};
// height and speed along the swoop and drop arc by animation frame
EM09_ARC_TBL Em09_arc_tbl =
{
    {
        0x0604,   // 0 //
        0x04C9,   // 1 //
        0x0410,   // 2 //
        0x050A,   // 3 //
        0x0659,   // 4 //
        0x0834,   // 5 //
        0x09C4,   // 6 //
        0x0B54,   // 7 //
        0x0C80,   // 8 //
        0x0DAC,   // 9 //
        0x0ED8,   // 10 //
        0x1004,   // 11 //
        0x10CC,   // 12 //
        0x1194,   // 13 //
        0x11F8,   // 14 //
        0x125C,   // 15 //
        0x125C,   // 16 //
        0x125C,   // 17 //
        0x125C,   // 18 //
        0x125C,   // 19 //
        0x125C,   // 20 //
        0x125C,   // 21 //
        0x125C,   // 22 //
        0x125C,   // 23 //
        0x125C,   // 24 //
        0x12C0,   // 25 //
        0x1324,   // 26 //
        0x1388,   // 27 //
        0x13EC,   // 28 //
        0x13EC,   // 29 //
        0x13EC,   // 30 //
        0x13EC,   // 31 //
        0x13EC,   // 32 //
    },
    {
        0x0000,   // 0 //
        0x0000,   // 1 //
        0x0000,   // 2 //
        0x0000,   // 3 //
        0x0000,   // 4 //
        0x0000,   // 5 //
        0x0000,   // 6 //
        0x0000,   // 7 //
        0x0000,   // 8 //
        0x0000,   // 9 //
        0x0000,   // 10 //
        0x0000,   // 11 //
        0x0000,   // 12 //
        0x0000,   // 13 //
        0x0000,   // 14 //
        0x0000,   // 15 //
        0x0000,   // 16 //
        0x0000,   // 17 //
        0x0096,   // 18 //
        0x0096,   // 19 //
        0x0096,   // 20 //
        0x0096,   // 21 //
        0x0096,   // 22 //
        0x0096,   // 23 //
        0x0096,   // 24 //
        0x0096,   // 25 //
        0x0096,   // 26 //
        0x0096,   // 27 //
        0x0000,   // 28 //
        0x0000,   // 29 //
        0x0000,   // 30 //
        0x0000,   // 31 //
        0x0000,   // 32 //
    },
};
// animation of the hold action by Type
SCHAR Em09_mot_tbl[4] =
{
    0x00,   // 0 //
    0x06,   // 1 //
    0x06,   // 2 //
    0x00,   // 3 //
};
