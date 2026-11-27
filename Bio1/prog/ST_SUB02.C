/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ST_SUB02.C
 */

#include "TYPES.H"
#include "ST_SUB02.H"
#include "FILE.H"
#include "INVENTORY.H"
#include "ITEM.H"
#include "MAP.H"
#include "MOJIDISP.H"
#include "SCE1.H"
#include "SND.H"
#include "ST_SUB00.H"
#include "ST_SUB01.H"
#include "ST_SUB03.H"
#include "SCRATCHPAD.H"
#include "FLAG.H"
#include "SCENARIO.H"

// functions without a C definition
VOID Mess_set(USHORT, INT);

UINT Use_flg[2];   // item-use flag words

VOID St_menu0_item(VOID)
{
    UCHAR t;
    UCHAR i;
    UCHAR v;
    INT x;

    switch (St.Sub_sts)
    {
    case 0:
        St.Sub_sts = 1;
        St.Sub_timer = 0x80;
        St.Move_anm[0] = 0;
    case 1:
        if (++St.Sub_timer & 8)
        {
            St.Sub_sts = 2;
            St.St_flg &= 0xFD;
        }

        Name_disp(St.Item_id, 0, (USHORT)(0x30 - (USHORT)Scr_ofs[0]), (USHORT)(0xBA - (USHORT)Scr_ofs[1]));

        break;
    case 2:
        if (!(KEY_TRG_CUR & 0x8000))
        {
            Name_disp(St.Item_id, 0, (USHORT)(0x30 - (USHORT)Scr_ofs[0]), (USHORT)(0xBA - (USHORT)Scr_ofs[1]));

            if (MAIN.SaveData.Key_trg & 0x4000)
            {
                switch (St.Ibox_sel)
                {
                case 0:
                    if (St.Item_id < 0xB)
                    {
                        St_item_equip_chg();
                        St.Sub_sts = 3;
                        St.Sub_timer &= 0x7F;
                        Snd_se_play(SE_BANK_ENEMY, 6, 0);
                    }
                    else
                    {
                        St.Sub_sts = 4;
                        St.Act_sts = 0;
                    }
                    break;
                case 1:
                case 2:
                    St.Sub_sts = 4;
                    St.Act_sts = 0;
                    St.St_flg &= 0xFB;
                    Snd_se_play(SE_BANK_ENEMY, 6, 0);
                    break;
                }
                break;
            }
            if (G.Padd1_trg & (PAD_UP | PAD_DOWN))
            {
                if (G.Padd1_trg & PAD_UP)
                {
                    if (St.Ibox_sel != 0)
                    {
                        St.Ibox_sel--;
                    }
                }
                else if (St.Ibox_sel < 2)
                {
                    St.Ibox_sel++;
                }
                Snd_se_play(SE_BANK_ENEMY, 4, 0);
            }
            if (!(St.St_flg & 2))
            {
                break;
            }
            St.Sub_timer &= 0x7F;
        }
        St.Sub_sts = 3;
        Snd_se_play(SE_BANK_ENEMY, 5, 0);
    case 3:
        if (!((St.Sub_timer -= 1) & 0x7F))
        {
            St.Sub_rtn0 = 1;
            St_sel_item_set();
            if (St.St_flg & 0x80)
            {
                St.St_flg |= 1;

                return;
            }
        }

        Name_disp(St.Item_id, 0, (USHORT)(0x30 - (USHORT)Scr_ofs[0]), (USHORT)(0xBA - (USHORT)Scr_ofs[1]));

        break;
    case 4:
        St_item_act_tbl[St.Ibox_sel]();

        if (St.St_flg & 0x40)
        {
            return;
        }

        if (St.St_flg & 2)
        {
            St.Sub_sts = 3;
            St.Sub_timer &= 0x7F;
        }
        else if (St.St_flg & 4)
        {
            St.Sub_sts = 2;
        }
        break;
    }
    if (St.Sub_timer != 0 && !(St.Move_anm[0] & 8))
    {
        ScratchPad->Spr.attribute = 0x01000040;
        ScratchPad->Spr.tpage = 0x1C;
        ScratchPad->Spr.cy = 0x1E4;
        ScratchPad->Spr.cx = 0;
        ScratchPad->Spr.x = 0x90;
        if (!(St.Sub_sts & 1) && St.Move_anm[0] == 0)
        {
            ScratchPad->Spr.u = 0x30;
            ScratchPad->Spr.v = 0x60;
            ScratchPad->Spr.w = 0x30;
            ScratchPad->Spr.h = 0x18;
            ScratchPad->Spr.y = St.Ibox_sel * 0x18 + 0x39;
            GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 5);
        }
        ScratchPad->Spr.y = 0x39;
        ScratchPad->Spr.u = 0x30;
        if (St.Wep_flg != 0)
        {
            ScratchPad->Spr.v = 0;
        }
        else
        {
            ScratchPad->Spr.v = 0x18;
        }
        t = St.Sub_timer;
        i = 0;
        if (!(t & 0x80))
        {
            ScratchPad->Spr.x += (8 - St.Sub_timer) * 6;
            ScratchPad->Spr.y += (8 - St.Sub_timer) * 3;
            ScratchPad->Spr.u += (8 - St.Sub_timer) * 6;
            ScratchPad->Spr.v += (8 - St.Sub_timer) * 3;
        }
        ScratchPad->Spr.w = (t & 0x7F) * 6;
        ScratchPad->Spr.h = (t & 0x7F) * 3;
        ScratchPad->Spr.cy = 0x1E4;
        do
        {
            if ((St.Sub_timer & 0x80) && St.Move_anm[0] != 0)
            {
                x = St.Move_anm[0] - i;
                ScratchPad->Spr.x = (SHORT)x * 8;  // sign-extends the halfword
                if (x * 8 < 0)
                {
                    ScratchPad->Spr.x = 0x90;
                }
                else
                {
                    ScratchPad->Spr.x += 0x90;
                }
            }
            GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 5);
            v = ScratchPad->Spr.v;
            ScratchPad->Spr.v = v + 0x18;
            if (i == 0 && St.Wep_flg != 0)
            {
                ScratchPad->Spr.v = v + 0x30;
            }
            i++;
            ScratchPad->Spr.y += 0x18;
        } while (i < 3);
    }
}

