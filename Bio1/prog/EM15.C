/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM15.C
 */

#include "EM15.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Em15_Main(VOID)
{
    Em15_routine_0[G.pEm->Routine_0]();
    G.pEm->Coord_flg = 0;
    G.pEm->Zone_in_flg = (!(G.pEm->Type & 0x80)) ? FUNC(HIT_CK_POINT4)(&G.pEm->Pos_x, G.Cccut) : 0;  // Hit_ck_point4
}

VOID Em15_Init(VOID)
{
    G.pEm->Routine_0++;
    G.pEm->Routine_1 = 0;
    G.pEm->Life = 1;
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_WALL;
    G.pEm->Move_no = 0;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0;
    FUNC(INIT_PARTS_WORK)();  // Init_Parts_work
    FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);  // Joint_move
    G.pEm->Neck_point_x = G.pEm->Pos_x;
    G.pEm->Neck_point_y = G.pEm->Pos_y;
    G.pEm->Neck_point_z = G.pEm->Pos_z;
    EM15_EF(pos_x) = G.pEm->Pos_x << 0x10;
    EM15_EF(pos_z) = G.pEm->Pos_z << 0x10;
}

VOID Em15_Move(VOID)
{
    if ((G.pEm->Type & 0x80) || (G.pEm->Type & 0x20))
    {
        G.pEm->Routine_1 = 0;
    }
    else
    {
        if (G.pEm->Type & 0x40)
        {
            G.pEm->Routine_1 = 0;
            G.pEm->Type = G.pEm->Type & 0xBF;
        }
        Em15_mv_tbl[(G.pEm->Type & 0xF)]();
    }
}

VOID Em15_mv00(VOID)
{
    if (G.pEm->Routine_1 == 0)
    {
        G.pEm->Routine_1++;
        G.pEm->Hokan_flg = G.pEm->Move_no != 0 ? 7 : 0;
        G.pEm->Move_no = 0U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Timer0 = 8;
        Em15_spd_set(G.pEm->Pos_x, G.pEm->Pos_z, G.pEm->Neck_point_x, G.pEm->Neck_point_z, G.pEm->Timer0);
    }
    if (G.pEm->Timer0 != 0)
    {
        Em15_pos_add();
        G.pEm->Timer0--;
    }
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);  // Joint_move
}

VOID Em15_mv01(VOID)
{
    Em15_mv01_tbl[G.pEm->Routine_1]();
}

VOID Em15_mv01_set(VOID)
{
    G.pEm->Routine_1++;
    Em15_reach_set(-0x32, -0x32, 1);
}

VOID Em15_mv_move(VOID)
{
    EM15_EF(pos_x) = EM15_EF(pos_x) + EM15_EF(vel_x);
    EM15_EF(pos_z) = EM15_EF(pos_z) + EM15_EF(vel_z);
    G.pEm->Pos_x = EM15_EF(pos_x) >> 0x10;
    G.pEm->Pos_z = EM15_EF(pos_z) >> 0x10;
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)  // Joint_move
    {
        G.pEm->Type = G.pEm->Type | 0x20;
        G.pEm->Routine_1 = 0;
    }
    if (G.Player.Id & 1)
    {
        if (G.pEm->Move_cnt != 3 && G.pEm->Move_cnt != 0xD)
        {
        }
        else
        {
            FUNC(SND_SE_PLAY)(2, 0x18, 0);  // Snd_se_play
        }
        return;
    }
    if (G.pEm->Move_cnt == 8)
    {
        FUNC(SND_SE_PLAY)(2, 0x18, 0);  // Snd_se_play
    }
}

VOID Em15_mv02(VOID)
{
    Em15_mv02_tbl[G.pEm->Routine_1]();
}

VOID Em15_mv02_set(VOID)
{
    G.pEm->Routine_1++;
    Em15_reach_set(0x32, 0, G.Player.Id & 1 ? 2 : 1);
}

VOID Em15_mv03(VOID)
{
    Em15_mv03_tbl[G.pEm->Routine_1]();
}

