/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SND.C
 */

#include "TYPES.H"
#include <LIBSPU.H>
#include "SND.H"
#include "GAME.H"
#include "GUIDE_CK.H"
#include "SCE0.H"
#include "ST_SUB00.H"
#include "ST_SUB03.H"
#include "FILECODE.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "MONITOR.H"

UCHAR Snd_unused_vg_set[4];              // unused
USHORT Snd_unused_rev2;                    // unused
CHAR Seq_table[516];        // sequence table storage given to the sound library
SndVolume Main_vol;                      // master volume of the BGM fade, saved when it starts and stepped while it runs

UCHAR* Snd_pre_adr;         // preloaded sound bank 1 data, advanced past each part while the bank is opened

SND_VOL_DATA Pan_vol;                // left and right volume of the sound effect being started
INT Snd_unused_err;               // unused
UCHAR* Snd_vh_core;               // VAB header of the core sound bank (VAB 0)
UCHAR* Snd_vab1_head;               // VAB header of the weapon sound bank (VAB 1)
INT* Snd_vh_room;                  // VAB header of the room sound bank (VAB 2)
UCHAR* Snd_vab3_head;               // VAB header of the enemy sound bank (VAB 3)
UCHAR* Bgm_vh;           // VAB header of the BGM bank (VAB 4)

UINT Snd_keyon_mask;                  // voice key-on mask of the sound system
SCHAR Spu_key_stat;                      // start of the SPU voice key status buffer

static INT Bgm_size;                   // size of the BGM bank file being loaded

static UCHAR* pBgm_foot;                // read position in the trailer of the BGM bank file

static UCHAR* pSeqm;                    // address of the SEP data of the BGM bank file
static UCHAR* pVhm;                 // address the VAB header of the BGM bank is copied to

static UCHAR* pVbm;                 // address of the VAB body of the BGM bank file

static SCHAR Fade_end_cnt;             // set when a BGM fade reaches silence, never read

VOID Snd_sys_init(VOID)
{
    Snd_keyon_mask = 0;

    G.Scenario.Vab_id[0] = -1;

    Bgm_status_mask = 0xFF;

    G.Scenario.Vab_id[1] = -1;
    G.Scenario.Vab_id[2] = -1;
    G.Scenario.Vab_id[3] = -1;
    G.Scenario.Vab_id[4] = -1;

    G.Scenario.Sep_no = -1;

    Seqvol[0].left = 0x5F;
    Seqvol[0].right = 0x5F;
    Seqvol[1].left = 0x5F;
    Seqvol[1].right = 0x5F;
    Seqvol[2].left = 0x5F;
    Seqvol[2].right = 0x5F;

    SsInit();
    SsSetTableSize(Seq_table, 1, 3);

    SsSetTickMode(1);

    SsUtSetReverbType(1);
    do
    {
    } while (SpuClearReverbWorkArea(1) == -1);
    SsUtReverbOn();
    SsUtSetReverbDepth(0x1E, 0x1E);
    Snd_sys_start();
}

VOID Snd_sys_start(VOID)
{
    SsStart();
    SsSetMVol(0x7F, 0x7F);
    SsSetRVol(0x50, 0x50);
    SsSetSerialAttr(0, 0, 0);
    SsSetSerialVol(0, 0x7F, 0x7F);
}

VOID Snd_sys_init2(VOID)
{
    SCHAR sep;
    SCHAR Vab_core;
    SCHAR Vab_arms;
    SCHAR Vab_room;
    SCHAR Vab_enem;
    SCHAR Vab_bgm;

    sep = SEP_NO_CUR;
    if (sep >= 0)
    {
        SsSepClose(sep);
        MAIN.Scenario.Sep_no = -1;
    }

    Vab_core = VAB_ID_CUR(0);
    if (Vab_core >= 0)
    {
        SsVabClose(Vab_core);
        MAIN.Scenario.Vab_id[0] = -1;
    }

    Vab_arms = VAB_ID_CUR(1);
    if (Vab_arms >= 0)
    {
        SsVabClose(Vab_arms);
        MAIN.Scenario.Vab_id[1] = -1;
    }

    Vab_room = VAB_ID_CUR(2);
    if (Vab_room >= 0)
    {
        SsVabClose(Vab_room);
        MAIN.Scenario.Vab_id[2] = -1;
    }

    Vab_enem = VAB_ID_CUR(3);
    if (Vab_enem >= 0)
    {
        SsVabClose(Vab_enem);
        MAIN.Scenario.Vab_id[3] = -1;
    }

    Vab_bgm = VAB_ID_CUR(4);
    if (Vab_bgm >= 0)
    {
        SsVabClose(Vab_bgm);
        MAIN.Scenario.Vab_id[4] = -1;
    }
    SsUtReverbOff();
    SsUtSetReverbType(0);
    SsUtSetReverbDepth(0, 0);
    SsEnd();
    SsQuit();
}

VOID Snd_title_init(VOID)
{
    Snd_sys_init();
}

VOID Snd_vab3_close(VOID)
{
    SCHAR vab;

    vab = VAB_ID_CUR(3);
    if (vab >= 0)
    {
        SsVabClose(vab);
        MAIN.Scenario.Vab_id[3] = -1;
    }
}

VOID Snd_sys_end(VOID)
{
    SsEnd();
    SsQuit();
}

VOID Snd_sys_stereo(UINT sorm)
{
    CdlATV volume;

    if (sorm & 0xFF)
    {
        SsSetStereo();
        volume.val0 = 0x7F;
        volume.val1 = 0;
        volume.val2 = 0x7F;
        volume.val3 = 0;
    }
    else
    {
        SsSetMono();
        volume.val0 = 0x3F;
        volume.val1 = 0x3F;
        volume.val2 = 0x3F;
        volume.val3 = 0x3F;
    }
    CdMix(&volume); // pointer re-typed; type-only conversion
}

