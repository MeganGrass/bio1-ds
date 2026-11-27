# PADIF.S - LIBPAD module PADIF: the pad interrupt state table padIntFunc and its five states (SLUS-00748)
# Resident Evil 2 (SLUS-00748) decompilation
# 376 code words (1504 bytes) and 8 data words. Assembled with ASPSX 2.56 (-G0) and used in place of the stock PADIF member
# of the PSY-Q 4.2 LIBPAD archive, so the member keeps its archive position.
#
# What the module is
#   The pad driver is PSY-Q's LIBPAD (PadInitDirect / PadInitMtap). PADMAIN's SIO0 interrupt routine
#   (_padPortStep) calls padIntFunc[_padSioState++], one state per interrupt, and PADIF holds those states. A state puts
#   the next byte (padState4: all the remaining bytes) of the polling packet on the port with _padSioRW / _padSioRW2 and
#   returns the byte it received (>= 0) or a negative code; the interrupt routine answers a negative code by going on to the
#   next port (_padFuncNextPort). The codes: -2 and -20 come back from _padSioRW2 / _padSioRW (the byte could not be
#   exchanged), -3 is padState4's "no acknowledge" (_padClrIntSio0 timed out), -9 is a reply that does not fit the protocol.
#
#     state  routine    job
#       0    padState0  SendAuto on the channel, then start the packet (the 0x01 address byte)
#       1    padState1  light-gun hook, auto-info of ports A and B, command byte (0x42, or the port's mode byte)
#       2    padState2  auto-info of ports C and D, second command byte (multitap select); the reply is the id
#                       byte, whose low nibble gives the byte count padByteCount
#       3    padState3  next byte out, the reply must be 0x5A
#       4    padState4  multitap preamble, bookkeeping for the other channel, the remaining bytes; the reply to
#                       the last byte is read straight from the SIO0 data register
#
# Data (.data): padByteCount (bytes of the packet still to move), padData (pointer to the SIO0 data
#   register, 0x1F801040), padSendFlag (result of SendAuto in state 0), then the exported table padIntFunc[5]. The five
#   routines and the three variables are file-local; only padIntFunc is exported (the member header of the
#   archive keeps that export list). _padFunc*, _padSioChan, _padChanStart, _padGunExec, _padModeMtap, _padInfoDir and
#   _padFixResult belong to PADMAIN; PadInitDirect / PadInitMtap and the sequence modules PADSEQD / PADSEQM point each
#   _padFunc* at the direct or the multitap flavour of the routine.
#
# Why assembly
#   This revision of PADIF is in no PSY-Q archive (4.2: 1472 bytes, 4.3: 1520, this one: 1504). Against the 4.2 module it
#   has the 0x5A / 0 tests of the reply in padState3 (+5 words) and the _padModeMtap test in padState4 (+5 words), and lacks
#   the two padding words at the end of the module. No compiler or assembler on hand reproduces the code: many
#   branch delay slots hold a store, a load or the release of the stack frame, which the 4.0 compiler (it writes the
#   release as its own instruction, one word more per routine) and ASPSX 2.56 (it never fills a delay slot) do not do. A member
#   that the C compiler cannot reproduce is kept as assembly source, and PADIF is one of them.
#
# Source conventions (ASPSX 2.56; sdklibbuild gives it a CRLF copy)
#   - Registers are numeric except $sp (ASPSX 2.56 knows no $v0, $a0, $s0 or $ra): $1 at, $2-$3 v0-v1, $4-$5 a0-a1,
#     $16-$21 s0-s5, $31 ra.
#   - Branch delay slots are written out under .set noreorder; ASPSX 2.56 never fills one.
#   - "lw $2,sym", "sw $2,sym" and "la $3,sym" are assembler macros (lui + lw, lui $at + sw, lui + addiu): they give the
#     instruction pairs, each half with its own relocation, so the linker supplies the addresses.
#   - The two "--padByteCount" tests of padState4 keep the lui before the branch and put the store into the branch's delay
#     slot. ASPSX 2.56 has no %hi / %lo operators ("sw $2,sym($1)" expands to three words), and it does not accept a .word as
#     the instruction of a delay slot (the branch that follows is reported as being in a delay slot). So the lui is written
#     with the hi expression, and the branch and the store are .word values: the comment on the line names the
#     instruction, the branch offset is computed from the labels, and the store takes the low half of the address of
#     padByteCount from the linker. These two sites (four words) are the only instructions not written as mnemonics.
#
# Layout
#   - The relocation fields of the 101 words that carry one hold what the link's own symbol addresses require; the table
#     padIntFunc holds the offsets (0x000, 0x040, 0x118, 0x1D8, 0x264) from the start of the code.

	.set    noreorder

