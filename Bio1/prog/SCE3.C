/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SCE3.C
 */

#include "TYPES.H"
#include <RAND.H>
#include <STDIO.H>
#include <STRINGS.H>
#include "SCE3.H"
#include "CD.H"
#include "CUT.H"
#include "MAP.H"
#include "MOJIDISP.H"
#include "SCE1.H"
#include "SCE2.H"
#include "SND.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "SCENARIO.H"
#include "MONITOR.H"

// functions without a C definition
INT Mess_set(UCHAR, INT);

UCHAR Sce3_logo_state;                          // logo animation state: 0=not started, 1=running, 2=finished
PANEL_WORK* pPanel_work;  // interactive screen state record in use

static UCHAR Com_pic_flg;                   // message picture flags: bits 0-1 picture, 0x10/0x20 strip A/B, 0x80 flip
static UCHAR Com_sts_no;                  // status banner index into Com_sts_tbl (0=none)
static UCHAR Com_door_sel;                    // selected door in bits 0-6, 0x80 once that door is unlocked
static SCHAR Com_led_flg;                   // LED sprites shown: bit 0 first LED, bit 1 second LED (flickers)
static UCHAR Com_col_r;                     // sprite tint red (0x80=normal)
static UCHAR Com_col_g;                     // sprite tint green (0x80=normal)
static UCHAR Com_col_b;                     // sprite tint blue (0x80=normal)
static COM_WND_WORK Com_wnd_wk[4];              // windows 0-2 text Pl_wk, window 3 on-screen keyboard
static COM_WND_WORK* pCom_wnd;              // window being animated by Com_wnd_main
static COM_TEXT_REC Com_text_wk[2];        // editable text fields
static COM_CURSOR_WORK Com_cursor_wk;                    // text caret / menu Sel_no bar state
static UCHAR Com_pad[0x8];  // padding: keeps the following data at its linked position
static COM_TYPER_WORK Com_typer_wk;                    // auto-typer state
static COM_TYPER_WORK* pCom_typer;                // Com_typer_wk as seen by the typer handlers

static SHORT Com_cut_no;                // camera cut to switch to; bit 0x80 set while the change is pending
static USHORT Com_cut_wait;             // frames to wait before the pending camera cut is applied
static LIGHT_CUT_DATA Com_light_bak[3];  // room lights saved on entry, restored on exit
static INT Com_ambient_bak[3];                // room ambient colour (red, green, blue) saved on entry

static COM_LOGO_WORK Com_logo_wk[9];  // logo animation sprite slots
static COM_LOGO_WORK* pCom_logo;      // logo slot being updated

VOID Sce_com_disp(VOID)
{
    Com_lab_tbl[pPanel_work->Rno]();
    Com_cursor_draw();
    Com_text_draw();
    Com_wnd_draw();
    Com_key_draw();
    Com_door_pic_draw();
    Com_sts_draw();
    Com_led_draw();

    Com_cut_set();
}

VOID Com_lab_init(VOID)
{
    ROOM_WORK* p;
    UCHAR i;

    *(INT*)pPanel_work = 1;  // state 1 (update), substate/step/substep 0; pointer type conversion (no code)

    VIEW(UCHAR, Sce_cut_save) = CUT_NO_CUR;  // save the current camera cut id; UCHAR view of the field (plain access changes the code)

    MAIN.SaveData.Cut_no = 4;
    Cut_check(1);

    STOP_FLG_CUR &= ~(STOP_PLAYER | STOP_SUB_PLAYER | STOP_ENEMY_MOVE | STOP_PAD_LOCK);  // clear room update flags
    p = (ROOM_WORK*)&pPanel_work->Bak_pos_y;  // pointer type conversion (no code)
    *(INT*)p = MAIN.Player.Pos_y;  // save the player height in the screen state; pointer type conversion (no code)
    G.Player.Pos_y = -9000;  // move the player out of view

    Com_light_bak[0] = ROOM_CUR->Light[0];
    Com_light_bak[1] = MAIN.pRoom->Light[1];
    Com_light_bak[2] = MAIN.pRoom->Light[2];
    Com_ambient_bak[0] = MAIN.pRoom->Ambient_r;
    Com_ambient_bak[1] = MAIN.pRoom->Ambient_g;
    Com_ambient_bak[2] = MAIN.pRoom->Ambient_b;

    MAIN.pRoom->Light[0] = Com_light_tbl[0];
    MAIN.pRoom->Light[1] = Com_light_tbl[1];
    MAIN.pRoom->Light[2] = Com_light_tbl[2];
    p = MAIN.pRoom;
    p->Ambient_r = 1800;
    p->Ambient_g = 1800;
    p->Ambient_b = 1800;
    GsSetAmbient(p->Ambient_r, p->Ambient_g, 1800);
    Com_door_sel = 0;
    Com_cursor_wk.Input_en = 0;
    for (i = 0; i < 4; i++)
    {
        Com_wnd_wk[i].enabled = 0;
        Com_wnd_wk[i].Anm_req = 0;
        Com_wnd_wk[i].alpha = 0;

        Com_wnd_wk[i].Keyboard_flg = 0;
    }

    Com_wnd_wk[3].Keyboard_flg = 1;

    for (i = 0; i < 2; i++)
    {
        Com_text_wk[i].enabled = 0;
    }

    Com_col_r = Com_col_g = Com_col_b = 0x80;

    G.Enemy[0].Type = 0x40;
    G.Enemy[1].Type = 0x40;
}

VOID Com_lab_main(VOID)
{
    Com_stage_tbl[pPanel_work->Sub_rno]();
    Com_wnd_main();
    Com_typer_main();
}

VOID Com_stage_boot(VOID)
{
    if (pPanel_work->Anm_rno == 0)
    {
        Com_boot_main();
        return;
    }
    Com_boot_logo();
}

VOID Com_boot_main(VOID)
{
    Com_boot_tbl[pPanel_work->Boot_rno]();
    ScratchPad->Spr.tpage = 7;
    ScratchPad->Spr.cy = 0x1EB;
    ScratchPad->Spr.mx = 0x3C;
    ScratchPad->Spr.my = 0x34;
    ScratchPad->Spr.y = -0x2B;
    ScratchPad->Spr.v = 0x68;
    ScratchPad->Spr.w = 0x78;
    ScratchPad->Spr.cx = 0;
    ScratchPad->Spr.rotate = 0;
    ScratchPad->Spr.x = 0;
    ScratchPad->Spr.u = 0;
    ScratchPad->Spr.h = 0x68;
    GsSortSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 3U);
}

VOID Com_boot_zoom_start(VOID)
{
    pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;
    Com_led_flg = 1;
    ScratchPad->Spr.scaley = 0x64;
    ScratchPad->Spr.scalex = 0;
    Com_pic_flg = 0x83;
    pPanel_work->Timer = 0;
}

VOID Com_boot_zoom_wide(VOID)
{
    SHORT* pScaley = &ScratchPad->Spr.scaley;  // plain Spr.scaley changes scheduling
    pPanel_work->Timer = pPanel_work->Timer + 0x800;
    ScratchPad->Spr.attribute = 0x01000000;
    ScratchPad->Spr.r = 0x80;
    ScratchPad->Spr.g = 0x80;
    ScratchPad->Spr.b = 0x80;
    *pScaley = 0x64;
    ScratchPad->Spr.scalex = pPanel_work->Timer;
    if (VIEW(SHORT, pPanel_work->Timer) >= 0x1000)  // SHORT view of the field (plain access changes the code)
    {
        pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;
        pPanel_work->Timer = 0x64U;
    }
}

VOID Com_boot_zoom_tall(VOID)
{
    SHORT Scale_y;

    Scale_y = pPanel_work->Timer + 0x800;
    pPanel_work->Timer = Scale_y;
    if (Scale_y >= 0x1001)
    {
        pPanel_work->Timer = 0x1000U;
        pPanel_work->Cnt = 0x5A;
        pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;
        Com_pic_flg = 0;
    }
    ScratchPad->Spr.attribute = 0x01000000;
    ScratchPad->Spr.scalex = 0x1000;
    ScratchPad->Spr.scaley = pPanel_work->Timer;
}

VOID Com_boot_flash(VOID)
{
    ULONG* s = &ScratchPad->Spr.attribute;
    PANEL_WORK* p = pPanel_work;
    PANEL_WORK* q;
    INT v;
    SHORT t;

    p->Cnt--;
    v = 0x41000000;
    if (p->Cnt & 1)
    {
        v = 0x01000000;
    }
    *s = v;
    ScratchPad->Spr.r = p->Cnt / 2;
    ScratchPad->Spr.g = p->Cnt / 2;
    t = p->Cnt;
    ScratchPad->Spr.scalex = 0x1000;
    ScratchPad->Spr.scaley = 0x1000;
    ScratchPad->Spr.b = t / 2;
    if (p->Cnt == 0x3C)
    {
        Snd_se_play(SE_BANK_ROOM, 0x17, 0);
    }
    q = pPanel_work;
    if (q->Cnt < 0x1E)
    {
        Com_led_flg = 3;
    }
    if (q->Cnt == 0)
    {
        Com_led_flg = 1;
        Sce3_logo_state = 0;
        q->Anm_rno++;
        pPanel_work->Boot_rno = 0;

        if ((G.SaveData.Key & 0x4000) || (KEY_NOW & 0x8000))
        {
            Com_pic_flg = 1;
            pPanel_work->Boot_rno = 1;
            pPanel_work->Timer = 0x1E;
        }
    }
}

VOID Com_boot_logo(VOID)
{
    USHORT Time_cnt;

    USHORT Time_cnt2;

    UCHAR sub;

    sub = pPanel_work->Boot_rno;
    switch (sub)
    {
    case 0:
        Com_logo_main(pPanel_work);
        if (Sce3_logo_state == 2)
        {
            Com_pic_flg = 1;
            pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;
            pPanel_work->Timer = 0x1EU;
            return;
        }

        return;
    case 1:
        Time_cnt = pPanel_work->Timer - 1;
        pPanel_work->Timer = Time_cnt;
        if ((Time_cnt << 0x10) == 0)
        {
            pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;
            pPanel_work->Timer = 0x1EU;
            pPanel_work->Cnt = 0;
            return;
        }
        break;
    case 2:
        if (Com_fade_main(8, 2) & 0xFF)
        {
            Time_cnt2 = pPanel_work->Timer - 1;
            pPanel_work->Timer = Time_cnt2;
            if ((Time_cnt2 << 0x10) == 0)
            {
                pPanel_work->Sub_rno = pPanel_work->Sub_rno + 1;
                pPanel_work->Anm_rno = 0;
            }
        }
        break;
    }
}

VOID Com_stage_login(VOID)
{
    Com_login_tbl[pPanel_work->Anm_rno]();
}

VOID Com_login_init(VOID)
{
    pPanel_work->Anm_rno = pPanel_work->Anm_rno + 1;
    pPanel_work->Boot_rno = 0;
    Com_wnd_set(0, -0x90, -0x68, 0x10, 4, 3, &Com_login_row_tbl);
    Com_wnd_wk[0].enabled = 1;

    Com_wnd_wk[0].Anm_req = 1;

    Com_wnd_wk[3].Anm_req = 1;

    Com_wnd_wk[3].enabled = 1;

    Com_text_wk[0].x = 7;

    Com_wnd_wk[3].Repeat_timer = 0;
    Com_login_row_tbl[2].width = 0;
    Com_text_wk[0].enabled = 0;
    Com_text_wk[0].Wnd_idx = 0;

    Com_text_wk[0].y = 2;
    Com_text_wk[0].masked = 0;
    Com_text_wk[1].enabled = 0;
    Com_text_clr(0);

    Com_cut_no = 0x85;

    Com_sts_no = 0;

    Com_cut_wait = 0;
    pPanel_work->Cursor_x = 1;

    pPanel_work->Cursor_y = 0;

    G.Enemy[0].Type = 0x47;
    G.Enemy[1].Type = 0x47;
}

VOID Com_login_in_sub(VOID)
{
    Com_login_in_tbl[pPanel_work->Boot_rno]();
}

VOID Com_login_open_wait(VOID)
{
    if (Com_wnd_wk[0].Anm_req == 0 && Com_wnd_wk[3].Anm_req == 0)
    {
        pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;
        pPanel_work->Timer = 0;
        Com_text_wk[0].enabled = 1;
        Com_cursor_wk.Input_en = 1;
        Com_cursor_wk.mode = 0;
    }
}

VOID Com_login_key(VOID)
{
    SHORT key;

    UCHAR* pCancel = &Com_wnd_wk[3].nRow;  // the keyboard window's nRow byte doubles as the cancel flag

    *pCancel = 0;

    key = Com_key_get();
    if (key == -1)
    {
        pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;
        return;
    }
    if (key == -2 || (G.SaveData.Key_trg & 0x8000))
    {
        pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;
        *pCancel = 1;

        return;
    }
    if (key != 0)
    {
        Com_text_add(key, (SHORT)pPanel_work->Timer * sizeof(COM_TEXT_REC) + Com_text_wk[0].text);  // real width or sign conversion
        if (Com_cursor_wk.pos != 7)
        {
            Com_cursor_wk.Blink_cnt = 0;
        }
    }
}

VOID Com_login_pass(VOID)
{
    if (Com_wnd_wk[3].nRow != 0)
    {
        VIEW(INT, pPanel_work->Rno) = 2;  // state 2 (finish); INT view of the field (plain access changes the code)
        return;
    }

    pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;
    Com_text_wk[1].enabled = 1;
    Com_login_row_tbl[2].width = 0xC;
    Com_text_wk[1].masked = 1;
    Com_text_wk[1].x = 0xC;
    Com_text_wk[1].y = 3;
    Com_text_wk[1].Wnd_idx = 0;
    pPanel_work->Timer = 1;
    Com_text_clr(1);
}

VOID Com_login_close(VOID)
{
    if (Com_wnd_wk[3].nRow != 0)
    {
        VIEW(INT, pPanel_work->Rno) = 2;  // state 2 (finish); INT view of the field (plain access changes the code)
        return;
    }
    {
        INT Tmp = pPanel_work->Boot_rno + 1;
        pPanel_work->Boot_rno = Tmp;
    }
    Com_wnd_wk[0].Anm_req = 0x81;
    Com_wnd_wk[3].Anm_req = 0x81;
    G.Enemy[0].Type = 0x40;
    G.Enemy[1].Type = 0x40;

    Com_pic_flg = 2;

    Com_cursor_wk.Input_en = 0;
    Com_cut_wait = 0;
    Com_cut_no = 0x84;
}

VOID Com_login_close_wait(VOID)
{
    if (Com_wnd_wk[0].Anm_req == 0 && Com_wnd_wk[3].Anm_req == 0)
    {
        pPanel_work->Anm_rno = pPanel_work->Anm_rno + 1;
        pPanel_work->Boot_rno = 0;
        Com_text_wk[0].enabled = 0;
        Com_text_wk[1].enabled = 0;
        Com_wnd_wk[0].enabled = 0;
        Com_wnd_wk[3].enabled = 0;
    }
}

