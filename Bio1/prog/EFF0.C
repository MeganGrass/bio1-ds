/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EFF0.C
 */

#include "EFF1.H"
#include <RAND.H>
#include "EFF0.H"
#include "ATTACK.H"
#include "PL_WEP.H"
#include "SCE0.H"
#include "FLAG.H"
#include "SCA.H"
#include "SCE1.H"
#include "SND.H"

VOID Esp_nop(VOID)
{
}

VOID Esp_00(VOID)
{
}

VOID Esp_03(VOID)
{
    ESP_POOL[G.Esp_idx].Anm_time = 0;
}

VOID Esp_0d(VOID)
{
    if (pESP->Free2-- == 0)
    {
        Esp_1c();
    }
}

VOID Esp_0e(VOID)
{
    pESP->Be_flg ^= 0x8000;
}

VOID Esp_14(VOID)
{
    Esp_1c();
    pESP->Speed_y -= 0x18;
}

VOID Esp_1b(VOID)
{
    pESP->pAnm_cur += 1;
    pESP->Anm_time = 0;
}

VOID Esp_1c(VOID)
{
    USHORT Bak_dir;
    UCHAR Bak_scale;

#ifdef __INTELLISENSE__  // void* arithmetic is not valid C for IntelliSense
    *(UCHAR**)&pESP->pMoveseq += sizeof(MSEQ_REC);
#else
    pESP->pMoveseq += sizeof(MSEQ_REC);
#endif
    Bak_dir = pESP->DirY;
    Bak_scale = pESP->Mag;
    *(MSEQ_REC*)pESP = *(MSEQ_REC*)pESP->pMoveseq;  // reinterprets the memory as MSEQ_REC

    if (pESP->Routine0 == 0)
    {
        Esp_2f();
    }

    pESP->DirY = Bak_dir;
    pESP->Mag = Bak_scale;
}

VOID Esp_1d(VOID)
{
    pESP->pAnm_cur = rand() % 3 + pESP->pAnm_cur;
    pESP->Anm_time = pESP->pAnm_cur->Time;
    pESP->pSprg_cur = (INT*)pESP->pSprg + pESP->pAnm_cur->uv_index;  // pSprg is a table of words
}

VOID Esp_1f(VOID)
{
    pESP->pAnm_cur = pESP->pAnmseq;
    pESP->pAnm_cur = pESP->Free0 + pESP->pAnm_cur;
    pESP->Anm_time = pESP->pAnm_cur->Time;
    pESP->pSprg_cur = (INT*)pESP->pSprg + pESP->pAnm_cur->uv_index;  // pSprg is a table of words
}

VOID Esp_0c(VOID)
{
    if (pESP->Free2-- == 0)
    {
        ScratchVec[0] = pESP->Hot_x;
        ScratchPad->Vec.vy = pESP->Hot_y;
        ScratchPad->Vec.vz = pESP->Hot_z;
        Esp_call(pESP->Type,
            pESP->Id,
            pESP->DirY,

            pESP->pMat, &ScratchPad->Vec, 0);

        Esp_1c();
    }
}

VOID Esp_12(VOID)
{
    UCHAR c;

    UCHAR Loop_ctr;
    UCHAR i;

    c = ESP_SLOTS[ESP_IDX].h.Free0--;
    if (c == 0)
    {
        if (ESP_FREE_CNT == 0)
        {
            ESP_SLOTS[ESP_IDX].h.Routine0 = 1;
            return;
        }

        Loop_ctr = ESP_FREE_CNT;
        i = 0x40;

        do
        {
            i--;
            if (ESP_SLOTS[i].h.Routine0 == 0)
            {
                ESP_FREE_CNT--;
            }
        } while (ESP_FREE_CNT == Loop_ctr);
        ESP_SLOTS[i].Mov_x = ESP_SLOTS[i].Mov_y = ESP_SLOTS[i].Mov_z = 0;
        ESP_SLOTS[i].Pos_x = ESP_SLOTS[i].Pos_y = ESP_SLOTS[i].Pos_z = 0;
        ESP_SLOTS[i].M00 = ESP_SLOTS[i].M01 = ESP_SLOTS[i].m02 = 0;
        ESP_SLOTS[i].m10 = ESP_SLOTS[i].M11 = ESP_SLOTS[i].m12 = 0;
        ESP_SLOTS[i].M20 = ESP_SLOTS[i].M21 = ESP_SLOTS[i].M22 = 0;
        ESP_SLOTS[i].trans_x = ESP_SLOTS[i].Trans_y = ESP_SLOTS[i].Trans_z = 0;
        ESP_SLOTS[i].Otz = 0;
        ESP_SLOTS[i].Otz4 = 0;
        ESP_SLOTS[i].Type = ESP_SLOTS[ESP_IDX].Type;
        ESP_SLOTS[i].Id = ESP_SLOTS[ESP_IDX].Id;
        ESP_SLOTS[i].pMat = ESP_SLOTS[ESP_IDX].pMat;
        ESP_SLOTS[i].spawn = ESP_SLOTS[ESP_IDX].spawn;
        ESP_SLOTS[i].pEsp_hdr = ESP_SLOTS[ESP_IDX].pEsp_hdr;
        ESP_SLOTS[i].pAnmseq = ESP_SLOTS[i].pAnm_cur = ESP_SLOTS[ESP_IDX].pAnmseq;
        ESP_SLOTS[i].pSprg = ESP_SLOTS[i].pSprg_cur = ESP_SLOTS[ESP_IDX].pSprg;
        ESP_SLOTS[i].pMoveseq_top = ESP_SLOTS[i].pMoveseq = ESP_SLOTS[ESP_IDX].pMoveseq_top;
        ESP_SLOTS[i].Hot_x = ESP_SLOTS[ESP_IDX].spawn.vx;
        ESP_SLOTS[i].Hot_y = ESP_SLOTS[ESP_IDX].spawn.vy;
        ESP_SLOTS[i].Hot_z = ESP_SLOTS[ESP_IDX].spawn.vz;
        ESP_SLOTS[i].pMoveseq_top = ESP_SLOTS[ESP_IDX].pMoveseq_top;
        ESP_SLOTS[i].pMoveseq = ESP_SLOTS[ESP_IDX].pMoveseq + 1;
        ESP_SLOTS[i].h = *ESP_SLOTS[i].pMoveseq;

        ESP_SLOTS[ESP_IDX].h.Routine0 = 1;
    }
}

