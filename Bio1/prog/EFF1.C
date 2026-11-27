/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EFF1.C
 */

#include "EFF1.H"
#include "EFF0.H"
#include "CUT.H"
#include "JOINT.H"
#include "FILECODE.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "GAME.H"

UCHAR* Eff_tim_adr[8]; // texture images of the loaded sprite sets

VOID Esp_init_R(VOID)
{
    UCHAR i;
    UCHAR c;
    ROOM_WORK* p;
    ESP_SPR_TBL* w;
    ESP_SPR_TBL* q;
    ROOM_WORK* r;
    INT t;
    INT o;

    G.Scenario.nEsp_free = 0x40;
    Memclr((UINT*)MAIN.Esp, 0x840);  // the effect slots are cleared as words
    i = 0;
    w = &MAIN.Esp_spr;
    while (i < 8)
    {
        c = (&MAIN.Esp_spr.Espid[8])[i];
        q = (ESP_SPR_TBL*)(i + (INT)w);  // reinterprets the memory as ESP_SPR_TBL
        if (c == 0xFF)
        {
            i = 8;
        }
        else
        {
            i++;
            w->pEspdt[c] = -1;
            w->pEspmv[c] = -1;
            q->Espid[8] = 0xFF;
        }
    }
    p = G.pRoom;
    Esp_data_set0(p->pEsp_hed, p->pEsp_end, (INT)p, 8);
    for (i = 0; i < 8; i++)
    {
        r = ROOM_CUR;
        o = i;
        o <<= 2;
        t = *(INT*)((UCHAR*)r->pEsp_tim - o);  // word view of the same memory
        o += (INT)Eff_tim_adr;  // signed conversion (unsigned operand)
        *(INT*)o = t + (INT)r;  // real width or sign conversion
    }
    Esp_data_set1(8);
}

VOID Esp_init_C(VOID)
{
    UCHAR i;
    ESP_SPR_TBL* w;
    ESP_SPR_TBL* pSpr_tbl2;
    UINT base;
    UINT t;
    UCHAR n;
    INT* q;
    INT v;

    G.Scenario.nEsp_free = 0x40;
    i = 0;
    w = &MAIN.Esp_spr;
    do
    {
        *(i + w->Espid) = 0xFF;
        i++;
    } while (i < 16);
    i = 0;
    pSpr_tbl2 = &MAIN.Esp_spr;
    do
    {
        pSpr_tbl2->pEspdt[i] = -1;
        pSpr_tbl2->pEspmv[i] = -1;
        i++;
    } while (i < 50);
    base = FREE_WORK_UINT;

    Cd_read(CORE00_ESP, (VOID*)base);

    t = ((USHORT)floc[CORE00_ESP].size & 0xFFFC) + (UINT)MAIN.pFree_work;  // truncates to an unsigned halfword

    FREE_WORK_UINT = t;

    if ((USHORT)floc[CORE00_ESP].size & 3)  // truncates to an unsigned halfword
    {
        FREE_WORK_UINT = t + 4;
    }
#ifdef __INTELLISENSE__  // void* arithmetic is not valid C for IntelliSense
    *(UCHAR**)&MAIN.pFree_work -= 4;
#else
    MAIN.pFree_work -= 4;
#endif
    n = Esp_data_set0((UCHAR*)base, MAIN.pFree_work, base, 0);
    FREE_WORK_UINT = (UINT)OVERLAY_AREA;
    q = (INT*)OVERLAY_AREA;  // word view of the same memory

    Cd_read(CORE00_ETM, OVERLAY_AREA);

    q = BYTE_OFS(INT, q, (USHORT)floc[CORE00_ETM].size & 0xFFFC);
    if ((USHORT)floc[CORE00_ETM].size & 3)  // truncates to an unsigned halfword
    {
        q++;
    }
    i = 0;
    do
    {
        q--;
        v = FREE_WORK_INT;
        v += *q;
        Eff_tim_adr[i] = (UCHAR*)v;  // byte view of the same memory
        i++;
    } while (i < n);
    Esp_data_set1(0);
}