VOID Com_login_ck_sub(VOID)
{
    Com_login_ck_tbl[pPanel_work->Boot_rno]();
}

VOID Com_login_type_start(VOID)
{
    pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;
    Com_typer_wk.active = 1;
    Com_typer_wk.line = 0;
}

VOID Com_login_type_next(VOID)
{
    SHORT line;
    UCHAR* pTyper = &Com_typer_wk.active;

    if (*pTyper == 0)
    {
        line = Com_typer_wk.line + 1;
        Com_typer_wk.line = line;
        if (line == 2)
        {
            pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;
            return;
        }
        *pTyper = 1;
    }
}

VOID Com_login_ck(VOID)
{
    if (strcmp(Com_text_wk[0].text, Com_user_tbl) != 0 || strcmp(Com_text_wk[1].text, Com_pass_tbl) != 0)
    {
        pPanel_work->Boot_rno = 3;
        Com_sts_no = 4;
        pPanel_work->Timer = 0x5A;
        Snd_se_play(SE_BANK_ROOM, 0x19, 0);
        return;
    }
    pPanel_work->Boot_rno = 4;
    pPanel_work->Timer = 0x5A;
}

VOID Com_login_fail(VOID)
{
    SHORT Time_cnt;
    SHORT Next_cnt;

    Time_cnt = pPanel_work->Timer;
    Next_cnt = Time_cnt - (Time_cnt > 0);
    pPanel_work->Timer = Next_cnt;
    if ((Next_cnt << 0x10) == 0 && (G.Enemy[0].Type & 0x20) && (G.Enemy[1].Type & 0x20))
    {
        pPanel_work->Anm_rno = 0;
        pPanel_work->Boot_rno = 0;
    }

    if ((SHORT)pPanel_work->Timer == 0x4B)  // real width or sign conversion
    {
        G.Enemy[0].Type = 0x44;
        G.Enemy[1].Type = 0x44;
    }
}

VOID Com_login_ok(VOID)
{
    SHORT Time_cnt;
    SHORT Next_cnt;

    Time_cnt = pPanel_work->Timer;
    Next_cnt = Time_cnt - (Time_cnt > 0);
    pPanel_work->Timer = Next_cnt;
    if ((Next_cnt << 0x10) == 0)
    {
        pPanel_work->Sub_rno = pPanel_work->Sub_rno + 1;
        pPanel_work->Anm_rno = 0;
    }

    if ((SHORT)pPanel_work->Timer >= 0x33)  // real width or sign conversion
    {
        Com_quad_draw(-0x3C, -0x5F, 3, 1, Com_strip_a_tbl, 7);
        return;
    }
    Com_pic_flg |= 0x10;
}

VOID Com_stage_menu(VOID)
{
    if (pPanel_work->Anm_rno == 0)
    {
        Com_menu_set();
        return;
    }
    Com_menu_main();
}

VOID Com_menu_set(VOID)
{
    pPanel_work->Anm_rno = pPanel_work->Anm_rno + 1;
    pPanel_work->Boot_rno = 0;
    Com_wnd_set(1, -0x10, -0x38, 4, 3, 3, &Com_side_row_tbl);

    Com_wnd_wk[1].enabled = 0;
    Com_wnd_wk[1].Anm_req = 0;

    Com_wnd_set(2, -0x90, -0x68, 0x10, 3, 3, &Com_pa_row_tbl);
    Com_wnd_wk[2].enabled = 1;

    Com_wnd_wk[2].Anm_req = 1;
    Com_cursor_wk.Wnd_idx = 1;

    Com_cursor_wk.mode = 1;
    Com_cursor_wk.Bar_w = 5;
    Com_cut_no = 0x85;

    Com_cursor_wk.Base_x = 0;
    Com_cursor_wk.Base_y = 0;
    Com_cursor_wk.pos = 0;

    Com_cursor_wk.Blink_cnt = 0;

    Com_cursor_wk.Input_en = 0;
    Com_text_wk[0].enabled = 0;
    Com_text_wk[1].enabled = 0;
    Com_sts_no = 0;

    Com_cut_wait = 0;

    pPanel_work->Timer = 0xC;

    pPanel_work->Cnt = 2;
}

VOID Com_menu_main(VOID)
{
    if (Com_cursor_wk.Input_en != 0)
    {
        if ((G.Padd1_trg & PAD_UP) && Com_cursor_wk.pos != 0)
        {
            Com_cursor_wk.pos--;
            Com_cursor_wk.Blink_cnt = 0;
        }
        if ((G.Padd1_trg & PAD_DOWN) && Com_cursor_wk.pos < pPanel_work->Cnt)
        {
            Com_cursor_wk.pos++;
            Com_cursor_wk.Blink_cnt = 0;
        }
    }
    Com_menu_tbl[pPanel_work->Boot_rno]();
}

VOID Com_menu_open(VOID)
{
    SHORT Time_cnt;
    SHORT Next_cnt;

    Time_cnt = pPanel_work->Timer;
    if (Time_cnt != 0)
    {
        Next_cnt = Time_cnt - 1;
        pPanel_work->Timer = Next_cnt;
        if ((Next_cnt << 0x10) == 0)
        {
            Com_wnd_wk[1].enabled = 1;
            Com_wnd_wk[1].Anm_req = 1;
        }
    }
    else if (Com_wnd_wk[0].Anm_req == 0 && Com_wnd_wk[1].Anm_req == 0)
    {
        pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;
        Com_cursor_wk.Input_en = 1;

        G.Enemy[0].Type = 0x47;
        G.Enemy[1].Type = 0x47;
    }
}

VOID Com_menu_sel(VOID)
{
    if (G.SaveData.Key_trg & 0x4000)
    {
        switch (Com_cursor_wk.pos)
        {
        case 0:
            Com_door_sel = 0;
            pPanel_work->Boot_rno = 4;
            break;
        case 1:
            Com_door_sel = 1;
            pPanel_work->Boot_rno = 4;
            break;
        case 2:
            pPanel_work->Boot_rno = 2;
            Com_cursor_wk.Input_en = 0;
            break;
        }
    }
    if (G.SaveData.Key_trg & 0x8000)
    {
        pPanel_work->Boot_rno = 2;
        Com_cursor_wk.Input_en = 0;
    }
}

VOID Com_menu_cmd(VOID)
{
    pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;
    Com_wnd_set(0, 0x10, -0x28, 3, 4, 4, &Com_menu_row_tbl);

    Com_wnd_wk[0].enabled = 1;
    Com_cursor_wk.Input_en = 1;
    Com_wnd_wk[0].alpha = 0x80;
    Com_cursor_wk.mode = 1;

    Com_cursor_wk.Base_y = 2;
    Com_cursor_wk.Bar_w = 4;

    Com_wnd_wk[0].visible = 0;

    Com_cursor_wk.Wnd_idx = 0;
    Com_cursor_wk.Base_x = 0;
    Com_cursor_wk.pos = 0;
    Com_cursor_wk.Blink_cnt = 0;

    pPanel_work->Cnt = 1;
    G.Enemy[0].Type = 0x40;
}

VOID Com_menu_exit(VOID)
{
    UCHAR flag;
    PANEL_WORK* q;

    flag = G_BASE->SaveData.Key_trg >> 15;
    if (G.SaveData.Key_trg & 0x4000)
    {
        if (Com_cursor_wk.pos == 0)
        {
            *(INT*)pPanel_work = 2;  // state 2 (finish); pointer type conversion (no code)
            return;
        }
        flag = 1;
    }
    if (flag != 0)
    {
        flag = 1;
        pPanel_work->Boot_rno = flag;
        Com_cursor_wk.Wnd_idx = flag;
        Com_cursor_wk.mode = flag;
        Com_cursor_wk.Input_en = flag;
        q = pPanel_work;
        Com_cursor_wk.Bar_w = 5;

        G.Enemy[0].Type = 0x47;
        G.Enemy[1].Type = 0x47;

        Com_cursor_wk.Base_x = 0;
        Com_cursor_wk.Base_y = 0;
        Com_cursor_wk.pos = 0;
        Com_cursor_wk.Blink_cnt = 0;
        Com_wnd_wk[0].enabled = 0;
        q->Cnt = 2;
    }
}

VOID Com_menu_close(VOID)
{
    pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;

    Com_pic_flg = 0x12;

    Com_wnd_wk[1].Anm_req = 0x81;
    Com_cut_no = 0x84;
    Com_cursor_wk.Input_en = 0;
    pPanel_work->Timer = 0xC;
}

VOID Com_menu_close_wait(VOID)
{
    SHORT Time_cnt;
    SHORT Next_cnt;
    SHORT Settle_cnt;
    PANEL_WORK* obj;
    UCHAR t;

    Time_cnt = pPanel_work->Timer;
    if (Time_cnt != 0)
    {
        Next_cnt = Time_cnt - 1;
        pPanel_work->Timer = Next_cnt;
        if ((Next_cnt << 0x10) == 0)
        {
            Com_wnd_wk[2].Anm_req = 0x81;
        }
    }
    else if (Com_wnd_wk[1].Anm_req == 0 && Com_wnd_wk[2].Anm_req == 0)
    {
        pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;

        obj = pPanel_work;
        Com_wnd_wk[1].enabled = 0;
        Com_wnd_wk[2].enabled = 0;
        obj->Timer = 0x5A;
        t = Com_door_sel;
        if (t == 0)
        {
            G.Enemy[0].Type = 0x46;
            Settle_cnt = 1;
        }
        else
        {
            G.Enemy[0].Type = 0x48;
            Settle_cnt = 2;
        }
        obj->Cnt = Settle_cnt;
    }
}

VOID Com_menu_arm_wait(VOID)
{
    USHORT count;

    if (MAIN.Enemy[0].Type & 0x20)
    {
        count = pPanel_work->Cnt - 1;
        pPanel_work->Cnt = count;
        if ((count << 0x10) != 0)
        {
            G.Enemy[0].Type = 0x46;
            return;
        }
        pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;
        G.Enemy[1].Type = 0x47;
        Com_led_flg |= 2;
    }
}

VOID Com_menu_blink(VOID)
{
    USHORT Time_cnt;

    Time_cnt = pPanel_work->Timer - 1;
    pPanel_work->Timer = Time_cnt;
    if ((Time_cnt << 0x10) == 0)
    {
        if (Com_door_sel == 1)
        {
            pPanel_work->Sub_rno = 4;
            pPanel_work->Anm_rno = 0;
            return;
        }
        pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;
        Com_sts_no = 0;
        pPanel_work->Timer = 0x1EU;
        pPanel_work->Cnt = 0;
        return;
    }
    Com_sts_no = ((pPanel_work->Timer >> 4) ^ 1) & 1;
}

VOID Com_menu_cnt(VOID)
{
    USHORT Time_cnt;
    SHORT count;

    Time_cnt = pPanel_work->Timer - 1;
    pPanel_work->Timer = Time_cnt;
    if ((Time_cnt << 0x10) == 0)
    {
        pPanel_work->Timer = 0x1EU;
        count = pPanel_work->Cnt + 1;
        pPanel_work->Cnt = count;
        if (count == 3)
        {
            pPanel_work->Sub_rno = 3;
            pPanel_work->Anm_rno = 0;
            Com_pic_flg = 0x32;
            pPanel_work->Timer = 0x1EU;
            pPanel_work->Cnt = 0U;
        }
    }
    if (pPanel_work->Cnt != 0)
    {
        Com_quad_draw(-0x3C, -0x5F, 0x40, pPanel_work->Cnt, Com_strip_b_tbl, 7);
    }
}

VOID Com_stage_cmd(VOID)
{
    Com_cmd_tbl[pPanel_work->Anm_rno]();
}

VOID Com_cmd_set(VOID)
{
    USHORT Time_cnt;

    Time_cnt = pPanel_work->Timer - 1;
    pPanel_work->Timer = Time_cnt;
    if ((Time_cnt << 0x10) == 0)
    {
        pPanel_work->Anm_rno = pPanel_work->Anm_rno + 1;
        pPanel_work->Boot_rno = 0;
        Com_led_flg = 1;
        Com_wnd_set(2, -0x90, -0x68, 0x10, 4, 4, &Com_pb_row_tbl);

        Com_text_wk[0].Wnd_idx = 2;
        Com_text_wk[0].x = 0xC;

        Com_wnd_wk[2].enabled = 1;
        Com_wnd_wk[2].Anm_req = 1;
        Com_wnd_wk[3].Anm_req = 1;

        Com_text_wk[0].enabled = 1;
        Com_text_wk[0].y = 3;
        Com_text_wk[0].masked = 1;
        Com_text_clr(0);

        Com_cut_no = 0x85;

        Com_cursor_wk.mode = 0;
        Com_cursor_wk.Input_en = 0;
        Com_wnd_wk[3].enabled = 1;
        Com_wnd_wk[3].Repeat_timer = 0;

        pPanel_work->Cursor_x = 1;

        pPanel_work->Cursor_y = 0;
        pPanel_work->Timer = 0U;
        Snd_se_play(SE_BANK_ROOM, 0x19, 0);

        G.Enemy[0].Type = 0x47;
        G.Enemy[1].Type = 0x47;
    }
}

VOID Com_cmd_input_wait(VOID)
{
    if (Com_wnd_wk[2].Anm_req == 0 && Com_wnd_wk[3].Anm_req == 0)
    {
        Com_cursor_wk.Input_en = 1;
        if (pPanel_work->Boot_rno != 0)
        {
            pPanel_work->Anm_rno = pPanel_work->Anm_rno + 1;
            pPanel_work->Boot_rno = 0U;
            return;
        }
        Com_login_key();
    }
}

VOID Com_cmd_sub(VOID)
{
    Com_cmd_sub_tbl[pPanel_work->Boot_rno]();
}

VOID Com_cmd_close(VOID)
{
    {
        INT Tmp = pPanel_work->Boot_rno + 1;
        pPanel_work->Boot_rno = Tmp;
    }
    Com_wnd_wk[2].Anm_req = 0x81;
    Com_wnd_wk[3].Anm_req = 0x81;

    Com_pic_flg = 0x32;

    Com_cursor_wk.Input_en = 0;
    G.Enemy[0].Type = 0x40;
    G.Enemy[1].Type = 0x40;
}

VOID Com_cmd_close_end(VOID)
{
    UCHAR* pAnm_req2 = &Com_wnd_wk[2].Anm_req;
    if (*pAnm_req2 == 0 && Com_wnd_wk[3].Anm_req == 0)
    {
        pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;

        Com_wnd_wk[2].enabled = 0;

        *pAnm_req2 = 0;

        Com_wnd_wk[3].enabled = 0;

        if (Com_wnd_wk[3].nRow != 0)
        {
            G.Enemy[1].Type = 0x46;
        }
        else
        {
            Com_typer_wk.active = 1;
            Com_typer_wk.line = 0;
        }
        Com_cut_wait = 2;
        Com_cut_no = 0x84;
    }
}

