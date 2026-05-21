	.text
	.attribute		4, 16
	.globl			RV335_usDelay   
	.p2align		2
	.type			RV335_usDelay,@function
RV335_usDelay:
	srli		a1, a0, 3
	addi		a2, a1, -1
	rptb		.LB_end, a2
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
.LB_end:
	ret