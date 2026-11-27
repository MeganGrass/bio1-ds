/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\ENEMY.C
 */

#include "ENEMY.H"
#include "CINESCO.H"
#include "EFF1.H"
#include "EM_SUB.H"
#include "GAME.H"
#include "JOINT.H"
#include "PL_WEP.H"
#include "SCE1.H"
#include "SCE_AT.H"
#include "FILECODE.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Emd_load(pEm, id)
ENEMY_WORK* pEm;  // parameter
UCHAR id;  // narrow parameter, passed as int
{
    INT unused[9];
    UCHAR cx;
    UCHAR cy;

    UCHAR Pl_no;
    UCHAR* pEmd_row;
    UCHAR* pEmd_row2;

    UCHAR t;
    UINT size;
    VOID** ppLoad;
    UCHAR* pBase;
    UINT* pEnd;
    ENEMY_WORK* pEm_cur;

    cx = G.Vram_cur.b.Ctpage;
    cy = G.Vram_cur.b.Cclut;
    if ((G.Status_flg & STAT_PL_ALT_MODEL) && id < 2)
    {
        id = 0x33;
    }

    if (G.Status_flg & STAT_ARRANGE)
    {
        if (id < 2)
        {
            id = 0x34;
        }
        if (id == 3)
        {
            id = 0x35;
        }
        if ((UCHAR)(id - 0x24) < 2 || id == 0x27 || id == 0x30)  // cast kept: range test on the low byte of the model id
        {
            id += 0x14;
        }
        if (id == 0x33 && Flg_ck(&MAIN.SaveData.Scenario_flg, 0x6F) != 0)
        {
            id = G.Player.Id & 1;
        }
    }
    pEmd_row = (MAIN.Player.Id & 1) * 0x8A + Emd_file_tbl;
    Cd_read(*(USHORT*)(id * 2 + pEmd_row), G.pFree_work);  // cast kept: Emd_file_tbl is a byte table of 16-bit file codes
    Pl_no = G.Player.Id & 1;
    pEmd_row2 = Pl_no * 0x8A + Emd_file_tbl;
    size = floc[*(USHORT*)(id * 2 + pEmd_row2)].size;  // cast kept: Emd_file_tbl is a byte table of 16-bit file codes
    if ((G.Status_flg & STAT_PL_ALT_MODEL) && id == 0x33)
    {
        id = Pl_no;
    }
    if (G.Status_flg & STAT_ARRANGE)
    {
        INT k;

        do
        {
            do
            {
                k = 0x34;
            } while (0);
        } while (0);
        if (id == k)
        {
            id = MAIN.Player.Id & 1;
        }
        if (id == 0x35)
        {
            id = 3;
        }
        if (id == 0x1A)
        {
            G.pEm->Id = 0;
        }
    }
    ppLoad = &MAIN.pFree_work;
    pBase = *ppLoad;
    pEnd = (UINT*)(pBase + (size >> 2) * 4);  // cast kept: the model block ends in a table of word offsets

    *ppLoad = (UCHAR*)((*(pEnd - 1) >> 2) * 4 + (INT)pBase);

    pEm_cur = G.pEm;
    t = pEm_cur->Id;
    if (t >= 0x20)
    {
        pEm_cur->Free[0] = (UCHAR)G.Vram_cur.b.Cclut;  // truncates to a byte
        pEm_cur->Free[1] = CTPAGE_NOW;  // cast kept: the page byte is saved as an unsigned byte
    }

    Mapping_tim(*ppLoad, pEm_cur, size);

    if ((Emd_clut_mask >> t) & 1)
    {
        Cinesco_fade_add(cy, t);
    }
    pEm->pTmd = pBase + (*(pEnd - 2) >> 2) * 4;
    Mapping_tmd(2, pEm->pTmd, cx, cy);  // cast kept: Mapping_tmd takes the model as a byte pointer
    pEm->pSeq_t_ptr = pBase + (*(pEnd - 3) >> 2) * 4;
    pEm->pKan_t_ptr = pBase + (*(pEnd - 4) >> 2) * 4;
    if (*(pEnd - 5) != 0)
    {
        G.Player.pSub1_kan_t_ptr = pBase;
        G.Player.pSub1_seq_t_ptr = pBase + (*(pEnd - 5) >> 2) * 4;
    }
    Task_sleep(1);
}

VOID Em_wep_load(VOID)
{
    ENEMY_WORK* pEm2;
    PARTS_W* pP_ptr;
    UCHAR Wep_no;
    UCHAR Model_idx;
    UINT cur;
    UCHAR Clut_bak;
    UCHAR Tpage_bak;
    ENEMY_WORK* pEm;
    UINT size;

    pEm = EM_NOW;

    Wep_no = pEm->Type;
    if (Wep_no != 0)
    {
        Model_idx = pEm->Id - 0x20;
        if (Model_idx >= 5)
        {
            Model_idx = 2;
        }
        if (Model_idx >= 12)
        {
            Model_idx = 3;
        }
        pP_ptr = pEm->pSin_parts_ptr;
        pP_ptr += 14;

        if (G.System_flg & SYS_DOOR_PROCESS)
        {
            Cd_read(Wep_model_tbl[Model_idx][Wep_no], G.pFree_work);
        }
        else
        {
            Cd_read_wait(Wep_model_tbl[Model_idx][Wep_no], G.pFree_work);
        }
        pEm2 = EM_NOW;

        size = floc[Wep_model_tbl[Model_idx][pEm2->Type]].size;
        cur = FREE_WORK_UINT;  // cast kept: the load pointer is used as an integer address
        ScratchUword[8] = cur;

        FREE_WORK_UINT = ((size >> 2) << 2) + cur;  // cast kept: the load pointer is used as an integer address

        pP_ptr->pTmd = (VOID*)cur;
        Clut_bak = CCLUT_CUR;
        MAIN.Vram_cur.b.Cclut = pEm2->Free[0];
        Tpage_bak = CTPAGE_CUR;
        MAIN.Vram_cur.b.Ctpage = pEm2->Free[1];
        Mapping_tmd(2, pP_ptr->pTmd, MAIN.Vram_cur.b.Ctpage, (UCHAR)MAIN.Vram_cur.b.Cclut);  // truncates to a byte
        MAIN.Vram_cur.b.Cclut = Clut_bak;
        MAIN.Vram_cur.b.Ctpage = Tpage_bak;
        pP_ptr->pTmd = (UCHAR*)pP_ptr->pTmd + 0xC;
        pP_ptr->pPacket = MAIN.pFree_work;
        FREE_WORK_UINT = (UINT)GsPresetObject((GsDOBJ5*)&pP_ptr->Attribute, pP_ptr->pPacket);  // cast kept: GsPresetObject returns the next free address, stored as the load pointer
    }
}

