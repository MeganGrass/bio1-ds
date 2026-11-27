/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\MAP.C
 */

#include "TYPES.H"

#include "MAP.H"
#include "GAME.H"
#include "ITEM.H"
#include "SCE1.H"
#include "FILECODE.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "SCENARIO.H"

MAP_SCR_WORK Map_scr_work; // map screen state
RECT Map_upload_rect; // VRAM destination rectangle of the map image and blink colour uploads

static USHORT Map_blink_col; // palette colour written for the blinking current room
static SCHAR Map_blink_timer; // frames left in the current map zoom hold
static UCHAR Map_blink_val; // Kido of the blinking current room, ramps between 0x50 and 0xA0

INT St_map_main(VOID)
{
    MAP_SCR_WORK* p;

    p = &Map_scr_work;
    if ((G.SaveData.Key_trg & 0x8000) && Map_scr_work.Zoom_mode >= 7U)
    {
        p->state = 0;
    }
    switch (p->state)
    {
    case 0:
        if (KEY_TRG_CUR & 0x8000)
        {
            *(INT*)p = 0;    // clear state, substate, Blink_mode and Zoom_mode together

            Snd_se_play(SE_BANK_ENEMY, 5, 0);
            return -1;
        }
        Map_tab_main(p);
        if (MAIN.SaveData.Key_trg & 0x4000)
        {
            *(INT*)p = 1;    // state 1, other three cleared
        }
        return 0;
    case 1:
        Map_disp_main(p);
        if ((G.SaveData.Key_trg & 0x4000) && (p->Zoom_mode == 0 || p->Zoom_mode >= 7U))
        {
            if (p->Zoom_mode == 8)
            {
                p->Zoom_mode = 3;
            }
            if (p->Zoom_mode == 9)
            {
                p->Zoom_mode = 2;
            }
            Snd_se_play(SE_BANK_ENEMY, 9, 0);
        }
        if (G.SaveData.Key_trg & 0x8000)
        {
            p->Zoom_mode = 7;
            Snd_se_play(SE_BANK_ENEMY, 5, 0);
        }
        return 0;
    }
    return 0;
}

VOID Map_tab_main(MAP_SCR_WORK* pMap_scr)
{
    UCHAR substate;

    substate = pMap_scr->substate;
    switch (substate)
    {
    case 0:
        Map_tab_load(pMap_scr);
        pMap_scr->substate++;
        return;
    case 1:
        Map_tab_move(pMap_scr);
        return;
    }
}

VOID Map_tab_load(MAP_SCR_WORK* pMap_scr)
{
    UINT i;
    UINT sel;
    UCHAR* cur;
    INT bits;

    sel = 0;
    bits = pMap_scr->Layout_mask;
    for (i = 0; i < 4; i++)
    {
        if ((bits >> i) & 1)
        {
            sel = i;
        }
    }
    cur = &St.Res_id;
    if (*cur != sel + 0x80)
    {
        *cur = sel + 0x80;

        Cd_read_wait(Map_tab_file_tbl[sel], MENU_TEX_BUF);

        VIEW(INT, Map_upload_rect) = 0x01000140;  // INT* view of the field (a plain access changes the code)
        VIEW(INT, Map_upload_rect.w) = 0x880080;  // INT* view of the width and height (a plain access changes the code)

        Tim_trans(MENU_TEX_BUF, &Map_upload_rect, 0x1C);
    }
    Map_layout_set(pMap_scr);
}

VOID Map_tab_move(MAP_SCR_WORK* pMap_scr)
{
    if (!(pMap_scr->mode & 1))
    {
        if ((G.Padd1_trg & PAD_UP) && (pMap_scr->Area_flg & 1))
        {
            pMap_scr->Room_idx[pMap_scr->layout]++;
            Snd_se_play(SE_BANK_ENEMY, 4, 0);
        }
        if ((G.Padd1_trg & PAD_DOWN) && (pMap_scr->Area_flg & 2))
        {
            pMap_scr->Room_idx[pMap_scr->layout]--;
            Snd_se_play(SE_BANK_ENEMY, 4, 0);
        }
        if (G.Padd1_trg & PAD_LEFT)
        {
            if (pMap_scr->Layout_mask & 8)
            {
                switch (pMap_scr->layout)
                {
                case 0:
                    pMap_scr->layout = 3;
                    break;
                case 1:
                    pMap_scr->layout = 2;
                    break;
                case 2:
                    pMap_scr->layout = 2;
                    break;
                case 3:
                    pMap_scr->layout = 1;
                    break;
                }
                Snd_se_play(SE_BANK_ENEMY, 4, 0);
            }
            else
            {
                if (pMap_scr->layout != 2 && ((pMap_scr->Layout_mask >> (pMap_scr->layout + 1)) & 1))
                {
                    pMap_scr->layout++;
                    Snd_se_play(SE_BANK_ENEMY, 4, 0);
                }
            }
        }
        if (G.Padd1_trg & PAD_RIGHT)
        {
            if (pMap_scr->Layout_mask & 8)
            {
                switch (pMap_scr->layout)
                {
                case 1:
                    pMap_scr->layout = 3;
                    break;
                case 2:
                    pMap_scr->layout = 1;
                    break;
                case 0:
                case 3:
                    pMap_scr->layout = 0;
                    break;
                }
                Snd_se_play(SE_BANK_ENEMY, 4, 0);
            }
            else
            {
                if (pMap_scr->layout != 0 && ((pMap_scr->Layout_mask >> (pMap_scr->layout - 1)) & 1))
                {
                    pMap_scr->layout--;
                    Snd_se_play(SE_BANK_ENEMY, 4, 0);
                }
            }
        }
    }
    Map_layout_set(pMap_scr);
}

