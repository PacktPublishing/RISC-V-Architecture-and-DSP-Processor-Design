/********************************************************************************
 ! \file source/vector/mac_SP_RVxCV.s
 !
 ! \brief  C-Callable multiply-and-accumulate of a real vector and a 
               complex vector
 ! \author XingYuanyuan
 ! \date   05/31/2022
 
  HISTORY:
     05/31/2022 - original (XingYuanyuan)
 
  DESCRIPTION: C-Callable multiply-and-accumulate of a real vector and a 
               complex vector
                y_re = sum(x[i]*w_re[i])
                y_im = sum(x[i]*w_im[i])
 
  FUNCTION: 
    extern complex_float mac_SP_RVxCV(const complex_float *w,
                             const float32 *x, const Uint16 N)
 
  USAGE:       y = mac_SP_RVxCV(w, x, N);
 
  PARAMETERS:  complex_float *w = input complex array
               float32 *x  = input real array
               Uint16 N = length of w and x arrays
  
  RETURNS:     complex_float y = complex result

  BENCHMARK:
  
  NOTES:
    1)  The inputs and return value are of type 'complex_float':
  
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
#            ra          锛歳eturn address锛岃繑鍥炲湴鍧�
#            sp          : Stack pointer,鏍堟寚閽�
#            s0 / fp     : Saved register / Frame pointer , 淇濆瓨瀵勫瓨鍣�/甯ф寚閽�
#            s1 - s11    锛歋aved register,淇濆瓨瀵勫瓨鍣�
#            a0 - a1     : Fcuntion arguments/Return values , 鍑芥暟鍙傛暟/杩斿洖鍊�
#            a2 - a7     : Fcuntion arguments , 鍑芥暟鍙傛暟
#            t0          锛歍emporary/alternate link register , 涓存椂瀵勫瓨鍣�, 澶囩敤閾炬帴瀵勫瓨鍣�
#            t1 - t6     锛歍emporaries  ,涓存椂瀵勫瓨鍣�
#            ft0 - ft11  : FP temporaries  ,娴偣涓存椂瀵勫瓨鍣�
#            fs0 - fs11  : FP saved registers  ,娴偣淇濆瓨瀵勫瓨鍣�
#            fa0 - fa1   : FP arguments/Return values  ,娴偣鍙傛暟/杩斿洖鍊�
#            fa2 - fa7   : FP arguments  ,娴偣鍙傛暟
# =============================================================================

# optimized :
#		loop -> repeat
#		flw/fsw -> flw.pm/fsw.pm


		.text												# enter text section , 杩涘叆浠ｇ爜娈�				
		.attribute		4, 16								# stack_align 鐨勭紪鍙锋槸4 , 16 Byte瀵归綈  
		.attribute		5, "rv32i2p0_m2p0_f2p0_c2p0_xsc"	# arch 鐨勭紪鍙锋槸5 ,rv32IMFC  
															# 2P0浠ｈ〃鍏跺熀鏈琁SA鐨勯粯璁ょ増鏈�  
		.file			"vector_macRVbyCV.c"				# emit filename FILE LOCAL symbol table  
		.globl			mac_SP_RVxCV						# declare global symbol , 澹版槑鍏ㄥ眬绗﹀彿  
		.p2align		1									# align to power of 2 ,鍚庣画浠ｇ爜鎸� power(2) 瀛楄妭瀵归綈  
		.type			mac_SP_RVxCV,@function				# 灏嗙被鍨嬭缃负鍑芥暟鎴栧璞� 

mac_SP_RVxCV:
# a0  is pointer to w[0] , a1 is pointer to x[0], a2 is N
		beqz			a2, .LBB1_4							# a2 == 0 ? (pc + LBB1_4) : next , check N is 0
															# \if N == 0 , jump to LBB1_4
		fmv.w.x			fa0, zero							# fa0 = zero = 0
#		addi			a0, a0, 4							# a0 = a0 + 4 , a0 is w_im
		fmv.s			fa1, fa0							# fa1 = fa0 , return values

# main loop ,calculate  y_re = sum(x[i]*w_re[i])
# 			            y_im = sum(x[i]*w_im[i])
# .LBB1_2:
		addi 		   	a2,a2,-1
		flw.pm			ft1,a0,4
		

		rptb			LBB1_2,a2
		flw.pm			ft0,a1,4
		flw.pm			ft2,a0,4
		fmadd.s			fa0, ft0, ft1, fa0					# fa0 = ft0 * ft1 + fa0, y_re += x[i] * w_re[i]
		fmadd.s			fa1, ft0, ft2, fa1					# fa1 = ft0 * ft2 + fa1 ,y_im += x[i] * w_im[i]
		nop
		flw.pm			ft1,a0,4

		LBB1_2:												# return
		ret
# exception handling 
.LBB1_4:
		fmv.w.x			fa1, zero							# fa1 = zero
		fmv.s			fa0, fa1							# fa0 = fa1
		ret													# return

# end of function mac_SP_RVxCV()
# =============================================================================

.Lfunc_end1:
		.size			mac_SP_RVxCV, .Lfunc_end1-mac_SP_RVxCV
		.ident			"clang version 14.0.4 (ssh://git_repos@192.168.10.228/home/git_repos/share_repos/llvm-project.git 29f1039a7285a5c3a9c353d054140bf2556d4c4d)"
		.section		".note.GNU-stack","",@progbits
		.addrsig

# =============================================================================
#   End of File
# =============================================================================
