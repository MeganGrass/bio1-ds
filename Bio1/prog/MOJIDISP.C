/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\MOJIDISP.C
 */

#include "TYPES.H"

#include "MOJIDISP.H"
#include "INVENTORY.H"
#include "ITEM.H"
#include "NAMAE.H"
#include "SCE1.H"
#include "SCE_AT.H"
#include "ST_SUB03.H"
#include "FLAG.H"

// functions without a C definition
INT Mess_set();

VOID Mess_item_get(VOID)
{
    if (pAot_data->pRec->Item_id == 0x4D)
    {
        Flg_on(&MAIN.SaveData.Scenario_flg, 0x7F);  // address passed as an integer
        return;
    }

    St_get_item();
}

VOID Mess_item_use(VOID)
{
    UCHAR i;

    G.SaveData.Use_id = SEL_ID_CUR;

    if (MAIN.SaveData.Sel_id == 0xD)
    {
        return;
    }
    for (i = 0; SEL_ID_CUR != pItem_work_base[i].Id; i++)
    {
    }

    if (SEL_ID_CUR < 11)
    {
        pItem_work_base[i].Id = 0;
        if (EQUIP_NO_CUR == i + 1)
        {
            MAIN.SaveData.Equip_no = 0;
        }
    }
    else
    {
        if (pItem_work_base[i].Num == 0)
        {
            return;
        }
        pItem_work_base[i].Num--;
        if (pItem_work_base[i].Num != 0)
        {
            return;
        }

        if ((UCHAR)(SEL_ID_CUR - 0x33) < 10)   // MAIN field accessed as UCHAR*: plain access changes the code
        {
            MAIN.System_flg |= SYS_REQ_ITEM_USE;
            return;
        }

        pItem_work_base[i].Id = 0;
    }
    Sort_item();
}

VOID Mess_item_clr(VOID)
{
    ITEM_WORK* pItem = pItem_work_base;
    INT id = G.SaveData.Sel_id;
    UCHAR i;

    for (i = 0; id != pItem[i].Id; i++)
    {
    }
    pItem_work_base[i].Id = 0;
    Sort_item(pItem, id);   // Sort_item takes no parameters; the call still passes both
}

VOID Mess_act_nop(VOID)
{
}

VOID Moji_tbl_print(USHORT idx)
{
    UCHAR* pStr;
    UCHAR n;
    UCHAR y;

    ScratchPad->Spr.attribute = 0x40;
    ScratchPad->Spr.w = 8;
    ScratchPad->Spr.h = 0xE;

    ScratchPad->Spr.r = 0x80;
    ScratchPad->Spr.g = 0x80;
    ScratchPad->Spr.b = 0x80;
    ScratchPad->Spr.mx = 0;
    ScratchPad->Spr.my = 0;
    ScratchPad->Spr.rotate = 0;

    pStr = Moji_tbl[idx];
    do
    {
        n = *pStr++;
        ScratchPad->Spr.cx = ((*pStr & 3) << 4) + 0x100;
        ScratchPad->Spr.cy = (*pStr++ >> 2) + 0x1E0;
        ScratchPad->Spr.x = *pStr++ * 2 - Scr_ofs[0];
        ScratchPad->Spr.y = *pStr++ * 2 - Scr_ofs[1];
        do
        {
            switch (*pStr)
            {
            case 0xFF:
                ScratchPad->Spr.x += 4;
                break;

            case 0xF8:
                pStr++;
                y = *pStr / 18 * 14 - 0x2E;
                goto set;
            case 0xF9:
                pStr++;
                ScratchPad->Spr.tpage = 0x1F;
                ScratchPad->Spr.v = *pStr / 18 * 14;
                goto draw;
            case 0xFA:
                pStr++;
                ScratchPad->Spr.tpage = 0x1F;
                ScratchPad->Spr.v = *pStr / 18 * 14 - 0x3C;
                goto draw;
            default:
                y = *pStr / 18 * 14 + 0x1C;
            set:
                ScratchPad->Spr.v = y;
                ScratchPad->Spr.tpage = 0x1E;
            draw:
                ScratchPad->Spr.u = *pStr % 18 * 8;

                GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 2);

            case 0:
                ScratchPad->Spr.x += 8;
                break;
            }
            pStr++;
        } while (--n);
    } while (*pStr != 0);
}

VOID Name_disp(Item_id, Attr, Pos_x, Pos_y)
UCHAR Item_id;  // narrow parameter, passed as int
UCHAR Attr;  // narrow parameter, passed as int
USHORT Pos_x;  // narrow parameter, passed as int
SHORT Pos_y;  // narrow parameter, passed as int
{
    UINT Prio;

    SHORT x;
    SHORT h;
    UCHAR* pName;
    UCHAR v;

    if (Item_id != 0)
    {
        Prio = Attr >> 4;
        if (Prio == 0)
        {
            Prio = 2;
        }
        x = Attr & 3;
        h = (Attr & 0xF) >> 2;
        ScratchPad->Spr.attribute = 0x40;

        ScratchPad->Spr.w = 8;

        ScratchPad->Spr.h = 0xE;

        ScratchPad->Spr.x = Pos_x;
        ScratchPad->Spr.y = Pos_y;

        ScratchPad->Spr.cx = (x << 4) + 0x100;
        ScratchPad->Spr.cy = h + 0x1E0;

        ScratchPad->Spr.r = 0x80;
        ScratchPad->Spr.g = 0x80;
        ScratchPad->Spr.b = 0x80;
        ScratchPad->Spr.mx = 0;
        ScratchPad->Spr.my = 0;
        ScratchPad->Spr.rotate = 0;

        pName = name_ptr_set(Item_id);
        while (*pName != 7)
        {
            switch (*pName)
            {
            case 0xF8:
                pName++;
                ScratchPad->Spr.tpage = 0x1E;
                v = *pName / 18 * 14 - 0x2E;
                break;
            case 0xF9:
                pName++;
                ScratchPad->Spr.tpage = 0x1F;
                v = *pName / 18 * 14;
                break;
            case 0xFA:
                pName++;
                ScratchPad->Spr.tpage = 0x1F;
                v = *pName / 18 * 14 - 0x3C;
                break;

            default:
                ScratchPad->Spr.tpage = 0x1E;
                v = *pName / 18 * 14 + 0x1C;
                break;
            }
            ScratchPad->Spr.v = v;
            ScratchPad->Spr.u = *pName % 18 * 8;

            NOPROTO(VOID, GsSortFlipSprite)(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], Prio);

            pName++;
            ScratchPad->Spr.x += 8;
        }
    }
}

UCHAR* name_ptr_set(INT Item_id)
{
    INT flag_idx;
    UCHAR* pName;
    UINT idx;
    UCHAR Item_attribute;

    idx = (Item_id - 1) & 0xFF;
    pName = Name_tbl[idx];

    if (idx < 0x4DU)
    {
        Item_attribute = Item_data_tbl.items[idx].Item_attribute;
        if (!(Item_attribute & 0x80))
        {
            flag_idx = Item_attribute & 0xFF;
            if (Flg_ck(&MAIN.SaveData.Item_c_flg, flag_idx) == 0)  // address passed as an integer
            {
                pName = Name_alt_tbl[flag_idx];
            }
        }
    }
    return pName;
}

VOID Mess_print(INT Pos_x, INT Pos_y, INT Attr, UCHAR* pMess)
{
    UINT Prio;

    UCHAR* pPtr;
    INT v;
    UCHAR Clt;

    pPtr = pMess;
    Prio = (UINT)(Attr & 0xFF) >> 4;  // unsigned conversion (signed operand)
    if (Prio == 0)
    {
        Prio = 2;
    }
    Clt = Attr & 0xF;
    ScratchPad->Spr.attribute = 0x40;
    ScratchPad->Spr.w = 8;
    ScratchPad->Spr.h = 0xE;
    ScratchPad->Spr.cx = ((Clt & 3) << 4) + 0x100;
    ScratchPad->Spr.cy = (Clt >> 2) + 0x1E0;

    ScratchPad->Spr.r = 0x80;
    ScratchPad->Spr.g = 0x80;
    ScratchPad->Spr.b = 0x80;
    ScratchPad->Spr.mx = 0;
    ScratchPad->Spr.my = 0;
    ScratchPad->Spr.rotate = 0;

    ScratchPad->Spr.x = Pos_x - Scr_ofs[0];
    ScratchPad->Spr.y = Pos_y - Scr_ofs[1];
    while (*pPtr != 1)
    {
        if (*pPtr == 0)
        {
            goto adv8;
        }
        switch (*pPtr)
        {
        case 0xFF:
            ScratchPad->Spr.x += 4;
            break;
        case 0xF8:
            pPtr++;
            v = *pPtr / 18 * 0xE - 0x2E;
            goto setv;
        case 0xF9:
            pPtr++;
            ScratchPad->Spr.tpage = 0x1F;
            ScratchPad->Spr.v = *pPtr / 18 * 0xE;
            goto draw;
        case 0xFA:
            pPtr++;
            ScratchPad->Spr.tpage = 0x1F;
            ScratchPad->Spr.v = *pPtr / 18 * 0xE - 0x3C;
            goto draw;
        default:
            v = *pPtr / 18 * 0xE + 0x1C;
        setv:
            ScratchPad->Spr.v = v;
            ScratchPad->Spr.tpage = 0x1E;
        draw:
            ScratchPad->Spr.u = *pPtr % 18 * 8;

            NOPROTO(VOID, GsSortFlipSprite)(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], Prio);  // unprototyped call (prototype changes the code)

        adv8:
            ScratchPad->Spr.x += 8;
            break;
        case 0xFB:
            break;
        case 1:
        case 7:
            return;
        }
        pPtr++;
    }
}

VOID Print8(INT x, INT y, INT c, INT p)
{
    ULONG* attr;
    UINT prio;

    UCHAR col;

    UCHAR* pPtr;
    UCHAR ch;
    SCHAR t;

    INT k;

    prio = (UINT)(c & 0xFF) >> 4;  // unsigned conversion (signed operand)
    if (prio == 0)
    {
        prio = 2;
    }
    col = c & 0xF;

    attr = &ScratchPad->Spr.attribute;
    k = p & 0xFF;
    *attr = k ? 0x40000040 : 0x40;

    ScratchPad->Spr.w = 8;
    ScratchPad->Spr.h = 8;

    ScratchPad->Spr.tpage = 0x1E;

    ScratchPad->Spr.r = 0x80;
    ScratchPad->Spr.g = 0x80;
    ScratchPad->Spr.b = 0x80;
    ScratchPad->Spr.mx = 0;
    ScratchPad->Spr.my = 0;
    ScratchPad->Spr.rotate = 0;

    ScratchPad->Spr.x = x - Scr_ofs[0];
    ScratchPad->Spr.y = y - Scr_ofs[1];
    if (p & 0xFF)
    {
        col += 8;
    }
    k = col & 3;
    ScratchPad->Spr.cx = k * 0x10 + 0x100;
    ScratchPad->Spr.cy = (col >> 2) + 0x1E0;
    pPtr = ScratchPad->Text_buf;
    do
    {
        ch = *pPtr++;
        if (ch != ' ')
        {
            t = ch - ' ';
            ScratchPad->Spr.u = (t & 0x1F) * 8;
            ScratchPad->Spr.v = (t & 0xE0) >> 2;
            NOPROTO(VOID, GsSortFastSprite)(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], prio);
        }
        ScratchPad->Spr.x += 8;
    } while (*pPtr != 0);
}

