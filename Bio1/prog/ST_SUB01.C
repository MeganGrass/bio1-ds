/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ST_SUB01.C
 */

#include "TYPES.H"
#include <RAND.H>
#include "ST_SUB01.H"
#include "ITEM.H"
#include "MOJIDISP.H"
#include "PL_SCE.H"
#include "SCE1.H"
#include "SND.H"
#include "ST_SUB00.H"
#include "ST_SUB02.H"
#include "ST_SUB03.H"
#include "SCENARIO.H"

#define ECG_X1 (*(SHORT*)&St.Ecg_x1)  // cast kept: end x of the primary trace as a SHORT, St.Ecg_x1 and Ecg_x1h hold its two bytes

VOID Ecg_label_disp(UCHAR status, UCHAR* frame)
{
    GsOT* ot;
    UCHAR t;
    INT danger_v;
    INT caution_v;
    INT danger2_v;

    switch (status)
    {
    case 0:
        *frame = Ecg_step_get(Ecg_danger_step_tbl);
        t = Ecg_danger_frm_tbl[*frame];
        *frame = t;
        if (*frame != 0)
        {
            ot = MAIN.Ot0;

            danger_v = t * 8 + 0x830;
            ScratchPad->Spr.v = danger_v;

            GsSortFlipSprite(&ScratchPad->Spr, ot + G.Cbuf_id, 4);
        }

        break;
    case 1:
        *frame = Ecg_step_get(Ecg_caution_step_tbl);
        t = Ecg_caution_frm_tbl[*frame];
        *frame = t;
        if (*frame != 0)
        {
            ot = MAIN.Ot0;

            caution_v = t * 8 + 0x818;
            ScratchPad->Spr.v = caution_v;

            GsSortFlipSprite(&ScratchPad->Spr, ot + G.Cbuf_id, 4);
        }

        break;
    case 2:
    case 3:
        ScratchPad->Spr.v = 0;
        GsSortFlipSprite(&ScratchPad->Spr, MAIN.Ot0 + G.Cbuf_id, 4);
        break;
    case 4:
        *frame = Ecg_step_get(Ecg_danger_step_tbl);
        t = Ecg_danger_frm_tbl[*frame];
        *frame = t;
        if (*frame != 0)
        {
            ot = MAIN.Ot0;

            danger2_v = t * 8 + 0x840;
            ScratchPad->Spr.v = danger2_v;

            GsSortFlipSprite(&ScratchPad->Spr, ot + G.Cbuf_id, 4);
        }
        break;
    }
}

