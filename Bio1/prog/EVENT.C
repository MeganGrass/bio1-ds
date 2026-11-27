/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EVENT.C
 */

#include "TYPES.H"
#include "EVENT.H"
#include "PL_WEP.H"
#include "ROOM.H"
#include "SCE0.H"
#include "FLAG.H"
#include "SCE1.H"

VOID Event_init(SCE_TASK* pSce, INT Evt_no)
{
    pSce->Status = 1;
    VIEW(SHORT, pSce->Routine0) = 0;  // SHORT* view of the field (a plain access changes the code)
    pSce->Data = (UCHAR*)Sce_event_adr[Evt_no];  // byte view of the same memory
    pSce->pWork = EM_CUR;
    pSce->Sub_ctr = -1;
}

INT Event_exec(UINT Task_level, INT Evt_no)
{
    UINT i = Task_level;

    if (i >= 8)
    {
        i = 0;
        if (G.Scenario.Sce_task[0].Status != 0)
        {
            while (1)
            {
                if (i == 7)
                {
                    break;
                }
                i++;
                if (MAIN.Scenario.Sce_task[i].Status == 0)
                {
                    break;
                }
            }
        }
    }
    Event_init(&MAIN.Scenario.Sce_task[i], Evt_no);
}

VOID Sce_scheduler_main(VOID)
{
    SCE_TASK* pSce;
    INT D0;
    UINT Level;

    if (!(G.Stop_flg & STOP_SCE))
    {
        return;
    }
    for (Level = 0; Level < 8; Level++)
    {
        G.Scenario.pSce_cur = &MAIN.Scenario.Sce_task[Level];
        if (MAIN.Scenario.Sce_task[Level].Status == 0)
        {
            continue;
        }
    loop:
        pSce = G.Scenario.pSce_cur;
        D0 = *pSce->Data;
        switch (D0)
        {
        case 0xF6:
            SCE_CUR->Sub_ctr++;
            MAIN.Scenario.pSce_cur->Data++;
        case 0xF7:
            if (Flg_ck(&MAIN, 0xE) == 0 && !(G.SaveData.Mess_flg & MESS_FLG_ACTIVE))  // address passed as an integer
            {
                VIEW_VIA_CHAR(SCE_TASK*, G.Scenario.pSce_cur)->Data++;  // SCE_TASK* view of the field (a plain access changes the code)
                G.Scenario.pSce_cur->Sub_ctr--;
            }
            goto end;
        case 0xF8:
            SCE_CUR->Sub_ctr++;
            SCE_CUR->Loop_ctr[MAIN.Scenario.pSce_cur->Sub_ctr] = *(USHORT*)(MAIN.Scenario.pSce_cur->Data + 2);  // USHORT* view of the same memory
            MAIN.Scenario.pSce_cur->Data++;
        case 0xF9:
            if (--G.Scenario.pSce_cur->Loop_ctr[VIEW_VIA_CHAR(SCE_TASK*, G.Scenario.pSce_cur)->Sub_ctr] == 0)  // SCE_TASK* view of the field (a plain access changes the code)
            {
                SCE_NOW->Data += 3;
                SCE_NOW->Sub_ctr--;
            }
            goto end;
        case 0xFA:
            SCE_CUR->Sub_ctr++;
            SCE_CUR->Loop_ctr[MAIN.Scenario.pSce_cur->Sub_ctr] = *(USHORT*)(MAIN.Scenario.pSce_cur->Data + 2);  // USHORT* view of the same memory
            MAIN.Scenario.pSce_cur->pRet[MAIN.Scenario.pSce_cur->Sub_ctr] = MAIN.Scenario.pSce_cur->Data += 4;
            goto loop;
        case 0xFB:
            if (--VIEW_VIA_CHAR(SCE_TASK*, G.Scenario.pSce_cur)->Loop_ctr[G.Scenario.pSce_cur->Sub_ctr] != 0)  // SCE_TASK* view of the field (a plain access changes the code)
            {
                G.Scenario.pSce_cur->Data = G.Scenario.pSce_cur->pRet[G.Scenario.pSce_cur->Sub_ctr];
            }
            else
            {
                G.Scenario.pSce_cur->Data++;
                G.Scenario.pSce_cur->Sub_ctr--;
            }
            goto loop;
        case 0xFC:
            SCE_CUR->Sub_ctr++;
            SCE_CUR->Call_ptr[MAIN.Scenario.pSce_cur->Sub_ctr] = MAIN.Scenario.pSce_cur->Data + 2;
            MAIN.Scenario.pSce_cur->pRet[MAIN.Scenario.pSce_cur->Sub_ctr] = MAIN.Scenario.pSce_cur->Data += MAIN.Scenario.pSce_cur->Data[1];
            goto loop;
        case 0xFD:
            G.Scenario.pSce_data = SCE_CUR->Call_ptr[MAIN.Scenario.pSce_cur->Sub_ctr];
            if (Sce_jmp_tbl[*G.Scenario.pSce_data](D0, pSce) != 0)
            {
                MAIN.Scenario.pSce_cur->Data = MAIN.Scenario.pSce_cur->pRet[MAIN.Scenario.pSce_cur->Sub_ctr];
            }
            else
            {
                SCE_CUR->Data++;
                MAIN.Scenario.pSce_cur->Sub_ctr--;
            }
            goto loop;
        case 0xFF:
            G.Scenario.pSce_cur->Status = 0;
            goto inc;
        default:
            switch (pSce->Routine0)
            {
            case 0:
                switch (D0)
                {
                case 0:
                    G.Scenario.pSce_cur->Data++;
                    goto loop;
                case 1:
                    SCE_CUR->Routine0 = 1;
                    MAIN.Scenario.pSce_cur->Data++;
                    goto loop;
                case 2:
                    SCE_CUR->Routine0 = 2;
                    SCE_CUR->Data++;
                    MAIN.Scenario.pSce_cur->pWork->Routine_1 = 2;
                    MAIN.Scenario.pSce_cur->pWork->Routine_2 = 0;
                    MAIN.Scenario.pSce_cur->pWork->Routine_3 = 0;
                    goto loop;
                case 3:
                    SCE_CUR->Routine0 = 2;
                    MAIN.Scenario.pSce_cur->Data++;
                    goto loop;
                case 4:
                    Work_set(D0, pSce);
                    goto loop;
                case 5:
                    Evt_exec_cmd(D0, pSce);
                    goto loop;
                case 6:
                    Evt_sce_run(D0, pSce);
                    goto loop;
                case 7:
                    Evt_op_exec(D0, pSce);
                    goto loop;
                case 8:
                    Evt_chain(D0, pSce);
                    goto end;
                case 9:
                    MAIN.Scenario.Sce_task[SCE_CUR->Data[1]].Status = 0;
                    MAIN.Scenario.pSce_cur->Data += 2;
                    goto loop;
                default:
                    pSce->Status = 0;
                    return;
                }
            case 1:
                if (Evt_anm_set(D0, pSce) == 0)  // passes 2 arguments, Evt_anm_set declares 0
                {
                    goto end;
                }
                goto loop;
            case 2:
                Evt_spd_set(D0, pSce);  // passes 2 arguments, Evt_spd_set declares 0
                goto loop;
            case 3:
                if (Evt_move_set(D0, pSce) == 0)  // passes 2 arguments, Evt_move_set declares 0
                {
                    goto end;
                }
                goto loop;
            }
            goto loop;
        case 0xFE:
        inc:
            G.Scenario.pSce_cur->Data++;
        }
    end:
        if (*G.Scenario.pSce_cur->Data == 0xFF)
        {
            G.Scenario.pSce_cur->Status = 0;
        }
    }
}

