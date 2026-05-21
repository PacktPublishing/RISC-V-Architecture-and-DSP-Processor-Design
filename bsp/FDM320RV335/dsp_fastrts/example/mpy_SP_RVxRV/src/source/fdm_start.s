	.global _fdm_start
	.section .text_fdm_start,"ax"
_fdm_start:
	.option norvc

	# init regs
	lui      ra, 0x0;
	# lui      sp, 0x0;
	lui      tp, 0x0;
	lui      t0, 0x0;
	lui      t1, 0x0;
	lui      t2, 0x0;
	lui      fp, 0x0;
	lui      s1, 0x0;
	lui      a0, 0x0;
	lui      a1, 0x0;
	lui      a2, 0x0;
	lui      a3, 0x0;
	lui      a4, 0x0;
	lui      a5, 0x0;
	lui      a6, 0x0;
	lui      a7, 0x0;
	lui      s2, 0x0;
	lui      s3, 0x0;
	lui      s4, 0x0;
	lui      s5, 0x0;
	lui      s6, 0x0;
	lui      s7, 0x0;
	lui      s8, 0x0;
	lui      s9, 0x0;
	lui      s10,0x0;
	lui      s11,0x0;
	lui      t3, 0x0;
	lui      t4, 0x0;
	lui      t5, 0x0;
	lui      t6, 0x0;
	
	FMV.W.X      ft0, zero;
	FMV.W.X      ft1, zero;
	FMV.W.X      ft2, zero;
	FMV.W.X      ft3, zero;
	FMV.W.X      ft4, zero;
	FMV.W.X      ft5, zero;
	FMV.W.X      ft6, zero;
	FMV.W.X      ft7, zero;
	FMV.W.X      fs0, zero;
	FMV.W.X      fs1, zero;
	FMV.W.X      fa0, zero;
	FMV.W.X      fa1, zero;
	FMV.W.X      fa2, zero;
	FMV.W.X      fa3, zero;
	FMV.W.X      fa4, zero;
	FMV.W.X      fa5, zero;
	FMV.W.X      fa6, zero;
	FMV.W.X      fa7, zero;
	FMV.W.X      fs2, zero;
	FMV.W.X      fs3, zero;
	FMV.W.X      fs4, zero;
	FMV.W.X      fs5, zero;
	FMV.W.X      fs6, zero;
	FMV.W.X      fs7, zero;
	FMV.W.X      fs8, zero;
	FMV.W.X      fs9, zero;
	FMV.W.X      fs10, zero;
	FMV.W.X      fs11, zero;
	FMV.W.X      ft8, zero;
	FMV.W.X      ft9, zero;
	FMV.W.X      ft10, zero;
	FMV.W.X      ft11, zero;

	# init regs done

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
   jal     main
   j       exit