VOID Esp_1a(VOID)
{
    ScratchVec[0] = pESP->Pos_x;
    ScratchPad->Vec.vy = pESP->Pos_y;
    ScratchPad->Vec.vz = pESP->Pos_z;

    ScratchWord[0] = Esp_call(pESP->Free0, pESP->Free1, pESP->DirY, 0, &ScratchPad->Vec, 0);
}

VOID Esp_1e(VOID)
{
    ScratchVec[0] = pESP->Pos_x;
    ScratchPad->Vec.vy = pESP->Pos_y;
    ScratchPad->Vec.vz = pESP->Pos_z;

    Esp_call(pESP->Free0, pESP->Free1, pESP->DirY, 0, &ScratchPad->Vec, 0);

    Esp_1c();
}

VOID Esp_02(VOID)
{
    UCHAR* idx;
    INT v;

    idx = &MAIN.Esp_idx;
    v = ESP_POOL[*idx].Free3;
    if (v == 0)
    {
        Esp_1c();

        ESP_POOL[*idx].Mat = *ESP_POOL[*idx].pMat;
    }
    else
    {
        ESP_POOL[*idx].Free3 = v - 1;
    }
}

VOID Esp_0b(VOID)
{
    USHORT Bak_dir;
    UCHAR Bak_free;

    UCHAR Bak_scale;

    Esp_1f();
#ifdef __INTELLISENSE__  // void* arithmetic is not valid C for IntelliSense
    *(UCHAR**)&pESP->pMoveseq += sizeof(MSEQ_REC);
#else
    pESP->pMoveseq += sizeof(MSEQ_REC);
#endif
    Bak_dir = pESP->DirY;
    Bak_free = pESP->Free3;

    Bak_scale = pESP->Mag;
    if (G.SaveData.Sce_random & 1)
    {
#ifdef __INTELLISENSE__  // void* arithmetic is not valid C for IntelliSense
        *(UCHAR**)&pESP->pMoveseq += sizeof(MSEQ_REC);
#else
        pESP->pMoveseq += sizeof(MSEQ_REC);
#endif
        *(ESP_STEP_REC*)pESP = *(ESP_STEP_REC*)pESP->pMoveseq;  // reinterprets the memory as ESP_STEP_REC
    }
    else
    {
        *(ESP_STEP_REC*)pESP = *(ESP_STEP_REC*)pESP->pMoveseq;  // reinterprets the memory as ESP_STEP_REC
#ifdef __INTELLISENSE__  // void* arithmetic is not valid C for IntelliSense
        *(UCHAR**)&pESP->pMoveseq += sizeof(MSEQ_REC);
#else
        pESP->pMoveseq += sizeof(MSEQ_REC);
#endif
    }

    pESP->DirY = Bak_dir + pESP->DirY;

    pESP->Free3 = Bak_free;

    pESP->Mag = Bak_scale;

    pESP->Mat = *pESP->pMat;
}

VOID Esp_26(VOID)
{
    UCHAR t[10] = { 7, 13, 8, 8, 15, 15, 15, 17, 17, 17 };

    pESP->Routine1 = 1;
    pESP->Mat_mode = 2;
    pESP->Mat = *pESP->pMat;
    pESP->Mag = t[pESP->Free0];
}

VOID Esp_28(VOID)
{
    Esp_02();
    ESP_POOL[G.Esp_idx].Free2 = G.Player.Be_flg;
}

VOID Esp_04(VOID)
{
    if (pESP->Free3-- == 0)
    {
        Esp_1c();
        pESP->Mag = 0x1E;
        pESP->Hot_x -= 0xFA;
    }
}

VOID Esp_06(VOID)
{
    if (pESP->Free3-- == 0)
    {
        Esp_1c();
    }
}

VOID Esp_05(VOID)
{
    ESP_WORK* pool;

    ScratchPad->Vec.vx = pESP->Pos_x;
    pool = ESP_POOL;
    ScratchPad->Vec.vy = pESP->Pos_y;
    ScratchPad->Vec.vz = pESP->Pos_z;
    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0;
    if (Sca_ck_point_r(&ScratchPad->Vec, &ScratchPad->Sv, 4) != 0)
    {
        pESP->Speed_x = pESP->Speed_y = pESP->Speed_z = 0;
        pool[ESP_IDX].Routine0 = 1;
    }
}

VOID Esp_08(VOID)
{
    INT Tmp;

    ScratchVec[0] = pESP->Pos_x;
    ScratchPad->Vec.vy = pESP->Pos_y;
    ScratchPad->Vec.vz = pESP->Pos_z;
    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0;
    if (Sca_ck_point_r(&ScratchPad->Vec, &ScratchPad->Sv, 4) != 0)
    {
        pESP->Routine0 = 1;
        pESP->Free0 = 2;
        pESP->Free2 = 1;
        Tmp = -pESP->Speed_x;  // sign-extends the halfword
        pESP->Speed_x = Tmp;
        if (G.Player.Type - 2 == pESP->Free3)
        {
            ScratchPad->Vec.vx = pESP->Pos_x;
            ScratchPad->Vec.vy = pESP->Pos_y;
            ScratchPad->Vec.vz = pESP->Pos_z;
            Snd_se_stad(1, 0xA, 0, &ScratchPad->Vec);
        }
    }
}

