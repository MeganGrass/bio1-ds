/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM0C.C
 */

#include "EM0C.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Em0c_Init(VOID)
{
    ENEMY_WORK* pSub;
    PARTS_W* pP_ptr;
    INT unused[4];    // unused local: reserves stack space

    G.pEm->Coord_flg = 0;
    G.pEm->Damage_no = 0;
    FUNC(INIT_PARTS_WORK)();
    ScratchWord[7] = 0x808080;
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vx = 0;
    FUNC(KAGE_WORK_INIT)(ScratchSvU, &G.pEm->Kage_work, 0x3E8U, 0x3E8);
    G.pEm->Life = 0xDC;
    if (G.pEm->Id == 0x10)
    {
        G.pEm->Life = 0x258;
    }
    G.pEm->pAt_tbl = pEm0c_sca;
    FUNC(SET_ENEMY_WORK)(1, 0x1388, G.pEm->Parts_num, &EM0C_FREE_P->heart);
    pSub = EM0C_FREE_P->heart;
    pSub->Cdir_x = 0;
    pSub->Cdir_y = 0;
    pSub->Cdir_z = 0;
    pSub->Pos_x = 0x138;
    pSub->Pos_y = -0x365;
    pSub->Pos_z = -0x11F;
    pSub->Old_x = 0x1000;
    pSub->Timer0 = 0x15;
    EM0C_FREE_P->uv_scroll_step = 0;
    EM0C_FREE_P->hit_mask = 0;
    EM0C_FREE_P->look_at_timer = 3;
    EM0C_FREE_P->Dummy178 = 3;
    EM0C_FREE_P->flags = 0;
    EM0C_FREE_P->attack_cooldown = 0;
    EM0C_FREE_P->rocket_flag = 0;
    EM0C_FREE_P->special_timer = 0xD2;
    EM0C_FREE_P->room_hit = 0;
    EM0C_FREE_P->Wander_rno = 0;

    EM0C_FREE_P->close_frames = 0;
    EM0C_FREE_P->frenzy_timer = 0;

    EM_ROUTINE_WORD = ROUTINES(0, 0xB, 0, 8);
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0;
    G.pEm->Move_no = 6;
    if (G.pEm->Id == 0x10)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 9, 1, 1);
    }
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x40);
    G.pEm->Neck_no = 2;
    G.pEm->Neck_flg = 2;
    G.pEm->Neck_spd_y = 0xA0;
    G.pEm->Neck_spd_x = 0x60;
    if (G.pEm->Id == 0xC)
    {
        return;
    }
    pEm0c_ghost = G.pFree_work;
    pP_ptr = G.pEm->pSin_parts_ptr;
    pP_ptr += 8;
#ifdef __INTELLISENSE__
    G.pFree_work = (UCHAR*)G.pFree_work + 2 * sizeof(PARTS_W);  // IntelliSense rejects void* arithmetic
#else
    G.pFree_work += 2 * sizeof(PARTS_W);
#endif
    ScratchWord[0] = 2;
    while ((ScratchWord[0] = ScratchWord[0] - 1) != -1)
    {
        pEm0c_ghost[ScratchWord[0]].Be_flg = pP_ptr->Be_flg;
        pEm0c_ghost[ScratchWord[0]].Workm = pP_ptr->Workm;
        pEm0c_ghost[ScratchWord[0]].pTmd = pP_ptr->pTmd;
        pEm0c_ghost[ScratchWord[0]].Attribute = pP_ptr->Attribute;
        pEm0c_ghost[ScratchWord[0]].Obj_id = pP_ptr->Obj_id;
        pEm0c_ghost[ScratchWord[0]].Coord_flg = pP_ptr->Coord_flg;
    }
    ScratchWord[2] = 2;
    while ((ScratchWord[2] = ScratchWord[2] - 1) != -1)
    {
        pEm0c_ghost[ScratchWord[2]].pPacket = G.pFree_work;
        G.pFree_work = (VOID*)FUNC(GS_PRESET_OBJECT)(&pEm0c_ghost[ScratchWord[2]].Attribute, G.pFree_work);  // untyped pointer, the callee takes the address as is
    }
    FUNC(MODEL_COLOR_PLUS)(pEm0c_ghost, 0xFF, 0xFFU, 0xFF);
    FUNC(MODEL_COLOR_PLUS)(pEm0c_ghost + 1, 0x30000, 0x300000U, 0x300000);
    pP_ptr = G.pEm->pSin_parts_ptr;
    pP_ptr += 8;
    pEm0c_trail = G.pFree_work;
    FUNC(EM0C_TRAIL_ALLOC)(9, pEm0c_trail, 0x70U);
    EM0C_FREE_P->attack_cooldown = 0x5A;
}

VOID Em0c_trail_move(VOID)
{
    PARTS_W* pP_ptr;

    pP_ptr = G.pEm->pSin_parts_ptr;
    pP_ptr += 8;
    FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);
    FUNC(GS_MUL_COORD0)(G.pEm->M, &pP_ptr[-8].Coord, &ScratchPad->Mat.m[0][0]);
    FUNC(GS_MUL_COORD3)(&ScratchPad->Mat, &pP_ptr[-7].Coord);
    FUNC(GS_MUL_COORD3)(&ScratchPad->Mat, &pP_ptr[-2].Coord);
    FUNC(GS_MUL_COORD3)(&ScratchPad->Mat, &pP_ptr[-1].Coord);
    FUNC(GS_MUL_COORD3)(&ScratchPad->Mat, &pP_ptr->Coord);
    FUNC(EM0C_TRAIL_PUSH)(pEm0c_trail, &pP_ptr->Workm, &ScratchPad->Mat.m[0][0], &Em0c_trail_near, &Em0c_trail_far, 0, Em0c_trail_timer.segments);
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        if (Em0c_trail_timer.timer < Em0c_trail_timer.segments)
        {
            Em0c_trail_timer.segments--;
        }
        Em0c_trail_timer.timer--;
    }
}

VOID Em0c_draw_claw_ghosts(VOID)
{
    PARTS_W* pP_ptr;
    MATRIX mtx;

    if (G.pEm->Id == 0xC)
    {
        return;
    }
    pP_ptr = G.pEm->pSin_parts_ptr;
    pP_ptr += 8;
    ScratchPtr[14] = (VOID*)pP_ptr->pTmd;  // untyped pointer, the callee takes the address as is
    FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);
    FUNC(GS_MUL_COORD0)(G.pEm->M, &pP_ptr[-8].Coord, &ScratchPad->Mat);
    FUNC(GS_MUL_COORD3)(&ScratchPad->Mat, &pP_ptr[-7].Coord);
    FUNC(GS_MUL_COORD3)(&ScratchPad->Mat, &pP_ptr[-2].Coord);
    FUNC(GS_MUL_COORD3)(&ScratchPad->Mat, &pP_ptr[-1].Coord);
    FUNC(GS_MUL_COORD3)(&ScratchPad->Mat, &pP_ptr->Coord);
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        pEm0c_ghost[1].Workm = ScratchPad->Mat;
        Em0c_ghost_scale.a += Em0c_ghost_add;
        Em0c_ghost_scale.b += Em0c_ghost_add;
        ScratchPad->Vec.vx = ScratchPad->Vec.vz = Em0c_ghost_scale.a;  // sign-extends the halfword
        ScratchPad->Vec.vy = Em0c_ghost_scale.b;
        FUNC(SCALE_MATRIX)(&ScratchPad->Mat, &ScratchPad->Vec);
        if (Em0c_ghost_scale.a >= 0x1771 || Em0c_ghost_scale.a < 0xBB8)  // sign-extends the halfword
        {
            Em0c_ghost_add *= -1;
        }
        pEm0c_ghost->Workm = ScratchPad->Mat;
    }
    if (G.pEm->Zone_in_flg == 0)
    {
        return;
    }
    ScratchWord[0] = 2;
    while ((ScratchWord[0] = ScratchWord[0] - 1) != -1)
    {
        FUNC(GS_MUL_COORD0)(Monitor.pWs_mat, &pEm0c_ghost[ScratchWord[0]].Workm, &mtx);
        FUNC(GS_SET_LS_MATRIX)(&mtx);
        FUNC(GS_PRST_TG3_NL)(ScratchUptr[14][4], ScratchUptr[14][0], ScratchUptr[14][2], (UCHAR*)(pEm0c_ghost[ScratchWord[0]].pPacket) + G.Cbuf_id * sizeof(POLY_GT3) + 8, ScratchUptr[14][5], 4, &MAIN.Ot1[G.Cbuf_id]);
    }
}

VOID Em0c_Main(VOID)
{
    PARTS_W* pP_ptr;

    if (G.pEm->Routine_0 != 0 && (G.pEm->Type & 0x40))
    {
        G.pEm->Routine_0 = 8U;
    }
    if (G.pEm->Routine_0 != 0 && (G.pEm->Type & 0x80))
    {
        return;
    }
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        Em0c_routine_0[G.pEm->Routine_0]();
        if (G.pEm->Routine_2 != 9)
        {
            FUNC(OBA_CK_HIT)(&MAIN.Player.Be_flg, G.pEm);
            FUNC(OBA_CK_EM)();
            EM0C_FREE_P->room_hit = EM0C_FREE_P->room_hit | FUNC(SCA_CK_HIT)(&G.pEm->Pos_x, G.pEm->pAt_tbl->At_w);
            EM0C_FREE_P->Dir_ctrl_flg = ScratchWord[6];
        }
        Em0c_scroll_body_uvs(Em0c_uv_scroll_u_tbl[EM0C_FREE_P->uv_scroll_step], Em0c_uv_scroll_v_tbl[EM0C_FREE_P->uv_scroll_step]);
        if (++EM0C_FREE_P->uv_scroll_step >= 0x1E)
        {
            EM0C_FREE_P->uv_scroll_step = 0;
        }
        if (EM0C_FREE_P->flags & 2)
        {
            if (EM0C_FREE_P->look_at_timer == 0)
            {
                G.pEm->Neck_flg = 0x13;
                pP_ptr = G.Player.pSin_parts_ptr;
                pP_ptr++;
                G.pEm->Neck_point_x = pP_ptr->Workm.t[0];
                G.pEm->Neck_point_y = pP_ptr->Workm.t[1];
                G.pEm->Neck_point_z = pP_ptr->Workm.t[2];
            }
            else if (EM0C_FREE_P->look_at_timer < 4U)
            {
                EM0C_FREE_P->look_at_timer--;
                G.pEm->Neck_flg = 0;
                G.pEm->Damage_no = 0;
            }
            else
            {
                EM0C_FREE_P->look_at_timer--;
                G.pEm->Neck_flg = 0x33;
                G.pEm->Neck_point_x = 0x190;
                G.pEm->Neck_point_y = 0xE0C;
            }
        }
        if (!(G.pEm->Life & 0x8000))
        {
            FUNC(ROT_NECK)();
        }
        EM0C_FREE_P->saved_state = EM_ROUTINE_WORD;
    }
    if (!(EM0C_FREE_P->flags & 8))
    {
        Em0c_draw_heart();
    }
    G.pEm->Zone_in_flg = FUNC(HIT_CK_POINT4)(&G.pEm->Pos_x, G.Cccut);
    if (G.pEm->Id != 0xC && !(EM0C_FREE_P->flags & 8))
    {
        Em0c_draw_claw_ghosts();
        if (Em0c_trail_timer.timer & 0x8000)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            pP_ptr += 8;
            Em0c_trail_timer.segments = 8;
            ScratchWord[5] = 7;
            if (Em0c_ghost_scale.b >= 0x1F41)
            {
                Em0c_trail_far.vy += 0x3E8;
            }
            Em0c_trail_far.vy -= 0x320;
            for (;;)
            {
                Em0c_trail_far.vy += 0x64;
                FUNC(EM0C_TRAIL_PUSH)(pEm0c_trail, &pP_ptr->Workm, &pP_ptr->Workm, &Em0c_trail_near, &Em0c_trail_far, ScratchWord[5], 0);
                if ((ScratchWord[5] = ScratchWord[5] - 1) == -1)
                {
                    break;
                }
            }
            FUNC(EM0C_TRAIL_PUSH)(pEm0c_trail, &pP_ptr->Workm, &pP_ptr->Workm, &Em0c_trail_near, &Em0c_trail_near, 8, 0);
            Em0c_trail_timer.timer &= 0x7FFF;
            if (Em0c_ghost_scale.b >= 0x1F41)
            {
                Em0c_trail_far.vy -= 0x3E8;
            }
        }
        if (Em0c_trail_timer.timer != 0)
        {
            Em0c_trail_move();
        }
    }
    if (EM0C_FREE_P->flags & 1)
    {
        pP_ptr = G.pEm->pSin_parts_ptr;
        if (G.pEm->Zone_in_flg != 0)
        {
            FUNC(MUL_KAGE)(&G.pEm->Pos_x, &G.pEm->Kage_work, 0U, G.pEm->Cdir_y);
        }
    }
    if (!(EM0C_FREE_P->flags & 8) && G.pEm->Id == 0x10 && G.pEm->Life < 0xC9)
    {
        FUNC(FLG_ON)(&MAIN.Room_flg[0], ROOM_SLIDE);
        G.pEm->Life = 0xC8;
    }
    FUNC(SCE_AT_CHECK)(G.pEm, 2);
}

VOID Em0c_Move(VOID)
{
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    G.pEm->Be_flg = G.pEm->Be_flg | 0x40;
    FUNC(EM_CK_HANI)(0xFA0);
    FUNC(EM_CK_ALERT)(0xFA0);
    switch (G.pEm->Routine_1)
    {
    case 0:
        Em0c_think();

    case 1:
        Em0c_act();
    }
}

VOID Em0c_Damage(VOID)
{
    INT unused;    // unused local: reserves stack space

    if (G.pEm->Routine_1 == 0)
    {
        EM_ROUTINE_WORD = EM0C_FREE_P->saved_state;
        if (EM0C_FREE_P->look_at_timer == 0)
        {
            EM0C_FREE_P->look_at_timer = 8;
        }
        if (EM0C_FREE_P->special_timer != 0)
        {
            EM0C_FREE_P->special_timer = 0xD2;
        }
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchPad->Vec.vy = -0x834;
        ScratchWord[1] = FUNC(GET_DIR_TGT)(G.Player.Pos_x, G.Player.Pos_z);

        FUNC(ESP_CALL)(0, 0, ScratchWord[1], G.pEm->M, &ScratchPad->Vec, 0);
    }
}

