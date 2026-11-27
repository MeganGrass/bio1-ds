/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog2\ENDING.C
 */

#include "ENDING.H"
#include "SCRATCHPAD.H"
#include "FILECODE.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "MONITOR.H"

static UINT Ending_pad0;          // padding: keeps the following data at its linked position

static UCHAR* pEnding_str_buf;                 // movie streaming buffer
static UCHAR* pEnding_work_buf;                // work buffer for loaded files: credits text data, later the game data file
static UINT Ending_pad1;               // padding: keeps the following data at its linked position
static UCHAR* pEnding_strip_buf;               // buffer for the expanded credits text strip image
static UCHAR* pEnding_pl_pic;           // buffer for the player picture (320 by 240, 16 bits)
static UCHAR* pEnding_model_head;               // buffer for the result screen model file
static UCHAR* pEnding_tmd;               // model data inside pEnding_model_head
static UCHAR* pEnding_pkt_buf;              // GPU packet buffer for the result screen model
static UCHAR* pEnding_free_work;               // load pointer position the effect system initialisation starts from
static INT Ending_blk_adr;                // address of the next credits text block
static USHORT* pEnding_run;              // run lengths of the current text block
static UCHAR* pEnding_val;              // byte values of the current text block
static INT Ending_img_adr;                // address of the current text strip image
static ENDING_TEXT_HEAD* pEnding_blk;    // header of the current text block
static END_SPR_WORK Ending_spr_wk[21];             // sprite slots
static END_SPR_WORK* pEnding_spr_new;              // slot selected by the last allocation
static END_SPR_WORK* pEnding_spr_cur;           // slot being drawn
static END_SPR_WORK* pEnding_bg_a;              // background slot, left 256 pixels
static END_SPR_WORK* pEnding_bg_b;              // background slot, right 64 pixels
static END_SPR_WORK* pEnding_pic;          // slot driving the picture cover boxes
static END_SPR_WORK* pEnding_txt;             // caption text slot, also used for the logo zoom
static END_SPR_WORK* Ending_lbl_tbl[5];     // epilogue and result label slots
static UINT Ending_pad2[2];      // padding: keeps the following data at its linked position
static UCHAR Ending_cap_no;                // caption page buffer being filled (0 or 1)
static INT Ending_final_shot_step;               // cleared at scene reset, never read
static UINT Ending_scrl_pos;              // credits scroll position, 16.16 fixed point
static UINT Ending_scrl_old;           // scroll position at which the last strip was started
static INT Ending_scrl_lim;                // credits scroll end position, never read
static UINT Ending_pad4[5];         // padding: keeps the following data at its linked position
static INT Ending_str_end_flg;                   // nonzero once the movie has ended or its volume was cut
static SHORT Ending_scrl_mode;              // credits scroll mode copied from Ending_def_tbl
static USHORT Ending_row_no;             // video memory row index of the current text strip
static SHORT Ending_scrl_ofs;             // horizontal offset added to the credits strips
static USHORT Ending_str_frm;               // current movie frame
static USHORT Ending_cap_end_flg;          // 1 once the last caption page has been shown
static CAPTION_TBL* pEnding_cap;      // current caption page entry
static ENDING_BG_REC* pEnding_bg_rec;         // current background cue, never used
static ENDING_PIC_REC* pEnding_pic_rec;  // current picture cue
static GsRVIEW2 Ending_view;                 // camera view of the epilogue and result scenes
static VECTOR Ending_esp_pos;              // position of the clear time effect
static GsCOORDINATE2 Ending_coord0;       // result model root coordinate system
static GsCOORDINATE2 Ending_coord1;       // result model spinning coordinate system
static GsCOORDINATE2 Ending_coord2;       // result model leaf coordinate system
static UINT Ending_pad3[24];        // padding: keeps the following data at its linked position
static GsDOBJ5 Ending_obj;              // result screen model object
static GsF_LIGHT Ending_light;               // result screen flat light
static SHORT* pEnding_pkt;              // cursor into the model packet buffer
static SVECTOR Ending_rot;            // rotation angles for the model coordinate systems
static UCHAR Ending_rocket_flg;       // nonzero when the rocket launcher is held, stored or awarded
static UCHAR Ending_no;                      // ending number, 1 to 7
static UCHAR Ending_time_flg;          // 1 when the play time is 3 hours or less
static UCHAR Ending_flg_7e;           // 1 when event flag 0x7E was already set

static UCHAR Ending_flg_7a;            // 1 when event flag 0x7A was already set

static UCHAR Ending_set_no;                 // caption and cue list set (0 or 1), never assigned so set 0 is used
static UCHAR Ending_rno0;                    // main step: 0 credits, 1 movie captions, 2 logo zoom, 0x63 finished
static UCHAR Ending_rno1;                 // state within the current step
static UCHAR Ending_cap_sts;                // caption state: 0 waiting to show, 1 waiting to hide
static SHORT Ending_wait_cnt;               // frame counter for timed states
static USHORT Ending_frm_cnt;                  // credits frame counter

static SCHAR Ending_unk_c8;                // cleared at start, never read

static SCHAR Ending_unk_cc;                // cleared at start, never read
static SCHAR Ending_unk_d0;                // cleared at start, never read
static UCHAR Ending_pic_sts;            // state of the picture cue list
static SCHAR Ending_unk_d8;                // cleared at start, never read
static SCHAR Ending_unk_dc;                // cleared at start, never read
static INT Ending_zoom_z;                   // logo zoom depth
static INT Ending_zoom_spd;                // logo zoom depth velocity
static INT Ending_zoom_add;                // logo zoom depth acceleration
static INT* pEnding_src;                  // source pointer of the game data copy
static INT* pEnding_dst;                  // destination pointer of the game data copy
static INT Ending_room_head[48];              // room header block used by the epilogue scene
static INT Ending_sts_bak;          // status flags saved at the start

static GsBOXF Ending_box_a;                   // picture cover box, horizontal band
static GsBOXF Ending_box_b;                   // picture cover box, vertical band