VOID Print14(INT x, INT y, INT c, UCHAR p)
{
    UCHAR col;
    UINT prio;
    UCHAR* pPtr;
    SHORT ch;

    prio = (UINT)(c & 0xFF) >> 4;  // unsigned conversion (signed operand)
    if (prio == 0)
    {
        prio = 2;
    }
    col = c & 0xF;
    (&ScratchPad->Spr)->attribute = p ? 0x40000040 : 0x40; // plain Spr.attribute changes scheduling

    ScratchPad->Spr.w = 8;

    ScratchPad->Spr.h = 0xE;

    ScratchPad->Spr.tpage = 0x1E;

    ScratchPad->Spr.r = 0x80;
    ScratchPad->Spr.g = 0x80;
    ScratchPad->Spr.b = 0x80;
    ScratchPad->Spr.mx = 0;
    ScratchPad->Spr.my = 0;
    ScratchPad->Spr.rotate = 0;

    ScratchPad->Spr.x = x - Scr_ofs[0];
    ScratchPad->Spr.y = y - Scr_ofs[1];
    if (p)
    {
        col += 8;
    }
    ScratchPad->Spr.cx = ((col & 3) << 4) + 0x100;
    ScratchPad->Spr.cy = (col >> 2) + 0x1E0;
    pPtr = ScratchPad->Text_buf;
    do
    {
        ch = *pPtr;
        if (ch != ' ')
        {
            ScratchPad->Spr.u = *pPtr % 18 * 8;

            ScratchPad->Spr.v = *pPtr / 18 * 0xE;
            NOPROTO(VOID, GsSortFlipSprite)(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], prio);  // unprototyped call (prototype changes the code)
        }
        pPtr += 1;

        ScratchPad->Spr.x += 8;
    } while (*pPtr != 0);
}

INT Mess_set(msg, msg_id)
UCHAR msg;      // message number and option bits
SHORT msg_id;   // update flag bits cleared while the message is shown
{
    USHORT old;
    USHORT* base;

    if (MESS_FLG_CUR & MESS_FLG_ACTIVE)
    {
        return 1;
    }
    old = STOP_FLG_CUR;
    MAIN.SaveData.Mess_flg = MESS_FLG_ACTIVE;
    G.Stop_data = msg_id;
    G.Stop_keep = old;
    MAIN.Stop_flg = old & ~msg_id;
    if (msg & 0x40)
    {
        G.Mess_top = Mess_tbl[msg & 0x3F];
    }
    else
    {
        base = G.pRoom->pMessage;
        VIEW(USHORT*, MAIN.Mess_top) = base;   // MAIN field accessed as USHORT**: plain access changes the code
        {
            INT msg_ofs = (msg & 0x3F) * 2;
            VIEW(INT, MAIN.Mess_top) = *BYTE_OFS(USHORT, base, msg_ofs) + (INT)base;  // INT* view of the field (a plain access changes the code)
        }
    }
    G.Mess_r_no0 = 0;
    if (G.System_flg & SYS_STATUS_MASK)
    {
        G.Pos_y_def = 0xBA - Scr_ofs[1];
    }
    else
    {
        G.Pos_y_def = 0xB5 - Scr_ofs[1];
    }
    G.Mess_type = msg & 0x80;

    return 0;
}

INT Item_mess_set(USHORT item_idx, SHORT Stop_mask)
{
    USHORT old;

    if (MESS_FLG_CUR & MESS_FLG_ACTIVE)
    {
        return 1;
    }
    old = STOP_FLG_CUR;
    MAIN.SaveData.Mess_flg = MESS_FLG_ACTIVE;
    G.Mess_type = 0x80;
    G.Stop_data = Stop_mask;
    G.Mess_r_no0 = 0;
    G.Stop_keep = old;
    MAIN.Stop_flg = old & ~Stop_mask;
    G.Mess_top = Item_mess_tbl[item_idx];
    G.Pos_y_def = 0xBA - Scr_ofs[1];

    return 0;
}

VOID Mess_disp(VOID)
{
    UCHAR c;
    INT v;
    UCHAR** ppMess_end;
    UCHAR* pPtr;
    UCHAR* pMess_time;
    UCHAR* pMess_wait;
    UCHAR* pNext;

    ScratchPad->Spr.r = 0x80;
    ScratchPad->Spr.g = 0x80;
    ScratchPad->Spr.b = 0x80;
    ScratchPad->Spr.mx = 0;
    ScratchPad->Spr.my = 0;
    ScratchPad->Spr.rotate = 0;

    switch (G.Mess_r_no0)
    {
    case 0:
        G.Mess_r_no0 = 1;
        MESS_WAIT_CUR = VLOOP_CUR == 0 ? 4 : 2;
        MESS_TIME_CUR = MAIN.Vloop == 0 ? 2 : 1;
        G.Mcol_init = 0;
        G.Mcol_next = 0;
        G.Mess_end = G.Mess_top;
    case 1:
        v = MESS_TIME_CUR;
        MAIN.Mess_time = v - 1;
        if (G.Mess_type != 0)
        {
            if (MAIN.Mess_time == 0)
            {
                goto scan;
            }
            if (G.SaveData.Key & 0x4000)
            {
                MAIN.Mess_time = v - 2;
            }
        }
        if (G.Mess_time != 0)
        {
            break;
        }
    scan:
        if (*G.Mess_end != 0)
        {
            ppMess_end = &MAIN.Mess_end;
            pMess_time = &MAIN.Mess_time;
            pMess_wait = &MAIN.Mess_wait;
            do
            {
                pPtr = *ppMess_end;
                if ((UCHAR)(*pPtr - 12) < 0xEC)  // truncates to a byte
                {
                    goto out;
                }
                switch (*pPtr)
                {
                case 1:
                    *ppMess_end = pPtr + 1;
                    if (pPtr[1] != 0)
                    {
                        UCHAR Sub_ctr;

                        G.Mess_r_no0 = 6;
                        Sub_ctr = pPtr[1];

                        if (G.Vloop == 0)
                        {
                            Sub_ctr <<= 1;
                        }
                        *pMess_time = Sub_ctr;
                    }
                    else
                    {
                        G.Mess_r_no0 = 5;
                    }
                    goto end;
                case 3:
                    *ppMess_end = pPtr + 1;
                    if (pPtr[1] != 0)
                    {
                        UCHAR Sub_ctr;

                        G.Mess_r_no0 = 3;
                        Sub_ctr = pPtr[1];

                        if (G.Vloop == 0)
                        {
                            Sub_ctr <<= 1;
                        }
                        *pMess_time = Sub_ctr;
                    }
                    else
                    {
                        G.Mess_r_no0 = 2;
                    }
                    (*ppMess_end)++;
                    goto end;
                case 8:
                    G.Mess_r_no0 = 4;
                    goto end;
                case 4:
                    *ppMess_end = pPtr + 1;
                    if (pPtr[1] == 0)
                    {
                        *ppMess_end = pPtr + 2;
                        if (pPtr[2] != 4)
                        {
                            INT k = 4;

                            do
                            {
                                switch (**ppMess_end)
                                {
                                case 5:
                                case 6:
                                case 0xF8:
                                case 0xF9:
                                case 0xFA:
                                    (*ppMess_end)++;
                                    break;
                                case 1:
                                case 2:
                                case 3:
                                case 4:
                                    break;
                                case 8:
                                case 9:
                                case 10:
                                    break;
                                }
                                pNext = *ppMess_end;
                                *ppMess_end = pNext + 1;
                            } while (pNext[1] != k);
                        }
                        (*ppMess_end)++;
                    }
                    {
                        UCHAR d;
                        UCHAR* t;

                        t = *ppMess_end;
                        d = *t;
                        if (G.Vloop == 0)
                        {
                            d <<= 1;
                        }
                        *pMess_wait = d;
                        *ppMess_end = t + 1;
                    }
                    break;
                case 0xF8:
                case 0xF9:
                case 0xFA:
                    (*ppMess_end)++;

                    goto out;

                case 5:
                    *ppMess_end = pPtr + 1;
                    G.Mcol_next = pPtr[1];
                case 2:
                    (*ppMess_end)++;
                    break;
                case 6:
                {
                    UCHAR w;

                    G.Mess_svp = pPtr;
                    *ppMess_end = pPtr + 1;
                    if (pPtr[1] != 0)
                    {
                        w = pPtr[1];
                    }
                    else
                    {
                        w = MAIN.SaveData.Sel_id;   // cast form: keeps the register allocation
                    }
                    G.Mess_end = name_ptr_set(w);
                    break;
                }
                case 7:
                    *ppMess_end = G.Mess_svp + 2;
                    break;
                }
            } while (*G.Mess_end != 0);
        }
    out:
        MESS_END_CUR++;
        G.Mess_time = G.Mess_wait;
        break;
    case 2:
        if (G.SaveData.Key_trg & (0x4000 | 0x8000))
        {
            UCHAR* width = &MAIN.Mess_time;

            G.Mess_r_no0 = 1;
            G.Mess_top = G.Mess_end;
            *width = G.Vloop == 0 ? 2 : 1;
            G.Mcol_init = G.Mcol_next;
            break;
        }
        {
            INT t;

            MESS_TIME_CUR--;
            t = MAIN.Mess_time;
            if (G.Vloop == 0)
            {
                if (t & 0x30)
                {
                    goto draw2;
                }
                break;
            }
            if (!(t & 0x18))
            {
                break;
            }
        }
    draw2:
        ScratchPad->Spr.attribute = 0x40;
        ScratchPad->Spr.w = 8;
        ScratchPad->Spr.h = 0xE;
        ScratchPad->Spr.tpage = 0x1E;
        ScratchPad->Spr.u = 0x58;
        ScratchPad->Spr.v = 0x1C;
        ScratchPad->Spr.cx = 0x100;
        ScratchPad->Spr.cy = 0x1E0;
        ScratchPad->Spr.x = 0x99 - Scr_ofs[0];
        ScratchPad->Spr.y = G.Pos_y_def + 0x1E;

        GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 2);
        break;

    case 3:
    {
        UCHAR* width = &MAIN.Mess_time;

        (*width)--;
        if (*width != 0)
        {
            break;
        }

        G.Mess_r_no0 = 1;
        c = G.Mess_wait;
        G.Mess_top = G.Mess_end;
        if (G.Vloop == 0)
        {
            c <<= 1;
        }

        *width = c;
        G.Mcol_init = G.Mcol_next;
        break;
    }

    case 4:
        if (G.SaveData.Key_trg & 0x4000)
        {
            MESS_FLG_CUR &= ~MESS_FLG_ACTIVE;
            G.Stop_flg = G.Stop_keep;
            Branch_disp();
            return;
        }
        if (G.Padd1_trg & (PAD_RIGHT | PAD_LEFT))
        {
            G.Mess_time = 0;
            MESS_FLG_CUR ^= MESS_FLG_ANSWER;
        }
        {
            INT t;

            MESS_TIME_CUR--;
            t = MAIN.Mess_time;
            if (G.Vloop == 0)
            {
                if (t & 0x30)
                {
                    goto draw4;
                }
                goto text;
            }
            if (!(t & 0x18))
            {
                goto text;
            }
        }
    draw4:
        ScratchPad->Spr.attribute = 0x40;
        if (G.SaveData.Mess_flg & MESS_FLG_ANSWER)
        {
            ScratchPad->Spr.x = 0xF8 - Scr_ofs[0];
        }
        else
        {
            ScratchPad->Spr.x = 0xD0 - Scr_ofs[0];
        }
        ScratchPad->Spr.w = 8;
        ScratchPad->Spr.h = 0xE;
        ScratchPad->Spr.tpage = 0x1E;
        ScratchPad->Spr.u = 0x10;
        ScratchPad->Spr.v = 0x1C;
        ScratchPad->Spr.cx = 0x100;
        ScratchPad->Spr.cy = 0x1E0;
        ScratchPad->Spr.y = G.Pos_y_def + 0x10;

        GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 2);

    text:
        sprintf(ScratchPad->Text_buf, "Yes  No"); // CHAR* for sprintf: Text_buf is a UCHAR buffer
        {
            INT text_y = (USHORT)Scr_ofs[1] + 0x10;  // real width or sign conversion
            Print14(0xD8, (USHORT)(G.Pos_y_def + text_y), 0, 0);  // real width or sign conversion
        }
        break;
    case 5:
        if (!(G.SaveData.Key_trg & (0x4000 | 0x8000)))
        {
            break;
        }
        MESS_FLG_CUR &= ~MESS_FLG_ACTIVE;
        if (!(STOP_FLG_CUR & STOP_PLAYER))
        {
            KEY_CUR &= 0xF000;
            KEY_OLD_CUR &= 0xF000;
        }
        MAIN.Stop_flg = G.Stop_keep;
        return;
    case 6:
        MESS_TIME_CUR--;
        if (MAIN.Mess_time != 0)
        {
            break;
        }
        MESS_FLG_CUR &= ~MESS_FLG_ACTIVE;
        if (!(STOP_FLG_CUR & STOP_PLAYER))
        {
            KEY_CUR &= 0xF000;
            KEY_OLD_CUR &= 0xF000;
        }
        MAIN.Stop_flg = G.Stop_keep;
        return;
    }
