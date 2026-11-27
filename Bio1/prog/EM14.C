/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM14.C
 */

#include "EM14.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Em14_Main(VOID)
{
    Em14_routine_0[G.pEm->Routine_0]();
    G.pEm->Coord_flg = 0;
    G.pEm->Zone_in_flg = (!(G.pEm->Type & 0x80)) ? FUNC(HIT_CK_POINT4)(&G.pEm->Pos_x, G.Cccut) : 0;  // Hit_ck_point4
}

VOID Em14_Init(VOID)
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
    G.pEm->Neck_point_z = G.pEm->Pos_z;
    EM14_EF(pos_x) = G.pEm->Pos_x << 0x10;
    EM14_EF(pos_z) = G.pEm->Pos_z << 0x10;
}

VOID Em14_Move(VOID)
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
        Em14_mv_tbl[(G.pEm->Type & 0xF)]();
    }
}

VOID Em14_mv00(VOID)
{
    if (G.pEm->Routine_1 == 0)
    {
        G.pEm->Routine_1++;
        G.pEm->Hokan_flg = G.pEm->Move_no != 0 ? 7 : 0;
        G.pEm->Move_no = 0U;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Timer0 = 8U;
        Em14_spd_set(G.pEm->Pos_x, G.pEm->Pos_z, G.pEm->Neck_point_x, G.pEm->Neck_point_z, G.pEm->Timer0);
        if ((__builtin_abs(G.pEm->Pos_x - G.pEm->Neck_point_x)) + (__builtin_abs(G.pEm->Pos_z - G.pEm->Neck_point_z)) < 0xC8)
        {
            G.pEm->Timer0 = 0U;
        }
    }
    if (G.pEm->Timer0 != 0)
    {
        Em14_pos_add();
        G.pEm->Timer0--;
    }
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);  // Joint_move
}

VOID Em14_mv01(VOID)
{
    Em14_mv01_tbl[G.pEm->Routine_1]();
}

VOID Em14_mv01_set(VOID)
{
    G.pEm->Routine_1++;
    Em14_reach_set(-0x28, 0x28, 1);
}

VOID Em14_mv_move(VOID)
{
    EM14_EF(pos_x) = EM14_EF(pos_x) + EM14_EF(vel_x);
    EM14_EF(pos_z) = EM14_EF(pos_z) + EM14_EF(vel_z);
    G.pEm->Pos_x = EM14_EF(pos_x) >> 0x10;
    G.pEm->Pos_z = EM14_EF(pos_z) >> 0x10;
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

VOID Em14_mv02(VOID)
{
    Em14_mv02_tbl[G.pEm->Routine_1]();
}

VOID Em14_mv02_set(VOID)
{
    G.pEm->Routine_1++;
    Em14_reach_set(0x28, 0, G.Player.Id & 1 ? 2 : 1);
}

VOID Em14_mv03(VOID)
{
    Em14_mv03_tbl[G.pEm->Routine_1]();
}

VOID Em14_mv03_set(VOID)
{
    G.pEm->Routine_1++;
    Em14_reach_set(-0x14, -0x28, G.Player.Id & 1 ? 3 : 2);
}

VOID Em14_mv04(VOID)
{
    Em14_mv04_tbl[G.pEm->Routine_1]();
}

VOID Em14_mv04_set(VOID)
{
    G.pEm->Routine_1++;
    G.pEm->Routine_2 = 0;
    G.pEm->Move_no = 4;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 7;
    if (!(G.Player.Id & 1))
    {
        Em14_spd_set(G.pEm->Pos_x, G.pEm->Pos_z, G.pEm->Pos_x, G.pEm->Neck_point_z + 0xC8, 0x12);
        FUNC(XA_CMD)(1, 0xB8);  // Xa_cmd
        G.System_flg |= SYS_XA_VOICE;
        return;
    }
    FUNC(XA_CMD)(1, 0xB9);  // Xa_cmd
    G.System_flg |= SYS_XA_VOICE;
}

VOID Em14_mv04_key(VOID)
{
    if (G.Player.Id & 1)
    {
        Em14_mv04_jill();
        return;
    }
    Em14_mv04_key_tbl[G.pEm->Routine_2]();
}

VOID Em14_mv04_press(VOID)
{
    UCHAR frame;

    frame = G.pEm->Move_cnt;
    if (frame < 0x12U)
    {
        Em14_pos_add();
    }
    if (frame != 0x15 && frame != 0x22)
    {
    }
    else
    {
        FUNC(SND_SE_PLAY)(2, 0x1A, 0);  // Snd_se_play
    }
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)  // Joint_move
    {
        G.pEm->Routine_2++;
        G.pEm->Timer0 = 4;
    }
}

