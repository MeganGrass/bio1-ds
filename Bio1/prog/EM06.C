/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM06.C
 */

#include "TYPES.H"

#include "EM06.H"
#include "FLAG.H"
#include "MONITOR.H"

#define EM_ROUTINE_23 EM_ROUTINE_HALF2 // cast kept: Routine_2 and Routine_3 are written together as one halfword

VOID Em06_Init(VOID)
{
    if (G.Status_flg & STAT_ARRANGE)
    {
        Em06_pl_life_lim[0] = 0x4B;
        Em06_pl_life_lim[1] = 0x34;
    }

    EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);  // cast kept: Routine_0..3 are written as one word
    G.pEm->Coord_flg = 0;
    G.pEm->Dummy0c1 = 0;
    G.pEm->Timer0 = 0;

    G.pEm->Move_no = 0x15;

    G.pEm->Damage_no = 0;

    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0;

    FUNC(INIT_PARTS_WORK)();

    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);

    ScratchWord[7] = 0x808080;
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vx = 0;
    FUNC(KAGE_WORK_INIT)(&ScratchPad->Sv, &G.pEm->Kage_work, 0x3E8, 0x3E8);

    G.pEm->Life = *(Em06_life + (Api_rand() & 0xF));
    G.pEm->pAt_tbl = pEm06_sca;
    if (G.pEm->Type & 0x20)
    {
        G.pEm->pAt_tbl = Em06_sca_late;
    }
    if (G.SaveData.Stage_no >= 5U)
    {
        G.pEm->pAt_tbl = Em06_sca_late;
    }
    if ((G.pEm->Type & 0xF) < 2U)
    {
        EM06_FREE_P->npc_target = (VOID*)&MAIN.Enemy;  // cast kept: the first enemy slot stored as an entity pointer
        if (G.pEm == EM06_FREE_P->npc_target)
        {
            EM06_FREE_P->npc_target = &MAIN.Enemy[1];
        }
        if (!(EM06_FREE_P->npc_target->Be_flg & 1))
        {
            G.pEm->Type = 2U;
        }
    }

    if (!(G.pEm->Type & 0xF))
    {
        G.pEm->Type = G.pEm->Type | 2;
    }
    EM06_FREE_P->Sikai_flg = 0;

    EM06_FREE_P->airborne = 0;
    EM06_FREE_P->decap_claw_joint = 0;
    EM06_FREE_P->decap_chance = 0;

    EM06_FREE_P->rsv_188 = 0;
    EM06_FREE_P->decap_cooldown = 0;

    EM06_FREE_P->Wander_rno = 0;

    G.pEm->Neck_no = 2;
    EM06_FREE_P->hit_window_timer = 0;
    EM06_FREE_P->hit_window_count = 0;
    if (G.pEm->Type & 8)
    {
        G.pEm->Routine_1 = 1;
        G.pEm->Routine_2 = 0xB;
    }
}

VOID Em06_Main(VOID)
{
    USHORT unused;    // unused local: reserves stack space

    if (G.pEm->Routine_0 != 0 && (G.pEm->Type & 0x40))
    {
        G.pEm->Routine_0 = 8U;
    }
    if (--EM06_FREE_P->hit_window_timer < 0)
    {
        EM06_FREE_P->hit_window_timer = 0;
        EM06_FREE_P->hit_window_count = 0;
    }
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        Em06_routine_0.state[G.pEm->Routine_0]();
        if (G.pEm->Routine_0 != 5)
        {
            FUNC(OBA_SET_OFS)(G.pEm);
            FUNC(OBA_CK_HIT)(&MAIN.Player.Be_flg, G.pEm);
            FUNC(OBA_CK_EM)();
            EM06_FREE_P->room_hit = EM06_FREE_P->room_hit | FUNC(SCA_CK_HIT)(&G.pEm->Pos_x, G.pEm->pAt_tbl->At_w);
            EM06_FREE_P->Dir_ctrl_flg = ScratchHalf[12];
        }
    }
    G.pEm->Zone_in_flg = FUNC(HIT_CK_POINT4)(&G.pEm->Pos_x, G.Cccut);
    if (G.pEm->Zone_in_flg != 0)
    {
        FUNC(MUL_KAGE)(&G.pEm->Pos_x, &G.pEm->Kage_work, 0U, G.pEm->Cdir_y);
    }
}

VOID Em06_Move(VOID)
{
    if (G.pEm->Type & 0x80)
    {
        return;
    }
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    G.pEm->Be_flg = G.pEm->Be_flg | 0x40;
    FUNC(EM_CK_HANI)(0xFA0);
    ScratchWord[0] = G.Player.Pos_y - G.pEm->Pos_y;
    if (ScratchWord[0] != 0)
    {
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        if (ScratchWord[0] < 0)
        {
            G.pEm->Be_flg = G.pEm->Be_flg | 0x20;
        }
        else
        {
            G.pEm->Be_flg = G.pEm->Be_flg | 0x80;
        }
    }
    if (EM06_FREE_P->decap_cooldown != 0)
    {
        EM06_FREE_P->decap_cooldown--;
    }
    if (G.pEm->Routine_1 == 0)
    {
        Em06_variant_dispatch();
    }
    Em06_Move_main();
}

VOID Em06_Damage(VOID)
{
    if (G.pEm->Routine_1 == 0)
    {
        EM06_FREE_P->hit_window_count++;
        if (EM06_FREE_P->hit_window_timer == 0)
        {
            EM06_FREE_P->hit_window_timer = 0x32;
        }
        EM_ROUTINE_WORD = ROUTINES(0, 0, 1, 2);  // cast kept: Routine_0..3 are written as one word
        G.pEm->Routine_2 = Em06_routine_0.seed[(G.pEm->Damage_no & 7) - 1];
        G.pEm->Routine_2 += (((UINT)FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400)) >> 10) & 1;  // cast kept: the distance test result is shifted as an unsigned value
        if (EM06_FREE_P->airborne != 0)
        {
            ScratchWord[7] = G.pEm->Routine_2;
            G.pEm->Routine_2 = 4U;
            if (G.pEm->Pos_y >= -0x1F3)
            {
                G.pEm->Pos_y = 0;
                EM06_FREE_P->airborne = 0;
                G.pEm->Routine_2 = ScratchWord[7];
            }
        }
        if ((G.pEm->Be_flg & EM_BE_ATTACK_MASK) == 0x20)
        {
            G.pEm->Routine_2 = 5U;
        }
        if (G.pEm->Type & 8)
        {
            G.pEm->Routine_2 = 8U;
        }
    }
    Em06_dm[G.pEm->Routine_2]();
}

VOID Em06_Die(VOID)
{
    if (G.pEm->Routine_1 == 0)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 0, 1, 3);  // cast kept: Routine_0..3 are written as one word
        G.pEm->Routine_2 = Em06_routine_0.seed[(G.pEm->Damage_no & 7) - 1];
        if ((G.pEm->Damage_no & 3) != 2)
        {
            G.pEm->Routine_2 = G.pEm->Routine_2 + ((((UINT)FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400U)) >> 0xA) & 1);  // cast kept: the distance test result is shifted as an unsigned value
        }
        if (EM06_FREE_P->airborne != 0)
        {
            ScratchWord[7] = G.pEm->Routine_2;
            G.pEm->Routine_2 = 4U;
            if (G.pEm->Pos_y >= -0x1F3)
            {
                G.pEm->Pos_y = 0;
                EM06_FREE_P->airborne = 0;
                G.pEm->Routine_2 = ScratchWord[7];
            }
        }
        if ((G.pEm->Be_flg & EM_BE_ATTACK_MASK) == 0x20)
        {
            G.pEm->Routine_2 = 6U;
        }
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg, G.pEm->Em_set_flg);
    }
    Em06_die[G.pEm->Routine_2]();
}

VOID Em06_Die_fall(VOID)
{
    G.pEm->Move_no = 0x11;
    G.pEm->Spd_cur = 0;
    Em06_Die_main();
    if (G.pEm->Move_cnt != 0x32 && G.pEm->Move_cnt != 0x24)
    {
    }
    else
    {
        FUNC(SND_SE_ENEM)(7, 0);
    }
    if (G.pEm->Move_cnt == 0x4B)
    {
        FUNC(SND_SE_ENEM)(4, 0);
    }
    if (G.pEm->Hokan_flg == 0)
    {
        if (G.pEm->Move_cnt >= 0x18U)
        {
            Em06_recenter_on_joint(1);
            return;
        }
        Em06_recenter_on_joint(0);
    }
}

VOID Em06_Die_thrash(VOID)
{
    G.pEm->Move_no = 7;
    G.pEm->Spd_cur = 0x46;
    if (G.pEm->Move_cnt < 3U)
    {
        G.pEm->Spd_cur = 0xFA;
    }
    Em06_Die_main();

    if (G.pEm->Move_cnt != 0xD && G.pEm->Move_cnt != 0x17)
    {
        return;
    }
    FUNC(SND_SE_ENEM)(4, 0);
}

VOID Em06_Die_settle(VOID)
{
    G.pEm->Move_no = 0xF;
    G.pEm->Spd_cur = 0;
    Em06_Die_main();
    Em06_recenter_on_joint(0);
}

VOID Em06_Die_collapse(VOID)
{
    G.pEm->Move_no = 6;
    G.pEm->Spd_cur = 0;
    Em06_Die_main();
}

VOID Em06_Dead(VOID)
{
}

VOID Em06_Move_main(VOID)
{
    ScratchWord[7] = FUNC(EM_SIKAI_PROC)();

    if (EM06_FREE_P->Sikai_flg == 0)
    {
        if ((ScratchWord[7] >> 1) == 0)
        {
            EM06_FREE_P->Sikai_flg = EM06_FREE_P->Sikai_flg & ~1;
            EM06_FREE_P->Sikai_flg = EM06_FREE_P->Sikai_flg | (ScratchWord[7] & 1);
        }
        if (EM06_FREE_P->Sikai_flg != 0)
        {
            EM06_FREE_P->decap_cooldown = 0x3C;
        }
    }
    FUNC(ROOT_CK)(G.Player.Pos_x, G.Player.Pos_z, &G.pEm->Dest_x, &G.pEm->Dest_z);
    Em06_mv[G.pEm->Routine_2]();
}

