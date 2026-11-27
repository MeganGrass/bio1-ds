/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SCE0.C
 */

#include "TYPES.H"
#include <LIBSPU.H>
#include "SCE0.H"
#include "SCA.H"
#include "CD.H"
#include "CINESCO.H"
#include "CUT.H"
#include "EFF1.H"
#include "ENEMY.H"
#include "EVENT.H"
#include "GAME.H"
#include "INVENTORY.H"
#include "JOINT.H"
#include "MAIN.H"
#include "PAD.H"
#include "PLAYER.H"
#include "PSP.H"
#include "SCE1.H"
#include "SCE_AT.H"
#include "SND.H"
#include "ST_SUB02.H"
#include "ST_SUB03.H"
#include "SUB.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "SCENARIO.H"

// functions without a C definition
INT Mess_set(UINT, USHORT);

LONG Bgm_status_mask;  // BGM state mask: bit (3 + sequence) per playing sequence, shifted up by 8 while stopped

USHORT Sprite_hide_mask;  // mask of room sprite groups to hide

OBJ_MODEL_WORK* pOm_tbl[16];  // item model slots of the current room
OBJ_MODEL_WORK* Omodel_tbl[16];  // object model slot buffer of the room
MODEL_LINK_INFO* pItem_model_last;  // model link record of the last item model set up
MODEL_LINK_INFO* pOmodel_last;  // model link record of the last object model set up
UCHAR Omodel_num;  // number of object models set up in the room
SCE_AOT* pAot_last;  // last used entry of the room action table

SndVolume Seqvol[3];  // left/right volume of each SEP sequence set by the script

UCHAR Imodel_num;  // number of item models set up in the room
SCE_AOT Sce_aot_tbl[24];  // room action table of the current room

VOID Sce_aot_init(VOID)
{
    UINT i;

    i = 0;
    do
    {
        Sce_aot_tbl[i].Id = SCE_NOTHING;
        i += 1;
    } while (i < 0x18U);
    pAot_last = Sce_aot_tbl;
}

INT Evt_end(VOID)
{
    G.Scenario.Ifel_ctr = 0;
    return 0;
}

INT Ifel_ck(VOID)
{
    UCHAR* pScd;
    UCHAR** sp;
    USHORT op;

    pScd = SCE_DATA_CUR;
    sp = SCE_SP_CUR;
    op = *(USHORT*)pScd;  // halfword view of the same memory
    pScd += 2;
    MAIN.Scenario.pSce_data = pScd;
    MAIN.Scenario.ppSce_sp = sp + 1;

    *sp = (UCHAR*)((op >> 8) + (INT)pScd);  // real width or sign conversion

    IFEL_CTR_CUR++;
    return 1;
}

INT Else_ck(VOID)
{
    UCHAR* pScd;

    SCE_SP_WORD -= 4;
    IFEL_CTR_CUR--;
    pScd = SCE_DATA_CUR;

    MAIN.Scenario.pSce_data = (UCHAR*)(pScd[1] + (INT)pScd);  // byte view of the same memory

    return 1;
}

INT Endif(VOID)
{
    SCE_SP_WORD -= 4;
    IFEL_CTR_CUR--;
    SCE_DATA_CUR += 2;
    return 1;
}

INT Ck(VOID)
{
    USHORT* pScd;
    INT* pFlg;
    UINT x;
    USHORT* q;

    pScd = SCE_DATA(USHORT);
    x = *pScd;
    SCE_DATA(USHORT) = pScd + 1;
    switch (x >> 8)
    {
    case FLAG_SCENARIO:
        pFlg = (INT*)&MAIN.SaveData.Scenario_flg[0];  // word view of the same memory
        break;
    case FLAG_SCENARIO2:
        pFlg = (INT*)&MAIN.SaveData.Scenario_flg2[0];  // word view of the same memory
        break;
    case FLAG_KEY:
        pFlg = (INT*)&MAIN.SaveData.Key_flg[0];  // word view of the same memory
        break;
    case FLAG_ENEMY:
        pFlg = (INT*)&MAIN.SaveData.Enemy_flg[0];  // word view of the same memory
        break;
    case FLAG_ROOM_WK:
        pFlg = &MAIN.Room_flg[0];
        break;
    case FLAG_SYSTEM:
        pFlg = &MAIN.System_flg;
        break;
    case FLAG_STOP:
        pFlg = (INT*)&MAIN.Stop_flg;  // word view of the same memory
        break;
    case FLAG_ITEM:
        pFlg = (INT*)&MAIN.SaveData.Item_flg[0];  // word view of the same memory
        break;
    case FLAG_ROOM:
        pFlg = (INT*)&MAIN.SaveData.Room_flg[0];  // word view of the same memory
        break;
    case FLAG_USE:
        pFlg = Use_flg;
        break;

    case 10:
        SCE_DATA_CUR += 2;
        return G.Player.Damage_no >> 7;
    }
    q = SCE_DATA(USHORT);
    x = *q;
    SCE_DATA(USHORT) = q + 1;

    return (((UINT)(pFlg[(x >> 5) & 7] << (x & 0x1F))) >> 31) ^ (x >> 8);  // unsigned conversion (signed operand)
}

static const UINT Sce_pad1 = 0;  // padding: keeps the following read-only data at its linked position

INT Set(VOID)
{
    USHORT* pScd;
    UINT* pFlg;
    UINT x;
    INT bit;

    pScd = SCE_DATA(USHORT);
    SCE_DATA(USHORT) = pScd + 2;
    switch (pScd[0] >> 8)
    {
    case FLAG_SCENARIO:
        pFlg = (UINT*)&MAIN.SaveData.Scenario_flg[0];  // word view of the same memory
        break;
    case FLAG_SCENARIO2:
        pFlg = (UINT*)&MAIN.SaveData.Scenario_flg2[0];  // word view of the same memory
        break;
    case FLAG_KEY:
        pFlg = (UINT*)&MAIN.SaveData.Key_flg[0];  // word view of the same memory
        break;
    case FLAG_ENEMY:
        pFlg = (UINT*)&MAIN.SaveData.Enemy_flg[0];  // word view of the same memory
        break;
    case FLAG_ROOM_WK:
        pFlg = MAIN.Room_flg;
        break;
    case FLAG_SYSTEM:
        pFlg = &MAIN.System_flg;
        break;
    case FLAG_STOP:
        pFlg = (UINT*)&MAIN.Stop_flg;  // word view of the same memory
        break;
    case FLAG_ITEM:
        pFlg = (UINT*)&MAIN.SaveData.Item_flg[0];  // word view of the same memory
        break;
    case FLAG_ROOM:
        pFlg = (UINT*)&MAIN.SaveData.Room_flg[0];  // word view of the same memory
        break;
    case FLAG_USE:
        pFlg = Use_flg;
        break;
    }
    x = pScd[1];
    pFlg += (x & 0xE0) >> 5;
    bit = x & 0x1F;
    switch (x >> 8)
    {
    case SET_SET:
        *pFlg |= 0x80000000 >> bit;
        return 1;
    case SET_CLEAR:
        *pFlg &= ~(0x80000000 >> bit);
        return 1;
    case SET_TOGGLE:
        *pFlg ^= 0x80000000 >> bit;
        return 1;
    }
}

INT Ck_byte(VOID)
{
    USHORT* pScd;
    UINT cur;
    UINT val;
    UCHAR op;

    pScd = SCE_DATA(USHORT);
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 2);
    cur = (&MAIN.SaveData.Stage_no)[*pScd++ >> 8];
    op = *pScd;
    val = *pScd >> 8;
    switch (op)
    {
    case CMP_EQ:
        return cur == val;
    case CMP_GT:
        return val < cur;
    case CMP_GE:
        return cur >= val;
    case CMP_LT:
        return cur < val;
    case CMP_LE:
        return val >= cur;
    case CMP_NE:
        return cur != val;
    }
}

static UCHAR Imodel_tpage;  // texture page cursor taken before the last item model texture was mapped
static UCHAR Imodel_clut;  // CLUT row cursor taken before the last item model texture was mapped
static UCHAR Sce_bss_pad[0x4];  // padding: keeps the following data at its linked position
static UCHAR Omodel_tpage;  // texture page cursor taken before the last object model texture was mapped
static UCHAR Omodel_clut;  // CLUT row cursor taken before the last object model texture was mapped

INT Cmp(VOID)
{
    INT unused[2];
    SCE_CMP_REC* pScd;
    INT cur;
    INT val;
    UCHAR op;

    pScd = SCE_DATA(SCE_CMP_REC);
    SCE_DATA_NEXT(pScd, 6);
    op = pScd->Operator;
    cur = ((USHORT*)&MAIN.SaveData.Fade_kido)[pScd->Flag];  // halfword view of the same memory
    val = pScd->Cmp_val;
    switch (op)
    {
    case CMP_EQ:
        return cur == val;
    case CMP_GT:
        return val < cur;
    case CMP_GE:
        return cur >= val;
    case CMP_LT:
        return cur < val;
    case CMP_LE:
        return val >= cur;
    case CMP_NE:
        return cur != val;
    }
}

INT Set_byte(VOID)
{
    USHORT* pScd;
    UINT i;

    pScd = SCE_DATA(USHORT);
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 2);
    i = *pScd++ >> 8;
    (&MAIN.SaveData.Stage_no)[i] = *pScd;
    return 1;
}

INT Save(VOID)
{
    SCE_SAVE_REC* pScd;

    pScd = SCE_DATA(SCE_SAVE_REC);
    SCE_DATA_NEXT(pScd, 4);
    (&MAIN.SaveData.Fade_kido)[pScd->head >> 8] = pScd->Set_val;
    return 1;
}

INT Nop(VOID)
{
    SCE_DATA_CUR += 2;
    return 1;
}

