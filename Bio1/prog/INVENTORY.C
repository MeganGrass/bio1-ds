/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\INVENTORY.C
 */

#include "TYPES.H"

#include "INVENTORY.H"
#include "EFF1.H"
#include "GAME.H"
#include "ITEM.H"
#include "MOJIDISP.H"
#include "SCE0.H"
#include "SCE1.H"
#include "SCE_AT.H"
#include "SND.H"
#include "ST_SUB01.H"
#include "ST_SUB03.H"
#include "SUB.H"
#include "FILECODE.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "SCENARIO.H"
#include "MONITOR.H"
#include "ST_SUB00.H"

UCHAR Item_pix_mask;           // bit mask of the item icon cells in use
ITEM_WORK* pItem_found; // Item_work slot found by the last Search_item_id
UCHAR Item_pix_idx[8];        // icon cell assigned to each Item_work slot

VOID Status_init(VOID)
{
    SCHAR init_chris[5] = { 0x01, 0x00, 0x41, 0x01, 0x00 };
    SCHAR init_jill[7] = { 0x01, 0x00, 0x02, 0x0F, 0x41, 0x01, 0x00 };

    INT flag;

    UCHAR* pInit;
    UCHAR i;
    UCHAR max;

    flag = G.Status_flg & STAT_ARRANGE;
    if (flag != 0)
    {
        init_jill[2] = 4;
    }

    ((INT*)&G.SaveData.Item_flg[0])[0] = 0xBFFFFFFF;  // word view of the same memory
    ((INT*)&G.SaveData.Item_flg[0])[1] = -1;  // word view of the same memory
    ((INT*)&G.SaveData.Item_flg[0])[2] = -1;  // word view of the same memory
    ((INT*)&G.SaveData.Item_flg[0])[3] = -1;  // word view of the same memory
    ((INT*)&G.SaveData.Item_flg[0])[4] = -1;  // word view of the same memory
    ((INT*)&G.SaveData.Item_flg[0])[6] = -1;  // word view of the same memory
    ((INT*)&G.SaveData.Item_flg[0])[7] = -1;  // word view of the same memory

    G.SaveData.Unk_20c[1] = 0xF0;
    G.SaveData.Unk_20c[2] = 0xF0;

    ((INT*)&G.SaveData.Item_flg[0])[5] = 0xFFF7FFFF;  // word view of the same memory

    G.SaveData.Unk_20c[0] = 7;

    if (G.Player.Id & 3)
    {
        pInit = init_jill;
        max = 8;
    }
    else
    {
        pInit = init_chris;
        max = 6;

        G.SaveData.Item_work2[0].Id = flag != 0 ? 4 : 2;

        G.SaveData.Item_work2[0].Num = 0xF;
    }
    i = 0;
    while (*pInit != 0)
    {
        ITEM_WORK_TBL[i].Id = *pInit++;
        MAIN.SaveData.Item_work[i].Num = *pInit++;
        Item_pix_idx[i] = i;
        i++;
    }
    Item_pix_mask = (1 << i) - 1;
    G.SaveData.Item_num = i;
    for (; i < max; i++)
    {
        ITEM_WORK_TBL[i].Id = 0;
        MAIN.SaveData.Item_work[i].Num = 0;
    }
}

VOID Item_lost(UCHAR* pSlot)
{
    UCHAR n;

    n = ((UINT)((UCHAR*)pItem_work_base + G.SaveData.Item_num * 2 - pSlot) >> 1) - 1;  // unsigned conversion (signed operand)
    for (; n != 0; n--)
    {
        pSlot[0] = pSlot[2];
        pSlot[1] = pSlot[3];
        pSlot += 2;
    }
    pSlot[0] = 0;
    pSlot[1] = 0;
}