VOID Em06_variant_dispatch(VOID)
{
    ScratchWord[0] = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);
    if (G.pEm->Type & 0x10)
    {
        Em06_coward_ai();
        return;
    }
    Em06_Move_type[(G.pEm->Type & 0xF)]();
}

VOID Em06_Move_type00(VOID)
{
    USHORT dist_off;
    USHORT player_hp;
    ENEMY_WORK* pSub;

    if (G.pEm->Type == 0)
    {
        G.pEm->Type = 2U;
        return;
    }

    pSub = EM06_FREE_P->npc_target;
    if (pSub->Life < 0)
    {
        G.pEm->Type = 2U;
    }
    ScratchWord[1] = __builtin_abs(pSub->Pos_x - G.pEm->Pos_x) + __builtin_abs(pSub->Pos_z - G.pEm->Pos_z);
    ScratchWord[2] = G.pEm->Routine_2;
    if (pSub->Damage_no != 0)
    {
        if (ScratchWord[2] != 2)
        {
            G.pEm->Routine_3 = 0;
            G.pEm->Hokan_flg = 7;
        }
        G.pEm->Routine_2 = 2U;
        G.pEm->Routine_1 = 0;
    }
    if (EM06_FREE_P->Sikai_flg != 0 && ScratchWord[0] < 0xFA0)
    {
        if (ScratchWord[2] != 2)
        {
            G.pEm->Routine_3 = 0;
            G.pEm->Hokan_flg = 7;
        }
        G.pEm->Routine_2 = 2U;
        G.pEm->Routine_1 = 0;
    }
    if ((G.Player.Damage_no & 0x80) && ScratchWord[0] < 0xBB8)  // cast kept: the player Damage_no is a SCHAR, tested as an unsigned byte
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_23 = 9;
    }
    if (G.Player.Damage_no != 0)  // cast kept: the player Damage_no is a SCHAR, tested as an unsigned byte
    {
        return;
    }

    if (FUNC(SIKAI_CK)(&MAIN.Player.Pos_x) == 0 && ScratchWord[0] < 0xBB8)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x80) == 0)
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_23 = 4;
        }
    }
    if (EM06_FREE_P->Sikai_flg != 0 && (G.pEm->Type & 0xF) != 7)
    {
        dist_off = ScratchWord[0] - 5000;
        if (ScratchWord[0] > ScratchWord[1])
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x20) == 0 && ScratchWord[1] < 0xFA0)
            {
                if (FUNC(GOTO00_CK)(&pSub->Pos_x, 0x200) == 0 && pSub->Spd_cur < 0x32)
                {
                    G.pEm->Routine_1 = 1;
                    EM_ROUTINE_23 = 6;
                    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
                }
            }
        }
        if (FUNC(GOTO00_CK)(&pSub->Pos_x, 0x200) == 0 && EM06_FREE_P->room_hit != 0 && ScratchWord[1] < 0xBB8)
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_23 = 6;
            G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        }
    }

    ScratchWord[0] = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);

    dist_off = ScratchWord[0] - 3000;
    player_hp = G.Player.Life;

    if (ScratchWord[0] < 0x7D0)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x100) == 0 && EM06_FREE_P->decap_chance != 0)
        {
            ScratchPad->Sv.vx = G.Player.Pos_x - G.pEm->Pos_x;
            ScratchPad->Sv.vy = 0;
            ScratchPad->Sv.vz = G.Player.Pos_z - G.pEm->Pos_z;
            ScratchPad->Mat = *Monitor.pId_mat;
            ScratchWord[0] = -0x96;
            ScratchWord[0] = ScratchWord[0] + (G.SaveData.Sce_random & 1) * 0x12C;
            FUNC(ROT_MATRIX_Y)(ScratchWord[0], &ScratchPad->Mat);
            FUNC(APPLY_MATRIX_SV)(&ScratchPad->Mat, &ScratchPad->Sv, &ScratchPad->Sv);
            EM06_FREE_P->decap_claw_joint = (ScratchWord[0] + 0x96) / 100 + 6;
            EM06_FREE_P->leap_target_x = G.pEm->Pos_x + ScratchPad->Sv.vx;
            EM06_FREE_P->leap_target_z = G.pEm->Pos_z + ScratchPad->Sv.vz;
            FUNC(ADD_SPEED_XZ)(0);
            G.pEm->Cdir_y += 0x20 - (EM06_FREE_P->decap_claw_joint - 6) * 0x14;

            G.pEm->Routine_1 = 1;
            EM_ROUTINE_23 = 5;

            return;
        }
    }
}

VOID Em06_Move_type01(VOID)
{
    ENEMY_WORK* pSub;

    pSub = EM06_FREE_P->npc_target;
    ScratchWord[1] = __builtin_abs(pSub->Pos_x - G.pEm->Pos_x) + __builtin_abs(pSub->Pos_z - G.pEm->Pos_z);
    if (G.pEm->Routine_2 != 8)
    {
        G.pEm->Routine_3 = 0;
        G.pEm->Hokan_flg = 7;
    }
    G.pEm->Routine_2 = 8U;
    G.pEm->Routine_1 = 0;
}

VOID Em06_Move_type02(VOID)
{
    USHORT player_hp;

    ScratchWord[0] = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);

    ScratchWord[2] = G.pEm->Routine_2;
    player_hp = G.Player.Life;

    EM06_FREE_P->decap_chance = 0;
    if (player_hp < (Em06_pl_life_lim[G.Player.Id & 1] & 0xFFFF) && G.Player.Life > 0)
    {
        EM06_FREE_P->rsv_188 = 0;
        if (G.pEm->Life < 0x28)
        {
            EM06_FREE_P->decap_chance = Em06_decap_chance_low[Api_rand() & 0xF];
        }
        if (G.pEm->Life < 0x4F)
        {
            EM06_FREE_P->decap_chance = Em06_decap_chance_mid[Api_rand() & 0xF];
        }
        if (G.Player.Life >= 0x10)
        {
            if (ScratchWord[0] >= 0x157C && G.Player.Damage_no == 0)  // cast kept: the player Damage_no is a SCHAR, tested as an unsigned byte
            {
            }
            else
            {
                EM06_FREE_P->decap_chance = 0;
            }
        }
    }

    if (EM06_FREE_P->decap_cooldown != 0)
    {
        EM06_FREE_P->decap_chance = 0;
    }
    if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x2C8) == 0 && ScratchWord[0] < 0x1900 && EM06_FREE_P->decap_chance != 0)
    {
        ScratchPad->Sv.vx = G.Player.Pos_x - G.pEm->Pos_x;
        ScratchPad->Sv.vy = 0;
        ScratchPad->Sv.vz = G.Player.Pos_z - G.pEm->Pos_z;
        ScratchPad->Mat = *Monitor.pId_mat;
        ScratchWord[0] = -0x96;
        ScratchWord[0] = ScratchWord[0] + (Api_rand() & 1) * 0x12C;
        FUNC(ROT_MATRIX_Y)(ScratchWord[0], &ScratchPad->Mat);
        FUNC(APPLY_MATRIX_SV)(&ScratchPad->Mat, &ScratchPad->Sv, &ScratchPad->Sv);
        EM06_FREE_P->decap_claw_joint = (ScratchWord[0] + 0x96) / 100 + 6;
        EM06_FREE_P->leap_target_x = G.pEm->Pos_x + ScratchPad->Sv.vx;
        EM06_FREE_P->leap_target_z = G.pEm->Pos_z + ScratchPad->Sv.vz;
        FUNC(ADD_SPEED_XZ)(0);
        G.pEm->Cdir_y = (USHORT)G.pEm->Cdir_y + 0x20 - (EM06_FREE_P->decap_claw_joint - 6) * 0x14;  // truncates to an unsigned halfword
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_23 = 5;
        return;
    }
    if (EM06_FREE_P->Sikai_flg != 0 && ScratchWord[0] < 0x1770)
    {
        if (ScratchWord[2] != 2)
        {
            G.pEm->Routine_3 = 0;
            G.pEm->Hokan_flg = 7;
        }
        G.pEm->Routine_2 = 2U;
        G.pEm->Routine_1 = 0;
    }
    if ((G.Player.Damage_no & 0x80) && ScratchWord[0] < 0xBB8)  // cast kept: the player Damage_no is a SCHAR, tested as an unsigned byte
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_23 = 9;
    }
    if (G.Player.Damage_no != 0)  // cast kept: the player Damage_no is a SCHAR, tested as an unsigned byte
    {
        return;
    }

    if (FUNC(SIKAI_CK)(&MAIN.Player.Pos_x) == 0 && ScratchWord[0] < 0xBB8)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x80) == 0)
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_23 = 4;

            return;
        }
    }

    if (EM06_FREE_P->Sikai_flg != 0 && (G.pEm->Type & 0xF) != 7 && ScratchWord[0] >= 0x1901 && FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x20) == 0)
    {
        if (G.Player.Routine_2 == 0x12 || (G.Player.Routine_2 == 0x13 && (G.SaveData.Sce_random & 1)))
        {
            if (FUNC(CDIR_CK)(&MAIN.Player) == 0)
            {
                G.pEm->Routine_1 = 1;
                EM_ROUTINE_23 = 6;
                G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
            }
        }
    }
}

VOID Em06_Move_type04(VOID)
{
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    if (ScratchWord[0] < 0xFA0)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_23 = 8;

        G.pEm->Type = G.pEm->Type + 0xFC;
    }
}

