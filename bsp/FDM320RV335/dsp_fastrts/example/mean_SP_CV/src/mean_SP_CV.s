/********************************************************************************
 ! \file source/vector/mean_SP_CV.s
 !
 ! \brief C-Callable mean of real and imag parts of a complex vector.
 ! \author XingYuanyuan
 ! \date   06/01/2022
 
  HISTORY:s
    06/01/2022 - original (XingYuanyuan)
 
  DESCRIPTION: C-Callable mean of real and imag parts of a complex vector.
               y_re = sum(x_re[i]) / N
               y_im = sum(x_im[i]) / N
  FUNCTION:    
    extern complex_float mean_SP_CV_2(const complex_float *x, const Uint16 N)
 
  USAGE:       y = mean_SP_CV_2(x, N);
 
  PARAMETERS:  complex_float *x = input array
               Uint16 N = length of x array
 
  RETURNS:     complex_float y = result
 
  BENCHMARK:   
 
  NOTES:
    1)The type 'complex_float' is defined as
 
       typedef struct {
          float32 dat[2];
       } complex_float;
 
       Element dat[0] is the real part, dat[1] is the imag part.

    2) The algorithm is mean_re = sum(re)/N
                        mean_im = sum(im)/N
       1/N is first computed and stored off.  The summations of the real
       and imaginary parts are then computed, and multiplied by 1/N. 
	   
 
  Target:            SpringCore
 
********************************************************************************/
 
# =============================================================================
#
# $Copyright: Copyright (C) 2022 FDM TEK ALL RIGHTS RESERVED $
#
# =============================================================================

# =============================================================================
# Calling convention:
#            ra          £ºreturn address£¬·µ»ØµØÖ·
#            sp          : Stack pointer,Õ»Ö¸Õë
#            s0 / fp     : Saved register / Frame pointer , ±£´æ¼Ä´æÆ÷/Ö¡Ö¸Õë
#            s1 - s11    £ºSaved register,±£´æ¼Ä´æÆ÷
#            a0 - a1     : Fcuntion arguments/Return values , º¯Êý²ÎÊý/·µ»ØÖµ
#            a2 - a7     : Fcuntion arguments , º¯Êý²ÎÊý
#            t0          £ºTemporary/alternate link register , ÁÙÊ±¼Ä´æÆ÷, ±¸ÓÃÁ´½Ó¼Ä´æÆ÷
#            t1 - t6     £ºTemporaries  ,ÁÙÊ±¼Ä´æÆ÷
#            ft0 - ft11  : FP temporaries  ,¸¡µãÁÙÊ±¼Ä´æÆ÷
#            fs0 - fs11  : FP saved registers  ,¸¡µã±£´æ¼Ä´æÆ÷
#            fa0 - fa1   : FP arguments/Return values  ,¸¡µã²ÎÊý/·µ»ØÖµ
#            fa2 - fa7   : FP arguments  ,¸¡µã²ÎÊý
# =============================================================================
	

# TODO : 
#	loop -> repeat
#	flw -> flw.pm

		.text													# enter text section , ½øÈë´úÂë¶Î				
		.attribute		4, 16									# stack_align µÄ±àºÅÊÇ4 , 16 Byte¶ÔÆë  
		.attribute		5, "rv32i2p0_m2p0_f2p0_c2p0_xsc"		# arch µÄ±àºÅÊÇ5 ,rv32IMFC  
																# 2P0´ú±íÆä»ù±¾ISAµÄÄ¬ÈÏ°æ±¾  
		.file			"vector_meanCV.c"			   			# emit filename FILE LOCAL symbol table  
		.globl			mean_SP_CV								# declare global symbol , ÉùÃ÷È«¾Ö·ûºÅ  
		.p2align		1										# align to power of 2 ,ºóÐø´úÂë°´ power(2) ×Ö½Ú¶ÔÆë  
		.type			mean_SP_CV,@function					# ½«ÀàÐÍÉèÖÃÎªº¯Êý»ò¶ÔÏó 

.LCPI1_0:
		.word				0x3f800000							# Emits a word value at the current position.
																# 0011,1111,1000,0000,0000,0000,0000,0000
