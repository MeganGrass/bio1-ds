# PADMAIN.S - PSY-Q LIBPAD member PADMAIN, later-build revision, as ASPSX assembly source
#
# WHAT IT IS
#   The core of the pad library: the vsync interrupt handler that polls both pad ports over the SIO0
#   serial channel, the calls that enable, start and stop it, and the byte-exchange routines the
#   protocol state machine (the padIntFunc table of the PADIF member) is built on.  In link order:
#
#       PadEnableCom       public   switch channel 0 / channel 1 on or off
#       _padSetVsyncParam  public   fill in the interrupt record
#       _padIsVsync        static   interrupt verifier: is a vsync pending?
#       _padIntPad         static   the interrupt handler
#       _padChkVsync       public   read and clear "the handler ran"
#       _padStartCom       public   install the handler (PadStartCom in later libraries)
#       _padStopCom        public   remove it (PadStopCom)
#       _padInitSioMode    public   program SIO0 for a port, deliver pending results
#       _padPortStep       static   one step of the protocol state machine
#       _padSioRW          public   exchange one byte, kernel-style time-out
#       _padSioRW2         public   exchange one byte, timer-2 polling time-out
#       _padClrIntSio0     public   acknowledge the SIO0 interrupt, wait for /ACK to go idle
#       _padWaitRXready    public   wait for a received byte
#
#   then the library state: .data (revision stamp, the hook pointers the init modules fill in, the
#   channel and state variables, two register pointers) and .bss (the interrupt record, two counters).
#   The names are the library's own where a later PSY-Q library exports them (4.6 PDRESRES: _padIntPad,
#   _padIsVsync, _padVsyncParam, _padRestPeriod, _padCalledIntPad); _padPortStep, _padIStat and _padSio0
#   are ours (the originals are not exported).
#
# SIZE AND ORIGIN
#   .text  760 words (3040 bytes), between PADENTRY and LIBAPI's SysEnqIntRP
#   .data  0x80 bytes     (the first 8 bytes are the revision stamp)
#   .bss   0x20 bytes
#   The member is the PSY-Q 4.2 PADMAIN (756 words) in a later build of the same lineage.  Against
#   the stock 4.2 member 14 of its 760 words differ, none of them relocated:
#       _padSioRW    5 words   builds the timer-2 counter address (0x1F801120) once, in $2, in front
#                              of the branch that selects the baud divisor, and reads the counter
#                              through it                                                        +1 word
#       _padSioRW2   8 words   masks the counter halfword into a second register (andi 0xFFFF) and
#                              reads the timer-2 target register twice, as WAITRC2's chkRC2wait
#                              does; three branch offsets move with it                           +2 words
#       final nop    1 word    padding: every section of the original object is a multiple of
#                              16 bytes (text 3040, data 0x80, bss 0x20)                         +1 word
#   No C source reproduces these bytes with the project's compiler (CC1PSX 4.0 + ASPSX 2.56 neither
#   schedules the delay slots nor splits address macros around them the way the original library
#   was assembled), which is why this member is assembly.
#
# HOW IT IS BUILT
#   This file is assembled with ASPSX -q -G0 (exit 2 = warnings only) and put into
#   OBJ\PSX\SDKLIB\LIBPAD.LIB in place of the stock member, at its archive position.  The archive
#   keeps the stock member's export list, so every symbol it names is defined below with .globl:
#   10 functions and the 23 data symbols from _padFuncNextPort to _padFixResult.
# SOURCE CONVENTIONS (ASPSX 2.56 dialect)
#   * Registers are numeric ($0 zero, $1 at, $2-$3 v0-v1, $4-$7 a0-a3, $8-$15 t0-t7, $16-$23 s0-s7,
#     $24-$25 t8-t9, $31 ra); ASPSX knows no other names.  Each routine says what its registers hold.
#   * .set noreorder: ASPSX does not fill delay slots, so every slot is written out, and the
#     instruction in a slot is indented two extra spaces.  Load delays are written out as nops.
#   * "lw $3,sym" is the ASPSX address macro  lui $3,hi(sym) / lw $3,lo(sym)($3);  "sw $2,sym" is
#     lui $1,hi / sw $2,lo($1);  "la $3,sym" is lui / addiu;  "lw $2,sym($2)" is lui $1 / addu $1,$1,$2
#     / lw $2,lo($1).  The object carries a HI16/LO16 relocation pair for each, resolved by the linker.
#   * Five address pairs are split around a branch or call: the lui in front, the store (or
#     addiu) in the delay slot.  ASPSX 2.56 has no %hi/%lo and cannot put a symbolic displacement in a
#     lone load or store, so there the lui and the addiu use the arithmetic form of %hi/%lo
#         HI(x) = ((x+0x8000)>>16)        LO(x) = ((((x)&0xffff)^0x8000)-0x8000)
#     (the same rewrite tools\ccps.pl applies to compiler output) and a store is a .word whose low
#     16 bits the linker fills with the low half of the symbol:  0xac220000+((x)&0xffff)  is
#     "sw $2,lo(x)($1)".  The spelled-out instruction is in the comment.  These stretches sit in
#     .set noat ... .set at.  Because ASPSX does not count a .word as filling a delay slot it warns
#     "Multi-word instruction cannot fit into delay slot" at the two macros that follow such a
#     .word (a la and a lw); the object is not affected.
#   * Local labels are not symbols in the object; only the .globl names are exported.
#   * Lines end in CR LF (ASPSX rejects a bare LF).
#
# HARDWARE USED
#   SIO0     0x1F801040 +0 DATA (byte)  +4 STAT (half)  +8 MODE  +0xA CTRL  +0xE BAUD
#            STAT bit 0 TX ready, bit 1 RX ready, bit 7 /ACK level, bit 9 interrupt
#            CTRL bit 4 acknowledge, bit 6 reset, bit 13 port select (0x1003 port 1, 0x3003 port 2)
#   I_STAT   0x1F801070, I_MASK 0x1F801074: bit 0 vsync, bit 7 SIO0 (a bit is cleared by writing 0)
#   timer 2  0x1F801120 counter (16 bit), +4 mode (bit 9: system clock / 8), +8 target
#   setRC2wait(n) stores _waitTime = n and _startTime = the timer-2 counter; chkRC2wait() is non-zero
#   once the elapsed time (in /8 units) reaches _waitTime (WAITRC2).
#
# PORT RECORD (PADPORT, 0xF0 bytes, two of them from _padInfoDir; the fields touched here)
#   +0x0C  base    pointer _padInitSioMode addresses record n from (base + n * 0xF0)
#   +0x36  flag    byte, see _padInitSioMode
#   +0x3C  rx      receive buffer (bytes read back from the pad)
#   +0x40  tx      transmit buffer
#   +0x44  rxPos   next index into rx
#   +0x45  rxCnt   number of bytes exchanged
#   +0x50  flag    byte, see _padInitSioMode
#   +0xE8  type    byte, 8 selects the shorter time-out in _padInitSioMode
#
# CONTENT
#   * the assembled object is 760 words (3040 bytes) and defines the stock member's 33 exports; apart
#     from relocated fields the 14 words listed above are the only ones that differ from the stock member
#   * 122 relocated sites (28 jal/j, 94 hi/lo pairs); the 0x80-byte .data block carries the revision
#     stamp 50 73 0c 22 8b 99 42 10 and .bss is 0x20 bytes
#
	.text
	.align  2
	.set    noreorder