VOID St_disp_ECG(VOID)
{
    UCHAR Loop_ctr;
    UCHAR* pState;
    INT rnd;
    UCHAR Wave_idx;
    UCHAR* pFace;
    INT rem;
    SHORT x0;
    SHORT x1;
    ECG_SEG_REC* pSeg;
    SCHAR x_start;
    INT line_x0;
    INT line_x1;
    INT gline_x0;
    INT gline_x1;

    pState = &St.Ecg_rtn;
    switch (*pState)
    {
    case 0:
        *pState = 1;

        ECG_X1 = 0x84;
        St.Ecg2_x1 = 0x84;

    case 1:
        ScratchPad->Line.attribute = 0;
        ScratchPad->Gline.attribute = 0;
        if (++ECG_X1 >= 0x84)
        {
            ECG_X1 = 0x35;
            if (G.Player.Life != 0)
            {
                St.Ecg_type = (G.Player.Life - 1) / (G.Player.Max_life >> 2);

                if (St.Ecg_type >= 4)
                {
                    St.Ecg_type = 3;
                }
            }
            else
            {
                St.Ecg_type = 0;
            }
            St.Ecg_col[0] = Ecg_rgb_tbl[St.Ecg_type][0];
            St.Ecg_col[1] = Ecg_rgb_tbl[St.Ecg_type][1];
            St.Ecg_col[2] = Ecg_rgb_tbl[St.Ecg_type][2];
            if ((G.Player.Sca_info & 0x22) && (rand() & 1))
            {
                St.Ecg_type = 4;
            }

            rnd = rand() & 3;
            pFace = &St.Face_anm;
            Wave_idx = rnd;
            if ((*pFace & 3) == Wave_idx)
            {
                Wave_idx = (rnd + 1) & 3;
            }
            Wave_idx |= St.Ecg_type << 2;

            *pFace = Wave_idx;

            St.pEcg_wave = Ecg_line_tbl[Wave_idx];
        }

        St.Ecg_x0 = ECG_X1 + 0xF;
        if (++St.Ecg2_x1 >= 0x84)
        {
            St.Ecg2_x1 = ECG_X1 - 0x1F;
            St.pEcg2_wave = St.pEcg_wave;
            St.Ecg2_col[0] = St.Ecg_col[0];
            St.Ecg2_col[1] = St.Ecg_col[1];
            St.Ecg2_col[2] = St.Ecg_col[2];
        }
        St.Ecg2_x0 = St.Ecg2_x1 + 0x1F;
        if (G.Player.Sca_info & 0x22)
        {
            St.Ecg_type = 4;
        }
        ScratchPad->Spr.attribute = 0x1000040;
        ScratchPad->Spr.x = 0x64;
        ScratchPad->Spr.y = 0xA8;
        ScratchPad->Spr.u = 0x60;
        ScratchPad->Spr.w = 0x20;
        ScratchPad->Spr.h = 8;
        ScratchPad->Spr.cy = 0x1E4;
        ScratchPad->Spr.cx = 0;
        ScratchPad->Spr.tpage = 0x1C;

        Ecg_label_disp(St.Ecg_type, &Loop_ctr);

        if (St.Ecg_x0 >= 0x54 && ECG_X1 < 0x84)
        {
            ScratchPad->Line.r = St.Ecg_col[0];
            ScratchPad->Line.g = St.Ecg_col[1];
            ScratchPad->Line.b = St.Ecg_col[2];
            x0 = St.Ecg_x0;
            if (St.Ecg_x0 >= 0x84)
            {
                x0 = 0x83;
            }
            x1 = ECG_X1;
            if (ECG_X1 < 0x54)
            {
                x1 = 0x54;
            }
            pSeg = St.pEcg_wave;
            while (x0 < pSeg->Start_x + 0x54)
            {
                pSeg++;
            }
            ScratchPad->Line.y1 = pSeg->Start_y + 0xA2;
            line_x0 = x0 - 0x54;
            ScratchPad->Line.y0 = pSeg->Start_y + pSeg->Slope * (line_x0 - pSeg->Start_x) + 0xA2;
            ScratchPad->Line.x0 = x0;

            x_start = pSeg->Start_x;
            if (x1 >= (SHORT)(x_start + 0x54))  // cast kept: x_start is a SCHAR, the sum is compared as a SHORT
            {
                ScratchPad->Line.x1 = x1;
                GsSortLine(&ScratchPad->Line, MAIN.Ot0 + G.Cbuf_id, 4);
            }
            else
            {
                ScratchPad->Line.x1 = x_start + 0x54;

                GsSortLine(&ScratchPad->Line, MAIN.Ot0 + G.Cbuf_id, 4);
                if (pSeg->Start_x != 0)
                {
                    pSeg++;
                    while (x1 < pSeg->Start_x + 0x54)
                    {
                        ScratchPad->Line.x0 = pSeg->End_x + 0x54;
                        ScratchPad->Line.x1 = pSeg->Start_x + 0x54;
                        ScratchPad->Line.y0 = pSeg[-1].Start_y + 0xA2;
                        ScratchPad->Line.y1 = pSeg->Start_y + 0xA2;
                        GsSortLine(&ScratchPad->Line, MAIN.Ot0 + G.Cbuf_id, 4);
                        pSeg++;
                    }
                    ScratchPad->Line.x0 = pSeg->End_x + 0x54;
                    ScratchPad->Line.x1 = x1;
                    ScratchPad->Line.y0 = pSeg[-1].Start_y + 0xA2;
                    line_x1 = x1 - 0x54;
                    ScratchPad->Line.y1 = pSeg->Start_y + pSeg->Slope * (line_x1 - pSeg->Start_x) + 0xA2;
                    GsSortLine(&ScratchPad->Line, MAIN.Ot0 + G.Cbuf_id, 4);
                }
            }
        }
        if (St.Ecg2_x0 >= 0x54 && St.Ecg2_x1 < 0x84)
        {
            UCHAR dr;
            UCHAR dg;
            UCHAR db;

            x0 = St.Ecg2_x0;
            dr = St.Ecg2_col[0] >> 5;
            dg = St.Ecg2_col[1] >> 5;
            db = St.Ecg2_col[2] >> 5;
            if (St.Ecg2_x0 >= 0x84)
            {
                rem = St.Ecg2_x0 - 0x83;
                ScratchPad->Gline.r0 = St.Ecg2_col[0] - dr * rem;
                ScratchPad->Gline.g0 = St.Ecg2_col[1] - dg * rem;
                ScratchPad->Gline.b0 = St.Ecg2_col[2] - db * rem;

                x0 = 0x83;
            }
            else
            {
                ScratchPad->Gline.r0 = St.Ecg2_col[0];
                ScratchPad->Gline.g0 = St.Ecg2_col[1];
                ScratchPad->Gline.b0 = St.Ecg2_col[2];
            }
            x1 = St.Ecg2_x1;
            if (St.Ecg2_x1 < 0x54)
            {
                x1 = 0x54;
            }
            pSeg = St.pEcg2_wave;
            while (x0 < pSeg->Start_x + 0x54)
            {
                pSeg++;
            }
            ScratchPad->Gline.y1 = pSeg->Start_y + 0xA2;
            gline_x0 = x0 - 0x54;
            ScratchPad->Gline.y0 = pSeg->Start_y + pSeg->Slope * (gline_x0 - pSeg->Start_x) + 0xA2;
            ScratchPad->Gline.x0 = x0;

            x_start = pSeg->Start_x;
            if (x1 >= (SHORT)(x_start + 0x54))  // cast kept: x_start is a SCHAR, the sum is compared as a SHORT
            {
                ScratchPad->Gline.x1 = x1;
                Ecg_seg_disp(dr, dg, db);
                return;
            }

            ScratchPad->Gline.x1 = x_start + 0x54;

            Ecg_seg_disp(dr, dg, db);
            if (pSeg->Start_x == 0)
            {
                return;
            }
            pSeg++;
            while (x1 < pSeg->Start_x + 0x54)
            {
                ScratchPad->Gline.x0 = pSeg->End_x + 0x54;
                ScratchPad->Gline.x1 = pSeg->Start_x + 0x54;
                ScratchPad->Gline.y0 = pSeg[-1].Start_y + 0xA2;
                ScratchPad->Gline.y1 = pSeg->Start_y + 0xA2;
                Ecg_seg_disp(dr, dg, db);
                pSeg++;
            }
            ScratchPad->Gline.x0 = pSeg->End_x + 0x54;
            ScratchPad->Gline.x1 = x1;
            ScratchPad->Gline.y0 = pSeg[-1].Start_y + 0xA2;
            gline_x1 = x1 - 0x54;
            ScratchPad->Gline.y1 = pSeg->Start_y + pSeg->Slope * (gline_x1 - pSeg->Start_x) + 0xA2;
            Ecg_seg_disp(dr, dg, db);
        }
        return;
    case 2:
        *pState = 3;

        St.Ecg_x0 = 0xAF;

    case 3:
        ScratchPad->Line.attribute = 0x50000000;
        ScratchPad->Line.x0 = 0x54;
        ScratchPad->Line.x1 = 0x83;
        ScratchPad->Line.r = 0;
        ScratchPad->Line.g = 0;
        ScratchPad->Line.b = 0;
        if (St.Ecg_x0 >= 0x84)
        {
            if (St.Ecg_x0 < 0x92)
            {
                ScratchPad->Line.y0 = 0x92;

                Loop_ctr = 0x92 - St.Ecg_x0;
                {
                    INT ecg = ~(Loop_ctr << 4);
                    St.Ecg_type = ecg;
                }
                Loop_ctr = 0xF - Loop_ctr;
            }
            else
            {
                St.Ecg_type = 0xFF;
                ScratchPad->Line.y0 = St.Ecg_x0;

                Loop_ctr = 0xF;
            }
            if (St.St_flg & 0x20)
            {
                ScratchPad->Line.g = St.Ecg_type;
                if (St.St_flg & 0x10)
                {
                    ScratchPad->Line.r = St.Ecg_type;
                    ScratchPad->Line.b = St.Ecg_type;
                }
            }
            else
            {
                ScratchPad->Line.b = St.Ecg_type;
            }
            do
            {
                ScratchPad->Line.y1 = ScratchPad->Line.y0;
                GsSortLine(&ScratchPad->Line, MAIN.Ot0 + G.Cbuf_id, 4);
                if (ScratchPad->Line.r != 0)
                {
                    ScratchPad->Line.r -= 0x10;
                }
                if (ScratchPad->Line.g != 0)
                {
                    ScratchPad->Line.g -= 0x10;
                }
                if (ScratchPad->Line.b != 0)
                {
                    ScratchPad->Line.b -= 0x10;
                }

                ScratchPad->Line.y0++;

                Loop_ctr--;
            } while (ScratchPad->Line.y0 < 0xB0 && Loop_ctr != 0);

            St.Ecg_x0 -= 2;
        }
        else
        {
            St.Ecg_rtn = 0;
            St.Act_res = 0;
            St.St_flg &= 0xCF;
        }

        break;
    }
}