INT Evt_anm_set(INT D0, SCE_TASK* pTask)
{
    ENEMY_WORK* pEw;
    EVT_CMD_REC* pScd;
    SHORT* pArg;
    INT v;
    UCHAR c;
    SCHAR t;

    pScd = (EVT_CMD_REC*)SCE_CUR->Data;  // reinterprets the memory as EVT_CMD_REC
    switch (pScd->Opcode)
    {
    case 0x81:
        MAIN.Scenario.pSce_cur->Data = (UCHAR*)&pScd->data0;  // byte view of the same memory
        MAIN.Scenario.pSce_cur->pWork->Neck_flg = *(USHORT*)pScd >> 8;  // halfword view of the same memory
        if ((SCE_CUR->pWork->Neck_flg & 0xF) == 0)
        {
            break;
        }
        MAIN.Scenario.pSce_cur->Data += 8;
        pEw = MAIN.Scenario.pSce_cur->pWork;
        if (pEw->Neck_flg == 0x93)
        {
            pArg = &pScd->data0;
            switch (*pArg++)
            {
            case 0:
                pEw->pTgt = PL_EM;
                break;
            case 1:
                pEw->pTgt = &MAIN.Enemy[*pArg];
                break;
            case 2:
                pEw->pTgt = (ENEMY_WORK*)Omodel_tbl[*pArg];  // reinterprets the memory as ENEMY_WORK
                break;
            case 3:
                pEw->pTgt = (ENEMY_WORK*)pOm_tbl[*pArg];  // reinterprets the memory as ENEMY_WORK
                break;
            }
            pArg += 2;
        }
        else
        {
            if (pEw->Neck_flg & 0x20)
            {
                pArg = &pScd->data0;
                if (*pArg < 0)
                {
                    pEw->Neck_point_x = *pArg++ + 0x1000;
                }
                else
                {
                    pEw->Neck_point_x = *pArg++;
                }
                if (*pArg < 0)
                {
                    G.Scenario.pSce_cur->pWork->Neck_point_y = *pArg++ + 0x1000;
                }
                else
                {
                    G.Scenario.pSce_cur->pWork->Neck_point_y = *pArg++;
                }
                G.Scenario.pSce_cur->pWork->Neck_point_z = *pArg++;
            }
            else
            {
                pEw->Neck_point_x = pScd->data0;
                MAIN.Scenario.pSce_cur->pWork->Neck_point_y = pScd->data1;
                MAIN.Scenario.pSce_cur->pWork->Neck_point_z = pScd->data2;
                pArg = BYTE_OFS(SHORT, pScd, 8);
            }
        }
        SCE_CUR->pWork->Neck_spd_y = *pArg & 0xFF ? *pArg : 0xC0;
        MAIN.Scenario.pSce_cur->pWork->Neck_spd_x = *pArg >> 8 ? *pArg >> 8 : 0x40;
        break;
    case 0x82:
        SCE_CUR->pWork->Neck_flg &= 0xEF;
        MAIN.Scenario.pSce_cur->Data++;
        break;
    case 0x83:
    {
        UINT a;
        UINT b;

        SCE_CUR_ROUTINE_UHALF0 = 8;
        a = *(USHORT*)MAIN.Scenario.pSce_cur->Data >> 8;  // halfword view of the same memory
        if (a & 0x10)
        {
            if (MAIN.Scenario.pSce_cur->pWork->Sca_info & 0x80)
            {
                MAIN.Scenario.pSce_cur->pWork->Routine_2 = a & 0xF;
            }
            else
            {
                SCE_CUR_ROUTINE_UHALF2 = a & 0xF;
                MAIN.Scenario.pSce_cur->pWork->Sca_info |= 0x80;
            }
        }
        else
        {
            SCE_CUR_ROUTINE_UHALF2 = a;
            MAIN.Scenario.pSce_cur->pWork->Sca_info &= 0x7F;
        }
        SCE_CUR->Data += 2;
        MAIN.Scenario.pSce_cur->pWork->Root_x = *(USHORT*)MAIN.Scenario.pSce_cur->Data;  // halfword view of the same memory
        MAIN.Scenario.pSce_cur->Data += 2;
        MAIN.Scenario.pSce_cur->pWork->Root_z = *(USHORT*)MAIN.Scenario.pSce_cur->Data;  // halfword view of the same memory
        MAIN.Scenario.pSce_cur->Data += 2;
        MAIN.Scenario.pSce_cur->pWork->Set_flg_no = *MAIN.Scenario.pSce_cur->Data++;
        SCE_CUR->pWork->Sce_free0 = 0x28;
        MAIN.Scenario.pSce_cur->pWork->Sce_flg = 0;
    }
    break;
    case 0x84:
    {
        UINT a;
        UINT b;

        SCE_CUR_ROUTINE_UHALF0 = 8;
        SCE_CUR_ROUTINE_UHALF2 = 1;
        a = *(USHORT*)MAIN.Scenario.pSce_cur->Data;  // halfword view of the same memory
        MAIN.Scenario.pSce_cur->Data += 2;
        b = *(USHORT*)MAIN.Scenario.pSce_cur->Data;  // halfword view of the same memory
        MAIN.Scenario.pSce_cur->Data += 2;
        MAIN.Scenario.pSce_cur->pWork->Move_no = a >> 8;
        MAIN.Scenario.pSce_cur->pWork->Set_flg_no = b;
        SCE_CUR->pWork->Sce_flg = (b & 0xFF00) >> 6;
        MAIN.Scenario.pSce_cur->pWork->Sce_free0 = 0;
    }
    break;
    case 0x89:
    {
        UINT a;

        a = *(USHORT*)pScd;  // halfword view of the same memory
        MAIN.Scenario.pSce_cur->Data = (UCHAR*)&pScd->data0;  // byte view of the same memory
        MAIN.Scenario.pSce_cur->pWork->Move_cnt = a >> 8;
        MAIN.Scenario.pSce_cur->pWork->Move_wait = 0;
        MAIN.Scenario.pSce_cur->pWork->Hokan_flg = 7;
        SCE_CUR->pWork->Spd_cur = 0;
        if (MAIN.Scenario.pSce_cur->pWork->Sce_flg & 0x20)
        {
            MAIN.Scenario.pSce_cur->pWork->Hokan_flg = 0;
        }
        if (MAIN.Scenario.pSce_cur->pWork->Id < 0x20)
        {
            if (MAIN.Scenario.pSce_cur->pWork->Move_no >= 0x10)
            {
                MAIN.Scenario.pSce_cur->pWork->Routine_3 = 3;
            }
            else
            {
                MAIN.Scenario.pSce_cur->pWork->Routine_3 = Aim_data.Anm_chain[MAIN.Scenario.pSce_cur->pWork->Move_no][0] + 1;
                MAIN.Scenario.pSce_cur->pWork->Move_no = Aim_data.Anm_chain[MAIN.Scenario.pSce_cur->pWork->Move_no][1];
            }
        }
        else
        {
            MAIN.Scenario.pSce_cur->pWork->Routine_3 = 1;
        }
    }
    break;
    case 0x8A:
    {
        UINT a;

        a = *(USHORT*)pScd;  // halfword view of the same memory
        MAIN.Scenario.pSce_cur->Data = (UCHAR*)&pScd->data0;  // byte view of the same memory
        MAIN.Scenario.pSce_cur->pWork->Move_cnt = a >> 8;
        MAIN.Scenario.pSce_cur->pWork->Move_wait = 0;
        MAIN.Scenario.pSce_cur->pWork->Hokan_flg = 7;
        SCE_CUR->pWork->Spd_cur = 0;
        if (MAIN.Scenario.pSce_cur->pWork->Sce_flg & 0x20)
        {
            MAIN.Scenario.pSce_cur->pWork->Hokan_flg = 0;
        }
    }
    break;
    case 0x85:
    {
        UINT a;
        UINT b;

        a = *(USHORT*)pScd;  // halfword view of the same memory
        MAIN.Scenario.pSce_cur->Data = (UCHAR*)&pScd->data0;  // byte view of the same memory
        b = (USHORT)pScd->data0;  // truncates to an unsigned halfword
        MAIN.Scenario.pSce_cur->Data = (UCHAR*)&pScd->data1;  // byte view of the same memory
        SCE_CUR_ROUTINE_UHALF0 = 8;
        SCE_CUR_ROUTINE_UHALF2 = a >> 8;
        MAIN.Scenario.pSce_cur->pWork->Move_no = b;
        MAIN.Scenario.pSce_cur->pWork->Set_flg_no = b >> 8;
        SCE_CUR->pWork->Sce_free0 = 0;
        MAIN.Scenario.pSce_cur->pWork->Sce_flg = 0;
    }
    break;
    case 0x86:
        SCE_CUR_ROUTINE_UHALF0 = 1;
        SCE_CUR_ROUTINE_UHALF2 = 0;
        MAIN.Scenario.pSce_cur->pWork->Damage_no = 0;
        MAIN.Scenario.pSce_cur->Data++;
        break;
    case 0x87:
    {
        UINT a;
        UINT b;

        a = *(USHORT*)pScd;  // halfword view of the same memory
        MAIN.Scenario.pSce_cur->Data = (UCHAR*)&pScd->data0;  // byte view of the same memory
        b = (USHORT)pScd->data0;  // truncates to an unsigned halfword
        MAIN.Scenario.pSce_cur->Data = (UCHAR*)&pScd->data1;  // byte view of the same memory
        switch (a >> 8)
        {
        case 0:
            MAIN.Scenario.pSce_cur->pWork->Sce_flg |= b;
            break;
        case 1:
            MAIN.Scenario.pSce_cur->pWork->Sce_flg = b;
            break;
        case 2:
            MAIN.Scenario.pSce_cur->pWork->Sce_flg ^= b;
            break;
        }
    }
    break;
    case 0x88:
    {
        UINT b;

        MAIN.Scenario.pSce_cur->Data = (UCHAR*)&pScd->data0;  // byte view of the same memory
        b = (USHORT)pScd->data0;  // truncates to an unsigned halfword
        MAIN.Scenario.pSce_cur->Data = (UCHAR*)&pScd->data1;  // byte view of the same memory
        MAIN.Scenario.pSce_cur->pWork->Sce_free0 = b;
    }
    break;
    case 0x80:
        MAIN.Scenario.pSce_cur->pWork->Routine_1 = 0;
    case 0x8B:
        SCE_CUR->Dummy01 = 0;
        SCE_CUR->Data++;
        MAIN.Scenario.pSce_cur->Routine0 = 0;
        break;
    case 0:
        MAIN.Scenario.pSce_cur->Data = &pScd->index;
        break;
    }
    return 1;
}