# Pad channel record that every state receives in $a0 (only the fields PADIF touches).
	.equ    CH_INFO,0x0c                # char *info: the port info records of the channel, INFO_SIZE bytes each (ports A..D)
	.equ    CH_MODE,0x36                # u_char mode: command byte of the port, 0 = plain poll (sent as 0x42)
	.equ    CH_RX,0x3c                  # u_char *rx: receive buffer of the packet
	.equ    CH_RXCNT,0x44               # u_char rxcnt: number of bytes received so far
	.equ    INFO_SIZE,240               # size of one info record

# ---------------------------------------------------------------------------------------------------------------------
# padState0 - state 0 (16 words)
#   static int padState0(PADCHAN *p)
#   {
#       padSendFlag = (*_padFuncSendAuto)(p);       /* p is still in $a0 when the pointer is called */
#       return _padSioRW(p, -2);                    /* -2 starts the packet: _padSioRW puts the 0x01 address byte on the line */
#   }
#   padSendFlag is what states 1 and 2 test before they push the auto-info of the four ports. $16 (s0) = p.
# ---------------------------------------------------------------------------------------------------------------------
	.text
padState0:
	addiu   $sp,$sp,-24                 # frame of 24 bytes (saves $s0 and $ra)
	sw      $16,16($sp)                 # save $s0
	lw      $2,_padFuncSendAuto         # $v0 = the SendAuto routine of the port flavour (lui + lw)
	sw      $31,20($sp)                 # save $ra
	jalr    $2                          # (*_padFuncSendAuto)(p)
	move    $16,$4                      #   delay slot: $s0 = p
	move    $4,$16                      # $a0 = p
	sw      $2,padSendFlag              # padSendFlag = result (lui $at + sw)
	jal     _padSioRW                   # return _padSioRW(p, -2)
	li      $5,-2                       #   delay slot: -2 = start of packet
	lw      $31,20($sp)                 # epilogue: restore $ra and $s0 ...
	lw      $16,16($sp)
	jr      $31                         # ... and return; the frame is released in the delay slot
	addiu   $sp,$sp,24                  #   delay slot: release the frame

# ---------------------------------------------------------------------------------------------------------------------
# padState1 - state 1 (54 words)
#   static int padState1(PADCHAN *p)
#   {
#       if (_padSioChan == _padChanStart && _padGunExec) {      /* light gun: first channel of the cycle only */
#           (*_padFuncGetGunPos)();
#           (*_padFuncSetGunPort)();
#       }
#       if (padSendFlag) {                                      /* auto-info of ports A and B */
#           (*_padFuncSendAuto)(p->info);
#           (*_padFuncSendAuto)(p->info + INFO_SIZE);
#       }
#       if (p->mode != 0)                                       /* the mode byte is read twice */
#           return _padSioRW2(p, p->mode);
#       return _padSioRW2(p, 0x42);                             /* 0x42: the poll command */
#   }
#   The loads of _padSioChan and _padChanStart are scheduled ahead of the frame setup. $16 (s0) = p.
# ---------------------------------------------------------------------------------------------------------------------
padState1:
	lw      $3,_padSioChan              # $v1 = _padSioChan
	lw      $2,_padChanStart            # $v0 = _padChanStart
	addiu   $sp,$sp,-24                 # frame of 24 bytes
	sw      $16,16($sp)                 # save $s0
	move    $16,$4                      # $s0 = p
	bne     $3,$2,padState1_auto        # not the first channel of the cycle: no gun handling
	sw      $31,20($sp)                 #   delay slot: save $ra
	lw      $2,_padGunExec              # $v0 = _padGunExec
	nop                                 # load delay
	beq     $2,$0,padState1_auto        # no gun active
	nop                                 #   delay slot
	lw      $2,_padFuncGetGunPos        # (*_padFuncGetGunPos)()
	nop                                 # load delay
	jalr    $2
	nop                                 #   delay slot
	lw      $2,_padFuncSetGunPort       # (*_padFuncSetGunPort)()
	nop                                 # load delay
	jalr    $2
	nop                                 #   delay slot
