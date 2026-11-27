/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\MONITOR.H
 */

#ifndef MONITOR_H
#define MONITOR_H

#include "TYPES.H"

// Index of each entry of the overlay API function table (Api_tbl_src in MONITOR.C, copied into Monitor.Func).
// Name rule: API_ + the function name in upper case, words split at '_' and at every lower-case/digit to upper-case change
// (GsMapModelingData -> GS_MAP_MODELING_DATA, L_pos_ckXZ -> L_POS_CK_XZ); a repeated function gets its index appended.
#define TASK_EXECUTE                   0
#define TASK_SLEEP                     1
#define TASK_EXIT                      2
#define TASK_KILL                      3
#define TASK_CHAIN                     4
#define TASK_SUSPEND                   5
#define TASK_SIGNAL                    6
#define TASK_LEXECUTE                  7
#define TASK_LCHAIN                    8
#define GS_MAP_MODELING_DATA           9
#define GS_INIT_COORDINATE2            10
#define GS_SET_LIGHT_MATRIX            11
#define GS_SET_PROJECTION              12
#define GS_PRESET_OBJECT               13
#define GS_SET_FLAT_LIGHT              14
#define GS_SET_LIGHT_MODE              15
#define GS_LINK_OBJECT5                16
#define GS_SET_LS_MATRIX               17
#define GS_SORT_OBJECT5                18
#define GS_SET_REF_VIEW2               19
#define GS_SET_AMBIENT                 20
#define GS_GET_LS                      21
#define GS_CLEAR_OT                    22
#define GS_GET_ACTIVE_BUFF             23
#define ROT_MATRIX                     24
#define SS_SET_SERIAL_ATTR             25
#define SS_SET_SERIAL_VOL              26
#define GS_SWAP_DISP_BUFF              27
#define STR_DECODE_WAIT                28
#define GS_INIT_GRAPH2                 29
#define STR_FRM_GET                    30
#define CLOSE_MOVIE                    31
#define GS_INIT_GRAPH                  32
#define PLAY_MOVIE                     33
#define STR_OPEN                       34
#define CLEAR_IMAGE                    35
#define LOAD_IMAGE                     36
#define DRAW_SYNC                      37
#define PAD_INIT                       38
#define PAD_RAW_GET                    39
#define VSYNC                          40
#define RESET_GRAPH                    41
#define BG_IMG_BUF_SET                 42
#define FADE_START                     43
#define MAPPING_TIM                    44
#define MOJI_TBL_PRINT                 45
#define LOAD_CLUT                      46
#define SND_SE_PLAY                    47
#define GAME_START                     48
#define INIT_GRAPH                     49
#define INIT_GEOM                      50
#define GS_DEF_DISP_BUFF               51
#define GS_INIT3_D                     52
#define CD_READ                        53
#define KAGE_WORK_SET                  54
#define KAGE_WORK_INIT                 55
#define INIT_PARTS_WORK                56
#define JOINT_MOVE                     57
#define OBA_SET_OFS                    58
#define OBA_CK_HIT                     59
#define OBA_CK_EM                      60
#define SCA_CK_POINT2                  61
#define SCA_CK_HIT                     62
#define HIT_CK_POINT4                  63
#define MUL_KAGE                       64
#define JOINT_POS_TRANS                65
#define EM_CK_HANI                     66
#define L_POS_CK_XZ                    67
#define SIKAI_CK                       68
#define GET_DIR_TGT                    69
#define RAND                           70
#define GOTO00_CK                      71
#define ADD_SPEED_XZ                   72
#define GOTO00                         73
#define EM_SIKAI_PROC                  74
#define CDIR_CK                        75
#define SET_BASE_POS                   76
#define NULL_POS_SET                   77
#define GACHA_CH                       78
#define ESP_CALL                       79
#define JOINT_ATCK_SET                 80
#define KAGE_WORK_COLOR_SET            81
#define KAGE_WORK_SIZE_ADD             82
#define COMP_MATRIX                    83
#define SQUARE0                        84
#define SQUARE_ROOT0                   85
#define SND_SYS_END                    86
#define TRANS_MATRIX                   87
#define SET_ROT_MATRIX                 88
#define SET_TRANS_MATRIX               89
#define SET_GEOM_SCREEN                90
#define SET_POLY_FT4                   91
#define GET_CLUT                       92
#define GET_TPAGE                      93
#define SET_SEMI_TRANS                 94
#define ROT_TRANS_PERS4                95
#define ADD_PRIM                       96
#define GS_SORT_FLIP_SPRITE            97
#define GS_SORT_FAST_SPRITE            98
#define SS_INIT                        99
#define SS_QUIT                        100
#define SND_TITLE_INIT                 101
#define SND_VAB3_CLOSE                 102
#define SND_SYS_ARMS_SET               103
#define SND_SE_STAD                    104
#define FADE_REQ_SET                   105
#define SPRINTF                        106
#define PRINT8                         107
#define PRINT14                        108
#define EM_WALL_TURN                   109
#define ROT_MATRIX_Y                   110
#define APPLY_MATRIX_SV                111
#define MODEL_COLOR_PLUS               112
#define MUL_MATRIX                     113
#define SCA_CK_POS                     114
#define GUIDE_CHECK                    115
#define ROOT_EDGE_SET                  116
#define ROT_VECTOR                     117
#define XA_CMD                         118
#define SET_ENEMY_WORK                 119
#define EM_WEB_PROC                    120
#define SCALE_MATRIX                   121
#define EM_CK_ALERT                    122
#define KAGE_HALF_SET                  123
#define FOOT_SET_PL                    124
#define L_POS_CK_XZM                   125
#define EM_FALL_MOVE                   126
#define SCA_CK_POINT_R                 127
#define MUL_LMATRIX                    128
#define SET_LIGHT                      129
#define MUL_MATRIX0                    130
#define SET_LIGHT_MATRIX               131
#define GS_PRST_TG3_L                  132
#define GS_MUL_COORD0                  133
#define MEMCPY                         134
#define APPLY_MATRIX_LV                135
#define CATAN                          136
#define LOAD_AVERAGE_SHORT12           137
#define SND_SE_WALK                    138
#define SND_SE_ENEM                    139
#define SPU_GET_ALL_KEYS_STATUS        140
#define ROT_NECK                       141
#define FLG_ON                         142
#define GS_MUL_COORD2                  143
#define LOAD_AVERAGE12                 144
#define STORE_IMAGE                    145
#define MUL_MATRIX2                    146
#define GS_MUL_COORD3                  147
#define FLG_OFF                        148
#define ROOT_CK                        149
#define RND                            150
#define CLUT_FADE_SABUN_ADD            151
#define CLUT_FADE_SET                  152
#define CLUT_FADE_OFF                  153
#define VECTOR_NORMAL                  154
#define CARD_ACCESS                    155
#define EM_WANDER_TURN                 156
#define ESP_CALL_API                   157
#define FLG_CK                         158
#define SCE_AT_CHECK                   159
#define ESP_INIT_C                     160
#define ESP_KILL                       161
#define ESP_MOVE                       162
#define MAPPING_TMD                    163
#define SET_MOVIE_VOLUME2              164
#define CD_SET_STEREO                  165
#define MEMCLR                         166
#define SRAND                          167
#define GS_PRST_TG3_NL                 168
#define EM0C_TRAIL_ALLOC               169
#define EM0C_TRAIL_PUSH                170
#define STATUS_INIT                    171
#define GS_SORT_SPRITE                 172
#define GS_SORT_BOX_FILL               173
#define SND_VAB1_LOAD                  174
#define PAD_VIB_SET0                   175
#define PAD_VIB_SET1                   176
#define PAD_VIB_FADE_SET               177
#define CONFIG                         178
#define FNT_PRINT                      179
#define OPEN_MOVIE                     180
#define CD_FILE_READ                   181
#define API_END                            182  // terminator entry
#define API_COUNT                          183  // number of entries, including the terminator