VOID Em0c_Die(VOID)
{
    if (G.pEm->Routine_1 == 0)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 0, 1, 3);
        if (G.pEm->Id == 0x10)
        {
            G.pEm->Routine_2 = 1U;
        }
    }
    Em0c_die[G.pEm->Routine_2]();
}

VOID Em0c_Dead(VOID)
{
}

VOID Em0c_act(VOID)
{
    INT unused;    // unused local: reserves stack space

    ScratchWord[7] = FUNC(EM_SIKAI_PROC)();
    FUNC(ROOT_CK)(G.Player.Pos_x, G.Player.Pos_z, &G.pEm->Dest_x, &G.pEm->Dest_z);
    Em0c_mv[G.pEm->Routine_2]();
}

VOID Em0c_think(VOID)
{
    if (G.Player.Life & 0x8000)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 0, 1, 1);
        return;
    }
    if (G.pEm->Life < 0x3C)
    {
        EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask | 0x80;
    }
    ScratchWord[0] = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);

    if (ScratchWord[0] < 0xFA0)
    {
        if ((++EM0C_FREE_P->close_frames & 0xFF) >= 0x79U)
        {
            EM0C_FREE_P->frenzy_timer = 0x3C;
            EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask | 0x80;
        }
    }
    else
    {
        EM0C_FREE_P->close_frames = 0;
    }

    Em0c_mv_think[((G.pEm->Id >> 2) & 0xFF)]();
}

VOID Em0c_think_lab(VOID)
{
    if (EM0C_FREE_P->attack_cooldown != 0)
    {
        EM0C_FREE_P->attack_cooldown--;
    }
    EM0C_FREE_P->special_timer = 0;
    if (EM0C_FREE_P->attack_cooldown == 0)
    {
        if (ScratchWord[0] < 0xA8C)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x340) == 0)  // address passed as an integer
            {
                EM_ROUTINE_WORD = ROUTINES(0, 4, 1, 1);
            }
        }
        if (EM0C_FREE_P->frenzy_timer != 0 && ScratchWord[0] < 0xA8C)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x340) == 0)  // address passed as an integer
            {
                EM_ROUTINE_WORD = ROUTINES(0, 4, 1, 1);
                EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask | 0x80;
                return;
            }
        }
        if (ScratchWord[0] < 0x7D0)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200) == 0)  // address passed as an integer
            {
                EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);
                if (G.Player.Life < 0x10)
                {
                    G.pEm->Routine_2 += FUNC(CDIR_CK)(&MAIN.Player) == 0 ? 4 : 0;  // address passed as an integer
                }
            }
        }
    }
}

VOID Em0c_think_roof(VOID)
{
    if (EM0C_FREE_P->attack_cooldown != 0)
    {
        EM0C_FREE_P->attack_cooldown--;
    }
    if (G.Player.Type == 0xA)
    {
        EM0C_FREE_P->rocket_flag = EM0C_FREE_P->rocket_flag | 0x80;
    }
    if (EM0C_FREE_P->special_timer != 0)
    {
        EM0C_FREE_P->special_timer--;
    }
    if (EM0C_FREE_P->attack_cooldown == 0)
    {
        if (ScratchWord[0] >= 0x2EE1)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x40) == 0)  // address passed as an integer
            {
                EM_ROUTINE_WORD = ROUTINES(0, 8, 1, 1);
            }
        }
        if (EM0C_FREE_P->frenzy_timer != 0 && ScratchWord[0] < 0xA8C)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x340) == 0)  // address passed as an integer
            {
                EM_ROUTINE_WORD = ROUTINES(0, 4, 1, 1);
                EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask | 0x80;
                return;
            }
        }
        if (ScratchWord[0] < 0xA8C)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x340) == 0)  // address passed as an integer
            {
                EM_ROUTINE_WORD = ROUTINES(0, 4, 1, 1);
            }
        }
        if (ScratchWord[0] < 0x7D0)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200) == 0)  // address passed as an integer
            {
                EM_ROUTINE_WORD = ROUTINES(0, 3, 1, 1);
                if (G.Player.Life < 0x10)
                {
                    G.pEm->Routine_2 += FUNC(CDIR_CK)(&MAIN.Player) == 0 ? 4 : 0;  // address passed as an integer
                }
            }
        }
        if (ScratchWord[0] < 0x1B58)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x4C8) != 0)  // address passed as an integer
            {
                EM_ROUTINE_WORD = ROUTINES(0, 5, 1, 1);
            }
        }
        if (EM0C_FREE_P->special_timer == 0)
        {
            EM_ROUTINE_WORD = ROUTINES(0, 0xB, 1, 1);
        }
    }
    if (FUNC(FLG_CK)(&MAIN.Room_flg, ROOM_COM) != 0 && ScratchWord[0] < 0xA8C)
    {
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200) == 0)  // address passed as an integer
        {
            EM_ROUTINE_WORD = ROUTINES(0, 7, 1, 1);
        }
    }
}

VOID Em0c_Move_pause(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0;
        G.pEm->Timer0 = 0x3CU;
        G.pEm->Hokan_flg = 0x1F;

    case 1:
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);
        if (G.pEm->Timer0-- == 0)
        {
            G.pEm->Routine_1 = 0;
            EM_ROUTINE_HALF2 = 1;
        }
        return;
    }
}

VOID Em0c_Move_walk(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 1;

    case 1:
        ScratchPad->Vec.vx = G.pEm->Dest_x;
        ScratchPad->Vec.vy = 0;
        ScratchPad->Vec.vz = G.pEm->Dest_z;
        FUNC(EM_WANDER_TURN)(EM0C_FREE_P->Dir_ctrl_flg, &EM0C_FREE_P->room_hit, &EM0C_FREE_P->Wander_rno, 0x28, 0x28);

        if (EM0C_FREE_P->frenzy_timer != 0)
        {
            EM0C_FREE_P->frenzy_timer--;
            FUNC(EM_WANDER_TURN)(EM0C_FREE_P->Dir_ctrl_flg, &EM0C_FREE_P->room_hit, &EM0C_FREE_P->Wander_rno, 0x28, 0x28);
        }

        FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
    }
    if (G.pEm->Move_cnt >= 6U && G.pEm->Move_cnt < 0x1FU)
    {
        if (G.pEm->Move_cnt == 6)
        {
            FUNC(SND_SE_ENEM)(0U, 0);
        }
        Em0c_root_motion(0, 0);
        G.pEm->Spd_cur = G.pEm->Spd_cur + 0x19;
        FUNC(ADD_SPEED_XZ)(0U);
        return;
    }
    if (G.pEm->Move_cnt == 0x1F)
    {
        FUNC(SND_SE_ENEM)(0U, 0);
    }
    Em0c_root_motion(1, 0);
    G.pEm->Spd_cur = G.pEm->Spd_cur + 0x19;
    FUNC(ADD_SPEED_XZ)(0U);
}

VOID Em0c_Move_nop(VOID)
{
}

VOID Em0c_Move_swipe(VOID)
{
    PARTS_W* pP_ptr;
    USHORT hit_frame;

    switch (G.pEm->Routine_3)
    {
    case 0:
        EM0C_FREE_P->flags = EM0C_FREE_P->flags & 0xFB;
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 5;
        G.pEm->Spd_cur = 0xC8;
        FUNC(SND_SE_ENEM)(1, 0);
        EM0C_FREE_P->attack_cooldown = 0xA;

        EM0C_FREE_P->close_frames = EM0C_FREE_P->close_frames + 0x14;

        Em0c_trail_timer.timer = 0x801F;

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
        if (G.pEm->Spd_cur != 0)
        {
            FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x10);  // address passed as an integer

            if (EM0C_FREE_P->frenzy_timer != 0)
            {
                FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x10);  // address passed as an integer
            }
        }
        hit_frame = G.pEm->Move_cnt - 5;
        if (hit_frame < 4U)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            pP_ptr += 8;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0x1F4;
            if (FUNC(L_POS_CK_XZM)(&pP_ptr->Workm, &ScratchPad->Vec, 0x320, &MAIN.Player.Pos_x) != 0 && !(G.Player.Life & 0x8000))
            {
                FUNC(MODEL_COLOR_PLUS)(pP_ptr, 0xFF, 0x80880, 0x808080);
                ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
                ScratchPad->Vec.vy = 0x320;

                FUNC(ESP_CALL)(0, 3, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);

                PL_ROUTINE_WORD = ROUTINES(0, 1, 0xC, 6);
                G.Player.pTgt = G.pEm;
                G.Player.Move_no = FUNC(CDIR_CK)(&MAIN.Player);  // address passed as an integer
                EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask | 1;
                if (!(EM0C_FREE_P->flags & 4))
                {
                    FUNC(SND_SE_ENEM)(2, 0);
                    EM0C_FREE_P->flags = EM0C_FREE_P->flags | 4;
                }
            }
        }

        if (hit_frame == 3 && (EM0C_FREE_P->hit_mask & 1))
        {
            G.Player.Life = G.Player.Life - 0x12;
        }

        if (G.pEm->Move_cnt == 0x32)
        {
            G.pEm->Routine_3 = 2U;
        }
        break;
    case 2:
        EM_ROUTINE_WORD = ROUTINES(0, 1, 0, 1);
        EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask & 0x74;
    }
    G.pEm->Spd_cur -= G.pEm->Move_cnt * 5;
    if (G.pEm->Spd_cur < 0)
    {
        G.pEm->Spd_cur = 0;
    }
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em0c_Move_slash(VOID)
{
    USHORT hit_frame;
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        EM0C_FREE_P->flags = EM0C_FREE_P->flags & 0xFB;
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 4;
        EM0C_FREE_P->attack_cooldown = 0xF;

    case 1:
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 8);  // address passed as an integer

        if (EM0C_FREE_P->frenzy_timer != 0)
        {
            FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x28);  // address passed as an integer
        }

        if (G.pEm->Move_cnt == 7)
        {
            FUNC(SND_SE_ENEM)(1, 0);
            Em0c_trail_timer.timer = 0x8008;
        }

        hit_frame = G.pEm->Move_cnt + 0xFFF9;

        if (hit_frame < 5U)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            pP_ptr += 8;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0;
            if (FUNC(L_POS_CK_XZM)(&pP_ptr->Workm, &ScratchPad->Vec, 0x5DC, &MAIN.Player.Pos_x) != 0 && !(G.Player.Life & 0x8000))
            {
                FUNC(MODEL_COLOR_PLUS)(pP_ptr, 0xFF, 0x80880, 0x808080);
                PL_ROUTINE_WORD = ROUTINES(0, 1, 0xC, 6);

                if (G.pEm->Id == 0x10 && G.Player.Life - 0x12 > 0)
                {
                    PL_ROUTINE_WORD = ROUTINES(0, 2, 0xC, 6);
                    ScratchWord[3] = G.Player.Pos_x - G.pEm->Pos_x;
                    ScratchWord[4] = G.Player.Pos_z - G.pEm->Pos_z;
                    if (ScratchWord[3] != 0)
                    {
                        ScratchWord[5] = FUNC(CATAN)(ScratchWord[4] * 0x1000 / ScratchWord[3]);
                        G.Player.Cdir_y = ((ScratchWord[3] < 0 ? 0x400 : 0xC00) - ScratchWord[5]) & 0xFFF;
                    }
                    else
                    {
                        G.Player.Cdir_y = ScratchWord[4] > 0 ? 0x800 : 0;
                    }
                    EM0C_FREE_P->special_timer = 0xD2;
                }
                G.Player.pTgt = G.pEm;
                G.pEm->Cdir_y = G.pEm->Cdir_y + 0x400;
                G.Player.Move_no = FUNC(CDIR_CK)(&MAIN.Player);  // address passed as an integer
                G.pEm->Cdir_y -= 0x400;
                EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask | 2;
                if (!(EM0C_FREE_P->flags & 4))
                {
                    EM0C_FREE_P->flags = EM0C_FREE_P->flags | 4;
                }
            }
        }
        if (hit_frame == 5 && (EM0C_FREE_P->hit_mask & 2))
        {
            G.Player.Life = G.Player.Life - 0x12;

            if (G.Player.Life & 0x8000)
            {
                G.Player.Life = 1;
                EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask | 0x80;
                FUNC(SND_SE_ENEM)(2, 0);
            }
            else
            {
                FUNC(SND_SE_ENEM)(3, 0);
            }
        }
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
        if ((EM0C_FREE_P->hit_mask & 0x82) == 0x82 && G.pEm->Move_cnt == 0x11)
        {
            EM_ROUTINE_HALF2 = 6;
            FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x20);  // address passed as an integer
        }
        return;
    case 2:
        EM_ROUTINE_WORD = ROUTINES(0, 1, 0, 1);
        ScratchWord[0] = (__builtin_abs(G.Player.Pos_x - G.pEm->Pos_x)) + (__builtin_abs(G.Player.Pos_z - G.pEm->Pos_z));
        if (G.pEm->Id == 0x10 && ScratchWord[0] >= 0x1389 && (EM0C_FREE_P->hit_mask & 2))
        {
            if (Api_rand() & 1)
            {
                EM_ROUTINE_WORD = ROUTINES(0, 0xA, 1, 1);
            }
        }
        EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask & 0x75;
    }
}

