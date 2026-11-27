/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog2\SELECT.H
 */

#ifndef SELECT_H
#define SELECT_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

typedef struct SELECT_STS_WORK SELECT_STS_WORK;
typedef struct SELECT_PL_WORK SELECT_PL_WORK;
typedef struct SELECT_WORK SELECT_WORK;
typedef struct SELECT_UV_TBL SELECT_UV_TBL;

struct SELECT_STS_WORK
{
    UCHAR Main_no;              // screen state: 0=fade in, 1=idle, 2=swap animation, 3..6=confirm and leave, 7=back out to the title
    UCHAR substate;             // step 0..5 of the swap animation while Main_no is 2
    SCHAR unused;               // cleared at start-up; never read
};

struct SELECT_PL_WORK
{
    SHORT pos_x;        // portrait x position
    SHORT pos_y;        // portrait y position
    SHORT pos_z;        // portrait depth for the perspective transform (0xF0 in front, 0x110 behind)
    SHORT vel_x;        // x speed during the swap animation
    USHORT vel_y;       // y speed during the swap animation
    SCHAR Add_x;        // x acceleration during the swap animation
    SCHAR accel_y;      // y acceleration during the swap animation
    UCHAR Ot_no;        // ordering-table layer offset of the portrait (0 in front, 1 behind)
    UCHAR Kido;         // colour modulation of the portrait quads (0x80 = full)
    UCHAR pad[2];       // padding: keeps the following fields at their layout offsets
    POLY_FT4 a[2][2];   // portrait quads: [draw buffer][left half, right half]
    POLY_FT4 b[2][5];   // frame quads: [draw buffer][three corner pieces, two edge strips]
};

struct SELECT_WORK
{
    INT Rtp_flg;                // flag output of the perspective transform
    INT Dummy004;               // never used
    MATRIX matrix;              // identity rotation with zero translation, loaded as the transform matrices
    INT Rtp_p;                  // interpolation output of the perspective transform
    INT Dummy02c;               // never used
    SVECTOR rot;                // rotation angles (all zero)
    VECTOR trans;               // translation (all zero)
    UCHAR character;            // character shown in front: 0=Chris, 1=Jill
    UCHAR Dummy049[7];          // never used
    SVECTOR Quad_sv[4];         // Vtx of the quad being transformed to screen space
    SELECT_STS_WORK state;      // screen state machine
    UCHAR Dummy073[5];          // never used
    SELECT_PL_WORK Pl_wk[2];    // portrait panel of each character, indexed by character
    UCHAR Left_flg;             // 1 when left was held at the swap request, else 0
    UCHAR timer;                // frame counter: swap animation steps, arrow blink, end wait
};

struct SELECT_UV_TBL
{
    UCHAR x;                    // left texture coordinate
    UCHAR y;                    // top texture coordinate
    UCHAR w;                    // width in texels
    UCHAR h;                    // height in texels
};

// set the texture coordinates of the four Vtx of a textured quad primitive (as the library macro of the same name)
#define setUV4(p, _u0, _v0, _u1, _v1, _u2, _v2, _u3, _v3) \
    (p)->u0 = (_u0), (p)->v0 = (_v0), (p)->u1 = (_u1), (p)->v1 = (_v1), \
    (p)->u2 = (_u2), (p)->v2 = (_v2), (p)->u3 = (_u3), (p)->v3 = (_v3)

// set the first vertex colour of a primitive (as the library macro of the same name)
#ifdef __INTELLISENSE__  // the SDK header already defines it
#undef setRGB0
#endif
#define setRGB0(p, _r0, _g0, _b0) (p)->r0 = (_r0), (p)->g0 = (_g0), (p)->b0 = (_b0)

/*
    overlay entry point: load and set up the two character portraits, then run the character
    select loop each frame; the player picks Chris or Jill, the choice is stored in the
    player id and the next program is chained
 */
VOID main(VOID);

/*
    queue both portraits (portrait quads and their frame quads) into the ordering table at
    their current positions and depths with the panel Kido applied, panel 1 first
 */
VOID Select_pl_disp(VOID);

/*
    queue the two blinking Sel_no arrows beside the portraits
 */
VOID Select_arrow_disp(VOID);

/*
    advance both Pl_wk by their current x and y velocities
 */
VOID Select_scrl_move(VOID);

/*
    add the x and y accelerations of both Pl_wk to their velocities
 */
VOID Select_scrl_add(VOID);

extern SELECT_UV_TBL Select_chris_uv_tbl[2];   // texture rectangles of the Chris portrait halves
extern SELECT_UV_TBL Select_jill_uv_tbl[2];    // texture rectangles of the Jill portrait halves
extern SELECT_UV_TBL Select_frm_uv_tbl[5]; // texture rectangles of the portrait frame pieces

#endif
