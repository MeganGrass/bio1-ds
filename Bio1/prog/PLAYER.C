/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\PLAYER.C
 */

#include "TYPES.H"
#include "PLAYER.H"
#include "CUT.H"
#include "EM_SUB.H"
#include "ENEMY.H"
#include "JOINT.H"
#include "MAG.H"
#include "MAIN.H"
#include "OBA.H"
#include "OMA.H"
#include "PAD.H"
#include "PL_SCE.H"
#include "PL_WEP.H"
#include "SCE_AT.H"
#include "SND.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "SCA.H"
#include "ST_SUB03.H"

USHORT Pl_turn_walk; // turn speed per frame while walking or backing, set by Pad_set
SHORT Pl_turn_run;  // turn speed per frame while running, set by Pad_set

VOID Player_move(VOID)
{
#ifdef __INTELLISENSE__
    extern INT (*Pl_routine_0_view[])();  // IntelliSense does not know the __asm__ label
#else
    extern INT (*Pl_routine_0_view[])() __asm__("Pl_routine");
#endif
    INT unused[8];
    UCHAR Zone_flg;

    G.pEm = PL_EM;

    if (G.Stop_flg & STOP_PLAYER)
    {
        Pl_routine_0_view[G.Player.Routine_0]();
        Rot_neck();
    }
    Oba_set_ofs(PL_EM);
    if (G.Player.Routine_0 != 5 && G.Player.Routine_2 != 0x11)
    {
        Oba_ck_em();
        Sca_ck_hit(&PL_POS_VEC, G.Player.pAt_tbl->At_w);
    }
    G.Player.Coord_flg = 0;
    G.Player.Zone_in_flg &= 0xFE;
    Zone_flg = G.Player.Zone_in_flg | Hit_ck_point4(&PL_POS_VEC, G.Cccut);
    G.Player.Zone_in_flg = Zone_flg;
    if ((Zone_flg & 0x7F) && !(G.Player.Sce_flg & 0x40))
    {
        MulKage(&PL_POS_VEC, &MAIN.Player.Kage_work, G.Player.Ground, G.Player.Cdir_y);
    }
    if ((G.System_flg & SYS_MIRROR_MATRIX) && Mirror_in_view(&G.pRoom->Cut[G.SaveData.Cut_no].View, (G.System_flg & SYS_MIRROR_SIDE) >> 1, &PL_POS_VEC) != 0)  // UCHAR* view of the field (a plain access changes the code)
    {
        Mirror_trans();
    }
    Mag_down();
}

VOID Pl_init(VOID)
{
    G.SaveData.Key = 0;
    VIEW(INT, G.Player.Routine_0) = 1;  // INT* view of the field (a plain access changes the code)
    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vx = 0;
    ScratchWord[7] = 0x808080;
    Kage_work_init(&ScratchPad->Sv, &MAIN.Player.Kage_work, 0x1F4, 0x2BC);
    EM_CUR->Hokan_flg = 0;
    MAIN.pEm->Move_no = 0;
    MAIN.pEm->Move_cnt = 0;
    MAIN.pEm->Move_wait = 0;
    Joint_move(0, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x400);
    G.Player.pSca_ofs->Off_x = 0;
    G.Player.pSca_ofs->Off_z = 0;
    G.Player.pSca_ofs->Off_y = G.Player.pAt_tbl->Off_y;
    G.Player.Zone_in_flg = 0;
    G.Player.Damage_no = 0;
    G.Player.Turn_timer = 0;
    G.Player.Old_x = G.Player.Pos_x;
    G.Player.Old_y = G.Player.Pos_y;
    G.Player.Old_z = G.Player.Pos_z;
    G.Player.Sca_info |= 0x10;
    Mag_anm_clr();
}

VOID Player_set(VOID)
{
    G.pEm = PL_EM;
    Pl_work_init();
    G.Vram_cur.b.Cclut = 7;
    G.Vram_cur.b.Ctpage = 0x16;
    Pl_model_set();
    GsInitCoordinate2(0, (GsCOORDINATE2*)&MAIN.Player.Coord_flg);  // the embedded GsCOORDINATE2 starts at Coord_flg
    Wep_joint_set(0xE);

    G.Player.Type = 0;

    if (G.SaveData.Equip_no != 0)
    {
        G.Player.Type = *(G.SaveData.Equip_no * 2 + &MAIN.SaveData.Item_boxwork[47].Id);
    }
    Pl_weapon_ch(G.Player.Type, 0xE, WEP_ANIM_BUF, WEP_PACKET_BUF);
    G.Player.Pos_y = 0;
    G.Player.Ground = 0;
    G.Player.Pos_x = G.Player.Old_x;
    G.Player.Pos_z = PL_OLD_Z_SHORT;
    {
        PARTS_W* pSin_part = G.Player.pSin_parts_ptr + 1;  // the second joint; a plain G.Player.pSin_parts_ptr[1] reloads the pointer for every store
        pSin_part->Free3 = 0;
        pSin_part->Free4 = 0;
        pSin_part->Free5 = 0x10;
    }
    G.Player.pAt_tbl = Pl_sca_ptr[(G.Player.Id & 1) * 2];  // the records of the two players are the first and the third

    Mag_anm_clr(PL_OLD_Z_SHORT);
}

VOID Pl_work_init(VOID)
{
    G.Player.Timer2 = 0;
    G.Player.Move_no = 0;
    G.Player.Move_cnt = 0;
    G.Player.Move_wait = 0;
    G.Player.Dummy0c0 = 0;
    G.Player.Be_flg = EM_BE_ENABLED;
    G.Player.Zone_in_flg = 1;
    G.Player.pCoord2 = (GsCOORDINATE2*)&MAIN.Player.Coord_flg;
    PL_ROUTINE_WORD = 0;
    G.Player.Dummy0c1 = 0;
    G.Player.Spd_cur = 0;
    G.Player.Damage_no = 0;
    G.Player.Spd_x = 0;
    G.Player.Spd_y = 0;
    G.Player.Spd_z = 0;
    G.Player.Cdir_x = 0;
    G.Player.Cdir_z = 0;
    PL_M_HALF(0) = 0x1000;
    PL_M_HALF(2) = 0;
    PL_M_HALF(4) = 0;
    PL_M_HALF(6) = 0;
    PL_M_HALF(8) = 0x1000;
    PL_M_HALF(10) = 0;
    PL_M_HALF(12) = 0;
    PL_M_HALF(14) = 0;
    PL_M_HALF(16) = 0x1000;
    G.Player.Scale = 0;
    G.Player.Neck_flg = 0;
}

VOID Pl_move(VOID)
{
#ifdef __INTELLISENSE__
    extern INT (*Pl_mv_view[])();  // IntelliSense does not know the __asm__ label
#else
    extern INT (*Pl_mv_view[])() __asm__("Pl_mv");
#endif

    SHORT health;
    INT Lethal;
    UCHAR timer;

    if (G_PLAYER->Life < 0)
    {
        if (G.Player.Timer0 == 0x7FFF)
        {
            G.Player.Cdir_y += 0x800;
        }
        PL_ROUTINE_WORD = 3;
        G.Player.Routine_2 = 0xC8;
        G.Player.Damage_no = 1;
        if (G.Status_flg & STAT_WATER)
        {
            G.Player.Life = 1;
        }
    }
    else
    {
        if (G.Player.Damage_no & 0x3F)
        {
            PL_ROUTINE_HALF0 = 2;
            G.Player.Routine_3 = 0;
            return;
        }
        if (G.Player.Sca_info & 0x62)
        {
            timer = G.Player.Poison_timer;
            G.Player.Poison_timer = timer + 0xFF;
            if (timer == 0)
            {
                G.Player.Poison_timer = 0x78;
                Lethal = G.Player.Sca_info & 0x40;
                if (Lethal != 0)
                {
                    G.Player.Poison_timer = 7;
                }
                health = G.Player.Life - 2;
                G.Player.Life = health;
                if (health < 0 && Lethal == 0)
                {
                    G.Player.Life = 1;
                }
            }
        }
        if (G_PLAYER->Turn_timer != 0)
        {
            G.Player.Turn_timer -= 1;
        }
        Pl_mv_view[G.Player.Routine_1]();
    }
}

VOID Pl_damage(VOID)
{
    if (G.Player.Routine_1 == 0)
    {
        Pl_hit_react_common();
    }
}

VOID Pl_die(VOID)
{
    pl_die00();
}

VOID Pl_dead(VOID)
{
    G.Stop_flg = 0;
}

