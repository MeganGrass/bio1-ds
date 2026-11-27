/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\EM_SUB.H
 */

#ifndef EM_SUB_H
#define EM_SUB_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

struct WEB_WORK
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
    UCHAR Dummy08b;                 // padding
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
    ENEMY_WORK* pTgt;               // first web of the group, walked by the web update loop
    UCHAR Timer2;                   // frames in flight while the web is thrown
    UCHAR Move_no;                  // animation sequence index
    UCHAR Move_cnt;                 // frame index within the animation sequence
    UCHAR Move_wait;                // frames left before the next animation frame is read
    UCHAR Dummy0c0;                 // padding
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
    USHORT Dir_add;                 // sideways turn added to the DirY each frame, reused as the initial fall speed when thrown
    UCHAR Dummy16e[2];              // padding
    USHORT Hit_done;                // 1 once this web has hit the player
    SHORT swing;                    // DirY sway added each frame, sign flips every frame
    USHORT Se_wait;                 // frames left before the touch sound can play again
};

typedef struct GT3_COL_REC GT3_COL_REC;

struct GT3_COL_REC
{
    UCHAR pad0[0xC];        // padding: precedes the first vertex colour word
    UINT c0;                // colour of the first vertex (rgb in the low 24 bits, mode byte above)
    UCHAR pad10[8];         // padding: precedes the second vertex colour word
    UINT c1;                // colour of the second vertex
    UCHAR pad1C[8];         // padding: precedes the third vertex colour word
    UINT c2;                // colour of the third vertex
};

typedef struct TMD_GT3_REC TMD_GT3_REC;

struct TMD_GT3_REC
{
    UCHAR pad0[0x12];       // padding: precedes the vertex indices
    USHORT a;               // vertex index of the first corner
    UCHAR pad14[2];         // padding: between the vertex indices
    USHORT b;               // vertex index of the second corner
    UCHAR pad18[2];         // padding: between the vertex indices
    USHORT c;               // vertex index of the third corner
};

typedef struct GT3_BUF GT3_BUF;

struct GT3_BUF
{
    POLY_GT3 p;             // triangle packet of the first display buffer
    POLY_GT3 other_buffer;  // triangle packet of the other display buffer
};

typedef VOID (*EM_SUB_FUNC)(VOID);

/*
    move the current entity forward by speed along its facing and apply a gravity-driven vertical
    drop (Fall_spd plus gravity per elapsed step); on reaching floor_y the entity is clamped
    to it and the last drop is returned, otherwise the fall timer advances and 0 is returned
 */
SHORT Em_fall_move(SHORT speed, INT Fall_spd, SHORT gravity, SHORT floor_y);

/*
    step a joint that has broken away from the current entity: rotate it, move it by its
    velocity and apply gravity; on passing the ground height restart it from a fixed height
    (or put it on the ground once flagged) and reset its fall counter; a flagged joint that is
    already on the ground is only marked as settled; always returns 0
 */
INT Parts_down(PARTS_W* pP_ptr, SHORT gravity, SHORT Ground, UCHAR ref_index);

/*
    turn the current entity while it is slowed down: after limit frames below two thirds of its
    speed, start a timed forced turn; while forced, apply the turn and return 1; otherwise steer
    toward the waypoint by at most turn per call and return 0
 */
INT Em_wander_turn(UINT speed, UCHAR* pTurn_flg, UCHAR* counter, SHORT turn, UCHAR limit);

/*
    return the signed turn (+Add_dir or -Add_dir) the current entity needs to face the target,
    or 0 when its facing is already within range
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
INT Goto00_ck(VECTOR* target, INT Add_dir);

/*
    turn the current entity toward the target by at most Add_dir
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
VOID Goto00(VECTOR* target, INT Add_dir);

/*
    steering angle of the current entity toward the scratchpad target: when the room collision
    blocks the way, latch a sideways Yoko_ofs direction and keep it for a Time_cnt, otherwise
    turn directly toward the target by at most Max_turn
 */
SHORT Em_wall_turn(INT Max_turn, UCHAR room_hit, SHORT* swerve_angle, UCHAR* swerve_latch);

/*
    return non-zero when pP lies outside the view cone of the current entity (half angle
    Hani_dir) or the line to it is blocked by room collision
 */
INT Dir_pos_ck(INT Hani_dir, VECTOR* pP);

/*
    return 1 when pP lies within Ll (summed X and Z distance) of the current entity and
    between the two edge rays at plus and minus ang of its facing
 */
INT L_pos_ckXZ(SHORT ang, SHORT Ll, VECTOR* pP);

