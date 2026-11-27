/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SCE_AT.C
 */

#include "TYPES.H"
#include <LIBETC.H>
#include "SCE_AT.H"
#include "CUT.H"
#include "EFF1.H"
#include "EVENT.H"
#include "GAME.H"
#include "INVENTORY.H"
#include "MAP.H"
#include "MEM_CARD.H"
#include "MOJIDISP.H"
#include "PAD.H"
#include "SCE0.H"
#include "SCE1.H"
#include "SND.H"
#include "ST_SUB02.H"
#include "ST_SUB03.H"
#include "SCENARIO.H"
#include "FLAG.H"

// functions without a C definition
INT Mess_set();

PSP Room_psp[64];  // sprite records of the current camera's room sprites
SCE_AOT* pAot_data;  // room action entry of the active pickup, item box, save or drawer sequence
ENEMY_WORK* pPush_om;  // object model being pushed by the player
OBJ_MODEL_WORK* Oma_climb_om;  // room object model the player is Climb_flg

static INT Key_work;  // item slot of the lock item looked up by the drawer sequence
static USHORT Dir_spd;  // current swing speed of the item box lid
static USHORT Spd_add;  // swing speed change per frame of the item box lid
static OBJ_MODEL_WORK* pOwork;  // lid object model of the item box being opened
static INT Ink_no;  // item slot of the ink ribbon found by the typewriter entry

VOID Rot_vector(SHORT dir, SVECTOR* p, SVECTOR* q)
{
    SVECTOR rot;

    rot.vy = dir;
    rot.vz = 0;
    rot.vx = 0;
    RotMatrix(&rot, &ScratchPad->Mat);
    ApplyMatrix(&ScratchPad->Mat, p, &ScratchPad->Vec);
    q->vx = ScratchPad->Vec.vx;
    q->vy = ScratchPad->Vec.vy;
    q->vz = ScratchPad->Vec.vz;
}

INT Hani_ck(VECTOR* p, SEARCH_AOT* cp)
{
    if (cp->W < (UINT)(p->vx - cp->X) || cp->D < (UINT)(p->vz - cp->Z))  // unsigned conversion (signed operand)
    {
        return 0;
    }
    return 1;
}

INT Sce_Door(SCE_AOT* pAot)
{
    UINT Key_type;
    UCHAR Key_id;

    if (G.System_flg & SYS_CLIMB)
    {
        return 0;
    }
    if ((pAot->pRec->Key_id & 0x40) && (G.Player.Id & 3) == 3)
    {
        Mess_set(0xD6, 0xFF);

        return 0;
    }
    Key_id = pAot->pRec->Key_id;
    if ((Key_id & 0x80) && Flg_ck(&MAIN.SaveData.Key_flg, Key_id & 0x3F) == 0)  // address passed as an integer
    {
        Key_type = pAot->pRec->Key_type;

        if (Key_type == 0x33 && (G.Player.Id & 3) == 1)
        {
            if (Flg_ck(&MAIN.SaveData.Scenario_flg, 0x7C) != 0)  // address passed as an integer
            {
                G.SaveData.Sel_id = 0xD;

                goto play;
            }
            Key_type = 0xD;
            goto sound;
        }
        if (Key_type == 0xFE)
        {
            Mess_set(0xD4, 0xFF);

            Snd_se_play(SE_BANK_ROOM, 0x21, 0);
            Flg_on(&MAIN.SaveData.Key_flg, pAot->pRec->Key_id & 0x3F);  // address passed as an integer
            goto ret0;
        }
        if (Key_type == 0xFF)
        {
            Key_type = 0xB;
            goto sound;
        }
        if ((INT)Search_item_id(Key_type) >= 0)  // signed conversion (unsigned operand)
        {
            G.Evt_use = 1;

            G.SaveData.Sel_id = Key_type;

        play:
            Mess_set(0xC3, 0xFF);

            if (STAGE_NO_NOW == 4 && G.SaveData.Room_no == 5)
            {
                Snd_se_play(SE_BANK_BGM, 0x17, 0);
                Snd_se_play(SE_BANK_BGM, 0x18, 0);
                Snd_se_play(SE_BANK_BGM, 0x19, 0);
            }
            else
            {
                Snd_se_play(SE_BANK_ROOM, 0x22, 0);
            }
            Flg_on(&MAIN.SaveData.Key_flg, pAot->pRec->Key_id & 0x3F);  // address passed as an integer
            return 0;
        }

        Key_type -= 0x33;
        if (Key_type >= 10)
        {
            Key_type = 10;
        }
    sound:
        Snd_se_play(SE_BANK_ROOM, 0x14, 0);

        Mess_set(Key_type + 0xC8, 0xFF);
    ret0:
        return 0;
    }
    {
        IN_DOOR_WORK* pDoor = (IN_DOOR_WORK*)pAot->pRec;
        G.Menu_req = 1;
        G.Stop_flg = 0;
        G.pSce_aot_on = pDoor;
    }
    MAIN.System_flg |= SYS_DOOR_LOAD;
    return 0;
}