VOID Em06_coward_ai(VOID)
{
    if (EM06_FREE_P->Sikai_flg != 0 && ScratchWord[0] < 0x2328)
    {
        G.pEm->Type = G.pEm->Type & 0xEF;
    }
    if (ScratchWord[0] < 0xFA0)
    {
        G.pEm->Type = G.pEm->Type & 0xEF;
    }
}

VOID Em06_Move_idle(VOID)
{
    Em06_idle_variant_tbl[(G.pEm->Type & 0xF)]();
}

VOID Em06_idle_stand(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0x15;

        G.pEm->Timer0 = 0x1EU;

        G.pEm->Hokan_flg = 7;
        EM06_FREE_P->decap_claw_joint = 0;
        G.pEm->Spd_cur = 0;
    }
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
    if (G.pEm->Timer0-- == 0)
    {
        if (G.pEm->Type & 0x10)
        {
            G.pEm->Timer0 = 0xC8U;
            return;
        }
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_23 = 1;
    }
}

VOID Em06_idle_walk(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 1;
        G.pEm->Hokan_flg = 7;
        G.pEm->Spd_cur = 0;
        EM06_FREE_P->decap_claw_joint = 0;
    }
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
}

VOID Em06_Move_chase(VOID)
{
    G.pEm->Move_no = 0x10;
    G.pEm->Spd_cur = 0x28;
    ScratchPad->Vec = PL_POS_VEC;  // cast kept: VECTOR view of Pos_x/y/z/Workm, a 16-byte struct copy
    ScratchWord[7] = 6;

    FUNC(EM_WANDER_TURN)(EM06_FREE_P->Dir_ctrl_flg, &EM06_FREE_P->room_hit, &EM06_FREE_P->Wander_rno, 0x18, 0x3C);

    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;

        G.pEm->Hokan_flg = 3;

        EM06_FREE_P->decap_claw_joint = 0;
    }
    if (G.pEm->Move_cnt == 1)
    {
        FUNC(SND_SE_ENEM)(0U, 0);
    }
    if (G.pEm->Move_cnt == 0x20)
    {
        FUNC(SND_SE_ENEM)(0U, 0);
    }
    ScratchWord[0] = (__builtin_abs(G.Player.Pos_x - G.pEm->Pos_x)) + (__builtin_abs(G.Player.Pos_z - G.pEm->Pos_z));
    if (ScratchWord[0] < 0x7D0)
    {
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x18);
    }

    FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);

    FUNC(ADD_SPEED_XZ)(0U);
}

VOID Em06_Move_nop(VOID)
{
}

VOID Em06_Move_atck(VOID)
{
    Em06_attack_sub_tbl[G.pEm->Routine_3]();
    Em06_recenter_on_joint(1);
}

VOID Em06_atk_start(VOID)
{
    G.pEm->Routine_3 = 1;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 7;
    G.pEm->Spd_cur = 0;
    G.pEm->Move_no = 5;
}

VOID Em06_atk_swing(VOID)
{
    UCHAR frame;
    PARTS_W* pP_ptr;

    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)
    {
        G.pEm->Routine_3++;
        if (!(G.pEm->Type & 0x40))
        {
            EM_ROUTINE_WORD = ROUTINES(0, 1, 0, 1);  // cast kept: Routine_0..3 are written as one word
        }
        return;
    }
    if (!(G.pEm->Type & 0x40))
    {
        frame = G.pEm->Move_cnt + 0xFC;

        if (frame < 8U)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            pP_ptr += 9;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchWord[1] = FUNC(L_POS_CK_XZM)(&pP_ptr->Workm, &ScratchPad->Vec, 0x320, &MAIN.Player.Pos_x);

            if (ScratchWord[1] != 0 && G.Player.Damage_no == 0)  // cast kept: the player Damage_no is a SCHAR, tested as an unsigned byte
            {
                FUNC(SND_SE_ENEM)(5, 0);
                G.Player.Damage_no = 1;

                PL_ROUTINE_WORD = ROUTINES(0, 0x64, 0, 2);  // cast kept: Routine_0..3 are written as one word

                FUNC(PAD_VIB_SET0)(7, 0);
                FUNC(PAD_VIB_SET1)(3, 0xFA, 4);

                G.pEm->Routine_3 = 2U;

                G.Player.Life -= 0xD;
            }
        }
    }
}

VOID Em06_atk_lunge(VOID)
{
    PARTS_W* pP_ptr;

    G.pEm->Spd_cur = 0xC8;
    FUNC(ADD_SPEED_XZ)(0x52C);
    G.pEm->Routine_3 = 3;
    G.pEm->Timer0 = 4;
    pP_ptr = G.pEm->pSin_parts_ptr;
    pP_ptr += 9;
    ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
    ScratchPad->Vec.vx = 0xC8;

    FUNC(ESP_CALL)(0, 0, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);

    FUNC(MODEL_COLOR_PLUS)(pP_ptr, 0x30, 0x80820, 0x606060);
}

VOID Em06_atk_recover(VOID)
{
    G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
    if (G.pEm->Timer0 != 0)
    {
        G.pEm->Timer0--;
        G.Player.Pos_x += G.pEm->Spd_x;
        G.Player.Pos_z += G.pEm->Spd_z;
    }
}

VOID Em06_atk_finish(VOID)
{
    ENEMY_WORK* pSub;

    EM_ROUTINE_HALF0 = 1;  // cast kept: Routine_0 and Routine_1 are written together as one halfword
    ScratchWord[7] = (Api_rand() & 3) == 0;
    if (!(G.pEm->Type & 0xF))
    {
        pSub = EM06_FREE_P->npc_target;
        if ((pSub->Life & 0x80) == 0)
        {
            ScratchWord[7] = Api_rand() & 1;
        }
    }
    if ((G.pEm->Type & 0xF) == 7)
    {
        ScratchWord[7] = 0;
    }
    G.pEm->Routine_1 = ScratchWord[7];
    G.pEm->Routine_2 = ScratchWord[7] * 5 + 1;
    G.pEm->Routine_3 = 0;
}

VOID Em06_Move_decap(VOID)
{
    EM06_FREE_P->decap_chance = 0;

    Em06_decap_leap_sub_tbl[G.pEm->Routine_3]();
    if (G.pEm->Move_cnt < 0x17U)
    {
        ScratchPad->Vec.vx = EM06_FREE_P->leap_target_x;
        ScratchPad->Vec.vz = EM06_FREE_P->leap_target_z;
        ScratchPad->Vec.vy = 0;
        G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&ScratchPad->Vec, 0x18U);
        FUNC(ADD_SPEED_XZ)(0);
        return;
    }
    if (G.pEm->Move_cnt < 0x1FU)
    {
        G.pEm->Spd_cur = 0x32;
        FUNC(ADD_SPEED_XZ)(0x800);
        return;
    }
}

VOID Em06_decap_leap_start(VOID)
{
    G.pEm->Routine_3 = 1;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 7;
    G.pEm->Spd_cur = 0x12C;
    G.pEm->Move_no = 0x12 - EM06_FREE_P->decap_claw_joint;
    if (G.pEm->Type & 0x40)
    {
        G.pEm->Move_no = 0xC;
    }
    FUNC(SND_SE_ENEM)(2, 0);
}

VOID Em06_decap_strike(VOID)
{
    UCHAR frame;
    PARTS_W* pP_ptr;

    if (!(G.pEm->Type & 0x40) && G.Player.Life > 0)
    {
        frame = G.pEm->Move_cnt + 0xF9;

        pP_ptr = G.Player.pSin_parts_ptr;
        if (frame < 8U && !(pP_ptr[1].Be_flg & 0x40))
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            pP_ptr = pP_ptr + EM06_FREE_P->decap_claw_joint;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchWord[1] = FUNC(L_POS_CK_XZM)(&pP_ptr->Workm, &ScratchPad->Vec, 0x2BC, &MAIN.Player.Pos_x);
            if (ScratchWord[1] != 0)
            {
                G.Player.Damage_no = 1;
                PL_ROUTINE_WORD = ROUTINES(0, 0, 6, 7);  // cast kept: Routine_0..3 are written as one word
                G.pEm->Routine_3 = 3U;
                G.pEm->Timer0 = 4;
                pP_ptr = G.pEm->pSin_parts_ptr;
                pP_ptr = pP_ptr + EM06_FREE_P->decap_claw_joint;
                ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
                ScratchPad->Vec.vx = 0xC8;

                FUNC(ESP_CALL)(0, 3, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);

                FUNC(MODEL_COLOR_PLUS)(pP_ptr, 0x30, 0x80820, 0x606060);
                FUNC(SND_SE_ENEM)(5, 0);

                FUNC(PAD_VIB_SET0)(8, 0);

                return;
            }
        }
    }
    G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
}

VOID Em06_decap_leap_end(VOID)
{
    EM_ROUTINE_WORD = ROUTINES(0, 1, 0, 1);  // cast kept: Routine_0..3 are written as one word
    EM06_FREE_P->decap_claw_joint = 0;
}

VOID Em06_decap_leap_track(VOID)
{
    PARTS_W* pPl_parts;

    G.pEm->Routine_3 += FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
    pPl_parts = G.Player.pSin_parts_ptr;
    pPl_parts[1].Be_flg = pPl_parts[1].Be_flg & 0xFE;
    Em06_Catch_set(EM06_FREE_P->decap_claw_joint);
}

VOID Em06_decap_leap_to_hold(VOID)
{
    EM_ROUTINE_23 = 7;
}

