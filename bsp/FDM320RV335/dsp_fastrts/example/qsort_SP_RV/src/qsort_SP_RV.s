
/*******************************************************************************
 ! \file source/vector/qsort_SP_RS.s
 !
 ! \brief  Sorts a real array
 ! \author XingYuayuan
 ! \date   06/08/2022

  HISTORY:
     06/08/2022 - original ( XingYuayuan )

  FUNCTION:    
 
  USAGE:       
 
  PARAMETERS:  
 
  RETURNS:   
 
  BENCHMARK:  
 
  NOTES:
 
  Target:    SpringCore

*******************************************************************************/

# =============================================================================
#
# $Copyright: Copyright (C) 2022 FDM TEK ALL RIGHTS RESERVED $
#
# =============================================================================

# =============================================================================
# Calling convention:
#            ra          ：return address，返回地址
#            sp          : Stack pointer,栈指针
#            s0 / fp     : Saved register / Frame pointer , 保存寄存器/帧指针
#            s1 - s11    ：Saved register,保存寄存器
#            a0 - a1     : Fcuntion arguments/Return values , 函数参数/返回值
#            a2 - a7     : Fcuntion arguments , 函数参数
#            t0          ：Temporary/alternate link register , 临时寄存器, 备用链接寄存器
#            t1 - t6     ：Temporaries  ,临时寄存器
#            ft0 - ft11  : FP temporaries  ,浮点临时寄存器
#            fs0 - fs11  : FP saved registers  ,浮点保存寄存器
#            fa0 - fa1   : FP arguments/Return values  ,浮点参数/返回值
#            fa2 - fa7   : FP arguments  ,浮点参数
# =============================================================================


# TI 仅提供了c 程序

		.text
		.attribute			4, 16
		.attribute			5, "rv32i2p0_m2p0_f2p0_c2p0"
		.file				"vector_qsortRV.c"
		.globl				qsort_SP_RV
		.p2align			1
		.type				qsort_SP_RV,@function

qsort_SP_RV:
		CSRRWI t0, 0x808, 0x7
	 	addi				sp, sp, -32
	 	sw					ra, 28(sp)
	 	sw					s0, 24(sp)
	 	sw					s1, 20(sp)
	 	sw					s2, 16(sp)
	 	sw					s3, 12(sp)
	 	sw					s4, 8(sp)
	 	sw					s5, 4(sp)
	 	sw					s6, 0(sp)
	 	li					a2, 2
	 	bltu				a1, a2, .LBB1_20
	 	mv					s6, a1
	 	mv					s1, a0
	 	slli				a0, a1, 1
	 	andi				a6, a0, -4
	 	slli				a0, a6, 16
	 	srli				a0, a0, 16
	 	add					a7, s1, a0
	 	lui					s2, 16
	 	addi				s3, s2, -4
	 	li					s4, 1
.LBB1_2:
		slli				a0, s6, 16
		srli				a0, a0, 16
		addi				s5, a0, -1
		slli				a2, s5, 2
		li					s0, 0
		and					a0, a2, s3
		bnez				a0, .LBB1_5
		j					.LBB1_18
.LBB1_3:
		mv					a6, a5
		mv					a7, a0
.LBB1_4:
		and					a0, a2, t2
		and					a1, s0, t2
		bgeu				a1, a0, .LBB1_16
.LBB1_5:
		flw					ft1, 0(a7)
.LBB1_6:
		mv					a3, s0
		slli				a0, s0, 16
		srli				a0, a0, 16
		add					a1, s1, a0
		flw					ft0, 0(a1)
		flt.s				a0, ft0, ft1
		addi				s0, s0, 4
		bnez				a0, .LBB1_6
.LBB1_7:
		mv					a5, a2
		slli				a0, a2, 16
		srli				a0, a0, 16
		add					a0, a0, s1
		flw					ft2, 0(a0)
		flt.s				a4, ft1, ft2
		addi				a2, a2, -4
		bnez				a4, .LBB1_7
		addi				t2, s2, -1
		and					a4, a5, t2
		and					t0, a3, t2
		bgeu				t0, a4, .LBB1_13
		fsw					ft2, 0(a1)
		and					t1, a6, t2
		fsw					ft0, 0(a0)
		beq					t1, a4, .LBB1_11
		mv					a3, a6
		mv					a1, a7
.LBB1_11:
		beq					t1, t0, .LBB1_3
		mv					a5, a3
		mv					a0, a1
		j					.LBB1_3
.LBB1_13:
		beq					t0, a4, .LBB1_15
		mv					s0, a3
		mv					a2, a5
		j					.LBB1_4
.LBB1_15:
		addi				a2, a3, -4
.LBB1_16:
		slli				a0, a2, 16
		srli				a1, a0, 16
		beqz				a1, .LBB1_18
		srli				a0, a0, 18
		addi				a1, a0, 1
		mv					a0, s1
		call				qsort_SP_RV
.LBB1_18:
		slli				a0, s0, 16
		srli				a0, a0, 16
		srli				a1, a0, 2
		bgeu				a1, s5, .LBB1_20
		add					s1, s1, a0
		sub					s6, s6, a1
		slli				a0, s6, 16
		srli				a0, a0, 16
		slli				a1, s6, 1
		andi				a6, a1, -4
		and					a1, a1, s3
		add					a7, s1, a1
		bltu				s4, a0, .LBB1_2
.LBB1_20:
		lw					ra, 28(sp)
		lw					s0, 24(sp)
		lw					s1, 20(sp)
		lw					s2, 16(sp)
		lw					s3, 12(sp)
		lw					s4, 8(sp)
		lw					s5, 4(sp)
		lw					s6, 0(sp)
		addi				sp, sp, 32
		ret

# end of function qsort_SP_RS()
# =============================================================================

.Lfunc_end1:
		.size		 	qsort_SP_RV, .Lfunc_end1-qsort_SP_RV
		.ident			"clang version 14.0.4 (ssh://git_repos@192.168.10.228/home/git_repos/share_repos/llvm-project.git 	29f1039a7285a5c3a9c353d054140bf2556d4c4d)"
		.section		".note.GNU-stack","",@progbits
		.addrsig

# =============================================================================
#   End of File
# =============================================================================