typedef INT (*API_FUNC)(); // overlay API function

typedef struct
{
    UCHAR b[2];             // two bytes of one XA sector table entry
} XA_SECTOR_ENT;

struct TASK
{
    USHORT Tcb_status;    // task state; 1 sleeping, 2 start requested, 4 resume requested, 0x7F running
    SHORT Tcb_sleep_ctr;  // frames left to sleep
    LONG (*pTcb_pc)();    // entry point of the task
    ULONG Tcb_th;         // thread handle
    struct TCB* pTcb;     // thread control block of the task
    ULONG Tcb_init_sp;    // initial stack pointer
    struct EXEC Tcb_exec; // executable header of the loaded task program
    ULONG* pPc;           // reserved, not accessed
    USHORT R_no[6];       // reserved, not accessed
    SHORT Ctr[8];         // reserved, not accessed
    UCHAR Free[16];       // task work bytes used by the loaded program
};

struct MONITOR
{
    API_FUNC Func[API_COUNT];       // function table the overlay programs call
    UCHAR Dummy2dc[0x24];           // padding
    UCHAR* pBs_check_sum;           // background file checksum table of the stage
    UCHAR* pBgm_room_tbl;           // room background music table of the stage
    XA_SECTOR_ENT* pXa_sector;      // XA voice sector table of the stage
    MATRIX* pLight_mtx;             // light world-screen matrix
    MATRIX* pId_mat;                // identity matrix (GsIDMATRIX), translation is the zero vector
    MATRIX* pWs_mat;                // world-screen matrix
    API_FUNC Api_ptr;               // scratch slot holding the table entry being called
    VOID (**Em_move_tbl)(VOID);               // update routine of each enemy type
    UCHAR Dummy320[4];              // padding
    TASK Task[3];                   // task slots
    UCHAR Task2_stack[0x2DC];       // stack of task 2
    UCHAR Task1_stack[0x300];       // stack of task 1
    UCHAR Task0_stack[0x700];       // stack of task 0
};

