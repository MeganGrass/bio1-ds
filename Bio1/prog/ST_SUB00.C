/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ST_SUB00.C
 */

#include "TYPES.H"
#include "ST_SUB00.H"
#include "CUT.H"
#include "ENEMY.H"
#include "EVENT.H"
#include "FILE.H"
#include "GAME.H"
#include "INVENTORY.H"
#include "ITEM.H"
#include "JOINT.H"
#include "MAIN.H"
#include "MAP.H"
#include "SCE1.H"
#include "SCE_AT.H"
#include "SND.H"
#include "ST_SUB01.H"
#include "ST_SUB03.H"
#include "SUB.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "SCENARIO.H"
#include "MONITOR.H"

// functions without a C definition
VOID Mess_set(USHORT, INT);

GsRVIEW2 View_ref;   // reference view of the 3D scene

VOID Status(VOID)
{
    INT Ex_slot;
    INT pl_id;
    INT Used_id;
    UINT Req_mask;

    G.Vloop = 0;
    Task_suspend(0);
    ResetGraph(1);
    GsInitGraph2(0x140, 0xF0, 4, 0, 0);
    SetGeomOffset(0x70, 0x4C);
    Fade_box_pos_set();
    ScratchPad->vram_rect.x = 0x180;
    ScratchPad->vram_rect.y = 0x100;
    ScratchPad->vram_rect.w = 0x40;
    ScratchPad->vram_rect.h = 0x100;
    StoreImage(&ScratchPad->vram_rect, (ULONG*)IMG_TRANS_BUF); // fixed buffer address as typed pointer
    G.Fade_hrate = 2;
    G.Clr_r = 0;
    G.Clr_g = 0;
    G.Clr_b = 0;
    G.Fade_add = -0x1800;
    Evt_fade_start();
    Task_sleep(1);

    if (G.System_flg & SYS_REQ_FILE)
    {
        St.Main_rtn = 8;
    }
    else
    {
        St.St_type = 5;

        Req_mask = 0x4000;

        if (!(G.System_flg & SYS_REQ_TYPE5))
        {
            St.St_type = 4;
            if (!(G.System_flg & SYS_REQ_ITEM_SCE))
            {
                do
                {
                    St.St_type--;
                } while (St.St_type != 0 && !((Req_mask >> St.St_type) & MAIN.System_flg));
            }
        }
        St.Main_rtn = 0;
        St.View_rno = 0;
        St.View_act = 0;

        St.Icursol_main = 8;

        St.Res_id = 0;
        St.Model_sts = 0;
        switch (St.St_type)
        {
        case 3:
        case 4:
            St.Item_id = pAot_data->pRec->Item_id;

            Item_model_load(&St.Main_rtn);  // passes 1 argument, Item_model_load declares 0

            break;
        case 2:
            St_init01(&St.Main_rtn);  // passes 1 argument, St_init01 declares 0
            break;
        case 1:
            if (SEL_ID_CUR >= 0x33)
            {
                St.St_type = 6;
                St.Item_id = MAIN.SaveData.Sel_id;

                Item_model_load(&St.Main_rtn);  // passes 1 argument, Item_model_load declares 0
            }

            break;
        }

        pl_id = G.Player.Id & 3;
        Ex_slot = (pl_id + 1) & 2;

        St.Chr_flg = Ex_slot;
        St.nSlot = Ex_slot + 6;
        St.Chr_flg = Ex_slot | (G.Player.Id & 1);

        if (pl_id == 3)
        {
            St.Radio_flg = 0;
            if (Flg_ck(&MAIN.SaveData.Scenario_flg, 0x7F) == 0 && Flg_ck(&MAIN.SaveData.Scenario_flg2, 0x38) != 0 && Flg_ck(&MAIN.SaveData.Scenario_flg2, 0x22) == 0) // address passed as UINT
            {
                St.Radio_flg = 1;
            }
        }
        else
        {
            St.Radio_flg = Flg_ck(&MAIN.SaveData.Scenario_flg, 0x7F); // address passed as UINT
        }

        St.Act_res = 0;
        St.St_flg = 0;
        GsSetProjection(0xC0);
        View_ref.vpx = 0x3A98;
        View_ref.vpy = 0;
        View_ref.vpz = 0;
        View_ref.vrx = 0;
        View_ref.vry = 0;
        View_ref.vrz = 0;
        View_ref.rz = 0;
        View_ref.super = 0;
        GsSetRefView2(&View_ref);
        Item_light[0].vx = 0x64;
        Item_light[0].vy = 0x50;
        Item_light[0].vz = 0x50;
        Item_light[1].vx = -0x64;
        Item_light[1].vy = 0x50;
        Item_light[1].vz = 0x50;
        Item_light[2].vx = 0;
        Item_light[2].vy = -0x64;
        Item_light[2].vz = 0x50;
        GsSetAmbient(0x199, 0x199, 0x199);
        GsSetLightMode(0);
    }

    while (1)
    {
        ScratchPad->Spr.r = 0x80;
        ScratchPad->Spr.g = 0x80;
        ScratchPad->Spr.b = 0x80;
        ScratchPad->Spr.mx = 0;
        ScratchPad->Spr.my = 0;
        ScratchPad->Spr.rotate = 0;
        switch (St.Main_rtn)
        {
        case 0:
            if (G.SaveData.Fade_kido < 0)
            {
                switch (St.St_type)
                {
                case 0:
                    St.Main_rtn = 1;
                    St.Sub_rtn0 = 0;

                    Map_scr_init();
                    File_get_init();
                    break;
                case 1:
                    St.Main_rtn = 2;
                    Mess_set(0xC4, 0);
                    break;
                case 3:
                case 4:
                    St.Main_rtn = 4;
                    St.View_rno = 0;
                    break;
                case 2:
                    St.Main_rtn = 3;
                    St.Sub_rtn0 = 0;

                    break;
                case 5:
                    St.Main_rtn = 5;
                    St_init03();

                    break;

                case 6:
                    St.Main_rtn = 6;
                    Map_scr_init();
                    File_get_init();
                    St.View_rno = 0;

                    break;
                }
                St.Ecg_rtn = 0;
                St.Face_anm = 0xFF;
            }
            break;
        case 1:
            if ((MAIN.SaveData.Key_trg & 0x8000) && St.Sub_rtn0 == 1)
            {
                goto quit;
            }

            if (!(G.Padd1 & PAD_SELECT) && (G.Padd1_trg & PAD_START) && St.View_act == 0 && St.Act_res == 0)
            {
                goto quit;
            }
            St_main00();

            if (St.St_flg & 0x41)
            {
                goto quit;
            }

            if (St.Main_rtn != 7)
            {
                St_disp_ECG();
            }

            if (!(St.St_flg & 0x41))
            {
                break;
            }
        quit:
            St_fade_out_set();
            Snd_se_play(SE_BANK_ENEMY, 5, 0);
            G.SaveData.Mess_flg = 0;
            break;
        case 2:
            if (G.SaveData.Mess_flg & MESS_FLG_ACTIVE)
            {
                St_disp_ECG();
            }
            else
            {
                St_fade_out_set();
            }
            break;
        case 4:
            if ((UCHAR)Item_view_main() != 0) // width or sign conversion
            {
                St_fade_out_set();
            }
            else
            {
                St_disp_ECG();
            }
            break;
        case 3:
            if ((UCHAR)St_main01() != 0) // width or sign conversion
            {
                St_fade_out_set();
            }
            else
            {
                St_disp_ECG();
            }
            break;
        case 5:
            if (St_main03() != 0)
            {
                St_fade_out_set();
            }
            else
            {
                St_disp_ECG();
            }
            break;
        case 6:
            if ((UCHAR)Item_view_main() != 0) // width or sign conversion
            {
                St.Main_rtn = 1;
                St.St_type = 0;
                St.Sub_rtn0 = 0;
            }
            St_disp_ECG();
            break;
        case 7:
            if (St.Model_sts & 0x80)
            {
                G.pEm = PL_EM;
                PartsWork_link(PL_PAC_BUF);
            }
            G.Player.Hokan_flg = 0;
            G.Player.Move_cnt = 0;
            G.Player.Move_wait = 0;
            G.Player.Damage_no = 0;
            if ((St.St_flg & 0x80) && (Used_id = G.SaveData.Use_id) < 0x1F && Used_id >= 0x1D)
            {
                *(INT*)&PL_EM->Routine_0 = 0x10008;  // INT* view of the field (a plain access changes the code)
                MAIN.Player.Move_no = 0x37;
                MAIN.Player.Sce_flg = 0x20;
                MAIN.Player.Set_flg_no = 0x20;
                Joint_move(0, MAIN.Player.pRoom_kan_t_ptr, MAIN.Player.pRoom_seq_t_ptr, 0x400);
            }
            else
            {
                PL_ROUTINE_WORD = 1;
                G.Player.Move_no = 0;
                Joint_move(0, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x400);
            }
            switch (St.St_type)
            {
            case 0:
                if (St.St_flg & 0x80)
                {
                    switch (G.SaveData.Use_id)
                    {
                    case 0x1D:
                    case 0x1E:
                        G.Player.Type = 0;
                        St.Model_sts = -1;
                        break;

                    case 0x4D:
                        G.Player.Type = 0xB;
                        St.Model_sts = -1;
                        break;
                    default:
                        St_equip_wep_set();
                        break;
                    }
                    break;
                }
                St_equip_wep_set();
                break;
            case 1:
            case 2:
                St_equip_wep_set();
                break;
            }
            if (St.Model_sts != 0)
            {
                Pl_weapon_ch(G.Player.Type, 0xE, WEP_ANIM_BUF, WEP_PACKET_BUF);
            }
            if (MAIN.System_flg & SYS_MIRROR_MATRIX)
            {
                Mirror_weapon_cp(0xE);
            }
            while ((MAIN.System_flg & SYS_FADE_OUT) || G.Cbuf_id != 0)
            {
                Task_sleep(1);
            }
            St_exit();
            Task_exit();

        case 8:
            St_reset();
            while (File_pickup_main() == 0)
            {
                Task_sleep(1);
            }
            if (G.Cbuf_id != 0)
            {
                Task_sleep(1);
            }
            St_exit();
            Task_exit();
            break;
        }

        if (St.Main_rtn != 7)
        {
            St_inv_disp();

            if (St.St_type == 2)
            {
                St_disp_itembox();
            }
        }

        Task_sleep(1);
    }
}