INT Cut_chg(VOID)
{
    INT cut;

    VOID** zones_addr;

    VCUT* zones;
    VCUT* pCcut2;
    SCE_CUT_SET_REC* pScd;

    G.SaveData.Cut_bak = CUT_NO_CUR;
    pScd = SCE_DATA(SCE_CUT_SET_REC);
    SCE_DATA_NEXT(pScd, 1);

    MAIN.SaveData.Cut_no = pScd->Cut_no;

    zones = G.pRoom->pVcut;

    cut = MAIN.SaveData.Cut_no;
    CCCUT_CUR = zones;
    zones_addr = (VOID**)VCUT_SENTINEL;  // untyped pointer, the callee takes the address as is

    if (zones->Fcut != cut)
    {
        zones_addr = &MAIN.Cccut;

        do
        {
            pCcut2 = CCCUT_CUR;
            MAIN.Cccut = pCcut2 + 1;
        } while (pCcut2[1].Fcut != cut);
    }
    Cut_set();
    MAIN.System_flg |= SYS_CAM_LOCK;
    SCE_DATA_WORD = (INT)MAIN.Scenario.pSce_data + 1;  // INT* view of the field (a plain access changes the code)

    return 1;
}

INT Cut_old(VOID)
{
    INT cut;

    VOID** zones_addr;

    VCUT* zones;
    VCUT* pCcut2;

    CUT_NO_CUR = G.SaveData.Cut_bak;

    zones = G.pRoom->pVcut;

    cut = MAIN.SaveData.Cut_no;
    CCCUT_CUR = zones;
    zones_addr = (VOID**)VCUT_SENTINEL;  // untyped pointer, the callee takes the address as is

    if (zones->Fcut != cut)
    {
        zones_addr = &G.Cccut;

        do
        {
            pCcut2 = CCCUT_CUR;
            MAIN.Cccut = pCcut2 + 1;
        } while (pCcut2[1].Fcut != cut);
    }
    Cut_set();
    MAIN.System_flg &= ~SYS_CAM_LOCK;
    SCE_DATA_CUR += 2;

    return 1;
}

INT Message_on(VOID)
{
    USHORT* pScd;
    UCHAR** ppSce_data;
    USHORT head;

    pScd = SCE_DATA(USHORT);
    head = *pScd;
    *(ppSce_data = &MAIN.Scenario.pSce_data) = (UCHAR*)(pScd + 1);
    Mess_set(head >> 8, pScd[1]);
    *ppSce_data += 2;
    return 1;
}

INT Sce_bgmtbl_set(VOID)
{
    USHORT* pScd;
    USHORT a;
    UINT b;

    pScd = SCE_DATA(USHORT);
    a = pScd[0];
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 1);
    b = pScd[1];
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 2);
    ((UCHAR(*)[0x20])&MAIN.SaveData.Bgm_tbl)[a >> 8][b & 0xFF] = b >> 8;
    return 1;
}

INT Nop_2E(VOID)
{
}

INT Sce_bgm_play(VOID)
{
    USHORT* pScd;
    UINT seq;

    pScd = SCE_DATA(USHORT);

    seq = *pScd >> 8;

    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 1);

    SsSepPlay(G.Scenario.Sep_no, seq, 1, 1);
    Bgm_status_mask |= 1 << (seq + 3);

    return 1;
}

INT Sce_bgm_restore(VOID)
{
    UINT Bgm_sts;

    SCE_DATA_CUR += 2;
    if (G.Scenario.Bgm_tgt_sts != 0xFF)
    {
        Bgm_sts = (UINT)Bgm_status_mask >> 8;  // unsigned conversion (signed operand)

        Bgm_status_mask = Bgm_sts;
        if (Bgm_sts & 8)
        {
            SsSepSetVol(SEP_NO_CUR, 0, Seqvol[0].left, Seqvol[0].right);
            SsSepPlay(MAIN.Scenario.Sep_no, 0, 1, 1);
        }
        if (Bgm_status_mask & 0x10)
        {
            SsSepSetVol(SEP_NO_CUR, 1, Seqvol[1].left, Seqvol[1].right);
            SsSepPlay(MAIN.Scenario.Sep_no, 1, 1, 1);
        }
        if (Bgm_status_mask & 0x20)
        {
            SsSepSetVol(SEP_NO_CUR, 2, Seqvol[2].left, Seqvol[2].right);
            SsSepPlay(MAIN.Scenario.Sep_no, 2, 1, 1);
        }
    }
    return 1;
}

INT Sce_bgm_stop_all(VOID)
{
    SCE_DATA_CUR += 2;
    if (G.Scenario.Bgm_tgt_sts != 0xFF)
    {
        if ((SsIsEos(SEP_NO_CUR, 0) << 0x10) != 0)
        {
            SsSepSetVol(MAIN.Scenario.Sep_no, 0, 0, 0);
        }

        if ((SsIsEos(MAIN.Scenario.Sep_no, 1) << 0x10) != 0)
        {
            SsSepSetVol(MAIN.Scenario.Sep_no, 1, 0, 0);
        }

        if ((SsIsEos(MAIN.Scenario.Sep_no, 2) << 0x10) != 0)
        {
            SsSepSetVol(MAIN.Scenario.Sep_no, 2, 0, 0);
        }

        if ((SsIsEos(MAIN.Scenario.Sep_no, 0) << 0x10) != 0)
        {
            SsSepStop(MAIN.Scenario.Sep_no, 0);
        }
        if ((SsIsEos(MAIN.Scenario.Sep_no, 1) << 0x10) != 0)
        {
            SsSepStop(MAIN.Scenario.Sep_no, 1);
        }
        if ((SsIsEos(MAIN.Scenario.Sep_no, 2) << 0x10) != 0)
        {
            SsSepStop(MAIN.Scenario.Sep_no, 2);
        }
        SsUtAllKeyOff(0);

        Bgm_status_mask <<= 8;
    }
    return 1;
}

INT Sce_snd_vol_set(VOID)
{
    USHORT* pScd;
    USHORT a;
    USHORT b;

    INT i;

    pScd = SCE_DATA(USHORT);
    a = pScd[0];
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 1);
    b = pScd[1];
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 2);

    if (G.Scenario.Sep_no >= 0)
    {
        SsSepSetVol(G.Scenario.Sep_no, a >> 8, b & 0xFF, b >> 8);
    }
    i = a >> 8;
    Seqvol[i].left = b & 0xFF;
    Seqvol[i].right = b >> 8;

    return 1;
}

INT Sce_bgm_stop(VOID)
{
    INT bit;
    USHORT* pScd;
    UINT seq;

    pScd = SCE_DATA(USHORT);

    seq = *pScd >> 8;

    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 1);
    bit = 1 << (seq + 3);

    if (Bgm_status_mask & bit)
    {
        SsSepSetVol(SEP_NO_CUR, seq, 0, 0);
        SsSepStop(MAIN.Scenario.Sep_no, seq);
        SpuSetKey(0, Snd_keyon_mask == 0);

        Bgm_status_mask &= ~bit;

        SsSepSetVol(MAIN.Scenario.Sep_no, seq, 0x5F, 0x5F);
    }
    return 1;
}

INT Nop_26(VOID)
{
}

INT Sce_snd_fade_set(VOID)
{
    Snd_bgm_fade_ON(* SCE_DATA(USHORT) >> 8, 0x7F);
    MAIN.Scenario.pSce_data += 2;
    return 1;
}

INT Sce_bgm_vol_ramp(VOID)
{
    USHORT Ramp;
    UINT seq;
    SCE_BGM_RAMP_REC* pScd;

    pScd = SCE_DATA(SCE_BGM_RAMP_REC);
    seq = pScd->head >> 8;
    SCE_DATA_NEXT(pScd, 2);
    Ramp = pScd->Ramp;
    SCE_DATA_NEXT(pScd, 4);
    if (Bgm_status_mask & (1 << (seq + 3)))
    {
        SsSepSetCrescendo(G.Scenario.Sep_no, seq, (SCHAR)Ramp, Ramp >> 8);  // sign-extends the byte
    }

    return 1;
}

INT Se_on(VOID)
{
    INT unused[4];
    INT type;
    USHORT w0;
    USHORT w1;
    USHORT w2;
    SCE_SE_ON_REC* pScd;

    pScd = SCE_DATA(SCE_SE_ON_REC);
    w0 = pScd->head;
    SCE_DATA_NEXT(pScd, 2);
    w1 = pScd->sound;
    SCE_DATA_NEXT(pScd, 4);
    w2 = pScd->where;
    SCE_DATA_NEXT(pScd, 6);
    type = w2 & 0xFF;
    switch (type)
    {
    case 0:
        ScratchPad->Vec.vx = pScd->vx;  // sign-extends the halfword
        SCE_DATA_NEXT(pScd, 8);
        ScratchPad->Vec.vy = 0;
        ScratchPad->Vec.vz = pScd->z;  // sign-extends the halfword
        SCE_DATA_NEXT(pScd, 0xA);
        Snd_se_stad(w0 >> 8, w1 & 0xFF, (w1 << 0x10) >> 0x18, &ScratchPad->Vec);
        break;
    case 1:
        SCE_DATA_NEXT(pScd, 0xA);
        Snd_se_stad(w0 >> 8, w1 & 0xFF, (w1 << 0x10) >> 0x18, &MAIN.Player.Pos_x);
        break;
    case 2:
        SCE_DATA_NEXT(pScd, 0xA);
        Snd_se_stad(w0 >> 8, w1 & 0xFF, (w1 << 0x10) >> 0x18, &MAIN.Enemy[w2 >> 8].Pos_x);
        break;
    case 3:
        SCE_DATA_NEXT(pScd, 0xA);
        Snd_se_play(w0 >> 8, w0 >> 8, (w1 << 0x10) >> 0x18);
        break;
    }
    return 1;
}

INT Equip_Item_ck(VOID)
{
    SCE_ITEM_OP_REC* pScd;

    pScd = SCE_DATA(SCE_ITEM_OP_REC);
    SCE_DATA_NEXT(pScd, 2);
    return (&pItem_work_base[G.SaveData.Equip_no])[-1].Id == pScd->Item_no;
}

INT Sce_picked_item_ck(VOID)
{
    SCE_ITEM_OP_REC* pScd;

    pScd = SCE_DATA(SCE_ITEM_OP_REC);
    SCE_DATA_NEXT(pScd, 2);
    return G.SaveData.Get_id == pScd->Item_no;
}

INT Sce_used_item_ck(VOID)
{
    SCE_ITEM_OP_REC* pScd;

    pScd = SCE_DATA(SCE_ITEM_OP_REC);
    SCE_DATA_NEXT(pScd, 2);
    return G.SaveData.Use_id == pScd->Item_no;
}