VOID Map_layout_set(MAP_SCR_WORK* pMap_scr)
{
    SCHAR bit;
    UINT i;

    pMap_scr->area = Map_floor_tbl[pMap_scr->layout][pMap_scr->Room_idx[pMap_scr->layout]];
    pMap_scr->Area_flg = 0;
    i = pMap_scr->Room_idx[pMap_scr->layout];
    if (i + 1 != Map_floor_cnt_tbl[pMap_scr->layout])
    {
        bit = Map_floor_tbl[pMap_scr->layout][i + 1];
        if (((pMap_scr->Area_mask >> bit) & 1) || Map_area_ck(bit) != 0)
        {
            pMap_scr->Area_flg |= 1;
        }
    }
    if (i != 0)
    {
        bit = Map_floor_tbl[pMap_scr->layout][i - 1];
        if (((pMap_scr->Area_mask >> bit) & 1) || Map_area_ck(bit) != 0)
        {
            pMap_scr->Area_flg |= 2;
        }
    }
    for (i = 0; i < 4; i++)
    {
        if (i == pMap_scr->layout)
        {
            Map_tab_spr[i + 1].r = Map_tab_spr[i + 1].g = Map_tab_spr[i + 1].b = 0x80;
        }
        else
        {
            Map_tab_spr[i + 1].r = Map_tab_spr[i + 1].g = Map_tab_spr[i + 1].b = 0x40;
        }
    }
    Map_tab_spr[0].r = Map_tab_spr[0].g = Map_tab_spr[0].b = 0x80;
    for (i = 5; i < 9; i++)
    {
        Map_tab_spr[i].r = Map_tab_spr[i].g = Map_tab_spr[i].b = 0x80;
    }
    Map_tab_draw(pMap_scr);
}

VOID Map_tab_draw(MAP_SCR_WORK* pMap_scr)
{
    INT i;

    UCHAR t;

    GsSortFastSprite(&Map_tab_spr[0], &MAIN.Ot1[G.Cbuf_id], 0x10);
    for (i = 0; i < 4; i++)
    {
        if ((pMap_scr->Layout_mask >> i) & 1)
        {
            GsSortFastSprite(&Map_tab_spr[i + 1], &MAIN.Ot1[G.Cbuf_id], 0xF);
        }
    }
    Map_tab_spr[5].v = pMap_scr->layout * 0x10 + 0x48;
    GsSortFastSprite(&Map_tab_spr[5], &MAIN.Ot1[CBUF_ID_CUR], 0xF);
    i = pMap_scr->layout;
    Map_tab_spr[6].v = 0x60 - (Map_label_tbl[i] + pMap_scr->Room_idx[i]) * 0x10;
    GsSortFastSprite(&Map_tab_spr[6], &MAIN.Ot1[MAIN.Cbuf_id], 0xF);
    t = pMap_scr->Hilite_timer + 1;
    i = (t & 0x30) != 0;
    pMap_scr->Hilite_timer = t;
    if (pMap_scr->Area_flg & 1)
    {
        Map_tab_spr[7].u = i * 8 - 0x60;
        GsSortFastSprite(&Map_tab_spr[7], &MAIN.Ot1[MAIN.Cbuf_id], 0xF);
    }
    if (pMap_scr->Area_flg & 2)
    {
        Map_tab_spr[8].u = i * 8 - 0x50;
        GsSortFastSprite(&Map_tab_spr[8], &MAIN.Ot1[MAIN.Cbuf_id], 0xF);
    }
}

VOID Map_disp_main(MAP_SCR_WORK* pMap_scr)
{
    INT Ofs_r;
    SCHAR* base;
    INT i;
    GsSPRITE* spr;
    SCHAR Col_val;

    switch (pMap_scr->substate)
    {
    case 0:
        Map_tex_load(pMap_scr);
        pMap_scr->substate++;
        break;
    case 1:
        Map_anm_main(pMap_scr);
        break;
    }
    i = 1;
    Col_val = 0x30;
    base = &Map_tab_spr[0].r;
    spr = (GsSPRITE*)(base + 0x10);  // the sprite table is read as GsSPRITE records from the offset 0x10 of its first record
    Ofs_r = sizeof(GsSPRITE);
    Map_tab_spr[0].b = 0x40;
    Map_tab_spr[0].g = 0x40;
    *base = 0x40;
loop:
    spr->b = Col_val;
    spr->g = Col_val;
    spr++;
    (&Map_tab_spr[0].r)[Ofs_r] = Col_val;
    Ofs_r += sizeof(GsSPRITE);
    i += 1;
    if (i < 9)
    {
        goto loop;
    }
    Map_tab_draw(pMap_scr);
}

