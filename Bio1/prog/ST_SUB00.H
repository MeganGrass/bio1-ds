/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ST_SUB00.H
 */

#ifndef ST_SUB00_H
#define ST_SUB00_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

typedef VOID (*ST_SUB00_FUNC)(VOID);    // menu handler

/*
    status menu task: set up the graphics, pick the screen mode from the game flags (status
    screen, prompt, item box, item viewer, map or file pickup), then run the per-frame state
    machine and draw the Item_work until the screen is closed, finally restoring the game state
    and exiting the task
 */
VOID Status(VOID);

/*
    start leaving the menu: enter the exit state, start the fade out, restore the saved VRAM
    area and clear the screen colour to black
 */
VOID St_fade_out_set(VOID);

/*
    reinitialise the graphics and 3D setup, the screen origin, the camera cut and the room
    lights after the menu, set the vsync mode and signal the waiting task
 */
VOID St_exit(VOID);

/*
    make the weapon of the player follow the item in the equipped slot, updating the arms
    sound set and flagging the model for reload on a change
 */
VOID St_equip_wep_set(VOID);

/*
    draw the Item_work screen: equipped item icon, slot icons with their counts, empty
    slots, character portrait, tab buttons, frame strips, slot frames and backdrop boxes
 */
VOID St_inv_disp(VOID);

/*
    pop the last 12-byte sprite record (x, y, w, h, u, v) before the table pointer into the
    scratch sprite and step the table pointer back by one record
 */
VOID St_spr_pop(VOID);

/*
    draw the count of an item as up to three digits right of the sprite position, or a fixed
    mark instead of the digits when the flag of that weapon type is set; item types that carry
    no count draw nothing
 */
VOID Print_num(UCHAR type, UCHAR num, UCHAR pri);

/*
    load and map the model file and TIM of the selected item when it differs from the loaded one
    Declared without a parameter list: a caller passes an argument the function ignores
 */
VOID Item_model_load();

extern GsRVIEW2 View_ref;            // reference view of the 3D scene
extern UCHAR St_tab_tbl[0x30];        // sprite records (x, y, w, h, u, v) of the four menu tab buttons
extern UCHAR St_strip_tbl[0x38];     // four frame strip entries: sprite record plus flip and repeat word
extern UCHAR St_slot_tbl6[0x80];    // nine slot frame tile entries for a six slot Item_work
extern UCHAR St_slot_tbl8[0x80];    // nine slot frame tile entries for an eight slot Item_work
extern UCHAR St_box_tbl[0x20];      // four backdrop box fill rectangles (x, y, w, h)
extern USHORT St_slot_pos_tbl[28];            // screen position (x then y) of each menu cursor cell, then four trailing values

#endif      // ST_SUB00_H
