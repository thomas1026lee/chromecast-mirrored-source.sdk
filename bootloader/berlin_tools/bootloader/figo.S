#include "chipctrl_const.h"
#include "ddr_const.h"
#include "figo_const.h"

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



.global	VerifyImage
VerifyImage:

		/*
		 * r0 -- ARM image source address
		 * r1 -- ARM image size
		 * r2 -- ARM image destination address
		 */
		STMDB		sp!, {r4-r8, lr}
		bl		flush_all_dcache
		/*
		 * check the input key index first, must NOT be 0x00 or 0xFF
		 */
		ADD		r7, r0, #64
		LDR		r8, [r7]
		LDR		r7, =0xFF
		AND		r8, r7
		LDR		r7, =0x0
		CMP		r7, r8
		MOVEQ		r0, #2
		BEQ		GO_OUT

		LDR		r7, =0xFF
		CMP		r7, r8
		MOVEQ		r0, #2
		BEQ		GO_OUT

		/*
		 * waiting for FIGO ready
		 */
FIGO_VERIFY:
		LDR		r7, =DRM_SECURITY_STATUS_REG
		LDR		r7, [r7]
		LDR		r8, =MSK32SECSTATUS_CFG_flag
		AND		r7, r8
		LDR		r8, =SECSTATUS_CFG_flag_DISABLED
		CMP		r7, r8

		BEQ		FIGO_VERIFY

		LDR		r8, =SECSTATUS_CFG_flag_FAILED
		CMP		r7, r8

		MOVEQ		r0, #1
		BEQ		GO_OUT

		/*
		 * check command status
		 */
		LDR		r7, =DRM_ROM_CMD_STAT_REG
		LDR		r7, [r7]
		LDR		r8, =MSK32DRMROM_CMD_STAT_en
		AND		r7, r8
		LDR		r8, =DRM_ROM_CMD_STAT_EN
		CMP		r7, r8

		MOVEQ		r0, #2
		BEQ		GO_OUT

		/*
		 * command type is ARM image
		 */
//;		LDR		r7, =DDR_RANDOM_GENERATOR_REG	// Random Number Generator in DDR
//;		LDR		r8, [r7]
		LDR             r8, =0xFFFFFFFF
		LDR		r7, =MSK32DRMROM_CMD_CMD_CFG_nonce
		AND		r8, r7
		LDR		r7, =DRMROM_CMD_TYPE_LD_ARMIMG
		ORR		r8, r7
		LDR		r7, =DRM_ROM_CMD_CFG_REG
		STR		r8, [r7]

		/*
		 * CRC value
		 */
		LDR		r7, =DRM_ROM_CMD_CRC_REG
		MOV		r8, #0
		STR		r8, [r7]

		/*
		 * Image Size
		 */
		LDR		r7, =DRM_ROM_CMD_IMG_SIZE_REG
		STR		r1, [r7]

		/*
		 * Image Src Address
		 */
		LDR		r7, =DRM_ROM_CMD_IMG_SRC_ADDR_REG
		BIC		r0, #31			// 32 bytes allignment
		STR		r0, [r7]

		/*
		 * Image Dst Address
		 */
		LDR		r7, =DRM_ROM_CMD_IMG_DST_ADDR_REG
		BIC		r2, #31			// 32 bytes alignment
		STR		r2, [r7]

		/*
		 * Kick off FIGO
		 */
		LDR		r7, =DRM_ROM_CMD_STAT_REG
		LDR		r8, =DRM_ROM_CMD_STAT_EN
		STR		r8, [r7]

FIGO_IS_BUSY:
		/*
		 * Waiting for FIGO done
		 */
		LDR		r7, =DRM_ROM_CMD_STAT_REG
		LDR		r7, [r7]
		LDR		r8, =MSK32DRMROM_CMD_STAT_en
		AND		r7, r8
		LDR		r8, =DRM_ROM_CMD_STAT_EN
		CMP		r7, r8

		BEQ		FIGO_IS_BUSY

		/*
		 * Check type
		 */
		LDR		r7, =DRM_ROM_CMD_RESPONSE_CFG_REG
		LDR		r7, [r7]
		LDR		r8, =MSK32DRMROM_CMD_RSP_CFG_tag
		AND		r7, r8
		LDR		r8, =DRMROM_CMD_TYPE_LD_ARMIMG
		CMP		r7, r8

		MOVNE		r0, #3
		BNE		GO_OUT

		/*
		 * Check verification result
		 */
		LDR		r7, =DRM_ROM_CMD_RESPONSE_ERR_REG
		LDR		r7, [r7]
		LDR		r8, =MSK32DRMROM_CMD_RSP_DAT1_error
		AND		r7, r8
		LDR		r8, =DRM_ROM_CMD_RESPONSE_SUCCEED
		CMP		r7, r8

		MOVNE		r0, #4
		BNE		GO_OUT

		bl		flush_all_dcache
		MOV		r0, #0			// return as FIGO success

GO_OUT:
		LDMIA		sp!, {r4-r8, pc}