#---------------------------------------------------------------------------------------------------
# PadEnableCom  (70 words)
#
# unsigned long PadEnableCom(unsigned long mode);
#     mode bit 0: channel 0 (port 1), bit 1: channel 1 (port 2); returns the previous mode.
#
# The interrupt handler polls the channels _padChanStart .. _padChanStop, so the state is kept as
# "channel 0 is on when _padChanStart == 0" and "channel 1 is on when _padChanStop == 1"; the empty
# range 1..0 means both are off.  A channel that is off has its _padRestPeriod[] counter running (the
# handler counts vsyncs up to 150); a channel that comes back after 150 or more vsyncs has its port
# record cleared first (_padFuncClrInfo), so the pad is detected afresh.  _padIntExec is held at 0
# while the state changes.
#
# $16 = mode   $17 = previous mode (return value)   $18, later $16 = &_padRestPeriod[0], [1]
#---------------------------------------------------------------------------------------------------
	.globl  PadEnableCom
PadEnableCom:
	lw      $3,_padChanStop
	lw      $2,_padChanStart
	addiu   $sp,$sp,-32
	sw      $16,16($sp)
	move    $16,$4
	sw      $31,28($sp)
	sw      $18,24($sp)
	sw      $17,20($sp)
	# previous mode = (_padChanStop << 1) | (_padChanStart == 0); nothing to do if it equals mode
	sll     $3,$3,1
	sltiu   $2,$2,1
	or      $17,$3,$2
	beq     $17,$16,PadEnableCom_ret
	  andi  $2,$16,0x1
	# hold the interrupt handler off while the channels change:  _padIntExec = 0
	sw      $0,_padIntExec
	# channel 0, mode bit 0 ($2 = mode & 1 from the delay slot above)
	beq     $2,$0,PadEnableCom_ch0_off
	  li    $2,1
	# on:  _padChanStart = 0;  if (_padRestPeriod[0] >= 150) _padFuncClrInfo(_padInfoDir);
	#      _padRestPeriod[0] = 0 (delay slot of the j below)
	la      $18,_padRestPeriod
	lw      $2,0($18)
	sw      $0,_padChanStart
	slti    $2,$2,150
	bne     $2,$0,PadEnableCom_ch0_done
	  nop
	lw      $4,_padInfoDir
	lw      $2,_padFuncClrInfo
	nop
	jalr    $2
	  nop
	# _padRestPeriod[0] = 0  (delay slot of the j: reached with and without the clear call)
PadEnableCom_ch0_done:
	j       PadEnableCom_ch1
	  sw    $0,0($18)
	# off: _padChanStart = 1 ($2 is still 1 from the delay slot of the beq)
PadEnableCom_ch0_off:
	sw      $2,_padChanStart
	# channel 1, mode bit 1: the same, with _padChanStop = 1 / 0 and the second port record (+0xF0)
PadEnableCom_ch1:
	andi    $2,$16,0x2
	beq     $2,$0,PadEnableCom_ch1_off
	  li    $3,1
	la      $16,_padRestPeriod+4
	lw      $2,0($16)
	sw      $3,_padChanStop
	slti    $2,$2,150
	bne     $2,$0,PadEnableCom_ch1_done
	  nop
	lw      $4,_padInfoDir
	lw      $2,_padFuncClrInfo
	nop
	jalr    $2
	  addiu $4,$4,240
PadEnableCom_ch1_done:
	j       PadEnableCom_run
	  sw    $0,0($16)
PadEnableCom_ch1_off:
	sw      $0,_padChanStop
	# handler enabled again:  _padIntExec = 1
PadEnableCom_run:
	li      $2,1
	sw      $2,_padIntExec
	# return the previous mode
PadEnableCom_ret:
	move    $2,$17
	lw      $31,28($sp)
	lw      $18,24($sp)
	lw      $17,20($sp)
	lw      $16,16($sp)
	jr      $31
	  addiu $sp,$sp,32

#---------------------------------------------------------------------------------------------------
# _padSetVsyncParam  (11 words)
#
# void _padSetVsyncParam(void);
#     Fills in _padVsyncParam, the 4-word record SysEnqIntRP() queues:
#         +0 next = 0     +4 handler = _padIntPad     +8 verifier = _padIsVsync     +12 pad = 0
#
# $2 = &_padVsyncParam + 4: the compiler kept that one address and stores at -4, 0, 4 and 8 from it.
#---------------------------------------------------------------------------------------------------
	.globl  _padSetVsyncParam
_padSetVsyncParam:
	la      $2,_padVsyncParam+4
	la      $3,_padIntPad
	sw      $3,0($2)
	la      $3,_padIsVsync
	sw      $3,4($2)
	sw      $0,-4($2)
	jr      $31
	  sw    $0,8($2)

#---------------------------------------------------------------------------------------------------
# _padIsVsync  (static; 26 words)
#
# long _padIsVsync(void);        the record's verifier, called by the kernel before the handler
#     Returns 1 when a vsync interrupt is both enabled (I_MASK bit 0) and pending (I_STAT bit 0), after
#     calling the light-gun hook _padFuncIntGun if one is installed; otherwise 0.
#
# $3 = _padIStat (&I_STAT; I_MASK is at +4)
#---------------------------------------------------------------------------------------------------
_padIsVsync:
	lw      $3,_padIStat
	addiu   $sp,$sp,-24
	sw      $31,16($sp)
	# if (!(I_MASK & 1)) return 0;
	lw      $2,4($3)                # I_MASK
	nop
	andi    $2,$2,0x1               # vsync enabled?
	beq     $2,$0,_padIsVsync_ret
	  move  $2,$0
	# if (!(I_STAT & 1)) return 0;
	lw      $2,0($3)                # I_STAT
	nop
	andi    $2,$2,0x1               # vsync pending?
	beq     $2,$0,_padIsVsync_ret
	  move  $2,$0
	# if (_padFuncIntGun != 0) _padFuncIntGun();  return 1;
	lw      $2,_padFuncIntGun
	nop
	beq     $2,$0,_padIsVsync_yes
	  nop
	jalr    $2
	  nop