VOID Snd_sys_core_set(UCHAR core_type_no)
{
    SCHAR* pVab;
    UCHAR* pData;

    USHORT* pEdh;

    USHORT* pVbd;
    USHORT* pTbl;

    INT idx;

    pVab = MAIN.Scenario.Vab_id;
    if (*pVab >= 0)
    {
        SsVabClose(*pVab);
        *pVab = -1;
    }

    pData = (UCHAR*)Core_edt_buf; // fixed buffer address as typed pointer

    idx = core_type_no;

    do
    {
        pTbl = Core_edh;
    } while (0);
    pEdh = pTbl + idx;
open_retry:
    Cd_read(*pEdh, pData);
    Snd_vh_core = pData + ((INT*)((USHORT)floc[*pEdh].size + (INT)pData))[-2]; // pointer re-typed; type-only conversion
    if ((G.Scenario.Vab_id[0] = SsVabOpenHeadSticky(Snd_vh_core, 0, 0x1020)) == -1)
    {
        goto open_retry;
    }
    pTbl = Core_vbd;
    pVbd = pTbl + core_type_no;
loop2:
    Cd_read(*pVbd, SND_VB_BUF);
    if (SsVabTransBody((UCHAR*)SND_VB_BUF, G.Scenario.Vab_id[0]) == -1) // fixed buffer address as typed pointer
    {
        goto loop2;
    }
loop3:
    Task_sleep(1);
    if (SsVabTransCompleted(0) == 0)
    {
        goto loop3;
    }
}

VOID Snd_sys_arms_set(INT arms_type_no, UCHAR* pVb_buf)
{
    UCHAR* pData;

    UCHAR* pVh;

    if (VAB_ID_CUR(1) >= 0)
    {
        SsVabClose(MAIN.Scenario.Vab_id[1]);
        MAIN.Scenario.Vab_id[1] = -1;
    }

    pData = (UCHAR*)Arms_edt_buf; // fixed buffer address as typed pointer
    do
    {
        Cd_read(Arms_edh[arms_type_no & 0xFF], pData);
        pVh = pData + *(INT*)(pData + (USHORT)floc[Arms_edh[arms_type_no & 0xFF]].size - 8); // pointer re-typed; type-only conversion
        Snd_vab1_head = pVh;
    } while ((G.Scenario.Vab_id[1] = SsVabOpenHeadSticky(pVh, 1, 0x7300)) == -1);
    do
    {
        Cd_read(Arms_vbd[arms_type_no & 0xFF], pVb_buf);
    } while (SsVabTransBody(pVb_buf, G.Scenario.Vab_id[1]) == -1);
    do
    {
        Task_sleep(1);
    } while (SsVabTransCompleted(0) == 0);
}

VOID Snd_sys_room_set(VOID)
{
    SCHAR vab;

    SCHAR New_id;

    Snd_vh_room = G.pRoom->pVh0;

    vab = VAB_ID_CUR(2);
    if (vab >= 0)
    {
        SsVabClose(vab);
        MAIN.Scenario.Vab_id[2] = -1;
    }
    if (*Snd_vh_room != 0)
    {
        do
        {
            New_id = SsVabOpenHeadSticky((UCHAR*)Snd_vh_room, 2, 0xE800U); // pointer re-typed; type-only conversion
            G.Scenario.Vab_id[2] = New_id;
        } while (New_id == -1);

        do
        {
        } while (SsVabTransBody(G.pRoom->pVb0, G.Scenario.Vab_id[2]) == -1);
        do
        {
            Task_sleep(1);
        } while ((SsVabTransCompleted(0) << 0x10) == 0);
    }
}

VOID Snd_sys_enem_set(INT enem_type_no)
{
    UCHAR* pData;

    USHORT* pFile;

    UCHAR* pVh;

    VOID** ppLoad;

    USHORT* pTbl;

    INT idx;

    if (VAB_ID_CUR(3) >= 0)
    {
        SsVabClose(MAIN.Scenario.Vab_id[3]);
        MAIN.Scenario.Vab_id[3] = -1;
    }

    pData = (UCHAR*)SND_EDT_BUF; // fixed buffer address as typed pointer

    idx = enem_type_no & 0xFF;

    do
    {
        pTbl = Enem_edh;
    } while (0);
    pFile = pTbl + idx;
open_retry:
    Cd_read(*pFile, pData);
    pVh = pData + *(INT*)(pData + (USHORT)floc[*pFile].size - 8); // pointer re-typed; type-only conversion
    Snd_vab3_head = pVh;
    if ((G.Scenario.Vab_id[3] = SsVabOpenHeadSticky(pVh, 3, 0x37800)) == -1)
    {
        goto open_retry;
    }

    pTbl = Enem_vbd;
    pFile = pTbl + (enem_type_no & 0xFF);
    ppLoad = &MAIN.pFree_work;
loop2:
    Cd_read(*pFile, *ppLoad);
    if (SsVabTransBody(*ppLoad, G.Scenario.Vab_id[3]) == -1)
    {
        goto loop2;
    }
    do
    {
        Task_sleep(1);
    } while (SsVabTransCompleted(0) == 0);
}

