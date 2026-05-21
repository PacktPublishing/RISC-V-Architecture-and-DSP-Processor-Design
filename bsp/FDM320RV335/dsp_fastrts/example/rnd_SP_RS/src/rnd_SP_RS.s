
/*******************************************************************************
 ! \file source/vector/rnd_SP_RS.s
 !
 ! \brief  C-Callable unbiased rounding of a floating point scalar
 ! \author XingYuayuan
 ! \date   06/07/2022
 
  HISTORY:
     06/07/2022 - original ( XingYuayuan )

  FUNCTION:    extern float32 rnd_SP_RS(const float32 x);
 
  USAGE:       y = rnd_SP_RS(x);
 
  PARAMETERS:  float32 x = input value
 
  RETURNS:     float32 y = result
 
  BENCHMARK:  
 
  NOTES:
 
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
	

# TODO ：
#    TI use FRACF32 instruction ,instead of system library

		.text												# enter text section , 进入代码段				
		.attribute		4, 16								# stack_align 的编号是4 , 16 Byte对齐  
		.attribute		5, "rv32i2p0_m2p0_f2p0_c2p0_xsc"		# arch 的编号是5 ,rv32IMFC  
															# 2P0代表其基本ISA的默认版本  
		.file			"vector_roundRS.c"					# emit filename FILE LOCAL symbol table  
		.globl			rnd_SP_RS							# declare global symbol , 声明全局符号  
		.p2align		1									# align to power of 2 ,后续代码按 power(2) 字节对齐  
		.type			rnd_SP_RS,@function					# 将类型设置为函数或对象
				
rnd_SP_RS:
		addi			sp, sp, -16
		sw				ra, 12(sp)
 		call			roundf@plt
		lw				ra, 12(sp)
		addi			sp, sp, 16
		ret

# end of function rnd_SP_RS()
# =============================================================================

.Lfunc_end1:
		.size			rnd_SP_RS, .Lfunc_end1-rnd_SP_RS
		.ident			"clang version 14.0.4 (ssh://git_repos@192.168.10.228/home/git_repos/share_repos/llvm-project.git 29f1039a7285a5c3a9c353d054140bf2556d4c4d)"
		.section		".note.GNU-stack","",@progbits
		.addrsig

# =============================================================================
#   End of File
# =============================================================================