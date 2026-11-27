/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\PL_SCE.C
 */

#include "TYPES.H"
#include "PL_SCE.H"
#include "EM_SUB.H"
#include "ENEMY.H"
#include "JOINT.H"
#include "PL_WEP.H"
#include "SCE1.H"
#include "SND.H"
#include "SCRATCHPAD.H"
#include "MEM_MAP.H"

// this unit passes the late flag as int and takes the angle result as int

VOID Pl_Scenario(VOID)
{
#ifdef __INTELLISENSE__
    extern INT (*Pl_sce_tbl[])();  // asm label alias unsupported by IntelliSense
#else
    extern INT (*Pl_sce_tbl[])() __asm__("Pl_sce");
#endif
    USHORT f;
    INT t;

    Pl_sce_tbl[G_PLAYER->Routine_2]();
    if (G.Player.Sce_flg & 2)
    {
        Pl_sce_tbl[G.Player.Routine_2]();
    }
    f = G.Player.Sce_flg;
    if (f & 4)
    {
        t = f & 8;
        Foot_set_pl(t != 0);
    }
}

VOID Pl_sce00(VOID)
{
    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 0;
        G.Player.Hokan_flg = 3;

    case 1:
        Joint_move(G.Player.Sce_flg & 1, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x400);
        break;
    case 2:
        Pl_weapon_ch(G.Player.Type, 0xE, WEP_ANIM_BUF, WEP_PACKET_BUF);
        G.Player.Routine_3 = 6;
        Joint_move(0, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x400);
        break;
    case 3:
        Pl_weapon_ch(G.Player.Type, 0xE, WEP_ANIM_BUF, WEP_PACKET_BUF);
        G.Player.Routine_3 = 6;
        Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
        break;
    case 4:
        Pl_weapon_ch(G.Player.Type, 0xE, WEP_ANIM_BUF, WEP_PACKET_BUF);
        G.Player.Routine_3 = 6;
        Joint_move(0, G.Player.pRoom_kan_t_ptr, G.Player.pRoom_seq_t_ptr, 0x400);
        break;
    case 5:
        Pl_weapon_ch(G.Player.Type, 0xE, WEP_ANIM_BUF, WEP_PACKET_BUF);
        G.Player.Routine_3 = 6;
        Joint_move(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400);
        break;
    case 6:
        break;
    }
}

VOID Pl_sce01(VOID)
{
    switch (G.Player.Routine_3)
    {
    case 0:
        MAIN.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 7;
        G.Player.Spd_cur = 0;
        if (G.Player.Sce_flg & 0x20)
        {
            G.Player.Hokan_flg = 0;
        }
        if ((UCHAR)G.Player.Move_no >= 0x3E)  // truncates to a byte
        {
            G.Player.Routine_3 = 4;
            PL_MOVE_NO_UBYTE -= 0x3E;
        }
        else if ((UCHAR)G.Player.Move_no >= 0x10)  // truncates to a byte
        {
            G.Player.Routine_3 = 3;
        }
        else
        {
            G.Player.Routine_3 = Aim_data.Anm_chain[(UCHAR)G.Player.Move_no][0] + 1;  // truncates to a byte
            G.Player.Move_no = Aim_data.Anm_chain[(UCHAR)G.Player.Move_no][1];  // truncates to a byte
        }
        G.Player.Timer0 = 1;
        break;
    case 1:
        if ((G.Player.Sce_flg & 0x80) && G.Player.Timer0-- == 0)
        {
            G.Player.Timer0 = 1;
            break;
        }
        if (Joint_move(G.Player.Sce_flg & 1, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x200))
        {
            G.Player.Routine_3 = 5;
            G.Player.Cdir_y += G.Player.Sce_free0;
        }
        break;
    case 2:
        if ((G.Player.Sce_flg & 0x80) && G.Player.Timer0-- == 0)
        {
            G.Player.Timer0 = 1;
            break;
        }
        if (Joint_move(G.Player.Sce_flg & 1, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x200))
        {
            G.Player.Routine_3 = 5;
            G.Player.Cdir_y += G.Player.Sce_free0;
            G.Player.Move_no += 5;
        }
        break;
    case 3:
        if ((G.Player.Sce_flg & 0x80) && G.Player.Timer0-- == 0)
        {
            G.Player.Timer0 = 1;
            break;
        }
        if (Joint_move(G.Player.Sce_flg & 1, G.Player.pRoom_kan_t_ptr, G.Player.pRoom_seq_t_ptr, 0x200))
        {
            G.Player.Routine_3 = 5;
            G.Player.Cdir_y += G.Player.Sce_free0;
        }
        break;
    case 4:
        if ((G.Player.Sce_flg & 0x80) && G.Player.Timer0-- == 0)
        {
            G.Player.Timer0 = 1;
            break;
        }
        if (Joint_move(G.Player.Sce_flg & 1, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x200))
        {
            G.Player.Routine_3 = 5;
            G.Player.Cdir_y += G.Player.Sce_free0;
            G.Player.Move_no += 0x3E;
        }
        break;
    case 5:
        Flg_on(&MAIN.Room_flg, G.Player.Set_flg_no);  // address passed as an integer
        G.Player.Sce_free0 = 0;
        if (G.Player.Sce_flg & 0x10)
        {
            G.Player.Routine_3 = 0;
        }
        break;
    }
}

