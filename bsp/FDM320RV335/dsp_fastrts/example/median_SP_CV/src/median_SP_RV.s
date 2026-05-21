/********************************************************************************
 ! \file source/vector/median_SP_RV.s
 !
 ! \brief  Median of a real valued array of floats.
 ! \author XingYuanyuan
 ! \date   06/02/2022
 ! \note   For best performance, compile with full optimization:
          -o4, -mf5, no -g
 
  HISTORY:
     06/02/2022 - original (XingYuanyuan)
 
  DESCRIPTION:	 Median of a real array of floats.  Input array is NOT preserved. 
  				This routine uses a Quickselect method based on the algorithm  
  				described in "Numerical recipes in C", Second Edition, Cambridge University 
				Press, 1992, Section 8.5, ISBN 0-521-43108-5				
 
  FUNCTION:  float median_SP_RV(float *, uint16_t);

  USAGE:       z = median_SP_RV(x, N);
 
  PARAMETERS:  float *x = pointer to input array
               uint16_t N = length of x array
 
  RETURNS:     float z = result

 
  BENCHMARK:  

 
  NOTES:
  		1) This function is destructive to the input array x in that it will be
 			sorted during function execution.  If this is not allowable, use

 			median_noreorder_SP_CV().
   
 
  Target:            SpringCore
 
********************************************************************************/

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
	

# 实现过程较为复杂，TI 仅提供了c 程序


		.text
		.attribute			4, 16
		.attribute			5, "rv32i2p0_m2p0_f2p0_c2p0"
		.file				"vector_medianRV.c"
		.globl				_nullFunction10
		.p2align			1
		.type				_nullFunction10,@function
_nullFunction10:
		ret
.Lfunc_end0:
		.size				_nullFunction10, .Lfunc_end0-_nullFunction10
		.section			.sdata,"aw",@progbits
		.p2align			2
.LCPI1_0:
		.word				0x7f7fffff
.LCPI1_1:
		.word				0x3f000000
		.text			
		.globl				median_SP_RV
		.p2align			1
		.type				median_SP_RV,@function

median_SP_RV:
		addi				t0, a1, -1
		slli				a2, t0, 16
		srli				a2, a2, 16
		srli				a7, a2, 1
		beqz				a2, .LBB1_39
		li					a4, 0
		li					a6, 0
		addi				t1, a0, 4
		addi				t2, a0, -4
		lui					a2, 16
		addi				t4, a2, -1
		j					.LBB1_3
.LBB1_2:
		and					a2, t0, t4
		and					a4, a6, t4
		bgeu				a4, a2, .LBB1_34
.LBB1_3:
		slli				a2, t0, 16
		srli				a2, a2, 16
		addi				t6, a4, 1
		beq					t6, a2, .LBB1_22
		add					a5, a4, a2
		slli				a5, a5, 1
		andi				a5, a5, -4
		add					a5, a5, a0
		flw					ft0, 0(a5)
		slli				a2, a2, 2
		add					t5, a0, a2
		flw					ft1, 0(t5)
		flt.s				a2, ft1, ft0
		beqz				a2, .LBB1_6
		fsw					ft1, 0(a5)
		fsw					ft0, 0(t5)
		j					.LBB1_7
.LBB1_6:
		fmv.s				ft0, ft1
.LBB1_7:
		slli				a2, a4, 2
		add					t3, a0, a2
		flw					ft1, 0(t3)
		flt.s				a2, ft0, ft1
		beqz				a2, .LBB1_9
		fsw					ft0, 0(t3)
		fsw					ft1, 0(t5)
		flw					ft1, 0(t3)
.LBB1_9:
		flw					ft0, 0(a5)
		flt.s				a2, ft1, ft0
		beqz				a2, .LBB1_11
		fsw					ft1, 0(a5)
		fsw					ft0, 0(t3)
		flw					ft0, 0(a5)
.LBB1_11:
		slli				a2, t6, 2
		add					a2, a2, a0
		flw					ft1, 0(a2)
		fsw					ft1, 0(a5)
		fsw					ft0, 0(a2)
		mv					a4, t0
.LBB1_12:
		slli				a2, t6, 16
		flw					ft0, 0(t3)
		srli				a2, a2, 16
		slli				a2, a2, 2
		add					a2, a2, t1
.LBB1_13:
		flw					ft1, 0(a2)
		addi				t6, t6, 1
		flt.s				a5, ft1, ft0
		addi				a2, a2, 4
		bnez				a5, .LBB1_13
		slli				a2, a4, 16
		srli				a2, a2, 16
		slli				a2, a2, 2
		add					a2, a2, t2
.LBB1_15:
	  	flw					ft1, 0(a2)
	  	mv					a5, a4
	  	addi				a4, a4, -1
	  	flt.s				a3, ft0, ft1
	  	addi				a2, a2, -4
	  	bnez				a3, .LBB1_15
	  	and					a2, a4, t4
	  	and					a3, t6, t4
	  	bltu				a2, a3, .LBB1_18
	  	slli				a3, t6, 16
	  	srli				a3, a3, 16
	  	slli				a3, a3, 2
	  	add					a3, a3, a0
	  	slli				a2, a2, 2
	  	add					a2, a2, a0
	  	flw					ft0, 0(a2)
	  	flw					ft1, 0(a3)
	  	fsw					ft0, 0(a3)
	  	fsw					ft1, 0(a2)
	  	j					.LBB1_12
