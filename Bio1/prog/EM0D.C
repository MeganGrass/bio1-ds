/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM0D.C
 */

#include "EM0D.H"
#include "JOINT.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Em0d_Init(VOID)
{
    PARTS_W* pP_ptr;
    ENEMY_WORK* pSub;

    EM_ROUTINE_WORD = ROUTINES(0, 1, 0, 1);  // state 1, Key_rno 0, Routine_2 1, Act_sts 0
    G.pEm->Coord_flg = 0;
    G.pEm->Damage_no = 0;
    FUNC(INIT_PARTS_WORK)();  // Init_Parts_work
    ScratchWord[7] = 0x808080;
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchSvU[0] = 0;
    FUNC(KAGE_WORK_INIT)(&ScratchPad->Sv, &G.pEm->Kage_work, 0x3E8, 0x3E8);  // Kage_work_init
    G.pEm->Life = 0xBEA;
    if (G.pEm->Id == 0x12)
    {
        G.pEm->Life = 0x190;
    }
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Move_no = (G.pEm->Type & 2) * 5 + 1;
    if ((G.pEm->Type & 0xF) == 2)
    {
        G.pEm->Move_no = 2;
    }
    G.pEm->Hokan_flg = 0;
    pP_ptr = G.pEm->pSin_parts_ptr;
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vx = pP_ptr->Coord.t[0];
    FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);  // RotMatrix
    FUNC(APPLY_MATRIX_SV)(G.pEm->M, &ScratchPad->Sv, &ScratchPad->Sv);  // ApplyMatrixSV
    G.pEm->Pos_x = G.pEm->Pos_x - ScratchPad->Sv.vx;
    G.pEm->Pos_z = G.pEm->Pos_z - ScratchPad->Sv.vz;
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);  // Joint_move
    FUNC(JOINT_POS_TRANS)(0);  // Joint_pos_trans
    pP_ptr = G.pEm->pSin_parts_ptr;
    G.pEm->Pos_x = pP_ptr->Workm.t[0];
    G.pEm->Pos_y = 0;
    G.pEm->Pos_z = pP_ptr->Workm.t[2];
    G.pEm->Old_x = G.pEm->Pos_x;
    G.pEm->Old_y = 0;
    G.pEm->Old_z = G.pEm->Pos_z;
    EM0D_FREE_P->Ground_y = pP_ptr[7].Workm.t[1];
    G.pEm->pAt_tbl = pEm0d_sca;
    G.pEm->pSca_ofs->Off_x = 0;
    G.pEm->pSca_ofs->Off_z = 0;
    G.pEm->pSca_ofs->Off_y = G.pEm->pAt_tbl->Off_y;
    pP_ptr->Coord.t[0] = 0;
    pP_ptr->Coord.t[2] = 0;
    pP_ptr[3].Be_flg |= 8;
    pP_ptr[4].Be_flg |= 8;
    pP_ptr[5].Be_flg |= 8;
    pP_ptr[6].Be_flg |= 8;
    pP_ptr[7].Be_flg |= 8;
    pP_ptr[8].Be_flg |= 8;
    pP_ptr[9].Be_flg |= 8;
    pP_ptr[10].Be_flg |= 8;
    pP_ptr[11].Be_flg |= 8;
    pP_ptr[12].Be_flg |= 8;
    pP_ptr[13].Be_flg |= 8;
    pP_ptr[14].Be_flg |= 8;
    FUNC(JOINT_POS_TRANS)(0);  // Joint_pos_trans
    EM0D_FREE_P->Move_spd = 0;
    EM0D_FREE_P->Sway_add = 0x40;
    EM0D_FREE_P->form = 0;
    EM0D_FREE_P->Bite_cnt = 0;
    EM0D_FREE_P->Swallow_scale = 0;
    EM0D_FREE_P->Bite_dir = 0;
    EM0D_FREE_P->Bite_wait = 0;
    EM0D_FREE_P->Way_idx = 0;
    EM0D_FREE_P->Die_scale = 0;
    EM0D_FREE_P->Die_cnt = 0;
    EM0D_FREE_P->Pl_low_flg = 0;
    EM0D_FREE_P->wall_frames = 0;
    EM0D_FREE_P->Dust_timer = 0;
    EM0D_FREE_P->Swallow_wait = 0;
    pSub = (VOID*)&MAIN.Enemy[0].Be_flg;  // untyped pointer, the callee takes the address as is
    G.Enemy_num += 0xC;
    pP_ptr = G.pEm->pSin_parts_ptr;
    ScratchWord[0] = 0xD;
    pSub += 12;
    while ((ScratchWord[0] = ScratchWord[0] - 1) != 0)
    {
        FUNC(MEMCPY)(pSub, G.pEm, 0x18C);  // Memcpy
        pSub->pTgt = (ENEMY_WORK*)(&pP_ptr[ScratchWord[0]] + 2);  // reinterprets the memory as ENEMY_WORK
        pSub->Pos_x = (&pP_ptr[ScratchWord[0]] + 2)->Workm.t[0];
        pSub->Pos_y = (&pP_ptr[ScratchWord[0]] + 2)->Workm.t[1];
        pSub->Pos_z = (&pP_ptr[ScratchWord[0]] + 2)->Workm.t[2];
        pSub->Old_x = G.pEm->Pos_x;
        pSub->Old_y = G.pEm->Pos_y;
        pSub->Old_z = G.pEm->Pos_z;
        pSub->Type = 1;
        pSub->Life = -1;
        pSub->Be_flg = pSub->Be_flg | 0x64;
        if ((G.pEm->Type & 0xF) == 2 || (G.pEm->Type & 0xF) == 6)
        {
            pSub->Be_flg = pSub->Be_flg & 0xFB;
        }
        pSub--;
    }
    Em0d_joint_set();
    if (!(G.pEm->Type & 0xF) || (G.pEm->Type & 0xF) == 4)
    {
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_WALL;
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 5;
        if (G.pEm->Type & 0x80)
        {
            G.pEm->Routine_0 = 4;
        }
        return;
    }
    EM0D_FREE_P->form = 1;
    if (G.pEm->Id == 0xD)
    {
        FUNC(FLG_ON)(&MAIN.SaveData.Scenario_flg[0], 0x10);  // Flg_on
    }
    if (G.pEm->Type & 0x80)
    {
        G.pEm->Routine_0 = 4;
    }
}

VOID Em0d_Main(VOID)
{
    PARTS_W* pP_ptr;
    ENEMY_WORK* pSub;

    if ((G.Stop_flg & STOP_ENEMY_MOVE) && !(G.pEm->Type & 1))
    {
        Em0d_routine_0[G.pEm->Routine_0]();
        if (EM0D_FREE_P->Swallow_wait != 0)
        {
            EM0D_FREE_P->Swallow_wait--;
        }
    }
    if (G.pEm->Type == 1)
    {
        pP_ptr = (PARTS_W*)G.pEm->pTgt;  // reinterprets the memory as PARTS_W
        G.pEm->Pos_x = pP_ptr->Workm.t[0];
        G.pEm->Pos_y = pP_ptr->Workm.t[1];
        G.pEm->Pos_z = pP_ptr->Workm.t[2];
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        FUNC(EM_CK_HANI)(0xFA0);  // Em_ck_hani
        if (G.pEm->Pos_y < -0x5DC)
        {
            G.pEm->Be_flg = G.pEm->Be_flg | 0xC0;
        }
        if (G.pEm->Damage_no & 7)
        {
            G.pEm->Damage_no--;
            ScratchPad->Vec.vx = 0;
            ScratchPad->Vec.vy = -0x12C;
            if (pP_ptr->Workm.t[1] < -0x5DC)
            {
                ScratchPad->Vec.vy = 0x12C;
            }
            ScratchPad->Vec.vz = 0;

            FUNC(ESP_CALL)(0, 8U, 0U, &pP_ptr->Workm, &ScratchPad->Vec, 0);  // Esp_call

            if ((G.pEm->Damage_no & 0x78) != 0x40 && (G.pEm->Damage_no & 0x78) != 0x50)
            {
            }
            else
            {
                G.Enemy[0].Life -= 0x41;

                G.Enemy[0].Cdir_y += FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x10U);  // Goto00_ck
                if (G.Enemy[0].Life < 0)
                {
                    ENEMY_ROUTINE_WORD(0) = 3;
                }
            }
            if (!(G.pEm->Damage_no & 7))
            {
                G.Enemy[0].Life -= 0xF;

                if (((PARTS_W*)G.pEm->pTgt)->Parts_no < 0xAU)  // reinterprets the memory as PARTS_W
                {
                    G.Enemy[0].Life = G.Enemy[0].Life;
                }
                G.Enemy[0].Cdir_y += FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x10U);  // Goto00_ck
                if (G.Enemy[0].Life < 0)
                {
                    ENEMY_ROUTINE_WORD(0) = 3;
                }
            }
            G.pEm->Damage_no = G.pEm->Damage_no & 0x87;
        }
    }
    ScratchWord[0] = FUNC(SCA_CK_HIT)(&G.pEm->Pos_x, G.pEm->pAt_tbl->At_w);  // Sca_ck_hit
    if (G.pEm->Type == 1 && ScratchWord[0] != 0)
    {
        ScratchWord[6] = pP_ptr->Parts_no;
        {
            typedef INT SCOPE_MARK;
            if (pP_ptr->Parts_no == 3)
            {
                ScratchWord[1] = pP_ptr->Workm.t[0] - G.pEm->Pos_x;
                ScratchWord[2] = pP_ptr->Workm.t[2] - G.pEm->Pos_z;
                ScratchWord[7] = 4;
                while ((ScratchWord[7] = ScratchWord[7] - 1) != 0)
                {
                    (pP_ptr - ScratchWord[7])->Workm.t[0] -= ScratchWord[1];
                    (pP_ptr - ScratchWord[7])->Workm.t[2] -= ScratchWord[2];
                }
                pP_ptr[1].Workm.t[0] -= ScratchWord[1];
                pP_ptr[1].Workm.t[2] -= ScratchWord[2];
                pP_ptr->Workm.t[0] = G.pEm->Pos_x;
                pP_ptr->Workm.t[2] = G.pEm->Pos_z;
                ScratchWord[6] = 4;
            }
            else
            {
                pP_ptr->Workm.t[0] = G.pEm->Pos_x;
                pP_ptr->Workm.t[2] = G.pEm->Pos_z;
                if (pP_ptr->Parts_no >= 6U && EM0D_FREE_P->form == 0)
                {
                    Em0d_chain_align(&pP_ptr[-1], pP_ptr);
                }
            }
        }
        for (; ScratchWord[6] < 0xF; ScratchWord[6]++, pP_ptr++)
        {
            Em0d_chain_follow(&pP_ptr[-1], pP_ptr, 0x30);
        }
        pP_ptr = (PARTS_W*)G.pEm->pTgt;  // reinterprets the memory as PARTS_W
        ScratchPtr[8] = G.pEm;
        G.pEm = (VOID*)&MAIN.Enemy[0].Be_flg;  // untyped pointer, the callee takes the address as is
        if (pP_ptr->Parts_no == 4 && EM0D_FREE_P->Swallow_scale != 0 && EM0D_FREE_P->form == 0)
        {
            ScratchPad->Vec.vx = ScratchPad->Vec.vy = ScratchPad->Vec.vz = EM0D_FREE_P->Swallow_scale - 0x12C;
            FUNC(SCALE_MATRIX)(&pP_ptr[-1].Workm, &ScratchPad->Vec);  // ScaleMatrix
        }
        if (pP_ptr->Parts_no == 3 && EM0D_FREE_P->Swallow_scale != 0)
        {
            ScratchPad->Vec.vx = ScratchPad->Vec.vy = ScratchPad->Vec.vz = EM0D_FREE_P->Swallow_scale - 0x12C;
            FUNC(SCALE_MATRIX)(&pP_ptr->Workm, &ScratchPad->Vec);  // ScaleMatrix
        }
        G.pEm = ScratchPtr[8];
    }
    G.pEm->Zone_in_flg = G.pEm->Zone_in_flg & 0x80;
    G.pEm->Zone_in_flg = G.pEm->Zone_in_flg | FUNC(HIT_CK_POINT4)(&G.pEm->Pos_x, G.Cccut);  // Hit_ck_point4
    if (G.pEm->Type == 1)
    {
        pP_ptr = (PARTS_W*)G.pEm->pTgt;  // reinterprets the memory as PARTS_W
        pSub = MAIN.Enemy;
        if (pSub->Life < 0)
        {
            G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
            G.pEm->Damage_no = 0x80U;
        }
        if (pP_ptr->Parts_no == 0xE)
        {
            ScratchWord[0] = 0xC;
            pP_ptr = pSub->pSin_parts_ptr;
            while ((ScratchWord[0] = ScratchWord[0] - 1) != -1)
            {
                if (pSub->Zone_in_flg & 0x7F)
                {
                    FUNC(MUL_KAGE)(pP_ptr->Workm.t, &pSub->Kage_work, 0U, pSub->Cdir_y);  // MulKage
                }
                pSub++;
                pP_ptr = (PARTS_W*)pSub->pTgt;  // reinterprets the memory as PARTS_W
            }
        }
    }
}

