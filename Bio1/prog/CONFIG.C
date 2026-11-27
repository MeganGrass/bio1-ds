/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\CONFIG.C
 */

#include "TYPES.H"

#include "CONFIG.H"
#include "MOJIDISP.H"
#include "PAD.H"
#include "SND.H"
#include "SCENARIO.H"

OPT_WORK Cnf_work; // options and key configuration screen work area

INT Cnf_key_main(VOID)
{
    UCHAR* state = &Cnf_work.Key_rno;

    ScratchPad->Spr.attribute = 0x01000040;
    ScratchPad->Spr.cx = 0;
    ScratchPad->Spr.cy = 0x1FF;
    ScratchPad->Spr.tpage = 0x15;
    ScratchPad->Spr.r = 0;
    ScratchPad->Spr.g = 0;
    ScratchPad->Spr.b = 0;
    ScratchPad->Spr.mx = 0;
    ScratchPad->Spr.my = 0;
    ScratchPad->Spr.rotate = 0;
    switch (*state)
    {
    case 0:
        *state = 1;
        Cnf_work.Sub_mode = 0;
        Cnf_work.Pad_rep_old = 0;

        Cnf_work.Sel_no = G.SaveData.Key_idx & 3;

    case 1:

        if (!(G.Padd1 & PAD_SELECT) && (G.Padd1_trg & PAD_START))
        {
            return 1;
        }
        if (!Cnf_key_padd())
        {
            return 1;
        }
        break;
    }
    Cnf_key_frame_disp();
    Cnf_key_layout_disp();
    return 0;
}

VOID Cnf_key_frame_disp(VOID)
{
    UCHAR i;
    UCHAR* p;

    switch (Cnf_work.Sub_mode)
    {
    case 1:
    case 2:
    case 3:
        if (Cnf_work.Key_cur < 12)
        {
            ScratchPad->Spr.attribute = 0x41000040;
            ScratchPad->Spr.x = Cnf_slotx_tbl[Cnf_work.Key_cur];
            ScratchPad->Spr.y = Cnf_sloty_tbl[Cnf_work.Key_cur >> 1];
            ScratchPad->Spr.w = 0x46;
            ScratchPad->Spr.h = 0xE;
            ScratchPad->Spr.u = 0;
            ScratchPad->Spr.v = 0x20;
            GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[MAIN.Cbuf_id], 4);
        }
        else
        {
            ScratchPad->Rect.attribute = 0x40000000;
            if (Cnf_work.Key_cur & 1)
            {
                ScratchPad->Rect.x = 0x79;
            }
            else
            {
                ScratchPad->Rect.x = -0x1B;
            }
            ScratchPad->Rect.w = 0xE;
            ScratchPad->Rect.h = 0xE;
            ScratchPad->Rect.r = 0x98;
            ScratchPad->Rect.g = 0x98;
            ScratchPad->Rect.b = 0x20;
            ScratchPad->Rect.y = (Cnf_work.Key_cur - 12) / 2 * 16 + 0x27;
            GsSortBoxFill(&ScratchPad->Rect, &MAIN.Ot0[MAIN.Cbuf_id], 4);
        }
    case 0:
        p = &Cnf_work.Sel_no;
        ScratchPad->Rect.attribute = 0x50000000;
        ScratchPad->Rect.x = Cnf_typex_tbl[*p];
        ScratchPad->Rect.y = -0x5F;
        if (*p < 3)
        {
            ScratchPad->Rect.w = 0xE;
        }
        else
        {
            ScratchPad->Rect.w = 0x2E;
        }
        ScratchPad->Rect.h = 0xE;
        ScratchPad->Rect.r = 0;
        ScratchPad->Rect.g = 0;
        ScratchPad->Rect.b = 0x80;
        GsSortBoxFill(&ScratchPad->Rect, &MAIN.Ot0[CBUF_ID_CUR], 2);
        ScratchPad->Rect.attribute = 0x60000000;
        ScratchPad->Rect.w = 0x2E;
        ScratchPad->Rect.r = 0x30;
        ScratchPad->Rect.g = 0x30;
        ScratchPad->Rect.b = 0x30;
        if (Cnf_work.Sel_no != 4)
        {
            ScratchPad->Rect.x = 0x5C;

            GsSortBoxFill(&ScratchPad->Rect, &MAIN.Ot0[MAIN.Cbuf_id], 2);
        }
        break;
    }
    i = 4;
    do
    {
        i--;
        if (i != (G.SaveData.Key_idx & 3))
        {
            ScratchPad->Rect.x = Cnf_typex_tbl[i];
            GsSortBoxFill(&ScratchPad->Rect, &MAIN.Ot0[MAIN.Cbuf_id], 2);
        }
        if (i == 3)
        {
            ScratchPad->Rect.w = 0xE;
        }
    } while (i != 0);
    ScratchPad->Spr.attribute = 0x1000040;

    ScratchPad->Spr.x = -9;

    ScratchPad->Spr.y = -0x60;
    ScratchPad->Spr.w = 0x32;
    ScratchPad->Spr.h = 0x10;
    ScratchPad->Spr.u = 0;
    ScratchPad->Spr.v = 0xC8;
    GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[CBUF_ID_CUR], 4);

    ScratchPad->Spr.x = 0x2A;

    ScratchPad->Spr.w = 0x30;
    ScratchPad->Spr.v = 0xD8;
    GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[MAIN.Cbuf_id], 4);
    ScratchPad->Spr.x = -0xC;
    ScratchPad->Spr.y = -0x6A;
    ScratchPad->Spr.w = 0x20;
    ScratchPad->Spr.h = 8;
    ScratchPad->Spr.v = 0xC0;
    GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[MAIN.Cbuf_id], 4);
}