INT Evt_move_set(INT D0, SCE_TASK* pTask)
{
    UCHAR* Data;
    ENEMY_WORK* pEw;
    SCE_TASK* pSce2;
    SCE_TASK* pSce;
    SCE_TASK* pSce3;

    pSce = SCE_CUR;
    G.SaveData.Key = 0xFF;
    pEw = pSce->pWork;
    if (pEw->Routine_2 != *pSce->Data)
    {
        pEw->Routine_3 = 0;
    }
    pSce2 = MAIN.Scenario.pSce_cur;
    Data = pSce2->Data;
    pSce2->Data = Data + 1;
    pSce2->pWork->Routine_2 = *Data;
    pSce3 = MAIN.Scenario.pSce_cur;
    pSce3->Data = pSce3->Data + 1;
    return 1;
}

VOID Evt_spd_set(INT D0, SCE_TASK* pTask)
{
    VOID* pField;

    switch (*G.Scenario.pSce_cur->Data)
    {
    case 0:
        G.Scenario.pSce_cur->Data++;
        break;
    case 2:
        VIEW_VIA_CHAR(SCE_TASK*, G.Scenario.pSce_cur)->pWork->Pos_x += SCE_NOW->pWork->Spd_x;  // SCE_TASK* view of the field (a plain access changes the code)
        SCE_NOW->pWork->Pos_y += SCE_NOW->pWork->Spd_y;
        SCE_NOW->pWork->Pos_z += SCE_NOW->pWork->Spd_z;
        SCE_NOW->Data++;
        break;
    case 3:
        VIEW_VIA_CHAR(SCE_TASK*, G.Scenario.pSce_cur)->pWork->Cdir_x += SCE_NOW->pWork->Aspd_x;  // SCE_TASK* view of the field (a plain access changes the code)
        SCE_NOW->pWork->Cdir_y += SCE_NOW->pWork->Aspd_y;
        SCE_NOW->pWork->Cdir_z += ((OBJ_MODEL_WORK*)SCE_NOW->pWork)->Aspd_z;  // reinterprets the memory as OBJ_MODEL_WORK
        SCE_NOW->Data++;
        break;
    case 4:
        VIEW_VIA_CHAR(SCE_TASK*, G.Scenario.pSce_cur)->pWork->Pos_x += SCE_NOW->pWork->Spd_x;  // SCE_TASK* view of the field (a plain access changes the code)
        SCE_NOW->pWork->Pos_y += SCE_NOW->pWork->Spd_y;
        SCE_NOW->pWork->Pos_z += SCE_NOW->pWork->Spd_z;
        SCE_NOW->pWork->Cdir_x += SCE_NOW->pWork->Aspd_x;
        SCE_NOW->pWork->Cdir_y += SCE_NOW->pWork->Aspd_y;
        SCE_NOW->pWork->Cdir_z += ((OBJ_MODEL_WORK*)SCE_NOW->pWork)->Aspd_z;  // reinterprets the memory as OBJ_MODEL_WORK
        SCE_NOW->Data++;
        break;
    case 5:
        VIEW_VIA_CHAR(SCE_TASK*, G.Scenario.pSce_cur)->pWork->Spd_x = (SCHAR)SCE_NOW->Data[1];  // SCE_TASK* view of the field (a plain access changes the code)
        SCE_NOW->pWork->Spd_y = (SCHAR)SCE_NOW->Data[2];  // SCE_TASK* view of the field (a plain access changes the code)
        SCE_NOW->pWork->Spd_z = (SCHAR)SCE_NOW->Data[3];  // SCE_TASK* view of the field (a plain access changes the code)
        SCE_NOW->Data += 4;
        break;
    case 6:
        VIEW_VIA_CHAR(SCE_TASK*, G.Scenario.pSce_cur)->pWork->Aspd_x = (SCHAR)SCE_NOW->Data[1];  // SCE_TASK* view of the field (a plain access changes the code)
        SCE_NOW->pWork->Aspd_y = (SCHAR)SCE_NOW->Data[2];  // SCE_TASK* view of the field (a plain access changes the code)
        ((OBJ_MODEL_WORK*)SCE_NOW->pWork)->Aspd_z = (SCHAR)SCE_NOW->Data[3];  // SCE_TASK* view of the field (a plain access changes the code)
        SCE_NOW->Data += 4;
        break;
    case 7:
        VIEW_VIA_CHAR(SCE_TASK*, G.Scenario.pSce_cur)->pWork->Pos_x = ((EVT_CMD_REC*)SCE_NOW->Data)->data0;  // SCE_TASK* view of the field (a plain access changes the code)
        SCE_NOW->pWork->Pos_y = ((EVT_CMD_REC*)SCE_NOW->Data)->data1;  // reinterprets the memory as EVT_CMD_REC
        SCE_NOW->pWork->Pos_z = ((EVT_CMD_REC*)SCE_NOW->Data)->data2;  // reinterprets the memory as EVT_CMD_REC
        SCE_NOW->Data += 8;
        break;
    case 11:
        VIEW_VIA_CHAR(SCE_TASK*, G.Scenario.pSce_cur)->pWork->Cdir_x = ((EVT_CMD_REC*)SCE_NOW->Data)->data0;  // SCE_TASK* view of the field (a plain access changes the code)
        SCE_NOW->pWork->Cdir_y = ((EVT_CMD_REC*)SCE_NOW->Data)->data1;  // reinterprets the memory as EVT_CMD_REC
        SCE_NOW->pWork->Cdir_z = ((EVT_CMD_REC*)SCE_NOW->Data)->data2;  // reinterprets the memory as EVT_CMD_REC
        SCE_NOW->Data += 8;
        break;
    case 8:
        *(UCHAR*)(SCE_CUR->Data[1] + (INT)MAIN.Scenario.pSce_cur->pWork + 0x84) = MAIN.Scenario.pSce_cur->Data[2];  // SCE_TASK* view of the field (a plain access changes the code)
        MAIN.Scenario.pSce_cur->Data += 3;
        break;
    case 9:
        SCE_CUR->pWork->Damage_no = MAIN.Scenario.pSce_cur->Data[1];
        MAIN.Scenario.pSce_cur->Data += 2;
        break;
    case 10:
        switch (*(USHORT*)G.Scenario.pSce_cur->Data >> 8)  // halfword view of the same memory
        {
        case 0:
            pField = &G.Scenario.pSce_cur->pWork->Pos_x;
            break;
        case 1:
            pField = &G.Scenario.pSce_cur->pWork->Pos_y;
            break;
        case 2:
            pField = &G.Scenario.pSce_cur->pWork->Pos_z;
            break;
        case 3:
            pField = &G.Scenario.pSce_cur->pWork->Life;
            goto half;
        case 4:
            pField = &G.Scenario.pSce_cur->pWork->Root_x;
            goto half;
        case 5:
            pField = &G.Scenario.pSce_cur->pWork->Root_z;
            goto half;
        }
        *(INT*)pField = ((EVT_CMD_REC*)G.Scenario.pSce_cur->Data)->data0;  // INT* view of the same memory
        goto next;
    half:
        *(USHORT*)pField = ((EVT_CMD_REC*)G.Scenario.pSce_cur->Data)->data0;  // USHORT* view of the same memory
    next:
        G.Scenario.pSce_cur->Data += 4;
        break;
    case 1:
        SCE_CUR->Routine0 = 0;
        MAIN.Scenario.pSce_cur->Data++;
        break;
    }
}