VOID Pl_weapon_ch(INT Wep_no, INT joint_index, VOID* buf, UINT* anim_object)
{
    PARTS_W* pP_ptr;
    UINT* pEnd;
    UINT t;
    UCHAR saved;

    INT n = Wep_no;

    pP_ptr = G.Player.pSin_parts_ptr;

    pP_ptr += joint_index & 0xFF;
    if (G.Status_flg & STAT_ARRANGE)
    {
        if ((G.Player.Id & 3) < 2 && ((n & 0xFF) == 2 || (n & 0xFF) == 4))
        {
            n = 0xC;
            G.Player.Type = 2;
        }
    }

    if (G.System_flg & SYS_DOOR_PROCESS)
    {
        Cd_read(Wep_anim_file[(G.Player.Id & 3) * 13 + (n & 0xFF)], buf);
    }
    else
    {
        Cd_read_wait(Wep_anim_file[(G.Player.Id & 3) * 13 + (n & 0xFF)], buf);
    }

    t = floc[Wep_anim_file[(G.Player.Id & 3) * 13 + (n & 0xFF)]].size;
    G.Player.pSub0_kan_t_ptr = buf;
    pEnd = (UINT*)((UCHAR*)buf + (t >> 2) * 4);  // cast kept: the animation block ends in a table of word offsets
    G.Player.pSub0_seq_t_ptr = (UCHAR*)buf + (pEnd[-2] >> 2) * 4;

    if ((n & 0xFF) != 0)
    {
        pP_ptr->pTmd = (UCHAR*)buf + (pEnd[-1] >> 2) * 4;
        t = CCLUT_UBYTE_CUR;
        MAIN.Vram_cur.b.Cclut = 7;
        saved = CTPAGE_CUR;
        MAIN.Vram_cur.b.Ctpage = 0x16;
        Mapping_tmd(2, pP_ptr->pTmd, 0x16, 7);
        MAIN.Vram_cur.b.Cclut = t;
        MAIN.Vram_cur.b.Ctpage = saved;
        pP_ptr->pPacket = anim_object;
        pP_ptr->pTmd = (UCHAR*)pP_ptr->pTmd + 0xC;
    }
    else
    {
        pP_ptr->pTmd = G.Player.pParts_tmd;
        pP_ptr->pPacket = G.Player.pParts_packet;
    }
    GsPresetObject((GsDOBJ5*)&pP_ptr->Attribute, pP_ptr->pPacket);  // cast kept: the object and packet words are viewed as the SDK types
}

VOID Spl_Scenario(VOID)
{
    ENEMY_WORK* pEm;
    INT zero;
    USHORT flags;
    ENEMY_WORK* e;

    Spl_sce[EM_CUR->Routine_2]();
    e = EM_CUR;
    pEm = e;
    if (pEm->Sce_flg & 2)
    {
        Spl_sce[pEm->Routine_2]();
    }
    flags = EM_CUR->Sce_flg;
    if (flags & 4)
    {
        zero = 0;
        Foot_set_pl((flags & 8) != zero);
    }
}

VOID Walk_spd_set(USHORT speed)
{
    ENEMY_WORK* e;
    ENEMY_WORK* e2;
    ENEMY_WORK* e3;

    e = EM_CUR;
    e->Spd_cur = speed;
    if (e->Move_cnt - 0x15 < 7U)
    {
        e->Spd_cur = speed - 0xD;
    }
    e2 = EM_CUR;
    if (e2->Move_cnt - 7 < 7U)
    {
        e2->Spd_cur = e2->Spd_cur - 0xD;
    }
    if (EM_CUR->Move_cnt - 0x17 < 3U)
    {
        EM_CUR->Spd_cur = EM_CUR->Spd_cur - 0xE;
    }
    e3 = EM_CUR;
    if (e3->Move_cnt - 9 < 3U)
    {
        e3->Spd_cur = e3->Spd_cur - 0xE;
    }
}

VOID Spl_sce00(VOID)
{
    UCHAR step;
    ENEMY_WORK* e;

    e = EM_CUR;
    step = e->Routine_3;
    switch (step)
    {
    case 0:
        e->Move_cnt = 0;
        EM_CUR->Move_wait = 0;
        EM_CUR->Move_no = 0;
        EM_CUR->Routine_3 = 1U;
        EM_CUR->Hokan_flg = 7;

    case 1:
        Joint_move(EM_NOW->Sce_flg & 1, EM_NOW->pKan_t_ptr, EM_NOW->pSeq_t_ptr, 0x200);
        return;
    }
}

VOID Spl_sce01(VOID)
{
    USHORT Time_cnt;
    UCHAR step;
    ENEMY_WORK* e;
    ENEMY_WORK* e3;
    ENEMY_WORK* e2;
    ENEMY_WORK* e4;
    ENEMY_WORK* e5;

    e = EM_CUR;
    step = e->Routine_3;
    switch (step)
    {
    case 0:
        e->Move_cnt = 0;
        EM_CUR->Move_wait = 0;
        EM_CUR->Routine_3 = 1U;
        EM_CUR->Hokan_flg = 7;
        e2 = EM_CUR;
        e2->Spd_cur = 0;
        if (e2->Sce_flg & 0x20)
        {
            e2->Hokan_flg = 0;
        }
        EM_CUR->Timer0 = 1;

    case 1:
        if (G_PEM_BYTE(ENEMY_WORK)->Sce_flg & 0x80)  // G_PEM_BYTE: a plain or pointer-typed pEm changes the code
        {
            Time_cnt = EM_NOW->Timer0;
            EM_NOW->Timer0 = Time_cnt - 1;
            if (Time_cnt == 0)
            {
                EM_NOW->Timer0 = 1U;
                return;
            }
        }
        e3 = EM_CUR;
        if (Joint_move(e3->Sce_flg & 1, e3->pKan_t_ptr, e3->pSeq_t_ptr, 0x200) != 0)
        {
            EM_CUR->Routine_3 = 2U;
            e4 = EM_CUR;
            e4->Cdir_y += e4->Sce_free0;
            return;
        }
        return;
    case 2:
        Flg_on(&MAIN.Room_flg, e->Set_flg_no);
        e5 = EM_CUR;
        e5->Sce_free0 = 0U;
        if (e5->Sce_flg & 0x10)
        {
            e5->Routine_3 = 0U;
        }
        break;
    }
}

