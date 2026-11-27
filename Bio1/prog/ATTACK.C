/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ATTACK.C
 */

#include "ATTACK.H"
#include <RAND.H>
#include "SCA.H"
#include "EFF1.H"
#include "EM_SUB.H"
#include "MAIN.H"
#include "PAD.H"
#include "FLAG.H"
#include "SCE1.H"
#include "SCE_AT.H"

#define PL_CDIR_Y_S ((SHORT)MAIN.Player.Cdir_y)  // Cdir_y of the player: the same-type cast is needed, the plain read changes the code

INT Pos_pl_at_ck(INT radius, SHORT x, SHORT z)
{
    ENEMY_WORK* pPl = PL_EM;
    ENEMY_WORK* pEm0 = MAIN.Enemy;
    ENEMY_WORK* pHit;
    INT r;
    SHORT hp;
    UCHAR v;

    ScratchWord[0] = radius;
    pHit = Pl_dist_ck(x, z, (VOID*)&MAIN.Player.Be_flg) ? pPl : 0;  // pointer type conversion (no code)
    if (pHit != 0 && Em_line_ck(POS_VEC(pHit)) == 0)
    {
        if (pHit->Damage_no != 0)
        {
            return 1;
        }
        if ((UCHAR)(G.Enemy[0].Id - 3) < 2)  // real width or sign conversion
        {
            r = rand();
            if ((r & 1) * (rand() & 1))
            {
                MAIN.Player.Poison_timer = 150;
                MAIN.Player.Sca_info |= 2;
            }
        }

        hp = pHit->Life;
        pHit->Life = hp - 5;

        if (pEm0->Id == 3)
        {
            pHit->Life = hp - 10;
        }
        if (pEm0->Id == 4)
        {
            pHit->Life = pHit->Life - 7;
        }
        pHit->Damage_no = 1;
        ROUTINE_HALF0(pHit) = 2;  // SHORT view of the field (plain access changes the code)
        ROUTINE_HALF2(pHit) = 100;  // SHORT view of the field (plain access changes the code)
        if (pHit->Life < 0)
        {
            ROUTINE_WORD(pHit) = 3;  // INT view of the field (plain access changes the code)
        }
        Pad_vib_set0(2, 0);
        Pad_vib_set1(7, 200, 0);
        return 1;
    }

    if (G.Enemy[1].Id != 0x13)
    {
        return 0;
    }
    v = G.SaveData.Stage_no;
    if (v != 2)
    {
        return 0;
    }
    if (G.SaveData.Room_no != 0xC)
    {
        return 0;
    }
    if (Em_dist_ck(x, z, (VOID*)&MAIN.Enemy[1].Be_flg) == 0)  // pointer type conversion (no code)
    {
        return 0;
    }
    if (G.Enemy[1].Damage_no != 0)
    {
        return 1;
    }
    G.Enemy[1].Damage_no = 9;
    ENEMY_ROUTINE_WORD(1) = 3;
    G.Enemy[1].Life -= 5;
    if (G.Enemy[1].Life >= 0)
    {
        ENEMY_ROUTINE_WORD(1) = v;
    }
    return 1;
}

INT Pl_dist_ck(SHORT x, SHORT z, ENEMY_WORK* pEm)
{
    INT dx;
    INT dz;

    dx = pEm->Pos_x - x;
    dz = pEm->Pos_z - z;
    return NOPROTO(LONG, SquareRoot0)(dx * dx + dz * dz, z) < ScratchWord[0];  // unprototyped call (prototype changes the code)
}

INT Em_dist_ck(SHORT x, SHORT z, ENEMY_WORK* pEm)
{
    INT dx;
    INT dz;

    dx = pEm->Pos_x - x;
    dz = pEm->Pos_z - z;
    return NOPROTO(LONG, SquareRoot0)(dx * dx + dz * dz, z) < ScratchWord[0] + 0x1F4;  // unprototyped call (prototype changes the code)
}

UINT Em_ck_hani(UINT range)
{
    INT dx;
    INT dz;
    UINT dist;
    ENEMY_WORK* pEm;
    ENEMY_WORK* pEm2;

    pEm = EM_CUR;
    dx = MAIN.Player.Pos_x - pEm->Pos_x;
    dz = MAIN.Player.Pos_z - pEm->Pos_z;
    dist = SquareRoot0(dx * dx + dz * dz);
    if (dist < range)
    {
        pEm2 = MAIN.pEm;
        pEm2->Be_flg |= 0x20;
    }
    return dist;
}

UINT Em_ck_alert(UINT range)
{
    INT dx;
    INT dz;
    UINT dist;
    ENEMY_WORK* pEm;
    ENEMY_WORK* pEm2;

    pEm = EM_CUR;
    dx = MAIN.Player.Pos_x - pEm->Pos_x;
    dz = MAIN.Player.Pos_z - pEm->Pos_z;
    dist = SquareRoot0(dx * dx + dz * dz);
    if (dist < range)
    {
        pEm2 = MAIN.pEm;
        pEm2->Be_flg |= 0x80;
    }
    return dist;
}

INT Weapon_at_ck(INT Type)
{
    UCHAR buf[32];
    ENEMY_WORK* pEm;
    ENEMY_WORK* pHit;
    INT pRet;
    UCHAR n;
    UCHAR type;
    UCHAR r;
    INT k;

    pRet = ENEMY_NUM_CUR;
    pHit = 0;
    if (pRet != 0)
    {
        pEm = MAIN.Enemy;
        n = MAIN.Enemy_num;
        ScratchWord[7] = 0;
        ScratchWord[6] = n;
        do
        {
            if (pEm->Be_flg != 0)
            {
                buf[--ScratchWord[6]] = ScratchWord[7];
            }
            if (ScratchWord[6] == 0)
            {
                break;
            }
            pEm++;
        } while (++ScratchWord[7] < 30);
        pEm = &MAIN.Enemy[buf[0]];
        ScratchWord[0] = 0x7FFFFFFF;
        ScratchWord[6] = Type;
        type = ScratchWord[6] - 1;
        ScratchWord[2] = type;
        Type =Wep_range_tbl[MAIN.Player.Id & 1][type];

        if ((G.Status_flg & STAT_ARRANGE) && type == 1 && Wep_crit_tbl[rand() & 0xF] != 0)
        {
            type = 3;
            ScratchWord[2] = 3;
            ScratchWord[6] = 4;
        }
        while (n-- != 0)
        {
            if ((pEm->Be_flg & MAIN.Player.Be_flg & EM_BE_ATTACK_MASK) && pEm->Damage_no == 0 && Wep_hit_ck_tbl[type](Type, pEm) != 0)
            {
                pHit = pEm;
            }
            pEm = &MAIN.Enemy[buf[n]];
        }
        if (pHit == 0 || (Em_line_ck(POS_VEC(pHit)) != 0 && type < 5))
        {
            pRet = 0;
        }
        else
        {
            pRet = pHit->Id < 20;
            if (pRet)
            {
                r = pHit->Id;
                ScratchWord[1] = r;
                ScratchWord[2] = type;
                ScratchPad->Vec.vx = Wep_at_tbl[0][r][type].Eff_x;
                ScratchPad->Vec.vy = Wep_at_tbl[0][r][type].Eff_y;
                ScratchPad->Vec.vz = Wep_at_tbl[0][r][type].Eff_z;
                ScratchWord[3] = Wep_at_tbl[0][r][type].fx_type;
                {
                    INT t9 = Wep_at_tbl[0][r][type].Esp_dt;
                    ScratchWord[5] = pHit->Life;
                    ScratchWord[4] = t9;
                }
                pHit->Damage_no = Wep_at_tbl[0][r][type].hit_state;
                if (G.Status_flg & STAT_EASY_MODE)
                {
                    pHit->Life -= Wep_at_tbl[1][r][type].Damage;
                }
                else if ((G.Status_flg & STAT_ARRANGE) && !(STATUS_FLG_NOW & STAT_ARRANGE_EASY))
                {
                    pHit->Life -= Wep_at_tbl[2][r][type].Damage;
                }
                else
                {
                    pHit->Life -= Wep_at_tbl[0][r][type].Damage;
                }
                k = MAIN.Player.Be_flg >> 5;
                if (k != 1)
                {
                    pHit->Damage_no += k;
                }
                pHit->Damage_no |= ScratchWord[6] << 3;
                Wep_set_tbl[type](pHit);
                ROUTINE_WORD(pHit) = 3;  // INT view of the field (plain access changes the code)
                if (pHit->Life >= 0)
                {
                    ROUTINE_WORD(pHit) = 2;  // INT view of the field (plain access changes the code)
                }
                pRet = 1;
            }
        }
    }
    return pRet;
}

VOID Wep_set_knife(ENEMY_WORK* pEm)
{
    SHORT t[2][3] =   // per character: effect offset from the player's joint 14
    {
        { 153, 0, 0 },   // 0 //
        { -380, 0, 0 }   // 1 //
    };
    PARTS_W* pP_ptr;
    INT n;
    SHORT* v;

    pP_ptr = MAIN.Player.pSin_parts_ptr;
    Snd_se_stad(1, 1, 0, &MAIN.Player.Pos_x);  // address as integer
    Em_dm_rtn_tbl[ScratchWord[1]](pEm);
    n = ScratchWord[3];
    if (n != 1)
    {
        v = t[MAIN.Player.Id & 1];
        ScratchVec[0] = v[0];
        ScratchPad->Vec.vy = v[1];
        ScratchPad->Vec.vz = v[2];
        Esp_call(n, ScratchWord[4], 0, &pP_ptr[14].Workm, &ScratchPad->Vec, 0);
    }
}

VOID Wep_set_rtn(ENEMY_WORK* pEm)
{
    Em_dm_rtn_tbl[ScratchWord[1]](pEm);
}