VOID Com_cmd_type_wait(VOID)
{
    if (Com_wnd_wk[3].nRow != 0)
    {
        if (G.Enemy[1].Type & 0x20)
        {
            VIEW(INT, pPanel_work->Rno) = 0x201;  // state 1, substate 2 (menu stage); INT view of the field (plain access changes the code)
        }
    }
    else if (Com_typer_wk.active == 0)
    {
        pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;
    }
}

VOID Com_cmd_ck(VOID)
{
    if (strcmp(Com_text_wk[0].text, Com_release_tbl) != 0)
    {
        pPanel_work->Boot_rno = 4;
        Com_sts_no = 2;
        pPanel_work->Timer = 0x5A;
        Snd_se_play(SE_BANK_ROOM, 0x19, 0);
        return;
    }
    pPanel_work->Boot_rno = 5;
    Com_sts_no = 3;
    pPanel_work->Timer = 0x5A;
    Com_led_flg |= 2;
}

VOID Com_cmd_fail(VOID)
{
    SHORT Time_cnt;
    SHORT Next_cnt;

    Time_cnt = pPanel_work->Timer;
    Next_cnt = Time_cnt - (Time_cnt > 0);
    pPanel_work->Timer = Next_cnt;
    if ((Next_cnt << 0x10) == 0 && (G.Enemy[0].Type & 0x20) && (G.Enemy[1].Type & 0x20))
    {
        pPanel_work->Sub_rno = 2;
        pPanel_work->Anm_rno = 0;
    }

    if ((SHORT)pPanel_work->Timer == 0x4B)  // real width or sign conversion
    {
        G.Enemy[0].Type = 0x44;
        G.Enemy[1].Type = 0x44;
    }
}

VOID Com_cmd_ok(VOID)
{
    SHORT Time_cnt;
    SHORT Next_cnt;

    Time_cnt = pPanel_work->Timer;
    Next_cnt = Time_cnt - (Time_cnt > 0);
    pPanel_work->Timer = Next_cnt;
    if ((Next_cnt << 0x10) == 0)
    {
        pPanel_work->Sub_rno = pPanel_work->Sub_rno + 1;
        pPanel_work->Anm_rno = 0;
        Com_text_wk[0].enabled = 0;
        Com_led_flg = 1;
    }
}

VOID Com_stage_unlock(VOID)
{
    COM_QUAD_REC* marker;
    COM_POINT_REC* origin;

    Com_unlock_tbl[pPanel_work->Anm_rno]();
    if (!(Com_door_sel & 0x7F))
    {
        origin = (COM_POINT_REC*)Com_door_ofs0;  // the two bytes of the record are x and y
        marker = Com_door_mark0;
    }
    else
    {
        origin = (COM_POINT_REC*)Com_door_ofs1;  // the two bytes of the record are x and y
        marker = Com_door_mark1;
    }
    if (pPanel_work->Timer & 0x10)
    {
        Com_quad_draw(origin->x + 0xFFC4, origin->y + 0xFFA1, 5, 1, marker, 0xA);
    }
}

VOID Com_cmd_unlock(VOID)
{
    INT Lock_flg;

    pPanel_work->Anm_rno = pPanel_work->Anm_rno + 1;

    pPanel_work->Boot_rno = 0;

    Com_led_flg = 1;
    Com_sts_no = 0;

    Com_cut_no = 0x84;

    Com_door_sel |= 0x80;  // mark the door unlocked

    pPanel_work->Timer = 0x5A;
    Map_area_set(5);

    Lock_flg = 0x26;  // unlock flag of door 1

    if (!(Com_door_sel & 0x7F))
    {
        Lock_flg = 0x25;  // unlock flag of door 0
    }

    if (Flg_ck(&MAIN.SaveData.Key_flg, Lock_flg) != 0)  // address as integer
    {
        pPanel_work->Timer = 0x20;
        return;
    }
    Flg_on(&MAIN.SaveData.Key_flg, Lock_flg);  // address as integer

    G.Enemy[0].Type = 0x45;
    G.Enemy[1].Type = 0x45;
}

VOID Com_cmd_unlock_hold(VOID)
{
    USHORT Time_cnt;

    Time_cnt = pPanel_work->Timer - 1;
    pPanel_work->Timer = Time_cnt;
    if ((Time_cnt << 0x10) == 0)
    {
        pPanel_work->Anm_rno = pPanel_work->Anm_rno + 1;

        pPanel_work->Timer = 0x10U;
    }
}

VOID Com_cmd_unlock_msg(VOID)
{
    USHORT Time_cnt;

    if (pPanel_work->Boot_rno == 0)
    {
        pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;

        Com_wnd_set(0, -0x38, -0x40, 5, 1, 1, &Com_unlock_row_tbl);

        Com_wnd_wk[0].enabled = 1;
        pPanel_work->Cnt = 0x96U;
    }
    Time_cnt = pPanel_work->Cnt - 1;
    pPanel_work->Cnt = Time_cnt;
    if ((Time_cnt << 0x10) == 0 || (G.SaveData.Key_trg & 0x4000) || (KEY_TRG_NOW & 0x8000))
    {
        VIEW(INT, pPanel_work->Rno) = 0x201;  // state 1, substate 2 (menu stage); INT view of the field (plain access changes the code)
        Com_wnd_wk[0].enabled = 0;
        Com_door_sel = 0;

        return;
    }
    Com_quad_draw(-0x38, -0x40, 3, 1, Com_unlock_quad_tbl, 0xA);
}

VOID Com_lab_end(VOID)
{
    if (pPanel_work->Sub_rno == 0)
    {
        Com_end_main();
        return;
    }
    Com_end_room_set();
}

VOID Com_end_main(VOID)
{
    Com_end_tbl[pPanel_work->Anm_rno]();

    Com_typer_main();
    Com_wnd_main();

    if (pPanel_work->Anm_rno != 0)
    {
        ScratchPad->Spr.attribute = 0x05000000;
        ScratchPad->Spr.r = 0x80;
        ScratchPad->Spr.g = 0x80;
        ScratchPad->Spr.b = 0x80;
        ScratchPad->Spr.tpage = 7;
        ScratchPad->Spr.cy = 0x1EB;
        ScratchPad->Spr.mx = 0x3C;
        ScratchPad->Spr.my = 0x34;
        ScratchPad->Spr.y = -0x2B;
        ScratchPad->Spr.v = 0x68;
        ScratchPad->Spr.w = 0x78;
        ScratchPad->Spr.cx = 0;
        ScratchPad->Spr.rotate = 0;
        ScratchPad->Spr.x = 0;
        ScratchPad->Spr.u = 0;
        ScratchPad->Spr.h = 0x68;
        GsSortSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 3U);
    }
}

VOID Com_end_start(VOID)
{
    UCHAR i;

    switch (pPanel_work->Boot_rno)
    {
    case 0:
        pPanel_work->Boot_rno += 1;
        for (i = 0; i < 4; i++)
        {
            if (Com_wnd_wk[i].enabled != 0)
            {
                Com_wnd_wk[i].Anm_req = 0x81;
            }
            else
            {
                Com_wnd_wk[i].Anm_req = 0;
            }
        }
        Com_text_wk[0].text[0] = 0x13;  // key codes of the shutdown text
        Com_text_wk[0].text[1] = 8;
        Com_text_wk[0].text[2] = 0x15;
        Com_text_wk[0].text[3] = 0x14;

        Com_typer_wk.active = 1;

        Com_cursor_wk.Input_en = 0;

        Com_text_wk[0].text[4] = 0;

        Com_typer_wk.line = 0;

        Com_cut_wait = 0;
        Com_cut_no = 0x84;

        break;
    case 1:
        if (Com_typer_wk.active == 0)
        {
            pPanel_work->Cnt = 0;
            pPanel_work->Boot_rno = pPanel_work->Boot_rno + 1;
            Com_led_flg = 3;
            break;
        }
        break;
    case 2:
        Com_fade_main(8, 0);
        if (pPanel_work->Cnt != 0 && Com_wnd_wk[0].Anm_req == 0 && Com_wnd_wk[1].Anm_req == 0 && Com_wnd_wk[2].Anm_req == 0 && Com_wnd_wk[3].Anm_req == 0)
        {
            Com_wnd_wk[0].enabled = 0;
            Com_wnd_wk[1].enabled = 0;
            Com_wnd_wk[2].enabled = 0;
            Com_wnd_wk[3].enabled = 0;
            pPanel_work->Anm_rno = pPanel_work->Anm_rno + 1;
            Com_pic_flg = 0;
            ScratchPad->Spr.scalex = 0x1000;
            ScratchPad->Spr.scaley = 0x1000;
            Com_led_flg = 0;
            pPanel_work->Timer = 0x1000;
        }
        break;
    }
}

VOID Com_end_shrink_v(VOID)
{
    pPanel_work->Timer = pPanel_work->Timer - 0x7D0;
    ScratchPad->Spr.scalex = 0x1000;
    ScratchPad->Spr.scaley = pPanel_work->Timer;
    if ((SHORT)pPanel_work->Timer < 0x65)  // sign-extends the halfword
    {
        pPanel_work->Anm_rno = pPanel_work->Anm_rno + 1;
        pPanel_work->Timer = 0x1000U;
    }
}

VOID Com_end_shrink_h(VOID)
{
    PANEL_WORK* p = pPanel_work;
    USHORT t;

    p->Timer -= 0x800;
    ScratchPad->Spr.attribute = 0x01000000;
    ScratchPad->Spr.r = 0x80;
    ScratchPad->Spr.g = 0x80;
    ScratchPad->Spr.b = 0x80;
    t = p->Timer;
    ScratchPad->Spr.scaley = 0x60;
    ScratchPad->Spr.scalex = t;
    if ((SHORT)p->Timer <= 0)  // real width or sign conversion
    {
        ScratchPad->Spr.scalex = 0;
        p->Sub_rno++;
        pPanel_work->Timer = 0x1E;
    }
}

VOID Com_end_room_set(VOID)
{
    ROOM_WORK* w;

    if (--VIEW(SHORT, pPanel_work->Timer) == 0)  // SHORT view of the field (plain access changes the code)
    {
        ROOM_CUR->Light[0] = Com_light_bak[0];
        MAIN.pRoom->Light[1] = Com_light_bak[1];
        MAIN.pRoom->Light[2] = Com_light_bak[2];

        w = MAIN.pRoom;

        w->Ambient_r = Com_ambient_bak[0];
        w->Ambient_g = Com_ambient_bak[1];
        w->Ambient_b = Com_ambient_bak[2];
        GsSetAmbient(w->Ambient_r, w->Ambient_g, w->Ambient_b);

        G.Enemy[0].Type = 0x80;
        G.Enemy[1].Type = 0x80;
        pPanel_work->Rno = 0;

        STOP_FLG_CUR |= STOP_PLAYER | STOP_SUB_PLAYER | STOP_ENEMY_MOVE | STOP_PAD_LOCK;  // restore room update flags

        G.SaveData.Cut_no = Sce_cut_save;

        Cut_check(1);
        Flg_off(&MAIN.SaveData.Scenario_flg, 0x20);  // clear the screen-active flag; address as integer

        G.Player.Pos_y = pPanel_work->Bak_pos_y;
    }
}

VOID Com_wnd_main(VOID)
{
    INT i;

    pCom_wnd = Com_wnd_wk;
    i = 0;
    do
    {
        if (pCom_wnd->enabled != 0 && (pCom_wnd->Anm_req & 1))
        {
            if (pCom_wnd->Anm_sts == 0)
            {
                Com_wnd_anm_init();
            }
            else
            {
                Com_wnd_anm_main();
            }
        }
        i += 1;
        pCom_wnd++;
    } while ((i & 0xFF) < 4U);
}

VOID Com_wnd_anm_init(VOID)
{
    SHORT v;

    pCom_wnd->Anm_sts++;
    if (pCom_wnd->Anm_req & 0x80)
    {
        v = 0xFF;
        if (pCom_wnd->Keyboard_flg != 0)
        {
            v = 0x80;
        }
        pCom_wnd->alpha = v;
        pCom_wnd->visible = 1;
    }
    else
    {
        pCom_wnd->alpha = 0;
        pCom_wnd->visible = 1;
    }
}

VOID Com_wnd_anm_main(VOID)
{
    UCHAR mode;

    COM_WND_WORK* p;
    COM_WND_WORK* q;
    INT c;
    INT v;
    INT w;

    q = pCom_wnd;
    c = q->Anm_req;
    do
    {
        mode = q->Keyboard_flg;
    } while (0);
    p = q;
    if (c & 0x80)
    {
        v = p->alpha;
        if (mode != 0)
        {
            v -= 8;
        }
        else
        {
            v -= 0x10;
        }
        p->alpha = v;
        if (q->alpha <= 0)
        {
            q->Anm_sts++;

            pCom_wnd->alpha = 0;
            pCom_wnd->Anm_req = 0;
            pCom_wnd->Anm_sts = 0;
        }
    }
    else
    {
        v = p->alpha;
        if (mode != 0)
        {
            w = v + 8;
        }
        else
        {
            w = v + 0x10;
        }
        p->alpha = w;

        v = pCom_wnd->alpha;
        if (pCom_wnd->Keyboard_flg != 0 && v < 0x80)
        {
            return;
        }

        if (pCom_wnd->Keyboard_flg != 0 || v >= 0xFF)
        {
            pCom_wnd->Anm_sts++;
            pCom_wnd->alpha = 0x80;
            pCom_wnd->visible = 0;
            pCom_wnd->Anm_req = 0;
            pCom_wnd->Anm_sts = 0;
        }
    }
}

VOID Com_typer_main(VOID)
{
    UCHAR* pTyper = &Com_typer_wk.active;
    pCom_typer = (COM_TYPER_WORK*)pTyper;  // pointer type conversion (no code)
    if (*pTyper != 0)
    {
        Com_typer_tbl[Com_typer_wk.state]();
    }
}

VOID Com_typer_start(VOID)
{
    pCom_typer->state = pCom_typer->state + 1;
    pCom_typer->Moji_idx = 0;
}

VOID Com_typer_key(VOID)
{
    UCHAR v;
    UCHAR a;
    UCHAR b;

    pCom_typer->state++;
    v = Com_text_wk[pCom_typer->line].text[pCom_typer->Moji_idx];
    if (v == 0)
    {
        pCom_typer->state = 3;
        G.Enemy[0].Type = 0x46;
        return;
    }
    a = Com_arm_tbl[v];
    if (a & 0x80)
    {
        G.Enemy[1].Type = (a & 0x7F) | 0x40;
    }
    else
    {
        G.Enemy[0].Type = (a & 0x7F) | 0x40;
    }
    b = Com_arm_tbl[(Com_text_wk[0].text + 2)[pCom_typer->line * sizeof(COM_TEXT_REC) + pCom_typer->Moji_idx]];
    if (G.Player.Id & 1)
    {
        if ((a & 0x80) == (b & 0x80))
        {
            pCom_typer->timer = 0xD;
        }
        else
        {
            pCom_typer->timer = 6;
        }
    }
    else
    {
        pCom_typer->timer = (rand() & 0xF) + 0xA;
    }
}

