/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\FILE.C
 */

#include "TYPES.H"
#include "FILE.H"
#include "GAME.H"
#include "ITEM.H"
#include "MAP.H"
#include "MOJIDISP.H"
#include "SCE0.H"
#include "SCE1.H"
#include "SCE_AT.H"
#include "SND.H"
#include "SCRATCHPAD.H"
#include "FILECODE.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "SCENARIO.H"
#include "JOINT.H"

// functions without a C definition
VOID Mess_set();

PICKUP_WORK File_pickup_work; // state of the file screen (list, reader and pickup display)

INT St_file_main(VOID)
{
    FILE_WORK* p = (FILE_WORK*)&File_pickup_work;  // reinterprets the memory as FILE_WORK
    UCHAR mode;

    mode = p->mode;
    switch (mode)
    {
    case 0:
        if (File_pickup_work.closing == 0 && (G.SaveData.Key_trg & 0x8000))
        {
            if ((UINT)(File_pickup_work.Open_req - 1) >= 2)  // unsigned conversion (signed operand)
            {
                Snd_se_play(SE_BANK_ENEMY, 5, 0);
            }
            File_pickup_work.step = 2;
            File_pickup_work.closing = 1;
        }
        File_book_main(p);

        if (p->busy == 0 && (G.SaveData.Key_trg & 0x4000) && p->step == 1 && p->selected != 0xFF)
        {
            p->Open_req = 1;
            p->busy = 1;
            Snd_se_play(SE_BANK_ENEMY, 6, 0);
        }
        break;
    case 1:
        File_reader_main(p);
        if (File_pickup_work.closing == 0)
        {
            if (KEY_TRG_CUR & 0x8000)
            {
                File_pickup_work.step = 0xD;
                File_pickup_work.closing = 1;
                Snd_se_play(SE_BANK_ENEMY, 5, 0);
            }
            if ((MAIN.SaveData.Key_trg & 0x4000) && File_pickup_work.Exit_sel == mode)
            {
                File_pickup_work.step = 0xD;
                File_pickup_work.closing = 1;
                Snd_se_play(SE_BANK_ENEMY, 5, 0);
            }
        }
        break;
    }
    if (p->Close_req != 0)
    {
        p->Close_req = 0;
        return -1;
    }
    return 0;
}

VOID File_book_main(FILE_WORK* pWk)
{
    UCHAR state;

    state = pWk->state;
    switch (state)
    {
    case 0:
        File_book_init(pWk);
        pWk->state = pWk->state + 1;
        return;
    case 1:
        File_book_pad(pWk);
        return;
    }
}

VOID File_book_init(FILE_WORK* pWk)
{
    INT i;
    SCHAR n;

    pWk->Slide_spd = 0x80;
    pWk->Close_req = 0;
    pWk->List_page = 0;

    pWk->Slide_y = -0x7F;

    pWk->Slide_x = 0;
    if (SYSTEM_FLG_NOW & SYS_PLAYER)
    {
        pWk->Book_no = 1;
    }
    else
    {
        pWk->Book_no = 0;
    }
    Pickup_seen_set(pWk);
    for (i = 0; i < 8; i++)
    {
        if (File_ck(Book_tbl[pWk->Book_no][i]) != 0)
        {
            pWk->selected = i;
            break;
        }

        if (i == 7)
        {
            pWk->selected = 0xFF;
        }
    }

    Pickup_fade_box.b = 0;

    Pickup_fade_box.g = 0;
    Pickup_fade_box.r = 0;

    n = pWk->List_page;
    File_tex_load(Book_tex_tbl[n], n + 0x90, 0);
    File_book_disp(pWk);
}

VOID File_book_pad(FILE_WORK* p)
{
    INT dir;
    INT i;
    INT j;
    SCHAR c;
    INT t;

    if (p->busy == 0)
    {
        if (p->selected != 0xFF)
        {
            if ((G.Padd1 & PAD_UP) || (PADD1_NOW & PAD_DOWN))
            {
                p->Hold_timer++;
            }
            else
            {
                p->Hold_timer = 0;
            }
            t = G.Padd1_trg & PAD_UP;
            dir = t != 0;
            if (G.Padd1_trg & PAD_DOWN)
            {
                dir = 2;
            }
            c = p->Hold_timer;
            if (c > 20 && c % 3 == 0)
            {
                if (G.Padd1 & PAD_UP)
                {
                    dir = 1;
                }
                if (G.Padd1 & PAD_DOWN)
                {
                    dir = 2;
                }
            }
            if (dir == 1)
            {
                for (i = 0; i < 8; i++)
                {
                    j = p->selected - i - 1;
                    if (j < 0)
                    {
                        break;
                    }

                    if (File_ck(Book_tbl[p->Book_no][p->List_page * 8 + j]))
                    {
                        p->selected = j;
                        Snd_se_play(SE_BANK_ENEMY, 4, 0);
                        break;
                    }
                }
            }
            if (dir == 2)
            {
                for (i = 0; i < 8; i++)
                {
                    j = p->selected + i + 1;

                    if (j >= 8)
                    {
                        break;
                    }

                    if (File_ck(Book_tbl[p->Book_no][p->List_page * 8 + j]))
                    {
                        p->selected = j;
                        Snd_se_play(SE_BANK_ENEMY, 4, 0);
                        break;
                    }
                }
            }
        }
        if (G.Padd1_trg & PAD_RIGHT)
        {
            if (p->List_page + 1 >= 2)
            {
                p->List_page = 1;
            }
            else
            {
                p->step = 3;
                Snd_se_play(SE_BANK_ENEMY, 4, 0);
            }
        }
        if (G.Padd1_trg & PAD_LEFT)
        {
            if (p->List_page - 1 < 0)
            {
                p->List_page = 0;
            }
            else
            {
                p->step = 8;
                Snd_se_play(SE_BANK_ENEMY, 4, 0);
            }
        }
    }
    File_book_disp(p);
}

