#include "ctypes.h"
#include "memmap.h"
#include "galois_io.h"
#ifndef WIN32
#include "global.h"
#endif
#include "sys_memmap.h"
//#include "GaloisTime.h"
#include "galois_speed.h"


#define BFM_HOST_Bus_Write32    GA_REG_WORD32_WRITE
#define BFM_HOST_Bus_Read32     GA_REG_WORD32_READ

#if defined(BERLIN)
#ifdef __ECOS__
//	for eCos
#define	HW_show_speed(a)	printf a

#else
//	for Linux or UBOOT
#ifdef	__LINUX_KERNEL__
//#define	HW_show_speed(a)	galois_printk a
#define	HW_show_speed(a)	
#endif

#ifdef	__UBOOT__
#define	HW_show_speed(a)	printf a
#endif

#ifdef BERLIN_SINGLE_CPU
#define HW_show_speed(a)	printf a
#endif

#endif

#define DYNAMIC_POWER_CONTROL

#define        HW_show_speed(a)        lgpl_printf a


static SOC_FREQ_VECTOR mSocFreqVector={0};
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

static SOC_FREQ_VECTOR mSocOrigDivSaved = {0};
static SOC_FREQ_VECTOR mSocDivSaved = {0};
static volatile int  mSocShadow_Switch =0;
static volatile int  mSocShadow_Switch1 =0;
static volatile int  mSocShadow_Select =0;
static volatile int  mSocShadow_Select1 = 0;
static volatile int  mSocShadow_Select2 = 0;

#if (BERLIN_CHIP_VERSION < BERLIN_BG2)
float VcoDiv[] = 
{
	1,		/// * 0000: 1
	1.5,	/// * 0001: 1.5
	2,		/// * 0010: 2
	2.5,	/// * 0011: 2.5
	3,		/// * 0100: 3
	4,		/// * 0101: 4
	8,		/// * 0110: 8
	6,		/// * 0111: 6
	1,		/// * 1000: undefined
	10,		/// * 1001: 10
	12,		/// * 1010: 12
	14,		/// * 1011: 14
	1,		/// * 1100: undefined
	1,		/// * 1101: undefined
	1,		/// * 1110: undefined
	1		/// * 1111: undefined
};
const char *AVPLL[] =
{
	"AVPllA5",
	"AVPllA6",
	"AVPllB6",
	"AVPllB7",
	"CPU1Pll"
};

//this enum index into all the following arrays
static enum AVPLL_FREQ
{
	VCO_FREQ_1_258G=0,
	VCO_FREQ_1_350G,
	VCO_FREQ_1_510G,
	VCO_FREQ_1_620G,
	VCO_FREQ_1_854G,
	VCO_FREQ_2_225G,
//	VCO_FREQ_2_227G
};
#else

#define RefClkIn 25
//div=a/b
#if (BERLIN_CHIP_VERSION == BERLIN_BG2CD_A0)
int VcoDivA_B[] =
{
	1, 1,
	0, 2,
	2, 1,
	0, 2,
	3, 1,
	4, 1,
	0, 1,
	6, 1,
	8, 1,

};
#else
int VcoDivA_B[] =
{
	1, 1,
	3, 2,
	2, 1,
	5, 2,
	3, 1,
	4, 1,
	5, 1,
	6, 1,
	8, 1,

};
#endif
/*
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
*/
const char *AVPLL[] =
{
	"AVPllB4",
	"AVPllB5",
	"AVPllB6",
	"AVPllB7",
};

int current_freq[2][8];

enum 
{
	VCO_FREQ_1_080G=0,
	VCO_FREQ_1_260G,
	VCO_FREQ_1_350G,
	VCO_FREQ_1_485G,	
	VCO_FREQ_1_512G,
	VCO_FREQ_1_620G,
	VCO_FREQ_1_856G,
	VCO_FREQ_2_227G
};

unsigned char diag_avpllRegFBDIV[]=
{
	43,	//VCO_FREQ_1_080G,
	50, //VCO_FREQ_1_260G,
	54, //VCO_FREQ_1_350G,
	59, //VCO_FREQ_1_485G,
	60, //VCO_FREQ_1_512G,
	65, //VCO_FREQ_1_620G,
	74, //VCO_FREQ_1_856G,
	89, //VCO_FREQ_2_227G
};

#endif

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

#if (BERLIN_CHIP_VERSION < BERLIN_B_0)
//	for Berlin A series
#elif (BERLIN_CHIP_VERSION < BERLIN_BG2)
//	For Berlin B or C series
//	this function shall be call only once

///     # 0x00068 avPllCtl10
///               %unsigned 16 avPllFreq_Offset_B6       0x0
///               %unsigned 16 avPllFreq_Offset_B7       0x0
///     # 0x0006C avPllCtl11
///               %unsigned 16 avPllFreq_Offset_B8       0x864D
///                                    ###
///                                    * Frequency offset to the output clock
///                                    * DEFAULT: SET FREQ_OFFSET_A8[15:0]
///                                    * And
///                                    * FREQ_OFFSET_B8[15:0]
///                                    * to be
///                                    * d'3645 = 0000 1110 0011 1101
///                                    * for 1.5105G
///                                    * d'-1618 = 1000 0110 0101 0010 for 1.62G
///                                    * d'588 = 0000 0010 0100 1100 for 2.2275G
///                                    ###

///     # 0x00080 avPllCtl16
///               %unsigned 9  avPllDiv_B5               0x4
///               %unsigned 9  avPllDiv_B6               0x4
///               %unsigned 9  avPllDiv_B7               0x4
///                                    ###
///                                    * Output clock divider ratio. N=DIV_A1[8:0]
///                                    * Divider ratio=N
///                                    * Minimum: 4
///                                    * Program this for divide ratio other than 1,1.5,2,3,7.5or 15
///                                    * Please see DIVIDER ARCHITECTURE for more details
///                                    ###

///     # 0x00088 avPllCtl18
///               %unsigned 4  avPllDiv2_B3              0x0
///               %unsigned 4  avPllDiv2_B4              0x0
///               %unsigned 4  avPllDiv2_B5              0x0
///               %unsigned 4  avPllDiv2_B6              0x0
///               %unsigned 4  avPllDiv2_B7              0x0
///                                    ###
///                                    * Output Clock divider ratio. DIV2_XX [3] is the enable bit.
///                                    * Set to 1 if divide by 1,1.5,2,3,7.5or
///                                    * Set div2_xx[3] to 0 if not divide by above ratios.
///                                    * If DIV2_XX[3] = 1 then
///                                    * DIV2_XX[2:0] =
///                                    * 000 : 1
///                                    * 001: 1.5
///                                    * 010 : 2
///                                    * 011 : 3
///                                    * 100 : 7.5
///                                    * 110: 15


//	if avpll is set, all clock will take AvPLL_B6 as a reference clock
int	get_avpll_B6_clock(unsigned int * avpll_b6_clock){
	
	T32Gbl_avPllCtl reg;
	T32Gbl_avPllCtl10 regData10;
	T32Gbl_avPllCtl16 regData16;
	T32Gbl_avPllCtl18 regData18;

	int	B6_Freq_Offset ; 
	int	B6_avPllDiv ; 
	int	B6_avPllDiv2 ;
	
	//	Normally, the steps to caculate the AVPLL_B6 are:
	//	1.	read register to get the reference clock
	//	2.	read register to get the avPllDiv2 and avPllDiv
	//	3.	read register to get the Freq_Offset and 1000PPM
	//	4.	caculate the B6 clock out
	
	//	Since a lot of constraints,	for Berlin, AVPLL_B's clock reference is always 1.620G
	//	To fit the clock constraints,	B6 clock will always 800M or 400M
	//	for 800M & 400M, here is the register value 

//; *********************************************************************************
//; avPll divider and offset
//; *********************************************************************************
#define AVPLL_B6_DIV2_800                           (0x0000A000)
#define AVPLL_B6_ppmOffset_800                      (0x199A)

#define AVPLL_B6_DIV2_810                           (0x0000A000)
#define AVPLL_B6_ppmOffset_810                      (0)

#define AVPLL_B6_DIV2_405                           (0)
#define AVPLL_B6_ppmOffset_405                      (0)

#define AVPLL_B6_DIV2_400                           (0)
#define AVPLL_B6_ppmOffset_400                      (0x199A)
	
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl10), &regData10.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl16), &regData16.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl18), &regData18.u32);

	B6_Freq_Offset = regData10.uavPllCtl_avPllFreq_Offset_B6 ;
	B6_avPllDiv = regData16.uavPllCtl_avPllDiv_B6 ;	
	B6_avPllDiv2 = regData18.uavPllCtl_avPllDiv2_B6 ;

	HW_show_speed(("%s:	B6_Freq_Offset = %d, B6_avPllDiv = %d, B6_avPllDiv2 = %d\n" ,  \
					__FUNCTION__ , B6_Freq_Offset, B6_avPllDiv, B6_avPllDiv2 )) ;
	
	if(B6_avPllDiv2&0x8){
		//	avPllDiv2 is enabe and the div is 2
		*avpll_b6_clock = 800 ;
	}else{
		//	avPllDiv2 is disable, using avPllDiv, avPllDiv is 4
		*avpll_b6_clock = 400 ;
	}

	HW_show_speed(("%s, avpll_b6_clock is %d\n", __FUNCTION__, *avpll_b6_clock));
	return 0 ;
}