VOID Em0d_Move(VOID)
{
    if (G.pEm->Type & 0x80)
    {
        return;
    }
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    G.pEm->Be_flg = G.pEm->Be_flg | 0x40;
    if (FUNC(EM_CK_ALERT)(0xFA0U) < 0xFA0U)  // Em_ck_alert
    {
        G.pEm->Be_flg = G.pEm->Be_flg & 0xBF;
    }
    if (EM0D_FREE_P->form == 0)
    {
        G.pEm->Be_flg = G.pEm->Be_flg & 0x7F;
        FUNC(EM_CK_HANI)(0xFA0U);  // Em_ck_hani
    }
    if (G.pEm->Routine_1 == 0)
    {
        Em0d_Move_br();
    }
    Em0d_Move_act();
    EM0D_FREE_P->saved_state = EM_ROUTINE_WORD;
    if (EM0D_FREE_P->Dust_timer != 0)
    {
        if (!(EM0D_FREE_P->Dust_timer-- & 7))
        {
            Em0d_dust_set(Api_rand() & 1);  // rand
        }
    }
}

VOID Em0d_Damage(VOID)
{
    if (EM0D_FREE_P->Bite_dir != 0)
    {
        EM_ROUTINE_WORD = EM0D_FREE_P->saved_state;
        return;
    }
    if (EM0D_FREE_P->Bite_wait != 0)
    {
        EM_ROUTINE_WORD = EM0D_FREE_P->saved_state;
        return;
    }
    if (G.pEm->Routine_1 == 0)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 0, 1, 2);  // state 2, Key_rno 1, Routine_2 0, Act_sts 0
    }
    Em0d_dm00();
}

VOID Em0d_Die(VOID)
{
    if (G.pEm->Routine_1 == 0)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 0, 1, 3);  // state 3, Key_rno 1, Routine_2 0, Act_sts 0
    }
    Em0d_die00();
}

VOID Em0d_Wait(VOID)
{
    if (!(G.pEm->Type & 0x80))
    {
        G.pEm->Routine_0 = 1;
    }
}

VOID Em0d_Move_act(VOID)
{
    Em0d_mv_tbl[G.pEm->Routine_2]();
}

VOID Em0d_Move_br(VOID)
{
    ScratchWord[0] = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);
    Em0d_br_tbl[(G.pEm->Type & 7)]();
}

VOID Em0d_br_type00(VOID)
{
    INT unused;
    FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x30);  // Goto00
    if (G.Player.Damage_no == 0)
    {
        ScratchWord[1] = FUNC(SIKAI_CK)(&MAIN.Player.Pos_x);  // Sikai_ck

        if (ScratchWord[0] < 0x7D0 && ScratchWord[1] == 0 && G.Player.Life >= 0xA)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 2, 1, 1);  // state 1, Key_rno 1, Routine_2 2, Act_sts 0
        }

        if (ScratchWord[0] < EM0D_FREE_P->form * 0x3E8 + 0x1388 && ScratchWord[1] == 0 && G.Player.Life >= 0xA)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200) == 0)  // Goto00_ck
            {
                EM_ROUTINE_WORD = ROUTINES(0, 2, 1, 1);  // state 1, Key_rno 1, Routine_2 2, Act_sts 0
            }
        }
        if (ScratchWord[0] < 0x1B58 && EM0D_FREE_P->Bite_cnt >= 5)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);  // state 1, Key_rno 1, Routine_2 3, Act_sts 0
        }
        if (ScratchWord[0] < 0xFA0 && EM0D_FREE_P->form != 0 && ScratchWord[1] == 0)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x40) == 0 && G.Player.Life - 0xA < 0 && EM0D_FREE_P->Swallow_wait == 0)  // Goto00_ck
            {
                EM_ROUTINE_WORD = ROUTINES(0, 4, 1, 1);  // state 1, Key_rno 1, Routine_2 4, Act_sts 0
            }
        }

        if (G.Player.Life - 0xA < 0 && EM0D_FREE_P->form == 0)
        {
            EM0D_FREE_P->Pl_low_flg = 1;
            EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);  // state 1, Key_rno 1, Routine_2 3, Act_sts 0
        }
        if (G.pEm->Pos_z >= 0x4269 && EM0D_FREE_P->form != 0 && G.pEm->Routine_2 == 2)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);  // state 1, Key_rno 1, Routine_2 3, Act_sts 0

            EM0D_FREE_P->Pl_low_flg = 0;
        }
        if (EM0D_FREE_P->wall_frames >= 0x97U)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 8, 1, 1);  // state 1, Key_rno 1, Routine_2 8, Act_sts 0
        }
        if (G.Player.Sca_info & 8)
        {
            if (EM0D_FREE_P->form != 0 && G.Player.Life - 0xA >= 0)
            {
                EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);  // state 1, Key_rno 1, Routine_2 3, Act_sts 0

                EM0D_FREE_P->Pl_low_flg = 0;
            }
        }
        else if (G.Player.Life >= 0xA)
        {
        }
        if (G.pEm->Life < 0xAF0)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 7, 1, 1);  // state 1, Key_rno 1, Routine_2 7, Act_sts 0
        }
    }
    Em0d_sway_add();
}

VOID Em0d_br_type01(VOID)
{
    INT unused;
    FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x30);  // Goto00
    if (G.Player.Damage_no == 0)
    {
        ScratchWord[1] = FUNC(SIKAI_CK)(&MAIN.Player.Pos_x);  // Sikai_ck

        if (ScratchWord[0] < 0x7D0 && ScratchWord[1] == 0 && G.Player.Life >= 0xA)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 2, 1, 1);  // state 1, Key_rno 1, Routine_2 2, Act_sts 0
        }

        if (ScratchWord[0] < EM0D_FREE_P->form * 0x3E8 + 0x1388 && ScratchWord[1] == 0 && G.Player.Life >= 0xA)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200) == 0)  // Goto00_ck
            {
                EM_ROUTINE_WORD = ROUTINES(0, 2, 1, 1);  // state 1, Key_rno 1, Routine_2 2, Act_sts 0
            }
        }
        if (ScratchWord[0] < 0x1B58 && EM0D_FREE_P->Bite_cnt >= 5)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);  // state 1, Key_rno 1, Routine_2 3, Act_sts 0
        }
        if (ScratchWord[0] < 0xFA0 && EM0D_FREE_P->form != 0 && ScratchWord[1] == 0)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x40) == 0 && G.Player.Life - 0xA < 0 && EM0D_FREE_P->Swallow_wait == 0)  // Goto00_ck
            {
                EM_ROUTINE_WORD = ROUTINES(0, 4, 1, 1);  // state 1, Key_rno 1, Routine_2 4, Act_sts 0
            }
        }

        if (G.Player.Life - 0xA < 0 && EM0D_FREE_P->form == 0)
        {
            EM0D_FREE_P->Pl_low_flg = 1;
            EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);  // state 1, Key_rno 1, Routine_2 3, Act_sts 0
        }
    }
    Em0d_sway_add();
}

VOID Em0d_mv00(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 1;
        G.pEm->Hokan_flg = 7;
        EM0D_FREE_P->Move_spd = 0;
        G.pEm->Timer0 = (Api_rand() & 0xF) + 0xF;  // rand
    }
    Em0d_anm_step(0, 0x200);
    if (G.pEm->Timer0-- == 0)
    {
        EM_ROUTINE_HALF2 = 1;
    }
    Em0d_Move_ck(0x30);
}

VOID Em0d_mv01(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = EM0D_FREE_P->form + 1;
        G.pEm->Hokan_flg = 7;
        EM0D_FREE_P->Move_spd = 0;
        G.pEm->Spd_cur = 0xA0;

        if (G.Player.Life - 0xA >= 0 && EM0D_FREE_P->Pl_low_flg == 0 && !(EM0D_FREE_P->Bite_wait & 0x80))
        {
        }
        else if (EM0D_FREE_P->form != 0)
        {
            G.pEm->Move_no = 5U;
        }
    }
    if (EM0D_FREE_P->Bite_wait & 0x7F)
    {
        EM0D_FREE_P->Bite_wait--;
        if (EM0D_FREE_P->Bite_wait == 0)
        {
            EM0D_FREE_P->Bite_wait = 0;
            G.pEm->Routine_3 = 0U;
        }
    }
    if (G.pEm->Move_no == 5)
    {
        if (G.pEm->Move_cnt != 0xF && G.pEm->Move_cnt != 0x28)
        {
        }
        else
        {
            FUNC(SND_SE_ENEM)(1, 0);  // Snd_se_enem
        }
    }
    Em0d_anm_step(0, 0x200);
    if (EM0D_FREE_P->Dust_timer != 0 && !(G.pEm->Move_cnt & 7))
    {
        Em0d_dust_set(Api_rand() & 1);  // rand
    }
    FUNC(ADD_SPEED_XZ)(EM0D_FREE_P->Move_spd);  // Add_speedXZ
    Em0d_Move_ck(0x30);
}