VOID Em0c_Move_thrust(VOID)
{
    USHORT hit_frame;
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 7;
        EM0C_FREE_P->attack_cooldown = 0xF;

    case 1:
        if (G.pEm->Move_cnt == 0xA)
        {
            Em0c_trail_timer.timer = 0x8008;
        }
        if (G.pEm->Move_cnt == 0xC)
        {
            FUNC(SND_SE_ENEM)(1, 0);
        }

        hit_frame = G.pEm->Move_cnt + 0xFFF8;

        if (hit_frame < 8U)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            pP_ptr += 8;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0;
            if (FUNC(L_POS_CK_XZM)(&pP_ptr->Workm, &ScratchPad->Vec, 0x5DC, &MAIN.Player.Pos_x) != 0 && !(G.Player.Life & 0x8000))
            {
                FUNC(MODEL_COLOR_PLUS)(pP_ptr, 0xFF, 0x80880, 0x808080);
                PL_ROUTINE_WORD = ROUTINES(0, 1, 0xC, 6);

                if (G.Player.Life - 0x14 > 0)
                {
                    PL_ROUTINE_WORD = ROUTINES(0, 2, 0xC, 6);
                    ScratchWord[3] = G.Player.Pos_x - G.pEm->Pos_x;
                    ScratchWord[4] = G.Player.Pos_z - G.pEm->Pos_z;
                    if (ScratchWord[3] != 0)
                    {
                        ScratchWord[5] = FUNC(CATAN)(ScratchWord[4] * 0x1000 / ScratchWord[3]);
                        G.Player.Cdir_y = ((ScratchWord[3] < 0 ? 0xC00 : 0x1400) - ScratchWord[5]) & 0xFFF;
                    }
                    else
                    {
                        G.Player.Cdir_y = ScratchWord[4] > 0 ? 0x1000 : 0x800;
                    }
                }
                EM0C_FREE_P->special_timer = 0xD2;
                G.Player.pTgt = G.pEm;
                G.pEm->Cdir_y = G.pEm->Cdir_y + 0x400;
                G.Player.Move_no = FUNC(CDIR_CK)(&MAIN.Player);  // address passed as an integer
                G.pEm->Cdir_y = G.pEm->Cdir_y - 0x400;
                EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask | 4;
            }
        }
        if (hit_frame == 7 && (EM0C_FREE_P->hit_mask & 4))
        {
            G.Player.Life = G.Player.Life - 0x14;

            if (G.Player.Life < 0)
            {
                FUNC(SND_SE_ENEM)(2, 0);
            }
            else
            {
                FUNC(SND_SE_ENEM)(3, 0);
            }
        }
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)
        {
            G.pEm->Routine_3 = 2U;
            G.pEm->Cdir_y = G.pEm->Cdir_y + 0x800;
        }
        break;
    case 2:
        EM_ROUTINE_WORD = ROUTINES(0, 1, 0, 1);
        ScratchWord[0] = (__builtin_abs(G.Player.Pos_x - G.pEm->Pos_x)) + (__builtin_abs(G.Player.Pos_z - G.pEm->Pos_z));
        if (G.pEm->Id == 0x10 && ScratchWord[0] >= 0x1389 && (EM0C_FREE_P->hit_mask & 4))
        {
            if (Api_rand() & 1)
            {
                EM_ROUTINE_WORD = ROUTINES(0, 0xA, 1, 1);
            }
        }
        EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask & 0xFB;
    }
    if (G.pEm->Move_cnt < 0xEU)
    {
        Em0c_root_motion(0, 1);
        return;
    }
    Em0c_root_motion(1, 1);
}

VOID Em0c_Move_backhand(VOID)
{
    USHORT hit_frame;
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        EM0C_FREE_P->flags = EM0C_FREE_P->flags & 0xFB;
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 3;
        G.pEm->Spd_cur = 0x12CU;
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x30);  // address passed as an integer
        Em0c_trail_timer.timer = 0xF;
        Em0c_trail_timer.segments = 8;

    case 1:
        if (G.pEm->Move_cnt == 4)
        {
            FUNC(SND_SE_ENEM)(1, 0);
        }

        hit_frame = G.pEm->Move_cnt + 0xFFFC;

        if (hit_frame < 4U)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            pP_ptr += 8;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0x1F4;
            if (FUNC(L_POS_CK_XZM)(&pP_ptr->Workm, &ScratchPad->Vec, 0x320, &MAIN.Player.Pos_x) != 0 && !(G.Player.Life & 0x8000))
            {
                FUNC(MODEL_COLOR_PLUS)(pP_ptr, 0xFF, 0x80880, 0x808080);
                PL_ROUTINE_WORD = ROUTINES(0, 0, 0xC, 6);
                G.Player.pTgt = G.pEm;
                G.pEm->Cdir_y = G.pEm->Cdir_y - 0x400;
                G.Player.Move_no = FUNC(CDIR_CK)(&MAIN.Player);  // address passed as an integer
                G.pEm->Cdir_y = G.pEm->Cdir_y + 0x400;
                EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask | 8;
                if (!(EM0C_FREE_P->flags & 4))
                {
                    FUNC(SND_SE_ENEM)(2, 0);
                    EM0C_FREE_P->flags = EM0C_FREE_P->flags | 4;
                }
            }
        }
        if (hit_frame == 3 && (EM0C_FREE_P->hit_mask & 8))
        {
            G.Player.Life = G.Player.Life - 0x12;

            if ((G.Player.Life & 0x8000) && (EM0C_FREE_P->hit_mask & 0x82) == 0x82)
            {
                G.Player.Life = 1;
            }
        }
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
        G.pEm->Spd_cur = G.pEm->Spd_cur - G.pEm->Move_cnt * 3;
        if (G.pEm->Spd_cur < 0)
        {
            G.pEm->Spd_cur = 0U;
        }
        if ((EM0C_FREE_P->hit_mask & 0x8A) == 0x8A && G.pEm->Move_cnt == 0xC && G.Player.Damage_no != 0)
        {
            FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x20);  // address passed as an integer
            EM_ROUTINE_HALF2 = 3;
            if (G.Player.Life < 0x10)
            {
                G.pEm->Routine_2 += FUNC(CDIR_CK)(&MAIN.Player) == 0 ? 4 : 0;  // address passed as an integer
            }
        }
        break;
    case 2:
        EM_ROUTINE_WORD = ROUTINES(0, 1, 0, 1);
        EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask & 0x75;
    }
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em0c_Move_impale(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 5;
        G.pEm->Spd_cur = 0xC8;
        G.pEm->Damage_no = 1;
        FUNC(SET_BASE_POS)(&MAIN.Player.Be_flg);
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x400);
        G.Player.Cdir_y = G.pEm->Cdir_y;
        PL_ROUTINE_WORD = ROUTINES(0, 0, 0xC, 7);
        G.Player.pTgt = G.pEm;
        FUNC(SND_SE_ENEM)(1, 0);
        Em0c_trail_timer.timer = 0x802F;

    case 1:
        if (G.pEm->Move_cnt != 5 && G.pEm->Move_cnt != 0x5C)
        {
        }
        else
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            pP_ptr += 8;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0x320;

            FUNC(ESP_CALL)(0, 3, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);
        }
        if (G.pEm->Move_cnt == 5)
        {
            FUNC(SND_SE_ENEM)(4, 0);
        }
        if (G.pEm->Move_cnt < 0x61U)
        {
            if (!(G.pEm->Move_cnt % 7 & 0xFF))
            {
                pP_ptr = G.pEm->pSin_parts_ptr;
                pP_ptr += 8;
                ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
                ScratchPad->Vec.vy = 0x320;

                FUNC(ESP_CALL)(0, 0, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);
            }
        }
        if (G.pEm->Move_cnt == 0x5F)
        {
            FUNC(SND_SE_ENEM)(7, 0);
        }
        FUNC(NULL_POS_SET)(G.pEm, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr);
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
        if (G.pEm->Spd_cur != 0)
        {
            FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x10);
        }
        break;
    case 2:
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = 1;
        EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask & 0xF8;
        G.pEm->Be_flg = G.pEm->Be_flg & ~EM_BE_NO_HIT;
    }
    G.pEm->Spd_cur = G.pEm->Spd_cur - G.pEm->Move_cnt * 5;
    if (G.pEm->Spd_cur < 0)
    {
        G.pEm->Spd_cur = 0;
    }
    FUNC(ADD_SPEED_XZ)(0);
    G.pEm->Root_x = G.pEm->Root_x + G.pEm->Spd_x;
    G.pEm->Root_z = G.pEm->Root_z + G.pEm->Spd_z;
    G.Player.Root_x += G.pEm->Spd_x;
    G.Player.Root_z += G.pEm->Spd_z;
}