void show_speed(void)
{
	unsigned int FbDiv, RfDiv, VcoDivSel;
	unsigned int memPll;
	unsigned int sysPll;
	unsigned int cpu1Pll;
	unsigned int divider;
	unsigned int D3Switch, Switch, Select;
	int nfccswitch;
	T32Gbl_ClkSwitch	ClkSwitch;
	T32Gbl_ClkSwitch1	ClkSwitch1;
	T32Gbl_clkSelect	clkSelect;
	T32Gbl_clkSelect1	clkSelect1;
	T32Gbl_clkSelect2	clkSelect2;
	unsigned int PllSel;
	unsigned int avpll_b6_clock ; 
	HW_show_speed(("\n -------- Clock setting --------\n"));
	get_avpll_B6_clock(&avpll_b6_clock) ;
	{
		// memPll
		T32Gbl_memPllCtl	memPllCtl;
		T32Gbl_memPllCtl1	memPllCtl1;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_memPllCtl), &memPllCtl.u32);
		FbDiv = memPllCtl.umemPllCtl_memPllFbDiv;
		RfDiv = memPllCtl.umemPllCtl_memPllRfDiv;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_memPllCtl1), &memPllCtl1.u32);
		VcoDivSel = memPllCtl1.umemPllCtl_memPllVcoDivSel;
		memPll = (FbDiv + 2) * 25 / (RfDiv + 2) / (1 << VcoDivSel);
		mSocFreqVector.ddr = memPll/2;
		if(mSocDivSaved.ddr ==0) mSocDivSaved.ddr = 2;
		HW_show_speed((" memPLL       frequency %d\n", memPll));
	}
	{
		// sysPll
		T32Gbl_sysPllCtl	sysPllCtl;
		T32Gbl_sysPllCtl1	sysPllCtl1;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPllCtl), &sysPllCtl.u32);
		FbDiv = sysPllCtl.usysPllCtl_sysPllFbDiv;
		RfDiv = sysPllCtl.usysPllCtl_sysPllRfDiv;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPllCtl1), &sysPllCtl1.u32);
		VcoDivSel = sysPllCtl1.usysPllCtl_sysPllVcoDivSel;
		sysPll = (FbDiv + 2) * 25 / (RfDiv + 2) / (1 << VcoDivSel);
		mSocFreqVector.syspll = sysPll;
		if (mSocDivSaved.syspll ==0) mSocDivSaved.syspll = 1;
		HW_show_speed((" sysPLL       frequency %d\n", mSocFreqVector.syspll));
	}
	{
		// cpu1Pll
		T32Gbl_cpu1PllCtl	cpu1PllCtl;
		T32Gbl_cpu1PllCtl1	cpu1PllCtl1;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpu1PllCtl), &cpu1PllCtl.u32);
		FbDiv = cpu1PllCtl.ucpu1PllCtl_cpu1PllFbDiv;
		RfDiv = cpu1PllCtl.ucpu1PllCtl_cpu1PllRfDiv;
		if (RfDiv & 0xF)	// (M=REFDIV[3:0]+2, when REFDIV[3:0] not zero.)
			RfDiv = (RfDiv & 0xF) + 2;
		else				// (M=REFDIV[4]+1, when REFDIV[3:0] = zero.)
			RfDiv = (RfDiv >> 4) + 1;
		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpu1PllCtl1), &cpu1PllCtl1.u32);
		if (cpu1PllCtl1.ucpu1PllCtl_cpu1PllDiffClkEn)		// Differential clock output
			VcoDivSel = cpu1PllCtl1.ucpu1PllCtl_cpu1PllVcoDivSelDiff;
		else												// Single-ended clock output
			VcoDivSel = cpu1PllCtl1.ucpu1PllCtl_cpu1PllVcoDivSelSe;
		cpu1Pll = (FbDiv) * 25 / RfDiv;
