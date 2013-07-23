#ifndef _CHIPCTRL_CONST_H_
#define _CHIPCTRL_CONST_H_

#include "memmap.h"
#include "ra_gbl.h"
#include "mem_ctrl.h"

#define	CHIP_CTRL_REG_BASE		(0x00 + MEMMAP_CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_PIN_MUX_REG		(RA_Gbl_pinMux + CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_BOOT_STRAP_REG	(RA_Gbl_bootStrap + CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_SYS_PLL_CTL		(RA_Gbl_sysPllCtl + CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_RA_GBL_CLK_SELECT	(RA_Gbl_clkSelect + CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_RA_GBL_CLK_SWITCH	(RA_Gbl_ClkSwitch + CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_RESET_TRIGGER_REG	(RA_Gbl_ResetTrigger + CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_RESET_STATUS_REG	(RA_Gbl_ResetStatus + CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_SW_GENERIC0_REG	(RA_Gbl_sw_generic0 + CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_SW_GENERIC1_REG	(RA_Gbl_sw_generic1 + CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_SW_GENERIC2_REG	(RA_Gbl_sw_generic2 + CHIP_CTRL_REG_BASE)
#define	CHIP_CTRL_SW_GENERIC3_REG	(RA_Gbl_sw_generic3 + CHIP_CTRL_REG_BASE)


#define	CHIP_CTRL_BOOT_SRC_MASK		MSK32Gbl_bootStrap_bootSrc
#define	CHIP_CTRL_BOOT_SRC_SPI		(0x03 << LSb32Gbl_bootStrap_bootSrc)
#define	CHIP_CTRL_BOOT_SRC_NAND		(0x01 << LSb32Gbl_bootStrap_bootSrc)
#define	CHIP_CTRL_BOOT_SRC_ROM		(0x00 << LSb32Gbl_bootStrap_bootSrc)
#define	CHIP_CTRL_BOOT_ENG_EN		(0x01 << LSb32Gbl_bootStrap_ENG_EN)

#define	CHIP_CTRL_BOOT_SW_STRAP_MASK	MSK32Gbl_bootStrap_softwareStrap
#define	CHIP_CTRL_BOOT_SW_RESERVED	(0x01 << LSb32Gbl_bootStrap_softwareStrap)
#define	CHIP_CTRL_BOOT_SW_DDR3		(0x02 << LSb32Gbl_bootStrap_softwareStrap)
#define	CHIP_CTRL_BOOT_SW_TEST_DDR	(0x04 << LSb32Gbl_bootStrap_softwareStrap)

#define	CHIP_CTRL_CHIP_RESET		bGbl_ResetStatus_ChipResetStatus
#define	CHIP_CTRL_CHIP_RESET_MASK	MSK32Gbl_ResetStatus_ChipResetStatus
#define	CHIP_CTRL_CHIP_RESET_WD0	MSK32Gbl_ResetStatus_wd0Status
#define	CHIP_CTRL_CHIP_RESET_WD1	MSK32Gbl_ResetStatus_wd1Status
#define	CHIP_CTRL_CHIP_RESET_WD2	MSK32Gbl_ResetStatus_wd2Status
#define	CHIP_CTRL_CHIP_RESET_WD_MASK	(CHIP_CTRL_CHIP_RESET_WD0 | CHIP_CTRL_CHIP_RESET_WD1 | CHIP_CTRL_CHIP_RESET_WD2)

#define	CHIP_CTRL_CPU0_SYNC_RESET	(bGbl_ResetTrigger_cpu0SyncReset << LSb32Gbl_ResetTrigger_cpu0SyncReset)
#define	CHIP_CTRL_CPU0_SYNC_MASK	MSK32Gbl_ResetTrigger_cpu0SyncReset

#define	CHIP_CTRL_CPU1_SYNC_RESET	(bGbl_ResetTrigger_cpu1SyncReset << LSb32Gbl_ResetTrigger_cpu1SyncReset)
#define	CHIP_CTRL_CPU1_SYNC_MASK	MSK32Gbl_ResetTrigger_cpu1SyncReset

#define	CHIP_CTRL_SYS_PLL_LOCK		(bGbl_sysPllCtl_sysPllPu << LSb32Gbl_sysPllCtl_sysPllPu)

