/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\PAD.C
 */

#include "TYPES.H"
#include "PAD.H"
#include "GAME.H"
#include "MAIN.H"
#include "PLAYER.H"
#include "FLAG.H"
#include "SDK.H"

ULONG GsBACKC; // unused

USHORT Pad_stand_turn_spd; // turn speed per frame while turning on the spot, set by Pad_set
UCHAR Act_mtr_lv[2];                   // vibration motor levels sent to the controller: small motor, large motor
SCHAR Ana_mode;                        // analog stick tilt level, 0 to 10, index into Pad_speed_scale_tbl
UCHAR Padd_data[2][36];                // raw pad buffer of each port: status, terminal id, button bytes and analog bytes
UCHAR Vib_ok_id;                       // vibration setup state of port 0: 0 = not set up, 1 = set up, 2 = set up with the alignment table
UCHAR Pad_device_now;                  // controller type of port 0 (4 = digital pad, 7 = analog pad, 0 = none)
VIB_REQ Vib_stack0[33];                // vibration requests of the small motor
VIB_REQ Vib_stack1[33];                // vibration requests of the large motor

static UCHAR Pad_type_old; // controller type of port 0 on the previous frame

VOID Pad_def_set(VOID)
{
    G.SaveData.Key = 0;
    G.SaveData.Key_tbl[0] = 0x1000;
    G.SaveData.Key_tbl[1] = 0x2000;
    G.SaveData.Key_tbl[2] = 0x4000;
    G.SaveData.Key_tbl[3] = 0x8000;
    G.SaveData.Key_tbl[4] = 0x1000;
    G.SaveData.Key_tbl[5] = 0x4000;
    G.SaveData.Key_tbl[6] = 0x80;
    G.SaveData.Key_tbl[7] = 0x80;
    G.SaveData.Key_tbl[8] = 8;
    G.SaveData.Key_tbl[9] = 0x20;
    G.SaveData.Key_tbl[10] = 8;
    G.SaveData.Key_tbl[11] = 4;
    G.SaveData.Key_tbl[12] = 0x20;
    G.SaveData.Key_tbl[13] = 0x10;
    G.SaveData.Key_tbl[14] = 0x40;
    G.SaveData.Key_tbl[15] = 0x80;
}

VOID Pad_def_set2(VOID)
{
    G.SaveData.Key = 0;
    G.SaveData.Key_tbl[0] = 0x1000;
    G.SaveData.Key_tbl[1] = 0x2000;
    G.SaveData.Key_tbl[2] = 0x4000;
    G.SaveData.Key_tbl[3] = 0x8000;
    G.SaveData.Key_tbl[4] = 0x1000;
    G.SaveData.Key_tbl[5] = 0x4000;
    G.SaveData.Key_tbl[6] = 0x80;
    G.SaveData.Key_tbl[7] = 0x40;
    G.SaveData.Key_tbl[8] = 8;
    G.SaveData.Key_tbl[9] = 0x20;
    G.SaveData.Key_tbl[10] = 8;
    G.SaveData.Key_tbl[11] = 4;
    G.SaveData.Key_tbl[12] = 0x20;
    G.SaveData.Key_tbl[13] = 0x10;
    G.SaveData.Key_tbl[14] = 0x40;
    G.SaveData.Key_tbl[15] = 0x80;
}