VOID Snd_bgm_main(INT Bgm_cnf)
{
    UCHAR* pDatam2;
    UCHAR* pDatam;
    INT File_len;
    INT i;
    SCHAR id;

loop:

    if (VAB_ID_CUR(4) >= 0)
    {
        SsVabClose(MAIN.Scenario.Vab_id[4]);
        MAIN.Scenario.Vab_id[4] = -1;
    }
    Bgm_vh = pSeqm = pVhm = Bgm_main_vh_buf;
    pDatam2 = FREE_WORK_CUR;
    i = Bgm_cnf & 7;
    pVbm = pDatam2;
    pBgm_foot = pDatam2;
    Cd_read(Bgm_bank_file[(MON_BGM_ROOM_TBL + ROOM_NO_CUR * 4)[i]], pDatam2);
    File_len = floc[Bgm_bank_file[(Monitor.pBgm_room_tbl + MAIN.SaveData.Room_no * 4)[i]]].size;
    Bgm_size = File_len;
    pDatam = pBgm_foot + Bgm_size;
    pBgm_foot = pDatam - 8;
    pSeqm += *(INT*)(pDatam - 8);
    pBgm_foot = pDatam - 0xC;
    pVbm += *(INT*)(pDatam - 0xC);
    Memcpy(pVhm, MAIN.pFree_work, *(INT*)(pDatam - 0xC));

    id = SsVabOpenHeadSticky(Bgm_main_vh_buf, 4, 0x408B0); // fixed buffer address as typed pointer
    MAIN.Scenario.Vab_id[4] = id;
    if (id == -1)
    {
        goto loop;
    }
    if (SsVabTransBody(pVbm, id) == -1)
    {
        goto loop;
    }

    do
    {
        Task_sleep(1);
    } while (SsVabTransCompleted(0) == 0);

    Snd_bgm_sep_open(Bgm_cnf, pSeqm);
}

VOID Snd_bgm_sep_open(UCHAR Bgm_cnf, VOID* pSep)
{
    INT i;

    i = 3;

    if (SEP_NO_CUR >= 0)
    {
        SsSepClose(MAIN.Scenario.Sep_no);
        MAIN.Scenario.Sep_no = -1;
    }
    while ((MAIN.Scenario.Sep_no = SsSepOpen(pSep, G.Scenario.Vab_id[4], i)) == -1)  // word view of the same memory
    {
    }
    do
    {
        i--;

        SsSepSetVol(SEP_NO_CUR, i, Seqvol[i].left, Seqvol[i].right);

        SsSepPlay(MAIN.Scenario.Sep_no, i, 0, 1);
    } while (i != 0);
}

VOID Snd_bgm_play_ck(VOID)
{
    if (BGM_TGT_STS_CUR != 0xFF)
    {
        if (MAIN.Scenario.Bgm_tgt_sts & 8)
        {
            SsSepReplay(G.Scenario.Sep_no, 0); // callee declared with another prototype; cast gives the call type
            Bgm_status_mask |= 8;
        }

        if (MAIN.Scenario.Bgm_tgt_sts & 0x10)
        {
            SsSepReplay(G.Scenario.Sep_no, 1); // callee declared with another prototype; cast gives the call type
            Bgm_status_mask |= 0x10;
        }

        if (MAIN.Scenario.Bgm_tgt_sts & 0x20)
        {
            SsSepReplay(G.Scenario.Sep_no, 2); // callee declared with another prototype; cast gives the call type
            Bgm_status_mask |= 0x20;
        }
    }
}

