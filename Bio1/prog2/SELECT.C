/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog2\SELECT.C
 */

#include "SELECT.H"
#include "SCRATCHPAD.H"
#include "FILECODE.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "MONITOR.H"

static SELECT_WORK Select_wk; // character select screen work area

VOID main(VOID)
{
    UCHAR buf;      // draw buffer index
    UCHAR quad;     // quad index within the portrait or frame set

    G.Vloop = 0;
    Select_wk.state.Main_no = 0;
    Select_wk.state.substate = 0;
    Select_wk.state.unused = 0;
    FUNC(SND_TITLE_INIT)();  // Snd_title_init
    G.pFree_work = OVERLAY_AREA;  // work buffer for file loads
    G.SaveData.Room_no = 0x1B;
    FUNC(SND_SYS_ARMS_SET)(0xD, OVERLAY_AREA);  // Snd_sys_arms_set

    FUNC(CD_READ)(SELECT_TIM, LOAD_BUF);  // Cd_read

    VIEW(SHORT, G.Vram_cur.b.Ctpage) = 0xA05;  // texture page and CLUT row where the next texture is placed; SHORT* view of the field (a plain access changes the code)
    FUNC(MAPPING_TIM)(LOAD_BUF);  // Mapping_tim

    FUNC(CD_READ)(SEL_BACK_PIX, LOAD_BUF);  // Cd_read

    // identity matrix for the perspective transform
    Select_wk.rot.vx = 0;
    Select_wk.rot.vy = 0;
    Select_wk.rot.vz = 0;
    Select_wk.trans.vx = 0;
    Select_wk.trans.vy = 0;
    Select_wk.trans.vz = 0;
    FUNC(ROT_MATRIX)(&Select_wk.rot, &Select_wk.matrix);  // RotMatrix
    FUNC(TRANS_MATRIX)(&Select_wk.matrix, &Select_wk.trans);  // TransMatrix
    FUNC(SET_ROT_MATRIX)(&Select_wk.matrix);  // SetRotMatrix
    FUNC(SET_TRANS_MATRIX)(&Select_wk.matrix);  // SetTransMatrix
    FUNC(SET_GEOM_SCREEN)(0xF0);  // SetGeomScreen
    // initialise the portrait and frame quads of both characters for both draw buffers
    buf = 2;
    do
    {
        buf--;
        quad = 2;
        do
        {
            quad--;
            FUNC(SET_POLY_FT4)(&Select_wk.Pl_wk[0].a[buf][quad]);  // SetPolyFT4
            FUNC(SET_POLY_FT4)(&Select_wk.Pl_wk[1].a[buf][quad]);  // SetPolyFT4
            setUV4(&Select_wk.Pl_wk[0].a[buf][quad], Select_chris_uv_tbl[quad].x, Select_chris_uv_tbl[quad].y, Select_chris_uv_tbl[quad].x + Select_chris_uv_tbl[quad].w, Select_chris_uv_tbl[quad].y, Select_chris_uv_tbl[quad].x, Select_chris_uv_tbl[quad].y + Select_chris_uv_tbl[quad].h, Select_chris_uv_tbl[quad].x + Select_chris_uv_tbl[quad].w, Select_chris_uv_tbl[quad].y + Select_chris_uv_tbl[quad].h);
            setUV4(&Select_wk.Pl_wk[1].a[buf][quad], Select_jill_uv_tbl[quad].x, Select_jill_uv_tbl[quad].y, Select_jill_uv_tbl[quad].x + Select_jill_uv_tbl[quad].w, Select_jill_uv_tbl[quad].y, Select_jill_uv_tbl[quad].x, Select_jill_uv_tbl[quad].y + Select_jill_uv_tbl[quad].h, Select_jill_uv_tbl[quad].x + Select_jill_uv_tbl[quad].w, Select_jill_uv_tbl[quad].y + Select_jill_uv_tbl[quad].h);
            Select_wk.Pl_wk[0].a[buf][quad].clut = FUNC(GET_CLUT)(0, quad + 0x1EA);  // GetClut
            Select_wk.Pl_wk[1].a[buf][quad].clut = FUNC(GET_CLUT)(0, quad * 2 + 0x1EA);  // GetClut
            Select_wk.Pl_wk[0].a[buf][quad].tpage = FUNC(GET_TPAGE)(1, 0, 0x140, 0);  // GetTPage
            Select_wk.Pl_wk[1].a[buf][quad].tpage = FUNC(GET_TPAGE)(1, 0, 0x140, 0);  // GetTPage
            FUNC(SET_SEMI_TRANS)(&Select_wk.Pl_wk[0].a[buf][quad], 0);  // SetSemiTrans
            FUNC(SET_SEMI_TRANS)(&Select_wk.Pl_wk[1].a[buf][quad], 0);  // SetSemiTrans
        } while (quad != 0);
        quad = 5;
        do
        {
            quad--;
            FUNC(SET_POLY_FT4)(&Select_wk.Pl_wk[0].b[buf][quad]);  // SetPolyFT4
            FUNC(SET_POLY_FT4)(&Select_wk.Pl_wk[1].b[buf][quad]);  // SetPolyFT4
            setUV4(&Select_wk.Pl_wk[0].b[buf][quad], Select_frm_uv_tbl[quad].x, Select_frm_uv_tbl[quad].y, Select_frm_uv_tbl[quad].x + Select_frm_uv_tbl[quad].w, Select_frm_uv_tbl[quad].y, Select_frm_uv_tbl[quad].x, Select_frm_uv_tbl[quad].y + Select_frm_uv_tbl[quad].h, Select_frm_uv_tbl[quad].x + Select_frm_uv_tbl[quad].w, Select_frm_uv_tbl[quad].y + Select_frm_uv_tbl[quad].h);
            setUV4(&Select_wk.Pl_wk[1].b[buf][quad], Select_frm_uv_tbl[quad].x, Select_frm_uv_tbl[quad].y, Select_frm_uv_tbl[quad].x + Select_frm_uv_tbl[quad].w, Select_frm_uv_tbl[quad].y, Select_frm_uv_tbl[quad].x, Select_frm_uv_tbl[quad].y + Select_frm_uv_tbl[quad].h, Select_frm_uv_tbl[quad].x + Select_frm_uv_tbl[quad].w, Select_frm_uv_tbl[quad].y + Select_frm_uv_tbl[quad].h);
            Select_wk.Pl_wk[0].b[buf][quad].clut = FUNC(GET_CLUT)(0, 0x1EA);  // GetClut
            Select_wk.Pl_wk[1].b[buf][quad].clut = FUNC(GET_CLUT)(0, 0x1EA);  // GetClut
            Select_wk.Pl_wk[0].b[buf][quad].tpage = FUNC(GET_TPAGE)(1, 2, 0x140, 0);  // GetTPage
            Select_wk.Pl_wk[1].b[buf][quad].tpage = FUNC(GET_TPAGE)(1, 2, 0x140, 0);  // GetTPage
            setRGB0(&Select_wk.Pl_wk[0].b[buf][quad], 0x40, 0x40, 0x40);
            setRGB0(&Select_wk.Pl_wk[1].b[buf][quad], 0x40, 0x40, 0x40);
            FUNC(SET_SEMI_TRANS)(&Select_wk.Pl_wk[0].b[buf][quad], 1);  // SetSemiTrans
            FUNC(SET_SEMI_TRANS)(&Select_wk.Pl_wk[1].b[buf][quad], 1);  // SetSemiTrans
        } while (quad != 0);
    } while (buf != 0);
    Select_wk.character = 0;
    // panel 0 (Chris) starts in front and bright, panel 1 (Jill) behind and dimmed
    Select_wk.Pl_wk[0].Ot_no = 0;
    Select_wk.Pl_wk[1].Ot_no = 1;
    Select_wk.Pl_wk[0].Kido = 0x80;
    Select_wk.Pl_wk[1].Kido = 0x50;
    Select_wk.Pl_wk[0].pos_x = 0x88;
    Select_wk.Pl_wk[0].pos_y = 0x48;
    Select_wk.Pl_wk[0].pos_z = 0xF0;
    Select_wk.Pl_wk[1].pos_x = 0xC8;
    Select_wk.Pl_wk[1].pos_y = 0x68;
    Select_wk.Pl_wk[1].pos_z = 0x110;
    G.System_flg = (G.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_IMAGE;
    G.Fade_hrate = 2;
    G.Fade_add = -0x800;
    FUNC(FADE_START)();  // Evt_fade_start
    do
    {
        // sprite descriptor in the scratchpad used by the arrow sprites
        ScratchPad->Spr.attribute = 0x01000040;
        ScratchPad->Spr.tpage = 5;
        ScratchPad->Spr.cx = 0;
        ScratchPad->Spr.r = 0x80;
        ScratchPad->Spr.g = 0x80;
        ScratchPad->Spr.b = 0x80;
        ScratchPad->Spr.mx = 0;
        ScratchPad->Spr.my = 0;
        ScratchPad->Spr.rotate = 0;
        switch (Select_wk.state.Main_no)
        {
        case 0:  // wait for the fade in to finish
            if (G.SaveData.Fade_kido >= 0)
            {
                break;
            }
            Select_wk.state.Main_no = 1;
            Select_wk.timer = 0;
        case 1:  // idle: wait for an input
            if ((G.Padd & 4) && (G.Padd & 8))  // L1 and R1 held: back out
            {
                Select_wk.state.Main_no = 7;
                Select_wk.state.substate = 0;
                G.System_flg = (G.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;
                FUNC(FADE_REQ_SET)(2, 0xC00);  // Evt_fade_req_set
                break;
            }

            if (G.Padd_trg & (PAD_TRIANGLE | PAD_CIRCLE | PAD_X | PAD_SQUARE | PAD_START))  // start or a face button: confirm
            {
                FUNC(SND_SE_PLAY)(1, 1, 0);  // Snd_se_play
                Select_wk.state.Main_no = 3;
                Select_wk.state.substate = 0;
                G.Fade_hrate = 1;
                G.Fade_add = 0x100;
                FUNC(FADE_START)();  // Evt_fade_start
                break;
            }

            if (!(G.Padd & (PAD_SELECT | PAD_RIGHT | PAD_LEFT)))  // none of left, right, select held
            {
                Select_wk.timer--;
                Select_arrow_disp();

                break;
            }
            Select_wk.state.Main_no = 2;
            Select_wk.state.substate = 0;
            Select_wk.Left_flg = (G.Padd >> 0xF) & 1;  // 1 when left is held
            FUNC(SND_SE_PLAY)(1, 0, 0);  // Snd_se_play
            if (Select_wk.Left_flg != Select_wk.character)
            {
                Select_wk.Pl_wk[0].Add_x = -2;
                Select_wk.Pl_wk[0].accel_y = -1;
            }
            else
            {
                Select_wk.Pl_wk[0].Add_x = 2;
                Select_wk.Pl_wk[0].accel_y = 1;
            }
            Select_wk.Pl_wk[1].Add_x = -Select_wk.Pl_wk[0].Add_x;
            Select_wk.Pl_wk[1].accel_y = -Select_wk.Pl_wk[0].accel_y;
            Select_wk.Pl_wk[0].vel_x = 0;
            Select_wk.Pl_wk[0].vel_y = 0;
            Select_wk.Pl_wk[1].vel_x = 0;
            Select_wk.Pl_wk[1].vel_y = 0;
            Select_wk.timer = 8;
        case 2:  // swap animation: the Pl_wk trade places and depth
            Select_wk.timer--;
            switch (Select_wk.state.substate)
            {
            case 0:
                if (Select_wk.timer != 0)
                {
                    goto scroll;
                }
                Select_wk.state.substate = 1;
                Select_wk.timer = (Select_wk.Left_flg ^ 1) << 2;
                Select_wk.Pl_wk[0].vel_x += Select_wk.Pl_wk[0].Add_x;
                Select_wk.Pl_wk[0].vel_y += Select_wk.Pl_wk[0].accel_y;
                Select_wk.Pl_wk[1].vel_x += Select_wk.Pl_wk[1].Add_x;
                Select_wk.Pl_wk[1].vel_y += Select_wk.Pl_wk[1].accel_y;
            case 1:
                if (Select_wk.timer != 0)
                {
                    break;
                }
                Select_wk.state.substate = 2;
                Select_wk.timer = 0xF;
                Select_wk.Pl_wk[0].Add_x = -Select_wk.Pl_wk[0].Add_x;
                Select_wk.Pl_wk[0].accel_y = -Select_wk.Pl_wk[0].accel_y;
                Select_wk.Pl_wk[1].Add_x = -Select_wk.Pl_wk[1].Add_x;
                Select_wk.Pl_wk[1].accel_y = -Select_wk.Pl_wk[1].accel_y;
            case 2:
                if (Select_wk.Pl_wk[0].vel_x == 0)
                {
                    Select_wk.state.substate = 3;
                    Select_wk.Pl_wk[0].Ot_no ^= 1;
                    Select_wk.Pl_wk[1].Ot_no ^= 1;
                }
            case 3:
                Select_wk.Pl_wk[Select_wk.character].pos_z += 2;
                Select_wk.Pl_wk[Select_wk.character ^ 1].pos_z -= 2;
                Select_wk.Pl_wk[Select_wk.character].Kido -= 3;
                Select_wk.Pl_wk[Select_wk.character ^ 1].Kido += 3;
                if (Select_wk.timer != 0)
                {
                    goto scroll;
                }
                Select_wk.state.substate = 4;
                Select_wk.timer = Select_wk.Left_flg << 2;
                Select_wk.Pl_wk[0].vel_x += Select_wk.Pl_wk[0].Add_x;
                Select_wk.Pl_wk[0].vel_y += Select_wk.Pl_wk[0].accel_y;
                Select_wk.Pl_wk[1].vel_x += Select_wk.Pl_wk[1].Add_x;
                Select_wk.Pl_wk[1].vel_y += Select_wk.Pl_wk[1].accel_y;
            case 4:
                if (Select_wk.timer != 0)
                {
                    break;
                }
                Select_wk.state.substate = 5;
                Select_wk.timer = 7;
                Select_wk.Pl_wk[0].Add_x = -Select_wk.Pl_wk[0].Add_x;
                Select_wk.Pl_wk[0].accel_y = -Select_wk.Pl_wk[0].accel_y;
                Select_wk.Pl_wk[1].Add_x = -Select_wk.Pl_wk[1].Add_x;
                Select_wk.Pl_wk[1].accel_y = -Select_wk.Pl_wk[1].accel_y;
            case 5:
                if (Select_wk.timer == 0)
                {
                    Select_wk.state.Main_no = 1;
                    Select_wk.character ^= 1;
                }
            default:
            scroll:  // keep accelerating the Pl_wk
                Select_scrl_add();
            }
            Select_scrl_move();
            break;
        case 3:  // wait for the fade to finish, then start the fade out
            if (G.SaveData.Fade_kido >= 0)
            {
                break;
            }
            Select_wk.state.Main_no = 4;
            G.Fade_hrate = 1;
            G.Fade_add = -0x100;
            FUNC(FADE_START)();  // Evt_fade_start
            G.System_flg = (G.System_flg & ~SYS_BG_MODE_MASK) | SYS_BG_MODE_CLEAR;
            break;
        case 4:  // wait for the fade out to finish
            if (G.SaveData.Fade_kido >= 0)
            {
                break;
            }
            Select_wk.state.Main_no = 5;
            Select_wk.timer = 0xF0;
            break;
        case 5:  // hold for 0xF0 frames
            Select_wk.timer--;
            if (Select_wk.timer != 0)
            {
                break;
            }
        case 6:  // store the chosen character and chain to the next program
            G.SaveData.Pl_id = Select_wk.character;
            if (Select_wk.character != 0)
            {
                G.System_flg |= SYS_PLAYER;  // Jill selected
            }
            G.Vloop = 2;
            FUNC(SND_SYS_END)();  // Snd_sys_end

            FUNC(TASK_LCHAIN)(PROLOGUE_EXE);  // Task_lchain

            break;
        case 7:  // wait for the fade out to finish, then chain back
            if (!(G.System_flg & SYS_FADE_OUT))
            {
                G.Vloop = 2;
                FUNC(SND_SYS_END)();  // Snd_sys_end

                FUNC(TASK_LCHAIN)(TITLE_EXE);  // Task_lchain
            }
        }
        if (Select_wk.state.Main_no < 4)
        {
            Select_pl_disp();
        }
        FUNC(TASK_SLEEP)(1);  // Task_sleep
    } while (1);
}

VOID Select_pl_disp(VOID)
{
    UCHAR Pl_no;
    UCHAR quad;
    UCHAR Ofs;

    Pl_no = 2;
    do
    {
        Pl_no--;
        Select_wk.Quad_sv[0].vx = Select_wk.Pl_wk[Pl_no].pos_x - 0x60;
        Select_wk.Quad_sv[0].vy = Select_wk.Pl_wk[Pl_no].pos_y - 0x20;
        Select_wk.Quad_sv[0].vz = Select_wk.Pl_wk[Pl_no].pos_z;
        Select_wk.Quad_sv[1].vx = Select_wk.Quad_sv[0].vx + 0x50;
        Select_wk.Quad_sv[1].vy = Select_wk.Quad_sv[0].vy;
        Select_wk.Quad_sv[1].vz = Select_wk.Quad_sv[0].vz;
        Select_wk.Quad_sv[2].vx = Select_wk.Quad_sv[0].vx;
        Select_wk.Quad_sv[2].vy = Select_wk.Quad_sv[0].vy + 0x7F;
        Select_wk.Quad_sv[2].vz = Select_wk.Quad_sv[0].vz;
        Select_wk.Quad_sv[3].vx = Select_wk.Quad_sv[1].vx;
        Select_wk.Quad_sv[3].vy = Select_wk.Quad_sv[2].vy;
        Select_wk.Quad_sv[3].vz = Select_wk.Quad_sv[0].vz;
        FUNC(ROT_TRANS_PERS4)(&Select_wk.Quad_sv[0], &Select_wk.Quad_sv[1], &Select_wk.Quad_sv[2], &Select_wk.Quad_sv[3], &Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][0].x0, &Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][0].x1, &Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][0].x2, &Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][0].x3, &Select_wk.Rtp_p, &Select_wk.Rtp_flg);  // RotTransPers4
        Select_wk.Quad_sv[0].vx = Select_wk.Quad_sv[1].vx;
        Select_wk.Quad_sv[1].vx = Select_wk.Quad_sv[0].vx + 0x70;
        Select_wk.Quad_sv[2].vx = Select_wk.Quad_sv[0].vx;
        Select_wk.Quad_sv[3].vx = Select_wk.Quad_sv[1].vx;
        FUNC(ROT_TRANS_PERS4)(&Select_wk.Quad_sv[0], &Select_wk.Quad_sv[1], &Select_wk.Quad_sv[2], &Select_wk.Quad_sv[3], &Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][1].x0, &Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][1].x1, &Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][1].x2, &Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][1].x3, &Select_wk.Rtp_p, &Select_wk.Rtp_flg);  // RotTransPers4
        quad = 2;
        do
        {
            quad--;
            setRGB0(&Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][quad], Select_wk.Pl_wk[Pl_no].Kido, Select_wk.Pl_wk[Pl_no].Kido, Select_wk.Pl_wk[Pl_no].Kido);
            Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][quad].x0 -= 0xA0;
            Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][quad].y0 -= 0x78;
            Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][quad].x1 -= 0xA0;
            Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][quad].y1 -= 0x78;
            Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][quad].x2 -= 0xA0;
            Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][quad].y2 -= 0x78;
            Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][quad].x3 -= 0xA0;
            Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][quad].y3 -= 0x78;
            FUNC(ADD_PRIM)(OT0_CUR[G.Cbuf_id].org + (Select_wk.Pl_wk[Pl_no].Ot_no + 4), &Select_wk.Pl_wk[Pl_no].a[G.Cbuf_id][quad]);
        } while (quad != 0);
        Ofs = (Select_wk.Pl_wk[Pl_no].pos_z - 0xF0) >> 3;
        Select_wk.Quad_sv[0].vx = Select_wk.Pl_wk[Pl_no].pos_x - 0x57;
        Select_wk.Quad_sv[0].vx -= Ofs;
        Select_wk.Quad_sv[0].vy = Select_wk.Pl_wk[Pl_no].pos_y + 0x5C;
        Select_wk.Quad_sv[0].vy -= Ofs;
        Select_wk.Quad_sv[0].vz = Select_wk.Pl_wk[Pl_no].pos_z;
        Select_wk.Quad_sv[1].vx = Select_wk.Quad_sv[0].vx + 0xC;
        Select_wk.Quad_sv[1].vy = Select_wk.Quad_sv[0].vy;
        Select_wk.Quad_sv[1].vz = Select_wk.Quad_sv[0].vz;
        Select_wk.Quad_sv[2].vx = Select_wk.Quad_sv[0].vx;
        Select_wk.Quad_sv[2].vy = Select_wk.Quad_sv[0].vy + 0xC;
        Select_wk.Quad_sv[2].vz = Select_wk.Quad_sv[0].vz;
        Select_wk.Quad_sv[3].vx = Select_wk.Quad_sv[1].vx;
        Select_wk.Quad_sv[3].vy = Select_wk.Quad_sv[2].vy;
        Select_wk.Quad_sv[3].vz = Select_wk.Quad_sv[0].vz;
        FUNC(ROT_TRANS_PERS4)(&Select_wk.Quad_sv[0], &Select_wk.Quad_sv[1], &Select_wk.Quad_sv[2], &Select_wk.Quad_sv[3], &Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][0].x0, &Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][0].x1, &Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][0].x2, &Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][0].x3, &Select_wk.Rtp_p, &Select_wk.Rtp_flg);  // RotTransPers4
        Select_wk.Quad_sv[0].vx += 0xB4;
        Select_wk.Quad_sv[1].vx += 0xB4;
        Select_wk.Quad_sv[2].vx += 0xB4;
        Select_wk.Quad_sv[3].vx += 0xB4;
        FUNC(ROT_TRANS_PERS4)(&Select_wk.Quad_sv[0], &Select_wk.Quad_sv[1], &Select_wk.Quad_sv[2], &Select_wk.Quad_sv[3], &Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][1].x0, &Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][1].x1, &Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][1].x2, &Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][1].x3, &Select_wk.Rtp_p, &Select_wk.Rtp_flg);  // RotTransPers4
        Select_wk.Quad_sv[0].vy -= 0x73;
        Select_wk.Quad_sv[1].vy -= 0x73;
        Select_wk.Quad_sv[2].vy -= 0x73;
        Select_wk.Quad_sv[3].vy -= 0x73;
        FUNC(ROT_TRANS_PERS4)(&Select_wk.Quad_sv[0], &Select_wk.Quad_sv[1], &Select_wk.Quad_sv[2], &Select_wk.Quad_sv[3], &Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][2].x0, &Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][2].x1, &Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][2].x2, &Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][2].x3, &Select_wk.Rtp_p, &Select_wk.Rtp_flg);  // RotTransPers4
        Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][3].x0 = Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][0].x1;
        Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][3].y0 = Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][0].y1;
        Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][3].x1 = Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][1].x0;
        Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][3].y1 = Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][1].y0;
        Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][3].x2 = Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][0].x3;
        Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][3].y2 = Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][0].y3;
        Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][3].x3 = Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][1].x2;
        Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][3].y3 = Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][1].y2;
        Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][4].x0 = Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][2].x2;
        Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][4].y0 = Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][2].y2;
        Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][4].x1 = Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][2].x3;
        Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][4].y1 = Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][2].y3;
        Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][4].x2 = Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][1].x0;
        Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][4].y2 = Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][1].y0;
        Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][4].x3 = Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][1].x1;
        Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][4].y3 = Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][1].y1;
        quad = 5;
        do
        {
            quad--;
            Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][quad].x0 -= 0xA0;
            Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][quad].y0 -= 0x78;
            Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][quad].x1 -= 0xA0;
            Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][quad].y1 -= 0x78;
            Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][quad].x2 -= 0xA0;
            Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][quad].y2 -= 0x78;
            Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][quad].x3 -= 0xA0;
            Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][quad].y3 -= 0x78;
            FUNC(ADD_PRIM)(OT0_CUR[G.Cbuf_id].org + (Select_wk.Pl_wk[Pl_no].Ot_no + 4), &Select_wk.Pl_wk[Pl_no].b[G.Cbuf_id][quad]);
        } while (quad != 0);
    } while (Pl_no != 0);
}

