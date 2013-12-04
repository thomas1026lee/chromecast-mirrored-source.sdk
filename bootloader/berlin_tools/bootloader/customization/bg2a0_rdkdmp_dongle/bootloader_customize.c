#include "bootloader_customize.h"

static void do_power_tuning()
{
    register unsigned int val;

    lgpl_printf("Power tuning\n");

    /*
     * Turn off PHYs/modules
     */

    // Turn on SATA clock to allow configuring PHY
    val = readl(0xF7EA0150);
    val |= (1<<9);             // SATA
    writel(val, 0xF7EA0150);

    // Power down USB PHY
    //writel(0x00001680, 0xF7B74034); // USB0
    //writel(0x00001680, 0xF7B78034); // USB1

    //writel(0, 0xF7ED0800); // USB0
    //writel(0, 0xF7EE0800); // USB1

    // Turn off SATA PHY 0
    writel(0x00A6, 0xF7E90178);
    writel(0x8122, 0xF7E9017C);
    writel(0x0081, 0xF7E90178);
    writel(0x8901, 0xF7E9017C);

    // Turn off SATA PHY 1
    writel(0x00A6, 0xF7E901F8);
    writel(0x8122, 0xF7E901FC);
    writel(0x0081, 0xF7E901F8);
    writel(0x8901, 0xF7E901FC);

    // Power down ADAC
    writel(4, 0xF7D30020);

    // Power down LVDS
    writel(0x021000FF, 0xF7F70098); // LVDS0
    writel(0x021000FF, 0xF7F700A0); // LVDS1

    // Power down VDAC
    writel(0, 0xF7F7005C);  // VDAC_a
    writel(0, 0xF7F70064);  // VDAC_b
    writel(0, 0xF7F7006C);  // VDAC_c
    writel(0, 0xF7F70074);  // VDAC_d

    /*
     * Turn off clocks
     */

    // clkEnable
    val = readl(0xF7EA0150);

    val &= ~(1<<7);             // geth0
    val &= ~(1<<9);             // SATA
    //val &= ~(1<<11);            // USB0 core
    //val &= ~(1<<12);            // USB1 core
    val &= ~(1<<22);            // audio0clk
    val &= ~(1<<23);            // audio1clk
    val &= ~(1<<24);            // audio2clk
    val &= ~(1<<25);            // audio3clk

    val &= ~(1<<28);            // video1clk
    val &= ~(1<<29);            // video2clk

    writel(val, 0xF7EA0150);

    lgpl_printf("Disable clocks: 0xF7EA0150 - %08X \n", val);

    // gc360ClkCtrl: disable clock
    val  = readl(0xF7EA024C);
    val &= ~1;
    writel(val, 0xF7EA024C);

    lgpl_printf("Disable gc360 clocks: 0xF7EA024C - %08X \n", val);

}

void bootloader_customize_initialize_postprocess(unsigned int boot_state)
{
	int port = 0;
#if 0
	unsigned int uiWarmDown_2_Linux_Addr = readl((SM_WARMDOWN_2_LINUX_ADDR)) ;
	power_on_sdio_slot1();
#endif

	SM_GPIO_PortSetInOut(1, 0);
	SM_GPIO_PortSetInOut(3, 0);
	SM_GPIO_PortSetInOut(5, 0);
	for (port=8; port<16; port++) {
	     SM_GPIO_PortSetInOut(port, 0);
	}

	for (port=0; port<24; port++) {
	     GPIO_PortSetInOut(port, 0);
	}
	for (port=28; port<32; port++) {
	     GPIO_PortSetInOut(port, 0);
	}

#if 0
	if ((boot_state != MV_SoC_STATE_WARMDOWN_1) &&
	     (boot_state != MV_SoC_STATE_WARMDOWN_2) &&
	     !((boot_state == MV_SoC_STATE_WARMUP_1) && (uiWarmDown_2_Linux_Addr != 0xFFFFFFFF)))
	     power_on_sdio_slot0();
#endif

    do_power_tuning();
}

void bootloader_customize_load_image_preprocess(unsigned int boot_state)
{
}

void bootloader_customize_load_image_postprocess(unsigned int boot_state)
{
}

void bootloader_customize_response_to_sm_preprocess(unsigned int boot_state)
{
}

void bootloader_customize_start_kernel_preprocess(unsigned int boot_state)
{
}
