/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ROOM.C
 */

#include "ROOM.H"
#include "CINESCO.H"
#include "CUT.H"
#include "EFF1.H"
#include "ENEMY.H"
#include "EVENT.H"
#include "GAME.H"
#include "INVENTORY.H"
#include "JOINT.H"
#include "MAP.H"
#include "PLAYER.H"
#include "SCA.H"
#include "SCE0.H"
#include "SCE1.H"
#include "SCE2.H"
#include "SND.H"
#include "ST_SUB03.H"
#include "ZENTAI.H"
#include "FILECODE.H"
#include "FLAG.H"

UCHAR* pInit_scd_top;                // room init script bytecode address
INT* Sce_event_adr;      // room event script address table

static USHORT Room_chg_life;              // main character health saved while the partner is played
static USHORT Room_chg_sca_flg;      // main character collision flags saved while the partner is played

UCHAR Rdt_stage_get(VOID)
{
    UCHAR i = 0;

    UCHAR tbl[7][9] =
    {
        { 0x01, 0x07, 0x0B, 0x0D, 0x12, 0x13, 0x1A, 0x1C, 0xFF },   // 0 //
        { 0x01, 0x02, 0x03, 0x04, 0x12, 0xFF, 0xFF, 0xFF, 0xFF },   // 1 //
        { 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },   // 2 //
        { 0x00, 0x04, 0x05, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },   // 3 //
        { 0x04, 0x05, 0x07, 0x11, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },   // 4 //
        { 0x01, 0x04, 0x07, 0x0B, 0x0D, 0x12, 0x13, 0x1A, 0x1C },   // 5 //
        { 0x01, 0x02, 0x03, 0x04, 0x12, 0x18, 0x1A, 0xFF, 0xFF },   // 6 //
    };

    if (!(G.Status_flg & STAT_ARRANGE))
    {
        return G.SaveData.Stage_no;
    }
    do
    {
        if (tbl[STAGE_NO_CUR][i] != 0xFF && ROOM_NO_CUR == tbl[MAIN.SaveData.Stage_no][i])
        {
            return MAIN.SaveData.Stage_no + 7;
        }
        i++;
    } while (i < 9);
    return G.SaveData.Stage_no;
}

