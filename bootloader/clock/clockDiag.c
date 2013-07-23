//#include "Diag.h"
#include "io.h"
//#include "pllDiag.h"
#include "memmap.h"
#include "global.h"
//#include "clockDiag.h"

#ifndef BOOTLOADER
#define UBOOT
#endif

#define dbg_printf(a, ...) 
enum divider_index
{
	DIVIDED_BY_2 = 1,
	DIVIDED_BY_4,
	DIVIDED_BY_6,
	DIVIDED_BY_8,
	DIVIDED_BY_12
};
#if 0
const char *AVPLL[] =
{
	"AVPllB4",
	"AVPllB5",
	"AVPllB6",
	"AVPllB7",
};

float VcoDiv[] = 
{
	1,		/// * 0000: 1
	1.5,	/// * 0001: 1.5
	2,		/// * 0010: 2
	2.5,	/// * 0011: 2.5
	3,		/// * 0100: 3
	4,		/// * 0101: 4
	5,		/// * 0110: 5
	6,		/// * 0111: 6
	8,		/// * 1000: 8
};
#endif
enum Clocks
{
	SYSCLK,
	DRMCLK,
	CFGCLK,
	GFXCLK,
	ZSPCLK,
	PERIFCLK,
	PCUBECLK,
	VSCOPECLK,
	NFCECCCLK,
	VPPSYSCLK,
	APPCLK,
	GFX3DCORECLK,
	GFX3DSYSCLK,
	ARCREFCLK,
//	HDMIRXFPLLREFCLK,
//	HDMIRXTCLK,
	VIPCLK,
	SDIOXINCLK,
	SDIO1XINCLK,
	GFX3DEXTRACLK,
	GC360CLK,
	NONCLK,
};

#if defined(UBOOT)
const char* g_aClocks[] =
{
	"sysClk",
	"drmClk",
	"cfgClk",
	"gfxClk",
	"zspClk",
	"perifClk",
	"pCubeClk",
	"vScopeClk",
	"nfcEccClk",
	"vppSysClk",
	"appClk",
	"gfx3DCoreClk",
	"gfx3DSysClk",
	"arcRefClk",
//	"hdmirxFpllRefClk",
//	"hdmirxTclk",
	"vipClk",
	"sdioXinClk",
	"sdio1XinClk",
	"gfx3DExtraClk",
	"gc360Clk"
};
#endif

#if 0
enum ClockDiagCmds
{
	// Exit submenu
	CMD_CLOCK_EXIT,
	CMD_CLOCK_HELP,
	// Tests
	CMD_CLOCK_LIST_AVPLL_VCO_FREQUENCIES,
	CMD_CLOCK_SET_AVPLL_B,
	CMD_CLOCK_CHANGE_CPUCLK,
	CMD_CLOCK_CHANGE_CPUPLL,
	CMD_CLOCK_LIST_INDICES,
	CMD_CLOCK_CHANGE_OTHER_CLOCK,
	CMD_CLOCK_CHANGE_SYSPLL,
	CMD_CLOCK_MAX,
	CMD_CLOCK_UNKNOWN
};

static const char* g_aClockCmds[] = 
{
	"EXIT",
	"HELP",
	"APVCO",
	"AVPLLB",
	"CPUCLK",
	"CPUPLL",
	"LIST",
	"OTHER",
	"SYSPLL"
};

static const char* g_aClockCmdHelp[] =
{
	"N/A",
	"[]; Help",
	"[]; list AVPLL VCO frequencies",
	"[Channel] [freq_index]; set AVPLL_B frequency",
	"[PllSwitch] [PllSelect] [div]; change cpuClk source and divider",
	"[frequency]; change cpuPll frequency",
	"[]; list clock indices",
	"[index] [PllSwitch] [PllSelect] [div]; change source and divider of other clocks",
	"[frequency]; change sysPll frequency",
};

const unsigned int clock_divider[] =
{
	1,
	2,
	4,
	6,
	8,
	12,
	1,
	1
};

int speed_cpu;
unsigned int RefClkIn = 25;
unsigned int RefDiv = 2;
extern float current_freq[2][8];

int parseClockCmd(char *pCmd)	// Pointer to a text string that contains the command and its arguments.
{
	int cmdIndex;
	unsigned int data1, data2, data3, data4;
	unsigned int result;
	int iResult = 0;
	int i;

	// Find the command index based on user input
	skipSpace( &pCmd );

	// Ignore blank command and/or comment only lines.
	if (( *pCmd == '\0' ) || ( *pCmd == ';' ))
		return 0;

	// Parse the command.
	cmdIndex = findCmdIndex( &pCmd, g_aClockCmds, (int)CMD_CLOCK_MAX );

	// Handle the case where a command is entered without arguments
	switch ( cmdIndex )
	{
		case CMD_CLOCK_LIST_AVPLL_VCO_FREQUENCIES:
			dbg_printf(PRN_RES, " AVPLL VCO frequencies\n");
			for (i = 0; i < 6; i++)
				dbg_printf(PRN_RES, "  %d: %f\n", i, diag_vcoFreqs[i]);
			dbg_printf(PRN_RES, " use AVPLL LSCLK command to find out available frequencies\n");
			break;
		case CMD_CLOCK_SET_AVPLL_B:
			data1 = 7;						// Channel
			data2 = 0;						// Index
			skipSpace( &pCmd );
			if ( *pCmd != '\0' )
				pCmd = getIntToken( pCmd, &data1 );
			skipSpace( &pCmd );
			if ( *pCmd != '\0' )
				pCmd = getIntToken( pCmd, &data2 );

			iResult = diag_clockFreq_B(data2, data1);

			dbg_printf(PRN_RES, " Setting AVPLL_B%d to index %d", data1, data2);
			if (!iResult)
				dbg_printf(PRN_RES, " Passed.\n");
			else
				dbg_printf(PRN_RES, " Failed.\n Please pick a different frequency scheme\n");
			break;
		case CMD_CLOCK_CHANGE_CPUCLK:
			data1 = 0;		// PllSwitch
			data2 = 0;		// PllSelect
			data3 = 1;		// Divider
			skipSpace( &pCmd );
			if ( *pCmd != '\0' )
				pCmd = getIntToken( pCmd, &data1 );
			skipSpace( &pCmd );
			if ( *pCmd != '\0' )
				pCmd = getIntToken( pCmd, &data2 );
			skipSpace( &pCmd );
			if ( *pCmd != '\0' )
				pCmd = getIntToken( pCmd, &data3 );

			if (data1 > 1 || data2 > 4)
			{
				iResult = 1;
				dbg_printf(PRN_ERR, " invalid parameter\n");
			}
			else if ((data3 != 1) && (data3 != 2) && (data3 != 3) && (data3 != 4) && (data3 != 6) && (data3 != 8) && (data3 != 12))
			{
				iResult = 1;
				dbg_printf(PRN_ERR, " invalid divider\n");
			}
			else
			{
				diag_clock_change_cpuClk(data1, data2, data3);

				dbg_printf(PRN_RES, "  Changed cpuClk, now\n");
				list_speed(LIST_ALL_SPEEDS);
			}
			break;
		case CMD_CLOCK_CHANGE_CPUPLL:
			data1 = 400;	// Frequency
			skipSpace( &pCmd );
			if ( *pCmd != '\0' )
				pCmd = getIntToken( pCmd, &data1 );

			result = diag_clock_change_cpuPll(data1);
			dbg_printf(PRN_RES, " cpuPll set to %d\n", result);
			break;
		case CMD_CLOCK_LIST_INDICES:
			for (i = 0; i < NONCLK; i++)
				dbg_printf(PRN_RES, " %-2d: %s\n", i, g_aClocks[i]);
			break;
		case CMD_CLOCK_CHANGE_OTHER_CLOCK:
			data1 = 0;		// Index
			data2 = 0;		// PllSwitch
			data3 = 0;		// PllSelect
			data4 = 1;		// Divider
			skipSpace( &pCmd );
			if ( *pCmd != '\0' )
				pCmd = getIntToken( pCmd, &data1 );
			skipSpace( &pCmd );
			if ( *pCmd != '\0' )
				pCmd = getIntToken( pCmd, &data2 );
			skipSpace( &pCmd );
			if ( *pCmd != '\0' )
				pCmd = getIntToken( pCmd, &data3 );
			skipSpace( &pCmd );
			if ( *pCmd != '\0' )
				pCmd = getIntToken( pCmd, &data4 );

			if ((data1 >=  NONCLK)|| (data2 > 1) || (data3 > 4))
			{
				iResult = 1;
				dbg_printf(PRN_ERR, " invalid parameter\n");
			}
			else if ((data4 != 1) && (data4 != 2) && (data4 != 3) && (data4 != 4) && (data4 != 6) && (data4 != 8) && (data4 != 12))
			{
				iResult = 1;
				dbg_printf(PRN_ERR, " invalid divider\n");
			}
			else
			{
				diag_clock_change_otherClk(data1, data2, data3, data4);

				dbg_printf(PRN_RES, "  Changed %s, now\n", g_aClocks[data1]);
				list_speed(LIST_ALL_SPEEDS);
			}
			break;
		case CMD_CLOCK_CHANGE_SYSPLL:
			data1 = 400;	// Frequency
			skipSpace( &pCmd );
			if ( *pCmd != '\0' )
				pCmd = getIntToken( pCmd, &data1 );

			result = diag_clock_change_sysPll(data1);
			dbg_printf(PRN_RES, " sysPll set to %d\n", result);
			break;
		case CMD_CLOCK_HELP:
		default:
			printCommandHelp(g_aClockCmds, g_aClockCmdHelp, CMD_CLOCK_MAX);
			break;
	}
	return iResult;
}