VOID Sort_item(VOID)
{
    UCHAR sel;
    UCHAR n;
    UCHAR total;
    UCHAR j;
    UCHAR k;
    UCHAR i;

    sel = G.SaveData.Equip_no - 1;
    n = 8;
    if ((G.Player.Id & 3) != 1)
    {
        n = 6;
    }
    total = n;
    j = 0;
    k = 0;
    do
    {
        i = j;
        if (pItem_work_base[i].Id != 0)
        {
            if (i != k)
            {
                pItem_work_base[k].Id = pItem_work_base[i].Id;
                pItem_work_base[k].Num = pItem_work_base[i].Num;
                Item_pix_idx[k] = Item_pix_idx[i];
                if (i == sel)
                {
                    sel = k;
                }
            }
            k++;
        }
        else
        {
            if (i < ITEM_NUM_CUR)
            {
                Item_pix_mask &= ~(1 << Item_pix_idx[i]);
            }
        }
        j++;
        n--;
    } while (n != 0);
    total -= k;
    G.SaveData.Equip_no = sel + 1;
    G.SaveData.Item_num = k;
    while (total)
    {
        i = k;
        pItem_work_base[i].Id = 0;
        pItem_work_base[i].Num = 0;
        k++;
        total--;
    }
}

VOID Item_num_set(VOID)
{
    UCHAR max;
    UCHAR n;

    max = 8;
    if ((G.Player.Id & 3) != 1)
    {
        max = 6;
    }
    for (n = 0; pItem_work_base[n].Id != 0; n++)
    {
        if (n >= max)
        {
            break;
        }
    }
    G.SaveData.Item_num = n;
}

VOID Status_init_m(VOID* Img_buf)
{
    UCHAR i;
    INT j;

    Cd_read(ITEM_ALL_PIX, Img_buf);

    Item_num_set();
    i = G.SaveData.Item_num;
    Item_pix_mask = (1 << i) - 1;

    if (i != 0)
    {
        i--;

    loop:
        do
        {
            j = i;
            Item_pix_idx[j] = i;
            Pix_trans(Item_data_tbl.items[(pItem_work_base[j].Id - 1)].model_id - 1, j, Img_buf);
        } while (0);
        i--;
        if (j != 0)
        {
            goto loop;
        }
    }
}

UINT Search_item_id(INT id)
{
    UINT i;

    for (i = 0; i < G.SaveData.Item_num; i++)
    {
        if (id == pItem_work_base[i].Id)
        {
            pItem_found = &pItem_work_base[i];
            return i;
        }
    }
    pItem_found = (ITEM_WORK*)&MAIN.Def_item;
    return -1;
}

VOID St_get_item(VOID)
{
    SCE_AOT* pAct;

    SEARCH_AOT* pZone;
    ITEM_WORK* pSlot;

    INT num;

    UCHAR id;
    UCHAR n;
    UCHAR i;
    UCHAR bit;
    UCHAR limit;
    UINT total;

    pAct = pAot_data;
    pAct->Id = 0;
    pOm_tbl[pAct->pRec->Model_no]->Be_flg = 0;
    if (pOm_tbl[pAct->pRec->Model_no]->Push_cnt != 0)
    {
        ESP_FREE_CNT++;

        Memclr((UINT*)(&MAIN.Esp[pOm_tbl[pAct->pRec->Model_no]->Push_cnt]), 0x21);  // word view of the same memory
    }
    Flg_off(&MAIN.SaveData.Item_flg[0], pAct->pRec->Item_flg_no);
    GET_ID_CUR = pAct->pRec->Item_id;

    pZone = pAct->pRec;
    num = pZone->Item_num;
    id = pZone->Item_id;

    if (G.SaveData.Sel_id == 0x2F)
    {
        num = 2;
        if (G.Status_flg & STAT_ARRANGE)
        {
            num = 3;
        }
    }

    if ((G.Status_flg & STAT_EASY_MODE) || (STATUS_FLG_NOW & STAT_ARRANGE_EASY))
    {
        if ((UCHAR)(SEL_ID_CUR - 11) < 8U || MAIN.SaveData.Sel_id == 0x2F)  // UCHAR* view of the field (a plain access changes the code)
        {
            num = (UCHAR)num * 2;  // truncates to a byte
        }
    }

    n = 8;

    if ((UCHAR)(SEL_ID_CUR - 11) < 8U || MAIN.SaveData.Sel_id == 0x2F)  // UCHAR* view of the field (a plain access changes the code)
    {
        if ((G.Player.Id & 3) != 1)
        {
            n = 6;
        }
        i = 0;
        limit = n;
        do
        {
            pSlot = &pItem_work_base[i];
            if (pSlot->Id == id)
            {
                total = (UCHAR)num + pSlot->Num;  // truncates to a byte

                if (total < 251)
                {
                    pSlot->Num = total;

                    return;
                }
                if (ITEM_NUM_CUR < limit)
                {
                    pSlot->Num = 250;

                    num = total + 6;
                    break;
                }
            }
            i++;
        } while (--n);
    }
    pItem_work_base[ITEM_NUM_CUR].Id = id;
    pItem_work_base[MAIN.SaveData.Item_num].Num = num;
    bit = 0;
    while ((Item_pix_mask >> bit) & 1)
    {
        bit++;
    }
    do
    {
        total = 1;
        Item_pix_idx[ITEM_NUM_CUR] = bit;
    } while (0);
    Item_pix_mask |= total << bit;

    Pix_trans(pAot_data->pRec->Model_no, bit, G.pRoom->pRaw, num);  // passes 4 arguments, Pix_trans declares 3

    ITEM_NUM_CUR++;
}