INT Ecg_step_get(UCHAR* steps)
{
    INT idx;
    UINT x;
    UCHAR* p;

    p = steps;
    x = (St.Ecg_x1 - 0x35) & 0xFF;
    idx = 0;
    if (x >= *p)
    {
        do
        {
            p += 1;
            idx += 1;
        } while (x >= *p);
    }
    return idx & 0xFF;
}

VOID Ecg_seg_disp(INT dr, INT dg, INT db)
{
    INT width;

    width = (ScratchPad->Gline.x0 & 0xFF) - (ScratchPad->Gline.x1 & 0xFF);  // the width is the difference of the two low bytes
    ScratchPad->Gline.r1 = ScratchPad->Gline.r0 - dr * width;
    ScratchPad->Gline.g1 = ScratchPad->Gline.g0 - dg * width;
    ScratchPad->Gline.b1 = ScratchPad->Gline.b0 - db * width;
    GsSortGLine(&ScratchPad->Gline, &MAIN.Ot0[G.Cbuf_id], 4U);
    ScratchPad->Gline.r0 = ScratchPad->Gline.r1;
    ScratchPad->Gline.g0 = ScratchPad->Gline.g1;
    ScratchPad->Gline.b0 = ScratchPad->Gline.b1;
}

VOID St_main00(VOID)
{
    UCHAR* st;
    USHORT* pad;

    st = &St.Sub_rtn0;
    switch (*st)
    {
    case 0:
        *st = 1;
        St.Blink_timer = 0;
        St_sel_item_set();

    case 1:
        if (G.SaveData.Key_trg & 0x4000)
        {
            UCHAR* sel = &St.Icursol_main;

            if (*sel & 0xF8)
            {
                if (St.Item_id == 0)
                {
                    goto dec;
                }
                St.Wep_flg = St.Item_id < 11;
            }
            else if (*sel == 4)
            {
                if (St.Radio_flg == 0)
                {
                dec:
                    (((STATUS*)(VOID*)&St)->Blink_timer)--;  // cast kept: the plain St.Blink_timer-- changes the generated code
                    goto draw;
                }
                if (Flg_ck(Use_flg, 0x3F) == 0)
                {
                    St.View_rno = 0;

                    St.Item_id = 0x4D;

                    Item_model_load();
                }
            }
            St.Sub_rtn0 = 2;
            St.Sub_sts = 0;
            St.Ibox_sel = 0;
            St.Blink_timer = 0;
            Snd_se_play(SE_BANK_ENEMY, 6, 0);
        }
        else
        {
            pad = &MAIN.Padd1_trg;
            if (*pad & 0xF000)
            {
                if (*pad & 0xA000)
                {
                    UCHAR* sel = &St.Icursol_main;

                    *sel ^= 2;
                }
                if (*pad & 0x1000)
                {
                    UCHAR* sel = &St.Icursol_main;

                    if (*sel & 0xFC)
                    {
                        *sel -= 4;
                    }
                    else
                    {
                        *sel += (St.nSlot + 2) * 2;
                    }
                }
                else if (*pad & 0x4000)
                {
                    UCHAR* sel = &St.Icursol_main;

                    *sel += 4;
                    if (*sel > 2 * (St.nSlot + 3))
                    {
                        *sel &= 2;
                    }
                }
                St_sel_item_set();
                St.Blink_timer = 0;
                Snd_se_play(SE_BANK_ENEMY, 4, 0);
            }
            else
            {
                UCHAR* Loop_ctr = &St.Blink_timer;

                (*Loop_ctr)--;
            }
        }
    draw:
        St_disp_cursol0();

        Name_disp(St.Item_id, 0, (USHORT)(0x30 - (USHORT)Scr_ofs[0]), (USHORT)(0xBA - (USHORT)Scr_ofs[1]));

        break;
    case 2:
        if (St.Icursol_main & 0xF8)
        {
            St_menu0_item();
        }
        else
        {
            St_menu0_proc_tbl[St.Icursol_main >> 1]();
        }

        if (!(St.St_flg & 0x41) && St.Main_rtn != 7)
        {
            St_disp_cursol0();
        }

        break;
    }
}