//		cpu1Pll = (float)cpu1Pll / VcoDiv[VcoDivSel];
		HW_show_speed((" cpu1PLL	frequency %d\n", cpu1Pll));
	}
	// DDR clk
	HW_show_speed((" ddrClk		frequency %d\n", mSocFreqVector.ddr));

	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32);
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), &ClkSwitch1.u32);
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), &clkSelect.u32);
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect1.u32);
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), &clkSelect2.u32);
	mSocShadow_Switch = ClkSwitch.u32;
	mSocShadow_Switch1 = ClkSwitch1.u32;
	mSocShadow_Select = clkSelect.u32;
	mSocShadow_Select1 = clkSelect1.u32;
	mSocShadow_Select2 = clkSelect2.u32;
	// CPU1
	D3Switch = ClkSwitch.uClkSwitch_cpu1ClkD3Switch;
	Switch = ClkSwitch.uClkSwitch_cpu1ClkSwitch;
	Select = clkSelect.uclkSelect_cpu1ClkSel;
	divider = get_divider(D3Switch, Switch, Select);
	PllSel = clkSelect.uclkSelect_cpu1ClkPllSel;
	if(mSocDivSaved.cpu1 ==0) mSocDivSaved.cpu1 = divider;
	if (ClkSwitch.uClkSwitch_cpu1ClkPllSwitch) {		// AVPLL
		mSocFreqVector.cpu1 = avpll_b6_clock/divider ;
		HW_show_speed((" cpu1Clk 	frequency %d (use %s)\n", mSocFreqVector.cpu1, AVPLL[PllSel] ));
	} else {
		mSocFreqVector.cpu1 = cpu1Pll/divider;
		HW_show_speed((" cpu1Clk	frequency %d (use cpu1pll)\n", mSocFreqVector.cpu1));
	}
	
	// CPU0
	D3Switch = ClkSwitch.uClkSwitch_cpu0ClkD3Switch;
	Switch = ClkSwitch.uClkSwitch_cpu0ClkSwitch;
	Select = clkSelect.uclkSelect_cpu0ClkSel;
	divider = get_divider(D3Switch, Switch, Select);
	PllSel = clkSelect.uclkSelect_cpu0ClkPllSel;
	if( mSocDivSaved.cpu0 ==0) mSocDivSaved.cpu0 = divider;
	if (ClkSwitch.uClkSwitch_cpu0ClkPllSwitch) {		// AVPLL or cpu1Pll
		if (PllSel == 4) {
			mSocFreqVector.cpu0 = cpu1Pll/divider;
			HW_show_speed((" cpu0Clk	frequency %d (use cpu1Pll)\n", mSocFreqVector.cpu0));
		} else if (clkSelect.uclkSelect_cpu0ClkPllSel < 4) {
			mSocFreqVector.cpu0 = avpll_b6_clock/divider ; 
			HW_show_speed((" cpu0Clk	frequency %d (use %s)\n", mSocFreqVector.cpu0, AVPLL[PllSel]));
		} else {
			HW_show_speed((" cpu0Clk	invalid PllSel %d\n", PllSel));
		}
	} else {	// sysPll
		mSocFreqVector.cpu0 = sysPll/divider;
		HW_show_speed((" cpu0Clk	frequency %d (use sysPll)\n", mSocFreqVector.cpu0));
	}

	{
			// sysClk
		D3Switch = ClkSwitch.uClkSwitch_sysClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_sysClkSwitch;
		Select = clkSelect.uclkSelect_sysClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_sysClkPllSel;
		if(mSocDivSaved.sys == 0) mSocDivSaved.sys = divider;
		if (ClkSwitch.uClkSwitch_sysClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.sys = cpu1Pll/divider;
				HW_show_speed((" sysClk		frequency %d (use cpu1Pll)\n", mSocFreqVector.sys));
			} else if (PllSel < 4) {
				mSocFreqVector.sys = avpll_b6_clock/divider ;
				HW_show_speed((" sysClk		frequency %d (use  %s)\n", mSocFreqVector.sys, AVPLL[PllSel]));
			} else {
				HW_show_speed((" sysClk		invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.sys = sysPll/divider;
			HW_show_speed((" sysClk		frequency %d (use sysPll)\n", mSocFreqVector.sys));
		}

		// drmFigoClk
		D3Switch = ClkSwitch.uClkSwitch_drmFigoClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_drmFigoClkSwitch;
		Select = clkSelect.uclkSelect_drmFigoClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_drmFigoClkPllSel;
		if(mSocDivSaved.drm ==0) mSocDivSaved.drm = divider;
		if (ClkSwitch.uClkSwitch_drmFigoClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.drm = cpu1Pll/divider;
				HW_show_speed((" drmClk		frequency %d (use cpu1Pll)\n", mSocFreqVector.drm));
			} else if (PllSel < 4) {
				mSocFreqVector.drm = avpll_b6_clock/divider ;
				HW_show_speed((" drmClk		frequency %d (use %s)\n", mSocFreqVector.drm, AVPLL[PllSel]));
				mSocFreqVector.drm = avpll_b6_clock/divider ;
			} else {
				HW_show_speed((" drmClk		invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.drm = sysPll/divider;
			HW_show_speed((" drmClk		frequency %d (use sysPll)\n", mSocFreqVector.drm));
		}

			// cfgClk
		D3Switch = ClkSwitch.uClkSwitch_cfgClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_cfgClkSwitch;
		Select = clkSelect.uclkSelect_cfgClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_cfgClkPllSel;
		if(mSocDivSaved.cfg ==0) mSocDivSaved.cfg = divider;
		if (ClkSwitch.uClkSwitch_cfgClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.cfg = cpu1Pll/divider;
				HW_show_speed((" cfgClk		frequency %d (use cpu1Pll)\n", mSocFreqVector.cfg));
//				MV_Time_System_Timer_Clock = cpu1Pll * 1000 * 1000 / divider;								// it is used in GaloisTime.c
#if !defined(BERLIN_SINGLE_CPU)
#ifdef __ECOS__
#if	(CPUINDEX == 1)
//       			_CYGNUM_HAL_RTC_PERIOD_value = MV_Time_System_Timer_Clock / MV_OS_TICKNUM_PER_SECOND - 1;	// it is used in galois_misc.c
       			_CYGNUM_HAL_RTC_PERIOD_value = (cpu1Pll * 1000 * 1000 / divider) / MV_OS_TICKNUM_PER_SECOND - 1;	// it is used in galois_misc.c
#endif
#endif
#endif
			} else if (PllSel < 4) {
				mSocFreqVector.cfg = avpll_b6_clock/divider ;
				HW_show_speed((" cfgClk		frequency %d (use %s)\n", mSocFreqVector.cfg, AVPLL[PllSel]));
#if !defined(BERLIN_SINGLE_CPU)
#ifdef __ECOS__
#if	(CPUINDEX == 1)
//       			_CYGNUM_HAL_RTC_PERIOD_value = MV_Time_System_Timer_Clock / MV_OS_TICKNUM_PER_SECOND - 1;	// it is used in galois_misc.c
       			_CYGNUM_HAL_RTC_PERIOD_value = (avpll_b6_clock * 1000 * 1000 / divider) / MV_OS_TICKNUM_PER_SECOND - 1;	// it is used in galois_misc.c
#endif
#endif
#endif
			} else {
				HW_show_speed((" cfgClk		invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.cfg = sysPll/divider;
			HW_show_speed((" cfgClk		frequency %d (use sysPll)\n", mSocFreqVector.cfg));
//			MV_Time_System_Timer_Clock = sysPll * 1000 * 1000 / divider;								// it is used in GaloisTime.c
#if !defined(BERLIN_SINGLE_CPU)
#ifdef __ECOS__
#if	(CPUINDEX == 1)
//        	_CYGNUM_HAL_RTC_PERIOD_value = MV_Time_System_Timer_Clock / MV_OS_TICKNUM_PER_SECOND - 1;	// it is used in galois_misc.c
        	_CYGNUM_HAL_RTC_PERIOD_value = (sysPll * 1000 * 1000 / divider) / MV_OS_TICKNUM_PER_SECOND - 1;	// it is used in galois_misc.c
#endif
#endif
#endif
		}

		// gfxClk
		D3Switch = ClkSwitch.uClkSwitch_gfxClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_gfxClkSwitch;
		Select = clkSelect1.uclkSelect_gfxClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_gfxClkPllSel;
		if(mSocDivSaved.gfx ==0) mSocDivSaved.gfx = divider;
		if (ClkSwitch.uClkSwitch_gfxClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.gfx = cpu1Pll/divider;
				HW_show_speed((" gfxClk		frequency %d (use cpu1Pll)\n", mSocFreqVector.gfx));
			} else if (PllSel < 4) {
				mSocFreqVector.gfx = avpll_b6_clock/divider ;
				HW_show_speed((" gfxClk		frequency %d (use %s)\n", mSocFreqVector.gfx, AVPLL[PllSel]));
			} else {
				HW_show_speed((" gfxClk		invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.gfx = sysPll/divider;
			HW_show_speed((" gfxClk		frequency %d (use sysPll)\n", mSocFreqVector.gfx));
		}
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
        // zspClk
		D3Switch = ClkSwitch.uClkSwitch_zspClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_zspClkSwitch;
		Select = clkSelect1.uclkSelect_zspClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect1.uclkSelect_zspClkPllSel;
		if(mSocDivSaved.adsp ==0) mSocDivSaved.adsp = divider;
		if (ClkSwitch.uClkSwitch_zspClkPllSwitch) {       // AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.adsp = cpu1Pll/divider;
				HW_show_speed((" DSPClk		frequency %d (use cpu1Pll)\n", mSocFreqVector.adsp));
			} else if (PllSel < 4) {
				mSocFreqVector.adsp = avpll_b6_clock/divider ;
				HW_show_speed((" DSPClk		frequency %d (use %s)\n", mSocFreqVector.adsp, AVPLL[PllSel]));
				mSocFreqVector.adsp = avpll_b6_clock/divider ;
			} else {
				HW_show_speed((" DSPClk		invalid PllSel %d\n", PllSel));
			}
		} else {    // sysPll
			mSocFreqVector.adsp = sysPll/divider;
			HW_show_speed((" DSPClk		frequency %d (use sysPll)\n", mSocFreqVector.adsp));
		}
#endif

		// perifClk
		D3Switch = ClkSwitch.uClkSwitch_perifClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_perifClkSwitch;
		Select = clkSelect1.uclkSelect_perifClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect1.uclkSelect_perifClkPllSel;
		if(mSocDivSaved.perif ==0) mSocDivSaved.perif = divider;
		if (ClkSwitch.uClkSwitch_perifClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.perif = cpu1Pll/divider;
				HW_show_speed((" perifClk	frequency %d (use cpu1Pll)\n", mSocFreqVector.perif));
			} else if (PllSel < 4) {
				mSocFreqVector.perif = avpll_b6_clock/divider ;
				HW_show_speed((" perifClk	frequency %d (use %s)\n", mSocFreqVector.perif, AVPLL[PllSel]));
			} else {
				HW_show_speed((" perifClk	invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.perif = sysPll/divider;
			HW_show_speed((" perifClk	frequency %d (use sysPll)\n", mSocFreqVector.perif));
		}

			// pCubeClk
		D3Switch = ClkSwitch.uClkSwitch_pCubeClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_pCubeClkSwitch;
		Select = clkSelect1.uclkSelect_pCubeClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect1.uclkSelect_pCubeClkPllSel;
		if(mSocDivSaved.pcube ==0) mSocDivSaved.pcube = divider;
		if (ClkSwitch.uClkSwitch_pCubeClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.pcube = cpu1Pll/divider;
				HW_show_speed((" pCubeClk	frequency %d (use cpu1Pll)\n", mSocFreqVector.pcube));
			} else if (PllSel < 4) {
				mSocFreqVector.pcube = avpll_b6_clock ;
				HW_show_speed((" pCubeClk	frequency %d (use %s)\n", mSocFreqVector.pcube, AVPLL[PllSel]));
			} else {
				HW_show_speed((" pCubeClk	invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.pcube = sysPll/divider;
			HW_show_speed((" pCubeClk	frequency %d (use sysPll)\n", mSocFreqVector.pcube));
		}

#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		D3Switch = ClkSwitch1.uClkSwitch_vScopeClkD3Switch;
#else
		D3Switch = ClkSwitch.uClkSwitch_vScopeClkD3Switch;
#endif
		// vScopeClk
		Switch = ClkSwitch.uClkSwitch_vScopeClkSwitch;
		Select = clkSelect1.uclkSelect_vScopeClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect1.uclkSelect_vScopeClkPllSel;
		if(mSocDivSaved.vscope ==0) mSocDivSaved.vscope = divider;
		if (ClkSwitch.uClkSwitch_vScopeClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.vscope = cpu1Pll/divider;
				HW_show_speed((" vScopeClk	frequency %d (use cpu1Pll)\n", mSocFreqVector.vscope));
			} else if (PllSel < 4) {
				mSocFreqVector.vscope = avpll_b6_clock/divider ;
				HW_show_speed((" vScopeClk	frequency %d (use %s)\n",mSocFreqVector.vscope, AVPLL[PllSel]));

			} else {
				HW_show_speed((" vScopeClk	invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.vscope = sysPll/divider;
			HW_show_speed((" vScopeClk	frequency %d (use sysPll)\n", mSocFreqVector.vscope));
		}

			// nfcEccClk
		D3Switch = ClkSwitch1.uClkSwitch_nfcEccClkD3Switch;
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		Switch = ClkSwitch1.uClkSwitch_nfcEccClkSwitch;
		Select = clkSelect2.uclkSelect_nfcEccClkSel;
#else
		Switch = ClkSwitch.uClkSwitch_nfcEccClkSwitch;
		Select = clkSelect1.uclkSelect_nfcEccClkSel;
#endif
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect1.uclkSelect_nfcEccClkPllSel;
		if(mSocDivSaved.nfcecc ==0) mSocDivSaved.nfcecc = divider;
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		nfccswitch = ClkSwitch1.uClkSwitch_nfcEccClkPllSwitch;
#else
		nfccswitch = ClkSwitch.uClkSwitch_nfcEccClkPllSwitch;
#endif
		if (nfccswitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.nfcecc = cpu1Pll/divider;
				HW_show_speed((" nfcEccClk	frequency %d (use cpu1Pll)\n", mSocFreqVector.nfcecc));
			} else if (PllSel < 4) {
				mSocFreqVector.nfcecc = avpll_b6_clock/divider ;
				HW_show_speed((" nfcEccClk	frequency %d (use %s)\n", mSocFreqVector.nfcecc, AVPLL[PllSel]));

			} else {
				HW_show_speed((" nfcEccClk	invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.nfcecc = sysPll/divider;
			HW_show_speed((" nfcEccClk	frequency %d (use sysPll)\n", mSocFreqVector.nfcecc));
		}

		// vppSysClk
		D3Switch = ClkSwitch1.uClkSwitch_vppSysClkD3Switch;
		Switch = ClkSwitch1.uClkSwitch_vppSysClkSwitch;
		Select = clkSelect2.uclkSelect_vppSysClkSel;
		divider = get_divider(D3Switch, Switch, Select);
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		PllSel = clkSelect2.uclkSelect_vppSysClkPllSel;
#else
		PllSel = clkSelect1.uclkSelect_vppSysClkPllSel;
#endif
		if(mSocDivSaved.vpp ==0) mSocDivSaved.vpp = divider;
		if (ClkSwitch1.uClkSwitch_vppSysClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.vpp = cpu1Pll/divider;
				HW_show_speed((" vppSysClk	frequency %d (use cpu1Pll)\n", mSocFreqVector.vpp));
			} else if (PllSel < 4) {
				mSocFreqVector.vpp = avpll_b6_clock/divider ;
				HW_show_speed((" vppSysClk	frequency %d (use %s)\n", mSocFreqVector.vpp, AVPLL[PllSel]));
			} else {
				HW_show_speed((" vppSysClk	invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.vpp = sysPll/divider;
			HW_show_speed((" vppSysClk	frequency %d (use sysPll)\n", mSocFreqVector.vpp));
		}

		// appClk
		D3Switch = ClkSwitch1.uClkSwitch_appClkD3Switch;
		Switch = ClkSwitch1.uClkSwitch_appClkSwitch;
		Select = clkSelect2.uclkSelect_appClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect2.uclkSelect_appClkPllSel;
		if(mSocDivSaved.app ==0) mSocDivSaved.app = divider;
		if (ClkSwitch1.uClkSwitch_appClkPllSwitch) {		// AVPLL or cpu1Pll
			if (PllSel == 4) {
				mSocFreqVector.app = cpu1Pll/divider;
				HW_show_speed((" appClk		frequency %d (use cpu1Pll)\n", mSocFreqVector.app));
			} else if (PllSel < 4) {
				mSocFreqVector.app = avpll_b6_clock/divider ;
				HW_show_speed((" appClk		frequency %d (use %s)\n", mSocFreqVector.app, AVPLL[PllSel]));

			} else {
				HW_show_speed(( " appClk		invalid PllSel %d\n", PllSel));
			}
		} else {	// sysPll
			mSocFreqVector.app = sysPll/divider;
			HW_show_speed(( " appClk		frequency %d (use sysPll)\n", mSocFreqVector.app));
		}
	}
}

static void GetSettingFromDivider(unsigned int divider, unsigned int *D3Switch, unsigned int *Switch, unsigned int *Select)
{
	if(divider ==1)
	{
		*D3Switch = 0; 	// No use D3
		*Switch = 0;	// No Use divider
		*Select = 1;	// default 2
	}
	else if(divider ==2)
	{
		*D3Switch = 0; 	// No use D3
		*Switch = 1;	// Use divider
		*Select = 1;	// default 2
	}
	else if(divider ==3)
	{
		*D3Switch = 1; 	// Use D3
		*Switch = 1;	// Use divider
		*Select = 1;	// default 2
	} 
	else 
	{
		*D3Switch = 0; 	// No use D3
		*Switch = 1;	// Use divider
		if(divider >= 12 ) 
			*Select = 5; // div 12
		else 
			*Select  = divider/2;
	}
}

static volatile int mv_in_clock_changing =0; // for multi task protection.
static volatile int mv_in_lower_vmeta =0; // for multi task protection.
static volatile int mv_in_upper_vmeta =0; // for multi task protection.
static volatile int mv_in_lower_zsp =0; // for multi task protection.
static volatile int mv_in_upper_zsp =0; // for multi task protection.


UNSG32 Dynamic_Lower_vMeta_Speed(void)
{
#if (BERLIN_CHIP_VERSION < BERLIN_B_0)
#elif (BERLIN_CHIP_VERSION < BERLIN_BG2)

#ifdef DYNAMIC_POWER_CONTROL
	unsigned int divider;
	unsigned int D3Switch, Switch, Select;
	unsigned int PllSel;
	T32Gbl_ClkSwitch	*ClkSwitch;
	T32Gbl_ClkSwitch1	*ClkSwitch1;
	T32Gbl_clkSelect1	*clkSelect1;
	if(mv_in_lower_vmeta || mv_in_upper_vmeta || mv_in_lower_zsp || mv_in_upper_zsp) return -1;
	mv_in_lower_vmeta++;
	if(mv_in_clock_changing) 
	{
		mv_in_lower_vmeta--;
		return -1;
	}
	mv_in_clock_changing ++;
	if(mSocOrigDivSaved.pcube == 0)
	{
//HW_show_speed(" lower vMeta Clock \n");
		mSocOrigDivSaved.vscope = mSocDivSaved.vscope;
	  	mSocOrigDivSaved.pcube = mSocDivSaved.pcube;
	  	ClkSwitch = (T32Gbl_ClkSwitch	*)&mSocShadow_Switch;
	  	ClkSwitch1 = (T32Gbl_ClkSwitch1	*)&mSocShadow_Switch1;
	  	clkSelect1 = (T32Gbl_clkSelect1	*)&mSocShadow_Select1;
//	  	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32);
//		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), &ClkSwitch1.u32);
//		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect1.u32);
		// pCubeClk
		if (ClkSwitch ->uClkSwitch_pCubeClkPllSwitch && clkSelect1 ->uclkSelect_pCubeClkPllSel!= 4) 
		{ // Source from AVPLL Have to adjutst AVPLL, no appliable
//HW_show_speed(" error AVPLL \n");			
		}
		else
		{
			ClkSwitch ->uClkSwitch_pCubeClkD3Switch = 0; 	// No use D3
			ClkSwitch ->uClkSwitch_pCubeClkSwitch = 1;	// Use divider
			if(mSocDivSaved.pcube < 8)
			{
				clkSelect1 ->uclkSelect_pCubeClkSel = 4;  	// 4: 1/8; 5:1/12
				mSocDivSaved.pcube = 8;
			} else
			{
				clkSelect1->uclkSelect_pCubeClkSel = 5;  	// 5:1/12
				mSocDivSaved.pcube = 12;
			}
			mSocFreqVector.pcube = mSocFreqVector.pcube*mSocOrigDivSaved.pcube/mSocDivSaved.pcube;
		}
		
		// vScope clk
		if (ClkSwitch ->uClkSwitch_vScopeClkPllSwitch && clkSelect1 ->uclkSelect_vScopeClkPllSel != 4) 
		{ // Source from AVPLL Have to adjutst AVPLL, no appliable
//HW_show_speed(" error AVPLL \n");
		}
		else
		{
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
			ClkSwitch1 ->uClkSwitch_vScopeClkD3Switch =0;	// Not D3
#else
			ClkSwitch ->uClkSwitch_vScopeClkD3Switch =0;	 //Not D3
#endif
			ClkSwitch ->uClkSwitch_vScopeClkSwitch = 1;	// Use divider
			if(mSocDivSaved.vscope < 8)
			{
				clkSelect1 ->uclkSelect_vScopeClkSel = 4;  	// 4: 1/8; 5:1/12
				mSocDivSaved.vscope = 8;
			} else
			{
				clkSelect1 ->uclkSelect_vScopeClkSel = 5;  	// 5:1/12
				mSocDivSaved.vscope = 12;
			}
			mSocFreqVector.vscope = mSocFreqVector.vscope*mSocOrigDivSaved.vscope/mSocDivSaved.vscope;
		}
#if 0
HW_show_speed("Low vMeta slect/D3/swith(%x-%x-%x) (%x-%x-%x) \n",
 clkSelect1->uclkSelect_vScopeClkSel, ClkSwitch1->uClkSwitch_vScopeClkD3Switch, ClkSwitch->uClkSwitch_vScopeClkSwitch,
 clkSelect1->uclkSelect_pCubeClkSel, ClkSwitch->uClkSwitch_pCubeClkD3Switch, ClkSwitch->uClkSwitch_pCubeClkSwitch);
#endif
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), mSocShadow_Select1);
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), mSocShadow_Switch1);
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), mSocShadow_Switch);
	}
#endif
	mv_in_clock_changing --;
	mv_in_lower_vmeta --;
	return 0;
#endif
}
UNSG32 Dynamic_Recover_vMeta_Speed(void)
{
#if (BERLIN_CHIP_VERSION < BERLIN_B_0)
#elif (BERLIN_CHIP_VERSION < BERLIN_BG2)

#ifdef DYNAMIC_POWER_CONTROL
	unsigned int divider;
	unsigned int D3Switch, Switch, Select;
	unsigned int PllSel;
	T32Gbl_ClkSwitch	*ClkSwitch;
	T32Gbl_ClkSwitch1	*ClkSwitch1;
	T32Gbl_clkSelect1	*clkSelect1;
	if(mv_in_lower_vmeta || mv_in_upper_vmeta || mv_in_lower_zsp || mv_in_upper_zsp) return -1;
	mv_in_upper_vmeta++;
	if(mv_in_clock_changing)
	{
		mv_in_upper_vmeta--;
		return -1;
	}
	mv_in_clock_changing ++;
	if(mSocOrigDivSaved.pcube)
	{
		ClkSwitch = (T32Gbl_ClkSwitch	*)&mSocShadow_Switch;
	  	ClkSwitch1 = (T32Gbl_ClkSwitch1	*)&mSocShadow_Switch1;
	  	clkSelect1 = (T32Gbl_clkSelect1	*)&mSocShadow_Select1;
//	  	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32);
//		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), &ClkSwitch1.u32);
//		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect1.u32);
		// pCubeClk
		if (ClkSwitch ->uClkSwitch_pCubeClkPllSwitch && clkSelect1 ->uclkSelect_pCubeClkPllSel!= 4)
		{ // Source from AVPLL Have to adjutst AVPLL, no appliable	
		}
		else
		{
			GetSettingFromDivider(mSocOrigDivSaved.pcube, &D3Switch, &Switch, &Select);
			ClkSwitch ->uClkSwitch_pCubeClkD3Switch = D3Switch; 	// D3
			ClkSwitch ->uClkSwitch_pCubeClkSwitch = Switch;	//  divider
			clkSelect1 ->uclkSelect_pCubeClkSel = Select;		// default 2
			mSocFreqVector.pcube = mSocFreqVector.pcube*mSocDivSaved.pcube/mSocOrigDivSaved.pcube;
		}

		// vScope clk
		if (ClkSwitch ->uClkSwitch_vScopeClkPllSwitch && clkSelect1 ->uclkSelect_vScopeClkPllSel != 4) 
		{ // Source from AVPLL Have to adjutst AVPLL, no appliable
		}
		else
		{
			GetSettingFromDivider(mSocOrigDivSaved.vscope, &D3Switch, &Switch, &Select);
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
			ClkSwitch1 ->uClkSwitch_vScopeClkD3Switch = D3Switch;	// Not D3
#else
			ClkSwitch ->uClkSwitch_vScopeClkD3Switch = D3Switch;	 //Not D3
#endif
			ClkSwitch ->uClkSwitch_vScopeClkSwitch = Switch;	// Use divider
			clkSelect1->uclkSelect_vScopeClkSel = Select;  	// Select
			mSocFreqVector.vscope = mSocFreqVector.vscope*mSocDivSaved.vscope/mSocOrigDivSaved.vscope;
		}
#if 0
HW_show_speed("Recover vMeta slect/D3/swith(%x-%x-%x) (%x-%x-%x) \n",
 clkSelect1->uclkSelect_vScopeClkSel, ClkSwitch1->uClkSwitch_vScopeClkD3Switch, ClkSwitch->uClkSwitch_vScopeClkSwitch,
 clkSelect1->uclkSelect_pCubeClkSel, ClkSwitch->uClkSwitch_pCubeClkD3Switch, ClkSwitch->uClkSwitch_pCubeClkSwitch);
#endif
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), mSocShadow_Select1);
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), mSocShadow_Switch1);
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), mSocShadow_Switch);	
		mSocDivSaved.vscope = mSocOrigDivSaved.vscope;
	  	mSocDivSaved.pcube = mSocOrigDivSaved.pcube;
		mSocOrigDivSaved.vscope = 0;
	  	mSocOrigDivSaved.pcube = 0;
	}
#endif
	mv_in_clock_changing --;
	mv_in_upper_vmeta --;
	return 0;
#endif
}

UNSG32  Dynamic_Lower_ZSP_Speed(void)
{
#if (BERLIN_CHIP_VERSION < BERLIN_B_0)
#elif (BERLIN_CHIP_VERSION < BERLIN_BG2)
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)

#ifdef DYNAMIC_POWER_CONTROL
	T32Gbl_ClkSwitch	*ClkSwitch;
	T32Gbl_clkSelect1	*clkSelect1;
	if(mv_in_lower_vmeta || mv_in_upper_vmeta || mv_in_lower_zsp || mv_in_upper_zsp) return -1;
	mv_in_lower_zsp++;
	if(mv_in_clock_changing) 
	{
		mv_in_lower_zsp--;
		return -1;
	}
	mv_in_clock_changing ++;
	if(mSocOrigDivSaved.adsp == 0)
	{
//HW_show_speed(" lower ZSP Clock \n");
		mSocOrigDivSaved.adsp = mSocDivSaved.adsp;
		ClkSwitch = (T32Gbl_ClkSwitch	*)&mSocShadow_Switch;
	  	clkSelect1 = (T32Gbl_clkSelect1	*)&mSocShadow_Select1;
//	  	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32);
//		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect1.u32);
		if (ClkSwitch->uClkSwitch_zspClkPllSwitch && clkSelect1 ->uclkSelect_zspClkPllSel!= 4) 
		{ // Source from AVPLL Have to adjutst AVPLL, no appliable
			
		}
		else
		{
			ClkSwitch ->uClkSwitch_zspClkD3Switch = 0; 	// No use D3
			ClkSwitch ->uClkSwitch_zspClkSwitch = 1;	// Use divider
			if(mSocDivSaved.adsp < 8)
			{
				clkSelect1 ->uclkSelect_zspClkSel = 4;  	// 4: 1/8; 5:1/12
				mSocDivSaved.adsp = 8;
			} else
			{
				clkSelect1 ->uclkSelect_zspClkSel = 5;  	// 5:1/12
				mSocDivSaved.adsp = 12;
			}
			mSocFreqVector.adsp = mSocFreqVector.adsp*mSocOrigDivSaved.adsp/mSocDivSaved.adsp;
		}
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), mSocShadow_Select1);
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), mSocShadow_Switch);
	}