VOID St_fade_out_set(VOID)
{
    St.Main_rtn = 7;

    Evt_fade_req_set(2, 0xC00);
    ScratchPad->vram_rect.x = 0x180;
    ScratchPad->vram_rect.y = 0x100;
    ScratchPad->vram_rect.w = 0x40;
    ScratchPad->vram_rect.h = 0x100;
    LoadImage(&ScratchPad->vram_rect, (ULONG*)IMG_TRANS_BUF); // fixed buffer address as typed pointer
    G.Clr_r = 0;
    G.Clr_g = 0;
    G.Clr_b = 0;
}

VOID St_exit(VOID)
{
    INT i;
    INT t;

    ResetGraph(1);

    GsInitGraph2(0x140U, 0xF0U, 0U, 0U, 0U);
    GsDefDispBuff(0U, 0U, 0U, 0xF0U);
    GsInit3D();
    i = 3;
    Fade_box_pos_set();
    Cut_set();
    do
    {
        i--;
        t = i & 0xFF;
        GsSetFlatLight(t, &G.pRoom->Light[t].Flat);
    } while (t != 0);
    GsSetAmbient(G.pRoom->Ambient_r, G.pRoom->Ambient_g, G.pRoom->Ambient_b);
    GsSetLightMode(0);
    G.Vloop = 2;
    MAIN.System_flg &= ~SYS_STATUS_MASK;
    Task_signal(0);
}

