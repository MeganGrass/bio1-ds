/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ITEM.C
 */

#include "TYPES.H"
#include "ITEM.H"
#include "GAME.H"
#include "INVENTORY.H"
#include "MOJIDISP.H"
#include "SCE1.H"
#include "SCE_AT.H"
#include "ST_SUB00.H"
#include "ST_SUB03.H"
#include "SUB.H"
#include "FILECODE.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "SCENARIO.H"

// functions without a C definition
INT Mess_set();

GsF_LIGHT Item_light[3];    // flat lights of the item viewer, faded with the zoom
STATUS St;         // work area of the status menu and the item viewer

INT Item_view_main(VOID)
{
    UINT* pPkt;
    VOID* tmd;
    UCHAR n;
    UCHAR k;
    UCHAR num;
    ITEM_WORK* pSlot;
    UCHAR id;
    UCHAR* pCoord;

    switch (St.View_rno)
    {
    case 0:
        St.View_rno = 1;
        GsMapModelingData(ITEM_VIEW_TMD);
        tmd = ITEM_VIEW_DATA;
        St.nView_obj = *(INT*)ITEM_VIEW_NOBJ;
        pCoord = (UCHAR*)&St.Coord[0];  // byte view of the same memory
        GsInitCoordinate2(0, &St.Coord[2]);
        pPkt = (UINT*)PL_PAC_BUF;  // word view of the same memory
        GsInitCoordinate2(&St.Coord[2], &St.Coord[1]);
        GsInitCoordinate2(&St.Coord[1], (GsCOORDINATE2*)pCoord);  // pCoord is a byte pointer to the coordinate block St.Coord[0]
        n = St.nView_obj;
        k = 2 - n;
        // the nested do/while (0) wrappers are kept: they change the generated loop structure
        do
        {
            do
            {
                n--;
                GsLinkObject5((ULONG)tmd, &St.Obj[k], n);
                St.Obj[k].attribute = 0;
                St.Obj[k].coord2 = &St.Coord[n];
                pPkt = (UINT*)GsPresetObject(&St.Obj[k], (ULONG*)pPkt);  // word view of the same memory
                do
                {
                    do
                    {
                        St.Obj_rot[k].vx = 0;
                        St.Obj_rot[k].vy = 0;
                        St.Obj_rot[k].vz = 0;
                        St.Coord[k].coord.t[0] = 0;
                        St.Coord[k].coord.t[1] = 0;
                        St.Coord[k].coord.t[2] = 0;
                    } while (0);
                } while (0);
                k++;
            } while (0);
        } while (n != 0);
        St.Rot.vx = 0;
        St.Rot.vy = 0;
        St.Rot.vz = 0;
        St.Coord[2].coord.t[0] = 0xFFFF5100;
        St.Coord[2].coord.t[1] = 0;
        St.Coord[2].coord.t[2] = 0;
        St.Spin_x = 0;
        St.Spin_y = 0;
        St.Spin_z = 0;
        RotMatrix(&St.Rot, &St.Coord[2].coord);
        St.Timer = 0x40;
        // falls through into the zoom-in step
    case 1:
        St.Coord[2].coord.t[0] += 0x322;
        St.Rot.vy += 0xC0;
        St.Timer--;
        St.Rot.vz += 0x80;
        if (St.Timer == 0)
        {
            switch (St.St_type)
            {
            case 0:
                St.View_rno = 2;
                St.View_act = 0;
                break;
            case 3:
                St.View_rno = 3;
                id = St.Item_id;
                SEL_ID_CUR = id;
                if (G.SaveData.Item_num < St.nSlot)
                {
                    Mess_set(0xC0, 0);
                    break;
                }
                if ((UINT)(id - 0xB) < 8 || MAIN.SaveData.Sel_id == 0x2F)  // unsigned conversion (signed operand)
                {
                    num = pAot_data->pRec->Item_num;
                    if (MAIN.SaveData.Sel_id == 0x2F)
                    {
                        num = 2;
                        if (G.Status_flg & STAT_ARRANGE)
                        {
                            num = 3;
                        }
                    }
                    // Status_flg is read as a signed INT here
                    if ((G.Status_flg & STAT_EASY_MODE) || (STATUS_FLG_NOW & STAT_ARRANGE_EASY))
                    {
                        num *= 2;
                    }
                    k = 0;
                    n = St.nSlot;
                    do
                    {
                        pSlot = &pItem_work_base[k];
                        n--;
                        if (pSlot->Id == G.SaveData.Sel_id && (UINT)(num + pSlot->Num) < 0xFB)  // unsigned conversion (signed operand)
                        {
                            Mess_set(0xC0, 0);
                            break;
                        }
                        k++;
                    } while (n != 0);
                }
                Mess_set(0xC2, 0);
                break;
            case 4:
                St.View_rno = 3;
                G.SaveData.Sel_id = St.Item_id;
                Mess_set(0xC1, 0);
                break;
            case 6:
                St.View_rno = 4;
                Mess_set(0xC7, 0);
                break;
            }
        }
        G.Clr_b = 0x20 - (St.Timer >> 1);
        n = (0x40 - St.Timer) * 4 - 1;
        Item_light[0].r = n;
        Item_light[0].g = n;
        Item_light[0].b = n;
        Item_light[1].r = n;
        Item_light[1].g = n;
        Item_light[1].b = n;
        Item_light[2].r = n;
        Item_light[2].g = n;
        Item_light[2].b = n;
        GsSetFlatLight(0, &Item_light[0]);
        GsSetFlatLight(1, &Item_light[1]);
        GsSetFlatLight(2, &Item_light[2]);
        break;
    case 2:
        Item_act_tbl[St.View_act]();
        break;
    case 4:
        St.Rot.vy -= 0x10;
        // falls through into the message wait
    case 3:
        if (G.SaveData.Mess_flg & MESS_FLG_ACTIVE)
        {
            if (St.St_type != 4 && *G.Mess_end == 8 && (G.Padd1_trg & (PAD_RIGHT | PAD_LEFT)))
            {
                Snd_se_play(SE_BANK_ENEMY, 4, 0);
            }
        }
        else
        {
            St.View_rno = 5;
            St.Timer = 0x40;
            if (St.St_type == 4)
            {
                Mess_item_get();
            }
            else if (G.SaveData.Mess_flg & MESS_FLG_ANSWER)
            {
                Snd_se_play(SE_BANK_ENEMY, 5, 0);
            }
            else
            {
                Snd_se_play(SE_BANK_ENEMY, 6, 0);
            }
        }
        break;
    case 5:
        St.Timer--;
        if (St.Timer == 0)
        {
            return 1;
        }
        St.Coord[2].coord.t[0] -= 0x322;
        St.Rot.vy -= 0xC0;
        n = St.Timer * 4 - 1;
        St.Rot.vz -= 0x80;
        G.Clr_b = St.Timer >> 1;
        Item_light[0].r = n;
        Item_light[0].g = n;
        Item_light[0].b = n;
        Item_light[1].r = n;
        Item_light[1].g = n;
        Item_light[1].b = n;
        Item_light[2].r = n;
        Item_light[2].g = n;
        Item_light[2].b = n;
        GsSetFlatLight(0, &Item_light[0]);
        GsSetFlatLight(1, &Item_light[1]);
        GsSetFlatLight(2, &Item_light[2]);
        break;
    }
    if (St.View_rno != 0)
    {
        Item_model_disp();
    }
    if (St.St_type == 0 && St.View_act == 0)
    {
        Name_disp(St.Item_id, 0, (USHORT)(0x30 - Scr_ofs[0]), (USHORT)(0xBA - Scr_ofs[1]));  // truncates to an unsigned halfword
    }
    return 0;
}

VOID Item_act_disp(VOID)
{
    INT zoom;
    INT zoom2;

    if (G.SaveData.Key_trg & 0x4000)
    {
        if (Item_exam_ck())
        {
            if (St.View_act == 1)
            {
                Name_disp(St.Item_id, 0, (0x30 - (USHORT)Scr_ofs[0]) & 0xFFFF, (0xBA - (USHORT)Scr_ofs[1]) & 0xFFFF);  // truncates to an unsigned halfword
            }
        }
        else
        {
            St.View_act = 2;
            Item_mess_set((St.Item_id - 1) & 0xFFFF, 0);
        }
    }
    else
    {
        if (G.SaveData.Key_trg & 0x8000)
        {
            St.View_rno = 5;
            St.Timer = 0x40;
            St.Spin_y = 0;
            goto block_28;
        }
        if (G.SaveData.Key & 0x3000)
        {
            if (G.SaveData.Key & 0x1000)
            {
                zoom = St.Coord[2].coord.t[0] + 0x64;
                St.Coord[2].coord.t[0] = zoom;
                if (zoom >= 0x2921)
                {
                    St.Coord[2].coord.t[0] = 0x2920;
                }
            }
            else
            {
                zoom2 = St.Coord[2].coord.t[0] - 0x64;
                St.Coord[2].coord.t[0] = zoom2;
                if (zoom2 < 0x1980)
                {
                    St.Coord[2].coord.t[0] = 0x1980;
                }
            }
        }
        if (G.SaveData.Key & 0xC00)
        {
            if (G.SaveData.Key & 0x800)
            {
                St.Spin_x = 0x20;
            }
            else
            {
                St.Spin_x = -0x20;
            }
        }
        else
        {
            St.Spin_x = 0;
        }
        if (G.Padd1 & (PAD_RIGHT | PAD_LEFT))
        {
            if (G.Padd1 & PAD_LEFT)
            {
                St.Spin_y = 0x20;
            }
            else
            {
                St.Spin_y = -0x20;
            }
        }
        else
        {
            St.Spin_y = 0;
        }
        if (G.Padd1 & (PAD_UP | PAD_DOWN))
        {
            if (G.Padd1 & PAD_UP)
            {
                St.Spin_z = -0x20;
            }
            else
            {
                St.Spin_z = 0x20;
            }
        }
        else
        {
        block_28:
            St.Spin_z = 0;
        }
        if ((G.Padd1 & (PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT)) || (G.SaveData.Key & 0xC00))
        {
            Item_rot_set();
        }
        Item_arrow_disp();
    }
}

VOID Item_act_spin(VOID)
{
    UCHAR* p;
    UCHAR s;
    SHORT t;

    p = &St.Timer;
    (*p)++;
    s = *p * 4;
    if (s > 0x20)
    {
        s = 0x20;
    }
    t = (St.Obj_rot[1].vy + s) & 0xFFF;
    St.Obj_rot[1].vy = t;
    St.Obj_rot[0].vy = (St.Obj_rot[0].vy - s * 2) & 0xFFF;
    if (t >= 0x301)
    {
        St.View_act = 2;
        Mess_set(Item_exam_mess_tbl[Item_data_tbl.items[St.Item_id - 1].Item_attribute], 0);
    }
    else
    {
        Name_disp(St.Item_id, 0, (0x30 - (USHORT)Scr_ofs[0]) & 0xFFFF, (0xBA - (USHORT)Scr_ofs[1]) & 0xFFFF);  // truncates to an unsigned halfword
    }
}

VOID Item_act_wait(VOID)
{
    INT item_id;

    if (!(G.SaveData.Mess_flg & MESS_FLG_ACTIVE))
    {
        if (St.Item_id != 0x3E)
        {
            if ((item_id = St.Item_id) >= 0x3E && item_id < 0x41 && St.Obj_rot[1].vy != 0)
            {
                St.View_act = 3;
                St.Timer = 0x40;
                return;
            }
            goto skip_tail;
        }
        if (St.Obj_rot[1].vy != 0)
        {
            Flg_on(&MAIN.SaveData.Item_c_flg, 0xD);  // address passed as an integer
        }
    skip_tail:
        St.View_act = 0;
    }
}

VOID Item_act_medal(VOID)
{
    UCHAR v;
    UCHAR idx;
    UCHAR* Loop_ctr;

    Loop_ctr = &St.Timer;
    (*Loop_ctr)--;
    St.Coord[2].coord.t[0] -= 0x322;
    St.Rot.vy -= 0xC0;
    v = *Loop_ctr * 4 - 1;
    St.Rot.vz -= 0x80;
    G.Clr_b = *Loop_ctr >> 1;
    Item_light[0].r = v;
    Item_light[0].g = v;
    Item_light[0].b = v;
    Item_light[1].r = v;
    Item_light[1].g = v;
    Item_light[1].b = v;
    Item_light[2].r = v;
    Item_light[2].g = v;
    Item_light[2].b = v;
    GsSetFlatLight(0, &Item_light[0]);
    GsSetFlatLight(1, &Item_light[1]);
    GsSetFlatLight(2, &Item_light[2]);
    if (*Loop_ctr == 0)
    {
        St.Item_id += 0xC1;
        idx = (St.Icursol_main >> 1) - 4;
        Cd_read_wait(MEDAL_PIX, LOAD_BUF);
        Pix_trans(St.Item_id, Item_pix_idx[idx], LOAD_BUF);
        St.Item_id += 0x24;
        pItem_work_base[idx].Id = St.Item_id;
        pItem_work_base[idx].Num = 1;
        Item_model_load();
        St.View_rno = 0;
    }
}

