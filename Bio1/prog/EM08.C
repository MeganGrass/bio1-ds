/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM08.C
 */

#include "TYPES.H"

#include "EM08.H"
#include "JOINT.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Em08_Init(VOID)
{
    ENEMY_WORK* pSub;
    ENEMY_WORK* pEm;

    // state word: Act_sts << 24 | Routine_2 << 16 | Key_rno << 8 | state
    EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
    G.pEm->Zone_in_flg = 1;
    G.pEm->Coord_flg = 0;
    G.pEm->Damage_no = 0;
    FUNC(INIT_PARTS_WORK)();
    ScratchWord[7] = 0x404040;
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vx = 0;
    FUNC(KAGE_WORK_INIT)(&ScratchPad->Sv, &G.pEm->Kage_work, 0x7D0, 0x12C);
    Api_rand();
    G.pEm->Life = 0x28U;
    if (!(G.pEm->Type & 1))
    {
        G.pEm->Life = G.pEm->Life + 0x64;
    }
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Move_no = 2;
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
    G.pEm->pAt_tbl = pEm08_sca_vine;  // reinterprets the memory as AT_TBL
    EM08_FREE_P->Rot_spd = 0;
    EM08_FREE_P->Dummy176 = 0;
    EM08_FREE_P->Eff_timer = 0;
    EM08_FREE_P->ambient_effect_timer = (Api_rand() & 0xFF) + 0x32;
    if (!(G.pEm->Type & 1))
    {
        G.pEm->Damage_no = 1;
        G.pEm->pAt_tbl = pEm08_sca_main;  // reinterprets the memory as AT_TBL
        G.pEm->pSca_ofs[1].Off_x = 0;
        G.pEm->pSca_ofs[1].Off_y = 0;
        G.pEm->pSca_ofs[1].Off_z = 0;
        FUNC(SET_ENEMY_WORK)(1, 0x384, G.pEm->Parts_num + 1, &G.pEm->pTgt);
        pSub = G.pEm->pTgt;
        pSub->Type = 0;
        pSub->Damage_no = 0;
        pSub->Scale = 0x1000;
        pSub->Spd_y = 0;
        pSub->Spd_z = 1;
        pSub->Old_x = 0;
        pSub->Old_y = 1;
        pSub->Old_z = 4;
        pSub->Old_dir = 0;
        pSub->Cdir_x = 0;
        pSub->Cdir_y = 0;
        pSub->Cdir_z = 0;
        pSub->Timer0 = 0x59;
        pSub->Spd_x = 0;
        pSub->Hokan_flg = 7;
        pSub->Ground = 0;
        EM08_BODY_OF(pSub)->Scale_x = 0x1000;
        EM08_BODY_OF(pSub)->Scale_y = 0x1000;
        EM08_BODY_OF(pSub)->Scale_z = 0x1000;
        if ((G.pEm->Type & 0xF) == 8)
        {
            FUNC(CLUT_FADE_SABUN_ADD)(-1, -2, 0, 0, 0x200, 8);
            pSub->Old_z = 0;
        }
        (ROUTINE_WORD(pSub)) = 0;  // INT* view of the field (a plain access changes the code)
        if (G.pEm->Type == 4)
        {
            pSub->Life = -1;
            pSub->Routine_0 = 2;
            pSub->Routine_3 = 3;
            pSub->Ground = 0xA20;
            EM08_BODY_OF(pSub)->Scale_x = 0x19DC;
            EM08_BODY_OF(pSub)->Scale_y = 0x818;
            EM08_BODY_OF(pSub)->Scale_z = 0x19DC;
            pSub->Hokan_flg = 1;
        }
        ScratchWord[7] = 0x606060;
        ScratchPad->Sv.vz = 0;
        ScratchPad->Sv.vy = 0;
        ScratchPad->Sv.vx = 0;
        FUNC(KAGE_WORK_INIT)(&ScratchPad->Sv, &pSub->Kage_work, 0xBB8, 0xBB8);
        pEm = MAIN.Enemy;
        ScratchWord[0] = G.Enemy_num;
        while (ScratchWord[0] != 0)
        {
            pEm->pTgt = pSub;
            pEm++;
            ScratchWord[0]--;
        }
        FUNC(SET_ENEMY_WORK)(1, 0xDC, G.pEm->Parts_num, &EM08_FREE_P->roots);
        pSub = EM08_FREE_P->roots;
        pSub->Cdir_x = 0;
        pSub->Cdir_y = 0;
        pSub->Cdir_z = 0;
        pSub->Timer0 = 0x59;
        pSub->Type = 0;
        pSub->Damage_no = 0;
        pSub->Spd_y = 0;
        pSub->Spd_z = 1;
        pSub->Old_x = 0;
        pSub->Old_y = 1;
        pSub->Scale = 0x1000;
        pSub->Spd_x = 0;
        if (G.pEm->Type == 4)
        {
            pSub->Scale = 0x3E8;
            pSub->Pos_y = -0x514;
            pSub->Routine_0 = 2;
            pSub->Routine_3 = 2;
        }
    }
    if (G.pEm->Type & 0x40)
    {
        G.pEm->Routine_0 = 8;
    }
    if (G.pEm->Type == 4)
    {
        G.pEm->Routine_0 = 4;
        FUNC(CLUT_FADE_SABUN_ADD)(-4, -5, 0, 0, 0x200, 8);
    }
    if ((G.pEm->Type & 0xF) == 6)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 0xB, 1, 8);
        Em08_Act_suspend();
    }

    if (G.pEm->Type == 5)
    {
        G.pEm->Scale = 0x12C0;
    }
}

VOID Em08_Main(VOID)
{
    PARTS_W* pP_ptr;
    INT unused;    // unused local: reserves stack space
    UCHAR Rno_m1;
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        Rno_m1 = G.pEm->Routine_0 + 0xFF;

        if ((G.pEm->Type & 0x40) && Rno_m1 < 2U)
        {
            if (EM_ROUTINE_UHALF2 == 0x309)
            {
                EM_ROUTINE_WORD = ROUTINES(3, 0xF, 0, 8);
            }
            else
            {
                EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 8);
            }
        }
        Em08_routine_0[G.pEm->Routine_0]();
        pP_ptr = G.pEm->pSin_parts_ptr;
        G.pEm->pSca_ofs->Off_x = pP_ptr[14].Workm.t[0] - G.pEm->Pos_x;
        G.pEm->pSca_ofs->Off_y = pP_ptr[14].Workm.t[1] - G.pEm->Pos_y;
        G.pEm->pSca_ofs->Off_z = pP_ptr[14].Workm.t[2] - G.pEm->Pos_z;
    }
    if (!(G.pEm->Type & 1))
    {
        Em08_Disp_copy();
        pP_ptr = G.pEm->pSin_parts_ptr;
    }
    if (G.pEm->Type == 4)
    {
        G.pEm->Zone_in_flg = 0;
        return;
    }
    pP_ptr = G.pEm->pSin_parts_ptr;
    if (!(G.pEm->Damage_no & 0x80))
    {
        FUNC(KAGE_HALF_SET)(&G.pEm->Kage_work, pP_ptr[11].Workm.t[1] / 7 + 0x7D0, pP_ptr[11].Workm.t[1] / 70 + 0x12C);
    }
    FUNC(MUL_KAGE)(pP_ptr[11].Workm.t, &G.pEm->Kage_work, 0, G.pEm->Cdir_y);
}

VOID Em08_Move(VOID)
{
    Em08_Move_main();
}

VOID Em08_Move_main(VOID)
{
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    G.pEm->Be_flg = G.pEm->Be_flg | 0x40;
    EM08_FREE_P->player_distance = FUNC(SQUARE_ROOT0)((G.Player.Pos_x - G.pEm->Pos_x) * (G.Player.Pos_x - G.pEm->Pos_x) + (G.Player.Pos_z - G.pEm->Pos_z) * (G.Player.Pos_z - G.pEm->Pos_z));
    if (EM08_FREE_P->player_distance < 0x2710U)
    {
        G.pEm->Be_flg = G.pEm->Be_flg | 0x80;
    }
    if (G.pEm->Routine_1 == 0)
    {
        if (G.pEm->pTgt->Damage_no != 0)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 0, 1, 2);
            if (!(Api_rand() & 3))
            {
                G.pEm->Routine_2 = 1U;
            }
            if (G.pEm->Type & 1)
            {
                if (Api_rand() & 3)
                {
                    EM_ROUTINE_WORD = ROUTINES(0, 1, 1, 1);
                    G.pEm->Routine_2 += !(Api_rand() & 3);
                }
            }
            return;
        }
        Em08_Sel_run();
    }
    Em08_Act_run();
    if (--EM08_FREE_P->ambient_effect_timer == 0)
    {
        EM08_FREE_P->ambient_effect_timer = (Api_rand() & 0x3F) + 0x32;
    }
    if (!(G.pEm->Type & 1))
    {
        if (EM08_FREE_P->ambient_effect_timer == 1 && EM08_FREE_P->player_distance >= 0x1965U)
        {
            ScratchPad->Vec.vy = -0x2710;
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + G.Player.Pos_x - 0x100;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + G.Player.Pos_z - 0x100;

            if (!(Api_rand() & 1))
            {
                FUNC(ESP_CALL)(0x1E, 1, 0, 0, &ScratchPad->Vec, 0);
            }
        }

        if (EM08_FREE_P->ambient_effect_timer < 0x1E && EM08_FREE_P->player_distance >= 0x1965U && !((USHORT)EM08_FREE_P->ambient_effect_timer & 3))  // truncates to an unsigned halfword
        {
            ScratchPad->Vec.vy = -0x2710;
            ScratchPad->Vec.vx = G.Player.Pos_x - KEEP_VALUE((Api_rand() & 0x1FF) - 0x100);  // KEEP_VALUE keeps the operand a separate value; the plain form and a cached local change the code
            ScratchPad->Vec.vz = G.Player.Pos_z - KEEP_VALUE((Api_rand() & 0x1FF) - 0x100);  // KEEP_VALUE keeps the operand a separate value; the plain form and a cached local change the code

            FUNC(ESP_CALL)(0, 0x1C, 0, 0, &ScratchPad->Vec, 0);
        }
    }
    if (EM08_FREE_P->ambient_effect_timer % 50 == 0)
    {
        ScratchPad->Vec.vy = -0x2710;
        ScratchPad->Vec.vx = Em08_esp_ofs_x[Api_rand() & 1] + (KEEP_VALUE(-0x400) + (Api_rand() & 0x7FF));  // KEEP_VALUE keeps the constant out of GCC 2.6.3's folding; the plain form and a local change the code
        ScratchPad->Vec.vz = Em08_esp_ofs_z[Api_rand() & 1] + (KEEP_VALUE(-0x400) + (Api_rand() & 0x7FF));  // KEEP_VALUE keeps the constant out of GCC 2.6.3's folding; the plain form and a local change the code

        FUNC(ESP_CALL)(0, 0x1C, 0, 0, &ScratchPad->Vec, 0);
    }
}

VOID Em08_Damage(VOID)
{
    Em08_Dm_sel();
    if (--EM08_FREE_P->ambient_effect_timer == 0)
    {
        EM08_FREE_P->ambient_effect_timer = (Api_rand() & 0x3F) + 0x32;
    }
    if (!(G.pEm->Type & 1))
    {
        if (EM08_FREE_P->ambient_effect_timer == 1 && EM08_FREE_P->player_distance >= 0x1965U)
        {
            ScratchPad->Vec.vy = -0x2710;
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + G.Player.Pos_x - 0x100;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + G.Player.Pos_z - 0x100;

            if (!(Api_rand() & 1))
            {
                FUNC(ESP_CALL)(0x1E, 1, 0, 0, &ScratchPad->Vec, 0);
            }
        }

        if (EM08_FREE_P->ambient_effect_timer < 0x1E && EM08_FREE_P->player_distance >= 0x1965U && !((USHORT)EM08_FREE_P->ambient_effect_timer & 3))  // truncates to an unsigned halfword
        {
            ScratchPad->Vec.vy = -0x2710;
            ScratchPad->Vec.vx = G.Player.Pos_x - KEEP_VALUE((Api_rand() & 0x1FF) - 0x100);  // KEEP_VALUE keeps the operand a separate value; the plain form and a cached local change the code
            ScratchPad->Vec.vz = G.Player.Pos_z - KEEP_VALUE((Api_rand() & 0x1FF) - 0x100);  // KEEP_VALUE keeps the operand a separate value; the plain form and a cached local change the code

            FUNC(ESP_CALL)(0, 0x1C, 0, 0, &ScratchPad->Vec, 0);
        }
    }
    if (EM08_FREE_P->ambient_effect_timer % 50 == 0)
    {
        ScratchPad->Vec.vy = -0x2710;
        ScratchPad->Vec.vx = Em08_esp_ofs_x[Api_rand() & 1] + (KEEP_VALUE(-0x400) + (Api_rand() & 0x7FF));  // KEEP_VALUE keeps the constant out of GCC 2.6.3's folding; the plain form and a local change the code
        ScratchPad->Vec.vz = Em08_esp_ofs_z[Api_rand() & 1] + (KEEP_VALUE(-0x400) + (Api_rand() & 0x7FF));  // KEEP_VALUE keeps the constant out of GCC 2.6.3's folding; the plain form and a local change the code

        FUNC(ESP_CALL)(0, 0x1C, 0, 0, &ScratchPad->Vec, 0);
    }
}