VOID Em0d_mv02(VOID)
{
    PARTS_W* pP_ptr;
    UCHAR Bite_frm;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = EM0D_FREE_P->form * 0xA + 3;
        G.pEm->Spd_cur = 0x64;
        EM0D_FREE_P->Bite_cnt++;
        EM0D_FREE_P->Bite_dir = 1;
        FUNC(SND_SE_ENEM)(2, 0);  // Snd_se_enem

    case 1:
        Bite_frm = G.pEm->Move_cnt - Em0d_bite_tbl[EM0D_FREE_P->form].first_frame;
        if (Bite_frm < Em0d_bite_tbl[EM0D_FREE_P->form].Frm_num)
        {
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vx = 0x3E8;
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchWord[1] = FUNC(L_POS_CK_XZM)(&pP_ptr->Workm, &ScratchPad->Vec, 0x320, &MAIN.Player.Pos_x);  // L_pos_ckXZM
            if (G.Player.Routine_2 >= 0x66U && (G.pEm->Move_cnt & 1))
            {
                FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);  // Esp_call

                ScratchPad->Vec.vx = 0;
                ScratchPad->Vec.vy = -0x12C - EM0D_FREE_P->form * 0x5DC;

                FUNC(ESP_CALL)(0, 0, 0, &MAIN.Player.M, &ScratchPad->Vec, 0);  // Esp_call
            }
            if (ScratchWord[1] != 0 && G.Player.Damage_no == 0)
            {
                ScratchWord[7] = FUNC(CDIR_CK)(&MAIN.Player);  // Cdir_ck

                G.Player.Life -= 0x1C;

                if (G.Player.Life < 0)
                {
                    G.Player.Life = 1;
                }
                if (G.Player.Life >= 0)
                {
                    G.Player.Damage_no = ScratchWord[7] + 1;
                    G.Player.Routine_2 = ScratchWord[7] + 0x66;
                }

                FUNC(PAD_VIB_SET0)(5, 0);  // Pad_vib_set0
                FUNC(PAD_VIB_SET1)(5, 0xC8, 0);  // Pad_vib_set1

                ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
                ScratchPad->Vec.vx = 0x3E8;
                pP_ptr = G.pEm->pSin_parts_ptr;

                FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);  // Esp_call

                FUNC(MODEL_COLOR_PLUS)(&pP_ptr[1], 0x30, 0x80820, 0x606060);  // Model_color_plus
                G.pEm->Cdir_y = G.pEm->Cdir_y + 0x100;
                if (G.pEm->Id == 0xD)
                {
                    if (FUNC(FLG_CK)(&MAIN.SaveData.Scenario_flg, 0x47) == 0)  // Flg_ck
                    {
                        FUNC(FLG_ON)(&MAIN.SaveData.Scenario_flg2, 0x43);  // Flg_on
                        G.Player.Sca_info |= 0x20;
                    }
                }
                EM0D_FREE_P->Bite_dir = -1;
                EM0D_FREE_P->Bite_wait = 0;
                EM0D_FREE_P->wall_frames = 0;
                EM0D_FREE_P->Dust_timer = 0xF0;
                FUNC(SND_SE_ENEM)(4, 0);  // Snd_se_enem
            }
        }
        G.pEm->Routine_3 += Em0d_anm_step(0, 0x200);
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x38);  // Goto00
        if (G.pEm->Move_cnt == EM0D_FREE_P->form * 5 + 8)
        {
            if (EM0D_FREE_P->form != 0 && EM0D_FREE_P->Wall_wait >= 0xBU)
            {
                G.pEm->Routine_1 = 0;
                EM_ROUTINE_HALF2 = 1;
                EM0D_FREE_P->Bite_dir = 0;
                EM0D_FREE_P->Bite_cnt = 5;
                G.pEm->Damage_no = 0;
                FUNC(GOTO00)(&MAIN.Player.Pos_x, -0x10);  // Goto00
                break;
            }
            G.pEm->Spd_cur = 0xC8;
            FUNC(SND_SE_ENEM)(3, 0);  // Snd_se_enem
        }
        break;
    case 2:
        G.pEm->Timer0 = 0x1EU;
        G.pEm->Spd_cur = 0x78;
        G.pEm->Routine_3 = EM0D_FREE_P->form + 3;

    case 3:
        FUNC(GOTO00)(&MAIN.Player.Pos_x, EM0D_FREE_P->Bite_dir * 0x10);  // Goto00
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_1 = 0;
            EM_ROUTINE_HALF2 = 1;
            EM0D_FREE_P->Bite_dir = 0;
            G.pEm->Damage_no = 0;
        }
        break;
    case 4:
        G.pEm->Routine_3 = 5U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 4;
        G.pEm->Hokan_flg = 7;
        G.pEm->Spd_cur = 0x78;

    case 5:
        FUNC(GOTO00)(&MAIN.Player.Pos_x, EM0D_FREE_P->Bite_dir * 0x10);  // Goto00
        if (Em0d_anm_step(0, 0x200) & 0xFF)
        {
            G.pEm->Routine_1 = 0;
            EM_ROUTINE_HALF2 = 1;
            EM0D_FREE_P->Bite_dir = 0;
            G.pEm->Damage_no = 0;
        }
    }
    FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
    Em0d_Move_ck(0x30);
}

VOID Em0d_mv03(VOID)
{
    INT unused;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 4;
        G.pEm->Hokan_flg = 7;
        G.pEm->Spd_cur = 0x78;
        EM0D_FREE_P->Bite_cnt = 0;
        if (EM0D_FREE_P->Pl_low_flg != 0 && EM0D_FREE_P->form != 0)
        {
            G.pEm->Routine_3 = 3;
            break;
        }

    case 1:
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x30);  // Goto00
        G.pEm->Routine_3 = G.pEm->Routine_3 + Em0d_anm_step(EM0D_FREE_P->form, 0x200);
        break;
    case 2:
        if (EM0D_FREE_P->form == 0)
        {
            EM0D_FREE_P->form = 1;
            G.pEm->Routine_3 = 3;
        }
        else
        {
            EM0D_FREE_P->form = 0;
            G.pEm->Routine_1 = 0;
            EM_ROUTINE_HALF2 = 1;
        }
        break;
    case 3:
        G.pEm->Routine_3 = 4;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 5;
        G.pEm->Hokan_flg = 7;
        G.pEm->Spd_cur = 0x46;
        G.pEm->Timer0 = 0x3C;

    case 4:
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x30);  // Goto00
        Em0d_anm_step(EM0D_FREE_P->form, 0x200);
        if (G.pEm->Move_cnt == 0xF || G.pEm->Move_cnt == 0x28)
        {
            FUNC(SND_SE_ENEM)(1, 0);  // Snd_se_enem
        }
        if (!(G.pEm->Timer0--))  // signed conversion (unsigned operand)
        {
            G.pEm->Routine_1 = 0;
            EM_ROUTINE_HALF2 = 1;
        }
    }
    Em0d_sway_add();
    FUNC(ADD_SPEED_XZ)(EM0D_FREE_P->Move_spd);  // Add_speedXZ
    Em0d_Move_ck(0x30);
}

VOID Em0d_mv04(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 0xE;
        ++EM0D_FREE_P->Bite_cnt;
        G.pEm->Damage_no = 1;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
        G.Player.Be_flg |= EM_BE_NO_HIT;
        G.Player.Be_flg |= EM_BE_NO_WALL;
        PL_ROUTINE_WORD = ROUTINES(0, 0, 0xD, 7);
        EM0D_FREE_P->Swallow_wait = 0x1E;

    case 1:
        Em0d_anm_step(0, 0x200);
        ScratchWord[0] = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);
        G.pEm->Spd_cur = 0xA0;
        if (ScratchWord[0] < 0x9C4)
        {
            G.pEm->Spd_cur = 0;
        }
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x30);  // Goto00
        if (G.pEm->Move_cnt == 0xA)
        {
            FUNC(SND_SE_ENEM)(3, 0);  // Snd_se_enem
        }
        if (G.pEm->Move_cnt == 0x14)
        {
            G.pEm->Damage_no = 0;
            if (ScratchWord[0] >= 0x7D1)
            {
                PL_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
                G.pEm->Routine_1 = 0;
                EM_ROUTINE_HALF2 = 1;
                G.Player.Be_flg &= ~EM_BE_NO_HIT;
                G.Player.Be_flg &= ~EM_BE_NO_WALL;
                G.Player.Damage_no = 0;
                break;
            }
            {
                G.pEm->Routine_3 = 2;

                G.pEm->Damage_no = 1;

                ScratchWord[5] = 0xD;
                while ((ScratchWord[5] = ScratchWord[5] - 1) != -1)
                {
                    (MAIN.Enemy + ScratchWord[5])->Be_flg |= 4;
                }

                pP_ptr = G.Player.pSin_parts_ptr;
                pP_ptr->Be_flg &= 0xFE;
                pP_ptr[1].Be_flg &= 0xFE;
                pP_ptr[9].Be_flg &= 0xFE;
                pP_ptr[12].Be_flg &= 0xFE;
                G.pEm->Spd_cur = 0;
                ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
                ScratchPad->Vec.vy = 0x3E8;

                FUNC(ESP_CALL)(0, 3, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);  // Esp_call
                FUNC(ESP_CALL)(0, 0, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);  // Esp_call
                FUNC(ESP_CALL)(0, 3, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0);  // Esp_call
                FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0);  // Esp_call
                FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[3].Workm, &ScratchPad->Vec, 0);  // Esp_call
                FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[6].Workm, &ScratchPad->Vec, 0);  // Esp_call

                FUNC(MODEL_COLOR_PLUS)(&pP_ptr[3], 0x70, 0x484860, 0xA0A0A0);  // Model_color_plus
                FUNC(MODEL_COLOR_PLUS)(&pP_ptr[4], 0x70, 0x484860, 0xA0A0A0);  // Model_color_plus
                FUNC(MODEL_COLOR_PLUS)(&pP_ptr[5], 0x70, 0x484860, 0xA0A0A0);  // Model_color_plus
                FUNC(MODEL_COLOR_PLUS)(&pP_ptr[6], 0x70, 0x484860, 0xA0A0A0);  // Model_color_plus
                FUNC(MODEL_COLOR_PLUS)(&pP_ptr[7], 0x70, 0x484860, 0xA0A0A0);  // Model_color_plus
                FUNC(MODEL_COLOR_PLUS)(&pP_ptr[8], 0x70, 0x484860, 0xA0A0A0);  // Model_color_plus
                FUNC(SND_SE_ENEM)(5, 0);  // Snd_se_enem

                FUNC(SND_SE_STAD)(2, (G.Player.Id & 1) + 0x17, 0, &G.pEm->Pos_x);  // Snd_se_stad

                FUNC(PAD_VIB_SET0)(6, 0);  // Pad_vib_set0
                FUNC(PAD_VIB_SET0)(7, 0xA);  // Pad_vib_set0
                FUNC(PAD_VIB_SET0)(6, 0x16);  // Pad_vib_set0
                FUNC(PAD_VIB_SET1)(0xF, 0xFA, 0);  // Pad_vib_set1
                FUNC(PAD_VIB_FADE_SET)(0x1E, 0xFA, 0, 0xF);  // Pad_vib_fade_set
            }
        }
        FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
        break;
    case 2:
        pP_ptr = G.pEm->pSin_parts_ptr;
        G.pEm->Routine_3 = 3;
        FUNC(MUL_LMATRIX)(&pP_ptr->Workm, &MAIN.Player.M[0], &Em0d_catch_m);  // MulLMatrix
        ScratchPad->Sv.vx = 0;
        ScratchPad->Sv.vy = 0;
        ScratchSv[2] = -0xC8;
        FUNC(ROT_MATRIX)(&ScratchPad->Sv, &ScratchPad->Mat);  // RotMatrix
        FUNC(MUL_MATRIX2)(&ScratchPad->Mat, &Em0d_catch_m);  // MulMatrix2
        Em0d_catch_m.t[0] = 0x937;
        Em0d_catch_m.t[1] = 0x2BC;
        Em0d_catch_m.t[2] = 0;
        EM0D_FREE_P->Swallow_scale = 0x1388U;

        FUNC(PAD_VIB_SET1)(0xF, 0xFA, 0);  // Pad_vib_set1

    case 3:
        pP_ptr = G.pEm->pSin_parts_ptr;
        G.Player.Zone_in_flg |= 0x80;
        FUNC(GS_MUL_COORD0)(&pP_ptr->Workm, &Em0d_catch_m, &MAIN.Player.M[0]);  // GsMulCoord0
        G.pEm->Routine_3 = G.pEm->Routine_3 + Em0d_anm_step(0, 0x200);
        if (G.pEm->Move_cnt >= 0x43U && G.pEm->Move_cnt < 0x4BU)
        {
            if (G.pEm->Move_cnt == 0x43)
            {
                FUNC(SND_SE_ENEM)(6, 0);  // Snd_se_enem

                FUNC(PAD_VIB_SET0)(4, 0);  // Pad_vib_set0
                FUNC(PAD_VIB_SET1)(4, 0xFA, 0);  // Pad_vib_set1
            }
            if (G.pEm->Move_cnt == 0x4A)
            {
                FUNC(SND_SE_ENEM)(7, 0);  // Snd_se_enem
                FUNC(SND_SE_ENEM)(4, 0);  // Snd_se_enem

                FUNC(PAD_VIB_SET1)(0xF, 0xC8, 0);  // Pad_vib_set1
                FUNC(PAD_VIB_FADE_SET)(0x2D, 0xC8, 0, 0xF);  // Pad_vib_fade_set
                FUNC(PAD_VIB_SET0)(8, 0);  // Pad_vib_set0
                FUNC(PAD_VIB_SET0)(6, 0x10);  // Pad_vib_set0
            }
            Em0d_catch_m.t[0] -= 0x78;
            Em0d_catch_m.t[1] -= 0x28;
            pP_ptr = G.Player.pSin_parts_ptr;
            pP_ptr[2].Be_flg &= 0xFE;
            pP_ptr[10].Be_flg &= 0xFE;
            pP_ptr[13].Be_flg &= 0xFE;
            pP_ptr[11].Be_flg &= 0xFE;
            pP_ptr[14].Be_flg &= 0xFE;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0x1F4;

            FUNC(ESP_CALL)(0, 3, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);  // Esp_call
            FUNC(ESP_CALL)(0, 0, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);  // Esp_call
            FUNC(ESP_CALL)(0, 3, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0);  // Esp_call
            FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0);  // Esp_call
            FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[3].Workm, &ScratchPad->Vec, 0);  // Esp_call
            FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[6].Workm, &ScratchPad->Vec, 0);  // Esp_call

            ScratchPad->Sv.vx = 0;
            ScratchPad->Sv.vy = ((G.pEm->Cdir_y - (SHORT)G.Player.Cdir_y + 0x800) & 0xFFF) / 8;  // sign-extends the halfword
            ScratchSv[2] = 0;
            FUNC(ROT_MATRIX)(&ScratchPad->Sv, &ScratchPad->Mat);  // RotMatrix
            FUNC(MUL_MATRIX)(&Em0d_catch_m, &ScratchPad->Mat);  // MulMatrix
            EM0D_FREE_P->Swallow_scale = EM0D_FREE_P->Swallow_scale + 0x64;
        }
        break;
    case 4:
        G.pEm->Routine_3 = 5;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 4;
        G.pEm->Spd_cur = 0;
        G.Player.Life = -1;

    case 5:
        G.pEm->Routine_3 += Em0d_anm_step(1, 0x200);
        FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
        if (G.pEm->Move_cnt == 0xF)
        {
            ScratchWord[0] = 0xD;
            while ((ScratchWord[0] = ScratchWord[0] - 1) != -1)
            {
                (MAIN.Enemy + ScratchWord[0])->Be_flg |= 4;
            }
        }
        break;
    case 6:
        G.pEm->Routine_3 = 7;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 1;
        G.pEm->Spd_cur = 0;
        G.pEm->Timer0 = 0x96;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_WALL;

    case 7:
        Em0d_catch_m.t[0] -= 0xA;
        Em0d_catch_m.t[1] -= 6;
        Em0d_anm_step(0, 0x200);
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 8;
        }
        if (G.pEm->Timer0 == 0x6E)
        {
            pP_ptr = G.Player.pSin_parts_ptr;
            pP_ptr[3].Be_flg &= 0xFE;
            pP_ptr[4].Be_flg &= 0xFE;
            pP_ptr[5].Be_flg &= 0xFE;
            pP_ptr[6].Be_flg &= 0xFE;
            pP_ptr[7].Be_flg &= 0xFE;
            pP_ptr[8].Be_flg &= 0xFE;
        }
        break;
    case 8:;
    }
    Em0d_Move_ck(0x30);
    if (G.pEm->Routine_3 >= 2U)
    {
        if (!(Api_rand() & 1))  // rand
        {
            Em0d_dust_set(0);
        }
        if (!(Api_rand() & 1))  // rand
        {
            Em0d_dust_set(1);
        }
        if (!(Api_rand() & 1))  // rand
        {
            Em0d_dust_set(2);
        }
    }
    if (EM0D_FREE_P->Swallow_scale != 0)
    {
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec.vx = ScratchPad->Vec.vy = ScratchPad->Vec.vz = EM0D_FREE_P->Swallow_scale - 0x12C;
        FUNC(SCALE_MATRIX)(&pP_ptr[3].Workm, &ScratchPad->Vec);  // ScaleMatrix
        ScratchPad->Vec.vx = ScratchPad->Vec.vy = ScratchPad->Vec.vz = EM0D_FREE_P->Swallow_scale;
        FUNC(SCALE_MATRIX)(&pP_ptr[2].Coord, &ScratchPad->Vec);  // ScaleMatrix
    }
}

