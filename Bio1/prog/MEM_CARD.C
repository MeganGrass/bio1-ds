/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\MEM_CARD.C
 */

#include "MEM_CARD.H"
#include "CUT.H"
#include "EVENT.H"
#include "INVENTORY.H"
#include "MOJIDISP.H"
#include "SND.H"
#include "ST_SUB03.H"
#include <LIBETC.H>
#include <MEMORY.H>
#include <STRINGS.H>
#include "SCRATCHPAD.H"
#include "FILECODE.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "GAME.H"
#include "MONITOR.H"

CFILE_INFO File_info;                            // files found on the card being listed

static INT Mc_status_bak;            // status flag bits 1-2 saved on entry, restored on exit
static UCHAR Mc_port;                      // card slot in use (0 or 1)

static UCHAR Card_ready;               // 1 when the last card info request timed out
static UCHAR Card_poll_req;               // 1 when a card info request was issued this frame
static UCHAR Pad_stop;                      // 1 while menu input is locked out

static INT Ev0;                                  // software card event: I/O complete
static INT Ev1;                                  // software card event: time-out
static INT Ev2;                                  // software card event: new card
static INT Ev3;                                  // software card event: error
static INT Ev10;                                 // hardware card event: I/O complete
static INT Ev11;                                 // hardware card event: time-out
static INT Ev13;                                 // hardware card event: error

static UCHAR Mc_pad[0x30];         // padding: keeps the following data at its linked position

VOID Cursor_disp(INT X, INT Y, INT hidden)
{
    GsSPRITE* Sub_ctr;

    if (hidden == 0)
    {
        Sub_ctr = &ScratchPad->Spr;
        Sub_ctr->x = X - 0xA0;
        Sub_ctr->y = Y - 0x78;
        VIEW(INT, Sub_ctr->w) = 0xE0008;  // INT* view of the field (a plain access changes the code)
        Sub_ctr->tpage = 0x1E;
        VIEW(USHORT, Sub_ctr->u) = 0x1C10;  // USHORT* view of the field (a plain access changes the code)

        Sub_ctr->attribute &= 0xBFFFFFFF;

        VIEW(INT, Sub_ctr->cx) = 0x01E00100;  // INT* view of the field (a plain access changes the code)
        GsSortFastSprite(Sub_ctr, MAIN.Ot0 + G.Cbuf_id, 2);
    }
}

