/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM0F.C
 */

#include "EM0F.H"
#include "SCRATCHPAD.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Em0f_Main(VOID)
{
    PARTS_W* pP_ptr;

    pP_ptr = G.pEm->pSin_parts_ptr;
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        Em0f_routine_0[G.pEm->Routine_0]();

        G.pEm->pSca_ofs->Off_x = pP_ptr[11].Workm.t[0] - G.pEm->Pos_x;
        G.pEm->pSca_ofs->Off_y = pP_ptr[11].Workm.t[1] - G.pEm->Pos_y;
        G.pEm->pSca_ofs->Off_z = pP_ptr[11].Workm.t[2] - G.pEm->Pos_z;
    }
    G.pEm->Coord_flg = 0;
    G.pEm->Zone_in_flg = FUNC(HIT_CK_POINT4)(&G.pEm->Pos_x, G.Cccut);  // Hit_ck_point4
    if (G.pEm->Zone_in_flg != 0 && (((USHORT)EM0F_EF(Kage_flg)) & 1))  // truncates to an unsigned halfword
    {
        FUNC(MUL_KAGE)(pP_ptr[11].Workm.t, &G.pEm->Kage_work, 0, G.pEm->Cdir_y);  // MulKage
    }
}

VOID Em0f_Init(VOID)
{
    UINT i;
    PARTS_W* pP_ptr;

    EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
    G.pEm->Life = 1;
    G.pEm->pAt_tbl = &Em0f_sca;
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT | EM_BE_NO_OBJ;
    G.pEm->Move_no = 0;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0;
    FUNC(INIT_PARTS_WORK)();  // Init_Parts_work
    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0;
    ScratchWord[7] = 0x404040;
    FUNC(KAGE_WORK_INIT)(&ScratchPad->Sv, &G.pEm->Kage_work, 0x1F4, 0x64);  // Kage_work_init
    EM0F_EF(Kage_flg) = 1;
    EM0F_EF(attack_cooldown) = 0;
    EM0F_EF(Idle_cnt) = 0;
    EM0F_EF(First_flg) = 1;

    EM0F_EF(hit_count) = 0;

    FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);  // Joint_move
    Em0f_side_old = 0;

    if (G.pEm->Type & 2)
    {
        pP_ptr = G.pEm->pSin_parts_ptr;
        i = 0;
        while (i < 0xFU)
        {
            pP_ptr->Be_flg = pP_ptr->Be_flg & 0xFE;
            pP_ptr++;
            i++;
        }

        G.pEm->Life = -1;

        EM0F_EF(Kage_flg) = 0;
    }
}

VOID Em0f_Move(VOID)
{
    UCHAR Type_flg;

    Type_flg = G.pEm->Type;
    if (Type_flg & 0x40)
    {
        G.pEm->Routine_0 = 3;
        G.pEm->Routine_1 = 0U;
        return;
    }
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    if (Type_flg != 0 && Type_flg != 1)
    {
    }
    else
    {
        FUNC(EM_CK_HANI)(0x1388);  // Em_ck_hani
    }
    if (Type_flg != 3 && Type_flg != 4)
    {
    }
    else
    {
        G.pEm->Be_flg = G.pEm->Be_flg | 0xC0;
        FUNC(EM_CK_HANI)(0x1388);  // Em_ck_hani
    }
    EM0F_FREE_P->player_dist = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);
    if (G.pEm->Routine_1 == 0)
    {
        EM_ROUTINE_HALF2 = 0;
        Em0f_Type_sel();
    }
    Em0f_Move_type[G.pEm->Routine_1]();

    EM0F_EF(saved_state) = EM_ROUTINE_WORD;
}

VOID Em0f_Type_sel(VOID)
{
    if (G.pEm->Type & 0x80)
    {
        Em0f_watch_set();
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_OBJ;
    }
    else
    {
        Em0f_set_tbl[(G.pEm->Type & 0xF)]();
    }
    EM_ROUTINE_HALF2 = 0;
}

VOID Em0f_watch_set(VOID)
{
    G.pEm->Routine_1 = 1;
    G.pEm->Be_flg = G.pEm->Be_flg & ~EM_BE_NO_OBJ;
    EM0F_EF(First_flg) = 1;
}