INT Sce_Message(SCE_AOT* pAot)
{
    Mess_set(pAot->Data0, pAot->Data1);
    return 0;
}

INT Sce_lock_ck(SCE_AOT* pAot)
{
    if (G.SaveData.Equip_no == 0 || (&pItem_work_base[G.SaveData.Equip_no])[-1].Id != pAot->pRec->Item_id)
    {
        pAot_data = pAot;
        Mess_set(0xC1, 0xFF);
    }
    return 0;
}

INT Sce_Item(SCE_AOT* pAot)
{
    pAot_data = pAot;
    if (pAot->Data0 == 0)
    {
        MAIN.System_flg |= SYS_REQ_ITEM_GET;
        return 0;
    }
    return 1;
}

INT Sce_stairs_x(SCE_AOT* pAot)
{
    ENEMY_WORK* pEm;
    ENEMY_WORK* pEm2;

    SEARCH_AOT* pZone;

    pZone = pAot->pRec;
    if (G.pEm->Pos_x - pZone->X < (pZone->W >> 1))
    {
        if ((G.pEm->Cdir_y + 0x400) & 0x800)
        {
            goto end;
        }

        G.pEm->Timer0 = 1;
    }
    else
    {
        if ((G.pEm->Cdir_y - 0x400) & 0x800)
        {
            goto end;
        }
        G.pEm->Timer0 = -1;
    }

    EM_CUR->Routine_1 = 0;
    pEm = MAIN.pEm;
    STOP_FLG_CUR &= ~STOP_ACTION;
    pEm->Zone_in_flg = pEm->Zone_in_flg & 0xEF;
    pEm2 = MAIN.pEm;
    if (((pEm2->Timer0 >> 1) ^ pAot->Data0) & 1)
    {
        pEm2->Zone_in_flg = pEm2->Zone_in_flg | 0x10;
    }
end:
    G.pEm->Zone_in_flg = G.pEm->Zone_in_flg | 0x20;
    STATUS_FLG_CUR |= STAT_STAIRS_ZONE;
    return 0;
}

INT Sce_stairs_z(SCE_AOT* pAot)
{
    ENEMY_WORK* pEm;
    ENEMY_WORK* pEm2;

    SEARCH_AOT* pZone;

    pZone = pAot->pRec;
    if (G.pEm->Pos_z - pZone->Z < (pZone->D >> 1))
    {
        if ((G.pEm->Cdir_y + 0x800) & 0x800)
        {
            goto end;
        }

        G.pEm->Timer0 = 1;
    }
    else
    {
        if (G.pEm->Cdir_y & 0x800)
        {
            goto end;
        }
        G.pEm->Timer0 = -1;
    }

    EM_CUR->Routine_1 = 0;
    pEm = MAIN.pEm;
    STOP_FLG_CUR &= ~STOP_ACTION;
    pEm->Zone_in_flg = pEm->Zone_in_flg & 0xEF;
    pEm2 = MAIN.pEm;
    if (((pEm2->Timer0 >> 1) ^ pAot->Data0) & 1)
    {
        pEm2->Zone_in_flg = pEm2->Zone_in_flg | 0x10;
    }
end:
    G.pEm->Zone_in_flg = G.pEm->Zone_in_flg | 0x60;
    STATUS_FLG_CUR |= STAT_STAIRS_ZONE;
    return 0;
}

