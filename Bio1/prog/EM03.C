/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM03.C
 */

#include "EM03.H"
#include "SCRATCHPAD.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Em03_Init(VOID)
{
    G.pEm->Routine_1 = 0;
    G.pEm->Routine_2 = 0;
    G.pEm->Routine_3 = 0;
    G.pEm->Coord_flg = 0;
    G.pEm->Dummy0c1 = 0;
    G.pEm->Timer0 = 0;
    G.pEm->Timer2 = 0;
    G.pEm->Damage_no = 0;
    G.pEm->Move_no = 0;

    G.pEm->Routine_0 = 1;
    Api_rand();  // pseudo-random number
    G.pEm->Life = Em03_life[Api_rand() & 0xF];  // pseudo-random number
    if (G.pEm->Type != 4)
    {
        if (G.pEm->Type < 2U)
        {
            G.pEm->Cdir_x = 0;
            G.pEm->Cdir_z = 0;
            G.pEm->Pos_y = 0;
        }
        if (G.pEm->Type >= 2U)
        {
            G.pEm->Cdir_x = 0x800;
            G.pEm->Cdir_z = 0;
            G.pEm->Pos_y = -0x17E8;
            if (G.SaveData.Stage_no == 3)
            {
                G.pEm->Pos_y = -0x1C20;
            }
            if (G.pEm->Type >= 4U)
            {
                G.pEm->Type = 2U;
            }
        }
    }
    else
    {
        G.pEm->Type = 0U;

        G.pEm->Cdir_x = 0;
        G.pEm->Cdir_z = 0;
        G.pEm->Pos_y = 0;

        G.pEm->Routine_0 = 3;
        G.pEm->Life = -1;
        G.pEm->Damage_no = 0x11;
    }
    FUNC(INIT_PARTS_WORK)();  // initialise the joint parts

    FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
    ScratchWord[7] = 0x808080;
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vx = 0;
    FUNC(KAGE_WORK_INIT)(&ScratchPad->Sv, &G.pEm->Kage_work, 0x4B0, 0x4B0);  // initialise the shadow

    G.pEm->pAt_tbl = pEm03_sca;
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    G.pEm->Scale = (G.pEm->Type & 4) * 0xAAA;
    EM03_EF(attack_cooldown) = 0;
    EM03_EF(death_started) = 0;
    EM03_EF(wall_stuck_frames) = 0;
    EM03_EF(Wound_cnt) = 0;
    return;
}

VOID Em03_Main(VOID)
{
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        Em03_routine_0[G.pEm->Routine_0]();
        if (G.pEm->Routine_0 != 5)
        {
            FUNC(OBA_SET_OFS)(G.pEm);  // rotate the hit shape offsets
            G.pEm->Free[2] = FUNC(OBA_CK_HIT)(&MAIN.Player.Be_flg, G.pEm);  // hit test against the player
            FUNC(OBA_CK_EM)();  // collide with the other enemies
            EM03_EF(room_hit) = FUNC(SCA_CK_HIT)(&G.pEm->Pos_x, G.pEm->pAt_tbl->At_w);  // collide with the room boundaries
        }
    }
    if (!(G.Stop_flg & STOP_ENEMY_MOVE) && G.pEm->Routine_0 == 3 && G.pEm->Routine_3 == 6)
    {
        Em03_routine_0[G.pEm->Routine_0]();
    }
    G.pEm->Zone_in_flg = FUNC(HIT_CK_POINT4)(&G.pEm->Pos_x, G.Cccut);  // camera switch zone of the position
    if (G.pEm->Zone_in_flg != 0 && !(G.pEm->Type & 2))
    {
        FUNC(MUL_KAGE)(&G.pEm->Pos_x, &G.pEm->Kage_work, 0U, G.pEm->Cdir_y);  // draw the shadow
    }
}