VOID St_equip_wep_set(VOID)
{
    ITEM_WORK* pSlot;

    if (G.SaveData.Equip_no != 0)
    {
        pSlot = &pItem_work_base[G.SaveData.Equip_no];

        if (G.Player.Type != pSlot[-1].Id)
        {
            G.Player.Type = pSlot[-1].Id;

            if ((G.Status_flg & STAT_ARRANGE) && G.Player.Type == 4)
            {
                G.Player.Type = 2;
            }

            Snd_sys_arms_set(G.Player.Type, LOAD_BUF);
            goto skip_tail;
        }
    }
    else if (G.Player.Type != 0)
    {
        G.Player.Type = 0;
    skip_tail:
        St.Model_sts = -1;
    }
}

VOID St_inv_disp(VOID)
{
    UCHAR n;
    UCHAR j;
    INT c;
    UCHAR Loop_ctr;
    SHORT dx;
    SHORT dy;
    ITEM_WORK* pSlot;
    UCHAR* q;
    UCHAR* r;
    UCHAR** ppFrm1;
    UCHAR** ppFrm2;
    UCHAR** ppFrm3;

    ScratchPad->Spr.attribute = 0x01000040;
    ScratchPad->Spr.cx = 0;
    ScratchPad->Spr.tpage = 0x1C;
    if (EQUIP_NO_CUR != 0)
    {
        ScratchPad->Spr.u = 0x58;
        ScratchPad->Spr.v = Item_pix_idx[MAIN.SaveData.Equip_no - 1] << 5;
        ScratchPad->Spr.tpage = 0x1D;
    }
    else
    {
        ScratchPad->Spr.u = 0x90;
        ScratchPad->Spr.v = 0xBE;
    }
    ScratchPad->Spr.x = 0xA0;
    ScratchPad->Spr.y = 0x92;
    ScratchPad->Spr.w = 0x28;
    ScratchPad->Spr.h = 0x1E;
    ScratchPad->Spr.cy = 0x1E6;
    GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 4);
    if (EQUIP_NO_CUR != 0)
    {
        ScratchPad->Spr.tpage = 0x1C;

        pSlot = &pItem_work_base[MAIN.SaveData.Equip_no];

        Print_num(pSlot[-1].Id, pSlot[-1].Num, 3);
    }

    n = G.SaveData.Item_num;
    j = (n + 4) << 1;
    if (!(St.Chr_flg & 2))
    {
        j += 4;
    }
    while (n != 0)
    {
        n--;

        ScratchPad->Spr.y = St_slot_pos_tbl[j -= 1];
        ScratchPad->Spr.x = St_slot_pos_tbl[--j];

        ScratchPad->Spr.tpage = 0x1D;
        ScratchPad->Spr.w = 0x28;
        ScratchPad->Spr.h = 0x1E;
        ScratchPad->Spr.u = 0x58;
        ScratchPad->Spr.v = Item_pix_idx[n] << 5;
        ScratchPad->Spr.cy = 0x1E6;
        GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 4);
        ScratchPad->Spr.tpage = 0x1C;

        pSlot = &pItem_work_base[n];

        Print_num(pSlot->Id, pSlot->Num, 3);
    }

    ScratchPad->Spr.w = 0x28;
    ScratchPad->Spr.h = 0x1E;
    ScratchPad->Spr.u = 0x90;
    ScratchPad->Spr.v = 0xBE;
    ScratchPad->Spr.cy = 0x1E6;
    n = St.nSlot - G.SaveData.Item_num;
    j = 0x18;
    while (n != 0)
    {
        n--;

        ScratchPad->Spr.y = St_slot_pos_tbl[j -= 1];
        ScratchPad->Spr.x = St_slot_pos_tbl[--j];

        GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 4);
    }

    ScratchPad->Spr.x = 0x16;
    ScratchPad->Spr.y = 0x92;
    ScratchPad->Spr.w = 0x1E;
    ScratchPad->Spr.h = 0x1E;
    ScratchPad->Spr.v = 0xE0;
    ScratchPad->Spr.cy = 0x1E5;
    ScratchPad->Spr.u = (G.Player.Id & 3) << 5;
    GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 4);
    n = 8;
    ScratchPad->Spr.cy = 0x1E4;
    ScratchPad->Spr.r = 0x50;
    ScratchPad->Spr.g = 0x50;
    ScratchPad->Spr.b = 0x50;
    ppFrm1 = &St.pFrm_rec;
    *ppFrm1 = St_strip_tbl;
    do
    {
        n -= 2;
        St_spr_pop();
        (&ScratchPad->Spr)->attribute = 0x01000000;
        if (St.St_type == 2)
        {
            if ((St.Icursol_main | 2) == n)
            {
                ScratchPad->Spr.attribute = 0x01000040;
            }
            else if (n == 4)
            {
                ScratchPad->Spr.r = 0x20;
                ScratchPad->Spr.g = 0x20;
                ScratchPad->Spr.b = 0x20;
            }
        }
        else if (St.Main_rtn == 1 && St.Icursol_main == n && St.Sub_rtn0 < 2)
        {
            ScratchPad->Spr.attribute = 0x01000040;
        }

        if (n == 4 && St.Radio_flg == 0)
        {
            ScratchPad->Spr.v = 0x40;
        }
        GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 3);
    } while (n != 0);

    ScratchPad->Spr.attribute = 0x01000040;
    St.pFrm_rec = St_tab_tbl;
    do
    {
        St_spr_pop();
        GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 4);
    } while (St.pFrm_rec > St_tab_tbl - 0x108);

    St.pFrm_rec = St_slot_tbl6;
    ppFrm2 = &St.pFrm_rec;
    do
    {
        *ppFrm2 -= 2;
        q = St.pFrm_rec;
        ScratchPad->Spr.attribute = ((*(SHORT*)q << 16) & 0xC00000) | 0x01000040; // pointer re-typed; type-only conversion
        do
        {
            c = *(USHORT*)q; // pointer re-typed; type-only conversion
            c <<= 16;
            c >>= 24;
        } while (0);
        Loop_ctr = c;
        St_spr_pop();
        c &= 0x80;
        if (c != 0)
        {
            dy = ScratchPad->Spr.h;
            dx = 0;
        }
        else
        {
            dx = ScratchPad->Spr.w;
            dy = 0;
        }
        Loop_ctr &= 0x7F;
        do
        {
            GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 4);
            Loop_ctr--;
            ScratchPad->Spr.x += dx;
            ScratchPad->Spr.y += dy;
        } while (Loop_ctr != 0);
    } while (St.pFrm_rec > St_strip_tbl);

    if (St.Chr_flg & 2)
    {
        St.pFrm_rec = St_slot_tbl8;
    }
    else
    {
        St.pFrm_rec = St_slot_tbl6;
    }
    do
    {
        n = 9;
    } while (0);
    St.pFrm_rec += 0x7E;
    ppFrm3 = &St.pFrm_rec;
    do
    {
        *ppFrm3 -= 2;
        r = St.pFrm_rec;
        ScratchPad->Spr.attribute = ((*(SHORT*)r << 16) & 0xC00000) | 0x01000040; // pointer re-typed; type-only conversion
        do
        {
            c = *(USHORT*)r; // pointer re-typed; type-only conversion
            n--;
            c <<= 16;
            c >>= 24;
        } while (0);
        Loop_ctr = c;
        St_spr_pop();
        c &= 0x80;
        if (c != 0)
        {
            dx = 0;
            dy = ScratchPad->Spr.h;
        }
        else
        {
            dx = ScratchPad->Spr.w;
            dy = 0;
        }
        Loop_ctr &= 0x7F;
        do
        {
            GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 4);
            Loop_ctr--;
            ScratchPad->Spr.x += dx;
            ScratchPad->Spr.y += dy;
        } while (Loop_ctr != 0);
    } while (n != 0);

    ScratchPad->Rect.attribute = 0;
    ScratchPad->Rect.r = 0;
    ScratchPad->Rect.g = 0;
    ScratchPad->Rect.b = 0;
    St.pFrm_rec = (UCHAR*)St_slot_pos_tbl; // pointer re-typed; type-only conversion
    do
    {
        ScratchPad->Rect.h = ((USHORT*)St.pFrm_rec)[-1]; // pointer re-typed; type-only conversion
        ScratchPad->Rect.w = ((USHORT*)St.pFrm_rec)[-2]; // pointer re-typed; type-only conversion
        ScratchPad->Rect.y = ((USHORT*)St.pFrm_rec)[-3]; // pointer re-typed; type-only conversion
        ScratchPad->Rect.x = ((USHORT*)St.pFrm_rec)[-4]; // pointer re-typed; type-only conversion
        GsSortBoxFill(&ScratchPad->Rect, &MAIN.Ot0[G.Cbuf_id], 4);
    } while ((St.pFrm_rec -= 8) > St_box_tbl);
}