VOID Em0d_mv05(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 1;
        G.pEm->Hokan_flg = 7;
        EM0D_FREE_P->Move_spd = 0;
        G.pEm->Spd_cur = 0xBE;
        if (G.pEm->Type == 4)
        {
            G.pEm->Spd_cur = 0x82;
            EM0D_FREE_P->Way_idx = 3;
        }
        G.pEm->Timer0 = 2U;
        G.pEm->Damage_no = 1;
        if (G.pEm->Id == 0xD)
        {
            FUNC(FLG_ON)(&MAIN.SaveData.Scenario_flg, 0x10);  // Flg_on
        }
    }
    ScratchPad->Vec.vx = Em0d_entry_tbl[EM0D_FREE_P->Way_idx].x;
    ScratchPad->Vec.vy = 0;
    ScratchPad->Vec.vz = Em0d_entry_tbl[EM0D_FREE_P->Way_idx].z;
    FUNC(GOTO00)(&ScratchPad->Vec, 0x30);  // Goto00
    if (FUNC(SQUARE_ROOT0)((ScratchPad->Vec.vx - G.pEm->Pos_x) * (ScratchPad->Vec.vx - G.pEm->Pos_x) + (ScratchPad->Vec.vz - G.pEm->Pos_z) * (ScratchPad->Vec.vz - G.pEm->Pos_z)) < 0x1F4U)  // SquareRoot0
    {
        if (G.pEm->Timer0-- == 0)
        {
            EM_ROUTINE_HALF2 = 6;
        }
        EM0D_FREE_P->Way_idx++;
        FUNC(SND_SE_ENEM)(0, 0);  // Snd_se_enem
    }
    if (G.pEm->Type & 0x3F)
    {
        ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x2904;  // rand
        ScratchPad->Vec.vy = (Api_rand() & 0x3FF) * -1;  // rand
        ScratchPad->Vec.vz = (Api_rand() & 0x3FF) + 0x2904;  // rand

        FUNC(ESP_CALL)(9, 0x11, 0, Monitor.pId_mat, &ScratchPad->Vec, 0);  // Esp_call
    }
    else
    {
        ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x10CC;  // rand

        ScratchPad->Vec.vy = (Api_rand() & 0x3FF) * -1;  // rand
        ScratchPad->Vec.vz = (Api_rand() & 0x3FF) + 0x5C94;  // rand

        FUNC(ESP_CALL)(9, 0x11, 0, Monitor.pId_mat, &ScratchPad->Vec, 0);  // Esp_call
    }
    Em0d_anm_step(0, 0x200);
    FUNC(ADD_SPEED_XZ)(EM0D_FREE_P->Move_spd);  // Add_speedXZ
    Em0d_Move_ck(0x30);
}

VOID Em0d_mv06(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 4;
        G.pEm->Hokan_flg = 7;
        G.pEm->Spd_cur = 0;
        EM0D_FREE_P->form = 1;
        EM0D_FREE_P->Way_idx = 0;

    case 1:
        if (Em0d_anm_step(0U, 0x200) & 0xFF)
        {
            G.pEm->Routine_3 = 2;
            if (G.pEm->Type == 0)
            {
                G.pEm->Routine_3 = 6;
            }
        }
        return;
    case 2:
        G.pEm->Routine_3 = 3;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = EM0D_FREE_P->form * 0xA + 3;

        FUNC(PAD_VIB_SET1)(5, 0xB4, 0x12);  // Pad_vib_set1
        FUNC(PAD_VIB_FADE_SET)(0x1E, 0xB4, 0, 0x17);  // Pad_vib_fade_set

    case 3:
        G.pEm->Routine_3 += Em0d_anm_step(0U, 0x200);
        if (G.pEm->Move_cnt == EM0D_FREE_P->form * 5 + 8)
        {
            FUNC(SND_SE_ENEM)(3, 0);  // Snd_se_enem
            G.pEm->Spd_cur = 0xBE;
        }
        if (G.pEm->Move_cnt >= 0x18U)
        {
            G.pEm->Spd_cur = 0x32;
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x7D0;  // rand
            ScratchPad->Vec.vy = 0;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0xE74;  // rand

            FUNC(ESP_CALL)(9, 0x13, 0, Monitor.pId_mat, &ScratchPad->Vec, 0);  // Esp_call

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x7D0;  // rand
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0xE74;  // rand

            FUNC(ESP_CALL)(9, 0x15, 0, Monitor.pId_mat, &ScratchPad->Vec, 0);  // Esp_call

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x7D0;  // rand
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0xE74;  // rand

            FUNC(ESP_CALL)(0xE, 0x13, 0, Monitor.pId_mat, &ScratchPad->Vec, 0);  // Esp_call

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x7D0;  // rand
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0xE74;  // rand

            FUNC(ESP_CALL)(0xE, 0x11, 0, Monitor.pId_mat, &ScratchPad->Vec, 0);  // Esp_call

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x7D0;  // rand
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0xE74;  // rand

            FUNC(ESP_CALL)(9, 0x15, 0, Monitor.pId_mat, &ScratchPad->Vec, 0);  // Esp_call

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x7D0;  // rand
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0xE74;  // rand

            FUNC(ESP_CALL)(0xE, 0x13, 0, Monitor.pId_mat, &ScratchPad->Vec, 0);  // Esp_call

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x7D0;  // rand
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0xE74;  // rand

            FUNC(ESP_CALL)(0xE, 0x11, 0, Monitor.pId_mat, &ScratchPad->Vec, 0);  // Esp_call

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x7D0;  // rand
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0xE74;  // rand

            FUNC(ESP_CALL)(0xE, 0x13, 0, Monitor.pId_mat, &ScratchPad->Vec, 0);  // Esp_call

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x7D0;  // rand
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0xE74;  // rand

            FUNC(ESP_CALL)(0xE, 0x11, 0, Monitor.pId_mat, &ScratchPad->Vec, 0);  // Esp_call

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x7D0;  // rand
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0xE74;  // rand

            FUNC(ESP_CALL)(0x11, 0x11, 0, Monitor.pId_mat, &ScratchPad->Vec, 0);  // Esp_call
        }
        FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
        Em0d_Move_ck(0x30);
        return;
    case 4:
        G.pEm->Routine_3 = 5;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 4;
        G.pEm->Hokan_flg = 7;

    case 5:
        G.pEm->Routine_3 += Em0d_anm_step(0U, 0x200);
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x30);  // Goto00
        return;
    case 6:
        G.pEm->Routine_3 = 7;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 5;
        G.pEm->Hokan_flg = 7;
        G.pEm->Spd_cur = 0x46;
        G.pEm->Timer0 = 0x5A;

    case 7:
        Em0d_anm_step(EM0D_FREE_P->form, 0x200);
        if (G.pEm->Move_cnt != 0xF && G.pEm->Move_cnt != 0x28)
        {
        }
        else
        {
            FUNC(SND_SE_ENEM)(1, 0);  // Snd_se_enem
        }
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_1 = 0;
            EM_ROUTINE_HALF2 = 1;
            G.pEm->Damage_no = 0;
            ScratchWord[0] = 0xD;
            while ((ScratchWord[0] = ScratchWord[0] - 1) != -1)
            {
                (MAIN.Enemy + ScratchWord[0])->Be_flg &= 0xFB;
            }
        }
    }
}