VOID Com_typer_wait(VOID)
{
    COM_TYPER_WORK* obj;
    INT n;

    obj = pCom_typer;
    if ((--obj->timer << 16) == 0)
    {
        obj->Moji_idx += 1 + (G.Player.Id & 1);
        n = obj->Moji_idx;
        if (Com_text_wk[obj->line].text[n] == 0)
        {
            obj->state = 3;
            G.Enemy[0].Type = 0x46;
        }
        else
        {
            obj->state = 1;
        }
    }
}

VOID Com_typer_end(VOID)
{
    if (G.Enemy[0].Type & 0x20)
    {
        pCom_typer->active = 0;
        pCom_typer->state = 0;
    }
}

SHORT Com_key_get(VOID)
{
    INT rsv_slot[1];
    SHORT cur;
    SHORT r;
    INT oldx;
    INT oldy;
    {
        COM_WND_WORK* f = &Com_wnd_wk[3];

        f->enabled &= 0x7F;
    }
    cur = pPanel_work->Cursor_y * 8 + pPanel_work->Cursor_x;
    if (G.SaveData.Key_trg & 0x4000)
    {
        r = cur;
        switch (cur)
        {
        case 0:
            r = -2;
            break;
        case 0x17:
            r = -1;
            break;
        case 0x1D:
            r = -3;
            break;
        default:
            if (r >= 0x17)
            {
                r--;
            }
            break;
        }
        {
            UCHAR* f = &Com_wnd_wk[3].enabled;

            *f |= 0x80;
        }
        return r;
    }
    if (Com_wnd_wk[3].cols == (G.Padd1 & (PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT)) && Com_wnd_wk[3].cols != 0)
    {
        if (Com_wnd_wk[3].Repeat_timer != 0)
        {
            Com_wnd_wk[3].Repeat_timer--;
            return 0;
        }
        Com_wnd_wk[3].Repeat_timer = 15;
    }
    else
    {
        Com_wnd_wk[3].Repeat_timer = 8;
    }
    Com_wnd_wk[3].cols = G.Padd1_trg & (PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT);
    {
        PANEL_WORK* p = pPanel_work;

        oldx = p->Cursor_x;
        oldy = p->Cursor_y;
        if ((G.Padd1 & PAD_UP) && oldy > 0)
        {
            p->Cursor_y = oldy - 1;
        }
    }
    if (G.Padd1 & PAD_DOWN)
    {
        PANEL_WORK* p = pPanel_work;

        if (p->Cursor_y < 3)
        {
            p->Cursor_y = p->Cursor_y + 1;
        }
    }
    if (G.Padd1 & PAD_LEFT)
    {
        PANEL_WORK* p = pPanel_work;

        if (--p->Cursor_x < 0)
        {
            p->Cursor_x = 7;
        }
    }
    if (G.Padd1 & PAD_RIGHT)
    {
        PANEL_WORK* p = pPanel_work;
        INT t = p->Cursor_x;

        p->Cursor_x = t + 1;
        if (cur == 0x1D)
        {
            p->Cursor_x = t + 2;
        }
        if (pPanel_work->Cursor_x >= 8)
        {
            pPanel_work->Cursor_x = 0;
        }
    }
    {
        PANEL_WORK* p = pPanel_work;
        INT y = p->Cursor_y;
        SHORT c2 = y * 8 + p->Cursor_x;

        if (c2 == 0x1F)
        {
            p->Cursor_y = y - 1;
        }
        if (c2 == 0x1E)
        {
            pPanel_work->Cursor_x = pPanel_work->Cursor_x - 1;
        }
    }
    if (oldx != pPanel_work->Cursor_x || oldy != pPanel_work->Cursor_y)
    {
        UCHAR* f = &Com_wnd_wk[3].enabled;

        *f |= 0x80;
    }
    return 0;
}

VOID Com_text_add(SHORT ch, UCHAR* buf)
{
    UCHAR* pos;
    UCHAR n;

    if ((USHORT)(ch + 2) >= 2)  // real width or sign conversion
    {
        pos = &Com_cursor_wk.pos;
        n = *pos;
        if (ch == -3)
        {
            if (n != 0 && (n < 7 || buf[n] == 0))
            {
                (*pos)--;
            }
            buf[Com_cursor_wk.pos] = 0;
        }
        else
        {
            buf[n] = ch;
            if (n < 7)
            {
                (*pos)++;
            }
        }
    }
}

VOID Com_text_clr(INT Text_no)
{
    UCHAR* p;
    UCHAR i;
    INT t;

    i = 0;
    p = Com_text_wk[Text_no & 0xFF].text;
    do
    {
        p[i] = 0;
        i++;
    } while (i < 9);
    t = (Text_no & 0xFF) * sizeof(COM_TEXT_REC);
    Com_cursor_wk.Wnd_idx = *(&Com_text_wk[0].Wnd_idx + t);
    Com_cursor_wk.Base_x = *BYTE_OFS(USHORT, &Com_text_wk[0].x, t);  // USHORT view of the field (plain access changes the code)
    Com_cursor_wk.Base_y = *BYTE_OFS(USHORT, &Com_text_wk[0].y, t);  // USHORT view of the field (plain access changes the code)
    Com_cursor_wk.pos = 0;
    Com_cursor_wk.Blink_cnt = 0;
    Com_cursor_wk.Text_idx = Text_no;
}

VOID Com_wnd_set(UCHAR win, SHORT x, SHORT y, SHORT cols, USHORT rows, UCHAR nRow, VOID* pRow)
{
    Com_wnd_wk[win].x = x;
    Com_wnd_wk[win].y = y;
    Com_wnd_wk[win].cols = cols;

    Com_wnd_wk[win].alpha = 0x80;
    Com_wnd_wk[win].visible = 0;
    Com_wnd_wk[win].Anm_sts = 0;

    Com_wnd_wk[win].rows = rows;
    Com_wnd_wk[win].nRow = nRow;
    Com_wnd_wk[win].pRow = pRow;
}

VOID Com_key_draw(VOID)
{
    UCHAR* flags = &Com_wnd_wk[3].enabled;
    PANEL_WORK* q;

    UCHAR Bak_r;
    UCHAR Bak_g;
    UCHAR Bak_b;

    SHORT Key_idx;

    if ((*flags & 0x7F) && Com_wnd_wk[3].alpha != 0)
    {
        q = pPanel_work;

        ScratchPad->Spr.attribute = 0x01000000;
        ScratchPad->Spr.tpage = 9;
        ScratchPad->Spr.cy = 0x1EC;
        ScratchPad->Spr.cx = 0;

        ScratchPad->Spr.w = 0x18;
        ScratchPad->Spr.h = 0x18;

        ScratchPad->Spr.mx = 0;
        ScratchPad->Spr.my = 0;
        do
        {
            ScratchPad->Spr.x = q->Cursor_x * 0x18 - 0x58;
            ScratchPad->Spr.y = q->Cursor_y * 0x18 + 2;
            ScratchPad->Spr.u = q->Cursor_x * 0x18 + 0x10;
            Bak_r = Com_col_r;
            Bak_g = Com_col_g;
            Bak_b = Com_col_b;
        } while (0);

        Com_col_r = ((COM_WND_WORK*)&Com_wnd_wk)[3].alpha;  // pointer type conversion (no code)
        Com_col_g = Com_wnd_wk[3].alpha;
        Com_col_b = Com_wnd_wk[3].alpha;

        *&ScratchPad->Spr.r = Com_wnd_wk[3].alpha;  // *& form kept: the plain member store changes the scheduling
        *&ScratchPad->Spr.g = Com_wnd_wk[3].alpha;  // *& form kept: the plain member store changes the scheduling
        *&ScratchPad->Spr.b = Com_wnd_wk[3].alpha;  // *& form kept: the plain member store changes the scheduling
        ScratchPad->Spr.v = q->Cursor_y * 0x18 + 0x20;

        Key_idx = q->Cursor_y * 8 + q->Cursor_x;
        if (Key_idx == 0x17)
        {
            ScratchPad->Spr.h = 0x30;
        }
        if (Key_idx == 0x1D)
        {
            ScratchPad->Spr.w = 0x30;
        }
        if (Com_cursor_wk.Blink_hold != 0 || !(Com_cursor_wk.Blink_cnt & 0x10))
        {
            GsSortFastSprite(&ScratchPad->Spr, MAIN.Ot0 + G.Cbuf_id, 3);
        }
        if (*flags & 0x80)
        {
            Com_cursor_wk.Blink_hold = 0x14;
        }
        Com_cursor_wk.Blink_hold -= Com_cursor_wk.Blink_hold != 0;

        Com_quad_draw(-0x68, -0xE, 0x83, 0xA, Com_key_frm, 9);

        Com_col_r = Bak_r;
        Com_col_g = Bak_g;
        Com_col_b = Bak_b;
    }
}

VOID Com_wnd_draw(VOID)
{
    COM_QUAD_REC* t;
    COM_QUAD_REC* q;
    COM_WND_WORK* b;
    COM_WND_ROW_REC* row;
    SHORT i;

    SHORT Bak_r, Bak_g, Bak_b;

    SHORT j, k;

    INT f;

    USHORT x, y;

    INT pri;

    b = Com_wnd_wk;
    t = &Com_frm_tbl[1];
    q = Com_frm_tbl;

    Bak_r = Com_col_r;
    Bak_g = Com_col_g;
    Bak_b = Com_col_b;

    for (i = 0; i < 3; i++)
    {
        if (b->enabled == 0 || b->alpha == 0)
        {
            b++;
            continue;
        }

        t[0].x = (b->cols + 1) << 4;
        t[6].x = (b->cols + 1) << 4;
        t[1].y = (b->rows + 1) << 4;
        t[4].y = (b->rows + 1) << 4;
        t[2].x = (b->cols + 1) << 4;
        t[2].y = (b->rows + 1) << 4;
        t[3].count = b->cols;
        t[4].count = b->cols;
        t[5].count = b->rows;
        t[6].count = b->rows;

        x = b->x;
        y = b->y;
        f = b->visible;
        Com_col_r = b->alpha;
        Com_col_g = b->alpha;
        Com_col_b = b->alpha;
        Com_quad_draw(x, y, f != 0 ? 0x84 : 4, 8, q, 10);
        (&ScratchPad->Spr)->attribute = b->visible != 0 ? 0x41000000 : 0x01000000; // plain Spr.attribute changes scheduling

        ScratchPad->Spr.tpage = 9;
        ScratchPad->Spr.cy = 0x1EC;
        ScratchPad->Spr.cx = 0;
        ScratchPad->Spr.w = 0x10;
        ScratchPad->Spr.h = 0x10;
        ScratchPad->Spr.mx = 0;
        ScratchPad->Spr.my = 0;

        ScratchPad->Spr.r = b->alpha;
        ScratchPad->Spr.g = b->alpha;
        ScratchPad->Spr.b = b->alpha;

        row = b->pRow;
        ScratchPad->Spr.y = y + 0x10;
        for (j = 0; j < b->nRow; row++, j++)
        {
            ScratchPad->Spr.x = x + 0x10;
            if (row->width != 0)
            {
                ScratchPad->Spr.u = row->u;
                ScratchPad->Spr.v = row->v;
                ScratchPad->Spr.w = row->width * 8;
                ScratchPad->Spr.h = row->h * 16;
                ScratchPad->Spr.tpage = row->tpage;
                ScratchPad->Spr.cy = ((row->tpage - 7) >> 1) + 0x1EB;

                GsSortFastSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 4);
            }
            if (row->width < b->cols * 2)
            {
                ScratchPad->Spr.x = x + (row->width + 2) * 8;
                ScratchPad->Spr.u = 0xE0;
                ScratchPad->Spr.v = 0x30;
                ScratchPad->Spr.w = 8;
                ScratchPad->Spr.h = 0x10;
                ScratchPad->Spr.tpage = 9;
                ScratchPad->Spr.cy = 0x1EC;
                for (k = 0; k < b->cols * 2 - row->width; k++)
                {
                    GsSortFastSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 4);

                    ScratchPad->Spr.x += 8;
                }
            }
            ScratchPad->Spr.y += ScratchPad->Spr.h;
        }
        b++;
    }

    Com_col_r = Bak_r;
    Com_col_g = Bak_g;
    Com_col_b = Bak_b;
}

VOID Com_quad_draw(SHORT x, SHORT y, USHORT flags, UCHAR n, COM_QUAD_REC* pQuad, USHORT tpage)
{
    USHORT Top_flg;
    UCHAR i;
    UCHAR j;
    UCHAR Loop_ctr;

    INT t;

    Top_flg = flags & 0x40;
    i = 0;
    ScratchPad->Spr.cx = 0;
    ScratchPad->Spr.mx = 0;
    ScratchPad->Spr.my = 0;
    ScratchPad->Spr.tpage = tpage;
    ScratchPad->Spr.cy = (tpage - 7) / 2 + 0x1EB;
    for (; i < n; pQuad++, i++)
    {
        t = (flags & 0x80) << 23;
        t |= ((pQuad->flags & 0xC0) << 16) | KEEP_VALUE(0x01000000);  // KEEP_VALUE keeps the constant out of GCC 2.6.3's folding; the plain form and a local change the code
        ScratchPad->Spr.attribute = t;
        ScratchPad->Spr.x = x + pQuad->x;
        ScratchPad->Spr.y = y + pQuad->y;
        ScratchPad->Spr.u = pQuad->u;
        ScratchPad->Spr.v = pQuad->v;
        ScratchPad->Spr.w = pQuad->w;
        ScratchPad->Spr.h = pQuad->h;
        ScratchPad->Spr.r = Com_col_r;
        ScratchPad->Spr.g = Com_col_g;
        ScratchPad->Spr.b = Com_col_b;
        Loop_ctr = pQuad->count + (pQuad->count == 0);

        if (!(pQuad->flags & 1))
        {
            for (j = 0; j < Loop_ctr; j++)
            {
                if (!Top_flg)
                {
                    GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], flags & 0x3F);
                }
                else
                {
                    GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot1[G.Cbuf_id], 0x3FF);
                }
                ScratchPad->Spr.x += pQuad->w;
            }
        }
        else
        {
            for (j = 0; j < Loop_ctr; j++)
            {
                if (!Top_flg)
                {
                    GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], flags & 0x3F);
                }
                else
                {
                    GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot1[G.Cbuf_id], 0x3FF);
                }
                ScratchPad->Spr.y += pQuad->h;
            }
        }
    }
}