mean_SP_CV:
# a0 is pointer to x[0] ,  a1 is N 
# ft0 is 1 /N , ft1 is y_re , ft2 is y_im
		lui					a2, %hi(.LCPI1_0)					# compute the address of .LCPI1_0
																# a2 = (address of LCPI1_0) << 12 
																# hi£º use the 20-bit high part of the physical address of the symbol
		flw					ft0, %lo(.LCPI1_0)(a2)				# ft0 = *( a2 + address(LCPI1_0) ) = 1
		fcvt.s.wu			ft1, a1								# ft1 = a1	= N			
		fdiv.s				ft0, ft0, ft1						# ft0 = ft0 / ft1 = 1 / N
		beqz				a1, .LBB1_3							# if N = 0 £¬jump to LBB1_3
		fmv.w.x				ft1, zero							# ft1 = zero = 0
#		addi				a0, a0, 4							# a0 = a0 + 4 , a0 is x_im
		srli				a1, a1, 1
		fmv.s				ft2, ft1							# ft2 = ft1 = 0

# main loop , calculate  y_re = sum(x_re[i])
#						 y_im = sum(x_im[i])
# .LBB1_2:
#		flw					ft3, -4(a0)							# ft3 = a0 - 4 = x_re
#		flw					ft4, 0(a0)							# ft4 = a0 + 0 = x_im
		addi				a1,a1,-2

		flw.pm				ft3, a0,4
		flw.pm				ft4, a0,4	
		fadd.s				ft1, ft1, ft3						# ft1 = ft1 + ft3 , y_re += x_re[i]  
		fadd.s				ft2, ft2, ft4						# ft2 = ft2 + ft4 , y_im += x_im[i]	
		flw.pm				ft5, a0,4
		flw.pm				ft6, a0,4	
		flw.pm				ft3, a0,4
		flw.pm				ft4, a0,4
		fadd.s				ft1, ft1, ft5						# ft1 = ft1 + ft3 , y_re += x_re[i]  
		fadd.s				ft2, ft2, ft6						# ft2 = ft2 + ft4 , y_im += x_im[i]	
	
		flw.pm				ft5, a0,4
		flw.pm				ft6, a0,4

		rptb				.LBB1_2,a1

		fadd.s				ft1, ft1, ft3						# ft1 = ft1 + ft3 , y_re += x_re[i]  
		fadd.s				ft2, ft2, ft4						# ft2 = ft2 + ft4 , y_im += x_im[i]	
		nop
		flw.pm				ft3, a0,4
		flw.pm				ft4, a0,4
		fadd.s				ft1, ft1, ft5						# ft1 = ft1 + ft3 , y_re += x_re[i]  
		fadd.s				ft2, ft2, ft6						# ft2 = ft2 + ft4 , y_im += x_im[i]
		nop
		flw.pm				ft5, a0,4
		flw.pm				ft6, a0,4	

.LBB1_2:
		j					.LBB1_4								# jump to LBB1_4

# exception handling 
.LBB1_3:
		fmv.w.x				ft2, zero							# ft2 = 0, y_im = 0
		fmv.s				ft1, ft2							# ft1 = ft2 = 0 , y_re = 0

# multiplied by 1/N , y_re = y_re / N ,y_im = y_im / N
.LBB1_4:
		fmul.s				fa0, ft0, ft1						# fa0 = ft0 * ft1 , fa0 = y_re / N
		fmul.s				fa1, ft0, ft2						# fa1 = ft0 * ft2 , fa0 = y_im / N
		ret														# return


# end of function mean_SP_CV()
# =============================================================================

.Lfunc_end1:
		.size				mean_SP_CV, .Lfunc_end1-mean_SP_CV
		.ident				"clang version 14.0.4 (ssh://git_repos@192.168.10.228/home/git_repos/share_repos/llvm-project.git 29f1039a7285a5c3a9c353d054140bf2556d4c4d)"
		.section			".note.GNU-stack","",@progbits
		.addrsig

# =============================================================================
#   End of File
# =============================================================================
