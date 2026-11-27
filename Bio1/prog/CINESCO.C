/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\CINESCO.C
 */

#include "CINESCO.H"

USHORT Cinesco_fade_rows; // number of CLUT rows held in each captured buffer
CINESCO_FADE_REC Cinesco_fade_tbl[4]; // fade entries: identifier, row, colour deltas and range
UINT* pCinesco_fade_src; // source copy of the CLUT rows captured from VRAM
UINT* pCinesco_fade_dst; // working copy of the CLUT rows, uploaded to VRAM after a fade
USHORT Cinesco_fade_num; // number of fade entries registered

VOID Cinesco_fade_clr(VOID)
{
    CINESCO_FADE_REC* pEntry;
    UCHAR i;

    i = 0;
    pEntry = Cinesco_fade_tbl;
    pCinesco_fade_src = 0;
    pCinesco_fade_dst = 0;
    Cinesco_fade_num = 0;
    Cinesco_fade_rows = 0;
    do
    {
        pEntry->dr = 0;
        pEntry->dg = 0;
        pEntry->db = 0;
        pEntry->start = 0;
        pEntry->count = 0x100;
        pEntry->active = 0;
        pEntry++;
        i++;
    } while (i < 4);
}

VOID Cinesco_fade_capture(INT Row_end)
{
    RECT rect;
    UINT* pBuf;
    USHORT i;

    if (Cinesco_fade_num != 0)
    {
        Cinesco_fade_rows = Row_end - 0xA;
        rect.x = 0;
        rect.y = 0x1EA;
        pBuf = FREE_WORK_CUR;
        rect.w = 0x100;
        pCinesco_fade_src = pBuf;

        pCinesco_fade_dst = (UINT*)((Cinesco_fade_rows << 9) + (INT)pBuf);  // word view of the same memory
        FREE_WORK_INT = (Cinesco_fade_rows << 10) + (INT)pBuf;  // the load pointer is advanced as an integer address (see FREE_WORK_INT)

        rect.h = Cinesco_fade_rows;
        StoreImage(&rect, (ULONG*)pBuf);  // word view of the same memory
        DrawSync(0);
        for (i = 0; i < (Cinesco_fade_rows << 7); i++)
        {
            pCinesco_fade_dst[i] = pCinesco_fade_src[i];
        }
    }
}

INT Cinesco_fade_add(row, id)
UINT row;  // parameter
SCHAR id;  // narrow parameter, passed as int
{
    INT idx;

    if (Cinesco_fade_num >= 4U)
    {
        return -1;
    }
    Cinesco_fade_tbl[Cinesco_fade_num].x0 = id;
    Cinesco_fade_tbl[Cinesco_fade_num].row = row + 0xF6;
    idx = Cinesco_fade_num << 0x18;
    Cinesco_fade_num += 1;
    return idx >> 0x18;
}

VOID Cinesco_fade_main(VOID)
{
    RECT rc;
    USHORT changed;
    USHORT i;
    USHORT j;
    USHORT* src;
    USHORT* dst;
    USHORT c;
    SHORT r;
    SHORT g;
    SHORT b;
    CINESCO_FADE_REC* pEntry;
    SCHAR d;
    SCHAR d1;
    INT row;

    changed = 0;
    pEntry = Cinesco_fade_tbl;
    if (Cinesco_fade_num != 0)
    {
        for (i = 0; i < 4; i++, pEntry++)
        {
            if (pEntry->active)
            {
                changed = 1;
                pEntry->active = 0;
                row = pEntry->row << 9;
                src = BYTE_OFS(USHORT, pCinesco_fade_src, row);
                dst = BYTE_OFS(USHORT, pCinesco_fade_dst, row);
                for (j = pEntry->start; j < pEntry->start + pEntry->count; j++)
                {
                    c = src[j];
                    if (c != 0)
                    {
                        do
                        {
                            g = c & 0x3E0;
                            b = c & 0x7C00;
                        } while (0);
                        d1 = pEntry->dr;
                        r = c & 0x1F;
                        r += d1;
                        if (r & ~0x1F)
                        {
                            r = d1 > 0 ? 0x1F : 1;
                        }
                        d = pEntry->dg;
                        g += d << 5;
                        if (g & ~0x3E0)
                        {
                            g = d > 0 ? 0x3E0 : 1;
                        }
                        d = pEntry->db;
                        b += d << 10;
                        if (b & ~0x7C00)
                        {
                            b = d > 0 ? 0x7C00 : 1;
                        }
                        dst[j] = r | (b | g) | (c & 0x8000);
                    }
                }
            }
        }
        if (changed)
        {
            rc.x = 0;
            rc.y = 0x1EA;
            rc.w = 0x100;
            rc.h = Cinesco_fade_rows;
            LoadImage(&rc, (ULONG*)pCinesco_fade_dst);  // word view of the same memory
        }
    }
}

