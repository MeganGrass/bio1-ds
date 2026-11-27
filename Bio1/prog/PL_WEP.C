/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\PL_WEP.C
 */

#include "PL_WEP.H"
#include "ATTACK.H"
#include "EFF1.H"
#include "EM_SUB.H"
#include "GUIDE_CK.H"
#include "INVENTORY.H"
#include "ITEM.H"
#include "JOINT.H"
#include "MAG.H"
#include "PAD.H"
#include "PLAYER.H"
#include "SCE0.H"
#include "SCE1.H"
#include "SND.H"
#include "ST_SUB03.H"
#include <RAND.H>
#include "FLAG.H"
#include "MONITOR.H"

// this unit calls these with plain int arguments and results (the forms it was compiled with)

VOID Pl_gun_aim(VOID)
{
    UCHAR* p;

    USHORT x;
    INT y;

    switch (MAIN.Player.Routine_3)
    {
    case 0:
        G.Player.Routine_3 = 1;
        G.Player.Move_no = 5;
        G.Player.Hokan_flg = 3;
        G.Player.Move_cnt = 0;
        G.Player.Damage_no = 0;
        G.Player.Spd_cur = 1;
        G.Player.Aim_flg = 0;
        if (G.Status_flg & STAT_WATER)
        {
            Snd_se_walk(0, 0);
        }

        if (Aim_auto_en != 0)
        {
            G.Player.Aim_sts = Em_ck();
        }

        G.Player.Aim_sts |= 2;

    case 1:

        if (Aim_auto_en != 0)
        {
            if (G.Padd1 & PAD_L1)
            {
                G.Player.Aim_sts = 0;
            }
            p = &MAIN.Player.Be_flg;
            if (G.Player.Aim_sts == 3)
            {
                ScratchWord[7] = ((G.Player.Id & 1) << 5) + 0xF0;

                if (Goto00_ck(POS_VEC(G.Player.pTgt), 0x200) == 0)
                {
                    ScratchWord[7] = ((G.Player.Id & 1) << 5) + 0xC0;
                }

                if ((*(INT*)p & 0xFF0100) == 0x20000)  // pointer type conversion (no code)
                {
                    ScratchWord[7] += 0x20;
                }
                Goto00(POS_VEC(G.Player.pTgt), ScratchWord[7]);
            }
        }

        G.Player.Routine_3 += Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);

        x = G.SaveData.Key;
        if (x & 2)
        {
            G.Player.Cdir_y = (SHORT)(G.Player.Cdir_y + 0x48 - (G.Player.Id & 1) * 0x10);  // sign-extends the halfword
        }
        if (x & 8)
        {
            y = G.Player.Cdir_y - 0x48;
            G.Player.Cdir_y = y + (G.Player.Id & 1) * 0x10;
        }

        if (G.Player.Type != 0xA && ((!(G.Player.Id & 1) && G.Player.Move_cnt >= 5 && (G.Player.Type == 2 || G.Player.Type - 4 < 2U)) || G.Player.Move_cnt >= 0xA))
        {
            if (G.SaveData.Key & KEY_DOWN)
            {
                G.Player.Routine_2 = 0x15;
                G.Player.Move_no = 0xB;
                G.Player.Aim_flg &= 0x1F;
                G.Player.Routine_3 = 0;

                PL_BE_FLG_CUR &= EM_BE_STATE_MASK;

                G.Player.Aim_flg |= 0x20;
                return;
            }

            if (G.SaveData.Key & KEY_UP)
            {
                G.Player.Routine_2 = 0x15;
                G.Player.Move_no = 8;
                G.Player.Aim_flg &= 0x1F;
                G.Player.Routine_3 = 0;

                PL_BE_FLG_CUR &= EM_BE_STATE_MASK;

                G.Player.Aim_flg |= 0x80;
                return;
            }
        }
        break;
    case 2:
        PL_ROUTINE_HALF2 = 0x13;
        break;
    }
}

VOID Pl_gun_raise(VOID)
{
    UINT x;
    INT y;
    INT t;

    switch (G_PLAYER->Routine_3)
    {
    case 0:
        x = G.Player.Aim_flg;
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 3;
        G.Player.Spd_cur = 0;
        t = x >> 7;
        y = t * 2;
        y += t;
        y += 7;
        x &= 0x20;
        x = x != 0;
        y += x * 6;
        G.Player.Move_no = y;
    case 1:
        Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
        break;
    case 2:
        if (G.Player.Hokan_flg == 0)
        {
            y = G.Player.Type;
            G.Player.Routine_3 = 3;
            G.Player.Hokan_flg = 7;
            G.Player.Move_wait = 0;
            G.Player.Move_cnt = 0;
            G.Player.Spd_cur = 1;
            G.Player.Move_no = (y - 1) * 3 + 5 + (G.Player.Aim_flg >> 7) + ((G.Player.Aim_flg & 0x20) >> 4);
            if (G.Status_flg & STAT_WATER)
            {
                Snd_se_walk(0, 0);
            }
        }
        else
        {
            Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x200);
        }
        break;
    case 3:
        if (G.Player.Hokan_flg == 0)
        {
            x = G.Player.Aim_flg;
            G.Player.Routine_3 = 2;
            G.Player.Hokan_flg = 7;
            G.Player.Move_cnt = 0;
            G.Player.Move_wait = 0;
            G.Player.Spd_cur = 1;
            t = x >> 7;
            y = t * 2;
            y += t;
            y += 7;
            x &= 0x20;
            x = x != 0;
            y += x * 6;
            G.Player.Move_no = y;
        }
        else
        {
            Joint_move(0, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x200);
        }
        break;
    }
}

VOID Pl_gun_hold(VOID)
{
    UCHAR old;

    UCHAR cur;
    USHORT pad;
    INT sel;
    INT prev;
    INT off;
    INT t;
    INT v;
    INT h;

    old = G_PLAYER->Aim_flg;
    pad = G.SaveData.Key;
    ScratchWord[7] = old;

    G.Player.Aim_flg = (old & 0x1F) | 0x40;

    if (pad & 0x10)
    {
        G.Player.Aim_flg = (G.Player.Aim_flg & 0x1F) | 0x80;
    }
    if (pad & 0x20)
    {
        G.Player.Aim_flg = (G.Player.Aim_flg & 0x1F) | 0x20;
    }
    cur = G.Player.Aim_flg;
    sel = cur & 0xA0;
    if ((old & 0xA0) != sel)
    {
        do
        {
            G.Player.Routine_2 = 0x15;
        } while (0);
        h = cur >> 7;
        v = h * 2;
        v += h;
        v += 5;
        t = cur & 0x20;
        t = t != 0;
        v += t * 6;
        G_PLAYER->Routine_3 = 0;
        G.Player.Move_no = v;
    }
    if ((old & 0x40) && sel != 0)
    {
        do
        {
            G.Player.Routine_2 = 0x15;
        } while (0);
        h = cur >> 7;
        v = h * 2;
        v += h;
        v += 5;
        t = cur & 0x20;
        t = t != 0;
        v += t * 6;
        G_PLAYER->Routine_3 = 0;
        G.Player.Move_no = v;
    }
    prev = ScratchWord[7];
    if ((prev & 0xA0) && (G.Player.Aim_flg & 0x40))
    {
        G.Player.Routine_2 = 0x16;
        G.Player.Routine_3 = 0;
        PL_MOVE_NO_UBYTE = ((prev & 0x80) >> 7) * 3 + 5 + ((prev & 0x20) >> 5) * 6;
    }
    if (!(G.SaveData.Key & KEY_AIM))
    {
        G.Player.Routine_2 = 0x17;
        G.Player.Routine_3 = 0;
        return;
    }
    if (G.SaveData.Key & 0x40)
    {
        if (Tama_ck_i())
        {
            G.Player.Routine_2 = 0x14;
            G.Player.Routine_3 = -(G.Player.Type == 6) & 3;

            off = (G.Player.Type - 2) * 10;
            Pad_vib_set0(*BYTE_OFS(SHORT, &Pl_vib_tbl[0].duration, off), 2);  // SHORT view of the field (plain access changes the code)
            off = (G.Player.Type - 2) * 10;
            Pad_vib_set1(*BYTE_OFS(SHORT, &Pl_vib_tbl[0].duration, off), 0xFA, 2);  // SHORT view of the field (plain access changes the code)
            off = (G.Player.Type - 2) * 10;
            Pad_vib_fade_set(*BYTE_OFS(SHORT, &Pl_vib_tbl[0].Fade_len, off), *BYTE_OFS(SHORT, &Pl_vib_tbl[0].ramp_from, off),  // SHORT view of the field (plain access changes the code)
                *BYTE_OFS(SHORT, &Pl_vib_tbl[0].ramp_to, off), *BYTE_OFS(SHORT, &Pl_vib_tbl[0].duration, off) + 2);  // SHORT view of the field (plain access changes the code)

            return;
        }
        if (G.SaveData.Key_trg & 0x40)
        {
            if (Flg_ck(&MAIN.SaveData.Scenario_flg, 0x7A) == 0 || pItem_work_base[G.SaveData.Equip_no - 1].Id != 5)  // address as integer
            {
                Snd_se_stad(1, 9, 0, &MAIN.Player.Pos_x);  // address as integer
            }

            if (Tama_reload_ck() && G.Player.Type < 6)
            {
                PL_ROUTINE_UHALF2 = 0x18;

                return;
            }
        }
    }

    if ((G.Padd1_trg & PAD_L1) && pl_mv05_sub() != 0)
    {
        PL_ROUTINE_UHALF2 = 0x1A;
        return;
    }

    if (G.SaveData.Key & KEY_RIGHT)
    {
        {
            INT dir = G.Player.Cdir_y + 0x48;
            G.Player.Cdir_y = dir - (G.Player.Id & 1) * 16;
        }

        if (G.Player.Routine_3 < 2)
        {
            G.Player.Routine_3 = 2;
            G.Player.Hokan_flg = 0;
        }
    }
    else if (G.SaveData.Key & KEY_LEFT)
    {
        {
            INT dir = G.Player.Cdir_y - 0x48;
            G.Player.Cdir_y = dir + (G.Player.Id & 1) * 16;
        }

        if (G.Player.Routine_3 < 2)
        {
            G.Player.Routine_3 = 2;
            G.Player.Hokan_flg = 0;
        }
    }
    else if (G.Player.Routine_3 >= 2)
    {
        G.Player.Routine_3 = 0;
    }
}