VOID St_spr_pop(VOID)
{
    UCHAR* p = ((STATUS*)(VOID*)&St)->pFrm_rec; // St read through a STATUS cast; plain St changes scheduling
    UCHAR** pp;

    do
    {
    } while (0);

    ScratchPad->Spr.v = *(p - 2);
    ScratchPad->Spr.u = *(p - 4);

    ScratchPad->Spr.h = *(USHORT*)(p - 6); // pointer re-typed; type-only conversion
    ScratchPad->Spr.w = *(USHORT*)(p - 8); // pointer re-typed; type-only conversion
    ScratchPad->Spr.y = *(USHORT*)(p - 10); // pointer re-typed; type-only conversion
    ScratchPad->Spr.x = *(USHORT*)(p - 12); // pointer re-typed; type-only conversion
    pp = &St.pFrm_rec;
    *pp -= 12;
}

VOID Print_num(UCHAR type, UCHAR num, UCHAR pri)
{
    UCHAR* pStr;
    UCHAR shown;
    UCHAR n;
    INT digit;
    INT small;
    UCHAR u;

    if ((type >= 0x13 || type == 1) && type != 0x2F)
    {
        return;
    }
    ScratchPad->Spr.cy = 0x1E4;
    ScratchPad->Spr.h = 8;
    ScratchPad->Spr.y += 0x14;

    if ((Flg_ck(&MAIN.SaveData.Scenario_flg, 0x7E) == 0 || type != 0xA) && // address passed as UINT
        (Flg_ck(&MAIN.SaveData.Scenario_flg, 0x7A) == 0 || type != 5)) // address passed as UINT
    {
        if (type != 6 && type < 0xB)
        {
            num &= 0x7F;
        }

        sprintf(ScratchPad->Text_buf, "%03d", num); // CHAR* for sprintf: Text_buf is a UCHAR buffer

        if (type >= 0xB)
        {
            ScratchPad->Spr.x += 0xE;
        }
        else
        {
            ScratchPad->Spr.x += 4;
        }

        ScratchPad->Spr.w = 8;

        switch (type)
        {
        case 8:
        case 13:
        case 17:
            u = 0x88;
            break;
        case 9:
        case 18:
            u = 0xC0;
            break;
        default:
            u = 0x80;
            break;
        }
        ScratchPad->Spr.u = u;
        pStr = ScratchPad->Text_buf;
        shown = 0;
        n = 3;

        small = type < 0xB;

        do
        {
            digit = *pStr & 0xF;
            n--;
            if (digit != 0 || shown || n == 0)
            {
                ScratchPad->Spr.v = digit * 8;
                GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], pri);
                shown = 1;
            }

            if (shown || !small)
            {
                ScratchPad->Spr.x += 8;
            }

            pStr++;
        } while (n);
    }
    else
    {
        ScratchPad->Spr.w = 0xA;
        ScratchPad->Spr.u = 0x98;
        ScratchPad->Spr.v = 0x70;

        ScratchPad->Spr.x += 6;

        GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], pri);
    }
}