VOID Em06_Move_dodge(VOID)
{
    Em06_dodge_sub_tbl[G.pEm->Routine_3]();
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em06_dodge_start(VOID)
{
    G.pEm->Routine_3 = 1;
    G.pEm->Routine_1 = 1;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Timer2 = 0;
    G.pEm->Hokan_flg = 7;
    G.pEm->Spd_cur = 0x5A;
    G.pEm->Move_no = 0x13;
    EM06_FREE_P->airborne = 1;
    EM06_FREE_P->decap_claw_joint = 0;
    if (G.pEm->Type & 0x40)
    {
        G.pEm->Spd_cur = 0x3C;
    }
}

VOID Em06_dodge_hop(VOID)
{
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
    if (G.pEm->Move_cnt & 4)
    {
        G.pEm->Routine_3 = 2;
        G.pEm->Spd_cur = 0x82;
        FUNC(SND_SE_ENEM)(2, 0);
    }
}

VOID Em06_dodge_swipe(VOID)
{
    UCHAR frame;
    PARTS_W* pP_ptr;

    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    if (G.pEm->Move_cnt >= 9U)
    {
        G.pEm->Be_flg = G.pEm->Be_flg | 0x80;
    }
    if (!(G.pEm->Type & 0x40))
    {
        ScratchWord[7] = 2;
        do
        {
            frame = G.pEm->Move_cnt - Em06_swipe_reach_tbl[ScratchWord[7]].first_frame;
            if (frame < Em06_swipe_reach_tbl[ScratchWord[7]].Frm_num && G.Player.Damage_no == 0)  // cast kept: the player Damage_no is a SCHAR, tested as an unsigned byte
            {
                pP_ptr = G.pEm->pSin_parts_ptr;
                pP_ptr = pP_ptr + Em06_swipe_reach_tbl[ScratchWord[7]].joint;
                ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
                ScratchPad->Vec.vx = Em06_swipe_reach_tbl[ScratchWord[7]].Off_x;
                ScratchWord[1] = FUNC(L_POS_CK_XZM)(&pP_ptr->Workm, &ScratchPad->Vec, Em06_swipe_reach_tbl[ScratchWord[7]].radius, &MAIN.Player.Pos_x);

                if (ScratchWord[1] != 0 && G.Player.Damage_no == 0)  // cast kept: the player Damage_no is a SCHAR, tested as an unsigned byte
                {
                    ScratchWord[2] = FUNC(CDIR_CK)(&MAIN.Player);
                    G.Player.Damage_no = ScratchWord[2] + 2;
                    G.Player.Routine_2 = ScratchWord[2] + 0x66;
                    ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
                    ScratchPad->Vec.vx = 0x64;

                    FUNC(ESP_CALL)(0, 0, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);

                    G.pEm->Routine_3 = 4;
                    FUNC(MODEL_COLOR_PLUS)(pP_ptr, 0x30, 0x80820, 0x606060);
                    FUNC(SND_SE_ENEM)(3, 0);

                    G.Player.Life -= 0x14;

                    FUNC(PAD_VIB_SET0)(7, 0);
                    FUNC(PAD_VIB_SET1)(3, 0xFA, 4);

                    return;
                }
            }
        } while (--ScratchWord[7] != -1);
    }
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
    if (FUNC(EM_FALL_MOVE)(G.pEm->Spd_cur, 0x244, -0x32, 0) != 0)
    {
        G.pEm->Routine_3 = 3;
        G.pEm->Spd_cur = 0x5A;
        EM06_FREE_P->airborne = 0;
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        return;
    }
}

VOID Em06_dodge_end(VOID)
{
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)
    {
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_23 = 2;
        return;
    }
}

VOID Em06_dodge_land(VOID)
{
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    if (G.pEm->Move_cnt >= 9U)
    {
        G.pEm->Be_flg = G.pEm->Be_flg | 0xC0;
    }
    if (FUNC(EM_FALL_MOVE)(G.pEm->Spd_cur, 0x244, -0x32, 0) != 0)
    {
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_23 = 2;
        EM06_FREE_P->airborne = 0;
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        return;
    }
}

VOID Em06_Move_catch(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no++;
        G.pEm->Spd_cur = 0;

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
        Em06_Catch_set(EM06_FREE_P->decap_claw_joint);
        return;
    case 2:
        G.pEm->Routine_3 = 3U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no++;
        G.pEm->Timer0 = 5U;

    case 3:
        G.pEm->Timer0 = G.pEm->Timer0 - FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        if (G.pEm->Timer0 == 0)
        {
            G.pEm->Routine_3 = 4U;
        }
        Em06_Catch_set(EM06_FREE_P->decap_claw_joint);
        return;
    case 4:
        EM_ROUTINE_WORD = ROUTINES(0, 1, 0, 1);  // cast kept: Routine_0..3 are written as one word
        break;
    }
}

VOID Em06_Move_yori(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 2;

        G.pEm->Spd_cur = 0xF0;

        G.pEm->Timer0 = 0xAU;
        EM06_FREE_P->decap_claw_joint = 0;
        EM06_FREE_P->approach_frames = 0;
    }
    if (++EM06_FREE_P->approach_frames == 0x5A)
    {
        EM_ROUTINE_23 = 1;
    }
    FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
    if (G.pEm->Move_cnt == 1)
    {
        FUNC(SND_SE_ENEM)(1U, 0);
    }
    if (G.pEm->Move_cnt == 0xF)
    {
        FUNC(SND_SE_ENEM)(1U, 0);
    }
    ScratchPad->Vec = PL_POS_VEC;  // cast kept: VECTOR view of Pos_x/y/z/Workm, a 16-byte struct copy
    ScratchWord[7] = 7;

    FUNC(EM_WANDER_TURN)(EM06_FREE_P->Dir_ctrl_flg, &EM06_FREE_P->room_hit, &EM06_FREE_P->Wander_rno, 0x60, 0x3C);

    ScratchWord[0] = (__builtin_abs(G.Player.Pos_x - G.pEm->Pos_x)) + (__builtin_abs(G.Player.Pos_z - G.pEm->Pos_z));

    G.pEm->Timer0--;
    ScratchWord[0] = (__builtin_abs(G.Player.Pos_x - G.pEm->Pos_x)) + (__builtin_abs(G.Player.Pos_z - G.pEm->Pos_z));
    if (ScratchWord[0] < 0xFA0)
    {
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x60);
    }
    FUNC(ADD_SPEED_XZ)(0U);
}

VOID Em06_Move_jump(VOID)
{
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Timer2 = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 0U;
        G.pEm->Spd_cur = 0x5A;
        EM06_FREE_P->decap_claw_joint = 0;
        EM06_FREE_P->airborne = 1;

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
        if (G.pEm->Move_cnt < 0xAU)
        {
            break;
        }
    case 2:
        if (FUNC(EM_FALL_MOVE)(0, 0, -0x3C, 0) != 0)
        {
            G.pEm->Routine_3 = 3U;
            G.pEm->Move_cnt = 0U;
            G.pEm->Move_wait = 0;
            G.pEm->Hokan_flg = 7;
            G.pEm->Move_no++;
            G.pEm->Spd_cur = 0;
            FUNC(SND_SE_ENEM)(4, 0);
        }
        break;
    case 3:
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)
        {
            G.pEm->Routine_1 = 0;
            EM_ROUTINE_23 = 1;
            EM06_FREE_P->airborne = 0;
            G.pEm->Be_flg = G.pEm->Be_flg & ~EM_BE_NO_WALL;
            if (G.pEm->Type & 0x40)
            {
                FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);
                EM_ROUTINE_23 = 0;
            }
        }
    }
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em06_Move_ikaku(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3++;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 1;
        G.pEm->Timer2 = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 0x17;

    case 1:
        if (G.pEm->Move_cnt == 8)
        {
            FUNC(SND_SE_ENEM)(7, 0);
        }
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)
        {
            if (G.pEm->Type & 0x40)
            {
                FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);
            }
            EM_ROUTINE_23 = 0;
        }
    }
}

VOID Em06_Move_back(VOID)
{
    ScratchWord[0] = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3++;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 1;
        G.pEm->Timer2 = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 0x14;
        G.pEm->Spd_cur = 0x96;
        if (ScratchWord[0] >= 0xBB9)
        {
            G.pEm->Spd_cur = 0xFA;
        }
        EM06_FREE_P->backstep_dir = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 1) + 2;

    case 1:
        if (FUNC(JOINT_MOVE)(1, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 2, 0, 1);  // cast kept: Routine_0..3 are written as one word
        }
        G.pEm->Cdir_y += (EM06_FREE_P->backstep_dir - 2) << 6;
    }
    FUNC(ADD_SPEED_XZ)(EM06_FREE_P->backstep_dir * 0x400);
}

VOID Em06_Move_tojo(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        switch (STAGE_ROOM)  // Stage_no and Room_no switched on as one halfword
        {
        case ((0xA) << 8 | (2)):
            Em06_jump_kind = 2;
            break;
        case ((3) << 8 | (5)):
            Em06_jump_kind = 3;
            break;
        case ((9) << 8 | (5)):
            Em06_jump_kind = 4;
            if (G.SaveData.Room_no_old != 0xA && G.SaveData.Room_no_old != 0x15)
            {
            }
            else
            {
                Em06_jump_kind = 5;
            }
            break;
        }
        G.pEm->Routine_3++;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = ((SCHAR*)Em06_intro_move_tbl)[G.pEm->Type * 2 - 16];  // the table read as bytes

    case 1:
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 2, 0, 1);  // cast kept: Routine_0..3 are written as one word
            G.pEm->Cdir_y = (G.pEm->Cdir_y + ((USHORT*)Em06_intro_move_tbl - 6)[G.pEm->Type]) & 0xFFF;  // cast kept: Em06_intro_move_tbl is a byte table read as halfwords
            G.pEm->Type = 2U;
            return;
        }
        if (G.pEm->Move_cnt < 6U)
        {
            G.pEm->Pos_x = G.pEm->Pos_x + Em06_intro_move_tbl[Em06_jump_kind].x * 2;
            G.pEm->Pos_z = G.pEm->Pos_z + Em06_intro_move_tbl[Em06_jump_kind].y * 2;
        }
        if (G.pEm->Move_cnt == 0x14)
        {
            FUNC(SND_SE_ENEM)(7, 0);
        }
        return;
    }
}

