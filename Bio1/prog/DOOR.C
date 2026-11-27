/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\DOOR.C
 */

#include "DOOR.H"
#include <RAND.H>
#include "SUB.H"
#include "GAME.H"
#include "SCRATCHPAD.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "MONITOR.H"

DOOR_WORK Door_work[12]; // door objects: coordinate system, model, velocities and rotation
UCHAR Door_snd_bank; // sound bank of the door, script byte variable 1
DOOR_VIEW_REC Door_camera; // door camera: view point and reference point, and their deltas
DOOR_CMD_WORK* Door_cmd_now; // command slot being run

SCREEN_SHAKE_WORK Door_shake; // screen shake offsets: x in the first byte, y in the second

INT Door_status; // door script state: bit 0 running, bit 1 dispatching scripts
UCHAR* Door_obj_tbl; // model table of the door file
UCHAR Door_anim_type; // door animation type, script byte variable 3
ULONG* pDoor_pkt_adr; // next free polygon packet of the door objects
UCHAR Door_byte_var5; // script byte variable 5, a scratch value

VOID* Gs_packet_p[2]; // polygon packet area of each of the two draw buffers

DOOR_CMD_WORK Door_cmd_slot[8]; // script command slots

INT Door_op_end(VOID)
{
    Door_status = 0;
    return 0;
}

INT Door_op_clear(VOID)
{
    DOOR_CMD_WORK* p = Door_cmd_now;

    p->active = 0;
    p->Loop_num = 0;
    p->pOp = 0;
    p->Loop_adr[0] = 0;
    p->Loop_cnt[0] = 0;
    return 0;
}

INT Door_op_wait_free(VOID)
{
    DOOR_OP_REC* op;

    op = Door_cmd_now->pOp;
    if (Door_cmd_slot[op->index].active != 0)
    {
        return 0;
    }
    Door_cmd_now->pOp = &op->data0;
    return 1;
}

INT Door_op_yield2(VOID)
{
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 2;
    return 0;
}

INT Door_op_jmp_byte(VOID)
{
    DOOR_OPB_REC* p;
    SCHAR op;
    SCHAR* var;
    SCHAR val;

    p = Door_cmd_now->pOp;  // reinterprets the memory as DOOR_OPB_REC
    op = p->data1;
    var = Door_byte_tbl[(SCHAR)p->data0];  // byte view of the same memory
    val = p->data2;
    switch (op)
    {
    case 0:
        if (*var == val)  // sign-extends the byte
        {
            goto skip;
        }
        break;
    case 1:
        if (*var > val)  // sign-extends the byte
        {
            goto skip;
        }
        break;
    case 2:
        if (*var >= val)  // sign-extends the byte
        {
            goto skip;
        }
        break;
    case 3:
        if (*var < val)  // sign-extends the byte
        {
            goto skip;
        }
        break;
    case 4:
        if (*var <= val)  // sign-extends the byte
        {
            goto skip;
        }
        break;
    case 5:
        if (*var != val)  // sign-extends the byte
        {
            goto skip;
        }
        break;
    }
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + p->index;
skip:
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 6;
    return 1;
}

INT Door_op_jmp_short(VOID)
{
    DOOR_OP_REC* p;
    SCHAR op;
    SHORT* var;
    SHORT val;

    p = Door_cmd_now->pOp;  // reinterprets the memory as DOOR_OP_REC
    op = ((UCHAR*)p)[3];  // byte view of the same memory
    var = Door_short_tbl[(SCHAR)((UCHAR*)p)[2]];  // halfword view of the same memory
    val = p->data1;
    switch (op)
    {
    case 0:
        if (*var == val)  // sign-extends the halfword
        {
            goto skip;
        }
        break;
    case 1:
        if (*var > val)  // sign-extends the halfword
        {
            goto skip;
        }
        break;
    case 2:
        if (*var >= val)  // sign-extends the halfword
        {
            goto skip;
        }
        break;
    case 3:
        if (*var < val)  // sign-extends the halfword
        {
            goto skip;
        }
        break;
    case 4:
        if (*var <= val)  // sign-extends the halfword
        {
            goto skip;
        }
        break;
    case 5:
        if (*var != val)  // sign-extends the halfword
        {
            goto skip;
        }
        break;
    }
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + p->index;
skip:
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 6;
    return 1;
}