VOID Cinesco_fade_sabun_add(dr, dg, db, start, count, id)
SCHAR dr;  // narrow parameter, passed as int
SCHAR dg;  // narrow parameter, passed as int
SCHAR db;  // narrow parameter, passed as int
SHORT start;  // narrow parameter, passed as int
USHORT count;  // narrow parameter, passed as int
UCHAR id;  // narrow parameter, passed as int
{
    UCHAR i;
    CINESCO_FADE_REC* pEntry;

    for (pEntry = Cinesco_fade_tbl, i = 0; i < 4; pEntry++, i++)
    {
        if (pEntry->x0 == id)
        {
            pEntry->start = start;
            pEntry->count = count;
            pEntry->dr = dr + pEntry->dr;
            pEntry->dg = dg + pEntry->dg;
            pEntry->db = db + pEntry->db;
            if (pEntry->dr < -0x1F)
            {
                pEntry->dr = -0x1F;
            }
            if (pEntry->dr >= 0x20)
            {
                pEntry->dr = 0x1F;
            }
            if (pEntry->dg < -0x1F)
            {
                pEntry->dg = -0x1F;
            }
            if (pEntry->dg >= 0x20)
            {
                pEntry->dg = 0x1F;
            }
            if (pEntry->db < -0x1F)
            {
                pEntry->db = -0x1F;
            }
            if (pEntry->db >= 0x20)
            {
                pEntry->db = 0x1F;
            }
            pEntry->active = 1;
            return;
        }
    }
}

VOID Cinesco_fade_set(dr, dg, db, start, count, id)
SCHAR dr;  // narrow parameter, passed as int
SCHAR dg;  // narrow parameter, passed as int
SCHAR db;  // narrow parameter, passed as int
SHORT start;  // narrow parameter, passed as int
USHORT count;  // narrow parameter, passed as int
UCHAR id;  // narrow parameter, passed as int
{
    INT i;
    CINESCO_FADE_REC* pEntry;
    INT one;
    UCHAR* pActive;
    pEntry = Cinesco_fade_tbl;
    i = 0;
    one = 1;
    pActive = (UCHAR*)Cinesco_fade_tbl + one;  // byte view of the same memory
loop_1:
    if (pEntry->x0 == id)
    {
        ((CINESCO_FADE_REC*)(pActive - 1))->dr = dr;  // reinterprets the memory as CINESCO_FADE_REC
        ((CINESCO_FADE_REC*)(pActive - 1))->dg = dg;  // reinterprets the memory as CINESCO_FADE_REC
        ((CINESCO_FADE_REC*)(pActive - 1))->db = db;  // reinterprets the memory as CINESCO_FADE_REC
        ((CINESCO_FADE_REC*)(pActive - 1))->start = start;  // reinterprets the memory as CINESCO_FADE_REC
        ((CINESCO_FADE_REC*)(pActive - 1))->count = count;  // reinterprets the memory as CINESCO_FADE_REC
        *pActive = one;
        return;
    }

    pActive += sizeof(CINESCO_FADE_REC);
    i += 1;
    pEntry++;
    if ((i & 0xFF) >= 4U)
    {
        do
        {
        } while (0);
        return;
    }
    goto loop_1;
}

VOID Cinesco_fade_off(UCHAR id, SHORT start, SHORT count)
{
    CINESCO_FADE_REC* pEntry;
    UCHAR i;

    for (pEntry = Cinesco_fade_tbl, i = 0; i < 4; pEntry++, i++)
    {
        if (pEntry->x0 == id)
        {
            pEntry->dr = 0;
            pEntry->dg = 0;
            pEntry->db = 0;
            pEntry->start = start;
            pEntry->count = count;
            pEntry->active = 1;
            return;
        }
    }
}