VOID Com_text_draw(VOID)
{
    COM_TEXT_REC* e;
    INT n;
    UCHAR i;
    UCHAR j;
    UCHAR k;
    SHORT x;
    SHORT y;
    USHORT Bx;
    USHORT by;

    e = Com_text_wk;

    ScratchPad->Spr.tpage = 9;
    ScratchPad->Spr.cy = 0x1EC;
    ScratchPad->Spr.cx = 0;
    ScratchPad->Spr.w = 16;
    ScratchPad->Spr.h = 16;
    ScratchPad->Spr.mx = 0;
    ScratchPad->Spr.my = 0;
    sprintf(ScratchPad->Text_buf, "*"); // CHAR* for sprintf: Text_buf is a UCHAR buffer
    for (i = 0; i < 2; e++, i++)
    {
        n = e->Wnd_idx;

        (&ScratchPad->Spr)->attribute = Com_wnd_wk[n].visible ? 0x41000000 : 0x01000000;
        if (e->enabled != 0 && Com_wnd_wk[n].enabled != 0 && Com_wnd_wk[n].Anm_req == 0 && Com_wnd_wk[n].alpha != 0)
        {
            Bx = Com_wnd_wk[n].x;
            by = Com_wnd_wk[n].y;
            {
                INT ofs_x = e->x * 8 + 16;
                ScratchPad->Spr.x = Bx + ofs_x;
            }
            {
                INT ofs_y = e->y * 16 + 16;
                ScratchPad->Spr.y = by + ofs_y;
            }
            for (j = 0; j < 8; j++)
            {
                k = e->text[j];
                if (k != 0)
                {
                    k--;
                    ScratchPad->Spr.u = (k & 0xF) * 16;
                    ScratchPad->Spr.v = k & 0x10;
                    if (e->masked != 0)
                    {
                        x = ScratchPad->Spr.x;
                        y = (&ScratchPad->Spr)->y; // plain Spr.y changes scheduling
                        sprintf(ScratchPad->Text_buf, "*"); // CHAR* for sprintf: Text_buf is a UCHAR buffer
                        Print8(x + 0xA4, y + 0x7C, 0, 0);

                        ScratchPad->Spr.attribute = Com_wnd_wk[n].visible ? 0x41000000 : 0x01000000;

                        ScratchPad->Spr.tpage = 9;
                        ScratchPad->Spr.cy = 0x1EC;
                        ScratchPad->Spr.w = 16;
                        ScratchPad->Spr.h = 16;
                        ScratchPad->Spr.u = 0xB0;
                        ScratchPad->Spr.x = x;
                        ScratchPad->Spr.y = y;
                        ScratchPad->Spr.cx = 0;
                        ScratchPad->Spr.v = 0xF;
                    }
                    GsSortFastSprite(&ScratchPad->Spr, MAIN.Ot0 + G.Cbuf_id, 3);
                }
                ScratchPad->Spr.x += 16;
            }
        }
    }
}

VOID Com_cursor_draw(VOID)
{
    UCHAR win;
    UCHAR col;
    UCHAR Loop_ctr;
    UCHAR c;
    UCHAR i;
    USHORT y;

    win = Com_cursor_wk.Wnd_idx;
    col = Com_cursor_wk.pos;
    if (Com_cursor_wk.Input_en == 0 || Com_wnd_wk[win].enabled == 0)
    {
        return;
    }
    (&ScratchPad->Spr)->attribute = 0x01000040;
    ScratchPad->Spr.tpage = 9;
    ScratchPad->Spr.cy = 0x1EC;
    ScratchPad->Spr.w = 0x10;
    ScratchPad->Spr.h = 0x10;
    ScratchPad->Spr.cx = 0;
    ScratchPad->Spr.mx = 0;
    ScratchPad->Spr.my = 0;
    Loop_ctr = ++Com_cursor_wk.Blink_cnt;
    if (Com_cursor_wk.mode == 0)
    {
        ScratchPad->Spr.u = 0xB0;
        ScratchPad->Spr.v = 0x10;
        {
            INT ofs_x = (Com_cursor_wk.Base_x + col * 2) * 8 + 0x10;
            ScratchPad->Spr.x = Com_wnd_wk[win].x + ofs_x;
        }
        y = Com_wnd_wk[win].y + (Com_cursor_wk.Base_y * 16 + 0x10);
        ScratchPad->Spr.y = y;
        c = Com_text_wk[Com_cursor_wk.Text_idx].text[col];
        if (Loop_ctr & 0x10)
        {
            if (c != 0)
            {
                return;
            }
            ScratchPad->Spr.v = 0xF;
        }
        if (col == 7 && c != 0)
        {
            ScratchPad->Spr.v = 0x1F;
            ScratchPad->Spr.h = 1;
            ScratchPad->Spr.y = y + 0xF;
        }
        GsSortFastSprite(&ScratchPad->Spr, MAIN.Ot0 + G.Cbuf_id, 3);
    }
    else if (!(Loop_ctr & 0x10))
    {
        ScratchPad->Spr.attribute = 0x41000040;
        ScratchPad->Spr.u = 0xC0;
        ScratchPad->Spr.v = 0x10;
        {
            INT ofs_x = Com_cursor_wk.Base_x * 8 + 8;
            ScratchPad->Spr.x = Com_wnd_wk[win].x + ofs_x;
        }
        {
            INT ofs_y = (col + Com_cursor_wk.Base_y) * 16 + 0xE;
            ScratchPad->Spr.y = Com_wnd_wk[win].y + ofs_y;
        }
        for (i = 0; i < Com_cursor_wk.Bar_w; i++)
        {
            GsSortFastSprite(&ScratchPad->Spr, MAIN.Ot0 + G.Cbuf_id, 3);
            ScratchPad->Spr.x += 0x10;
        }
    }
}

VOID Com_sts_draw(VOID)
{
    INT Quad_num;
    INT msg;

    if (Com_sts_no != 0)
    {
        Quad_num = 1;
        if (Com_sts_no == 4)
        {
            Quad_num = 2;
        }
        Com_quad_draw(-0x30, -0x3D, 0x40, Quad_num, &Com_sts_tbl[Com_sts_no], 7);  // pointer type conversion (no code)
    }
    msg = Com_pic_flg & 3;
    if (msg != 0)
    {
        if (msg == 2)
        {
            if (Com_pic_flg & 0x10)
            {
                Com_quad_draw(-0x3C, -0x5F, 0x40, 2, Com_strip_a_tbl, 7);  // pointer type conversion (no code)
            }
            if (Com_pic_flg & 0x20)
            {
                Com_quad_draw(-0x3C, -0x5F, 0x40, 3, Com_strip_b_tbl, 7);  // pointer type conversion (no code)
            }
        }
        Com_quad_draw(-0x3C, -0x5F, (Com_pic_flg & 0x80) | 0x40, 1, &Com_msg_pic_tbl[Com_pic_flg & 3], 7);  // pointer type conversion (no code)
    }
}

VOID Com_door_pic_draw(VOID)
{
    UCHAR sel;
    UCHAR* p;
    COM_QUAD_REC* q;
    INT idx;
    UCHAR i;
    UCHAR x;

    if (Com_door_sel & 0x80)
    {
        idx = Com_door_sel & 0x7F;
        sel = Com_door_sel;
        if (idx == 0)
        {
            if (sel)
            {
                p = Com_door_ofs0;
                q = Com_door_mark0;
            }
            else
            {
                p = Com_door_ofs0;
                q = Com_door_mark0;
            }
        }
        else
        {
            p = Com_door_ofs1;
            q = Com_door_mark1;
        }
        q++;
        x = *p++;
        Com_quad_draw(x + 0xFFC4, p[0] + 0xFFA1, 0x40, p[1], q, 0xA);
        Com_quad_draw(-0x18, -0x1F, 0x40, idx + 1, &Com_door_pic_tbl[idx], 0xA);
        Com_quad_draw(-0x20, -0x57, 0x40, 4, Com_door_frm_tbl, 8);
        for (i = 0; i < 6; i++)
        {
            Com_quad_draw(-0x20, i * 0x10 - 0x57, 0x40, 1, &Com_door_frm_tbl[-1], 0xA);
        }
    }
}

VOID Com_led_draw(VOID)
{
    ScratchPad->Spr.attribute = 0x41000040;
    ScratchPad->Spr.tpage = 7;
    ScratchPad->Spr.cy = 0x1EB;
    ScratchPad->Spr.w = 6;
    ScratchPad->Spr.cx = 0;
    ScratchPad->Spr.mx = 0;
    ScratchPad->Spr.my = 0;
    ScratchPad->Spr.h = 4;
    if (Com_led_flg & 1)
    {
        ScratchPad->Spr.x = 0x6F;
        ScratchPad->Spr.y = -0x28;
        ScratchPad->Spr.u = 0xE8;
        ScratchPad->Spr.v = 0x88;
        GsSortFastSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 4U);
    }
    if ((Com_led_flg & 2) && (rand() & 1))
    {
        ScratchPad->Spr.x = 0x6F;
        ScratchPad->Spr.y = -0x23;
        ScratchPad->Spr.u = 0xE8;
        ScratchPad->Spr.v = 0x98;
        GsSortFastSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 4U);
    }
}

INT Com_fade_main(SHORT step, SCHAR msg_flags)
{
    INT done;
    done = 1;

    if ((SHORT)pPanel_work->Timer != 2)  // sign-extends the halfword
    {
        if (pPanel_work->Cnt == 0)
        {
            Com_col_r -= step;
            Com_col_g -= step;
            Com_col_b -= step;
            if ((Com_col_r == 0 || Com_col_g == 0) || (done = 0, Com_col_b == 0))
            {
                Com_col_b = 0;
                Com_col_g = 0;
                Com_col_r = 0;
                Com_pic_flg = msg_flags;
                pPanel_work->Cnt++;
            }
            done = 0;
            return done;
        }
        Com_col_r += step;
        Com_col_g += step;
        Com_col_b += step;
        if ((Com_col_r >= 0x80U || Com_col_g >= 0x80U) || (done = 0, (Com_col_b < 0x80U) == 0))
        {
            Com_col_b = 0x80;
            Com_col_g = 0x80;
            Com_col_r = 0x80;
            pPanel_work->Cnt++;
            return 1;
        }
        return done;
    }
    return done;
}

VOID Com_cut_set(VOID)
{
    if (Com_cut_no & 0x80)
    {
        if (Com_cut_wait != 0)
        {
            Com_cut_wait -= 1;
            return;
        }
        G.SaveData.Cut_no = Com_cut_no & 0x7F;
        Cut_check(1);
        Com_cut_wait = 0;
        Com_cut_no &= 0x7F;
    }
}

VOID Com_logo_main(VOID* pPanel)
{
    COM_LOGO_WORK* logo;
    INT i;

    if (Sce3_logo_state == 0)
    {
        pCom_logo = Com_logo_wk;
        i = 0;
        do
        {
            pCom_logo->active = 0U;
            pCom_logo->substep = 0;
            i += 1;
            pCom_logo++;
        } while ((i & 0xFF) < 9U);
        logo = Com_logo_get();
        pCom_logo = logo;
        logo->behaviour = 0;
        pCom_logo->x = 0;
        pCom_logo->y = 0xFFD50000;
        Xa_cmd(1, 0xB7);
        MAIN.System_flg |= SYS_XA_VOICE;  // set the stream-playing flag that the wordmark hold waits on to clear
        Sce3_logo_state++;
    }
    pCom_logo = Com_logo_wk;
    i = 0;
    do
    {
        if (pCom_logo->active != 0)
        {
            Com_logo_tbl[pCom_logo->behaviour]();
        }
        i += 1;
        pCom_logo++;
    } while ((i & 0xFF) < 9U);
    pCom_logo = Com_logo_wk;
    ScratchPad->Spr.attribute = 0x01000040;
    ScratchPad->Spr.cx = 0;
    i = 0;
    do
    {
        if (pCom_logo->active != 0)
        {
            ScratchPad->Spr.tpage = 0xB;
            ScratchPad->Spr.cy = 0x1ED;
            ScratchPad->Spr.mx = pCom_logo->Pivot_x;
            ScratchPad->Spr.my = pCom_logo->Pivot_y;
            ScratchPad->Spr.u = pCom_logo->Tex_u;
            ScratchPad->Spr.v = pCom_logo->Tex_v;
            ScratchPad->Spr.w = pCom_logo->width;
            ScratchPad->Spr.h = pCom_logo->height;
            ScratchPad->Spr.scalex = pCom_logo->Scale_x;
            ScratchPad->Spr.scaley = pCom_logo->Scale_y;
            ScratchPad->Spr.rotate = pCom_logo->angle << 0xC;
            ScratchPad->Spr.x = pCom_logo->x >> 16;
            ScratchPad->Spr.y = pCom_logo->y >> 16;
            GsSortSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], pCom_logo->Ot_pri);
        }
        i += 1;
        pCom_logo++;
    } while ((i & 0xFF) < 9U);
}

VOID Com_logo_hub_move(VOID)
{
    INT angle;

    Com_logo_hub_tbl[pCom_logo->substep]();
    angle = pCom_logo->angle + 0xF;
    pCom_logo->angle = angle;
    if (angle == 0x168)
    {
        pCom_logo->angle = 0;
    }
}

VOID Com_logo_hub_init(VOID)
{
    pCom_logo->substep = pCom_logo->substep + 1;
    pCom_logo->Tex_u = 0x20;
    pCom_logo->Tex_v = 0x18;
    pCom_logo->width = 0x20;
    pCom_logo->Pivot_x = 0x10;
    pCom_logo->Scale_x = 0x1000;
    pCom_logo->Scale_y = 0x1000;
    pCom_logo->height = 0x28;
    pCom_logo->Ot_pri = 5;
    pCom_logo->Pivot_y = 0x28;
    pCom_logo->angle = 0;

    pCom_logo->timer = 3;
}

VOID Com_logo_hub_spin(VOID)
{
    USHORT timer;
    COM_LOGO_WORK* child;

    if (pCom_logo->angle == 0)
    {
        timer = pCom_logo->timer - 1;
        pCom_logo->timer = timer;
        if ((timer << 0x10) == 0)
        {
            pCom_logo->substep = pCom_logo->substep + 1;
            pCom_logo->timer = 4U;
            Com_logo_hub_spawn();
            child = Com_logo_get();
            child->behaviour = 3;
            child->parent = pCom_logo;
        }
    }
}

VOID Com_logo_hub_spawn(VOID)
{
    INT angle;
    SCHAR behaviour;
    USHORT timer;
    COM_LOGO_WORK* child;

    angle = pCom_logo->angle;
    if (angle == pCom_logo->angle / 90 * 0x5A)
    {
        timer = pCom_logo->timer - 1;
        pCom_logo->timer = timer;
        if ((timer << 0x10) == 0)
        {
            pCom_logo->substep = pCom_logo->substep + 1;
            pCom_logo->timer = 3U;
            pCom_logo->Hub_sts = 0;
            child = Com_logo_get();
            behaviour = 2;
        }
        else
        {
            child = Com_logo_get(angle, pCom_logo);  // passes 2 arguments, Com_logo_get declares 0
            behaviour = 1;
        }
        child->behaviour = behaviour;
        child->x = pCom_logo->x;
        child->y = pCom_logo->y;
        child->angle = pCom_logo->angle;
        child->parent = pCom_logo;
    }
}