VOID File_book_disp(FILE_WORK* p)
{
    INT i;

    INT j;
    INT k;

    INT yo;
    INT off;
    SCHAR n;

    UCHAR c;

    UCHAR id;

    INT m;

    UCHAR(*tbl)[16];

    switch (p->step)
    {
    case 0:
        p->Slide_spd /= 2;
        p->Slide_y += p->Slide_spd;
        if (p->Slide_y >= 0)
        {
            p->Slide_y = 0;
            p->Slide_spd = 1;
            p->step++;
        }
        break;
    case 1:
        p->busy = 0;
        p->Slide_spd = 1;
        break;
    case 2:
        p->Slide_y -= p->Slide_spd;
        p->Slide_spd *= 2;
        if (p->Slide_y < -0x67)
        {
            VIEW(INT, p->mode) = 0;  // INT* view of the field (a plain access changes the code)

            p->Close_req = 1;
        }
        break;
    case 3:
        p->busy = 1;
        p->Slide_spd = 1;
        p->step++;

    case 4:
        p->Slide_x -= p->Slide_spd;
        p->Slide_spd *= 2;
        if (p->Slide_x < -0x77)
        {
            p->Slide_x = 0x7F;
            p->Slide_spd = 0x80;
            p->step++;
        }
        break;
    case 5:

        p->List_page++;

        for (i = 0; i < 8; i++)
        {
            if (File_ck(Book_tbl[p->Book_no][p->List_page * 8 + i]) != 0)
            {
                p->selected = i;
                break;
            }

            if (i == 7)
            {
                p->selected = 0xFF;
            }
        }
        p->step++;
        break;
    case 6:
        n = p->List_page;
        File_tex_load(Book_tex_tbl[n], n + 0x90, 0);
        p->step++;

    case 7:
        p->Slide_spd /= 2;
        p->Slide_x -= p->Slide_spd;
        if (p->Slide_x <= 0)
        {
            p->Slide_x = 0;
            p->step = 1;
        }
        break;
    case 8:
        p->busy = 1;
        p->Slide_spd = 1;
        p->step++;

    case 9:
        p->Slide_x += p->Slide_spd;
        p->Slide_spd *= 2;
        if (p->Slide_x >= 0x78)
        {
            p->Slide_x = -0x7F;
            p->Slide_spd = 0x80;
            p->step++;
        }
        break;
    case 10:

        p->List_page--;

        for (i = 0; i < 8; i++)
        {
            if (File_ck(Book_tbl[p->Book_no][p->List_page * 8 + i]) != 0)
            {
                p->selected = i;
                break;
            }

            if (i == 7)
            {
                p->selected = 0xFF;
            }
        }
        p->step++;
        break;
    case 11:
        n = p->List_page;
        File_tex_load(Book_tex_tbl[n], n + 0x90, 0);
        p->step++;

    case 12:
        p->Slide_spd /= 2;
        p->Slide_x += p->Slide_spd;
        if (p->Slide_x >= 0)
        {
            p->Slide_x = 0;
            p->step = 1;
        }
        break;
    }

    c = p->Open_req;
    switch (c)
    {
    case 0:
        break;
    case 1:
        if (Pickup_fade_main(0) != 0)
        {
            VIEW(INT, p->mode) = c;  // INT* view of the field (a plain access changes the code)
        }
        p->busy = 1;
        break;
    case 2:
        if (Pickup_fade_main(1) != 0)
        {
            p->Slide_x = 0;
            p->Slide_y = 0;
            p->Open_req = 0;
        }
        p->busy = 1;
        break;
    }

    if (p->step == 1)
    {
        p->Blink_timer++;
        if (!(p->Blink_timer & 0x30))
        {
            p->Arrow_blink = 1;
        }
        else
        {
            p->Arrow_blink = 0;
        }
        (&Book_cursor_spr.Arrow_l.u)[0] = p->Arrow_blink * 8 - 0x50;
        Book_cursor_spr.Arrow_r.u = p->Arrow_blink * 8 - 0x40;

        k = p->List_page;
        if (k != 0)
        {
            GsSortFastSprite((GsSPRITE*)((&Book_cursor_spr.Arrow_l.u) - 0xE), MAIN.Ot1 + G.Cbuf_id, 0xF);  // byte view of the same memory
        }

        if (p->List_page != 1)
        {
            GsSortFastSprite((GsSPRITE*)((&Book_cursor_spr.Arrow_l.u) + 0x16), MAIN.Ot1 + G.Cbuf_id, 0xF);  // byte view of the same memory
        }
    }
    Book_list_spr.x = p->Slide_x + 0x20;
    Book_list_spr.y = p->Slide_y + 0x18;
    GsSortFastSprite(BYTE_OFS(GsSPRITE, &Book_list_spr, -4), MAIN.Ot1 + G.Cbuf_id, 0x10);  // byte view of the same memory
    if (p->selected == 0xFF)
    {
        return;
    }
    i = 0;
    tbl = Book_tbl;
    for (off = 0, yo = 0x29; i < 8; off += sizeof(GsSPRITE), i++, yo += 9)
    {
        if (File_ck(tbl[p->Book_no][p->List_page * 8 + i]) != 0)
        {
            {
                INT ofs_x = i / 2 + 0x8C;
                ((GsSPRITE*)(Book_entry_spr + off))->x = p->Slide_x + ofs_x;  // byte view of the same memory
            }
            ((GsSPRITE*)(Book_entry_spr + off))->y = p->Slide_y + yo;  // byte view of the same memory
            GsSortFastSprite((GsSPRITE*)(Book_entry_spr + off), MAIN.Ot1 + CBUF_ID_CUR, 0xF);  // reinterprets the memory as GsSPRITE
        }
    }
    Book_cursor_spr.x = p->Slide_x + ((p->selected >> 1) + 0x8C);
    {
        INT ofs_y = p->selected * 9 + 0x29;
        Book_cursor_spr.y = p->Slide_y + ofs_y;
    }
    GsSortFastSprite(BYTE_OFS(GsSPRITE, &Book_cursor_spr, -4), MAIN.Ot1 + G.Cbuf_id, 0xE);  // byte view of the same memory

    if (p->step != 1)
    {
        return;
    }

    id = Book_tbl[p->Book_no][p->List_page * 8 + p->selected];

    Name_disp(id + 0x5F, 0x40, File_name_x[id] - Scr_ofs[0] + 0x1E, 0xC1 - Scr_ofs[1]);

    do
    {
        i = 0;
        (((SPR_REC*)&Book_up_spr)->u) = p->Arrow_blink * 8 - 0x30;  // reinterprets the memory as SPR_REC

        m = 0x67;
    } while (0);
    m -= (USHORT)Scr_ofs[0];  // truncates to an unsigned halfword

    Book_up_spr.y = 0xB7 - Scr_ofs[1];

    Book_up_spr.x = m;

    Book_down_spr.u = p->Arrow_blink * 8 - 0x20;

    Book_down_spr.x = m;

    Book_down_spr.y = 0xCF - Scr_ofs[1];

    for (; i < 8; i++)
    {
        j = p->selected - i - 1;
        if (j < 0)
        {
            break;
        }
        if (File_ck(Book_tbl[p->Book_no][p->List_page * 8 + j]) != 0)
        {
            GsSortFastSprite((GsSPRITE*)Book_up_attr, MAIN.Ot0 + G.Cbuf_id, 3);
            break;
        }
    }
    for (i = 0; i < 8; i++)
    {
        j = p->selected + i + 1;
        if (j >= 8)
        {
            break;
        }
        if (File_ck(Book_tbl[p->Book_no][p->List_page * 8 + j]) != 0)
        {
            GsSortFastSprite((GsSPRITE*)Book_down_attr, MAIN.Ot0 + G.Cbuf_id, 3);
            break;
        }
    }
}

