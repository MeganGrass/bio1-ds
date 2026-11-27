/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM02.C
 */

#include "EM02.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Em02_Main(VOID)
{
    INT old_x;
    INT old_z;
    INT unused[4];  // unused local: reserves stack space

    old_x = G.pEm->Pos_x;
    old_z = G.pEm->Pos_z;
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        Em02_routine_0[G.pEm->Routine_0]();
        FUNC(OBA_SET_OFS)(G.pEm);  // Oba_set_ofs
        FUNC(OBA_CK_HIT)(&MAIN.Player.Be_flg, G.pEm);  // Oba_ck_hit
        FUNC(OBA_CK_EM)();  // Oba_ck_em
        EM02_EF(Ck_flg) = (EM02_EF(Ck_flg) << 1) & 0xE;
        if (!(G.pEm->Be_flg & EM_BE_NO_WALL))
        {
            EM02_EF(Ck_flg) |= (SCHAR)Em02_sca_ck();  // sign-extends the byte
        }
        else
        {
            G.pEm->Old_x = G.pEm->Pos_x;
            G.pEm->Old_y = G.pEm->Pos_y;
            G.pEm->Old_z = G.pEm->Pos_z;
        }
    }
    if (!(EM02_EF(Ck_flg) & 1) && G.pEm->Pos_x == old_x && G.pEm->Pos_z == old_z)
    {
        EM02_EF(Ck_flg) |= 0x10;
    }
    G.pEm->Coord_flg = 0;
    G.pEm->Zone_in_flg = FUNC(HIT_CK_POINT4)(&G.pEm->Pos_x, G.Cccut);  // Hit_ck_point4
    if (G.pEm->Zone_in_flg != 0)
    {
        FUNC(MUL_KAGE)(&G.pEm->Pos_x, &G.pEm->Kage_work, 0, G.pEm->Cdir_y);  // MulKage
    }
}

VOID Em02_Init(VOID)
{
    EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);  // state 1, Key_rno 0, Routine_2 0, Act_sts 0
    Api_rand();  // rand

    G.pEm->Life = Em02_life[Api_rand() & 0xF] + (Api_rand() & 3);  // rand

    G.pEm->Damage_no = 0;
    EM02_EF(Ck_flg) = 0;
    EM02_EF(Move_flg) = 1;
    EM02_EF(Ai_flg) = 0x80;
    G.pEm->pAt_tbl = &Em02_sca;
    G.pEm->Be_flg &= EM_BE_STATE_MASK;
    G.pEm->Move_no = 0;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    FUNC(INIT_PARTS_WORK)();  // Init_Parts_work
    G.pEm->Timer0 = 0;
    EM02_EF(Sence_cnt) = 0;
    EM02_EF(Step_cnt) = 0;
    EM02_EF(Yoko_ofs) = 0;

    EM02_EF(Tgt_timer) = 0;

    G.pEm->Spd_cur = 0;
    EM02_EF(Path_ck) = 0;
    EM02_EF(Sence_flg) = 0;
    ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
    ScratchPad->Vec.vx = Em02_sca.Off_x;
    FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, &ScratchPad->Mat);  // RotMatrix
    FUNC(APPLY_MATRIX_LV)(&ScratchPad->Mat, &ScratchPad->Vec, &ScratchPad->Vec);  // ApplyMatrixLV
    G.pEm->Old_x += ScratchPad->Vec.vx;
    G.pEm->Old_z += ScratchPad->Vec.vz;
    Em02_bark_flg = 0;
    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = -0x50;
    ScratchWord[7] = 0x808080;
    FUNC(KAGE_WORK_INIT)(&ScratchPad->Sv, &G.pEm->Kage_work, 0x550, 0x200);  // Kage_work_init
    switch (G.pEm->Type)
    {
    case 6:
    case 7:
    case 0x80:
        G.pEm->Be_flg |= EM_BE_NO_WALL;
        break;
    default:
        G.pEm->Pos_y = 0;
        break;
    }
}

VOID Em02_Move(VOID)
{
    INT unused;  // unused local: reserves stack space

    if (G.pEm->Type == 0x80)
    {
        G.pEm->Routine_1 = 0U;
    }
    EM02_EF(player_dist) = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);
    EM02_EF(Path_ck) = FUNC(EM_SIKAI_PROC)();  // Em_sikai_proc
    G.pEm->Be_flg &= EM_BE_STATE_MASK;
    G.pEm->Be_flg |= 0x40;
    if (EM02_EF(Sence_flg) == 0)
    {
        FUNC(EM_CK_HANI)(0x1388);  // Em_ck_hani
    }
    if (G.pEm->Routine_1 == 0)
    {
        Em02_Move_set();
        EM_ROUTINE_HALF2 = 0;
    }
    Em02_mv[G.pEm->Routine_1]();
}

VOID Em02_Move_set(VOID)
{
    EM02_EF(Sence_flg) = 0;
    G.pEm->Move_wait = 0;
    if (EM02_EF(Ai_flg) & 0x80)
    {
        G.pEm->Move_cnt = 0;
    }
    if (G.pEm->Type == 0x80)
    {
        Em02_mv08_set();
        return;
    }
    Em02_mv_set_tbl[G.pEm->Type]();
}

VOID Em02_mv08_set(VOID)
{
    G.pEm->Routine_1 = 8;
    G.pEm->Move_no = 0;
    G.pEm->Hokan_flg = 0xF;
}

VOID Em02_mv01_set(VOID)
{
    EM02_EF(Move_flg) = 4;

    if (G.pEm->Type == 3)
    {
        Em02_mv09_set();

        return;
    }

    G.pEm->Routine_1 = 1;
    G.pEm->Move_no = Em02_walk_tbl[(G.pEm->Type >> 1) & 0xFF].anim_id;

    G.pEm->Move_cnt = Api_rand() & 0x1F;  // rand

    G.pEm->Spd_cur = Em02_walk_tbl[(G.pEm->Type >> 1) & 0xFF].Move_spd;
    EM02_EF(turn_step) = (((G.pEm->Type >> 1) & 0xFF) + 1) * 0x10;

    EM02_EF(Move_flg) = G.pEm->Type & 1;

    EM02_EF(Yoko_ofs) = (Api_rand() + 0x1E) & 0xF;  // rand

    G.pEm->Hokan_flg = 0xF;
}

VOID Em02_mv02_set(VOID)
{
    if (G.pEm->Hokan_flg >= 8U)
    {
        G.pEm->Hokan_flg = 7U;
    }

    EM02_EF(turn_step) = (Api_rand() & 7) + 0x3D;  // rand
    EM02_EF(Sence_cnt) = 0;
    EM02_EF(Step_cnt) = 0;
    EM02_EF(Yoko_ofs) = 0;
    EM02_EF(ballistic_gravity) = 0;
    if (EM02_EF(Ai_flg) & 1)
    {
        EM02_EF(Yoko_ofs) = (Api_rand() & 0xF) + 0x14;  // rand
    }
    G.pEm->Routine_1 = 2;
    G.pEm->Dest_x = G.Player.Pos_x;
    G.pEm->Dest_z = G.Player.Pos_z;
    EM02_EF(Ai_flg) = (EM02_EF(Ai_flg) & 0xFFF9) | 0x80;
}

VOID Em02_mv03_set(VOID)
{
    G.pEm->Timer2 = 0;
    switch (G.pEm->Type)
    {
    case 5:
        if (G.pEm->Move_no == 0x10)
        {
            G.pEm->Cdir_y -= 0x20;
        }
        else if (G.pEm->Move_no == 0xF)
        {
            G.pEm->Cdir_y += 0x20;
        }
        G.pEm->Move_no = 6U;
        G.pEm->Spd_cur = 0x41;
        EM02_EF(Leap_spd_y) = 0x12C;
        EM02_EF(ballistic_gravity) = -0x28;
        break;
    case 6:
        G.pEm->Be_flg |= EM_BE_NO_HIT | EM_BE_NO_WALL;
        G.pEm->Spd_cur = 0x41;
        EM02_EF(Leap_spd_y) = 0x1E0;
        EM02_EF(ballistic_gravity) = -0x28;
        G.pEm->Move_no = 6U;

        EM02_EF(Move_flg) = 5;

        G.pEm->Damage_no = 1;
        break;
    case 7:
        G.pEm->Be_flg |= EM_BE_NO_WALL;
        G.pEm->Move_no = 7U;
        G.pEm->Timer2 = 0xA;
        G.pEm->Spd_cur = 0x118;
        EM02_EF(Leap_spd_y) = 0x190;
        EM02_EF(ballistic_gravity) = -0x28;

        EM02_EF(Move_flg) = 5;

        G.pEm->Damage_no = 1;
        break;
    }
    G.pEm->Routine_1 = 3;
    G.pEm->Hokan_flg = 3;
    G.pEm->Timer0 = 8;
    EM02_EF(Sence_cnt) = 0;
    EM02_EF(Blood_cnt) = 0;
}