int diag_clock_change_cpuPll(unsigned int frequency)
{
#if BG2_DV_BOARD
	unsigned int vcoDiv_sel_setting;
	unsigned int kvco;
	unsigned int vco_vrng;
	unsigned int FbDiv;
	unsigned int vco;
	unsigned int cpuPll;
	TGbl_ClkSwitch		ClkSwitch;
	TGbl_cpuPllCtl		cpuPllCtl;
	T32Gbl_ResetTrigger	ResetTrigger;
	int i;

	if (frequency >= 1250 && frequency <= 2500)
		vcoDiv_sel_setting = 0;
	else if (frequency < 1250 && frequency >= 625)
		vcoDiv_sel_setting = 2;
	else if (frequency < 625 && frequency >= 313)
		vcoDiv_sel_setting = 5;
	else if (frequency < 313 && frequency >= 157)
		vcoDiv_sel_setting = 8;
	else 
	{
		dbg_printf(PRN_RES, " invalid frequency %d MHz\n", frequency);
		return -1;
	}

	vco = frequency * VcoDiv[vcoDiv_sel_setting];
	if (vco < 1300)
		kvco = 1;
	else if (vco < 1500)
		kvco = 2;
	else if (vco < 1700)
		kvco = 3;
	else if (vco < 1950)
		kvco = 4;
	else if (vco < 2200)
		kvco = 5;
	else if (vco < 2500)
		kvco = 6;
	else 
	{
		dbg_printf(PRN_RES, " invalid vco %d MHz. Won't happen\n", frequency);
		return -1;
	}
	
	vco_vrng = kvco - 1;
	
	FbDiv = vco * RefDiv / RefClkIn;

	// bypass ON
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32[0]);
	ClkSwitch.uClkSwitch_cpu1PLLSWBypass = 1;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);

	// change Pll frequency
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpuPllCtl), &cpuPllCtl.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpuPllCtl1), &cpuPllCtl.u32[1]);
	cpuPllCtl.ucpuPllCtl_cpuPllRfDiv = RefDiv;
	cpuPllCtl.ucpuPllCtl_cpuPllFbDiv = FbDiv;
	cpuPllCtl.ucpuPllCtl_cpuPllKvco = kvco;
	cpuPllCtl.ucpuPllCtl_cpuPllVcoRng = vco_vrng;
	cpuPllCtl.ucpuPllCtl_cpuPllVcoDivSelSe = vcoDiv_sel_setting;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpuPllCtl), cpuPllCtl.u32[0]);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpuPllCtl1), cpuPllCtl.u32[1]);

	// reset
	ResetTrigger.u32 = 0;
	ResetTrigger.uResetTrigger_sysPllSyncReset = 1;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ResetTrigger), ResetTrigger.u32);
	for (i = 0;  i < 400000; i++);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ResetTrigger), 0);

	// bypass OFF
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32[0]);
	ClkSwitch.uClkSwitch_cpu1PLLSWBypass = 0;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);

	// read Pll frequency
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpuPllCtl), &cpuPllCtl.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpuPllCtl1), &cpuPllCtl.u32[1]);
	FbDiv = cpuPllCtl.ucpuPllCtl_cpuPllFbDiv;
	RefDiv = cpuPllCtl.ucpuPllCtl_cpuPllRfDiv;
	vcoDiv_sel_setting = cpuPllCtl.ucpuPllCtl_cpuPllVcoDivSelSe;
	cpuPll = FbDiv * RefClkIn / RefDiv / VcoDiv[vcoDiv_sel_setting];

	return cpuPll;
#endif
}

int diag_clock_change_sysPll(unsigned int frequency)
{
#if BG2_DV_BOARD //YH: this needs to be updated for new PLL IP
	unsigned int vcoDiv_sel_setting;
	unsigned int kvco;
	unsigned int vco_vrng;
	unsigned int FbDiv;
	unsigned int vco;
	unsigned int sysPll;
	TGbl_ClkSwitch		ClkSwitch;
	TGbl_sysPllCtl		sysPllCtl;
	T32Gbl_ResetTrigger	ResetTrigger;
	int i;

	if (frequency >= 1250 && frequency <= 2500)
		vcoDiv_sel_setting = 0;
	else if (frequency < 1250 && frequency >= 625)
		vcoDiv_sel_setting = 2;
	else if (frequency < 625 && frequency >= 313)
		vcoDiv_sel_setting = 5;
	else if (frequency < 313 && frequency >= 157)
		vcoDiv_sel_setting = 8;
	else 
	{
		dbg_printf(PRN_RES, " invalid frequency %d MHz\n", frequency);
		return -1;
	}

	vco = frequency * VcoDiv[vcoDiv_sel_setting];
	if (vco < 1300)
		kvco = 1;
	else if (vco < 1500)
		kvco = 2;
	else if (vco < 1700)
		kvco = 3;
	else if (vco < 1950)
		kvco = 4;
	else if (vco < 2200)
		kvco = 5;
	else if (vco < 2500)
		kvco = 6;
	else 
	{
		dbg_printf(PRN_RES, " invalid vco %d MHz. Won't happen\n", frequency);
		return -1;
	}
	
	vco_vrng = kvco - 1;
	
	FbDiv = vco * RefDiv / RefClkIn;

	// bypass ON
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32[0]);
	ClkSwitch.uClkSwitch_sysPLLSWBypass = 1;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);

	// change Pll frequency
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPllCtl), &sysPllCtl.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPllCtl1), &sysPllCtl.u32[1]);
	sysPllCtl.usysPllCtl_sysPllRfDiv = RefDiv;
	sysPllCtl.usysPllCtl_sysPllFbDiv = FbDiv;
	sysPllCtl.usysPllCtl_sysPllKvco = kvco;
	sysPllCtl.usysPllCtl_sysPllVcoRng = vco_vrng;
	sysPllCtl.usysPllCtl_sysPllVcoDivSelSe = vcoDiv_sel_setting;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPllCtl), sysPllCtl.u32[0]);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPllCtl1), sysPllCtl.u32[1]);

	// reset
	ResetTrigger.u32 = 0;
	ResetTrigger.uResetTrigger_sysPllSyncReset = 1;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ResetTrigger), ResetTrigger.u32);
	for (i = 0;  i < 400000; i++);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ResetTrigger), 0);

	// bypass OFF
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32[0]);
	ClkSwitch.uClkSwitch_sysPLLSWBypass = 0;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);

	// read Pll frequency
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPllCtl), &sysPllCtl.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPllCtl1), &sysPllCtl.u32[1]);

	FbDiv = sysPllCtl.usysPllCtl_sysPllFbDiv;
	RefDiv = sysPllCtl.usysPllCtl_sysPllRfDiv;
	vcoDiv_sel_setting = sysPllCtl.usysPllCtl_sysPllVcoDivSelSe;
	sysPll = FbDiv * RefClkIn / RefDiv / VcoDiv[vcoDiv_sel_setting];

	return sysPll;