VOID Em03_Move(VOID)
{
    if (G.pEm->Type & 0x80)
    {
        return;
    }
    switch (G.pEm->Routine_1)
    {
    case 0:
        Em03_Pick();

    case 1:
        Em03_Mv_proc();
    }
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    G.pEm->Be_flg = G.pEm->Be_flg | 0x40;
    FUNC(EM_CK_HANI)(0xFA0);  // range check to the player
    if (G.pEm->Type & 2)
    {
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        ScratchWord[0] = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);
        if (ScratchWord[0] < 0xBB8 && G.pEm->Routine_2 != 8)
        {
            G.pEm->Routine_1 = 1U;
            EM_ROUTINE_UHALF2 = 8;  // USHORT view of the field (plain access changes the code)
        }
        if (G.Player.Routine_2 == 0x14 && G.Player.Routine_3 == 0)
        {
            G.pEm->Routine_1 = 1U;
            EM_ROUTINE_UHALF2 = 8;  // USHORT view of the field (plain access changes the code)
        }
    }
    if (EM03_EF(attack_cooldown) != 0)
    {
        EM03_EF(attack_cooldown)--;
    }
    if (EM03_EF(room_hit) != 0)
    {
        EM03_EF(wall_stuck_frames)++;
        return;
    }
    EM03_EF(wall_stuck_frames) = 0U;
}

VOID Em03_Damage(VOID)
{
    if (G.pEm->Routine_2 != 0)
    {
        return;
    }
    G.pEm->Move_no = 1;
    Em03_Dm_rtn();
    return;
}

VOID Em03_Die(VOID)
{
    if (G.pEm->Routine_1 == 0)
    {
        G.pEm->Routine_2 = G.pEm->Damage_no >> 3;
        G.pEm->Routine_1 = 1;
        if (EM03_EF(death_started) != 0)
        {
            G.pEm->Routine_3 = 3;
            G.pEm->Routine_2 = 2;
        }
        EM03_EF(death_started) = 1;
    }
    switch (G.pEm->Routine_2)
    {
    case 1:
    case 2:
    case 6:
    case 8:
    case 9:
        Em03_Die_flip();
        break;
    default:
        G.pEm->Move_no = 1;
        Em03_Die_burst();
    }
}

VOID Em03_Dead(VOID)
{
}

VOID Em03_Nop5(VOID)
{
}

VOID Em03_Mv_proc(VOID)
{
    ScratchWord[7] = FUNC(EM_SIKAI_PROC)();  // sight check against the player
    if ((ScratchWord[7] >> 1) == 0)
    {
        G.pEm->Free[1] = G.pEm->Free[1] & 0xFFFE;
        G.pEm->Free[1] = G.pEm->Free[1] | (ScratchWord[7] & 1);
    }
    switch (G.pEm->Routine_2)
    {
    case 0:
        Em03_mv00();
        return;
    case 1:
        G.pEm->Move_no = 3;
        G.pEm->Cdir_y = G.pEm->Cdir_y + ((G.pEm->Free[1] & 1) & 0xFFFF) * FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x10);  // turn step toward the target; address as integer
        Em03_Walk_move();
        G.pEm->Spd_cur = G.pEm->Spd_cur + 0x3C;
        FUNC(ADD_SPEED_XZ)(0);  // move by the speed on x and z
        return;
    case 2:
        Em03_mv02();
        return;
    case 3:
        G.pEm->Move_no = 2;
        G.pEm->Free[0] = 0x80;
        Em03_Yori_move();
        return;
    case 4:
        G.pEm->Move_no = 2;
        G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x30);  // turn step toward the target; address as integer
        Em03_Walk_move();
        G.pEm->Spd_cur = G.pEm->Spd_cur + 0x64;
        FUNC(ADD_SPEED_XZ)(0);  // move by the speed on x and z
        return;
    case 5:
        G.pEm->Move_no = 3;
        G.pEm->Free[0] = 0x20;
        Em03_Yori_move();
        return;
    case 6:
        Em03_mv06();
        return;
    case 7:
        Em03_mv07();
        return;
    case 8:
        Em03_mv08();
        return;
    case 9:
        Em03_mv09();
        return;
    case 10:
        Em03_mv0a();
        return;
    case 11:
        G.pEm->Move_no = 3;
        G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x10);  // turn step toward the target; address as integer
        Em03_Walk_move();
        G.pEm->Spd_cur = G.pEm->Spd_cur + 0x14;
        FUNC(ADD_SPEED_XZ)(0);  // move by the speed on x and z
        break;
    }
}

VOID Em03_Pick(VOID)
{
    ScratchWord[0] = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);
    Em03_pick_tbl[G.pEm->Type]();
}