#endif
	mv_in_clock_changing --;
	mv_in_lower_zsp --;
	return 0;
#endif
#endif	
}
UNSG32 Dynamic_Recover_ZSP_Speed(void)
{
#if (BERLIN_CHIP_VERSION < BERLIN_B_0)
#elif (BERLIN_CHIP_VERSION < BERLIN_BG2)
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)

#ifdef DYNAMIC_POWER_CONTROL
	unsigned int divider;
	unsigned int D3Switch, Switch, Select;
	unsigned int PllSel;
	
	T32Gbl_ClkSwitch	*ClkSwitch;
	T32Gbl_clkSelect1	*clkSelect1;
	if(mv_in_lower_vmeta || mv_in_upper_vmeta || mv_in_lower_zsp || mv_in_upper_zsp) return -1;
	mv_in_upper_zsp++;
	if(mv_in_clock_changing)
	{
		mv_in_upper_zsp--;
		return -1;
	}
	mv_in_clock_changing ++;
	if(mSocOrigDivSaved.adsp)
	{
//HW_show_speed("Recover ZSP Clock \n");
		ClkSwitch = (T32Gbl_ClkSwitch	*)&mSocShadow_Switch;
	  	clkSelect1 = (T32Gbl_clkSelect1	*)&mSocShadow_Select1;
//	  	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32);
//		GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect1.u32);
		if (ClkSwitch ->uClkSwitch_zspClkPllSwitch && clkSelect1 ->uclkSelect_zspClkPllSel!= 4)
		{ // Source from AVPLL Have to adjutst AVPLL, no appliable			
		}
		else
		{
			GetSettingFromDivider(mSocOrigDivSaved.adsp, &D3Switch, &Switch, &Select);
			ClkSwitch -> uClkSwitch_zspClkD3Switch = D3Switch; 	// D3
			ClkSwitch -> uClkSwitch_zspClkSwitch = Switch;	//  divider
			clkSelect1 -> uclkSelect_zspClkSel = Select;		// default 2
			mSocFreqVector.adsp = mSocFreqVector.adsp*mSocDivSaved.adsp/mSocOrigDivSaved.adsp;
		}
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), mSocShadow_Select1);
		GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), mSocShadow_Switch);
		mSocDivSaved.adsp = mSocOrigDivSaved.adsp;
		mSocOrigDivSaved.adsp = 0;
	}
