/*******************************************************************************
 ! \file source/vector/mpy_SP_RSxRV.s
 !
 ! \brief  C-Callable multiplication of a real scalar, a real vector, and a 
 !         real vector
 ! \author XingYuanyuan
 ! \date   06/06/2022
 
  HISTORY:
    06/06/2022 - original (XingYuanyuan)
 
  DESCRIPTION: C-Callable multiplication of a real scalar and a real vector
               y[i] = c*x[i]
 
  FUNCTION: 
    extern void mpy_SP_RSxRV(float32 *y, const float32 *x, const float32 c, 
                         const Uint16 N)
 
  USAGE:       mpy_SP_RSxRV(y, x, c, N);
 
  PARAMETERS:  float32 *y = result real array
               float32 *x = input real array
               float32 c  = input real scalar
               Uint16 N = length of x and y array
 
  RETURNS:     none
 
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

# optimized : 
#		loop -> repeat
#		flw/fsw -> flw.pm/fsw.pm
# TODO : 
#	  Refer to TI function, parallel computing 

		.text													# enter text section , 进入代码段
		.attribute			4, 16								# stack_align 的编号是4 , 16 Byte 对齐
		.attribute			5, "rv32i2p0_m2p0_f2p0_c2p0_xsc"	# arch 的编号是5 ,rv32IMFC 
																# 2P0代表其基本ISA的默认版本
		.file				"vector_mpyRSbyRV.c"				# emit filename FILE LOCAL symbol table
		.globl	 			mpy_SP_RSxRV						# declare global symbol, 声明全局符号
		.p2align			1									# align to power of 2 ,后续代码按 power(2) 字节对齐
		.type				mpy_SP_RSxRV,@function				# 将类型设置为函数或对象

mpy_SP_RSxRV:
# a0 is pointer to  y, a1 is  pointer to x, a2 is N	
# fa0 is c
		beqz				a2, .LBB1_2  					# if N == 0 , jump to LBB1_2

#  calculate : y[i] = c*x[i]
#	.LBB1_1:
		srli				a2, a2, 1
		addi				a2,a2,-2

		flw.pm				ft0,a1,4
		flw.pm				ft2,a1,4
		fmul.s				ft1, ft0, fa0					# ft0 = ft0 * fa0 = x[i] * c
		fmul.s				ft3, ft2, fa0					# ft0 = ft0 * fa0 = x[i] * c
		flw.pm				ft0,a1,4
		flw.pm				ft2,a1,4

		rptb				LBB1_1,a2
		fsw.pm				a0,ft1,4						#  *(a0 + 0) = ft0 , store y
		fmul.s				ft1, ft0, fa0					# ft0 = ft0 * fa0 = x[i] * c
		fsw.pm				a0,ft3,4						#  *(a0 + 0) = ft0 , store y
		flw.pm				ft0,a1,4
		fmul.s				ft3, ft2, fa0					# ft0 = ft0 * fa0 = x[i] * c
		flw.pm				ft2,a1,4
	LBB1_1:
# return 
.LBB1_2:
		fsw.pm				a0,ft1,4						#  *(a0 + 0) = ft0 , store y
		fsw.pm				a0,ft3,4						#  *(a0 + 0) = ft0 , store y
		ret													# return

# end of function mpy_SP_RSxRV()
# =============================================================================


.Lfunc_end1:
		.size				mpy_SP_RSxRV, .Lfunc_end1-mpy_SP_RSxRV
		.ident				"clang version 14.0.4 (ssh://git_repos@192.168.10.228/home/git_repos/share_repos/llvm-project.git 	29f1039a7285a5c3a9c353d054140bf2556d4c4d)"
		.section			".note.GNU-stack","",@progbits
		.addrsig

# =============================================================================
#   End of File
# =============================================================================