/*******************************************************************************
 ! \file source/vector/mpy_SP_RSxRVxRV.s
 !
 ! \brief  C-Callable multiplication of a real scalar, a real vector, and a 
 !         real vector
 ! \author XingYuanyuan
 ! \date   06/07/2022
 
  HISTORY:
    06/07/2022 - original (XingYuanyuan)
 
  DESCRIPTION: C-Callable multiplication of a real scalar, a real vector, 
               and a real vector
               y[i] = c*w[i]*x[i]
 
  FUNCTION: 
    extern void mpy_SP_RSxRVxRV(float32 *y, const float32 *w, 
                           const float32 *x, const float32 c, const Uint16 N)
 
  USAGE:       mpy_SP_RSxRVxRV(y, w, x, c, N);
 
  PARAMETERS:  float32 *y = result real array
               float32 *w = input real array #1
               float32 *x = input real array #2
               float32  c = input real scalar
               Uint16   N = length of w, x and y arrays
 
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
#  	 	Refer to TI function, parallel computing 

		.text												# enter text section , 进入代码段				
		.attribute		4, 16								# stack_align 的编号是4 , 16 Byte对齐  
		.attribute		5, "rv32i2p0_m2p0_f2p0_c2p0_xsc"	# arch 的编号是5 ,rv32IMFC  
															# 2P0代表其基本ISA的默认版本  
		.file			"vector_mpyRSbyRVbyRv.c"			# emit filename FILE LOCAL symbol table  
		.globl			mpy_SP_RSxRVxRV						# declare global symbol , 声明全局符号  
		.p2align		1									# align to power of 2 ,后续代码按 power(2) 字节对齐  
		.type			mpy_SP_RSxRVxRV,@function			# 将类型设置为函数或对象 

mpy_SP_RSxRVxRV:
# a0 is pinter to y[0],a1 is pinter to w[0], a2 is pinter to x[0]
# a3 is N , fa0 is c
		beqz			a3, .LBB1_2							# if N == 0 , jump to LBB1_2

# main loop, calculate y[i] = c*w[i]*x[i]
# .LBB1_1:
		addi			a3, a3, -2
		flw.pm			ft0, a1,4
		flw.pm			ft1, a2,4
		fmul.s			ft0, ft0, fa0						# ft0 = ft0 * fa0 = w[i] * c
		fmul.s			ft2, ft0, ft1						# ft0 = ft0 * ft1 , y[i] = w[i] * c * x[i]
		flw.pm			ft0, a1,4
		flw.pm			ft1, a2,4

		rptb			LBB1_1,a3			
		fmul.s			ft3, ft0, fa0						# ft0 = ft0 * fa0 = w[i] * c
		fsw.pm			a0,ft2,4	
		flw.pm			ft0, a1,4
		fmul.s			ft2, ft3, ft1						# ft0 = ft0 * ft1 , y[i] = w[i] * c * x[i]
		flw.pm			ft1, a2,4

		LBB1_1:

# return
.LBB1_2:
		fsw.pm			a0,ft2,4
		ret													# return


# end of function mpy_SP_RSxRVxRV()
# =============================================================================

.Lfunc_end1:
		.size			mpy_SP_RSxRVxRV, .Lfunc_end1-mpy_SP_RSxRVxRV
		.ident			"clang version 14.0.4 (ssh://git_repos@192.168.10.228/home/git_repos/share_repos/llvm-project.git 29f1039a7285a5c3a9c353d054140bf2556d4c4d)"
		.section		".note.GNU-stack","",@progbits
		.addrsig

# =============================================================================
#   End of File
# =============================================================================