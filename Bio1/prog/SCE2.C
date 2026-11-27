/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SCE2.C
 */

#include "TYPES.H"
#include <STDIO.H>
#include "SCE2.H"
#include "CUT.H"
#include "MOJIDISP.H"
#include "PSP.H"
#include "SCE1.H"
#include "SCE3.H"
#include "SCRATCHPAD.H"
#include "FLAG.H"
#include "SCENARIO.H"
#include "MONITOR.H"

// functions without a C definition
INT Mess_set();

PANEL_WORK Sce_panel_work;   // state record shared by the passcode panel, computer lab and lab slides

USHORT Sce_cut_save;        // camera cut saved when a screen opens, restored when it closes

static INT Sce_open_old;                        // screen-open flag (scenario flag 0x20) of the previous frame
static UCHAR Sce_panel_act_flg;                 // 1 while the tile grid is drawn and accepts input
static UCHAR Sce_panel_tile_tbl[9];               // tile states of the 3x3 grid, row-major (1=lit)
static UCHAR Sce_bss_pad[0x4];   // padding: keeps the following data at its linked position

VOID Sce_scr_clr(VOID)
{
    VIEW(INT, Sce_panel_work) = 0;  // INT* view of the field (a plain access changes the code)
    Sce_open_old = 0;
}

VOID Pc_event_ck(VOID)
{
    pPanel_work = &Sce_panel_work;
    if (Flg_ck(&MAIN.SaveData.Scenario_flg, 0x20) != 0)  // address passed as an integer
    {
        if (Sce_open_old == 0)
        {
            *(INT*)pPanel_work = 0;  // INT* view of the same memory
        }
        if (Flg_ck(&MAIN.Room_flg, ROOM_PANEL) != 0)  // address passed as an integer
        {
            Sce_panel_disp();
        }
        else if (Flg_ck(&MAIN.Room_flg, ROOM_COM) != 0)  // address passed as an integer
        {
            Sce_com_disp();
        }
        else if (Flg_ck(&MAIN.Room_flg, ROOM_SLIDE) != 0)  // address passed as an integer
        {
            Sce_slide_disp();
        }
    }
    Sce_open_old = Flg_ck(&MAIN.SaveData.Scenario_flg, 0x20);  // address passed as an integer
}

VOID Sce_panel_disp(VOID)
{
    Sce_panel_rno_tbl[pPanel_work->Rno]();
    Sce_panel_draw();
    Sce_panel_spr_set();
}

VOID Sce_panel_init(VOID)
{
    UCHAR i;
    UCHAR* s;

    pPanel_work->Rno = pPanel_work->Rno + 1;
    pPanel_work->Sub_rno = 0;
    Sce_cut_save = CUT_NO_CUR;
    MAIN.SaveData.Cut_no = 5;
    s = Sce_panel_init_tbl00;
    Cut_change();

    STOP_FLG_CUR &= ~STOP_PLAY_MASK;

    pPanel_work->Cursor_x = 0;
    pPanel_work->Cursor_y = 0;
    if (Flg_ck(&MAIN.SaveData.Scenario_flg, 0x1E) != 0)  // address passed as an integer
    {
        if (G.Player.Id == 1)
        {
            s = Sce_panel_init_tbl01;
        }
    }
    else if (Flg_ck(&MAIN.SaveData.Scenario_flg, 0x1F) != 0)  // address passed as an integer
    {
        s = Sce_panel_init_tbl02;
        if (G.Player.Id == 1)
        {
            s = Sce_panel_init_tbl03;
        }
    }
    for (i = 0; i < 9; i++)
    {
        Sce_panel_tile_tbl[i] = *s++;
    }
    Sce_panel_act_flg = 0;
    Mess_set(0xE, 0xFF);
}

VOID Sce_panel_main(VOID)
{
    Sce_panel_upd_tbl[pPanel_work->Sub_rno]();
}

VOID Sce_panel_wait_mess(VOID)
{
    if (!(MESS_FLG_CUR & MESS_FLG_ACTIVE))
    {
        if (MAIN.SaveData.Mess_flg != 0)
        {
            pPanel_work->Rno = 2;
            pPanel_work->Sub_rno = 0;

            return;
        }
        pPanel_work->Sub_rno = 1;
        Sce_panel_act_flg = 1;
    }
}