VOID Map_tex_load(MAP_SCR_WORK* pMap_scr)
{
    UCHAR* flag = &St.Res_id;
    SHORT* p;
    INT half;
    INT n;
    UINT i;

    half = 0;

    if (*flag != pMap_scr->area + 0x84)
    {
        *flag = pMap_scr->area + 0x84;

        Cd_read_wait(Map_floor_file_tbl[pMap_scr->area], (MENU_TEX_BUF));

        p = (SHORT*)MAP_FLOOR_FLG;  // halfword view of the same memory
        if (Map_area_ck(pMap_scr->area) == 0)
        {
            for (i = 6; i != 0; i--)
            {
                *p++ = 0;
            }
        }
        else
        {
            half = 1;
            p = (SHORT*)MAP_ROOM_FLG2;  // halfword view of the same memory
        }

        for (i = 0; i < Map_room_cnt_tbl[Map_stage_tbl[pMap_scr->area]]; i++)
        {
            if (Flg_ck(&MAIN.SaveData.Room_flg[0], i + Map_flg_base_tbl[Map_stage_tbl[pMap_scr->area]]) == 0)
            {
                if (half)
                {
                    p++;
                    for (n = 3; n != 0; n--)
                    {
                        *p++ = 0;
                    }
                    p += 2;
                }
                else
                {
                    for (n = 6; n != 0; n--)
                    {
                        *p++ = 0;
                    }
                }
            }
            else
            {
                p += 6;
            }
        }
        Map_upload_rect.x = 0x140;
        Map_upload_rect.y = 0x188;
        Map_upload_rect.w = 0x80;
        Map_upload_rect.h = 0x70;
        Tim_trans((VOID*)MENU_TEX_BUF, &Map_upload_rect, 0x1F);  // untyped pointer, the callee takes the address as is
    }

    if ((pMap_scr->mode == 2 && pMap_scr->area == 0) || (pMap_scr->mode == 4 && pMap_scr->area == 6))
    {
        pMap_scr->Hilite_mode = 1;
    }
    else
    {
        pMap_scr->Hilite_mode = 0;
    }
    Map_anm_main(pMap_scr);
}

