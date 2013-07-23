#include "io.h"
#include "memmap.h"
#include "global.h"
#include "apb_perf_base.h"
#include "pin_settings.h"
#include "gpio.h"

#if (BERLIN_CHIP_VERSION == BERLIN_BG2CD_A0)
void pin_init()
{
	//writel(0x18209248, (MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux));
	//writel(0, (MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1));

	/*
	 * pinmux
	 */
	T32Gbl_pinMux pinmux0;
	T32Gbl_pinMux1 pinmux1;

	pinmux0.upinMux_gp0 = SOC_GROUP0;

	pinmux0.upinMux_gp1 = SOC_GROUP1;

	pinmux0.upinMux_gp2 = SOC_GROUP2;

	pinmux0.upinMux_gp3 = SOC_GROUP3;

	pinmux0.upinMux_gp4 = SOC_GROUP4;

	pinmux0.upinMux_gp5 = SOC_GROUP5;

	pinmux0.upinMux_gp6 = SOC_GROUP6;

	pinmux0.upinMux_gp7 = SOC_GROUP7;

	pinmux0.upinMux_gp8 = SOC_GROUP8;

	pinmux0.upinMux_gp9 = SOC_GROUP9;

	pinmux0.upinMux_gp10 = SOC_GROUP10;

	pinmux1.upinMux_gp11 = SOC_GROUP11;

	pinmux1.upinMux_gp12 = SOC_GROUP12;

	pinmux1.upinMux_gp13 = SOC_GROUP13;

	pinmux1.upinMux_gp14 = SOC_GROUP14;

	pinmux1.upinMux_gp15 = SOC_GROUP15;

	writel(pinmux0.u32, (MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux));
	writel(pinmux1.u32, (MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1));

}

#else	// BG2

void pin_init()
{
	/*
	 * pinmux
	 */
	T32Gbl_pinMux pinmux_soc;
	T32Gbl_pinMux1 pinmux_soc1;
	T32smSysCtl_SM_GSM_SEL pinmux_sm;
	pinmux_soc.upinMux_gp0 = SOC_GROUP0;

	pinmux_soc.upinMux_gp1 = SOC_GROUP1;

	pinmux_soc.upinMux_gp2 = SOC_GROUP2;

	pinmux_soc.upinMux_gp3 = SOC_GROUP3;

	pinmux_soc.upinMux_gp4 = SOC_GROUP4;

	pinmux_soc.upinMux_gp5 = SOC_GROUP5;

	pinmux_soc.upinMux_gp6 = SOC_GROUP6;

	pinmux_soc.upinMux_gp7 = SOC_GROUP7;

	pinmux_soc.upinMux_gp8 = SOC_GROUP8;

	pinmux_soc.upinMux_gp9 = SOC_GROUP9;

	pinmux_soc.upinMux_gp10 = SOC_GROUP10;

	pinmux_soc.upinMux_gp11 = SOC_GROUP11;

	pinmux_soc.upinMux_gp12 = SOC_GROUP12;

	pinmux_soc1.upinMux_gp13 = SOC_GROUP13;

	pinmux_soc1.upinMux_gp14 = SOC_GROUP14;

	pinmux_soc1.upinMux_gp15 = SOC_GROUP15;

	pinmux_soc1.upinMux_gp16 = SOC_GROUP16;

	pinmux_soc1.upinMux_gp17 = SOC_GROUP17;

	pinmux_soc1.upinMux_gp18 = SOC_GROUP18;

	pinmux_soc1.upinMux_gp19 = SOC_GROUP19;

	pinmux_soc1.upinMux_gp20 = SOC_GROUP20;

	pinmux_soc1.upinMux_gp21 = SOC_GROUP21;

	pinmux_soc1.upinMux_gp22 = SOC_GROUP22;

	pinmux_soc1.upinMux_gp23 = SOC_GROUP23;

	pinmux_soc1.upinMux_gp24 = SOC_GROUP24;

	pinmux_soc1.upinMux_gp25 = SOC_GROUP25;

#if (BERLIN_CHIP_VERSION >= BERLIN_BG2_A0)
#if	defined(EMMC_BOOT)
		pinmux_soc1.upinMux_gp26 = SOC_GP26_EMMC;
#elif defined(NAND_BOOT)
		pinmux_soc1.upinMux_gp26 = SOC_GP26_NAND;
#else
		pinmux_soc1.upinMux_gp26 = SOC_GROUP26;
#endif

	pinmux_soc1.upinMux_gp27 = SOC_GROUP27;

	pinmux_soc1.upinMux_gp28 = SOC_GROUP28;

#endif

	writel(pinmux_soc.u32, (MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux));
	writel(pinmux_soc1.u32, (MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1));

	//def CONFIG_SM // need set SM's pinmux too
	pinmux_sm.uSM_GSM_SEL_GSM0_SEL = SM_GROUP0;

	pinmux_sm.uSM_GSM_SEL_GSM1_SEL = SM_GROUP1;

	pinmux_sm.uSM_GSM_SEL_GSM2_SEL = SM_GROUP2;

	pinmux_sm.uSM_GSM_SEL_GSM3_SEL = SM_GROUP3;

	pinmux_sm.uSM_GSM_SEL_GSM4_SEL = SM_GROUP4;

	pinmux_sm.uSM_GSM_SEL_GSM5_SEL = SM_GROUP5;

	pinmux_sm.uSM_GSM_SEL_GSM6_SEL = SM_GROUP6;

	pinmux_sm.uSM_GSM_SEL_GSM7_SEL = SM_GROUP7;

	pinmux_sm.uSM_GSM_SEL_GSM8_SEL = SM_GROUP8;

	pinmux_sm.uSM_GSM_SEL_GSM9_SEL = SM_GROUP9;

	pinmux_sm.uSM_GSM_SEL_GSM10_SEL = SM_GROUP10;

#if (BERLIN_CHIP_VERSION >= BERLIN_BG2_A0)
	pinmux_sm.uSM_GSM_SEL_GSM11_SEL = SM_GROUP11;
#endif

	writel(pinmux_sm.u32, (SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL));
}

#if 0
void gpio_init()
{
	int i;
	/*
	 * GPIO settings
	 */
	for(i=0; sm_gpio_table[i][0] != -1; i++) {
		SM_GPIO_PortSetInOut(sm_gpio_table[i][0], sm_gpio_table[i][1]);
		SM_GPIO_PortWrite(sm_gpio_table[i][0], sm_gpio_table[i][2]);
//		printf("sm port %d, in/out %d, value: %d\n", sm_gpio_table[i][0], sm_gpio_table[i][1], sm_gpio_table[i][2]);
	}

	for(i=0; soc_gpio_table[i][0] != -1; i++) {
		GPIO_PortSetInOut(soc_gpio_table[i][0], soc_gpio_table[i][1]);
		GPIO_PortWrite(soc_gpio_table[i][0], soc_gpio_table[i][2]);
//		printf("soc port %d, in/out %d, value: %d\n", soc_gpio_table[i][0], soc_gpio_table[i][1], soc_gpio_table[i][2]);
	}
}
#endif
#endif