VOID Select_arrow_disp(VOID)
{
    ScratchPad->Spr.x = -0x88;
    ScratchPad->Spr.y = -0x16;
    ScratchPad->Spr.u = 0xC0;
    ScratchPad->Spr.v = 0x30;
    ScratchPad->Spr.w = 0xC;
    ScratchPad->Spr.h = 0xB;
    ScratchPad->Spr.cy = 0x1EA;
    if (!(Select_wk.timer & 0x30))
    {
        ScratchPad->Spr.v = ScratchPad->Spr.v + 0x20;
    }
    FUNC(GS_SORT_FLIP_SPRITE)(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 4);  // GsSortFlipSprite
    ScratchPad->Spr.x = 0x4C;
    ScratchPad->Spr.v = ScratchPad->Spr.v + 0x10;
    FUNC(GS_SORT_FLIP_SPRITE)(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 4);  // GsSortFlipSprite
}

VOID Select_scrl_move(VOID)
{
    Select_wk.Pl_wk[0].pos_x += Select_wk.Pl_wk[0].vel_x;
    Select_wk.Pl_wk[0].pos_y += Select_wk.Pl_wk[0].vel_y;
    Select_wk.Pl_wk[1].pos_x += Select_wk.Pl_wk[1].vel_x;
    Select_wk.Pl_wk[1].pos_y += Select_wk.Pl_wk[1].vel_y;
}

