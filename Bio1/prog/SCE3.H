/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SCE3.H
 */

#ifndef SCE3_H
#define SCE3_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

struct COM_ROW_REC
{
    UCHAR u;       // texture u of the row
    UCHAR pad1[3]; // padding
    UCHAR v;       // texture v of the row
    UCHAR pad5[3]; // padding
    SHORT width;   // row length in characters
    SHORT height;  // row height in text lines
    SHORT tpage;   // texture page
    SHORT pad0E;   // padding
};

struct COM_POINT_REC
{
    UCHAR x; // screen x of the point
    UCHAR y; // screen y of the point
};

struct COM_QUAD_REC
{
    USHORT x;    // x offset from the draw origin
    USHORT y;    // y offset from the draw origin
    UCHAR u;     // texture u
    UCHAR v;     // texture v
    USHORT w;    // width in pixels
    USHORT h;    // height in pixels
    UCHAR flags; // bit 0 repeats downward instead of rightward, bits 6-7 are the blend mode
    UCHAR count; // repeat count, 0 draws once
};

struct COM_WND_ROW_REC
{
    UCHAR u;        // texture u of the row's text image
    UCHAR pad1[3];  // padding
    UCHAR v;        // texture v of the row's text image
    UCHAR pad5[3];  // padding

    SHORT width;    // text width in 8-pixel units, 0 draws nothing
    SHORT h;        // text height in 16-pixel lines
    USHORT tpage;   // texture page of the text image
    UCHAR pad0E[2]; // padding
};

struct COM_WND_WORK
{
    UCHAR enabled;          // 1 while the window is in use
    UCHAR nRow;             // number of row descriptors; window 3 reuses it as the cancel flag
    USHORT x;               // screen x
    USHORT y;               // screen y
    USHORT cols;            // width in 16-pixel cells
    USHORT rows;            // height in 16-pixel cells
    UCHAR pad0A[2];         // padding
    COM_WND_ROW_REC* pRow;  // row descriptors of the window text
    UCHAR Anm_req;          // animation request: bit 0 runs it, bit 7 closes the window, 0 idle
    UCHAR Anm_sts;          // 0 before the animation starts, nonzero while it runs
    SHORT alpha;            // Kido, 0 hidden to 0xFF full
    SHORT visible;          // 1 while fading, drawn semi-transparent
    SHORT Keyboard_flg;     // 1 on the keyboard window: fades only up to half Kido in smaller steps
    UCHAR pad18[2];         // padding
    SHORT Repeat_timer;     // key repeat timer of the keyboard window
};

struct COM_TEXT_REC
{
    UCHAR enabled; // 1 while the text is shown
    UCHAR Wnd_idx; // window the text is drawn in
    SHORT x;       // text cell x in the window
    SHORT y;       // text cell y in the window
    UCHAR text[9]; // character codes (glyph index + 1), 0 ends the text
    UCHAR masked;  // 1 draws '*' in place of the characters
    UCHAR pad[2];  // padding
};

struct COM_CURSOR_WORK
{
    UCHAR Input_en;     // 1 while the caret or Sel_no bar is active
    UCHAR Wnd_idx;      // window holding the caret
    SHORT Base_x;       // cell x origin of the field or menu
    SHORT Base_y;       // cell y origin of the field or menu
    UCHAR mode;         // 0 text caret, 1 menu Sel_no bar
    UCHAR pos;          // caret column or selected menu row
    UCHAR Bar_w;        // Sel_no bar width in 16-pixel cells
    UCHAR Text_idx;     // text entry the caret edits
    UCHAR Blink_cnt;    // frame counter, bit 4 drives the blink
    UCHAR Blink_hold;   // frames the keyboard cursor stays lit after a key press
};

