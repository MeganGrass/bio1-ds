/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\def\SCRATCHPAD.H
 */

#ifndef SCRATCHPAD_H
#define SCRATCHPAD_H

#include "TYPES.H"

struct SCRATCHPAD_BUF
{
    INT tmp00;              // scratch word, reused by each routine for its own temporaries
    INT tmp04;              // scratch word, reused by each routine for its own temporaries
    INT tmp08;              // scratch word, reused by each routine for its own temporaries
    INT tmp0C;              // scratch word, reused by each routine for its own temporaries
    INT tmp10;              // scratch word, reused by each routine for its own temporaries
    INT tmp14;              // scratch word, reused by each routine for its own temporaries
    INT tmp18;              // scratch word, reused by each routine for its own temporaries
    INT tmp1C;              // scratch word, reused by each routine for its own temporaries
    INT tmp20;              // scratch word, reused by each routine for its own temporaries
    INT tmp24;              // scratch word, reused by each routine for its own temporaries
    INT tmp28;              // scratch word, reused by each routine for its own temporaries
    INT tmp2C;              // scratch word, reused by each routine for its own temporaries
    INT tmp30;              // scratch word, reused by each routine for its own temporaries
    INT tmp34;              // scratch word, reused by each routine for its own temporaries
    INT tmp38;              // scratch word, reused by each routine for its own temporaries
    INT tmp3C;              // scratch word, reused by each routine for its own temporaries
    UCHAR Text_buf[64];     // scratch text buffer: formatted strings and the memory card area name
    UCHAR Dummy80[128];     // padding
    UCHAR Root_path[16];    // room route search: path being built
    UCHAR Root_best[16];    // room route search: best path found
    SHORT Poxz[48];         // room route search: zone points of the path
    MATRIX Item_mat;        // item viewer work matrix
    MATRIX Item_mat2;       // item viewer second work matrix (transpose of the first)
    VECTOR Item_vec;        // item viewer rotated vector
    SVECTOR Item_sv;        // item viewer input vector
    UCHAR Dummy1d8[400];    // padding
    RECT vram_rect;         // VRAM transfer rectangle
    GsBOXF Rect;            // filled box primitive (GsSortBoxFill)
    GsSPRITE Spr;           // sprite (GsSortSprite)
    GsGLINE Gline;          // gradient line primitive (GsSortGLine)
    GsLINE Line;            // line primitive (GsSortLine)
    SVECTOR Sv;             // short vector scratch
    VECTOR Vec;             // vector scratch
    MATRIX Mat;             // matrix scratch: rotation m and translation t
};

// The overlay views of the scratchpad sit at fixed offsets; the member names are those of the SDK type.
#define SCRATCH_OFS(Member) ((INT)&((SCRATCHPAD_BUF*)0)->Member)
typedef CHAR ScratchpadBuf_size[(sizeof(SCRATCHPAD_BUF) == 0x400) ? 1 : -1];
typedef CHAR ScratchpadItemMat_ofs[(SCRATCH_OFS(Item_mat) == 0x180) ? 1 : -1];
typedef CHAR ScratchpadItemMat2_ofs[(SCRATCH_OFS(Item_mat2) == 0x1A0) ? 1 : -1];
typedef CHAR ScratchpadItemVec_ofs[(SCRATCH_OFS(Item_vec) == 0x1C0) ? 1 : -1];
typedef CHAR ScratchpadItemSv_ofs[(SCRATCH_OFS(Item_sv) == 0x1D0) ? 1 : -1];
typedef CHAR ScratchpadRect_ofs[(SCRATCH_OFS(Rect) == 0x370) ? 1 : -1];
typedef CHAR ScratchpadSpr_ofs[(SCRATCH_OFS(Spr) == 0x380) ? 1 : -1];
typedef CHAR ScratchpadGline_ofs[(SCRATCH_OFS(Gline) == 0x3A4) ? 1 : -1];
typedef CHAR ScratchpadLine_ofs[(SCRATCH_OFS(Line) == 0x3B8) ? 1 : -1];
typedef CHAR ScratchpadSv_ofs[(SCRATCH_OFS(Sv) == 0x3C8) ? 1 : -1];
typedef CHAR ScratchpadVec_ofs[(SCRATCH_OFS(Vec) == 0x3D0) ? 1 : -1];
typedef CHAR ScratchpadMat_ofs[(SCRATCH_OFS(Mat) == 0x3E0) ? 1 : -1];

#define ScratchPad ((SCRATCHPAD_BUF*)SCRATCHPAD_ADR) // scratchpad: the 1 KB fast RAM of the CPU