#define Monitor (*(MONITOR*)MONITOR_ADR) // task scheduler workspace
#define FUNC(Id) (Monitor.Api_ptr = Monitor.Func[Id]) // call an overlay API function through the table: FUNC(INIT_PARTS_WORK)(args)
#define Api_rand() FUNC(RAND)() // rand() of the main program, called through the API table

/*
    set up the three task slots (thread control blocks, initial stack pointers, global
    pointer), copy the overlay API table into the scheduler workspace and record the main
    global pointer
 */
VOID Init_scheduler(VOID);

/*
    run the three task slots once in order: start a new task through OpenTh, count down a
    sleeping task, and switch to every ready task through ChangeTh
 */
VOID Scheduler(VOID);

/*
    load program file Task_no from disc into task slot Level (header sector first, then the
    body at its load address), give it the main global pointer and start it
 */
VOID Task_lexecute(INT Level, INT Task_no);

/*
    replace the current task with program file Task_no, loaded with blocking reads, keeping
    its global pointer, and chain to its entry
 */
VOID Task_lchain(INT Task_no);

/*
    start entry pFuncall in task slot Level on the next scheduler pass
 */
VOID Task_execute(INT Level, VOID (*pFuncall)(VOID));

/*
    put the current task to sleep for Sleep_counter scheduler passes, saving the return
    address, and switch back to the scheduler
    Declared without a parameter list: callers pass extra arguments; old-style definition (narrow parameter)
 */
VOID Task_sleep();

/*
    end the current task: free its slot, close its thread and switch to the scheduler
 */
VOID Task_exit(VOID);

/*
    end the task in slot Level and close its thread when the slot is in use
 */
VOID Task_kill(INT Level);

/*
    restart the current task at pFuncall: close its thread, mark the slot for a new start
    and switch to the scheduler
 */
VOID Task_chain(LONG (*pFuncall)());

/*
    suspend task slot Level so the scheduler skips it (status bit 0x40)
 */
VOID Task_suspend(INT Level);

/*
    resume a suspended task slot Level (clears status bit 0x40)
 */
VOID Task_signal(INT Level);

/*
    return the status word of task slot Level (0=free)
 */
INT Task_status(INT Level);

extern TASK* Task_now;            // task slot being set up or scheduled
extern VOID* Api_tbl_src[];         // overlay API function table, copied into the scheduler workspace at start-up

#endif