VOID Em0c_Move_charge(VOID)
{
    USHORT hit_frame;
    PARTS_W* pP_ptr;
    VECTOR saved_pos;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = G.pEm->Move_cnt >> 1;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 2;
        G.pEm->Spd_cur = 0x190U;
        if (FUNC(FLG_CK)(&MAIN.Room_flg[0], ROOM_COM) != 0)
        {
            G.pEm->Spd_cur = 0x12CU;
        }
        EM0C_FREE_P->attack_cooldown = 0xF;

    case 1:
        if (FUNC(FLG_CK)(&MAIN.Room_flg[0], ROOM_COM) != 0)
        {
            FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x30);
        }
        else
        {
            FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x20);
        }
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
        ScratchWord[0] = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);
        if ((ScratchWord[0] < 0x1388 && FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x80) == 0) || FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x384) != 0)
        {
            G.pEm->Routine_3 = 2U;
        }
        saved_pos = EM_POS_VEC;
        ScratchWord[0] = FUNC(SCA_CK_HIT)(&G.pEm->Pos_x, G.pEm->pAt_tbl->At_w);
        G.pEm->Pos_x = saved_pos.vx;
        G.pEm->Pos_y = saved_pos.vy;
        G.pEm->Pos_z = saved_pos.vz;
        if (ScratchWord[0] != 0)
        {
            G.pEm->Routine_3 = 2U;
        }
        if (G.pEm->Move_cnt == 3 || G.pEm->Move_cnt == 0xF)
        {
            FUNC(SND_SE_ENEM)(0, 0);
        }
        break;
    case 2:
        G.pEm->Routine_3 = 3U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 9;
        G.pEm->Spd_cur = 0x12CU;

    case 3:
        if (G.pEm->Move_cnt == 8)
        {
            Em0c_trail_timer.timer = 0x800F;
        }
        hit_frame = G.pEm->Move_cnt - 0xF;
        if (hit_frame < 5U)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            pP_ptr += 8;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0x1F4;
            if (FUNC(L_POS_CK_XZM)(&pP_ptr->Workm, &ScratchPad->Vec, 0x708, &MAIN.Player.Pos_x) != 0 && !(G.Player.Life & 0x8000))
            {
                FUNC(MODEL_COLOR_PLUS)(pP_ptr, 0xFF, 0x80880, 0x808080);
                PL_ROUTINE_WORD = ROUTINES(0, 0, 0xC, 6);
                G.Player.pTgt = G.pEm;
                G.pEm->Cdir_y -= 0x400;
                G.Player.Move_no = FUNC(CDIR_CK)(&MAIN.Player.Be_flg);
                G.pEm->Cdir_y = G.pEm->Cdir_y + 0x400;
                EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask | 8;
                EM0C_FREE_P->special_timer = 0xD2;
            }
        }

        if (hit_frame == 4 && (EM0C_FREE_P->hit_mask & 8))
        {
            G.Player.Life = G.Player.Life - 0x1E;

            FUNC(SND_SE_ENEM)(2, 0);
        }

        if (G.pEm->Move_cnt >= 8U && G.pEm->Move_cnt < 0xFU)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0x3E8;

            FUNC(ESP_CALL)(0x11, 4, 0, &pP_ptr[8].Workm, &ScratchPad->Vec, 0);

            ScratchPad->Vec.vy = 0x384;

            FUNC(ESP_CALL)(0x11, 4, 0, &pP_ptr[8].Workm, &ScratchPad->Vec, 0);
        }
        if (G.pEm->Move_cnt == 7)
        {
            FUNC(SND_SE_ENEM)(5, 0);
        }
        if (G.pEm->Move_cnt == 0xD)
        {
            FUNC(SND_SE_ENEM)(1, 0);
        }
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
        G.pEm->Spd_cur = G.pEm->Spd_cur - G.pEm->Move_cnt;
        if (G.pEm->Spd_cur < 0)
        {
            G.pEm->Spd_cur = 0U;
        }
        break;
    case 4:
        EM_ROUTINE_WORD = ROUTINES(0, 1, 0, 1);
        EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask & 0xF7;
    }
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em0c_Move_erupt(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Pos_y = 0x12C;
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 6;
        FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
        G.pEm->Timer0 = 0x3CU;
        FUNC(XA_CMD)(1U, 0x2D);
        G.System_flg |= SYS_XA_VOICE;
        FUNC(SND_SE_STAD)(2U, 0x1C, 0, &G.pEm->Pos_x);
        FUNC(SND_SE_STAD)(2U, 0x1D, 0, &G.pEm->Pos_x);
        FUNC(SND_SE_STAD)(2U, 0x1E, 0, &G.pEm->Pos_x);
        FUNC(SRAND)(0xB23U);

    case 1:
        if (--G.pEm->Timer0 == 0)
        {
            G.pEm->Routine_3++;
            return;
        }
        if (G.pEm->Timer0 >= 0x1A && G.pEm->Timer0 < 0x21)
        {
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x1194;
            ScratchPad->Vec.vy = (Api_rand() & 0x1FF) * -1;
            ScratchPad->Vec.vz = -(Api_rand() & 0x7FF);
            ScratchWord[7] = Api_rand() & 0xFFF;

            FUNC(ESP_CALL)(0x18U, 3, ScratchWord[7], G.pEm->M, &ScratchPad->Vec, 0xF);

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x1194;
            ScratchPad->Vec.vy = (Api_rand() & 0x1FF) * -1;
            ScratchPad->Vec.vz = -(Api_rand() & 0x7FF);
            ScratchWord[7] = Api_rand() & 0xFFF;

            FUNC(ESP_CALL)(0x18U, 5, ScratchWord[7], G.pEm->M, &ScratchPad->Vec, 0x14);

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x1194;
            ScratchPad->Vec.vy = (Api_rand() & 0x1FF) * -1;
            ScratchPad->Vec.vz = -(Api_rand() & 0x7FF);
            ScratchWord[7] = Api_rand() & 0xFFF;

            FUNC(ESP_CALL)(0x18U, 5, ScratchWord[7], G.pEm->M, &ScratchPad->Vec, 0xF);

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x1194;
            ScratchPad->Vec.vy = (Api_rand() & 0x1FF) * -1;
            ScratchPad->Vec.vz = -(Api_rand() & 0x7FF);
            ScratchWord[7] = Api_rand() & 0xFFF;

            FUNC(ESP_CALL)(0x19U, 6, ScratchWord[7], G.pEm->M, &ScratchPad->Vec, 0x19);

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x1194;
            ScratchPad->Vec.vy = (Api_rand() & 0x1FF) * -1;
            ScratchPad->Vec.vz = -(Api_rand() & 0x7FF);
            ScratchWord[7] = Api_rand() & 0xFFF;

            FUNC(ESP_CALL)(0x19U, 6, ScratchWord[7], G.pEm->M, &ScratchPad->Vec, 0xA);

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x1194;
            ScratchPad->Vec.vy = (Api_rand() & 0x1FF) * -1;
            ScratchPad->Vec.vz = -(Api_rand() & 0x7FF);
            ScratchWord[7] = Api_rand() & 0xFFF;

            FUNC(ESP_CALL)(0x19U, 6, ScratchWord[7], G.pEm->M, &ScratchPad->Vec, 0xF);

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x1194;
            ScratchPad->Vec.vy = (Api_rand() & 0x1FF) * -1;
            ScratchPad->Vec.vz = -(Api_rand() & 0x7FF);
            ScratchWord[7] = Api_rand() & 0xFFF;

            FUNC(ESP_CALL)(0x19U, 6, ScratchWord[7], G.pEm->M, &ScratchPad->Vec, 0xA);

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x1194;
            ScratchPad->Vec.vy = (Api_rand() & 0x1FF) * -1;
            ScratchPad->Vec.vz = -(Api_rand() & 0x7FF);
            ScratchWord[7] = Api_rand() & 0xFFF;

            FUNC(ESP_CALL)(0x18U, 3, ScratchWord[7], G.pEm->M, &ScratchPad->Vec, 0x14);

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x1194;
            ScratchPad->Vec.vy = (Api_rand() & 0x1FF) * -1;
            ScratchPad->Vec.vz = -(Api_rand() & 0x7FF);
            ScratchWord[7] = Api_rand() & 0xFFF;

            FUNC(ESP_CALL)(0x19U, 6, ScratchWord[7], G.pEm->M, &ScratchPad->Vec, 0xA);

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x1194;
            ScratchPad->Vec.vy = (Api_rand() & 0x1FF) * -1;
            ScratchPad->Vec.vz = -(Api_rand() & 0x7FF);
            ScratchWord[7] = Api_rand() & 0xFFF;

            FUNC(ESP_CALL)(0x19U, 6, ScratchWord[7], G.pEm->M, &ScratchPad->Vec, 5);

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x1194;
            ScratchPad->Vec.vy = (Api_rand() & 0x1FF) * -1;
            ScratchPad->Vec.vz = -(Api_rand() & 0x7FF);
            ScratchWord[7] = Api_rand() & 0xFFF;

            FUNC(ESP_CALL)(0x18U, 3, ScratchWord[7], G.pEm->M, &ScratchPad->Vec, 5);
        }
        if (G.pEm->Timer0 >= 0x26 && G.pEm->Timer0 < 0x37)
        {
            ScratchPad->Vec.vx = (Api_rand() & 0xFFF) - 0xBB8;
            ScratchPad->Vec.vy = (Api_rand() & 0x1FF) * -1;
            ScratchPad->Vec.vz = Api_rand() & 0x7FF;

            FUNC(ESP_CALL)(9U, 5, 0, G.pEm->M, &ScratchPad->Vec, 0x1E);

            ScratchPad->Vec.vx = (Api_rand() & 0xFFF) - 0xBB8;
            ScratchPad->Vec.vy = (Api_rand() & 0x1FF) * -1;
            ScratchPad->Vec.vz = -(Api_rand() & 0x7FF);

            FUNC(ESP_CALL)(9U, 5, 0, G.pEm->M, &ScratchPad->Vec, 0x1E);

            ScratchPad->Vec.vx = (Api_rand() & 0xFFF) - 0xBB8;
            ScratchPad->Vec.vy = (Api_rand() & 0x1FF) * -1;
            ScratchPad->Vec.vz = -(Api_rand() & 0x7FF);

            FUNC(ESP_CALL)(9U, 5, 0, G.pEm->M, &ScratchPad->Vec, 0x1E);
        }
        return;
    case 2:
        FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
        if (G.pEm->Move_cnt == 0x21)
        {
            G.pEm->Routine_3++;
            G.pEm->Timer0 = 1U;
        }
        G.pEm->Pos_z = G.pEm->Pos_z - 8;
        G.pEm->Pos_x = G.pEm->Pos_x - 8;
        return;
    case 3:
        if (--G.pEm->Timer0 > 0)
        {
            return;
        }
        G.pEm->Routine_3 += FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
        if (G.pEm->Move_cnt < 0x23U)
        {
            G.pEm->Pos_z = G.pEm->Pos_z - 0x14;
            G.pEm->Pos_x = G.pEm->Pos_x - 0x14;
        }
        G.pEm->Pos_y -= 0xA;
        if (G.pEm->Pos_y < 0)
        {
            G.pEm->Pos_y = 0;
        }
        if (G.pEm->Move_cnt >= 0x24U && G.pEm->Move_cnt < 0x5AU && !(G.pEm->Move_cnt & 3))
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchWord[7] = Api_rand() & 0xF;
            ScratchPad->Vec.vx = 0;
            ScratchPad->Vec.vy = (Api_rand() & 0x1FF) * -1;
            ScratchPad->Vec.vz = 0;

            FUNC(ESP_CALL)(9U, 1, 0, &pP_ptr[ScratchWord[7]].Workm, &ScratchPad->Vec, 0x14);

            ScratchPad->Vec.vx = 0;
            ScratchPad->Vec.vy = (Api_rand() & 0x1FF) * -1;
            ScratchPad->Vec.vz = 0;
            ScratchWord[7] = Api_rand() & 0xF;

            FUNC(ESP_CALL)(9U, 1, 0, &pP_ptr[ScratchWord[7]].Workm, &ScratchPad->Vec, 0x14);
        }
        if (G.pEm->Move_cnt == 0x2B)
        {
            EM0C_FREE_P->flags = EM0C_FREE_P->flags | 1;
        }
        return;
    case 4:
        G.pEm->Routine_1 = 0;
        EM_ROUTINE_HALF2 = 1;
        EM0C_FREE_P->flags = EM0C_FREE_P->flags | 2;

    default:
        ;
    }
}

VOID Em0c_Move_rush(VOID)
{
    USHORT hit_frame;
    PARTS_W* pP_ptr;
    VECTOR saved_pos;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = G.pEm->Move_cnt >> 1;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 2;
        G.pEm->Spd_cur = 0x190U;
        if (FUNC(FLG_CK)(&MAIN.Room_flg[0], ROOM_COM) != 0)
        {
            G.pEm->Spd_cur = 0x12CU;
        }
        G.pEm->Timer0 = 0x3C;
        EM0C_FREE_P->attack_cooldown = 0xD;

    case 1:
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x80);
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
        ScratchWord[0] = __builtin_abs(G.Player.Pos_x - G.pEm->Pos_x) + __builtin_abs(G.Player.Pos_z - G.pEm->Pos_z);
        if (ScratchWord[0] < 0x1770)
        {
            if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x200) == 0)
            {
                EM_ROUTINE_WORD = ROUTINES(0, 1, 0, 1);
                if (EM0C_FREE_P->special_timer == 0)
                {
                    EM_ROUTINE_WORD = ROUTINES(2, 0xA, 1, 1);
                }
                EM0C_FREE_P->special_timer = 0xD2;
            }
        }
        saved_pos = EM_POS_VEC;
        ScratchWord[0] = FUNC(SCA_CK_HIT)(&G.pEm->Pos_x, G.pEm->pAt_tbl->At_w);
        G.pEm->Pos_x = saved_pos.vx;
        G.pEm->Pos_y = saved_pos.vy;
        G.pEm->Pos_z = saved_pos.vz;
        if (G.pEm->Timer0 != 0)
        {
            --G.pEm->Timer0;
        }
        if (!(ScratchWord[0] == 0 && G.pEm->Timer0 != 0))
        {
            EM_ROUTINE_WORD = ROUTINES(0, 1, 0, 1);
            if (EM0C_FREE_P->special_timer == 0)
            {
                EM_ROUTINE_WORD = ROUTINES(2, 0xA, 1, 1);
            }
            EM0C_FREE_P->special_timer = 0xD2;
        }
        if (G.pEm->Move_cnt == 3 || G.pEm->Move_cnt == 0xF)
        {
            FUNC(SND_SE_ENEM)(0, 0);
        }
        break;
    case 2:
        EM0C_FREE_P->flags = EM0C_FREE_P->flags & 0xFB;
        G.pEm->Routine_3 = 3U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 3;
        G.pEm->Move_no = 0xB;
        EM0C_FREE_P->attack_cooldown = 0xF;

    case 3:
        FUNC(GOTO00)(&MAIN.Player.Pos_x, 8);
        if (G.pEm->Move_cnt == 6)
        {
            Em0c_trail_timer.timer = 0x800A;
        }
        if (G.pEm->Move_cnt == 0xA)
        {
            FUNC(SND_SE_ENEM)(1, 0);
        }
        if (G.pEm->Move_cnt < 3U)
        {
            G.pEm->Spd_cur = G.pEm->Spd_cur + 0x1E;
        }
        hit_frame = G.pEm->Move_cnt - 9;
        if (hit_frame < 8U)
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            pP_ptr += 8;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0;
            if (FUNC(L_POS_CK_XZM)(&pP_ptr->Workm, &ScratchPad->Vec, 0x4B0, &MAIN.Player.Pos_x) != 0 && !(G.Player.Life & 0x8000))
            {
                FUNC(MODEL_COLOR_PLUS)(pP_ptr, 0xFF, 0x80880, 0x808080);
                PL_ROUTINE_WORD = ROUTINES(0, 1, 0xC, 6);

                if (G.Player.Life - 0x12 > 0)
                {
                    PL_ROUTINE_WORD = ROUTINES(0, 2, 0xC, 6);
                    ScratchWord[3] = G.Player.Pos_x - G.pEm->Pos_x;
                    ScratchWord[4] = G.Player.Pos_z - G.pEm->Pos_z;
                    if (ScratchWord[3] != 0)
                    {
                        ScratchWord[5] = FUNC(CATAN)(ScratchWord[4] * 0x1000 / ScratchWord[3]);
                        G.Player.Cdir_y = ((ScratchWord[3] < 0 ? 0xC00 : 0x1400) - ScratchWord[5]) & 0xFFF;
                    }
                    else
                    {
                        G.Player.Cdir_y = ScratchWord[4] > 0 ? 0x1000 : 0x800;
                    }
                }
                G.Player.pTgt = G.pEm;
                G.pEm->Cdir_y += 0x400;
                G.Player.Move_no = FUNC(CDIR_CK)(&MAIN.Player.Be_flg);
                G.pEm->Cdir_y -= 0x400;
                EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask | 2;
                if (!(EM0C_FREE_P->flags & 4))
                {
                    EM0C_FREE_P->flags = EM0C_FREE_P->flags | 4;
                }
            }
        }
        if (Em0c_ghost_scale.b >= 0x1389 && hit_frame >= 0xAU)
        {
            Em0c_ghost_scale.b = Em0c_ghost_scale.b - 0x320;
        }
        if (hit_frame == 8 && (EM0C_FREE_P->hit_mask & 2))
        {
            G.Player.Life = G.Player.Life - 0x1E;

            if (G.Player.Life & 0x8000)
            {
                G.Player.Life = 1;
                EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask | 0x80;
                FUNC(SND_SE_ENEM)(2, 0);
            }
            else
            {
                FUNC(SND_SE_ENEM)(3, 0);
            }
        }
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x400);
        if ((EM0C_FREE_P->hit_mask & 0x82) == 0x82 && G.pEm->Move_cnt == 0x11)
        {
            EM_ROUTINE_HALF2 = 6;
            FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x20);
        }
        if (!(EM0C_FREE_P->hit_mask & 2) && G.pEm->Move_cnt < 0x11U)
        {
            FUNC(GOTO00)(&MAIN.Player.Pos_x, 0x40);
        }
        if (G.pEm->Spd_cur != 0 && G.pEm->Move_cnt >= 9U)
        {
            G.pEm->Spd_cur = G.pEm->Spd_cur - G.pEm->Move_cnt;
            if (G.pEm->Spd_cur < 0)
            {
                G.pEm->Spd_cur = 0U;
            }
        }
        break;
    case 4:
        Em0c_ghost_scale.b = Em0c_ghost_scale.a;
        EM_ROUTINE_WORD = ROUTINES(0, 1, 0, 1);
        if (G.pEm->Id == 0x10 && (EM0C_FREE_P->hit_mask & 2))
        {
            if (Api_rand() & 1)
            {
                EM_ROUTINE_WORD = ROUTINES(0, 0xA, 1, 1);
                EM_ROUTINE_WORD = ROUTINES(0, 5, 1, 1);
            }
        }
        EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask & 0x75;
    }
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em0c_Move_rise(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        G.pEm->Move_no = 0xC;
    }
    if (G.pEm->Move_cnt >= 0x29U && Em0c_ghost_scale.b < 0x2328)
    {
        Em0c_ghost_scale.b = Em0c_ghost_scale.b + 0x3E8;
    }
    if (G.pEm->Move_cnt == 0x28)
    {
        FUNC(SND_SE_ENEM)(1, 0);
    }
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80) != 0)
    {
        EM_ROUTINE_WORD = ROUTINES(0, 0xA, 1, 1);
    }
    Em0c_root_motion(1, 1);
}