VOID Em0f_hidden_set(VOID)
{
    UINT i;
    PARTS_W* pP_ptr;

    G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_OBJ;
    G.pEm->Routine_1 = 1;
    G.pEm->Move_no = G.Player.Id & 1 ? 0x10 : 0xF;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0;
    EM0F_EF(Kage_flg) = 0;
    EM0F_EF(Side_no) = 0;
    pP_ptr = G.pEm->pSin_parts_ptr;
    i = 0;
    while (i < 0xFU)
    {
        pP_ptr->Be_flg = pP_ptr->Be_flg & 0xFE;
        pP_ptr++;
        i++;
    }
}

VOID Em0f_catch_set(VOID)
{
    G.pEm->Routine_1 = 2;
    G.pEm->Move_no = 1;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0x1F;
}

VOID Em0f_strike_set(VOID)
{
    G.pEm->Routine_1 = 3;
    if (G.pEm->Cdir_y >= 0x800 && G.pEm->Cdir_y < 0x1000)
    {
        G.pEm->Move_no = 2;
    }
    else
    {
        G.pEm->Move_no = 9;
    }
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 7;
    G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
    FUNC(SND_SE_ENEM)(0, 0);  // Snd_se_enem
}

VOID Em0f_ambush_set(VOID)
{
    UINT i;
    PARTS_W* pP_ptr;
    INT unused;    // unused local: reserves stack space

    G.pEm->Routine_1 = 4;
    G.pEm->Move_no = G.Player.Id & 1 ? 0x10 : 0xF;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0;
    pP_ptr = G.pEm->pSin_parts_ptr;
    i = 0;
    while (i < 0xFU)
    {
        pP_ptr->Be_flg = pP_ptr->Be_flg & 0xFE;
        pP_ptr++;
        i++;
    }
    EM0F_EF(vine_segment) = 0xE;
    G.pEm->Timer0 = 0;
    EM0F_EF(timer) = 2;
    EM0F_EF(First_flg) = 1;
    EM0F_EF(Dir_bak) = G.pEm->Cdir_y;
    G.pEm->Cdir_y = FUNC(GET_DIR_TGT)(G.Player.Pos_x, G.Player.Pos_z);  // Get_dir_tgt
    Em0f_Catch_seize();
    FUNC(SND_SE_ENEM)(2, 0);  // Snd_se_enem
}

VOID Em0f_dead_set(VOID)
{
    G.pEm->Routine_1 = 5;
    G.pEm->Life = -1;
    G.pEm->Move_no = (!(Api_rand() & 1)) ? 7 : 0;  // rand
    G.pEm->Move_cnt = Api_rand() & 0xF;  // rand
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0;
    G.pEm->Timer0 = 1;
    if (G.pEm->Type == 7)
    {
        FUNC(CLUT_FADE_SET)(2, -3, 0, 0, 0x100, G.pEm->Id);  // Cinesco_fade_set
    }
}

VOID Em0f_sway_set(VOID)
{
    UCHAR side;

    G.pEm->Routine_1 = 6;
    side = Api_rand() & 1;  // rand
    if ((Em0f_side_old & 3) == side * 3)
    {
        side ^= 1;
    }
    Em0f_side_old = ((Em0f_side_old << 1) & ~1) | side;
    G.pEm->Move_no = side != 0 ? 0x11 : 0x13;
    G.pEm->Move_cnt = (Api_rand() & 7) * 3;  // rand
    if (G.pEm->Move_no == 0x11)
    {
        G.pEm->Move_cnt = G.pEm->Move_cnt + (Api_rand() & 7) * 3;  // rand
    }
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0x1F;
    EM0F_EF(Idle_cnt) = 0;
    G.pEm->Timer0 = 1;
    if (G.pEm->Type == 0x29)
    {
        EM0F_EF(timer) = 0x28;
        EM0F_EF(Fade_timer) = 0x28;
        EM0F_EF(Dir_bak) = __builtin_abs(2);
        EM0F_EF(Side_no) = 3;
        FUNC(SND_SE_ENEM)(4U, 0U);  // Snd_se_enem
    }
}