VOID Pl_gun_fire(VOID)
{
    Pl_fire_tbl[G.Player.Routine_3]();
}

VOID Pl_fire_start(VOID)
{
    INT Low_flg;

    INT y;

    G.Player.Routine_3 = 1;
    G.Player.Hokan_flg = 3;
    G.Player.Move_cnt = 0;
    G.Player.Spd_cur = 1;

    y = (G.Player.Aim_flg >> 7) * 3 + 6;
    do
    {
        Low_flg = G.Player.Aim_flg & 0x20;
    } while (0);
    Low_flg = Low_flg != 0;
    G_PLAYER->Move_no = y + Low_flg * 6;
    Pl_view_esp_set(Low_flg);

    if (G.Status_flg & STAT_WATER)
    {
        Snd_se_walk(1, 0);
    }
    Pl_fire_shoot();
}

VOID Pl_fire_shoot(VOID)
{
    SHORT Wep_no;
    INT k;
    INT b;
    INT r;
    UCHAR t;

    Wep_no = G.Player.Type - 2;
    G.Player.Routine_3 += Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
    if (G.Player.Move_cnt == 2)
    {
        Hand_ck();
    }
    if (G.Player.Move_cnt == Pl_flash_tbl[Wep_no].frame)
    {
        pItem_work_base[G.SaveData.Equip_no - 1].Num--;
        if (Wep_no == 8)
        {
            ScratchWord[3] = (Tama_ck_i() & 3) + 8;
            ScratchVec[0] = Pl_flash_tbl[ScratchWord[3]].Ofs_x;
            ScratchPad->Vec.vy = Pl_flash_tbl[ScratchWord[3]].off_y;
            ScratchPad->Vec.vz = Pl_flash_tbl[ScratchWord[3]].off_z;
            Esp_call(Pl_flash_tbl[ScratchWord[3]].fx_type, Pl_flash_tbl[ScratchWord[3]].Esp_dt, 0, &G.Player.pSin_parts_ptr[14].Workm, &ScratchPad->Vec, 0);
        }
        else
        {
            ScratchVec[0] = Pl_flash_tbl[Wep_no].Ofs_x;
            ScratchPad->Vec.vy = Pl_flash_tbl[Wep_no].off_y;
            ScratchPad->Vec.vz = Pl_flash_tbl[Wep_no].off_z;
            Esp_call(Pl_flash_tbl[Wep_no].fx_type, Pl_flash_tbl[Wep_no].Esp_dt, 0, &G.Player.pSin_parts_ptr[14].Workm, &ScratchPad->Vec, 0);
            if (Wep_no == 2)
            {
                ScratchPad->Vec.vx = 0x96;
                ScratchPad->Vec.vy = 0x17C;
                ScratchPad->Vec.vz = 0;
                Esp_call(0x11, 3, 0, &G.Player.pSin_parts_ptr[14].Workm, &ScratchPad->Vec, 0);
            }
            if (Wep_no == 3)
            {
                ScratchPad->Vec.vx = 0x96;
                ScratchPad->Vec.vy = 0x17C;
                ScratchPad->Vec.vz = 0;
                Esp_call(0x11, 0xB, 0, &G.Player.pSin_parts_ptr[14].Workm, &ScratchPad->Vec, 0);
            }
        }
    }
    if (G.Player.Move_cnt == Pl_fire_hit_tbl[Wep_no].frame)
    {
        if (Wep_no < 6)
        {
            Weapon_at_ck(Pl_fire_hit_tbl[Wep_no].hit_type);
        }
        Snd_se_stad(1, Pl_fire_hit_tbl[Wep_no].Se_no1, 0, &MAIN.Player.Pos_x);  // address as integer
        Snd_se_stad(1, Pl_fire_hit_tbl[Wep_no].Se_no2, 0, &MAIN.Player.Pos_x);  // address as integer
    }
    if (G.Player.Move_cnt == Pl_shell_tbl[Wep_no].frame)
    {
        b = G.Player.Id & 1;
        ScratchVec[0] = Pl_shell_tbl[Wep_no].Ofs_x;
        ScratchPad->Vec.vy = Pl_shell_tbl[Wep_no].off_y + b * (1 - Wep_no) * 300;
        ScratchPad->Vec.vz = Pl_shell_tbl[Wep_no].off_z;
        if (Wep_no == 8)
        {
            ScratchPad->Vec.vx = Pl_shell_tbl[8].Ofs_x;
            ScratchPad->Vec.vy = Pl_shell_tbl[8].off_y + b * 500;
            ScratchPad->Vec.vz = Pl_shell_tbl[8].off_z;
        }
        r = Esp_call(Pl_shell_tbl[Wep_no].fx_type, Pl_shell_tbl[Wep_no].Esp_dt, -(Wep_no != 8) & 0x555, MATRIX_OF(&MAIN.Player), &ScratchPad->Vec, 0);
        ScratchWord[0] = r;

        if (Wep_no == 8)
        {
            MAIN.Esp[r].Free0 = Wep_no;
        }
        else
        {
            MAIN.Esp[r].Free3 = Wep_no;
        }
    }
    if (G.Player.Move_cnt == Pl_flash2_tbl[Wep_no].frame)
    {
        if (Wep_no == 8)
        {
            ScratchWord[3] = (Tama_ck_i() & 3) + 8;
            ScratchVec[0] = Pl_flash2_tbl[ScratchWord[3]].Ofs_x;
            ScratchPad->Vec.vy = Pl_flash2_tbl[ScratchWord[3]].off_y;
            ScratchPad->Vec.vz = Pl_flash2_tbl[ScratchWord[3]].off_z;
            ScratchWord[4] = Esp_call(Pl_flash2_tbl[ScratchWord[3]].fx_type, Pl_flash2_tbl[ScratchWord[3]].Esp_dt, 0, &G.Player.pSin_parts_ptr[14].Workm, &ScratchPad->Vec, 0);
            MAIN.Esp[ScratchWord[4]].Free0 = Wep_no;
        }
        else
        {
            ScratchVec[0] = Pl_flash2_tbl[Wep_no].Ofs_x;
            ScratchPad->Vec.vy = Pl_flash2_tbl[Wep_no].off_y;
            ScratchPad->Vec.vz = Pl_flash2_tbl[Wep_no].off_z;
            ScratchWord[3] = Esp_call(Pl_flash2_tbl[Wep_no].fx_type, Pl_flash2_tbl[Wep_no].Esp_dt, 0, &G.Player.pSin_parts_ptr[14].Workm, &ScratchPad->Vec, 0);
            MAIN.Esp[ScratchWord[3]].Free0 = Wep_no;
        }
    }
    if (Wep_no == 1 && (G.Player.Move_cnt == 7 || G.Player.Move_cnt == 9))
    {
        Weapon_at_ck(Pl_fire_hit_tbl[1].hit_type);
    }
    if (G.Player.Move_cnt > Pl_fire_end_tbl[Wep_no] && !(G.SaveData.Key & KEY_AIM))
    {
        PL_ROUTINE_HALF2 = 0x13;  // SHORT view of the field (plain access changes the code)
    }
    if (G.Player.Move_cnt == Pl_fire_end_tbl[Wep_no] - 2 && Wep_no == 1)
    {
        Snd_se_stad(1, 0xB, 0, &MAIN.Player.Pos_x);  // address as integer
    }
    if (G.Player.Move_cnt > Pl_fire_end_tbl[Wep_no] && Wep_no == 1)
    {
        t = G.Player.Aim_flg;
        if (((t & 0xA0) && !(G.SaveData.Key & (KEY_UP | KEY_DOWN))) || ((t & 0x80) && !(G.SaveData.Key & KEY_UP)) || ((t & 0x20) && !(G.SaveData.Key & KEY_DOWN)))
        {
            PL_ROUTINE_HALF2 = 0x13;  // SHORT view of the field (plain access changes the code)
            G.Player.Aim_flg = (G.Player.Aim_flg & 0x1F) | 0x40;
        }
    }
}