#endif
}

void diag_clock_change_cpuClk(unsigned int pllSwitch, unsigned int pllSelect, unsigned int divider)
{
#if BG2_DV_BOARD //YH: this needs to be updated for new PLL IP
	TGbl_clkSelect		clkSelect;
	TGbl_ClkSwitch		ClkSwitch;
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), &clkSelect.u32[0]);
	
	// cpuPll bypass ON
	ClkSwitch.uClkSwitch_cpu1PLLSWBypass = 1;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);

	// cpuClk use default 
	ClkSwitch.uClkSwitch_cpu0ClkPllSwitch = 0;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);

	// change divider to divided-by-3 first
	ClkSwitch.uClkSwitch_cpu0ClkD3Switch = 1;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);

	// change divider to target
	switch (divider)
	{
		case 1:
			ClkSwitch.uClkSwitch_cpu0ClkSwitch = 0;
			break;
		case 2:
			ClkSwitch.uClkSwitch_cpu0ClkSwitch = 1;
			clkSelect.uclkSelect_cpu0ClkSel = DIVIDED_BY_2;
			break;
		case 3:
			break;
		case 4:
			ClkSwitch.uClkSwitch_cpu0ClkSwitch = 1;
			clkSelect.uclkSelect_cpu0ClkSel = DIVIDED_BY_4;
			break;
		case 6:
			ClkSwitch.uClkSwitch_cpu0ClkSwitch = 1;
			clkSelect.uclkSelect_cpu0ClkSel = DIVIDED_BY_6;
			break;
		case 8:
			ClkSwitch.uClkSwitch_cpu0ClkSwitch = 1;
			clkSelect.uclkSelect_cpu0ClkSel = DIVIDED_BY_8;
			break;
		case 12:
			ClkSwitch.uClkSwitch_cpu0ClkSwitch = 1;
			clkSelect.uclkSelect_cpu0ClkSel = DIVIDED_BY_12;
			break;
		default:
			dbg_printf(PRN_ERR, " this is impossible\n");
			break;
	}
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);

	// turn off divided-by-3 if not divided by 3
	if (divider != 3)
	{
		ClkSwitch.uClkSwitch_cpu0ClkD3Switch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
	}

	// change Pll Select
	clkSelect.uclkSelect_cpu0ClkPllSel = pllSelect;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);

	// change Pll Switch
	ClkSwitch.uClkSwitch_cpu0ClkPllSwitch = pllSwitch;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);

	// bypass OFF
	ClkSwitch.uClkSwitch_cpu1PLLSWBypass = 0;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
#endif
}

#endif