VOID Snd_bgm_set(VOID)
{
    INT unused[2];
    UCHAR cur;

    UCHAR prev;

    UCHAR* pTarget;

    pTarget = &MAIN.Scenario.Bgm_tgt_sts;

    cur = G.Bgm_tbl_adr[ROOM_NO_CUR];

    prev = Bgm_status_mask;

    BGM_STS_OLD_CUR = prev;
    *pTarget = cur;

    if (MAIN.Scenario.Bgm_tgt_sts == 0xFF)
    {
        if (MAIN.Scenario.Bgm_sts_old != 0xFF)
        {
            Snd_bgm_fade_all();
            Bgm_status_mask = 0xFF;
        }

        STATUS_FLG_CUR &= ~STAT_DOOR_WAIT;

        return;
    }

    if (*(Monitor.pBgm_room_tbl + MAIN.SaveData.Room_no * 4 + (cur & 7)) != *(Monitor.pBgm_room_tbl + G.SaveData.Room_no_old * 4 + (prev & 7)) && MAIN.Scenario.Bgm_sts_old != 0xFF && (prev & 0x38))
    {
        Snd_bgm_fade_all();

        STATUS_FLG_CUR &= ~STAT_DOOR_WAIT;
    }

    switch (BGM_TGT_STS_CUR >> 6)
    {
    case 0:
    {
        UCHAR* pRoom;
        UCHAR* pPrevRoom;
        UCHAR a;

        USHORT b;

        INT t;

        if (BGM_STS_OLD_CUR == 0xFF)
        {
            STATUS_FLG_CUR &= ~STAT_DOOR_WAIT;
            Snd_bgm_main(MAIN.Scenario.Bgm_tgt_sts);

            Snd_bgm_play_ck();
            break;
        }

        a = MAIN.Scenario.Bgm_tgt_sts;

        b = MAIN.Scenario.Bgm_sts_old;

        pRoom = &MAIN.SaveData.Room_no;
        pPrevRoom = &MAIN.SaveData.Room_no_old;
        if (*(Monitor.pBgm_room_tbl + *pRoom * 4 + (a & 7)) != *(Monitor.pBgm_room_tbl + *pPrevRoom * 4 + (b & 7)))
        {
            Snd_bgm_fade_all();

            STATUS_FLG_CUR &= ~STAT_DOOR_WAIT;
            Snd_bgm_main(MAIN.Scenario.Bgm_tgt_sts);

            Snd_bgm_play_ck();
            break;
        }

        STATUS_FLG_CUR &= ~STAT_DOOR_WAIT;

        if (*(Monitor.pBgm_room_tbl + *pRoom * 4 + (a & 7)) != *(Monitor.pBgm_room_tbl + *pPrevRoom * 4 + (b & 7)))
        {
            break;
        }

        t = a & 8;
        if (t != (b & 8))
        {
            if (t)
            {
                if (!SsIsEos(SEP_NO_CUR, 0))
                {
                    SsSepPlay(MAIN.Scenario.Sep_no, 0, 1, 1);
                }
            }
            else if (SsIsEos(SEP_NO_CUR, 0))
            {
                SsSepStop(MAIN.Scenario.Sep_no, 0);
            }
        }
        if ((G.Scenario.Bgm_tgt_sts & 0x10) != (G.Scenario.Bgm_sts_old & 0x10))
        {
            if (G.Scenario.Bgm_tgt_sts & 0x10)
            {
                if (!SsIsEos(SEP_NO_CUR, 1))
                {
                    SsSepPlay(MAIN.Scenario.Sep_no, 1, 1, 1);
                }
            }
            else if (SsIsEos(SEP_NO_CUR, 1))
            {
                SsSepStop(MAIN.Scenario.Sep_no, 1);
            }
        }
        if ((G.Scenario.Bgm_tgt_sts & 0x20) != (G.Scenario.Bgm_sts_old & 0x20))
        {
            if (G.Scenario.Bgm_tgt_sts & 0x20)
            {
                if (!SsIsEos(SEP_NO_CUR, 2))
                {
                    SsSepPlay(MAIN.Scenario.Sep_no, 2, 1, 1);
                }
            }
            else if (SsIsEos(SEP_NO_CUR, 2))
            {
                SsSepStop(MAIN.Scenario.Sep_no, 2);
            }
        }
        break;
    }

    case 1:
        Snd_bgm_fade_all();

        STATUS_FLG_CUR &= ~STAT_DOOR_WAIT;
        Snd_bgm_main(MAIN.Scenario.Bgm_tgt_sts);

        break;
    case 2:

        MAIN.Scenario.Bgm_tgt_sts &= 0x7F;

        Snd_bgm_fade_all();

        STATUS_FLG_CUR &= ~STAT_DOOR_WAIT;
        Snd_bgm_main(MAIN.Scenario.Bgm_tgt_sts);
        if ((MAIN.Scenario.Bgm_tgt_sts >> 6) == 0)
        {
            Snd_bgm_play_ck();
        }
        break;
    }

    STATUS_FLG_CUR &= ~STAT_DOOR_WAIT;

    Bgm_status_mask = G.Scenario.Bgm_tgt_sts;
}

VOID Snd_bgm_play(SHORT seq)
{
    SsSepPlay(G.Scenario.Sep_no, seq, 1, 1);
}

VOID Snd_bgm_stop(SHORT seq)
{
    if (G.Scenario.Bgm_tgt_sts != 0xFF)
    {
        SsSepStop(G.Scenario.Sep_no, seq);
    }
}

VOID Snd_bgm_vol_set(SHORT seq, SHORT Vol_l, SHORT Vol_r)
{
    SsSepSetVol(G.Scenario.Sep_no, seq, Vol_l, Vol_r);
}

VOID Snd_se_stad(UCHAR Edt_no, UCHAR SENo, INT flg, VOID* pVec)
{
    EDT_TABLE_WORK* pSseff;
    UCHAR* pVh;
    SHORT Vid;

    INT ok;

    switch (Edt_no)
    {
    case 0:

        if (SENo >= 2)
        {
            return;
        }

        pSseff = (EDT_TABLE_WORK*)Core_edt_buf; // buffer address viewed as an EDT_TABLE_WORK record

        Vid = G.Scenario.Vab_id[0]; // width or sign conversion

        pVh = Snd_vh_core;
        break;
    case 1:

        if (G.Status_flg & STAT_CARD_SE)
        {
            ok = SENo < 0x10;
        }
        else
        {
            ok = SENo < 0x30;
        }
        if (!ok)
        {
            return;
        }

        pSseff = (EDT_TABLE_WORK*)Arms_edt_buf; // buffer address viewed as an EDT_TABLE_WORK record

        Vid = G.Scenario.Vab_id[1]; // width or sign conversion

        pVh = Snd_vab1_head;
        break;
    case 2:

        if (SENo >= 0x30)
        {
            return;
        }
        Vid = G.Scenario.Vab_id[2]; // width or sign conversion

        pVh = (UCHAR*)Snd_vh_room; // pointer re-typed; type-only conversion
        pSseff = G.pRoom->pEdt0;
        break;
    case 3:

        if (SENo >= 0x10)
        {
            return;
        }

        pSseff = (EDT_TABLE_WORK*)SND_EDT_BUF; // buffer address viewed as an EDT_TABLE_WORK record

        Vid = G.Scenario.Vab_id[3]; // width or sign conversion

        pVh = Snd_vab3_head;
        break;
    case 4:

        if (SENo >= 0x30)
        {
            return;
        }
        Vid = G.Scenario.Vab_id[4]; // width or sign conversion

        pVh = Bgm_vh;
        pSseff = G.pRoom->pEdt0;
        break;

    default:
        return;
    }
    pSseff += SENo;
    pVh += (pSseff->prog << 9) + KEEP_VALUE((pSseff->tone << 5) + 0x820);  // KEEP_VALUE keeps the operand a separate value; the plain form and a cached local change the code
    Snd_se_3D(pVec);

    Snd_keyon_mask |= 1 << SsUtKeyOn(Vid, pSseff->prog, pSseff->tone, VAG_ATR(pVh)->min, VAG_ATR(pVh)->shift, Pan_vol.left, Pan_vol.right);
}