VOID Em02_mv07_set(VOID)
{
    G.pEm->Routine_1 = 7;
    G.pEm->Move_no = 4;
    G.pEm->Hokan_flg = 0;
    G.pEm->Damage_no = 1;
}

VOID Em02_mv04_set(VOID)
{
    G.pEm->Routine_1 = 4;
    if (EM02_EF(turn_delta) == 0)
    {
        EM02_EF(turn_delta) = Api_rand() & 1 ? 0x80 : -0x80;  // rand
    }
    G.pEm->Timer0 = 8;
    EM02_EF(Sence_cnt) = 0;
    EM02_EF(Ai_flg) |= 0x80;
}

VOID Em02_mv05_set(VOID)
{
    G.pEm->Routine_1 = 5;
    G.pEm->Move_no = 0;
    G.pEm->Hokan_flg = 0xF;
    G.pEm->Move_cnt = 0;
    G.pEm->Timer0 = 0x14;
    EM02_EF(turn_delta) = 0;
    EM02_EF(Sence_cnt) = 0;
    G.pEm->Damage_no = 1;
}

VOID Em02_mv09_set(VOID)
{
    G.pEm->Routine_1 = 9;
    G.pEm->Move_no = 1;

    G.pEm->Move_cnt = Api_rand() & 0x1F;  // rand

    G.pEm->Hokan_flg = 0;
    EM02_EF(Sence_cnt) = 0;
    G.pEm->Spd_cur = 0;
    if (!(EM02_EF(Move_flg) & 4))
    {
        EM02_EF(Move_flg) |= 2;
    }
    G.pEm->Damage_no = 0;
}

VOID Em02_mv02(VOID)
{
    INT unused;  // unused local: reserves stack space
    if (G.pEm->Routine_2 < 2U)
    {
        FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
    }
    Em02_mv02_tbl[G.pEm->Routine_2]();
    Em02_at_ck();
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)  // Joint_move
    {
        FUNC(SND_SE_ENEM)(0, 0);  // Snd_se_enem
    }
}

VOID Em02_mv02_run(VOID)
{
    SHORT turn;
    SHORT unused;  // unused local: reserves stack space

    Rate_speed_up(0x14, 0xF0);
    ScratchPad->Vec.vx = G.pEm->Dest_x;
    ScratchPad->Vec.vy = 0;
    ScratchPad->Vec.vz = G.pEm->Dest_z;

    if (EM02_EF(Tgt_timer) == 0 && EM02_EF(Yoko_ofs) < 0xA && G.pEm->Spd_cur >= 0xF0)
    {
        turn = FUNC(GOTO00_CK)(&ScratchPad->Vec, (SHORT)EM02_EF(turn_step));  // Goto00_ck
        FUNC(GOTO00)(&ScratchPad->Vec, (SHORT)EM02_EF(turn_step));  // Goto00
    }
    else
    {
        if (EM02_EF(Ck_flg) & 0x11)
        {
            EM02_EF(ballistic_gravity) = FUNC(GOTO00_CK)(&ScratchPad->Vec, ((SHORT)EM02_EF(turn_step)) + (EM02_EF(Ai_flg) & 1 ? 0 : 0x20));  // Goto00_ck
            EM02_EF(Ai_flg) &= 0xFFFE;
        }
        turn = EM02_EF(ballistic_gravity);
        G.pEm->Cdir_y += turn;
        if (--EM02_EF(Yoko_ofs) < 0xB)
        {
            EM02_EF(Yoko_ofs) = 5;
            EM02_EF(Ai_flg) &= 0xFFFE;
        }
    }
    if (!(EM02_EF(Path_ck) & 2))
    {
        if (!(EM02_EF(Path_ck) & 1))
        {
            if (++EM02_EF(Step_cnt) == 6)
            {
                G.pEm->Routine_1 = 0;
                G.pEm->Type = 0xC;
                EM02_EF(Move_flg) |= 4;
                return;
            }
        }
        else
        {
            EM02_EF(Step_cnt) = 0U;
            G.pEm->Spd_cur = 0xF0;
            G.pEm->Dest_x = G.Player.Pos_x;
            G.pEm->Dest_z = G.Player.Pos_z;
        }
    }
    Set_run_motion(turn);

    if (!(EM02_EF(Move_flg) & 2))
    {
        if (EM02_EF(Ck_flg) & 0x11)
        {
            EM02_EF(Sence_cnt) += 2;
            if ((SHORT)EM02_EF(Sence_cnt) >= 0x10)  // sign-extends the halfword
            {
                G.pEm->Routine_1 = 0;
                G.pEm->Type = 9;
                EM02_EF(turn_delta) = turn << 1;
                EM02_EF(Ai_flg) &= 0xFF7F;
            }
        }
        else
        {
            EM02_EF(Sence_cnt) = (SHORT)EM02_EF(Sence_cnt) - ((SHORT)EM02_EF(Sence_cnt) >= 1);  // sign-extends the halfword
        }
    }
    else
    {
        if (Em02_pl_turn_ck(4) != 0 && EM02_EF(Tgt_timer) == 0)
        {
            EM_ROUTINE_HALF2 = 1;
        }
        EM02_EF(Tgt_timer) = EM02_EF(Tgt_timer) - (EM02_EF(Tgt_timer) >= 1);
    }
    if (EM02_EF(Ai_flg) & 0xC)
    {
        EM_ROUTINE_HALF2 = 2;
    }
}

VOID Em02_mv02_turn(VOID)
{
    SHORT dir;

    Rate_speed_up(0x14, 0xF0);
    dir = Em02_wall_step(0x5A, 0x40);
    if (dir != 3)
    {
        Set_run_motion(dir - 1);
    }
    if (EM02_EF(Ai_flg) & 0x10)
    {
        G.pEm->Routine_1 = 0;
        G.pEm->Routine_2 = 4;
        EM02_EF(turn_step) = (Api_rand() & 7) + 0x3D;  // rand
        EM02_EF(Ai_flg) &= 0xFF7F;

        EM02_EF(Tgt_timer) = (Api_rand() & 3) * 2 + 8;  // rand
    }
}

VOID Em20_Mv_run20(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3++;
        G.pEm->Timer0 = 0;
    }
    ScratchPad->Vec.vx = G.pEm->Dest_x;
    ScratchPad->Vec.vy = 0;
    ScratchPad->Vec.vz = G.pEm->Dest_z;
    EM02_EF(turn_delta) = FUNC(GOTO00_CK)(&ScratchPad->Vec, 0x60);  // Goto00_ck
    FUNC(ADD_SPEED_XZ)(-G.pEm->Timer0);  // Add_speedXZ
    if (G.pEm->Routine_3 == 1)
    {
        Rate_speed_down(0xE, 0xA);
        if (G.pEm->Spd_cur < 0xB)
        {
            G.pEm->Routine_3++;
        }
    }
    else
    {
        Rate_speed_up(0xE, 0xF0);
    }
    G.pEm->Cdir_y += EM02_EF(turn_delta);
    G.pEm->Timer0 += EM02_EF(turn_delta);
    Set_run_motion(EM02_EF(turn_delta));
    if (!(__builtin_abs(G.pEm->Timer0) < 0x800) || EM02_EF(turn_delta) == 0)
    {
        G.pEm->Routine_2 = 0;
        EM02_EF(Ai_flg) &= 0xFFF3;
    }
}

VOID Em02_mv03(VOID)
{
    Em02_mv03_tbl[G.pEm->Routine_2]();
    Set_blood(G.pEm, 4, 0);
}

VOID Em02_mv03_crouch(VOID)
{
    FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
    if ((--G.pEm->Timer0 != 0) && G.pEm->Timer2 == 0)
    {
    }
    else
    {
        G.pEm->Routine_2++;
        G.pEm->Spd_cur = 0x118;
        EM02_EF(Sence_flg) = 1;
        if (G.pEm->Type != 7)
        {
            FUNC(SND_SE_ENEM)(6, 0);  // Snd_se_enem
        }
    }
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
}