VOID File_reader_main(FILE_WORK* pWk)
{
    UCHAR state;

    Pickup_fade_main(2);
    state = pWk->state;
    switch (state)
    {
    case 0:
        File_reader_init(pWk);
        pWk->state = pWk->state + 1;
        return;
    case 1:
        File_reader_pad(pWk);
        return;
    }
}

VOID File_reader_init(FILE_WORK* pWk)
{
    pWk->file_page = 0;
    pWk->Exit_sel = 0;
    File_reader_disp(pWk);
}

VOID File_reader_pad(FILE_WORK* pWk)
{
    UINT max;

    max = File_page_num[Book_tbl[pWk->Book_no][pWk->List_page * 8 + pWk->selected]];

    if (pWk->busy == 0)
    {
        if ((G.Padd1 & PAD_RIGHT) || (PADD1_NOW & PAD_LEFT))
        {
            pWk->Hold_timer++;
        }
        else
        {
            pWk->Hold_timer = 0;
        }
        if ((G.Padd1_trg & PAD_RIGHT) || ((G.Padd1 & PAD_RIGHT) && pWk->Hold_timer >= 21))
        {
            if (pWk->file_page + 1 >= max)
            {
                if (pWk->Exit_sel == 0)
                {
                    pWk->Exit_sel = 1;
                    Snd_se_play(SE_BANK_ENEMY, 4, 0);
                }
            }
            else
            {
                pWk->busy = 1;
                pWk->step = 5;
                Snd_se_play(SE_BANK_ENEMY, 8, 0);
            }
        }

        if ((G.Padd1_trg & PAD_LEFT) || ((G.Padd1 & PAD_LEFT) && pWk->Hold_timer >= 21))
        {
            if (pWk->Exit_sel == 1)
            {
                pWk->Exit_sel = 0;
                Snd_se_play(SE_BANK_ENEMY, 4, 0);
            }
            else if (pWk->file_page - 1 < 0)
            {
                pWk->file_page = 0;
            }
            else
            {
                pWk->busy = 1;
                pWk->step = 8;
                Snd_se_play(SE_BANK_ENEMY, 8, 0);
            }
        }
    }
    File_reader_disp(pWk);
}

VOID File_reader_disp(FILE_WORK* pWk)
{
    INT i;

    switch (pWk->step)
    {
    case 0:

        File_tex_load(Reader_frame_no, i + 0x96, 1);

        pWk->Slide_x = 0x128;
        pWk->Slide_y = 0;
        pWk->busy = 1;
        pWk->step++;

        goto inc;

    case 2:
        if (!(pWk->file_page % 2))
        {
            i = File_page_top[Book_tbl[pWk->Book_no][pWk->List_page * 8 + pWk->selected]] + pWk->file_page / 2;

            file_moji_load(File_page_idx[i], i + 0xB4, 2);
        }
        pWk->Slide_x = 0x128;
        pWk->Slide_spd = 0x12C;
        pWk->step++;
    case 3:
        pWk->Slide_spd /= 2;
        pWk->Slide_x -= pWk->Slide_spd;
        if (pWk->Slide_x <= 0)
        {
            pWk->Slide_x = 0;
            pWk->Slide_spd = 1;
            pWk->step++;
        }
        break;
    case 4:
        pWk->busy = 0;
        break;
    case 5:
        pWk->Slide_x = 0;
        pWk->Slide_spd = 1;
        pWk->step++;
    case 6:
        pWk->Slide_x -= pWk->Slide_spd;
        pWk->Slide_spd *= 2;
        if (pWk->Slide_x < -0x127)
        {
            pWk->step++;
            pWk->file_page++;
        }
        break;
    case 7:
        pWk->step = 2;
        break;
    case 8:
        pWk->Slide_x = 0;
        pWk->Slide_spd = 1;
        pWk->step++;
    case 9:
        pWk->Slide_x += pWk->Slide_spd;
        pWk->Slide_spd *= 2;
        if (pWk->Slide_x >= 0x128)
        {
            pWk->step++;
            pWk->file_page--;
        }
        break;
    case 11:
        if (pWk->file_page % 2)
        {
            i = File_page_top[Book_tbl[pWk->Book_no][pWk->List_page * 8 + pWk->selected]] + pWk->file_page / 2;

            file_moji_load(File_page_idx[i], i + 0xB4, 2);
        }
        pWk->Slide_x = -0x128;
        pWk->Slide_spd = 0x12C;
        pWk->step++;
    case 12:
        pWk->Slide_spd /= 2;
        pWk->Slide_x += pWk->Slide_spd;
        if (pWk->Slide_x >= 0)
        {
            pWk->Slide_x = 0;
            pWk->step = 4;
        }
        break;
    case 13:
        pWk->Slide_x = 0;
        pWk->Slide_spd = 1;
        pWk->step++;
    case 14:
        pWk->Slide_x -= pWk->Slide_spd;
        pWk->Slide_spd *= 2;
        if (pWk->Slide_x < -0x127)
        {
            pWk->step++;
        }
        break;

    case 17:
        File_tex_load(Book_tex_tbl[pWk->List_page], pWk->List_page + 0x90, 0);

    case 1:

    case 10:

    case 15:

    case 16:
    inc:
        pWk->step++;
        break;
    case 18:

        *(INT*)pWk = 0x02010100;  // INT* view of the same memory

        break;
    }
    Reader_page_spr.x = pWk->Slide_x + 0x18;

    Reader_page_spr.y = pWk->Slide_y + 0x18;

    Reader_page_spr.tpage = pWk->file_page % 2 + 0x15;

    Reader_page_spr.cy = pWk->file_page % 2 + 0x1FD;

    GsSortFastSprite(BYTE_OFS(GsSPRITE, &Reader_page_spr, -4), MAIN.Ot0 + G.Cbuf_id, 2);  // byte view of the same memory

    if (pWk->step == 4)
    {
        if (!(++pWk->Blink_timer & 0x30))
        {
            pWk->Arrow_blink = 1;
        }
        else
        {
            pWk->Arrow_blink = 0;
        }
        if (pWk->Exit_sel == 1)
        {
            pWk->Arrow_blink = 1;
            pWk->Blink_timer = 0;
        }

        (&Reader_arrow[0].u)[0] = pWk->Arrow_blink * 8;
        (&Reader_arrow[0].u)[0x24] = pWk->Arrow_blink * 8 + 0x10;

        if (pWk->file_page != 0)
        {
            GsSortFastSprite((GsSPRITE*)&(&Reader_arrow[0].u)[-0xE], MAIN.Ot0 + G.Cbuf_id, 2);  // the arrow sprite is read as a GsSPRITE starting 0xE bytes before its u field
        }
        GsSortFastSprite((GsSPRITE*)&(&Reader_arrow[0].u)[0x16], MAIN.Ot0 + G.Cbuf_id, 2);  // the arrow sprite is read as a GsSPRITE starting 0x16 bytes after its u field

        if (pWk->file_page == File_page_num[Book_tbl[pWk->Book_no][pWk->List_page * 8 + pWk->selected]] - 1)
        {
            if (pWk->Exit_sel == 1)
            {
                Reader_exit_spr.r = Reader_exit_spr.g = Reader_exit_spr.b = 0x50;
            }
            else
            {
                Reader_exit_spr.r = Reader_exit_spr.g = Reader_exit_spr.b = 0x20;
            }
            GsSortFastSprite(&Reader_exit_spr, MAIN.Ot0 + G.Cbuf_id, 2);
        }
    }
}