VOID Pl_fire_end_click(VOID)
{
    PL_ROUTINE_HALF2 = 0x13;  // SHORT view of the field (plain access changes the code)

    if ((&pItem_work_base[EQUIP_NO_CUR])[-1].Num == 0 && (Flg_ck(&MAIN.SaveData.Scenario_flg, 0x7A) == 0 || (&pItem_work_base[MAIN.SaveData.Equip_no])[-1].Id != 5))  // address as integer
    {
        Snd_se_stad(1, 9, 0, &MAIN.Player.Pos_x);  // address as integer
    }
}

VOID Pl_fire_reraise_start(VOID)
{
    INT Low_flg;

    G.Player.Routine_3 = 4;
    G.Player.Hokan_flg = 3;
    G.Player.Move_cnt = 0;
    G.Player.Move_wait = 0;
    G.Player.Spd_cur = 1;
    {
        INT low_mask = 0x20;
        PL_MOVE_NO_UBYTE = (G.Player.Aim_flg >> 7) * 3 + 6 + (Low_flg = (G.Player.Aim_flg & low_mask) != 0) * 6;  // UCHAR view of the field (plain access changes the code)
    }
    Pl_fire_reraise(Low_flg);
}

VOID Pl_fire_reraise(VOID)
{
    G.Player.Routine_3 += Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
}

VOID Pl_fire_hold_start(VOID)
{
    G.Player.Routine_3 = 6;
    G.Player.Move_cnt = 0;
    G.Player.Move_wait = 0;
    G.Player.Hokan_flg = 3;
    G.Player.Move_no = (G.Player.Aim_flg >> 7) + (((G.Player.Aim_flg >> 4) & 2) + 0xE);
    Pl_fire_hold();
}

VOID Pl_fire_hold(VOID)
{
    USHORT pad;
    INT old;
    UINT cur;
    INT t;
    INT a;
    INT b;

    pad = G.SaveData.Key;
    ScratchWord[7] = G.Player.Aim_flg;
    G.Player.Aim_flg = (G.Player.Aim_flg & 0x1F) | 0x40;
    if (pad & 0x10)
    {
        G.Player.Aim_flg = (G.Player.Aim_flg & 0x1F) | 0x80;
    }
    if (pad & 0x20)
    {
        G.Player.Aim_flg = (G.Player.Aim_flg & 0x1F) | 0x20;
    }
    if (Tama_ck_i() != 0)
    {
        if (G.Player.Move_cnt % 15 == 0)
        {
            Snd_se_stad(1, 3, 0, &MAIN.Player.Pos_x);
            Snd_se_stad(1, 4, 0, &MAIN.Player.Pos_x);

            Pad_vib_fade_set(0xC, 0xC8, 0x32, 0);
        }
        if (!(G.Player.Move_cnt & 3))
        {
            ScratchVec[0] = 0x21C;
            ScratchPad->Vec.vy = 0x4EC;
            ScratchPad->Vec.vz = 0;
            Esp_call(0xC, 0, 0, &G.Player.pSin_parts_ptr[14].Workm, &ScratchPad->Vec, 0);
        }
        pItem_work_base[G.SaveData.Equip_no - 1].Num--;
    }
    else
    {
        Snd_se_stad(1, 3, 0, &MAIN.Player.Pos_x);
        PL_ROUTINE_HALF2 = 0x13;  // SHORT view of the field (plain access changes the code)
        return;
    }
    if ((ScratchPad->tmp1C & 0x40) && ((cur = G.Player.Aim_flg) & 0xA0)) // plain ScratchWord[7] changes scheduling
    {
        goto set;
    }
    else if (
        ((old = ScratchWord[7]) & 0xA0) && ((cur = G.Player.Aim_flg) & 0x40))
    {
        G.Player.Routine_3 = 8;
        {
            INT base = ((old & 0x80) >> 7) * 3 + 5;
            a = base + ((old & 0x20) >> 5) * 6;
        }
        b = cur | 1;
        goto store;
    }
    else if ((ScratchWord[7] & 0xA0) != ((cur = G.Player.Aim_flg) & 0xA0))
    {
    set:
        G.Player.Routine_3 = 8;
        {
            INT base = (cur >> 7) * 3 + 5;
            t = cur & 0x20;
            t = t != 0;
            a = base + t * 6;
        }
        b = cur & 0xFE;
    store:
        G.Player.Move_no = a;
        G.Player.Aim_flg = b;
    }
    else
    {
        Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
        if (!(G.SaveData.Key & 0x40))
        {
            PL_ROUTINE_HALF2 = 0x13;  // SHORT view of the field (plain access changes the code)
            return;
        }
        if (G.SaveData.Key & KEY_RIGHT)
        {
            G.Player.Cdir_y += 0x20;
        }
        else if (G.SaveData.Key & KEY_LEFT)
        {
            G.Player.Cdir_y -= 0x20;
        }
    }
}

VOID Pl_fire_nop(VOID)
{
}

VOID Pl_fire_reverse_start(VOID)
{
    G.Player.Routine_3 = 9;
    G.Player.Move_cnt = 0;
    G.Player.Move_wait = 0;
    G.Player.Hokan_flg = 3;
    Snd_se_stad(1, 6, 0, &MAIN.Player.Pos_x);  // address as integer
    Pl_fire_reverse();
}

VOID Pl_fire_reverse(VOID)
{
    G.Player.Routine_3 += Joint_move(G.Player.Aim_flg | 1, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
    if (G.SaveData.Key & (KEY_UP | KEY_DOWN))
    {
        G.Player.Routine_3 = 5;
    }
}

VOID Pl_fire_loop(VOID)
{
    G.Player.Routine_3 = 5;
}

VOID Pl_gun_aim_chg(VOID)
{
    switch (MAIN.Player.Routine_3)
    {
    case 0:
        G.Player.Routine_3 = 1;
        G.Player.Hokan_flg = 3;
        G.Player.Move_cnt = 0;
        G.Player.Damage_no = 0;
        G.Player.Spd_cur = 1;
        if (G.Status_flg & STAT_WATER)
        {
            Snd_se_walk(0, 0);
        }

    case 1:
        G.Player.Routine_3 += Joint_move(ScratchWord[7], G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400);
        if (G.SaveData.Key & (KEY_UP | KEY_DOWN))
        {
        case 2:
            PL_ROUTINE_HALF2 = 0x13;  // SHORT view of the field (plain access changes the code)
        }
        return;
    }
}

VOID Pl_gun_holster(VOID)
{
    if (G.SaveData.Key & KEY_RIGHT)
    {
        G.Player.Cdir_y += 0x50;
    }
    if (G.SaveData.Key & KEY_LEFT)
    {
        G.Player.Cdir_y -= 0x50;
    }
    if (G.Player.Routine_3 == 0)
    {
        G.Player.Routine_3 = 1;
        G.Player.Move_no = 5;
        G.Player.Hokan_flg = 3;
        G.Player.Spd_cur = 1;
        if (G.Status_flg & STAT_WATER)
        {
            Snd_se_walk(0, 0);
        }
    }
    if (Joint_move(1, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400) != 0)
    {
        G.Player.Routine_1 = 0;
        PL_ROUTINE_HALF2 = 0;  // SHORT view of the field (plain access changes the code)
        G.Player.Spd_cur = 0;
    }
}

VOID Pl_gun_reload(VOID)
{
#ifdef __INTELLISENSE__
    extern INT (*reload_fx_tbl_view[])();  // asm label alias unsupported by IntelliSense
#else
    extern INT (*reload_fx_tbl_view[])() __asm__("reload_fx_tbl");
#endif
    USHORT Upd_flg;
    UCHAR coll_flags;

    switch (MAIN.Player.Routine_3)
    {
    case 0:

        G.Player.Routine_3 = 1;
        G.Player.Move_no = 0xE;
        G.Player.Hokan_flg = 3;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Spd_cur = 1;

        G.Player.Sca_info |= 0x80;
        STOP_FLG_CUR &= ~STOP_ACTION;

        if (G.Status_flg & STAT_WATER)
        {
            Snd_se_walk(0, 0);
        }

    case 1:
        reload_fx_tbl_view[MAIN.Player.Type]();
        if (Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400) != 0)
        {
            MAIN_PL_ROUTINE_HALF2 = 0x13;
            coll_flags = G.Player.Sca_info & 0x7F;
            G.Player.Sca_info = coll_flags;

            Upd_flg = STOP_FLG_CUR | STOP_ACTION;

            MAIN.Stop_flg = Upd_flg;
            if (G.Player.Type >= 7U)
            {
                MAIN_PL_ROUTINE_HALF2 = 0x218;
                G.Player.Hokan_flg = 7;
                G.Player.Move_no = 7;
                G.Player.Sca_info = coll_flags | 0x80;
                G.Player.Move_cnt = 0;
                G.Player.Move_wait = 0;
                MAIN.Stop_flg = Upd_flg & ~STOP_ACTION;
                return;
            }
        }
        return;
    case 2:
        Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x200);
        if (G.Player.Hokan_flg == 0)
        {
            PL_ROUTINE_HALF2 = 0x13;  // SHORT view of the field (plain access changes the code)
            G.Player.Sca_info &= 0x7F;

            MAIN.Stop_flg = STOP_FLG_CUR | STOP_ACTION;
        }
        break;
    }
}