VOID Map_anm_main(MAP_SCR_WORK* pMap_scr)
{
    if (Map_stage_tbl[pMap_scr->area] == pMap_scr->area_group)
    {
        switch (pMap_scr->Blink_mode)
        {
        case 0:
            Map_blink_val = 0x50;
            pMap_scr->Blink_mode++;
            break;
        case 1:
            if ((Map_blink_val = Map_blink_val + 2) > 0xA0)
            {
                pMap_scr->Blink_mode++;
            }
            break;
        case 2:
            if ((Map_blink_val = Map_blink_val - 2) < 0x50)
            {
                pMap_scr->Blink_mode = 1;
            }
            break;
        }
        Map_blink_col = (Map_blink_val >> 3) | 0x8000;
        Map_upload_rect.x = pMap_scr->room * 6 + 0x10;
        Map_upload_rect.y = 0x1FF;
        Map_upload_rect.w = 1;
        Map_upload_rect.h = 1;
        LoadImage(&Map_upload_rect, (ULONG*)&Map_blink_col);  // word view of the same memory
    }
    switch (pMap_scr->Zoom_mode)
    {
    case 0:

        Map_spr->scalex = Map_spr[1].scalex = Map_spr[2].scalex = 1;
        Map_spr[0].scaley = Map_spr[1].scaley = Map_spr[2].scaley = 200;

        Map_spr[0].attribute &= ~0x10000000;
        pMap_scr->Zoom_mode++;
        Snd_se_play(SE_BANK_ENEMY, 9, 0);
    case 1:
        Map_spr[0].y = 2;
        Map_spr[0].w = 0xA1;

        Map_spr[1].y = 0;
        Map_spr[2].y = 0;

        Map_spr[0].x = 0;
        Map_spr[1].x = 0;
        Map_spr[2].x = 0;

        Map_spr[2].w = 0x51;

        Map_spr[1].w = 0x51;

        pMap_scr->Zoom_mode++;
        break;
    case 2:
        Map_spr[0].scalex *= 3;
        Map_spr[1].scalex *= 3;
        Map_spr[2].scalex *= 3;
        if (Map_spr[0].scalex >= 0xFFF)
        {
            Map_spr[0].scalex = Map_spr[1].scalex = Map_spr[2].scalex = 0xFFF;

            pMap_scr->Zoom_mode++;
        }
        break;
    case 3:
        Map_spr[0].scaley += 900;
        Map_spr[1].scaley += 900;
        Map_spr[2].scaley += 900;
        if (Map_spr[0].scaley >= 0x15DC)
        {
            Map_spr[0].scaley = Map_spr[1].scaley = Map_spr[2].scaley = 0x15DC;

            pMap_scr->Zoom_mode++;
        }
        break;
    case 4:
        Map_spr[0].scaley -= 300;
        Map_spr[1].scaley -= 300;
        Map_spr[2].scaley -= 300;
        if (Map_spr[0].scaley <= 0x1000)
        {
            Map_spr[0].w = 0xA0;

            Map_spr[2].w = 0x50;

            Map_spr[1].w = 0x50;
            Map_spr[0].scaley = Map_spr[1].scaley = Map_spr[2].scaley = 0x1000;
            Map_spr[0].scalex = Map_spr[1].scalex = Map_spr[2].scalex = 0x1000;

            Map_spr[0].x = 0x70;
            Map_spr[1].x = 0x70;
            Map_spr[2].x = 0x70;

            Map_spr[1].y = 0x4C;
            Map_spr[2].y = 0x4C;

            Map_spr[0].y = 0x4E;

            Map_blink_timer = 8;
            pMap_scr->Zoom_mode++;
        }
        break;
    case 5:
        if (--Map_blink_timer == 0)
        {
            Map_spr[0].attribute |= 0x10000000;
            pMap_scr->Zoom_mode++;
            Map_blink_timer = 10;
        }
        break;
    case 6:
        if (pMap_scr->Hilite_mode == 1)
        {
            if (--Map_blink_timer == 0)
            {
                pMap_scr->Hilite_mode = 2;
            }
        }
        break;
    case 7:
        Map_spr->w = 0xA1;

        Map_spr[2].w = 0x51;

        Map_spr[1].w = 0x51;
        Map_spr[0].scalex = Map_spr[1].scalex = Map_spr[2].scalex = 0xFFF;
        Map_spr[1].y = 0;
        Map_spr[2].y = 0;

        Map_spr[0].x = 0;
        Map_spr[1].x = 0;
        Map_spr[2].x = 0;

        Map_spr[0].attribute &= ~0x10000000;
        pMap_scr->Arrow_anm = 0;

        Map_spr[0].y = 2;

        if (pMap_scr->Hilite_mode != 0)
        {
            pMap_scr->Hilite_mode = 1;
        }
        pMap_scr->Zoom_mode++;
    case 8:
        Map_spr[0].scaley -= 650;
        Map_spr[1].scaley -= 650;

        Map_spr[2].scaley -= 650;

        if (Map_spr[0].scaley < 2)
        {
            Map_spr[0].scaley = Map_spr[1].scaley = Map_spr[2].scaley = 200;

            pMap_scr->Zoom_mode++;
        }
        break;
    case 9:
        Map_spr[0].scalex /= 2;
        Map_spr[1].scalex /= 2;
        Map_spr[2].scalex /= 2;
        if (Map_spr[0].scalex < 2)
        {
            Map_spr[0].scalex = Map_spr[1].scalex = Map_spr[2].scalex = 1;
            if (pMap_scr->mode & 1)
            {
                *(INT*)pMap_scr = 0x106;    // state 6, substate 1, Blink_mode and Zoom_mode cleared
            }
            else
            {
                *(INT*)pMap_scr = 0x100;    // state 0, substate 1, Blink_mode and Zoom_mode cleared
            }
        }
        break;
    }
    Map_disp_draw(pMap_scr);
}

VOID Map_disp_draw(MAP_SCR_WORK* pMap_scr)
{
    GsSPRITE* Sub_ctr;
    INT i;
    UCHAR st;

    for (i = 0; i < 3; i++)
    {
        GsSortSprite(&Map_spr[i], &MAIN.Ot1[G.Cbuf_id], 14);
    }
    if (pMap_scr->Zoom_mode == 6)
    {
        if (pMap_scr->Arrow_anm < 0xD0)
        {
            do
            {
                Map_spr[3].y = (pMap_scr->Arrow_anm >> 1) + 0x18;
                Map_spr[3].v = (pMap_scr->Arrow_anm >> 1) + 0x84;
            } while (0);
            VIEW(GsSPRITE, Map_spr[4].attribute).y = (pMap_scr->Arrow_anm >> 1) + 0x18;  // GsSPRITE* view of the field (a plain access changes the code)
            VIEW(GsSPRITE, Map_spr[4].attribute).v = (pMap_scr->Arrow_anm >> 1) + 0x88;  // GsSPRITE* view of the field (a plain access changes the code)
            VIEW(GsSPRITE, Map_spr[5].attribute).y = (pMap_scr->Arrow_anm >> 1) + 0x18;  // GsSPRITE* view of the field (a plain access changes the code)
            VIEW(GsSPRITE, Map_spr[5].attribute).v = (pMap_scr->Arrow_anm >> 1) + 0x88;  // GsSPRITE* view of the field (a plain access changes the code)
            Sub_ctr = &Map_spr[3];
            for (i = 0; i < 3; i++)
            {
                GsSortFastSprite(Sub_ctr, &MAIN.Ot1[G.Cbuf_id], 13);
                Sub_ctr++;
            }
        }
        pMap_scr->Arrow_anm++;
    }
    st = pMap_scr->Hilite_mode;
    if (st >= 2 && pMap_scr->mode != 0)
    {
        i = (pMap_scr->Hilite_frm / 3) & 7;
        switch (st)
        {
        case 2:
            Snd_se_play(SE_BANK_ENEMY, 0xA, 0);
            pMap_scr->Hilite_mode = 3;

        case 3:
            pMap_scr->Hilite_frm += 2;
            if (pMap_scr->Hilite_frm >= 0x17)
            {
                pMap_scr->Hilite_mode = 4;
                pMap_scr->Hilite_frm = 0x17;
            }
            break;
        case 4:
            if (--pMap_scr->Hilite_frm == 0)
            {
                pMap_scr->Hilite_mode = 5;
                pMap_scr->Hilite_frm = 0x14;
            }
            break;
        case 5:
            i = 0;
            if (--pMap_scr->Hilite_frm == 0)
            {
                pMap_scr->Hilite_frm = 0;
                pMap_scr->Hilite_mode = 2;
            }
            break;
        }
        if (pMap_scr->mode / 3 == 0)
        {
            Map_spr[6].x = 0x2A;
            Map_spr[6].y = 0x3A;
        }
        else
        {
            Map_spr[6].x = 0x86;
            Map_spr[6].y = 0x40;
        }
        Map_spr[6].u = i * 8;
        GsSortFastSprite(&Map_spr[6], &MAIN.Ot1[G.Cbuf_id], 12);
    }
}

