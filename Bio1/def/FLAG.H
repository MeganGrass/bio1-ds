/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\def\FLAG.H
 */

#ifndef FLAG_H
#define FLAG_H

// G.Status_flg (prefix STAT_): scene status and system / mode state of the game (bits 0-4, 16-18, 20-29, 31); every bit of this
// word is a STAT_ name and is used only with G.Status_flg
// bits 0-3 are cleared on every room set-up (STAT_SCENE_CLR_MASK); bit 3 has no setter in this tree
#define STAT_WATER           (1UL << 0)  // player stands in water: set by Sce_Water every frame, cleared after Player_move; changes the
                                         // player's life floor, the shell casing and the weapon routines
#define STAT_QUAKE           (1UL << 1)  // screen quake enabled (Door_quake_set is called by the main loop); saved and cleared while the
                                         // memory card screen runs
#define STAT_BG_BORDER       (1UL << 2)  // the background layer is drawn as a border (Door_border_set) instead of the image; the config screen
                                         // moves it to and from Key_idx bit 4
#define STAT_SHADOW_CLAMP    (1UL << 3)  // shadow projection depth is clamped to 0xFF0 (Kage draw in PLAYER.C); no setter found
#define STAT_SCENE_CLR_MASK  (STAT_WATER | STAT_QUAKE | STAT_BG_BORDER | STAT_SHADOW_CLAMP) // bits cleared by Set_room
#define STAT_BG_DRAW         (1UL << 4)  // Bg_draw is copying the room background: pad vibration is held off meanwhile
#define STAT_ARRANGE_EASY    (1UL << 16) // Arrange mode with the Beginner bonus (Dc_mode 3, together with STAT_ARRANGE): normal damage table
                                         // and doubled item counts like easy mode
#define STAT_ARRANGE         (1UL << 17) // Arrange mode (Dc_mode 2 and 3); selects the alternate room and checksum tables
#define STAT_EASY_MODE       (1UL << 18) // easy mode (Dc_mode 1): more life and starting items
#define STAT_CD_POLL         (1UL << 20) // Cd_read_wait is running: Cd_read polls with VSync instead of sleeping the task
#define STAT_CARD_SE         (1UL << 21) // memory card screen is up: only the first 0x10 sound effects of the core bank are playable
                                         // (Snd_se_play)
#define STAT_STAIRS_ZONE     (1UL << 22) // player is inside a stairs zone (set by Sce_stairs_x and Sce_stairs_z); read by the stairs move
                                         // pl_mv08
#define STAT_DOOR_WAIT       (1UL << 23) // door sequence is held until the old BGM has faded out: set by Door_init, cleared by the BGM
                                         // routines of SND.C or by Door_op_flg_clr
#define STAT_STAIRS_ON       (1UL << 24) // player is climbing stairs: set by pl_mv08 after the turn and cleared when the climb ends
#define STAT_PSP_HIDE        (1UL << 25) // room sprite layer is not drawn (Psp_trans); no setter found
#define STAT_PL_ALT_MODEL    (1UL << 26) // alternate player model: ids 0 and 1 load model entry 0x33 (Emd_load); set by Set_room when
                                         // Pl_chg_no has bit 3
#define STAT_TIMER           (1UL << 27) // countdown timer shown (Count_down_timer), cleared when it reaches 180
#define STAT_DEMO_MODE       (1UL << 28) // demo playback: the pad word comes from the recorded input
#define STAT_NEW_GAME        (1UL << 29) // a new game was started (Pl_data_init); kept through game end and soft reset
#define STAT_RESET_KEEP_MASK (STAT_NEW_GAME | (1UL << 19)) // bits that survive the end of a game and the soft reset (STAT_NEW_GAME and the
                                                           // unnamed 0x80000)
#define STAT_NO_DIE_DEMO     (1UL << 31) // sign bit: Die_move_end skips Die_demo and the death fade uses the short rate; no setter found

// G.System_flg (prefix SYS_): script flag bank 5: scene, player and screen status, system requests and screen modes; every bit
// of this word is a SYS_ name and is used only with G.System_flg; room scripts can set and clear any of the bits
#define SYS_MIRROR_MATRIX (1UL << 0)  // mirror reflection is drawn (Mirror_in_view)
#define SYS_MIRROR_SIDE   (1UL << 1)  // mirror side passed to Mirror_in_view; the script command Mirror_set loads both mirror bits from its
                                      // mode byte
