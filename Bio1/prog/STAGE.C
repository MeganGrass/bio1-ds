/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\prog\STAGE.C
 */

#include "TYPES.H"
#include "STAGE.H"
#include "EM13.H"
#include "ROOM.H"
#include "SCE0.H"
#include "SND.H"
#include "SUB_PL.H"
#include "MEM_MAP.H"
#include "FLAG.H"
#include "MONITOR.H"

VOID Set_stage(VOID)
{
    RECT vram_rect;

    G.SaveData.Room_no_old = 0x1F;

    BGM_TBL_ADR_CUR = MAIN.SaveData.Bgm_tbl + (G.SaveData.Stage_no << 5);

    if (G.Scenario.Sep_no >= 0)
    {
        Snd_bgm_fade_all();
    }
    Bgm_status_mask = 0xFF;
    FREE_WORK_CUR = (UINT*)OVERLAY_AREA;  // address form kept: the plain store changes the code
    Stage_ov_load();

    G.pFree_work_head = MAIN.pFree_work;

    if (G.Status_flg & STAT_DEMO_MODE)
    {
        MAIN.Bgm_tbl_adr[G.SaveData.Room_no] = 0xFF;
    }

    vram_rect.x = 0x3C0;
    vram_rect.y = 0x1C4;
    vram_rect.w = 0x40;
    vram_rect.h = 0x38;

    LoadImage(&vram_rect, MAIN.pFree_work);

    Set_room();
}

VOID Stage_ov_load(VOID)
{
    VOID (**pMove_tbl)();

    Task_lexecute(2, (USHORT)((SHORT*)Stage_ov_tbl)[G.SaveData.Stage_no]);  // cast kept: overlay table entry read as a 16-bit file code from a byte table
    G.pFree_work = Monitor.Task[2].Tcb_exec.t_size + (UCHAR*)OVERLAY_AREA;
    Task_sleep(1);
    pMove_tbl = Monitor.Em_move_tbl;
    pMove_tbl[0x13] = Em13_Main;

    pMove_tbl[0x17] = Spl;
    pMove_tbl[0x18] = Spl;
    pMove_tbl[0x19] = Spl;
    pMove_tbl[0x1A] = Spl;
    pMove_tbl[0x1B] = Spl;
    pMove_tbl[0x1C] = Spl;
    pMove_tbl[0x1D] = Spl;
    pMove_tbl[0x1E] = Spl;
    pMove_tbl[0x1F] = Spl;
    pMove_tbl[0x20] = Spl;
    pMove_tbl[0x21] = Spl;
    pMove_tbl[0x22] = Spl;
    pMove_tbl[0x23] = Spl;
    pMove_tbl[0x24] = Spl;
    pMove_tbl[0x25] = Spl;
    pMove_tbl[0x26] = Spl;
    pMove_tbl[0x27] = Spl;
    pMove_tbl[0x28] = Spl;
    pMove_tbl[0x29] = Spl;
    pMove_tbl[0x2A] = Spl;
    pMove_tbl[0x2B] = Spl;
    pMove_tbl[0x2C] = Spl;
    pMove_tbl[0x2D] = Spl;
    pMove_tbl[0x2E] = Spl;
    pMove_tbl[0x2F] = Spl;
}