VOID Em0c_Die_restrain(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0U;
        G.pEm->Hokan_flg = 0xF;
        G.pEm->Move_no = 8;
        G.pEm->Life = -1;

        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg, G.pEm->Em_set_flg);  // address passed as an integer

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x100);
        if (G.pEm->Move_cnt == 0x19 && (G.pEm->Move_wait & 1))
        {
            FUNC(SND_SE_ENEM)(6, 0U);
        }
        if (G.pEm->Move_cnt == 0x32 && (G.pEm->Move_wait & 1))
        {
            FUNC(SND_SE_ENEM)(6, 0U);
        }

        break;

    case 2:
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT | EM_BE_NO_OBJ;
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg, G.pEm->Em_set_flg);  // address passed as an integer
    }
    Em0c_root_motion(0, 0);
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em0c_Die_sce(VOID)
{
    FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg, G.pEm->Em_set_flg);  // address passed as an integer
    G.pEm->Type = G.pEm->Type | 0x40;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Move_no = 0;
    G.pEm->Hokan_flg = 0;
    EM_ROUTINE_WORD = ROUTINES(1, 0, 0, 8);

    FUNC(SND_VAB1_LOAD)();
}

VOID Pldie_Em0c(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 2;
        G.Player.Hokan_flg = 0;
        G.Player.Damage_no = 0x80;  // unsigned byte store of the SCHAR
        G.Player.Be_flg |= EM_BE_NO_HIT | EM_BE_NO_WALL;

        FUNC(PAD_VIB_SET1)(8, 0x50, 0);
        FUNC(PAD_VIB_SET0)(0xF, 6);

    case 1:
        if (G.Player.Move_cnt == 8)
        {
            FUNC(SND_SE_STAD)(3, 3, 0, &MAIN.Player.Pos_x);  // address passed as an integer
            pP_ptr = G.Player.pSin_parts_ptr;
            FUNC(MODEL_COLOR_PLUS)(pP_ptr, 0x30, 0x80820, 0x606060);
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[1], 0x30, 0x80820, 0x606060);
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[2], 0x30, 0x80820, 0x606060);
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[9], 0x30, 0x80820, 0x606060);
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[12], 0x30, 0x80820, 0x606060);

            FUNC(PAD_VIB_FADE_SET)(0x1E, 0x50, 0xC8, 5);
            FUNC(PAD_VIB_FADE_SET)(0x5A, 0xC8, 0, 0x23);
        }
        if (G.Player.Move_cnt < 9U)
        {
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = -0x5DC;

            FUNC(ESP_CALL)(0, 0, 0, &MAIN.Player.M, &ScratchPad->Vec, 0);
        }
        if (G.Player.Move_cnt == 0x5C)
        {
            FUNC(PAD_VIB_SET1)(5, 0x96, 0);
            FUNC(PAD_VIB_FADE_SET)(0x1E, 0x96, 0, 5);
        }
        if (G.Player.Move_cnt >= 0x60U)
        {
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);

            FUNC(ESP_CALL)(0, 0, 0, &MAIN.Player.M, &ScratchPad->Vec, 0);
        }
        FUNC(NULL_POS_SET)(&MAIN.Player, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr);  // address passed as an integer
        G.Player.Routine_3 += FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
        return;
    case 2:
        G.Player.Life = -1;
    }
}

VOID Pldamege_Em0c(VOID)
{
    Pldamage_Em0c_tbl[G.Player.Routine_2]();
}

VOID Em0c_pldm00(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 3;
        G.Player.Spd_cur = 0x258;
        G.Player.Damage_no = 1;

        FUNC(PAD_VIB_SET0)(6, 0);
        FUNC(PAD_VIB_FADE_SET)(8, 0x50, 0xC8, 3);
        FUNC(PAD_VIB_FADE_SET)(5, 0xC8, 0, 0xB);

    case 1:
        if (G.Player.Life < 0x1F)
        {
            FUNC(GOTO00)(POS_VEC(G.Player.pTgt), 0x40);
        }
        if (G.Player.Move_cnt == 3)
        {
            FUNC(SND_SE_STAD)(3, (UCHAR)G.Player.Move_no + 1, 0, &MAIN.Player.Pos_x);  // address passed as an integer
        }
        if (G.Player.Move_cnt < 4U)
        {
            pP_ptr = G.Player.pTgt->pSin_parts_ptr;
            pP_ptr += 8;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0x320;

            FUNC(ESP_CALL)(0, 0, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);

            ScratchPad->Vec.vy = -0x514;

            FUNC(ESP_CALL)(0, 0, 0, &MAIN.Player.M, &ScratchPad->Vec, 0);
        }
        G.Player.Routine_3 += FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
        break;
    case 2:
        PL_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
        G.Player.Damage_no = 0;
    }
    G.Player.Spd_cur -= G.Player.Move_cnt * 0x28;
    if (G.Player.Spd_cur < 0)
    {
        G.Player.Spd_cur = 0;
    }
    FUNC(ADD_SPEED_XZ)(G.Player.pTgt->Cdir_y + 0xDF4 - (SHORT)G.Player.Cdir_y);  // sign-extends the halfword
}

VOID Em0c_pldm01(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 3;
        G.Player.Spd_cur = 0x1F4;
        G.Player.Damage_no = 1;

        FUNC(PAD_VIB_SET0)(6, 0);
        FUNC(PAD_VIB_FADE_SET)(8, 0x50, 0xC8, 3);
        FUNC(PAD_VIB_FADE_SET)(5, 0xC8, 0, 0xB);

    case 1:
        if (G.Player.Life < 0x1F)
        {
            FUNC(GOTO00)(POS_VEC(G.Player.pTgt), 0x40);
        }
        if (G.Player.Move_cnt == 3)
        {
            FUNC(SND_SE_STAD)(3, (UCHAR)G.Player.Move_no + 1, 0, &MAIN.Player.Pos_x);  // address passed as an integer
        }
        if (G.Player.Move_cnt < 4U)
        {
            pP_ptr = G.Player.pTgt->pSin_parts_ptr;
            pP_ptr += 8;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0x320;

            FUNC(ESP_CALL)(0, 0, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);

            ScratchPad->Vec.vy = -0x5DC;

            FUNC(ESP_CALL)(0, 0, 0, &MAIN.Player.M, &ScratchPad->Vec, 0);
        }
        G.Player.Routine_3 += FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
        break;
    case 2:
        PL_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
        G.Player.Damage_no = 0;
    }
    G.Player.Spd_cur -= G.Player.Move_cnt * 0x1E;
    if (G.Player.Spd_cur < 0)
    {
        G.Player.Spd_cur = 0;
    }
    FUNC(ADD_SPEED_XZ)(G.Player.pTgt->Cdir_y + 0x1F4 - (SHORT)G.Player.Cdir_y);  // sign-extends the halfword
}

VOID Em0c_pldm02(VOID)
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
        G.Player.Move_no = 5;
        G.Player.Hokan_flg = 4;
        G.Player.Spd_cur = 0x384;
        G.Player.Damage_no = 1;

        FUNC(PAD_VIB_SET0)(6, 0);
        FUNC(PAD_VIB_FADE_SET)(8, 0x50, 0xC8, 3);
        FUNC(PAD_VIB_FADE_SET)(0x14, 0xC8, 0, 0xB);

    case 1:
        G.Player.Spd_cur -= G.Player.Move_cnt * 0xC;
        if (G.Player.Move_cnt == 3)
        {
            FUNC(SND_SE_STAD)(3, 2, 0, &MAIN.Player.Pos_x);  // address passed as an integer
        }
        G.Player.Routine_3 += FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
        FUNC(ADD_SPEED_XZ)(0x800);
        saved_pos = PL_POS_VEC;
        ScratchWord[0] = FUNC(SCA_CK_HIT)(&MAIN.Player.Pos_x, G.Player.pAt_tbl->At_w);
        PL_POS_VEC = saved_pos;
        if (ScratchWord[0] != 0)
        {
            G.Player.Routine_3 = 5;
            G.Player.Damage_no = 0;
        }
        return;
    case 2:
        G.Player.Routine_3 = 3;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 6;
        G.Player.Hokan_flg = 3;
        FUNC(SND_SE_STAD)(2, 0x1F, 0, &MAIN.Player.Pos_x);  // address passed as an integer

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
        G.Player.Spd_cur -= 0xC;
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
        G.Player.Move_no = 3;
        G.Player.Hokan_flg = 3;
        FUNC(SND_SE_STAD)(2, 0x20, 0, &MAIN.Player.Pos_x);  // address passed as an integer
        FUNC(SND_SE_STAD)(3, 2, 0, &MAIN.Player.Pos_x);  // address passed as an integer
        pP_ptr = G.Player.pSin_parts_ptr;
        ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
        ScratchPad->Vec.vx = ScratchPad->Vec.vx - 0x190;

        FUNC(ESP_CALL)(9, 0x16, 0, &pP_ptr[5].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(9, 0x16, 0, &pP_ptr[8].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(9, 0x11, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(9, 0x11, 0, &pP_ptr[3].Workm, &ScratchPad->Vec, 0);
        FUNC(ESP_CALL)(9, 0x11, 0, &pP_ptr[6].Workm, &ScratchPad->Vec, 0);

        FUNC(PAD_VIB_FADE_SET)(5, 0x64, 0xFA, 0);
        FUNC(PAD_VIB_FADE_SET)(0x1E, 0xFA, 0, 5);

    case 6:
        G.Player.Routine_3 += FUNC(JOINT_MOVE)(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
        return;
    case 7:
        G.Player.Routine_3 = 8;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 4;
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
    }
}

VOID Em0c_root_motion(UCHAR Flg, UCHAR apply)
{
    PARTS_W* pP_ptr;
    UCHAR i;

    pP_ptr = G.pEm->pSin_parts_ptr;
    FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);
    FUNC(GS_MUL_COORD0)(G.pEm->M, &pP_ptr->Coord, &ScratchPad->Mat.m[0][0]);
    if (G.pEm->Scale != 0)
    {
        ScratchPad->Vec.vx = ScratchPad->Vec.vy = ScratchPad->Vec.vz = G.pEm->Scale;
        FUNC(SCALE_MATRIX)(&ScratchPad->Mat, &ScratchPad->Vec);
    }
    pP_ptr = &pP_ptr[Flg * 3 + 11];
    i = 3;
    while (i != 0)
    {
        FUNC(GS_MUL_COORD3)(&ScratchPad->Mat, &(pP_ptr - i)[1].Coord);
        i--;
    }
    ScratchPad->Mat.t[0] = ScratchPad->Mat.t[0] - pP_ptr->Workm.t[0];
    ScratchPad->Mat.t[1] = 0;
    ScratchPad->Mat.t[2] = ScratchPad->Mat.t[2] - pP_ptr->Workm.t[2];
    FUNC(SQUARE0)(&ScratchPad->Mat.t, &ScratchPad->Vec);
    G.pEm->Spd_cur = FUNC(SQUARE_ROOT0)(ScratchPad->Vec.vx + ScratchPad->Vec.vz);
    if (apply != 0)
    {
        G.pEm->Pos_x = G.pEm->Pos_x - ScratchPad->Mat.t[0];
        G.pEm->Pos_z = G.pEm->Pos_z - ScratchPad->Mat.t[2];
    }
}

VOID Em0c_draw_heart(VOID)
{
    MATRIX mtx;
    PARTS_W* pP_ptr;
    SHORT health;

    if (!(G.pEm->Zone_in_flg & 0x7F))
    {
        return;
    }
    health = G.pEm->Life;
    FUNC(SET_LIGHT)(&G.pEm->Pos_x);
    pP_ptr = G.pEm->pSin_parts_ptr;
    ScratchPtr[6] = G.pEm;
    G.pEm = EM0C_FREE_P->heart;
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        FUNC(ROT_MATRIX)(&G.pEm->Cdir_x, G.pEm->M);
        if (!(health & 0x8000))  // sign-extends the halfword
        {
            ScratchPad->Vec.vx = ScratchPad->Vec.vy = ScratchPad->Vec.vz = G.pEm->Old_x + 0x1F4;
            FUNC(SCALE_MATRIX)(G.pEm->M, &ScratchPad->Vec);
            G.pEm->Old_x = G.pEm->Old_x + Em0c_heartbeat_tbl[G.pEm->Timer0] * 2;
            if (G.pEm->Timer0-- == 0)
            {
                G.pEm->Timer0 = 0x15;
            }
        }
    }
    FUNC(GS_MUL_COORD0)(&pP_ptr[1].Workm, G.pEm->M, &mtx);
    G.pEm->Old_y = mtx.t[1];
    FUNC(GS_MUL_COORD0)(Monitor.pWs_mat, &mtx, &ScratchPad->Mat);
    FUNC(MUL_MATRIX0)(Monitor.pLight_mtx, &pP_ptr[1].Workm, &mtx);
    ScratchPtr[14] = G.pEm->pTmd;
    FUNC(SET_LIGHT_MATRIX)(&mtx);
    FUNC(GS_SET_LS_MATRIX)(&ScratchPad->Mat);
    FUNC(GS_PRST_TG3_L)(ScratchUptr[14][4], ScratchUptr[14][0], ScratchUptr[14][2], (UCHAR*)G.pEm->pPacket + G.Cbuf_id * sizeof(POLY_GT3) + 8, ScratchUptr[14][5], 4, &MAIN.Ot1[G.Cbuf_id]);
    G.pEm = ScratchPtr[6];
}

VOID Em0c_scroll_body_uvs(SHORT du, SHORT dv)
{
    TMD_MEM* pTmd;
    POLY_GT3* pPrim;
    LONG n;
    INT prim_idx;
    INT tbl_idx;
    PARTS_W* pP_ptr;

    pP_ptr = G.pEm->pSin_parts_ptr;
    pP_ptr += 2;
    pTmd = (TMD_MEM*)pP_ptr->pTmd;  // reinterprets the memory as TMD_MEM
    pPrim = (POLY_GT3*)((UCHAR*)pP_ptr->pPacket + 8);
    n = pTmd->n_poly;
    prim_idx = 0;
    tbl_idx = 0;
    while (n-- != 0)
    {
        if (prim_idx == Em0c_uv_scroll_poly_tbl[tbl_idx])
        {
            pPrim[0].u0 += du;
            pPrim[0].u1 += du;
            pPrim[0].u2 += du;
            pPrim[0].v0 += dv;
            pPrim[0].v1 += dv;
            pPrim[0].v2 += dv;
            pPrim[1].u0 += du;
            pPrim[1].u1 += du;
            pPrim[1].u2 += du;
            pPrim[1].v0 += dv;
            pPrim[1].v1 += dv;
            pPrim[1].v2 += dv;
            tbl_idx++;
        }
        prim_idx++;
        pPrim += 2;
    }
}