VOID Pl_Em_damage(VOID)
{
    if (G.Player.Routine_3 == 0)
    {
        if ((G.System_flg & SYS_PUSH_OBJ) || (MAIN.Player.Sca_info & 0x80))
        {
            STOP_FLG_CUR |= STOP_ACTION;
        }
        MAIN.Player.Sca_info &= 0x7F;
    }
    (&MAIN.Pl_move[0])[G.Player.Routine_1]();  // reinterprets the memory as PL_FUNC
}

VOID Pl_Em_die(VOID)
{
    if (G.Player.Routine_3 == 0)
    {
        if ((G.System_flg & SYS_PUSH_OBJ) || (MAIN.Player.Sca_info & 0x80))
        {
            STOP_FLG_CUR |= STOP_ACTION;
        }
        MAIN.Player.Sca_info &= 0x7F;
    }
    MAIN.Pl_move[19 + G.Player.Routine_1]();
}

VOID Pl_state_anim_window2(VOID)
{
    MAIN.Pl_move[38 + G.Player.Routine_1]();
}

VOID Pl_mv_sub0(VOID)
{
    pl_mv00_pad();
    Pl_mv_sub2();
}

VOID Pl_mv_sub2(VOID)
{
    switch (G.Player.Routine_2)
    {
    case 0:
        pl_mv00();
        break;
    case 2:
        G_PLAYER->Cdir_y = (G.Player.Cdir_y + Pl_turn_walk) & 0xFFF;
        pl_mv01();
        break;
    case 3:
        G_PLAYER->Cdir_y = (G.Player.Cdir_y - Pl_turn_walk) & 0xFFF;

    case 1:
        pl_mv01();
        break;
    case 4:
        G_PLAYER->Cdir_y = (G.Player.Cdir_y + Pad_stand_turn_spd) & 0xFFF;
        pl_mv04();
        break;
    case 5:
        G_PLAYER->Cdir_y = (G.Player.Cdir_y - Pad_stand_turn_spd) & 0xFFF;

        pl_mv04();
        break;
    case 6:
        G_PLAYER->Cdir_y = (G.Player.Cdir_y + Pl_turn_walk) & 0xFFF;
        pl_mv03();
        break;
    case 7:
        G_PLAYER->Cdir_y = (G.Player.Cdir_y - Pl_turn_walk) & 0xFFF;

    case 8:
        pl_mv03();
        break;
    case 9:
        Pl_nop09();
        break;
    case 11:
        pl_mv07();
        break;

    case 12:
        pl_mv06();
        break;
    case 14:
        ScratchPad->Sv.vz = 0;
        ScratchPad->Sv.vy = 0;
        G_PLAYER->Cdir_y = (G.Player.Cdir_y + Pl_turn_run) & 0xFFF;
        ScratchPad->Sv.vx = G.Player.Spd_cur;
        RotMatrix((SVECTOR*)&MAIN.Player.Cdir_x, &ScratchPad->Mat);  // reinterprets the memory as SVECTOR
        ApplyMatrixSV(&ScratchPad->Mat, &ScratchPad->Sv, (SVECTOR*)&MAIN.Player.Spd_x);  // reinterprets the memory as SVECTOR
        pl_mv02();
        break;
    case 15:
        ScratchPad->Sv.vz = 0;
        ScratchPad->Sv.vy = 0;

        G_PLAYER->Cdir_y = (G.Player.Cdir_y - Pl_turn_run) & 0xFFF;

        ScratchPad->Sv.vx = G.Player.Spd_cur;
        RotMatrix((SVECTOR*)&MAIN.Player.Cdir_x, &ScratchPad->Mat);  // reinterprets the memory as SVECTOR
        ApplyMatrixSV(&ScratchPad->Mat, &ScratchPad->Sv, (SVECTOR*)&MAIN.Player.Spd_x);  // reinterprets the memory as SVECTOR
    case 13:
        pl_mv02();
        break;
    case 16:
        pl_mv0a();
        break;

    case 10:
    case 17:
        pl_mv08();
        break;
    }
}

VOID Pl_mv_sub1(VOID)
{
#ifdef __INTELLISENSE__
    extern INT (*Pl_act_tbl_view[])();  // IntelliSense does not know the __asm__ label
#else
    extern INT (*Pl_act_tbl_view[])() __asm__("Pl_act_tbl");
#endif

    Pl_act_tbl_view[G.Player.Routine_2]();
}

VOID pl_mv05(VOID)
{
    switch (G.Player.Routine_2)
    {
    case 18:
        Pl_gun_aim();
        break;
    case 19:
        Pl_gun_raise();
        Pl_gun_hold();
        break;
    case 20:
        Pl_gun_fire();
        break;
    case 21:
        ScratchWord[7] = 0;
        Pl_gun_aim_chg();
        break;
    case 22:
        ScratchWord[7] = 1;
        Pl_gun_aim_chg();
        break;
    case 23:
        Pl_gun_holster();
        break;
    case 24:
        Pl_gun_reload();
        break;
    case 25:
        Pl_gun_click();

        break;
    case 26:
        Pl_gun_lock();

        break;
    }
    Foot_set_pl(0);
    if (G.Player.Type == 6)
    {
        Hand_ck();
    }
}

VOID Pl_mv_sub4(VOID)
{
#ifdef __INTELLISENSE__
    extern INT (*Pl_act_tbl_view[])();  // IntelliSense does not know the __asm__ label
#else
    extern INT (*Pl_act_tbl_view[])() __asm__("Pl_act_tbl");
#endif
    Pl_act_tbl_view[G.Player.Routine_2]();
}

VOID Pl_hit_react_common(VOID)
{
    if (G.Player.Routine_3 == 0)
    {
        if ((G.System_flg & SYS_PUSH_OBJ) || (MAIN.Player.Sca_info & 0x80))
        {
            STOP_FLG_CUR |= STOP_ACTION;
        }
        MAIN.Player.Sca_info &= 0x7F;
    }
    switch (G.Player.Routine_2)
    {
    case 0x64:
    case 0x65:
        pl_dm00();
        return;
    case 0x66:
        pl_dm01();
        if (G.Player.Move_cnt >= 4U)
        {
            G.Player.Spd_cur = 0x1E;
        }
        Add_speedXZ(0x800);
        return;
    case 0x67:
        pl_dm01();
        if (G.Player.Move_cnt >= 4U)
        {
            G.Player.Spd_cur = 0x1E;
        }
        Add_speedXZ(0);
        return;
    case 0x68:
        pl_dm02();

    default:
        return;
    }
}

VOID pl_mv00_pad(VOID)
{
    INT t;

    t = G.SaveData.Key & (0x40 | 0x80);
    if ((t == 0x80 || t == (0x40 | 0x80)) && (G.SaveData.Key_trg & KEY_INSPECT))
    {
        if (Oma_pl_updown_ck() != 0)
        {
            G.Player.Damage_no = 0x80;

            G.Player.Routine_1 = 1;
            PL_ROUTINE_HALF2 = 10;
            STOP_FLG_CUR &= ~STOP_ACTION;

            MAIN.System_flg |= SYS_CLIMB;
            return;
        }
        if (Check_action_object() != 0)
        {
            G.Player.Routine_1 = 1;
            PL_ROUTINE_HALF2 = 12;
            STOP_FLG_CUR &= ~STOP_ACTION;
            G.Player.Sca_info |= 0x80;
            return;
        }

        if (G.Player.Zone_in_flg & 0x20)
        {
            STOP_FLG_CUR &= ~STOP_ACTION;

            G.Player.Damage_no = 0x80;
            G.Player.Routine_1 = 1;
            MAIN_PL_ROUTINE_HALF2 = 0x11;

            if (G.System_flg & SYS_LADDER)
            {
                MAIN_PL_ROUTINE_HALF2 = 0xB;
            }
            return;
        }
    }
    if (G.System_flg & SYS_CLIMB)
    {
        Pl_pad_turn();

        return;
    }
    if (G.System_flg & SYS_PUSH_OBJ)
    {
        G.Player.Routine_1 = 1;
        PL_ROUTINE_HALF2 = 0x10;
        STOP_FLG_CUR &= ~STOP_ACTION;
        return;
    }
    if (G.Player.Zone_in_flg & 0x20)
    {
        STOP_FLG_CUR &= ~STOP_ACTION;

        G.Player.Damage_no = 0x80;
        G.Player.Routine_1 = 1;
        MAIN_PL_ROUTINE_HALF2 = 0x11;
        if (G.System_flg & SYS_LADDER)
        {
            MAIN_PL_ROUTINE_HALF2 = 0xB;
        }

        return;
    }
    if ((G.SaveData.Key & KEY_AIM) && G.Player.Type != 0 && G.Player.Type < 11)
    {
        G.Player.Routine_1 = 3;
        if (G.Player.Type == 1)
        {
            G.Player.Routine_1 = 4;
        }
        PL_ROUTINE_HALF2 = 0x12;
        return;
    }
    switch (G.SaveData.Key & (KEY_UP | KEY_RIGHT | KEY_DOWN | KEY_LEFT))
    {
    case 1:
        G.Player.Routine_2 = 1;
        if (!(G.Key_old & 0x1))
        {
            G.Player.Routine_3 = 0;
        }
        break;
    case 3:
        G.Player.Routine_2 = 2;
        if (!(G.Key_old & 0x1))
        {
            G.Player.Routine_3 = 0;
        }
        break;
    case 9:
        G.Player.Routine_2 = 3;
        if (!(G.Key_old & 0x1))
        {
            G.Player.Routine_3 = 0;
        }
        break;
    case 2:
        if (MAIN.Player.Routine_2 != 4)
        {
            G.Player.Routine_3 = 0;
        }
        G.Player.Routine_2 = 4;
        break;
    case 8:
        if (MAIN.Player.Routine_2 != 5)
        {
            G.Player.Routine_3 = 0;
        }
        G.Player.Routine_2 = 5;
        break;
    case 6:
        G.Player.Routine_2 = 6;
        if (!(G.Key_old & 0x4))
        {
            G.Player.Routine_3 = 0;
        }
        break;
    case 12:
        G.Player.Routine_2 = 7;
        if (!(G.Key_old & 0x4))
        {
            G.Player.Routine_3 = 0;
        }
        break;
    case 4:
        G.Player.Routine_2 = 8;
        if (!(G.Key_old & 0x4))
        {
            G.Player.Routine_3 = 0;
        }
        break;
    case 5:
    case 7:
    case 10:
    case 11:
        break;
    }
}