VOID Set_room(VOID)
{
    UINT i;
    ENEMY_WORK* pEm;
    ENEMY_WORK* pEm_prev;
    UCHAR model_id;
    UINT new_id;
    UCHAR t1;
    UCHAR t2;
    UINT Old_id;
    USHORT Life_bak;

    INT Rsv[6];    // unused local: reserves stack space

    G.pFree_work = G.pFree_work_head;
    MAIN.System_flg &= ~SYS_ROOM_CLR_MASK;

    STATUS_FLG_CUR &= ~STAT_SCENE_CLR_MASK;

    Cinesco_fade_clr();
    Sce_aot_init();
    G.Room_flg[1] = 0;
    G.Room_flg[0] = 0;
    i = 0;
    do
    {
        MAIN.Scenario.Sce_task[i].Status = 0;
        i++;
    } while (i < 8);
    G.Player.Sce_flg = 0;
    MAIN.System_flg &= ~SYS_CAM_LOCK;
    G.Player.Sca_info &= 0xBF;
    St_room_set(G.SaveData.Stage_no, G.SaveData.Room_no);
    G.SaveData.Fade1_kido = -1;
    Sce_work_clr();
    VIEW(UCHAR, G.Tpage_clut_bak) = 0;  // UCHAR* view of the field (a plain access changes the code)

    Sce_scr_clr();

    Old_id = G.Player.Id;

    new_id = PL_CHG_NO_CUR;

    if (Old_id != new_id)
    {
        model_id = MAIN.SaveData.Pl_chg_no;

        if (model_id & 8)
        {
            if (STATUS_FLG_CUR & STAT_ARRANGE)
            {
                if (Flg_ck(&MAIN.SaveData.Scenario_flg[0], 0x6F) != 0 || Flg_ck(&MAIN.SaveData.Scenario_flg[0], 0x70) != 0)
                {
                    MAIN.Status_flg |= STAT_PL_ALT_MODEL;
                }
                else
                {
                    MAIN.Status_flg &= ~STAT_PL_ALT_MODEL;
                }
            }
            else
            {
                MAIN.Status_flg |= STAT_PL_ALT_MODEL;
            }
            PL_CHG_NO_CUR &= 7;
        }
        else
        {
            if (!(new_id < 4 && Old_id < 4))
            {
                goto skip;
            }
            G.Player.Id = model_id;

            if (MAIN.SaveData.Pl_chg_no == 3)
            {
                Life_bak = G.Player.Life;

                G.Player.Life = 0x58;

                G.Player.Max_life = 0x58;

                Room_chg_sca_flg = G.Player.Sca_info;

                Room_chg_life = Life_bak;
                if (G.Status_flg & STAT_EASY_MODE)
                {
                    G.Player.Life = 0x8C;
                    G.Player.Max_life = 0x8C;
                }
                if (G.Status_flg & STAT_ARRANGE)
                {
                    G.Player.Life = 0x40;
                    G.Player.Max_life = 0x40;
                }

                G.Player.Sca_info = 0;
                pItem_work_base = (ITEM_WORK*)&MAIN.SaveData.Item_work2[0].Id;  // reinterprets the memory as ITEM_WORK
                t1 = EQUIP_NO_CUR;
                MAIN.SaveData.Equip_no = 0;
                G.SaveData.Equip_bak = t1;
            }
            else
            {
                G.Player.Max_life = 0x8C;

                if (G.Status_flg & STAT_EASY_MODE)
                {
                    G.Player.Max_life = 0xB4;
                }
                if (G.Status_flg & STAT_ARRANGE)
                {
                    G.Player.Max_life = 0x64;
                }

                pItem_work_base = (ITEM_WORK*)&MAIN.SaveData.Item_work[0].Id;  // reinterprets the memory as ITEM_WORK
                G.Player.Sca_info = Room_chg_sca_flg;
                G.Player.Life = Room_chg_life;
                t2 = EQUIP_BAK_CUR;
                MAIN.SaveData.Equip_bak = 0;
                G.SaveData.Equip_no = t2;
            }
        }

        Status_init_m(FREE_WORK_CUR);
        PL_ROUTINE_WORD = 0;
        Player_set();

        Snd_sys_arms_set(G.Player.Type, MAIN.pFree_work);

    skip:

        G.Player.Id = PL_CHG_NO_CUR;
        Snd_sys_enem_set(MAIN.SaveData.Pl_chg_no);
    }

    i = 0;

    Rdt_adr_set();

    Sca_rtn_init();
    Snd_sys_room_set();
    Esp_init_R();

    G.pFree_work = G.pRoom->pVb0;
    if (G.pRoom->nOmodel != 0)
    {
        do
        {
            Omodel_tbl[i] = FREE_WORK_CUR;
            MAIN.pFree_work = Omodel_tbl[i] + 1;
            Omodel_tbl[i]->Be_flg = 0;
            i++;
        } while (i < ROOM_CUR->nOmodel);
    }

    i = 0;
    if (G.pRoom->nItem != 0)
    {
        do
        {
            pOm_tbl[i] = FREE_WORK_CUR;
            MAIN.pFree_work = pOm_tbl[i] + 1;
            pOm_tbl[i]->Be_flg = 0;
            i++;
        } while (i < ROOM_CUR->nItem);
    }

    G.Vram_cur.Tpage_clut = 0xA06;
    ENEMY_NUM_CUR = 0;
    EM_ID_OLD_CUR = 0xFF;
    Omodel_num = 0;
    Imodel_num = 0;

    Sce_scheduler(pInit_scd_top);
    STOP_FLG_CUR |= STOP_SCE;
    Sce_scheduler_main();
    MAIN.Stop_flg &= ~STOP_SCE;
    Snd_bgm_set();
    G.Player.pRoom_kan_t_ptr = G.pRoom->pEmr;
    G.Player.pRoom_seq_t_ptr = G.pRoom->pEdd;
    G.pEm = MAIN.Enemy;

    MAIN.Em_id_old = 0xFF;
    i = 0;
    if (MAIN.Enemy_num != 0)
    {
        do
        {
            pEm = G.pEm;
            if (pEm->Be_flg & EM_BE_ENABLED)
            {
                if (pEm->Id != EM_ID_OLD_CUR)
                {
                    MAIN.Em_id_old = pEm->Id;
                    Emd_load(pEm, pEm->Id + 4);
                }
                else
                {
                    pEm->pKan_t_ptr = pEm_prev->pKan_t_ptr;
                    pEm->pSeq_t_ptr = pEm_prev->pSeq_t_ptr;
                    pEm->pTmd = (UCHAR*)pEm_prev->pTmd - 0xC;
                }
                PartsWork_set(EM_CUR, sizeof(PARTS_W));
                Em_tmd_link(MAIN.pEm->pTmd);
                MAIN.pFree_work = PartsWork_link(FREE_WORK_CUR);
                pEm_prev = MAIN.pEm;
                if (G.System_flg & SYS_MIRROR_MATRIX)
                {
                    Mirror_model_cp();
                }
                i++;
            }
            EM_CUR++;
        } while (i < G.Enemy_num);
    }

    Cinesco_fade_capture((UCHAR)G_BASE->Vram_cur.b.Cclut);
    G.Cccut = G.pRoom->pVcut;
    G.Tpage_clut_bak = G.Vram_cur.Tpage_clut;
    if (!(G.System_flg & SYS_CAM_LOCK))
    {
        G.SaveData.Cut_no = 0;

        Cut_check(1);
    }
    else
    {
        Cut_change();
    }

    if (STAGE_NO_NOW == 2 && G.SaveData.Room_no == 3)
    {
        Snd_pre_adr = FREE_WORK_CUR;
        MAIN.pFree_work = (UCHAR*)MAIN.pFree_work + Snd_bank1_load();
    }
}

