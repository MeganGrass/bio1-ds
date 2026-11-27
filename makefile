## Resident Evil Director's Cut DUAL SHOCK Version (1998)
## Complete Decompilation (MIPS R3000A ASM -> C)
##
## Sony PlayStation (1994)
## SLUS-00747

## Builds the USA PS1 executables: OBJ\PSX\USA\SLUS_007.47 and OBJ\PSX\USA\PROG2\*.EXE (12 overlays).
##
##   setup.bat          once: builds GCC 2.6.3 into tools\gcc-2.6.3-psx (tools\build_gcc.bat), then the custom PSY-Q libraries into tools\custom-sdk ("make sdklib")
##   build.bat psx-usa  or  make [-j N]   compile every unit, link the 13 programs, check them against retail
##   make clean | rebuild
##
## tools\sdkroot.ini defines the local PSY-Q SDK paths; tools\cc263.exe and the other tools read it too.
-include tools/sdkroot.ini
$(foreach v,PSYQ_3_3 PSYQ_3_5 PSYQ_3_7 PSYQ_4_0 PSYQ_4_2 PSYQ_4_3,$(if $($(v)),,$(error $(v) is not set -- add it to tools\sdkroot.ini)))

## location of the custom PSY-Q libraries that setup.bat builds
CUSTOM_SDK	=tools\custom-sdk
SHELL		:=C:/Windows/System32/cmd.exe

## The devkitPro make is an MSYS program. MSYS rewrites a lone "/c" argument into the path "C:/" before it starts cmd.exe, so
## "//c" is passed instead. A native Windows make passes "/c" unchanged.
## PSYLINK's own /c /m /n flags are mangled the same way unless PSYLINK is the literal first word of its recipe line, so it is.
ifneq ($(findstring msys,$(MAKE_HOST))$(findstring cygwin,$(MAKE_HOST)),)
.SHELLFLAGS	:=//c
else
.SHELLFLAGS	:=/c
endif
PATH		:=$(PSYQ_4_0)\PSYQ;$(PATH)
CC			=tools\cc263.exe
LINK		=PSYLINK
CPE			=tools\cpe2exe.exe

## Directories: objects are binaries and go to OBJ (one file per unit, named like its source); intermediates (.i .s, link scripts, maps) go to
## tmp\build\psx-usa. The sources of the main program are in Bio1\prog; those of the overlay programs themselves (ENDING LOGO PROLOGUE SELECT
## STAGE1..7 TITLE, .C and .H) are in Bio1\prog2; the base headers (TYPES FLAG MEM_MAP SCENARIO SCRATCHPAD SDK FILECODE FILE_LOC) are in Bio1\def. A unit of overlay stageN is stageN_<name>.c.
OBJ_DIR		=OBJ\PSX\USA
OBJ_FWD		=OBJ/PSX/USA
STAGE		=tmp\build\psx-usa
SRC			=Bio1\prog
LINKFILE	=makefile.link

## tools\cc263.exe: preprocess with GCC 2.6.3's cpp, compile with its cc1 (tools\gcc-2.6.3-psx), convert the assembly to CRLF and
## assemble it with ASPSX 2.56 (PSY-Q 4.0) into a PsyQ object. -G 0: no small-data (.sbss) split, so every uninitialised static lands in
## one .bss in declaration order, as in retail. The main program is compiled at -O2, the overlays at -O0. cc263 searches the folder of
## the source for the #include files; the PsyQ headers (<LIBGPU.H> ...) are found through -I.
CCFLAGS_main	=-O2 -G0 -A0
CCFLAGS_ovl		=-O0 -G0 -A0
INCOPT			=-I$(SRC) -IBio1\def -I$(STAGE)\sdkinc