VOID Em0f_Move_watch(VOID)
{
    if (G.pEm->Type & 0x20)
    {
        G.pEm->Routine_1 = 0;
        return;
    }
    if (EM0F_EF(Idle_cnt) == 0)
    {
        G.pEm->Move_no = (!(Api_rand() & 1)) ? 7 : 0;  // rand
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        EM0F_EF(Idle_cnt) = Api_rand() & 0x1F;  // rand
        return;
    }
    EM0F_EF(Idle_cnt)--;
    if (G.pEm->Type != 0x80)
    {
        Em0f_watch_tbl[(G.pEm->Type & 0xF)]();
    }
    if (G.pEm->Type != 2)
    {
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80) != 0)  // Joint_move
        {
            G.pEm->Routine_1 = 0;
        }
    }
    if (PL_ROUTINE_WORD == ((0) << 24 | (0x14) << 16 | (3) << 8 | (1)))
    {
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80) != 0)  // Joint_move
        {
            G.pEm->Routine_1 = 0;
        }
    }
}

VOID Em0f_catch_ck(VOID)
{
    if (EM0F_EF(player_dist) < 0xA8D)
    {
        G.pEm->Routine_1 = 0;
        G.pEm->Type = 3;
    }
}

VOID Em0f_strike_ck(VOID)
{
    INT range;

    if (EM0F_EF(attack_cooldown) != 0)
    {
        EM0F_EF(attack_cooldown)--;
        return;
    }
    range = EM0F_EF(First_flg) != 0 ? 0xBB8 : 0x9C4;
    if (EM0F_EF(player_dist) <= range && G.Player.Damage_no == 0)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400) == 0)  // Goto00_ck
        {
            G.pEm->Routine_1 = 0;
            G.pEm->Type = 4;
        }
    }
}

VOID Em0f_ambush_ck(VOID)
{
    if (G.pEm->Type != 2)
    {
        G.pEm->Routine_1 = 0;
        return;
    }
    if (EM0F_EF(player_dist) >= 0xFA1)
    {
        EM0F_EF(Side_no) = 0U;
        EM0F_EF(attack_cooldown) = 0;
        return;
    }
    if (EM0F_EF(player_dist) < 0xC45)
    {
        if (EM0F_EF(Side_no) == 0)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200) == 0)  // Goto00_ck
            {
                EM0F_EF(Side_no) = 2U;
            }
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x600) != 0)  // Goto00_ck
            {
                EM0F_EF(Side_no) = 1U;
            }
            return;
        }
        if (G.Player.Damage_no == 0)
        {
            EM0F_EF(Dir_bak) = G.pEm->Cdir_y;
            G.pEm->Cdir_y = G.pEm->Cdir_y + ((((SHORT)EM0F_EF(Side_no)) - 1) << 0xB);  // sign-extends the halfword
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200) == 0)  // Goto00_ck
            {
                G.pEm->Routine_1 = 0;
                G.pEm->Type = 5U;
            }
            G.pEm->Cdir_y = EM0F_EF(Dir_bak);
        }
    }
}

VOID Em0f_Move_catch(VOID)
{
    if (G.pEm->Type & 0x20)
    {
        G.pEm->Routine_1 = 0;
        return;
    }
    EM0F_EF(Mot_end) = FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);  // Joint_move
    Em0f_catch_tbl[G.pEm->Routine_2]();
}

VOID Em0f_Catch_wait(VOID)
{
    if (EM0F_EF(Mot_end) != 0 && G.pEm->Move_no == 1)
    {
        G.pEm->Move_no = 8U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0xF;
    }
    if (G.pEm->Move_no != 1)
    {
        Em0f_Idle_set();
        if (G.Player.Routine_2 == 0x14)
        {
            FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);  // Joint_move
        }
    }
    if (EM0F_EF(player_dist) >= 0x2135)
    {
        G.pEm->Routine_1 = 0;
        G.pEm->Type = 0;
    }
    if (EM0F_EF(attack_cooldown) != 0)
    {
        EM0F_EF(attack_cooldown)--;
        return;
    }
    if (EM0F_EF(player_dist) < 0x708 && G.Player.Damage_no == 0)
    {
        G.pEm->Routine_2++;
        G.pEm->Move_no = G.Player.Id & 1 ? 0xA : 4;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0;
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200) != 0)  // Goto00_ck
        {
            G.pEm->Routine_2 = 4U;
            EM0F_EF(timer) = 2;
            return;
        }
    }
}