VOID Pl_reload_beretta(VOID)
{
    if ((PL_MOVE_WORD & 0xFFFF0000) == 0x010A0000 && (Tama_once_ck() & 0xFF))
    {
        Mag_set();
    }

    if ((PL_MOVE_WORD & 0xFFFF0000) == 0x01110000)
    {
        Tama_reload();
        Snd_se_stad(1, 5, 0, &MAIN.Player.Pos_x);  // address as integer
    }
}

VOID Pl_reload_shotgun(VOID)
{
    if ((G.Player.Move_cnt == 0xF || G.Player.Move_cnt == 0x19 || G.Player.Move_cnt == 0x23) && G.Player.Move_wait == 1)
    {
        Snd_se_stad(1, 9, 5, &MAIN.Player.Pos_x);  // address as integer
        if (G.Player.Move_cnt == 0xF)
        {
            Tama_reload();
        }
    }
}

VOID Pl_reload_colt(VOID)
{
    INT unused[6];    // unused local: reserves stack space

    if ((PL_MOVE_WORD & 0xFFFF0000) == 0x010C0000 && (Tama_once_ck() & 0xFF))
    {
        ScratchVec[0] = G.Player.pSin_parts_ptr[14].Workm.t[0];
        ScratchPad->Vec.vy = G.Player.pSin_parts_ptr[14].Workm.t[1];
        ScratchPad->Vec.vz = G.Player.pSin_parts_ptr[14].Workm.t[2];
        Esp_call(5, 4, 0, 0, &ScratchPad->Vec, 5);
    }
    if ((PL_MOVE_WORD & 0xFFFF0000) == 0x011C0000)
    {
        Tama_reload();
        Snd_se_stad(1, 9, 0xA, &MAIN.Player.Pos_x);  // address as integer
    }
}

VOID Pl_reload_def(VOID)
{
    if ((PL_MOVE_WORD & 0xFFFF0000) == 0x01120000)
    {
        Tama_reload();
        Snd_se_stad(1, 5, 0, &MAIN.Player.Pos_x);  // address as integer
    }
}

VOID Pl_gun_lock(VOID)
{
    INT* w;
    UCHAR c;
    INT v;
    INT h;
    INT t;

    if (G.Player.Routine_3 != 0 && (G.Padd1_trg & PAD_L1))
    {
        pl_mv05_sub();
    }
    switch (G_PLAYER->Routine_3)
    {
    case 0:
        G.Player.Spd_cur = 1;
        G.Player.Hokan_flg = 0;
        G.Player.Routine_3 = 1;

    case 1:
        if (MAIN.Player.Hokan_flg == 0)
        {
            G.Player.Routine_3 = 2;
            G.Player.Move_wait = 0;
            G.Player.Move_cnt = 0;
            G.Player.Hokan_flg = 7;
            G.Player.Move_no = (G.Player.Type - 1) * 3 + 5 + (G.Player.Aim_flg >> 7) + ((G.Player.Aim_flg & 0x20) >> 4);
            if (G.Status_flg & STAT_WATER)
            {
                Snd_se_walk(0, 0);
            }
            return;
        }
        Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x200);
        break;
    case 2:
        if (G.Player.Hokan_flg == 0)
        {
            c = G.Player.Aim_flg;
            G.Player.Routine_3 = 1;
            G.Player.Hokan_flg = 7;
            G.Player.Move_cnt = 0;
            G.Player.Move_wait = 0;
            h = c >> 7;
            v = h * 2;
            v += h;
            v += 7;
            t = c & 0x20;
            t = t != 0;
            v += t * 6;
            G.Player.Move_no = v;
            return;
        }
        Joint_move(0, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x200);
        break;
    }
    w = &ScratchWord[7];
    *w = ((G_PLAYER->Id & 1) << 5) + 0xF0;
    if (Goto00_ck(POS_VEC(G.Player.pTgt), 0x200) == 0)
    {
        *w = ((G.Player.Id & 1) << 5) + 0xC0;
    }
    Goto00(POS_VEC(MAIN.Player.pTgt), *w);
    if (Goto00_ck(POS_VEC(G.Player.pTgt), 0x20) == 0)
    {
        PL_ROUTINE_HALF2 = 0x13;  // SHORT view of the field (plain access changes the code)
    }
}

VOID Pl_gun_click(VOID)
{
    USHORT Time_cnt;

    if (MAIN.Player.Routine_3 == 0)
    {
        G.Player.Routine_3 = 1;
        G.Player.Timer0 = 0xF;
        Snd_se_stad(1, 9, 0, &MAIN.Player.Pos_x);  // address as integer
        if (G.Status_flg & STAT_WATER)
        {
            Snd_se_walk(0, 0);
        }
    }
    Time_cnt = MAIN.Player.Timer0;
    G.Player.Timer0 = Time_cnt - 1;
    if (Time_cnt == 0)
    {
        PL_ROUTINE_HALF2 = 0x13;  // SHORT view of the field (plain access changes the code)
    }
}

VOID Pl_knife_aim(VOID)
{
    USHORT angle;

    if (G_PLAYER->Routine_3 == 0)
    {
        G.Player.Routine_3 = 1;
        G.Player.Move_no = 5;
        G.Player.Hokan_flg = 3;
        G.Player.Move_cnt = 0;
        G.Player.Damage_no = 0;
        G.Player.Spd_cur = 1;
        G.Player.Timer0 = 0;

        if (Aim_auto_en != 0)
        {
            G.Player.Aim_sts = Em_ck();

            goto aim_ck;
        }
    }
    else
    {
    aim_ck:
        if (Aim_auto_en != 0)
        {
            G.Player.Aim_sts |= 2;

            if (G.Padd1 & PAD_L1)
            {
                G.Player.Aim_sts = 0;
            }
            if (G.Player.Aim_sts == 3)
            {
                ScratchWord[7] = ((G.Player.Id & 1) << 5) + 0xF0;

                if (Goto00_ck(POS_VEC(G.Player.pTgt), 0x200) == 0)
                {
                    ScratchWord[7] = ((G.Player.Id & 1) << 5) + 0xC0;
                }
                Goto00(POS_VEC(G.Player.pTgt), ScratchWord[7]);
            }
        }
    }

    if (Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400) != 0)
    {
        PL_ROUTINE_HALF2 = 0x13;  // SHORT view of the field (plain access changes the code)
        G.Player.Hokan_flg = 0;
    }

    Foot_set_pl(0);

    if (G.SaveData.Key & KEY_RIGHT)
    {
        angle = G.Player.Cdir_y + 0x20;
        G.Player.Cdir_y = angle;
    }
    else if (G.SaveData.Key & KEY_LEFT)
    {
        angle = G.Player.Cdir_y - 0x20;
        G.Player.Cdir_y = angle;
    }
}

VOID Pl_knife_hold(VOID)
{
    INT* pPl_flg;

    USHORT angle;
    UCHAR Aim_bak2;
    UCHAR Aim_bak3;
    UCHAR Aim_bak;

    Aim_bak = (PL_BE_FLG_CUR & EM_BE_STATE_MASK) | 0x40;
    ScratchWord[7] = MAIN.Player.Be_flg;
    MAIN.Player.Be_flg = Aim_bak;
    if (G.SaveData.Key & 0x10)
    {
        MAIN.Player.Be_flg = (Aim_bak & 0x1F) | 0x80;
    }
    if (G.SaveData.Key & 0x20)
    {
        MAIN.Player.Be_flg = (MAIN.Player.Be_flg & EM_BE_STATE_MASK) | 0x20;
    }
    G.Player.Aim_flg &= 0xFD;
    if (G.Player.Routine_3 == 0)
    {
        if (G.Player.Hokan_flg == 0)
        {
            G.Player.Routine_3 = 1;
            G.Player.Move_cnt = 0;
            G.Player.Move_wait = 0;
            G.Player.Hokan_flg = 0xF;
            Aim_bak2 = MAIN.Player.Be_flg;
            G.Player.Spd_cur = 1;
            G.Player.Move_no = ((Aim_bak2 & 0x80) >> 6) + (((Aim_bak2 & 0x20) >> 3) + 0xA);
            if (G.Status_flg & STAT_WATER)
            {
                Snd_se_walk(0, 0);
            }
        }
        Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x100);
    }

    pPl_flg = (INT*)&MAIN.Player.Be_flg;  // pointer type conversion (no code)

    if (MAIN.Player.Hokan_flg == 0)
    {
        G.Player.Routine_3 = 0;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 0xF;
        Aim_bak3 = *pPl_flg;
        G.Player.Spd_cur = 1;
        G.Player.Move_no = ((Aim_bak3 & 0x80) >> 6) + (((Aim_bak3 & 0x20) >> 3) + 9);
    }
    Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x100);

    Foot_set_pl(1);

    if (G.Player.Timer0 != 0)
    {
        G.Player.Timer0 -= 1;
    }
    if ((G.SaveData.Key & 0x40) && G.Player.Timer0 == 0)
    {
        PL_ROUTINE_HALF2 = 0x14;  // SHORT view of the field (plain access changes the code)
        G.Player.Timer0 = 0xA;
        return;
    }
    if (!(G.SaveData.Key & KEY_AIM))
    {
        PL_ROUTINE_HALF2 = 0x15;  // SHORT view of the field (plain access changes the code)

        return;
    }

    if (G.SaveData.Key & KEY_RIGHT)
    {
        angle = (SHORT)G.Player.Cdir_y + 0x48 - (G.Player.Id & 1) * 0x10;  // real width or sign conversion

        G.Player.Cdir_y = angle;
    }
    else if (G.SaveData.Key & KEY_LEFT)
    {
        angle = (SHORT)G.Player.Cdir_y - 0x48 + (G.Player.Id & 1) * 0x10;  // real width or sign conversion

        G.Player.Cdir_y = angle;
    }
}

