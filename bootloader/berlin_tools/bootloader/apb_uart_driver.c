#include "com_type.h" /* include <common.h> for __UBOOT__ vsprintf */

#include "galois_common.h"
#include "galois_io.h"
#include "apb_perf_base.h"
#include "apb_uart.h"
#include "apb_uart_drv.h"
#include "ra_gbl.h"
#include "util.h"

#define __UART_OUTPUT_BUF_SIZE 80
static SIGN8 apb_print_buf[__UART_OUTPUT_BUF_SIZE];

/*******************************************************************************
* Function:    APB_UART_init
*
* Description: initialize UART
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   tclk-- UART source clock
*			   baud-- UART baud rate
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void APB_UART_init(UNSG32 channel, UNSG32 tclk, UNSG32 baud)
{

    UNSG32 base = APB_UART_INST0_BASE;
	UNSG32 ulDivisor;
	UNSG32 tmp ;

	if (channel == 0) base = APB_UART_INST0_BASE;
	else if (channel == 1) base = APB_UART_INST1_BASE;
    //UART FIFO control register:FCR(Enable FIFO mode)
    GA_REG_WORD32_WRITE(base+APB_UART_FCR, (3<<6) | (3<<4) | (1<<2) | (1<<1) | (1<<0));

    //UART modem control register:MCR
    GA_REG_WORD32_WRITE(base+APB_UART_MCR, 0);

    //UART line control register: Normal,No parity,1 stop,8 bits and enable baud divider register
    GA_REG_WORD32_WRITE(base+APB_UART_LCR, 0x3 | BIT7);

    //UART control register
    //GA_REG_WORD32_WRITE(base+OFS_UCON, 0x245);
	
    GA_REG_WORD32_READ(base+APB_UART_LCR, &tmp);
#define UART_LCR_DLAB   0x80    /* Divisor latch access bit */

    //UART baud divider register
    ulDivisor = (tclk/baud+8)/16;
	//Before setting i_uart_0_UART_DLL/i_uart_0_UART_DLH, the Divisor latch access bit should be set
	GA_REG_WORD32_WRITE(base+APB_UART_LCR, tmp | UART_LCR_DLAB) ;
    GA_REG_WORD32_WRITE(base+APB_UART_DLL, ulDivisor & 0xff);
    GA_REG_WORD32_WRITE(base+APB_UART_DLH, (ulDivisor >> 8) & 0xff);
	//	After setting i_uart_0_UART_DLL/i_uart_0_UART_DLH, the Divisor latch access bit should be cleared to access other bits
    GA_REG_WORD32_WRITE(base+APB_UART_LCR, tmp & ~UART_LCR_DLAB) ;

    
}
/*******************************************************************************
* Function:    APB_UART_putc
*
* Description: initialize UART
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   c-- output characterPB_UART_init
* Outputs:     none
*
* Return:      none
*******************************************************************************/
void APB_UART_putc(UNSG32 channel, SIGN8 ch)
{
    UNSG32 base = APB_UART_INST0_BASE;
    UNSG32 status, ulReg;

	if (channel == 0) base = APB_UART_INST0_BASE;
	else if (channel == 1) base = APB_UART_INST1_BASE;

    //Enable thr for write? LCR
	GA_REG_WORD32_READ(base+APB_UART_LCR, &ulReg);
    GA_REG_WORD32_WRITE(base+APB_UART_LCR, ulReg & ~(BIT7));
	
    // Wait for Tx FIFO not full(Check THRE bit)
    do
    {
        GA_REG_WORD32_READ(base+APB_UART_LSR, &status);
    }
    while (!(status & BIT5)) ;

    //UART TX data register
    GA_REG_WORD32_WRITE(base+APB_UART_THR, ch);
}