VOID Item_aot_ret(VOID)
{
    SCE_AOT* pAct;

    pAct = pAot_data;
    pAct->Id = 0;
    pAct->pRec->Item_num = pItem_found->Num;
    pOm_tbl[pAct->pRec->Model_no]->Be_flg = 1;
    Flg_on(&MAIN.SaveData.Item_flg, pAct->pRec->Item_flg_no);  // address passed as an integer
    G.SaveData.Use_id = pAct->pRec->Item_id;
    pItem_found->Id = 0;
    pItem_found->Num = 0U;
    Item_lost((UCHAR*)pItem_found);  // byte view of the same memory
    VIEW(SCHAR, MAIN.SaveData.Item_num) -= 1;  // SCHAR* view of the field (a plain access changes the code)
}

INT St_main01(VOID)
{
    UCHAR slot;
    UCHAR box_id;
    UCHAR Box_num;
    INT new_id;
    UCHAR n;
    UCHAR* pRno;
    UCHAR* pCur1;
    UCHAR* pCur2;
    UCHAR* pCur3;
    UCHAR* b0;
    UCHAR* pBox;

    UCHAR* pRow1;
    UCHAR* pRow2;

    UCHAR* pBox_tbl;
    USHORT* pPadd;

    if (G_BASE->Padd1_trg & PAD_START)
    {
        goto cancel;
    }
    pRno = &St.Sub_rtn0;
    switch (*pRno)
    {
    case 0:
        *pRno = 1;
        St.Ibox_sel = 0;
        St.Blink_timer = 0;
        St_sel_item_set();
    case 1:
        if (G.SaveData.Key_trg & 0x8000)
        {
        cancel:

            Snd_se_play(SE_BANK_ENEMY, 5, 0);
            return 1;
        }
        if (G.SaveData.Key_trg & 0x4000)
        {
            Snd_se_play(SE_BANK_ENEMY, 6, 0);
            if (!(St.Icursol_main & 0xF8))
            {
                return 1;
            }
            St.Sub_rtn0 = 2;
            St.Sub_sts = 0;
            St.Blink_timer = 0;
            (&St.Sub_wait)[0] = 0xF;
            (&St.Sub_wait)[1] = 0;
            St.Move_anm[0] = 0;
            break;
        }
        if (G.Padd1_trg & (PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT))
        {
            if (G.Padd1_trg & (PAD_RIGHT | PAD_LEFT))
            {
                pCur1 = &St.Icursol_main;
                if (*pCur1 & 0xF8)
                {
                    *pCur1 ^= 2;
                }
            }
            if (G.Padd1_trg & PAD_UP)
            {
                pCur2 = &St.Icursol_main;
                *pCur2 -= 4;
                if (!(*pCur2 & 0xFC))
                {
                    *pCur2 &= 2;
                    *pCur2 |= (St.nSlot + 2) * 2;
                }
            }
            else if (G.Padd1_trg & PAD_DOWN)
            {
                pCur3 = &St.Icursol_main;
                *pCur3 += 4;
                if (*pCur3 > (St.nSlot + 3) * 2)
                {
                    *pCur3 = (*pCur3 & 2) | 4;
                }
            }
            St_sel_item_set();
            St.Blink_timer = 0;
            Snd_se_play(SE_BANK_ENEMY, 4, 0);
        }
        else
        {
            b0 = &St.Blink_timer;
            (*b0)--;
        }
        break;
    case 2:
        if (G_BASE->SaveData.Key_trg & 0x8000)
        {
            *pRno = 1;
            Snd_se_play(SE_BANK_ENEMY, 5, 0);
            break;
        }
        if (G_BASE->SaveData.Key_trg & 0x4000)
        {
            pBox_tbl = &MAIN.SaveData.Item_boxwork[0].Id;

            if ((St.Item_id | pBox_tbl[St.Ibox_sel * 2]) == 0)
            {
                break;
            }
            *pRno = 1;
            Snd_se_play(SE_BANK_ENEMY, 6, 0);
            slot = (St.Icursol_main >> 1) - 4;
            if (EQUIP_NO_CUR - 1 == slot)
            {
                MAIN.SaveData.Equip_no = 0;
            }

            do
            {
                pBox = &pBox_tbl[St.Ibox_sel * 2];

                box_id = *pBox;

                Box_num = pBox[1];

                *pBox = pItem_work_base[slot].Id;

                MAIN.SaveData.Item_boxwork[St.Ibox_sel].Num = pItem_work_base[slot].Num;

                pItem_work_base[slot].Id = box_id;
                pItem_work_base[slot].Num = Box_num;
            } while (0);

            if (slot < G.SaveData.Item_num)
            {
                new_id = box_id;
            }
            else
            {
                n = 0;
                while ((Item_pix_mask >> n) & 1)
                {
                    n++;
                }
                Item_pix_idx[slot] = n;
                Item_pix_mask |= 1 << n;
                do
                {
                    new_id = box_id;
                } while (0);
            }

            if (new_id != 0)
            {
                Pix_trans(Item_data_tbl.items[(new_id - 1)].model_id - 1, Item_pix_idx[slot], LOAD_BUF);
            }
            Sort_item();
            St_sel_item_set();

            pRow1 = &St.Ibox_sel;
            if (MAIN.SaveData.Item_boxwork[*pRow1].Id != 0)
            {
                Ibox_pix_trans(Item_data_tbl.items[(MAIN.SaveData.Item_boxwork[*pRow1].Id - 1)].model_id - 1, *pRow1 & 7);
            }
            break;
        }
        if (!(G_BASE->Padd1_trg & (PAD_UP | PAD_DOWN)) && St.Move_anm[0] == 0)
        {
            break;
        }
        pPadd = &MAIN.Padd1;
        if (!(*pPadd & (PAD_UP | PAD_DOWN)))
        {
            (&St.Sub_wait)[0] = 0xF;
            (&St.Sub_wait)[1] = 0;
            break;
        }
        *pRno = 3;
        St.Move_anm[0] = 1;
        Snd_se_play(SE_BANK_ROOM, 0x21, 0);
        if (*pPadd & PAD_UP)
        {
            St.Res_id = 0xF;
            St.Wep_flg = -1;
            if (St.Ibox_sel != 0)
            {
                St.Ibox_sel--;
            }
            else
            {
                St.Ibox_sel = 0x2F;
            }
            pRow2 = &St.Ibox_sel;
            if (MAIN.SaveData.Item_boxwork[*pRow2].Id != 0)
            {
                Ibox_pix_trans(Item_data_tbl.items[(MAIN.SaveData.Item_boxwork[*pRow2].Id - 1)].model_id - 1, *pRow2 & 7);
            }
        }
        else
        {
            St.Res_id = 0;
            St.Wep_flg = 1;

            n = St.Ibox_sel + 1;
            if (n == 0x30)
            {
                n = 0;
            }
            if (MAIN.SaveData.Item_boxwork[n].Id != 0)
            {
                Ibox_pix_trans(Item_data_tbl.items[(MAIN.SaveData.Item_boxwork[n].Id - 1)].model_id - 1, n & 7);
            }
        }
        if ((G.Padd1 >> 8) & (&St.Sub_wait)[1])
        {
            if ((&St.Sub_wait)[0] == 0)
            {
                if (St.Wep_flg > 0)
                {
                    St.Wep_flg = 3;
                }
                else
                {
                    St.Wep_flg = -3;
                }
            }
        }
        else
        {
            goto reset;
        }
    case 3:
        if ((G.Padd1 >> 8) & (&St.Sub_wait)[1])
        {
            if ((&St.Sub_wait)[0] != 0)
            {
                (&St.Sub_wait)[0]--;
            }
        }
        else
        {
        reset:
            (&St.Sub_wait)[0] = 0xF;
            if (G.Padd1 & (PAD_UP | PAD_DOWN))
            {
                if (G.Padd1 & PAD_UP)
                {
                    (&St.Sub_wait)[1] = 0x10;
                }
                else
                {
                    (&St.Sub_wait)[1] = 0x40;
                }
            }
            else
            {
                (&St.Sub_wait)[1] = 0;
            }
        }

        St.Res_id += St.Wep_flg;

        if (St.Res_id == 0xF)
        {
            St.Res_id = 0;
        }
        if (St.Res_id == 0)
        {
            St.Sub_rtn0 = 2;

            if (St.Wep_flg > 0)
            {
                if (St.Ibox_sel != 0x2F)
                {
                    St.Ibox_sel++;
                }
                else
                {
                    St.Ibox_sel = 0;
                }
            }
            St.Wep_flg = 0;
        }
        break;
    }
    St_disp_cursol0();

    Name_disp(St.Item_id, 0, 0x30 - Scr_ofs[0], 0xBA - Scr_ofs[1]);

    return 0;
}