VOID Com_logo_hub_hold(VOID)
{
    USHORT timer;

    if (pCom_logo->angle == 0)
    {
        timer = pCom_logo->timer - 1;
        pCom_logo->timer = timer;
        if ((timer << 0x10) == 0)
        {
            pCom_logo->substep = pCom_logo->substep + 1;
            pCom_logo->Hub_sts = 1;
        }
    }
}

VOID Com_logo_move_out(VOID)
{
    INT Min_x;
    INT x;
    x = pCom_logo->x;

    Min_x = 0xFFD60000;

    if (x <= Min_x)
    {
        pCom_logo->substep = pCom_logo->substep + 1;

        pCom_logo->x = -0x2A0000;

        return;
    }

    pCom_logo->x = x + 0xFFFD0000;
    pCom_logo->Scale_x = pCom_logo->Scale_x - 0xD0;
    pCom_logo->Scale_y = pCom_logo->Scale_y - 0xD0;
}

VOID Com_logo_hub_end(VOID)
{
    SCHAR* pStep;
    INT angle;
    if (1)
    {
        angle = pCom_logo->angle;
        if (angle + 0xF == (angle + 0xF) / 90 * 0x5A)
        {
            COM_LOGO_WORK* logo;
            logo = pCom_logo;
            pCom_logo->active = 0;
            pStep = &pCom_logo->substep;
            logo->Hub_sts = 2;
            *pStep = 0;
        }
    }
}

VOID Com_logo_wa_move(VOID)
{
    Com_logo_wa_tbl[pCom_logo->substep]();
}

VOID Com_logo_wa_init(VOID)
{
    pCom_logo->substep = pCom_logo->substep + 1;
    pCom_logo->Tex_u = 0x20;
    pCom_logo->Tex_v = 0x18;
    pCom_logo->height = 0x28;
    pCom_logo->width = 0x20;
    pCom_logo->Pivot_x = 0x10;
    pCom_logo->Scale_x = 0x1000;
    pCom_logo->Scale_y = 0x1000;
    pCom_logo->Pivot_y = 0x28;
    pCom_logo->Ot_pri = 5;
}

VOID Com_logo_wa_seed(VOID)
{
    COM_LOGO_WORK* t;
    UCHAR* d;

    t = pCom_logo->parent;
    if (t->angle == 0x10E)
    {
        pCom_logo->substep++;
        pCom_logo->Hub_sts = 0;
        t = Com_logo_get(t);  // passes 1 argument, Com_logo_get declares 0
        t->behaviour = 2;
        t->x = pCom_logo->x;
        t->y = pCom_logo->y;
        t->angle = pCom_logo->angle;
        t->parent = pCom_logo;
    }
}

VOID Com_logo_wa_spin(VOID)
{
    COM_LOGO_WORK* obj;
    COM_LOGO_WORK* t;
    INT v;

    obj = pCom_logo;
    t = obj->parent;
    v = obj->angle + 0xF;
    obj->angle = v;
    if (v == 0x168)
    {
        obj->angle = 0;
    }
    if (t->Hub_sts == 1)
    {
        pCom_logo->substep++;
        pCom_logo->Hub_sts = 1;
    }
}

VOID Com_logo_spin_out(VOID)
{
    INT angle;

    angle = pCom_logo->angle + 0xF;
    pCom_logo->angle = angle;
    if (angle == 0x168)
    {
        pCom_logo->angle = 0;
    }
    Com_logo_move_out();
}

VOID Com_logo_retire(VOID)
{
    COM_LOGO_WORK* obj;
    COM_LOGO_WORK* t;
    INT v;

    obj = pCom_logo;
    t = obj->parent;
    v = obj->angle + 0xF;
    obj->angle = v;
    if (v == 0x168)
    {
        obj->angle = 0;
    }
    if (t->Hub_sts == 2)
    {
        COM_LOGO_WORK* p = pCom_logo;

        p->active = 0;
        p->Hub_sts = 2;
        pCom_logo->substep = 0;
    }
}

VOID Com_logo_wb_move(VOID)
{
    Com_logo_wb_tbl[pCom_logo->substep]();
}

VOID Com_logo_wb_init(VOID)
{
    pCom_logo->substep = pCom_logo->substep + 1;
    pCom_logo->Tex_u = 0;
    pCom_logo->Tex_v = 0x18;
    pCom_logo->height = 0x28;
    pCom_logo->width = 0x20;
    pCom_logo->Pivot_x = 0x10;
    pCom_logo->Scale_x = 0x1000;
    pCom_logo->Scale_y = 0x1000;
    pCom_logo->Pivot_y = 0x28;
    pCom_logo->Ot_pri = 5;
}

VOID Com_logo_wb_wait(VOID)
{
    if (pCom_logo->parent->angle == pCom_logo->angle + 0x2D)
    {
        pCom_logo->substep = pCom_logo->substep + 1;
    }
}

VOID Com_logo_wb_spin(VOID)
{
    COM_LOGO_WORK* obj;
    COM_LOGO_WORK* t;
    INT v;

    obj = pCom_logo;
    t = obj->parent;
    v = obj->angle + 0xF;
    obj->angle = v;
    if (v == 0x168)
    {
        obj->angle = 0;
    }
    if (t->Hub_sts == 1)
    {
        pCom_logo->substep++;
    }
}

VOID Com_logo_wm_move(VOID)
{
    Com_logo_wm_tbl[pCom_logo->substep]();
}

VOID Com_logo_wm_init(VOID)
{
    pCom_logo->substep = pCom_logo->substep + 1;
    pCom_logo->Tex_u = 0x18;
    pCom_logo->Tex_v = 0;
    pCom_logo->height = 0x18;
    pCom_logo->Scale_x = 0x1000;
    pCom_logo->Scale_y = 0x1000;
    pCom_logo->x = 0x3B0000;
    pCom_logo->y = 0xFFC90000;
    pCom_logo->width = 0;
    pCom_logo->Pivot_x = 0;
    pCom_logo->Pivot_y = 0;
    pCom_logo->angle = 0;
    pCom_logo->Ot_pri = 4;
}

VOID Com_logo_wm_wipe(VOID)
{
    INT x;
    INT Wipe_x;

    x = pCom_logo->x + 0xFFFEE000;
    pCom_logo->x = x;
    Wipe_x = x >> 0x10;
    pCom_logo->width = 0x3B - Wipe_x;
    if (Wipe_x < -0x1E)
    {
        pCom_logo->substep = pCom_logo->substep + 1;
        pCom_logo->x = -0x1E0000;
    }
}

VOID Com_logo_wm_wait(VOID)
{
    if (pCom_logo->parent->Hub_sts == 2)
    {
        pCom_logo->substep = pCom_logo->substep + 1;
        pCom_logo->x = 0xFFCA0000;
        pCom_logo->width = 0x70;
        pCom_logo->Tex_u = 0;

        pCom_logo->timer = 0x5A;
    }
}

VOID Com_logo_wm_hold(VOID)
{
    SHORT timer;
    SHORT left;

    timer = pCom_logo->timer;
    left = timer - (timer > 0);
    pCom_logo->timer = left;
    if ((left << 0x10) <= 0 && !(G.System_flg & SYS_XA_VOICE))
    {
        pCom_logo->substep = pCom_logo->substep + 1;
        pCom_logo->width = 0x70;
        pCom_logo->x = 0x20000;
        pCom_logo->y = pCom_logo->y + 0xC0000;
        pCom_logo->Tex_u = 0;
        pCom_logo->Pivot_x = 0x38;
        pCom_logo->Pivot_y = 0xC;
        pCom_logo->Scale_x = 0x1000;
        pCom_logo->Scale_y = 0x1000;
    }
}

VOID Com_logo_wm_shrink(VOID)
{
    COM_LOGO_WORK* p;
    SHORT x;
    USHORT y;
    INT z;

    p = pCom_logo;
    if (p->Scale_x >= 0xAAB)
    {
        p->Scale_x -= 0x32;
        p->Scale_y -= 0x32;
    }
    else
    {
        p->Scale_x = 0xAAA;
        p->Scale_y = 0xAAA;
    }
    z = pCom_logo->y + 0x10000;
    pCom_logo->y = z;
    if ((z >> 0x10) >= -0x12)
    {
        pCom_logo->y = -0x120000;
        pCom_logo->active = 0;
        Sce3_logo_state = 2;
    }
}

COM_LOGO_WORK* Com_logo_get(VOID)
{
    UINT one;
    COM_LOGO_WORK* slot;
    INT i;
    slot = Com_logo_wk;
    i = 0;
    one = 1U;
loop_1:
    if (slot->active != 0)
    {
        i += 1;
        slot++;
        if ((i & 0xFF) >= 9U)
        {
            return 0;
        }
        goto loop_1;
    }

    slot->active = one;
    slot->substep = 0;
    return slot;
}

VOID Sce_slide_disp(VOID)
{
    if (G.Stop_flg & STOP_SCE)
    {
        Com_slide_tbl[Sce_panel_work.Rno]();
    }
}

#define SLIDE VIEW(PANEL_SLIDE_WORK, Sce_panel_work)  // the lab slide record: the same memory as Sce_panel_work, with Timer and Cnt as byte pairs

VOID Com_slide_start(VOID)
{
    UCHAR cut;

    USHORT* Proc_flg = &MAIN.Stop_flg;  // room update flags

    Sce_panel_work.Rno = 1;
    Sce_panel_work.Sub_rno = 0;
    G.Vloop = 0;

    *Proc_flg &= 0xFEFF;

    cut = CUT_NO_CUR;
    MAIN.SaveData.Cut_no = 4;
    G.SaveData.Cut_bak = cut;
    Cut_change();
    SLIDE.Slide_no = 0;
    SLIDE.Slide_pass = 0;
    SLIDE.Mess_no = 0;
    SLIDE.Wait_cnt = 0x10;
    Snd_bgm_stop(BGM_MAIN);
    Snd_bgm_play(BGM_SUB0);
    Snd_bgm_vol_set(BGM_SUB1, 0x14, 0x14);
    Snd_bgm_play(BGM_SUB1);
    Sce_panel_work.Cursor_x = -0x60;
    Sce_panel_work.Cursor_y = -0x42;
}

VOID Com_slide_main(VOID)
{
    INT Scrl_in;
    INT Scrl_out;

    switch (Sce_panel_work.Sub_rno)
    {
    case 0:
        SLIDE.Wait_cnt--;
        if (SLIDE.Wait_cnt == 0)
        {
            Sce_panel_work.Sub_rno = 1;
            Sce_panel_work.Cursor_x = 0x66;
        case 1:
            Scrl_in = Sce_panel_work.Cursor_x - 0x12;
            Sce_panel_work.Cursor_x = Scrl_in;
            if (Scrl_in == 0x1E)
            {
                Snd_se_play(SE_BANK_ROOM, 0x1B, 0);
            }
            if (Sce_panel_work.Cursor_x == -0x60)
            {
                Sce_panel_work.Sub_rno = 2;
                Mess_set(Sce_panel_work.Cnt, 0);
            }
        }
        break;
    case 2:
        if (!(G.SaveData.Mess_flg & MESS_FLG_ACTIVE))
        {
            Sce_panel_work.Sub_rno = 3;
            Snd_se_play(SE_BANK_ROOM, 0x1A, 0);
        }
        break;
    case 3:
        Scrl_out = Sce_panel_work.Cursor_x - 0x12;
        Sce_panel_work.Cursor_x = Scrl_out;
        if (Scrl_out < -0x127)
        {
            Sce_panel_work.Sub_rno = 0;
            SLIDE.Wait_cnt = 0x10;
            if (SLIDE.Slide_no < 5U)
            {
                SLIDE.Slide_no += 1;
            }
            else if (SLIDE.Slide_pass != 0)
            {
                Sce_panel_work.Rno = 2;
            }
            else
            {
                SLIDE.Slide_pass = 1;
            }
            VIEW(UCHAR, SLIDE.Mess_no) += 1;  // UCHAR view of the field (plain access changes the code)
        }
        break;
    }
    Com_slide_draw();
}

VOID Com_slide_draw(VOID)
{
    COM_SLIDE_REC* p;
    UINT t;

    ScratchPad->Rect.attribute = 0x60000000;
    p = Com_slide_bar_tbl + 4;
    ScratchPad->Rect.r = 0x18;
    ScratchPad->Rect.g = 0x18;
    ScratchPad->Rect.b = 0x18;
    do
    {
        ScratchPad->Rect.h = p[-1].h;
        ScratchPad->Rect.w = p[-1].w;
        ScratchPad->Rect.y = p[-1].y;
        ScratchPad->Rect.x = p[-1].x;
        p--;
        GsSortBoxFill(&ScratchPad->Rect, MAIN.Ot0 + G.Cbuf_id, 4);
    } while (Com_slide_bar_tbl < p);
    if (Sce_panel_work.Sub_rno != 0)
    {
        ScratchPad->Rect.y = -0x42;
        ScratchPad->Rect.h = 0x7F;
        ScratchPad->Spr.u = 0;

        if ((UINT)(Sce_panel_work.Cursor_x + 0x67) < 0xE)  // real width or sign conversion
        {
            ScratchPad->Rect.w = 0xBF;

            ScratchPad->Rect.x = Sce_panel_work.Cursor_x;
        }
        else if (Sce_panel_work.Sub_rno != 1)
        {
            ScratchPad->Rect.x = -0x67;

            ScratchPad->Rect.w = Sce_panel_work.Cursor_x + 0x126;
            ScratchPad->Spr.u = -0x67 - Sce_panel_work.Cursor_x;
        }
        else
        {
            ScratchPad->Rect.x = Sce_panel_work.Cursor_x;
            ScratchPad->Rect.w = 0x66 - Sce_panel_work.Cursor_x;
        }
        if (SLIDE.Slide_pass != 0 || VIEW(UCHAR, SLIDE.Slide_no) != 5)  // UCHAR view of the field (plain access changes the code)
        {
            ScratchPad->Spr.y = ScratchPad->Rect.y;
            ScratchPad->Rect.r = 0x80;
            ScratchPad->Rect.g = 0x80;
            ScratchPad->Rect.b = 0x80;
            ScratchPad->Spr.attribute = 0x41000040;
            t = (SLIDE.Slide_no >> 1) * 0xC0;
            ScratchPad->Spr.x = ScratchPad->Rect.x;
            ScratchPad->Spr.u += t & 0x7F;
            ScratchPad->Spr.v = (SLIDE.Slide_no & 1) << 7;
            ScratchPad->Spr.w = ScratchPad->Rect.w;
            ScratchPad->Spr.h = ScratchPad->Rect.h;
            ScratchPad->Spr.cx = 0;
            ScratchPad->Spr.cy = SLIDE.Slide_no + 0x1ED;
            ScratchPad->Spr.tpage = (t >> 7) + 9;
            GsSortFlipSprite(&ScratchPad->Spr, MAIN.Ot0 + G.Cbuf_id, 4);
        }
        else
        {
            ScratchPad->Rect.r = 0x40;
            ScratchPad->Rect.g = 0x40;
            ScratchPad->Rect.b = 0x40;
        }
        GsSortBoxFill(&ScratchPad->Rect, MAIN.Ot0 + G.Cbuf_id, 4);
    }
}

