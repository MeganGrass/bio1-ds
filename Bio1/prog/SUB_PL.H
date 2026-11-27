/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\SUB_PL.H
 */

#ifndef SUB_PL_H
#define SUB_PL_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

struct SUBPLAY_WORK
{
    UCHAR Be_flg;                   // slot state bits: 0 active, 1 hit checks off, 2 wall collision off, 3 prop collision off, 4 last wall slide axis, 5/7 player in range/alert
    UCHAR Id;                       // entity type id, selects the model and behaviour routines
    UCHAR Type;                     // behaviour parameter from the spawn record
    UCHAR Zone_in_flg;              // non-zero while the position is inside the current camera cut quad, gates drawing
    AT_TBL* pAt_tbl;                // collision box record: offsets, half height and radius
    AT_WORK* pSca_ofs;              // collision centre offset used by entity-to-entity hit checks
    UINT Attribute;                 // model object attribute word
    GsCOORDINATE2* pCoord2;         // coordinate node of the model object, points into this record
    INT pTmd;                       // address of the loaded model data
    VOID* pPacket;                    // render packet buffer of the model object
    INT Coord_flg;                  // flag word at the head of the coordinate block, cleared each frame
    UCHAR M[0x34 - 0x20];           // local rotation matrix
    INT Pos_x;                      // world position x
    INT Pos_y;                      // world position y
    INT Pos_z;                      // world position z
    INT Workm;                      // first word of the coordinate block world matrix workspace
    UCHAR Dummy044[0x6C - 0x44];    // rest of the coordinate block, not accessed directly
    SHORT Old_x;                   // x position at the end of the previous frame
    SHORT Old_y;                   // y position at the end of the previous frame
    SHORT Old_z;                    // z position at the end of the previous frame
    USHORT Cdir_x;                  // pitch, 12-bit angle
    SHORT Cdir_y;                   // DirY, 12-bit angle
    SHORT Cdir_z;                   // roll, 12-bit angle
    SHORT Spd_x;                    // movement step x per frame
    SHORT Spd_y;                    // movement step y per frame
    SHORT Spd_z;                    // movement step z per frame
    SHORT Old_dir;                 // DirY at the end of the previous frame
    USHORT Aspd_x;                  // pitch speed set by script commands
    USHORT Aspd_y;                  // DirY speed set by script commands
    UCHAR Routine_0;                // main state, index into the state routine table
    UCHAR Routine_1;                // step within the state
    UCHAR Routine_2;                // scripted action id, index into the action routine table
    UCHAR Routine_3;                // step within the scripted action
    SHORT Life;                     // hit points, negative when dead
    UCHAR Damage_no;                // Damage reaction set when hit, 0 = none
    UCHAR Dummy08b;                 // padding, never accessed
    UCHAR Hokan_flg;                // pose blend steps left after an animation change
    UCHAR Parts_num;                // number of pSin_parts_ptr in the model
    SHORT Ground;                   // ground height used for the shadow and landing
    VOID* pKan_t_ptr;               // joint animation data of the model
    VOID* pSeq_t_ptr;               // animation sequence table of the model
    PARTS_W* pSin_parts_ptr;        // joint work array
    VOID* pParts_tmd;                 // model data of the weapon part
    VOID* pParts_packet;              // render packet of the weapon part
    UCHAR Dummy0a4[0xAC - 0xA4];    // padding
    PARTS_W* pM_char_ptr;         // joint work array used for the mirror reflection
    VOID* pM_option_tmd;                // weapon model copy used for the mirror reflection
    VOID* pM_option_packet;                // weapon render packet used for the mirror reflection
    ENEMY_WORK* pTgt;               // enemy being aimed at
    UCHAR Timer2;                   // frame counter of a fall or death sequence
    UCHAR Move_no;                  // animation sequence index
    UCHAR Move_cnt;                 // frame index within the animation sequence
    UCHAR Move_wait;                // frames left before the next animation frame is read
    UCHAR Dummy0c0;                 // padding, cleared on spawn
    SCHAR Dummy0c1;                 // unused, cleared at player setup
    SHORT Spd_cur;                  // current forward speed per frame
    SHORT Timer0;                   // frames left in the current action step
    USHORT Root_x;                  // x of the animation root anchor: position minus the root offset
    USHORT Root_z;                  // z of the animation root anchor: position minus the root offset
    USHORT Scale;                   // model scale factor, 0 = unscaled
    INT Neck_point_x;               // script-set head-turn target x, or DirY angle
    INT Neck_point_y;               // script-set head-turn target y, or pitch angle
    INT Neck_point_z;               // script-set head-turn target z, or sweep counter
    UCHAR Neck_flg;                 // head-turn bits: 4 enabled, 0 DirY follows target, 5 target is an angle, 6 sweep, 7 track the aim target
    UCHAR Neck_spd_y;               // head DirY step per frame
    UCHAR Neck_spd_x;               // head pitch step per frame
    UCHAR Set_flg_no;               // flag number raised when the scripted action finishes
    UCHAR Sca_info;                 // condition bits: 3 wall hit this frame, 1/5/6 poison levels, 7 scripted action lock
    UCHAR Neck_no;                  // index of the joint turned by the head-turn
    USHORT Sce_free0;               // script-set timer or angle parameter of the scripted action
    USHORT Sce_flg;                 // script-set option bits: 0 play animation reversed, 6 hide shadow
    SHORT Turn_timer;               // frames left before the next turn is allowed
    KAGE_WORK Kage_work;            // ground shadow work
    VOID* pKage_work;                 // not referenced by the code
    UCHAR Dummy160;                 // padding
    UCHAR Slot_info;                // enemy slot in the low nibble, sound set in bits 4-6, bit 7 = not saved when leaving the room
    UCHAR Dummy162;                 // padding
    UCHAR Em_set_flg;               // event flag raised when killed, 0xFF = none
    UCHAR Sikai_cnt;                // line-of-sight check counter in the low 5 bits, bit 5 = out of sight
    UCHAR Dummy165;                 // padding
    SHORT Dest_x;                  // x of the last known target position to walk toward
    SHORT Dest_z;                  // z of the last known target position to walk toward
    UCHAR Dummy16a;                 // padding
    UCHAR Step_sw;                  // not referenced by the code
    SCHAR Seq_end;                  // 1 when the current animation has finished, set by the animation step
    UCHAR Mot_flg;                  // bit 0 plays the animation reversed
    UCHAR Idle_timer;               // frames left in the idle pause
    UCHAR Neck_timer;               // frames left before a new head-turn target is picked
    UCHAR Neck_rnd0;                // last head DirY pick, avoids repeats
    UCHAR Neck_rnd1;                // last head pitch pick, avoids repeats
    UCHAR Neck_rnd2;                // last pitch step pick, avoids repeats
    UCHAR Neck_rnd3;                // last DirY step pick, avoids repeats
    UCHAR zone;                     // navigation zone the entity stands in
    UCHAR Next_zone;                // next navigation zone on the route to the player, bit 4 = route reached the player's zone
    SHORT heading;                  // target heading toward the next zone, 12-bit angle
};

