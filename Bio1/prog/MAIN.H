/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\MAIN.H
 */

#ifndef MAIN_H
#define MAIN_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
extern ULONG GsTRATE; // GPU library: transparency rate
extern ULONG GsZOVER; // GPU library: depth overflow setting used when sorting objects
extern ULONG GsDISPON; // GPU library: display-on state
extern USHORT GsORGOFSY; // GPU library: draw origin y offset
extern MATRIX GsWSMATRIX_ORG; // GPU library: saved copy of the world-screen matrix
extern UCHAR Pl_turn_run_pad[2]; // padding: keeps the neighbouring data at its linked position
extern UCHAR Door_snd_bank_pad[4]; // padding: keeps the neighbouring data at its linked position
extern UCHAR GsLMODE_pad[16]; // padding: keeps the neighbouring data at its linked position
extern UCHAR GsLIGNR_pad[8]; // padding: keeps the neighbouring data at its linked position
extern UCHAR GsZOVER_pad[4]; // padding: keeps the neighbouring data at its linked position
extern UCHAR Door_cmd_now_pad[4]; // padding: keeps the neighbouring data at its linked position
extern UCHAR Item_light_pad[8]; // padding: keeps the neighbouring data at its linked position
extern MATRIX GsLIGHTWSMATRIX; // GPU library: light-to-screen matrix
extern UCHAR GsORGOFSX_pad[4]; // padding: keeps the neighbouring data at its linked position
extern UCHAR GsORGOFSY_pad[4]; // padding: keeps the neighbouring data at its linked position
extern UCHAR Svm_vh_pad[4]; // padding: keeps the neighbouring data at its linked position
extern UCHAR Sca_rtn_tbl_pad[176]; // padding: keeps the neighbouring data at its linked position
extern UCHAR Sca_hit_top_pad[4]; // padding: keeps the neighbouring data at its linked position
extern UCHAR Door_status_pad[2]; // padding: keeps the neighbouring data at its linked position
extern UCHAR Door_obj_tbl_pad[4]; // padding: keeps the neighbouring data at its linked position
extern MATRIX GsIDMATRIX; // GPU library: identity matrix
extern UCHAR StFinalSector_pad[32]; // padding: keeps the neighbouring data at its linked position
extern UCHAR Sce3_logo_state_pad[4]; // padding: keeps the neighbouring data at its linked position
extern MATRIX GsWSMATRIX; // GPU library: world-screen matrix
// the data declarations above stay ahead of <LIBGS.H>: that keeps the symbol numbering of the object
#include <LIBGS.H>

typedef VOID (*MAIN_FUNC)(VOID);

/*
    per-frame game loop: set up the system and both draw environments, then each frame read
    input, handle the soft-reset hold, graphics reset request, BGM fade and the full-screen fade
    pass, run the scheduler and message display, draw the screen dim and room light overlays,
    swap buffers, clear the screen or upload the background image, apply screen shake, play a
    requested movie and draw the ordering tables
 */
VOID main(VOID);

/*
    initialise the libraries (callbacks, geometry, memory card, controllers, graphics, root
    counter), the ordering tables, packet areas, scheduler, default key bindings and screen dim
    polygons, then start the Init_main and InitGeom_func Task
 */
VOID Init_system(VOID);

/*
    initialise the graphics system (No_reset selects the variant that keeps the GPU state), set the
    display buffers and 3D, reset the GPU/CD, clear the graphics reset request and centre the
    screen origin
 */
VOID Init_graph(INT No_reset);

/*
    reset the GPU, the MDEC decoder and the CD-ROM system
 */
VOID Reset_graph(VOID);

/*
    startup task: start the texture loading task, show a full-screen picture read from disc with a
    fade in and out, wait for the loading task, set the pad main mode and chain to program 0x154
 */
VOID Init_main(VOID);

/*
    position the fade box over the screen: at (-0xA0, -0x78) when the screen origin offset x is
    0xA0 (origin at the screen centre), otherwise at (0, 0)
 */
VOID Fade_box_pos_set(VOID);

/*
    advance the attract-demo timer while the demo is running, restarting it near the demo length
 */
VOID Demo_timer_main(VOID);

/*
    soft-reset task: fade out and shut down sound, clear the fixed work area (keeping the status
    flag set by Pl_data_init), restore the sound and fade state and packet areas, chain to
    program 0x15E
 */
VOID Soft_reset(VOID);

/*
    task: load the two global texture files and map them into VRAM, then exit
 */
VOID Tex_load(VOID);

/*
    task: run InitGeom, then exit
 */
VOID InitGeom_func(VOID);

extern UCHAR Xa_start_sector_pad[1]; // padding: keeps the neighbouring data at its linked position
extern UCHAR Snd_seq_tick_mode_pad[4]; // padding: keeps the neighbouring data at its linked position
extern UCHAR Snd_seq_tick_mode_pad2[80]; // padding: keeps the neighbouring data at its linked position
extern UCHAR Door_next_stage_room_pad[2]; // padding: keeps the neighbouring data at its linked position
extern UCHAR PSDIDX_pad[2]; // padding: keeps the neighbouring data at its linked position
extern UCHAR Svm_vab_total_pad[4]; // padding: keeps the neighbouring data at its linked position
extern UCHAR Spu_key_stat_pad[16]; // padding: keeps the neighbouring data at its linked position
extern UCHAR Xa_start_loc_pad[4]; // padding: keeps the neighbouring data at its linked position
extern UCHAR Pad_act_align[0x8]; // actuator alignment table for PadSetActAlign: 0x00 and 0x01 assign the two motors, 0xFF leaves a slot unused
extern GsBOXF Fade_box; // full-screen box used for fades and room light overlays
extern POLY_F4 Curtain[2][2]; // screen dim polygons: a top and a bottom bar for each display buffer

#endif