VOID Rdt_adr_set(VOID)
{
    RCUT* pRcut;
    INT* pOfs;
    MODEL_LINK_INFO* pOmodel;
    MODEL_LINK_INFO* pImodel;

    UCHAR stage;
    UCHAR room;

    INT i;
    INT base;

    ROOM_CUR = G.pFree_work;
    pRcut = &((ROOM_WORK*)G.pFree_work)->Cut[0];  // reinterprets the memory as ROOM_WORK

    stage = Rdt_stage_get();
    room = G.SaveData.Room_no;
    if (Rdt_same_tbl[stage][room] != 0)
    {
        if ((UCHAR)(stage - 5) < 2 || (UCHAR)(stage - 12) < 2)  // truncates to a byte
        {
            Cd_read(Rdt_tbl[stage][room], MAIN.pRoom);
        }
        else
        {
            Cd_read(Rdt_tbl[stage][room] + 1, MAIN.pRoom);
        }
    }
    else
    {
        if ((UCHAR)(stage - 5) < 2 || (UCHAR)(stage - 12) < 2)  // truncates to a byte
        {
            Cd_read((((UINT)G.System_flg >> 23) & 1) + Rdt_tbl[stage][room], MAIN.pRoom);  // unsigned conversion (signed operand)
        }
        else
        {
            Cd_read((((UINT)G.System_flg >> 23) & 1) + KEEP_VALUE(Rdt_tbl[stage][room] + 1), MAIN.pRoom);  // unsigned conversion (signed operand); KEEP_VALUE keeps the operand a separate value; the plain form and a cached local change the code
        }
    }

    for (i = MAIN.pRoom->nCut; i != 0; i--)
    {
        base = FREE_WORK_INT;
        pRcut->pSp = (VOID*)(base + (INT)pRcut->pSp);
        pRcut->pSp_tim = (VOID*)(base + (INT)pRcut->pSp_tim);
        pRcut++;
    }
    for (pOfs = (INT*)&ROOM_CUR->pVcut; pOfs < (INT*)&ROOM_CUR->Cut; pOfs++)  // the room pointer table as words, relocated one by one
    {
        *pOfs = FREE_WORK_INT + *pOfs;
    }
    pOmodel = G.pRoom->pOmodel;
    for (i = G.pRoom->nOmodel; i != 0; i--, pOmodel++)
    {
        Omodel_tbl[i - 1] = 0;
        if (pOmodel->Tmd_adr != 0)
        {
            pOmodel->Tmd_adr = (VOID*)(FREE_WORK_INT + (INT)pOmodel->Tmd_adr);
        }
        if (pOmodel->Tim_adr != 0)
        {
            pOmodel->Tim_adr = (VOID*)(FREE_WORK_INT + (INT)pOmodel->Tim_adr);
        }
    }
    pImodel = G.pRoom->pImodel;
    for (i = G.pRoom->nItem; i != 0; i--, pImodel++)
    {
        pOm_tbl[i - 1] = 0;
        if (pImodel->Tmd_adr != 0)
        {
            pImodel->Tmd_adr = (VOID*)(FREE_WORK_INT + (INT)pImodel->Tmd_adr);
        }
        if (pImodel->Tim_adr != 0)
        {
            pImodel->Tim_adr = (VOID*)(FREE_WORK_INT + (INT)pImodel->Tim_adr);
        }
    }
    pInit_scd_top = G.pRoom->pScdx;
    pRoom_scd_main = G.pRoom->pScd;
    Sce_event_adr = G.pRoom->pScd1;
    pOfs = Sce_event_adr;
    while (*pOfs != 0)
    {
        *pOfs += (INT)Sce_event_adr;  // real width or sign conversion
        pOfs++;
    }
    GsSetAmbient(G.pRoom->Ambient_r, G.pRoom->Ambient_g, G.pRoom->Ambient_b);
    GsSetLightMode(0);
}