VOID Pl_knife_swing(VOID)
{
    UCHAR tbl[6][2] = { { 6, 7 }, { 4, 2 }, { 4, 4 }, { 3, 8 }, { 4, 2 }, { 4, 4 } };

    UCHAR* pFlg;
    UCHAR* pFrm_wnd;
    INT* s;
    UCHAR Aim_bak;
    PARTS_W* pP_ptr;
    INT Anm_idx;

    pFlg = &MAIN.Player.Be_flg;
    if (MAIN.Player.Routine_3 == 0)
    {
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 3;
        Aim_bak = *pFlg;
        G.Player.Spd_cur = 1;
        G.Player.Move_no = (UCHAR)((Aim_bak >> 7) + (UCHAR)(((Aim_bak & 0x20) >> 4) + 6));  // real width or sign conversion
    }
    if (G.Player.Move_cnt == 7 && (G.pEm->Move_wait & 1))
    {
        Snd_se_stad(1, 0, 0, &MAIN.Player.Pos_x);  // address as integer
    }

    if ((G.Status_flg & STAT_WATER) && G.Player.Pos_y - Omodel_tbl[0]->Coord.t[1] > 1000)
    {
        if ((UCHAR)G.Player.Move_no == 7)  // real width or sign conversion
        {
            if (G.Player.Move_cnt < 3)
            {
                pP_ptr = G.Player.pSin_parts_ptr;
                ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
                ScratchPad->Vec.vx = 0x96;
                Esp_call(0x17, 8, 0, &pP_ptr[14].Workm, &ScratchPad->Vec, 0);
            }
            if (G.Player.Move_cnt == 0 && G.pEm->Move_wait == 0)
            {
                Snd_se_walk(1, 0);
            }
        }
        else
        {
            if (!(G.Player.Move_cnt & 1))
            {
                pP_ptr = G.Player.pSin_parts_ptr;
                ScratchPad->Vec = MATRIX_POS(Monitor.pId_mat);
                ScratchPad->Vec.vx = 0x96;
                Esp_call(0x17, 8, 0, &pP_ptr[14].Workm, &ScratchPad->Vec, 0);
            }
            if (G.Player.Move_cnt == 0 && G.pEm->Move_wait == 0)
            {
                Snd_se_stad(1, 2, 0, &MAIN.Player.Pos_x);  // address as integer
            }
        }
    }
    Anm_idx = (UCHAR)MAIN.Player.Move_no - 6;  // real width or sign conversion
    G.Player.Damage_no = 0;

    pFrm_wnd = tbl[(G.Player.Id & 1) * 3 + Anm_idx];

    if ((UCHAR)(G.Player.Move_cnt - pFrm_wnd[0]) < pFrm_wnd[1])  // real width or sign conversion
    {
        if (!(G.Player.Aim_flg & 2))
        {
            if (Weapon_at_ck(1) != 0)
            {
                Pad_vib_set1(5, 0x96, 0);

                if ((UCHAR)G.Player.Move_no != 6 || G.Enemy[0].Id == 8 || G.Enemy[0].Id == 0xD || G.Enemy[1].Id == 0x13)  // real width or sign conversion
                {
                    MAIN.Player.Aim_flg |= 2;
                }
            }
        }
    }
    if (Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400) != 0)
    {
        PL_ROUTINE_HALF2 = 0x13;  // SHORT view of the field (plain access changes the code)
    }

    Foot_set_pl(0);
}

VOID Pl_knife_holster(VOID)
{
    if (G.SaveData.Key & KEY_RIGHT)
    {
        G.Player.Cdir_y += 0x50;
    }
    if (G.SaveData.Key & KEY_LEFT)
    {
        G.Player.Cdir_y -= 0x50;
    }
    if (G.Player.Routine_3 == 0)
    {
        G.Player.Routine_3 = 1;
        G.Player.Move_no = 5;
        G.Player.Hokan_flg = 3;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Spd_cur = 1;
    }
    if (Joint_move(1, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x400) != 0)
    {
        G.Player.Routine_1 = 0;
        PL_ROUTINE_HALF2 = 0;  // SHORT view of the field (plain access changes the code)
    }

    Foot_set_pl(0);
}

VOID Pl_knife_turn(VOID)
{
    INT* pTmp;
    UCHAR* p;
    UCHAR* q;

    if (G.Player.Routine_3 != 0 && (G.Padd1_trg & PAD_L1))
    {
        pl_mv05_sub();
    }
    p = &MAIN.Player.Be_flg;
    switch (G_PLAYER->Routine_3)
    {
    case 0:
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Hokan_flg = 0xF;
        G.Player.Spd_cur = 1;
        G.Player.Move_no = ((*p & 0x80) >> 6) + (((*p & 0x20U) >> 3) + 9);

    case 1:
        q = &MAIN.Player.Be_flg;
        if (MAIN.Player.Hokan_flg == 0)
        {
            G.Player.Routine_3 = 2;
            G.Player.Move_cnt = 0;
            G.Player.Move_wait = 0;
            G.Player.Hokan_flg = 0xF;
            G.Player.Move_no = ((*q & 0x80) >> 6) + (((*q & 0x20U) >> 3) + 0xA);
            if (G.Status_flg & STAT_WATER)
            {
                Snd_se_walk(0, 0);
            }
        }
        Joint_move(0, G_PLAYER->pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x100);
        Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x100);
        break;
    case 2:
        if (G.Player.Hokan_flg == 0)
        {
            G.Player.Routine_3 = 1;
            G.Player.Move_cnt = 0;
            G.Player.Move_wait = 0;
            G.Player.Hokan_flg = 0xF;
            G.Player.Move_no = ((*p & 0x80) >> 6) + (((*p & 0x20U) >> 3) + 9);
        }
        Joint_move(0, G.Player.pSub0_kan_t_ptr, G.Player.pSub0_seq_t_ptr, 0x100);
        break;
    }
    pTmp = &ScratchWord[7];
    *pTmp = ((G_PLAYER->Id & 1) << 5) + 0xF0;
    if (Goto00_ck(POS_VEC(G.Player.pTgt), 0x200) == 0)
    {
        *pTmp = ((G.Player.Id & 1) << 5) + 0xC0;
    }
    Goto00(POS_VEC(G.Player.pTgt), *pTmp);
    if (Goto00_ck(POS_VEC(G.Player.pTgt), 0x20) == 0)
    {
        PL_ROUTINE_HALF2 = 0x13;  // SHORT view of the field (plain access changes the code)
    }
}

VOID pl_die00(VOID)
{
    PL_DIE_REC t[2] = { { 25, 0 }, { 15, 0x400 } };
    PL_DIE_REC* q;
    USHORT v;

    switch (MAIN.Player.Routine_3)
    {
    case 0:
        {
            v = t[G.Player.Id & 1].speed;
        }
        G.Player.Routine_3 = 1;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 4;
        G.Player.Spd_cur = v;
        Snd_se_stad(3, 3, 0, &MAIN.Player.Pos_x);
        G.Player.Timer0 = 0xB4;
        G.Player.Damage_no = 1;

        G.Player.Hokan_flg = 0;

        STOP_FLG_CUR &= ~STOP_ACTION;

        Pad_vib_set0(0xF, 0);
        Pad_vib_fade_set(0x14, 0, 0xFA, 0);
        Pad_vib_fade_set(0x50, 0xFA, 0, 0x14);

    case 1:
        if ((PL_MOVE_WORD & 0xFFFF0000) == 0x01190000)
        {
            Snd_se_walk(2, 0);
        }
        G.Player.Routine_3 += Joint_move(0, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x400);
        q = &t[G.Player.Id & 1];
        Add_speedXZ(q->push);
        break;
    case 2:

        if (STAGE_ROOM != 0x501 && STAGE_ROOM != 0x903 && STAGE_ROOM != 0x704)
        {
            Kage_work_color_set(&EM_CUR->Kage_work, 1, 2, 0xFFFF50);
            Kage_work_size_add(&MAIN.pEm->Kage_work, -0x64, -0x64);
            G.Player.Routine_3 = 3;
            G.Player.Damage_no = 0x80;
            break;
        }
        G.Player.Routine_0 = 4;

        break;
    case 3:
        Kage_work_size_add(&G.pEm->Kage_work, 0x10, 0x10);
        if (--G.Player.Timer0 == 0x20)
        {
            G.Player.Routine_0 = 4;
        }
        break;
    }
}