VOID Map_scr_init(VOID)
{
    MAP_SCR_WORK* p = &Map_scr_work;
    UINT r;
    INT i;
    INT j;

    Map_scr_work.Room_idx[0] = 1;
    Map_scr_work.Room_idx[1] = 1;
    Map_scr_work.Room_idx[2] = 1;
    Map_scr_work.Room_idx[3] = 3;

    VIEW(INT, Map_scr_work) = 0;    // clear state, substate, Blink_mode and Zoom_mode together

    Map_scr_work.room = G.SaveData.Room_no;
    Map_scr_work.area_group = G.SaveData.Stage_no % 5;
    switch (Map_scr_work.area_group)
    {
    case 0:
        if (G.SaveData.Room_no == 7 && CUT_NO_NOW >= 3 && CUT_NO_NOW < 7)
        {
            p->area = 0;
            p->room = 0x1D;
        }
        else if (G.SaveData.Room_no == 15 && CUT_NO_NOW >= 3 && CUT_NO_NOW < 5)
        {
            p->area = 0;
            p->room = 0x1E;
        }
        else if (G.SaveData.Room_no == 16 && CUT_NO_NOW == 0)
        {
            p->area = 2;
            p->room = 0x1E;
            p->area_group = 1;
        }
        else
        {
            p->area = 0;
        }
        break;
    case 1:
        if (G.SaveData.Room_no == 15 && (G.SaveData.Cut_no == 2 || G.SaveData.Cut_no == 5))
        {
            p->area = 1;
            p->room = 0x1D;
        }
        else if (G.SaveData.Room_no == 12 && CUT_NO_NOW >= 4 && CUT_NO_NOW < 8)
        {
            p->area = 0;
            p->room = 0x1F;
            p->area_group = 0;
        }
        else if (G.SaveData.Room_no < 0x1A)
        {
            p->area = 1;
        }
        else
        {
            p->area = 2;
        }
        break;
    case 2:
        if (G.SaveData.Room_no == 11 && CUT_NO_NOW >= 5 && CUT_NO_NOW < 7)
        {
            p->area = 4;
            p->room = 0x11;
        }
        else if (G.SaveData.Room_no == 15 && CUT_NO_NOW == 6)
        {
            p->area = 4;
            p->room = 0x12;
        }
        else if (G.SaveData.Room_no < 6)
        {
            p->area = 3;
        }
        else
        {
            p->area = 4;
        }
        break;
    case 3:
        if (G.SaveData.Room_no < 13)
        {
            p->area = 5;
        }
        else
        {
            p->area = 6;
        }
        break;
    case 4:
        r = G.SaveData.Room_no;
        if (r == 7 && G.SaveData.Cut_no < 4)
        {
            p->area = 9;
            p->room = 0x16;
        }
        else if (r < 2)
        {
            p->area = 7;
        }
        else if (r < 5)
        {
            p->area = 8;
        }
        else if (r < 0x13)
        {
            p->area = 9;
        }
        else
        {
            p->area = 10;
        }
        break;
    }
    Map_mask_set(p);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (Map_floor_tbl[i][j] == p->area)
            {
                p->Room_idx[i] = j;
            }
        }
    }
    Map_hilite_set();
    if (p->mode != 0)
    {
        p->mode++;
    }

    p->layout = Map_area_tbl[p->area];

    p->Hilite_mode = 0;
}

