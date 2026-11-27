/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\def\MEM_MAP.H
 */

#ifndef MEM_MAP_H
#define MEM_MAP_H

// fixed RAM addresses of the objects Monitor and ScratchPad
#define SCRATCHPAD_ADR               0x1F800000          // ScratchPad: the 1 KB fast RAM of the CPU (struct SCRATCHPAD_BUF)
#define MONITOR_ADR                  0x801FE900          // Monitor: the task scheduler workspace (struct MONITOR)

// player, weapon and effect work areas
#define VCUT_SENTINEL                ((VOID*)0x800C0000) // Fake list head used as prev-link while walking camera switch zones
#define PL_EMD_BUF                   ((VOID*)0x800D0000) // Buffer where player model data and room model files are loaded
#define PL_PAC_BUF                   ((VOID*)0x800DCC00) // Parts work array for models; linked and used as packet area
#define WEP_ANIM_BUF                 ((VOID*)0x800EA800) // Load area for the equipped weapon model data
#define WEP_PACKET_BUF               ((VOID*)0x800F2200) // End of the weapon model load area; also used as the weapon anim object slot passed
                                                         // to weapon change
#define ESP_WORK_BUF                 ((VOID*)0x800F3C00) // Room data load pointer for rooms loaded below the main load area
#define SND_VH_OFS_TBL               ((VOID*)0x800F53F8) // Table just before the VAB header copy; holds per-bank header offsets

// sound bank and effect table buffers
#define Arms_edt_buf                 ((VOID*)0x800F5400) // Temp buffer for sound bank header and effect table loaded from CD
#define Core_edt_buf                 ((VOID*)0x800F6400) // Buffer holding loaded sound effect table and sample data
#define SND_EDT_BUF                  ((VOID*)0x800F7400) // Sound effect (EDT) table and its data buffer
#define Bgm_main_vh_buf              ((VOID*)0x800F8400) // BGM VAB header and sequence data buffer opened as sticky VAB

// logo movie buffers and the overlay load area
#define LOGO_SLICE_BUF               ((VOID*)0x80100000) // Logo movie: slice buffer for raw STR data read from disc
#define LOGO_RING_BUF                ((VOID*)0x80102D00) // Movie stream ring buffer for the logo movie
#define OVERLAY_AREA                 ((VOID*)0x80105400) // Load area for room/stage data and overlay programs
#define LOGO_VLC_BUF0                ((VOID*)0x8010ED00) // First VLC decode buffer for the logo movie stream
#define SYS_PKT_BUF1                 ((VOID*)0x80115400) // Second system GPU packet buffer (double-buffered with the first)
#define LOGO_VLC_BUF1                ((VOID*)0x8011ED00) // Logo movie: second VLC decode buffer (double buffering)

// title screen sprite data and the title overlay
#define TITLE_SPRITE_SET_0_BUFFER    ((VOID*)0x80155400) // First title image/sprite data, loaded then sent to VRAM
#define TITLE_SPRITE_TIM_SECOND      ((VOID*)0x80157C40) // Second title sprite TIM data in the loaded title overlay
#define TITLE_SPRITE_DATA            ((VOID*)0x8015A4C0) // Title screen sprite data passed to overlay loader API
#define TITLE_OVERLAY_API_ARG_BUFFER ((VOID*)0x8015CD00) // Argument buffer passed to the title overlay API call
#define TITLE_SPRITE_TIM_7           ((VOID*)0x8015F540) // Title screen sprite image data (TIM) loaded for sprite slot 7
#define TITLE_SPRITE_SET_7_BUFFER    ((VOID*)0x80161D80) // Last title sprite data block, sent to VRAM
#define TITLE_OVERLAY_LOAD_AREA      ((VOID*)0x80180000) // Load area of the overlay program started from the title

// CD loader and the shared work buffer
#define SND_VB_BUF                   ((VOID*)0x801C10EC) // Load buffer for core VAB sound bank body before SPU transfer
#define CD_HEAD_BUF                  ((VOID*)0x801D08EC) // Buffer receiving the first CD sector of a file before full load
#define CD_REG_BAK                   ((VOID*)0x801D08FC) // Saved task register context copied into a task slot after a CD read
#define TIM_BUF                      ((VOID*)0x801D10D8) // CD read destination just before the shared work buffer
#define LOAD_BUF                     ((VOID*)0x801D10EC) // Large shared work buffer for file loads and image decoding
#define DOOR_TMD_OFS_ADDR            ((VOID*)0x801D10F0) // Door model data header field, offset made absolute on load
#define LOAD_TIM_OFS_ADDR            ((VOID*)0x801D10F4) // Offset slot in shared buffer header, relocated to absolute pointer
#define LOAD_PIX_BUF                 ((VOID*)0x801D1100) // Start of decoded 32-bit image pixels in the shared work buffer
#define MDEC_DEC_BUF                 ((VOID*)0x801D30EC) // Scratch area for movie frame decode, inside the shared buffer
#define DOOR_PKT_TOP_ADDR            ((VOID*)0x801DD0F8) // Door scene packet buffer counter area

// map, menu and item viewer buffers
#define MENU_TEX_BUF                 ((VOID*)0x801E10EC) // Load buffer for map and menu images before VRAM upload
#define FILE_CLUT_DATA_BUFFER        ((VOID*)0x801E110C) // Palette (CLUT) data uploaded to VRAM from the loaded file
#define MAP_FLOOR_FLG                ((VOID*)0x801E110E) // Per-floor map visited flags inside the loaded map floor file
#define MAP_ROOM_FLG2                ((VOID*)0x801E111A) // Map room table entries read as shorts, second half
#define FILE_SCREEN_CLUT_DATA        ((VOID*)0x801E112C) // Palette data uploaded to VRAM for the file screen
#define ST_ITEM_TMD                  ((VOID*)0x801E130C) // Status menu 3D model (TMD) inside the loaded image buffer
#define ITEM_VIEW_TMD                ((VOID*)0x801E1310) // Item viewer model data mapped by the graphics system
#define ITEM_VIEW_NOBJ               ((VOID*)0x801E1314) // Object count of the item viewer model after mapping its TMD data
#define ITEM_VIEW_DATA               ((VOID*)0x801E1318) // Model data (TMD) of the item viewer after its header

// door scene packet areas, image transfer buffer and the task stack
#define DOOR_PKT_A_ADDR              ((VOID*)0x801E6D38) // First system GPU packet area used during door scenes
#define DOOR_PKT_B_ADDR              ((VOID*)0x801EEB1C) // Second system GPU packet area used during door scenes (double buffer)
#define IMG_TRANS_BUF                ((VOID*)0x801F6900) // Scratch buffer for VRAM image store/load and decoded background
#define TASK0_STACK_TOP              0x801FFA80          // Initial stack pointer of the first scheduler task

#endif      // MEM_MAP_H
