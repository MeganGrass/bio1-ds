/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\GAME.C
 */

#include "TYPES.H"
#include <LIBCD.H>
#include <LIBETC.H>
#include <RAND.H>
#include <STDIO.H>
#include "GAME.H"
#include "CD.H"
#include "CUT.H"
#include "DOOR.H"
#include "EFF1.H"
#include "ENEMY.H"
#include "EVENT.H"
#include "INVENTORY.H"
#include "JOINT.H"
#include "MAIN.H"
#include "MOJIDISP.H"
#include "OM.H"
#include "OPTIONS.H"
#include "PAD.H"
#include "PLAYER.H"
#include "PL_WEP.H"
#include "PSP.H"
#include "SCE0.H"
#include "SCE1.H"
#include "SCE2.H"
#include "SCE_AT.H"
#include "SND.H"
#include "STAGE.H"
#include "ST_SUB00.H"
#include "ST_SUB02.H"
#include "ST_SUB03.H"
#include "SUB.H"
#include "ZENTAI.H"
#include "SCRATCHPAD.H"
#include "FILECODE.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "SCENARIO.H"
#include "MONITOR.H"
#include "ROOM.H"

INT Game_unused_word;                  // work word cleared at session start, never read
PDEMO demo_tbl0;                     // attract demo record being played
AT_WORK* pSca_hit_cur;           // next free collision hit record of the pool
AT_WORK* Sca_hit_top;      // first pool record after the player's, the reset point of the pool
AT_WORK Sca_at_tbl_stat[64];       // collision hit records; the first belongs to the player
UCHAR* pRoom_scd_main;            // main script of the current room, run every frame
UCHAR Door_next_stage_room;        // destination stage and room byte of the door being taken
INT Play_vsync;                   // VSync count at the start of the game loop, used to accumulate play time

VOID Game_start(VOID)
{
    INT Game_res;
    INT status;

    Bg_rgb_clr();

    STOP_FLG_CUR &= ~STOP_DEMO_TIMER;

    Snd_sys_init();
    Title_game_init();
    Game_res = Game_loop();

    Pad_vib_init();

    G.System_flg = 0;

    status = STATUS_FLG_CUR;
    if (status & STAT_DEMO_MODE)
    {
        MAIN.Status_flg = status & STAT_RESET_KEEP_MASK;
        Task_lchain(TITLE_EXE);
    }

    if (Game_res == 0)
    {
        GAME_PLAY_TIME_CUR += VSync(-1) - Play_vsync;

        Task_lchain(ENDING_EXE);
    }

    MAIN.Status_flg &= STAT_RESET_KEEP_MASK;

    Task_lchain(LOGO_EXE);
}