## The PsyQ headers the sources include (<LIBGPU.H> ...): GCC 2.6.3's preprocessor cannot read the SDK's CRLF files (a backslash before CRLF is not a
## line continuation), so LF copies are made in tmp\build\psx-usa\sdkinc, once, from the SDK of tools\sdkroot.ini (PSYQ_3_7).
SDK_HEADERS	:=ABS ASM ASSERT CONVERT CTYPE FS GTEMAC GTENOM GTEREG INLINE_A INLINE_C INLINE_O KERNEL LIBCD LIBCOMB LIBETC LIBGPU LIBGS LIBGTE LIBGUN LIBMATH LIBPRESS LIBSIO LIBSN LIBSND LIBSPU LIBTAP LIMITS MALLOC MEMORY QSORT R3000 RAND ROMIO SETJMP STDARG STDDEF STDIO STDLIB STRING STRINGS SYS/ERRNO SYS/FCNTL SYS/FILE SYS/IOCTL SYS/TYPES
SDKINC_FWD	=tmp/build/psx-usa/sdkinc
SDKINC_H	=$(foreach h,$(SDK_HEADERS),$(SDKINC_FWD)/$(h).H)
$(SDKINC_FWD)/%.H: tools/lf2crlf.exe
	if not exist $(subst /,\,$(patsubst %/,%,$(dir $@))) mkdir $(subst /,\,$(patsubst %/,%,$(dir $@)))
	tools\lf2crlf.exe --lf $(PSYQ_3_7)\INCLUDE\$(subst /,\,$*).H $(subst /,\,$@)

## /n raises PSYLINK 2.52's module limit: the main link takes about 50 objects plus the library modules it pulls in
LINKOPT		=/c /m /n 1024

## Catches "forgot to run setup.bat" up front with a clear message, instead of a confusing failure deep in the PSYLINK step.
ifeq ($(filter sdklib clean,$(MAKECMDGOALS)),)
ifeq ($(wildcard $(CUSTOM_SDK)/LIBPAD.LIB),)
$(error $(CUSTOM_SDK)\LIBPAD.LIB not found -- run setup.bat once to build the custom PSY-Q libraries)
endif
ifeq ($(wildcard $(CUSTOM_SDK)/LIBGS.LIB),)
$(error $(CUSTOM_SDK)\LIBGS.LIB not found -- run setup.bat again to rebuild the custom PSY-Q libraries)
endif
endif

## Units of each program, in link order (the order is part of the result: it decides every address)
MAIN_UNITS	:=attack cd cinesco config cut mem_card door eff0 eff1 em13 enemy event file game guide_ck inventory item joint mag main map mojidisp monitor movie namae psp oba sce_at om oma options pad player pl_wep pl_sce room sce0 sce1 sce2 sce3 sca snd stage st_sub00 st_sub01 st_sub02 st_sub03 sub_pl sub str zentai em_sub tyrant
OVERLAYS	:=logo prologue title select ending stage1 stage2 stage3 stage4 stage5 stage6 stage7
logo_UNITS	:=logo
prologue_UNITS	:=prologue
title_UNITS	:=title
select_UNITS	:=select
ending_UNITS	:=ending
stage1_UNITS	:=stage1 stage1_em00 stage1_em02 stage1_em05 stage1_em0f
stage2_UNITS	:=stage2 stage2_em00 stage2_em05 stage2_em0d
stage3_UNITS	:=stage3 stage3_em02 stage3_em04 stage3_em06 stage3_em0a stage3_em0c
stage4_UNITS	:=stage4 stage4_em00 stage4_em03 stage4_em07 stage4_em08 stage4_em0b stage4_em0e stage4_em0f
stage5_UNITS	:=stage5 stage5_em00 stage5_em09 stage5_em0c stage5_em14 stage5_em15
stage6_UNITS	:=stage6 stage6_em00 stage6_em03 stage6_em05 stage6_em06 stage6_em0f
stage7_UNITS	:=stage7 stage7_em00 stage7_em05 stage7_em06 stage7_em0d