// overlay file codes of the stages (16-bit entries indexed by stage id), then data not read here
UCHAR Stage_ov_tbl[0x118] =
{
    0x57,   // 0 //
    0x01,   // 1 //
    0x58,   // 2 //
    0x01,   // 3 //
    0x59,   // 4 //
    0x01,   // 5 //
    0x5A,   // 6 //
    0x01,   // 7 //
    0x5B,   // 8 //
    0x01,   // 9 //
    0x5C,   // 10 //
    0x01,   // 11 //
    0x5D,   // 12 //
    0x01,   // 13 //
    0x00,   // 14 //
    0x00,   // 15 //
    0x08,   // 16 //
    0x00,   // 17 //
    0x0C,   // 18 //
    0x00,   // 19 //
    0x08,   // 20 //
    0x00,   // 21 //
    0x08,   // 22 //
    0x00,   // 23 //
    0x50,   // 24 //
    0x00,   // 25 //
    0x78,   // 26 //
    0x00,   // 27 //
    0x08,   // 28 //
    0x00,   // 29 //
    0x84,   // 30 //
    0x00,   // 31 //
    0x08,   // 32 //
    0x00,   // 33 //
    0x08,   // 34 //
    0x00,   // 35 //
    0x58,   // 36 //
    0x00,   // 37 //
    0x78,   // 38 //
    0x00,   // 39 //
    0xC0,   // 40 //
    0x00,   // 41 //
    0x0C,   // 42 //
    0x00,   // 43 //
    0x08,   // 44 //
    0x00,   // 45 //
    0x08,   // 46 //
    0x00,   // 47 //
    0x60,   // 48 //
    0x00,   // 49 //
    0x70,   // 50 //
    0x00,   // 51 //
    0xC0,   // 52 //
    0x00,   // 53 //
    0x84,   // 54 //
    0x00,   // 55 //
    0x08,   // 56 //
    0x00,   // 57 //
    0x08,   // 58 //
    0x00,   // 59 //
    0x60,   // 60 //
    0x00,   // 61 //
    0x78,   // 62 //
    0x00,   // 63 //
    0x10,   // 64 //
    0x00,   // 65 //
    0x0C,   // 66 //
    0x00,   // 67 //
    0x10,   // 68 //
    0x00,   // 69 //
    0x08,   // 70 //
    0x00,   // 71 //
    0x30,   // 72 //
    0x00,   // 73 //
    0x78,   // 74 //
    0x00,   // 75 //
    0x10,   // 76 //
    0x00,   // 77 //
    0x84,   // 78 //
    0x00,   // 79 //
    0x10,   // 80 //
    0x00,   // 81 //
    0x08,   // 82 //
    0x00,   // 83 //
    0x40,   // 84 //
    0x00,   // 85 //
    0x78,   // 86 //
    0x00,   // 87 //
    0x08,   // 88 //
    0x00,   // 89 //
    0x14,   // 90 //
    0x00,   // 91 //
    0x18,   // 92 //
    0x00,   // 93 //
    0x70,   // 94 //
    0x00,   // 95 //
    0x98,   // 96 //
    0x00,   // 97 //
    0x00,   // 98 //
    0x00,   // 99 //
    0xC0,   // 100 //
    0x00,   // 101 //
    0x14,   // 102 //
    0x00,   // 103 //
    0x08,   // 104 //
    0x00,   // 105 //
    0x70,   // 106 //
    0x00,   // 107 //
    0xB0,   // 108 //
    0x00,   // 109 //
    0x00,   // 110 //
    0x00,   // 111 //
    0x20,   // 112 //
    0x00,   // 113 //
    0x0C,   // 114 //
    0x00,   // 115 //
    0xA0,   // 116 //
    0x00,   // 117 //
    0x0C,   // 118 //
    0x00,   // 119 //
    0x00,   // 120 //
    0x00,   // 121 //
    0x98,   // 122 //
    0x00,   // 123 //
    0x08,   // 124 //
    0x00,   // 125 //
    0x8C,   // 126 //
    0x00,   // 127 //
    0x80,   // 128 //
    0x00,   // 129 //
    0x28,   // 130 //
    0x00,   // 131 //
    0x00,   // 132 //
    0x00,   // 133 //
    0xB8,   // 134 //
    0x00,   // 135 //
    0x94,   // 136 //
    0x00,   // 137 //
    0x8E,   // 138 //
    0x00,   // 139 //
    0x3C,   // 140 //
    0x00,   // 141 //
    0x26,   // 142 //
    0x00,   // 143 //
    0x84,   // 144 //
    0x00,   // 145 //
    0xBA,   // 146 //
    0x00,   // 147 //
    0xCC,   // 148 //
    0x00,   // 149 //
    0x0C,   // 150 //
    0x00,   // 151 //
    0x68,   // 152 //
    0x00,   // 153 //
    0x04,   // 154 //
    0x00,   // 155 //
    0x00,   // 156 //
    0x00,   // 157 //
    0x80,   // 158 //
    0x00,   // 159 //
    0xCC,   // 160 //
    0x00,   // 161 //
    0x30,   // 162 //
    0x00,   // 163 //
    0x68,   // 164 //
    0x00,   // 165 //
    0x04,   // 166 //
    0x00,   // 167 //
    0x00,   // 168 //
    0x00,   // 169 //
    0x84,   // 170 //
    0x00,   // 171 //
    0xD0,   // 172 //
    0x00,   // 173 //
    0x92,   // 174 //
    0x00,   // 175 //
    0x08,   // 176 //
    0x00,   // 177 //
    0x1E,   // 178 //
    0x00,   // 179 //
    0xB8,   // 180 //
    0x00,   // 181 //
    0x2E,   // 182 //
    0x00,   // 183 //
    0xD8,   // 184 //
    0x00,   // 185 //
    0xB0,   // 186 //
    0x00,   // 187 //
    0x04,   // 188 //
    0x00,   // 189 //
    0x08,   // 190 //
    0x00,   // 191 //
    0x68,   // 192 //
    0x00,   // 193 //
    0x80,   // 194 //
    0x00,   // 195 //
    0x2C,   // 196 //
    0x01,   // 197 //
    0xB0,   // 198 //
    0x00,   // 199 //
    0x04,   // 200 //
    0x00,   // 201 //
    0x08,   // 202 //
    0x00,   // 203 //
    0x7C,   // 204 //
    0x00,   // 205 //
    0x80,   // 206 //
    0x00,   // 207 //
    0xD0,   // 208 //
    0x00,   // 209 //
    0xB0,   // 210 //
    0x00,   // 211 //
    0x08,   // 212 //
    0x00,   // 213 //
    0x08,   // 214 //
    0x00,   // 215 //
    0x88,   // 216 //
    0x00,   // 217 //
    0x50,   // 218 //
    0x00,   // 219 //
    0x30,   // 220 //
    0x01,   // 221 //
    0xB0,   // 222 //
    0x00,   // 223 //
    0x08,   // 224 //
    0x00,   // 225 //
    0x08,   // 226 //
    0x00,   // 227 //
    0x90,   // 228 //
    0x00,   // 229 //
    0x50,   // 230 //
    0x00,   // 231 //
    0x00,   // 232 //
    0x00,   // 233 //
    0xB6,   // 234 //
    0x00,   // 235 //
    0xC0,   // 236 //
    0x00,   // 237 //
    0x02,   // 238 //
    0x00,   // 239 //
    0x00,   // 240 //
    0x00,   // 241 //
    0xA6,   // 242 //
    0x00,   // 243 //
    0xC0,   // 244 //
    0x00,   // 245 //
    0xB6,   // 246 //
    0x00,   // 247 //
    0x80,   // 248 //
    0x00,   // 249 //
    0x02,   // 250 //
    0x00,   // 251 //
    0x00,   // 252 //
    0x00,   // 253 //
    0xA6,   // 254 //
    0x00,   // 255 //
    0x00,   // 256 //
    0x00,   // 257 //
    0xD8,   // 258 //
    0x00,   // 259 //
    0x78,   // 260 //
    0x00,   // 261 //
    0x10,   // 262 //
    0x00,   // 263 //
    0x00,   // 264 //
    0x00,   // 265 //
    0x88,   // 266 //
    0x00,   // 267 //
    0x78,   // 268 //
    0x00,   // 269 //
    0xD8,   // 270 //
    0x00,   // 271 //
    0xC8,   // 272 //
    0x00,   // 273 //
    0x10,   // 274 //
    0x00,   // 275 //
    0x00,   // 276 //
    0x00,   // 277 //
    0xA8,   // 278 //
    0x00,   // 279 //
};