VOID Spl_sce02(VOID)
{
    INT dx;
    INT dz;
    INT r;
    ENEMY_WORK* e0;
    ENEMY_WORK* e1;
    ENEMY_WORK* e2;
    ENEMY_WORK* e3;
    ENEMY_WORK* e4;
    ENEMY_WORK* e5;
    ENEMY_WORK* e6;

    e0 = EM_CUR;
    switch (e0->Routine_3)
    {
    case 0:
        e0->Move_cnt = 0;
        EM_CUR->Move_wait = 0;
        EM_CUR->Move_no = 7;
        EM_CUR->Routine_3 = 1;
        EM_CUR->Hokan_flg = 7;

    case 1:
        e1 = EM_CUR;
        ScratchPad->Vec.vx = e1->Root_x;
        ScratchVec[1] = 0;  // typed scratch vector view (a plain field access changes the code)
        ScratchPad->Vec.vz = e1->Root_z;
        r = Goto00_ck(&ScratchPad->Vec, VIEW(SHORT, e1->Sce_free0));  // cast kept: Sce_free0 is passed as a signed halfword
        e2 = EM_CUR;
        e2->Cdir_y += r;
        Joint_move(e2->Sce_flg & 1, e2->pKan_t_ptr, e2->pSeq_t_ptr, 0x200);
        if (Goto00_ck(&ScratchPad->Vec, 0x16A) == 0)
        {
            EM_CUR->Routine_3 = 2;
        }
        break;
    case 2:
        e0->Move_cnt = 0;
        EM_CUR->Move_wait = 0;
        EM_CUR->Move_no = 7;
        EM_CUR->Routine_3 = 3;
        EM_CUR->Hokan_flg = 7;

    case 3:
        if (EM_CUR->Move_cnt == 8)
        {
            Snd_se_walk(0, 0);
        }

        if (EM_CUR->Move_cnt == 0x16)
        {
            Snd_se_walk(0, -4);
        }
        Walk_spd_set(0x5D);
        e3 = EM_CUR;
        ScratchPad->Vec.vx = e3->Root_x;
        ScratchVec[1] = 0;  // typed scratch vector view (a plain field access changes the code)
        ScratchPad->Vec.vz = e3->Root_z;
        Goto00(&ScratchPad->Vec, VIEW(SHORT, e3->Sce_free0));  // cast kept: Sce_free0 is passed as a signed halfword
        e4 = EM_CUR;
        Joint_move(e4->Sce_flg & 1, e4->pKan_t_ptr, e4->pSeq_t_ptr, 0x200);
        Add_speedXZ(0);
        e5 = EM_CUR;
        dx = e5->Pos_x - e5->Root_x;
        dz = e5->Pos_z - e5->Root_z;
        if (SquareRoot0(dx * dx + dz * dz) < 0x96)
        {
            Flg_on(&MAIN.Room_flg[0], EM_CUR->Set_flg_no);  // cast kept: Flg_on takes the flag table as an address
            e6 = EM_CUR;
            if (!(e6->Sca_info & 0x80))
            {
                ROUTINE_HALF2(e6) = 0;  // cast kept: Routine_2 and Routine_3 are cleared as one halfword
            }
        }
        break;
    }
}

VOID Spl_sce03(VOID)
{
    INT dx;
    INT dz;
    SHORT Time_cnt;
    UCHAR step;
    UCHAR Flg_no;
    ENEMY_WORK* e2;
    ENEMY_WORK* e5;
    ENEMY_WORK* pEm8;
    ENEMY_WORK* e;
    ENEMY_WORK* e3;
    ENEMY_WORK* e4;
    ENEMY_WORK* e6;
    ENEMY_WORK* e7;
    ENEMY_WORK* pEm9;
    ENEMY_WORK* pEm10;

    step = EM_NOW->Routine_3;
    switch (step)
    {
    case 0:
        EM_CUR->Move_cnt = 0U;
        EM_CUR->Move_wait = 0;
        EM_CUR->Move_no = 8;
        EM_CUR->Routine_3 = 1;
        EM_CUR->Hokan_flg = 7;

    case 1:
        e = EM_CUR;
        ScratchPad->Vec.vx = e->Root_x;
        ScratchVec[1] = 0;  // typed scratch vector view (a plain field access changes the code)
        ScratchPad->Vec.vz = e->Root_z;
        dx = Goto00_ck(&ScratchPad->Vec, VIEW(SHORT, e->Sce_free0));  // cast kept: Sce_free0 is passed as a signed halfword
        e2 = EM_CUR;
        e2->Cdir_y += dx;
        Joint_move(e2->Sce_flg & 1, e2->pKan_t_ptr, e2->pSeq_t_ptr, 0x200);
        if (Goto00_ck(&ScratchPad->Vec, 0x16A) == 0)
        {
            EM_CUR->Routine_3 = 2;
            return;
        }
    default:
        return;
    case 2:
        e3 = EM_CUR;
        e3->Spd_cur = 0xD2U;
        e3->Move_cnt = 0U;
        EM_CUR->Move_wait = 0;
        EM_CUR->Move_no = 8;
        EM_CUR->Routine_3 = 3;
        EM_CUR->Hokan_flg = 7;

    case 3:
        if (EM_CUR->Move_cnt == 0)
        {
            Snd_se_walk(1, 0);
        }

        if (EM_CUR->Move_cnt == 0xA)
        {
            Snd_se_walk(1, -4);
        }
        e4 = EM_CUR;
        ScratchPad->Vec.vx = e4->Root_x;
        ScratchVec[1] = 0;  // typed scratch vector view (a plain field access changes the code)
        ScratchPad->Vec.vz = e4->Root_z;
        Goto00(&ScratchPad->Vec, VIEW(SHORT, e4->Sce_free0));  // cast kept: Sce_free0 is passed as a signed halfword
        e5 = EM_CUR;
        Joint_move(e5->Sce_flg & 1, e5->pKan_t_ptr, e5->pSeq_t_ptr, 0x200);
        Add_speedXZ(0);
        e6 = EM_CUR;
        dx = e6->Pos_x - e6->Root_x;
        dz = e6->Pos_z - e6->Root_z;
        if (SquareRoot0(dx * dx + dz * dz) < 0xFA)
        {
            EM_CUR->Routine_3 = 4;
            e7 = EM_CUR;
            if (e7->Sca_info & 0x80)
            {
                e7->Routine_3 = 3;
                Flg_on(&MAIN.Room_flg, EM_CUR->Set_flg_no);
            }
        }
        break;
    case 4:
        EM_CUR->Move_cnt = 0U;
        EM_CUR->Move_wait = 0;
        EM_CUR->Move_no = 0;
        EM_CUR->Routine_3 = 5;
        EM_CUR->Hokan_flg = 7;
        EM_CUR->Timer0 = 0U;

    case 5:
        pEm8 = EM_CUR;
        Joint_move(pEm8->Sce_flg & 1, pEm8->pKan_t_ptr, pEm8->pSeq_t_ptr, 0x200);
        pEm9 = EM_CUR;
        Time_cnt = pEm9->Timer0 + 1;
        pEm9->Timer0 = Time_cnt;
        if (Time_cnt >= 4)
        {
            pEm9->Routine_3 = 6;
        }
        pEm10 = EM_CUR;
        pEm10->Spd_cur = pEm10->Spd_cur - 0x1E;
        Add_speedXZ(0);
        return;
    case 6:
        Flg_no = EM_NOW->Set_flg_no;
        ROUTINE_HALF2(EM_NOW) = 0;  // cast kept: Routine_2 and Routine_3 are cleared as one halfword
        Flg_on(&MAIN.Room_flg, Flg_no);
        break;
    }
}

