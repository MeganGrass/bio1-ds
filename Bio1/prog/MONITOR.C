/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\MONITOR.C
 */

#include "MONITOR.H"
#include "ATTACK.H"
#include "CD.H"
#include "CINESCO.H"
#include "CUT.H"
#include "EFF1.H"
#include "EM_SUB.H"
#include "EVENT.H"
#include "GAME.H"
#include "GUIDE_CK.H"
#include "INVENTORY.H"
#include "JOINT.H"
#include "MAIN.H"
#include "MEM_CARD.H"
#include "MOJIDISP.H"
#include "OBA.H"
#include "OPTIONS.H"
#include "PAD.H"
#include "PLAYER.H"
#include "PL_WEP.H"
#include "SCA.H"
#include "SCE1.H"
#include "SCE3.H"
#include "SCE_AT.H"
#include "SND.H"
#include "STR.H"
#include "ST_SUB03.H"
#include "SUB.H"
#include "TYRANT.H"
#include "MEM_MAP.H"
#include <LIBSND.H>
#include <LIBSPU.H>
#include <LIBETC.H>
#include <STDIO.H>
#include <RAND.H>
#include "SDK.H"

TASK* Task_now;                                // task slot being set up or scheduled

#define TASK_CUR_BYTE VIEW_VIA_CHAR(TASK*, G.Scenario.pTask_cur)  // the running task pointer re-read through a byte pointer view of its address (a plain pTask_cur, TASK_CUR or a local pointer changes the generated code)
#define TASK_CUR (*&MAIN.Scenario.pTask_cur)  // running task, read through the address of its pointer (the plain member read changes the code)

static LONG (*pPc)();                                          // return address of the sleeping task, saved by Task_sleep

VOID Init_scheduler(VOID)
{
    INT i;
    struct ToT* pToT;
    API_FUNC* dst;
    VOID** src;

    pToT = (struct ToT*)0x100;  // the BIOS table of tables lives at the fixed address 0x100
    for (i = 0; i < 3; i++)
    {
        (Monitor.Task + i)->Tcb_status = 0;
        Monitor.Task[i].pTcb = &((struct TCB*)pToT[2].head)[i + 1];  // head of the third table of the BIOS table of tables is the TCB array

        Monitor.Task[i].pTcb->reg[35] = 0x404;
        Monitor.Task[i].Tcb_exec.gp = GetGp();
    }

    Task_now = &Monitor.Task[0];
    Task_now->Tcb_init_sp = TASK0_STACK_TOP;
    Task_now = &Monitor.Task[1];
    Task_now->Tcb_init_sp = (ULONG)&Monitor.Task0_stack;
    Task_now = &Monitor.Task[2];
    Task_now->Tcb_init_sp = (ULONG)&Monitor.Task1_stack;

    dst = &Monitor.Func[TASK_EXECUTE];
    i = 0;
    src = Api_tbl_src;

    for (; i < API_COUNT; i++)
    {
        *dst++ = *src++;
    }
    G.Scenario.Main_gp = GetGp();
}

VOID Scheduler(VOID)
{
    G.Scenario.pTask_cur = Monitor.Task;
    Task_now = Monitor.Task;
    do
    {
        switch (TASK_CUR->Tcb_status)
        {
        case 2:
            EnterCriticalSection();
            TASK_CUR->Tcb_th = OpenTh(
                TASK_CUR->pTcb_pc,
                TASK_CUR->Tcb_init_sp,
                TASK_CUR->Tcb_exec.gp);
            ExitCriticalSection();
            TASK_CUR->Tcb_status = 0x7F;
            ChangeTh(TASK_CUR->Tcb_th);
            break;
        case 1:
            if (--TASK_CUR->Tcb_sleep_ctr != 0)
            {
                break;
            }

        case 4:
            TASK_CUR_BYTE->Tcb_status = 0x7F;
            ChangeTh(G.Scenario.pTask_cur->Tcb_th);
            break;
        }
        TASK_CUR++;
    } while (TASK_CUR < &Monitor.Task[3]);
}