INT Sce_Flg_chg(USHORT* pAot, UINT* pFlg)
{
    UINT v;

    switch (pAot[1])
    {
    case FLAG_SCENARIO:
        pFlg = ((UINT*)&MAIN.SaveData.Scenario_flg[0]) + (pAot[2] >> 5);  // word view of the same memory
        break;
    case FLAG_SCENARIO2:
        pFlg = ((UINT*)&MAIN.SaveData.Scenario_flg2[0]) + (pAot[2] >> 5);  // word view of the same memory
        break;
    case FLAG_KEY:
        pFlg = ((UINT*)&MAIN.SaveData.Key_flg[0]) + (pAot[2] >> 5);  // word view of the same memory
        break;
    case FLAG_ENEMY:
        pFlg = ((UINT*)&MAIN.SaveData.Enemy_flg[0]) + (pAot[2] >> 5);  // word view of the same memory
        break;
    case FLAG_ROOM_WK:
        pFlg = MAIN.Room_flg + (pAot[2] >> 5);
        break;
    case FLAG_SYSTEM:
        pFlg = &MAIN.System_flg;
        break;
    case FLAG_STOP:
        pFlg = (UINT*)&MAIN.Stop_flg;  // word view of the same memory
        break;
    case FLAG_ITEM:
        pFlg = ((UINT*)&MAIN.SaveData.Item_flg[0]) + (pAot[2] >> 5);  // word view of the same memory
        break;
    case FLAG_ROOM:
        pFlg = ((UINT*)&MAIN.SaveData.Room_flg[0]) + (pAot[2] >> 5);  // word view of the same memory
        break;
    case FLAG_USE:
        pFlg = Use_flg + (pAot[2] >> 5);
        break;
    }
    if (pAot[3] != 0)
    {
        v = (0x80000000U >> (pAot[2] & 0x1F)) | *pFlg;
    }
    else
    {
        v = ~(0x80000000U >> (pAot[2] & 0x1F)) & *pFlg;
    }

    *pFlg = v;

    return 0;
}

INT Sce_Itembox(SCE_AOT* pAot)
{
    USHORT Upd_flg;

    if (IBOX_STS_CUR == 0)
    {
        if (G.Player.Damage_no == 0 && !(G.System_flg & SYS_REQ_MASK))
        {
            Upd_flg = STOP_FLG_CUR;
            if (Upd_flg & 0x40)
            {
                MAIN.Ibox_sts = 1;

                MAIN.Stop_flg = Upd_flg & ~(STOP_PLAYER | STOP_ENEMY_MOVE | STOP_ACTION);
                Snd_se_play(SE_BANK_ROOM, 0x20, 0);
                pAot_data = pAot;
            }
        }
    }
    return 0;
}

INT Sce_Event(SCE_AOT* pAot)
{
    Event_exec((UCHAR)pAot->Data0, (UCHAR)pAot->Data1);
    return 0;
}

INT Sce_Normal(VOID)
{
    return 0;
}

INT Sce_Water(VOID)
{
    INT next;

    STATUS_FLG_CUR |= STAT_WATER;
    if (G.Player.Spd_cur > 0 && G.Cbuf_id != 0)
    {
        ScratchPad->Vec.vy = Omodel_tbl[0]->Coord.t[1] - G.Player.Pos_y;
        ScratchVec[0] = Water_x_tbl[Water_idx];
        ScratchPad->Vec.vz = Water_z_tbl[Water_idx];

        Esp_call(0x17, 8, 0, MATRIX_OF(&MAIN.Player), &ScratchPad->Vec, 0);

        next = Water_idx + 1;
        Water_idx = next;
        if (next >= 6)
        {
            Water_idx = 0;
        }
    }
    return 0;
}

