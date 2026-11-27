/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\GUIDE_CK.H
 */

#ifndef GUIDE_CK_H
#define GUIDE_CK_H

#include "TYPES.H"
#include "SCRATCHPAD.H"

typedef struct AREA_PT_REC AREA_PT_REC;
typedef struct AREA_REC AREA_REC;
typedef struct FLOOR_REC FLOOR_REC;

struct AREA_PT_REC
{
    SHORT x;    // X coordinate
    SHORT z;    // Z coordinate
};

struct AREA_REC
{
    USHORT id;      // zone count (first record) or adjacency mask of the previous zone; not read
    USHORT x0;      // zone minimum X
    USHORT z0;      // zone minimum Z
    USHORT x1;      // zone maximum X
    USHORT z1;      // zone maximum Z
    USHORT pad;     // direction bits of the zone, not read here
};

struct FLOOR_REC
{
    USHORT x;       // rectangle minimum X
    USHORT y;       // rectangle minimum Z
    USHORT w;       // rectangle width along X
    USHORT h;       // rectangle depth along Z
    UCHAR id;       // floor surface id of the rectangle, added to the footstep sound number
    UCHAR pad;      // padding: completes the 10-byte record
};

/*
    find the next waypoint for the current entity on the way to the point (x, z) through the
    room's walkable zones; z == 0 selects the zone index x and its centre as target.
    writes the waypoint to pOut_x/pOut_z and returns the next zone, the current zone | 0x10 when
    already in the target zone (waypoint is the target) or -1 when no route exists
 */
SCHAR Root_ck(SHORT x, SHORT z, SHORT* pOut_x, SHORT* pOut_z);

/*
    search the zone adjacency from zone src towards zone dst stepping through increasing zone
    numbers, keeping the shortest route in scratchpad; returns the first zone of the best route
    or -1
 */
SCHAR Root_ck_cw(INT src, UCHAR dst, SHORT x, SHORT z);

/*
    same as Root_ck_cw stepping through decreasing zone numbers
 */
SCHAR Root_ck_ccw(INT src, UCHAR dst, SHORT x, SHORT z);

/*
    returns the heading from (x, z) towards the point on the edge shared by zone src (the zone
    containing (x, z) when src is negative) and zone dst; the edge point is left in scratchpad
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
SHORT Root_edge_dir(INT x, INT z, INT dst, INT src);

/*
    clamp coordinate v into the span start..end of a shared zone edge, pulling in an end by an
    eighth of the span plus a margin when that end is blocked by collision; bit 0x8000 of edge
    selects the Z-constant edge; returns the span centre when no span is left
 */
INT Root_edge_lim(SHORT v, USHORT edge, SHORT start, SHORT end);

/*
    returns the index of the zone containing the point or 0xFF when none, and leaves the
    direction bits and adjacency mask of that zone in scratchpad
 */
INT Guide_check(INT target_x, INT target_z);

/*
    leave the point on the edge shared by the two zones in scratchpad (X then Z): the shared
    boundary coordinate and the midpoint of the overlapping span;
    returns 0 when the zones share an X boundary, 1 when they share a Z boundary
 */
INT Root_edge_set(USHORT Area0, USHORT Area1);

/*
    returns the collision flags of the room collision at the point (x, z)
 */
USHORT Sca_ck_point(SHORT x, SHORT z);

/*
    returns the heading (0..0xFFF) from the point Hontai to the point Target
 */
INT Direction_ck(INT Hontai_x, INT Hontai_z, INT Target_x, INT Target_z);

/*
    returns the heading from the current entity towards the point (x, z)
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
INT Get_dir_tgt(INT x, INT z);

/*
    returns the floor surface id of the room's footstep rectangle containing the point (the table
    is not terminated, so a containing rectangle must exist)
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
INT Floor_check(INT check_x, INT check_z);

struct BLOCK_REC
{
    USHORT Min_x;   // zone minimum x
    USHORT Min_z;   // zone minimum z
    USHORT Max_x;   // zone maximum x
    USHORT Max_z;   // zone maximum z
    USHORT Dir_bit; // direction bits of the zone, read into scratch by the route search
    USHORT Abut;    // bit mask of the zones sharing an edge with this zone, bit n = zone n
};

#endif      // GUIDE_CK_H