VOID Em15_mv03_set(VOID)
{
    G.pEm->Routine_1++;
    Em15_reach_set(-0x14, 0x32, G.Player.Id & 1 ? 3 : 2);
}

VOID Em15_mv04(VOID)
{
    Em15_mv04_tbl[G.pEm->Routine_1]();
}

VOID Em15_mv04_set(VOID)
{
    SHORT off_z;

    G.pEm->Routine_1++;
    G.pEm->Move_no = 4;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 7;
    if (G.Player.Id & 1)
    {
        EM15_EF(Slide_cnt) = 0xA;
        off_z = 0xC8;
    }
    else
    {
        EM15_EF(Slide_cnt) = 0x12;
        off_z = 0x12C;
    }
    Em15_spd_set(G.pEm->Pos_x, G.pEm->Pos_z, G.pEm->Pos_x, G.pEm->Neck_point_z - off_z, EM15_EF(Slide_cnt));
}

VOID Em15_mv04_key(VOID)
{
    if (G.pEm->Move_cnt < EM15_EF(Slide_cnt))
    {
        Em15_pos_add();
    }
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)  // Joint_move
    {
        G.pEm->Type = G.pEm->Type | 0x20;
        G.pEm->Routine_1 = 0;
    }
    if ((G.Player.Id & 1) && G.pEm->Move_cnt == 0xD)
    {
        FUNC(SND_SE_PLAY)(2, 0x1B, 0);  // Snd_se_play
    }
}

VOID Em15_mv04_end(VOID)
{
    Em15_pos_add();
    if (--G.pEm->Timer0 == 0)
    {
        G.pEm->Type = G.pEm->Type | 0x20;
        G.pEm->Routine_1 = 0;
    }
}

VOID Em15_mv05(VOID)
{
    Em15_mv05_tbl[G.pEm->Routine_1]();
}

VOID Em15_mv05_set(VOID)
{
    G.pEm->Routine_1++;
    G.pEm->Routine_2 = 0;
}

VOID Em15_mv05_run(VOID)
{
    if (!(G.Player.Id & 1))
    {
        Em15_mv05_chris_tbl[G.pEm->Routine_2]();
    }
    else
    {
        Em15_mv05_jill_tbl[G.pEm->Routine_2]();
    }
}

VOID Em15_mv05_slide(VOID)
{
    G.pEm->Routine_2++;
    G.pEm->Timer0 = 4;
    Em15_spd_set(G.pEm->Pos_x, G.pEm->Pos_z, G.pEm->Neck_point_x, G.pEm->Neck_point_z - 0x50, G.pEm->Timer0);
}

VOID Em15_mv05_wait(VOID)
{
    Em15_pos_add();
    if (--G.pEm->Timer0 == 0)
    {
        G.pEm->Routine_2++;
        G.pEm->Move_no = 5;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
        G.pEm->Timer0 = 4U;
        Em15_spd_set(G.pEm->Pos_x, G.pEm->Pos_z, G.pEm->Neck_point_x - 0x64, G.pEm->Pos_z, G.pEm->Timer0);
    }
}

VOID Em15_mv05_play(VOID)
{
    if (G.pEm->Timer0 != 0)
    {
        Em15_pos_add();
        G.pEm->Timer0--;
    }
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)  // Joint_move
    {
        G.pEm->Type = G.pEm->Type | 0x20;
        G.pEm->Routine_1 = 0;
    }
}

VOID Em15_mv05_jill(VOID)
{
    G.pEm->Routine_2++;
    G.pEm->Move_no = 5;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 7;
}

VOID Em15_mv06(VOID)
{
    Em15_mv06_tbl[G.pEm->Routine_1]();
}

VOID Em15_mv06_set(VOID)
{
    G.pEm->Routine_1++;
    Em15_reach_set(-0x28, 0, 6);
    Em15_spd_set(G.pEm->Pos_x, G.pEm->Pos_z, G.pEm->Neck_point_x - 0x28, G.pEm->Neck_point_z - 0x28, 4);
    G.pEm->Timer0 = 4;
}