_padIsVsync_yes:
	li      $2,1
_padIsVsync_ret:
	lw      $31,16($sp)
	addiu   $sp,$sp,24
	jr      $31
	  nop

#---------------------------------------------------------------------------------------------------
# _padIntPad  (static; 91 words)
#
# long _padIntPad(void);         the vsync interrupt handler, always returns 0
#     Once per vsync it
#       - sets _padCalledIntPad (read and cleared by _padChkVsync);
#       - counts _padRestPeriod[ch] up to 150 for each channel that is off;
#       - when _padIntExec is set and at least one channel is on: initialises SIO0 for the first
#         channel (_padInitSioMode; if that returns 0 it calls _padFuncNextPort(0xFFFF)), clears
#         _padTotalCurr, runs _padPortStep for every channel from _padChanStart to _padChanStop
#         and sets the SIO0 baud register back to 0x88.
#
# $3 = _padChanStart, later _padSioChan   $2 = _padChanStop   $4 = rest counter / port record
#---------------------------------------------------------------------------------------------------
_padIntPad:
	addiu   $sp,$sp,-24
	lw      $3,_padChanStart
	li      $2,1
	sw      $31,16($sp)
	# _padCalledIntPad = 1;  (the store is in the delay slot of the beq: it always runs)
	# if (_padChanStart != 0)  /* channel 0 is off */  if (_padRestPeriod[0] < 150) _padRestPeriod[0]++;
	.set    noat
	lui     $1,((_padCalledIntPad+0x8000)>>16)
	beq     $3,$0,_padIntPad_ch1
	  .word 0xac220000+((_padCalledIntPad)&0xffff)  # sw $2,lo(_padCalledIntPad)($1)
	.set    at
	la      $4,_padRestPeriod
	lw      $3,0($4)
	nop
	slti    $2,$3,150
	beq     $2,$0,_padIntPad_ch1
	  addiu $2,$3,1
	sw      $2,0($4)
	# if (_padChanStop == 0)  /* channel 1 is off */  if (_padRestPeriod[1] < 150) _padRestPeriod[1]++;
_padIntPad_ch1:
	lw      $2,_padChanStop
	nop
	bne     $2,$0,_padIntPad_poll
	  nop
	la      $4,_padRestPeriod+4
	lw      $3,0($4)
	nop
	slti    $2,$3,150
	beq     $2,$0,_padIntPad_poll
	  addiu $2,$3,1
	sw      $2,0($4)
	# if (_padIntExec == 0) return 0;     if (_padChanStop < _padChanStart) return 0;   /* both off */
_padIntPad_poll:
	lw      $2,_padIntExec
	nop
	beq     $2,$0,_padIntPad_ret
	  nop
	lw      $3,_padChanStart
	lw      $2,_padChanStop
	nop
	slt     $2,$2,$3
	bne     $2,$0,_padIntPad_ret
	  sll   $4,$3,4
	# $4 = _padChanStart * 0xF0 (n * 15 * 16; the sll is the delay slot of the bne above)
	subu    $4,$4,$3
	lw      $2,_padInfoDir
	sll     $4,$4,4
	# _padSioState = 0;  _padSioChan = _padChanStart;
	# if (_padInitSioMode(_padInfoDir + _padSioChan) == 0) _padFuncNextPort(0xFFFF);
	sw      $0,_padSioState
	sw      $3,_padSioChan
	jal     _padInitSioMode
	  addu  $4,$2,$4
	bne     $2,$0,_padIntPad_range
	  nop
	lw      $2,_padFuncNextPort
	nop
	jalr    $2
	  li    $4,0xffff
	# _padTotalCurr = 0;  while the channel is within range:
_padIntPad_range:
	lw      $3,_padSioChan
	lw      $2,_padChanStop
	sw      $0,_padTotalCurr
	slt     $2,$2,$3
	bne     $2,$0,_padIntPad_baud
	  nop
	# do { _padPortStep(_padInfoDir + _padSioChan); } while (_padSioChan <= _padChanStop);
	# (_padPortStep moves _padSioChan on)
_padIntPad_step:
	sll     $4,$3,4
	subu    $4,$4,$3
	lw      $2,_padInfoDir
	sll     $4,$4,4
	jal     _padPortStep
	  addu  $4,$2,$4
	lw      $3,_padSioChan
	lw      $2,_padChanStop
	nop
	slt     $2,$2,$3
	beq     $2,$0,_padIntPad_step
	  nop
	# SIO0 baud = 0x88
_padIntPad_baud:
	lw      $3,_padSio0
	li      $2,136          # BAUD value 0x88
	sh      $2,14($3)               # SIO0 BAUD
	# return 0
_padIntPad_ret:
	lw      $31,16($sp)
	move    $2,$0
	jr      $31
	  addiu $sp,$sp,24

#---------------------------------------------------------------------------------------------------
# _padChkVsync  (5 words)
#
# long _padChkVsync(void);       returns _padCalledIntPad and clears it
#     (the clearing store is the delay slot of the jr)
#---------------------------------------------------------------------------------------------------
	.globl  _padChkVsync
_padChkVsync:
	lw      $2,_padCalledIntPad
	.set    noat
	lui     $1,((_padCalledIntPad+0x8000)>>16)
	jr      $31
	  .word 0xac200000+((_padCalledIntPad)&0xffff)  # sw $0,lo(_padCalledIntPad)($1)
	.set    at

#---------------------------------------------------------------------------------------------------
# _padStartCom  (51 words)
#
# void _padStartCom(void);
#     Installs the vsync handler.  _padIntExec = 0, then inside a critical section: the record is
#     dequeued and queued again at priority 2 (so it is never queued twice), a pending vsync is
#     acknowledged (I_STAT = ~1), vsync is enabled in I_MASK and ChangeClearRCnt(3, 0) is called
#     (_padStopCom undoes it with ChangeClearRCnt(3, 1)).  Then both port records are cleared
#     (_padFuncClrInfo), both rest counters zeroed and _padIntExec set to 1.
#
# $16 = &_padVsyncParam   $3 = _padIStat
#---------------------------------------------------------------------------------------------------
	.globl  _padStartCom