struct COM_LOGO_WORK
{
    UCHAR active;           // 1 while the slot is in use
    UCHAR behaviour;        // behaviour routine index: 0=hub 1=wedge A 2=wedge B 3=wordmark
    UCHAR substep;          // step within the behaviour routine
    UCHAR Dummy03[5];       // padding
    INT x;                  // screen x, 16.16 fixed point, centre origin
    INT y;                  // screen y, 16.16 fixed point, centre origin
    UCHAR pad10[4];         // padding
    USHORT timer;           // frames left in the current step
    SHORT Hub_sts;          // hub progress seen by the wedges and wordmark: 0=spawning 1=held 2=retired
    COM_LOGO_WORK* parent;  // slot that spawned this one and is watched for Hub_sts
    SCHAR Tex_u;            // sprite texture u
    SCHAR Tex_v;            // sprite texture v
    SHORT width;            // sprite width in pixels
    SHORT height;           // sprite height in pixels
    SHORT Ot_pri;           // ordering table priority
    SHORT Pivot_x;          // sprite rotation pivot x
    SHORT Pivot_y;          // sprite rotation pivot y
    SHORT Scale_x;          // horizontal scale, 0x1000 = 1.0
    SHORT Scale_y;          // vertical scale, 0x1000 = 1.0
    INT angle;              // rotation in degrees, 0 to 359
};

struct COM_TYPER_WORK
{
    UCHAR active;       // 1 while the auto-typer is running
    UCHAR state;        // typer step: 0=begin 1=press key 2=wait 3=finish
    UCHAR Dummy02[2];   // padding
    INT Moji_idx;       // index of the next character in the current line
    UCHAR Dummy08[8];   // padding
    SHORT timer;        // frames left before the next key press
    SHORT line;         // index of the text field being typed
};

typedef struct COM_SLIDE_REC COM_SLIDE_REC;

struct COM_SLIDE_REC
{
    USHORT x;   // left edge
    USHORT y;   // top edge
    USHORT w;   // width
    USHORT h;   // height
};

typedef VOID (*COM_FUNC)(VOID);   // handler taking no arguments

/*
    dispatch the lab computer screen state (init, update, finish), then draw caret, text, windows,
    keyboard, door picture, status and LEDs and apply any pending camera cut change
 */
VOID Sce_com_disp(VOID);

/*
    enter the lab computer screen: switch to the terminal camera cut, move the player out of view,
    swap the room lights and ambient for the lab lights, and reset windows, caret and text fields
 */
VOID Com_lab_init(VOID);

/*
    run the handler of the current stage, then advance the window animations and the auto-typer
 */
VOID Com_lab_main(VOID);

/*
    boot stage: step 0 runs the monitor turn-on effect, later steps run the logo animation
 */
VOID Com_stage_boot(VOID);

/*
    run the current monitor turn-on substep, then draw the monitor sprite
 */
VOID Com_boot_main(VOID);

/*
    start the monitor turn-on: set the first LED flag, the initial sprite scale and message flags
 */
VOID Com_boot_zoom_start(VOID);

/*
    widen the monitor sprite by 0x800 per frame until it reaches full width
 */
VOID Com_boot_zoom_wide(VOID);

/*
    grow the monitor sprite vertically by 0x800 per frame until it reaches full height,
    then start the flash Time_cnt
 */
VOID Com_boot_zoom_tall(VOID);

/*
    count down while flashing the monitor sprite and play a sound; at zero move on to the logo step,
    skipping the logo animation when confirm or cancel is held
 */
VOID Com_boot_flash(VOID);

/*
    logo step: run the logo animation until it finishes, hold, fade the screen out
    and advance to the login stage
 */
VOID Com_boot_logo(VOID);

/*
    login stage: dispatch the current step (init, input, check)
 */
VOID Com_stage_login(VOID);

/*
    open the login window and keyboard, place the first text field and request the terminal camera cut
 */
VOID Com_login_init(VOID);

/*
    dispatch the current substep of the login input step
 */
VOID Com_login_in_sub(VOID);

/*
    wait for the login window and keyboard to finish opening, then enable the first text field and caret
 */
VOID Com_login_open_wait(VOID);

/*
    read the on-screen keyboard: advance on enter, flag a cancel on cancel or the cancel button,
    otherwise append the typed key to the current text field
 */
VOID Com_login_key(VOID);

/*
    leave the screen when cancelled, otherwise enable the masked password field
 */
VOID Com_login_pass(VOID);