VOID Em08_Dm_sel(VOID)
{
    ENEMY_WORK* pSub;
    PARTS_W* pP_ptr;

    pSub = G.pEm->pTgt;
    if (G.pEm->pTgt->Life & 0x8000)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 0, 1, 3);
        G.pEm->Damage_no = 1;
        G.pEm->Life = -1;
        return;
    }
    if (G.pEm->Routine_1 == 0 && (G.pEm->Damage_no & 7))
    {
        G.pEm->Damage_no = G.pEm->Damage_no & 0xF8;
        G.pEm->Damage_no = G.pEm->Damage_no | 1;
        EM_ROUTINE_WORD = ROUTINES(0, 0, 1, 2);
        if (Api_rand() & 1)
        {
            G.pEm->Routine_2 = 1;
        }
        if ((G.pEm->Damage_no & 0x78) == 8)
        {
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0xC8;
            pP_ptr = G.Player.pSin_parts_ptr;

            FUNC(ESP_CALL)(0U, 0x18, 0, &pP_ptr[14].Workm, &ScratchPad->Vec, 0);
        }
        else if (!(pSub->Damage_no & 7))
        {
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchWord[0] = Api_rand() & 7;

            FUNC(ESP_CALL)(0U, 0x1B, 0, &pP_ptr[ScratchWord[0] + 7].Workm, &ScratchPad->Vec, 0);

            ScratchPad->Vec.vx = G.Player.Pos_x - G.pEm->Pos_x;
            ScratchPad->Vec.vy = G.pEm->Pos_y;
            ScratchPad->Vec.vz = G.Player.Pos_z - G.pEm->Pos_z;
            FUNC(VECTOR_NORMAL)(&ScratchPad->Vec, &ScratchPad->Vec);
            ScratchPad->Vec.vx = ScratchPad->Vec.vx >> 1;
            ScratchPad->Vec.vy = ScratchPad->Vec.vy >> 1;
            ScratchPad->Vec.vz = ScratchPad->Vec.vz >> 1;
            ScratchPad->Vec.vx = ScratchPad->Vec.vx + G.pEm->Pos_x;
            ScratchPad->Vec.vy = G.pEm->Pos_y + 0x1194;
            ScratchPad->Vec.vz = ScratchPad->Vec.vz + G.pEm->Pos_z;

            FUNC(ESP_CALL)(0U, 0x18, 0, Monitor.pId_mat, &ScratchPad->Vec, 0);
        }
        if ((Api_rand() & 1) && (G.pEm->Type & 1) && G.pEm->Life < 0xA)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 0, 1, 3);
            G.pEm->Life = -1;
            pSub->Old_z--;

            if (pSub->Old_z == 1)
            {
                FUNC(FLG_ON)(&MAIN.SaveData.Scenario_flg, 0x29);  // address passed as an integer
                if (G.Player.Id & 1)
                {
                    G.Stop_flg |= STOP_PAD_LOCK;
                }
                ScratchWord[0] = 6;
                while ((ScratchWord[0] = ScratchWord[0] - 1) != -1)
                {
                    if (!((MAIN.Enemy + ScratchWord[0])->Life & 0x8000))
                    {
                        ROUTINE_WORD(MAIN.Enemy + ScratchWord[0]) = (0) << 24 | (9) << 16 | (1) << 8 | (1);  // INT* view of the field (a plain access changes the code)
                        (MAIN.Enemy + ScratchWord[0])->Damage_no = 1;
                    }
                }
            }
            return;
        }
        if (pSub->Old_z < 2)
        {
            G.pEm->Damage_no = 0;
        }
        pSub->Damage_no = 1U;
        pSub->Type = 1;
        return;
    }
    switch (G.pEm->Routine_2)
    {
    case 0:
        Em08_Act_recoil();
        return;
    case 1:
        Em08_Act_flinch();
    }
}

VOID Em08_Nop(VOID)
{
}

VOID Em08_Die(VOID)
{
    if (G.pEm->Routine_1 == 0)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 0, 1, 3);
        ScratchWord[1] = --G.pEm->pTgt->Old_z;
        if (ScratchWord[1] == 1)
        {
            FUNC(FLG_ON)(&MAIN.SaveData.Scenario_flg, 0x29);  // address passed as an integer
            if (G.Player.Id & 1)
            {
                G.Stop_flg |= STOP_PAD_LOCK;
            }
            ScratchWord[0] = 6;
            while (--ScratchWord[0] != -1)
            {
                if (!((MAIN.Enemy + ScratchWord[0])->Life & 0x8000))
                {
                    ROUTINE_WORD(MAIN.Enemy + ScratchWord[0]) = (0) << 24 | (9) << 16 | (1) << 8 | (1);  // INT* view of the field (a plain access changes the code)
                    (MAIN.Enemy + ScratchWord[0])->Damage_no = 1;
                }
            }
        }
        if (!(G.pEm->Type & 1))
        {
            G.pEm->pTgt->Life = -1;
            EM_TGT_ROUTINE_WORD = 2;
            VIEW(INT, EM08_FREE_P->roots->Routine_0) = 2;  // INT* view of the field (a plain access changes the code)
        }
    }
    if (G.pEm->Routine_2 != 0)
    {
        return;
    }
    Em08_Act_death();
    return;
}

VOID Em08_Dead(VOID)
{
}

VOID Em08_Act_run(VOID)
{
    if (G.pEm->pTgt->Life & 0x8000)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 0, 1, 3);
        G.pEm->Damage_no = 1;
        G.pEm->Life = -1;
        return;
    }
    Em08_act_tbl[G.pEm->Routine_2]();
}

VOID Em08_Sel_run(VOID)
{
    ScratchWord[0] = EM08_FREE_P->player_distance;

    Em08_sel_tbl[(G.pEm->Type & 7)]();
}

VOID Em08_Sel_def(VOID)
{
    USHORT Tilt;
    INT pad14;    // unused local: reserves stack space
    UCHAR actions[8] = { 1, 2, 3, 1, 3, 1, 1, 2 };

    if (G.Player.Damage_no != 0)
    {
        return;
    }
    if (ScratchWord[0] < 0x2AF8)
    {
        G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 4);  // address passed as an integer
    }
    if (ScratchWord[0] < 0x2710)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x80) == 0 && G.Player.Damage_no == 0)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 1, 1, 1);
            G.pEm->Routine_2 = G.pEm->Routine_2 + (Api_rand() & 1);
        }
    }
    Tilt = (G.pEm->Cdir_z - 0xE80) & 0xFFF;
    if (ScratchWord[0] < 0x2328)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x100) == 0 && G.Player.Damage_no == 0)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);
            G.pEm->Routine_2 = actions[(Api_rand() & 7)];
        }
    }
}

VOID Em08_Act_nop(VOID)
{
}

VOID Em08_Sel_nop(VOID)
{
}

VOID Em08_Sel_type5(VOID)
{
    INT pad10;    // unused local: reserves stack space
    INT pad14;    // unused local: reserves stack space
    UCHAR actions[8] = { 1, 2, 1, 1, 2, 1, 1, 2 };

    if (G.Player.Damage_no != 0)
    {
        return;
    }
    if (ScratchWord[0] < 0x2AF8)
    {
        G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 4);  // address passed as an integer
    }
    if (ScratchWord[0] < 0x2710)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x80) == 0 && G.Player.Damage_no == 0)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 1, 1, 1);
            G.pEm->Routine_2 = G.pEm->Routine_2 + (Api_rand() & 1);
        }
    }
    if (ScratchWord[0] < 0x2328)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x100) == 0 && G.Player.Damage_no == 0)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);
            G.pEm->Routine_2 = actions[Api_rand() & 7];
        }
    }
}

VOID Em08_Act_idle(VOID)
{
    USHORT Tilt;
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Timer0 = (Api_rand() & 0xF) + 0xF;
        G.pEm->Hokan_flg = 0x3F;
        EM08_FREE_P->Yaw_add = Api_rand() & 0xF;
        EM08_FREE_P->Tilt_add = Api_rand() & 7;
        EM08_FREE_P->Yaw_add -= EM08_FREE_P->Yaw_add * (Api_rand() & 2);
        EM08_FREE_P->Tilt_add -= EM08_FREE_P->Tilt_add * (Api_rand() & 2);
        if (G.pEm->Move_no == 8)
        {
            G.pEm->Move_no = 9U;
        }
        G.pEm->Cdir_y = (G.pEm->Cdir_y - EM08_FREE_P->Yaw_add / 2) & 0xFFF;
        G.pEm->Cdir_z = G.pEm->Cdir_z - EM08_FREE_P->Tilt_add / 2;

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        break;
    case 2:
        G.pEm->Routine_3 = G.pEm->Routine_3 + Em08_Joint_move(1, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        break;
    case 3:
        G.pEm->Routine_3 = 0U;
        break;
    }
    if (!(G.pEm->Type & 0x40))
    {
        G.pEm->Cdir_y = (G.pEm->Cdir_y + EM08_FREE_P->Yaw_add) & 0xFFF;
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 8U);  // address passed as an integer
    }
    ScratchWord[0] = G.pEm->Cdir_z;
    G.pEm->Cdir_z = G.pEm->Cdir_z + EM08_FREE_P->Tilt_add;
    pP_ptr = G.pEm->pSin_parts_ptr;
    Tilt = (G.pEm->Cdir_z - 0x3E) & 0xFFF;
    if (Tilt < 0xE32U)
    {
        G.pEm->Cdir_z = G.pEm->Cdir_z - EM08_FREE_P->Tilt_add;
    }
    if (pP_ptr[14].Workm.t[1] >= -0x257)
    {
        G.pEm->Cdir_z = ScratchWord[0] - 0x10;
        G.pEm->Move_no = Em08_idle_mot_tbl[((UINT)Api_rand()) % 10];  // unsigned conversion (signed operand)
        G.pEm->Hokan_flg = 0x3F;
    }
    if (G.pEm->Timer0-- == 0)
    {
        G.pEm->Move_no = Em08_idle_mot_tbl[((UINT)Api_rand()) % 10];  // unsigned conversion (signed operand)
        G.pEm->Hokan_flg = 0x3F;
    }
}

VOID Em08_Act_sweep(VOID)
{
    struct
    {
        UCHAR a;
        UCHAR b;
        SCHAR c;
    } Sweep_tbl[2] = { { 0, 1, 1 }, { 1, 0, -1 } };
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;
        G.pEm->Move_no = 0xC;
        G.pEm->Timer0 = 0x32U;
        EM08_FREE_P->Sweep_dir = 1;
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 1) >= 0U)  // address passed as an integer
        {
        }
        else
        {
            EM08_FREE_P->Sweep_dir = 0;
        }

    case 1:
        Em08_Joint_move(Sweep_tbl[EM08_FREE_P->Sweep_dir].b, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        G.pEm->Cdir_y += Sweep_tbl[EM08_FREE_P->Sweep_dir].c * 0x10;
        pP_ptr = G.pEm->pSin_parts_ptr;
        if (pP_ptr[14].Workm.t[1] < -0x9C4)
        {
            G.pEm->Cdir_z += 8;
        }
        if (pP_ptr[14].Workm.t[1] >= -0x3E7)
        {
            G.pEm->Cdir_z = G.pEm->Cdir_z - 8;
        }
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 2U;
        }
        return;
    case 2:
        G.pEm->Timer0 = 0xFU;
        G.pEm->Routine_3 = 3U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;

    case 3:
        Em08_Joint_move(Sweep_tbl[EM08_FREE_P->Sweep_dir].a, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 4U;
        }
        return;
    case 4:
        G.pEm->Routine_3 = 5U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;
        G.pEm->Move_no = 0xC;

    case 5:
        EM08_FREE_P->Rot_spd = EM08_FREE_P->Rot_spd + 4;
        if (EM08_FREE_P->Rot_spd >= 0x29)
        {
            EM08_FREE_P->Rot_spd = EM08_FREE_P->Rot_spd + 0x1C;
        }
        Em08_Joint_move(Sweep_tbl[EM08_FREE_P->Sweep_dir].b, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        G.pEm->Cdir_y -= EM08_FREE_P->Rot_spd * Sweep_tbl[EM08_FREE_P->Sweep_dir].c;
        if (EM08_FREE_P->Rot_spd >= 0xC1)
        {
            G.pEm->Routine_3 = 6U;
            FUNC(SND_SE_STAD)(2, 3, 0, G.pEm->pSin_parts_ptr[14].Workm.t);
        }
        Em08_Sweep_hit();
        return;
    case 6:
        G.pEm->Cdir_z = G.pEm->Cdir_z - 8;
        Em08_Joint_move(Sweep_tbl[EM08_FREE_P->Sweep_dir].a, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        Em08_Sweep_hit();
        if (EM08_FREE_P->Rot_spd > 0)
        {
            G.pEm->Cdir_y -= EM08_FREE_P->Rot_spd * Sweep_tbl[EM08_FREE_P->Sweep_dir].c;
            EM08_FREE_P->Rot_spd = EM08_FREE_P->Rot_spd - 0x10;
            G.pEm->Damage_no = 0;
            if (!(G.pEm->Type & 1) && G.pEm->pTgt->Old_z >= 2)
            {
                G.pEm->Damage_no = 1;
            }
            return;
        }
        G.pEm->Routine_3 = 7U;
        G.pEm->Cdir_y = G.pEm->Cdir_y - Sweep_tbl[EM08_FREE_P->Sweep_dir].c * 8;
        return;
    case 7:
        G.pEm->Cdir_y += Sweep_tbl[EM08_FREE_P->Sweep_dir].c * 8;
        EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
        EM08_FREE_P->Rot_spd = 0;

    default: LABEL_END
    }
}

VOID Em08_Sweep_hit(VOID)
{
    USHORT dist;

    dist = EM08_FREE_P->player_distance + 0xF830;

    if (G.pEm->Type == 5)
    {
        dist = EM08_FREE_P->player_distance + 0xF254;
    }

    if (dist < 0x157CU)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, EM08_FREE_P->Rot_spd) == 0 && G.Player.Damage_no == 0 && EM08_FREE_P->Rot_spd >= 0x51)
        {
            G.pEm->Cdir_z = G.pEm->Cdir_z - 0xC8;
            G.pEm->Routine_3 = 6;
            G.pEm->Cdir_y = G.pEm->Cdir_y + 0x400;
            ScratchWord[7] = FUNC(CDIR_CK)(&MAIN.Player);
            G.pEm->Cdir_y = G.pEm->Cdir_y - 0x400;
            G.Player.Damage_no = 1;
            if (ScratchWord[7] != 0)
            {
                if (EM08_FREE_P->Sweep_dir == 0)
                {
                    PL_ROUTINE_WORD = ROUTINES(0, 0, 8, 6);
                    G.Player.Cdir_y = G.pEm->Cdir_y + 0x400;
                    FUNC(SND_SE_STAD)(2, 4, 0, G.pEm->pSin_parts_ptr[14].Workm.t);
                }
                else
                {
                    PL_ROUTINE_WORD = ROUTINES(5, 2, 8, 6);
                    Em08_catch_mat.t[0] = G.pEm->Cdir_y + 0x400;
                    FUNC(SND_SE_STAD)(2, 2, 0, G.pEm->pSin_parts_ptr[14].Workm.t);
                }
            }
            else if (EM08_FREE_P->Sweep_dir != 0)
            {
                PL_ROUTINE_WORD = ROUTINES(0, 0, 8, 6);
                G.Player.Cdir_y = G.pEm->Cdir_y - 0x400;
                FUNC(SND_SE_STAD)(2, 4, 0, G.pEm->pSin_parts_ptr[14].Workm.t);
            }
            else
            {
                PL_ROUTINE_WORD = ROUTINES(5, 2, 8, 6);
                Em08_catch_mat.t[0] = G.pEm->Cdir_y - 0x400;
                FUNC(SND_SE_STAD)(2, 2, 0, G.pEm->pSin_parts_ptr[14].Workm.t);
            }

            G.Player.Life -= 0x19;

            if (G.Player.Life < 0)
            {
                G.Player.Life = 1;
            }
        }
    }
}

