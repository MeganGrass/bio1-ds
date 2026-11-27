/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\STR.C
 */

#include "TYPES.H"
#include <LIBETC.H>
#include <LIBPRESS.H>
#include <LIBSN.H>
#include "STR.H"

#include "CD.H"

INT Str_unused_ring;     // unused
static UCHAR Sync_ret[4]; // result bytes of the CD sync wait in Close_movie

static UCHAR Str_pad0[0x8]; // padding: keeps the following data at its linked position

static ULONG* Rl_buff[2];           // the two VLC decode buffers, filled alternately
static VOID* pImage_buff;               // buffer receiving the decoded slices
static UCHAR Str_pad1[0x4];  // padding: keeps the following data at its linked position
static INT End_frame;               // last frame number of the movie
static UINT Isrgb24;               // nonzero = 24-bit output, 0 = 16-bit output
static INT Cbuf_id;            // index of the VLC buffer that holds the frame being decoded
static UCHAR Str_pad2[0x10]; // padding: keeps the following data at its linked position
static UCHAR Str_pad3[0x38]; // unused
static StHEADER Frm_head;       // header of the frame last VLC-decoded, with width and height rounded up to 16
static INT Start_sector;               // start sector of the movie file
static RECT Slice_rect;             // VRAM rectangle of the slice being uploaded (overlays STR_SLICE_WORK)
static UINT Str_pad4;              // padding word of the slice state
static INT Slice_size;              // size of one decoded slice, in words
static UINT Slice_idx;            // number of the slice being decoded
static UINT Slice_cnt;            // slices per frame
static volatile INT Mdec_flg;    // nonzero while a slice decode is in progress
static INT Str_frame;           // number of the frame most recently handed to the MDEC by Play_movie

static INT Old_no; // frame number of the frame played before the current one

static INT Pic_w;      // frame width in pixels
static INT Str_height;     // frame height in pixels
static CdlLOC Loc;     // disc position used by Cd_seek_pause
static CdlFILE Str_cdfile; // CD file entry of the movie opened by name
static CdlATV Cd_atv;   // CD audio mixer levels

static STR_GETLOC_BUF Getloc_ret; // reply of the CD position query

static UINT* pRing; // stream ring buffer given to Str_open

static UCHAR* Pc_frm_buf[2];           // frame buffers of the PC-file stream
static INT Pc_frm_start;            // first frame to play from the PC-file stream
static UINT Pc_frm_end;             // last frame of the PC-file stream
static VOID (*pPc_open_cb)();    // called after each frame has been read from the PC file
static INT (*pPc_end_cb)();    // called when the end frame or the end of the PC file is reached
static StHEADER Pc_sec_head[2]; // sector header of the frame read into each PC-file frame buffer
static INT Pc_buf_idx;              // index of the PC-file frame buffer in use
static INT Pc_fd;                    // file handle of the PC-file stream
static INT Pc_sub_flg;         // nonzero when the PC file stores sectors with CD sub-headers
static INT Pc_sub_head[8];          // sub-header bytes read before a sector
static INT Pc_eof_flg;                   // nonzero once the PC file is exhausted or the end frame was read
static INT Pc_sec_cnt;          // sectors read for the last frame

static INT Str_pad5; // padding word after the sector count

CD_AUDIO_DATA Cd_audio = { 0x80, 1, 0 }; // CD audio volume, stereo flag and decode-skip flag