_padStartCom:
	addiu   $sp,$sp,-24
	sw      $31,20($sp)
	sw      $16,16($sp)
	# _padIntExec = 0  (delay slot of the call)
	.set    noat
	lui     $1,((_padIntExec+0x8000)>>16)
	jal     EnterCriticalSection
	  .word 0xac200000+((_padIntExec)&0xffff)       # sw $0,lo(_padIntExec)($1)
	.set    at
	# SysDeqIntRP(2, &_padVsyncParam);  SysEnqIntRP(2, &_padVsyncParam);
	li      $4,2
	la      $16,_padVsyncParam
	jal     SysDeqIntRP
	  move  $5,$16
	li      $4,2
	jal     SysEnqIntRP
	  move  $5,$16
	# I_STAT = ~1;  I_MASK |= 1 (delay slot of the call);  ChangeClearRCnt(3, 0);
	li      $4,3
	lw      $3,_padIStat
	li      $2,-2           # ~1
	sw      $2,0($3)                # I_STAT = ~1: acknowledge the vsync
	lw      $2,4($3)                # I_MASK
	move    $5,$0
	ori     $2,$2,0x1
	jal     ChangeClearRCnt
	  sw    $2,4($3)                # I_MASK |= 1: enable vsync
	# ExitCriticalSection();
	jal     ExitCriticalSection
	  nop
	# _padFuncClrInfo(_padInfoDir);  _padFuncClrInfo(_padInfoDir + 1);   /* + 0xF0 bytes */
	lw      $4,_padInfoDir
	lw      $2,_padFuncClrInfo
	nop
	jalr    $2
	  nop
	lw      $4,_padInfoDir
	lw      $2,_padFuncClrInfo
	nop
	jalr    $2
	  addiu $4,$4,240
	# _padRestPeriod[1] = _padRestPeriod[0] = 0;   _padIntExec = 1;
	la      $2,_padRestPeriod
	sw      $0,4($2)
	sw      $0,0($2)
	li      $2,1
	sw      $2,_padIntExec
	lw      $31,20($sp)
	lw      $16,16($sp)
	jr      $31
	  addiu $sp,$sp,24

#---------------------------------------------------------------------------------------------------
# _padStopCom  (17 words)
#
# void _padStopCom(void);
#     EnterCriticalSection(); ChangeClearRCnt(3, 1); SysDeqIntRP(2, &_padVsyncParam);
#     ExitCriticalSection();
#---------------------------------------------------------------------------------------------------
	.globl  _padStopCom
_padStopCom:
	addiu   $sp,$sp,-24
	sw      $31,16($sp)
	jal     EnterCriticalSection
	  nop
	li      $4,3
	jal     ChangeClearRCnt
	  li    $5,1
	la      $5,_padVsyncParam
	jal     SysDeqIntRP
	  li    $4,2
	jal     ExitCriticalSection
	  nop
	lw      $31,16($sp)
	addiu   $sp,$sp,24
	jr      $31
	  nop

#---------------------------------------------------------------------------------------------------
# _padInitSioMode  (133 words)
#
# long long _padInitSioMode(PADPORT *port);       $16 = port
#     Programs SIO0 for the channel in _padSioChan: CTRL = 0x40 (reset), CTRL = 0, MODE = 0x0D (8 bit,
#     x1), BAUD = 0x88, setRC2wait(80) for a port of type 8 and setRC2wait(145) for any other, CTRL =
#     0x1003 for channel 0 or 0x3003 for channel 1.  Then
#       - a pending _padFixResult[chan] >= 0 is delivered first: while it is above 0 it is counted
#         down and _padFuncRecvAuto is called for the record base + n * 0xF0; when it reaches 0 it
#         is set to -1 and _padFuncRecvAuto(port) and _padFuncClrCmdNo(port) are called;
#       - if SIO0 shows an interrupt (STAT bit 9): CTRL bit 4 acknowledges it; if it is still there a
#         1 is sent and, once a byte has arrived, the function returns v0 = 0, v1 = that byte;
#         if not, I_STAT bit 7 is cleared and the flags below are tested;
#       - otherwise, when either flag byte of the port (+0x50, +0x36) is zero the first receive byte is
#         cleared and 1 is returned, else 0.
#
# $16 = port   $17 = &_padFixResult   $3 = _padSio0
#---------------------------------------------------------------------------------------------------
	.globl  _padInitSioMode
_padInitSioMode:
	addiu   $sp,$sp,-32
	sw      $16,16($sp)
	move    $16,$4
	# SIO0: CTRL = 0x40 (reset);  CTRL = 0;  MODE = 0x0D;  BAUD = 0x88;
	lw      $3,_padSio0
	li      $2,64           # CTRL reset bit
	sw      $31,24($sp)
	sw      $17,20($sp)
	sh      $2,10($3)               # SIO0 CTRL
	li      $2,13           # MODE 0x0D
	sh      $0,10($3)               # SIO0 CTRL
	sh      $2,8($3)                # SIO0 MODE
	li      $2,136          # BAUD 0x88
	sh      $2,14($3)               # SIO0 BAUD
	# setRC2wait(port->type == 8 ? 80 : 145);
	lbu     $3,232($16)
	li      $2,8
	bne     $3,$2,_padInitSioMode_rc2
	  li    $4,145
	li      $4,80
_padInitSioMode_rc2:
	jal     setRC2wait
	  nop
	# CTRL = _padSioChan ? 0x3003 : 0x1003;
	lw      $2,_padSioChan
	lw      $4,_padSio0
	beq     $2,$0,_padInitSioMode_ctrl
	  li    $3,4099         # CTRL for channel 0: 0x1003
	li      $3,12291                # CTRL for channel 1: 0x3003
_padInitSioMode_ctrl:
	sll     $2,$2,2
	sh      $3,10($4)               # SIO0 CTRL
	# n = _padFixResult[_padSioChan];  if (n < 0) goto irq;  ($3 = &_padFixResult in the delay slot)
	lw      $2,_padFixResult($2)
	lui     $3,((_padFixResult+0x8000)>>16)
	bltz    $2,_padInitSioMode_irq
	  addiu $3,$3,((((_padFixResult)&0xffff)^0x8000)-0x8000)
	# if (n > 0) do { n = --_padFixResult[chan];  _padFuncRecvAuto(port->base + n * 0xF0); }
	#            while (_padFixResult[chan] > 0);
	blez    $2,_padInitSioMode_fixed
	  nop
	move    $17,$3
