/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ST_SUB03.C
 */

#include "TYPES.H"
#include "ST_SUB03.H"
#include "GAME.H"
#include "INVENTORY.H"
#include "ITEM.H"
#include "MOJIDISP.H"
#include "SCE1.H"
#include "SND.H"
#include "ST_SUB00.H"
#include "ST_SUB02.H"
#include "SCRATCHPAD.H"
#include "FILECODE.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "SCENARIO.H"
#include "ST_SUB01.H"
#include "SUB.H"

// functions without a C definition
INT Mess_set();

ITEM_WORK* pItem_work_base;   // item slots of the active character

VOID St_com_mix(VOID)
{
    UCHAR n;
    USHORT y;
    UCHAR* p1;
    UCHAR* p2;
    UCHAR* p3;
    UCHAR* p5;
    UCHAR* pAnm;
    UCHAR* pFlg;
    UCHAR* pFlg2;

    if (St.Act_sts != 0)
    {
        n = (St.Icursol_sub >> 1) - 4;
    }
    else
    {
        n = (St.Icursol_main >> 1) - 4;
    }

    switch (St.Act_sts)
    {
    case 0:
        St.Act_sts = 1;
        St.Move_anm[1] = 0;
        St.Move_anm[2] = 0;
        St.Sub_wait = 0xFF;
        St.Icursol_sub = St.Icursol_main;
        break;
    case 1:
        p1 = &St.Move_anm[1];
        *p1 += 4;
        St.Move_anm[2] += 3;
        if (*p1 != 0x10)
        {
            break;
        }
        St.Act_sts = 2;
        St.St_flg &= 0xF7;

    case 2:
        if (!(G.SaveData.Key_trg & 0x8000))
        {
            if (G.SaveData.Key_trg & 0x4000)
            {
                if (St.Icursol_main != St.Icursol_sub &&
                    n < G.SaveData.Item_num &&
                    Item_data_tbl.items[St.Item_id - 1].Mix_idx >= 0)
                {
                    switch ((UCHAR)Check_item_mix())  // truncates to a byte
                    {
                    case 4:
                        St.Act_sts = 3;
                        St.St_flg |= 8;
                        St_sel_item_set();
                        break;
                    case 1:
                        St.Act_sts = 4;
                        St.Act_res = 1;

                        Mess_set(0xF1, 0);

                        break;
                    case 2:
                        St.Act_sts = 4;
                        St.Act_res = 4;

                        Mess_set(0xF4, 0);

                        break;
                    case 3:
                        St.Act_sts = 4;
                        St.Act_res = 5;

                        Mess_set(0xF5, 0);

                        break;
                    }
                    St.Sub_wait = 0;
                    break;
                }
            }
            else
            {
                if (PADD1_TRG_CUR & (PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT))
                {
                    if (MAIN.Padd1_trg & (PAD_RIGHT | PAD_LEFT))
                    {
                        p2 = &St.Icursol_sub;
                        *p2 ^= 2;
                    }
                    if (MAIN.Padd1_trg & PAD_UP)
                    {
                        p3 = &St.Icursol_sub;
                        *p3 -= 4;
                        if (!(*p3 & 0xF8))
                        {
                            *p3 &= 2;
                            *p3 |= (St.nSlot + 2) * 2;
                        }
                    }
                    else if (MAIN.Padd1_trg & PAD_DOWN)
                    {
                        St.Icursol_sub += 4;
                        if (St.Icursol_sub > (St.nSlot + 3) * 2)
                        {
                            St.Icursol_sub = (St.Icursol_sub & 2) | 8;
                        }
                    }
                    St.Sub_wait = 0;
                    Snd_se_play(SE_BANK_ENEMY, 4, 0);
                }
            }
            p5 = &St.Sub_wait;
            (*p5)--;
            break;
        }
        St.Act_sts = 3;
        St.Sub_wait = 0;
        Snd_se_play(SE_BANK_ENEMY, 5, 0);

    case 3:
        pAnm = &St.Move_anm[1];
        *pAnm -= 4;
        St.Move_anm[2] -= 3;
        if (*pAnm != 0)
        {
            break;
        }
        if (St.Act_res != 0)
        {
            St.Act_sts = 4;

            Mess_set((*(volatile UCHAR*)&St.Act_res) + 0xF0, 0);  // UCHAR* view of the field (a plain access changes the code)

            break;
        }
        if (St.St_flg & 8)
        {
            St.St_flg |= 2;
        }
        else
        {
            St.St_flg |= 4;
        }
        break;
    case 4:
        if (G.SaveData.Mess_flg & MESS_FLG_ACTIVE)
        {
            break;
        }
        switch (St.Act_res)
        {
        case 2:
            pFlg = &St.St_flg;
            *pFlg |= 0x40;

            return;

        case 3:
            pFlg2 = &St.St_flg;
            *pFlg2 |= 2;

            break;
        case 1:
            St.Act_sts = 3;

            break;
        case 4:
            St_mix_set();
            break;
        case 5:
            St.Act_sts = 2;
            break;
        }
        St.Act_res = 0;
        break;
    }
    if (St.Act_res == 0)
    {
        if (St.Act_sts != 3)
        {
            if (n < G.SaveData.Item_num)
            {
                Name_disp(pItem_work_base[n].Id, 0, (USHORT)(0x30 - (USHORT)Scr_ofs[0]), (USHORT)(0xBA - (USHORT)Scr_ofs[1]));
            }
        }
        else
        {
            Name_disp(St.Item_id, 0, (USHORT)(0x30 - (USHORT)Scr_ofs[0]), (USHORT)(0xBA - (USHORT)Scr_ofs[1]));
        }
    }
    if (St.Act_sts != 4 || St.Move_anm[1] != 0)
    {
        ScratchPad->Spr.attribute = 0x1000040;
        ScratchPad->Spr.tpage = 0x1C;
        ScratchPad->Spr.cy = 0x1E4;
        ScratchPad->Spr.u = 0x6C;
        ScratchPad->Spr.cx = 0;
        ScratchPad->Spr.v = 0x60;
        if (!(St.Sub_wait & 0x20))
        {
            ScratchPad->Spr.u = 0x74;
        }
        ScratchPad->Spr.w = 4;
        ScratchPad->Spr.h = 4;
        ScratchPad->Spr.x = St_slot_pos_tbl[St.Icursol_sub] + 0x12;
        {
            INT ofs_y = St.Move_anm[2] - 0xD;
            ScratchPad->Spr.y = y = (&St_slot_pos_tbl[1])[St.Icursol_sub] - ofs_y;
        }
        if (!(St.Chr_flg & 2))
        {
            ScratchPad->Spr.y = y + 0x1E;
        }
        GsSortFastSprite(&ScratchPad->Spr, MAIN.Ot0 + CBUF_ID_CUR, 4);
        ScratchPad->Spr.v += 8;
        ScratchPad->Spr.y += St.Move_anm[2] * 2;
        GsSortFastSprite(&ScratchPad->Spr, MAIN.Ot0 + MAIN.Cbuf_id, 4);
        ScratchPad->Spr.v += 8;
        ScratchPad->Spr.x -= St.Move_anm[1];
        y = (&St_slot_pos_tbl[1])[St.Icursol_sub];
        ScratchPad->Spr.y = y + 0xD;
        if (!(St.Chr_flg & 2))
        {
            ScratchPad->Spr.y = y + 0x2B;
        }
        GsSortFastSprite(&ScratchPad->Spr, MAIN.Ot0 + MAIN.Cbuf_id, 4);
        ScratchPad->Spr.v += 8;
        ScratchPad->Spr.x += St.Move_anm[1] * 2;
        GsSortFastSprite(&ScratchPad->Spr, MAIN.Ot0 + MAIN.Cbuf_id, 4);
    }
}