VOID Esp_07(VOID)
{
    if (pESP->Pos_y + pESP->Speed_y >= 0)
    {
        pESP->Routine0 = 1;
        pESP->Routine1 = 1;
        pESP->Be_flg |= 0x4007;
        pESP->Add_x = pESP->Add_y = pESP->Add_z = 0;
        pESP->Speed_x = pESP->Speed_y = pESP->Speed_z = 0;
        {
            INT pos_y = pESP->Hot_y + pESP->Mat.t[1];
            pESP->Mov_y = -pos_y;
        }
    }
}

VOID Esp_09(VOID)
{
    pESP->Be_flg = 0x4003;
    if (MAIN.Player.Ground < pESP->Pos_y + pESP->Speed_y)
    {
        pESP->Be_flg |= 0x400B;
        pESP->Routine0 = 0x2F;
        pESP->Routine1 = 0;
        {
            INT pos_y = pESP->Hot_y + pESP->Mat.t[1];
            pESP->Mov_y = G.Player.Ground - pos_y;
        }
    }
}

VOID Esp_0a(VOID)
{
    if (G.Status_flg & STAT_WATER)
    {
        if (ESP_CASINGS[ESP_IDX].Pos_y + ESP_CASINGS[ESP_IDX].Speed_y > Omodel_tbl[0]->Coord.t[1])
        {
            ESP_CASINGS[ESP_IDX].Be_flg |= 0x400B;
            ESP_CASINGS[ESP_IDX].Routine0 = 0x2F;
            ESP_CASINGS[ESP_IDX].Routine1 = 0;
            {
                INT pos_y = ESP_CASINGS[ESP_IDX].Hot_y + ESP_CASINGS[ESP_IDX].Trans_y;
                ESP_CASINGS[ESP_IDX].Mov_y = G.Player.Ground - pos_y;
            }
            ScratchVec[0] = ESP_CASINGS[ESP_IDX].Pos_x;
            ScratchPad->Vec.vy = ESP_CASINGS[ESP_IDX].Pos_y;
            ScratchPad->Vec.vz = ESP_CASINGS[ESP_IDX].Pos_z;

            Esp_call(0x17, 8, ESP_CASINGS[ESP_IDX].DirY, 0, &ScratchPad->Vec, 5);
        }
    }
    else
    {
        if (G.Player.Ground < ESP_CASINGS[ESP_IDX].Pos_y + ESP_CASINGS[ESP_IDX].Speed_y)
        {
            ESP_CASINGS[ESP_IDX].Be_flg |= 0x400B;
            ESP_CASINGS[ESP_IDX].pMoveseq += 1;

            ESP_CASINGS[ESP_IDX].pMoveseq += (rand() & SCE_RANDOM_CUR) % ESP_CASINGS[ESP_IDX].Free1;

            ESP_CASINGS[ESP_IDX].Routine1 = ESP_CASINGS[ESP_IDX].pMoveseq->Routine1;
            ESP_CASINGS[ESP_IDX].Speed_x = ESP_CASINGS[ESP_IDX].pMoveseq->Speed_x;
            ESP_CASINGS[ESP_IDX].Speed_y = ESP_CASINGS[ESP_IDX].pMoveseq->Speed_y;

            ESP_CASINGS[ESP_IDX].Speed_z = ESP_CASINGS[ESP_IDX].pMoveseq->Speed_z + (rand() & MAIN.SaveData.Sce_random) % ESP_CASINGS[ESP_IDX].Free1 * 10;

            ESP_CASINGS[ESP_IDX].DirY += ESP_CASINGS[ESP_IDX].pMoveseq->DirY;
            {
                INT pos_y = ESP_CASINGS[ESP_IDX].Hot_y + ESP_CASINGS[ESP_IDX].Trans_y;
                ESP_CASINGS[ESP_IDX].Mov_y = G.Player.Ground - pos_y;
            }
            if (ESP_CASINGS[ESP_IDX].Free2 != 0)
            {
                INT t = ESP_CASINGS[ESP_IDX].Speed_x;
                ESP_CASINGS[ESP_IDX].Speed_x = -t;
            }
            if (G.Player.Type - 2 == ESP_CASINGS[ESP_IDX].Free3)
            {
                ScratchPad->Vec.vx = ESP_CASINGS[ESP_IDX].Pos_x;
                ScratchPad->Vec.vy = ESP_CASINGS[ESP_IDX].Pos_y;
                ScratchPad->Vec.vz = ESP_CASINGS[ESP_IDX].Pos_z;
                Snd_se_stad(1, 10, 0, &ScratchPad->Vec);
            }
        }
    }
}

VOID Esp_0f(VOID)
{
    if (pESP->Pos_y + pESP->Speed_y >= 0)
    {
        pESP->Routine0 = 0x2F;
        pESP->Routine1 = 0;
        {
            INT pos_y = pESP->Hot_y + pESP->Mat.t[1];
            pESP->Mov_y = -pos_y;
        }
        Esp_1a();
        ESP_POOL[ScratchWord[0]].Mag = ESP_POOL[ScratchWord[0]].Mag + KEEP_VALUE(-3) + pESP->Mag;  // KEEP_VALUE keeps the constant out of GCC 2.6.3's folding; the plain form and a local change the code
    }
}

VOID Esp_13(VOID)
{
    if (pESP->Pos_y + pESP->Speed_y >= 0)
    {
        pESP->Routine0 = 0x2F;
        pESP->Be_flg = 0x4007;
        pESP->Add_x = pESP->Add_y = pESP->Add_z = 0;
        pESP->Speed_x = pESP->Speed_y = pESP->Speed_z = 0;
        {
            INT pos_y = pESP->Hot_y + pESP->Mat.t[1];
            pESP->Mov_y = -pos_y;
        }
    }
}

