/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\MAIN.C
 */

#include "TYPES.H"
#include <LIBETC.H>
#include <LIBPRESS.H>

#include "MAIN.H"
#include "CD.H"
#include "CINESCO.H"
#include "DOOR.H"
#include "EVENT.H"
#include "GAME.H"
#include "MEM_CARD.H"
#include "MOJIDISP.H"
#include "MOVIE.H"
#include "PAD.H"
#include "SCE3.H"
#include "SND.H"
#include "ST_SUB03.H"
#include "ZENTAI.H"
#include "FILECODE.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "MONITOR.H"
#include "SDK.H"
#include "SUB.H"

ULONG GsTRATE; // GPU library: transparency rate

ULONG GsZOVER; // GPU library: depth overflow setting used when sorting objects
ULONG GsDISPON; // GPU library: display-on state
USHORT GsORGOFSY; // GPU library: draw origin y offset

MATRIX GsWSMATRIX_ORG; // GPU library: saved copy of the world-screen matrix

UCHAR Pl_turn_run_pad[2]; // padding: keeps the neighbouring data at its linked position

UCHAR Door_snd_bank_pad[4]; // padding: keeps the neighbouring data at its linked position
UCHAR GsLMODE_pad[16]; // padding: keeps the neighbouring data at its linked position
UCHAR GsLIGNR_pad[8]; // padding: keeps the neighbouring data at its linked position
UCHAR GsZOVER_pad[4]; // padding: keeps the neighbouring data at its linked position

UCHAR Door_cmd_now_pad[4]; // padding: keeps the neighbouring data at its linked position

UCHAR Item_light_pad[8]; // padding: keeps the neighbouring data at its linked position
MATRIX GsLIGHTWSMATRIX; // GPU library: light-to-screen matrix
UCHAR GsORGOFSX_pad[4]; // padding: keeps the neighbouring data at its linked position

UCHAR GsORGOFSY_pad[4]; // padding: keeps the neighbouring data at its linked position

UCHAR Svm_vh_pad[4]; // padding: keeps the neighbouring data at its linked position
UCHAR Sca_rtn_tbl_pad[176]; // padding: keeps the neighbouring data at its linked position

UCHAR Sca_hit_top_pad[4]; // padding: keeps the neighbouring data at its linked position

UCHAR Door_status_pad[2]; // padding: keeps the neighbouring data at its linked position

UCHAR Door_obj_tbl_pad[4]; // padding: keeps the neighbouring data at its linked position

MATRIX GsIDMATRIX; // GPU library: identity matrix

UCHAR StFinalSector_pad[32]; // padding: keeps the neighbouring data at its linked position
UCHAR Sce3_logo_state_pad[4]; // padding: keeps the neighbouring data at its linked position

MATRIX GsWSMATRIX; // GPU library: world-screen matrix
UCHAR Xa_start_sector_pad[1]; // padding: keeps the neighbouring data at its linked position

UCHAR Snd_seq_tick_mode_pad[4]; // padding: keeps the neighbouring data at its linked position

UCHAR Snd_seq_tick_mode_pad2[80]; // padding: keeps the neighbouring data at its linked position

UCHAR Door_next_stage_room_pad[2]; // padding: keeps the neighbouring data at its linked position
UCHAR PSDIDX_pad[2]; // padding: keeps the neighbouring data at its linked position

UCHAR Svm_vab_total_pad[4]; // padding: keeps the neighbouring data at its linked position

UCHAR Spu_key_stat_pad[16]; // padding: keeps the neighbouring data at its linked position

UCHAR Xa_start_loc_pad[4]; // padding: keeps the neighbouring data at its linked position

