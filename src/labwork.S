  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Edited by M Bjorkman at 2020-01-26
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm

  #
  # Please add your own code below this line
  #
	.data
	.align 2
mywait: .word 6634
	.text
# Convert hexadecimal to ascii
hexasc:
	andi	$t0,$a0,0xf	# Only get 4 least significant bits of $a0
	slti	$t1,$t0,0xa	# 1 of 0-9, 0 if A-F
	beq	$0,$t1,halet
	nop
	addi	$t0,$t0,0x30
	j	haret
	nop
halet:
	addi	$t0,$t0,0x37	# $t0 is already 10, so 10 becomes 0x41 (which is A)
haret:
	move	$v0,$t0
	jr	$ra
	nop

# Delay to wait
.global delay
delay:
	slti	$t8,$a0,1
	bnez	$t8,dend	# skip delay if argument 1 (, ms) is less than 1
	nop
	move	$t0,$a0
	la	$t3,mywait
	lw	$t3,($t3)	# $t3 is waiting constsnt
dwhile:
	addi	$t0,$t0,-1	# remove 1 ms
	add	$t1,$0,$0	# set 'i' to 0
dfor:
	addi	$t1,$t1,1	# add 1 to 'i'
	slt	$t9,$t1,$t3	# if 'i' less than waiting constant, contiue loop
	bnez	$t9,dfor
	nop
	slti	$t8,$t0,1	# if ms is less than 1, end loop
	beqz	$t8,dwhile
	nop
dend:
	jr	$ra
	nop

# Save timestamp to a string
.global time2string
time2string:
	PUSH $s0
	PUSH $s1
	PUSH $s2
	PUSH $s3
	andi	$s0,$a1,0xf	# Minute least sig
	andi	$s1,$a1,0xf0	# Minute most sig
	srl	$s1,$s1,0x4
	andi	$s2,$a1,0xf00	# Hour least sig
	srl	$s2,$s2,0x8
	andi	$s3,$a1,0xf000	# Hour most sig
	srl	$s3,$s3,0xc
	PUSH $a0
	PUSH $ra
	move	$a0,$s0		# Argument
	jal	hexasc		# call hexasc
	nop
	move	$s0,$v0
	move	$a0,$s1		# Argument
	jal	hexasc		# call hexasc
	nop
	move	$s1,$v0
	move	$a0,$s2		# Argument
	jal	hexasc		# call hexasc
	nop
	move	$s2,$v0
	move	$a0,$s3		# Argument
	jal	hexasc		# call hexasc
	nop
	move	$s3,$v0
	POP $ra
	POP $a0
	li $t0, 0x3A
	sb $s3, 0($a0)
	sb $s2, 1($a0)
	sb $t0, 2($a0)
	sb $s1, 3($a0)
	sb $s0, 4($a0)
	sb $0, 5($a0)
	POP $s3
	POP $s2
	POP $s1
	POP $s0
	jr	$ra
	nop

# Enable interrupts
.global enable_interrupt
enable_interrupt:
	ei
	jr	$ra
	nop