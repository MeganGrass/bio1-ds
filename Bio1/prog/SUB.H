/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SUB.H
 */

#ifndef SUB_H
#define SUB_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

struct TMD_HEAD
{
    UINT id;                // TMD file id word
    UINT flags;             // TMD flags, 0 while the addresses still need relocation
    UINT nobj;              // number of objects
    UCHAR* obj0_vert_top;   // start of the first object's table (first TMD_MEM.ver_top slot); the TMD_MEM records follow from here
};

/*
    map a TMD for rendering and offset the texture page (Mode 0), the CLUT row (Mode 1) or both (Mode 2)
    of every textured primitive; returns the TMD id
 */
INT Mapping_tmd(UCHAR Mode, TMD_HEAD* pTmd_adr, INT Ctp, INT Ccl);

/*
    map a TMD if needed, link its first object to a GsDOBJ5 under the given coordinate system
    and preset the object, advancing the packet area pointer
 */
VOID Tmd_obj_link(TMD_HEAD* pTmd_adr, GsDOBJ5* pDobj, VOID* pCoord);

/*
    upload a TIM image to the next free texture page and advance the texture page cursor; when the
    image carries a CLUT, upload it to the next free CLUT row and advance the CLUT row cursor
 */
VOID Mapping_tim(VOID* pTim_Adr);

/*
    upload one Item_work item icon from the icon image list to the item icon slot in VRAM
    Declared without a parameter list: a caller passes a fourth argument the function ignores
 */
VOID Pix_trans();

/*
    upload one item box icon from the icon image list to the item box slot in VRAM
 */
VOID Ibox_pix_trans(INT icon, UINT slot);

#endif      // SUB_H