INT Str_open(SCHAR* fname, UINT Frm_start, INT Frm_end, UINT* pRing_buf, UINT Ring_len, UINT* pRl_buf0, UINT* pRl_buf1, VOID* pSlice_buf, UINT flags)
{
    INT unused[2];

    ULONG* addr;
    StHEADER* header;

    UINT i;

    pRing = pRing_buf;

    End_frame = Frm_end;
    Cbuf_id = 0;
    Mdec_flg = 0;
    Str_frame = 0;

    Old_no = 0;

    Isrgb24 = flags;
    Rl_buff[0] = (ULONG*)pRl_buf0;  // word view of the same memory
    Rl_buff[1] = (ULONG*)pRl_buf1;  // word view of the same memory
    pImage_buff = pSlice_buf;
    Cd_audio.Skip_flg = (flags >> 1) & 1;
    DecDCTReset(0);
    DecDCToutCallback(Mdec_callback);

    while (CdControlB(8, 0, 0) == 0)
    {
    }
    if (CdSearchFile(&Str_cdfile, fname) == 0)
    {
        return 1;
    }

    StSetRing((ULONG*)pRing_buf, Ring_len);  // word view of the same memory
    StSetStream(1, Frm_start, 0x0FFFFFFF, 0, 0);
    if (Str_cd_start(fname) != 0)
    {
        return 1;
    }

    i = 0;

    while (StGetNext(&addr, (ULONG**)&header) != 0)  // word view of the same memory
    {
        VSync(0);
        i++;
        if (i > 600)
        {
            return 1;
        }
    }
    Frm_head = *header;
    Frm_head.width = (Frm_head.width + 8) / 16 * 16;
    Frm_head.height = (Frm_head.height + 8) / 16 * 16;
    DecDCTvlc(addr, Rl_buff[Cbuf_id]);
    StFreeRing(addr);
    return 0;
}

INT Open_movie(INT file_id, UINT Frm_start, INT Frm_end, ULONG* pRing_buf, UINT Ring_len, ULONG* pRl_buf0, ULONG* pRl_buf1, VOID* pSlice_buf, UINT flags)
{
    CdlLOC loc;

    UCHAR Param[4];
    UCHAR result[8];

    INT size;
    ULONG* pBs_buff;
    StHEADER* pRead_header;

    INT v;
    UINT pos;

    UINT Time_out;

    End_frame = Frm_end;
    Cbuf_id = 0;
    Mdec_flg = 0;
    Str_frame = 0;

    Old_no = 0;

    Isrgb24 = flags;
    Rl_buff[0] = pRl_buf0;
    Rl_buff[1] = pRl_buf1;
    pImage_buff = pSlice_buf;
    Cd_audio.Skip_flg = (flags >> 1) & 1;
    DecDCTReset(0);
    DecDCToutCallback(Mdec_callback);
    StSetRing(pRing_buf, Ring_len);
    StSetStream(1, Frm_start, 0x0FFFFFFF, 0, 0);
    Start_sector = Cd_floc_get(file_id, &size);
    CdIntToPos(Start_sector, &loc);
    Movie_xa_stop();
    Param[0] = 1;
    Param[1] = 1;

    do
    {
        do
        {
        retry:
            if (CdSync(0, 0) != 2)
            {
                do
                {
                } while (Cd_recover_a8() == 0);

                goto retry;
            }
        } while (CdControlB(0xD, Param, 0) == 0 || CdControlB(0x15, (UCHAR*)&loc, 0) == 0);  // byte view of the same memory
    } while (CdRead2(0x1C8) == 0);

    do
    {
    } while (CdControlB(0x11, 0, Getloc_ret.result) == 0);
    pos = CdPosToInt(&Getloc_ret.position);
    v = Start_sector;
    if (pos < v - 100 || v + 5 < pos)
    {
        return 1;
    }

    Time_out = 0;

    do
    {
        if (StGetNext(&pBs_buff, (ULONG**)&pRead_header) == 0)  // word view of the same memory
        {
            goto found;
        }

        VSync(0);
        Time_out++;
    } while (Time_out < 301);
    return 1;
found:
    Movie_xa_start();

    Frm_head = *pRead_header;
    Frm_head.width = (Frm_head.width + 8) / 16 * 16;
    Frm_head.height = (Frm_head.height + 8) / 16 * 16;
    Pic_w = Frm_head.width;
    Str_height = Frm_head.height;
    DecDCTvlc(pBs_buff, Rl_buff[Cbuf_id]);
    StFreeRing(pBs_buff);
    return 0;
}