VOID Em0d_mv07(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 1;
        G.pEm->Hokan_flg = 0xF;
        G.pEm->Spd_cur = 0xB4;
        EM0D_FREE_P->wall_frames = 0;
        EM0D_FREE_P->Way_idx = 4;
        if (G.pEm->Pos_x < 0x2328 && G.pEm->Pos_z < 0x59D8)
        {
            EM0D_FREE_P->Way_idx = 3;
        }
        if (G.pEm->Pos_z < 0x3C8C)
        {
            EM0D_FREE_P->Way_idx = 2;
        }
        if (G.pEm->Pos_z < 0x2328)
        {
            EM0D_FREE_P->Way_idx = 0;
        }
        if (G.pEm->Pos_x >= 0x2581)
        {
            EM0D_FREE_P->Way_idx = 1;
        }
        EM0D_FREE_P->form = 0;
        EM0D_FREE_P->Bite_dir = 1;
        EM0D_FREE_P->Sway_add = 1;
        EM0D_FREE_P->Move_spd = 0;
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg, G.pEm->Em_set_flg);  // Flg_on

        Em0d_flee_timer = 0;
    }

    if (EM0D_FREE_P->Way_idx >= 7U)
    {
        ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x10CC;  // rand

        ScratchPad->Vec.vy = (Api_rand() & 0x3FF) * -1;  // rand
        ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x58AC;  // rand

        FUNC(ESP_CALL)(9, 0x11U, 0, Monitor.pId_mat, &ScratchPad->Vec, 0);  // Esp_call
    }
    ScratchPad->Vec.vx = Em0d_flee_tbl[EM0D_FREE_P->Way_idx].x;
    ScratchPad->Vec.vy = 0;
    ScratchPad->Vec.vz = Em0d_flee_tbl[EM0D_FREE_P->Way_idx].z;
    FUNC(GOTO00)(&ScratchPad->Vec, 0x30U);  // Goto00
    if (FUNC(SQUARE_ROOT0)((ScratchPad->Vec.vx - G.pEm->Pos_x) * (ScratchPad->Vec.vx - G.pEm->Pos_x) + (ScratchPad->Vec.vz - G.pEm->Pos_z) * (ScratchPad->Vec.vz - G.pEm->Pos_z)) < 0x320U)  // SquareRoot0
    {
        ++EM0D_FREE_P->Way_idx;
        if (EM0D_FREE_P->Way_idx == 1)
        {
            EM0D_FREE_P->Way_idx = 2;
        }
        EM0D_FREE_P->Move_spd = 0;
        if (EM0D_FREE_P->Way_idx == 6)
        {
            ScratchWord[0] = 4;
            while ((ScratchWord[0] = ScratchWord[0] - 1) != -1)
            {
                (MAIN.Enemy + ScratchWord[0])->Be_flg |= 4;
            }
        }
        if (EM0D_FREE_P->Way_idx == 7)
        {
            ScratchWord[0] = 4;
            while ((ScratchWord[0] = ScratchWord[0] - 1) != -1)
            {
                (&MAIN.Enemy[4])[ScratchWord[0]].Be_flg |= 4;
            }
        }
        if (EM0D_FREE_P->Way_idx == 8)
        {
            ScratchWord[0] = 5;
            while ((ScratchWord[0] = ScratchWord[0] - 1) != -1)
            {
                (&MAIN.Enemy[8])[ScratchWord[0]].Be_flg |= 4;
            }
        }
        if (EM0D_FREE_P->Way_idx == 9)
        {
            G.pEm->Type = G.pEm->Type | 0x80;
        }
    }
    if (EM0D_FREE_P->wall_frames >= 0x97U)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 8, 1, 1);  // state 1, Key_rno 1, Routine_2 8, Act_sts 0
    }
    if (Em0d_flee_timer >= 0x2329)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 8, 1, 1);  // state 1, Key_rno 1, Routine_2 8, Act_sts 0
    }
    if (EM0D_FREE_P->Way_idx < 6U)
    {
        Em0d_flee_timer++;
    }
    Em0d_anm_step(0, 0x100);
    if (EM0D_FREE_P->Way_idx < 4U)
    {
        Em0d_sway_add();
    }
    FUNC(ADD_SPEED_XZ)(EM0D_FREE_P->Move_spd);  // Add_speedXZ
    Em0d_Move_ck(0x38);
}

VOID Em0d_mv08(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 1;
        G.pEm->Hokan_flg = 0xF;
        G.pEm->Spd_cur = 0xB4;
        EM0D_FREE_P->wall_frames = 0;
        EM0D_FREE_P->Way_idx = 3;
        if (G.pEm->Pos_x >= 0x2329)
        {
            EM0D_FREE_P->Way_idx = 2;
        }
        if (G.pEm->Pos_z < 0x4A38)
        {
            EM0D_FREE_P->Way_idx = 1;
        }
        if (G.pEm->Pos_z < 0x3A98)
        {
            EM0D_FREE_P->Way_idx = 0;
        }
        EM0D_FREE_P->form = 0;
        EM0D_FREE_P->Bite_dir = 1;
        EM0D_FREE_P->Sway_add = 1;
        EM0D_FREE_P->Move_spd = 0;
    }
    if (EM0D_FREE_P->wall_frames >= 0x97U)
    {
        G.pEm->Routine_3 = 0U;
    }
    ScratchPad->Vec.vx = Em0d_repos_tbl[EM0D_FREE_P->Way_idx].x;
    ScratchPad->Vec.vy = 0;
    ScratchPad->Vec.vz = Em0d_repos_tbl[EM0D_FREE_P->Way_idx].z;
    FUNC(GOTO00)(&ScratchPad->Vec, 0x30);  // Goto00
    if (FUNC(SQUARE_ROOT0)((ScratchPad->Vec.vx - G.pEm->Pos_x) * (ScratchPad->Vec.vx - G.pEm->Pos_x) + (ScratchPad->Vec.vz - G.pEm->Pos_z) * (ScratchPad->Vec.vz - G.pEm->Pos_z)) < 0x320U)  // SquareRoot0
    {
        EM0D_FREE_P->Move_spd = 0;
        EM_ROUTINE_WORD = ROUTINES(0, 1, 0, 1);  // state 1, Key_rno 0, Routine_2 1, Act_sts 0
    }
    Em0d_anm_step(0, 0x100);
    Em0d_sway_add();
    FUNC(ADD_SPEED_XZ)(EM0D_FREE_P->Move_spd);  // Add_speedXZ
    Em0d_Move_ck(0x38);
}

VOID Em0d_dm00(VOID)
{
    PARTS_W* pP_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = EM0D_FREE_P->form + 9;
        G.pEm->Hokan_flg = 7;
        EM0D_FREE_P->Move_spd = 0;
        G.pEm->Spd_cur = 0x46;
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchPad->Vec.vy = -0x12C;
        if (pP_ptr->Workm.t[1] < -0x5DC)
        {
            ScratchPad->Vec.vy = 0x12C;
        }

        FUNC(ESP_CALL)(0, 8, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);  // Esp_call

        FUNC(SND_SE_ENEM)(2, 0);  // Snd_se_enem

    case 1:
        G.pEm->Routine_3 += Em0d_anm_step(0, 0x200);
        break;
    case 2:
        G.pEm->Routine_3 = 3U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 5;
        G.pEm->Hokan_flg = 7;
        G.pEm->Spd_cur = 0x78;
        G.pEm->Timer0 = 0x3C;

    case 3:
        Em0d_anm_step(0, 0x200);
        if (G.pEm->Move_cnt != 0xF && G.pEm->Move_cnt != 0x28)
        {
        }
        else
        {
            FUNC(SND_SE_ENEM)(1, 0);  // Snd_se_enem
        }
        ScratchWord[0] = 1;
        if (G.pEm->Timer0 >= 0x1F)
        {
            ScratchWord[0] = -1;
        }
        FUNC(GOTO00)(&MAIN.Player.Pos_x, ScratchWord[0] * 0x30);  // Goto00
        if (G.pEm->Timer0-- == 0)
        {
            EM_ROUTINE_HALF0 = 1;
            EM_ROUTINE_HALF2 = 1;
            EM0D_FREE_P->form = 1;
            G.pEm->Damage_no = 0;
            EM0D_FREE_P->Bite_wait = 0xDA;
            FUNC(SND_SE_ENEM)(1, 0);  // Snd_se_enem
        }
        Em0d_sway_add();
        pP_ptr;
    }
    FUNC(ADD_SPEED_XZ)(EM0D_FREE_P->Move_spd);  // Add_speedXZ
    Em0d_Move_ck(0x30);
}

VOID Em0d_sway_add(VOID)
{
    USHORT Bias_spd;

    Bias_spd = EM0D_FREE_P->Move_spd + 0x384;
    if (Bias_spd >= 0x709U)
    {
        EM0D_FREE_P->Move_spd = EM0D_FREE_P->Move_spd - EM0D_FREE_P->Sway_add;
        EM0D_FREE_P->Sway_add = EM0D_FREE_P->Sway_add / 64;
        EM0D_FREE_P->Sway_add *= -1;
        FUNC(SND_SE_ENEM)(0, 0);  // Snd_se_enem
    }
    if ((UCHAR)EM0D_FREE_P->Sway_add & 0x3F)  // truncates to a byte
    {
        EM0D_FREE_P->Sway_add = EM0D_FREE_P->Sway_add * 2;
    }
    EM0D_FREE_P->Move_spd = EM0D_FREE_P->Move_spd + EM0D_FREE_P->Sway_add;
}