VOID Cnf_key_layout_disp(VOID)
{
    UCHAR i;
    UCHAR n;
    USHORT bits;
    USHORT mask;
    USHORT* tbl;
    UCHAR* sel;
    USHORT x;
    USHORT y;

    i = 12;
    do
    {
        i--;
        ScratchPad->Spr.attribute = 0x01000040;
        ScratchPad->Spr.x = Cnf_slotx_tbl[i] + 0x1C;
        ScratchPad->Spr.y = Cnf_sloty_tbl[i >> 1];
        ScratchPad->Spr.w = 0xE;
        ScratchPad->Spr.h = 0xE;
        ScratchPad->Spr.u = (i & 0x7E) * 8;
        ScratchPad->Spr.v = (i & 1) * 0x10;
        GsSortFlipSprite(&ScratchPad->Spr, MAIN.Ot0 + G.Cbuf_id, 4);
    } while (i != 0);
    ScratchPad->Spr.attribute = 0x40;

    ScratchPad->Spr.w = 8;

    ScratchPad->Spr.cx = 0x100;
    ScratchPad->Spr.cy = 0x1E0;

    ScratchPad->Spr.h = 0xE;

    ScratchPad->Spr.tpage = 0x1F;

    tbl = Key_type[G.SaveData.Key_idx & 3];
    i = 8;
    do
    {
        i--;

        Mess_print(Cnf_labelx_tbl[i], y = (i >> 1) * 16 + 0xA0, 0x30, Cnf_label_tbl[i]);

        bits = tbl[Cnf_slot_tbl[i]];

        if (bits != 0)
        {
            ScratchPad->Spr.y--;

            if (bits & 0xF000)
            {
                ScratchPad->Spr.u = 0x70;

                mask = 0x1000;
            }
            else
            {
                mask = 1;

                ScratchPad->Spr.u = 0;
            }

            ScratchPad->Spr.v = 0x8C;
            ScratchPad->Spr.w = 0xE;

            for (n = 0; !(bits & (mask << n)); n++)
            {
            }

            ScratchPad->Spr.x = i & 1 ? 0x79 : -0x1B;

            ScratchPad->Spr.cx = 0x100;
            ScratchPad->Spr.cy = 0x1E0;
            ScratchPad->Spr.tpage = 0x1E;

            ScratchPad->Spr.u += n * 14;

            GsSortFlipSprite(&ScratchPad->Spr, MAIN.Ot0 + G.Cbuf_id, 3);
        }
    } while (i != 0);
    sel = Cnf_act_tbl0;
    if ((G.SaveData.Key_idx & 0x40) && (KEY_IDX_NOW & 3) == 3)
    {
        sel = Cnf_act_tbl1;
    }
    i = 12;

loop3:
    i--;
    bits = Cnf_bit_tbl[i];
    n = 7;
    if ((G.SaveData.Key_idx & 0x40) && (KEY_IDX_NOW & 3) == 3)
    {
        n = 8;
    }
    while (n != 0 && !(bits & tbl[sel[n - 1] & 0xF]))
    {
        n--;
    }
    x = Cnf_slotx_tbl[i] + 0xA0;
    y = Cnf_sloty_tbl[i >> 1] + 0x79;
    mask = 0x33;
    if (n != 0)
    {
        mask = 0x30;
    }
    if ((G.SaveData.Key_idx & 0x40) && (KEY_IDX_NOW & 3) == 3)
    {
        x += Cnf_namex_tbl1[n];
        Mess_print((USHORT)x, (USHORT)y, (UCHAR)mask, Cnf_name_tbl1[n]);
    }
    else
    {
        x += Cnf_namex_tbl0[n];
        Mess_print((USHORT)x, (USHORT)y, (UCHAR)mask, Cnf_name_tbl0[n]);
    }
    if (i != 0)
    {
        goto loop3;
    }
}