VOID St_menu0_map(VOID)
{
    if (St_map_main() != 0)
    {
        St.Sub_rtn0 = 1;
        St.Model_sts &= 0x80;
    }
}

VOID St_menu0_file(VOID)
{
    if (St_file_main() != 0)
    {
        St.Sub_rtn0 = 1;
        St.Model_sts &= 0x80;
    }
}

VOID St_menu0_radio(VOID)
{
    if (Flg_ck(Use_flg, 0x3F) != 0)
    {
        St.Sub_rtn0 = 1;

        G.SaveData.Use_id = 0x4D;

        St.St_flg |= 0x81;
        Snd_se_play(SE_BANK_ENEMY, 7, 0);
        return;
    }
    if (Item_view_main() != 0)
    {
        St.Sub_rtn0 = 1;
        St.Item_id = 0;
    }
}

VOID St_menu0_exit(VOID)
{
    St.Sub_rtn0 = 1;

    St_fade_out_set();
}

VOID St_item_equip_chg(VOID)
{
    UCHAR slot;

    slot = (St.Icursol_main >> 1) - 3;
    if (EQUIP_NO_CUR != (slot & 0xFF))
    {
        MAIN.SaveData.Equip_no = slot;
        return;
    }
    MAIN.SaveData.Equip_no = 0;
}

VOID St_item_use(VOID)
{
    UCHAR i;
    UCHAR slot;

    if (St.Act_sts == 0)
    {
        slot = (St.Icursol_main >> 1) - 4;

        for (i = 9; St.Item_id < Item_use_tbl[i]; i--)
        {
        }
        if (((UCHAR (**)(UCHAR))St_item_use_tbl)[i](slot))  // the handlers are declared with other prototypes: the table is VOID*
        {
            Snd_se_play(SE_BANK_ENEMY, 6, 0);
            if (pItem_work_base[slot].Num != 0)
            {
                pItem_work_base[slot].Num--;
                if (pItem_work_base[slot].Num == 0)
                {
                    pItem_work_base[slot].Id = 0;
                    Sort_item();

                    St_sel_item_set();
                }
            }
            St.St_flg |= 2;

            Name_disp(St.Item_id, 0, (USHORT)(0x30 - (USHORT)Scr_ofs[0]), (USHORT)(0xBA - (USHORT)Scr_ofs[1]));
        }
        else
        {
            St.Act_sts = 1;

            Mess_set(i + 0xF6, 0);

            St.St_flg &= 0xFB;
        }
    }
    else if (!(G.SaveData.Mess_flg & MESS_FLG_ACTIVE))
    {
        St.St_flg |= 4;
    }
}

INT St_item_use_none(VOID)
{
    return 0;
}

INT St_item_use_none1(VOID)
{
    return 0;
}

INT St_item_use_flg(VOID)
{
    UCHAR* pSel_item = &St.Item_id;

    if (Flg_ck(Use_flg, *pSel_item - 0x1B) != 0)
    {
        G.SaveData.Use_id = *pSel_item;
        St.St_flg |= 0x80;
        return 1;
    }
    return 0;
}