VOID Map_mask_set(MAP_SCR_WORK* pMap_scr)
{
    UINT i;
    UINT j;
    INT flags;

    pMap_scr->Area_mask = 0;
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < Map_room_cnt_tbl[i]; j++)
        {
            if (Flg_ck(&MAIN.SaveData.Room_flg[0], j + Map_flg_base_tbl[i]) != 0)
            {
                switch (i)
                {
                case 0:
                    pMap_scr->Area_mask |= 1;
                    break;
                case 1:
                    if (j < 0x1A || j == 0x1D)
                    {
                        pMap_scr->Area_mask |= 2;
                    }
                    else
                    {
                        pMap_scr->Area_mask |= 4;
                    }
                    break;
                case 2:

                    if (j < 6 || j == 0x10)
                    {
                        pMap_scr->Area_mask |= 8;
                    }
                    else
                    {
                        pMap_scr->Area_mask |= 0x10;
                    }
                    break;
                case 3:
                    if (j < 0xD)
                    {
                        pMap_scr->Area_mask |= 0x20;
                    }
                    else
                    {
                        pMap_scr->Area_mask |= 0x40;
                    }
                    break;
                case 4:

                    if (j < 2 || j == 0x16)
                    {
                        pMap_scr->Area_mask |= 0x80;
                    }
                    else if (j < 5)
                    {
                        pMap_scr->Area_mask |= 0x100;
                    }
                    else if (j < 0x13)
                    {
                        pMap_scr->Area_mask |= 0x200;
                    }
                    else
                    {
                        pMap_scr->Area_mask |= 0x400;
                    }
                    break;
                }
            }
        }
    }
    for (i = 0; i < 11; i++)
    {
        if (Map_area_ck(i) != 0)
        {
            pMap_scr->Area_mask |= 1 << i;
        }
    }
    flags = pMap_scr->Area_mask;
    pMap_scr->Layout_mask = 0;
    for (i = 0; i < 11; i++)
    {
        if ((flags >> i) & 1)
        {
            pMap_scr->Layout_mask |= 1 << Map_area_tbl[i];
        }
    }
}

VOID St_room_set(UINT stage, INT room)
{
    Flg_on(&MAIN.SaveData.Room_flg, Map_flg_base_tbl[stage % 5] + room);  // address passed as an integer
}

VOID Map_area_set(INT Map_no)
{
    Flg_on(&MAIN.SaveData.Room_flg, Map_no + 0x7C);  // address passed as an integer
}

INT Map_area_ck(UINT a, INT n)
{
    switch (a)
    {
    case 0:
        n = 0;
        break;
    case 1:
        n = 1;
        break;
    case 3:
        n = 2;
        break;
    case 4:
        n = 3;
        break;
    case 5:
    case 6:
        n = 4;
        break;
    case 8:
    case 9:
        n = 5;
        break;
    case 2:
    case 7:
    case 10:
        return 0;
    }
    return Flg_ck(&MAIN.SaveData.Room_flg, n + 0x7C);  // address passed as an integer
}

VOID St_file_set(INT file_id)
{
    Flg_on(&MAIN.SaveData.Room_flg, file_id + 0x82);  // address passed as an integer
}

INT File_ck(UCHAR file_id)
{
    if (file_id == 0xFF)
    {
        return 0;
    }
    if (file_id == 0xFE)
    {
        return 0;
    }
    return Flg_ck(&MAIN.SaveData.Room_flg, file_id + 0x82);  // address passed as an integer
}

VOID Tim_trans(UCHAR* pTim_Adr, RECT* rect, INT y)
{
    RECT Rect;
    GsIMAGE Tim_image;

    GsGetTimInfo((ULONG*)(pTim_Adr + 4), &Tim_image);  // word view of the same memory
    LoadImage(rect, Tim_image.pixel);
    DrawSync(0);
    if ((Tim_image.pmode >> 3) & 1)
    {
        Rect.x = 0;
        Rect.y = y + 0x1E0;
        Rect.w = Tim_image.cw;
        Rect.h = Tim_image.ch;
        LoadImage(&Rect, Tim_image.clut);
    }
    DrawSync(0);
}

INT St_main03(VOID)
{
    MAP_SCR_WORK* p = &Map_scr_work;

    switch (p->state)
    {
    case 0:
        p->Time_cnt = 10;
        p->state++;
    case 1:
        if (p->Time_cnt-- != 0)
        {
            break;
        }
        p->Time_cnt = 5;
        p->state++;
        Snd_se_play(SE_BANK_ENEMY, 6, 0);
        break;
    case 2:
        Map_tab_main(p);
        if (p->Time_cnt-- != 0)
        {
            break;
        }
        p->Time_cnt = 0x14;

        p->Zoom_mode = 0;

        p->Blink_mode = 0;
        p->substate = 0;

        p->state++;
        break;
    case 3:
        Map_disp_main(p);
        if (p->Zoom_mode != 6)
        {
            break;
        }
        if (p->Time_cnt-- != 0)
        {
            break;
        }
        p->Hilite_mode = 2;
        p->state++;
        break;
    case 4:
        Map_disp_main(p);

        if ((G.SaveData.Key_trg & 0x8000) || (G.Padd1_trg & PAD_START))
        {
            p->Zoom_mode = 7;
            p->state++;
            Snd_se_play(SE_BANK_ENEMY, 5, 0);
        }
        break;
    case 5:
        Map_disp_main(p);
        break;
    case 6:
        p->Time_cnt = 5;
        p->state++;
        Snd_se_play(SE_BANK_ENEMY, 5, 0);
    case 7:
        Map_tab_main(p);
        if (p->Time_cnt-- != 0)
        {
            break;
        }
        p->Time_cnt = 5;
        p->state++;
        break;
    case 8:
        if (p->Time_cnt-- != 0)
        {
            break;
        }
        p->Exit_flg = 1;
        p->state++;
        Snd_se_play(SE_BANK_ENEMY, 5, 0);
        break;
    }
    return -(p->Exit_flg != 0);
}

