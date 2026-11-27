/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ST_SUB02.H
 */

#ifndef ST_SUB02_H
#define ST_SUB02_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

/*
    item submenu state machine: open the panel, pick one of the three rows (use or equip,
    examine, combine), run the chosen action handler, then close the panel; draws the panel
 */
VOID St_menu0_item(VOID);

/*
    map tab: run the map screen until it ends, then return to browsing
 */
VOID St_menu0_map(VOID);

/*
    file tab: run the file screen until it ends, then return to browsing
 */
VOID St_menu0_file(VOID);

/*
    radio tab: when flag 0x3F is set, mark item 0x4D as the used item, request the menu to close
    and play a sound; otherwise run the item viewer until it ends
 */
VOID St_menu0_radio(VOID);

/*
    exit tab: return to browsing and start leaving the menu
 */
VOID St_menu0_exit(VOID);

/*
    toggle the equipped slot: equip the slot under the cursor, or clear the equipment when that
    slot is already equipped
 */
VOID St_item_equip_chg(VOID);

/*
    use the selected item: find its use type, run the handler of that type, and on success play
    a sound and consume one of the item; otherwise show the message of that use type
 */
VOID St_item_use(VOID);

/*
    use type handler of group 0: the item cannot be used, returns 0
 */
INT St_item_use_none(VOID);

/*
    use type handler of groups 1 and 2: the item cannot be used, returns 0
 */
INT St_item_use_none1(VOID);

/*
    use type handler of group 3: when the flag (item id - 0x1B) is set, mark the item as used by
    the menu and return 1, otherwise return 0
 */
INT St_item_use_flg(VOID);

/*
    use type handler of groups 4 and 5: when the flag (item id - 0x1B) is set, mark the item as
    used by the menu and return 1, otherwise return 0
 */
INT St_item_use_flg4(VOID);

/*
    use type handler of group 6, for item 0x3E: when flag 0x23 is set, mark the item as used by the menu
    and return 1, otherwise return 0
 */
INT St_item_use_book(VOID);

/*
    use type handler of group 7, for healing items: restore health by the low nibble of the heal value of
    the item (1: a third of the maximum, 2: two thirds, 3: full) and clear a status condition
    by the high nibble (1 or 2); returns 1 when something was applied
 */
UCHAR St_item_use_heal(VOID);

/*
    use type handler of group 8: the item can always be used, returns 1
 */
INT St_item_use_always(VOID);

/*
    use type handler of group 9: the item cannot be used, returns 0
 */
INT St_item_use_none9(VOID);

/*
    examine action of the item submenu: load the item model, open the view, run the item
    viewer until it ends, then close the view and draw the item name
 */
VOID St_com_inspect(VOID);

extern UINT Use_flg[2];    // item-use flag words
extern VOID (*St_item_act_tbl[3])(VOID);        // submenu row actions: use, examine, combine
extern VOID* St_item_use_tbl[9];           // use type handlers, indexed by use type group

#endif      // ST_SUB02_H