VOID Pad_set(VOID)
{
    UINT State;
    UCHAR Vib;
    INT Stk_x;
    UCHAR Stk_y;
    UCHAR Stk_act;
    UCHAR force;
    UCHAR Pow0;
    UCHAR Pow1;
    USHORT Key_old;
    USHORT mask;
    UCHAR Cnt;
    USHORT* pKey_tbl;
    USHORT Padd1;
    USHORT Btn2;
    USHORT cur;
    UINT Padd_trg;
    INT chk;
    UINT* pPadd;

    force = 0;
    if (Padd_data[0][0] == 0xFF)
    {
        Padd_data[0][2] = Padd_data[0][3] = 0xFF;
    }
    if (Padd_data[1][0] == 0xFF)
    {
        Padd_data[1][2] = Padd_data[1][3] = 0xFF;
    }
    State = PadGetState(0);
    if (State >= 2)
    {
        Pad_device_now = PadInfoMode(0, 1, 0);
        Vib = PadInfoMode(0, 2, 0);
        if (Pad_type_old != Pad_device_now)
        {
            Vib_ok_id = 0;
        }
        if (Pad_device_now != 4)
        {
            if (Pad_device_now != 7)
            {
                goto reset;
            }
            Stk_x = Padd_data[0][6];
            Stk_y = Padd_data[0][7];
        }
        if (Vib_ok_id == 0)
        {
            PadSetAct(0, Act_mtr_lv, 2);
            if (State == 2)
            {
                Vib_ok_id = 1;
            }
            else if (State == 6 && PadSetActAlign(0, Pad_act_align) != 0)
            {
                Vib_ok_id = 2;
            }
        }
    }
    else
    {
    reset:
        Padd_data[0][3] = 0xFF;
        Padd_data[0][2] = 0xFF;
        Padd_data[1][3] = 0xFF;
        Padd_data[1][2] = 0xFF;
        Vib_ok_id = 0;
        Pad_device_now = 0;
    }
    Act_mtr_lv[1] = 0;
    Act_mtr_lv[0] = 0;
    Pad_type_old = Pad_device_now;
    if (Vib != 0)
    {
        if ((G.System_flg & SYS_CONFIG) ||
            ((!(G.SaveData.Mess_flg & MESS_FLG_ACTIVE) || (G.System_flg & SYS_DIM)) && !(G.Status_flg & STAT_DEMO_MODE) &&
                !(STATUS_FLG_NOW & STAT_BG_DRAW) && G.Menu_req == 0))
        {
            Act_mtr_lv[0] = SetPadActData(Vib_stack0);
            Act_mtr_lv[1] = SetPadActData(Vib_stack1);
        }
    }
    else
    {
        if ((G.System_flg & SYS_CONFIG) ||
            ((!(G.SaveData.Mess_flg & MESS_FLG_ACTIVE) || (G.System_flg & SYS_DIM)) && !(G.Status_flg & STAT_DEMO_MODE) &&
                !(STATUS_FLG_NOW & STAT_BG_DRAW) && G.Menu_req == 0))
        {
            Pow0 = SetPadActData(Vib_stack0);
            Pow1 = SetPadActData(Vib_stack1);
            if (Pow0 != 0 || Pow1 != 0)
            {
                Act_mtr_lv[0] = 0x40;
                Act_mtr_lv[1] = 0xFF;
            }
        }
    }

    Key_old = 0;

    if (KEY_IDX_CUR & 0x20)
    {
        Act_mtr_lv[1] = 0;
        Act_mtr_lv[0] = 0;
    }

    mask = 0x8000;
    Cnt = 16;

    pKey_tbl = Key_type[MAIN.SaveData.Key_idx & 3];

    Padd1 = G.Padd1;
    do
    {
        Cnt--;
        if (Padd1 & pKey_tbl[Cnt])
        {
            Key_old |= mask;
        }
        mask >>= 1;
    } while (Cnt != 0);

    if (Key_old & 1)
    {
        Key_old &= 0xFFFB;
    }
    if (Key_old & 2)
    {
        Key_old &= 0xFFF7;
    }

    G.Key_old = G.SaveData.Key;
    G.Padd_trg = PADD_CUR;
    if (G.Status_flg & STAT_DEMO_MODE)
    {
        if ((G.Stop_flg & STOP_DEMO_TIMER) && demo_tbl0.Pdemo_cnt != 0)
        {
            MAIN.Padd = demo_tbl0.Padd[demo_tbl0.Pdemo_cnt];
        }
        else
        {
            G.Padd = 0;
        }
    }
    else
    {
        MAIN.Padd = ~(Padd_data[0][3] | ((Padd_data[1][2] << 24) + (Padd_data[1][3] << 16) + (Padd_data[0][2] << 8)));
    }
    if (!(G.Status_flg & STAT_DEMO_MODE))
    {
        if (Pad_device_now == 7)
        {
            Ana_mode = 0;
            Stk_act = 0;
            if (Stk_y < 0x30)
            {
                pPadd = &MAIN.Padd;
                *pPadd |= PAD_UP;
            }
            if (Stk_y > 0xD0)
            {
                pPadd = &MAIN.Padd;
                *pPadd |= PAD_DOWN;
            }
            if ((UCHAR)Stk_x < 0x50)  // truncates to a byte
            {
                if ((UCHAR)Stk_x != 0)  // truncates to a byte
                {
                    Stk_x = 0x50 - Stk_x;
                    Ana_mode = (Stk_x & 0xFF) / 8;
                }
                else
                {
                    Ana_mode = 10;
                }
                Stk_act |= 1;
                pPadd = &MAIN.Padd;
                *pPadd |= PAD_LEFT;
            }
            if ((UCHAR)Stk_x > 0xB0)  // truncates to a byte
            {
                if ((UCHAR)Stk_x < 0xFF)  // truncates to a byte
                {
                    Stk_x += 0x50;
                    Ana_mode = (Stk_x & 0xFF) / 8;
                }
                else
                {
                    Ana_mode = 10;
                }
                Stk_act |= 1;
                pPadd = &MAIN.Padd;
                *pPadd |= PAD_RIGHT;
            }
            if (G.Padd1 & 0x400)
            {
                pPadd = &MAIN.Padd;
                *pPadd |= PAD_START;
            }
        }
    }
    else
    {
        force = 1;
    }
    G.SaveData.Key = 0;

    Padd_trg = (PADD_TRG_CUR ^ *(pPadd = &MAIN.Padd)) & MAIN.Padd;
    cur = *pPadd;

    mask = 0x8000;
    Cnt = 16;

    MAIN.Padd_trg = Padd_trg;
    Btn2 = G.Padd1 = cur;
    G.Padd2 = MAIN.Padd >> 16;
    G.Padd2_trg = Padd_trg >> 16;
    G.Padd1_trg = MAIN.Padd_trg;

    do
    {
        Cnt--;

        if (Btn2 & pKey_tbl[Cnt])
        {
            KEY_CUR |= mask;
        }
        mask >>= 1;
    } while (Cnt != 0);

    chk = G.SaveData.Key & (KEY_RIGHT | KEY_LEFT);
    if (chk != 0)
    {
        force = 1;
    }
    if (!(G.Status_flg & STAT_DEMO_MODE) && Pad_device_now == 7)
    {
        if (Stk_act)
        {
            if ((UCHAR)Stk_x != 0xFF)  // truncates to a byte
            {
                PADD_CUR &= ~PAD_RIGHT;
                PADD_TRG_CUR &= ~PAD_RIGHT;
                PADD1_CUR &= ~PAD_RIGHT;
                PADD1_TRG_CUR &= ~PAD_RIGHT;
            }
            if ((UCHAR)Stk_x != 0)  // truncates to a byte
            {
                PADD_CUR &= ~PAD_LEFT;
                PADD_TRG_CUR &= ~PAD_LEFT;
                PADD1_CUR &= ~PAD_LEFT;
                PADD1_TRG_CUR &= ~PAD_LEFT;
            }
        }
        else if (chk != 0)
        {
            Ana_mode = 10;
        }
    }
    if (force)
    {
        Pad_stand_turn_spd = 0x60;
        Pl_turn_walk = 0x28;
        Pl_turn_run = 0x30;
    }
    else
    {
        Pad_stand_turn_spd = Pad_speed_scale_tbl[Ana_mode] * 0x60 / 100;
        {
            INT scale = Pad_speed_scale_tbl[Ana_mode];
            Pl_turn_walk = scale >> 1;  // signed conversion (unsigned operand)
        }
        Pl_turn_run = Pad_speed_scale_tbl[Ana_mode] * 0x30 / 70;
    }

    if (KEY_CUR & KEY_UP)
    {
        MAIN.SaveData.Key &= ~KEY_DOWN;
    }
    if (MAIN.SaveData.Key & KEY_RIGHT)
    {
        MAIN.SaveData.Key &= ~KEY_LEFT;
    }
    G.SaveData.Key_trg = (MAIN.SaveData.Key & Key_old) ^ MAIN.SaveData.Key;
}