INT St_item_use_flg4(VOID)
{
    UCHAR* pSel_item = &St.Item_id;

    if (Flg_ck(Use_flg, *pSel_item - 0x1B) != 0)
    {
        G.SaveData.Use_id = *pSel_item;
        St.St_flg |= 0x80;
        return 1;
    }
    return 0;
}

INT St_item_use_book(VOID)
{
    UCHAR* pSel_item = &St.Item_id;

    if (*pSel_item == 0x3E)
    {
        if (Flg_ck(Use_flg, 0x23) != 0)
        {
            G.SaveData.Use_id = *pSel_item;
            St.St_flg |= 0x80;
            return 1;
        }

        return 0;
    }
    return 0;
}

UCHAR St_item_use_heal(VOID)
{
    UCHAR Heal_val;
    UCHAR lo;
    UCHAR hi;
    UCHAR pRet;
    UCHAR* pFlg;
    UCHAR* pSt_flg;
    UCHAR Pl_hit_flg;

    Heal_val = (&Item_data_tbl.Heal_tbl[-1])[St.Item_id];
    pRet = 0;
    if (Heal_val != 0)
    {
        lo = Heal_val & 0xF;
        hi = Heal_val & 0xF0;
        if (lo != 0 && G_PLAYER->Life < G.Player.Max_life)
        {
            St.St_flg |= 0x20;
            pRet = 1;
            switch (lo)
            {
            case 1:
                G.Player.Life += G.Player.Max_life / 3;

                break;
            case 2:
                G.Player.Life += G.Player.Max_life * 2 / 3;

                break;
            case 3:
                G.Player.Life = G.Player.Max_life;
                break;
            }
            if (G_PLAYER->Life > G.Player.Max_life)
            {
                G.Player.Life = G.Player.Max_life;
            }
        }
        if (hi != 0 && ((Pl_hit_flg = G_PLAYER->Sca_info) & 0x22))
        {
            switch (hi)
            {
            case 0x10:
                if (Pl_hit_flg & 0x20)
                {
                    Flg_off(&MAIN.SaveData.Scenario_flg2[0], 0x43);
                    pFlg = &St.St_flg;
                    G.Player.Sca_info &= 0xDF;
                    *pFlg |= 0x10;
                    pRet = 1;
                }
                break;
            case 0x20:
                if (Pl_hit_flg & 2)
                {
                    pRet = 1;
                    G.Player.Sca_info = Pl_hit_flg & 0xFD;
                    pSt_flg = &St.St_flg;
                    *pSt_flg |= 0x10;
                }
                break;
            }
        }
    }
    if (pRet != 0)
    {
        St.Ecg_rtn = 2;
        St.Act_res = 0xFF;
    }
    return pRet;
}

INT St_item_use_always(VOID)
{
    return 1;
}

INT St_item_use_none9(VOID)
{
    return 0;
}

VOID St_com_inspect(VOID)
{
    UCHAR* p;
    UCHAR* q;

    switch ((&St.Act_sts)[0])
    {
    case 0:
        (&St.Act_sts)[0] = 1;
        Item_model_load();

    case 1:
        p = &St.Move_anm[0];
        if (!(++(*p) & 8))
        {
            break;
        }
        (&St.Act_sts)[0] = 2;
        St.View_rno = 0;

    case 2:
        if (!(Item_view_main() & 0xFF))
        {
            return;
        }
        (&St.Act_sts)[0] = 3;

    case 3:
        q = &St.Move_anm[0];
        (*q)--;
        if (*q == 0)
        {
            St.St_flg |= 4;
        }
        break;
    default:
        return;
    }

    if (St_slot_pos_tbl)
    {
        Name_disp(St.Item_id, 0, (USHORT)(0x30 - (USHORT)Scr_ofs[0]), (USHORT)(0xBA - (USHORT)Scr_ofs[1]));
    }
    else
    {
        Name_disp(St.Item_id, 0, (USHORT)(0x30 - (USHORT)Scr_ofs[0]), (USHORT)(0xBA - (USHORT)Scr_ofs[1]));
    }
}

// submenu row actions: use, examine, combine
VOID (*St_item_act_tbl[3])(VOID) =
{
    St_item_use,   // 0 //
    St_com_inspect,       // 1 //
    St_com_mix,           // 2 //
};

// use type handlers, indexed by use type group
VOID* St_item_use_tbl[9] =
{
    St_item_use_none,         // 0 //
    St_item_use_none1,      // 1 //
    St_item_use_none1,      // 2 //
    St_item_use_flg,      // 3 //
    St_item_use_flg4,   // 4 //
    St_item_use_flg4,   // 5 //
    St_item_use_book,     // 6 //
    St_item_use_heal,         // 7 //
    St_item_use_always,       // 8 //
};
