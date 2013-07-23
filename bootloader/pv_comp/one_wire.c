/*******************************************************
 *
 *  Control Marvell 88PG877 through 1-wire interface
 *
 ********************************************************/
#include <common.h>
#include "memmap.h"
#include "global.h"
#include "apbRegBase.h"
#include "apb_gpio.h"
#include "ctypes.h"
#include "util.h"
#include "gpio.h"

#define ONEWIRE_GPIO_PORT_NUM   16
#define DELAY_LONG      26
#define DELAY_SHORT     5

#define GPIO_DDR_REG    (APB_GPIO2_BASE + APB_GPIO_SWPORTA_DDR)
#define GPIO_DR_REG     (APB_GPIO2_BASE + APB_GPIO_SWPORTA_DR)

#define PG877_PREAMBLE  0x152
#define PG877_DATA_BIT  18

int g_dr_high, g_dr_low;

static void ndelay()
{
	
        __asm__ __volatile__ (
                "MOV r0, r0"	"\n\t"
				:
				:
				:"r0"
       ) ;		

}

//writing a 16 bit frame out, with start and stop bits, total 18 bits on wire
//For 88PG847, the 16 bits are:
//bit[15,12]: chip select
//bit[11,8]: reg addr
//bit[7, 0]: data field
static void one_wire_write_frame(unsigned int value, unsigned int bit)
{
	unsigned int mask=1<<bit;
	int i=0;
	
    mask >>=1;
	while(mask) {
		if(value&mask) {
            // "1" pulse
            //high
			BFM_HOST_Bus_Write32(GPIO_DR_REG, g_dr_high);	
			for(i=0; i<DELAY_LONG; i++)
				ndelay() ; 

			//low
			BFM_HOST_Bus_Write32(GPIO_DR_REG, g_dr_low);
			for(i=0; i<DELAY_SHORT; i++)
				ndelay() ;

		} else {
            // "0" pulse
            //high
			BFM_HOST_Bus_Write32(GPIO_DR_REG, g_dr_high);
			for(i=0; i<DELAY_SHORT; i++)
				ndelay() ;

			//low
			BFM_HOST_Bus_Write32(GPIO_DR_REG, g_dr_low);
			for(i=0; i<DELAY_LONG; i++)
				ndelay() ; 

		}		
		mask >>=1;
	}
}

static void one_wire_init(void)
{
	T32Gbl_pinMux pinmux_soc;
	unsigned int idx=ONEWIRE_GPIO_PORT_NUM%8;
    unsigned int g_ddr, g_dr;

	// Set as input
	BFM_HOST_Bus_Read32(GPIO_DDR_REG, &g_ddr);
	g_ddr &= ~(1<<idx);
	BFM_HOST_Bus_Write32(GPIO_DDR_REG, g_ddr);

    ndelay();
	one_wire_write_frame(0xAAAA, 2); // dummy write to put code in cache

	// Set as output
	BFM_HOST_Bus_Read32(GPIO_DDR_REG, &g_ddr);
	g_ddr |= (1<<idx);
	BFM_HOST_Bus_Write32(GPIO_DDR_REG, g_ddr);

	//drive low to create idle state
	BFM_HOST_Bus_Read32(GPIO_DR_REG, &g_dr);
	g_dr_low = g_dr & (~(1<<idx));
	g_dr_high = g_dr | (1<<idx);
	BFM_HOST_Bus_Write32(GPIO_DR_REG, g_dr_low);
}

static int percent_table[][2] = {
    {0x8, -100},
    {0x9, -75},
    {0xA, -50},
    {0xB, -25},
    {0xC, 25},
    {0xD, 50},
    {0xE, 75},
    {0xF, 100},
    {-1, -1},
};

int GpioOneWireVoltCtrl(int vout)
{
    int i=0;
	unsigned int data;
    int trim;
	
	if(vout>1099 || vout <900)
		return -1;
	
    //caculate trim
    trim = (vout - 1000); //percent
    if(trim == 0)
    {
        printf("set Vout to 1000mV, default setting by hardware \n");
        return 0;
    }
    while(percent_table[i][0]>0) {
        if(trim <= percent_table[i][1])
            break;
        i++;
    }
    if(percent_table[i][0]<0)
        return -1;

    //vout and percentage
    data = (0x9 << 4) | percent_table[i][0];
    printf("vout_percentage %x, set Vout to %dmV\n", data, 1000+percent_table[i][1]);
    //add preamble
    data |= PG877_PREAMBLE << 8;
    //add stop bit
    data = data<<1 | 1;

    one_wire_init();
	printf("GpioOneWireVoltCtrl, data = 0x%x\n", data);
	one_wire_write_frame(data, PG877_DATA_BIT);
	return 0;
}