void diag_clock_change_otherClk(unsigned int index, unsigned int pllSwitch, unsigned int pllSelect, unsigned int divider)
{
#if 1//YH: this needs to be updated for new PLL IP
	TGbl_clkSelect		clkSelect;
	TGbl_ClkSwitch		ClkSwitch;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), &ClkSwitch.u32[1]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), &clkSelect.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect.u32[1]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), &clkSelect.u32[2]);
	
	// sysPll bypass ON
	ClkSwitch.uClkSwitch_sysPLLSWBypass = 1;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);

	if (index == SYSCLK)
	{
		// sysClk use default sysPll
		ClkSwitch.uClkSwitch_sysClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_sysClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_sysClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_sysClkSwitch = 1;
				clkSelect.uclkSelect_sysClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_sysClkSwitch = 1;
				clkSelect.uclkSelect_sysClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_sysClkSwitch = 1;
				clkSelect.uclkSelect_sysClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_sysClkSwitch = 1;
				clkSelect.uclkSelect_sysClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_sysClkSwitch = 1;
				clkSelect.uclkSelect_sysClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_sysClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_sysClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_sysClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	} 
	else if (index == DRMCLK)
	{
		// drmFigoClk use default sysPll
		ClkSwitch.uClkSwitch_drmFigoClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_drmFigoClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_drmFigoClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_drmFigoClkSwitch = 1;
				clkSelect.uclkSelect_drmFigoClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_drmFigoClkSwitch = 1;
				clkSelect.uclkSelect_drmFigoClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_drmFigoClkSwitch = 1;
				clkSelect.uclkSelect_drmFigoClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_drmFigoClkSwitch = 1;
				clkSelect.uclkSelect_drmFigoClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_drmFigoClkSwitch = 1;
				clkSelect.uclkSelect_drmFigoClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_drmFigoClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_drmFigoClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_drmFigoClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == CFGCLK)
	{
		// cfgClk use default sysPll
		ClkSwitch.uClkSwitch_cfgClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_cfgClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_cfgClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_cfgClkSwitch = 1;
				clkSelect.uclkSelect_cfgClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_cfgClkSwitch = 1;
				clkSelect.uclkSelect_cfgClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_cfgClkSwitch = 1;
				clkSelect.uclkSelect_cfgClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_cfgClkSwitch = 1;
				clkSelect.uclkSelect_cfgClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_cfgClkSwitch = 1;
				clkSelect.uclkSelect_cfgClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_cfgClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_cfgClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_cfgClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == GFXCLK)
	{
		// gfxClk use default sysPll
		ClkSwitch.uClkSwitch_gfxClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_gfxClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_gfxClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_gfxClkSwitch = 1;
				clkSelect.uclkSelect_gfxClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_gfxClkSwitch = 1;
				clkSelect.uclkSelect_gfxClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_gfxClkSwitch = 1;
				clkSelect.uclkSelect_gfxClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_gfxClkSwitch = 1;
				clkSelect.uclkSelect_gfxClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_gfxClkSwitch = 1;
				clkSelect.uclkSelect_gfxClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_gfxClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_gfxClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_gfxClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == ZSPCLK)
	{
		// zspClk use default sysPll
		ClkSwitch.uClkSwitch_zspClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_zspClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_zspClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_zspClkSwitch = 1;
				clkSelect.uclkSelect_zspClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_zspClkSwitch = 1;
				clkSelect.uclkSelect_zspClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_zspClkSwitch = 1;
				clkSelect.uclkSelect_zspClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_zspClkSwitch = 1;
				clkSelect.uclkSelect_zspClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_zspClkSwitch = 1;
				clkSelect.uclkSelect_zspClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_zspClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_zspClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_zspClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == PERIFCLK)
	{
		// perifClk use default sysPll
		ClkSwitch.uClkSwitch_perifClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_perifClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_perifClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_perifClkSwitch = 1;
				clkSelect.uclkSelect_perifClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_perifClkSwitch = 1;
				clkSelect.uclkSelect_perifClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_perifClkSwitch = 1;
				clkSelect.uclkSelect_perifClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_perifClkSwitch = 1;
				clkSelect.uclkSelect_perifClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_perifClkSwitch = 1;
				clkSelect.uclkSelect_perifClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_perifClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_perifClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_perifClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == PCUBECLK)
	{
		// pCubeClk use default sysPll
		ClkSwitch.uClkSwitch_pCubeClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_pCubeClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_pCubeClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_pCubeClkSwitch = 1;
				clkSelect.uclkSelect_pCubeClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_pCubeClkSwitch = 1;
				clkSelect.uclkSelect_pCubeClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_pCubeClkSwitch = 1;
				clkSelect.uclkSelect_pCubeClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_pCubeClkSwitch = 1;
				clkSelect.uclkSelect_pCubeClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_pCubeClkSwitch = 1;
				clkSelect.uclkSelect_pCubeClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_pCubeClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_pCubeClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_pCubeClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == VSCOPECLK)
	{
		// vScopeClk use default sysPll
		ClkSwitch.uClkSwitch_vScopeClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_vScopeClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_vScopeClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_vScopeClkSwitch = 1;
				clkSelect.uclkSelect_vScopeClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_vScopeClkSwitch = 1;
				clkSelect.uclkSelect_vScopeClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_vScopeClkSwitch = 1;
				clkSelect.uclkSelect_vScopeClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_vScopeClkSwitch = 1;
				clkSelect.uclkSelect_vScopeClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_vScopeClkSwitch = 1;
				clkSelect.uclkSelect_vScopeClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_vScopeClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_vScopeClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_vScopeClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == NFCECCCLK)
	{
		// nfcEccClk use default sysPll
		ClkSwitch.uClkSwitch_nfcEccClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_nfcEccClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_nfcEccClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_nfcEccClkSwitch = 1;
				clkSelect.uclkSelect_nfcEccClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_nfcEccClkSwitch = 1;
				clkSelect.uclkSelect_nfcEccClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_nfcEccClkSwitch = 1;
				clkSelect.uclkSelect_nfcEccClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_nfcEccClkSwitch = 1;
				clkSelect.uclkSelect_nfcEccClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_nfcEccClkSwitch = 1;
				clkSelect.uclkSelect_nfcEccClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_nfcEccClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_nfcEccClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_nfcEccClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == VPPSYSCLK)
	{
		// vppSysClk use default sysPll
		ClkSwitch.uClkSwitch_vppSysClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_vppSysClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_vppSysClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_vppSysClkSwitch = 1;
				clkSelect.uclkSelect_vppSysClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_vppSysClkSwitch = 1;
				clkSelect.uclkSelect_vppSysClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_vppSysClkSwitch = 1;
				clkSelect.uclkSelect_vppSysClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_vppSysClkSwitch = 1;
				clkSelect.uclkSelect_vppSysClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_vppSysClkSwitch = 1;
				clkSelect.uclkSelect_vppSysClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_vppSysClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_vppSysClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_vppSysClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == APPCLK)
	{
		// appClk use default sysPll
		ClkSwitch.uClkSwitch_appClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to divided-by-3 first
		ClkSwitch.uClkSwitch_appClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// change divider to target
		switch (divider)
		{
			case 1:
				ClkSwitch.uClkSwitch_appClkSwitch = 0;
				break;
			case 2:
				ClkSwitch.uClkSwitch_appClkSwitch = 1;
				clkSelect.uclkSelect_appClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				ClkSwitch.uClkSwitch_appClkSwitch = 1;
				clkSelect.uclkSelect_appClkSel = DIVIDED_BY_4;
				break;
			case 6:
				ClkSwitch.uClkSwitch_appClkSwitch = 1;
				clkSelect.uclkSelect_appClkSel = DIVIDED_BY_6;
				break;
			case 8:
				ClkSwitch.uClkSwitch_appClkSwitch = 1;
				clkSelect.uclkSelect_appClkSel = DIVIDED_BY_8;
				break;
			case 12:
				ClkSwitch.uClkSwitch_appClkSwitch = 1;
				clkSelect.uclkSelect_appClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			ClkSwitch.uClkSwitch_appClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
		}

		// change Pll Select
		clkSelect.uclkSelect_appClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), clkSelect.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), clkSelect.u32[1]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), clkSelect.u32[2]);

		// change Pll Switch
		ClkSwitch.uClkSwitch_appClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), ClkSwitch.u32[1]);
	}
	else if (index == GFX3DCORECLK)
	{
		T32Gbl_gfx3DCoreClkCtrl gfx3DCoreClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DCoreClkCtrl), &gfx3DCoreClkCtrl.u32);
		
		// use default sysPll
		gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DCoreClkCtrl), gfx3DCoreClkCtrl.u32);

		// change divider to divided-by-3 first
		gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DCoreClkCtrl), gfx3DCoreClkCtrl.u32);

		// change divider to target
		switch (divider)
		{
			case 1:
				gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkSwitch = 0;
				break;
			case 2:
				gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkSwitch = 1;
				gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkSwitch = 1;
				gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkSel = DIVIDED_BY_4;
				break;
			case 6:
				gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkSwitch = 1;
				gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkSel = DIVIDED_BY_6;
				break;
			case 8:
				gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkSwitch = 1;
				gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkSel = DIVIDED_BY_8;
				break;
			case 12:
				gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkSwitch = 1;
				gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DCoreClkCtrl), gfx3DCoreClkCtrl.u32);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DCoreClkCtrl), gfx3DCoreClkCtrl.u32);
		}

		// change Pll Select
		gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DCoreClkCtrl), gfx3DCoreClkCtrl.u32);

		// change Pll Switch
		gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DCoreClkCtrl), gfx3DCoreClkCtrl.u32);
	}
	else if (index == GFX3DSYSCLK)
	{
		T32Gbl_gfx3DSysClkCtrl gfx3DSysClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DSysClkCtrl), &gfx3DSysClkCtrl.u32);
		
		// use default sysPll
		gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DSysClkCtrl), gfx3DSysClkCtrl.u32);

		// change divider to divided-by-3 first
		gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DSysClkCtrl), gfx3DSysClkCtrl.u32);

		// change divider to target
		switch (divider)
		{
			case 1:
				gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkSwitch = 0;
				break;
			case 2:
				gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkSwitch = 1;
				gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkSwitch = 1;
				gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkSel = DIVIDED_BY_4;
				break;
			case 6:
				gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkSwitch = 1;
				gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkSel = DIVIDED_BY_6;
				break;
			case 8:
				gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkSwitch = 1;
				gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkSel = DIVIDED_BY_8;
				break;
			case 12:
				gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkSwitch = 1;
				gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DSysClkCtrl), gfx3DSysClkCtrl.u32);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DSysClkCtrl), gfx3DSysClkCtrl.u32);
		}

		// change Pll Select
		gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DSysClkCtrl), gfx3DSysClkCtrl.u32);

		// change Pll Switch
		gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DSysClkCtrl), gfx3DSysClkCtrl.u32);
	}
	else if (index == ARCREFCLK)
	{
		T32Gbl_arcRefClkCtrl arcRefClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_arcRefClkCtrl), &arcRefClkCtrl.u32);
		
		// use default sysPll
		arcRefClkCtrl.uarcRefClkCtrl_ClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_arcRefClkCtrl), arcRefClkCtrl.u32);

		// change divider to divided-by-3 first
		arcRefClkCtrl.uarcRefClkCtrl_ClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_arcRefClkCtrl), arcRefClkCtrl.u32);

		// change divider to target
		switch (divider)
		{
			case 1:
				arcRefClkCtrl.uarcRefClkCtrl_ClkSwitch = 0;
				break;
			case 2:
				arcRefClkCtrl.uarcRefClkCtrl_ClkSwitch = 1;
				arcRefClkCtrl.uarcRefClkCtrl_ClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				arcRefClkCtrl.uarcRefClkCtrl_ClkSwitch = 1;
				arcRefClkCtrl.uarcRefClkCtrl_ClkSel = DIVIDED_BY_4;
				break;
			case 6:
				arcRefClkCtrl.uarcRefClkCtrl_ClkSwitch = 1;
				arcRefClkCtrl.uarcRefClkCtrl_ClkSel = DIVIDED_BY_6;
				break;
			case 8:
				arcRefClkCtrl.uarcRefClkCtrl_ClkSwitch = 1;
				arcRefClkCtrl.uarcRefClkCtrl_ClkSel = DIVIDED_BY_8;
				break;
			case 12:
				arcRefClkCtrl.uarcRefClkCtrl_ClkSwitch = 1;
				arcRefClkCtrl.uarcRefClkCtrl_ClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_arcRefClkCtrl), arcRefClkCtrl.u32);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			arcRefClkCtrl.uarcRefClkCtrl_ClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_arcRefClkCtrl), arcRefClkCtrl.u32);
		}

		// change Pll Select
		arcRefClkCtrl.uarcRefClkCtrl_ClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_arcRefClkCtrl), arcRefClkCtrl.u32);

		// change Pll Switch
		arcRefClkCtrl.uarcRefClkCtrl_ClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_arcRefClkCtrl), arcRefClkCtrl.u32);
	}
