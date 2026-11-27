/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EVENT.H
 */

#ifndef EVENT_H
#define EVENT_H

#include "TYPES.H"

struct EVT_CMD_REC
{
    UCHAR Opcode; // script event command Opcode
    UCHAR index;  // operand byte: jump distance or target index
    SHORT data0;  // operand word 0
    SHORT data1;  // operand word 1
    SHORT data2;  // operand word 2
};

struct EVT_EXEC_REC
{
    UCHAR Dummy0[1];    // Opcode
    UCHAR slot;         // event slot to run in
    UCHAR script;       // script id to run
};

/*
    activate a script event task: set its script pointer from the room's event script table,
    take the current entity as its work entity and reset its state and stack
 */
VOID Event_init(SCE_TASK* pSce, INT Evt_no);

/*
    start an event script in a task slot; a slot number of 8 or more picks the first free slot,
    or the last slot when all are busy
 */
INT Event_exec(UINT Task_level, INT Evt_no);

/*
    run every active script event task once while script updates are enabled: voice wait, sleep,
    for and while control opcodes are handled in place, other opcodes go to the handler of the
    task state (command, animation, movement or behavior)
 */
VOID Sce_scheduler_main(VOID);

/*
    animation state of an event task: execute one animation command on the work entity (look-at
    on or off, walk to, play animation, action, reset entity state, flag operation, timer, frame
    chain, set frame, stop)
 */
INT Evt_anm_set(INT D0, SCE_TASK* pTask);

/*
    behavior state of an event task: set the action behavior of the work entity from the next
    script byte, resetting its action state when the behavior changes, and force the held pad
    bits to 0xFF
 */
INT Evt_move_set(INT D0, SCE_TASK* pTask);

/*
    movement state of an event task: execute one movement command on the work entity (advance
    position and rotation by their speeds, set the speeds, position, rotation, a byte of the entity
    at a script-given index, the hit state or one member, or return to the command state)
 */
VOID Evt_spd_set(INT D0, SCE_TASK* pTask);

/*
    select the work entity of the task: the player, an enemy, an object model or an item model
 */
VOID Work_set(INT D0, SCE_TASK* pTask);

/*
    start the event script given by the command in another task slot
 */
VOID Evt_exec_cmd(INT D0, SCE_TASK* pTask);

/*
    restart the current task with another event script
 */
VOID Evt_chain(INT D0, SCE_TASK* pTask);

/*
    run the embedded script command sequence through the script interpreter, then skip past it
 */
VOID Evt_sce_run(INT D0, SCE_TASK* pTask);

/*
    run a single script Opcode handler: point the script instruction pointer after the command
    header, advance the task by the length byte and call the handler of the Opcode
 */
VOID Evt_op_exec();

/*
    start the pending fade when no fade is running and a fade step is set: the fade state starts
    at its maximum for a negative step and at 0 for a positive step
 */
VOID Evt_fade_start(VOID);

/*
    request a fade of the given type and step when no fade is running
 */
VOID Evt_fade_req_set(SCHAR fade_type, SHORT step);

extern UCHAR Em_se_dat00[0x90];       // packed record block 00 referenced by em_se_tbl
extern UCHAR Em_se_dat01[0x24];       // packed record block 01 referenced by em_se_tbl
extern UCHAR Em_se_dat02[0xC8];       // packed record block 02 referenced by em_se_tbl
extern UCHAR Em_se_dat03[0x38];       // packed record block 03 referenced by em_se_tbl
extern UCHAR Em_se_dat04[0x14];       // packed record block 04 referenced by em_se_tbl
extern UCHAR Em_se_dat05[0x14];       // packed record block 05 referenced by em_se_tbl
extern UCHAR Em_se_dat06[0x14];       // packed record block 06 referenced by em_se_tbl
extern UCHAR Em_se_dat07[0x14];       // packed record block 07 referenced by em_se_tbl
extern UCHAR Em_se_dat08[0x24];       // packed record block 08 referenced by em_se_tbl
extern UCHAR Em_se_dat09[0x6C];       // packed record block 09 referenced by em_se_tbl
extern UCHAR Em_se_dat10[0x14];       // packed record block 10 referenced by em_se_tbl
extern UCHAR Em_se_dat11[0x14];       // packed record block 11 referenced by em_se_tbl
extern UCHAR Em_se_dat12[0x14];       // packed record block 12 referenced by em_se_tbl
extern UCHAR Em_se_dat13[0x14];       // packed record block 13 referenced by em_se_tbl
extern UCHAR Em_se_dat14[0x6C];       // packed record block 14 referenced by em_se_tbl
extern UCHAR Em_se_dat15[0x14];       // packed record block 15 referenced by em_se_tbl
extern UINT em_se_tbl[113];                     // 13 leading data words, then 50 { word, record block pointer } pairs

#endif      // EVENT_H