VOID Com_slide_exit(VOID)
{
    G.SaveData.Cut_no = G.SaveData.Cut_bak;
    Cut_change();
    Flg_off(&MAIN.SaveData.Scenario_flg, 0x20);  // clear the screen-active flag; address as integer
    Snd_bgm_stop(BGM_SUB0);
    Snd_bgm_stop(BGM_SUB1);
    Snd_bgm_play(BGM_MAIN);
    G.Vloop = 2;
    STOP_FLG_CUR |= STOP_PAD_LOCK;  // restore the pad flag

    MAIN.System_flg &= ~SYS_DIM;
}

VOID Bg_img_buf_set(VOID* Buf_a, VOID* Buf_b, INT mode)
{
    G.pBg_buf_a = Buf_a;
    G.pBg_buf_b = Buf_b;
    G.Bg_img_mode = mode;
}

VOID Bg_img_trans(SHORT y)
{
    RECT Dst_rect;
    UINT* pPix;

    pPix = (UINT*)LOAD_BUF;  // pointer type conversion (no code)
    if (G.Bg_img_mode == 1)
    {
        pPix = (UINT*)LOAD_PIX_BUF;  // pointer type conversion (no code)
    }
    Dst_rect.w = 0x140;
    Dst_rect.y = y;
    Dst_rect.x = 0;
    Dst_rect.h = 0xF0;
    LoadImage(&Dst_rect, (ULONG*)pPix);  // pointer type conversion (no code)
}

// lights applied to the room while the terminal is in use
LIGHT_CUT_DATA Com_light_tbl[3] =
{
    { { 14300, -1950, 9650, 120, 120, 120 }, 0, 6735 },   // 0 //
    { { 4450, -2650, 12000, 120, 120, 120 }, 0, 7055 },   // 1 //
    { { 14700, -1600, 5600, 120, 120, 120 }, 0, 7495 },   // 2 //
};

// row descriptors of the login window (texture u/v, width, height, texture page)
COM_ROW_REC Com_login_row_tbl[3] =
{
    { 0, { 0x00, 0x00, 0x00 }, 144, { 0x00, 0x00, 0x00 }, 32, 2, 9, 0 },   // 0 //
    { 0, { 0x00, 0x00, 0x00 }, 224, { 0x00, 0x00, 0x00 }, 7, 1, 7, 0 },    // 1 //
    { 0, { 0x00, 0x00, 0x00 }, 240, { 0x00, 0x00, 0x00 }, 0, 1, 7, 0 },    // 2 //
};

// expected user name as key codes (1 = first letter key), zero padded
UCHAR Com_user_tbl[0x8] =
{
    0x0A,   // 0 //
    0x0F,   // 1 //
    0x08,   // 2 //
    0x0E,   // 3 //
    0x00,   // 4 //
    0x00,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
};

// expected password as key codes, zero padded
UCHAR Com_pass_tbl[0x4] =
{
    0x01,   // 0 //
    0x04,   // 1 //
    0x01,   // 2 //
    0x00,   // 3 //
};

// row descriptors of the menu panel window (3 rows of COM_WND_ROW_REC)
UCHAR Com_pa_row_tbl[0x30] =
{
    0x00,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x00,   // 3 //
    0xB0,   // 4 //
    0x00,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x20,   // 8 //
    0x00,   // 9 //
    0x01,   // 10 //
    0x00,   // 11 //
    0x09,   // 12 //
    0x00,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0x00,   // 16 //
    0x00,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
    0x00,   // 20 //
    0x00,   // 21 //
    0x00,   // 22 //
    0x00,   // 23 //
    0x00,   // 24 //
    0x00,   // 25 //
    0x00,   // 26 //
    0x00,   // 27 //
    0x07,   // 28 //
    0x00,   // 29 //
    0x00,   // 30 //
    0x00,   // 31 //
    0x60,   // 32 //
    0x00,   // 33 //
    0x00,   // 34 //
    0x00,   // 35 //
    0xF0,   // 36 //
    0x00,   // 37 //
    0x00,   // 38 //
    0x00,   // 39 //
    0x0D,   // 40 //
    0x00,   // 41 //
    0x01,   // 42 //
    0x00,   // 43 //
    0x07,   // 44 //
    0x00,   // 45 //
    0x00,   // 46 //
    0x00,   // 47 //
};

// row descriptors of the menu side window (3 rows of COM_WND_ROW_REC)
UCHAR Com_side_row_tbl[0x30] =
{
    0x38,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x00,   // 3 //
    0xE0,   // 4 //
    0x00,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x03,   // 8 //
    0x00,   // 9 //
    0x01,   // 10 //
    0x00,   // 11 //
    0x07,   // 12 //
    0x00,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0x50,   // 16 //
    0x00,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
    0xE0,   // 20 //
    0x00,   // 21 //
    0x00,   // 22 //
    0x00,   // 23 //
    0x03,   // 24 //
    0x00,   // 25 //
    0x01,   // 26 //
    0x00,   // 27 //
    0x07,   // 28 //
    0x00,   // 29 //
    0x00,   // 30 //
    0x00,   // 31 //
    0xC8,   // 32 //
    0x00,   // 33 //
    0x00,   // 34 //
    0x00,   // 35 //
    0xE0,   // 36 //
    0x00,   // 37 //
    0x00,   // 38 //
    0x00,   // 39 //
    0x07,   // 40 //
    0x00,   // 41 //
    0x01,   // 42 //
    0x00,   // 43 //
    0x07,   // 44 //
    0x00,   // 45 //
    0x00,   // 46 //
    0x00,   // 47 //
};

// row descriptors of the confirmation window (4 rows of COM_WND_ROW_REC)
UCHAR Com_menu_row_tbl[0x40] =
{
    0xC8,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x00,   // 3 //
    0xF0,   // 4 //
    0x00,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x06,   // 8 //
    0x00,   // 9 //
    0x01,   // 10 //
    0x00,   // 11 //
    0x07,   // 12 //
    0x00,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0x00,   // 16 //
    0x00,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
    0x00,   // 20 //
    0x00,   // 21 //
    0x00,   // 22 //
    0x00,   // 23 //
    0x00,   // 24 //
    0x00,   // 25 //
    0x00,   // 26 //
    0x00,   // 27 //
    0x07,   // 28 //
    0x00,   // 29 //
    0x00,   // 30 //
    0x00,   // 31 //
    0xE0,   // 32 //
    0x00,   // 33 //
    0x00,   // 34 //
    0x00,   // 35 //
    0x68,   // 36 //
    0x00,   // 37 //
    0x00,   // 38 //
    0x00,   // 39 //
    0x04,   // 40 //
    0x00,   // 41 //
    0x01,   // 42 //
    0x00,   // 43 //
    0x07,   // 44 //
    0x00,   // 45 //
    0x00,   // 46 //
    0x00,   // 47 //
    0xE0,   // 48 //
    0x00,   // 49 //
    0x00,   // 50 //
    0x00,   // 51 //
    0x78,   // 52 //
    0x00,   // 53 //
    0x00,   // 54 //
    0x00,   // 55 //
    0x04,   // 56 //
    0x00,   // 57 //
    0x01,   // 58 //
    0x00,   // 59 //
    0x07,   // 60 //
    0x00,   // 61 //
    0x00,   // 62 //
    0x00,   // 63 //
};

// row descriptors of the command window (4 rows of COM_WND_ROW_REC)
UCHAR Com_pb_row_tbl[0x40] =
{
    0x00,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x00,   // 3 //
    0xC0,   // 4 //
    0x00,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x15,   // 8 //
    0x00,   // 9 //
    0x01,   // 10 //
    0x00,   // 11 //
    0x09,   // 12 //
    0x00,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0x00,   // 16 //
    0x00,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
    0xD0,   // 20 //
    0x00,   // 21 //
    0x00,   // 22 //
    0x00,   // 23 //
    0x15,   // 24 //
    0x00,   // 25 //
    0x01,   // 26 //
    0x00,   // 27 //
    0x09,   // 28 //
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
    0x00,   // 39 //
    0x00,   // 40 //
    0x00,   // 41 //
    0x00,   // 42 //
    0x00,   // 43 //
    0x07,   // 44 //
    0x00,   // 45 //
    0x00,   // 46 //
    0x00,   // 47 //
    0x00,   // 48 //
    0x00,   // 49 //
    0x00,   // 50 //
    0x00,   // 51 //
    0xF0,   // 52 //
    0x00,   // 53 //
    0x00,   // 54 //
    0x00,   // 55 //
    0x0C,   // 56 //
    0x00,   // 57 //
    0x01,   // 58 //
    0x00,   // 59 //
    0x07,   // 60 //
    0x00,   // 61 //
    0x00,   // 62 //
    0x00,   // 63 //
};

// expected release command as key codes, zero padded
UCHAR Com_release_tbl[0x8] =
{
    0x0D,   // 0 //
    0x0F,   // 1 //
    0x0C,   // 2 //
    0x05,   // 3 //
    0x00,   // 4 //
    0x00,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
};

// row descriptors of the unlock prompt window (1 row of COM_WND_ROW_REC)
UCHAR Com_unlock_row_tbl[0x10] =
{
    0xA8,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x00,   // 3 //
    0xD0,   // 4 //
    0x00,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x07,   // 8 //
    0x00,   // 9 //
    0x01,   // 10 //
    0x00,   // 11 //
    0x09,   // 12 //
    0x00,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
};

// quad descriptor of the unlock prompt text (1 COM_QUAD_REC)
COM_QUAD_REC Com_unlock_quad_tbl[1] =
{
    { 0x0048, 0x0010, 0x08, 0xC0, 0x0018, 0x0010, 0x00, 0x00 },   // 0 //
};

// arm object animation per key code: bit 7 selects the arm, bits 0-6 the animation
UCHAR Com_arm_tbl[0x20] =
{
    0x83,   // 0 //
    0x82,   // 1 //
    0x81,   // 2 //
    0x01,   // 3 //
    0x02,   // 4 //
    0x02,   // 5 //
    0x02,   // 6 //
    0x03,   // 7 //
    0x83,   // 8 //
    0x82,   // 9 //
    0x81,   // 10 //
    0x01,   // 11 //
    0x01,   // 12 //
    0x02,   // 13 //
    0x02,   // 14 //
    0x03,   // 15 //
    0x83,   // 16 //
    0x82,   // 17 //
    0x81,   // 18 //
    0x01,   // 19 //
    0x01,   // 20 //
    0x01,   // 21 //
    0x02,   // 22 //
    0x03,   // 23 //
    0x83,   // 24 //
    0x82,   // 25 //
    0x81,   // 26 //
    0x01,   // 27 //
    0x01,   // 28 //
    0x01,   // 29 //
    0x03,   // 30 //
    0x03,   // 31 //
};

// quad descriptors of the window frame (Vtx, edges, fill)
COM_QUAD_REC Com_frm_tbl[8] =
{
    { 0x0000, 0x0000, 0x50, 0x20, 0x0010, 0x0010, 0x00, 0x00 },   // 0 //
    { 0x0000, 0x0000, 0x70, 0x20, 0x0010, 0x0010, 0x00, 0x00 },   // 1 //
    { 0x0000, 0x0000, 0x50, 0x40, 0x0010, 0x0010, 0x00, 0x00 },   // 2 //
    { 0x0000, 0x0000, 0x70, 0x40, 0x0010, 0x0010, 0x00, 0x00 },   // 3 //
    { 0x0010, 0x0000, 0x60, 0x20, 0x0010, 0x0010, 0x00, 0x00 },   // 4 //
    { 0x0010, 0x0000, 0x60, 0x40, 0x0010, 0x0010, 0x00, 0x00 },   // 5 //
    { 0x0000, 0x0010, 0x50, 0x30, 0x0010, 0x0010, 0x01, 0x00 },   // 6 //
    { 0x0000, 0x0010, 0x70, 0x30, 0x0010, 0x0010, 0x01, 0x00 },   // 7 //
};

// quad descriptors of the on-screen keyboard frame (9 COM_QUAD_REC)
COM_QUAD_REC Com_key_frm[9] =
{
    { 0x0010, 0x0010, 0x10, 0x20, 0x00C0, 0x0060, 0x00, 0x00 },   // 0 //
    { 0x0008, 0x0008, 0x00, 0x48, 0x0010, 0x0008, 0x00, 0x0C },   // 1 //
    { 0x00C8, 0x0008, 0x00, 0x48, 0x0008, 0x0008, 0x00, 0x00 },   // 2 //
    { 0x0008, 0x0010, 0x08, 0x50, 0x0008, 0x0010, 0x00, 0x00 },   // 3 //
    { 0x0008, 0x0020, 0x08, 0x60, 0x0008, 0x0010, 0x01, 0x05 },   // 4 //
    { 0x0008, 0x0070, 0x08, 0x70, 0x0008, 0x0008, 0x00, 0x00 },   // 5 //
    { 0x0010, 0x0070, 0x20, 0x80, 0x0010, 0x0008, 0x00, 0x0B },   // 6 //
    { 0x00C0, 0x0070, 0x30, 0x80, 0x0018, 0x0008, 0x00, 0x00 },   // 7 //
    { 0x00D0, 0x0008, 0x50, 0x80, 0x0008, 0x0010, 0x01, 0x06 },   // 8 //
};

// quad descriptors of the message pictures, indexed by message number (3 COM_QUAD_REC)
COM_QUAD_REC Com_msg_pic_tbl[3] =
{
    { 0x00D0, 0x0068, 0x50, 0x80, 0x0008, 0x0008, 0x00, 0x00 },   // 0 //
    { 0x0000, 0x0000, 0x00, 0x00, 0x0078, 0x0068, 0x00, 0x00 },   // 1 //
    { 0x0000, 0x0000, 0x78, 0x00, 0x0078, 0x0068, 0x00, 0x00 },   // 2 //
};

