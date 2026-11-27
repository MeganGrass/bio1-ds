/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\DOOR.H
 */

#ifndef DOOR_H
#define DOOR_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>

typedef UCHAR DOOR_PKT_BUF[0x7DE4]; // polygon packet buffer of the door objects

union DOOR_MENU_WORK
{
    UCHAR raw[0x15C4C];                            // whole workspace
    struct
    {
        UCHAR Door_file[0xC00C];    // door file image
        UCHAR Door_pkt[0x9C40];     // polygon packets of the door objects
    } door;                                        // layout while a door is shown
    struct
    {
        UCHAR File_bg[0x10000];                     // door file image or room background
        UCHAR menu_texture_and_model_work[0x5C4C];  // menu textures and model work
    } menu;                                        // layout while the menu is shown
};

struct DOOR_WORK
{
    USHORT flags;        // door object flags
    SHORT id;            // door object id
    GsDOBJ5 dobj;        // model object of the door
    GsCOORDINATE2 coord; // coordinate system of the door
    SVECTOR rot;         // rotation of the door
    SHORT vel[3];        // velocity of the door
    SHORT rotvel[3];     // rotation velocity of the door
    UCHAR* vert;         // vertex list of the door model
};

struct DOOR_VIEW_REC
{
    INT v[8];   // view values: view point, reference point, rz and super
    SHORT d[6]; // per-frame change of the view point and reference point
};

struct SCREEN_SHAKE_WORK
{
    SCHAR timer;     // screen shake offset x
    SCHAR amplitude; // screen shake offset y
};

struct DOOR_OP_REC
{
    UCHAR Opcode; // door script Opcode
    UCHAR index;  // operand byte: door slot or jump distance
    USHORT data0; // operand word 0
    USHORT data1; // operand word 1
    SHORT data2; // operand word 2
    USHORT data3; // operand word 3
};

struct DOOR_OPB_REC
{
    UCHAR Opcode; // door script Opcode
    UCHAR index;  // operand byte: door slot or jump distance
    UCHAR data0;  // operand byte 0
    UCHAR data1;  // operand byte 1
    SCHAR data2;  // operand byte 2
};

struct DOOR_VTX_REC
{
    USHORT x; // vertex x
    USHORT y; // vertex y
    USHORT z; // vertex z
};

struct DOOR_CMD_WORK
{
    USHORT active;      // 1 while the script slot is running
    USHORT Loop_num;    // number of open loops
    VOID* pOp;          // script read pointer
    INT Loop_adr[8];    // script address to return to, per open loop
    USHORT Loop_cnt[8]; // iterations left, per open loop
};

typedef INT (*DOOR_OP_FUNC)(VOID);

extern SCHAR Door_shake_x;            // screen shake x offset (aliases the first byte of Door_shake)

/*
    script op: stop the door script (clear the running state); yields
 */
INT Door_op_end(VOID);

/*
    script op: clear the command slot being run (inactive, no loops, no script); yields
 */
INT Door_op_clear(VOID);

/*
    script op: wait until the command slot named by the op is inactive, then continue; yields while it is active
 */
INT Door_op_wait_free(VOID);

/*
    script op: skip the 2-byte op and yield for one frame
 */
INT Door_op_yield2(VOID);

/*
    script op: compare a signed byte variable with a value using the comparator (0 ==, 1 >, 2 >=, 3 <, 4 <=, 5 !=);
    when the comparison is false jump ahead by the op's offset, otherwise fall through to the next op
 */
INT Door_op_jmp_byte(VOID);

/*
    script op: as Door_op_jmp_byte with a 16-bit script variable and value
 */
INT Door_op_jmp_short(VOID);

/*
    script op: push a loop (start of the loop body and repeat count) on the command slot's loop stack
 */
INT Door_op_loop_push(VOID);

/*
    script op: count the innermost loop down; at zero pop it and continue, otherwise jump back to the start of
    the loop body and yield
 */
INT Door_op_loop(VOID);

/*
    script op: start the command slot named by the low byte of the op word on the script named by its high byte
 */
INT Door_op_activate(VOID);

/*
    script op: clear the command slot named by the op (inactive, no loops, no script)
 */
INT Door_op_clear_entry(VOID);

/*
    script op: store a value in a byte script variable
 */
INT Door_op_byte_set(VOID);

/*
    script op: add a value to a byte script variable
 */
INT Door_op_byte_add(VOID);

/*
    script op: store a value in a 16-bit script variable
 */
INT Door_op_short_set(VOID);

/*
    script op: add a value to a 16-bit script variable
 */
INT Door_op_short_add(VOID);

/*
    script op: start a fade in with the given fade type, step and fading state
 */
INT Door_op_fade_in(VOID);

/*
    script op: start a fade out with the given fade type, step and fading state
 */
INT Door_op_fade_out(VOID);