UCHAR Cnf_key_padd(VOID)
{
    INT r;
    UCHAR k;
    UCHAR n;
    UCHAR* tbl;
    UCHAR i;
    UCHAR v;
    UCHAR b;
    USHORT val;

    switch (Cnf_work.Sub_mode)
    {
    case 0:

        if (G.Padd1_trg & PAD_TRIANGLE)
        {
            Snd_se_play(SE_BANK_ENEMY, 5, 0);

            return 0;
        }

        if (G.Padd1_trg & PAD_X)
        {
            Snd_se_play(SE_BANK_ENEMY, 6, 0);

            if (Cnf_work.Sel_no == 4)
            {
                return 0;
            }
            if (Cnf_work.Sel_no == 3 && (G.SaveData.Key_idx & 3) == Cnf_work.Sel_no)
            {
                Cnf_work.Sub_mode = 1;
                Cnf_work.Key_cur = 0;
                Cnf_work.Pad_rep_old = 0;
            }

            KEY_IDX_CUR &= 0xFC;

            MAIN.SaveData.Key_idx |= Cnf_work.Sel_no;
            break;
        }
        r = Pad_rep_get(&Cnf_work.Repeat_timer, &Cnf_work.Pad_rep_old, 0xE6, 0xA000);
        if (r & 0xA000)
        {
            Snd_se_play(SE_BANK_ENEMY, 4, 0);

            if (r & 0x8000)
            {
                if (Cnf_work.Sel_no != 0)
                {
                    Cnf_work.Sel_no--;
                    break;
                }
                Cnf_work.Sel_no = 4;
                break;
            }
            if (Cnf_work.Sel_no == 4)
            {
                Cnf_work.Sel_no = 0;
                break;
            }
            Cnf_work.Sel_no++;
            break;
        }
        break;
    case 1:

        if (G.Padd1_trg & PAD_TRIANGLE)
        {
            Cnf_work.Sub_mode = 0;
            Cnf_work.Pad_rep_old = 0;

            Snd_se_play(SE_BANK_ENEMY, 5, 0);

            break;
        }
        if (G.Padd1_trg & PAD_SELECT)
        {
            if ((PADD1_CUR & (PAD_R2 | PAD_R1)) == (PAD_R2 | PAD_R1) && !(MAIN.Padd1 & ~(PAD_R2 | PAD_R1 | PAD_SELECT)))
            {
                KEY_IDX_CUR ^= 0x40;
            }

            if (G.SaveData.Key_idx & 0x40)
            {
                Pad_def_set2();
                break;
            }
            Pad_def_set();
            break;
        }

        if (G.Padd1_trg & PAD_X)
        {
            Snd_se_play(SE_BANK_ENEMY, 6, 0);

            Cnf_work.Key_sel = 0;
            Cnf_work.Pad_rep_old = 0;
            {
                INT second_page = Cnf_work.Key_cur >= 0xC;
                Cnf_work.Sub_mode = second_page + 2;
            }
            break;
        }
        r = Pad_rep_get(&Cnf_work.Repeat_timer, &Cnf_work.Pad_rep_old, 0xE6, 0xF000);
        if ((USHORT)r == 0)  // truncates to an unsigned halfword
        {
            break;
        }

        Snd_se_play(SE_BANK_ENEMY, 4, 0);

        if ((UINT)(Cnf_work.Key_cur - 4) >= 8)  // unsigned conversion (signed operand)
        {
            if (r & 0xA000)
            {
                Cnf_work.Key_cur ^= 1;
            }
            if (!(r & 0x5000))
            {
                break;
            }
            if (r & 0x1000)
            {
                if (Cnf_work.Key_cur & 0xFE)
                {
                    Cnf_work.Key_cur -= 2;
                    break;
                }
                Cnf_work.Key_cur += 0x12;
                break;
            }
            Cnf_work.Key_cur += 2;
            if (Cnf_work.Key_cur >= 0x14)
            {
                Cnf_work.Key_cur &= 1;
                break;
            }
            break;
        }
        k = Cnf_key_idx_get(r);
        if (k == 0)
        {
            break;
        }
        k--;
        v = Cnf_work.Key_cur - 4;
        Cnf_work.Key_cur = Cnf_move_tbl[v][k];
        break;
    case 2:
        n = 8;
        if (G.SaveData.Key_idx & 0x40)
        {
            tbl = Cnf_act_tbl1;
        }
        else
        {
            n = 7;
            tbl = Cnf_act_tbl0;
        }

        if (G.Padd1_trg & (PAD_TRIANGLE | PAD_X))
        {
            if (G.Padd1_trg & PAD_X)
            {
                Snd_se_play(SE_BANK_ENEMY, 6, 0);

                i = n;
                r = Cnf_bit_tbl[Cnf_work.Key_cur];
                b = tbl[Cnf_work.Key_sel];
                do
                {
                    i--;
                    v = tbl[i];
                    if ((r & 0xFFFF) == KEY_TBL_CUR[v & 0xF])
                    {
                        val = 0;
                        if (b != 0xFE)
                        {
                            val = KEY_TBL_CUR[b & 0xF];
                        }
                        do
                        {
                            KEY_TBL_CUR[v & 0xF] = val;
                            v >>= 4;
                        } while (v != 0);
                        i = 0;
                    }
                } while (i != 0);
                if (b != 0xFE)
                {
                    i = b;
                    do
                    {
                        KEY_TBL_CUR[i & 0xF] = r;
                        i >>= 4;
                    } while (i != 0);
                }
            }
            else
            {
                Snd_se_play(SE_BANK_ENEMY, 5, 0);
            }
            Cnf_work.Sub_mode = 1;
            Cnf_work.Pad_rep_old = 0;
        }
        else
        {
            r = Pad_rep_get(&Cnf_work.Repeat_timer, &Cnf_work.Pad_rep_old, 0xE6, 0x5000);
            if (r & 0x5000)
            {
                Snd_se_play(SE_BANK_ENEMY, 4, 0);

                if (r & 0x1000)
                {
                    if (Cnf_work.Key_sel != 0)
                    {
                        Cnf_work.Key_sel--;
                    }
                    else
                    {
                        Cnf_work.Key_sel = n;
                    }
                }
                else if (Cnf_work.Key_sel != n)
                {
                    Cnf_work.Key_sel++;
                }
                else
                {
                    Cnf_work.Key_sel = 0;
                }
            }
        }
        Cnf_key_act_disp();
        break;
    case 3:

        if (G.Padd1_trg & (PAD_TRIANGLE | PAD_X))
        {
            if (G.Padd1_trg & PAD_X)
            {
                Snd_se_play(SE_BANK_ENEMY, 6, 0);

                i = 0xC;
                r = Cnf_btn_tbl[Cnf_work.Key_sel];
                b = Cnf_act_tbl1[Cnf_work.Key_cur];
                do
                {
                    i--;
                    if ((r & 0xFFFF) == KEY_TBL_CUR[Cnf_slot_tbl[i]])
                    {
                        KEY_TBL_CUR[Cnf_slot_tbl[i]] = KEY_TBL_CUR[b];
                        i = 0;
                    }
                } while (i != 0);
                MAIN.SaveData.Key_tbl[b] = r;
            }
            else
            {
                Snd_se_play(SE_BANK_ENEMY, 5, 0);
            }
            Cnf_work.Sub_mode = 1;
            Cnf_work.Pad_rep_old = 0;
        }
        else
        {
            r = Pad_rep_get(&Cnf_work.Repeat_timer, &Cnf_work.Pad_rep_old, 0xE6, 0xF000);

            if ((USHORT)r != 0)  // truncates to an unsigned halfword
            {
                Snd_se_play(SE_BANK_ENEMY, 4, 0);
                if (r & 0xA000)
                {
                    Cnf_work.Key_sel ^= 1;
                }
                if (r & 0x5000)
                {
                    if (r & 0x1000)
                    {
                        if (!(Cnf_work.Key_sel & 0xFE))
                        {
                            Cnf_work.Key_sel = (Cnf_work.Key_sel & 1) + 0xC;
                        }
                        Cnf_work.Key_sel -= 2;
                    }
                    else
                    {
                        Cnf_work.Key_sel += 2;
                        if (Cnf_work.Key_sel >= 0xC)
                        {
                            Cnf_work.Key_sel &= 1;
                        }
                    }
                }
            }
        }
        Cnf_key_btn_disp();
        break;
    }
    return 1;
}