padState1_auto:         # auto-info of ports A and B
	lw      $2,padSendFlag              # if (padSendFlag) ...
	nop                                 # load delay
	beq     $2,$0,padState1_cmd         # ... else skip the auto-info
	nop                                 #   delay slot
	lw      $4,CH_INFO($16)             # (*_padFuncSendAuto)(p->info)
	lw      $2,_padFuncSendAuto
	nop                                 # load delay
	jalr    $2
	nop                                 #   delay slot
	lw      $4,CH_INFO($16)             # (*_padFuncSendAuto)(p->info + INFO_SIZE)
	lw      $2,_padFuncSendAuto
	nop                                 # load delay
	jalr    $2
	addiu   $4,$4,INFO_SIZE             #   delay slot: $a0 = p->info + 240
padState1_cmd:          # command byte
	lbu     $2,CH_MODE($16)             # $v0 = p->mode (tested here ...)
	nop                                 # load delay
	bne     $2,$0,padState1_modeByte    # nonzero: the mode byte is the command
	move    $4,$16                      #   delay slot: $a0 = p
	j       padState1_send              # zero: the command is 0x42
	li      $5,0x42                     #   delay slot: $a1 = 0x42 (poll)
padState1_modeByte:
	lbu     $5,CH_MODE($16)             # $a1 = p->mode (... and read a second time)
padState1_send:
	jal     _padSioRW2                  # return _padSioRW2(p, command)
	nop                                 #   delay slot
	lw      $31,20($sp)                 # epilogue
	lw      $16,16($sp)
	jr      $31
	addiu   $sp,$sp,24                  #   delay slot: release the frame

# ---------------------------------------------------------------------------------------------------------------------
# padState2 - state 2 (48 words)
#   static int padState2(PADCHAN *p)
#   {
#       int r;
#       if (padSendFlag) {                                      /* auto-info of ports C and D */
#           (*_padFuncSendAuto)(p->info + 2 * INFO_SIZE);
#           (*_padFuncSendAuto)(p->info + 3 * INFO_SIZE);
#       }
#       r = _padSioRW2(p, p->mode == 0 ? _padModeMtap : 0);     /* second command byte; the reply is the id byte */
#       if (r < 0) return r;
#       if ((r & 0xf0) == 0) return -9;                         /* high nibble 0: no usable id */
#       padByteCount = (r & 0x0f) << 1;                         /* low nibble = 16-bit words that follow */
#       if (padByteCount == 0) padByteCount = 32;               /* nibble 0 is taken as the maximum */
#       return 0;
#   }
#   padSendFlag is loaded ahead of the frame setup. $16 (s0) = p.
# ---------------------------------------------------------------------------------------------------------------------
padState2:
	lw      $2,padSendFlag              # $v0 = padSendFlag
	addiu   $sp,$sp,-24                 # frame of 24 bytes
	sw      $16,16($sp)                 # save $s0
	move    $16,$4                      # $s0 = p
	beq     $2,$0,padState2_probe       # padSendFlag == 0: no auto-info
	sw      $31,20($sp)                 #   delay slot: save $ra
	lw      $4,CH_INFO($16)             # (*_padFuncSendAuto)(p->info + 2 * INFO_SIZE)
	lw      $2,_padFuncSendAuto
	nop                                 # load delay
	jalr    $2
	addiu   $4,$4,INFO_SIZE*2           #   delay slot: $a0 = p->info + 480
	lw      $4,CH_INFO($16)             # (*_padFuncSendAuto)(p->info + 3 * INFO_SIZE)
	lw      $2,_padFuncSendAuto
	nop                                 # load delay
	jalr    $2
	addiu   $4,$4,INFO_SIZE*3           #   delay slot: $a0 = p->info + 720
padState2_probe:                # second command byte
	lbu     $2,CH_MODE($16)             # $v0 = p->mode
	nop                                 # load delay
	bne     $2,$0,padState2_send        # mode != 0: the second byte is 0
	move    $5,$0                       #   delay slot: $a1 = 0
	lw      $5,_padModeMtap             # mode == 0: $a1 = _padModeMtap