VOID File_tex_load(INT file_id, INT Res_id, INT Tex_slot)
{
    RECT Rect;
    UCHAR* pRes_id = &St.Res_id;

    if (*pRes_id != Res_id)
    {
        *pRes_id = Res_id;

        DrawSync(0);

        Cd_read_wait(file_id, MENU_TEX_BUF);

        if (Tex_slot == 2)
        {
            Rect.x = 0;
            Rect.w = 0x10;
            Rect.h = 1;
            Rect.y = File_clut_row + 0x1E0;
            LoadImage(&Rect, (ULONG*)MENU_TEX_BUF);  // word view of the same memory
            DrawSync(0);

            Rect.x = 0;
            Rect.w = 0x10;
            Rect.h = 1;
            Rect.y = File_clut_row + 0x1E1;
            LoadImage(&Rect, (ULONG*)FILE_CLUT_DATA_BUFFER);  // word view of the same memory

            DrawSync(0);

            LoadImage(&File_clut_rect, (ULONG*)FILE_SCREEN_CLUT_DATA);  // word view of the same memory

            DrawSync(0);

            return;
        }
        Tim_trans(MENU_TEX_BUF, &File_tex_tbl[Tex_slot].rect, File_tex_tbl[Tex_slot].Clut_row);
    }
}

VOID file_moji_load(INT page, INT Res_id, INT Tex_slot)
{
    UINT sector;
    UCHAR* pRes_id;
    SHORT Clut_rect[4];
    USHORT Bak_sec_lo;
    UCHAR Bak_sec_hi;

    pRes_id = &St.Res_id;
    if (*pRes_id != Res_id)
    {
        USHORT* pLba = &floc[FILEM_PIX].sector;
        *pRes_id = Res_id;
        DrawSync(0);
        Bak_sec_hi = floc[FILEM_PIX].sec_high;
        Bak_sec_lo = *pLba;
        sector = Bak_sec_lo + (Bak_sec_hi << 0x10) + File_page_tbl[page].Sector_ofs;
        *pLba = sector;
        floc[FILEM_PIX].sec_high = sector >> 0x10;
        floc[FILEM_PIX].size = File_page_tbl[page].size;
        floc[FILEM_PIX].sum = File_page_sum[page];
        Cd_read_wait(FILEM_PIX, MENU_TEX_BUF);
        *pLba = Bak_sec_lo;
        floc[FILEM_PIX].sec_high = Bak_sec_hi;
        if (Tex_slot == 2)
        {
            Clut_rect[0] = 0;
            Clut_rect[2] = 0x10;
            Clut_rect[3] = 1;
            Clut_rect[1] = File_clut_row + 0x1E0;
            LoadImage((RECT*)Clut_rect, (ULONG*)MENU_TEX_BUF);  // reinterprets the memory as RECT
            DrawSync(0);

            Clut_rect[0] = 0;
            Clut_rect[2] = 0x10;
            Clut_rect[3] = 1;
            Clut_rect[1] = File_clut_row + 0x1E1;
            LoadImage((RECT*)Clut_rect, (ULONG*)FILE_CLUT_DATA_BUFFER);  // reinterprets the memory as RECT

            DrawSync(0);
            LoadImage(&File_clut_rect, (ULONG*)FILE_SCREEN_CLUT_DATA);  // word view of the same memory

            DrawSync(0);
            return;
        }
        Tim_trans(MENU_TEX_BUF, &File_tex_tbl[Tex_slot].rect, File_tex_tbl[Tex_slot].Clut_row);
    }
}

INT Pickup_fade_main(UINT mode)
{
    INT pRet;
    INT x;

    pRet = 0;
    switch (mode)
    {
    case 0:
        x = Pickup_fade_box.r + 0x20;

        Pickup_fade_box.r = Pickup_fade_box.g = Pickup_fade_box.b = x;

        if (x >= 0xFF)
        {
            pRet = -1;

            Pickup_fade_box.r = Pickup_fade_box.g = Pickup_fade_box.b = 0xFF;
        }
        break;
    case 1:
        x = Pickup_fade_box.r - 0x20;

        Pickup_fade_box.r = Pickup_fade_box.g = Pickup_fade_box.b = x;

        if (x <= 0)
        {
            pRet = -1;

            Pickup_fade_box.r = Pickup_fade_box.g = Pickup_fade_box.b = 0;
        }
        break;
    case 2:

        Pickup_fade_box.r = Pickup_fade_box.g = Pickup_fade_box.b = 0xFF;

        break;
    case 3:

        Pickup_fade_box.r = Pickup_fade_box.g = Pickup_fade_box.b = 0;

        break;
    }
    GsSortBoxFill(&Pickup_fade_box, &MAIN.Ot0[G.Cbuf_id], 3);
    return pRet;
}

VOID File_get_init(VOID)
{
    Pickup_tbl_init();
    St_reset();

    File_pickup_work.closing = 1;
}