/*******************************************************************************
* Function: APB_UART_tstc
*
* Description: Test if there's input character from UART.
*
* Inputs: channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*
* Outputs: none
*
* Return: 0-- there's no char input
* 		  1-- there's char input 
*******************************************************************************/
UNSG8 APB_UART_tstc(UNSG32 channel)
{
	UNSG32 base = APB_UART_INST0_BASE;
	UNSG32 status;

	if (channel == 0) base = APB_UART_INST0_BASE;
	else if (channel == 1) base = APB_UART_INST1_BASE;
	GA_REG_WORD32_READ(base+APB_UART_LSR, &status);
	if (status & BIT0)
		return 1;
	else
		return 0;
}
/*******************************************************************************
* Function:    APB_UART_getc_nonblock
*
* Description: non blocking mode read charactor from UART
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   c-- output character
* Outputs:     none
*
* Return:      0-- didn't get any character
*			   1-- get one character
*******************************************************************************/
UNSG8 APB_UART_getc_nonblock(UNSG32 channel, UNSG8 *ch)
{
    UNSG32 base = APB_UART_INST0_BASE;
    UNSG32 status, ulReg;
	UNSG32 data;

	if (channel == 0) base = APB_UART_INST0_BASE;
	else if (channel == 1) base = APB_UART_INST1_BASE;    
	GA_REG_WORD32_READ(base+APB_UART_LSR, &status);
    if (status & BIT0) /* If there's char[s] in RBR or FIFO */
    {
    	//Enable rbr for read
		GA_REG_WORD32_READ(base+APB_UART_LCR, &ulReg);
    	GA_REG_WORD32_WRITE(base+APB_UART_LCR, ulReg & ~(BIT7));
		
        GA_REG_WORD32_READ(base+APB_UART_RBR, &data);
		*ch = data & 0xFF;
        return 1;
    }

    return 0;
}
/*******************************************************************************
* Function:    APB_UART_getc
*
* Description: blocking mode read charactor from UART. Wait until get something
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
* Outputs:     none
*
* Return:      return UIN8 value.
*******************************************************************************/
UNSG8 APB_UART_getc(UNSG32 channel)
{
    UNSG8 ch;

    while(!APB_UART_getc_nonblock(channel, &ch));
	return ch;
}
#if 0
/*******************************************************************************
* Function:    APB_UART_write
*
* Description: Write the buffer into UART.
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   buf-- pointer point to the string what will be written to UART
*			   len-- length of the string
* Outputs:     none
*
* Return:      none.
*******************************************************************************/
void APB_UART_write(UNSG32 channel, SIGN8 *buf)
{
	SIGN8 ch;
	UNSG32 len;
	len=strlen(buf);
    while(len-- > 0) 
    {
		ch = *buf++;
		APB_UART_putc(channel, ch );
		if (ch=='\n')
		{
			ch='\r';
			APB_UART_putc(channel, ch );		
		}	
    }
        
}
#endif

/*******************************************************************************
* Function:    APB_UART_read
*
* Description: read the string from UART.
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   buf-- pointer point to the string what will be written to UART
*			   len-- length of the string
* Outputs:     none
*
* Return:      none.
*******************************************************************************/
void APB_UART_read(UNSG32 channel, SIGN8 *buf, UNSG32 len)
{
    while(len-- > 0)
        *buf++ = APB_UART_getc(channel);
}
/*******************************************************************************
* Function:    APB_UART_printf
*
* Description: printf sytle output.print the output to UART
*
* Inputs:      channel-- choose UART0 or UART1, 0 means UART0, 1 means UART1
*			   szFormat-- print format
* Outputs:     none
*
* Return:      none
*******************************************************************************/
#if 0
void APB_UART_printf(UNSG32 channel, UNSG8* szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);
	/*
	 * The uboot has no vsnprintf() implementation itself.
	 * uboot doesn't call APB_UART_printf().
	 */
#if defined(__UBOOT__)
	vsprintf(apb_print_buf, szFormat, args);
#else
	vsnprintf(apb_print_buf, __UART_OUTPUT_BUF_SIZE, szFormat, args);
#endif

	APB_UART_write(channel, apb_print_buf);
	va_end(args);
}
#endif