VOID Em03_Pick_floor0(VOID)
{
    if (ScratchWord[0] < 0x1770 && !(G.pEm->Free[1] & 1))
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 3;  // SHORT view of the field (plain access changes the code)
    }
    if (EM03_EF(attack_cooldown) == 0)
    {
        if (!(Api_rand() & 0x1FF) && (G.pEm->Free[1] & 1))  // pseudo-random number
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_HALF2 = 6;  // SHORT view of the field (plain access changes the code)
        }
        if (ScratchWord[0] < 0x1B58)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x80) == 0)  // turn step toward the target; address as integer
            {
                G.pEm->Routine_1 = 1;
                EM_ROUTINE_HALF2 = (Api_rand() & 1) + 6;  // pseudo-random number; SHORT view of the field (plain access changes the code)
            }
        }
    }
    if (G.Player.Routine_2 == 0x14 && G.Player.Routine_3 == 0)
    {
        if (!(Api_rand() & 1))  // pseudo-random number
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_HALF2 = 0xA;  // SHORT view of the field (plain access changes the code)
        }
    }
    if (EM03_EF(wall_stuck_frames) >= 0x1F)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 0xA;  // SHORT view of the field (plain access changes the code)
    }
    if (G.Player.Damage_no & 0x80)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 0xB;  // SHORT view of the field (plain access changes the code)
    }
}

VOID Em03_Pick_floor1(VOID)
{
    ScratchWord[6] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x80);  // turn step toward the target
    ScratchWord[7] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x100);  // turn step toward the target
    if (G.Player.Routine_2 == 0x14 && ScratchWord[6] != 0 && G.Player.Routine_3 == 0)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 3;  // SHORT view of the field (plain access changes the code)
    }
    if (ScratchWord[0] < 0x1F40 && ScratchWord[6] != 0)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 3;  // SHORT view of the field (plain access changes the code)
    }
    if (EM03_EF(attack_cooldown) == 0)
    {
        if (ScratchWord[0] < 0x1D4C && (G.pEm->Free[1] & 1))
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_HALF2 = (Api_rand() & 1) + 6;  // pseudo-random number; SHORT view of the field (plain access changes the code)
        }
        if (ScratchWord[0] < 0x1770 && ScratchWord[7] == 0)
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_HALF2 = 7;  // SHORT view of the field (plain access changes the code)
        }
    }
    if (G.Player.Routine_2 == 0x14 && G.Player.Routine_3 == 0)
    {
        if (!(Api_rand() & 0xF))  // pseudo-random number
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_HALF2 = 0xA;  // SHORT view of the field (plain access changes the code)
        }
    }
    if (EM03_EF(wall_stuck_frames) >= 0x1F)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 0xA;  // SHORT view of the field (plain access changes the code)
    }
}

VOID Em03_Pick_ceiling(VOID)
{
    if (ScratchWord[0] < 0xBB8)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 8;  // SHORT view of the field (plain access changes the code)
        return;
    }
    if (G.Player.Routine_2 == 0x14 && G.Player.Routine_3 == 0)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 8;  // SHORT view of the field (plain access changes the code)
        return;
    }
    if (ScratchWord[0] >= 0x1389)
    {
        G.pEm->Routine_1 = 1;
        EM_ROUTINE_HALF2 = 2;  // SHORT view of the field (plain access changes the code)
    }
    if (ScratchWord[0] >= 0x1389)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x80) == 0)  // turn step toward the target; address as integer
        {
            G.pEm->Routine_1 = 1;
            EM_ROUTINE_HALF2 = 0xB;  // SHORT view of the field (plain access changes the code)
        }
    }
}

VOID Em03_mv00(VOID)
{
    Em03_mv00_tbl[G.pEm->Type]();
}

VOID Em03_mv00_pick(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0;
        G.pEm->Timer0 = (USHORT)((Api_rand() & 0x3F) + 0x50 - (G.pEm->Free[1] & 1) * 0x50);  // pseudo-random number; real width or sign conversion
        G.pEm->Hokan_flg = 3;

    case 1:
        FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_1 = 0;
            G.pEm->Routine_2 = Em03_mv_next_tbl[G.pEm->Free[1] & 1][Api_rand() & 7];  // pseudo-random number
            G.pEm->Routine_3 = 0U;
        }
        return;
    }
}