INT File_pickup_main(VOID)
{
    FILE_WORK* p;

    UCHAR* q;

    INT k;
    UINT i;

    p = (FILE_WORK*)&File_pickup_work;  // reinterprets the memory as FILE_WORK
    switch (p->mode)
    {
    case 0:
        if (File_pickup_work.state == 0 && Pickup_fade_main(0) != 0)
        {
            p->mode = 1;

            if (G.System_flg & SYS_PLAYER)
            {
                File_pickup_work.Book_no = 1;
            }
            else
            {
                File_pickup_work.Book_no = 0;
            }
            k = pAot_data->pRec->Item_id - 0x5F;

            St_file_set(k);
            Pickup_seen_set(p);
            Snd_se_play(SE_BANK_ENEMY, 6, 0);

            q = Book_tbl[p->Book_no];

            for (i = 0; i < 16; i++)
            {
                if (*q++ == k)
                {
                    p->List_page = i >> 3;
                    p->selected = i & 7;

                    p->file_page = 0;
                    p->busy = 1;
                    p->Exit_sel = 0;
                }
            }
        }
        if (p->state == 1)
        {
            if (Pickup_fade_main(1) != 0)
            {
                p->Close_req = 1;
            }
            if (p->Open_req == 2)
            {
                ScratchPad->vram_rect.x = 0x180;
                ScratchPad->vram_rect.y = 0x100;
                ScratchPad->vram_rect.w = 0x40;
                ScratchPad->vram_rect.h = 0x100;
                LoadImage(&ScratchPad->vram_rect, (ULONG*)IMG_TRANS_BUF);  // word view of the same memory
                PL_ROUTINE_WORD = 1;
                G.Player.Hokan_flg = 0;
                G.Player.Move_no = 0;
                G.Player.Move_cnt = 0;
                G.Player.Move_wait = 0;
                Joint_move(0, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x400);
                p->Open_req = 0;
                G.SaveData.Sel_id = pAot_data->pRec->Item_id;
                Mess_set(0xC6);
            }
        }
        break;
    case 1:
        File_reader_main(p);
        if (File_pickup_work.closing == 0)
        {
            if (KEY_TRG_CUR & 0x8000)
            {
                File_pickup_work.step = 13;
                File_pickup_work.closing = 1;
                Snd_se_play(SE_BANK_ENEMY, 5, 0);
            }
            if ((MAIN.SaveData.Key_trg & 0x4000) && File_pickup_work.Exit_sel == 1)
            {
                File_pickup_work.step = 13;
                File_pickup_work.closing = 1;
                Snd_se_play(SE_BANK_ENEMY, 5, 0);
            }
        }
        break;
    }
    if (p->Close_req != 1)
    {
        return 0;
    }
    if (G.SaveData.Mess_flg & MESS_FLG_ACTIVE)
    {
        return 0;
    }
    *(UCHAR*)pOm_tbl[pAot_data->pRec->Model_no] = 0;  // UCHAR* view of the same memory
    pAot_data->Id = 0;
    Flg_off(&MAIN.SaveData.Item_flg[0], pAot_data->pRec->Item_flg_no);
    G.SaveData.Get_id = pAot_data->pRec->Item_id;
    p->Close_req = 0;
    return -1;
}

VOID St_reset(VOID)
{
    Pickup_tbl_init();
    VIEW(INT, File_pickup_work) = 0;  // INT* view of the field (a plain access changes the code)

    File_pickup_work.List_page = 0;
    File_pickup_work.selected = 0;
    File_pickup_work.file_page = 0;
    File_pickup_work.Book_no = 0;
    File_pickup_work.Close_req = 0;
    File_pickup_work.closing = 0;
    File_pickup_work.Slide_spd = 0;
    File_pickup_work.Slide_x = 0;
    File_pickup_work.Slide_y = 0;
    File_pickup_work.Exit_sel = 0;
    File_pickup_work.Arrow_blink = 0;
    File_pickup_work.Blink_timer = 0;
    File_pickup_work.Hold_timer = 0;
}

VOID Pickup_seen_set(FILE_WORK* pWk)
{
    SCHAR* pBook_no;
    UCHAR* table;
    INT i;
    UCHAR* pBook_tbl = Book_tbl[0];
    i = 0;
    table = &*pBook_tbl;
loop_1:
    if (*(pWk->Book_no * 0x10 + table + i) == 0xFE)
    {
        if (File_ck(0) != 0)
        {
            pBook_no = &pWk->Book_no;
            *(*pBook_no * 0x10 + table + i) = 0;
            return;
        }

        if (File_ck(1) != 0)
        {
            *(pWk->Book_no * 0x10 + table + i) = 1;
            return;
        }
        goto next_slot;
    }

next_slot:
    i += 1;

    if (i >= 0x10)
    {
        return;
    }
    goto loop_1;
}

VOID Pickup_tbl_init(VOID)
{
    Book_tbl[0][0] = 0xF;
    Book_tbl[0][1] = 2;
    Book_tbl[0][2] = 0xFE;
    Book_tbl[0][3] = 3;
    Book_tbl[0][4] = 5;
    Book_tbl[0][5] = 6;
    Book_tbl[0][6] = 7;
    Book_tbl[0][7] = 8;
    Book_tbl[0][8] = 9;
    Book_tbl[0][9] = 0xA;
    Book_tbl[0][10] = 0xC;
    Book_tbl[0][11] = 0xD;
    Book_tbl[0][12] = 0xE;
    Book_tbl[0][13] = 0xFF;
    Book_tbl[0][14] = 0xFF;
    Book_tbl[0][15] = 0xFF;
    Book_tbl[1][0] = 0xF;
    Book_tbl[1][1] = 2;
    Book_tbl[1][2] = 0xFE;
    Book_tbl[1][3] = 3;
    Book_tbl[1][4] = 4;
    Book_tbl[1][5] = 5;
    Book_tbl[1][6] = 6;
    Book_tbl[1][7] = 7;
    Book_tbl[1][8] = 8;
    Book_tbl[1][9] = 9;
    Book_tbl[1][10] = 0xA;
    Book_tbl[1][11] = 0xB;
    Book_tbl[1][12] = 0xC;
    Book_tbl[1][13] = 0xD;
    Book_tbl[1][14] = 0xE;
    Book_tbl[1][15] = 0xFF;
}

// page images: sector offset in the page archive file and byte size, by page entry
FILE_PAGE_REC File_page_tbl[46] =
{
    { 0, 49216 },      // 0 //
    { 25, 49216 },     // 1 //
    { 50, 49216 },     // 2 //
    { 75, 49216 },     // 3 //
    { 100, 49216 },    // 4 //
    { 125, 49216 },    // 5 //
    { 150, 49216 },    // 6 //
    { 175, 49216 },    // 7 //
    { 200, 49216 },    // 8 //
    { 225, 49216 },    // 9 //
    { 250, 49216 },    // 10 //
    { 275, 49216 },    // 11 //
    { 300, 49216 },    // 12 //
    { 325, 49216 },    // 13 //
    { 350, 49216 },    // 14 //
    { 375, 49216 },    // 15 //
    { 400, 49216 },    // 16 //
    { 425, 49216 },    // 17 //
    { 450, 49216 },    // 18 //
    { 475, 49216 },    // 19 //
    { 500, 49216 },    // 20 //
    { 525, 49216 },    // 21 //
    { 550, 49216 },    // 22 //
    { 575, 49216 },    // 23 //
    { 600, 49216 },    // 24 //
    { 625, 49216 },    // 25 //
    { 650, 49216 },    // 26 //
    { 675, 49216 },    // 27 //
    { 700, 49216 },    // 28 //
    { 725, 49216 },    // 29 //
    { 750, 49216 },    // 30 //
    { 775, 49216 },    // 31 //
    { 800, 49216 },    // 32 //
    { 825, 49216 },    // 33 //
    { 850, 49216 },    // 34 //
    { 875, 49216 },    // 35 //
    { 900, 49216 },    // 36 //
    { 925, 49216 },    // 37 //
    { 950, 49216 },    // 38 //
    { 975, 49216 },    // 39 //
    { 1000, 49216 },   // 40 //
    { 1025, 49216 },   // 41 //
    { 1050, 49216 },   // 42 //
    { 1075, 49216 },   // 43 //
    { 1100, 49216 },   // 44 //
    { 1125, 49216 },   // 45 //
};