INT Door_op_loop_push(VOID)
{
    Door_cmd_now->Loop_adr[Door_cmd_now->Loop_num] = (INT)((UCHAR*)Door_cmd_now->pOp + 4);  // signed conversion (unsigned operand)
    Door_cmd_now->Loop_cnt[Door_cmd_now->Loop_num] = ((DOOR_OP_REC*)Door_cmd_now->pOp)->data0;  // reinterprets the memory as DOOR_OP_REC
    Door_cmd_now->Loop_num = Door_cmd_now->Loop_num + 1;
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 4;
    return 1;
}

INT Door_op_loop(VOID)
{
    USHORT count;
    DOOR_CMD_WORK* pCmd;
    INT four;
    pCmd = Door_cmd_now;
    pCmd = BYTE_OFS(DOOR_CMD_WORK, pCmd, Door_cmd_now->Loop_num * 2);
    four = 4;
    count = (SHORT)pCmd->Loop_cnt[-1] - 1;  // sign-extends the halfword
    pCmd->Loop_cnt[-1] = count;
    if ((count << 0x10) <= 0)
    {
        Door_cmd_now->Loop_num = Door_cmd_now->Loop_num - 1;
        *((INT*)((UCHAR*)Door_cmd_now + four)) = (INT)((UCHAR*)Door_cmd_now->pOp + 2);  // real width or sign conversion
        return 1;
    }
    Door_cmd_now->pOp = *((VOID**)((UCHAR*)Door_cmd_now + Door_cmd_now->Loop_num * four + four));
    return 0;
}

INT Door_op_activate(VOID)
{
    USHORT packed;

    packed = ((DOOR_OP_REC*)Door_cmd_now->pOp)->data0;  // reinterprets the memory as DOOR_OP_REC
    Door_cmd_slot[packed & 0xFF].active = 1;
    Door_cmd_slot[packed & 0xFF].Loop_num = 0;
    Door_cmd_slot[packed & 0xFF].pOp = (VOID*)(*(INT**)LOAD_BUF)[packed >> 8];  // untyped pointer, the callee takes the address as is
    Door_cmd_slot[packed & 0xFF].Loop_cnt[0] = 0;
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 4;
    return 1;
}

INT Door_op_clear_entry(VOID)
{
    INT i;

    i = * ((USHORT*)Door_cmd_now->pOp) >> 8;  // halfword view of the same memory
    Door_cmd_slot[i].active = 0;
    Door_cmd_slot[i].Loop_num = 0;
    Door_cmd_slot[i].pOp = 0;
    Door_cmd_slot[i].Loop_adr[0] = 0;
    Door_cmd_slot[i].Loop_cnt[0] = 0;
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 2;
    return 1;
}

INT Door_op_byte_set(VOID)
{
    DOOR_OPB_REC* op;

    op = Door_cmd_now->pOp;
    *(SCHAR*)Door_byte_tbl[op->index] = op->data0;  // SCHAR* view of the same memory
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 4;
    return 1;
}

INT Door_op_byte_add(VOID)
{
    UCHAR* var;
    DOOR_OPB_REC* op;

    op = Door_cmd_now->pOp;
    var = Door_byte_tbl[op->index];
    *var += op->data0;
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 4;
    return 1;
}

INT Door_op_short_set(VOID)
{
    DOOR_OP_REC* op;

    op = Door_cmd_now->pOp;
    *(SHORT*)Door_short_tbl[op->index] = op->data0;  // SHORT* view of the same memory
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 4;
    return 1;
}

INT Door_op_short_add(VOID)
{
    USHORT* var;
    DOOR_OP_REC* op;

    op = Door_cmd_now->pOp;
    var = Door_short_tbl[op->index];
    *var += op->data0;
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 4;
    return 1;
}

INT Door_op_fade_in(VOID)
{
    UCHAR* op;

    op = Door_cmd_now->pOp;
    G.Fade_hrate = op[1];
    G.Fade_add = -(*(USHORT*)(op + 2) << 7);  // halfword view of the same memory
    FADE_KIDO_CUR = *(USHORT*)(op + 4);  // unsigned halfword of the command bytes
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 6;
    return 1;
}

INT Door_op_fade_out(VOID)
{
    UCHAR* op;

    op = Door_cmd_now->pOp;
    G.Fade_hrate = op[1];
    G.Fade_add = *(USHORT*)(op + 2) << 7;  // halfword view of the same memory
    FADE_KIDO_CUR = *(USHORT*)(op + 4);  // unsigned halfword of the command bytes
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 6;
    return 1;
}