VOID Em0f_Catch_seize(VOID)
{
    G.pEm->Routine_2++;
    G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
    G.Player.Move_no = FUNC(CDIR_CK)(&MAIN.Player) + 2;  // Cdir_ck
    G.Player.Cdir_y = G.pEm->Cdir_y;
    FUNC(SET_BASE_POS)(&MAIN.Player);  // Set_base_pos
    G.Player.Damage_no = 1;
    G.Player.Routine_0 = 6;
    G.Player.Routine_1 = 0xF;
    G.Player.Routine_2 = 0;
    G.Player.Routine_3 = 0;
    G.Player.Be_flg |= EM_BE_NO_HIT;
    FUNC(SND_SE_ENEM)(2, 0);  // Snd_se_enem

    FUNC(PAD_VIB_SET0)(8, 0);  // Pad_vib_set0
    FUNC(PAD_VIB_FADE_SET)(0x14, 0x50, 0x82, 0);  // Pad_vib_fade_set
}

VOID Em0f_Catch_hold(VOID)
{
    EM0F_EF(Dir_bak) = G.pEm->Cdir_y;
    FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x80);  // Goto00
    if (EM0F_EF(Mot_end) != 0)
    {
        G.pEm->Routine_2++;
        G.pEm->Move_no++;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0;
        G.pEm->Timer0 = 0x3C;
        EM0F_EF(timer) = 0xC;
        FUNC(SND_SE_ENEM)(3, 0);  // Snd_se_enem

        if ((G.Player.Id & 3) != 3)
        {
            FUNC(SND_SE_STAD)(2, (G.Player.Id & 1) + 0x17, 0, &MAIN.Player.Pos_x);  // Snd_se_stad
            return;
        }
        FUNC(SND_SE_STAD)(3, 0, 0, &MAIN.Player.Pos_x);  // Snd_se_stad
    }
}

VOID Em0f_Catch_squeeze(VOID)
{
    if (--EM0F_EF(timer) == 0)
    {
        G.Player.Life -= 2;

        if (G.pEm->Type == 5 && G.Player.Life < 0)
        {
            G.Player.Life = 0;
        }

        EM0F_EF(timer) = 0xCU;
    }
    if (G.pEm->Timer0 > 0 && G.Player.Life >= 0)
    {
        G.pEm->Timer0 -= FUNC(GACHA_CH)() != 0 ? 4 : 1;  // Gacha_ch
    }
    else if (EM0F_EF(Mot_end) != 0)
    {
        G.pEm->Routine_2++;
        G.pEm->Move_no++;
        G.pEm->Move_wait = 0;
        G.Player.Routine_3 = 2;

        FUNC(PAD_VIB_FADE_SET)(0x19U, 0x96, 0, 0);  // Pad_vib_fade_set
    }
}

VOID Em0f_Catch_end(VOID)
{
    if (EM0F_EF(Mot_end) != 0)
    {
        G.pEm->Routine_1 = 0;
        G.pEm->Type = 0;
        G.pEm->Be_flg = G.pEm->Be_flg & ~EM_BE_NO_HIT;
        EM0F_EF(attack_cooldown) = 0x1E;
        G.pEm->Cdir_y = EM0F_EF(Dir_bak);
    }
}

VOID Em0f_Catch_lash(VOID)
{
    if (EM0F_EF(Mot_end) != 0)
    {
        if (--EM0F_EF(timer) == 0)
        {
            if (EM0F_EF(player_dist) < 0xA8D)
            {
                G.pEm->Routine_2 = 0;
                G.pEm->Move_no = 8U;
                G.pEm->Move_cnt = 0;
                G.pEm->Move_wait = 0;
                G.pEm->Hokan_flg = 0xF;
            }
            else
            {
                G.pEm->Routine_1 = 0;
                G.pEm->Type = 3;
            }
            EM0F_EF(attack_cooldown) = 0x1E;
            return;
        }
        G.pEm->Move_no = G.pEm->Move_no + 2;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0xF;
        FUNC(SND_SE_ENEM)(0, 0);  // Snd_se_enem
    }
}