VOID St_init01(VOID)
{
    Cd_read(ITEMBOX_TIM, LOAD_BUF);

    G.Vram_cur.b.Cclut = 0x1C;
    G.Vram_cur.b.Ctpage = 0x15;
    Mapping_tim(LOAD_BUF);
    Task_sleep(1);

    Cd_read(ITEM_ALL_PIX, LOAD_BUF);

    if (G.SaveData.Item_boxwork[0].Id != 0)
    {
        Ibox_pix_trans(Item_data_tbl.items[G.SaveData.Item_boxwork[0].Id - 1].model_id - 1, 0);
    }

    St.Ibox_sel = 0;
    St.Res_id = 0;
    St.Wep_flg = 0;
}

VOID Ibox_sort(VOID)
{
    UCHAR n;
    UCHAR Loop_ctr;
    UCHAR i;
    UCHAR j;

    UCHAR* tbl;
    UCHAR* tbl2;
    INT a;
    INT b;
    INT c;

    n = 0x30;
    Loop_ctr = 0x30;

    i = 0;
    j = 0;

    do
    {
        tbl = &MAIN.SaveData.Item_boxwork[0].Id;

        a = i * 2;

        if (tbl[a] != 0)
        {
            if (i != j)
            {
                b = j * 2;

                tbl[b] = tbl[a];
                (&MAIN.SaveData.Item_boxwork[0].Id)[b + 1] = (&MAIN.SaveData.Item_boxwork[0].Id)[a + 1];
            }
            j++;
        }
        i++;

        n--;
    } while (n != 0);

    Loop_ctr -= j;
    if (Loop_ctr)
    {
        do
        {
            tbl2 = &MAIN.SaveData.Item_boxwork[0].Id;

            c = j * 2;

            tbl2[c] = 0;
            (&MAIN.SaveData.Item_boxwork[0].Id)[c + 1] = 0;
            j++;

            Loop_ctr--;
        } while (Loop_ctr != 0);
    }
}