VOID Esp_20(VOID)
{
    if (pESP->Pos_y + pESP->Speed_y >= 0)
    {
        pESP->Add_x = pESP->Add_y = pESP->Add_z = 0;
        pESP->Speed_x = pESP->Speed_y = pESP->Speed_z = 0;
        {
            INT pos_y = pESP->Hot_y + pESP->Mat.t[1];
            pESP->Mov_y = -pos_y;
        }
        Esp_1f();
        Esp_1c();
    }
}

VOID Esp_32(VOID)
{
    if (pESP->Pos_y + pESP->Speed_y >= 0)
    {
        pESP->Routine0 = 0x2F;
        pESP->Routine1 = 0;
        pESP->Be_flg = 6;
        pESP->Add_x = pESP->Add_y = pESP->Add_z = 0;
        pESP->Speed_x = pESP->Speed_y = pESP->Speed_z = 0;
        {
            INT pos_y = pESP->Hot_y + pESP->Mat.t[1];
            pESP->Mov_y = -pos_y;
        }
    }
}

VOID Esp_10(VOID)
{
    UCHAR t;

    t = pESP->Free3;
    pESP->Free3 = t - 1;
    if (t != 0)
    {
        pESP->Be_flg |= (rand() % 2) << 7;
        pESP->Speed_x =
            (120 - rand() % 6 * 50) * (SCHAR)pESP->Free2;  // sign-extends the byte
        pESP->Add_y = -8 - rand() % 6;
        pESP->Mag -= rand() % 2;
        pESP->DirY = rand() % 6 * 60 * 4096 / 360;
        pESP->Free0 = rand() % 6;
        Esp_1f();
    }
    else
    {
        pESP->Routine0 = 0x11;
        pESP->Free3 = 0;
        pESP->Mat_mode = 2;
        pESP->Mat = *pESP->pMat;
    }
}

VOID Esp_11(VOID)
{
    ScratchPad->Vec.vx = pESP->Pos_x;
    ScratchPad->Vec.vy = pESP->Pos_y;
    ScratchPad->Vec.vz = pESP->Pos_z;
    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0;
    if (pESP->Speed_y <= 0 || Sca_ck_point_r(&ScratchPad->Vec, &ScratchPad->Sv, 0x3C) != 0)
    {
        pESP->Add_x = pESP->Add_x * -1;  // sign-extends the halfword
    }
    if (pESP->Speed_y < (SCHAR)pESP->Free3)  // sign-extends the byte
    {
        pESP->Speed_x =
            (SCHAR)pESP->Free2 * (30 - rand() % 4);  // sign-extends the byte
        pESP->Free3 =
            (pESP->Speed_y = 70 - rand() % 4 * 2) / -3;
        pESP->Speed_z = pESP->Speed_z +
                                                                    (SCHAR)pESP->Free2 * (rand() % 5);  // sign-extends the byte
        pESP->Add_x =
            (SCHAR)pESP->Free2 * (rand() % 3);  // sign-extends the byte
        pESP->Add_y = -2 - rand() % 2;
        pESP->Free2 = (SCHAR)pESP->Free2 * -1;  // sign-extends the byte
    }
}

VOID Esp_15(VOID)
{
    if (pESP->Free0-- == 0)
    {
        pESP->Routine0 = 0x16;
        pESP->Speed_y = 0;
        pESP->Add_y = pESP->Free2;
    }
    Esp_16();
}

VOID Esp_16(VOID)
{
    ScratchPad->Vec.vx = pESP->Pos_x;
    ScratchPad->Vec.vy = pESP->Pos_y;
    ScratchPad->Vec.vz = pESP->Pos_z;
    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0;
    if (Sca_ck_point_r(&ScratchPad->Vec, &ScratchPad->Sv, 4) != 0)
    {
        Esp_splat_set();
    }
    if (Pos_pl_at_ck(0x258, pESP->Pos_x, pESP->Pos_z) != 0)
    {
        Esp_splat_set();
    }
}

VOID Esp_17(VOID)
{
    if (pESP->Pos_y + pESP->Speed_y >= 0)
    {
        Esp_splat_set();
        pESP->Be_flg |= 0x4007;
    }
}

VOID Esp_splat_set(VOID)
{
    pESP->pAnm_cur = pESP->pAnmseq + 5;
    pESP->Anm_cnt = pESP->pAnm_cur->uv_index;
    pESP->Anm_time = pESP->pAnm_cur->Time;

    pESP->pSprg_cur = pESP->Anm_cnt * 4 + (UCHAR*)pESP->pSprg;

    pESP->Routine0 = 1;
    pESP->Routine1 = 1;
    pESP->Speed_x = pESP->Speed_y = pESP->Add_x = pESP->Add_y = 0;
    if (pESP->Free1 != 0)
    {
        ScratchVec[0] = pESP->Pos_x;
        ScratchPad->Vec.vy = pESP->Pos_y;
        ScratchPad->Vec.vz = pESP->Pos_z;

        Esp_call(9, 1, 0, 0, &ScratchPad->Vec, pESP->Free3);

        Snd_se_stad(2, 8, 0, &ScratchPad->Vec);
    }
}

VOID Esp_18(VOID)
{
    if (pESP->Free3 & 1)
    {
        pESP->Speed_x = pESP->Speed_x + rand() % 5;
    }
    if (pESP->Free3 & 2)
    {
        pESP->Speed_y = pESP->Speed_y + rand() % 5;
    }
    if (pESP->Free3 & 4)
    {
        pESP->Speed_z = pESP->Speed_z + rand() % 5;
    }
    if (pESP->Free3 & 8)
    {
        pESP->Add_x = pESP->Add_x + rand() % 5;
    }
    if (pESP->Free3 & 0x10)
    {
        pESP->Add_y = pESP->Add_y + rand() % 5;
    }
    if (pESP->Free3 & 0x20)
    {
        pESP->Add_z = pESP->Add_z + rand() % 5;
    }
}