void set_uart_pinmux()
{
/*
	{
		T32Gbl_pinMux reg;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),&(reg.u32)); // read current pinmux
		// enable GPIO16
		reg.upinMux_gp2 = Gbl_pinMux_gp2_MODE_0 ;
		// gp10, gp11, I2C

		reg.upinMux_gp10 = Gbl_pinMux_gp10_MODE_1;
		reg.upinMux_gp11 = Gbl_pinMux_gp11_MODE_1;
		// gp14-- gp20 : DVIO[]
//		reg.upinMux_gp14 = Gbl_pinMux_gp14_MODE_1;//eswar commented for 100610tag
//		reg.upinMux_gp15 = Gbl_pinMux_gp15_MODE_1;//eswar commented for 100610tag
//		reg.upinMux_gp16 = Gbl_pinMux_gp16_MODE_1;//;eswar
		// gp5, gp6, gp,7, gp8, gp9, ETH 0
		reg.upinMux_gp5 = Gbl_pinMux_gp5_MODE_2;
		reg.upinMux_gp6 = Gbl_pinMux_gp6_MODE_2;
		reg.upinMux_gp7 = Gbl_pinMux_gp7_MODE_2;
		reg.upinMux_gp8 = Gbl_pinMux_gp8_MODE_2;
		reg.upinMux_gp9 = Gbl_pinMux_gp9_MODE_2;
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),(reg.u32));
	}

	{
		T32Gbl_pinMux1 reg;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1),&(reg.u32)); // read current pinmux
		reg.upinMux_gp16 = Gbl_pinMux_gp16_MODE_1;//ESWAR
		reg.upinMux_gp17 = Gbl_pinMux_gp17_MODE_1;
		reg.upinMux_gp18 = Gbl_pinMux_gp18_MODE_1;
		// gp14-- gp20 : DVIO[]
		reg.upinMux_gp19 = Gbl_pinMux_gp19_MODE_1;
		// gp14-- gp20 : DVIO[]
		reg.upinMux_gp20 = Gbl_pinMux_gp20_MODE_1;
		
		// gp25: DV0 H_syc, Vsync, FID
		reg.upinMux_gp25 = Gbl_pinMux_gp25_MODE_1;
		// gp22. gp23: DV1 H_syc, Vsync, FID
		// I2S0 MCLK
		reg.upinMux_gp21 = Gbl_pinMux_gp21_MODE_1;
		// I2S1 MCLK
		//Satya - Modified for 0712 Tag. Now this is DVIO pinmux
		reg.upinMux_gp23 = Gbl_pinMux_gp23_MODE_1;
		// gp22. : DV1 H_syc, Vsync
		reg.upinMux_gp22 = Gbl_pinMux_gp22_MODE_1;
		// gp24  : ETH MDO mode 1
		reg.upinMux_gp24 = Gbl_pinMux_gp24_MODE_1;

		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1),(reg.u32));
	}
	{
		T32Gbl_PadSelect reg;
		GA_REG_WORD32_READ(MEMMAP_CHIP_CTRL_REG_BASE+ RA_Gbl_PadSelect, &(reg.u32));
		reg.uPadSelect_DVIO_OEN = Gbl_PadSelect_DVIO_OEN_Enable; 
		GA_REG_WORD32_WRITE(MEMMAP_CHIP_CTRL_REG_BASE+ RA_Gbl_PadSelect, (reg.u32));
	}
*/	{
#if (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
		T32smSysCtl_SM_GSM_SEL reg;
		GA_REG_WORD32_READ(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL, &(reg.u32));
		// ENABLE SPI2
		// SM_SPI2_SS0N
//		reg.uSM_GSM_SEL_GSM7_SEL = 1; 
		// SM_SPI2_SS1N
//		reg.uSM_GSM_SEL_GSM8_SEL = 1; 
		// SM_SPI2_SS2N
//		reg.uSM_GSM_SEL_GSM0_SEL = 1; 
		// SM_GPIO[5] (power OK), deselect SM_SPI2_SS3N
//		reg.uSM_GSM_SEL_GSM1_SEL = 0; 
		// SM_SPI2_SCLK, SM_SPI2_SDO
//		reg.uSM_GSM_SEL_GSM6_SEL = 1; 

		// Remove other group settings, and only ENABLE UART0 RXD & TXD (GSM4 mode 0).
		// Note: It should sync with global pinmux configurations in bootloader.
		//       And currently, SM uses UART 0 too.
		reg.uSM_GSM_SEL_GSM4_SEL = 0;//eswar changed for 10-06-tag 
		GA_REG_WORD32_WRITE(SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL, (reg.u32));
#endif
	}


}