VOID St_disp_itembox(VOID)
{
    UCHAR n;
    UCHAR i;
    UCHAR item;

    INT i2;

    SHORT V_ofs;

    INT y;
    SHORT Row_y;

    INT t;

    INT base;
    INT k;
    INT c31;

    ScratchPad->Spr.attribute = 0x01000040;
    ScratchPad->Spr.cy = 0x1FC;
    ScratchPad->Spr.cx = 0;
    ScratchPad->Spr.tpage = 0x15;
    St.pFrm_rec = Ibox_spr_tbl + sizeof(Ibox_spr_tbl);
    do
    {
        St_spr_pop();
        GsSortFlipSprite(&ScratchPad->Spr, MAIN.Ot0 + G.Cbuf_id, 4);
    } while (St.pFrm_rec > Ibox_spr_tbl);

    ScratchPad->Rect.attribute = 0;
    ScratchPad->Rect.r = 0;
    ScratchPad->Rect.g = 0;
    ScratchPad->Rect.b = 0;
    St.pFrm_rec = Ibox_frm_tbl2 + sizeof(Ibox_frm_tbl2);
    do
    {
        ScratchPad->Rect.h = ((USHORT*)St.pFrm_rec)[-1];  // halfword view of the same memory
        ScratchPad->Rect.w = ((USHORT*)St.pFrm_rec)[-2];  // halfword view of the same memory
        ScratchPad->Rect.y = ((USHORT*)St.pFrm_rec)[-3];  // halfword view of the same memory
        ScratchPad->Rect.x = ((USHORT*)St.pFrm_rec)[-4];  // halfword view of the same memory
        GsSortBoxFill(&ScratchPad->Rect, MAIN.Ot0 + G.Cbuf_id, 4);
    } while ((St.pFrm_rec -= 8) > Ibox_frm_tbl2);

    ScratchPad->Rect.attribute = 0x60000000;
    ScratchPad->Rect.r = 0x30;
    ScratchPad->Rect.g = 0x30;
    ScratchPad->Rect.b = 0x30;
    ScratchPad->Rect.x = 0x2A;
    ScratchPad->Rect.y = 0x22;
    ScratchPad->Rect.w = 0x7E;
    ScratchPad->Rect.h = 0xF;
    GsSortBoxFill(&ScratchPad->Rect, MAIN.Ot0 + CBUF_ID_CUR, 4);
    ScratchPad->Rect.y = 0x41;
    GsSortBoxFill(&ScratchPad->Rect, MAIN.Ot0 + MAIN.Cbuf_id, 4);

    if (St.Sub_rtn0 < 2)
    {
        ScratchPad->Rect.y = 0x22;
        ScratchPad->Rect.h = 0x2E;
        GsSortBoxFill(&ScratchPad->Rect, MAIN.Ot0 + MAIN.Cbuf_id, 4);
        ScratchPad->Rect.x = 0x5C;
        ScratchPad->Rect.y = 0x58;
        ScratchPad->Rect.w = 0x28;
        ScratchPad->Rect.h = 0x1E;
        GsSortBoxFill(&ScratchPad->Rect, MAIN.Ot0 + MAIN.Cbuf_id, 4);
    }

    n = 1;

    if (St.Res_id != 0)
    {
        n = 2;
    }
    t = St.Res_id * 2;
    y = 0x58 - t;
    V_ofs = t;
    ScratchPad->Spr.y = 0x58;
    i = St.Ibox_sel;
    ScratchPad->Spr.h = 0x1E - t;
    do
    {
        ScratchPad->Spr.x = 0x5C;
        ScratchPad->Spr.w = 0x28;
        ScratchPad->Spr.cy = 0x1E6;

        i2 = i * 2;
        item = (&MAIN.SaveData.Item_boxwork[0].Id)[i2];

        if (item != 0)
        {
            ScratchPad->Spr.tpage = 0x15;
            ScratchPad->Spr.u = (i & 1) * 40;

            ScratchPad->Spr.v = V_ofs + (((i << 4) & 0x60) + (-0x80));

            GsSortFlipSprite(&ScratchPad->Spr, MAIN.Ot0 + G.Cbuf_id, 5);
            ScratchPad->Spr.y = y;
            ScratchPad->Spr.tpage = 0x1C;

            Print_num(item, ((&MAIN.SaveData.Item_boxwork[0].Id) + 1)[i2], 4);
        }
        else
        {
            ScratchPad->Spr.u = 0x90;

            ScratchPad->Spr.tpage = 0x1C;

            ScratchPad->Spr.v = V_ofs - 0x42;
            GsSortFlipSprite(&ScratchPad->Spr, MAIN.Ot0 + G.Cbuf_id, 5);
        }
        do
        {
            y += 0x1E;
        } while (0);
        ScratchPad->Spr.h = 0x1E;
        ScratchPad->Spr.y = y;
        V_ofs = 0;

        if (i != 0x2F)
        {
            i++;
        }
        else
        {
            i = 0;
        }
        n--;
    } while (n != 0);

    n = 3;
    if (St.Res_id != 0)
    {
        n = 4;
    }
    if (St.Ibox_sel != 0)
    {
        i = St.Ibox_sel - 1;
    }
    else
    {
        i = 0x2F;
    }

    Row_y = 0x23 - St.Res_id;

    sprintf(ScratchPad->Text_buf, "_Nothing_");
    do
    {
        item = MAIN.SaveData.Item_boxwork[i].Id;

        if (item != 0)
        {
            Name_disp(item, 0x50, 0x2A, Row_y);
        }
        else
        {
            Print14(0x2A, Row_y, 0x53, 0);
        }

        Row_y += 0xF;

        if (i != 0x2F)
        {
            i++;
        }
        else
        {
            i = 0;
        }
        n--;
    } while (n != 0);

    if ((UINT)(St.Ibox_sel - 1) >= 0x2D)  // unsigned conversion (signed operand)
    {
        k = St.Res_id;
        ScratchPad->Line.r = 0xFF;
        do
        {
            ScratchPad->Line.g = 0xEF;
            ScratchPad->Line.x0 = 0x2A;
            ScratchPad->Line.x1 = 0xA8;
            c31 = 0x31;
            ScratchPad->Line.attribute = 0;
        } while (0);
        ScratchPad->Line.b = 0;
        do
        {
            base = c31 - k;
        } while (0);
        y = (SHORT)base;  // sign-extends the halfword

        if (St.Ibox_sel != 0)
        {
            y = base + (0x30 - St.Ibox_sel) * 15;
        }
        ScratchPad->Line.y0 = y;
        ScratchPad->Line.y1 = y;
        GsSortLine(&ScratchPad->Line, MAIN.Ot0 + G.Cbuf_id, 5);
    }

    n = 3;
    if (St.Ibox_sel != 0)
    {
        i = St.Ibox_sel - 1;
    }
    else
    {
        i = 0x2F;
    }
    if (St.Wep_flg < 0)
    {
        i++;
    }
    ScratchPad->Spr.attribute = 0x01000040;
    ScratchPad->Spr.x = 0xB0;

    ScratchPad->Spr.u = 0x40;

    ScratchPad->Spr.v = 0x38;
    ScratchPad->Spr.w = 6;

    ScratchPad->Spr.h = 1;

    ScratchPad->Spr.cy = 0x1FC;
    ScratchPad->Spr.cx = 0;
    ScratchPad->Spr.tpage = 0x15;
    do
    {
        if (i >= 0x30)
        {
            i = 0;
        }

        ScratchPad->Spr.y = i + 0x21;
        GsSortFlipSprite(&ScratchPad->Spr, MAIN.Ot0 + G.Cbuf_id, 5);
        i++;

        n--;
    } while (n != 0);

    St.pFrm_rec = Ibox_frm_tbl2;
    do
    {
        St_spr_pop();
        GsSortFlipSprite(&ScratchPad->Spr, MAIN.Ot0 + G.Cbuf_id, 5);
    } while (St.pFrm_rec > Ibox_frm_tbl2 - 0x24);
}