#endif
	mv_in_clock_changing --;
	mv_in_upper_zsp --;
	return 0;
#endif
#endif

}

UNSG32 GaloisGetCPUDivider(unsigned int * cpu0_clock_divider,
						   unsigned int * cpu1_clock_divider)
{
#if (BERLIN_CHIP_VERSION < BERLIN_B_0)
#elif (BERLIN_CHIP_VERSION < BERLIN_BG2)
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)

	*cpu0_clock_divider = mSocDivSaved.cpu0 ; 
	*cpu1_clock_divider = mSocDivSaved.cpu1 ;
	return 0;
#endif
#endif

}

#else
//For BG2 Z1, Z2 , A0

static int diag_pll_B_VCO_Setting = VCO_FREQ_1_620G;

static int get_diag_pll_B_VCO_Setting(void)
{
	int i;
	T32Gbl_avPllCtl32 regData32;
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl32), &regData32.u32);
	unsigned int FbDiv = regData32.uavPllCtl_avPllFbDiv_B;
	for (i = 0; i < sizeof(diag_avpllRegFBDIV) / sizeof(diag_avpllRegFBDIV[0]); i++) {
		if (FbDiv == diag_avpllRegFBDIV[i])
			return i;
	}
	// return the default value
	return VCO_FREQ_1_620G;
}

//all the VCO freq reqired for video and audio in MHz
static int diag_vcoFreqs[]=
{
	1080, 	//8 bit HDMI
	1260,	//10 bit HDMI
	1350,	//10 bit HDMI
	1485,	//8 bit HDMI
	1512, 	//8 bit HDMI	and 12 bit HDMI
	1620,	//12 bit HDMI
	1856.25,//10 bit HDMI
	2227.5,	//12 bit HDMI
};

static int divHDMICode[] = 
{
	1, 2, 4, 6
};

static int divAV1Code[] =
{
	1, 2, 5, 5
};

static int IS_REVERT()
{
#if 0
    //Ax and B0 chip AVPLLB channels are reverted
    int reg1;
    BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ProductId_ext), &reg1);
    return ( reg1 >= 0xA0 );
#else
	return 1;
#endif
}

static void diag_avpllFreq_B(int chId)
{
	int vco_freq=diag_vcoFreqs[diag_pll_B_VCO_Setting];
	int freq_from_dpll;
	int offset, sync1, sync2, divHDMI, divAV1, divAV2, divAV3;
	int enDPLL;
    int is_a0;
	
	T32Gbl_avPllCtl42 regData42;
	T32Gbl_avPllCtl43 regData43;
	T32Gbl_avPllCtl44 regData44;
	T32Gbl_avPllCtl45 regData45;
	T32Gbl_avPllCtl46 regData46;
	T32Gbl_avPllCtl61 regData61;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl42), &regData42.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl43), &regData43.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl44), &regData44.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl45), &regData45.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl46), &regData46.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl61), &regData61.u32);
	
	enDPLL = regData61.uavPllCtl_avPllEnDpllC_B & (1<<(chId-1));
	
