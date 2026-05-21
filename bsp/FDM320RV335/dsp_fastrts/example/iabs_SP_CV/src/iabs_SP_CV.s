/********************************************************************************
! \file source/vector/iabs_SP_CV.s
!
! \brief   C-Callable inverse absolute value of a complex vector
! \author XingYuanyuan
! \date   05/28/2022

 HISTORY:
   05/27/2022 - original (XingYuanyuan)

 DESCRIPTION:  C-Callable inverse absolute value of a complex vector
				y[i] = 1/(sqrt((x[i].r * x[i].r) + (x[i].i * x[i].i)));

 FUNCTION: 
   extern void iabs_SP_CV(float32 *y, const complex_float *x, const Uint16 N)

 USAGE:       iabs_SP_CV(y, x, N);

 PARAMETERS:  float32 *y = results array
              complex_float *x = input array
              Uint16 N = length of x and y arrays

 RETURNS:     none

 BENCHMARK:
 
 NOTES:
   1) The inputs and return value are of type 'complex_float':

      typedef struct {
         float32 dat[2];
      } complex_float;

	Element dat[0] is the real part, dat[1] is the imag part.

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

# optimized :
#		loop -> repeat
#		flw/fsw -> flw.pm/fsw.pm
# TODO : 
#   	Refer to TI function, parallel computing

		.text												# enter text section , 进入代码段				
		.attribute		4, 16								# stack_align 的编号是4 , 16 Byte对齐  
		.attribute		5, "rv32i2p0_m2p0_f2p0_c2p0_xsc"	# arch 的编号是5 ,rv32IMFC  
															# 2P0代表其基本ISA的默认版本  
		.file			"vector_iabsCV.c"					# emit filename FILE LOCAL symbol table  
		.globl			iabs_SP_CV
		.p2align		1
		.type			iabs_SP_CV,@function
.LCPI1_0:
		.word			0x3f800000							# Emits a word value at the current position.
															# 0011,1111,1000,0000,0000,0000,0000,0000
iabs_SP_CV:
# a0 is pointer to y[0] , a1 is pointer to x[0] , a2 is N
		addi			sp, sp, -32							# sp = sp - 16 , 分配栈帧
															# sp : 栈指针
		sw				ra, 28(sp)							# *(sp + 28) = ra , save to the stack, to store return address
		sw				s0, 24(sp)							# *(sp + 24) = s0 , save to the stack, to store const complex_float *x
		sw				s1, 20(sp)							# *(sp + 20) = s1 , save to the stack, to store float32 *y 
		sw				s2, 16(sp)							# *(sp + 16) = s2 , save to the stack, to store Uint16 N
		fsw				fs0, 12(sp)							# *(sp + 12) = fs0 , 
		beqz			a2, .LBB1_5							# a2 == 0 ? : (pc + LBB1_5) : next
															# \if N == 0, jump to LBB1_5
		mv				s2, a2								# s2 = a2 = N
		mv				a3, a0								# s1 = a0 = y
		lui				a0, %hi(.LCPI1_0)					# compute address of LCPI1_0 , 0011,1111,1000,0000,0000
															# a0 = (address of LCPI1_0) << 12 =  0011,1111,1000,0000,0000,0000,0000,0000 = 0x3f800000
															# hi： use the 20-bit high part of the physical address of the symbol
		flw				fs0, %lo(.LCPI1_0)(a0)				# fs0 = *(a0 + address(LCPI1_0))  = *(a0 + 0)   
															# lo :  use the 12-bit low part of the physical address of the symbol
#		addi			s0, a1, 4							# s0 = a1 + 4 , x 的第2个元素,x_im 


# main loop , calculate y[i] = 1/(sqrt((x[i].r * x[i].r) + (x[i].i * x[i].i)));
.LBB1_2:
		flw				ft0, 4(a1)	
	    flw.pm			ft1, a1,8							# 3	
	    fmul.s			ft0, ft0, ft0						# 	ft0 = ft0 * fto = x[i]_i * x[i]_i 
	    fmadd.s			ft0, ft1, ft1, ft0					# 	ft0 = ft1 * ft1 + ft0 = x[i]_r * x[i]_r + x[i]_i * x[i]_i
		fsqrt.s			fa0, ft0							# 	fa0 = sqrtf (ft0) = sqrt((x[i]_r * x[i]_r) + (x[i]_i * x[i]_i))

		flw				ft3, 4(a1)	
	    flw.pm			ft1, a1,8	

		addi			s2,s2,-2

		rptb			.LBB1_3,s2
					
	    fmul.s			ft0, ft3, ft3						#5 		ft0 = ft0 * fto = x[i]_i * x[i]_i 
	    fmadd.s			ft4, ft1, ft1, ft0					#5  	ft0 = ft1 * ft1 + ft0 = x[i]_r * x[i]_r + x[i]_i * x[i]_i
		fdiv.s			ft2, fs0, fa0						# 		ft0 = fs0 / fa0 , fs0 == 1
		fsqrt.s			fa0, ft4							#17  	fa0 = sqrtf (ft0) = sqrt((x[i]_r * x[i]_r) + (x[i]_i * x[i]_i))
		fsw.pm			a3,ft2,4
		flw				ft3, 4(a1)	
	    flw.pm			ft1, a1,8	
.LBB1_3:													# \if N ≠ 0 ，jump to LBB1_2
		fdiv.s			ft2, fs0, fa0						# ft0 = fs0 / fa0 , fs0 == 1

.LBB1_5:
		lw				ra, 28(sp)							# ra = *(sp + 28) ,restora			
		lw				s0, 24(sp)							# s0 = *(sp + 24) ,restore 
		lw				s1, 20(sp)							# s1 = *(sp + 20) ,restore 
		lw				s2, 16(sp)							# s2 = *(sp + 16) ,restore 
		flw				fs0, 12(sp)							# fs0 = *(sp + 12) ,restore 
		addi			sp, sp, 32							# sp = sp +32
		fsw.pm			a3,ft2,4
		ret													# return

# end of function iabs_SP_CV()
# =============================================================================

.Lfunc_end1:
		.size			iabs_SP_CV, .Lfunc_end1-iabs_SP_CV
		.ident			"clang version 14.0.4 (ssh://git_repos@192.168.10.228/home/git_repos/share_repos/llvm-project.git 	29f1039a7285a5c3a9c353d054140bf2556d4c4d)"
		.section		".note.GNU-stack","",@progbits
		.addrsig

# =============================================================================
#   End of File
# =============================================================================