static const UINT Snd_pad_mid = 0;    // unused constant between the functions

VOID Snd_se_play(Edt_no, SENo, Pan)
UCHAR Edt_no;  // narrow parameter, passed as int
UCHAR SENo;  // narrow parameter, passed as int
UCHAR Pan;  // narrow parameter, passed as int
{
    SHORT Vid;

    UCHAR* pVh;
    EDT_TABLE_WORK* pSseff;

    SCHAR Pno;
    SCHAR Tno;

    switch (Edt_no)
    {
    case 0:

        if (SENo >= 2)
        {
            return;
        }
        Vid = G.Scenario.Vab_id[0]; // width or sign conversion

        pVh = Snd_vh_core;

        pSseff = (EDT_TABLE_WORK*)Core_edt_buf; // buffer address viewed as an EDT_TABLE_WORK record

        break;
    case 1:

        if (!(G.Status_flg & STAT_CARD_SE ? SENo < 0x10 : SENo < 0x30))
        {
            return;
        }
        Vid = G.Scenario.Vab_id[1]; // width or sign conversion

        pVh = Snd_vab1_head;

        pSseff = (EDT_TABLE_WORK*)Arms_edt_buf; // buffer address viewed as an EDT_TABLE_WORK record

        break;
    case 2:

        if (SENo >= 0x30)
        {
            return;
        }
        Vid = G.Scenario.Vab_id[2]; // width or sign conversion

        pVh = (UCHAR*)Snd_vh_room; // pointer re-typed; type-only conversion
        pSseff = G.pRoom->pEdt0;
        break;
    case 3:

        if (SENo >= 0x10)
        {
            return;
        }

        pSseff = (EDT_TABLE_WORK*)SND_EDT_BUF; // buffer address viewed as an EDT_TABLE_WORK record

        Vid = G.Scenario.Vab_id[3]; // width or sign conversion

        pVh = Snd_vab3_head;
        break;
    case 4:

        if (SENo >= 0x30)
        {
            return;
        }
        Vid = G.Scenario.Vab_id[4]; // width or sign conversion

        pVh = Bgm_vh;
        pSseff = G.pRoom->pEdt0;
        break;

    default:
        return;
    }
    pSseff += SENo;
    Pno = pSseff->prog;
    Tno = pSseff->tone;
    pVh += (Pno << 9) + KEEP_VALUE((Tno << 5) + 0x820);  // KEEP_VALUE keeps the operand a separate value; the plain form and a cached local change the code

    Snd_keyon_mask |= 1 << SsUtKeyOn(Vid, Pno, Tno, VAG_ATR(pVh)->min, VAG_ATR(pVh)->shift, VAG_ATR(pVh)->vol, VAG_ATR(pVh)->vol);
}

VOID Snd_se_walk(UCHAR no, INT Pan)
{
    INT kutu;
    INT v;
    INT x;

    EDT_TABLE_WORK* pSseff;
    UCHAR* pVh;

    if (no < 3)
    {
        pSseff = G.pRoom->pEdt0;
        if ((G.System_flg & SYS_CLIMB) && G.Player.Ground == -0x708)
        {
            no += 0x23;
        }
        else
        {
            kutu = Floor_check(G.pEm->Pos_x, G.pEm->Pos_z);
            x = no;
            if (G.Status_flg & STAT_WATER)
            {
                {
                    INT step_no = x - 3;
                    v = step_no + kutu;
                }
            }
            else
            {
                v = x + kutu;
            }

            no = v;
        }
        if (no < 0x30)
        {
            pSseff += no;
            pVh = (UCHAR*)Snd_vh_room; // pointer re-typed; type-only conversion
            pVh += (pSseff->prog << 9) + KEEP_VALUE((pSseff->tone << 5) + 0x820);  // KEEP_VALUE keeps the operand a separate value; the plain form and a cached local change the code
            Snd_se_3D(POS_VEC(G.pEm));
            Snd_keyon_mask |= 1 << SsUtKeyOn(G.Scenario.Vab_id[2], pSseff->prog, pSseff->tone, VAG_ATR(pVh)->min, VAG_ATR(pVh)->shift, Pan_vol.left, Pan_vol.right);
        }
    }
}

VOID Snd_se_enem(UCHAR SENo, INT Pan)
{
    ENEMY_WORK* pEm;
    EDT_TABLE_WORK* pSseff;
    UCHAR* pVh;
    UCHAR slot;

    if (SENo < 10)
    {
        pVh = (UCHAR*)Snd_vh_room; // pointer re-typed; type-only conversion
        pEm = G.pEm;
        pSseff = G.pRoom->pEdt0;

        slot = pEm->Slot_info;
        slot = (slot & 0x70) >> 4;
        SENo += slot * 10;
        if (SENo < 0x30)
        {
            pSseff += SENo;
            pVh += (pSseff->prog << 9) + KEEP_VALUE((pSseff->tone << 5) + 0x820);  // KEEP_VALUE keeps the operand a separate value; the plain form and a cached local change the code
            Snd_se_3D(POS_VEC(pEm));
            Snd_keyon_mask |= 1 << SsUtKeyOn(G.Scenario.Vab_id[2], pSseff->prog, pSseff->tone, VAG_ATR(pVh)->min, VAG_ATR(pVh)->shift,
                                        Pan_vol.left, Pan_vol.right);
        }
    }
}