VOID Task_lexecute(INT Level, INT Task_no)
{
    INT Size;

    Size = floc[Task_no].size;
    floc[Task_no].size = 0x800;

    CHECK_SUM_FLG_CUR = 1;

    Cd_read(Task_no, CD_HEAD_BUF);

    MAIN.Check_sum_flg = 0;

    Memcpy(&Monitor.Task[Level].Tcb_exec, CD_REG_BAK, 0x3C);
    floc[Task_no].size = Size - 0x800;
    floc[Task_no].sector++;
    Cd_read(Task_no, (VOID*)Monitor.Task[Level].Tcb_exec.t_addr);
    floc[Task_no].sector--;
    floc[Task_no].size = Size;
    Monitor.Task[Level].Tcb_exec.gp = G.Scenario.Main_gp;
    Task_execute(Level, (VOID (*)(VOID))Monitor.Task[Level].Tcb_exec.pc0);
}

VOID Task_lchain(INT Task_no)
{
    INT Size;
    INT gp;

    Size = floc[Task_no].size;
    gp = TASK_CUR->Tcb_exec.gp;
    floc[Task_no].size = 0x800;

    CHECK_SUM_FLG_CUR = 1;

    Cd_read_wait(Task_no, CD_HEAD_BUF);

    MAIN.Check_sum_flg = 0;

    Memcpy(&MAIN.Scenario.pTask_cur->Tcb_exec, CD_REG_BAK, 0x3C);
    floc[Task_no].size = Size - 0x800;
    floc[Task_no].sector++;

    Cd_read_wait(Task_no, (VOID*)MAIN.Scenario.pTask_cur->Tcb_exec.t_addr);

    floc[Task_no].sector--;
    floc[Task_no].size = Size;
    TASK_CUR->Tcb_exec.gp = gp;
    Task_chain((LONG (*)())MAIN.Scenario.pTask_cur->Tcb_exec.pc0);
}

VOID Task_execute(INT Level, VOID (*pFuncall)(VOID))
{
    Monitor.Task[Level].pTcb_pc = (LONG (*)())pFuncall;
    Monitor.Task[Level].Tcb_status = 2;
}

VOID Task_sleep(Sleep_counter)
SHORT Sleep_counter;  // narrow parameter, passed as int
{
    TASK* pCt;

    pCt = TASK_CUR;
    pCt->Tcb_sleep_ctr = Sleep_counter;
    pCt->Tcb_status = 1;

#ifdef __INTELLISENSE__
    pPc = 0;  // IntelliSense has no inline assembly
#else
    __asm__ volatile("move $8, %0\n\tsw $31, 0($8)" : : "r"(&pPc) : "$8", "memory");  // store the return address of the caller in pPc
#endif
    MAIN.Scenario.pTask_cur->pTcb_pc = pPc;

    ChangeTh(0xFF000000U);
}

VOID Task_exit(VOID)
{
    TASK_CUR->Tcb_status = 0;
    EnterCriticalSection();
    CloseTh(MAIN.Scenario.pTask_cur->Tcb_th);
    ExitCriticalSection();
    ChangeTh(0xFF000000);
}

VOID Task_kill(INT Level)
{
    if ((Monitor.Task + Level)->Tcb_status != 0)
    {
        (Monitor.Task + Level)->Tcb_status = 0;
        EnterCriticalSection();
        CloseTh(Monitor.Task[Level].Tcb_th);
        ExitCriticalSection();
    }
}

VOID Task_chain(LONG (*pFuncall)())
{
    TASK* pCt;

    pCt = TASK_CUR;
    pCt->Tcb_status = 2;
    pCt->pTcb_pc = pFuncall;
    EnterCriticalSection();
    CloseTh(MAIN.Scenario.pTask_cur->Tcb_th);
    ExitCriticalSection();
    ChangeTh(0xFF000000);
}

VOID Task_suspend(INT Level)
{
    (Monitor.Task + Level)->Tcb_status |= 0x40;
}

VOID Task_signal(INT Level)
{
    (Monitor.Task + Level)->Tcb_status &= ~0x40;
}