VOID Em0c_clut_stp_set(VOID)
{
    RECT rect;
    PARTS_W* pSlot;
    UINT clut[0x200];
    USHORT* pClut_pos;

    pSlot = (VOID*)G.pEm->pSin_parts_ptr->pTmd;  // untyped pointer, the callee takes the address as is
    pClut_pos = pSlot->pCoord2;
    pClut_pos += 3;
    rect.x = *pClut_pos & 0x3F;
    rect.y = (*pClut_pos & 0x7FC0) >> 6;
    rect.w = 0x100;
    rect.h = 2;
    FUNC(STORE_IMAGE)(&rect, clut);
    FUNC(DRAW_SYNC)(0);
    ScratchWord[0] = 0x200;
    while (--ScratchWord[0] != -1)
    {
        clut[ScratchWord[0]] |= 0x80008000;
    }
    Em0c_clut_ofs.y += 0x10;
    Em0c_clut_ofs.x += 0x10;
    FUNC(LOAD_IMAGE)(&rect, clut);
    FUNC(DRAW_SYNC)(0);
}

VOID Em0c_Scenario(VOID)
{
    pEm0c_victim = (VOID*)&MAIN.Enemy;  // address passed as an integer
    if (G.pEm->Type & 0x40)
    {
        Em0c_sce_mv[G.pEm->Routine_2]();
        return;
    }
    EM_ROUTINE_WORD = ROUTINES(0, 1, 0, 1);
    EM0C_FREE_P->flags = EM0C_FREE_P->flags | 2;
}

VOID Em0c_Sce_idle(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 0;
        G.pEm->Hokan_flg = 0x1F;

        VIEW(INT, Em0c_trail_timer) = 0;  // INT* view of the field (a plain access changes the code)
    }
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);
}

VOID Em0c_Sce_walk(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3++;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        G.pEm->Move_no = 1;
        G.pEm->Timer0 = 2U;
        if (G.pEm->Type & 1)
        {
            G.pEm->Timer0 = 1U;
        }

    case 1:
        ScratchPad->Vec.vx = G.pEm->Root_x;
        ScratchPad->Vec.vy = 0;
        ScratchPad->Vec.vz = G.pEm->Root_z;
        FUNC(GOTO00)(&ScratchPad->Vec, 0x28U);
        if (--G.pEm->Timer0 == 0)
        {
            FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80U);
            G.pEm->Timer0 = 2U;
            if (G.pEm->Type & 1)
            {
                G.pEm->Timer0 = 1U;
            }
        }
        if (FUNC(SQUARE_ROOT0)((G.pEm->Pos_x - G.pEm->Root_x) * (G.pEm->Pos_x - G.pEm->Root_x) + (G.pEm->Pos_z - G.pEm->Root_z) * (G.pEm->Pos_z - G.pEm->Root_z)) < 0x5DCU)
        {
            G.pEm->Routine_3++;
        }
        break;
    case 2:
        FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);  // address passed as an integer
        if (G.pEm->Sca_info & 0x80)
        {
            G.pEm->Routine_3 = 1U;
        }
        else
        {
            EM_ROUTINE_HALF2 = 0;
        }
        break;
    }
    if (G.pEm->Move_cnt != 3 && G.pEm->Move_cnt != 0x24)
    {
    }
    else if ((G.pEm->Timer0 & 1) != 0)
    {
        FUNC(SND_SE_ENEM)(0, 0U);
    }
    if (G.pEm->Move_cnt >= 4U && G.pEm->Move_cnt < 0x23U)
    {
        Em0c_root_motion(0, 0);
        G.pEm->Spd_cur = G.pEm->Spd_cur + 5;
        FUNC(ADD_SPEED_XZ)(0);
        return;
    }
    Em0c_root_motion(1, 0);
    G.pEm->Spd_cur = G.pEm->Spd_cur + 5;
    FUNC(ADD_SPEED_XZ)(0);
}

VOID Em0c_Sce_glass(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Pos_y = 0;
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        G.pEm->Move_no = 6;

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);
        if (G.pEm->Move_cnt == 0x1E)
        {
            ScratchPad->Vec.vx = 0x2A30;
            ScratchPad->Vec.vy = -0x1004;
            ScratchPad->Vec.vz = 0x1388;

            FUNC(ESP_CALL)(1, 0U, 0, 0, &ScratchPad->Vec, 0);

            FUNC(SND_SE_STAD)(2, 0x19U, 0, &ScratchPad->Vec);

            FUNC(PAD_VIB_FADE_SET)(0xA, 0x96U, 0x50, 0);
        }

        if (G.pEm->Move_cnt == 0x55)
        {
            ScratchPad->Vec.vx = 0x2A30;
            ScratchPad->Vec.vy = -0x1004;
            ScratchPad->Vec.vz = 0x1388;
            FUNC(XA_CMD)(1, 0xB3U);
            G.System_flg |= SYS_XA_VOICE;
            FUNC(SND_SE_STAD)(2, 0x1AU, 0, &ScratchPad->Vec);
            FUNC(SND_SE_STAD)(2, 0x1BU, 0, &ScratchPad->Vec);

            FUNC(PAD_VIB_SET0)(8, 0U);
            FUNC(PAD_VIB_SET1)(0xA, 0xFAU, 0);
            FUNC(PAD_VIB_FADE_SET)(0xF, 0xFAU, 0x50, 0xA);
            FUNC(PAD_VIB_SET0)(4, 0x19U);
            FUNC(PAD_VIB_SET0)(4, 0x20U);
            FUNC(PAD_VIB_SET0)(8, 0x28U);
        }

        if (G.pEm->Move_cnt == 0x59)
        {
            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x254;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x2E18;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x14B4;

            FUNC(ESP_CALL)(0x15, 1U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x254;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x2E18;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x12C0;

            FUNC(ESP_CALL)(0x15, 2U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x14);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x254;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x2E18;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x12C0;

            FUNC(ESP_CALL)(0x15, 3U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x254;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x2E18;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x12C0;

            FUNC(ESP_CALL)(0x15, 4U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x1E);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x254;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x2E18;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x12C0;

            FUNC(ESP_CALL)(0x15, 5U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x254;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x2E18;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x12C0;

            FUNC(ESP_CALL)(0x15, 6U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x14);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x254;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x2E18;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x12C0;

            FUNC(ESP_CALL)(0x15, 7U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x14);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x254;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x2E18;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x12C0;

            FUNC(ESP_CALL)(0x15, 1U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x254;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x2E18;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x12C0;

            FUNC(ESP_CALL)(0x15, 5U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x254;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x2E18;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x12C0;

            FUNC(ESP_CALL)(0x15, 6U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x1E);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x254;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x2E18;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x12C0;

            FUNC(ESP_CALL)(0x15, 7U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x14);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x254;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x2E18;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x12C0;

            FUNC(ESP_CALL)(0x15, 1U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x254;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x2E18;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x12C0;

            FUNC(ESP_CALL)(0x15, 5U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x254;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x2E18;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x12C0;

            FUNC(ESP_CALL)(0x15, 6U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x1E);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x254;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x2E18;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x12C0;

            FUNC(ESP_CALL)(0x15, 7U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x14);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x254;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x2E18;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x12C0;

            FUNC(ESP_CALL)(0x15, 1U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xB00;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x28A0;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x1388;

            FUNC(ESP_CALL)(0x15, 1U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xB00;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x28A0;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x1388;

            FUNC(ESP_CALL)(0x15, 2U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x14);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xB00;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x28A0;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x1388;

            FUNC(ESP_CALL)(0x15, 3U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xB00;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x28A0;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x1388;

            FUNC(ESP_CALL)(0x15, 4U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x14);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xB00;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x28A0;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x1388;

            FUNC(ESP_CALL)(0x15, 5U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xB00;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x28A0;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x1388;

            FUNC(ESP_CALL)(0x15, 6U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x19);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xB00;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x28A0;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x1388;

            FUNC(ESP_CALL)(0x15, 7U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x14);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xB00;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x28A0;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x1388;

            FUNC(ESP_CALL)(0x15, 1U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xB00;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x28A0;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x1388;

            FUNC(ESP_CALL)(0x15, 5U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xB00;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x28A0;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x1388;

            FUNC(ESP_CALL)(0x15, 6U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x14);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xB00;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x1FF) + 0x28A0;
            ScratchPad->Vec.vz = (Api_rand() & 0x1FF) + 0x1388;

            FUNC(ESP_CALL)(0x15, 7U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xF);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x31C;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2AF8;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 7U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xF);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x31C;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2AF8;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 1U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x19);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x31C;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2AF8;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 5U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xF);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x31C;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2AF8;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 6U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x31C;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2AF8;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 7U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x31C;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2AF8;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 1U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x31C;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2AF8;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 6U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x14);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x31C;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2AF8;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 7U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x31C;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2AF8;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 1U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xCF4;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2AF8;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 1U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xF);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xCF4;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2AF8;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 2U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x19);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xCF4;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2AF8;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 3U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xF);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xCF4;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2AF8;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 4U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xCF4;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2AF8;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 5U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 5);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xCF4;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2AF8;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 3U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xF);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xCF4;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2AF8;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 4U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xF);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xCF4;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2AF8;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 5U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xA);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x31C;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2A30;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 7U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x19);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x31C;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2A30;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 1U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 5);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x31C;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2A30;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 6U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xF);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x31C;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2A30;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 7U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0x19);

            ScratchWord[7] = (Api_rand() & 0x1FF) - 0x31C;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2A30;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 1U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 5);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xCF4;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2A30;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 1U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 5);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xCF4;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2A30;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 2U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 5);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xCF4;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2A30;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 3U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xF);

            ScratchWord[7] = (Api_rand() & 0x1FF) + 0xCF4;
            ScratchPad->Vec.vy = -0xA28 - (Api_rand() & 0xFFF);
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) + 0x2A30;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) + 0x1324;

            FUNC(ESP_CALL)(0x15, 4U, ScratchWord[7], Monitor.pId_mat, &ScratchPad->Vec, 0xF);
        }

        if (G.pEm->Move_cnt == 0x91)
        {
            EM0C_FREE_P->flags = EM0C_FREE_P->flags | 1;
        }

        if (G.pEm->Move_cnt == 0x97 || G.pEm->Move_cnt == 0x8F)
        {
            FUNC(SND_SE_ENEM)(0, 0U);
        }
        return;
    case 2:
        FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);  // address passed as an integer
        EM_ROUTINE_HALF2 = 0;
    }
}

VOID Em0c_Sce_pod(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Pos_y = -0xC8;
        G.pEm->Routine_3++;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0;
        G.pEm->Move_no = 6;
        G.pEm->Timer0 = 5U;
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);

    case 1:
        if (--G.pEm->Timer0 == 0)
        {
            G.pEm->Timer0 = 5U;
            G.pEm->Pos_y = G.pEm->Pos_y - 5;
            if (G.pEm->Pos_y == -0x15E)
            {
                G.pEm->Routine_3++;
            }
        }
        break;
    case 2:
        if (--G.pEm->Timer0 == 0)
        {
            G.pEm->Timer0 = 5U;
            G.pEm->Pos_y = G.pEm->Pos_y + 5;
            if (G.pEm->Pos_y == -0xC8)
            {
                G.pEm->Routine_3--;
            }
        }
        break;
    case 3:
        G.pEm->Pos_y = G.pEm->Pos_y + 4;
        if (G.pEm->Pos_y > 0)
        {
            FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);  // address passed as an integer
            G.pEm->Routine_3++;
            G.pEm->Pos_y = 0;
        }
        break;
    }
    if (FUNC(FLG_CK)(&MAIN.Room_flg, ROOM_SLIDE) != 0)  // address passed as an integer
    {
        G.pEm->Routine_3 = 3U;
    }
}

VOID Em0c_Sce_impale(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        G.pEm->Move_no = 5;
        G.pEm->Spd_cur = 0x64;
        FUNC(SET_BASE_POS)(pEm0c_victim);
        G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;
        FUNC(GOTO00)(&pEm0c_victim->Pos_x, 0x400);
        pEm0c_victim->Be_flg = pEm0c_victim->Be_flg | EM_BE_NO_HIT | EM_BE_NO_WALL;
        (ROUTINE_WORD(pEm0c_victim)) = 0x30001;  // INT* view of the field (a plain access changes the code)
        FUNC(SND_SE_ENEM)(1, 0);
        return;
    case 1:
        if (G.pEm->Move_cnt == 6)
        {
            FUNC(SND_SE_ENEM)(4, 0);
        }
        if (G.pEm->Move_cnt != 6 && G.pEm->Move_cnt != 0x5C)
        {
        }
        else
        {
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0x320;

            FUNC(ESP_CALL)(0, 3, 0, &pP_ptr[8].Workm, &ScratchPad->Vec, 0);

            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[8], 0xFF, 0x80880, 0x808080);
        }
        if (G.pEm->Move_cnt < 0x61U)
        {
            if (!((G.pEm->Move_cnt % 7) & 0xFF))
            {
                pP_ptr = G.pEm->pSin_parts_ptr;
                ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
                ScratchPad->Vec.vy = 0x320;

                FUNC(ESP_CALL)(0, 0, 0, &pP_ptr[8].Workm, &ScratchPad->Vec, 0);
            }
        }
        if (G.pEm->Move_cnt == 0x5F)
        {
            FUNC(SND_SE_ENEM)(7, 0);
        }
        FUNC(NULL_POS_SET)(G.pEm, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr);
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);
        if (G.pEm->Spd_cur != 0)
        {
            FUNC(GOTO00)(&pEm0c_victim->Pos_x, 0x10);
        }
        G.pEm->Spd_cur = G.pEm->Spd_cur - G.pEm->Move_cnt * 5;
        if (G.pEm->Spd_cur < 0)
        {
            G.pEm->Spd_cur = 0;
        }
        FUNC(ADD_SPEED_XZ)(0);
        G.pEm->Root_x = G.pEm->Root_x + G.pEm->Spd_x;
        G.pEm->Root_z = G.pEm->Root_z + G.pEm->Spd_z;
        pEm0c_victim->Root_x = pEm0c_victim->Root_x + G.pEm->Spd_x;
        pEm0c_victim->Root_z = pEm0c_victim->Root_z + G.pEm->Spd_z;
        return;
    case 2:
        FUNC(FLG_ON)(&MAIN.Room_flg[0], ROOM_COM);
        EM_ROUTINE_HALF2 = 0;
        EM0C_FREE_P->hit_mask = EM0C_FREE_P->hit_mask & 0xF8;
        G.pEm->Be_flg = G.pEm->Be_flg & ~EM_BE_NO_HIT;
    }
}

