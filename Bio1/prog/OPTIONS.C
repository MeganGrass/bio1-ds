/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\OPTIONS.C
 */

#include "OPTIONS.H"
#include "CONFIG.H"
#include "CUT.H"
#include "EVENT.H"
#include "GAME.H"
#include "JOINT.H"
#include "MOJIDISP.H"
#include "PAD.H"
#include "SND.H"
#include "SUB.H"
#include <LIBSND.H>
#include <STDIO.H>
#include "FILECODE.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "SCENARIO.H"
#include "MONITOR.H"
#include "SDK.H"

INT Opt_from_title; // 1 when the options screen was opened from the title, 0 from the game

static UCHAR Opt_pad_img; // controller type whose picture is shown, 0xFF before the first picture

VOID Config(VOID)
{
    OPT_WORK* pOpt;

    INT Load_bak;

    INT flags;
    UCHAR v;

    INT i;

    INT rep;
    UCHAR on;
    UCHAR* pSel;
    UCHAR* pTimer;
    UCHAR c;
    INT one;
    UCHAR st;

    G.Vloop = 0;
    if (Monitor.Task[1].Tcb_status == 0)
    {
        Opt_from_title = 1;
        Snd_sys_init();
        if (VAB_ID_CUR(1) >= 0)
        {
            SsVabClose(MAIN.Scenario.Vab_id[1]);
            MAIN.Scenario.Vab_id[1] = -1;
        }
        Load_bak = FREE_WORK_INT;
        FREE_WORK_CUR = LOAD_BUF;
        Snd_sys_enem_set(0);
        FREE_WORK_INT = Load_bak;
    }
    else
    {
        Opt_from_title = 0;
        Task_suspend(0);
    }

    pOpt = &Cnf_work;
    pOpt->Opt_rno = 0;

    Cnf_work.Sel_no = 0;

    v = KEY_IDX_CUR & 0xEF;
    flags = STATUS_FLG_CUR;
    KEY_IDX_CUR = v;
    if (flags & STAT_BG_BORDER)
    {
        MAIN.SaveData.Key_idx = v | 0x10;
        MAIN.Status_flg = flags & ~STAT_BG_BORDER;
    }
    G.Fade_hrate = 2;

    G.Fade_add = -0x800;
    Evt_fade_start();
    MAIN.System_flg = (MAIN.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;

    Cd_read_wait(CONFIG_TIM, LOAD_BUF);

    G.Vram_cur.b.Cclut = 0x1F;
    G.Vram_cur.b.Ctpage = 0x15;
    Mapping_tim(LOAD_BUF);
    Task_sleep(1);

    Opt_pad_img = 0xFF;

    MAIN.System_flg = (MAIN.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_IMAGE;

    SetTile(&pOpt->Tile[0]);
    SetTile(&pOpt->Tile[1]);
    SetSemiTrans(&pOpt->Tile[0], 1);
    SetSemiTrans(&pOpt->Tile[1], 1);
    SetDrawMode(&Cnf_work.Dr_mode[0], 0, 0, GetGraphType() == 1 || GetGraphType() == 2 ? 0x300 : 0xC0, 0);
    SetDrawMode(&Cnf_work.Dr_mode[1], 0, 0, GetGraphType() == 1 || GetGraphType() == 2 ? 0x300 : 0xC0, 0);
    Cnf_work.Pad_fade_lv = 0;
    Cnf_work.Pad_fade_rno = 0;
    for (i = 0; i < 2; i++)
    {
        Cnf_work.Tile[i].x0 = 0;
        Cnf_work.Tile[i].y0 = (1 - i) * 240 + 48;
        Cnf_work.Tile[i].w = 320;
        Cnf_work.Tile[i].h = 103;
    }

    for (;;)
    {
        st = 1;
        one = 1;

        pSel = &Cnf_work.Sel_no;

        pTimer = pSel + 6;

        do
        {
        } while (0);

        Opt_pad_img_proc(&Opt_pad_img);

        switch (Cnf_work.Opt_rno)
        {
        case 0:
            if (G.SaveData.Fade_kido >= 0)
            {
                break;
            }

            Cnf_work.Opt_rno = st;
            Cnf_work.Pad_rep_old = 0;

        case 1:

            if (((MAIN.Padd1 & PAD_SELECT) || !(G.Padd1_trg & PAD_START)) && !(G.Padd1_trg & PAD_TRIANGLE))
            {
                if (G.Padd1_trg & PAD_X)
                {
                    Snd_se_play(SE_BANK_ENEMY, 6, 0);
                    switch (*pSel)
                    {
                    case 0:
                        pSel[-3] = 2;
                        pSel[-2] = 0;
                        break;
                    case 1:
                        pSel[-3] = 3;
                        break;
                    case 2:

                        pSel[-3] = 5;
                        break;
                    case 3:

                        Config_exit();
                        break;
                    }
                }
                else
                {
                    rep = Pad_rep_get(pTimer, (USHORT*)(pTimer + 1), 0xE6, 0xA000);  // halfword view of the same memory
                    if (rep & 0xA000)
                    {
                        Snd_se_play(SE_BANK_ENEMY, 4, 0);
                        if (rep & 0x8000)
                        {
                            if ((((OPT_WORK*)(VOID*)&Cnf_work)->Sel_no) == 0)  // reinterprets the memory as OPT_WORK
                            {
                                Cnf_work.Sel_no = 4;
                            }
                            Cnf_work.Sel_no--;
                        }
                        else
                        {
                            (((OPT_WORK*)(VOID*)&Cnf_work)->Sel_no)++;  // reinterprets the memory as OPT_WORK

                            if ((((OPT_WORK*)(VOID*)&Cnf_work)->Sel_no) == 4)  // reinterprets the memory as OPT_WORK
                            {
                                (((OPT_WORK*)(VOID*)&Cnf_work)->Sel_no) = 0;  // reinterprets the memory as OPT_WORK
                            }
                        }
                    }
                }
            }
            else
            {
                Snd_se_play(SE_BANK_ENEMY, 5, 0);

                Config_exit();
            }

            break;
        case 2:
            while (!(UCHAR)Cnf_key_main())  // truncates to a byte
            {
                Task_sleep(1);

                Opt_pad_img_proc(&Opt_pad_img);
            }

            if (!(G.Padd1_trg & PAD_START))
            {
                Cnf_work.Opt_rno = 1;
                (((OPT_WORK*)(VOID*)&Cnf_work)->Sel_no) = 0;  // reinterprets the memory as OPT_WORK
            }
            else
            {
                Snd_se_play(SE_BANK_ENEMY, 5, 0);
                Config_exit();
            }

            break;
        case 3:

            if ((MAIN.Padd1 & PAD_SELECT) || !(G.Padd1_trg & PAD_START))
            {
                if (!(G.Padd1_trg & (PAD_TRIANGLE | PAD_X)))
                {
                    if (G.Padd1_trg & (PAD_UP | PAD_DOWN))
                    {
                        Snd_se_play(SE_BANK_ENEMY, 4, 0);

                        KEY_IDX_CUR ^= 0x80;
                        Snd_sys_stereo((MAIN.SaveData.Key_idx >> 7) ^ 1);
                    }
                    ScratchPad->Spr.cy = 0x1FF;

                    ScratchPad->Spr.tpage = 0x15;
                    ScratchPad->Spr.y = -0x40;
                    ScratchPad->Spr.w = 0x58;

                    ScratchPad->Spr.cx = 0;

                    ScratchPad->Spr.r = 0;
                    ScratchPad->Spr.g = 0;
                    ScratchPad->Spr.b = 0;
                    ScratchPad->Spr.mx = 0;
                    ScratchPad->Spr.my = 0;
                    ScratchPad->Spr.rotate = 0;
                    ScratchPad->Spr.attribute = 0x1000040;
                    ScratchPad->Spr.x = -0x2C;

                    ScratchPad->Spr.h = 0x12;
                    ScratchPad->Spr.u = 0;
                    ScratchPad->Spr.v = 0xE8;
                    GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[CBUF_ID_CUR], 2);
                    ScratchPad->Spr.y = -0x2C;
                    GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[MAIN.Cbuf_id], 2);
                    on = MAIN.SaveData.Key_idx >> 7;

                    c = on;

                    sprintf(ScratchPad->Text_buf, "STEREO"); // CHAR* for sprintf: Text_buf is a UCHAR buffer

                    Print14(0x88, 0x3B, c * 3, 0);

                    c = on ^ 1;

                    sprintf(ScratchPad->Text_buf, "MONO"); // CHAR* for sprintf: Text_buf is a UCHAR buffer

                    Print14(0x90, 0x4F, (UCHAR)(c * 3), 0);  // truncates to a byte
                }
                else
                {
                    Cnf_work.Opt_rno = 1;

                    Snd_se_play(SE_BANK_ENEMY, 6, 0);
                }
            }
            else
            {
                Snd_se_play(SE_BANK_ENEMY, 5, 0);

                Config_exit();
            }
            break;
        case 4:

            if (Opt_from_title != 0)
            {
                Snd_sys_end();

                Task_lchain(TITLE_EXE);
            }

            if (!(MAIN.System_flg & SYS_FADE_OUT))
            {
                G.Player.Hokan_flg = 0;
                G.Player.Move_cnt = 0;
                G.Player.Move_wait = 0;
                G.Player.Damage_no = 0;

                PL_ROUTINE_WORD = one;
                G.Player.Move_no = 0;
                Joint_move(0, G.Player.pKan_t_ptr, G.Player.pSeq_t_ptr, 0x400);
                Cut_set();

                MAIN.System_flg &= ~SYS_STATUS_MASK;

                v = KEY_IDX_CUR;
                if (v & 0x10)
                {
                    MAIN.SaveData.Key_idx = v & 0xEF;

                    STATUS_FLG_CUR |= STAT_BG_BORDER;
                }

                G.Vloop = 2;

                Task_signal(0);
                Task_exit();
            }
            break;
        case 5:

            if ((G.Padd1 & PAD_SELECT) || !(G.Padd1_trg & PAD_START))
            {
                if (!(G.Padd1_trg & (PAD_TRIANGLE | PAD_X)))
                {
                    if (G.Padd1_trg & (PAD_UP | PAD_DOWN))
                    {
                        Snd_se_play(SE_BANK_ENEMY, 4, 0);
                        KEY_IDX_CUR ^= 0x20;
                    }
                    ScratchPad->Spr.cy = 0x1FF;
                    ScratchPad->Spr.tpage = 0x15;
                    ScratchPad->Spr.y = -0x40;
                    ScratchPad->Spr.w = 0x58;
                    ScratchPad->Spr.cx = 0;
                    ScratchPad->Spr.r = 0;
                    ScratchPad->Spr.g = 0;
                    ScratchPad->Spr.b = 0;
                    ScratchPad->Spr.mx = 0;
                    ScratchPad->Spr.my = 0;
                    ScratchPad->Spr.rotate = 0;
                    ScratchPad->Spr.attribute = 0x1000040;
                    ScratchPad->Spr.x = -0x2C;
                    ScratchPad->Spr.h = 0x12;
                    ScratchPad->Spr.u = 0;
                    ScratchPad->Spr.v = 0xE8;
                    GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[CBUF_ID_CUR], 2);
                    ScratchPad->Spr.y = -0x2C;
                    GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[MAIN.Cbuf_id], 2);
                    on = (MAIN.SaveData.Key_idx >> 5) & 1;
                    c = on;
                    sprintf(ScratchPad->Text_buf, "ON"); // CHAR* for sprintf: Text_buf is a UCHAR buffer

                    Print14(0x99, 0x3B, c * 3, 0);

                    c = on ^ 1;
                    sprintf(ScratchPad->Text_buf, "OFF"); // CHAR* for sprintf: Text_buf is a UCHAR buffer

                    Print14(0x95, 0x4F, (UCHAR)(c * 3), 0);  // truncates to a byte
                }
                else
                {
                    Pad_vib_set0(7, 0);
                    Pad_vib_set1(9, 0xFF, 0);
                    Pad_vib_fade_set(4, 0xFF, 0x3C, 0xA);
                    Cnf_work.Opt_rno = 1;
                    Snd_se_play(SE_BANK_ENEMY, 6, 0);
                }
            }
            else
            {
                Snd_se_play(SE_BANK_ENEMY, 5, 0);
                Config_exit();
            }
            break;
        }
        if (Cnf_work.Opt_rno != 4)
        {
            ScratchPad->Spr.cy = 0x1FF;
            ScratchPad->Spr.cx = 0;
            ScratchPad->Spr.tpage = 0x15;
            ScratchPad->Spr.r = 0;
            ScratchPad->Spr.g = 0;
            ScratchPad->Spr.b = 0;
            ScratchPad->Spr.mx = 0;
            ScratchPad->Spr.my = 0;
            ScratchPad->Spr.rotate = 0;
            Opt_box_draw();
            Cnf_key_layout_disp();
        }
        Task_sleep(1);
    }
}