// item box sprite records (12 bytes each: x, y, w, h, u, v), popped from the end
UCHAR Ibox_spr_tbl[60] =
{
    0x29,   // 0 //
    0x00,   // 1 //
    0x21,   // 2 //
    0x00,   // 3 //
    0x80,   // 4 //
    0x00,   // 5 //
    0x01,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x29,   // 12 //
    0x00,   // 13 //
    0x50,   // 14 //
    0x00,   // 15 //
    0x80,   // 16 //
    0x00,   // 17 //
    0x01,   // 18 //
    0x00,   // 19 //
    0x00,   // 20 //
    0x00,   // 21 //
    0x2F,   // 22 //
    0x00,   // 23 //
    0xAF,   // 24 //
    0x00,   // 25 //
    0x18,   // 26 //
    0x00,   // 27 //
    0x08,   // 28 //
    0x00,   // 29 //
    0x07,   // 30 //
    0x00,   // 31 //
    0x48,   // 32 //
    0x00,   // 33 //
    0x38,   // 34 //
    0x00,   // 35 //
    0xAF,   // 36 //
    0x00,   // 37 //
    0x53,   // 38 //
    0x00,   // 39 //
    0x08,   // 40 //
    0x00,   // 41 //
    0x07,   // 42 //
    0x00,   // 43 //
    0x48,   // 44 //
    0x00,   // 45 //
    0x40,   // 46 //
    0x00,   // 47 //
    0x5B,   // 48 //
    0x00,   // 49 //
    0x57,   // 50 //
    0x00,   // 51 //
    0x2A,   // 52 //
    0x00,   // 53 //
    0x20,   // 54 //
    0x00,   // 55 //
    0x10,   // 56 //
    0x00,   // 57 //
    0x30,   // 58 //
    0x00,   // 59 //
};