INT Task_status(INT Level)
{
    return Monitor.Task[Level].Tcb_status;
}

// overlay API function table, copied into the scheduler workspace at start-up
VOID* Api_tbl_src[API_COUNT] =
{
    Task_execute,                        // 0 //
    Task_sleep,                          // 1 //
    Task_exit,                           // 2 //
    Task_kill,                           // 3 //
    Task_chain,                          // 4 //
    Task_suspend,                        // 5 //
    Task_signal,                         // 6 //
    Task_lexecute,                       // 7 //
    Task_lchain,                         // 8 //
    GsMapModelingData,                   // 9 //
    GsInitCoordinate2,                   // 10 //
    GsSetLightMatrix,                    // 11 //
    GsSetProjection,                     // 12 //
    GsPresetObject,                      // 13 //
    GsSetFlatLight,                      // 14 //
    GsSetLightMode,                      // 15 //
    GsLinkObject5,                       // 16 //
    GsSetLsMatrix,                       // 17 //
    GsSortObject5,                       // 18 //
    GsSetRefView2,                       // 19 //
    GsSetAmbient,                        // 20 //
    GsGetLs,                             // 21 //
    GsClearOt,                           // 22 //
    GsGetActiveBuff,                     // 23 //
    RotMatrix,                           // 24 //
    SsSetSerialAttr,                     // 25 //
    SsSetSerialVol,                      // 26 //
    GsSwapDispBuff,                      // 27 //
    Str_decode_wait,                     // 28 //
    GsInitGraph2,                        // 29 //
    Str_frm_get,                       // 30 //
    Close_movie,                         // 31 //
    GsInitGraph,                         // 32 //
    Play_movie,                          // 33 //
    Str_open,                    // 34 //
    ClearImage,                          // 35 //
    LoadImage,                           // 36 //
    DrawSync,                            // 37 //
    PadInit,                             // 38 //
    Pad_raw_get,                 // 39 //
    VSync,                               // 40 //
    Reset_graph,                      // 41 //
    Bg_img_buf_set,          // 42 //
    Evt_fade_start,                         // 43 //
    Mapping_tim,                         // 44 //
    Moji_tbl_print,            // 45 //
    LoadClut,                            // 46 //
    Snd_se_play,                            // 47 //
    Game_start,                          // 48 //
    Init_graph,                       // 49 //
    InitGeom,                            // 50 //
    GsDefDispBuff,                       // 51 //
    GsInit3D,                            // 52 //
    Cd_read,                             // 53 //
    Kage_work_set,                       // 54 //
    Kage_work_init,                      // 55 //
    Init_Parts_work,                     // 56 //
    Joint_move,                          // 57 //
    Oba_set_ofs,                         // 58 //
    Oba_ck_hit,                          // 59 //
    Oba_ck_em,                           // 60 //
    Sca_ck_point2,      // 61 //
    Sca_ck_hit,                          // 62 //
    Hit_ck_point4,                       // 63 //
    MulKage,                             // 64 //
    Joint_pos_trans,                     // 65 //
    Em_ck_hani,           // 66 //
    L_pos_ckXZ,                          // 67 //
    Sikai_ck,                            // 68 //
    Get_dir_tgt,            // 69 //
    rand,                                // 70 //
    Goto00_ck,                           // 71 //
    Add_speedXZ,                         // 72 //
    Goto00,                              // 73 //
    Em_sikai_proc,              // 74 //
    Cdir_ck,                             // 75 //
    Set_base_pos,                        // 76 //
    Null_pos_set,                        // 77 //
    Gacha_ch,                            // 78 //
    Esp_call,                            // 79 //
    Joint_atck_set,           // 80 //
    Kage_work_color_set,                 // 81 //
    Kage_work_size_add,                  // 82 //
    CompMatrix,                          // 83 //
    Square0,                             // 84 //
    SquareRoot0,                         // 85 //
    Snd_sys_end,                           // 86 //
    TransMatrix,                         // 87 //
    SetRotMatrix,                        // 88 //
    SetTransMatrix,                      // 89 //
    SetGeomScreen,                       // 90 //
    SetPolyFT4,                          // 91 //
    GetClut,                             // 92 //
    GetTPage,                            // 93 //
    SetSemiTrans,                        // 94 //
    RotTransPers4,                       // 95 //
    AddPrim,                             // 96 //
    GsSortFlipSprite,                    // 97 //
    GsSortFastSprite,                    // 98 //
    SsInit,                              // 99 //
    SsQuit,                              // 100 //
    Snd_title_init,                        // 101 //
    Snd_vab3_close,                   // 102 //
    Snd_sys_arms_set,                    // 103 //
    Snd_se_stad,                         // 104 //
    Evt_fade_req_set,                          // 105 //
    sprintf,                             // 106 //
    Print8,                              // 107 //
    Print14,                             // 108 //
    Em_wall_turn,       // 109 //
    RotMatrixY,                          // 110 //
    ApplyMatrixSV,                       // 111 //
    Model_color_plus,                    // 112 //
    MulMatrix,                           // 113 //
    Sca_ck_pos,          // 114 //
    Guide_check,                         // 115 //
    Root_edge_set,               // 116 //
    Rot_vector,                          // 117 //
    Xa_cmd,                       // 118 //
    Set_enemy_work,                      // 119 //
    Em_web_proc,             // 120 //
    ScaleMatrix,                         // 121 //
    Em_ck_alert,            // 122 //
    Kage_half_set,                  // 123 //
    Foot_set_pl,                         // 124 //
    L_pos_ckXZM,                         // 125 //
    Em_fall_move,               // 126 //
    Sca_ck_point_r,   // 127 //
    MulLMatrix,                          // 128 //
    Set_light,                           // 129 //
    MulMatrix0,                          // 130 //
    SetLightMatrix,                      // 131 //
    GsPrstTG3L,                          // 132 //
    GsMulCoord0,                         // 133 //
    Memcpy,                              // 134 //
    ApplyMatrixLV,                       // 135 //
    catan,                               // 136 //
    LoadAverageShort12,                  // 137 //
    Snd_se_walk,                         // 138 //
    Snd_se_enem,                         // 139 //
    SpuGetAllKeysStatus,                 // 140 //
    Rot_neck,                            // 141 //
    Flg_on,                              // 142 //
    GsMulCoord2,                         // 143 //
    LoadAverage12,                       // 144 //
    StoreImage,                          // 145 //
    MulMatrix2,                          // 146 //
    GsMulCoord3,                         // 147 //
    Flg_off,                             // 148 //
    Root_ck,                             // 149 //
    Rnd,                                 // 150 //
    Cinesco_fade_sabun_add,              // 151 //
    Cinesco_fade_set,                    // 152 //
    Cinesco_fade_off,                    // 153 //
    VectorNormal,                        // 154 //
    Card_access,                         // 155 //
    Em_wander_turn,           // 156 //
    Esp_call_api,                        // 157 //
    Flg_ck,                              // 158 //
    Sce_at_check,                        // 159 //
    Esp_init_C,                          // 160 //
    Esp_kill,                            // 161 //
    Esp_move,                            // 162 //
    Mapping_tmd,                         // 163 //
    Set_movie_volume2,                   // 164 //
    Cd_set_stereo,                       // 165 //
    Memclr,                              // 166 //
    srand,                               // 167 //
    GsPrstTG3NL,                         // 168 //
    Em0c_trail_alloc,                  // 169 //
    Em0c_trail_push,                   // 170 //
    Status_init,                         // 171 //
    GsSortSprite,                        // 172 //
    GsSortBoxFill,                       // 173 //
    Snd_vab1_load,        // 174 //
    Pad_vib_set0,                        // 175 //
    Pad_vib_set1,                        // 176 //
    Pad_vib_fade_set,                    // 177 //
    Config,                              // 178 //
    FntPrint,                            // 179 //
    Open_movie,                          // 180 //
    Cd_file_read,                        // 181 //
    0,                                         // 182 //
};