#define SYS_CUT_HOLD      (1UL << 2)  // camera cut changes are deferred: Cut_check only records the new cut and raises SYS_CUT_PENDING
#define SYS_ROOM_CLR_MASK (SYS_MIRROR_MATRIX | SYS_MIRROR_SIDE | SYS_CUT_HOLD | (1UL << 3)) // bits cleared by Set_room and by Title_game_init
#define SYS_LADDER        (1UL << 4)  // a ladder zone was entered (set by Sce_ladder_set): the action on the zone is the ladder climb
                                      // (Routine_2 0xB) instead of 0x11; cleared when the climb starts
#define SYS_CUT_PENDING   (1UL << 5)  // deferred camera cut change: Game_loop clears it and calls Cut_change
#define SYS_PUSH_OBJ      (1UL << 6)  // player is pushing a movable object: set by Om_move when it hit something, tested by the player
                                      // routines
#define SYS_CLIMB         (1UL << 7)  // player is on a climbable object (Oma_pl_updown_ck): the pad turns the player instead of walking
#define SYS_REQ_FILE      (1UL << 8)  // status request: open the file pickup screen (St_main_rtn 8); set when an Aot of id 0xD is picked up
#define SYS_REQ_TYPE5     (1UL << 9)  // status request: status screen type 5 (no setter in this tree, room scripts can raise it)
#define SYS_REQ_ITEM_SCE  (1UL << 10) // status request: item acquired by script (Sce_Item_get, status screen type 4)
#define SYS_REQ_ITEM_GET  (1UL << 11) // status request: item picked up from the floor or a drawer (status screen type 3)
#define SYS_REQ_ITEMBOX   (1UL << 12) // status request: item box (Sce_Itembox, status screen type 2)
#define SYS_REQ_ITEM_USE  (1UL << 13) // status request: an item was used up (status screen type 1)
#define SYS_REQ_MASK      (SYS_REQ_FILE | SYS_REQ_TYPE5 | SYS_REQ_ITEM_SCE | SYS_REQ_ITEM_GET | SYS_REQ_ITEMBOX | SYS_REQ_ITEM_USE | (1UL << 14)) // any status screen request pending: Game_loop opens
                                                                                                                                                  // the status screen
#define SYS_STATUS_SCREEN (1UL << 15) // status or config screen task is running (set by Menu_exec after Task_execute)
#define SYS_STATUS_MASK   (SYS_REQ_MASK | SYS_STATUS_SCREEN) // all status screen bits: requests and SYS_STATUS_SCREEN; they move the
                                                             // message window up
#define SYS_DIM           (1UL << 16) // screen dim requested: the main loop darkens Curtain_kido step by step
#define SYS_XA_VOICE      (1UL << 17) // an XA voice is streaming: set by Xa_cmd users, cleared when the stream stops
#define SYS_MOVIE         (1UL << 18) // movie playback requested: the main loop plays Movie_id and clears it
#define SYS_GRAPH_RESET   (1UL << 19) // graphics re-initialisation requested: the main loop calls Init_graph(1)
#define SYS_CAM_LOCK      (1UL << 20) // camera cuts are locked by a script (the Cut_chg script command sets it, Cut_old clears it); Cut_check
                                      // is skipped
#define SYS_CONFIG        (1UL << 22) // the menu request is the config screen (Select+Start or no pad) instead of the status screen; also lets
                                      // pad vibration run
#define SYS_PLAYER        (1UL << 23) // second character (Pl_id & 3 not zero, set by Load_pop): selects the second Rdt file and the second
                                      // file book
#define SYS_GAME_OVER     (1UL << 24) // the game ends: player died, demo finished or timer ran out; stops the elapsed-time counter and starts
                                      // the Die_fade_out sequence
#define SYS_DOOR_LOAD     (1UL << 25) // the screen change is a room change (door) and not the menu: Menu_req 1 calls Set_door; set at game
                                      // start
#define SYS_DOOR_PROCESS  (1UL << 26) // door sequence or room load is running: Cut_set skips the view and background, loaders read data,
                                      // Set_room waits for it to clear
#define SYS_CARD_LOAD     (1UL << 28) // the game was loaded from the memory card (Load_pop); Title_game_init then restores the save data block
                                      // from the card buffer