VOID Em03_mv00_hold(VOID)
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
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        return;
    }
}

VOID Em03_Walk_move(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Timer0 = (Api_rand() & 0x1F) * G.pEm->Routine_2;  // pseudo-random number
        if (!((G.pEm->Routine_2 >> 1) & 0xFF))
        {
            G.pEm->Timer0 = G.pEm->Timer0 + 0x50;
        }

    case 1:
        if (Em03_step_se_tbl[G.pEm->Move_cnt] != 0)
        {
            FUNC(SND_SE_ENEM)(Em03_step_se_tbl[G.pEm->Move_cnt], 0);  // enemy sound effect
        }
        FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        if (G.pEm->Move_cnt != 0 && G.pEm->Move_cnt < 9U)
        {
            Em03_Leg_spd_set(1, G.pEm->Scale);
        }
        else
        {
            Em03_Leg_spd_set(0, G.pEm->Scale);
        }
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_1 = 0;
            EM_ROUTINE_UHALF2 = 0;  // USHORT view of the field (plain access changes the code)
        }
    }
}

VOID Em03_Yori_move(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Damage_no = 0;
        G.pEm->Routine_3 = 1U;
        G.pEm->Timer0 = (Api_rand() & 0x1F) + 0x50;  // pseudo-random number

    case 1:
        ScratchWord[7] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, G.pEm->Free[0]);  // turn step toward the target
        if (!(G.pEm->Timer0-- != 0) || ScratchWord[7] == 0)
        {
            G.pEm->Routine_3 = 2U;
        }
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        G.pEm->Cdir_y = G.pEm->Cdir_y + ScratchWord[7];
        return;
    case 2:
        G.pEm->Routine_1 = 0;
        G.pEm->Routine_2 = (G.pEm->Free[1] & 1) * 0xB;
        G.pEm->Routine_3 = 0U;
        G.pEm->Dest_x = G.Player.Pos_x;
        G.pEm->Dest_z = G.Player.Pos_z;
        return;
    }
}

VOID Em03_mv02(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 3;
        G.pEm->Timer0 = Api_rand() & 0x3F;  // pseudo-random number
        G.pEm->Hokan_flg = 3;
        EM03_EF(turn_step) = 0x18U;
        if ((G.pEm->Timer0 & 1) != 0)
        {
            EM03_EF(turn_step) = -0x18;
        }

    case 1:
        G.pEm->Cdir_y = G.pEm->Cdir_y + (EM03_EF(turn_step));
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        if (G.pEm->Free[1] & 1)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, G.pEm->Free[0]) == 0)  // turn step toward the target; address as integer
            {
                G.pEm->Routine_2 = 4;
                G.pEm->Routine_3 = 0U;
                return;
            }
        }
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_1 = 0;
            G.pEm->Routine_2 = (G.pEm->Free[1] & 1) * 4;
            G.pEm->Routine_3 = 0U;
        }
        return;
    }
}

