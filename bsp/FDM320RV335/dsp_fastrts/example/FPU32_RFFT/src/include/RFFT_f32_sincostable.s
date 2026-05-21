	/*******************************************************************************
 ! \file  
 !
 ! \brief   
 ! \author XingYuanyuan
 ! \date   08/18/2022

  HISTORY:
    08/18/2022 - original (XingYuanyuan)

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

# optimized:
#  		   uint16_t -> uint32_t

	.text
	.attribute		4, 16
	.data
	.p2align		2                               # -- Begin function RFFT_f32_sincostable
.LCPI0_0:
	.word			0x3bc90fdb                      # float 0.00613592332
	.text	
	.globl			RFFT_f32_sincostable
	.p2align		1
	.type			RFFT_f32_sincostable,@function
RFFT_f32_sincostable:                   # @RFFT_f32_sincostable
# %bb.0:
	addi			sp, sp, -32
	sw				ra, 28(sp)                      # 4-byte Folded Spill
	sw				s0, 24(sp)                      # 4-byte Folded Spill
	sw				s1, 20(sp)                      # 4-byte Folded Spill
	sw				s2, 16(sp)                      # 4-byte Folded Spill
	sw				s3, 12(sp)                      # 4-byte Folded Spill
	fsw				fs0, 8(sp)                      # 4-byte Folded Spill
	lui				a1, %hi(.LCPI0_0)
	flw				fs0, %lo(.LCPI0_0)(a1)
	mv				s2, a0
	li				s1, 0
	li				s0, 0
	li				s3, 1024
.LBB0_1:        	                        # =>This Inner Loop Header: Depth=1
	fcvt.s.w		ft0, s0
	fmul.s			fa0, ft0, fs0
	call			sinf
	lw				a0, 8(s2)
	add				a0, a0, s1
	fsw				fa0, 0(a0)
	addi			s0, s0, 1
	addi			s1, s1, 4
	bne				s0, s3, .LBB0_1
# %bb.2:	
	lw				ra, 28(sp)                      # 4-byte Folded Reload
	lw				s0, 24(sp)                      # 4-byte Folded Reload
	lw				s1, 20(sp)                      # 4-byte Folded Reload
	lw				s2, 16(sp)                      # 4-byte Folded Reload
	lw				s3, 12(sp)                      # 4-byte Folded Reload
	flw				fs0, 8(sp)                      # 4-byte Folded Reload
	addi			sp, sp, 32
	ret
.Lfunc_end0:
	.size		RFFT_f32_sincostable, .Lfunc_end0-RFFT_f32_sincostable
                                        # -- End function
	.ident		"clang version 14.0.4 (ssh://git_repos@192.168.10.228/home/git_repos/share_repos/llvm-project.git 29f1039a7285a5c3a9c353d054140bf2556d4c4d)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