VOID Sce_panel_pad(VOID)
{
    INT Row_up;
    INT Col_left;
    INT Row_down;
    INT Col_right;

    if (G.Padd1_trg & PAD_UP)
    {
        Row_up = pPanel_work->Cursor_y;
        if (Row_up > 0)
        {
            pPanel_work->Cursor_y = Row_up - 1;
        }
    }
    if (G.Padd1_trg & PAD_DOWN)
    {
        Row_down = pPanel_work->Cursor_y;
        if (Row_down < 2)
        {
            pPanel_work->Cursor_y = Row_down + 1;
        }
    }
    if (G.Padd1_trg & PAD_LEFT)
    {
        Col_left = pPanel_work->Cursor_x;
        if (Col_left > 0)
        {
            pPanel_work->Cursor_x = Col_left - 1;
        }
    }
    if (G.Padd1_trg & PAD_RIGHT)
    {
        Col_right = pPanel_work->Cursor_x;
        if (Col_right < 2)
        {
            pPanel_work->Cursor_x = Col_right + 1;
        }
    }
    if (KEY_TRG_CUR & 0x4000)
    {
        pPanel_work->Sub_rno = pPanel_work->Sub_rno + 1;
    }
    if (MAIN.SaveData.Key_trg & 0x8000)
    {
        pPanel_work->Rno = pPanel_work->Rno + 1;
        pPanel_work->Sub_rno = 0U;
    }
}

VOID Sce_panel_tile_chg(VOID)
{
    USHORT x;
    USHORT y;
    USHORT i;
    USHORT sum;

    sum = 0;
    Snd_se_play(SE_BANK_ROOM, 0x17, 0);
    y = pPanel_work->Cursor_y;
    x = pPanel_work->Cursor_x;
    Sce_panel_tile_tbl[y * 3 + x] ^= 1;
    if (y != 0)
    {
        Sce_panel_tile_tbl[(y - 1) * 3 + x] ^= 1;
    }
    if (y < 2)
    {
        Sce_panel_tile_tbl[(y + 1) * 3 + x] ^= 1;
    }
    if (x != 0)
    {
        Sce_panel_tile_tbl[y * 3 + (x - 1)] ^= 1;
    }
    if (x < 2)
    {
        Sce_panel_tile_tbl[y * 3 + (x + 1)] ^= 1;
    }
    for (i = 0; i < 9; i++)
    {
        sum += Sce_panel_tile_tbl[i];
    }
    if (sum == 9)
    {
        pPanel_work->Sub_rno = 3;
        pPanel_work->Anm_rno = 0;
        Sce_panel_act_flg = 0;
        pPanel_work->Timer = 30;
    }
    else
    {
        pPanel_work->Sub_rno = 1;
    }
}

VOID Sce_panel_anm_main(VOID)
{
    Sce_panel_anm_tbl[pPanel_work->Anm_rno]();
}

VOID Sce_panel_anm_start(VOID)
{
    USHORT Time_cnt;

    Time_cnt = pPanel_work->Timer - 1;
    pPanel_work->Timer = Time_cnt;
    if ((Time_cnt << 0x10) == 0)
    {
        pPanel_work->Anm_rno = pPanel_work->Anm_rno + 1;
        pPanel_work->Timer = 1U;
        pPanel_work->Cnt = 0;
    }
}

VOID Sce_panel_anm_step(VOID)
{
    USHORT Time_cnt;
    SHORT Tile_idx;
    SHORT Delay_idx;
    UCHAR delay;

    Time_cnt = pPanel_work->Timer - 1;
    pPanel_work->Timer = Time_cnt;
    if ((Time_cnt << 0x10) == 0)
    {
        Tile_idx = pPanel_work->Cnt;
        pPanel_work->Cnt = Tile_idx + 1;
        Sce_panel_tile_tbl[Sce_panel_seq_tbl[Tile_idx]] = 0;  // sign-extends the halfword
        Delay_idx = pPanel_work->Cnt;
        pPanel_work->Cnt = Delay_idx + 1;
        delay = Sce_panel_seq_tbl[Delay_idx];  // sign-extends the halfword
        pPanel_work->Timer = delay;
        if (delay == 0)
        {
            pPanel_work->Anm_rno = pPanel_work->Anm_rno + 1;
            pPanel_work->Timer = 0x1EU;
        }
    }
}

VOID Sce_panel_anm_end(VOID)
{
    USHORT Time_cnt;

    Time_cnt = pPanel_work->Timer - 1;
    pPanel_work->Timer = Time_cnt;
    if ((Time_cnt << 0x10) == 0)
    {
        pPanel_work->Rno = pPanel_work->Rno + 1;
        Snd_se_play(SE_BANK_ROOM, 0x18, 0);
        Flg_on(&MAIN.SaveData.Scenario_flg, 0x21);  // address passed as an integer
    }
}

VOID Sce_panel_end(VOID)
{
    STOP_FLG_CUR |= STOP_PLAY_MASK;

    G.SaveData.Cut_no = Sce_cut_save;
    Cut_change();
    Sce_panel_act_flg = 0;
    Flg_off(&MAIN.SaveData.Scenario_flg, 0x20);  // address passed as an integer
}