VOID Em08_Act_spit(VOID)
{
    PARTS_W* pP_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;
        G.pEm->Move_no = 7;
        G.pEm->Damage_no = 1;
        EM08_FREE_P->Rot_spd = 4U;

    case 1:
        if (Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40) & 0xFF)
        {
            G.pEm->Routine_3 = 2;
            G.pEm->Cdir_z = G.pEm->Cdir_z - EM08_FREE_P->Rot_spd;
            EM08_FREE_P->Rot_spd = EM08_FREE_P->Rot_spd - 0x30;
            FUNC(SND_SE_STAD)(2, 1, 0, G.pEm->pSin_parts_ptr[14].Workm.t);
            return;
        }
        ScratchWord[0] = G.pEm->pTgt->Damage_no * 0x20 + 0x10;
        FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, ScratchWord[0]);
        if (G.pEm->Move_cnt < 8U)
        {
            G.pEm->Cdir_z += 0x40;
            return;
        }
        if (G.pEm->Move_cnt >= 0xDU)
        {
            G.pEm->Cdir_z = G.pEm->Cdir_z - EM08_FREE_P->Rot_spd;
            EM08_FREE_P->Rot_spd += 0x18;
        }
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        if (G.pEm->Move_cnt >= 0xBU && G.Player.Damage_no == 0)
        {
            if (FUNC(L_POS_CK_XZM)(&pP_ptr[15].Workm, &ScratchPad->Vec, 0x4B0, &MAIN.Player.Pos_x) != 0 && pP_ptr[15].Workm.t[1] >= -0x9C3)
            {
                G.Player.Damage_no = 1;

                PL_ROUTINE_WORD = ROUTINES(0, 0x64, 0, 2);

                FUNC(PAD_VIB_SET1)(5, 0xFA, 4);

                EM08_FREE_P->Rot_spd = EM08_FREE_P->Rot_spd - 8;

                G.Player.Life -= 0xF;

                if (G.Player.Life < 0)
                {
                    G.Player.Life = 1;
                }

                FUNC(SND_SE_STAD)(2, 2, 0, G.pEm->pSin_parts_ptr[14].Workm.t);
            }
        }
        return;
    case 2:
        G.pEm->Cdir_z = G.pEm->Cdir_z - EM08_FREE_P->Rot_spd;
        EM08_FREE_P->Rot_spd = EM08_FREE_P->Rot_spd - 8;
        if (EM08_FREE_P->Rot_spd < 0)
        {
            G.pEm->Routine_3 = 3;
        }
        return;
    case 3:
        G.pEm->Move_no = 2;
        G.pEm->Routine_3 = 4;
        EM08_FREE_P->Rot_spd = 0x10 - (Api_rand() & 1) * 0x20;
        G.pEm->Hokan_flg = 0x3F;

    case 4:
        G.pEm->Cdir_z += 0x10;
        if (G.pEm->Cdir_z >= -0x18F)
        {
            G.pEm->Routine_3 = 5;
        }
        G.pEm->Cdir_y += EM08_FREE_P->Rot_spd;
        Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        return;
    case 5:
        G.pEm->Cdir_y += EM08_FREE_P->Rot_spd / 2;
        EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
        G.pEm->Move_no = 4;
        G.pEm->Damage_no = 0;
        if (!(G.pEm->Type & 1) && G.pEm->pTgt->Old_z >= 2)
        {
            G.pEm->Damage_no = 1;
        }
    }
}

VOID Em08_Act_move(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;
        G.pEm->Move_no = 8;
        G.pEm->Timer0 = 0x3CU;
        EM08_FREE_P->Rot_spd = 8;

    case 1:
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 5U;
        }
        G.pEm->Cdir_y -= 0x28;
        G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x10);  // address passed as an integer
        G.pEm->Cdir_y = G.pEm->Cdir_y + 0x28;
        Em08_Joint_move(1, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        pP_ptr = G.pEm->pSin_parts_ptr;
        if (pP_ptr[14].Workm.t[1] < -0x9C4)
        {
            G.pEm->Cdir_z = G.pEm->Cdir_z + 0x10;
            break;
        }
        if (pP_ptr[14].Workm.t[1] >= -0x3E7)
        {
            G.pEm->Cdir_z -= 0x10;
            break;
        }
        if (G.pEm->Timer0 < 0x1E)
        {
            G.pEm->Routine_3 = 2U;
            G.pEm->Timer0 = 0x3CU;
        }
        break;
    case 2:
        if ((G.pEm->Timer0-- != 0) && G.Player.Damage_no == 0)
        {
        }
        else
        {
            G.pEm->Routine_3 = 5U;
        }
        G.pEm->Cdir_y -= 0x30;
        G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x10);  // address passed as an integer
        G.pEm->Cdir_y = G.pEm->Cdir_y + 0x30;
        pP_ptr = G.pEm->pSin_parts_ptr;
        ScratchWord[0] = __builtin_abs(G.Player.Pos_x - pP_ptr[14].Workm.t[0]) + __builtin_abs(G.Player.Pos_z - pP_ptr[14].Workm.t[2]);
        ScratchWord[1] = __builtin_abs(G.Player.Pos_x - pP_ptr[12].Workm.t[0]) + __builtin_abs(G.Player.Pos_z - pP_ptr[12].Workm.t[2]);

        if ((ScratchWord[0] < 0x320 || ScratchWord[1] < 0x1F4) && !(G.Player.Sca_info & 8) && G.Player.Damage_no == 0 && G.Player.Pos_y == 0 && FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x20) == -0x20)
        {
            G.pEm->Routine_3 = 3U;
            G.pEm->Move_no = 0xE;
            G.pEm->Damage_no = 1;
            EM08_FREE_P->Catch_parts = 0xF;
            PL_ROUTINE_WORD = ROUTINES(0, 1, 8, 6);
            G.Player.Damage_no = 1;
            G.Player.Spd_x = (pP_ptr[11].Workm.t[0] - G.Player.Pos_x) / 6;
            G.Player.Spd_y = (pP_ptr[11].Workm.t[1] + 0x7EF - G.Player.Pos_y) / 6;
            G.Player.Spd_z = (pP_ptr[11].Workm.t[2] - G.Player.Pos_z) / 6;
            G.pEm->Move_cnt = 0U;
            G.pEm->Move_wait = 0;
            G.pEm->Hokan_flg = 3;
            goto action_state3;
        }
        Em08_Joint_move(1, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        G.pEm->Cdir_z = G.pEm->Cdir_z + EM08_FREE_P->Rot_spd;
        if (pP_ptr[14].Workm.t[1] >= -0x5DB && !(EM08_FREE_P->Rot_spd & 0x80000000))
        {
            EM08_FREE_P->Rot_spd *= -1;
        }
        if (pP_ptr[14].Workm.t[1] < -0x9C4 && (EM08_FREE_P->Rot_spd & 0x80000000))
        {
            EM08_FREE_P->Rot_spd *= -1;
        }
        break;
    case 3:
    action_state3:
        pP_ptr = G.pEm->pSin_parts_ptr;
        if (G.pEm->Move_cnt >= 4U)
        {
            ScratchPad->Vec.vx = -0x60F;
            ScratchPad->Vec.vy = 0;
            ScratchPad->Vec.vz = 0x2BC;
            FUNC(APPLY_MATRIX_LV)(&pP_ptr[EM08_FREE_P->Catch_parts].Workm, &ScratchPad->Vec, &ScratchPad->Vec);
            G.Player.Spd_x = (ScratchPad->Vec.vx + pP_ptr[EM08_FREE_P->Catch_parts].Workm.t[0] - G.Player.Pos_x) / 2;
            G.Player.Spd_y = (ScratchPad->Vec.vy + pP_ptr[EM08_FREE_P->Catch_parts].Workm.t[1] - G.Player.Pos_y) / 2;
            G.Player.Spd_z = (ScratchPad->Vec.vz + pP_ptr[EM08_FREE_P->Catch_parts].Workm.t[2] - G.Player.Pos_z) / 2;
        }
        if (pP_ptr[14].Workm.t[1] >= -0x7CF)
        {
            G.pEm->Cdir_z -= 0x10;
        }
        G.Player.Pos_x += G.Player.Spd_x;
        G.Player.Pos_y += G.Player.Spd_y;
        G.Player.Pos_z += G.Player.Spd_z;
        G.pEm->Routine_3 = G.pEm->Routine_3 + Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        break;
    case 4:
        FUNC(SND_SE_STAD)(2, 5, 0, G.pEm->pSin_parts_ptr[14].Workm.t);
        EM_ROUTINE_WORD = ROUTINES(0, 4, 1, 1);
        break;
    case 5:
        G.pEm->Timer0 = 0x14U;
        G.pEm->Routine_3 = 6U;
        EM08_FREE_P->Rot_spd = 0;
        pP_ptr = G.pEm->pSin_parts_ptr;
        if (pP_ptr[14].Workm.t[1] >= -0x7CF)
        {
            EM08_FREE_P->Rot_spd = 0x10;
        }
        G.pEm->Move_no = 5;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;

    case 6:
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 7U;
        }
        G.pEm->Cdir_z = G.pEm->Cdir_z - EM08_FREE_P->Rot_spd;
        G.pEm->Cdir_y = G.pEm->Cdir_y + 8;
        Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        break;
    case 7:
        EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
        G.pEm->Move_no = 9;
        G.pEm->Damage_no = 0;
        if (!(G.pEm->Type & 1) && G.pEm->pTgt->Old_z >= 2)
        {
            G.pEm->Damage_no = 1;
        }
    }
    FUNC(KAGE_HALF_SET)(&MAIN.Player.Kage_work, G.Player.Pos_y / 17 + 0x1F4, G.Player.Pos_y / 17 + 0x2BC);  // address passed as an integer
}

VOID Em08_Act_hold(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Timer0 = 0xAU;
        G.pEm->Routine_3 = 1U;
        EM08_FREE_P->Rot_spd = 0x18;
        pP_ptr = G.pEm->pSin_parts_ptr;
        G.Player.Be_flg |= EM_BE_NO_HIT;
        FUNC(MUL_LMATRIX)(&pP_ptr[EM08_FREE_P->Catch_parts].Workm, &MAIN.Player.M, &Em08_catch_mat);  // address passed as an integer
        Em08_catch_mat.t[0] = -0x60F;
        Em08_catch_mat.t[1] = 0;
        Em08_catch_mat.t[2] = 0x2BC;
        FUNC(SND_SE_STAD)(2, 6, 0, G.pEm->pSin_parts_ptr[14].Workm.t);
        FUNC(SND_SE_STAD)(2, (G.Player.Id & 1) + 0x17, 0, &MAIN.Player.Pos_x);  // address passed as an integer

    case 1:
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 2U;
        }
        G.pEm->Cdir_z = G.pEm->Cdir_z - EM08_FREE_P->Rot_spd;
        pP_ptr = G.pEm->pSin_parts_ptr;
        G.Player.Zone_in_flg |= 0x80;
        FUNC(GS_MUL_COORD0)(&pP_ptr[EM08_FREE_P->Catch_parts].Workm, &Em08_catch_mat, &MAIN.Player.M);  // address passed as an integer
        break;
    case 2:
        G.pEm->Routine_3 = 3U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;
        G.pEm->Move_no = 0xF;
        if (G.Player.Life < 0x28)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 8, 1, 1);
            break;
        }
    case 3:
        G.pEm->Routine_3 += Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        G.pEm->Cdir_z = G.pEm->Cdir_z - 4;
        pP_ptr = G.pEm->pSin_parts_ptr;
        G.Player.Zone_in_flg |= 0x80;
        FUNC(GS_MUL_COORD0)(&pP_ptr[EM08_FREE_P->Catch_parts].Workm, &Em08_catch_mat, &MAIN.Player.M);  // address passed as an integer
        break;
    case 4:
        G.pEm->Timer0 = 0xFU;
        G.pEm->Routine_3 = 5U;

    case 5:
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 6U;
        }
        G.pEm->Cdir_z = G.pEm->Cdir_z - 8;
        pP_ptr = G.pEm->pSin_parts_ptr;
        G.Player.Zone_in_flg |= 0x80;
        FUNC(GS_MUL_COORD0)(&pP_ptr[EM08_FREE_P->Catch_parts].Workm, &Em08_catch_mat, &MAIN.Player.M);  // address passed as an integer
        break;
    case 6:
        G.pEm->Routine_3 = 7U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        G.pEm->Move_no = 0;
        EM08_FREE_P->Rot_spd = 8;

    case 7:
        G.pEm->Cdir_z += EM08_FREE_P->Rot_spd;
        EM08_FREE_P->Rot_spd = EM08_FREE_P->Rot_spd + 1;
        if (G.Player.Pos_y >= -0x5DB)
        {
            FUNC(SND_SE_STAD)(2, 0x1A, 0, &MAIN.Player.Pos_x);  // address passed as an integer
            goto block;
        }
        Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);
        pP_ptr = G.pEm->pSin_parts_ptr;
        G.Player.Zone_in_flg |= 0x80;
        FUNC(GS_MUL_COORD0)(&pP_ptr[EM08_FREE_P->Catch_parts].Workm, &Em08_catch_mat, &MAIN.Player.M);  // address passed as an integer
        break;
    case 8:
    block:
        G.pEm->Routine_3 = 9U;
        G.Player.Pos_y = 0;
        G.Player.Zone_in_flg &= 0x7F;
        G.Player.Be_flg &= ~EM_BE_NO_HIT;
        G.pEm->Move_no = 3;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;
        PL_ROUTINE_WORD = ROUTINES(0, 2, 8, 6);

        G.Player.Life = G.Player.Life - 0x28;

    case 9:
        G.pEm->Routine_3 += Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        G.pEm->Cdir_z -= 2;
        break;
    case 10:
        G.pEm->Damage_no = 0;
        EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
        if (!(G.pEm->Type & 1) && G.pEm->pTgt->Old_z >= 2)
        {
            G.pEm->Damage_no = 1;
        }
    }
    FUNC(KAGE_HALF_SET)(&MAIN.Player.Kage_work, G.Player.Pos_y / 17 + 0x1F4, G.Player.Pos_y / 17 + 0x2BC);  // address passed as an integer
}