INT Play_movie(INT Dst_x_flg, SHORT dst_y)
{
    INT unused[2];

    ULONG* pBs_buff;
    StHEADER* pRead_header;
    UINT Time_out;
    INT Ret;

    INT h;

    INT t;
    INT u;

    USHORT x;

    Time_out = 0;

    while (StGetNext(&pBs_buff, (ULONG**)&pRead_header) != 0)  // word view of the same memory
    {
        VSync(0);
        Time_out++;
        if (Time_out >= 180)
        {
            return 1;
        }
    }

    Pic_w = Frm_head.width;
    Str_height = Frm_head.height;

    Str_frame = Frm_head.frameCount;

    if (Frm_head.frameCount >= End_frame || Frm_head.frameCount < Old_no)
    {
        Ret = 1;
    }
    else
    {
        Ret = 0;
    }
    Old_no = Str_frame;

    if (Dst_x_flg & 0x400)
    {
        Cd_audio.Skip_flg = 1;
    }
    if (Dst_x_flg & 0x800)
    {
        Cd_audio.Skip_flg = 0;
    }
    Slice_rect.x = Dst_x_flg & 0x3FF;
    Slice_rect.y = dst_y;
    t = 0x10;
    if (Isrgb24)
    {
        t = 0x18;
    }
    x = Str_height;
    Slice_rect.w = t;

    h = Frm_head.height;

    Slice_idx = 0;
    Slice_rect.h = x;

    Slice_cnt = (UINT)Pic_w >> 4;  // unsigned conversion (signed operand)
    u = h << 4;

    if (Isrgb24)
    {
        Slice_size = ((h << 5) + u) / 4;
    }
    else
    {
        Slice_size = h << 3;
    }
    DecDCTReset(1);
    if (Cd_audio.Skip_flg == 0)
    {
        DecDCTin(Rl_buff[Cbuf_id], Isrgb24);
        DecDCTout(pImage_buff, Slice_size);
    }
    Cbuf_id = Cbuf_id == 0;
    Frm_head = *pRead_header;
    Frm_head.width = (Frm_head.width + 8) / 16 * 16;
    Frm_head.height = (Frm_head.height + 8) / 16 * 16;
    DecDCTvlc(pBs_buff, Rl_buff[Cbuf_id]);
    StFreeRing(pBs_buff);
    return Ret;
}

VOID Close_movie(VOID)
{
    INT sync;

    Movie_xa_stop();

    DecDCToutCallback(0);
    CdDataCallback(0);
    CdReadyCallback(0);

    StUnSetRing();

    do
    {
        sync = CdSync(1, Sync_ret);
    } while (sync == 0);
    if (sync == 5)
    {
        do
        {
        } while (Cd_recover_90() == 0);
    }
}

VOID Str_decode_wait(VOID)
{
    while (Mdec_flg != 0)
    {
    }
}

INT Str_end_ck(VOID)
{
    return (UINT)Str_frame >= End_frame;  // unsigned conversion (signed operand)
}

INT Str_frm_get(VOID)
{
    return Str_frame;
}

INT Str_cd_start(VOID)
{
    UCHAR param[8];

    Movie_xa_stop();

    param[0] = 1;
    param[1] = 1;
    Start_sector = CdPosToInt(&Str_cdfile.pos);

    do
    {
        do
        {
        inner:
            if (CdSync(0, 0) == 5)
            {
                while (Cd_recover_a8() == 0)
                {
                }
                goto inner;
            }
        } while (CdControlB(0xD, param, 0) == 0 || CdControlB(0x15, (UCHAR*)&Str_cdfile, 0) == 0);  // byte view of the same memory
    } while (CdRead2(0x1C8) == 0);

    Movie_xa_start();
    return 0;
}

VOID Mdec_callback(VOID)
{
    Slice_idx += 1;
    if (StCdIntrFlag != 0)
    {
        StCdInterrupt();
        StCdIntrFlag = 0;
    }
    LoadImage(&Slice_rect, pImage_buff);
    if (Slice_idx < Slice_cnt)
    {
        Slice_rect.x += Isrgb24 ? 24 : 16;
        DecDCTout(pImage_buff, ((STR_SLICE_WORK*)&Slice_rect)->Size);
        Mdec_flg = 1;
    }
    else
    {
        Mdec_flg = 0;
    }
}