UCHAR Esp_data_set0(UCHAR* ptr8, INT* ptr32, INT addr, UCHAR type)
{
    UCHAR pRet;

    UCHAR cnt0;

    UCHAR id;

    ESP_SPR_TBL* p;
    UCHAR* q;
    INT v;
    INT k;
    INT w;
    INT tbl;

    pRet = 8;
    cnt0 = 0;
    tbl = (UINT)&MAIN.Esp_spr;  // address passed as an integer
    do
    {
        id = ptr8[cnt0];
        k = cnt0 + type;
        ESPID_CUR[k] = id;
        if (id == 0xFF)
        {
            pRet = cnt0;
            cnt0 = 8;
        }
        else
        {
            v = ptr32[-cnt0];
            v += addr;
            p = (ESP_SPR_TBL*)(id * 4 + tbl);  // reinterprets the memory as ESP_SPR_TBL
            p->pEspdt[0] = v;
            p->pEspmv[0] = v;
            q = (UCHAR*)p->pEspdt[0];  // byte view of the same memory
            cnt0++;
            w = (q[2] + q[0]) * 4 + 8;

            p->pEspmv[0] = w + v;
        }
    } while (cnt0 < 8);
    return pRet;
}

VOID Esp_data_set1(INT start)
{
    GsIMAGE Tim;
    RECT rect;
    UCHAR id;
    UCHAR ch;
    UCHAR i;
    UCHAR n;

    UCHAR page;
    USHORT x;
    USHORT y;
    USHORT cx;
    USHORT cy;

    USHORT* p;
    UCHAR* q;

    INT k;

    USHORT c;

    if ((start & 0xFF) == 8)
    {
        page = G.Esp_spr.Tpage_cur;
        x = (&G.Esp_spr.Tex_x)[0];
        y = (&G.Esp_spr.Tex_x)[1];
        cx = (&G.Esp_spr.Clut_x)[0];
        cy = (&G.Esp_spr.Clut_x)[1];
    }
    else
    {
        G.Esp_spr.Tpage_cur = 0x18;
        (&G.Esp_spr.Tex_x)[0] = 0;
        (&G.Esp_spr.Tex_x)[1] = 0;
        (&G.Esp_spr.Clut_x)[0] = 0;
        (&G.Esp_spr.Clut_x)[1] = 4;
        page = 0x18;
        x = 0;
        y = 0;
        cx = 0;
        cy = 4;
    }
    i = 0;
    k = start & 0xFF;
    goto test;

    do
    {
        id = MAIN.Esp_spr.Espid[i + (start & 0xFF)];
        GsGetTimInfo((ULONG*)(Eff_tim_adr[i] + 4), &Tim);  // word view of the same memory

        if (y + Tim.ph > 0x100)
        {
            x += 0x40;
            y = 3;
            page++;
        }
        if (cy + Tim.ch >= 0x20)
        {
            cx++;
            cy = 4;
        }

        rect.x = x + 0x200;
        rect.y = y + 0x100;
        rect.w = Tim.pw;
        rect.h = Tim.ph;
        ch = Tim.ch;
        LoadImage(&rect, Tim.pixel);
        DrawSync(0);
        if ((Tim.pmode >> 3) & 1)
        {
            rect.x = cx * 16 + 0x100;
            rect.y = cy + 0x1E0;
            rect.w = Tim.cw;
            rect.h = Tim.ch;
            LoadImage(&rect, Tim.clut);
            DrawSync(0);
        }

        c = cx + (((cy + 0x1E0) << 6) + 0x10);

        p = ((USHORT**)&MAIN.Esp_spr.pEspdt[0])[id];  // halfword view of the same memory

        n = 0;

        p[2] = c;
        ((UCHAR*)p)[6] = page;  // byte view of the same memory

        q = (UCHAR*)(p[1] * 4 + (UINT)p + 9);  // byte view of the same memory

        do
        {
            n++;
            *q += y;
            q += 4;
        } while (n < p[0]);

        y += Tim.ph;

        cy = ch + cy;

        i++;
        if (i >= 8)
        {
            break;
        }

        k = i + (start & 0xFF);
    test:;
    } while (ESPID_CUR[k] != 0xFF);
    if ((start & 0xFF) == 0)
    {
        G.Esp_spr.Tpage_cur = page;
        (&G.Esp_spr.Tex_x)[0] = x;
        (&G.Esp_spr.Tex_x)[1] = y;
        (&G.Esp_spr.Clut_x)[0] = cx;
        (&G.Esp_spr.Clut_x)[1] = cy;
    }
}