VOID Em02_mv03_air(VOID)
{
    PARTS_W* pP_ptr;
    INT vel_y;
    SHORT reach;

    G.pEm->Be_flg |= 0x40;
    FUNC(EM_FALL_MOVE)(G.pEm->Spd_cur, EM02_EF(Leap_spd_y), EM02_EF(ballistic_gravity), 0);  // Em_fall_move
    vel_y = EM02_EF(Leap_spd_y) + EM02_EF(ballistic_gravity) * (G.pEm->Timer2 + 1);
    pP_ptr = G.pEm->pSin_parts_ptr;
    pP_ptr += 4;
    ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

    reach = G.Player.Life >= 0x11 ? 0x3E8 : 0x320;

    if (G.Player.Sca_info & 8)
    {
        reach = 0x5DC;
    }
    if ((*(SHORT*)&EM02_EF(Sence_cnt)) == 0)  // SHORT* view of the field (a plain access changes the code)
    {
        EM02_EF(Sence_cnt) = FUNC(L_POS_CK_XZM)(&pP_ptr->Workm, &ScratchPad->Vec, reach, &MAIN.Player.Pos_x);  // L_pos_ckXZM
        if ((*(SHORT*)&EM02_EF(Sence_cnt)) != 0 && G.Player.Damage_no == 0 && G.pEm->Pos_y >= -0x707 && (Em02_pl_bite() & 0xFF) == 1)  // SHORT* view of the field (a plain access changes the code)
        {
            if (G.pEm->Move_cnt != 0)
            {
                FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x100);  // Joint_move
            }
            return;
        }
    }
    if (vel_y <= 0)
    {
        if (((SHORT)EM02_EF(Sence_cnt)) == 2)  // sign-extends the halfword
        {
            G.pEm->Routine_1 = 6;
            G.pEm->Routine_2 = 0U;
            G.pEm->Be_flg |= EM_BE_NO_HIT;
            G.Player.Be_flg |= EM_BE_NO_HIT;
        }
        else
        {
            G.pEm->Routine_2++;
            if (G.pEm->Move_no == 6)
            {
                G.pEm->Move_no++;
                G.pEm->Move_wait = 0;
                G.pEm->Hokan_flg = 0xF;
            }
        }
    }
    if (G.pEm->Move_cnt != 0)
    {
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x100);  // Joint_move
    }
}

VOID Em02_mv03_land(VOID)
{
    G.pEm->Be_flg |= 0x40;
    FUNC(EM_FALL_MOVE)(G.pEm->Spd_cur, EM02_EF(Leap_spd_y), EM02_EF(ballistic_gravity), 0);  // Em_fall_move
    if (G.pEm->Pos_y >= 0)
    {
        G.pEm->Routine_2++;
        G.pEm->Pos_y = 0;
        G.pEm->Be_flg = (G.pEm->Be_flg & 0xF9) | 0x20;
        FUNC(SND_SE_ENEM)(5, 0);  // Snd_se_enem
    }
    if (G.pEm->Move_cnt != 9)
    {
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x100);  // Joint_move
    }
}

VOID Em02_mv03_rec(VOID)
{
    FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
    Rate_speed_down(0x40, 0xF0);
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x100) != 0)  // Joint_move
    {
        G.pEm->Be_flg &= ~EM_BE_NO_WALL;
        G.pEm->Routine_1 = 0;
        G.pEm->Damage_no = 0;
        EM02_EF(Ai_flg) &= 0xFFFE;

        if (!(EM02_EF(Ck_flg) & 0x11) || G.pEm->Type == 6 || G.pEm->Type == 7 || (EM02_EF(Move_flg) & 2))
        {
            G.pEm->Type = 4U;
        }
        else
        {
            G.pEm->Type = 9U;
            if ((EM02_EF(turn_delta) = FUNC(GOTO00_CK)(&ScratchPad->Vec, (SHORT)EM02_EF(turn_step)) * 2) == 0)  // Goto00_ck
            {
                EM02_EF(turn_delta) = Em02_turn2_ck(1, -1) * (((SHORT)EM02_EF(turn_step)) << 1);  // sign-extends the halfword
            }
        }
    }
}

VOID Em02_mv03_tumble(VOID)
{
    if (G.pEm->Move_cnt < 0x10U)
    {
        FUNC(ADD_SPEED_XZ)(0x800);  // Add_speedXZ
    }
    if (EM02_EF(Yoko_ofs) < 0x800)
    {
        G.pEm->Cdir_y += 0x80;
        EM02_EF(Yoko_ofs) += 0x80;
    }
    FUNC(EM_FALL_MOVE)(0, EM02_EF(Leap_spd_y), EM02_EF(ballistic_gravity), 0);  // Em_fall_move
    if (G.pEm->Pos_y >= 0 && EM02_EF(Step_cnt) == 0)
    {
        FUNC(SND_SE_ENEM)(5, 0);  // Snd_se_enem
        EM02_EF(Step_cnt) = 1;
    }
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x100) != 0)  // Joint_move
    {
        G.pEm->Type = 4;
        G.pEm->Routine_1 = 0;
        G.pEm->Spd_cur = 0;
        EM02_EF(Ai_flg) |= 1;
    }
}

VOID Em02_mv04(VOID)
{
    G.pEm->Cdir_y += EM02_EF(turn_delta);
    FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
    Em02_mv04_tbl[G.pEm->Routine_2]();
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)  // Joint_move
    {
        FUNC(SND_SE_ENEM)(0, 0);  // Snd_se_enem
    }
}

VOID Em02_mv04_hold(VOID)
{
    if (G.pEm->Timer0 > 0)
    {
        G.pEm->Timer0--;
    }
    else
    {
        if (!(EM02_EF(Ck_flg) & 0x11))
        {
            G.pEm->Routine_1 = 0;
            G.pEm->Type = 4;
            EM02_EF(Ai_flg) &= 0xFF7F;

            if (!(EM02_EF(Move_flg) & 1))
            {
                EM02_EF(Yoko_ofs) = 0x28;
            }
        }
        else if ((((SHORT)EM02_EF(Sence_cnt))++ >= 8) && EM02_EF(Step_cnt) == 0)  // sign-extends the halfword
        {
            G.pEm->Routine_2++;
            EM02_EF(turn_delta) = -EM02_EF(turn_delta);
            EM02_EF(Sence_cnt) = 0U;
            return;
        }
        Em02_at_ck();
    }
    Set_run_motion(EM02_EF(turn_delta));
}

VOID Em02_mv04_acc(VOID)
{
    if (EM02_EF(Step_cnt) != 0)
    {
        Rate_speed_up(0x32, 0x82);
    }

    if (!(EM02_EF(Ck_flg) & 0x11))
    {
        G.pEm->Routine_1 = 0;
        G.pEm->Type = 4;
        EM02_EF(Ai_flg) &= 0xFF7F;

        if (!(EM02_EF(Move_flg) & 1))
        {
            EM02_EF(Yoko_ofs) = 0x28;
        }
    }
    else if (((SHORT)EM02_EF(Sence_cnt))++ >= 8)  // sign-extends the halfword
    {
        G.pEm->Routine_2++;
        G.pEm->Spd_cur = -0x82;
        G.pEm->Move_no = 0xA;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        EM02_EF(Sence_cnt) = 0U;
        return;
    }
    Set_run_motion(EM02_EF(turn_delta));
}

VOID Em02_mv04_spring(VOID)
{
    if (G.pEm->Move_cnt < 0xFU)
    {
        if (G.pEm->Move_cnt >= 8U)
        {
            Rate_speed_down(0x20, 0x28);
        }
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x40);  // Goto00
    }
    else
    {
        G.pEm->Spd_cur = 0;
        EM02_EF(turn_delta) = 0;
    }
    if (G.pEm->Move_cnt >= 0x13U)
    {
        G.pEm->Routine_1 = 0;
        G.pEm->Type = 4;
    }
}

VOID Em02_mv05(VOID)
{
    UCHAR frame;

    frame = G.pEm->Move_cnt;
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);  // Joint_move
    G.pEm->Move_cnt = frame;
    if (G.pEm->Routine_2 == 0)
    {
        Em02_mv05_wait();
        return;
    }
    Em02_mv05_track();
}

VOID Em02_mv05_wait(VOID)
{
    INT unused;  // unused local: reserves stack space

    if (EM02_EF(player_dist) >= 0x1964 && PL_ROUTINE_WORD != ((0) << 24 | (0x14) << 16 | (3) << 8 | (1)) && G.pEm->Type != 0xB)  // state 1, Key_rno 3, Routine_2 0x14, Act_sts 0
    {
        return;
    }
    G.pEm->Routine_2++;
    EM02_EF(Yoko_ofs) = 0;
    G.pEm->Move_no = 1;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0x1F;
    G.pEm->Timer0 = (Api_rand() & 0xF) + 0x20;  // rand
}