VOID Pl_sce02(VOID)
{
    INT dx;
    INT dz;
    UCHAR act;
    VOID* unused;
    ENEMY_WORK* e;
    ENEMY_WORK* e2;
    ENEMY_WORK* e3;
    ENEMY_WORK* e4;

    e = EM_CUR;
    act = e->Routine_3;
    switch (act)
    {
    case 0:
        e->Move_cnt = 0U;
        MAIN.pEm->Move_wait = 0;
        MAIN.pEm->Move_no = 2;
        MAIN.pEm->Routine_3 = 1U;
        MAIN.pEm->Hokan_flg = 3;

    case 1:
        e2 = EM_CUR;
        ScratchPad->Vec.vx = e2->Root_x;
        ScratchPad->Vec.vz = e2->Root_z;
        ScratchPad->Vec.vy = 0;
        MAIN.pEm->Cdir_y += Goto00_ck(&ScratchPad->Vec, (SHORT)G.Player.Sce_free0);
        Joint_move(G.Player.Sce_flg & 1, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
        if (Goto00_ck(&ScratchPad->Vec, 0x16A) == 0)
        {
            MAIN.pEm->Routine_3 = 2U;
            return;
        }
        return;
    case 2:
        e->Move_cnt = 0U;
        MAIN.pEm->Move_wait = 0;
        MAIN.pEm->Move_no = 2;
        MAIN.pEm->Routine_3 = 3U;
        MAIN.pEm->Hokan_flg = 3;

    case 3:
        if (EM_CUR->Move_cnt == 8)
        {
            Snd_se_walk(0, 0);
        }
        if (MAIN.pEm->Move_cnt == 0x16)
        {
            Snd_se_walk(0, -4);
        }
        Walk_spd_set(0x5D);
        e3 = MAIN.pEm;
        ScratchPad->Vec.vx = e3->Root_x;
        ScratchPad->Vec.vz = e3->Root_z;
        ScratchPad->Vec.vy = 0;
        Goto00(&ScratchPad->Vec, (SHORT)G.Player.Sce_free0);
        Joint_move(G.Player.Sce_flg & 1, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
        Add_speedXZ(0);
        e4 = MAIN.pEm;
        dx = e4->Pos_x - e4->Root_x;
        dz = e4->Pos_z - e4->Root_z;
        if (SquareRoot0(dx * dx + dz * dz) < 0x96)
        {
            Flg_on(&MAIN.Room_flg, G.Player.Set_flg_no);  // address passed as an integer
            if (!(G.Player.Sca_info & 0x80))
            {
                MAIN_EM_ROUTINE_WORD = 1;
            }
        }
        break;
    }
}

VOID Pl_sce03(VOID)
{
    INT dx;
    INT dz;
    SHORT Loop_ctr;
    ENEMY_WORK* e1;
    ENEMY_WORK* e2;
    ENEMY_WORK* e3;
    ENEMY_WORK* e4;
    ENEMY_WORK* e5;
    ENEMY_WORK* e6;
    ENEMY_WORK* e7;

    switch (G.pEm->Routine_3)
    {
    case 0:
        EM_CUR->Move_cnt = 0;
        MAIN.pEm->Move_wait = 0;
        MAIN.pEm->Move_no = 2;
        MAIN.pEm->Routine_3 = 1;
        MAIN.pEm->Hokan_flg = 3;

    case 1:
        e1 = EM_CUR;
        ScratchPad->Vec.vx = e1->Root_x;
        ScratchPad->Vec.vz = e1->Root_z;
        ScratchPad->Vec.vy = 0;
        MAIN.pEm->Cdir_y += Goto00_ck(&ScratchPad->Vec, (SHORT)G.Player.Sce_free0);
        Joint_move(G.Player.Sce_flg & 1, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
        if (Goto00_ck(&ScratchPad->Vec, 0x16A) == 0)
        {
            MAIN.pEm->Routine_3 = 2;
        }
        break;
    case 2:
        e2 = EM_CUR;
        e2->Spd_cur = 0xD2;
        e2->Move_cnt = 0;
        MAIN.pEm->Move_wait = 0;
        MAIN.pEm->Move_no = 3;
        MAIN.pEm->Routine_3 = 3;
        MAIN.pEm->Hokan_flg = 3;

    case 3:
        if (EM_CUR->Move_cnt == 0)
        {
            Snd_se_walk(1, 0);
        }
        if (MAIN.pEm->Move_cnt == 0xA)
        {
            Snd_se_walk(1, -4);
        }
        e3 = MAIN.pEm;
        ScratchPad->Vec.vx = e3->Root_x;
        ScratchPad->Vec.vz = e3->Root_z;
        ScratchPad->Vec.vy = 0;
        Goto00(&ScratchPad->Vec, (SHORT)G.Player.Sce_free0);
        Joint_move(G.Player.Sce_flg & 1, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
        Add_speedXZ(0);
        e4 = MAIN.pEm;
        dx = e4->Pos_x - e4->Root_x;
        dz = e4->Pos_z - e4->Root_z;
        if (SquareRoot0(dx * dx + dz * dz) < 0xFA)
        {
            if (G.Player.Sca_info & 0x80)
            {
                Flg_on(&MAIN.Room_flg[0], G.Player.Set_flg_no);
                break;
            }
            MAIN.pEm->Routine_3 = 4;
        }
        break;
    case 4:
        EM_CUR->Move_cnt = 0;
        MAIN.pEm->Move_wait = 0;
        MAIN.pEm->Move_no = 0;
        MAIN.pEm->Routine_3 = 5;
        MAIN.pEm->Hokan_flg = 3;
        MAIN.pEm->Timer0 = 0;

    case 5:
        e5 = EM_CUR;
        Joint_move(G.Player.Sce_flg & 1, e5->pKan_t_ptr, e5->pSeq_t_ptr, 0x400);
        e6 = MAIN.pEm;
        Loop_ctr = e6->Timer0 + 1;
        e6->Timer0 = Loop_ctr;
        if (Loop_ctr >= 4)  // sign-extends the halfword
        {
            e6->Routine_3 = 6;
        }
        e7 = MAIN.pEm;
        e7->Spd_cur -= 0x1E;
        Add_speedXZ(0);
        break;
    case 6:
        EM_ROUTINE_WORD = 1;
        Flg_on(&MAIN.Room_flg[0], G.Player.Set_flg_no);
        break;
    }
}

VOID Pl_sce04(VOID)
{
    INT dx;
    INT dz;
    UCHAR act;
    UCHAR frame;
    UCHAR frame2;
    ENEMY_WORK* e;
    ENEMY_WORK* e2;
    ENEMY_WORK* e3;

    act = G.pEm->Routine_3;
    switch (act)
    {
    case 0:
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 3;
        G.pEm->Move_no = 3;

    case 1:
        frame = G.pEm->Move_cnt;
        if (frame == 8 || frame == 0x16)
        {
            Snd_se_walk(0, 0);
        }
        MAIN.Player.Spd_cur = 0x3C;
        frame2 = G.pEm->Move_cnt;
        if (frame2 >= 5U || frame2 < 8U)
        {
            G.Player.Spd_cur = 0x40;
        }
        e = EM_CUR;
        ScratchPad->Vec.vx = e->Root_x;
        ScratchPad->Vec.vz = e->Root_z;
        ScratchPad->Vec.vy = 0;
        e->Cdir_y = (e->Cdir_y + 0x800) & 0xFFF;
        Goto00(&ScratchPad->Vec, (SHORT)G.Player.Sce_free0);
        e2 = MAIN.pEm;
        e2->Cdir_y = e2->Cdir_y - 0x800;
        Joint_move(G.Player.Sce_flg & 1, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x400);
        Add_speedXZ(0x800);
        e3 = MAIN.pEm;
        dx = e3->Pos_x - e3->Root_x;
        dz = e3->Pos_z - e3->Root_z;
        if (SquareRoot0(dx * dx + dz * dz) < 0x64)
        {
            Flg_on(&MAIN.Room_flg, G.Player.Set_flg_no);  // address passed as an integer
            if (!(G.Player.Sca_info & 0x80))
            {
                MAIN_EM_ROUTINE_WORD = 1;
            }
        }
        return;
    }
}

VOID Pl_sce05(VOID)
{
    INT dx;
    INT dz;
    UCHAR act;
    UCHAR frame;
    ENEMY_WORK* e;
    ENEMY_WORK* e2;
    ENEMY_WORK* e3;

    act = G.pEm->Routine_3;
    switch (act)
    {
    case 0:
        G.Player.Routine_3 = 1;
        G.Player.Hokan_flg = 3;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.pEm->Move_no = 2;
        G.Player.Spd_cur = 0x1D;

    case 1:
        frame = G.pEm->Move_cnt;
        if ((frame == 7 || frame == 0x1B) && G.pEm->Move_wait == 2)
        {
            Snd_se_walk(0, -4);
        }
        e = EM_CUR;
        ScratchPad->Vec.vx = e->Root_x;
        ScratchPad->Vec.vz = e->Root_z;
        ScratchPad->Vec.vy = 0;
        e->Cdir_y = (e->Cdir_y + 0x800) & 0xFFF;
        Goto00(&ScratchPad->Vec, (SHORT)G.Player.Sce_free0);
        e2 = MAIN.pEm;
        e2->Cdir_y = e2->Cdir_y - 0x800;
        Joint_move(G.Player.Sce_flg & 1, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x400);
        Add_speedXZ(0x800);
        e3 = MAIN.pEm;
        dx = e3->Pos_x - e3->Root_x;
        dz = e3->Pos_z - e3->Root_z;
        if (SquareRoot0(dx * dx + dz * dz) < 0x64)
        {
            Flg_on(&MAIN.Room_flg, G.Player.Set_flg_no);  // address passed as an integer
            if (!(G.Player.Sca_info & 0x80))
            {
                MAIN_EM_ROUTINE_WORD = 1;
            }
        }
        return;
    }
}

VOID Pl_sce06(VOID)
{
    UCHAR act;
    ENEMY_WORK* e;
    ENEMY_WORK* e2;

    e = EM_CUR;
    act = e->Routine_3;
    switch (act)
    {
    case 0:
        e->Move_cnt = 0;
        MAIN.pEm->Move_wait = 0;
        MAIN.pEm->Move_no = 2;
        MAIN.pEm->Routine_3 = 1U;
        MAIN.pEm->Hokan_flg = 3;

    case 1:
        e2 = EM_CUR;
        ScratchPad->Vec.vx = e2->Root_x;
        ScratchPad->Vec.vz = e2->Root_z;
        ScratchPad->Vec.vy = 0;
        Goto00(&ScratchPad->Vec, 0x38);
        Joint_move(G.Player.Sce_flg & 1, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
        if (Goto00_ck(&ScratchPad->Vec, (SHORT)G.Player.Sce_free0) == 0)
        {
            MAIN.pEm->Routine_3 = 2U;
            return;
        }
        return;
    case 2:
        ROUTINE_WORD(e) = 1;  // INT* view of the field (a plain access changes the code)
        Flg_on(&MAIN.Room_flg, G.Player.Set_flg_no);  // address passed as an integer
        break;
    }
}

VOID Pl_sce07(VOID)
{
    UCHAR act;
    ENEMY_WORK* e;
    ENEMY_WORK* e2;

    e = EM_CUR;
    act = e->Routine_3;
    switch (act)
    {
    case 0:
        e->Move_cnt = 0;
        MAIN.pEm->Move_wait = 0;
        e2 = MAIN.pEm;
        e2->Move_no -= 5;
        MAIN.pEm->Routine_3 = 1U;
        MAIN.pEm->Hokan_flg = 3;

    case 1:
        G.pEm->Routine_3 += Joint_move(G.Player.Sce_flg & 1, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
        break;
    case 2:
        Flg_on(&MAIN.Room_flg, G.Player.Set_flg_no);  // address passed as an integer
        break;
    }
    MAIN.Player.Cdir_y += G.Player.Sce_free0;
}

VOID Pl_sce08(VOID)
{
    UCHAR act;
    ENEMY_WORK* e;
    ENEMY_WORK* e2;

    e = EM_CUR;
    act = e->Routine_3;
    switch (act)
    {
    case 0:
        e->Move_cnt = 0;
        MAIN.pEm->Move_wait = 0;
        e2 = MAIN.pEm;
        e2->Move_no -= 5;
        MAIN.pEm->Routine_3 = 1U;
        MAIN.pEm->Hokan_flg = 3;

    case 1:
        G.pEm->Routine_3 += Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
        return;
    case 2:
        Flg_on(&MAIN.Room_flg, G.Player.Set_flg_no);  // address passed as an integer
        return;
    }
}

VOID Pl_sce09(VOID)
{
    UCHAR act;
    ENEMY_WORK* e;
    ENEMY_WORK* e2;

    e = EM_CUR;
    act = e->Routine_3;
    switch (act)
    {
    case 0:
        e->Move_cnt = 0;
        MAIN.pEm->Move_wait = 0;
        e2 = MAIN.pEm;
        e2->Move_no -= 5;
        MAIN.pEm->Routine_3 = 1U;
        MAIN.pEm->Hokan_flg = 3;

    case 1:
        G.pEm->Routine_3 += Joint_move(1, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
        return;
    case 2:
        ROUTINE_WORD(e) = 1;  // INT* view of the field (a plain access changes the code)
        Flg_on(&MAIN.Room_flg, G.Player.Set_flg_no);  // address passed as an integer
        return;
    }
}

// player behaviour routines of the script-controlled state, by Routine_2
PL_SCE_FUNC Pl_sce[10] =
{
    Pl_sce00,   // 0 //
    Pl_sce01,   // 1 //
    Pl_sce02,   // 2 //
    Pl_sce03,   // 3 //
    Pl_sce04,   // 4 //
    Pl_sce05,   // 5 //
    Pl_sce06,   // 6 //
    Pl_sce07,   // 7 //
    Pl_sce08,   // 8 //
    Pl_sce09,   // 9 //
};

// EKG waveform segments (x start, x end, y start, slope), 4 bytes each, grouped into 20 waveforms
UCHAR Ecg_wave_tbl[0x384] =
{
    0x20,   // 0 //
    0x2F,   // 1 //
    0x00,   // 2 //
    0x00,   // 3 //
    0x1E,   // 4 //
    0x20,   // 5 //
    0xFC,   // 6 //
    0x02,   // 7 //
    0x1B,   // 8 //
    0x1E,   // 9 //
    0x02,   // 10 //
    0xFE,   // 11 //
    0x1A,   // 12 //
    0x1B,   // 13 //
    0x01,   // 14 //
    0x01,   // 15 //
    0x15,   // 16 //
    0x1A,   // 17 //
    0xF2,   // 18 //
    0x03,   // 19 //
    0x11,   // 20 //
    0x15,   // 21 //
    0x02,   // 22 //
    0xFC,   // 23 //
    0x10,   // 24 //
    0x11,   // 25 //
    0x00,   // 26 //
    0x02,   // 27 //
    0x00,   // 28 //
    0x10,   // 29 //
    0x00,   // 30 //
    0x00,   // 31 //
    0x1F,   // 32 //
    0x2F,   // 33 //
    0x00,   // 34 //
    0x00,   // 35 //
    0x1D,   // 36 //
    0x1F,   // 37 //
    0xFC,   // 38 //
    0x02,   // 39 //
    0x1B,   // 40 //
    0x1D,   // 41 //
    0x00,   // 42 //
    0xFE,   // 43 //
    0x1A,   // 44 //
    0x1B,   // 45 //
    0x03,   // 46 //
    0xFD,   // 47 //
    0x19,   // 48 //
    0x1A,   // 49 //
    0x02,   // 50 //
    0x01,   // 51 //
    0x18,   // 52 //
    0x19,   // 53 //
    0xFF,   // 54 //
    0x03,   // 55 //
    0x15,   // 56 //
    0x18,   // 57 //
    0xF3,   // 58 //
    0x04,   // 59 //
    0x13,   // 60 //
    0x15,   // 61 //
    0xFD,   // 62 //
    0xFB,   // 63 //
    0x11,   // 64 //
    0x13,   // 65 //
    0x03,   // 66 //
    0xFD,   // 67 //
    0x10,   // 68 //
    0x11,   // 69 //
    0x01,   // 70 //
    0x02,   // 71 //
    0x0F,   // 72 //
    0x10,   // 73 //
    0x00,   // 74 //
    0x01,   // 75 //
    0x00,   // 76 //
    0x0F,   // 77 //
    0x00,   // 78 //
    0x00,   // 79 //
    0x20,   // 80 //
    0x2F,   // 81 //
    0x00,   // 82 //
    0x00,   // 83 //
    0x1E,   // 84 //
    0x20,   // 85 //
    0xFA,   // 86 //
    0x03,   // 87 //
    0x1D,   // 88 //
    0x1E,   // 89 //
    0xFC,   // 90 //
    0xFE,   // 91 //
    0x1A,   // 92 //
    0x1D,   // 93 //
    0x02,   // 94 //
    0xFD,   // 95 //
    0x18,   // 96 //
    0x1A,   // 97 //
    0xFE,   // 98 //
    0x02,   // 99 //
    0x15,   // 100 //
    0x18,   // 101 //
    0xF2,   // 102 //
    0x04,   // 103 //
    0x11,   // 104 //
    0x15,   // 105 //
    0x02,   // 106 //
    0xFC,   // 107 //
    0x10,   // 108 //
    0x11,   // 109 //
    0x00,   // 110 //
    0x02,   // 111 //
    0x00,   // 112 //
    0x10,   // 113 //
    0x00,   // 114 //
    0x00,   // 115 //
    0x20,   // 116 //
    0x2F,   // 117 //
    0x00,   // 118 //
    0x00,   // 119 //
    0x1D,   // 120 //
    0x20,   // 121 //
    0xFA,   // 122 //
    0x02,   // 123 //
    0x1A,   // 124 //
    0x1D,   // 125 //
    0x03,   // 126 //
    0xFD,   // 127 //
    0x19,   // 128 //
    0x1A,   // 129 //
    0x02,   // 130 //
    0x01,   // 131 //
    0x15,   // 132 //
    0x19,   // 133 //
    0xF2,   // 134 //
    0x04,   // 135 //
    0x13,   // 136 //
    0x15,   // 137 //
    0xFC,   // 138 //
    0xFB,   // 139 //
    0x12,   // 140 //
    0x13,   // 141 //
    0x00,   // 142 //
    0xFC,   // 143 //
    0x11,   // 144 //
    0x12,   // 145 //
    0x03,   // 146 //
    0xFD,   // 147 //
    0x10,   // 148 //
    0x11,   // 149 //
    0x01,   // 150 //
    0x02,   // 151 //
    0x0F,   // 152 //
    0x10,   // 153 //
    0x00,   // 154 //
    0x01,   // 155 //
    0x00,   // 156 //
    0x0F,   // 157 //
    0x00,   // 158 //
    0x00,   // 159 //
    0x1F,   // 160 //
    0x2F,   // 161 //
    0x00,   // 162 //
    0x00,   // 163 //
    0x1D,   // 164 //
    0x1F,   // 165 //
    0xFE,   // 166 //
    0x02,   // 167 //
    0x1A,   // 168 //
    0x1D,   // 169 //
    0x02,   // 170 //
    0xFE,   // 171 //
    0x19,   // 172 //
    0x1A,   // 173 //
    0x01,   // 174 //
    0x01,   // 175 //
    0x18,   // 176 //
    0x19,   // 177 //
    0xFF,   // 178 //
    0x02,   // 179 //
    0x15,   // 180 //
    0x18,   // 181 //
    0xF6,   // 182 //
    0x03,   // 183 //
    0x11,   // 184 //
    0x15,   // 185 //
    0x02,   // 186 //
    0xFD,   // 187 //
    0x10,   // 188 //
    0x11,   // 189 //
    0x00,   // 190 //
    0x02,   // 191 //
    0x00,   // 192 //
    0x10,   // 193 //
    0x00,   // 194 //
    0x00,   // 195 //
    0x1F,   // 196 //
    0x2F,   // 197 //
    0x00,   // 198 //
    0x00,   // 199 //
    0x1E,   // 200 //
    0x1F,   // 201 //
    0xFF,   // 202 //
    0x01,   // 203 //
    0x1D,   // 204 //
    0x1E,   // 205 //
    0xFE,   // 206 //
    0x02,   // 207 //
    0x1A,   // 208 //
    0x1D,   // 209 //
    0x03,   // 210 //
    0xFE,   // 211 //
    0x19,   // 212 //
    0x1A,   // 213 //
    0x02,   // 214 //
    0x01,   // 215 //
    0x18,   // 216 //
    0x19,   // 217 //
    0x00,   // 218 //
    0x02,   // 219 //
    0x15,   // 220 //
    0x18,   // 221 //
    0xF7,   // 222 //
    0x03,   // 223 //
    0x12,   // 224 //
    0x15,   // 225 //
    0x03,   // 226 //
    0xFC,   // 227 //
    0x11,   // 228 //
    0x12,   // 229 //
    0x01,   // 230 //
    0x02,   // 231 //
    0x10,   // 232 //
    0x11,   // 233 //
    0x00,   // 234 //
    0x01,   // 235 //
    0x00,   // 236 //
    0x10,   // 237 //
    0x00,   // 238 //
    0x00,   // 239 //
    0x1F,   // 240 //
    0x2F,   // 241 //
    0x00,   // 242 //
    0x00,   // 243 //
    0x1E,   // 244 //
    0x1F,   // 245 //
    0xFE,   // 246 //
    0x02,   // 247 //
    0x1D,   // 248 //
    0x1E,   // 249 //
    0xFB,   // 250 //
    0x03,   // 251 //
    0x1C,   // 252 //
    0x1D,   // 253 //
    0xFE,   // 254 //
    0xFD,   // 255 //
    0x1A,   // 256 //
    0x1C,   // 257 //
    0x02,   // 258 //
    0xFE,   // 259 //
    0x17,   // 260 //
    0x1A,   // 261 //
    0xFC,   // 262 //
    0x02,   // 263 //
    0x15,   // 264 //
    0x17,   // 265 //
    0xF6,   // 266 //
    0x03,   // 267 //
    0x11,   // 268 //
    0x15,   // 269 //
    0x02,   // 270 //
    0xFD,   // 271 //
    0x10,   // 272 //
    0x11,   // 273 //
    0x00,   // 274 //
    0x02,   // 275 //
    0x00,   // 276 //
    0x10,   // 277 //
    0x00,   // 278 //
    0x00,   // 279 //
    0x1F,   // 280 //
    0x2F,   // 281 //
    0x00,   // 282 //
    0x00,   // 283 //
    0x1D,   // 284 //
    0x1F,   // 285 //
    0xFC,   // 286 //
    0x02,   // 287 //
    0x1C,   // 288 //
    0x1D,   // 289 //
    0xFF,   // 290 //
    0xFD,   // 291 //
    0x1A,   // 292 //
    0x1C,   // 293 //
    0x03,   // 294 //
    0xFE,   // 295 //
    0x19,   // 296 //
    0x1A,   // 297 //
    0x02,   // 298 //
    0x01,   // 299 //
    0x15,   // 300 //
    0x19,   // 301 //
    0xF6,   // 302 //
    0x03,   // 303 //
    0x14,   // 304 //
    0x15,   // 305 //
    0xFB,   // 306 //
    0xFB,   // 307 //
    0x12,   // 308 //
    0x14,   // 309 //
    0x03,   // 310 //
    0xFD,   // 311 //
    0x11,   // 312 //
    0x12,   // 313 //
    0x01,   // 314 //
    0x02,   // 315 //
    0x10,   // 316 //
    0x11,   // 317 //
    0x00,   // 318 //
    0x01,   // 319 //
    0x00,   // 320 //
    0x10,   // 321 //
    0x00,   // 322 //
    0x00,   // 323 //
    0x1D,   // 324 //
    0x2F,   // 325 //
    0x00,   // 326 //
    0x00,   // 327 //
    0x1C,   // 328 //
    0x1D,   // 329 //
    0xFF,   // 330 //
    0x01,   // 331 //
    0x1B,   // 332 //
    0x1C,   // 333 //
    0xFD,   // 334 //
    0x02,   // 335 //
    0x19,   // 336 //
    0x1B,   // 337 //
    0x01,   // 338 //
    0xFE,   // 339 //
    0x18,   // 340 //
    0x19,   // 341 //
    0xFF,   // 342 //
    0x02,   // 343 //
    0x16,   // 344 //
    0x18,   // 345 //
    0xF9,   // 346 //
    0x03,   // 347 //
    0x14,   // 348 //
    0x16,   // 349 //
    0xFF,   // 350 //
    0xFD,   // 351 //
    0x13,   // 352 //
    0x14,   // 353 //
    0x01,   // 354 //
    0xFE,   // 355 //
    0x12,   // 356 //
    0x13,   // 357 //
    0x00,   // 358 //
    0x01,   // 359 //
    0x00,   // 360 //
    0x12,   // 361 //
    0x00,   // 362 //
    0x00,   // 363 //
    0x1D,   // 364 //
    0x2F,   // 365 //
    0x00,   // 366 //
    0x00,   // 367 //
    0x1B,   // 368 //
    0x1D,   // 369 //
    0xFC,   // 370 //
    0x02,   // 371 //
    0x19,   // 372 //
    0x1B,   // 373 //
    0x02,   // 374 //
    0xFD,   // 375 //
    0x18,   // 376 //
    0x19,   // 377 //
    0x01,   // 378 //
    0x01,   // 379 //
    0x17,   // 380 //
    0x18,   // 381 //
    0xFF,   // 382 //
    0x02,   // 383 //
    0x15,   // 384 //
    0x17,   // 385 //
    0xF9,   // 386 //
    0x03,   // 387 //
    0x12,   // 388 //
    0x15,   // 389 //
    0x02,   // 390 //
    0xFD,   // 391 //
    0x11,   // 392 //
    0x12,   // 393 //
    0x00,   // 394 //
    0x02,   // 395 //
    0x00,   // 396 //
    0x11,   // 397 //
    0x00,   // 398 //
    0x00,   // 399 //
    0x1C,   // 400 //
    0x2F,   // 401 //
    0x00,   // 402 //
    0x00,   // 403 //
    0x1B,   // 404 //
    0x1C,   // 405 //
    0xFF,   // 406 //
    0x01,   // 407 //
    0x1A,   // 408 //
    0x1B,   // 409 //
    0xFD,   // 410 //
    0x02,   // 411 //
    0x18,   // 412 //
    0x1A,   // 413 //
    0x01,   // 414 //
    0xFE,   // 415 //
    0x17,   // 416 //
    0x18,   // 417 //
    0xFF,   // 418 //
    0x02,   // 419 //
    0x15,   // 420 //
    0x17,   // 421 //
    0xF9,   // 422 //
    0x03,   // 423 //
    0x13,   // 424 //
    0x15,   // 425 //
    0x01,   // 426 //
    0xFC,   // 427 //
    0x12,   // 428 //
    0x13,   // 429 //
    0x00,   // 430 //
    0x01,   // 431 //
    0x00,   // 432 //
    0x12,   // 433 //
    0x00,   // 434 //
    0x00,   // 435 //
    0x1D,   // 436 //
    0x2F,   // 437 //
    0x00,   // 438 //
    0x00,   // 439 //
    0x1C,   // 440 //
    0x1D,   // 441 //
    0xFF,   // 442 //
    0x01,   // 443 //
    0x1B,   // 444 //
    0x1C,   // 445 //
    0xFD,   // 446 //
    0x02,   // 447 //
    0x1A,   // 448 //
    0x1B,   // 449 //
    0x00,   // 450 //
    0xFD,   // 451 //
    0x19,   // 452 //
    0x1A,   // 453 //
    0x02,   // 454 //
    0xFE,   // 455 //
    0x18,   // 456 //
    0x19,   // 457 //
    0x01,   // 458 //
    0x01,   // 459 //
    0x17,   // 460 //
    0x18,   // 461 //
    0xFF,   // 462 //
    0x02,   // 463 //
    0x15,   // 464 //
    0x17,   // 465 //
    0xF9,   // 466 //
    0x03,   // 467 //
    0x14,   // 468 //
    0x15,   // 469 //
    0xFD,   // 470 //
    0xFC,   // 471 //
    0x13,   // 472 //
    0x14,   // 473 //
    0x00,   // 474 //
    0xFD,   // 475 //
    0x12,   // 476 //
    0x13,   // 477 //
    0x02,   // 478 //
    0xFE,   // 479 //
    0x11,   // 480 //
    0x12,   // 481 //
    0x00,   // 482 //
    0x02,   // 483 //
    0x00,   // 484 //
    0x11,   // 485 //
    0x00,   // 486 //
    0x00,   // 487 //
    0x1C,   // 488 //
    0x2F,   // 489 //
    0x00,   // 490 //
    0x00,   // 491 //
    0x1B,   // 492 //
    0x1C,   // 493 //
    0xFF,   // 494 //
    0x01,   // 495 //
    0x1A,   // 496 //
    0x1B,   // 497 //
    0xFD,   // 498 //
    0x02,   // 499 //
    0x18,   // 500 //
    0x1A,   // 501 //
    0x01,   // 502 //
    0xFE,   // 503 //
    0x17,   // 504 //
    0x18,   // 505 //
    0xFF,   // 506 //
    0x02,   // 507 //
    0x16,   // 508 //
    0x17,   // 509 //
    0xFC,   // 510 //
    0x03,   // 511 //
    0x15,   // 512 //
    0x16,   // 513 //
    0xFF,   // 514 //
    0xFD,   // 515 //
    0x14,   // 516 //
    0x15,   // 517 //
    0x01,   // 518 //
    0xFE,   // 519 //
    0x13,   // 520 //
    0x14,   // 521 //
    0x00,   // 522 //
    0x01,   // 523 //
    0x00,   // 524 //
    0x13,   // 525 //
    0x00,   // 526 //
    0x00,   // 527 //
    0x1C,   // 528 //
    0x2F,   // 529 //
    0x00,   // 530 //
    0x00,   // 531 //
    0x1B,   // 532 //
    0x1C,   // 533 //
    0xFE,   // 534 //
    0x02,   // 535 //
    0x19,   // 536 //
    0x1B,   // 537 //
    0x02,   // 538 //
    0xFE,   // 539 //
    0x18,   // 540 //
    0x19,   // 541 //
    0x01,   // 542 //
    0x01,   // 543 //
    0x17,   // 544 //
    0x18,   // 545 //
    0xFF,   // 546 //
    0x02,   // 547 //
    0x16,   // 548 //
    0x17,   // 549 //
    0xFC,   // 550 //
    0x03,   // 551 //
    0x14,   // 552 //
    0x16,   // 553 //
    0x02,   // 554 //
    0xFD,   // 555 //
    0x13,   // 556 //
    0x14,   // 557 //
    0x00,   // 558 //
    0x02,   // 559 //
    0x00,   // 560 //
    0x13,   // 561 //
    0x00,   // 562 //
    0x00,   // 563 //
    0x1C,   // 564 //
    0x2F,   // 565 //
    0x00,   // 566 //
    0x00,   // 567 //
    0x1B,   // 568 //
    0x1C,   // 569 //
    0xFF,   // 570 //
    0x01,   // 571 //
    0x1A,   // 572 //
    0x1B,   // 573 //
    0xFD,   // 574 //
    0x02,   // 575 //
    0x18,   // 576 //
    0x1A,   // 577 //
    0x01,   // 578 //
    0xFE,   // 579 //
    0x17,   // 580 //
    0x18,   // 581 //
    0x00,   // 582 //
    0x01,   // 583 //
    0x15,   // 584 //
    0x17,   // 585 //
    0xFC,   // 586 //
    0x02,   // 587 //
    0x14,   // 588 //
    0x15,   // 589 //
    0xFF,   // 590 //
    0xFD,   // 591 //
    0x13,   // 592 //
    0x14,   // 593 //
    0x01,   // 594 //
    0x02,   // 595 //
    0x12,   // 596 //
    0x13,   // 597 //
    0x00,   // 598 //
    0x01,   // 599 //
    0x00,   // 600 //
    0x12,   // 601 //
    0x00,   // 602 //
    0x00,   // 603 //
    0x1C,   // 604 //
    0x2F,   // 605 //
    0x00,   // 606 //
    0x00,   // 607 //
    0x1B,   // 608 //
    0x1C,   // 609 //
    0xFF,   // 610 //
    0x01,   // 611 //
    0x1A,   // 612 //
    0x1B,   // 613 //
    0xFD,   // 614 //
    0x02,   // 615 //
    0x19,   // 616 //
    0x1A,   // 617 //
    0x00,   // 618 //
    0xFD,   // 619 //
    0x18,   // 620 //
    0x19,   // 621 //
    0x02,   // 622 //
    0xFE,   // 623 //
    0x17,   // 624 //
    0x18,   // 625 //
    0x01,   // 626 //
    0x01,   // 627 //
    0x16,   // 628 //
    0x17,   // 629 //
    0xFF,   // 630 //
    0x02,   // 631 //
    0x15,   // 632 //
    0x16,   // 633 //
    0xFC,   // 634 //
    0x03,   // 635 //
    0x14,   // 636 //
    0x15,   // 637 //
    0x00,   // 638 //
    0xFC,   // 639 //
    0x13,   // 640 //
    0x14,   // 641 //
    0x02,   // 642 //
    0xFE,   // 643 //
    0x12,   // 644 //
    0x13,   // 645 //
    0x00,   // 646 //
    0x02,   // 647 //
    0x00,   // 648 //
    0x12,   // 649 //
    0x00,   // 650 //
    0x00,   // 651 //
    0x25,   // 652 //
    0x2F,   // 653 //
    0x00,   // 654 //
    0x00,   // 655 //
    0x24,   // 656 //
    0x25,   // 657 //
    0xFE,   // 658 //
    0x02,   // 659 //
    0x23,   // 660 //
    0x24,   // 661 //
    0xFB,   // 662 //
    0x03,   // 663 //
    0x21,   // 664 //
    0x23,   // 665 //
    0x03,   // 666 //
    0xFC,   // 667 //
    0x1E,   // 668 //
    0x21,   // 669 //
    0xF4,   // 670 //
    0x05,   // 671 //
    0x1B,   // 672 //
    0x1E,   // 673 //
    0x03,   // 674 //
    0xFB,   // 675 //
    0x1A,   // 676 //
    0x1B,   // 677 //
    0x00,   // 678 //
    0x03,   // 679 //
    0x18,   // 680 //
    0x1A,   // 681 //
    0x00,   // 682 //
    0x00,   // 683 //
    0x17,   // 684 //
    0x18,   // 685 //
    0xFF,   // 686 //
    0x01,   // 687 //
    0x15,   // 688 //
    0x17,   // 689 //
    0xF9,   // 690 //
    0x03,   // 691 //
    0x12,   // 692 //
    0x15,   // 693 //
    0x05,   // 694 //
    0xFC,   // 695 //
    0x0E,   // 696 //
    0x12,   // 697 //
    0xF1,   // 698 //
    0x05,   // 699 //
    0x0B,   // 700 //
    0x0E,   // 701 //
    0x03,   // 702 //
    0xFA,   // 703 //
    0x0A,   // 704 //
    0x0B,   // 705 //
    0x00,   // 706 //
    0x03,   // 707 //
    0x00,   // 708 //
    0x0A,   // 709 //
    0x00,   // 710 //
    0x00,   // 711 //
    0x25,   // 712 //
    0x2F,   // 713 //
    0x00,   // 714 //
    0x00,   // 715 //
    0x24,   // 716 //
    0x25,   // 717 //
    0xFF,   // 718 //
    0x01,   // 719 //
    0x22,   // 720 //
    0x24,   // 721 //
    0xF9,   // 722 //
    0x03,   // 723 //
    0x1F,   // 724 //
    0x22,   // 725 //
    0x05,   // 726 //
    0xFC,   // 727 //
    0x1B,   // 728 //
    0x1F,   // 729 //
    0xF1,   // 730 //
    0x05,   // 731 //
    0x18,   // 732 //
    0x1B,   // 733 //
    0x03,   // 734 //
    0xFA,   // 735 //
    0x17,   // 736 //
    0x18,   // 737 //
    0x00,   // 738 //
    0x03,   // 739 //
    0x15,   // 740 //
    0x17,   // 741 //
    0x00,   // 742 //
    0x00,   // 743 //
    0x14,   // 744 //
    0x15,   // 745 //
    0xFE,   // 746 //
    0x02,   // 747 //
    0x13,   // 748 //
    0x14,   // 749 //
    0xFB,   // 750 //
    0x03,   // 751 //
    0x11,   // 752 //
    0x13,   // 753 //
    0x03,   // 754 //
    0xFC,   // 755 //
    0x0E,   // 756 //
    0x11,   // 757 //
    0xF4,   // 758 //
    0x05,   // 759 //
    0x0B,   // 760 //
    0x0E,   // 761 //
    0x03,   // 762 //
    0xFB,   // 763 //
    0x0A,   // 764 //
    0x0B,   // 765 //
    0x00,   // 766 //
    0x03,   // 767 //
    0x00,   // 768 //
    0x0A,   // 769 //
    0x00,   // 770 //
    0x00,   // 771 //
    0x26,   // 772 //
    0x2F,   // 773 //
    0x00,   // 774 //
    0x00,   // 775 //
    0x24,   // 776 //
    0x26,   // 777 //
    0xFC,   // 778 //
    0x02,   // 779 //
    0x22,   // 780 //
    0x24,   // 781 //
    0x04,   // 782 //
    0xFC,   // 783 //
    0x1E,   // 784 //
    0x22,   // 785 //
    0xF4,   // 786 //
    0x04,   // 787 //
    0x1B,   // 788 //
    0x1E,   // 789 //
    0x03,   // 790 //
    0xFB,   // 791 //
    0x1A,   // 792 //
    0x1B,   // 793 //
    0x01,   // 794 //
    0x02,   // 795 //
    0x19,   // 796 //
    0x1A,   // 797 //
    0x00,   // 798 //
    0x01,   // 799 //
    0x16,   // 800 //
    0x19,   // 801 //
    0x00,   // 802 //
    0x00,   // 803 //
    0x14,   // 804 //
    0x16,   // 805 //
    0xFA,   // 806 //
    0x03,   // 807 //
    0x12,   // 808 //
    0x14,   // 809 //
    0x04,   // 810 //
    0xFB,   // 811 //
    0x11,   // 812 //
    0x12,   // 813 //
    0x00,   // 814 //
    0x04,   // 815 //
    0x0E,   // 816 //
    0x11,   // 817 //
    0xF1,   // 818 //
    0x05,   // 819 //
    0x0B,   // 820 //
    0x0E,   // 821 //
    0x03,   // 822 //
    0xFA,   // 823 //
    0x0A,   // 824 //
    0x0B,   // 825 //
    0x01,   // 826 //
    0x02,   // 827 //
    0x09,   // 828 //
    0x0A,   // 829 //
    0x00,   // 830 //
    0x01,   // 831 //
    0x00,   // 832 //
    0x09,   // 833 //
    0x00,   // 834 //
    0x00,   // 835 //
    0x26,   // 836 //
    0x2F,   // 837 //
    0x00,   // 838 //
    0x00,   // 839 //
    0x24,   // 840 //
    0x26,   // 841 //
    0xFA,   // 842 //
    0x03,   // 843 //
    0x22,   // 844 //
    0x24,   // 845 //
    0x04,   // 846 //
    0xFB,   // 847 //
    0x21,   // 848 //
    0x22,   // 849 //
    0x00,   // 850 //
    0x04,   // 851 //
    0x1E,   // 852 //
    0x21,   // 853 //
    0xF1,   // 854 //
    0x05,   // 855 //
    0x1B,   // 856 //
    0x1E,   // 857 //
    0x03,   // 858 //
    0xFA,   // 859 //
    0x1A,   // 860 //
    0x1B,   // 861 //
    0x01,   // 862 //
    0x02,   // 863 //
    0x19,   // 864 //
    0x1A,   // 865 //
    0x00,   // 866 //
    0x01,   // 867 //
    0x16,   // 868 //
    0x19,   // 869 //
    0x00,   // 870 //
    0x00,   // 871 //
    0x14,   // 872 //
    0x16,   // 873 //
    0xFC,   // 874 //
    0x02,   // 875 //
    0x12,   // 876 //
    0x14,   // 877 //
    0x04,   // 878 //
    0xFC,   // 879 //
    0x0E,   // 880 //
    0x12,   // 881 //
    0xF4,   // 882 //
    0x04,   // 883 //
    0x0B,   // 884 //
    0x0E,   // 885 //
    0x03,   // 886 //
    0xFB,   // 887 //
    0x0A,   // 888 //
    0x0B,   // 889 //
    0x01,   // 890 //
    0x02,   // 891 //
    0x09,   // 892 //
    0x0A,   // 893 //
    0x00,   // 894 //
    0x01,   // 895 //
    0x00,   // 896 //
    0x09,   // 897 //
    0x00,   // 898 //
    0x00,   // 899 //
};

// start of each waveform in Ecg_wave_tbl; entries 4 * status + variant, with the poison status last
ECG_SEG_REC* Ecg_line_tbl[20] =
{
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0x25C),   // 0 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0x234),   // 1 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0x210),   // 2 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0x1E8),   // 3 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0x1B4),   // 4 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0x190),   // 5 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0x16C),   // 6 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0x144),   // 7 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0x118),   // 8 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0xF0),    // 9 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0xC4),    // 10 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0xA0),    // 11 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0x74),    // 12 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0x50),    // 13 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0x20),    // 14 //
    (ECG_SEG_REC*)Ecg_wave_tbl,           // 15 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0x344),   // 16 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0x304),   // 17 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0x2C8),   // 18 //
    (ECG_SEG_REC*)(Ecg_wave_tbl + 0x28C),   // 19 //
};

// EKG line colour (RGB) by health status
UCHAR Ecg_rgb_tbl[4][3] =
{
    { 0xC0, 0x00, 0x00 },   // 0 //
    { 0xFF, 0x7F, 0x00 },   // 1 //
    { 0xD8, 0xD8, 0x00 },   // 2 //
    { 0x00, 0xFF, 0x00 },   // 3 //
};

// trace position thresholds of the caution label animation, ending with 0x50
UCHAR Ecg_caution_step_tbl[0x8] =
{
    0x0C,   // 0 //
    0x11,   // 1 //
    0x14,   // 2 //
    0x3C,   // 3 //
    0x3F,   // 4 //
    0x44,   // 5 //
    0x50,   // 6 //
    0x00,   // 7 //
};

// trace position thresholds of the danger label animation, ending with 0x50
UCHAR Ecg_danger_step_tbl[0xC] =
{
    0x03,   // 0 //
    0x07,   // 1 //
    0x21,   // 2 //
    0x25,   // 3 //
    0x2B,   // 4 //
    0x2F,   // 5 //
    0x49,   // 6 //
    0x4D,   // 7 //
    0x50,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
};

// caution label frame shown at each step, 0 = no label
UCHAR Ecg_caution_frm_tbl[0x8] =
{
    0x00,   // 0 //
    0x03,   // 1 //
    0x02,   // 2 //
    0x01,   // 3 //
    0x02,   // 4 //
    0x03,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
};

// danger label frame shown at each step, 0 = no label
UCHAR Ecg_danger_frm_tbl[0xC] =
{
    0x00,   // 0 //
    0x02,   // 1 //
    0x01,   // 2 //
    0x02,   // 3 //
    0x00,   // 4 //
    0x02,   // 5 //
    0x01,   // 6 //
    0x02,   // 7 //
    0x00,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
};
