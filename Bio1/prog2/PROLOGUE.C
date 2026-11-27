/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog2\PROLOGUE.C
 */

#include "PROLOGUE.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID main(VOID)
{
    RECT rect;

    FUNC(SND_TITLE_INIT)();  // Snd_title_init
    G.pFree_work = OVERLAY_AREA;  // work buffer for the movie
    rect.x = 0;
    rect.y = G.Cbuf_id != 0 ? 0xF0 : 0;
    rect.w = 0x1E0;
    rect.h = 0xF0;
    FUNC(VSYNC)(0);  // VSync
    FUNC(CLEAR_IMAGE)(&rect, 0, 0, 0);  // ClearImage
    FUNC(DRAW_SYNC)(0);  // DrawSync
    FUNC(TASK_SLEEP)(1);  // Task_sleep
    rect.y = G.Cbuf_id != 0 ? 0xF0 : 0;
    FUNC(VSYNC)(0);  // VSync
    FUNC(CLEAR_IMAGE)(&rect, 0, 0, 0);  // ClearImage
    FUNC(DRAW_SYNC)(0);  // DrawSync
    FUNC(VSYNC)(0);  // VSync
    FUNC(GS_INIT_GRAPH2)(0x140, 0xF0, 0, 0, 1);  // GsInitGraph2
    G.Scenario.Movie_id = 1;  // prologue movie
    G.Scenario.pMovie_buf = G.pFree_work;
    G.System_flg |= SYS_MOVIE;  // request movie playback from the main loop
    FUNC(TASK_SLEEP)(1);  // Task_sleep
    G.System_flg |= SYS_GRAPH_RESET;  // request graphics re-initialisation from the main loop
    FUNC(TASK_SLEEP)(1);  // Task_sleep
    FUNC(SND_SYS_END)();  // Snd_sys_end
    FUNC(TASK_CHAIN)FUNC(GAME_START);  // Task_chain, Game_start
}