VOID Esp_29(VOID)
{
    MAIN.Esp[ESP_IDX].Be_flg |= (rand() % 3) << 6;
    MAIN.Esp[ESP_IDX].Routine0 = MAIN.Esp[ESP_IDX].Free2;
}

VOID Esp_36(VOID)
{
    ESP_WORK* p;
    SHORT v;

    p = &ESP_POOL[G.Esp_idx];
    v = 15;
    if (rand() & 1)
    {
        v = 5;
    }
    p->Mov_x = rand() & 1 ? v : -v;
    p = &ESP_POOL[ESP_IDX];
    v = 15;
    if (rand() & 1)
    {
        v = 5;
    }
    p->Mov_y = rand() & 1 ? v : -v;
    p = &ESP_POOL[ESP_IDX];
    v = 15;
    if (rand() & 1)
    {
        v = 5;
    }
    p->Mov_z = rand() & 1 ? v : -v;
}

VOID Esp_19(VOID)
{
    pESP->Hot_x += (SCHAR)pESP->Free0 * 2;  // sign-extends the byte
    pESP->Hot_y += (SCHAR)pESP->Free1 * 2;  // sign-extends the byte
    pESP->Hot_z += (SCHAR)pESP->Free2 * 2;  // sign-extends the byte
    Esp_1c();
}

VOID Esp_21(VOID)
{
    Esp_1f();
    Esp_1c();
}

VOID Esp_22(VOID)
{
    UCHAR Bak_status;

    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0;
    ScratchPad->Vec.vx = pESP->Pos_x;

    ScratchPad->Vec.vy = pESP->Pos_y;
    ScratchPad->Vec.vz = pESP->Pos_z;
    Bak_status = PL_BE_FLG_CUR;
    MAIN.Player.Be_flg = pESP->Free2;
    if (Sca_ck_point_r(&ScratchPad->Vec, &ScratchPad->Sv, 4) != 0 && pESP->Free2 == 0x21)
    {
        Esp_1a();

        Esp_2f();
        MAIN.Player.Be_flg = Bak_status;

        return;
    }
    if (Sca_ck_point_r(&ScratchPad->Vec, &ScratchPad->Sv, 4) == 1)
    {
        Esp_1a();

        Esp_2f();
    }
    G.Player.Be_flg = Bak_status;
}

VOID Esp_25(VOID)
{
    UCHAR Bak_status;

    Bak_status = PL_BE_FLG_CUR;
    MAIN.Player.Be_flg = pESP->Free2;
    ScratchPad->Vec.vx = pESP->Pos_x;
    ScratchPad->Vec.vz = pESP->Pos_z;
    if (Weapon_at_ck(6) != 0)
    {
        Esp_1a();

        Esp_2f();
    }
    MAIN.Player.Be_flg = Bak_status;
}

VOID Esp_23(VOID)
{
    UCHAR t;

    t = pESP->Free3;
    pESP->Free3 = t - 1;
    if (t != 0)
    {
        pESP->Be_flg |= (rand() % 2) << 7;
        pESP->Speed_x =
            (60 - rand() % 6 * 25) * (SCHAR)pESP->Free2;  // sign-extends the byte
        pESP->Add_y = -10 - rand() % 6;
        pESP->Mag -= rand() % 2;
        pESP->DirY = rand() % 6 * 60 * 4096 / 360;
        pESP->Free0 = rand() % 6;
        Esp_1f();
    }
    else
    {
        pESP->Routine0 = 0x11;
        pESP->Free3 = 0;
        pESP->Mat_mode = 2;
        pESP->Mat = *pESP->pMat;
    }
}

VOID Esp_24(VOID)
{
    ScratchPad->Vec.vx = pESP->Pos_x;
    ScratchPad->Vec.vy = pESP->Pos_y;
    ScratchPad->Vec.vz = pESP->Pos_z;
    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0;
    if (pESP->Speed_y <= 0 || Sca_ck_point_r(&ScratchPad->Vec, &ScratchPad->Sv, 0x3C) != 0)
    {
        pESP->Add_x = pESP->Add_x * -1;  // sign-extends the halfword
    }
    if (pESP->Speed_y < (SCHAR)pESP->Free3)  // sign-extends the byte
    {
        pESP->Speed_x =
            (SCHAR)pESP->Free2 * (10 - rand() % 4);  // sign-extends the byte
        pESP->Free3 =
            (pESP->Speed_y = 70 - rand() % 4 * 2) / -3;
        pESP->Speed_z = pESP->Speed_z +
                                                                    (SCHAR)pESP->Free2 * (rand() % 5);  // sign-extends the byte
        pESP->Add_x =
            (SCHAR)pESP->Free2 * (rand() % 3);  // sign-extends the byte
        pESP->Add_y = -10 - rand() % 2;
        pESP->Free2 = (SCHAR)pESP->Free2 * -1;  // sign-extends the byte
    }
}

VOID Esp_27(VOID)
{
    pESP->Free2 = (SCHAR)pESP->Free2 - 1;  // sign-extends the byte
    pESP->Free3 = (SCHAR)pESP->Free3 - 1;  // sign-extends the byte
    if (pESP->Free3 == 0)
    {
        Esp_2f();
        return;
    }
    if (pESP->Free2 == 0)
    {
        pESP->Free2 = 4;

        Esp_call(pESP->Free0, pESP->Free1, pESP->DirY, pESP->pMat, (VECTOR*)&pESP->Org_x, pESP->Mag);  // Org_x, Org_y, Org_z as a VECTOR
    }
}