SCHAR Esp_call_api(UCHAR tp, UCHAR id, INT dir_y, MATRIX* pmat, VECTOR* svec, UCHAR scale)
{
    return Esp_call(tp, id, dir_y, pmat, svec, scale);
}

INT Esp_call(id, tp, dir_y, pmat, svec, scale)
UCHAR id;  // narrow parameter, passed as int
UCHAR tp;  // narrow parameter, passed as int
INT dir_y;  // parameter
MATRIX* pmat;  // parameter
VECTOR* svec;  // parameter
UCHAR scale;  // narrow parameter, passed as int
{
    INT first;
    INT found;
    INT i;
    UINT j;
    UINT n;
    UCHAR* data;
    UCHAR* q;
    ESP_DATA_HDR* p;
    INT off;

    first = 1;
    if (G.Scenario.nEsp_free != 0)
    {
        do
        {
            found = 1;
            i = 0x40;
            do
            {
                i--;
                if (ESP_SLOTS[i].h.Routine0 == 0)
                {
                    found = 0;

                    ESP_FREE_CNT--;
                }
            } while (i != 0 && found != 0);
            if (found != 0)
            {
                return -1;
            }

            ESP_SLOTS[i].Mov_x = ESP_SLOTS[i].Mov_y = ESP_SLOTS[i].Mov_z = 0;
            ESP_SLOTS[i].Pos_x = ESP_SLOTS[i].Pos_y = ESP_SLOTS[i].Pos_z = 0;
            ESP_SLOTS[i].M00 = ESP_SLOTS[i].M01 = ESP_SLOTS[i].m02 = 0;
            ESP_SLOTS[i].m10 = ESP_SLOTS[i].M11 = ESP_SLOTS[i].m12 = 0;
            ESP_SLOTS[i].M20 = ESP_SLOTS[i].M21 = ESP_SLOTS[i].M22 = 0;
            ESP_SLOTS[i].trans_x = ESP_SLOTS[i].Trans_y = ESP_SLOTS[i].Trans_z = 0;

            ESP_SLOTS[i].Otz4 = 0;

            ESP_SLOTS[i].Otz = 0;

            ESP_SLOTS[i].Type = id;
            ESP_SLOTS[i].Id = tp;
            ESP_SLOTS[i].Hot_x = svec->vx;
            ESP_SLOTS[i].Hot_y = svec->vy;
            ESP_SLOTS[i].Hot_z = svec->vz;

            ESP_SLOTS[i].pMat = pmat;
            ESP_SLOTS[i].spawn = *svec;
            off = id * 4;
            ESP_SLOTS[i].pEsp_hdr = *BYTE_OFS(UCHAR*, &MAIN.Esp_spr.pEspdt[0], off);  // the record pointer of effect type id, read by byte offset (a plain index changes the code)
            ESP_SLOTS[i].pAnmseq = ESP_SLOTS[i].pAnm_cur = *BYTE_OFS(UCHAR*, &MAIN.Esp_spr.pEspdt[0], off) + 8;  // byte view of the same memory
            p = *BYTE_OFS(ESP_DATA_HDR*, &MAIN.Esp_spr.pEspdt[0], off);  // reinterprets the memory as ESP_DATA_HDR
            {
                INT hdr_size = 8;
                ESP_SLOTS[i].pSprg = ESP_SLOTS[i].pSprg_cur = (VOID*)(p->Frm_num * 4 + hdr_size + (INT)p);  // address computed as an integer
            }

            ESP_SLOTS[i].Anm_time = ESP_SLOTS[i].pAnmseq[1];
            ESP_SLOTS[i].Anm_cnt = ESP_SLOTS[i].pAnmseq[0];
            if (first != 0)
            {
                data = *BYTE_OFS(UCHAR*, &MAIN.Esp_spr.pEspmv[0], off);  // byte view of the same memory
                data += data[tp & 7] * 4;
                first = 0;
                n = *(INT*)data;  // word view of the same memory
            }
            q = data + 4;
            for (j = 1; j < n; j++)
            {
                q += *(INT*)q * sizeof(ESP_STEP_REC) + 4;  // word view of the same memory
            }
            ESP_SLOTS[i].pMoveseq_top = ESP_SLOTS[i].pMoveseq = (ESP_STEP_REC*)(q + 4);  // reinterprets the memory as ESP_STEP_REC
            ESP_SLOTS[i].h = *ESP_SLOTS[i].pMoveseq;

            if (scale != 0)
            {
                ESP_SLOTS[i].h.Mag = scale;
            }

            ESP_SLOTS[i].h.Mat_mode = 1;
            ESP_SLOTS[i].h.DirY += dir_y;
            n--;
            if (pmat == 0)
            {
                ESP_SLOTS[i].pMat = &GsIDMATRIX;
            }
        } while (n != 0);
        return (SCHAR)i;
    }
}