end:
    mess_trans();
}

VOID Branch_disp(VOID)
{
    UCHAR action;
    VOID* next;
    UCHAR* branch;
    UCHAR* cur;

    cur = MESS_END_CUR;
    next = cur + 1;
    MAIN.Mess_end = next;

    branch = (VOID*)(*(cur + 1) * (G.SaveData.Mess_flg & MESS_FLG_ANSWER) + (INT)next);  // untyped pointer, the callee takes the address as is

    MAIN.Mess_end = branch + 1;
    action = *(branch + 1);
    MAIN.Mess_end = branch + 2;
    switch (action)
    {
    case 8:
        break;
    case 9:
        Mess_set(*(branch + 2), G.Stop_data);
        return;
    case 10:
        Mess_func_tbl[*(branch + 2)]();
        return;
    }
}

VOID mess_trans(VOID)
{
    UCHAR* pPtr;
    UCHAR* pSave;
    INT v;
    UCHAR c;

    ScratchPad->Spr.attribute = 0x40;

    ScratchPad->Spr.w = 8;

    ScratchPad->Spr.h = 14;

    pPtr = G.Mess_top;

    ScratchPad->Spr.x = 0x30 - Scr_ofs[0];
    ScratchPad->Spr.y = G.Pos_y_def;
    ScratchPad->Spr.cx = ((MCOL_INIT_CUR & 3) << 4) + 0x100;
    ScratchPad->Spr.cy = (MAIN.Mcol_init >> 2) + 0x1E0;
    while (pPtr != G.Mess_end)
    {
        if (*pPtr != 0)
        {
            switch (*pPtr)
            {
            case 3:
            case 4:
                pPtr++;
            case 1:
                pPtr++;
                continue;
            case 5:
                pPtr++;
                ScratchPad->Spr.cx = ((*pPtr & 3) << 4) + 0x100;
                ScratchPad->Spr.cy = (*pPtr >> 2) + 0x1E0;
                pPtr++;
                continue;
            case 2:
                ScratchPad->Spr.x = 0x30 - Scr_ofs[0];
                ScratchPad->Spr.y += 0x10;
                pPtr++;
                continue;
            case 0xF8:
                pPtr++;
                v = *pPtr / 18 * 14 - 0x2E;
                goto setv;
            case 0xF9:
                pPtr++;
                ScratchPad->Spr.tpage = 0x1F;
                ScratchPad->Spr.v = *pPtr / 18 * 14;
                goto draw;
            case 0xFA:
                pPtr++;
                ScratchPad->Spr.tpage = 0x1F;
                ScratchPad->Spr.v = *pPtr / 18 * 14 - 0x3C;
                goto draw;
            case 6:
                pSave = pPtr;
                pPtr++;
                if (*pPtr != 0)
                {
                    c = *pPtr;
                }
                else
                {
                    c = G.SaveData.Sel_id;
                }
                pPtr = name_ptr_set(c);
                continue;

            case 7:
                pPtr = pSave + 2;
                continue;
            default:
                v = *pPtr / 18 * 14 + 0x1C;
            setv:
                ScratchPad->Spr.v = v;
                ScratchPad->Spr.tpage = 0x1E;
            draw:
                ScratchPad->Spr.u = *pPtr % 18 * 8;

                GsSortFlipSprite(&ScratchPad->Spr, MAIN.Ot0 + G.Cbuf_id, 2);
            }
        }
        ScratchPad->Spr.x += 8;
        pPtr++;
    }
}

VOID Mess_nop(VOID)
{
}

// message branch actions, by action index
MESS_FUNC Mess_func_tbl[4] =
{
    Mess_item_get,    // 0 //
    Mess_item_use,    // 1 //
    Mess_item_clr,   // 2 //
    Mess_act_nop,     // 3 //
};