VOID Em06_act_swipe(VOID)
{
    PARTS_W* pP_ptr;

    G.pEm->Move_no = G.pEm->Routine_2 == 0 ? 3 : 4;
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Spd_cur = 0;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        FUNC(SND_SE_ENEM)(6, 0U);
        if (G.pEm->Type & 0x40)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

            FUNC(ESP_CALL)(3, 8U, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(9, 6U, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);
        }

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
        return;
    case 2:
        ScratchUword[0] = ((UINT)FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x20U)) >> 5;  // cast kept: the distance test result is shifted as an unsigned value
        if (!(Api_rand() & 1))
        {
            ScratchWord[0] = ((UINT)FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x80U)) >> 7;  // cast kept: the distance test result is shifted as an unsigned value
            G.pEm->Routine_2 += ScratchWord[0] == 0 ? 5 : 0;
            G.pEm->Routine_1 = ScratchWord[0] == 0;
        }
        if (G.pEm->Type & 2)
        {
            ScratchWord[0] = Api_rand() & 1;
            G.pEm->Routine_1 = ScratchWord[0];
            G.pEm->Routine_2 = ScratchWord[0] * 4 + 2;
            G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        }
        G.pEm->Dest_x = G.Player.Pos_x;
        G.pEm->Dest_z = G.Player.Pos_z;
        G.pEm->Damage_no = 0;
        if (G.pEm->Type & 0x40)
        {
            FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);
            EM_ROUTINE_23 = 0;
        }
        else
        {
            EM_ROUTINE_WORD = ROUTINES(0, 1, 0, 1);  // cast kept: Routine_0..3 are written as one word
        }
        if (EM06_FREE_P->hit_window_timer != 0 && EM06_FREE_P->hit_window_count == 2)
        {
            EM06_FREE_P->hit_window_timer = 0;
            EM06_FREE_P->hit_window_count = 0;
            G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
            EM_ROUTINE_WORD = ROUTINES(0, 0xA, 1, 1);  // cast kept: Routine_0..3 are written as one word
        }
        return;
    }
}

VOID Em06_act_leapattack(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Timer2 = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 7;
        FUNC(SND_SE_ENEM)(6, 0);
        if (G.pEm->Type & 0x40)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

            FUNC(ESP_CALL)(9, 6, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(3, 8, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);
        }

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);

    case 2:
        if (FUNC(EM_FALL_MOVE)(G.pEm->Spd_cur / 2, 0, -0x32, 0) != 0)
        {
            G.pEm->Routine_3 = 3U;
        }
        return;
    case 3:
        G.pEm->Routine_3 = 4U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 0x12;

    case 4:
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)
        {
            G.pEm->Routine_3 = 5U;
            G.pEm->Move_cnt = 0U;
            G.pEm->Move_wait = 0;
            G.pEm->Hokan_flg = 7;
            G.pEm->Move_no = 8;
            G.pEm->Timer0 = (Api_rand() & 0x20) + 0x2D;
            G.pEm->Spd_cur = 0;
            G.pEm->Damage_no = 0;
        }
        return;
    case 5:
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        G.pEm->Be_flg = G.pEm->Be_flg | 0x20;
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
        ScratchWord[0] = (__builtin_abs(G.Player.Pos_x - G.pEm->Pos_x)) + (__builtin_abs(G.Player.Pos_z - G.pEm->Pos_z));
        if (G.pEm->Timer0 == 3)
        {
            if (Api_rand() & 1)
            {
                G.pEm->Routine_3 = 0U;
                G.pEm->Routine_2 = 6;
            }
        }
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_3 = 6U;
        }
        return;
    case 6:
        G.pEm->Routine_3 = 7U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 0xF;
        G.pEm->Spd_cur = 0x50;
        FUNC(ADD_SPEED_XZ)(0xC00);
        G.pEm->Damage_no = 1;

        if ((G.SaveData.Sce_random & 1) && (G.pEm->Type & 0xF) != 7)
        {
            EM06_FREE_P->leap_followup = EM06_FREE_P->leap_followup | 1;
        }

    case 7:
        ScratchWord[0] = FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
        G.pEm->Routine_3 = G.pEm->Routine_3 + ScratchWord[0];

        if (G.pEm->Move_cnt < 0x1FU)
        {
            G.pEm->Cdir_y = G.pEm->Cdir_y + 0x40;
        }

        if (G.pEm->Move_cnt < 0x14U)
        {
            G.pEm->Pos_x = G.pEm->Pos_x + G.pEm->Spd_x;
            G.pEm->Pos_z = G.pEm->Pos_z + G.pEm->Spd_z;
        }
        if (ScratchWord[0] != 0)
        {
            G.pEm->Cdir_y = G.pEm->Cdir_y + 0x800;
        }
        return;
    case 8:
        if (G.pEm->Type & 0x40)
        {
            FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);
            EM_ROUTINE_23 = 0;
        }
        else
        {
            EM_ROUTINE_WORD = ROUTINES(0, 2, 0, 1);  // cast kept: Routine_0..3 are written as one word

            if (EM06_FREE_P->leap_followup & 1)
            {
                EM_ROUTINE_WORD = ROUTINES(0, 6, 1, 1);  // cast kept: Routine_0..3 are written as one word
                EM06_FREE_P->leap_followup = EM06_FREE_P->leap_followup & 0xFE;
            }
        }
        G.pEm->Spd_cur = 0;
        EM06_FREE_P->airborne = 0;
        G.pEm->Dest_x = G.Player.Pos_x;
        G.pEm->Dest_z = G.Player.Pos_z;
        G.pEm->Damage_no = 0;
        G.pEm->Be_flg = G.pEm->Be_flg | 0x40;
        break;
    }
}

VOID Em06_act_pounce(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        if (G.pEm->Type & 8)
        {
            if (G.pEm->Move_cnt >= 6U)
            {
                G.pEm->Cdir_y += ((USHORT*)Em06_intro_move_tbl - 6)[G.pEm->Type];  // cast kept: Em06_intro_move_tbl is a byte table read as halfwords
            }
            G.pEm->Type = 2U;
        }

        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;

        if (G.pEm->Type & 8)
        {
            G.pEm->Hokan_flg = 0;
        }

        G.pEm->Move_no = 7U;
        FUNC(SND_SE_ENEM)(6U, 0);
    }
    if (FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)  // cast kept: the sequence table address is passed as a pointer
    {
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no++;
        G.pEm->Timer0 = (Api_rand() & 0x20) + 0x1E;
        G.pEm->Spd_cur = 0;
        EM_ROUTINE_23 = 0x604;
        if (G.pEm->Type & 8)
        {
            G.pEm->Type = 2U;
        }
        if (G.pEm->Type & 0x40)
        {
            FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);
            EM_ROUTINE_23 = 0;
        }
        return;
    }
    G.pEm->Spd_cur = 0x46;
    if (G.pEm->Move_cnt < 3U)
    {
        G.pEm->Spd_cur = 0xFA;
    }
    if (G.pEm->Move_cnt == 0x10)
    {
        FUNC(SND_SE_ENEM)(4U, 0);
    }
    if (G.pEm->Type == 8)
    {
        if (G.pEm->Move_cnt >= 6U)
        {
            G.pEm->Cdir_y = G.pEm->Cdir_y - 0x400;
        }
        FUNC(ADD_SPEED_XZ)(0x400U);
        return;
    }
    if (G.pEm->Type == 9)
    {
        if (G.pEm->Move_cnt >= 6U)
        {
            G.pEm->Cdir_y = G.pEm->Cdir_y + 0x400;
        }
        FUNC(ADD_SPEED_XZ)(0xC00U);
        return;
    }
    FUNC(ADD_SPEED_XZ)(0x800U);
}

VOID Em06_act_flurry(VOID)
{
    PARTS_W* pP_ptr;

    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1U;
        G.pEm->Spd_cur = 0;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 6;
        FUNC(SND_SE_ENEM)(6, 0);
    }
    pP_ptr = G.pEm->pSin_parts_ptr;
    ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

    FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[3].Workm, &ScratchPad->Vec, 0);

    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)
    {
        EM_ROUTINE_23 = 0x604;
        if (G.pEm->Type & 0x40)
        {
            FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);
            EM_ROUTINE_23 = 0;
        }
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 6;
        G.pEm->Timer0 = 9;
        G.pEm->Spd_cur = 0;
        G.pEm->Damage_no = 0;
    }
}

VOID Em06_act_holdplayer(VOID)
{
    INT unused;    // unused local: reserves stack space

    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1U;
        G.pEm->Spd_cur = 0;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 0x16;
        G.pEm->Damage_no = 1;
    }
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 2, 0, 1);  // cast kept: Routine_0..3 are written as one word
        G.pEm->Spd_cur = 0;
        G.pEm->Damage_no = 0;
    }
}

VOID Em06_Die_main(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Life = -1;
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Timer0 = 0x46U;
        FUNC(SND_SE_ENEM)(7, 0);
        if (G.pEm->Type & 0x40)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

            FUNC(ESP_CALL)(9, 6, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(3, 8, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);
            FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[1].Workm, &ScratchPad->Vec, 0);
        }

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);

        FUNC(ADD_SPEED_XZ)(0x800);
        return;
    case 2:

        FUNC(KAGE_WORK_COLOR_SET)(&G.pEm->Kage_work, 1, 2, 0xFFFF50);

        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, -0x64, -0x64);
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);
        G.pEm->Routine_3 = 3U;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT | EM_BE_NO_OBJ;
        G.pEm->Spd_cur = 0;
        return;
    case 3:
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, 0xC, 0xC);
        if (--G.pEm->Timer0 == 0)
        {
            G.pEm->Routine_3 = 4U;
        }
        return;
    case 4:
        if (G.pEm->Type & 0x40)
        {
            FUNC(FLG_ON)(&MAIN.Room_flg[0], G.pEm->Set_flg_no);
        }
        break;
    }
}