INT Sce_ladder_set(SCE_AOT* pAot)
{
    UCHAR Zone_flg;

    Zone_flg = G_PLAYER->Zone_in_flg;
    G.Player.Zone_in_flg = Zone_flg | 0x20;
    if (pAot->Data0 != 0)
    {
        G.Player.Zone_in_flg = Zone_flg | 0x30;
    }
    G.Player.Root_x = pAot->Data1;
    G.Player.Root_z = pAot->Data2;
    pAot->Data0 = pAot->Data0 ^ 1;
    MAIN.System_flg |= SYS_LADDER;
    return 0;
}

INT Sce_evt_flg_set(SCE_AOT* pAot)
{
    pAot_data = pAot;
    if (pAot->Data0 == 0)
    {
        MAIN.System_flg |= SYS_REQ_FILE;
        return 0;
    }
    return 1;
}

INT Sce_Hikidashi(SCE_AOT* pAot)
{
    INT msg;
    OBJ_MODEL_WORK* pOm;
    INT Cut_ck;
    UCHAR cut;
    VCUT* zones;
    VCUT* pCcut2;

    if (DESK_STS_CUR == 0)
    {
        if ((MAIN.Player.Damage_no == 0 && (!(G.System_flg & SYS_REQ_MASK))) && (G.Stop_flg & STOP_ACTION))
        {
            if (Flg_ck(&MAIN.SaveData.Item_flg, Sce_aot_tbl[pAot->Data1].Data2) != 0)  // address passed as an integer
            {
                if ((G.Player.Id & 3) == 3)
                {
                    msg = 0xD7;
                    goto msg_set;
                }

                if (Flg_ck(&MAIN.SaveData.Key_flg, pAot->Data0) == 0)  // address passed as an integer
                {
                    if ((INT)Search_item_id(0x3D) >= 0 || Flg_ck(&MAIN.SaveData.Scenario_flg, 0x7CU) != 0)  // address passed as an integer
                    {
                        pAot_data = pAot;
                        MAIN.Desk_sts = 1;
                        goto exit_hikidashi;
                    }
                    msg = 0xD8;
                msg_set:
                    Mess_set(msg, 0xFF);

                    return 0;
                }
                pOm = pOm_tbl[Sce_aot_tbl[pAot->Data1].Data1];
                pAot_data = pAot;
                pOm->Be_flg |= 1;
                Snd_se_play(SE_BANK_ROOM, 0x24, 0);

                G.SaveData.Cut_bak = CUT_NO_CUR;
                MAIN.SaveData.Cut_no = pAot->Data2;
                MAIN.Desk_sts = 0x23;
                zones = G.pRoom->pVcut;
                cut = *(volatile UCHAR*)&MAIN.SaveData.Cut_no;  // UCHAR* view of the field (a plain access changes the code)
                Cut_ck = zones->Fcut != cut;
                G.Cccut = zones;
                if (Cut_ck)
                {
                    do
                    {
                        pCcut2 = CCCUT_CUR;
                        MAIN.Cccut = pCcut2;
                        VIEW(VCUT*, MAIN.Cccut)++;  // VCUT** view of the field (a plain access changes the code)
                    } while (pCcut2[1].Fcut != cut);
                }
                STOP_FLG_CUR &= ~(STOP_PLAYER | STOP_ENEMY_MOVE | STOP_ACTION);
                goto exit_hikidashi;
            }
            return 0;
        }

        return 0;
    }
exit_hikidashi:
    return 0;
}