INT Pad_vib_set0(INT duration, INT delay)
{
    VIB_REQ* pReq;

    if (Pad_vib_count(Vib_stack0) < 0x20 && duration != 0)
    {
        Vib_cur0 = Pad_vib_get_free(Vib_stack0);
        Vib_cur0->Flg = 1;
        pReq = Vib_cur0;
        pReq->Pow = duration != 0;
        pReq->Time = duration;
        Vib_cur0->Delay = delay;
        Vib_cur0->Step = 0;
    }
}

INT Pad_vib_set1(INT duration, INT Power, INT delay)
{
    VIB_REQ* pReq;

    if (Pad_vib_count(Vib_stack1) < 0x20U && duration != 0)
    {
        pReq = Pad_vib_get_free(Vib_stack1);
        Vib_cur1 = pReq;
        pReq->Flg = 1;
        Vib_cur1->Time = duration;
        Vib_cur1->Pow = Power;
        Vib_cur1->Delay = delay;
        Vib_cur1->Step = 0;
    }
}

INT Pad_vib_fade_set(INT duration, INT Pow_st, INT Pow_ed, INT delay)
{
    VIB_REQ* pReq;

    if (Pad_vib_count(Vib_stack1) < 0x20U && duration != 0)
    {
        pReq = Pad_vib_get_free(Vib_stack1);
        Vib_cur1 = pReq;
        pReq->Flg = 1;
        Vib_cur1->Time = duration;
        Vib_cur1->Delay = delay;
        Vib_cur1->Pow_cur = Pow_st << 7;
        Vib_cur1->Step = ((Pow_ed - Pow_st) << 7) / duration;
    }
}