/*
    leave the screen when cancelled, otherwise close the login window and keyboard and request the camera cut
 */
VOID Com_login_close(VOID);

/*
    wait for the login window and keyboard to close, then move to the check step
 */
VOID Com_login_close_wait(VOID);

/*
    dispatch the current substep of the login check step
 */
VOID Com_login_ck_sub(VOID);

/*
    start the auto-typer on its first line
 */
VOID Com_login_type_start(VOID);

/*
    when the auto-typer is idle, start the next line, or advance once both lines are typed
 */
VOID Com_login_type_next(VOID);

/*
    compare the typed user name and password with the stored ones: on mismatch play a sound and
    show banner 4, otherwise accept
 */
VOID Com_login_ck(VOID);

/*
    count down the rejection banner, then restart the login once the arm objects are idle
 */
VOID Com_login_fail(VOID);

/*
    count down the acceptance banner (drawing the strip A message), then advance to the menu stage
 */
VOID Com_login_ok(VOID);

/*
    menu stage: step 0 sets the menu up, later steps handle input
 */
VOID Com_stage_menu(VOID);

/*
    set up the menu windows and open the panel, place the caret on the first entry and request the camera cut
 */
VOID Com_menu_set(VOID);

/*
    move the caret with the pad while input is enabled, then run the current menu substep
 */
VOID Com_menu_main(VOID);

/*
    open the side window after a delay, then enable caret input once the window animations have finished
 */
VOID Com_menu_open(VOID);

/*
    confirm selects door 0 or door 1 at the caret, or opens the exit confirmation; cancel opens
    the exit confirmation
 */
VOID Com_menu_sel(VOID);

/*
    open the confirmation window and enable the caret on it
 */
VOID Com_menu_cmd(VOID);

/*
    confirm on the first entry leaves the screen; any other confirm, or cancel, returns to the menu
 */
VOID Com_menu_exit(VOID);

/*
    start closing the menu windows and request the camera cut
 */
VOID Com_menu_close(VOID);

/*
    wait for the menu windows to close, then set the arm object flags for the selected door
 */
VOID Com_menu_close_wait(VOID);

/*
    repeat the arm animation until the counter runs out, then set the second LED flag and advance
 */
VOID Com_menu_arm_wait(VOID);

/*
    blink the status banner until the timer runs out, then go to the unlock stage for door 1,
    or start the Time_cnt
 */
VOID Com_menu_blink(VOID);

/*
    count three intervals while drawing the strip B message, then advance to the command stage
 */
VOID Com_menu_cnt(VOID);

/*
    command stage: dispatch the current step (setup, wait for input, handle the command)
 */
VOID Com_stage_cmd(VOID);

/*
    after the timer, open the command window and keyboard and prepare the masked text field
 */
VOID Com_cmd_set(VOID);

/*
    once the windows are open enable the caret and read the typed command; advance when submitted
 */
VOID Com_cmd_input_wait(VOID);

/*
    dispatch the current substep of the command step
 */
VOID Com_cmd_sub(VOID);

/*
    close the command window and keyboard
 */
VOID Com_cmd_close(VOID);

/*
    once the windows are closed start the auto-typer (or raise an arm flag when cancelled)
    and request the camera cut
 */
VOID Com_cmd_close_end(VOID);

/*
    wait for the auto-typer to finish; when cancelled return to the menu once the arm object is idle
 */
VOID Com_cmd_type_wait(VOID);

/*
    compare the typed command with the release command: on mismatch play a sound and show banner 2,
    otherwise show banner 3 and set the second LED flag
 */
VOID Com_cmd_ck(VOID);

/*
    count down the rejection banner, then return to the menu once the arm objects are idle
 */
VOID Com_cmd_fail(VOID);

/*
    count down the acceptance banner, then advance to the unlock stage
 */
VOID Com_cmd_ok(VOID);

/*
    unlock stage: run the current step and blink the door marker of the selected door
 */
VOID Com_stage_unlock(VOID);

/*
    mark the selected door unlocked, record the area as known and set its unlock flag
    (a flag that was already set skips the arm animation)
 */
VOID Com_cmd_unlock(VOID);