VOID Esp_move(VOID)
{
    GsRVIEW2 view;

    G.Esp_idx = 0x40;
    do
    {
        VIEW(SCHAR, MAIN.Esp_idx) -= 1;  // SCHAR* view of the field (a plain access changes the code)
        if (pESP->Routine0 != 0)
        {
            ScratchPad->Mat = GsIDMATRIX;
            Esp_pos();
            if (G.System_flg & SYS_MIRROR_MATRIX)
            {
                Mirror_view((GsRVIEW2*)(MAIN_PROOM_INT + (CUT_NO_CUR) * 0x2C + 0x9C), &view,  // address of the mirror view record as an integer

                    (G.System_flg & SYS_MIRROR_SIDE) >> 1, Mirror_pos);
                GsSetRefView2(&view);
                ScratchPad->Mat = GsIDMATRIX;
                (&ScratchPad->Mat)->m[0][0] *= -1;
                GsMulCoord2(&ScratchPad->Mat, &GsWSMATRIX);
                Esp_mirror_disp();

                GsSetRefView2((GsRVIEW2*)((INT)MAIN.pRoom + MAIN.SaveData.Cut_no * 0x2C + 0x9C));  // signed conversion (unsigned operand)
            }
        }
    } while (G.Esp_idx != 0);
}

