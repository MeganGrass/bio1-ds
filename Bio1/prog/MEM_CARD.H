/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\MEM_CARD.H
 */

#ifndef MEM_CARD_H
#define MEM_CARD_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <KERNEL.H>

struct CFILE_INFO
{
    CHAR title[15][22]; // title of each file found
    UCHAR num;          // number of files found
    UINT total_size;    // total size of the files
    UINT size[15];      // size of each file
};

struct SAVE_POS_REC
{
    UCHAR stage; // stage id
    UCHAR room;  // room id
};

typedef struct MC_DEV_BUF MC_DEV_BUF;
typedef struct MC_FNAME_BUF MC_FNAME_BUF;

struct MC_DEV_BUF
{
    CHAR s[6];      // device name text such as "bu00:" with terminator
};

struct MC_FNAME_BUF
{
    CHAR s[14];     // save file name text such as "BASLUS-00551" followed by the slot digit and terminator
};

/*
    queue the blinking menu cursor sprite at (X, Y) into the current ordering table;
    nothing is drawn while hidden is non-zero
 */
VOID Cursor_disp(INT X, INT Y, INT hidden);

/*
    run the memory card save/load screen: fade in, then step the slot select, card check,
    file list and write/read states every frame until a save or load completes or the
    player backs out, then fade out; Load_flg 0=save 1=load, Save_buf holds the five save
    records from +0xA00, Ribbon_flg consumes an ink ribbon on save, Se_bank is the sound
    bank of the menu sounds (2 also redraws the stage background on exit);
    returns 0 when a save or load completed, 1 when cancelled
 */
INT Card_access(INT Load_flg, UCHAR* Save_buf, INT Ribbon_flg, UCHAR Se_bank);

/*
    enter the card screen: set the vsync mode, save and clear status flag bits 1-2 (set
    bit 21 for sound bank 2), read disc file 0x1E, fade in and wait for the fade
 */
VOID Cardaccess_init(INT Se_bank);

/*
    request card info for slot 0/1; returns non-zero when the last request timed out,
    meaning the card was removed
 */
INT Card_poll(INT slot);

/*
    leave the card screen: fade out, restore the vsync mode, redraw the stage background
    when Mode is 2 or clear VRAM otherwise, and restore the saved status flag bits
 */
VOID Cardaccess_exit(INT Mode);

/*
    map each of the five save slots to its file in File_info by the slot digit in the file
    name; F_num[slot] is the file index, or 5 for an empty slot
 */
VOID Search_f_num(UCHAR* F_num);

/*
    draw the card slot Sel_no texts: save or load header, the two memory card names
    and the do-not text
 */
VOID Card_menu_disp(INT Load_flg);

/*
    draw the five save slot rows (slot number, character name, save count, area name,
    or NO DATA), the save/load header and the card name; character Sel_char of row Sel_file
    is replaced by the highlight code; returns 1 when that character was not blank
 */
INT Card_file_disp(INT Load_flg, UCHAR* F_num, UCHAR* Save_buf, UCHAR Sel_file, UCHAR Sel_char);

/*
    draw the card name and the card formatting prompt with its two choices
 */
VOID Card_prompt_disp(VOID);

/*
    copy the player id, position, angle and collision flags from the player work into the
    save data
 */
VOID Save_push(VOID);

/*
    restore the game flags and status flags implied by a loaded save: the mode byte of the
    save sets the matching status flag bits, the player id is copied back to the player work
 */
VOID Load_pop(VOID);

/*
    copy the saved record into slot file of the save buffer, then play the save-complete
    animation that sweeps the highlight over the characters of that slot row
 */
VOID Card_save_anim(UCHAR file, INT Load_flg, UCHAR* Save_buf, UCHAR* F_num, UCHAR Se_bank);

/*
    build the save file title text in Mess for slot Cursor: character name, mode tag, slot
    digit and save count as full-width text, followed by the area name of the saved position
 */
VOID Card_mess_make(UCHAR* Mess, VOID* Save_data, INT Cursor);

/*
    classify the saved position as a save area 0-6 (index into the area name tables)
    from its stage and room
 */
INT Save_area_get(SAVE_POS_REC* pSave_pos);

/*
    show the card checking message, then probe both slots; returns a bit mask of the slots
    without a usable card (bit 0 = slot 0, bit 1 = slot 1)
 */
INT Wslot_check(VOID);

/*
    list the save files of the selected card, map them to the five slots and read each into
    its record in buf; returns 0 when ok, otherwise the card or format check result
 */
INT Check_card(UCHAR* F_num, UCHAR* buf);

/*
    open the software and hardware card events and enable them
 */
VOID Card_event_set(VOID);

/*
    clear the pending software card events
 */
VOID Clear_event(VOID);

/*
    clear the pending hardware card events
 */
VOID Clear_event2(VOID);

/*
    create and write a card file of Cap bytes from Head, with the block count and Title set
    in the header; retries each step, then reads the file back to verify;
    returns 0 when ok, 1 when it cannot be opened, 2 when the write or verify fails
 */
INT Card_write(_CARD* Head, INT Port, CHAR* Name, CHAR* Title, UINT Cap);

/*
    read the first 0x800 bytes of card file Name into Head, retrying; returns 0 when ok,
    1 when the file cannot be opened, 2 when the read fails; declared without parameters
    because Check_card passes a fourth argument
 */
INT Card_read();

