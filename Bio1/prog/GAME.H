/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\GAME.H
 */

#ifndef GAME_H
#define GAME_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBSND.H>

struct SP_WORK
{
    UINT attribute; // sprite attribute word
    SHORT x;        // screen x
    SHORT y;        // screen y
    USHORT w;       // width
    USHORT h;       // height
    UCHAR u;        // texture u
    UCHAR v;        // texture v
    USHORT clut_y;  // palette row (y)
};

// the template fields as the words that GsSPRITE takes them in (x and y, w and h, u and v), read as one value
#define SP_WORK_XY(p) VIEW(INT, (p)->x)
#define SP_WORK_WH(p) VIEW(INT, (p)->w)
#define SP_WORK_UV(p) VIEW(USHORT, (p)->u)

typedef VOID (*GAME_FUNC)(VOID);

/*
    program entry of the game: reset the clear colour, initialise sound and the game session
    and run the game loop, then chain to the next task: 0x15E when the attract demo ended,
    0x153 (after adding the elapsed play time) when the loop returned 0, otherwise 0x154
 */
VOID Game_start(VOID);

/*
    start a game session: clear the work area, then load the continue data when game flag
    0x10000000 is set (otherwise a new game or attract demo record), set the player's health
    from the character and difficulty flags, reset the item slot, room action and collision
    hit pools, set up the player, effects and sound, and load the stage
 */
VOID Title_game_init(VOID);

/*
    run the game frame by frame until the player dies: menu, fade and pause requests, the
    scripted Time_cnt timer, script execution, camera cuts, then entity, player, object and
    effect updates; on death run the fade-out and death sequence and return 1 unless bit 31
    of the status flags is set
 */
INT Game_loop(VOID);

/*
    start the fade-out that follows a death or a timeout: clear the menu choice and most game
    flags; with game flag 0x40000 stop all BGM and voice, otherwise start a fade (type 2, step 0x800
    in the attract demo; else type 2 when status bit 31 is set or type 1, step 0x100)
 */
VOID Die_fade_out(VOID);

/*
    death sequence: clear the room light state, menu choice and screen dim, play the death
    screen unless in the attract demo or status bit 31 is set, clear VRAM, wait for any sound
    fade to end, shut the sound down and reset the BGM state
 */
VOID Die_move_end(VOID);

/*
    take the current door: read its record, reset the sequence volumes, load the door texture and
    run the door task, place the player at the destination and, unless the door stays inside the
    room, load the next stage or room; then rebuild the view and background (or refresh the
    camera cut for an in-room door) and play the door sound unless the door is silent
 */
VOID Set_door(VOID);

/*
    run the config menu (when game flag 0x400000 is set) or the status menu as a task, then
    set game flag 0x8000 and clear 0x400000
 */
VOID Menu_exec(VOID);

/*
    set the clear colour to black
 */
VOID Bg_rgb_clr(VOID);

/*
    set the three flat lights from the room light table: lights of type 0 are attenuated
    linearly by their horizontal distance from pV within their range, the others are copied
 */
VOID Set_light(VECTOR* pV);

/*
    load the next attract demo record (file 0x14 plus the demo index, which wraps after
    three), copy its player state and Item_work to the save data and force key config preset 1
 */
VOID Demo_pl_load();

/*
    set up a new game: initialise the status, start in room 6 of stage 0 at the fixed position
    and angle, and set status flag 0x20000000
 */
VOID Pl_data_init(VOID);

/*
    during the attract demo: when the recording is within 10 frames of its end, a button is
    pressed or game flag 0x1000000 is set, reset its timer, fade out the BGM, request the end
    of the demo and restore the saved controller configuration
 */
VOID Demo_end_ck(VOID);

/*
    read file File_no of the CD file table to Address and wait until the read completes and
    its checksum verifies, restarting it on failure or timeout; waits between polls by yielding
    to the scheduler, or by VSync while status flag 0x100000 is set
 */
VOID Cd_read(INT File_no, VOID* Address);

/*
    read a file with Cd_read while status flag 0x100000 is set (blocking mode)
 */
VOID Cd_read_wait(INT File_no, VOID* Address);

/*
    show the loading message (0x5D in the attract demo, 0x5C when game flag 0x10000000 is set,
    0x5B otherwise) and end the task
 */
VOID Load_mess_disp(VOID);

/*
    death screen: load its graphics, aim the camera at the player, fade in, then draw the four
    background sprites, the text and the wave distortion while the player turns, until the
    fade out completes or the button is pressed
 */
VOID Die_demo(VOID);

/*
    load the scratchpad sprite from the template pTpl with the vertical scale Scale_y
 */
VOID Set_sp_work(SP_WORK* pTpl, INT Scale_y);

/*
    draw the template as 256 one-pixel columns displaced vertically by a sine wave whose
    amplitude grows with t
 */
VOID Die_wave_draw(INT t);

struct PDEMO
{
    UCHAR Room_no;          // room the demo starts in
    SCHAR Stage_no;         // stage the demo starts in
    UCHAR Item_num;         // Item_work count at demo start
    UCHAR Equip_no;         // equipped Item_work slot at demo start
    UCHAR Pl_id;            // player character of the demo
    UCHAR Dummy05;          // padding
    SHORT Pos_x;            // player start position x
    SHORT Pos_z;            // player start position z
    SHORT Cdir_y;           // player start facing angle
    SHORT Dummy0c;          // padding
    USHORT Pdemo_cnt;       // frame counter of the playback
    USHORT Pdemo_len;       // number of recorded frames
    SHORT Dummy12;          // padding
    UCHAR Room_no_old;      // previous room at demo start
    UCHAR Dummy15[3];       // padding
    UCHAR Item_work[0x18];  // Item_work and Item_work2 contents at demo start (0x18 bytes, copied to Item_work)
    USHORT Padd[1200];      // recorded pad word of each frame
    USHORT Key_bak;         // controller type saved while the demo runs
    SHORT Dummy992;         // unused tail halfword of the demo record (the record is 0x994 bytes)
};

extern INT Game_unused_word;                       // work word cleared at session start, never read
extern PDEMO demo_tbl0;                          // attract demo record being played
extern AT_WORK* pSca_hit_cur;                // next free collision hit record of the pool
extern AT_WORK* Sca_hit_top;           // first pool record after the player's, the reset point of the pool
extern AT_WORK Sca_at_tbl_stat[64];            // collision hit records; the first belongs to the player
extern UCHAR* pRoom_scd_main;                 // main script of the current room, run every frame
extern UCHAR Door_next_stage_room;             // destination stage and room byte of the door being taken
extern INT Play_vsync;                        // VSync count when play-time counting last started; set at the start of the game loop
extern SP_WORK Die_sp_work[4];                    // quadrant sprites of the death screen
extern SP_WORK Die_txt_sp;                    // text sprite of the death screen
extern SP_WORK Wave_sp;                             // template sprite of the wave distortion of the death screen text

#endif
