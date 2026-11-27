/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\CD.C
 */

#include "CD.H"
#include <ASSERT.H>
#include <LIBETC.H>
#include <LIBSND.H>
#include "FILECODE.H"
#include "FLAG.H"
#include "MONITOR.H"

UCHAR Cd_status_res[8]; // result bytes of the last drive status query
UINT Xa_start_retry; // number of attempts made to start the XA stream
INT Cd_file_size; // byte size of the file being read

UCHAR Cd_xa_filter; // XA filter file number for the set-filter command
SCHAR Xa_filter_chan; // XA filter channel number, follows the file number

UINT Cd_xa_frame_cnt; // frames since the XA stream was started or last checked

INT Cd_sector; // current sector number
UINT Xa_end_sector; // last sector of the XA clip being played
INT Xa_start_sector; // first sector of the XA clip being played
INT Xa_st_no; // XA state: 0=stopped, 1=starting, 2=playing; index into xa_control_tbl
INT Cd_file_no; // file number of the file being read
INT Cd_stereo_mode; // 1=stereo mix, 0=mono mix
INT Cd_read_busy; // set to 1 when a file read is started
UCHAR* pCd_read_buf; // destination buffer of the file being read
INT Xa_vol_info; // current XA volume level
CdlLOC Xa_start_loc; // disc position at which the XA clip starts
CdlLOC Cd_read_loc; // disc position at which the file read starts
CdlATV Cdvol; // CD audio mix levels passed to CdMix

static INT Xa_fade_in; // volume counter while fading in, and the sync result while initialising
static UCHAR Xa_sync_res[8]; // result bytes of the drive status query while the XA stream starts
static UCHAR Xa_getloc_res[8]; // result bytes of the position and status queries while XA plays
static SCHAR Xa_getloc_pad; // byte after the query result, cleared after each position query

static UCHAR Cd_bss_pad[0x4]; // padding: keeps the following data at its linked position

static INT Xa_fade_out; // volume counter while fading out

VOID Cd_init(VOID)
{
    while (CdInit() == 0)
    {
    }

    G.Check_sum_flg = 0;
    Xa_set_vol(0x80);
    Xa_st_no = 0;
}

VOID Xa_cmd(INT cmd, INT clip)
{
    INT unused[4];
    INT op;
    INT vol;

    op = cmd & 0xFFFF;
    switch (op)
    {
    case 0:
        break;
    case 1:
        Xa_play(clip & 0xFFFF);
        return;
    case 2:
        vol = Xa_vol_info;
        if (vol != 0)
        {
            do
            {
                Set_volume(vol & 0xFF);
                vol -= 1;
            } while (vol != 0);
        }
        Set_volume(0);
        Cd_ctrl_retry(9, 0, 0);
        Xa_st_no = 0;
        MAIN.System_flg &= ~SYS_XA_VOICE;
        return;
    }
}

VOID Cd_nop(VOID)
{
}

INT Cd_file_read(INT file, UINT* dst)
{
    INT unused[2];

    UCHAR mode;

    mode = 0x80;
    Xa_st_no = 0;
    pCd_read_buf = (UCHAR*)dst;  // byte view of the same memory
    Cd_file_no = file;

    Cd_read_busy = 1;

    Cd_sector = Cd_floc_get(file, &Cd_file_size);
    CdIntToPos(Cd_sector, &Cd_read_loc);

loop:

    Cd_ctrl_retry(1, 0, 0);

    if (!(Cd_status_res[0] & 2))
    {
        CdReset(0);
        return 1;
    }

    if (CdControlB(0xE, &mode, 0) == 0)
    {
        goto loop;
    }
    if (CdControlB(2, (UCHAR*)&Cd_read_loc, 0) == 0)  // byte view of the same memory
    {
        goto loop;
    }
    if (CdRead((UINT)(Cd_file_size + 0x7FF) >> 0xB, (ULONG*)dst, mode) == 0)  // unsigned conversion (signed operand)
    {
        goto loop;
    }

    return 0;
}

INT Cd_floc_get(UINT file, INT* pSize)
{
    INT unused;
    if (pSize != 0)
    {
        *pSize = floc[file].size;
    }
    if (file >= 0x3D8U)
    {
        return 0;
    }
    return (floc[file].sec_high << 0x10) | floc[file].sector;
}