INT Check_item_mix(VOID)
{
    UCHAR* pMix;
    UCHAR Slot_m;
    UCHAR Slot_s;
    UCHAR id_m;
    UCHAR id_s;
    UCHAR mix_max;
    UCHAR op;
    INT pRet;
    UINT t;

    Slot_m = (St.Icursol_main >> 1) - 4;
    Slot_s = (St.Icursol_sub >> 1) - 4;
    pMix = Item_mix_tbl[Item_data_tbl.items[St.Item_id - 1].Mix_idx];
    id_m = pItem_work_base[Slot_m].Id;
    id_s = pItem_work_base[Slot_s].Id;
    for (mix_max = *pMix++; mix_max != 0; mix_max--, pMix += 4)
    {
        if (id_s != pMix[0])
        {
            continue;
        }

        op = pMix[3];
        if (op >= 6)
        {
            UCHAR Loop_ctr;
            if (op == 6)
            {
                Loop_ctr = pItem_work_base[Slot_m].Num;
            }
            else
            {
                Loop_ctr = pItem_work_base[Slot_s].Num;
            }
            if (Loop_ctr != 0)
            {
                return 0;
            }
        }

        Snd_se_play(SE_BANK_ENEMY, 6, 0);
        if ((UCHAR)(id_m - 0x13) < 8)  // truncates to a byte
        {
            if ((t = MAIN.SaveData.Room_no) != 9)
            {
                return 1;
            }
            if (G.SaveData.Stage_no != 3)
            {
                return 1;
            }
        }
        else if ((UCHAR)(id_m - 0x43) < 9)  // truncates to a byte
        {
            return 2;
        }
        pMix++;
        pItem_work_base[Slot_m].Id = *pMix++;
        pItem_work_base[Slot_s].Id = *pMix;
        if (pMix[1] != 0)
        {
            ((VOID (**)(UCHAR, UCHAR))St_mix_tbl)[pMix[1]](Slot_m, Slot_s);  // the handlers are declared with other prototypes: the table is VOID*
        }
        St_mix_pix(Slot_m, Slot_s, id_m, id_s);
        Sort_item();
        return 4;
    }
    pRet = 0;
    t = id_m - 0x43;

    t = t < 9;

    if (t)
    {
        pRet = -((UINT)(id_s - 0x43) < 9) & 3;  // unsigned conversion (signed operand)
    }
end:
    return pRet;
}