VOID Cnf_key_act_disp(VOID)
{
    SCHAR* actions;
    INT color;
    UCHAR** names;
    INT Sel_y;
    INT Cur_y;
    INT idx;
    INT i;
    INT y;
    INT n;
    INT x;
    UCHAR* pName_x;
    INT v;

    if (G.SaveData.Key_idx & 0x40)
    {
        actions = Cnf_act_tbl1;
        names = Cnf_name_tbl1;
        pName_x = Cnf_namex_tbl1;
        n = 8;
    }
    else
    {
        actions = Cnf_act_tbl0;
        names = Cnf_name_tbl0;
        pName_x = Cnf_namex_tbl0;
        n = 7;
    }
    x = 0xC1;
    if (Cnf_work.Key_cur & 1)
    {
        x = 0x39;
    }

    y = 0x4D;

    if (G.SaveData.Key_idx & 0x40)
    {
        y = 0x47;
    }
    i = 0;
    do
    {
        n -= 1;
        color = 0x30;
        if (MAIN.SaveData.Key_tbl[actions[i & 0xFF] & 0xF] != 0)
        {
            color = 0x33;
        }
        i += 1;
        Cur_y = y & 0xFFFF;
        y += 0x10;
        idx = i & 0xFF;

        Mess_print(x + pName_x[idx], Cur_y, color, names[idx]);
    } while (n & 0xFF);

    Mess_print(x + *pName_x, y & 0xFFFF, 0x33, *names);

    ScratchPad->Rect.attribute = 0x50000000;
    ScratchPad->Rect.x = x - 0xA0;
    Sel_y = Cnf_work.Key_sel * 0x10;
    ScratchPad->Rect.y = Sel_y - 0x2C;
    if (KEY_IDX_CUR & 0x40)
    {
        ScratchPad->Rect.y = Sel_y - 0x32;
    }
    ScratchPad->Rect.w = 0x46;
    ScratchPad->Rect.h = 0xE;
    ScratchPad->Rect.r = 0;
    ScratchPad->Rect.g = 0;
    ScratchPad->Rect.b = 0x80;
    GsSortBoxFill(&ScratchPad->Rect, &MAIN.Ot0[CBUF_ID_CUR], 3U);
    ScratchPad->Spr.attribute = 0x01000040;
    ScratchPad->Spr.cy = 0x1FF;
    ScratchPad->Spr.tpage = 0x15;
    ScratchPad->Spr.x = x - 0xAD;
    ScratchPad->Spr.y = 3;
    ScratchPad->Spr.v = 0x63;
    ScratchPad->Spr.cx = 0;
    ScratchPad->Spr.u = 0;
    ScratchPad->Spr.w = 0x60;
    ScratchPad->Spr.h = 0x5D;
    if (MAIN.SaveData.Key_idx & 0x40)
    {
        ScratchPad->Spr.y = -3;

        v = 0x153;
        ScratchPad->Spr.v = v;

        ScratchPad->Spr.h = 0x6D;
    }
    GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[MAIN.Cbuf_id], 3U);
    ScratchPad->Spr.v = 0x30;
    ScratchPad->Spr.h = 0x43;
    ScratchPad->Spr.y -= 0x43;
    GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[MAIN.Cbuf_id], 3U);
}

