/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\NAMAE.H
 */

#ifndef NAMAE_H
#define NAMAE_H

#include "TYPES.H"

struct ITEM_NAME_DATA
{
    UCHAR Name00[13];   // 0x000 //
    UCHAR Name01[8];    // 0x00D //
    UCHAR Name02[8];    // 0x015 //
    UCHAR Name03[12];   // 0x01D //
    UCHAR Name04[13];   // 0x029 //
    UCHAR Name05[8];    // 0x036 //
    UCHAR Name06[12];   // 0x03E //
    UCHAR Name07[5];    // 0x04A //
    UCHAR Name08[7];    // 0x04F //
    UCHAR Name09[9];    // 0x056 //
    UCHAR Name10[14];   // 0x05F //
    UCHAR Name11[5];    // 0x06D //
    UCHAR Name12[13];   // 0x072 //
    UCHAR Name13[12];   // 0x07F //
    UCHAR Name14[13];   // 0x08B //
    UCHAR Name15[13];   // 0x098 //
    UCHAR Name16[6];    // 0x0A5 //
    UCHAR Name17[9];    // 0x0AB //
    UCHAR Name18[9];    // 0x0B4 //
    UCHAR Name19[9];    // 0x0BD //
    UCHAR Name20[10];   // 0x0C6 //
    UCHAR Name21[9];    // 0x0D0 //
    UCHAR Name22[7];    // 0x0D9 //
    UCHAR Name23[7];    // 0x0E0 //
    UCHAR Name24[15];   // 0x0E7 //
    UCHAR Name25[6];    // 0x0F6 //
    UCHAR Name26[13];   // 0x0FC //
    UCHAR Name27[11];   // 0x109 //
    UCHAR Name28[7];    // 0x114 //
    UCHAR Name29[12];   // 0x11B //
    UCHAR Name30[11];   // 0x127 //
    UCHAR Name31[10];   // 0x132 //
    UCHAR Name32[12];   // 0x13C //
    UCHAR Name33[11];   // 0x148 //
    UCHAR Name34[12];   // 0x153 //
    UCHAR Name35[9];    // 0x15F //
    UCHAR Name36[10];   // 0x168 //
    UCHAR Name37[8];    // 0x172 //
    UCHAR Name38[8];    // 0x17A //
    UCHAR Name39[11];   // 0x182 //
    UCHAR Name40[6];    // 0x18D //
    UCHAR Name41[7];    // 0x193 //
    UCHAR Name42[11];   // 0x19A //
    UCHAR Name43[11];   // 0x1A5 //
    UCHAR Name44[10];   // 0x1B0 //
    UCHAR Name45[11];   // 0x1BA //
    UCHAR Name46[8];    // 0x1C5 //
    UCHAR Name47[11];   // 0x1CD //
    UCHAR Name48[11];   // 0x1D8 //
    UCHAR Name49[12];   // 0x1E3 //
    UCHAR Name50[10];   // 0x1EF //
    UCHAR Name51[10];   // 0x1F9 //
    UCHAR Name52[11];   // 0x203 //
    UCHAR Name53[11];   // 0x20E //
    UCHAR Name54[11];   // 0x219 //
    UCHAR Name55[11];   // 0x224 //
    UCHAR Name56[14];   // 0x22F //
    UCHAR Name57[8];    // 0x23D //
    UCHAR Name58[8];    // 0x245 //
    UCHAR Name59[12];   // 0x24D //
    UCHAR Name60[12];   // 0x259 //
    UCHAR Name61[10];   // 0x265 //
    UCHAR Name62[9];    // 0x26F //
    UCHAR Name63[8];    // 0x278 //
    UCHAR Name64[12];   // 0x280 //
    UCHAR Name65[9];    // 0x28C //
    UCHAR Name66[11];   // 0x295 //
    UCHAR Name67[12];   // 0x2A0 //
    UCHAR Name68[12];   // 0x2AC //
    UCHAR Name69[13];   // 0x2B8 //
    UCHAR Name70[6];    // 0x2C5 //
    UCHAR Name71[9];    // 0x2CB //
    UCHAR Name72[11];   // 0x2D4 //
    UCHAR Name73[10];   // 0x2DF //
    UCHAR Name74[12];   // 0x2E9 //
    UCHAR Name75[1];    // 0x2F5 //
    UCHAR Name76[11];   // 0x2F6 //
    UCHAR Name77[1];    // 0x301 //
    UCHAR Name78[1];    // 0x302 //
    UCHAR Name79[1];    // 0x303 //
    UCHAR Name80[1];    // 0x304 //
    UCHAR Name81[1];    // 0x305 //
    UCHAR Name82[1];    // 0x306 //
    UCHAR Name83[1];    // 0x307 //
    UCHAR Name84[18];   // 0x308 //
    UCHAR Name85[15];   // 0x31A //
    UCHAR Name86[7];    // 0x329 //
    UCHAR Name87[12];   // 0x330 //
    UCHAR Name88[15];   // 0x33C //
    UCHAR Name89[4];    // 0x34B //
    UCHAR Name90[10];   // 0x34F //
    UCHAR Name91[16];   // 0x359 //
    UCHAR Name92[20];   // 0x369 //
    UCHAR Name93[16];   // 0x37D //
    UCHAR Name94[16];   // 0x38D //
    UCHAR Name95[12];   // 0x39D //
    UCHAR Name96[12];   // 0x3A9 //
    UCHAR Name97[12];   // 0x3B5 //
    UCHAR Name98[12];   // 0x3C1 //
    UCHAR Name99[8];    // 0x3CD //
    UCHAR pad[3];       // 0x3D5 //
};

typedef struct ITEM_NAME_DATA ITEM_NAME_DATA;

extern ITEM_NAME_DATA Item_name_pool;   // item name texts in game-font codes, each ended by 0x07
extern UCHAR* Name_tbl[];                // item name text, by item id - 1
extern UCHAR* Name_alt_tbl[];             // name text of an item whose examine flag is not set, by flag index

#endif      // NAMAE_H