## Every object depends on every header: a changed header recompiles everything (no per-header tracking, as in bio2-ds)
HEADERS		:=$(wildcard Bio1/prog/*.H Bio1/prog2/*.H Bio1/def/*.H)

all: build

## upper case of a word (the overlays are named in upper case in makefile.link: STAGE3, LOGO, ...)
uc = $(subst z,Z,$(subst y,Y,$(subst x,X,$(subst w,W,$(subst v,V,$(subst u,U,$(subst t,T,$(subst s,S,$(subst r,R,$(subst q,Q,$(subst p,P,$(subst o,O,$(subst n,N,$(subst m,M,$(subst l,L,$(subst k,K,$(subst j,J,$(subst i,I,$(subst h,H,$(subst g,G,$(subst f,F,$(subst e,E,$(subst d,D,$(subst c,C,$(subst b,B,$(subst a,A,$(1)))))))))))))))))))))))))))

## unit rule: $(1) program (main or an overlay), $(2) unit (the file name of the source without .C, in lower case here and in
## makefile.link: stage3_em06 is Bio1\prog\EM06.C and the object stage3_em06.obj). The sections of an overlay's objects
## are named after the overlay (-sSTAGE3: STAGE3.text, STAGE3.data, ...) so that makefile.link can give every overlay a group of its own;
## the main program's objects keep .text, .data, ... The compiler's intermediate files (.i, .s) go to tmp\build\psx-usa\cc.
##
## The source of unit stageN_<NAME> is <NAME>.C when that file exists (one source per enemy, compiled once for every overlay that has the
## enemy; -DSTAGE=N selects what differs between the overlays), else STAGEN_<NAME>.C (code of that overlay alone). Every other unit's
## source is <UNIT>.C. Each program has an intermediate folder of its own because the overlays compile the same source in parallel.
srcname = $(call uc,$(if $(wildcard Bio1/prog/$(call uc,$(patsubst $(1)_%,%,$(2))).C),$(patsubst $(1)_%,%,$(2)),$(2)))
## the folder of a source: the overlay programs' own units (ENDING, LOGO, PROLOGUE, SELECT, STAGE1..7, TITLE) are in Bio1\prog2, all others in Bio1\prog
srcpath = $(firstword $(wildcard Bio1/prog2/$(1).C Bio1/prog/$(1).C))
stagedef = -DLANGUAGE_C $(if $(filter stage%,$(1)),-DSTAGE=$(patsubst stage%,%,$(1)))
define UNIT_RULE
$(OBJ_FWD)/$(2).obj: $(call srcpath,$(call srcname,$(1),$(2))) $(HEADERS) makefile | $(SDKINC_H)
	$(CC) $(if $(filter main,$(1)),$(CCFLAGS_main),$(CCFLAGS_ovl) -s$(call uc,$(1))) $(call stagedef,$(1)) $(INCOPT) -w$(STAGE)\cc\$(1) $(subst /,\,$(call srcpath,$(call srcname,$(1),$(2)))) -o$(OBJ_DIR)\$(2).obj
endef
$(foreach u,$(MAIN_UNITS),$(eval $(call UNIT_RULE,main,$(u))))
$(foreach p,$(OVERLAYS),$(foreach u,$($(p)_UNITS),$(eval $(call UNIT_RULE,$(p),$(u)))))

## One PSYLINK run links all 13 programs (makefile.link): the main program into the CPE, each overlay into an image of its own.
## First every overlay's objects get the overlay's name as the prefix of their global names, together with the SDK startup object
## NONE2.OBJ (tools\ovlprefix.exe; see makefile.link for why). The copies are intermediates and live in tmp\build; OBJ keeps the
## compiled objects. $(1) is an overlay folder; the prefixed copy of NONE2.OBJ is the rule's target, the objects come with it.
OVL_DIR		=$(STAGE)\ovl
OVL_FWD		=tmp/build/psx-usa/ovl
LINK_DIR	=$(STAGE)\link
LINK_FWD	=tmp/build/psx-usa/link
define OVERLAY_RULE
$(OVL_FWD)/$(call uc,$(1))/NONE2.OBJ: $(foreach u,$($(1)_UNITS),$(OBJ_FWD)/$(u).obj) tools/ovlprefix.exe makefile
	tools\ovlprefix.exe $(call uc,$(1)) $(OVL_DIR)\$(call uc,$(1)) $(foreach u,$($(1)_UNITS),$(OBJ_DIR)\$(u).obj) --plain $(PSYQ_3_5)\PSX\LIB\NONE2.OBJ
endef
$(foreach p,$(OVERLAYS),$(eval $(call OVERLAY_RULE,$(p))))

## The link. gen_linkscript.exe fills in the placeholders of makefile.link (SDK paths from tools\sdkroot.ini, and OBJ, OVL, LINK from here).
## PSYLINK writes the main program to the CPE and each overlay's group to $(LINK_DIR)\<OVERLAY>.BIN; the old .CPE is removed first so a
## failed link can never leave a stale executable. PSYLINK must be the literal first word of its recipe line (see the top of this file).
LIBDEPS		=$(CUSTOM_SDK)/LIBPAD.LIB $(CUSTOM_SDK)/LIBGS.LIB tools/gen_linkscript.exe tools/cpe2exe.exe
LINKED		=$(LINK_FWD)/SLUS.CPE
$(LINKED): $(foreach u,$(MAIN_UNITS),$(OBJ_FWD)/$(u).obj) $(foreach p,$(OVERLAYS),$(OVL_FWD)/$(call uc,$(p))/NONE2.OBJ) $(LINKFILE) $(LIBDEPS) makefile
	tools\gen_linkscript.exe $(LINKFILE) $(LINK_DIR)\MAKEFILE.LNK OBJ=$(OBJ_DIR) OVL=$(OVL_DIR) LINK=$(LINK_DIR)
	if exist $(LINK_DIR)\SLUS.CPE del $(LINK_DIR)\SLUS.CPE
	$(LINK) $(LINKOPT) @$(LINK_DIR)\MAKEFILE.LNK,$(LINK_DIR)\SLUS.CPE,$(LINK_DIR)\SLUS.SYM,$(LINK_DIR)\SLUS.MAP

## the executables: SLUS_007.47 from the CPE, each PROG2 overlay from its image. The overlay's load address is that of its group, its
## entry point that of its startup symbol and the end of what is stored where its .sbss/.bss begin, all from the link map.
$(OBJ_FWD)/SLUS_007.47: $(LINKED)
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CPE) $(LINK_DIR)\SLUS.CPE $(OBJ_DIR)\SLUS_007.47
define OVERLAY_EXE_RULE
$(OBJ_FWD)/PROG2/$(call uc,$(1)).EXE: $(LINKED)
	if not exist $(OBJ_DIR)\PROG2 mkdir $(OBJ_DIR)\PROG2
	$(CPE) --bin $(LINK_DIR)\$(call uc,$(1)).BIN $(OBJ_DIR)\PROG2\$(call uc,$(1)).EXE --group $(call uc,$(1)) --bss $(call uc,$(1)) --pc-symbol $(call uc,$(1))___SN_ENTRY_POINT --map $(LINK_DIR)\SLUS.MAP
endef
$(foreach p,$(OVERLAYS),$(eval $(call OVERLAY_EXE_RULE,$(p))))

OUTPUTS		=$(OBJ_FWD)/SLUS_007.47 $(foreach p,$(OVERLAYS),$(OBJ_FWD)/PROG2/$(call uc,$(p)).EXE)

## the retail check runs last: it compares all 13 executables with the pinned retail SHA-256 values
build: $(OUTPUTS)
	tools\retailcheck.exe --quiet

## Builds the custom PSY-Q libraries into $(CUSTOM_SDK), once -- run this via setup.bat, not as part of a normal build.
## LIBPAD: PsyQ 4.2's library with PADMAIN and PADIF replaced in place by Bio1\lib\PADMAIN.S / PADIF.S (assembled here with
## ASPSX -G0; the .S files are LF, ASPSX needs CRLF, so tools\lf2crlf.exe makes a build-only copy first) and PsyQ 4.3's WAITRC2 moved
## to the end. LIBGS: PsyQ 3.7's library with GS_131, GS_133 and GS_134 replaced in place by the PsyQ 4.0 members.
## tools\sdklibbuild.exe composes both archives.
sdklib:
	if not exist $(CUSTOM_SDK) mkdir $(CUSTOM_SDK)
	if not exist $(STAGE)\sdk mkdir $(STAGE)\sdk
	tools\lf2crlf.exe Bio1\lib\PADMAIN.S $(STAGE)\sdk\padmain.s
	tools\lf2crlf.exe Bio1\lib\PADIF.S $(STAGE)\sdk\padif.s
## ASPSX exits with 2 on warnings (PADMAIN.S has two delay-slot warnings) but still writes the object, so the leading "-" ignores the exit code;
## sdklibbuild fails if an object is missing.
	if exist $(STAGE)\sdk\padmain.obj del $(STAGE)\sdk\padmain.obj
	if exist $(STAGE)\sdk\padif.obj del $(STAGE)\sdk\padif.obj
	-cd $(STAGE)\sdk&& aspsx -q -G0 padmain.s -o padmain.obj
	-cd $(STAGE)\sdk&& aspsx -q -G0 padif.s -o padif.obj
	tools\sdklibbuild.exe $(CUSTOM_SDK) $(STAGE)\sdk\padmain.obj $(STAGE)\sdk\padif.obj

check:
	tools\retailcheck.exe

rebuild:
	$(MAKE) clean
	$(MAKE) all

clean:
	if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	if exist $(STAGE) rmdir /s /q $(STAGE)

.PHONY: all build sdklib check rebuild clean