VOID Wep_joint_set(INT joint_idx)
{
    PARTS_W* pP_ptr;

    pP_ptr = G_PEM_BYTE(ENEMY_WORK)->pSin_parts_ptr;  // G_PEM_BYTE: a plain access changes the code

    pP_ptr = &pP_ptr[joint_idx & 0xFF];
    EM_NOW->pParts_tmd = pP_ptr->pTmd;
    EM_NOW->pParts_packet = pP_ptr->pPacket;
}

VOID Foot_set_pl(UCHAR R_no)
{
    INT dx;
    PARTS_W* pP_ptr;
    INT n;
    PARTS_W* pP_ptr1;
    ENEMY_WORK* pEm;

    pEm = EM_CUR;
    n = 3;
    pP_ptr = pEm->pSin_parts_ptr;
    pP_ptr1 = pP_ptr;
    RotMatrix((SVECTOR*)&pEm->Cdir_x, (MATRIX*)pEm->M);  // pointer type conversion (no code)
    GsMulCoord0((MATRIX*)MAIN.pEm->M, &pP_ptr->Coord, &ScratchPad->Mat);  // pointer type conversion (no code)
    GsMulCoord3(&ScratchPad->Mat, &pP_ptr[2].Coord);
    pP_ptr1 = &pP_ptr1[R_no * 3 + 5];
    do
    {
        GsMulCoord3(&ScratchPad->Mat, &(pP_ptr1 - (n & 0xFF))[1].Coord);
        n -= 1;
    } while (n & 0xFF);
    dx = ScratchPad->Mat.t[0] - pP_ptr1->Workm.t[0];
    ScratchPad->Mat.t[1] = 0;
    ScratchPad->Mat.t[0] = dx;
    ScratchPad->Mat.t[2] -= pP_ptr1->Workm.t[2];
    G_PEM_BYTE(ENEMY_WORK)->Pos_x -= dx;  // G_PEM_BYTE: a plain access changes the code
    EM_NOW->Pos_z -= ScratchPad->Mat.t[2];
}

VOID Rot_neck(VOID)
{
    PARTS_W* pP_ptr;
    UCHAR f;
    UCHAR f3;
    UCHAR f2;
    INT v;
    SHORT ang;
    SHORT cur;
    SHORT cur2;
    UCHAR Add_dir_y;
    UCHAR Add_dir_z;
    SHORT Y;
    INT X;
    INT Z;
    USHORT R;
    SHORT b;
    f = EM_CUR->Neck_flg;
    if (!(f & 0x10))
    {
        return;
    }
    if (f & 0x40)
    {
        v = MAIN.pEm->Neck_point_z;
        if (!(v & 0x10000))
        {
            MAIN.pEm->Neck_point_z = (v << 1) | 0x10000;
            MAIN.pEm->Neck_flg |= 0x20;
        }
        f2 = MAIN.pEm->Neck_flg;
        if ((f2 & 0x40) && (USHORT)MAIN.pEm->Neck_point_z == 0)  // truncates to an unsigned halfword
        {
            MAIN.pEm->Neck_flg = f2 & 0x1C;
        }
    }
    if (EM_CUR->Neck_flg & 0x80)
    {
        MAIN.pEm->Neck_point_x = MAIN.pEm->pTgt->Pos_x;
        MAIN.pEm->Neck_point_y = MAIN.pEm->pTgt->Pos_y - 0xA28;
        MAIN.pEm->Neck_point_z = MAIN.pEm->pTgt->Pos_z;
    }
    ang = MAIN.pEm->Neck_point_x * (MAIN.pEm->Neck_flg & 1);
    pP_ptr = MAIN.pEm->pSin_parts_ptr;
    pP_ptr = &pP_ptr[MAIN.pEm->Neck_no];
    Add_dir_y = MAIN.pEm->Neck_spd_y;
    Add_dir_z = MAIN.pEm->Neck_spd_x;
    if (!(MAIN.pEm->Neck_flg & 0x20))
    {
        v = Get_dir_tgt(MAIN.pEm->Neck_point_x, MAIN.pEm->Neck_point_z);
        ang = ((v - MAIN.pEm->Cdir_y) & 0xFFF) * (MAIN.pEm->Neck_flg & 1);
    }
    cur = pP_ptr->Free3;
    if (((ang - cur + Add_dir_y) & 0xFFF) < Add_dir_y * 2)
    {
        Y = cur;

        pP_ptr->Free3 = ang;

        if (((ang + 0x2C8) & 0xFFF) >= 0x591)
        {
            pP_ptr->Free3 = Y;
        }
        if ((EM_CUR->Neck_flg & 0x40) && (EM_CUR->Neck_flg & 1))
        {
            MAIN.pEm->Neck_point_x = 0x1000 - MAIN.pEm->Neck_point_x;
            MAIN.pEm->Neck_point_z -= 1;
        }
    }
    else if (ang == 0)
    {
        if ((cur & 0xFFF) > 0x800)
        {
            pP_ptr->Free3 = (Add_dir_y + cur) & 0xFFF;
        }
        else
        {
            pP_ptr->Free3 = (cur - Add_dir_y) & 0xFFF;
        }
    }
    else if ((SHORT)((ang - cur) & 0xFFF) < 0x800)  // real width or sign conversion
    {
        pP_ptr->Free3 = (Add_dir_y + cur) & 0xFFF;

        if (((pP_ptr->Free3 - 0x2C8) & 0xFFF) < 0xA70)
        {
            pP_ptr->Free3 = 0x2C8;
        }
    }
    else
    {
        pP_ptr->Free3 = (cur - Add_dir_y) & 0xFFF;
        if (((pP_ptr->Free3 + 0x2C8) & 0xFFF) >= 0x591)
        {
            pP_ptr->Free3 = 0xD38;
        }
    }
    f3 = EM_CUR->Neck_flg;
    {
        INT bit = 2;
        INT look = (f3 & bit) != 0;
        ang = MAIN.pEm->Neck_point_y * look;
    }
    if (!(f3 & 0x20))
    {
        X = MAIN.pEm->Neck_point_x - pP_ptr->Workm.t[0];
        Z = MAIN.pEm->Neck_point_z - pP_ptr->Workm.t[2];
        v = SquareRoot0(X * X + Z * Z);
        Y = MAIN.pEm->Neck_point_y - pP_ptr->Workm.t[1];
        {
            INT bit = 2;
            b = (MAIN.pEm->Neck_flg & bit) != 0;
        }

        R = v;

        ang = b * (Y > 0 ? 0xC00 : 0x400);

        if (R != 0)
        {
            {
                INT bit = 2;
                INT dir = catan((Y << 12) / R);
                INT look = (G.pEm->Neck_flg & bit) != 0;
                ang = dir * look;
            }
        }
    }
    cur2 = pP_ptr->Free4;
    if (((ang - cur2 + Add_dir_z) & 0xFFF) < Add_dir_z * 2)
    {
        pP_ptr->Free4 = ang;
        if (((ang + 0x138) & 0xFFF) >= 0x271)
        {
            pP_ptr->Free4 = cur2;
        }
        if ((VIEW(INT, EM_CUR->Neck_flg) & 0x41) == 0x40)  // INT view of the field (plain access changes the code)
        {
            MAIN.pEm->Neck_flg ^= 2;
            MAIN.pEm->Neck_point_z -= 1;
        }
    }
    else if (((cur2 - ang) & 0xFFF) > 0x800)
    {
        pP_ptr->Free4 = (Add_dir_z + cur2) & 0xFFF;

        if (((pP_ptr->Free4 - 0x138) & 0xFFF) < 0x6C8)
        {
            pP_ptr->Free4 -= Add_dir_z;
        }
    }
    else
    {
        pP_ptr->Free4 = (cur2 - Add_dir_z) & 0xFFF;
        if (((pP_ptr->Free4 + 0x138) & 0xFFF) >= 0x939)
        {
            pP_ptr->Free4 += Add_dir_z;
        }
    }
}