//    HW_show_speed(("---------------------------------------\n"));
	//find offset, sync1, sync2, divHDMI, divAV1, divAV2, divAV3
	switch(chId)
	{
		case 1:
		{
			T32Gbl_avPllCtl33 regData33;
			T32Gbl_avPllCtl46 regData46;
			T32Gbl_avPllCtl54 regData54;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl33), &regData33.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl46), &regData46.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl54), &regData54.u32);
			
			offset = regData33.uavPllCtl_avPllFreqOffsetC1_B;
			sync1 =  regData46.uavPllCtl_avPllPSync1C1_B;
			sync2 =  regData54.uavPllCtl_avPllPSync2C1_B;
			divHDMI= regData42.uavPllCtl_avPllDivHdmiC1_B;
			divAV1 = regData42.uavPllCtl_avPllDivAv1C1_B;
			divAV2 = regData43.uavPllCtl_avPllDivAv2C1_B;
			divAV3 = regData45.uavPllCtl_avPllDivAv3C1_B;
		}
		break;
		case 2:
		{
			T32Gbl_avPllCtl34 regData34;
			T32Gbl_avPllCtl47 regData47;
			T32Gbl_avPllCtl55 regData55;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl34), &regData34.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl47), &regData47.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl55), &regData55.u32);
			
			offset = regData34.uavPllCtl_avPllFreqOffsetC2_B;
			sync1 =  regData47.uavPllCtl_avPllPSync1C2_B;
			sync2 =  regData55.uavPllCtl_avPllPSync2C2_B;
			divHDMI= regData42.uavPllCtl_avPllDivHdmiC2_B;
			divAV1 = regData43.uavPllCtl_avPllDivAv1C2_B;
			divAV2 = regData43.uavPllCtl_avPllDivAv2C2_B;
			divAV3 = regData45.uavPllCtl_avPllDivAv3C2_B;
		}
		break;
		case 3:
		{
			T32Gbl_avPllCtl35 regData35;
			T32Gbl_avPllCtl48 regData48;
			T32Gbl_avPllCtl56 regData56;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl35), &regData35.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl48), &regData48.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl56), &regData56.u32);
			offset = regData35.uavPllCtl_avPllFreqOffsetC3_B;
			sync1 =  regData48.uavPllCtl_avPllPSync1C3_B;
			sync2 =  regData56.uavPllCtl_avPllPSync2C3_B;
			divHDMI= regData42.uavPllCtl_avPllDivHdmiC3_B;
			divAV1 = regData43.uavPllCtl_avPllDivAv1C3_B;
			divAV2 = regData44.uavPllCtl_avPllDivAv2C3_B;
			divAV3 = regData45.uavPllCtl_avPllDivAv3C3_B;
		}
		break;
		case 4:
		{
			T32Gbl_avPllCtl36 regData36;
			T32Gbl_avPllCtl49 regData49;
			T32Gbl_avPllCtl57 regData57;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl36), &regData36.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl49), &regData49.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl57), &regData57.u32);
			offset = regData36.uavPllCtl_avPllFreqOffsetC4_B;
			sync1 =  regData49.uavPllCtl_avPllPSync1C4_B;
			sync2 =  regData57.uavPllCtl_avPllPSync2C4_B;
			divHDMI= regData42.uavPllCtl_avPllDivHdmiC4_B;
			divAV1 = regData43.uavPllCtl_avPllDivAv1C4_B;
			divAV2 = regData44.uavPllCtl_avPllDivAv2C4_B;
			divAV3 = regData45.uavPllCtl_avPllDivAv3C4_B;
		}
		break;
		case 5:
		{
			T32Gbl_avPllCtl37 regData37;
			T32Gbl_avPllCtl50 regData50;
			T32Gbl_avPllCtl58 regData58;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl37), &regData37.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl50), &regData50.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl58), &regData58.u32);
			offset = regData37.uavPllCtl_avPllFreqOffsetC5_B;
			sync1 =  regData50.uavPllCtl_avPllPSync1C5_B;
			sync2 =  regData58.uavPllCtl_avPllPSync2C5_B;
			divHDMI= regData42.uavPllCtl_avPllDivHdmiC5_B;
			divAV1 = regData43.uavPllCtl_avPllDivAv1C5_B;
			divAV2 = regData44.uavPllCtl_avPllDivAv2C5_B;
			divAV3 = regData45.uavPllCtl_avPllDivAv3C5_B;
		}
		break;
		case 6:
		{
			T32Gbl_avPllCtl38 regData38;
			T32Gbl_avPllCtl51 regData51;
			T32Gbl_avPllCtl59 regData59;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl38), &regData38.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl51), &regData51.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl59), &regData59.u32);
			offset = regData38.uavPllCtl_avPllFreqOffsetC6_B;
			sync1 =  regData51.uavPllCtl_avPllPSync1C6_B;
			sync2 =  regData59.uavPllCtl_avPllPSync2C6_B;
			divHDMI= regData42.uavPllCtl_avPllDivHdmiC6_B;
			divAV1 = regData43.uavPllCtl_avPllDivAv1C6_B;
			divAV2 = regData44.uavPllCtl_avPllDivAv2C6_B;
			divAV3 = regData45.uavPllCtl_avPllDivAv3C6_B;
		}
		break;
		case 7:
		{
			T32Gbl_avPllCtl39 regData39;
			T32Gbl_avPllCtl52 regData52;
			T32Gbl_avPllCtl60 regData60;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl39), &regData39.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl52), &regData52.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl60), &regData60.u32);
			offset = regData39.uavPllCtl_avPllFreqOffsetC7_B;
			sync1 =  regData52.uavPllCtl_avPllPSync1C7_B;
			sync2 =  regData60.uavPllCtl_avPllPSync2C7_B;
			divHDMI= regData42.uavPllCtl_avPllDivHdmiC7_B;
			divAV1 = regData43.uavPllCtl_avPllDivAv1C7_B;
			divAV2 = regData45.uavPllCtl_avPllDivAv2C7_B;
			divAV3 = regData46.uavPllCtl_avPllDivAv3C7_B;
		}
		break;
		default:
			HW_show_speed(("Invalid Channel\n"));
			return;
		
	}
	
//	HW_show_speed(("vco %d, sync1 %d, sync2 %dMHz(DPLL)\n", vco_freq, sync1, sync2));
	if(enDPLL != 0)
		freq_from_dpll = vco_freq*sync2/sync1;
	else
		freq_from_dpll = vco_freq;
	
	if(offset& (1<<18))
	{
		offset = - (offset & ((1<<18)-1));
	}
//	freq_from_offset = vco_freq/((double)offset/4194304+1);
	
	//figure out which divider used
	if(divHDMI != 0)
	{
		divHDMI &= 0x3;
//		HW_show_speed(("divHDMI on: %d\n", divHDMI));
		
		freq_from_dpll = freq_from_dpll/divHDMICode[divHDMI];
//		freq_from_offset = freq_from_offset/divHDMICode[divHDMI];
	}
	
	if(divAV1 != 0)
	{
		divAV1 &= 0x3;
//		HW_show_speed(("divAV1 on: %d\n", divAV1));
		
		freq_from_dpll = freq_from_dpll/divAV1Code[divAV1];
//		freq_from_offset = freq_from_offset/divAV1Code[divAV1];
	}
	
	if(divAV2 != 0)
	{
		if(divAV3 == 0)
		{
//			HW_show_speed(("divAV2 on: %d\n", divAV2));
			freq_from_dpll = freq_from_dpll/divAV2;
//			freq_from_offset = freq_from_offset/divAV2;
		}
		else
		{
//			HW_show_speed(("divAV2 on: %d\n", divAV2));
//			HW_show_speed(("divAV3 on: %d\n", divAV3));
//			if( (((int)((double)divAV2/4+.5))-1) != (divAV3&0x7))
//			{
//				HW_show_speed(("Invalid divAV2 and divAV3 combination\n"));
//			}
			freq_from_dpll = freq_from_dpll*2/divAV2;
//			freq_from_offset = freq_from_offset*2/divAV2;
		}
	}
	
//	HW_show_speed(("PLLB C%d Freq is: %dMHz(DPLL)\n", chId, freq_from_dpll));
	
    if(IS_REVERT()) {
        //A1
        if((chId == 7)|| (chId == 0))
	        current_freq[1][chId]=freq_from_dpll;
        else
            //only 1 ~ 6 are reverted
	        current_freq[1][7-chId]=freq_from_dpll;
    } else
	    current_freq[1][chId]=freq_from_dpll;
}

void analyze_speed(void)
{
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
	unsigned int speed_cpu;
    int is_a0;
    int i;
    diag_pll_B_VCO_Setting = get_diag_pll_B_VCO_Setting();
    for(i=0;i<8;i++){
        current_freq[0][i]=0;
        current_freq[1][i]=0;
    }

    if(IS_REVERT()) {
        //A1
	    diag_avpllFreq_B(3);
	    diag_avpllFreq_B(2);
	    diag_avpllFreq_B(1);
	    diag_avpllFreq_B(7);
    } else {
	    diag_avpllFreq_B(4);
	    diag_avpllFreq_B(5);
	    diag_avpllFreq_B(6);
	    diag_avpllFreq_B(7);
    }
	// memPll
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_memPllCtl), &memPllCtl.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_memPllCtl1), &memPllCtl.u32[1]);
	FbDiv = memPllCtl.umemPllCtl_memPllFbDiv;
	RfDiv = memPllCtl.umemPllCtl_memPllRfDiv;
	vcoDiv_sel_setting = memPllCtl.umemPllCtl_memPllVcoDivSelDiff;
	memPll = FbDiv * RefClkIn / RfDiv;
//	memPll = (float)memPll / VcoDiv[vcoDiv_sel_setting];
	memPll = VcoDivA_B[2*vcoDiv_sel_setting + 1] * memPll / VcoDivA_B[2*vcoDiv_sel_setting];
	mSocFreqVector.mempll = memPll;

	// sysPll
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPllCtl), &sysPllCtl.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sysPllCtl1), &sysPllCtl.u32[1]);
	FbDiv = sysPllCtl.usysPllCtl_sysPllFbDiv;
	RfDiv = sysPllCtl.usysPllCtl_sysPllRfDiv;
	vcoDiv_sel_setting = sysPllCtl.usysPllCtl_sysPllVcoDivSelSe;
	sysPll = FbDiv * RefClkIn / RfDiv;
