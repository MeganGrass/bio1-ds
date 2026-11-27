/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\TYRANT.H
 */

#ifndef TYRANT_H
#define TYRANT_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

typedef struct EM0C_DR_REC EM0C_DR_REC;
typedef struct EM0C_DR_TBL EM0C_DR_TBL;
typedef struct EM0C_PRIM_REC EM0C_PRIM_REC;
typedef struct EM0C_TRAIL_REC EM0C_TRAIL_REC;

struct EM0C_DR_REC
{
    DR_MODE Dr_mode;    // draw-mode primitive: selects the texture page and blend mode for the quad
    POLY_F4 p;          // semi-transparent flat-shaded quad
};

struct EM0C_DR_TBL
{
    EM0C_DR_REC q[4];   // four draw-mode and quad pairs
    UCHAR pad[0x70];    // padding: completes the set to 0x100 bytes, the room of a record's matrices and edge points
};

struct EM0C_PRIM_REC
{
    UCHAR hdr[0xC];     // draw-mode primitive in front of the quad (set up by Em0c_trail_alloc)
    POLY_F4 p;          // flat-shaded quad drawn for the trail segment
};

struct EM0C_TRAIL_REC
{
    EM0C_PRIM_REC a[2]; // quad for the previous record's edge to this record's mid edge, one per display buffer
    EM0C_PRIM_REC b[2]; // quad for this record's mid edge to its end edge, one per display buffer
    MATRIX A;           // first end matrix of the recorded edge
    MATRIX B;           // second end matrix of the recorded edge
    MATRIX M;           // midpoint of A and B, element by element
    SVECTOR C;          // near edge point of the trail
    SVECTOR D;          // far edge point of the trail
};

/*
    set up n sets of four semi-transparent flat quads, each preceded by its draw-mode primitive,
    with colours that brighten per set starting from col, then advance the load pointer past them
 */
VOID Em0c_trail_alloc(UCHAR n, EM0C_DR_TBL* pPrims, UINT col);

/*
    store the current trail edge (matrices pMat0 and pMat1, their midpoint matrix and the edge
    points pVec0 and pVec1) as a record: with n equal to 0 the record goes to slot idx; otherwise
    the n older records move back one slot, the record goes first and every record is projected
    into two quads queued in the active display buffer's ordering table
 */
VOID Em0c_trail_push(EM0C_TRAIL_REC* pRec0, MATRIX* pMat0, MATRIX* pMat1, SVECTOR* pVec0, SVECTOR* pVec1, UCHAR idx, UCHAR n);

#endif
