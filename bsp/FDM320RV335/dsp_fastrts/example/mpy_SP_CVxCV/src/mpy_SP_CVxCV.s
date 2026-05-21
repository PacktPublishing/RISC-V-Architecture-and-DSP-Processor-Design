 /*******************************************************************************
 ! \file source/vector/mpy_SP_CVxCV.s
 !
 ! \brief C-Callable multiplication of a complex vector and a complex vector
 ! \author XingYuanyuan
 ! \date   06/04/2022
 
  HISTORY:
    06/04/2022 - original (XingYuanyuan)
 
  DESCRIPTION: C-Callable multiplication of a complex vector and a complex 
               vector
               y_re[i] = w_re[i]*x_re[i] - w_im[i]*x_im[i]
               y_im[i] = w_re[i]*x_im[i] + w_im[i]*x_re[i]
 
  FUNCTION: 
    extern void mpy_SP_CVxCV(complex_float *y, const complex_float *w, 
                     const complex_float *x, const Uint16 N)
 
  USAGE:       mpy_SP_CVxCV(y, w, x, N);
 
  PARAMETERS:  complex_float *y = result complex array
               complex_float *w = input complex array #1
               complex_float *x = input complex array #2
               Uint16 N = length of w, x, and y arrays
 
  RETURNS:     none
 
  BENCHMARK:  
 
  NOTES:
    1) The inputs and return value are of type 'complex_float':
 
       typedef struct {
          float32 dat[2];
#		bnez			a3, .LBB1_2						# if N ≠ 0 ,jump to LBB1_2
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
#            t0          ：Temporary/alternate link register , 临时寄存器, 备用
#            t1 - t6     ：Temporaries  ,临时寄存器
#            ft0 - ft11  : FP temporaries  ,浮点临时寄存器
#            fs0 - fs11  : FP saved registers  ,浮点保存寄存器
#            fa0 - fa1   : FP arguments/Return values  ,浮点参数/返回值
#            fa2 - fa7   : FP arguments  ,浮点参数
# =============================================================================


# optimized : 
#	loop -> repeat
#	fsw -> fsw.pm

		.text												# enter text section , 进入代码段				
		.attribute		4, 16								# stack_align 的编号是4 , 16 Byte对齐  
		.attribute		5, "rv32i2p0_m2p0_f2p0_c2p0_xsc"	# arch 的编号是5 ,rv32IMFC  
															# 2P0代表其基本ISA的默认版本  
		.file			"vector_mpyCVbyCV.c"				# emit filename FILE LOCAL symbol table  
		.globl			mpy_SP_CVxCV						# declare global symbol , 声明全局符号  
		.p2align		1									# align to power of 2 ,后续代码按 power(2) 字节对齐  
		.type			mpy_SP_CVxCV,@function				# 将类型设置为函数或对象 

mpy_SP_CVxCV:
# a0 is pointer to y[0], a1 is pointer to w[0] , a2 is pointer to x[0] , a3 is N
		beqz			a3, .LBB1_3						# if N = 0 ,jump to LBB1_3
#		addi			a0, a0, 4						# a0 = a0 + 4 ,y_im

# main loop ,calculate  y_re[i] = w_re[i]*x_re[i] - w_im[i]*x_im[i]
#   					y_im[i] = w_re[i]*x_im[i] + w_im[i]*x_re[i]
# .LBB1_2:
		addi 			a3,a3,-2
		flw.pm			ft0, a1, 4							# ft0 = *(a1 + 0) , w_re
		flw.pm			ft1, a1, 4							# ft1 = *(a1 + 4) , w_im
		flw.pm			ft3, a2, 4						# ft2 = *(a2 + 4) , x_im
		fneg.s			ft4, ft1						# ft4 = -ft1 , -w_im
		flw.pm			ft2, a2, 4						# ft3 = *(a2 + 0) , x_re
		fmul.s			ft4, ft2, ft4					# ft4 =  ft2 * ft4 = x_im[i] *(-w_im[i]) 			
		fmadd.s			ft9, ft0, ft3, ft4				# ft0 = ft0 * ft3 + ft4 = w_re[i] *x_re[i] - x_im[i] * w_im[i]
														# *(a0 - 4) = ft0 ,store y_re[i]

		flw				ft8, -8(a1)						# ft3 = *(a1 + 0) = w_re
		fmul.s			ft6, ft1, ft3					# ft0 = ft1 * ft0 = w_im[i] * x_re[i]
		fmadd.s			ft7, ft8, ft2, ft6				# ft0 = ft3 * ft2 + ft0 = w_re[i] * x_im[i] + w_im[i] * x_re[i]

		flw.pm			ft0, a1, 4							# ft0 = *(a1 + 0) , w_re
		flw.pm			ft1, a1, 4							# ft1 = *(a1 + 4) , w_im
		flw.pm			ft3, a2, 4						# ft2 = *(a2 + 4) , x_im
		fneg.s			ft4, ft1						# ft4 = -ft1 , -w_im
		flw.pm			ft2, a2, 4						# ft3 = *(a2 + 0) , x_re
		flw				ft8, -8(a1)						# ft3 = *(a1 + 0) = w_re


		rptb    		LBB1_2,a3   
					

		fmul.s			ft4, ft2, ft4					# ft4 =  ft2 * ft4 = x_im[i] *(-w_im[i]) 		
		fsw.pm			a0, ft9, 4 	
 		fsw.pm			a0,ft7,4						# *(a0 + 0) = ft0 ,store y_im[i]	
		fmul.s			ft6, ft1, ft3					# ft0 = ft1 * ft0 = w_im[i] * x_re[i]
		fmadd.s			ft9, ft0, ft3, ft4				# ft0 = ft0 * ft3 + ft4 = w_re[i] *x_re[i] - x_im[i] * w_im[i]
														# *(a0 - 4) = ft0 ,store y_re[i]

		flw.pm			ft0, a1, 4							# ft0 = *(a1 + 0) , w_re
		flw.pm			ft1, a1, 4							# ft1 = *(a1 + 4) , w_im
		fmadd.s			ft7, ft8, ft2, ft6				# ft0 = ft3 * ft2 + ft0 = w_re[i] * x_im[i] + w_im[i] * x_re[i]
		fneg.s			ft4, ft1						# ft4 = -ft1 , -w_im
		flw.pm			ft3, a2, 4						# ft2 = *(a2 + 4) , x_im


		flw.pm			ft2, a2, 4						# ft3 = *(a2 + 0) , x_re
		flw				ft8, -8(a1)						# ft3 = *(a1 + 0) = w_re
	LBB1_2:
		fsw.pm			a0, ft9, 4 	
 		fsw.pm			a0,ft7,4						# *(a0 + 0) = ft0 ,store y_im[i]
# return 
.LBB1_3:
		ret												# return
		
# end of function mac_SP_CVxCV()
# =============================================================================

.Lfunc_end1:
		.size			mpy_SP_CVxCV, .Lfunc_end1-mpy_SP_CVxCV
		.ident			"clang version 14.0.4 (ssh://git_repos@192.168.10.228/home/git_repos/share_repos/llvm-project.git 29f1039a7285a5c3a9c353d054140bf2556d4c4d)"
		.section		".note.GNU-stack","",@progbits
		.addrsig

# =============================================================================
#   End of File
# =============================================================================