VOID Em0d_die00(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 6;
        G.pEm->Hokan_flg = 7;
        EM0D_FREE_P->Move_spd = 0;
        G.pEm->Spd_cur = 0;
        G.pEm->Damage_no = 1;
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchPad->Vec.vy = -0x12C;
        if (pP_ptr->Workm.t[1] < -0x5DC)
        {
            ScratchPad->Vec.vy = 0x1F4;
        }

        FUNC(ESP_CALL)(0, 0xBU, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);  // Esp_call
        FUNC(ESP_CALL)(0, 8U, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);  // Esp_call

        FUNC(SND_SE_ENEM)(2, 0U);  // Snd_se_enem
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg, G.pEm->Em_set_flg);  // Flg_on

    case 1:
        G.pEm->Routine_3 += Em0d_anm_step(0, 0x200);
        FUNC(ADD_SPEED_XZ)(EM0D_FREE_P->Move_spd);  // Add_speedXZ
        Em0d_Move_ck(0x30);
        break;
    case 2:
        G.pEm->Routine_3 = 3;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 7;
        G.pEm->Hokan_flg = 7;
        G.pEm->Spd_cur = 0x46;
        FUNC(SND_SE_ENEM)(1, 0U);  // Snd_se_enem

    case 3:
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x30U);  // Goto00
        G.pEm->Routine_3 += Em0d_anm_step(0, 0x200);
        FUNC(ADD_SPEED_XZ)(EM0D_FREE_P->Move_spd);  // Add_speedXZ
        Em0d_Move_ck(0x30);
        break;
    case 4:
        G.pEm->Routine_3 = 5;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0xF;
        G.pEm->Hokan_flg = 7;
        G.pEm->Spd_cur = 0x64;
        FUNC(SND_SE_ENEM)(2, 0U);  // Snd_se_enem

    case 5:
        G.pEm->Routine_3 += Em0d_anm_step(0, 0x200);
        if (G.pEm->Move_cnt == 0x32)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0x1F4;

            FUNC(ESP_CALL)(0, 0xBU, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);  // Esp_call
            FUNC(ESP_CALL)(0, 8U, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);  // Esp_call

            FUNC(PAD_VIB_SET1)(5, 0x96U, 0);  // Pad_vib_set1
            FUNC(PAD_VIB_FADE_SET)(0x2D, 0x96U, 0, 5);  // Pad_vib_fade_set
        }
        FUNC(ADD_SPEED_XZ)(EM0D_FREE_P->Move_spd);  // Add_speedXZ
        Em0d_Move_ck(0x30);
        break;
    case 6:
        G.pEm->Spd_cur = 0;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT | EM_BE_NO_WALL;
        ScratchWord[0] = 0xD;
        while ((ScratchWord[0] = ScratchWord[0] - 1) != -1)
        {
            (MAIN.Enemy + ScratchWord[0])->Be_flg |= 4;
        }
        G.pEm->Routine_3 = 7;
        EM0D_FREE_P->Die_scale = 0x1000;
        G.pEm->Timer0 = 0;

    case 7:
        FUNC(ADD_SPEED_XZ)(EM0D_FREE_P->Move_spd);  // Add_speedXZ
        Em0d_Move_ck(0x30);
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec.vx = (0x1000 - EM0D_FREE_P->Die_scale) / 2 + 0x1000;
        ScratchPad->Vec.vy = EM0D_FREE_P->Die_scale;
        ScratchPad->Vec.vz = (0x1000 - EM0D_FREE_P->Die_scale) / 2 + 0x1000;
        G.pEm->Zone_in_flg = G.pEm->Zone_in_flg | 0x80;
        FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);  // RotMatrix
        FUNC(SCALE_MATRIX)(G.pEm->M, &ScratchPad->Vec);  // ScaleMatrix
        G.pEm->Pos_y = G.pEm->Pos_y + 2;
        FUNC(SCALE_MATRIX)(&pP_ptr[3].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[4].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[5].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[6].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[7].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[8].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[9].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[10].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[11].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[12].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[13].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[14].Workm, &ScratchPad->Vec);  // ScaleMatrix
        if (G.pEm->Timer0 < 0x50)
        {
            ++G.pEm->Timer0;
            if ((++G.pEm->Timer0 & 7) == 0)
            {
                FUNC(CLUT_FADE_SABUN_ADD)(-1, 0U, 0, 0, 0x200, G.pEm->Id);  // Cinesco_fade_sabun_add
            }
            if ((++G.pEm->Timer0 & 7) == 0)
            {
                FUNC(CLUT_FADE_SABUN_ADD)(0, -1U, 0, 0, 0x200, G.pEm->Id);  // Cinesco_fade_sabun_add
            }
            if ((G.pEm->Timer0 & 0x1F) == 0)
            {
                FUNC(CLUT_FADE_SABUN_ADD)(0, 0U, -1, 0, 0x200, G.pEm->Id);  // Cinesco_fade_sabun_add
            }
        }
        EM0D_FREE_P->Die_scale -= 0x14;
        if (EM0D_FREE_P->Die_scale < 0x1F4)
        {
            G.pEm->Routine_3 = 8;
            G.pEm->Timer0 = 0;
            ScratchWord[0] = 0xD;
            while ((ScratchWord[0] = ScratchWord[0] - 1) != -1)
            {
                FUNC(KAGE_WORK_COLOR_SET)(&MAIN.Enemy[ScratchWord[0]].Kage_work, 1U, 2, 0xAFDF9F);  // Kage_work_color_set
            }
        }
        break;
    case 8:
        FUNC(ADD_SPEED_XZ)(EM0D_FREE_P->Move_spd);  // Add_speedXZ
        Em0d_Move_ck(0x30);
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec.vx = 0x1706 - G.pEm->Timer0 * 0x10;
        ScratchPad->Vec.vy = 0x1F4 - G.pEm->Timer0;
        ScratchPad->Vec.vz = 0x1706 - G.pEm->Timer0 * 0x10;
        G.pEm->Zone_in_flg = G.pEm->Zone_in_flg | 0x80;
        FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);  // RotMatrix
        FUNC(SCALE_MATRIX)(G.pEm->M, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[3].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[4].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[5].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[6].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[7].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[8].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[9].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[10].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[11].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[12].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[13].Workm, &ScratchPad->Vec);  // ScaleMatrix
        FUNC(SCALE_MATRIX)(&pP_ptr[14].Workm, &ScratchPad->Vec);  // ScaleMatrix
        if (++G.pEm->Timer0 >= 0x15F)
        {
            G.pEm->Routine_3 = 9;
        }
    }
    if (EM0D_FREE_P->Die_cnt < 0x320U)
    {
        ++EM0D_FREE_P->Die_cnt;
        if (!(EM0D_FREE_P->Die_cnt & 0xF))
        {
            ScratchPad->Vec.vy = 0x1F4;
            ScratchPad->Vec.vx = (Api_rand() & 0x3FF) - 0x200;  // rand
            ScratchPad->Vec.vz = (Api_rand() & 0x3FF) - 0x200;  // rand
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchWord[7] = Api_rand() & 0xF;  // rand

            FUNC(ESP_CALL)(9, 0x11U, 0, &pP_ptr[ScratchWord[7]].Workm, &ScratchPad->Vec, 0x14);  // Esp_call

            ScratchWord[7] = Api_rand() & 0xF;  // rand
            ScratchPad->Vec.vx = (Api_rand() & 0x3FF) - 0x200;  // rand
            ScratchPad->Vec.vz = (Api_rand() & 0x3FF) - 0x200;  // rand

            FUNC(ESP_CALL)(9, 0x11U, 0, &pP_ptr[ScratchWord[7]].Workm, &ScratchPad->Vec, 0x14);  // Esp_call

            ScratchWord[7] = Api_rand() & 0xF;  // rand
            ScratchPad->Vec.vx = (Api_rand() & 0x3FF) - 0x200;  // rand
            ScratchPad->Vec.vz = (Api_rand() & 0x3FF) - 0x200;  // rand

            FUNC(ESP_CALL)(9, 0x11U, 0, &pP_ptr[ScratchWord[7]].Workm, &ScratchPad->Vec, 0x14);  // Esp_call
        }
        if (!((EM0D_FREE_P->Die_cnt + 3) & 7) && EM0D_FREE_P->Die_cnt < 0x258U)
        {
            ScratchPad->Vec.vy = 0x1F4;
            ScratchPad->Vec.vx = 0;
            ScratchPad->Vec.vz = 0;
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchWord[7] = Api_rand() & 0xF;  // rand
            ScratchPad->Vec.vx = (Api_rand() & 0x3FF) - 0x200;  // rand
            ScratchPad->Vec.vz = (Api_rand() & 0x3FF) - 0x200;  // rand

            FUNC(ESP_CALL)(9, 0x11U, 0, &pP_ptr[ScratchWord[7]].Workm, &ScratchPad->Vec, 0x14);  // Esp_call

            ScratchWord[7] = Api_rand() & 0xF;  // rand
            ScratchPad->Vec.vx = (Api_rand() & 0x3FF) - 0x200;  // rand
            ScratchPad->Vec.vz = (Api_rand() & 0x3FF) - 0x200;  // rand

            FUNC(ESP_CALL)(9, 0x11U, 0, &pP_ptr[ScratchWord[7]].Workm, &ScratchPad->Vec, 0x14);  // Esp_call

            ScratchWord[7] = Api_rand() & 0xF;  // rand
            ScratchPad->Vec.vx = (Api_rand() & 0x3FF) - 0x200;  // rand
            ScratchPad->Vec.vz = (Api_rand() & 0x3FF) - 0x200;  // rand

            FUNC(ESP_CALL)(9, 0x11U, 0, &pP_ptr[ScratchWord[7]].Workm, &ScratchPad->Vec, 0x14);  // Esp_call
        }
    }
}

VOID Em0d_pldie(VOID)
{
    Pldie_Em0d_tbl[G.Player.Routine_2]();
}

VOID Em0d_pldie00(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 2;
        G.Player.Hokan_flg = 3;
        G.Player.Damage_no = 1;

    case 1:
        G.Player.Routine_3 += FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);  // Joint_move

        if (G.Enemy[0].Life < 0)
        {
            PL_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
            G.Player.Be_flg &= ~EM_BE_NO_HIT;
            G.Player.Be_flg &= ~EM_BE_NO_WALL;
            G.Player.Damage_no = 0;
        }

        return;
    case 2:
        pP_ptr = G.Enemy[0].pSin_parts_ptr;  // reinterprets the memory as PARTS_W
        G.Player.Zone_in_flg |= 0x80;
        FUNC(GS_MUL_COORD0)(&pP_ptr->Workm, &Em0d_catch_m, &MAIN.Player.M);  // GsMulCoord0
    }
}

VOID Em0d_chain_follow(PARTS_W* pParent, PARTS_W* pP_ptr, SHORT Max_turn)
{
    INT dx;
    INT dz;
    USHORT dist;
    ENEMY_WORK* pHead;
    INT Rsv2;
    VECTOR New_pos;
    ScratchWord[0] = pP_ptr->Cdir.vy;
    pHead = MAIN.Enemy;
    ScratchPad->Vec.vx = pP_ptr->Coord.t[0] << 0xC;
    ScratchPad->Vec.vy = pP_ptr->Coord.t[1] << 0xC;
    ScratchPad->Vec.vz = pP_ptr->Coord.t[2] << 0xC;
    FUNC(APPLY_MATRIX_LV)(&pParent->Workm, &ScratchPad->Vec, &ScratchPad->Vec);  // ApplyMatrixLV
    New_pos = ScratchPad->Vec;
    ScratchPad->Vec.vx = ScratchPad->Vec.vx / 35 * 0x22;
    ScratchPad->Vec.vy = ScratchPad->Vec.vy / 35 * 0x22;
    ScratchPad->Vec.vz = ScratchPad->Vec.vz / 35 * 0x22;
    ScratchPad->Vec.vx = ScratchPad->Vec.vx + (pParent->Workm.t[0] << 0xC);
    ScratchPad->Vec.vy = ScratchPad->Vec.vy + (pParent->Workm.t[1] << 0xC);
    ScratchPad->Vec.vz = ScratchPad->Vec.vz + (pParent->Workm.t[2] << 0xC);
    New_pos.vx += pParent->Workm.t[0] << 0xC;
    New_pos.vy += pParent->Workm.t[1] << 0xC;
    New_pos.vz += pParent->Workm.t[2] << 0xC;
    dx = ScratchPad->Vec.vx - (pP_ptr->Workm.t[0] << 0xC);
    dz = ScratchPad->Vec.vz - (pP_ptr->Workm.t[2] << 0xC);
    if ((dist = FUNC(SQUARE_ROOT0)((dx >> 0xC) * (dx >> 0xC) + (dz >> 0xC) * (dz >> 0xC))) < 0x32U)  // SquareRoot0
    {
        goto set_matrix;
    }
    dx = dx >> 4;
    dz = dz >> 4;
    if (dx != 0)
    {
        pP_ptr->Cdir.vy = (dx < 0 ? 0x800 : 0x1000) - FUNC(CATAN)(dz * 0x1000 / dx);  // catan
    }
    else
    {
        pP_ptr->Cdir.vy = dz > 0 ? 0xC00 : 0x400;
    }
    ScratchWord[1] = (pP_ptr->Cdir.vy - ScratchWord[0] + Max_turn) & 0xFFF;
    if (Max_turn + Max_turn < ScratchWord[1])
    {
        pP_ptr->Cdir.vy = ScratchWord[0] - Max_turn;
        if (Max_turn + 0x800 >= ScratchWord[1])
        {
            pP_ptr->Cdir.vy = pP_ptr->Cdir.vy + (Max_turn + Max_turn);
        }
    }
    else if (pHead->Spd_cur < 0x64)
    {
        pP_ptr->Cdir.vy = ScratchWord[0];
    }
    dx = pParent->Cdir.vy;
    if (pP_ptr->Parts_no < 5U)
    {
        if (pP_ptr->Parts_no == 3)
        {
            dx = G.Enemy[0].Cdir_y;
        }
        if ((dz = (pP_ptr->Cdir.vy - dx + 0x400) & 0xFFF) >= 0x801)
        {
            pP_ptr->Cdir.vy = dx - 0x400;
            if (dz < 0xC00)
            {
                pP_ptr->Cdir.vy = dx + 0x400;
            }
        }
    }
set_matrix:
    ScratchPad->Mat = *Monitor.pId_mat;
    FUNC(ROT_MATRIX)(&pP_ptr->Cdir, &pP_ptr->Workm);  // RotMatrix
    pP_ptr->Workm.t[0] = New_pos.vx >> 0xC;
    pP_ptr->Workm.t[1] = New_pos.vy >> 0xC;
    pP_ptr->Workm.t[2] = New_pos.vz >> 0xC;
}

