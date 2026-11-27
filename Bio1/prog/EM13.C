/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM13.C
 */

#include "TYPES.H"
#include "EM13.H"
#include "CUT.H"
#include "SCE1.H"
#include "SCRATCHPAD.H"
#include "FLAG.H"
#include "JOINT.H"
#include "OBA.H"
#include "PLAYER.H"

VOID Em13_Main(VOID)
{
    ENEMY_WORK* pEm2;
    ENEMY_WORK* pEm;

    Em13_routine_0[EM_CUR->Routine_0]();
    pEm = MAIN.pEm;
    pEm->Coord_flg = 0;
    MAIN.pEm->Zone_in_flg = Hit_ck_point4(POS_VEC(pEm), CCCUT_CUR);
    pEm2 = MAIN.pEm;
    if (pEm2->Zone_in_flg != 0)
    {
        MulKage(POS_VEC(pEm2), &pEm2->Kage_work, 0, pEm2->Cdir_y);
    }
}

VOID Em13_Init(VOID)
{
    ENEMY_WORK* pEm2;
    ENEMY_WORK* pEm;
    ENEMY_WORK* pEm3;

    pEm = EM_CUR;
    ROUTINE_WORD(pEm) = 1;  // INT* view of the field (a plain access changes the code)
    pEm->Cdir_x = 0;
    pEm->Cdir_z = 0;
    Init_Parts_work();
    ScratchPad->Sv.vx = 0;
    ScratchPad->Sv.vy = 0;
    ScratchPad->Sv.vz = 0;
    ScratchWord[7] = 0xFFFFFF;
    Kage_work_init(&ScratchPad->Sv, &MAIN.pEm->Kage_work, 0xA, 0x3E8);
    pEm2 = MAIN.pEm;

    pEm2->Life = 0x37;
    pEm2->Scale = 0x1000;
    pEm2->pAt_tbl = Em13_sca_tbl[G.Player.Id & 1];

    Oba_set_ofs(pEm2);
    MAIN.pEm->Move_no = 0;
    MAIN.pEm->Move_cnt = 0;
    MAIN.pEm->Move_wait = 0;
    MAIN.pEm->Hokan_flg = 0;
    pEm3 = MAIN.pEm;
    Joint_move(0, pEm3->pKan_t_ptr, pEm3->pSeq_t_ptr, 0x400);
}

VOID Em13_Move(VOID)
{
    ENEMY_WORK* pEm;
    ENEMY_WORK* pEm2;

    pEm = EM_CUR;
    pEm->Be_flg &= EM_BE_STATE_MASK;
    pEm2 = MAIN.pEm;
    pEm2->Be_flg |= 0x40;
}

VOID Em13_Damage(VOID)
{
    ENEMY_WORK* pEm;
    PARTS_W* pP_ptr;
    UINT k;

    pEm = G.pEm;
    k = pEm->Damage_no >> 3;
    pP_ptr = pEm->pSin_parts_ptr;
    ROUTINE_WORD(pEm) = 1;  // INT* view of the field (a plain access changes the code)
    if (k == 1 || k >= 6)
    {
        ScratchWord[7] = 0;
        if (pEm->Life < 50)
        {
            ScratchWord[7] = 1;
        }
        if (pEm->Life < 40)
        {
            ScratchWord[7] = 2;
        }
        if (pEm->Life < 30)
        {
            ScratchWord[7] = 3;
        }
        if (pEm->Life < 20)
        {
            ScratchWord[7] = 4;
        }
        if (pEm->Life < 10)
        {
            ScratchWord[7] = 5;
        }
        while (--ScratchWord[7] != -1)
        {
            (pP_ptr + 5 - ScratchWord[7])->Be_flg = 0;
        }
    }
    G.pEm->Damage_no = 0;
}

VOID Em13_Die(VOID)
{
    INT n;

    ENEMY_WORK* pEm;

    ScratchWord[7] = 5;
    do
    {
        EM_CUR->pSin_parts_ptr[ScratchWord[7]].Be_flg = 0;
        n = ScratchWord[7] - 1;
        ScratchWord[7] = n;
    } while (n != -1);

    pEm = EM_CUR;
    pEm->Be_flg |= EM_BE_NO_HIT;
    Flg_on(&MAIN.SaveData.Enemy_flg, MAIN.pEm->Em_set_flg);  // address passed as an integer
    MAIN_EM_ROUTINE_WORD = 4;
}

VOID Em13_Dead(VOID)
{
}

AT_TBL Em13_sca00 = { 0x8000, -700, 0, 0, 0, 0x1F4 };    // flags 0x8000, offset x -700, radius 500

AT_TBL Em13_sca01 = { 0x8000, -500, 0, 0, 0, 0x1F4 };    // flags 0x8000, offset x -500, radius 500

AT_TBL* Em13_sca_tbl[2] = { &Em13_sca00, &Em13_sca01 };    // collision records indexed by player id & 1

// state handlers of the web, indexed by state
EM13_FUNC Em13_routine_0[5] =
{
    Em13_Init,      // 0 //
    Em13_Move,      // 1 //
    Em13_Damage,   // 2 //
    Em13_Die,   // 3 //
    Em13_Dead,      // 4 //
};