VOID Em15_lever_move(VOID)
{
    EM15_EF(pos_x) = EM15_EF(pos_x) + EM15_EF(vel_x);
    EM15_EF(pos_z) = EM15_EF(pos_z) + EM15_EF(vel_z);
    G.pEm->Pos_x = EM15_EF(pos_x) >> 0x10;
    G.pEm->Pos_z = EM15_EF(pos_z) >> 0x10;
    if (--G.pEm->Timer0 == 0)
    {
        G.pEm->Routine_1++;
        G.pEm->Move_no = 6;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
    }
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);  // Joint_move
}

VOID Em15_lever_pull(VOID)
{
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)  // Joint_move
    {
        G.pEm->Type = G.pEm->Type | 0x20;
    }
    if (G.pEm->Move_cnt == 9)
    {
        FUNC(SND_SE_PLAY)(2, 0x18, 0);  // Snd_se_play
    }
}

VOID Em15_mv07(VOID)
{
    Em15_mv07_tbl[G.pEm->Routine_1]();
}

VOID Em15_mv07_set(VOID)
{
    G.pEm->Routine_1++;
    G.pEm->Timer0 = 0xA0;
    G.pEm->Hokan_flg = G.pEm->Move_no != 0 ? 7 : 0;
    G.pEm->Move_no = 0U;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
}

VOID Em15_mv07_drop(VOID)
{
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);  // Joint_move
    if (--G.pEm->Timer0 != 0)
    {
        return;
    }
    G.pEm->Routine_1++;
    G.pEm->Timer0 = 0xCU;
    EM15_EF(pos_y) = G.pEm->Pos_y << 0x10;
    EM15_EF(vel_y) = 0xFFF40000;
    Em15_spd_set(G.pEm->Pos_x, G.pEm->Pos_z, G.pEm->Neck_point_x - 0x64, G.pEm->Neck_point_z - 0x190, G.pEm->Timer0);
}

VOID Em15_mv07_fall(VOID)
{
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);  // Joint_move
    if (G.pEm->Timer0 != 0)
    {
        Em15_pos_add();
        EM15_EF(pos_y) = EM15_EF(pos_y) + EM15_EF(vel_y);
        EM15_EF(vel_y) = EM15_EF(vel_y) + 0x28000;
        if ((G.pEm->Pos_y = EM15_EF(pos_y) >> 0x10) > G.pEm->Neck_point_y)
        {
            G.pEm->Pos_y = G.pEm->Neck_point_y;
        }
        G.pEm->Timer0--;
        return;
    }
    G.pEm->Routine_1++;
    G.pEm->Pos_y = G.pEm->Neck_point_y;
    G.pEm->Move_no = 2;
    G.pEm->Move_cnt = 0xE;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 7;
    G.pEm->Timer0 = 0x5A;
}

VOID Em15_mv07_loop(VOID)
{
    UCHAR end_frame;
    UCHAR Jill_flg;

    Jill_flg = G.Player.Id & 1;
    end_frame = Jill_flg != 0 ? 0xA : 0xE;
    if (G.pEm->Move_cnt < end_frame)
    {
        FUNC(JOINT_MOVE)(0U, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);  // Joint_move
    }
    if (--G.pEm->Timer0 == 0)
    {
        G.pEm->Timer0 = Jill_flg == 0 ? (Api_rand() & 0x1F) + 0xA : 0xA;  // rand
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
    }
}

VOID Em15_mv08(VOID)
{
    Em15_mv08_tbl[G.pEm->Routine_1]();
}

VOID Em15_mv08_set(VOID)
{
    G.pEm->Routine_1++;
    Em15_reach_set(-0x28, 0, 6);
    Em15_spd_set(G.pEm->Pos_x, G.pEm->Pos_z, G.pEm->Neck_point_x - 0x28, G.pEm->Neck_point_z - 0x8C, 4);
    G.pEm->Timer0 = 4;
}