/*
    wait for the timer, then advance to the prompt
 */
VOID Com_cmd_unlock_hold(VOID);

/*
    show the prompt window until the counter runs out or confirm/cancel is pressed,
    then return to the menu
 */
VOID Com_cmd_unlock_msg(VOID);

/*
    finish state: substate 0 plays the shutdown sequence, later substates restore the room
 */
VOID Com_lab_end(VOID);

/*
    run the current shutdown step, update typer and windows, and draw the monitor sprite after step 0
 */
VOID Com_end_main(VOID);

/*
    shutdown: close the open windows, auto-type the shutdown text, fade the screen out
    and start the monitor shrink
 */
VOID Com_end_start(VOID);

/*
    shrink the monitor sprite vertically until nearly flat, then advance
 */
VOID Com_end_shrink_v(VOID);

/*
    shrink the monitor sprite horizontally until it vanishes, then advance and set the restore timer
 */
VOID Com_end_shrink_h(VOID);

/*
    after the timer restore room lights, ambient, camera cut and player height,
    clear the screen-active flag and leave the screen
 */
VOID Com_end_room_set(VOID);

/*
    advance the open/close animation of every enabled window
 */
VOID Com_wnd_main(VOID);

/*
    start a window animation: alpha starts at full when closing and at zero when opening
 */
VOID Com_wnd_anm_init(VOID);

/*
    step the window alpha toward its target and finish the animation when the limit is reached
 */
VOID Com_wnd_anm_main(VOID);

/*
    run the current auto-typer state while the typer is active
 */
VOID Com_typer_main(VOID);

/*
    start typing the current line from its first character
 */
VOID Com_typer_start(VOID);

/*
    select the arm object and press delay for the next character, or finish at the end of the text
 */
VOID Com_typer_key(VOID);

/*
    wait out the press delay, then step to the next character or finish at the end of the text
 */
VOID Com_typer_wait(VOID);

/*
    wait for the arm object to become idle, then deactivate the typer
 */
VOID Com_typer_end(VOID);

/*
    move the on-screen keyboard cursor with the pad and return the confirmed key: key code,
    -1 enter, -2 cancel, -3 delete, 0 when none
 */
SHORT Com_key_get(VOID);

/*
    type ch into the text buffer at the caret (delete when ch is -3) and move the caret
 */
VOID Com_text_add(SHORT ch, UCHAR* buf);

/*
    clear text field rec and move the caret to its position
 */
VOID Com_text_clr(INT rec);

/*
    set up window win: position, size in columns and rows, row descriptor table and animation state
 */
VOID Com_wnd_set(UCHAR win, SHORT x, SHORT y, SHORT cols, USHORT rows, UCHAR nRow, VOID* pRow);

/*
    draw the keyboard cursor and the keyboard frame quads while the keyboard window is open
 */
VOID Com_key_draw(VOID);

/*
    draw the frame quads and the text row sprites of the three panel windows
 */
VOID Com_wnd_draw(VOID);

/*
    draw n quad descriptors from rec at offset x, y on texture page tpage
 */
VOID Com_quad_draw(SHORT x, SHORT y, USHORT flags, UCHAR n, COM_QUAD_REC* rec, USHORT tpage);

/*
    draw the characters of every enabled text field; masked fields draw a star per character
 */
VOID Com_text_draw(VOID);

/*
    draw the blinking text caret, or the menu Sel_no bar
 */
VOID Com_cursor_draw(VOID);

/*
    draw the status banner and the message pictures
 */
VOID Com_sts_draw(VOID);

/*
    draw the picture of the selected door once it is unlocked
 */
VOID Com_door_pic_draw(VOID);

/*
    draw the LED sprites selected by the LED flags
 */
VOID Com_led_draw(VOID);

/*
    fade the screen tint down in steps of step and set the message flags when dark, then fade back up;
    returns 1 when the fade is complete
 */
INT Com_fade_main(SHORT step, SCHAR msg_flags);

/*
    apply the pending camera cut change once its delay has run out
 */
VOID Com_cut_set(VOID);

/*
    run the logo animation: on the first call clear the slots and spawn the hub, then step every
    active slot and draw them
 */