VOID Em02_mv05_track(VOID)
{
    Em02_neck_rot();
    if (--G.pEm->Timer0 == 0)
    {
        G.pEm->Routine_1 = 0;
        G.pEm->Type = 0xC;
    }
}

VOID Em02_mv06(VOID)
{
    if (G.pEm->Routine_2 >= 2U)
    {
        return;
    }
    if (G.pEm->Routine_2 == 0)
    {
        Em02_mv06_begin();
        return;
    }
    Em02_mv06_hold();
}

VOID Em02_mv06_begin(VOID)
{
    G.pEm->Routine_2++;
    G.pEm->Move_no = 8;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0;
    EM02_EF(Blood_cnt) = 0;
    G.Player.Be_flg |= EM_BE_NO_HIT | EM_BE_NO_WALL;
    G.Player.Cdir_y = G.pEm->Cdir_y;
    FUNC(SET_BASE_POS)(&MAIN.Player);  // Set_base_pos
    EM02_EF(Yoko_ofs) = 0;
    G.Player.Damage_no = 1;
    PL_ROUTINE_WORD = ROUTINES(0, 0, 2, 7);  // state 7, Key_rno 2, Routine_2 0, Act_sts 0
    G.pEm->Spd_cur = 0;

    FUNC(PAD_VIB_FADE_SET)(0x14, 0x78, 0x78, 0);  // Pad_vib_fade_set
    FUNC(PAD_VIB_FADE_SET)(0x2D, 0xFA, 0x64, 0x28);  // Pad_vib_fade_set
}

VOID Em02_mv06_hold(VOID)
{
    PARTS_W* pPl_parts;
    PARTS_W* pP_ptr;
    UCHAR frame;

    G.pEm->Pos_y = 0;
    if (G.pEm->Move_cnt < 0x14U)
    {
        FUNC(SET_BASE_POS)(&MAIN.Player.Be_flg);  // Set_base_pos
    }
    FUNC(NULL_POS_SET)(G.pEm, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr);  // Null_pos_set
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x100) != 0)  // Joint_move
    {
        G.pEm->Routine_2++;
    }
    frame = G.pEm->Move_cnt;
    if (frame == 0x2A)
    {
        G.Player.Life = -1;

        FUNC(PAD_VIB_SET0)(0xF, 0);  // Pad_vib_set0
        FUNC(PAD_VIB_FADE_SET)(0x14, 0xFA, 0x64, 0);  // Pad_vib_fade_set
    }
    if (frame == 0x28)
    {
        FUNC(SND_SE_ENEM)(3, 0);  // Snd_se_enem

        FUNC(PAD_VIB_SET0)(0xF, 0);  // Pad_vib_set0
        FUNC(PAD_VIB_FADE_SET)(0x1E, 0xFA, 0x64, 0);  // Pad_vib_fade_set
    }
    if (frame == 0x5E)
    {
        FUNC(SND_SE_ENEM)(7, 0);  // Snd_se_enem

        FUNC(PAD_VIB_SET0)(8, 0);  // Pad_vib_set0
    }
    if (frame == 0x66)
    {
        EM02_EF(Blood_cnt) = 6;
        pPl_parts = G.Player.pSin_parts_ptr;
        pP_ptr = pPl_parts;
        FUNC(MODEL_COLOR_PLUS)(pP_ptr, 0x30, 0x80820, 0x606060);  // Model_color_plus
        pP_ptr = &pPl_parts[1];
        FUNC(MODEL_COLOR_PLUS)(pP_ptr, 0x30, 0x80820, 0x606060);  // Model_color_plus
        pP_ptr = &pPl_parts[9];
        FUNC(MODEL_COLOR_PLUS)(pP_ptr, 0x30, 0x80820, 0x606060);  // Model_color_plus
        pP_ptr = &pPl_parts[12];
        FUNC(MODEL_COLOR_PLUS)(pP_ptr, 0x30, 0x80820, 0x606060);  // Model_color_plus
        pP_ptr = G.pEm->pSin_parts_ptr;
        pP_ptr += 4;
        FUNC(MODEL_COLOR_PLUS)(pP_ptr, 0x30, 0x80820, 0x606060);  // Model_color_plus
        FUNC(SND_SE_STAD)(3, 3, 0, &MAIN.Player.Pos_x);  // Snd_se_stad

        FUNC(PAD_VIB_SET0)(8, 0);  // Pad_vib_set0
        FUNC(PAD_VIB_FADE_SET)(8, 0x64, 0xFA, 0);  // Pad_vib_fade_set
        FUNC(PAD_VIB_FADE_SET)(0x3C, 0xFA, 0, 8);  // Pad_vib_fade_set
    }
    Set_blood((VOID*)&MAIN.Player.Be_flg, 1, 3);  // untyped pointer, the callee takes the address as is
    Set_blood(G.pEm, 3, 0);
}

VOID Em02_mv07(VOID)
{
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x100);  // Joint_move
    if (G.pEm->Routine_2 == 0)
    {
        Em02_mv07_wait();
        return;
    }
    Em02_mv07_track();
}

VOID Em02_mv07_wait(VOID)
{
    if (EM02_EF(player_dist) >= 0x1195 && PL_ROUTINE_WORD != ((0) << 24 | (0x14) << 16 | (3) << 8 | (1)))  // state 1, Key_rno 3, Routine_2 0x14, Act_sts 0
    {
        return;
    }
    G.pEm->Routine_2++;
    G.pEm->Move_no++;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0xF;
    EM02_EF(Yoko_ofs) = 0;
}

VOID Em02_mv07_track(VOID)
{
    Em02_neck_rot();
    if (G.pEm->Move_cnt == 0x21)
    {
        G.pEm->Routine_1 = 0;
        G.pEm->Type = 4;
        G.pEm->Damage_no = 0;
    }
}

VOID Em02_mv08(VOID)
{
    G.pEm->Routine_1 = 0;
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x100);  // Joint_move
}

VOID Em02_mv01(VOID)
{
    if (EM02_EF(player_dist) >= 0x1194 && PL_ROUTINE_WORD != ((0) << 24 | (0x14) << 16 | (3) << 8 | (1)))  // state 1, Key_rno 3, Routine_2 0x14, Act_sts 0
    {
    }
    else
    {
        G.pEm->Routine_1 = 0;
        G.pEm->Type = 4;
        G.pEm->Hokan_flg = 7;
    }
    FUNC(ADD_SPEED_XZ)(8);  // Add_speedXZ
    Em02_mv01_tbl[G.pEm->Routine_2]();

    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x100);  // Joint_move
    if (G.pEm->Move_cnt != 0 && G.pEm->Move_cnt != 0x1B)
    {
        return;
    }

    FUNC(SND_SE_ENEM)(9, 0);  // Snd_se_enem
}

VOID Em02_mv01_begin(VOID)
{
    G.pEm->Routine_2++;
    G.pEm->Pos_y = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0xF;
    EM02_EF(Sence_cnt) = 0;
    EM02_EF(Step_cnt) = 0;
    EM02_EF(Yoko_ofs) = 0;
}

VOID Em02_mv01_pick(VOID)
{
    SHORT dir;

    if ((EM02_EF(Ck_flg) & 1) || (EM02_EF(Move_flg) == 0 && G.pEm->Timer0 == 0))
    {
        G.pEm->Routine_2++;
        G.pEm->Timer0 = 0x400;
        if ((dir = Em02_turn2_ck(3, 0)) == 0)
        {
            if (EM02_EF(Move_flg) == 0)
            {
                EM02_EF(turn_delta) = Api_rand() & 1 ? EM02_EF(turn_step) : -EM02_EF(turn_step);  // rand
            }
            else
            {
                EM02_EF(turn_delta) = EM02_EF(turn_step);
            }
        }
        else
        {
            EM02_EF(turn_delta) = (USHORT)dir * EM02_EF(turn_step);  // truncates to an unsigned halfword
            EM02_EF(Step_cnt) = 0;
        }
        if (EM02_EF(Move_flg) == 0)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200) < 0U)  // Goto00_ck
            {
                G.pEm->Timer0 = G.pEm->Timer0 >> 2;
            }
        }
    }
    else
    {
        G.pEm->Timer0--;
    }
}