VOID Cnf_key_btn_disp(VOID)
{
    INT x;
    UCHAR i;
    UCHAR j;
    USHORT code;
    USHORT mask;
    INT flag;
    UCHAR* p;

    x = 0x21;
    if (Cnf_work.Key_cur & 1)
    {
        x = -0x3F;
    }
    i = 12;
    ScratchPad->Spr.attribute = 0x40;

    ScratchPad->Spr.w = 0xE;

    ScratchPad->Spr.h = 0xE;
    ScratchPad->Spr.cy = 0x1E0;
    ScratchPad->Spr.tpage = 0x1E;
    do
    {
        i--;

        ScratchPad->Spr.x = x + (i & 1) * 16;
        ScratchPad->Spr.y = (i >> 1) * 16 - 2;

        code = Cnf_btn_tbl[i];
        if (code & 0xF000)
        {
            ScratchPad->Spr.u = 0x70;

            mask = 0x1000;
        }
        else
        {
            mask = 1;

            ScratchPad->Spr.u = 0;
        }

        ScratchPad->Spr.v = 0x8C;

        j = 0;
        while (!(code & (mask << j)))
        {
            j++;
        }
        flag = 0;

        ScratchPad->Spr.u += j * 14;

        j = 8;
        do
        {
            j--;
            if (KEY_TBL_CUR[Cnf_slot_tbl[j]] == code)
            {
                flag = 0x30;
            }
        } while (j != 0 && flag == 0);
        ScratchPad->Spr.cx = flag | 0x100;
        GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 3);
    } while (i != 0);
    ScratchPad->Rect.attribute = 0x50000000;
    ScratchPad->Rect.w = 0xE;
    ScratchPad->Rect.h = 0xE;
    ScratchPad->Rect.r = 0;
    ScratchPad->Rect.g = 0;
    ScratchPad->Rect.b = 0x80;
    p = &Cnf_work.Key_sel;
    ScratchPad->Rect.x = x + (*p & 1) * 16;
    ScratchPad->Rect.y = (*p >> 1) * 16 - 2;
    GsSortBoxFill(&ScratchPad->Rect, &MAIN.Ot0[CBUF_ID_CUR], 3);
    ScratchPad->Spr.attribute = 0x01000040;
    ScratchPad->Spr.cy = 0x1FF;
    ScratchPad->Spr.tpage = 0x15;
    ScratchPad->Spr.x = x - 0xD;
    ScratchPad->Spr.y = -0x16;
    ScratchPad->Spr.u = 0x60;
    ScratchPad->Spr.cx = 0;
    ScratchPad->Spr.v = 0;
    ScratchPad->Spr.w = 0x20;
    ScratchPad->Spr.h = 0x80;
    GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[MAIN.Cbuf_id], 3);
    ScratchPad->Spr.v = 0x80;
    ScratchPad->Spr.w = 0x18;
    ScratchPad->Spr.x += 0x20;
    GsSortFastSprite(&ScratchPad->Spr, &MAIN.Ot0[MAIN.Cbuf_id], 3);
}