INT Door_model_set(VOID)
{
    DOOR_OP_REC* rec;
    DOOR_WORK* obj;
    GsDOBJ5* dobj;
    USHORT f;

    INT t;
    DOOR_CMD_WORK* g;

    rec = Door_cmd_now->pOp;
    obj = &Door_work[rec->index];
    if (((UCHAR*)rec)[3] == 0xFF)  // byte view of the same memory
    {
        GsInitCoordinate2(0, &obj->coord);
    }
    else
    {
        GsInitCoordinate2(&Door_work[((UCHAR*)rec)[3]].coord, &obj->coord);  // byte view of the same memory
    }
    dobj = &obj->dobj;
    if (rec->data1 & 0x8000)
    {
        do
        {
            GsLinkObject5((ULONG)Door_obj_tbl, dobj, ((UCHAR*)rec)[2]);  // unsigned conversion (signed operand)
            pDoor_pkt_adr = GsPresetObject(dobj, pDoor_pkt_adr);
        } while (0);
        f = obj->flags;
        obj->dobj.coord2 = &obj->coord;
        if (f & 0x2000)
        {
            obj->dobj.attribute = 0;
        }
        else
        {
            obj->dobj.attribute = 0x40;
        }
    }

    do
    {
        do
        {
            t = ((UCHAR*)rec)[2];  // byte view of the same memory
        } while (0);
    } while (0);
    g = Door_cmd_now;
    {
        UINT tmd_ofs = t * sizeof(TMD_MEM);
        obj->vert = ((TMD_HEAD*)((*(UCHAR**)DOOR_TMD_OFS_ADDR) + tmd_ofs))->obj0_vert_top;  // byte view of the same memory
    }

    obj->flags = rec->data1;
    obj->id = ((UCHAR*)rec)[2];  // byte view of the same memory

    g->pOp = (UCHAR*)g->pOp + 6;

    return 1;
}

INT Door_op_cam_set(VOID)
{
    SHORT* src;
    INT* p;

    GsSetProjection(0x101);
    p = Door_camera.v;
    src = (VOID*)((UCHAR*)Door_cmd_now->pOp + 2);
    for (; p <= Door_camera.v + 5; p++)
    {
        *p = *src++ << ((DOOR_OP_REC*)Door_cmd_now->pOp)->index;  // reinterprets the memory as DOOR_OP_REC
    }
    do
    {
        p[0] = 0;
        p[1] = 0;
        Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 0xE;
    } while (0);
    return 1;
}

INT Door_op_sabun_set(VOID)
{
    USHORT* end;
    USHORT* src;
    USHORT* dst;

    dst = Door_camera.d;
    end = &Door_camera.d[5];
    src = (VOID*)((UCHAR*)Door_cmd_now->pOp + 2);
    if ((UINT)end >= (UINT)Door_camera.d)  // unsigned conversion (signed operand)
    {
        do
        {
            *dst = *src;
            dst += 1;
            src += 1;
        } while (end >= dst);
    }
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 0xE;
    return 1;
}

INT Door_op_cam_add(VOID)
{
    INT* p = Door_camera.v;
    SHORT* q;

    for (q = Door_camera.d; q <= &Door_camera.d[5]; q++)
    {
        *p++ += *q;
    }
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 2;
    return 1;
}

INT Door_op_sabun_add(VOID)
{
    USHORT* delta;
    DOOR_OPB_REC* op;

    op = Door_cmd_now->pOp;
    delta = &Door_camera.d[op->data0];
    *delta += (SCHAR)op->data1;  // real width or sign conversion
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 4;
    return 1;
}

INT Door_op_pos_set(VOID)
{
    DOOR_OP_REC* op;
    INT* pos;

    op = Door_cmd_now->pOp;  // reinterprets the memory as DOOR_OP_REC
    pos = BYTE_OFS(INT, Door_work[0].coord.coord.t, op->index * sizeof(DOOR_WORK));
    pos[0] = (SHORT)op->data0;  // sign-extends the halfword
    pos[1] = (SHORT)op->data1;  // sign-extends the halfword
    pos[2] = op->data2;  // sign-extends the halfword
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 8;
    return 1;
}

INT Door_op_vel_set(VOID)
{
    DOOR_OP_REC* op;
    USHORT* vel;

    op = Door_cmd_now->pOp;  // reinterprets the memory as DOOR_OP_REC
    vel = Door_work[op->index].vel;
    vel[0] = op->data0;
    vel[1] = op->data1;
    vel[2] = op->data2;
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 8;
    return 1;
}