VOID Em0f_Move_strike(VOID)
{
    if (G.pEm->Type & 0x20)
    {
        G.pEm->Routine_1 = 0;
        return;
    }
    Em0f_strike_tbl[G.pEm->Routine_2]();
}

VOID Em0f_Strike_swing(VOID)
{
    UCHAR Hit_dir;

    EM0F_EF(Mot_end) = FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);  // Joint_move
    if (EM0F_EF(Mot_end) != 0)
    {
        G.pEm->Routine_2++;
        EM0F_EF(Idle_cnt) = 0;
        EM0F_EF(First_flg) = 0;
    }
    if (G.pEm->Move_cnt == 6 && G.Player.Damage_no == 0)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400) == 0)  // Goto00_ck
        {
            G.Player.Routine_3 = 0;
            Hit_dir = (SHORT)G.Player.Cdir_y < 0x401 || (SHORT)G.Player.Cdir_y >= 0xC00;  // sign-extends the halfword
            G.Player.Damage_no = Hit_dir + 1;
            G.Player.Routine_2 = Hit_dir + 0x66;

            FUNC(PAD_VIB_SET1)(5, 0x64, 0);  // Pad_vib_set1
            FUNC(PAD_VIB_SET0)(3, 0);  // Pad_vib_set0

            G.Player.Life -= 2;
            if (G.Player.Life < 0)
            {
                G.Player.Life = 0;
            }

            FUNC(SND_SE_ENEM)(1, 0);  // Snd_se_enem
            EM0F_EF(attack_cooldown) = 1;
        }
    }
}

VOID Em0f_Strike_recover(VOID)
{
    Em0f_Idle_set();
    Em0f_strike_ck();
    if (EM0F_EF(player_dist) >= 0x2134)
    {
        G.pEm->Routine_1 = 0;
        G.pEm->Type = 1;
    }
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);  // Joint_move
    if (G.Player.Routine_2 == 0x14)
    {
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);  // Joint_move
    }
}

VOID Em0f_Move_ambush(VOID)
{
    EM0F_EF(Mot_end) = FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);  // Joint_move
    Em0f_ambush_tbl[G.pEm->Routine_2]();
}

VOID Em0f_Ambush_extend(VOID)
{
    SHORT seg;
    PARTS_W* pP_ptr;

    if (EM0F_EF(timer) == 0 && G.pEm->Timer0 < 4)
    {
        pP_ptr = G.pEm->pSin_parts_ptr;
        pP_ptr = pP_ptr + (EM0F_EF(vine_segment));
        seg = EM0F_EF(vine_segment);
        while (seg >= Em0f_extend_tbl[G.pEm->Timer0])
        {
            pP_ptr->Be_flg = pP_ptr->Be_flg | 1;
            pP_ptr--;
            seg--;
        }
        EM0F_EF(vine_segment) = seg;
        G.pEm->Timer0++;
    }
    else
    {
        EM0F_EF(timer)--;
    }
    if (EM0F_EF(Mot_end) != 0)
    {
        G.pEm->Routine_2++;
        G.pEm->Move_no = G.Player.Id & 1 ? 0xB : 5;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0;

        G.Player.Be_flg |= EM_BE_NO_OBJ;

        G.pEm->Timer0 = 0x3C;
        EM0F_EF(timer) = 0xC;
        FUNC(SND_SE_ENEM)(3, 0);  // Snd_se_enem

        if ((G.Player.Id & 3) != 3)
        {
            FUNC(SND_SE_STAD)(2, (G.Player.Id & 1) + 0x17, 0, &MAIN.Player.Pos_x);  // Snd_se_stad
            return;
        }
        FUNC(SND_SE_STAD)(3, 0, 0, &MAIN.Player.Pos_x);  // Snd_se_stad
    }
}

VOID Em0f_Ambush_retract_set(VOID)
{
    G.pEm->Routine_2++;
    G.pEm->Move_no = 0x12;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0;
    EM0F_EF(vine_segment) = 0;
    G.pEm->Timer0 = 0;
}