VOID Em03_mv06(VOID)
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
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        G.pEm->Spd_cur = 0x64;
        FUNC(ADD_SPEED_XZ)(0);  // move by the speed on x and z
        return;
    case 2:
        G.pEm->Routine_3 = 3;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 0xB;
        G.pEm->Timer0 = (Api_rand() & 0x1F) + 0x14;  // pseudo-random number

    case 3:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        ScratchWord[7] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x40);  // turn step toward the target
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
        G.pEm->Timer0 = (Api_rand() & 0x1F) + 0xA;  // pseudo-random number

    case 5:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x10);  // turn step toward the target
        G.pEm->Spd_cur = 0x12C;
        FUNC(ADD_SPEED_XZ)(0);  // move by the speed on x and z
        if ((G.pEm->Timer0-- != 0) && FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200) == 0)  // turn step toward the target
        {
        }
        else
        {
            G.pEm->Routine_3 = 6;
        }
        ScratchWord[0] = (__builtin_abs(G.Player.Pos_x - G.pEm->Pos_x)) + (__builtin_abs(G.Player.Pos_z - G.pEm->Pos_z));
        if (G.pEm->Free[2] != 0 && G.Player.Damage_no == 0)
        {
            ScratchWord[7] = FUNC(CDIR_CK)(&MAIN.Player.Be_flg);  // facing check against the player
            G.pEm->Routine_3 = 6;
            FUNC(SND_SE_ENEM)(4, 0);  // enemy sound effect

            G.Player.Life -= 0x12;

            if (G.Player.Life < 0)
            {
                G.Player.Life = 1;
            }
            if (G.Player.Life >= 0)
            {
                G.Player.Damage_no = ScratchWord[7] + 1;
                G.Player.Routine_2 = ScratchWord[7] + 0x66;
            }

            FUNC(PAD_VIB_SET0)(3, 0);  // small-motor vibration
            FUNC(PAD_VIB_SET1)(8, 0xFA, 0);  // large-motor vibration
        }
        return;
    case 6:
        G.pEm->Routine_3 = 7;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 9;
        G.pEm->Timer0 = 7;

    case 7:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        G.pEm->Spd_cur = 0x64;
        FUNC(ADD_SPEED_XZ)(0);  // move by the speed on x and z
        return;
    case 8:
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = 0;  // SHORT view of the field (plain access changes the code)
        EM03_EF(attack_cooldown) = 0xF;
    }
}

VOID Em03_mv07(VOID)
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

        FUNC(ESP_CALL)(0x1E, 0, 0, G.pEm->M, &ScratchPad->Vec, 0);  // spawn an effect

        FUNC(SND_SE_ENEM)(7, 0);  // enemy sound effect

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        return;
    case 2:
        EM_ROUTINE_HALF0 = 1;  // SHORT view of the field (plain access changes the code)
        EM_ROUTINE_HALF2 = 0xB;  // SHORT view of the field (plain access changes the code)
        EM03_EF(attack_cooldown) = 0xF;
    }
}

VOID Em03_mv08(VOID)
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
        G.pEm->Cdir_x = G.pEm->Cdir_x - 0x100;
        if (G.pEm->Cdir_x & 0x80000000)
        {
            G.pEm->Cdir_x = 0U;
        }
        if (!(G.pEm->Pos_y & 0x80000000))
        {
            G.pEm->Pos_y = 0;
            G.pEm->Cdir_x = 0U;
            G.pEm->Routine_3 = 2U;
        }
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        return;
    case 2:
        G.pEm->Routine_3 = 3U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 7;
        FUNC(SND_SE_ENEM)(3, 0);  // enemy sound effect
        G.pEm->Type = G.pEm->Type & 0xFD;

    case 3:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        return;
    case 4:
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = 0;  // SHORT view of the field (plain access changes the code)

    default:;
    }
}

VOID Em03_mv09(VOID)
{
}

VOID Em03_mv0a(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_no = 6;
        EM03_EF(turn_step) = 0x32C - (Api_rand() & 1) * 0x658;  // pseudo-random number

        EM03_EF(turn_step) += (Api_rand() & 0xFFF) * !(G.pEm->Free[1] & 1);  // pseudo-random number

        G.pEm->Spd_cur = (Api_rand() & 0x3F) + 0xC8;  // pseudo-random number
        FUNC(ADD_SPEED_XZ)(EM03_EF(turn_step));  // move by the speed on x and z

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        G.pEm->Cdir_y = G.pEm->Cdir_y + FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x40);  // turn step toward the target; address as integer
        G.pEm->Pos_x = G.pEm->Pos_x + G.pEm->Spd_x;
        G.pEm->Pos_y = G.pEm->Pos_y + G.pEm->Spd_y;
        G.pEm->Pos_z = G.pEm->Pos_z + G.pEm->Spd_z;
        return;
    case 2:
        G.pEm->Damage_no = 0;
        G.pEm->Routine_1 = 0;
        G.pEm->Routine_2 = (G.pEm->Free[1] & 1) * 4;
        G.pEm->Routine_3 = 0U;
        EM03_EF(wall_stuck_frames) = 0;
    }
}