VOID Work_set(INT D0, SCE_TASK* pTask)
{
    UCHAR* obj;
    UCHAR* pScd;

    pScd = VIEW_VIA_CHAR(SCE_TASK*, G.Scenario.pSce_cur)->Data;  // SCE_TASK* view of the field (a plain access changes the code)
    SCE_NOW->Data = pScd + 1;
    switch (pScd[1])
    {
    case 0:
        SCE_NOW->pWork = PL_EM;
        break;
    case 1:
        SCE_NOW->pWork = &MAIN.Enemy[pScd[2]];
        break;
    case 2:
        SCE_NOW->pWork = (ENEMY_WORK*)Omodel_tbl[pScd[2]];  // reinterprets the memory as ENEMY_WORK
        break;
    case 3:
        SCE_NOW->pWork = (ENEMY_WORK*)pOm_tbl[pScd[2]];  // reinterprets the memory as ENEMY_WORK
        break;
    }
    G.Scenario.pSce_cur->Data += 2;
}

VOID Evt_exec_cmd(INT D0, SCE_TASK* pTask)
{
    SCE_TASK* pSce;
    EVT_EXEC_REC* pScd;
    SCE_TASK* pSce2;

    pSce = SCE_CUR;
    pScd = (EVT_EXEC_REC*)pSce->Data;  // reinterprets the memory as EVT_EXEC_REC
    pSce->Data = (UCHAR*)pScd + 1;  // byte view of the same memory
    Event_exec(pScd->slot, pScd->script);
    pSce2 = MAIN.Scenario.pSce_cur;
    pSce2->Data = pSce2->Data + 3;
}

VOID Evt_chain(INT D0, SCE_TASK* pTask)
{
    Event_init(G.Scenario.pSce_cur, SCE_NOW->Data[1]);
}

VOID Evt_sce_run(INT D0, SCE_TASK* pTask)
{
    SCE_TASK* pSce;
    EVT_CMD_REC* pScd;

    Sce_scheduler(SCE_CUR->Data + 2);
    pSce = MAIN.Scenario.pSce_cur;
    pScd = (EVT_CMD_REC*)pSce->Data;  // reinterprets the memory as EVT_CMD_REC
    pSce->Data = pScd->index + pSce->Data;
}

VOID Evt_op_exec()
{
    UCHAR* pScd;
    USHORT h;

    pScd = G.Scenario.pSce_cur->Data;
    h = *(USHORT*)pScd;  // halfword view of the same memory
    G.Scenario.pSce_data = pScd + 2;
    G.Scenario.pSce_cur->Data = (h >> 8) + VIEW_VIA_CHAR(SCE_TASK*, G.Scenario.pSce_cur)->Data;  // SCE_TASK* view of the field (a plain access changes the code)
    Sce_jmp_tbl[(pScd)[2]](G.Scenario.pSce_cur, pScd);
}

VOID Evt_fade_start(VOID)
{
    if (FADE_KIDO_CUR <= 0 && G.Fade_add != 0)
    {
        MAIN.SaveData.Fade_kido = -(G.Fade_add < 1) & 0x7FFF;
    }
}