VOID Item_model_load(VOID)
{
    if (St.Item_id != St.Res_id)
    {
        St.Res_id = St.Item_id;
        if ((St.Model_sts & ~0x80) != Item_data_tbl.items[St.Item_id - 1].model_id)
        {
            St.Model_sts = Item_data_tbl.items[St.Item_id - 1].model_id;

            Cd_read_wait(Item_model_tbl[St.Model_sts - 1], LOAD_BUF);

            G.Vram_cur.b.Cclut = 0x1C;
            G.Vram_cur.b.Ctpage = 0x15;
            Mapping_tim(LOAD_BUF);
            Mapping_tmd(2, ST_ITEM_TMD, 0x15, 0x1C);
            St.Model_sts |= 0x80;
        }
    }
}

// sprite records (x, y, w, h, u, v) of the four menu tab buttons
UCHAR St_tab_tbl[0x30] =
{
    0xD0,   // 0 //
    0x00,   // 1 //
    0x10,   // 2 //
    0x00,   // 3 //
    0x30,   // 4 //
    0x00,   // 5 //
    0x10,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x00,   // 12 //
    0x01,   // 13 //
    0x10,   // 14 //
    0x00,   // 15 //
    0x30,   // 16 //
    0x00,   // 17 //
    0x10,   // 18 //
    0x00,   // 19 //
    0x00,   // 20 //
    0x00,   // 21 //
    0x20,   // 22 //
    0x00,   // 23 //
    0xD0,   // 24 //
    0x00,   // 25 //
    0x20,   // 26 //
    0x00,   // 27 //
    0x30,   // 28 //
    0x00,   // 29 //
    0x10,   // 30 //
    0x00,   // 31 //
    0x00,   // 32 //
    0x00,   // 33 //
    0x10,   // 34 //
    0x00,   // 35 //
    0x00,   // 36 //
    0x01,   // 37 //
    0x20,   // 38 //
    0x00,   // 39 //
    0x30,   // 40 //
    0x00,   // 41 //
    0x10,   // 42 //
    0x00,   // 43 //
    0x00,   // 44 //
    0x00,   // 45 //
    0x30,   // 46 //
    0x00,   // 47 //
};