// quad descriptors of the status banners, indexed by Com_sts_no (8 COM_QUAD_REC)
COM_QUAD_REC Com_sts_tbl[8] =
{
    { 0x0000, 0x0000, 0x00, 0x68, 0x0078, 0x0068, 0x00, 0x00 },   // 0 //
    { 0x0000, 0x0000, 0x78, 0x68, 0x0064, 0x0020, 0x00, 0x00 },   // 1 //
    { 0x0000, 0x0000, 0x78, 0x88, 0x0064, 0x0020, 0x00, 0x00 },   // 2 //
    { 0x0000, 0x0000, 0x78, 0xA8, 0x0064, 0x0020, 0x00, 0x00 },   // 3 //
    { 0x0000, 0x0000, 0x78, 0xC8, 0x0064, 0x0018, 0x00, 0x00 },   // 4 //
    { 0x0000, 0x0018, 0x78, 0xA0, 0x0064, 0x0008, 0x00, 0x00 },   // 5 //
    { 0x000C, 0x0016, 0x84, 0x24, 0x002A, 0x0004, 0x00, 0x00 },   // 6 //
    { 0x000C, 0x0012, 0x84, 0x20, 0x002A, 0x0004, 0x00, 0x00 },   // 7 //
};

// quad descriptors of message strip A (2 COM_QUAD_REC)
COM_QUAD_REC Com_strip_a_tbl[2] =
{
    { 0x000C, 0x001D, 0x84, 0x17, 0x002A, 0x0003, 0x00, 0x00 },   // 0 //
    { 0x000C, 0x0020, 0x84, 0x14, 0x0014, 0x0003, 0x00, 0x00 },   // 1 //
};

// quad descriptors of message strip B (3 COM_QUAD_REC)
COM_QUAD_REC Com_strip_b_tbl[3] =
{
    { 0x000C, 0x0027, 0x84, 0x17, 0x001E, 0x0003, 0x00, 0x00 },   // 0 //
    { 0x000C, 0x002A, 0x8A, 0x17, 0x001E, 0x0003, 0x00, 0x00 },   // 1 //
    { 0x000C, 0x002F, 0x84, 0x14, 0x0010, 0x0003, 0x00, 0x00 },   // 2 //
};

// door 0 map: x and y origin, then quad count
UCHAR Com_door_ofs0[0x4] =
{
    0x32,   // 0 //
    0x14,   // 1 //
    0x09,   // 2 //
    0x04,   // 3 //
};

// door 0 map quads: the blinking marker first, then the map (10 COM_QUAD_REC)
COM_QUAD_REC Com_door_mark0[10] =
{
    { 0x0000, 0x0008, 0x68, 0xE0, 0x0014, 0x0008, 0x01, 0x03 },   // 0 //
    { 0x0000, 0x0000, 0x60, 0xC0, 0x0010, 0x0018, 0x00, 0x00 },   // 1 //
    { 0x0020, 0x0000, 0x60, 0xC0, 0x0010, 0x0018, 0x80, 0x00 },   // 2 //
    { 0x0010, 0x0000, 0x60, 0xC0, 0x0010, 0x0001, 0x00, 0x00 },   // 3 //
    { 0x0000, 0x0010, 0x60, 0xC0, 0x0010, 0x0011, 0x40, 0x00 },   // 4 //
    { 0x0020, 0x0010, 0x60, 0xC0, 0x0010, 0x0011, 0xC0, 0x00 },   // 5 //
    { 0x0010, 0x0020, 0x60, 0xC0, 0x0010, 0x0001, 0x00, 0x00 },   // 6 //
    { 0x0000, 0x0008, 0x60, 0xC0, 0x0004, 0x0004, 0x00, 0x00 },   // 7 //
    { 0x0004, 0x0008, 0x60, 0xC0, 0x0010, 0x0018, 0x80, 0x00 },   // 8 //
    { 0x0020, 0x0000, 0x60, 0xC0, 0x0010, 0x0018, 0x40, 0x00 },   // 9 //
};

// door 1 map: x and y origin, then quad count
UCHAR Com_door_ofs1[0x4] =
{
    0x1C,   // 0 //
    0x08,   // 1 //
    0x1C,   // 2 //
    0x06,   // 3 //
};

// door 1 map quads: the blinking marker first, then the map (30 COM_QUAD_REC)
COM_QUAD_REC Com_door_mark1[30] =
{
    { 0x000C, 0x0004, 0x68, 0xE0, 0x000C, 0x000C, 0x00, 0x00 },   // 0 //
    { 0x0000, 0x0000, 0x60, 0xC0, 0x000D, 0x0014, 0x00, 0x00 },   // 1 //
    { 0x000C, 0x0001, 0x60, 0xC0, 0x000D, 0x0004, 0x40, 0x00 },   // 2 //
    { 0x0018, 0x0000, 0x60, 0xC0, 0x0005, 0x0004, 0x00, 0x00 },   // 3 //
    { 0x001C, 0x0001, 0x60, 0xC0, 0x0015, 0x0004, 0x40, 0x00 },   // 4 //
    { 0x0030, 0x0005, 0x60, 0xC0, 0x0015, 0x0010, 0x40, 0x00 },   // 5 //
    { 0x003C, 0x0014, 0x60, 0xC0, 0x0009, 0x0015, 0xC0, 0x00 },   // 6 //
    { 0x003C, 0x0029, 0x60, 0xC0, 0x0008, 0x000C, 0x40, 0x00 },   // 7 //
    { 0x003C, 0x0035, 0x60, 0xC0, 0x0008, 0x0018, 0xC0, 0x00 },   // 8 //
    { 0x003C, 0x004D, 0x60, 0xC0, 0x0008, 0x000C, 0xC0, 0x00 },   // 9 //
    { 0x0030, 0x004D, 0x60, 0xC0, 0x000C, 0x000C, 0x40, 0x00 },   // 10 //
    { 0x0030, 0x0035, 0x60, 0xC0, 0x000C, 0x0018, 0x40, 0x00 },   // 11 //
    { 0x0030, 0x0029, 0x60, 0xC0, 0x0009, 0x000C, 0xC0, 0x00 },   // 12 //
    { 0x0038, 0x0034, 0x60, 0xC0, 0x0004, 0x0001, 0x00, 0x00 },   // 13 //
    { 0x0030, 0x0015, 0x60, 0xC0, 0x0009, 0x0014, 0x40, 0x00 },   // 14 //
    { 0x0018, 0x0021, 0x60, 0xC0, 0x0018, 0x0008, 0x40, 0x00 },   // 15 //
    { 0x000C, 0x0015, 0x60, 0xC0, 0x000D, 0x000C, 0x40, 0x00 },   // 16 //
    { 0x0000, 0x0014, 0x60, 0xC0, 0x000D, 0x0001, 0x00, 0x00 },   // 17 //
    { 0x0004, 0x0004, 0x60, 0xC0, 0x0008, 0x0011, 0x00, 0x00 },   // 18 //
    { 0x0005, 0x0005, 0x60, 0xC0, 0x0008, 0x000C, 0xC0, 0x00 },   // 19 //
    { 0x000D, 0x0005, 0x60, 0xC0, 0x000C, 0x000C, 0xC0, 0x00 },   // 20 //
    { 0x0018, 0x0010, 0x60, 0xC0, 0x0001, 0x0010, 0x00, 0x00 },   // 21 //
    { 0x000C, 0x0014, 0x60, 0xC0, 0x000C, 0x0001, 0x00, 0x00 },   // 22 //
    { 0x001C, 0x0005, 0x60, 0xC0, 0x0014, 0x000C, 0x40, 0x00 },   // 23 //
    { 0x001C, 0x0014, 0x60, 0xC0, 0x0010, 0x000D, 0x00, 0x00 },   // 24 //
    { 0x0024, 0x0015, 0x60, 0xC0, 0x0008, 0x0010, 0xC0, 0x00 },   // 25 //
    { 0x001D, 0x0020, 0x60, 0xC0, 0x0008, 0x0004, 0x80, 0x00 },   // 26 //
    { 0x0031, 0x0015, 0x60, 0xC0, 0x0008, 0x0010, 0xC0, 0x00 },   // 27 //
    { 0x003C, 0x001C, 0x60, 0xC0, 0x0008, 0x000C, 0x00, 0x00 },   // 28 //
    { 0xFFFE, 0xFFFE, 0x70, 0xD0, 0x0010, 0x0010, 0x00, 0x05 },   // 29 //
};

// frame quads of the door picture
COM_QUAD_REC Com_door_frm_tbl[4] =
{
    { 0x004E, 0xFFFC, 0x41, 0x12, 0x0002, 0x0020, 0x01, 0x03 },   // 0 //
    { 0xFFFE, 0xFFFC, 0x01, 0x10, 0x0029, 0x0002, 0x00, 0x02 },   // 1 //
    { 0xFFFC, 0xFFFC, 0x01, 0x10, 0x0002, 0x0020, 0x01, 0x03 },   // 2 //
    { 0xFFFC, 0x005C, 0x01, 0x41, 0x002A, 0x0002, 0x00, 0x02 },   // 3 //
};

// quads of the door picture, indexed by door
COM_QUAD_REC Com_door_pic_tbl[3] =
{
    { 0x0000, 0x0000, 0x50, 0x50, 0x0020, 0x0018, 0x00, 0x00 },   // 0 //
    { 0x0000, 0x0000, 0x50, 0x50, 0x0010, 0x0018, 0x00, 0x00 },   // 1 //
    { 0x0010, 0x0000, 0x70, 0x50, 0x0010, 0x0018, 0x00, 0x00 },   // 2 //
};

// screen state handlers: init, update, finish
COM_FUNC Com_lab_tbl[3] =
{
    Com_lab_init,     // 0 //
    Com_lab_main,   // 1 //
    Com_lab_end,   // 2 //
};

// stage handlers: boot, login, menu, command, unlocked
COM_FUNC Com_stage_tbl[5] =
{
    Com_stage_boot,       // 0 //
    Com_stage_login,      // 1 //
    Com_stage_menu,       // 2 //
    Com_stage_cmd,    // 3 //
    Com_stage_unlock,   // 4 //
};

// monitor turn-on substeps
COM_FUNC Com_boot_tbl[4] =
{
    Com_boot_zoom_start,   // 0 //
    Com_boot_zoom_wide,    // 1 //
    Com_boot_zoom_tall,    // 2 //
    Com_boot_flash,        // 3 //
};

// login steps: init, input, check
COM_FUNC Com_login_tbl[3] =
{
    Com_login_init,          // 0 //
    Com_login_in_sub,   // 1 //
    Com_login_ck_sub,   // 2 //
};

// login input substeps
COM_FUNC Com_login_in_tbl[6] =
{
    Com_login_open_wait,     // 0 //
    Com_login_key,        // 1 //
    Com_login_pass,   // 2 //
    Com_login_key,        // 3 //
    Com_login_close,        // 4 //
    Com_login_close_wait,    // 5 //
};

// login check substeps
COM_FUNC Com_login_ck_tbl[5] =
{
    Com_login_type_start,   // 0 //
    Com_login_type_next,   // 1 //
    Com_login_ck,    // 2 //
    Com_login_fail,    // 3 //
    Com_login_ok,    // 4 //
};

// menu substeps
COM_FUNC Com_menu_tbl[9] =
{
    Com_menu_open,           // 0 //
    Com_menu_sel,         // 1 //
    Com_menu_cmd,     // 2 //
    Com_menu_exit,   // 3 //
    Com_menu_close,          // 4 //
    Com_menu_close_wait,     // 5 //
    Com_menu_arm_wait,     // 6 //
    Com_menu_blink,          // 7 //
    Com_menu_cnt,      // 8 //
};

// command steps: setup, wait for input, handle the command
COM_FUNC Com_cmd_tbl[3] =
{
    Com_cmd_set,        // 0 //
    Com_cmd_input_wait,   // 1 //
    Com_cmd_sub,        // 2 //
};

// command substeps
COM_FUNC Com_cmd_sub_tbl[6] =
{
    Com_cmd_close,   // 0 //
    Com_cmd_close_end,     // 1 //
    Com_cmd_type_wait,     // 2 //
    Com_cmd_ck,        // 3 //
    Com_cmd_fail,        // 4 //
    Com_cmd_ok,        // 5 //
};

// unlock stage steps
COM_FUNC Com_unlock_tbl[3] =
{
    Com_cmd_unlock,          // 0 //
    Com_cmd_unlock_hold,     // 1 //
    Com_cmd_unlock_msg,   // 2 //
};

// shutdown steps
COM_FUNC Com_end_tbl[3] =
{
    Com_end_start,      // 0 //
    Com_end_shrink_v,   // 1 //
    Com_end_shrink_h,   // 2 //
};

// auto-typer states
COM_FUNC Com_typer_tbl[4] =
{
    Com_typer_start,    // 0 //
    Com_typer_key,    // 1 //
    Com_typer_wait,     // 2 //
    Com_typer_end,   // 3 //
};

// logo slot behaviours: hub, wedge A, wedge B, wordmark
COM_FUNC Com_logo_tbl[4] =
{
    Com_logo_hub_move,        // 0 //
    Com_logo_wa_move,    // 1 //
    Com_logo_wb_move,    // 2 //
    Com_logo_wm_move,   // 3 //
};

// hub substeps
COM_FUNC Com_logo_hub_tbl[6] =
{
    Com_logo_hub_init,      // 0 //
    Com_logo_hub_spin,   // 1 //
    Com_logo_hub_spawn,     // 2 //
    Com_logo_hub_hold,      // 3 //
    Com_logo_move_out,      // 4 //
    Com_logo_hub_end,       // 5 //
};

// wedge A substeps
COM_FUNC Com_logo_wa_tbl[5] =
{
    Com_logo_wa_init,    // 0 //
    Com_logo_wa_seed,    // 1 //
    Com_logo_wa_spin,    // 2 //
    Com_logo_spin_out,   // 3 //
    Com_logo_retire,    // 4 //
};

// wedge B substeps
COM_FUNC Com_logo_wb_tbl[5] =
{
    Com_logo_wb_init,       // 0 //
    Com_logo_wb_wait,   // 1 //
    Com_logo_wb_spin,       // 2 //
    Com_logo_spin_out,      // 3 //
    Com_logo_retire,       // 4 //
};

// wordmark substeps
COM_FUNC Com_logo_wm_tbl[5] =
{
    Com_logo_wm_init,        // 0 //
    Com_logo_wm_wipe,     // 1 //
    Com_logo_wm_wait,   // 2 //
    Com_logo_wm_hold,        // 3 //
    Com_logo_wm_shrink,      // 4 //
};

// slide show state handlers: start, update, finish
COM_FUNC Com_slide_tbl[3] =
{
    Com_slide_start,    // 0 //
    Com_slide_main,   // 1 //
    Com_slide_exit,   // 2 //
};

// frame bars drawn around the slide picture (x, y, w, h)
COM_SLIDE_REC Com_slide_bar_tbl[4] =
{
    { 0xFF60, 0xFF88, 0x0140, 0x0030 },   // 0 //
    { 0xFF60, 0x0042, 0x0140, 0x0036 },   // 1 //
    { 0xFF60, 0xFFB8, 0x0039, 0x008A },   // 2 //
    { 0x0066, 0xFFB8, 0x003A, 0x008A },   // 3 //
};