VOID Item_model_disp(VOID)
{
    INT i;
    INT n;
    INT j;

    St.Rot.vx &= 0xFFF;
    St.Rot.vy &= 0xFFF;
    St.Rot.vz &= 0xFFF;
    RotMatrix(&St.Rot, &St.Coord[2].coord);
    j = 2;
    St.Coord[2].flg = 0;
    n = St.nView_obj;
    n--;
    do
    {
        j--;
        i = j & 0xFF;
        RotMatrix(&St.Obj_rot[i], &St.Coord[i].coord);
        St.Coord[i].flg = 0;
        GsGetLs(St.Obj[i].coord2, &ScratchPad->Item_mat);
        GsSetLightMatrix(&ScratchPad->Item_mat);
        GsSetLsMatrix(&ScratchPad->Item_mat);
        GsSortObject5(&St.Obj[i], &MAIN.Ot1[G.Cbuf_id], 4, ScratchUlong);
    } while (n-- & 0xFF);
}

VOID Item_rot_set(VOID)
{
    INT t;
    INT x;
    INT y;
    INT z;

    ScratchPad->Item_mat.t[0] = St.Coord[2].coord.t[0];
    ScratchPad->Item_mat.t[1] = St.Coord[2].coord.t[1];
    ScratchPad->Item_mat.t[2] = St.Coord[2].coord.t[2];
    RotMatrix((SVECTOR*)&St.Spin_x, &ScratchPad->Item_mat);  // reinterprets the memory as SVECTOR
    MulMatrix2(&ScratchPad->Item_mat, &St.Coord[2].coord);
    x = St.Coord[2].coord.m[0][2];
    t = x * x;
    y = St.Coord[2].coord.m[1][2];
    t += y * y;
    z = St.Coord[2].coord.m[2][2];
    t += z * z;
    t = (x << 0xC) / (t / 4096);
    St.Rot.vx = ratan2(-y, z);
    St.Rot.vy = ratan2(t, SquareRoot12(0x1000 - t * t / 4096));
    St.Rot.vz = 0;
    ScratchPad->Item_mat.t[0] = 0;
    ScratchPad->Item_mat.t[1] = 0;
    ScratchPad->Item_mat.t[2] = 0;
    RotMatrix(&St.Rot, &ScratchPad->Item_mat);
    TransposeMatrix(&ScratchPad->Item_mat, &ScratchPad->Item_mat2);
    ScratchPad->Item_sv.vx = St.Coord[2].coord.m[0][0];
    ScratchPad->Item_sv.vy = St.Coord[2].coord.m[1][0];
    ScratchPad->Item_sv.vz = St.Coord[2].coord.m[2][0];
    ApplyMatrix(&ScratchPad->Item_mat2, &ScratchPad->Item_sv, &ScratchPad->Item_vec);
    St.Rot.vz = ratan2(ScratchPad->Item_vec.vy, ScratchPad->Item_vec.vx);
}

VOID Item_arrow_disp(VOID)
{
    INT i;
    INT n;
    INT t;
    UINT mask;
    USHORT* pArrow_x;

    ScratchPad->Spr.attribute = 0x01000040;
    mask = 0x8000;
    n = 4;
    pArrow_x = (USHORT*)Item_arrow_x_tbl;  // halfword view of the same memory
    ScratchPad->Spr.tpage = 0x1C;
    ScratchPad->Spr.v = 0x7C;
    ScratchPad->Spr.w = 9;
    ScratchPad->Spr.h = 9;
    ScratchPad->Spr.cx = 0;
    ScratchPad->Spr.cy = 0x1E4;
    do
    {
        n--;
        i = n & 0xFF;
        t = i * 2;
        ScratchPad->Spr.x = pArrow_x[i];
        ScratchPad->Spr.y = *(USHORT*)(Item_arrow_y_tbl + t);  // halfword view of the same memory
        ScratchPad->Spr.u = 0x80;
        if (!(mask & G.Padd1))
        {
            ScratchPad->Spr.u = 0x8C;
        }
        GsSortFlipSprite(&ScratchPad->Spr, &MAIN.Ot0[G.Cbuf_id], 4);
        ScratchPad->Spr.v += 0xF4;
        mask >>= 1;
    } while (i != 0);
}

UCHAR Item_exam_ck(VOID)
{
    UCHAR* pItem_id;
    INT item;
    INT id;
    UCHAR r;
    UCHAR e;
    UCHAR Exam_no;
    SVECTOR* pRot;

    pItem_id = &St.Item_id;
    item = *pItem_id;
    Exam_no = Item_data_tbl.items[item - 1].Item_attribute;
    if (!(Exam_no & 0x80))
    {
        id = Exam_no;
        e = Item_exam_type_tbl[id];
        if (e & 0xF0)
        {
            St.pFrm_rec = (e & 0xF) * 0xC + Item_exam_ang_tbl;
            e >>= 4;
            pRot = (SVECTOR*)(pItem_id + 0x49);  // reinterprets the memory as SVECTOR
            // the nested do/while (0) wrapper is kept: it changes the generated loop structure
            do
            {
                do
                {
                    r = Item_ang_ck(pRot->vx, 1);
                    e--;
                } while (0);
                r |= Item_ang_ck(pRot->vy, 2);
                r |= Item_ang_ck(pRot->vz, 4);
            } while (e != 0 && r != 7);
            if (r != 7)
            {
                return 0;
            }
            switch (((UCHAR*)pRot)[-0x49])  // byte view of the same memory
            {
            case 0x3E:
                if (*(SHORT*)((UCHAR*)pRot - 6) != 0)  // halfword view of the same memory
                {
                    break;
                }
            case 0x3F:
            case 0x40:
                St.View_act = 1;
                St.Timer = 0;
                return 1;
            }
            St.View_act = 2;
            Flg_on(&MAIN.SaveData.Item_c_flg, Exam_no);  // address passed as an integer
            Mess_set(Item_exam_mess_tbl[Exam_no], 0);
            return 1;
        }
        St.View_act = 2;
        Flg_on(&MAIN.SaveData.Item_c_flg, id);  // address passed as an integer
        Mess_set(Item_exam_mess_tbl[id], 0);
        return 1;
    }
    if (item < 10)
    {
        if (item < 7 && item != 5)
        {
            return 0;
        }
    }
    else if (item == 0x13)
    {
        goto snd19;
    }
    else
    {
        return 0;
    }
    if (pItem_work_base[(UCHAR)((St.Icursol_main >> 1) - 4)].Num != 0)  // truncates to a byte
    {
        St.View_act = 2;
        G.SaveData.Sel_id = St.Item_id + 9;
        Mess_set(0xEF, 0);
        return 1;
    }
    return 0;
snd19:
    if (Flg_ck(&MAIN.SaveData.Scenario_flg, 0x13) != 0 || Flg_ck(&MAIN.SaveData.Scenario_flg, 0) != 0)  // address passed as an integer
    {
        St.View_act = 2;
        Mess_set(0xF0, 0);
        return 1;
    }
    return 0;
}

INT Item_ang_ck(SHORT angle, UCHAR pRet)
{
    SHORT** pp;
    SHORT** ppFrm;
    SHORT* p;
    SHORT a;
    USHORT t;
    SHORT b;
    INT d;

    p = (SHORT*)((STATUS*)(VOID*)&St)->pFrm_rec;  // halfword view of the same memory
    a = p[0];
    if (a != 0)
    {
        // the empty do/while (0) blocks are kept: they change the generated code
        do
        {
        } while (0);
        b = p[1];
        pp = (SHORT**)&St.pFrm_rec;  // halfword view of the same memory
        *pp += 2;
        t = (angle + a) & 0xFFF;
        d = (t - b) & 0xFFF;  // sign-extends the halfword
        if (a * 2 < d)
        {
            return 0;
        }
        return pRet;
    }
    do
    {
    } while (0);
    ppFrm = (SHORT**)&St.pFrm_rec;  // halfword view of the same memory
    *ppFrm += 2;
    return pRet;
}

// viewer action handlers, by View_act
ITEM_FUNC Item_act_tbl[4] =
{
    Item_act_disp,         // 0 //
    Item_act_spin,    // 1 //
    Item_act_wait,    // 2 //
    Item_act_medal,   // 3 //
};

// x position of each direction arrow (USHORT: up, right, down, left)
UCHAR Item_arrow_x_tbl[8] =
{
    0x6D,   // 0 //
    0x00,   // 1 //
    0xB6,   // 2 //
    0x00,   // 3 //
    0x6D,   // 4 //
    0x00,   // 5 //
    0x22,   // 6 //
    0x00,   // 7 //
};

// y position of each direction arrow (USHORT: up, right, down, left)
UCHAR Item_arrow_y_tbl[8] =
{
    0x1A,   // 0 //
    0x00,   // 1 //
    0x49,   // 2 //
    0x00,   // 3 //
    0x76,   // 4 //
    0x00,   // 5 //
    0x49,   // 6 //
    0x00,   // 7 //
};

// file number of each item model, by model id - 1
USHORT Item_model_tbl[76] =
{
    I05V_IVM,   // 0 //
    I00V_IVM,   // 1 //
    I02V_IVM,   // 2 //
    I04V_IVM,   // 3 //
    I01V_IVM,   // 4 //
    I08V_IVM,   // 5 //
    I07V_IVM,   // 6 //
    I26V_IVM,   // 7 //
    I16V_IVM,   // 8 //
    I06V_IVM,   // 9 //
    I15V_IVM,   // 10 //
    I11V_IVM,   // 11 //
    I14V_IVM,   // 12 //
    I12V_IVM,   // 13 //
    I13V_IVM,   // 14 //
    I50V_IVM,   // 15 //
    I21V_IVM,   // 16 //
    I22V_IVM,   // 17 //
    I48V_IVM,   // 18 //
    I58V_IVM,   // 19 //
    I47V_IVM,   // 20 //
    I49V_IVM,   // 21 //
    I62V_IVM,   // 22 //
    I09V_IVM,   // 23 //
    I03V_IVM,   // 24 //
    I19V_IVM,   // 25 //
    I72V_IVM,   // 26 //
    I38V_IVM,   // 27 //
    I39V_IVM,   // 28 //
    I41V_IVM,   // 29 //
    I42V_IVM,   // 30 //
    I45V_IVM,   // 31 //
    I24V_IVM,   // 32 //
    I25V_IVM,   // 33 //
    I43V_IVM,   // 34 //
    I54V_IVM,   // 35 //
    I37V_IVM,   // 36 //
    I44V_IVM,   // 37 //
    I55V_IVM,   // 38 //
    I46V_IVM,   // 39 //
    I60V_IVM,   // 40 //
    I61V_IVM,   // 41 //
    I59V_IVM,   // 42 //
    I56V_IVM,   // 43 //
    I17V_IVM,   // 44 //
    I60V_L_IVM,   // 45 //
    I60V_R_IVM,   // 46 //
    I30V_IVM,   // 47 //
    I28V_IVM,   // 48 //
    I29V_IVM,   // 49 //
    I27V_IVM,   // 50 //
    I34V_IVM,   // 51 //
    I73V_IVM,   // 52 //
    I32V_IVM,   // 53 //
    I31V_IVM,   // 54 //
    I36V_IVM,   // 55 //
    I33V_IVM,   // 56 //
    I63V_IVM,   // 57 //
    I53V_IVM,   // 58 //
    I51V_IVM,   // 59 //
    I52V_IVM,   // 60 //
    I23V_IVM,   // 61 //
    I20V_IVM,   // 62 //
    I66V_IVM,   // 63 //
    I64V_IVM,   // 64 //
    I65V_IVM,   // 65 //
    I69V_IVM,   // 66 //
    I67V_IVM,   // 67 //
    I68V_IVM,   // 68 //
    I70V_IVM,   // 69 //
    I71V_IVM,   // 70 //
    I75V_IVM,   // 71 //
    I57V_IVM,   // 72 //
    I57V_IVM,   // 73 //
    I00V_S1_IVM,   // 74 //
    0x0000,   // 75 //
};

// combine rule lists for Mix_idx 0-3: a count byte, then 4-byte rules {other item id, new id of the target item, new id of the other item, operation}
UCHAR Item_mix_tbl0[0x14] =
{
    0x01,   // 0 //
    0x0B,   // 1 //
    0x02,   // 2 //
    0x0B,   // 3 //
    0x01,   // 4 //
    0x01,   // 5 //
    0x0C,   // 6 //
    0x03,   // 7 //
    0x0C,   // 8 //
    0x01,   // 9 //
    0x01,   // 10 //
    0x0B,   // 11 //
    0x04,   // 12 //
    0x0B,   // 13 //
    0x01,   // 14 //
    0x01,   // 15 //
    0x0E,   // 16 //
    0x05,   // 17 //
    0x0E,   // 18 //
    0x01,   // 19 //
};

// combine rule lists for Mix_idx 4-7
UCHAR Item_mix_tbl1[0x2C] =
{
    0x01,   // 0 //
    0x0F,   // 1 //
    0x06,   // 2 //
    0x0F,   // 3 //
    0x01,   // 4 //
    0x03,   // 5 //
    0x10,   // 6 //
    0x07,   // 7 //
    0x10,   // 8 //
    0x01,   // 9 //
    0x11,   // 10 //
    0x08,   // 11 //
    0x11,   // 12 //
    0x06,   // 13 //
    0x12,   // 14 //
    0x09,   // 15 //
    0x12,   // 16 //
    0x06,   // 17 //
    0x03,   // 18 //
    0x10,   // 19 //
    0x07,   // 20 //
    0x10,   // 21 //
    0x06,   // 22 //
    0x11,   // 23 //
    0x08,   // 24 //
    0x11,   // 25 //
    0x01,   // 26 //
    0x12,   // 27 //
    0x09,   // 28 //
    0x12,   // 29 //
    0x06,   // 30 //
    0x03,   // 31 //
    0x10,   // 32 //
    0x07,   // 33 //
    0x10,   // 34 //
    0x06,   // 35 //
    0x11,   // 36 //
    0x08,   // 37 //
    0x11,   // 38 //
    0x06,   // 39 //
    0x12,   // 40 //
    0x09,   // 41 //
    0x12,   // 42 //
    0x01,   // 43 //
};