//	sysPll = (float)sysPll / VcoDiv[vcoDiv_sel_setting];
	sysPll = VcoDivA_B[2*vcoDiv_sel_setting + 1] * sysPll / VcoDivA_B[2*vcoDiv_sel_setting];
	mSocFreqVector.syspll = sysPll;

	// cpuPll
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpuPllCtl), &cpuPllCtl.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_cpuPllCtl1), &cpuPllCtl.u32[1]);
	FbDiv = cpuPllCtl.ucpuPllCtl_cpuPllFbDiv;
	RfDiv = cpuPllCtl.ucpuPllCtl_cpuPllRfDiv;
	vcoDiv_sel_setting = cpuPllCtl.ucpuPllCtl_cpuPllVcoDivSelSe;
	cpuPll = FbDiv * RefClkIn / RfDiv;
//	cpuPll = (float)cpuPll / VcoDiv[vcoDiv_sel_setting];
	cpuPll = VcoDivA_B[2*vcoDiv_sel_setting + 1] * cpuPll / VcoDivA_B[2*vcoDiv_sel_setting];
	mSocFreqVector.cpupll = cpuPll;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch), &ClkSwitch.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), &ClkSwitch.u32[1]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect), &clkSelect.u32[0]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect1), &clkSelect.u32[1]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), &clkSelect.u32[2]);

	// DDR
	mSocFreqVector.ddr = memPll/4;

	// CPU
	D3Switch = ClkSwitch.uClkSwitch_cpu0ClkD3Switch;
	Switch = ClkSwitch.uClkSwitch_cpu0ClkSwitch;
	Select = clkSelect.uclkSelect_cpu0ClkSel;
	divider = get_divider(D3Switch, Switch, Select);
	PllSel = clkSelect.uclkSelect_cpu0ClkPllSel;
	if (ClkSwitch.uClkSwitch_cpu0ClkPllSwitch)		// AVPLL or memPll
	{
		if (PllSel == 4)			// memPll
			speed_cpu = memPll / divider;
		else
			speed_cpu = current_freq[1][PllSel + 4] / divider;
	}
	else
		speed_cpu = cpuPll / divider;

	mSocFreqVector.cpu0 = speed_cpu;

	if (1)
	{
		T32Gbl_gfx3DCoreClkCtrl gfx3DCoreClkCtrl;
		T32Gbl_gfx3DSysClkCtrl gfx3DSysClkCtrl;
		T32Gbl_arcRefClkCtrl arcRefClkCtrl;
//		T32Gbl_hdmirxFpllRefClkCtrl hdmirxFpllRefClkCtrl;
//		T32Gbl_hdmirxTclkCtrl hdmirxTclkCtrl;
		T32Gbl_vipClkCtrl vipClkCtrl;
		T32Gbl_sdioXinClkCtrl sdioXinClkCtrl;
		T32Gbl_sdio1XinClkCtrl sdio1XinClkCtrl;
		T32Gbl_gfx3DExtraClkCtrl gfx3DExtraClkCtrl;
		T32Gbl_gc360ClkCtrl gc360ClkCtrl;
		// sysClk
		D3Switch = ClkSwitch.uClkSwitch_sysClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_sysClkSwitch;
		Select = clkSelect.uclkSelect_sysClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_sysClkPllSel;
		if (ClkSwitch.uClkSwitch_sysClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.sys = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.sys = 1;
		else {	// sysPll
			mSocFreqVector.sys = sysPll / divider;
		}

		// drmFigoClk
		D3Switch = ClkSwitch.uClkSwitch_drmFigoClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_drmFigoClkSwitch;
		Select = clkSelect.uclkSelect_drmFigoClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_drmFigoClkPllSel;
		if (ClkSwitch.uClkSwitch_drmFigoClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.drm = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.drm = 1;
		else {	// sysPll
			mSocFreqVector.drm = sysPll/divider;
		}

		// cfgClk
		D3Switch = ClkSwitch.uClkSwitch_cfgClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_cfgClkSwitch;
		Select = clkSelect.uclkSelect_cfgClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_cfgClkPllSel;
		if (ClkSwitch.uClkSwitch_cfgClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.cfg = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.cfg = 1;
		else {	// sysPll
			mSocFreqVector.cfg = sysPll / divider;
		}

		// gfxClk
		D3Switch = ClkSwitch.uClkSwitch_gfxClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_gfxClkSwitch;
		Select = clkSelect.uclkSelect_gfxClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_gfxClkPllSel;
		if (ClkSwitch.uClkSwitch_gfxClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.gfx = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.gfx = 1;
		else {	// sysPll
			mSocFreqVector.gfx = sysPll / divider;
		}

		// zspClk
		D3Switch = ClkSwitch.uClkSwitch_zspClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_zspClkSwitch;
		Select = clkSelect.uclkSelect_zspClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_zspClkPllSel;
		if (ClkSwitch.uClkSwitch_zspClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.adsp = current_freq[1][PllSel + 4] / divider;
			} else 
				mSocFreqVector.adsp = 1;
		else {	// sysPll
			mSocFreqVector.adsp = sysPll / divider;
		}

		// perifClk
		D3Switch = ClkSwitch.uClkSwitch_perifClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_perifClkSwitch;
		Select = clkSelect.uclkSelect_perifClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_perifClkPllSel;
		if (ClkSwitch.uClkSwitch_perifClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.perif = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.perif = 1;
		else {	// sysPll
			mSocFreqVector.perif = sysPll/divider;
		}

		// pCubeClk
		D3Switch = ClkSwitch.uClkSwitch_pCubeClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_pCubeClkSwitch;
		Select = clkSelect.uclkSelect_pCubeClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_pCubeClkPllSel;
		if (ClkSwitch.uClkSwitch_pCubeClkPllSwitch)		// AVPLL 
			if (PllSel < 4) {
				mSocFreqVector.pcube = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.pcube = 1;
		else {	// sysPll
			mSocFreqVector.pcube = sysPll / divider;
		}

		// vScopeClk
		D3Switch = ClkSwitch.uClkSwitch_vScopeClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_vScopeClkSwitch;
		Select = clkSelect.uclkSelect_vScopeClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_vScopeClkPllSel;
		if (ClkSwitch.uClkSwitch_vScopeClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.vscope = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.vscope = 1;
		else {	// sysPll
			mSocFreqVector.vscope = sysPll / divider;
		}

		// nfcEccClk
		D3Switch = ClkSwitch.uClkSwitch_nfcEccClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_nfcEccClkSwitch;
		Select = clkSelect.uclkSelect_nfcEccClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_nfcEccClkPllSel;
		if (ClkSwitch.uClkSwitch_nfcEccClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.nfcecc = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.nfcecc = 1;
		else {	// sysPll
			mSocFreqVector.nfcecc = sysPll / divider;
		}

		// vppSysClk
		D3Switch = ClkSwitch.uClkSwitch_vppSysClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_vppSysClkSwitch;
		Select = clkSelect.uclkSelect_vppSysClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_vppSysClkPllSel;
		if (ClkSwitch.uClkSwitch_vppSysClkPllSwitch)		// AVPLL
			if (PllSel < 4){
				 mSocFreqVector.vpp = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.vpp = 1;
		else {	// sysPll
			 mSocFreqVector.vpp = sysPll / divider;
		}

		// appClk
		D3Switch = ClkSwitch.uClkSwitch_appClkD3Switch;
		Switch = ClkSwitch.uClkSwitch_appClkSwitch;
		Select = clkSelect.uclkSelect_appClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = clkSelect.uclkSelect_appClkPllSel;
		if (ClkSwitch.uClkSwitch_appClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				 mSocFreqVector.app = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.app = 1;
		else {	// sysPll
			mSocFreqVector.app = sysPll / divider;
		}

		// gfx3DCoreClk
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DCoreClkCtrl), &gfx3DCoreClkCtrl.u32);
		D3Switch = gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkD3Switch;
		Switch = gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkSwitch;
		Select = gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkPllSel;
		if (gfx3DCoreClkCtrl.ugfx3DCoreClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.gfx3dcore = current_freq[1][PllSel + 4] / divider;
			}else
				mSocFreqVector.gfx3dcore = 1;
		else {	// sysPll
			mSocFreqVector.gfx3dcore = sysPll / divider;
		}

		// gfx3DSysClk
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DSysClkCtrl), &gfx3DSysClkCtrl.u32);
		D3Switch = gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkD3Switch;
		Switch = gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkSwitch;
		Select = gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkPllSel;
		if (gfx3DSysClkCtrl.ugfx3DSysClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.gfx3dsys = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.gfx3dsys = 1;
		else {	// sysPll
			mSocFreqVector.gfx3dsys = sysPll / divider;
		}

		// arcRefClk
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_arcRefClkCtrl), &arcRefClkCtrl.u32);
		D3Switch = arcRefClkCtrl.uarcRefClkCtrl_ClkD3Switch;
		Switch = arcRefClkCtrl.uarcRefClkCtrl_ClkSwitch;
		Select = arcRefClkCtrl.uarcRefClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = arcRefClkCtrl.uarcRefClkCtrl_ClkPllSel;
		if (arcRefClkCtrl.uarcRefClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.arcref = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.arcref = 1;
		else {	// sysPll
			mSocFreqVector.arcref = sysPll / divider;
		}

/*		// hdmirxFpllRefClk
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxFpllRefClkCtrl), &hdmirxFpllRefClkCtrl.u32);
		D3Switch = hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkD3Switch;
		Switch = hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkSwitch;
		Select = hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkPllSel;
		if (hdmirxFpllRefClkCtrl.uhdmirxFpllRefClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.hdmirxfpllref = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.hdmirxfpllref = 1;
		else{	// sysPll
			mSocFreqVector.hdmirxfpllref= sysPll / divider;
		}

		// hdmirxTclk
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_hdmirxTclkCtrl), &hdmirxTclkCtrl.u32);
		D3Switch = hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkD3Switch;
		Switch = hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkSwitch;
		Select = hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkPllSel;
		if (hdmirxTclkCtrl.uhdmirxTclkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.hdmirxt = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.hdmirxt = 1;
		else {	// sysPll
			mSocFreqVector.hdmirxt = sysPll / divider;
		}
*/
		// vipClk
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_vipClkCtrl), &vipClkCtrl.u32);
		D3Switch = vipClkCtrl.uvipClkCtrl_ClkD3Switch;
		Switch = vipClkCtrl.uvipClkCtrl_ClkSwitch;
		Select = vipClkCtrl.uvipClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = vipClkCtrl.uvipClkCtrl_ClkPllSel;
		if (vipClkCtrl.uvipClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.vip = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.vip = 1;
		else {	// sysPll
			mSocFreqVector.vip = sysPll / divider;
		}

		// sdioXinClk
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdioXinClkCtrl), &sdioXinClkCtrl.u32);
		D3Switch = sdioXinClkCtrl.usdioXinClkCtrl_ClkD3Switch;
		Switch = sdioXinClkCtrl.usdioXinClkCtrl_ClkSwitch;
		Select = sdioXinClkCtrl.usdioXinClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = sdioXinClkCtrl.usdioXinClkCtrl_ClkPllSel;
		if (sdioXinClkCtrl.usdioXinClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.sdioxin = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.sdioxin = 1;
		else {	// sysPll
			mSocFreqVector.sdioxin = sysPll / divider;
		}

		// sdio1XinClk
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_sdio1XinClkCtrl), &sdio1XinClkCtrl.u32);
		D3Switch = sdio1XinClkCtrl.usdio1XinClkCtrl_ClkD3Switch;
		Switch = sdio1XinClkCtrl.usdio1XinClkCtrl_ClkSwitch;
		Select = sdio1XinClkCtrl.usdio1XinClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = sdio1XinClkCtrl.usdio1XinClkCtrl_ClkPllSel;
		if (sdio1XinClkCtrl.usdio1XinClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.sdio1xin = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.sdio1xin = 1;
		else {	// sysPll
			mSocFreqVector.sdio1xin = sysPll / divider;
		}

		// gfx3DExtraClk
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gfx3DExtraClkCtrl), &gfx3DExtraClkCtrl.u32);
		D3Switch = gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkD3Switch;
		Switch = gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkSwitch;
		Select = gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkPllSel;
		if (gfx3DExtraClkCtrl.ugfx3DExtraClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.gfx3dextra = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.gfx3dextra = 1;
		else {	// sysPll
			mSocFreqVector.gfx3dextra = sysPll / divider;
		}

		// gc360Clk
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_gc360ClkCtrl), &gc360ClkCtrl.u32);
		D3Switch = gc360ClkCtrl.ugc360ClkCtrl_ClkD3Switch;
		Switch = gc360ClkCtrl.ugc360ClkCtrl_ClkSwitch;
		Select = gc360ClkCtrl.ugc360ClkCtrl_ClkSel;
		divider = get_divider(D3Switch, Switch, Select);
		PllSel = gc360ClkCtrl.ugc360ClkCtrl_ClkPllSel;
		if (gc360ClkCtrl.ugc360ClkCtrl_ClkPllSwitch)		// AVPLL
			if (PllSel < 4) {
				mSocFreqVector.gc360 = current_freq[1][PllSel + 4] / divider;
			} else
				mSocFreqVector.gc360 = 1;
		else {	// sysPll
			mSocFreqVector.gc360 = sysPll / divider;
		}
		if(gc360ClkCtrl.ugc360ClkCtrl_ClkEN == 0)
			mSocFreqVector.gc360 = 0;
	}
}