VOID Em08_Act_grab(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;
        G.pEm->Move_no = 8;
        G.pEm->Timer0 = 0x1EU;

    case 1:
        G.pEm->Cdir_y = G.pEm->Cdir_y - 0x30;
        G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x10);  // address passed as an integer
        G.pEm->Cdir_y = G.pEm->Cdir_y + 0x30;
        Em08_Joint_move(1, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        pP_ptr = G.pEm->pSin_parts_ptr;
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 2U;
            G.pEm->Move_no = 0xE;
            G.Player.Spd_x = (pP_ptr[11].Workm.t[0] - G.Player.Pos_x) / 6;
            G.Player.Spd_y = (pP_ptr[11].Workm.t[1] + 0x7EF - G.Player.Pos_y) / 6;
            G.Player.Spd_z = (pP_ptr[11].Workm.t[2] - G.Player.Pos_z) / 6;
            G.pEm->Move_cnt = 0U;
            G.pEm->Move_wait = 0;
            G.pEm->Hokan_flg = 3;
        }
        if (pP_ptr[14].Workm.t[1] < -0x9C4)
        {
            G.pEm->Cdir_z = G.pEm->Cdir_z + 0x10;
        }
        if (pP_ptr[14].Workm.t[1] >= -0x3E7)
        {
            G.pEm->Cdir_z = G.pEm->Cdir_z - 0x10;
        }
        break;
    case 2:
        pP_ptr = G.pEm->pSin_parts_ptr;
        if (G.pEm->Move_cnt >= 4U)
        {
            ScratchPad->Vec.vx = -0x60F;
            ScratchPad->Vec.vy = 0;
            ScratchPad->Vec.vz = 0x2BC;
            FUNC(APPLY_MATRIX_LV)(&pP_ptr[15].Workm, &ScratchPad->Vec, &ScratchPad->Vec);
            G.Player.Spd_x = (ScratchPad->Vec.vx + pP_ptr[15].Workm.t[0] - G.Player.Pos_x) / 2;
            G.Player.Spd_y = (ScratchPad->Vec.vy + pP_ptr[15].Workm.t[1] - G.Player.Pos_y) / 2;
            G.Player.Spd_z = (ScratchPad->Vec.vz + pP_ptr[15].Workm.t[2] - G.Player.Pos_z) / 2;
        }
        G.Player.Pos_x += G.Player.Spd_x;
        G.Player.Pos_y += G.Player.Spd_y;
        G.Player.Pos_z += G.Player.Spd_z;
        G.pEm->Routine_3 = G.pEm->Routine_3 + Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        break;
    case 3:
        G.pEm->Timer0 = 0xAU;
        G.pEm->Routine_3 = 4U;
        EM08_FREE_P->Rot_spd = 0x18;
        pP_ptr = G.pEm->pSin_parts_ptr;
        G.Player.Be_flg |= EM_BE_NO_HIT;
        FUNC(MUL_LMATRIX)(&pP_ptr[15].Workm, &MAIN.Player.M, &Em08_catch_mat);  // address passed as an integer
        Em08_catch_mat.t[0] = -0x60F;
        Em08_catch_mat.t[1] = 0;
        Em08_catch_mat.t[2] = 0x2BC;
        FUNC(SND_SE_STAD)(2, 5, 0, G.pEm->pSin_parts_ptr[14].Workm.t);

        FUNC(PAD_VIB_SET1)(8, 0x8C, 0);
        FUNC(PAD_VIB_SET1)(8, 0x78, 0x14);
        FUNC(PAD_VIB_SET1)(0xC, 0x64, 0x23);

    case 4:
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 5U;
            FUNC(SND_SE_STAD)(2, 6, 0, G.pEm->pSin_parts_ptr[14].Workm.t);
            FUNC(SND_SE_STAD)(2, (G.Player.Id & 1) + 0x17, 0, &MAIN.Player.Pos_x);  // address passed as an integer
        }
        G.pEm->Cdir_z = G.pEm->Cdir_z - EM08_FREE_P->Rot_spd;
        pP_ptr = G.pEm->pSin_parts_ptr;
        G.Player.Zone_in_flg |= 0x80;
        FUNC(GS_MUL_COORD0)(&pP_ptr[15].Workm, &Em08_catch_mat, &MAIN.Player.M);  // address passed as an integer
        break;
    case 5:
        pP_ptr = G.pEm->pSin_parts_ptr;
        G.Player.Zone_in_flg |= 0x80;
        FUNC(GS_MUL_COORD0)(&pP_ptr[15].Workm, &Em08_catch_mat, &MAIN.Player.M);  // address passed as an integer
    }
    FUNC(KAGE_HALF_SET)(&MAIN.Player.Kage_work, G.Player.Pos_y / 17 + 0x1F4, G.Player.Pos_y / 17 + 0x2BC);  // address passed as an integer
}

VOID Em08_Act_suspend(VOID)
{
    PARTS_W* pP_ptr;
    USHORT Swing;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        pP_ptr = G.pEm->pSin_parts_ptr;
        G.pEm->Move_no = 0xE;
        G.pEm->Move_cnt = 4;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0;
        Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        if (!(G.Player.Id & 1) && (G.pEm->Type & 0xF) == 6)
        {
            Em08_catch_mat = Em08_hold_mat;
        }
        EM08_FREE_P->Yaw_add = 1;
        EM08_FREE_P->Tilt_add = 1;
        EM08_FREE_P->Yaw_swing = 3;
        EM08_FREE_P->Tilt_swing = 6;
        EM08_FREE_P->Rot_spd = 1;

        G.pEm->Timer0 = 0;

    case 1:

        if (G.pEm->Timer0 % 20 == 0)
        {
            FUNC(PAD_VIB_SET1)(8, 0x50, 0);
        }
        G.pEm->Timer0++;

        if (EM08_FREE_P->Rot_spd-- == 0)
        {
            G.pEm->Cdir_y = G.pEm->Cdir_y - EM08_FREE_P->Yaw_swing;
            G.pEm->Cdir_z = G.pEm->Cdir_z - EM08_FREE_P->Tilt_swing;
            EM08_FREE_P->Yaw_swing = EM08_FREE_P->Yaw_swing - EM08_FREE_P->Yaw_add;
            EM08_FREE_P->Tilt_swing = EM08_FREE_P->Tilt_swing - EM08_FREE_P->Tilt_add;
            Swing = EM08_FREE_P->Yaw_swing + 4;
            if (Swing >= 9U)
            {
                EM08_FREE_P->Yaw_add *= -1;
            }
            Swing = EM08_FREE_P->Tilt_swing + 7;
            if (Swing >= 0xFU)
            {
                EM08_FREE_P->Tilt_add *= -1;
            }
            EM08_FREE_P->Rot_spd = 1;
        }
        pP_ptr = G.pEm->pSin_parts_ptr;
        G.Player.Zone_in_flg |= 0x80;
        FUNC(GS_MUL_COORD0)(&pP_ptr[15].Workm, &Em08_catch_mat, &MAIN.Player.M);  // address passed as an integer
    }
    FUNC(KAGE_HALF_SET)(&MAIN.Player.Kage_work, G.Player.Pos_y / 17 + 0x1F4, G.Player.Pos_y / 17 + 0x2BC);  // address passed as an integer
}

VOID Em08_Act_release(VOID)
{
    PARTS_W* pP_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;
        G.pEm->Move_no = 0xF;
        PL_ROUTINE_WORD = ROUTINES(0, 1, 8, 6);
        FUNC(SND_SE_STAD)(2, 6, 0, G.pEm->pSin_parts_ptr[14].Workm.t);
        FUNC(SND_SE_STAD)(2, (G.Player.Id & 1) + 0x17, 0, &MAIN.Player.Pos_x);  // address passed as an integer

    case 1:
        G.pEm->Routine_3 += Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        G.pEm->Cdir_z = G.pEm->Cdir_z - 4;
        pP_ptr = G.pEm->pSin_parts_ptr;
        G.Player.Zone_in_flg |= 0x80;
        FUNC(GS_MUL_COORD0)(&pP_ptr[15].Workm, &Em08_catch_mat, &MAIN.Player.M);  // address passed as an integer
        break;
    case 2:
        G.pEm->Timer0 = 0xFU;
        G.pEm->Routine_3 = 3U;

    case 3:
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 4U;
        }
        G.pEm->Cdir_z = G.pEm->Cdir_z - 8;
        pP_ptr = G.pEm->pSin_parts_ptr;
        G.Player.Zone_in_flg |= 0x80;
        FUNC(GS_MUL_COORD0)(&pP_ptr[15].Workm, &Em08_catch_mat, &MAIN.Player.M);  // address passed as an integer
        break;
    case 4:
        G.pEm->Routine_3 = 5U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;
        G.pEm->Move_no = 0;
        EM08_FREE_P->Rot_spd = 8;

    case 5:
        G.pEm->Cdir_z += EM08_FREE_P->Rot_spd;
        EM08_FREE_P->Rot_spd = EM08_FREE_P->Rot_spd + 1;
        if (G.Player.Pos_y >= -0x5DB)
        {
            FUNC(SND_SE_STAD)(2, 0x1A, 0, &MAIN.Player.Pos_x);  // address passed as an integer
            goto block;
        }
        Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        pP_ptr = G.pEm->pSin_parts_ptr;
        G.Player.Zone_in_flg |= 0x80;
        FUNC(GS_MUL_COORD0)(&pP_ptr[15].Workm, &Em08_catch_mat, &MAIN.Player.M);  // address passed as an integer
        break;
    case 6:
    block:
        G.pEm->Routine_3 = 7;
        G.Player.Pos_y = 0;
        G.Player.Zone_in_flg &= 0x7F;
        G.Player.Be_flg &= ~EM_BE_NO_HIT;
        G.pEm->Move_no = 3;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;

        G.Player.Damage_no = 1;

        PL_ROUTINE_WORD = ROUTINES(0, 2, 8, 6);
        G.Player.Pos_x = 0x1757;
        G.Player.Pos_z = 0x3BBD;
        G.Player.Cdir_y = 0x499;
    case 7:
        G.pEm->Routine_3 += Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        G.pEm->Cdir_z = G.pEm->Cdir_z - 2;
        break;
    case 8:

        EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 8);
    }
    FUNC(KAGE_HALF_SET)(&MAIN.Player.Kage_work, G.Player.Pos_y / 17 + 0x1F4, G.Player.Pos_y / 17 + 0x2BC);  // address passed as an integer
}

VOID Em08_Act_kill(VOID)
{
    PARTS_W* pP_ptr;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1;
        G.pEm->Timer0 = 0x1E;
        FUNC(SND_SE_STAD)(2, 6, 0, G.pEm->pSin_parts_ptr[14].Workm.t);

    case 1:
        pP_ptr = G.pEm->pSin_parts_ptr;
        FUNC(GS_MUL_COORD0)(&pP_ptr[EM08_FREE_P->Catch_parts].Workm, &Em08_catch_mat, &MAIN.Player.M);  // address passed as an integer
        pP_ptr[12].Be_flg = pP_ptr[12].Be_flg & 0xFD;
        pP_ptr[12].Cdir.vx = pP_ptr[12].Cdir.vx + 2;
        FUNC(ROT_MATRIX)(&pP_ptr[12].Cdir, &pP_ptr[12].Coord);
        FUNC(GS_MUL_COORD0)(&pP_ptr[11].Workm, &pP_ptr[12].Coord, &pP_ptr[12].Workm);
        G.pEm->Cdir_z -= 4;
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 2;
            G.pEm->Timer0 = 0x5A;
            PL_ROUTINE_WORD = ROUTINES(4, 1, 8, 6);
            EM08_FREE_P->Rot_spd = 0x18;
        }
        return;
    case 2:
        ScratchWord[0] = Api_rand() & 3;
        G.pEm->Cdir_y = G.pEm->Cdir_y + EM08_FREE_P->Rot_spd / (ScratchWord[0] + 1);
        G.pEm->Cdir_z -= 1;
        pP_ptr = G.pEm->pSin_parts_ptr;
        FUNC(GS_MUL_COORD0)(&pP_ptr[EM08_FREE_P->Catch_parts].Workm, &Em08_catch_mat, &MAIN.Player.M);  // address passed as an integer
        FUNC(GS_MUL_COORD0)(&pP_ptr[11].Workm, &pP_ptr[12].Coord, &pP_ptr[12].Workm);
        EM08_FREE_P->Rot_spd *= -1;
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 3;
            G.pEm->Timer0 = 0x50;
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[9], 0x30, 0x80820, 0x606060);
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[9], 0x30, 0x80820, 0x606060);
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[9], 0x30, 0x80820, 0x606060);
            pP_ptr = G.Player.pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

            FUNC(ESP_CALL)(0, 3, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(0, 3, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);

            pP_ptr[2].Be_flg = pP_ptr[2].Be_flg | 8;
            pP_ptr->Be_flg = pP_ptr->Be_flg | 0x10;
            PL_ROUTINE_WORD = ROUTINES(0, 0, 8, 7);
        }
        return;
    case 3:
        pP_ptr = G.pEm->pSin_parts_ptr;
        FUNC(GS_MUL_COORD0)(&pP_ptr[EM08_FREE_P->Catch_parts].Workm, &Em08_catch_mat, &MAIN.Player.M);  // address passed as an integer
        pP_ptr[12].Cdir.vx = pP_ptr[12].Cdir.vx + 0x10;
        FUNC(ROT_MATRIX)(&pP_ptr[12].Cdir, &pP_ptr[12].Coord);
        FUNC(GS_MUL_COORD0)(&pP_ptr[11].Workm, &pP_ptr[12].Coord, &pP_ptr[12].Workm);
        G.Player.Life = -1;
        return;
    case 4:
        EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
        pP_ptr = G.pEm->pSin_parts_ptr;
        pP_ptr[12].Be_flg = pP_ptr[12].Be_flg | 2;
        PL_ROUTINE_WORD = ROUTINES(2, 0, 8, 7);

    default: LABEL_END
    }
}