INT Obj_model_flg_set(VOID)
{
    SCE_OBJ_FLG_REC* pScd;

    pScd = SCE_DATA(SCE_OBJ_FLG_REC);
    SCE_DATA_NEXT(pScd, 4);
    pOm_tbl[pScd->index]->Be_flg = pScd->value;
    return 1;
}

INT Keep_Item_ck(VOID)
{
    INT unused[1];
    UCHAR* pScd;
    UINT i;

    i = 0;
    pScd = SCE_DATA_CUR;
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 2);
    for (; i < G.SaveData.Item_num; i++)
    {
        if (pScd[1] == pItem_work_base[i].Id)
        {
            return 1;
        }
    }
    return 0;
}

INT Aot_reset(VOID)
{
    SCE_AOT_RESET_REC* pScd;
    SCE_AOT* pAot;

    pScd = SCE_DATA(SCE_AOT_RESET_REC);
    SCE_DATA_NEXT(pScd, 0xA);
    pAot = &Sce_aot_tbl[pScd->Aot];
    pAot->Id = pScd->Id;
    pAot->Type = pScd->Type;
    pAot->Data0 = pScd->Data0;
    pAot->Data1 = pScd->Data1;
    pAot->Data2 = pScd->Data2;
    return 1;
}

INT Aot_arm(VOID)
{
    SCE_AOT_RESET_REC* pScd;
    SCE_AOT* pAot;

    pScd = SCE_DATA(SCE_AOT_RESET_REC);
    SCE_DATA_NEXT(pScd, 4);
    pAot = &Sce_aot_tbl[pScd->Aot];
    pAot->Id = pScd->Id;
    pAot->Type = pScd->Type;
    return 1;
}

INT Evt_exec(VOID)
{
    USHORT ev;
    SCE_EVT_EXEC_REC* pScd;

    pScd = SCE_DATA(SCE_EVT_EXEC_REC);
    SCE_DATA_NEXT(pScd, 2);
    ev = pScd->event;
    Event_exec(ev & 0xFF, ev >> 8);
    MAIN.Scenario.pSce_data += 2;
    return 1;
}

INT Evt_kill(VOID)
{
    MAIN.Scenario.Sce_task[* SCE_DATA(USHORT) >> 8].Status = 0;
    MAIN.Scenario.pSce_data += 2;
    return 1;
}

INT Mirror_set(VOID)
{
    SCE_MIRROR_REC* pScd;
    USHORT a;
    USHORT b;
    USHORT c;
    INT save;

    MAIN.System_flg &= ~(SYS_MIRROR_MATRIX | SYS_MIRROR_SIDE);
    pScd = SCE_DATA(SCE_MIRROR_REC);
    MAIN.System_flg |= pScd->Mirror_flg;
    a = pScd->Mirror_min;
    b = pScd->Mirror_max;
    c = pScd->Mirror_pos;
    save = MAIN_PEM_INT;
    MAIN_PEM(PLAYER_WORK) = &MAIN.Player;  // pEm is switched to the player work
    Mirror_min = a;
    Mirror_max = b;
    Mirror_pos = c;
    Mirror_model_cp();
    Init_mirror_weapon();
    Mirror_weapon_cp(0xE);
    MAIN_PEM_INT = save;
    MAIN.Scenario.pSce_data += 8;
    return 1;
}

INT Aot_set(VOID)
{
    SCE_AOT* pAot;
    SCE_AOT_SET_REC* pScd;
    SCE_AOT_SET_REC* q;

    pScd = SCE_DATA(SCE_AOT_SET_REC);
    pAot = &Sce_aot_tbl[pScd->Aot];
    if (pAot_last < pAot)
    {
        pAot_last = pAot;
    }
    pAot->Id = pScd->Id;
    pAot->Type = SCE_DATA(SCE_AOT_SET_REC)->Type;
    q = SCE_DATA(SCE_AOT_SET_REC);
    pAot->Data0 = q->Data0;
    pAot->Data1 = q->Data1;
    pAot->Data2 = q->Data2;
    pAot->pRec = BYTE_OFS(SEARCH_AOT, q, 2);
    SCE_DATA_NEXT(q, 0x12);
    return 1;
}

INT Door_aot_set(VOID)
{
    UCHAR** ppSce_data;
    INT slot;
    SCE_AOT* pAot;
    UCHAR* pScd;

    slot = SCE_DATA_CUR[1];
    pAot = &Sce_aot_tbl[slot];
    if (pAot_last < pAot)
    {
        pAot_last = pAot;
    }
    pAot->Id = SCE_DOOR;
    ppSce_data = &MAIN.Scenario.pSce_data;
    pAot->Type = (*ppSce_data)[0x19];
    pAot->Data0 = slot;
    pScd = MAIN.Scenario.pSce_data;
    pAot->pRec = (SEARCH_AOT*)(pScd + 2);  // reinterprets the memory as SEARCH_AOT
    MAIN.Scenario.pSce_data = pScd + 0x1A;
    return 1;
}

INT Item_aot_set(VOID)
{
    SCE_AOT* pAot;
    SCE_ITEM_AOT_REC* pScd;
    SCE_ITEM_AOT_REC* pScd2;
    UCHAR* pScd3;
    SCE_ITEM_AOT_REC* pScd4;
    SCE_ITEM_AOT_REC* pScd5;
    SCE_ITEM_AOT_REC* pScd6;
    MODEL_LINK_INFO* pRec;
    OBJ_MODEL_WORK* pOm;
    UCHAR* coord;
    UCHAR kind;
    USHORT flags;
    UINT f;
    INT Mot_no;

    kind = G.Scenario.pSce_data[0xA];

    pAot = &Sce_aot_tbl[G.Scenario.pSce_data[1] & 0x7F];

    if (kind >= 0x54)
    {
        pAot->Id = Flg_ck(&MAIN.SaveData.Item_flg, G.Scenario.pSce_data[0x16]) ? 0xD : SCE_NOTHING;  // address passed as an integer
    }
    else if (kind >= 0x4E)
    {
        pAot->Id = Flg_ck(&MAIN.SaveData.Item_flg, G.Scenario.pSce_data[0x16]) ? 0xF : SCE_NOTHING;  // address passed as an integer
    }
    else
    {
        pAot->Id = Flg_ck(&MAIN.SaveData.Item_flg, G.Scenario.pSce_data[0x16]) ? SCE_ITEM : SCE_NOTHING;  // address passed as an integer
    }
    pScd = SCE_DATA(SCE_ITEM_AOT_REC);
    flags = pScd->flags;
    pScd->flags = flags & 1;
    pAot->Type = pScd->Type;
    pScd2 = SCE_DATA(SCE_ITEM_AOT_REC);
    pAot->Data0 = pScd2->flags;
    pAot->Data1 = pScd2->Om_no;
    pAot->Data2 = pScd2->Item_flg;
    pAot->pRec = BYTE_OFS(SEARCH_AOT, pScd2, 2);
    f = flags & 0xFFFE;
    if (pAot_last < pAot)
    {
        pAot_last = pAot;
    }
    pRec = &G.pRoom->pImodel[pScd2->Om_no];
    pOm = pOm_tbl[pScd2->Om_no];
    if (pRec->Tmd_adr == 0)
    {
        pOm->pTmd = 0;
        coord = (UCHAR*)&pOm->Coord;  // byte view of the same memory
    }
    else
    {
        if ((Imodel_num == 0 || pItem_model_last->Tim_adr != pRec->Tim_adr) && pRec->Tim_adr != 0)
        {
            Imodel_tpage = G.Vram_cur.Tpage_clut;
            Imodel_clut = G.Vram_cur.b.Cclut;
            Mapping_tim(pRec->Tim_adr);
        }
        if (Imodel_num == 0 || pItem_model_last->Tmd_adr != pRec->Tmd_adr)
        {
            Mapping_tmd(2, pRec->Tmd_adr, Imodel_tpage, Imodel_clut);
        }
        Tmd_obj_link(pRec->Tmd_adr, (GsDOBJ5*)&pOm->Attribute, &pOm->Coord_flg);
        pScd3 = G.Scenario.pSce_data;
        if (pScd3[0xD] == 0xFF)
        {
            pOm->pSuper = 0;
            coord = (UCHAR*)&pOm->Coord;  // byte view of the same memory
        }
        else if (pScd3[0xD] == 0xFE)
        {
            pOm->pSuper = (VOID*)&MAIN.Player.Coord_flg;  // untyped pointer, the callee takes the address as is
            coord = MAIN.Player.M;
        }
        else
        {
            pOm->pSuper = (VOID*)&Omodel_tbl[pScd3[0xD]]->Coord_flg;  // untyped pointer, the callee takes the address as is
            coord = (UCHAR*)&Omodel_tbl[pScd3[0xD]]->Coord;  // byte view of the same memory
        }
        GsInitCoordinate2(pOm->pSuper, (GsCOORDINATE2*)&pOm->Coord_flg);  // untyped pointer, the callee takes the address as is
    }

    pOm->Push_cnt = 0;

    if (Flg_ck(&MAIN.SaveData.Item_flg, SCE_DATA_CUR[0x16]) != 0)
    {
        pOm->Push_cnt = 0;
        if (f & 0x8000)
        {
            switch (f & 0xF00)
            {
            case 0x000:
                Mot_no = 3;
                break;
            case 0x100:
                Mot_no = 0xB;
                break;
            case 0x200:
                Mot_no = 0x13;
                break;
            case 0x300:
                Mot_no = 0x1B;
                break;
            case 0x400:
                Mot_no = 4;
                break;
            case 0x500:
                Mot_no = 0xC;
                break;
            case 0x600:
                Mot_no = 0x14;
                break;
            case 0x700:
                Mot_no = 0x1C;
                break;
            default:
                Mot_no = 0x14;
                break;
            }
            pScd4 = VIEW(SCE_ITEM_AOT_REC*, G.Scenario.pSce_data);  // SCE_ITEM_AOT_REC** view of the field (a plain access changes the code)
            if (pScd4->parent == 0xFF)
            {
                ScratchPad->Vec.vx = 0;
                ScratchPad->Vec.vy = -((f & 0xF0) * 2);
                ScratchPad->Vec.vz = 0;
            }
            else
            {
                ScratchPad->Vec.vx = pScd4->pos_x;
                ScratchPad->Vec.vy = pScd4->pos_y - (f & 0xF0) * 2;
                ScratchPad->Vec.vz = pScd4->pos_z;
            }

            pOm->Push_cnt = Esp_call(0xB, Mot_no, 0, (MATRIX*)coord, &ScratchPad->Vec, 0);  // matrix of the model as bytes
        }
    }
    pOm->Be_flg = Flg_ck(&MAIN.SaveData.Item_flg, SCE_DATA_CUR[0x16]) != 0;
    pOm->Cdir_x = 0;
    pOm->Id = Imodel_num;
    pScd5 = SCE_DATA(SCE_ITEM_AOT_REC);
    pOm->Cdir_y = pScd5->angle;
    pOm->Cdir_z = 0;
    pOm->Attribute = 0x40000000;

    if (pScd5->Aot & 0x80)
    {
        pOm->Attribute = 0x40000040;
    }
    pScd6 = SCE_DATA(SCE_ITEM_AOT_REC);

    pItem_model_last = pRec;

    pOm->Coord.t[0] = pScd6->pos_x;
    pOm->Coord.t[1] = pScd6->pos_y;

    Imodel_num++;

    pOm->Coord.t[2] = pScd6->pos_z;
    SCE_DATA_NEXT(pScd6, 0x1A);

    return 1;
}