INT Door_op_pos_add(VOID)
{
    INT Ord_ofs;
    SHORT* vel;
    INT* pos;

    Ord_ofs = ((DOOR_OP_REC*)Door_cmd_now->pOp)->index * sizeof(DOOR_WORK);  // reinterprets the memory as DOOR_OP_REC
    pos = BYTE_OFS(INT, Door_work[0].coord.coord.t, Ord_ofs);
    vel = BYTE_OFS(SHORT, Door_work[0].vel, Ord_ofs);
    pos[0] += vel[0];
    pos[1] += vel[1];
    pos[2] += vel[2];
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 2;
    return 1;
}

INT Door_op_vel_add(VOID)
{
    DOOR_OPB_REC* p;
    USHORT* q;

    p = Door_cmd_now->pOp;  // reinterprets the memory as DOOR_OPB_REC
    q = &Door_work[p->index].vel[p->data0];
    *q += (SCHAR)p->data1;  // real width or sign conversion

    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 4;
    return 1;
}

INT Door_op_rot_set(VOID)
{
    DOOR_OP_REC* op;
    USHORT* rot;

    op = Door_cmd_now->pOp;  // reinterprets the memory as DOOR_OP_REC
    rot = (USHORT*)&Door_work[op->index].rot;  // halfword view of the same memory
    rot[0] = op->data0;
    rot[1] = op->data1;
    rot[2] = op->data2;
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 8;
    return 1;
}

INT Door_op_rvel_set(VOID)
{
    SHORT* rotvel;
    DOOR_OPB_REC* op;

    op = Door_cmd_now->pOp;  // reinterprets the memory as DOOR_OPB_REC
    rotvel = Door_work[op->index].rotvel;
    rotvel[0] = (SCHAR)op->data0;  // sign-extends the byte
    rotvel[1] = (SCHAR)op->data1;  // sign-extends the byte
    rotvel[2] = op->data2;  // sign-extends the byte
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 6;
    return 1;
}

INT Door_op_rot_add(VOID)
{
    USHORT* rotvel;
    USHORT* rot;

    rot = (USHORT*)&Door_work[((DOOR_OP_REC*)Door_cmd_now->pOp)->index].rot;  // halfword view of the same memory
    rotvel = Door_work[((DOOR_OP_REC*)Door_cmd_now->pOp)->index].rotvel;  // halfword view of the same memory
    rot[0] += rotvel[0];
    rot[1] += rotvel[1];
    rot[2] += rotvel[2];
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 2;
    return 1;
}

INT Door_op_rvel_add(VOID)
{
    DOOR_OPB_REC* p;
    USHORT* q;

    p = Door_cmd_now->pOp;  // reinterprets the memory as DOOR_OPB_REC
    q = &Door_work[p->index].rotvel[p->data0];
    *q += (SCHAR)p->data1;  // real width or sign conversion
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 4;
    return 1;
}

INT Door_op_vtx_set(VOID)
{
    DOOR_CMD_WORK* obj;
    DOOR_OP_REC* p;
    USHORT* q;

    obj = Door_cmd_now;
    p = obj->pOp;
    q = BYTE_OFS(USHORT, &Door_work[p->index].vert, ((UCHAR*)p)[2] * 8);
    q[0] = p->data1;
    q[1] = p->data2;
    q[2] = p->data3;
    obj->pOp = (UCHAR*)obj->pOp + 0xA;
    return 1;
}

INT Door_op_vtx_add(VOID)
{
    DOOR_VTX_REC* vert;
    DOOR_OP_REC* op;

    op = Door_cmd_now->pOp;
    vert = (DOOR_VTX_REC*)Door_work[op->index].vert;  // reinterprets the memory as DOOR_VTX_REC
    vert = BYTE_OFS(DOOR_VTX_REC, vert, ((UCHAR*)op)[2] * 8);
    vert->x = vert->x + op->data1;
    vert->y = vert->y + op->data2;
    vert->z = vert->z + op->data3;
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 0xA;
    return 1;
}

INT Door_op_mess_set(VOID)
{
    DOOR_OP_REC* op;

    op = Door_cmd_now->pOp;
    Mess_set(op->index, op->data0);
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 4;
    return 1;
}