VOID Esp_pos(VOID)
{
    SVECTOR sxy;
    VECTOR pos;
    UCHAR* pUv_cur;
    USHORT t;
    UINT q;

    UINT z;

    if (G.Stop_flg & STOP_ESP_FX)
    {
        Esp_tblj[ESP_ACTORS[ESP_IDX].Routine0]();
    }
    if (ESP_ACTORS[ESP_IDX].Be_flg & 2)
    {
        RotMatrixY(ESP_ACTORS[ESP_IDX].DirY, &ScratchPad->Mat);
        ApplyMatrixSV(&ScratchPad->Mat, (SVECTOR*)&ESP_ACTORS[ESP_IDX].Mov_x, &ScratchPad->Sv);  // reinterprets the memory as SVECTOR
        (&ScratchPad->Sv)->vx += ESP_ACTORS[ESP_IDX].Hot_x;
        (&ScratchPad->Sv)->vy += ESP_ACTORS[ESP_IDX].Hot_y;
        (&ScratchPad->Sv)->vz += ESP_ACTORS[ESP_IDX].Hot_z;
        if (ESP_ACTORS[ESP_IDX].Mat_mode == 1)
        {
            ESP_ACTORS[ESP_IDX].matrix = *ESP_ACTORS[ESP_IDX].pMat;
        }
        ApplyMatrix(&ESP_ACTORS[ESP_IDX].matrix, &ScratchPad->Sv, &ScratchPad->Vec);
        ScratchPad->Sv.vx = ScratchPad->Vec.vx + ESP_ACTORS[ESP_IDX].matrix.t[0];
        ScratchPad->Sv.vy = ScratchPad->Vec.vy + ESP_ACTORS[ESP_IDX].matrix.t[1];
        ScratchPad->Sv.vz = ScratchPad->Vec.vz + ESP_ACTORS[ESP_IDX].matrix.t[2];
        if (ESP_ACTORS[ESP_IDX].Be_flg & 4)
        {
            ScratchPad->Sv.vy = 0;
        }
        if (ESP_ACTORS[ESP_IDX].Be_flg & 8)
        {
            ScratchPad->Sv.vy = G.Player.Ground;
        }
        ESP_ACTORS[ESP_IDX].Pos_x = ScratchPad->Sv.vx;
        ESP_ACTORS[ESP_IDX].Pos_y = ScratchPad->Sv.vy;
        ESP_ACTORS[ESP_IDX].Pos_z = ScratchPad->Sv.vz;
        pos.vx = ScratchPad->Sv.vx;
        pos.vy = ScratchPad->Sv.vy;
        pos.vz = ScratchPad->Sv.vz;
        SetTransMatrix(&GsWSMATRIX);
        SetRotMatrix(&GsWSMATRIX);

        ESP_ACTORS[ESP_IDX].Otz = RotTransPers(&ScratchPad->Sv, (LONG*)&sxy, &ScratchLong[0], &ScratchLong[0]) & 0xFFFF;  // word view of the same memory
        ESP_ACTORS[ESP_IDX].Otz4 = (USHORT)ESP_ACTORS[ESP_IDX].Otz * 4;  // truncates to an unsigned halfword

        sxy.vx -= 0xA0;
        sxy.vy -= G.Cbuf_id == 0 ? 0x168 : 0x78;
        if (G.Stop_flg & STOP_ESP_FX)
        {
            ((SVECTOR*)&pESP->Speed_x)->vx += ESP_ACTORS[ESP_IDX].Add_x;  // reinterprets the memory as SVECTOR
            ESP_ACTORS[ESP_IDX].Speed_y += ESP_ACTORS[ESP_IDX].Add_y;
            ESP_ACTORS[ESP_IDX].Speed_z += ESP_ACTORS[ESP_IDX].Add_z;
            ((SVECTOR*)&pESP->Mov_x)->vx += ((SVECTOR*)&pESP->Speed_x)->vx;  // reinterprets the memory as SVECTOR
            ESP_ACTORS[ESP_IDX].Mov_y += ESP_ACTORS[ESP_IDX].Speed_y;
            ESP_ACTORS[ESP_IDX].Mov_z += ESP_ACTORS[ESP_IDX].Speed_z;
        }
    }
    if (G.Stop_flg & STOP_ESP_FX)
    {
        Esp_tblj[ESP_ACTORS[ESP_IDX].Routine1]();
    }
    if ((ESP_ACTORS[ESP_IDX].Be_flg & 1) && (G.Stop_flg & STOP_ESP_FX))
    {
        Esp_anm_move();
    }
    if (!(ESP_ACTORS[ESP_IDX].Be_flg & 0x8000) && Hit_ck_point4(&pos, G.Cccut) != 0)
    {
        pUv_cur = ESP_ACTORS[ESP_IDX].pSprg_cur;
        t = ESP_ACTORS[ESP_IDX].pEsp_hdr[2];
        ScratchPad->Spr.attribute = ((ESP_ACTORS[ESP_IDX].Be_flg >> 6) << 22) | 0x40;
        ScratchPad->Spr.w = ESP_ACTORS[ESP_IDX].pAnm_cur[2];
        ScratchPad->Spr.h = ESP_ACTORS[ESP_IDX].pAnm_cur[3];
        ScratchPad->Spr.mx = 0x80 - pUv_cur[2];
        ScratchPad->Spr.my = 0x80 - pUv_cur[3];
        ScratchPad->Spr.x = sxy.vx;
        ScratchPad->Spr.y = sxy.vy;
        ScratchPad->Spr.tpage = ESP_ACTORS[ESP_IDX].pEsp_hdr[3];
        ScratchPad->Spr.u = pUv_cur[0];
        ScratchPad->Spr.v = pUv_cur[1];
        ScratchPad->Spr.cx = (t & 0x3F) * 16;
        ScratchPad->Spr.cy = (ESP_ACTORS[ESP_IDX].Id >> 3) + (t >> 6);
        q = (ScratchPad->Spr.w << 8) * (G.pRoom->Cut[G.SaveData.Cut_no].ViewR * ESP_ACTORS[ESP_IDX].Mag) / (ESP_ACTORS[ESP_IDX].Otz4 + 1);
        if (q > 0x7FFF)
        {
            ScratchPad->Spr.scalex = ScratchPad->Spr.scaley = -0x8000;
        }
        else
        {
            ScratchPad->Spr.scalex = ScratchPad->Spr.scaley = q;
        }

        z = ESP_ACTORS[G.Esp_idx].Otz >> 4;
        if (z < 0x400)
        {
            GsSortSprite(&ScratchPad->Spr, &MAIN.Ot1[G.Cbuf_id], z);
        }
    }
}