INT Sce_em_set(VOID)
{
    INT Slot_ofs;
    UCHAR killed;
    ENEMY_WORK* pEm;
    SCE_EM_SET_REC* pScd2;
    SCE_EM_SET_REC* pScd3;
    SCE_EM_SET_REC* pScd;
    ENEMY_WORK* pEm3;
    ENEMY_WORK* pEm6;
    ENEMY_WORK* pEm2;
    ENEMY_WORK* pEm4;
    ENEMY_WORK* pEm5;
    ENEMY_WORK* pEm7;

    killed = SCE_DATA(SCE_EM_SET_REC)->Em_set_flg;
    if (killed == 0xFF || Flg_ck(&MAIN.SaveData.Enemy_flg, killed) == 0)  // address passed as an integer
    {
        pScd = SCE_DATA(SCE_EM_SET_REC);
        Slot_ofs = (pScd->slot & 0xF) * 0x18C;
        EM_CUR = &MAIN.Enemy[pScd->slot & 0xF];
        *(INT*)(((UCHAR*)&MAIN.Enemy[0].Pos_y) + Slot_ofs) = pScd->Pos_y;  // INT* view of the same memory
        *(Slot_ofs + &MAIN.Enemy[0].Slot_info) = pScd->slot & 0xF;
        pEm = MAIN.pEm;
        pEm->Slot_info = pEm->Slot_info | (SCE_DATA(SCE_EM_SET_REC)->Se_set_no * 0x10);
        if (SCE_DATA(SCE_EM_SET_REC)->No_restore == 0 || (pEm2 = MAIN.pEm, pEm2->Slot_info = pEm2->Slot_info | 0x80, SCE_DATA(SCE_EM_SET_REC)->No_restore == 0))
        {
            if (Em_save_get(SCE_DATA(SCE_EM_SET_REC)->slot, pScd) == 0)
            {
                goto em_init;
            }
        }
        else
        {
        em_init:
            EM_CUR->Be_flg = 1U;
            MAIN.pEm->Type = SCE_DATA(SCE_EM_SET_REC)->Type;
            pScd2 = SCE_DATA(SCE_EM_SET_REC);
            pEm3 = MAIN.pEm;
            pEm3->Cdir_y = pScd2->Cdir_y;
            pEm3->Pos_x = pScd2->Pos_x;
            pEm3->Pos_z = pScd2->Pos_z;
            pEm3->Old_x = pEm3->Pos_x;
            pEm3->Old_y = pEm3->Pos_y;
            pEm3->Old_z = pEm3->Pos_z;
            pEm3->Move_no = pScd2->anim_id;
            MAIN.pEm->Move_cnt = SCE_DATA(SCE_EM_SET_REC)->anim_frame;
            MAIN.pEm->Move_wait = 1;
        }
        pEm4 = EM_CUR;
        if (pEm4->Be_flg & EM_BE_ENABLED)
        {
            (ROUTINE_WORD(pEm4)) = 0;  // INT* view of the field (a plain access changes the code)
            pEm4->Id = SCE_DATA(SCE_EM_SET_REC)->Id;
            MAIN.pEm->Em_set_flg = SCE_DATA(SCE_EM_SET_REC)->Em_set_flg;
            pScd3 = SCE_DATA(SCE_EM_SET_REC);
            pEm5 = MAIN.pEm;
            pEm5->Cdir_x = pScd3->Cdir_x;
            pEm5->Cdir_z = pScd3->Cdir_z;
            pEm5->Damage_no = 0;
            pEm6 = MAIN.pEm;
            pEm6->Scale = 0;
            pEm6->Sca_info = 0;
            MAIN.pEm->Neck_flg = 0;
            pEm7 = MAIN.pEm;
            pEm7->pAt_tbl = Pl_sca_ptr[0];
            pEm7->pSca_ofs = pSca_hit_cur;  // reinterprets the memory as AT_WORK
            pSca_hit_cur += SCE_DATA(SCE_EM_SET_REC)->Sca_num;
            ENEMY_NUM_CUR += 1;
        }
    }
    SCE_DATA_WORD = (INT)MAIN.Scenario.pSce_data + 0x16;  // INT* view of the field (a plain access changes the code)
    return 1;
}

INT Movie_on(VOID)
{
    USHORT* pScd;

    MAIN.System_flg |= SYS_MOVIE;
    pScd = SCE_DATA(USHORT);
    G.Scenario.Movie_id = *pScd >> 8;
    G.Scenario.pMovie_buf = G.pFree_work;
    SCE_DATA(USHORT) = pScd + 1;
    return 1;
}

INT Xa_on(VOID)
{
    USHORT* pScd;
    USHORT head;

    pScd = SCE_DATA(USHORT);
    head = pScd[0];
    SCE_DATA(USHORT) = pScd + 1;
    Xa_cmd(head >> 8, pScd[1]);
    MAIN.Scenario.pSce_data += 2;
    MAIN.System_flg |= SYS_XA_VOICE;
    return 1;
}

INT Obj_model_set(VOID)
{
    INT id;
    SHORT z;
    SHORT x;
    USHORT angle;
    SHORT y;
    UCHAR Id_flg;
    INT parent;
    UCHAR* pRaw;
    SCE_OBJ_SET_REC* pScd;
    OBJ_MODEL_WORK* pOm;
    MODEL_LINK_INFO* pRec;
    VOID* attr;

    id = ((SCE_OBJ_SET_REC*)G.Scenario.pSce_data)->Om_no & 0x3F;  // reinterprets the memory as SCE_OBJ_SET_REC
    pRec = &G.pRoom->pOmodel[id];
    pOm = Omodel_tbl[id];
    if (pRec->Tmd_adr == 0)
    {
        pOm->pTmd = 0;
    }
    else
    {
        if ((Omodel_num == 0 || pOmodel_last->Tim_adr != pRec->Tim_adr) && pRec->Tim_adr != 0)
        {
            Omodel_tpage = G.Vram_cur.Tpage_clut;
            Omodel_clut = G.Vram_cur.b.Cclut;
            Mapping_tim(pRec->Tim_adr);
        }
        if (Omodel_num == 0 || (attr = &pOm->Attribute, pOmodel_last->Tmd_adr != pRec->Tmd_adr))
        {
            Mapping_tmd(2, pRec->Tmd_adr, Omodel_tpage, Omodel_clut);
            Id_flg = ((SCE_OBJ_SET_REC*)G.Scenario.pSce_data)->Om_no;  // reinterprets the memory as SCE_OBJ_SET_REC
            if (Id_flg & 0x80)
            {
                Cinesco_fade_add(Omodel_clut, Id_flg & 0xBF);
            }
            attr = &pOm->Attribute;
        }
        Tmd_obj_link(pRec->Tmd_adr, attr, &pOm->Coord_flg);
        pRaw = G.Scenario.pSce_data;
        parent = pRaw[3];
        switch (parent)
        {
        case 0xFF:
            pOm->pSuper = 0;
            break;
        case 0xFE:
            pOm->pSuper = (GsCOORDINATE2*)&MAIN.Player.Coord_flg;  // the embedded GsCOORDINATE2 starts at Coord_flg
            break;
        default:
            if (parent < 0x80U)
            {
                pOm->pSuper = (VOID*)&Omodel_tbl[parent]->Coord_flg;  // untyped pointer, the callee takes the address as is
            }
            else
            {
                pOm->pSuper = (VOID*)&MAIN.Enemy[pRaw[3] & 0x7F].Coord_flg;  // parent coordinate block as an untyped pointer
            }
            break;
        }
        GsInitCoordinate2(pOm->pSuper, (GsCOORDINATE2*)&pOm->Coord_flg);  // the embedded GsCOORDINATE2 starts at Coord_flg
    }
    pOm->Attribute = 0x40000000;
    if (SCE_DATA(SCE_OBJ_SET_REC)->Be_flg & 0x10)
    {
        pOm->Attribute = 0x40000040;
    }
    pOm->Be_flg = SCE_DATA(SCE_OBJ_SET_REC)->Be_flg;
    pOm->Id = SCE_DATA(SCE_OBJ_SET_REC)->Om_no & 0x7F;
    pOm->Cdir_x = 0;
    pScd = SCE_DATA(SCE_OBJ_SET_REC);
    angle = pScd->Cdir_y;
    pOm->pAt_tbl = (AT_TBL*)&pOm->Sca_flg;  // reinterprets the memory as AT_TBL
    pOm->Cdir_z = 0;
    pOm->Sca_flg = -0x8000;
    pOm->Old_dir = angle;
    pOm->Cdir_y = angle;
    pOm->Sca_ofs_x = pScd->half_x;
    pOm->Sca_ofs_y = pScd->At_h;
    pOm->Sca_ofs_z = pScd->half_z;
    pOm->Sca_half_h = pScd->At_h;
    pOm->Sca_r = pScd->radius;
    pOm->Probe1.vx = pScd->Probe1_x;
    pOm->Probe1.vz = pScd->Probe1_z;
    pOm->Probe2.vx = pScd->Probe2_x;
    pOm->Probe2.vz = pScd->Probe2_z;
    x = pScd->Pos_x;
    pOm->Old_x = x;
    pOm->Coord.t[0] = x;  // sign-extends the halfword
    y = pScd->Pos_y;
    pOmodel_last = pRec;
    pOm->Old_y = y;
    pOm->Coord.t[1] = y;
    z = pScd->Pos_z;
    Omodel_num += 1;
    pOm->Old_z = z;
    pOm->Coord.t[2] = z;  // sign-extends the halfword
    SCE_DATA_NEXT(pScd, 0x1C);
    return 1;
}