.LBB1_18:
		slli				a3, a2, 2
		add					a3, a3, a0
		flw					ft1, 0(a3)
		fsw					ft1, 0(t3)
		fsw					ft0, 0(a3)
		bltu				a7, a2, .LBB1_20
		mv					a6, t6
.LBB1_20:
		bltu				a2, a7, .LBB1_2
		addi				t0, a5, -2
		j					.LBB1_2
.LBB1_22:
		slli				a3, a4, 2
		add					a3, a3, a0
		flw					ft0, 0(a3)
		slli				a2, a2, 2
		add					a2, a2, a0
		flw					ft1, 0(a2)
		flt.s				a4, ft1, ft0
		beqz				a4, .LBB1_24
		fsw					ft1, 0(a3)
		fsw					ft0, 0(a2)
.LBB1_24:
		slli				a2, a7, 2
		add					a2, a2, a0
		flw					fa0, 0(a2)
		andi				a2, a1, 1
		bnez				a2, .LBB1_40
		srli				a2, a1, 1
		li					a4, 2
		lui					a3, %hi(.LCPI1_0)
		flw					ft0, %lo(.LCPI1_0)(a3)
		bltu				a1, a4, .LBB1_36
		li					a3, 0
		li					a1, 0
		fsub.s				ft1, ft0, fa0
		j					.LBB1_29
.LBB1_27:
		fmv.s				ft1, ft3
		fmv.s				ft0, ft2
.LBB1_28:
		addi				a3, a3, 1
		slli				a4, a3, 16
		srli				a4, a4, 16
		addi				a0, a0, 8
		bgeu				a4, a2, .LBB1_37
.LBB1_29:
		flw					ft2, 0(a0)
		flt.s				a4, fa0, ft2
		beqz				a4, .LBB1_31
		fsub.s				ft3, ft2, fa0
		flt.s				a4, ft3, ft1
		addi				a1, a1, 1
		bnez				a4, .LBB1_32
.LBB1_31:
		fmv.s				ft3, ft1
		fmv.s				ft2, ft0
.LBB1_32:
		flw					ft0, 4(a0)
		flt.s				a4, fa0, ft0
		beqz				a4, .LBB1_27
		fsub.s				ft1, ft0, fa0
		flt.s				a4, ft1, ft3
		addi				a1, a1, 1
		beqz				a4, .LBB1_27
		j					.LBB1_28
.LBB1_34:
		andi				a2, a1, 1
		bnez				a2, .LBB1_39
		slli				a2, a7, 2
		add					a2, a2, a0
		flw					fa0, 0(a2)
		srli				a2, a1, 1
		li					a4, 2
		lui					a3, %hi(.LCPI1_0)
		flw					ft0, %lo(.LCPI1_0)(a3)
		bgeu				a1, a4, .LBB1_41
.LBB1_36:
		li					a1, 0
.LBB1_37:
		slli				a0, a1, 16
		srli				a0, a0, 16
		bltu				a0, a2, .LBB1_40
		lui					a0, %hi(.LCPI1_1)
		flw					ft1, %lo(.LCPI1_1)(a0)
		fadd.s				ft0, fa0, ft0
		fmul.s				fa0, ft0, ft1
		ret
.LBB1_39:
		slli				a1, a7, 2
		add					a0, a0, a1
		flw					fa0, 0(a0)
.LBB1_40:
		ret
.LBB1_41:
		li					a3, 0
		li					a1, 0
		fsub.s				ft1, ft0, fa0
		j					.LBB1_44
.LBB1_42:
		fmv.s				ft1, ft3
		fmv.s				ft0, ft2
.LBB1_43:			
		addi				a3, a3, 1
		slli				a4, a3, 16
		srli				a4, a4, 16
		addi				a0, a0, 8
		bgeu				a4, a2, .LBB1_37
.LBB1_44:
		flw					ft2, 0(a0)
		flt.s				a4, fa0, ft2
		beqz				a4, .LBB1_46
		fsub.s				ft3, ft2, fa0
		flt.s				a4, ft3, ft1
		addi				a1, a1, 1
		bnez				a4, .LBB1_47
.LBB1_46:			
		fmv.s				ft3, ft1
		fmv.s				ft2, ft0
.LBB1_47:
		flw					ft0, 4(a0)
		flt.s				a4, fa0, ft0
		beqz				a4, .LBB1_42
		fsub.s				ft1, ft0, fa0
		flt.s				a4, ft1, ft3
		addi				a1, a1, 1
		beqz				a4, .LBB1_42
		j					.LBB1_43


# end of function median_SP_RV()
# =============================================================================


.Lfunc_end1:
	.size					median_SP_RV, .Lfunc_end1-median_SP_RV
	.ident					"clang version 14.0.4 (ssh://git_repos@192.168.10.228/home/git_repos/share_repos/llvm-project.git 29f1039a7285a5c3a9c353d054140bf2556d4c4d)"
	.section				".note.GNU-stack","",@progbits
	.addrsig

# =============================================================================
#   End of File
# =============================================================================
