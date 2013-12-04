#define	PROCESSOR_ID_REG_BASE	0xF8430000

.global IRQ_Handler
IRQ_Handler:
//		;STMDB   r13!, {r0 -r3,r14} ;save registers
		PUSH     {r0-r5,r12,lr}
		
//		;check CPU ID
		LDR		r0, =PROCESSOR_ID_REG_BASE
		LDR		r0, [r0]
		AND		r0, r0, #0x03
		CMP		r0, #0					//; if the current is CPU #0
		BNE		GO_TO_CPU1_IRQ
		
		LDR		r0, _IRQ_entry
		BLX		r0
		B		IRQ_DONE
		
GO_TO_CPU1_IRQ:
		LDR		r0, _IRQ_entry
		BLX		r0
		
IRQ_DONE:
		//;restore registers
		//;LDMIA   r13!, {r0 -r3,r14}
		POP      {r0-r5,r12,lr}
		
		//;return from IRQ
		subs	pc, r14, #4             //; return to where I got interrupted from!

_IRQ_entry:
		.word 	GIC_IRQ_Handler