padState2_send:
	jal     _padSioRW2                  # r = _padSioRW2(p, $a1)
	move    $4,$16                      #   delay slot: $a0 = p
	move    $3,$2                       # $v1 = r
	bltz    $3,padState2_ret            # r < 0: return r
	nop                                 #   delay slot
	andi    $2,$3,0xf0                  # high nibble of the id byte
	bne     $2,$0,padState2_count
	andi    $2,$3,0xf                   #   delay slot: $v0 = low nibble
	j       padState2_ret               # high nibble 0: return -9
	li      $2,-9                       #   delay slot: $v0 = -9
padState2_count:                # byte count from the id byte
	sll     $2,$2,1                     # low nibble * 2 = number of bytes
	sw      $2,padByteCount             # padByteCount = bytes (lui $at + sw)
	bne     $2,$0,padState2_ret         # count != 0: done, return 0
	move    $2,$0                       #   delay slot: $v0 = 0
	li      $2,32                       # nibble 0: 32 bytes
	sw      $2,padByteCount
	move    $2,$0                       # return 0
padState2_ret:
	lw      $31,20($sp)                 # epilogue
	lw      $16,16($sp)
	jr      $31
	addiu   $sp,$sp,24                  #   delay slot: release the frame

# ---------------------------------------------------------------------------------------------------------------------
# padState3 - state 3 (35 words)
#   static int padState3(PADCHAN *p)
#   {
#       int mtap = 0, r;
#       if ((*p->rx >> 4) == 8)                                 /* id nibble 8 in the first received byte: multitap */
#           mtap = (p->mode == 0);
#       r = _padSioRW2(p, (*_padFuncGetTxd)(p, mtap) & 0xff);   /* next byte out; the reply should be 0x5A */
#       if (r == 0x5a) return r;
#       if (r == 0) return r;
#       if (r >= 0) return -9;
#       return r;
#   }
#   This state shifts the nibble with srl, padState4 uses sra for the same test. $16 (s0) = p.
# ---------------------------------------------------------------------------------------------------------------------
padState3:
	addiu   $sp,$sp,-24                 # frame of 24 bytes
	sw      $16,16($sp)                 # save $s0
	move    $16,$4                      # $s0 = p
	sw      $31,20($sp)                 # save $ra
	lw      $2,CH_RX($16)               # $v0 = p->rx
	nop                                 # load delay
	lbu     $2,0($2)                    # $v0 = *p->rx
	li      $3,8                        # $v1 = 8
	srl     $2,$2,4                     # $v0 = *p->rx >> 4
	bne     $2,$3,padState3_send        # not a multitap id
	move    $5,$0                       #   delay slot: mtap = 0
	lbu     $2,CH_MODE($16)             # $v0 = p->mode
	nop                                 # load delay
	sltiu   $5,$2,1                     # mtap = (p->mode == 0)
padState3_send:
	lw      $2,_padFuncGetTxd           # $v0 = _padFuncGetTxd
	nop                                 # load delay
	jalr    $2                          # byte = (*_padFuncGetTxd)(p, mtap)
	move    $4,$16                      #   delay slot: $a0 = p
	move    $4,$16                      # $a0 = p
	jal     _padSioRW2                  # r = _padSioRW2(p, byte & 0xff)
	andi    $5,$2,0xff                  #   delay slot: $a1 = byte & 0xff
	move    $3,$2                       # $v1 = r
	li      $2,0x5a                     # $v0 = 0x5a
	beq     $3,$2,padState3_ret         # r == 0x5a: return r
	move    $2,$3                       #   delay slot: $v0 = r
	beq     $3,$0,padState3_ret         # r == 0: return r
	nop                                 #   delay slot
	bgez    $3,padState3_ret            # any other r >= 0: return -9
	li      $2,-9                       #   delay slot: $v0 = -9
	move    $2,$3                       # r < 0: return r
padState3_ret:
	lw      $31,20($sp)                 # epilogue
	lw      $16,16($sp)
	jr      $31
	addiu   $sp,$sp,24                  #   delay slot: release the frame