VOID Em14_mv04_wait(VOID)
{
    if (!(G.System_flg & SYS_XA_VOICE))
    {
        G.pEm->Type = G.pEm->Type | 0x20;
        G.pEm->Routine_1 = 0;
    }
}

VOID Em14_mv04_jill(VOID)
{
    if (G.pEm->Routine_2 == 0)
    {
        if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)  // Joint_move
        {
            G.pEm->Routine_2++;
        }
        return;
    }
    if (!(G.System_flg & SYS_XA_VOICE))
    {
        G.pEm->Type = G.pEm->Type | 0x20;
        G.pEm->Routine_1 = 0;
    }
}

VOID Em14_mv05(VOID)
{
    if (G.Player.Id & 1)
    {
        return;
    }
    Em14_mv05_tbl[G.pEm->Routine_1]();
}

VOID Em14_mv05_set(VOID)
{
    G.pEm->Routine_1++;
    G.pEm->Timer0 = 4U;
    Em14_spd_set(G.pEm->Pos_x, G.pEm->Pos_z, G.pEm->Neck_point_x, G.pEm->Neck_point_z + 0x50, G.pEm->Timer0);
}

VOID Em14_mv05_wait(VOID)
{
    Em14_pos_add();
    if (--G.pEm->Timer0 == 0)
    {
        G.pEm->Routine_1++;
        G.pEm->Move_no = 5;
        G.pEm->Move_cnt = 0;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
    }
}

VOID Em14_mv05_back(VOID)
{
    if (!(G.Player.Id & 1) && G.pEm->Move_cnt == 0xF)
    {
        FUNC(SND_SE_PLAY)(2, 0x1C, 0);  // Snd_se_play
    }
    if (FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200) != 0)  // Joint_move
    {
        G.pEm->Routine_1++;
        G.pEm->Timer0 = 4U;
        Em14_spd_set(G.pEm->Pos_x, G.pEm->Pos_z, G.pEm->Neck_point_x, G.pEm->Neck_point_z, G.pEm->Timer0);
        G.pEm->Move_no = 0;
        G.pEm->Move_cnt = 0U;
        G.pEm->Move_wait = 0;
        G.pEm->Hokan_flg = 7;
    }
}

VOID Em14_mv05_end(VOID)
{
    Em14_pos_add();
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);  // Joint_move
    if (--G.pEm->Timer0 == 0)
    {
        G.pEm->Type = G.pEm->Type | 0x20;
        G.pEm->Routine_1 = 0;
    }
}

VOID Em14_mv06(VOID)
{
    Em14_mv06_tbl[G.pEm->Routine_1]();
}

VOID Em14_mv06_set(VOID)
{
    G.pEm->Routine_1++;
    Em14_reach_set(-0x14, 0xA0, G.Player.Id & 1 ? 3 : 2);
}

VOID Em14_mv07(VOID)
{
    Em14_mv07_tbl[G.pEm->Routine_1]();
}

VOID Em14_mv07_set(VOID)
{
    G.pEm->Routine_1++;
    G.pEm->Timer0 = 0x96;
    G.pEm->Hokan_flg = G.pEm->Move_no != 0 ? 7 : 0;
    G.pEm->Move_no = 0U;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
}

VOID Em14_mv07_wait(VOID)
{
    INT off_z;

    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x200);  // Joint_move
    if (--G.pEm->Timer0 != 0)
    {
        return;
    }
    G.pEm->Routine_1++;
    G.pEm->Move_no = 4;
    G.pEm->Move_cnt = (!(G.Player.Id & 1)) ? 0x28 : 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0x1F;
    G.pEm->Timer0 = 0x18U;
    off_z = G.Player.Id & 1 ? 0x8C : 0x46;
    Em14_spd_set(G.pEm->Pos_x, G.pEm->Pos_z, G.pEm->Neck_point_x - 0x64, G.pEm->Neck_point_z + off_z, G.pEm->Timer0);
}