VOID Esp_mirror_disp(VOID)
{
    DVECTOR sxy[2];
    VECTOR v;
    UINT z;
    USHORT zz;
    UCHAR* pUv;
    USHORT clut;
    UINT scale;

    UINT otz;

    if (pESP->Be_flg & 2)
    {
        v.vx = pESP->Pos_x;
        v.vy = pESP->Pos_y;
        v.vz = pESP->Pos_z;
        SetTransMatrix(&GsWSMATRIX);
        SetRotMatrix(&GsWSMATRIX);

        z = 0xFFFF & RotTransPers((SVECTOR*)&pESP->Pos_x, (LONG*)&sxy[0], &ScratchLong[0], &ScratchLong[0]);  // reinterprets the memory as SVECTOR

        sxy[0].vx -= 0xA0;
        zz = z * 4;
        sxy[0].vy -= G.Cbuf_id == 0 ? 0x168 : 0x78;
    }

    if (!(pESP->Be_flg & 0x8000) && Mirror_in_view((VOID*)(MAIN_PROOM_INT + (CUT_NO_CUR) * 0x2C + 0x9C), (G.System_flg & SYS_MIRROR_SIDE) >> 1, &v) != 0 && Hit_ck_point4(&v, G.Cccut) != 0)  // type conversion
    {
        pUv = pESP->pSprg_cur;
        clut = pESP->pEsp_hdr->Clut;
        ScratchPad->Spr.attribute = ((pESP->Be_flg >> 6) << 22) | 0x40;
        ScratchPad->Spr.w = pESP->pAnm_cur->w;
        ScratchPad->Spr.h = pESP->pAnm_cur->h;
        ScratchPad->Spr.mx = 0x80 - pUv[2];
        ScratchPad->Spr.my = 0x80 - pUv[3];
        ScratchPad->Spr.x = sxy[0].vx;
        ScratchPad->Spr.y = sxy[0].vy;
        ScratchPad->Spr.tpage = pESP->pEsp_hdr->Tpage;
        ScratchPad->Spr.u = pUv[0];
        ScratchPad->Spr.v = pUv[1];
        ScratchPad->Spr.cx = (clut & 0x3F) << 4;
        ScratchPad->Spr.cy = (pESP->Id >> 3) + (clut >> 6);
        {
            INT cut_ofs = MAIN.SaveData.Cut_no * 0x2C;
            scale = (ScratchPad->Spr.w << 8) * (((ROOM_WORK*)((INT)MAIN.pRoom + cut_ofs))->Cut[0].ViewR * pESP->Mag) / (zz + 1);  // reinterprets the memory as ROOM_WORK
        }
        if (scale > 0x7FFF)
        {
            ScratchPad->Spr.scalex = ScratchPad->Spr.scaley = -0x8000;
        }
        else
        {
            ScratchPad->Spr.scalex = ScratchPad->Spr.scaley = scale;
        }

        otz = z >> 4;
        if (otz < 0x400)
        {
            GsSortSprite(&ScratchPad->Spr, MAIN.Ot1 + G.Cbuf_id, otz);
        }
    }
}

VOID Esp_anm_move(VOID)
{
    UCHAR* pFrm;
    UCHAR a;
    UCHAR b;

    if (pESP->Anm_time == 0)
    {
        pFrm = (UCHAR*)pESP->pAnm_cur;  // byte view of the same memory
        pFrm += 4;
        pESP->Anm_cnt++;
        a = pFrm[0];
        b = pFrm[1];
        if ((a == 0) & (b == 0))
        {
            if (((USHORT*)pESP)[0] != 0)  // halfword view of the same memory
            {
                ESP_FREE_CNT++;
                pESP->Routine0 = pESP->Routine1 = 0;
            }

            return;
        }
        if (b == 0xFF)
        {
            pFrm = (UCHAR*)pESP->pAnmseq + a * 4;  // byte view of the same memory
            pESP->Anm_cnt = pFrm[0];
        }
        pESP->Anm_time = pFrm[1];
        pESP->pSprg_cur =
            ((UCHAR*)pESP->pSprg + pFrm[0] * 4);
        pESP->pAnm_cur = (SSEQUENCE*)pFrm;  // reinterprets the memory as SSEQUENCE
    }
    ESP_POOL[G.Esp_idx].Anm_time--;
}