INT Card_access(INT Load_flg, UCHAR* Save_buf, INT Ribbon_flg, UCHAR Se_bank)
{
    CFILE_INFO Finfo;
    INT Rsv;
    UCHAR Card_mess[72];
    UCHAR F_num[8];
    UCHAR Cursor_bak;
    UCHAR cursor_prev;
    INT Total_size;
    UCHAR Key_wait;
    UCHAR Cdisp;
    UCHAR sel_cursor;
    UCHAR Rno;
    UCHAR Cursor;
    UCHAR Err_code;
    UCHAR cursor_max;
    UCHAR mode;
    INT blink_cnt;
    USHORT pad;
    INT X;
    UCHAR idx;
    UCHAR* p;
    UCHAR* pTitle;

    Cursor = 0;
    Rno = 0;
    mode = 0;

    blink_cnt = 0;

    Cardaccess_init(Se_bank);

    Pad_stop = 1;

    while (1)
    {
        Card_poll_req = 0;
        if (Pad_stop == 0)
        {
            if (G.Padd1_trg & (PAD_SELECT | PAD_UP | PAD_DOWN))
            {
                Key_wait = 6;
                goto input;
            }
            if (Key_wait == 0)
            {
                Key_wait = 2;
            input:
                if (Rno == 6 || Rno == 2)
                {
                    pad = G.Padd1;
                    if ((pad & 0x4000) || (pad & 0x100))
                    {
                        blink_cnt = 0;
                        Cursor++;
                        Snd_se_play(Se_bank, 0x1E, 0);
                    }
                    else if (pad & 0x1000)
                    {
                        blink_cnt = 0;
                        Cursor--;
                        Snd_se_play(Se_bank, 0x1E, 0);
                    }
                    if ((SCHAR)Cursor < 0)  // sign-extends the byte
                    {
                        Cursor = cursor_max;
                    }
                    if (Cursor > cursor_max)
                    {
                        Cursor = 0;
                    }
                }
                else if ((G.Padd1 & PAD_DOWN) && Cursor < cursor_max)
                {
                    blink_cnt = 0;
                    Cursor++;

                    Snd_se_play(Se_bank, 0x1B, 0);
                }
                else if ((G.Padd1 & PAD_SELECT) && cursor_max != 0)
                {
                    blink_cnt = 0;
                    Cursor++;
                    Snd_se_play(Se_bank, 0x1B, 0);
                    if (Cursor == cursor_max + 1)
                    {
                        Cursor = 0;
                    }
                }
                else if ((G.Padd1 & PAD_UP) && Cursor != 0)
                {
                    blink_cnt = 0;
                    Cursor--;
                    Snd_se_play(Se_bank, 0x1B, 0);
                }

                if (cursor_max != 0 && cursor_max < Cursor)
                {
                    Cursor = cursor_max;
                }
            }
            else
            {
                Key_wait--;
            }
        }

        if (Cursor != cursor_prev)
        {
            blink_cnt = 0;
        }
        blink_cnt = (blink_cnt + 1) & 0x1F;
        Cdisp = blink_cnt >> 4;

        cursor_prev = Cursor;

        switch (Rno)
        {
        case 0:
            X = Wslot_check();
            if (X == 1 || X == 2)
            {
                Mc_port = X & 1;
                Rno = 4;
            }
            else if (X == 3)
            {
                Mc_port = 0;
                Rno = 3;

                Err_code = 4;
            }
            else
            {
                Rno = 2;

                Pad_stop = 0;
            }
            break;
        case 1:
            mode = 0;
            cursor_max = 1;

            if ((G.SaveData.Key_trg & 0x4000) || (G.Padd_trg & PAD_START))
            {
                Snd_se_play(Se_bank, 0x1D, 0);
                if (Cursor == 1)
                {
                    goto exit_card;
                }
                Cursor = 0;

                Pad_stop = 1;

                Rno = 0;
            }
            else if (G.SaveData.Key_trg & 0x8000)
            {
                Snd_se_play(Se_bank, 0x1C, 0);
                goto exit_card;
            }
            else
            {
                Mess_print(0x4C, 0x60, 0, Mc_mess_use);
                Mess_print(0x4C, 0x78, 0, Mc_mess_no_use);
                Cursor_disp(0x42, Cursor * 24 + 0x60, Cdisp);
            }
            break;
        case 2:
            mode = 1;
            cursor_max = 2;

            if ((G.SaveData.Key_trg & 0x4000) || (G.Padd_trg & PAD_START))
            {
                Snd_se_play(Se_bank, 0x1D, 0);
                if (Cursor == 2)
                {
                    goto exit_card;
                }

                Rno = 4;

                Pad_stop = 1;

                Mc_port = Cursor;
            }
            else if (G.SaveData.Key_trg & 0x8000)
            {
                Snd_se_play(Se_bank, 0x1C, 0);
                goto exit_card;
            }
            break;
        case 3:
            cursor_max = 0;
            Cursor = 0;
            mode = 0;

            if (Err_code != 4)
            {
                if (Mc_port == 0)
                {
                    Mess_print(0x6C, 0x18, 0, Mc_mess_card1);
                }
                else
                {
                    Mess_print(0x6C, 0x18, 0, Mc_mess_card2);
                }
            }

            Mess_print(Mc_err_pos_tbl[Err_code], 0x60, 1, Mc_err_tbl[Err_code]);

            if ((MAIN.SaveData.Key_trg & (0x4000 | 0x8000)) || (G.Padd_trg & PAD_START))
            {
                Snd_se_play(Se_bank, 0x1D, 0);

                Pad_stop = 0;

                Rno = 1;
                Cursor = 0;
            }
            break;
        case 4:
            mode = 0;

            Mess_print(0x20, 0xB2, 0, Mc_mess_check);
            Mess_print(0x20, 0xC0, 0, Mc_mess_pull1);

            Task_sleep(1);
            Rno = 5;
            break;
        case 5:
            mode = 0;

            Pad_stop = 0;

            cursor_max = 5;

            Err_code = Card_check(Mc_port);
            if (Err_code & 1)
            {
                if (Err_code == 1)
                {
                    Rno = 3;
                    Err_code = 4;
                }
                else if (Err_code == 3)
                {
                    Rno = 3;
                    Err_code = 1;
                }
                break;
            }

            Err_code = Card_format_check(Mc_port);
            if (Err_code == 0)
            {
                Err_code = Check_card(F_num, Save_buf);
                Card_search(Mc_port, &Finfo, "");
                Total_size = Finfo.total_size;
            }

            switch (Err_code)
            {
            case 0:
                if (Load_flg == 1 && File_info.num == 0)
                {
                    Rno = 3;

                    Err_code = 3;
                }
                else if (Load_flg == 0 && File_info.num == 0 && Total_size == 0x1E000)
                {
                    Rno = 3;
                    Err_code = 2;
                }
                else
                {
                    Cursor = 0;
                    Rno = 6;
                }
                break;
            case 1:
                Rno = 3;
                Err_code = 4;
                break;

            case 2:
                if (Load_flg == 1)
                {
                    Rno = 3;
                    Err_code = 6;
                }
                else
                {
                    Cursor = 1;

                    Rno = 10;
                }

                break;
            case 3:
                Rno = 3;
                Err_code = 1;
                break;
            }

            break;
        case 6:
            cursor_max = 5;
            mode = 2;

            if ((G_BASE->SaveData.Key_trg & 0x4000) || (G.Padd_trg & PAD_START))
            {
                if (Cursor == 5)
                {
                    Snd_se_play(Se_bank, 0x1C, 0);
                    goto exit_card;
                }
                Snd_se_play(Se_bank, 0x1F, 0);

                sel_cursor = Cursor;

                if (Load_flg == 0 && Card_search(Mc_port, &Finfo, "BASLUS-00551"
                                                                       "*") != 0)
                {
                    VSync(0);
                    Err_code = Card_check(Mc_port);

                    if (Err_code != 0)
                    {
                        Rno = 3;

                        Err_code = 4;
                        break;
                    }
                }

                Pad_stop = 1;

                Rno = 7;
            }
            else if (G_BASE->SaveData.Key_trg & 0x8000)
            {
                Snd_se_play(Se_bank, 0x1C, 0);
                Cursor = 0;

                mode = 0;

                Rno = 14;
            }
            else if (Card_poll(Mc_port) != 0)
            {
                Mc_port = 0;
                Rno = 3;
                Err_code = 4;
            }
            else
            {
                Cursor_disp(0x26, Cursor * 20 + 0x38, Cdisp);
            }
            break;
        case 7:
            Cursor = sel_cursor;

            if (Load_flg == 0)
            {
                pTitle = Finfo.title[Cursor];
                *(MC_FNAME_BUF*)pTitle = *(MC_FNAME_BUF*)"BASLUS-00551"
                                           "0";
                pTitle[12] += Cursor;

                Rno = 13;

                if (F_num[Cursor] < 5)
                {
                    Cursor_bak = Cursor;

                    Pad_stop = 0;
                    Cursor = 1;
                }
                else
                {
                    Rno = 15;

                    if (Total_size == 0x1E000)
                    {
                        Err_code = 2;
                        Rno = 12;

                        Cursor_bak = Cursor;
                    }
                }
            }
            else
            {
                Rno = 9;
                if (F_num[Cursor] == 5)
                {
                    Err_code = 3;
                    Rno = 12;

                    Pad_stop = 0;

                    Cursor_bak = Cursor;
                }
            }
            break;
        case 8:
            Cursor = sel_cursor;

            idx = Cursor;

            Save_push();

            p = Save_buf + 0xA00 + idx * 0x240;
            Memcpy(p, &MAIN.SaveData.Stage_no, 0x13C);

            Memcpy(&ScratchWord[1], Item_pix_idx, 9);
            ScratchWord[0] = ITEM_NUM_CUR;
            if (Ribbon_flg != 0)
            {
                G.SaveData.Sel_id = 0x2F;
                Mess_item_use();
            }

            Card_mess_make(Card_mess, p, idx);

            if ((UCHAR)Card_write(&MAIN.SaveData.Card_Head, Mc_port, Finfo.title[idx], Card_mess, 0x800) == 0)  // keeps the low byte of the result (the compare is on a UCHAR)
            {
                F_num[idx] = 0;
                Card_save_anim(idx, Load_flg, Save_buf, F_num, Se_bank);

                Err_code = 0;
                SAVE_CNT_CUR++;
                goto out;
            }

            Memcpy(&MAIN.SaveData.Item_work[0].Id, p + 0x124, 0x18);

            Memcpy(Item_pix_idx, &ScratchWord[1], 9);
            Rno = 3;

            MAIN.SaveData.Item_num = ScratchWord[0];

            Err_code = 5;

            Pad_stop = 0;

            break;
        case 9:
            Memcpy(&MAIN.SaveData.Stage_no, (UCHAR*)(Save_buf + 0xA00 + sel_cursor * 0x240), 0x240);  // byte view of the same memory

            Err_code = 0;

            Load_pop();

            goto out;
        case 10:
            mode = 3;
            cursor_max = 1;

            if ((G_BASE->SaveData.Key_trg & 0x4000) || (G.Padd_trg & PAD_START))
            {
                Snd_se_play(Se_bank, 0x1D, 0);

                if (Cursor != 0)
                {
                    Cursor = 0;
                    Rno = 1;
                }
                else
                {
                    Rno = 11;
                }

                break;
            }

            if (G.SaveData.Key_trg & 0x8000)
            {
                Snd_se_play(Se_bank, 0x1C, 0);

                Pad_stop = 1;

                mode = 0;
                Rno = 0;
                Cursor = 0;
            }

            if (Card_poll(Mc_port) != 0)
            {
                Mc_port = 0;
                Rno = 3;
                Err_code = 4;
            }

            break;
        case 11:
            mode = 0;

            idx = Card_check(Mc_port);
            if (idx == 0)
            {
                Err_code = Card_format_check(Mc_port);
                if (Err_code == 2 && Card_format(Mc_port) == 0)
                {
                    Pad_stop = 1;

                    Rno = 4;
                }
                else
                {
                    Err_code = 0;
                    Rno = 3;
                }
            }
            else
            {
                Err_code = (idx == 1) * 4;

                Cursor = 0;
                Rno = 3;
            }
            break;
        case 12:
            cursor_max = 0;
            Cursor = 0;

            if ((MAIN.SaveData.Key_trg & (0x4000 | 0x8000)) || (G.Padd_trg & PAD_START))
            {
                Snd_se_play(Se_bank, 0x1D, 0);

                Pad_stop = 0;

                if (Err_code == 2)
                {
                    Rno = 1;
                }
                else
                {
                    Rno = 6;

                    Cursor = Cursor_bak;
                    cursor_max = Cursor;
                }
            }
            Mess_print(Mc_err_pos_tbl[Err_code], 0xC4, 1, Mc_err_tbl[Err_code]);

            if (Card_poll(Mc_port) != 0)
            {
                Mc_port = 0;
                Rno = 3;
                Err_code = 4;
            }

            break;
        case 13:
            cursor_max = 1;

            if ((G_BASE->SaveData.Key_trg & 0x4000) || (G.Padd_trg & PAD_START))
            {
                Snd_se_play(Se_bank, 0x1D, 0);
                if (Cursor == 1)
                {
                    Cursor = Cursor_bak;

                    Rno = 6;

                    cursor_max = Cursor;
                }
                else
                {
                    Cursor = Cursor_bak;

                    cursor_max = Cursor;

                    Pad_stop = 1;
                    Rno = 15;
                }

                break;
            }
            if (G_BASE->SaveData.Key_trg & 0x8000)
            {
                Snd_se_play(Se_bank, 0x1C, 0);
                Cursor = Cursor_bak;

                Rno = 6;

                cursor_max = Cursor;
            }

            if (Card_poll(Mc_port) != 0)
            {
                Mc_port = 0;
                Rno = 3;
                Err_code = 4;
            }

            break;

        case 14:
            if (Card_check(Mc_port ^ 1) != 0)
            {
                goto exit_card;
            }
            Cursor = 0;
            Rno = 2;
            break;
        case 15:
        case 16:
            Mess_print(0x20, 0xB2, 0, Mc_mess_saving);
            Mess_print(0x20, 0xC0, 0, Mc_mess_pull2);
            Rno++;
            if (Rno == 17)
            {
                Rno = 8;
            }

            break;
        }

        Card_ready = 0;
        if (Card_poll_req != 0 && Get_card_event() == 1)
        {
            Card_ready = 1;
        }

        switch (mode)
        {
        case 0:
            break;
        case 1:
            Card_menu_disp(Load_flg);

            Cursor_disp(0x62, Cursor * 24 + 0x60, Cdisp);

            break;
        case 2:
            Card_file_disp(Load_flg, F_num, Save_buf, 0, 0x1A);
            if (Rno == 13)
            {
                Mess_print(0x78, 0xB2, 1, Mc_mess_overwrite);
                Mess_print(0x94, 0xC4, 0, Mc_mess_yes);
                Mess_print(0x94, 0xD6, 0, Mc_mess_no);

                Cursor_disp(0x8A, Cursor * 18 + 0xC4, Cdisp);
            }
            break;
        case 3:
            Card_prompt_disp();

            Cursor_disp(0x3A, Cursor * 24 + 0x60, Cdisp);

            break;
        }

        Task_sleep(1);
    }
exit_card:
    Err_code = 1;
out:
    Cardaccess_exit(Se_bank);

    return Err_code;
}