VOID Em03_Dm_rtn(VOID)
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
        FUNC(SND_SE_ENEM)(4, 0);  // enemy sound effect
        if (!(G.pEm->Damage_no & 1))
        {
            ScratchWord[7] = Api_rand() & 7;  // pseudo-random number
            ScratchWord[6] = 8;
            ScratchWord[5] = 0;
            while ((ScratchWord[6] = ScratchWord[6] - 1) != -1)
            {
                if (Em03_wound_tbl[ScratchWord[7]] == Em03_wound_use[ScratchWord[6]])
                {
                    ScratchWord[5] = 1;
                }
            }
            if (ScratchWord[5] == 0)
            {
                pP_ptr = G.pEm->pSin_parts_ptr;
                Em03_wound_use[EM03_EF(Wound_cnt)] = Em03_wound_tbl[ScratchWord[7]];
                FUNC(JOINT_ATCK_SET)(&pP_ptr[Em03_wound_tbl[ScratchWord[7]]], 0x1E, 0x14, 3);  // tint a joint for the hit effect
                FUNC(JOINT_ATCK_SET)(&pP_ptr[Em03_wound_tbl[ScratchWord[7]]] + 1, 0x1E, 0x14, 3);  // tint a joint for the hit effect
                ScratchPad->Vec.vz = 0;
                ScratchPad->Vec.vy = 0;
                ScratchPad->Vec.vx = 0;

                FUNC(ESP_CALL)(0, 8, 0, &(&pP_ptr[Em03_wound_tbl[ScratchWord[7]]])[1].Workm, &ScratchPad->Vec, 0);  // spawn an effect
                FUNC(ESP_CALL)(0, 8, 0, &pP_ptr[Em03_wound_tbl[ScratchWord[7]]].Workm, &ScratchPad->Vec, 0);  // spawn an effect

                FUNC(SND_SE_ENEM)(5, 0);  // enemy sound effect
                EM03_EF(Wound_cnt)++;
            }
        }

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        return;
    case 2:
        EM_ROUTINE_HALF0 = 1;  // SHORT view of the field (plain access changes the code)
        G.pEm->Routine_2 = (G.pEm->Type & 1) + 3;
        G.pEm->Routine_3 = 0U;
        G.pEm->Dest_x = G.Player.Pos_x;
        G.pEm->Dest_z = G.Player.Pos_z;
        G.pEm->Damage_no = 0U;
        if (G.pEm->Free[1] & 1)
        {
            G.pEm->Routine_1 = 1;
            G.pEm->Routine_2 = 0xA;
        }
        return;
    }
}

VOID Em03_Die_burst(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Spd_cur = 0;
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Timer0 = 0x1EU;
        G.pEm->Hokan_flg = 3;
        pP_ptr = G.pEm->pSin_parts_ptr;
        FUNC(JOINT_ATCK_SET)(pP_ptr, 0x1EU, 0x1E, 3);  // tint a joint for the hit effect
        FUNC(JOINT_ATCK_SET)(&pP_ptr[19], 0x1EU, 0x1E, 3);  // tint a joint for the hit effect
        ScratchPad->Vec.vz = 0;
        ScratchPad->Vec.vy = 0;
        ScratchPad->Vec.vx = 0;

        FUNC(ESP_CALL)(0, 8U, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);  // spawn an effect
        FUNC(ESP_CALL)(0, 8U, 0, &pP_ptr[19].Workm, &ScratchPad->Vec, 0);  // spawn an effect

        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);  // set the enemy flag bit

        FUNC(SND_SE_ENEM)(5, 0U);  // enemy sound effect
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

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        return;
    case 2:
        FUNC(KAGE_WORK_COLOR_SET)(&G.pEm->Kage_work, 1U, 2, 0xDF809F);  // set the shadow colour
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, -0x64U, -0x64);  // resize the shadow
        G.pEm->Routine_3 = 3U;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_OBJ;
        G.pEm->Spd_cur = 0;
        G.pEm->Timer0 = 0x5AU;

        G.pEm->pAt_tbl = pEm03_sca_dead;

    case 3:
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, 3U, 3);  // resize the shadow
        if (--G.pEm->Timer0 == 0)
        {
            G.pEm->Routine_3 = 6U;
            FUNC(SET_ENEMY_WORK)(((G.pEm->Damage_no >> 3) & 0xFF) * 0x2 + 8, 0xF2U, G.pEm->Parts_num, &G.pEm->pTgt);  // spawn a copy of the enemy
        }
        return;
    case 6:
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);  // set the enemy flag bit
        FUNC(EM_WEB_PROC)(((G.pEm->Damage_no >> 3) & 0xFF) * 2 + 8);  // update the webs
        break;
    }
}