_padInitSioMode_fix:
	lw      $3,_padSioChan
	nop
	sll     $3,$3,2
	addu    $3,$3,$17
	lw      $2,0($3)
	nop
	addiu   $2,$2,-1
	sll     $4,$2,4
	subu    $4,$4,$2
	sll     $4,$4,4
	sw      $2,0($3)
	lw      $2,12($16)
	lw      $3,_padFuncRecvAuto
	nop
	jalr    $3
	  addu  $4,$2,$4
	lw      $2,_padSioChan
	nop
	sll     $2,$2,2
	addu    $2,$2,$17
	lw      $2,0($2)
	nop
	bgtz    $2,_padInitSioMode_fix
	  nop
	# if (_padFixResult[chan] == 0) { _padFixResult[chan] = -1; _padFuncRecvAuto(port); _padFuncClrCmdNo(port); }
_padInitSioMode_fixed:
	lw      $2,_padSioChan
	la      $3,_padFixResult
	sll     $2,$2,2
	addu    $5,$2,$3
	lw      $2,0($5)
	nop
	bne     $2,$0,_padInitSioMode_irq
	  li    $3,-1
	move    $4,$16
	lw      $2,_padFuncRecvAuto
	nop
	jalr    $2
	  sw    $3,0($5)
	lw      $2,_padFuncClrCmdNo
	nop
	jalr    $2
	  move  $4,$16
	# if (STAT & 0x200) {                       /* SIO0 interrupt pending */
	#     CTRL |= 0x10;                          /* acknowledge */
	#     if (STAT & 0x200) { DATA = 1;  _padWaitRXready();  return (v0 = 0, v1 = DATA); }
	#     I_STAT = ~0x80;
	# }
_padInitSioMode_irq:
	lw      $3,_padSio0
	nop
	lhu     $2,4($3)                # SIO0 STAT
	nop
	andi    $2,$2,0x200             # STAT bit 9: interrupt
	beq     $2,$0,_padInitSioMode_flags
	  nop
	lhu     $2,10($3)               # SIO0 CTRL
	nop
	ori     $2,$2,0x10              # CTRL bit 4: acknowledge
	sh      $2,10($3)               # SIO0 CTRL
	lhu     $2,4($3)                # SIO0 STAT
	nop
	andi    $2,$2,0x200             # STAT bit 9 again
	beq     $2,$0,_padInitSioMode_noirq
	  li    $2,1
	sb      $2,0($3)                # DATA = 1
	jal     _padWaitRXready
	  nop
	lw      $3,_padSio0
	move    $2,$0
	lbu     $3,0($3)                # v1 = DATA
	j       _padInitSioMode_ret
	  nop
_padInitSioMode_noirq:
	lw      $3,_padIStat
	li      $2,-129
	sw      $2,0($3)                # I_STAT = ~0x80: clear the SIO0 interrupt
	# if (port->flag50 == 0 || port->flag36 == 0) { *port->rx = 0; return 1; }   return 0;
_padInitSioMode_flags:
	lbu     $2,80($16)
	nop
	beq     $2,$0,_padInitSioMode_clear
	  nop
	lbu     $2,54($16)
	nop
	bne     $2,$0,_padInitSioMode_ret
	  move  $2,$0
_padInitSioMode_clear:
	lw      $3,60($16)
	li      $2,1
	sb      $0,0($3)
_padInitSioMode_ret:
	lw      $31,24($sp)
	lw      $17,20($sp)
	lw      $16,16($sp)
	jr      $31
	  addiu $sp,$sp,32

#---------------------------------------------------------------------------------------------------
# _padPortStep  (static; 50 words)
#
# void _padPortStep(PADPORT *port);        $4 = port (passed on unchanged to the state handler)
#     One step of the protocol state machine of a port: r = padIntFunc[_padSioState++](port) (the
#     state handlers of PADIF).  A negative r goes to _padFuncNextPort(r).  Otherwise, when
#     _padSioState is not 0, the time-out is armed (setRC2wait(60)) and _padClrIntSio0() must
#     succeed or _padFuncNextPort(-3) is called.  A state of 5 or more is stepped back by one.
#
# $5 = &_padSioState   $3 = state   $4 = r
#---------------------------------------------------------------------------------------------------
_padPortStep:
	addiu   $sp,$sp,-24
	la      $5,_padSioState
	sw      $31,16($sp)
	lw      $3,0($5)
	nop
	sll     $2,$3,2
	lw      $2,padIntFunc($2)
	addiu   $3,$3,1
	jalr    $2
	  sw    $3,0($5)
	move    $4,$2
	# if (r < 0) goto error;
	bltz    $4,_padPortStep_error
	  nop
	# if (_padSioState != 0) { setRC2wait(60);  if (_padClrIntSio0() == 0) _padFuncNextPort(-3); }
	lw      $2,_padSioState
	nop
	beq     $2,$0,_padPortStep_wrap
	  nop
	jal     setRC2wait
	  li    $4,60
	jal     _padClrIntSio0
	  nop
	bne     $2,$0,_padPortStep_wrap
	  nop
	lw      $2,_padFuncNextPort
	nop
	jalr    $2
	  li    $4,-3
	# if (_padSioState >= 5) _padSioState = _padSioState - 1;  (the addiu is in the delay slot)
_padPortStep_wrap:
	lw      $3,_padSioState
	nop
	slti    $2,$3,5
	bne     $2,$0,_padPortStep_ret
	  addiu $2,$3,-1
	.set    noat
	lui     $1,((_padSioState+0x8000)>>16)
	j       _padPortStep_ret
	  .word 0xac220000+((_padSioState)&0xffff)      # sw $2,lo(_padSioState)($1)
	.set    at
	# _padFuncNextPort(r)
_padPortStep_error:
	lw      $2,_padFuncNextPort
	nop
	jalr    $2
	  nop
_padPortStep_ret:
	lw      $31,16($sp)
	addiu   $sp,$sp,24
	jr      $31
	  nop

#---------------------------------------------------------------------------------------------------
# _padSioRW  (118 words)
#
# long _padSioRW(PADPORT *port, long data);        $16 = port   $18 = data   $17 = received byte
#     Exchanges one byte over SIO0 and returns the byte received.
#     data < 0: the byte ~data is sent without waiting for the interrupt: rxPos = 0xFF, rxCnt = 1,
#         *tx = ~data; the receive register is read first, then the function waits for TX ready
#         (STAT bit 0), then until chkRC2wait() reports that the delay armed by the caller has
#         elapsed, and writes ~data.
#     data >= 0: baud divisor $6 = 0x22 when the port's first received byte has the high nibble 8
#         and rxPos is 9 or more, else 0x88.  setRC2wait(430) is done inline (_startTime = timer-2
#         counter, _waitTime = 430).  After a byte has been received it is read, BAUD is set, and
#         the function waits for the SIO0 interrupt (I_STAT bit 7); chkRC2wait() != 0 meanwhile
#         gives up with -20.  Then data is sent, rxCnt++ and rx[rxPos++] = the received byte.
#
# $5 / $3 = _padSio0   $2 = 0x1F801120, the timer-2 counter
#---------------------------------------------------------------------------------------------------
	.globl  _padSioRW