VOID Em0f_Ambush_retract(VOID)
{
    SHORT seg;
    PARTS_W* pP_ptr;

    if (G.pEm->Move_cnt >= 3U && EM0F_EF(vine_segment) < 0xF)
    {
        pP_ptr = G.pEm->pSin_parts_ptr;
        pP_ptr = pP_ptr + (EM0F_EF(vine_segment));
        seg = EM0F_EF(vine_segment);
        while (seg <= Em0f_retract_tbl[G.pEm->Timer0])  // sign-extends the halfword
        {
            pP_ptr->Be_flg = pP_ptr->Be_flg & 0xFE;
            pP_ptr++;
            seg++;
        }
        EM0F_EF(vine_segment) = seg;
        G.pEm->Timer0++;
    }
    if (EM0F_EF(Mot_end) != 0)
    {
        G.pEm->Routine_1 = 0;
        if (G.pEm->Type != 0x80)
        {
            G.pEm->Type = 2U;
        }
        G.pEm->Cdir_y = EM0F_EF(Dir_bak);
        EM0F_EF(Side_no) = 0;
        EM0F_EF(attack_cooldown) = 0;
    }
}

VOID Em0f_Move_dead(VOID)
{
    G.pEm->Life = -1;

    if (G.pEm->Timer0 != 0)
    {
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x100);  // Joint_move
        G.pEm->Timer0--;
    }
}

VOID Em0f_Move_sway(VOID)
{
    INT unused;    // unused local: reserves stack space

    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80) != 0)  // Joint_move
    {
        if ((Api_rand() & 0x80) >= 0x7EU)  // rand
        {
            G.pEm->Move_no = Api_rand() & 1 ? 0x11 : 0x13;  // rand
            G.pEm->Hokan_flg = 0x1F;
        }
    }
    if (G.pEm->Type != 0x29)
    {
        return;
    }
    if (--EM0F_EF(timer) == 0)
    {
        if ((*(SHORT*)&EM0F_EF(Dir_bak)) != 0)  // SHORT* view of the field (a plain access changes the code)
        {
            FUNC(CLUT_FADE_SABUN_ADD)(1, 0, 0, 0, 0x100, G.pEm->Id);  // Cinesco_fade_sabun_add
            (*(SHORT*)&EM0F_EF(Dir_bak))--;  // SHORT* view of the field (a plain access changes the code)
        }
        EM0F_EF(timer) = 0x28U;
    }
    if (--EM0F_EF(Fade_timer) == 0)
    {
        if (EM0F_EF(Side_no) != 0)
        {
            FUNC(CLUT_FADE_SABUN_ADD)(0, -1, 0, 0, 0x100, G.pEm->Id);  // Cinesco_fade_sabun_add
            EM0F_EF(Side_no)--;
        }
        EM0F_EF(Fade_timer) = 0x28U;
    }
}

VOID Em0f_Damage(VOID)
{
    if ((++EM0F_EF(hit_count) & 0xFFFF) >= 4U)
    {
        FUNC(FLG_ON)(&MAIN.SaveData.Scenario_flg, 0x5B);  // Flg_on
    }
    EM_ROUTINE_WORD = EM0F_EF(saved_state);
    G.pEm->Damage_no = 0;
    Em0f_Move();
}

VOID Em0f_Die(VOID)
{
    if (G.pEm->Routine_1 >= 3U)
    {
        return;
    }
    Em0f_die_tbl[G.pEm->Routine_1]();
}

VOID Em0f_Die_begin(VOID)
{
    G.pEm->Hokan_flg = 0x1F;
    G.pEm->Routine_1 = 1;
    EM0F_EF(Idle_cnt) = 0;
    G.pEm->Timer0 = ((Api_rand()) & 0x1F) + (Api_rand() & 0x1F) + 1;  // rand
    G.pEm->Life = -1;
    Em0f_Die_writhe();
}