VOID Em02_mv01_turn(VOID)
{
    G.pEm->Cdir_y += EM02_EF(turn_delta);
    G.pEm->Timer0 -= __builtin_abs(*&EM02_FREE_P->turn_delta);  // *& form keeps the access (a plain member access changes the code)
    if (G.pEm->Timer0 < 0)
    {
        G.pEm->Routine_2 = 0;
        if (EM02_EF(Move_flg) == 0)
        {
            G.pEm->Timer0 = (Api_rand() & 0x1F) + 0x4B;  // rand
        }
    }
}

VOID Em02_mv09(VOID)
{
    Rate_speed_up(2, 0x28);
    FUNC(ADD_SPEED_XZ)(0);  // Add_speedXZ
    Em02_mv09_tbl[G.pEm->Routine_2]();

    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x100);  // Joint_move
    if (G.pEm->Move_cnt != 0 && G.pEm->Move_cnt != 0x1B)
    {
    }
    else
    {
        FUNC(SND_SE_ENEM)(9, 0);  // Snd_se_enem
    }
    Em02_neck_rot();
    if (Em02_pl_turn_ck(8) != 0 && G.pEm->Routine_2 != 3)
    {
        G.pEm->Routine_2 = 3U;
        G.pEm->Routine_3 = 0;
    }
    if (EM02_EF(player_dist) < 0x9C4)
    {
        EM02_EF(Sence_cnt)++;
        if (EM02_EF(Move_flg) & 4)
        {
            EM02_EF(Sence_cnt) = 0x41U;
        }
        if (G.Player.Spd_cur >= 0xC9)
        {
            EM02_EF(Sence_cnt) += 0x14;
        }
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x80) == 0 && EM02_EF(player_dist) >= 0x321)  // Goto00_ck
        {
            G.pEm->Routine_1 = 0;
            G.pEm->Type = 5;
            Em02_bark_flg = 1;
        }
    }
    else
    {
        EM02_EF(Sence_cnt) = (SHORT)EM02_EF(Sence_cnt) - ((SHORT)EM02_EF(Sence_cnt) >= 1);  // sign-extends the halfword

        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x80) == 0 && G.Player.Spd_cur >= 0xC9)  // Goto00_ck
        {
            EM02_EF(Sence_cnt) += 6;
        }
    }
    if (!((SHORT)EM02_EF(Sence_cnt) < 0x41 && PL_ROUTINE_WORD != ((0) << 24 | (0x14) << 16 | (3) << 8 | (1))) || (Em02_bark_flg != 0 && !(EM02_EF(Move_flg) & 4)))  // state 1, Key_rno 3, Routine_2 0x14, Act_sts 0
    {
        G.pEm->Routine_1 = 0;
        G.pEm->Type = 4;
        EM02_EF(Yoko_ofs) = 0;
        if (Em02_bark_flg == 0)
        {
            FUNC(SND_SE_ENEM)(6, 0);  // Snd_se_enem
            Em02_bark_flg = 1;
        }
    }
}

VOID Em02_mv09_begin(VOID)
{
    G.pEm->Routine_2++;
    EM02_EF(turn_step) = (Api_rand() & 7) + 0x10;  // rand
    EM02_EF(Step_cnt) = 0;
    if (EM02_EF(Yoko_ofs) < 0)
    {
        EM02_EF(turn_step) = -EM02_EF(turn_step);
    }
    G.pEm->Timer0 = (Api_rand() & 0x1F) + 0x2D;  // rand
}

VOID Em02_mv09_arc(VOID)
{
    G.pEm->Cdir_y += EM02_EF(turn_step);
    if (--G.pEm->Timer0 == 0)
    {
        G.pEm->Routine_2++;
        G.pEm->Timer0 = (USHORT)((Api_rand() & 7) - (USHORT)((Api_rand() & 0xF) - 30));  // rand
    }
}

VOID Em02_mv09_face(VOID)
{
    if ((--G.pEm->Timer0 != 0) && !(EM02_EF(Ck_flg) & 1))
    {
        return;
    }
    EM02_EF(turn_step) = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, (Api_rand() & 7) + 0x10);  // Goto00_ck, rand
    G.pEm->Timer0 = 2U;
    G.pEm->Routine_2 = 1;
    G.pEm->Timer0 = (Api_rand() & 0x1F) + 0x2D;  // rand
    if ((Api_rand() & 0x3F) >= 0x31U)  // rand
    {
        FUNC(SND_SE_ENEM)(2U, 0);  // Snd_se_enem
    }
}

VOID Em02_mv09_wall(VOID)
{
    Em02_wall_step(0x258, 0x10);
    if (EM02_EF(Ai_flg) & 0x10)
    {
        G.pEm->Routine_2 = 0;
    }
}

INT Em02_wall_step(SHORT Max_cnt, SHORT Turn_rate)
{
    SHORT turn;
    turn = Em02_wall_tbl[G.pEm->Routine_3](Turn_rate);

    if (Em02_pl_turn_ck(4) == 0 && EM02_EF(Step_cnt) >= 0xD)
    {
        EM02_EF(turn_delta) = 0;
        EM02_EF(Ai_flg) |= 0x10;
    }
    if (++EM02_EF(Step_cnt) > Max_cnt)
    {
        G.pEm->Routine_3 = 0U;
    }
    return turn != 0 ? turn > 0 ? 2 : 0 : 1;
}

SHORT Em02_wall00(SHORT Turn_rate)
{
    G.pEm->Routine_3++;
    EM02_EF(Step_cnt) = 0;
    if ((*(SHORT*)&EM02_EF(turn_step) = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, Turn_rate)) == 0)  // Goto00_ck
    {
        EM02_EF(turn_step) = Turn_rate;
    }
    EM02_EF(Ai_flg) &= 0xFFEF;
    return Em02_wall01(Turn_rate);
}

SHORT Em02_wall01(SHORT Turn_rate)
{
    USHORT turn;
    turn = 0;
    if (EM02_EF(Ck_flg) & 1)
    {
        G.pEm->Routine_3++;
        turn = EM02_EF(turn_step);
    }
    if (EM02_EF(Step_cnt) >= 9)
    {
        EM02_EF(Step_cnt) = 8;
    }
    return turn;  // sign-extends the halfword
}

SHORT Em02_wall02(SHORT Turn_rate)
{
    USHORT turn;
    turn = 0;
    if (!(EM02_EF(Ck_flg) & 0xF))
    {
        G.pEm->Routine_3++;
        G.pEm->Timer0 = 0;
        if ((EM02_EF(turn_delta) = Em02_turn2_ck(2, 1)) == 0)
        {
            EM02_EF(turn_delta) = -1;
        }
    }
    G.pEm->Cdir_y += EM02_EF(turn_step);
    turn = EM02_EF(turn_step);
    if (EM02_EF(Step_cnt) >= 9)
    {
        EM02_EF(Step_cnt) = 8;
    }
    return turn;  // sign-extends the halfword
}

SHORT Em02_wall03(SHORT Turn_rate)
{
    if ((Em02_turn_ck(EM02_EF(turn_delta) * 0x400, 1) == 0 && G.pEm->Timer0 == 0) || (EM02_EF(Ck_flg) & 1))
    {
        G.pEm->Routine_3 = 4;
        EM02_EF(turn_step) = EM02_EF(turn_delta);
        G.pEm->Timer0 = 0x400;
        return 0;
    }
    G.pEm->Timer0 = G.pEm->Timer0 - (G.pEm->Timer0 >= 1);
    return 0;
}

SHORT Em02_wall04(SHORT Turn_rate)
{
    G.pEm->Cdir_y += EM02_EF(turn_step) * (USHORT)Turn_rate;
    G.pEm->Timer0 -= (USHORT)Turn_rate;
    if (G.pEm->Timer0 <= 0)
    {
        G.pEm->Routine_3 = 3;
        G.pEm->Timer0 = 4U;
    }
    return EM02_EF(turn_step);  // sign-extends the halfword
}

VOID Em02_Damage(VOID)
{
    G.pEm->Be_flg &= EM_BE_STATE_MASK;

    if (!(EM02_EF(Ai_flg) & 0x20))
    {
        G.pEm->Be_flg |= 0x40;
    }

    FUNC(EM_CK_HANI)(0x1388);  // Em_ck_hani
    Em02_dm[G.pEm->Routine_1]();
}

