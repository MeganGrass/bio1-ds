/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\MOVIE.H
 */

#ifndef MOVIE_H
#define MOVIE_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>

typedef struct CAPTION_REC CAPTION_REC;
typedef struct CAPTION_ID CAPTION_ID;
typedef struct MOVIE_TBL MOVIE_TBL;

struct CAPTION_REC
{
    INT Start_frame;        // first movie frame the subtitle is shown on (-1 ends the table)
    INT Disp_count;         // number of frames the subtitle stays
    USHORT No;              // index of the subtitle text block
    USHORT nLine;           // number of text lines
    USHORT Pos_x;           // screen x position
    USHORT Pos_y;           // screen y position
};

struct CAPTION_ID
{
    CAPTION_REC* pTbl_ptr;  // subtitle entry table of the movie
    INT Filecode;           // disc file holding the subtitle image bundle
};

struct MOVIE_TBL
{
    INT Filecode;           // disc file of the movie
    USHORT N_frame;         // last frame to play
    USHORT ring_sectors;    // size of the stream ring buffer in sectors
    USHORT Caption_id;      // subtitle table id (0xFF=none)
    USHORT flags;           // stream open flags
    UINT Rl_size;           // size of the VLC decode buffer in bytes
    USHORT Cancel;          // pad buttons that skip the movie (0=cannot be skipped)
    USHORT Dummy12;         // padding: keeps the entry size
};

/*
    play movie Movie_no using Work_top as stream memory: open the stream, decode and display
    frames until the movie ends or a skip button is pressed, then clear or restore the
    display for the caller; movie 1 with player id 1 skips frames 0x97E-0xA15 while its XA
    audio is stopped
    Declared without a parameter list: a caller passes a third argument the function ignores
 */
VOID Movie_play();

extern MOVIE_TBL Movie_tbl[];           // movie table by movie id: disc file, last frame, buffer sizes, flags, skip buttons
extern CAPTION_REC Caption_open_tbl[];     // subtitle entries of the opening movie
extern CAPTION_REC Caption_end_a_tbl[];     // subtitle entries of ending A
extern CAPTION_REC Caption_end_b_tbl[];     // subtitle entries of ending B

#endif      // MOVIE_H