VOID pl_mv00(VOID)
{
    switch (MAIN.Player.Routine_3)
    {
    case 0:
        G.Player.Routine_3 = 1;

        G.Player.Hokan_flg = 3;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 0;
        G.Player.Timer0 = 0x64;
        G.Player.Damage_no = 0;
        G.Player.Spd_cur = 0;

    case 1:
        Joint_move(0, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x400);
        if ((--G.Player.Timer0 << 16) == 0 && (G.Stop_flg & STOP_PAD_LOCK))
        {
            G.Player.Move_cnt = 0;
            G.Player.Move_wait = 0;
            G.Player.Move_no = 0;
            G.Player.Hokan_flg = 3;
            G.Player.Routine_3 += 1;
        }
        break;
    case 2:
        if (Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400) != 0)
        {
            G.Player.Move_no = 1;
            G.Player.Move_cnt = 0;
            G.Player.Move_wait = 0;
            G.Player.Hokan_flg = 3;
            G.Player.Routine_3 += 1;
        }
        break;
    case 3:
        Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
        break;
    }
}

VOID pl_mv01(VOID)
{
    UCHAR tbl[2][4] = { { 21, 23, 13, 14 }, { 20, 22, 15, 15 } };
    UCHAR* t;
    UCHAR* q;
    INT v;
    ENEMY_WORK* pEm;

    if (!(G.SaveData.Key & KEY_UP))
    {
        G.Player.Routine_3 = 2;
    }
    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Move_no = 2;
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 3;
        G.Player.Timer0 = 0;
        if (G.Status_flg & STAT_WATER)
        {
            Snd_se_walk(0, 0);
        }
    case 1:
        if (G.SaveData.Key & KEY_RUN)
        {
            G.Player.Routine_1 = 1;
            PL_ROUTINE_HALF2 = 0xD;
        }
        if (G.Player.Timer0 == 0)
        {
            if (G.pEm->Move_cnt == 8)
            {
                Snd_se_walk(0, 0);

                if (G.Status_flg & STAT_WATER)
                {
                    Pad_vib_fade_set(0x14, 0, 0x46, 0);
                    Pad_vib_fade_set(0xA, 0x46, 0, 0x14);
                }
            }

            if (G.pEm->Move_cnt == 0x16)
            {
                Snd_se_walk(0, -4);

                if (G.Status_flg & STAT_WATER)
                {
                    Pad_vib_fade_set(0x14, 0, 0x46, 0);
                    Pad_vib_fade_set(0xA, 0x46, 0, 0x14);
                }
            }
        }
        G_PLAYER->Spd_cur = 0x5D;
        pEm = G.pEm;
        t = tbl[G.Player.Id & 1];
        if ((UCHAR)(v = pEm->Move_cnt - t[0]) < 7)  // truncates to a byte
        {
            G.Player.Spd_cur -= t[2];
        }
        if (pEm->Move_cnt >= 7 && pEm->Move_cnt < 14)
        {
            G.Player.Spd_cur -= t[2];
            q = t;
        }
        else
        {
            q = t;
        }
        if ((UCHAR)(v = pEm->Move_cnt - q[1]) < 3)  // truncates to a byte
        {
            MAIN.Player.Spd_cur -= q[3];
        }
        if (pEm->Move_cnt >= 9 && pEm->Move_cnt < 12)
        {
            MAIN.Player.Spd_cur -= t[3];
        }
        if (G.Status_flg & STAT_WATER)
        {
            SHORT Time_cnt;
            SHORT speed = G.Player.Spd_cur;
            Time_cnt = G.Player.Timer0;
            G.Player.Spd_cur = speed / 2;
            G.Player.Timer0 = Time_cnt - 1;
            if (Time_cnt == 0)
            {
                Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
                G.Player.Timer0 = 1;
            }
        }
        else
        {
            Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);

            G.Player.Timer0 = 0;
        }
        break;
    case 2:
        PL_ROUTINE_HALF2 = 0;
        Snd_se_walk(0, 0);
        break;
    }
    Add_speedXZ(0);
}

VOID pl_mv04(VOID)
{
    if (!(G.SaveData.Key & (KEY_RIGHT | KEY_LEFT)))
    {
        G.Player.Routine_3 = 2;
    }
    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Move_no = 2;
        G.Player.Routine_3 = 1;
        G.Player.Hokan_flg = 3;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Damage_no = 0;
        G.Player.Spd_cur = 1;
        if (G.Status_flg & STAT_WATER)
        {
            Snd_se_walk(0, 0);
        }

    case 1:
        if (EM_CUR->Move_cnt == 8)
        {
            Snd_se_walk(0, 0);
        }
        if (MAIN.pEm->Move_cnt == 0x16)
        {
            Snd_se_walk(0, -4);
        }
        Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
        return;
    case 2:
        PL_ROUTINE_HALF2 = 0;
        Snd_se_walk(0, 0);
        return;
    }
}

VOID Pl_pad_turn(VOID)
{
    switch (G.SaveData.Key & (KEY_UP | KEY_RIGHT | KEY_DOWN | KEY_LEFT))
    {
    case 1:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 9:
    case 10:
    case 11:
    case 12:
        break;
    case 2:
        if (MAIN.Player.Routine_2 != 4)
        {
            G.Player.Routine_3 = 0;
        }
        G.Player.Routine_2 = 4;
        break;
    case 8:
        if (MAIN.Player.Routine_2 != 5)
        {
            G.Player.Routine_3 = 0;
        }
        G.Player.Routine_2 = 5;
        break;
    }
}

VOID Pl_nop09(VOID)
{
}

VOID pl_mv06(VOID)
{
    switch (MAIN.Player.Routine_3)
    {
    case 0:
        G.Player.Routine_3 = 1;

        G.Player.Move_no = 4;
        G.Player.Move_cnt = 0;
        G.Player.Hokan_flg = 3;

    case 1:
        if (Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400) != 0)
        {
            if (pAot_data->Id == 0xD)
            {
                MAIN.System_flg |= SYS_REQ_FILE;
            }
            else
            {
                MAIN.System_flg |= SYS_REQ_ITEM_GET;
            }
            G.Player.Move_cnt = 0;
            G.Player.Move_no = 0;
            G.Player.Hokan_flg = 0;
            G.Player.Routine_3 += 1;
            STOP_FLG_CUR |= STOP_ACTION;
            G.Player.Sca_info &= 0x7F;
        }
        break;
    case 2:
        Joint_move(0, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x400);
        G.Player.Routine_1 = 0;
        PL_ROUTINE_HALF2 = 0;
        break;
    }
}