INT Cd_sum_ck(VOID)
{
    INT unused[2];
    UCHAR sum;
    INT t;
    UINT i;
    UINT n;
    UCHAR* p;
    UCHAR want;

    if (G.Check_sum_flg != 0)
    {
        return 1;
    }
    sum = 0;
    p = pCd_read_buf;
    i = 0;
    n = Cd_file_size;
    if (n != 0)
    {
        do
        {
            UCHAR c = *p;

            do
            {
                i += 0x200;
            } while (0);
            sum ^= c;
            p += 0x200;
        } while (i < n);
    }
    want = floc[Cd_file_no].sum;
    t = sum;
    if (t != want)
    {
        printf("SUM Error!!%x:%x\n", t, want);

        return 0;
    }

    return 1;
}

INT Cd_bg_sum_ck(VOID)
{
    INT t;
    UCHAR sum;
    INT unused;
    UINT i;
    UCHAR* p;
    UCHAR want;

    sum = 0;
    p = pCd_read_buf;
    i = 0;
    do
    {
        sum ^= *p;
        p += 0x800;
        i += 0x800;
    } while (i <= 0x7FFFU);
    want = *(Monitor.pBs_check_sum + G.SaveData.Room_no * 8 + G.SaveData.Cut_no);
    t = sum;
    if (t != want)
    {
        printf("SUM Error!!%x:%x\n", t, want);

        return 0;
    }
    return 1;
}

VOID Xa_play(UINT clip)
{
    UINT n;
    INT start;
    INT Base_sector;
    INT chan;
    XA_SECTOR_ENT* ent;
    UINT Next_n;

    ent = Monitor.pXa_sector;
    chan = 0;
    n = 0;
    if (clip != 0)
    {
        do
        {
            if (*(USHORT*)ent & 0x8000)  // halfword view of the same memory
            {
                chan += 1;
                ent++;
            }
            Next_n = n + 1;
            n = Next_n;
            ent++;
        } while (Next_n < clip);
    }

    Cd_xa_filter = 1;

    Xa_filter_chan = chan;
    Base_sector = Cd_floc_get(*BYTE_OFS(USHORT, &Xa_no, G.SaveData.Stage_no * 2), &n);
    Xa_start_sector = Base_sector;
    Xa_end_sector = Base_sector;

    start = (*(USHORT*)ent[0].b & 0x7FFF) * 0x10 + chan + Base_sector;  // halfword view of the same memory

    Xa_start_sector = start;

    Xa_end_sector = (*(USHORT*)ent[1].b & 0x7FFF) * 0x10 + chan + Base_sector;  // halfword view of the same memory

    CdIntToPos(start, &Xa_start_loc);
    {
        INT unused[2];
    }

    Xa_start_retry = 0;

    Xa_start();
}

VOID Xa_start(VOID)
{
    UCHAR Cd_mode;

    INT Retry_ok;

    SsSetSerialAttr(0, 0, 1);
    Cd_mode = 0xC8;
    Cd_ctrl_retry(1, 0, 0);
loop_1:

    Retry_ok = Xa_start_retry < 5U;
    Xa_start_retry += 1;
    if (Retry_ok == 0)
    {
        Xa_st_no = 0;
        return;
    }

    if (CdControlB(0xEU, &Cd_mode, 0) == 0 || CdControlB(0xDU, &Cd_xa_filter, 0) == 0 || CdControlB(0x15U, (UCHAR*)&Xa_start_loc, 0) == 0 || CdControl(0x1BU, (UCHAR*)&Xa_start_loc, 0) == 0)  // byte view of the same memory
    {
        VSync(2);

        goto loop_1;
    }

    Cd_xa_frame_cnt = 0;

    Xa_st_no = 1;
}

VOID Xa_set_vol(INT vol)
{
    Xa_vol_info = vol & 0xFF;
}

VOID Set_volume(UCHAR vol)
{
    UINT half;

    if (Cd_stereo_mode == 1)
    {
        Cdvol.val0 = vol;
        Cdvol.val1 = 0;
        Cdvol.val2 = vol;
        Cdvol.val3 = 0;
    }
    else
    {
        half = (vol & 0xFF) >> 1;
        Cdvol.val0 = half;
        Cdvol.val1 = half;
        Cdvol.val2 = half;
        Cdvol.val3 = half;
    }
    CdMix(&Cdvol);
}

VOID Cd_stereo_off(VOID)
{
    Cd_stereo_mode = 0;
    Set_volume(Xa_vol_info);
}

VOID Cd_stereo_on(VOID)
{
    Cd_stereo_mode = 1;
    Set_volume(Xa_vol_info);
}

VOID Xa_control(VOID)
{
    if (Xa_st_no >= 3U)
    {
        Xa_st_no = 0;
    }

    xa_control_tbl[Xa_st_no]();
}

VOID Xa_control_stop(VOID)
{
    MAIN.System_flg &= ~SYS_XA_VOICE;
}

