/*
 *  Copyright Marvell Semiconductor, Inc. 2006. All rights reserved.
 *
 *  This code initialises the Integrator board (eg REMAP) before calling
 *  TCM Initialization and MMU Initialization if they exist.
 *  this allows scatter loading to relocate code into the TCMs
 *
 *  This code must be run from a privileged mode
 */

#include "memmap.h"
#include "ra_gbl.h"
/*
 * cpu id register
 */
#define PROCESSOR_ID_REG_BASE           (0x00 + MEMMAP_PROCESSOR_ID_REG_BASE)


/*
 * chip control registers
 */
#define CHIP_CTRL_REG_BASE              (0x00 + MEMMAP_CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_PIN_MUX_REG           (RA_Gbl_pinMux + CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_BOOT_STRAP_REG        (RA_Gbl_bootStrap + CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_SYS_PLL_CTL           (RA_Gbl_sysPllCtl + CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_RA_GBL_CLK_SELECT     (RA_Gbl_clkSelect + CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_RA_GBL_CLK_SWITCH     (RA_Gbl_ClkSwitch + CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_RESET_TRIGGER_REG     (RA_Gbl_ResetTrigger + CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_RESET_STATUS_REG      (RA_Gbl_ResetStatus + CHIP_CTRL_REG_BASE)


/*
 * Declear Extra Features Regsiters -R15
 */
#define	R15_NCNB_WAIT			(0x01 << 30)
#define	R15_STREAM_EN			(0x01 << 29)
#define	R15_WR_ALLOC_EN			(0x01 << 28)

/*
 * SATA global HBA control register offset
 * SATA register base is MEMMAP_SATA_REG_BASE
 */
#define SATA_HOST_CTL           (0x004)


.globl BootLoaderEntry
BootLoaderEntry:
		//save r0, r1
		mov		r3, r0
		mov		r4, r1

		//;	    set stack for irq stack
		MRS		r0, cpsr
		BIC		r0, r0, #0x1F       //MODE_MASK
		ORR		r0, r0, #0x12       //IRQ_MODE
		MSR		cpsr, r0            //enter irq mode
		LDR		sp, _irq_stack_top

		//;      back to supervisor mode again
		/*
		 * force to be supervisor mode
		 */
		MRS		r0, cpsr		// load program status register
		BIC		r0, r0, #0x1F		// r0 = r0 & !0x1F
		ORR		r0, r0, #0xD3		// r0 = r0 | 0xD3
		MSR		cpsr_c, r0		// Supervisor with disabled IRQ/FIQ

		LDR		sp, _stack_top                  /* stack base                   */

		/*
		 * setup important register
		 * setup memory timing
		 */

		/*
		* disable MMU/PU stuff and caches
		*/
		MRC		p15, 0, r0, c1, c0, 0
		BIC		r0, r0, #0x00001000             /* clear bits 12 (---I)        */
		BIC		r0, r0, #0x00000005             /* clear bits 2:0 (-D-M)       */
		MCR		p15, 0, r0, c1, c0, 0

		/*
		 * invalidate I/D caches
		 */
		MOV		r0, #0
		MCR		p15, 0, r0, c7, c5, 0		/* Invalidate entire instuction cache	*/
		BL		invalidate_dcache

		BL		InitMultiLevelMMU		/* Initial MMU & enable caches */
	
		/*
		 * branch to C Library entry point
		 */
		mov		r0, r3
		mov		r1, r4
		MOV		lr, pc
		LDR		pc, _c_entry
		SWI		0x123456			/* Termination Sign of ARM applications	*/

_c_entry:
		.word		StartBootLoader

_stack_top:
		.word		__stack_start

_irq_stack_top:
		.word		__stack_start + 0x10000