VOID Em02_dm00(VOID)
{
    UCHAR beh;

    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 3;

    EM02_EF(Ai_flg) |= 0x20;

    beh = G.pEm->Type;
    if (!(beh >= 4U && beh < 8U) && beh != 9 && (G.pEm->Damage_no & 1))
    {
        G.pEm->Routine_1 = 1;
        G.pEm->Move_no = 9;
    }
    else
    {
        EM02_EF(turn_step) = (G.Player.Cdir_y & 0xFFF) - (G.pEm->Cdir_y & 0xFFF);

        EM02_EF(turn_step) += Em02_dm_dir_tbl[Api_rand() & 7];  // rand

        if (beh == 4 || (EM02_EF(Sence_flg) & 0x7F) == 0)
        {
            G.pEm->Routine_1 = 3;
            G.pEm->Move_no = 0xC;
            G.pEm->Hokan_flg = 0;
            G.pEm->Spd_cur = 0xF0;
            FUNC(SND_SE_ENEM)(1, 0);  // Snd_se_enem
        }
        else
        {
            G.pEm->Routine_1 = 2;
            G.pEm->Move_no = 0xB;
            G.pEm->Spd_cur >>= 1;
            EM02_EF(Sence_flg) |= ~0x7F;
        }
    }
    G.pEm->Routine_2 = 0;
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
    FUNC(SND_SE_ENEM)(8, 0U);  // Snd_se_enem
}

VOID Em02_dm01(VOID)
{
    EM02_EF(Yoko_ofs) = FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // Joint_move
    if (G.pEm->Routine_2 == 0)
    {
        Em02_dm01_end();
        return;
    }
    Em02_dm01_up();
}

VOID Em02_dm01_end(VOID)
{
    if (EM02_EF(Yoko_ofs) != 0)
    {
        G.pEm->Routine_2++;
        G.pEm->Move_no = 0xA;
        G.pEm->Move_wait = 0;
        G.pEm->Spd_cur = 0x82;
    }
}

VOID Em02_dm01_up(VOID)
{
    if (G.pEm->Move_cnt < 0xFU)
    {
        if (G.pEm->Move_cnt >= 8U)
        {
            Rate_speed_down(0x20, 0x28);
        }
        FUNC(ADD_SPEED_XZ)(0x800);  // Add_speedXZ
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x40);  // Goto00
    }
    if (EM02_EF(Yoko_ofs) != 0)
    {
        G.pEm->Routine_0 = 1;
        G.pEm->Routine_1 = 0;
        G.pEm->Type = 4;
        G.pEm->Damage_no = 0;
    }
}

VOID Em02_dm02(VOID)
{
    FUNC(ADD_SPEED_XZ)((SHORT)EM02_EF(turn_step));  // Add_speedXZ
    FUNC(EM_FALL_MOVE)(0, EM02_EF(Leap_spd_y), EM02_EF(ballistic_gravity), 0);  // Em_fall_move
    if (G.pEm->Pos_y >= 0)
    {
        G.pEm->Routine_1++;
        G.pEm->Move_no++;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0;
        G.pEm->Pos_y = 0;
        FUNC(SND_SE_ENEM)(1, 0);  // Snd_se_enem
    }
    if (G.pEm->Move_cnt != 0)
    {
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);  // Joint_move
    }
}

VOID Em02_dm03(VOID)
{
    if (G.pEm->Move_no == 0xC)
    {
        FUNC(ADD_SPEED_XZ)((SHORT)EM02_EF(turn_step));  // Add_speedXZ
        Rate_speed_down(0xA, 0);
    }
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)  // Joint_move
    {
        G.pEm->Move_no++;

        G.pEm->Damage_no = 0;

        if (G.pEm->Move_no == 0xE)
        {
            EM02_EF(Ai_flg) &= 0xFFDF;

            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200) == 0)  // Goto00_ck
            {
                G.pEm->Routine_1++;
                G.pEm->Move_no = 3U;
            }
            else
            {
                G.pEm->Routine_1 = 1U;
                G.pEm->Routine_2 = 1;
                G.pEm->Move_no = 0xAU;
                G.pEm->Spd_cur = 0x82;
            }
        }
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
    }
}

VOID Em02_dm04(VOID)
{
    if (G.pEm->Move_cnt == 0xA && G.pEm->Move_wait == 1)
    {
        FUNC(SND_SE_ENEM)(2, 0);  // Snd_se_enem
    }
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400) != 0)  // Joint_move
    {
        G.pEm->Routine_0 = 1;
        G.pEm->Routine_1 = 0;
        G.pEm->Type = 4;
        G.pEm->Damage_no = 0;
        EM02_EF(Sence_flg) &= 0x7F;
        FUNC(SND_SE_ENEM)(6, 0);  // Snd_se_enem
    }
}

VOID Em02_Die(VOID)
{
    if (G.pEm->Routine_1 >= 4U)
    {
        return;
    }
    Em02_die_tbl[G.pEm->Routine_1]();
}

VOID Em02_die00(VOID)
{
    FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg, G.pEm->Em_set_flg);  // Flg_on
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 7;
    G.pEm->Routine_2 = 0;
    EM02_EF(turn_step) = (G.Player.Cdir_y & 0xFFF) - (G.pEm->Cdir_y & 0xFFF);
    if (G.pEm->Type == 4 || (EM02_EF(Sence_flg) & 0x7F) == 0)
    {
        G.pEm->Routine_1 = 2;
        G.pEm->Move_no = 0xC;
        G.pEm->Hokan_flg = 0;
        G.pEm->Spd_cur = 0xF0;
    }
    else
    {
        G.pEm->Routine_1 = 1;
        G.pEm->Move_no = 0xB;
        G.pEm->Spd_cur >>= 1;
    }
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);  // Joint_move
    FUNC(SND_SE_ENEM)(4, 0U);  // Snd_se_enem
}

VOID Em02_die02(VOID)
{
    if (G.pEm->Move_no == 0xC)
    {
        FUNC(ADD_SPEED_XZ)((SHORT)EM02_EF(turn_step));  // Add_speedXZ
        Rate_speed_down(0xA, 0);
    }
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)  // Joint_move
    {
        G.pEm->Move_no++;
        G.pEm->Move_wait = 0;
    }
    if (G.pEm->Move_no == 0xD)
    {
        G.pEm->Routine_1++;
        G.pEm->Be_flg |= EM_BE_NO_HIT | EM_BE_NO_OBJ;
        G.pEm->Spd_cur = 0;
        G.pEm->Timer0 = 0x46;

        FUNC(KAGE_WORK_COLOR_SET)(&G.pEm->Kage_work, 1, 2, 0xFFFF50);  // Kage_work_color_set

        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, -0x5A, -0x5A);  // Kage_work_size_add
    }
}

VOID Em02_die03(VOID)
{
    if (G.pEm->Timer0 != 0)
    {
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, 6, 6);  // Kage_work_size_add
        G.pEm->Timer0--;
        return;
    }
    G.pEm->Routine_1++;
}

VOID Em02_Dead(VOID)
{
    return;
}

VOID Em02_mv00(VOID)
{
}

VOID Em02_pl_rec(VOID)
{
    if (G.Player.Routine_3 >= 2U)
    {
        return;
    }
    if (G.Player.Routine_3 == 0)
    {
        Em02_pl_rec_set();
        return;
    }
    Em02_pl_rec_run();
}

VOID Em02_pl_rec_set(VOID)
{
    INT unused;  // unused local: reserves stack space
    G.Player.Routine_3++;
    G.Player.Move_no = 2;
    G.Player.Move_cnt = 0;
    G.Player.Move_wait = 0;
    G.Player.Hokan_flg = 0;
    G.Player.Spd_cur = 0;
    FUNC(SND_SE_STAD)(3, 2, 0, &MAIN.Player.Pos_x);  // Snd_se_stad
}

VOID Em02_pl_rec_run(VOID)
{
    if (G.Player.Move_cnt == 0xF)
    {
        FUNC(SND_SE_WALK)(2, 0);  // Snd_se_walk
    }
    if (G.Player.Life >= 0)
    {
        FUNC(NULL_POS_SET)(&MAIN.Player, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr);  // Null_pos_set
    }
    if (FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x200) != 0)  // Joint_move
    {
        G.Player.Cdir_y += 0x800;
        G.Player.Routine_3++;
    }
}