VOID Sce_panel_draw(VOID)
{
    INT x;

    INT y;
    INT c;
    INT k;
    INT Row;
    INT y2;
    INT Row3;
    USHORT i;
    USHORT j;

    if (Sce_panel_act_flg != 0)
    {
        sprintf(ScratchPad->Text_buf, "$$$$$$$$$"); // CHAR* for sprintf: Text_buf is a UCHAR buffer
        x = j * 8 + 0x7B;
        Print14(x, 0x8C, 0, 1);
        Print14(x, 0xC4, 0, 1);

        for (i = 0; i < 3; i++)
        {
            sprintf(ScratchPad->Text_buf, "$$"); // CHAR* for sprintf: Text_buf is a UCHAR buffer

            k = i;
            y = (k + 1) * 0xE;

            Print14(0x7B, y + 0x8C, 0, 1);
            sprintf(ScratchPad->Text_buf, "$"); // CHAR* for sprintf: Text_buf is a UCHAR buffer
            Print14(0xBB, y + 0x8C, 0, 1);

            j = 0;
            Row = k;
            Row3 = Row * 3;
            y2 = y;
            for (; j < 3; j++)
            {
                c = 3;
                if (pPanel_work->Cursor_x == j)
                {
                    if (pPanel_work->Cursor_y == Row)
                    {
                        c = 1;
                    }
                }

                sprintf(ScratchPad->Text_buf, "%c$", Row3 + j + 0x31); // CHAR* for sprintf: Text_buf is a UCHAR buffer
                Print14(j * 0x10 + 0x8B, y2 + 0x8C, c, 1);
            }
        }
    }
}

VOID Sce_panel_spr_set(VOID)
{
    INT Spr_id;
    UCHAR* tile;

    tile = Sce_panel_tile_tbl;
    Spr_id = 1;
    do
    {
        if (*tile++ != 0)
        {
            Psp_on(Spr_id & 0xFF);
        }
        else
        {
            Psp_off(Spr_id & 0xFF);
        }
        Spr_id += 1;
    } while ((Spr_id & 0xFF) < 0xAU);
}

// initial tile layout (row-major, 1=lit), default
UCHAR Sce_panel_init_tbl00[0xC] =
{
    0x00,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x00,   // 3 //
    0x00,   // 4 //
    0x00,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x01,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
};

// initial tile layout when scenario flag 0x1F is set
UCHAR Sce_panel_init_tbl02[0xC] =
{
    0x01,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x01,   // 3 //
    0x01,   // 4 //
    0x00,   // 5 //
    0x01,   // 6 //
    0x00,   // 7 //
    0x01,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
};

// initial tile layout when scenario flag 0x1E is set and the player id is 1
UCHAR Sce_panel_init_tbl01[0xC] =
{
    0x00,   // 0 //
    0x01,   // 1 //
    0x00,   // 2 //
    0x01,   // 3 //
    0x01,   // 4 //
    0x01,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x01,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
};

// initial tile layout when scenario flag 0x1F is set and the player id is 1
UCHAR Sce_panel_init_tbl03[0xC] =
{
    0x01,   // 0 //
    0x00,   // 1 //
    0x01,   // 2 //
    0x00,   // 3 //
    0x01,   // 4 //
    0x01,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x01,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
};

// solved animation: (tile index, delay in frames) pairs; a zero delay ends the sequence
UCHAR Sce_panel_seq_tbl[0x14] =
{
    0x00,   // 0 //
    0x1E,   // 1 //
    0x01,   // 2 //
    0x19,   // 3 //
    0x02,   // 4 //
    0x10,   // 5 //
    0x05,   // 6 //
    0x08,   // 7 //
    0x08,   // 8 //
    0x06,   // 9 //
    0x07,   // 10 //
    0x04,   // 11 //
    0x06,   // 12 //
    0x02,   // 13 //
    0x03,   // 14 //
    0x01,   // 15 //
    0x04,   // 16 //
    0x00,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
};

// panel state handlers: init, update, finish
SCE2_FUNC Sce_panel_rno_tbl[3] =
{
    Sce_panel_init,     // 0 //
    Sce_panel_main,   // 1 //
    Sce_panel_end,   // 2 //
};

// update substate handlers: wait message, input, toggle, animation
SCE2_FUNC Sce_panel_upd_tbl[4] =
{
    Sce_panel_wait_mess,       // 0 //
    Sce_panel_pad,              // 1 //
    Sce_panel_tile_chg,             // 2 //
    Sce_panel_anm_main,   // 3 //
};

// solved animation step handlers: start, step, finish
SCE2_FUNC Sce_panel_anm_tbl[3] =
{
    Sce_panel_anm_start,    // 0 //
    Sce_panel_anm_step,     // 1 //
    Sce_panel_anm_end,   // 2 //
};