VOID Evt_fade_req_set(SCHAR fade_type, SHORT step)
{
    if (G.SaveData.Fade_kido <= 0)
    {
        G.Fade_hrate = fade_type;
        G.Fade_add = step;
        MAIN.System_flg |= SYS_FADE_OUT;
    }
}

// packed record block 00 referenced by em_se_tbl
UCHAR Em_se_dat00[0x90] =
{
    0xFF,   // 0 //
    0x07,   // 1 //
    0x00,   // 2 //
    0x04,   // 3 //
    0x01,   // 4 //
    0x02,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x04,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x70,   // 12 //
    0x17,   // 13 //
    0x60,   // 14 //
    0xF0,   // 15 //
    0xCA,   // 16 //
    0x08,   // 17 //
    0xFF,   // 18 //
    0x07,   // 19 //
    0x00,   // 20 //
    0x04,   // 21 //
    0x01,   // 22 //
    0x02,   // 23 //
    0x00,   // 24 //
    0x00,   // 25 //
    0x00,   // 26 //
    0x04,   // 27 //
    0x00,   // 28 //
    0x00,   // 29 //
    0x64,   // 30 //
    0x19,   // 31 //
    0x60,   // 32 //
    0xF0,   // 33 //
    0xCA,   // 34 //
    0x08,   // 35 //
    0xFF,   // 36 //
    0x07,   // 37 //
    0x00,   // 38 //
    0x04,   // 39 //
    0x01,   // 40 //
    0x02,   // 41 //
    0x00,   // 42 //
    0x00,   // 43 //
    0x00,   // 44 //
    0x0C,   // 45 //
    0x00,   // 46 //
    0x00,   // 47 //
    0x40,   // 48 //
    0x1F,   // 49 //
    0x60,   // 50 //
    0xF0,   // 51 //
    0x36,   // 52 //
    0x10,   // 53 //
    0xFF,   // 54 //
    0x07,   // 55 //
    0x00,   // 56 //
    0x04,   // 57 //
    0x01,   // 58 //
    0x02,   // 59 //
    0x00,   // 60 //
    0x00,   // 61 //
    0x00,   // 62 //
    0x0C,   // 63 //
    0x00,   // 64 //
    0x00,   // 65 //
    0x1C,   // 66 //
    0x25,   // 67 //
    0x60,   // 68 //
    0xF0,   // 69 //
    0x36,   // 70 //
    0x10,   // 71 //
    0xFF,   // 72 //
    0x07,   // 73 //
    0x00,   // 74 //
    0x04,   // 75 //
    0x01,   // 76 //
    0x02,   // 77 //
    0x00,   // 78 //
    0x00,   // 79 //
    0x00,   // 80 //
    0x08,   // 81 //
    0x00,   // 82 //
    0x00,   // 83 //
    0x06,   // 84 //
    0x09,   // 85 //
    0x60,   // 86 //
    0xF0,   // 87 //
    0x4C,   // 88 //
    0x1D,   // 89 //
    0xFF,   // 90 //
    0x07,   // 91 //
    0x00,   // 92 //
    0x04,   // 93 //
    0x01,   // 94 //
    0x02,   // 95 //
    0x00,   // 96 //
    0x00,   // 97 //
    0x00,   // 98 //
    0x08,   // 99 //
    0x00,   // 100 //
    0x00,   // 101 //
    0x06,   // 102 //
    0x09,   // 103 //
    0x60,   // 104 //
    0xF0,   // 105 //
    0x58,   // 106 //
    0x1B,   // 107 //
    0xFF,   // 108 //
    0x07,   // 109 //
    0x00,   // 110 //
    0x04,   // 111 //
    0x01,   // 112 //
    0x02,   // 113 //
    0x00,   // 114 //
    0x00,   // 115 //
    0x00,   // 116 //
    0x08,   // 117 //
    0x00,   // 118 //
    0x00,   // 119 //
    0x06,   // 120 //
    0x09,   // 121 //
    0x60,   // 122 //
    0xF0,   // 123 //
    0x70,   // 124 //
    0x17,   // 125 //
    0xFF,   // 126 //
    0x07,   // 127 //
    0x00,   // 128 //
    0x04,   // 129 //
    0x01,   // 130 //
    0x02,   // 131 //
    0x00,   // 132 //
    0x00,   // 133 //
    0x00,   // 134 //
    0x08,   // 135 //
    0x00,   // 136 //
    0x00,   // 137 //
    0x06,   // 138 //
    0x09,   // 139 //
    0x60,   // 140 //
    0xF0,   // 141 //
    0x7C,   // 142 //
    0x15,   // 143 //
};

// packed record block 01 referenced by em_se_tbl
UCHAR Em_se_dat01[0x24] =
{
    0xFF,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x05,   // 3 //
    0x01,   // 4 //
    0x02,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x04,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x1A,   // 12 //
    0x3B,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0x7B,   // 16 //
    0x32,   // 17 //
    0xFF,   // 18 //
    0x00,   // 19 //
    0x00,   // 20 //
    0x05,   // 21 //
    0x01,   // 22 //
    0x02,   // 23 //
    0x00,   // 24 //
    0x00,   // 25 //
    0x00,   // 26 //
    0x00,   // 27 //
    0x00,   // 28 //
    0x00,   // 29 //
    0x49,   // 30 //
    0x69,   // 31 //
    0x00,   // 32 //
    0x00,   // 33 //
    0xD4,   // 34 //
    0x20,   // 35 //
};

