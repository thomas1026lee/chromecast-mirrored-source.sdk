	.align		2
	.globl		sm_start
sm_start:
	.word		(. + 4)
	.incbin		"sm.bin"
	.globl		sm_end
sm_end:
	.word		.