// four frame strip entries: sprite record plus flip and repeat word
UCHAR St_strip_tbl[0x38] =
{
    0x20,   // 0 //
    0x00,   // 1 //
    0x80,   // 2 //
    0x00,   // 3 //
    0xA0,   // 4 //
    0x00,   // 5 //
    0x0C,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x00,   // 9 //
    0x98,   // 10 //
    0x00,   // 11 //
    0x40,   // 12 //
    0x01,   // 13 //
    0xCC,   // 14 //
    0x00,   // 15 //
    0x10,   // 16 //
    0x00,   // 17 //
    0x04,   // 18 //
    0x00,   // 19 //
    0x10,   // 20 //
    0x00,   // 21 //
    0x78,   // 22 //
    0x00,   // 23 //
    0x88,   // 24 //
    0x00,   // 25 //
    0x00,   // 26 //
    0x82,   // 27 //
    0x30,   // 28 //
    0x01,   // 29 //
    0x10,   // 30 //
    0x00,   // 31 //
    0x04,   // 32 //
    0x00,   // 33 //
    0x10,   // 34 //
    0x00,   // 35 //
    0x7C,   // 36 //
    0x00,   // 37 //
    0x88,   // 38 //
    0x00,   // 39 //
    0x00,   // 40 //
    0x82,   // 41 //
    0xDC,   // 42 //
    0x00,   // 43 //
    0xB0,   // 44 //
    0x00,   // 45 //
    0x10,   // 46 //
    0x00,   // 47 //
    0x08,   // 48 //
    0x00,   // 49 //
    0x6C,   // 50 //
    0x00,   // 51 //
    0x80,   // 52 //
    0x00,   // 53 //
    0x00,   // 54 //
    0x05,   // 55 //
};