// packed record block 02 referenced by em_se_tbl
UCHAR Em_se_dat02[0xC8] =
{
    0xFF,   // 0 //
    0x0D,   // 1 //
    0x00,   // 2 //
    0x06,   // 3 //
    0x01,   // 4 //
    0x02,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x04,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x98,   // 12 //
    0x3A,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0xC8,   // 16 //
    0x32,   // 17 //
    0xFF,   // 18 //
    0x08,   // 19 //
    0x00,   // 20 //
    0x06,   // 21 //
    0x01,   // 22 //
    0x02,   // 23 //
    0x00,   // 24 //
    0x00,   // 25 //
    0x00,   // 26 //
    0x08,   // 27 //
    0xE4,   // 28 //
    0x0E,   // 29 //
    0x10,   // 30 //
    0x40,   // 31 //
    0xA8,   // 32 //
    0xE4,   // 33 //
    0xC8,   // 34 //
    0x32,   // 35 //
    0xFF,   // 36 //
    0x08,   // 37 //
    0x01,   // 38 //
    0x06,   // 39 //
    0x01,   // 40 //
    0x02,   // 41 //
    0x00,   // 42 //
    0x00,   // 43 //
    0x00,   // 44 //
    0x00,   // 45 //
    0xE4,   // 46 //
    0x0E,   // 47 //
    0x30,   // 48 //
    0x43,   // 49 //
    0xA8,   // 50 //
    0xE4,   // 51 //
    0xC8,   // 52 //
    0x32,   // 53 //
    0xFF,   // 54 //
    0x08,   // 55 //
    0x01,   // 56 //
    0x06,   // 57 //
    0x01,   // 58 //
    0x02,   // 59 //
    0x00,   // 60 //
    0x00,   // 61 //
    0xF4,   // 62 //
    0x0D,   // 63 //
    0xE4,   // 64 //
    0x0E,   // 65 //
    0x30,   // 66 //
    0x43,   // 67 //
    0xA8,   // 68 //
    0xE4,   // 69 //
    0x58,   // 70 //
    0x34,   // 71 //
    0xFF,   // 72 //
    0x08,   // 73 //
    0x01,   // 74 //
    0x06,   // 75 //
    0x01,   // 76 //
    0x02,   // 77 //
    0x00,   // 78 //
    0x00,   // 79 //
    0xE8,   // 80 //
    0x05,   // 81 //
    0xE4,   // 82 //
    0x0E,   // 83 //
    0x10,   // 84 //
    0x40,   // 85 //
    0xA8,   // 86 //
    0xE4,   // 87 //
    0x38,   // 88 //
    0x31,   // 89 //
    0xFF,   // 90 //
    0x08,   // 91 //
    0x01,   // 92 //
    0x06,   // 93 //
    0x01,   // 94 //
    0x02,   // 95 //
    0x00,   // 96 //
    0x00,   // 97 //
    0xF4,   // 98 //
    0x01,   // 99 //
    0xE4,   // 100 //
    0x0E,   // 101 //
    0x30,   // 102 //
    0x43,   // 103 //
    0xA8,   // 104 //
    0xE4,   // 105 //
    0x38,   // 106 //
    0x31,   // 107 //
    0xFF,   // 108 //
    0x08,   // 109 //
    0x01,   // 110 //
    0x06,   // 111 //
    0x01,   // 112 //
    0x02,   // 113 //
    0x00,   // 114 //
    0x00,   // 115 //
    0xC4,   // 116 //
    0x09,   // 117 //
    0xE4,   // 118 //
    0x0E,   // 119 //
    0x48,   // 120 //
    0x3F,   // 121 //
    0xA8,   // 122 //
    0xE4,   // 123 //
    0x58,   // 124 //
    0x34,   // 125 //
    0xFF,   // 126 //
    0x08,   // 127 //
    0x01,   // 128 //
    0x06,   // 129 //
    0x01,   // 130 //
    0x02,   // 131 //
    0x00,   // 132 //
    0x00,   // 133 //
    0xE8,   // 134 //
    0x03,   // 135 //
    0xE4,   // 136 //
    0x0E,   // 137 //
    0xA0,   // 138 //
    0x41,   // 139 //
    0xA8,   // 140 //
    0xE4,   // 141 //
    0x58,   // 142 //
    0x34,   // 143 //
    0xFF,   // 144 //
    0x08,   // 145 //
    0x01,   // 146 //
    0x06,   // 147 //
    0x01,   // 148 //
    0x02,   // 149 //
    0x00,   // 150 //
    0x00,   // 151 //
    0xE8,   // 152 //
    0x03,   // 153 //
    0xE4,   // 154 //
    0x0E,   // 155 //
    0xA0,   // 156 //
    0x41,   // 157 //
    0xA8,   // 158 //
    0xE4,   // 159 //
    0xC8,   // 160 //
    0x32,   // 161 //
    0xFF,   // 162 //
    0x07,   // 163 //
    0x00,   // 164 //
    0x06,   // 165 //
    0x01,   // 166 //
    0x02,   // 167 //
    0x00,   // 168 //
    0x00,   // 169 //
    0xE8,   // 170 //
    0x03,   // 171 //
    0x00,   // 172 //
    0x00,   // 173 //
    0xB0,   // 174 //
    0x36,   // 175 //
    0x00,   // 176 //
    0x00,   // 177 //
    0xB0,   // 178 //
    0x36,   // 179 //
    0xFF,   // 180 //
    0x03,   // 181 //
    0x02,   // 182 //
    0x06,   // 183 //
    0x01,   // 184 //
    0x02,   // 185 //
    0x00,   // 186 //
    0x08,   // 187 //
    0xE8,   // 188 //
    0x03,   // 189 //
    0x00,   // 190 //
    0x00,   // 191 //
    0xF8,   // 192 //
    0x2A,   // 193 //
    0xF0,   // 194 //
    0xD8,   // 195 //
    0x20,   // 196 //
    0x4E,   // 197 //
    0x00,   // 198 //
    0x00,   // 199 //
};

// packed record block 03 referenced by em_se_tbl
UCHAR Em_se_dat03[0x38] =
{
    0xFF,   // 0 //
    0x02,   // 1 //
    0x00,   // 2 //
    0x07,   // 3 //
    0x01,   // 4 //
    0x02,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x04,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x74,   // 12 //
    0x59,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0x38,   // 16 //
    0x3B,   // 17 //
    0xFF,   // 18 //
    0x00,   // 19 //
    0x01,   // 20 //
    0x07,   // 21 //
    0x01,   // 22 //
    0x02,   // 23 //
    0x00,   // 24 //
    0x00,   // 25 //
    0x00,   // 26 //
    0x04,   // 27 //
    0x00,   // 28 //
    0x00,   // 29 //
    0x74,   // 30 //
    0x59,   // 31 //
    0x00,   // 32 //
    0x00,   // 33 //
    0x38,   // 34 //
    0x3B,   // 35 //
    0xFF,   // 36 //
    0x15,   // 37 //
    0x00,   // 38 //
    0x07,   // 39 //
    0x01,   // 40 //
    0x02,   // 41 //
    0x00,   // 42 //
    0x00,   // 43 //
    0x78,   // 44 //
    0x05,   // 45 //
    0x00,   // 46 //
    0x00,   // 47 //
    0x18,   // 48 //
    0x47,   // 49 //
    0x00,   // 50 //
    0x00,   // 51 //
    0x40,   // 52 //
    0x1F,   // 53 //
    0x00,   // 54 //
    0x00,   // 55 //
};

// packed record block 04 referenced by em_se_tbl
UCHAR Em_se_dat04[0x14] =
{
    0xFF,   // 0 //
    0x03,   // 1 //
    0x02,   // 2 //
    0x08,   // 3 //
    0x01,   // 4 //
    0x02,   // 5 //
    0x00,   // 6 //
    0x08,   // 7 //
    0x00,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x9D,   // 12 //
    0x5D,   // 13 //
    0x18,   // 14 //
    0xE8,   // 15 //
    0x47,   // 16 //
    0x2E,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
};

// packed record block 05 referenced by em_se_tbl
UCHAR Em_se_dat05[0x14] =
{
    0xFF,   // 0 //
    0x14,   // 1 //
    0x00,   // 2 //
    0x09,   // 3 //
    0x01,   // 4 //
    0x02,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x04,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0xE0,   // 12 //
    0x2E,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0x90,   // 16 //
    0x28,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
};

// packed record block 06 referenced by em_se_tbl
UCHAR Em_se_dat06[0x14] =
{
    0xFF,   // 0 //
    0x00,   // 1 //
    0x03,   // 2 //
    0x0A,   // 3 //
    0x01,   // 4 //
    0x02,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x02,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0xD7,   // 12 //
    0x14,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0xAA,   // 16 //
    0x0F,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
};