VOID pl_mv03(VOID)
{
    ENEMY_WORK* pEm;
    UCHAR n;
    INT found;
    UCHAR v;

    if (!(G.SaveData.Key & KEY_DOWN))
    {
        G.Player.Routine_3 = 2;
    }
    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 3;
        G.Player.Damage_no = 0;
        G.Player.Timer0 = 0;
        if (G.Status_flg & STAT_WATER)
        {
            Snd_se_walk(0, 0);
        }

    case 1:
        pEm = MAIN.Enemy;
        n = G.Enemy_num;
        found = 0;
        if (n != 0)
        {
        loop:
            if (!(pEm->Be_flg & EM_BE_ENABLED))
            {
                pEm++;
                goto next;
            }

            if (pEm->Life >= 0 && !(pEm->Type & 0x80))
            {
                found |= L_pos_ckXZ(0x200, 0x1F40, POS_VEC(pEm));
            }
            pEm++;
            n--;

        next:
            if (n & 0xFF)
            {
                goto loop;
            }
        }
        if (!(found & 0xFF))
        {
            if ((UCHAR)MAIN.Player.Move_no != 3)  // truncates to a byte
            {
                G.Player.Move_no = 3;
                G.Player.Move_cnt = 0;
                G.Player.Move_wait = 0;

                G.Player.Hokan_flg = 3;
            }
            if (G.Player.Timer0 == 0 && (G.pEm->Move_cnt == 8 || G.pEm->Move_cnt == 0x16))
            {
                Snd_se_walk(0, 0);

                if (G.Status_flg & STAT_WATER)
                {
                    Pad_vib_fade_set(0xF, 0, 0x46, 0);
                    Pad_vib_fade_set(0xA, 0x46, 0, 0xF);
                }
            }
            MAIN.Player.Spd_cur = 0x3C;
            v = G.pEm->Move_cnt;
            if (v >= 5 || v < 8)
            {
                G.Player.Spd_cur = 0x40;
            }
        }
        else
        {
            if ((UCHAR)MAIN.Player.Move_no != 2)  // truncates to a byte
            {
                G.Player.Move_no = 2;
                G.Player.Move_cnt = 0;
                G.Player.Move_wait = 0;

                G.Player.Hokan_flg = 3;
            }

            if ((G.pEm->Move_cnt == 7 || G.pEm->Move_cnt == 0x1B) && G.pEm->Move_wait == 1
                && G.Player.Timer0 == 0)
            {
                Snd_se_walk(0, -4);

                if (G.Status_flg & STAT_WATER)
                {
                    Pad_vib_fade_set(0xF, 0, 0x46, 0);
                    Pad_vib_fade_set(0xA, 0x46, 0, 0xF);
                }
            }

            G.Player.Spd_cur = 0x3C;
        }
        if (G.Status_flg & STAT_WATER)
        {
            SHORT Time_cnt;
            SHORT speed = G.Player.Spd_cur;
            Time_cnt = G.Player.Timer0;
            G.Player.Spd_cur = speed / 2;
            G.Player.Timer0 = Time_cnt - 1;
            if (Time_cnt == 0)
            {
                Joint_move(0, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x400);
                G.Player.Timer0 = 1;
            }
        }
        else
        {
            Joint_move(0, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x400);
        }
        break;
    case 2:
        PL_ROUTINE_HALF2 = 0;
        Snd_se_walk(0, 0);
        break;
    }
    Add_speedXZ(0x800);
}

VOID pl_mv02(VOID)
{
    INT unused;
    USHORT pad;
    USHORT Time_cnt;
    INT start_frame;
    UINT Stop_frm;
    UCHAR Stop_timer;

    if (MAIN.System_flg & SYS_PUSH_OBJ)
    {
        G.Player.Routine_1 = 1;
        PL_ROUTINE_HALF2 = 0x10;
        return;
    }

    if (G.SaveData.Key_trg & KEY_INSPECT)
    {
        if (Oma_pl_updown_ck() != 0)
        {
            G.Player.Damage_no = 0x80;
            G.Player.Routine_1 = 1;
            PL_ROUTINE_HALF2 = 0xA;
            STOP_FLG_CUR &= ~STOP_ACTION;

            MAIN.System_flg |= SYS_CLIMB;
            return;
        }

        if (Check_action_object() != 0)
        {
            G.Player.Routine_1 = 1;
            PL_ROUTINE_HALF2 = 0xC;

            MAIN.Stop_flg = STOP_FLG_CUR & ~STOP_ACTION;

            G.Player.Sca_info |= 0x80;

            return;
        }

        if (G.Player.Zone_in_flg & 0x20)
        {
            G.Player.Damage_no = 0x80;
            G.Player.Routine_1 = 1;

            MAIN.Stop_flg = STOP_FLG_CUR & ~STOP_ACTION;

            MAIN_PL_ROUTINE_HALF2 = 0x11;
            if (MAIN.System_flg & SYS_LADDER)
            {
                MAIN_PL_ROUTINE_HALF2 = 0xB;
            }
        }
        else
        {
            goto block_10;
        }
    }
    else
    {
    block_10:
        if (MAIN.Player.Routine_3 < 2U)
        {
            pad = G.SaveData.Key;
            if (!(pad & 1))
            {
                G.Player.Routine_3 = 2;
            }

            if ((pad & 0x100) && G.Player.Type != 0)
            {
                G.Player.Routine_3 = 2;
            }
        }

        MAIN.Player.Cdir_y = (((G.Player.Cdir_y + Pl_turn_run * ((G.SaveData.Key & (0x1 | 0x2)) >> 1)) & 0xFFF) - Pl_turn_run * ((G.SaveData.Key & (0x1 | 0x8)) >> 3)) & 0xFFF;

        switch (G.Player.Routine_3)
        {
        case 0:
            G.Player.Spd_cur = 0xD2;
            start_frame = G.Player.Move_cnt;
            G.Player.Move_wait = 1;
            G.Player.Move_no = 3;
            G.Player.Routine_3 = 1;
            G.Player.Hokan_flg = 3;
            G.Player.Damage_no = 0;
            G.Player.Move_cnt = 1;
            ScratchWord[0] = start_frame;
            if (start_frame - 0xA < 0xFU)
            {
                G.Player.Move_cnt = 0xC;
            }
            G.Player.Timer0 = 0;
            if (G.Status_flg & STAT_WATER)
            {
                Snd_se_walk(1, 0);
            }

        case 1:
            if (G.Player.Timer0 == 0)
            {
                if (G.pEm->Move_cnt == 0)
                {
                    Snd_se_walk(1, 0);

                    if (G.Status_flg & STAT_WATER)
                    {
                        Pad_vib_fade_set(8, 0, 0x50, 0);
                        Pad_vib_fade_set(0xF, 0x50, 0, 8);
                    }
                }

                if (G.pEm->Move_cnt == 0xA)
                {
                    Snd_se_walk(1, -4);

                    if (G.Status_flg & STAT_WATER)
                    {
                        Pad_vib_fade_set(8, 0, 0x50, 0);
                        Pad_vib_fade_set(0xF, 0x50, 0, 8);
                    }
                }
            }
            MAIN.Player.Spd_cur = 0xD2;
            if (G.Status_flg & STAT_WATER)
            {
                Time_cnt = G.Player.Timer0;
                G.Player.Spd_cur = 0x69;
                G.Player.Timer0 = Time_cnt - 1;
                if (Time_cnt == 0)
                {
                    Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
                    G.Player.Timer0 = 1;
                }
            }
            else
            {
                Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
                G.Player.Timer0 = 0;
            }
            if (!(G.SaveData.Key & KEY_RUN))
            {
                PL_ROUTINE_HALF2 = 0x101;
                Stop_frm = G.Player.Move_cnt;
                G.Player.Routine_1 = 0;
                G.Player.Move_cnt = 0xA;
                ScratchWord[0] = Stop_frm;
                if (Stop_frm - 1 < 0xBU)
                {
                    G.Player.Move_cnt = 0x19;
                }
                G.Player.Move_no = 2;
                G.Player.Hokan_flg = 3;
            }
            break;
        case 2:
            G.Player.Move_cnt = 0;
            G.Player.Move_wait = 0;
            G.Player.Move_no = 0;
            G.Player.Routine_3 = 3;
            G.Player.Hokan_flg = 3;
            G.Player.Timer2 = 0;

        case 3:
            Joint_move(0, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x400);
            Stop_timer = G.Player.Timer2 + 1;
            G.Player.Timer2 = Stop_timer;
            if ((Stop_timer & 0xFF) >= 4U)
            {
                G.Player.Routine_1 = 0;
                PL_ROUTINE_HALF2 = 0;
                G.Player.Spd_cur = 0;
                Snd_se_walk(1, 0);
            }
            G.Player.Spd_cur -= 0x1E;
            break;
        }

        Add_speedXZ(0);
    }
}