/*
    list the files of card Port matching the pattern Pattern into Buff (names, sizes, count,
    total size); returns 0 when any file was found, 1 otherwise
 */
INT Card_search(INT Port, CFILE_INFO* Buff, CHAR* Pattern);

/*
    load the file system of card Port, up to three tries; returns 0 when ok, otherwise the
    last event code (1=no card, 2=unformatted, 3=error)
 */
INT Card_format_check(INT Port);

/*
    probe card Port until the result is stable (up to 16 tries) and clear its new-card status
    when it is ok; returns 0 when ok, 1 when no card, 3 on error
 */
INT Card_check(INT Port);

/*
    format card Port; returns 0 when ok
 */
INT Card_format(INT Port);

/*
    wait for one of the four software card events; returns 0=done, 1=time-out,
    2=new card, 3=error (also when none arrives in time)
 */
INT Get_card_event(VOID);

/*
    wait for one of the three hardware card events; returns 0=done, 1=time-out,
    2=error, 3=none arrived in time
 */
INT Get_card_event2(VOID);

/*
    clear the new-card status of card Port, up to ten tries; returns the last event code
 */
INT Card_clear(INT Port);

/*
    rename a file on the current card from Src_name to Dst_name
 */
VOID Card_rename(INT unused, CHAR* Src_name, CHAR* Dst_name);

// menu and message texts below are game-font glyph codes ending with 0x01
extern UCHAR Mc_mess_save[];                    // text "SAVE"
extern UCHAR Mc_mess_load[];                    // text "LOAD"
extern UCHAR Mc_mess_card1[];             // text "MEMORY CARD 1"
extern UCHAR Mc_mess_card2[];             // text "MEMORY CARD 2"
extern UCHAR Mc_mess_do_not[];                   // text "DO NOT"
extern UCHAR Mc_mess_sv_card[];          // text "SAVE ON CARD 1 OR 2?"
extern UCHAR Mc_mess_ld_card[];        // text "LOAD FROM CARD 1 OR 2?"
extern UCHAR Mc_mess_check[];     // text "CHECKING MEMORY CARD(S)"
extern UCHAR Mc_mess_overwrite[];               // text "OVERWRITE?"
extern UCHAR Mc_mess_yes[];                     // text "YES"
extern UCHAR Mc_mess_no[];                      // text "NO"
extern UCHAR Mc_mess_fmt_req[];    // text "FORMATTING IS REQUIRED"
extern UCHAR Mc_mess_fmt_yes[];          // text "FORMAT THIS CARD"
extern UCHAR Mc_mess_fmt_no[];     // text "DO NOT FORMAT THIS CARD"
extern UCHAR Mc_mess_use[];           // text "USE MEMORY CARD"
extern UCHAR Mc_mess_no_use[];       // text "DON'T USE MEMORY CARD"
extern UCHAR Mc_mess_no_data[];                  // text "NO DATA"
extern UCHAR Mc_mess_pull1[];            // text "DO NOT PULL OUT THE MEMORY CARD"
extern UCHAR Mc_mess_saving[];               // text "NOW SAVING..."
extern UCHAR Mc_mess_pull2[];           // text "DO NOT PULL OUT THE MEMORY CARD"
extern UCHAR Mc_err00[];       // text "FAILED TO FORMAT THE CARD"
extern UCHAR Mc_err01[];          // text "ACCESS ERROR"
extern UCHAR Mc_err02[];         // text "TOO MANY FILES"
extern UCHAR Mc_err03[];        // text "THERE IS NO FILE"
extern UCHAR Mc_err04[];      // text "MEMORY CARD IS NOT INSERTED"
extern UCHAR Mc_err05[];           // text "FAIL IN SAVE"
extern UCHAR Mc_err06[];   // text "FORMATTING IS REQUIRED"
extern UCHAR Mc_area00[];           // area text "M.Room 1F", each glyph followed by code 0xFB
extern UCHAR Mc_area01[];       // area text "M.Hall 1F", each glyph followed by code 0xFB
extern UCHAR Mc_area02[];         // area text "Courtyard Room B1", each glyph followed by code 0xFB
extern UCHAR Mc_area03[];        // area text "Guardhouse 1F", each glyph followed by code 0xFB
extern UCHAR Mc_area04[];        // area text "Laboratory B3", each glyph followed by code 0xFB
extern UCHAR Mc_area05[];      // area text "M.Storeroom 1F", glyphs followed by code 0xFB
extern UCHAR Mc_area06[];     // area text "Courtyard Path B1", each glyph followed by code 0xFB
extern USHORT Mc_err_pos_tbl[];                  // text x position of each error message, by error code
extern UCHAR* Mc_err_tbl[];              // error message text, by error code
extern UCHAR* Mc_area_tbl[];              // save area name text, by area index
extern UCHAR Mc_mess_chris[];              // save row text "Chris" with a save count placeholder
extern UCHAR Mc_mess_jill[];               // save row text "Jill" with a save count placeholder
extern UCHAR Mc_wait_tbl[];              // display time of each step of the save-complete highlight sweep
extern CHAR Mc_src_path[];            // card path buffer of the rename source, starts as "bu00:"
extern CHAR Mc_dst_path[];            // card path buffer of the rename destination, starts as "bu00:"
extern CFILE_INFO File_info;                // files found on the card being listed
extern UCHAR Mc_area_name[][0x15];             // full-width save area names appended to the save file title, by area index
extern const MC_DEV_BUF Mc_fname_tmpl;                   // card device path template "bu00:"

#endif