// packed record block 07 referenced by em_se_tbl
UCHAR Em_se_dat07[0x14] =
{
    0xFF,   // 0 //
    0x0D,   // 1 //
    0x00,   // 2 //
    0x1D,   // 3 //
    0x01,   // 4 //
    0x02,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x04,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0xD4,   // 12 //
    0x30,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0xB8,   // 16 //
    0x0B,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
};

// packed record block 08 referenced by em_se_tbl
UCHAR Em_se_dat08[0x24] =
{
    0xFF,   // 0 //
    0x00,   // 1 //
    0x03,   // 2 //
    0x0F,   // 3 //
    0x01,   // 4 //
    0x02,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x08,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x03,   // 12 //
    0x46,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0xE6,   // 16 //
    0x25,   // 17 //
    0xFF,   // 18 //
    0x06,   // 19 //
    0x00,   // 20 //
    0x0F,   // 21 //
    0x01,   // 22 //
    0x02,   // 23 //
    0x00,   // 24 //
    0x00,   // 25 //
    0xDC,   // 26 //
    0x05,   // 27 //
    0x00,   // 28 //
    0x00,   // 29 //
    0x53,   // 30 //
    0x40,   // 31 //
    0x00,   // 32 //
    0x00,   // 33 //
    0xEC,   // 34 //
    0x12,   // 35 //
};

// packed record block 09 referenced by em_se_tbl
UCHAR Em_se_dat09[0x6C] =
{
    0xFF,   // 0 //
    0x19,   // 1 //
    0x00,   // 2 //
    0x11,   // 3 //
    0x01,   // 4 //
    0x02,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x40,   // 8 //
    0x06,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x38,   // 12 //
    0x18,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0x6C,   // 16 //
    0x20,   // 17 //
    0xFF,   // 18 //
    0x18,   // 19 //
    0x00,   // 20 //
    0x11,   // 21 //
    0x01,   // 22 //
    0x02,   // 23 //
    0x00,   // 24 //
    0x00,   // 25 //
    0x48,   // 26 //
    0x0D,   // 27 //
    0x00,   // 28 //
    0x00,   // 29 //
    0x94,   // 30 //
    0x11,   // 31 //
    0x00,   // 32 //
    0x00,   // 33 //
    0x50,   // 34 //
    0x14,   // 35 //
    0xFF,   // 36 //
    0x1A,   // 37 //
    0x00,   // 38 //
    0x11,   // 39 //
    0x01,   // 40 //
    0x02,   // 41 //
    0x00,   // 42 //
    0x00,   // 43 //
    0xA4,   // 44 //
    0x06,   // 45 //
    0x00,   // 46 //
    0x00,   // 47 //
    0xC8,   // 48 //
    0x19,   // 49 //
    0x00,   // 50 //
    0x00,   // 51 //
    0xE8,   // 52 //
    0x1C,   // 53 //
    0xFF,   // 54 //
    0x1B,   // 55 //
    0x00,   // 56 //
    0x11,   // 57 //
    0x01,   // 58 //
    0x02,   // 59 //
    0x00,   // 60 //
    0x00,   // 61 //
    0x28,   // 62 //
    0x0A,   // 63 //
    0x00,   // 64 //
    0x00,   // 65 //
    0x44,   // 66 //
    0x16,   // 67 //
    0x00,   // 68 //
    0x00,   // 69 //
    0x7C,   // 70 //
    0x15,   // 71 //
    0xFF,   // 72 //
    0x00,   // 73 //
    0x00,   // 74 //
    0x11,   // 75 //
    0x01,   // 76 //
    0x02,   // 77 //
    0x00,   // 78 //
    0x00,   // 79 //
    0x00,   // 80 //
    0x00,   // 81 //
    0x00,   // 82 //
    0x00,   // 83 //
    0xD8,   // 84 //
    0x0E,   // 85 //
    0x00,   // 86 //
    0x00,   // 87 //
    0x24,   // 88 //
    0x2C,   // 89 //
    0xFF,   // 90 //
    0x00,   // 91 //
    0x00,   // 92 //
    0x11,   // 93 //
    0x01,   // 94 //
    0x02,   // 95 //
    0x00,   // 96 //
    0x00,   // 97 //
    0x00,   // 98 //
    0x08,   // 99 //
    0x00,   // 100 //
    0x00,   // 101 //
    0xC8,   // 102 //
    0x4B,   // 103 //
    0x00,   // 104 //
    0x00,   // 105 //
    0xC0,   // 106 //
    0x2B,   // 107 //
};

// packed record block 10 referenced by em_se_tbl
UCHAR Em_se_dat10[0x14] =
{
    0xFF,   // 0 //
    0x00,   // 1 //
    0x02,   // 2 //
    0x12,   // 3 //
    0x01,   // 4 //
    0x02,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x08,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x10,   // 12 //
    0x27,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0x38,   // 16 //
    0x31,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
};

// packed record block 11 referenced by em_se_tbl
UCHAR Em_se_dat11[0x14] =
{
    0xFF,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x13,   // 3 //
    0x01,   // 4 //
    0x02,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x0C,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x28,   // 12 //
    0x23,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0x68,   // 16 //
    0x10,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
};

// packed record block 12 referenced by em_se_tbl
UCHAR Em_se_dat12[0x14] =
{
    0xFF,   // 0 //
    0x14,   // 1 //
    0x00,   // 2 //
    0x15,   // 3 //
    0x01,   // 4 //
    0x02,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x00,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x20,   // 12 //
    0x03,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0xEC,   // 16 //
    0x13,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
};

// packed record block 13 referenced by em_se_tbl
UCHAR Em_se_dat13[0x14] =
{
    0xFF,   // 0 //
    0x00,   // 1 //
    0x00,   // 2 //
    0x16,   // 3 //
    0x01,   // 4 //
    0x02,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0xAC,   // 8 //
    0x0D,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x1C,   // 12 //
    0x0B,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0x53,   // 16 //
    0x0B,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
};