VOID Pad_vib_init(VOID)
{
    UINT i;

    for (i = 0; i < 33; i++)
    {
        Vib_stack0[i].Flg = 0;
        Vib_stack1[i].Flg = 0;
    }
    Vib_cur0 = Vib_stack0;
    Vib_cur1 = Vib_stack1;
    Act_mtr_lv[1] = 0;
    Act_mtr_lv[0] = 0;
}

UCHAR SetPadActData(VIB_REQ* pReq)
{
    UCHAR Max = 0;
    UINT i = 0;
    USHORT Sum;

    do
    {
        if (pReq->Flg != 0)
        {
            if (pReq->Delay != 0)
            {
                pReq->Delay--;
            }
            else
            {
                pReq->Time--;
                if (pReq->Time == 0)
                {
                    pReq->Flg = 0;
                }
                if (pReq->Step != 0)
                {
                    Sum = pReq->Pow_cur + pReq->Step;
                    pReq->Pow_cur = Sum;
                    pReq->Pow = Sum >> 7;
                }
                if (Max < pReq->Pow)
                {
                    Max = pReq->Pow;
                }
            }
        }
        i++;
        pReq++;
    } while (i < 32);
    return Max;
}

VIB_REQ* Pad_vib_get_free(VIB_REQ* pReq)
{
    UINT i;

    for (i = 0; i < 32; i++, pReq++)
    {
        if (!pReq->Flg)
        {
            break;
        }
    }
    return pReq;
}

UINT Pad_vib_count(VIB_REQ* pReq)
{
    INT n;
    UINT i;

    n = 0;
    for (i = 0; i < 32; i++, pReq++)
    {
        if (pReq->Flg)
        {
            n++;
        }
    }
    return n;
}

INT Pad_raw_get(INT Pad_no)
{
    if (Padd_data[0][0] != 0)
    {
        return 0;
    }
    return ~(Padd_data[0][3] | (((Padd_data[1][2] << 0x18) + (Padd_data[1][3] << 0x10)) | (Padd_data[0][2] << 8)));
}