INT Hikidashi_entrance(VOID)
{
    OBJ_MODEL_WORK* pOm;
    INT v;

    switch (G.Desk_sts)
    {
    case 0:
        break;
    case 1:
    case 2:
        Key_work = Search_item_id(0x3D);

        v = Flg_ck(&MAIN.SaveData.Scenario_flg, 0x7C);  // address passed as an integer
        if (v == 0)
        {
            v = 0x3D;
        }
        else
        {
            v = 0xD;
        }

        do
        {
            MAIN.SaveData.Sel_id = v;
        } while (0);

        Mess_set(0xD9, 0xFF);
        G.Desk_sts = 3;
        break;
    case 3:
        if (!(G.SaveData.Mess_flg & MESS_FLG_ACTIVE))
        {
            if (!(G.SaveData.Mess_flg & MESS_FLG_ANSWER))
            {
                Flg_on(&MAIN.SaveData.Key_flg, pAot_data->Data0);  // address passed as an integer
                Snd_se_play(SE_BANK_ROOM, 0x26, 0);

                v = Flg_ck(&MAIN.SaveData.Scenario_flg, 0x7C);  // address passed as an integer
                if (v == 0)
                {
                    v = 0x3D;
                }
                else
                {
                    v = 0xD;
                }

                G.SaveData.Sel_id = v;

                Mess_set(0xC3, 0xFF);
            }
            G.Desk_sts = 0;
        }
        break;
    case 4:
        Cut_change();
        pOm = pOm_tbl[pAot_data->Data1];
        G.Desk_sts = 0;
        pOm->Be_flg &= 0xFE;
        break;
    case 5:
        G.Desk_sts = 4;
        pAot_data = &Sce_aot_tbl[pAot_data->Data1];
        MAIN.System_flg |= SYS_REQ_ITEM_GET;
        G.SaveData.Cut_no = G.SaveData.Cut_bak;
        STOP_FLG_CUR |= STOP_PLAYER | STOP_ENEMY_MOVE | STOP_ACTION;
        break;

    case 35:
        Cut_change();

    default:
        DESK_STS_CUR--;
        break;
    }
}

INT Sce_pickup(SCE_AOT* pAot)
{
    pAot->Id = SCE_NOTHING;
    pOm_tbl[pAot->Data1]->Be_flg = 0;
    Flg_off(&MAIN.SaveData.Item_flg, pAot->pRec->Item_flg_no);  // address passed as an integer

    Map_area_set(pAot->pRec->Item_id - 0x4E);

    G.SaveData.Get_id = pAot->pRec->Item_id;
    return 0;
}

INT Sce_Save(SCE_AOT* pAot)
{
    INT Key_work;

    if (TYPEWRITER_STS_CUR == 0)
    {
        if (!(G.System_flg & SYS_REQ_MASK) && (STOP_FLG_CUR & STOP_ACTION))
        {
            Key_work = Search_item_id(0x2F);
            if (Key_work >= 0)
            {
                pAot_data = pAot;
                pAot->Data0 = Key_work;

                MAIN.Typewriter_sts = 1;

                STOP_FLG_CUR &= ~(STOP_PLAYER | STOP_ENEMY_MOVE | STOP_ACTION);
            }
            else
            {
                Mess_set(0xDE, 0xFF);
                MAIN.Typewriter_sts = 0;
            }
        }
    }
    return 0;
}

INT Sce_slope_set(SCE_AOT* pAot, INT Dist_in)
{
    INT dist;
    USHORT dir;
    SEARCH_AOT* pZone;
    SEARCH_AOT* pZone2;

    dist = Dist_in;
    dir = pAot->Data0;
    switch (dir)
    {
    case 0:
        dist = G.Player.Pos_x - pAot->pRec->X;
        break;
    case 1:
        pZone = pAot->pRec;
        dist = pZone->X + pZone->W - G.Player.Pos_x;
        break;
    case 2:
        dist = G.Player.Pos_z - pAot->pRec->Z;
        break;
    case 3:
        pZone2 = pAot->pRec;
        dist = pZone2->Z + pZone2->D - G.Player.Pos_z;
        break;
    }
    G.Player.Pos_y = ((SHORT)(dist / pAot->Data1) + 1) * (SHORT)pAot->Data2;  // sign-extends the halfword
    G.Player.Ground = ((SHORT)(dist / pAot->Data1) + 1) * (SHORT)pAot->Data2;  // sign-extends the halfword
    return 0;
}