VOID Em0d_chain_align(PARTS_W* pParent, PARTS_W* pP_ptr)
{
    INT dx;
    INT dz;

    ScratchPad->Vec.vx = pP_ptr->Coord.t[0] << 0xC;
    ScratchPad->Vec.vy = pP_ptr->Coord.t[1] << 0xC;
    ScratchPad->Vec.vz = pP_ptr->Coord.t[2] << 0xC;
    FUNC(APPLY_MATRIX_LV)(&pParent->Workm, &ScratchPad->Vec, &ScratchPad->Vec);  // ApplyMatrixLV
    ScratchPad->Vec.vx = ScratchPad->Vec.vx / 35 * 0x22;
    ScratchPad->Vec.vy = ScratchPad->Vec.vy / 35 * 0x22;
    ScratchPad->Vec.vz = ScratchPad->Vec.vz / 35 * 0x22;
    ScratchPad->Vec.vx = ScratchPad->Vec.vx + (pParent->Workm.t[0] << 0xC);
    ScratchPad->Vec.vy = ScratchPad->Vec.vy + (pParent->Workm.t[1] << 0xC);
    ScratchPad->Vec.vz = ScratchPad->Vec.vz + (pParent->Workm.t[2] << 0xC);
    dx = ScratchPad->Vec.vx - (pP_ptr->Workm.t[0] << 0xC);
    dz = ScratchPad->Vec.vz - (pP_ptr->Workm.t[2] << 0xC);
    dx = dx >> 4;
    dz = dz >> 4;
    if (dx != 0)
    {
        ScratchWord[1] = (dx < 0 ? 0x800 : 0x1000) - FUNC(CATAN)(dz * 0x1000 / dx);  // catan
    }
    else
    {
        ScratchWord[1] = dz > 0 ? 0xC00 : 0x400;
    }
    if (((ScratchWord[1] - pP_ptr->Cdir.vy + 0x800) & 0xFFF) < 0x800)
    {
        ScratchWord[1] = (pP_ptr->Cdir.vy - ScratchWord[1]) & 0xFFF;
        if (ScratchWord[1] >= 0x31)
        {
            ScratchWord[1] = 0x30;
        }
        pParent->Cdir.vy = pParent->Cdir.vy - ScratchWord[1];
    }
    else
    {
        ScratchWord[1] = (ScratchWord[1] - pP_ptr->Cdir.vy) & 0xFFF;
        if (ScratchWord[1] >= 0x31)
        {
            ScratchWord[1] = 0x30;
        }
        pParent->Cdir.vy = pParent->Cdir.vy + ScratchWord[1];
    }
    ScratchPad->Mat = *Monitor.pId_mat;
    FUNC(ROT_MATRIX)(&pParent->Cdir, &pParent->Workm);  // RotMatrix
}

UCHAR Em0d_anm_step(UCHAR reverse, SHORT Hokan_step)
{
    SHORT* pFrm_data;
    ANM_REC* pAnim;
    ANM_FRAME_REC* pFrm;
    ANM_REC* pEntry;
    PARTS_W* pP_ptr;
    INT unused;
    VECTOR Root_sa;

    if (G.pEm->Move_wait >= 2)
    {
        --G.pEm->Move_wait;
        return 0;
    }
    pAnim = G.pEm->pKan_t_ptr;
    pFrm_data = (SHORT*)(pAnim + ((SHORT*)pAnim)[1] / 4);  // halfword view of the same memory
    ScratchWord[0] = (*(SHORT*)&pAnim[1].Frm_ofs) / 2;  // signed 16-bit load of the USHORT
    pAnim = G.pEm->pSeq_t_ptr;
    pEntry = pAnim + G.pEm->Move_no;  // reinterprets the memory as ANM_REC
    pFrm = (ANM_FRAME_REC*)(pAnim + (pEntry->Frm_ofs >> 2));  // reinterprets the memory as ANM_FRAME_REC
    if (reverse != 0)
    {
        pFrm = (ANM_FRAME_REC*)((UCHAR*)pFrm + ((pEntry->Frm_num - G.pEm->Move_cnt) * 4 - 4));  // reinterprets the memory as ANM_FRAME_REC
    }
    else
    {
        pFrm = BYTE_OFS(ANM_FRAME_REC, pFrm, G.pEm->Move_cnt * 4);
    }
    pFrm_data += pFrm->Pose_idx * ScratchWord[0];
    pP_ptr = G.pEm->pSin_parts_ptr;
    pP_ptr->Coord.t[1] = *++pFrm_data;
    pFrm_data += 5;
    ScratchWord[6] = 3;
    ScratchWord[5] = G.pEm->Hokan_flg;
    if (ScratchWord[5] == 0)
    {
        while ((ScratchWord[6] = ScratchWord[6] - 1) != -1)
        {
            pP_ptr->Cdir.vx = *pFrm_data;
            pP_ptr->Cdir.vy = *++pFrm_data;
            pP_ptr->Cdir.vz = *++pFrm_data;
            pFrm_data++;
            FUNC(ROT_MATRIX)(&pP_ptr->Cdir, &pP_ptr->Coord);  // RotMatrix
            pP_ptr++;
        }
    }
    else
    {
        while ((ScratchWord[6] = ScratchWord[6] - 1) != -1)
        {
            ScratchSvU[0] = *pFrm_data;
            ScratchSvU[1] = *++pFrm_data;
            ScratchSvU[2] = *++pFrm_data;
            FUNC(LOAD_AVERAGE_SHORT12)(&pP_ptr->Cdir, &ScratchPad->Sv, Hokan_step * ScratchWord[5], Hokan_step * (0x1000 / Hokan_step - ScratchWord[5]), &pP_ptr->Cdir);  // LoadAverageShort12
            pFrm_data++;
            FUNC(ROT_MATRIX)(&pP_ptr->Cdir, &pP_ptr->Coord);  // RotMatrix
            pP_ptr++;
        }
    }
    FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);  // RotMatrix
    FUNC(GS_MUL_COORD0)(G.pEm->M, &pP_ptr[-3].Coord, &pP_ptr[-3].Workm);  // GsMulCoord0
    FUNC(GS_MUL_COORD0)(&pP_ptr[-3].Workm, &pP_ptr[-1].Coord, &pP_ptr[-1].Workm);  // GsMulCoord0
    pP_ptr[-1].Workm.t[0] = pP_ptr[-1].Workm.t[0] << 9;
    pP_ptr[-1].Workm.t[1] = pP_ptr[-1].Workm.t[1] << 9;
    pP_ptr[-1].Workm.t[2] = pP_ptr[-1].Workm.t[2] << 9;
    Root_sa = MATRIX_POS(&pP_ptr[10].Workm);
    Root_sa.vy = EM0D_FREE_P->Ground_y << 9;
    Root_sa.vx <<= 9;
    Root_sa.vz <<= 9;
    if (ScratchWord[5] == 0)
    {
        ScratchWord[7] = *++pFrm_data;
        if (G.pEm->Move_cnt == 0)
        {
            pP_ptr->Free1 = ScratchHalf[14];
        }
        pP_ptr->Cdir.vy = pP_ptr->Cdir.vy + (ScratchHalf[14] - pP_ptr->Free1);
        pP_ptr->Free1 = ScratchHalf[14];
        pP_ptr->Cdir.vz = *++pFrm_data + pP_ptr[-1].Cdir.vz + pP_ptr[-3].Cdir.vz;
        pFrm_data++;
    }
    else
    {
        ScratchSvU[0] = 0;
        ScratchWord[7] = *++pFrm_data;
        if (G.pEm->Move_cnt == 0)
        {
            pP_ptr->Free1 = ScratchHalf[14];
        }
        ScratchSvU[1] = pP_ptr->Cdir.vy + ScratchHalf[14] - pP_ptr->Free1;
        pP_ptr->Free1 = ScratchHalf[14];
        ScratchSvU[2] = *++pFrm_data + pP_ptr[-1].Cdir.vz + pP_ptr[-3].Cdir.vz;
        FUNC(LOAD_AVERAGE_SHORT12)(&pP_ptr->Cdir, &ScratchPad->Sv, Hokan_step * ScratchWord[5], Hokan_step * (0x1000 / Hokan_step - ScratchWord[5]), &pP_ptr->Cdir);  // LoadAverageShort12
        pFrm_data++;
    }
    ScratchPad->Vec.vx = pP_ptr->Coord.t[0] << 9;
    ScratchPad->Vec.vy = pP_ptr->Coord.t[1] << 9;
    ScratchPad->Vec.vz = pP_ptr->Coord.t[2] << 9;
    FUNC(ROT_MATRIX)(&pP_ptr->Cdir, &pP_ptr->Workm);  // RotMatrix
    FUNC(APPLY_MATRIX_LV)(&pP_ptr[-1].Workm, &ScratchPad->Vec, &ScratchPad->Vec);  // ApplyMatrixLV
    pP_ptr->Workm.t[0] = pP_ptr[-1].Workm.t[0] + ScratchPad->Vec.vx;
    pP_ptr->Workm.t[1] = pP_ptr[-1].Workm.t[1] + ScratchPad->Vec.vy;
    pP_ptr->Workm.t[2] = pP_ptr[-1].Workm.t[2] + ScratchPad->Vec.vz;
    pP_ptr++;
    ScratchWord[6] = 0xB;
    if (ScratchWord[5] == 0)
    {
        while ((ScratchWord[6] = ScratchWord[6] - 1) != -1)
        {
            ScratchWord[7] = *++pFrm_data;
            if (G.pEm->Move_cnt == 0)
            {
                pP_ptr->Free1 = ScratchHalf[14];
            }
            pP_ptr->Cdir.vy = pP_ptr->Cdir.vy + (ScratchHalf[14] - pP_ptr->Free1);
            pP_ptr->Free1 = ScratchHalf[14];
            pP_ptr->Cdir.vz = *++pFrm_data + pP_ptr[-1].Cdir.vz;
            pFrm_data++;
            ScratchPad->Vec.vx = pP_ptr->Coord.t[0] << 9;
            ScratchPad->Vec.vy = pP_ptr->Coord.t[1] << 9;
            ScratchPad->Vec.vz = pP_ptr->Coord.t[2] << 9;
            if (ScratchWord[6] == 7)
            {
                pP_ptr->Cdir.vz = 0;
            }
            FUNC(ROT_MATRIX)(&pP_ptr->Cdir, &pP_ptr->Workm);  // RotMatrix
            FUNC(APPLY_MATRIX_LV)(&pP_ptr[-1].Workm, &ScratchPad->Vec, &ScratchPad->Vec);  // ApplyMatrixLV
            pP_ptr->Workm.t[0] = pP_ptr[-1].Workm.t[0] + ScratchPad->Vec.vx;
            pP_ptr->Workm.t[1] = pP_ptr[-1].Workm.t[1] + ScratchPad->Vec.vy;
            pP_ptr->Workm.t[2] = pP_ptr[-1].Workm.t[2] + ScratchPad->Vec.vz;
            pP_ptr++;
        }
    }
    else
    {
        while ((ScratchWord[6] = ScratchWord[6] - 1) != -1)
        {
            ScratchSvU[0] = 0;
            ScratchWord[7] = *++pFrm_data;
            if (G.pEm->Move_cnt == 0)
            {
                pP_ptr->Free1 = ScratchHalf[14];
            }
            ScratchSvU[1] = pP_ptr->Cdir.vy + ScratchHalf[14] - pP_ptr->Free1;
            pP_ptr->Free1 = ScratchHalf[14];
            ScratchSvU[2] = *++pFrm_data + pP_ptr[-1].Cdir.vz;
            FUNC(LOAD_AVERAGE_SHORT12)(&pP_ptr->Cdir, &ScratchPad->Sv, Hokan_step * ScratchWord[5], Hokan_step * (0x1000 / Hokan_step - ScratchWord[5]), &pP_ptr->Cdir);  // LoadAverageShort12
            pFrm_data++;
            ScratchPad->Vec.vx = pP_ptr->Coord.t[0] << 9;
            ScratchPad->Vec.vy = pP_ptr->Coord.t[1] << 9;
            ScratchPad->Vec.vz = pP_ptr->Coord.t[2] << 9;
            if (ScratchWord[6] == 7)
            {
                pP_ptr->Cdir.vz = 0;
            }
            FUNC(ROT_MATRIX)(&pP_ptr->Cdir, &pP_ptr->Workm);  // RotMatrix
            FUNC(APPLY_MATRIX_LV)(&pP_ptr[-1].Workm, &ScratchPad->Vec, &ScratchPad->Vec);  // ApplyMatrixLV
            pP_ptr->Workm.t[0] = pP_ptr[-1].Workm.t[0] + ScratchPad->Vec.vx;
            pP_ptr->Workm.t[1] = pP_ptr[-1].Workm.t[1] + ScratchPad->Vec.vy;
            pP_ptr->Workm.t[2] = pP_ptr[-1].Workm.t[2] + ScratchPad->Vec.vz;
            pP_ptr++;
        }
        --G.pEm->Hokan_flg;
    }
    Root_sa.vx -= pP_ptr[-2].Workm.t[0];
    Root_sa.vy -= pP_ptr[-2].Workm.t[1];
    Root_sa.vz -= pP_ptr[-2].Workm.t[2];
    pP_ptr--;
    ScratchWord[6] = 0xC;
    while ((ScratchWord[6] = ScratchWord[6] - 1) != -1)
    {
        pP_ptr->Workm.t[0] = (pP_ptr->Workm.t[0] + Root_sa.vx) >> 9;
        pP_ptr->Workm.t[1] = (pP_ptr->Workm.t[1] + Root_sa.vy) >> 9;
        pP_ptr->Workm.t[2] = (pP_ptr->Workm.t[2] + Root_sa.vz) >> 9;
        pP_ptr--;
    }
    pP_ptr++;
    G.pEm->Pos_x += Root_sa.vx >> 9;
    G.pEm->Pos_z += Root_sa.vz >> 9;
    pP_ptr = G.pEm->pSin_parts_ptr;
    pP_ptr->Coord.t[1] += Root_sa.vy >> 9;
    G.pEm->Move_wait = pFrm->Move_wait;
    if (++G.pEm->Move_cnt > pEntry->Frm_num - 1)
    {
        G.pEm->Move_cnt = 0;
        return 1;
    }
    return 0;
}

