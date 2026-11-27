/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ST_SUB03.H
 */

#ifndef ST_SUB03_H
#define ST_SUB03_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

typedef VOID (*ST_SUB03_FUNC)(VOID);    // menu handler

/*
    item combine state of the status menu: pick the partner item, check the mix, then animate
    and apply it or show the result message
 */
VOID St_com_mix(VOID);

/*
    look up the combine entry joining the item at the cursor and the item at the partner
    cursor; apply its result items and effect, returning 4 when mixed, 0 when no entry matches
    or a count check fails, 1 when refused outside its room, 2 when the matching entry waits
    for a confirmation (item ids 0x43-0x4B) and 3 when two such items have no matching entry
 */
INT Check_item_mix(VOID);

/*
    confirm or cancel a pending combine; on confirm swap in the result items of the matching
    combine entry and refresh the slots
 */
VOID St_mix_set(VOID);

/*
    combine effect: move the count of the source slot into the destination slot up to the item
    limit, leaving the excess in the source and emptying it when everything fits
 */
VOID St_mix01(INT Dst_slot, INT Src_slot);

/*
    same as St_mix01 with the slot arguments in the opposite order
 */
VOID St_mix02(INT Src_slot, INT Dst_slot);

/*
    combine effect: add the count of the source slot to the destination slot up to 250,
    leaving the excess in the source and emptying it when everything fits
 */
VOID St_mix03(INT Dst_slot, INT Src_slot);

/*
    combine effect: set the result of the menu action to 2 and set bit 0x16 of the flag table
 */
VOID St_mix04(VOID);

/*
    combine effect: set the result of the menu action to 3
 */
VOID St_mix05(VOID);

/*
    combine effect: replace the destination count by the count of the source slot capped at the
    item limit, leaving the excess in the source and emptying it when everything fits
 */
VOID St_mix06(INT Dst_slot, INT Src_slot);

/*
    same as St_mix06 with the slot arguments in the opposite order
 */
VOID St_mix07(INT Src_slot, INT Dst_slot);

/*
    after a combine, reload the icon file when needed and upload the icon of each of the two
    slots whose item changed
 */
VOID St_mix_pix(UCHAR Slot_a, UCHAR Slot_b, UCHAR Old_a, UCHAR Old_b);

/*
    read the door texture file chosen by the door texture index, set the VRAM page state
    and map its TIM
 */
VOID Door_texture_load(VOID);

/*
    copy Size bytes from pSrc to pDst, one byte at a time
 */
VOID Memcpy(VOID* pDst, VOID* pSrc, INT Size);

/*
    clear a VRAM rectangle to black
 */
VOID Vram_clr(SHORT x, SHORT y, SHORT w, SHORT h);

extern ITEM_WORK* pItem_work_base;    // item slots of the active character
extern VOID* St_mix_tbl[8];               // combine effect handlers, indexed by the effect number of a combine entry
extern SHORT Dtex_file[34];                    // door texture file codes, indexed by door texture index

#endif      // ST_SUB03_H