// checksum of each page image, by page entry
UCHAR File_page_sum[48] =
{
    0x9F,   // 0 //
    0xBC,   // 1 //
    0xEE,   // 2 //
    0x8B,   // 3 //
    0xBC,   // 4 //
    0xCC,   // 5 //
    0xF8,   // 6 //
    0xCB,   // 7 //
    0xAF,   // 8 //
    0x9B,   // 9 //
    0xDB,   // 10 //
    0xBB,   // 11 //
    0xFF,   // 12 //
    0xE7,   // 13 //
    0x99,   // 14 //
    0xEF,   // 15 //
    0xBE,   // 16 //
    0xEC,   // 17 //
    0xEE,   // 18 //
    0xCE,   // 19 //
    0xDB,   // 20 //
    0xBE,   // 21 //
    0x98,   // 22 //
    0xD8,   // 23 //
    0xDB,   // 24 //
    0xCB,   // 25 //
    0xFF,   // 26 //
    0xCA,   // 27 //
    0x9C,   // 28 //
    0xEF,   // 29 //
    0xB9,   // 30 //
    0x89,   // 31 //
    0xDB,   // 32 //
    0xAB,   // 33 //
    0xAE,   // 34 //
    0xDB,   // 35 //
    0xD1,   // 36 //
    0xBE,   // 37 //
    0x48,   // 38 //
    0xDB,   // 39 //
    0xD7,   // 40 //
    0xDB,   // 41 //
    0x8C,   // 42 //
    0xDB,   // 43 //
    0x9B,   // 44 //
    0xAE,   // 45 //
    0x00,   // 46 //
    0x00,   // 47 //
};

USHORT Book_tex_tbl[2] = { FILE000_TIM, FILE001_TIM };  // file numbers of the list textures, by list page

// page archive entry of each page image (one image holds two reader pages), by image index
USHORT File_page_idx[46] =
{
    0x0000,   // 0 //
    0x0001,   // 1 //
    0x0002,   // 2 //
    0x0003,   // 3 //
    0x0004,   // 4 //
    0x0005,   // 5 //
    0x0006,   // 6 //
    0x0007,   // 7 //
    0x0008,   // 8 //
    0x0009,   // 9 //
    0x000A,   // 10 //
    0x000B,   // 11 //
    0x000C,   // 12 //
    0x000D,   // 13 //
    0x000E,   // 14 //
    0x000F,   // 15 //
    0x0010,   // 16 //
    0x0011,   // 17 //
    0x0012,   // 18 //
    0x0013,   // 19 //
    0x0014,   // 20 //
    0x0015,   // 21 //
    0x0016,   // 22 //
    0x0017,   // 23 //
    0x0018,   // 24 //
    0x0019,   // 25 //
    0x001A,   // 26 //
    0x001B,   // 27 //
    0x001C,   // 28 //
    0x001D,   // 29 //
    0x001E,   // 30 //
    0x001F,   // 31 //
    0x0020,   // 32 //
    0x0021,   // 33 //
    0x0022,   // 34 //
    0x0023,   // 35 //
    0x0024,   // 36 //
    0x0025,   // 37 //
    0x0026,   // 38 //
    0x0027,   // 39 //
    0x0028,   // 40 //
    0x0029,   // 41 //
    0x002A,   // 42 //
    0x002B,   // 43 //
    0x002C,   // 44 //
    0x002D,   // 45 //
};

USHORT Reader_frame_no = FILEMARR_PIX;  // file number of the reader frame texture

USHORT Reader_pad = 0x0000;  // padding: keeps the following data at its linked position

// page count of each file, by file id
UCHAR File_page_num[16] =
{
    0x04,   // 0 //
    0x06,   // 1 //
    0x0B,   // 2 //
    0x04,   // 3 //
    0x02,   // 4 //
    0x06,   // 5 //
    0x07,   // 6 //
    0x07,   // 7 //
    0x0A,   // 8 //
    0x05,   // 9 //
    0x05,   // 10 //
    0x04,   // 11 //
    0x03,   // 12 //
    0x03,   // 13 //
    0x03,   // 14 //
    0x04,   // 15 //
};

// x position of each file's name in the list, by file id
UCHAR File_name_x[16] =
{
    0x08,   // 0 //
    0x08,   // 1 //
    0x18,   // 2 //
    0x34,   // 3 //
    0x20,   // 4 //
    0x14,   // 5 //
    0x40,   // 6 //
    0x28,   // 7 //
    0x10,   // 8 //
    0x00,   // 9 //
    0x10,   // 10 //
    0x10,   // 11 //
    0x20,   // 12 //
    0x20,   // 13 //
    0x20,   // 14 //
    0x20,   // 15 //
};

FILE_TEX_REC File_tex_tbl[2] = { { { 320, 256, 128, 104 }, 28 }, { { 320, 448, 16, 8 }, 28 } };  // VRAM rectangle and CLUT row of texture slot 0 (list) and slot 1 (reader frame)

RECT File_clut_rect = { 320, 256, 128, 192 };  // VRAM rectangle that receives the page image of texture slot 2

USHORT File_clut_row = 0x001D;  // CLUT row used for the page image palettes

USHORT File_pad = 0x0000;  // padding: keeps the following data at its linked position

UINT Book_list_attr = 0x01000000;  // attribute word of the list background sprite, read as the start of its record