VOID St_init03(VOID)
{
    UCHAR area;
    MAP_SCR_WORK* pMap_wk = &Map_scr_work;

    Map_hilite_set();
    pMap_wk->state = 0;

    if (Map_scr_work.mode == 1)
    {
        area = Map_area_tbl[0];
        Map_scr_work.area_group = 1;
        Map_scr_work.Room_idx[0] = 1;
        Map_scr_work.Room_idx[1] = 1;
        Map_scr_work.Room_idx[2] = 1;
        Map_scr_work.area = 0;
        Map_scr_work.room = 0;
        Map_scr_work.Room_idx[3] = 3;
        Map_scr_work.layout = area;
    }
    else
    {
        area = Map_area_tbl[6];
        Map_scr_work.area_group = 5;
        Map_scr_work.area = 6;
        Map_scr_work.Room_idx[0] = 1;
        Map_scr_work.Room_idx[1] = 1;
        Map_scr_work.room = 0;
        Map_scr_work.Room_idx[2] = 0;
        Map_scr_work.Room_idx[3] = 3;
        Map_scr_work.layout = area;
    }
    pMap_wk->Area_flg = 0;
    pMap_wk->Layout_mask = 0;
    pMap_wk->Arrow_anm = 0;
    pMap_wk->Hilite_mode = 0;
    pMap_wk->Hilite_timer = 0;
    pMap_wk->Time_cnt = 0;
    pMap_wk->Dummy13 = 0;
    pMap_wk->Exit_flg = 0;
    pMap_wk->Hilite_frm = 0;
    pMap_wk->Area_mask = 0;
    Map_mask_set(pMap_wk);
}

VOID Map_hilite_set(VOID)
{
    VOID* f2 = &MAIN.SaveData.Scenario_flg[0];
    VOID* f1 = &MAIN.SaveData.Scenario_flg2[0];
    MAP_SCR_WORK* q = &Map_scr_work;

    if (!(G.System_flg & SYS_PLAYER))
    {
        if (Flg_ck(&MAIN.SaveData.Scenario_flg2, 0x2D) != 0 && Flg_ck(&MAIN.SaveData.Scenario_flg, 0x49) == 0)  // address passed as an integer
        {
            Map_scr_work.mode = 1;
            return;
        }
        if (Flg_ck(f1, 0x2E) != 0 && Flg_ck(f2, 0x4A) == 0)
        {
            q->mode = 1;
            return;
        }
        if (Flg_ck(f1, 0x38) != 0 && Flg_ck(f2, 0x48) == 0)
        {
            q->mode = 3;
            return;
        }
        q->mode = 0;
    }
    else
    {
        if (Flg_ck(&MAIN.SaveData.Scenario_flg2, 0x23) != 0 && Flg_ck(&MAIN.SaveData.Scenario_flg, 0x49) == 0)  // address passed as an integer
        {
            Map_scr_work.mode = 1;
            return;
        }
        q->mode = 0;
    }
}

// disc file of the tab image of each layout
USHORT Map_tab_file_tbl[4] =
{
    MAP0D_TIM, // 0 //
    MAP0C_TIM, // 1 //
    MAP00_TIM, // 2 //
    MAP0E_TIM, // 3 //
};

// disc file of the floor map image of each area
USHORT Map_floor_file_tbl[12] =
{
    MAP01_TIM, // 0 //
    MAP02_TIM, // 1 //
    MAP03_TIM, // 2 //
    MAP04_TIM, // 3 //
    MAP05_TIM, // 4 //
    MAP0A_TIM, // 5 //
    MAP0B_TIM, // 6 //
    MAP06_TIM, // 7 //
    MAP07_TIM, // 8 //
    MAP08_TIM, // 9 //
    MAP09_TIM, // 10 //
    0x0000, // 11 //
};

// first room visited flag of each stage group
UCHAR Map_flg_base_tbl[0x8] =
{
    0x00,   // 0 //
    0x20,   // 1 //
    0x3F,   // 2 //
    0x52,   // 3 //
    0x64,   // 4 //
    0x00,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
};