#if 0 //not in A0
	else if (index == HDMIRXFPLLREFCLK)
	{
		T32Gbl_hdmirxFpllRefClkCtrl hdmirxFpllRefClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxFpllRefClkCtrl), &hdmirxFpllRefClkCtrl.u32);
		
		// use default sysPll
		hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxFpllRefClkCtrl), hdmirxFpllRefClkCtrl.u32);

		// change divider to divided-by-3 first
		hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxFpllRefClkCtrl), hdmirxFpllRefClkCtrl.u32);

		// change divider to target
		switch (divider)
		{
			case 1:
				hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkSwitch = 0;
				break;
			case 2:
				hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkSwitch = 1;
				hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkSwitch = 1;
				hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkSel = DIVIDED_BY_4;
				break;
			case 6:
				hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkSwitch = 1;
				hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkSel = DIVIDED_BY_6;
				break;
			case 8:
				hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkSwitch = 1;
				hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkSel = DIVIDED_BY_8;
				break;
			case 12:
				hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkSwitch = 1;
				hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxFpllRefClkCtrl), hdmirxFpllRefClkCtrl.u32);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxFpllRefClkCtrl), hdmirxFpllRefClkCtrl.u32);
		}

		// change Pll Select
		hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxFpllRefClkCtrl), hdmirxFpllRefClkCtrl.u32);

		// change Pll Switch
		hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxFpllRefClkCtrl), hdmirxFpllRefClkCtrl.u32);
	}
	else if (index == HDMIRXTCLK)
	{
		T32Gbl_hdmirxTclkCtrl hdmirxTclkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxTclkCtrl), &hdmirxTclkCtrl.u32);
		
		// use default sysPll
		hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxTclkCtrl), hdmirxTclkCtrl.u32);

		// change divider to divided-by-3 first
		hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxTclkCtrl), hdmirxTclkCtrl.u32);

		// change divider to target
		switch (divider)
		{
			case 1:
				hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkSwitch = 0;
				break;
			case 2:
				hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkSwitch = 1;
				hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkSwitch = 1;
				hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkSel = DIVIDED_BY_4;
				break;
			case 6:
				hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkSwitch = 1;
				hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkSel = DIVIDED_BY_6;
				break;
			case 8:
				hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkSwitch = 1;
				hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkSel = DIVIDED_BY_8;
				break;
			case 12:
				hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkSwitch = 1;
				hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxTclkCtrl), hdmirxTclkCtrl.u32);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxTclkCtrl), hdmirxTclkCtrl.u32);
		}

		// change Pll Select
		hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxTclkCtrl), hdmirxTclkCtrl.u32);

		// change Pll Switch
		hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxTclkCtrl), hdmirxTclkCtrl.u32);
	}