UCHAR Em02_pl_bite(VOID)
{
    UCHAR facing;

    G.pEm->Damage_no = 0;

    if (G.Player.Life < 0x11)
    {
        if (FUNC(CDIR_CK)(&MAIN.Player) == 0)  // Cdir_ck
        {
            G.Player.Be_flg |= EM_BE_NO_HIT;
            G.pEm->Be_flg |= EM_BE_NO_HIT;
            G.pEm->Spd_cur = 0;
            EM02_EF(Sence_cnt) = 2;
            G.Player.Cdir_y = (G.pEm->Cdir_y + 0x800) & 0xFFF;
            G.pEm->Damage_no = 1;
            G.Player.Damage_no = 1;
            PL_ROUTINE_WORD = ROUTINES(2, 0, 2, 7);  // state 7, Key_rno 2, Routine_2 0, Act_sts 2
            G.pEm->Be_flg &= ~EM_BE_NO_WALL;
            return EM02_EF(Sence_cnt);
        }
    }
    G.Player.Routine_3 = FUNC(CDIR_CK)(&MAIN.Player);  // Cdir_ck
    G.pEm->Root_x = G.Player.Pos_x;
    G.pEm->Root_z = G.Player.Pos_z;
    if (G.pEm->Type != 6)
    {
        G.pEm->Routine_2 = 4;
        G.pEm->Move_no = 0x11;
        G.pEm->Move_cnt = 1;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0xF;
        G.pEm->Be_flg &= ~EM_BE_NO_WALL;
        G.pEm->Spd_cur >>= 2;
    }
    G.Player.Root_x = G.Player.Pos_x;
    G.Player.Root_z = G.Player.Pos_z;
    facing = FUNC(CDIR_CK)(&MAIN.Player);  // Cdir_ck
    G.Player.Damage_no = facing + 1;
    G.Player.Routine_2 = facing + 0x66;

    FUNC(PAD_VIB_SET0)(5, 0);  // Pad_vib_set0
    FUNC(PAD_VIB_SET1)(5, 0xC8, 0);  // Pad_vib_set1

    G.Player.Life -= 0x10;

    EM02_EF(turn_step) = 0x200;
    EM02_EF(Yoko_ofs) = 0;
    EM02_EF(Blood_cnt) = 3;
    EM02_EF(Step_cnt) = 0;
    FUNC(SND_SE_ENEM)(3, 0);  // Snd_se_enem
    return EM02_EF(Sence_cnt);
}

VOID Em02_at_ck(VOID)
{
    INT unused;  // unused local: reserves stack space
    SHORT hp;
    SHORT chance;

    hp = G.Player.Life;
    if (EM02_EF(player_dist) < 0x1388 && G.Player.Damage_no == 0)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x100) == 0)  // Goto00_ck
        {
            chance = G.Player.Sca_info & 8 ? 0x64 : 0x7E;

            if (hp < 0x11)  // sign-extends the halfword
            {
                if (FUNC(CDIR_CK)(&MAIN.Player) != 0)  // Cdir_ck
                {
                    if ((Api_rand() & 0x7F) < (UINT)chance)  // rand
                    {
                        G.pEm->Routine_1 = 0;
                        G.pEm->Type = 9;
                        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x100);  // Joint_move
                        return;
                    }
                }
            }
            G.pEm->Routine_1 = 0;
            G.pEm->Type = 5;
            EM02_EF(Ai_flg) |= 0x80;
            EM02_EF(Yoko_ofs) = 0;
            return;
        }
        if (EM02_EF(Yoko_ofs) < 0xA)
        {
            if (++EM02_EF(Yoko_ofs) >= 0xA)
            {
                EM02_EF(Yoko_ofs) += (SHORT)((Api_rand() & 7) + 0x1E);  // rand
                EM02_EF(ballistic_gravity) = 0;
            }
        }
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400) == 0)  // Goto00_ck
        {
            EM02_EF(Ai_flg) |= 2;
            return;
        }
        if ((EM02_EF(Ai_flg) & 3) == 2)
        {
            if ((Api_rand() & 0x40) >= 0x39U)  // rand
            {
                EM02_EF(Ai_flg) = (EM02_EF(Ai_flg) & 0xFFFD) | 5;
            }
        }
    }
}

VOID Set_blood(ENEMY_WORK* pEm, UCHAR joint_idx, UCHAR fx_type)
{
    PARTS_W* pP_ptr;

    if (EM02_EF(Blood_cnt) <= 0)
    {
        return;
    }
    pP_ptr = pEm->pSin_parts_ptr;
    pP_ptr = &pP_ptr[joint_idx];
    ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

    FUNC(ESP_CALL)(0, fx_type, 0, Monitor.pId_mat, pP_ptr->Workm.t, 0);  // Esp_call

    EM02_EF(Blood_cnt)--;
}

INT Em02_sca_ck(VOID)
{
    VECTOR ofs;
    SCHAR hit;

    ofs = MATRIX_POS(Monitor.pId_mat);
    ofs.vx = Em02_sca.Off_x;
    FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, &ScratchPad->Mat);  // RotMatrix
    FUNC(APPLY_MATRIX_LV)(&ScratchPad->Mat, &ofs, &ofs);  // ApplyMatrixLV
    G.pEm->Pos_x += ofs.vx;
    G.pEm->Pos_z += ofs.vz;
    hit = FUNC(SCA_CK_HIT)(&G.pEm->Pos_x, 0x190);  // Sca_ck_hit
    G.pEm->Pos_x -= ofs.vx;
    G.pEm->Pos_z -= ofs.vz;
    return hit >= 1;
}

SCHAR Em02_turn_ck(SHORT Dir_ofs, USHORT Spd_mul)
{
    INT old_x;
    INT old_z;
    INT Old_vx;
    INT Old_vz;
    USHORT Old_spd;
    SCHAR blocked;

    old_x = G.pEm->Pos_x;
    old_z = G.pEm->Pos_z;
    Old_vx = G.pEm->Old_x;
    Old_vz = G.pEm->Old_z;
    Old_spd = G.pEm->Spd_cur;
    G.pEm->Spd_cur = Old_spd * Spd_mul;
    FUNC(ADD_SPEED_XZ)(Dir_ofs);  // Add_speedXZ
    blocked = Em02_sca_ck();
    G.pEm->Pos_x = old_x;
    G.pEm->Pos_z = old_z;
    G.pEm->Old_x = Old_vx;
    G.pEm->Old_z = Old_vz;
    G.pEm->Spd_cur = Old_spd;
    return blocked;
}

SCHAR Em02_turn2_ck(SHORT Spd_mul, SCHAR Both_val)
{
    SCHAR Ck_pos;
    SCHAR Ck_neg;
    Ck_pos = NOPROTO(SCHAR, Em02_turn_ck)(0x400, Spd_mul);  // called through an unprototyped function type: arguments are passed unconverted
    Ck_neg = -NOPROTO(SCHAR, Em02_turn_ck)(-1024, Spd_mul);  // called through an unprototyped function type: arguments are passed unconverted
    if (Ck_pos != 0 && Ck_neg != 0)
    {
        return Both_val;
    }
    return Ck_pos + Ck_neg;  // sign-extends the byte
}

VOID Rate_speed_up(USHORT step, SHORT limit)
{
    if (G.pEm->Spd_cur < limit)
    {
        G.pEm->Spd_cur += step;
        if (G.pEm->Spd_cur > limit)
        {
            G.pEm->Spd_cur = limit;
        }
    }
}

VOID Rate_speed_down(USHORT step, SHORT limit)
{
    if (G.pEm->Spd_cur > limit)
    {
        G.pEm->Spd_cur -= step;
        if (G.pEm->Spd_cur < limit)
        {
            G.pEm->Spd_cur = limit;
        }
    }
}

VOID Set_run_motion(SHORT turn)
{
    UCHAR Mot_no;

    Mot_no = Em02_run_mot_tbl[(turn != 0) ? (turn > 0 ? 2 : 0) : 1];
    if (G.pEm->Move_no != Mot_no)
    {
        G.pEm->Move_no = Mot_no;
        G.pEm->Move_wait = 0;
        if (G.pEm->Hokan_flg < 4U)
        {
            G.pEm->Hokan_flg = 7U;
        }
    }
}