VOID Em0c_Sce_rocket(VOID)
{
    typedef struct
    {
        UCHAR pad[0x9C];    // leading room data fields before the per-cut table
        struct
        {
            INT a;              // x target the entity Spd_x is derived from
            USHORT b;           // y value the entity Spd_y is derived from
            UCHAR p6[2];        // padding
            INT c;              // z target the entity Spd_z is derived from
            UCHAR rest[0x20];   // remaining fields of the cut record
        } arr[1];           // cut records indexed by the cut number
    } ROOM_VIEW_DATA;

    PARTS_W* pP_ptr;
    SVECTOR rot;
    VECTOR pos;

    switch (G.pEm->Routine_3)
    {
    case 0:
        EM0C_FREE_P->flags = EM0C_FREE_P->flags | 8;
        G.pEm->Routine_3 = 1U;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Move_no = 0;
        EM0C_FREE_P->flags = EM0C_FREE_P->flags & 0xFD;
        G.pEm->Neck_flg = 0;
        G.pEm->Timer0 = 0;
        FUNC(SRAND)(0x5FE);
        EM0C_FREE_P->heart->Old_z = -0x12C;
        FUNC(SND_SE_STAD)(2, 0x19, 0, &G.pEm->Pos_x);
        FUNC(SND_SE_STAD)(2, 0x1A, 0, &G.pEm->Pos_x);
        FUNC(SND_SE_STAD)(2, 0x1B, 0, &G.pEm->Pos_x);
        pP_ptr = G.pEm->pSin_parts_ptr;
        pP_ptr->Be_flg |= 0x28;
        FUNC(JOINT_ATCK_SET)(pP_ptr, 0x13, 0xC, 3);
        FUNC(JOINT_ATCK_SET)(&pP_ptr[1], 0x13, 0, 3);
        FUNC(JOINT_ATCK_SET)(&pP_ptr[3], 0x13, 0, 3);
        FUNC(JOINT_ATCK_SET)(&pP_ptr[6], 0x13, 0, 3);
        FUNC(JOINT_ATCK_SET)(&pP_ptr[9], 0x13, 0, 3);
        ScratchPad->Sv.vx = 0;
        ScratchPad->Sv.vy = -0x1F4;
        ScratchPad->Sv.vz = 0;
        rot.vx = 0x40;
        rot.vy = 0;
        rot.vz = 0x60;
        Em0c_limb_launch(&pP_ptr[2], &ScratchPad->Sv, &rot, 0xF, 3);
        ScratchPad->Sv.vx = 0xA;
        ScratchPad->Sv.vy = -0x190;
        ScratchPad->Sv.vz = 0x19;
        rot.vx = 0x40;
        rot.vy = 0x60;
        rot.vz = 0;
        Em0c_limb_launch(&pP_ptr[4], &ScratchPad->Sv, &rot, 0xF, 3);
        ScratchPad->Sv.vx = -0x1E;
        ScratchPad->Sv.vy = -0x1C2;
        ScratchPad->Sv.vz = 0xF;
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0x40;
        Em0c_limb_launch(&pP_ptr[7], &ScratchPad->Sv, &rot, 0xF, 3);
        ScratchPad->Sv.vx = -0x14;
        ScratchPad->Sv.vy = -0x15E;
        ScratchPad->Sv.vz = -0xA;
        rot.vx = 0;
        rot.vy = 0x60;
        rot.vz = 0;
        Em0c_limb_launch(&pP_ptr[10], &ScratchPad->Sv, &rot, 0xF, 3);
        ScratchPad->Sv.vx = -0x14;
        ScratchPad->Sv.vy = -0x12C;
        ScratchPad->Sv.vz = 0xF;
        rot.vx = 0;
        rot.vy = 0x40;
        rot.vz = 0x40;
        Em0c_limb_launch(&pP_ptr[12], &ScratchPad->Sv, &rot, 0xF, 3);
        G.pEm->Timer0 = 0;
        G.pEm->Spd_x = (((ROOM_VIEW_DATA*)G.pRoom)->arr[G.SaveData.Cut_no].a - G.pEm->Pos_x) / 50;  // reinterprets the memory as ROOM_VIEW_DATA
        G.pEm->Spd_y = ((ROOM_VIEW_DATA*)G.pRoom)->arr[G.SaveData.Cut_no].b - (G.pEm->Pos_y - 0x3E8) / 50;  // reinterprets the memory as ROOM_VIEW_DATA
        G.pEm->Spd_z = (((ROOM_VIEW_DATA*)G.pRoom)->arr[G.SaveData.Cut_no].c - G.pEm->Pos_z) / 50;  // reinterprets the memory as ROOM_VIEW_DATA

        FUNC(PAD_VIB_SET0)(5, 0);
        FUNC(PAD_VIB_FADE_SET)(5, 0x64, 0xFA, 0);
        FUNC(PAD_VIB_SET1)(0xF, 0xFA, 5);
        FUNC(PAD_VIB_FADE_SET)(0x1E, 0xFA, 0x3C, 0x14);
        FUNC(PAD_VIB_SET1)(0x14, 0x3C, 0x32);
        FUNC(PAD_VIB_FADE_SET)(0x4B, 0x3C, 0, 0x46);
        FUNC(PAD_VIB_SET0)(2, 7);
        FUNC(PAD_VIB_SET0)(0xF, 0xB);

    case 1:
        if (G.pEm->Timer0 == 2)
        {
            FUNC(SND_SE_STAD)(2, 0x19, 0, &G.pEm->Pos_x);
            FUNC(SND_SE_STAD)(2, 0x1A, 0, &G.pEm->Pos_x);
            FUNC(SND_SE_STAD)(2, 0x1B, 0, &G.pEm->Pos_x);
        }
        if (G.pEm->Timer0 == 6)
        {
            FUNC(SND_SE_STAD)(2, 0x19, 0, &G.pEm->Pos_x);
            FUNC(SND_SE_STAD)(2, 0x1A, 0, &G.pEm->Pos_x);
            FUNC(SND_SE_STAD)(2, 0x1B, 0, &G.pEm->Pos_x);
        }
        if (G.pEm->Timer0 == 9)
        {
            FUNC(SND_SE_STAD)(2, 0x19, 0, &G.pEm->Pos_x);
        }
        if (G.pEm->Move_cnt < 0xEU)
        {
            pos.vx = G.pEm->Pos_x + G.pEm->Timer0 * 0xE7A / 12;
            pos.vy = G.pEm->Pos_y + -(G.pEm->Timer0 * 0xFB0) / 12 - 0x9C4;
            pos.vz = G.pEm->Pos_z + -(G.pEm->Timer0 * 0x9AE) / 12;
            ScratchPad->Vec.vx = pos.vx;
            ScratchPad->Vec.vy = pos.vy;
            ScratchPad->Vec.vz = pos.vz;

            FUNC(ESP_CALL)(0xE, 3, 0, 0, &pos, 0xA);

            ScratchPad->Vec.vx = pos.vx + 0x40 - (Api_rand() & 0x7F);
            ScratchPad->Vec.vy = pos.vy + 0x40 - (Api_rand() & 0x7F);
            ScratchPad->Vec.vz = pos.vz + 0x40 - (Api_rand() & 0x7F);

            FUNC(ESP_CALL)(0xE, 3, 0, 0, &ScratchPad->Vec, 0x14);

            ++G.pEm->Timer0;
            if (G.pEm->Timer0 >= 0xB)
            {
                G.pEm->Timer0 = 0xA;
                ScratchPad->Vec.vx = pos.vx + 0x100 - (Api_rand() & 0x1FF);
                ScratchPad->Vec.vy = pos.vy + 0x40 - (Api_rand() & 0x7F);
                ScratchPad->Vec.vz = pos.vz + 0x100 - (Api_rand() & 0x1FF);

                FUNC(ESP_CALL)(0xE, 3, 0, 0, &ScratchPad->Vec, 0xF);

                ScratchPad->Vec.vx = pos.vx + 0x100 - (Api_rand() & 0x1FF);
                ScratchPad->Vec.vy = pos.vy + 0x40 - (Api_rand() & 0x7F);
                ScratchPad->Vec.vz = pos.vz + 0x100 - (Api_rand() & 0x1FF);

                FUNC(ESP_CALL)(0xE, 0xB, 0, 0, &ScratchPad->Vec, 0xA);

                EM0C_FREE_P->flags = EM0C_FREE_P->flags & 0xFE;
            }
        }
        pP_ptr = G.pEm->pSin_parts_ptr;
        if (G.pEm->Move_cnt == 8)
        {
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[2], 0, 0x102810, 0x202030);
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[4], 0, 0x102810, 0x202030);
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[5], 0, 0x102810, 0x202030);
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[7], 0, 0x102810, 0x202030);
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[8], 0, 0x102810, 0x202030);
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[10], 0, 0x102810, 0x202030);
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[11], 0, 0x102810, 0x202030);
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[12], 0, 0x102810, 0x202030);
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[13], 0, 0x102810, 0x202030);
            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[14], 0, 0x102810, 0x202030);
        }
        if (G.pEm->Move_cnt < 0xAU && !(G.pEm->Move_cnt & 7))
        {
            ScratchPad->Vec.vx = (Api_rand() & 0x3FF) - 0x1F4;
            ScratchPad->Vec.vy = Api_rand() & 0x3FF;
            ScratchPad->Vec.vz = (Api_rand() & 0x3FF) - 0x1F4;

            FUNC(ESP_CALL)(0xE, 3, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0x1E);
            FUNC(ESP_CALL)(0xE, 3, 0, &pP_ptr[4].Workm, &ScratchPad->Vec, 0x1E);
            FUNC(ESP_CALL)(0xE, 3, 0, &pP_ptr[7].Workm, &ScratchPad->Vec, 0x1E);
            FUNC(ESP_CALL)(0xE, 3, 0, &pP_ptr[10].Workm, &ScratchPad->Vec, 0x1E);
            FUNC(ESP_CALL)(0xE, 3, 0, &pP_ptr[12].Workm, &ScratchPad->Vec, 0x1E);
        }
        if (G.pEm->Move_cnt >= 0x11U && G.pEm->Move_cnt < 0x13U)
        {
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x3E8;
            ScratchPad->Vec.vy = -0x3E8 - (Api_rand() & 0x7FF);
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) - 0x3E8;

            FUNC(ESP_CALL)(9, 5, 0, G.pEm->M, &ScratchPad->Vec, 0x1E);

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x3E8;
            ScratchPad->Vec.vy = -0x3E8 - (Api_rand() & 0x7FF);
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) - 0x3E8;

            FUNC(ESP_CALL)(9, 0x15, 0, G.pEm->M, &ScratchPad->Vec, 0x1E);

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x3E8;
            ScratchPad->Vec.vy = -0x3E8 - (Api_rand() & 0x7FF);
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) - 0x3E8;

            FUNC(ESP_CALL)(9, 0x15, 0, G.pEm->M, &ScratchPad->Vec, 0x1E);

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x3E8;
            ScratchPad->Vec.vy = -0x3E8 - (Api_rand() & 0x7FF);
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) - 0x3E8;

            FUNC(ESP_CALL)(0xE, 1, 0, G.pEm->M, &ScratchPad->Vec, 0x1E);

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x3E8;
            ScratchPad->Vec.vy = -0x3E8 - (Api_rand() & 0x7FF);
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) - 0x3E8;

            FUNC(ESP_CALL)(0xE, 0x11, 0, G.pEm->M, &ScratchPad->Vec, 0x1E);

            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x3E8;
            ScratchPad->Vec.vy = -0x3E8 - (Api_rand() & 0x7FF);
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) - 0x3E8;

            FUNC(ESP_CALL)(0xE, 1, 0, G.pEm->M, &ScratchPad->Vec, 0x1E);
        }
        if (G.pEm->Move_cnt >= 0x13U && !((G.pEm->Move_cnt + 1) & 3))
        {
            ScratchPad->Vec.vx = (Api_rand() & 0x3FF) - 0x1F4;
            ScratchPad->Vec.vy = Api_rand() & 0x3FF;
            ScratchPad->Vec.vz = (Api_rand() & 0x3FF) - 0x1F4;

            FUNC(ESP_CALL)(9, 0xD, 0, &pP_ptr[2].Workm, &ScratchPad->Vec, 0x1E);
            FUNC(ESP_CALL)(9, 0xD, 0, &pP_ptr[4].Workm, &ScratchPad->Vec, 0x1E);
            FUNC(ESP_CALL)(9, 0xD, 0, &pP_ptr[7].Workm, &ScratchPad->Vec, 0x1E);
            FUNC(ESP_CALL)(9, 0xD, 0, &pP_ptr[10].Workm, &ScratchPad->Vec, 0x1E);
            FUNC(ESP_CALL)(9, 0xD, 0, &pP_ptr[12].Workm, &ScratchPad->Vec, 0x1E);
        }
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)
        {
            G.pEm->Routine_3 = 2U;
            G.pEm->Timer0 = 0;
        }
        if (EM0C_FREE_P->heart->Old_y < -0x190)
        {
            EM0C_FREE_P->heart->Pos_y = EM0C_FREE_P->heart->Pos_y + EM0C_FREE_P->heart->Old_z;
            EM0C_FREE_P->heart->Cdir_z = EM0C_FREE_P->heart->Cdir_z + 0x18;
        }
        EM0C_FREE_P->heart->Old_z = EM0C_FREE_P->heart->Old_z + 0xF;
        Em0c_Limb_move(&pP_ptr[2]);
        Em0c_Limb_move(&pP_ptr[4]);
        Em0c_Limb_move(&pP_ptr[7]);
        Em0c_Limb_move(&pP_ptr[10]);
        Em0c_Limb_move(&pP_ptr[12]);
        return;
    case 2:
        if (G.pEm->Timer0 >= 0xC9)
        {
            G.pEm->Routine_3 = 3U;
            G.pEm->Life = -1;
        }
        if ((++G.pEm->Timer0 & 3) == 0)
        {
            ScratchPad->Vec.vx = (Api_rand() & 0x7FF) - 0x3E8;
            ScratchPad->Vec.vy = 0;
            ScratchPad->Vec.vz = (Api_rand() & 0x7FF) - 0x3E8;

            FUNC(ESP_CALL)(9, 0xD, 0, G.pEm->M, &ScratchPad->Vec, 0x1E);
        }
        if ((G.pEm->Timer0 & 7) == 0)
        {
            ScratchPad->Vec.vx = 0;
            ScratchPad->Vec.vy = 0x3E8;
            ScratchPad->Vec.vz = 0;
            pP_ptr = G.pEm->pSin_parts_ptr;

            FUNC(ESP_CALL)(9, 0xD, 0, &pP_ptr[4].Workm, &ScratchPad->Vec, 0x1E);
            FUNC(ESP_CALL)(9, 0xD, 0, &pP_ptr[7].Workm, &ScratchPad->Vec, 0x1E);
            FUNC(ESP_CALL)(9, 0xD, 0, &pP_ptr[10].Workm, &ScratchPad->Vec, 0x1E);
            FUNC(ESP_CALL)(9, 0xD, 0, &pP_ptr[12].Workm, &ScratchPad->Vec, 0x1E);

            return;
        }
    }
}