INT Sce_at_check(ENEMY_WORK* pEm, INT Att)
{
    INT pRet;
    INT (**tbl)();
    SCHAR n;
    SCE_AOT* pAot;
    UINT t;
    SHORT* s;

    s = &ScratchPad->Sv.vx;

    *s = 0x258;
    ScratchPad->Sv.vz = 0;
    Rot_vector(G.Player.Cdir_y, &ScratchPad->Sv, &ScratchPad->Sv);
    ScratchPad->Vec.vx = *s + G.Player.Pos_x;
    ScratchPad->Vec.vz = ScratchPad->Sv.vz + G.Player.Pos_z;
    pAot = Sce_aot_tbl;

    G.pEm->Zone_in_flg &= 0xDF;

    pRet = 0;

    if (pAot_last >= pAot)
    {
        tbl = Sce_at_jump_tbl;
        n = 1;
        do
        {
            if (pAot->Id != SCE_NOTHING)
            {
                t = pAot->Type;
                if ((t & Att) && (t >> 7) != 1)
                {
                    if (pAot->Type & SCE_UNDER)
                    {
                        if (Hani_ck((VECTOR*)&pEm->Pos_x, pAot->pRec) & 0xFFFF)  // reinterprets the memory as VECTOR
                        {
                            G.SaveData.Em_aot_no = n;
                            pRet = tbl[pAot->Id](pAot);
                        }
                    }
                    else if (Hani_ck(&ScratchPad->Vec, pAot->pRec) & 0xFFFF)
                    {
                        G.SaveData.Front_aot_no = n;
                        pRet = tbl[pAot->Id](pAot);
                    }
                }
            }
            pAot++;
            n++;
        } while (pAot_last >= pAot);
    }
    return pRet;
}

INT Check_action_object(VOID)
{
    INT (**tbl)();
    SCHAR n;
    SCE_AOT* pAot;
    SHORT* s;

    s = &ScratchPad->Sv.vx;
    *s = 0x258;
    ScratchPad->Sv.vz = 0;
    Rot_vector(G.Player.Cdir_y, &ScratchPad->Sv, &ScratchPad->Sv);
    ScratchPad->Vec.vx = *s + G.Player.Pos_x;
    ScratchPad->Vec.vz = ScratchPad->Sv.vz + G.Player.Pos_z;
    pAot = Sce_aot_tbl;
    if (pAot_last >= pAot)
    {
        tbl = Sce_at_jump_tbl;
        n = 1;
        do
        {
            if (pAot->Id != SCE_NOTHING)
            {
                if ((pAot->Type & SCE_PLAYER) && (pAot->Type >> 7) != 0)
                {
                    if (pAot->Type & SCE_UNDER)
                    {
                        if (Hani_ck((VECTOR*)&MAIN.Player.Pos_x, pAot->pRec) & 0xFFFF)  // reinterprets the memory as VECTOR
                        {
                            G.SaveData.Em_aot_no = n;
                            return tbl[pAot->Id](pAot);
                        }
                    }
                    else if (Hani_ck(&ScratchPad->Vec, pAot->pRec) & 0xFFFF)
                    {
                        G.SaveData.Front_aot_no = n;
                        return tbl[pAot->Id](pAot);
                    }
                }
            }
            pAot++;
            n++;
        } while (pAot_last >= pAot);
    }
    return 0;
}