// encoded text of all global pMessage, one after another; Mess_tbl points into it
UCHAR Mess_data[0x83C] =
{
    0x33,   // 0 //
    0x45,   // 1 //
    0x48,   // 2 //
    0x48,   // 3 //
    0x00,   // 4 //
    0x55,   // 5 //
    0x4B,   // 6 //
    0x51,   // 7 //
    0x00,   // 8 //
    0x50,   // 9 //
    0x3D,   // 10 //
    0x47,   // 11 //
    0x41,   // 12 //
    0x02,   // 13 //
    0x50,   // 14 //
    0x44,   // 15 //
    0x41,   // 16 //
    0x00,   // 17 //
    0x05,   // 18 //
    0x01,   // 19 //
    0x06,   // 20 //
    0x00,   // 21 //
    0x05,   // 22 //
    0x00,   // 23 //
    0x1B,   // 24 //
    0x08,   // 25 //
    0x02,   // 26 //
    0x0A,   // 27 //
    0x00,   // 28 //
    0x01,   // 29 //
    0x00,   // 30 //
    0x35,   // 31 //
    0x4B,   // 32 //
    0x51,   // 33 //
    0x00,   // 34 //
    0x43,   // 35 //
    0x4B,   // 36 //
    0x50,   // 37 //
    0x00,   // 38 //
    0x50,   // 39 //
    0x44,   // 40 //
    0x41,   // 41 //
    0x00,   // 42 //
    0x05,   // 43 //
    0x01,   // 44 //
    0x06,   // 45 //
    0x00,   // 46 //
    0x05,   // 47 //
    0x00,   // 48 //
    0x79,   // 49 //
    0x01,   // 50 //
    0x00,   // 51 //
    0x35,   // 52 //
    0x4B,   // 53 //
    0x51,   // 54 //
    0x00,   // 55 //
    0x3F,   // 56 //
    0x3D,   // 57 //
    0x4A,   // 58 //
    0x3A,   // 59 //
    0x50,   // 60 //
    0x00,   // 61 //
    0x3F,   // 62 //
    0x3D,   // 63 //
    0x4E,   // 64 //
    0x4E,   // 65 //
    0x55,   // 66 //
    0x00,   // 67 //
    0x3D,   // 68 //
    0x4A,   // 69 //
    0x55,   // 70 //
    0x00,   // 71 //
    0x49,   // 72 //
    0x4B,   // 73 //
    0x4E,   // 74 //
    0x41,   // 75 //
    0x02,   // 76 //
    0x45,   // 77 //
    0x50,   // 78 //
    0x41,   // 79 //
    0x49,   // 80 //
    0x4F,   // 81 //
    0x79,   // 82 //
    0x01,   // 83 //
    0x00,   // 84 //
    0x35,   // 85 //
    0x4B,   // 86 //
    0x51,   // 87 //
    0x00,   // 88 //
    0x44,   // 89 //
    0x3D,   // 90 //
    0x52,   // 91 //
    0x41,   // 92 //
    0x00,   // 93 //
    0x51,   // 94 //
    0x4F,   // 95 //
    0x41,   // 96 //
    0x40,   // 97 //
    0x02,   // 98 //
    0x50,   // 99 //
    0x44,   // 100 //
    0x41,   // 101 //
    0x00,   // 102 //
    0x05,   // 103 //
    0x01,   // 104 //
    0x06,   // 105 //
    0x00,   // 106 //
    0x05,   // 107 //
    0x00,   // 108 //
    0x79,   // 109 //
    0x01,   // 110 //
    0x00,   // 111 //
    0x33,   // 112 //
    0x45,   // 113 //
    0x48,   // 114 //
    0x48,   // 115 //
    0x00,   // 116 //
    0x55,   // 117 //
    0x4B,   // 118 //
    0x51,   // 119 //
    0x00,   // 120 //
    0x4C,   // 121 //
    0x51,   // 122 //
    0x50,   // 123 //
    0x00,   // 124 //
    0x40,   // 125 //
    0x4B,   // 126 //
    0x53,   // 127 //
    0x4A,   // 128 //
    0x00,   // 129 //
    0x50,   // 130 //
    0x44,   // 131 //
    0x41,   // 132 //
    0x02,   // 133 //
    0x05,   // 134 //
    0x01,   // 135 //
    0x06,   // 136 //
    0x00,   // 137 //
    0x05,   // 138 //
    0x00,   // 139 //
    0x1B,   // 140 //
    0x08,   // 141 //
    0x02,   // 142 //
    0x0A,   // 143 //
    0x01,   // 144 //
    0x01,   // 145 //
    0x00,   // 146 //
    0x33,   // 147 //
    0x45,   // 148 //
    0x48,   // 149 //
    0x48,   // 150 //
    0x00,   // 151 //
    0x55,   // 152 //
    0x4B,   // 153 //
    0x51,   // 154 //
    0x00,   // 155 //
    0x51,   // 156 //
    0x4F,   // 157 //
    0x41,   // 158 //
    0x02,   // 159 //
    0x50,   // 160 //
    0x44,   // 161 //
    0x41,   // 162 //
    0x00,   // 163 //
    0x05,   // 164 //
    0x01,   // 165 //
    0x06,   // 166 //
    0x00,   // 167 //
    0x05,   // 168 //
    0x00,   // 169 //
    0x1B,   // 170 //
    0x08,   // 171 //
    0x02,   // 172 //
    0x0A,   // 173 //
    0x01,   // 174 //
    0x01,   // 175 //
    0x00,   // 176 //
    0x05,   // 177 //
    0x01,   // 178 //
    0x06,   // 179 //
    0x00,   // 180 //
    0x05,   // 181 //
    0x00,   // 182 //
    0x02,   // 183 //
    0x44,   // 184 //
    0x3D,   // 185 //
    0x4F,   // 186 //
    0x00,   // 187 //
    0x3E,   // 188 //
    0x41,   // 189 //
    0x41,   // 190 //
    0x4A,   // 191 //
    0x00,   // 192 //
    0x42,   // 193 //
    0x45,   // 194 //
    0x48,   // 195 //
    0x41,   // 196 //
    0x40,   // 197 //
    0x79,   // 198 //
    0x03,   // 199 //
    0x00,   // 200 //
    0x01,   // 201 //
    0x01,   // 202 //
    0x30,   // 203 //
    0x44,   // 204 //
    0x45,   // 205 //
    0x4F,   // 206 //
    0x00,   // 207 //
    0x47,   // 208 //
    0x41,   // 209 //
    0x55,   // 210 //
    0x00,   // 211 //
    0x45,   // 212 //
    0x4F,   // 213 //
    0x00,   // 214 //
    0x51,   // 215 //
    0x4F,   // 216 //
    0x41,   // 217 //
    0x48,   // 218 //
    0x41,   // 219 //
    0x4F,   // 220 //
    0x4F,   // 221 //
    0x00,   // 222 //
    0x4A,   // 223 //
    0x4B,   // 224 //
    0x53,   // 225 //
    0x79,   // 226 //
    0x02,   // 227 //
    0x20,   // 228 //
    0x45,   // 229 //
    0x4F,   // 230 //
    0x3F,   // 231 //
    0x3D,   // 232 //
    0x4E,   // 233 //
    0x40,   // 234 //
    0x1B,   // 235 //
    0x08,   // 236 //
    0x02,   // 237 //
    0x0A,   // 238 //
    0x02,   // 239 //
    0x01,   // 240 //
    0x00,   // 241 //
    0x02,   // 242 //
    0x25,   // 243 //
    0x50,   // 244 //
    0x3A,   // 245 //
    0x4F,   // 246 //
    0x00,   // 247 //
    0x48,   // 248 //
    0x4B,   // 249 //
    0x3F,   // 250 //
    0x47,   // 251 //
    0x41,   // 252 //
    0x40,   // 253 //
    0x79,   // 254 //
    0x03,   // 255 //
    0x00,   // 256 //
    0x02,   // 257 //
    0x1D,   // 258 //
    0x00,   // 259 //
    0x3F,   // 260 //
    0x3D,   // 261 //
    0x4E,   // 262 //
    0x52,   // 263 //
    0x45,   // 264 //
    0x4A,   // 265 //
    0x43,   // 266 //
    0x00,   // 267 //
    0x4B,   // 268 //
    0x42,   // 269 //
    0x00,   // 270 //
    0x3D,   // 271 //
    0x00,   // 272 //
    0x4F,   // 273 //
    0x53,   // 274 //
    0x4B,   // 275 //
    0x4E,   // 276 //
    0x40,   // 277 //
    0x79,   // 278 //
    0x01,   // 279 //
    0x00,   // 280 //
    0x02,   // 281 //
    0x25,   // 282 //
    0x50,   // 283 //
    0x3A,   // 284 //
    0x4F,   // 285 //
    0x00,   // 286 //
    0x48,   // 287 //
    0x4B,   // 288 //
    0x3F,   // 289 //
    0x47,   // 290 //
    0x41,   // 291 //
    0x40,   // 292 //
    0x79,   // 293 //
    0x03,   // 294 //
    0x00,   // 295 //
    0x02,   // 296 //
    0x1D,   // 297 //
    0x00,   // 298 //
    0x3F,   // 299 //
    0x3D,   // 300 //
    0x4E,   // 301 //
    0x52,   // 302 //
    0x45,   // 303 //
    0x4A,   // 304 //
    0x43,   // 305 //
    0x00,   // 306 //
    0x4B,   // 307 //
    0x42,   // 308 //
    0x00,   // 309 //
    0x3D,   // 310 //
    0x4A,   // 311 //
    0x00,   // 312 //
    0x3D,   // 313 //
    0x4E,   // 314 //
    0x49,   // 315 //
    0x4B,   // 316 //
    0x4E,   // 317 //
    0x79,   // 318 //
    0x01,   // 319 //
    0x00,   // 320 //
    0x02,   // 321 //
    0x25,   // 322 //
    0x50,   // 323 //
    0x3A,   // 324 //
    0x4F,   // 325 //
    0x00,   // 326 //
    0x48,   // 327 //
    0x4B,   // 328 //
    0x3F,   // 329 //
    0x47,   // 330 //
    0x41,   // 331 //
    0x40,   // 332 //
    0x79,   // 333 //
    0x03,   // 334 //
    0x00,   // 335 //
    0x02,   // 336 //
    0x1D,   // 337 //
    0x00,   // 338 //
    0x3F,   // 339 //
    0x3D,   // 340 //
    0x4E,   // 341 //
    0x52,   // 342 //
    0x45,   // 343 //
    0x4A,   // 344 //
    0x43,   // 345 //
    0x00,   // 346 //
    0x4B,   // 347 //
    0x42,   // 348 //
    0x00,   // 349 //
    0x3D,   // 350 //
    0x00,   // 351 //
    0x4F,   // 352 //
    0x44,   // 353 //
    0x45,   // 354 //
    0x41,   // 355 //
    0x48,   // 356 //
    0x40,   // 357 //
    0x79,   // 358 //
    0x01,   // 359 //
    0x00,   // 360 //
    0x02,   // 361 //
    0x25,   // 362 //
    0x50,   // 363 //
    0x3A,   // 364 //
    0x4F,   // 365 //
    0x00,   // 366 //
    0x48,   // 367 //
    0x4B,   // 368 //
    0x3F,   // 369 //
    0x47,   // 370 //
    0x41,   // 371 //
    0x40,   // 372 //
    0x79,   // 373 //
    0x03,   // 374 //
    0x00,   // 375 //
    0x02,   // 376 //
    0x1D,   // 377 //
    0x00,   // 378 //
    0x3F,   // 379 //
    0x3D,   // 380 //
    0x4E,   // 381 //
    0x52,   // 382 //
    0x45,   // 383 //
    0x4A,   // 384 //
    0x43,   // 385 //
    0x00,   // 386 //
    0x4B,   // 387 //
    0x42,   // 388 //
    0x00,   // 389 //
    0x3D,   // 390 //
    0x00,   // 391 //
    0x44,   // 392 //
    0x41,   // 393 //
    0x48,   // 394 //
    0x49,   // 395 //
    0x41,   // 396 //
    0x50,   // 397 //
    0x79,   // 398 //
    0x01,   // 399 //
    0x00,   // 400 //
    0x30,   // 401 //
    0x44,   // 402 //
    0x41,   // 403 //
    0x00,   // 404 //
    0x40,   // 405 //
    0x4B,   // 406 //
    0x4B,   // 407 //
    0x4E,   // 408 //
    0x00,   // 409 //
    0x45,   // 410 //
    0x4F,   // 411 //
    0x00,   // 412 //
    0x50,   // 413 //
    0x45,   // 414 //
    0x43,   // 415 //
    0x44,   // 416 //
    0x50,   // 417 //
    0x48,   // 418 //
    0x55,   // 419 //
    0x02,   // 420 //
    0x48,   // 421 //
    0x4B,   // 422 //
    0x3F,   // 423 //
    0x47,   // 424 //
    0x41,   // 425 //
    0x40,   // 426 //
    0x79,   // 427 //
    0x03,   // 428 //
    0x00,   // 429 //
    0x30,   // 430 //
    0x44,   // 431 //
    0x41,   // 432 //
    0x4E,   // 433 //
    0x41,   // 434 //
    0x3A,   // 435 //
    0x4F,   // 436 //
    0x00,   // 437 //
    0x3D,   // 438 //
    0x00,   // 439 //
    0x4C,   // 440 //
    0x48,   // 441 //
    0x3D,   // 442 //
    0x50,   // 443 //
    0x41,   // 444 //
    0x00,   // 445 //
    0x4B,   // 446 //
    0x4A,   // 447 //
    0x00,   // 448 //
    0x4E,   // 449 //
    0x45,   // 450 //
    0x43,   // 451 //
    0x44,   // 452 //
    0x50,   // 453 //
    0x02,   // 454 //
    0x44,   // 455 //
    0x3D,   // 456 //
    0x4A,   // 457 //
    0x40,   // 458 //
    0x00,   // 459 //
    0x4F,   // 460 //
    0x45,   // 461 //
    0x40,   // 462 //
    0x41,   // 463 //
    0x79,   // 464 //
    0x01,   // 465 //
    0x00,   // 466 //
    0x02,   // 467 //
    0x25,   // 468 //
    0x50,   // 469 //
    0x3A,   // 470 //
    0x4F,   // 471 //
    0x00,   // 472 //
    0x48,   // 473 //
    0x4B,   // 474 //
    0x3F,   // 475 //
    0x47,   // 476 //
    0x41,   // 477 //
    0x40,   // 478 //
    0x79,   // 479 //
    0x03,   // 480 //
    0x00,   // 481 //
    0x02,   // 482 //
    0x30,   // 483 //
    0x44,   // 484 //
    0x41,   // 485 //
    0x00,   // 486 //
    0x40,   // 487 //
    0x4B,   // 488 //
    0x4B,   // 489 //
    0x4E,   // 490 //
    0x00,   // 491 //
    0x4F,   // 492 //
    0x3D,   // 493 //
    0x55,   // 494 //
    0x4F,   // 495 //
    0x00,   // 496 //
    0x78,   // 497 //
    0x1F,   // 498 //
    0x48,   // 499 //
    0x4B,   // 500 //
    0x4F,   // 501 //
    0x41,   // 502 //
    0x50,   // 503 //
    0x19,   // 504 //
    0x79,   // 505 //
    0x01,   // 506 //
    0x00,   // 507 //
    0x02,   // 508 //
    0x25,   // 509 //
    0x50,   // 510 //
    0x3A,   // 511 //
    0x4F,   // 512 //
    0x00,   // 513 //
    0x48,   // 514 //
    0x4B,   // 515 //
    0x3F,   // 516 //
    0x47,   // 517 //
    0x41,   // 518 //
    0x40,   // 519 //
    0x79,   // 520 //
    0x03,   // 521 //
    0x00,   // 522 //
    0x02,   // 523 //
    0x30,   // 524 //
    0x44,   // 525 //
    0x41,   // 526 //
    0x00,   // 527 //
    0x4C,   // 528 //
    0x48,   // 529 //
    0x3D,   // 530 //
    0x50,   // 531 //
    0x41,   // 532 //
    0x00,   // 533 //
    0x4F,   // 534 //
    0x3D,   // 535 //
    0x55,   // 536 //
    0x4F,   // 537 //
    0x00,   // 538 //
    0x0C,   // 539 //
    0x0C,   // 540 //
    0x0E,   // 541 //
    0x79,   // 542 //
    0x01,   // 543 //
    0x00,   // 544 //
    0x02,   // 545 //
    0x25,   // 546 //
    0x50,   // 547 //
    0x3A,   // 548 //
    0x4F,   // 549 //
    0x00,   // 550 //
    0x48,   // 551 //
    0x4B,   // 552 //
    0x3F,   // 553 //
    0x47,   // 554 //
    0x41,   // 555 //
    0x40,   // 556 //
    0x79,   // 557 //
    0x03,   // 558 //
    0x00,   // 559 //
    0x02,   // 560 //
    0x30,   // 561 //
    0x44,   // 562 //
    0x41,   // 563 //
    0x00,   // 564 //
    0x4C,   // 565 //
    0x48,   // 566 //
    0x3D,   // 567 //
    0x50,   // 568 //
    0x41,   // 569 //
    0x00,   // 570 //
    0x4F,   // 571 //
    0x3D,   // 572 //
    0x55,   // 573 //
    0x4F,   // 574 //
    0x00,   // 575 //
    0x0C,   // 576 //
    0x0C,   // 577 //
    0x0F,   // 578 //
    0x79,   // 579 //
    0x01,   // 580 //
    0x00,   // 581 //
    0x02,   // 582 //
    0x25,   // 583 //
    0x50,   // 584 //
    0x3A,   // 585 //
    0x4F,   // 586 //
    0x00,   // 587 //
    0x48,   // 588 //
    0x4B,   // 589 //
    0x3F,   // 590 //
    0x47,   // 591 //
    0x41,   // 592 //
    0x40,   // 593 //
    0x79,   // 594 //
    0x03,   // 595 //
    0x00,   // 596 //
    0x30,   // 597 //
    0x44,   // 598 //
    0x41,   // 599 //
    0x00,   // 600 //
    0x40,   // 601 //
    0x4B,   // 602 //
    0x4B,   // 603 //
    0x4E,   // 604 //
    0x00,   // 605 //
    0x4F,   // 606 //
    0x3D,   // 607 //
    0x55,   // 608 //
    0x4F,   // 609 //
    0x02,   // 610 //
    0x78,   // 611 //
    0x1F,   // 612 //
    0x4B,   // 613 //
    0x4A,   // 614 //
    0x50,   // 615 //
    0x4E,   // 616 //
    0x4B,   // 617 //
    0x48,   // 618 //
    0x00,   // 619 //
    0x2E,   // 620 //
    0x4B,   // 621 //
    0x4B,   // 622 //
    0x49,   // 623 //
    0x19,   // 624 //
    0x79,   // 625 //
    0x01,   // 626 //
    0x00,   // 627 //
    0x02,   // 628 //
    0x78,   // 629 //
    0x2C,   // 630 //
    0x4B,   // 631 //
    0x53,   // 632 //
    0x41,   // 633 //
    0x4E,   // 634 //
    0x00,   // 635 //
    0x2E,   // 636 //
    0x4B,   // 637 //
    0x4B,   // 638 //
    0x49,   // 639 //
    0x19,   // 640 //
    0x03,   // 641 //
    0x00,   // 642 //
    0x30,   // 643 //
    0x44,   // 644 //
    0x41,   // 645 //
    0x00,   // 646 //
    0x40,   // 647 //
    0x4B,   // 648 //
    0x4B,   // 649 //
    0x4E,   // 650 //
    0x00,   // 651 //
    0x45,   // 652 //
    0x4F,   // 653 //
    0x00,   // 654 //
    0x50,   // 655 //
    0x45,   // 656 //
    0x43,   // 657 //
    0x44,   // 658 //
    0x50,   // 659 //
    0x48,   // 660 //
    0x55,   // 661 //
    0x02,   // 662 //
    0x48,   // 663 //
    0x4B,   // 664 //
    0x3F,   // 665 //
    0x47,   // 666 //
    0x41,   // 667 //
    0x40,   // 668 //
    0x79,   // 669 //
    0x01,   // 670 //
    0x00,   // 671 //
    0x02,   // 672 //
    0x25,   // 673 //
    0x50,   // 674 //
    0x3A,   // 675 //
    0x4F,   // 676 //
    0x00,   // 677 //
    0x48,   // 678 //
    0x4B,   // 679 //
    0x3F,   // 680 //
    0x47,   // 681 //
    0x41,   // 682 //
    0x40,   // 683 //
    0x79,   // 684 //
    0x01,   // 685 //
    0x00,   // 686 //
    0x02,   // 687 //
    0x25,   // 688 //
    0x50,   // 689 //
    0x3A,   // 690 //
    0x4F,   // 691 //
    0x00,   // 692 //
    0x48,   // 693 //
    0x4B,   // 694 //
    0x3F,   // 695 //
    0x47,   // 696 //
    0x41,   // 697 //
    0x40,   // 698 //
    0x00,   // 699 //
    0x42,   // 700 //
    0x4E,   // 701 //
    0x4B,   // 702 //
    0x49,   // 703 //
    0x00,   // 704 //
    0x45,   // 705 //
    0x4A,   // 706 //
    0x4F,   // 707 //
    0x45,   // 708 //
    0x40,   // 709 //
    0x41,   // 710 //
    0x79,   // 711 //
    0x01,   // 712 //
    0x00,   // 713 //
    0x02,   // 714 //
    0x35,   // 715 //
    0x4B,   // 716 //
    0x51,   // 717 //
    0x00,   // 718 //
    0x51,   // 719 //
    0x4A,   // 720 //
    0x48,   // 721 //
    0x4B,   // 722 //
    0x3F,   // 723 //
    0x47,   // 724 //
    0x41,   // 725 //
    0x40,   // 726 //
    0x00,   // 727 //
    0x45,   // 728 //
    0x50,   // 729 //
    0x79,   // 730 //
    0x01,   // 731 //
    0x00,   // 732 //
    0x02,   // 733 //
    0x25,   // 734 //
    0x50,   // 735 //
    0x3A,   // 736 //
    0x4F,   // 737 //
    0x00,   // 738 //
    0x48,   // 739 //
    0x4B,   // 740 //
    0x3F,   // 741 //
    0x47,   // 742 //
    0x41,   // 743 //
    0x40,   // 744 //
    0x79,   // 745 //
    0x03,   // 746 //
    0x00,   // 747 //
    0x31,   // 748 //
    0x4F,   // 749 //
    0x41,   // 750 //
    0x00,   // 751 //
    0x50,   // 752 //
    0x44,   // 753 //
    0x41,   // 754 //
    0x00,   // 755 //
    0x48,   // 756 //
    0x4B,   // 757 //
    0x3F,   // 758 //
    0x47,   // 759 //
    0x4C,   // 760 //
    0x45,   // 761 //
    0x3F,   // 762 //
    0x47,   // 763 //
    0x00,   // 764 //
    0x50,   // 765 //
    0x4B,   // 766 //
    0x00,   // 767 //
    0x4B,   // 768 //
    0x4C,   // 769 //
    0x41,   // 770 //
    0x4A,   // 771 //
    0x02,   // 772 //
    0x50,   // 773 //
    0x44,   // 774 //
    0x41,   // 775 //
    0x00,   // 776 //
    0x40,   // 777 //
    0x4B,   // 778 //
    0x4B,   // 779 //
    0x4E,   // 780 //
    0x79,   // 781 //
    0x01,   // 782 //
    0x00,   // 783 //
    0x02,   // 784 //
    0x25,   // 785 //
    0x3A,   // 786 //
    0x52,   // 787 //
    0x41,   // 788 //
    0x00,   // 789 //
    0x43,   // 790 //
    0x4B,   // 791 //
    0x50,   // 792 //
    0x00,   // 793 //
    0x50,   // 794 //
    0x4B,   // 795 //
    0x00,   // 796 //
    0x44,   // 797 //
    0x51,   // 798 //
    0x4E,   // 799 //
    0x4E,   // 800 //
    0x55,   // 801 //
    0x1A,   // 802 //
    0x01,   // 803 //
    0x00,   // 804 //
    0x30,   // 805 //
    0x44,   // 806 //
    0x41,   // 807 //
    0x4E,   // 808 //
    0x41,   // 809 //
    0x00,   // 810 //
    0x45,   // 811 //
    0x4F,   // 812 //
    0x00,   // 813 //
    0x4A,   // 814 //
    0x4B,   // 815 //
    0x00,   // 816 //
    0x50,   // 817 //
    0x45,   // 818 //
    0x49,   // 819 //
    0x41,   // 820 //
    0x00,   // 821 //
    0x50,   // 822 //
    0x4B,   // 823 //
    0x00,   // 824 //
    0x3F,   // 825 //
    0x44,   // 826 //
    0x41,   // 827 //
    0x3F,   // 828 //
    0x47,   // 829 //
    0x02,   // 830 //
    0x45,   // 831 //
    0x50,   // 832 //
    0x79,   // 833 //
    0x01,   // 834 //
    0x00,   // 835 //
    0x02,   // 836 //
    0x30,   // 837 //
    0x44,   // 838 //
    0x41,   // 839 //
    0x00,   // 840 //
    0x40,   // 841 //
    0x41,   // 842 //
    0x4F,   // 843 //
    0x47,   // 844 //
    0x00,   // 845 //
    0x45,   // 846 //
    0x4F,   // 847 //
    0x00,   // 848 //
    0x48,   // 849 //
    0x4B,   // 850 //
    0x3F,   // 851 //
    0x47,   // 852 //
    0x41,   // 853 //
    0x40,   // 854 //
    0x79,   // 855 //
    0x01,   // 856 //
    0x00,   // 857 //
    0x02,   // 858 //
    0x30,   // 859 //
    0x44,   // 860 //
    0x41,   // 861 //
    0x00,   // 862 //
    0x40,   // 863 //
    0x41,   // 864 //
    0x4F,   // 865 //
    0x47,   // 866 //
    0x00,   // 867 //
    0x45,   // 868 //
    0x4F,   // 869 //
    0x00,   // 870 //
    0x48,   // 871 //
    0x4B,   // 872 //
    0x3F,   // 873 //
    0x47,   // 874 //
    0x41,   // 875 //
    0x40,   // 876 //
    0x79,   // 877 //
    0x03,   // 878 //
    0x00,   // 879 //
    0x33,   // 880 //
    0x45,   // 881 //
    0x48,   // 882 //
    0x48,   // 883 //
    0x00,   // 884 //
    0x55,   // 885 //
    0x4B,   // 886 //
    0x51,   // 887 //
    0x00,   // 888 //
    0x51,   // 889 //
    0x4F,   // 890 //
    0x41,   // 891 //
    0x02,   // 892 //
    0x50,   // 893 //
    0x44,   // 894 //
    0x41,   // 895 //
    0x00,   // 896 //
    0x05,   // 897 //
    0x01,   // 898 //
    0x06,   // 899 //
    0x00,   // 900 //
    0x05,   // 901 //
    0x00,   // 902 //
    0x1B,   // 903 //
    0x08,   // 904 //
    0x02,   // 905 //
    0x0A,   // 906 //
    0x01,   // 907 //
    0x01,   // 908 //
    0x00,   // 909 //
    0x02,   // 910 //
    0x04,   // 911 //
    0x00,   // 912 //
    0x00,   // 913 //
    0x00,   // 914 //
    0x00,   // 915 //
    0x00,   // 916 //
    0x00,   // 917 //
    0x00,   // 918 //
    0x00,   // 919 //
    0x00,   // 920 //
    0x00,   // 921 //
    0x00,   // 922 //
    0x1F,   // 923 //
    0x44,   // 924 //
    0x4E,   // 925 //
    0x45,   // 926 //
    0x4F,   // 927 //
    0x79,   // 928 //
    0x79,   // 929 //
    0x79,   // 930 //
    0x04,   // 931 //
    0x10,   // 932 //
    0x03,   // 933 //
    0x30,   // 934 //
    0x02,   // 935 //
    0x00,   // 936 //
    0x04,   // 937 //
    0x00,   // 938 //
    0x00,   // 939 //
    0x00,   // 940 //
    0x00,   // 941 //
    0x00,   // 942 //
    0x00,   // 943 //
    0x00,   // 944 //
    0x00,   // 945 //
    0x00,   // 946 //
    0x00,   // 947 //
    0x1F,   // 948 //
    0x44,   // 949 //
    0x4E,   // 950 //
    0x45,   // 951 //
    0x4F,   // 952 //
    0x79,   // 953 //
    0x79,   // 954 //
    0x79,   // 955 //
    0x04,   // 956 //
    0x02,   // 957 //
    0x01,   // 958 //
    0x30,   // 959 //
    0x04,   // 960 //
    0x02,   // 961 //
    0x30,   // 962 //
    0x44,   // 963 //
    0x41,   // 964 //
    0x55,   // 965 //
    0x00,   // 966 //
    0x44,   // 967 //
    0x3D,   // 968 //
    0x52,   // 969 //
    0x41,   // 970 //
    0x00,   // 971 //
    0x41,   // 972 //
    0x4F,   // 973 //
    0x3F,   // 974 //
    0x3D,   // 975 //
    0x4C,   // 976 //
    0x41,   // 977 //
    0x40,   // 978 //
    0x02,   // 979 //
    0x45,   // 980 //
    0x4A,   // 981 //
    0x50,   // 982 //
    0x4B,   // 983 //
    0x00,   // 984 //
    0x50,   // 985 //
    0x44,   // 986 //
    0x41,   // 987 //
    0x00,   // 988 //
    0x49,   // 989 //
    0x3D,   // 990 //
    0x4A,   // 991 //
    0x4F,   // 992 //
    0x45,   // 993 //
    0x4B,   // 994 //
    0x4A,   // 995 //
    0x03,   // 996 //
    0x30,   // 997 //
    0x53,   // 998 //
    0x44,   // 999 //
    0x41,   // 1000 //
    0x4E,   // 1001 //
    0x41,   // 1002 //
    0x00,   // 1003 //
    0x50,   // 1004 //
    0x44,   // 1005 //
    0x41,   // 1006 //
    0x55,   // 1007 //
    0x00,   // 1008 //
    0x50,   // 1009 //
    0x44,   // 1010 //
    0x4B,   // 1011 //
    0x51,   // 1012 //
    0x43,   // 1013 //
    0x44,   // 1014 //
    0x50,   // 1015 //
    0x02,   // 1016 //
    0x45,   // 1017 //
    0x50,   // 1018 //
    0x00,   // 1019 //
    0x53,   // 1020 //
    0x3D,   // 1021 //
    0x4F,   // 1022 //
    0x00,   // 1023 //
    0x4F,   // 1024 //
    0x3D,   // 1025 //
    0x42,   // 1026 //
    0x41,   // 1027 //
    0x79,   // 1028 //
    0x03,   // 1029 //
    0x30,   // 1030 //
    0x35,   // 1031 //
    0x41,   // 1032 //
    0x50,   // 1033 //
    0x3C,   // 1034 //
    0x3C,   // 1035 //
    0x3C,   // 1036 //
    0x01,   // 1037 //
    0x30,   // 1038 //
    0x04,   // 1039 //
    0x02,   // 1040 //
    0x35,   // 1041 //
    0x4B,   // 1042 //
    0x51,   // 1043 //
    0x00,   // 1044 //
    0x44,   // 1045 //
    0x3D,   // 1046 //
    0x52,   // 1047 //
    0x41,   // 1048 //
    0x00,   // 1049 //
    0x4B,   // 1050 //
    0x4A,   // 1051 //
    0x3F,   // 1052 //
    0x41,   // 1053 //
    0x02,   // 1054 //
    0x04,   // 1055 //
    0x02,   // 1056 //
    0x3D,   // 1057 //
    0x43,   // 1058 //
    0x3D,   // 1059 //
    0x45,   // 1060 //
    0x4A,   // 1061 //
    0x00,   // 1062 //
    0x41,   // 1063 //
    0x4A,   // 1064 //
    0x50,   // 1065 //
    0x41,   // 1066 //
    0x4E,   // 1067 //
    0x41,   // 1068 //
    0x40,   // 1069 //
    0x03,   // 1070 //
    0x30,   // 1071 //
    0x04,   // 1072 //
    0x02,   // 1073 //
    0x50,   // 1074 //
    0x44,   // 1075 //
    0x41,   // 1076 //
    0x00,   // 1077 //
    0x53,   // 1078 //
    0x4B,   // 1079 //
    0x4E,   // 1080 //
    0x48,   // 1081 //
    0x40,   // 1082 //
    0x00,   // 1083 //
    0x4B,   // 1084 //
    0x42,   // 1085 //
    0x02,   // 1086 //
    0x04,   // 1087 //
    0x02,   // 1088 //
    0x4F,   // 1089 //
    0x51,   // 1090 //
    0x4E,   // 1091 //
    0x52,   // 1092 //
    0x45,   // 1093 //
    0x52,   // 1094 //
    0x3D,   // 1095 //
    0x48,   // 1096 //
    0x00,   // 1097 //
    0x44,   // 1098 //
    0x4B,   // 1099 //
    0x4E,   // 1100 //
    0x4E,   // 1101 //
    0x4B,   // 1102 //
    0x4E,   // 1103 //
    0x79,   // 1104 //
    0x03,   // 1105 //
    0x30,   // 1106 //
    0x04,   // 1107 //
    0x02,   // 1108 //
    0x23,   // 1109 //
    0x4B,   // 1110 //
    0x4B,   // 1111 //
    0x40,   // 1112 //
    0x00,   // 1113 //
    0x48,   // 1114 //
    0x51,   // 1115 //
    0x3F,   // 1116 //
    0x47,   // 1117 //
    0x1A,   // 1118 //
    0x01,   // 1119 //
    0x30,   // 1120 //
    0x04,   // 1121 //
    0x02,   // 1122 //
    0x1E,   // 1123 //
    0x41,   // 1124 //
    0x00,   // 1125 //
    0x4F,   // 1126 //
    0x49,   // 1127 //
    0x3D,   // 1128 //
    0x4E,   // 1129 //
    0x50,   // 1130 //
    0x1A,   // 1131 //
    0x02,   // 1132 //
    0x04,   // 1133 //
    0x02,   // 1134 //
    0x22,   // 1135 //
    0x45,   // 1136 //
    0x43,   // 1137 //
    0x44,   // 1138 //
    0x50,   // 1139 //
    0x45,   // 1140 //
    0x4A,   // 1141 //
    0x43,   // 1142 //
    0x00,   // 1143 //
    0x42,   // 1144 //
    0x4B,   // 1145 //
    0x41,   // 1146 //
    0x4F,   // 1147 //
    0x03,   // 1148 //
    0x30,   // 1149 //
    0x04,   // 1150 //
    0x02,   // 1151 //
    0x45,   // 1152 //
    0x4F,   // 1153 //
    0x4A,   // 1154 //
    0x3A,   // 1155 //
    0x50,   // 1156 //
    0x00,   // 1157 //
    0x50,   // 1158 //
    0x44,   // 1159 //
    0x41,   // 1160 //
    0x00,   // 1161 //
    0x4B,   // 1162 //
    0x4A,   // 1163 //
    0x48,   // 1164 //
    0x55,   // 1165 //
    0x02,   // 1166 //
    0x04,   // 1167 //
    0x02,   // 1168 //
    0x53,   // 1169 //
    0x3D,   // 1170 //
    0x55,   // 1171 //
    0x00,   // 1172 //
    0x50,   // 1173 //
    0x4B,   // 1174 //
    0x00,   // 1175 //
    0x4F,   // 1176 //
    0x51,   // 1177 //
    0x4E,   // 1178 //
    0x52,   // 1179 //
    0x45,   // 1180 //
    0x52,   // 1181 //
    0x41,   // 1182 //
    0x03,   // 1183 //
    0x30,   // 1184 //
    0x04,   // 1185 //
    0x02,   // 1186 //
    0x50,   // 1187 //
    0x44,   // 1188 //
    0x45,   // 1189 //
    0x4F,   // 1190 //
    0x00,   // 1191 //
    0x44,   // 1192 //
    0x4B,   // 1193 //
    0x4E,   // 1194 //
    0x4E,   // 1195 //
    0x4B,   // 1196 //
    0x4E,   // 1197 //
    0x79,   // 1198 //
    0x01,   // 1199 //
    0x40,   // 1200 //
    0x02,   // 1201 //
    0x25,   // 1202 //
    0x50,   // 1203 //
    0x3A,   // 1204 //
    0x4F,   // 1205 //
    0x00,   // 1206 //
    0x3D,   // 1207 //
    0x4A,   // 1208 //
    0x00,   // 1209 //
    0x4B,   // 1210 //
    0x48,   // 1211 //
    0x40,   // 1212 //
    0x00,   // 1213 //
    0x50,   // 1214 //
    0x55,   // 1215 //
    0x4C,   // 1216 //
    0x41,   // 1217 //
    0x53,   // 1218 //
    0x4E,   // 1219 //
    0x45,   // 1220 //
    0x50,   // 1221 //
    0x41,   // 1222 //
    0x4E,   // 1223 //
    0x79,   // 1224 //
    0x03,   // 1225 //
    0x00,   // 1226 //
    0x25,   // 1227 //
    0x42,   // 1228 //
    0x00,   // 1229 //
    0x25,   // 1230 //
    0x00,   // 1231 //
    0x44,   // 1232 //
    0x3D,   // 1233 //
    0x40,   // 1234 //
    0x00,   // 1235 //
    0x3D,   // 1236 //
    0x4A,   // 1237 //
    0x00,   // 1238 //
    0x05,   // 1239 //
    0x01,   // 1240 //
    0x25,   // 1241 //
    0x2A,   // 1242 //
    0x27,   // 1243 //
    0x00,   // 1244 //
    0x2E,   // 1245 //
    0x25,   // 1246 //
    0x1E,   // 1247 //
    0x1E,   // 1248 //
    0x2B,   // 1249 //
    0x2A,   // 1250 //
    0x05,   // 1251 //
    0x00,   // 1252 //
    0x18,   // 1253 //
    0x00,   // 1254 //
    0x25,   // 1255 //
    0x02,   // 1256 //
    0x3F,   // 1257 //
    0x4B,   // 1258 //
    0x51,   // 1259 //
    0x48,   // 1260 //
    0x40,   // 1261 //
    0x00,   // 1262 //
    0x4F,   // 1263 //
    0x3D,   // 1264 //
    0x52,   // 1265 //
    0x41,   // 1266 //
    0x00,   // 1267 //
    0x49,   // 1268 //
    0x55,   // 1269 //
    0x00,   // 1270 //
    0x4C,   // 1271 //
    0x4E,   // 1272 //
    0x4B,   // 1273 //
    0x43,   // 1274 //
    0x4E,   // 1275 //
    0x41,   // 1276 //
    0x4F,   // 1277 //
    0x4F,   // 1278 //
    0x79,   // 1279 //
    0x79,   // 1280 //
    0x79,   // 1281 //
    0x01,   // 1282 //
    0x00,   // 1283 //
    0x35,   // 1284 //
    0x4B,   // 1285 //
    0x51,   // 1286 //
    0x00,   // 1287 //
    0x3F,   // 1288 //
    0x3D,   // 1289 //
    0x4A,   // 1290 //
    0x00,   // 1291 //
    0x4F,   // 1292 //
    0x3D,   // 1293 //
    0x52,   // 1294 //
    0x41,   // 1295 //
    0x00,   // 1296 //
    0x55,   // 1297 //
    0x4B,   // 1298 //
    0x51,   // 1299 //
    0x4E,   // 1300 //
    0x00,   // 1301 //
    0x4C,   // 1302 //
    0x4E,   // 1303 //
    0x4B,   // 1304 //
    0x43,   // 1305 //
    0x4E,   // 1306 //
    0x41,   // 1307 //
    0x4F,   // 1308 //
    0x4F,   // 1309 //
    0x02,   // 1310 //
    0x53,   // 1311 //
    0x45,   // 1312 //
    0x50,   // 1313 //
    0x44,   // 1314 //
    0x00,   // 1315 //
    0x50,   // 1316 //
    0x44,   // 1317 //
    0x45,   // 1318 //
    0x4F,   // 1319 //
    0x79,   // 1320 //
    0x03,   // 1321 //
    0x00,   // 1322 //
    0x33,   // 1323 //
    0x45,   // 1324 //
    0x48,   // 1325 //
    0x48,   // 1326 //
    0x00,   // 1327 //
    0x55,   // 1328 //
    0x4B,   // 1329 //
    0x51,   // 1330 //
    0x00,   // 1331 //
    0x51,   // 1332 //
    0x4F,   // 1333 //
    0x41,   // 1334 //
    0x02,   // 1335 //
    0x50,   // 1336 //
    0x44,   // 1337 //
    0x41,   // 1338 //
    0x00,   // 1339 //
    0x05,   // 1340 //
    0x01,   // 1341 //
    0x25,   // 1342 //
    0x2A,   // 1343 //
    0x27,   // 1344 //
    0x00,   // 1345 //
    0x2E,   // 1346 //
    0x25,   // 1347 //
    0x1E,   // 1348 //
    0x1E,   // 1349 //
    0x2B,   // 1350 //
    0x2A,   // 1351 //
    0x05,   // 1352 //
    0x00,   // 1353 //
    0x1B,   // 1354 //
    0x08,   // 1355 //
    0x00,   // 1356 //
    0x01,   // 1357 //
    0x00,   // 1358 //
    0x30,   // 1359 //
    0x44,   // 1360 //
    0x41,   // 1361 //
    0x00,   // 1362 //
    0x4A,   // 1363 //
    0x51,   // 1364 //
    0x49,   // 1365 //
    0x3E,   // 1366 //
    0x41,   // 1367 //
    0x4E,   // 1368 //
    0x00,   // 1369 //
    0x0C,   // 1370 //
    0x0C,   // 1371 //
    0x0E,   // 1372 //
    0x00,   // 1373 //
    0x45,   // 1374 //
    0x4F,   // 1375 //
    0x00,   // 1376 //
    0x41,   // 1377 //
    0x4A,   // 1378 //
    0x43,   // 1379 //
    0x4E,   // 1380 //
    0x3D,   // 1381 //
    0x52,   // 1382 //
    0x41,   // 1383 //
    0x40,   // 1384 //
    0x79,   // 1385 //
    0x01,   // 1386 //
    0x00,   // 1387 //
    0x30,   // 1388 //
    0x44,   // 1389 //
    0x41,   // 1390 //
    0x00,   // 1391 //
    0x4A,   // 1392 //
    0x51,   // 1393 //
    0x49,   // 1394 //
    0x3E,   // 1395 //
    0x41,   // 1396 //
    0x4E,   // 1397 //
    0x00,   // 1398 //
    0x0C,   // 1399 //
    0x0C,   // 1400 //
    0x0F,   // 1401 //
    0x00,   // 1402 //
    0x45,   // 1403 //
    0x4F,   // 1404 //
    0x00,   // 1405 //
    0x41,   // 1406 //
    0x4A,   // 1407 //
    0x43,   // 1408 //
    0x4E,   // 1409 //
    0x3D,   // 1410 //
    0x52,   // 1411 //
    0x41,   // 1412 //
    0x40,   // 1413 //
    0x79,   // 1414 //
    0x01,   // 1415 //
    0x00,   // 1416 //
    0x1D,   // 1417 //
    0x00,   // 1418 //
    0x40,   // 1419 //
    0x41,   // 1420 //
    0x4F,   // 1421 //
    0x47,   // 1422 //
    0x00,   // 1423 //
    0x47,   // 1424 //
    0x41,   // 1425 //
    0x55,   // 1426 //
    0x79,   // 1427 //
    0x01,   // 1428 //
    0x00,   // 1429 //
    0x1D,   // 1430 //
    0x00,   // 1431 //
    0x4F,   // 1432 //
    0x50,   // 1433 //
    0x4E,   // 1434 //
    0x3D,   // 1435 //
    0x4A,   // 1436 //
    0x43,   // 1437 //
    0x41,   // 1438 //
    0x00,   // 1439 //
    0x49,   // 1440 //
    0x3D,   // 1441 //
    0x4E,   // 1442 //
    0x47,   // 1443 //
    0x00,   // 1444 //
    0x45,   // 1445 //
    0x4F,   // 1446 //
    0x00,   // 1447 //
    0x3F,   // 1448 //
    0x3D,   // 1449 //
    0x4E,   // 1450 //
    0x52,   // 1451 //
    0x41,   // 1452 //
    0x40,   // 1453 //
    0x02,   // 1454 //
    0x44,   // 1455 //
    0x41,   // 1456 //
    0x4E,   // 1457 //
    0x41,   // 1458 //
    0x79,   // 1459 //
    0x01,   // 1460 //
    0x00,   // 1461 //
    0x25,   // 1462 //
    0x50,   // 1463 //
    0x3A,   // 1464 //
    0x4F,   // 1465 //
    0x00,   // 1466 //
    0x4F,   // 1467 //
    0x4C,   // 1468 //
    0x41,   // 1469 //
    0x3F,   // 1470 //
    0x45,   // 1471 //
    0x3D,   // 1472 //
    0x48,   // 1473 //
    0x48,   // 1474 //
    0x55,   // 1475 //
    0x00,   // 1476 //
    0x3F,   // 1477 //
    0x4B,   // 1478 //
    0x3D,   // 1479 //
    0x50,   // 1480 //
    0x41,   // 1481 //
    0x40,   // 1482 //
    0x00,   // 1483 //
    0x3D,   // 1484 //
    0x4A,   // 1485 //
    0x40,   // 1486 //
    0x02,   // 1487 //
    0x48,   // 1488 //
    0x4B,   // 1489 //
    0x4B,   // 1490 //
    0x47,   // 1491 //
    0x4F,   // 1492 //
    0x00,   // 1493 //
    0x45,   // 1494 //
    0x49,   // 1495 //
    0x4C,   // 1496 //
    0x4B,   // 1497 //
    0x4E,   // 1498 //
    0x50,   // 1499 //
    0x3D,   // 1500 //
    0x4A,   // 1501 //
    0x50,   // 1502 //
    0x79,   // 1503 //
    0x79,   // 1504 //
    0x79,   // 1505 //
    0x01,   // 1506 //
    0x00,   // 1507 //
    0x30,   // 1508 //
    0x44,   // 1509 //
    0x41,   // 1510 //
    0x4E,   // 1511 //
    0x41,   // 1512 //
    0x00,   // 1513 //
    0x49,   // 1514 //
    0x51,   // 1515 //
    0x4F,   // 1516 //
    0x50,   // 1517 //
    0x00,   // 1518 //
    0x3E,   // 1519 //
    0x41,   // 1520 //
    0x00,   // 1521 //
    0x3D,   // 1522 //
    0x00,   // 1523 //
    0x3F,   // 1524 //
    0x48,   // 1525 //
    0x4B,   // 1526 //
    0x4F,   // 1527 //
    0x41,   // 1528 //
    0x50,   // 1529 //
    0x02,   // 1530 //
    0x4F,   // 1531 //
    0x4B,   // 1532 //
    0x49,   // 1533 //
    0x41,   // 1534 //
    0x53,   // 1535 //
    0x44,   // 1536 //
    0x41,   // 1537 //
    0x4E,   // 1538 //
    0x41,   // 1539 //
    0x79,   // 1540 //
    0x01,   // 1541 //
    0x00,   // 1542 //
    0x25,   // 1543 //
    0x50,   // 1544 //
    0x4F,   // 1545 //
    0x00,   // 1546 //
    0x41,   // 1547 //
    0x4A,   // 1548 //
    0x40,   // 1549 //
    0x00,   // 1550 //
    0x45,   // 1551 //
    0x4F,   // 1552 //
    0x00,   // 1553 //
    0x4F,   // 1554 //
    0x4D,   // 1555 //
    0x51,   // 1556 //
    0x3D,   // 1557 //
    0x4E,   // 1558 //
    0x41,   // 1559 //
    0x3B,   // 1560 //
    0x4F,   // 1561 //
    0x44,   // 1562 //
    0x3D,   // 1563 //
    0x4C,   // 1564 //
    0x41,   // 1565 //
    0x40,   // 1566 //
    0x79,   // 1567 //
    0x01,   // 1568 //
    0x00,   // 1569 //
    0x25,   // 1570 //
    0x50,   // 1571 //
    0x4F,   // 1572 //
    0x00,   // 1573 //
    0x41,   // 1574 //
    0x4A,   // 1575 //
    0x40,   // 1576 //
    0x00,   // 1577 //
    0x45,   // 1578 //
    0x4F,   // 1579 //
    0x00,   // 1580 //
    0x44,   // 1581 //
    0x41,   // 1582 //
    0x54,   // 1583 //
    0x79,   // 1584 //
    0x3B,   // 1585 //
    0x4F,   // 1586 //
    0x44,   // 1587 //
    0x3D,   // 1588 //
    0x4C,   // 1589 //
    0x41,   // 1590 //
    0x40,   // 1591 //
    0x79,   // 1592 //
    0x01,   // 1593 //
    0x00,   // 1594 //
    0x1D,   // 1595 //
    0x48,   // 1596 //
    0x48,   // 1597 //
    0x00,   // 1598 //
    0x4C,   // 1599 //
    0x3D,   // 1600 //
    0x43,   // 1601 //
    0x41,   // 1602 //
    0x4F,   // 1603 //
    0x00,   // 1604 //
    0x3D,   // 1605 //
    0x4E,   // 1606 //
    0x41,   // 1607 //
    0x00,   // 1608 //
    0x3E,   // 1609 //
    0x48,   // 1610 //
    0x3D,   // 1611 //
    0x4A,   // 1612 //
    0x47,   // 1613 //
    0x79,   // 1614 //
    0x02,   // 1615 //
    0x33,   // 1616 //
    0x44,   // 1617 //
    0x3D,   // 1618 //
    0x50,   // 1619 //
    0x3A,   // 1620 //
    0x4F,   // 1621 //
    0x00,   // 1622 //
    0x45,   // 1623 //
    0x50,   // 1624 //
    0x00,   // 1625 //
    0x42,   // 1626 //
    0x4B,   // 1627 //
    0x4E,   // 1628 //
    0x79,   // 1629 //
    0x79,   // 1630 //
    0x79,   // 1631 //
    0x1B,   // 1632 //
    0x01,   // 1633 //
    0x00,   // 1634 //
    0x30,   // 1635 //
    0x44,   // 1636 //
    0x41,   // 1637 //
    0x4E,   // 1638 //
    0x41,   // 1639 //
    0x00,   // 1640 //
    0x53,   // 1641 //
    0x3D,   // 1642 //
    0x4F,   // 1643 //
    0x00,   // 1644 //
    0x3D,   // 1645 //
    0x00,   // 1646 //
    0x49,   // 1647 //
    0x41,   // 1648 //
    0x40,   // 1649 //
    0x3D,   // 1650 //
    0x48,   // 1651 //
    0x00,   // 1652 //
    0x45,   // 1653 //
    0x4A,   // 1654 //
    0x00,   // 1655 //
    0x50,   // 1656 //
    0x44,   // 1657 //
    0x41,   // 1658 //
    0x02,   // 1659 //
    0x3E,   // 1660 //
    0x4B,   // 1661 //
    0x4B,   // 1662 //
    0x47,   // 1663 //
    0x79,   // 1664 //
    0x01,   // 1665 //
    0x00,   // 1666 //
    0x1D,   // 1667 //
    0x00,   // 1668 //
    0x3F,   // 1669 //
    0x44,   // 1670 //
    0x41,   // 1671 //
    0x49,   // 1672 //
    0x45,   // 1673 //
    0x3F,   // 1674 //
    0x3D,   // 1675 //
    0x48,   // 1676 //
    0x00,   // 1677 //
    0x50,   // 1678 //
    0x4B,   // 1679 //
    0x00,   // 1680 //
    0x47,   // 1681 //
    0x45,   // 1682 //
    0x48,   // 1683 //
    0x48,   // 1684 //
    0x00,   // 1685 //
    0x50,   // 1686 //
    0x44,   // 1687 //
    0x41,   // 1688 //
    0x02,   // 1689 //
    0x53,   // 1690 //
    0x41,   // 1691 //
    0x41,   // 1692 //
    0x40,   // 1693 //
    0x4F,   // 1694 //
    0x79,   // 1695 //
    0x01,   // 1696 //
    0x00,   // 1697 //
    0x05,   // 1698 //
    0x01,   // 1699 //
    0x06,   // 1700 //
    0x00,   // 1701 //
    0x05,   // 1702 //
    0x00,   // 1703 //
    0x00,   // 1704 //
    0x48,   // 1705 //
    0x4B,   // 1706 //
    0x3D,   // 1707 //
    0x40,   // 1708 //
    0x41,   // 1709 //
    0x40,   // 1710 //
    0x79,   // 1711 //
    0x01,   // 1712 //
    0x00,   // 1713 //
    0x25,   // 1714 //
    0x00,   // 1715 //
    0x40,   // 1716 //
    0x4B,   // 1717 //
    0x4A,   // 1718 //
    0x3A,   // 1719 //
    0x50,   // 1720 //
    0x00,   // 1721 //
    0x4A,   // 1722 //
    0x41,   // 1723 //
    0x41,   // 1724 //
    0x40,   // 1725 //
    0x00,   // 1726 //
    0x50,   // 1727 //
    0x44,   // 1728 //
    0x45,   // 1729 //
    0x4F,   // 1730 //
    0x00,   // 1731 //
    0x3D,   // 1732 //
    0x4A,   // 1733 //
    0x55,   // 1734 //
    0x02,   // 1735 //
    0x49,   // 1736 //
    0x4B,   // 1737 //
    0x4E,   // 1738 //
    0x41,   // 1739 //
    0x79,   // 1740 //
    0x01,   // 1741 //
    0x00,   // 1742 //
    0x30,   // 1743 //
    0x4B,   // 1744 //
    0x4B,   // 1745 //
    0x00,   // 1746 //
    0x40,   // 1747 //
    0x3D,   // 1748 //
    0x4A,   // 1749 //
    0x43,   // 1750 //
    0x41,   // 1751 //
    0x4E,   // 1752 //
    0x4B,   // 1753 //
    0x51,   // 1754 //
    0x4F,   // 1755 //
    0x00,   // 1756 //
    0x50,   // 1757 //
    0x4B,   // 1758 //
    0x00,   // 1759 //
    0x49,   // 1760 //
    0x45,   // 1761 //
    0x54,   // 1762 //
    0x00,   // 1763 //
    0x44,   // 1764 //
    0x41,   // 1765 //
    0x4E,   // 1766 //
    0x41,   // 1767 //
    0x79,   // 1768 //
    0x01,   // 1769 //
    0x00,   // 1770 //
    0x30,   // 1771 //
    0x44,   // 1772 //
    0x41,   // 1773 //
    0x00,   // 1774 //
    0x4C,   // 1775 //
    0x4B,   // 1776 //
    0x45,   // 1777 //
    0x4F,   // 1778 //
    0x4B,   // 1779 //
    0x4A,   // 1780 //
    0x00,   // 1781 //
    0x43,   // 1782 //
    0x3D,   // 1783 //
    0x4F,   // 1784 //
    0x00,   // 1785 //
    0x41,   // 1786 //
    0x4A,   // 1787 //
    0x52,   // 1788 //
    0x41,   // 1789 //
    0x48,   // 1790 //
    0x4B,   // 1791 //
    0x4C,   // 1792 //
    0x41,   // 1793 //
    0x40,   // 1794 //
    0x79,   // 1795 //
    0x01,   // 1796 //
    0x00,   // 1797 //
    0x30,   // 1798 //
    0x44,   // 1799 //
    0x41,   // 1800 //
    0x00,   // 1801 //
    0x49,   // 1802 //
    0x45,   // 1803 //
    0x54,   // 1804 //
    0x45,   // 1805 //
    0x4A,   // 1806 //
    0x43,   // 1807 //
    0x00,   // 1808 //
    0x4F,   // 1809 //
    0x41,   // 1810 //
    0x41,   // 1811 //
    0x49,   // 1812 //
    0x4F,   // 1813 //
    0x00,   // 1814 //
    0x50,   // 1815 //
    0x4B,   // 1816 //
    0x00,   // 1817 //
    0x44,   // 1818 //
    0x3D,   // 1819 //
    0x52,   // 1820 //
    0x41,   // 1821 //
    0x02,   // 1822 //
    0x42,   // 1823 //
    0x3D,   // 1824 //
    0x45,   // 1825 //
    0x48,   // 1826 //
    0x41,   // 1827 //
    0x40,   // 1828 //
    0x79,   // 1829 //
    0x00,   // 1830 //
    0x00,   // 1831 //
    0x25,   // 1832 //
    0x50,   // 1833 //
    0x00,   // 1834 //
    0x40,   // 1835 //
    0x45,   // 1836 //
    0x4F,   // 1837 //
    0x3D,   // 1838 //
    0x4C,   // 1839 //
    0x4C,   // 1840 //
    0x41,   // 1841 //
    0x3D,   // 1842 //
    0x4E,   // 1843 //
    0x41,   // 1844 //
    0x40,   // 1845 //
    0x79,   // 1846 //
    0x01,   // 1847 //
    0x00,   // 1848 //
    0x33,   // 1849 //
    0x45,   // 1850 //
    0x48,   // 1851 //
    0x48,   // 1852 //
    0x00,   // 1853 //
    0x55,   // 1854 //
    0x4B,   // 1855 //
    0x51,   // 1856 //
    0x00,   // 1857 //
    0x49,   // 1858 //
    0x45,   // 1859 //
    0x54,   // 1860 //
    0x00,   // 1861 //
    0x50,   // 1862 //
    0x44,   // 1863 //
    0x41,   // 1864 //
    0x00,   // 1865 //
    0x44,   // 1866 //
    0x41,   // 1867 //
    0x4E,   // 1868 //
    0x3E,   // 1869 //
    0x4F,   // 1870 //
    0x1B,   // 1871 //
    0x08,   // 1872 //
    0x00,   // 1873 //
    0x01,   // 1874 //
    0x00,   // 1875 //
    0x29,   // 1876 //
    0x45,   // 1877 //
    0x54,   // 1878 //
    0x45,   // 1879 //
    0x4A,   // 1880 //
    0x43,   // 1881 //
    0x00,   // 1882 //
    0x50,   // 1883 //
    0x44,   // 1884 //
    0x41,   // 1885 //
    0x4F,   // 1886 //
    0x41,   // 1887 //
    0x00,   // 1888 //
    0x40,   // 1889 //
    0x4B,   // 1890 //
    0x41,   // 1891 //
    0x4F,   // 1892 //
    0x00,   // 1893 //
    0x4A,   // 1894 //
    0x4B,   // 1895 //
    0x50,   // 1896 //
    0x00,   // 1897 //
    0x4F,   // 1898 //
    0x41,   // 1899 //
    0x41,   // 1900 //
    0x49,   // 1901 //
    0x02,   // 1902 //
    0x50,   // 1903 //
    0x4B,   // 1904 //
    0x00,   // 1905 //
    0x53,   // 1906 //
    0x4B,   // 1907 //
    0x4E,   // 1908 //
    0x47,   // 1909 //
    0x79,   // 1910 //
    0x01,   // 1911 //
    0x00,   // 1912 //
    0x35,   // 1913 //
    0x4B,   // 1914 //
    0x51,   // 1915 //
    0x00,   // 1916 //
    0x3F,   // 1917 //
    0x3D,   // 1918 //
    0x4A,   // 1919 //
    0x3A,   // 1920 //
    0x50,   // 1921 //
    0x00,   // 1922 //
    0x51,   // 1923 //
    0x4F,   // 1924 //
    0x41,   // 1925 //
    0x00,   // 1926 //
    0x50,   // 1927 //
    0x44,   // 1928 //
    0x45,   // 1929 //
    0x4F,   // 1930 //
    0x00,   // 1931 //
    0x3D,   // 1932 //
    0x48,   // 1933 //
    0x4B,   // 1934 //
    0x4A,   // 1935 //
    0x41,   // 1936 //
    0x79,   // 1937 //
    0x01,   // 1938 //
    0x00,   // 1939 //
    0x30,   // 1940 //
    0x44,   // 1941 //
    0x45,   // 1942 //
    0x4F,   // 1943 //
    0x00,   // 1944 //
    0x3F,   // 1945 //
    0x44,   // 1946 //
    0x41,   // 1947 //
    0x49,   // 1948 //
    0x45,   // 1949 //
    0x3F,   // 1950 //
    0x3D,   // 1951 //
    0x48,   // 1952 //
    0x00,   // 1953 //
    0x3F,   // 1954 //
    0x3D,   // 1955 //
    0x4A,   // 1956 //
    0x00,   // 1957 //
    0x3E,   // 1958 //
    0x41,   // 1959 //
    0x00,   // 1960 //
    0x49,   // 1961 //
    0x45,   // 1962 //
    0x54,   // 1963 //
    0x41,   // 1964 //
    0x40,   // 1965 //
    0x02,   // 1966 //
    0x53,   // 1967 //
    0x45,   // 1968 //
    0x50,   // 1969 //
    0x44,   // 1970 //
    0x00,   // 1971 //
    0x4B,   // 1972 //
    0x50,   // 1973 //
    0x44,   // 1974 //
    0x41,   // 1975 //
    0x4E,   // 1976 //
    0x00,   // 1977 //
    0x3F,   // 1978 //
    0x44,   // 1979 //
    0x41,   // 1980 //
    0x49,   // 1981 //
    0x45,   // 1982 //
    0x3F,   // 1983 //
    0x3D,   // 1984 //
    0x48,   // 1985 //
    0x4F,   // 1986 //
    0x79,   // 1987 //
    0x01,   // 1988 //
    0x00,   // 1989 //
    0x35,   // 1990 //
    0x4B,   // 1991 //
    0x51,   // 1992 //
    0x00,   // 1993 //
    0x3F,   // 1994 //
    0x3D,   // 1995 //
    0x4A,   // 1996 //
    0x3A,   // 1997 //
    0x50,   // 1998 //
    0x00,   // 1999 //
    0x51,   // 2000 //
    0x4F,   // 2001 //
    0x41,   // 2002 //
    0x00,   // 2003 //
    0x45,   // 2004 //
    0x50,   // 2005 //
    0x00,   // 2006 //
    0x44,   // 2007 //
    0x41,   // 2008 //
    0x4E,   // 2009 //
    0x41,   // 2010 //
    0x79,   // 2011 //
    0x01,   // 2012 //
    0x00,   // 2013 //
    0x01,   // 2014 //
    0x00,   // 2015 //
    0x01,   // 2016 //
    0x00,   // 2017 //
    0x1E,   // 2018 //
    0x4B,   // 2019 //
    0x4B,   // 2020 //
    0x47,   // 2021 //
    0x4F,   // 2022 //
    0x00,   // 2023 //
    0x3D,   // 2024 //
    0x4E,   // 2025 //
    0x41,   // 2026 //
    0x00,   // 2027 //
    0x51,   // 2028 //
    0x4F,   // 2029 //
    0x41,   // 2030 //
    0x42,   // 2031 //
    0x51,   // 2032 //
    0x48,   // 2033 //
    0x00,   // 2034 //
    0x4B,   // 2035 //
    0x4A,   // 2036 //
    0x48,   // 2037 //
    0x55,   // 2038 //
    0x00,   // 2039 //
    0x45,   // 2040 //
    0x42,   // 2041 //
    0x02,   // 2042 //
    0x55,   // 2043 //
    0x4B,   // 2044 //
    0x51,   // 2045 //
    0x00,   // 2046 //
    0x4E,   // 2047 //
    0x41,   // 2048 //
    0x3D,   // 2049 //
    0x40,   // 2050 //
    0x00,   // 2051 //
    0x50,   // 2052 //
    0x44,   // 2053 //
    0x41,   // 2054 //
    0x49,   // 2055 //
    0x79,   // 2056 //
    0x01,   // 2057 //
    0x00,   // 2058 //
    0x35,   // 2059 //
    0x4B,   // 2060 //
    0x51,   // 2061 //
    0x00,   // 2062 //
    0x40,   // 2063 //
    0x4B,   // 2064 //
    0x4A,   // 2065 //
    0x3A,   // 2066 //
    0x50,   // 2067 //
    0x00,   // 2068 //
    0x4A,   // 2069 //
    0x41,   // 2070 //
    0x41,   // 2071 //
    0x40,   // 2072 //
    0x00,   // 2073 //
    0x50,   // 2074 //
    0x4B,   // 2075 //
    0x00,   // 2076 //
    0x51,   // 2077 //
    0x4F,   // 2078 //
    0x41,   // 2079 //
    0x00,   // 2080 //
    0x50,   // 2081 //
    0x44,   // 2082 //
    0x45,   // 2083 //
    0x4F,   // 2084 //
    0x02,   // 2085 //
    0x45,   // 2086 //
    0x50,   // 2087 //
    0x41,   // 2088 //
    0x49,   // 2089 //
    0x00,   // 2090 //
    0x3D,   // 2091 //
    0x50,   // 2092 //
    0x00,   // 2093 //
    0x50,   // 2094 //
    0x44,   // 2095 //
    0x41,   // 2096 //
    0x00,   // 2097 //
    0x49,   // 2098 //
    0x4B,   // 2099 //
    0x49,   // 2100 //
    0x41,   // 2101 //
    0x4A,   // 2102 //
    0x50,   // 2103 //
    0x79,   // 2104 //
    0x01,   // 2105 //
    0x00,   // 2106 //
    0x00,   // 2107 //
};