VOID Memclr(UINT* p, INT r)
{
    while (r)
    {
        *p++ = 0;
        r--;
    }
}

INT Esp_kill(mask, id, tp, c, pmat)
UCHAR mask;  // narrow parameter, passed as int
UCHAR id;  // narrow parameter, passed as int
UCHAR tp;  // narrow parameter, passed as int
UCHAR c;  // narrow parameter, passed as int
MATRIX* pmat;  // parameter
{
    INT wk_no;

    UCHAR m;

    wk_no = 64;
    do
    {
        wk_no--;

        m = 0;
        if (mask & 1)
        {
            m = ESP_POOL[wk_no].Type == id;
        }

        if ((mask & 2) && ESP_POOL[wk_no].Id == tp)
        {
            m |= 2;
        }

        if ((mask & 4) && ESP_POOL[wk_no].Free2 == c)
        {
            m |= 4;
        }
        if ((mask & 8) && ESP_POOL[wk_no].pMat == pmat)
        {
            m |= 8;
        }
        if (m == mask)
        {
            ESP_FREE_CNT++;
            ESP_POOL[wk_no].Routine1 = 0;
            ESP_POOL[wk_no].Routine0 = 0;
        }
    } while (wk_no != 0);
}

// effect behaviors, by behavior number
ESP_FUNC Esp_tblj[64] =
{
    Esp_00,                           // 0 //
    Esp_00,                           // 1 //
    Esp_02,                           // 2 //
    Esp_03,                           // 3 //
    Esp_04,                           // 4 //
    Esp_05,                           // 5 //
    Esp_06,                           // 6 //
    Esp_07,                           // 7 //
    Esp_08,                           // 8 //
    Esp_09,                           // 9 //
    Esp_0a,                           // 10 //
    Esp_0b,                           // 11 //
    Esp_0c,                           // 12 //
    Esp_0d,                           // 13 //
    Esp_0e,                           // 14 //
    Esp_0f,                           // 15 //
    Esp_10,                           // 16 //
    Esp_11,                           // 17 //
    Esp_12,                           // 18 //
    Esp_13,                           // 19 //
    Esp_14,                           // 20 //
    Esp_15,                           // 21 //
    Esp_16,                           // 22 //
    Esp_17,                           // 23 //
    Esp_18,                           // 24 //
    Esp_19,                           // 25 //
    Esp_1a,                           // 26 //
    Esp_1b,                           // 27 //
    Esp_1c,                           // 28 //
    Esp_1d,                           // 29 //
    Esp_1e,                           // 30 //
    Esp_1f,                           // 31 //
    Esp_20,                           // 32 //
    Esp_21,                           // 33 //
    Esp_22,                           // 34 //
    Esp_23,                           // 35 //
    Esp_24,                           // 36 //
    Esp_25,                           // 37 //
    Esp_26,                           // 38 //
    Esp_27,                           // 39 //
    Esp_28,                           // 40 //
    Esp_29,                           // 41 //
    Esp_2a,                           // 42 //
    Esp_2b,                           // 43 //
    Esp_2c,                           // 44 //
    Esp_2d,                           // 45 //
    Esp_2e,                           // 46 //
    Esp_2f,                           // 47 //
    Esp_30,                           // 48 //
    Esp_31,                           // 49 //
    Esp_32,                           // 50 //
    Esp_33,                           // 51 //
    Esp_34,                           // 52 //
    Esp_35,                           // 53 //
    Esp_36,                           // 54 //
    (ESP_FUNC)Esp_37,                     // 55 //
    Esp_38,                           // 56 //
    Esp_39,                           // 57 //
    Esp_3a,                           // 58 //
    Esp_3b,                           // 59 //
    Esp_nop,                            // 60 //
    Esp_nop,                            // 61 //
    Esp_nop,                            // 62 //
    Esp_nop,                            // 63 //
};
