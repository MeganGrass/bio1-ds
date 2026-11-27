/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SUB.C
 */

#include "TYPES.H"
#include "SUB.H"
#include "SCRATCHPAD.H"
#include "MEM_MAP.H"

INT Mapping_tmd(UCHAR Mode, TMD_HEAD* pTmd_adr, INT Ctp, INT Ccl)
{
    INT n;
    UINT nObj;
    UINT i;
    UINT j;
    UINT* pPrim;
    UINT hdr;
    UINT Length;
    UINT nPoly;

    n = pTmd_adr->id;
    if (pTmd_adr->flags == 0)
    {
        GsMapModelingData((ULONG*)&pTmd_adr->flags);
    }
    nObj = pTmd_adr->nobj;
    pTmd_adr = (TMD_HEAD*)&pTmd_adr->obj0_vert_top;  // reinterprets the memory as TMD_HEAD
    for (i = 0; i < nObj; i++, pTmd_adr = (TMD_HEAD*)((TMD_MEM*)pTmd_adr + 1))  // reinterprets the memory as TMD_HEAD
    {
        pPrim = (UINT*)((TMD_MEM*)pTmd_adr)->prim_top;  // word view of the same memory
        nPoly = ((TMD_MEM*)pTmd_adr)->n_poly;  // reinterprets the memory as TMD_MEM
        for (j = 0; j < nPoly; j++)
        {
            hdr = *pPrim;
            Length = hdr >> 8;
            if (hdr & 0x04000000)
            {
                switch (Mode)
                {
                case 2:
                    pPrim[2] += Ctp << 16;
                case 1:
                    pPrim[1] += Ccl << 22;
                    break;
                case 0:
                    pPrim[2] += Ctp << 16;
                    break;
                }
            }
            pPrim += (Length & 0xFF) + 1;
        }
    }
    return n;
}

VOID Tmd_obj_link(TMD_HEAD* pTmd_adr, GsDOBJ5* pDobj, VOID* pCoord)
{
    if (pTmd_adr->flags == 0)
    {
        GsMapModelingData((ULONG*)&pTmd_adr->flags);
    }
    GsLinkObject5((ULONG)&pTmd_adr->obj0_vert_top, pDobj, 0);
    pDobj->coord2 = pCoord;  // pCoord points at the embedded coordinate block
    pDobj->attribute = 0;
    MAIN.pFree_work = GsPresetObject(pDobj, FREE_WORK_CUR);
}

VOID Mapping_tim(VOID* pTim_Adr)
{
    RECT Rect;
    GsIMAGE Tim_image;
    INT x;
    INT w;

    GsGetTimInfo((ULONG*)((UCHAR*)pTim_Adr + 4), &Tim_image);  // word view of the same memory

    x = CTPAGE_CUR << 6;
    if (MAIN.Vram_cur.b.Ctpage >= 0x10)
    {
        x -= 0x400;
    }

    Rect.x = x;

    Rect.y = (MAIN.Vram_cur.b.Ctpage >= 0x10) << 8;
    Rect.w = Tim_image.pw;
    Rect.h = Tim_image.ph;
    LoadImage(&Rect, Tim_image.pixel);
    w = Tim_image.pw + 0x3F;
    if (w < 0)
    {
        w = Tim_image.pw + 0x7E;
    }
    MAIN.Vram_cur.b.Ctpage += w >> 6;
    DrawSync(0);
    if ((Tim_image.pmode >> 3) & 1)
    {
        Rect.x = Tim_image.cx;
        Rect.y = CCLUT_UBYTE_CUR + 0x1E0;
        Rect.w = Tim_image.cw;
        Rect.h = Tim_image.ch;
        LoadImage(&Rect, Tim_image.clut);
        MAIN.Vram_cur.b.Cclut += Tim_image.ch;

        DrawSync(0);
    }
}

VOID Pix_trans(INT icon, INT slot, VOID* icons)
{
    ScratchPad->vram_rect.x = 0x36C;
    ScratchPad->vram_rect.y = (slot << 5) + 0x100;
    ScratchPad->vram_rect.w = 0x14;
    ScratchPad->vram_rect.h = 0x1E;
    LoadImage(&ScratchPad->vram_rect, (ULONG*)((UCHAR*)icons + icon * 0x4B0));  // word view of the same memory
}

VOID Ibox_pix_trans(INT icon, UINT slot)
{
    ScratchPad->vram_rect.x = (slot & 1) * 0x14 + 0x140;

    ScratchPad->vram_rect.y = ((slot >> 1) << 5) + 0x180;

    ScratchPad->vram_rect.w = 0x14;
    ScratchPad->vram_rect.h = 0x1E;
    LoadImage(&ScratchPad->vram_rect, (ULONG*)(icon * 0x4B0 + (UCHAR*)LOAD_BUF));  // word view of the same memory
}