# ---------------------------------------------------------------------------------------------------------------------
# padState4 - state 4 (223 words)
#   Last state: the multitap preamble, the bookkeeping for the other channel, the remaining bytes of the packet and the
#   last byte. Registers: $17 (s1) = p, $20 (s4) = mtap, $16 (s0) = i, later the pointer fix, $18 (s2) = off, later
#   chan * INFO_SIZE, $19 (s3) = cur (a port info pointer), $21 (s5) = 3; $4 (a0) is chan for a moment, then st.
#   static int padState4(PADCHAN *p)
#   {
#       int mtap = 0, i, off, chan, st, v, r;
#       int *fix;
#       char *cur;
#
#       (*_padFuncCurrLimit)(p);                               /* p is still in $a0 */
#       if (_padModeMtap && (*p->rx >> 4) == 8)                /* sra here, srl in padState3 */
#           mtap = (p->mode == 0);
#       if (mtap) {                                            /* multitap preamble: 5 bytes (first one without CurrLimit) */
#           i = -1; off = -INFO_SIZE;
#           do {
#               if (--padByteCount <= 0) break;
#               if (i >= 0) (*_padFuncCurrLimit)(p->info + off);
#               r = _padSioRW2(p, (*_padFuncGetTxd)(p, 1) & 0xff);
#               if (r < 0) return r;
#               setRC2wait(60);                                /* wait for the acknowledge of the byte ... */
#               if (!_padClrIntSio0()) return -3;              /* ... and clear it; -3 = no acknowledge */
#               i++;                                           /* (in the delay slot of the test above) */
#               off += INFO_SIZE;                              /* (in the delay slot of the loop branch) */
#           } while (i < 4);
#       }
#       chan = (_padSioChan == 0);                             /* the other channel */
#       if (padByteCount > 1) {
#           fix = &_padFixResult[chan];
#           do {
#               st = *fix;
#               if (st < 0) break;
#               if (st > 0) {
#                   cur = *(char **)((char *)_padInfoDir + chan * INFO_SIZE + CH_INFO) + st * INFO_SIZE - INFO_SIZE;
#                   (*_padFuncRecvAuto)(cur);
#               }
#               st = *fix;
#               if (st == 3) {
#                   (*_padFuncRecvAuto)(cur - INFO_SIZE);
#                   v = 1;
#                   goto store;
#               }
#               if (st < 4) {
#                   if (st < 2 && st >= 0) {
#                       cur = (char *)_padInfoDir + chan * INFO_SIZE;
#                       (*_padFuncRecvAuto)(cur);
#                       (*_padFuncClrCmdNo)(cur);
#                       v = -1;
#   store:              *fix = v;
#                   }
#               } else if (st == 4) {
#                   *fix = 3;
#               }
#               r = _padSioRW(p, (*_padFuncGetTxd)(p, mtap) & 0xff);
#               if (r < 0) return r;
#               setRC2wait(60);
#               if (!_padClrIntSio0()) return -3;
#               padByteCount--;
#           } while (padByteCount > 1);
#       }
#       while (--padByteCount > 0) {                           /* the remaining bytes */
#           r = _padSioRW(p, (*_padFuncGetTxd)(p, mtap) & 0xff);
#           if (r < 0) return r;
#           setRC2wait(60);
#           if (!_padClrIntSio0()) return -3;
#       }
#       _padWaitRXready();                                     /* the reply to the last byte is read straight from ... */
#       p->rx[p->rxcnt++] = *padData;                          /* ... the SIO0 data register */
#       (*_padFuncNextPort)(0);
#       return 0;
#   }
# ---------------------------------------------------------------------------------------------------------------------
padState4:
	addiu   $sp,$sp,-48                 # frame of 48 bytes (saves $s0-$s5 and $ra)
	sw      $17,20($sp)                 # save $s1
	move    $17,$4                      # $s1 = p
	lw      $2,_padFuncCurrLimit        # $v0 = _padFuncCurrLimit
	sw      $31,40($sp)                 # save $ra, $s5 ... $s2
	sw      $21,36($sp)
	sw      $20,32($sp)
	sw      $19,28($sp)
	sw      $18,24($sp)
	jalr    $2                          # (*_padFuncCurrLimit)(p), p is still in $a0
	sw      $16,16($sp)                 #   delay slot: save $s0
	lw      $2,_padModeMtap             # if (_padModeMtap) ...
	nop                                 # load delay
	beq     $2,$0,padState4_mtapTest    # ... else mtap stays 0
	move    $20,$0                      #   delay slot: mtap = 0
	lw      $2,CH_RX($17)               # $v0 = p->rx
	nop                                 # load delay
	lbu     $2,0($2)                    # $v0 = *p->rx
	li      $3,8                        # $v1 = 8
	sra     $2,$2,4                     # $v0 = *p->rx >> 4
	bne     $2,$3,padState4_mtapTest    # not a multitap id
	nop                                 #   delay slot
	lbu     $2,CH_MODE($17)             # $v0 = p->mode
	nop                                 # load delay
	sltiu   $20,$2,1                    # mtap = (p->mode == 0)