VOID Snd_voice_ck(VOID)
{
    INT voice;

    SpuGetAllKeysStatus(&Spu_key_stat);
    voice = 0;
    do
    {
        if ((&Spu_key_stat)[voice] == 0)
        {
            if ((1 << voice) == 0)
            {
                Snd_keyon_mask &= 1;
                voice += 1;
            }
            else
            {
                Snd_keyon_mask = 0;
                goto next_voice;
            }
        }
        else
        {
        next_voice:
            voice += 1;
        }
    } while (voice >= 0x18);
}

VOID Snd_key_off_all(VOID)
{
    SpuSetKey(0, Snd_keyon_mask);
    Snd_keyon_mask = 0;
}

VOID Snd_se_3D(VECTOR* pVec)
{
    INT d;
    INT Sesz;
    INT Sedir;

    UINT a;

    SHORT x;
    USHORT y;
    SHORT ck_flg1;
    SHORT ck_flg0;

    INT q;

    INT k;
    SHORT vol0;
    SHORT vol1;
    INT t;

    INT e;

    ScratchSv[0] = ROOM_CUR->Cut[CUT_NO_CUR].View.vpx - pVec->vx; // plain Sv field changes scheduling

    ScratchPad->Sv.vz = MAIN.pRoom->Cut[MAIN.SaveData.Cut_no].View.vpz - pVec->vz;

    d = SquareRoot0(ScratchPad->Sv.vx * ScratchPad->Sv.vx +

                    ScratchPad->Sv.vz * ScratchPad->Sv.vz);

    Sedir = ScratchPad->Sv.vx * ScratchPad->Sv.vx;
    e = SquareRoot0(Sedir + ScratchPad->Sv.vz * ScratchPad->Sv.vz);

    t = View_ref.vpy - ScratchPad->Sv.vy;
    Sesz = SquareRoot0(t * t + d * e);
    a = Direction_ck(MAIN.pRoom->Cut[MAIN.SaveData.Cut_no].View.vpx,

        MAIN.pRoom->Cut[MAIN.SaveData.Cut_no].View.vpz,
        pVec->vx, pVec->vz);

    t = a - Direction_ck(MAIN.pRoom->Cut[MAIN.SaveData.Cut_no].View.vpx,

                MAIN.pRoom->Cut[MAIN.SaveData.Cut_no].View.vpz,
                MAIN.pRoom->Cut[MAIN.SaveData.Cut_no].View.vrx,
                MAIN.pRoom->Cut[MAIN.SaveData.Cut_no].View.vrz);

    Sedir = t & 0xFFF;

    x = Sedir;
    y = Sedir;
    if (x != 0 && x != 0x1000 && x != 0x800)
    {
        ck_flg0 = 0;
        if (y > 0x800)
        {
            y = ~Sedir;
            y &= 0x7FF;

            ck_flg0 = 1;
            ck_flg1 = 0;
        }
        else
        {
            ck_flg1 = 1;
        }
        if (y > 0x400)
        {
            y = ~y;
        }

        a = y & 0x7FF;

        if (a > 0x40)
        {
            q = (INT)a / (Sesz / 2000 + 24); // width or sign conversion

            vol0 = q + 0x7F;
            vol1 = 0x7F - q;

            Pan_vol.right = ck_flg0 * ((vol1 < 30) * 30 + (vol1 >= 30) * vol1) + ck_flg1 * ((vol0 > 0x7F) * 0x7F + (vol0 < 0x80) * vol0);
            Pan_vol.left = ck_flg0 * ((vol0 >= 0x80) * 0x7F + (vol0 < 0x80) * vol0) + ck_flg1 * ((vol1 < 30) * 30 + (vol1 >= 30) * vol1);

            goto done;
        }
    }
    Pan_vol.right = 0x7F;
    Pan_vol.left = 0x7F;
done:

    Pan_vol.left -= k = Sesz / 500;
    Pan_vol.right -= k;
    if (Pan_vol.left < 30)
    {
        Pan_vol.left = 30;
    }

    if (Pan_vol.right < 30)
    {
        Pan_vol.right = 30;
    }
    Pan_vol.left &= 0x7F;
    Pan_vol.right &= 0x7F;
}

INT Snd_bgm_fade_ON(time, mode)
SCHAR time;  // narrow parameter, passed as int
SCHAR mode;  // narrow parameter, passed as int
{
    SsGetMVol(&Main_vol);
    G.Scenario.Bgm_fade_add = time;
    G.Fade_rtn = mode;
}

VOID Snd_bgm_fade(VOID)
{
    SCHAR v;
    SCHAR w;

    v = FADE_RTN_CUR;
    if (v < 0)
    {
        w = v + 1;
        MAIN.Fade_rtn = w;
        if (w == -1)
        {
            MAIN.Fade_rtn = 0;
            SsSetMVol(0x7F, 0x7F);
        }
        else if (w == -2)
        {
            if (SEP_NO_CUR >= 0)
            {
                SsSepClose(MAIN.Scenario.Sep_no);
                MAIN.Scenario.Sep_no = -1;
            }
            if (VAB_ID_CUR(4) >= 0)
            {
                SsVabClose(MAIN.Scenario.Vab_id[4]);
                MAIN.Scenario.Vab_id[4] = -1;
            }
        }
        else if (w == -4)
        {
            SsUtAllKeyOff(0);
        }
        else if (w == -29)
        {
            if (SEP_NO_CUR >= 0)
            {
                if (SsIsEos(MAIN.Scenario.Sep_no, 0))
                {
                    SsSepStop(MAIN.Scenario.Sep_no, 0);
                }
                if (SsIsEos(MAIN.Scenario.Sep_no, 1))
                {
                    SsSepStop(MAIN.Scenario.Sep_no, 1);
                }
                if (SsIsEos(MAIN.Scenario.Sep_no, 2))
                {
                    SsSepStop(MAIN.Scenario.Sep_no, 2);
                }
            }
        }
    }
    else
    {
        Main_vol.left += G.Scenario.Bgm_fade_add; // width or sign conversion
        Main_vol.right += G.Scenario.Bgm_fade_add; // width or sign conversion
        if (((SCHAR)Main_vol.left < 1) & ((SCHAR)Main_vol.right < 1)) // width or sign conversion
        {
            Main_vol.right = 0;
            Main_vol.left = 0;

            MAIN.Fade_rtn = -30;

            Fade_end_cnt = 30;
        }
        SsSetMVol(Main_vol.left, Main_vol.right);
    }
}