#endif
	else if (index == VIPCLK)
	{
		T32Gbl_vipClkCtrl vipClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_vipClkCtrl), &vipClkCtrl.u32);
		
		// use default sysPll
		vipClkCtrl.uvipClkCtrl_ClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_vipClkCtrl), vipClkCtrl.u32);

		// change divider to divided-by-3 first
		vipClkCtrl.uvipClkCtrl_ClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_vipClkCtrl), vipClkCtrl.u32);

		// change divider to target
		switch (divider)
		{
			case 1:
				vipClkCtrl.uvipClkCtrl_ClkSwitch = 0;
				break;
			case 2:
				vipClkCtrl.uvipClkCtrl_ClkSwitch = 1;
				vipClkCtrl.uvipClkCtrl_ClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				vipClkCtrl.uvipClkCtrl_ClkSwitch = 1;
				vipClkCtrl.uvipClkCtrl_ClkSel = DIVIDED_BY_4;
				break;
			case 6:
				vipClkCtrl.uvipClkCtrl_ClkSwitch = 1;
				vipClkCtrl.uvipClkCtrl_ClkSel = DIVIDED_BY_6;
				break;
			case 8:
				vipClkCtrl.uvipClkCtrl_ClkSwitch = 1;
				vipClkCtrl.uvipClkCtrl_ClkSel = DIVIDED_BY_8;
				break;
			case 12:
				vipClkCtrl.uvipClkCtrl_ClkSwitch = 1;
				vipClkCtrl.uvipClkCtrl_ClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_vipClkCtrl), vipClkCtrl.u32);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			vipClkCtrl.uvipClkCtrl_ClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_vipClkCtrl), vipClkCtrl.u32);
		}

		// change Pll Select
		vipClkCtrl.uvipClkCtrl_ClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_vipClkCtrl), vipClkCtrl.u32);

		// change Pll Switch
		vipClkCtrl.uvipClkCtrl_ClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_vipClkCtrl), vipClkCtrl.u32);
	}
	else if (index == SDIOXINCLK)
	{
		T32Gbl_sdioXinClkCtrl sdioXinClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdioXinClkCtrl), &sdioXinClkCtrl.u32);
		
		// use default sysPll
		sdioXinClkCtrl.usdioXinClkCtrl_ClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdioXinClkCtrl), sdioXinClkCtrl.u32);

		// change divider to divided-by-3 first
		sdioXinClkCtrl.usdioXinClkCtrl_ClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdioXinClkCtrl), sdioXinClkCtrl.u32);

		// change divider to target
		switch (divider)
		{
			case 1:
				sdioXinClkCtrl.usdioXinClkCtrl_ClkSwitch = 0;
				break;
			case 2:
				sdioXinClkCtrl.usdioXinClkCtrl_ClkSwitch = 1;
				sdioXinClkCtrl.usdioXinClkCtrl_ClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				sdioXinClkCtrl.usdioXinClkCtrl_ClkSwitch = 1;
				sdioXinClkCtrl.usdioXinClkCtrl_ClkSel = DIVIDED_BY_4;
				break;
			case 6:
				sdioXinClkCtrl.usdioXinClkCtrl_ClkSwitch = 1;
				sdioXinClkCtrl.usdioXinClkCtrl_ClkSel = DIVIDED_BY_6;
				break;
			case 8:
				sdioXinClkCtrl.usdioXinClkCtrl_ClkSwitch = 1;
				sdioXinClkCtrl.usdioXinClkCtrl_ClkSel = DIVIDED_BY_8;
				break;
			case 12:
				sdioXinClkCtrl.usdioXinClkCtrl_ClkSwitch = 1;
				sdioXinClkCtrl.usdioXinClkCtrl_ClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdioXinClkCtrl), sdioXinClkCtrl.u32);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			sdioXinClkCtrl.usdioXinClkCtrl_ClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdioXinClkCtrl), sdioXinClkCtrl.u32);
		}

		// change Pll Select
		sdioXinClkCtrl.usdioXinClkCtrl_ClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdioXinClkCtrl), sdioXinClkCtrl.u32);

		// change Pll Switch
		sdioXinClkCtrl.usdioXinClkCtrl_ClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdioXinClkCtrl), sdioXinClkCtrl.u32);
	}
	else if (index == SDIO1XINCLK)
	{
		T32Gbl_sdio1XinClkCtrl sdio1XinClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdio1XinClkCtrl), &sdio1XinClkCtrl.u32);
		
		// use default sysPll
		sdio1XinClkCtrl.usdio1XinClkCtrl_ClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdio1XinClkCtrl), sdio1XinClkCtrl.u32);

		// change divider to divided-by-3 first
		sdio1XinClkCtrl.usdio1XinClkCtrl_ClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdio1XinClkCtrl), sdio1XinClkCtrl.u32);

		// change divider to target
		switch (divider)
		{
			case 1:
				sdio1XinClkCtrl.usdio1XinClkCtrl_ClkSwitch = 0;
				break;
			case 2:
				sdio1XinClkCtrl.usdio1XinClkCtrl_ClkSwitch = 1;
				sdio1XinClkCtrl.usdio1XinClkCtrl_ClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				sdio1XinClkCtrl.usdio1XinClkCtrl_ClkSwitch = 1;
				sdio1XinClkCtrl.usdio1XinClkCtrl_ClkSel = DIVIDED_BY_4;
				break;
			case 6:
				sdio1XinClkCtrl.usdio1XinClkCtrl_ClkSwitch = 1;
				sdio1XinClkCtrl.usdio1XinClkCtrl_ClkSel = DIVIDED_BY_6;
				break;
			case 8:
				sdio1XinClkCtrl.usdio1XinClkCtrl_ClkSwitch = 1;
				sdio1XinClkCtrl.usdio1XinClkCtrl_ClkSel = DIVIDED_BY_8;
				break;
			case 12:
				sdio1XinClkCtrl.usdio1XinClkCtrl_ClkSwitch = 1;
				sdio1XinClkCtrl.usdio1XinClkCtrl_ClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdio1XinClkCtrl), sdio1XinClkCtrl.u32);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			sdio1XinClkCtrl.usdio1XinClkCtrl_ClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdio1XinClkCtrl), sdio1XinClkCtrl.u32);
		}

		// change Pll Select
		sdio1XinClkCtrl.usdio1XinClkCtrl_ClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdio1XinClkCtrl), sdio1XinClkCtrl.u32);

		// change Pll Switch
		sdio1XinClkCtrl.usdio1XinClkCtrl_ClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdio1XinClkCtrl), sdio1XinClkCtrl.u32);
	}
	else if (index == GFX3DEXTRACLK)
	{
		T32Gbl_gfx3DExtraClkCtrl gfx3DExtraClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DExtraClkCtrl), &gfx3DExtraClkCtrl.u32);
		
		// use default sysPll
		gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DExtraClkCtrl), gfx3DExtraClkCtrl.u32);

		// change divider to divided-by-3 first
		gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DExtraClkCtrl), gfx3DExtraClkCtrl.u32);

		// change divider to target
		switch (divider)
		{
			case 1:
				gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkSwitch = 0;
				break;
			case 2:
				gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkSwitch = 1;
				gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkSwitch = 1;
				gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkSel = DIVIDED_BY_4;
				break;
			case 6:
				gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkSwitch = 1;
				gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkSel = DIVIDED_BY_6;
				break;
			case 8:
				gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkSwitch = 1;
				gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkSel = DIVIDED_BY_8;
				break;
			case 12:
				gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkSwitch = 1;
				gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DExtraClkCtrl), gfx3DExtraClkCtrl.u32);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DExtraClkCtrl), gfx3DExtraClkCtrl.u32);
		}

		// change Pll Select
		gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DExtraClkCtrl), gfx3DExtraClkCtrl.u32);

		// change Pll Switch
		gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DExtraClkCtrl), gfx3DExtraClkCtrl.u32);
	}
	else if (index == GC360CLK)
	{
		T32Gbl_gc360ClkCtrl gc360ClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gc360ClkCtrl), &gc360ClkCtrl.u32);
		
		// use default sysPll
		gc360ClkCtrl.ugc360ClkCtrl_ClkPllSwitch = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gc360ClkCtrl), gc360ClkCtrl.u32);

		// change divider to divided-by-3 first
		gc360ClkCtrl.ugc360ClkCtrl_ClkD3Switch = 1;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gc360ClkCtrl), gc360ClkCtrl.u32);

		// change divider to target
		switch (divider)
		{
			case 1:
				gc360ClkCtrl.ugc360ClkCtrl_ClkSwitch = 0;
				break;
			case 2:
				gc360ClkCtrl.ugc360ClkCtrl_ClkSwitch = 1;
				gc360ClkCtrl.ugc360ClkCtrl_ClkSel = DIVIDED_BY_2;
				break;
			case 3:
				break;
			case 4:
				gc360ClkCtrl.ugc360ClkCtrl_ClkSwitch = 1;
				gc360ClkCtrl.ugc360ClkCtrl_ClkSel = DIVIDED_BY_4;
				break;
			case 6:
				gc360ClkCtrl.ugc360ClkCtrl_ClkSwitch = 1;
				gc360ClkCtrl.ugc360ClkCtrl_ClkSel = DIVIDED_BY_6;
				break;
			case 8:
				gc360ClkCtrl.ugc360ClkCtrl_ClkSwitch = 1;
				gc360ClkCtrl.ugc360ClkCtrl_ClkSel = DIVIDED_BY_8;
				break;
			case 12:
				gc360ClkCtrl.ugc360ClkCtrl_ClkSwitch = 1;
				gc360ClkCtrl.ugc360ClkCtrl_ClkSel = DIVIDED_BY_12;
				break;
			default:
				dbg_printf(PRN_ERR, " this is impossible\n");
				break;
		}
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gc360ClkCtrl), gc360ClkCtrl.u32);

		// turn off divided-by-3 if not divided by 3
		if (divider != 3)
		{
			gc360ClkCtrl.ugc360ClkCtrl_ClkD3Switch = 0;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gc360ClkCtrl), gc360ClkCtrl.u32);
		}

		// change Pll Select
		gc360ClkCtrl.ugc360ClkCtrl_ClkPllSel = pllSelect;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gc360ClkCtrl), gc360ClkCtrl.u32);

		// change Pll Switch
		gc360ClkCtrl.ugc360ClkCtrl_ClkPllSwitch = pllSwitch;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gc360ClkCtrl), gc360ClkCtrl.u32);
	}
	else
		dbg_printf(PRN_ERR, " this is impossible\n");

	// bypass OFF
	ClkSwitch.uClkSwitch_sysPLLSWBypass = 0;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), ClkSwitch.u32[0]);
