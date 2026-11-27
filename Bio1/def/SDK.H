/*
 *  Resident Evil Director's Cut DUAL SHOCK Version (1998)
 *  Complete Decompilation (MIPS R3000A ASM -> C)
 *
 *  Sony PlayStation (1994)
 *  SLUS-00747
 *
 *  D:\Bio1\def\SDK.H
 */

#ifndef SDK_H
#define SDK_H

#include "TYPES.H"

// PsyQ functions without a usable public prototype: no header of the PsyQ include folder the build uses (PSYQ_3_7\INCLUDE) declares them.
// GetGraphType, GsPrstTG3L and GsPrstTG3NL only appear inside macros or never-compiled sections of the SDK headers, and the libpad
// functions are in the PsyQ 4.2 LIBPAD.H, which is not on the include path. One prototype each, grouped by library. The game calls them
// without a parameter check, so GsPrstTG3L / GsPrstTG3NL keep the unprototyped form (the SDK's prototype has other parameter types).

// LIBGPU (PsyQ 3.7): the SDK only uses it inside the SetDrawMode / DR_MODE macros
INT GetGraphType(VOID);

// LIBGS (PsyQ 3.7): the TMD primitive sorters; only their address and a call without a return value are used
VOID GsPrstTG3L();
VOID GsPrstTG3NL();

// LIBPAD (PsyQ 4.2; custom LIBPAD.LIB, see makefile)
VOID PadInitDirect(UCHAR* pad1, UCHAR* pad2);
VOID PadStartCom(VOID);
INT PadSetMainMode(INT socket, INT offs, INT lock);
INT PadGetState(INT port);
INT PadInfoMode(INT port, INT term, INT offs);
VOID PadSetAct(INT port, UCHAR* table, INT width);
INT PadSetActAlign(INT port, UCHAR* table);

#endif
