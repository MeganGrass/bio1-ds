/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\CUT.H
 */

#ifndef CUT_H
#define CUT_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

/*
    apply the pending camera: load its VRAM cursor, rebuild the room sprites and, outside door mode,
    refresh the view, redraw the background and apply the room sprite hide mask
 */
INT Cut_set(VOID);

/*
    set the projection and the reference view from the current room's camera record
 */
VOID Set_view(VOID);

/*
    redraw the room background of the current cut: read its background image from the room file,
    verify the checksum, decode it to VRAM and copy it to the other display buffer; pad vibration
    is suspended while loading
 */
VOID Bg_draw(VOID);

/*
    test the player position against the camera switch zones of the current cut and switch
    to the target camera on a hit (deferred while the busy flag is set); redraws the current cut
    when nothing was hit and Flg is non-zero
 */
VOID Cut_check(INT Flg);

/*
    point the camera switch zone cursor at the zones of the current cut and apply it
 */
VOID Cut_change(VOID);

struct VCUT
{
    USHORT Tcut;    // camera cut switched to inside the zone
    USHORT Fcut;    // camera cut in which the zone is active
    USHORT X0;      // zone corner 0, x
    USHORT Z0;      // zone corner 0, z
    USHORT X1;      // zone corner 1, x
    USHORT Z1;      // zone corner 1, z
    USHORT X2;      // zone corner 2, x
    USHORT Z2;      // zone corner 2, z
    USHORT X3;      // zone corner 3, x
    USHORT Z3;      // zone corner 3, z
};

/*
    test whether the point lies inside the quadrilateral of a camera switch zone, using its
    four corner edges; returns 1 when inside
 */
INT Hit_ck_point4(VECTOR* pPos, VCUT* pVc);

#endif      // CUT_H