VOID Config_exit(VOID)
{
    Cnf_work.Opt_rno = 4;
    Evt_fade_req_set(2, 0x800);
    MAIN.System_flg = (MAIN.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;
}

VOID Opt_box_draw(VOID)
{
    UCHAR i;
    UCHAR* pSel;
    USHORT* pBox_x;
    INT col;

    ScratchPad->Rect.attribute = 0x60000000;
    ScratchPad->Rect.x = -0xA0;
    ScratchPad->Rect.y = -0x48;
    ScratchPad->Rect.w = 0x140;
    ScratchPad->Rect.h = 0xC0;
    ScratchPad->Rect.r = 0x48;
    ScratchPad->Rect.g = 0x48;
    ScratchPad->Rect.b = 0x48;
    GsSortBoxFill(&ScratchPad->Rect, &MAIN.Ot0[G.Cbuf_id], 3);
    ScratchPad->Rect.y = -0x5F;
    ScratchPad->Rect.h = 0xE;

    i = 4;

    pBox_x = Opt_box_tbl;
    col = 0x30;
    pSel = &Cnf_work.Sel_no;
    do
    {
        i--;
        ScratchPad->Rect.x = pBox_x[i];
        if (i != 0)
        {
            ScratchPad->Rect.w = 0x2E;
        }
        else
        {
            ScratchPad->Rect.w = col;
        }
        if (i == *pSel)
        {
            if (pSel[-3] == 3 || pSel[-3] == 5)
            {
                continue;
            }
            ScratchPad->Rect.attribute = 0x50000000;
            ScratchPad->Rect.r = 0;
            ScratchPad->Rect.g = 0;
            ScratchPad->Rect.b = 0x80;
        }
        else
        {
            ScratchPad->Rect.attribute = 0x60000000;
            ScratchPad->Rect.r = col;
            ScratchPad->Rect.g = col;
            ScratchPad->Rect.b = col;
        }
        GsSortBoxFill(&ScratchPad->Rect, &MAIN.Ot0[G.Cbuf_id], 2);
    } while (i != 0);
}

VOID Opt_pad_img_proc(UCHAR* pType)
{
    INT n;

    switch (Cnf_work.Pad_fade_rno)
    {
    case 0:
        if (*pType != Pad_device_now)
        {
            if (Pad_device_now == 7)
            {
                Cnf_work.Pad_img_sts = 2;
                if (*pType != 0xFF)
                {
                    Cnf_work.Pad_fade_lv = 0x3F;
                    Cnf_work.Pad_fade_rno++;
                    Cnf_work.Pad_img_sts = 0x82;
                }
                else
                {
                    Cnf_work.Pad_fade_rno = 3;
                }
                *pType = Pad_device_now;
            }
            else if (*pType == 7 || *pType == 0xFF)
            {
                Cnf_work.Pad_img_sts = 1;
                if (*pType != 0xFF)
                {
                    Cnf_work.Pad_fade_lv = 0x3F;
                    Cnf_work.Pad_fade_rno++;
                    Cnf_work.Pad_img_sts = 0x81;
                }
                else
                {
                    Cnf_work.Pad_fade_rno = 3;
                }
                *pType = Pad_device_now;
            }
        }
        break;
    case 1:
        if ((Cnf_work.Pad_fade_lv += 0x40) == 0xFF)
        {
            Cnf_work.Pad_fade_rno++;
        }
        break;
    case 2:
        Cnf_work.Pad_fade_rno++;
        break;
    case 3:

        n = OPTION2_PIX;

        if (((((OPT_WORK*)(VOID*)&Cnf_work)->Pad_img_sts) & 3) == 1)  // reinterprets the memory as OPT_WORK
        {
            n = OPTION1_PIX;
        }
        Cd_read_wait(n, LOAD_BUF);
        if (Cnf_work.Pad_img_sts & 0x80)
        {
            Cnf_work.Pad_fade_lv = 0xFF;
            Cnf_work.Pad_fade_rno++;
        }
        else
        {
            Cnf_work.Pad_fade_rno = 0;
        }
        break;
    case 4:
        if ((Cnf_work.Pad_fade_lv -= 0x40) == 0x3F)
        {
            Cnf_work.Pad_fade_lv = 0;
            Cnf_work.Pad_fade_rno = 0;
        }
        break;
    }
    AddPrim(MAIN.Ot0[CBUF_ID_CUR].org + 1, &Cnf_work.Dr_mode[MAIN.Cbuf_id]);
    Cnf_work.Tile[MAIN.Cbuf_id].r0 = Cnf_work.Tile[MAIN.Cbuf_id].g0 = Cnf_work.Tile[MAIN.Cbuf_id].b0 = Cnf_work.Pad_fade_lv;
    AddPrim(MAIN.Ot0[MAIN.Cbuf_id].org, &Cnf_work.Tile[MAIN.Cbuf_id]);
}

// x position of each entry box in the options menu
USHORT Opt_box_tbl[4] =
{
    0xFFC4,    // 0 //
    0xFFF8,    // 1 //
    0x002A,    // 2 //
    0x005C     // 3 //
};