static const UINT Em_jump_pad = 0; // padding: separates the jump tables of the functions before and after it

VOID Spl_sce04(VOID)
{
    INT dx;
    INT dz;
    INT frame2;
    UCHAR step;
    UCHAR frame;
    ENEMY_WORK* e;
    ENEMY_WORK* e2;
    ENEMY_WORK* e3;
    ENEMY_WORK* e4;
    ENEMY_WORK* e5;

    e = EM_CUR;
    step = e->Routine_3;
    switch (step)
    {
    case 0:
        e->Move_cnt = 0;
        EM_CUR->Move_wait = 0;
        EM_CUR->Routine_3 = 1U;
        EM_CUR->Hokan_flg = 7;
        EM_CUR->Move_no = 3;

    case 1:
        frame = EM_NOW->Move_cnt;
        if (frame == 8 || frame == 0x16)
        {
            Snd_se_walk(0, 0);
        }
        frame2 = G_PEM_BYTE(ENEMY_WORK)->Move_cnt;  // G_PEM_BYTE: a plain or pointer-typed pEm changes the code
        EM_NOW->Spd_cur = 0x3C;
        if (frame2 >= 5U || frame2 < 8U)
        {
            EM_NOW->Spd_cur = 0x40;
        }
        e2 = EM_CUR;
        ScratchPad->Vec.vx = e2->Root_x;
        ScratchPad->Vec.vz = e2->Root_z;
        ScratchPad->Vec.vy = 0;
        e2->Cdir_y = (e2->Cdir_y + 0x800) & 0xFFF;
        Goto00(&ScratchPad->Vec, (SHORT)e2->Sce_free0);
        e3 = EM_CUR;
        e3->Cdir_y -= 0x800;
        Joint_move(e3->Sce_flg & 1, e3->pKan_t_ptr, e3->pSeq_t_ptr, 0x200);
        Add_speedXZ(0x800);
        e4 = EM_CUR;
        dx = e4->Pos_x - e4->Root_x;
        dz = e4->Pos_z - e4->Root_z;
        if (SquareRoot0(dx * dx + dz * dz) < 0x64)
        {
            Flg_on(&MAIN.Room_flg, EM_CUR->Set_flg_no);
            e5 = EM_CUR;
            if (!(e5->Sca_info & 0x80))
            {
                ROUTINE_HALF2(e5) = 0;  // cast kept: Routine_2 and Routine_3 are cleared as one halfword
            }
        }
        return;
    }
}

VOID Spl_sce05(VOID)
{
    INT dx;
    INT dz;
    UCHAR step;
    UCHAR frame;
    ENEMY_WORK* e;
    ENEMY_WORK* e2;
    ENEMY_WORK* e3;
    ENEMY_WORK* e4;
    ENEMY_WORK* e5;

    e = EM_CUR;
    step = e->Routine_3;
    switch (step)
    {
    case 0:
        e->Move_cnt = 0;
        EM_CUR->Move_wait = 0;
        EM_CUR->Routine_3 = 1U;
        EM_CUR->Hokan_flg = 7;
        EM_CUR->Move_no = 2;
        EM_CUR->Spd_cur = 0x1D;

    case 1:
        frame = EM_NOW->Move_cnt;
        if ((frame == 7 || frame == 0x1B) && EM_NOW->Move_wait == 2)
        {
            Snd_se_walk(0, -4);
        }
        e2 = EM_CUR;
        ScratchPad->Vec.vx = e2->Root_x;
        ScratchPad->Vec.vz = e2->Root_z;
        ScratchPad->Vec.vy = 0;
        e2->Cdir_y = (e2->Cdir_y + 0x800) & 0xFFF;
        Goto00(&ScratchPad->Vec, (SHORT)e2->Sce_free0);
        e3 = EM_CUR;
        e3->Cdir_y -= 0x800;
        Joint_move(e3->Sce_flg & 1, e3->pKan_t_ptr, e3->pSeq_t_ptr, 0x200);
        Add_speedXZ(0x800);
        e4 = EM_CUR;
        dx = e4->Pos_x - e4->Root_x;
        dz = e4->Pos_z - e4->Root_z;
        if (SquareRoot0(dx * dx + dz * dz) < 0x64)
        {
            Flg_on(&MAIN.Room_flg, EM_CUR->Set_flg_no);
            e5 = EM_CUR;
            if (!(e5->Sca_info & 0x80))
            {
                ROUTINE_HALF2(e5) = 0;  // cast kept: Routine_2 and Routine_3 are cleared as one halfword
            }
        }
        return;
    }
}