VOID Em0f_Die_writhe(VOID)
{
    if (G.pEm->Timer0 != 0)
    {
        if ((--G.pEm->Timer0) == 0)
        {
            G.pEm->Move_no = 0xE;
            G.pEm->Move_cnt = 0U;
            G.pEm->Move_wait = 0;
            G.pEm->Hokan_flg = 0x1F;
            goto fall_mot;
        }
        Em0f_Move_sway();
        return;
    }
    if (1)
    {
    fall_mot:
        if (FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80) != 0)  // Joint_move
        {
            G.pEm->Routine_1++;
            Api_rand();  // rand
            G.pEm->Move_no = (!(Api_rand() & 1)) ? (7) : (0);  // rand
            G.pEm->Move_cnt = Api_rand() & 0xF;  // rand
            G.pEm->Move_wait = 0;
            G.pEm->Hokan_flg = 0x1F;
            G.pEm->Timer0 = (Api_rand() & 0xF) + 0x32;  // rand
            EM0F_EF(timer) = 4;
            EM0F_EF(Fade_timer) = 8;
        }
    }
}

VOID Em0f_Die_fade(VOID)
{
    if ((Api_rand() & 0x3F) < (UINT)G.pEm->Timer0)  // rand
    {
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);  // Joint_move
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);  // Joint_move
    }
    if (--G.pEm->Timer0 == 0)
    {
        G.pEm->Routine_1++;
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg, G.pEm->Em_set_flg);  // Flg_on
    }
    if (G.pEm->Type & 1)
    {
        if (--EM0F_EF(timer) == 0)
        {
            FUNC(CLUT_FADE_SABUN_ADD)(1, 0, 0, 0, 0x100, G.pEm->Id);  // Cinesco_fade_sabun_add
            EM0F_EF(timer) = 4U;
        }
        if (--EM0F_EF(Fade_timer) == 0)
        {
            FUNC(CLUT_FADE_SABUN_ADD)(0, 1, 0, 0, 0x100, G.pEm->Id);  // Cinesco_fade_sabun_add
            EM0F_EF(Fade_timer) = 8U;
        }
    }
}

VOID Em0f_Dead(VOID)
{
}

VOID Em0f_Move_nop(VOID)
{
}

VOID Pl_Em0f_hold(VOID)
{
    Pl_Em0f_hold_tbl[G.Player.Routine_3]();
}

VOID Pl_Em0f_hold_start(VOID)
{
    G.Player.Routine_3++;
    G.Player.Move_cnt = 0;
    G.Player.Move_wait = 0;
    G.Player.Hokan_flg = 0;
    FUNC(NULL_POS_SET)(&MAIN.Player, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr);  // Null_pos_set
}

VOID Pl_Em0f_hold_squeezed(VOID)
{
    FUNC(NULL_POS_SET)(&MAIN.Player, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr);  // Null_pos_set
    FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x80);  // Joint_move
    if (G.Player.Move_cnt >= 0x19U)
    {
        G.Player.Move_cnt = 0xA;

        FUNC(PAD_VIB_FADE_SET)(0xA, 0x50, 0x82, 0);  // Pad_vib_fade_set
        FUNC(PAD_VIB_FADE_SET)(0xA, 0x82, 0x50, 0xA);  // Pad_vib_fade_set
    }
}

VOID Pl_Em0f_hold_release(VOID)
{
    FUNC(NULL_POS_SET)(&MAIN.Player, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr);  // Null_pos_set
    if (G.Player.Move_cnt != 0x27 && G.Player.Life >= 0)
    {
    }
    else
    {
        PL_ROUTINE_HALF0 = 1;
        PL_ROUTINE_HALF2 = 0;
        G.Player.Damage_no = 0;
        if ((UCHAR)G.Player.Move_no == 2)  // truncates to a byte
        {
            G.Player.Cdir_y += 0x800;
        }

        G.Player.Be_flg &= 0xF5;
    }
    FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x80);  // Joint_move
}

VOID Em0f_Idle_set(VOID)
{
    if (EM0F_EF(Idle_cnt) == 0)
    {
        G.pEm->Move_no = Api_rand() & 1 ? 8 : 3;  // rand
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        EM0F_EF(Idle_cnt) = Api_rand() & 0x1F;  // rand
        if (G.pEm->Move_no == 3 && G.pEm->Move_cnt >= 0x14U)
        {
            G.pEm->Move_cnt = G.pEm->Move_cnt + 0xF6;
        }
        return;
    }
    EM0F_EF(Idle_cnt)--;
}