_padSioRW:
	addiu   $sp,$sp,-32
	sw      $16,16($sp)
	move    $16,$4
	sw      $18,24($sp)
	move    $18,$5
	sw      $31,28($sp)
	# if (data >= 0) goto xfer;
	bgez    $18,_padSioRW_xfer
	  sw    $17,20($sp)
	# byte = DATA;  port->rxPos = 0xFF;  port->rxCnt = 1;  *port->tx = ~data;
	lw      $2,_padSio0
	lw      $3,64($16)
	lbu     $4,0($2)                # DATA
	li      $2,255
	sb      $2,68($16)
	li      $2,1
	sb      $2,69($16)
	nor     $2,$0,$18
	sb      $2,0($3)
	# wait for STAT bit 0 (TX ready);  while (chkRC2wait() == 0) ;   DATA = ~data;  return byte;
	lw      $3,_padSio0
	nop
	lhu     $2,4($3)                # SIO0 STAT
	nop
	andi    $2,$2,0x1               # STAT bit 0: TX ready
	bne     $2,$0,_padSioRW_rcwait
	  andi  $17,$4,0xff
_padSioRW_txwait:
	lhu     $2,4($3)                # SIO0 STAT
	nop
	andi    $2,$2,0x1               # STAT bit 0: TX ready
	beq     $2,$0,_padSioRW_txwait
	  nop
_padSioRW_rcwait:
	jal     chkRC2wait
	  nop
	beq     $2,$0,_padSioRW_rcwait
	  nor   $2,$0,$18
	lw      $3,_padSio0
	nop
	sb      $2,0($3)                # DATA = ~data
	j       _padSioRW_ret
	  move  $2,$17
	# $6 = (((*port->rx >> 4) == 8) && port->rxPos >= 9) ? 0x22 : 0x88;
_padSioRW_xfer:
	lw      $2,60($16)
	nop
	lbu     $2,0($2)
	li      $3,8
	sra     $2,$2,4
	bne     $2,$3,_padSioRW_std
	  li    $6,136
	lbu     $2,68($16)
	nop
	sltiu   $2,$2,9
	bne     $2,$0,_padSioRW_timer
	  lui   $2,0x1f80               # delay slot (4.2: nop); the fall-through path repeats it
	li      $6,34
_padSioRW_std:
	lui     $2,0x1f80
	# $2 = 0x1F801120;  _startTime = *(unsigned short *)$2;  _waitTime = 430;
_padSioRW_timer:
	ori     $2,$2,0x1120
	lw      $5,_padSio0
	lhu     $4,0($2)
	lhu     $3,4($5)                # SIO0 STAT
	li      $2,430
	sw      $2,_waitTime
	sw      $4,_startTime
	andi    $3,$3,0x2               # STAT bit 1: byte received
	bne     $3,$0,_padSioRW_rx
	  move  $3,$5
	# wait until STAT bit 1 (a byte has been received)
_padSioRW_rxwait:
	lhu     $2,4($3)                # SIO0 STAT
	nop
	andi    $2,$2,0x2               # STAT bit 1: byte received
	beq     $2,$0,_padSioRW_rxwait
	  nop
	# byte = DATA;  BAUD = $6;   wait for I_STAT bit 7, with chkRC2wait() as the time-out (-20)
_padSioRW_rx:
	lw      $2,_padSio0
	lw      $3,_padIStat
	lbu     $4,0($2)                # DATA
	sh      $6,14($2)               # SIO0 BAUD
	lw      $2,0($3)                # I_STAT
	nop
	andi    $2,$2,0x80              # I_STAT bit 7: SIO0
	bne     $2,$0,_padSioRW_tx
	  andi  $17,$4,0xff
_padSioRW_irqwait:
	jal     chkRC2wait
	  nop
	bne     $2,$0,_padSioRW_ret
	  li    $2,-20
	lw      $2,_padIStat
	nop
	lw      $2,0($2)                # I_STAT
	nop
	andi    $2,$2,0x80              # I_STAT bit 7: SIO0
	beq     $2,$0,_padSioRW_irqwait
	  nop
	# DATA = data;  port->rxCnt++;  port->rx[port->rxPos] = byte;  port->rxPos++;  return byte;
_padSioRW_tx:
	lw      $2,_padSio0
	nop
	sb      $18,0($2)               # DATA = data
	lbu     $2,69($16)
	lbu     $4,68($16)
	lw      $3,60($16)
	addiu   $2,$2,1
	addu    $3,$3,$4
	sb      $2,69($16)
	sb      $17,0($3)
	lbu     $2,68($16)
	nop
	addiu   $2,$2,1
	sb      $2,68($16)
	move    $2,$17
_padSioRW_ret:
	lw      $31,28($sp)
	lw      $18,24($sp)
	lw      $17,20($sp)
	lw      $16,16($sp)
	jr      $31
	  addiu $sp,$sp,32

#---------------------------------------------------------------------------------------------------
# _padSioRW2  (141 words)
#
# long _padSioRW2(PADPORT *port, long data);       $16 = port   $19 = data   $18 = byte   $17 = baud
#     Exchanges one byte like _padSioRW with data >= 0, but waits for the SIO0 interrupt with its own
#     timer-2 test instead of chkRC2wait(); returns the byte, or -2 on a time-out.
#       - $17 = 0x88, or 0x22 when (*port->rx >> 4) == 8 and rxPos >= 9;
#       - wait for STAT bit 1, setRC2wait(400), byte = DATA; BAUD = 0x22 when rxPos == 0 and the byte's
#         high nibble is 8, else $17;
#       - wait for I_STAT bit 7; meanwhile the timer-2 test (an inline chkRC2wait): elapsed =
#         counter - _startTime, plus the target (0x10000 when it is 0) if the counter wrapped; time
#         out when elapsed >= _waitTime if timer-2 mode bit 9 is set, else when elapsed / 8 >= _waitTime;
#       - a port that is not of type 8 in state 2 waits setRC2wait(60) / chkRC2wait() first;
#       - DATA = data;  rxCnt++;  rx[rxPos] = byte unless rxPos == 0xFF;  rxPos++.
#
# $3 = _padSio0, later the counter   $9 = 0x1F801120 (counter)   $6 = 0x1F801128 (target)
# $8 = 0x1F801124 (mode)   $10 = 0x10000   $4 = _startTime   $5 = _waitTime   $7 = &I_STAT
#---------------------------------------------------------------------------------------------------
	.globl  _padSioRW2