// nine slot frame tile entries for a six slot Item_work
UCHAR St_slot_tbl6[0x80] =
{
    0xD0,   // 0 //
    0x00,   // 1 //
    0x4E,   // 2 //
    0x00,   // 3 //
    0x08,   // 4 //
    0x00,   // 5 //
    0x08,   // 6 //
    0x00,   // 7 //
    0x60,   // 8 //
    0x00,   // 9 //
    0x58,   // 10 //
    0x00,   // 11 //
    0x00,   // 12 //
    0x01,   // 13 //
    0xD8,   // 14 //
    0x00,   // 15 //
    0x4E,   // 16 //
    0x00,   // 17 //
    0x04,   // 18 //
    0x00,   // 19 //
    0x08,   // 20 //
    0x00,   // 21 //
    0x68,   // 22 //
    0x00,   // 23 //
    0x58,   // 24 //
    0x00,   // 25 //
    0x00,   // 26 //
    0x01,   // 27 //
    0xDC,   // 28 //
    0x00,   // 29 //
    0x4E,   // 30 //
    0x00,   // 31 //
    0x10,   // 32 //
    0x00,   // 33 //
    0x08,   // 34 //
    0x00,   // 35 //
    0x6C,   // 36 //
    0x00,   // 37 //
    0x58,   // 38 //
    0x00,   // 39 //
    0x00,   // 40 //
    0x05,   // 41 //
    0x2C,   // 42 //
    0x01,   // 43 //
    0x4E,   // 44 //
    0x00,   // 45 //
    0x04,   // 46 //
    0x00,   // 47 //
    0x08,   // 48 //
    0x00,   // 49 //
    0x7C,   // 50 //
    0x00,   // 51 //
    0x58,   // 52 //
    0x00,   // 53 //
    0x00,   // 54 //
    0x01,   // 55 //
    0x30,   // 56 //
    0x01,   // 57 //
    0x4E,   // 58 //
    0x00,   // 59 //
    0x08,   // 60 //
    0x00,   // 61 //
    0x08,   // 62 //
    0x00,   // 63 //
    0x80,   // 64 //
    0x00,   // 65 //
    0x50,   // 66 //
    0x00,   // 67 //
    0x00,   // 68 //
    0x01,   // 69 //
    0xD0,   // 70 //
    0x00,   // 71 //
    0x56,   // 72 //
    0x00,   // 73 //
    0x08,   // 74 //
    0x00,   // 75 //
    0x0F,   // 76 //
    0x00,   // 77 //
    0xB8,   // 78 //
    0x00,   // 79 //
    0x28,   // 80 //
    0x00,   // 81 //
    0x00,   // 82 //
    0x84,   // 83 //
    0xD8,   // 84 //
    0x00,   // 85 //
    0x56,   // 86 //
    0x00,   // 87 //
    0x04,   // 88 //
    0x00,   // 89 //
    0x1E,   // 90 //
    0x00,   // 91 //
    0x68,   // 92 //
    0x00,   // 93 //
    0x60,   // 94 //
    0x00,   // 95 //
    0x00,   // 96 //
    0x83,   // 97 //
    0x2C,   // 98 //
    0x01,   // 99 //
    0x56,   // 100 //
    0x00,   // 101 //
    0x04,   // 102 //
    0x00,   // 103 //
    0x1E,   // 104 //
    0x00,   // 105 //
    0x7C,   // 106 //
    0x00,   // 107 //
    0x60,   // 108 //
    0x00,   // 109 //
    0x00,   // 110 //
    0x83,   // 111 //
    0x30,   // 112 //
    0x01,   // 113 //
    0x56,   // 114 //
    0x00,   // 115 //
    0x08,   // 116 //
    0x00,   // 117 //
    0x0F,   // 118 //
    0x00,   // 119 //
    0x60,   // 120 //
    0x00,   // 121 //
    0x60,   // 122 //
    0x00,   // 123 //
    0x00,   // 124 //
    0x86,   // 125 //
    0x00,   // 126 //
    0x00,   // 127 //
};