typedef struct SPL_DEF_TBL SPL_DEF_TBL;
typedef VOID (*SPL_FUNC)(VOID);

struct SPL_DEF_TBL
{
    struct
    {
        USHORT Near_dist; // switch to Near_next when closer to the player than this (0 = never)
        USHORT Far_dist;  // switch to Far_next at this distance or farther (0 = never)
    } Swap_dist[4];  // swap distances, by walk behaviour
    struct
    {
        UCHAR Near_next; // walk behaviour after a near swap
        UCHAR Far_next;  // walk behaviour after a far swap
    } Swap_next[4]; // swap targets, by walk behaviour
    AT_TBL sca_info[10]; // collision box of each NPC character, in character init order
};

/*
    state routine of an NPC that follows the player: enter the walking state on the first call, run the
    sight check on the player, switch the walk behaviour by distance to the player, find the entity's zone,
    run the walk behaviour, advance the animation, play footsteps and run the collision checks
 */
VOID Spl_Auto(VOID);

/*
    standing walk behaviour: idle animation for a random time, then animations 5 and 6 in turn,
    while the head looks around
 */
VOID Spl_walk00(VOID);

/*
    walking behaviour toward the player: choose the heading, play animation 7, advance when the entity
    faces within 384 angle units of the heading and turn in place otherwise, while the head looks around
 */
VOID Spl_walk01(VOID);

/*
    running behaviour toward the player: choose the heading, run (animation 8) when facing within 384
    angle units of the heading, otherwise turn and, within 512 angle units, walk (animation 7); the head
    looks at the waypoint
 */
VOID Spl_walk02(VOID);

/*
    retreating behaviour: back away (animation 3) while the player is within 512 angle units ahead and
    looking at the player, otherwise turn away from the player and walk (animation 7)
 */
VOID Spl_walk03(VOID);

/*
    set sub-state 1 on the current entity and reset its head look-at
 */
VOID Spl_auto_set(VOID);

/*
    switch the walk behaviour by distance to the player: below the near distance of the current behaviour
    take its near target, at or beyond the far distance take its far target
 */
VOID Spl_walk_chg(VOID);

/*
    choose the heading toward the player: ask the route finder for the next zone, aim straight at the
    player when the player's zone is reached, otherwise at the crossing point on the shared edge of the
    zones (or the zone edge heading when the crossing is blocked), and store the waypoint
 */
VOID Spl_dir_set(VOID);

/*
    test whether the crossing point (x or z, by mode) lies inside the open span of the edge shared by the
    entity's zone and the next zone, with the span trimmed at obstacles; returns 1 when inside
 */
UCHAR Spl_hani_ck(UCHAR mode, SHORT x, SHORT z);

/*
    turn the facing angle toward the heading by up to base_step (plus 8 for odd entity ids) per call,
    snapping to the heading when within one step
 */
VOID Spl_dir_turn(INT base_step);

/*
    move the current entity dist units along its facing direction in X and Z
 */