VOID Em0d_joint_set(VOID)
{
    SHORT* pFrm_data;
    ANM_REC* pAnim;
    USHORT* pFrm;
    ANM_REC* pEntry;
    PARTS_W* pP_ptr;
    UCHAR i;
    UCHAR unused_u8;
    UCHAR count;
    INT unused[6];

    pAnim = G.pEm->pKan_t_ptr;
    pFrm_data = (SHORT*)(pAnim + ((SHORT*)pAnim)[1] / 4);  // halfword view of the same memory
    ScratchWord[0] = (*(SHORT*)&pAnim[1].Frm_ofs) / 2;  // signed 16-bit load of the USHORT
    pAnim = G.pEm->pSeq_t_ptr;
    pEntry = pAnim + G.pEm->Move_no;  // reinterprets the memory as ANM_REC
    pFrm = (USHORT*)(pAnim + (pEntry->Frm_ofs >> 2));  // halfword view of the same memory
    pFrm += G.pEm->Move_cnt * 2;
    pFrm_data += *pFrm * ScratchWord[0];
    pP_ptr = G.pEm->pSin_parts_ptr;
    pP_ptr->Coord.t[1] = *++pFrm_data;
    pFrm_data += 5;
    i = 0;
    count = G.pEm->Parts_num;
    while (i < count)
    {
        if (i < 3)
        {
            pP_ptr->Cdir.vx = *pFrm_data;
            pP_ptr->Cdir.vy = *++pFrm_data;
            pP_ptr->Cdir.vz = *++pFrm_data;
            pFrm_data++;
            FUNC(ROT_MATRIX)(&pP_ptr->Cdir, &pP_ptr->Coord);  // RotMatrix
            if (i == 0)
            {
                FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);  // RotMatrix
                FUNC(GS_MUL_COORD0)(G.pEm->M, &pP_ptr->Coord, &pP_ptr->Workm);  // GsMulCoord0
            }
            if (i & 2)
            {
                FUNC(GS_MUL_COORD0)(&pP_ptr[-2].Workm, &pP_ptr->Coord, &pP_ptr->Workm);  // GsMulCoord0
            }
        }
        else
        {
            ScratchWord[7] = *++pFrm_data;
            pP_ptr->Free1 = ScratchWord[7];
            if (i == 3)
            {
                pP_ptr->Cdir.vy = ScratchWord[7] + G.pEm->Cdir_y;
                pP_ptr->Cdir.vz = *++pFrm_data + pP_ptr[-1].Cdir.vz + pP_ptr[-3].Cdir.vz;
            }
            else
            {
                pP_ptr->Cdir.vy = ScratchWord[7] + pP_ptr[-1].Cdir.vy;
                pP_ptr->Cdir.vz = *++pFrm_data + pP_ptr[-1].Cdir.vz;
            }
            pFrm_data++;
            if (i == 7)
            {
                pP_ptr->Cdir.vz = 0;
            }
            FUNC(ROT_MATRIX)(&pP_ptr->Cdir, &pP_ptr->Workm);  // RotMatrix
            FUNC(APPLY_MATRIX_LV)(&pP_ptr[-1].Workm, pP_ptr->Coord.t, &ScratchPad->Vec);  // ApplyMatrixLV
            pP_ptr->Workm.t[0] = pP_ptr[-1].Workm.t[0] + ScratchPad->Vec.vx;
            pP_ptr->Workm.t[1] = pP_ptr[-1].Workm.t[1] + ScratchPad->Vec.vy;
            pP_ptr->Workm.t[2] = pP_ptr[-1].Workm.t[2] + ScratchPad->Vec.vz;
        }
        pP_ptr++;
        i++;
    }
}

VOID Em0d_Move_ck(SHORT Max_turn)
{
    PARTS_W* pP_ptr;
    VECTOR Old_pos;
    pP_ptr = G.pEm->pSin_parts_ptr;
    pP_ptr += 3;
    Old_pos = EM_POS_VEC;
    if (G.pEm->Routine_2 != 5 && G.pEm->Routine_2 != 6 && G.pEm->Routine_2 != 7)
    {
        FUNC(OBA_CK_HIT)(&MAIN.Player.Be_flg, G.pEm);  // Oba_ck_hit
    }
    ScratchWord[0] = FUNC(SCA_CK_HIT)(&G.pEm->Pos_x, G.pEm->pAt_tbl->At_w);  // Sca_ck_hit
    if (!(G.pEm->Type & 1))
    {
        if (ScratchWord[0] != 0)
        {
            EM0D_FREE_P->wall_frames++;
            EM0D_FREE_P->Wall_wait = 0x14;
        }
        else if (EM0D_FREE_P->Wall_wait != 0)
        {
            if (!(EM0D_FREE_P->Wall_wait--))  // signed conversion (unsigned operand)
            {
                EM0D_FREE_P->wall_frames = 0;
            }
        }
    }
    Old_pos.vx -= G.pEm->Pos_x;
    Old_pos.vz -= G.pEm->Pos_z;
    FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);  // RotMatrix
    FUNC(GS_MUL_COORD0)(G.pEm->M, &pP_ptr[-3].Coord, &pP_ptr[-3].Workm);  // GsMulCoord0
    FUNC(GS_MUL_COORD0)(&pP_ptr[-3].Workm, &pP_ptr[-1].Coord, &pP_ptr[-1].Workm);  // GsMulCoord0
    Em0d_chain_follow(&pP_ptr[-1], pP_ptr, Max_turn);
    Em0d_chain_follow(pP_ptr, &pP_ptr[1], Max_turn);
    Em0d_chain_follow(&pP_ptr[1], &pP_ptr[2], Max_turn);
    Em0d_chain_follow(&pP_ptr[2], &pP_ptr[3], Max_turn);
    Em0d_chain_follow(&pP_ptr[3], &pP_ptr[4], Max_turn);
    Em0d_chain_follow(&pP_ptr[4], &pP_ptr[5], Max_turn);
    Em0d_chain_follow(&pP_ptr[5], &pP_ptr[6], Max_turn);
    Em0d_chain_follow(&pP_ptr[6], &pP_ptr[7], Max_turn);
    if (G.pEm->Spd_cur == 0 && EM0D_FREE_P->Die_scale == 0)
    {
        return;
    }
    Em0d_chain_follow(&pP_ptr[7], &pP_ptr[8], Max_turn);
    Em0d_chain_follow(&pP_ptr[8], &pP_ptr[9], Max_turn);
    Em0d_chain_follow(&pP_ptr[9], &pP_ptr[10], Max_turn / 2);
    Em0d_chain_follow(&pP_ptr[10], &pP_ptr[11], Max_turn / 2);
}

VOID Em0d_dust_set(INT Ofs_idx)
{
    ScratchPad->Mat = *Monitor.pId_mat;
    ScratchPad->Mat.t[2] = Em0d_dust_tbl[Ofs_idx].z - Em0d_dust_tbl[Ofs_idx].z * (Api_rand() & 2);  // rand
    ScratchPad->Mat.t[1] = 0x32;
    ScratchPad->Mat.t[0] = Em0d_dust_tbl[Ofs_idx].x;
    FUNC(GS_MUL_COORD2)(&G.pEm->pSin_parts_ptr[1].Workm, &ScratchPad->Mat.m[0][0]);  // GsMulCoord2
    ScratchPad->Vec.vx = ScratchPad->Mat.t[0];
    ScratchPad->Vec.vy = ScratchPad->Mat.t[1];
    ScratchPad->Vec.vz = ScratchPad->Mat.t[2];

    FUNC(ESP_CALL)(0, 4, 0, Monitor.pId_mat, &ScratchPad->Vec, 0x14);  // Esp_call
}

static AT_TBL Em0d_sca_tbl = { 0x8000, 0, -2000, 0, 0x07D0, 0x0320 };  // collision info of the yawn: flags 0x8000, y offset -2000, half height 2000, radius 800
AT_TBL* pEm0d_sca = &Em0d_sca_tbl;      // collision info pointer read by Em0d_Init
MATRIX Em0d_catch_m = { 0 };              // transform of the captured player while swallowed

// state routines by state number
VOID (*Em0d_routine_0[5])(VOID) =
{
    Em0d_Init,          // 0 //
    Em0d_Move,   // 1 //
    Em0d_Damage,       // 2 //
    Em0d_Die,           // 3 //
    Em0d_Wait,    // 4 //
};

// action routines by Routine_2
VOID (*Em0d_mv_tbl[9])(VOID) =
{
    Em0d_mv00,         // 0 //
    Em0d_mv01,         // 1 //
    Em0d_mv02,         // 2 //
    Em0d_mv03,         // 3 //
    Em0d_mv04,      // 4 //
    Em0d_mv05,        // 5 //
    Em0d_mv06,       // 6 //
    Em0d_mv07,         // 7 //
    Em0d_mv08,   // 8 //
};

// action selectors by behavior flags (0 = none)
VOID (*Em0d_br_tbl[7])(VOID) =
{
    Em0d_br_type00,     // 0 //
    0,                           // 1 //
    Em0d_br_type00,     // 2 //
    0,                           // 3 //
    Em0d_br_type01,   // 4 //
    0,                           // 5 //
    Em0d_br_type01,   // 6 //
};

// hurtful bite frames by form
EM0D_BITE_TBL Em0d_bite_tbl[2] =
{
    { 0x0D, 6 },   // 0 //
    { 0x12, 5 },   // 1 //
};

// waypoints of the entry path
EM0D_WAY_TBL Em0d_entry_tbl[6] =
{
    { 0x1194, 0x5FB4 },   // 0 //
    { 0x2AF8, 0x5FB4 },   // 1 //
    { 0x2904, 0x36B0 },   // 2 //
    { 0x2134, 0x2CEC },   // 3 //
    { 0x1A90, 0x3778 },   // 4 //
    { 0x0ED8, 0x20D0 },   // 5 //
};

// waypoints of the flee path
EM0D_WAY_TBL Em0d_flee_tbl[9] =
{
    { 0x1C84, 0x2710 },   // 0 //
    { 0x2EE0, 0x2EE0 },   // 1 //
    { 0x2AF8, 0x3C8C },   // 2 //
    { 0x2AF8, 0x61A8 },   // 3 //
    { 0x2AF8, 0x61A8 },   // 4 //
    { 0x1964, 0x61A8 },   // 5 //
    { 0x09C4, 0x6590 },   // 6 //
    { 0x09C4, 0x7918 },   // 7 //
    { 0x3A98, 0x7918 },   // 8 //
};

// waypoints of the reposition path
EM0D_WAY_TBL Em0d_repos_tbl[4] =
{
    { 0x1964, 0x1D4C },   // 0 //
    { 0x2CEC, 0x4C2C },   // 1 //
    { 0x1D4C, 0x5FB4 },   // 2 //
    { 0x2CEC, 0x5FB4 },   // 3 //
};

PL_FUNC Pldie_Em0d_tbl[1] = { Em0d_pldie00 };  // player animation routine by Routine_2

// dust effect offsets by index
EM0D_DUST_TBL Em0d_dust_tbl[3] =
{
    { 0x012C, 0x0190 },   // 0 //
    { 0x0258, 0x0064 },   // 1 //
    { 0x0320, 0x0032 },   // 2 //
};

SHORT Em0d_flee_timer;  // frames spent fleeing