// combine rule lists for Mix_idx 8-11
UCHAR Item_mix_tbl2[0x28] =
{
    0x03,   // 0 //
    0x02,   // 1 //
    0x0B,   // 2 //
    0x02,   // 3 //
    0x02,   // 4 //
    0x04,   // 5 //
    0x0B,   // 6 //
    0x04,   // 7 //
    0x02,   // 8 //
    0x0B,   // 9 //
    0x0B,   // 10 //
    0x0B,   // 11 //
    0x03,   // 12 //
    0x02,   // 13 //
    0x03,   // 14 //
    0x0C,   // 15 //
    0x03,   // 16 //
    0x02,   // 17 //
    0x0C,   // 18 //
    0x0C,   // 19 //
    0x0C,   // 20 //
    0x03,   // 21 //
    0x02,   // 22 //
    0x04,   // 23 //
    0x0D,   // 24 //
    0x04,   // 25 //
    0x02,   // 26 //
    0x0D,   // 27 //
    0x0D,   // 28 //
    0x0D,   // 29 //
    0x03,   // 30 //
    0x02,   // 31 //
    0x05,   // 32 //
    0x0E,   // 33 //
    0x05,   // 34 //
    0x02,   // 35 //
    0x0E,   // 36 //
    0x0E,   // 37 //
    0x0E,   // 38 //
    0x03,   // 39 //
};

// combine rule lists for Mix_idx 12-15
UCHAR Item_mix_tbl3[0x3C] =
{
    0x02,   // 0 //
    0x06,   // 1 //
    0x0F,   // 2 //
    0x06,   // 3 //
    0x02,   // 4 //
    0x0F,   // 5 //
    0x0F,   // 6 //
    0x0F,   // 7 //
    0x03,   // 8 //
    0x04,   // 9 //
    0x07,   // 10 //
    0x10,   // 11 //
    0x07,   // 12 //
    0x02,   // 13 //
    0x08,   // 14 //
    0x10,   // 15 //
    0x07,   // 16 //
    0x07,   // 17 //
    0x09,   // 18 //
    0x10,   // 19 //
    0x07,   // 20 //
    0x07,   // 21 //
    0x10,   // 22 //
    0x10,   // 23 //
    0x10,   // 24 //
    0x03,   // 25 //
    0x04,   // 26 //
    0x07,   // 27 //
    0x11,   // 28 //
    0x08,   // 29 //
    0x07,   // 30 //
    0x08,   // 31 //
    0x11,   // 32 //
    0x08,   // 33 //
    0x02,   // 34 //
    0x09,   // 35 //
    0x11,   // 36 //
    0x08,   // 37 //
    0x07,   // 38 //
    0x11,   // 39 //
    0x11,   // 40 //
    0x11,   // 41 //
    0x03,   // 42 //
    0x04,   // 43 //
    0x07,   // 44 //
    0x12,   // 45 //
    0x09,   // 46 //
    0x07,   // 47 //
    0x08,   // 48 //
    0x12,   // 49 //
    0x09,   // 50 //
    0x07,   // 51 //
    0x09,   // 52 //
    0x12,   // 53 //
    0x09,   // 54 //
    0x02,   // 55 //
    0x12,   // 56 //
    0x12,   // 57 //
    0x12,   // 58 //
    0x03,   // 59 //
};

// combine rule lists for Mix_idx 16-19
UCHAR Item_mix_tbl4[0x58] =
{
    0x00,   // 0 //
    0x07,   // 1 //
    0x14,   // 2 //
    0x13,   // 3 //
    0x14,   // 4 //
    0x00,   // 5 //
    0x15,   // 6 //
    0x13,   // 7 //
    0x1A,   // 8 //
    0x00,   // 9 //
    0x16,   // 10 //
    0x13,   // 11 //
    0x13,   // 12 //
    0x05,   // 13 //
    0x17,   // 14 //
    0x13,   // 15 //
    0x13,   // 16 //
    0x04,   // 17 //
    0x18,   // 18 //
    0x13,   // 19 //
    0x13,   // 20 //
    0x05,   // 21 //
    0x19,   // 22 //
    0x13,   // 23 //
    0x17,   // 24 //
    0x00,   // 25 //
    0x1A,   // 26 //
    0x13,   // 27 //
    0x16,   // 28 //
    0x00,   // 29 //
    0x07,   // 30 //
    0x14,   // 31 //
    0x13,   // 32 //
    0x1A,   // 33 //
    0x00,   // 34 //
    0x15,   // 35 //
    0x13,   // 36 //
    0x15,   // 37 //
    0x00,   // 38 //
    0x16,   // 39 //
    0x13,   // 40 //
    0x19,   // 41 //
    0x00,   // 42 //
    0x17,   // 43 //
    0x13,   // 44 //
    0x13,   // 45 //
    0x05,   // 46 //
    0x18,   // 47 //
    0x13,   // 48 //
    0x13,   // 49 //
    0x04,   // 50 //
    0x19,   // 51 //
    0x13,   // 52 //
    0x13,   // 53 //
    0x04,   // 54 //
    0x1A,   // 55 //
    0x13,   // 56 //
    0x13,   // 57 //
    0x05,   // 58 //
    0x07,   // 59 //
    0x14,   // 60 //
    0x13,   // 61 //
    0x13,   // 62 //
    0x05,   // 63 //
    0x15,   // 64 //
    0x13,   // 65 //
    0x19,   // 66 //
    0x00,   // 67 //
    0x16,   // 68 //
    0x13,   // 69 //
    0x16,   // 70 //
    0x00,   // 71 //
    0x17,   // 72 //
    0x13,   // 73 //
    0x13,   // 74 //
    0x05,   // 75 //
    0x18,   // 76 //
    0x13,   // 77 //
    0x13,   // 78 //
    0x04,   // 79 //
    0x19,   // 80 //
    0x13,   // 81 //
    0x13,   // 82 //
    0x05,   // 83 //
    0x1A,   // 84 //
    0x13,   // 85 //
    0x17,   // 86 //
    0x00,   // 87 //
};

// combine rule lists for Mix_idx 20-23
UCHAR Item_mix_tbl5[0x74] =
{
    0x07,   // 0 //
    0x14,   // 1 //
    0x13,   // 2 //
    0x13,   // 3 //
    0x04,   // 4 //
    0x15,   // 5 //
    0x13,   // 6 //
    0x13,   // 7 //
    0x05,   // 8 //
    0x16,   // 9 //
    0x13,   // 10 //
    0x13,   // 11 //
    0x05,   // 12 //
    0x17,   // 13 //
    0x13,   // 14 //
    0x17,   // 15 //
    0x00,   // 16 //
    0x18,   // 17 //
    0x13,   // 18 //
    0x13,   // 19 //
    0x04,   // 20 //
    0x19,   // 21 //
    0x13,   // 22 //
    0x18,   // 23 //
    0x00,   // 24 //
    0x1A,   // 25 //
    0x13,   // 26 //
    0x13,   // 27 //
    0x05,   // 28 //
    0x07,   // 29 //
    0x14,   // 30 //
    0x13,   // 31 //
    0x13,   // 32 //
    0x05,   // 33 //
    0x15,   // 34 //
    0x13,   // 35 //
    0x13,   // 36 //
    0x04,   // 37 //
    0x16,   // 38 //
    0x13,   // 39 //
    0x13,   // 40 //
    0x04,   // 41 //
    0x17,   // 42 //
    0x13,   // 43 //
    0x13,   // 44 //
    0x04,   // 45 //
    0x18,   // 46 //
    0x13,   // 47 //
    0x18,   // 48 //
    0x00,   // 49 //
    0x19,   // 50 //
    0x13,   // 51 //
    0x13,   // 52 //
    0x04,   // 53 //
    0x1A,   // 54 //
    0x13,   // 55 //
    0x1B,   // 56 //
    0x00,   // 57 //
    0x07,   // 58 //
    0x14,   // 59 //
    0x13,   // 60 //
    0x17,   // 61 //
    0x00,   // 62 //
    0x15,   // 63 //
    0x13,   // 64 //
    0x13,   // 65 //
    0x04,   // 66 //
    0x16,   // 67 //
    0x13,   // 68 //
    0x13,   // 69 //
    0x05,   // 70 //
    0x17,   // 71 //
    0x13,   // 72 //
    0x18,   // 73 //
    0x00,   // 74 //
    0x18,   // 75 //
    0x13,   // 76 //
    0x13,   // 77 //
    0x04,   // 78 //
    0x19,   // 79 //
    0x13,   // 80 //
    0x19,   // 81 //
    0x00,   // 82 //
    0x1A,   // 83 //
    0x13,   // 84 //
    0x13,   // 85 //
    0x05,   // 86 //
    0x07,   // 87 //
    0x14,   // 88 //
    0x13,   // 89 //
    0x16,   // 90 //
    0x00,   // 91 //
    0x15,   // 92 //
    0x13,   // 93 //
    0x13,   // 94 //
    0x05,   // 95 //
    0x16,   // 96 //
    0x13,   // 97 //
    0x17,   // 98 //
    0x00,   // 99 //
    0x17,   // 100 //
    0x13,   // 101 //
    0x13,   // 102 //
    0x05,   // 103 //
    0x18,   // 104 //
    0x13,   // 105 //
    0x1B,   // 106 //
    0x00,   // 107 //
    0x19,   // 108 //
    0x13,   // 109 //
    0x13,   // 110 //
    0x05,   // 111 //
    0x1A,   // 112 //
    0x13,   // 113 //
    0x1A,   // 114 //
    0x00,   // 115 //
};

// combine rule lists for Mix_idx 24-27
UCHAR Item_mix_tbl6[0x10] =
{
    0x00,   // 0 //
    0x01,   // 1 //
    0x2F,   // 2 //
    0x2F,   // 3 //
    0x2F,   // 4 //
    0x03,   // 5 //
    0x01,   // 6 //
    0x32,   // 7 //
    0x2C,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x01,   // 11 //
    0x31,   // 12 //
    0x2C,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
};

// combine rule lists for Mix_idx 28-31
UCHAR Item_mix_tbl7[0x2C] =
{
    0x00,   // 0 //
    0x02,   // 1 //
    0x44,   // 2 //
    0x46,   // 3 //
    0x00,   // 4 //
    0x00,   // 5 //
    0x48,   // 6 //
    0x49,   // 7 //
    0x00,   // 8 //
    0x00,   // 9 //
    0x05,   // 10 //
    0x43,   // 11 //
    0x46,   // 12 //
    0x00,   // 13 //
    0x00,   // 14 //
    0x44,   // 15 //
    0x47,   // 16 //
    0x00,   // 17 //
    0x00,   // 18 //
    0x45,   // 19 //
    0x48,   // 20 //
    0x00,   // 21 //
    0x00,   // 22 //
    0x47,   // 23 //
    0x4A,   // 24 //
    0x00,   // 25 //
    0x00,   // 26 //
    0x48,   // 27 //
    0x4B,   // 28 //
    0x00,   // 29 //
    0x00,   // 30 //
    0x03,   // 31 //
    0x44,   // 32 //
    0x48,   // 33 //
    0x00,   // 34 //
    0x00,   // 35 //
    0x46,   // 36 //
    0x49,   // 37 //
    0x00,   // 38 //
    0x00,   // 39 //
    0x47,   // 40 //
    0x4B,   // 41 //
    0x00,   // 42 //
    0x00,   // 43 //
};

// combine rule lists for Mix_idx 32-35
UCHAR Item_mix_tbl8[0x18] =
{
    0x01,   // 0 //
    0x45,   // 1 //
    0x49,   // 2 //
    0x00,   // 3 //
    0x00,   // 4 //
    0x02,   // 5 //
    0x44,   // 6 //
    0x4A,   // 7 //
    0x00,   // 8 //
    0x00,   // 9 //
    0x45,   // 10 //
    0x4B,   // 11 //
    0x00,   // 12 //
    0x00,   // 13 //
    0x02,   // 14 //
    0x43,   // 15 //
    0x49,   // 16 //
    0x00,   // 17 //
    0x00,   // 18 //
    0x44,   // 19 //
    0x4B,   // 20 //
    0x00,   // 21 //
    0x00,   // 22 //
    0x00,   // 23 //
};

// combine rule lists for Mix_idx 36-37
UCHAR Item_mix_tbl9[0x4] =
{
    0x00,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x00,   // 3 //
};