VOID St_mix_set(VOID)
{
    UCHAR Slot_m;
    UCHAR Slot_s;
    UCHAR id_m;
    UCHAR id_s;
    UCHAR* pMix;
    UCHAR* pMix_tbl;

    if (G.SaveData.Mess_flg != 0)
    {
        Snd_se_play(SE_BANK_ENEMY, 5, 0);
        St.Act_sts = 2;
        return;
    }
    Snd_se_play(SE_BANK_ENEMY, 6, 0);
    St.Act_sts = 3;
    Slot_m = (St.Icursol_main >> 1) - 4;
    Slot_s = (St.Icursol_sub >> 1) - 4;
    St.St_flg |= 8;
    pMix_tbl = Item_mix_tbl[Item_data_tbl.items[St.Item_id - 1].Mix_idx];
    id_m = pItem_work_base[Slot_m].Id;
    id_s = pItem_work_base[Slot_s].Id;
    pMix = pMix_tbl + 1;
    while (id_s != *pMix)
    {
        pMix += 4;
    }
    pMix++;
    pItem_work_base[Slot_m].Id = pMix[0];
    pItem_work_base[Slot_s].Id = pMix[1];
    St_mix_pix(Slot_m, Slot_s, id_m, id_s);
    Sort_item();
    St_sel_item_set();
}

VOID St_mix01(INT Dst_slot, INT Src_slot)
{
    UCHAR max;
    UINT total;
    UINT Dst_cnt;
    ITEM_WORK* dst;

    dst = &pItem_work_base[Dst_slot & 0xFF];

    Dst_cnt = dst->Num & 0x7F;

    total = pItem_work_base[Src_slot & 0xFF].Num + Dst_cnt;

    max = Item_data_tbl.items[dst->Id - 1].Item_max;

    if (max < total)
    {
        dst->Num = max;
        pItem_work_base[Src_slot & 0xFF].Num = total - max;

        return;
    }

    dst->Num = total;
    pItem_work_base[Src_slot & 0xFF].Id = 0;
}