#endif
}
#if 0
unsigned int get_divider(unsigned int D3Switch, unsigned int Switch, unsigned int Select)
{
	unsigned int divider;
	if (D3Switch)
		divider = 3;
	else
	{
		if (!Switch)
			divider = 1;
		else
			divider = clock_divider[Select];
	}
	return divider;
}

void list_speed(int level)
{
#if BG2_DV_BOARD //YH: this needs to be updated for new PLL IP
	unsigned int FbDiv;
	unsigned int RfDiv;
	unsigned int vcoDiv_sel_setting;
	TGbl_memPllCtl	memPllCtl;
	TGbl_sysPllCtl	sysPllCtl;
	TGbl_cpuPllCtl	cpuPllCtl;
	unsigned int memPll;
	unsigned int sysPll;
	unsigned int cpuPll;
	unsigned int divider;
	unsigned int D3Switch, Switch, Select;
	TGbl_ClkSwitch	ClkSwitch;
	TGbl_clkSelect	clkSelect;
	unsigned int PllSel;

	// memPll
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_memPllCtl), &memPllCtl.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_memPllCtl1), &memPllCtl.u32[1]);
	FbDiv = memPllCtl.umemPllCtl_memPllFbDiv;
	RfDiv = memPllCtl.umemPllCtl_memPllRfDiv;
	vcoDiv_sel_setting = memPllCtl.umemPllCtl_memPllVcoDivSelDiff;
	memPll = FbDiv * RefClkIn / RfDiv;
	memPll = (float)memPll / VcoDiv[vcoDiv_sel_setting];
	dbg_printf(PRN_RES, " memPll		frequency %d\n", memPll);

	// sysPll
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPllCtl), &sysPllCtl.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPllCtl1), &sysPllCtl.u32[1]);
	FbDiv = sysPllCtl.usysPllCtl_sysPllFbDiv;
	RfDiv = sysPllCtl.usysPllCtl_sysPllRfDiv;
	vcoDiv_sel_setting = sysPllCtl.usysPllCtl_sysPllVcoDivSelSe;
	sysPll = FbDiv * RefClkIn / RfDiv;
	sysPll = (float)sysPll / VcoDiv[vcoDiv_sel_setting];
	dbg_printf(PRN_RES, " sysPll		frequency %d\n", sysPll);

	// cpuPll
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpuPllCtl), &cpuPllCtl.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpuPllCtl1), &cpuPllCtl.u32[1]);
	FbDiv = cpuPllCtl.ucpuPllCtl_cpuPllFbDiv;
	RfDiv = cpuPllCtl.ucpuPllCtl_cpuPllRfDiv;
	vcoDiv_sel_setting = cpuPllCtl.ucpuPllCtl_cpuPllVcoDivSelSe;
	cpuPll = FbDiv * RefClkIn / RfDiv;
	cpuPll = (float)cpuPll / VcoDiv[vcoDiv_sel_setting];
	dbg_printf(PRN_RES, " cpuPll		frequency %d\n", cpuPll);

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), &ClkSwitch.u32[1]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), &clkSelect.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect.u32[1]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), &clkSelect.u32[2]);

		// DDR
	dbg_printf(PRN_RES, " dClk		frequency %d\n", memPll / 4);

		// CPU
	D3Switch = ClkSwitch.uClkSwitch_cpu0ClkD3Switch;
	Switch = ClkSwitch.uClkSwitch_cpu0ClkSwitch;
	Select = clkSelect.uclkSelect_cpu0ClkSel;
	divider = get_divider(D3Switch, Switch, Select);
	PllSel = clkSelect.uclkSelect_cpu0ClkPllSel;
	if (ClkSwitch.uClkSwitch_cpu0ClkPllSwitch)		// AVPLL or memPll
	{
		if (PllSel == 4)							// memPll
		{
			speed_cpu = memPll / divider;
			dbg_printf(PRN_RES, " cpuClk		frequency %d\n", speed_cpu);	
		}
		else
		{	
			speed_cpu = current_freq[1][PllSel + 4] / divider;
			dbg_printf(PRN_RES, " cpuClk		frequency %d [%s]\n", 
					speed_cpu, AVPLL[PllSel]);
		}
	}
	else
	{
		speed_cpu = cpuPll / divider;
		dbg_printf(PRN_RES, " cpuClk		frequency %d\n", speed_cpu);
	}

	if (level == LIST_ALL_SPEEDS)
	{
			// sysClk
		D3Switch = ClkSwitch.uClkSwitch_sysClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_sysClkSwitch;
		Select = clkSelect.uclkSelect_sysClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_sysClkPllSel;
		if (ClkSwitch.uClkSwitch_sysClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " sysClk		frequency %.0f [%s]\n",
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " sysClk		invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " sysClk		frequency %d\n", sysPll / divider);

			// drmFigoClk
		D3Switch = ClkSwitch.uClkSwitch_drmFigoClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_drmFigoClkSwitch;
		Select = clkSelect.uclkSelect_drmFigoClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_drmFigoClkPllSel;
		if (ClkSwitch.uClkSwitch_drmFigoClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " drmClk		frequency %.0f [%s]\n",
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " drmClk		invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " drmClk		frequency %d\n", sysPll / divider);

			// cfgClk
		D3Switch = ClkSwitch.uClkSwitch_cfgClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_cfgClkSwitch;
		Select = clkSelect.uclkSelect_cfgClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_cfgClkPllSel;
		if (ClkSwitch.uClkSwitch_cfgClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " cfgClk		frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " cfgClk		invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " cfgClk		frequency %d\n", sysPll / divider);

			// gfxClk
		D3Switch = ClkSwitch.uClkSwitch_gfxClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_gfxClkSwitch;
		Select = clkSelect.uclkSelect_gfxClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_gfxClkPllSel;
		if (ClkSwitch.uClkSwitch_gfxClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " gfxClk	frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " gfxClk		invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " gfxClk		frequency %d\n", sysPll / divider);

			// zspClk
		D3Switch = ClkSwitch.uClkSwitch_zspClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_zspClkSwitch;
		Select = clkSelect.uclkSelect_zspClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_zspClkPllSel;
		if (ClkSwitch.uClkSwitch_zspClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " zspClk	frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " zspClk	invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " zspClk		frequency %d\n", sysPll / divider);

			// perifClk
		D3Switch = ClkSwitch.uClkSwitch_perifClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_perifClkSwitch;
		Select = clkSelect.uclkSelect_perifClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_perifClkPllSel;
		if (ClkSwitch.uClkSwitch_perifClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " perifClk	frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " perifClk	invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " perifClk	frequency %d\n", sysPll / divider);

			// pCubeClk
		D3Switch = ClkSwitch.uClkSwitch_pCubeClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_pCubeClkSwitch;
		Select = clkSelect.uclkSelect_pCubeClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_pCubeClkPllSel;
		if (ClkSwitch.uClkSwitch_pCubeClkPllSwitch)		// AVPLL 
			if (PllSel < 4)
				dbg_printf(PRN_RES, " pCubeClk	frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " pCubeClk	invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " pCubeClk	frequency %d\n", sysPll / divider);

			// vScopeClk
		D3Switch = ClkSwitch.uClkSwitch_vScopeClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_vScopeClkSwitch;
		Select = clkSelect.uclkSelect_vScopeClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_vScopeClkPllSel;
		if (ClkSwitch.uClkSwitch_vScopeClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " vScopeClk	frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " vScopeClk	invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " vScopeClk	frequency %d\n", sysPll / divider);

			// nfcEccClk
		D3Switch = ClkSwitch.uClkSwitch_nfcEccClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_nfcEccClkSwitch;
		Select = clkSelect.uclkSelect_nfcEccClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_nfcEccClkPllSel;
		if (ClkSwitch.uClkSwitch_nfcEccClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " nfcEccClk	frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " nfcEccClk	invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " nfcEccClk	frequency %d\n", sysPll / divider);

			// vppSysClk
		D3Switch = ClkSwitch.uClkSwitch_vppSysClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_vppSysClkSwitch;
		Select = clkSelect.uclkSelect_vppSysClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_vppSysClkPllSel;
		if (ClkSwitch.uClkSwitch_vppSysClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " vppSysClk	frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " vppSysClk	invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " vppSysClk	frequency %d\n", sysPll / divider);

			// appClk
		D3Switch = ClkSwitch.uClkSwitch_appClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_appClkSwitch;
		Select = clkSelect.uclkSelect_appClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_appClkPllSel;
		if (ClkSwitch.uClkSwitch_appClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " appClk		frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " appClk		invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " appClk		frequency %d\n", sysPll / divider);

			// gfx3DCoreClk
		T32Gbl_gfx3DCoreClkCtrl gfx3DCoreClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DCoreClkCtrl), &gfx3DCoreClkCtrl.u32);
		D3Switch = gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkD3Switch;
		Switch = gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkSwitch;
		Select = gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkPllSel;
		if (gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " gfx3DCoreClk	frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " gfx3DCoreClk	invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " gfx3DCoreClk	frequency %d\n", sysPll / divider);

			// gfx3DSysClk
		T32Gbl_gfx3DSysClkCtrl gfx3DSysClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DSysClkCtrl), &gfx3DSysClkCtrl.u32);
		D3Switch = gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkD3Switch;
		Switch = gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkSwitch;
		Select = gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkPllSel;
		if (gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " gfx3DSysClk	frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " gfx3DSysClk	invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " gfx3DSysClk	frequency %d\n", sysPll / divider);

			// arcRefClk
		T32Gbl_arcRefClkCtrl arcRefClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_arcRefClkCtrl), &arcRefClkCtrl.u32);
		D3Switch = arcRefClkCtrl.uarcRefClkCtrl_ClkD3Switch;
		Switch = arcRefClkCtrl.uarcRefClkCtrl_ClkSwitch;
		Select = arcRefClkCtrl.uarcRefClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = arcRefClkCtrl.uarcRefClkCtrl_ClkPllSel;
		if (arcRefClkCtrl.uarcRefClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " arcRefClk	frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " arcRefClk	invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " arcRefClk	frequency %d\n", sysPll / divider);