VOID Em06_Die_pounce(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Timer2 = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Timer0 = 0x46;
        G.pEm->Move_no = 0x11;
        FUNC(SND_SE_ENEM)(7, 0);

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);

    case 2:
        if (FUNC(EM_FALL_MOVE)(G.pEm->Spd_cur / 2, 0, -0x32, 0) != 0)
        {
            G.pEm->Routine_3 = 3U;
            G.pEm->Spd_cur = 0;
            EM06_FREE_P->airborne = 0;
        }
        return;
    case 3:
        G.pEm->Routine_3 = 4U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 0x12;
        FUNC(SND_SE_ENEM)(4, 0);

    case 4:
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)
        {
            G.pEm->Routine_2 = 0;
            G.pEm->Routine_3 = 2U;
        }
        break;
    }
}

VOID Pldie_Em06(VOID)
{
    PARTS_W* pPl_parts;

    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;

        G.Player.Move_no = G.Player.Damage_no + 0xFF;

        G.Player.Timer2 = 0xB4;  // cast kept: Damage_no and Timer2 of the player are SCHAR, used as unsigned bytes
        G.Stop_flg &= ~STOP_ACTION;
        G.Player.Hokan_flg = 3;
        pPl_parts = G.Player.pSin_parts_ptr;
        pPl_parts[1].Be_flg = pPl_parts[1].Be_flg | 8;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

        FUNC(ESP_CALL)(0, 3, 0, &pPl_parts[1].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(0, 3, 0, Monitor.pId_mat, pPl_parts[1].Workm.t, 0);

        G.Player.Life = -1;

        FUNC(PAD_VIB_SET0)(0xA, 0xA);
        FUNC(PAD_VIB_SET0)(8, 0x19);
        FUNC(PAD_VIB_SET1)(0x14, 0xC8, 0);
        FUNC(PAD_VIB_FADE_SET)(0x5A, 0xC8, 0, 0x14);

    case 1:
        if (G.pEm->Move_cnt < 0xAU)
        {
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = -0x898;

            FUNC(ESP_CALL)(0, 0, 0, G.pEm->M, &ScratchPad->Vec, 0);
        }
        if (G.pEm->Move_cnt == 3)
        {
            pPl_parts = G.Player.pSin_parts_ptr;
            FUNC(MODEL_COLOR_PLUS)(pPl_parts, 0x30, 0x80820, 0x606060);
            FUNC(MODEL_COLOR_PLUS)(&pPl_parts[9], 0x30, 0x80820, 0x606060);
            FUNC(MODEL_COLOR_PLUS)(&pPl_parts[12], 0x30, 0x80820, 0x606060);
        }
        if (G.pEm->Move_cnt == 0x2A)
        {
            FUNC(SND_SE_WALK)(2, 0);
        }
        G.Player.Routine_3 += FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
        FUNC(FOOT_SET_PL)(0);
        return;
    case 2:
        ScratchPad->Sv.vx = -0x384;
        ScratchPad->Sv.vy = 0;
        ScratchPad->Sv.vz = 0;
        ScratchPad->Mat = *Monitor.pId_mat;
        FUNC(ROT_MATRIX_Y)((SHORT)G.Player.Cdir_y, &ScratchPad->Mat);  // sign-extends the halfword
        FUNC(APPLY_MATRIX_SV)(&ScratchPad->Mat, &ScratchPad->Sv, &ScratchPad->Sv);
        G.pEm->Kage_work.Off_x = G.pEm->Kage_work.Off_x + ScratchPad->Sv.vx;
        G.pEm->Kage_work.Off_y = G.pEm->Kage_work.Off_y + ScratchPad->Sv.vy;
        G.pEm->Kage_work.Off_z = G.pEm->Kage_work.Off_z + ScratchPad->Sv.vz;
        FUNC(KAGE_WORK_COLOR_SET)(&G.pEm->Kage_work, 1, 2, 0xFFFF50);
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, -0xC8, -0xC8);
        G.Player.Routine_3 = 3;
        G.Player.Damage_no = 0x80;  // cast kept: Damage_no and Timer2 of the player are SCHAR, used as unsigned bytes
        return;
    case 3:
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, 0x10, 0x10);
        if (G.Player.Timer2 == 0xA0)
        {
            G.Fade_hrate = 1;
            G.Fade_add = 0x100;
            FUNC(FADE_START)();
        }

        if (((G.Player.Timer2 = G.Player.Timer2 + 0xFF) & 0xFF) == 0x20)  // cast kept: Damage_no and Timer2 of the player are SCHAR, used as unsigned bytes
        {
            G.Player.Routine_0 = 4;
        }
        return;
    case 4:
        G.Player.Routine_0 = 4;
        break;
    }
}

VOID Em06_Catch_set(UCHAR claw_joint)
{
    PARTS_W* pP_ptr;
    PARTS_W* pPl_parts;

    pP_ptr = G.pEm->pSin_parts_ptr;
    FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);
    FUNC(GS_MUL_COORD0)(G.pEm->M, &pP_ptr->Coord, &ScratchPad->Mat);
    FUNC(GS_MUL_COORD3)(&ScratchPad->Mat, &pP_ptr[1].Coord);
    pP_ptr = pP_ptr + EM06_FREE_P->decap_claw_joint;
    ScratchWord[0] = 3;
    for (;;)
    {
        FUNC(GS_MUL_COORD3)(&ScratchPad->Mat, &(pP_ptr - ScratchWord[0])[1].Coord);
        if ((ScratchWord[0] = ScratchWord[0] - 1) == 0)
        {
            break;
        }
    }
    pP_ptr->Workm = ScratchPad->Mat;
    pPl_parts = G.Player.pSin_parts_ptr;
    pPl_parts++;
    MATRIX_POS(&pPl_parts->Workm) = MATRIX_POS(&pP_ptr->Workm);
    ScratchPad->Sv.vx = 0xC8;
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Mat = *Monitor.pId_mat;
    FUNC(ROT_MATRIX_Y)(G.pEm->Cdir_y + 0xBB8 - (EM06_FREE_P->decap_claw_joint - 6) * 0x258, &ScratchPad->Mat);
    FUNC(APPLY_MATRIX_SV)(&ScratchPad->Mat, &ScratchPad->Sv, &ScratchPad->Sv);
    pPl_parts->Workm.t[0] = pPl_parts->Workm.t[0] + ScratchPad->Sv.vx;
    pPl_parts->Workm.t[2] = pPl_parts->Workm.t[2] + ScratchPad->Sv.vz;
}

VOID Em06_recenter_on_joint(UCHAR Flg)
{
    PARTS_W* pP_ptr;
    UCHAR i;

    pP_ptr = G.pEm->pSin_parts_ptr;
    FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);
    FUNC(GS_MUL_COORD0)(G.pEm->M, &pP_ptr->Coord, &ScratchPad->Mat);
    pP_ptr = &pP_ptr[Flg * 3 + 12];
    i = 3;
    while (i != 0)
    {
        FUNC(GS_MUL_COORD3)(&ScratchPad->Mat, &(pP_ptr - i)[1].Coord);
        i--;
    }
    ScratchPad->Mat.t[0] = ScratchPad->Mat.t[0] - pP_ptr->Workm.t[0];
    ScratchPad->Mat.t[1] = 0;
    ScratchPad->Mat.t[2] = ScratchPad->Mat.t[2] - pP_ptr->Workm.t[2];
    G.pEm->Pos_x = G.pEm->Pos_x - ScratchPad->Mat.t[0];
    G.pEm->Pos_z = G.pEm->Pos_z - ScratchPad->Mat.t[2];
}

VOID Em06_Scenario(VOID)
{
    if (!(G.pEm->Type & 0x40))
    {
        G.pEm->Routine_0 = 1;
        return;
    }
    pEm06_victim = (VOID*)&MAIN.Enemy;  // cast kept: the first enemy slot stored as an entity pointer
    Em06_sce_mv[G.pEm->Routine_2]();
}

VOID Em06_Sce_idle(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Move_no = 0x15;
        G.pEm->Spd_cur = 0;
        G.pEm->Routine_3++;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
    }
    FUNC(JOINT_MOVE)(G.pEm->Sce_flg & 1, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);
}

VOID Em06_Sce_walk(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_no = 0x10;
        G.pEm->Spd_cur = 0x28;
        G.pEm->Routine_3++;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        EM06_FREE_P->decap_claw_joint = 0;

    case 1:
        ScratchPad->Vec.vx = G.pEm->Root_x;
        ScratchPad->Vec.vz = G.pEm->Root_z;
        ScratchPad->Vec.vy = 0;
        FUNC(GOTO00)(&ScratchPad->Vec, 0x40U);
        FUNC(JOINT_MOVE)(G.pEm->Sce_flg & 1, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80U);
        FUNC(ADD_SPEED_XZ)(0);
        if (G.pEm->Move_cnt == 1)
        {
            FUNC(SND_SE_ENEM)(0, 0U);
        }
        if (G.pEm->Move_cnt == 0x20)
        {
            FUNC(SND_SE_ENEM)(0, 0U);
        }
        if (FUNC(SQUARE_ROOT0)((G.pEm->Pos_x - G.pEm->Root_x) * (G.pEm->Pos_x - G.pEm->Root_x) + (G.pEm->Pos_z - G.pEm->Root_z) * (G.pEm->Pos_z - G.pEm->Root_z)) < 0x96U)
        {
            G.pEm->Routine_3++;
        }
        return;
    case 2:
        FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);
        if (G.pEm->Sca_info & 0x80)
        {
            G.pEm->Routine_3 = 1U;
            return;
        }
        EM_ROUTINE_23 = 0;
        return;
    }
}