// packed record block 14 referenced by em_se_tbl
UCHAR Em_se_dat14[0x6C] =
{
    0xFF,   // 0 //
    0x05,   // 1 //
    0x00,   // 2 //
    0x17,   // 3 //
    0x01,   // 4 //
    0x02,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x04,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x65,   // 12 //
    0x4C,   // 13 //
    0x84,   // 14 //
    0xEA,   // 15 //
    0xD8,   // 16 //
    0x27,   // 17 //
    0xFF,   // 18 //
    0x05,   // 19 //
    0x00,   // 20 //
    0x17,   // 21 //
    0x01,   // 22 //
    0x02,   // 23 //
    0x00,   // 24 //
    0x00,   // 25 //
    0x00,   // 26 //
    0x04,   // 27 //
    0x00,   // 28 //
    0x00,   // 29 //
    0x44,   // 30 //
    0x48,   // 31 //
    0x84,   // 32 //
    0xEA,   // 33 //
    0xD8,   // 34 //
    0x27,   // 35 //
    0xFF,   // 36 //
    0x05,   // 37 //
    0x00,   // 38 //
    0x17,   // 39 //
    0x01,   // 40 //
    0x02,   // 41 //
    0x00,   // 42 //
    0x00,   // 43 //
    0x00,   // 44 //
    0x04,   // 45 //
    0x00,   // 46 //
    0x00,   // 47 //
    0x5C,   // 48 //
    0x44,   // 49 //
    0x84,   // 50 //
    0xEA,   // 51 //
    0xD8,   // 52 //
    0x27,   // 53 //
    0xFF,   // 54 //
    0x05,   // 55 //
    0x00,   // 56 //
    0x17,   // 57 //
    0x01,   // 58 //
    0x02,   // 59 //
    0x00,   // 60 //
    0x00,   // 61 //
    0x00,   // 62 //
    0xFC,   // 63 //
    0x00,   // 64 //
    0x00,   // 65 //
    0x98,   // 66 //
    0x3A,   // 67 //
    0x78,   // 68 //
    0xEC,   // 69 //
    0x28,   // 70 //
    0x0A,   // 71 //
    0xFF,   // 72 //
    0x05,   // 73 //
    0x00,   // 74 //
    0x17,   // 75 //
    0x01,   // 76 //
    0x02,   // 77 //
    0x00,   // 78 //
    0x00,   // 79 //
    0x00,   // 80 //
    0xFC,   // 81 //
    0x00,   // 82 //
    0x00,   // 83 //
    0xE0,   // 84 //
    0x2E,   // 85 //
    0x78,   // 86 //
    0xEC,   // 87 //
    0x28,   // 88 //
    0x0A,   // 89 //
    0xFF,   // 90 //
    0x05,   // 91 //
    0x00,   // 92 //
    0x17,   // 93 //
    0x01,   // 94 //
    0x02,   // 95 //
    0x00,   // 96 //
    0x00,   // 97 //
    0x00,   // 98 //
    0xFC,   // 99 //
    0x00,   // 100 //
    0x00,   // 101 //
    0x70,   // 102 //
    0x17,   // 103 //
    0x78,   // 104 //
    0xEC,   // 105 //
    0x28,   // 106 //
    0x0A,   // 107 //
};

// packed record block 15 referenced by em_se_tbl
UCHAR Em_se_dat15[0x14] =
{
    0xFF,   // 0 //
    0x06,   // 1 //
    0x02,   // 2 //
    0x1A,   // 3 //
    0x01,   // 4 //
    0x02,   // 5 //
    0x00,   // 6 //
    0x00,   // 7 //
    0x00,   // 8 //
    0x04,   // 9 //
    0x00,   // 10 //
    0x00,   // 11 //
    0x31,   // 12 //
    0x0D,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0x25,   // 16 //
    0x5D,   // 17 //
    0x00,   // 18 //
    0x00,   // 19 //
};

// 13 leading data words, then 50 { word, record block pointer } pairs; one pair points back at the table start
UINT em_se_tbl[113] =
{
    0x1B0001FF,                    // 0 //
    0x00000200,                    // 1 //
    0x00000800,                    // 2 //
    0x00004A6A,                    // 3 //
    0x00006464,                    // 4 //
    0x00000000,                    // 5 //
    0x00000000,                    // 6 //
    0x00000000,                    // 7 //
    0x00000000,                    // 8 //
    0x00000000,                    // 9 //
    0x00000000,                    // 10 //
    0x00000000,                    // 11 //
    0x00000000,                    // 12 //
    0x00000008,                    // 13 //
    (UINT)Em_se_dat00,   // 14 //
    0x00000002,                    // 15 //
    (UINT)Em_se_dat01,   // 16 //
    0x00000001,                    // 17 //
    (UINT)Em_se_dat02,   // 18 //
    0x00000001,                    // 19 //
    (UINT)Em_se_dat03,   // 20 //
    0x00000001,                    // 21 //
    (UINT)Em_se_dat04,   // 22 //
    0x00000001,                    // 23 //
    (UINT)Em_se_dat05,   // 24 //
    0x00000001,                    // 25 //
    (UINT)Em_se_dat06,   // 26 //
    0x00000000,                    // 27 //
    0x00000000,                    // 28 //
    0x00000000,                    // 29 //
    0x00000000,                    // 30 //
    0x00000000,                    // 31 //
    0x00000000,                    // 32 //
    0x00000000,                    // 33 //
    0x00000000,                    // 34 //
    0x00000001,                    // 35 //
    (UINT)Em_se_dat08,   // 36 //
    0x00000000,                    // 37 //
    0x00000000,                    // 38 //
    0x00000004,                    // 39 //
    (UINT)Em_se_dat09,   // 40 //
    0x00000001,                    // 41 //
    (UINT)Em_se_dat10,   // 42 //
    0x00000000,                    // 43 //
    (UINT)Em_se_dat11,   // 44 //
    0x00000000,                    // 45 //
    0x00000000,                    // 46 //
    0x00000001,                    // 47 //
    (UINT)Em_se_dat12,   // 48 //
    0x00000000,                    // 49 //
    (UINT)Em_se_dat13,   // 50 //
    0x00000006,                    // 51 //
    (UINT)Em_se_dat14,   // 52 //
    0x00000000,                    // 53 //
    0x00000000,                    // 54 //
    0x00000000,                    // 55 //
    0x00000000,                    // 56 //
    0x00000001,                    // 57 //
    (UINT)Em_se_dat15,   // 58 //
    0x00000001,                    // 59 //
    (UINT)em_se_tbl,               // 60 //
    0x00000000,                    // 61 //
    0x00000000,                    // 62 //
    0x00000001,                    // 63 //
    (UINT)Em_se_dat07,   // 64 //
    0x00000000,                    // 65 //
    0x00000000,                    // 66 //
    0x00000000,                    // 67 //
    0x00000000,                    // 68 //
    0x00000000,                    // 69 //
    0x00000000,                    // 70 //
    0x00000000,                    // 71 //
    0x00000000,                    // 72 //
    0x00000000,                    // 73 //
    0x00000000,                    // 74 //
    0x00000000,                    // 75 //
    0x00000000,                    // 76 //
    0x00000000,                    // 77 //
    0x00000000,                    // 78 //
    0x00000000,                    // 79 //
    0x00000000,                    // 80 //
    0x00000000,                    // 81 //
    0x00000000,                    // 82 //
    0x00000000,                    // 83 //
    0x00000000,                    // 84 //
    0x00000000,                    // 85 //
    0x00000000,                    // 86 //
    0x00000000,                    // 87 //
    0x00000000,                    // 88 //
    0x00000000,                    // 89 //
    0x00000000,                    // 90 //
    0x00000000,                    // 91 //
    0x00000000,                    // 92 //
    0x00000000,                    // 93 //
    0x00000000,                    // 94 //
    0x00000000,                    // 95 //
    0x00000000,                    // 96 //
    0x00000000,                    // 97 //
    0x00000000,                    // 98 //
    0x00000000,                    // 99 //
    0x00000000,                    // 100 //
    0x00000000,                    // 101 //
    0x00000000,                    // 102 //
    0x00000000,                    // 103 //
    0x00000000,                    // 104 //
    0x00000000,                    // 105 //
    0x00000000,                    // 106 //
    0x00000000,                    // 107 //
    0x00000000,                    // 108 //
    0x00000000,                    // 109 //
    0x00000000,                    // 110 //
    0x00000000,                    // 111 //
    0x00000000,                    // 112 //
};