padState4_mtapTest:             # multitap preamble if mtap
	beq     $20,$0,padState4_ports      # no multitap: skip the preamble
	nop                                 #   delay slot
	li      $16,-1                      # i = -1
	li      $18,-INFO_SIZE              # off = -INFO_SIZE
padState4_mtapLoop:
	lw      $2,padByteCount             # $v0 = padByteCount (lui $v0 + lw)
	nop                                 # load delay
	addiu   $2,$2,-1                    # $v0 = padByteCount - 1
	# --- "--padByteCount" with the store in the delay slot of the branch: lui $at, blez, sw. The branch and the store are
	# --- .word values (see "Source conventions" at the top): the comments give the instruction each one encodes.
	.set    noat
	lui     $1,((padByteCount+0x8000)>>16)  # $at = hi(padByteCount)
padState4_mtapCountBr:
	.word   0x18400000|(((padState4_ports-padState4_mtapCountBr-4)>>2)&0xffff)  # blez $v0,padState4_ports
	.word   0xac220000|(padByteCount&0xffff)  # sw $v0,lo(padByteCount)($at), in the delay slot of the blez
	.set    at
	bltz    $16,padState4_mtapSend      # i < 0: no CurrLimit for the first byte
	nop                                 #   delay slot
	lw      $4,CH_INFO($17)             # (*_padFuncCurrLimit)(p->info + off)
	lw      $2,_padFuncCurrLimit
	nop                                 # load delay
	jalr    $2
	addu    $4,$4,$18                   #   delay slot: $a0 = p->info + off
padState4_mtapSend:
	move    $4,$17                      # $a0 = p
	lw      $2,_padFuncGetTxd           # (*_padFuncGetTxd)(p, 1)
	nop                                 # load delay
	jalr    $2
	li      $5,1                        #   delay slot: $a1 = 1
	move    $4,$17                      # $a0 = p
	jal     _padSioRW2                  # r = _padSioRW2(p, byte & 0xff)
	andi    $5,$2,0xff                  #   delay slot: $a1 = byte & 0xff
	bltz    $2,padState4_ret            # r < 0: return r
	nop                                 #   delay slot
	jal     setRC2wait                  # setRC2wait(60)
	li      $4,60                       #   delay slot: $a0 = 60
	jal     _padClrIntSio0              # ok = _padClrIntSio0()
	nop                                 #   delay slot
	beq     $2,$0,padState4_noAck       # no acknowledge: return -3
	addiu   $16,$16,1                   #   delay slot: i++
	slti    $2,$16,4                    # i < 4: next byte
	bne     $2,$0,padState4_mtapLoop
	addiu   $18,$18,INFO_SIZE           #   delay slot: off += INFO_SIZE
padState4_ports:                # bookkeeping for the other channel while padByteCount > 1
	lw      $3,_padSioChan              # $v1 = _padSioChan
	lw      $2,padByteCount             # $v0 = padByteCount
	nop                                 # load delay
	slti    $2,$2,2                     # padByteCount < 2: on to the last loop
	bne     $2,$0,padState4_lastTest
	sltiu   $4,$3,1                     #   delay slot: chan = (_padSioChan == 0)
	la      $3,_padFixResult            # $v1 = &_padFixResult[0]
	sll     $2,$4,2                     # $v0 = chan * 4
	addu    $16,$2,$3                   # $s0 = fix = &_padFixResult[chan]
	sll     $2,$4,4                     # $v0 = chan * 16
	subu    $2,$2,$4                    # $v0 = chan * 15
	sll     $18,$2,4                    # $s2 = chan * 240 = chan * INFO_SIZE
	li      $21,3                       # $s5 = 3