UCHAR Cnf_key_idx_get(USHORT code)
{
    UCHAR i;

    i = 8;
    do
    {
        i -= 1;
        if ((code >> 8) == Cnf_dir_tbl[i])
        {
            return i + 1;
        }
    } while (i != 0);
    return 0;
}

INT Pad_rep_get(UCHAR* timer, USHORT* pHeld_old, UCHAR rates, INT mask)
{
    INT Trg;
    USHORT res;

    Trg = mask & G.Padd1_trg;

    res = Trg;

    if (mask & (PADD1_CUR & *pHeld_old))
    {
        (*timer)--;
        if (*timer == 0)
        {
            res = Trg | (mask & MAIN.Padd1);
            *timer = rates & 0xF;
        }
    }
    else
    {
        *timer = rates >> 4;
    }
    *pHeld_old = G.Padd1;
    return res;
}

// x position of each controller type tab (entry 4 is the exit box, entry 5 is unused)
SHORT Cnf_typex_tbl[6] =
{
    -8, // 0 //
    9,  // 1 //
    26, // 2 //
    43, // 3 //
    92, // 4 //
    0,  // 5 //
};

// x position of each of the 12 button slots
SHORT Cnf_slotx_tbl[12] =
{
    -128,   // 0 //
    58,     // 1 //
    -128,   // 2 //
    58,     // 3 //
    -113,   // 4 //
    43,     // 5 //
    -150,   // 6 //
    5,      // 7 //
    -75,    // 8 //
    80,     // 9 //
    -113,   // 10 //
    43,     // 11 //
};

// y position of each button slot row, by slot / 2
SHORT Cnf_sloty_tbl[6] =
{
    -67, // 0 //
    -48, // 1 //
    -28, // 2 //
    -8,  // 3 //
    -8,  // 4 //
    12,  // 5 //
};

// cursor target when moving in each of the 8 directions (columns), by slot - 4 (rows)
UCHAR Cnf_move_tbl[8][8] =
{
    { 0x02, 0x03, 0x05, 0x08, 0x06, 0x06, 0x05, 0x03 },   // 0 //
    { 0x03, 0x02, 0x04, 0x09, 0x07, 0x07, 0x04, 0x02 },   // 1 //
    { 0x04, 0x04, 0x08, 0x0A, 0x0A, 0x0B, 0x09, 0x05 },   // 2 //
    { 0x05, 0x05, 0x09, 0x0B, 0x0B, 0x0A, 0x08, 0x04 },   // 3 //
    { 0x04, 0x05, 0x07, 0x0B, 0x0A, 0x0A, 0x06, 0x04 },   // 4 //
    { 0x05, 0x04, 0x06, 0x0A, 0x0B, 0x0B, 0x07, 0x05 },   // 5 //
    { 0x06, 0x08, 0x0B, 0x0D, 0x0C, 0x0D, 0x0B, 0x06 },   // 6 //
    { 0x07, 0x09, 0x0A, 0x0C, 0x0D, 0x0C, 0x0A, 0x07 },   // 7 //
};

// direction codes (high byte of the pad bits): up, up-right, right, down-right, down, down-left, left, up-left
UCHAR Cnf_dir_tbl[8] =
{
    0x10,   // 0 //
    0x30,   // 1 //
    0x20,   // 2 //
    0x60,   // 3 //
    0x40,   // 4 //
    0xC0,   // 5 //
    0x80,   // 6 //
    0x90,   // 7 //
};

// pad button bit of each of the 12 button slots
USHORT Cnf_bit_tbl[12] =
{
    0x0001,   // 0 //
    0x0002,   // 1 //
    0x0004,   // 2 //
    0x0008,   // 3 //
    0x1000,   // 4 //
    0x0010,   // 5 //
    0x8000,   // 6 //
    0x0080,   // 7 //
    0x2000,   // 8 //
    0x0020,   // 9 //
    0x4000,   // 10 //
    0x0040,   // 11 //
};

// binding index of each row of the 7-entry action list (0xFE = unassign, two nibbles = two bindings)
UCHAR Cnf_act_tbl0[8] =
{
    0x00,   // 0 //
    0x02,   // 1 //
    0x01,   // 2 //
    0x03,   // 3 //
    0x76,   // 4 //
    0x09,   // 5 //
    0x08,   // 6 //
    0xFE,   // 7 //
};