// CD file number of each room file, by stage row and room
USHORT Rdt_tbl[14][32] =
{
    { ROOM100_BSS, ROOM101_BSS, ROOM102_BSS, ROOM103_BSS, ROOM104_BSS, ROOM105_BSS, ROOM106_BSS, ROOM107_BSS, ROOM108_BSS, ROOM109_BSS, ROOM10A_BSS, ROOM10B_BSS, ROOM10C_BSS, ROOM10D_BSS, ROOM10E_BSS, ROOM10F_BSS, ROOM110_BSS, ROOM111_BSS, ROOM112_BSS, ROOM113_BSS, ROOM114_BSS, ROOM115_BSS, ROOM116_BSS, ROOM117_BSS, ROOM118_BSS, ROOM119_BSS, ROOM11A_BSS, ROOM11B_BSS, ROOM11C_BSS, 0x01DE, 0x01DE, 0x01DE },   // 0 //
    { ROOM200_BSS, ROOM201_BSS, ROOM202_BSS, ROOM203_BSS, ROOM204_BSS, ROOM205_BSS, ROOM206_BSS, ROOM207_BSS, ROOM208_BSS, ROOM209_BSS, ROOM20A_BSS, ROOM20B_BSS, ROOM20C_BSS, ROOM20D_BSS, ROOM20E_BSS, ROOM20F_BSS, ROOM210_BSS, ROOM211_BSS, ROOM212_BSS, ROOM213_BSS, ROOM214_BSS, ROOM215_BSS, ROOM216_BSS, ROOM217_BSS, ROOM218_BSS, ROOM219_BSS, ROOM21A_BSS, ROOM21B_BSS, ROOM21C_BSS, 0x01DE, 0x01DE, 0x01DE },   // 1 //
    { ROOM300_BSS, ROOM301_BSS, ROOM302_BSS, ROOM303_BSS, ROOM304_BSS, ROOM305_BSS, ROOM306_BSS, ROOM307_BSS, ROOM308_BSS, ROOM309_BSS, ROOM30A_BSS, ROOM30B_BSS, ROOM30C_BSS, ROOM30D_BSS, ROOM30E_BSS, ROOM30F_BSS, ROOM310_BSS, ROOM311_BSS, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE },   // 2 //
    { ROOM400_BSS, ROOM401_BSS, ROOM402_BSS, ROOM403_BSS, ROOM404_BSS, ROOM405_BSS, ROOM406_BSS, ROOM407_BSS, ROOM408_BSS, ROOM409_BSS, ROOM40A_BSS, ROOM40B_BSS, ROOM40C_BSS, ROOM40D_BSS, ROOM40E_BSS, ROOM40F_BSS, ROOM410_BSS, ROOM411_BSS, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE },   // 3 //
    { ROOM500_BSS, ROOM501_BSS, ROOM502_BSS, ROOM503_BSS, ROOM504_BSS, ROOM505_BSS, ROOM506_BSS, ROOM507_BSS, ROOM508_BSS, ROOM509_BSS, ROOM50A_BSS, ROOM50B_BSS, ROOM50C_BSS, ROOM50D_BSS, ROOM50E_BSS, ROOM50F_BSS, ROOM510_BSS, ROOM511_BSS, ROOM512_BSS, ROOM513_BSS, ROOM514_BSS, ROOM515_BSS, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE },   // 4 //
    { ROOM6000_RDT, ROOM6010_RDT, ROOM6020_RDT, ROOM6030_RDT, ROOM6040_RDT, ROOM6050_RDT, ROOM6060_RDT, ROOM6070_RDT, ROOM6080_RDT, ROOM6090_RDT, ROOM60A0_RDT, ROOM60B0_RDT, ROOM60C0_RDT, ROOM60D0_RDT, ROOM60E0_RDT, ROOM60F0_RDT, ROOM6100_RDT, ROOM6110_RDT, ROOM6120_RDT, ROOM6130_RDT, ROOM6140_RDT, ROOM6150_RDT, ROOM6160_RDT, ROOM6170_RDT, ROOM6180_RDT, ROOM6190_RDT, ROOM61A0_RDT, ROOM61B0_RDT, ROOM61C0_RDT, ROOM1000_RDT, ROOM1000_RDT, ROOM1000_RDT },   // 5 //
    { ROOM7000_RDT, ROOM7010_RDT, ROOM7020_RDT, ROOM7030_RDT, ROOM7040_RDT, ROOM7050_RDT, ROOM7060_RDT, ROOM7070_RDT, ROOM7080_RDT, ROOM7090_RDT, ROOM70A0_RDT, ROOM70B0_RDT, ROOM70C0_RDT, ROOM70D0_RDT, ROOM70E0_RDT, ROOM70F0_RDT, ROOM7100_RDT, ROOM7110_RDT, ROOM7120_RDT, ROOM7130_RDT, ROOM7140_RDT, ROOM7150_RDT, ROOM7160_RDT, ROOM7170_RDT, ROOM7180_RDT, ROOM7190_RDT, ROOM71A0_RDT, ROOM71B0_RDT, ROOM71C0_RDT, ROOM1000_RDT, ROOM1000_RDT, ROOM1000_RDT },   // 6 //
    { ROOM100_BSS, ROOM801_BSS, ROOM102_BSS, ROOM103_BSS, ROOM104_BSS, ROOM105_BSS, ROOM106_BSS, ROOM807_BSS, ROOM108_BSS, ROOM109_BSS, ROOM10A_BSS, ROOM80B_BSS, ROOM10C_BSS, ROOM80D_BSS, ROOM10E_BSS, ROOM10F_BSS, ROOM110_BSS, ROOM111_BSS, ROOM812_BSS, ROOM813_BSS, ROOM114_BSS, ROOM115_BSS, ROOM116_BSS, ROOM117_BSS, ROOM118_BSS, ROOM119_BSS, ROOM81A_BSS, ROOM11B_BSS, ROOM81C_BSS, 0x01DE, 0x01DE, 0x01DE },   // 7 //
    { ROOM200_BSS, ROOM901_BSS, ROOM902_BSS, ROOM903_BSS, ROOM904_BSS, ROOM205_BSS, ROOM206_BSS, ROOM207_BSS, ROOM208_BSS, ROOM209_BSS, ROOM20A_BSS, ROOM20B_BSS, ROOM20C_BSS, ROOM20D_BSS, ROOM20E_BSS, ROOM20F_BSS, ROOM210_BSS, ROOM211_BSS, ROOM912_BSS, ROOM213_BSS, ROOM214_BSS, ROOM215_BSS, ROOM216_BSS, ROOM217_BSS, ROOM918_BSS, ROOM219_BSS, ROOM21A_BSS, ROOM21B_BSS, ROOM21C_BSS, 0x01DE, 0x01DE, 0x01DE },   // 8 //
    { ROOMA00_BSS, ROOM301_BSS, ROOM302_BSS, ROOM303_BSS, ROOM304_BSS, ROOM305_BSS, ROOM306_BSS, ROOM307_BSS, ROOM308_BSS, ROOM309_BSS, ROOM30A_BSS, ROOM30B_BSS, ROOM30C_BSS, ROOM30D_BSS, ROOM30E_BSS, ROOMA0F_BSS, ROOM310_BSS, ROOM311_BSS, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE },   // 9 //
    { ROOMB00_BSS, ROOM401_BSS, ROOM402_BSS, ROOM403_BSS, ROOMB04_BSS, ROOMB05_BSS, ROOM406_BSS, ROOM407_BSS, ROOM408_BSS, ROOM409_BSS, ROOM40A_BSS, ROOM40B_BSS, ROOM40C_BSS, ROOM40D_BSS, ROOM40E_BSS, ROOM40F_BSS, ROOM410_BSS, ROOM411_BSS, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE },   // 10 //
    { ROOM500_BSS, ROOM501_BSS, ROOM502_BSS, ROOM503_BSS, ROOMC04_BSS, ROOMC05_BSS, ROOM506_BSS, ROOMC07_BSS, ROOM508_BSS, ROOM509_BSS, ROOM50A_BSS, ROOM50B_BSS, ROOM50C_BSS, ROOM50D_BSS, ROOM50E_BSS, ROOM50F_BSS, ROOM510_BSS, ROOMC11_BSS, ROOM512_BSS, ROOM513_BSS, ROOM514_BSS, ROOM515_BSS, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE, 0x01DE },   // 11 //
    { ROOM6000_RDT, ROOMD010_RDT, ROOM6020_RDT, ROOM6030_RDT, ROOMD040_RDT, ROOM6050_RDT, ROOM6060_RDT, ROOMD070_RDT, ROOM6080_RDT, ROOM6090_RDT, ROOM60A0_RDT, ROOMD0B0_RDT, ROOM60C0_RDT, ROOMD0D0_RDT, ROOM60E0_RDT, ROOM60F0_RDT, ROOM6100_RDT, ROOM6110_RDT, ROOMD120_RDT, ROOMD130_RDT, ROOM6140_RDT, ROOM6150_RDT, ROOM6160_RDT, ROOM6170_RDT, ROOM6180_RDT, ROOM6190_RDT, ROOMD1A0_RDT, ROOM61B0_RDT, ROOMD1C0_RDT, ROOM1000_RDT, ROOM1000_RDT, ROOM1000_RDT },   // 12 //
    { ROOM7000_RDT, ROOME010_RDT, ROOME020_RDT, ROOME030_RDT, ROOME040_RDT, ROOM7050_RDT, ROOM7060_RDT, ROOM7070_RDT, ROOM7080_RDT, ROOM7090_RDT, ROOM70A0_RDT, ROOM70B0_RDT, ROOM70C0_RDT, ROOM70D0_RDT, ROOM70E0_RDT, ROOM70F0_RDT, ROOM7100_RDT, ROOM7110_RDT, ROOME120_RDT, ROOM7130_RDT, ROOM7140_RDT, ROOM7150_RDT, ROOM7160_RDT, ROOM7170_RDT, ROOME180_RDT, ROOM7190_RDT, ROOME1A0_RDT, ROOM71B0_RDT, ROOM71C0_RDT, ROOM1000_RDT, ROOM1000_RDT, ROOM1000_RDT },   // 13 //
};

// per room: nonzero when the file is the same in both game modes
UCHAR Rdt_same_tbl[14][32] =
{
    { 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 0 //
    { 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00 },   // 1 //
    { 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 2 //
    { 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 3 //
    { 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 4 //
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 5 //
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00 },   // 6 //
    { 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 7 //
    { 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00 },   // 8 //
    { 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 9 //
    { 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 10 //
    { 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 11 //
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // 12 //
    { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00 },   // 13 //
};