VOID Em0c_limb_launch(PARTS_W* pP_ptr, SVECTOR* pVel, VOID* pRot, USHORT gravity, UCHAR bounces)
{
    pP_ptr->Be_flg = pP_ptr->Be_flg | 8;
    pP_ptr->Free0 = pVel->vy;
    pP_ptr->Free1 = pVel->vx;
    pP_ptr->Free2 = pVel->vz;
    pP_ptr->Free3 = gravity;
    pP_ptr->Free4 = bounces;
    pP_ptr->Cdir = *(SVECTOR*)pRot;  // reinterprets the memory as SVECTOR
}

VOID Em0c_Limb_move(PARTS_W* pP_ptr)
{
    if (pP_ptr->Free4 == 0)
    {
        return;
    }
    pP_ptr->Workm.t[0] = pP_ptr->Workm.t[0] + pP_ptr->Free1;
    pP_ptr->Workm.t[2] = pP_ptr->Workm.t[2] + pP_ptr->Free2;
    pP_ptr->Free0 = pP_ptr->Free0 + pP_ptr->Free3;
    pP_ptr->Workm.t[1] = pP_ptr->Workm.t[1] + pP_ptr->Free0;
    if (pP_ptr->Workm.t[1] >= -0xC7)
    {
        pP_ptr->Workm.t[1] = pP_ptr->Workm.t[1] - pP_ptr->Free0;
        pP_ptr->Free0 /= -2;
        pP_ptr->Free4--;
    }
    FUNC(ROT_MATRIX)(&pP_ptr->Cdir, &pP_ptr->Coord);
    FUNC(MUL_MATRIX2)(&pP_ptr->Coord, &pP_ptr->Workm);
}

VOID Em0c_Sce_anim(VOID)
{
    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3++;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        G.pEm->Move_no = 0xA;
        G.pEm->Spd_cur = 0;

        FUNC(PAD_VIB_SET1)(0x1E, 0x50, 0);

    case 1:
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80) != 0)
        {
            FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);  // address passed as an integer
            EM_ROUTINE_HALF2 = 0;
        }
    }
}

VOID Em0c_Sce_face(VOID)
{
    if (G.pEm->Routine_3 == 0)
    {
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 0x1F;
        G.pEm->Damage_no = 0;
        if (FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x400) != 0)  // address passed as an integer
        {
            G.pEm->Move_no = 1;
        }
        G.pEm->Routine_3 = 1U;
        G.pEm->Timer0 = (Api_rand() & 0x3F) + 0x28;
    }
    if (G.pEm->Type & 1)
    {
        ScratchWord[7] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x14);
    }
    else
    {
        ScratchWord[7] = FUNC(GOTO00_CK)(&MAIN.Player.Pos_x, 0x28);
    }
    if ((G.pEm->Timer0-- == 0) || ScratchWord[7] == 0)
    {
        FUNC(FLG_ON)(&MAIN.Room_flg, G.pEm->Set_flg_no);  // address passed as an integer
        EM_ROUTINE_HALF2 = 0;
        G.pEm->Dest_x = G.Player.Pos_x;
        G.pEm->Dest_z = G.Player.Pos_z;
    }
    else
    {
        FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);
        G.pEm->Cdir_y += ScratchWord[7];
    }
    if (G.pEm->Move_cnt == 3 || G.pEm->Move_cnt == 0x24)
    {
        FUNC(SND_SE_ENEM)(0, 0);
    }
}

VOID Em0c_Sce_strike(VOID)
{
    PARTS_W* pP_ptr;

    switch (G.pEm->Routine_3)
    {
    case 0:
        G.pEm->Routine_3++;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Move_no = 4;
        G.pEm->Hokan_flg = 7;
        FUNC(SND_SE_ENEM)(1, 0);

    case 1:
        G.pEm->Routine_3 = G.pEm->Routine_3 + FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);
        if (G.pEm->Move_cnt == 0xB)
        {
            (ROUTINE_WORD(pEm0c_victim)) = 0x10001;  // INT* view of the field (a plain access changes the code)
            pP_ptr = G.pEm->pSin_parts_ptr;
            ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
            ScratchPad->Vec.vy = 0x320;

            FUNC(ESP_CALL)(0, 3, 0, &pP_ptr[8].Workm, &ScratchPad->Vec, 0);

            FUNC(MODEL_COLOR_PLUS)(&pP_ptr[8], 0xFF, 0x80880, 0x808080);
            FUNC(SND_SE_ENEM)(2, 0);
        }
        break;
    case 2:
        FUNC(FLG_ON)(&MAIN.Room_flg, ROOM_COM);  // address passed as an integer
        EM_ROUTINE_HALF2 = 0;
    }
}

static AT_TBL Em0c_sca00 = { 0x8000, 0, -2000, 0, 0x7D0, 0x320 };  // collision volume record: flags 0x8000, y offset -2000, half height 2000, radius 800
AT_TBL* pEm0c_sca = &Em0c_sca00;    // collision volume used by Em0c_Init
PARTS_W* pEm0c_ghost = 0;                      // two claw ghost joint copies, allocated in Em0c_Init
EM0C_SCALE_DATA Em0c_ghost_scale = { 0x0BB8, 0x012C };  // current claw ghost scale
SCHAR Em0c_ghost_add = 0xC8;                // per-tick change of the claw ghost scale (-56 at start)
VOID* pEm0c_trail = 0;                             // trail record block, allocated in Em0c_Init
EM0C_TRAIL_DATA Em0c_trail_timer = { 0x0000, 8 };      // trail lifetime and segment count
SVECTOR Em0c_trail_near = { 0, -0x012C, 0, 0 };         // near edge point of the trail, relative to the claw joint
SVECTOR Em0c_trail_far = { 0, 0x05DC, 0, 0 };           // far edge point of the trail, relative to the claw joint
// state handlers by entity state: 0=init, 1=think/act, 2=hit, 3=forced, 4=empty, 8=scripted
VOID (*Em0c_routine_0[9])(VOID) =
{
    Em0c_Init,           // 0 //
    Em0c_Move,         // 1 //
    Em0c_Damage,      // 2 //
    Em0c_Die,   // 3 //
    Em0c_Dead,     // 4 //
    0,                     // 5 //
    0,                     // 6 //
    0,                     // 7 //
    Em0c_Scenario,      // 8 //
};
// u offset of the body texture scroll per step (the first 30 entries are used)
SCHAR Em0c_uv_scroll_u_tbl[32] =
{
    0x00,    // 0 //
    0x00,    // 1 //
    0x00,    // 2 //
    0x3B,    // 3 //
    0x00,    // 4 //
    0x00,    // 5 //
    0x00,    // 6 //
    -0x3B,   // 7 //
    0x00,    // 8 //
    0x00,    // 9 //
    0x00,    // 10 //
    0x00,    // 11 //
    0x00,    // 12 //
    0x00,    // 13 //
    0x3B,    // 14 //
    0x00,    // 15 //
    0x00,    // 16 //
    0x00,    // 17 //
    0x00,    // 18 //
    -0x3B,   // 19 //
    0x00,    // 20 //
    0x00,    // 21 //
    0x00,    // 22 //
    0x00,    // 23 //
    0x3B,    // 24 //
    0x00,    // 25 //
    0x00,    // 26 //
    -0x3B,   // 27 //
    0x00,    // 28 //
    0x00,    // 29 //
    0x00,    // 30 //
    0x00,    // 31 //
};
// v offset of the body texture scroll per step
SHORT Em0c_uv_scroll_v_tbl[30] =
{
    0x0000,    // 0 //
    0x0000,    // 1 //
    0x0000,    // 2 //
    0x0093,    // 3 //
    0x0000,    // 4 //
    0x0000,    // 5 //
    0x0000,    // 6 //
    -0x0093,   // 7 //
    0x0000,    // 8 //
    0x0000,    // 9 //
    0x0000,    // 10 //
    0x0000,    // 11 //
    0x0000,    // 12 //
    0x0000,    // 13 //
    0x0093,    // 14 //
    0x0000,    // 15 //
    0x0000,    // 16 //
    0x0000,    // 17 //
    0x0000,    // 18 //
    -0x0093,   // 19 //
    0x0000,    // 20 //
    0x0000,    // 21 //
    0x0000,    // 22 //
    0x0000,    // 23 //
    0x0093,    // 24 //
    0x0000,    // 25 //
    0x0000,    // 26 //
    -0x0093,   // 27 //
    0x0000,    // 28 //
    0x0000,    // 29 //
};
// forced behaviors by Routine_2: 0=restrained, 1=yield to script
VOID (*Em0c_die[2])(VOID) =
{
    Em0c_Die_restrain,     // 0 //
    Em0c_Die_sce,   // 1 //
};
// state-1 behaviors by Routine_2; values 9 to 0xB continue into the think table that follows
VOID (*Em0c_mv[9])(VOID) =
{
    Em0c_Move_pause,         // 0 //
    Em0c_Move_walk,          // 1 //
    Em0c_Move_nop,           // 2 //
    Em0c_Move_swipe,    // 3 //
    Em0c_Move_slash,    // 4 //
    Em0c_Move_thrust,   // 5 //
    Em0c_Move_backhand,      // 6 //
    Em0c_Move_impale,        // 7 //
    Em0c_Move_charge,        // 8 //
};
// think functions by id >> 2: 0=erupt, 1=rush, 2=rise, 3=laboratory, 4=roof
VOID (*Em0c_mv_think[10])(VOID) =
{
    Em0c_Move_erupt,   // 0 //
    Em0c_Move_rush,    // 1 //
    Em0c_Move_rise,    // 2 //
    Em0c_think_lab,        // 3 //
    Em0c_think_roof,       // 4 //
    0,                       // 5 //
    0,                       // 6 //
    0,                       // 7 //
    0,                       // 8 //
    0,                       // 9 //
};
// player reactions to a Tyrant attack by Routine_2
VOID (*Pldamage_Em0c_tbl[3])(VOID) =
{
    Em0c_pldm00,   // 0 //
    Em0c_pldm01,   // 1 //
    Em0c_pldm02,   // 2 //
};
// per-tick heart scale change over one heartbeat, indexed by the heart's tick counter
SCHAR Em0c_heartbeat_tbl[24] =
{
    -0x7A,   // 0 //
    -0x72,   // 1 //
    -0x62,   // 2 //
    -0x4A,   // 3 //
    0x00,    // 4 //
    0x52,    // 5 //
    0x48,    // 6 //
    0x34,    // 7 //
    0x00,    // 8 //
    -0x34,   // 9 //
    -0x48,   // 10 //
    -0x52,   // 11 //
    0x00,    // 12 //
    0x4A,    // 13 //
    0x62,    // 14 //
    0x72,    // 15 //
    0x7A,    // 16 //
    0x66,    // 17 //
    0x3E,    // 18 //
    0x00,    // 19 //
    -0x3E,   // 20 //
    -0x66,   // 21 //
    0x00,    // 22 //
    0x00,    // 23 //
};
// indices of the body model primitives scrolled by Em0c_scroll_body_uvs, ascending; the trailing 0 is never matched
USHORT Em0c_uv_scroll_poly_tbl[10] =
{
    0x0015,   // 0 //
    0x0016,   // 1 //
    0x0017,   // 2 //
    0x002D,   // 3 //
    0x0041,   // 4 //
    0x0056,   // 5 //
    0x0057,   // 6 //
    0x0080,   // 7 //
    0x0085,   // 8 //
    0x0000,   // 9 //
};
EM0C_CLUT_DATA Em0c_clut_ofs = { 0x0000, 0x0100 };  // offset advanced by the unused CLUT routine; never read
ENEMY_WORK* pEm0c_victim = 0;                      // victim entity of the scripted impale and strike
// scripted behaviors by Routine_2
VOID (*Em0c_sce_mv[17])(VOID) =
{
    Em0c_Sce_idle,           // 0 //
    0,                                  // 1 //
    Em0c_Sce_walk,        // 2 //
    0,                                  // 3 //
    0,                                  // 4 //
    0,                                  // 5 //
    0,                                  // 6 //
    0,                                  // 7 //
    0,                                  // 8 //
    0,                                  // 9 //
    Em0c_Sce_glass,    // 10 //
    Em0c_Sce_pod,            // 11 //
    Em0c_Sce_impale,         // 12 //
    Em0c_Sce_rocket,   // 13 //
    Em0c_Sce_anim,           // 14 //
    Em0c_Sce_face,    // 15 //
    Em0c_Sce_strike,         // 16 //
};