VOID Pl_neck(VOID)
{
    PARTS_W* pP_ptr;

    if (EM_NOW->Neck_flg != 0)
    {
        do
        {
            pP_ptr = EM_NOW->pSin_parts_ptr;
            pP_ptr = &pP_ptr[EM_NOW->Neck_no];  // real width or sign conversion
            ScratchPad->Sv.vx = 0;
            ScratchPad->Sv.vy = pP_ptr->Free3;
            ScratchPad->Sv.vz = pP_ptr->Free4;
            ScratchPad->Mat = GsIDMATRIX;
            RotMatrixYXZ(&ScratchPad->Sv, &ScratchPad->Mat);
        } while (0);
        pP_ptr = (PARTS_W*)&pP_ptr->Workm;  // pointer type conversion (no code)
        CompMatrix((MATRIX*)pP_ptr, &ScratchPad->Mat, (MATRIX*)pP_ptr);  // pointer type conversion (no code)
    }
}

VOID Hand_ck(VOID)
{
    SHORT lo;
    SHORT hi;
    INT Chr_no;
    INT unused1;    // unused local: reserves stack space
    PARTS_W* pP_ptr;
    INT Rsv2;    // unused local: reserves stack space
    INT Rsv3;    // unused local: reserves stack space
    ENEMY_WORK* pEm;

    pEm = EM_CUR;
    pP_ptr = pEm->pSin_parts_ptr;
    RotMatrix((SVECTOR*)&pEm->Cdir_x, (MATRIX*)pEm->M);  // pointer type conversion (no code)
    GsMulCoord0((MATRIX*)MAIN.pEm->M, &pP_ptr->Coord, &ScratchPad->Mat);  // pointer type conversion (no code)
    GsMulCoord3(&ScratchPad->Mat, &pP_ptr[9].Coord);
    GsMulCoord3(&ScratchPad->Mat, &pP_ptr[10].Coord);
    GsMulCoord3(&ScratchPad->Mat, &pP_ptr[11].Coord);
    MAIN.Player.Be_flg = (PL_BE_FLG_CUR & EM_BE_STATE_MASK) | 0x40;
    if (G.Player.Type != 0xA)
    {
        Chr_no = G.Player.Id & 1;
        lo = Aim_data.pitch[Chr_no].min + G.Player.Pos_y;
        hi = Aim_data.pitch[Chr_no].max + G.Player.Pos_y;
        if (G.Player.Type < 6 && G.Player.Type != 3)
        {
            lo = Aim_data.pitch[Chr_no].Alt_min + G.Player.Pos_y;
            hi = Aim_data.pitch[Chr_no].Alt_max + G.Player.Pos_y;
        }
        if (ScratchPad->Mat.t[1] < lo)
        {
            MAIN.Player.Be_flg = (PL_BE_FLG_CUR & EM_BE_STATE_MASK) | 0x80;
        }
        if (hi < ScratchPad->Mat.t[1])
        {
            MAIN.Player.Be_flg = (PL_BE_FLG_CUR & EM_BE_STATE_MASK) | 0x20;
        }
    }
}

UCHAR Tama_ck_i(VOID)
{
    UCHAR item_id;

    UCHAR slot;

    UCHAR ammo;

    ITEM_WORK* pI_wk2;

    ITEM_WORK* pI_wk3;

    ITEM_WORK* pI_wk;

    slot = EQUIP_NO_CUR;
    if (slot != 0)
    {
        pI_wk = &pItem_work_base[slot];

        item_id = pI_wk[-1].Id;
        if (item_id != 1)
        {
            if (item_id == 6)
            {
                return pI_wk[-1].Num;
            }

            ammo = pI_wk[-1].Num & 0x7F;
            if (ammo == 0)
            {
                if (Flg_ck(&MAIN.SaveData.Scenario_flg, 0x7E) != 0)  // address as integer
                {
                    pI_wk2 = &pItem_work_base[MAIN.SaveData.Equip_no];

                    if (pI_wk2[-1].Id == 0xA)
                    {
                        pI_wk2[-1].Num = 4;
                        return 4U;
                    }

                    goto block_9;
                }
            block_9:
                ammo = 0;
                if (Flg_ck(&MAIN.SaveData.Scenario_flg, 0x7A) != 0)  // address as integer
                {
                    pI_wk3 = (ITEM_WORK*)((UCHAR*)pItem_work_base + (G.SaveData.Equip_no << 1));  // pointer type conversion (no code)
                    if (pI_wk3[-1].Id == 5)
                    {
                        pI_wk3[-1].Num = 6;
                        return 6U;
                    }

                    goto block_12;
                }

                return ammo;
            }

            return ammo;
        }
    }
block_12:

    ammo = 0;
    return ammo;
}

UCHAR Tama_reload_ck(VOID)
{
    return (Search_item_id(G.Player.Type + 9) + 1) & 0xFF;
}

INT Tama_once_ck(VOID)
{
    UCHAR count;
    ITEM_WORK* pI_wk;

    pI_wk = &pItem_work_base[G.SaveData.Equip_no];

    count = pI_wk[-1].Num;
    if (count & 0x80)
    {
        return 0;
    }
    pI_wk[-1].Num = count | 0x80;
    return 1;
}

VOID Tama_reload(VOID)
{
    ITEM_WORK* pI_wk;
    UCHAR best;
    UCHAR key;
    UCHAR Lim;
    UCHAR n;
    UCHAR i;

    UCHAR idx;

    best = 0;
    key = MAIN.Player.Type + 9;
    Lim = Item_data_tbl.items[G.Player.Type - 1].Item_max;
    n = 8;
    if ((G.Player.Id & 3) != 1)
    {
        n = 6;
    }
    i = 0;
    do
    {
        pI_wk = &pItem_work_base[i];
        n--;

        if (pI_wk->Id == key && best < pI_wk->Num)
        {
            idx = i;
            best = pI_wk->Num;
        }

        i++;
    } while (n != 0);
    if (Lim < best)
    {
        pItem_work_base[G.SaveData.Equip_no - 1].Num = Lim;
        pItem_work_base[idx].Num = best - Lim;
        return;
    }
    pItem_work_base[G.SaveData.Equip_no - 1].Num = best;

    pItem_work_base[idx].Id = 0;

    Sort_item();
}

INT Pl_view_esp_set(VOID)
{
    GsRVIEW2 V0;
    VECTOR a;
    VECTOR b;
    VECTOR n;

    UCHAR flag;

    INT dx;
    INT dz;

    INT h;

    INT hp;
    PARTS_W* pP_ptr;

    flag = 0;
    if (G.Player.Type >= 6)
    {
        return 0;
    }
    if (G.Player.Type == 3)
    {
        flag = 1;
    }

    V0 = G.pRoom->Cut[G.SaveData.Cut_no].View;
    dx = V0.vrx - V0.vpx;
    dz = V0.vrz - V0.vpz;

    ScratchWord[7] = dz > 0 ? 0xC00 : 0x400;

    if (dx != 0)
    {
        ScratchWord[7] = (dx < 0 ? 0x800 : 0x1000) - catan((dz << 12) / dx);
    }
    h = flag * 0xA0;

    pP_ptr = MAIN.Player.pSin_parts_ptr;
    hp = h + 0xA0;
    ScratchSv[0] = hp; // plain Sv.vx changes scheduling
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0;
    ScratchPad->Mat = GsIDMATRIX;
    pP_ptr = &pP_ptr[14];
    RotMatrixY(ScratchWord[7] + 0x400, &ScratchPad->Mat);
    ApplyMatrixSV(&ScratchPad->Mat, &ScratchPad->Sv, &ScratchPad->Sv);

    a.vx = (ScratchPad->Sv.vx + V0.vpx - pP_ptr->Workm.t[0]) / 18;
    a.vz = (ScratchPad->Sv.vz + V0.vpz - pP_ptr->Workm.t[2]) / 18;
    a.vy = (hp + V0.vpy - pP_ptr->Workm.t[1]) / 18;

    b.vx = (V0.vpx - ScratchPad->Sv.vx - pP_ptr->Workm.t[0]) / 18;
    b.vz = (V0.vpz - ScratchPad->Sv.vz - pP_ptr->Workm.t[2]) / 18;
    {
        INT ofs_y = -0xA0;
        b.vy = (h + ofs_y + V0.vpy - pP_ptr->Workm.t[1]) / 18;
    }

    ScratchPad->Sv.vx = flag * 400;
    ScratchPad->Sv.vy = 1000;
    ScratchPad->Sv.vz = 0;
    ApplyMatrix(&pP_ptr->Workm, &ScratchPad->Sv, &n);
    OuterProduct0(&n, &a, &a);
    OuterProduct0(&n, &b, &b);
    if (a.vy & 0x80000000)
    {
        if (b.vy & 0x80000000)
        {
            return 0;
        }
        if (!((a.vx ^ b.vx) & 0x80000000))
        {
            return 0;
        }
        if ((a.vz ^ b.vz) & 0x80000000)
        {
            goto action;
        }
    }
    return 0;
action:
    ScratchVec[0] = V0.vrx - V0.vpx;
    ScratchPad->Vec.vy = V0.vry - V0.vpy;
    ScratchPad->Vec.vz = V0.vrz - V0.vpz;
    VectorNormal(&ScratchPad->Vec, &ScratchPad->Vec);
    {
        INT jitter = -0x80;
        ScratchPad->Vec.vx = ScratchPad->Vec.vx / 8 + V0.vpx - ((rand() & 0xFF) + jitter);
        ScratchPad->Vec.vy = ScratchPad->Vec.vy / 8 + V0.vpy - ((rand() & 0xFF) + jitter);
        ScratchPad->Vec.vz = ScratchPad->Vec.vz / 8 + V0.vpz - ((rand() & 0xFF) + jitter);
    }
    Esp_call(5, 0x12, 0, MON_ID_MAT, &ScratchPad->Vec, 0);
    if (flag)
    {
        {
            INT kick = 0x100;
            ScratchPad->Vec.vx = ScratchPad->Vec.vx + kick - (rand() & 0x1FF);
            ScratchPad->Vec.vy = ScratchPad->Vec.vy + kick - (rand() & 0x1FF);
            ScratchPad->Vec.vz = ScratchPad->Vec.vz + kick - (rand() & 0x1FF);
        }
        Esp_call(5, 0x12, 0, Monitor.pId_mat, &ScratchPad->Vec, 0);
        {
            INT kick = 0x100;
            ScratchPad->Vec.vx = ScratchPad->Vec.vx + kick - (rand() & 0x1FF);
            ScratchPad->Vec.vy = ScratchPad->Vec.vy + kick - (rand() & 0x1FF);
            ScratchPad->Vec.vz = ScratchPad->Vec.vz + kick - (rand() & 0x1FF);
        }
        Esp_call(5, 0x12, 0, Monitor.pId_mat, &ScratchPad->Vec, 0);
        {
            INT kick = 0x100;
            ScratchPad->Vec.vx = ScratchPad->Vec.vx + kick - (rand() & 0x1FF);
            ScratchPad->Vec.vy = ScratchPad->Vec.vy + kick - (rand() & 0x1FF);
            ScratchPad->Vec.vz = ScratchPad->Vec.vz + kick - (rand() & 0x1FF);
        }
        Esp_call(5, 0x12, 0, Monitor.pId_mat, &ScratchPad->Vec, 0);
    }
    return 1;
}