#define SYS_FADE_OUT      (1UL << 29) // full-screen fade running: set by Evt_fade_req_set, cleared by the main loop when it ends
#define SYS_BG_MODE_CLEAR (1UL << 30) // screen mode: solid clear colour (Clr_r/g/b)
#define SYS_BG_MODE_IMAGE (1UL << 31) // screen mode: background image (Bg_img_trans or Door_border_set); the sign bit
#define SYS_BG_MODE_MASK  (SYS_BG_MODE_CLEAR | SYS_BG_MODE_IMAGE) // screen mode bits

// G.Stop_flg (prefix STOP_): NOTE a SET bit means that part of the game RUNS; a clear bit stops it (the opposite of bio2, where
// a set STOP_ bit stops the part). Messages clear the bits in Stop_data while they are shown
#define STOP_PLAYER     (1UL << 0) // player routine runs (Player_move); with it clear the message end flushes the pad
#define STOP_SUB_PLAYER (1UL << 1) // partner routine runs (Spl)
#define STOP_ENEMY_MOVE (1UL << 2) // enemy routines run (tested first in every enemy move function)
#define STOP_ESP_FX     (1UL << 3) // effect and particle routines run
#define STOP_ACTION     (1UL << 6) // the player may start an action (examine, item, door, save, menu); cleared while one is running
#define STOP_SCE        (1UL << 7) // scenario tasks run (Sce_task); also set while the room init script runs
#define STOP_PAD_LOCK   (1UL << 8) // the pad reaches the player; with it clear only the cancel and confirm buttons stay
#define STOP_DEMO_TIMER (1UL << 9) // the recorded pad data of the demo advances
#define STOP_ALL        ((1UL << 16) - 1) // every bit set (the value restored after a door change and after the item box animation)
#define STOP_PLAY_MASK  (STOP_PLAYER | STOP_SUB_PLAYER | STOP_ENEMY_MOVE | STOP_ESP_FX | STOP_ACTION | STOP_PAD_LOCK) // player, partner, enemies, effects, actions and
                                                                                                                      // pad: the bits the panel and slide events switch
                                                                                                                      // off and back on
// bits 0x10, 0x20 and 0x400-0x8000 have no isolated user

// G.Room_flg (prefix ROOM_): 64-bit bit vector (script flag bank 4, Flg_on/Flg_ck). Word 0 is cleared on every room set-up,
// word 1 every frame (bits 0x20-0x3F)
// the indices below are the ones used by name from code; the rest are set from room data (ENEMY_WORK.Set_flg_no)
#define ROOM_PANEL 0x1D // the panel puzzle is shown (Pc_event_ck calls Sce_panel_disp)
#define ROOM_COM   0x1E // the computer panel is shown (Sce_com_disp); EM0C reuses it as its enraged state
#define ROOM_SLIDE 0x1F // the slide panel is shown (Sce_slide_disp); EM0C reuses it as its dead state

// ENEMY_WORK.Be_flg and PLAYER_WORK.Be_flg (bio2 EM_BE_*; the bit values differ). Bits 0-4 are the slot state, bits 5-7 hold
// the attack class that hit the slot
#define EM_BE_ENABLED     (1UL << 0) // slot in use: every enemy loop skips the slot while it is clear
#define EM_BE_NO_HIT      (1UL << 1) // hit checks off: shots and melee pass through (dying and scripted enemies)
#define EM_BE_NO_WALL     (1UL << 2) // wall collision off (Sca_ck is skipped)
#define EM_BE_NO_OBJ      (1UL << 3) // object collision off (Oma is skipped)
#define EM_BE_SLIDE_AXIS  (1UL << 4) // axis of the last wall slide (Sca_ck)
#define EM_BE_STATE_MASK  (EM_BE_ENABLED | EM_BE_NO_HIT | EM_BE_NO_WALL | EM_BE_NO_OBJ | EM_BE_SLIDE_AXIS) // the slot state bits: ANDing with it clears the
                                                                                                           // attack class
#define EM_BE_ATTACK_MASK (7UL << 5) // attack class bits 0x20, 0x40 and 0x80, set by the damage code and read by the Damage routines

// Mess_flg: message window state (bio2 MESS_FLG_*)
#define MESS_FLG_ACTIVE (1UL << 7) // a message window is open: set by Mess_set, cleared when it is closed
#define MESS_FLG_ANSWER (1UL << 0) // answer chosen in a two-choice message (toggled by the cursor)

#endif      // FLAG_H