VOID Spl_sce06(VOID)
{
    UCHAR step;
    ENEMY_WORK* e;
    ENEMY_WORK* e3;
    ENEMY_WORK* e2;

    e = EM_CUR;
    step = e->Routine_3;
    switch (step)
    {
    case 0:
        e->Move_cnt = 0;
        EM_CUR->Move_wait = 0;
        EM_CUR->Move_no = 7;
        EM_CUR->Routine_3 = 1U;
        EM_CUR->Hokan_flg = 7;

    case 1:
        e2 = EM_CUR;
        ScratchPad->Vec.vx = e2->Root_x;
        ScratchPad->Vec.vz = e2->Root_z;
        ScratchPad->Vec.vy = 0;
        Goto00(&ScratchPad->Vec, (SHORT)e2->Sce_free0);
        e3 = EM_CUR;
        Joint_move(e3->Sce_flg & 1, e3->pKan_t_ptr, e3->pSeq_t_ptr, 0x200);
        if (Goto00_ck(&ScratchPad->Vec, 0x28) == 0)
        {
            EM_CUR->Routine_3 = 2U;
            return;
        }
        return;
    case 2:
        ROUTINE_HALF2(e) = 0;  // cast kept: Routine_2 and Routine_3 are cleared as one halfword
        Flg_on(&MAIN.Room_flg, e->Set_flg_no);
        break;
    }
}

VOID Spl_sce07(VOID)
{
    UCHAR step;
    ENEMY_WORK* e;

    e = EM_CUR;
    step = e->Routine_3;
    switch (step)
    {
    case 0:
        e->Move_cnt = 0;
        EM_CUR->Move_wait = 0;
        EM_CUR->Routine_3 = 1U;
        EM_CUR->Hokan_flg = 7;

    case 1:
        EM_CUR->Routine_3 = EM_CUR->Routine_3 + Joint_move(EM_CUR->Sce_flg & 1, EM_CUR->pKan_t_ptr, EM_CUR->pSeq_t_ptr, 0x200);
        break;
    case 2:
        Flg_on(&MAIN.Room_flg, e->Set_flg_no);
        break;
    }
    EM_NOW->Cdir_y += EM_NOW->Sce_free0;
}

VOID Spl_sce08(VOID)
{
    PARTS_W* base;
    UCHAR k;
    USHORT c;
    INT r;

    k = G.pEm->Type - 2;
    switch (G.pEm->Routine_3)
    {
    case 0:
        EM_CUR->Move_cnt = 0;
        EM_CUR->Move_wait = 0;
        EM_CUR->Routine_3 = 1;
        EM_CUR->Hokan_flg = 3;
        if (k == 3)
        {
            EM_CUR->Routine_3 = 3;
            EM_CUR->Move_no = 0x17;
            goto anim;
        }
    case 1:
        base = G.pEm->pSin_parts_ptr;
        base += 14;
        if (G.pEm->Move_cnt == Pl_flash_tbl[k].frame)
        {
            ScratchVec[0] = Pl_flash_tbl[k].Ofs_x;  // typed scratch vector view (a plain field access changes the code)
            ScratchPad->Vec.vy = Pl_flash_tbl[k].off_y;
            ScratchPad->Vec.vz = Pl_flash_tbl[k].off_z;

            Esp_call(Pl_flash_tbl[k].fx_type, Pl_flash_tbl[k].Esp_dt, 0, &base->Workm, &ScratchPad->Vec, 0);

            if (k == 2)
            {
                ScratchPad->Vec.vx = 0x96;
                ScratchPad->Vec.vy = 0x17C;
                ScratchPad->Vec.vz = 0;

                Esp_call(0x11, 3, 0, &base->Workm, &ScratchPad->Vec, 0);
            }
        }
        if (EM_CUR->Move_cnt == Pl_shell_tbl[k].frame)
        {
            ENEMY_WORK* obj = EM_CUR;
            INT r1;

            ScratchVec[0] = Pl_shell_tbl[k].Ofs_x;  // typed scratch vector view (a plain field access changes the code)
            ScratchPad->Vec.vy = Pl_shell_tbl[k].off_y + (obj->Id & 1) * (1 - k) * 300;
            ScratchPad->Vec.vz = Pl_shell_tbl[k].off_z;
            r1 = Esp_call(Pl_shell_tbl[k].fx_type, Pl_shell_tbl[k].Esp_dt, k != 8 ? 0x555 : 0, MATRIX_OF(obj), &ScratchPad->Vec, 0);

            ScratchWord[0] = r1;
            MAIN.Esp[r1].Free3 = k;
        }
        if (EM_CUR->Move_cnt == Pl_flash2_tbl[k].frame)
        {
            INT r2;

            ScratchVec[0] = Pl_flash2_tbl[k].Ofs_x;  // typed scratch vector view (a plain field access changes the code)
            ScratchPad->Vec.vy = Pl_flash2_tbl[k].off_y;
            ScratchPad->Vec.vz = Pl_flash2_tbl[k].off_z;

            r2 = Esp_call(Pl_flash2_tbl[k].fx_type, Pl_flash2_tbl[k].Esp_dt, 0, &base->Workm, &ScratchPad->Vec, 0);

            ScratchWord[0] = r2;
            MAIN.Esp[r2].Free0 = k;
        }
        r = Joint_move(0, EM_CUR->pKan_t_ptr,
            EM_CUR->pSeq_t_ptr, 0x400);
        EM_CUR->Routine_3 += r;
        break;
    case 2:
        Flg_on(&MAIN.Room_flg, G.pEm->Set_flg_no);
        break;
    case 3:
    anim:
        r = Joint_move(0, EM_CUR->pKan_t_ptr,
            EM_CUR->pSeq_t_ptr, 0x400);
        EM_CUR->Routine_3 += r;
        break;
    case 4:
        EM_CUR->Routine_3 = 5;
        EM_CUR->Move_wait = 0;
        EM_CUR->Move_no = 0x14;
        EM_CUR->Hokan_flg = 3;
        EM_CUR->Timer0 = 15;
    case 5:
        if (EM_CUR->Move_cnt % 6 == 0)
        {
            ENEMY_WORK* obj = EM_CUR;

            ScratchVec[0] = 0x21C;  // typed scratch vector view (a plain field access changes the code)
            ScratchPad->Vec.vy = 0x4EC;
            ScratchPad->Vec.vz = 0;

            Esp_call(12, 0, 0, &obj->pSin_parts_ptr[14].Workm, &ScratchPad->Vec, 0);
        }
        c = EM_CUR->Timer0;
        EM_CUR->Timer0 = c - 1;
        if (c == 0)
        {
            EM_CUR->Timer0 = 15;
            Snd_se_stad(2, 0x1E, 0, &EM_CUR->Pos_x);
            Snd_se_stad(2, 0x1F, 0, &EM_CUR->Pos_x);
        }
        Joint_move(0, EM_CUR->pKan_t_ptr,
            EM_CUR->pSeq_t_ptr, 0x400);
        EM_CUR->Cdir_y += EM_CUR->Sce_free0;
        break;
    }
}