VOID Snd_bgm_fade_all(VOID)
{
    INT i;

    if (SEP_NO_CUR != -1)
    {
        i = 2;
        if ((SsIsEos(MAIN.Scenario.Sep_no, 0) << 16) != 0)
        {
            SsSepSetCrescendo(MAIN.Scenario.Sep_no, 0, -0x5F, 2);
        }

        if ((SsIsEos(MAIN.Scenario.Sep_no, 1) << 16) != 0)
        {
            SsSepSetCrescendo(MAIN.Scenario.Sep_no, 1, -0x5F, 2);
        }

        if ((SsIsEos(MAIN.Scenario.Sep_no, 2) << 16) != 0)
        {
            SsSepSetCrescendo(MAIN.Scenario.Sep_no, 2, -0x5F, 2);
        }

        do
        {
            i--;

            Task_sleep(1);
        } while (i != 0);

        if ((SsIsEos(SEP_NO_CUR, 0) << 16) != 0)
        {
            SsSepStop(MAIN.Scenario.Sep_no, 0);
        }

        if ((SsIsEos(MAIN.Scenario.Sep_no, 1) << 16) != 0)
        {
            SsSepStop(MAIN.Scenario.Sep_no, 1);
        }
        if ((SsIsEos(MAIN.Scenario.Sep_no, 2) << 16) != 0)
        {
            SsSepStop(MAIN.Scenario.Sep_no, 2);
        }

        i = 8;

        do
        {
            i--;
            Task_sleep(1);
        } while (i != 0);

        SsUtAllKeyOff(0);

        i = 2;

        do
        {
            i--;
            Task_sleep(1);
        } while (i != 0);
        if (SEP_NO_CUR >= 0)
        {
            SsSepClose(MAIN.Scenario.Sep_no);
            MAIN.Scenario.Sep_no = -1;
        }
        if (VAB_ID_CUR(4) >= 0)
        {
            SsVabClose(MAIN.Scenario.Vab_id[4]);
            MAIN.Scenario.Vab_id[4] = -1;
        }
    }
}

INT Snd_bank1_load(VOID)
{
    INT* base = (INT*)Snd_pre_adr; // pointer re-typed; type-only conversion
    USHORT* pEdh1 = &Arms_edh[10];
    USHORT* id2;

    INT size;
    INT off;
    UINT o;

    Cd_read(*pEdh1, base + 1);

    size = (USHORT)floc[*pEdh1].size; // width or sign conversion

    id2 = &Arms_vbd[10];

    off = size + 4;

    o = off & 0xFFFF;
    do
    {
        *base = size;
    } while (0);
    Cd_read(*id2, (UCHAR*)base + o); // pointer re-typed; type-only conversion
    return ((USHORT)floc[*id2].size + off) & 0xFFFF; // width or sign conversion
}

VOID Snd_vab1_load(VOID)
{
    INT Edh_size;
    UCHAR* buf;
    SCHAR vab;
    SCHAR New_id;
    USHORT Edh_size16;
    UCHAR* pVh_pre;
    UCHAR* pEdh_src;
    UCHAR* pVb_pre;

    vab = VAB_ID_CUR(1);
    if (vab >= 0)
    {
        SsVabClose(vab);
        MAIN.Scenario.Vab_id[1] = -1;
    }
    Edh_size16 = *(SHORT*)Snd_pre_adr; // pointer re-typed; type-only conversion
    pEdh_src = Snd_pre_adr + 4;
    Snd_pre_adr = pEdh_src;
    Edh_size = Edh_size16 & 0xFFFF;
    buf = (UCHAR*)Arms_edt_buf; // fixed buffer address as typed pointer
    Memcpy(buf, pEdh_src, Edh_size);
    pVh_pre = *(INT*)((UCHAR*)SND_VH_OFS_TBL + Edh_size) + buf; // fixed buffer address as typed pointer
    Snd_vab1_head = pVh_pre;
    New_id = SsVabOpenHeadSticky(pVh_pre, 1, 0x7300U);
    MAIN.Scenario.Vab_id[1] = New_id;
    pVb_pre = &Snd_pre_adr[Edh_size];
    Snd_pre_adr = pVb_pre;
    SsVabTransBody(pVb_pre, New_id);
    SsVabTransCompleted(1);
}