// binding index of each row of the 8-entry action list (0xFE = unassign, entries 9 to 11 are zero padding)
UCHAR Cnf_act_tbl1[12] =
{
    0x00,   // 0 //
    0x02,   // 1 //
    0x01,   // 2 //
    0x03,   // 3 //
    0x07,   // 4 //
    0x06,   // 5 //
    0x09,   // 6 //
    0x08,   // 7 //
    0xFE,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
};

// binding index of each of the 8 action rows
UCHAR Cnf_slot_tbl[8] =
{
    0x04,   // 0 //
    0x0A,   // 1 //
    0x05,   // 2 //
    0x0B,   // 3 //
    0x0E,   // 4 //
    0x0C,   // 5 //
    0x0F,   // 6 //
    0x0D,   // 7 //
};

// message text: names of action list entries 0, 1 and 2
UCHAR Cnf_name_dat0[20] =
{
    0x2A,   // 0 //
    0x2B,   // 1 //
    0x00,   // 2 //
    0x31,   // 3 //
    0x2F,   // 4 //
    0x21,   // 5 //
    0x01,   // 6 //
    0x22,   // 7 //
    0x2B,   // 8 //
    0x2E,   // 9 //
    0x33,   // 10 //
    0x1D,   // 11 //
    0x2E,   // 12 //
    0x20,   // 13 //
    0x01,   // 14 //
    0x1E,   // 15 //
    0x1D,   // 16 //
    0x1F,   // 17 //
    0x27,   // 18 //
    0x01,   // 19 //
};

// message text: names of action list entries 3 to 7
UCHAR Cnf_name_dat1[36] =
{
    0x2E,   // 0 //
    0x79,   // 1 //
    0x30,   // 2 //
    0x31,   // 3 //
    0x2E,   // 4 //
    0x2A,   // 5 //
    0x01,   // 6 //
    0x28,   // 7 //
    0x79,   // 8 //
    0x30,   // 9 //
    0x31,   // 10 //
    0x2E,   // 11 //
    0x2A,   // 12 //
    0x01,   // 13 //
    0x1D,   // 14 //
    0x1F,   // 15 //
    0x30,   // 16 //
    0x25,   // 17 //
    0x2B,   // 18 //
    0x2A,   // 19 //
    0x01,   // 20 //
    0x20,   // 21 //
    0x1D,   // 22 //
    0x2F,   // 23 //
    0x24,   // 24 //
    0x01,   // 25 //
    0x23,   // 26 //
    0x21,   // 27 //
    0x30,   // 28 //
    0xFF,   // 29 //
    0x2E,   // 30 //
    0x21,   // 31 //
    0x1D,   // 32 //
    0x20,   // 33 //
    0x35,   // 34 //
    0x01,   // 35 //
};

// message text: names of the two split action entries of the 8-entry list
UCHAR Cnf_name_dat2[16] =
{
    0x2F,   // 0 //
    0x21,   // 1 //
    0x1D,   // 2 //
    0x2E,   // 3 //
    0x1F,   // 4 //
    0x24,   // 5 //
    0x01,   // 6 //
    0x1D,   // 7 //
    0x30,   // 8 //
    0x30,   // 9 //
    0x1D,   // 10 //
    0x1F,   // 11 //
    0x27,   // 12 //
    0x01,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
};

// name text of the 7-entry action list: entry 0 is the unassign row, entry 1 + n the name of row n
UCHAR* Cnf_name_tbl0[8] =
{
    Cnf_name_dat0,        // 0 //
    Cnf_name_dat0 + 7,    // 1 //
    Cnf_name_dat0 + 15,   // 2 //
    Cnf_name_dat1,        // 3 //
    Cnf_name_dat1 + 7,    // 4 //
    Cnf_name_dat1 + 14,   // 5 //
    Cnf_name_dat1 + 21,   // 6 //
    Cnf_name_dat1 + 26,   // 7 //
};

// name text of the 8-entry action list, indexed like Cnf_name_tbl0
UCHAR* Cnf_name_tbl1[9] =
{
    Cnf_name_dat0,        // 0 //
    Cnf_name_dat0 + 7,    // 1 //
    Cnf_name_dat0 + 15,   // 2 //
    Cnf_name_dat1,        // 3 //
    Cnf_name_dat1 + 7,    // 4 //
    Cnf_name_dat2,        // 5 //
    Cnf_name_dat2 + 7,    // 6 //
    Cnf_name_dat1 + 21,   // 7 //
    Cnf_name_dat1 + 26,   // 8 //
};

// x offset of each name of Cnf_name_tbl0
UCHAR Cnf_namex_tbl0[8] =
{
    0x0B,   // 0 //
    0x07,   // 1 //
    0x13,   // 2 //
    0x0B,   // 3 //
    0x0B,   // 4 //
    0x0B,   // 5 //
    0x13,   // 6 //
    0x01,   // 7 //
};