VOID Spl_sce09(VOID)
{
    UCHAR step;
    ENEMY_WORK* e;
    ENEMY_WORK* e2;

    e = EM_CUR;
    step = e->Routine_3;
    switch (step)
    {
    case 0:
        e->Move_cnt = 0;
        EM_CUR->Move_wait = 0;
        EM_CUR->Routine_3 = 1U;
        EM_CUR->Hokan_flg = 3;

    case 1:
        e2 = EM_CUR;
        EM_CUR->Routine_3 = EM_CUR->Routine_3 + Joint_move(1, e2->pKan_t_ptr, e2->pSeq_t_ptr, 0x400);
        return;
    case 2:
        ROUTINE_HALF2(e) = 0;  // cast kept: Routine_2 and Routine_3 are cleared as one halfword
        Flg_on(&MAIN.Room_flg, e->Set_flg_no, e);  // passes 3 arguments, Flg_on declares 2
        return;
    }
}

VOID Spl_sce0a(VOID)
{
    UCHAR step;
    ENEMY_WORK* e;
    ENEMY_WORK* e2;

    e = EM_CUR;
    step = e->Routine_3;
    switch (step)
    {
    case 0:
        e->Move_cnt = 0;
        EM_CUR->Move_wait = 0;
        EM_CUR->Routine_3 = 1U;
        EM_CUR->Hokan_flg = 7;
        EM_CUR->Spd_cur = 0;

    case 1:
        EM_CUR->Routine_3 = EM_CUR->Routine_3 + Joint_move(EM_CUR->Sce_flg & 1, EM_CUR->pKan_t_ptr, EM_CUR->pSeq_t_ptr, 0x200);
        return;
    case 2:
        Flg_on(&MAIN.Room_flg, e->Set_flg_no);
        e2 = EM_CUR;
        if (e2->Sce_flg & 0x10)
        {
            e2->Routine_3 = 0U;
        }
        return;
    }
}

VOID Tmd_tpage_set(TMD_MEM* obj, INT tpage)
{
    INT unused[2];
    INT next;
    INT count;
    INT left;
    USHORT* pTpage;

    count = obj->n_poly;
    ScratchWord[0] = count;
    left = count - 1;
    pTpage = (USHORT*)(obj->prim_top + 6);  // cast kept: the texture page word of the primitive, 16-bit at byte offset 6
    ScratchWord[0] = left;
    if (left != -1)
    {
        do
        {
            *pTpage = (*pTpage & 0xFFC0) | (tpage & 0x3F);
            pTpage += 14;
            next = ScratchWord[0] - 1;
            ScratchWord[0] = next;
        } while (next != -1);
    }
}

VOID Em_main(VOID)
{
    INT unused[8];
    INT i;
    ENEMY_WORK* pEm;

    G.pEm = MAIN.Enemy;
    for (i = 0; i < G.Enemy_num; EM_CUR++)
    {
        if (i >= 0x1E)
        {
            break;
        }
        pEm = EM_CUR;
        if (pEm->Be_flg & EM_BE_ENABLED)
        {
            Monitor.Em_move_tbl[pEm->Id]();
            if ((G.System_flg & SYS_MIRROR_MATRIX) &&
                Mirror_in_view((MAIN.SaveData.Cut_no * 0x2C + (UCHAR*)(MAIN.pRoom) + 0x9C), (G.System_flg & SYS_MIRROR_SIDE) >> 1,  // untyped pointer, the callee takes the address as is
                    &EM_CUR->Pos_x) != 0)
            {
                Mirror_trans();
            }
            i++;
        }
    }
}

VOID Em_save_set(VOID)
{
    INT i;
    INT unused;
    ENEMY_WORK* pEm_slot;
    UCHAR* rooms_left;
    INT Room_num;
    ENEMY_WORK* pEm2;
    CORPSE_SAVE_WORK* pSave_em;
    CORPSE_SAVE_WORK* pSave_cur;
    ENEMY_WORK* pEm;
    ENEMY_WORK* pEm3;

    i = 0;
    if (G.pSce_aot_on->Next_room != G.SaveData.Room_no_old)
    {
        rooms_left = &((CORPSE_SAVE_WORK*)&MAIN.Em_save_buf[0])[0].rooms_left;  // cast kept: Em_save_buf is a byte buffer holding CORPSE_SAVE_WORK records
        do
        {
            Room_num = *rooms_left;

            i += 1;

            if (Room_num != 0)
            {
                *rooms_left = Room_num - 1;
            }

            rooms_left += sizeof(CORPSE_SAVE_WORK);
        } while (i < 0x10);
    }
    G.pEm = MAIN.Enemy;
    G.Em_save_cur = (CORPSE_SAVE_WORK*)&MAIN.Em_save_buf;  // the buffer as its CORPSE_SAVE_WORK entries
    if (G.Enemy_num != 0)
    {
        do
        {
            if (EM_SAVE_CUR->rooms_left != 0)
            {
                do
                {
                    pSave_cur = EM_SAVE_CUR;
                    EM_SAVE_CUR = pSave_cur + 1;
                } while (pSave_cur[1].rooms_left != 0);
            }
            pEm = EM_CUR;
            if (pEm->Life < 0)
            {
                EM_SAVE_CUR->status_flags = 0;
                if (EM_CUR->Em_set_flg != 0xFF)
                {
                }
                else
                {
                    goto block_13;
                }
            }
            else
            {
                EM_SAVE_CUR->status_flags = pEm->Be_flg & 0xF;
            block_13:
                pEm2 = EM_CUR;
                if (pEm2->Slot_info >= 0)  // cast kept: Slot_info is tested as a signed byte
                {
                    EM_SAVE_CUR->behavior_flags = pEm2->Type;
                    EM_SAVE_CUR->Room_no = G.SaveData.Room_no;
                    EM_SAVE_CUR->Slot_info = EM_CUR->Slot_info;
                    pSave_em = EM_SAVE_CUR;
                    pEm3 = EM_CUR;
                    pSave_em->state = pEm3->Routine_0;
                    pSave_em->pos_x = pEm3->Pos_x;
                    pSave_em->pos_y = pEm3->Pos_y;
                    pSave_em->pos_z = pEm3->Pos_z;
                    pSave_em->angle = pEm3->Cdir_y;
                    pSave_em->rooms_left = 5U;
                }
            }

            VIEW(SCHAR, MAIN.Enemy_num) -= 1;  // cast kept: Enemy_num is decremented as a signed byte

            EM_CUR++;
        } while (MAIN.Enemy_num != 0);
    }
    i = 0x1D;
    pEm_slot = &MAIN.Enemy[29];
    do
    {
        pEm_slot->Be_flg = 0;
        i -= 1;
        pEm_slot--;
    } while (i >= 0);
}