// BGM bank file codes
USHORT Bgm_bank_file[58] =
{
    SEP00_HSB,    // 0 //
    SEP01_HSB,    // 1 //
    SEP02_HSB,    // 2 //
    SEP03_HSB,    // 3 //
    SEP04_HSB,    // 4 //
    SEP05_HSB,    // 5 //
    SEP06_HSB,    // 6 //
    SEP07_HSB,    // 7 //
    SEP08_HSB,    // 8 //
    SEP09_HSB,    // 9 //
    SEP0A_HSB,   // 10 //
    SEP0B_HSB,   // 11 //
    SEP0C_HSB,   // 12 //
    SEP0D_HSB,   // 13 //
    SEP0E_HSB,   // 14 //
    SEP0F_HSB,   // 15 //
    SEP10_HSB,   // 16 //
    SEP11_HSB,   // 17 //
    SEP12_HSB,   // 18 //
    SEP13_HSB,   // 19 //
    SEP14_HSB,   // 20 //
    SEP15_HSB,   // 21 //
    SEP16_HSB,   // 22 //
    SEP17_HSB,   // 23 //
    SEP18_HSB,   // 24 //
    SEP19_HSB,   // 25 //
    SEP1A_HSB,   // 26 //
    SEP1B_HSB,   // 27 //
    SEP1C_HSB,   // 28 //
    SEP1D_HSB,   // 29 //
    SEP1E_HSB,   // 30 //
    SEP1F_HSB,   // 31 //
    SEP20_HSB,   // 32 //
    SEP21_HSB,   // 33 //
    SEP22_HSB,   // 34 //
    SEP23_HSB,   // 35 //
    SEP24_HSB,   // 36 //
    SEP25_HSB,   // 37 //
    SEP26_HSB,   // 38 //
    SEP27_HSB,   // 39 //
    SEP28_HSB,   // 40 //
    SEP29_HSB,   // 41 //
    SEP2A_HSB,   // 42 //
    SEP2B_HSB,   // 43 //
    SEP2C_HSB,   // 44 //
    SEP2D_HSB,   // 45 //
    SEP2E_HSB,   // 46 //
    SEP2F_HSB,   // 47 //
    SEP30_HSB,   // 48 //
    SEP31_HSB,   // 49 //
    SEP32_HSB,   // 50 //
    SEP33_HSB,   // 51 //
    SEP34_HSB,   // 52 //
    SEP35_HSB,   // 53 //
    SEP36_HSB,   // 54 //
    SEP37_HSB,   // 55 //
    SEP38_HSB,   // 56 //
    0x0000,            // 57 //
};

// core sound bank header file codes, by core type
USHORT Core_edh[16] =
{
    DOORWOD_HED,    // 0 //
    DOORMTL_HED,   // 1 //
    DOORBRK_HED,   // 2 //
    DOORREB_HED,   // 3 //
    DOORSTJ_HED,   // 4 //
    DOORSTW_HED,   // 5 //
    DOORELV_HED,   // 6 //
    DOORNEW_HED,   // 7 //
    DOORGAT_HED,   // 8 //
    DOORSMT_HED,   // 9 //
    DOOROLD_HED,   // 10 //
    DOORLAD_HED,   // 11 //
    DOORAIR_HED,   // 12 //
    DOORLAB_HED,   // 13 //
    DOORFTN_HED,   // 14 //
    0x0000,             // 15 //
};

// core sound bank body file codes, by core type
USHORT Core_vbd[16] =
{
    DOORWOD_VB,    // 0 //
    DOORMTL_VB,   // 1 //
    DOORBRK_VB,   // 2 //
    DOORREB_VB,   // 3 //
    DOORSTJ_VB,   // 4 //
    DOORSTW_VB,    // 5 //
    DOORELV_VB,   // 6 //
    DOORNEW_VB,   // 7 //
    DOORGAT_VB,   // 8 //
    DOORSMT_VB,   // 9 //
    DOOROLD_VB,   // 10 //
    DOORLAD_VB,   // 11 //
    DOORAIR_VB,   // 12 //
    DOORLAB_VB,   // 13 //
    DOORFTN_VB,   // 14 //
    0x0000,             // 15 //
};

// weapon sound bank header file codes, by arms type
USHORT Arms_edh[16] =
{
    WP001_HED,    // 0 //
    WP001_HED,    // 1 //
    WP002_HED,    // 2 //
    WP003_HED,    // 3 //
    WP004_HED,    // 4 //
    WP005_HED,    // 5 //
    WP006_HED,    // 6 //
    WP007_HED,    // 7 //
    WP008_HED,    // 8 //
    WP009_HED,    // 9 //
    WP00A_HED,    // 10 //
    BIO_HED,   // 11 //
    EVIL_HED,    // 12 //
    SELECT_HED,    // 13 //
    ENDING_HED,    // 14 //
    0x0000,             // 15 //
};

// weapon sound bank body file codes, by arms type
USHORT Arms_vbd[16] =
{
    WP001_VB,    // 0 //
    WP001_VB,    // 1 //
    WP002_VB,    // 2 //
    WP003_VB,    // 3 //
    WP004_VB,    // 4 //
    WP005_VB,    // 5 //
    WP006_VB,    // 6 //
    WP007_VB,    // 7 //
    WP008_VB,    // 8 //
    WP009_VB,    // 9 //
    WP00A_VB,    // 10 //
    BIO_VB,   // 11 //
    EVIL_VB,    // 12 //
    SELECT_VB,    // 13 //
    ENDING_VB,    // 14 //
    0x0000,             // 15 //
};

// enemy sound bank header file codes, by enemy type
USHORT Enem_edh[8] =
{
    CHAR00_HED,   // 0 //
    CHAR01_HED,   // 1 //
    CHAR02_HED,   // 2 //
    CHAR02_HED,   // 3 //
    CHAR03_HED,   // 4 //
    CHAR04_HED,   // 5 //
    CHAR05_HED,   // 6 //
    CHAR05_HED,   // 7 //
};

// enemy sound bank body file codes, by enemy type
USHORT Enem_vbd[8] =
{
    CHAR00_VB,   // 0 //
    CHAR01_VB,   // 1 //
    CHAR02_VB,   // 2 //
    CHAR02_VB,   // 3 //
    CHAR03_VB,   // 4 //
    CHAR04_VB,   // 5 //
    CHAR05_VB,   // 6 //
    CHAR05_VB,   // 7 //
};
