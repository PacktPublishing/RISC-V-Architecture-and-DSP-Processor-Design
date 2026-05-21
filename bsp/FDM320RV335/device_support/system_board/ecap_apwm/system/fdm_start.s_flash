	.global _fdm_start
	.section .text_fdm_start,"ax"
_fdm_start:
	.option norvc
	# disable watchdog
	# SysCtrlRegs->WDCR= 0x0068;
	li	t1, 0x68
	li  t0, 0xe052
	sh  t1, 0(t0)
	# init sp
	la sp, __stack_top
	# copy data from flash_g to dmem
	li a0, 0x10000
	li a1, 0x610000
	li a2, 0x10000
copy_loop_d:
	lb t0, 0(a1)
	sb t0, 0(a0)
	addi a0, a0, 1
	addi a1, a1, 1
	addi a2, a2, -1
	bnez a2, copy_loop_d
	# copy data from flash_g to dmem done

	j _sc_start		;

_sc_start:
.option push
.option norelax
   la      gp, __global_pointer$
.option pop

   jal     main
   j       exit