INT Em_save_get(INT slot, VOID* pScd)
{
    INT i;

    G.Em_save_cur = (CORPSE_SAVE_WORK*)&MAIN.Em_save_buf[0];  // the buffer as its CORPSE_SAVE_WORK entries
    i = 0;
    do
    {
        if (EM_SAVE_CUR->rooms_left != 0 &&
            EM_SAVE_CUR->Room_no == ROOM_NO_CUR &&  // form kept: the plain read changes the code
            EM_SAVE_CUR->Slot_info == slot)
        {
            EM_CUR->Be_flg = EM_SAVE_CUR->status_flags;
            EM_CUR->Type = EM_SAVE_CUR->behavior_flags;
            EM_CUR->Pos_x = EM_SAVE_CUR->pos_x;
            if (EM_CUR->Type & 0x70)
            {
                EM_CUR->Pos_y = EM_SAVE_CUR->pos_y;
            }
            EM_CUR->Pos_z = EM_SAVE_CUR->pos_z;
            EM_CUR->Cdir_y = EM_SAVE_CUR->angle;
            EM_SAVE_CUR->rooms_left = 0;
            return 1;
        }
        i++;
        EM_SAVE_CUR++;
    } while (i < 16);
    return 0;
}

// CD file number of each enemy model as a 16-bit value (low byte, high byte): entries 0 to 68 are the
// models of the first player character by model id, entries 69 to 137 those of the second
UCHAR Emd_file_tbl[276] =
{
    0x1F, 0x00,   // 0 //
    0x20, 0x00,   // 1 //
    0x21, 0x00,   // 2 //
    0x22, 0x00,   // 3 //
    0x23, 0x00,   // 4 //
    0x24, 0x00,   // 5 //
    0x25, 0x00,   // 6 //
    0x26, 0x00,   // 7 //
    0x27, 0x00,   // 8 //
    0x28, 0x00,   // 9 //
    0x29, 0x00,   // 10 //
    0x2A, 0x00,   // 11 //
    0x2B, 0x00,   // 12 //
    0x2C, 0x00,   // 13 //
    0x2D, 0x00,   // 14 //
    0x2E, 0x00,   // 15 //
    0x2F, 0x00,   // 16 //
    0x30, 0x00,   // 17 //
    0x31, 0x00,   // 18 //
    0x32, 0x00,   // 19 //
    0x33, 0x00,   // 20 //
    0x34, 0x00,   // 21 //
    0x35, 0x00,   // 22 //
    0x36, 0x00,   // 23 //
    0x37, 0x00,   // 24 //
    0x38, 0x00,   // 25 //
    0x39, 0x00,   // 26 //
    0x2D, 0x00,   // 27 //
    0x2D, 0x00,   // 28 //
    0x2D, 0x00,   // 29 //
    0x2D, 0x00,   // 30 //
    0x2D, 0x00,   // 31 //
    0x2D, 0x00,   // 32 //
    0x2D, 0x00,   // 33 //
    0x2D, 0x00,   // 34 //
    0x2D, 0x00,   // 35 //
    0x3A, 0x00,   // 36 //
    0x3B, 0x00,   // 37 //
    0x3C, 0x00,   // 38 //
    0x3D, 0x00,   // 39 //
    0x3E, 0x00,   // 40 //
    0x3F, 0x00,   // 41 //
    0x40, 0x00,   // 42 //
    0x41, 0x00,   // 43 //
    0x42, 0x00,   // 44 //
    0x43, 0x00,   // 45 //
    0x44, 0x00,   // 46 //
    0x45, 0x00,   // 47 //
    0x46, 0x00,   // 48 //
    0x47, 0x00,   // 49 //
    0x48, 0x00,   // 50 //
    0x49, 0x00,   // 51 //
    0x4B, 0x00,   // 52 //
    0x4D, 0x00,   // 53 //
    0x2D, 0x00,   // 54 //
    0x2D, 0x00,   // 55 //
    0x4E, 0x00,   // 56 //
    0x4F, 0x00,   // 57 //
    0x3C, 0x00,   // 58 //
    0x50, 0x00,   // 59 //
    0x3E, 0x00,   // 60 //
    0x3F, 0x00,   // 61 //
    0x40, 0x00,   // 62 //
    0x41, 0x00,   // 63 //
    0x42, 0x00,   // 64 //
    0x43, 0x00,   // 65 //
    0x44, 0x00,   // 66 //
    0x45, 0x00,   // 67 //
    0x51, 0x00,   // 68 //
    0x1F, 0x00,   // 69 //
    0x20, 0x00,   // 70 //
    0x21, 0x00,   // 71 //
    0x22, 0x00,   // 72 //
    0x52, 0x00,   // 73 //
    0x53, 0x00,   // 74 //
    0x54, 0x00,   // 75 //
    0x55, 0x00,   // 76 //
    0x56, 0x00,   // 77 //
    0x57, 0x00,   // 78 //
    0x58, 0x00,   // 79 //
    0x59, 0x00,   // 80 //
    0x5A, 0x00,   // 81 //
    0x5B, 0x00,   // 82 //
    0x5C, 0x00,   // 83 //
    0x5D, 0x00,   // 84 //
    0x5E, 0x00,   // 85 //
    0x5F, 0x00,   // 86 //
    0x60, 0x00,   // 87 //
    0x61, 0x00,   // 88 //
    0x62, 0x00,   // 89 //
    0x63, 0x00,   // 90 //
    0x64, 0x00,   // 91 //
    0x65, 0x00,   // 92 //
    0x66, 0x00,   // 93 //
    0x67, 0x00,   // 94 //
    0x68, 0x00,   // 95 //
    0x5C, 0x00,   // 96 //
    0x5C, 0x00,   // 97 //
    0x5C, 0x00,   // 98 //
    0x5C, 0x00,   // 99 //
    0x5C, 0x00,   // 100 //
    0x5C, 0x00,   // 101 //
    0x5C, 0x00,   // 102 //
    0x5C, 0x00,   // 103 //
    0x5C, 0x00,   // 104 //
    0x3A, 0x00,   // 105 //
    0x3B, 0x00,   // 106 //
    0x3C, 0x00,   // 107 //
    0x3D, 0x00,   // 108 //
    0x3E, 0x00,   // 109 //
    0x3F, 0x00,   // 110 //
    0x40, 0x00,   // 111 //
    0x41, 0x00,   // 112 //
    0x42, 0x00,   // 113 //
    0x43, 0x00,   // 114 //
    0x44, 0x00,   // 115 //
    0x45, 0x00,   // 116 //
    0x46, 0x00,   // 117 //
    0x47, 0x00,   // 118 //
    0x48, 0x00,   // 119 //
    0x4A, 0x00,   // 120 //
    0x4C, 0x00,   // 121 //
    0x4D, 0x00,   // 122 //
    0x2D, 0x00,   // 123 //
    0x2D, 0x00,   // 124 //
    0x4E, 0x00,   // 125 //
    0x4F, 0x00,   // 126 //
    0x3C, 0x00,   // 127 //
    0x50, 0x00,   // 128 //
    0x3E, 0x00,   // 129 //
    0x3F, 0x00,   // 130 //
    0x40, 0x00,   // 131 //
    0x41, 0x00,   // 132 //
    0x42, 0x00,   // 133 //
    0x43, 0x00,   // 134 //
    0x44, 0x00,   // 135 //
    0x45, 0x00,   // 136 //
    0x51, 0x00,   // 137 //
};

