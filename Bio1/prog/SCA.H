/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SCA.H
 */

#ifndef SCA_H
#define SCA_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include "SCRATCHPAD.H"

struct SCA_DATA
{
    USHORT Max_x;   // wall box maximum x
    USHORT Max_z;   // wall box maximum z
    USHORT Min_x;   // wall box minimum x
    USHORT Min_z;   // wall box minimum z
    USHORT Id;      // wall shape code, 4 and 5 are skipped by ray tests
    USHORT Type;    // shape flag word: 0x300 marks it as blocking for ray tests, bits 8-11 are set by the script; low byte feeds the height value of the Sca_ck_hit branch
};

struct SCA_BOX_REC
{
    USHORT Max_x;   // boundary maximum x
    USHORT Max_z;   // boundary maximum z
    USHORT Min_x;   // boundary minimum x
    USHORT Min_z;   // boundary minimum z
    UCHAR Id;       // boundary shape code, selects the response callback in Sca_rtn_tbl
    UCHAR Unk_9;    // high byte of the Id halfword; read as h[-1] in Sca_ck_hit (SCA.C): bits 0-6 scaled by 1000, bit 7 tested
    USHORT Type;    // shape flag word, bit 8 marks a boundary used by the weapon Damage check, bits 8-9 are the hit result; low byte feeds the height value of the Sca_ck_hit branch
};

typedef INT (*SCA_RTN_FN)(); // response callback of a collision shape type

/*
    relocate the room collision data (per-area boundary counts become boundary pointers)
    and install the response callbacks of the boundary shape types
 */
VOID Sca_rtn_init(VOID);

/*
    returns the shape type of the boundary when the point (scratchpad position plus ofs) lies
    inside the boundary grown by radius, or -1 when outside
 */
INT Sca_ck_type(SVECTOR* ofs, SCA_BOX_REC* box, USHORT radius);

/*
    returns the floor flags (high byte) of the boundary when the point (scratchpad position plus
    ofs) lies inside the boundary grown by radius, or -1 when outside
 */
SHORT Sca_ck_flg(SVECTOR* ofs, SCA_BOX_REC* box, USHORT radius);

/*
    returns the collision map area (0..3) of the point pos + ofs relative to the split
    coordinates Sx and Sz: bit 0 is set below Sx, bit 1 below Sz
 */
UINT Sca_get_area(VECTOR* pos, SVECTOR* ofs, INT Sx, INT Sz);

/*
    returns the outside code of the point (scratchpad position plus ofs) against the box
    x_lo..x_hi, z_lo..z_hi: one bit per side the point lies beyond, 0 when inside
 */
INT Sca_ck_out(SVECTOR* ofs, INT x_hi, INT z_hi, INT x_lo, INT z_lo);

/*
    test the current entity at pPos with radius R against the collision boundaries of its area
    and push it out of the shapes it touches.
    returns 0 when unobstructed, 1 when pushed out to a free position, 2 when still blocked
    (previous position restored); 3 instead when a boundary value was stored in the scratchpad
 */
INT Sca_ck_hit(VECTOR* pPos, USHORT R);

/*
    test two points offset from the current entity (rotated by its angle) against the collision
    boundaries; entity position and angle are restored when blocked.
    returns 0 when collision is disabled, the second point's hit flags when the first point is
    clear, 0x80 when blocked, otherwise 1
 */
UCHAR Sca_ck_point2(SVECTOR* pt0, SVECTOR* pt1);

/*
    returns the collision flag bits (mask 0x300) of the boundaries containing the point pos + ofs,
    or 1 as soon as a boundary has both bits set
 */
USHORT Sca_ck_pos(VECTOR* pos, SVECTOR* ofs);

/*
    same as Sca_ck_pos with the boundaries grown by radius
 */
INT Sca_ck_point_r(VECTOR* pos, SVECTOR* ofs, USHORT radius);

/*
    line test from the current entity position along the vector pV2 against the blocking boundaries
    of group Att (coordinates scaled down by 18); returns 1 when the segment crosses a diagonal
    of a boundary box, 0 otherwise
 */
INT Sca_ck_line(VECTOR* pV2, INT Att);

/*
    box response: push the current entity out of the box (grown by its radius) along the axis
    whose correction opposes its movement (the smaller correction when both do or the single one
    is large); the old position is restored when neither does. bit 0x10 of the status byte is
    cleared by a push along X and set by a push along Z
 */
VOID Sca_hit_box(SHORT* pScd, VECTOR* pos, SVECTOR* old);

/*
    circle response: push the current entity out of the circle (widened by its radius) along
    the line to the centre; status flag 0x10 is set when the push is mainly along Z, cleared
    otherwise. returns 1 when pushed, 0 when outside
 */
INT Sca_hit_circle(SCA_BOX_REC* pT_xz, VECTOR* pos);

/*
    set collision flag 8 on the current entity
 */
VOID Sca_hit_flg(VOID);

extern USHORT* pSca_last_grp;             // start of the boundary group last tested by Sca_ck_line
extern SCA_RTN_FN Sca_rtn_tbl[6];    // response callback per boundary shape type

struct SCA_HEAD
{
    SHORT Cx;           // room reference x used to pick the collision quadrant
    SHORT Cz;           // room reference z used to pick the collision quadrant
    USHORT* Group[5];   // start of each wall group in the collision data, entry 4 is the end of the last group
};

#endif      // SCA_H
