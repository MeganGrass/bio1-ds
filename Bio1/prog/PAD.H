/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\PAD.H
 */

#ifndef PAD_H
#define PAD_H

#include "TYPES.H"

struct VIB_REQ
{
    UCHAR Flg;      // 1 while the request is active
    UCHAR Pow;      // current vibration strength
    USHORT Delay;   // frames to wait before the request starts
    USHORT Time;    // frames remaining
    SHORT Step;     // strength change per frame, scaled by 128
    USHORT Pow_cur; // current strength, scaled by 128
};

/*
    clear the held-button word and load the default button bindings into the key config
 */
VOID Pad_def_set(VOID);

/*
    clear the held-button word and load the default button bindings into the key config, with
    binding 7 set to 0x40
 */
VOID Pad_def_set2(VOID);

/*
    per-frame pad update: query the state and type of the controller on port 0 and set up its
    vibration, drive the motors from the vibration stacks, map the raw buttons through the selected
    button mapping table into the held, trigger and pressed words (using the recorded input while a
    demo plays), and turn analog stick tilt into direction bits and turn speeds
 */
VOID Pad_set(VOID);

/*
    queue a small-motor vibration of duration frames after a delay of delay frames, when the stack has room
 */
INT Pad_vib_set0(INT duration, INT delay);

/*
    queue a large-motor vibration of duration frames at power vol after a delay of delay frames,
    when the stack has room
 */
INT Pad_vib_set1(INT duration, INT vol, INT delay);

/*
    queue a large-motor vibration of duration frames whose power ramps from vol_start to vol_end,
    after a delay of delay frames, when the stack has room
 */
INT Pad_vib_fade_set(INT duration, INT vol_start, INT vol_end, INT delay);

/*
    clear both vibration stacks, point the current-slot pointers at their first slots and clear the
    motor levels
 */
VOID Pad_vib_init(VOID);

/*
    advance every request of a vibration stack by one frame (delay Time_cnt, duration, power Ramp)
    and return the highest power among the active requests
 */
UCHAR SetPadActData(VIB_REQ* pReq);

/*
    return the first free request slot of a vibration stack, or the slot past the end when it is full
 */
VIB_REQ* Pad_vib_get_free(VIB_REQ* pReq);

/*
    count the active requests of a vibration stack
 */
UINT Pad_vib_count(VIB_REQ* pReq);

/*
    return the active-high button bits of both ports as one word; 0 when the port 0 pad is not ready
 */
INT Pad_raw_get(INT Pad_no);

extern USHORT Pad_stand_turn_spd; // turn speed per frame while turning on the spot, set by Pad_set
extern UCHAR Act_mtr_lv[2];                   // vibration motor levels sent to the controller: small motor, large motor
extern SCHAR Ana_mode;                        // analog stick tilt level, 0 to 10, index into Pad_speed_scale_tbl
extern UCHAR Padd_data[2][36];                // raw pad buffer of each port: status, terminal id, button bytes and analog bytes
extern UCHAR Vib_ok_id;                       // vibration setup state of port 0: 0 = not set up, 1 = set up, 2 = set up with the alignment table
extern UCHAR Pad_device_now;                  // controller type of port 0 (4 = digital pad, 7 = analog pad, 0 = none)
extern VIB_REQ Vib_stack0[33];                // vibration requests of the small motor
extern VIB_REQ Vib_stack1[33];                // vibration requests of the large motor
extern VIB_REQ* Vib_cur0;                     // request slot last queued on the small motor stack
extern VIB_REQ* Vib_cur1;                     // request slot last queued on the large motor stack
extern UCHAR Pad_speed_scale_tbl[];          // turn speed percentage by analog tilt level (entries 0 to 10), then the three default button mapping tables
extern USHORT* Key_type[];                    // button mapping tables: three defaults inside Pad_speed_scale_tbl, then the custom bindings of the key config

#endif
