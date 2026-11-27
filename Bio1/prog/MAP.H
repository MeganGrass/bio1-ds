/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\MAP.H
 */

#ifndef MAP_H
#define MAP_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

struct MAP_SCR_WORK
{
    UCHAR state;        // screen state step
    UCHAR substate;     // 0 load the map, 1 run the browser
    UCHAR Blink_mode;   // blink phase of the current room: 0 start, 1 brighten, 2 darken
    UCHAR Zoom_mode;    // map zoom step, 0 idle, 7 and above closing
    SCHAR layout;       // selected map layout, 0-3
    SCHAR area_group;   // stage group of the player's area
    SCHAR area;         // map area shown
    SCHAR room;         // room id of the player's room
    UCHAR Room_idx[4];  // selected area index in each layout's area list
    UCHAR Area_flg;     // bit 0 next area available, bit 1 previous area available
    UCHAR Layout_mask;  // layouts that can be shown, one bit per layout
    UCHAR Arrow_anm;    // scroll counter of the arrow animation
    UCHAR mode;         // 0 browsing, nonzero while an objective is highlighted, bit 0 locks navigation
    UCHAR Hilite_mode;  // objective highlight phase, 0-5
    UCHAR Hilite_timer; // frame counter of the navigation arrow blink
    UCHAR Time_cnt;     // frame Time_cnt between steps of the open and close sequence
    UCHAR Dummy13;      // unused; only cleared
    UCHAR Exit_flg;     // 1 once the closing sequence is finished
    UCHAR Hilite_frm;   // animation frame of the objective highlight
    USHORT Area_mask;   // one bit per area known to the player
};

typedef VOID (*MAP_FUNC)(VOID);

/*
    map screen update for one frame, returns -1 when the screen is left and 0 otherwise
    state 0: tab Sel_no, confirm opens the floor map, cancel leaves
    state 1: floor map display, cancel starts the zoom out and returns to state 0 once it
    ends, confirm zooms in again while it is zooming out
    cancel while the zoom out runs returns to state 0 at once
 */
INT St_map_main(VOID);

/*
    tab update by substate: load the tab image first, then handle the tab navigation
 */
VOID Map_tab_main(MAP_SCR_WORK* pMap_scr);

/*
    select the highest unlocked layout, load and upload its tab image when it is not the one
    already in memory, then refresh the layout state
 */
VOID Map_tab_load(MAP_SCR_WORK* pMap_scr);

/*
    tab navigation: up and down step the floor within the layout, left and right switch to
    another unlocked layout, all ignored while mode bit 0 is set; then refresh the layout state
 */
VOID Map_tab_move(MAP_SCR_WORK* pMap_scr);

/*
    set the current area from the layout and floor, work out which neighbouring floors can be
    stepped to (Area_flg), tint the browser sprites (selected layout brighter) and draw them
 */
VOID Map_layout_set(MAP_SCR_WORK* pMap_scr);

/*
    queue the map browser sprites: background, unlocked layout tabs, layout and floor markers,
    and the up and down arrows (blinking) for the floors that can be stepped to
 */
VOID Map_tab_draw(MAP_SCR_WORK* pMap_scr);

/*
    floor map display update by substate: load the floor textures first, then animate; dim the
    browser sprites and draw them
 */
VOID Map_disp_main(MAP_SCR_WORK* pMap_scr);

/*
    load the floor map image of the current area when it changed, clear the colours of the rooms
    not yet visited (only part of them once the map of the area was found), upload the image,
    set the objective highlight state, then animate
 */
VOID Map_tex_load(MAP_SCR_WORK* pMap_scr);

/*
    blink the colour of the current room (when the shown area is in the current stage group), run
    the zoom animation of the map sprites: Zoom_mode 0..4 open (widen, stretch, settle), 5..6 hold
    and wait for the highlight, 7..9 close (shrink, then return to the tab state, or to state 6 of
    the scripted sequence when mode bit 0 is set); then draw the map
 */
VOID Map_anm_main(MAP_SCR_WORK* pMap_scr);

/*
    queue the map sprites; while Zoom_mode is 6 advance the one-pixel-row strips (Arrow_anm),
    and while the objective highlight is active animate its marker sprite
 */
VOID Map_disp_draw(MAP_SCR_WORK* pMap_scr);

/*
    initialise the map screen for the current room: floor indices, current room and area
    group, the starting area chosen from the stage, room and cut, the area and layout masks,
    the objective highlight and the starting layout
 */
VOID Map_scr_init(VOID);

/*
    build the mask of visible areas from the visited room flags and the known area maps, and the
    mask of layouts that contain a visible area
 */
VOID Map_mask_set(MAP_SCR_WORK* pMap_scr);

/*
    set the visited flag of a room of the stage group (stage % 5)
 */
VOID St_room_set(UINT stage, INT room);

/*
    set the found flag of map Map_no (0..5, the map index that Map_area_ck derives from an area)
 */
VOID Map_area_set(INT Map_no);

/*
    return whether the map covering area a was found (areas 5 and 6, and 8 and 9, share a map);
    areas 2, 7 and 10 have no map and return 0
 */
INT Map_area_ck();

/*
    set the flag that a file was collected
 */
VOID St_file_set(INT file_id);

/*
    return whether a file was collected; 0xFE and 0xFF are never collected
 */
INT File_ck(UCHAR file_id);

/*
    upload the pixel data of the TIM at pTim_Adr to rect and its palette, when present, to
    the row y + 0x1E0
 */
VOID Tim_trans(UCHAR* pTim_Adr, RECT* rect, INT y);

/*
    scripted map screen sequence, returns -1 once finished
    states 0..2: short waits and the tab load, 3: open the floor map and wait for the highlight
    state 4: wait for the cancel or start input, 5: close, 6..8: leave
 */
INT St_main03(VOID);

/*
    initialise the scripted map display: objective highlight, then area 0 when mode is 1 or
    area 6 otherwise, cleared counters and the area mask
 */
VOID St_init03(VOID);

/*
    choose the objective highlight mode (0 = none) from the scenario flags
 */
VOID Map_hilite_set(VOID);

extern MAP_SCR_WORK Map_scr_work; // map screen state
extern RECT Map_upload_rect; // VRAM destination rectangle of the map image and blink colour uploads
extern USHORT Map_tab_file_tbl[]; // disc file of the tab image of each layout
extern USHORT Map_floor_file_tbl[]; // disc file of the floor map image of each area
extern UCHAR Map_flg_base_tbl[]; // first room visited flag of each stage group
extern UCHAR Map_room_cnt_tbl[]; // number of rooms of each stage group
extern SCHAR Map_floor_tbl[4][4]; // areas of each layout, one per floor (-1 = none)
extern UCHAR Map_area_tbl[]; // layout of each area
extern UCHAR Map_stage_tbl[]; // stage group of each area
extern UCHAR Map_floor_cnt_tbl[]; // number of floors of each layout
extern UCHAR Map_label_tbl[]; // first floor label row of each layout
extern GsSPRITE Map_tab_spr[9]; // browser sprites: background, 4 layout tabs, layout marker, floor marker, up and down arrows
extern GsSPRITE Map_spr[]; // map sprites: 0-2 map image strips, 3-5 one-pixel-row strips animated while Zoom_mode is 6, 6 objective marker
extern UCHAR Map_spr_tail[4]; // padding: keeps the following data at its linked position

#endif