INT Sce_psp_set(VOID)
{
    USHORT sprite;
    SCE_PSP_SET_REC* pScd;

    pScd = SCE_DATA(SCE_PSP_SET_REC);
    SCE_DATA_NEXT(pScd, 2);
    sprite = pScd->sprite;
    if ((sprite >> 8) != 0)
    {
        Psp_off(sprite & 0xFF);
    }
    else
    {
        Psp_on(sprite & 0xFF);
    }
    SCE_DATA_WORD = (INT)MAIN.Scenario.pSce_data + 2;  // INT* view of the field (a plain access changes the code)
    return 1;
}

INT Sce_fade_set(VOID)
{
    USHORT* pScd;
    USHORT* pArg;
    USHORT v;
    SHORT b;

    pScd = SCE_DATA(USHORT);
    v = *pScd;
    SCE_DATA(USHORT) = pScd + 1;
    G.SaveData.Fade1_hrate = v >> 8;
    pArg = pScd + 1;
    b = *pArg;
    SCE_DATA(USHORT) = pScd + 2;
    G.SaveData.Fade1_add = b;
    pArg = pScd + 2;
    v = *pArg;

    MASK_R_CUR = MASK_G_CUR = MASK_B_CUR = 0;

    if (v & 1)
    {
        MAIN.Mask_b = 0xFF;
    }
    if (v & 2)
    {
        MAIN.Mask_g = 0xFF;
    }
    if (v & 4)
    {
        MAIN.Mask_r = 0xFF;
    }
    SCE_DATA(USHORT) = pScd + 3;
    if (b != 0)
    {
        {
            INT fade_in = b < 1;
            G.SaveData.Fade1_kido = -fade_in & 0x7FFF;
        }
    }

    return 1;
}

INT Vib_set0(VOID)
{
    SCE_VIB_REC* pScd;
    USHORT a;

    pScd = SCE_DATA(SCE_VIB_REC);
    SCE_DATA_NEXT(pScd, 2);
    a = pScd->Time;
    SCE_DATA_NEXT(pScd, 4);
    Pad_vib_set0(a, pScd->Attr);
    MAIN.Scenario.pSce_data += 2;
    return 1;
}

INT Vib_set1(VOID)
{
    SCE_VIB_REC* pScd;
    USHORT c;
    USHORT a;

    pScd = SCE_DATA(SCE_VIB_REC);
    c = pScd->head;
    SCE_DATA_NEXT(pScd, 2);
    a = pScd->Time;
    SCE_DATA_NEXT(pScd, 4);
    Pad_vib_set1(a, c >> 8, pScd->Attr);
    MAIN.Scenario.pSce_data += 2;
    return 1;
}

INT Vib_fade_set(VOID)
{
    USHORT strength;
    USHORT duration;
    SCE_VIB_FADE_REC* pScd;

    if ((SCE_DATA(SCE_VIB_FADE_REC)->head >> 8) != 0)
    {
        Pad_vib_init();
    }
    pScd = SCE_DATA(SCE_VIB_FADE_REC);
    SCE_DATA_NEXT(pScd, 2);
    strength = pScd->strength;
    SCE_DATA_NEXT(pScd, 4);
    duration = pScd->Time;
    SCE_DATA_NEXT(pScd, 6);
    Pad_vib_fade_set(duration, strength & 0xFF, strength >> 8, pScd->Attr);
    MAIN.Scenario.pSce_data += 2;
    return 1;
}

INT Vib_stop(VOID)
{
    SCE_DATA_CUR += 2;
    Pad_vib_init();

    return 1;
}

INT Sce_pl_pos_set(VOID)
{
    SHORT x;
    SHORT y;
    SHORT z;
    SCE_PL_POS_REC* pScd;

    pScd = SCE_DATA(SCE_PL_POS_REC);
    MAIN.Player.Sce_flg &= 0xFFF3;
    G.Player.Cdir_x = pScd->Cdir_x;
    G.Player.Cdir_y = pScd->Cdir_y;
    G.Player.Cdir_z = pScd->Cdir_z;
    x = pScd->Pos_x;
    G.Player.Old_x = x;
    G.Player.Pos_x = x;  // sign-extends the halfword
    y = pScd->Pos_y;
    G.Player.Old_y = y;
    G.Player.Pos_y = y;  // sign-extends the halfword
    z = pScd->Pos_z;
    SCE_DATA_NEXT(pScd, 0xE);
    G.Player.Old_z = z;
    G.Player.Pos_z = z;  // sign-extends the halfword
    return 1;
}

INT Sce_em_pos_set(VOID)
{
    USHORT* pScd;
    ENEMY_WORK* pEm;
    SHORT v;

    pScd = SCE_DATA(USHORT);
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 7);
    pEm = &MAIN.Enemy[(SHORT)*pScd++ >> 8];  // signed 16-bit script word, sign-extended before the shift
    pEm->Sce_flg &= 0xFFF3;

    do
    {
        pEm->Cdir_x = *pScd++;
        pEm->Cdir_y = *pScd++;
        pEm->Cdir_z = *pScd++;
        v = *pScd++;
    } while (0);
    pEm->Old_x = v;
    pEm->Pos_x = v;
    v = pScd[0];
    pEm->Old_y = v;
    pEm->Pos_y = v;
    v = pScd[1];
    pEm->Old_z = v;
    pEm->Pos_z = v;
    return 1;
}

INT Obj_trans_set(VOID)
{
    SHORT* pScd;
    OBJ_MODEL_WORK* pOm;
    SHORT v;

    pScd = SCE_DATA(SHORT);
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 7);
    pOm = Omodel_tbl[*pScd++ >> 8];
    pOm->Cdir_x = *pScd++;
    pOm->Cdir_y = *pScd++;
    pOm->Cdir_z = *pScd++;
    pOm->Old_x = v = *pScd++;
    pOm->Coord.t[0] = v;
    pOm->Old_y = v = *pScd++;
    pOm->Coord.t[1] = v;
    pOm->Old_z = v = *pScd++;
    pOm->Coord.t[2] = v;
    return 1;
}

INT Item_cnt_ck(VOID)
{
    UCHAR* pScd;
    UINT i;
    UINT id;
    UINT val;
    INT Loop_ctr;

    pScd = SCE_DATA_CUR;
    Loop_ctr = 0;
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 4);
    id = *(USHORT*)pScd >> 8;  // halfword view of the same memory
    for (i = 0; i < G.SaveData.Item_num; i++)
    {
        if (id == pItem_work_base[i].Id || id == pItem_work_base[i].Id + 11)
        {
            val = pItem_work_base[i].Num;
        }
        switch (id)
        {
        case 11:
            if (pItem_work_base[i].Id == 2)
            {
                Loop_ctr++;
                val += pItem_work_base[i].Num;
            }
            break;
        case 12:
            if (pItem_work_base[i].Id == 3)
            {
                Loop_ctr++;
                val += pItem_work_base[i].Num;
            }
            break;
        case 13:
            if (pItem_work_base[i].Id == 4 || pItem_work_base[i].Id == 5)
            {
                Loop_ctr++;
                val += pItem_work_base[i].Num;
            }
            break;
        case 15:
            if (pItem_work_base[i].Id == 6)
            {
                Loop_ctr++;
                val += pItem_work_base[i].Num;
            }
            break;
        case 16:
        case 17:
        case 18:
            if (pItem_work_base[i].Id == 7 || pItem_work_base[i].Id == 8 || pItem_work_base[i].Id == 9)
            {
                Loop_ctr++;
                val += pItem_work_base[i].Num;
            }
            break;
        case 10:
            val = pItem_work_base[i].Num;
            Loop_ctr++;
            break;
        }
    }
    pScd += 2;
    id = *(USHORT*)pScd >> 8;  // halfword view of the same memory
    if (Loop_ctr != 0)
    {
        switch (pScd[0])
        {
        case CMP_EQ:
            return val == id;
        case CMP_GT:
            return id < val;
        case CMP_GE:
            return val >= id;
        case CMP_LT:
            return val < id;
        case CMP_LE:
            return id >= val;
        case CMP_NE:
            return val != id;
        }
    }
    return 0;
}

INT Cut_auto(VOID)
{
    if (((SCE_CUT_AUTO_REC*)G.Scenario.pSce_data)->OnOff == 0)  // reinterprets the memory as SCE_CUT_AUTO_REC
    {
        MAIN.System_flg &= ~SYS_CAM_LOCK;
    }
    else
    {
        MAIN.System_flg = MAIN.System_flg | SYS_CAM_LOCK;
    }
    SCE_DATA_CUR += 2;
    return 1;
}

INT Aot_on(VOID)
{
    UCHAR* pScd;
    SCE_AOT* pAot;

    pScd = SCE_DATA_CUR;
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 4);
    pAot = &Sce_aot_tbl[pScd[1]];
    Sce_at_jump_tbl[pScd[2]](pAot);
    return 1;
}