VOID* Em02_back_rot(USHORT rx, USHORT ry, USHORT rz)
{
    PARTS_W* pP_ptr;
    pP_ptr = G.pEm->pSin_parts_ptr;
    pP_ptr += 1;
    pP_ptr->Cdir.vx += rx;
    pP_ptr->Cdir.vy += ry;
    pP_ptr->Cdir.vz += rz;
    FUNC(ROT_MATRIX)(&pP_ptr->Cdir, &pP_ptr->Coord);  // RotMatrix
    pP_ptr++;
    pP_ptr->Cdir.vx += rx + ((SHORT)rx >> 2);  // sign-extends the halfword
    pP_ptr->Cdir.vy += ry + ((SHORT)ry >> 2);  // sign-extends the halfword
    pP_ptr->Cdir.vz += rz + ((SHORT)rz >> 2);  // sign-extends the halfword
    FUNC(ROT_MATRIX)(&pP_ptr->Cdir, &pP_ptr->Coord);  // RotMatrix
    pP_ptr++;
    pP_ptr->Cdir.vx += rx + ((SHORT)rx >> 1);  // sign-extends the halfword
    pP_ptr->Cdir.vy += ry + ((SHORT)ry >> 1);  // sign-extends the halfword
    pP_ptr->Cdir.vz += rz + ((SHORT)rz >> 1);  // sign-extends the halfword
    FUNC(ROT_MATRIX)(&pP_ptr->Cdir, &pP_ptr->Coord);  // RotMatrix
    return pP_ptr;
}

SCHAR Em02_neck_rot(VOID)
{
    PARTS_W* pP_ptr;
    INT unused;  // unused local: reserves stack space
    VECTOR saved_pos;
    USHORT Dir_bak;
    SHORT Lim_flg;

    Lim_flg = 0;
    pP_ptr = ((PARTS_W * (*)(INT, INT, INT)) Em02_back_rot)(0, EM02_EF(Yoko_ofs), 0);  // called through an unprototyped function type: arguments are passed unconverted
    saved_pos.vx = G.pEm->Pos_x;
    saved_pos.vz = G.pEm->Pos_z;
    Dir_bak = G.pEm->Cdir_y;
    G.pEm->Pos_x = pP_ptr->Coord.t[0] + pP_ptr->Workm.t[0];
    G.pEm->Pos_z = pP_ptr->Coord.t[2] + pP_ptr->Workm.t[2];
    G.pEm->Cdir_y += EM02_EF(Yoko_ofs) + (EM02_EF(Yoko_ofs) >> 1);
    EM02_EF(Yoko_ofs) += FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 8);  // Goto00_ck
    if (EM02_EF(Yoko_ofs) < -0x100)
    {
        EM02_EF(Yoko_ofs) = -0x100;
        Lim_flg = 1;
    }
    if (EM02_EF(Yoko_ofs) >= 0x101)
    {
        EM02_EF(Yoko_ofs) = 0x100;
        Lim_flg = 1;
    }
    G.pEm->Pos_x = saved_pos.vx;
    G.pEm->Pos_z = saved_pos.vz;
    G.pEm->Cdir_y = Dir_bak;
    return Lim_flg;  // sign-extends the byte
}

SCHAR Em02_pl_turn_ck(SHORT Spd_mul)
{
    USHORT Dir_pl;
    Dir_pl = FUNC(GET_DIR_TGT)(G.Player.Pos_x, G.Player.Pos_z);  // Get_dir_tgt
    return FN_CAST(SCHAR, (SHORT, SHORT), Em02_turn_ck)(Dir_pl - G.pEm->Cdir_y, Spd_mul);  // called through an unprototyped function type: arguments are passed unconverted
}

// collision info of the dog: flags, offset x/y/z, half height, radius
AT_TBL Em02_sca =
{
    0x8000,
    0x01F4,
    -0x0320,
    0,
    0x0320,
    0x0190,
};

// base health of a new dog, indexed by a random value
UCHAR Em02_life[16] =
{
    0x77,   // 0 //
    0x63,   // 1 //
    0x77,   // 2 //
    0x63,   // 3 //
    0x77,   // 4 //
    0x63,   // 5 //
    0x77,   // 6 //
    0x63,   // 7 //
    0x63,   // 8 //
    0x63,   // 9 //
    0x3B,   // 10 //
    0x63,   // 11 //
    0x3B,   // 12 //
    0x63,   // 13 //
    0x3B,   // 14 //
    0x63,   // 15 //
};

INT Em02_bark_flg = 0; // set once a dog has barked, so it does not bark again

// state handlers, indexed by the enemy state
VOID (*Em02_routine_0[5])(VOID) =
{
    Em02_Init,           // 0 //
    Em02_Move,   // 1 //
    Em02_Damage,        // 2 //
    Em02_Die,            // 3 //
    Em02_Dead,     // 4 //
};

// behaviour handlers, indexed by the enemy sub-state
VOID (*Em02_mv[10])(VOID) =
{
    Em02_mv00,   // 0 //
    Em02_mv01,      // 1 //
    Em02_mv02,       // 2 //
    Em02_mv03,        // 3 //
    Em02_mv04,      // 4 //
    Em02_mv05,       // 5 //
    Em02_mv06,        // 6 //
    Em02_mv07,        // 7 //
    Em02_mv08,         // 8 //
    Em02_mv09,       // 9 //
};

// functions that enter a sub-state, indexed by the behaviour id
VOID (*Em02_mv_set_tbl[13])(VOID) =
{
    Em02_mv01_set,     // 0 //
    Em02_mv01_set,     // 1 //
    Em02_mv01_set,     // 2 //
    Em02_mv01_set,     // 3 //
    Em02_mv02_set,      // 4 //
    Em02_mv03_set,   // 5 //
    Em02_mv03_set,   // 6 //
    Em02_mv03_set,   // 7 //
    Em02_mv07_set,       // 8 //
    Em02_mv04_set,     // 9 //
    Em02_mv05_set,      // 10 //
    Em02_mv05_set,      // 11 //
    Em02_mv09_set,      // 12 //
};

EM02_WALK_REC Em02_walk_tbl[2] = { { 1, 0, 0x28 }, { 0xE, 0, 0x82 } }; // walk animation and speed of the two patrol gaits

// chase handlers, indexed by Routine_2
VOID (*Em02_mv02_tbl[3])(VOID) =
{
    Em02_mv02_run,    // 0 //
    Em02_mv02_turn,   // 1 //
    Em20_Mv_run20,   // 2 //
};

// leap handlers, indexed by Routine_2
VOID (*Em02_mv03_tbl[5])(VOID) =
{
    Em02_mv03_crouch,     // 0 //
    Em02_mv03_air,   // 1 //
    Em02_mv03_land,       // 2 //
    Em02_mv03_rec,    // 3 //
    Em02_mv03_tumble,     // 4 //
};

// strafe handlers, indexed by Routine_2
VOID (*Em02_mv04_tbl[3])(VOID) =
{
    Em02_mv04_hold,     // 0 //
    Em02_mv04_acc,    // 1 //
    Em02_mv04_spring,   // 2 //
};

// patrol handlers, indexed by Routine_2
VOID (*Em02_mv01_tbl[3])(VOID) =
{
    Em02_mv01_begin,       // 0 //
    Em02_mv01_pick,   // 1 //
    Em02_mv01_turn,        // 2 //
};

// stalk handlers, indexed by Routine_2
VOID (*Em02_mv09_tbl[4])(VOID) =
{
    Em02_mv09_begin,        // 0 //
    Em02_mv09_arc,          // 1 //
    Em02_mv09_face,       // 2 //
    Em02_mv09_wall,   // 3 //
};

// wall-following steps, indexed by Act_sts
SHORT (*Em02_wall_tbl[5])(SHORT) =
{
    Em02_wall00,     // 0 //
    Em02_wall01,     // 1 //
    Em02_wall02,    // 2 //
    Em02_wall03,   // 3 //
    Em02_wall04,      // 4 //
};

// Damage reaction handlers, indexed by the enemy sub-state
VOID (*Em02_dm[5])(VOID) =
{
    Em02_dm00,     // 0 //
    Em02_dm01,   // 1 //
    Em02_dm02,    // 2 //
    Em02_dm03,      // 3 //
    Em02_dm04,     // 4 //
};

// random turn offsets added to the knock-back direction, read as eight SHORT values
SHORT Em02_dm_dir_tbl[8] =
{
    0x0080, -0xC0, -0x80, 0x00C0, -0x100, 0x100, 0x100, -0x100,
};

// death sequence steps, indexed by the enemy sub-state
VOID (*Em02_die_tbl[4])(VOID) =
{
    Em02_die00,        // 0 //
    Em02_dm02,   // 1 //
    Em02_die02,        // 2 //
    Em02_die03,       // 3 //
};

// run animation id by turn direction: negative, none, positive
UCHAR Em02_run_mot_tbl[4] =
{
    0x0F,   // 0 //
    0x02,   // 1 //
    0x10,   // 2 //
    0x00,   // 3 //
};