/*
 *  Scratch words without a struct member.
 *  A member access ScratchPad->tmp1C and a plain array element are different memory references to GCC 2.6.3: the
 *  scheduler treats them differently, so some routines only match when a word is read through the array view.
 *  The arrays below are views of the same memory; the index is the word number (offset / 4) from the scratchpad base.
 *  ScratchWord[n] = ScratchPad->tmpNN with NN = 4 * n (hex); ScratchPtr / ScratchUptr / ScratchTmd / ScratchUword / ScratchHalf are the same words as
 *  pointer, UINT pointer, TMD pointer, UINT and USHORT (ScratchHalf is indexed in halfwords: word n = ScratchHalf[2 * n]).
 */
typedef INT SCRATCH_WORDS[16];
typedef UINT SCRATCH_UWORDS[16];
typedef LONG SCRATCH_LONGS[16];
typedef ULONG SCRATCH_ULONGS[16];
typedef USHORT SCRATCH_HALVES[32];
typedef VOID* SCRATCH_PTRS[16];
typedef UINT* SCRATCH_UPTRS[16];
typedef TMD_MEM* SCRATCH_TMDS[16];
typedef INT SCRATCH_VEC_WORDS[4];
typedef SHORT SCRATCH_SV_WORDS[4];
typedef USHORT SCRATCH_SV_UWORDS[4];
typedef INT SCRATCH_RECT_WORDS[4];
typedef INT SCRATCH_SPR_WORDS[9];
typedef USHORT SCRATCH_SPR_HALVES[18];

#define ScratchWord (*(SCRATCH_WORDS*)SCRATCHPAD_ADR)                // tmp00..tmp3C as INT
#define ScratchUword (*(SCRATCH_UWORDS*)SCRATCHPAD_ADR)              // tmp00..tmp3C as UINT
#define ScratchLong (*(SCRATCH_LONGS*)SCRATCHPAD_ADR)                // tmp00..tmp3C as LONG (the type of the PsyQ long* arguments)
#define ScratchUlong (*(SCRATCH_ULONGS*)SCRATCHPAD_ADR)              // tmp00..tmp3C as ULONG (the type of the PsyQ u_long* arguments)
#define ScratchHalf (*(SCRATCH_HALVES*)SCRATCHPAD_ADR)               // tmp00..tmp3C as USHORT, low half first
#define ScratchPtr (*(SCRATCH_PTRS*)SCRATCHPAD_ADR)                  // tmp00..tmp3C as pointer
#define ScratchUptr (*(SCRATCH_UPTRS*)SCRATCHPAD_ADR)                // tmp00..tmp3C as UINT pointer
#define ScratchTmd (*(SCRATCH_TMDS*)SCRATCHPAD_ADR)                  // tmp00..tmp3C as TMD pointer
#define ScratchVec (*(SCRATCH_VEC_WORDS*)&ScratchPad->Vec)           // Vec.vx, vy, vz, pad as INT
#define ScratchSv (*(SCRATCH_SV_WORDS*)&ScratchPad->Sv)              // Sv.vx, vy, vz, pad as SHORT
#define ScratchSvU (*(SCRATCH_SV_UWORDS*)&ScratchPad->Sv)            // Sv.vx, vy, vz, pad as USHORT
#define ScratchRect (*(SCRATCH_RECT_WORDS*)&ScratchPad->Rect)        // GsBOXF as INT words
#define ScratchSpr (*(SCRATCH_SPR_WORDS*)&ScratchPad->Spr)           // GsSPRITE as INT words
#define ScratchSprHalf (*(SCRATCH_SPR_HALVES*)&ScratchPad->Spr)      // GsSPRITE as USHORT halfwords
#define RECT_XY ScratchRect[1]               // Rect.x and Rect.y as one word
#define RECT_WH ScratchRect[2]               // Rect.w and Rect.h as one word
#define RECT_RGB ScratchRect[3]              // Rect.r, g, b and pad as one word
#define SPR_XY ScratchSpr[1]                 // Spr.x and Spr.y as one word
#define SPR_WH ScratchSpr[2]                 // Spr.w and Spr.h as one word
#define SPR_CXY ScratchSpr[4]                // Spr.cx and Spr.cy as one word
#define SPR_RGB ScratchSpr[5]                // Spr.r, g, b and pad as one word
#define SPR_MXY ScratchSpr[6]                // Spr.mx and Spr.my as one word
#define SPR_UV ScratchSprHalf[7]             // Spr.u and Spr.v as one halfword

#endif      // SCRATCHPAD_H