VOID Select_scrl_add(VOID)
{
    Select_wk.Pl_wk[0].vel_x += Select_wk.Pl_wk[0].Add_x;
    Select_wk.Pl_wk[0].vel_y += Select_wk.Pl_wk[0].accel_y;
    Select_wk.Pl_wk[1].vel_x += Select_wk.Pl_wk[1].Add_x;
    Select_wk.Pl_wk[1].vel_y += Select_wk.Pl_wk[1].accel_y;
}

SELECT_UV_TBL Select_chris_uv_tbl[2] = { { 0x00, 0x00, 0x50, 0x7F }, { 0x50, 0x00, 0x70, 0x7F } }; // Chris portrait: left and right halves
SELECT_UV_TBL Select_jill_uv_tbl[2] = { { 0x00, 0x00, 0x50, 0x7F }, { 0x00, 0x80, 0x70, 0x7F } }; // Jill portrait: left and right halves

// frame pieces drawn behind each portrait: three Vtx and two edge strips
SELECT_UV_TBL Select_frm_uv_tbl[5] =
{
    { 0xC0, 0x20, 0x0C, 0x0C },   // 0 //
    { 0xD0, 0x20, 0x0C, 0x0C },   // 1 //
    { 0xD0, 0x10, 0x0C, 0x0C },   // 2 //
    { 0xD0, 0x20, 0x01, 0x01 },   // 3 //
    { 0xD0, 0x20, 0x01, 0x01 },   // 4 //
};