_padSioRW2:
	addiu   $sp,$sp,-40
	sw      $16,16($sp)
	move    $16,$4
	sw      $19,28($sp)
	move    $19,$5
	sw      $31,32($sp)
	sw      $18,24($sp)
	sw      $17,20($sp)
	# $17 = (((*port->rx >> 4) == 8) && port->rxPos >= 9) ? 0x22 : 0x88;
	lw      $2,60($16)
	nop
	lbu     $2,0($2)
	li      $3,8
	sra     $2,$2,4
	bne     $2,$3,_padSioRW2_rxwait0
	  li    $17,136
	lbu     $2,68($16)
	nop
	sltiu   $2,$2,9
	bne     $2,$0,_padSioRW2_rxwait0
	  nop
	li      $17,34
	# wait until STAT bit 1, then setRC2wait(400)
_padSioRW2_rxwait0:
	lw      $3,_padSio0
	nop
_padSioRW2_rxwait:
	lhu     $2,4($3)                # SIO0 STAT
	nop
	andi    $2,$2,0x2               # STAT bit 1: byte received
	beq     $2,$0,_padSioRW2_rxwait
	  nop
	jal     setRC2wait
	  li    $4,400
	# byte = DATA ($18);   BAUD = (rxPos == 0 && (byte >> 4) == 8) ? 0x22 : $17;
	lw      $4,_padSio0
	nop
	lbu     $2,0($4)                # DATA
	lbu     $3,68($16)
	nop
	bne     $3,$0,_padSioRW2_baud
	  andi  $18,$2,0xff
	sra     $3,$18,4
	li      $2,8
	beq     $3,$2,_padSioRW2_baud22
	  li    $2,34
_padSioRW2_baud:
	sh      $17,14($4)              # SIO0 BAUD
	j       _padSioRW2_irqwait
	  nop
_padSioRW2_baud22:
	sh      $2,14($4)               # SIO0 BAUD
	# if (I_STAT & 0x80) goto got;   else poll until the interrupt or the time-out
_padSioRW2_irqwait:
	lw      $3,_padIStat
	nop
	lw      $2,0($3)                # I_STAT
	nop
	andi    $2,$2,0x80              # I_STAT bit 7: SIO0
	bne     $2,$0,_padSioRW2_got
	  nop
	# the poll loop keeps the timer-2 register addresses in registers
	lui     $9,0x1f80
	ori     $9,$9,0x1120
	lui     $6,0x1f80
	ori     $6,$6,0x1128
	lui     $10,0x1
	lui     $8,0x1f80
	ori     $8,$8,0x1124
	move    $7,$3
	lw      $4,_startTime
	lw      $5,_waitTime
	# $3 = counter = *(unsigned short *)$9 & 0xFFFF;   if (counter < _startTime) counter += target ? target : 0x10000;
	# (the target register is read again before adding it; 4.2 read it once)
_padSioRW2_poll:
	lhu     $2,0($9)                # timer-2 counter
	nop
	andi    $3,$2,0xffff
	sltu    $2,$3,$4
	beq     $2,$0,_padSioRW2_mode
	  nop
	lhu     $2,0($6)                # timer-2 target
	nop
	beq     $2,$0,_padSioRW2_wrap
	  nop
	lhu     $2,0($6)                # timer-2 target, read again
	j       _padSioRW2_mode
	  addu  $3,$2,$3
_padSioRW2_wrap:
	addu    $3,$3,$10
	# if (timer-2 mode & 0x200) { if (counter - _startTime >= _waitTime) return -2; }
	# if ((counter - _startTime) / 8 >= _waitTime) return -2;       /* delay slot of the beq loads -2 */
	# if (!(I_STAT & 0x80)) goto poll;
_padSioRW2_mode:
	lhu     $2,0($8)                # timer-2 mode
	nop
	andi    $2,$2,0x200             # timer-2 mode bit 9: system clock / 8
	beq     $2,$0,_padSioRW2_div8
	  subu  $2,$3,$4
	sltu    $2,$2,$5
	beq     $2,$0,_padSioRW2_ret
	  li    $2,-2
_padSioRW2_div8:
	subu    $2,$3,$4
	srl     $2,$2,3
	sltu    $2,$2,$5
	beq     $2,$0,_padSioRW2_ret
	  li    $2,-2
	lw      $2,0($7)                # I_STAT
	nop
	andi    $2,$2,0x80              # I_STAT bit 7: SIO0
	beq     $2,$0,_padSioRW2_poll
	  nop
	# if (port->type != 8 && _padSioState == 2) { setRC2wait(60); while (chkRC2wait() == 0) ; }
_padSioRW2_got:
	lbu     $3,232($16)
	li      $2,8
	beq     $3,$2,_padSioRW2_tx
	  li    $2,2
	lw      $3,_padSioState
	nop
	bne     $3,$2,_padSioRW2_tx
	  nop
	jal     setRC2wait
	  li    $4,60
_padSioRW2_gap:
	jal     chkRC2wait
	  nop
	beq     $2,$0,_padSioRW2_gap
	  nop
	# DATA = data;  port->rxCnt++;
_padSioRW2_tx:
	lw      $2,_padSio0
	nop
	sb      $19,0($2)               # DATA = data
	lbu     $2,69($16)
	lbu     $3,68($16)
	addiu   $2,$2,1
	sb      $2,69($16)
	# if (port->rxPos != 0xFF) port->rx[port->rxPos] = byte;
	li      $2,255
	beq     $3,$2,_padSioRW2_pos
	  nop
	lbu     $3,68($16)
	lw      $2,60($16)
	nop
	addu    $2,$2,$3
	sb      $18,0($2)
	# port->rxPos++;  return byte;
_padSioRW2_pos:
	lbu     $3,68($16)
	move    $2,$18
	addiu   $3,$3,1
	sb      $3,68($16)
_padSioRW2_ret:
	lw      $31,32($sp)
	lw      $19,28($sp)
	lw      $18,24($sp)
	lw      $17,20($sp)
	lw      $16,16($sp)
	jr      $31
	  addiu $sp,$sp,40

#---------------------------------------------------------------------------------------------------
# _padClrIntSio0  (36 words)
#
# long _padClrIntSio0(void);
#     I_STAT = ~0x80 (clear the SIO0 interrupt), then while the /ACK level (STAT bit 7) is active:
#     return 0 as soon as chkRC2wait() reports the time-out.  Otherwise CTRL |= 0x10 (acknowledge)
#     and return 1.
#---------------------------------------------------------------------------------------------------
	.globl  _padClrIntSio0