VOID Fwd_movie(INT Frame)
{
    CdlLOC loc;

    StClearRing();

    CdIntToPos(Start_sector + Frame * 0xA, &loc);

    do
    {
    wait_cd:
        if (CdSync(1, 0) != 2)
        {
            goto wait_cd;
        }

        if (CdControlB(9, 0, 0) == 0)
        {
            goto wait_cd;
        }
        if (CdControl(0x15, (UCHAR*)&loc, 0) == 0)
        {
            goto wait_cd;
        }
    } while (CdRead2(0x1C8) == 0);
}

VOID Movie_xa_start(VOID)
{
    INT half;
    INT Mono_vol;
    UINT level;

    if (Cd_audio.stereo != 0)
    {
        level = 0;
        do
        {
            Cd_atv.val0 = level;
            Cd_atv.val1 = 0;
            Cd_atv.val2 = level;
            Cd_atv.val3 = 0;
            CdMix(&Cd_atv);
            level += 1;
        } while (Cd_audio.volume >= level);
        return;
    }
    Mono_vol = 0;
    do
    {
        half = Mono_vol / 2;
        Cd_atv.val0 = half;
        Cd_atv.val1 = half;
        Cd_atv.val2 = half;
        Cd_atv.val3 = half;
        CdMix(&Cd_atv);
        Mono_vol += 1;
    } while (Cd_audio.volume >= (UINT)Mono_vol);  // unsigned conversion (signed operand)
}

VOID Movie_xa_stop(VOID)
{
    INT half;
    INT level;
    INT Mono_vol;

    if (Cd_audio.stereo != 0)
    {
        level = Cd_audio.volume;
        if (level >= 0)
        {
            do
            {
                Cd_atv.val0 = level;
                Cd_atv.val1 = 0;
                Cd_atv.val2 = level;
                Cd_atv.val3 = 0;
                CdMix(&Cd_atv);
                level -= 2;
            } while (level >= 0);
        }
    }
    else
    {
        Mono_vol = Cd_audio.volume;
        if (Mono_vol >= 0)
        {
            do
            {
                half = Mono_vol / 2;
                Cd_atv.val0 = half;
                Cd_atv.val1 = half;
                Cd_atv.val2 = half;
                Cd_atv.val3 = half;
                CdMix(&Cd_atv);
                Mono_vol -= 2;
            } while (Mono_vol >= 0);
        }
    }
}

VOID Set_movie_volume2(UINT Volume)
{
    UINT half;

    Cd_audio.volume = Volume;
    if (Cd_audio.stereo != 0)
    {
        Cd_atv.val0 = Volume;
        Cd_atv.val1 = 0;
        Cd_atv.val2 = Volume;
        Cd_atv.val3 = 0;
    }
    else
    {
        half = Volume >> 1;
        Cd_atv.val0 = half;
        Cd_atv.val1 = half;
        Cd_atv.val3 = Cd_atv.val2 = half;
    }
    CdMix(&Cd_atv);
}

VOID Cd_set_stereo(INT mode)
{
    UINT half;

    Cd_audio.stereo = mode;
    if (mode != 0)
    {
        Cd_atv.val1 = 0;
        Cd_atv.val3 = 0;
        Cd_atv.val0 = Cd_audio.volume;
        Cd_atv.val2 = Cd_audio.volume;
    }
    else
    {
        half = (UINT)Cd_audio.volume >> 1;  // unsigned conversion (signed operand)
        Cd_atv.val0 = half;
        Cd_atv.val1 = half;
        Cd_atv.val2 = half;
        Cd_atv.val3 = half;
    }
    CdMix(&Cd_atv);
}

INT Cd_recover_90(VOID)
{
    UCHAR res[8];
    UCHAR com[8];

    com[0] = 0x90;
    CdControlB(0xE, com, res);
    if (res[0] & 0x15)
    {
        return 0;
    }

    VSync(3);
    Cd_seek_pause();
    return 1;
}