SPR_REC0 Book_list_spr = { 32, 24, 160, 104, 21, 0, 0, 0, 508, { 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };  // list background sprite (its attribute word is the preceding Book_list_attr)

// nine entry marker sprite records of the list (the first eight are drawn), then the attribute word of the cursor sprite
UCHAR Book_entry_spr[0x148] =
{
    0x00,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x01,   // 3 //
    0x8C,   // 4 //
    0x00,   // 5 //
    0x29,   // 6 //
    0x00,   // 7 //
    0x04,   // 8 //
    0x00,   // 9 //
    0x08,   // 10 //
    0x00,   // 11 //
    0x15,   // 12 //
    0x00,   // 13 //
    0xA8,   // 14 //
    0x00,   // 15 //
    0x00,   // 16 //
    0x00,   // 17 //
    0xFC,   // 18 //
    0x01,   // 19 //
    0x80,   // 20 //
    0x80,   // 21 //
    0x80,   // 22 //
    0x00,   // 23 //
    0x00,   // 24 //
    0x00,   // 25 //
    0x00,   // 26 //
    0x00,   // 27 //
    0x00,   // 28 //
    0x00,   // 29 //
    0x00,   // 30 //
    0x00,   // 31 //
    0x00,   // 32 //
    0x00,   // 33 //
    0x00,   // 34 //
    0x00,   // 35 //
    0x00,   // 36 //
    0x00,   // 37 //
    0x00,   // 38 //
    0x01,   // 39 //
    0x8C,   // 40 //
    0x00,   // 41 //
    0x32,   // 42 //
    0x00,   // 43 //
    0x04,   // 44 //
    0x00,   // 45 //
    0x08,   // 46 //
    0x00,   // 47 //
    0x15,   // 48 //
    0x00,   // 49 //
    0xA8,   // 50 //
    0x00,   // 51 //
    0x00,   // 52 //
    0x00,   // 53 //
    0xFC,   // 54 //
    0x01,   // 55 //
    0x80,   // 56 //
    0x80,   // 57 //
    0x80,   // 58 //
    0x00,   // 59 //
    0x00,   // 60 //
    0x00,   // 61 //
    0x00,   // 62 //
    0x00,   // 63 //
    0x00,   // 64 //
    0x00,   // 65 //
    0x00,   // 66 //
    0x00,   // 67 //
    0x00,   // 68 //
    0x00,   // 69 //
    0x00,   // 70 //
    0x00,   // 71 //
    0x00,   // 72 //
    0x00,   // 73 //
    0x00,   // 74 //
    0x01,   // 75 //
    0x8D,   // 76 //
    0x00,   // 77 //
    0x3B,   // 78 //
    0x00,   // 79 //
    0x04,   // 80 //
    0x00,   // 81 //
    0x08,   // 82 //
    0x00,   // 83 //
    0x15,   // 84 //
    0x00,   // 85 //
    0xA8,   // 86 //
    0x00,   // 87 //
    0x00,   // 88 //
    0x00,   // 89 //
    0xFC,   // 90 //
    0x01,   // 91 //
    0x80,   // 92 //
    0x80,   // 93 //
    0x80,   // 94 //
    0x00,   // 95 //
    0x00,   // 96 //
    0x00,   // 97 //
    0x00,   // 98 //
    0x00,   // 99 //
    0x00,   // 100 //
    0x00,   // 101 //
    0x00,   // 102 //
    0x00,   // 103 //
    0x00,   // 104 //
    0x00,   // 105 //
    0x00,   // 106 //
    0x00,   // 107 //
    0x00,   // 108 //
    0x00,   // 109 //
    0x00,   // 110 //
    0x01,   // 111 //
    0x8D,   // 112 //
    0x00,   // 113 //
    0x44,   // 114 //
    0x00,   // 115 //
    0x04,   // 116 //
    0x00,   // 117 //
    0x08,   // 118 //
    0x00,   // 119 //
    0x15,   // 120 //
    0x00,   // 121 //
    0xA8,   // 122 //
    0x00,   // 123 //
    0x00,   // 124 //
    0x00,   // 125 //
    0xFC,   // 126 //
    0x01,   // 127 //
    0x80,   // 128 //
    0x80,   // 129 //
    0x80,   // 130 //
    0x00,   // 131 //
    0x00,   // 132 //
    0x00,   // 133 //
    0x00,   // 134 //
    0x00,   // 135 //
    0x00,   // 136 //
    0x00,   // 137 //
    0x00,   // 138 //
    0x00,   // 139 //
    0x00,   // 140 //
    0x00,   // 141 //
    0x00,   // 142 //
    0x00,   // 143 //
    0x00,   // 144 //
    0x00,   // 145 //
    0x00,   // 146 //
    0x01,   // 147 //
    0x8E,   // 148 //
    0x00,   // 149 //
    0x4D,   // 150 //
    0x00,   // 151 //
    0x04,   // 152 //
    0x00,   // 153 //
    0x08,   // 154 //
    0x00,   // 155 //
    0x15,   // 156 //
    0x00,   // 157 //
    0xA8,   // 158 //
    0x00,   // 159 //
    0x00,   // 160 //
    0x00,   // 161 //
    0xFC,   // 162 //
    0x01,   // 163 //
    0x80,   // 164 //
    0x80,   // 165 //
    0x80,   // 166 //
    0x00,   // 167 //
    0x00,   // 168 //
    0x00,   // 169 //
    0x00,   // 170 //
    0x00,   // 171 //
    0x00,   // 172 //
    0x00,   // 173 //
    0x00,   // 174 //
    0x00,   // 175 //
    0x00,   // 176 //
    0x00,   // 177 //
    0x00,   // 178 //
    0x00,   // 179 //
    0x00,   // 180 //
    0x00,   // 181 //
    0x00,   // 182 //
    0x01,   // 183 //
    0x8E,   // 184 //
    0x00,   // 185 //
    0x56,   // 186 //
    0x00,   // 187 //
    0x04,   // 188 //
    0x00,   // 189 //
    0x08,   // 190 //
    0x00,   // 191 //
    0x15,   // 192 //
    0x00,   // 193 //
    0xA8,   // 194 //
    0x00,   // 195 //
    0x00,   // 196 //
    0x00,   // 197 //
    0xFC,   // 198 //
    0x01,   // 199 //
    0x80,   // 200 //
    0x80,   // 201 //
    0x80,   // 202 //
    0x00,   // 203 //
    0x00,   // 204 //
    0x00,   // 205 //
    0x00,   // 206 //
    0x00,   // 207 //
    0x00,   // 208 //
    0x00,   // 209 //
    0x00,   // 210 //
    0x00,   // 211 //
    0x00,   // 212 //
    0x00,   // 213 //
    0x00,   // 214 //
    0x00,   // 215 //
    0x00,   // 216 //
    0x00,   // 217 //
    0x00,   // 218 //
    0x01,   // 219 //
    0x8F,   // 220 //
    0x00,   // 221 //
    0x5F,   // 222 //
    0x00,   // 223 //
    0x04,   // 224 //
    0x00,   // 225 //
    0x08,   // 226 //
    0x00,   // 227 //
    0x15,   // 228 //
    0x00,   // 229 //
    0xA8,   // 230 //
    0x00,   // 231 //
    0x00,   // 232 //
    0x00,   // 233 //
    0xFC,   // 234 //
    0x01,   // 235 //
    0x80,   // 236 //
    0x80,   // 237 //
    0x80,   // 238 //
    0x00,   // 239 //
    0x00,   // 240 //
    0x00,   // 241 //
    0x00,   // 242 //
    0x00,   // 243 //
    0x00,   // 244 //
    0x00,   // 245 //
    0x00,   // 246 //
    0x00,   // 247 //
    0x00,   // 248 //
    0x00,   // 249 //
    0x00,   // 250 //
    0x00,   // 251 //
    0x00,   // 252 //
    0x00,   // 253 //
    0x00,   // 254 //
    0x01,   // 255 //
    0x8F,   // 256 //
    0x00,   // 257 //
    0x68,   // 258 //
    0x00,   // 259 //
    0x04,   // 260 //
    0x00,   // 261 //
    0x08,   // 262 //
    0x00,   // 263 //
    0x15,   // 264 //
    0x00,   // 265 //
    0xA8,   // 266 //
    0x00,   // 267 //
    0x00,   // 268 //
    0x00,   // 269 //
    0xFC,   // 270 //
    0x01,   // 271 //
    0x80,   // 272 //
    0x80,   // 273 //
    0x80,   // 274 //
    0x00,   // 275 //
    0x00,   // 276 //
    0x00,   // 277 //
    0x00,   // 278 //
    0x00,   // 279 //
    0x00,   // 280 //
    0x00,   // 281 //
    0x00,   // 282 //
    0x00,   // 283 //
    0x00,   // 284 //
    0x00,   // 285 //
    0x00,   // 286 //
    0x00,   // 287 //
    0x00,   // 288 //
    0x00,   // 289 //
    0x00,   // 290 //
    0x01,   // 291 //
    0x90,   // 292 //
    0x00,   // 293 //
    0x71,   // 294 //
    0x00,   // 295 //
    0x04,   // 296 //
    0x00,   // 297 //
    0x08,   // 298 //
    0x00,   // 299 //
    0x15,   // 300 //
    0x00,   // 301 //
    0xA8,   // 302 //
    0x00,   // 303 //
    0x00,   // 304 //
    0x00,   // 305 //
    0xFC,   // 306 //
    0x01,   // 307 //
    0x80,   // 308 //
    0x80,   // 309 //
    0x80,   // 310 //
    0x00,   // 311 //
    0x00,   // 312 //
    0x00,   // 313 //
    0x00,   // 314 //
    0x00,   // 315 //
    0x00,   // 316 //
    0x00,   // 317 //
    0x00,   // 318 //
    0x00,   // 319 //
    0x00,   // 320 //
    0x00,   // 321 //
    0x00,   // 322 //
    0x00,   // 323 //
    0x00,   // 324 //
    0x00,   // 325 //
    0x00,   // 326 //
    0x01,   // 327 //
};

SPR_REC2 Book_cursor_spr = { 0, 0, 4, 8, 21, -96, 0, 0, 508, { 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, { 0x01000000, 60, 72, 8, 8, 21, 176, 0, 0, 508, 128, 128, 128, 0, 0, 0, 0, 0 }, { 0x01000000, 156, 72, 8, 8, 21, 192, 0, 0, 508, 128, 128, 128, 0, 0, 0, 0, 0 } };  // list Sel_no cursor sprite with its left and right page arrow sprites (its attribute word ends Book_entry_spr)

// attribute word of the up arrow sprite
UCHAR Book_up_attr[0x4] =
{
    0x00,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x01,   // 3 //
};

SPR_REC0 Book_up_spr = { 0, 0, 8, 8, 21, -48, 0, 0, 508, { 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };  // up arrow sprite of the list

// attribute word of the down arrow sprite
UCHAR Book_down_attr[0x4] =
{
    0x00,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x01,   // 3 //
};

SPR_REC1 Book_down_spr = { 0, 0, 8, 8, 21, -32, 0, 0, 508, { 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };  // down arrow sprite of the list

SPR_REC0 Reader_page_spr = { 24, 24, 256, 192, 21, 0, 0, 0, 509, { 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } };  // page sprite of the reader

// header sprites of the reader, not drawn by any code
GsSPRITE Reader_head_spr[6] =
{
    { 0x51000000, 48, 48, 208, 120, 21, 0, 0, 0, 508, 0, 0, 0, 0, 0, 0, 0, 0 },      // 0 //
    { 0x51000000, 48, 168, 48, 24, 21, 208, 0, 0, 508, 0, 0, 0, 0, 0, 0, 0, 0 },     // 1 //
    { 0x51000000, 96, 168, 48, 24, 21, 208, 24, 0, 508, 0, 0, 0, 0, 0, 0, 0, 0 },    // 2 //
    { 0x51000000, 144, 168, 48, 24, 21, 208, 48, 0, 508, 0, 0, 0, 0, 0, 0, 0, 0 },   // 3 //
    { 0x51000000, 192, 168, 48, 24, 21, 208, 72, 0, 508, 0, 0, 0, 0, 0, 0, 0, 0 },   // 4 //
    { 0x51000000, 240, 168, 16, 24, 21, 208, 96, 0, 508, 0, 0, 0, 0, 0, 0, 0, 0 },   // 5 //
};

// left and right page arrow sprites of the reader
GsSPRITE Reader_arrow[2] =
{
    { 0x01000000, 24, 116, 8, 8, 21, 0, 192, 0, 508, 128, 128, 128, 0, 0, 0, 0, 0 },     // 0 //
    { 0x01000000, 272, 116, 8, 8, 21, 16, 192, 0, 508, 128, 128, 128, 0, 0, 0, 0, 0 },   // 1 //
};

GsSPRITE Reader_exit_spr = { 0x01000000, 280, 111, 24, 16, 28, 128, 136, 0, 484, 64, 64, 64, 0, 0, 0, 0, 0 };  // exit icon sprite of the reader

// index of the first page image of each file, by file id
UCHAR File_page_top[16] =
{
    0x00,   // 0 //
    0x02,   // 1 //
    0x05,   // 2 //
    0x0B,   // 3 //
    0x0D,   // 4 //
    0x0E,   // 5 //
    0x11,   // 6 //
    0x15,   // 7 //
    0x19,   // 8 //
    0x1E,   // 9 //
    0x21,   // 10 //
    0x24,   // 11 //
    0x26,   // 12 //
    0x28,   // 13 //
    0x2A,   // 14 //
    0x2C,   // 15 //
};

// file ids shown by the list, by file set and slot (0xFE=placeholder, 0xFF=empty)
UCHAR Book_tbl[2][16] =
{
    { 0x0F, 0x02, 0xFE, 0x03, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0C, 0x0D, 0x0E, 0xFF, 0xFF, 0xFF },   // 0 //
    { 0x0F, 0x02, 0xFE, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0xFF },   // 1 //
};

GsBOXF Pickup_fade_box = { 0x60000000, 0, 0, 320, 240, 0, 0, 0 };  // full-screen box used to fade the file screens