// combine rule list of each Mix_idx (pointer into the combine rule arrays)
UCHAR* Item_mix_tbl[38] =
{
    Item_mix_tbl0,          // 0 //
    Item_mix_tbl0 + 0x5,    // 1 //
    Item_mix_tbl0 + 0xA,    // 2 //
    Item_mix_tbl0 + 0xF,    // 3 //
    Item_mix_tbl1,          // 4 //
    Item_mix_tbl1 + 0x5,    // 5 //
    Item_mix_tbl1 + 0x12,   // 6 //
    Item_mix_tbl1 + 0x1F,   // 7 //
    Item_mix_tbl2,          // 8 //
    Item_mix_tbl2 + 0xD,    // 9 //
    Item_mix_tbl2 + 0x16,   // 10 //
    Item_mix_tbl2 + 0x1F,   // 11 //
    Item_mix_tbl3,          // 12 //
    Item_mix_tbl3 + 0x9,    // 13 //
    Item_mix_tbl3 + 0x1A,   // 14 //
    Item_mix_tbl3 + 0x2B,   // 15 //
    Item_mix_tbl4,          // 16 //
    Item_mix_tbl4 + 0x1,    // 17 //
    Item_mix_tbl4 + 0x1E,   // 18 //
    Item_mix_tbl4 + 0x3B,   // 19 //
    Item_mix_tbl5,          // 20 //
    Item_mix_tbl5 + 0x1D,   // 21 //
    Item_mix_tbl5 + 0x3A,   // 22 //
    Item_mix_tbl5 + 0x57,   // 23 //
    Item_mix_tbl6,          // 24 //
    Item_mix_tbl6 + 0x1,    // 25 //
    Item_mix_tbl6 + 0x6,    // 26 //
    Item_mix_tbl6 + 0xB,    // 27 //
    Item_mix_tbl7,          // 28 //
    Item_mix_tbl7 + 0x1,    // 29 //
    Item_mix_tbl7 + 0xA,    // 30 //
    Item_mix_tbl7 + 0x1F,   // 31 //
    Item_mix_tbl8,          // 32 //
    Item_mix_tbl8 + 0x5,    // 33 //
    Item_mix_tbl8 + 0xE,    // 34 //
    Item_mix_tbl8 + 0x17,   // 35 //
    Item_mix_tbl9,          // 36 //
    Item_mix_tbl9 + 0x1,    // 37 //
};

// icon image number (1-based, 0 = none) of an item, by Mix_idx
UCHAR Item_img_tbl[0x28] =
{
    0x00,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x00,   // 3 //
    0x00,   // 4 //
    0x00,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x00,   // 9 //
    0x01,   // 10 //
    0x02,   // 11 //
    0x00,   // 12 //
    0x03,   // 13 //
    0x04,   // 14 //
    0x05,   // 15 //
    0x06,   // 16 //
    0x00,   // 17 //
    0x00,   // 18 //
    0x07,   // 19 //
    0x08,   // 20 //
    0x09,   // 21 //
    0x0A,   // 22 //
    0x0B,   // 23 //
    0x0C,   // 24 //
    0x00,   // 25 //
    0x0D,   // 26 //
    0x0E,   // 27 //
    0x0E,   // 28 //
    0x00,   // 29 //
    0x00,   // 30 //
    0x00,   // 31 //
    0x10,   // 32 //
    0x11,   // 33 //
    0x12,   // 34 //
    0x13,   // 35 //
    0x14,   // 36 //
    0x15,   // 37 //
    0x00,   // 38 //
    0x00,   // 39 //
};

// per-item records (max count, icon, combine and examine index) by item id - 1, then 13 further raw records
ITEM_INFO_TBL Item_data_tbl =
{
    {
        { 0, 1, -128, 128 },      // 0 //
        { 15, 2, 0, 128 },        // 1 //
        { 7, 3, 1, 128 },         // 2 //
        { 15, 75, 2, 128 },       // 3 //
        { 6, 4, 3, 128 },         // 4 //
        { 240, 5, 4, 128 },       // 5 //
        { 6, 6, 5, 128 },         // 6 //
        { 6, 6, 6, 128 },         // 7 //
        { 6, 6, 7, 128 },         // 8 //
        { 4, 7, -128, 128 },      // 9 //
        { 15, 8, 8, 128 },        // 10 //
        { 7, 9, 9, 128 },         // 11 //
        { 6, 10, 10, 128 },       // 12 //
        { 6, 11, 11, 128 },       // 13 //
        { 240, 12, 12, 128 },     // 14 //
        { 6, 13, 13, 128 },       // 15 //
        { 6, 14, 14, 128 },       // 16 //
        { 6, 15, 15, 128 },       // 17 //
        { 1, 16, 16, 128 },       // 18 //
        { 1, 17, 17, 128 },       // 19 //
        { 1, 18, 18, 128 },       // 20 //
        { 1, 19, 19, 128 },       // 21 //
        { 1, 20, 20, 128 },       // 22 //
        { 1, 21, 21, 128 },       // 23 //
        { 1, 22, 22, 128 },       // 24 //
        { 1, 23, 23, 128 },       // 25 //
        { 1, 24, 24, 128 },       // 26 //
        { 1, 25, -128, 128 },     // 27 //
        { 0, 26, -128, 0 },       // 28 //
        { 0, 27, -128, 1 },       // 29 //
        { 1, 28, -128, 128 },     // 30 //
        { 1, 29, -128, 128 },     // 31 //
        { 1, 30, -128, 128 },     // 32 //
        { 1, 31, -128, 128 },     // 33 //
        { 1, 32, -128, 128 },     // 34 //
        { 1, 33, -128, 128 },     // 35 //
        { 1, 34, -128, 128 },     // 36 //
        { 1, 35, -128, 2 },       // 37 //
        { 1, 36, -128, 128 },     // 38 //
        { 1, 37, -128, 128 },     // 39 //
        { 1, 38, -128, 128 },     // 40 //
        { 1, 39, -128, 128 },     // 41 //
        { 1, 40, -128, 128 },     // 42 //
        { 1, 41, 28, 128 },       // 43 //
        { 1, 42, -128, 128 },     // 44 //
        { 1, 43, -128, 128 },     // 45 //
        { 2, 44, 25, 128 },       // 46 //
        { 0, 45, -128, 128 },     // 47 //
        { 1, 46, 26, 128 },       // 48 //
        { 1, 47, 27, 128 },       // 49 //
        { 0, 48, -128, 3 },       // 50 //
        { 0, 49, -128, 4 },       // 51 //
        { 0, 50, -128, 5 },       // 52 //
        { 0, 51, -128, 6 },       // 53 //
        { 0, 52, -128, 7 },       // 54 //
        { 0, 53, -128, 8 },       // 55 //
        { 0, 54, -128, 9 },       // 56 //
        { 0, 55, -128, 10 },      // 57 //
        { 0, 56, -128, 128 },     // 58 //
        { 0, 57, -128, 11 },      // 59 //
        { 1, 58, -128, 12 },      // 60 //
        { 1, 59, -128, 13 },      // 61 //
        { 1, 60, -128, 14 },      // 62 //
        { 1, 61, -128, 15 },      // 63 //
    },
    {
        0x01, 0x3E, 0x80, 0x80,   // 64 //
        0x01, 0x3F, 0x80, 0x80,   // 65 //
        0x01, 0x40, 0x1D, 0x80,   // 66 //
        0x01, 0x41, 0x1E, 0x80,   // 67 //
        0x01, 0x42, 0x1F, 0x80,   // 68 //
        0x01, 0x43, 0x20, 0x80,   // 69 //
        0x01, 0x44, 0x21, 0x80,   // 70 //
        0x01, 0x45, 0x22, 0x80,   // 71 //
        0x01, 0x46, 0x23, 0x80,   // 72 //
        0x01, 0x47, 0x24, 0x80,   // 73 //
        0x01, 0x48, 0x25, 0x80,   // 74 //
        0x00, 0x49, 0x80, 0x80,   // 75 //
        0x00, 0x4A, 0x80, 0x80,   // 76 //
    },
};

// first item id of each use-type group (first 10 bytes); later bytes are further per-item data
UCHAR Item_use_tbl[0x18] =
{
    0x0B,   // 0 //
    0x13,   // 1 //
    0x14,   // 2 //
    0x1B,   // 3 //
    0x33,   // 4 //
    0x3D,   // 5 //
    0x3E,   // 6 //
    0x41,   // 7 //
    0x4C,   // 8 //
    0x4C,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x03,   // 12 //
    0x10,   // 13 //
    0x00,   // 14 //
    0x01,   // 15 //
    0x20,   // 16 //
    0x03,   // 17 //
    0x02,   // 18 //
    0x21,   // 19 //
    0x23,   // 20 //
    0x03,   // 21 //
    0x22,   // 22 //
    0x00,   // 23 //
};

// message id shown for each examine entry
UCHAR Item_exam_mess_tbl[0x10] =
{
    0xEA,   // 0 //
    0xEB,   // 1 //
    0xEE,   // 2 //
    0xE0,   // 3 //
    0xE1,   // 4 //
    0xE2,   // 5 //
    0xE3,   // 6 //
    0xE8,   // 7 //
    0xE9,   // 8 //
    0xE4,   // 9 //
    0xE5,   // 10 //
    0xE7,   // 11 //
    0xE6,   // 12 //
    0xEC,   // 13 //
    0xED,   // 14 //
    0xED,   // 15 //
};

// examine angle windows: 12-byte sets of three (tolerance, target angle) SHORT pairs for x, y, z
UCHAR Item_exam_ang_tbl[0x30] =
{
    0x00,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x00,   // 3 //
    0x2C,   // 4 //
    0x01,   // 5 //
    0x00,   // 6 //
    0x0C,   // 7 //
    0x00,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x00,   // 12 //
    0x00,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0x30,   // 16 //
    0x02,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
    0x30,   // 20 //
    0x02,   // 21 //
    0x00,   // 22 //
    0x08,   // 23 //
    0x00,   // 24 //
    0x00,   // 25 //
    0x00,   // 26 //
    0x00,   // 27 //
    0x30,   // 28 //
    0x02,   // 29 //
    0x00,   // 30 //
    0x00,   // 31 //
    0x30,   // 32 //
    0x02,   // 33 //
    0x00,   // 34 //
    0x00,   // 35 //
    0x00,   // 36 //
    0x00,   // 37 //
    0x00,   // 38 //
    0x00,   // 39 //
    0x90,   // 40 //
    0x01,   // 41 //
    0x00,   // 42 //
    0x04,   // 43 //
    0x00,   // 44 //
    0x00,   // 45 //
    0x00,   // 46 //
    0x00,   // 47 //
};

// per examine entry: high nibble = pose count, low nibble = first window set
UCHAR Item_exam_type_tbl[0x10] =
{
    0x10,   // 0 //
    0x10,   // 1 //
    0x00,   // 2 //
    0x11,   // 3 //
    0x11,   // 4 //
    0x11,   // 5 //
    0x11,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x11,   // 9 //
    0x11,   // 10 //
    0x21,   // 11 //
    0x00,   // 12 //
    0x13,   // 13 //
    0x13,   // 14 //
    0x13,   // 15 //
};