VOID Esp_2a(VOID)
{
    switch (pESP->Free3)
    {
    case 0:
        pESP->Free0 = G.Player.Be_flg;
        pESP->Free1 = G.Player.Type;
        pESP->Free3++;
        break;
    case 1:
        pESP->Mat_mode = 2;
        pESP->Mat = *pESP->pMat;
        pESP->Free3++;
        break;
    case 2:
        ScratchVec[0] = pESP->Pos_x;
        ScratchPad->Vec.vy = pESP->Pos_y;
        ScratchPad->Vec.vz = pESP->Pos_z;
        Esp_call(8, 1, pESP->DirY, 0,

            &ScratchPad->Vec, 0);

        break;
    }
}

VOID Esp_2b(VOID)
{
    INT unused;
    INT Rsv2;
    INT Next_pos_y;
    UCHAR Status_lo;

    UCHAR Bak_status;
    UCHAR* pStatus = &MAIN.Player.Be_flg;

    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0;
    ScratchPad->Vec.vx = pESP->Pos_x;
    ScratchPad->Vec.vy = pESP->Pos_y;
    ScratchPad->Vec.vz = pESP->Pos_z;

    if (G.Player.Ground < pESP->Pos_y + pESP->Speed_y)
    {
        Esp_impact_set(1);
        return;
    }
    Bak_status = *pStatus;

    Status_lo = Bak_status & 0x1F;
    *pStatus = Status_lo;
    Next_pos_y = pESP->Pos_y + pESP->Speed_y;

    if (G.Player.Ground - 0xAF0 < Next_pos_y)
    {
        if (G.Player.Ground - 0x1F4 < Next_pos_y)
        {
            *pStatus = Status_lo | 0x20;
        }
        else
        {
            *pStatus = Status_lo | 0x40;
        }
    }
    else
    {
        *pStatus = Status_lo | 0x80;
    }

    if (Sca_ck_point_r(&ScratchPad->Vec, &ScratchPad->Sv, 4) == 1)
    {
        G.Player.Be_flg = Bak_status;

        Esp_impact_set(1);
        return;
    }
    if (Weapon_at_ck(pESP->Free1) != 0)
    {
        G.Player.Be_flg = Bak_status;

        Esp_impact_set(0);
        return;
    }

    G.Player.Be_flg = Bak_status;

    if (pESP->Free2++ >= 9U)
    {
        pESP->pAnm_cur = pESP->pAnmseq;
    }
}

VOID Esp_2c(VOID)
{
    ScratchVec[0] = pESP->Hot_x;
    ScratchPad->Vec.vy = pESP->Hot_y;
    ScratchPad->Vec.vz = pESP->Hot_z;
    switch (pESP->Free0)
    {
    case 0:
        Esp_call(1, pESP->Id + 1,
            pESP->DirY,

            pESP->pMat, &ScratchPad->Vec, 0);

        pESP->Free0 = 1;
        return;
    case 1:
        Esp_call(1, pESP->Id + 2,
            pESP->DirY,

            pESP->pMat, &ScratchPad->Vec, 0);

        Esp_call(1, pESP->Id + 3,
            pESP->DirY,

            pESP->pMat, &ScratchPad->Vec, 0);

        pESP->Free0 = 2;
        return;
    case 2:
        Esp_call(1, pESP->Id + 4,
            pESP->DirY,

            pESP->pMat, &ScratchPad->Vec, 0);

        Esp_2f();
        return;
    }
}

VOID Esp_2d(VOID)
{
    INT x;
    UINT z;

    UCHAR save;

    Esp_1a();

    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0;

    ScratchPad->Vec.vx = x = pESP->Pos_x;

    ScratchPad->Vec.vy = pESP->Pos_y;

    ScratchPad->Vec.vz = z = pESP->Pos_z;
    if ((x & 0x80000000) || (z & 0x80000000))
    {
        Esp_kill(7, pESP->Type,
            pESP->Id,
            pESP->Free2, 0);
    }

    if (Sca_ck_point_r(&ScratchPad->Vec, &ScratchPad->Sv, 4) == 1)
    {
        ScratchPad->Vec.vx = pESP->Pos_x;

        ScratchPad->Vec.vy = pESP->Pos_y;
        ScratchPad->Vec.vz = pESP->Pos_z;
        Esp_rocket_set();
        return;
    }
    save = PL_BE_FLG_CUR;
    MAIN.Player.Be_flg = (save & 0x1F) | 0x40;
    if (Weapon_at_ck(0xA) != 0)
    {
        MAIN.Player.Be_flg = save;

        ScratchPad->Vec.vx = pESP->Pos_x;

        ScratchPad->Vec.vy = pESP->Pos_y;
        ScratchPad->Vec.vz = pESP->Pos_z;
        Snd_se_stad(1, 0xF, 4, &ScratchPad->Vec);
        Snd_se_stad(1, 0xF, 0, &ScratchPad->Vec);

        Esp_kill(7, pESP->Type,

            pESP->Id,

            pESP->Free2, 0);

        return;
    }
    MAIN.Player.Be_flg = save;
}

VOID Esp_2e(VOID)
{
    if (pESP->Free2-- == 0)
    {
        Esp_2f();
    }
}

VOID Esp_2f(VOID)
{
    ESP_FREE_CNT += 1;
    pESP->Routine0 = pESP->Routine1 = 0;
}

VOID Esp_31(VOID)
{
    if (ESP_POOL[G.Esp_idx].Be_flg == 6)
    {
        ESP_POOL[G.Esp_idx].Be_flg = 0x8006;
    }
    else
    {
        ESP_POOL[G.Esp_idx].Be_flg = 6;
    }
    if (ESP_POOL[G.Esp_idx].Free1-- == 0)
    {
        Esp_2f();
    }
}