VOID Em06_Sce_run(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        G.pEm->Move_no = 2;
        G.pEm->Spd_cur = 0xC8;

    case 1:
        ScratchPad->Vec.vx = G.pEm->Root_x;
        ScratchPad->Vec.vz = G.pEm->Root_z;
        ScratchPad->Vec.vy = 0;
        FUNC(JOINT_MOVE)(G.pEm->Sce_flg & 1, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80U);
        if (G.pEm->Move_cnt == 1)
        {
            FUNC(SND_SE_ENEM)(1, 0U);
        }
        if (G.pEm->Move_cnt == 0xF)
        {
            FUNC(SND_SE_ENEM)(1, 0U);
        }
        FUNC(GOTO00)(&ScratchPad->Vec, 0x60U);
        FUNC(ADD_SPEED_XZ)(0);
        if (FUNC(SQUARE_ROOT0)((G.pEm->Pos_x - G.pEm->Root_x) * (G.pEm->Pos_x - G.pEm->Root_x) + (G.pEm->Pos_z - G.pEm->Root_z) * (G.pEm->Pos_z - G.pEm->Root_z)) < 0x96U)
        {
            G.pEm->Routine_3++;
        }
        return;
    case 2:
        FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);
        if (G.pEm->Sca_info & 0x80)
        {
            G.pEm->Routine_3 = 1U;
            return;
        }
        EM_ROUTINE_23 = 0;
        return;
    }
}

VOID Em06_Sce_dodge(VOID)
{
    Em06_sce_dodge_mv[G.pEm->Routine_3]();
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em06_Sce_dodge30(VOID)
{
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)
    {
        FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);
        EM_ROUTINE_23 = 0;
    }
}

VOID Em06_Sce_decap(VOID)
{
    Em06_sce_decap_mv[G.pEm->Routine_3]();
    if (G.pEm->Move_cnt < 0x17U)
    {
        ScratchPad->Vec.vx = EM06_FREE_P->leap_target_x;
        ScratchPad->Vec.vz = EM06_FREE_P->leap_target_z;
        ScratchPad->Vec.vy = 0;
        G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&ScratchPad->Vec, 0x18U);
        FUNC(ADD_SPEED_XZ)(0);
        return;
    }
    if (G.pEm->Move_cnt < 0x1FU)
    {
        G.pEm->Spd_cur = 0x32;
        FUNC(ADD_SPEED_XZ)(0x800);
        return;
    }
}

VOID Em06_Sce_decap20(VOID)
{
    FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);
    EM_ROUTINE_23 = 0;
}

VOID Em06_Sce_swipe(VOID)
{
    Em06_sce_swipe_mv[G.pEm->Routine_3]();
    Em06_recenter_on_joint(1);
}

VOID Em06_Sce_swipe20(VOID)
{
    PARTS_W* pP_ptr;

    pP_ptr = G.pEm->pSin_parts_ptr;
    FUNC(MODEL_COLOR_PLUS)(&pP_ptr[9], 0x30U, 0x80820, 0x606060);
    FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);
    EM_ROUTINE_23 = 0;
}

VOID Em06_Sce_bite(VOID)
{
    Em06_sce_bite_mv[G.pEm->Routine_3]();
    if (G.pEm->Move_cnt < 0x17U)
    {
        ScratchPad->Vec.vx = EM06_FREE_P->leap_target_x;
        ScratchPad->Vec.vz = EM06_FREE_P->leap_target_z;
        ScratchPad->Vec.vy = 0;
        G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&ScratchPad->Vec, 0x18U);
        FUNC(ADD_SPEED_XZ)(0);
        return;
    }
    if (G.pEm->Move_cnt < 0x1FU)
    {
        G.pEm->Spd_cur = 0x32;
        FUNC(ADD_SPEED_XZ)(0x800);
        return;
    }
}

VOID Em06_Sce_bite00(VOID)
{
    G.pEm->Routine_3 = 1;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 7;
    G.pEm->Spd_cur = 0x12C;
    G.pEm->Move_no = 0x12 - EM06_FREE_P->decap_claw_joint;
    G.pEm->Move_no = 0xC;
    FUNC(SND_SE_ENEM)(2, 0);
}

VOID Em06_Sce_bite10(VOID)
{
    UCHAR frame;
    PARTS_W* pP_ptr;

    frame = G.pEm->Move_cnt + 0xF9;

    pP_ptr = pEm06_victim->pSin_parts_ptr;
    if (frame < 8U && !(pP_ptr[1].Be_flg & 0x40))
    {
        pP_ptr = G.pEm->pSin_parts_ptr;
        pP_ptr = pP_ptr + EM06_FREE_P->decap_claw_joint;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        pEm06_victim->Damage_no = 1;
        (ROUTINE_WORD(pEm06_victim)) = 0x20001;  // cast kept: Routine_0..3 are written as one word
        G.pEm->Routine_3 = 2U;
        G.pEm->Timer0 = 4;
        pP_ptr = G.pEm->pSin_parts_ptr;
        pP_ptr = pP_ptr + EM06_FREE_P->decap_claw_joint;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchPad->Vec.vx = 0xC8;

        FUNC(ESP_CALL)(0, 0, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);

        FUNC(MODEL_COLOR_PLUS)(pP_ptr, 0x30, 0x80820, 0x606060);
        FUNC(SND_SE_ENEM)(5, 0);
        return;
    }
    G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
}

VOID Em06_Sce_bite20(VOID)
{
    PARTS_W* pSub_parts;

    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)
    {
        FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);
        EM_ROUTINE_23 = 7;
    }
    pSub_parts = pEm06_victim->pSin_parts_ptr;
    pSub_parts[1].Be_flg = pSub_parts[1].Be_flg & 0xFE;
    Em06_Sce_catch_set(6);
}

VOID Em06_Sce_catch(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3++;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        G.pEm->Move_no++;
        G.pEm->Spd_cur = 0;

    case 1:
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80) != 0)
        {
            G.pEm->Routine_3++;
            G.pEm->Move_cnt = 0;
            G.pEm->Move_wait = 0;
            G.pEm->Move_no++;
        }
        Em06_Sce_catch_set(6);
        return;
    case 2:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);
        Em06_Sce_catch_set(6);
        return;
    }
}

VOID Em06_Sce_catch_set(SCHAR claw_joint)
{
    PARTS_W* pP_ptr;
    PARTS_W* pSub_parts;

    pP_ptr = G.pEm->pSin_parts_ptr;
    FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);
    FUNC(COMP_MATRIX)(G.pEm->M, &pP_ptr->Coord, &ScratchPad->Mat);
    FUNC(COMP_MATRIX)(&ScratchPad->Mat, &pP_ptr[1].Coord, &ScratchPad->Mat);
    pP_ptr += 6;
    ScratchWord[0] = 3;
    for (;;)
    {
        FUNC(COMP_MATRIX)(&ScratchPad->Mat, &(pP_ptr - ScratchWord[0])[1].Coord, &ScratchPad->Mat);
        if ((ScratchWord[0] = ScratchWord[0] - 1) == 0)
        {
            break;
        }
    }
    pP_ptr->Workm = ScratchPad->Mat;
    pSub_parts = pEm06_victim->pSin_parts_ptr;
    pSub_parts++;
    MATRIX_POS(&pSub_parts->Workm) = MATRIX_POS(&pP_ptr->Workm);
    ScratchPad->Sv.vx = 0xC8;
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Mat = *Monitor.pId_mat;
    FUNC(ROT_MATRIX_Y)(G.pEm->Cdir_y + 0xBB8 - (EM06_FREE_P->decap_claw_joint - 6) * 0x258, &ScratchPad->Mat);
    FUNC(APPLY_MATRIX_SV)(&ScratchPad->Mat, &ScratchPad->Sv, &ScratchPad->Sv);
    pSub_parts->Workm.t[0] = pSub_parts->Workm.t[0] + ScratchPad->Sv.vx;
    pSub_parts->Workm.t[2] = pSub_parts->Workm.t[2] + ScratchPad->Sv.vz;
}

VOID Em06_Sce_stalk(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        G.pEm->Damage_no = 0;
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400) != 0)
        {
            G.pEm->Move_no = 0x10;
        }
        G.pEm->Routine_3 = 1U;
        G.pEm->Timer0 = (Api_rand() & 0x3F) + 0x1E;
    }
    ScratchWord[7] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x64);
    if ((G.pEm->Timer0-- == 0) || ScratchWord[7] == 0)
    {
        FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);
        EM_ROUTINE_23 = 0;
        G.pEm->Dest_x = G.Player.Pos_x;
        G.pEm->Dest_z = G.Player.Pos_z;
    }
    else
    {
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);
        G.pEm->Cdir_y += ScratchHalf[14];
    }
}

static AT_TBL Em06_sca00 = { 0x8000, 0, -180, 0, 0xB4, 0x384 };  // collision volume: flags 0x8000, y offset -180, half height 180, radius 900

static AT_TBL Em06_sca01 = { 0x8001, 0, 0, 0, 0, 0 };  // collision volume: flags 0x8001, no offset or extent

static AT_TBL Em06_sca02 = { 0x8000, 0, -180, 0, 0xB4, 0x1F4 };  // collision volume used from stage 5 onwards or with Type bit 5: radius 500

AT_TBL* pEm06_sca = &Em06_sca00;  // collision volume of the Hunter
static AT_TBL* Em06_sca_adr01 = &Em06_sca01;  // address of the second collision volume; never read
AT_TBL* Em06_sca_late = &Em06_sca02;  // collision volume used from stage 5 onwards or with Type bit 5

// starting health by random index 0 to 15
UCHAR Em06_life[16] =
{
    0x5F,   // 0 //
    0x5F,   // 1 //
    0x5F,   // 2 //
    0x5F,   // 3 //
    0x4F,   // 4 //
    0x5F,   // 5 //
    0x4F,   // 6 //
    0x6F,   // 7 //
    0x5F,   // 8 //
    0x5F,   // 9 //
    0x4F,   // 10 //
    0x5F,   // 11 //
    0x5F,   // 12 //
    0x4F,   // 13 //
    0x5F,   // 14 //
    0x5F,   // 15 //
};

