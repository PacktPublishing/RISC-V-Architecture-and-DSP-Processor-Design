	
/*******************************************************************************
 ! \file source/vector/mpy_SP_RMxRM.s
 !
 ! \brief  C-Callable multiplication of two real matrices
 ! \author  XingYuanyuan
 ! \date    06/06/2022
 
 HISTORY:s
   06/06/2022 - original (XingYuanyuan)
 
  DESCRIPTION: C-Callable multiplication of two real matrices y[] = w[] * x[]
                 where   w[] is of dimension m x n
                         x[] is of dimension n x p
                         y[] is of dimension m x p
 
  Note that in C, a two-dimensional array has elements stored sequentially 
  down the column. For example, this array:
                              | 1 2 |
                              | 3 4 |
 
  will be stored in memory as [1, 3, 2, 4].  It could be declared in C as 
  x[4] or x[2][2].
      for(i = 0U; i < m; i++)
    	{
    	    for(j = 0U; j < p; j++)
    	    {
    	        *(y + i + j*p) = (fsize_t)0.0;
    	        for(k = 0U; k < n; k++)
    	        {
    	            *(y + i + j*p) += *(w + i + k*m) * *(x + j*n + k);
    	        }
    	    }
    	}
 
  FUNCTION: 
    extern void mpy_SP_RMxRM(float *y, const float *w, const float *x, 
                        const uint16_t m, const uint16_t n, const uint16_t p)
 
  USAGE:       mpy_SP_RMxRM(y, w, x, m, n, p);
 
  PARAMETERS:  float* y = pointer to result matrix
               float* w = pointer to 1st src matrix
               float* x = pointer to 2nd src matrix
               m = dimension m of matrices
               n = dimension n of matrices
               p = dimension p of matrices
 
  RETURNS:     none
 
  BENCHMARK:
 
  NOTES:
  	 There are no restrictions on the values for n, m, and p with this 
     function.

   Target:            SpringCore
  
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
#            t0          锛歍emporary/alternate link register , 涓存椂瀵勫瓨鍣�, 澶囩敤
#            t1 - t6     锛歍emporaries  ,涓存椂瀵勫瓨鍣�
#            ft0 - ft11  : FP temporaries  ,娴偣涓存椂瀵勫瓨鍣�
#            fs0 - fs11  : FP saved registers  ,娴偣淇濆瓨瀵勫瓨鍣�
#            fa0 - fa1   : FP arguments/Return values  ,娴偣鍙傛暟/杩斿洖鍊�
#            fa2 - fa7   : FP arguments  ,娴偣鍙傛暟
# =============================================================================

# optimized :  only optimized inner loop
#	loop -> repeat
#	flw -> flw.pm
# TODO : 
#	loop -> repeat
#   Refer to TI function mpy_SP_RMxRM_2.s , parallel computing

		.text												# enter text section , 杩涘叆浠ｇ爜娈�				
		.attribute		4, 16								# stack_align 鐨勭紪鍙锋槸4 , 16 Byte瀵归綈  
		.attribute		5, "rv32i2p0_m2p0_f2p0_c2p0_xsc"	# arch 鐨勭紪鍙锋槸5 ,rv32IMFC  
															# 2P0浠ｈ〃鍏跺熀鏈琁SA鐨勯粯璁ょ増鏈�  
		.file			"vector_mpyRMbyRM.c"				# emit filename FILE LOCAL symbol table  
		.globl			mpy_SP_RMxRM						# declare global symbol , 澹版槑鍏ㄥ眬绗﹀彿  
		.p2align		1									# align to power of 2 ,鍚庣画浠ｇ爜鎸� power(2) 瀛楄妭瀵归綈  
		.type			mpy_SP_RMxRM,@function				# 灏嗙被鍨嬭缃负鍑芥暟鎴栧璞� 

mpy_SP_RMxRM:
# a0 is pointer to y[0], a1 is pointer to w[0],  a2 is pointer to x[0], 
# a3 is m , a4 is n , a5 is p 
		CSRRWI t0, 0x808, 0x7
		addi			sp, sp, -16							# sp = sp - 16 ,
		sw				s0, 12(sp)							# *(sp + 12) = s0 , Saved register
		beqz			a3, .LBB1_9							# if m == 0 , jump to LBB1_9
		li				a6, 0								# a6 = 0
		slli			a7, a3, 2							# a7 = a3 << 2 = m * 2^2 = m * 4
		slli			t0, a4, 2							# t0 = a4 << 2 = n * 2^2 = n * 4
		fmv.w.x			ft0, zero							# ft0 = zero 
		mv				t5, a4								# t5 = a4 = n
		srli  			t5, t5, 1
		addi			t5,t5,-2
		j				.LBB1_3								# jump to LBB1_3

.LBB1_2:
		addi			a6, a6, 1							# a6 = a6 + 1 , i++
		addi			a1, a1, 4							# a1 = a1 + 4 , *w + 4 * i
		beq				a6, a3, .LBB1_9						# if a6 == a3 ,jump to LBB1_9
															# \if i == m, jump to LBB1_9 , jump out of outer loop

.LBB1_3:
		beqz			a5, .LBB1_2							# if p == 0 , jump to LBB1_2
		li				t1, 0								# t1 = 0
		slli			s0, a6, 2							# s0 = a6 << 2 =  4 * i
		add				t2, a0, s0							# t2 = a0 + s0 = *y + i * 4 
															# increment pointer of y
		mv				t3, a2								# t3 = a2 , pointer to x[i]
		j				.LBB1_6								# jump to LBB1_6

.LBB1_5:
	
		addi			t1, t1, 1							# t1 = t1 + 1 , j++
		add				t3, t3, t0							# t3 = t3 + t0 = *x + 4 * n * j 
															# increment pointer of x
		beq				t1, a5, .LBB1_2						# if t1 == a5 , jump to LBB1_2
															# \if j == p , jump to LBB1_2 ,jump out of middle loop 

.LBB1_6:
		mul				s0, t1, a5							# s0 = t1 * a5 = j * p
		slli			s0, s0, 2							# s0 = s0 * 4 = j * p * 4
		add				t4, t2, s0							# t4 = t2 + s0 =  *y + 4 * i  + j * p * 4 
		 													# 绛変环浜�: *(y + i + j * p)
		sw				zero, 0(t4)							# *(t4 + 0) = zero
		beqz			a4, .LBB1_5							# if a4 == 0 , jump to LBB1_5
															# \if n == 0 , jump to LBB1_5

		mv				t6, t3								# t6 = t3 = *x + 4 * n * j 
		mv				s0, a1								# s0 = a1 , *w + 4 * i
		fmv.s			ft1, ft0							# ft1 = ft0 = 0

		flw				ft2, 0(s0)						
		flw.pm          ft3,t6,4
		fmadd.s			ft1, ft2, ft3, ft1						
														
		add				s0, s0, a7	
		flw				ft2, 0(s0)						
		flw.pm          ft3,t6,4

		rptb			LBB1_8,t5
	
		add				s0, s0, a7	
		fmadd.s			ft1, ft2, ft3, ft1
		nop
		flw				ft2, 0(s0)						
		flw.pm          ft3,t6,4
		add				s0, s0, a7	
		fmadd.s			ft1, ft2, ft3, ft1
		nop
		flw				ft2, 0(s0)						
		flw.pm          ft3,t6,4

	LBB1_8:	
		fmadd.s			ft1, ft2, ft3, ft1																		
		fsw				ft1, 0(t4)		

		j				.LBB1_5								# jump to LBB1_5
		

.LBB1_9:
		lw				s0, 12(sp)							# *( sp + 12) = s0 ,restore s0
		addi			sp, sp, 16							# sp = sp + 16 , pop
		ret													# return

# end of function mpy_SP_RMxRM()
# =============================================================================

.Lfunc_end1:
		.size			mpy_SP_RMxRM, .Lfunc_end1-mpy_SP_RMxRM
		.ident			"clang version 14.0.4 (ssh://git_repos@192.168.10.228/home/git_repos/share_repos/llvm-project.git 	29f1039a7285a5c3a9c353d054140bf2556d4c4d)"
		.section		".note.GNU-stack","",@progbits
		.addrsig
# =============================================================================
#   End of File
# =============================================================================