#define UART_DLL_25M_115200 	0x0E
#define UART_DLH_25M_115200 	0x00
#define UART_DLL_36M_115200 	0x14
#define UART_DLH_36M_115200 	0x00
#define UART_DLL_48M_115200 	0x1A
#define UART_DLH_48M_115200 	0x00
#if 0 //FPGA
#define UART_DLL				UART_DLL_36M_115200
#define UART_DLH				UART_DLH_36M_115200
#else
#define UART_DLL				UART_DLL_25M_115200
#define UART_DLH				UART_DLH_25M_115200
#endif

void init_dbg_port(UNSG32 uart_base)
{
		UNSG32 read;
    // check uart
    BFM_HOST_Bus_Read32((uart_base+0x0c),&read);       // LCR
    read |=0x83;                                       // DLAB bit 7; 8 bit data latch
    BFM_HOST_Bus_Write32( (uart_base+0x0c),read);      // enable DLAB
    // program DLL and DLH

    //BFM_HOST_Bus_Write32( (uart_base+0x00),0x20);      // DLL 5M->9600 baudrate
    //BFM_HOST_Bus_Write32( (uart_base+0x00),0x10);      // DLL 5M->19200
    //BFM_HOST_Bus_Write32( (uart_base+0x00),0x03);      // DLL 5M->115200 baudrate (2.7)
    BFM_HOST_Bus_Write32( (uart_base+0x00),UART_DLL);    // DLL 5M->19200 baudrate  -->Eswar
  //  BFM_HOST_Bus_Write32( (uart_base+0x00),0x1A);
    //BFM_HOST_Bus_Write32( (uart_base+0x00),0x14);       // DLL 6M->19200 baudrate
    BFM_HOST_Bus_Write32( (uart_base+0x04),UART_DLH);        // DLH
    read &=0x7F;
    BFM_HOST_Bus_Write32( (uart_base+0x0c),read);      // disable DLAB

#if UART_BAUD_SETTING_DELAY
	//this delay is needed for baudrate setting after the chip is coming out of reset
	//without the delay, the UART is not functioning the first time and only works
	//after the second time the baudrate is programmed.
    //for (dump = 100; dump > 0; dumpelay--);
    for (dump = 10000; dump > 0; dump--);
#endif

    BFM_HOST_Bus_Write32( (uart_base+0x08),0x01);      // fifo mode
    BFM_HOST_Bus_Read32((uart_base+0x10),&read);

    read &=0xFFFFFFEF;
    BFM_HOST_Bus_Write32( (uart_base+0x10),read);

    BFM_HOST_Bus_Read32((uart_base+0x10),&read);

    BFM_HOST_Bus_Read32((uart_base+0x7c),&read);        // read USR status;0x06
    BFM_HOST_Bus_Read32((uart_base+0x80),&read);        // TFL ; 0x00
/*
	BFM_HOST_Bus_Write32( (uart_base+0x00),0x47);       // G
    BFM_HOST_Bus_Write32( (uart_base+0x00),0x61);       // a
    BFM_HOST_Bus_Write32( (uart_base+0x00),0x6C);       // l
    BFM_HOST_Bus_Write32( (uart_base+0x00),0x6F);       // o
    BFM_HOST_Bus_Write32( (uart_base+0x00),0x69);       // i
    BFM_HOST_Bus_Write32( (uart_base+0x00),0x73);       // s
   */
}