VOID main(VOID)
{
    RECT rect;
    UCHAR Inv_idx;
    INT Sts_bak;
    USHORT i;
    INT Str_sts;

    Ending_sts_bak = G.Status_flg;
    G.Status_flg &= STAT_RESET_KEEP_MASK;

    G.System_flg |= SYS_GRAPH_RESET;
    FUNC(TASK_SLEEP)(1);

    FUNC(SND_TITLE_INIT)();

    FUNC(SND_SYS_ARMS_SET)(0xE, (OVERLAY_AREA));

    G.pFree_work = (UCHAR*)OVERLAY_AREA;  // byte view of the same memory
#ifdef __INTELLISENSE__  // void* arithmetic is not valid C for IntelliSense
    *(UCHAR**)&G.pFree_work += 0x10000;
    pEnding_work_buf = G.pFree_work;
    *(UCHAR**)&G.pFree_work += 0x40000;
    pEnding_strip_buf = G.pFree_work;
    *(UCHAR**)&G.pFree_work += 0x1000;
    pEnding_pl_pic = G.pFree_work;
    *(UCHAR**)&G.pFree_work += 0x25800;
    pEnding_free_work = G.pFree_work;
    *(UCHAR**)&G.pFree_work += 0x10000;
    pEnding_str_buf = G.pFree_work;
    *(UCHAR**)&G.pFree_work += 0x30000;
    pEnding_model_head = G.pFree_work;
    pEnding_tmd = pEnding_model_head + 0x10220;
    *(UCHAR**)&G.pFree_work += 0x30000;
#else
    G.pFree_work += 0x10000;
    pEnding_work_buf = G.pFree_work;
    G.pFree_work += 0x40000;
    pEnding_strip_buf = G.pFree_work;
    G.pFree_work += 0x1000;
    pEnding_pl_pic = G.pFree_work;
    G.pFree_work += 0x25800;
    pEnding_free_work = G.pFree_work;
    G.pFree_work += 0x10000;
    pEnding_str_buf = G.pFree_work;
    G.pFree_work += 0x30000;
    pEnding_model_head = G.pFree_work;
    pEnding_tmd = pEnding_model_head + 0x10220;
    G.pFree_work += 0x30000;
#endif
    pEnding_pkt_buf = G.pFree_work;

    Ending_no_set();

    G.pFree_work = (UCHAR*)OVERLAY_AREA;  // byte view of the same memory

    if (Ending_def_tbl[Ending_no - 1].d != 0)
    {
        FUNC(CD_READ)(EN07_TIM, G.pFree_work);
    }
    else
    {
        FUNC(CD_READ)(EN05_TIM, G.pFree_work);
    }

    FUNC(CD_READ)(I73V_IVM, pEnding_model_head);

    if (G.Player.Id & 3)
    {
        FUNC(CD_READ)(JILL01_PIX, pEnding_pl_pic);
    }
    else
    {
        FUNC(CD_READ)(CLIS01_PIX, pEnding_pl_pic);
    }

    G.Vram_cur.b.Ctpage = 0x1B;
    G.Vram_cur.b.Cclut = 0x1B;
    FUNC(MAPPING_TIM)(G.pFree_work);

    G.pFree_work = pEnding_free_work;
    FUNC(ESP_INIT_C)();
    FUNC(MEMCLR)(&MAIN.Esp[0].Routine0, 0x840);

    FUNC(VSYNC)(0);
    rect.x = 0;
    rect.y = 0;
    rect.w = 0x1E0;
    rect.h = 0x1E0;
    FUNC(CLEAR_IMAGE)(&rect, 0, 0, 0);
    FUNC(DRAW_SYNC)(0);
    G.Scenario.Movie_id = 0xE;
    G.Scenario.pMovie_buf = pEnding_str_buf;
    G.System_flg |= SYS_MOVIE;
    FUNC(TASK_SLEEP)(1);
    FUNC(TASK_SLEEP)(1);

    rect.x = 0;
    rect.y = G.Cbuf_id != 0 ? 0xF0 : 0;
    rect.w = 0x1E0;
    rect.h = 0xF0;
    FUNC(VSYNC)(0);
    FUNC(CLEAR_IMAGE)(&rect, 0, 0, 0);
    FUNC(DRAW_SYNC)(0);
    FUNC(TASK_SLEEP)(1);
    rect.y = G.Cbuf_id != 0 ? 0xF0 : 0;
    FUNC(VSYNC)(0);
    FUNC(CLEAR_IMAGE)(&rect, 0, 0, 0);
    FUNC(DRAW_SYNC)(0);
    FUNC(VSYNC)(0);
    FUNC(GS_INIT_GRAPH2)(0x140, 0xF0, 0, 0, 1);
    FUNC(GS_INIT3_D)();

    if (Ending_def_tbl[Ending_no - 1].b == 0)
    {
        FUNC(CD_READ)(BIO_TIM, pEnding_work_buf);

        G.Vram_cur.b.Cclut = 0x1A;
        G.Vram_cur.b.Ctpage = 0xF;
        FUNC(MAPPING_TIM)(pEnding_work_buf);
    }
    if (Ending_def_tbl[Ending_no - 1].b != 0)
    {
        FUNC(CD_READ)(STAFFDS_STF, pEnding_work_buf);
    }
    else
    {
        FUNC(CD_READ)(STAFFDS_STF, pEnding_work_buf);
    }
    rect.x = 0;
    rect.y = 0x1FF;
    rect.w = 0x100;
    rect.h = 1;
    FUNC(LOAD_IMAGE)(&rect, pEnding_work_buf);
    FUNC(DRAW_SYNC)(0);
    pEnding_work_buf += 0x200;

    G.Scenario.Movie_id = Ending_no + 0xE;
    G.Scenario.pMovie_buf = pEnding_str_buf;
    G.System_flg |= SYS_MOVIE;
    FUNC(TASK_SLEEP)(1);

    FUNC(GS_INIT_GRAPH2)(0x140, 0xF0, 0, 0, 0);
    FUNC(GS_INIT3_D)();

    FUNC(TASK_SLEEP)(1);

    Ending_reset();
    Ending_spr_clr();

    Ending_scrl_mode = Ending_def_tbl[Ending_no - 1].b;
    Ending_rno0 = Ending_scrl_mode ^ 1;
    Ending_rno1 = 0;
    Ending_cap_sts = 0;

    Ending_wait_cnt = 0;
    Ending_frm_cnt = 0;
    if (Ending_rno0 == 1)
    {
        Ending_bg_set();
    }

    Ending_unk_c8 = 0;
    Ending_unk_cc = 0;
    Ending_unk_d0 = 0;

    Ending_pic_sts = 0;
    Ending_unk_d8 = 0;
    Ending_unk_dc = 0;

    Ending_scrl_pos = 0;
    Ending_scrl_old = 0;

    Ending_scrl_lim = 0x0E100000;

    G.System_flg |= SYS_BG_MODE_CLEAR;
    FUNC(SET_MOVIE_VOLUME2)(0x80);

    FUNC(CD_SET_STEREO)(((G.SaveData.Key_idx >> 7) ^ 1) & 0xFF);

    FUNC(SS_SET_SERIAL_ATTR)(0, 0, 1);

str_start:
    Str_sts = FUNC(OPEN_MOVIE)(STFR_STR, 2, 0xF73, pEnding_str_buf, 0x1E, pEnding_str_buf + 0xF000, pEnding_str_buf + 0x1D000, pEnding_str_buf + 0x2B000, 0);

    if (Str_sts == 1)
    {
        FUNC(CLOSE_MOVIE)();
        goto str_start;
    }

    Ending_scrl_pos = 0;
    Ending_scrl_ofs = 0;

    Ending_cap_end_flg = 0;

    Ending_str_end_flg = 0;
    while (1)
    {
        if (Ending_str_end_flg == 0 && Ending_frm_cnt >= 0x3DE1U)
        {
            FUNC(SET_MOVIE_VOLUME2)(0);
            Ending_str_end_flg = 1;
        }
        switch (Ending_rno0)
        {
        case 0:
            if (Ending_scrl_pos <= 0x0E100000U && Ending_frm_cnt >= 0x12CU)
            {
                if (Ending_scrl_pos - Ending_scrl_old > 0xFFFFFU)
                {
                    Ending_spr_get();
                    Ending_strip_set();
                    Ending_scrl_old = Ending_scrl_pos;
                }

                Ending_scrl_pos += 0x4000;
            }

            if (Ending_frm_cnt >= 0x3B4CU)
            {
                for (i = 0; i < 0x15U; i++)
                {
                    if (Ending_spr_wk[i].active != 0 && Ending_spr_wk[i].type == 0)
                    {
                        Ending_spr_wk[i].type = 1;
                    }
                }
            }

            if (Ending_frm_cnt == 0x3D0E)
            {
                for (i = 0; i < 0x15U; i++)
                {
                    if (Ending_spr_wk[i].active != 0)
                    {
                        Ending_spr_wk[i].Fade_spd = -0x800;
                    }
                }
            }

            if (Ending_frm_cnt >= 0x3DE1U)
            {
                Ending_rno0 = 0x63;
                Ending_rno1 = 0;
                Ending_time_flg = 0;
                Ending_spr_clr();
            }

            if ((G.Padd1_trg & (PAD_TRIANGLE | PAD_CIRCLE | PAD_X | PAD_SQUARE | PAD_SELECT | PAD_START)) && Ending_frm_cnt >= 0x12DU)
            {
                Ending_rno0 = 0x63;
            }

            Ending_frm_cnt++;
            break;
        case 1:
            FUNC(DRAW_SYNC)(0);
            if (Ending_str_end_flg == 0)
            {
                Ending_str_end_flg = FUNC(PLAY_MOVIE)(0x240, 0);
                FUNC(STR_DECODE_WAIT)();
                Ending_str_frm = FUNC(STR_FRM_GET)();
                if (Ending_str_end_flg != 0)
                {
                    FUNC(SET_MOVIE_VOLUME2)(0);

                    Ending_rno0 = 2;
                    Ending_rno1 = 0;
                    Ending_time_flg = 0;
                    Ending_spr_clr();
                }
            }

            Ending_cap_main();
            Ending_nop();
            Ending_pic_main();

            if ((G.Padd1_trg & (PAD_TRIANGLE | PAD_CIRCLE | PAD_X | PAD_SQUARE | PAD_SELECT | PAD_START)) && Ending_str_frm >= 0x4CU)
            {
                Ending_rno0 = 0x63;
            }

            if (Ending_str_frm >= 0x691U)
            {
                Ending_rno0 = 2;
                Ending_rno1 = 0;
                Ending_time_flg = 0;
                Ending_spr_clr();
            }
            Ending_frm_cnt += 4;
            break;
        case 2:
            Ending_zoom_main();
            Ending_frm_cnt += 2;
            break;
        case 3:
        default:
            break;
        }
        Ending_spr_draw();
        FUNC(TASK_SLEEP)(1);
        if (Ending_rno0 >= 4U)
        {
            break;
        }
    }
    FUNC(CLOSE_MOVIE)();
    FUNC(SS_SET_SERIAL_ATTR)(0, 0, 0);

    if (Ending_def_tbl[Ending_no - 1].a == 1)
    {
        FUNC(TASK_SLEEP)(0x96);

        rect.x = 0;
        rect.y = G.Cbuf_id != 0 ? 0xF0 : 0;
        rect.w = 0x1E0;
        rect.h = 0xF0;
        FUNC(VSYNC)(0);
        FUNC(CLEAR_IMAGE)(&rect, 0, 0, 0);
        FUNC(DRAW_SYNC)(0);
        FUNC(TASK_SLEEP)(1);
        rect.y = G.Cbuf_id != 0 ? 0xF0 : 0;
        FUNC(VSYNC)(0);
        FUNC(CLEAR_IMAGE)(&rect, 0, 0, 0);
        FUNC(DRAW_SYNC)(0);
        FUNC(VSYNC)(0);
        FUNC(GS_INIT_GRAPH2)(0x140, 0xF0, 0, 0, 1);
        FUNC(GS_INIT3_D)();
        G.Scenario.Movie_id = 0xE + 8;
        G.Scenario.pMovie_buf = pEnding_str_buf;
        G.System_flg |= SYS_MOVIE;
        FUNC(TASK_SLEEP)(1);
    }

    FUNC(GS_INIT_GRAPH2)(0x140, 0xF0, 0, 0, 0);
    FUNC(GS_INIT3_D)();
    FUNC(TASK_SLEEP)(1);
    Ending_rno0 = 0;
    Ending_rno1 = 0;
    while (Ending_rno0 == 0)
    {
        Ending_epi_main();
        Ending_spr_draw();
        FUNC(TASK_SLEEP)(1);
    }

    Ending_rocket_flg = 0;
    for (i = 0; i < G.SaveData.Item_num; i++)
    {
        if (ITEM_WORK_TBL[i].Id == 0x38)
        {
            Ending_rocket_flg++;
            break;
        }
    }
    i = 0;
    for (; i < 0x30U; i++)
    {
        if (ITEM_BOXWORK_CUR[i].Id == 0x38)
        {
            Ending_rocket_flg++;
            break;
        }
    }

    if (Ending_sts_bak & 0x20000)
    {
        Ending_rocket_flg++;
    }

    if (Ending_def_tbl[Ending_no - 1].c == 1 && Ending_rocket_flg == 0)
    {
        Ending_wait_cnt = 0;
        Ending_rno0 = 0;
        Ending_rno1 = 0;
        Ending_cap_sts = 0;
        while (Ending_rno0 == 0)
        {
            Ending_res_main();
            FUNC(TASK_SLEEP)(1);
        }

        Ending_rocket_flg++;
    }
    FUNC(TASK_SLEEP)(1);

    Ending_flg_7a = 0;

    Ending_flg_7e = 0;
    if (FUNC(FLG_CK)(&MAIN.SaveData.Scenario_flg, 0x7E) != 0)
    {
        Ending_flg_7e = 1;
    }

    if (FUNC(FLG_CK)(&MAIN.SaveData.Scenario_flg, 0x7A) != 0)
    {
        Ending_flg_7a = 1;
    }

    FUNC(CD_READ)(BIO_CARD_DAT, pEnding_work_buf);

    pEnding_src = (INT*)pEnding_work_buf;  // word view of the same memory
    pEnding_dst = (INT*)&MAIN.SaveData.Card_Head.magic;  // word view of the same memory
    for (i = 0; i < 0x107U; i++)
    {
        *pEnding_dst++ = *pEnding_src++;
    }

    Ending_spr_clr();

    G.SaveData.Save_cnt = 0;
    G.SaveData.Stage_no = 0;

    G.Player.Sca_info = 0;
    G.Player.Life = G.SaveData.Pl_life = 0x8C - (G.Player.Id & 1) * 0x2C;

    if (Ending_sts_bak & 0x40000)
    {
        G.SaveData.Pl_life = G.Player.Life = 0xB4 - (G.Player.Id & 1) * 0x1E;
        *(INT*)G.SaveData.Item_boxwork = 0x1E0B1E0B;  // INT* view of the same memory
    }

    if (Ending_sts_bak & 0x20000)
    {
        G.SaveData.Pl_life = G.Player.Life = 0x64 - (G.Player.Id & 1) * 0x1E;

        G.Status_flg |= STAT_ARRANGE;

        if (Ending_sts_bak & 0x10000)
        {
            *(INT*)G.SaveData.Item_boxwork = 0x1E0B1E0B;  // INT* view of the same memory
        }
    }

    FUNC(STATUS_INIT)();

    G.Status_flg &= ~STAT_ARRANGE;

    Inv_idx = G.SaveData.Item_num;
    if (Ending_time_flg != 0 || Ending_flg_7e != 0)
    {
        FUNC(FLG_ON)(&MAIN.SaveData.Scenario_flg, 0x7E);
        ITEM_WORK_TBL[Inv_idx].Id = 0xA;
        ITEM_WORK_TBL[Inv_idx].Num = 1;
        Inv_idx++;

        G.SaveData.Item_num++;
    }

    if (Ending_sts_bak & 0x20000)
    {
        if (Ending_no == 6 || Ending_no == 7 || Ending_flg_7a != 0)
        {
            FUNC(FLG_ON)(&MAIN.SaveData.Scenario_flg, 0x7A);
            ITEM_WORK_TBL[Inv_idx].Id = 5;
            ITEM_WORK_TBL[Inv_idx].Num = 1;
            G.SaveData.Item_num++;
        }
    }
    else if (Ending_rocket_flg != 0)
    {
        ITEM_WORK_TBL[Inv_idx].Id = 0x38;
        ITEM_WORK_TBL[Inv_idx].Num = 1;

        G.SaveData.Item_num++;
    }

    FUNC(FLG_ON)(&MAIN.SaveData.Scenario_flg, 0x7B);

    if (Ending_sts_bak & 0x40000)
    {
        G.SaveData.Dc_mode = 1;
    }
    if (Ending_sts_bak & 0x20000)
    {
        G.SaveData.Dc_mode = 2;
    }
    if (Ending_sts_bak & 0x10000)
    {
        G.SaveData.Dc_mode = 3;
    }
    Sts_bak = G.Status_flg;
    G.Status_flg = Ending_sts_bak;

    FUNC(CARD_ACCESS)(0, pEnding_work_buf, 0, 1);

    G.Status_flg = Sts_bak;

    G.System_flg = (G.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;

    G.Vloop = 2;
    FUNC(SND_SYS_END)();

    FUNC(TASK_LCHAIN)(LOGO_EXE);
}

VOID Ending_reset(VOID)
{
    INT unused[4];
    Ending_final_shot_step = 0;
    G.Vloop = 0;
    Ending_blk_adr = (INT)pEnding_work_buf;  // signed conversion (unsigned operand)
    Ending_row_no = 0;
}

VOID Ending_bg_set(VOID)
{
    Ending_spr_get();
    pEnding_bg_a = pEnding_spr_new;
    pEnding_bg_a->active = 1;
    pEnding_bg_a->type = 1;
    pEnding_bg_a->Ot_pri = 4;
    pEnding_bg_a->x = 0;
    pEnding_bg_a->y = 0;
    pEnding_bg_a->Fade_val = 0;
    pEnding_bg_a->Fade_spd = 0;
    pEnding_bg_a->Tex_flg = 0x02000000;
    pEnding_bg_a->Tex_w = 0x100;
    pEnding_bg_a->Tex_h = 0xF0;
    pEnding_bg_a->tex_page = FUNC(GET_TPAGE)(2, 0, 0x240, 0);
    pEnding_bg_a->Tex_u = 0;
    pEnding_bg_a->Tex_v = 0;
    Ending_spr_get();
    pEnding_bg_b = pEnding_spr_new;
    pEnding_bg_b->active = 1;
    pEnding_bg_b->type = 1;
    pEnding_bg_b->Ot_pri = 3;
    pEnding_bg_b->x = 0x01000000;
    pEnding_bg_b->y = 0;
    pEnding_bg_b->Fade_val = -0x8000;
    pEnding_bg_b->Fade_spd = 0;
    pEnding_bg_b->Tex_flg = 0x02000000;
    pEnding_bg_b->Tex_w = 0x40;
    pEnding_bg_b->Tex_h = 0xF0;
    pEnding_bg_b->tex_page = FUNC(GET_TPAGE)(2, 0, 0x340, 0);
    pEnding_bg_b->Tex_u = 0;
    pEnding_bg_b->Tex_v = 0;
}

VOID Ending_spr_get(VOID)
{
    USHORT i;
    USHORT found;

    for (i = 0; i < 0x15U; i++)
    {
        if (Ending_spr_wk[i].active == 0)
        {
            pEnding_spr_new = &Ending_spr_wk[i];

            found = i;

            return;
        }
        pEnding_spr_new = 0;
    }
}

VOID Ending_spr_clr(VOID)
{
    USHORT i;

    for (i = 0; i < 0x15U; i++)
    {
        Ending_spr_wk[i].active = 0;
    }
}

VOID Ending_strip_set(VOID)
{
    USHORT Rect_x;
    USHORT Rect_y;
    SHORT Rect_w;
    SHORT Rect_h;
    USHORT unused;

    pEnding_spr_new->active = 1;
    pEnding_spr_new->type = 0;
    pEnding_spr_new->Ot_pri = 3;
    pEnding_spr_new->Vram_x = 0x180U;
    pEnding_spr_new->Vram_y = Ending_row_no * 0x10;
    pEnding_spr_new->x = 0x200000;

    pEnding_spr_new->y = (0x100 - ((Ending_scrl_pos >> 0x10) & 0xF)) << 0x10;

    pEnding_spr_new->Fade_val = -0x8000;
    pEnding_spr_new->Fade_spd = 0;

    pEnding_spr_new->Tex_flg = 0x01000000;

    pEnding_spr_new->Tex_w = 0x100;
    pEnding_spr_new->Tex_h = 0x10;
    pEnding_spr_new->clut_x = 0;
    pEnding_spr_new->clut_y = 0x1FF;
    pEnding_spr_new->tex_page = FUNC(GET_TPAGE)(0, 0, pEnding_spr_new->Vram_x, pEnding_spr_new->Vram_y);
    pEnding_spr_new->Tex_u = 0;
    pEnding_spr_new->Tex_v = pEnding_spr_new->Vram_y;
    Rect_x = pEnding_spr_new->Vram_x;
    Rect_y = pEnding_spr_new->Vram_y;
    Rect_w = 0x80;
    Rect_h = 0x10;
    Ending_blk_set();
    FUNC(LOAD_IMAGE)(&Rect_x, Ending_img_adr);
    if (++Ending_row_no >= 0x12)
    {
        Ending_row_no = 0;
    }
}

VOID Ending_blk_set(VOID)
{
    INT Rsv;
    USHORT n;
    INT Rsv2;
    SCHAR* pDst;

    pEnding_blk = (ENDING_TEXT_HEAD*)Ending_blk_adr;  // reinterprets the memory as ENDING_TEXT_HEAD
    pEnding_run = (USHORT*)((UCHAR*)Ending_blk_adr + pEnding_blk->Run_ofs);  // halfword view of the same memory
    pEnding_val = (UCHAR*)Ending_blk_adr + pEnding_blk->Val_ofs;  // byte view of the same memory
    Ending_blk_adr += pEnding_blk->size;
    if (pEnding_blk->Val_ofs == 0xFFFF)
    {
        Ending_img_adr = (INT)pEnding_run;  // signed conversion (unsigned operand)
        return;
    }
    pDst = pEnding_strip_buf;
    while (*pEnding_run != 0)
    {
        n = 0;
        while (n < *pEnding_run)
        {
            *pDst++ = *pEnding_val;
            n++;
        }
        pEnding_run++;
        pEnding_val++;
    }
    Ending_img_adr = (INT)pEnding_strip_buf;  // signed conversion (unsigned operand)
    return;
}

VOID Ending_nop(VOID)
{
}

VOID Ending_pic_main(VOID)
{
    switch (Ending_pic_sts)
    {
    case 0:
        pEnding_pic_rec = Ending_pic_tbl[Ending_set_no];
        Ending_pic_sts = 1;
        Ending_spr_get();
        pEnding_pic = pEnding_spr_new;
        pEnding_pic->active = 1;
        pEnding_pic->type = 7;
        pEnding_pic->Fade_val = 0;
        pEnding_pic->Fade_spd = 0;

        pEnding_pic->Box_spd_x = 0U;
        pEnding_pic->Box_spd_y = 0U;

        Ending_box_a.attribute = 0;

        Ending_box_a.x = -0xA0;

        Ending_box_a.y = -0xF0;

        Ending_box_a.w = 0x140;
        Ending_box_a.h = 0x78;

        Ending_box_a.b = 0x80;
        Ending_box_a.g = 0x80;
        Ending_box_a.r = 0x80;
        Ending_box_b.attribute = 0;
        Ending_box_b.x = -0x140;

        Ending_box_b.y = -0x78;
        Ending_box_b.w = 0xA0;
        Ending_box_b.h = 0xF0;
        Ending_box_b.b = 0;
        Ending_box_b.g = 0;
        Ending_box_b.r = 0;

    case 1:
        if (Ending_str_frm >= pEnding_pic_rec->Show_frm)
        {
            pEnding_pic->Fade_spd = 0;

            pEnding_pic->Fade_val = 0;
            pEnding_pic->Box_spd_x = 0U;
            pEnding_pic->Box_spd_y = 0U;

            Ending_box_a.y = pEnding_pic_rec->Box_y - 0x78;
            Ending_box_b.x = pEnding_pic_rec->Box_x - 0xA0;
            Ending_pic_sts = 2;
        }
        return;
    case 2:
        if (Ending_str_frm >= pEnding_pic_rec->Move_frm)
        {
            pEnding_pic->Box_spd_x = pEnding_pic_rec->Box_spd_x;
            pEnding_pic->Box_spd_y = pEnding_pic_rec->Box_spd_y;

            pEnding_pic_rec++;
            if (pEnding_pic_rec->Show_frm == 0xFFFF)
            {
                Ending_pic_sts = 0x63;
            }
            else
            {
                Ending_pic_sts = 1;
            }
        }
        return;
    }
    return;
}

VOID Ending_cap_main(VOID)
{
    RECT rect;
    USHORT i;

    switch (Ending_rno1)
    {
    case 0:
        Ending_cap_end_flg = 0;
        Ending_row_no = 0;
        Ending_cap_no = 0;

        pEnding_cap = Ending_cap_tbl[Ending_set_no];

        Ending_cap_set();
        pEnding_txt->x = pEnding_cap->Pos_x << 0x10;
        Ending_rno1++;
        Ending_cap_sts = 0;
        i = 0;
        while (i < 0xFU)
        {
            Ending_blk_set();
            rect.x = Ending_cap_no * 0x80 + 0x140;
            rect.y = Ending_row_no * 0x10;
            rect.w = 0x80;
            rect.h = 0x10;
            FUNC(LOAD_IMAGE)(&rect, Ending_img_adr);
            FUNC(DRAW_SYNC)(0);
            Ending_row_no++;
            i++;
        }

    case 1:
        if (Ending_row_no < 0xFU && Ending_cap_end_flg == 0)
        {
            Ending_blk_set();
            rect.x = Ending_cap_no * 0x80 + 0x140;
            rect.y = Ending_row_no * 0x10;
            rect.w = 0x80;
            rect.h = 0x10;
            FUNC(LOAD_IMAGE)(&rect, Ending_img_adr);
            Ending_row_no++;
        }
        switch (Ending_cap_sts)
        {
        case 0:
            if (Ending_str_frm >= pEnding_cap->Start_frame)
            {
                Ending_cap_no ^= 1;
                pEnding_txt->tex_page = FUNC(GET_TPAGE)(1, 0, ((Ending_cap_no ^ 1) & 0xFF) * 0x80 + 0x140, 0);
                pEnding_txt->x = pEnding_cap->Pos_x << 0x10;
                pEnding_txt->Fade_val = 0U;
                pEnding_txt->Fade_spd = 0x1100;
                if (pEnding_cap[1].Start_frame == 0xFFFF)
                {
                    Ending_cap_end_flg = 1;
                }
                else
                {
                    Ending_row_no = 0;
                }
                Ending_cap_sts++;
            }
            break;
        case 1:
            if ((USHORT)pEnding_txt->Fade_val == 0x8000)  // truncates to an unsigned halfword
            {
                pEnding_txt->Tex_flg &= 0xBFFFFFFF;
            }

            if (Ending_str_frm >= pEnding_cap->Hide_frm)
            {
                pEnding_txt->Tex_flg |= 0x40000000;

                pEnding_txt->Fade_spd = -0x1100;
                pEnding_cap++;
                if (Ending_cap_end_flg != 0)
                {
                    Ending_rno1 = 0x63;
                }
                else
                {
                    Ending_cap_sts = 0;
                }
            }
            break;
        }
        return;
    }
    return;
}

VOID Ending_cap_set(VOID)
{
    RECT rect;

    Ending_spr_get();
    pEnding_txt = pEnding_spr_new;
    pEnding_txt->active = 1;
    pEnding_txt->type = 2;
    pEnding_txt->x = 0;
    pEnding_txt->y = 0;
    pEnding_txt->Fade_val = 0;
    pEnding_txt->Fade_spd = 0;

    pEnding_txt->Tex_flg = 0x51000000;

    pEnding_txt->Tex_w = 0x100;
    pEnding_txt->Tex_h = 0xF0;
    pEnding_txt->tex_page = FUNC(GET_TPAGE)(1, 0, ((Ending_cap_no ^ 1) & 0xFF) * 0x80 + 0x140, 0);
    pEnding_txt->Tex_u = 0;
    pEnding_txt->Tex_v = 0;
    pEnding_txt->clut_x = 0;
    pEnding_txt->clut_y = 0x1FF;
    rect.x = 0x140;
    rect.y = 0;
    rect.w = 0x100;
    rect.h = 0xF0;
    FUNC(CLEAR_IMAGE)(&rect, 0, 0, 0);
    FUNC(DRAW_SYNC)(0);
}

VOID Ending_spr_draw(VOID)
{
    USHORT i;

    pEnding_spr_cur = Ending_spr_wk;
    i = 0;
    while (i < 0x15U)
    {
        if (pEnding_spr_cur->active != 0)
        {
            pEnding_spr_cur->Scr_x = (pEnding_spr_cur->x >> 0x10) - 0xA0;
            pEnding_spr_cur->Scr_y = (pEnding_spr_cur->y >> 0x10) - 0x78;
            if (pEnding_spr_cur->Fade_spd != 0)
            {
                pEnding_spr_cur->Fade_val += pEnding_spr_cur->Fade_spd;
                if (pEnding_spr_cur->Fade_val < 0)
                {
                    if (pEnding_spr_cur->Fade_spd > 0)
                    {
                        pEnding_spr_cur->Fade_val = -0x8000;
                    }
                    else
                    {
                        pEnding_spr_cur->Fade_val = 0U;
                    }
                    pEnding_spr_cur->Fade_spd = 0;
                }
            }
            pEnding_spr_cur->Col_r = pEnding_spr_cur->Fade_val >> 8;
            pEnding_spr_cur->Col_g = pEnding_spr_cur->Fade_val >> 8;
            pEnding_spr_cur->Col_b = pEnding_spr_cur->Fade_val >> 8;
            switch (pEnding_spr_cur->type)
            {
            case 0:
                pEnding_spr_cur->Scr_x += Ending_scrl_ofs;
                if (pEnding_spr_cur->Fade_val != 0)
                {
                    FUNC(GS_SORT_FAST_SPRITE)(&pEnding_spr_cur->Tex_flg, &MAIN.Ot0[G.Cbuf_id], pEnding_spr_cur->Ot_pri);
                }

                pEnding_spr_cur->y -= 0x4000;

                if (pEnding_spr_cur->y < -0x100000)
                {
                    pEnding_spr_cur->active = 0U;
                }
                break;
            case 1:
                if (pEnding_spr_cur->Fade_val != 0)
                {
                    FUNC(GS_SORT_FAST_SPRITE)(&pEnding_spr_cur->Tex_flg, &MAIN.Ot2[G.Cbuf_id], 0xFU);
                }
                break;
            case 2:
                if (pEnding_spr_cur->Fade_val != 0)
                {
                    FUNC(GS_SORT_FAST_SPRITE)(&pEnding_spr_cur->Tex_flg, &MAIN.Ot0[G.Cbuf_id], 2U);
                }
                break;
            case 3:
                pEnding_spr_cur->tex_page = 0x1E;

                pEnding_spr_cur->Tex_w = 8;

                pEnding_spr_cur->Tex_h = 0xE;
                pEnding_spr_cur->clut_x = 0x100;
                pEnding_spr_cur->clut_y = 0x1E0;

                FUNC(SPRINTF)(ScratchPad->Text_buf, "%02d:%02d%1c%02d", G.SaveData.Game_play_time / 216000, G.SaveData.Game_play_time % 216000 / 3600, 0x5E, G.SaveData.Game_play_time % 3600 / 60);

                if (pEnding_spr_cur->Fade_val != 0)
                {
                    Ending_text_draw();
                }
                break;
            case 5:
                pEnding_spr_cur->tex_page = 0x1E;

                pEnding_spr_cur->Tex_w = 8;

                pEnding_spr_cur->Tex_h = 0xE;
                pEnding_spr_cur->clut_x = 0x100;
                pEnding_spr_cur->clut_y = 0x1E0;
                if (G.SaveData.Save_cnt == 0)
                {
                    FUNC(SPRINTF)(ScratchPad->Text_buf, "%02d", G.SaveData.Save_cnt);
                }
                else
                {
                    FUNC(SPRINTF)(ScratchPad->Text_buf, "%02d", G.SaveData.Save_cnt - 1);
                }
                if (pEnding_spr_cur->Fade_val != 0)
                {
                    Ending_text_draw();
                }
                break;

            case 6:
                FUNC(GS_SORT_SPRITE)(&pEnding_spr_cur->Tex_flg, &MAIN.Ot0[G.Cbuf_id], 2U);
                break;
            case 7:
                Ending_box_a.r = pEnding_spr_cur->Fade_val >> 8;
                Ending_box_a.g = pEnding_spr_cur->Fade_val >> 8;
                Ending_box_a.b = pEnding_spr_cur->Fade_val >> 8;
                Ending_box_b.r = pEnding_spr_cur->Fade_val >> 8;
                Ending_box_b.g = pEnding_spr_cur->Fade_val >> 8;
                Ending_box_b.b = pEnding_spr_cur->Fade_val >> 8;

                Ending_box_a.y += pEnding_spr_cur->Box_spd_y;
                Ending_box_b.x += pEnding_spr_cur->Box_spd_x;
                if (Ending_box_b.x < -0x140 || Ending_box_b.x >= 0xA1)
                {
                    pEnding_spr_cur->Box_spd_x = 0U;
                    pEnding_spr_cur->Box_spd_y = 0U;
                }

                FUNC(GS_SORT_BOX_FILL)(&Ending_box_a, &MAIN.Ot2[G.Cbuf_id], 0xEU);
                FUNC(GS_SORT_BOX_FILL)(&Ending_box_b, &MAIN.Ot2[G.Cbuf_id], 0xEU);
                break;
            }
        }
        pEnding_spr_cur++;
        i++;
    }
}

VOID Ending_no_set(VOID)
{
    if (FUNC(FLG_CK)(&MAIN.SaveData.Scenario_flg2, 0xC0) != 0)  // address passed as an integer
    {
        if (FUNC(FLG_CK)(&MAIN.SaveData.Scenario_flg2, 0x4B) != 0)  // address passed as an integer
        {
            Ending_no = 3;
            return;
        }
        else if (!(G.Player.Id & 3))
        {
            Ending_no = 1;
            return;
        }
        else
        {
            Ending_no = 2;
            return;
        }
    }
    if (FUNC(FLG_CK)(&MAIN.SaveData.Scenario_flg2, 0x4B) == 0)  // address passed as an integer
    {
        if (!(G.Player.Id & 3))
        {
            Ending_no = 4;
            return;
        }
        else
        {
            Ending_no = 5;
            return;
        }
    }
    if (!(G.Player.Id & 3))
    {
        Ending_no = 6;
        return;
    }
    else
    {
        Ending_no = 7;
        return;
    }
}

VOID Ending_epi_main(VOID)
{
    switch (Ending_rno1)
    {
    case 0:
        G.Stop_flg |= STOP_ESP_FX;

        G.Vloop = 2;

        Ending_scrl_mode = 1;
        Ending_wait_cnt = 0;
        Ending_spr_clr();
        Ending_bg_set();
        pEnding_bg_a->Fade_val = 0;
        pEnding_bg_a->Fade_spd = 0x800;
        pEnding_bg_b->Fade_val = 0;
        pEnding_bg_b->Fade_spd = 0x800;
        Ending_epi_set();

        Ending_rno1 = 1;

        break;
    case 1:
        Ending_wait_cnt++;

        if (Ending_wait_cnt >= 0x3DU)
        {
            Ending_wait_cnt = 0;

            Ending_rno1 = 2;

            Ending_lbl_tbl[0]->active = 1;
            Ending_lbl_tbl[0]->Fade_spd = 0x800;
            Ending_lbl_tbl[1]->active = 1;
            Ending_lbl_tbl[1]->Fade_spd = 0x800;
        }
        break;
    case 2:
        Ending_wait_cnt++;

        if ((USHORT)Ending_lbl_tbl[0]->Fade_val == 0x8000)  // truncates to an unsigned halfword
        {
        }
        if (Ending_wait_cnt >= 0x3DU)
        {
            Ending_wait_cnt = 0;

            Ending_rno1 = 3;

            Ending_lbl_tbl[2]->active = 1;
            Ending_lbl_tbl[2]->Fade_spd = 0x800;
            Ending_lbl_tbl[3]->active = 1;
            Ending_lbl_tbl[3]->Fade_spd = 0x800;
        }
        break;

    case 3:
        Ending_wait_cnt++;

        if ((USHORT)Ending_lbl_tbl[2]->Fade_val == 0x8000)  // truncates to an unsigned halfword
        {
        }

        if (Ending_wait_cnt >= 0x5BU)
        {
            Ending_wait_cnt = 0;

            Ending_rno1 = 4;
        }
        break;
    case 4:
        Ending_wait_cnt++;
        if (Ending_wait_cnt < 0x709U && !(G.Padd1_trg & (PAD_TRIANGLE | PAD_CIRCLE | PAD_X | PAD_SQUARE | PAD_SELECT | PAD_START)))
        {
        }
        else
        {
            Ending_wait_cnt = 0;
            Ending_rno1 = 5;
            if (Ending_no == 6)
            {
                Ending_rno1 = 6;
            }
        }
        break;
    case 5:
        Ending_wait_cnt = 0;
        Ending_rno1 = 0xA;
        pEnding_bg_a->Fade_spd = -0x800;
        pEnding_bg_b->Fade_spd = -0x800;
        Ending_lbl_tbl[0]->Fade_spd = -0x800;
        Ending_lbl_tbl[1]->Fade_spd = -0x800;
        Ending_lbl_tbl[2]->Fade_spd = -0x800;
        Ending_lbl_tbl[3]->Fade_spd = -0x800;
        break;
    case 6:
        Ending_wait_cnt = 0;
        Ending_rno1 = 7;
        Ending_lbl_tbl[0]->Fade_spd = -0x800;
        Ending_lbl_tbl[1]->Fade_spd = -0x800;
        Ending_lbl_tbl[2]->Fade_spd = -0x800;
        Ending_lbl_tbl[3]->Fade_spd = -0x800;
        break;
    case 7:
        Ending_wait_cnt++;
        if (Ending_wait_cnt >= 0x5BU)
        {
            Ending_lbl_tbl[4]->active = 1;
            Ending_lbl_tbl[4]->Fade_spd = 0x800;
            Ending_lbl_tbl[4]->Tex_flg |= 0x40000000;
            Ending_wait_cnt = 0;
            Ending_rno1 = 8;
        }
        break;
    case 8:
        Ending_wait_cnt++;
        if ((USHORT)Ending_lbl_tbl[4]->Fade_val == 0x8000)  // truncates to an unsigned halfword
        {
            Ending_lbl_tbl[4]->Tex_flg &= 0xBFFFFFFF;
        }

        if (Ending_wait_cnt < 0x97U && !(G.Padd1_trg & (PAD_TRIANGLE | PAD_CIRCLE | PAD_X | PAD_SQUARE | PAD_SELECT | PAD_START)))
        {
        }
        else
        {
            Ending_wait_cnt = 0;
            Ending_rno1 = 9;
            pEnding_bg_a->Fade_spd = -0x800;
            pEnding_bg_b->Fade_spd = -0x800;
        }
        break;
    case 9:
        Ending_wait_cnt++;

        if (Ending_wait_cnt < 0x97U && !(G.Padd1_trg & (PAD_TRIANGLE | PAD_CIRCLE | PAD_X | PAD_SQUARE | PAD_SELECT | PAD_START)))
        {
        }
        else
        {
            Ending_wait_cnt = 0;
            Ending_rno1 = 0xA;
            Ending_lbl_tbl[4]->Fade_spd = -0x800;
            Ending_lbl_tbl[4]->Tex_flg |= 0x40000000;
        }
        break;
    case 10:
        Ending_wait_cnt++;
        if (Ending_wait_cnt >= 0x1FU)
        {
            Ending_time_set();

            Ending_esp_pos.vx = 0;
            Ending_esp_pos.vy = 0;
            Ending_esp_pos.vz = 0;

            pEnding_bg_a->active = 0;
            pEnding_bg_b->active = 0;
            Ending_lbl_tbl[0]->active = 0;
            Ending_lbl_tbl[1]->active = 0;
            Ending_lbl_tbl[2]->active = 0;
            Ending_lbl_tbl[3]->active = 0;

            Ending_lbl_tbl[4]->active = 0;

            if (Ending_time_flg != 0)
            {
                FUNC(ESP_CALL)(0xB, 0xA, 0, 0, &Ending_esp_pos, 0);

                FUNC(SND_SE_PLAY)(1, 0, 0);
                FUNC(SND_SE_PLAY)(1, 1, 0);
            }

            Ending_rno1 = 0xB;

            Ending_wait_cnt = 0;
        }
        break;

    case 11:
        Ending_wait_cnt++;
        if (Ending_wait_cnt >= 0x33U)
        {
            FUNC(ESP_KILL)(3, 0xB, 0xA, 0, 0);

            Ending_rno1 = 0xC;
        }
        break;

    case 12:
        Ending_wait_cnt = 0;
        Ending_rno0 = 0x63;
        break;
    }
    if (Ending_time_flg != 0)
    {
        FUNC(ESP_MOVE)();
    }
}

VOID Ending_epi_set(VOID)
{
    USHORT i;
    USHORT first;
    USHORT count;
    UINT Hour;
    RECT rect;

    if (Ending_def_tbl[Ending_no - 1].d != 0)
    {
        first = 0;
        count = 5;
    }
    else
    {
        first = 7;
        count = 5;
    }
    i = 0;
    while (i < count)
    {
        Ending_spr_get();
        pEnding_spr_new->active = 1;

        pEnding_spr_new->type = Ending_spr_tbl[first + i].type;

        pEnding_spr_new->Ot_pri = 3;

        pEnding_spr_new->x = Ending_spr_tbl[first + i].x << 0x10;
        if ((G.Player.Id & 3) && Ending_def_tbl[Ending_no - 1].d != 0)
        {
            pEnding_spr_new->x += 0x9C0000;
        }
        pEnding_spr_new->y = Ending_spr_tbl[first + i].y << 0x10;

        pEnding_spr_new->Fade_spd = 0;
        pEnding_spr_new->Fade_val = 0;

        pEnding_spr_new->Tex_flg = 0x10000000;

        pEnding_spr_new->Tex_u = 0;

        pEnding_spr_new->Tex_v = Ending_spr_tbl[first + i].Tex_v;

        pEnding_spr_new->Tex_w = 0x100;

        pEnding_spr_new->Tex_h = Ending_spr_tbl[first + i].Tex_h;

        pEnding_spr_new->clut_x = 0;
        pEnding_spr_new->clut_y = 0x1FB;
        pEnding_spr_new->tex_page = 0x1B;

        if (pEnding_spr_new->type == 3)
        {
            for (Hour = G.SaveData.Game_play_time / 216000; Hour / 100 != 0; Hour /= 10)
            {
                pEnding_spr_new->x -= 0x40000;
            }
        }

        Ending_lbl_tbl[i] = pEnding_spr_new;

        i++;
    }

    i = 0;
    while (i < count)
    {
        Ending_lbl_tbl[i]->active = 0;
        i++;
    }
    Ending_view.vpx = 0x5DC;

    Ending_view.vpy = -0x5DC;
    Ending_view.vpz = -0x5DC;

    Ending_view.vrx = 0x1770;

    Ending_view.vry = 0;
    Ending_view.vrz = 0;
    Ending_view.rz = 0;
    Ending_view.super = 0;
    FUNC(GS_SET_PROJECTION)(0x82);
    FUNC(GS_SET_REF_VIEW2)(&Ending_view);
    G.SaveData.Cut_no = 0;
    G.Cccut = Ending_cam_tbl;
    G.pRoom = (ROOM_WORK*)&Ending_room_head;  // reinterprets the memory as ROOM_WORK
    G.pRoom->Cut[G.SaveData.Cut_no].View = Ending_view;
    G.pRoom->Cut[G.SaveData.Cut_no].ViewR = 0x82;
    rect.x = 0x240;
    rect.y = 0;
    rect.w = 0x140;
    rect.h = 0xF0;
    FUNC(DRAW_SYNC)(0);

    if (Ending_def_tbl[Ending_no - 1].d != 0)
    {
        FUNC(LOAD_IMAGE)(&rect, pEnding_pl_pic);
        return;
    }
    FUNC(CLEAR_IMAGE)(&rect, 0, 0, 0);
}

VOID Ending_time_set(VOID)
{
    if (G.SaveData.Game_play_time <= 0x9E340U)
    {
        Ending_time_flg = 1;
        G.pRoom->pSca = &Ending_sca_tbl;
        return;
    }
    Ending_time_flg = 0;
}

VOID Ending_zoom_main(VOID)
{
    switch (Ending_rno1)
    {
    case 0:
        G.Vloop = 0;
        Ending_spr_clr();
        Ending_spr_get();
        pEnding_spr_new->active = 1;
        pEnding_spr_new->type = 6;
        pEnding_spr_new->x = 0xA00000;
        pEnding_spr_new->y = 0x780000;
        pEnding_spr_new->Fade_val = 0;
        pEnding_spr_new->Fade_spd = 0x300;
        pEnding_spr_new->Tex_flg = 0;
        pEnding_spr_new->Tex_w = 0x100;
        pEnding_spr_new->Tex_h = 0x44;
        pEnding_spr_new->clut_x = 0;
        pEnding_spr_new->clut_y = 0x1FA;
        pEnding_spr_new->tex_page = 0xF;
        pEnding_spr_new->Tex_u = 0;
        pEnding_spr_new->Tex_v = 0;
        pEnding_spr_new->Pivot_x = 0x80;
        pEnding_spr_new->Pivot_y = 0x22;
        pEnding_spr_new->rotate = 0;
        pEnding_spr_new->Scale_x = -0x8000;
        pEnding_spr_new->Scale_y = -0x8000;
        pEnding_txt = pEnding_spr_new;
        Ending_rno1 = 1;
        Ending_zoom_z = 0x2000;
        Ending_zoom_spd = 0x300;
        Ending_zoom_add = -4;
        Ending_wait_cnt = 0;
        return;
    case 1:
        Ending_zoom_z += Ending_zoom_spd;
        Ending_zoom_spd += Ending_zoom_add;
        pEnding_txt->Scale_x = 0x10000000 / Ending_zoom_z;
        pEnding_txt->Scale_y = 0x10000000 / Ending_zoom_z;
        if (Ending_zoom_z > 0x97FF)
        {
            Ending_rno1 = 2;
        }
        Ending_wait_cnt++;
        return;
    case 2:
        Ending_zoom_z += Ending_zoom_spd;
        Ending_zoom_spd += Ending_zoom_add;
        pEnding_txt->Scale_x = 0x10000000 / Ending_zoom_z;
        pEnding_txt->Scale_y = 0x10000000 / Ending_zoom_z;
        if (Ending_zoom_z > 0xFFFF)
        {
            pEnding_txt->Fade_spd = -0x200;
            Ending_rno1 = 3;
        }
        Ending_wait_cnt++;
        return;
    case 3:
        Ending_zoom_z += Ending_zoom_spd;
        Ending_zoom_spd += Ending_zoom_add;
        pEnding_txt->Scale_x = 0x10000000 / Ending_zoom_z;
        pEnding_txt->Scale_y = 0x10000000 / Ending_zoom_z;
        if (Ending_zoom_z <= 0x177FF && Ending_zoom_spd > 0)
        {
        }
        else
        {
            pEnding_txt->active = 0;
            Ending_rno1 = 4;
        }
        Ending_wait_cnt++;
        return;
    case 4:
        Ending_wait_cnt++;
        if (Ending_wait_cnt >= 0x12CU)
        {
            Ending_rno0 = 0x63;
            Ending_wait_cnt = 0;
        }
        break;
    }
}

VOID Ending_res_main(VOID)
{
    INT Ls_m;
    INT unused[5];

    switch (Ending_rno1)
    {
    case 0:
        G.Vloop = 2;
        Ending_spr_clr();
        Ending_bg_set();

        pEnding_bg_a->Fade_val = 0x6000;
        pEnding_bg_b->Fade_val = 0x6000;

        Ending_res_set();
        G.SaveData.Fade_kido = -1;
        G.Fade_hrate = 2;
        G.Fade_add = -0x800;
        FUNC(FADE_START)();
        Ending_wait_cnt = 0;
        Ending_rno1++;

        FUNC(SND_SE_PLAY)(1, 2, 0);
        FUNC(SND_SE_PLAY)(1, 3, 0);

    case 1:
        Ending_wait_cnt++;

        if (Ending_wait_cnt >= 0x10EU || (G.Padd1_trg & (PAD_TRIANGLE | PAD_CIRCLE | PAD_X | PAD_SQUARE | PAD_SELECT | PAD_START)))
        {
            G.Fade_hrate = 2;
            G.Fade_add = 0x800;
            FUNC(FADE_START)();
            Ending_rno1++;
        }
        break;
    case 2:
        if (G.SaveData.Fade_kido >= 0x7681)
        {
            Ending_rno0++;
        }
        break;
    }
    Ending_spr_draw();
    Ending_rot.vz += 0x20;
    FUNC(ROT_MATRIX)(&Ending_rot, &Ending_coord1.coord);
    Ending_coord1.flg = 0;
    FUNC(GS_GET_LS)(Ending_obj.coord2, &Ls_m);
    FUNC(GS_SET_LIGHT_MATRIX)(&Ls_m);
    FUNC(GS_SET_LS_MATRIX)(&Ls_m);
    FUNC(GS_SORT_OBJECT5)(&Ending_obj, &MAIN.Ot1[G.Cbuf_id], 4, ScratchPad); // scratchpad address passed as an integer argument
}

VOID Ending_res_set(VOID)
{
    USHORT i;
    INT pad14[10];
    INT* pTmd;
    RECT rect;

    FUNC(CD_READ)(RC1121_PIX, pEnding_pl_pic);

    rect.x = 0x240;
    rect.y = 0;
    rect.w = 0x140;
    rect.h = 0xF0;
    FUNC(DRAW_SYNC)(0);
    FUNC(LOAD_IMAGE)(&rect, pEnding_pl_pic);

    i = 5;
    while (i < 7U)
    {
        Ending_spr_get();
        pEnding_spr_new->active = 1;
        pEnding_spr_new->type = Ending_spr_tbl[i].type;
        pEnding_spr_new->Ot_pri = 3;
        pEnding_spr_new->x = Ending_spr_tbl[i].x << 0x10;
        pEnding_spr_new->y = Ending_spr_tbl[i].y << 0x10;
        pEnding_spr_new->Fade_spd = 0;
        pEnding_spr_new->Fade_val = -0x8000;

        pEnding_spr_new->Tex_flg = 0;

        pEnding_spr_new->Tex_u = 0;
        pEnding_spr_new->Tex_v = Ending_spr_tbl[i].Tex_v;
        pEnding_spr_new->Tex_w = 0x100;
        pEnding_spr_new->Tex_h = Ending_spr_tbl[i].Tex_h;
        pEnding_spr_new->clut_x = 0;
        pEnding_spr_new->clut_y = 0x1FB;
        pEnding_spr_new->tex_page = 0x1B;
        Ending_lbl_tbl[i] = pEnding_spr_new;
        i++;
    }
    G.Vram_cur.b.Cclut = 0x1C;
    G.Vram_cur.b.Ctpage = 0x15;
    FUNC(MAPPING_TIM)(pEnding_model_head);
    FUNC(MAPPING_TMD)(2, pEnding_tmd, 0x15, 0x1C);
    FUNC(GS_INIT_COORDINATE2)(0, &Ending_coord0);
    FUNC(GS_INIT_COORDINATE2)(&Ending_coord0, &Ending_coord1);
    FUNC(GS_INIT_COORDINATE2)(&Ending_coord1, &Ending_coord2);
    Ending_view.vpx = 0x2EE0;
    Ending_view.vpy = 0;
    Ending_view.vpz = 0;
    Ending_view.vrx = 0;
    Ending_view.vry = 0;
    Ending_view.vrz = 0;
    Ending_view.rz = 0;
    Ending_view.super = 0;
    FUNC(GS_SET_REF_VIEW2)(&Ending_view);
    FUNC(GS_SET_PROJECTION)(0x1A4);
    Ending_light.vx = 0x64;
    Ending_light.vy = 0x64;
    Ending_light.vz = 0x64;
    Ending_light.r = 0xFF;
    Ending_light.g = 0xFF;
    Ending_light.b = 0xFF;
    FUNC(GS_SET_FLAT_LIGHT)(0, &Ending_light);
    FUNC(GS_SET_AMBIENT)(0xFFF, 0xFFF, 0xFFF);
    FUNC(GS_SET_LIGHT_MODE)(0);
    pTmd = (INT*)pEnding_tmd;  // word view of the same memory
    pTmd++;
    FUNC(GS_MAP_MODELING_DATA)(pTmd);
    pTmd += 2;
    FUNC(GS_LINK_OBJECT5)(pTmd, &Ending_obj, 0);
    Ending_obj.attribute = 0;
    Ending_obj.coord2 = &Ending_coord2;
    pEnding_pkt = (SHORT*)pEnding_pkt_buf;  // halfword view of the same memory
    pEnding_pkt = (SHORT*)FUNC(GS_PRESET_OBJECT)(&Ending_obj, pEnding_pkt);  // halfword view of the same memory
    Ending_rot.vx = 0xAA;
    Ending_rot.vy = 0;
    Ending_rot.vz = 0;
    FUNC(ROT_MATRIX)(&Ending_rot, &Ending_coord2.coord);
    Ending_coord2.flg = 0;
    Ending_rot.vx = 0;
    Ending_rot.vy = 0;
    Ending_rot.vz = 0;
    FUNC(ROT_MATRIX)(&Ending_rot, &Ending_coord1.coord);
    Ending_coord1.flg = 0;
    Ending_rot.vx = -0x6A4;
    Ending_rot.vy = 0;
    Ending_rot.vz = 0;
    FUNC(ROT_MATRIX)(&Ending_rot, &Ending_coord0.coord);
    Ending_coord0.coord.t[0] = 0;
    Ending_coord0.coord.t[1] = 0x1F4;
    Ending_coord0.coord.t[2] = 0;
    Ending_coord0.flg = 0;
    Ending_rot.vx = 0;
    Ending_rot.vy = 0;
    Ending_rot.vz = 0;
}

VOID Ending_text_draw(VOID)
{
    UCHAR ch;
    UCHAR* p;

    p = ScratchPad->Text_buf;
    for (;;)
    {
        ch = *p;
        if (ch != 0x20)
        {
            pEnding_spr_cur->Tex_u = ((*p % 18) & 0xFF) * 8;

            pEnding_spr_cur->Tex_v = ((*p / 18) & 0xFF) * 0xE;
            FUNC(GS_SORT_FAST_SPRITE)(&pEnding_spr_cur->Tex_flg, &MAIN.Ot0[G.Cbuf_id], 2);
        }

        pEnding_spr_cur->Scr_x += 8;

        p++;
        if (*p == 0)
        {
            return;
        }
    }
}

// caption page list for set 0: show frame, hide frame and x position per page, ended by 0xFFFF
UINT Ending_cap_tbl00[32] =
{
    0x00A6003D,   // 0 //
    0x00B500A0,   // 1 //
    0x000000E2,   // 2 //
    0x011E00F1,   // 3 //
    0x012D0000,   // 4 //
    0x0000015A,   // 5 //
    0x01960169,   // 6 //
    0x01A50000,   // 7 //
    0x00A001D2,   // 8 //
    0x020701E1,   // 9 //
    0x021600A0,   // 10 //
    0x00A0027F,   // 11 //
    0x02BB028E,   // 12 //
    0x02CA0000,   // 13 //
    0x000002F7,   // 14 //
    0x03330306,   // 15 //
    0x03420000,   // 16 //
    0x0000036F,   // 17 //
    0x03E7037E,   // 18 //
    0x03F600A0,   // 19 //
    0x00A00423,   // 20 //
    0x04570432,   // 21 //
    0x046600A0,   // 22 //
    0x00000493,   // 23 //
    0x04CF04A2,   // 24 //
    0x04DE0000,   // 25 //
    0x00200547,   // 26 //
    0x05CE0556,   // 27 //
    0xFFFF00A0,   // 28 //
    0x00000000,   // 29 //
    0x00000000,   // 30 //
    0x00000000,   // 31 //
};

// caption page list for set 1, same layout as set 0
UINT Ending_cap_tbl01[32] =
{
    0x00A1003D,   // 0 //
    0x00B000A0,   // 1 //
    0x000000DD,   // 2 //
    0x011900EC,   // 3 //
    0x01280000,   // 4 //
    0x00000155,   // 5 //
    0x018C0164,   // 6 //
    0x019B0000,   // 7 //
    0x00A001C8,   // 8 //
    0x020401D7,   // 9 //
    0x021300A0,   // 10 //
    0x00A00277,   // 11 //
    0x02B30286,   // 12 //
    0x02C20000,   // 13 //
    0x000002EF,   // 14 //
    0x032B02FE,   // 15 //
    0x033A0000,   // 16 //
    0x00000367,   // 17 //
    0x03DA0376,   // 18 //
    0x03E900A0,   // 19 //
    0x00A00416,   // 20 //
    0x04520425,   // 21 //
    0x046100A0,   // 22 //
    0x0000048E,   // 23 //
    0x04CA049D,   // 24 //
    0x04D90000,   // 25 //
    0x0020055B,   // 26 //
    0x05CE056A,   // 27 //
    0xFFFF00A0,   // 28 //
    0x00000000,   // 29 //
    0x00000000,   // 30 //
    0x00000000,   // 31 //
};

// caption page list per text set
CAPTION_TBL* Ending_cap_tbl[2] =
{
    (CAPTION_TBL*)Ending_cap_tbl00,     // 0 //
    (CAPTION_TBL*)Ending_cap_tbl01,   // 1 //
};

// background cue list for set 0: show frame, hide frame, x and y per cue (not referenced by code)
UINT Ending_bg_tbl00[28] =
{
    0x00A6003D,   // 0 //
    0x0000FFF6,   // 1 //
    0x011E00B5,   // 2 //
    0x0000000A,   // 3 //
    0x0196012D,   // 4 //
    0x0000000A,   // 5 //
    0x020701A5,   // 6 //
    0x0000FFF6,   // 7 //
    0x027F0216,   // 8 //
    0x0000FFF6,   // 9 //
    0x02F7028E,   // 10 //
    0x0000000A,   // 11 //
    0x036F0306,   // 12 //
    0x0000000A,   // 13 //
    0x03E7037E,   // 14 //
    0x0000FFF6,   // 15 //
    0x045703F6,   // 16 //
    0x0000FFF6,   // 17 //
    0x04CF0466,   // 18 //
    0x0000000A,   // 19 //
    0x054704DE,   // 20 //
    0x0000000A,   // 21 //
    0x05CE0556,   // 22 //
    0x0000FFF6,   // 23 //
    0x068205DD,   // 24 //
    0x00000000,   // 25 //
    0x0000FFFF,   // 26 //
    0x00000000,   // 27 //
};

// background cue list for set 1 (not referenced by code)
UINT Ending_bg_tbl01[28] =
{
    0x00A1003D,   // 0 //
    0x0000FFF6,   // 1 //
    0x011900B0,   // 2 //
    0x0000000A,   // 3 //
    0x018C0128,   // 4 //
    0x0000000A,   // 5 //
    0x0204019B,   // 6 //
    0x0000FFF6,   // 7 //
    0x02770213,   // 8 //
    0x0000FFF6,   // 9 //
    0x02EF0286,   // 10 //
    0x0000000A,   // 11 //
    0x036702FE,   // 12 //
    0x0000000A,   // 13 //
    0x03DA0376,   // 14 //
    0x0000FFF6,   // 15 //
    0x045203E9,   // 16 //
    0x0000FFF6,   // 17 //
    0x04CA0461,   // 18 //
    0x0000000A,   // 19 //
    0x055B04D9,   // 20 //
    0x0000000A,   // 21 //
    0x05CE056A,   // 22 //
    0x0000FFF6,   // 23 //
    0x068205DD,   // 24 //
    0x00000000,   // 25 //
    0x0000FFFF,   // 26 //
    0x00000000,   // 27 //
};

// background cue list per text set (not referenced by code)
ENDING_BG_REC* Ending_bg_tbl[2] =
{
    (ENDING_BG_REC*)Ending_bg_tbl00,   // 0 //
    (ENDING_BG_REC*)Ending_bg_tbl01,   // 1 //
};

// picture cue list for set 0: show frame, move frame, box position and box velocity, ended by 0xFFFF
UINT Ending_pic_tbl00[6] =
{
    0x03420306,   // 0 //
    0x00820000,   // 1 //
    0x0014FFEC,   // 2 //
    0x0000FFFF,   // 3 //
    0x00000000,   // 4 //
    0x00000000,   // 5 //
};

// picture cue list for set 1, same layout as set 0
UINT Ending_pic_tbl01[6] =
{
    0x0177012D,   // 0 //
    0x00820000,   // 1 //
    0x0014FFEC,   // 2 //
    0x0000FFFF,   // 3 //
    0x00000000,   // 4 //
    0x00000000,   // 5 //
};

UINT Ending_pic_tbl02[3] = { 0x00000000, 0x00000000, 0x00000000 };   // empty picture cue list

// picture cue list per text set
ENDING_PIC_REC* Ending_pic_tbl[2] =
{
    (ENDING_PIC_REC*)Ending_pic_tbl00,   // 0 //
    (ENDING_PIC_REC*)Ending_pic_tbl01,   // 1 //
};

// options per ending number - 1: extra movie, scroll mode, launcher award, alternate graphics
ENDING_DEF_REC Ending_def_tbl[7] =
{
    { 1, 1, 0, 0 },   // 0 //
    { 1, 1, 0, 0 },   // 1 //
    { 1, 1, 0, 0 },   // 2 //
    { 0, 1, 0, 1 },   // 3 //
    { 0, 1, 0, 1 },   // 4 //
    { 0, 1, 1, 1 },   // 5 //
    { 0, 1, 1, 1 },   // 6 //
};

// sprite slots for the epilogue (entries 0 to 4 and 7 to 11) and the result screen (entries 5 and 6):
// type, x, y, texture row and height
ENDING_SPR_REC Ending_spr_tbl[12] =
{
    { 0x0002, 0x0000, 0x0030, 0x00, 0x18 },   // 0 //
    { 0x0003, 0x0030, 0x0050, 0x00, 0x00 },   // 1 //
    { 0x0002, 0x0000, 0x0074, 0x18, 0x18 },   // 2 //
    { 0x0005, 0x0048, 0x0094, 0x00, 0x00 },   // 3 //
    { 0x0002, 0x0025, 0x0066, 0x30, 0x28 },   // 4 //
    { 0x0002, 0x0020, 0x0020, 0x58, 0x30 },   // 5 //
    { 0x0002, 0x0020, 0x00C0, 0x88, 0x20 },   // 6 //
    { 0x0002, 0x0020, 0x0016, 0x00, 0x36 },   // 7 //
    { 0x0003, 0x0080, 0x0058, 0x00, 0x00 },   // 8 //
    { 0x0002, 0x0020, 0x007A, 0x36, 0x36 },   // 9 //
    { 0x0005, 0x0098, 0x00B8, 0x00, 0x00 },   // 10 //
    { 0x0002, 0x0025, 0x0066, 0x30, 0x28 },   // 11 //
};

// camera switch zone data for the epilogue room
INT Ending_cam_tbl[5] =
{
    0x00090000,   // 0 //
    0x00000000,   // 1 //
    0x7D000000,   // 2 //
    0x7D007D00,   // 3 //
    0x00007D00,   // 4 //
};

// empty collision block for the epilogue room
UINT Ending_sca_tbl[6] =
{
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000
};