VOID Em14_mv07_move(VOID)
{
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x80);  // Joint_move
    if (G.Player.Id & 1)
    {
        if (G.pEm->Move_cnt >= 6U)
        {
            G.pEm->Move_cnt = 5U;
        }
    }
    else
    {
        G.pEm->Move_cnt = 0x28U;
    }
    if (--G.pEm->Timer0 != 0)
    {
        Em14_pos_add();
        return;
    }
    G.pEm->Routine_1++;
}

VOID Em14_mv07_end(VOID)
{
}

VOID Em14_reach_set(INT Ofs_x, INT off_z, UCHAR Mot_no)
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
    EM14_EF(vel_x) = ((G.pEm->Neck_point_x + Ofs_x - G.pEm->Pos_x) << 0x10) / 15;
    EM14_EF(vel_z) = ((G.pEm->Neck_point_z + off_z - G.pEm->Pos_z) << 0x10) / 15;
    Em14_spd_set(G.pEm->Pos_x, G.pEm->Pos_z, G.pEm->Neck_point_x + Ofs_x, G.pEm->Neck_point_z + off_z, 0xF);
}

VOID Em14_spd_set(INT Src_x, INT Src_z, INT Dst_x, INT Dst_z, USHORT frames)
{
    EM14_EF(vel_x) = ((Dst_x - Src_x) << 0x10) / frames;
    EM14_EF(vel_z) = ((Dst_z - Src_z) << 0x10) / frames;
}

VOID Em14_pos_add(VOID)
{
    EM14_EF(pos_x) = EM14_EF(pos_x) + EM14_EF(vel_x);
    EM14_EF(pos_z) = EM14_EF(pos_z) + EM14_EF(vel_z);
    G.pEm->Pos_x = EM14_EF(pos_x) >> 0x10;
    G.pEm->Pos_z = EM14_EF(pos_z) >> 0x10;
}

VOID (*Em14_routine_0[2])(VOID) = { Em14_Init, Em14_Move };    // right arm state handlers by entity state
// right arm commands by Type & 0xF
VOID (*Em14_mv_tbl[8])(VOID) =
{
    Em14_mv00,        // 0 //
    Em14_mv01,     // 1 //
    Em14_mv02,     // 2 //
    Em14_mv03,     // 3 //
    Em14_mv04,       // 4 //
    Em14_mv05,        // 5 //
    Em14_mv06,   // 6 //
    Em14_mv07,        // 7 //
};
VOID (*Em14_mv01_tbl[2])(VOID) = { Em14_mv01_set, Em14_mv_move };    // right arm command 1 steps by sub-state
VOID (*Em14_mv02_tbl[2])(VOID) = { Em14_mv02_set, Em14_mv_move };    // right arm command 2 steps by sub-state
VOID (*Em14_mv03_tbl[2])(VOID) = { Em14_mv03_set, Em14_mv_move };    // right arm command 3 steps by sub-state
VOID (*Em14_mv04_tbl[2])(VOID) = { Em14_mv04_set, Em14_mv04_key };    // right arm command 4 steps by sub-state
VOID (*Em14_mv04_key_tbl[2])(VOID) = { Em14_mv04_press, Em14_mv04_wait };    // right arm Chris typing steps by Routine_2
// right arm command 5 steps by sub-state
VOID (*Em14_mv05_tbl[4])(VOID) =
{
    Em14_mv05_set,        // 0 //
    Em14_mv05_wait,   // 1 //
    Em14_mv05_back,    // 2 //
    Em14_mv05_end,      // 3 //
};
VOID (*Em14_mv06_tbl[2])(VOID) = { Em14_mv06_set, Em14_mv_move };    // right arm command 6 steps by sub-state
// right arm command 7 steps by sub-state
VOID (*Em14_mv07_tbl[4])(VOID) =
{
    Em14_mv07_set,        // 0 //
    Em14_mv07_wait,   // 1 //
    Em14_mv07_move,   // 2 //
    Em14_mv07_end,    // 3 //
};
