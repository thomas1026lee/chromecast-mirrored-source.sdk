//;  Copyright Marvell Semiconductor, Inc. 2006. All rights reserved.
//;
//;  This code initialises the Integrator board (eg REMAP) before calling
//;  TCM Initialization and MMU Initialization if they exist.
//;  this allows scatter loading to relocate code into the TCMs
//;
//;  This code must be run from a privileged mode
#define Access_R_ONLY	ox2
#define	Access_R_W	0x3

.global InitMultiLevelMMU
InitMultiLevelMMU:
	//; --- if MMU/MPU enabled - disable (useful for ARMulator tests)
        MRC     p15, 0, r0, c1, c0, 0		//; read CP15 register 1 into r0
        BIC     r0, r0, #0x1				//; clear bit 0
        MCR     p15, 0, r0, c1, c0, 0		//; write value back


	STMFD	r13!, {r0-r12,r14}

	BL		FLAT_MAP_SECTION
	LDR		r0, ttb
	MCR		p15, 0, r0, c2, c0, 0	//; write TTB address to CP15 register
	LDR		r0, =0x3 //accesses domain #0 are not checked against the access permission
	MCR		p15, 0, r0, c3, c0, 0	//; bits so a permission fault cannot be generated.
	MRC		p15, 0, r0, c1, c0, 0
	ORR		r0, r0, #0x1000		//; icache on
	ORR		r0, r0, #0x4		//; dcache on
	ORR		r0, r0, #0x1		//; mmu on
	MCR		p15, 0, r0, c1, c0, 0

	NOP
	LDMFD	r13!, {r0-r12, r14}
	BX		r14

	//; --- make section descriptor
	//; --- r0 = section mva base
	//; --- r1 = section pa base
	//; --- r2 = AP
	//; --- r3 = domain
	//; --- r4 = c,b bit
	//; --- r7 = coarse L2 page table base
	//; --- return r0 = 1st level descriptor
	//; --- return r1 = 1st level descriptor addr
	//; bit 31-20: section base address
	//; bit 19-12: SBZ
	//; bit 11,10: Access Permission
	//; bit 9:     SBZ
	//; bit 8-5:   domain
	//; bit 4:     1
	//; bit 3:     Cachable
	//; bit 2:     Bufferable
	//; bit 1,0:   10 (section descriptor)

MK_SECTION_DS:
	MOV	r1, r1, LSR #20				//; | clear the last 20 bits
	MOV	r1, r1, LSL #20				//; | section base addr
	ORR	r1, r1, r2, LSL #10			//; | access permission bits
	ORR	r1, r1, r3, LSL #5			//; | domain
	LDR	r3, ttb
	ORR	r1, r1, r4, LSL #2			//; | shift domain
	ORR	r1, r1, #0x2				//; | level 1 entry is section
	MOV	r2, r1
	MOV	r1, r0, LSR #20
	MOV	r1, r1, LSL #2
	MOV	r3, r3, LSR #14
	MOV	r3, r3, LSL #14
	ORR	r1, r3, r1                  //; | get the TTB entry address in r1
	MOV	r0, r2
	BX	r14


	//; --- build flat map secion
FLAT_MAP_SECTION:
	STMFD	r13!, {r2-r9, r14}
	MOV	r5, #0x0
	MOV	r6, #0			//; counter
FLAT_MAP_SECTION_LOOP:
	MOV	r0, r5
	MOV	r1, r5
	MOV	r2, #Access_R_W		//; access permission -- read/write
	MOV	r3, #0x0		//; domain #0
	MOV	r4, #0x0		//; default non-cacheable/non-bufferable
	CMP	r6, #1024		//; first 1 GB address space
	MOVLT	r4, #0x3		//; cacheable/bufferable
	BL	MK_SECTION_DS
	STR	r0, [r1]
	ADD	r5, r5, #0x100000		//; incrememt 1 MB
	ADD	r6, r6, #1
	CMP	r6, #4096			//; 4 GB address space
	BLT	FLAT_MAP_SECTION_LOOP		//; < 4GB
	LDMFD	r13!, {r2-r9, r14}
	BX	r14

//; put ttb to a safe place, table size is 16KB
ttb:		.word	__ttb_base_start 
