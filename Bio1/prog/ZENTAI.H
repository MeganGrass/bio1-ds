/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ZENTAI.H
 */

#ifndef ZENTAI_H
#define ZENTAI_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>

typedef VOID (*ZENTAI_FUNC)(VOID);

/*
    clear the first seven words of the game state block (game and status flags, system flags,
    display buffer, clear colour and the following state bytes) and the menu choice
 */
VOID Init_global(VOID);

/*
    set the vsync wait count to 2, mark no sound fade running and set the screen fade state and
    the room light fade box state to -1 (no fade, box off)
 */
VOID Init_snd_fade(VOID);

/*
    zero the work words from the default item fields of the game state block through the effect,
    player and enemy work, up to the save data block
 */
VOID Game_work_clr(VOID);

/*
    clear the per-frame script work: the picked up and used item ids, the position action ids
    and the second system flag word
 */
VOID Sce_work_clr(VOID);

/*
    zero the words from p up to end (always at least one word)
 */
VOID Memclr_dw(INT* p, INT* end);

extern VOID* pFixed_work;           // pointer to the fixed work area

#endif