VOID St_mix02(INT Src_slot, INT Dst_slot)
{
    UCHAR max;
    UINT total;
    UINT Dst_cnt;
    ITEM_WORK* dst;

    dst = &pItem_work_base[Dst_slot & 0xFF];

    Dst_cnt = dst->Num & 0x7F;
    total = Dst_cnt + pItem_work_base[Src_slot & 0xFF].Num;

    max = Item_data_tbl.items[dst->Id - 1].Item_max;

    if (max < total)
    {
        dst->Num = max;
        pItem_work_base[Src_slot & 0xFF].Num = total - max;

        return;
    }
    dst->Num = total;
    pItem_work_base[Src_slot & 0xFF].Id = 0;
}

VOID St_mix03(INT Dst_slot, INT Src_slot)
{
    UINT total;
    ITEM_WORK* dst;

    dst = &pItem_work_base[Dst_slot & 0xFF];

    total = pItem_work_base[Src_slot & 0xFF].Num + dst->Num;
    if (total >= 0xFBU)
    {
        dst->Num = 0xFAU;
        pItem_work_base[Src_slot & 0xFF].Num = total + 6;

        return;
    }

    dst->Num = total;
    pItem_work_base[Src_slot & 0xFF].Id = 0;
}

VOID St_mix04(VOID)
{
    St.Act_res = 2;
    Flg_on(&MAIN.SaveData.Scenario_flg, 0x16);  // address passed as an integer
}

VOID St_mix05(VOID)
{
    St.Act_res = 3;
}

VOID St_mix06(INT Dst_slot, INT Src_slot)
{
    INT Src_ofs;
    UCHAR max;
    UCHAR Src_cnt;
    INT one;
    ITEM_WORK* dst;
    ITEM_WORK* src;

    Src_ofs = (Src_slot & 0xFF) * 2;
    dst = &pItem_work_base[Dst_slot & 0xFF];
    one = 1;
    Src_cnt = *((SCHAR*)(Src_ofs + (INT)pItem_work_base) + one);  // byte view of the same memory
    max = Item_data_tbl.items[dst->Id - one].Item_max;

    if (max < Src_cnt)
    {
        dst->Num = max;
        src = &pItem_work_base[Src_slot & 0xFF];
        if (one)
        {
            src->Num = src->Num - max;
            return;
        }
    }

    dst->Num = Src_cnt;
    pItem_work_base[Src_slot & 0xFF].Id = 0;
}

VOID St_mix07(INT Src_slot, INT Dst_slot)
{
    INT Src_ofs;
    UCHAR max;
    UCHAR Src_cnt;
    ITEM_WORK* dst;
    ITEM_WORK* src;

    Src_ofs = (Src_slot & 0xFF) << 1;
    dst = &pItem_work_base[Dst_slot & 0xFF];
    Src_cnt = ((ITEM_WORK*)(Src_ofs + (INT)pItem_work_base))->Num;  // reinterprets the memory as ITEM_WORK
    max = Item_data_tbl.items[dst->Id - 1].Item_max;

    if (max < Src_cnt)
    {
        dst->Num = max;
        src = BYTE_OFS(ITEM_WORK, pItem_work_base, Src_ofs);
        src->Num -= max;

        return;
    }

    dst->Num = Src_cnt;
    *(SCHAR*)(Src_ofs + (INT)pItem_work_base) = 0;  // real width or sign conversion
}