VOID Cardaccess_init(INT Se_bank)
{
    INT status_flags;
    INT Status_flg2;

    G.Vloop = 2;

    MAIN.System_flg = (MAIN.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;
    status_flags = STATUS_FLG_CUR;
    Mc_status_bak = status_flags & (STAT_QUAKE | STAT_BG_BORDER);
    Status_flg2 = status_flags & ~(STAT_QUAKE | STAT_BG_BORDER);
    MAIN.Status_flg = Status_flg2;
    if ((Se_bank & 0xFF) == 2)
    {
        MAIN.Status_flg = Status_flg2 | STAT_CARD_SE;
    }
    Task_sleep(2);

    Cd_read(TYPE00_TIM, TIM_BUF);

    G.Fade_hrate = 2;
    G.Fade_add = -0x400;

    MAIN.System_flg = (MAIN.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_IMAGE;

    Evt_fade_start();

    do
    {
        Task_sleep(1);
    } while (G.SaveData.Fade_kido >= 0);
    Task_sleep(1);
}

INT Card_poll(INT slot)
{
    if (_card_info(slot * 0x10) != 0)
    {
        Card_poll_req = 1;
        return Card_ready != 0;
    }
    return 0;
}

VOID Cardaccess_exit(INT Mode)
{
    G.Fade_hrate = 2;
    G.Fade_add = 0x400;
    Evt_fade_start();

    if (G.SaveData.Fade_kido >= 0)
    {
        do
        {
            Task_sleep(1);
        } while (G.SaveData.Fade_kido >= 0);
    }

    G.Vloop = 2;

    Task_sleep(1);

    if ((Mode & 0xFF) == 2)
    {
        Bg_draw();

        MAIN.System_flg = (MAIN.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_IMAGE;
    }
    else
    {
        Vram_clr(0, 0, 0x140, 0x1E0);
    }

    MAIN.Status_flg = (STATUS_FLG_CUR | Mc_status_bak) & ~STAT_CARD_SE;
}

VOID Search_f_num(UCHAR* F_num)
{
    UINT i;

    i = 0;
    do
    {
        F_num[i] = 5;
        i++;
    } while (i < 5);
    for (i = 0; i < 5; i++)
    {
        if (i >= File_info.num)
        {
            break;
        }
        {
            INT num = File_info.title[i][12] - '0';
            *(F_num + num) = i;
        }
    }
}

VOID Card_menu_disp(INT Load_flg)
{
    if (Load_flg == 0)
    {
        Mess_print(0x50, 0x40, 0, Mc_mess_sv_card);
        Mess_print(0x6C, 0x90, 0, Mc_mess_save);
    }
    else
    {
        Mess_print(0x48, 0x40, 0, Mc_mess_ld_card);
        Mess_print(0x6C, 0x90, 0, Mc_mess_load);
    }
    Mess_print(0x6C, 0x60, 0, Mc_mess_card1);
    Mess_print(0x6C, 0x78, 0, Mc_mess_card2);
    Mess_print(0x6C, 0x90, 0, Mc_mess_do_not);
}

INT Card_file_disp(INT Load_flg, UCHAR* F_num, UCHAR* Save_buf, UCHAR Sel_file, UCHAR Sel_char)
{
    INT i;
    INT pRet;

    INT Col;

    pRet = 0;
    for (i = 0; i < 5; i++)
    {
        *ScratchPad->Text_buf = i + 0xD;

        *(ScratchPad->Text_buf + 1) = 0x79;
        ScratchPad->Text_buf[2] = 1;

        Mess_print(0x30, i * 0x14 + 0x38, 0, ScratchPad->Text_buf); // buffer address passed as an integer argument
        ScratchPad->Text_buf[0] = 0;
        ScratchPad->Text_buf[1] = 0;
        Col = 0;

        if (F_num[i] < 5)
        {
            if (((UCHAR*)Save_buf + i * 0x240)[0xA2B] & 3)  // byte view of the same memory
            {
                Memcpy(&ScratchPad->Text_buf[2], Mc_mess_jill, 9);
            }
            else
            {
                Memcpy(&ScratchPad->Text_buf[2], Mc_mess_chris, 9);
            }

            Memcpy(&ScratchPad->Text_buf[11], Mc_area_tbl[Save_area_get((SAVE_POS_REC*)(Save_buf + 0xA00 + i * 0x240)) & 0xFF], 0x22);
            {
                UCHAR t = ((UCHAR*)Save_buf + i * 0x240)[0xA28];  // byte view of the same memory
                *(ScratchPad->Text_buf + 8) += t / 10;
                *(ScratchPad->Text_buf + 9) += ((UCHAR*)Save_buf + i * 0x240)[0xA28] % 10;  // UCHAR* view of the same memory
            }

            Col = ((UCHAR*)Save_buf + i * 0x240)[0xA33];  // byte view of the same memory
        }
        else
        {
            Memcpy(&ScratchPad->Text_buf[2], Mc_mess_no_data, 0x12);
        }
        if (i == Sel_file)
        {
            if (Sel_char >= 9)
            {
                Sel_char += Sel_char - 9;
            }

            if (ScratchPad->Text_buf[2 + Sel_char] != 0)
            {
                pRet = 1;
            }

            ScratchPad->Text_buf[2 + Sel_char] = 1;
        }

        if (F_num[i] < 5)
        {
            if (((UCHAR*)Save_buf + i * 0x240)[0xA33] == 3)  // byte view of the same memory
            {
                Col = 1;
                if (*(ScratchPad->Text_buf + 7) != 1)
                {
                    Col = 2;
                    ScratchPad->Text_buf[7] = 1;
                    Mess_print(0x30, i * 0x14 + 0x38, 0x11, ScratchPad->Text_buf); // buffer address passed as an integer argument
                    ScratchPad->Text_buf[7] = 0x38;
                }
            }
        }
        Mess_print(0x30, i * 0x14 + 0x38, Col, ScratchPad->Text_buf); // buffer address passed as an integer argument
    }
    Mess_print(0x30, 0x9C, 0, Mc_mess_do_not);
    if (Load_flg == 0)
    {
        Mess_print(0x30, 0x9C, 0, Mc_mess_save);
    }
    else
    {
        Mess_print(0x30, 0x9C, 0, Mc_mess_load);
    }
    if (Mc_port == 0)
    {
        Mess_print(0x6C, 0x18, 0, Mc_mess_card1);
    }
    else
    {
        Mess_print(0x6C, 0x18, 0, Mc_mess_card2);
    }
    return pRet;
}

VOID Card_prompt_disp(VOID)
{
    if (Mc_port == 0)
    {
        Mess_print(0x6C, 0x18, 0, Mc_mess_card1);
    }
    else
    {
        Mess_print(0x6C, 0x18, 0, Mc_mess_card2);
    }

    Mess_print(0x48, 0x40, 1, Mc_mess_fmt_req);

    Mess_print(0x44, 0x60, 0, Mc_mess_fmt_yes);
    Mess_print(0x44, 0x78, 0, Mc_mess_fmt_no);
}

VOID Save_push(VOID)
{
    G.SaveData.Pl_id = G.Player.Id;

    G.SaveData.Pl_pos_x = G.Player.Pos_x;

    G.SaveData.Pl_pos_z = G.Player.Pos_z;

    G.SaveData.Pl_cdir_y = G.Player.Cdir_y;
    G.SaveData.Pl_sca_info = G.Player.Sca_info;
}

VOID Load_pop(VOID)
{
    INT flags;

    MAIN.System_flg |= SYS_CARD_LOAD;

    if (DC_MODE_CUR == 1)
    {
        STATUS_FLG_CUR |= STAT_EASY_MODE;
    }
    if (MAIN.SaveData.Dc_mode == 2)
    {
        STATUS_FLG_CUR |= STAT_ARRANGE;
    }
    if (MAIN.SaveData.Dc_mode == 3)
    {
        flags = STATUS_FLG_CUR | STAT_ARRANGE;
        MAIN.Status_flg = flags | STAT_ARRANGE_EASY;
    }

    G.Player.Id = G.SaveData.Pl_id;
    if (G.SaveData.Pl_id & 3)
    {
        MAIN.System_flg |= SYS_PLAYER;
    }
}

VOID Card_save_anim(UCHAR file, INT Load_flg, UCHAR* Save_buf, UCHAR* F_num, UCHAR Se_bank)
{
    INT i;
    INT found;
    INT n;

    Memcpy((UCHAR*)(Save_buf + 0xA00 + file * 0x240), &MAIN.SaveData.Stage_no, 0x34);  // address passed as an integer

    found = 0;
    i = 0;
    do
    {
        if (found != 0)
        {
            Snd_se_play(Se_bank & 0xFF, 0x1F, 0);
        }

        n = Mc_wait_tbl[i] >> 1;

        if (n > 0)
        {
            do
            {
                found = Card_file_disp(Load_flg, F_num, Save_buf, file, i & 0xFF);
                Task_sleep(1);
                n--;
            } while (n > 0);
        }
        i++;
    } while (i < 0x1B);
}

VOID Card_mess_make(UCHAR* Mess, VOID* Save_data, INT Cursor)
{
    UCHAR* p;
    INT* flags;

    UCHAR* num;

    if (G.SaveData.Pl_id & 1)
    {
        Memcpy(Mess, "\202\161\201\104\202\144\201\174\202\143\202\142\201\100\201\100\201\100\201\100\202\120"
                     "\201\104\201\100\202\151\202\211\202\214\202\214\201\100\201\136\202\117\202\117\201\136",
            0x2C);
    }
    else
    {
        Memcpy(Mess, "\202\161\201\104\202\144\201\174\202\143\202\142\201\100\201\100\201\100\201\100\202\120"
                     "\201\104\201\100\202\142\202\210\202\222\202\211\202\223\201\136\202\117\202\117\201\136",
            0x2C);
    }
    p = Mess + 0xE;
    Memcpy(p, "\201\100\201\100", 4);
    flags = &MAIN.Status_flg;
    if (*flags & STAT_EASY_MODE)
    {
        Memcpy(p, "\202\163\202\222", 4);
    }
    if (*flags & STAT_ARRANGE)
    {
        Memcpy(p, "\202\140\202\204", 4);
    }
    if (*flags & STAT_ARRANGE_EASY)
    {
        Memcpy(p, "\202\140\202\163", 4);
    }

    num = &MAIN.SaveData.Save_cnt;

    Mess[0x15] += Cursor;

    Mess[0x27] += *num / 10;
    Mess[0x29] += *num % 10;

    Memcpy(Mess + 0x2C, Mc_area_name[Save_area_get(Save_data) & 0xFF], 0x15);
}

INT Save_area_get(SAVE_POS_REC* pSave_pos)
{
    INT area;
    UCHAR room;

    area = (pSave_pos->stage % 5) & 0xFF;
    if (area == 0)
    {
        room = pSave_pos->room;
        if (room == 6)
        {
            area = 1;
        }
        if (area == 0 && room == 0x18)
        {
            area = 5;
        }
    }
    if (area == 2 && pSave_pos->room == 7)
    {
        area = 6;
    }
    return area;
}

INT Wslot_check(VOID)
{
    UCHAR Slot0;
    INT Ret2;

    Mess_print(0x20, 0xB2, 0, Mc_mess_check);
    Mess_print(0x20, 0xC0, 0, Mc_mess_pull1);

    Task_sleep(2);
    Slot0 = Card_check(0) & 1;
    return Slot0 | ((Ret2 = Card_check(1) & 1) * 2);
}

INT Check_card(UCHAR* F_num, UCHAR* buf)
{
    UINT i;
    INT off;
    UINT c;
    INT Status;

    Card_search(Mc_port, &File_info, "BASLUS-00551*");

    Search_f_num(F_num);
    if (File_info.num != 0)
    {
        for (i = 0, off = 0xA00; i < 5; i++, off += 0x240)
        {
            c = *F_num++;

            if (c == 5)
            {
                continue;
            }

            if (Card_read(buf, Mc_port, File_info.title[c], 0xA00) != 0)
            {
                goto err;
            }

            Memcpy(buf + off, buf + 0x200, 0x240);
        }
        return 0;
    }
err:
    Status = Card_check(Mc_port);

    if (Status != 0)
    {
        if (Status == 3)
        {
            return 3;
        }
        return Card_format_check(Mc_port);
    }

    return 0;
}

VOID Card_event_set(VOID)
{
    EnterCriticalSection();
    Ev0 = OpenEvent(0xF4000001, 4, 0x2000, 0);
    Ev1 = OpenEvent(0xF4000001, 0x100, 0x2000, 0);
    Ev2 = OpenEvent(0xF4000001, 0x2000, 0x2000, 0);
    Ev3 = OpenEvent(0xF4000001, 0x8000, 0x2000, 0);
    Ev10 = OpenEvent(0xF0000011, 4, 0x2000, 0);
    Ev11 = OpenEvent(0xF0000011, 0x100, 0x2000, 0);
    Ev13 = OpenEvent(0xF0000011, 0x8000, 0x2000, 0);
    ExitCriticalSection();
    EnableEvent(Ev0);
    EnableEvent(Ev1);
    EnableEvent(Ev2);
    EnableEvent(Ev3);
    EnableEvent(Ev10);
    EnableEvent(Ev11);
    EnableEvent(Ev13);
}

VOID Clear_event(VOID)
{
    TestEvent(Ev0);
    TestEvent(Ev1);
    TestEvent(Ev2);
    TestEvent(Ev3);
}

VOID Clear_event2(VOID)
{
    TestEvent(Ev10);
    TestEvent(Ev11);
    TestEvent(Ev13);
}

const MC_DEV_BUF Mc_fname_tmpl = { "bu00:" };  // card device path template "bu00:"

INT Card_write(_CARD* Head, INT Port, CHAR* Name, CHAR* Title, UINT Cap)
{
    _CARD Cardinf;
    struct
    {
        MC_DEV_BUF s;
        CHAR rest[14];
    } Filename;
    UINT Cnt;
    INT i;
    INT Fd;
    INT Timer;
    UINT width;

    Filename.s = Mc_fname_tmpl;

    memset(Filename.rest, 0, 0xE);
    width = Cap;
    Head->blockEntry = width / 0x2000 + (width % 0x2000 != 0);

    Filename.s.s[2] = Port + Filename.s.s[2];

    strcpy(&Filename.s.s[5], Name);
    strcpy(Head->title, Title);
    close(open(Filename.s.s, (Head->blockEntry << 16) | 0x200));
    Cnt = 0;
    do
    {
        i = 0x1E;
        do
        {
            Fd = open(Filename.s.s, 2);
            if (Fd != -1)
            {
                goto opened;
            }
            VSync(2);
        } while (--i != 0);
        return 1;
    opened:
        i = 0x1E;
        do
        {
            if (write(Fd, Head, width) == width)
            {
                goto written;
            }
            VSync(2);
        } while (--i != 0);
        return 2;
    written:
        close(Fd);
        Fd = open(Filename.s.s, 1);
        if (Fd == -1)
        {
            break;
        }
        Timer = read(Fd, &Cardinf, sizeof(_CARD));
        close(Fd);
        Cnt++;
        if (Timer == sizeof(_CARD))
        {
            return 0;
        }
    } while (Cnt < 3);
    return 2;
}

INT Card_read(UCHAR* Head, INT Port, CHAR* Name)
{
    struct
    {
        MC_DEV_BUF s;
        CHAR rest[14];
    } Filename;

    INT Cnt;
    INT Fd;

    do
    {
        Filename.s = Mc_fname_tmpl;

        memset(Filename.rest, 0, 0xE);
        Filename.s.s[2] += Port;
        strcpy(&Filename.s.s[5], Name);
    } while (0);
    Cnt = 30;
    do
    {
        Fd = open(Filename.s.s, 1);
        if (Fd != -1)
        {
            goto opened;
        }
        VSync(1);
    } while (--Cnt != 0);
    return 1;
opened:
    Cnt = 30;
    do
    {
        if (read(Fd, Head, 0x800) == 0x800)
        {
            close(Fd);
            return 0;
        }
        VSync(1);
    } while (--Cnt != 0);
    return 2;
}

INT Card_search(INT Port, CFILE_INFO* Buff, CHAR* Pattern)
{
    struct DIRENTRY Dir;
    struct
    {
        MC_DEV_BUF s;

        CHAR rest[26];
    } Key;

    Key.s = Mc_fname_tmpl;

    memset(Key.rest, 0, 0x1A);

    strcpy(&Key.s.s[5], Pattern);
    Key.s.s[2] += Port;
    Buff->num = 0;
    Buff->total_size = 0;
    if (firstfile(Key.s.s, &Dir) != &Dir)
    {
        return 1;
    }
    {
        do
        {
            strcpy(Buff->title[Buff->num], Dir.name);
            Buff->size[Buff->num] = Dir.size;
            Buff->total_size += Dir.size;
            Buff->num++;
        } while (nextfile(&Dir) == &Dir);
    }

    return 0;
}

INT Card_format_check(INT Port)
{
    INT Tries;
    INT Event;

    Tries = 3;

    Clear_event();

    do
    {
        if (_card_load(Port * 0x10) != 0)
        {
            Event = Get_card_event();

            if (Event == 0)
            {
                return 0;
            }
        }
    } while (--Tries);

    return Event;
}

INT Card_check(INT Port)
{
    INT Ret;
    INT Cnt;
    INT Old_ret;

    Ret = Ret;  // self-assignment: shapes register allocation
    Cnt = 0x10;

    Old_ret = 0xFF;

    Clear_event();

loop_1:
    if (_card_info(Port * 0x10) != 0)
    {
        Ret = Get_card_event();

        if ((Ret & 1) || Ret != Old_ret)
        {
            Old_ret = Ret;
            goto block_5;
        }
    }
    else
    {
    block_5:
        Cnt -= 1;
        if (Cnt != 0)
        {
            goto loop_1;
        }
    }

    if (Ret == 2)
    {
        Ret = 0;
    }
    if (Ret == 0)
    {
        Card_clear(Port);
    }

    return Ret;
}

INT Card_format(INT Port)
{
    MC_DEV_BUF Key;

    Key = Mc_fname_tmpl;

    Key.s[2] += Port;
    return format(Key.s) ^ 1;
}

INT Get_card_event(VOID)
{
    INT Cnt;

    for (Cnt = 0x3D08F; Cnt != 0; Cnt--)
    {
        if (TestEvent(Ev0))
        {
            return 0;
        }
        if (TestEvent(Ev1))
        {
            return 1;
        }
        if (TestEvent(Ev2))
        {
            return 2;
        }
        if (TestEvent(Ev3))
        {
            return 3;
        }
    }
    return 3;
}

INT Get_card_event2(VOID)
{
    INT Cnt;

    for (Cnt = 0x3D08F; Cnt != 0; Cnt--)
    {
        if (TestEvent(Ev10))
        {
            return 0;
        }

        if (TestEvent(Ev11))
        {
            return 1;
        }
        if (TestEvent(Ev13))
        {
            return 2;
        }
    }
    return 3;
}

INT Card_clear(INT Port)
{
    INT Tries;
    INT Event;

    Tries = 10;
    Event = 2;
    Clear_event2();
    do
    {
        if (_card_clear(Port * 0x10) != 0)
        {
            Event = Get_card_event2();
            if (Event == 0)
            {
                break;
            }
        }
    } while (--Tries);
    return Event;
}

VOID Card_rename(INT unused, CHAR* Src_name, CHAR* Dst_name)
{
    Mc_src_path[2] = Mc_src_path[2] + '0' + Mc_port;
    Mc_dst_path[2] = Mc_dst_path[2] + '0' + Mc_port;
    strcpy(&Mc_src_path[5], Src_name);
    strcpy(&Mc_dst_path[5], Dst_name);
    rename(Mc_src_path, Mc_dst_path);
}

// text "SAVE"
UCHAR Mc_mess_save[0xC] =
{
    0x00,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x00,   // 3 //
    0x00,   // 4 //
    0x00,   // 5 //
    0x00,   // 6 //
    0x2F,   // 7 //
    0x1D,   // 8 //
    0x32,   // 9 //
    0x21,   // 10 //
    0x01,   // 11 //
};

// text "LOAD"
UCHAR Mc_mess_load[0xC] =
{
    0x00,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x00,   // 3 //
    0x00,   // 4 //
    0x00,   // 5 //
    0x00,   // 6 //
    0x28,   // 7 //
    0x2B,   // 8 //
    0x1D,   // 9 //
    0x20,   // 10 //
    0x01,   // 11 //
};

// text "MEMORY CARD 1"
UCHAR Mc_mess_card1[0x10] =
{
    0x29,   // 0 //
    0x21,   // 1 //
    0x29,   // 2 //
    0x2B,   // 3 //
    0x2E,   // 4 //
    0x35,   // 5 //
    0x00,   // 6 //
    0x1F,   // 7 //
    0x1D,   // 8 //
    0x2E,   // 9 //
    0x20,   // 10 //
    0x00,   // 11 //
    0x0D,   // 12 //
    0x01,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
};

// text "MEMORY CARD 2"
UCHAR Mc_mess_card2[0x10] =
{
    0x29,   // 0 //
    0x21,   // 1 //
    0x29,   // 2 //
    0x2B,   // 3 //
    0x2E,   // 4 //
    0x35,   // 5 //
    0x00,   // 6 //
    0x1F,   // 7 //
    0x1D,   // 8 //
    0x2E,   // 9 //
    0x20,   // 10 //
    0x00,   // 11 //
    0x0E,   // 12 //
    0x01,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
};

// text "DO NOT"
UCHAR Mc_mess_do_not[0x8] =
{
    0x20,   // 0 //
    0x2B,   // 1 //
    0x00,   // 2 //
    0x2A,   // 3 //
    0x2B,   // 4 //
    0x30,   // 5 //
    0x01,   // 6 //
    0x00,   // 7 //
};

// text "SAVE ON CARD 1 OR 2?"
UCHAR Mc_mess_sv_card[0x18] =
{
    0x2F,   // 0 //
    0x1D,   // 1 //
    0x32,   // 2 //
    0x21,   // 3 //
    0x00,   // 4 //
    0x2B,   // 5 //
    0x2A,   // 6 //
    0x00,   // 7 //
    0x1F,   // 8 //
    0x1D,   // 9 //
    0x2E,   // 10 //
    0x20,   // 11 //
    0x00,   // 12 //
    0x0D,   // 13 //
    0x00,   // 14 //
    0x2B,   // 15 //
    0x2E,   // 16 //
    0x00,   // 17 //
    0x0E,   // 18 //
    0x1B,   // 19 //
    0x01,   // 20 //
    0x00,   // 21 //
    0x00,   // 22 //
    0x00,   // 23 //
};

// text "LOAD FROM CARD 1 OR 2?"
UCHAR Mc_mess_ld_card[0x18] =
{
    0x28,   // 0 //
    0x2B,   // 1 //
    0x1D,   // 2 //
    0x20,   // 3 //
    0x00,   // 4 //
    0x22,   // 5 //
    0x2E,   // 6 //
    0x2B,   // 7 //
    0x29,   // 8 //
    0x00,   // 9 //
    0x1F,   // 10 //
    0x1D,   // 11 //
    0x2E,   // 12 //
    0x20,   // 13 //
    0x00,   // 14 //
    0x0D,   // 15 //
    0x00,   // 16 //
    0x2B,   // 17 //
    0x2E,   // 18 //
    0x00,   // 19 //
    0x0E,   // 20 //
    0x1B,   // 21 //
    0x01,   // 22 //
    0x00,   // 23 //
};

// text "CHECKING MEMORY CARD(S)"
UCHAR Mc_mess_check[0x18] =
{
    0x1F,   // 0 //
    0x24,   // 1 //
    0x21,   // 2 //
    0x1F,   // 3 //
    0x27,   // 4 //
    0x25,   // 5 //
    0x2A,   // 6 //
    0x23,   // 7 //
    0x00,   // 8 //
    0x29,   // 9 //
    0x21,   // 10 //
    0x29,   // 11 //
    0x2B,   // 12 //
    0x2E,   // 13 //
    0x35,   // 14 //
    0x00,   // 15 //
    0x1F,   // 16 //
    0x1D,   // 17 //
    0x2E,   // 18 //
    0x20,   // 19 //
    0x38,   // 20 //
    0x2F,   // 21 //
    0x01,   // 22 //
    0x00,   // 23 //
};

// text "OVERWRITE?"
UCHAR Mc_mess_overwrite[0xC] =
{
    0x2B,   // 0 //
    0x32,   // 1 //
    0x21,   // 2 //
    0x2E,   // 3 //
    0x33,   // 4 //
    0x2E,   // 5 //
    0x25,   // 6 //
    0x30,   // 7 //
    0x21,   // 8 //
    0x1B,   // 9 //
    0x01,   // 10 //
    0x00,   // 11 //
};

// text "YES"
UCHAR Mc_mess_yes[0x4] =
{
    0x35,   // 0 //
    0x21,   // 1 //
    0x2F,   // 2 //
    0x01,   // 3 //
};

// text "NO"
UCHAR Mc_mess_no[0x4] =
{
    0x2A,   // 0 //
    0x2B,   // 1 //
    0x01,   // 2 //
    0x00,   // 3 //
};

// text "FORMATTING IS REQUIRED"
UCHAR Mc_mess_fmt_req[0x18] =
{
    0x22,   // 0 //
    0x2B,   // 1 //
    0x2E,   // 2 //
    0x29,   // 3 //
    0x1D,   // 4 //
    0x30,   // 5 //
    0x30,   // 6 //
    0x25,   // 7 //
    0x2A,   // 8 //
    0x23,   // 9 //
    0x00,   // 10 //
    0x25,   // 11 //
    0x2F,   // 12 //
    0x00,   // 13 //
    0x2E,   // 14 //
    0x21,   // 15 //
    0x2D,   // 16 //
    0x31,   // 17 //
    0x25,   // 18 //
    0x2E,   // 19 //
    0x21,   // 20 //
    0x20,   // 21 //
    0x01,   // 22 //
    0x00,   // 23 //
};

// text "FORMAT THIS CARD"
UCHAR Mc_mess_fmt_yes[0x14] =
{
    0x22,   // 0 //
    0x2B,   // 1 //
    0x2E,   // 2 //
    0x29,   // 3 //
    0x1D,   // 4 //
    0x30,   // 5 //
    0x00,   // 6 //
    0x30,   // 7 //
    0x24,   // 8 //
    0x25,   // 9 //
    0x2F,   // 10 //
    0x00,   // 11 //
    0x1F,   // 12 //
    0x1D,   // 13 //
    0x2E,   // 14 //
    0x20,   // 15 //
    0x01,   // 16 //
    0x00,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
};

// text "DO NOT FORMAT THIS CARD"
UCHAR Mc_mess_fmt_no[0x18] =
{
    0x20,   // 0 //
    0x2B,   // 1 //
    0x00,   // 2 //
    0x2A,   // 3 //
    0x2B,   // 4 //
    0x30,   // 5 //
    0x00,   // 6 //
    0x22,   // 7 //
    0x2B,   // 8 //
    0x2E,   // 9 //
    0x29,   // 10 //
    0x1D,   // 11 //
    0x30,   // 12 //
    0x00,   // 13 //
    0x30,   // 14 //
    0x24,   // 15 //
    0x25,   // 16 //
    0x2F,   // 17 //
    0x00,   // 18 //
    0x1F,   // 19 //
    0x1D,   // 20 //
    0x2E,   // 21 //
    0x20,   // 22 //
    0x01,   // 23 //
};

// text "USE MEMORY CARD"
UCHAR Mc_mess_use[0x10] =
{
    0x31,   // 0 //
    0x2F,   // 1 //
    0x21,   // 2 //
    0x00,   // 3 //
    0x29,   // 4 //
    0x21,   // 5 //
    0x29,   // 6 //
    0x2B,   // 7 //
    0x2E,   // 8 //
    0x35,   // 9 //
    0x00,   // 10 //
    0x1F,   // 11 //
    0x1D,   // 12 //
    0x2E,   // 13 //
    0x20,   // 14 //
    0x01,   // 15 //
};

// text "DON'T USE MEMORY CARD"
UCHAR Mc_mess_no_use[0x18] =
{
    0x20,   // 0 //
    0x2B,   // 1 //
    0x2A,   // 2 //
    0x3A,   // 3 //
    0x30,   // 4 //
    0x00,   // 5 //
    0x31,   // 6 //
    0x2F,   // 7 //
    0x21,   // 8 //
    0x00,   // 9 //
    0x29,   // 10 //
    0x21,   // 11 //
    0x29,   // 12 //
    0x2B,   // 13 //
    0x2E,   // 14 //
    0x35,   // 15 //
    0x00,   // 16 //
    0x1F,   // 17 //
    0x1D,   // 18 //
    0x2E,   // 19 //
    0x20,   // 20 //
    0x01,   // 21 //
    0x00,   // 22 //
    0x00,   // 23 //
};

// text "NO DATA"
UCHAR Mc_mess_no_data[0x8] =
{
    0x2A,   // 0 //
    0x2B,   // 1 //
    0x00,   // 2 //
    0x20,   // 3 //
    0x1D,   // 4 //
    0x30,   // 5 //
    0x1D,   // 6 //
    0x01,   // 7 //
};

// text "DO NOT PULL OUT THE MEMORY CARD"
UCHAR Mc_mess_pull1[0x20] =
{
    0x20,   // 0 //
    0x2B,   // 1 //
    0x00,   // 2 //
    0x2A,   // 3 //
    0x2B,   // 4 //
    0x30,   // 5 //
    0x00,   // 6 //
    0x2C,   // 7 //
    0x31,   // 8 //
    0x28,   // 9 //
    0x28,   // 10 //
    0x00,   // 11 //
    0x2B,   // 12 //
    0x31,   // 13 //
    0x30,   // 14 //
    0x00,   // 15 //
    0x30,   // 16 //
    0x24,   // 17 //
    0x21,   // 18 //
    0x00,   // 19 //
    0x29,   // 20 //
    0x21,   // 21 //
    0x29,   // 22 //
    0x2B,   // 23 //
    0x2E,   // 24 //
    0x35,   // 25 //
    0x00,   // 26 //
    0x1F,   // 27 //
    0x1D,   // 28 //
    0x2E,   // 29 //
    0x20,   // 30 //
    0x01,   // 31 //
};

// text "NOW SAVING..."
UCHAR Mc_mess_saving[0x10] =
{
    0x2A,   // 0 //
    0x2B,   // 1 //
    0x33,   // 2 //
    0x00,   // 3 //
    0x2F,   // 4 //
    0x1D,   // 5 //
    0x32,   // 6 //
    0x25,   // 7 //
    0x2A,   // 8 //
    0x23,   // 9 //
    0x79,   // 10 //
    0x79,   // 11 //
    0x79,   // 12 //
    0x01,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
};

// text "DO NOT PULL OUT THE MEMORY CARD"
UCHAR Mc_mess_pull2[0x20] =
{
    0x20,   // 0 //
    0x2B,   // 1 //
    0x00,   // 2 //
    0x2A,   // 3 //
    0x2B,   // 4 //
    0x30,   // 5 //
    0x00,   // 6 //
    0x2C,   // 7 //
    0x31,   // 8 //
    0x28,   // 9 //
    0x28,   // 10 //
    0x00,   // 11 //
    0x2B,   // 12 //
    0x31,   // 13 //
    0x30,   // 14 //
    0x00,   // 15 //
    0x30,   // 16 //
    0x24,   // 17 //
    0x21,   // 18 //
    0x00,   // 19 //
    0x29,   // 20 //
    0x21,   // 21 //
    0x29,   // 22 //
    0x2B,   // 23 //
    0x2E,   // 24 //
    0x35,   // 25 //
    0x00,   // 26 //
    0x1F,   // 27 //
    0x1D,   // 28 //
    0x2E,   // 29 //
    0x20,   // 30 //
    0x01,   // 31 //
};

// text "FAILED TO FORMAT THE CARD"
UCHAR Mc_err00[0x1C] =
{
    0x22,   // 0 //
    0x1D,   // 1 //
    0x25,   // 2 //
    0x28,   // 3 //
    0x21,   // 4 //
    0x20,   // 5 //
    0x00,   // 6 //
    0x30,   // 7 //
    0x2B,   // 8 //
    0x00,   // 9 //
    0x22,   // 10 //
    0x2B,   // 11 //
    0x2E,   // 12 //
    0x29,   // 13 //
    0x1D,   // 14 //
    0x30,   // 15 //
    0x00,   // 16 //
    0x30,   // 17 //
    0x24,   // 18 //
    0x21,   // 19 //
    0x00,   // 20 //
    0x1F,   // 21 //
    0x1D,   // 22 //
    0x2E,   // 23 //
    0x20,   // 24 //
    0x01,   // 25 //
    0x00,   // 26 //
    0x00,   // 27 //
};

// text "ACCESS ERROR"
UCHAR Mc_err01[0x10] =
{
    0x1D,   // 0 //
    0x1F,   // 1 //
    0x1F,   // 2 //
    0x21,   // 3 //
    0x2F,   // 4 //
    0x2F,   // 5 //
    0x00,   // 6 //
    0x21,   // 7 //
    0x2E,   // 8 //
    0x2E,   // 9 //
    0x2B,   // 10 //
    0x2E,   // 11 //
    0x01,   // 12 //
    0x00,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
};

// text "TOO MANY FILES"
UCHAR Mc_err02[0x10] =
{
    0x30,   // 0 //
    0x2B,   // 1 //
    0x2B,   // 2 //
    0x00,   // 3 //
    0x29,   // 4 //
    0x1D,   // 5 //
    0x2A,   // 6 //
    0x35,   // 7 //
    0x00,   // 8 //
    0x22,   // 9 //
    0x25,   // 10 //
    0x28,   // 11 //
    0x21,   // 12 //
    0x2F,   // 13 //
    0x01,   // 14 //
    0x00,   // 15 //
};

// text "THERE IS NO FILE"
UCHAR Mc_err03[0x14] =
{
    0x30,   // 0 //
    0x24,   // 1 //
    0x21,   // 2 //
    0x2E,   // 3 //
    0x21,   // 4 //
    0x00,   // 5 //
    0x25,   // 6 //
    0x2F,   // 7 //
    0x00,   // 8 //
    0x2A,   // 9 //
    0x2B,   // 10 //
    0x00,   // 11 //
    0x22,   // 12 //
    0x25,   // 13 //
    0x28,   // 14 //
    0x21,   // 15 //
    0x01,   // 16 //
    0x00,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
};

// text "MEMORY CARD IS NOT INSERTED"
UCHAR Mc_err04[0x1C] =
{
    0x29,   // 0 //
    0x21,   // 1 //
    0x29,   // 2 //
    0x2B,   // 3 //
    0x2E,   // 4 //
    0x35,   // 5 //
    0x00,   // 6 //
    0x1F,   // 7 //
    0x1D,   // 8 //
    0x2E,   // 9 //
    0x20,   // 10 //
    0x00,   // 11 //
    0x25,   // 12 //
    0x2F,   // 13 //
    0x00,   // 14 //
    0x2A,   // 15 //
    0x2B,   // 16 //
    0x30,   // 17 //
    0x00,   // 18 //
    0x25,   // 19 //
    0x2A,   // 20 //
    0x2F,   // 21 //
    0x21,   // 22 //
    0x2E,   // 23 //
    0x30,   // 24 //
    0x21,   // 25 //
    0x20,   // 26 //
    0x01,   // 27 //
};

// text "FAIL IN SAVE"
UCHAR Mc_err05[0x10] =
{
    0x22,   // 0 //
    0x1D,   // 1 //
    0x25,   // 2 //
    0x28,   // 3 //
    0x00,   // 4 //
    0x25,   // 5 //
    0x2A,   // 6 //
    0x00,   // 7 //
    0x2F,   // 8 //
    0x1D,   // 9 //
    0x32,   // 10 //
    0x21,   // 11 //
    0x01,   // 12 //
    0x00,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
};

// text "FORMATTING IS REQUIRED"
UCHAR Mc_err06[0x18] =
{
    0x22,   // 0 //
    0x2B,   // 1 //
    0x2E,   // 2 //
    0x29,   // 3 //
    0x1D,   // 4 //
    0x30,   // 5 //
    0x30,   // 6 //
    0x25,   // 7 //
    0x2A,   // 8 //
    0x23,   // 9 //
    0x00,   // 10 //
    0x25,   // 11 //
    0x2F,   // 12 //
    0x00,   // 13 //
    0x2E,   // 14 //
    0x21,   // 15 //
    0x2D,   // 16 //
    0x31,   // 17 //
    0x25,   // 18 //
    0x2E,   // 19 //
    0x21,   // 20 //
    0x20,   // 21 //
    0x01,   // 22 //
    0x00,   // 23 //
};

// area text "M.Room 1F", each glyph followed by code 0xFB
UCHAR Mc_area00[0x24] =
{
    0x29,   // 0 //
    0xFB,   // 1 //
    0x79,   // 2 //
    0xFB,   // 3 //
    0x2E,   // 4 //
    0xFB,   // 5 //
    0x4B,   // 6 //
    0xFB,   // 7 //
    0x4B,   // 8 //
    0xFB,   // 9 //
    0x49,   // 10 //
    0xFB,   // 11 //
    0x00,   // 12 //
    0xFB,   // 13 //
    0x0D,   // 14 //
    0xFB,   // 15 //
    0x22,   // 16 //
    0xFB,   // 17 //
    0x00,   // 18 //
    0xFB,   // 19 //
    0x00,   // 20 //
    0xFB,   // 21 //
    0x00,   // 22 //
    0xFB,   // 23 //
    0x00,   // 24 //
    0xFB,   // 25 //
    0x00,   // 26 //
    0xFB,   // 27 //
    0x00,   // 28 //
    0xFB,   // 29 //
    0x00,   // 30 //
    0xFB,   // 31 //
    0x00,   // 32 //
    0xFB,   // 33 //
    0x00,   // 34 //
    0x00,   // 35 //
};

// area text "M.Hall 1F", each glyph followed by code 0xFB
UCHAR Mc_area01[0x24] =
{
    0x29,   // 0 //
    0xFB,   // 1 //
    0x79,   // 2 //
    0xFB,   // 3 //
    0x24,   // 4 //
    0xFB,   // 5 //
    0x3D,   // 6 //
    0xFB,   // 7 //
    0x48,   // 8 //
    0xFB,   // 9 //
    0x48,   // 10 //
    0xFB,   // 11 //
    0x00,   // 12 //
    0xFB,   // 13 //
    0x0D,   // 14 //
    0xFB,   // 15 //
    0x22,   // 16 //
    0xFB,   // 17 //
    0x00,   // 18 //
    0xFB,   // 19 //
    0x00,   // 20 //
    0xFB,   // 21 //
    0x00,   // 22 //
    0xFB,   // 23 //
    0x00,   // 24 //
    0xFB,   // 25 //
    0x00,   // 26 //
    0xFB,   // 27 //
    0x00,   // 28 //
    0xFB,   // 29 //
    0x00,   // 30 //
    0xFB,   // 31 //
    0x00,   // 32 //
    0xFB,   // 33 //
    0x00,   // 34 //
    0x00,   // 35 //
};

// area text "Courtyard Room B1", each glyph followed by code 0xFB
UCHAR Mc_area02[0x24] =
{
    0x1F,   // 0 //
    0xFB,   // 1 //
    0x4B,   // 2 //
    0xFB,   // 3 //
    0x51,   // 4 //
    0xFB,   // 5 //
    0x4E,   // 6 //
    0xFB,   // 7 //
    0x50,   // 8 //
    0xFB,   // 9 //
    0x55,   // 10 //
    0xFB,   // 11 //
    0x3D,   // 12 //
    0xFB,   // 13 //
    0x4E,   // 14 //
    0xFB,   // 15 //
    0x40,   // 16 //
    0xFB,   // 17 //
    0x00,   // 18 //
    0xFB,   // 19 //
    0x2E,   // 20 //
    0xFB,   // 21 //
    0x4B,   // 22 //
    0xFB,   // 23 //
    0x4B,   // 24 //
    0xFB,   // 25 //
    0x49,   // 26 //
    0xFB,   // 27 //
    0x00,   // 28 //
    0xFB,   // 29 //
    0x1E,   // 30 //
    0xFB,   // 31 //
    0x0D,   // 32 //
    0xFB,   // 33 //
    0x00,   // 34 //
    0x00,   // 35 //
};

// area text "Guardhouse 1F", each glyph followed by code 0xFB
UCHAR Mc_area03[0x24] =
{
    0x23,   // 0 //
    0xFB,   // 1 //
    0x51,   // 2 //
    0xFB,   // 3 //
    0x3D,   // 4 //
    0xFB,   // 5 //
    0x4E,   // 6 //
    0xFB,   // 7 //
    0x40,   // 8 //
    0xFB,   // 9 //
    0x44,   // 10 //
    0xFB,   // 11 //
    0x4B,   // 12 //
    0xFB,   // 13 //
    0x51,   // 14 //
    0xFB,   // 15 //
    0x4F,   // 16 //
    0xFB,   // 17 //
    0x41,   // 18 //
    0xFB,   // 19 //
    0x00,   // 20 //
    0xFB,   // 21 //
    0x0D,   // 22 //
    0xFB,   // 23 //
    0x22,   // 24 //
    0xFB,   // 25 //
    0x00,   // 26 //
    0xFB,   // 27 //
    0x00,   // 28 //
    0xFB,   // 29 //
    0x00,   // 30 //
    0xFB,   // 31 //
    0x00,   // 32 //
    0xFB,   // 33 //
    0x00,   // 34 //
    0x00,   // 35 //
};

// area text "Laboratory B3", each glyph followed by code 0xFB
UCHAR Mc_area04[0x24] =
{
    0x28,   // 0 //
    0xFB,   // 1 //
    0x3D,   // 2 //
    0xFB,   // 3 //
    0x3E,   // 4 //
    0xFB,   // 5 //
    0x4B,   // 6 //
    0xFB,   // 7 //
    0x4E,   // 8 //
    0xFB,   // 9 //
    0x3D,   // 10 //
    0xFB,   // 11 //
    0x50,   // 12 //
    0xFB,   // 13 //
    0x4B,   // 14 //
    0xFB,   // 15 //
    0x4E,   // 16 //
    0xFB,   // 17 //
    0x55,   // 18 //
    0xFB,   // 19 //
    0x00,   // 20 //
    0xFB,   // 21 //
    0x1E,   // 22 //
    0xFB,   // 23 //
    0x0F,   // 24 //
    0xFB,   // 25 //
    0x00,   // 26 //
    0xFB,   // 27 //
    0x00,   // 28 //
    0xFB,   // 29 //
    0x00,   // 30 //
    0xFB,   // 31 //
    0x00,   // 32 //
    0xFB,   // 33 //
    0x00,   // 34 //
    0x00,   // 35 //
};

// area text "M.Storeroom 1F", glyphs followed by code 0xFB
UCHAR Mc_area05[0x24] =
{
    0x29,   // 0 //
    0xFB,   // 1 //
    0x79,   // 2 //
    0xFB,   // 3 //
    0x2F,   // 4 //
    0xFB,   // 5 //
    0x50,   // 6 //
    0xFB,   // 7 //
    0x4B,   // 8 //
    0xFB,   // 9 //
    0x4E,   // 10 //
    0xFB,   // 11 //
    0x41,   // 12 //
    0xFB,   // 13 //
    0x4E,   // 14 //
    0xFB,   // 15 //
    0x4B,   // 16 //
    0xFB,   // 17 //
    0x4B,   // 18 //
    0xFB,   // 19 //
    0x49,   // 20 //
    0xFB,   // 21 //
    0x00,   // 22 //
    0xFB,   // 23 //
    0x0D,   // 24 //
    0x22,   // 25 //
    0xFB,   // 26 //
    0xFB,   // 27 //
    0x00,   // 28 //
    0xFB,   // 29 //
    0x00,   // 30 //
    0xFB,   // 31 //
    0x00,   // 32 //
    0xFB,   // 33 //
    0x00,   // 34 //
    0x00,   // 35 //
};

// area text "Courtyard Path B1", each glyph followed by code 0xFB
UCHAR Mc_area06[0x24] =
{
    0x1F,   // 0 //
    0xFB,   // 1 //
    0x4B,   // 2 //
    0xFB,   // 3 //
    0x51,   // 4 //
    0xFB,   // 5 //
    0x4E,   // 6 //
    0xFB,   // 7 //
    0x50,   // 8 //
    0xFB,   // 9 //
    0x55,   // 10 //
    0xFB,   // 11 //
    0x3D,   // 12 //
    0xFB,   // 13 //
    0x4E,   // 14 //
    0xFB,   // 15 //
    0x40,   // 16 //
    0xFB,   // 17 //
    0x00,   // 18 //
    0xFB,   // 19 //
    0x2C,   // 20 //
    0xFB,   // 21 //
    0x3D,   // 22 //
    0xFB,   // 23 //
    0x50,   // 24 //
    0xFB,   // 25 //
    0x44,   // 26 //
    0xFB,   // 27 //
    0x00,   // 28 //
    0xFB,   // 29 //
    0x1E,   // 30 //
    0xFB,   // 31 //
    0x0D,   // 32 //
    0xFB,   // 33 //
    0x00,   // 34 //
    0x00,   // 35 //
};

// full-width save area names appended to the save file title, by area index
UCHAR Mc_area_name[7][0x15] =
{
    { 0x82, 0x6C, 0x82, 0x81, 0x82, 0x8E, 0x82, 0x93, 0x82, 0x89, 0x82, 0x8F, 0x82, 0x8E, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x00 },   // 0 //
    { 0x82, 0x6C, 0x82, 0x81, 0x82, 0x8E, 0x82, 0x93, 0x82, 0x89, 0x82, 0x8F, 0x82, 0x8E, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x00 },   // 1 //
    { 0x82, 0x62, 0x82, 0x8F, 0x82, 0x95, 0x82, 0x92, 0x82, 0x94, 0x82, 0x99, 0x82, 0x81, 0x82, 0x92, 0x82, 0x84, 0x81, 0x40, 0x00 },   // 2 //
    { 0x82, 0x66, 0x82, 0x95, 0x82, 0x81, 0x82, 0x92, 0x82, 0x84, 0x82, 0x88, 0x82, 0x8F, 0x82, 0x95, 0x82, 0x93, 0x82, 0x85, 0x00 },   // 3 //
    { 0x82, 0x6B, 0x82, 0x81, 0x82, 0x82, 0x82, 0x8F, 0x82, 0x92, 0x82, 0x81, 0x82, 0x94, 0x82, 0x8F, 0x82, 0x92, 0x82, 0x99, 0x00 },   // 4 //
    { 0x82, 0x6C, 0x82, 0x81, 0x82, 0x8E, 0x82, 0x93, 0x82, 0x89, 0x82, 0x8F, 0x82, 0x8E, 0x81, 0x40, 0x81, 0x40, 0x81, 0x40, 0x00 },   // 5 //
    { 0x82, 0x62, 0x82, 0x8F, 0x82, 0x95, 0x82, 0x92, 0x82, 0x94, 0x82, 0x99, 0x82, 0x81, 0x82, 0x92, 0x82, 0x84, 0x81, 0x40, 0x00 },   // 6 //
};

USHORT Mc_err_pos_tbl[8] = { 0x003C, 0x0070, 0x0068, 0x0060, 0x0034, 0x0070, 0x0048, 0x0000 };  // text x position of each error message, by error code

// error message text, by error code
UCHAR* Mc_err_tbl[7] =
{
    Mc_err00,       // 0 //
    Mc_err01,          // 1 //
    Mc_err02,         // 2 //
    Mc_err03,        // 3 //
    Mc_err04,      // 4 //
    Mc_err05,           // 5 //
    Mc_err06,   // 6 //
};

// save area name text, by area index
UCHAR* Mc_area_tbl[7] =
{
    Mc_area00,         // 0 //
    Mc_area01,     // 1 //
    Mc_area02,       // 2 //
    Mc_area03,      // 3 //
    Mc_area04,      // 4 //
    Mc_area05,    // 5 //
    Mc_area06,   // 6 //
};

// save row text "Chris" with a save count placeholder
UCHAR Mc_mess_chris[0xC] =
{
    0x1F,   // 0 //
    0x44,   // 1 //
    0x4E,   // 2 //
    0x45,   // 3 //
    0x4F,   // 4 //
    0x38,   // 5 //
    0x0C,   // 6 //
    0x0C,   // 7 //
    0x38,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
};

// save row text "Jill" with a save count placeholder
UCHAR Mc_mess_jill[0xC] =
{
    0x26,   // 0 //
    0x45,   // 1 //
    0x48,   // 2 //
    0x48,   // 3 //
    0x00,   // 4 //
    0x38,   // 5 //
    0x0C,   // 6 //
    0x0C,   // 7 //
    0x38,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
};

// display time of each step of the save-complete highlight sweep
UCHAR Mc_wait_tbl[0x1C] =
{
    0x0A,   // 0 //
    0x0B,   // 1 //
    0x0C,   // 2 //
    0x0A,   // 3 //
    0x09,   // 4 //
    0x14,   // 5 //
    0x12,   // 6 //
    0x0A,   // 7 //
    0x14,   // 8 //
    0x13,   // 9 //
    0x0C,   // 10 //
    0x08,   // 11 //
    0x0A,   // 12 //
    0x09,   // 13 //
    0x0A,   // 14 //
    0x0B,   // 15 //
    0x09,   // 16 //
    0x0A,   // 17 //
    0x08,   // 18 //
    0x09,   // 19 //
    0x0A,   // 20 //
    0x09,   // 21 //
    0x08,   // 22 //
    0x0A,   // 23 //
    0x08,   // 24 //
    0x0A,   // 25 //
    0x18,   // 26 //
    0x00,   // 27 //
};

CHAR Mc_src_path[0x1C] = { 0x62, 0x75, 0x30, 0x30, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  // card path buffer of the rename source, starts as "bu00:"

CHAR Mc_dst_path[0x1C] = { 0x62, 0x75, 0x30, 0x30, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  // card path buffer of the rename destination, starts as "bu00:"