/*
    script op: set up door object slot: link its parent coordinate system, and when the op requests drawing
    link the model from the door file's object table and preset its packets; store its flags, model id
    and the model's vertex table
 */
INT Door_model_set(VOID);

/*
    script op: set the projection and load the camera viewpoint and reference point (shifted by the op's
    scale) into the camera record
 */
INT Door_op_cam_set(VOID);

/*
    script op: load the 6 camera deltas from the script
 */
INT Door_op_sabun_set(VOID);

/*
    script op: add the 6 camera deltas to the camera viewpoint and reference point
 */
INT Door_op_cam_add(VOID);

/*
    script op: add a signed byte to one of the camera deltas
 */
INT Door_op_sabun_add(VOID);

/*
    script op: set the position of a door object
 */
INT Door_op_pos_set(VOID);

/*
    script op: set the velocity of a door object
 */
INT Door_op_vel_set(VOID);

/*
    script op: add the velocity of a door object to its position
 */
INT Door_op_pos_add(VOID);

/*
    script op: add a signed byte to one component of the velocity of a door object
 */
INT Door_op_vel_add(VOID);

/*
    script op: set the rotation of a door object
 */
INT Door_op_rot_set(VOID);

/*
    script op: set the rotation velocity of a door object from three signed bytes
 */
INT Door_op_rvel_set(VOID);

/*
    script op: add the rotation velocity of a door object to its rotation
 */
INT Door_op_rot_add(VOID);

/*
    script op: add a signed byte to one component of the rotation velocity of a door object
 */
INT Door_op_rvel_add(VOID);

/*
    script op: set one vertex of a door object's model
 */
INT Door_op_vtx_set(VOID);

/*
    script op: add an offset to one vertex of a door object's model
 */
INT Door_op_vtx_add(VOID);

/*
    script op: show a message
 */
INT Door_op_mess_set(VOID);

/*
    script op: play a sound effect; while the pause flag is set stop dispatching scripts and yield
 */
INT Door_op_se_set(VOID);

/*
    script op: set the flags of a door object when it is in use
 */
INT Door_op_flg_set(VOID);

/*
    script op: set the texture page of every polygon of a door object's model
 */
INT Door_op_tpage_set(VOID);

/*
    script op: advance both polygon packet area pointers; yields
 */
INT Door_op_pkt_add(VOID);

/*
    script op: pause script dispatching until the pause flag in the status flags is clear; yields
 */
INT Door_op_disp_off(VOID);

/*
    script op: clear the pause flag in the status flags
 */
INT Door_op_flg_clr(VOID);

/*
    set up the door scene: choose the vsync mode, clear the ordering tables, redirect the polygon packet
    areas, reset the door objects, camera and command slots, relocate the door file and start its first script
 */
VOID Door_init(VOID);

/*
    leave the door scene: restore the vsync mode, packet areas and the fading and status flags
 */
VOID Door_exit(VOID);

/*
    per-frame door loop: run every active command slot's script until it yields, set the camera,
    update each door object's matrix and sort the drawn ones into the ordering tables; runs until
    the script stops it
 */
VOID Door_move(VOID);

/*
    door task: set up, run the door loop, leave and end the task
 */
VOID Door_main(VOID);

/*
    pick a random screen shake offset of 0 or 1 in each axis with a random sign and apply it to the draw offset
 */
VOID Door_quake_set(VOID);

/*
    clear the display buffer area selected by the active buffer and upload the background image into it
    inset by two pixels, displaced by the screen shake offset while shaking
 */
VOID Door_border_set(VOID);

extern DOOR_WORK Door_work[12];       // door objects: coordinate system, model, velocities and rotation
extern UCHAR Door_snd_bank;                   // sound bank of the door, script byte variable 1
extern DOOR_VIEW_REC Door_camera;             // door camera: view point and reference point, and their deltas
extern DOOR_CMD_WORK* Door_cmd_now;           // command slot being run
extern SCREEN_SHAKE_WORK Door_shake; // screen shake offsets: x in the first byte, y in the second

extern INT Door_status;                        // door script state: bit 0 running, bit 1 dispatching scripts
extern UCHAR* Door_obj_tbl;           // model table of the door file
extern UCHAR Door_anim_type;                // door animation type, script byte variable 3
extern ULONG* pDoor_pkt_adr;          // next free polygon packet of the door objects
extern UCHAR Door_byte_var5;               // script byte variable 5, a scratch value

extern VOID* Gs_packet_p[2];                 // polygon packet area of each of the two draw buffers

extern DOOR_CMD_WORK Door_cmd_slot[8];          // script command slots
extern VOID* Door_byte_tbl[7];                  // addresses of the byte script variables
extern VOID* Door_short_tbl[1];                 // addresses of the 16-bit script variables
extern DOOR_OP_FUNC Door_op_tbl[38];       // script op handlers, by Opcode

#endif      // DOOR_H