VOID Em03_Die_flip(VOID)
{
    PARTS_W* pP_ptr;

    if (G.pEm->Routine_3 == 1 || G.pEm->Routine_3 == 3)
    {
        if (G.pEm->Damage_no & 2)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            FUNC(JOINT_ATCK_SET)(pP_ptr, 0x1EU, 0x1E, 3);  // tint a joint for the hit effect
            FUNC(JOINT_ATCK_SET)(&pP_ptr[19], 0x1EU, 0x1E, 3);  // tint a joint for the hit effect
            ScratchPad->Vec.vz = 0;
            ScratchPad->Vec.vy = 0;
            ScratchPad->Vec.vx = 0;

            FUNC(ESP_CALL)(0, 8U, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);  // spawn an effect
            FUNC(ESP_CALL)(0, 8U, 0, &pP_ptr[19].Workm, &ScratchPad->Vec, 0);  // spawn an effect

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

        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);  // set the enemy flag bit

    case 1:
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        if (G.pEm->Move_cnt >= 0xEU)
        {
            G.pEm->Be_flg = G.pEm->Be_flg | 0x20;
        }
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        return;
    case 2:
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_OBJ;
        G.pEm->Move_no = 5;
        G.pEm->Routine_3 = 3U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Damage_no = 0U;
        VIEW(USHORT, G.pEm->Kage_work.Prim[0].r0) = 0x809F;  // USHORT view of the field (plain access changes the code)
        VIEW(USHORT, G.pEm->Kage_work.Prim[1].r0) = 0x809F;  // USHORT view of the field (plain access changes the code)
        G.pEm->Kage_work.Prim[0].b0 = 0xDF;
        G.pEm->Kage_work.Prim[1].b0 = 0xDF;

    case 3:
        G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
        G.pEm->Be_flg = G.pEm->Be_flg | 0x20;
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        return;
    case 4:
        G.pEm->Move_no = 5;
        G.pEm->Routine_3 = 3U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 1;
        FUNC(SND_SE_ENEM)(5, 0U);  // enemy sound effect
        FUNC(KAGE_WORK_COLOR_SET)(&G.pEm->Kage_work, 1U, 2, 0xDF809F);  // set the shadow colour
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, -0x64U, -0x64);  // resize the shadow
        G.pEm->Routine_3 = 5U;
        G.pEm->Timer0 = 0x78U;
        G.pEm->Spd_cur = 0;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_OBJ;

        G.pEm->pAt_tbl = pEm03_sca_dead;

    case 5:
        FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, 3U, 3);  // resize the shadow
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);  // advance the pose animation
        if (--G.pEm->Timer0 == 0)
        {
            G.pEm->Routine_3 = 6U;
            FUNC(SET_ENEMY_WORK)(((G.pEm->Damage_no >> 3) & 0xFF) * 0x2 + 4, 0xF2U, G.pEm->Parts_num, &G.pEm->pTgt);  // spawn a copy of the enemy
        }
        return;
    case 6:
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg[0], G.pEm->Em_set_flg);  // set the enemy flag bit
        FUNC(EM_WEB_PROC)(((G.pEm->Damage_no >> 3) & 0xFF) * 2 + 4);  // update the webs
        break;
    }
}