// item box frame strip sprite records (12 bytes each), popped from the end
UCHAR Ibox_frm_tbl[36] =
{
    0x29,   // 0 //
    0x00,   // 1 //
    0x22,   // 2 //
    0x00,   // 3 //
    0x80,   // 4 //
    0x00,   // 5 //
    0x2E,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x00,   // 9 //
    0x01,   // 10 //
    0x00,   // 11 //
    0xAF,   // 12 //
    0x00,   // 13 //
    0x20,   // 14 //
    0x00,   // 15 //
    0x08,   // 16 //
    0x00,   // 17 //
    0x20,   // 18 //
    0x00,   // 19 //
    0x00,   // 20 //
    0x00,   // 21 //
    0x30,   // 22 //
    0x00,   // 23 //
    0xAF,   // 24 //
    0x00,   // 25 //
    0x40,   // 26 //
    0x00,   // 27 //
    0x08,   // 28 //
    0x00,   // 29 //
    0x12,   // 30 //
    0x00,   // 31 //
    0x08,   // 32 //
    0x00,   // 33 //
    0x30,   // 34 //
    0x00,   // 35 //
};

// item box backdrop box records (8 bytes each: x, y, w, h), popped from the end
UCHAR Ibox_frm_tbl2[40] =
{
    0x2A,   // 0 //
    0x00,   // 1 //
    0x18,   // 2 //
    0x00,   // 3 //
    0x7E,   // 4 //
    0x00,   // 5 //
    0x09,   // 6 //
    0x00,   // 7 //
    0x2A,   // 8 //
    0x00,   // 9 //
    0x51,   // 10 //
    0x00,   // 11 //
    0x31,   // 12 //
    0x00,   // 13 //
    0x0F,   // 14 //
    0x00,   // 15 //
    0x5B,   // 16 //
    0x00,   // 17 //
    0x51,   // 18 //
    0x00,   // 19 //
    0x2A,   // 20 //
    0x00,   // 21 //
    0x06,   // 22 //
    0x00,   // 23 //
    0x85,   // 24 //
    0x00,   // 25 //
    0x51,   // 26 //
    0x00,   // 27 //
    0x23,   // 28 //
    0x00,   // 29 //
    0x0F,   // 30 //
    0x00,   // 31 //
    0x5C,   // 32 //
    0x00,   // 33 //
    0x77,   // 34 //
    0x00,   // 35 //
    0x28,   // 36 //
    0x00,   // 37 //
    0x09,   // 38 //
    0x00,   // 39 //
};