// encoded item description strings, referenced from Item_mess_tbl
UCHAR Item_mess_dat[0x6A8] =
{
    0x30,   // 0 //
    0x44,   // 1 //
    0x45,   // 2 //
    0x4F,   // 3 //
    0x00,   // 4 //
    0x40,   // 5 //
    0x4B,   // 6 //
    0x41,   // 7 //
    0x4F,   // 8 //
    0x4A,   // 9 //
    0x3A,   // 10 //
    0x50,   // 11 //
    0x00,   // 12 //
    0x4F,   // 13 //
    0x41,   // 14 //
    0x41,   // 15 //
    0x49,   // 16 //
    0x00,   // 17 //
    0x50,   // 18 //
    0x4B,   // 19 //
    0x00,   // 20 //
    0x3E,   // 21 //
    0x41,   // 22 //
    0x02,   // 23 //
    0x41,   // 24 //
    0x4A,   // 25 //
    0x4B,   // 26 //
    0x51,   // 27 //
    0x43,   // 28 //
    0x44,   // 29 //
    0x00,   // 30 //
    0x42,   // 31 //
    0x4B,   // 32 //
    0x4E,   // 33 //
    0x00,   // 34 //
    0x50,   // 35 //
    0x44,   // 36 //
    0x45,   // 37 //
    0x4F,   // 38 //
    0x00,   // 39 //
    0x49,   // 40 //
    0x45,   // 41 //
    0x4F,   // 42 //
    0x4F,   // 43 //
    0x45,   // 44 //
    0x4B,   // 45 //
    0x4A,   // 46 //
    0x79,   // 47 //
    0x01,   // 48 //
    0x00,   // 49 //
    0x1E,   // 50 //
    0x41,   // 51 //
    0x4E,   // 52 //
    0x41,   // 53 //
    0x50,   // 54 //
    0x50,   // 55 //
    0x3D,   // 56 //
    0x00,   // 57 //
    0x29,   // 58 //
    0x15,   // 59 //
    0x0E,   // 60 //
    0x22,   // 61 //
    0x2F,   // 62 //
    0x79,   // 63 //
    0x00,   // 64 //
    0x1D,   // 65 //
    0x51,   // 66 //
    0x50,   // 67 //
    0x4B,   // 68 //
    0x49,   // 69 //
    0x3D,   // 70 //
    0x50,   // 71 //
    0x45,   // 72 //
    0x3F,   // 73 //
    0x02,   // 74 //
    0x48,   // 75 //
    0x4B,   // 76 //
    0x3D,   // 77 //
    0x40,   // 78 //
    0x41,   // 79 //
    0x40,   // 80 //
    0x00,   // 81 //
    0x53,   // 82 //
    0x45,   // 83 //
    0x50,   // 84 //
    0x44,   // 85 //
    0x00,   // 86 //
    0x15,   // 87 //
    0x49,   // 88 //
    0x49,   // 89 //
    0x00,   // 90 //
    0x3E,   // 91 //
    0x51,   // 92 //
    0x48,   // 93 //
    0x48,   // 94 //
    0x41,   // 95 //
    0x50,   // 96 //
    0x4F,   // 97 //
    0x79,   // 98 //
    0x01,   // 99 //
    0x00,   // 100 //
    0x2E,   // 101 //
    0x41,   // 102 //
    0x49,   // 103 //
    0x45,   // 104 //
    0x4A,   // 105 //
    0x43,   // 106 //
    0x50,   // 107 //
    0x4B,   // 108 //
    0x4A,   // 109 //
    0x00,   // 110 //
    0x29,   // 111 //
    0x14,   // 112 //
    0x13,   // 113 //
    0x0C,   // 114 //
    0x79,   // 115 //
    0x02,   // 116 //
    0x1D,   // 117 //
    0x00,   // 118 //
    0x4C,   // 119 //
    0x51,   // 120 //
    0x49,   // 121 //
    0x4C,   // 122 //
    0x3B,   // 123 //
    0x3D,   // 124 //
    0x3F,   // 125 //
    0x50,   // 126 //
    0x45,   // 127 //
    0x4B,   // 128 //
    0x4A,   // 129 //
    0x00,   // 130 //
    0x4F,   // 131 //
    0x44,   // 132 //
    0x4B,   // 133 //
    0x50,   // 134 //
    0x43,   // 135 //
    0x51,   // 136 //
    0x4A,   // 137 //
    0x79,   // 138 //
    0x01,   // 139 //
    0x00,   // 140 //
    0x2C,   // 141 //
    0x4B,   // 142 //
    0x53,   // 143 //
    0x41,   // 144 //
    0x4E,   // 145 //
    0x42,   // 146 //
    0x51,   // 147 //
    0x48,   // 148 //
    0x00,   // 149 //
    0x43,   // 150 //
    0x51,   // 151 //
    0x4A,   // 152 //
    0x00,   // 153 //
    0x3F,   // 154 //
    0x3D,   // 155 //
    0x4A,   // 156 //
    0x00,   // 157 //
    0x3E,   // 158 //
    0x41,   // 159 //
    0x00,   // 160 //
    0x48,   // 161 //
    0x4B,   // 162 //
    0x3D,   // 163 //
    0x40,   // 164 //
    0x41,   // 165 //
    0x40,   // 166 //
    0x02,   // 167 //
    0x53,   // 168 //
    0x45,   // 169 //
    0x50,   // 170 //
    0x44,   // 171 //
    0x00,   // 172 //
    0x79,   // 173 //
    0x0F,   // 174 //
    0x11,   // 175 //
    0x13,   // 176 //
    0x00,   // 177 //
    0x49,   // 178 //
    0x3D,   // 179 //
    0x43,   // 180 //
    0x4A,   // 181 //
    0x51,   // 182 //
    0x49,   // 183 //
    0x00,   // 184 //
    0x4E,   // 185 //
    0x4B,   // 186 //
    0x51,   // 187 //
    0x4A,   // 188 //
    0x40,   // 189 //
    0x4F,   // 190 //
    0x79,   // 191 //
    0x01,   // 192 //
    0x00,   // 193 //
    0x1F,   // 194 //
    0x3D,   // 195 //
    0x4A,   // 196 //
    0x00,   // 197 //
    0x50,   // 198 //
    0x44,   // 199 //
    0x4E,   // 200 //
    0x4B,   // 201 //
    0x53,   // 202 //
    0x00,   // 203 //
    0x42,   // 204 //
    0x48,   // 205 //
    0x3D,   // 206 //
    0x49,   // 207 //
    0x41,   // 208 //
    0x00,   // 209 //
    0x42,   // 210 //
    0x4B,   // 211 //
    0x4E,   // 212 //
    0x00,   // 213 //
    0x15,   // 214 //
    0x00,   // 215 //
    0x4F,   // 216 //
    0x41,   // 217 //
    0x3F,   // 218 //
    0x79,   // 219 //
    0x02,   // 220 //
    0x53,   // 221 //
    0x45,   // 222 //
    0x50,   // 223 //
    0x44,   // 224 //
    0x00,   // 225 //
    0x49,   // 226 //
    0x3D,   // 227 //
    0x54,   // 228 //
    0x79,   // 229 //
    0x00,   // 230 //
    0x42,   // 231 //
    0x51,   // 232 //
    0x41,   // 233 //
    0x48,   // 234 //
    0x79,   // 235 //
    0x01,   // 236 //
    0x00,   // 237 //
    0x1D,   // 238 //
    0x00,   // 239 //
    0x48,   // 240 //
    0x3D,   // 241 //
    0x51,   // 242 //
    0x4A,   // 243 //
    0x3F,   // 244 //
    0x44,   // 245 //
    0x41,   // 246 //
    0x4E,   // 247 //
    0x00,   // 248 //
    0x3F,   // 249 //
    0x3D,   // 250 //
    0x4A,   // 251 //
    0x00,   // 252 //
    0x3E,   // 253 //
    0x41,   // 254 //
    0x00,   // 255 //
    0x48,   // 256 //
    0x4B,   // 257 //
    0x3D,   // 258 //
    0x40,   // 259 //
    0x41,   // 260 //
    0x40,   // 261 //
    0x02,   // 262 //
    0x53,   // 263 //
    0x45,   // 264 //
    0x50,   // 265 //
    0x44,   // 266 //
    0x00,   // 267 //
    0x52,   // 268 //
    0x3D,   // 269 //
    0x4E,   // 270 //
    0x45,   // 271 //
    0x4B,   // 272 //
    0x51,   // 273 //
    0x4F,   // 274 //
    0x00,   // 275 //
    0x4E,   // 276 //
    0x4B,   // 277 //
    0x51,   // 278 //
    0x4A,   // 279 //
    0x40,   // 280 //
    0x4F,   // 281 //
    0x79,   // 282 //
    0x01,   // 283 //
    0x00,   // 284 //
    0x2B,   // 285 //
    0x4A,   // 286 //
    0x41,   // 287 //
    0x00,   // 288 //
    0x4F,   // 289 //
    0x44,   // 290 //
    0x4B,   // 291 //
    0x50,   // 292 //
    0x00,   // 293 //
    0x3F,   // 294 //
    0x3D,   // 295 //
    0x4A,   // 296 //
    0x00,   // 297 //
    0x40,   // 298 //
    0x41,   // 299 //
    0x4F,   // 300 //
    0x50,   // 301 //
    0x4E,   // 302 //
    0x4B,   // 303 //
    0x55,   // 304 //
    0x00,   // 305 //
    0x3D,   // 306 //
    0x4A,   // 307 //
    0x55,   // 308 //
    0x02,   // 309 //
    0x50,   // 310 //
    0x3D,   // 311 //
    0x4E,   // 312 //
    0x43,   // 313 //
    0x41,   // 314 //
    0x50,   // 315 //
    0x79,   // 316 //
    0x01,   // 317 //
    0x00,   // 318 //
    0x1F,   // 319 //
    0x48,   // 320 //
    0x45,   // 321 //
    0x4C,   // 322 //
    0x00,   // 323 //
    0x42,   // 324 //
    0x4B,   // 325 //
    0x4E,   // 326 //
    0x00,   // 327 //
    0x1E,   // 328 //
    0x41,   // 329 //
    0x4E,   // 330 //
    0x41,   // 331 //
    0x50,   // 332 //
    0x50,   // 333 //
    0x3D,   // 334 //
    0x79,   // 335 //
    0x01,   // 336 //
    0x00,   // 337 //
    0x2F,   // 338 //
    0x44,   // 339 //
    0x41,   // 340 //
    0x48,   // 341 //
    0x48,   // 342 //
    0x4F,   // 343 //
    0x00,   // 344 //
    0x42,   // 345 //
    0x4B,   // 346 //
    0x4E,   // 347 //
    0x00,   // 348 //
    0x50,   // 349 //
    0x44,   // 350 //
    0x41,   // 351 //
    0x00,   // 352 //
    0x4F,   // 353 //
    0x44,   // 354 //
    0x4B,   // 355 //
    0x50,   // 356 //
    0x43,   // 357 //
    0x51,   // 358 //
    0x4A,   // 359 //
    0x79,   // 360 //
    0x01,   // 361 //
    0x00,   // 362 //
    0x29,   // 363 //
    0x4B,   // 364 //
    0x4E,   // 365 //
    0x41,   // 366 //
    0x00,   // 367 //
    0x4C,   // 368 //
    0x4B,   // 369 //
    0x53,   // 370 //
    0x41,   // 371 //
    0x4E,   // 372 //
    0x42,   // 373 //
    0x51,   // 374 //
    0x48,   // 375 //
    0x00,   // 376 //
    0x50,   // 377 //
    0x44,   // 378 //
    0x3D,   // 379 //
    0x4A,   // 380 //
    0x00,   // 381 //
    0x49,   // 382 //
    0x3D,   // 383 //
    0x43,   // 384 //
    0x4A,   // 385 //
    0x51,   // 386 //
    0x49,   // 387 //
    0x02,   // 388 //
    0x4E,   // 389 //
    0x4B,   // 390 //
    0x51,   // 391 //
    0x4A,   // 392 //
    0x40,   // 393 //
    0x4F,   // 394 //
    0x79,   // 395 //
    0x00,   // 396 //
    0x00,   // 397 //
    0x22,   // 398 //
    0x4B,   // 399 //
    0x4E,   // 400 //
    0x00,   // 401 //
    0x50,   // 402 //
    0x44,   // 403 //
    0x41,   // 404 //
    0x00,   // 405 //
    0x1F,   // 406 //
    0x79,   // 407 //
    0x2C,   // 408 //
    0x55,   // 409 //
    0x50,   // 410 //
    0x44,   // 411 //
    0x4B,   // 412 //
    0x4A,   // 413 //
    0x79,   // 414 //
    0x01,   // 415 //
    0x00,   // 416 //
    0x79,   // 417 //
    0x0F,   // 418 //
    0x11,   // 419 //
    0x13,   // 420 //
    0x00,   // 421 //
    0x49,   // 422 //
    0x3D,   // 423 //
    0x43,   // 424 //
    0x4A,   // 425 //
    0x51,   // 426 //
    0x49,   // 427 //
    0x00,   // 428 //
    0x4E,   // 429 //
    0x4B,   // 430 //
    0x51,   // 431 //
    0x4A,   // 432 //
    0x40,   // 433 //
    0x4F,   // 434 //
    0x79,   // 435 //
    0x02,   // 436 //
    0x22,   // 437 //
    0x4B,   // 438 //
    0x4E,   // 439 //
    0x00,   // 440 //
    0x50,   // 441 //
    0x44,   // 442 //
    0x41,   // 443 //
    0x00,   // 444 //
    0x1F,   // 445 //
    0x4B,   // 446 //
    0x48,   // 447 //
    0x50,   // 448 //
    0x00,   // 449 //
    0x2C,   // 450 //
    0x55,   // 451 //
    0x50,   // 452 //
    0x44,   // 453 //
    0x4B,   // 454 //
    0x4A,   // 455 //
    0x79,   // 456 //
    0x01,   // 457 //
    0x00,   // 458 //
    0x22,   // 459 //
    0x51,   // 460 //
    0x41,   // 461 //
    0x48,   // 462 //
    0x00,   // 463 //
    0x42,   // 464 //
    0x4B,   // 465 //
    0x4E,   // 466 //
    0x00,   // 467 //
    0x50,   // 468 //
    0x44,   // 469 //
    0x41,   // 470 //
    0x00,   // 471 //
    0x42,   // 472 //
    0x48,   // 473 //
    0x3D,   // 474 //
    0x49,   // 475 //
    0x41,   // 476 //
    0x50,   // 477 //
    0x44,   // 478 //
    0x4E,   // 479 //
    0x4B,   // 480 //
    0x53,   // 481 //
    0x41,   // 482 //
    0x4E,   // 483 //
    0x79,   // 484 //
    0x01,   // 485 //
    0x00,   // 486 //
    0x2C,   // 487 //
    0x4B,   // 488 //
    0x53,   // 489 //
    0x41,   // 490 //
    0x4E,   // 491 //
    0x42,   // 492 //
    0x51,   // 493 //
    0x48,   // 494 //
    0x00,   // 495 //
    0x4E,   // 496 //
    0x4B,   // 497 //
    0x51,   // 498 //
    0x4A,   // 499 //
    0x40,   // 500 //
    0x4F,   // 501 //
    0x00,   // 502 //
    0x42,   // 503 //
    0x4B,   // 504 //
    0x4E,   // 505 //
    0x00,   // 506 //
    0x50,   // 507 //
    0x44,   // 508 //
    0x41,   // 509 //
    0x02,   // 510 //
    0x3E,   // 511 //
    0x3D,   // 512 //
    0x56,   // 513 //
    0x4B,   // 514 //
    0x4B,   // 515 //
    0x47,   // 516 //
    0x3D,   // 517 //
    0x79,   // 518 //
    0x01,   // 519 //
    0x00,   // 520 //
    0x1D,   // 521 //
    0x00,   // 522 //
    0x43,   // 523 //
    0x48,   // 524 //
    0x3D,   // 525 //
    0x4F,   // 526 //
    0x4F,   // 527 //
    0x00,   // 528 //
    0x3E,   // 529 //
    0x4B,   // 530 //
    0x50,   // 531 //
    0x50,   // 532 //
    0x48,   // 533 //
    0x41,   // 534 //
    0x00,   // 535 //
    0x50,   // 536 //
    0x4B,   // 537 //
    0x00,   // 538 //
    0x4C,   // 539 //
    0x51,   // 540 //
    0x50,   // 541 //
    0x02,   // 542 //
    0x3F,   // 543 //
    0x44,   // 544 //
    0x41,   // 545 //
    0x49,   // 546 //
    0x45,   // 547 //
    0x3F,   // 548 //
    0x3D,   // 549 //
    0x48,   // 550 //
    0x4F,   // 551 //
    0x00,   // 552 //
    0x45,   // 553 //
    0x4A,   // 554 //
    0x79,   // 555 //
    0x01,   // 556 //
    0x00,   // 557 //
    0x30,   // 558 //
    0x44,   // 559 //
    0x41,   // 560 //
    0x4E,   // 561 //
    0x41,   // 562 //
    0x3A,   // 563 //
    0x4F,   // 564 //
    0x00,   // 565 //
    0x53,   // 566 //
    0x3D,   // 567 //
    0x50,   // 568 //
    0x41,   // 569 //
    0x4E,   // 570 //
    0x00,   // 571 //
    0x45,   // 572 //
    0x4A,   // 573 //
    0x00,   // 574 //
    0x50,   // 575 //
    0x44,   // 576 //
    0x41,   // 577 //
    0x02,   // 578 //
    0x3E,   // 579 //
    0x4B,   // 580 //
    0x50,   // 581 //
    0x50,   // 582 //
    0x48,   // 583 //
    0x41,   // 584 //
    0x79,   // 585 //
    0x01,   // 586 //
    0x00,   // 587 //
    0x30,   // 588 //
    0x44,   // 589 //
    0x45,   // 590 //
    0x4F,   // 591 //
    0x00,   // 592 //
    0x45,   // 593 //
    0x4F,   // 594 //
    0x00,   // 595 //
    0x4A,   // 596 //
    0x41,   // 597 //
    0x41,   // 598 //
    0x40,   // 599 //
    0x41,   // 600 //
    0x40,   // 601 //
    0x00,   // 602 //
    0x50,   // 603 //
    0x4B,   // 604 //
    0x00,   // 605 //
    0x43,   // 606 //
    0x41,   // 607 //
    0x4A,   // 608 //
    0x41,   // 609 //
    0x4E,   // 610 //
    0x3D,   // 611 //
    0x50,   // 612 //
    0x41,   // 613 //
    0x02,   // 614 //
    0x32,   // 615 //
    0x3B,   // 616 //
    0x26,   // 617 //
    0x2B,   // 618 //
    0x28,   // 619 //
    0x30,   // 620 //
    0x79,   // 621 //
    0x01,   // 622 //
    0x00,   // 623 //
    0x30,   // 624 //
    0x44,   // 625 //
    0x45,   // 626 //
    0x4F,   // 627 //
    0x00,   // 628 //
    0x45,   // 629 //
    0x4F,   // 630 //
    0x00,   // 631 //
    0x4A,   // 632 //
    0x4B,   // 633 //
    0x50,   // 634 //
    0x00,   // 635 //
    0x50,   // 636 //
    0x44,   // 637 //
    0x41,   // 638 //
    0x00,   // 639 //
    0x3F,   // 640 //
    0x44,   // 641 //
    0x41,   // 642 //
    0x49,   // 643 //
    0x45,   // 644 //
    0x3F,   // 645 //
    0x3D,   // 646 //
    0x48,   // 647 //
    0x00,   // 648 //
    0x25,   // 649 //
    0x02,   // 650 //
    0x4A,   // 651 //
    0x41,   // 652 //
    0x41,   // 653 //
    0x40,   // 654 //
    0x79,   // 655 //
    0x01,   // 656 //
    0x00,   // 657 //
    0x2A,   // 658 //
    0x4B,   // 659 //
    0x53,   // 660 //
    0x00,   // 661 //
    0x25,   // 662 //
    0x00,   // 663 //
    0x3F,   // 664 //
    0x3D,   // 665 //
    0x4A,   // 666 //
    0x00,   // 667 //
    0x40,   // 668 //
    0x41,   // 669 //
    0x4F,   // 670 //
    0x50,   // 671 //
    0x4E,   // 672 //
    0x4B,   // 673 //
    0x55,   // 674 //
    0x00,   // 675 //
    0x50,   // 676 //
    0x44,   // 677 //
    0x3D,   // 678 //
    0x50,   // 679 //
    0x00,   // 680 //
    0x45,   // 681 //
    0x52,   // 682 //
    0x55,   // 683 //
    0x02,   // 684 //
    0x49,   // 685 //
    0x4B,   // 686 //
    0x4A,   // 687 //
    0x4F,   // 688 //
    0x50,   // 689 //
    0x41,   // 690 //
    0x4E,   // 691 //
    0x79,   // 692 //
    0x01,   // 693 //
    0x00,   // 694 //
    0x25,   // 695 //
    0x50,   // 696 //
    0x3A,   // 697 //
    0x4F,   // 698 //
    0x00,   // 699 //
    0x50,   // 700 //
    0x4B,   // 701 //
    0x4B,   // 702 //
    0x00,   // 703 //
    0x40,   // 704 //
    0x3D,   // 705 //
    0x4A,   // 706 //
    0x43,   // 707 //
    0x41,   // 708 //
    0x4E,   // 709 //
    0x4B,   // 710 //
    0x51,   // 711 //
    0x4F,   // 712 //
    0x00,   // 713 //
    0x50,   // 714 //
    0x4B,   // 715 //
    0x00,   // 716 //
    0x42,   // 717 //
    0x45,   // 718 //
    0x4E,   // 719 //
    0x41,   // 720 //
    0x1A,   // 721 //
    0x02,   // 722 //
    0x20,   // 723 //
    0x4B,   // 724 //
    0x4F,   // 725 //
    0x41,   // 726 //
    0x00,   // 727 //
    0x45,   // 728 //
    0x50,   // 729 //
    0x00,   // 730 //
    0x44,   // 731 //
    0x3D,   // 732 //
    0x52,   // 733 //
    0x41,   // 734 //
    0x00,   // 735 //
    0x3D,   // 736 //
    0x4A,   // 737 //
    0x4B,   // 738 //
    0x50,   // 739 //
    0x44,   // 740 //
    0x41,   // 741 //
    0x4E,   // 742 //
    0x00,   // 743 //
    0x51,   // 744 //
    0x4F,   // 745 //
    0x41,   // 746 //
    0x1B,   // 747 //
    0x01,   // 748 //
    0x00,   // 749 //
    0x30,   // 750 //
    0x44,   // 751 //
    0x41,   // 752 //
    0x4E,   // 753 //
    0x41,   // 754 //
    0x3A,   // 755 //
    0x4F,   // 756 //
    0x00,   // 757 //
    0x3D,   // 758 //
    0x00,   // 759 //
    0x4F,   // 760 //
    0x3F,   // 761 //
    0x4E,   // 762 //
    0x3D,   // 763 //
    0x50,   // 764 //
    0x3F,   // 765 //
    0x44,   // 766 //
    0x79,   // 767 //
    0x02,   // 768 //
    0x2F,   // 769 //
    0x4B,   // 770 //
    0x49,   // 771 //
    0x41,   // 772 //
    0x4B,   // 773 //
    0x4A,   // 774 //
    0x41,   // 775 //
    0x00,   // 776 //
    0x49,   // 777 //
    0x3D,   // 778 //
    0x55,   // 779 //
    0x00,   // 780 //
    0x44,   // 781 //
    0x3D,   // 782 //
    0x52,   // 783 //
    0x41,   // 784 //
    0x00,   // 785 //
    0x51,   // 786 //
    0x4F,   // 787 //
    0x41,   // 788 //
    0x40,   // 789 //
    0x00,   // 790 //
    0x45,   // 791 //
    0x50,   // 792 //
    0x79,   // 793 //
    0x01,   // 794 //
    0x00,   // 795 //
    0x25,   // 796 //
    0x50,   // 797 //
    0x3A,   // 798 //
    0x4F,   // 799 //
    0x00,   // 800 //
    0x4F,   // 801 //
    0x44,   // 802 //
    0x45,   // 803 //
    0x4A,   // 804 //
    0x45,   // 805 //
    0x4A,   // 806 //
    0x43,   // 807 //
    0x00,   // 808 //
    0x3E,   // 809 //
    0x41,   // 810 //
    0x3D,   // 811 //
    0x51,   // 812 //
    0x50,   // 813 //
    0x45,   // 814 //
    0x42,   // 815 //
    0x51,   // 816 //
    0x48,   // 817 //
    0x48,   // 818 //
    0x55,   // 819 //
    0x79,   // 820 //
    0x01,   // 821 //
    0x00,   // 822 //
    0x30,   // 823 //
    0x44,   // 824 //
    0x41,   // 825 //
    0x00,   // 826 //
    0x50,   // 827 //
    0x45,   // 828 //
    0x50,   // 829 //
    0x48,   // 830 //
    0x41,   // 831 //
    0x00,   // 832 //
    0x45,   // 833 //
    0x4F,   // 834 //
    0x00,   // 835 //
    0x78,   // 836 //
    0x29,   // 837 //
    0x4B,   // 838 //
    0x4B,   // 839 //
    0x4A,   // 840 //
    0x48,   // 841 //
    0x45,   // 842 //
    0x43,   // 843 //
    0x44,   // 844 //
    0x50,   // 845 //
    0x02,   // 846 //
    0x2F,   // 847 //
    0x4B,   // 848 //
    0x4A,   // 849 //
    0x3D,   // 850 //
    0x50,   // 851 //
    0x3D,   // 852 //
    0x19,   // 853 //
    0x79,   // 854 //
    0x01,   // 855 //
    0x00,   // 856 //
    0x1D,   // 857 //
    0x00,   // 858 //
    0x49,   // 859 //
    0x41,   // 860 //
    0x40,   // 861 //
    0x3D,   // 862 //
    0x48,   // 863 //
    0x00,   // 864 //
    0x42,   // 865 //
    0x4E,   // 866 //
    0x4B,   // 867 //
    0x49,   // 868 //
    0x00,   // 869 //
    0x50,   // 870 //
    0x44,   // 871 //
    0x41,   // 872 //
    0x02,   // 873 //
    0x4F,   // 874 //
    0x41,   // 875 //
    0x3F,   // 876 //
    0x4B,   // 877 //
    0x4A,   // 878 //
    0x40,   // 879 //
    0x00,   // 880 //
    0x20,   // 881 //
    0x4B,   // 882 //
    0x4B,   // 883 //
    0x49,   // 884 //
    0x00,   // 885 //
    0x1E,   // 886 //
    0x4B,   // 887 //
    0x4B,   // 888 //
    0x47,   // 889 //
    0x79,   // 890 //
    0x01,   // 891 //
    0x00,   // 892 //
    0x1D,   // 893 //
    0x00,   // 894 //
    0x49,   // 895 //
    0x41,   // 896 //
    0x40,   // 897 //
    0x3D,   // 898 //
    0x48,   // 899 //
    0x00,   // 900 //
    0x42,   // 901 //
    0x4E,   // 902 //
    0x4B,   // 903 //
    0x49,   // 904 //
    0x00,   // 905 //
    0x50,   // 906 //
    0x44,   // 907 //
    0x41,   // 908 //
    0x02,   // 909 //
    0x42,   // 910 //
    0x45,   // 911 //
    0x4E,   // 912 //
    0x4F,   // 913 //
    0x50,   // 914 //
    0x00,   // 915 //
    0x20,   // 916 //
    0x4B,   // 917 //
    0x4B,   // 918 //
    0x49,   // 919 //
    0x00,   // 920 //
    0x1E,   // 921 //
    0x4B,   // 922 //
    0x4B,   // 923 //
    0x47,   // 924 //
    0x79,   // 925 //
    0x01,   // 926 //
    0x00,   // 927 //
    0x2A,   // 928 //
    0x4B,   // 929 //
    0x53,   // 930 //
    0x00,   // 931 //
    0x25,   // 932 //
    0x00,   // 933 //
    0x3F,   // 934 //
    0x3D,   // 935 //
    0x4A,   // 936 //
    0x00,   // 937 //
    0x49,   // 938 //
    0x4B,   // 939 //
    0x52,   // 940 //
    0x41,   // 941 //
    0x00,   // 942 //
    0x50,   // 943 //
    0x44,   // 944 //
    0x41,   // 945 //
    0x02,   // 946 //
    0x41,   // 947 //
    0x48,   // 948 //
    0x41,   // 949 //
    0x52,   // 950 //
    0x3D,   // 951 //
    0x50,   // 952 //
    0x4B,   // 953 //
    0x4E,   // 954 //
    0x79,   // 955 //
    0x01,   // 956 //
    0x00,   // 957 //
    0x25,   // 958 //
    0x50,   // 959 //
    0x00,   // 960 //
    0x4F,   // 961 //
    0x41,   // 962 //
    0x41,   // 963 //
    0x49,   // 964 //
    0x4F,   // 965 //
    0x00,   // 966 //
    0x50,   // 967 //
    0x4B,   // 968 //
    0x00,   // 969 //
    0x3E,   // 970 //
    0x41,   // 971 //
    0x00,   // 972 //
    0x3D,   // 973 //
    0x00,   // 974 //
    0x4F,   // 975 //
    0x50,   // 976 //
    0x3D,   // 977 //
    0x4E,   // 978 //
    0x50,   // 979 //
    0x3B,   // 980 //
    0x51,   // 981 //
    0x4C,   // 982 //
    0x00,   // 983 //
    0x02,   // 984 //
    0x40,   // 985 //
    0x45,   // 986 //
    0x4F,   // 987 //
    0x47,   // 988 //
    0x79,   // 989 //
    0x01,   // 990 //
    0x00,   // 991 //
    0x25,   // 992 //
    0x50,   // 993 //
    0x00,   // 994 //
    0x4E,   // 995 //
    0x41,   // 996 //
    0x4C,   // 997 //
    0x4E,   // 998 //
    0x41,   // 999 //
    0x4F,   // 1000 //
    0x41,   // 1001 //
    0x4A,   // 1002 //
    0x50,   // 1003 //
    0x4F,   // 1004 //
    0x00,   // 1005 //
    0x53,   // 1006 //
    0x45,   // 1007 //
    0x4A,   // 1008 //
    0x40,   // 1009 //
    0x79,   // 1010 //
    0x01,   // 1011 //
    0x00,   // 1012 //
    0x25,   // 1013 //
    0x00,   // 1014 //
    0x3F,   // 1015 //
    0x3D,   // 1016 //
    0x4A,   // 1017 //
    0x00,   // 1018 //
    0x4F,   // 1019 //
    0x45,   // 1020 //
    0x43,   // 1021 //
    0x4A,   // 1022 //
    0x3D,   // 1023 //
    0x48,   // 1024 //
    0x00,   // 1025 //
    0x1E,   // 1026 //
    0x4E,   // 1027 //
    0x3D,   // 1028 //
    0x40,   // 1029 //
    0x00,   // 1030 //
    0x53,   // 1031 //
    0x45,   // 1032 //
    0x50,   // 1033 //
    0x44,   // 1034 //
    0x00,   // 1035 //
    0x50,   // 1036 //
    0x44,   // 1037 //
    0x45,   // 1038 //
    0x4F,   // 1039 //
    0x79,   // 1040 //
    0x01,   // 1041 //
    0x00,   // 1042 //
    0x25,   // 1043 //
    0x50,   // 1044 //
    0x00,   // 1045 //
    0x4F,   // 1046 //
    0x41,   // 1047 //
    0x41,   // 1048 //
    0x49,   // 1049 //
    0x4F,   // 1050 //
    0x00,   // 1051 //
    0x50,   // 1052 //
    0x4B,   // 1053 //
    0x00,   // 1054 //
    0x3E,   // 1055 //
    0x41,   // 1056 //
    0x00,   // 1057 //
    0x4F,   // 1058 //
    0x4B,   // 1059 //
    0x49,   // 1060 //
    0x41,   // 1061 //
    0x00,   // 1062 //
    0x47,   // 1063 //
    0x45,   // 1064 //
    0x4A,   // 1065 //
    0x40,   // 1066 //
    0x02,   // 1067 //
    0x4B,   // 1068 //
    0x42,   // 1069 //
    0x00,   // 1070 //
    0x4E,   // 1071 //
    0x41,   // 1072 //
    0x4F,   // 1073 //
    0x41,   // 1074 //
    0x3D,   // 1075 //
    0x4E,   // 1076 //
    0x3F,   // 1077 //
    0x44,   // 1078 //
    0x00,   // 1079 //
    0x4E,   // 1080 //
    0x41,   // 1081 //
    0x4C,   // 1082 //
    0x4B,   // 1083 //
    0x4E,   // 1084 //
    0x50,   // 1085 //
    0x79,   // 1086 //
    0x01,   // 1087 //
    0x00,   // 1088 //
    0x1D,   // 1089 //
    0x00,   // 1090 //
    0x3F,   // 1091 //
    0x3D,   // 1092 //
    0x4E,   // 1093 //
    0x52,   // 1094 //
    0x45,   // 1095 //
    0x4A,   // 1096 //
    0x43,   // 1097 //
    0x00,   // 1098 //
    0x4B,   // 1099 //
    0x42,   // 1100 //
    0x00,   // 1101 //
    0x50,   // 1102 //
    0x44,   // 1103 //
    0x41,   // 1104 //
    0x00,   // 1105 //
    0x49,   // 1106 //
    0x4B,   // 1107 //
    0x4B,   // 1108 //
    0x4A,   // 1109 //
    0x79,   // 1110 //
    0x01,   // 1111 //
    0x00,   // 1112 //
    0x1D,   // 1113 //
    0x00,   // 1114 //
    0x3F,   // 1115 //
    0x3D,   // 1116 //
    0x4E,   // 1117 //
    0x52,   // 1118 //
    0x45,   // 1119 //
    0x4A,   // 1120 //
    0x43,   // 1121 //
    0x00,   // 1122 //
    0x4B,   // 1123 //
    0x42,   // 1124 //
    0x00,   // 1125 //
    0x50,   // 1126 //
    0x44,   // 1127 //
    0x41,   // 1128 //
    0x00,   // 1129 //
    0x4F,   // 1130 //
    0x50,   // 1131 //
    0x3D,   // 1132 //
    0x4E,   // 1133 //
    0x79,   // 1134 //
    0x01,   // 1135 //
    0x00,   // 1136 //
    0x1D,   // 1137 //
    0x00,   // 1138 //
    0x3F,   // 1139 //
    0x3D,   // 1140 //
    0x4E,   // 1141 //
    0x52,   // 1142 //
    0x45,   // 1143 //
    0x4A,   // 1144 //
    0x43,   // 1145 //
    0x00,   // 1146 //
    0x4B,   // 1147 //
    0x42,   // 1148 //
    0x00,   // 1149 //
    0x50,   // 1150 //
    0x44,   // 1151 //
    0x41,   // 1152 //
    0x00,   // 1153 //
    0x4F,   // 1154 //
    0x51,   // 1155 //
    0x4A,   // 1156 //
    0x79,   // 1157 //
    0x01,   // 1158 //
    0x00,   // 1159 //
    0x25,   // 1160 //
    0x50,   // 1161 //
    0x3A,   // 1162 //
    0x4F,   // 1163 //
    0x00,   // 1164 //
    0x51,   // 1165 //
    0x4F,   // 1166 //
    0x41,   // 1167 //
    0x40,   // 1168 //
    0x00,   // 1169 //
    0x53,   // 1170 //
    0x45,   // 1171 //
    0x50,   // 1172 //
    0x44,   // 1173 //
    0x00,   // 1174 //
    0x3D,   // 1175 //
    0x02,   // 1176 //
    0x50,   // 1177 //
    0x55,   // 1178 //
    0x4C,   // 1179 //
    0x41,   // 1180 //
    0x53,   // 1181 //
    0x4E,   // 1182 //
    0x45,   // 1183 //
    0x50,   // 1184 //
    0x41,   // 1185 //
    0x4E,   // 1186 //
    0x79,   // 1187 //
    0x01,   // 1188 //
    0x00,   // 1189 //
    0x2F,   // 1190 //
    0x4B,   // 1191 //
    0x49,   // 1192 //
    0x41,   // 1193 //
    0x00,   // 1194 //
    0x42,   // 1195 //
    0x48,   // 1196 //
    0x51,   // 1197 //
    0x45,   // 1198 //
    0x40,   // 1199 //
    0x00,   // 1200 //
    0x45,   // 1201 //
    0x4F,   // 1202 //
    0x00,   // 1203 //
    0x48,   // 1204 //
    0x41,   // 1205 //
    0x42,   // 1206 //
    0x50,   // 1207 //
    0x79,   // 1208 //
    0x01,   // 1209 //
    0x00,   // 1210 //
    0x2A,   // 1211 //
    0x4B,   // 1212 //
    0x50,   // 1213 //
    0x44,   // 1214 //
    0x45,   // 1215 //
    0x4A,   // 1216 //
    0x43,   // 1217 //
    0x00,   // 1218 //
    0x4F,   // 1219 //
    0x4C,   // 1220 //
    0x41,   // 1221 //
    0x3F,   // 1222 //
    0x45,   // 1223 //
    0x3D,   // 1224 //
    0x48,   // 1225 //
    0x79,   // 1226 //
    0x01,   // 1227 //
    0x00,   // 1228 //
    0x2A,   // 1229 //
    0x4B,   // 1230 //
    0x50,   // 1231 //
    0x44,   // 1232 //
    0x45,   // 1233 //
    0x4A,   // 1234 //
    0x43,   // 1235 //
    0x00,   // 1236 //
    0x45,   // 1237 //
    0x49,   // 1238 //
    0x4C,   // 1239 //
    0x4B,   // 1240 //
    0x4E,   // 1241 //
    0x50,   // 1242 //
    0x3D,   // 1243 //
    0x4A,   // 1244 //
    0x50,   // 1245 //
    0x79,   // 1246 //
    0x01,   // 1247 //
    0x00,   // 1248 //
    0x1D,   // 1249 //
    0x00,   // 1250 //
    0x3F,   // 1251 //
    0x3D,   // 1252 //
    0x4E,   // 1253 //
    0x52,   // 1254 //
    0x45,   // 1255 //
    0x4A,   // 1256 //
    0x43,   // 1257 //
    0x00,   // 1258 //
    0x4B,   // 1259 //
    0x42,   // 1260 //
    0x00,   // 1261 //
    0x50,   // 1262 //
    0x44,   // 1263 //
    0x41,   // 1264 //
    0x00,   // 1265 //
    0x49,   // 1266 //
    0x4B,   // 1267 //
    0x4B,   // 1268 //
    0x4A,   // 1269 //
    0x79,   // 1270 //
    0x01,   // 1271 //
    0x00,   // 1272 //
    0x1D,   // 1273 //
    0x00,   // 1274 //
    0x47,   // 1275 //
    0x41,   // 1276 //
    0x55,   // 1277 //
    0x00,   // 1278 //
    0x50,   // 1279 //
    0x4B,   // 1280 //
    0x00,   // 1281 //
    0x41,   // 1282 //
    0x4A,   // 1283 //
    0x50,   // 1284 //
    0x41,   // 1285 //
    0x4E,   // 1286 //
    0x00,   // 1287 //
    0x50,   // 1288 //
    0x44,   // 1289 //
    0x41,   // 1290 //
    0x00,   // 1291 //
    0x1F,   // 1292 //
    0x4B,   // 1293 //
    0x4A,   // 1294 //
    0x50,   // 1295 //
    0x4E,   // 1296 //
    0x4B,   // 1297 //
    0x48,   // 1298 //
    0x02,   // 1299 //
    0x2E,   // 1300 //
    0x4B,   // 1301 //
    0x4B,   // 1302 //
    0x49,   // 1303 //
    0x79,   // 1304 //
    0x01,   // 1305 //
    0x00,   // 1306 //
    0x25,   // 1307 //
    0x00,   // 1308 //
    0x3F,   // 1309 //
    0x3D,   // 1310 //
    0x4A,   // 1311 //
    0x00,   // 1312 //
    0x44,   // 1313 //
    0x41,   // 1314 //
    0x3D,   // 1315 //
    0x48,   // 1316 //
    0x00,   // 1317 //
    0x3D,   // 1318 //
    0x4A,   // 1319 //
    0x55,   // 1320 //
    0x00,   // 1321 //
    0x53,   // 1322 //
    0x4B,   // 1323 //
    0x51,   // 1324 //
    0x4A,   // 1325 //
    0x40,   // 1326 //
    0x00,   // 1327 //
    0x53,   // 1328 //
    0x45,   // 1329 //
    0x50,   // 1330 //
    0x44,   // 1331 //
    0x02,   // 1332 //
    0x50,   // 1333 //
    0x44,   // 1334 //
    0x45,   // 1335 //
    0x4F,   // 1336 //
    0x79,   // 1337 //
    0x01,   // 1338 //
    0x00,   // 1339 //
    0x2B,   // 1340 //
    0x4A,   // 1341 //
    0x48,   // 1342 //
    0x55,   // 1343 //
    0x00,   // 1344 //
    0x4B,   // 1345 //
    0x4A,   // 1346 //
    0x41,   // 1347 //
    0x00,   // 1348 //
    0x40,   // 1349 //
    0x4B,   // 1350 //
    0x4F,   // 1351 //
    0x41,   // 1352 //
    0x00,   // 1353 //
    0x45,   // 1354 //
    0x4F,   // 1355 //
    0x00,   // 1356 //
    0x48,   // 1357 //
    0x41,   // 1358 //
    0x42,   // 1359 //
    0x50,   // 1360 //
    0x79,   // 1361 //
    0x01,   // 1362 //
    0x00,   // 1363 //
    0x25,   // 1364 //
    0x50,   // 1365 //
    0x3A,   // 1366 //
    0x4F,   // 1367 //
    0x00,   // 1368 //
    0x3D,   // 1369 //
    0x00,   // 1370 //
    0x48,   // 1371 //
    0x4B,   // 1372 //
    0x3F,   // 1373 //
    0x3D,   // 1374 //
    0x48,   // 1375 //
    0x00,   // 1376 //
    0x44,   // 1377 //
    0x41,   // 1378 //
    0x4E,   // 1379 //
    0x3E,   // 1380 //
    0x79,   // 1381 //
    0x01,   // 1382 //
    0x00,   // 1383 //
    0x25,   // 1384 //
    0x50,   // 1385 //
    0x3A,   // 1386 //
    0x4F,   // 1387 //
    0x00,   // 1388 //
    0x3D,   // 1389 //
    0x00,   // 1390 //
    0x49,   // 1391 //
    0x45,   // 1392 //
    0x54,   // 1393 //
    0x50,   // 1394 //
    0x51,   // 1395 //
    0x4E,   // 1396 //
    0x41,   // 1397 //
    0x00,   // 1398 //
    0x4B,   // 1399 //
    0x42,   // 1400 //
    0x00,   // 1401 //
    0x43,   // 1402 //
    0x4E,   // 1403 //
    0x41,   // 1404 //
    0x41,   // 1405 //
    0x4A,   // 1406 //
    0x02,   // 1407 //
    0x3D,   // 1408 //
    0x4A,   // 1409 //
    0x40,   // 1410 //
    0x00,   // 1411 //
    0x4E,   // 1412 //
    0x41,   // 1413 //
    0x40,   // 1414 //
    0x00,   // 1415 //
    0x44,   // 1416 //
    0x41,   // 1417 //
    0x4E,   // 1418 //
    0x3E,   // 1419 //
    0x4F,   // 1420 //
    0x79,   // 1421 //
    0x01,   // 1422 //
    0x00,   // 1423 //
    0x25,   // 1424 //
    0x50,   // 1425 //
    0x3A,   // 1426 //
    0x4F,   // 1427 //
    0x00,   // 1428 //
    0x3D,   // 1429 //
    0x00,   // 1430 //
    0x49,   // 1431 //
    0x45,   // 1432 //
    0x54,   // 1433 //
    0x50,   // 1434 //
    0x51,   // 1435 //
    0x4E,   // 1436 //
    0x41,   // 1437 //
    0x00,   // 1438 //
    0x4B,   // 1439 //
    0x42,   // 1440 //
    0x00,   // 1441 //
    0x0E,   // 1442 //
    0x00,   // 1443 //
    0x43,   // 1444 //
    0x4E,   // 1445 //
    0x41,   // 1446 //
    0x41,   // 1447 //
    0x4A,   // 1448 //
    0x02,   // 1449 //
    0x44,   // 1450 //
    0x41,   // 1451 //
    0x4E,   // 1452 //
    0x3E,   // 1453 //
    0x4F,   // 1454 //
    0x79,   // 1455 //
    0x01,   // 1456 //
    0x00,   // 1457 //
    0x25,   // 1458 //
    0x50,   // 1459 //
    0x3A,   // 1460 //
    0x4F,   // 1461 //
    0x00,   // 1462 //
    0x3D,   // 1463 //
    0x00,   // 1464 //
    0x49,   // 1465 //
    0x45,   // 1466 //
    0x54,   // 1467 //
    0x50,   // 1468 //
    0x51,   // 1469 //
    0x4E,   // 1470 //
    0x41,   // 1471 //
    0x00,   // 1472 //
    0x4B,   // 1473 //
    0x42,   // 1474 //
    0x00,   // 1475 //
    0x43,   // 1476 //
    0x4E,   // 1477 //
    0x41,   // 1478 //
    0x41,   // 1479 //
    0x4A,   // 1480 //
    0x02,   // 1481 //
    0x3D,   // 1482 //
    0x4A,   // 1483 //
    0x40,   // 1484 //
    0x00,   // 1485 //
    0x3E,   // 1486 //
    0x48,   // 1487 //
    0x51,   // 1488 //
    0x41,   // 1489 //
    0x00,   // 1490 //
    0x44,   // 1491 //
    0x41,   // 1492 //
    0x4E,   // 1493 //
    0x3E,   // 1494 //
    0x4F,   // 1495 //
    0x79,   // 1496 //
    0x01,   // 1497 //
    0x00,   // 1498 //
    0x25,   // 1499 //
    0x50,   // 1500 //
    0x3A,   // 1501 //
    0x4F,   // 1502 //
    0x00,   // 1503 //
    0x3D,   // 1504 //
    0x00,   // 1505 //
    0x49,   // 1506 //
    0x45,   // 1507 //
    0x54,   // 1508 //
    0x50,   // 1509 //
    0x51,   // 1510 //
    0x4E,   // 1511 //
    0x41,   // 1512 //
    0x00,   // 1513 //
    0x4B,   // 1514 //
    0x42,   // 1515 //
    0x00,   // 1516 //
    0x43,   // 1517 //
    0x4E,   // 1518 //
    0x41,   // 1519 //
    0x41,   // 1520 //
    0x4A,   // 1521 //
    0x18,   // 1522 //
    0x02,   // 1523 //
    0x3E,   // 1524 //
    0x48,   // 1525 //
    0x51,   // 1526 //
    0x41,   // 1527 //
    0x00,   // 1528 //
    0x3D,   // 1529 //
    0x4A,   // 1530 //
    0x40,   // 1531 //
    0x00,   // 1532 //
    0x4E,   // 1533 //
    0x41,   // 1534 //
    0x40,   // 1535 //
    0x00,   // 1536 //
    0x44,   // 1537 //
    0x41,   // 1538 //
    0x4E,   // 1539 //
    0x3E,   // 1540 //
    0x4F,   // 1541 //
    0x79,   // 1542 //
    0x01,   // 1543 //
    0x00,   // 1544 //
    0x25,   // 1545 //
    0x50,   // 1546 //
    0x3A,   // 1547 //
    0x4F,   // 1548 //
    0x00,   // 1549 //
    0x3D,   // 1550 //
    0x00,   // 1551 //
    0x49,   // 1552 //
    0x45,   // 1553 //
    0x54,   // 1554 //
    0x50,   // 1555 //
    0x51,   // 1556 //
    0x4E,   // 1557 //
    0x41,   // 1558 //
    0x00,   // 1559 //
    0x4B,   // 1560 //
    0x42,   // 1561 //
    0x00,   // 1562 //
    0x0F,   // 1563 //
    0x00,   // 1564 //
    0x43,   // 1565 //
    0x4E,   // 1566 //
    0x41,   // 1567 //
    0x41,   // 1568 //
    0x4A,   // 1569 //
    0x02,   // 1570 //
    0x44,   // 1571 //
    0x41,   // 1572 //
    0x4E,   // 1573 //
    0x3E,   // 1574 //
    0x4F,   // 1575 //
    0x79,   // 1576 //
    0x01,   // 1577 //
    0x00,   // 1578 //
    0x25,   // 1579 //
    0x50,   // 1580 //
    0x3A,   // 1581 //
    0x4F,   // 1582 //
    0x00,   // 1583 //
    0x3D,   // 1584 //
    0x00,   // 1585 //
    0x49,   // 1586 //
    0x45,   // 1587 //
    0x54,   // 1588 //
    0x50,   // 1589 //
    0x51,   // 1590 //
    0x4E,   // 1591 //
    0x41,   // 1592 //
    0x00,   // 1593 //
    0x4B,   // 1594 //
    0x42,   // 1595 //
    0x00,   // 1596 //
    0x0E,   // 1597 //
    0x00,   // 1598 //
    0x43,   // 1599 //
    0x4E,   // 1600 //
    0x41,   // 1601 //
    0x41,   // 1602 //
    0x4A,   // 1603 //
    0x02,   // 1604 //
    0x44,   // 1605 //
    0x41,   // 1606 //
    0x4E,   // 1607 //
    0x3E,   // 1608 //
    0x4F,   // 1609 //
    0x00,   // 1610 //
    0x3D,   // 1611 //
    0x4A,   // 1612 //
    0x40,   // 1613 //
    0x00,   // 1614 //
    0x3E,   // 1615 //
    0x48,   // 1616 //
    0x51,   // 1617 //
    0x41,   // 1618 //
    0x00,   // 1619 //
    0x44,   // 1620 //
    0x41,   // 1621 //
    0x4E,   // 1622 //
    0x3E,   // 1623 //
    0x79,   // 1624 //
    0x01,   // 1625 //
    0x00,   // 1626 //
    0x30,   // 1627 //
    0x44,   // 1628 //
    0x41,   // 1629 //
    0x00,   // 1630 //
    0x3E,   // 1631 //
    0x3D,   // 1632 //
    0x50,   // 1633 //
    0x50,   // 1634 //
    0x41,   // 1635 //
    0x4E,   // 1636 //
    0x55,   // 1637 //
    0x00,   // 1638 //
    0x45,   // 1639 //
    0x4F,   // 1640 //
    0x00,   // 1641 //
    0x4F,   // 1642 //
    0x50,   // 1643 //
    0x45,   // 1644 //
    0x48,   // 1645 //
    0x48,   // 1646 //
    0x02,   // 1647 //
    0x3F,   // 1648 //
    0x44,   // 1649 //
    0x3D,   // 1650 //
    0x4E,   // 1651 //
    0x43,   // 1652 //
    0x41,   // 1653 //
    0x40,   // 1654 //
    0x79,   // 1655 //
    0x01,   // 1656 //
    0x00,   // 1657 //
    0x1D,   // 1658 //
    0x00,   // 1659 //
    0x3E,   // 1660 //
    0x41,   // 1661 //
    0x4E,   // 1662 //
    0x41,   // 1663 //
    0x50,   // 1664 //
    0x50,   // 1665 //
    0x3D,   // 1666 //
    0x00,   // 1667 //
    0x29,   // 1668 //
    0x15,   // 1669 //
    0x0E,   // 1670 //
    0x22,   // 1671 //
    0x2F,   // 1672 //
    0x00,   // 1673 //
    0x1D,   // 1674 //
    0x51,   // 1675 //
    0x50,   // 1676 //
    0x4B,   // 1677 //
    0x49,   // 1678 //
    0x3D,   // 1679 //
    0x50,   // 1680 //
    0x45,   // 1681 //
    0x3F,   // 1682 //
    0x79,   // 1683 //
    0x02,   // 1684 //
    0x1F,   // 1685 //
    0x51,   // 1686 //
    0x4F,   // 1687 //
    0x50,   // 1688 //
    0x4B,   // 1689 //
    0x49,   // 1690 //
    0x00,   // 1691 //
    0x41,   // 1692 //
    0x40,   // 1693 //
    0x45,   // 1694 //
    0x50,   // 1695 //
    0x45,   // 1696 //
    0x4B,   // 1697 //
    0x4A,   // 1698 //
    0x79,   // 1699 //
    0x01,   // 1700 //
    0x00,   // 1701 //
    0x00,   // 1702 //
    0x00,   // 1703 //
};

