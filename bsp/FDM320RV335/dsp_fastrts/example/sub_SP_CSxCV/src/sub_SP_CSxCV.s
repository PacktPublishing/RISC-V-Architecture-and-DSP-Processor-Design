/*******************************************************************************
 ! \file source/vector/sub_SP_CSxCV.s
 !
 ! \brief  C-Callable subtraction of a complex scalar from a complex vector
 !           y_re[i] = x_re[i] - c_re
 !           y_im[i] = x_im[i] - c_im
 ! \author XingYuayuan
 ! \date   06/08/2022
 
  HISTORY:
     06/08/2022 - original ( XingYuayuan )

  FUNCTION: 
    extern void sub_SP_CSxCV(complex_float *y, const complex_float *x, 
                             const complex_float c, const Uint16 N)
  
  USAGE:       sub_SP_CSxCV(y, x, c, N);
  
  PARAMETERS:  complex_float *y = result complex array
               complex_float *x = input complex array
               complex_float  c = input complex scalar
               Uint16 N = length of x and y arrays
  
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
		.file			"vector_subCSfromCV.c"				# emit filename FILE LOCAL symbol table  
		.globl			sub_SP_CSxCV						# declare global symbol , 澹版槑鍏ㄥ眬绗﹀彿  
		.p2align		1									# align to power of 2 ,鍚庣画浠ｇ爜鎸� power(2) 瀛楄妭瀵归綈  
		.type			sub_SP_CSxCV,@function				# 灏嗙被鍨嬭缃负鍑芥暟鎴栧璞�

sub_SP_CSxCV:
# a0 is pointer to y[0], a1 is pointer to x[0],  a2 is N 
# fa0 is  c_re , fa1 is c_im
		beqz			a2, .LBB1_3						# if N == 0 ,jump to LBB1_3



		addi			a2, a2,-2	

		flw.pm			ft0, a1,4
		flw.pm			ft1,a1,4
		fsub.s			ft3, ft0, fa0					# ft0 = ft0 - fa0 ,y_re[i] = x_re[i] - c_re
		fsub.s			ft2, ft1, fa1					# ft0 = ft0 - fa1 ,y_im[i] = x_im[i] - c_im

		flw.pm			ft0, a1,4
		flw.pm			ft1,a1,4

		rptb 			LBB1_2,a2
		fsw.pm			a0,ft3,4
		fsw.pm			a0,ft2,4

		fsub.s			ft3, ft0, fa0					# ft0 = ft0 - fa0 ,y_re[i] = x_re[i] - c_re
		fsub.s			ft2, ft1, fa1					# ft0 = ft0 - fa1 ,y_im[i] = x_im[i] - c_im
		nop
		flw.pm			ft0, a1,4
		flw.pm			ft1, a1,4
	
	LBB1_2:
		fsw.pm			a0,ft3,4
		fsw.pm			a0,ft2,4
# return
.LBB1_3:
		ret												# return	

# end of function sub_SP_CSxCV()
# =============================================================================

.Lfunc_end1:
		.size			sub_SP_CSxCV, .Lfunc_end1-sub_SP_CSxCV
		.ident			"clang version 14.0.4 (ssh://git_repos@192.168.10.228/home/git_repos/share_repos/llvm-project.git 29f1039a7285a5c3a9c353d054140bf2556d4c4d)"
		.section		".note.GNU-stack","",@progbits
		.addrsig

# =============================================================================
#   End of File
# =============================================================================