// nine slot frame tile entries for an eight slot Item_work
UCHAR St_slot_tbl8[0x80] =
{
    0xD0,   // 0 //
    0x00,   // 1 //
    0x30,   // 2 //
    0x00,   // 3 //
    0x08,   // 4 //
    0x00,   // 5 //
    0x08,   // 6 //
    0x00,   // 7 //
    0x60,   // 8 //
    0x00,   // 9 //
    0x58,   // 10 //
    0x00,   // 11 //
    0x00,   // 12 //
    0x01,   // 13 //
    0xD8,   // 14 //
    0x00,   // 15 //
    0x30,   // 16 //
    0x00,   // 17 //
    0x04,   // 18 //
    0x00,   // 19 //
    0x08,   // 20 //
    0x00,   // 21 //
    0x68,   // 22 //
    0x00,   // 23 //
    0x58,   // 24 //
    0x00,   // 25 //
    0x00,   // 26 //
    0x01,   // 27 //
    0xDC,   // 28 //
    0x00,   // 29 //
    0x30,   // 30 //
    0x00,   // 31 //
    0x10,   // 32 //
    0x00,   // 33 //
    0x08,   // 34 //
    0x00,   // 35 //
    0x6C,   // 36 //
    0x00,   // 37 //
    0x58,   // 38 //
    0x00,   // 39 //
    0x00,   // 40 //
    0x05,   // 41 //
    0x2C,   // 42 //
    0x01,   // 43 //
    0x30,   // 44 //
    0x00,   // 45 //
    0x04,   // 46 //
    0x00,   // 47 //
    0x08,   // 48 //
    0x00,   // 49 //
    0x7C,   // 50 //
    0x00,   // 51 //
    0x58,   // 52 //
    0x00,   // 53 //
    0x00,   // 54 //
    0x01,   // 55 //
    0x30,   // 56 //
    0x01,   // 57 //
    0x30,   // 58 //
    0x00,   // 59 //
    0x08,   // 60 //
    0x00,   // 61 //
    0x08,   // 62 //
    0x00,   // 63 //
    0x80,   // 64 //
    0x00,   // 65 //
    0x50,   // 66 //
    0x00,   // 67 //
    0x00,   // 68 //
    0x01,   // 69 //
    0xD0,   // 70 //
    0x00,   // 71 //
    0x38,   // 72 //
    0x00,   // 73 //
    0x08,   // 74 //
    0x00,   // 75 //
    0x0F,   // 76 //
    0x00,   // 77 //
    0xB8,   // 78 //
    0x00,   // 79 //
    0x28,   // 80 //
    0x00,   // 81 //
    0x00,   // 82 //
    0x86,   // 83 //
    0xD8,   // 84 //
    0x00,   // 85 //
    0x38,   // 86 //
    0x00,   // 87 //
    0x04,   // 88 //
    0x00,   // 89 //
    0x1E,   // 90 //
    0x00,   // 91 //
    0x68,   // 92 //
    0x00,   // 93 //
    0x60,   // 94 //
    0x00,   // 95 //
    0x00,   // 96 //
    0x84,   // 97 //
    0x2C,   // 98 //
    0x01,   // 99 //
    0x38,   // 100 //
    0x00,   // 101 //
    0x04,   // 102 //
    0x00,   // 103 //
    0x1E,   // 104 //
    0x00,   // 105 //
    0x7C,   // 106 //
    0x00,   // 107 //
    0x60,   // 108 //
    0x00,   // 109 //
    0x00,   // 110 //
    0x84,   // 111 //
    0x30,   // 112 //
    0x01,   // 113 //
    0x38,   // 114 //
    0x00,   // 115 //
    0x08,   // 116 //
    0x00,   // 117 //
    0x0F,   // 118 //
    0x00,   // 119 //
    0x60,   // 120 //
    0x00,   // 121 //
    0x60,   // 122 //
    0x00,   // 123 //
    0x00,   // 124 //
    0x88,   // 125 //
    0x00,   // 126 //
    0x00,   // 127 //
};

// four backdrop box fill rectangles (x, y, w, h)
UCHAR St_box_tbl[0x20] =
{
    0x00,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x00,   // 3 //
    0x40,   // 4 //
    0x01,   // 5 //
    0x0C,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x00,   // 9 //
    0x0C,   // 10 //
    0x00,   // 11 //
    0x09,   // 12 //
    0x00,   // 13 //
    0x80,   // 14 //
    0x00,   // 15 //
    0xC7,   // 16 //
    0x00,   // 17 //
    0x0C,   // 18 //
    0x00,   // 19 //
    0x79,   // 20 //
    0x00,   // 21 //
    0x80,   // 22 //
    0x00,   // 23 //
    0x00,   // 24 //
    0x00,   // 25 //
    0x8C,   // 26 //
    0x00,   // 27 //
    0x40,   // 28 //
    0x01,   // 29 //
    0x64,   // 30 //
    0x00,   // 31 //
};

// screen position (x then y) of each menu cursor cell, then four trailing values
USHORT St_slot_pos_tbl[28] =
{
    0x00D0,   // 0 //
    0x0010,   // 1 //
    0x0100,   // 2 //
    0x0010,   // 3 //
    0x00D0,   // 4 //
    0x0020,   // 5 //
    0x0100,   // 6 //
    0x0020,   // 7 //
    0x00DC,   // 8 //
    0x0038,   // 9 //
    0x0104,   // 10 //
    0x0038,   // 11 //
    0x00DC,   // 12 //
    0x0056,   // 13 //
    0x0104,   // 14 //
    0x0056,   // 15 //
    0x00DC,   // 16 //
    0x0074,   // 17 //
    0x0104,   // 18 //
    0x0074,   // 19 //
    0x00DC,   // 20 //
    0x0092,   // 21 //
    0x0104,   // 22 //
    0x0092,   // 23 //
    0x0201,   // 24 //
    0x0403,   // 25 //
    0x0004,   // 26 //
    0x0000,   // 27 //
};