VOID Esp_impact_set(UCHAR Sca_hit)
{
    SCHAR idx;

    ScratchVec[0] = pESP->Pos_x;
    ScratchPad->Vec.vy = pESP->Pos_y;
    ScratchPad->Vec.vz = pESP->Pos_z;
    switch (pESP->Free1 - 7)
    {
    case 0:
        Snd_se_stad(1, 0xC, 4, &ScratchPad->Vec);
        Snd_se_stad(1, 0xC, 0, &ScratchPad->Vec);
        if (Sca_hit)
        {
            ScratchPad->Vec.vy -= 0x96;

            Esp_call(0xE, 3, pESP->DirY, 0, &ScratchPad->Vec, 0x19);

            ScratchPad->Vec.vy += 0x96;
            ScratchPad->Vec.vx -= 0xB4;
            ScratchPad->Vec.vz += 0xB4;

            Esp_call(0xE, 3, pESP->DirY, 0, &ScratchPad->Vec, 0x1E);

            ScratchPad->Vec.vy += 0x96;
            ScratchPad->Vec.vx += 0x154;
            ScratchPad->Vec.vz -= 0x168;

            Esp_call(0xE, 3, pESP->DirY, 0, &ScratchPad->Vec, 0x1C);

            ScratchPad->Vec.vx = pESP->Pos_x;
            ScratchPad->Vec.vy = pESP->Pos_y;
            ScratchPad->Vec.vz = pESP->Pos_z;
            ScratchPad->Vec.vy -= 0xC8;

            idx = Esp_call(9, 0xD, pESP->DirY, 0, &ScratchPad->Vec, 0x14);

            if (idx != -1)
            {
                ESP_POOL[idx].Routine0 = 1;
                ESP_POOL[idx].Be_flg = 0x4003;
                ESP_POOL[idx].Routine1 = 0;
                ESP_POOL[idx].Add_y = -1;
                ESP_POOL[idx].Speed_y = 0;
                ESP_POOL[idx].Speed_x = 0;
                ESP_POOL[idx].Add_x = 0;
            }
        }
        break;
    case 1:
        Snd_se_stad(1, 0xD, 4, &ScratchPad->Vec);
        Snd_se_stad(1, 0xD, 0, &ScratchPad->Vec);
        if (Sca_hit)
        {
            Esp_call(9, 0, pESP->DirY, 0, &ScratchPad->Vec, 0x10);
            Esp_call(9, 0, pESP->DirY + 0x555, 0, &ScratchPad->Vec, 0x10);
            Esp_call(9, 0, pESP->DirY + 0xAAA, 0, &ScratchPad->Vec, 0x10);
            idx = Esp_call(9, 0, pESP->DirY, 0, &ScratchPad->Vec, 0x14);

            if (idx != -1)
            {
                ESP_POOL[idx].Add_x = 0;
                ESP_POOL[idx].Speed_x = 0;
                ESP_POOL[idx].Speed_y = -0x32;
            }
        }
        break;
    case 2:
        Snd_se_stad(1, 0xE, 4, &ScratchPad->Vec);
        Snd_se_stad(1, 0xE, 0, &ScratchPad->Vec);
        if (Sca_hit)
        {
            Esp_call(0xE, 5, pESP->DirY, 0, &ScratchPad->Vec, 0x1B);

            ScratchPad->Vec.vx -= 0xB4;

            Esp_call(0xE, 4, pESP->DirY, 0, &ScratchPad->Vec, 0x17);

            ScratchPad->Vec.vx += 0x140;
            ScratchPad->Vec.vz += 0x8C;

            Esp_call(0xE, 4, pESP->DirY, 0, &ScratchPad->Vec, 0x16);

            ScratchPad->Vec.vz -= 0x118;

            Esp_call(0xE, 4, pESP->DirY, 0, &ScratchPad->Vec, 0x15);
        }
        break;
    }
    Esp_2f();
}

VOID Esp_33(VOID)
{
    if (Flg_ck(&MAIN.Room_flg, 0) != 0)  // address passed as an integer
    {
        ESP_POOL[G.Esp_idx].Routine0 = 1;
        Esp_1f();
    }
}

VOID Esp_30(VOID)
{
    pESP->Anm_time = 0;
    if (pESP->Mag != 0)
    {
        pESP->Mag -= 2;
    }
}

VOID Esp_34(VOID)
{
    UCHAR* idx;

    idx = &MAIN.Esp_idx;
    ESP_POOL[*idx].Mat = *ESP_POOL[*idx].pMat;
    ESP_POOL[*idx].Mat_mode = 2;
    ESP_POOL[*idx].Routine0 = 5;
    Esp_05();
}

VOID Esp_35(VOID)
{
    INT t;

    if (pESP->Pos_y +
            pESP->Speed_y >=
        0)
    {
        pESP->Routine0 = 1;
        pESP->Routine1 = 0;
        pESP->Add_x =
            pESP->Add_y =
                pESP->Add_z = 0;
        pESP->Speed_x =
            pESP->Speed_y =
                pESP->Speed_z = 0;
        t = -(pESP->Hot_y +
              pESP->Mat.t[1]);
        pESP->Mov_y = t;
    }
}