// x offset of each name of Cnf_name_tbl1
UCHAR Cnf_namex_tbl1[12] =
{
    0x0B,   // 0 //
    0x07,   // 1 //
    0x13,   // 2 //
    0x0B,   // 3 //
    0x0B,   // 4 //
    0x0B,   // 5 //
    0x0B,   // 6 //
    0x13,   // 7 //
    0x01,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
};

// message text: labels of action rows 0 to 6
UCHAR Cnf_label_dat0[76] =
{
    0x31,   // 0 //
    0x2C,   // 1 //
    0x2C,   // 2 //
    0x21,   // 3 //
    0x2E,   // 4 //
    0x00,   // 5 //
    0x1D,   // 6 //
    0x30,   // 7 //
    0x30,   // 8 //
    0x1D,   // 9 //
    0x1F,   // 10 //
    0x27,   // 11 //
    0x01,   // 12 //
    0x25,   // 13 //
    0x30,   // 14 //
    0x21,   // 15 //
    0x29,   // 16 //
    0x00,   // 17 //
    0x2E,   // 18 //
    0x79,   // 19 //
    0x30,   // 20 //
    0x31,   // 21 //
    0x2E,   // 22 //
    0x2A,   // 23 //
    0x01,   // 24 //
    0x28,   // 25 //
    0x2B,   // 26 //
    0x33,   // 27 //
    0x21,   // 28 //
    0x2E,   // 29 //
    0x00,   // 30 //
    0x1D,   // 31 //
    0x30,   // 32 //
    0x30,   // 33 //
    0x1D,   // 34 //
    0x1F,   // 35 //
    0x27,   // 36 //
    0x01,   // 37 //
    0x25,   // 38 //
    0x30,   // 39 //
    0x21,   // 40 //
    0x29,   // 41 //
    0x00,   // 42 //
    0x28,   // 43 //
    0x79,   // 44 //
    0x30,   // 45 //
    0x31,   // 46 //
    0x2E,   // 47 //
    0x2A,   // 48 //
    0x01,   // 49 //
    0x2F,   // 50 //
    0x21,   // 51 //
    0x28,   // 52 //
    0x21,   // 53 //
    0x1F,   // 54 //
    0x30,   // 55 //
    0x01,   // 56 //
    0x21,   // 57 //
    0x34,   // 58 //
    0x2C,   // 59 //
    0x1D,   // 60 //
    0x2A,   // 61 //
    0x20,   // 62 //
    0x00,   // 63 //
    0x25,   // 64 //
    0x30,   // 65 //
    0x21,   // 66 //
    0x29,   // 67 //
    0x01,   // 68 //
    0x1F,   // 69 //
    0x1D,   // 70 //
    0x2A,   // 71 //
    0x1F,   // 72 //
    0x21,   // 73 //
    0x28,   // 74 //
    0x01,   // 75 //
};

// message text: label of action row 7
UCHAR Cnf_label_dat1[12] =
{
    0x2F,   // 0 //
    0x24,   // 1 //
    0x2E,   // 2 //
    0x25,   // 3 //
    0x2A,   // 4 //
    0x27,   // 5 //
    0x00,   // 6 //
    0x25,   // 7 //
    0x30,   // 8 //
    0x21,   // 9 //
    0x29,   // 10 //
    0x01,   // 11 //
};

// label text of each of the 8 action rows
UCHAR* Cnf_label_tbl[8] =
{
    Cnf_label_dat0,        // 0 //
    Cnf_label_dat0 + 13,   // 1 //
    Cnf_label_dat0 + 25,   // 2 //
    Cnf_label_dat0 + 38,   // 3 //
    Cnf_label_dat0 + 50,   // 4 //
    Cnf_label_dat0 + 57,   // 5 //
    Cnf_label_dat0 + 69,   // 6 //
    Cnf_label_dat1,        // 7 //
};

// x position of each action row label
UCHAR Cnf_labelx_tbl[8] =
{
    0x1A,   // 0 //
    0xB2,   // 1 //
    0x1A,   // 2 //
    0xB2,   // 3 //
    0x32,   // 4 //
    0xB2,   // 5 //
    0x32,   // 6 //
    0xB2,   // 7 //
};

// pad button bit shown in each cell of the button list
USHORT Cnf_btn_tbl[12] =
{
    0x1000,   // 0 //
    0x0010,   // 1 //
    0x4000,   // 2 //
    0x0080,   // 3 //
    0x2000,   // 4 //
    0x0020,   // 5 //
    0x8000,   // 6 //
    0x0040,   // 7 //
    0x0004,   // 8 //
    0x0008,   // 9 //
    0x0001,   // 10 //
    0x0002,   // 11 //
};