VOID St_disp_cursol0(VOID)
{
    UCHAR v;
    USHORT y;
    GsOT* ot;
    UCHAR* pCursor_no = &St.Icursol_main;

    ScratchPad->Spr.attribute = 0x01000040;
    ScratchPad->Spr.tpage = 0x1C;
    ScratchPad->Spr.x = St_slot_pos_tbl[*pCursor_no];
    y = (&St_slot_pos_tbl[1])[*pCursor_no];  // cast kept: St_slot_pos_tbl is declared USHORT, the coordinates are signed
    ScratchPad->Spr.cx = 0;
    ScratchPad->Spr.cy = 0x1E4;
    ScratchPad->Spr.y = y;
    if (*pCursor_no & 0xF8)
    {
        ScratchPad->Spr.w = 0x28;
        ScratchPad->Spr.h = 0x1E;
        ScratchPad->Spr.u = 0x80;
        ScratchPad->Spr.v = 0xE0;
        if (!(St.Blink_timer & 0x20))
        {
            ScratchPad->Spr.u = 0xA8;
        }
        ScratchPad->Rect.x = 0xCE;
        if (!(St.Chr_flg & 2))
        {
            ScratchPad->Spr.y = y + 0x1E;
        }
        ot = MAIN.Ot0;
        GsSortFlipSprite(&ScratchPad->Spr, ot + G.Cbuf_id, 3U);
        v = 0x90;
    }
    else
    {
        ScratchPad->Spr.w = 0x30;
        ScratchPad->Spr.h = 0x10;
        ScratchPad->Spr.v = 0x50;
        ScratchPad->Spr.u = 0;
        if (St.Main_rtn == 3)
        {
            ScratchPad->Spr.x = St_slot_pos_tbl[(*pCursor_no) | 2];
        }
        ot = MAIN.Ot0;
        GsSortFlipSprite(&ScratchPad->Spr, ot + G.Cbuf_id, 4U);
        v = 0x98;
    }
    ScratchPad->Spr.v = v;
}

VOID St_sel_item_set(VOID)
{
    UINT slot;

    slot = ((St.Icursol_main >> 1) - 4) & 0xFF;
    if (slot < G.SaveData.Item_num)
    {
        St.Item_id = pItem_work_base[slot].Id;
        return;
    }
    St.Item_id = 0;
}

// tab handlers: map, file, radio, exit (function addresses stored as words, hence the  casts)
VOID (*St_menu0_proc_tbl[4])(VOID) =
{
    St_menu0_map,    // 0 //
    St_menu0_file,    // 1 //
    St_menu0_radio,    // 2 //
    St_menu0_exit,    // 3 //
};