VOID Esp_rocket_set(VOID)
{
    SCHAR r;

    ScratchVec[0] = pESP->Pos_x;
    ScratchPad->Vec.vy = pESP->Pos_y;
    ScratchPad->Vec.vz = pESP->Pos_z;
    Snd_se_stad(1, 0xF, 4, &ScratchPad->Vec);
    Snd_se_stad(1, 0xF, 0, &ScratchPad->Vec);

    Esp_call(0xE, 3, pESP->DirY, 0, &ScratchPad->Vec, 0x1C);
    Esp_call(0xE, 3, pESP->DirY, 0, &ScratchPad->Vec, 0x19);

    ScratchPad->Vec.vy -= 200;

    r = Esp_call(9, 0xD, pESP->DirY, 0, &ScratchPad->Vec, 0x13);

    if (r != -1)
    {
        ESP_POOL[r].Routine0 = 1;
        ESP_POOL[r].Routine1 = 0;
        ESP_POOL[r].Be_flg = 0x4003;
        ESP_POOL[r].Speed_y = 0;
        ESP_POOL[r].Speed_x = 0;
        ESP_POOL[r].Add_x = 0;
    }

    Esp_kill(7, pESP->Type,

        pESP->Id,

        pESP->Free2, 0);
}

VOID Esp_37(INT unused_arg)
{
    INT Next_pos_y;

    pESP->Free0--;
    if (pESP->Free0 == 0)
    {
        pESP->Free0 = 4;
        pESP->pAnm_cur = pESP->pAnmseq + 7;

        pESP->Anm_cnt = *(INT*)pESP->pAnm_cur;  // word view of the same memory

        pESP->Anm_time = pESP->pAnm_cur->Time;

        pESP->pSprg_cur = pESP->Anm_cnt * 4 + (UCHAR*)pESP->pSprg;
    }
    Next_pos_y = pESP->Pos_y + pESP->Speed_y;
    if (Next_pos_y >= -0x960)
    {
        if (Next_pos_y >= 0)
        {
            Esp_splat_set();
            pESP->Be_flg |= 0x4007;
        }
        if (G.Player.Routine_0 == 1)
        {
            if (Pos_pl_at_ck(0x384, pESP->Pos_x, pESP->Pos_z) != 0)
            {
                Esp_splat_set();
            }
        }
    }
}

VOID Esp_38(VOID)
{
    switch (pESP->Free0)
    {
    case 0:
        pESP->pAnm_cur = pESP->pAnmseq;
        pESP->pAnm_cur += 5;
        pESP->Anm_time = pESP->pAnm_cur->Time;
        pESP->pSprg_cur = (UINT*)pESP->pSprg + pESP->pAnm_cur->uv_index;  // pSprg is a table of words
        pESP->Free1 = 0xB;
        pESP->Free0 = 1;
    case 1:
        pESP->Be_flg = 0x4003;
        pESP->Mag = pESP->Free2;
        pESP->Free1--;
        if (pESP->Free1 != 0)
        {
            break;
        }
        pESP->Free0 = 2;
        pESP->Be_flg = 0;
        pESP->Mag = 0;
        break;
    case 2:
        pESP->pAnm_cur = pESP->pAnmseq;
        pESP->pAnm_cur += 5;
        pESP->Anm_time = pESP->pAnm_cur->Time;
        pESP->pSprg_cur = (UINT*)pESP->pSprg + pESP->pAnm_cur->uv_index;  // pSprg is a table of words

        pESP->Free1 = rand() % 5 * 10 + 100;

        pESP->Free0 = 3;
        pESP->Be_flg = 0;
        pESP->Mag = 0;
        break;
    case 3:
        pESP->Be_flg = 0;
        pESP->Mag = 0;
        pESP->Free1--;
        if (pESP->Free1 != 0)
        {
            break;
        }
        pESP->Free0 = 0;
        pESP->Free1 = 0xB;
        break;
    }
}

VOID Esp_39(VOID)
{
    switch (pESP->Free0)
    {
    case 0:
        pESP->Free1 = 1;
        pESP->Free0 = 1;
        break;
    case 1:
        pESP->Mat = *pESP->pMat;
        pESP->Mat_mode = 2;
        pESP->Free1 = 2;
        pESP->Free0 = 2;
        break;
    case 2:
        pESP->Free1++;
        if (pESP->Free1 == 9)
        {
            pESP->Free2--;
            pESP->Free1 = 4;
            pESP->pAnm_cur = pESP->pAnmseq;
            pESP->pAnm_cur += 4;
            pESP->Anm_time = pESP->pAnm_cur->Time;
            pESP->pSprg_cur = (INT*)pESP->pSprg + pESP->pAnm_cur->uv_index;  // pSprg is a table of words
        }
        if (pESP->Free2 == 0)
        {
            pESP->Free0 = 3;
        }
        break;
    }
}

VOID Esp_3a(VOID)
{
    Esp_02();
    ESP_POOL[G.Esp_idx].Free2 = Tama_ck_i() & 3;
}

VOID Esp_3b(VOID)
{
    INT r;

    UCHAR t;

    t = pESP->Free0;
    pESP->Free0 = t - 1;
    if (t == 0)
    {
        if (pESP->Free1 != 0)
        {
            ScratchVec[0] = pESP->Hot_x - (SCHAR)pESP->Free2;  // sign-extends the byte
            ScratchPad->Vec.vy = pESP->Hot_y + 10;
            ScratchPad->Vec.vz = pESP->Hot_z + (SCHAR)pESP->Free3;  // sign-extends the byte
            r = Esp_call(5, 3, pESP->DirY, pESP->pMat, &ScratchPad->Vec, pESP->Mag);
            ScratchWord[0] = r;
            ESP_POOL[r].Free3 = G.Player.Type - 2;
        }
        ScratchVec[0] = pESP->Hot_x + (SCHAR)pESP->Free2;  // sign-extends the byte
        ScratchPad->Vec.vy = pESP->Hot_y;
        ScratchPad->Vec.vz = pESP->Hot_z + (SCHAR)pESP->Free3;  // sign-extends the byte
        r = Esp_call(5, 3, pESP->DirY, pESP->pMat, &ScratchPad->Vec, pESP->Mag);
        ScratchWord[0] = r;
        ESP_POOL[r].Free3 = G.Player.Type - 2;
        Esp_1c();
    }
}