INT Sce_em_prm_set(VOID)
{
    UCHAR* pScd;

    ENEMY_WORK* pEm;
    PARTS_W* pParts;
    USHORT w;
    USHORT n;

    pScd = MAIN.Scenario.pSce_data;
    pScd += 2;
    w = *(USHORT*)pScd;  // halfword view of the same memory

    pEm = &MAIN.Enemy[w & 0xFF];

    pScd += 2;

    switch ((SCHAR)(w >> 8))  // sign-extends the byte
    {
    case 0:
        pEm->Type = pScd[0];
        SCE_DATA_CUR += 6;
        break;
    case 1:
        pEm->Routine_0 = 2;
        pEm->Routine_1 = 0;
        pEm->Routine_2 = 0;
        pEm->Routine_3 = 0;

        pEm->Life = *(USHORT*)pScd;  // halfword view of the same memory
        pEm->Damage_no = pScd[2];

        SCE_DATA_CUR += 8;
        break;
    case 2:
        pEm->Routine_2 = pScd[0];
        pEm->Routine_3 = 0;
        SCE_DATA_CUR += 6;
        break;
    case 3:
        switch ((SCHAR)(*(USHORT*)pScd >> 8))  // sign-extends the byte
        {
        case 0:
            pEm->Be_flg = pScd[0];
            break;
        case 1:
            pEm->Be_flg |= pScd[0];
            break;
        case 2:
            pEm->Be_flg ^= pScd[0];
            break;
        }
        SCE_DATA_CUR += 6;
        break;
    case 5:
        pEm->Cdir_y = *(USHORT*)pScd;  // halfword view of the same memory

        SCE_DATA_CUR += 6;
        break;
    case 6:
        pEm->Hokan_flg = 0;
        SCE_DATA_CUR += 4;
        break;
    case 8:
        pEm->Routine_0 = 9;
        pEm->Routine_1 = 0;
        pEm->Routine_2 = 0;
        pEm->Routine_3 = 0;
        SCE_DATA_CUR += 4;
        break;
    case 9:
        n = *(USHORT*)pScd;  // halfword view of the same memory

        pParts = pEm->pSin_parts_ptr;
        while (n != 0)
        {
            pParts->Be_flg ^= n & 1;
            n >>= 1;
            pParts++;
        }
        SCE_DATA_CUR += 6;
        break;
    case 10:
        pEm->Routine_3 = pScd[0];
        SCE_DATA_CUR += 6;
        break;
    }
    return 1;
}

static const UINT Sce_pad6 = 0;  // padding: keeps the following read-only data at its linked position

INT Sce_pl_prm_set(VOID)
{
    ENEMY_WORK* pEm = PL_EM;
    USHORT* pScd = (USHORT*)G.Scenario.pSce_data;
    USHORT n;
    PARTS_W* pParts;
    UCHAR v;
    INT* pp;

    switch ((SHORT)*pScd++ >> 8)  // sign-extends the halfword
    {
    case 1:
        pp = (INT*)&MAIN.Scenario.pSce_data;  // word view of the same memory

        v = *pScd;

        PL_ROUTINE_WORD = 0x01000001;
        G.Player.Move_cnt = 0;
        G.Player.Move_wait = 0;
        G.Player.Move_no = 0;
        G.Player.Hokan_flg = 3;
        G.Player.Timer0 = 100;
        G.Player.Spd_cur = 0;

        pEm->Damage_no = v;

        *pp += 4;
        break;
    case 0:
        G.SaveData.Equip_no = 0;
        pEm->Type = 0;
        SCE_DATA_CUR += 2;
        break;
    case 3:
        switch ((SHORT)*pScd >> 8)  // sign-extends the halfword
        {
        case 0:
            pEm->Be_flg = *pScd;
            break;
        case 1:
            pEm->Be_flg |= *pScd;
            break;
        case 2:
            pEm->Be_flg ^= *pScd;
            break;
        }
        SCE_DATA_CUR += 4;
        break;
    case 4:
        pEm->Routine_2 = 1;
        pEm->Routine_3 = 6;
        SCE_DATA_CUR += 2;
        break;
    case 5:
        pEm->Cdir_y = *pScd;
        SCE_DATA_CUR += 4;
        break;
    case 6:
        pEm->Hokan_flg = 0;
        SCE_DATA_CUR += 2;
        break;
    case 7:
        (ROUTINE_UHALF0(pEm)) = 1;  // USHORT* view of the field (a plain access changes the code)
        (ROUTINE_UHALF2(pEm)) = 0x200;  // USHORT* view of the field (a plain access changes the code)
        pEm->Damage_no = 0;
        pEm->Move_cnt = 0;
        pEm->Move_wait = 0;
        pEm->Move_no = 0;
        pEm->Hokan_flg = 3;
        SCE_DATA_CUR += 2;
        break;
    case 8:
        switch ((SHORT)*pScd >> 8)  // sign-extends the halfword
        {
        case 0:
            pEm->Sca_info = *pScd;
            break;
        case 1:
            pEm->Sca_info |= *pScd;
            break;
        case 2:
            pEm->Sca_info ^= *pScd;
            break;
        }
        SCE_DATA_CUR += 4;
        break;
    case 9:
        n = *pScd;
        pParts = pEm->pSin_parts_ptr;
        while (n != 0)
        {
            pParts->Be_flg ^= n & 1;
            n >>= 1;
            pParts++;
        }
        SCE_DATA_CUR += 4;
        break;
    case 10:
        if (*pScd >> 8)
        {
            pEm->Sce_flg |= 0x40;
        }
        else
        {
            pEm->Sce_flg &= ~0x40;
        }
        SCE_DATA_CUR += 4;
        break;
    }
    return 1;
}

static const UINT Sce_pad7 = 0;  // padding: keeps the following read-only data at its linked position

INT Sce_espr_on(VOID)
{
    INT v[3];
    MATRIX* pMat;
    SHORT* pScd;
    SHORT op;
    USHORT b;
    INT k;
    SHORT x;

    pScd = (SHORT*)G.Scenario.pSce_data;
    x = *pScd++;
    b = *pScd++;
    v[0] = *pScd++;
    op = x >> 8;  // sign-extends the halfword
    v[1] = *pScd++;
    v[2] = *pScd++;
    k = b >> 8;
    switch (b >> 8)
    {
    case 0:
        pMat = &GsIDMATRIX;
        break;
    case 1:
        pMat = MATRIX_OF(&MAIN.Player);
        break;
    default:
        if ((b >> 8) & 0x80)
        {
            pMat = &Omodel_tbl[(b >> 8) & 0x7F]->Coord;
        }
        else
        {
            pMat = (MATRIX*)&((ENEMY_WORK*)&MAIN.Esp[61].Speed_z)[b >> 8];
        }
        break;
    }

    Esp_call((UCHAR)op, (UCHAR)b, *pScd, pMat, (VECTOR*)v, 0);  // byte arguments (the old-style Esp_call does not narrow)

    SCE_DATA_CUR += 0xC;

    return 1;
}

INT Sce_espr_on2(VOID)
{
    INT v[3];
    SHORT* pScd;
    SHORT a;
    USHORT b;
    USHORT id;
    MATRIX* pMat;

    pScd = (SHORT*)G.Scenario.pSce_data;
    a = *pScd++;
    b = *pScd++;
    v[0] = *pScd++;
    id = a >> 8;  // sign-extends the halfword
    v[1] = *pScd++;
    v[2] = *pScd++;
    switch (b >> 8)
    {
    case 0:
        pMat = &GsIDMATRIX;
        break;
    case 1:
        pMat = MATRIX_OF(&MAIN.Player);
        break;
    default:
        if ((b >> 8) & 0x80)
        {
            pMat = &Omodel_tbl[id >> 8]->Coord;
        }
        else
        {
            pMat = (MATRIX*)&((ENEMY_WORK*)&MAIN.Esp[61].Speed_z)[id >> 8];
        }
        break;
    }

    Esp_call((UCHAR)id, (UCHAR)b, *pScd, pMat, (VECTOR*)v, 0);  // byte arguments (the old-style Esp_call does not narrow)

    G.SaveData.Tama_esp = id;
    VIEW(INT, G.pTama_m) = (INT)pMat;  // INT* view of the field (a plain access changes the code)
    SCE_DATA_CUR += 0xC;

    return 1;
}

INT Sce_espr_kill(VOID)
{
    Esp_kill(9, G.SaveData.Tama_esp, 0, 0, G.pTama_m);

    SCE_DATA_CUR += 2;

    return 1;
}

INT Sce_espr_kill2(VOID)
{
    USHORT* pScd;
    USHORT b;
    UINT a;

    pScd = SCE_DATA(USHORT);

    pScd++;
    pScd--;
    a = pScd[0];
    b = pScd[1];
    MAIN.Scenario.pSce_data += 4;

    Esp_kill(3, a >> 8, b & 0xFF, 0, 0);
    return 1;
}

INT Sce_espr_clr(VOID)
{
    INT i;

    G.Scenario.nEsp_free = 0;

    SCE_DATA_CUR += 2;

    do
    {
        i = ESP_FREE_CNT;
        MAIN.Esp[i].Routine1 = 0;
        (MAIN.Esp + i)->Routine0 = 0;
        MAIN.Scenario.nEsp_free += 1;
    } while (MAIN.Scenario.nEsp_free < 0x40);
    return 1;
}

INT Sce_espr_control(VOID)
{
    UCHAR** pp;
    UINT op;
    UINT val;
    UINT i;
    USHORT* pScd;

    pp = &MAIN.Scenario.pSce_data;
    pScd = (USHORT*)*pp;
    op = pScd[0] >> 8;
    *pp = (UCHAR*)(pScd + 1);
    val = pScd[1];
    *pp = (UCHAR*)(pScd + 2);
    for (i = 0; i < 0x40; i++)
    {
        if (VIEW(USHORT, MAIN.Esp[i].Routine0) != 0)  // USHORT* view of the field (a plain access changes the code)
        {
            switch (op)
            {
            case 0:
                MAIN.Esp[i].Be_flg = MAIN.Esp[i].Be_flg | val;
                break;
            case 1:
                MAIN.Esp[i].Be_flg &= ~val;
                break;
            case 2:
                MAIN.Esp[i].Be_flg = MAIN.Esp[i].Be_flg ^ val;
                break;
            }
        }
    }
    return 1;
}

INT Sce_atk_anim_set(VOID)
{
    volatile INT* Data = (INT*)&MAIN.Scenario.pSce_data;  // word view of the same memory
    USHORT a;
    USHORT b;
    USHORT* pScd;
    INT t;
    INT x;

    do
    {
        t = *Data + 4;
        pScd = *(USHORT**)Data;  // halfword view of the same memory
        a = pScd[0];
        x = a;
        b = pScd[1];
        *Data = t;
        MAIN.Player.Routine_0 = 8;
        MAIN.Player.Move_wait = 0;
        MAIN.Player.Hokan_flg = 0;
    } while (0);
    x += 0x200;
    x &= 0xFF00;
    PL_ROUTINE_HALF2 = x;
    G.Player.Move_no = b;
    b >>= 8;
    G.Player.Move_cnt = b;

    return 1;
}