VOID Em15_reach_set(INT Ofs_x, INT off_z, UCHAR Mot_no)
{
    UCHAR Mot_cur;

    Mot_cur = G.pEm->Move_no;
    if (Mot_no == 2 && Mot_cur == 1)
    {
        Mot_no = 3;
    }
    else if (Mot_no == 3 && Mot_cur == 3)
    {
        Mot_no = 2;
    }
    G.pEm->Move_no = Mot_no;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 7;
    Em15_spd_set(G.pEm->Pos_x, G.pEm->Pos_z, G.pEm->Neck_point_x + Ofs_x, G.pEm->Neck_point_z + off_z, 0xF);
}

VOID Em15_spd_set(INT Src_x, INT Src_z, INT Dst_x, INT Dst_z, SHORT frames)
{
    EM15_EF(vel_x) = ((Dst_x - Src_x) << 0x10) / frames;
    EM15_EF(vel_z) = ((Dst_z - Src_z) << 0x10) / frames;
    EM15_EF(pos_x) = G.pEm->Pos_x << 0x10;
    EM15_EF(pos_z) = G.pEm->Pos_z << 0x10;
}

VOID Em15_pos_add(VOID)
{
    EM15_EF(pos_x) = EM15_EF(pos_x) + EM15_EF(vel_x);
    EM15_EF(pos_z) = EM15_EF(pos_z) + EM15_EF(vel_z);
    G.pEm->Pos_x = EM15_EF(pos_x) >> 0x10;
    G.pEm->Pos_z = EM15_EF(pos_z) >> 0x10;
}

VOID (*Em15_routine_0[2])(VOID) = { Em15_Init, Em15_Move };    // left arm state handlers by entity state
// left arm commands by Type & 0xF
VOID (*Em15_mv_tbl[9])(VOID) =
{
    Em15_mv00,      // 0 //
    Em15_mv01,   // 1 //
    Em15_mv02,   // 2 //
    Em15_mv03,   // 3 //
    Em15_mv04,     // 4 //
    Em15_mv05,          // 5 //
    Em15_mv06,          // 6 //
    Em15_mv07,          // 7 //
    Em15_mv08,    // 8 //
};
VOID (*Em15_mv01_tbl[2])(VOID) = { Em15_mv01_set, Em15_mv_move };    // left arm command 1 steps by sub-state
VOID (*Em15_mv02_tbl[2])(VOID) = { Em15_mv02_set, Em15_mv_move };    // left arm command 2 steps by sub-state
VOID (*Em15_mv03_tbl[2])(VOID) = { Em15_mv03_set, Em15_mv_move };    // left arm command 3 steps by sub-state
// left arm command 4 steps by sub-state
VOID (*Em15_mv04_tbl[3])(VOID) =
{
    Em15_mv04_set,    // 0 //
    Em15_mv04_key,   // 1 //
    Em15_mv04_end,   // 2 //
};
VOID (*Em15_mv05_tbl[2])(VOID) = { Em15_mv05_set, Em15_mv05_run };    // left arm command 5 steps by sub-state
// left arm command 5 Chris steps by Routine_2
VOID (*Em15_mv05_chris_tbl[3])(VOID) =
{
    Em15_mv05_slide,   // 0 //
    Em15_mv05_wait,    // 1 //
    Em15_mv05_play,          // 2 //
};
VOID (*Em15_mv05_jill_tbl[2])(VOID) = { Em15_mv05_jill, Em15_mv05_play };    // left arm command 5 Jill steps by Routine_2
// left arm command 6 steps by sub-state
VOID (*Em15_mv06_tbl[3])(VOID) =
{
    Em15_mv06_set,      // 0 //
    Em15_lever_move,   // 1 //
    Em15_lever_pull,   // 2 //
};
// left arm command 7 steps by sub-state
VOID (*Em15_mv07_tbl[4])(VOID) =
{
    Em15_mv07_set,   // 0 //
    Em15_mv07_drop,   // 1 //
    Em15_mv07_fall,   // 2 //
    Em15_mv07_loop,   // 3 //
};
// left arm command 8 steps by sub-state
VOID (*Em15_mv08_tbl[3])(VOID) =
{
    Em15_mv08_set,   // 0 //
    Em15_lever_move,    // 1 //
    Em15_lever_pull,    // 2 //
};