VOID Spl_add_xz(SHORT dist);

/*
    reset the head look-at of the current entity: random timer, mode 0x10, zero target and default turn steps
 */
VOID Spl_neck_init();

/*
    set look-at mode 0x33 and count down the look timer; when it expires pick a new random head
    direction, avoiding a repeat of the previous pick; an odd Side_flg limits the choice to one side
 */
VOID Spl_neck_rnd_set(INT Side_flg);

/*
    aim the head look-at of the current entity at the point (x, 0, z) with a random timer (mode 0x11)
 */
VOID Spl_neck_pos_set(SHORT x, SHORT z);

/*
    play a footstep sound at the step frames of the walk (animations 3 and 7) and run (animation 8)
    cycles, offset by the entity id parity
 */
VOID Spl_foot_se_set(VOID);

/*
    per-frame update of an NPC entity: when update flag 2 is set run its state routine and turn the neck,
    then test its position against the camera zones and queue its shadow unless hidden by flag 0x40
 */
VOID Spl(VOID);

/*
    state 0 routine: set state 1 with a clear sub-state, behaviour and action state, zero the rotation
    and scale, set health to -1, initialise the joint parts, run the character init routine of its id
    and compute its collision hit offsets
 */
VOID Spl_Init(VOID);

/*
    character init of Chris: set up the shadow quad with colour 0x808080, reset the blend, run the
    first animation step, use collision box 0 and load the behaviour model
 */
VOID Spl_in20(VOID);

/*
    character init of Jill: set up the shadow quad with colour 0x808080, reset the blend, run the
    first animation step, use collision box 1 and load the behaviour model
 */
VOID Spl_in21(VOID);

/*
    character init of Barry: set up the shadow quad with colour 0x808080, reset the blend, run the
    first animation step, use collision box 2 and load the behaviour model
 */
VOID Spl_in22(VOID);

/*
    character init of Rebecca: set up the shadow quad, and when her flag bit 0xC0 is set start animation
    0x33 at frame 0x3D with a tinted, enlarged shadow and colour-added pSin_parts_ptr; use collision box 3 and
    load the behaviour model
 */
VOID Spl_in23(VOID);

/*
    character init of Wesker: set up the shadow quad, and when his flag bit 0x37 is set start animation
    0x30 at frame 0x6D with an enlarged tinted shadow; set a rotation delta of 0x10 on joint 1, use
    collision box 4, set status flag 2 for stage 0x1104 and load the behaviour model
 */
VOID Spl_in24(VOID);

/*
    character init of the Kenneth corpse: set up the shadow quad, start animation 0, use collision box 5
 */
VOID Spl_in25(VOID);

/*
    character init of the Forest corpse: set up the offset shadow quad, start animation 0, use collision box 6
 */
VOID Spl_in26(VOID);

/*
    character init of Richard: set up the shadow quad with colour 0x606060, run the first animation
    step and use collision box 7
 */
VOID Spl_in27(VOID);

/*
    character init of Enrico: set up the offset shadow quad with colour 0x404040, run the first
    animation step and use collision box 8
 */
VOID Spl_in28(VOID);

/*
    character init of the NPC with id 41: set up the shadow quad with colour 0x606060, start animation 0
    and use collision box 9
 */
VOID Spl_in29(VOID);

/*
    state 1 routine: run the idle behaviour selected by Routine_2
 */
VOID Spl_Sp_Move(VOID);

/*
    run the idle behaviour table entry selected by the entity id minus 28
 */
VOID Spl_sp00(VOID);

/*
    empty function
 */
VOID Spl_sp_nop(VOID);

/*
    play animation 0 of the current entity, stepping it once per call
 */
VOID Spl_sp_mot(VOID);

/*
    idle behaviour: slide backward with decaying speed (animation 0x35) until the scene collision is hit,
    then raise the voice playing game flag 0x20000, issue a voice command and play animation 0x36
    with a sound
 */
VOID Spl_sp01(VOID);

/*
    idle behaviour: play animation 0x33 with effects, joint colour flashes and a sound, then move,
    tint and shrink the shadow and regrow it by 16 per frame for 30 frames
 */
VOID Spl_sp02(VOID);

/*
    idle behaviour: play animation 0x30 with effects and joint colour flashes, then tint and shrink the
    shadow; for 250 frames clear status flag 2, set health to -1 and regrow the shadow by 6 per frame
 */
VOID Spl_sp03(VOID);

extern SPL_DEF_TBL Spl_def_tbl; // walk behaviour swap distances and targets, and the collision boxes of the NPC characters
extern SPL_FUNC Spl_walk_tbl[4];     // walk behaviour routines, by Routine_2
extern SPL_FUNC Spl_routine_0[25];            // NPC state routines by state (0 init, 1 special move, 8 scenario, 9 auto walk); from entry 10 the character init routines, by id - 22
extern SPL_FUNC Spl_sp_tbl[19];  // idle behaviour routines, by Routine_2; Spl_sp00 selects an entry by id - 28

#endif