VOID Em08_Act_wither(VOID)
{
    INT unused;    // unused local: reserves stack space
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;
        G.pEm->Move_no = 0xD;
        G.pEm->Scale = 0x1000U;
        if (!(G.pEm->Type & 1))
        {
            G.pEm->pTgt->Old_dir = 7;
            EM_TGT_ROUTINE_WORD = 1;
            VIEW(INT, EM08_FREE_P->roots->Routine_0) = 1;  // INT* view of the field (a plain access changes the code)
            FUNC(SND_SE_ENEM)(7, 0);

            FUNC(PAD_VIB_SET0)(3, 0);
            FUNC(PAD_VIB_SET1)(0x1E, 0x6E, 0);
            FUNC(PAD_VIB_FADE_SET)(0x50, 0x6E, 0, 0x1E);
        }

    case 1:

        G.pEm->Scale = G.pEm->Scale + 0xFFE0;

        G.pEm->Routine_3 += Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        return;
    case 2:
        G.pEm->Timer0 = 0xD2U;
        G.pEm->Routine_3 = 3U;
        if (!(G.pEm->Type & 1))
        {
            G.pEm->pTgt->Old_dir = 4;
            G.pEm->pTgt->Routine_3 = 2;
            EM08_FREE_P->roots->Routine_3 = 2;
        }

    case 3:
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 4U;
        }
        return;
    case 4:
        G.pEm->Routine_3 = 5U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;
        G.pEm->Move_no = 2;
        if (!(G.pEm->Type & 1))
        {
            G.pEm->pTgt->Old_dir = 0xE;
            G.pEm->pTgt->Routine_3 = 3;
            EM08_FREE_P->roots->Routine_3 = 3;

            FUNC(PAD_VIB_SET1)(0x3C, 0x46, 0);
        }

    case 5:
        G.pEm->Scale = G.pEm->Scale + 0x20;
        G.pEm->Routine_3 += Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        return;
    case 6:
        EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
        G.pEm->Scale = 0U;
        G.pEm->Damage_no = 0;
        if (!(G.pEm->Type & 1))
        {
            G.pEm->pTgt->Old_dir = 0;
            EM_TGT_ROUTINE_WORD = 0;
            VIEW(INT, EM08_FREE_P->roots->Routine_0) = 0;  // INT* view of the field (a plain access changes the code)
        }
        if (G.pEm->Type & 0x40)
        {
            G.pEm->Routine_0 = 8;
        }
    }
}

VOID Em08_Act_recoil(VOID)
{
    ENEMY_WORK* pSub;
    INT pad14;    // unused local: reserves stack space
    UCHAR Anm_tbl[8] = { 2, 3, 4, 5, 10, 11, 12, 13 };
    INT Rsv20;    // unused local: reserves stack space
    USHORT Swing;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3FU;
        G.pEm->Move_no = Anm_tbl[(Api_rand() & 7)];
        EM08_FREE_P->Rot_spd = 0x60 - (Api_rand() & 1) * 0xC0;
        EM08_FREE_P->Catch_parts = 8;
        G.pEm->Timer0 = (Api_rand() & 7) + 1;
        if (!(G.pEm->Type & 1))
        {
            G.pEm->Timer0 = 6U;
        }
        if (Api_rand() & 1)
        {
            G.pEm->Hokan_flg = 7U;
            G.pEm->Routine_3 = 3U;
            G.pEm->Move_no = 0U;
            FUNC(SND_SE_STAD)(2, 1, 0, G.pEm->pSin_parts_ptr[14].Workm.t);
            return;
        }
    case 1:
        Swing = EM08_FREE_P->Rot_spd + 0x60;
        if (Swing >= 0xC1U)
        {
            EM08_FREE_P->Catch_parts *= -1;
        }
        EM08_FREE_P->Rot_spd = EM08_FREE_P->Rot_spd - EM08_FREE_P->Catch_parts;
        if (EM08_FREE_P->Rot_spd == 0)
        {
            G.pEm->Move_cnt = 0;
            G.pEm->Move_wait = 0;
            G.pEm->Hokan_flg = 0x3FU;
            G.pEm->Move_no = Anm_tbl[(Api_rand() & 7)];
            if (--G.pEm->Timer0 == 0)
            {
                G.pEm->Routine_3 = 2U;
            }
        }
        if (G.pEm->Cdir_z < -0x17C)
        {
            G.pEm->Cdir_z = G.pEm->Cdir_z + 0x10;
        }
        G.pEm->Cdir_y = G.pEm->Cdir_y - EM08_FREE_P->Rot_spd;
        Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        return;
    case 2:
        EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
        G.pEm->Damage_no = 0;
        G.pEm->Move_no = 2U;
        if (!(G.pEm->Type & 1))
        {
            pSub = G.pEm->pTgt;
            pSub->Damage_no = 0;
            pSub->Type = 0;
            if (pSub->Old_z >= 2)
            {
                G.pEm->Damage_no = 1;
            }
        }
        if (G.pEm->Type & 0x40)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 8);
        }
        return;
    case 3:
        Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x40);  // address passed as an integer
        if (G.pEm->Cdir_z >= -0x253)
        {
            G.pEm->Cdir_z = G.pEm->Cdir_z - 0x40;
        }
        if (G.pEm->Hokan_flg == 0)
        {
            G.pEm->Routine_3 = 1U;
        }
    }
}

VOID Em08_Act_flinch(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;
        G.pEm->Move_no = 0xD;
        G.pEm->Timer0 = 3;

    case 1:
        if (G.pEm->Damage_no & 7)
        {
            G.pEm->Cdir_z -= 8;
        }
        if ((G.pEm->Type & 0x40) && !(G.pEm->Move_cnt & 7))
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchWord[7] = ((UINT)Api_rand()) % 5;  // unsigned conversion (signed operand)

            FUNC(ESP_CALL)(0xEU, 3, 0, &pP_ptr[ScratchWord[7]].Workm, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(9U, 0, 0, &pP_ptr[ScratchWord[7]].Workm, &ScratchPad->Vec, 0);

            FUNC(PAD_VIB_SET1)(5U, 0x6E, 0);

            EM08_FREE_P->Eff_timer = 0xA;
        }
        G.pEm->Routine_3 += Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        return;
    case 2:
        if (G.pEm->Damage_no & 7)
        {
            G.pEm->Cdir_z += 8;
        }
        if ((G.pEm->Type & 0x40) && !(G.pEm->Move_cnt & 7))
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchWord[7] = ((UINT)Api_rand()) % 5;  // unsigned conversion (signed operand)

            FUNC(ESP_CALL)(0xEU, 3, 0, &pP_ptr[ScratchWord[7]].Workm, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(9U, 0, 0, &pP_ptr[ScratchWord[7]].Workm, &ScratchPad->Vec, 0);

            FUNC(PAD_VIB_SET1)(5U, 0x6E, 0);
        }
        G.pEm->Routine_3 += Em08_Joint_move(1, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        return;
    case 3:
        EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
        G.pEm->Damage_no = 0U;
        if (!(G.pEm->Type & 1))
        {
            G.pEm->pTgt->Damage_no = 0;
            G.pEm->pTgt->Type = 0;
            if (G.pEm->pTgt->Old_z >= 2)
            {
                G.pEm->Damage_no = 1U;
            }
        }
        if (G.pEm->Type & 0x40)
        {
            G.pEm->Routine_0 = 8;
        }
    }
}

VOID Em08_Act_death(VOID)
{
    USHORT D0;
    USHORT D1;
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x3F;
        G.pEm->Move_no = 0xD;
        G.pEm->Scale = 0x1000;
        G.pEm->Timer0 = 0;
        EM08_FREE_P->Bounce_cnt = 5;
        EM08_FREE_P->Wilt_timer = (UINT)Api_rand() % 14 + 0xF;  // unsigned conversion (signed operand)
        EM08_FREE_P->Rot_spd = (Api_rand() & 7) + 8;
        EM08_FREE_P->Rot_spd -= (EM08_FREE_P->Rot_spd + EM08_FREE_P->Rot_spd) * (Api_rand() & 1);
        EM08_FREE_P->Yaw_swing = (Api_rand() & 1) * 0x800;
        EM08_FREE_P->Tilt_swing = (Api_rand() & 1) * 0x800 + 0x400;
        EM08_FREE_P->Yaw_add = 0x20 - (Api_rand() & 1) * 0x40;
        EM08_FREE_P->Tilt_add = 0x40 - (Api_rand() & 1) * 0x80;
        if (!(G.pEm->Type & 1))
        {
            FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg, G.pEm->Em_set_flg);  // address passed as an integer
            FUNC(SND_SE_ENEM)(7, 0U);
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

            FUNC(ESP_CALL)(0, 0x1BU, 0, G.pEm->M, &ScratchPad->Vec, 0);

            FUNC(PAD_VIB_SET0)(1, 0U);
            FUNC(PAD_VIB_SET1)(0x1E, 0x46U, 0);
        }
        FUNC(SND_SE_STAD)(2, 9U, 0, G.pEm->pSin_parts_ptr[9].Workm.t);

    case 1:
        Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        G.pEm->Cdir_z = G.pEm->Cdir_z + 0x10;

        G.pEm->Scale = G.pEm->Scale + 0xFFF0;

        if (!(EM08_FREE_P->Wilt_timer--))
        {
            G.pEm->Routine_3 = 2;
        }
        return;
    case 2:
        G.pEm->Routine_3 = G.pEm->Routine_3 + Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        G.pEm->Routine_3 = G.pEm->Routine_3 + Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);

    case 3:
        G.pEm->Move_no = 2;
        if (G.pEm->Scale >= 0x3E9U)
        {
            G.pEm->Scale = G.pEm->Scale + 0xFFF8;

            pP_ptr = G.pEm->pSin_parts_ptr;
            D0 = pP_ptr[7].Workm.t[1] - G.pEm->Pos_y + 0x32;
            D1 = pP_ptr[14].Workm.t[1] - G.pEm->Pos_y + 0x32;
            if (D0 >= 0x65U && D1 >= 0x65U)
            {
                D0 = (G.pEm->Cdir_x - EM08_FREE_P->Tilt_swing + 0x32) & 0xFFF;
                D1 = (G.pEm->Cdir_y - EM08_FREE_P->Yaw_swing + 0x32) & 0xFFF;
                if (D0 >= 0x65U)
                {
                    G.pEm->Cdir_x = G.pEm->Cdir_x + EM08_FREE_P->Tilt_add;  // sign-extends the byte
                }
                if (D1 >= 0x65U)
                {
                    G.pEm->Cdir_y = G.pEm->Cdir_y + EM08_FREE_P->Yaw_add;  // sign-extends the byte
                }
            }
            G.pEm->Pos_y = G.pEm->Pos_y + G.pEm->Timer0;
            G.pEm->Timer0 = G.pEm->Timer0 + 5;
            if (G.pEm->Pos_y >= -0xC7)
            {
                G.pEm->Timer0 = G.pEm->Timer0 / 3 * -1;
                G.pEm->Pos_y = -0xC8;
                if (!(EM08_FREE_P->Bounce_cnt--))
                {
                    G.pEm->Routine_3 = 4;
                    G.pEm->Damage_no = 0x80;
                }
                if (EM08_FREE_P->Bounce_cnt == 4)
                {
                    FUNC(SND_SE_STAD)(2, 8U, 0, G.pEm->pSin_parts_ptr[10].Workm.t);
                    ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

                    FUNC(ESP_CALL)(0, 0x1BU, 0, G.pEm->M, &ScratchPad->Vec, 0);

                    if (!(G.pEm->Type & 0x40))
                    {
                        ScratchPad->Vec.vy = -0x190;

                        FUNC(ESP_CALL)(9, 0x11U, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0);
                        FUNC(ESP_CALL)(9, 0x11U, 0, &pP_ptr[7].Workm, &ScratchPad->Vec, 0);
                        FUNC(ESP_CALL)(9, 0x11U, 0, &pP_ptr[13].Workm, &ScratchPad->Vec, 0);
                    }
                    ScratchPad->Vec.vy = -0x190;

                    FUNC(ESP_CALL)(9, 0x11U, 0, &pP_ptr[4].Workm, &ScratchPad->Vec, 0);
                    FUNC(ESP_CALL)(9, 0x11U, 0, &pP_ptr[9].Workm, &ScratchPad->Vec, 0);
                    FUNC(ESP_CALL)(9, 0x11U, 0, &pP_ptr[14].Workm, &ScratchPad->Vec, 0);
                }
            }
            Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
            Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
        }
        return;
    case 4:
        if (G.pEm->Scale >= 0x12DU)
        {
            G.pEm->Scale = G.pEm->Scale + 0xFFE0;

            FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, -0x15U, -1);
            pP_ptr = G.pEm->pSin_parts_ptr;
            D0 = pP_ptr[7].Workm.t[1] - G.pEm->Pos_y + 0x32;
            D1 = pP_ptr[14].Workm.t[1] - G.pEm->Pos_y + 0x32;
            if (D0 >= 0x65U && D1 >= 0x65U)
            {
                D0 = (G.pEm->Cdir_x - EM08_FREE_P->Tilt_swing + 0x32) & 0xFFF;
                D1 = (G.pEm->Cdir_y - EM08_FREE_P->Yaw_swing + 0x32) & 0xFFF;
                if (D0 >= 0x65U)
                {
                    G.pEm->Cdir_x = G.pEm->Cdir_x + EM08_FREE_P->Tilt_add;  // sign-extends the byte
                }
                if (D1 >= 0x65U)
                {
                    G.pEm->Cdir_y = G.pEm->Cdir_y + EM08_FREE_P->Yaw_add;  // sign-extends the byte
                }
            }
            Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
            Em08_Joint_move(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
            return;
        }
        if (G.pEm->Type & 1)
        {
            G.pEm->Zone_in_flg = 0;
        }
        G.pEm->Routine_3 = 5;
    }
}

