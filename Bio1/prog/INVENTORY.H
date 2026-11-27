/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\INVENTORY.H
 */

#ifndef INVENTORY_H
#define INVENTORY_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

typedef VOID (*INV_FUNC)(VOID); // handler taking no arguments

/*
    set the room item present flags, the pickup quantities and the starting Item_work of the
    player character (8-slot or 6-slot set, plus the companion's first slot for the 6-slot
    character), assign the icon cell of each held item and clear the unused slots
 */
VOID Status_init(VOID);

/*
    remove the item slot at pSlot: shift every following held slot down by one and clear the
    last held slot
 */
VOID Item_lost(UCHAR* pSlot);

/*
    pack the Item_work: move held items to the front keeping their order, carry their icon
    cells and the equipped slot along, free the icon cells of the emptied slots and clear the
    trailing slots
    Declared without a parameter list: a caller passes arguments the function ignores
 */
VOID Sort_item();

/*
    count the leading non-empty Item_work slots (up to the slot limit of the character) into
    the held item count
 */
VOID Item_num_set(VOID);

/*
    read the item icon images from disc into img_buf, rebuild the icon cell mask and assign an
    icon cell to every held item, then upload their icons
 */
VOID Status_init_m(VOID* img_buf);

/*
    find the Item_work slot that holds item id: return its index and store the slot in
    pItem_found, or return -1 and point it at a fixed fallback record
 */
UINT Search_item_id(INT id);

/*
    pick up the room item of the current action entry: disable the entry and hide its model
    and its sparkle effect (the Esp slot kept in Push_cnt), clear its present flag, then add
    it to the Item_work: stackable items (ids 11-18 and 47) merge into an existing slot up to a
    count of 250, the rest take the next free slot; assign and upload its icon
 */
VOID St_get_item(VOID);

/*
    put back the item found in pItem_found: restore its count to the room entry,
    show its model again, set its present flag, remove it from the Item_work
 */
VOID Item_aot_ret(VOID);

/*
    item box menu update for one frame, returns 1 when the menu closes and 0 otherwise
    Key_rno 1: cursor over the Item_work slots and box, confirm enters Key_rno 2, cancel closes
    Key_rno 2: swap the selected Item_work slot with the shown box slot, up and down start a scroll
    Key_rno 3: scroll the box list one entry, with held repeat
    then draw the cursor and the name of the selected item
 */
INT St_main01(VOID);

/*
    load the item box graphics and the item icon images from disc, map the texture, upload the
    icon of the first box item and reset the box scroll state
    Declared without a parameter list: a caller passes an argument the function ignores
 */
VOID St_init01();

/*
    pack the 48 item box entries: move used entries to the front keeping their order and clear
    the rest
 */
VOID Ibox_sort(VOID);

/*
    draw the item box menu: frame sprites and backdrop boxes, the shown box slots with icon and
    count, the names of the visible entries, the line marking the end of the list, the position
    ticks at the right edge and the frame strips
 */
VOID St_disp_itembox(VOID);

extern UCHAR Item_pix_mask;            // bit mask of the item icon cells in use
extern ITEM_WORK* pItem_found;  // Item_work slot found by the last Search_item_id
extern UCHAR Item_pix_idx[8];         // icon cell assigned to each Item_work slot
extern UCHAR Ibox_spr_tbl[60];         // item box sprite records (12 bytes each: x, y, w, h, u, v), popped from the end
extern UCHAR Ibox_frm_tbl[36];          // item box frame strip sprite records (12 bytes each), popped from the end
extern UCHAR Ibox_frm_tbl2[40];         // item box backdrop box records (8 bytes each: x, y, w, h), popped from the end

#endif