VOID Pl_nop(VOID)
{
}

VOID pl_mv07(VOID)
{
    UCHAR* p;
    INT dx;
    INT dz;
    INT dir_neg;
    USHORT prev;
    USHORT Rsv2;
    UINT unused;

    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Move_no = 2;
        G.Player.Hokan_flg = 3;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Spd_cur = 0x5D;
        G.Player.Routine_3 += 1;

    case 1:
        ScratchPad->Vec.vx = G.pEm->Root_x;
        ScratchPad->Vec.vz = G.pEm->Root_z;
        ScratchPad->Vec.vy = 0;
        Goto00(&ScratchPad->Vec, 0x40);
        Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x200);
        Add_speedXZ(0);
        dx = G.Player.Pos_x - G.Player.Root_x;
        dz = G.Player.Pos_z - G.Player.Root_z;
        if (SquareRoot0(dx * dx + dz * dz) < 0x384)
        {
            goto inc;
        }
        return;
    case 2:
        Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x200);
        prev = G.Player.Cdir_y;
        if (prev & 0x400)
        {
            G.Player.Cdir_y = prev - (UCHAR)(prev >> 2);  // truncates to a byte
        }
        else
        {
            G.Player.Cdir_y = prev + (UCHAR)(prev >> 2);  // truncates to a byte
        }
        if (G.Player.Cdir_y & 0x3E0)
        {
            break;
        }
    inc:
        MAIN.Player.Routine_3 += 1;
        return;
    case 3:
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 0x33;
        if (G.Player.Zone_in_flg & 0x10)
        {
            G.Player.Move_no = 0x35;
        }
        G.Player.Hokan_flg = 3;
        G.Player.Spd_cur = 0;
        G.Player.Routine_3 += 1;
        Kage_rect_set(&MAIN.Player.Kage_work, 0x320, 0x2BC, 0x2BC, 0x2BC);  // address passed as an integer

    case 4:
        if (G.Player.Zone_in_flg & 0x10)
        {
            if (G.Player.Move_cnt == 0xF)
            {
                G.Player.Ground = 0xA8C;
                ScratchWord[0] = -0x708;
                if ((SHORT)G.Player.Cdir_y >= 0x801)  // sign-extends the halfword
                {
                    ScratchWord[0] = 0x708;
                }
                G.Player.Kage_work.Off_z += ScratchWord[0];
            }
            if (G_PLAYER->Move_cnt == 0x1A)
            {
                Snd_se_stad(2, 0x17, 0, &MAIN.Player.Pos_x);  // address passed as an integer
            }

            if (G.Player.Move_cnt == 0x18)
            {
                Pad_vib_set0(2, 0);
                Pad_vib_set1(9, 0x78, 0);
                Pad_vib_set0(1, 3);
            }
        }
        else
        {
            if (G.Player.Move_cnt == Ladder_frm_tbl[G.Player.Spd_cur])
            {
                Snd_se_stad(2, 0x23, 0, &MAIN.Player.Pos_x);  // address passed as an integer
                G.Player.Spd_cur = G.Player.Spd_cur + 1;
            }
            if (G.Player.Move_cnt == 0x32)
            {
                Snd_se_stad(2, 0x2D, 0, &MAIN.Player.Pos_x);  // address passed as an integer
            }
        }
        G.Player.Routine_3 += Joint_move(0, G.Player.pRoom_kan_t_ptr, G.Player.pRoom_seq_t_ptr, 0x400);
        return;
    case 5:
        do
        {
            G.Player.Move_cnt = 0;
            G.Player.Move_wait = 0;
            G.Player.Hokan_flg = 0;
            G.Player.Spd_cur = 0;
            G.Player.Move_no += 1;
            Joint_move(0, G.Player.pRoom_kan_t_ptr, G.Player.pRoom_seq_t_ptr, 0x400);
            dir_neg = (SHORT)G.Player.Cdir_y < 0x801;  // sign-extends the halfword

            ScratchWord[2] = -0x3E8;
        } while (0);
        if (dir_neg == 0)
        {
            ScratchWord[2] = 0x3E8;
        }
        G_PLAYER->Pos_y = 0;
        if (G.Player.Zone_in_flg & 0x10)
        {
            ScratchWord[2] = -0x7D0;
            if (dir_neg == 0)
            {
                ScratchWord[2] = 0x7D0;
            }
            MAIN.Player.Pos_y = 0xA8C;
        }
        G_PLAYER->Pos_z += ScratchWord[2];
        G.Player.Old_x = G.Player.Pos_x;
        G.Player.Old_y = G.Player.Pos_y;
        G.Player.Old_z = G.Player.Pos_z;
        Kage_rect_set(&MAIN.Player.Kage_work, 0x1F4, 0x1F4, 0x2BC, 0x2BC);  // address passed as an integer
        G.Player.Kage_work.Off_x = 0;
        G.Player.Kage_work.Off_z = 0;

        G.Player.Ground = G.Player.Pos_y;
        if (G.Player.Zone_in_flg & 0x10)
        {
            G.Player.Routine_3 = 8;
            return;
        }
    case 6:
        G.Player.Routine_3 = 7;
        G.Player.Hokan_flg = 3;
        G.Player.Move_no = 2;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Spd_cur = 0x3C;
        G.Player.Timer0 = 0xF;
        PL_BE_FLG_CUR |= EM_BE_NO_WALL;
        return;
    case 7:
        if (G.pEm->Move_cnt == 8)
        {
            Snd_se_walk(0, 0);
        }
        p = &MAIN.Player.Be_flg;
        Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
        Add_speedXZ(0);
        prev = G.Player.Timer0;
        G.Player.Timer0 = prev - 1;
        if (prev == 0)
        {
            G.Player.Routine_3 = 8;
            *p &= 0xFB;
            Snd_se_walk(0, -4);
            return;
        }
        break;
    case 8:
        G.Player.Damage_no = 0;
        PL_ROUTINE_WORD = 1;

        G.Player.Zone_in_flg &= 0xEF;
        MAIN.System_flg &= ~SYS_LADDER;
        STOP_FLG_CUR |= STOP_ACTION;
        break;
    }
}

INT Em_ck(VOID)
{
    VECTOR V;
    UCHAR best_pos;
    UCHAR best_neg;
    UINT dmin;
    UINT dpos;
    UINT dneg;
    UINT d;
    UCHAR idx;
    UCHAR i;
    UCHAR count;
    ENEMY_WORK* pEm;
    INT* dist;
    INT dx;
    INT dz;

    i = 0;
    best_pos = 0;
    best_neg = 0;
    idx = 0;
    dmin = 0x7FFFFFFF;
    dpos = 0x7FFFFFFF;
    dneg = 0x7FFFFFFF;
    count = G.Enemy_num;
    pEm = MAIN.Enemy;

    if (count != 0)
    {
        dist = &ScratchWord[2];
        do
        {
            if (!(pEm->Be_flg & EM_BE_ENABLED))
            {
                i++;
                pEm++;
                continue;
            }
            if (pEm->Zone_in_flg != 0 && pEm->Life >= 0 && !(pEm->Type & 0xC0) && pEm->Id < 0x13)
            {
                V.vx = dx = pEm->Pos_x - G.Player.Pos_x;
                dz = pEm->Pos_z - MAIN.Player.Pos_z;
                V.vy = 0;
                V.vz = dz;
                *dist = SquareRoot0(dx * dx + dz * dz);

                if (Goto00_ck(POS_VEC(pEm), 0x800) == 0 && !Pl_aim_wall_ck(&V))
                {
                    d = *dist;

                    if (d < dmin)
                    {
                        dmin = d;
                        idx = i + 1;
                    }

                    if (pEm->Pos_y > 0)
                    {
                        if (d < dpos)
                        {
                            dpos = d;
                            best_pos = i + 1;
                        }
                    }
                    else if (d < dneg)
                    {
                        dneg = d;
                        best_neg = i + 1;
                    }
                }
            }

            i++;
            pEm++;
            count--;
        } while (count != 0);
    }
    if (idx == 0)
    {
        G.Player.pTgt = MAIN.Enemy;

        return 0;
    }

    if (G.Player.Type == 1)
    {
        G.Player.pTgt = &MAIN.Enemy[idx - 1];
    }
    else
    {
        if (best_pos != 0)
        {
            G.Player.pTgt = &MAIN.Enemy[best_pos - 1];
        }
        if (best_neg != 0)
        {
            G.Player.pTgt = &MAIN.Enemy[best_neg - 1];
        }
    }

    return 1;
}

