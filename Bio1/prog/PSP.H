/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\PSP.H
 */

#ifndef PSP_H
#define PSP_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

struct PSP_SPR_REC
{
    UCHAR u;      // texture u
    UCHAR v;      // texture v
    UCHAR x;      // x offset from the sprite group origin
    UCHAR y;      // y offset from the sprite group origin
    USHORT depth; // sort depth
    USHORT attr;  // sprite attribute bits
    USHORT w;     // width
    USHORT h;     // height
};

/*
    build the sprite records of the current camera's room sprites from the camera sprite mask data,
    store the sprite count in the room header and upload the sprite texture when any sprite exists
 */
VOID Psp_set(VOID);

/*
    queue every active room sprite into the current ordering table at a depth-derived priority;
    nothing is queued while status flag 0x2000000 is set
 */
VOID Psp_trans(VOID);

/*
    deactivate every room sprite of the given group
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
INT Psp_off(INT psp_id);

/*
    activate every room sprite of the given group
 */
VOID Psp_on(UCHAR psp_id);

#endif      // PSP_H
