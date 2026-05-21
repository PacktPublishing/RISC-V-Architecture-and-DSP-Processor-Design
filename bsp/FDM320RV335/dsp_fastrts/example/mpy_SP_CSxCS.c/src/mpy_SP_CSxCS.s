/********************************************************************************
 ! \file source/vector/mpy_SP_CSxCS.s
 !
 ! \brief C-Callable complex multiply of two floating point numbers
 ! \author XingYuanyuan
 ! \date   06/02/2022
 
  HISTORY:
    06/02/2022 - original (XingYuanyuan)
 
  DESCRIPTION: C-Callable complex multiply of two floating point numbers
     			y.r = w.r*x.r - w.i*x.i;
    			y.i = w.r*x.i + w.i*x.r;
 
  FUNCTION: 
    extern complex_float mpy_SP_CSxCS(const complex_float w, 
                                      const complex_float x);
 
  USAGE:       y = mpy_SP_CSxCS(w,x);
 
  PARAMETERS:  complex_float w = input #1
               complex_float x = input #2
 
  RETURNS:     complex_float y = result
 
  BENCHMARK:   
               
  NOTES:
    1) This is a highly optimized function.  The order of the instructions
       is very important.
    2) The restore of Save-on-entry register R4H is hidden in a parallel
       instruction after its last use.
    3) The input and return value is of type 'complex_float':
 
       typedef struct {
          float32 dat[2];
       } complex_float;
 
       Element dat[0] is the real part, dat[1] is the imag part.

  Target:            SpringCore
  
 *******************************************************************************/

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
#            t0          £ºTemporary/alternate link register , ÁÙÊ±¼Ä´æÆ÷, ±¸ÓÃ
#            t1 - t6     £ºTemporaries  ,ÁÙÊ±¼Ä´æÆ÷
#            ft0 - ft11  : FP temporaries  ,¸¡µãÁÙÊ±¼Ä´æÆ÷
#            fs0 - fs11  : FP saved registers  ,¸¡µã±£´æ¼Ä´æÆ÷
#            fa0 - fa1   : FP arguments/Return values  ,¸¡µã²ÎÊý/·µ»ØÖµ
#            fa2 - fa7   : FP arguments  ,¸¡µã²ÎÊý
# =============================================================================


		.text													         # enter text section , ½øÈë´úÂë¶Î				
		.attribute			4, 16								      # stack_align µÄ±àºÅÊÇ4 , 16 Byte¶ÔÆë  
		.attribute			5, "rv32i2p0_m2p0_f2p0_c2p0"		# arch µÄ±àºÅÊÇ5 ,rv32IMFC  
																         # 2P0´ú±íÆä»ù±¾ISAµÄÄ¬ÈÏ°æ±¾  
		.file				   "vector_mpyCSbyCS.c"				   # emit filename FILE LOCAL symbol table  
		.globl			   mpy_SP_CSxCS						   # declare global symbol , ÉùÃ÷È«¾Ö·ûºÅ  
		.p2align			   1									      # align to power of 2 ,ºóÐø´úÂë°´ power(2) ×Ö½Ú¶ÔÆë  
		.type				   mpy_SP_CSxCS,@function				# ½«ÀàÐÍÉèÖÃÎªº¯Êý»ò¶ÔÏó 

mpy_SP_CSxCS:
# ft0 is w_re , fa1 is w_im , fa2 is x_re , fa3 is x_im
# Optimally load the input values and do the math ,
#     		y.r = w.r*x.r - w.i*x.i;		
#    			y.i = w.r*x.i + w.i*x.r;

		fneg.s				ft0, fa1							      # ft0 = -fa1 = -w_im
		fmul.s				ft0, ft0, fa3						   # ft0 = ft0 * fa3 = -w_im * x_im
		fmul.s				ft1, fa1, fa2						   # ft1 = fa1 * fa2 = w_im * x_re
		fmadd.s				ft0, fa0, fa2, ft0				   # ft0 = fa0 * fa2 + ft0 = w_re * x_re - w_im * x_im
		fmadd.s				fa1, fa0, fa3, ft1				   # fa1 = fa0 * fa3 + ft1 = w_re * x_im + w_im * x_re
		fmv.s				   fa0, ft0							      # fa0 = ft0 ,store to return values register		

# Finish up
		ret														      # return

# end of function mpy_SP_CSxCS()
# =============================================================================

.Lfunc_end1:
		.size				mpy_SP_CSxCS, .Lfunc_end1-mpy_SP_CSxCS
		.ident				"clang version 14.0.4 (ssh://git_repos@192.168.10.228/home/git_repos/share_repos/llvm-project.git 29f1039a7285a5c3a9c353d054140bf2556d4c4d)"
		.section			".note.GNU-stack","",@progbits
		.addrsig

# =============================================================================
#   End of File
# =============================================================================