VOID Item_box_entrance(VOID)
{
    switch (G.Ibox_sts)
    {
    case 0:
        break;
    case 1:
        Dir_spd = 1;
        Spd_add = 1;
        IBOX_STS_CUR = 2;
        pOwork = Omodel_tbl[pAot_data->Data1];

    case 2:
        pOwork->Cdir_z -= Dir_spd;
        Dir_spd += Spd_add;
        if (pOwork->Cdir_z < -0xC7)  // sign-extends the halfword
        {
            G.Ibox_sts = 3;
            Spd_add = (SHORT)Spd_add * -1;  // sign-extends the halfword
        }
        break;
    case 3:
        pOwork->Cdir_z -= Dir_spd;
        Dir_spd += Spd_add;
        if ((SHORT)Dir_spd <= 0)  // sign-extends the halfword
        {
            G.Stop_flg = STOP_ALL;
            G.Ibox_sts = 4;
            MAIN.System_flg |= SYS_REQ_ITEMBOX;
        }
        break;
    case 4:
        G.Ibox_sts = 0;
        pOwork->Cdir_z = 0;
        break;
    }
}

INT Save_entrance(VOID)
{
    switch (G.Typewriter_sts)
    {
    case 0:
        break;
    case 1:
        Ink_no = pAot_data->Data0;

        Mess_set(0xDF, 0xFF);

        G.Typewriter_sts = 2;
        break;
    case 2:
        if (G.SaveData.Mess_flg & MESS_FLG_ACTIVE)
        {
            break;
        }
        if (!(G.SaveData.Mess_flg & MESS_FLG_ANSWER))
        {
            G.Fade_hrate = 2;
            G.Fade_add = 0x1000;
            Evt_fade_start();
            G.Typewriter_sts = 3;
        }
        else
        {
            G.Typewriter_sts = 0;
            STOP_FLG_CUR |= STOP_PLAYER | STOP_ENEMY_MOVE | STOP_ACTION;
        }
        break;
    case 3:
        if (G.SaveData.Fade_kido < 0)
        {
            GAME_PLAY_TIME_CUR += VSync(-1) - Play_vsync;
            Play_vsync = VSync(-1);
            Card_access(0, G.pFree_work, Ink_no + 1, 2);

            G.Fade_hrate = 2;
            G.Fade_add = -0x1000;
            Evt_fade_start();
            G.Typewriter_sts = 4;
        }
        break;
    case 4:
        if (G.SaveData.Fade_kido < 0)
        {
            G.Typewriter_sts = 0;
            STOP_FLG_CUR |= STOP_PLAYER | STOP_ENEMY_MOVE | STOP_ACTION;
        }
        break;
    }
}

UINT Key_lost(VOID)
{
    UCHAR Used_flg;

    Used_flg = EVT_USE_CUR;
    if ((Used_flg != 0 && Used_flg == 1) && (!(G.SaveData.Mess_flg & MESS_FLG_ACTIVE)))
    {
        Mess_item_use();
        MAIN.Evt_use = 0;
    }
}

INT Water_idx = 0;  // current index into the water effect offset tables

// water effect X offsets
INT Water_x_tbl[6] =
{
    0,      // 0 //
    -346,   // 1 //
    -346,   // 2 //
    0,      // 3 //
    346,    // 4 //
    346,    // 5 //
};

// water effect Z offsets
INT Water_z_tbl[6] =
{
    -400,   // 0 //
    346,    // 1 //
    -346,   // 2 //
    346,    // 3 //
    200,    // 4 //
    346,    // 5 //
};

// room action handler table, indexed by entry type
SCE_AT_FUNC Sce_at_jump_tbl[18] =
{
    0,                                      // 0 //
    Sce_Door,                    // 1 //
    Sce_Message,                 // 2 //
    Sce_lock_ck,                 // 3 //
    Sce_Item,                    // 4 //
    Sce_stairs_x,   // 5 //
    Sce_stairs_z,   // 6 //
    Sce_Flg_chg,                 // 7 //
    Sce_Itembox,                 // 8 //
    Sce_Event,                   // 9 //
    Sce_Normal,                  // 10 //
    Sce_Water,                   // 11 //
    Sce_ladder_set,             // 12 //
    Sce_evt_flg_set,         // 13 //
    Sce_Hikidashi,               // 14 //
    Sce_pickup,            // 15 //
    Sce_Save,                    // 16 //
    Sce_slope_set,         // 17 //
};