VOID Em03_Leg_spd_set(UCHAR Flg, INT scale)
{
    PARTS_W* pP_ptr;
    UCHAR i;

    pP_ptr = G.pEm->pSin_parts_ptr;
    FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);  // rotate the matrix
    FUNC(GS_MUL_COORD0)(G.pEm->M, &pP_ptr->Coord, &ScratchPad->Mat);  // concatenate the matrices
    if (scale != 0)
    {
        ScratchPad->Vec.vx = ScratchPad->Vec.vy = ScratchPad->Vec.vz = scale;
        FUNC(SCALE_MATRIX)(&ScratchPad->Mat, &ScratchPad->Vec);  // scale the matrix
    }
    pP_ptr = &pP_ptr[Flg * 8 + 4];
    if (!(pP_ptr[-1].Be_flg & 1))
    {
        G.pEm->Spd_cur = 0;
        return;
    }
    i = 2;
    while (i != 0)
    {
        FUNC(GS_MUL_COORD3)(&ScratchPad->Mat, &(pP_ptr - i)[1].Coord);  // concatenate the matrices
        i--;
    }
    ScratchPad->Mat.t[0] = ScratchPad->Mat.t[0] - pP_ptr->Workm.t[0];
    ScratchPad->Mat.t[1] = 0;
    ScratchPad->Mat.t[2] = ScratchPad->Mat.t[2] - pP_ptr->Workm.t[2];
    FUNC(SQUARE0)(&ScratchPad->Mat.t, &ScratchPad->Vec);  // square of the value
    G.pEm->Spd_cur = FUNC(SQUARE_ROOT0)(ScratchPad->Vec.vx + ScratchPad->Vec.vz);  // integer square root
}

static AT_TBL Em03_sca_tbl00 = { 0x8000, 0, -180, 0, 0xB4, 0x3E8 };    // collision volume of the living spider: flags 0x8000, y offset -0xB4, half height 0xB4, radius 0x3E8

static AT_TBL Em03_sca_tbl01 = { 0x8000, 0, 0, 0, 0, 0x1F4 };    // collision volume of the dead spider: flags 0x8000, no offset or height, radius 0x1F4

AT_TBL* pEm03_sca = &Em03_sca_tbl00;  // collision volume of the living spider

AT_TBL* pEm03_sca_dead = &Em03_sca_tbl01;  // collision volume of the dead spider

// start health, indexed by a random nibble
UCHAR Em03_life[16] =
{
    0x77,   // 0 //
    0x63,   // 1 //
    0x63,   // 2 //
    0x63,   // 3 //
    0x77,   // 4 //
    0x63,   // 5 //
    0x63,   // 6 //
    0x77,   // 7 //
    0x63,   // 8 //
    0x63,   // 9 //
    0x63,   // 10 //
    0x77,   // 11 //
    0x63,   // 12 //
    0x59,   // 13 //
    0x63,   // 14 //
    0x77,   // 15 //
};

// state handlers by entity state (0=init, 1=run, 2=damaged, 3=death, 4 and 5=empty)
VOID (*Em03_routine_0[6])(VOID) =
{
    Em03_Init,      // 0 //
    Em03_Move,       // 1 //
    Em03_Damage,   // 2 //
    Em03_Die,     // 3 //
    Em03_Dead,      // 4 //
    Em03_Nop5,      // 5 //
};

// behavior pick routines by Type (0 to 3)
VOID (*Em03_pick_tbl[4])(VOID) =
{
    Em03_Pick_floor0,    // 0 //
    Em03_Pick_floor1,    // 1 //
    Em03_Pick_ceiling,   // 2 //
    Em03_Pick_ceiling,   // 3 //
};

// next action by path flag bit 0 (row) and a random value 0 to 7; rows 2 and 3 are never indexed
UCHAR Em03_mv_next_tbl[4][8] =
{
    { 0x02, 0x01, 0x02, 0x02, 0x02, 0x01, 0x02, 0x05 },   // 0 //
    { 0x0B, 0x0B, 0x05, 0x0B, 0x03, 0x05, 0x04, 0x04 },   // 1 //
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 2 //
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 3 //
};

// idle routines by Type (0 to 3)
VOID (*Em03_mv00_tbl[4])(VOID) =
{
    Em03_mv00_pick,   // 0 //
    Em03_mv00_hold,        // 1 //
    Em03_mv00_hold,        // 2 //
    Em03_mv00_hold,        // 3 //
};

// step sound by animation frame (0 for none)
UCHAR Em03_step_se_tbl[16] =
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
UCHAR Em03_wound_tbl[8] =
{
    0x03,   // 0 //
    0x05,   // 1 //
    0x07,   // 2 //
    0x09,   // 3 //
    0x0B,   // 4 //
    0x0D,   // 5 //
    0x0F,   // 6 //
    0x11,   // 7 //
};

UCHAR Em03_wound_use[8] = { 0 };    // pSin_parts_ptr that already show a wound (0 for a free slot)