INT Cd_recover_a8(VOID)
{
    UCHAR res[8];
    UCHAR com[8];

    com[0] = 0xA8;
    CdControlB(0xE, com, res);
    if (res[0] & 0x15)
    {
        return 0;
    }

    VSync(3);
    Cd_seek_pause();
    return 1;
}

VOID Cd_seek_pause(VOID)
{
    INT size;

    CdIntToPos(Cd_floc_get(0, &size), &Loc);
    do
    {
    wait_cd:
        if (CdSync(1, 0) != 2)
        {
            goto wait_cd;
        }
        if (CdControlB(0x15, (UCHAR*)&Loc, 0) == 0)
        {
            goto wait_cd;
        }
    } while (CdControlB(9, 0, 0) == 0);
}

VOID Mdec_img_decode(ULONG* pBs, ULONG* pRl_buf, ULONG* pOut_buf, INT w, INT h, INT Rgb24_flg)
{
    UINT Pix_num;
    UINT Out_size;

    DecDCTReset(0);
    DecDCTvlc(pBs, pRl_buf);
    DecDCTin(pRl_buf, Rgb24_flg);
    Pix_num = w * h;
    if (Rgb24_flg != 0)
    {
        Out_size = (Pix_num * 3) >> 2;
    }
    else
    {
        Out_size = Pix_num >> 1;
    }
    DecDCTout(pOut_buf, Out_size);
    DecDCToutSync(0);
}

VOID Mdec_vram_decode(RECT* pDst_rect, UINT* pBs, UINT* pRl_buf, UINT* pSlice_buf, INT Rgb24_flg)
{
    RECT r;
    INT step;
    UINT total;
    UINT i;

    DecDCTReset(0);
    DecDCToutCallback(0);
    DecDCTvlc((ULONG*)pBs, (ULONG*)pRl_buf);  // word view of the same memory
    DecDCTin((ULONG*)pRl_buf, Rgb24_flg);  // word view of the same memory
    step = 0x10;
    if (Rgb24_flg != 0)
    {
        step = 0x18;
    }
    r.x = pDst_rect->x;
    r.y = pDst_rect->y;
    r.w = step;
    r.h = pDst_rect->h;
    total = (UINT)(pDst_rect->w * step) >> 4;  // unsigned conversion (signed operand)
    for (i = 0; i < total; i += step)
    {
        DecDCTout((ULONG*)pSlice_buf, (UINT)(step * (pDst_rect->h << 1)) >> 2);  // word view of the same memory
        DecDCToutSync(0);
        LoadImage(&r, (ULONG*)pSlice_buf);  // word view of the same memory
        DrawSync(0);
        r.x += step;
    }
}

VOID Mdec_img_upload(RECT* src, UINT* buf, INT Rgb24_flg)
{
    RECT r;
    UINT w;
    UINT n;
    UINT i;
    INT h;
    INT t;
    INT stride;
    UINT* p;

    w = 0x10;
    if (Rgb24_flg != 0)
    {
        w = 0x18;
    }
    r.x = src->x;
    r.y = src->y;
    r.w = w;
    r.h = src->h;
    h = src->h;
    n = (src->w * w) >> 4;
    t = h << 4;
    if (Rgb24_flg != 0)
    {
        stride = ((h << 5) + t) / 4;
        p = buf;
    }
    else
    {
        stride = h * 8;
        p = buf;
    }
    for (i = 0; i < n; i += w)
    {
        LoadImage(&r, (ULONG*)p);  // word view of the same memory
        DrawSync(0);
        p += stride;
        r.x += w;
    }
}

VOID Str_pc_buf_set(UCHAR* buf, UINT Sector_num)
{
    Pc_frm_buf[0] = buf;
    Pc_frm_buf[1] = buf + ((Sector_num >> 1) << 0xB);
}