VOID Pl_anim_limb_bounce(VOID)
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
        pP_ptr = G.Player.pSin_parts_ptr;
        pP_ptr->Free0 = 0;
        pP_ptr->Free1 = 0;
        pP_ptr->Free2 = 0;
        pP_ptr[2].Free0 = 0;
        pP_ptr[2].Free1 = 0;
        pP_ptr[2].Free2 = 0;
        pP_ptr[2].Cdir.vx = 0;
        pP_ptr[2].Cdir.vy = 0;
        pP_ptr[2].Cdir.vz = 0;
        FUNC(MODEL_COLOR_PLUS)(pP_ptr, 0x30, 0x80820, 0x606060);
        FUNC(MODEL_COLOR_PLUS)(&pP_ptr[2], 0x30, 0x80820, 0x606060);
        FUNC(MODEL_COLOR_PLUS)(&pP_ptr[1], 0x30, 0x80820, 0x606060);
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

        FUNC(ESP_CALL)(0, 3, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(0, 3, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(0, 3, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);

        FUNC(SND_SE_STAD)(3, 3, 0, &MAIN.Player.Pos_x);  // address passed as an integer
        FUNC(SND_SE_STAD)(4, 0, 0, &MAIN.Player.Pos_x);  // address passed as an integer
        G.Player.Be_flg |= EM_BE_NO_WALL;

        G.Player.Timer0 = 0xF;

        FUNC(PAD_VIB_FADE_SET)(0x1E, 0, 0xFA, 0);
        FUNC(PAD_VIB_SET0)(6, 0);
        FUNC(PAD_VIB_SET0)(8, 0xF);
        FUNC(PAD_VIB_SET0)(6, 0x19);

    case 1:

        if (G.Player.Timer0 != 0)
        {
            G.Player.Timer0--;
            FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
        }

        pP_ptr = G.Player.pSin_parts_ptr;
        if (pP_ptr->Cdir.vz >= -0x3FF)
        {
            pP_ptr->Cdir.vz = pP_ptr->Cdir.vz - pP_ptr->Free0;
            pP_ptr->Free0 = pP_ptr->Free0 + 8;
            FUNC(ROT_MATRIX)(&pP_ptr->Cdir, &pP_ptr->Coord);
        }
        pP_ptr += 2;
        if (pP_ptr->Cdir.vy < 0x100 && !(pP_ptr->Free0 & 0x8000))
        {
            pP_ptr->Cdir.vy = pP_ptr->Cdir.vy + 0x20;
            pP_ptr->Cdir.vx = pP_ptr->Cdir.vx - pP_ptr->Free0;
            pP_ptr->Free0 = pP_ptr->Free0 + 2;
            FUNC(ROT_MATRIX)(&pP_ptr->Cdir, &pP_ptr->Coord);
            FUNC(MUL_MATRIX2)(&pP_ptr->Coord, &pP_ptr->Workm);
            if (pP_ptr->Cdir.vy >= 0x100)
            {
                pP_ptr->Free0 = -0x7FFE;
                ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

                FUNC(ESP_CALL)(0, 3, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);

                FUNC(PAD_VIB_SET0)(0xA, 0);
                FUNC(PAD_VIB_SET1)(0x14, 0xFA, 0);
                FUNC(PAD_VIB_FADE_SET)(0x1E, 0xFA, 0, 0x14);
            }
        }
        return;
    case 2:
        pP_ptr = G.Player.pSin_parts_ptr;
        pP_ptr->Free0 = -0x7FFE;
        G.Player.Routine_3 = 3;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 0;
        G.Player.Hokan_flg = 0xF;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

        FUNC(ESP_CALL)(0, 3, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0);

        ScratchPad->Vec.vy = 0x1F4;

        FUNC(ESP_CALL)(0, 3, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);

    case 3:
        FUNC(JOINT_MOVE)(0, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x100);
        return;
    case 4:
        G.Player.Routine_3 = 5;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 3;

    case 5:
        if (FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400) != 0)
        {
            G.Player.Routine_3 = 6;
            G.Player.Move_wait = 0;
            G.Player.Move_no = 0;
            G.Player.Hokan_flg = 3;
        }
        return;
    case 6:
        FUNC(JOINT_MOVE)(0, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x400);

    default: LABEL_END
    }
}

VOID Em08_Pl_dm(VOID)
{
    Em08_pl_dm_tbl[G.Player.Routine_2]();
}

VOID Em08_Pl_down(VOID)
{
    PARTS_W* pP_ptr;
    INT pad1C;    // unused local: reserves stack space
    VECTOR saved_pos;

    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 6;
        G.Player.Hokan_flg = 4;
        G.Player.Spd_cur = 0x3E8;
        FUNC(SND_SE_STAD)(3, 2, 0, &MAIN.Player.Pos_x);  // address passed as an integer

        FUNC(PAD_VIB_SET1)(0xA, 0xFA, 0);
        FUNC(PAD_VIB_FADE_SET)(0xA, 0xFA, 0, 0xA);

    case 1:
        G.Player.Spd_cur -= G.Player.Move_cnt * 0xF;
        G.Player.Routine_3 += FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
        FUNC(ADD_SPEED_XZ)(0x800);
        saved_pos = PL_POS_VEC;  // address passed as an integer
        ScratchWord[0] = FUNC(SCA_CK_HIT)(&MAIN.Player.Pos_x, G.Player.pAt_tbl->At_w);
        PL_POS_VEC = saved_pos;  // address passed as an integer
        if (ScratchWord[0] != 0)
        {
            G.Player.Routine_3 = 5;
        }
        return;
    case 2:
        G.Player.Routine_3 = 3;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 7;
        G.Player.Hokan_flg = 3;
        FUNC(SND_SE_STAD)(2, 0x1D, 0, &MAIN.Player.Pos_x);  // address passed as an integer

    case 3:
        if (!(G.Player.Move_cnt & 1) && G.Player.Move_cnt < 0xAU)
        {
            pP_ptr = G.Player.pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

            FUNC(ESP_CALL)(9, 0x16, 0, &pP_ptr[5].Workm, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(9, 0x16, 0, &pP_ptr[8].Workm, &ScratchPad->Vec, 0);
        }
        G.Player.Routine_3 += FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
        FUNC(ADD_SPEED_XZ)(0x800);
        G.Player.Spd_cur -= 0xF;
        if (G.Player.Spd_cur < 0)
        {
            G.Player.Spd_cur = 0;
        }
        return;
    case 4:
        PL_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
        G.Player.Damage_no = 0;
        return;
    case 5:
        G.Player.Routine_3 = 6;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 4;
        G.Player.Hokan_flg = 3;
        FUNC(SND_SE_STAD)(2, 0x1A, 0, &MAIN.Player.Pos_x);  // address passed as an integer
        FUNC(SND_SE_STAD)(3, 2, 0, &MAIN.Player.Pos_x);  // address passed as an integer
        pP_ptr = G.Player.pSin_parts_ptr;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchPad->Vec.vx = ScratchPad->Vec.vx - 0x190;

        FUNC(ESP_CALL)(9, 0x16, 0, &pP_ptr[5].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(9, 0x16, 0, &pP_ptr[8].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(9, 0x11, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(9, 0x11, 0, &pP_ptr[3].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(9, 0x11, 0, &pP_ptr[6].Workm, &ScratchPad->Vec, 0);

        FUNC(PAD_VIB_SET1)(5, 0xFA, 0);
        FUNC(PAD_VIB_FADE_SET)(0x1E, 0xFA, 0, 5);

    case 6:
        G.Player.Routine_3 += FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
        return;
    case 7:
        G.Player.Routine_3 = 8;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 5;
        G.Player.Hokan_flg = 3;

    case 8:
        G.Player.Routine_3 += FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
        return;
    case 9:
        G.Player.Routine_3 = 0xA;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 4;
        G.Player.Hokan_flg = 3;

    case 10:
        G.Player.Routine_3 += FUNC(JOINT_MOVE)(1, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
        return;
    case 11:
        PL_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
        G.Player.Damage_no = 0;
        G.Player.Be_flg &= ~EM_BE_NO_HIT;

    default: LABEL_END
    }
}

VOID Em08_Pl_catch(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 0;
        G.Player.Hokan_flg = 3;

        FUNC(PAD_VIB_SET1)(0xA, 0x78, 0);
        FUNC(PAD_VIB_SET1)(0x14, 0x78, 0x14);

    case 1:
        G.Player.Routine_3 += FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
        return;
    case 2:
        G.Player.Routine_3 = 3;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 1;
        G.Player.Hokan_flg = 3;

    case 3:
        FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);

        if (G.Player.Move_cnt == 0)
        {
            FUNC(PAD_VIB_SET1)(0xA, 0x96, 0);
        }

        return;
    case 4:
        G.Player.Routine_3 = 5;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 2;
        G.Player.Hokan_flg = 3;

    case 5:
        FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);

        if (G.Player.Move_cnt == 0)
        {
            FUNC(PAD_VIB_SET1)(0xA, 0xC8, 0);
            FUNC(PAD_VIB_SET0)(0x14, 0);
            FUNC(PAD_VIB_SET1)(0xA, 0x78, 0xA);
        }

        return;
    case 6:
        pP_ptr = G.Player.pSin_parts_ptr;
        pP_ptr[2].Be_flg |= 0xC;
        G.Player.Routine_3 = 7;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

        FUNC(ESP_CALL)(0, 3, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(0, 0, 0, 0, pP_ptr->Workm.t, 0);

        FUNC(MODEL_COLOR_PLUS)(&pP_ptr[2], 0x30, 0x80820, 0x606060);
        FUNC(MODEL_COLOR_PLUS)(&pP_ptr[2], 0x30, 0x80820, 0x606060);

    default: LABEL_END
    }
}

VOID Em08_Pl_throw(VOID)
{
    PARTS_W* pP_ptr;
    INT unused;    // unused local: reserves stack space

    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 3;

        if (G.Player.Life < 0)
        {
            FUNC(SND_SE_STAD)(3, 3, 0, &MAIN.Player.Pos_x);  // address passed as an integer
        }

        FUNC(PAD_VIB_SET0)(0xA, 0);
        FUNC(PAD_VIB_SET1)(0xA, 0xFA, 0);
        FUNC(PAD_VIB_FADE_SET)(0x1E, 0xFA, 0, 0xA);

    case 1:
        if (G.Player.Move_cnt == 1)
        {
            pP_ptr = G.Player.pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

            FUNC(ESP_CALL)(9, 0x11, 0, &pP_ptr[4].Workm, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(9, 0x11, 0, &pP_ptr[7].Workm, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(9, 0x11, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(9, 0x11, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0);
        }
        if (G.Player.Move_cnt == 0xC)
        {
            G.Player.Routine_3 = 2;
            G.Player.Timer0 = 0x5A;
            if (G.Player.Life < 0)
            {
                G.Player.Routine_3 = 8;
            }
        }
        FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
        return;
    case 2:
        G.Player.Timer0 -= FUNC(GACHA_CH)() != 0 ? 4 : 1;
        if (G.Player.Timer0 < 0)
        {
            G.Player.Routine_3 = 3;
        }
        return;
    case 3:
        G.Player.Routine_3 += FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
        return;
    case 4:
        PL_ROUTINE_HALF2 = 0x700;
        return;
    case 5:
        G.Player.Routine_3 = 6;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 3;
        G.Player.Timer0 = 0;
        G.Player.Spd_cur = 0x1F4;
        G.Player.Hokan_flg = 7;

        FUNC(SND_SE_STAD)(3, 1, 0, &MAIN.Player.Pos_x);  // address passed as an integer
        FUNC(SND_SE_STAD)(2, 0x19, 0, &MAIN.Player.Pos_x);  // address passed as an integer

        FUNC(PAD_VIB_SET1)(0xA, 0xFA, 0);
        FUNC(PAD_VIB_FADE_SET)(0x1E, 0xFA, 0, 0xA);

    case 6:
        if (G.Player.Move_cnt != 5 && G.Player.Move_cnt != 7)
        {
        }
        else
        {
            pP_ptr = G.Player.pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

            FUNC(ESP_CALL)(9, 0x11, 0, &pP_ptr[5].Workm, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(9, 0x11, 0, &pP_ptr[8].Workm, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(9, 0x11, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(9, 0x11, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0);
        }
        G.Player.Routine_3 += FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x200);
        if (G.Player.Move_cnt < 0xFU)
        {
            ScratchWord[0] = (SHORT)G.Player.Cdir_y;  // sign-extends the halfword
            G.Player.Cdir_y = Em08_catch_mat.t[0];
            FUNC(ADD_SPEED_XZ)(0);
            G.Player.Cdir_y = ScratchWord[0];
            G.Player.Spd_cur -= G.Player.Timer0 * 5;
            G.Player.Timer0++;
        }
        return;
    case 7:
        PL_ROUTINE_HALF2 = 0x700;
        break;
    case 8:;
    }
}

VOID Em08_Disp_copy(VOID)
{
    MATRIX light;
    SHORT unused;    // unused local: reserves stack space
    SCHAR Rsv2;    // written once, never read

    Rsv2 = 0;
    ScratchPtr[6] = G.pEm;
    G.pEm = G.pEm->pTgt;
    FUNC(SET_LIGHT)(&G.pEm->Pos_x);
    FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        Em08_body_tbl[G.pEm->Routine_0]();
    }
    FUNC(SCALE_MATRIX)(G.pEm->M, &EM08_BODY_OF(G.pEm)->Scale_x);
    FUNC(GS_MUL_COORD0)(Monitor.pWs_mat, G.pEm->M, &ScratchPad->Mat);
    light = *Monitor.pId_mat;
    light.t[0] = G.pEm->Pos_x;
    light.t[1] = G.pEm->Pos_y;
    light.t[2] = G.pEm->Pos_z;
    FUNC(MUL_MATRIX0)(Monitor.pLight_mtx, &light, &light);
    ScratchPtr[14] = G.pEm->pTmd;
    FUNC(SET_LIGHT_MATRIX)(&light);
    FUNC(GS_SET_LS_MATRIX)(&ScratchPad->Mat);
    FUNC(GS_PRST_TG3_L)(ScratchUptr[14][4], ScratchUptr[14][0], ScratchUptr[14][2], (UCHAR*)G.pEm->pPacket + G.Cbuf_id * sizeof(POLY_GT3) + 8, ScratchUptr[14][5], 4, &MAIN.Ot1[G.Cbuf_id]);
    FUNC(MUL_KAGE)(&G.pEm->Pos_x, &G.pEm->Kage_work, 0, 0);
    G.pEm = ScratchPtr[6];
    G.pEm = EM08_FREE_P->roots;
    FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);
    ScratchPad->Vec.vx = ScratchPad->Vec.vy = ScratchPad->Vec.vz = G.pEm->Scale + Em08_pulse_tbl[0x59 - G.pEm->Timer0];
    FUNC(SCALE_MATRIX)(G.pEm->M, &ScratchPad->Vec);
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        Em08_roots_tbl[G.pEm->Routine_0]();
    }
    FUNC(GS_MUL_COORD0)(Monitor.pWs_mat, G.pEm->M, &ScratchPad->Mat);
    ScratchPtr[14] = G.pEm->pTmd;
    FUNC(GS_SET_LS_MATRIX)(&ScratchPad->Mat);
    FUNC(GS_PRST_TG3_L)(ScratchUptr[14][4], ScratchUptr[14][0], ScratchUptr[14][2], (UCHAR*)G.pEm->pPacket + G.Cbuf_id * sizeof(POLY_GT3) + 8, ScratchUptr[14][5], 4, &MAIN.Ot1[G.Cbuf_id]);
    G.pEm = ScratchPtr[6];
}

VOID Em08_Body_idle(VOID)
{
    UCHAR Swing;

    EM08_BODY_OF(G.pEm)->Scale_x = Em08_pulse_tbl[0x59 - G.pEm->Timer0] * 3 + 0x1000;
    EM08_BODY_OF(G.pEm)->Scale_y = Em08_pulse_tbl[0x59 - G.pEm->Timer0] * 3 + 0x1000;
    EM08_BODY_OF(G.pEm)->Scale_z = Em08_pulse_tbl[0x59 - G.pEm->Timer0] * 3 + 0x1000;
    if (G.pEm->Timer0 == 0x41)
    {
        FUNC(SND_SE_ENEM)(0, 0);
    }
    if (G.pEm->Timer0-- == 0)
    {
        G.pEm->Timer0 = 0x59;
    }
    G.pEm->Cdir_y = G.pEm->Cdir_y + G.pEm->Spd_y;
    G.pEm->Cdir_z = G.pEm->Cdir_z + G.pEm->Old_x;
    G.pEm->Spd_y = G.pEm->Spd_y - G.pEm->Spd_z;
    G.pEm->Old_x = G.pEm->Old_x - G.pEm->Old_y;
    Swing = G.pEm->Spd_y + 8;
    if (Swing >= 0x11U)
    {
        G.pEm->Spd_z *= -1;
    }
    Swing = G.pEm->Old_x + 7;
    if (Swing >= 0xFU)
    {
        G.pEm->Old_y = G.pEm->Old_y * -1;
    }
}

VOID Em08_Body_shrivel(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Spd_x = 0x80;
        FUNC(CLUT_FADE_SABUN_ADD)(-1, -1, 0, 0, 0x200, 8);
        G.pEm->Hokan_flg = 0xFU;
        G.pEm->Routine_3 = 1U;

    case 1:
        EM08_BODY_OF(G.pEm)->Scale_x += -0x18 - G.pEm->Spd_x;
        EM08_BODY_OF(G.pEm)->Scale_y += -0x18 - G.pEm->Spd_x;
        EM08_BODY_OF(G.pEm)->Scale_z += -0x18 - G.pEm->Spd_x;
        G.pEm->Spd_x *= -1;
        if (!G.pEm->Hokan_flg--)
        {
            FUNC(CLUT_FADE_SABUN_ADD)(0, -1, 0, 0, 0x200, 8);
        }
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, -0x10, -0x10);
        if (!(G.pEm->Type & 0x40) && !((G.pEm->Hokan_flg % 30) & 0xFF))
        {
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0xBB8;

            FUNC(ESP_CALL)(0, 0x1B, 0, G.pEm->M, &ScratchPad->Vec, 0);
        }
        break;
    case 2:
        break;
    case 3:
        G.pEm->Spd_x = -0x80;
        G.pEm->Routine_3 = 4U;

    case 4:
        EM08_BODY_OF(G.pEm)->Scale_x += 0x18 + G.pEm->Spd_x;
        EM08_BODY_OF(G.pEm)->Scale_y += 0x18 + G.pEm->Spd_x;
        EM08_BODY_OF(G.pEm)->Scale_z += 0x18 + G.pEm->Spd_x;
        G.pEm->Spd_x *= -1;
        G.pEm->Hokan_flg--;
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, 0x10, 0x10);
        if (!(G.pEm->Type & 0x40) && !((G.pEm->Hokan_flg % 30) & 0xFF))
        {
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0xBB8;

            FUNC(ESP_CALL)(0, 0x1B, 0, G.pEm->M, &ScratchPad->Vec, 0);
        }
    }
    if (!(Api_rand() & 0x1F))
    {
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchPad->Vec.vy = 0x7D0;
        ScratchPad->Vec.vx = Api_rand() & 0x1FF;
        ScratchPad->Vec.vz = Api_rand() & 0x1FF;

        FUNC(ESP_CALL)(0, 0x1C, 0, G.pEm->M, &ScratchPad->Vec, 0);
    }
}

VOID Em08_Body_fall(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Spd_x = 0;
        G.pEm->Routine_3 = 1U;
        G.pEm->Hokan_flg = 0x78U;

    case 1:
        G.pEm->Pos_y = G.pEm->Pos_y + G.pEm->Spd_x * 5;
        ++G.pEm->Spd_x;
        if (G.pEm->Spd_x < 6 && (G.pEm->Spd_x & 1) != 0)
        {
            FUNC(CLUT_FADE_SABUN_ADD)(-1, -1, 0, 0, 0x200, 8);
        }
        if (G.pEm->Pos_y >= 0xC8)
        {
            FUNC(PAD_VIB_SET1)(0xA, 0x96, 0);
            FUNC(PAD_VIB_FADE_SET)(0x1E, 0x96, 0, 0xA);

            --G.pEm->Spd_x;
            G.pEm->Pos_y = G.pEm->Pos_y - G.pEm->Spd_x * 5;
            G.pEm->Routine_3 = 2U;
            G.pEm->Hokan_flg--;
            EM08_BODY_OF(G.pEm)->Scale_y = EM08_BODY_OF(G.pEm)->Scale_y - 0x914;
            EM08_BODY_OF(G.pEm)->Scale_x = EM08_BODY_OF(G.pEm)->Scale_x + 0x9DC;
            EM08_BODY_OF(G.pEm)->Scale_z = EM08_BODY_OF(G.pEm)->Scale_z + 0x9DC;
            if (!(G.pEm->Type & 0x40))
            {
                ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
                ScratchPad->Vec.vx = 0x258;
                ScratchPad->Vec.vz = 0x258;
                ScratchPad->Vec.vy = -0xC8;

                FUNC(ESP_CALL)(9, 0x11, 0, G.pEm->M, &ScratchPad->Vec, 0x28);

                ScratchPad->Vec.vx = -0x258;
                ScratchPad->Vec.vz = 0x258;

                FUNC(ESP_CALL)(9, 0x11, 0x400, G.pEm->M, &ScratchPad->Vec, 0x28);

                ScratchPad->Vec.vx = -0x258;
                ScratchPad->Vec.vz = -0x258;
                FUNC(ESP_CALL)(9, 0x11, 0x200, G.pEm->M, &ScratchPad->Vec, 0x28);
                ScratchPad->Vec.vx = 0x258;
                ScratchPad->Vec.vz = -0x258;

                FUNC(ESP_CALL)(9, 0x11, 0x800, G.pEm->M, &ScratchPad->Vec, 0x28);

                ScratchPad->Vec.vx = 0;
                ScratchPad->Vec.vy = -0x258;
                ScratchPad->Vec.vz = 0;

                FUNC(ESP_CALL)(0, 0x1B, 0, G.pEm->M, &ScratchPad->Vec, 0x3C);
            }
            FUNC(SND_SE_STAD)(2, 8, 0, &G.pEm->Pos_x);
            FUNC(SND_SE_STAD)(2, 0, 0, &G.pEm->Pos_x);
            FUNC(SND_SE_STAD)(2, 0, 0, &G.pEm->Pos_x);
            FUNC(SND_SE_STAD)(2, 0, 0, &G.pEm->Pos_x);
            G.pEm->Spd_x = 0x3C;
        }
        return;
    case 2:
        if (G.pEm->Hokan_flg != 0)
        {
            EM08_BODY_OF(G.pEm)->Scale_y = KEEP_VALUE(EM08_BODY_OF(G.pEm)->Scale_y) + (G.pEm->Spd_x - (Api_rand() & 1) * KEEP_VALUE(G.pEm->Spd_x * 2));  // KEEP_VALUE keeps the operand a separate value; the plain form and a cached local change the code
            EM08_BODY_OF(G.pEm)->Scale_x = KEEP_VALUE(EM08_BODY_OF(G.pEm)->Scale_x) + (G.pEm->Spd_x - (Api_rand() & 1) * KEEP_VALUE(G.pEm->Spd_x * 2));  // KEEP_VALUE keeps the operand a separate value; the plain form and a cached local change the code
            EM08_BODY_OF(G.pEm)->Scale_z = KEEP_VALUE(EM08_BODY_OF(G.pEm)->Scale_z) + (G.pEm->Spd_x - (Api_rand() & 1) * KEEP_VALUE(G.pEm->Spd_x * 2));  // KEEP_VALUE keeps the operand a separate value; the plain form and a cached local change the code
            G.pEm->Spd_x *= -1;
            if (G.pEm->Spd_x < 0)
            {
                ++G.pEm->Spd_x;
            }
            G.pEm->Hokan_flg--;
        }
    }
}