/*
    collide the current entity against every other active enemy and return the combined hit
    flags; when enemy 0 has id 0xD or 0x12 and the player moved at least 0x1C3 units, ease
    the player back to a sixteenth of that displacement
 */
UCHAR Oba_ck_em(VOID);

/*
    recompute the animation base x/z of the current entity from its null-position offset and
    facing, and copy the result to pPm
 */
VOID Set_base_pos(ENEMY_WORK* pPm);

/*
    place pEm at its animation base plus the null-position offset of the given animation
    (header Kan, base Seq), rotated by its facing
 */
VOID Null_pos_set(ENEMY_WORK* pEm, KAN_HEAD* Kan, UCHAR* Seq);

/*
    return the buttons of mask 0xF0F0 pressed this frame (button-mashing check)
 */
INT Gacha_ch(VOID);

/*
    return 1 when pT lies on the negative side of the current entity's side axis (its heading
    plus 0x400), from the sign of the cross product with the vector to pT
 */
INT Cdir_ck2(VECTOR* pT);

/*
    return 1 when the facing of pT is within a quarter turn either side of the facing of the
    current entity
 */
INT Cdir_ck(ENEMY_WORK* pT);

/*
    start the special effect on a joint: tint its polygons (green for entity ids 3 and 4, red
    otherwise), store the duration and the two speeds, and repeat on the mirrored joint when
    mirroring is active
 */
VOID Joint_atck_set(PARTS_W* pP_ptr, SCHAR duration, SHORT Fall_spd, SHORT stride);

/*
    set every polygon of the joint's model to the flat colour r, g, b and flag the joint as
    running the special effect
 */
VOID Parts_col_set(PARTS_W* pP_ptr, UCHAR r, UCHAR g, UCHAR b);

/*
    replace the three vertex colours of every primitive of the joint (and of the mirrored
    joint when mirroring is active), keeping the mode byte of each colour word
 */
VOID Model_color_plus(PARTS_W* pMw, UINT Col0, UINT Pluscolor_1, UINT Pluscolor_2);

/*
    per-frame step of a joint whose faces fly apart as separate triangles: advance its height,
    then, when it is inside a camera zone, build a transform from the vertices of every
    vel_y-th face, project each triangle and queue it; ends the effect when the frame counter
    passes the duration
 */
VOID Joint_path_anim_step(PARTS_W* pP_ptr);

/*
    add 0x1E to the y coordinate of a random sixteenth of the model's vertices
 */
VOID Om_vtx_rnd(OBJ_MODEL_WORK* obj);

/*
    add 2 to the u texture coordinate of each vertex of the primitives of the joint listed in
    Scrl_prim_tbl
 */
VOID Parts_uv_scrl(PARTS_W* pP_ptr);

/*
    return non-zero when the line from the current entity to pTa is blocked by room collision
 */
UCHAR Sikai_ck(VECTOR* pTa);

/*
    combine the room collision line test from the current entity to the target over collision
    groups 3 down to 0
 */
INT Em_line_ck(VECTOR* target);

/*
    advance the current entity by its move speed along its facing plus muki, storing the step
    in its speed vector and adding it to its position
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
VOID Add_speedXZ(INT muki);

/*
    create n copies of the current entity in the load buffer, each with its own copy of the
    coordinate matrix, its facing turned by 0x100 times the number of copies still to create
    and a freshly linked object packet; the first copy is returned through out
 */
VOID Set_enemy_work(UCHAR n, INT unused_arg, UCHAR idx, ENEMY_WORK** out);

/*
    update and draw count web objects of the web spinner starting at the aim target of the
    current entity: run the swing and Dir_add states, burst a web (effect, sound, vibration)
    when the player touches it, Damage the player when a flying web hits, and queue the models
 */
VOID Em_web_proc(INT count);

/*
    advance the sight check counter of the current entity, testing the view of the player on
    its first four steps; when the player stayed in view record the player's position as the
    waypoint and return 1, return 0 when the player was out of view at any step, and 2 while
    the check is still counting
 */
INT Em_sikai_proc(VOID);

/*
    return 1 when pSv lies inside the square of half-size Ll around pP transformed by pM,
    in the X-Z plane
 */
INT L_pos_ckXZM(MATRIX* pM, VECTOR* pP, INT Ll, VECTOR* pSv);

/*
    step the 16-bit shift-register random generator and return its low byte
 */
UCHAR Rnd(VOID);

extern USHORT Scrl_prim_tbl[8];         // indices of the primitives whose texture coordinates are scrolled

#endif
