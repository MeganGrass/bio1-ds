/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\FILE.H
 */

#ifndef FILE_H
#define FILE_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

struct PICKUP_WORK
{
    UCHAR mode;         // screen mode
    UCHAR state;        // screen state
    UCHAR step;         // screen to open next
    UCHAR Open_req;     // open request, 1 or 2
    SCHAR List_page;    // page of the file list
    SCHAR selected;     // selected file
    SCHAR file_page;    // page within the selected file
    SCHAR Book_no;      // set of files shown
    SCHAR Close_req;    // close request
    UCHAR closing;      // 1 while the screen slides closed
    SHORT Slide_spd;    // slide speed
    SHORT Slide_x;      // slide position x
    SHORT Slide_y;      // slide position y
    SCHAR Exit_sel;     // exit entry selected
    SCHAR Arrow_blink;  // arrow blink state
    SCHAR Blink_timer;  // arrow blink timer
    SCHAR Hold_timer;   // button hold timer
};

struct FILE_WORK
{
    UCHAR mode;         // 0 file list, 1 file reader
    UCHAR state;        // 0 initialise, 1 running
    UCHAR step;         // slide animation step: 0 slide in, 1 idle, 2 slide out, 3-5 page turn
    UCHAR Open_req;     // fade request: 1 fade into the reader, 2 fade back to the list
    SCHAR List_page;    // page of the file list
    UCHAR selected;     // selected entry on the page, 0xFF if none
    SCHAR file_page;    // page of the open file in the reader
    SCHAR Book_no;      // file set shown, chosen by a game flag
    UCHAR Close_req;    // set once the screen has slid out, ends the screen
    UCHAR busy;         // nonzero while a transition blocks input
    SHORT Slide_spd;    // slide speed, doubles or halves every frame
    SHORT Slide_x;      // horizontal slide offset of the page turn
    SHORT Slide_y;      // vertical slide offset of the screen
    SCHAR Exit_sel;     // 1 when the exit button is selected in the reader
    SCHAR Arrow_blink;  // 1 while the page arrows are drawn lit
    UCHAR Blink_timer;  // frame counter, bits 4-5 drive the arrow blink
    SCHAR Hold_timer;   // frames the left or right button has been held
};

typedef struct SPR_REC SPR_REC;
typedef struct SPR_REC0 SPR_REC0;
typedef struct SPR_REC1 SPR_REC1;
typedef struct SPR_REC2 SPR_REC2;
typedef struct FILE_TEX_REC FILE_TEX_REC;
typedef struct FILE_PAGE_REC FILE_PAGE_REC;

struct SPR_REC
{
    SHORT x;            // screen x
    SHORT y;            // screen y
    USHORT w;           // width
    USHORT h;           // height
    USHORT tpage;       // texture page
    SCHAR u;            // texture u (the sprite attribute word precedes this record)
    UCHAR v;            // texture v
    SHORT cx;           // CLUT x
    SHORT cy;           // CLUT y
    UCHAR pad10[16];    // r, g, b and the unused hot-spot, scale and rotation fields
    GsSPRITE Arrow_l;   // left page arrow sprite
    GsSPRITE Arrow_r;   // right page arrow sprite
    GsSPRITE Ex_spr[6]; // further sprites that follow in memory
};

struct SPR_REC0
{
    SHORT x, y;             // screen position
    USHORT w, h, tpage;     // size and texture page
    SCHAR u;                // texture u (the sprite attribute word precedes this record)
    UCHAR v;                // texture v
    SHORT cx, cy;           // CLUT position
    UCHAR pad10[16];        // r, g, b and the unused hot-spot, scale and rotation fields
};

struct SPR_REC1
{
    SHORT x, y;             // screen position
    USHORT w, h, tpage;     // size and texture page
    SCHAR u;                // texture u (the sprite attribute word precedes this record)
    UCHAR v;                // texture v
    SHORT cx, cy;           // CLUT position
    UCHAR pad10[20];        // r, g, b, the unused hot-spot, scale and rotation fields and the attribute word of the following sprite
};

struct SPR_REC2
{
    SHORT x, y;         // screen position
    USHORT w, h, tpage; // size and texture page
    SCHAR u;            // texture u (the sprite attribute word precedes this record)
    UCHAR v;            // texture v
    SHORT cx, cy;       // CLUT position
    UCHAR pad10[16];    // r, g, b and the unused hot-spot, scale and rotation fields
    GsSPRITE Arrow_l;   // left page arrow sprite
    GsSPRITE Arrow_r;   // right page arrow sprite
};

struct FILE_TEX_REC
{
    RECT rect;              // VRAM rectangle that receives the image
    INT Clut_row;           // CLUT row of the image
};

struct FILE_PAGE_REC
{
    INT Sector_ofs;         // sector offset of the page image inside the page archive file
    INT size;               // byte size of the page image
};

typedef VOID (*FILE_FUNC)(VOID);

/*
    per-frame handler of the file screen of the status menu: in mode 0 the file list closes on
    the cancel button and opens the reader on the action button for a selected entry, in mode 1
    the reader closes on the cancel button or the action button on its exit icon; returns -1 once
    the screen has asked to close, 0 otherwise
 */
INT St_file_main(VOID);

/*
    state machine of the file list: state 0 initialises it, state 1 handles input and drawing
 */
VOID File_book_main(FILE_WORK* pWk);

/*
    initialise the file list: slide-in start position, file set (set 1 when game flag 0x800000
    is set), first collected entry as Sel_no, black fade box, then load and draw the list
 */
VOID File_book_init(FILE_WORK* pWk);