INT Str_pc_open(CHAR* name)
{
    INT fd;
    INT a;
    INT b;

    fd = PCopen(name, 0, 0);
    Pc_fd = fd;
    if (fd < 0)
    {
        return -1;
    }
    Pc_sub_flg = 0;
    PCread(fd, (CHAR*)Pc_sub_head, 8);  // byte view of the same memory
    a = Pc_sub_head[0] & 0xFF0000;
    b = Pc_sub_head[1] & 0xFF0000;
    Pc_sub_head[0] = a;
    Pc_sub_head[1] = b;
    if (a == 0x420000 && b == a)
    {
        Pc_sub_flg = 1;
    }
    else if (a == 0x640000 && b == a)
    {
        Pc_sub_flg = 1;
    }
    PClseek(Pc_fd, 0, 0);
    Pc_sec_head[Pc_buf_idx].frameCount = 0;
    do
    {
        Str_pc_frm_read(Pc_frm_buf[Pc_buf_idx]);
    } while (Pc_sec_head[Pc_buf_idx].frameCount < Pc_frm_start);
    if (pPc_open_cb != 0)
    {
        pPc_open_cb();
    }
    return 0;
}

VOID Str_pc_init(INT unused, INT Frm_start, INT Frm_end, VOID (*pOpen_cb)(), INT (*pEnd_cb)())
{
    Pc_buf_idx = 0;
    Pc_frm_end = Frm_end;
    pPc_open_cb = pOpen_cb;
    Pc_eof_flg = 0;
    Pc_sec_head[0].frameCount = 0;
    Pc_sec_head[1].frameCount = 0;
    Pc_frm_start = Frm_start;
    pPc_end_cb = pEnd_cb;
}

VOID Str_pc_close(VOID)
{
    PCclose(Pc_fd);
}

INT Str_pc_frm_next(VOID)
{
    INT idx;

    idx = Pc_buf_idx == 0;
    Pc_buf_idx = idx;
    Str_pc_frm_read(Pc_frm_buf[idx]);
    if (pPc_open_cb != 0)
    {
        pPc_open_cb();
    }
    if (Pc_sec_head[Pc_buf_idx].frameCount >= Pc_frm_end || Pc_eof_flg != 0)
    {
        if (pPc_end_cb != 0)
        {
            pPc_end_cb();
        }
    }
    return 0;
}

INT Str_pc_frm_get(UCHAR** pFrm_buf, VOID** header)
{
    *pFrm_buf = Pc_frm_buf[Pc_buf_idx];
    *header = &Pc_sec_head[Pc_buf_idx];
    return 0;
}

VOID Str_pc_frm_read(UCHAR* buf)
{
    INT magic;
    INT n;
    INT a;
    INT b;

    n = 0;
    if (Pc_eof_flg != 0)
    {
        return;
    }
    do
    {
        if (Pc_sub_flg != 0)
        {
            while (1)
            {
                if (PCread(Pc_fd, (CHAR*)Pc_sub_head, 8) == 0)  // byte view of the same memory
                {
                    Pc_eof_flg = 1;
                    return;
                }
                a = Pc_sub_head[0] & 0xFF0000;
                b = Pc_sub_head[1] & 0xFF0000;
                Pc_sub_head[0] = a;
                Pc_sub_head[1] = b;
                magic = 0x420000;
                if (a == magic)
                {
                    break;
                }
                PClseek(Pc_fd, 0x918, 1);
                n++;
            }
        }
        if (PCread(Pc_fd, (CHAR*)&Pc_sec_head[Pc_buf_idx], 0x14) == 0)  // byte view of the same memory
        {
            Pc_eof_flg = 1;
            return;
        }
        PClseek(Pc_fd, 0xC, 1);
        PCread(Pc_fd, buf, 0x7E0);
        buf += 0x7E0;
        if (Pc_sub_flg != 0)
        {
            PClseek(Pc_fd, 0x118, 1);
        }
        n++;
    } while (Pc_sec_head[Pc_buf_idx].secCount != Pc_sec_head[Pc_buf_idx].nSectors - 1);
    if (Pc_sec_head[Pc_buf_idx].frameCount >= Pc_frm_end)
    {
        Pc_eof_flg = 1;
    }
    Pc_sec_cnt = n;
}