INT pl_mv05_sub(VOID)
{
    VECTOR V;
    UCHAR i;
    ENEMY_WORK* pEm;
    SCHAR n;

    i = 0;
    pEm = G.Player.pTgt;
    n = G.Enemy_num;
    while (pEm != &MAIN.Enemy[i])
    {
        i++;
    }

    i++;

    pEm++;

    if (i == 30)
    {
        i = 0;
        pEm = MAIN.Enemy;
    }
    while (n != 0)
    {
        if (!(pEm->Be_flg & EM_BE_ENABLED))
        {
            i++;
            pEm++;
            if (i == 30)
            {
                i = 0;
                pEm = MAIN.Enemy;
            }
            continue;
        }
        if (pEm->Zone_in_flg != 0 && pEm->Life >= 0 && !(pEm->Type & 0xC0) && pEm->Id < 0x13)
        {
            V.vx = pEm->Pos_x - G.Player.Pos_x;
            V.vz = pEm->Pos_z - MAIN.Player.Pos_z;
            V.vy = 0;
            if (Pl_aim_wall_ck(&V) == 0)
            {
                MAIN.Player.pTgt = pEm;
                return 1;
            }
        }
        i++;
        pEm++;
        n--;
    }
    return 0;
}

UCHAR Pl_aim_wall_ck(VECTOR* dir)
{
    VECTOR v1;
    VECTOR v2;
    SCA_HEAD* pSca;
    USHORT* pWall;
    USHORT a, b, c, d;
    INT Ax, Az;
    INT px, pz;

    pSca = G.pRoom->pSca;
    pWall = pSca->Group[0];
    px = G.Player.Pos_x / 18;
    pz = G.Player.Pos_z / 18;
    Ax = dir->vx / 18;
    Az = dir->vz / 18;
    for (ScratchPtr[11] = pSca->Group[4]; pWall < (USHORT*)ScratchPtr[11]; pWall += 6)
    {
        a = pWall[0] / 18;
        b = pWall[1] / 18;
        c = pWall[2] / 18;
        d = pWall[3] / 18;
        if ((pWall[5] &= 0x300) != 0x300)
        {
            continue;
        }
        if (pWall[4] == 4 || pWall[4] == 5)
        {
            continue;
        }
        {
            INT z0, z1, x0, x1;
            x0 = c;
            x1 = a;
            z0 = b;
            z1 = d;
            ScratchPad->Vec.vy = 0;
            v1.vy = 0;
            v2.vy = 0;
            ScratchPad->Vec.vx = x0 - x1;
            ScratchPad->Vec.vz = z0 - z1;
            v1.vx = px + Ax - x1;
            v1.vz = pz + Az - z1;
            v2.vx = px - x1;
            v2.vz = pz - z1;
            OuterProduct0(&ScratchPad->Vec, &v1, &v1);
            OuterProduct0(&ScratchPad->Vec, &v2, &v2);
            if ((v1.vy ^ v2.vy) & 0x80000000)
            {
                v1.vx = x0 - px;
                v1.vz = z0 - pz;
                v1.vy = 0;
                OuterProduct0(dir, &v1, &v1);
                v2.vx = x1 - px;
                v2.vz = z1 - pz;
                v2.vy = 0;
                VectorNormal(&v2, &v2);
                OuterProduct0(dir, &v2, &v2);
                if ((v1.vy ^ v2.vy) & 0x80000000)
                {
                    return 1;
                }
            }
        }
        {
            INT z0, z1, x0, x1;
            x0 = a;
            x1 = c;
            z0 = b;
            z1 = d;
            ScratchPad->Vec.vy = 0;
            v1.vy = 0;
            v2.vy = 0;
            ScratchPad->Vec.vx = x0 - x1;
            ScratchPad->Vec.vz = z0 - z1;
            v1.vx = px + Ax - x1;
            v1.vz = pz + Az - z1;
            v2.vx = px - x1;
            v2.vz = pz - z1;
            OuterProduct0(&ScratchPad->Vec, &v1, &v1);
            OuterProduct0(&ScratchPad->Vec, &v2, &v2);
            if ((v1.vy ^ v2.vy) & 0x80000000)
            {
                v1.vx = x0 - px;
                v1.vz = z0 - pz;
                v1.vy = 0;
                OuterProduct0(dir, &v1, &v1);
                v2.vx = x1 - px;
                v2.vz = z1 - pz;
                v2.vy = 0;
                OuterProduct0(dir, &v2, &v2);
                if ((v1.vy ^ v2.vy) & 0x80000000)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

VOID Kage_work_init(SVECTOR* pPos, KAGE_WORK* pK, SHORT Half_x, SHORT Half_z)
{
    SVECTOR Sv;
    POLY_FT4* pFt4;

    pFt4 = &pK->Prim[0];
    VIEW(INT, pK->Prim[0].r0) = ScratchWord[7];  // INT* view of the field (a plain access changes the code)
    SetPolyFT4(pFt4);
    SetSemiTrans(pFt4, 1);
    pFt4->tpage = GetTPage(1, 2, 0x180, 0x100);
    pFt4->clut = GetClut(0, 0x1E7);
    pFt4->u0 = 0x51;
    pFt4->v0 = 0xC8;
    pFt4->u1 = 0x6B;
    pFt4->v1 = 0xC8;
    pFt4->u2 = 0x51;
    pFt4->v2 = 0xE5;
    pFt4->u3 = 0x6B;
    pFt4->v3 = 0xE5;
    Sv.vx = Half_x * -1;
    Sv.vy = 0;
    Sv.vz = Half_z;
    pK->Vtx[0] = Sv;
    Sv.vx = Sv.vx * -1;
    pK->Vtx[1] = Sv;
    Sv.vx = Sv.vx * -1;
    Sv.vz = Sv.vz * -1;
    pK->Vtx[2] = Sv;
    Sv.vx = Sv.vx * -1;
    pK->Vtx[3] = Sv;
    *(SVECTOR*)pK = *pPos;  // reinterprets the memory as SVECTOR

    Memcpy(&pK->Prim[1], pFt4, sizeof(POLY_FT4));
}

VOID Kage_work_color_set(KAGE_WORK* pK, INT Trans, INT Abr, INT Col)
{
    INT rgb;
    USHORT tpage;

    INT mask;

    tpage = GetTPage(1, 2, 0x180, 0x100);

    mask = 0xFF000000;

    pK->Prim[0].tpage = tpage;
    pK->Prim[1].tpage = tpage;
    SetSemiTrans(&pK->Prim[0], Trans);
    SetSemiTrans(&pK->Prim[1], Trans);

    rgb = Col & 0xFFFFFF;
    VIEW(INT, pK->Prim[0].r0) = rgb | (VIEW(INT, pK->Prim[0].r0) & mask);  // INT* view of the field (a plain access changes the code)
    VIEW(INT, pK->Prim[1].r0) = rgb | (VIEW(INT, pK->Prim[1].r0) & mask);  // INT* view of the field (a plain access changes the code)
}

VOID Kage_work_size_add(KAGE_WORK* pK, INT dx, INT dz)
{
    pK->Vtx[0].vx -= dx;
    pK->Vtx[0].vz += dz;
    pK->Vtx[1].vx += dx;
    pK->Vtx[1].vz += dz;
    pK->Vtx[2].vx -= dx;
    pK->Vtx[2].vz -= dz;
    pK->Vtx[3].vx += dx;
    pK->Vtx[3].vz -= dz;
}

VOID Kage_half_set(KAGE_WORK* pK, SHORT Half_x, INT Half_z)
{
    SHORT unused;
    SHORT Neg_w;

    Neg_w = -Half_x;
    pK->Vtx[0].vz = Half_z;
    pK->Vtx[1].vz = Half_z;
    pK->Vtx[0].vx = Neg_w;
    pK->Vtx[1].vx = Half_x;
    pK->Vtx[2].vx = Neg_w;
    pK->Vtx[2].vz = -Half_z;
    pK->Vtx[3].vx = Half_x;
    pK->Vtx[3].vz = -Half_z;
}

VOID Kage_rect_set(KAGE_WORK* pK, SHORT x_pos, SHORT x_neg, SHORT z_pos, INT z_neg)
{
    SHORT Min_x;
    INT Min_z;

    Min_x = -x_neg;
    pK->Vtx[0].vx = Min_x;
    VIEW(USHORT, pK->Vtx[0].vz) = z_pos;  // USHORT* view of the field (a plain access changes the code)
    pK->Vtx[1].vx = x_pos;
    pK->Vtx[1].vz = z_pos;
    pK->Vtx[2].vx = Min_x;
    pK->Vtx[3].vx = x_pos;
    Min_z = -z_neg;
    pK->Vtx[2].vz = Min_z;
    pK->Vtx[3].vz = Min_z;
}

VOID MulKage(VECTOR* pPos, KAGE_WORK* pK, SHORT Grand, SHORT Dir)
{
    MATRIX m;
    MATRIX Ls;
    INT unused[2];
    LONG p;
    LONG flag;
    UINT z;

    m = GsIDMATRIX;
    m.t[0] = pK->Off_x + pPos->vx;  // sign-extends the halfword
    m.t[1] = Grand;
    m.t[2] = pK->Off_z + pPos->vz;
    RotMatrixY(Dir, &m);
    GsMulCoord0(&GsWSMATRIX, &m, &Ls);
    SetRotMatrix(&Ls);
    SetTransMatrix(&Ls);
    z = RotAverage4(&pK->Vtx[0], &pK->Vtx[1], &pK->Vtx[2],
        &pK->Vtx[3], (LONG*)&pK->Prim[CBUF_ID_CUR].x0,  // LONG* view of the same memory
        (LONG*)&pK->Prim[MAIN.Cbuf_id].x1,  // word view of the same memory
        (LONG*)&pK->Prim[MAIN.Cbuf_id].x2,  // word view of the same memory
        (LONG*)&pK->Prim[MAIN.Cbuf_id].x3, &p, &flag);  // word view of the same memory

    if (G.Status_flg & STAT_SHADOW_CLAMP)
    {
        if (z >= 0xFF0)
        {
            z = 0xFF0;
        }
        AddPrim(MAIN.Ot1[MAIN.Cbuf_id].org + (z >> 4),
            &pK->Prim[MAIN.Cbuf_id]);
    }
    else
    {
        AddPrim(MAIN.Ot2[MAIN.Cbuf_id].org + 1,
            &pK->Prim[MAIN.Cbuf_id]);
    }
}

VOID Kage_work_set(INT nKage)
{
#ifdef __INTELLISENSE__
    FREE_WORK_CUR = (UCHAR*)FREE_WORK_CUR + (nKage & 0xFF) * sizeof(KAGE_WORK);  // IntelliSense rejects void* arithmetic
#else
    FREE_WORK_CUR += (nKage & 0xFF) * sizeof(KAGE_WORK);
#endif
}

VOID pl_dm00(VOID)
{
    switch (MAIN.Player.Routine_3)
    {
    case 0:
        G.Player.Move_no = 1;
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 3;
        G.Player.Spd_cur = 0;
        Snd_se_stad(3, 0, 0, &MAIN.Player.Pos_x);  // address passed as an integer

    case 1:
        if (Joint_move(0, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x400) != 0)
        {
            PL_ROUTINE_WORD = 1;
            G.Player.Damage_no = 0;
        }
        return;
    }
}

VOID pl_dm01(VOID)
{
    switch (MAIN.Player.Routine_3)
    {
    case 0:
        G.Player.Routine_3 = 1;
        G.Player.Hokan_flg = 3;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Spd_cur = 0xFA;

        G.Player.Move_no = G.Player.Damage_no + 0xFF;

        Snd_se_stad(3, 0, 0, &MAIN.Player.Pos_x);  // address passed as an integer

    case 1:
        if (Joint_move(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400) != 0)
        {
            PL_ROUTINE_WORD = 1;
            G.Player.Damage_no = 0;
        }
        return;
    }
}

VOID pl_dm02(VOID)
{
    switch (MAIN.Player.Routine_3)
    {
    case 0:
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 3;
        G.Player.Spd_cur = 0;

        G.Player.Move_no = G.Player.Damage_no + 0xFF;

        Snd_se_stad(3, 3, 0, &MAIN.Player.Pos_x);  // address passed as an integer

    case 1:
        if (Joint_move(0, G.Player.pSub1_kan_t_ptr, G.Player.pSub1_seq_t_ptr, 0x400) != 0)
        {
            PL_ROUTINE_WORD = 1;
            G.Player.Damage_no = 0;
        }
        return;
    }
}

VOID pl_mv0a(VOID)
{
    INT Se_no;

    switch (MAIN.Player.Routine_3)
    {
    case 0:
        G.Player.Move_no = 0x30;
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 3;

    case 1:
        if (Joint_move(0, G.Player.pRoom_kan_t_ptr, G.Player.pRoom_seq_t_ptr, 0x400) != 0)
        {
            G.Player.Routine_3 = 2;

            G.Player.Move_wait = 0;
            G.Player.Hokan_flg = 3;

            G.Player.Spd_cur = 0;

            G.Player.Move_no += 1;
            return;
        }
        return;
    case 2:
        Joint_move(0, G.Player.pRoom_kan_t_ptr, G.Player.pRoom_seq_t_ptr, 0x400);
        if (G.Player.Move_cnt < 0x10U)
        {
            G.Player.Spd_cur = 0x32;

            Add_speedXZ(0);
        }
        if (!(G.System_flg & SYS_PUSH_OBJ))
        {
            G.Player.Move_cnt = 0;
            G.Player.Move_wait = 0;

            G.Player.Routine_3 = 3;

            G.Player.Hokan_flg = 3;

            G.Player.Spd_cur = 0;

            G.Player.Move_no += 1;

            Pad_vib_init();
        }
        if (G.Player.Move_cnt == 1)
        {
            Se_no = 0x16;
            if (pPush_om->Id & 0x40)
            {
                Se_no = 0x17;
            }
            Snd_se_stad(2, Se_no, 0, &MAIN.Player.Pos_x);  // address passed as an integer

            Pad_vib_set0(1, 0);
            Pad_vib_set1(3, 0x78, 0);
            Pad_vib_fade_set(0xF, 0x78, 0, 3);

            return;
        }
        break;
    case 3:
        if (Joint_move(0, G.Player.pRoom_kan_t_ptr, G.Player.pRoom_seq_t_ptr, 0x400) != 0)
        {
            PL_ROUTINE_WORD = 1;
            STOP_FLG_CUR |= STOP_ACTION;
        }
        break;
    }
}

VOID pl_mv08(VOID)
{
    PARTS_W* pP_ptr;

    ENEMY_WORK* pEm;

    USHORT v;
    INT f;

    pP_ptr = G.Player.pSin_parts_ptr;
    switch (G.Player.Routine_3)
    {
    case 0:
        G.Player.Move_no = 2;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 3;
        G.Player.Spd_cur = 0;
        Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
        if (G.System_flg & SYS_CLIMB)
        {
            v = G.Player.Cdir_y;
            if (v & 0x200)
            {
                G.Player.Cdir_y = v + (UCHAR)(v >> 2);  // truncates to a byte
            }
            else
            {
                G.Player.Cdir_y = v - (UCHAR)(v >> 2);  // truncates to a byte
            }
            STATUS_FLG_CUR |= STAT_STAIRS_ON;
        }

        if (G.Status_flg & STAT_STAIRS_ZONE)
        {
            if (G.Player.Zone_in_flg & 0x40)
            {
                pEm = G.pEm;
                v = pEm->Cdir_y;
                if (!(v & 0x400))
                {
                    pEm->Cdir_y = v + (UCHAR)(v >> 2);  // truncates to a byte
                }
                else
                {
                    pEm->Cdir_y = v - (UCHAR)(v >> 2);  // truncates to a byte
                }
            }
            else
            {
                pEm = G.pEm;
                v = pEm->Cdir_y;
                if (v & 0x400)
                {
                    pEm->Cdir_y = v + (UCHAR)(v >> 2);  // truncates to a byte
                }
                else
                {
                    pEm->Cdir_y = v - (UCHAR)(v >> 2);  // truncates to a byte
                }
            }
        }

        v = MAIN.Player.Cdir_y;
        if (!(v & 0x3E0))
        {
            G.Player.Routine_3 = 1;

            G.Player.Cdir_y = v & 0xFF00;
        }
        break;
    case 1:
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 0x33;
        if (G.Player.Zone_in_flg & 0x10)
        {
            G.Player.Move_no = 0x35;
        }
        G.Player.Routine_3 = 2;
        G.Player.Hokan_flg = 3;
        G.Player.Spd_cur = 0;
        Kage_rect_set(&MAIN.Player.Kage_work, 0x320, 0x2BC, 0x2BC, 0x2BC);

    case 2:
        if (G.System_flg & SYS_CLIMB)
        {
            if (G.Player.Move_cnt == G.Player.Spd_cur * 15 + 12)
            {
                Snd_se_stad(2, 0x23, 0, &MAIN.Player.Pos_x);  // address passed as an integer
                G.Player.Spd_cur++;
            }
            if (G_PLAYER->Spd_cur == 3)
            {
                G_PLAYER->Spd_cur = 7;
            }
            if (G_PLAYER->Spd_cur == 7 && MAIN.Player.Move_cnt == 0x35)
            {
                Snd_se_stad(2, 0x23, 0, &MAIN.Player.Pos_x);  // address passed as an integer
            }
            if ((G.Player.Zone_in_flg & 0x10) && G.Player.Spd_cur == 2)
            {
                G.Player.Spd_cur = 5;
            }
            if (G_PLAYER->Spd_cur >= 5 && G.Player.Move_cnt == (G.Player.Spd_cur - 5) * 9 + 0x2C)
            {
                G.Player.Spd_cur = 6;
                Snd_se_walk(0, 0);
            }
        }
        else
        {
            if (G.Player.Move_cnt == G.Player.Spd_cur * 15 + 12)
            {
                Snd_se_stad(2, 0x2D, 0, &MAIN.Player.Pos_x);  // address passed as an integer

                G.Player.Spd_cur++;
            }
        }
        ScratchPad->Sv.vx = 0;
        ScratchPad->Sv.vz = 0;

        ScratchPad->Sv.vy = -0x29;

        if (G.Player.Zone_in_flg & 0x10)
        {
            ScratchPad->Sv.vy = 0x29;
        }

        if (G.SaveData.Room_no == 0xE || G.SaveData.Room_no == 5)
        {
            ScratchPad->Sv.vy = -0x24;
            if (G.Player.Zone_in_flg & 0x10)
            {
                ScratchPad->Sv.vy = 0x24;
            }
        }
        if (G.System_flg & SYS_CLIMB)
        {
            G.Player.Sce_flg |= 0x40;
        }
        G.Player.Kage_work.Off_x = pP_ptr->Workm.t[0] - G.Player.Pos_x;
        G.Player.Ground += ScratchPad->Sv.vy;
        G.Player.Kage_work.Off_z = pP_ptr->Workm.t[2] - G.Player.Pos_z;
        G.Player.Routine_3 += Joint_move(0, G.Player.pRoom_kan_t_ptr, G.Player.pRoom_seq_t_ptr, 0x400);
        break;
    case 3:
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 0;
        G.Player.Spd_cur = 0;
        G.Player.Move_no++;
        Joint_move(0, G.Player.pRoom_kan_t_ptr, G.Player.pRoom_seq_t_ptr, 0x400);
        ScratchWord[2] = 0;

        ScratchWord[0] = G.Player.Timer0 * 4350;

        if (G.Player.Cdir_y & 0x400)
        {
            ScratchWord[0] = 0;

            ScratchWord[2] = G.Player.Timer0 * 4350;
        }

        ScratchWord[1] = -0xB45;

        if (G.Player.Zone_in_flg & 0x10)
        {
            ScratchWord[1] = 0xB45;
        }
        if (G.SaveData.Room_no == 0xE || G.SaveData.Room_no == 5)
        {
            ScratchWord[0] = G.Player.Timer0 * 2114;

            ScratchWord[2] = 0;
            if (G.Player.Cdir_y & 0x400)
            {
                ScratchWord[0] = 0;

                ScratchWord[2] = G.Player.Timer0 * 2114;
            }

            ScratchWord[1] = -0x57A;
            if (G.Player.Zone_in_flg & 0x10)
            {
                ScratchWord[1] = 0x57A;
            }
        }
        f = MAIN.System_flg;
        if (f & 0x80)
        {
            ScratchWord[2] = 0;
            ScratchWord[0] = -G.Player.Timer0 * 1850;
            if (G.Player.Cdir_y & 0x400)
            {
                ScratchWord[0] = 0;
                ScratchWord[2] = G.Player.Timer0 * 1850;
            }
            ScratchWord[1] = -0x708;
            if (G.Player.Zone_in_flg & 0x10)
            {
                ScratchWord[1] = 0x708;
                MAIN.System_flg = f & ~SYS_CLIMB;

                G.Player.Sce_flg &= 0xFFBF;
            }
            STATUS_FLG_CUR &= ~STAT_STAIRS_ON;
        }
        G_PLAYER->Pos_x += ScratchWord[0];
        G.Player.Pos_y += ScratchWord[1];
        G.Player.Pos_z += ScratchWord[2];
        G.Player.Old_x = G.Player.Pos_x;
        G.Player.Old_y = G.Player.Pos_y;
        G.Player.Old_z = G.Player.Pos_z;
        Kage_rect_set(&MAIN.Player.Kage_work, 0x1F4, 0x1F4, 0x2BC, 0x2BC);
        G.Player.Kage_work.Off_x = 0;
        G.Player.Kage_work.Off_z = 0;
        G.Player.Damage_no = 0;
        PL_ROUTINE_WORD = 1;
        G.Player.Ground = G.Player.Pos_y;
        STOP_FLG_CUR |= STOP_ACTION;

        STATUS_FLG_CUR &= ~STAT_STAIRS_ZONE;

        break;
    }
}

// player collision box records, 12 bytes each; the first and third are chosen by the player id through Pl_sca_ptr
UCHAR Pl_sca_tbl[0x24] =
{
    0x00,   // 0 //
    0x80,   // 1 //
    0x00,   // 2 //
    0x00,   // 3 //
    0x06,   // 4 //
    0xFA,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0xFA,   // 8 //
    0x05,   // 9 //
    0xA6,   // 10 //
    0x01,   // 11 //
    0x00,   // 12 //
    0x80,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0x90,   // 16 //
    0x01,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
    0x00,   // 20 //
    0x00,   // 21 //
    0x00,   // 22 //
    0x00,   // 23 //
    0x00,   // 24 //
    0x80,   // 25 //
    0x00,   // 26 //
    0x00,   // 27 //
    0xBA,   // 28 //
    0xFA,   // 29 //
    0x00,   // 30 //
    0x00,   // 31 //
    0x46,   // 32 //
    0x05,   // 33 //
    0x74,   // 34 //
    0x01,   // 35 //
};

// pointers to the collision box records (the player id picks entry 0 or 2), then two constant words
AT_TBL* Pl_sca_ptr[5] =
{
    (AT_TBL*)Pl_sca_tbl,           // 0 //
    (AT_TBL*)(Pl_sca_tbl + 0xC),   // 1 //
    (AT_TBL*)(Pl_sca_tbl + 0x18),  // 2 //
    (AT_TBL*)0x00FA0005,           // 3 //
    (AT_TBL*)0x000500FA,           // 4 //
};

// player state routines, by state (0 init, 1 move, 2 Damage, 3 die, 4 dead)
PL_FUNC Pl_routine[5] =
{
    Pl_init,     // 0 //
    Pl_move,     // 1 //
    Pl_damage,   // 2 //
    Pl_die,      // 3 //
    Pl_dead,     // 4 //
};

// behavior routines run by Pl_mv_sub1 and Pl_mv_sub4, by Routine_2; the table goes on in Pl_mv
PL_FUNC Pl_act_tbl[4] =
{
    Pl_Em_damage,   // 0 //
    Pl_Em_die,   // 1 //
    Pl_state_anim_window2,   // 2 //
    Pl_Scenario,               // 3 //
};

// player sub-state routines, by Key_rno; also the entries of the behavior table from Routine_2 4
PL_FUNC Pl_mv[19] =
{
    Pl_mv_sub0,              // 0 //
    Pl_mv_sub1,              // 1 //
    Pl_mv_sub2,   // 2 //
    pl_mv05,                         // 3 //
    Pl_mv_sub4,              // 4 //
    Pl_nop09,          // 5 //
    pl_mv08,                         // 6 //
    pl_mv07,                         // 7 //
    pl_mv06,                         // 8 //
    pl_mv02,                         // 9 //
    Pl_nop,             // 10 //
    Pl_nop,             // 11 //
    pl_mv0a,                         // 12 //
    pl_mv08,                         // 13 //
    Pl_knife_aim,          // 14 //
    Pl_knife_hold,         // 15 //
    Pl_knife_swing,        // 16 //
    Pl_knife_holster,      // 17 //
    Pl_knife_turn,         // 18 //
};

UCHAR Ladder_frm_tbl[4] = { 0x0C, 0x1D, 0x27, 0x00 }; // animation frames of the climb step sounds, in playing order
