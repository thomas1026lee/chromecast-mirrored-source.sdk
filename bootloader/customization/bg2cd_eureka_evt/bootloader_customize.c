#include "bootloader_customize.h"
#include "galois_speed.h"
#include "apbRegBase.h"

extern void load_cust_key_store();

/*
 * Configure PG821A
 */

static unsigned int l_gpio_reg_ddr = APB_GPIO2_BASE + APB_GPIO_SWPORTA_DDR;
static unsigned int l_gpio_reg_dr  = APB_GPIO2_BASE + APB_GPIO_SWPORTA_DR;
static unsigned int l_gpio_ddr, l_gpio_dr_high, l_gpio_dr_low;

static unsigned int l_cpu_freq = 800;

static void no_op_delay(int ns)
{
	int nop_cycles;

	nop_cycles = l_cpu_freq * ns / 32000;
	while (nop_cycles--) {
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");

		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");

		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");

		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");

		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");

		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");

		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");

		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
		__asm__ __volatile__ ("MOV r0, r0"  "\n\t" :::"r0");
	}
}

inline static void __pg821a_SDI_write_one()
{
	BFM_HOST_Bus_Write32(l_gpio_reg_dr, l_gpio_dr_high);
	no_op_delay(6116);	/* ~6.4us */
	BFM_HOST_Bus_Write32(l_gpio_reg_dr, l_gpio_dr_low);
	no_op_delay(1390);	/* ~1.6us */
}

inline static void __pg821a_SDI_write_zero()
{
	BFM_HOST_Bus_Write32(l_gpio_reg_dr, l_gpio_dr_high);
	no_op_delay(1470);	/* ~1.6us */
	BFM_HOST_Bus_Write32(l_gpio_reg_dr, l_gpio_dr_low);
	no_op_delay(6090);	/* ~6.4us */
}

static void __pg821a_SDI_write(unsigned int reg, unsigned int data)
{
	unsigned int mask=0x8;

	/* start condition */
	BFM_HOST_Bus_Write32(l_gpio_reg_dr, l_gpio_dr_low);
	no_op_delay(16000);

	/* start condition and chip select */
	__pg821a_SDI_write_one();
	__pg821a_SDI_write_zero();
	__pg821a_SDI_write_one();
	__pg821a_SDI_write_zero();
	__pg821a_SDI_write_one();

	/* drive out 4bit reg addr */
	while(mask)
	{
		if(reg & mask)	/* "1" pulse */
			__pg821a_SDI_write_one();
		else		/* "0" pulse */
			__pg821a_SDI_write_zero();

		mask >>=1;
	}

	/* drive out 8bit data */
	mask=0x80;
	while(mask)
	{
		if(data & mask)	/* "1" pulse */
			__pg821a_SDI_write_one();
		else		/* "0" pulse */
			__pg821a_SDI_write_zero();

		mask >>=1;
	}

	/* write stop pulse */
	__pg821a_SDI_write_one();
	no_op_delay(90000);	/* minimux 80us between two write sequence */
}

void pg821a_SDI_write(unsigned int addr,unsigned int data, unsigned int port)
{
	T32Gbl_pinMux1 pinmux1;

	/* output '1' on GPO_20 */
	GPIO_PortWrite(20, 1);

	/* set pinmux to GPO_20 mode */
	pinmux1.u32 = readl(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1);
	pinmux1.upinMux_gp14 = G14_MODE1_IO_GPO_20;
	writel(pinmux1.u32, (MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1));

	/* CPU frequency in MHz */
	l_cpu_freq = GaloisGetFrequency(SOC_FREQ_CPU0) ;

	/* l_gpio_reg_dr: state, l_gpio_reg_ddr: direction */
	switch (port/8)
	{
		case 0:
			l_gpio_reg_dr = APB_GPIO0_BASE;
			break;
		case 1:
			l_gpio_reg_dr = APB_GPIO1_BASE;
			break;
		case 2:
			l_gpio_reg_dr = APB_GPIO2_BASE;
			break;
		case 3:
			l_gpio_reg_dr = APB_GPIO3_BASE;
			break;
		default:
			lgpl_printf("Invalid gpio id\n");
			return;
	}

	l_gpio_reg_ddr = l_gpio_reg_dr + 4;
	port %= 8;

	/* software control */
	BFM_HOST_Bus_Write32((l_gpio_reg_dr + 0x08), 0x00);

	/* set GPIO as input */
	BFM_HOST_Bus_Read32(l_gpio_reg_ddr, &l_gpio_ddr);
	l_gpio_ddr &= ~(1<<port);
	BFM_HOST_Bus_Write32(l_gpio_reg_ddr, l_gpio_ddr);

	BFM_HOST_Bus_Read32(l_gpio_reg_dr, &l_gpio_ddr);
	l_gpio_dr_high = l_gpio_ddr | (1<<port);
	l_gpio_dr_low  = l_gpio_ddr & ~(1<<port);

	/* dummy code to put in cache */
	no_op_delay(5000);
	__pg821a_SDI_write(0xFF,0xFF);

	/* set GPIO as output */
	BFM_HOST_Bus_Read32(l_gpio_reg_ddr, &l_gpio_ddr);
	l_gpio_ddr |= (1<<port);
	BFM_HOST_Bus_Write32(l_gpio_reg_ddr, l_gpio_ddr);

	/* drive low to create idle state */
	BFM_HOST_Bus_Write32(l_gpio_reg_dr, l_gpio_dr_low);

	__pg821a_SDI_write(addr,data);

	/* output '1' on GPO_20 - for old board which still has
           this pin connected to NAND */
	GPIO_PortWrite(20, 1);
}

void bootloader_customize_initialize_postprocess(unsigned int boot_state)
{
	/* 3.24V I/O */
	pg821a_SDI_write(1, 0x13, 20);

	/* Adjust SDIO driven strength */
	T32Gbl_PadSelect1 reg_padsel_1;
	T32Gbl_PadSelect2 reg_padsel_2;

	GA_REG_WORD32_READ(MEMMAP_CHIP_CTRL_REG_BASE+ RA_Gbl_PadSelect1, &(reg_padsel_1.u32));
	reg_padsel_1.uPadSelect_SD0_ZN = 4;
	GA_REG_WORD32_WRITE(MEMMAP_CHIP_CTRL_REG_BASE+ RA_Gbl_PadSelect1, (reg_padsel_1.u32));

	GA_REG_WORD32_READ(MEMMAP_CHIP_CTRL_REG_BASE+ RA_Gbl_PadSelect2, &(reg_padsel_2.u32));
	reg_padsel_2.uPadSelect_SD0_ZP = 4;
	GA_REG_WORD32_WRITE(MEMMAP_CHIP_CTRL_REG_BASE+ RA_Gbl_PadSelect2, (reg_padsel_2.u32));

	/* 1.8V SDIO */
	unsigned int reg_val;
	GA_REG_WORD32_READ(0xF7CC07D4, &(reg_val));
	reg_val = (reg_val >> 29) & 3;  /* bits [30:29] */

	if(reg_val == 1)
	{
		T32Gbl_PadSelect reg_padsel;

		GA_REG_WORD32_READ(MEMMAP_CHIP_CTRL_REG_BASE+ RA_Gbl_PadSelect, &(reg_padsel.u32));
		reg_padsel.uPadSelect_SD0_V18EN = Gbl_PadSelect_SD0_V18EN_V1P8;
		GA_REG_WORD32_WRITE(MEMMAP_CHIP_CTRL_REG_BASE+ RA_Gbl_PadSelect, (reg_padsel.u32));
	}
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
