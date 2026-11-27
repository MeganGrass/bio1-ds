/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\JOINT.H
 */

#ifndef JOINT_H
#define JOINT_H

#include "TYPES.H"
#include <SYS/TYPES.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include "SCRATCHPAD.H"

struct ANM_REC
{
    USHORT Frm_num; // number of frames in the animation
    USHORT Frm_ofs; // byte offset of the animation's frame records inside the sequence table (low 2 bits masked off)
};

struct ANM_FRAME_REC
{
    USHORT Pose_idx;    // index of the joint pose row shown by the frame
    UCHAR Move_wait;    // frames the pose is held, copied to the entity Move_wait
};

struct KO_TBL
{
    UCHAR ko_num;       // number of child pSin_parts_ptr
    UCHAR Dummy1[1];    // padding
    USHORT ko_t_off;    // byte offset of the child joint list from the table
};

struct KAN_HEAD
{
    USHORT Ko_ofs;      // byte offset of the joint hierarchy table, low 2 bits unused
    SHORT Bone_ofs;    // byte offset of the pose frame data
    UCHAR Parts_cnt;    // number of pSin_parts_ptr
    UCHAR pad05[1];     // padding
    SHORT frame_size;  // byte size of one pose frame
};

/*
    load the player model of the current character into its model buffer, allocate the joint
    work, link the joint objects (including one extra part) and reset the pSin_parts_ptr
 */
VOID Pl_model_set(VOID);

/*
    map the TMD modeling data of model tmd, link its first object to the current entity and
    reset the entity attribute and blend counter
 */
VOID Em_tmd_link(VOID* tmd);

/*
    advance the current entity animation by one step: hold while the frame timer runs, else read
    the frame root offset and joint rotations from the pose data Kan_t_ptr and sequence table
    Seq_t_ptr, blending from the previous pose while the blend counter is non-zero;
    the frame counts from the end of the sequence when Late_flg is set;
    returns 1 when the sequence wrapped to its first frame
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
INT Joint_move(INT Late_flg, SHORT* Kan_t_ptr, UCHAR* Seq_t_ptr, INT Hokan_step);

/*
    link each joint of the current entity to its model object and preset it into the packet
    buffer starting at Packet_top; returns the end of the used packets
 */
UINT* PartsWork_link(UINT* Packet_top);

/*
    initialise the pSin_parts_ptr of the current entity: identity matrix with the translation read from
    the animation header, zero rotation and rotation speed; selects joint 1 for look-at
 */
VOID Init_Parts_work(VOID);

/*
    restore the translation of joint part to its animation header offset and flag it for a
    matrix update
 */
VOID Parts_pos_set(INT part);

/*
    compute the root translation of the current animation frame of pEm into the scratchpad
    vector sv_x/sv_y/Sv_z, with the frame taken from the end when R_flg is set;
    the entity frame index is left unchanged
 */
VOID get_null_pos(ENEMY_WORK* pEm, KAN_HEAD* Kan, UCHAR* Seq, UCHAR R_flg);

/*
    take the joint count from the animation header of pEm and allocate its joint array of
    Parts_size bytes per joint from the model work cursor
 */
VOID PartsWork_set(ENEMY_WORK* pEm, INT Parts_size);

/*
    copy view pV0 to pV1 reflected about the mirror plane mxz: the x coordinates when flg
    is set, the z coordinates otherwise
    old-style definition: its narrow parameters arrive promoted to int, so callers pass int values unconverted
 */
VOID Mirror_view(GsRVIEW2* pV0, GsRVIEW2* pV1, INT flg, INT mxz);

/*
    test whether point pos is seen in the mirror from view point pV0: both must lie on the
    same side of the mirror plane (x plane when flg is set, else z plane) and the line from
    pV0 to the reflection of pos must meet the plane inside [Mirror_min, Mirror_max);
    that coordinate is left in the scratchpad; returns 1 when seen
 */
INT Mirror_in_view(VOID* pV0, INT flg, VOID* pos);

/*
    draw the mirror reflection of the current entity: update the mirror pSin_parts_ptr, swap them in,
    set the view reflected about the mirror plane and a world matrix flipped in x, draw the
    pSin_parts_ptr, then restore the view and pSin_parts_ptr
 */
VOID Mirror_trans(VOID);

/*
    flip the winding of every triangle in the TMD of dobj by swapping two corner vertices and
    their texture coordinates
 */
VOID mirror_TMD_GT3(GsDOBJ5* dobj);

/*
    copy the pSin_parts_ptr and TMD of the current entity into the mirror work area, relink each part to
    its copy, flip the triangle winding and preset the packets
 */
VOID Mirror_model_cp(VOID);

/*
    reserve the mirror weapon TMD buffer (0x7A00 bytes) and packet buffer (0x1A00 bytes) from
    the model work cursor
 */
VOID Init_mirror_weapon(VOID);

/*
    copy the TMD of joint Weapon_num into the mirror weapon buffer, relink the mirror joint to
    it, flip the triangle winding and preset the packets
 */
INT Mirror_weapon_cp(INT Weapon_num);

/*
    copy the flags and world matrices of the pSin_parts_ptr to the mirror pSin_parts_ptr; for mirrored pSin_parts_ptr,
    set the visible flag when Mirror_in_view sees the joint and the original joint is visible,
    else clear it
 */
VOID Parts_work_cp(VOID);

/*
    compute the joint world matrices of the current entity: root matrix from the entity
    rotation (not refreshed when entity flag 0x80 is set), scaled by scale when non-zero, then each
    child through ko_joint_trans3; skipped for entity ids 0xD and 0x12 with behavior flag 1
 */
VOID Joint_pos_trans(INT scale);

/*
    compute the world matrix of joint idx from parent matrix wm when it is flagged for update,
    then recurse into its children
 */
VOID ko_joint_trans3(MATRIX* wm, UCHAR* arm, UCHAR idx, PARTS_W* pO);

/*
    draw the pSin_parts_ptr of pEm from the last to the first: set the lighting and screen matrices of
    each visible joint and queue its triangles into the ordering table, lit or unlit by flag
    0x80; skipped for entity ids 0xD and 0x12 with behavior flag 1
 */
VOID Joint_trans2(ENEMY_WORK* pEm);

/*
    build light matrix nlm in the space of local matrix lwm: transposed lwm times klwm, with the
    translation difference of klwm and lwm rotated by the transposed lwm
 */
VOID MulLMatrix(MATRIX* lwm, MATRIX* klwm, MATRIX* nlm);

extern USHORT Mirror_min;     // lower bound of the mirror range along the mirror plane
extern USHORT Mirror_max;     // upper bound of the mirror range along the mirror plane
extern USHORT Mirror_pos;     // mirror plane position (x or z coordinate)

#endif
