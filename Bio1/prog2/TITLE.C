/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog2\TITLE.C
 */

#include "TITLE.H"
#include "SCRATCHPAD.H"
#include "FILECODE.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "MONITOR.H"

TITLE_WORK Ti; // title screen work area

VOID main(VOID)
{
    INT pad10[2];    // unused local: reserves stack space
    RECT rect;
    INT pad28[6];    // unused local: reserves stack space

    FUNC(SND_TITLE_INIT)();
    G.pFree_work = OVERLAY_AREA;

    FUNC(SND_SYS_ARMS_SET)(0xC, OVERLAY_AREA);

    G.SaveData.Fade_kido = -1;

    G.SaveData.Fade1_kido = -1;

    Ti.running = 0;
    Ti.page = 0;
    Ti.Sub_rtn0 = 0;

    G.Menu_req = 0;

    Ti.Dummy31 = 0;

    Title_init();
    FUNC(VSYNC)(0);
    rect.x = 0;
    rect.y = 0;
    rect.w = 0x1E0;
    rect.h = 0x1E0;
    FUNC(CLEAR_IMAGE)(&rect, 0, 0, 0);
    FUNC(DRAW_SYNC)(0);
    FUNC(GS_INIT_GRAPH2)(0x140, 0xF0, 0, 0, 1);
    FUNC(TASK_SLEEP)(1);
    G.System_flg = (G.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_IMAGE;
    G.Scenario.Movie_id = 0;
    G.Scenario.pMovie_buf = G.pFree_work;
    G.System_flg |= SYS_MOVIE;
    FUNC(TASK_SLEEP)(1);
    FUNC(TASK_SLEEP)(1);
    do
    {
        Title_main();
        FUNC(TASK_SLEEP)(1);
    } while (Ti.running != 0);
    switch (Ti.Cursor_no)
    {
    case 0:

        FUNC(SND_SYS_END)();

        G.Status_flg |= STAT_DEMO_MODE;
        FUNC(TASK_CHAIN)FUNC(GAME_START);

        FUNC(TASK_LCHAIN)(LOGO_EXE);

        return;
    case 1:

    case 4:
    case 5:
    case 6:

    case 7:

        FUNC(SND_SYS_END)();

        FUNC(TASK_LCHAIN)(SELECT_EXE);

    case 2:

        if (FUNC(CARD_ACCESS)(1, TITLE_OVERLAY_LOAD_AREA, 0, 1) != 0)
        {
            G.System_flg = (G.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;

            FUNC(SND_SYS_END)();

            FUNC(TASK_LCHAIN)(TITLE_EXE);
        }
        G.System_flg = (G.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;

        FUNC(SND_SYS_END)();

        FUNC(TASK_CHAIN)FUNC(GAME_START);

        return;
    case 3:
        G.System_flg = (G.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;
        FUNC(SND_SYS_END)();
        FUNC(TASK_CHAIN)FUNC(CONFIG);
        break;
    }
}

VOID Title_init(VOID)
{
    G.Vloop = 0;

    FUNC(BG_IMG_BUF_SET)(0x140, 0xF0, 0);

    FUNC(CD_READ)(TITLE_PIX, LOAD_BUF);
    FUNC(CD_READ)(TMOJI_TIM, TITLE_SPRITE_SET_0_BUFFER);

    G.Vram_cur.b.Cclut = 0x14;

    G.Vram_cur.b.Ctpage = 8;

    Ti.Spr_tpage_tbl[0] = G.Vram_cur.b.Ctpage;
    Ti.Spr_clut_tbl[0] = (UCHAR)G.Vram_cur.b.Cclut;  // truncates to a byte

    FUNC(MAPPING_TIM)(TITLE_SPRITE_SET_0_BUFFER);

    Ti.Spr_tpage_tbl[1] = Ti.Spr_tpage_tbl[2] = Ti.Spr_tpage_tbl[3] = G.Vram_cur.b.Ctpage;
    Ti.Spr_clut_tbl[1] = Ti.Spr_clut_tbl[2] = Ti.Spr_clut_tbl[3] = (UCHAR)G.Vram_cur.b.Cclut;  // truncates to a byte

    FUNC(MAPPING_TIM)(TITLE_SPRITE_TIM_SECOND);

    Ti.Spr_tpage_tbl[4] = G.Vram_cur.b.Ctpage;
    Ti.Spr_clut_tbl[4] = (UCHAR)G.Vram_cur.b.Cclut;  // truncates to a byte

    FUNC(MAPPING_TIM)(TITLE_SPRITE_DATA);

    Ti.Spr_tpage_tbl[5] = G.Vram_cur.b.Ctpage;
    Ti.Spr_clut_tbl[5] = (UCHAR)G.Vram_cur.b.Cclut;  // truncates to a byte

    FUNC(MAPPING_TIM)(TITLE_OVERLAY_API_ARG_BUFFER);

    Ti.Spr_tpage_tbl[6] = G.Vram_cur.b.Ctpage;
    Ti.Spr_clut_tbl[6] = (UCHAR)G.Vram_cur.b.Cclut;  // truncates to a byte

    FUNC(MAPPING_TIM)(TITLE_SPRITE_TIM_7);
    Ti.Spr_tpage_tbl[7] = G.Vram_cur.b.Ctpage;
    Ti.Spr_clut_tbl[7] = (UCHAR)G.Vram_cur.b.Cclut;  // truncates to a byte
    FUNC(MAPPING_TIM)(TITLE_SPRITE_SET_7_BUFFER);

    Ti.running = 1;
    Ti.Cursor_no = 1;

    G.System_flg &= ~SYS_BG_MODE_MASK;
}

VOID Title_main(VOID)
{
    INT unused[4];    // unused local: reserves stack space

    switch (Ti.page)
    {
    case 0:

        switch (Ti.Sub_rtn0)
        {
        case 0:

            Ti.Sub_rtn0 = 1;
            Ti.Timer_wk.timer = 0x80U;

        case 1:

            Ti.Timer_wk.timer -= 4;
            Title_sp_set(-0x80 - Ti.Timer_wk.timer, 0U);
            if (Ti.Timer_wk.timer == 0)
            {
                Ti.Sub_rtn0 = 2;

                Ti.Timer_wk.timer = 0x708U;
            }

            if (G.Padd_trg & (PAD_L2 | PAD_R2 | PAD_L1 | PAD_R1 | PAD_TRIANGLE | PAD_CIRCLE | PAD_X | PAD_SQUARE | PAD_SELECT | PAD_START))
            {
                Ti.Sub_rtn0 = 2;

                Ti.Timer_wk.timer = 0x708U;
            }
            break;
        case 2:

            Ti.Timer_wk.timer--;

            Title_sp_set(0x80, 0U);

            if (Ti.Timer_wk.timer == 0)
            {
                Ti.Sub_rtn0 = 3;

                G.Fade_hrate = 2;
                G.Fade_add = 0x400;
                FUNC(FADE_START)();
            }

            if (G.Padd_trg & (PAD_L2 | PAD_R2 | PAD_L1 | PAD_R1 | PAD_TRIANGLE | PAD_CIRCLE | PAD_X | PAD_SQUARE | PAD_SELECT | PAD_START))
            {
                Ti.page = 1;
                Ti.Sub_rtn0 = 2;
            }
            break;
        case 3:
            if (G.SaveData.Fade_kido >= 0x7B81)
            {
                Ti.Cursor_no = 0;

                G.SaveData.Fade_kido = 0x7FFF;
                G.Fade_add = 0;
                G.System_flg = (G.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;
                Title_exit();
            }

            if (G.Padd_trg & (PAD_L2 | PAD_R2 | PAD_L1 | PAD_R1 | PAD_TRIANGLE | PAD_CIRCLE | PAD_X | PAD_SQUARE | PAD_SELECT | PAD_START))
            {
                Ti.Sub_rtn0 = 2;

                G.SaveData.Fade_kido = -1;

                Ti.Timer_wk.timer = 0x708U;
            }
            Title_sp_set(0x80, 0U);
            break;
        case 4:
            Title_sp_set(0x80, 0U);
            if (G.SaveData.Fade_kido >= 0x7B81)
            {
                G.SaveData.Fade_kido = 0x7FFF;
                G.Fade_add = 0;

                Ti.page = 1;
                Ti.Sub_rtn0 = 0;

                G.System_flg = (G.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;
            }
        }

        return;

    case 1:

        switch (Ti.Sub_rtn0)
        {
        case 0:
            G.System_flg = (G.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_IMAGE;

            Ti.Sub_rtn0 = 1;

            G.Fade_hrate = 2;
            G.Fade_add = -0x400;
            FUNC(FADE_START)();

            if (Ti.Cursor_no >= 4U)
            {
                Ti.Cursor_no = 1;
            }

            return;

        case 1:
            if (G.SaveData.Fade_kido < 0)
            {
                Ti.Sub_rtn0 = 2;

                Ti.Timer_wk.timer = 0x708U;
            }

            if (Ti.Cursor_no >= 4U)
            {
                Title_sp_set(0x80, Ti.Cursor_no);
            }
            else
            {
                Title_mode_set(0x80, Ti.Cursor_no);
            }

            return;
        case 2:
            if (Ti.Cursor_no >= 4U)
            {
                Title_sp_set(0x80, Ti.Cursor_no);
            }
            else
            {
                Title_mode_set(0x80, Ti.Cursor_no);
            }

            if (G.Padd_trg & (PAD_TRIANGLE | PAD_CIRCLE | PAD_X | PAD_SQUARE | PAD_START))
            {
                if (Ti.Cursor_no == 1)
                {
                    Ti.Cursor_no = 4;

                    Ti.Sub_rtn0 = 0xA;

                    Ti.Timer_wk.timer = 0x708U;

                    Ti.Timer_wk.Hold_timer = 0x5A;
                }
                else
                {
                    G.Status_flg &= ~STAT_EASY_MODE;
                    G.Status_flg &= ~STAT_ARRANGE;
                    G.Status_flg &= ~STAT_ARRANGE_EASY;
                    G.SaveData.Dc_mode = 0;
                    FUNC(SND_SE_PLAY)(1, 0, 0);
                    FUNC(SND_SE_PLAY)(1, 1, 0);
                    Ti.Sub_rtn0 = 6;
                    G.Fade_hrate = 1;
                    G.Fade_add = 0x7F00;
                    FUNC(FADE_START)();
                    G.Vloop = 2;

                    if (Ti.Cursor_no == 2)
                    {
                        FUNC(PAD_VIB_SET0)(3, 0);
                        FUNC(PAD_VIB_SET1)(4, 0xFF, 0);
                        FUNC(PAD_VIB_FADE_SET)(2, 0xFF, 0x3C, 0xA);
                    }
                }

                return;
            }
            if (G.Padd_trg & (PAD_SELECT | PAD_UP | PAD_DOWN))
            {
                if (G.Padd_trg & PAD_UP)
                {
                    Ti.Cursor_no--;
                    if (Ti.Cursor_no == 0)
                    {
                        Ti.Cursor_no = 3;
                    }
                }
                else
                {
                    Ti.Cursor_no++;

                    if (Ti.Cursor_no >= 4U)
                    {
                        Ti.Cursor_no = 1;
                    }
                }

                Ti.Timer_wk.timer = 0x708U;
            }

            Ti.Timer_wk.timer--;
            if (Ti.Timer_wk.timer == 0)
            {
                Ti.Sub_rtn0 = 3;

                G.Fade_hrate = 2;
                G.Fade_add = 0x400;
                FUNC(FADE_START)();
            }

            return;

        case 3:
            if (G.SaveData.Fade_kido < 0)
            {
                Ti.Cursor_no = 0;

                Title_exit();
                return;
            }

            if (Ti.Cursor_no >= 4U)
            {
                Title_sp_set(0x80, Ti.Cursor_no);
            }
            else
            {
                Title_mode_set(0x80, Ti.Cursor_no);
            }

            if (G.Padd_trg & (PAD_L2 | PAD_R2 | PAD_L1 | PAD_R1 | PAD_TRIANGLE | PAD_CIRCLE | PAD_X | PAD_SQUARE | PAD_SELECT | PAD_START))
            {
                Ti.Sub_rtn0 = 0;

                G.Fade_add = -0x1000;
            }

            return;

        case 4:
            if (G.SaveData.Fade_kido < 0)
            {
                Title_exit();
                G.System_flg &= SYS_BG_MODE_MASK;
                return;
            }

            if (Ti.Cursor_no >= 4U)
            {
                Title_sp_set(0x80, Ti.Cursor_no);
            }
            else
            {
                Title_mode_set(0x80, Ti.Cursor_no);
            }

        case 6:
            if (G.SaveData.Fade_kido < 0)
            {
                Ti.Sub_rtn0 = 7;

                G.Fade_hrate = 1;
                G.Fade_add = -0x4000;
                FUNC(FADE_START)();

                if (Ti.Cursor_no >= 4U)
                {
                    Title_sp_set(0x80, Ti.Cursor_no);
                }
                else
                {
                    Title_mode_set(0x80, Ti.Cursor_no);
                }

                return;
            }
        case 7:
            if (G.SaveData.Fade_kido < 0)
            {
                Ti.Sub_rtn0 = 8;

                G.Fade_hrate = 1;
                G.Fade_add = -0x8000;
                FUNC(FADE_START)();

                if (Ti.Cursor_no >= 4U)
                {
                    Title_sp_set(0x80, Ti.Cursor_no);
                }
                else
                {
                    Title_mode_set(0x80, Ti.Cursor_no);
                }

                return;
            }
        case 8:
            if (G.SaveData.Fade_kido < 0)
            {
                Ti.Sub_rtn0 = 9;

                G.Fade_hrate = 1;
                G.Fade_add = -0x800;
                FUNC(FADE_START)();

                if (Ti.Cursor_no >= 4U)
                {
                    Title_sp_set(0x80, Ti.Cursor_no);
                }
                else
                {
                    Title_mode_set(0x80, Ti.Cursor_no);
                }

                return;
            }
        case 9:
            if (G.SaveData.Fade_kido < 0)
            {
                Ti.Sub_rtn0 = 4;

                G.Fade_hrate = 2;
                G.Fade_add = 0x270;
                FUNC(FADE_START)();

                if (Ti.Cursor_no >= 4U)
                {
                    Title_sp_set(0x80, Ti.Cursor_no);
                }
                else
                {
                    Title_mode_set(0x80, Ti.Cursor_no);
                }

                return;
            }

            if (Ti.Cursor_no >= 4U)
            {
                Title_sp_set(0x80, Ti.Cursor_no);
            }
            else
            {
                Title_mode_set(0x80, Ti.Cursor_no);
            }

            return;

        case 10:
            if (G.Padd_trg & (PAD_SELECT | PAD_UP | PAD_DOWN))
            {
                if (G.Padd_trg & PAD_UP)
                {
                    Ti.Cursor_no--;
                    if (Ti.Cursor_no < 4U)
                    {
                        Ti.Cursor_no = 6;
                    }
                }
                else
                {
                    Ti.Cursor_no++;
                    if (Ti.Cursor_no >= 7U)
                    {
                        Ti.Cursor_no = 4;
                    }
                }

                Ti.Timer_wk.timer = 0x708U;
            }
            if (G.Padd_trg & (PAD_TRIANGLE | PAD_CIRCLE | PAD_X | PAD_SQUARE | PAD_START))
            {
                FUNC(PAD_VIB_SET0)(3, 0);
                FUNC(PAD_VIB_SET1)(4, 0xFF, 0);
                FUNC(PAD_VIB_FADE_SET)(2, 0xFF, 0x3C, 0xA);

                G.Status_flg &= ~STAT_EASY_MODE;
                G.Status_flg &= ~STAT_ARRANGE;
                G.Status_flg &= ~STAT_ARRANGE_EASY;
                G.SaveData.Dc_mode = 0;

                if (Ti.Cursor_no == 5)
                {
                    G.Status_flg |= STAT_EASY_MODE;
                    G.SaveData.Dc_mode = 1;
                }

                if (Ti.Cursor_no == 6)
                {
                    G.Status_flg |= STAT_ARRANGE;
                    G.SaveData.Dc_mode = 2;
                    if (Ti.Timer_wk.Hold_timer == 0)
                    {
                        Ti.Cursor_no = 7;

                        G.Status_flg |= STAT_ARRANGE_EASY;
                        G.SaveData.Dc_mode = 3;
                    }
                }
                FUNC(SND_SE_PLAY)(1, 0, 0);
                FUNC(SND_SE_PLAY)(1, 1, 0);
                Ti.Sub_rtn0 = 6;
                G.Fade_hrate = 1;
                G.Fade_add = 0x7F00;
                FUNC(FADE_START)();
                G.Vloop = 2;
                return;
            }

            if (Ti.Cursor_no == 6)
            {
                if (G.Padd & PAD_RIGHT)
                {
                    Ti.Timer_wk.Hold_timer--;
                    if (Ti.Timer_wk.Hold_timer < 0)
                    {
                        Ti.Timer_wk.Hold_timer = 0;
                    }
                }
                if (Ti.Timer_wk.Hold_timer == 0)
                {
                    Ti.Cursor_no = 7;
                }
            }

            if (Ti.Cursor_no >= 4U)
            {
                Title_sp_set(0x80, Ti.Cursor_no);
            }
            else
            {
                Title_mode_set(0x80, Ti.Cursor_no);
            }
            if (Ti.Cursor_no == 7)
            {
                Ti.Cursor_no = 6;
            }
            Ti.Timer_wk.timer--;
            if (Ti.Timer_wk.timer == 0)
            {
                Ti.Sub_rtn0 = 3;
                G.Fade_hrate = 2;
                G.Fade_add = 0x400;
                FUNC(FADE_START)();
            }
            if ((G.Padd & 4) && (G.Padd & 8))
            {
                Ti.Sub_rtn0 = 0;
            }

            break;
        }
    }
}

VOID Title_nop(VOID)
{
}

VOID Title_sp_set(UCHAR bright, UCHAR option)
{
    INT unused;     // unused local: reserves stack space
    INT Rsv2;    // unused local: reserves stack space

    if (bright == 0x80)
    {
        ScratchPad->Spr.attribute = 0x10000000;
    }
    else
    {
        ScratchPad->Spr.attribute = 0x50000000;
    }
    ScratchPad->Spr.x = -0x82;
    ScratchPad->Spr.y = 0x26;
    ScratchPad->Spr.w = 0x100;
    ScratchPad->Spr.h = 0x50;

    ScratchPad->Spr.tpage = Ti.Spr_tpage_tbl[option];

    ScratchPad->Spr.u = 0;
    ScratchPad->Spr.v = 0;
    ScratchPad->Spr.cx = 0;

    ScratchPad->Spr.cy = Ti.Spr_clut_tbl[option] + 0x1E0;

    ScratchPad->Spr.r = bright;
    ScratchPad->Spr.g = bright;
    ScratchPad->Spr.b = bright;
    FUNC(GS_SORT_FAST_SPRITE)(&ScratchPad->Spr.attribute, &MAIN.Ot0[G.Cbuf_id], 2);
}

VOID Title_mode_set(UCHAR bright, UCHAR option)
{
    INT unused;     // unused local: reserves stack space
    INT Rsv2;    // unused local: reserves stack space

    if (bright == 0x80)
    {
        ScratchPad->Spr.attribute = 0x10000000;
    }
    else
    {
        ScratchPad->Spr.attribute = 0x50000000;
    }
    ScratchPad->Spr.x = -0x82;
    ScratchPad->Spr.y = 0x26;
    ScratchPad->Spr.w = 0x100;
    ScratchPad->Spr.h = 0x50;

    ScratchPad->Spr.tpage = Ti.Spr_tpage_tbl[1];

    ScratchPad->Spr.u = 0;
    ScratchPad->Spr.v = 0;
    ScratchPad->Spr.cx = 0;

    ScratchPad->Spr.cy = Ti.Spr_clut_tbl[1] + 0x1DF + option;

    ScratchPad->Spr.r = bright;
    ScratchPad->Spr.g = bright;
    ScratchPad->Spr.b = bright;
    FUNC(GS_SORT_FAST_SPRITE)(&ScratchPad->Spr.attribute, &MAIN.Ot0[G.Cbuf_id], 2);
}

VOID Title_exit(VOID)
{
    Ti.running = 0;

    G.System_flg = (G.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;
}