#define	CHIP_CTRL_CPU0CLKSEL		(0x01 << LSb32Gbl_clkSelect_cpu0ClkSel)
#define	CHIP_CTRL_CPU1CLKSEL		(0x01 << LSb32Gbl_clkSelect_cpu1ClkSel)
#define	CHIP_CTRL_SYSCLKSEL		(0x02 << LSb32Gbl_clkSelect_sysClkSel)
#define	CHIP_CTRL_CFGCLKSEL		(0x05 << LSb32Gbl_clkSelect_cfgClkSel)
#define	CHIP_CTRL_GFXCLKSEL		(0x02 << LSb32Gbl_clkSelect_gfxClkSel)
#define	CHIP_CTRL_PERIFCLKSEL		(0x05 << LSb32Gbl_clkSelect_perifClkSel)
#define	CHIP_CTRL_AUDIO0CLKSEL		(0x01 << LSb32Gbl_clkSelect_audio0ClkSel)
#define	CHIP_CTRL_AUDIO1CLKSEL		(0x01 << LSb32Gbl_clkSelect_audio1ClkSel)
#define	CHIP_CTRL_AUDIO2CLKSEL		(0x01 << LSb32Gbl_clkSelect_audio2ClkSel)
#define	CHIP_CTRL_AUDIO3CLKSEL		(0x01 << LSb32Gbl_clkSelect_audio3ClkSel)
#define	CHIP_CTRL_VIDEO0CLKSEL		(0x01 << LSb32Gbl_clkSelect_video0ClkSel)
#define	CHIP_CTRL_VIDEO1CLKSEL		(0x01 << LSb32Gbl_clkSelect_video1ClkSel)
#define	CHIP_CTRL_CLKSEL		(CHIP_CTRL_CPU0CLKSEL | CHIP_CTRL_CPU1CLKSEL | CHIP_CTRL_SYSCLKSEL | CHIP_CTRL_CFGCLKSEL |	\
					 CHIP_CTRL_GFXCLKSEL | CHIP_CTRL_PERIFCLKSEL | CHIP_CTRL_AUDIO0CLKSEL |				\
					 CHIP_CTRL_AUDIO1CLKSEL | CHIP_CTRL_AUDIO2CLKSEL | CHIP_CTRL_AUDIO3CLKSEL |			\
					 CHIP_CTRL_VIDEO0CLKSEL | CHIP_CTRL_VIDEO1CLKSEL)

#define	CHIP_CTRL_SYSCLLPASS		(0x00 << LSb32Gbl_ClkSwitch_sysPLLpass)
#define	CHIP_CTRL_VPROPLLPASS		(0x00 << LSb32Gbl_ClkSwitch_vproPLLpass)
#define	CHIP_CTRL_CPU0CLKSWITCH		(0x01 << LSb32Gbl_ClkSwitch_cpu0ClkSwitch)
#define	CHIP_CTRL_CPU0CLKD3SWITCH	(0x00 << LSb32Gbl_ClkSwitch_cpu0ClkD3Switch)
#define	CHIP_CTRL_CPU1CLKSWITCH		(0x01 << LSb32Gbl_ClkSwitch_cpu1ClkSwitch)
#define	CHIP_CTRL_CPU1CLKD3SWITCH	(0x00 << LSb32Gbl_ClkSwitch_cpu1ClkD3Switch)
#define	CHIP_CTRL_SYSCLKSWITCH		(0x01 << LSb32Gbl_ClkSwitch_sysClkSwitch)
#define	CHIP_CTRL_SYSD3CLKSWITCH	(0x00 << LSb32Gbl_ClkSwitch_sysClkD3Switch)
#define	CHIP_CTRL_CFGCLKSWITCH		(0x01 << LSb32Gbl_ClkSwitch_cfgClkSwitch)
#define	CHIP_CTRL_CFGD3CLKSWITCH	(0x00 << LSb32Gbl_ClkSwitch_cfgClkD3Switch)
#define	CHIP_CTRL_GFXCLKSWITCH		(0x01 << LSb32Gbl_ClkSwitch_gfxClkSwitch)
#define	CHIP_CTRL_GFXD3CLKSWITCH	(0x00 << LSb32Gbl_ClkSwitch_gfxClkD3Switch)
#define	CHIP_CTRL_PRIFCLKSWITCH		(0x01 << LSb32Gbl_ClkSwitch_perifClkSwitch)
#define	CHIP_CTRL_PRIFD3CLKSWITCH	(0x00 << LSb32Gbl_ClkSwitch_perifClkD3Switch)
#define	CHIP_CTRL_PCUBECLKSWITCH	(0x01 << LSb32Gbl_ClkSwitch_pCubeClkSwitch)
#define	CHIP_CTRL_VSCOPCLKSWITCH	(0x01 << LSb32Gbl_ClkSwitch_vScopeClkSwitch)
#define	CHIP_CTRL_CLKSWITCH		(CHIP_CTRL_SYSCLLPASS | CHIP_CTRL_VPROPLLPASS | CHIP_CTRL_CPU0CLKSWITCH	|			\
					 CHIP_CTRL_CPU1CLKSWITCH | CHIP_CTRL_SYSCLKSWITCH | CHIP_CTRL_CFGCLKSWITCH |			\
					 CHIP_CTRL_GFXCLKSWITCH | CHIP_CTRL_PRIFCLKSWITCH | CHIP_CTRL_PCUBECLKSWITCH |			\
					 CHIP_CTRL_VSCOPCLKSWITCH)

#endif