VIB_REQ* Vib_cur0 = Vib_stack0; // request slot last queued on the small motor stack
VIB_REQ* Vib_cur1 = Vib_stack1; // request slot last queued on the large motor stack

// turn speed percentage by analog tilt level (entries 0 to 10), then the three default button mapping tables
UCHAR Pad_speed_scale_tbl[0x6C] =
{
    0x00,   // 0 //
    0x10,   // 1 //
    0x1F,   // 2 //
    0x2D,   // 3 //
    0x3A,   // 4 //
    0x48,   // 5 //
    0x52,   // 6 //
    0x5A,   // 7 //
    0x5E,   // 8 //
    0x62,   // 9 //
    0x69,   // 10 //
    0x00,   // 11 //
    0x00,   // 12 //
    0x10,   // 13 //
    0x00,   // 14 //
    0x20,   // 15 //
    0x00,   // 16 //
    0x40,   // 17 //
    0x00,   // 18 //
    0x80,   // 19 //
    0x00,   // 20 //
    0x10,   // 21 //
    0x00,   // 22 //
    0x40,   // 23 //
    0x40,   // 24 //
    0x00,   // 25 //
    0x40,   // 26 //
    0x00,   // 27 //
    0x08,   // 28 //
    0x00,   // 29 //
    0x80,   // 30 //
    0x00,   // 31 //
    0x08,   // 32 //
    0x00,   // 33 //
    0x04,   // 34 //
    0x00,   // 35 //
    0x02,   // 36 //
    0x00,   // 37 //
    0x01,   // 38 //
    0x00,   // 39 //
    0x40,   // 40 //
    0x00,   // 41 //
    0x10,   // 42 //
    0x00,   // 43 //
    0x00,   // 44 //
    0x10,   // 45 //
    0x00,   // 46 //
    0x20,   // 47 //
    0x00,   // 48 //
    0x40,   // 49 //
    0x00,   // 50 //
    0x80,   // 51 //
    0x00,   // 52 //
    0x10,   // 53 //
    0x00,   // 54 //
    0x40,   // 55 //
    0x80,   // 56 //
    0x00,   // 57 //
    0x80,   // 58 //
    0x00,   // 59 //
    0x08,   // 60 //
    0x00,   // 61 //
    0x40,   // 62 //
    0x00,   // 63 //
    0x08,   // 64 //
    0x00,   // 65 //
    0x04,   // 66 //
    0x00,   // 67 //
    0x02,   // 68 //
    0x00,   // 69 //
    0x01,   // 70 //
    0x00,   // 71 //
    0x80,   // 72 //
    0x00,   // 73 //
    0x40,   // 74 //
    0x00,   // 75 //
    0x80,   // 76 //
    0x00,   // 77 //
    0x00,   // 78 //
    0x20,   // 79 //
    0x10,   // 80 //
    0x00,   // 81 //
    0x00,   // 82 //
    0x80,   // 83 //
    0x00,   // 84 //
    0x10,   // 85 //
    0x00,   // 86 //
    0x40,   // 87 //
    0x20,   // 88 //
    0x00,   // 89 //
    0x20,   // 90 //
    0x00,   // 91 //
    0x08,   // 92 //
    0x00,   // 93 //
    0x40,   // 94 //
    0x00,   // 95 //
    0x08,   // 96 //
    0x00,   // 97 //
    0x04,   // 98 //
    0x00,   // 99 //
    0x02,   // 100 //
    0x00,   // 101 //
    0x01,   // 102 //
    0x00,   // 103 //
    0x20,   // 104 //
    0x00,   // 105 //
    0x40,   // 106 //
    0x00,   // 107 //
};

// button mapping tables: three defaults inside Pad_speed_scale_tbl, then the custom bindings of the key config
USHORT* Key_type[4] =
{
    (USHORT*)(Pad_speed_scale_tbl + 0xC),    // 0 //
    (USHORT*)(Pad_speed_scale_tbl + 0x2C),   // 1 //
    (USHORT*)(Pad_speed_scale_tbl + 0x4C),   // 2 //
    G.SaveData.Key_tbl,     // 3 //
};