_padClrIntSio0:
	addiu   $sp,$sp,-24
	lw      $3,_padIStat
	lw      $4,_padSio0
	li      $2,-129
	sw      $31,16($sp)
	sw      $2,0($3)                # I_STAT = ~0x80: clear the SIO0 interrupt
	lhu     $2,4($4)                # SIO0 STAT
	nop
	andi    $2,$2,0x80              # STAT bit 7: /ACK level
	beq     $2,$0,_padClrIntSio0_ack
	  nop
_padClrIntSio0_wait:
	jal     chkRC2wait
	  nop
	bne     $2,$0,_padClrIntSio0_ret
	  move  $2,$0
	lw      $2,_padSio0
	nop
	lhu     $2,4($2)                # SIO0 STAT
	nop
	andi    $2,$2,0x80              # STAT bit 7: /ACK level
	bne     $2,$0,_padClrIntSio0_wait
	  nop
_padClrIntSio0_ack:
	lw      $4,_padSio0
	nop
	lhu     $3,10($4)               # SIO0 CTRL
	li      $2,1
	ori     $3,$3,0x10              # CTRL bit 4: acknowledge
	sh      $3,10($4)               # SIO0 CTRL
_padClrIntSio0_ret:
	lw      $31,16($sp)
	addiu   $sp,$sp,24
	jr      $31
	  nop

#---------------------------------------------------------------------------------------------------
# _padWaitRXready  (10 words + 1 padding nop)
#
# void _padWaitRXready(void);
#     Spins until SIO0 STAT bit 1 (a received byte is waiting) is set.
#     The nop after the jr's delay slot is the padding that makes the section a multiple of 16 bytes.
#---------------------------------------------------------------------------------------------------
	.globl  _padWaitRXready
_padWaitRXready:
	lw      $3,_padSio0
	nop
_padWaitRXready_loop:
	lhu     $2,4($3)                # SIO0 STAT
	nop
	andi    $2,$2,0x2               # STAT bit 1: byte received
	beq     $2,$0,_padWaitRXready_loop
	  nop
	jr      $31
	  nop
# padding: the text section of the original object is a multiple of 16 bytes (3040)
	nop

# ==============================================================================================
# .data   0x80 bytes
#   Every symbol from _padFuncNextPort to _padFixResult is exported (the stock member's export
#   list).  The hooks are zero in the file and are filled in at run time by the pad init code:
#   PadInitDirect writes _padFuncNextPort, _padFuncClrInfo, _padFuncClrCmdNo, _padInfoDir,
#   _padIntExec and _padModeMtap, and _padInitDirSeq writes _padFuncRecvAuto.
# ==============================================================================================
	.data

# library revision stamp: bytes 50 73 0c 22 8b 99 42 10  (stock 4.2: 50 73 0c 21 ad 48 42 00,
# 4.3: 50 73 0c 25 9b 53 43 00)
_padRevision:
	.word   0x220c7350,0x1042998b

# hooks, written by the init code.  This member calls _padFuncNextPort, _padFuncClrInfo,
# _padFuncRecvAuto, _padFuncClrCmdNo and _padFuncIntGun; the others are for other members.
	.globl  _padFuncNextPort
_padFuncNextPort:                               # void (*)(long)        advance to the next port / report an error code
	.word   0
	.globl  _padFuncClrInfo
_padFuncClrInfo:                                # void (*)(PADPORT *)   clear a port record
	.word   0
	.globl  _padFuncGetTxd
_padFuncGetTxd:
	.word   0
	.globl  _padFuncCurrLimit
_padFuncCurrLimit:
	.word   0
	.globl  _padFuncPtr2Port
_padFuncPtr2Port:
	.word   0
	.globl  _padFuncPort2Info
_padFuncPort2Info:
	.word   0
	.globl  _padFuncSendAuto
_padFuncSendAuto:
	.word   0
	.globl  _padFuncChkEng
_padFuncChkEng:
	.word   0
	.globl  _padFuncRecvAuto
_padFuncRecvAuto:                               # void (*)(PADPORT *)   handle the automatic answer of a port
	.word   0
	.globl  _padFuncClrCmdNo
_padFuncClrCmdNo:                               # void (*)(PADPORT *)   clear the command number of a port
	.word   0
	.globl  _padFuncIntGun
_padFuncIntGun:                                 # long (*)(void)        light-gun hook, called from _padIsVsync
	.word   0
	.globl  _padFuncSetGunPort
_padFuncSetGunPort:
	.word   0
	.globl  _padFuncGetGunPos
_padFuncGetGunPos:
	.word   0

# library state
	.globl  _padInfoDir
_padInfoDir:                                    # PADPORT *   the two port records, 0xF0 bytes each
	.word   0
	.globl  _padIntExec
_padIntExec:                                    # long        1: the vsync handler may poll the ports
	.word   0
	.globl  _padGunExec
_padGunExec:
	.word   0
	.globl  _padSioChan
_padSioChan:                                    # long        channel being served, 0 or 1
	.word   0
	.globl  _padSioState
_padSioState:                                   # long        state of the protocol state machine (index into padIntFunc)
	.word   0
	.globl  _padTotalCurr
_padTotalCurr:                                  # long        cleared by the handler before it steps the ports
	.word   0
	.globl  _padModeMtap
_padModeMtap:
	.word   0
	.globl  _padChanStart
_padChanStart:                                  # long        first channel polled: 0 when channel 0 is on, else 1
	.word   0
	.globl  _padChanStop
_padChanStop:                                   # long        last channel polled: 1 when channel 1 is on, else 0
	.word   1
	.globl  _padFixResult
_padFixResult:                                  # long[2]     per channel, -1: nothing pending (see _padInitSioMode)
	.word   -1,-1

# statics of this member
_padIStat:                                      # volatile unsigned long *   &I_STAT (I_MASK is at +4)
	.word   0x1f801070
_padSio0:                                       # volatile unsigned char *   SIO0 registers
	.word   0x1f801040
_padCalledIntPad:                               # long        set by _padIntPad, read and cleared by _padChkVsync
	.word   0

	.align  4                               # the section is padded to 0x80 bytes (12 zero bytes unused)

# ==============================================================================================
# .bss   0x20 bytes
# ==============================================================================================
	.bss
_padVsyncParam:                                 # the record SysEnqIntRP takes: next, handler, verifier, pad
	.space  16
_padRestPeriod:                                 # long[2]     vsyncs since each channel was switched off, up to 150
	.space  8
	.space  8                               # padding to 0x20 bytes (unused)