// number of rooms of each stage group
UCHAR Map_room_cnt_tbl[0x8] =
{
    0x20,   // 0 //
    0x1F,   // 1 //
    0x13,   // 2 //
    0x12,   // 3 //
    0x18,   // 4 //
    0x00,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
};

// areas of each layout, one per floor (-1 = none)
SCHAR Map_floor_tbl[4][4] =
{
    { 2, 0, 1, -1 },    // 0 //
    { 4, 3, -1, -1 },   // 1 //
    { 6, 5, -1, -1 },   // 2 //
    { 10, 9, 8, 7 },    // 3 //
};

// layout of each area
UCHAR Map_area_tbl[0xC] =
{
    0x00,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x01,   // 3 //
    0x01,   // 4 //
    0x02,   // 5 //
    0x02,   // 6 //
    0x03,   // 7 //
    0x03,   // 8 //
    0x03,   // 9 //
    0x03,   // 10 //
    0x00,   // 11 //
};

// stage group of each area
UCHAR Map_stage_tbl[0xC] =
{
    0x00,   // 0 //
    0x01,   // 1 //
    0x01,   // 2 //
    0x02,   // 3 //
    0x02,   // 4 //
    0x03,   // 5 //
    0x03,   // 6 //
    0x04,   // 7 //
    0x04,   // 8 //
    0x04,   // 9 //
    0x04,   // 10 //
    0x00,   // 11 //
};

// number of floors of each layout
UCHAR Map_floor_cnt_tbl[0x4] =
{
    0x03,   // 0 //
    0x02,   // 1 //
    0x02,   // 2 //
    0x04,   // 3 //
};

// first floor label row of each layout
UCHAR Map_label_tbl[0x4] =
{
    0x04,   // 0 //
    0x04,   // 1 //
    0x04,   // 2 //
    0x01,   // 3 //
};

// browser sprites: background, 4 layout tabs, layout marker, floor marker, up and down arrows
GsSPRITE Map_tab_spr[9] =
{
    { 0x01000000, 33, 32, 160, 72, 21, 0, 0, 0, 508, 128, 128, 128, 0, 0, 0, 0, 0 },       // 0 //
    { 0x01000000, 121, 32, 56, 64, 21, 160, 0, 0, 508, 0, 0, 0, 0, 0, 0, 0, 0 },           // 1 //
    { 0x01000000, 73, 32, 80, 64, 21, 0, 72, 0, 508, 0, 0, 0, 0, 0, 0, 0, 0 },             // 2 //
    { 0x01000000, 49, 32, 40, 64, 21, 80, 72, 0, 508, 0, 0, 0, 0, 0, 0, 0, 0 },            // 3 //
    { 0x01000000, 121, 32, 28, 64, 21, 200, 72, 0, 508, 0, 0, 0, 0, 0, 0, 0, 0 },          // 4 //
    { 0x01000000, 57, 104, 80, 16, 21, 120, 72, 0, 508, 128, 128, 128, 0, 0, 0, 0, 0 },    // 5 //
    { 0x01000000, 137, 104, 24, 16, 21, 232, 96, 0, 508, 128, 128, 128, 0, 0, 0, 0, 0 },   // 6 //
    { 0x01000000, 145, 96, 8, 8, 21, 160, 64, 0, 508, 128, 128, 128, 0, 0, 0, 0, 0 },      // 7 //
    { 0x01000000, 145, 120, 8, 8, 21, 176, 64, 0, 508, 128, 128, 128, 0, 0, 0, 0, 0 },     // 8 //
};

// map sprites: 0-2 map image strips, 3-5 one-pixel-row strips animated while Zoom_mode is 6, 6 objective marker
GsSPRITE Map_spr[7] =
{
    { 0x51000000, 0, 0, 160, 100, 21, 0, 136, 0, 511, 128, 128, 128, 80, 50, 4096, 4096, 0 },    // 0 //
    { 0x41000000, 0, 0, 80, 104, 21, 160, 136, 0, 511, 128, 128, 128, 80, 52, 4096, 4096, 0 },   // 1 //
    { 0x41000000, 0, 0, 80, 104, 21, 160, 136, 0, 511, 128, 128, 128, 0, 52, 4096, 4096, 0 },    // 2 //
    { 0x51000000, 33, 24, 160, 1, 21, 0, 128, 0, 511, 128, 128, 128, 0, 0, 0, 0, 0 },            // 3 //
    { 0x01000000, 33, 24, 80, 1, 21, 160, 128, 0, 511, 128, 128, 128, 0, 0, 0, 0, 0 },           // 4 //
    { 0x01000000, 113, 24, 80, 1, 21, 160, 128, 0, 511, 128, 128, 128, 0, 0, 0, 0, 0 },          // 5 //
    { 0x01000000, 0, 0, 8, 8, 21, 0, 240, 0, 511, 128, 128, 128, 0, 0, 0, 0, 0 },                // 6 //
};
UCHAR Map_spr_tail[4] = { 0x00, 0x00, 0x00, 0x00 }; // padding: keeps the following data at its linked position
