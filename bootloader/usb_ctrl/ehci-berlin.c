/*
 * (C) Copyright 2010
 * Marvell Semiconductor <www.marvell.com>
 * Written-by: Qingchao Qiu <qcqiu@marvell.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */
#include <common.h>
#include "usb.h"
#include "ehci.h"
#include "ehci-core.h"
#include "apb_perf_base.h"
#include <io.h>
#include <global.h>
#include "gpio.h"
#include "Firmware_Berlin_BG2CD_A0/ra_gbl.h"

#define USB_PHY_REG_BASE_USB0	0xF7B74000
#define USB_PHY_REG_BASE_USB1	0xF7B78000
#define USB_PHY_PLL_REG			0x04
#define USB_PHY_PLL_CONTROL_REG	0x08
#define USB_PHY_ANALOG_REG		0x34
#define USB_PHY_RX_CTRL_REG             0x20
#define USB_PLL_READY			(1<<15)
#define USBMODE_OFFSET			0x1a8
#define USBMODE_CM_HOST			3

#define USB0_OVERCURRENT_GPIO   0
#define USB0_POWERUP_GPIO 		2
#define USB1_OVERCURRENT_GPIO   3

static void usb_ehci_poweron_device(int on)
{
#ifdef CONFIG_USB1_STORAGE

#ifdef CONFIG_USB0_STORAGE
#error "Do not support to use USB0 and USB1 at a time, please select one.\n"
#endif
	GPIO_PortWrite(USB1_OVERCURRENT_GPIO,on);
#elif defined(CONFIG_USB0_STORAGE)
	GPIO_PortWrite(USB0_OVERCURRENT_GPIO,on);
	GPIO_PortWrite(USB0_POWERUP_GPIO,on);
#endif
}

/*
 * Create the appropriate control structures to manage
 * a new EHCI host controller.
 */
int ehci_hcd_init(void)
{
	u32 base_addr;

	usb_ehci_poweron_device(0);
#ifdef CONFIG_USB1_STORAGE
	{
		u32 temp;
		base_addr = MEMMAP_USB1_REG_BASE;
#if (BERLIN_CHIP_VERSION == BERLIN_BG2CD_A0)
		/* BG2CD use different DLL value */
		ehci_writel(USB_PHY_REG_BASE_USB1 + USB_PHY_PLL_REG, 0x54C0);
#else
		ehci_writel(USB_PHY_REG_BASE_USB1 + USB_PHY_PLL_REG, 0x5560);
#endif
		ehci_writel(USB_PHY_REG_BASE_USB1 + USB_PHY_PLL_CONTROL_REG, 0x2235);
		ehci_writel(USB_PHY_REG_BASE_USB1 + USB_PHY_ANALOG_REG, 0x5680);
		ehci_writel(USB_PHY_REG_BASE_USB1 + USB_PHY_RX_CTRL_REG, 0xAA79); // Set LPF_COEFF to '10'
		/* set USBMODE to host mode */
		temp = ehci_readl(base_addr + USBMODE_OFFSET);
		ehci_writel(base_addr + USBMODE_OFFSET, temp | USBMODE_CM_HOST);

		ehci_writel(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ResetTrigger, 1 << LSb32Gbl_ResetTrigger_usb1SyncReset);
	}
#elif defined(CONFIG_USB0_STORAGE)
	{
		base_addr = MEMMAP_USB0_REG_BASE;
		ehci_writel(USB_PHY_REG_BASE_USB0 + USB_PHY_PLL_REG, 0x5560);
		ehci_writel(USB_PHY_REG_BASE_USB0 + USB_PHY_PLL_CONTROL_REG, 0x2235);
		ehci_writel(USB_PHY_REG_BASE_USB0 + USB_PHY_ANALOG_REG, 0x5680);
		ehci_writel(USB_PHY_REG_BASE_USB0 + USB_PHY_RX_CTRL_REG, 0xAA79); // Set LPF_COEFF to '10'
		ehci_writel(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ResetTrigger, 1 << LSb32Gbl_ResetTrigger_usb0SyncReset);
	}
#else
#error "Please select usb0 or usb1 as your usb storage port.\n"
#endif

	usb_ehci_poweron_device(1);

	hccr = (struct ehci_hccr *)(base_addr + 0x100);
	hcor = (struct ehci_hcor *)((u32) hccr
			+ HC_LENGTH(ehci_readl(&hccr->cr_capbase)));

	debug("Berlin-ehci: init hccr %x and hcor %x hc_length %d\n",
		(u32)hccr, (u32)hcor,
		(u32)HC_LENGTH(ehci_readl(&hccr->cr_capbase)));

	return 0;
}

/*
 * Destroy the appropriate control structures corresponding
 * the the EHCI host controller.
 */
int ehci_hcd_stop(void)
{
	usb_ehci_poweron_device(0);
	return 0;
}
