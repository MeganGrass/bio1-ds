/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM0E.C
 */

#include "EM0E.H"
#include "SCRATCHPAD.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Em0e_Main(VOID)
{
    if (G.Stop_flg & STOP_ENEMY_MOVE)
    {
        Em0e_routine_0[G.pEm->Routine_0]();
        FUNC(OBA_SET_OFS)(G.pEm);
        FUNC(OBA_CK_HIT)(&MAIN.Player.Be_flg, G.pEm);
        FUNC(OBA_CK_EM)();
    }
    if (G.pEm->Type != 0)
    {
        G.pEm->Pos_x = EM0E_EF(Base_pos_x);
        G.pEm->Pos_z = EM0E_EF(Base_pos_z);
    }
    G.pEm->Coord_flg = 0;
    G.pEm->Zone_in_flg = FUNC(HIT_CK_POINT4)(&G.pEm->Pos_x, G.Cccut);
    if (G.pEm->Zone_in_flg != 0)
    {
        FUNC(MUL_KAGE)(&G.pEm->Pos_x, &G.pEm->Kage_work, 0, G.pEm->Cdir_y);
    }
}

VOID Em0e_Init(VOID)
{
    EM_ROUTINE_WORD = ROUTINES(0, 0, 0, 1);
    G.pEm->Life = 1;
    G.pEm->pAt_tbl = &Em0e_sca_info;  // reinterprets the memory as AT_TBL
    G.pEm->Be_flg = G.pEm->Be_flg & EM_BE_STATE_MASK;
    G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_WALL;
    G.pEm->Move_no = 0;
    G.pEm->Move_cnt = 0;
    G.pEm->Move_wait = 0;
    G.pEm->Hokan_flg = 0;
    FUNC(INIT_PARTS_WORK)();
    G.pEm->Scale = 0x1000;
    G.pEm->Damage_no = 1;
    EM0E_EF(Base_pos_x) = G.pEm->Pos_x;
    EM0E_EF(Base_pos_z) = G.pEm->Pos_z;
    EM0E_EF(Dummy172) = 0;
    EM0E_EF(Sink_cnt) = 2;
    EM0E_EF(Snd_timer) = 0x28;
    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = -0x50;
    ScratchWord[7] = 0x404040;
    FUNC(KAGE_WORK_INIT)(&ScratchPad->Sv, &G.pEm->Kage_work, 0xC00, 0xC00);
}

VOID Em0e_Move(VOID)
{
    G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_ATTACK_MASK;
    if (G.pEm->Type != 0x80)
    {
        if (G.pEm->Type == 0)
        {
            Em0e_mv00();
        }
        else
        {
            Em0e_mv01();
        }
    }
    FUNC(JOINT_MOVE)(0, G.pEm->pKan_t_ptr, G.pEm->pSeq_t_ptr, 0x100);
}

VOID Em0e_mv00(VOID)
{
    INT unused[10];    // unused local: reserves stack space

    if (G.pEm->Routine_1 >= 4U)
    {
        return;
    }
    Em0e_phase_tbl[G.pEm->Routine_1]();
    G.pEm->Pos_y += EM0E_EF(Shake_spd) >> 8;
    EM0E_EF(Shake_spd) = EM0E_EF(Shake_spd) > 0 ? -EM0E_EF(Shake_pow) : EM0E_EF(Shake_pow);
    if (--EM0E_EF(Snd_timer) == 0)
    {
        FUNC(SND_SE_STAD)(2U, 0x19U, 0, &G.pEm->Pos_x);

        EM0E_EF(Snd_timer) = (USHORT)((Api_rand() & 7) + 0xC + (Api_rand() & 7));  // truncates to an unsigned halfword
    }
    if ((Api_rand() & 0x1F) == 1)
    {
        FUNC(SND_SE_STAD)(2U, 0x19U, 0, &G.pEm->Pos_x);
    }
}

