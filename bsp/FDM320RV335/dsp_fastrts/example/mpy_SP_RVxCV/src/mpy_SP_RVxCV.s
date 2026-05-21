/*******************************************************************************
 ! \file source/vector/mpy_SP_RVxCV.s
 !
 ! \brief  C-Callable multiplication of a real vector and a complex vector
 ! \author XingYuanyuan
 ! \date   06/07/2022
 
  HISTORY:
    06/07/2022 - original (XingYuanyuan)
 
  DESCRIPTION: C-Callable multiplication of a real vector and a complex vector
               y_re[i] = x[i]*w_re[i]
               y_im[i] = x[i]*w_im[i]
 
  FUNCTION:   
    extern void mpy_SP_RVxCV(complex_float *y, const complex_float *w, 
                             const float32 *x, const Uint16 N)
 
  USAGE:       mpy_SP_RVxCV(y, w, x, N);
 
  PARAMETERS:  complex_float *y = result complex array
               complex_float *w = input complex array
               float32 *x  = input real array
               Uint16 N = length of w, x, and y arrays
 
  RETURNS:     none
 
  BENCHMARK:  
 
  NOTES:
    1) The inputs and return value are of type 'complex_float':
 
       typedef struct {
          float32 dat[2];
       } complex_float;

	Element dat[0] is the real part, dat[1] is the imag part.

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

		.text												# enter text section , 进入代码段				
		.attribute		4, 16								# stack_align 的编号是4 , 16 Byte对齐  
		.attribute		5, "rv32i2p0_m2p0_f2p0_c2p0_xsc"	# arch 的编号是5 ,rv32IMFC  
															# 2P0代表其基本ISA的默认版本  
		.file			"vector_mpyRVbyCV.c"				# emit filename FILE LOCAL symbol table  
		.globl			mpy_SP_RVxCV						# declare global symbol , 声明全局符号  
		.p2align		1									# align to power of 2 ,后续代码按 power(2) 字节对齐  
		.type			mpy_SP_RVxCV,@function				# 将类型设置为函数或对象

mpy_SP_RVxCV:
# a0 is pointer to y[0],  a1 is pointer to w[0], a2 is pointer to x[0], a3 is N
		beqz			a3, .LBB1_3							# if N == 0 ,jump to LBB1_3


# main loop , calculate 	y_re[i] = x[i]*w_re[i]
#							y_im[i] = x[i]*w_im[i]
# .LBB1_2:
		addi			a3, a3, -2
		flw.pm			ft0,a1,4	
		flw.pm			ft1,a2,4
		fmul.s			ft2, ft0, ft1						# ft0 = ft0 * ft1 , y_re[i] = w_re[i] * x[i]
	
		flw.pm			ft0,a1,4
		fmul.s			ft3, ft0, ft1						# ft0 = ft0 * ft1 , y_im[i] = w_im[i] * x[i]
		flw.pm			ft0,a1,4	
		flw.pm			ft1,a2,4
		flw.pm			ft4,a1,4

		rptb			LBB1_2,a3

		fsw.pm			a0,ft2, 4	

		fmul.s			ft2, ft0, ft1						# ft0 = ft0 * ft1 , y_re[i] = w_re[i] * x[i]
		fsw.pm			a0,ft3, 4
		flw.pm			ft0,a1,4
		fmul.s			ft3, ft4, ft1						# ft0 = ft0 * ft1 , y_im[i] = w_im[i] * x[i]
			
		flw.pm			ft1,a2,4
		flw.pm			ft4,a1,4

LBB1_2:
		fsw.pm			a0,ft2, 4	
		fsw.pm			a0,ft3, 4
.LBB1_3:
		ret

# end of function mpy_SP_RVxCV()
# =============================================================================

.Lfunc_end1:
		.size			mpy_SP_RVxCV, .Lfunc_end1-mpy_SP_RVxCV
		.ident			"clang version 14.0.4 (ssh://git_repos@192.168.10.228/home/git_repos/share_repos/llvm-project.git 29f1039a7285a5c3a9c353d054140bf2556d4c4d)"
		.section		".note.GNU-stack","",@progbits
		.addrsig

# =============================================================================
#   End of File
# =============================================================================