VOID Com_logo_main(VOID* pPanel);

/*
    run the current hub substep and rotate the hub by 15 degrees, wrapping at 360
 */
VOID Com_logo_hub_move(VOID);

/*
    set up the hub sprite
 */
VOID Com_logo_hub_init(VOID);

/*
    when the hub is at angle 0 count the timer down, then spawn a wedge and the wordmark slot
 */
VOID Com_logo_hub_spin(VOID);

/*
    at every quarter turn spawn a wedge slot (type A, type B on the last spawn) that copies the
    hub position and angle
 */
VOID Com_logo_hub_spawn(VOID);

/*
    when the hub is at angle 0 count the timer down, then signal phase 1 to the child slots
 */
VOID Com_logo_hub_hold(VOID);

/*
    slide the slot left while shrinking it, and advance once it reaches the limit
 */
VOID Com_logo_move_out(VOID);

/*
    at a quarter turn deactivate the hub slot and signal phase 2 to the child slots
 */
VOID Com_logo_hub_end(VOID);

/*
    dispatch the current substep of wedge A
 */
VOID Com_logo_wa_move(VOID);

/*
    set up the wedge A sprite
 */
VOID Com_logo_wa_init(VOID);

/*
    when the parent is at angle 270 spawn a wedge B slot at this slot's position and angle
 */
VOID Com_logo_wa_seed(VOID);

/*
    rotate by 15 degrees and, on the parent's phase 1 signal, advance and pass the signal on
 */
VOID Com_logo_wa_spin(VOID);

/*
    rotate by 15 degrees and slide the slot out
 */
VOID Com_logo_spin_out(VOID);

/*
    rotate until the parent signals phase 2, then deactivate the slot
 */
VOID Com_logo_retire(VOID);

/*
    dispatch the current substep of wedge B
 */
VOID Com_logo_wb_move(VOID);

/*
    set up the wedge B sprite
 */
VOID Com_logo_wb_init(VOID);

/*
    wait until the parent angle is 45 degrees ahead of this slot, then advance
 */
VOID Com_logo_wb_wait(VOID);

/*
    rotate by 15 degrees and advance on the parent's phase 1 signal
 */
VOID Com_logo_wb_spin(VOID);

/*
    dispatch the current substep of the wordmark
 */
VOID Com_logo_wm_move(VOID);

/*
    set up the wordmark sprite at zero width
 */
VOID Com_logo_wm_init(VOID);

/*
    reveal the wordmark by moving its edge left, then advance
 */
VOID Com_logo_wm_wipe(VOID);

/*
    wait for the parent's phase 2 signal, then show the full-width wordmark and set the hold timer
 */
VOID Com_logo_wm_wait(VOID);

/*
    hold the wordmark; when the timer is out and the game flag is clear, set up the shrink
 */
VOID Com_logo_wm_hold(VOID);

/*
    scale the wordmark down while moving it up; at the end deactivate the slot and set the
    logo state to finished
 */
VOID Com_logo_wm_shrink(VOID);

/*
    take the first free logo slot, clear its substep and return it; returns 0 when all nine are used
    Declared without a parameter list: callers pass 0, 1 or 2 arguments that the function ignores
 */
COM_LOGO_WORK* Com_logo_get();

/*
    dispatch the slide show state (start, update, finish) while update flag 0x80 is set
 */
VOID Sce_slide_disp(VOID);

/*
    start the slide show: switch to the slide camera cut, start the slide music tracks
    and set the first slide position
 */
VOID Com_slide_start(VOID);

/*
    step the slide show: slide the picture in, show its message, wait for the message to close,
    slide it out and move to the next slide; the last slide ends the show; then draw
 */
VOID Com_slide_main(VOID);

/*
    draw the frame bars and the current slide picture
 */
VOID Com_slide_draw(VOID);

/*
    end the slide show: restore the camera cut, clear the screen-active flag, stop the slide
    music tracks and resume track 0
 */
VOID Com_slide_exit(VOID);

/*
    store the two background image buffer addresses and the image mode
 */