INT Sce_Item_lost(VOID)
{
    INT id;
    SCE_ITEM_OP_REC* pScd;

    pScd = SCE_DATA(SCE_ITEM_OP_REC);
    SCE_DATA_NEXT(pScd, 2);
    id = Search_item_id(pScd->Item_no);
    if (id >= 0)
    {
        pItem_work_base[id].Id = 0;
        Sort_item();
        return 1;
    }
    return 0;
}

INT Sce_Item_get(VOID)
{
    Aot_on();
    MAIN.System_flg = (MAIN.System_flg | SYS_REQ_ITEM_SCE) ^ SYS_REQ_ITEM_GET;
    return 0;
}

INT Sca_id_set(VOID)
{
    USHORT v;
    USHORT* pScd;
    SCA_DATA* pT_xz;

    pScd = SCE_DATA(USHORT);
    v = pScd[0];
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 1);
    pT_xz = (SCA_DATA*)(((v >> 8) << 2) + (UCHAR*)G.pRoom->pSca);
    v = pScd[1];
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 2);
    pT_xz = &VIEW(SCA_DATA*, pT_xz->Min_x)[v & 0xFF];  // SCA_DATA** view of the field (a plain access changes the code)
    if (v >> 8)
    {
        pT_xz->Type = (pT_xz->Type & 0xF0FF) | (v & 0xF00);
    }
    v = *SCE_DATA(USHORT)++;
    pT_xz->Min_x = v;
    v = *SCE_DATA(USHORT)++;
    pT_xz->Min_z = v;
    v = *SCE_DATA(USHORT)++;
    pT_xz->Max_x = v;
    v = *SCE_DATA(USHORT)++;
    pT_xz->Max_z = v;
    return 1;
}

INT Obj_clut_set(VOID)
{
    USHORT* pScd;
    USHORT a;
    USHORT b;
    OBJ_MODEL_WORK* pOm;
    POLY_FT4* pPoly;
    UCHAR* pRaw;
    LONG n;

    pScd = SCE_DATA(USHORT);
    a = pScd[0];
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 1);
    b = pScd[1];
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 2);
    pOm = Omodel_tbl[a >> 8];
    pPoly = pOm->pPacket;
    n = pOm->pTmd->n_poly * 2;
    pRaw = (UCHAR*)pPoly + 8;  // byte view of the same memory
    a = (pPoly->tpage & 0xFFC0) | (b & 0xFF);
    goto test;
loop:
    (((POLY_FT4*)(pRaw - 8))->tpage) = a;  // reinterprets the memory as POLY_FT4
    pRaw += sizeof(POLY_FT4);
test:
    if (n-- != 0)
    {
        goto loop;
    }
    return 1;
}

INT Col_chg_set(VOID)
{
    UCHAR** pScd;
    SCHAR op;
    UCHAR b;
    UCHAR c;
    UCHAR d;
    UCHAR e;
    UCHAR f;
    UCHAR g;

    pScd = &MAIN.Scenario.pSce_data;
    (*pScd)++;
    op = *(*pScd)++;
    b = *(*pScd)++ - 0x80;
    c = *(*pScd)++;
    d = *(*pScd)++;
    e = *(*pScd)++;
    f = *(*pScd)++;
    g = *(*pScd)++;
    switch (op)
    {
    case 0:
        Cinesco_fade_sabun_add(e, f, g, c, d, b);
        break;
    case 1:
        Cinesco_fade_set(e, f, g, c, d, b);
        break;
    case 2:
        Cinesco_fade_off(b, c, d);
        break;
    }
    return 1;
}

INT Obj_flg_set(VOID)
{
    USHORT* pScd;
    USHORT a;
    USHORT b;

    pScd = SCE_DATA(USHORT);
    a = pScd[0];
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 1);
    b = pScd[1];
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 2);
    switch (a >> 8)
    {
    case 0:
        Omodel_tbl[b & 0xFF]->Be_flg = b >> 8;
        break;
    case 1:
        pOm_tbl[b & 0xFF]->Be_flg = b >> 8;
        break;
    }
    return 1;
}

INT Obj_prm_ck(VOID)
{
    USHORT* pScd;
    USHORT a;
    USHORT b;

    pScd = SCE_DATA(USHORT);
    a = pScd[0];
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 1);
    b = pScd[1];
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 2);
    switch (b & 0xFF)
    {
    case CMP_EQ:
        return Omodel_tbl[a >> 8]->Push_cnt == (b >> 8);
    case CMP_GT:
        return (b >> 8) < Omodel_tbl[a >> 8]->Push_cnt;
    case CMP_GE:
        return Omodel_tbl[a >> 8]->Push_cnt >= (b >> 8);
    case CMP_LT:
        return Omodel_tbl[a >> 8]->Push_cnt < (b >> 8);
    case CMP_LE:
        return (b >> 8) >= Omodel_tbl[a >> 8]->Push_cnt;
    case CMP_NE:
        return Omodel_tbl[a >> 8]->Push_cnt != (b >> 8);
    default:
        return 0;
    }
}

INT Sce_key_ck(VOID)
{
    USHORT* pScd;
    USHORT a;
    USHORT b;

    pScd = SCE_DATA(USHORT);
    a = pScd[0];
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 1);
    b = pScd[1];
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 2);
    if ((a >> 8) != 0)
    {
        return (b & G.SaveData.Key) == 0;
    }

    return b & G.SaveData.Key;
}

INT Sce_em_flg_get(VOID)
{
    USHORT* pScd;
    UCHAR v;

    pScd = SCE_DATA(USHORT);
    G.SaveData.Em_flg_bak = MAIN.Enemy[*pScd >> 8].Type;
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 1);
    return 1;
}

INT Cut_replace(VOID)
{
    SCE_CUT_REPL_REC* pScd;

    pScd = SCE_DATA(SCE_CUT_REPL_REC);
    (*(ROOM_WORK**)(CHAR*)&G.pRoom)->pVcut[pScd->zone].Fcut = pScd->Fcut;  // pRoom viewed as a ROOM_WORK** (a plain access changes the code)
    ROOM_NOW->pVcut[pScd->zone].Tcut = pScd->Tcut;
    SCE_DATA_NEXT(pScd, 4);
    return 1;
}

INT Obj_rot_set(VOID)
{
    INT unused[2];
    UINT head;
    UINT Head16;
    VOID* pScd;
    VOID* pArg;
    OBJ_MODEL_WORK* pOm;

    pScd = SCE_DATA_CUR;
    head = ((USHORT*)pScd)[0];  // halfword view of the same memory
    MAIN.Scenario.pSce_data = (UCHAR*)((UCHAR*)pScd + 2);
    Head16 = head & 0xFFFF;
    if ((head >> 0xF) == 0)
    {
        pOm = pOm_tbl[Head16 >> 8];
    }
    else
    {
        pOm = Omodel_tbl[(Head16 >> 8) & 0x7F];
    }
    if (pOm->Be_flg != 0)
    {
        pArg = SCE_DATA_CUR;
        pOm->Cdir_x = ((USHORT*)pArg)[0];  // halfword view of the same memory
        SCE_DATA_WORD = (INT)((UCHAR*)pArg + 2);  // INT* view of the field (a plain access changes the code)
        pOm->Cdir_z = ((USHORT*)pArg)[1];  // halfword view of the same memory
        SCE_DATA_WORD = (INT)((UCHAR*)pArg + 4);  // INT* view of the field (a plain access changes the code)
    }
    else
    {
        SCE_DATA_WORD = (INT)MAIN.Scenario.pSce_data + 4;  // INT* view of the field (a plain access changes the code)
    }
    return 1;
}

INT Sce_pl_dist_ck(VECTOR* pPos)
{
    INT dx;
    INT dz;

    VOID* args;

    VECTOR* pos;

    pos = pPos;
    args = G.Scenario.pSce_data + 2;
    switch (*(UCHAR*)&((SCE_PL_DIST_REC*)(SCE_DATA_NOW))->target)  // VOID** view of the field (a plain access changes the code)
    {
    case 0:
        pos = (VECTOR*)&MAIN.Enemy[(USHORT)((SCE_PL_DIST_REC*)SCE_DATA_NOW)->target >> 8].Pos_x;  // position fields viewed as a VECTOR
        break;
    case 1:
        pos = (VECTOR*)Omodel_tbl[(USHORT)((SCE_PL_DIST_REC*)(SCE_DATA_NOW))->target >> 8]->Coord.t;  // VOID** view of the field (a plain access changes the code)
        break;
    case 2:
        pos = (VECTOR*)pOm_tbl[(USHORT)((SCE_PL_DIST_REC*)(SCE_DATA_NOW))->target >> 8]->Coord.t;  // VOID** view of the field (a plain access changes the code)
        break;
    }
    dx = G.Player.Pos_x - pos->vx;
    dz = G.Player.Pos_z - pos->vz;
#ifdef __INTELLISENSE__
    SCE_DATA_CUR = (UCHAR*)SCE_DATA_CUR + 6;  // IntelliSense rejects void* arithmetic
#else
    SCE_DATA_CUR += 6;
#endif

    return ((SCE_PL_DIST_REC*)((UCHAR*)args - 2))->distance >= SquareRoot0(dx * dx + dz * dz);  // signed conversion (unsigned operand)
}

INT Dir_ck(VOID)
{
    SCE_PL_DIR_REC* pScd;

    pScd = SCE_DATA(SCE_PL_DIR_REC);
    SCE_DATA_NEXT(pScd, 6);

    return ((G.Player.Cdir_y - (*(volatile USHORT*)&pScd->Min_dir)) & 0xFFFF) <= pScd->Max_dir - pScd->Min_dir;  // USHORT* view of the field (a plain access changes the code)
}

INT Sce_light_prm_set(VOID)
{
    SHORT* pScd;
    RCUT* pView;
    INT idx;

    pScd = SCE_DATA(SHORT);
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 8);
    idx = *pScd++ >> 8;  // sign-extends the byte
    {
        INT cut_ofs = OFFSET_OF(ROOM_WORK, Cut[idx]);
        pView = BYTE_OFS(RCUT, G.pRoom, cut_ofs);
    }
    pView->View.vpx = *pScd++;
    pView->View.vpy = *pScd++;
    pView->View.vpz = *pScd++;
    pView->View.vrx = *pScd++;
    pView->View.vry = *pScd++;
    pView->View.vrz = *pScd++;
    pView->ViewR = *pScd;
    return 1;
}