/*
    file list input: up and down move the Sel_no to the previous or next collected entry
    (auto-repeat after 20 frames, every third frame), left and right change the list page; then
    draw the list
 */
VOID File_book_pad(FILE_WORK* p);

/*
    draw the file list and run its animation steps: slide-in and slide-out, list page changes,
    the fade box on opening the reader, the blinking page arrows, the entry markers, the cursor,
    the name of the selected file and the up and down arrows
 */
VOID File_book_disp(FILE_WORK* p);

/*
    state machine of the reader: keeps the fade box fully bright, state 0 initialises the
    reader, state 1 handles input and drawing
 */
VOID File_reader_main(FILE_WORK* pWk);

/*
    start the reader on the first page with the exit icon deselected and draw it
 */
VOID File_reader_init(FILE_WORK* pWk);

/*
    reader input: left and right turn the page (auto-repeat after 20 frames); moving right on the
    last page selects the exit icon and moving left deselects it; then draw the reader
 */
VOID File_reader_pad(FILE_WORK* pWk);

/*
    draw the reader and run its animation steps: load the frame and page images, slide pages in
    and out when turning, then draw the page sprite, the blinking arrows and, on the last page,
    the exit icon
 */
VOID File_reader_disp(FILE_WORK* pWk);

/*
    load file file_id into the load buffer and upload it to VRAM for texture slot Tex_slot,
    unless the resource Res_id is already loaded; slot 2 uploads two palette rows and the image
    rectangle directly
 */
VOID File_tex_load(INT file_id, INT Res_id, INT Tex_slot);

/*
    load the page image page of the page archive file into the load buffer by pointing the
    archive entry of the CD file table at the page's sector, size and checksum, then upload it as
    File_tex_load does; the entry is restored afterwards
 */
VOID file_moji_load(INT page, INT Res_id, INT Tex_slot);

/*
    step the fade box and queue it: mode 0 brightens it by 0x20 (returns -1 at full Kido),
    mode 1 darkens it by 0x20 (returns -1 at black), mode 2 sets white, mode 3 sets black;
    returns 0 otherwise
 */
INT Pickup_fade_main(UINT mode);

/*
    build the file table, reset the screen state and mark the screen as closing
 */
VOID File_get_init(VOID);

/*
    run the screen shown when a file is picked up: fade in, flag the file as collected, play the
    sound and open the reader at its page; on closing fade out and show the pickup message; once
    done, clear the picked item (model, room action entry, picked flag) and return -1; returns 0
    while running
 */
INT File_pickup_main(VOID);

/*
    rebuild the file table and clear the file screen state
 */
VOID St_reset(VOID);

/*
    replace the placeholder entry (0xFE) of the file table row of the current set with file 0
    or file 1, whichever has been collected
 */
VOID Pickup_seen_set(FILE_WORK* pWk);

/*
    fill both rows of the file table with the file ids in list order (0xFE marks the
    placeholder slot, 0xFF an empty slot)
 */
VOID Pickup_tbl_init(VOID);

extern PICKUP_WORK File_pickup_work;  // state of the file screen (list, reader and pickup display)
extern FILE_PAGE_REC File_page_tbl[46];             // page images: sector offset in the page archive file and byte size, by page entry
extern UCHAR File_page_sum[48];                   // checksum of each page image, by page entry
extern USHORT Book_tex_tbl[2];                    // file numbers of the list textures, by list page
extern USHORT File_page_idx[46];                      // page archive entry of each page image (one image holds two reader pages), by image index
extern USHORT Reader_frame_no;                    // file number of the reader frame texture
extern USHORT Reader_pad;                 // padding: keeps the following data at its linked position
extern UCHAR File_page_num[16];                       // page count of each file, by file id
extern UCHAR File_name_x[16];                           // x position of each file's name in the list, by file id
extern FILE_TEX_REC File_tex_tbl[2];                   // VRAM rectangle and CLUT row of texture slot 0 (list) and slot 1 (reader frame)
extern RECT File_clut_rect;                             // VRAM rectangle that receives the page image of texture slot 2
extern USHORT File_clut_row;                            // CLUT row used for the page image palettes
extern USHORT File_pad;                        // padding: keeps the following data at its linked position
extern UINT Book_list_attr;                          // attribute word of the list background sprite, read as the start of its record
extern SPR_REC0 Book_list_spr;                 // list background sprite (attribute word is the preceding Book_list_attr)
extern UCHAR Book_entry_spr[0x148];             // nine entry marker sprite records of the list (the first eight are drawn), then the attribute word of the cursor sprite
extern SPR_REC2 Book_cursor_spr;               // list Sel_no cursor sprite with its left and right page arrow sprites (attribute word ends Book_entry_spr)
extern UCHAR Book_up_attr[0x4];                    // attribute word of the up arrow sprite
extern SPR_REC0 Book_up_spr;                    // up arrow sprite of the list
extern UCHAR Book_down_attr[0x4];                  // attribute word of the down arrow sprite
extern SPR_REC1 Book_down_spr;                  // down arrow sprite of the list
extern SPR_REC0 Reader_page_spr;               // page sprite of the reader
extern GsSPRITE Reader_head_spr[6];           // header sprites of the reader, not drawn by any code
extern GsSPRITE Reader_arrow[2];                  // left and right page arrow sprites of the reader
extern GsSPRITE Reader_exit_spr;                   // exit icon sprite of the reader
extern UCHAR File_page_top[16];                       // index of the first page image of each file, by file id
extern UCHAR Book_tbl[2][16];                    // file ids shown by the list, by file set and slot (0xFE=placeholder, 0xFF=empty)
extern GsBOXF Pickup_fade_box;                          // full-screen box used to fade the file screens

#endif