// description string of each item, by item id - 1
UCHAR* Item_mess_tbl[77] =
{
    Item_mess_dat,           // 0 //
    Item_mess_dat + 0x32,    // 1 //
    Item_mess_dat + 0x65,    // 2 //
    Item_mess_dat + 0x67A,   // 3 //
    Item_mess_dat + 0x8D,    // 4 //
    Item_mess_dat + 0xC2,    // 5 //
    Item_mess_dat + 0xEE,    // 6 //
    Item_mess_dat + 0xEE,    // 7 //
    Item_mess_dat + 0xEE,    // 8 //
    Item_mess_dat + 0x11D,   // 9 //
    Item_mess_dat + 0x13F,   // 10 //
    Item_mess_dat + 0x152,   // 11 //
    Item_mess_dat + 0x16B,   // 12 //
    Item_mess_dat + 0x1A1,   // 13 //
    Item_mess_dat + 0x1CB,   // 14 //
    Item_mess_dat + 0x1E7,   // 15 //
    Item_mess_dat + 0x1E7,   // 16 //
    Item_mess_dat + 0x1E7,   // 17 //
    Item_mess_dat + 0x209,   // 18 //
    Item_mess_dat + 0x22E,   // 19 //
    Item_mess_dat + 0x24C,   // 20 //
    Item_mess_dat + 0x24C,   // 21 //
    Item_mess_dat + 0x270,   // 22 //
    Item_mess_dat + 0x270,   // 23 //
    Item_mess_dat + 0x270,   // 24 //
    Item_mess_dat + 0x270,   // 25 //
    Item_mess_dat + 0x292,   // 26 //
    Item_mess_dat + 0x2B7,   // 27 //
    Item_mess_dat + 0x4CD,   // 28 //
    Item_mess_dat + 0x4CD,   // 29 //
    Item_mess_dat + 0x2EE,   // 30 //
    Item_mess_dat + 0x2EE,   // 31 //
    Item_mess_dat + 0x31C,   // 32 //
    Item_mess_dat + 0x31C,   // 33 //
    Item_mess_dat + 0x337,   // 34 //
    Item_mess_dat + 0x359,   // 35 //
    Item_mess_dat + 0x37D,   // 36 //
    Item_mess_dat + 0x4CD,   // 37 //
    Item_mess_dat + 0x3A0,   // 38 //
    Item_mess_dat + 0x3BE,   // 39 //
    Item_mess_dat + 0x3E0,   // 40 //
    Item_mess_dat + 0x3F5,   // 41 //
    Item_mess_dat + 0x413,   // 42 //
    Item_mess_dat + 0x441,   // 43 //
    Item_mess_dat + 0x459,   // 44 //
    Item_mess_dat + 0x471,   // 45 //
    Item_mess_dat + 0x488,   // 46 //
    Item_mess_dat + 0x4A6,   // 47 //
    Item_mess_dat + 0x4CD,   // 48 //
    Item_mess_dat + 0x4CD,   // 49 //
    Item_mess_dat + 0x4CD,   // 50 //
    Item_mess_dat + 0x4CD,   // 51 //
    Item_mess_dat + 0x4CD,   // 52 //
    Item_mess_dat + 0x4CD,   // 53 //
    Item_mess_dat + 0x4CD,   // 54 //
    Item_mess_dat + 0x4CD,   // 55 //
    Item_mess_dat + 0x4CD,   // 56 //
    Item_mess_dat + 0x4CD,   // 57 //
    Item_mess_dat + 0x4F9,   // 58 //
    Item_mess_dat + 0x4CD,   // 59 //
    Item_mess_dat + 0x4CD,   // 60 //
    Item_mess_dat + 0x4CD,   // 61 //
    Item_mess_dat + 0x4CD,   // 62 //
    Item_mess_dat + 0x4CD,   // 63 //
    Item_mess_dat + 0x51B,   // 64 //
    Item_mess_dat + 0x53C,   // 65 //
    Item_mess_dat + 0x554,   // 66 //
    Item_mess_dat + 0x554,   // 67 //
    Item_mess_dat + 0x554,   // 68 //
    Item_mess_dat + 0x568,   // 69 //
    Item_mess_dat + 0x590,   // 70 //
    Item_mess_dat + 0x5B2,   // 71 //
    Item_mess_dat + 0x5DB,   // 72 //
    Item_mess_dat + 0x609,   // 73 //
    Item_mess_dat + 0x62B,   // 74 //
    Item_mess_dat + 0x4CD,   // 75 //
    Item_mess_dat + 0x65B,   // 76 //
};