VOID Em08_Roots_idle(VOID)
{
    if (G.pEm->Timer0-- == 0)
    {
        G.pEm->Timer0 = 0x59U;
    }
}

VOID Em08_Roots_pulse(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Spd_x = 0x10;
        G.pEm->Routine_3 = 1;
    case 1:
        G.pEm->Scale -= G.pEm->Spd_x + 0x30;
        G.pEm->Spd_x *= -1;
        break;
    case 2:
        break;
    case 3:
        G.pEm->Routine_3 = 4;
        G.pEm->Spd_x = -0x10;
    case 4:
        G.pEm->Scale += G.pEm->Spd_x + 0x30;
        G.pEm->Spd_x *= -1;
    }
}

VOID Em08_Roots_rise(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Spd_x = 0;
        G.pEm->Routine_3 = 1U;

    case 1:
        G.pEm->Pos_y = G.pEm->Pos_y + G.pEm->Spd_x * 4;
        G.pEm->Spd_x++;
        if (G.pEm->Pos_y >= -0x512)
        {
            G.pEm->Spd_x--;
            G.pEm->Pos_y = G.pEm->Pos_y - G.pEm->Spd_x * 4;
            ScratchPad->Vec.vy = ScratchPad->Vec.vy / 2;
        }
        if (G.pEm->Scale >= 0x3E8U)
        {
            G.pEm->Scale = G.pEm->Scale + 0xFFC8;
        }
    }
}

UCHAR Em08_Joint_move(UCHAR reverse, VOID* pKan_t_ptr, VOID* pSeq_t_ptr, SHORT Hokan_step)
{
    SHORT* pFrm_data;
    INT* pAnim;
    ANM_FRAME_REC* pFrm;
    ANM_REC* pEntry;
    PARTS_W* pP_ptr;
    UCHAR unused;    // unused local: reserves stack space
    UCHAR blend;
    USHORT diff;
    VECTOR rot;

    if (G.pEm->Move_wait != 0)
    {
        --G.pEm->Move_wait;
    }
    pAnim = pKan_t_ptr;
    pFrm_data = (SHORT*)(pAnim + ((SHORT*)pAnim)[1] / 4);  // halfword view of the same memory
    ScratchWord[0] = (*(SHORT*)&((ANM_REC*)pAnim)[1].Frm_ofs) / 2;  // SHORT view of Frm_ofs: unsigned load changes the divide
    pAnim = pSeq_t_ptr;
    pEntry = (ANM_REC*)(pAnim + G.pEm->Move_no);  // reinterprets the memory as ANM_REC
    pFrm = (ANM_FRAME_REC*)(pAnim + (pEntry->Frm_ofs >> 2));  // reinterprets the memory as ANM_FRAME_REC
    pFrm = BYTE_OFS(ANM_FRAME_REC, pFrm, G.pEm->Move_cnt * 4);
    if (reverse != 0)
    {
        pFrm = (ANM_FRAME_REC*)((UCHAR*)pFrm + ((pEntry->Frm_num - G.pEm->Move_cnt * 2) * 4 - 4));  // reinterprets the memory as ANM_FRAME_REC
    }
    pFrm_data += *(USHORT*)pFrm * ScratchWord[0];  // halfword view of the same memory
    pP_ptr = G.pEm->pSin_parts_ptr;
    blend = G.pEm->Hokan_flg;
    if (blend != 0)
    {
        --G.pEm->Hokan_flg;
    }
    pP_ptr->Coord.t[0] = *pFrm_data;
    if (blend == 0 && G.pEm->Move_wait != 0)
    {
        blend = 0x1000 / Hokan_step / (G.pEm->Move_wait + 1);
    }
    ScratchWord[4] = G.pEm->Parts_num;
    if (blend != 0)
    {
        pP_ptr->Coord.t[1] = pP_ptr->Coord.t[1] * Hokan_step * blend / 4096 + ((*++pFrm_data * Hokan_step * (0x1000 / Hokan_step - blend)) / 4096);
        pP_ptr->Coord.t[2] = *++pFrm_data;
        pFrm_data++;
        pFrm_data += 3;
        while ((ScratchWord[4] = ScratchWord[4] - 1) != -1)
        {
            ScratchPad->Vec.vx = *pFrm_data;
            ScratchPad->Vec.vy = *++pFrm_data;
            ScratchPad->Vec.vz = *++pFrm_data;
            ScratchPad->Sv = pP_ptr->Cdir;
            if (blend == 0x1000 / Hokan_step - 1)
            {
                ScratchWord[7] = 2;
                do
                {
                    diff = ScratchVec[ScratchWord[7]] - ScratchSvU[ScratchWord[7]] + 0x800;
                    if (diff > 0x1000)
                    {
                        ScratchSv[ScratchWord[7]] += !(diff & 0x8000) ? 0x1000 : -0x1000;
                    }
                } while ((ScratchWord[7] = ScratchWord[7] - 1) != -1);
            }
            ScratchPad->Vec.vx = ScratchPad->Vec.vx << 4;
            ScratchPad->Vec.vy = ScratchPad->Vec.vy << 4;
            ScratchPad->Vec.vz = ScratchPad->Vec.vz << 4;
            rot.vx = ScratchPad->Sv.vx << 4;
            rot.vy = ScratchPad->Sv.vy << 4;
            rot.vz = ScratchPad->Sv.vz << 4;
            FUNC(LOAD_AVERAGE12)(&rot, &ScratchPad->Vec, Hokan_step * blend, Hokan_step * (0x1000 / Hokan_step - blend), &rot);
            ScratchPad->Sv.vx = rot.vx >> 4;
            ScratchPad->Sv.vy = rot.vy >> 4;
            ScratchPad->Sv.vz = rot.vz >> 4;
            pP_ptr->Cdir = ScratchPad->Sv;
            pFrm_data++;
            FUNC(ROT_MATRIX)(&ScratchPad->Sv, &pP_ptr->Coord);
            pP_ptr++;
        }
    }
    else
    {
        pP_ptr->Coord.t[1] = *++pFrm_data;
        pP_ptr->Coord.t[2] = *++pFrm_data;
        pFrm_data++;
        pFrm_data += 3;
        while ((ScratchWord[4] = ScratchWord[4] - 1) != -1)
        {
            pP_ptr->Cdir.vx = *pFrm_data;
            pP_ptr->Cdir.vy = *++pFrm_data;
            pP_ptr->Cdir.vz = *++pFrm_data;
            pFrm_data++;
            FUNC(ROT_MATRIX)(&pP_ptr->Cdir, &pP_ptr->Coord);
            pP_ptr++;
        }
    }
    if (G.pEm->Move_wait == 0)
    {
        G.pEm->Move_wait = pFrm->Move_wait;
        if (++G.pEm->Move_cnt > pEntry->Frm_num - 1)
        {
            G.pEm->Move_cnt = 0;
            return 1;
        }
    }
    return 0;
}