padState4_portLoop:
	lw      $4,0($16)                   # $a0 = st = *fix
	nop                                 # load delay
	bltz    $4,padState4_lastTest       # st < 0: leave the loop
	nop                                 #   delay slot
	blez    $4,padState4_portSt         # st == 0: no RecvAuto for a port record
	sll     $2,$4,4                     #   delay slot: $v0 = st * 16
	lw      $3,_padInfoDir              # $v1 = _padInfoDir
	subu    $2,$2,$4                    # $v0 = st * 15
	addu    $3,$18,$3                   # $v1 = _padInfoDir + chan * INFO_SIZE
	lw      $3,CH_INFO($3)              # $v1 = the info pointer of that record
	sll     $2,$2,4                     # $v0 = st * 240
	addu    $3,$3,$2                    # $v1 = info + st * INFO_SIZE
	addiu   $19,$3,-INFO_SIZE           # $s3 = cur = info + (st - 1) * INFO_SIZE
	lw      $2,_padFuncRecvAuto         # (*_padFuncRecvAuto)(cur)
	nop                                 # load delay
	jalr    $2
	move    $4,$19                      #   delay slot: $a0 = cur
padState4_portSt:               # second look at st
	lw      $3,0($16)                   # $v1 = st = *fix (read again)
	nop                                 # load delay
	beq     $3,$21,padState4_portSt3    # st == 3
	nop                                 #   delay slot
	slti    $2,$3,4
	beq     $2,$0,padState4_portSt4     # st >= 4
	slti    $2,$3,2                     #   delay slot: $v0 = (st < 2)
	beq     $2,$0,padState4_portSend    # 2 <= st < 4, i.e. st == 2: nothing to change
	move    $4,$17                      #   delay slot: $a0 = p
	bltz    $3,padState4_portSend       # st < 0: nothing to change
	nop                                 #   delay slot
	j       padState4_portSt01          # st is 0 or 1
	nop                                 #   delay slot
padState4_portSt4:              # st >= 4
	li      $2,4
	bne     $3,$2,padState4_portSend    # st > 4: nothing to change
	move    $4,$17                      #   delay slot: $a0 = p
	j       padState4_portSend          # st == 4: *fix = 3
	sw      $21,0($16)                  #   delay slot: *fix = 3
padState4_portSt3:              # st == 3
	lw      $2,_padFuncRecvAuto         # (*_padFuncRecvAuto)(cur - INFO_SIZE)
	nop                                 # load delay
	jalr    $2
	addiu   $4,$19,-INFO_SIZE           #   delay slot: $a0 = cur - 240
	j       padState4_portStore         # *fix = 1
	li      $2,1                        #   delay slot: v = 1
padState4_portSt01:             # st is 0 or 1
	lw      $2,_padInfoDir              # cur = _padInfoDir + chan * INFO_SIZE
	nop                                 # load delay
	addu    $19,$2,$18
	lw      $2,_padFuncRecvAuto         # (*_padFuncRecvAuto)(cur)
	nop                                 # load delay
	jalr    $2
	move    $4,$19                      #   delay slot: $a0 = cur
	lw      $2,_padFuncClrCmdNo         # (*_padFuncClrCmdNo)(cur)
	nop                                 # load delay
	jalr    $2
	move    $4,$19                      #   delay slot: $a0 = cur
	li      $2,-1                       # v = -1
padState4_portStore:
	sw      $2,0($16)                   # *fix = v
	move    $4,$17                      # $a0 = p
padState4_portSend:             # one byte for this port
	lw      $2,_padFuncGetTxd           # (*_padFuncGetTxd)(p, mtap)
	nop                                 # load delay
	jalr    $2
	move    $5,$20                      #   delay slot: $a1 = mtap
	move    $4,$17                      # $a0 = p
	jal     _padSioRW                   # r = _padSioRW(p, byte & 0xff)
	andi    $5,$2,0xff                  #   delay slot: $a1 = byte & 0xff
	bltz    $2,padState4_ret            # r < 0: return r
	nop                                 #   delay slot
	jal     setRC2wait                  # setRC2wait(60)
	li      $4,60                       #   delay slot: $a0 = 60
	jal     _padClrIntSio0
	nop                                 #   delay slot
	bne     $2,$0,padState4_portNext    # acknowledged
	nop                                 #   delay slot
padState4_noAck:                # shared "return -3"
	j       padState4_ret               # no acknowledge: return -3
	li      $2,-3                       #   delay slot: $v0 = -3