VOID main(VOID)
{
    DRAWENV env[2];
    DR_MODE Dr_mode[2];
    USHORT Tcb_bak;
    USHORT Fade_sum;
    USHORT sum;
    SHORT cur;
    USHORT step;
    SHORT cur1;
    POLY_F4* Prim;
    POLY_F4* pPoly1;
    UCHAR buf;
    UCHAR level;
    UCHAR Level1;
    UCHAR* pLevel;

    Init_system();
    SetDefDrawEnv(&env[0], 0, 0xF0, 0x140, 0xF0);
    SetDefDrawEnv(&env[1], 0, 0, 0x140, 0xF0);
    SetDrawMode(&Dr_mode[0], 0, 0, GetGraphType() == 1 || GetGraphType() == 2 ? 0x300 : 0xC0, 0);
    SetDrawMode(&Dr_mode[1], 0, 0, GetGraphType() == 1 || GetGraphType() == 2 ? 0x300 : 0xC0, 0);
    while (1)
    {
        Pad_set();
        Demo_timer_main();

        if ((G.System_flg & SYS_STATUS_MASK) == SYS_STATUS_SCREEN)
        {
            if ((G.Padd1 & (PAD_SELECT | PAD_START)) == (PAD_SELECT | PAD_START))
            {
                RESET_CNT_CUR += MAIN.Vloop ? 2 : 1;
                if (RESET_CNT_CUR >= 0x79)
                {
                    MAIN.Reset_cnt = 0;
                    Task_kill(0);
                    Task_kill(1);
                    Task_kill(2);
                    G.SaveData.Mess_flg = 0;
                    G.SaveData.Fade_kido = G.SaveData.Fade1_kido = -1;
                    Init_graph(0);
                    MAIN.System_flg = (MAIN.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;
                    Vram_clr(0, 0, 0x1E0, 0x1E0);
                    Task_execute(0, Soft_reset);
                }
            }
            else
            {
                MAIN.Reset_cnt = 0;
            }
        }

        if (MAIN.System_flg & SYS_GRAPH_RESET)    // graphics reset requested
        {
            Init_graph(1);
        }
        if (G.Fade_rtn != 0)
        {
            Snd_bgm_fade();
        }
        if (MAIN.System_flg & SYS_FADE_OUT)    // full-screen fade active
        {
            if (G.SaveData.Fade_kido < 0)
            {
                Fade_box.attribute = (G.Fade_hrate << 28) | 0x40000000;
                Fade_sum = 0;
                if (G.Cbuf_id == 0)
                {
                    GsDefDispBuff(0, 0, 0, 0);
                }
                else
                {
                    GsDefDispBuff(0, 0xF0, 0, 0xF0);
                }
                Tcb_bak = 0;
                G.SaveData.Fade_kido = -(G.Fade_add < 1) & 0x7FFF;
                if (Monitor.Task[0].Tcb_status & 0x40)
                {
                    Tcb_bak = Monitor.Task[0].Tcb_status;
                }
                else
                {
                    Task_suspend(0);
                }
            }
            step = FADE_ADD_CUR;

            Fade_sum += step;
            sum = Fade_sum;

            Fade_box.r = Fade_box.g = Fade_box.b = step >> 7;

            if ((sum >> 10) == 1)
            {
                Fade_box.r = Fade_box.g = Fade_box.b = 8;
            }
            Fade_sum = sum & 0x2FF;
            GsSetWorkBase(Gs_packet_p[CBUF_ID_CUR]);
            GsClearOt(0, 0, MAIN.Ot0 + MAIN.Cbuf_id);
            GsSortBoxFill(&Fade_box, MAIN.Ot0 + MAIN.Cbuf_id, 0);
            Scheduler();
            DrawSync(0);
            VSync(0);
            DrawOTag((ULONG*)MAIN.Ot0[MAIN.Cbuf_id].tag);  // word view of the same memory
            FADE_KIDO_CUR += MAIN.Fade_add;
            if (MAIN.SaveData.Fade_kido >= 0)
            {
                continue;
            }
            MAIN.Fade_add = 0;
            MAIN.SaveData.Fade_kido = 0;
            MAIN.System_flg &= ~SYS_FADE_OUT;
            GsDefDispBuff(0, 0, 0, 0xF0);
            if (Tcb_bak == 0)
            {
                Task_signal(0);
            }
        }
        {
            VOID* pkt;
            CBUF_ID_CUR = GsGetActiveBuff();
            pkt = Gs_packet_p[CBUF_ID_CUR];

            G.Disp_y = -((*(volatile UCHAR*)&MAIN.Cbuf_id) == 0) & 0xF0;  // UCHAR* view of the field (a plain access changes the code)

            GsSetWorkBase(pkt);
        }
        GsClearOt(0, 0, MAIN.Ot1 + CBUF_ID_CUR);
        GsClearOt(0, 0, MAIN.Ot2 + CBUF_ID_CUR);
        GsClearOt(0, 0, MAIN.Ot0 + CBUF_ID_CUR);
        Scheduler();
        if (G.SaveData.Mess_flg & MESS_FLG_ACTIVE)
        {
            Mess_disp();
        }
        cur = FADE_KIDO_CUR;
        if (cur >= 0)  // sign-extends the halfword
        {
            Fade_box.attribute = (G.Fade_hrate << 28) | 0x40000000;
            Fade_box.r = Fade_box.g = Fade_box.b = cur >> 7;  // sign-extends the halfword
            MAIN.SaveData.Fade_kido = cur + MAIN.Fade_add;
            GsSortBoxFill(&Fade_box, MAIN.Ot0 + MAIN.Cbuf_id, 0);
        }
        if (MAIN.System_flg & SYS_DIM)    // screen dim requested
        {
            if (CURTAIN_KIDO_CUR < 0xF0)
            {
                MAIN.Curtain_kido += 0x10;
            }
        }
        else
        {
            if (CURTAIN_KIDO_CUR >= 0x10)
            {
                MAIN.Curtain_kido -= 0x10;
            }
        }
        buf = CBUF_ID_CUR;

        pLevel = &MAIN.Curtain_kido;
        level = *pLevel;

        Prim = &Curtain[0][buf];
        Prim->r0 = level;
        Prim->g0 = level;
        Curtain[0][buf].b0 = level;
        buf = MAIN.Cbuf_id;

        Level1 = *pLevel;

        pPoly1 = &Curtain[0][buf];
        pPoly1[2].r0 = Level1;
        pPoly1[2].g0 = Level1;
        Curtain[1][buf].b0 = Level1;
        if (!(G.System_flg & (SYS_STATUS_SCREEN | SYS_DOOR_PROCESS)))
        {
            if (*pLevel != 0)
            {
                AddPrim(MAIN.Ot0[MAIN.Cbuf_id].org + 4, &Dr_mode[MAIN.Cbuf_id]);
                AddPrim(MAIN.Ot0[MAIN.Cbuf_id].org + 3, &Curtain[0][MAIN.Cbuf_id]);
                AddPrim(MAIN.Ot0[MAIN.Cbuf_id].org + 3, &Curtain[0][MAIN.Cbuf_id + 2]);
            }
            cur1 = FADE1_KIDO_CUR;
            if (cur1 >= 0)  // sign-extends the halfword
            {
                Fade_box.attribute = (G.SaveData.Fade1_hrate << 28) | 0x40000000;
                Fade_box.r = Fade_box.g = Fade_box.b = cur1 >> 7;  // sign-extends the halfword

                Fade_box.r &= MASK_R_CUR;

                Fade_box.g &= G.Mask_g;
                Fade_box.b &= G.Mask_b;
                MAIN.SaveData.Fade1_kido = cur1 + G.SaveData.Fade1_add;
                GsSortBoxFill(&Fade_box, MAIN.Ot0 + MAIN.Cbuf_id, 5);
            }
        }
        Cinesco_fade_main();
        DrawSync(0);
        VSync(G.Vloop);

        Snd_voice_ck();

        ResetGraph(1);
        GsSwapDispBuff();

        if (MAIN.System_flg & SYS_BG_MODE_CLEAR)    // screen mode: solid clear colour
        {
            GsSortClear(G.Clr_r, G.Clr_g, G.Clr_b, MAIN.Ot2 + G.Cbuf_id);
        }
        else if (MAIN.System_flg < 0)    // screen mode: background image
        {
            if (G.Status_flg & STAT_BG_BORDER)
            {
                Door_border_set();
            }
            else
            {
                Bg_img_trans(-(G.Cbuf_id == 0) & 0xF0);
            }
        }
        if ((G.Status_flg & STAT_QUAKE) && !(G.System_flg & SYS_STATUS_MASK))
        {
            Door_quake_set();
        }
        DrawSync(0);
        if (MAIN.System_flg & SYS_MOVIE)    // movie requested
        {
            Movie_play(G.Scenario.Movie_id, G.Scenario.pMovie_buf, 1);  // passes 3 arguments, Movie_play declares 2
            G.SaveData.Key_trg = 0;
            G.SaveData.Key = 0;
            MAIN.System_flg &= ~SYS_MOVIE;
        }
        DrawOTag((ULONG*)MAIN.Ot2[CBUF_ID_CUR].tag);  // ULONG* view of the same memory
        DrawOTag((ULONG*)MAIN.Ot1[MAIN.Cbuf_id].tag);  // word view of the same memory
        DrawOTag((ULONG*)MAIN.Ot0[MAIN.Cbuf_id].tag);  // word view of the same memory
        PutDrawEnv(&env[MAIN.Cbuf_id]);
        Xa_control();
    }
}

VOID Init_system(VOID)
{
    INT i;

    ResetCallback();
    SetGraphDebug(0);

    InitGeom();

    InitCARD(0);
    StartCARD();
    _bu_init();
    PadInitDirect(Padd_data[0], Padd_data[1]);
    PadStartCom();

    Init_graph(0);
    GsClearDispArea(0, 0, 0);

    Card_event_set();
    SetRCnt(0xF4000001, 1, 0x2000);
    StartRCnt(0xF4000001);

    G.Vloop = 2;
    G.Ot1[0].length = 0xA;
    G.Ot1[0].org = MAIN.Ot1_tag[0];
    G.Ot1[1].length = 0xA;
    G.Ot1[1].org = MAIN.Ot1_tag[1];
    G.Ot2[0].length = 4;
    G.Ot2[0].org = MAIN.Ot2_tag[0];
    G.Ot2[1].length = 4;
    G.Ot2[1].org = MAIN.Ot2_tag[1];
    G.Ot0[0].length = 4;
    G.Ot0[0].org = MAIN.Ot0_tag[0];
    G.Ot0[1].length = 4;
    G.Ot0[1].org = MAIN.Ot0_tag[1];

    Pad_vib_init();

    Init_global();
    Init_snd_fade();
    Gs_packet_p[0] = &MAIN.Pkt_buf;  // address passed as an integer
    Gs_packet_p[1] = &MAIN.Pkt_buf[1];  // address passed as an integer
    MAIN.System_flg = (MAIN.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;
    Init_scheduler();
    Pad_def_set();

    G.SaveData.Key_idx = 0;

    for (i = 0; i < 2; i++)
    {
        SetPolyF4(&Curtain[0][i]);
        SetPolyF4(&Curtain[0][i + 2]);
        SetSemiTrans(&Curtain[0][i], 1);
        SetSemiTrans(&Curtain[0][i + 2], 1);
    }

    Task_execute(0, Init_main);

    Task_execute(1, InitGeom_func);
}

VOID Init_graph(INT No_reset)
{
    if (No_reset == 0)
    {
        GsInitGraph(0x140U, 0xF0U, 0U, 0U, 0U);
    }
    else
    {
        GsInitGraph2(0x140U, 0xF0U, 0U, 0U, 0U);
    }

    GsDefDispBuff(0U, 0U, 0U, 0xF0U);
    GsInit3D();
    Reset_graph();
    MAIN.System_flg &= ~SYS_GRAPH_RESET;

    Fade_box_pos_set();
}

VOID Reset_graph(VOID)
{
    ResetGraph(0);

    DecDCTReset(0);
    Cd_init();
}

VOID Init_main(VOID)
{
    INT unused;
    UINT i;
    UINT j;
    UINT k;
    VOID* Rsv2;

    Cd_read(GWARNIN1_TIM, LOAD_BUF);

    Task_execute(1, &Tex_load);
    Task_sleep(1);
    G.Bg_img_mode = 1;
    G.Fade_hrate = 2;
    G.Fade_add = -0x2000;
    MAIN.System_flg = (MAIN.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_IMAGE;
    i = 0;
    Evt_fade_start();
    do
    {
        Task_sleep(1);
        i += 1;
    } while (i < 4U);
    j = 0;
    do
    {
        Task_sleep(1);
        j += 1;
    } while (j < 0x78U);
    G.Fade_hrate = 2;
    G.Fade_add = 0x2000;
    k = 0;
    Evt_fade_start();
    do
    {
        Task_sleep(1);
        k += 1;
    } while (k < 4U);
    G.Bg_img_mode = 0;
    MAIN.System_flg = (MAIN.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;
    Task_sleep(2);
    while (Task_status(1) != 0)
    {
        Task_sleep(1);
    }
    PadSetMainMode(0, 1, 2);

    DrawSync(0);

    Task_lchain(LOGO_EXE);
}

VOID Fade_box_pos_set(VOID)
{
    if (Scr_ofs[0] == 0xA0)
    {
        Fade_box.x = -0xA0;
        Fade_box.y = -0x78;
        return;
    }
    Fade_box.y = 0;
    Fade_box.x = 0;
}

VOID Demo_timer_main(VOID)
{
    if ((G.Status_flg & STAT_DEMO_MODE) && (G.Stop_flg & STOP_DEMO_TIMER) && demo_tbl0.Pdemo_cnt != 0)
    {
        demo_tbl0.Pdemo_cnt += 1;
        if (demo_tbl0.Pdemo_cnt >= demo_tbl0.Pdemo_len - 1)
        {
            demo_tbl0.Pdemo_cnt = 0;
        }
    }
}

VOID Soft_reset(VOID)
{
    INT Bak_flg;

    Snd_bgm_fade_all();
    Snd_sys_init2();

    Bak_flg = STATUS_FLG_CUR & STAT_NEW_GAME;

    Memclr_dw((INT*)&MAIN, (INT*)&MAIN.SaveData.Key_tbl);  // the work area is cleared as words
    Init_snd_fade();
    Gs_packet_p[0] = &MAIN.Pkt_buf;  // address passed as an integer
    Gs_packet_p[1] = &MAIN.Pkt_buf[1];  // address passed as an integer

    MAIN.Status_flg |= Bak_flg;

    Task_lchain(TITLE_EXE);
}

VOID Tex_load(VOID)
{
    Cd_read(FONT_TIM, OVERLAY_AREA);

    TPAGE_CLUT_CUR = 0x1E;
    Mapping_tim(OVERLAY_AREA);

    Cd_read(STATUS_TIM, OVERLAY_AREA);

    MAIN.Vram_cur.Tpage_clut = 0x41C;
    Mapping_tim(OVERLAY_AREA);
    Task_exit();
}

VOID InitGeom_func(VOID)
{
    InitGeom(0);
    Task_exit();
}

// actuator alignment table for PadSetActAlign: 0x00 and 0x01 assign the two motors, 0xFF leaves a slot unused
UCHAR Pad_act_align[0x8] =
{
    0x00,   // 0 //
    0x01,   // 1 //
    0xFF,   // 2 //
    0xFF,   // 3 //
    0xFF,   // 4 //
    0xFF,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
};

GsBOXF Fade_box = { 0x60000000, -160, -120, 320, 240, 0, 0, 0 }; // full-screen box used for fades and room light overlays

// screen dim polygons: a top and a bottom bar for each display buffer
POLY_F4 Curtain[2][2] =
{
    { { 0x00000000, 0, 0, 0, 0, 0, 240, 320, 240, 0, 268, 320, 268 }, { 0x00000000, 0, 0, 0, 0, 0, 0, 320, 0, 0, 28, 320, 28 } },         // 0 //
    { { 0x00000000, 0, 0, 0, 0, 0, 452, 320, 452, 0, 480, 320, 480 }, { 0x00000000, 0, 0, 0, 0, 0, 212, 320, 212, 0, 240, 320, 240 } },   // 1 //
};

// compile-time checks of the GLOBAL field offsets and total size
typedef CHAR FixedWorkC3000_game_state_offset[(OFFSET_OF(GLOBAL, System_flg) == 0x0) ? 1 : -1];

typedef CHAR FixedWorkC3000_effects_offset[(OFFSET_OF(GLOBAL, Esp) == 0x24) ? 1 : -1];
typedef CHAR FixedWorkC3000_player_offset[(OFFSET_OF(GLOBAL, Player) == 0x2124) ? 1 : -1];
typedef CHAR FixedWorkC3000_enemies_offset[(OFFSET_OF(GLOBAL, Enemy) == 0x22A4) ? 1 : -1];
typedef CHAR FixedWorkC3000_unknown_510C_offset[(OFFSET_OF(GLOBAL, Em_save_buf) == 0x510C) ? 1 : -1];
typedef CHAR FixedWorkC3000_post_enemy_offset[(OFFSET_OF(GLOBAL, Die_rno) == 0x5454) ? 1 : -1];
typedef CHAR FixedWorkC3000_bio_card_offset[(OFFSET_OF(GLOBAL, SaveData) == 0x5460) ? 1 : -1];
typedef CHAR FixedWorkC3000_key_config_offset[(OFFSET_OF(GLOBAL, SaveData.Key_tbl) == 0x587C) ? 1 : -1];
typedef CHAR FixedWorkC3000_ordering_tables_offset[(OFFSET_OF(GLOBAL, Ot0) == 0x58A0) ? 1 : -1];
#ifndef __INTELLISENSE__  // offsets differ under the 32-bit IntelliSense layout
typedef CHAR FixedWorkC3000_player_anim_offset[(OFFSET_OF(GLOBAL, Pl_move) == 0x7A18) ? 1 : -1];
typedef CHAR FixedWorkC3000_door_room_work_offset[(OFFSET_OF(GLOBAL, pSce_aot_on) == 0x7AFC) ? 1 : -1];
typedef CHAR FixedWorkC3000_packets_offset[(OFFSET_OF(GLOBAL, Pkt_buf) == 0x7B28) ? 1 : -1];

typedef CHAR FixedWorkC3000_sound_scd_offset[(OFFSET_OF(GLOBAL, Scenario.Vab_id) == 0xC628) ? 1 : -1];
typedef CHAR FixedWorkC3000_scd_events_offset[(OFFSET_OF(GLOBAL, Scenario.Sce_task) == 0xC68C) ? 1 : -1];
typedef CHAR FixedWorkC3000_frame_runtime_offset[(OFFSET_OF(GLOBAL, Scenario.pTask_cur) == 0xC82C) ? 1 : -1];
typedef CHAR FixedWorkC3000_effect_sprites_offset[(OFFSET_OF(GLOBAL, Esp_spr) == 0xC878) ? 1 : -1];
typedef CHAR FixedWorkC3000_size[(sizeof(GLOBAL) == 0xCA24) ? 1 : -1];
#endif