VOID Em08_Scenario(VOID)
{
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    Em08_sce_act_tbl[G.pEm->Routine_2]();
    if (EM08_FREE_P->Eff_timer != 0)
    {
        if ((EM08_FREE_P->Eff_timer--) == 1)
        {
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchWord[7] = ((UINT)Api_rand()) % 5;  // unsigned conversion (signed operand)
            ScratchPad->Vec.vy = (Api_rand() & 0xFFF) + 0x200;
            if (G.pEm->pTgt->Pos_y >= -0x7CF)
            {
                ScratchPad->Vec.vy = 0;
            }
            ScratchPad->Vec.vx = 0x200 - (Api_rand() & 0x3FF);
            ScratchPad->Vec.vz = 0x200 - (Api_rand() & 0x3FF);

            FUNC(ESP_CALL)(0xEU, 3, 0, G.pEm->pTgt->M, &ScratchPad->Vec, 0x28);
            FUNC(ESP_CALL)(9U, 0, 0, G.pEm->pTgt->M, &ScratchPad->Vec, 0x28);

            EM08_FREE_P->Eff_timer = 0xD;
            FUNC(SND_SE_STAD)(2U, 0x1E, 0, &G.pEm->pTgt->Pos_x);

            if (G.pEm->pTgt->Pos_y < -0x1388)
            {
                FUNC(PAD_VIB_SET1)(((UINT)Api_rand()) % 5 + 3, ((UINT)Api_rand()) % 20 + 0x50, 0);  // unsigned conversion (signed operand)
            }

            if (G.pEm->pTgt->Pos_y >= -0x12B)
            {
                EM08_FREE_P->Eff_timer = 0;
            }
        }
    }
    if (!(G.pEm->Type & 0x40))
    {
        EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
    }
}

VOID Em08_Sce_face(VOID)
{
    ScratchWord[0] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x20U);
    G.pEm->Cdir_y = G.pEm->Cdir_y + ScratchWord[0];
    if (ScratchWord[0] == 0)
    {
        FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);  // address passed as an integer
    }
    Em08_Act_idle();
}

VOID Em08_Sce_recoil(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        EM08_FREE_P->Eff_timer = 0xA;
    }
    Em08_Act_recoil();
}

VOID Em08_Sce_death(VOID)
{
    if (G.pEm->Routine_1 == 0)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 0xE, 1, 8);
        ScratchWord[1] = --G.pEm->pTgt->Old_z;
        if (ScratchWord[1] == 1)
        {
            --G.pEm->pTgt->Old_z;
        }
        if (!(G.pEm->Type & 1))
        {
            G.pEm->pTgt->Life = -1;
            EM_TGT_ROUTINE_WORD = 2;
            VIEW(INT, EM08_FREE_P->roots->Routine_0) = 2;  // INT* view of the field (a plain access changes the code)
        }
    }
    Em08_Act_death();
}

// collision table of the vine: two AT_TBL records
AT_TBL Em08_sca_vine[2] =
{
    { 0x8000, 0, 0, 0, 0xFA, 0xC8 },   // 0 //
    { 0x8000, 0, 0, 0, 0xFA, 0xC8 },   // 1 //
};

// collision table of the main plant: two AT_TBL records
AT_TBL Em08_sca_main[2] =
{
    { 0, 0, 0, 0, 0xFA, 0xC8 },   // 0 //
    { 0x8000, 0, 0, 0, 0x1B58, 0x7D0 },   // 1 //
};

AT_TBL* pEm08_sca_vine = Em08_sca_vine;     // collision table of the vine

AT_TBL* pEm08_sca_main = Em08_sca_main;     // collision table of the main plant

MATRIX Em08_catch_mat = { 0 };      // transform of the held player relative to its grab joint; t[0] doubles as the player heading

// state handlers, by entity state
VOID (*Em08_routine_0[9])() =
{
    Em08_Init,          // 0 //
    Em08_Move,   // 1 //
    Em08_Damage,       // 2 //
    Em08_Die,           // 3 //
    Em08_Dead,        // 4 //
    0,                     // 5 //
    0,                     // 6 //
    0,                     // 7 //
    Em08_Scenario,     // 8 //
};

SHORT Em08_esp_ofs_x[2] = { 0x0D54, 0x45EC };     // x offsets of the ambient effects

// z offsets of the ambient effects (the first two are used)
SHORT Em08_esp_ofs_z[6] =
{
    0x0D54,   // 0 //
    0x46FA,   // 1 //
    0x0200,   // 2 //
    0x0200,   // 3 //
    0x0000,   // 4 //
    0x0000,   // 5 //
};

// action handlers, by Routine_2
VOID (*Em08_act_tbl[10])() =
{
    Em08_Act_idle,       // 0 //
    Em08_Act_sweep,      // 1 //
    Em08_Act_spit,       // 2 //
    Em08_Act_move,       // 3 //
    Em08_Act_hold,       // 4 //
    Em08_Act_grab,   // 5 //
    Em08_Act_suspend,    // 6 //
    Em08_Act_release,    // 7 //
    Em08_Act_kill,       // 8 //
    Em08_Act_wither,     // 9 //
};

// action selectors, by Type & 7
VOID (*Em08_sel_tbl[7])() =
{
    Em08_Sel_def,          // 0 //
    Em08_Sel_def,          // 1 //
    Em08_Sel_def,          // 2 //
    Em08_Sel_def,          // 3 //
    Em08_Sel_nop,      // 4 //
    Em08_Sel_type5,   // 5 //
    0,                              // 6 //
};

// animation ids picked at random for idle (the first ten are used)
UCHAR Em08_idle_mot_tbl[12] =
{
    0x02,   // 0 //
    0x03,   // 1 //
    0x04,   // 2 //
    0x05,   // 3 //
    0x06,   // 4 //
    0x09,   // 5 //
    0x0A,   // 6 //
    0x0B,   // 7 //
    0x0C,   // 8 //
    0x0D,   // 9 //
    0x0F,   // 10 //
    0x00,   // 11 //
};

// capture matrix preset used by the suspend action when the player id bit 0 is clear
MATRIX Em08_hold_mat =
{
    { { 0x0090, -0x0F93, -0x036B }, { -0x0FBC, -0x00F3, 0x01E7 }, { -0x0218, 0x034B, -0x0F5C } },
    { -0x060F, 0x0000, 0x02BC },
};

// player reaction handlers, by Routine_2
VOID (*Em08_pl_dm_tbl[3])() =
{
    Em08_Pl_down,   // 0 //
    Em08_Pl_catch,             // 1 //
    Em08_Pl_throw,              // 2 //
};

// scale pulse curve, indexed by 0x59 - Timer0 (the first 90 entries are used)
UCHAR Em08_pulse_tbl[180] =
{
    0x00,   // 0 //
    0x01,   // 1 //
    0x03,   // 2 //
    0x06,   // 3 //
    0x0A,   // 4 //
    0x0F,   // 5 //
    0x15,   // 6 //
    0x1C,   // 7 //
    0x24,   // 8 //
    0x2D,   // 9 //
    0x37,   // 10 //
    0x42,   // 11 //
    0x4E,   // 12 //
    0x5B,   // 13 //
    0x69,   // 14 //
    0x78,   // 15 //
    0x88,   // 16 //
    0x99,   // 17 //
    0xAC,   // 18 //
    0xBE,   // 19 //
    0xCD,   // 20 //
    0xD9,   // 21 //
    0xE2,   // 22 //
    0xE8,   // 23 //
    0xED,   // 24 //
    0xF1,   // 25 //
    0xF9,   // 26 //
    0xFC,   // 27 //
    0xFE,   // 28 //
    0xFF,   // 29 //
    0xFE,   // 30 //
    0xFD,   // 31 //
    0xFC,   // 32 //
    0xFB,   // 33 //
    0xF9,   // 34 //
    0xF7,   // 35 //
    0xF5,   // 36 //
    0xF3,   // 37 //
    0xF0,   // 38 //
    0xED,   // 39 //
    0xEA,   // 40 //
    0xE7,   // 41 //
    0xE3,   // 42 //
    0xDF,   // 43 //
    0xDB,   // 44 //
    0xD7,   // 45 //
    0xD2,   // 46 //
    0xCD,   // 47 //
    0xC8,   // 48 //
    0xC3,   // 49 //
    0xBD,   // 50 //
    0xB7,   // 51 //
    0xB1,   // 52 //
    0xAB,   // 53 //
    0xA4,   // 54 //
    0x9D,   // 55 //
    0x96,   // 56 //
    0x8F,   // 57 //
    0x87,   // 58 //
    0x7F,   // 59 //
    0x77,   // 60 //
    0x6F,   // 61 //
    0x71,   // 62 //
    0x6A,   // 63 //
    0x63,   // 64 //
    0x5C,   // 65 //
    0x56,   // 66 //
    0x50,   // 67 //
    0x4A,   // 68 //
    0x44,   // 69 //
    0x3F,   // 70 //
    0x3A,   // 71 //
    0x35,   // 72 //
    0x30,   // 73 //
    0x2C,   // 74 //
    0x28,   // 75 //
    0x24,   // 76 //
    0x20,   // 77 //
    0x1D,   // 78 //
    0x1A,   // 79 //
    0x17,   // 80 //
    0x14,   // 81 //
    0x12,   // 82 //
    0x10,   // 83 //
    0x0E,   // 84 //
    0x0C,   // 85 //
    0x0A,   // 86 //
    0x08,   // 87 //
    0x06,   // 88 //
    0x04,   // 89 //
    0x00,   // 90 //
    0x01,   // 91 //
    0x03,   // 92 //
    0x06,   // 93 //
    0x0A,   // 94 //
    0x0F,   // 95 //
    0x15,   // 96 //
    0x1C,   // 97 //
    0x24,   // 98 //
    0x2D,   // 99 //
    0x37,   // 100 //
    0x42,   // 101 //
    0x4E,   // 102 //
    0x5B,   // 103 //
    0x69,   // 104 //
    0x78,   // 105 //
    0x88,   // 106 //
    0x99,   // 107 //
    0xAC,   // 108 //
    0xBE,   // 109 //
    0x99,   // 110 //
    0x78,   // 111 //
    0x5B,   // 112 //
    0x42,   // 113 //
    0x2D,   // 114 //
    0x1C,   // 115 //
    0x0F,   // 116 //
    0x06,   // 117 //
    0x01,   // 118 //
    0x00,   // 119 //
    0x00,   // 120 //
    0x01,   // 121 //
    0x03,   // 122 //
    0x06,   // 123 //
    0x0A,   // 124 //
    0x0F,   // 125 //
    0x15,   // 126 //
    0x1C,   // 127 //
    0x24,   // 128 //
    0x2D,   // 129 //
    0x37,   // 130 //
    0x42,   // 131 //
    0x4E,   // 132 //
    0x5B,   // 133 //
    0x69,   // 134 //
    0x78,   // 135 //
    0x88,   // 136 //
    0x99,   // 137 //
    0xAC,   // 138 //
    0xBE,   // 139 //
    0x99,   // 140 //
    0x78,   // 141 //
    0x5B,   // 142 //
    0x42,   // 143 //
    0x2D,   // 144 //
    0x1C,   // 145 //
    0x0F,   // 146 //
    0x06,   // 147 //
    0x01,   // 148 //
    0x00,   // 149 //
    0x00,   // 150 //
    0x01,   // 151 //
    0x03,   // 152 //
    0x06,   // 153 //
    0x0A,   // 154 //
    0x0F,   // 155 //
    0x15,   // 156 //
    0x1C,   // 157 //
    0x24,   // 158 //
    0x2D,   // 159 //
    0x37,   // 160 //
    0x42,   // 161 //
    0x4E,   // 162 //
    0x5B,   // 163 //
    0x69,   // 164 //
    0x78,   // 165 //
    0x88,   // 166 //
    0x99,   // 167 //
    0xAC,   // 168 //
    0xBE,   // 169 //
    0x99,   // 170 //
    0x78,   // 171 //
    0x5B,   // 172 //
    0x42,   // 173 //
    0x2D,   // 174 //
    0x1C,   // 175 //
    0x0F,   // 176 //
    0x06,   // 177 //
    0x01,   // 178 //
    0x00,   // 179 //
};

// body state handlers, by body state
VOID (*Em08_body_tbl[3])() =
{
    Em08_Body_idle,      // 0 //
    Em08_Body_shrivel,   // 1 //
    Em08_Body_fall,      // 2 //
};

// roots state handlers, by roots state
VOID (*Em08_roots_tbl[3])() =
{
    Em08_Roots_idle,    // 0 //
    Em08_Roots_pulse,   // 1 //
    Em08_Roots_rise,    // 2 //
};

// script-driven action handlers, by Routine_2
VOID (*Em08_sce_act_tbl[20])() =
{
    Em08_Act_idle,         // 0 //
    0,                             // 1 //
    Em08_Sce_face,       // 2 //
    0,                             // 3 //
    0,                             // 4 //
    0,                             // 5 //
    0,                             // 6 //
    0,                             // 7 //
    0,                             // 8 //
    0,                             // 9 //
    Em08_Act_grab,     // 10 //
    Em08_Act_suspend,      // 11 //
    Em08_Act_release,      // 12 //
    Em08_Sce_recoil,            // 13 //
    Em08_Sce_death,             // 14 //
    Em08_Act_wither,       // 15 //
    Em08_Act_flinch,   // 16 //
    0,                             // 17 //
    0,                             // 18 //
    0,                             // 19 //
};
