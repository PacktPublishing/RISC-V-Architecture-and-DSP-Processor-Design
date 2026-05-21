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
	j _sc_start		;

_sc_start:
.option push
.option norelax
   la      gp, __global_pointer$
.option pop
#   auipc   gp, 1048336
#   addi    gp, gp, 1698
#   addi    a0, gp, 1560
#   auipc   a2, 14 
#   addi    a2, a2, 970
#   sub     a2, a2, a0 
# --------------start --------------
   la      a0, __bss_start 
   la      a2, __BSS_END__
   sub     a2, a2, a0		# bss region  length
# -------------- end  --------------
   li      a1, 0  
   jal     memset
#   auipc   a0, 0  
#   addi    a0, a0, 930
#   beqz    a0, _start+0x2e
#   auipc   a0, 4  
#   addi    a0, a0, -550
# --------------start --------------
   la      a0, __libc_fini_array
   beqz    a0, 1f
# -------------- end  --------------
   jal     atexit
1:
   jal     __libc_init_array
   lw      a0, 0(sp)
   addi    a1, sp, 4
   li      a2, 0
   # enable watchdog before call main
   # SysCtrlRegs->WDCR= 0x0068;
   li  t1, 0x28
   li  t0, 0xe052
   sh  t1, 0(t0)
   # SysCtrlRegs->WDKEY = 0x55
   li  t1, 0x55
   li  t0, 0xe04a
   sh  t1, 0(t0)
   # SysCtrlRegs->WDKEY = 0xAA
   li  t1, 0xAA
   li  t0, 0xe04a
   sh  t1, 0(t0)
   # jump main
   jal     main
   j       exit