padState4_portNext:
	lw      $2,padByteCount             # padByteCount--
	nop                                 # load delay
	addiu   $2,$2,-1
	sw      $2,padByteCount             # (lui $at + sw)
	slti    $2,$2,2                     # padByteCount >= 2: next round
	beq     $2,$0,padState4_portLoop
	nop                                 #   delay slot
	j       padState4_lastTest          # otherwise on to the last loop
	nop                                 #   delay slot
padState4_lastBody:             # one byte of the last loop
	move    $4,$17                      # $a0 = p
	lw      $2,_padFuncGetTxd           # (*_padFuncGetTxd)(p, mtap)
	nop                                 # load delay
	jalr    $2
	move    $5,$20                      #   delay slot: $a1 = mtap
	move    $4,$17                      # $a0 = p
	jal     _padSioRW                   # r = _padSioRW(p, byte & 0xff)
	andi    $5,$2,0xff                  #   delay slot: $a1 = byte & 0xff
	bltz    $2,padState4_ret            # r < 0: return r
	nop                                 #   delay slot
	jal     setRC2wait                  # setRC2wait(60)
	li      $4,60                       #   delay slot: $a0 = 60
	jal     _padClrIntSio0
	nop                                 #   delay slot
	beq     $2,$0,padState4_ret         # no acknowledge: return -3
	li      $2,-3                       #   delay slot: $v0 = -3
padState4_lastTest:             # while (--padByteCount > 0)
	lw      $2,padByteCount             # $v0 = padByteCount (lui $v0 + lw)
	nop                                 # load delay
	addiu   $2,$2,-1                    # $v0 = padByteCount - 1
	# --- The same arrangement as in the multitap loop: lui $at, bgtz, sw in the delay slot (.word values).
	.set    noat
	lui     $1,((padByteCount+0x8000)>>16)  # $at = hi(padByteCount)
padState4_lastCountBr:
	.word   0x1c400000|(((padState4_lastBody-padState4_lastCountBr-4)>>2)&0xffff)  # bgtz $v0,padState4_lastBody
	.word   0xac220000|(padByteCount&0xffff)  # sw $v0,lo(padByteCount)($at), in the delay slot of the bgtz
	.set    at
	jal     _padWaitRXready             # wait until the receiver holds the last byte
	nop                                 #   delay slot
	lbu     $3,CH_RXCNT($17)            # $v1 = c = p->rxcnt
	nop                                 # load delay
	addiu   $2,$3,1
	sb      $2,CH_RXCNT($17)            # p->rxcnt = c + 1
	lw      $2,padData                  # $v0 = padData (the SIO0 data register)
	lw      $4,CH_RX($17)               # $a0 = p->rx
	lbu     $2,0($2)                    # $v0 = *padData
	addu    $3,$3,$4                    # $v1 = &p->rx[c]
	sb      $2,0($3)                    # p->rx[c] = *padData
	lw      $2,_padFuncNextPort         # $v0 = _padFuncNextPort
	nop                                 # load delay
	jalr    $2                          # (*_padFuncNextPort)(0)
	move    $4,$0                       #   delay slot: $a0 = 0
	move    $2,$0                       # return 0
padState4_ret:
	lw      $31,40($sp)                 # epilogue: restore $ra, $s5 ... $s0
	lw      $21,36($sp)
	lw      $20,32($sp)
	lw      $19,28($sp)
	lw      $18,24($sp)
	lw      $17,20($sp)
	lw      $16,16($sp)
	jr      $31
	addiu   $sp,$sp,48                  #   delay slot: release the frame

# ---------------------------------------------------------------------------------------------------------------------
# Data (8 words)
# ---------------------------------------------------------------------------------------------------------------------
	.data
padByteCount:           # bytes of the packet still to be moved: set by padState2 from the id byte, counted down by padState4
	.word   0
padData:                # pointer to the SIO0 data register (JOY_DATA); padState4 reads the last byte from it
	.word   0x1f801040
padSendFlag:            # result of the SendAuto call of padState0, tested by padState1 and padState2
	.word   0
	.globl  padIntFunc
padIntFunc:             # the state table, indexed by _padSioState (the only exported symbol of the module)
	.word   padState0                   # state 0
	.word   padState1                   # state 1
	.word   padState2                   # state 2
	.word   padState3                   # state 3
	.word   padState4                   # state 4