VOID Em0f_Nop5(VOID)
{
}

// collision info of the plant: flags, offset x/y/z, half height, radius
AT_TBL Em0f_sca =
{
    0x8000,
    0,
    0,
    0,
    0,
    0x0190,
};

INT Em0f_side_old = 0;  // sway sides chosen so far, newest in bit 0

// state handlers, indexed by the enemy state
VOID (*Em0f_routine_0[5])(VOID) =
{
    Em0f_Init,          // 0 //
    Em0f_Move,   // 1 //
    Em0f_Damage,       // 2 //
    Em0f_Die,           // 3 //
    Em0f_Dead,    // 4 //
};

// sub-state handlers, indexed by the enemy sub-state
VOID (*Em0f_Move_type[7])(VOID) =
{
    Em0f_Move_nop,     // 0 //
    Em0f_Move_watch,    // 1 //
    Em0f_Move_catch,     // 2 //
    Em0f_Move_strike,   // 3 //
    Em0f_Move_ambush,   // 4 //
    Em0f_Move_dead,     // 5 //
    Em0f_Move_sway,     // 6 //
};

// functions that enter a sub-state, indexed by the behavior
VOID (*Em0f_set_tbl[10])(VOID) =
{
    Em0f_watch_set,    // 0 //
    Em0f_watch_set,    // 1 //
    Em0f_hidden_set,   // 2 //
    Em0f_catch_set,     // 3 //
    Em0f_strike_set,   // 4 //
    Em0f_ambush_set,   // 5 //
    Em0f_dead_set,     // 6 //
    Em0f_dead_set,     // 7 //
    Em0f_sway_set,     // 8 //
    Em0f_sway_set,     // 9 //
};

// watch range checks, indexed by the behavior
VOID (*Em0f_watch_tbl[3])(VOID) =
{
    Em0f_catch_ck,     // 0 //
    Em0f_strike_ck,   // 1 //
    Em0f_ambush_ck,   // 2 //
};

// grab steps
VOID (*Em0f_catch_tbl[6])(VOID) =
{
    Em0f_Catch_wait,          // 0 //
    Em0f_Catch_seize,         // 1 //
    Em0f_Catch_hold,          // 2 //
    Em0f_Catch_squeeze,       // 3 //
    Em0f_Catch_end,           // 4 //
    Em0f_Catch_lash,   // 5 //
};

// strike steps
VOID (*Em0f_strike_tbl[2])(VOID) =
{
    Em0f_Strike_swing,     // 0 //
    Em0f_Strike_recover,   // 1 //
};

// ambush steps
VOID (*Em0f_ambush_tbl[4])(VOID) =
{
    Em0f_Ambush_extend,          // 0 //
    Em0f_Catch_squeeze,           // 1 //
    Em0f_Ambush_retract_set,   // 2 //
    Em0f_Ambush_retract,         // 3 //
};

// vine joint index each extend stage runs down to
SHORT Em0f_extend_tbl[4] =
{
    0x0008,   // 0 //
    0x0004,   // 1 //
    0x0002,   // 2 //
    0x0000,   // 3 //
};

// vine joint index each retract stage runs up to
SHORT Em0f_retract_tbl[12] =
{
    0x0000,   // 0 //
    0x0000,   // 1 //
    0x0001,   // 2 //
    0x0002,   // 3 //
    0x0003,   // 4 //
    0x0004,   // 5 //
    0x0005,   // 6 //
    0x0007,   // 7 //
    0x0009,   // 8 //
    0x000C,   // 9 //
    0x000E,   // 10 //
    0x0000,   // 11 //
};

// death steps
VOID (*Em0f_die_tbl[3])(VOID) =
{
    Em0f_Die_begin,    // 0 //
    Em0f_Die_writhe,   // 1 //
    Em0f_Die_fade,     // 2 //
};

// player hold steps
VOID (*Pl_Em0f_hold_tbl[3])(VOID) =
{
    Pl_Em0f_hold_start,      // 0 //
    Pl_Em0f_hold_squeezed,   // 1 //
    Pl_Em0f_hold_release,    // 2 //
};