UINT Emd_clut_mask = 0x0004F900; // bit mask by model id: models that register a CLUT fade when loaded

// CD file number of the weapon model, by model group (model id - 0x20) then behavior number
USHORT Wep_model_tbl[5][7] =
{
    { WS202_TMD, WS202_TMD, WS202_TMD, WS202_TMD, WS202_TMD, WS202_TMD, WS202_TMD },   // 0 //
    { WS212_TMD, WS212_TMD, WS212_TMD, WS212_TMD, WS212_TMD, WS212_TMD, WS212_TMD },   // 1 //
    { WS224_TMD, WS224_TMD, WS224_TMD, WS224_TMD, WS224_TMD, WS225_TMD, WS225_TMD },   // 2 //
    { WS232_TMD, WS232_TMD, WS232_TMD, WS232_TMD, WS232_TMD, WS232_TMD, WS236_TMD },   // 3 //
    { WS242_TMD, WS242_TMD, WS242_TMD, WS242_TMD, WS242_TMD, WS242_TMD, WS242_TMD },   // 4 //
};

USHORT Wep_model_pad = 0; // padding: keeps the following data at its linked position

// CD file number of the weapon animation, by player character (id & 3) * 13 plus weapon number
USHORT Wep_anim_file[52] =
{
    W00_EMW,   // 0 //
    W01_EMW,   // 1 //
    W02_EMW,   // 2 //
    W03_EMW,   // 3 //
    W04_EMW,   // 4 //
    W04_EMW,   // 5 //
    W05_EMW,   // 6 //
    W06_EMW,   // 7 //
    W06_EMW,   // 8 //
    W06_EMW,   // 9 //
    W07_EMW,   // 10 //
    W0B_EMW,   // 11 //
    W0F_EMW,   // 12 //
    W10_EMW,   // 13 //
    W11_EMW,   // 14 //
    W12_EMW,   // 15 //
    W13_EMW,   // 16 //
    W14_EMW,   // 17 //
    W14_EMW,   // 18 //
    W15_EMW,   // 19 //
    W16_EMW,   // 20 //
    W16_EMW,   // 21 //
    W16_EMW,   // 22 //
    W17_EMW,   // 23 //
    W1B_EMW,   // 24 //
    W1F_EMW,   // 25 //
    W00_EMW,   // 26 //
    W01_EMW,   // 27 //
    W02_EMW,   // 28 //
    W03_EMW,   // 29 //
    W04_EMW,   // 30 //
    W04_EMW,   // 31 //
    W05_EMW,   // 32 //
    W06_EMW,   // 33 //
    W06_EMW,   // 34 //
    W06_EMW,   // 35 //
    W07_EMW,   // 36 //
    W0B_EMW,   // 37 //
    W0F_EMW,   // 38 //
    W30_EMW,   // 39 //
    W11_EMW,   // 40 //
    W32_EMW,   // 41 //
    W13_EMW,   // 42 //
    W14_EMW,   // 43 //
    W14_EMW,   // 44 //
    W15_EMW,   // 45 //
    W16_EMW,   // 46 //
    W16_EMW,   // 47 //
    W16_EMW,   // 48 //
    W17_EMW,   // 49 //
    W1B_EMW,   // 50 //
    W1F_EMW,   // 51 //
};

// scripted action handlers, by action behavior
ENEMY_FUNC Spl_sce[11] =
{
    Spl_sce00,   // 0 //
    Spl_sce01,   // 1 //
    Spl_sce02,   // 2 //
    Spl_sce03,   // 3 //
    Spl_sce04,   // 4 //
    Spl_sce05,   // 5 //
    Spl_sce06,   // 6 //
    Spl_sce07,   // 7 //
    Spl_sce08,   // 8 //
    Spl_sce09,   // 9 //
    Spl_sce0a,   // 10 //
};