VOID Wep_set_shotgun(ENEMY_WORK* pEm)
{
    if (ScratchWord[2] == 2 && ScratchWord[0] >= 0x2329)
    {
        pEm->Damage_no -= 1;
        pEm->Life += 0xA;
    }
    Em_dm_rtn_tbl[ScratchWord[1]](pEm);
}

VOID Wep_set_blood(ENEMY_WORK* pEm)
{
    PARTS_W* pP_ptr;
    VOID* pos;
    INT v;

    pP_ptr = pEm->pSin_parts_ptr;
    if (ScratchWord[3] == 1)
    {
        return;
    }
    if (Wep_at_tbl[0][ScratchWord[1]][ScratchWord[2]].Eff_x == 0x96 && (MAIN.Player.Be_flg & 0x20))
    {
        ScratchPad->Vec.vy += 0x3E8;
        if (pP_ptr[1].Workm.t[1] >= -0x3E7)
        {
            ScratchPad->Vec.vy = -0x12C;
        }
    }
    pos = &pEm->M;
    Esp_call(0xE, ScratchWord[4], PL_CDIR_Y_S - pEm->Cdir_y + 0x800, pos, &ScratchPad->Vec, 0);  // real width or sign conversion
    Esp_call(9, 0xD, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, pos, &ScratchPad->Vec, 0);  // real width or sign conversion
    v = ScratchWord[1];
    if (v != 7 && v != 0xA && v != 8)
    {
        ScratchPad->Vec.vy -= 0x1F4;
        Esp_call(0xE, 3, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, pos, &ScratchPad->Vec, 0);  // real width or sign conversion
        Esp_call(9, 0xD, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, pos, &ScratchPad->Vec, 0);  // real width or sign conversion
    }
    if (pEm->Life >= 0 || ScratchWord[3] != 0xE)
    {
        return;
    }
    v = ScratchWord[1];
    if (v != 5 && v != 7 && v != 0xA)
    {
        ScratchPad->Vec.vy = 0;
        ScratchVec[0] = -0x64;
        ScratchPad->Vec.vz = -0x12C;
        Esp_call(0xE, 6, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
        Esp_call(9, 0xD, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
        ScratchPad->Vec.vx = 0x64;
        ScratchPad->Vec.vz = 0x12C;
        Esp_call(0xE, 6, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
        Esp_call(9, 0xD, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
        if (Wep_at_tbl[0][ScratchWord[1]][ScratchWord[2]].Eff_x == 0x96)
        {
            ScratchPad->Vec.vy = -0x3E8;
            ScratchPad->Vec.vx = -0x64;
            ScratchPad->Vec.vz = -0x12C;
            Esp_call(0xE, 6, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
            Esp_call(9, 0xD, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
            ScratchPad->Vec.vx = 0x64;
            ScratchPad->Vec.vz = 0x12C;
            Esp_call(0xE, 6, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
            Esp_call(9, 0xD, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
        }
    }
    ScratchWord[7] = pEm->Parts_num;
    MAIN.pEm = pEm;
    while (--ScratchWord[7] != -1)
    {
        Model_color_plus(&pP_ptr[ScratchWord[7]], 0x202020, 0x101010, 0x303030);
        if (ScratchWord[1] == 0)
        {
            Model_color_plus(&pP_ptr[ScratchWord[7]], 0x202020, 0x101010, 0xA0A0A);
        }
    }
    MAIN.pEm = pEm;
}

VOID Wep_set_blood2(ENEMY_WORK* pEm)
{
    INT Joint_flg;
    INT i;
    INT i2;
    INT i3;
    INT i4;
    INT i5;
    VOID* pos;
    PARTS_W* pP_ptr;
    VOID* pEm_bak;

    pP_ptr = pEm->pSin_parts_ptr;
    if (ScratchWord[3] != 1)
    {
        if (Wep_at_tbl[0][ScratchWord[1]][ScratchWord[2]].Eff_x == 0x96 && (MAIN.Player.Be_flg & 0x20))
        {
            ScratchPad->Vec.vy += 0x3E8;
            if (pP_ptr[1].Workm.t[1] >= -0x3E7)
            {
                ScratchPad->Vec.vy = -0x12C;
            }
        }
        pos = &pEm->M;
        Esp_call(0xE, 7, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, pos, &ScratchPad->Vec, 0);  // real width or sign conversion
        Esp_call(9, 0xD, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, pos, &ScratchPad->Vec, 0);  // real width or sign conversion
        Esp_call(9, 0xD, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, pos, &ScratchPad->Vec, 0);  // real width or sign conversion
        if (ScratchWord[1] != 8)
        {
            ScratchWord[7] = 2;
            ScratchPad->Vec.vy -= 0xC8;
            do
            {
                Esp_call(9, 0xD, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
                i = ScratchWord[7] - 1;
                ScratchWord[7] = i;
            } while (i != -1);
            if (pEm->Life < 0 && ScratchWord[3] == 0 && Em_kan_tbl[ScratchWord[1]][0] != 0)
            {
                ScratchPad->Vec.vy = -0x1F4;
                ScratchWord[7] = 2;
                do
                {
                    Esp_call(9, 0xD, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
                    i2 = ScratchWord[7] - 1;
                    ScratchWord[7] = i2;
                } while (i2 != -1);
                Joint_flg = (Wep_at_tbl[0][ScratchWord[1]][ScratchWord[2]].Damage & 1) * (pEm->Type & 2);
                pEm_bak = EM_CUR;
                MAIN.pEm = pEm;
                ScratchWord[5] = Joint_flg;
                if (MAIN.Player.Be_flg & 0x20)
                {
                    if (Joint_flg == 0)
                    {
                        i3 = pEm->Parts_num - 1;
                        ScratchWord[7] = i3;
                        if (i3 >= pEm->Parts_num - 5)
                        {
                            do
                            {
                                Joint_atck_set(&pP_ptr[ScratchWord[7]], 0x1E, 2, 3);
                                i4 = ScratchWord[7] - 1;
                                ScratchWord[7] = i4;
                            } while (i4 >= pEm->Parts_num - 5);
                        }
                    }
                    else
                    {
                        goto joint_list;
                    }
                }
                else
                {
                joint_list:
                    ScratchWord[7] = 5;
                    do
                    {
                        Joint_atck_set(&pP_ptr[Em_kan_tbl[ScratchWord[1]][ScratchWord[7]]], 0x1E, 2, 3);
                        i5 = ScratchWord[7] - 1;
                        ScratchWord[7] = i5;
                    } while (i5 != -1);
                }
                MAIN.pEm = pEm_bak;
            }
        }
    }
}

VOID Wep_set_sparks(ENEMY_WORK* pEm)
{
    INT i;
    INT id;
    INT Rsv2;
    INT Rsv3;
    UCHAR unused1;
    VOID* pos;
    PARTS_W* pP_ptr;

    pP_ptr = pEm->pSin_parts_ptr;
    if (ScratchWord[3] != 1)
    {
        if (Wep_at_tbl[0][ScratchWord[1]][ScratchWord[2]].Eff_x == 0x96)
        {
            if (MAIN.Player.Be_flg & 0x20)
            {
                ScratchPad->Vec.vy += 0x3E8;
                if (pP_ptr[1].Workm.t[1] >= -0x3E7)
                {
                    ScratchPad->Vec.vy = -0x12C;
                }
            }
        }
        ScratchWord[7] = 2;
        do
        {
            Esp_call(9, 0, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
            i = ScratchWord[7] - 1;
            ScratchWord[7] = i;
        } while (i != -1);
        if (pEm->Life < 0 && ScratchWord[3] == 9)
        {
            id = ScratchWord[1];
            if (id != 5 && id != 7 && id != 0xA && id != 8)
            {
                ScratchPad->Vec.vy = 0;
                ScratchVec[0] = -0x64;
                pos = &pEm->M;
                ScratchPad->Vec.vz = -0x12C;
                Esp_call(9, 0, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, pos, &ScratchPad->Vec, 0);  // real width or sign conversion
                Esp_call(9, 1, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, pos, &ScratchPad->Vec, 0);  // real width or sign conversion
                ScratchPad->Vec.vx = 0x64;
                ScratchPad->Vec.vz = 0x12C;
                Esp_call(9, 0, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, pos, &ScratchPad->Vec, 0);  // real width or sign conversion
                Esp_call(9, 1, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, pos, &ScratchPad->Vec, 0);  // real width or sign conversion
            }
            ScratchWord[7] = pEm->Parts_num;
            MAIN.pEm = pEm;
            while (--ScratchWord[7] != -1)
            {
                Model_color_plus(&pP_ptr[ScratchWord[7]], 0x4040, 0x1010, 0x3030);
                if (ScratchWord[1] == 0)
                {
                    Model_color_plus(&pP_ptr[ScratchWord[7]], 0x2020, 0x1010, 0xA0A);
                }
            }
            MAIN.pEm = pEm;
        }
    }
}

VOID Wep_set_blood3(ENEMY_WORK* pEm)
{
    PARTS_W* pP_ptr;
    VOID* pos;
    INT v;

    pP_ptr = pEm->pSin_parts_ptr;
    if (ScratchWord[3] == 1)
    {
        return;
    }
    if (Wep_at_tbl[0][ScratchWord[1]][ScratchWord[2]].Eff_x == 0x96 && (MAIN.Player.Be_flg & 0x20))
    {
        ScratchPad->Vec.vy += 0x3E8;
        if (pP_ptr[1].Workm.t[1] >= -0x3E7)
        {
            ScratchPad->Vec.vy = -0x12C;
        }
    }
    pos = &pEm->M;
    Esp_call(0xE, ScratchWord[4], PL_CDIR_Y_S - pEm->Cdir_y + 0x800, pos, &ScratchPad->Vec, 0);  // real width or sign conversion
    Esp_call(9, 0xD, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, pos, &ScratchPad->Vec, 0);  // real width or sign conversion
    v = ScratchWord[1];
    if (v != 7 && v != 0xA && v != 8)
    {
        ScratchPad->Vec.vy -= 0x1F4;
        Esp_call(0xE, 3, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, pos, &ScratchPad->Vec, 0);  // real width or sign conversion
        Esp_call(9, 0xD, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, pos, &ScratchPad->Vec, 0);  // real width or sign conversion
    }
    if (pEm->Life < 0 && ScratchWord[3] != 2)
    {
        v = ScratchWord[1];
        if (v != 5 && v != 7 && v != 0xA)
        {
            ScratchPad->Vec.vy = 0;
            ScratchVec[0] = -0x64;
            ScratchPad->Vec.vz = -0x12C;
            Esp_call(0xE, 6, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
            Esp_call(9, 0xD, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
            ScratchPad->Vec.vx = 0x64;
            ScratchPad->Vec.vz = 0x12C;
            Esp_call(0xE, 6, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
            Esp_call(9, 0xD, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
            if (Wep_at_tbl[0][ScratchWord[1]][ScratchWord[2]].Eff_x == 0x96)
            {
                ScratchPad->Vec.vy = -0x3E8;
                ScratchPad->Vec.vx = -0x64;
                ScratchPad->Vec.vz = -0x12C;
                Esp_call(0xE, 6, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
                Esp_call(9, 0xD, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
                ScratchPad->Vec.vx = 0x64;
                ScratchPad->Vec.vz = 0x12C;
                Esp_call(0xE, 6, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
                Esp_call(9, 0xD, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
            }
        }
        ScratchWord[7] = pEm->Parts_num;
        MAIN.pEm = pEm;
        while (--ScratchWord[7] != -1)
        {
            Model_color_plus(&pP_ptr[ScratchWord[7]], 0x202020, 0x101010, 0x303030);
            if (ScratchWord[1] == 0)
            {
                Model_color_plus(&pP_ptr[ScratchWord[7]], 0x202020, 0x101010, 0xA0A0A);
            }
        }
        MAIN.pEm = pEm;
    }
    Wep_set_blood2(pEm);
}

VOID Em_dm_zombie(ENEMY_WORK* pEm)
{
    VOID* old;
    PARTS_W* pP_ptr;

    USHORT t;
    UCHAR n;

    if (ScratchWord[1] != 2)
    {
        if ((ScratchWord[2] == 2 && ScratchWord[0] < 3000 && (MAIN.Player.Be_flg >> 6) != 0) || ((UINT)(ScratchWord[2] - 3) < 2 && (MAIN.Player.Be_flg & 0x40)))  // real width or sign conversion
        {
            if (!(G.Status_flg & STAT_ARRANGE) || MAIN.SaveData.Stage_no != 6 || G.SaveData.Room_no != 0x1C)
            {
                pEm->Life = -300;
                old = EM_CUR;
                MAIN.pEm = pEm;
                Flg_on(&MAIN.SaveData.Enemy_flg, pEm->Em_set_flg);  // address as integer
                pP_ptr = pEm->pSin_parts_ptr;
                Joint_atck_set(&pP_ptr[2], 0x1E, 2, 3);
                Snd_se_enem(6, 0);
                MAIN.pEm = old;
                ScratchVec[0] = 100;
                ScratchPad->Vec.vy = -600;
                ScratchPad->Vec.vz = 0;
                Esp_call(0, 3, 0, &pP_ptr->Workm, &ScratchPad->Vec, 0);
                ScratchPad->Vec.vx = Wep_at_tbl[0][ScratchWord[1]][ScratchWord[2]].Eff_x;
                ScratchPad->Vec.vy = Wep_at_tbl[0][ScratchWord[1]][ScratchWord[2]].Eff_y;
                ScratchPad->Vec.vz = Wep_at_tbl[0][ScratchWord[1]][ScratchWord[2]].Eff_z;
                ScratchPad->Vec.vy -= 120;
                Esp_call(3, 0, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
                ScratchPad->Vec.vy += 120;
            }
        }
        if (MAIN.Player.Be_flg & 0x20)
        {
            ScratchPad->Vec.vy = -600;
            if (pEm->Type & 2)
            {
                ScratchPad->Vec.vy = -500;
            }
        }
    }
    if (G.Status_flg & STAT_DEMO_MODE)
    {
        pEm->Life++;
    }
    if ((MAIN.Player.Id & 1) && ScratchWord[2] == 1)
    {
        t = pEm->Life;
        pEm->Life = t - 3;
        if (ScratchWord[1] == 2)
        {
            pEm->Life = t - 7;
        }
    }
    if (ScratchWord[3] == 4)
    {
        if (ScratchWord[0] < 9000)
        {
            n = 4;
        loop:
            Esp_call(ScratchWord[3], n,
                PL_CDIR_Y_S - pEm->Cdir_y + 0x500 + (n << 8),  // real width or sign conversion
                MATRIX_OF(pEm), &ScratchPad->Vec, 0);
            if (n--)
            {
                goto loop;
            }
        }
        ScratchWord[3] = 0;
    }
    Esp_call(ScratchWord[3], ScratchWord[4], PL_CDIR_Y_S - pEm->Cdir_y + 0x800,  // real width or sign conversion
        MATRIX_OF(pEm), &ScratchPad->Vec, 0);
}

VOID Em_dm_basic(ENEMY_WORK* pEm)
{
    Esp_call(ScratchWord[3], ScratchWord[4], PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
}

VOID Em_dm_head(ENEMY_WORK* pEm)
{
    PARTS_W* pP_ptr;

    pP_ptr = pEm->pSin_parts_ptr;
    if (MAIN.Player.Be_flg & 0x20)
    {
        ScratchPad->Vec.vy += 0x3E8;
        if (pP_ptr[1].Workm.t[1] >= -0x31F)
        {
            ScratchPad->Vec.vy = -0x12C;
        }
    }

    Esp_call(3, 8, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion

    Esp_call(ScratchWord[3], ScratchWord[4], PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
}

VOID Em_dm_blood(ENEMY_WORK* pEm)
{
    if (ScratchWord[2] != 0)
    {
        if (ScratchWord[0] >= 0x1B59 && (rand() & 1))
        {
            pEm->Life = ScratchWord[5];
        }
        Esp_call(0, 0x18, PL_CDIR_Y_S - pEm->Cdir_y + 0x800, MATRIX_OF(pEm), &ScratchPad->Vec, 0);  // real width or sign conversion
    }
}

VOID Em_dm_none(ENEMY_WORK* pEm)
{
}

INT Wep_hit_ck_gun(INT range, ENEMY_WORK* pEm)
{
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    SVECTOR d;
    USHORT h;
    INT nh;
    INT pRet;

    h = pEm->pAt_tbl->At_w;

    if (pEm->Id < 2 || ((SCHAR)pEm->Id & 0xFF) == 0x11)  // real width or sign conversion
    {
        if ((MAIN.Player.Be_flg & 0x80) && ScratchWord[2] != 2)
        {
            return 0;
        }
    }

    if (((SCHAR)pEm->Id & 0xFF) == 4)  // real width or sign conversion
    {
        range -= 1000;
    }

    if (((SCHAR)pEm->Id & 0xFF) == 8)  // real width or sign conversion
    {
        range += 2000;
    }
    ScratchSv[0] = 50;
    if (MAIN.Player.Be_flg & 0x20)
    {
        ScratchPad->Sv.vx = 0;
    }
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = h + 200;
    Rot_vector(PL_CDIR_Y_S, &ScratchPad->Sv, &a);  // real width or sign conversion
    nh = -h;
    ScratchPad->Sv.vz = nh - 200;
    Rot_vector(PL_CDIR_Y_S, &ScratchPad->Sv, &b);  // real width or sign conversion
    ScratchPad->Sv.vx = 0x28A;
    ScratchPad->Sv.vz = h + range;
    Rot_vector(PL_CDIR_Y_S, &ScratchPad->Sv, &c);  // real width or sign conversion
    ScratchPad->Sv.vz = nh - range;
    Rot_vector(PL_CDIR_Y_S, &ScratchPad->Sv, &d);  // real width or sign conversion
    ScratchPad->Sv.vz = 0;
    Rot_vector(PL_CDIR_Y_S, &ScratchPad->Sv, &ScratchPad->Sv);  // real width or sign conversion
    if (Wep_hit_ck_cone(&a, &b, pEm) != 0)
    {
        pRet = 1;
    }
    else
    {
        pRet = Wep_hit_ck_cone(&c, &d, pEm) != 0;
    }
    return pRet;
}

INT Wep_hit_ck_cone(SVECTOR* pSide_a, SVECTOR* pSide_b, ENEMY_WORK* pEm)
{
    SHORT* pos = &ScratchPad->Sv.vx;
    INT dx;
    INT dz;
    UINT dist;

    dx = pEm->Pos_x - MAIN.Player.Pos_x;
    dz = pEm->Pos_z - MAIN.Player.Pos_z;
    if (Cross2d(*pos, pos[2], dx - pSide_a->vx, dz - pSide_a->vz) > 0)
    {
        return 0;
    }
    if (Cross2d(*pos, pos[2], dx - pSide_b->vx, dz - pSide_b->vz) < 0)
    {
        return 0;
    }
    if (Cross2d(pSide_a->vx - pSide_b->vx, pSide_a->vz - pSide_b->vz, dx - *pos, dz - pos[2]) > 0)
    {
        return 0;
    }
    dist = SquareRoot0(dx * dx + dz * dz);
    if (dist < ScratchUword[0])
    {
        ScratchUword[0] = dist;
        return 1;
    }
    return 0;
}

INT Wep_hit_ck_knife(INT range, ENEMY_WORK* pEm)
{
    SHORT t[2][3] =   // per character: offset from the player's joint 14
    {
        { 153, 0, 0 },   // 0 //
        { -380, 0, 0 }   // 1 //
    };
    MATRIX m;
    SHORT* v;
    INT dx;
    INT dz;
    UINT d;
    UCHAR kind;
    UCHAR* flags;

    ScratchPad->Mat = GsIDMATRIX;
    flags = &MAIN.Player.Be_flg;
    v = t[MAIN.Player.Id & 1];
    ScratchPad->Mat.t[0] = v[0];
    ScratchPad->Mat.t[1] = v[1];
    ScratchPad->Mat.t[2] = v[2];
    GsMulCoord0(&MAIN.Player.pSin_parts_ptr[14].Workm, &ScratchPad->Mat, &m);
    dx = pEm->Pos_x - m.t[0];
    ScratchWord[7] = pEm->pAt_tbl->At_w + range;
    dz = pEm->Pos_z - m.t[2];
    kind = pEm->Id;
    if (kind < 2 || pEm->Id == 0x11)
    {
        if (*flags & 0x80)
        {
            return 0;
        }
    }
    if ((pEm->Id == 2 && pEm->Pos_y >= -0x18F) || pEm->Id == 3)
    {
        if (MAIN.Player.Be_flg & 0x40)
        {
            return 0;
        }
    }
    kind = pEm->Id;
    if (kind == 5 || kind == 7 || kind == 9)
    {
        if (pEm->Pos_y < -0x12C0)
        {
            return 0;
        }
    }
    if (pEm->Id == 4)
    {
        ScratchWord[7] -= 0x320;
    }
    if (pEm->Id == 8)
    {
        ScratchWord[7] += 0x7D0;
    }
    if (pEm->Id == 7 || pEm->Id == 10)
    {
        ScratchWord[7] += 0x64;
    }
    d = SquareRoot0(dx * dx + dz * dz);
    if (d < ScratchUword[7] && d < ScratchUword[0])
    {
        ScratchUword[0] = d;
        return 1;
    }
    return 0;
}

INT Wep_hit_ck_tama(INT r, ENEMY_WORK* pEm)
{
    INT dx;
    INT dz;
    UINT d;
    UINT lim;

    dx = pEm->Pos_x - ScratchVec[0];
    lim = pEm->pAt_tbl->At_w + r;
    dz = pEm->Pos_z - ScratchVec[2];
    ScratchUword[7] = lim;
    if (pEm->Id == 4)
    {
        ScratchUword[7] = lim - 1000;
    }
    d = SquareRoot0(dx * dx + dz * dz);
    if (d < ScratchUword[7] && d < ScratchUword[0])
    {
        ScratchUword[0] = d;
        return 1;
    }
    return 0;
}

VOID Sca_hit_print(VOID)
{
    INT count;
    UINT* tbl;
    SCA_BOX_REC* p;
    SCA_BOX_REC* end;
    UINT k;
    SHORT hx;
    SHORT hz;
    INT pz;
    INT px;
    USHORT A;
    USHORT B;
    USHORT C;
    USHORT D;
    INT A2;
    INT B2;
    INT C2;
    INT D2;
    INT r;
    UINT quad;

    ScratchPad->Sv.vz = 0;
    ScratchPad->Sv.vx = 0x32;
    Rot_vector(PL_CDIR_Y_S, &ScratchPad->Sv, &ScratchPad->Sv);  // real width or sign conversion
    count = 0;
    k = 3;
    hx = MAIN.Player.Pos_x >> 3;
    hz = MAIN.Player.Pos_z >> 3;
    pz = hz;
    px = hx;
    do
    {
        tbl = (UINT*)(k * 4 + (UCHAR*)G.pRoom->pSca);
        p = (SCA_BOX_REC*)tbl[0];  // pointer type conversion (no code)
        end = (SCA_BOX_REC*)tbl[1];  // pointer type conversion (no code)
        if (p < end)
        {
            do
            {
                if (p->Type & 0x100)
                {
                    C = p->Min_x >> 3;
                    D = p->Min_z >> 3;
                    A = p->Max_x >> 3;
                    B = p->Max_z >> 3;
                    D2 = D;
                    C2 = C;
                    quad = (pz < (B + D2) >> 1) << 1;
                    quad |= px < (A + C2) >> 1;
                    B2 = B;
                    A2 = A;
                    switch (quad)
                    {
                    case 0:
                        if (Cross2d(ScratchPad->Sv.vx, ScratchPad->Sv.vz, A2 - px, B2 - pz) < 0)
                        {
                            FntPrint("right ");
                            r = Line_get_z(A - hx) + pz;
                            FntPrint("%d %d\n", count, r);
                            if (D2 < r && r < B2 && A2 < px)
                            {
                                FntPrint("%d Atari!\n", count);
                            }
                        }
                        else
                        {
                            FntPrint("left ");
                            r = Line_get_x(B - hz) + px;
                            FntPrint("%d %d\n", count, r);
                            if (C2 < r && r < A2 && B2 < pz)
                            {
                                FntPrint("%d Atari!\n", count);
                            }
                        }
                        break;
                    case 1:
                        if (Cross2d(ScratchPad->Sv.vx, ScratchPad->Sv.vz, C2 - px, B2 - pz) < 0)
                        {
                            FntPrint("right ");
                            r = Line_get_x(B - hz) + px;
                            FntPrint("%d %d\n", count, r);
                            if (C2 < r && r < A2 && B2 < pz)
                            {
                                FntPrint("%d Atari!\n", count);
                            }
                        }
                        else
                        {
                            FntPrint("left ");
                            r = Line_get_z(C - hx) + pz;
                            FntPrint("%d %d\n", count, r);
                            if (D2 < r && r < B2 && px < C2)
                            {
                                FntPrint("%d Atari!\n", count);
                            }
                        }
                        break;
                    case 2:
                        if (Cross2d(ScratchPad->Sv.vx, ScratchPad->Sv.vz, A2 - px, D2 - pz) < 0)
                        {
                            FntPrint("right ");
                            r = Line_get_x(D - hz) + px;
                            FntPrint("%d %d\n", count, r);
                            if (C2 < r && r < A2 && pz < D2)
                            {
                                FntPrint("%d Atari!\n", count);
                            }
                        }
                        else
                        {
                            FntPrint("left ");
                            r = Line_get_z(A - hx) + pz;
                            FntPrint("%d %d\n", count, r);
                            if (D2 < r && r < B2 && A2 < px)
                            {
                                FntPrint("%d Atari!\n", count);
                            }
                        }
                        break;
                    case 3:
                        if (Cross2d(ScratchPad->Sv.vx, ScratchPad->Sv.vz, C2 - px, D2 - pz) < 0)
                        {
                            FntPrint("right ");
                            r = Line_get_z(C - hx) + pz;
                            FntPrint("%d %d\n", count, r);
                            if (D2 < r && r < B2 && px < C2)
                            {
                                FntPrint("%d Atari!\n", count);
                            }
                        }
                        else
                        {
                            FntPrint("left ");
                            r = Line_get_x(D - hz) + px;
                            FntPrint("%d %d\n", count, r);
                            if (C2 < r && r < A2 && pz < D2)
                            {
                                FntPrint("%d Atari!\n", count);
                            }
                        }
                        break;
                    }
                }
                count++;
                p++;
            } while (p < end);
        }
        k++;
    } while (k < 4);
}

INT Line_get_x(SHORT dz)
{
    SHORT* p;

    p = &ScratchPad->Sv.vx;
    if (ScratchPad->Sv.vz == 0)
    {
        ScratchPad->Sv.vz = 1;
    }
    return (*p * dz) / ScratchPad->Sv.vz;
}

INT Line_get_z(SHORT dx)
{
    if (ScratchSv[0] == 0)
    {
        ScratchPad->Sv.vx = 1;
    }
    return ScratchPad->Sv.vz * dx / ScratchPad->Sv.vx;
}

INT Cross2d(INT a, INT b, INT c, INT d)
{
    return a * d - b * c;
}

INT Dot2d_swap(INT a, INT b, INT c, INT d)
{
    return a * d + b * c;
}

// hit test per weapon hit type: 0 knife, 1 to 4 gun, 5 to 9 projectile
INT (*Wep_hit_ck_tbl[10])(INT, ENEMY_WORK*) =
{
    Wep_hit_ck_knife,        // 0 //
    Wep_hit_ck_gun,          // 1 //
    Wep_hit_ck_gun,          // 2 //
    Wep_hit_ck_gun,          // 3 //
    Wep_hit_ck_gun,          // 4 //
    Wep_hit_ck_tama,   // 5 //
    Wep_hit_ck_tama,   // 6 //
    Wep_hit_ck_tama,   // 7 //
    Wep_hit_ck_tama,   // 8 //
    Wep_hit_ck_tama,   // 9 //
};

// post-hit effect function per weapon hit type
VOID (*Wep_set_tbl[10])(ENEMY_WORK*) =
{
    Wep_set_knife,      // 0 //
    Wep_set_rtn,   // 1 //
    Wep_set_shotgun,    // 2 //
    Wep_set_shotgun,    // 3 //
    Wep_set_shotgun,    // 4 //
    Wep_set_blood,      // 5 //
    Wep_set_blood2,     // 6 //
    Wep_set_sparks,     // 7 //
    Wep_set_blood,      // 8 //
    Wep_set_blood3,     // 9 //
};

// hit reaction function per enemy id
VOID (*Em_dm_rtn_tbl[20])(ENEMY_WORK*) =
{
    Em_dm_zombie,   // 0 //
    Em_dm_zombie,   // 1 //
    Em_dm_zombie,   // 2 //
    Em_dm_basic,    // 3 //
    Em_dm_basic,    // 4 //
    Em_dm_basic,    // 5 //
    Em_dm_head,     // 6 //
    Em_dm_basic,    // 7 //
    Em_dm_blood,    // 8 //
    Em_dm_none,     // 9 //
    Em_dm_basic,    // 10 //
    Em_dm_none,     // 11 //
    Em_dm_none,     // 12 //
    Em_dm_none,     // 13 //
    Em_dm_none,     // 14 //
    Em_dm_none,     // 15 //
    Em_dm_none,     // 16 //
    Em_dm_zombie,   // 17 //
    Em_dm_none,     // 18 //
    Em_dm_none,     // 19 //
};

// per enemy id: joint indices that get attack effects on a killing hit (first entry 0 means none)
UCHAR Em_kan_tbl[20][6] =
{
    { 0x02, 0x03, 0x04, 0x05, 0x07, 0x08 },   // 0 //
    { 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 },   // 1 //
    { 0x02, 0x03, 0x04, 0x05, 0x06, 0x08 },   // 2 //
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 3 //
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 4 //
    { 0x01, 0x02, 0x04, 0x05, 0x08, 0x09 },   // 5 //
    { 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 },   // 6 //
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 7 //
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 8 //
    { 0x08, 0x04, 0x05, 0x06, 0x09, 0x0A },   // 9 //
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 10 //
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 11 //
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 12 //
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 13 //
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 14 //
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 15 //
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 16 //
    { 0x02, 0x04, 0x05, 0x06, 0x07, 0x08 },   // 17 //
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 18 //
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 19 //
};

// hit reach per character (player id & 1, rows) and weapon hit type (columns)
INT Wep_range_tbl[2][10] =
{
    { 400, 1200, 2600, 800, 800, 400, 900, 900, 900, 1100 },   // 0 //
    { 500, 1300, 2600, 800, 800, 400, 900, 900, 900, 1100 },   // 1 //
};

// hit records per Damage table, enemy id and weapon hit type; table 0 is the default,
// table 1 applies with status flag 0x40000 and table 2 with flag 0x20000 without 0x10000
AT_ESP_REC Wep_at_tbl[3][20][10] =
{
    {
        { { 100, -1800, 0, 8, 0, 0, 1, 0 }, { 100, -2620, 0, 9, 0, 1, 1, 0 }, { 100, -2500, 0, 53, 4, 0, 2, 0 }, { 100, -2620, 0, 50, 4, 0, 2, 0 }, { 100, -2620, 0, 130, 4, 0, 2, 0 }, { 150, -1500, 0, 20, 14, 6, 1, 0 }, { 150, -1620, 0, 201, 0, 0, 2, 0 }, { 150, -1520, 0, 95, 9, 0, 1, 0 }, { 150, -1500, 0, 95, 14, 6, 1, 0 }, { 150, -1620, 0, 900, 0, 6, 2, 0 }, },   // 0 //
        { { 100, -1800, 0, 8, 0, 0, 1, 0 }, { 100, -2620, 0, 9, 0, 1, 1, 0 }, { 100, -2500, 0, 53, 4, 0, 2, 0 }, { 100, -2620, 0, 50, 4, 0, 2, 0 }, { 100, -2620, 0, 130, 4, 0, 2, 0 }, { 150, -1500, 0, 20, 14, 6, 1, 0 }, { 150, -1620, 0, 201, 0, 0, 2, 0 }, { 150, -1520, 0, 95, 9, 0, 1, 0 }, { 150, -1500, 0, 95, 14, 6, 1, 0 }, { 150, -1620, 0, 900, 0, 6, 2, 0 }, },   // 1 //
        { { 100, -1200, 0, 30, 0, 0, 1, 0 }, { 100, -1200, 0, 20, 0, 1, 1, 0 }, { 100, -1200, 0, 40, 4, 0, 2, 0 }, { 100, -1200, 0, 60, 4, 0, 2, 0 }, { 100, -1200, 0, 130, 4, 0, 2, 0 }, { 100, 0, 0, 20, 14, 6, 1, 0 }, { 100, -100, 0, 200, 0, 0, 2, 0 }, { 100, 0, 0, 100, 9, 0, 1, 0 }, { 100, 0, 0, 100, 14, 6, 1, 0 }, { 100, -100, 0, 900, 0, 6, 2, 0 }, },   // 2 //
        { { 0, -1200, 0, 10, 0, 8, 1, 0 }, { 0, -1220, 0, 20, 0, 9, 1, 0 }, { 0, -1100, 0, 40, 0, 8, 2, 0 }, { 0, -1220, 0, 40, 0, 8, 2, 0 }, { 0, -1220, 0, 130, 0, 8, 2, 0 }, { 0, -1100, 0, 20, 14, 7, 1, 0 }, { 0, -1120, 0, 100, 0, 0, 2, 0 }, { 0, -1120, 0, 100, 9, 0, 1, 0 }, { 0, -1100, 0, 200, 14, 7, 1, 0 }, { 0, -1120, 0, 900, 0, 7, 2, 0 }, },   // 3 //
        { { 0, -900, 0, 10, 0, 8, 1, 0 }, { 0, -920, 0, 14, 0, 9, 1, 0 }, { 0, -800, 0, 40, 0, 8, 2, 0 }, { 0, -920, 0, 50, 0, 8, 2, 0 }, { 0, -920, 0, 70, 0, 8, 2, 0 }, { 0, -800, 0, 20, 14, 7, 1, 0 }, { 0, -820, 0, 60, 0, 0, 2, 0 }, { 0, -820, 0, 60, 9, 0, 1, 0 }, { 0, -800, 0, 205, 14, 7, 1, 0 }, { 0, -820, 0, 900, 0, 7, 2, 0 }, },   // 4 //
        { { 0, 0, 0, 50, 0, 0, 1, 0 }, { 0, 0, 0, 26, 0, 0, 1, 0 }, { 0, 0, 0, 50, 0, 0, 2, 0 }, { 0, 0, 0, 50, 0, 0, 2, 0 }, { 0, 0, 0, 130, 0, 0, 2, 0 }, { 0, 0, 0, 20, 14, 5, 1, 0 }, { 0, 0, 0, 200, 0, 0, 2, 0 }, { 0, 0, 0, 60, 9, 0, 1, 0 }, { 0, 0, 0, 60, 14, 5, 1, 0 }, { 0, 0, 0, 900, 0, 5, 2, 0 }, },   // 5 //
        { { 0, -1500, 0, 16, 9, 6, 1, 0 }, { 0, -1500, 0, 14, 9, 6, 1, 0 }, { 0, -1500, 0, 32, 9, 6, 2, 0 }, { 0, -1500, 0, 40, 9, 6, 2, 0 }, { 0, -1500, 0, 130, 9, 6, 2, 0 }, { 150, -1500, 0, 20, 14, 6, 1, 0 }, { 150, -1500, 0, 100, 0, 0, 2, 0 }, { 150, -1500, 0, 200, 9, 0, 1, 0 }, { 150, -1500, 0, 100, 14, 6, 1, 0 }, { 150, -1500, 0, 900, 0, 6, 2, 0 }, },   // 6 //
        { { 0, 0, 0, 20, 0, 16, 1, 0 }, { 0, 0, 0, 30, 0, 17, 1, 0 }, { 0, 0, 0, 60, 0, 16, 2, 0 }, { 0, 0, 0, 70, 0, 16, 2, 0 }, { 0, 0, 0, 130, 0, 16, 2, 0 }, { 0, 0, 0, 20, 14, 4, 1, 0 }, { 0, 0, 0, 200, 0, 0, 2, 0 }, { 0, 0, 0, 80, 9, 0, 1, 0 }, { 0, 0, 0, 80, 14, 4, 1, 0 }, { 0, 0, 0, 900, 0, 4, 2, 0 }, },   // 7 //
        { { 0, 0, 0, 15, 1, 0, 1, 0 }, { 0, 1000, 0, 15, 0, 0, 1, 0 }, { 0, 1500, 0, 20, 0, 0, 2, 0 }, { 0, 1000, 0, 38, 0, 0, 2, 0 }, { 0, 1000, 0, 74, 0, 0, 2, 0 }, { 0, 1500, 0, 20, 14, 6, 1, 0 }, { 0, 1500, 0, 50, 2, 0, 2, 0 }, { 0, 1500, 0, 40, 9, 0, 1, 0 }, { 0, 1500, 0, 150, 14, 6, 1, 0 }, { 0, 1500, 0, 900, 2, 6, 2, 0 }, },   // 8 //
        { { 0, 0, 0, 17, 0, 0, 1, 0 }, { 0, 0, 0, 20, 1, 0, 1, 0 }, { 0, 0, 0, 30, 1, 0, 2, 0 }, { 0, 0, 0, 40, 1, 0, 2, 0 }, { 0, 0, 0, 130, 1, 0, 2, 0 }, { 150, -1500, 0, 20, 14, 6, 1, 0 }, { 150, -1500, 0, 200, 0, 0, 2, 0 }, { 150, -1500, 0, 60, 9, 0, 1, 0 }, { 150, -1500, 0, 60, 14, 6, 1, 0 }, { 150, -1500, 0, 900, 0, 6, 2, 0 }, },   // 9 //
        { { 0, 0, 0, 20, 0, 0, 1, 0 }, { 0, 0, 0, 20, 0, 0, 1, 0 }, { 0, 0, 0, 40, 0, 0, 2, 0 }, { 0, 0, 0, 50, 0, 0, 2, 0 }, { 0, 0, 0, 130, 0, 0, 2, 0 }, { 0, 0, 0, 30, 14, 3, 1, 0 }, { 0, 0, 0, 200, 0, 0, 2, 0 }, { 0, 0, 0, 60, 9, 0, 1, 0 }, { 0, 0, 0, 60, 14, 3, 1, 0 }, { 0, 0, 0, 900, 0, 3, 2, 0 }, },   // 10 //
        { { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, -1500, 0, 0, 14, 7, 1, 0 }, { 0, -1500, 0, 0, 0, 0, 2, 0 }, { 0, -1500, 0, 0, 9, 0, 1, 0 }, { 0, -1500, 0, 0, 14, 7, 1, 0 }, { 0, -1500, 0, 0, 0, 7, 2, 0 }, },   // 11 //
        { { 0, 0, 0, 10, 0, 0, 1, 0 }, { 0, 0, 0, 20, 1, 0, 1, 0 }, { 0, 0, 0, 30, 1, 0, 2, 0 }, { 0, 0, 0, 50, 1, 0, 2, 0 }, { 0, 0, 0, 80, 1, 0, 2, 0 }, { 150, -2000, 0, 20, 2, 6, 1, 0 }, { 150, -2000, 0, 100, 2, 0, 2, 0 }, { 150, -2000, 0, 100, 2, 0, 1, 0 }, { 150, -2000, 0, 100, 2, 6, 1, 0 }, { 150, -2000, 0, 900, 2, 6, 2, 0 }, },   // 12 //
        { { 0, 0, 0, 20, 0, 8, 1, 0 }, { 0, 0, 0, 30, 1, 0, 1, 0 }, { 0, 0, 0, 40, 1, 0, 2, 0 }, { 0, 0, 0, 40, 1, 0, 2, 0 }, { 0, 0, 0, 80, 1, 0, 2, 0 }, { 0, 0, 0, 20, 2, 7, 1, 0 }, { 0, 0, 0, 80, 2, 0, 2, 0 }, { 0, 0, 0, 130, 2, 0, 1, 0 }, { 0, 0, 0, 50, 2, 7, 1, 0 }, { 0, 0, 0, 900, 2, 7, 2, 0 }, },   // 13 //
        { { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, },   // 14 //
        { { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, },   // 15 //
        { { 0, 0, 0, 10, 0, 0, 1, 0 }, { 0, 0, 0, 20, 1, 0, 1, 0 }, { 0, 0, 0, 30, 1, 0, 2, 0 }, { 0, 0, 0, 50, 1, 0, 2, 0 }, { 0, 0, 0, 80, 1, 0, 2, 0 }, { 150, -2000, 0, 20, 2, 6, 1, 0 }, { 150, -2000, 0, 100, 2, 0, 2, 0 }, { 150, -2000, 0, 100, 2, 0, 1, 0 }, { 150, -2000, 0, 100, 2, 6, 1, 0 }, { 150, -2000, 0, 900, 2, 6, 2, 0 }, },   // 16 //
        { { 100, -1800, 0, 8, 0, 0, 1, 0 }, { 100, -2620, 0, 9, 0, 1, 1, 0 }, { 100, -1500, 0, 53, 4, 0, 2, 0 }, { 100, -2620, 0, 50, 4, 0, 2, 0 }, { 100, -2620, 0, 130, 4, 0, 2, 0 }, { 150, -2500, 0, 20, 14, 6, 1, 0 }, { 150, -1620, 0, 201, 0, 0, 2, 0 }, { 150, -1520, 0, 95, 9, 0, 1, 0 }, { 150, -1500, 0, 95, 14, 6, 1, 0 }, { 150, -1620, 0, 900, 0, 6, 2, 0 }, },   // 17 //
        { { 0, 0, 0, 20, 0, 8, 1, 0 }, { 0, 0, 0, 30, 1, 0, 1, 0 }, { 0, 0, 0, 40, 1, 0, 2, 0 }, { 0, 0, 0, 40, 1, 0, 2, 0 }, { 0, 0, 0, 80, 1, 0, 2, 0 }, { 0, 0, 0, 20, 2, 7, 1, 0 }, { 0, 0, 0, 80, 2, 0, 2, 0 }, { 0, 0, 0, 130, 2, 0, 1, 0 }, { 0, 0, 0, 50, 2, 7, 1, 0 }, { 0, 0, 0, 900, 2, 7, 2, 0 }, },   // 18 //
        { { 0, 0, 0, 10, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 2, 0 }, { 0, 0, 0, 0, 1, 0, 2, 0 }, { 0, 0, 0, 0, 1, 0, 2, 0 }, { 0, 0, 0, 2, 2, 7, 1, 0 }, { 0, 0, 0, 30, 2, 0, 2, 0 }, { 0, 0, 0, 30, 2, 0, 1, 0 }, { 0, 0, 0, 30, 2, 7, 1, 0 }, { 0, 0, 0, 900, 2, 7, 2, 0 }, },   // 19 //
    },   // 0 //
    {
        { { 100, -1800, 0, 11, 0, 0, 1, 0 }, { 100, -2620, 0, 12, 0, 1, 1, 0 }, { 100, -2500, 0, 53, 4, 0, 2, 0 }, { 100, -2620, 0, 50, 4, 0, 2, 0 }, { 100, -2620, 0, 130, 4, 0, 2, 0 }, { 150, -1500, 0, 20, 14, 6, 1, 0 }, { 150, -1620, 0, 201, 0, 0, 2, 0 }, { 150, -1520, 0, 95, 9, 0, 1, 0 }, { 150, -1500, 0, 95, 14, 6, 1, 0 }, { 150, -1620, 0, 900, 0, 6, 2, 0 }, },   // 0 //
        { { 100, -1800, 0, 11, 0, 0, 1, 0 }, { 100, -2620, 0, 12, 0, 1, 1, 0 }, { 100, -2500, 0, 53, 4, 0, 2, 0 }, { 100, -2620, 0, 50, 4, 0, 2, 0 }, { 100, -2620, 0, 130, 4, 0, 2, 0 }, { 150, -1500, 0, 20, 14, 6, 1, 0 }, { 150, -1620, 0, 201, 0, 0, 2, 0 }, { 150, -1520, 0, 95, 9, 0, 1, 0 }, { 150, -1500, 0, 95, 14, 6, 1, 0 }, { 150, -1620, 0, 900, 0, 6, 2, 0 }, },   // 1 //
        { { 100, -1200, 0, 34, 0, 0, 1, 0 }, { 100, -1200, 0, 27, 0, 1, 1, 0 }, { 100, -1200, 0, 50, 4, 0, 2, 0 }, { 100, -1200, 0, 60, 4, 0, 2, 0 }, { 100, -1200, 0, 130, 4, 0, 2, 0 }, { 100, 0, 0, 20, 14, 6, 1, 0 }, { 100, -100, 0, 200, 0, 0, 2, 0 }, { 100, 0, 0, 100, 9, 0, 1, 0 }, { 100, 0, 0, 100, 14, 6, 1, 0 }, { 100, -100, 0, 900, 0, 6, 2, 0 }, },   // 2 //
        { { 0, -1200, 0, 14, 0, 8, 1, 0 }, { 0, -1220, 0, 24, 0, 9, 1, 0 }, { 0, -1100, 0, 60, 0, 8, 2, 0 }, { 0, -1220, 0, 40, 0, 8, 2, 0 }, { 0, -1220, 0, 130, 0, 8, 2, 0 }, { 0, -1100, 0, 20, 14, 7, 1, 0 }, { 0, -1120, 0, 100, 0, 0, 2, 0 }, { 0, -1120, 0, 100, 9, 0, 1, 0 }, { 0, -1100, 0, 200, 14, 7, 1, 0 }, { 0, -1120, 0, 900, 0, 7, 2, 0 }, },   // 3 //
        { { 0, -900, 0, 18, 0, 8, 1, 0 }, { 0, -920, 0, 18, 0, 9, 1, 0 }, { 0, -800, 0, 42, 0, 8, 2, 0 }, { 0, -920, 0, 50, 0, 8, 2, 0 }, { 0, -920, 0, 70, 0, 8, 2, 0 }, { 0, -800, 0, 26, 14, 7, 1, 0 }, { 0, -820, 0, 70, 0, 0, 2, 0 }, { 0, -820, 0, 70, 9, 0, 1, 0 }, { 0, -800, 0, 205, 14, 7, 1, 0 }, { 0, -820, 0, 900, 0, 7, 2, 0 }, },   // 4 //
        { { 0, 0, 0, 60, 0, 0, 1, 0 }, { 0, 0, 0, 60, 0, 0, 1, 0 }, { 0, 0, 0, 60, 0, 0, 2, 0 }, { 0, 0, 0, 50, 0, 0, 2, 0 }, { 0, 0, 0, 130, 0, 0, 2, 0 }, { 0, 0, 0, 20, 14, 5, 1, 0 }, { 0, 0, 0, 200, 0, 0, 2, 0 }, { 0, 0, 0, 60, 9, 0, 1, 0 }, { 0, 0, 0, 60, 14, 5, 1, 0 }, { 0, 0, 0, 900, 0, 5, 2, 0 }, },   // 5 //
        { { 0, -1500, 0, 20, 9, 6, 1, 0 }, { 0, -1500, 0, 27, 9, 6, 1, 0 }, { 0, -1500, 0, 40, 9, 6, 2, 0 }, { 0, -1500, 0, 40, 9, 6, 2, 0 }, { 0, -1500, 0, 130, 9, 6, 2, 0 }, { 150, -1500, 0, 32, 14, 6, 1, 0 }, { 150, -1500, 0, 100, 0, 0, 2, 0 }, { 150, -1500, 0, 200, 9, 0, 1, 0 }, { 150, -1500, 0, 100, 14, 6, 1, 0 }, { 150, -1500, 0, 900, 0, 6, 2, 0 }, },   // 6 //
        { { 0, 0, 0, 20, 0, 16, 1, 0 }, { 0, 0, 0, 30, 0, 17, 1, 0 }, { 0, 0, 0, 60, 0, 16, 2, 0 }, { 0, 0, 0, 70, 0, 16, 2, 0 }, { 0, 0, 0, 130, 0, 16, 2, 0 }, { 0, 0, 0, 20, 14, 4, 1, 0 }, { 0, 0, 0, 200, 0, 0, 2, 0 }, { 0, 0, 0, 80, 9, 0, 1, 0 }, { 0, 0, 0, 80, 14, 4, 1, 0 }, { 0, 0, 0, 900, 0, 4, 2, 0 }, },   // 7 //
        { { 0, 0, 0, 20, 1, 0, 1, 0 }, { 0, 1000, 0, 20, 0, 0, 1, 0 }, { 0, 1500, 0, 30, 0, 0, 2, 0 }, { 0, 1000, 0, 38, 0, 0, 2, 0 }, { 0, 1000, 0, 74, 0, 0, 2, 0 }, { 0, 1500, 0, 20, 14, 6, 1, 0 }, { 0, 1500, 0, 60, 2, 0, 2, 0 }, { 0, 1500, 0, 45, 9, 0, 1, 0 }, { 0, 1500, 0, 150, 14, 6, 1, 0 }, { 0, 1500, 0, 900, 2, 6, 2, 0 }, },   // 8 //
        { { 0, 0, 0, 22, 0, 0, 1, 0 }, { 0, 0, 0, 22, 1, 0, 1, 0 }, { 0, 0, 0, 32, 1, 0, 2, 0 }, { 0, 0, 0, 40, 1, 0, 2, 0 }, { 0, 0, 0, 130, 1, 0, 2, 0 }, { 150, -1500, 0, 20, 14, 6, 1, 0 }, { 150, -1500, 0, 200, 0, 0, 2, 0 }, { 150, -1500, 0, 62, 9, 0, 1, 0 }, { 150, -1500, 0, 62, 14, 6, 1, 0 }, { 150, -1500, 0, 900, 0, 6, 2, 0 }, },   // 9 //
        { { 0, 0, 0, 60, 0, 0, 1, 0 }, { 0, 0, 0, 60, 0, 0, 1, 0 }, { 0, 0, 0, 60, 0, 0, 2, 0 }, { 0, 0, 0, 50, 0, 0, 2, 0 }, { 0, 0, 0, 130, 0, 0, 2, 0 }, { 0, 0, 0, 30, 14, 3, 1, 0 }, { 0, 0, 0, 200, 0, 0, 2, 0 }, { 0, 0, 0, 60, 9, 0, 1, 0 }, { 0, 0, 0, 60, 14, 3, 1, 0 }, { 0, 0, 0, 900, 0, 3, 2, 0 }, },   // 10 //
        { { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, -1500, 0, 0, 14, 7, 1, 0 }, { 0, -1500, 0, 0, 0, 0, 2, 0 }, { 0, -1500, 0, 0, 9, 0, 1, 0 }, { 0, -1500, 0, 0, 14, 7, 1, 0 }, { 0, -1500, 0, 0, 0, 7, 2, 0 }, },   // 11 //
        { { 0, 0, 0, 24, 0, 0, 1, 0 }, { 0, 0, 0, 20, 1, 0, 1, 0 }, { 0, 0, 0, 30, 1, 0, 2, 0 }, { 0, 0, 0, 50, 1, 0, 2, 0 }, { 0, 0, 0, 80, 1, 0, 2, 0 }, { 150, -2000, 0, 20, 2, 6, 1, 0 }, { 150, -2000, 0, 100, 2, 0, 2, 0 }, { 150, -2000, 0, 100, 2, 0, 1, 0 }, { 150, -2000, 0, 100, 2, 6, 1, 0 }, { 150, -2000, 0, 900, 2, 6, 2, 0 }, },   // 12 //
        { { 0, 0, 0, 26, 0, 8, 1, 0 }, { 0, 0, 0, 30, 1, 0, 1, 0 }, { 0, 0, 0, 40, 1, 0, 2, 0 }, { 0, 0, 0, 40, 1, 0, 2, 0 }, { 0, 0, 0, 80, 1, 0, 2, 0 }, { 0, 0, 0, 20, 2, 7, 1, 0 }, { 0, 0, 0, 80, 2, 0, 2, 0 }, { 0, 0, 0, 130, 2, 0, 1, 0 }, { 0, 0, 0, 50, 2, 7, 1, 0 }, { 0, 0, 0, 900, 2, 7, 2, 0 }, },   // 13 //
        { { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, },   // 14 //
        { { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, },   // 15 //
        { { 0, 0, 0, 24, 0, 0, 1, 0 }, { 0, 0, 0, 20, 1, 0, 1, 0 }, { 0, 0, 0, 30, 1, 0, 2, 0 }, { 0, 0, 0, 50, 1, 0, 2, 0 }, { 0, 0, 0, 80, 1, 0, 2, 0 }, { 150, -2000, 0, 20, 2, 6, 1, 0 }, { 150, -2000, 0, 100, 2, 0, 2, 0 }, { 150, -2000, 0, 100, 2, 0, 1, 0 }, { 150, -2000, 0, 100, 2, 6, 1, 0 }, { 150, -2000, 0, 900, 2, 6, 2, 0 }, },   // 16 //
        { { 100, -1800, 0, 14, 0, 0, 1, 0 }, { 100, -2620, 0, 15, 0, 1, 1, 0 }, { 100, -1500, 0, 53, 4, 0, 2, 0 }, { 100, -2620, 0, 50, 4, 0, 2, 0 }, { 100, -2620, 0, 130, 4, 0, 2, 0 }, { 150, -2500, 0, 20, 14, 6, 1, 0 }, { 150, -1620, 0, 201, 0, 0, 2, 0 }, { 150, -1520, 0, 95, 9, 0, 1, 0 }, { 150, -1500, 0, 95, 14, 6, 1, 0 }, { 150, -1620, 0, 900, 0, 6, 2, 0 }, },   // 17 //
        { { 0, 0, 0, 26, 0, 8, 1, 0 }, { 0, 0, 0, 30, 1, 0, 1, 0 }, { 0, 0, 0, 40, 1, 0, 2, 0 }, { 0, 0, 0, 40, 1, 0, 2, 0 }, { 0, 0, 0, 80, 1, 0, 2, 0 }, { 0, 0, 0, 20, 2, 7, 1, 0 }, { 0, 0, 0, 80, 2, 0, 2, 0 }, { 0, 0, 0, 130, 2, 0, 1, 0 }, { 0, 0, 0, 50, 2, 7, 1, 0 }, { 0, 0, 0, 900, 2, 7, 2, 0 }, },   // 18 //
        { { 0, 0, 0, 10, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 2, 0 }, { 0, 0, 0, 0, 1, 0, 2, 0 }, { 0, 0, 0, 0, 1, 0, 2, 0 }, { 0, 0, 0, 2, 2, 7, 1, 0 }, { 0, 0, 0, 30, 2, 0, 2, 0 }, { 0, 0, 0, 30, 2, 0, 1, 0 }, { 0, 0, 0, 30, 2, 7, 1, 0 }, { 0, 0, 0, 900, 2, 7, 2, 0 }, },   // 19 //
    },   // 1 //
    {
        { { 100, -1800, 0, 8, 0, 0, 1, 0 }, { 100, -2620, 0, 9, 0, 1, 1, 0 }, { 100, -2500, 0, 34, 4, 0, 2, 0 }, { 100, -2620, 0, 50, 4, 0, 2, 0 }, { 100, -2620, 0, 60, 4, 0, 2, 0 }, { 150, -1500, 0, 20, 14, 6, 1, 0 }, { 150, -1620, 0, 201, 0, 0, 2, 0 }, { 150, -1520, 0, 70, 9, 0, 1, 0 }, { 150, -1500, 0, 70, 14, 6, 1, 0 }, { 150, -1620, 0, 900, 0, 6, 2, 0 }, },   // 0 //
        { { 100, -1800, 0, 8, 0, 0, 1, 0 }, { 100, -2620, 0, 9, 0, 1, 1, 0 }, { 100, -2500, 0, 34, 4, 0, 2, 0 }, { 100, -2620, 0, 50, 4, 0, 2, 0 }, { 100, -2620, 0, 60, 4, 0, 2, 0 }, { 150, -1500, 0, 20, 14, 6, 1, 0 }, { 150, -1620, 0, 201, 0, 0, 2, 0 }, { 150, -1520, 0, 70, 9, 0, 1, 0 }, { 150, -1500, 0, 70, 14, 6, 1, 0 }, { 150, -1620, 0, 900, 0, 6, 2, 0 }, },   // 1 //
        { { 100, -1200, 0, 30, 0, 0, 1, 0 }, { 100, -1200, 0, 20, 0, 1, 1, 0 }, { 100, -1200, 0, 35, 4, 0, 2, 0 }, { 100, -1200, 0, 60, 4, 0, 2, 0 }, { 100, -1200, 0, 60, 4, 0, 2, 0 }, { 100, 0, 0, 20, 14, 6, 1, 0 }, { 100, -100, 0, 200, 0, 0, 2, 0 }, { 100, 0, 0, 90, 9, 0, 1, 0 }, { 100, 0, 0, 90, 14, 6, 1, 0 }, { 100, -100, 0, 900, 0, 6, 2, 0 }, },   // 2 //
        { { 0, -1200, 0, 10, 0, 8, 1, 0 }, { 0, -1220, 0, 15, 0, 9, 1, 0 }, { 0, -1100, 0, 24, 0, 8, 2, 0 }, { 0, -1220, 0, 30, 0, 8, 2, 0 }, { 0, -1220, 0, 40, 0, 8, 2, 0 }, { 0, -1100, 0, 20, 14, 7, 1, 0 }, { 0, -1120, 0, 100, 0, 0, 2, 0 }, { 0, -1120, 0, 100, 9, 0, 1, 0 }, { 0, -1100, 0, 200, 14, 7, 1, 0 }, { 0, -1120, 0, 900, 0, 7, 2, 0 }, },   // 3 //
        { { 0, -900, 0, 10, 0, 8, 1, 0 }, { 0, -920, 0, 12, 0, 9, 1, 0 }, { 0, -800, 0, 20, 0, 8, 2, 0 }, { 0, -920, 0, 50, 0, 8, 2, 0 }, { 0, -920, 0, 70, 0, 8, 2, 0 }, { 0, -800, 0, 20, 14, 7, 1, 0 }, { 0, -820, 0, 50, 0, 0, 2, 0 }, { 0, -820, 0, 50, 9, 0, 1, 0 }, { 0, -800, 0, 103, 14, 7, 1, 0 }, { 0, -820, 0, 900, 0, 7, 2, 0 }, },   // 4 //
        { { 0, 0, 0, 50, 0, 0, 1, 0 }, { 0, 0, 0, 26, 0, 0, 1, 0 }, { 0, 0, 0, 50, 0, 0, 2, 0 }, { 0, 0, 0, 50, 0, 0, 2, 0 }, { 0, 0, 0, 50, 0, 0, 2, 0 }, { 0, 0, 0, 20, 14, 5, 1, 0 }, { 0, 0, 0, 200, 0, 0, 2, 0 }, { 0, 0, 0, 60, 9, 0, 1, 0 }, { 0, 0, 0, 60, 14, 5, 1, 0 }, { 0, 0, 0, 900, 0, 5, 2, 0 }, },   // 5 //
        { { 0, -1500, 0, 16, 9, 6, 1, 0 }, { 0, -1500, 0, 14, 9, 6, 1, 0 }, { 0, -1500, 0, 25, 9, 6, 2, 0 }, { 0, -1500, 0, 40, 9, 6, 2, 0 }, { 0, -1500, 0, 80, 9, 6, 2, 0 }, { 150, -1500, 0, 20, 14, 6, 1, 0 }, { 150, -1500, 0, 50, 0, 0, 2, 0 }, { 150, -1500, 0, 80, 9, 0, 1, 0 }, { 150, -1500, 0, 50, 14, 6, 1, 0 }, { 150, -1500, 0, 900, 0, 6, 2, 0 }, },   // 6 //
        { { 0, 0, 0, 20, 0, 16, 1, 0 }, { 0, 0, 0, 30, 0, 17, 1, 0 }, { 0, 0, 0, 60, 0, 16, 2, 0 }, { 0, 0, 0, 70, 0, 16, 2, 0 }, { 0, 0, 0, 70, 0, 16, 2, 0 }, { 0, 0, 0, 20, 14, 4, 1, 0 }, { 0, 0, 0, 200, 0, 0, 2, 0 }, { 0, 0, 0, 80, 9, 0, 1, 0 }, { 0, 0, 0, 80, 14, 4, 1, 0 }, { 0, 0, 0, 900, 0, 4, 2, 0 }, },   // 7 //
        { { 0, 0, 0, 15, 1, 0, 1, 0 }, { 0, 1000, 0, 10, 0, 0, 1, 0 }, { 0, 1500, 0, 20, 0, 0, 2, 0 }, { 0, 1000, 0, 38, 0, 0, 2, 0 }, { 0, 1000, 0, 20, 0, 0, 2, 0 }, { 0, 1500, 0, 20, 14, 6, 1, 0 }, { 0, 1500, 0, 40, 2, 0, 2, 0 }, { 0, 1500, 0, 40, 9, 0, 1, 0 }, { 0, 1500, 0, 150, 14, 6, 1, 0 }, { 0, 1500, 0, 900, 2, 6, 2, 0 }, },   // 8 //
        { { 0, 0, 0, 10, 0, 0, 1, 0 }, { 0, 0, 0, 12, 1, 0, 1, 0 }, { 0, 0, 0, 20, 1, 0, 2, 0 }, { 0, 0, 0, 40, 1, 0, 2, 0 }, { 0, 0, 0, 41, 1, 0, 2, 0 }, { 150, -1500, 0, 20, 14, 6, 1, 0 }, { 150, -1500, 0, 60, 0, 0, 2, 0 }, { 150, -1500, 0, 60, 9, 0, 1, 0 }, { 150, -1500, 0, 100, 14, 6, 1, 0 }, { 150, -1500, 0, 900, 0, 6, 2, 0 }, },   // 9 //
        { { 0, 0, 0, 20, 0, 0, 1, 0 }, { 0, 0, 0, 20, 0, 0, 1, 0 }, { 0, 0, 0, 40, 0, 0, 2, 0 }, { 0, 0, 0, 50, 0, 0, 2, 0 }, { 0, 0, 0, 50, 0, 0, 2, 0 }, { 0, 0, 0, 30, 14, 3, 1, 0 }, { 0, 0, 0, 200, 0, 0, 2, 0 }, { 0, 0, 0, 60, 9, 0, 1, 0 }, { 0, 0, 0, 60, 14, 3, 1, 0 }, { 0, 0, 0, 900, 0, 3, 2, 0 }, },   // 10 //
        { { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, -1500, 0, 0, 14, 7, 1, 0 }, { 0, -1500, 0, 0, 0, 0, 2, 0 }, { 0, -1500, 0, 0, 9, 0, 1, 0 }, { 0, -1500, 0, 0, 14, 7, 1, 0 }, { 0, -1500, 0, 0, 0, 7, 2, 0 }, },   // 11 //
        { { 0, 0, 0, 10, 0, 0, 1, 0 }, { 0, 0, 0, 15, 1, 0, 1, 0 }, { 0, 0, 0, 20, 1, 0, 2, 0 }, { 0, 0, 0, 50, 1, 0, 2, 0 }, { 0, 0, 0, 40, 1, 0, 2, 0 }, { 150, -2000, 0, 20, 2, 6, 1, 0 }, { 150, -2000, 0, 35, 2, 0, 2, 0 }, { 150, -2000, 0, 35, 2, 0, 1, 0 }, { 150, -2000, 0, 35, 2, 6, 1, 0 }, { 150, -2000, 0, 900, 2, 6, 2, 0 }, },   // 12 //
        { { 0, 0, 0, 15, 0, 8, 1, 0 }, { 0, 0, 0, 18, 1, 0, 1, 0 }, { 0, 0, 0, 15, 1, 0, 2, 0 }, { 0, 0, 0, 40, 1, 0, 2, 0 }, { 0, 0, 0, 60, 1, 0, 2, 0 }, { 0, 0, 0, 20, 2, 7, 1, 0 }, { 0, 0, 0, 60, 2, 0, 2, 0 }, { 0, 0, 0, 120, 2, 0, 1, 0 }, { 0, 0, 0, 60, 2, 7, 1, 0 }, { 0, 0, 0, 900, 2, 7, 2, 0 }, },   // 13 //
        { { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, },   // 14 //
        { { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, { 0, 0, 0, 0, 1, 0, 0, 0 }, },   // 15 //
        { { 0, 0, 0, 10, 0, 0, 1, 0 }, { 0, 0, 0, 15, 1, 0, 1, 0 }, { 0, 0, 0, 20, 1, 0, 2, 0 }, { 0, 0, 0, 50, 1, 0, 2, 0 }, { 0, 0, 0, 40, 1, 0, 2, 0 }, { 150, -2000, 0, 20, 2, 6, 1, 0 }, { 150, -2000, 0, 35, 2, 0, 2, 0 }, { 150, -2000, 0, 35, 2, 0, 1, 0 }, { 150, -2000, 0, 35, 2, 6, 1, 0 }, { 150, -2000, 0, 900, 2, 6, 2, 0 }, },   // 16 //
        { { 100, -1800, 0, 8, 0, 0, 1, 0 }, { 100, -2620, 0, 9, 0, 1, 1, 0 }, { 100, -1500, 0, 34, 4, 0, 2, 0 }, { 100, -2620, 0, 50, 4, 0, 2, 0 }, { 100, -2620, 0, 60, 4, 0, 2, 0 }, { 150, -2500, 0, 20, 14, 6, 1, 0 }, { 150, -1620, 0, 201, 0, 0, 2, 0 }, { 150, -1520, 0, 70, 9, 0, 1, 0 }, { 150, -1500, 0, 70, 14, 6, 1, 0 }, { 150, -1620, 0, 900, 0, 6, 2, 0 }, },   // 17 //
        { { 0, 0, 0, 15, 0, 8, 1, 0 }, { 0, 0, 0, 18, 1, 0, 1, 0 }, { 0, 0, 0, 15, 1, 0, 2, 0 }, { 0, 0, 0, 40, 1, 0, 2, 0 }, { 0, 0, 0, 60, 1, 0, 2, 0 }, { 0, 0, 0, 20, 2, 7, 1, 0 }, { 0, 0, 0, 60, 2, 0, 2, 0 }, { 0, 0, 0, 120, 2, 0, 1, 0 }, { 0, 0, 0, 60, 2, 7, 1, 0 }, { 0, 0, 0, 900, 2, 7, 2, 0 }, },   // 18 //
        { { 0, 0, 0, 6, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 1, 0 }, { 0, 0, 0, 0, 1, 0, 2, 0 }, { 0, 0, 0, 0, 1, 0, 2, 0 }, { 0, 0, 0, 0, 1, 0, 2, 0 }, { 0, 0, 0, 2, 2, 7, 1, 0 }, { 0, 0, 0, 10, 2, 0, 2, 0 }, { 0, 0, 0, 20, 2, 0, 1, 0 }, { 0, 0, 0, 30, 2, 7, 1, 0 }, { 0, 0, 0, 900, 2, 7, 2, 0 }, },   // 19 //
    },   // 2 //
};

// handgun critical hit flags, picked by rand() & 0xF in the advanced mode
UCHAR Wep_crit_tbl[0x10] =
{
    0x01,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x00,   // 3 //
    0x00,   // 4 //
    0x00,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x00,   // 12 //
    0x00,   // 13 //
    0x01,   // 14 //
    0x00,   // 15 //
};

// build stamp text stored right behind the critical hit table (36 bytes, zero padded)
UCHAR Build_stamp[0x24] = "CAPCOM-Jul 27 1998-09:43:23-CAPCOM";   // build stamp