UCHAR Aim_auto_en = 1;  // nonzero while auto-aim is enabled

// pad vibration of a shot, by weapon
PL_VIB_REC Pl_vib_tbl[9] =
{
    { 3, 6, 0, 0, 0 },        // 0 //
    { 3, 9, 250, 0, 5 },      // 1 //
    { 3, 11, 250, 0, 5 },     // 2 //
    { 3, 11, 250, 0, 5 },     // 3 //
    { 3, 15, 0, 0, 0 },       // 4 //
    { 3, 15, 250, 0, 13 },    // 5 //
    { 3, 15, 250, 0, 13 },    // 6 //
    { 3, 15, 250, 0, 13 },    // 7 //
    { 11, 20, 250, 0, 15 },   // 8 //
};

// fire sub-state handlers, by action state
PL_WEP_FN Pl_fire_tbl[11] =
{
    Pl_fire_start,           // 0 //
    Pl_fire_shoot,         // 1 //
    Pl_fire_end_click,       // 2 //
    Pl_fire_reraise_start,   // 3 //
    Pl_fire_reraise,         // 4 //
    Pl_fire_hold_start,      // 5 //
    Pl_fire_hold,            // 6 //
    Pl_fire_nop,             // 7 //
    Pl_fire_reverse_start,   // 8 //
    Pl_fire_reverse,         // 9 //
    Pl_fire_loop,            // 10 //
};

// animation frame after which the shot ends, by weapon
UCHAR Pl_fire_end_tbl[0xC] =
{
    0x08,   // 0 //
    0x18,   // 1 //
    0x0C,   // 2 //
    0x0C,   // 3 //
    0x00,   // 4 //
    0x0D,   // 5 //
    0x0D,   // 6 //
    0x0D,   // 7 //
    0x12,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
};

// hit frame, kind and sounds of a shot, by weapon
PL_HIT_REC Pl_fire_hit_tbl[9] =
{
    { 2, 5, 7, 8, 0 },    // 0 //
    { 3, 5, 7, 8, 0 },    // 1 //
    { 4, 5, 7, 8, 0 },    // 2 //
    { 5, 5, 7, 8, 0 },    // 3 //
    { 6, 0, 0, 0, 0 },    // 4 //
    { 7, 2, 7, 8, 0 },    // 5 //
    { 8, 2, 7, 8, 0 },    // 6 //
    { 9, 2, 7, 8, 0 },    // 7 //
    { 10, 5, 7, 8, 0 },   // 8 //
};

// muzzle flash effect, by weapon
PL_ESP_REC Pl_flash_tbl[12] =
{
    { 1, 17, 0, 0, 110, 540, 0 },       // 0 //
    { 1, 17, 1, 0, 640, 1110, 0 },      // 1 //
    { 1, 17, 2, 0, 160, 610, 0 },       // 2 //
    { 1, 17, 10, 0, 160, 610, 0 },      // 3 //
    { 0, 0, 0, 0, 0, 0, 0 },            // 4 //
    { 2, 8, 7, 0, 400, 660, 0 },        // 5 //
    { 2, 8, 7, 0, 400, 660, 0 },        // 6 //
    { 2, 8, 7, 0, 400, 660, 0 },        // 7 //
    { 1, 11, 9, 0, -190, 1020, 90 },    // 8 //
    { 1, 11, 9, 0, -190, 1020, -60 },   // 9 //
    { 1, 11, 9, 0, -60, 1040, 90 },     // 10 //
    { 1, 11, 9, 0, -60, 1040, -60 },    // 11 //
};

// ejected shell effect, by weapon
PL_ESP_REC Pl_shell_tbl[9] =
{
    { 3, 5, 0, 0, 370, -2870, -220 },     // 0 //
    { 25, 5, 9, 0, 360, -2050, -440 },    // 1 //
    { 99, 0, 0, 0, 0, 0, 0 },             // 2 //
    { 99, 0, 0, 0, 0, 0, 0 },             // 3 //
    { 0, 0, 0, 0, 0, 0, 0 },              // 4 //
    { 99, 0, 0, 0, 0, 0, 0 },             // 5 //
    { 99, 0, 0, 0, 0, 0, 0 },             // 6 //
    { 99, 0, 0, 0, 0, 0, 0 },             // 7 //
    { 2, 9, 11, 0, 1400, -2800, -300 },   // 8 //
};

// second muzzle effect, by weapon
PL_ESP_REC Pl_flash2_tbl[12] =
{
    { 2, 9, 11, 0, 110, 500, 0 },     // 0 //
    { 2, 9, 11, 0, 640, 1060, 0 },    // 1 //
    { 2, 9, 11, 0, 160, 610, 0 },     // 2 //
    { 2, 9, 11, 0, 160, 610, 0 },     // 3 //
    { 0, 0, 0, 0, 0, 0, 0 },          // 4 //
    { 2, 9, 11, 0, 640, 1060, 0 },    // 5 //
    { 2, 9, 11, 0, 640, 1060, 0 },    // 6 //
    { 2, 9, 11, 0, 640, 1060, 0 },    // 7 //
    { 2, 8, 2, 0, 430, -830, 90 },    // 8 //
    { 2, 8, 2, 0, 430, -830, -60 },   // 9 //
    { 2, 8, 2, 0, 570, -810, 90 },    // 10 //
    { 2, 8, 2, 0, 570, -810, -60 },   // 11 //
};

UCHAR Pl_flash2_pad[8] = { 0x11, 0x0D, 0x21, 0x21, 0x11, 0x10, 0x10, 0x10 };    // data after the second muzzle table, unused

// data before Pl_reload_tbl; the reload dispatch indexes pointers from here
UCHAR reload_fx_tbl[0x8] =
{
    0x0A,   // 0 //
    0x0A,   // 1 //
    0x0C,   // 2 //
    0x0C,   // 3 //
    0x0A,   // 4 //
    0x07,   // 5 //
    0x07,   // 6 //
    0x07,   // 7 //
};

// reload effect handlers, by weapon
PL_WEP_FN Pl_reload_tbl[8] =
{
    Pl_reload_beretta,   // 0 //
    Pl_reload_shotgun,   // 1 //
    Pl_reload_colt,      // 2 //
    Pl_reload_colt,      // 3 //
    Pl_reload_def,   // 4 //
    Pl_reload_def,   // 5 //
    Pl_reload_def,   // 6 //
    Pl_reload_def,   // 7 //
};

// hand height bounds for the aim level and scripted animation chain
AIM_DATA Aim_data =
{
    { { 63510, 63880, 63006, 63256 }, { 63619, 63919, 63346, 63596 } },
    { { 0x00, 0x00 }, { 0x00, 0x01 }, { 0x00, 0x02 }, { 0x00, 0x03 }, { 0x00, 0x04 }, { 0x01, 0x00 }, { 0x01, 0x01 }, { 0x01, 0x02 }, { 0x01, 0x03 }, { 0x01, 0x04 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 }, { 0x00, 0x00 } },
};