#if 0 //not in A0
			// hdmirxFpllRefClk
		T32Gbl_hdmirxFpllRefClkCtrl hdmirxFpllRefClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxFpllRefClkCtrl), &hdmirxFpllRefClkCtrl.u32);
		D3Switch = hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkD3Switch;
		Switch = hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkSwitch;
		Select = hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkPllSel;
		if (hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " hdmirxFpllRefClk	frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " hdmirxFpllRefClk	invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " hdmirxFpllRefClk	frequency %d\n", sysPll / divider);

			// hdmirxTclk
		T32Gbl_hdmirxTclkCtrl hdmirxTclkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxTclkCtrl), &hdmirxTclkCtrl.u32);
		D3Switch = hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkD3Switch;
		Switch = hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkSwitch;
		Select = hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkPllSel;
		if (hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " hdmirxTclk	frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " hdmirxTclk	invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " hdmirxTclk	frequency %d\n", sysPll / divider);
#endif
			// vipClk
		T32Gbl_vipClkCtrl vipClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_vipClkCtrl), &vipClkCtrl.u32);
		D3Switch = vipClkCtrl.uvipClkCtrl_ClkD3Switch;
		Switch = vipClkCtrl.uvipClkCtrl_ClkSwitch;
		Select = vipClkCtrl.uvipClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = vipClkCtrl.uvipClkCtrl_ClkPllSel;
		if (vipClkCtrl.uvipClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " vipClk		frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " vipClk		invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " vipClk		frequency %d\n", sysPll / divider);

			// sdioXinClk
		T32Gbl_sdioXinClkCtrl sdioXinClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdioXinClkCtrl), &sdioXinClkCtrl.u32);
		D3Switch = sdioXinClkCtrl.usdioXinClkCtrl_ClkD3Switch;
		Switch = sdioXinClkCtrl.usdioXinClkCtrl_ClkSwitch;
		Select = sdioXinClkCtrl.usdioXinClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = sdioXinClkCtrl.usdioXinClkCtrl_ClkPllSel;
		if (sdioXinClkCtrl.usdioXinClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " sdioXinClk	frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " sdioXinClk	invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " sdioXinClk	frequency %d\n", sysPll / divider);

			// sdio1XinClk
		T32Gbl_sdio1XinClkCtrl sdio1XinClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdio1XinClkCtrl), &sdio1XinClkCtrl.u32);
		D3Switch = sdio1XinClkCtrl.usdio1XinClkCtrl_ClkD3Switch;
		Switch = sdio1XinClkCtrl.usdio1XinClkCtrl_ClkSwitch;
		Select = sdio1XinClkCtrl.usdio1XinClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = sdio1XinClkCtrl.usdio1XinClkCtrl_ClkPllSel;
		if (sdio1XinClkCtrl.usdio1XinClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " sdio1XinClk	frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " sdio1XinClk	invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " sdio1XinClk	frequency %d\n", sysPll / divider);

			// gfx3DExtraClk
		T32Gbl_gfx3DExtraClkCtrl gfx3DExtraClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DExtraClkCtrl), &gfx3DExtraClkCtrl.u32);
		D3Switch = gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkD3Switch;
		Switch = gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkSwitch;
		Select = gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkPllSel;
		if (gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " gfx3DExtraClk	frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " gfx3DExtraClk	invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " gfx3DExtraClk	frequency %d\n", sysPll / divider);

			// gc360Clk
		T32Gbl_gc360ClkCtrl gc360ClkCtrl;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gc360ClkCtrl), &gc360ClkCtrl.u32);
		D3Switch = gc360ClkCtrl.ugc360ClkCtrl_ClkD3Switch;
		Switch = gc360ClkCtrl.ugc360ClkCtrl_ClkSwitch;
		Select = gc360ClkCtrl.ugc360ClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = gc360ClkCtrl.ugc360ClkCtrl_ClkPllSel;
		if (gc360ClkCtrl.ugc360ClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4)
				dbg_printf(PRN_RES, " gc360Clk	frequency %.0f [%s]\n", 
						current_freq[1][PllSel + 4] / divider, AVPLL[PllSel]);
			else
				dbg_printf(PRN_ERR, " gc360Clk	invalid PllSel %d\n", PllSel);
		else	// sysPll
			dbg_printf(PRN_RES, " gc360Clk	frequency %d\n", sysPll / divider);
	}
#endif
}
#endif