INT Sce_em_posy_set(VOID)
{
    UINT target;
    SCE_EM_POSY_REC* pScd;

    pScd = SCE_DATA(SCE_EM_POSY_REC);
    target = pScd->head >> 8;
    SCE_DATA_NEXT(pScd, 2);
    if (target == 0)
    {
        G.Player.Ground = pScd->y;
    }
    else
    {
        *(SHORT*)&((ENEMY_WORK*)&MAIN.Player.Aspd_y)[target] = pScd->y;  // SHORT* view of the field (a plain access changes the code)
    }
    SCE_DATA_WORD = (INT)MAIN.Scenario.pSce_data + 2;  // INT* view of the field (a plain access changes the code)
    return 1;
}

INT Sce_pl_posy_add(VOID)
{
    SHORT* pScd;

    pScd = SCE_DATA(SHORT);
    MAIN.Player.Ground += (UINT)*pScd / 256;  // the halfword is sign extended and divided as an unsigned word (no rounding step for negative values)
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 1);
    return 1;
}

INT Sce_room_light_set(VOID)
{
    UINT i;
    INT off;
    SCE_LIGHT_PRM_REC* pScd;
    ROOM_WORK* pRoom;
    ROOM_WORK* pRoom2;
    SCE_LIGHT_PRM_REC* pScd2;

    SCE_DATA_CUR += 2;
    for (i = 0; i < 3; i++)
    {
        off = i * sizeof(LIGHT_CUT_DATA);
        pRoom = ROOM_CUR;
        pRoom = BYTE_OFS(ROOM_WORK, pRoom, off);
        pScd = SCE_DATA(SCE_LIGHT_PRM_REC);
        pRoom->Light[0].Flat.vx = pScd->x;
        pRoom->Light[0].Flat.vy = pScd->y;
        pRoom->Light[0].Flat.vz = pScd->z;
        pRoom->Light[0].Flat.r = pScd->r;
        *(MAIN_PROOM(UCHAR) + off + 0x19) = *(MAIN.Scenario.pSce_data + 7);
        *(MAIN_PROOM(UCHAR) + off + 0x1A) = *(MAIN.Scenario.pSce_data + 8);
        pRoom2 = MAIN.pRoom;
        pRoom2 = BYTE_OFS(ROOM_WORK, pRoom2, off);
        pScd2 = SCE_DATA(SCE_LIGHT_PRM_REC);
        pRoom2->Light[0].Mode = pScd2->type;
        pRoom2->Light[0].L = pScd2->radius;
        SCE_DATA_NEXT(pScd2, 0xC);
    }
    for (i = 0; i < 3; i++)
    {
        *(USHORT*)(MAIN_PROOM(UCHAR) + i * 2 + 6) = *SCE_DATA(USHORT);  // USHORT* view of the same memory
        MAIN.Scenario.pSce_data += 2;
    }
    GsSetAmbient(ROOM_NOW->Ambient_r, ROOM_NOW->Ambient_g, ROOM_NOW->Ambient_b);

    return 1;
}

INT Psp_mask_set(VOID)
{
    INT unused[1];
    UINT bits;
    UINT i;
    UINT j;

    bits = Sprite_hide_mask;
    for (i = 0; i < 16; i++)
    {
        if (bits & 1)
        {
            for (j = 0; j < *MAIN_PROOM(UCHAR); j++)
            {
                if (Room_psp[j].id == i)
                {
                    Room_psp[j].be_flg = 0;
                }
            }
        }
        bits >>= 1;
    }
    Sprite_hide_mask = 0;

    return 1;
}

INT Sce_psp_hide(VOID)
{
    USHORT* pScd;
    UINT sprite;

    pScd = SCE_DATA(USHORT);
    sprite = *pScd >> 8;
    MAIN.Scenario.pSce_data = (UCHAR*)(pScd + 1);
    if (sprite == 0xFF)
    {
        Sprite_hide_mask = 0;
    }
    else
    {
        Sprite_hide_mask |= 1 << sprite;
    }
    return 1;
}

INT Sce_Item_chg(VOID)
{
    INT slot;
    USHORT target;
    UCHAR v;
    SEARCH_AOT* pRec;

    target = ((SCE_ITEM_CHG_REC*)(SCE_DATA_NOW))->target;  // reinterprets the memory as SCE_ITEM_CHG_REC
    switch (((SCE_ITEM_CHG_REC*)(SCE_DATA_NOW))->head >> 8)  // reinterprets the memory as SCE_ITEM_CHG_REC
    {
    case 1:
        Sce_aot_tbl[target & 0xFF].pRec->Item_num = * (&MAIN.SaveData.Stage_no + (target >> 8));
        break;
    case 0:
        *(&MAIN.SaveData.Stage_no + (target >> 8)) = Sce_aot_tbl[target & 0xFF].pRec->Item_num;
        break;
    case 2:
        slot = Search_item_id(Sce_aot_tbl[target & 0xFF].pRec->Item_id);
        if (slot >= 0)
        {
            pRec = Sce_aot_tbl[target & 0xFF].pRec;
            v = pItem_work_base[slot].Num;
            *(&MAIN.SaveData.Stage_no + (target >> 8)) = v;
            pRec->Item_num = v;
        }
        break;
    }
    SCE_DATA_CUR += 4;
    return 1;
}

INT Sce_pl_parts_col(VOID)
{
    PARTS_W* pParts;

    pParts = G.Player.pSin_parts_ptr;
    Model_color_plus(pParts, 0x30, 0x80820, 0x606060);
    Model_color_plus(pParts + 1, 0x30, 0x80820, 0x606060);
    Model_color_plus(pParts + 2, 0x30, 0x80820, 0x606060);
    Model_color_plus(pParts + 9, 0x30, 0x80820, 0x606060);
    Model_color_plus(pParts + 12, 0x30, 0x80820, 0x606060);
    Model_color_plus(pParts + 3, 0x30, 0x80820, 0x606060);
    Model_color_plus(pParts + 4, 0x30, 0x80820, 0x606060);
    Model_color_plus(pParts + 5, 0x30, 0x80820, 0x606060);
    Model_color_plus(pParts + 6, 0x30, 0x80820, 0x606060);
    Model_color_plus(pParts + 7, 0x30, 0x80820, 0x606060);
    Model_color_plus(pParts + 8, 0x30, 0x80820, 0x606060);
    Model_color_plus(pParts + 10, 0x30, 0x80820, 0x606060);
    Model_color_plus(pParts + 11, 0x30, 0x80820, 0x606060);
    Model_color_plus(pParts + 13, 0x30, 0x80820, 0x606060);
    SCE_DATA_CUR += 2;
    return 1;
}

// script Opcode handler table, indexed by Opcode
INT (*Sce_jmp_tbl[83])() =
{
    Evt_end,                          // 0 //
    Ifel_ck,                          // 1 //
    Else_ck,                          // 2 //
    Endif,                            // 3 //
    Ck,                               // 4 //
    Set,                              // 5 //
    Ck_byte,        // 6 //
    Cmp,                              // 7 //
    Set_byte,         // 8 //
    Cut_chg,                          // 9 //
    Cut_old,                          // 10 //
    Message_on,                       // 11 //
    Door_aot_set,                     // 12 //
    Aot_set,                          // 13 //
    Nop,                              // 14 //
    Mirror_set,                       // 15 //
    Sce_used_item_ck,         // 16 //
    Sce_picked_item_ck,       // 17 //
    Aot_reset,                        // 18 //
    Aot_arm,        // 19 //
    Evt_exec,                         // 20 //
    Sce_bgm_play,               // 21 //
    Sce_bgm_stop,               // 22 //
    Se_on,                            // 23 //
    Item_aot_set,                     // 24 //
    Obj_model_flg_set,         // 25 //
    Keep_Item_ck,                     // 26 //
    Sce_em_set,                       // 27 //
    Sce_fade_set,                     // 28 //
    Equip_Item_ck,     // 29 //
    Xa_on,                            // 30 //
    Obj_model_set,                    // 31 //
    Sce_pl_pos_set,         // 32 //
    Sce_em_pos_set,          // 33 //
    Item_cnt_ck,        // 34 //
    Cut_auto,                         // 35 //
    Aot_on,                           // 36 //
    Sce_psp_set,        // 37 //
    Nop_26,                    // 38 //
    Sce_snd_fade_set,           // 39 //
    Sce_em_prm_set,         // 40 //
    Movie_on,                         // 41 //
    Sce_espr_on,                      // 42 //
    Sce_atk_anim_set,        // 43 //
    Sce_Item_lost,                    // 44 //
    Sce_Item_get,                     // 45 //
    Nop_2E,                    // 46 //
    Sce_snd_vol_set,        // 47 //
    Sca_id_set,                       // 48 //
    Save,                             // 49 //
    Obj_clut_set,           // 50 //
    Sce_pl_prm_set,        // 51 //
    Col_chg_set,                      // 52 //
    Obj_flg_set,           // 53 //
    Obj_prm_ck,         // 54 //
    Sce_bgmtbl_set,                   // 55 //
    Sce_key_ck,                       // 56 //
    Sce_em_flg_get,        // 57 //
    Cut_replace,                      // 58 //
    Obj_rot_set,       // 59 //
    Sce_pl_dist_ck,       // 60 //
    Sce_espr_on2,                     // 61 //
    Sce_espr_kill,                    // 62 //
    Dir_ck,                           // 63 //
    Sce_light_prm_set,        // 64 //
    Sce_em_posy_set,        // 65 //
    Sce_espr_kill2,                   // 66 //
    Sce_bgm_vol_ramp,        // 67 //
    Evt_kill,                         // 68 //
    Sce_pl_posy_add,        // 69 //
    Sce_room_light_set,        // 70 //
    Obj_trans_set,      // 71 //
    Sce_espr_clr,      // 72 //
    Sce_psp_hide,       // 73 //
    Sce_bgm_restore,            // 74 //
    Sce_bgm_stop_all,           // 75 //
    Sce_Item_chg,   // 76 //
    Sce_pl_parts_col,      // 77 //
    Sce_espr_control,                 // 78 //
    Vib_set0,                         // 79 //
    Vib_set1,                         // 80 //
    Vib_fade_set,                     // 81 //
    Vib_stop,           // 82 //
};