VOID Bg_img_buf_set(VOID* buf_a, VOID* buf_b, INT mode);

/*
    load a 320x240 background image (buffer chosen by the image mode) into VRAM at row y
 */
VOID Bg_img_trans(SHORT y);

extern UCHAR Sce3_logo_state;                         // logo animation state: 0=not started, 1=running, 2=finished
extern PANEL_WORK* pPanel_work; // interactive screen state record in use
extern LIGHT_CUT_DATA Com_light_tbl[3];                       // lab terminal room lights
extern COM_QUAD_REC Com_frm_tbl[8];                              // window frame quads
extern COM_FUNC Com_slide_tbl[3];                          // slide show state handlers
extern COM_SLIDE_REC Com_slide_bar_tbl[4];                        // slide frame bars
extern COM_ROW_REC Com_login_row_tbl[3];                                // login window row descriptors
extern UCHAR Com_user_tbl[0x8];                              // expected login user name (key codes)
extern UCHAR Com_pass_tbl[0x4];                              // expected login password (key codes)
extern UCHAR Com_pa_row_tbl[0x30];                            // menu panel row descriptors
extern UCHAR Com_side_row_tbl[0x30];                              // menu side window row descriptors
extern UCHAR Com_menu_row_tbl[0x40];                              // confirmation window row descriptors
extern UCHAR Com_pb_row_tbl[0x40];                            // command window row descriptors
extern UCHAR Com_release_tbl[0x8];                         // expected release command (key codes)
extern UCHAR Com_unlock_row_tbl[0x10];                      // unlock prompt row descriptors
extern COM_QUAD_REC Com_unlock_quad_tbl[1];                      // unlock prompt quad
extern UCHAR Com_arm_tbl[0x20];                             // arm animation per key code
extern COM_QUAD_REC Com_key_frm[9];                         // keyboard frame quads
extern COM_QUAD_REC Com_msg_pic_tbl[3];                           // message picture quads
extern COM_QUAD_REC Com_sts_tbl[8];                         // status banner quads
extern COM_QUAD_REC Com_strip_a_tbl[2];                                // message strip A quads
extern COM_QUAD_REC Com_strip_b_tbl[3];                                // message strip B quads
extern UCHAR Com_door_ofs0[0x4];                            // door 0 map origin and quad count
extern COM_QUAD_REC Com_door_mark0[10];                           // door 0 map quads
extern UCHAR Com_door_ofs1[0x4];                            // door 1 map origin and quad count
extern COM_QUAD_REC Com_door_mark1[30];                          // door 1 map quads
extern COM_QUAD_REC Com_door_frm_tbl[4];                        // door picture frame quads
extern COM_QUAD_REC Com_door_pic_tbl[3];                        // door picture quads
extern COM_FUNC Com_lab_tbl[3];                            // screen state handlers (init, update, finish)
extern COM_FUNC Com_stage_tbl[5];                                    // stage handlers
extern COM_FUNC Com_boot_tbl[4];                              // monitor turn-on substeps
extern COM_FUNC Com_login_tbl[3];                            // login steps
extern COM_FUNC Com_login_in_tbl[6];                       // login input substeps
extern COM_FUNC Com_login_ck_tbl[5];                       // login check substeps
extern COM_FUNC Com_menu_tbl[9];                              // menu substeps
extern COM_FUNC Com_cmd_tbl[3];                          // command steps
extern COM_FUNC Com_cmd_sub_tbl[6];                           // command substeps
extern COM_FUNC Com_unlock_tbl[3];                            // unlock stage steps
extern COM_FUNC Com_end_tbl[3];                            // shutdown steps
extern COM_FUNC Com_typer_tbl[4];                       // auto-typer states
extern COM_FUNC Com_logo_tbl[4];                   // logo slot behaviours
extern COM_FUNC Com_logo_hub_tbl[6];                    // hub substeps
extern COM_FUNC Com_logo_wa_tbl[5];                // wedge A substeps
extern COM_FUNC Com_logo_wb_tbl[5];                // wedge B substeps
extern COM_FUNC Com_logo_wm_tbl[5];               // wordmark substeps

#endif      // SCE3_H