INT Door_op_se_set(VOID)
{
    DOOR_OPB_REC* op;

    if (G.Status_flg & STAT_DOOR_WAIT)
    {
        Door_status &= ~2;
        return 0;
    }

    op = Door_cmd_now->pOp;
    Snd_se_play(op->index, op->data0, op->data1);

    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 4;

    return 1;
}

INT Door_op_flg_set(VOID)
{
    DOOR_OP_REC* op;

    op = Door_cmd_now->pOp;
    if (Door_work[op->index].flags != 0)
    {
        Door_work[op->index].flags = op->data0;
    }
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 4;
    return 1;
}

INT Door_op_tpage_set(VOID)
{
    USHORT page;
    POLY_FT4* q;
    UCHAR* cursor;
    LONG Packet_cnt;

    page = *(USHORT*)((UCHAR*)Door_cmd_now->pOp + 2);  // halfword view of the same memory
    q = (POLY_FT4*)Door_work[page & 0xFF].dobj.packet;  // reinterprets the memory as POLY_FT4
    Packet_cnt = (((TMD_MEM*)Door_work[page & 0xFF].dobj.tmd)->n_poly) * 2;  // reinterprets the memory as TMD_MEM
    cursor = (UCHAR*)q + 8;  // byte view of the same memory
    page = (q->tpage & 0xFFC0) | (page >> 8);
    while (Packet_cnt-- != 0)
    {
        ((POLY_FT4*)(cursor - 8))->tpage = page;  // reinterprets the memory as POLY_FT4
        cursor += sizeof(POLY_FT4);
    }
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 4;
    return 1;
}

INT Door_op_pkt_add(VOID)
{
    Gs_packet_p[0] = (UCHAR*)Gs_packet_p[0] + (((DOOR_OP_REC*)Door_cmd_now->pOp)->data0);  // reinterprets the memory as DOOR_OP_REC
    Gs_packet_p[1] = (UCHAR*)Gs_packet_p[1] + (((DOOR_OP_REC*)Door_cmd_now->pOp)->data0 >> 1);  // reinterprets the memory as DOOR_OP_REC

    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 4;

    return 0;
}

INT Door_op_disp_off(VOID)
{
    Door_status &= ~2;

    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 2;

    return 0;
}

INT Door_op_flg_clr(VOID)
{
    STATUS_FLG_CUR &= ~STAT_DOOR_WAIT;
    Door_cmd_now->pOp = (UCHAR*)Door_cmd_now->pOp + 2;

    return 1;
}

VOID Door_init(VOID)
{
    SHORT* p;
    INT* q;
    INT t;

    if (Door_anim_type & 0x20)
    {
        G.Vloop = 2;
    }
    else
    {
        G.Vloop = 0;
    }
    Door_anim_type &= 0x1F;

    GsClearOt(0, 0, MAIN.Ot0);
    GsClearOt(0, 0, &MAIN.Ot0[1]);
    GsClearOt(0, 0, MAIN.Ot1);
    GsClearOt(0, 0, &MAIN.Ot1[1]);
    GsClearOt(0, 0, MAIN.Ot2);
    GsClearOt(0, 0, &MAIN.Ot2[1]);
    Gs_packet_p[0] = DOOR_PKT_A_ADDR;
    Gs_packet_p[1] = DOOR_PKT_B_ADDR;

    Door_status = 3;

    Door_byte_var5 = 0;
    SYSTEM_FLG_CUR |= SYS_DOOR_PROCESS;

    STATUS_FLG_CUR |= STAT_DOOR_WAIT;

    pDoor_pkt_adr = (ULONG*)DOOR_PKT_TOP_ADDR;  // word view of the same memory
    for (p = (SHORT*)Door_work; p <= (SHORT*)Door_work + 0x2FE; p++)  // halfword view of the same memory
    {
        *p = 0;
    }
    for (p = (SHORT*)&Door_camera; p <= (SHORT*)&Door_camera + 0x15; p++)  // halfword view of the same memory
    {
        *p = 0;
    }

    for (p = (SHORT*)Door_cmd_slot; p <= (SHORT*)Door_cmd_slot + 0xDF; p++)  // halfword view of the same memory
    {
        *p = 0;
    }
    (*(INT*)LOAD_BUF) += (INT)LOAD_BUF;  // word view of the same memory
    t = (*(INT*)DOOR_TMD_OFS_ADDR) + (INT)LOAD_BUF;  // word view of the same memory
    (*(INT*)DOOR_TMD_OFS_ADDR) = t;  // word view of the same memory
    (*(INT*)LOAD_TIM_OFS_ADDR) += (INT)LOAD_BUF;  // word view of the same memory
    Door_obj_tbl = (UCHAR*)(t + 4);  // byte view of the same memory
    GsMapModelingData((ULONG*)Door_obj_tbl);  // word view of the same memory
    Door_obj_tbl += 8;
    for (q = *(INT**)LOAD_BUF; *q != 0; q++)  // word view of the same memory
    {
        *q += *(INT*)LOAD_BUF;  // INT* view of the same memory
    }
    Door_cmd_slot[0].active = 1;
    Door_cmd_slot[0].Loop_num = 0;
    Door_cmd_slot[0].pOp = (VOID*)*(*(INT**)LOAD_BUF);  // untyped pointer, the callee takes the address as is
    Door_cmd_slot[0].Loop_cnt[0] = 0;
    Task_sleep(1);
}