// decapitation chance flags by random index 0 to 15, used while the Hunter health is below 0x4F
UCHAR Em06_decap_chance_mid[16] =
{
    0x01,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x01,   // 3 //
    0x00,   // 4 //
    0x00,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x01,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x00,   // 12 //
    0x00,   // 13 //
    0x00,   // 14 //
    0x01,   // 15 //
};

// decapitation chance flags by random index 0 to 15, used while the Hunter health is below 0x28; the upper half is never indexed
UCHAR Em06_decap_chance_low[32] =
{
    0x00,   // 0 //
    0x01,   // 1 //
    0x00,   // 2 //
    0x01,   // 3 //
    0x00,   // 4 //
    0x01,   // 5 //
    0x00,   // 6 //
    0x01,   // 7 //
    0x01,   // 8 //
    0x00,   // 9 //
    0x01,   // 10 //
    0x00,   // 11 //
    0x01,   // 12 //
    0x00,   // 13 //
    0x00,   // 14 //
    0x01,   // 15 //
    0x00,   // 16 //
    0x00,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
    0x00,   // 20 //
    0x00,   // 21 //
    0x00,   // 22 //
    0x00,   // 23 //
    0x00,   // 24 //
    0x00,   // 25 //
    0x00,   // 26 //
    0x00,   // 27 //
    0x00,   // 28 //
    0x00,   // 29 //
    0x00,   // 30 //
    0x00,   // 31 //
};

// player health below which a decapitation may start, by player id & 1; padded to 4 bytes
UCHAR Em06_pl_life_lim[4] =
{
    0x62,   // 0 //
    0x43,   // 1 //
    0x00,   // 2 //
    0x00,   // 3 //
};

// state handlers by entity state, followed by the base Routine_2 bytes by (Damage_no & 7) - 1
EM06_STATE_TBL Em06_routine_0 =
{
    {
        Em06_Init,           // 0 //
        Em06_Move,            // 1 //
        Em06_Damage,    // 2 //
        Em06_Die,       // 3 //
        Em06_Dead,           // 4 //
        0,                           // 5 //
        0,                           // 6 //
        0,                           // 7 //
        Em06_Scenario,   // 8 //
    },
    {
        0x00,   // 0 //
        0x02,   // 1 //
        0x00,   // 2 //
        0x02,   // 3 //
        0x00,   // 4 //
        0x00,   // 5 //
        0x00,   // 6 //
        0x00,   // 7 //
    },
};

// act handlers by Routine_2 in state 2
VOID (*Em06_dm[9])(VOID) =
{
    Em06_act_swipe,        // 0 //
    Em06_act_swipe,        // 1 //
    Em06_act_pounce,       // 2 //
    Em06_act_swipe,        // 3 //
    Em06_act_leapattack,   // 4 //
    Em06_act_flurry,       // 5 //
    Em06_act_holdplayer,   // 6 //
    0,                       // 7 //
    Em06_act_pounce,       // 8 //
};

// death actions by Routine_2 in state 3
VOID (*Em06_die[7])(VOID) =
{
    Em06_Die_fall,       // 0 //
    Em06_Die_fall,       // 1 //
    Em06_Die_thrash,     // 2 //
    Em06_Die_fall,       // 3 //
    Em06_Die_pounce,     // 4 //
    Em06_Die_settle,     // 5 //
    Em06_Die_collapse,   // 6 //
};

// behaviors by Routine_2 in state 1
VOID (*Em06_mv[12])(VOID) =
{
    Em06_Move_idle,   // 0 //
    Em06_Move_chase,           // 1 //
    Em06_Move_yori,        // 2 //
    Em06_Move_nop,             // 3 //
    Em06_Move_atck,          // 4 //
    Em06_Move_decap,      // 5 //
    Em06_Move_dodge,           // 6 //
    Em06_Move_catch,        // 7 //
    Em06_Move_jump,       // 8 //
    Em06_Move_ikaku,          // 9 //
    Em06_Move_back,        // 10 //
    Em06_Move_tojo,           // 11 //
};

// variant AI handlers by Type & 0xF; entries 8 to 15 are not handlers: data words and zeros that are never dispatched
VOID (*Em06_Move_type[16])(VOID) =
{
    Em06_Move_type00,          // 0 //
    Em06_Move_type01,          // 1 //
    Em06_Move_type02,          // 2 //
    0,                            // 3 //
    Em06_Move_type04,          // 4 //
    Em06_Move_type04,          // 5 //
    Em06_Move_type04,          // 6 //
    Em06_Move_type02,          // 7 //
    (VOID (*)(VOID))0x02020102,   // 8 //
    (VOID (*)(VOID))0x05020102,   // 9 //
    (VOID (*)(VOID))0x0B050B0B,   // 10 //
    (VOID (*)(VOID))0x04040503,   // 11 //
    0,                            // 12 //
    0,                            // 13 //
    0,                            // 14 //
    0,                            // 15 //
};

// idle actions by Type & 0xF
VOID (*Em06_idle_variant_tbl[8])(VOID) =
{
    Em06_idle_stand,   // 0 //
    Em06_idle_walk,    // 1 //
    Em06_idle_stand,   // 2 //
    0,                   // 3 //
    Em06_idle_walk,    // 4 //
    Em06_idle_walk,    // 5 //
    Em06_idle_walk,    // 6 //
    Em06_idle_stand,   // 7 //
};

// attack steps by Act_sts
VOID (*Em06_attack_sub_tbl[5])(VOID) =
{
    Em06_atk_start,     // 0 //
    Em06_atk_swing,     // 1 //
    Em06_atk_lunge,     // 2 //
    Em06_atk_recover,   // 3 //
    Em06_atk_finish,    // 4 //
};

// decapitation leap steps by Act_sts
VOID (*Em06_decap_leap_sub_tbl[5])(VOID) =
{
    Em06_decap_leap_start,     // 0 //
    Em06_decap_strike,         // 1 //
    Em06_decap_leap_end,       // 2 //
    Em06_decap_leap_track,     // 3 //
    Em06_decap_leap_to_hold,   // 4 //
};

// dodge swipe reach: hit window, claw joint, test point and radius
EM06_REACH_REC Em06_swipe_reach_tbl[3] =
{
    { 0x07, 0, 0x0004, 0x000C, 0x05DC, 0x0258 },   // 0 //
    { 0x12, 0, 0x0002, 0x000F, 0x03E8, 0x0258 },   // 1 //
    { 0x14, 0, 0x0005, 0x0009, 0x0000, 0x05DC },   // 2 //
};

// dodge steps by Act_sts
VOID (*Em06_dodge_sub_tbl[5])(VOID) =
{
    Em06_dodge_start,   // 0 //
    Em06_dodge_hop,     // 1 //
    Em06_dodge_swipe,   // 2 //
    Em06_dodge_end,     // 3 //
    Em06_dodge_land,    // 4 //
};

// intro tables: entry 0 animation ids, entry 1 facing change, entries 2 to 5 jump displacement per tick by jump kind
EM06_INTRO_REC Em06_intro_move_tbl[6] =
{
    { 0x0018, 0x0019 },    // 0 //
    { -0x0400, 0x0400 },   // 1 //
    { 0x0000, -0x01E6 },   // 2 //
    { -0x018C, 0x0000 },   // 3 //
    { 0x0113, 0x0000 },    // 4 //
    { -0x0180, 0x0000 },   // 5 //
};

UCHAR Em06_jump_kind = 0;       // intro jump kind of the room, 2 to 5
ENEMY_WORK* pEm06_victim = 0;     // victim entity of the scripted bite

// scripted behaviors by Routine_2
VOID (*Em06_sce_mv[24])(VOID) =
{
    Em06_Sce_idle,             // 0 //
    0,                           // 1 //
    Em06_Sce_walk,             // 2 //
    Em06_Sce_run,              // 3 //
    0,                           // 4 //
    0,                           // 5 //
    0,                           // 6 //
    Em06_Sce_catch,            // 7 //
    0,                           // 8 //
    0,                           // 9 //
    Em06_Sce_dodge,        // 10 //
    Em06_Sce_decap,   // 11 //
    Em06_Sce_bite,       // 12 //
    Em06_Sce_swipe,        // 13 //
    Em06_act_swipe,            // 14 //
    Em06_act_leapattack,       // 15 //
    Em06_act_pounce,           // 16 //
    Em06_act_flurry,           // 17 //
    Em06_Move_ikaku,      // 18 //
    Em06_Die_fall,           // 19 //
    Em06_Die_collapse,       // 20 //
    Em06_Die_thrash,         // 21 //
    Em06_Sce_stalk,     // 22 //
    Em06_Move_jump,   // 23 //
};

// scripted dodge steps by Act_sts
VOID (*Em06_sce_dodge_mv[4])(VOID) =
{
    Em06_dodge_start,        // 0 //
    Em06_dodge_hop,          // 1 //
    Em06_dodge_swipe,        // 2 //
    Em06_Sce_dodge30,   // 3 //
};

// scripted decapitation leap steps by Act_sts
VOID (*Em06_sce_decap_mv[3])(VOID) =
{
    Em06_decap_leap_start,   // 0 //
    Em06_decap_strike,       // 1 //
    Em06_Sce_decap20,   // 2 //
};

// scripted swipe steps by Act_sts
VOID (*Em06_sce_swipe_mv[3])(VOID) =
{
    Em06_atk_start,          // 0 //
    Em06_atk_swing,          // 1 //
    Em06_Sce_swipe20,   // 2 //
};

// scripted bite steps by Act_sts
VOID (*Em06_sce_bite_mv[3])(VOID) =
{
    Em06_Sce_bite00,   // 0 //
    Em06_Sce_bite10,   // 1 //
    Em06_Sce_bite20,      // 2 //
};