VOID Xa_control_init(VOID)
{
    UCHAR* res;

    UCHAR st;

    Xa_fade_in = CdSync(1, 0);
    if (Xa_fade_in != 0)
    {
        if (Xa_fade_in == 5)
        {
            for (Xa_fade_in = Xa_vol_info; Xa_fade_in > 0; Xa_fade_in--)
            {
                Set_volume(Xa_fade_in);
            }
            Set_volume(0);
            Cd_ctrl_retry(9, 0, 0);
            Xa_st_no = 0;
            SsSetSerialAttr(0, 0, 0);
        }

        res = Xa_sync_res;
        if (CdControlB(1, 0, res) != 0)
        {
            st = *res;

            if ((st & 6) != 2 || Cd_xa_frame_cnt++ >= 0xB5)
            {
                for (Xa_fade_in = Xa_vol_info; Xa_fade_in > 0; Xa_fade_in--)
                {
                    Set_volume(Xa_fade_in);
                }

                Set_volume(0);
                Cd_ctrl_retry(9, 0, 0);
                Xa_st_no = 0;
                SsSetSerialAttr(0, 0, 0);
            }
            else if (!(st & 0x40) && (st & 0x20))
            {
                for (Xa_fade_in = 0; Xa_fade_in < (UINT)Xa_vol_info; Xa_fade_in++)  // unsigned conversion (signed operand)
                {
                    Set_volume(Xa_fade_in);
                }

                Set_volume(Xa_vol_info);

                Cd_xa_frame_cnt = 0;

                Xa_st_no = 2;
                SYSTEM_FLG_CUR |= SYS_XA_VOICE;
            }
        }
    }
}

VOID Xa_control_play(VOID)
{
    INT sector;

    if ((Cd_xa_frame_cnt & 0x1FF) != 0x100)
    {
        if (CdControlB(0x11U, 0, Xa_getloc_res) != 0)
        {
            Xa_getloc_pad = 0;
            sector = CdPosToInt((CdlLOC*)(Xa_getloc_res + 5));  // the response bytes from offset 5 are a CdlLOC position
            Cd_sector = sector;
            if (Xa_end_sector + 0x1194 < sector)
            {
                for (Xa_fade_out = Xa_vol_info; Xa_fade_out != 0; Xa_fade_out--)
                {
                    Set_volume(Xa_fade_out);
                }
                Xa_start();

                return;
            }
            else if (Xa_end_sector < sector)
            {
                for (Xa_fade_out = Xa_vol_info; Xa_fade_out != 0; Xa_fade_out--)
                {
                    Set_volume(Xa_fade_out);
                }
                Set_volume(0);

                Cd_ctrl_retry(9, 0, 0);
                Xa_st_no = 0;
                SsSetSerialAttr(0, 0, 0);
            }
        }
    }
    else if (CdControlB(1U, 0, Xa_getloc_res) != 0 && (Xa_getloc_res[0] & 0x34) != 0x20)
    {
        for (Xa_fade_out = Xa_vol_info; Xa_fade_out != 0; Xa_fade_out--)
        {
            Set_volume(Xa_fade_out);
        }
        Set_volume(0);
        Cd_ctrl_retry(9, 0, 0);
        Xa_st_no = 0;
        SsSetSerialAttr(0, 0, 0);
    }

    if (++Cd_xa_frame_cnt >= 0x8CBU)
    {
        for (Xa_fade_out = Xa_vol_info; Xa_fade_out != 0; Xa_fade_out--)
        {
            Set_volume(Xa_fade_out);
        }
        Set_volume(0);
        Cd_ctrl_retry(9, 0, 0);
        Xa_st_no = 0;
        SsSetSerialAttr(0, 0, 0);
    }
}

INT Cd_ctrl_retry(UCHAR com, UCHAR* param, UCHAR* res)
{
    UCHAR i;

    do
    {
        while (CdSync(1, 0) == 0)
        {
        }
        while (CdControl(1, 0, Cd_status_res) == 0)
        {
        }
    } while (Cd_status_res[0] & 0x50);

    for (i = 0; i < 0x50; i++)
    {
        if (CdControlB(com, param, res) != 0)
        {
            return 1;
        }
    }
    return 0;
}

// CD file index of the voice (XA) file, by stage id
USHORT Xa_no[8] =
{
    VOICE1_XAS,   // 0 //
    VOICE2_XAS,   // 1 //
    VOICE3_XAS,   // 2 //
    VOICE4_XAS,   // 3 //
    VOICE5_XAS,   // 4 //
    VOICE1_XAS,   // 5 //
    VOICE2_XAS,   // 6 //
    0,            // 7 //
};

XA_FUNC xa_control_tbl[3] = { Xa_control_stop, Xa_control_init, Xa_control_play }; // XA state handlers: stop, init, play