// start of each global message in Mess_data, by message number
UCHAR* Mess_tbl[62] =
{
    Mess_data,           // 0 //
    Mess_data + 0x1F,    // 1 //
    Mess_data + 0x34,    // 2 //
    Mess_data + 0x55,    // 3 //
    Mess_data + 0x70,    // 4 //
    Mess_data + 0x93,    // 5 //
    Mess_data + 0xB1,    // 6 //
    Mess_data + 0xCB,    // 7 //
    Mess_data + 0xF2,    // 8 //
    Mess_data + 0x119,   // 9 //
    Mess_data + 0x141,   // 10 //
    Mess_data + 0x169,   // 11 //
    Mess_data + 0x191,   // 12 //
    Mess_data + 0x1D3,   // 13 //
    Mess_data + 0x1FC,   // 14 //
    Mess_data + 0x221,   // 15 //
    Mess_data + 0x246,   // 16 //
    Mess_data + 0x274,   // 17 //
    Mess_data + 0x2A0,   // 18 //
    Mess_data + 0x2AF,   // 19 //
    Mess_data + 0x2CA,   // 20 //
    Mess_data + 0x2DD,   // 21 //
    Mess_data + 0x310,   // 22 //
    Mess_data + 0x325,   // 23 //
    Mess_data + 0x344,   // 24 //
    Mess_data + 0x35A,   // 25 //
    Mess_data + 0x38E,   // 26 //
    Mess_data + 0x3C0,   // 27 //
    Mess_data + 0x40F,   // 28 //
    Mess_data + 0x461,   // 29 //
    Mess_data + 0x4B1,   // 30 //
    Mess_data + 0x504,   // 31 //
    Mess_data + 0x102,   // 32 //
    Mess_data + 0x129,   // 33 //
    Mess_data + 0x151,   // 34 //
    Mess_data + 0x179,   // 35 //
    Mess_data + 0x54F,   // 36 //
    Mess_data + 0x56C,   // 37 //
    Mess_data + 0x589,   // 38 //
    Mess_data + 0x596,   // 39 //
    Mess_data + 0x5B6,   // 40 //
    Mess_data + 0x5E4,   // 41 //
    Mess_data + 0x607,   // 42 //
    Mess_data + 0x622,   // 43 //
    Mess_data + 0x63B,   // 44 //
    Mess_data + 0x663,   // 45 //
    Mess_data + 0x683,   // 46 //
    Mess_data + 0x6A2,   // 47 //
    Mess_data + 0x6B2,   // 48 //
    Mess_data + 0x6CF,   // 49 //
    Mess_data + 0x6EB,   // 50 //
    Mess_data + 0x706,   // 51 //
    Mess_data + 0x739,   // 52 //
    Mess_data + 0x754,   // 53 //
    Mess_data + 0x779,   // 54 //
    Mess_data + 0x7C6,   // 55 //
    Mess_data + 0x794,   // 56 //
    Mess_data + 0x7C6,   // 57 //
    Mess_data + 0x7C6,   // 58 //
    Mess_data + 0x7C6,   // 59 //
    Mess_data + 0x7C6,   // 60 //
    Mess_data + 0x80B,   // 61 //
};

UINT Moji_null = 0x00000000;   // zero word that the string table entry points at
UCHAR* Moji_tbl[1] = { (UCHAR*)&Moji_null };   // string table entry list for Moji_tbl_print