VOID Door_exit(VOID)
{
    G.Vloop = 2;
    Gs_packet_p[0] = &MAIN.Pkt_buf;  // address passed as an integer
    Gs_packet_p[1] = &MAIN.Pkt_buf[1];  // address passed as an integer

    G.SaveData.Fade_kido = -1;

    STATUS_FLG_CUR &= ~STAT_DOOR_WAIT;

    MAIN.System_flg ^= SYS_DOOR_PROCESS;
}

VOID Door_move(VOID)
{
    MATRIX m;
    DOOR_WORK* pDwork;

    INT t;

    while ((t = Door_status) & 1)
    {
        if (t & 2)
        {
            for (Door_cmd_now = Door_cmd_slot; Door_cmd_now < &Door_cmd_slot[8]; Door_cmd_now++)
            {
                if (Door_cmd_now->active != 0)
                {
                    while (Door_op_tbl[*(UCHAR*)Door_cmd_now->pOp]() != 0)  // byte view of the same memory
                    {
                    }
                }
            }
        }
        else if (!(G.Status_flg & STAT_DOOR_WAIT))
        {
            Door_status = t | 2;
        }
        GsSetRefView2((GsRVIEW2*)&Door_camera);  // the camera block is read by GsSetRefView2 as a GsRVIEW2
        for (pDwork = Door_work; pDwork < &Door_work[12]; pDwork++)
        {
            if (pDwork->flags & 0x4000)
            {
                RotMatrix(&pDwork->rot, &pDwork->coord.coord);
                pDwork->coord.flg = 0;
            }
            if (pDwork->flags & 0x8000)
            {
                GsGetLs(pDwork->dobj.coord2, &m);
                if (pDwork->flags & 0x2000)
                {
                    GsSetLightMatrix(&m);
                }
                GsSetLsMatrix(&m);
                pDwork->dobj.attribute &= ~0xE00;
                pDwork->dobj.attribute |= (pDwork->flags & 0xF0) << 5;
                switch (pDwork->flags & 0xF)
                {
                case 1:
                    GsSortObject5(&pDwork->dobj, MAIN.Ot0 + G.Cbuf_id, 10, ScratchUlong);
                    break;
                case 2:
                    GsSortObject5(&pDwork->dobj, MAIN.Ot1 + G.Cbuf_id, 4, ScratchUlong);
                    break;
                case 3:
                    GsSortObject5(&pDwork->dobj, MAIN.Ot2 + G.Cbuf_id, 10, ScratchUlong);
                    break;
                }
            }
        }
        Task_sleep(1);
    }
}

VOID Door_main(VOID)
{
    Door_init();
    Door_move();
    Door_exit();
    Task_exit();
}

VOID Door_quake_set(VOID)
{
    Door_shake_x = rand() % 2;

    ((SCREEN_SHAKE_WORK*)&Door_shake_x)->amplitude = rand() % 2;  // reinterprets the memory as SCREEN_SHAKE_WORK

    switch (rand() % 4)
    {
    case 0:
        break;
    case 1:

        ((SCREEN_SHAKE_WORK*)&Door_shake_x)->amplitude *= -1;  // reinterprets the memory as SCREEN_SHAKE_WORK

        break;
    case 2:
        Door_shake_x *= -1;
        break;
    case 3:
        Door_shake_x *= -1;

        ((SCREEN_SHAKE_WORK*)&Door_shake_x)->amplitude *= -1;  // reinterprets the memory as SCREEN_SHAKE_WORK

        break;
    }

    GsSetOffset(Door_shake_x + 0xA0, ((SCREEN_SHAKE_WORK*)&Door_shake_x)->amplitude + 0x78);  // reinterprets the memory as SCREEN_SHAKE_WORK
}