VOID St_mix_pix(UCHAR Slot_a, UCHAR Slot_b, UCHAR Old_a, UCHAR Old_b)
{
    ITEM_WORK* pSlot_a;
    ITEM_WORK* pSlot_b;
    UCHAR Img_a;
    UCHAR Img_b;
    UCHAR id;

    Img_a = 0;

    pSlot_a = &pItem_work_base[Slot_a];

    Img_b = 0;

    id = pSlot_a->Id;

    if (id != 0 && id != Old_a)
    {
        Img_a = Item_img_tbl[Item_data_tbl.items[pSlot_a->Id - 1].Mix_idx];
    }

    pSlot_b = &pItem_work_base[Slot_b];
    id = pSlot_b->Id;

    if (id != 0 && id != Old_b)
    {
        Img_b = Item_img_tbl[Item_data_tbl.items[pSlot_b->Id - 1].Mix_idx];
    }
    if (((St.Model_sts + 1) & ~0x80) && (Img_a | Img_b))
    {
        St.Model_sts |= 0x7F;

        Cd_read_wait(ITEM_MIX_PIX, LOAD_BUF);
    }
    if (Img_a)
    {
        Pix_trans(Img_a - 1, Item_pix_idx[Slot_a & 0xFF], LOAD_BUF);
    }
    if (Img_b)
    {
        Pix_trans(Img_b - 1, Item_pix_idx[Slot_b & 0xFF], LOAD_BUF);
    }
}

VOID Door_texture_load(VOID)
{
    Cd_read((USHORT)Dtex_file[(SCHAR)Door_anim_id[1]], LOAD_BUF);  // byte view of the same memory
    G.Vram_cur.Tpage_clut = 0x1F15;
    Mapping_tim((*(INT*)LOAD_TIM_OFS_ADDR) + (UCHAR*)LOAD_BUF);  // word view of the same memory
}

VOID Memcpy(VOID* pDst, VOID* pSrc, INT r)
{
    UCHAR* p = pDst;
    UCHAR* q = pSrc;

    while (r-- > 0)
    {
        *p++ = *q++;
    }
}

VOID Vram_clr(SHORT x, SHORT y, SHORT w, SHORT h)
{
    RECT Rect;

    Rect.x = x;
    Rect.y = y;
    Rect.w = w;
    Rect.h = h;
    ClearImage(&Rect, 0U, 0U, 0U);
}

// combine effect handlers, indexed by the effect number of a combine entry
VOID* St_mix_tbl[8] =
{
    St_item_use_none9,      // 0 //
    St_mix01,    // 1 //
    St_mix02,    // 2 //
    St_mix03,         // 3 //
    St_mix04,          // 4 //
    St_mix05,          // 5 //
    St_mix06,   // 6 //
    St_mix07,   // 7 //
};

// door texture file codes, indexed by door texture index
SHORT Dtex_file[34] =
{
    DOOR00_DOR,   // 0 //
    DOOR01_DOR,   // 1 //
    DOOR02_DOR,   // 2 //
    DOOR03_DOR,   // 3 //
    DOOR04_DOR,   // 4 //
    DOOR05_DOR,   // 5 //
    DOOR06_DOR,   // 6 //
    DOOR07_DOR,   // 7 //
    DOOR08_DOR,   // 8 //
    DOOR09_DOR,   // 9 //
    DOOR10_DOR,   // 10 //
    DOOR11_DOR,   // 11 //
    DOOR12_DOR,   // 12 //
    DOOR13_DOR,   // 13 //
    DOOR14_DOR,   // 14 //
    MON_DOR,   // 15 //
    ELE03_DOR,   // 16 //
    ELE01_DOR,   // 17 //
    ELE01A_DOR,   // 18 //
    ELE01B_DOR,   // 19 //
    ELE02_DOR,   // 20 //
    ELE04_DOR,   // 21 //
    KAI01_DOR,   // 22 //
    KAI03_DOR,   // 23 //
    KAI02_DOR,   // 24 //
    KAI04_DOR,   // 25 //
    LAD00_DOR,   // 26 //
    LAD01_DOR,   // 27 //
    DOOR00K_DOR,   // 28 //
    DOOR01K_DOR,   // 29 //
    DOOR03K_DOR,   // 30 //
    DOOR05K_DOR,   // 31 //
    DOOR06K_DOR,   // 32 //
    DOOR15_DOR,   // 33 //
};