VOID Em0e_phase_start(VOID)
{
    G.pEm->Routine_1++;
    EM0E_EF(Shake_spd) = 0;
    EM0E_EF(Shake_pow) = 0;
    G.pEm->Timer0 = 8;
    EM0E_EF(Fade_cnt) = 6;
    G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;

    G.pEm->Life = -1;

    FUNC(SND_SE_ENEM)(0x18, 0);
}

VOID Em0e_phase_rise(VOID)
{
    EM0E_EF(Shake_pow) = EM0E_EF(Shake_pow) + 0x200;
    if ((SHORT)EM0E_EF(Shake_pow) >= 0x1600)  // sign-extends the halfword
    {
        EM0E_EF(Shake_pow) = 0x1600U;
        G.pEm->Routine_1++;
    }
}

VOID Em0e_phase_shrink(VOID)
{
    if (G.pEm->Scale < 0x9C5U)
    {
        G.pEm->Routine_1++;
        G.pEm->Timer0 = 1U;
        EM0E_EF(Fade_cnt) = 1;
        return;
    }
    FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, -0xA, -0xA);
    G.pEm->Scale = G.pEm->Scale - 8;
    if (G.pEm->Timer0-- != 0)
    {
        return;
    }
    if (EM0E_EF(Fade_cnt) != 0)
    {
        FUNC(CLUT_FADE_SABUN_ADD)(0, -1, -2, 0, 0x100, G.pEm->Id);
        G.pEm->Timer0 = 8U;
        EM0E_EF(Fade_cnt)--;
    }
}

VOID Em0e_phase_sink(VOID)
{
    EM0E_EF(Shake_pow) = ((SHORT)EM0E_EF(Shake_pow)) - 0x70;  // sign-extends the halfword
    if ((++G.pEm->Timer0 & 3) == 0)
    {
        EM0E_EF(Sink_cnt) = EM0E_EF(Sink_cnt) + 1;
    }
    if ((SHORT)EM0E_EF(Shake_pow) <= 0)  // sign-extends the halfword
    {
        G.pEm->Routine_1++;
        FUNC(FLG_ON)(&MAIN.SaveData.Enemy_flg, G.pEm->Em_set_flg);  // address passed as an integer
        return;
    }
}

VOID Em0e_mv01(VOID)
{
    if (G.pEm->Routine_1 != 0)
    {
        return;
    }
    G.pEm->Routine_1++;
    G.pEm->Scale = 0x9C4;
    G.pEm->Be_flg = G.pEm->Be_flg | EM_BE_NO_HIT;

    G.pEm->Life = -1;

    FUNC(CLUT_FADE_SET)(0, -6, -0xC, 0, 0x100, G.pEm->Id);
    FUNC(KAGE_WORK_SIZE_ADD)(&G.pEm->Kage_work, -0x7D0, -0x7D0);
}

VOID Em0e_Damage(VOID)
{
    G.pEm->Routine_0 = 1;
    G.pEm->Routine_1 = 0;
    G.pEm->Damage_no = 0;
}

VOID Em0e_Nop3(VOID)
{
}

VOID Em0e_Dead(VOID)
{
    return;
}

VOID Em0e_Nop5(VOID)
{
}

AT_TBL Em0e_sca_info = { 0x8000, 0, 0, 0, 0x1770, 0x7D0 };  // flags 0x8000, no offset, half height 6000, radius 2000

// state handlers, indexed by the enemy state
VOID (*Em0e_routine_0[6])(VOID) =
{
    Em0e_Init,         // 0 //
    Em0e_Move,         // 1 //
    Em0e_Damage,    // 2 //
    Em0e_Nop3,   // 3 //
    Em0e_Dead,   // 4 //
    Em0e_Nop5,   // 5 //
};

// sub-state handlers of the idle movement, indexed by Key_rno
VOID (*Em0e_phase_tbl[4])(VOID) =
{
    Em0e_phase_start,    // 0 //
    Em0e_phase_rise,     // 1 //
    Em0e_phase_shrink,   // 2 //
    Em0e_phase_sink,     // 3 //
};
