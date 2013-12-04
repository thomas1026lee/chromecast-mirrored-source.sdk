#include <memmap.h>
#include <global.h>
#include <io.h>
#include "lgpl_printf.h"

#define OPT_ADDR    (0xF7CC0000 + 0x06A0)
#define BIND_INFO2_0  0x0130
#define BIND_INFO2_1  0x0134
#define BIND_INFO1_0  0x0138
#define BIND_INFO1_1  0x013C

#if defined(PWM)
extern int pwm(int volt);
#elif defined(ONEWIRE)
extern int GpioOneWireVoltCtrl(int vout);
#endif

struct leakage_table {
    int info;
    int volt;
};

static struct leakage_table l_table [] =
{
    {1724, 950},
    {1488, 975},
    {1264, 1000},
    {0,    1025},
    {-1, -1},
};

unsigned int get_leakage_info()
{
    return (readl(OPT_ADDR + BIND_INFO1_1)>>8) & 0xFF;
}

static int IS_A0()
{
    return (readl(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ProductId_ext) == 0xA0) &&
            (!(readl(0xF7CC0000 + 0x06A0 + 0x0134) & 0x40000000));
}

static int IS_A1()
{
    return (readl(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ProductId_ext) == 0xA0) &&
            (readl(0xF7CC0000 + 0x06A0 + 0x0134) & 0x40000000);
}

void pv_comp()
{
    int leakage_info;
    struct leakage_table *p;

    leakage_info = 16*get_leakage_info();

    lgpl_printf("leakage info %d.\n", leakage_info);
    p = l_table;
    while(p->info != -1) {
        if(p->info <= leakage_info)
            break;
        p++;
    }
    if(p->info == -1) {
        lgpl_printf("leakage info is not found in leakage table.\n");
		return;
    }

	if (IS_A0() || IS_A1())
		p->volt = 960;

    lgpl_printf("set voltage to %d.\n", p->volt);

#if defined(PWM)
	if(pwm(p->volt))
		lgpl_printf("pv compensation failed.\n");
#elif defined(ONEWIRE)
	GpioOneWireVoltCtrl(p->volt);
#else
#error "PV compensation mothod is not supportted."
#endif
}