VOID Door_border_set(VOID)
{
    RECT r;

    SHORT y0;

    UINT* src;
    INT y;

    r.x = 0;
    r.w = 0x140;
    r.h = 0xF0;
    r.y = -(CBUF_ID_CUR == 0) & 0xF0;
    ClearImage(&r, 0, 0, 0);
    src = (UINT*)LOAD_BUF;  // word view of the same memory
    if (G.Bg_img_mode == 1)
    {
        src = (UINT*)LOAD_PIX_BUF;  // word view of the same memory
    }

    y0 = 2;

    if ((G.Status_flg & STAT_QUAKE) && !(G.System_flg & SYS_STATUS_MASK))
    {
        r.x = ((SCREEN_SHAKE_WORK*)&Door_shake_x)->timer + 2;  // reinterprets the memory as SCREEN_SHAKE_WORK
        y = ((SCREEN_SHAKE_WORK*)&Door_shake_x)->amplitude;  // reinterprets the memory as SCREEN_SHAKE_WORK

        y0 = y + 2;

        if (MAIN.Cbuf_id == 0)
        {
            y0 = y + 0xF2;
        }
    }
    else
    {
        r.x = 2;

        if (G.Cbuf_id == 0)
        {
            y0 = 0xF2;
        }
    }

    if (((SCREEN_SHAKE_WORK*)&Door_shake_x)->amplitude)  // reinterprets the memory as SCREEN_SHAKE_WORK
    {
        r.y = y0;
    }
    else
    {
        r.y = y0;
    }

    r.w = 0x13C;
    r.h = 0xEC;
    LoadImage(&r, (ULONG*)src);  // word view of the same memory
}

// addresses of the byte script variables: door animation id, sound bank, door texture index,
// door animation type, next stage and room, scratch value, menu choice
VOID* Door_byte_tbl[7] =
{
    &Door_anim_id[0],                         // 0 //
    &Door_snd_bank,                        // 1 //
    &Door_anim_id[1],                 // 2 //
    &Door_anim_type,                     // 3 //
    &Door_next_stage_room,           // 4 //
    &Door_byte_var5,                    // 5 //
    &G.SaveData.Mess_flg,   // 6 //
};

VOID* Door_short_tbl[1] = { &Door_anim_id[0] }; // addresses of the 16-bit script variables: door animation id and texture index

// script op handlers, by Opcode
DOOR_OP_FUNC Door_op_tbl[38] =
{
    Door_op_end,                 // 0 //
    Door_op_clear,               // 1 //
    Door_op_wait_free,           // 2 //
    Door_op_yield2,              // 3 //
    Door_op_jmp_byte,            // 4 //
    Door_op_jmp_short,           // 5 //
    Door_op_loop_push,           // 6 //
    Door_op_loop,                // 7 //
    Door_op_activate,            // 8 //
    Door_op_clear_entry,         // 9 //
    Door_op_byte_set,         // 10 //
    Door_op_byte_add,         // 11 //
    Door_op_short_set,        // 12 //
    Door_op_short_add,        // 13 //
    Door_op_fade_in,             // 14 //
    Door_op_fade_out,            // 15 //
    Door_model_set,         // 16 //
    Door_op_cam_set,          // 17 //
    Door_op_sabun_set,          // 18 //
    Door_op_cam_add,    // 19 //
    Door_op_sabun_add,           // 20 //
    Door_op_pos_set,           // 21 //
    Door_op_vel_set,           // 22 //
    Door_op_pos_add,   // 23 //
    Door_op_vel_add,       // 24 //
    Door_op_rot_set,           // 25 //
    Door_op_rvel_set,        // 26 //
    Door_op_rot_add,   // 27 //
    Door_op_rvel_add,    // 28 //
    Door_op_vtx_set,      // 29 //
    Door_op_vtx_add,      // 30 //
    Door_op_mess_set,             // 31 //
    Door_op_se_set,                 // 32 //
    Door_op_flg_set,         // 33 //
    Door_op_tpage_set,         // 34 //
    Door_op_pkt_add,      // 35 //
    Door_op_disp_off,    // 36 //
    Door_op_flg_clr,        // 37 //
};