void show_speed(void)
{
	int i;
	analyze_speed();

	int vco_freq = diag_vcoFreqs[diag_pll_B_VCO_Setting];
	HW_show_speed((" VCO_B		frequency %d\n", vco_freq));

	for (i = 4; i < 8; i++)
		HW_show_speed((" AVPLLB[%d]	frequency %d\n", i, current_freq[1][i]));

	HW_show_speed((" cpuPll		frequency %d\n", mSocFreqVector.cpupll));
	HW_show_speed((" memPll		frequency %d\n", mSocFreqVector.mempll));
	HW_show_speed((" sysPll		frequency %d\n", mSocFreqVector.syspll));
	HW_show_speed((" dClk		frequency %d\n", mSocFreqVector.ddr));
	HW_show_speed((" cpuClk		frequency %d\n", mSocFreqVector.cpu0));
	HW_show_speed((" sysClk		frequency %d\n", mSocFreqVector.sys));
	HW_show_speed((" drmClk		frequency %d\n", mSocFreqVector.drm));
	HW_show_speed((" cfgClk		frequency %d\n", mSocFreqVector.cfg));
	HW_show_speed((" gfxClk		frequency %d\n", mSocFreqVector.gfx));
	HW_show_speed((" zspClk		frequency %d\n", mSocFreqVector.adsp));
	HW_show_speed((" perifClk	frequency %d\n", mSocFreqVector.perif));
	HW_show_speed((" pCubeClk	frequency %d\n", mSocFreqVector.pcube));
	HW_show_speed((" vScopeClk	frequency %d\n", mSocFreqVector.vscope));
	HW_show_speed((" nfcEccClk	frequency %d\n", mSocFreqVector.nfcecc));
	HW_show_speed((" vppSysClk	frequency %d\n", mSocFreqVector.vpp));
	HW_show_speed((" appClk		frequency %d\n", mSocFreqVector.app));
	HW_show_speed((" gfx3DCoreClk	frequency %d\n", mSocFreqVector.gfx3dcore));
	HW_show_speed((" gfx3DSysClk	frequency %d\n", mSocFreqVector.gfx3dsys));
	HW_show_speed((" arcRefClk	frequency %d\n", mSocFreqVector.arcref));
	HW_show_speed((" vipClk		frequency %d\n", mSocFreqVector.vip));
	HW_show_speed((" sdioXinClk	frequency %d\n", mSocFreqVector.sdioxin));
	HW_show_speed((" sdio1XinClk	frequency %d\n", mSocFreqVector.sdio1xin));
	HW_show_speed((" gfx3DExtraClk	frequency %d\n", mSocFreqVector.gfx3dextra));
	HW_show_speed((" gc360Clk	frequency %d\n", mSocFreqVector.gc360));
}


UNSG32 Dynamic_Lower_vMeta_Speed(void)
{
	    return (0);
}
UNSG32 Dynamic_Recover_vMeta_Speed(void)
{
	    return (0);
}
UNSG32  Dynamic_Lower_ZSP_Speed(void)
{
	    return (0);
}
UNSG32 Dynamic_Recover_ZSP_Speed(void)
{
	    return (0);
}
#endif

int GaloisGetAllFreq(SOC_FREQ_VECTOR * freq){
	memcpy(freq, &mSocFreqVector, sizeof(mSocFreqVector)) ;
	return 0 ;
}

UNSG32 GaloisGetCfgFrequency(void)
{
    return (mSocFreqVector.cfg);
}

UNSG32 GaloisGetFrequency(int FreqID)
{
#if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
	analyze_speed();
#endif

    switch (FreqID) {
        case SOC_FREQ_DDR:
             return (mSocFreqVector.ddr);
        case SOC_FREQ_CPU0:
             return (mSocFreqVector.cpu0);
#if (BERLIN_CHIP_VERSION < BERLIN_BG2)
        case SOC_FREQ_CPU1:
             return (mSocFreqVector.cpu1);
#endif
        case SOC_FREQ_SYSPLL:
             return (mSocFreqVector.syspll);
        case SOC_FREQ_SYS:
             return (mSocFreqVector.sys);
        case SOC_FREQ_VSCOPE:
             return (mSocFreqVector.vscope);
        case SOC_FREQ_PCUBE:
             return (mSocFreqVector.pcube);
        case SOC_FREQ_VPP:
             return (mSocFreqVector.vpp);
        case SOC_FREQ_CFG:
             return (mSocFreqVector.cfg);
        case SOC_FREQ_PERIF:
             return (mSocFreqVector.perif);
        case SOC_FREQ_GFX:
             return (mSocFreqVector.gfx);
        case SOC_FREQ_DRM:
             return (mSocFreqVector.drm);
        case SOC_FREQ_NFCECC:
             return (mSocFreqVector.nfcecc);
	case SOC_FREQ_APP:
             return (mSocFreqVector.app);
#if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
	case SOC_FREQ_GFX3DCORE:
             return (mSocFreqVector.gfx3dcore);
	case SOC_FREQ_GFX3DSYS:
             return (mSocFreqVector.gfx3dsys);
	case SOC_FREQ_ARCREF:
             return (mSocFreqVector.arcref);
	case SOC_FREQ_HDMIRXFPLLREF:
             return (mSocFreqVector.hdmirxfpllref);
	case SOC_FREQ_HDMIRXT:
             return (mSocFreqVector.hdmirxt);
	case SOC_FREQ_VIP:
             return (mSocFreqVector.vip);
	case SOC_FREQ_SDIOXIN:
             return (mSocFreqVector.sdioxin);
	case SOC_FREQ_SDIO1XIN:
             return (mSocFreqVector.sdio1xin);
	case SOC_FREQ_GFX3DEXTRA:
             return (mSocFreqVector.gfx3dextra);
	case SOC_FREQ_GC360:
             return (mSocFreqVector.gc360);
#endif
        default:
             return (0);
    }
}


UNSG32 GaloisGetChipID(void)
{
#if (BERLIN_CHIP_VERSION > BERLIN_B_0)
    T32Gbl_ProductId_ext ChipID_Ext;
    GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ProductId_ext), &ChipID_Ext.u32);
    return (ChipID_Ext.u32 & 0x00ff);
#else
    return 0;
#endif
}

#else
void show_speed(void)
{
}
UNSG32 GaloisGetFrequency(int FreqID)
{
     return (0);
}

UNSG32 GaloisGetChipID()
{
     return (0);
}
#endif
