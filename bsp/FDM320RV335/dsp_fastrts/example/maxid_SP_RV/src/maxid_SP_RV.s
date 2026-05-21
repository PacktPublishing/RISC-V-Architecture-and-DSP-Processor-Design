/********************************************************************************
 ! \file source/vector/maxidx_SP_RV.s
 !
 ! \brief  C-Callable index of maximum value of a real array
 ! \author XingYuanyuan
 ! \date   05/31/2022
 
  HISTORY:
    05/31/2022 - original (XingYuanyuan)
 
  DESCRIPTION: C-Callable index of maximum value of a real array 
				   if(x[i] > max)
				    {
                       max = x[i];
                       idx = i;
                    }
 
  FUNCTION:    Uint16 maxidx_SP_RV(const float32 *x, const Uint16 N);
 
  USAGE:       maxidx_SP_RV_2(x, N);
 
  PARAMETERS:  float32 x = input array
               Uint16 N = length of x
 
  RETURNS:     Uint16 index = index of maximum value in x
 
  BENCHMARK:  
 
  NOTES:
    1)  If more than one instance of the max value exists in x[], the function
       will return the index of the first occurrence (lowest index value).
    2) Register usage:
       R3H = index_candidate
       R2H = index
       R1H = max_candidate
       R0H = max
       XAR4 = pointer to x array
 
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
#		flw -> flw.pm
# TODO : 
#		loop -> repeat
# 		TI 鍦ㄤ竴涓惊鐜腑锛屽仛浜嗕袱娆℃瘮杈�


		.text												# enter text section , 杩涘叆浠ｇ爜娈�
		.attribute		4, 16								# stack_align 鐨勭紪鍙锋槸4 , 16 Byte瀵归綈
		.attribute		5, "rv32i2p0_m2p0_f2p0_c2p0_xsc"	# arch 鐨勭紪鍙锋槸5 ,rv32IMFC
															# 2P0浠ｈ〃鍏跺熀鏈琁SA鐨勯粯璁ょ増鏈�
		.file			"vector_maxidxRV.c"			   		# emit filename FILE LOCAL symbol table  
		.globl			maxidx_SP_RV						# declare global symbol , 澹版槑鍏ㄥ眬绗﹀彿
		.p2align		1									# align to power of 2 ,鍚庣画浠ｇ爜鎸� power(2) 瀛楄妭瀵归綈
		.type			maxidx_SP_RV,@function				# 灏嗙被鍨嬭缃负鍑芥暟鎴栧璞�

maxidx_SP_RV:
		CSRRWI t0, 0x808, 0x7
		beqz			a1, .LBB1_5							# a1 == 0 ? (pc + LBB1_5) : next , check N is 0
															# \if N == 0 , jump to LBB1_5
		li				a2, 0								# a2 = 0 , index_candidate 
															# Pseudoinstruction , Load Immediate
		li				a4, 0								# a4 = 0 , index_candidate
		fmv.w.x			ft0, zero							# ft0 = zero = 0
		fmv.w.x			ft7, zero							# ft0 = zero = 0
		srli			t1, a1, 2
		mv				t2, a0
		mv				t4, a0
		addi			a0, a0, 12
		j				.LBB1_3								# jump to LBB1_3

.LBB1_2:

.LBB1_3:
		flw.pm			ft1,t2,4
		flw.pm			ft4,t2,4
		flw.pm			ft5,t2,4
		flw.pm			ft6,t2,4

		addi			t1, t1, -1
		rptb			LBB1_4, t1
		FMAX.s			ft0, ft0, ft1
		FMAX.s			ft7, ft7, ft4
		nop
		flw.pm			ft1,t2,4
		flw.pm			ft4,t2,4
		FMAX.s			ft0, ft0, ft5
		FMAX.s			ft7, ft7, ft6
		nop
		flw.pm			ft5,t2,4
		flw.pm			ft6,t2,4		
LBB1_4:
		flw.pm			ft1,t4, 4
		FMAX.s			ft0, ft0, ft7
.LBB1_5:
		flt.s			t3, ft1, ft0
		flt.s			t5, ft2, ft0
		flw.pm			ft1,t4, 4
		flw.pm			ft2,t4, 4
		beq				t3, t5, .LBB1_5
		sub				a0, t4, a0							# a0 = a3 << 16 ,i is uint16_t
		srli			a0, a0, 2							# a0 = a0 >> 16
		bnez			t3, .LBB1_7

.LBB1_6:
		addi			a0, a0, -1
		ret			
.LBB1_7:
		ret													# return


# end of function maxidx_SP_RV()
# =============================================================================

.Lfunc_end1:
		.size			maxidx_SP_RV, .Lfunc_end1-maxidx_SP_RV
		.ident			"clang version 14.0.4 (ssh://git_repos@192.168.10.228/home/git_repos/share_repos/llvm-project.git 29f1039a7285a5c3a9c353d054140bf2556d4c4d)"
		.section		".note.GNU-stack","",@progbits
		.addrsig

# =============================================================================
#   End of File
# =============================================================================
