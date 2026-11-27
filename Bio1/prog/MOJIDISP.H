/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\MOJIDISP.H
 */

#ifndef MOJIDISP_H
#define MOJIDISP_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

typedef VOID (*MESS_FUNC)(VOID);

/*
    pick up the item of the current room action entry: item 0x4D only sets scenario flag 0x7F
    (radio obtained), any other item goes through the common pickup routine
 */
VOID Mess_item_get(VOID);

/*
    record the selected item as the item used on a room action and consume it: the lock pick
    (0x0D) is never consumed, an item below 11 has its slot emptied (clearing the equipped slot
    when it is that one), any other item loses one from its stack; the Item_work is then
    re-sorted, except that a used-up stack of keys 0x33..0x3C keeps its slot and sets game flag
    0x2000 instead
 */
VOID Mess_item_use(VOID);

/*
    remove the selected item from the Item_work and re-sort it
 */
VOID Mess_item_clr(VOID);

/*
    empty message branch action
 */
VOID Mess_act_nop(VOID);

/*
    draw string table entry idx: each group is a character count, a CLUT index, a position in
    half-pixels and the glyph codes, drawn as 8x14 sprites at priority 2
 */
VOID Moji_tbl_print(USHORT idx);

/*
    draw the name of Item_id at Pos_x, Pos_y (nothing for item 0); the high nibble of Attr is the
    ordering-table priority (0 = 2), the low nibble the CLUT index
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
VOID Name_disp(INT Item_id, INT Attr, INT Pos_x, INT Pos_y);

/*
    return the name string of item Item_id: items 1..0x4D that have an alternate name use it
    until their examined flag is set
 */
UCHAR* name_ptr_set(INT Item_id);

/*
    draw an encoded message string at Pos_x, Pos_y up to its end code (1 or 7); Attr as in
    Name_disp
 */
VOID Mess_print(INT Pos_x, INT Pos_y, INT Attr, UCHAR* pMess);

/*
    draw the scratchpad text buffer with the 8x8 ASCII font; c holds priority (high nibble)
    and CLUT index (low nibble), p != 0 draws semi-transparent and adds 8 to the CLUT index
 */
VOID Print8(INT x, INT y, INT c, INT p);

/*
    draw the scratchpad text buffer with the 8x14 message font, one glyph per character code;
    c and p as in Print8
 */
VOID Print14(INT x, INT y, INT c, UCHAR p);

/*
    start the message msg (bit 7: fast-forward with the confirm button, bit 6: global table
    instead of the room's message list, low 6 bits: index) and clear the update flags in msg_id
    until it closes

    returns 1 if a message is already active, otherwise 0
 */

/*
    start the description message of item item_idx with fast-forward; msg_id as in Mess_set

    returns 1 if a message is already active, otherwise 0
 */
INT Item_mess_set(USHORT item_idx, SHORT msg_id);

/*
    per-frame message state machine: 0 = start, 1 = reveal characters and run control codes,
    2 = page break waiting for a button, 3 = page break with timer, 4 = yes/no choice,
    5 = end waiting for a button, 6 = end with timer; then draw the revealed text
 */
VOID Mess_disp(VOID);

/*
    leave a yes/no choice: pick the branch record of the selected answer, then continue the
    text (action 8), start another message (9) or call the Mess_func_tbl entry (10)
    named by the record
 */
VOID Branch_disp(VOID);

/*
    draw the message text revealed so far, from Mess_top up to Mess_end, handling
    colour changes, line breaks and inserted item names
 */
VOID mess_trans(VOID);

/*
    empty function
 */
VOID Mess_nop(VOID);

extern MESS_FUNC Mess_func_tbl[4];                // message branch actions: take item, use item, remove item, none
extern UCHAR Mess_data[0x83C];        // encoded text of all global pMessage
extern UCHAR* Mess_tbl[];                   // start of each global message in the text pool, by message number
extern UINT Moji_null;                 // zero word pointed to by the string table entry
extern UCHAR* Moji_tbl[];                  // string table for Moji_tbl_print

#endif