VOID Title_game_init(VOID)
{
    INT* pNext_pool;
    INT Jill_flg;
    INT status;
    INT Jill_flg2;
    USHORT Life3;
    USHORT Life;
    USHORT Life2;
    INT Flg_mask;

    Pad_vib_init();

    Vram_clr(0, 0, 0x140, 0x1E0);
    MAIN.System_flg = (MAIN.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;
    Task_sleep(1);
    G.Vloop = 2;
    Flg_mask = 0xD4E900FF;
    MAIN.System_flg = ((MAIN.System_flg & Flg_mask) & (~SYS_ROOM_CLR_MASK)) | SYS_DOOR_PROCESS;
    Memclr_dw((INT*)&MAIN.Def_item, (INT*)&MAIN.SaveData);  // the work area is cleared as words
    G.SaveData.Fade1_add = 0;
    G.Fade_add = 0;
    FREE_WORK_INT = (INT)OVERLAY_AREA;  // INT view of the field (plain access changes the code)
    Task_execute(1, &Load_mess_disp);
    Cd_read_wait(BIO_CARD_DAT, MAIN.pFree_work);
    if (MAIN.System_flg & SYS_CARD_LOAD)
    {
        Memcpy((UCHAR*)&MAIN.SaveData, MAIN.pFree_work, 0x200);  // byte copy of the loaded save record
        Snd_sys_stereo((G.SaveData.Key_idx >> 7) ^ 1);
        G.Player.Old_x = G.SaveData.Pl_pos_x;
        G.Player.Old_z = G.SaveData.Pl_pos_z;
        G.Player.Cdir_y = G.SaveData.Pl_cdir_y;
        G.Player.Sca_info = G.SaveData.Pl_sca_info;
        G.Player.Life = G.SaveData.Pl_life;
        G.Player.Id = G.SaveData.Pl_id;
        PL_CHG_NO_CUR = G.SaveData.Pl_id;
        if (Flg_ck(&MAIN.SaveData.Scenario_flg, 0x2A) != 0)  // address as integer
        {
            MAIN.SaveData.Pl_chg_no += 8;
        }

        if (Flg_ck(&MAIN.SaveData.Scenario_flg, 0x6F) != 0 || Flg_ck(&MAIN.SaveData.Scenario_flg, 0x70) != 0)  // address as integer
        {
            MAIN.SaveData.Pl_chg_no += 8;
        }

        SAVE_CNT_CUR += 1;
    }
    else
    {
        G.Player.Sca_info = 0x10;
        G.Player.Id = G.SaveData.Pl_id;
        Memcpy((UCHAR*)&MAIN.SaveData, MAIN.pFree_work, 0x41C);  // byte copy of the loaded save record
        G.SaveData.Pl_chg_no = G.Player.Id;
        if (G.Status_flg & STAT_DEMO_MODE)
        {
            Demo_pl_load();
        }
        else
        {
            Pl_data_init();
        }

        Jill_flg = G_PLAYER->Id & 1;
        Life = 0x8C - Jill_flg * 0x2C;

        G.Player.Life = Life;

        PL_LIFE_CUR = Life;
        if ((STATUS_FLG_CUR) & STAT_EASY_MODE)  // type conversion
        {
            Life2 = 0xB4 - Jill_flg * 0x1E;
            G.Player.Life = Life2;
            MAIN.SaveData.Pl_life = Life2;
            DC_MODE_CUR = 1;
            VIEW(INT, G.SaveData.Item_boxwork[0].Id) = 0x1E0B1E0B;  // INT view of the field (plain access changes the code)
        }
        status = MAIN.Status_flg;
        if (status & STAT_ARRANGE)
        {
            DC_MODE_CUR = 2;
            Life3 = 0x64 - (G.Player.Id & 1) * 0x1E;
            G.Player.Life = Life3;
            MAIN.SaveData.Pl_life = Life3;
            if (status & 0x10000)
            {
                MAIN.SaveData.Dc_mode = 3;
                VIEW(INT, G.SaveData.Item_boxwork[0].Id) = 0x1E0B1E0B;  // INT view of the field (plain access changes the code)
            }
        }
    }
    Monitor.pId_mat = &GsIDMATRIX;
    Monitor.pWs_mat = &GsWSMATRIX;
    Monitor.pLight_mtx = &GsLIGHTWSMATRIX;
    Game_unused_word = 0;
    G.SaveData.Use_id = 0;
    G.SaveData.Get_id = 0;
    G.Def_item_num = 0;
    G.Def_item = 0;
    pItem_work_base = (ITEM_WORK*)&MAIN.SaveData.Item_work;  // address as integer; pointer type conversion (no code)
    G.Die_rno = 0;
    Status_init_m(LOAD_BUF);
    pAot_last = Sce_aot_tbl;
    pSca_hit_cur = Sca_at_tbl_stat;

    Jill_flg2 = G.Player.Id & 1;
    G.Player.Max_life = (-0x74) - Jill_flg2 * 0x2C;
    if (G.Status_flg & STAT_EASY_MODE)
    {
        G.Player.Max_life = (-0x4C) - Jill_flg2 * 0x1E;
    }
    if (G.Status_flg & STAT_ARRANGE)
    {
        G.Player.Max_life = 0x64 - Jill_flg2 * 0x1E;
    }
    pNext_pool = (INT*)&Sca_at_tbl_stat[1];  // pointer type conversion (no code)

    G.Player.pSca_ofs = Sca_at_tbl_stat;

    pSca_hit_cur = (AT_WORK*)pNext_pool;  // pointer type conversion (no code)
    Sca_hit_top = (AT_WORK*)pNext_pool;  // pointer type conversion (no code)

    G.Player.pAt_tbl = Pl_sca_ptr[0];

    Task_sleep(1, G.Status_flg, Sca_at_tbl_stat);

    Player_set();
    FREE_WORK_INT = (INT)ESP_WORK_BUF;  // INT view of the field (plain access changes the code)
    Esp_init_C();
    FREE_WORK_INT = (INT)OVERLAY_AREA;  // INT view of the field (plain access changes the code)
    G.Fade_rtn = 0;
    Bgm_status_mask = 0xFF;
    Snd_sys_core_set(0);
    Snd_sys_enem_set(G.Player.Id & 1);
    Snd_sys_arms_set(G.Player.Type, OVERLAY_AREA);
    MAIN.System_flg = MAIN.System_flg & ~SYS_DOOR_PROCESS;
    Set_stage();
}

INT Game_loop(VOID)
{
    USHORT Count_down_cnt;
    USHORT Sp_light_min;
    USHORT timer;
    USHORT Em_rnd_ptr_old;
    UINT flags;

    Count_down_cnt = 0;
    while (MAIN.SaveData.Mess_flg & MESS_FLG_ACTIVE)
    {
        Task_sleep(1);
    }
    Play_vsync = VSync(-1);

    MAIN.Scenario.Proc_flg_bak = 0xFFFF;

    MAIN.System_flg |= SYS_DOOR_LOAD;

    Sp_light_min = 0xFFFF;

    if (G.Status_flg & STAT_DEMO_MODE)
    {
        srand(0);
    }
    else
    {
        srand(VSync(-1));
    }

    Sprite_hide_mask = 0;

    goto start;
    for (;;)
    {
        Demo_end_ck();

        switch MENU_REQ_CUR
        {
        case 1:
            if (MAIN.SaveData.Mess_flg & MESS_FLG_ACTIVE)
            {
                break;
            }
            if (G.System_flg & SYS_DOOR_LOAD)
            {
                Evt_fade_req_set(2, 0x600);

                Sp_light_min = 0xFFFF;
            }
            else
            {
                Sp_light_min = MAIN.SaveData.Fade1_kido;

                MAIN.Reset_cnt = 0;

                Evt_fade_req_set(2, 0xC00);
            }

            MAIN.SaveData.Fade1_kido = -1;

            MAIN.System_flg = (MAIN.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;
            if (MAIN.System_flg & SYS_DOOR_LOAD)
            {
                Set_door();

                MAIN.Scenario.Proc_flg_bak = 0xFFFF;
            }
            else
            {
                Menu_exec();
            }
        start:
            G.Fade_hrate = 2;
            MAIN.System_flg = (MAIN.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_IMAGE;

            if (Flg_ck(&MAIN.SaveData.Scenario_flg[0], 0x7D) != 0)
            {
                MAIN.Fade_add = -0xA3;
            }
            else
            {
                MAIN.Fade_add = -0x1800;
            }
            Evt_fade_start();
            MAIN.SaveData.Fade1_kido = Sp_light_min;
            Task_sleep(1);
            if (G.System_flg & SYS_DOOR_LOAD)
            {
                MAIN.Stop_flg = STOP_ALL & ~(STOP_ACTION | STOP_SCE | STOP_DEMO_TIMER);
            }
            else
            {
                MAIN.Stop_flg = STOP_PLAYER;
            }
            MAIN.SaveData.Key_trg = 0;
            MAIN.SaveData.Key = 0;

            G.Menu_req = 2;
            MAIN.System_flg &= ~(SYS_DOOR_LOAD | 0x8000000);
            break;
        case 2:
            MAIN.Stop_flg = 0;
            MAIN.Menu_req = 3;
            break;
        case 3:
            if (MAIN.SaveData.Fade_kido <= 0)
            {
                MAIN.Menu_req = 0;

                timer = MAIN.Scenario.Proc_flg_bak;

                (*(volatile USHORT*)&MAIN.Stop_flg) = timer;  // USHORT* view of the field (a plain access changes the code)
                MAIN.Stop_flg = timer | STOP_DEMO_TIMER;

                Flg_off(&MAIN.SaveData.Scenario_flg, 0x7D);  // address as integer
            }
            break;
        }

        MAIN.SaveData.Sce_random = rand();
        Use_flg[0] = 0;
        Use_flg[1] = 0;
        MAIN.SaveData.Pl_life = MAIN.Player.Life;

        Hikidashi_entrance();
        Item_box_entrance();
        Save_entrance();

        Key_lost();

        if (Count_down_cnt >= 30 || MAIN.SaveData.Count_down_timer == 0x7FFF)
        {
            Count_down_cnt = 0;
            if (COUNT_DOWN_TIMER_CUR < 0x7FFE)
            {
                if (!(G.System_flg & SYS_GAME_OVER))
                {
                    MAIN.SaveData.Count_down_timer++;
                }
            }
            else
            {
                MAIN.SaveData.Count_down_timer = 0;
            }
        }

        if (STATUS_FLG_CUR & STAT_TIMER)
        {
            timer = COUNT_DOWN_TIMER_CUR;
            if (timer == 180)
            {
                flags = MAIN.System_flg | SYS_GAME_OVER;
                MAIN.Status_flg &= ~STAT_TIMER;
                G.Scenario.Movie_id = 2;
                G.Player.Be_flg = 0;

                MAIN.System_flg = flags | SYS_MOVIE;
                MAIN.Scenario.pMovie_buf = MAIN.pFree_work;

                goto check;
            }

            sprintf(ScratchPad->Text_buf, "%2d^%02d=%d%d", 2 - (timer / 60), 59 - (timer % 60), // UCHAR buffer passed as CHAR*: pointer-sign cast
                9 - (Count_down_cnt / 3), (Count_down_cnt + 1) % 10);

            Print14(0x7C, 0x20, MAIN.SaveData.Count_down_timer >= 120 ? 2 : 1, 0);
        }
        if (!(MAIN.System_flg & SYS_GAME_OVER))
        {
            Count_down_cnt++;
        }

        Sce_scheduler(pRoom_scd_main);

        Sce_scheduler_main();

        Sce_work_clr();

        Pc_event_ck();

        if (MAIN.System_flg & SYS_CUT_PENDING)
        {
            MAIN.System_flg &= ~SYS_CUT_PENDING;
            Cut_change();
        }
        else
        {
            Cut_check(0);
        }

        if (PL_DAMAGE_NO_CUR == 0 && (STOP_FLG_CUR & STOP_PAD_LOCK) && (STOP_FLG_CUR & STOP_ACTION) && !(MAIN.System_flg & SYS_STATUS_SCREEN))  //
        {
            if (Padd_data[0][0] == 0xFF && !(MAIN.System_flg & SYS_REQ_MASK) && !(G.Status_flg & STAT_DEMO_MODE))
            {
                MAIN.System_flg |= SYS_CONFIG;
                goto pause;
            }

            if ((G.Padd1 & (PAD_SELECT | PAD_START)) == (PAD_SELECT | PAD_START) && !(MAIN.System_flg & SYS_REQ_MASK))
            {
                MAIN.System_flg |= SYS_CONFIG;
                goto pause;
            }
            if ((MAIN.Padd1_trg & PAD_START) || (G.System_flg & SYS_REQ_MASK))
            {
            pause:
                MAIN.Menu_req = 1;
                MAIN.Scenario.Proc_flg_bak = STOP_FLG_CUR;
                MAIN.Stop_flg &= ~(STOP_PLAYER | STOP_ENEMY_MOVE | STOP_SCE);
            }
        }
        Em_main();
        if ((MAIN.Player.Zone_in_flg & 0x20) || !(MAIN.Stop_flg & STOP_PAD_LOCK))
        {
            KEY_CUR &= 0x4000 | 0x8000;
            KEY_TRG_CUR &= 0x4000 | 0x8000;
        }
        Player_move();
        STATUS_FLG_CUR &= ~STAT_WATER;
        Sce_at_check(PL_EM, SCE_PLAYER);

        Om_move();
        Om_trans();
        MAIN.pEm = MAIN.Enemy;
        Em_rnd_ptr_old = MAIN.Enemy_num;
        if (Em_rnd_ptr_old != 0)
        {
            do
            {
                if (EM_CUR->Be_flg & EM_BE_ENABLED)
                {
                    Em_rnd_ptr_old -= 1;
                    Joint_pos_trans(MAIN.pEm->Scale);
                    Pl_neck();
                    Joint_trans2(MAIN.pEm);
                }
                MAIN.pEm++;
            } while (Em_rnd_ptr_old != 0);
        }
        MAIN_PEM(UCHAR) = &MAIN.Player.Be_flg;  // pointer to a byte member, accessed as a byte (MAIN_PEM)
        Joint_pos_trans(G.Player.Scale);
        Pl_neck();
        Joint_trans2(PL_EM);
        Esp_move();
        Psp_trans();

        if (G.Player.Life < 0)
        {
            MAIN.System_flg |= SYS_GAME_OVER;
        }

    check:
        if (G.System_flg & SYS_GAME_OVER)
        {
            switch (VIEW(UCHAR, MAIN.Die_rno))  // UCHAR view of the field (plain access changes the code)
            {
            case 0:
                Die_fade_out();

                MAIN.Die_rno = 1;

            case 1:
                if (MAIN.SaveData.Fade_kido < 0)
                {
                    Die_move_end();
                    {
                    INT mask = STAT_NO_DIE_DEMO;
                    return (G.Status_flg & mask) == 0;
                }
                }
                break;
            }
        }
        Task_sleep(1);
    }
}

VOID Die_fade_out(VOID)
{
    SHORT unused1;  // unused local
    INT flags;
    SCHAR Rsv2;  // unused local

    G.SaveData.Mess_flg = 0;

    G.Menu_req = 0;

    flags = MAIN.System_flg & ~(SYS_FADE_OUT | SYS_DOOR_LOAD | SYS_DOOR_PROCESS | 0x8000000 | SYS_XA_VOICE | SYS_STATUS_MASK | SYS_PUSH_OBJ | SYS_CLIMB);
    MAIN.System_flg = flags;

    STOP_FLG_CUR &= ~STOP_PAD_LOCK;

    if (flags & 0x40000)
    {
        Sce_bgm_stop_all();

        Xa_cmd(2, 0);
        Task_sleep(1);
        G.SaveData.Fade_kido = -1;

        return;
    }

    if (G.Status_flg & STAT_DEMO_MODE)
    {
        G.Fade_hrate = 2;

        G.Fade_add = 0x800;
    }
    else
    {
        if (G.Status_flg < 0)
        {
            G.Fade_hrate = 2;
        }
        else
        {
            G.Fade_hrate = 1;
        }

        G.Fade_add = 0x100;
    }

    Evt_fade_start();
}

VOID Die_move_end(VOID)
{
    INT flags;

    G.SaveData.Fade1_kido = -1;

    G.SaveData.Mess_flg = 0;

    G.Curtain_kido = 0;
    flags = MAIN.System_flg & ~SYS_DIM;
    (*(volatile INT*)&MAIN.System_flg) = flags;  // INT* view of the field (a plain access changes the code)
    MAIN.System_flg = (flags & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;
    if (!(G.Status_flg & STAT_DEMO_MODE) && !(STATUS_FLG_NOW & STAT_NO_DIE_DEMO))
    {
        Die_demo();
    }
    Task_sleep(1);
    Vram_clr(0, 0, 0x140, 0x1E0);
    if (G.Fade_rtn != 0)
    {
        do
        {
            Task_sleep(1);
        } while (G.Fade_rtn != 0);
    }
    Snd_sys_init2();
    demo_tbl0.Pdemo_cnt = 0;
    Bgm_status_mask = 0xFF;
}

VOID Set_door(VOID)
{
    INT Door_bit;
    INT Same_room;
    INT Se_off;
    UCHAR Door_flg;
    IN_DOOR_WORK* pDoor;
    IN_DOOR_WORK* pDoor2;

    pDoor = SCE_AOT_ON_CUR;
    Door_anim_id[0] = pDoor->Anm_id;
    Door_snd_bank = pDoor->Snd_bank;
    Door_anim_id[1] = pDoor->Dtex_type;
    Door_anim_type = pDoor->Anm_type & 0x3F;
    Door_next_stage_room = pDoor->Next_room;
    Door_flg = pDoor->Anm_type;

    Pad_vib_init();

    Seqvol[0].right = 0x5F;
    Seqvol[0].left = 0x5F;
    Seqvol[1].right = 0x5F;
    Seqvol[1].left = 0x5F;
    Seqvol[2].right = 0x5F;
    Seqvol[2].left = 0x5F;
    Door_texture_load();
    Task_execute(1, &Door_main);
    Task_sleep(1);
    Snd_sys_core_set(Door_snd_bank);
    pDoor2 = MAIN.pSce_aot_on;
    G.Player.Pos_x = pDoor2->Next_pos_x;
    G.Player.Pos_y = pDoor2->Next_pos_y;
    G.Player.Pos_z = pDoor2->Next_pos_z;
    G.Player.Cdir_y = pDoor2->Next_cdir_y;
    G.Player.Ground = pDoor2->Next_pos_y;
    G.Player.Routine_0 = 0;
    G.Player.Old_x = pDoor2->Next_pos_x;
    Door_bit = Door_flg & 0xC0;
    G.Player.Old_y = pDoor2->Next_pos_y;
    G.Player.Old_z = pDoor2->Next_pos_z;
    if (!(Door_flg & 0x80))
    {
        Em_save_set();
        G.SaveData.Room_no_old = ROOM_NO_CUR;
        MAIN.SaveData.Room_no = Door_next_stage_room & 0x1F;
        pSca_hit_cur = Sca_hit_top;
        if (Door_next_stage_room >= 0x20U)
        {
            STAGE_NO_CUR = (Door_next_stage_room >> 5) + 0xFF;

            if (Flg_ck(&MAIN.SaveData.Scenario_flg, 0) != 0 && MAIN.SaveData.Stage_no < 2U)  // address as integer
            {
                MAIN.SaveData.Stage_no += 5;
            }
            Set_stage();
        }

        else
        {
            Set_room();
        }
    }
    Same_room = Door_bit & 0x80;
    if (G.System_flg & SYS_DOOR_PROCESS)
    {
        do
        {
            Task_sleep(1);
        } while (G.System_flg & SYS_DOOR_PROCESS);
    }
    if ((Door_bit & 0x80) == 0)
    {
        Set_view();
        Bg_draw();

        Psp_mask_set();

        Se_off = Door_bit & 0x40;
    }
    else
    {
        Cut_check(1);
        Se_off = Door_bit & 0x40;
    }
    if (Se_off == 0)
    {
        Snd_se_play(SE_BANK_CORE, 1, 0);
    }
}

VOID Menu_exec(VOID)
{
    INT* unused;    // unused local

    Task_sleep(1);

    if (G.System_flg & SYS_CONFIG)
    {
        Task_execute(1, &Config);
    }
    else
    {
        Task_execute(1, &Status);
    }

    MAIN.System_flg |= SYS_STATUS_SCREEN;
    Task_sleep(1);

    MAIN.System_flg &= ~SYS_CONFIG;
}

VOID Bg_rgb_clr(VOID)
{
    G.Clr_b = 0;
    G.Clr_g = 0;
    G.Clr_r = 0;
}

VOID Set_light(VECTOR* pV)
{
    GsF_LIGHT lt;
    UINT i;
    INT Dis;
    ROOM_WORK* pRoom;
    USHORT range;

    for (i = 0; i < 3; i++)
    {
        pRoom = ROOM_CUR;
        if (pRoom->Light[i].Mode == 0)
        {
            lt.vx = pV->vx - pRoom->Light[i].Flat.vx;
            lt.vy = pV->vy - pRoom->Light[i].Flat.vy;
            lt.vz = pV->vz - pRoom->Light[i].Flat.vz;
            Dis = SquareRoot0(lt.vx * lt.vx + lt.vz * lt.vz);
            pRoom = MAIN.pRoom;
            range = pRoom->Light[i].L;
            Dis = range - Dis;

            if (Dis < 0)
            {
                Dis = 0;
            }
            if (range != 0)
            {
                lt.r = pRoom->Light[i].Flat.r * Dis / range;
                lt.g = pRoom->Light[i].Flat.g * Dis / pRoom->Light[i].L;
                lt.b = pRoom->Light[i].Flat.b * Dis / pRoom->Light[i].L;
            }
            else
            {
                lt.r = lt.g = lt.b = 0;
            }
            GsSetFlatLight(i, &lt);
        }
        else
        {
            GsSetFlatLight(i, (GsF_LIGHT*)(i * sizeof(LIGHT_CUT_DATA) + (INT)pRoom->Light));  // pointer type conversion (no code); real width or sign conversion
        }
    }
}

VOID Demo_pl_load()
{
    if (DEMO_IDX_HALF >= 3U)
    {
        DEMO_IDX_HALF = 0;
    }

    Cd_read_wait(DEMO_IDX_HALF + PDEMO0_DAT, FREE_WORK_CUR);
    Memcpy((UCHAR*)&demo_tbl0, MAIN.pFree_work, 0x994);  // pointer type conversion (no code)
    DEMO_IDX_HALF += 1;
    demo_tbl0.Key_bak = KEY_IDX_CUR;

    MAIN.SaveData.Key_idx = (MAIN.SaveData.Key_idx & 0xFC) | 1;

    G.Player.Id = demo_tbl0.Pl_id;
    G.SaveData.Pl_id = demo_tbl0.Pl_id;
    G.SaveData.Pl_chg_no = demo_tbl0.Pl_id;
    G.SaveData.Stage_no = demo_tbl0.Stage_no;

    G.SaveData.Room_no = VIEW(USHORT, demo_tbl0);  // USHORT view of the field (plain access changes the code)

    G.SaveData.Room_no_old = demo_tbl0.Room_no_old;

    G.SaveData.Equip_no = demo_tbl0.Equip_no;
    G.SaveData.Item_num = demo_tbl0.Item_num;
    Memcpy(&MAIN.SaveData.Item_work[0].Id, demo_tbl0.Item_work, 0x18);
    demo_tbl0.Pdemo_cnt = 1;
    G.Player.Old_x = demo_tbl0.Pos_x;
    G.Player.Old_z = demo_tbl0.Pos_z;
    G.Player.Cdir_y = demo_tbl0.Cdir_y;
    Snd_sys_arms_set(G.Player.Type, OVERLAY_AREA);
}

VOID Pl_data_init(VOID)
{
    Status_init();

    G.SaveData.Room_no = 6;

    G.Player.Old_x = 0x4268;
    G.Player.Old_z = 0x1388;

    G.SaveData.Stage_no = 0;

    G.Player.Cdir_y = 0xC00;
    STATUS_FLG_CUR |= STAT_NEW_GAME;
}

VOID Demo_end_ck(VOID)
{
    if (G.Status_flg & STAT_DEMO_MODE)
    {
        if (demo_tbl0.Pdemo_len >= demo_tbl0.Pdemo_cnt + 0xA)
        {
            if (((Pad_raw_get(0) & 0xFFFF) && !(G.System_flg & SYS_GAME_OVER)) || (G.System_flg & SYS_GAME_OVER))
            {
                goto demo_reset;
            }
        }
        else
        {
        demo_reset:

            demo_tbl0.Pdemo_cnt = 1;
            Snd_bgm_fade_ON(-3, 0x2B);
            MAIN.System_flg |= SYS_GAME_OVER;
            STOP_FLG_CUR &= ~(STOP_PLAYER | STOP_SUB_PLAYER | STOP_ENEMY_MOVE | STOP_ESP_FX | STOP_SCE | STOP_PAD_LOCK);

            G.SaveData.Key_idx = demo_tbl0.Key_bak;
        }
    }
}

VOID Cd_read(INT File_no, VOID* Address)
{
    INT pRet;
    INT Cnt;

top:
    while (Cd_file_read(File_no, Address) != 0)
    {
    }

    Cnt = 0;
read:
    pRet = CdReadSync(1, 0);
    if (G.Vloop == 0)
    {
        Cnt += 1;
    }
    else
    {
        Cnt += 2;
    }

    if (pRet == 0)
    {
        if (Cd_sum_ck() != 0)
        {
            return;
        }
        goto top;
    }
    if (pRet == -1 || Cnt >= 0x385)
    {
        goto top;
    }
    if (G.Status_flg & STAT_CD_POLL)
    {
        VSync(0);
    }
    else
    {
        Task_sleep(1);
    }

    goto read;
}

VOID Cd_read_wait(INT File_no, VOID* Address)
{
    STATUS_FLG_CUR |= STAT_CD_POLL;
    Cd_read(File_no, Address);
    MAIN.Status_flg &= ~STAT_CD_POLL;
}

VOID Load_mess_disp(VOID)
{
    INT Msg_id;

    if (G.Status_flg & STAT_DEMO_MODE)
    {
        Msg_id = 0x5D;
    }
    else
    {
        Msg_id = 0x5B;

        if (G.System_flg & SYS_CARD_LOAD)
        {
            Msg_id = 0x5C;
        }
    }
    Mess_set(Msg_id, 0);
    Task_exit();
}

VOID Die_demo(VOID)
{
    UCHAR Rno[2];
    SHORT Kido;
    SHORT Cnt;
    UCHAR Pri;
    UCHAR i;
    ENEMY_WORK* pPl;

    Cd_read_wait(8, OVERLAY_AREA);

    G.Vram_cur.Tpage_clut = 0x1F06;
    Mapping_tim(OVERLAY_AREA);
    Gs_packet_p[0] = OVERLAY_AREA;
    Gs_packet_p[1] = SYS_PKT_BUF1;
    ScratchPad->Rect.attribute = 0x60000000;
    RECT_XY = 0xFF88FF60;
    RECT_WH = 0xF00140;
    RECT_RGB = 0;
    View_ref.vpx = PL_EM->Pos_x + 0x1388;
    View_ref.vpy = PL_EM->Pos_y - 0x1B58;
    View_ref.vpz = PL_EM->Pos_z;
    View_ref.vrx = PL_EM->Pos_x;
    View_ref.vry = PL_EM->Pos_y - 0x3E8;
    View_ref.vrz = PL_EM->Pos_z;
    GsSetRefView2(&View_ref);
    Kido = 0;
    Cnt = 0x3B;
    Pri = 0xFF;
    Rno[1] = 0;
    Rno[0] = 0;
    while (1)
    {
        ScratchPad->Spr.tpage = 6;
        SPR_MXY = 0;
        ScratchPad->Spr.scalex = 0x1000;
        ScratchPad->Spr.rotate = 0;
        SPR_RGB = 0x808080;
        switch (Rno[0])
        {
        case 0:
            Snd_bgm_fade_ON(-3, 0x2B);
            G.Fade_hrate = 1;
            G.Fade_add = -0x180;
            Evt_fade_start();
            Rno[0] = 1;
        case 1:
            if (G.SaveData.Fade_kido >= 0)
            {
                break;
            }

            Rno[0] = 2;
        case 2:
            if (Kido < 0xFF)
            {
                ScratchPad->Rect.r = ScratchPad->Rect.g = ScratchPad->Rect.b = Kido;
            }

            Kido += 3;
            if (Kido >= 0x40)
            {
                Die_wave_draw(Cnt);
                Cnt--;
            }

            if (Cnt < 4)
            {
                Rno[0] = 3;
            }
            break;
        case 3:
            Die_wave_draw(Cnt);
            Rno[1]++;
            if (Rno[1] >= 0x31)
            {
                Rno[0] = 4;
                Rno[1] = 0;
            }
            break;
        case 4:
            G.Fade_hrate = 2;
            MAIN.Fade_add = 0x180;
            Cnt = 4;
            Evt_fade_start();
            Pri = 0xFF;
            Rno[0] = 5;
        case 5:
            Die_wave_draw(Cnt);
            Cnt++;
        case 6:
            if (G.SaveData.Fade_kido < 0)
            {
                goto end;
            }
            break;
        }

        if ((MAIN.SaveData.Key & 0x4000) && Rno[0] != 0)
        {
            G.SaveData.Fade_kido = -1;
            Evt_fade_req_set(2, 0x800);
            goto end;
        }
        for (i = 0; i < 4; i++)
        {
            Set_sp_work(&Die_sp_work[i], 0x1000);
            GsSortSprite(&ScratchPad->Spr, MAIN.Ot2 + G.Cbuf_id, 1);
        }
        GsSortBoxFill(&ScratchPad->Rect, MAIN.Ot2 + CBUF_ID_CUR, 0);
        if ((SCHAR)Pri >= 0)  // real width or sign conversion
        {
            Set_sp_work(&Die_txt_sp, 0x1000);
            GsSortSprite(&ScratchPad->Spr, MAIN.Ot0 + MAIN.Cbuf_id, Pri); // plain MAIN.Cbuf_id changes scheduling
        }
        pPl = PL_EM;
        MAIN.Player.Cdir_y += 8;
        Task_sleep(1);
        if (pPl->Be_flg != 0)
        {
            G.pEm = pPl;
            Joint_pos_trans(G.Player.Scale);
            Pl_neck();
            Joint_trans2(PL_EM);
        }
    }

end:
    Gs_packet_p[0] = &MAIN.Pkt_buf;  // address as integer
    Gs_packet_p[1] = &MAIN.Pkt_buf[1];  // address as integer
}

VOID Set_sp_work(pTpl, Scale_y)
SP_WORK* pTpl;  // parameter
INT Scale_y;  // parameter
{
    UINT clut_y;
    USHORT uv;

    ScratchPad->Spr.attribute = pTpl->attribute;
    SPR_XY = SP_WORK_XY(pTpl);
    SPR_WH = SP_WORK_WH(pTpl);
    clut_y = pTpl->clut_y;

    uv = SP_WORK_UV(pTpl);

    ScratchPad->Spr.scaley = Scale_y;
    SPR_CXY = clut_y << 0x10;
    SPR_UV = uv;
}

VOID Die_wave_draw(INT t)
{
    INT scale;
    INT height;
    INT col;
    UINT Wave;
    INT y0;

    col = t * 8;
    Wave_sp.h = 0x40;
    Wave_sp.v = 0;
    Wave_sp.w = 1;
    scale = col * col / 16 + 0x1600;
    height = scale & 0xFFFF;
    if (height < 0)
    {
        height += 0x7F;
    }
    t *= t;
    y0 = -(height >> 7);
    col = 0;
    do
    {
        Set_sp_work(&Wave_sp, scale & 0xFFFF);
        ScratchPad->Spr.u = col;
        ScratchPad->Spr.x = col - 0x80;
        Wave = rsin(col << 8) * t;
        ScratchPad->Spr.y = y0 - (Wave >> 0xD);
        GsSortSprite(&ScratchPad->Spr, &MAIN.Ot1[G.Cbuf_id], 0U);
        col += 1;
    } while (col < 0x100);
}

// quadrant sprites of the death screen
SP_WORK Die_sp_work[4] =
{
    { 0x01000000, -160, -120, 160, 120, 0, 64, 0x01FF },   // 0 //
    { 0x01400000, -160, 0, 160, 120, 0, 64, 0x01FF },      // 1 //
    { 0x01800000, 0, -120, 160, 120, 0, 64, 0x01FF },      // 2 //
    { 0x01C00000, 0, 0, 160, 120, 0, 64, 0x01FF },         // 3 //
};

SP_WORK Die_txt_sp = { 0x41000000, -128, -32, 255, 64, 0, 0, 0x01FF };    // text sprite of the death screen

SP_WORK Wave_sp = { 0x41000000, -128, -32, 255, 1, 0, 0, 0x01FF };  // template sprite of the wave distortion of the death screen text
