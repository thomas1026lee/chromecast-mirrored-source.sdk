#include "com_type.h"
#include "memmap.h"
#include "galois_io.h"
#include "global.h"
#include "avpll.h"

#ifdef BERLIN

#if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
#define BFM_HOST_Bus_Write32 	GA_REG_WORD32_WRITE
#define BFM_HOST_Bus_Read32	GA_REG_WORD32_READ

static char dbg_buf[256];

enum PRINT_LEVEL {
	PRN_NONE=0,
	PRN_ERR,
	PRN_RES,
	PRN_INFO,
	PRN_DBG
};
static void dbg_printf(SIGN32 logLevel,const char* szFormat, ...)
{
#if 0
	va_list args;
	va_start(args, szFormat);
	vsnprintf(dbg_buf, 255, szFormat, args);
	printf("%s\n", dbg_buf);
	va_end(args);
#endif
}

typedef struct {
	unsigned int vco_freq_index;
	unsigned int av_freq_index;
	int freq_offset; //signed number
	unsigned int p_sync2;
	unsigned int p_sync1;
	unsigned int divAV1;
	unsigned int divAV2;
	unsigned int divAV3;
} CLK_FREQ;

double current_freq[2][7];

enum {
	HDMI_8BIT_MODE=0,
	HDMI_10BIT_MODE,
	HDMI_12BIT_MODE
};

enum {
	FRAME_RATE_59P94=0,
	FRAME_RATE_60
};

#define	OVERSAMPLE_HDMI			0
#define OVERSAMPLE_1X			1
#define OVERSAMPLE_2X			2
#define OVERSAMPLE_4X			4
#define OVERSAMPLE_8X			8

double diag_hdmiOverSample[] = {
	10,
	12.5,
	15
};

double diag_videoFreqs[] = {
	//video frequencies, pixel freq
	25.17482517,
	27,
	54,
	74.17582418,
	108,
	148.3516484
};

//all the VCO freq reqired for video and audio in MHz
double diag_vcoFreqs[]= {
	1080, 	//8 bit HDMI
	1260,	//10 bit HDMI
	1350,	//10 bit HDMI
	1485,	//8 bit HDMI
	1512, 	//8 bit HDMI	and 12 bit HDMI
	1620,	//12 bit HDMI
	1856.25,//10 bit HDMI
	2227.5,	//12 bit HDMI
};

//from Section 7 table
static unsigned char diag_avpllRegFBDIV[]=
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

//from Section 7 table, bit18 is signe bit
unsigned int diag_avpllRegFREQ_OFFSET_C8[]=
{
		 (19508),	//VCO_FREQ_1_080G,
		 (33554), 	//VCO_FREQ_1_260G,
		 (00000), 	//VCO_FREQ_1_350G,
		 (28435), 	//VCO_FREQ_1_485G,
		 (33554), 	//VCO_FREQ_1_512G,
	((1<<18)|(12905)), 	//VCO_FREQ_1_620G,
		 (14169), 	//VCO_FREQ_1_856G,
		 (4712), 	//VCO_FREQ_2_227G
};

unsigned int diag_avpllRegSPEED[]=
{
	0x0,	//1.08G<F<= 1.21G, for 1.08G
	0x0,	//1.21G<F<=1.4G	   for 1.26G
	0x1,	//1.21G<F<=1.4G	   for 1.35G
	0x2,	//1.4G<F<=1.61G    for 1.48G
	0x2,	//1.4G<F<=1.61G    for 1.51G
	0x3, 	//1.61G<F<= 1.86G  for 1.62G
	0x4, 	//1.61G<F<= 1.86G  for 1.85G
//	0x4,	//1.86G<F<= 2G	   not used
//	0x5,	//2G<F<=2.22G      not used
	0x6,	//F> 2.22G         for 2.22G
};

unsigned int diag_avpllRegINTPI[]=
{
	0x2,	//1G~1.4		for 1.08G
	0x2,	//1G~1.4		for 1.26G
	0x2,	//1G~1.4		for 1.35G
	0x3,	//1.4G~1.8G		for 1.48G
	0x3,	//1.4G~1.8G		for 1.51G
	0x3, 	//1.4G~1.8G		for 1.62G
	0x4, 	//1.8G~2.25G	for 1.85G
	0x4,	//1.8G~2.25G    for 2.22G
};


//vco is determined by video frequence and HDMI mode
static int vcoSelectionTable[3][6] =
{
	{AVPLL_VCO_FREQ_1_512G, AVPLL_VCO_FREQ_1_080G, AVPLL_VCO_FREQ_1_080G, AVPLL_VCO_FREQ_1_485G, AVPLL_VCO_FREQ_1_080G, AVPLL_VCO_FREQ_1_485G},
	{AVPLL_VCO_FREQ_1_260G, AVPLL_VCO_FREQ_1_350G, AVPLL_VCO_FREQ_1_350G, AVPLL_VCO_FREQ_1_856G, AVPLL_VCO_FREQ_1_350G, AVPLL_VCO_FREQ_1_856G},
	{AVPLL_VCO_FREQ_1_512G, AVPLL_VCO_FREQ_1_620G, AVPLL_VCO_FREQ_1_620G, AVPLL_VCO_FREQ_2_227G, AVPLL_VCO_FREQ_1_620G, AVPLL_VCO_FREQ_2_227G},
};

//1KPPM is determined by video frequence and 59Hz/60Hz mode
static int ppm1kSelectionTable[2][6] =
{
	{-1, 0, 0, -1, 0, -1}, //59.94Hz 1KPPM flag
	{ 0, 1, 1,  0, 1,  0}, //60Hz 1KPPM flag
};

// Just for Audio channel configuration. Video channel configuration uses other tables.
static CLK_FREQ clk_freqs[] =
{
	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_90P3168_MHZ,	-56508,	12544, 12375, 6, 0x5},	// 44.1KHz
	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_65P536_MHZ, 	-121161,114688,111375,6, 0x7},	// 32KHz
	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_98P304_MHZ,	-62130,	188416,185625,4, 0x17},	// 48KHz

	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_90P3168_MHZ,	-14712,	3136,	3125, 5, 0x9},
	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_65P536_MHZ,	-47104,	2048,	2025, 6, 0x5},
	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_98P304_MHZ,	125696,	16384,	16875,5, 0x8},

	{AVPLL_VCO_FREQ_1_512G,	AUDIO_FREQ_90P3168_MHZ,	-63883, 1904,	1875, 4, 0x11},
	{AVPLL_VCO_FREQ_1_512G, AUDIO_FREQ_65P536_MHZ,	13000,	23552,	23625,4, 0x17},
	{AVPLL_VCO_FREQ_1_512G, AUDIO_FREQ_98P304_MHZ,	106496, 1536,	1575, 4, 0xF},

	{AVPLL_VCO_FREQ_1_350G,	AUDIO_FREQ_90P3168_MHZ,	-14712,	3136,	3125, 4, 0xF},
	{AVPLL_VCO_FREQ_1_350G, AUDIO_FREQ_65P536_MHZ,	-80018,	28672,	28125,4, 0x15},
	{AVPLL_VCO_FREQ_1_350G, AUDIO_FREQ_98P304_MHZ,	-80018,	28672,	28125,5, 0x7},

	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_90P3168_MHZ,	-89347,	175616,	171875,4,0x15},
	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_65P536_MHZ,	48553,	917504,	928125,5,0xE},
	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_98P304_MHZ,	-25882,	311296,	309375,4,0x13},

	{AVPLL_VCO_FREQ_1_260G,	AUDIO_FREQ_90P3168_MHZ,	-14712,	3136,	3125,  5,0x7},
	{AVPLL_VCO_FREQ_1_260G, AUDIO_FREQ_65P536_MHZ,	49906,	38912,	39375, 4,0x13},
	{AVPLL_VCO_FREQ_1_260G, AUDIO_FREQ_98P304_MHZ,	-58919,	13312,	13125, 4,0xD},

	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_90P3168_MHZ,	-14712,	3136,	3125,  5,0x6},
	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_65P536_MHZ,	125696,	16384,	16875, 5,0x8},
	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_98P304_MHZ, 	-5213,	5632,	5625,  4,0xB},

	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_90P3168_MHZ,	115900,	100352,	103125,5,0x8},
	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_65P536_MHZ, 	-62130,	188416,	185625,4,0x17},
	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_98P304_MHZ, 	29696,	4096,	4125,  4,0xF},

	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_22P5792_MHZ, -56508, 12544, 12375, 6, 0x14},	// 44.1KHz
	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_16P384_MHZ,  29696,  4096,  4125,  6, 0x1B},	// 32KHz
	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_24P576_MHZ,  29696,  4096,  4125,  6, 0x12},  // 48KHz

	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_22P5792_MHZ, 104704, 5488,  5625,  6, 0xE},
	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_16P384_MHZ,  -47104, 2048,  2025,  6, 0x14},
	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_24P576_MHZ,  59234,  3328,  3375,  6, 0xD},

	{AVPLL_VCO_FREQ_1_512G, AUDIO_FREQ_22P5792_MHZ, 126750, 1456,  1500,  6, 0xD},
	{AVPLL_VCO_FREQ_1_512G, AUDIO_FREQ_16P384_MHZ,  106496, 1536,  1575,  6, 0x12},
	{AVPLL_VCO_FREQ_1_512G, AUDIO_FREQ_24P576_MHZ,  106496, 1536,  1575,  6, 0xC},

	{AVPLL_VCO_FREQ_1_350G, AUDIO_FREQ_22P5792_MHZ, -14712, 3136,  3125,  6, 0xC},
	{AVPLL_VCO_FREQ_1_350G, AUDIO_FREQ_16P384_MHZ,  125696, 16384, 16875, 6, 0x10},
	{AVPLL_VCO_FREQ_1_350G, AUDIO_FREQ_24P576_MHZ,  -5213,  5632,  5625,  6, 0xB},

	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_22P5792_MHZ, 115900, 100352,103125,6, 0x10},
	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_16P384_MHZ,  125696, 16384, 16875, 4, 0x6E},
	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_24P576_MHZ,  29696,  4096,  4125,  6, 0xF},

	{AVPLL_VCO_FREQ_1_260G, AUDIO_FREQ_22P5792_MHZ, 61280,  1232,	 1250,  6, 0xB},
	{AVPLL_VCO_FREQ_1_260G, AUDIO_FREQ_16P384_MHZ,  106496, 1024,	 1050,	6, 0xF},
	{AVPLL_VCO_FREQ_1_260G, AUDIO_FREQ_24P576_MHZ,  106496, 1024,	 1050,	6, 0xA},

	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_22P5792_MHZ, -181895,1176,	 1125,  6, 0xA},
	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_16P384_MHZ,  59234,  3328,	 3375,	6, 0xD},
	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_24P576_MHZ,  -98304, 1152,	 1125,	6, 0x9},

	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_22P5792_MHZ, 49589,  20384, 20625,	6, 0xD},
	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_16P384_MHZ,  29696,  4096,	 4125,	6, 0x12},
	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_24P576_MHZ,  29696,  4096,	 4125,  6, 0xC},

	// new entries for APLL deviation
	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_22P636_MHZ,  -66890,  22636,  22275,  6, 0x14},	//
	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_16P424_MHZ,  19408,  4106,  4125,  6, 0x1B},
	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_24P636_MHZ,  19408,  4106,  4125,  6, 0x12},

	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_22P636_MHZ,  93917,  39613,  40500,  6, 0xE}, //
	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_16P424_MHZ,  -57204,  2053,  2025,  6, 0x14},
	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_24P636_MHZ,  48875,  26689,  27000,  6, 0xD},

	{AVPLL_VCO_FREQ_1_512G, AUDIO_FREQ_22P636_MHZ,  115908, 73567,  75600,  6, 0xD},
	{AVPLL_VCO_FREQ_1_512G, AUDIO_FREQ_16P424_MHZ,  96021, 2053,  2100,  6, 0x12},
	{AVPLL_VCO_FREQ_1_512G, AUDIO_FREQ_24P636_MHZ,  96021, 2053,  2100,  6, 0xC},

	{AVPLL_VCO_FREQ_1_350G, AUDIO_FREQ_22P636_MHZ,  -25199,  5659,  5625,  6, 0xC},
	{AVPLL_VCO_FREQ_1_350G, AUDIO_FREQ_16P424_MHZ,  115174,  16424,  16875,  6, 0x10},
	{AVPLL_VCO_FREQ_1_350G, AUDIO_FREQ_24P636_MHZ,  -15415,  22583,  22500,  6, 0xB},

	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_22P636_MHZ,  105084, 181088,  185625,  6, 0x10},
	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_16P424_MHZ,  -72193, 188876,  185625,  6, 0x17},
	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_24P636_MHZ,  19408,  4106,  4125,  6, 0xF},

	{AVPLL_VCO_FREQ_1_260G, AUDIO_FREQ_22P636_MHZ,  50601, 62249,	 63000,	6, 0xB},
	{AVPLL_VCO_FREQ_1_260G, AUDIO_FREQ_16P424_MHZ,  96021, 2053,	 2100,	6, 0xF},
	{AVPLL_VCO_FREQ_1_260G, AUDIO_FREQ_24P636_MHZ,  96021, 2053,	 2100,	6, 0xA},

	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_22P636_MHZ,  -191964, 5659,	 5400,	6, 0xA},
	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_16P424_MHZ,  48875, 26689,	 27000,	6, 0xD},
	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_24P636_MHZ,  -108279, 2053,	 2000,	6, 0x9},

	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_22P636_MHZ,  38940,  73567,	 74250,  6, 0xD},
	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_16P424_MHZ,  19408,  4106,	 4125,  6, 0x12},
	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_24P636_MHZ,  19408,  4106,	 4125,  6, 0xC},

	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_90P544_MHZ,  -66890,  22636, 22275, 6, 0x5},
	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_65P696_MHZ,  -131081, 114968 , 111375, 6, 0x7},
	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_98P544_MHZ,  115174, 16424, 16875, 5, 0xB},

	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_90P544_MHZ,  -25199,  5659, 5625, 5, 0x9},
	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_65P696_MHZ,  -57204,  2053, 2025, 6, 0x5},
	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_98P544_MHZ,  115174, 16424, 16875, 5, 0x8},

	{AVPLL_VCO_FREQ_1_512G, AUDIO_FREQ_90P544_MHZ,  -74248,  96203, 94500, 4, 0x11},
	{AVPLL_VCO_FREQ_1_512G, AUDIO_FREQ_65P696_MHZ,  2753,  47219, 47250, 4, 0x17},
	{AVPLL_VCO_FREQ_1_512G, AUDIO_FREQ_98P544_MHZ,  -172123, 8212, 7875, 5, 0x8},

	{AVPLL_VCO_FREQ_1_350G, AUDIO_FREQ_90P544_MHZ,  -25199,  5659, 5625, 4, 0xF},
	{AVPLL_VCO_FREQ_1_350G, AUDIO_FREQ_65P696_MHZ,  -90038,  28742, 28125, 4, 0x15},
	{AVPLL_VCO_FREQ_1_350G, AUDIO_FREQ_98P544_MHZ,  -90038,  28742, 28125, 5, 0x7},

	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_90P544_MHZ,  105084,  181088, 185625, 6, 0x4},
	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_65P696_MHZ,  -243948,  65696, 61875, 6, 0x6},
	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_98P544_MHZ,  194979,  32848, 34375, 5, 0x9},

	{AVPLL_VCO_FREQ_1_260G, AUDIO_FREQ_90P544_MHZ,  -25199,  5659, 5625, 5, 0x7},
	{AVPLL_VCO_FREQ_1_260G, AUDIO_FREQ_65P696_MHZ,  -172123,  8212,	7875, 6, 0x4},
	{AVPLL_VCO_FREQ_1_260G, AUDIO_FREQ_98P544_MHZ,  -68990,  26689, 26250, 4, 0xD},

	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_90P544_MHZ,  -25199,  5659, 5625, 5, 0x6},
	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_65P696_MHZ,  115174,  16424,	 16875,  5, 0x8},
	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_98P544_MHZ,  -15415,  22583, 22500,  4, 0xB},

	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_90P544_MHZ,  105084,  181088, 185625, 5, 0x8},
	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_65P696_MHZ,  115174,  16424,	 16875,  5, 0xB},
	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_98P544_MHZ,  -243948,  65686, 61875,  5, 0x8},

	//
	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_22P522_MHZ,  -45999,  22522,  22275,  6, 0x14},	//
	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_16P342_MHZ,  40551,  8171,  8250,  6, 0x1B},
	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_24P514_MHZ,  40375,  12257,  12375,  6, 0x12},

	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_22P522_MHZ,  115623,  78827,  81000,  6, 0xE}, //
	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_16P342_MHZ,  -36445,  8171,  8100,  6, 0x14},
	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_24P514_MHZ,  5381,  134827,  135000,  5, 0x21},

	{AVPLL_VCO_FREQ_1_512G, AUDIO_FREQ_22P522_MHZ,  137725, 146393,  151200,  6, 0xD},
	{AVPLL_VCO_FREQ_1_512G, AUDIO_FREQ_16P342_MHZ,  -109390, 155249,  151200,  6, 0x13},
	{AVPLL_VCO_FREQ_1_512G, AUDIO_FREQ_24P514_MHZ,  -21712, 54281,  54000,  5, 0x1F},

	{AVPLL_VCO_FREQ_1_350G, AUDIO_FREQ_22P522_MHZ,  -4097,  11261,  11250,  6, 0xC},
	{AVPLL_VCO_FREQ_1_350G, AUDIO_FREQ_16P342_MHZ,  -117972,  138907,  135000,  6, 0x11},
	{AVPLL_VCO_FREQ_1_350G, AUDIO_FREQ_24P514_MHZ,  5381,  134827,  135000,  6, 0xB},

	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_22P522_MHZ,  126846, 180176,  185625,  6, 0x10},
	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_16P342_MHZ,  -51510, 187933,  185625,  6, 0x17},
	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_24P514_MHZ,  40379,  12257,  12375,  6, 0xF},

	{AVPLL_VCO_FREQ_1_260G, AUDIO_FREQ_22P522_MHZ,  72088, 123871,	 126000,6, 0xB},
	{AVPLL_VCO_FREQ_1_260G, AUDIO_FREQ_16P342_MHZ,  117549, 8171,	 8400,	6, 0xF},
	{AVPLL_VCO_FREQ_1_260G, AUDIO_FREQ_24P514_MHZ,  117373, 1751,	 1800,	6, 0xA},

	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_22P522_MHZ,  -171705, 11261,	 10800,	6, 0xA},
	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_16P342_MHZ,  70166,   106223, 108000,6, 0xD},
	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_24P514_MHZ,  -87944, 12257,	 12000,	6, 0x9},

	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_22P522_MHZ,  60367,  146393,	 148500,  6, 0xD},
	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_16P342_MHZ,  40551,  8171,	 8250,  6, 0x12},
	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_24P514_MHZ,  40379,  12257,	 12375,  6, 0xC},

	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_90P088_MHZ,  -45999,  22522, 22275, 6, 0x5},
	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_65P368_MHZ,  -110692, 114394 , 111375, 6, 0x7},
	{AVPLL_VCO_FREQ_2_227G, AUDIO_FREQ_98P056_MHZ,  136622, 49028, 50625, 5, 0xB},

	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_90P088_MHZ,  -4097,  11261, 11250, 5, 0x9},
	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_65P368_MHZ,  -36445,  8171, 8100, 6, 0x5},
	{AVPLL_VCO_FREQ_1_620G, AUDIO_FREQ_98P056_MHZ,  136622, 49028, 50625, 5, 0x8},

	{AVPLL_VCO_FREQ_1_512G, AUDIO_FREQ_90P088_MHZ,  -53393,  191437, 189000, 4, 0x11},
	{AVPLL_VCO_FREQ_1_512G, AUDIO_FREQ_65P368_MHZ,  23813,  187933, 189000, 4, 0x17},
	{AVPLL_VCO_FREQ_1_512G, AUDIO_FREQ_98P056_MHZ,  -152106, 3502, 3375, 5, 0x8},

	{AVPLL_VCO_FREQ_1_350G, AUDIO_FREQ_90P088_MHZ,  -4097,  11261, 11250, 4, 0xF},
	{AVPLL_VCO_FREQ_1_350G, AUDIO_FREQ_65P368_MHZ,  -69444,  57197, 56250, 4, 0x15},
	{AVPLL_VCO_FREQ_1_350G, AUDIO_FREQ_98P056_MHZ,  -69612,  85799, 84375, 5, 0x7},

	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_90P088_MHZ,  126846,  180176, 185625, 6, 0x4},
	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_65P368_MHZ,  -224126,  65368, 61875, 6, 0x6},
	{AVPLL_VCO_FREQ_1_856G, AUDIO_FREQ_98P056_MHZ,  216824,  98056, 103125, 5, 0x9},

	{AVPLL_VCO_FREQ_1_260G, AUDIO_FREQ_90P088_MHZ,  -4097,  11261, 11250, 5, 0x7},
	{AVPLL_VCO_FREQ_1_260G, AUDIO_FREQ_65P368_MHZ,  -151941,  8171,	7875, 6, 0x4},
	{AVPLL_VCO_FREQ_1_260G, AUDIO_FREQ_98P056_MHZ,  -48460,  22763, 22500, 4, 0xD},

	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_90P088_MHZ,  -4097,  11261, 11250, 5, 0x6},
	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_65P368_MHZ,  136798,  16342,	 16875,  5, 0x8},
	{AVPLL_VCO_FREQ_1_080G, AUDIO_FREQ_98P056_MHZ,  5381,  134827, 135000,  4, 0xB},

	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_90P088_MHZ,  126846,  180176, 185625, 5, 0x8},
	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_65P368_MHZ,  136798,  16342,	 16875,  5, 0xB},
	{AVPLL_VCO_FREQ_1_485G, AUDIO_FREQ_98P056_MHZ,  -224288,  196112, 185625,  5, 0x8},
};

int diag_pll_A_VCO_Setting = AVPLL_VCO_FREQ_2_227G;
int diag_pll_B_VCO_Setting = AVPLL_VCO_FREQ_1_620G;

unsigned int gAVPLLA_Channel_OutputClock[8] = {0};
unsigned int gAVPLLB_Channel_OutputClock[8] = {0};

void diag_setDPll_A(int enable, int p_sync1, int p_sync2, int chId)
{
	T32Gbl_avPllCtl30 regData30;

	double ppm_from_dpll;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl30), &regData30.u32);


	//disable DPll first
	regData30.uavPllCtl_avPllEnDpllC_A &= ~(1<<(chId-1));
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl30), regData30.u32);

	if(!p_sync1)
	{
		if(enable) dbg_printf(PRN_RES, "Warning p_sync1 is 0\n");
	}
	else
	{
		ppm_from_dpll = (double)p_sync2/p_sync1;
		dbg_printf(PRN_RES, "  DPLL PPM is %f\n", ppm_from_dpll);
	}

	switch(chId)
	{
		//set values
		case 1:
		{
			T32Gbl_avPllCtl15 regData15;
			T32Gbl_avPllCtl23 regData23;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl15), &regData15.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl23), &regData23.u32);
			regData15.uavPllCtl_avPllPSync1C1_A = p_sync1;
			regData23.uavPllCtl_avPllPSync2C1_A = p_sync2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl15), regData15.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl23), regData23.u32);
		}
		break;
		case 2:
		{
			T32Gbl_avPllCtl16 regData16;
			T32Gbl_avPllCtl24 regData24;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl16), &regData16.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl24), &regData24.u32);
			regData16.uavPllCtl_avPllPSync1C2_A = p_sync1;
			regData24.uavPllCtl_avPllPSync2C2_A = p_sync2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl16), regData16.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl24), regData24.u32);
		}
		break;
		case 3:
		{
			T32Gbl_avPllCtl17 regData17;
			T32Gbl_avPllCtl25 regData25;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), &regData17.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl25), &regData25.u32);
			regData17.uavPllCtl_avPllPSync1C3_A = p_sync1;
			regData25.uavPllCtl_avPllPSync2C3_A = p_sync2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), regData17.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl25), regData25.u32);
		}
		break;
		case 4:
		{
			T32Gbl_avPllCtl18 regData18;
			T32Gbl_avPllCtl26 regData26;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl18), &regData18.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl26), &regData26.u32);
			regData18.uavPllCtl_avPllPSync1C4_A = p_sync1;
			regData26.uavPllCtl_avPllPSync2C4_A = p_sync2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl18), regData18.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl26), regData26.u32);
		}
		break;
		case 5:
		{
			T32Gbl_avPllCtl19 regData19;
			T32Gbl_avPllCtl27 regData27;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl19), &regData19.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl27), &regData27.u32);
			regData19.uavPllCtl_avPllPSync1C5_A = p_sync1;
			regData27.uavPllCtl_avPllPSync2C5_A = p_sync2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl19), regData19.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl27), regData27.u32);
		}
		break;
		case 6:
		{
			T32Gbl_avPllCtl20 regData20;
			T32Gbl_avPllCtl28 regData28;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl20), &regData20.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl28), &regData28.u32);
			regData20.uavPllCtl_avPllPSync1C6_A = p_sync1;
			regData28.uavPllCtl_avPllPSync2C6_A = p_sync2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl20), regData20.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl28), regData28.u32);
		}
		break;
		case 7:
		{
			T32Gbl_avPllCtl21 regData21;
			T32Gbl_avPllCtl29 regData29;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl21), &regData21.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl29), &regData29.u32);
			regData21.uavPllCtl_avPllPSync1C7_A = p_sync1;
			regData29.uavPllCtl_avPllPSync2C7_A = p_sync2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl21), regData21.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl29), regData29.u32);
		}
		break;
		case 8:
		{
			T32Gbl_avPllCtl22 regData22;
			T32Gbl_avPllCtl30 regData30;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl22), &regData22.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl30), &regData30.u32);
			regData22.uavPllCtl_avPllPSync1C8_A = p_sync1;
			regData30.uavPllCtl_avPllPSync2C8_A = p_sync2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl22), regData22.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl30), regData30.u32);
		}
		break;
		default:
			dbg_printf(PRN_RES, "Invalid channel\n");
		break;
	}

	if(enable)
	{
		//enable DPLL
		regData30.uavPllCtl_avPllEnDpllC_A |= (1<<(chId-1));

		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl30), regData30.u32);
	}

}

void diag_setDPll_B(int enable, int p_sync1, int p_sync2, int chId)
{
	T32Gbl_avPllCtl61 regData61;
	double ppm_from_dpll;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl61), &regData61.u32);

	//disable DPLL first
	regData61.uavPllCtl_avPllEnDpllC_B &= ~(1<<(chId-1));
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl61), regData61.u32);

	if(!p_sync1)
	{
		if(enable) dbg_printf(PRN_RES, "Warning p_sync1 is 0\n");
	}
	else
	{
		ppm_from_dpll = (double)p_sync2/p_sync1;
		dbg_printf(PRN_RES, "  DPLL PPM is %f\n", ppm_from_dpll);
	}

	switch(chId)
	{
		//set values
		case 1:
		{
			T32Gbl_avPllCtl46 regData46;
			T32Gbl_avPllCtl54 regData54;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl46), &regData46.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl54), &regData54.u32);
			regData46.uavPllCtl_avPllPSync1C1_B = p_sync1;
			regData54.uavPllCtl_avPllPSync2C1_B = p_sync2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl46), regData46.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl54), regData54.u32);
		}
		break;
		case 2:
		{
			T32Gbl_avPllCtl47 regData47;
			T32Gbl_avPllCtl55 regData55;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl47), &regData47.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl55), &regData55.u32);
			regData47.uavPllCtl_avPllPSync1C2_B = p_sync1;
			regData55.uavPllCtl_avPllPSync2C2_B = p_sync2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl47), regData47.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl55), regData55.u32);
		}
		break;
		case 3:
		{
			T32Gbl_avPllCtl48 regData48;
			T32Gbl_avPllCtl56 regData56;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl48), &regData48.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl56), &regData56.u32);
			regData48.uavPllCtl_avPllPSync1C3_B = p_sync1;
			regData56.uavPllCtl_avPllPSync2C3_B = p_sync2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl48), regData48.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl56), regData56.u32);
		}
		break;
		case 4:
		{
			T32Gbl_avPllCtl49 regData49;
			T32Gbl_avPllCtl57 regData57;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl49), &regData49.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl57), &regData57.u32);
			regData49.uavPllCtl_avPllPSync1C4_B = p_sync1;
			regData57.uavPllCtl_avPllPSync2C4_B = p_sync2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl49), regData49.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl57), regData57.u32);
		}
		break;
		case 5:
		{
			T32Gbl_avPllCtl50 regData50;
			T32Gbl_avPllCtl58 regData58;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl50), &regData50.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl58), &regData58.u32);
			regData50.uavPllCtl_avPllPSync1C5_B = p_sync1;
			regData58.uavPllCtl_avPllPSync2C5_B = p_sync2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl50), regData50.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl58), regData58.u32);
		}
		break;
		case 6:
		{
			T32Gbl_avPllCtl51 regData51;
			T32Gbl_avPllCtl59 regData59;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl51), &regData51.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl59), &regData59.u32);
			regData51.uavPllCtl_avPllPSync1C6_B = p_sync1;
			regData59.uavPllCtl_avPllPSync2C6_B = p_sync2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl51), regData51.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl59), regData59.u32);
		}
		break;
		case 7:
		{
			T32Gbl_avPllCtl52 regData52;
			T32Gbl_avPllCtl60 regData60;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl52), &regData52.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl60), &regData60.u32);
			regData52.uavPllCtl_avPllPSync1C7_B = p_sync1;
			regData60.uavPllCtl_avPllPSync2C7_B = p_sync2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl52), regData52.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl60), regData60.u32);
		}
		break;
		case 8:
		{
			T32Gbl_avPllCtl53 regData53;
			T32Gbl_avPllCtl61 regData61;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl53), &regData53.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl61), &regData61.u32);
			regData53.uavPllCtl_avPllPSync1C8_B = p_sync1;
			regData61.uavPllCtl_avPllPSync2C8_B = p_sync2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl53), regData53.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl61), regData61.u32);
		}
		break;
		default:
			dbg_printf(PRN_RES, "Invalid channel\n");
		break;
	}

	if(enable)
	{
		//re-enable DPLL
		regData61.uavPllCtl_avPllEnDpllC_B |= (1<<(chId-1));
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl61), regData61.u32);
	}

}

//Yongchun Move from lower here to handle the resolution change could change the VCO.
static int avpll_inited[2] = {0, 0};
static double org_ppm[2] = {0.0, 0.0};
static double cur_ppm[2] = {0.0, 0.0};

static double offset_2_ppm(int offset) {
        int v = offset & 0x3ffff;

        if(offset & 0x40000) v = -v;

        return -((1000000.0 * v)/(4194304.0 + v));
}

static int ppm_2_offset(double ppm) {
        double c = -4194304.0 * ppm / (1000000.0 + ppm);

        int offset = (int)c;

        if(offset > 524287) {
                offset = 524287;
        }else if(offset < -524287) {
                offset = -524287;
        }

        return (offset < 0) ? ((-offset) | 0x40000) : offset;
}

void diag_setVCO_A(int vco_freq_index)
{
	T32Gbl_avPllCtl reg;
	T32Gbl_avPllCtl1 regData1;
	T32Gbl_avPllCtl9 regData9;

	dbg_printf(PRN_INFO, "PLLA %d VCO freq change to %f\n", diag_vcoFreqs[vco_freq_index]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), &reg.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl1), &regData1.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), &regData9.u32);

	//following three settings are done under reset to improve long term reliability
	diag_assertReset_A();
	regData1.uavPllCtl_avPllFbDiv_A = diag_avpllRegFBDIV[vco_freq_index];
	reg.uavPllCtl_avPllSpeed_A = diag_avpllRegSPEED[vco_freq_index];
	reg.uavPllCtl_avPllIntpi_A = diag_avpllRegINTPI[vco_freq_index];
	diag_deassertReset_A();

	regData9.uavPllCtl_avPllFreqOffsetC8_A = diag_avpllRegFREQ_OFFSET_C8[vco_freq_index];

	diag_pll_A_VCO_Setting = vco_freq_index;

    cur_ppm[0] = org_ppm[0] = offset_2_ppm(regData9.uavPllCtl_avPllFreqOffsetC8_A);
    avpll_inited[0] = 1;

	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), reg.u32);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl1), regData1.u32);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), regData9.u32);

	//toggle the offset_rdy bit
	{
		volatile int i;

		T32Gbl_avPllCtl9 regData;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), &regData.u32);

		regData.uavPllCtl_avPllFreqOffsetReadyC_A = (1<<7);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), regData.u32);

		//add some delay because need to be asserted by 30ns
		for(i=0; i<10000; i++);

		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), &regData.u32);

		regData.uavPllCtl_avPllFreqOffsetReadyC_A = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), regData.u32);
	}
}

void diag_setVCO_B(int vco_freq_index)
{
	T32Gbl_avPllCtl31 regData31;
	T32Gbl_avPllCtl32 regData32;
	T32Gbl_avPllCtl40 regData40;

	dbg_printf(PRN_INFO, "PLLB %d VCO freq change to %f\n", diag_vcoFreqs[vco_freq_index]);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl31), &regData31.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl32), &regData32.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl40), &regData40.u32);

	//following three settings are done under reset to improve long term reliability
	diag_assertReset_B();
	regData32.uavPllCtl_avPllFbDiv_B = diag_avpllRegFBDIV[vco_freq_index];
	regData31.uavPllCtl_avPllSpeed_B = diag_avpllRegSPEED[vco_freq_index];
	regData31.uavPllCtl_avPllIntpi_B = diag_avpllRegINTPI[vco_freq_index];
	diag_deassertReset_B();

	regData40.uavPllCtl_avPllFreqOffsetC8_B = diag_avpllRegFREQ_OFFSET_C8[vco_freq_index];

	diag_pll_B_VCO_Setting = vco_freq_index;

    cur_ppm[1] = org_ppm[1] = offset_2_ppm(regData40.uavPllCtl_avPllFreqOffsetC8_B);
    avpll_inited[1] = 1;

	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl40), regData40.u32);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl32), regData32.u32);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl31), regData31.u32);

	//toggle the offset_rdy bit
	{
		volatile int i;

		T32Gbl_avPllCtl40 regData;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl40), &regData.u32);

		regData.uavPllCtl_avPllFreqOffsetReadyC_B = (1<<7);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl40), regData.u32);

		//add some delay because need to be asserted by 30ns
		for(i=0; i<10000; i++);

		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl40), &regData.u32);

		regData.uavPllCtl_avPllFreqOffsetReadyC_B = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl40), regData.u32);
	}
}

void diag_setChanOffset_A(int offset, int chId)
{
	unsigned int reg_offset;
	double ppm_from_offset;

	if(offset<0)
	{
		reg_offset = (1<<18) | (-offset) ;
	}
	else
	{
		reg_offset = offset;
	}

	dbg_printf(PRN_INFO, "set A%d offset to 0x%x\n", chId, reg_offset);

	if(offset != 0)
	{
		ppm_from_offset = 1/((double)offset/4194304+1);
		dbg_printf(PRN_RES, "offset PPM is %f\n", ppm_from_offset);
	}

	//set offset register
	switch(chId)
	{
		case 1:
		{
			T32Gbl_avPllCtl2 regData;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl2), &regData.u32);
			regData.uavPllCtl_avPllFreqOffsetC1_A = reg_offset;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl2), regData.u32);
		}
		break;
		case 2:
		{
			T32Gbl_avPllCtl3 regData;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl3), &regData.u32);
			regData.uavPllCtl_avPllFreqOffsetC2_A = reg_offset;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl3), regData.u32);
		}
		break;
		case 3:
		{
			T32Gbl_avPllCtl4 regData;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl4), &regData.u32);
			regData.uavPllCtl_avPllFreqOffsetC3_A = reg_offset;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl4), regData.u32);
		}
		break;
		case 4:
		{
			T32Gbl_avPllCtl5 regData;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl5), &regData.u32);
			regData.uavPllCtl_avPllFreqOffsetC4_A = reg_offset;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl5), regData.u32);
		}
		break;
		case 5:
		{
			T32Gbl_avPllCtl6 regData;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl6), &regData.u32);
			regData.uavPllCtl_avPllFreqOffsetC5_A = reg_offset;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl6), regData.u32);
		}
		break;
		case 6:
		{
			T32Gbl_avPllCtl7 regData;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl7), &regData.u32);
			regData.uavPllCtl_avPllFreqOffsetC6_A = reg_offset;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl7), regData.u32);
		}
		break;
		case 7:
		{
			T32Gbl_avPllCtl8 regData;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl8), &regData.u32);
			regData.uavPllCtl_avPllFreqOffsetC7_A = reg_offset;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl8), regData.u32);
		}
		break;
		default:
			dbg_printf(PRN_RES, "Invlid Channel ID (1 to 7 only)\n");
		break;
	}

	//toggle the offset_rdy bit
	{
		volatile int i;

		T32Gbl_avPllCtl9 regData;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), &regData.u32);

		regData.uavPllCtl_avPllFreqOffsetReadyC_A = (1<<(chId-1));
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), regData.u32);

		//add some delay because need to be asserted by 30ns
		for(i=0; i<10000; i++);

		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), &regData.u32);

		regData.uavPllCtl_avPllFreqOffsetReadyC_A = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), regData.u32);
	}

}

void diag_setChanOffset_B(int offset, int chId)
{
	unsigned int reg_offset;
	double ppm_from_offset;

	if(offset<0)
	{
		reg_offset = (1<<18) | (-offset) ;
	}
	else
	{
		reg_offset = offset;
	}

	dbg_printf(PRN_INFO, "set A%d offset to 0x%x\n", chId, reg_offset);

	if(offset != 0)
	{
		ppm_from_offset = 1/((double)offset/4194304+1);
		dbg_printf(PRN_RES, "offset PPM is %f\n", ppm_from_offset);
	}

	//set offset register
	switch(chId)
	{
		case 1:
		{
			T32Gbl_avPllCtl33 regData;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl33), &regData.u32);
			regData.uavPllCtl_avPllFreqOffsetC1_B = reg_offset;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl33), regData.u32);
		}
		break;
		case 2:
		{
			T32Gbl_avPllCtl34 regData;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl34), &regData.u32);
			regData.uavPllCtl_avPllFreqOffsetC2_B = reg_offset;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl34), regData.u32);
		}
		break;
		case 3:
		{
			T32Gbl_avPllCtl35 regData;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl35), &regData.u32);
			regData.uavPllCtl_avPllFreqOffsetC3_B = reg_offset;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl35), regData.u32);
		}
		break;
		case 4:
		{
			T32Gbl_avPllCtl36 regData;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl36), &regData.u32);
			regData.uavPllCtl_avPllFreqOffsetC4_B = reg_offset;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl36), regData.u32);
		}
		break;
		case 5:
		{
			T32Gbl_avPllCtl37 regData;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl37), &regData.u32);
			regData.uavPllCtl_avPllFreqOffsetC5_B = reg_offset;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl37), regData.u32);
		}
		break;
		case 6:
		{
			T32Gbl_avPllCtl38 regData;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl38), &regData.u32);
			regData.uavPllCtl_avPllFreqOffsetC6_B = reg_offset;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl38), regData.u32);
		}
		break;
		case 7:
		{
			T32Gbl_avPllCtl39 regData;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl39), &regData.u32);
			regData.uavPllCtl_avPllFreqOffsetC7_B = reg_offset;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl39), regData.u32);
		}
		break;
		default:
			dbg_printf(PRN_RES, "Invlid Channel ID (1 to 7 only)\n");
		break;
	}

	//toggle the offset_rdy bit
	{
		volatile int i;

		T32Gbl_avPllCtl40 regData;
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl40), &regData.u32);

		regData.uavPllCtl_avPllFreqOffsetReadyC_B = (1<<(chId-1));
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl40), regData.u32);

		//add some delay because need to be asserted by 30ns
		for(i=0; i<10000; i++);

		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl40), &regData.u32);

		regData.uavPllCtl_avPllFreqOffsetReadyC_B = 0;
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl40), regData.u32);
	}

}

void diag_setHDMI_Div_A(int divHDMI, int chId)
{
	unsigned int div_reg;
	T32Gbl_avPllCtl11 regData;

	switch(divHDMI)
	{
		case 0:
			div_reg = 0; //disable DIV_HDMI
			break;
		case 1:
			div_reg = 4; //div by 1, by pass
			break;
		case 2:
			div_reg = 5; //div by 2
			break;
		case 4:
			div_reg = 6; //div by 4
			break;
		case 6:
			div_reg = 7; //div by 6
			break;
		default:
			{
				dbg_printf(PRN_RES, "Invalid divider for HDMI\n");
				return ;
			}
	}

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), &regData.u32);

	switch(chId)
	{
		case 1:
			regData.uavPllCtl_avPllDivHdmiC1_A = div_reg;
		break;
		case 2:
			regData.uavPllCtl_avPllDivHdmiC2_A = div_reg;
		break;
		case 3:
			regData.uavPllCtl_avPllDivHdmiC3_A = div_reg;
		break;
		case 4:
			regData.uavPllCtl_avPllDivHdmiC4_A = div_reg;
		break;
		case 5:
			regData.uavPllCtl_avPllDivHdmiC5_A = div_reg;
		break;
		case 6:
			regData.uavPllCtl_avPllDivHdmiC6_A = div_reg;
		break;
		case 7:
			regData.uavPllCtl_avPllDivHdmiC7_A = div_reg;
		break;
		default:
			dbg_printf(PRN_RES, "Invalid Channel Id\n");
		break;
	}

	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), regData.u32);
}

void diag_setHDMI_Div_B(int divHDMI, int chId)
{
	unsigned int div_reg;
	T32Gbl_avPllCtl42 regData;

	switch(divHDMI)
	{
		case 0:
			div_reg = 0; //disable DIV_HDMI
			break;
		case 1:
			div_reg = 4; //div by 1, by pass
			break;
		case 2:
			div_reg = 5; //div by 2
			break;
		case 4:
			div_reg = 6; //div by 4
			break;
		case 6:
			div_reg = 7; //div by 6
			break;
		default:
			{
				dbg_printf(PRN_RES, "Invalid divider for HDMI\n");
				return ;
			}
	}

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl42), &regData.u32);

	switch(chId)
	{
		case 1:
			regData.uavPllCtl_avPllDivHdmiC1_B = div_reg;
		break;
		case 2:
			regData.uavPllCtl_avPllDivHdmiC2_B = div_reg;
		break;
		case 3:
			regData.uavPllCtl_avPllDivHdmiC3_B = div_reg;
		break;
		case 4:
			regData.uavPllCtl_avPllDivHdmiC4_B = div_reg;
		break;
		case 5:
			regData.uavPllCtl_avPllDivHdmiC5_B = div_reg;
		break;
		case 6:
			regData.uavPllCtl_avPllDivHdmiC6_B = div_reg;
		break;
		case 7:
			regData.uavPllCtl_avPllDivHdmiC7_B = div_reg;
		break;
		default:
			dbg_printf(PRN_RES, "Invalid Channel Id\n");
		break;
	}

	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl42), regData.u32);
}

//divAV1 is the register value, not the actual divider
//mapping is (binary):
//000 = bypass, the whole divAV channel will be bypassed
//100 = div by 1
//101 = div by 2
//110 = div by 5
//111 = div by 5
void diag_setAV_Div_A(int divAV1, int divAV2, int chId)
{
	if(divAV2 !=0 && (divAV2<4 || divAV2>127))
	{
		dbg_printf(PRN_RES, "Invalid divider for AV1\n");
			return ;
	}


	//program DIV_AV1 and DIV_AV2
	switch(chId)
	{
		case 1:
		{
			T32Gbl_avPllCtl11 regData11;
			T32Gbl_avPllCtl12 regData12;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), &regData11.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), &regData12.u32);
			regData11.uavPllCtl_avPllDivAv1C1_A = divAV1;
			regData12.uavPllCtl_avPllDivAv2C1_A = divAV2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), regData11.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), regData12.u32);
		}
		break;
		case 2:
		{
			T32Gbl_avPllCtl12 regData12;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), &regData12.u32);
			regData12.uavPllCtl_avPllDivAv1C2_A = divAV1;
			regData12.uavPllCtl_avPllDivAv2C2_A = divAV2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), regData12.u32);
		}
		break;
		case 3:
		{
			T32Gbl_avPllCtl12 regData12;
			T32Gbl_avPllCtl13 regData13;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), &regData12.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl13), &regData13.u32);
			regData12.uavPllCtl_avPllDivAv1C3_A = divAV1;
			regData13.uavPllCtl_avPllDivAv2C3_A = divAV2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), regData12.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl13), regData13.u32);
		}
		break;
		case 4:
		{
			T32Gbl_avPllCtl12 regData12;
			T32Gbl_avPllCtl13 regData13;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), &regData12.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl13), &regData13.u32);
			regData12.uavPllCtl_avPllDivAv1C4_A = divAV1;
			regData13.uavPllCtl_avPllDivAv2C4_A = divAV2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), regData12.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl13), regData13.u32);
		}
		break;
		case 5:
		{
			T32Gbl_avPllCtl12 regData12;
			T32Gbl_avPllCtl13 regData13;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), &regData12.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl13), &regData13.u32);
			regData12.uavPllCtl_avPllDivAv1C5_A = divAV1;
			regData13.uavPllCtl_avPllDivAv2C5_A = divAV2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), regData12.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl13), regData13.u32);
		}
		break;
		case 6:
		{
			T32Gbl_avPllCtl12 regData12;
			T32Gbl_avPllCtl13 regData13;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), &regData12.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl13), &regData13.u32);
			regData12.uavPllCtl_avPllDivAv1C6_A = divAV1;
			regData13.uavPllCtl_avPllDivAv2C6_A = divAV2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), regData12.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl13), regData13.u32);
		}
		break;
		case 7:
		{
			T32Gbl_avPllCtl12 regData12;
			T32Gbl_avPllCtl14 regData14;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), &regData12.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl14), &regData14.u32);
			regData12.uavPllCtl_avPllDivAv1C7_A = divAV1;
			regData14.uavPllCtl_avPllDivAv2C7_A = divAV2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), regData12.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl14), regData14.u32);
		}
		break;
		default:
			dbg_printf(PRN_RES, "Invalid Channel Id\n");
		break;
	}
}

//divAV1 is the register value, not the actual divider
//mapping is (binary):
//000 = bypass
//100 = div by 1
//101 = div by 2
//110 = div by 5
//111 = div by 5
void diag_setAV_Div_B(int divAV1, int divAV2, int chId)
{

	if(divAV2 !=0 && (divAV2<4 || divAV2>127))
	{
		dbg_printf(PRN_RES, "Invalid divider for AV1\n");
			return ;
	}


	//program DIV_AV1 and DIV_AV2
	switch(chId)
	{
		case 1:
		{
			T32Gbl_avPllCtl42 regData42;
			T32Gbl_avPllCtl43 regData43;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl42), &regData42.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl43), &regData43.u32);
			regData42.uavPllCtl_avPllDivAv1C1_B = divAV1;
			regData43.uavPllCtl_avPllDivAv2C1_B = divAV2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl42), regData42.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl43), regData43.u32);
		}
		break;
		case 2:
		{
			T32Gbl_avPllCtl43 regData43;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl43), &regData43.u32);
			regData43.uavPllCtl_avPllDivAv1C2_B = divAV1;
			regData43.uavPllCtl_avPllDivAv2C2_B = divAV2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl43), regData43.u32);
		}
		break;
		case 3:
		{
			T32Gbl_avPllCtl43 regData43;
			T32Gbl_avPllCtl44 regData44;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl43), &regData43.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl44), &regData44.u32);
			regData43.uavPllCtl_avPllDivAv1C3_B = divAV1;
			regData44.uavPllCtl_avPllDivAv2C3_B = divAV2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl43), regData43.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl44), regData44.u32);
		}
		break;
		case 4:
		{
			T32Gbl_avPllCtl43 regData43;
			T32Gbl_avPllCtl44 regData44;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl43), &regData43.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl44), &regData44.u32);
			regData43.uavPllCtl_avPllDivAv1C4_B = divAV1;
			regData44.uavPllCtl_avPllDivAv2C4_B = divAV2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl43), regData43.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl44), regData44.u32);
		}
		break;
		case 5:
		{
			T32Gbl_avPllCtl43 regData43;
			T32Gbl_avPllCtl44 regData44;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl43), &regData43.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl44), &regData44.u32);
			regData43.uavPllCtl_avPllDivAv1C5_B = divAV1;
			regData44.uavPllCtl_avPllDivAv2C5_B = divAV2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl43), regData43.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl44), regData44.u32);
		}
		break;
		case 6:
		{
			T32Gbl_avPllCtl43 regData43;
			T32Gbl_avPllCtl44 regData44;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl43), &regData43.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl44), &regData44.u32);
			regData43.uavPllCtl_avPllDivAv1C6_B = divAV1;
			regData44.uavPllCtl_avPllDivAv2C6_B = divAV2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl43), regData43.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl44), regData44.u32);
		}
		break;
		case 7:
		{
			T32Gbl_avPllCtl43 regData43;
			T32Gbl_avPllCtl45 regData45;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl43), &regData43.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl45), &regData45.u32);
			regData43.uavPllCtl_avPllDivAv1C7_B = divAV1;
			regData45.uavPllCtl_avPllDivAv2C7_B = divAV2;
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl43), regData43.u32);
			BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl45), regData45.u32);
		}
		break;
		default:
			dbg_printf(PRN_RES, "Invalid Channel Id\n");
		break;
	}
}

//Input is the hex code for AV2 and AV3, see table III
void diag_setAV3_Div_A(int divAV2, int divAV3, int chId)
{
	T32Gbl_avPllCtl12 regData12;
	T32Gbl_avPllCtl13 regData13;
	T32Gbl_avPllCtl14 regData14;
	T32Gbl_avPllCtl15 regData15;
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), &regData12.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl13), &regData13.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl14), &regData14.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl15), &regData15.u32);


	//program DIV_AV1 and DIV_AV2
	switch(chId)
	{
		case 1:
		{
			regData12.uavPllCtl_avPllDivAv2C1_A = divAV2;
			regData14.uavPllCtl_avPllDivAv3C1_A = divAV3;
		}
		break;
		case 2:
		{
			regData12.uavPllCtl_avPllDivAv2C2_A = divAV2;
			regData14.uavPllCtl_avPllDivAv3C2_A = divAV3;
		}
		break;
		case 3:
		{
			regData13.uavPllCtl_avPllDivAv2C3_A = divAV2;
			regData14.uavPllCtl_avPllDivAv3C3_A = divAV3;
		}
		break;
		case 4:
		{
			regData13.uavPllCtl_avPllDivAv2C4_A = divAV2;
			regData14.uavPllCtl_avPllDivAv3C4_A = divAV3;
		}
		break;
		case 5:
		{
			regData13.uavPllCtl_avPllDivAv2C5_A = divAV2;
			regData14.uavPllCtl_avPllDivAv3C5_A = divAV3;			}
		break;
		case 6:
		{
			regData13.uavPllCtl_avPllDivAv2C6_A = divAV2;
			regData14.uavPllCtl_avPllDivAv3C6_A = divAV3;			}
		break;
		case 7:
		{
			regData14.uavPllCtl_avPllDivAv2C7_A = divAV2;
			regData15.uavPllCtl_avPllDivAv3C7_A = divAV3;			}
		break;
		default:
			dbg_printf(PRN_RES, "Invalid Channel Id\n");
		break;
	}

	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), regData12.u32);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl13), regData13.u32);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl14), regData14.u32);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl15), regData15.u32);

}

void diag_setAV3_Div_B(int divAV2, int divAV3, int chId)
{
	T32Gbl_avPllCtl43 regData43;
	T32Gbl_avPllCtl44 regData44;
	T32Gbl_avPllCtl45 regData45;
	T32Gbl_avPllCtl46 regData46;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl43), &regData43.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl44), &regData44.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl45), &regData45.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl46), &regData46.u32);

	switch(chId)
	{
		case 1:
		{
			regData43.uavPllCtl_avPllDivAv2C1_B = divAV2;
			regData45.uavPllCtl_avPllDivAv3C1_B = divAV3;
		}
		break;
		case 2:
		{
			regData43.uavPllCtl_avPllDivAv2C2_B = divAV2;
			regData45.uavPllCtl_avPllDivAv3C2_B = divAV3;
		}
		break;
		case 3:
		{
			regData44.uavPllCtl_avPllDivAv2C3_B = divAV2;
			regData45.uavPllCtl_avPllDivAv3C3_B = divAV3;
		}
		break;
		case 4:
		{
			regData44.uavPllCtl_avPllDivAv2C4_B = divAV2;
			regData45.uavPllCtl_avPllDivAv3C4_B = divAV3;
		}
		break;
		case 5:
		{
			regData44.uavPllCtl_avPllDivAv2C5_B = divAV2;
			regData45.uavPllCtl_avPllDivAv3C5_B = divAV3;
		}
		break;
		case 6:
		{
			regData44.uavPllCtl_avPllDivAv2C6_B = divAV2;
			regData45.uavPllCtl_avPllDivAv3C6_B = divAV3;
		}
		break;
		case 7:
		{
			regData45.uavPllCtl_avPllDivAv2C7_B = divAV2;
			regData46.uavPllCtl_avPllDivAv3C7_B = divAV3;
		}
		break;
		default:
			dbg_printf(PRN_RES, "Invalid Channel Id\n");
		break;
	}

	//write divAV2, divAV3 back
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl43), regData43.u32);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl44), regData44.u32);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl45), regData45.u32);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl46), regData46.u32);
}


#define OFFSET_1KPPM_INC	(-4194)
#define OFFSET_1KPPM_DEC	(4194)

//this is done by using offset and dpll
//inc=1 means increase freq to 1001/1000
//inc=0 means disable 1KPPM (by setting offset to 0 and disable DPLL)
//inc=-1 means decrease freq to 1000/1001
void diag_set1KPPM_A(int inc, int chId)
{
	if(inc)
	{
		if(inc>0)
		{
			//increase by 1KPPM
			diag_setChanOffset_A(OFFSET_1KPPM_INC, chId);
			diag_setDPll_A(1, 1000, 1001, chId);
		}
		else
		{
			//decrease by 1KPPM
			diag_setChanOffset_A(OFFSET_1KPPM_DEC, chId);
			diag_setDPll_A(1, 1001, 1000, chId);
		}
	}
	else
	{
		//set offset to 0 and disable DPLL
		diag_setChanOffset_A(0, chId);
		diag_setDPll_A(0, 0, 0, chId);
	}
}

void diag_set1KPPM_B(int inc, int chId)
{
	if(inc)
	{
		if(inc>0)
		{
			//increase by 1KPPM
			diag_setChanOffset_B(OFFSET_1KPPM_INC, chId);
			diag_setDPll_B(1, 1000, 1001, chId);
		}
		else
		{
			//decrease by 1KPPM
			diag_setChanOffset_B(OFFSET_1KPPM_DEC, chId);
			diag_setDPll_B(1, 1001, 1000, chId);
		}
	}
	else
	{
		//set offset to 0 and disable DPLL
		diag_setChanOffset_B(0, chId);
		diag_setDPll_B(0, 0, 0, chId);
	}
}

/* new reference in MHz, chan : chan_A 0, Chan_B 1 */
void diag_changeRefFreq(int vco_freq_index, int grp)
{
	if(grp ==0)
	{
		diag_setVCO_A(vco_freq_index);
	}
	else
	{
		diag_setVCO_B(vco_freq_index);
	}
}

void diag_setMasterSlaveB(int MasterSlaveB_A, int MasterSlaveB_B)
{
	T32Gbl_avPllCtl30 regData30;
	T32Gbl_avPllCtl61 regData61;
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl30), &regData30.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl61), &regData61.u32);

	regData30.uavPllCtl_avPllMasterSlaveB_A = MasterSlaveB_A;
	regData61.uavPllCtl_avPllMasterSlaveB_B = MasterSlaveB_B;

	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl30), regData30.u32);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl61), regData61.u32);
}

void diag_assertReset_A()
{
	T32Gbl_avPllCtl reg;

	//assert reset
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), &reg.u32);
	reg.uavPllCtl_avPllResetPll_A=1;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), reg.u32);
}

void diag_deassertReset_A()
{
	volatile int i;

	T32Gbl_avPllCtl reg;

	for(i=0; i<10000; i++);

	//deassert reset
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), &reg.u32);
	reg.uavPllCtl_avPllResetPll_A=0;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), reg.u32);
}

void diag_assertReset_B()
{
	T32Gbl_avPllCtl31 reg;

	//assert reset
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl31), &reg.u32);
	reg.uavPllCtl_avPllResetPll_B=1;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl31), reg.u32);
}

void diag_deassertReset_B()
{
	volatile int i;

	T32Gbl_avPllCtl31 reg;

	for(i=0; i<10000; i++);

	//deassert reset
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl31), &reg.u32);
	reg.uavPllCtl_avPllResetPll_B=0;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl31), reg.u32);

}

void diag_powerDown_A()
{
	T32Gbl_avPllCtl regData;
	T32Gbl_avPllCtl10 regData10;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), &regData.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl10), &regData10.u32);
	regData.uavPllCtl_avPllPu_A = 0;
	regData10.uavPllCtl_avPllPuC_A = 0;

	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), regData.u32);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl10), regData10.u32);

}

void diag_powerUp_A()
{
	T32Gbl_avPllCtl regData;
	T32Gbl_avPllCtl10 regData10;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), &regData.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl10), &regData10.u32);
	regData.uavPllCtl_avPllPu_A = 1;
	regData10.uavPllCtl_avPllPuC_A = 0x7f;

	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), regData.u32);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl10), regData10.u32);
}

void diag_powerDown_B()
{
	T32Gbl_avPllCtl31 regData31;
	T32Gbl_avPllCtl41 regData41;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl31), &regData31.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl41), &regData41.u32);
	regData31.uavPllCtl_avPllPu_B = 0;
	regData41.uavPllCtl_avPllPuC_B = 0;

	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl31), regData31.u32);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl41), regData41.u32);
}

void diag_powerUp_B()
{
	T32Gbl_avPllCtl31 regData31;
	T32Gbl_avPllCtl41 regData41;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl31), &regData31.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl41), &regData41.u32);
	regData31.uavPllCtl_avPllPu_B = 1;
	regData41.uavPllCtl_avPllPuC_B = 0x7f;

	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl31), regData31.u32);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl41), regData41.u32);
}

//this function will enable AVPLL A and B and turn on caliberation
void AVPLL_Enable(void)
{
	T32Gbl_avPllCtl31 regData31;
	T32Gbl_avPllCtl32 regData32;

	T32Gbl_avPllCtl12 regData12;
	T32Gbl_avPllCtl1 regData1;
	T32Gbl_avPllCtl regData;
	T32Gbl_clkEnable  regClkEnable;
	T32Gbl_PadSelect  regPadSelect;
	T32Gbl_avPllCtl11 regData11;
	T32Gbl_avPllCtl42 regData42;
	T32Gbl_avPllCtl9 regData9;
	T32Gbl_avPllCtl10 regData10;
	T32Gbl_avPllCtl40 regData40;
	T32Gbl_avPllCtl41 regData41;

	volatile int i;
	int avpll_initialized=0;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl31), &regData31.u32);
 	//speed is not defaut value, then pll must has been initialized.
	if(regData31.uavPllCtl_avPllSpeed_B != diag_avpllRegSPEED[0])
		avpll_initialized=1;
	//turn on the reset_A and reset_B
	//these should be always on
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), &regData11.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl42), &regData42.u32);
	regData11.uavPllCtl_avPllResetC_A = 0;
	regData42.uavPllCtl_avPllResetC_B = 0;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), regData11.u32);
	if(!avpll_initialized)
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl42), regData42.u32);

	//turn on offset
	//these should be always on
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), &regData9.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl10), &regData10.u32);
	regData9.uavPllCtl_avPllReserveC1_A = 1;
	regData9.uavPllCtl_avPllReserveC2_A = 1;
	regData10.uavPllCtl_avPllReserveC3_A = 1;
	regData10.uavPllCtl_avPllReserveC4_A = 1;
	regData10.uavPllCtl_avPllReserveC5_A = 1;
	regData10.uavPllCtl_avPllReserveC6_A = 1;
	regData10.uavPllCtl_avPllReserveC7_A = 1;
	regData10.uavPllCtl_avPllReserveC8_A = 1;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), regData9.u32);
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl10), regData10.u32);

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl40), &regData40.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl41), &regData41.u32);
	regData40.uavPllCtl_avPllReserveC1_B = 1;
	regData40.uavPllCtl_avPllReserveC2_B = 1;
	regData41.uavPllCtl_avPllReserveC3_B = 1;
	regData41.uavPllCtl_avPllReserveC4_B = 1;
	regData41.uavPllCtl_avPllReserveC5_B = 1;
	regData41.uavPllCtl_avPllReserveC6_B = 1;
	regData41.uavPllCtl_avPllReserveC7_B = 1;
	regData41.uavPllCtl_avPllReserveC8_B = 1;
	if(!avpll_initialized) {
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl40), regData40.u32);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl41), regData41.u32);
	}

        //set p9v to correct value
        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), &regData.u32);
        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl31), &regData31.u32);
        regData.uavPllCtl_avPllVddr1p45V_A = 0;
        regData.uavPllCtl_avPllVddr0p9V_A = 0x5;
        regData31.uavPllCtl_avPllVddr0p9V_B = 0x5;
        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), regData.u32);
	if(!avpll_initialized)
        	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl31), regData31.u32);

        //set ICP to correct value
        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl1), &regData1.u32);
        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl32), &regData32.u32);
        regData1.uavPllCtl_avPllIcp_A = 0x3;
        regData32.uavPllCtl_avPllIcp_B = 0x3;
        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl1), regData1.u32);
	if(!avpll_initialized)
        	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl32), regData32.u32);

	diag_assertReset_A();
	if(!avpll_initialized)
		diag_assertReset_B();

	diag_setVCO_A(AVPLL_VCO_FREQ_2_227G);
	if(!avpll_initialized)
		diag_setVCO_B(AVPLL_VCO_FREQ_1_620G);

	//power up
	diag_powerUp_A();
	if(!avpll_initialized)
		diag_powerUp_B();

	//reset, has delay inside function
	diag_deassertReset_A();
	if(!avpll_initialized)
		diag_deassertReset_B();

	//caliberate
	diag_calibrate_A();
	if(!avpll_initialized)
		diag_calibrate_B();

	//set up VCO frequency.
	diag_setVCO_A(AVPLL_VCO_FREQ_2_227G);
	if(!avpll_initialized)
		diag_setVCO_B(AVPLL_VCO_FREQ_1_620G);

	dbg_printf(PRN_RES, "Enabled AvPLL A and B Channels\n");

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkEnable), &regClkEnable.u32);
	regClkEnable.uclkEnable_audio0ClkEn = 1;
	regClkEnable.uclkEnable_audio1ClkEn = 1;
	regClkEnable.uclkEnable_audio2ClkEn = 1;
	regClkEnable.uclkEnable_audio3ClkEn = 1;
	regClkEnable.uclkEnable_audioHdClkEn = 1;
	regClkEnable.uclkEnable_video0ClkEn = 1;
	regClkEnable.uclkEnable_video1ClkEn = 1;
	regClkEnable.uclkEnable_video2ClkEn = 1;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkEnable), regClkEnable.u32);

	dbg_printf(PRN_RES, "Enable DVIO Pad\n");
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_PadSelect), &regPadSelect.u32);
	regPadSelect.uPadSelect_DVIO_OEN = 1;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_PadSelect), regPadSelect.u32);

        for (i=0; i<8; i++) {
            gAVPLLA_Channel_OutputClock[i] = 0;
            gAVPLLB_Channel_OutputClock[i] = 0;
        }

#if (BERLIN_CHIP_VERSION >= BERLIN_BG2_A0)
        diag_videoFreq_A(5, 0, 0, 1, 6); // set default video0clk to 148.5M
        diag_videoFreq_A(5, 0, 0, 1, 5); // set default video1clk to 148.5M
        diag_videoFreq_B(1, 2, 0, 1, 6); // set default video1clk to 148.5M
#else
        diag_videoFreq_A(5, 0, 0, 1, 1); // set default video0clk to 148.5M
        diag_videoFreq_A(5, 0, 0, 1, 2); // set default video1clk to 148.5M
        diag_videoFreq_B(1, 2, 0, 1, 1); // set default video1clk to 148.5M
#endif
}

void diag_calibrate_A(void)
{
	T32Gbl_avPllCtl1 regData1;
	volatile int i;

	//turn off caliberation
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl1), &regData1.u32);
	regData1.uavPllCtl_avPllPllCaliStart_A=0;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl1), regData1.u32);

	//add some delay
	for(i=0; i<10000; i++);

	//enable caliberation
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl1), &regData1.u32);
	regData1.uavPllCtl_avPllPllCaliStart_A=1;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl1), regData1.u32);
}

void diag_calibrate_B(void)
{
	T32Gbl_avPllCtl32 regData32;
	volatile int i;

	//turn off caliberation
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl32), &regData32.u32);
	regData32.uavPllCtl_avPllPllCaliStart_B=0;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl32), regData32.u32);

	//add some delay
	for(i=0; i<10000; i++);

	//enable caliberation
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl32), &regData32.u32);
	regData32.uavPllCtl_avPllPllCaliStart_B=1;
	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl32), regData32.u32);
}

int divHDMICode[] = {
	1, 2, 4, 6
};

int divAV1Code[] = {
	1, 2, 5, 5
};

void diag_avpllFreq_A(int chId)
{
	double vco_freq=diag_vcoFreqs[diag_pll_A_VCO_Setting];
	double freq_from_dpll, freq_from_offset;
	int offset, sync1, sync2, divHDMI, divAV1, divAV2, divAV3;
	int enDPLL;

	T32Gbl_avPllCtl11 regData11;
	T32Gbl_avPllCtl12 regData12;
	T32Gbl_avPllCtl13 regData13;
	T32Gbl_avPllCtl14 regData14;
	T32Gbl_avPllCtl15 regData15;
	T32Gbl_avPllCtl30 regData30;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), &regData11.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), &regData12.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl13), &regData13.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl14), &regData14.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl15), &regData15.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl30), &regData30.u32);

	enDPLL = regData30.uavPllCtl_avPllEnDpllC_A & (1<<(chId-1));

	//find offset, sync1, sync2, divHDMI, divAV1, divAV2, divAV3
	switch(chId)
	{
		case 1:
		{
			T32Gbl_avPllCtl2 regData2;
			T32Gbl_avPllCtl23 regData23;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl2), &regData2.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl23), &regData23.u32);

			offset = regData2.uavPllCtl_avPllFreqOffsetC1_A;
			sync1 =  regData15.uavPllCtl_avPllPSync1C1_A;
			sync2 =  regData23.uavPllCtl_avPllPSync2C1_A;
			divHDMI= regData11.uavPllCtl_avPllDivHdmiC1_A;
			divAV1 = regData11.uavPllCtl_avPllDivAv1C1_A;
			divAV2 = regData12.uavPllCtl_avPllDivAv2C1_A;
			divAV3 = regData14.uavPllCtl_avPllDivAv3C1_A;
		}
		break;
		case 2:
		{
			T32Gbl_avPllCtl3 regData3;
			T32Gbl_avPllCtl16 regData16;
			T32Gbl_avPllCtl24 regData24;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl3), &regData3.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl16), &regData16.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl24), &regData24.u32);

			offset = regData3.uavPllCtl_avPllFreqOffsetC2_A;
			sync1 =  regData16.uavPllCtl_avPllPSync1C2_A;
			sync2 =  regData24.uavPllCtl_avPllPSync2C2_A;
			divHDMI= regData11.uavPllCtl_avPllDivHdmiC2_A;
			divAV1 = regData12.uavPllCtl_avPllDivAv1C2_A;
			divAV2 = regData12.uavPllCtl_avPllDivAv2C2_A;
			divAV3 = regData14.uavPllCtl_avPllDivAv3C2_A;
		}
		break;
		case 3:
		{
			T32Gbl_avPllCtl4 regData4;
			T32Gbl_avPllCtl17 regData17;
			T32Gbl_avPllCtl25 regData25;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl4), &regData4.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), &regData17.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl25), &regData25.u32);
			offset = regData4.uavPllCtl_avPllFreqOffsetC3_A;
			sync1 =  regData17.uavPllCtl_avPllPSync1C3_A;
			sync2 =  regData25.uavPllCtl_avPllPSync2C3_A;
			divHDMI= regData11.uavPllCtl_avPllDivHdmiC3_A;
			divAV1 = regData12.uavPllCtl_avPllDivAv1C3_A;
			divAV2 = regData13.uavPllCtl_avPllDivAv2C3_A;
			divAV3 = regData14.uavPllCtl_avPllDivAv3C3_A;
		}
		break;
		case 4:
		{
			T32Gbl_avPllCtl5 regData5;
			T32Gbl_avPllCtl18 regData18;
			T32Gbl_avPllCtl26 regData26;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl5), &regData5.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl18), &regData18.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl26), &regData26.u32);
			offset = regData5.uavPllCtl_avPllFreqOffsetC4_A;
			sync1 =  regData18.uavPllCtl_avPllPSync1C4_A;
			sync2 =  regData26.uavPllCtl_avPllPSync2C4_A;
			divHDMI= regData11.uavPllCtl_avPllDivHdmiC4_A;
			divAV1 = regData12.uavPllCtl_avPllDivAv1C4_A;
			divAV2 = regData13.uavPllCtl_avPllDivAv2C4_A;
			divAV3 = regData14.uavPllCtl_avPllDivAv3C4_A;
		}
		break;
		case 5:
		{
			T32Gbl_avPllCtl6 regData6;
			T32Gbl_avPllCtl19 regData19;
			T32Gbl_avPllCtl27 regData27;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl6), &regData6.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl19), &regData19.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl27), &regData27.u32);
			offset = regData6.uavPllCtl_avPllFreqOffsetC5_A;
			sync1 =  regData19.uavPllCtl_avPllPSync1C5_A;
			sync2 =  regData27.uavPllCtl_avPllPSync2C5_A;
			divHDMI= regData11.uavPllCtl_avPllDivHdmiC5_A;
			divAV1 = regData12.uavPllCtl_avPllDivAv1C5_A;
			divAV2 = regData13.uavPllCtl_avPllDivAv2C5_A;
			divAV3 = regData14.uavPllCtl_avPllDivAv3C5_A;
		}
		break;
		case 6:
		{
			T32Gbl_avPllCtl7 regData7;
			T32Gbl_avPllCtl20 regData20;
			T32Gbl_avPllCtl28 regData28;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl7), &regData7.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl20), &regData20.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl28), &regData28.u32);
			offset = regData7.uavPllCtl_avPllFreqOffsetC6_A;
			sync1 =  regData20.uavPllCtl_avPllPSync1C6_A;
			sync2 =  regData28.uavPllCtl_avPllPSync2C6_A;
			divHDMI= regData11.uavPllCtl_avPllDivHdmiC6_A;
			divAV1 = regData12.uavPllCtl_avPllDivAv1C6_A;
			divAV2 = regData13.uavPllCtl_avPllDivAv2C6_A;
			divAV3 = regData14.uavPllCtl_avPllDivAv3C6_A;
		}
		break;
		case 7:
		{
			T32Gbl_avPllCtl8 regData8;
			T32Gbl_avPllCtl21 regData21;
			T32Gbl_avPllCtl29 regData29;
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl8), &regData8.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl21), &regData21.u32);
			BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl29), &regData29.u32);
			offset = regData8.uavPllCtl_avPllFreqOffsetC7_A;
			sync1 =  regData21.uavPllCtl_avPllPSync1C7_A;
			sync2 =  regData29.uavPllCtl_avPllPSync2C7_A;
			divHDMI= regData11.uavPllCtl_avPllDivHdmiC7_A;
			divAV1 = regData12.uavPllCtl_avPllDivAv1C7_A;
			divAV2 = regData14.uavPllCtl_avPllDivAv2C7_A;
			divAV3 = regData15.uavPllCtl_avPllDivAv3C7_A;
		}
		break;
		default:
			dbg_printf(PRN_RES, "Invalid Channel\n");
			return;

	}

	if(enDPLL != 0)
		freq_from_dpll = vco_freq*sync2/sync1;
	else
		freq_from_dpll = vco_freq;

	if(offset& (1<<18))
	{
		offset = - (offset & ((1<<18)-1));
	}
	freq_from_offset = vco_freq/((double)offset/4194304+1);

	//figure out which divider used
	if(divHDMI != 0)
	{
		divHDMI &= 0x3;
		dbg_printf(PRN_RES, "divHDMI on: %d\n", divHDMI);

		freq_from_dpll = freq_from_dpll/divHDMICode[divHDMI];
		freq_from_offset = freq_from_offset/divHDMICode[divHDMI];
	}

	if(divAV1 != 0)
	{
		divAV1 &= 0x3;
		dbg_printf(PRN_RES, "divAV1 on: %d\n", divAV1);

		freq_from_dpll = freq_from_dpll/divAV1Code[divAV1];
		freq_from_offset = freq_from_offset/divAV1Code[divAV1];
	}

	if(divAV2 != 0)
	{
		if(divAV3 == 0)
		{
			dbg_printf(PRN_RES, "divAV2 on: %d\n", divAV2);
			freq_from_dpll = freq_from_dpll/divAV2;
			freq_from_offset = freq_from_offset/divAV2;
		}
		else
		{
			dbg_printf(PRN_RES, "divAV2 on: %d\n", divAV2);
			dbg_printf(PRN_RES, "divAV3 on: %d\n", divAV3);
			if( (((int)((double)divAV2/4+.5))-1) != (divAV3&0x7))
			{
				dbg_printf(PRN_RES, "Invalid divAV2 and divAV3 combination\n");
			}
			freq_from_dpll = freq_from_dpll*2/divAV2;
			freq_from_offset = freq_from_offset*2/divAV2;
		}
	}

	dbg_printf(PRN_RES, "PLLA C%d Freq is: %fMHz(Offset) %fMHz(DPLL)\n", chId, freq_from_offset, freq_from_dpll);

	current_freq[0][chId]=freq_from_dpll;
}

void diag_avpllFreq_B(int chId)
{
	double vco_freq=diag_vcoFreqs[diag_pll_B_VCO_Setting];
	double freq_from_dpll, freq_from_offset;
	int offset, sync1, sync2, divHDMI, divAV1, divAV2, divAV3;
	int enDPLL;

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
			dbg_printf(PRN_RES, "Invalid Channel\n");
			return;

	}

	if(enDPLL != 0)
		freq_from_dpll = vco_freq*sync2/sync1;
	else
		freq_from_dpll = vco_freq;

	if(offset& (1<<18))
	{
		offset = - (offset & ((1<<18)-1));
	}
	freq_from_offset = vco_freq/((double)offset/4194304+1);

	//figure out which divider used
	if(divHDMI != 0)
	{
		divHDMI &= 0x3;
		dbg_printf(PRN_RES, "divHDMI on: %d\n", divHDMI);

		freq_from_dpll = freq_from_dpll/divHDMICode[divHDMI];
		freq_from_offset = freq_from_offset/divHDMICode[divHDMI];
	}

	if(divAV1 != 0)
	{
		divAV1 &= 0x3;
		dbg_printf(PRN_RES, "divAV1 on: %d\n", divAV1);

		freq_from_dpll = freq_from_dpll/divAV1Code[divAV1];
		freq_from_offset = freq_from_offset/divAV1Code[divAV1];
	}

	if(divAV2 != 0)
	{
		if(divAV3 == 0)
		{
			dbg_printf(PRN_RES, "divAV2 on: %d\n", divAV2);
			freq_from_dpll = freq_from_dpll/divAV2;
			freq_from_offset = freq_from_offset/divAV2;
		}
		else
		{
			dbg_printf(PRN_RES, "divAV2 on: %d\n", divAV2);
			dbg_printf(PRN_RES, "divAV3 on: %d\n", divAV3);
			if( (((int)((double)divAV2/4+.5))-1) != (divAV3&0x7))
			{
				dbg_printf(PRN_RES, "Invalid divAV2 and divAV3 combination\n");
			}
			freq_from_dpll = freq_from_dpll*2/divAV2;
			freq_from_offset = freq_from_offset*2/divAV2;
		}
	}

	dbg_printf(PRN_RES, "PLLB C%d Freq is: %fMHz(Offset) %fMHz(DPLL)\n", chId, freq_from_offset, freq_from_dpll);

	current_freq[1][chId]=freq_from_dpll;
}

void diag_vcoFreq_A(void)
{
	double ref_freq=25.0;
	double freq_from_offset;
	int offset, fbDiv;

	T32Gbl_avPllCtl1 regData1;
	T32Gbl_avPllCtl9 regData9;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl1), &regData1.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), &regData9.u32);

	//find offset, fbDiv
	offset = regData9.uavPllCtl_avPllFreqOffsetC8_A;
	fbDiv =  regData1.uavPllCtl_avPllFbDiv_A;

	if(offset& (1<<18))
	{
		offset = - (offset & ((1<<18)-1));
	}
	//note that the offset formula for VCO(C8) is different from C1-C7
	//according to John Ma, the formula is the same, just that ref_freq is CKOUT
	//and VCO freq is CKIN
	freq_from_offset = ref_freq*((double)offset/4194304+1);

	freq_from_offset = freq_from_offset*fbDiv;

	dbg_printf(PRN_RES, "PLLA VCO Freq is: %f\n", freq_from_offset);

}

void diag_vcoFreq_B(void)
{
	double ref_freq=25.0;
	double freq_from_offset;
	int offset, fbDiv;

	T32Gbl_avPllCtl32 regData32;
	T32Gbl_avPllCtl40 regData40;

	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl32), &regData32.u32);
	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl40), &regData40.u32);

	//find offset, fbDiv
	offset = regData40.uavPllCtl_avPllFreqOffsetC8_B;
	fbDiv =  regData32.uavPllCtl_avPllFbDiv_B;

	if(offset& (1<<18))
	{
		offset = - (offset & ((1<<18)-1));
	}
	//note that the offset formula for VCO(C8) is different from C1-C7
	//according to John Ma, the formula is the same, just that ref_freq is CKOUT
	//and VCO freq is CKIN
	freq_from_offset = ref_freq*((double)offset/4194304+1);

	freq_from_offset = freq_from_offset*fbDiv;

	dbg_printf(PRN_RES, "PLLB VCO Freq is: %f\n", freq_from_offset);

}

int diag_getVCOFreq(int hdmiMode, int freqIndex)
{
    return (vcoSelectionTable[hdmiMode][freqIndex]);
}

int diag_getChannelOutputFreq(int AVPLLIndex,int chID)
{
    if (chID<0 || chID>7)
        return 0;

    if (AVPLLIndex == 0) //AVPLL-A
        return (gAVPLLA_Channel_OutputClock[chID]);
    else
        return (gAVPLLB_Channel_OutputClock[chID]);
}

//freq_index, 0 to 5, the 6 VCLKs
//hdmiMode, HDMI_8BIT_MODE, HDMI_10BIT_MODE, HDMI_12BIT_MODE
//frameRate, FRAME_RATE_59P94 FRAME_RATE_60
//overSampleRate, 1, 2, 4, 8, 10, 12.5, 15
void diag_videoFreq_A(int freqIndex, int hdmiMode, int frameRate, float overSampleRate, int chId)
{
            int vcoFreqIndex=vcoSelectionTable[hdmiMode][freqIndex];
            double vcoFreq, videoFreq;
            int ppm1K=ppm1kSelectionTable[frameRate][freqIndex];
            double divider;

            if (chId<0 || chId>7)
                return;

            dbg_printf(PRN_RES, "Set PLLA C%d:\n", chId);
            //set VCO freq here
            diag_setVCO_A(vcoFreqIndex);
            dbg_printf(PRN_RES, "VCO freq=%f\n", diag_vcoFreqs[vcoFreqIndex]);

            //check to see if ppm1K is need
            vcoFreq = diag_vcoFreqs[vcoFreqIndex];
            if(ppm1K<0)
            {
                        vcoFreq = vcoFreq*1000/1001;
            }

            if(ppm1K>0)
            {
                        vcoFreq = vcoFreq*1001/1000;
            }
            diag_set1KPPM_A(ppm1K, chId);

            //get the video freq
            videoFreq = diag_videoFreqs[freqIndex];
            if(frameRate)
            {
                        //60Hz vclk is 1001/1000 times 59.94Hz vclk
                        videoFreq = videoFreq*1001/1000;
            }

            dbg_printf(PRN_RES, "Video freq=%f\n", videoFreq);

            //disable all dividers
            diag_setHDMI_Div_A(0, chId);
            diag_setAV_Div_A(0, 0, chId); //divAV1 is for divide by 1, 0 will disable the whole divAV chain
            diag_setAV3_Div_A(0, 0, chId);

            //set the divider
            if(overSampleRate>=10)
            {
                        //use HDMI divider
                        divider = vcoFreq/videoFreq/overSampleRate;
                        dbg_printf(PRN_RES, "HDMI divider is %f %d\n", divider, (int)(divider+0.5));

                        diag_setHDMI_Div_A((int)(divider+0.5), chId);
                        dbg_printf(PRN_RES, "PLLA C%d=%f\n", chId, videoFreq*overSampleRate);
            }
            else
            {
                        divider = vcoFreq*2/videoFreq/overSampleRate;
                        dbg_printf(PRN_RES, "AV divider is %f %d\n", divider/2, (int)(divider+0.5)/2);

                        if(((int)(divider+0.5))&1)
                        {
                                    //fractional divider, use AV2 and AV3
                                    //figure out AV3, AV3 = round(AV2/4)-1
                                    int divAV3 = ((int)((double)divider/4+0.5))-1;

                                    //this enables divAV channel
                                    diag_setAV_Div_A(4, 0, chId);

                                    //add enable bit
                                    divAV3 |= (1<<3);
                                    diag_setAV3_Div_A((int)(divider+0.5), divAV3, chId);
                        }
                        else
                        {
                                    //integer divider, use AV2 only
                                    diag_setAV_Div_A(4, (int)(divider+0.5)/2, chId);
                        }

                        dbg_printf(PRN_RES, "PLLA C%d=%f\n", chId, videoFreq*overSampleRate);
            }

            gAVPLLA_Channel_OutputClock[chId] = (unsigned int)(videoFreq*overSampleRate*1000000);

}

//freq_index, 0 to 5, the 6 VCLKs
//hdmiMode, HDMI_8BIT_MODE, HDMI_10BIT_MODE, HDMI_12BIT_MODE
//frameRate, FRAME_RATE_59P94 FRAME_RATE_60
//overSampleRate, 1, 2, 4, 8, 10, 12.5, 15
void diag_videoFreq_B(int freqIndex, int hdmiMode, int frameRate, float overSampleRate, int chId)
{
            int vcoFreqIndex=vcoSelectionTable[hdmiMode][freqIndex];
            double vcoFreq, videoFreq;
            int ppm1K=ppm1kSelectionTable[frameRate][freqIndex];
            double divider;

            if (chId<0 || chId>7)
                return;

            dbg_printf(PRN_RES, "Set PLLB C%d:\n", chId);

            //set VCO freq here
            diag_setVCO_B(vcoFreqIndex);
            dbg_printf(PRN_RES, "VCO freq=%f\n", diag_vcoFreqs[vcoFreqIndex]);
            //check to see if ppm1K is need
            vcoFreq = diag_vcoFreqs[vcoFreqIndex];

            if(ppm1K<0)
            {
                        vcoFreq = vcoFreq*1000/1001;
            }
            if(ppm1K>0)
            {
                        vcoFreq = vcoFreq*1001/1000;
            }

            diag_set1KPPM_B(ppm1K, chId);

            //get the video freq
            videoFreq = diag_videoFreqs[freqIndex];
            if(frameRate)
            {
                        //60Hz vclk is 1001/1000 times 59.94Hz vclk
                        videoFreq = videoFreq*1001/1000;
            }

            dbg_printf(PRN_RES, "Video freq=%f\n", videoFreq);

            //disable all dividers
            diag_setHDMI_Div_B(0, chId);
            diag_setAV_Div_B(0, 0, chId);
            diag_setAV3_Div_B(0, 0, chId);

            //set the divider
            if(overSampleRate>=10)
            {
                        //use HDMI divider
                        divider = vcoFreq/videoFreq/overSampleRate;
                        dbg_printf(PRN_RES, "HDMI divider is %f %d\n", divider, (int)(divider+0.5));

                        diag_setHDMI_Div_B((int)(divider+0.5), chId);
                        dbg_printf(PRN_RES, "PLLB C%d=%f\n", chId, videoFreq*overSampleRate);
            }
            else
            {
                        divider = vcoFreq*2/videoFreq/overSampleRate;
                        dbg_printf(PRN_RES, "AV divider is %f %d\n", divider/2, (int)((divider+0.5)/2));

                        if((((int)(divider+0.5)))&1)
                        {
                                    //fractional divider, use AV2 and AV3
                                    //figure out AV3, AV3 = round(AV2/4)-1
                                    int divAV3 = ((int)((double)divider/4+0.5))-1;

                                    //this enables divAV channel
                                    diag_setAV_Div_B(4, 0, chId);

                                    //add enable bit
                                    divAV3 |= (1<<3);
                                    diag_setAV3_Div_B((int)(divider+0.5), divAV3, chId);
                        }
                        else
                        {
                                    //integer divider, use AV2 only
                                    diag_setAV_Div_B(4, (int)(divider+0.5)/2, chId);
                        }

                        dbg_printf(PRN_RES, "PLLB C%d=%f\n", chId, videoFreq*overSampleRate);
            }

            gAVPLLB_Channel_OutputClock[chId] = (unsigned int)(videoFreq*overSampleRate*1000000);

}


//freqIndex is the index into clock_freq[] array
int diag_clockFreq_A(int freqIndex, int chId)
{

	if(freqIndex >= (sizeof(clk_freqs) / sizeof(CLK_FREQ)))
	{
		dbg_printf(PRN_RES, "freq Index not found\n");
		return 1;
	}

	if(diag_pll_A_VCO_Setting != clk_freqs[freqIndex].vco_freq_index)
	{
		dbg_printf(PRN_RES, "VCO frequency is changed to %f!\n",
			diag_vcoFreqs[clk_freqs[freqIndex].vco_freq_index]);

		diag_setVCO_A(clk_freqs[freqIndex].vco_freq_index);
	}

	//change offset
	diag_setChanOffset_A(clk_freqs[freqIndex].freq_offset, chId);

	//change p_sync
	diag_setDPll_A((clk_freqs[freqIndex].p_sync1!=0),
			clk_freqs[freqIndex].p_sync1,
			clk_freqs[freqIndex].p_sync2, chId);

	//disable all dividers
	diag_setHDMI_Div_A(0, chId);
	diag_setAV_Div_A(0, 0, chId);
	diag_setAV3_Div_A(0, 0, chId);

	//update now div
	diag_setAV_Div_A(clk_freqs[freqIndex].divAV1, clk_freqs[freqIndex].divAV2, chId);
	diag_setAV3_Div_A(clk_freqs[freqIndex].divAV2, clk_freqs[freqIndex].divAV3, chId);


	return 0;

}

int diag_clockFreq_B(int freqIndex, int chId)
{
	if(freqIndex >= (sizeof(clk_freqs) / sizeof(CLK_FREQ)))
	{
		dbg_printf(PRN_RES, "freq Index not found\n");
		return 1;
	}

	if(diag_pll_B_VCO_Setting != clk_freqs[freqIndex].vco_freq_index)
	{
		dbg_printf(PRN_RES, "VCO frequency is changed to %f!\n",
			diag_vcoFreqs[clk_freqs[freqIndex].vco_freq_index]);

		diag_setVCO_B(clk_freqs[freqIndex].vco_freq_index);
	}

	//change offset
	diag_setChanOffset_B(clk_freqs[freqIndex].freq_offset, chId);

	//change p_sync
	diag_setDPll_B((clk_freqs[freqIndex].p_sync1!=0),
			clk_freqs[freqIndex].p_sync1,
			clk_freqs[freqIndex].p_sync2, chId);

	//disable all dividers
	diag_setHDMI_Div_B(0, chId);
	diag_setAV_Div_B(0, 0, chId);
	diag_setAV3_Div_B(0, 0, chId);

	//update now div
	diag_setAV_Div_B(clk_freqs[freqIndex].divAV1, clk_freqs[freqIndex].divAV2, chId);
	diag_setAV3_Div_B(clk_freqs[freqIndex].divAV2, clk_freqs[freqIndex].divAV3, chId);


	return 0;
}

int get_freqTabIndex(int vco_freq_index, int av_freq_index)
{
	int i, tab_index=-1;

	for(i=0; i<(sizeof(clk_freqs) / sizeof(CLK_FREQ)); i++)
	{
		if(clk_freqs[i].vco_freq_index == vco_freq_index && clk_freqs[i].av_freq_index == av_freq_index)
		{
			tab_index = i; break;
		}
	}
	return tab_index;
}

int AVPLL_Set(int groupId, int chanId, int avFreqIndex)
{
	int freqTabIndex;

	if(groupId==0)	// PLL A
	{
		// Search freqIndex in lookup table by avFreqIndex and vco_freq_index.
		freqTabIndex = get_freqTabIndex(diag_pll_A_VCO_Setting, avFreqIndex);
		if(freqTabIndex == -1)
			return -1;
		diag_clockFreq_A(freqTabIndex, chanId);
	}
	else if(groupId==1) // PLL B
	{
		freqTabIndex = get_freqTabIndex(diag_pll_B_VCO_Setting, avFreqIndex);
		if(freqTabIndex == -1)
			return -1;

		diag_clockFreq_B(freqTabIndex, chanId);
	}

	return 0;
}

// secondary MCLK keeps 4x, other MCLK div4 to 1x
int AVPLL_SetDiv4()
{
	T32Gbl_ClkSwitch1  ClkSwitch1;
	T32Gbl_clkSelect2 ClkSelect2;
#ifdef INTERNAL_ADAC_DISABLE
	T32Gbl_clkSelect3 ClkSelect3;
#endif

        GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), &ClkSwitch1.u32);
        ClkSwitch1.uClkSwitch_audio0ClkSwitch = 1; // primary
        ClkSwitch1.uClkSwitch_audio2ClkSwitch = 1; // mic
        ClkSwitch1.uClkSwitch_audio3ClkSwitch = 1; // spdif
#ifdef INTERNAL_ADAC_DISABLE
	ClkSwitch1.uClkSwitch_audio1ClkSwitch = 1; // stereo
#endif
        GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ClkSwitch1), (ClkSwitch1.u32));

	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), &ClkSelect2.u32);
	ClkSelect2.uclkSelect_audio0ClkSel = Gbl_clkSelect_audio0ClkSel_d4;
	ClkSelect2.uclkSelect_audio2ClkSel = Gbl_clkSelect_audio2ClkSel_d4;
	ClkSelect2.uclkSelect_audio3ClkSel = Gbl_clkSelect_audio3ClkSel_d4;
	GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect2), (ClkSelect2.u32));

#ifdef INTERNAL_ADAC_DISABLE
	GA_REG_WORD32_READ((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect3), &ClkSelect3.u32);
	ClkSelect3.uclkSelect_audio1ClkSel = Gbl_clkSelect_audio1ClkSel_d4;
	GA_REG_WORD32_WRITE((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkSelect3), (ClkSelect3.u32));
#endif
}


// S/W workaround is not required at BG2.
void AVPLL_EnableMicClk(int en)
{
        T32Gbl_clkEnable  regClkEnable;
        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkEnable), &regClkEnable.u32);
	if (en) {
		en = 1;
	}
        regClkEnable.uclkEnable_audio2ClkEn = en;
        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkEnable), regClkEnable.u32);
}

#if 1 /* PCR_TRACKING -- BG2 */
// add read function here for adjustment, real adjustment to nominate frequency is cur-orig
void avpll_get_ppm(int grp, double *ppm_base, double *ppm_now)
{
	double ppm0, ppm;
	if(grp ==0)
	{
		ppm0 = org_ppm[0];
		ppm = cur_ppm[0];
	}
	else // AVPLL B
	{
		ppm0 = org_ppm[1];
		ppm = cur_ppm[1];
	}
	if(ppm_base) *ppm_base = ppm0;
	if(ppm_now) *ppm_now = ppm;
}

double avpll_adjust_ppm(double ppm_delta, int grp) {
        volatile int i;
        double ppm0, ppm;

        if(grp == 0) { // For AVPLL-A
                T32Gbl_avPllCtl9 regData9;

                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), &regData9.u32);

                if(avpll_inited[0] == 0) {
                        ppm0 = offset_2_ppm(regData9.uavPllCtl_avPllFreqOffsetC8_A);
                        cur_ppm[0] = org_ppm[0] = ppm0;
                        avpll_inited[0] = 1;
                }else{
                        ppm0 = org_ppm[0];
                }

                cur_ppm[0] += ppm_delta;
                ppm = cur_ppm[0];

                regData9.uavPllCtl_avPllFreqOffsetC8_A = ppm_2_offset(ppm);
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), regData9.u32);

                regData9.uavPllCtl_avPllFreqOffsetReadyC_A = 0x80;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), regData9.u32);

                for(i=0; i<10000; i++);

                regData9.uavPllCtl_avPllFreqOffsetReadyC_A = 0;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), regData9.u32);
        } else { // For AVPLL-B
                T32Gbl_avPllCtl40 regData40;

                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl40), &regData40.u32);

                if(avpll_inited[1] == 0) {
                        ppm0 = offset_2_ppm(regData40.uavPllCtl_avPllFreqOffsetC8_B);
                        cur_ppm[1] = org_ppm[1] = ppm0;
                        avpll_inited[1] = 1;
                }else{
                        ppm0 = org_ppm[1];
                }

                cur_ppm[1] += ppm_delta;
                ppm = cur_ppm[1];

                regData40.uavPllCtl_avPllFreqOffsetC8_B = ppm_2_offset(ppm);
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl40), regData40.u32);

                regData40.uavPllCtl_avPllFreqOffsetReadyC_B = 0x80;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl40), regData40.u32);

                for(i=0; i<10000; i++);

                regData40.uavPllCtl_avPllFreqOffsetReadyC_B = 0;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl40), regData40.u32);
        }

        printf("-- cur_ppm: %.1lf org_ppm: %.1lf\n", ppm, ppm0);
        return ppm - ppm0;
}
#endif
#else
#define AVPLPRINTF(x)

#define BFM_HOST_Bus_Write32 	GA_REG_WORD32_WRITE
#define	BFM_HOST_Bus_Read32		GA_REG_WORD32_READ


#define	CHIP_CTRL_REG_BASE			(0x00 + MEMMAP_CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_SW_GENERIC0_REG   (RA_Gbl_sw_generic0 + CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_SW_GENERIC1_REG   (RA_Gbl_sw_generic1 + CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_SW_GENERIC2_REG   (RA_Gbl_sw_generic2 + CHIP_CTRL_REG_BASE)
#define CHIP_CTRL_SW_GENERIC3_REG   (RA_Gbl_sw_generic3 + CHIP_CTRL_REG_BASE)
#ifdef BERLIN_SINGLE_CPU
#define reg(addr)   				(*((int*)(devmem_phy_to_virt(addr))))
#else
#define reg(addr)   				(*((int*)(addr)))
#endif
#define	PRE_DEFINED_AVPLL_SET_FLAG	(0xBF0A0001)

//using 2227.5Mhz input
double avFreqs1[]=
{
74.25,                  // 0
148.5,                  // 1
24.75,                  // 2
8.25,                   // 3
6.144,                  // 4
12.288,                 // 5
24.576,                 // 6
49.152,                 // 7
98.304,                 // 8
74.17582,               // 9
148.3516,               // 10
8.4672,                 // 11
11.2896,                // 12
16.9344,                // 13
22.5792,                // 14
33.8688,                // 15
8.192,                  // 16
12.288,                 // 17
16.384,                 // 18
24.576,                 // 19
9.216,                  // 20
12.288,                 // 21
18.432,                 // 22
36.864,                 // 23
741.76,                 // 24
112.64,                 // 25
1483.52,                // 26
2225.27,                // 27
742.50,                 // 28
1113.75,                // 29
1485.00,                // 30
2227.50,                // 31
148.35,                 // 32
296.70,                 // 33
148.50,                 // 34
297.00,                 // 35
//video frequencies
27,                     // 36 1620MHz 8bit and 12bit    or 1350MHz 10bit
54,                     // 37 1620MHz 8bit and 12bit    or 1350MHz 10bit
108,                    // 38 1620MHz 8bit and 12bit    or 1350MHz 10bit, not working on B0
27.027,                 // 39 1620MHz 8bit and 12bit    or 1350MHz 10bit
54.054,                 // 40 1620MHz 8bit and 12bit    or 1350MHz 10bit
108.108,                // 41 1620MHz 8bit and 12bit    or 1350MHz 10bit, not working on B0

74.25,                  // 42 2225.274725MHz 8bit and 12bit   or 1856.25MHz 10bit
148.5,                  // 43 2225.274725MHz 8bit and 12bit   or 1856.25MHz 10bit
74.17582418,            // 44 2225.274725MHz 8bit and 12bit   or 1856.25MHz 10bit
148.3516484,            // 45 2225.274725MHz 8bit and 12bit   or 1856.25MHz 10bit

25.175,                 // 46 1510.5MHz 8bit and 12bit        or 1258.75MHz 10bit
25.200175,              // 47 1510.5MHz 8bit and 12bit        or 1258.75MHz 10bit
};

//using 1620Mhz input
double avFreqs2[]=
{
270.000,
405.000,
540.000,
810.000,
1080.000,
1620.000,
270.270,
405.405,
540.540,
810.810,
1081.080,
1621.620,
54.000,
108.000,
216.000,
108.000,
216.000,
216.000,
54.054,
108.108,
216.216,
108.108,
216.216,
216.216
};

//using 1510Mhz input
double avFreqs3[]=
{
251.75,
377.625,
252.0018,
378.0026,
50.35,
100.7,
201.4,
50.40035,
100.8007,
201.6014
};

//Desired AV clock freq in MHz
double avFreqs[]=
{
8.467200      ,
11.289600     ,
16.934400     ,
22.579200     ,
33.868800     ,
6.144000      ,
8.192000      ,
12.288000     ,
16.384000     ,
24.576000     ,
9.216000      ,
12.288000     ,
18.432000     ,
24.576000     ,
36.864000
};


#define DIV2_BY_1               0
#define DIV2_BY_1P5             1
#define DIV2_BY_2               2
#define DIV2_BY_3               3
#define DIV2_BY_7P5             4
#define DIV2_BY_12P5            5
#define DIV2_BY_15              6
#define DIV2_BY_25              7
#define ENABLE_DIV2             (1<<3)

int pll_A_VCO_Setting = -1;
int pll_B_VCO_Setting = -1;

//VCO clock freq in MHz
double refFreqs[]=
{
        1258.75,
        1350,
        1510.5,
        1620,   //default PLL_B
        1854.395604,
        2225.274725,
};
unsigned int avpllRegV2I[]=
{
        0x4,    //1258.75MHz
        0x4,    //1350MHz
        0x3,    //1510.5MHz
        0x4,    //1620MHz
        0x4,    //1854.395604MHz
        0x4,    //2225.274725MHz
};

unsigned int avpllRegSPEED[]=
{
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
        0x1,    //1258.75MHz, 10 bit mode
        0x1,    //1350MHz,        10 bit mode
        0x0,    //1510.5MHz,  8 or 12 bit mode
        0x0,    //1620MHz,    8 or 12 bit mode
        0x3,    //1854.395604MHz
        0x2,    //2225.274725MHz
#else
        0x0,    //1258.75MHz, 10 bit mode
        0x0,    //1350MHz,        10 bit mode
        0x0,    //1510.5MHz,  8 or 12 bit mode
        0x0,    //1620MHz,    8 or 12 bit mode
        0x1,    //1854.395604MHz
        0x1,    //2225.274725MH
#endif
};

unsigned int avpllRegINTPI[]=
{
        0x2,    //1258.75MHz
        0x2,    //1350MHz
        0x2,    //1510.5MHz
        0x2,    //1620MHz
        0x3,    //1854.395604MHz
        0x3,    //2225.274725MHz
};

char avpllRegFBDIV[]=
{
        50,     //1258.75MHz
        54,     //1350MHz
        60,     //1510.5MHz
        65,     //1620MHz
        74,     //1854.395604MHz
        89,     //2225.274725MHz
};

short avpllRegFREQ_OFFSET_8[]=
{
        3670,   //1258.75MHz
        0,              //1350MHz
        3670,   //1510.5MHz
        0x864d, //1620MHz, this is -1613 and it is not two's complement
        1246,   //1854.395604MHz
        65,             //2225.274725MHz
};


void resetAvPll_A()
{
        volatile int i;

        T32Gbl_avPllCtl reg;

        //assert reset
        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), &reg.u32);
        reg.uavPllCtl_avPllReset_Pll_A=1;
        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), reg.u32);

        for(i=0; i<1000; i++);

        //deassert reset
        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), &reg.u32);
        reg.uavPllCtl_avPllReset_Pll_A=0;
        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), reg.u32);

        //dbg_printf(PRN_RES, "reset AVPLL A\n");
}

void resetAvPll_B()
{
        volatile int i;

        T32Gbl_avPllCtl reg;

        //assert reset
        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), &reg.u32);
        reg.uavPllCtl_avPllReset_Pll_B=1;
        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), reg.u32);

        for(i=0; i<1000; i++);

        //deassert reset
        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), &reg.u32);
        reg.uavPllCtl_avPllReset_Pll_B=0;
        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), reg.u32);

        //dbg_printf(PRN_RES, "reset AVPLL B\n");
}

#if 1 // PCR_TRACKING
static int avpll_inited[2] = {0, 0};
static double org_ppm[2] = {0.0, 0.0};
static double cur_ppm[2] = {0.0, 0.0};

static double offset_2_ppm(int offset) {
#if 0 // I don't know how this formular come from, it shall be the following one -- Yongchun
	int v = offset & 0x7fff;

	if(offset & 0x8000) v = -v;

	return (1000000.0 * offset) / (524288 - offset);
#else
	int v = offset & 0x7fff;

	if(offset & 0x8000) v = -v;

	return ((1000000.0 * v)/(524288.0 + v));
#endif
}

static int ppm_2_offset(double ppm) {
#if 0
	double c = 524288.0 * ppm / (1000000.0 + ppm);
#else
	double c = 524288.0 * ppm / (1000000.0 + ppm);
#endif
	int offset = (int)c;

	if(offset > 32767) {
		offset = 32767;
	}else if(offset < -32767) {
		offset = -32767;
	}
	return (offset < 0) ? ((-offset) | 0x8000) : offset;
}


void avpll_get_ppm(int grp, double *ppm_base, double *ppm_now)
{
	double ppm0, ppm;
	if(grp ==0)
	{
		ppm0 = org_ppm[0];
		ppm = cur_ppm[0];
	}
	else // AVPLL B
	{
		ppm0 = org_ppm[1];
		ppm = cur_ppm[1];
	}
	if(ppm_base) *ppm_base = ppm0;
	if(ppm_now) *ppm_now = ppm;
}

double avpll_adjust_ppm(double ppm_delta, int grp) {
	double ppm0, ppm;

	if(grp == 0) {
		T32Gbl_avPllCtl7 regData7;

		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl7), &regData7.u32);

		if(avpll_inited[0] == 0) {
			ppm0 = offset_2_ppm(regData7.uavPllCtl_avPllFreq_Offset_A8);
			cur_ppm[0] = org_ppm[0] = ppm0;
			avpll_inited[0] = 1;
		}else{
			ppm0 = org_ppm[0];
		}

		cur_ppm[0] += ppm_delta;
		ppm = cur_ppm[0];

		regData7.uavPllCtl_avPllFreq_Offset_A8 = ppm_2_offset(ppm);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl7), regData7.u32);
	}else{
		T32Gbl_avPllCtl11 regData11;

		if(avpll_inited[1] == 0) {
			ppm0 = offset_2_ppm(regData11.uavPllCtl_avPllFreq_Offset_B8);
			cur_ppm[1] = org_ppm[1] = ppm0;
			avpll_inited[1] = 1;
		}else{
			ppm0 = org_ppm[1];
		}

		cur_ppm[1] += ppm_delta;
		ppm = cur_ppm[1];

		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), &regData11.u32);
		regData11.uavPllCtl_avPllFreq_Offset_B8 = ppm_2_offset(ppm);
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), regData11.u32);
	}

	//printf("-- cur_ppm: %.1lf org_ppm: %.1lf\n", ppm, ppm0);
	return ppm - ppm0;
}
#endif

/* new reference in MHz, chan : chan_A 0, Chan_B 1 */
/* new reference in MHz, chan : chan_A 0, Chan_B 1 */
static void changeRefFreq(int vco_freq_index, int grp)
{
        T32Gbl_avPllCtl reg;
        T32Gbl_avPllCtl1 regData1;
        T32Gbl_avPllCtl2 regData2;
        T32Gbl_avPllCtl7 regData7;
        T32Gbl_avPllCtl11 regData11;
		unsigned int    uiBootflow_Flag ;


        //dbg_printf(PRN_RES, "PLL grp %d VCO freq change to %f\n", grp, refFreqs[vco_freq_index]);
        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), &reg.u32);
        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl1), &regData1.u32);
        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl2), &regData2.u32);
        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl7), &regData7.u32);
        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), &regData11.u32);
        if(grp ==0)
        {/* chan A */
                regData1.uavPllCtl_avPllFbDiv_A = avpllRegFBDIV[vco_freq_index];

                reg.uavPllCtl_avPllSpeed_A = avpllRegSPEED[vco_freq_index];
                regData2.uavPllCtl_avPllIntpi_A =avpllRegINTPI[vco_freq_index];
                regData1.uavPllCtl_avPllSelV2i_A = avpllRegV2I[vco_freq_index];
                regData7.uavPllCtl_avPllFreq_Offset_A8 = avpllRegFREQ_OFFSET_8[vco_freq_index];
                pll_A_VCO_Setting = vco_freq_index;
        }
        else
        {/* chan B */
                regData1.uavPllCtl_avPllFbDiv_B = avpllRegFBDIV[vco_freq_index];

                reg.uavPllCtl_avPllSpeed_B = avpllRegSPEED[vco_freq_index];
                regData2.uavPllCtl_avPllIntpi_B =avpllRegINTPI[vco_freq_index];
                regData1.uavPllCtl_avPllSelV2i_B = avpllRegV2I[vco_freq_index];
                regData11.uavPllCtl_avPllFreq_Offset_B8 = avpllRegFREQ_OFFSET_8[vco_freq_index];
                pll_B_VCO_Setting = vco_freq_index;
        }
        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), reg.u32);
        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl1), regData1.u32);
        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl2), regData2.u32);
        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl7), regData7.u32);
        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), regData11.u32);

        if(grp ==0)
        {
                resetAvPll_A();
        }
        else
        {
			uiBootflow_Flag = reg(CHIP_CTRL_SW_GENERIC0_REG) ;

			//	if boot flow set AVPLL,	this function does not set AVPLL again
			if(uiBootflow_Flag == PRE_DEFINED_AVPLL_SET_FLAG){

				printf("%s:		------------ ATTENTION !!!!!	----------\n", __FUNCTION__) ;
				printf("%s:		AVPLL_B is reset, This is very dangeous   \n", __FUNCTION__) ;
				printf("%s:		Please report this to xfgao@marvell.com   \n", __FUNCTION__) ;
				printf("%s: 	------------ ATTENTION !!!!!    ----------\n", __FUNCTION__) ;
			}
			resetAvPll_B();

        }
}

//this function return 1 if abs(val1-val2)/val2<5ppm
//assuming val1 and val2 are positive numbers
int lessThan5PPM(double val1, double val2)
{
        double temp=(val1-val2)/val2;

        return (temp<0.000005 && temp>-0.000005);
}

//this function will calculate ppm1K, postDiv and postDiv2 values to achiev clkAv frequency
//given the clkIn reference frequency. It will not use ppmOffset but rely on ppm1K only
int calcPPM1kAVPLL(double clkIn, double clkAv, int ppm1k_en, int *ppm1K, int *postDiv, int *postDiv2)
{
        double ratio=clkIn/clkAv;
        double tempClkIn, finalClkAv, div2;
        int       iRatio;

        *postDiv2=0;
        *postDiv=0;
        *ppm1K=0;

        //first see if ppm1K is needed
//        if(!lessThan5PPM((1.5*clkAv), clkIn) && !lessThan5PPM((7.5*clkAv), clkIn) && !lessThan5PPM((((int)(ratio+0.5))*clkAv), clkIn))
        if (ppm1k_en)
        {
                //cannot divide, assuming turn on ppm1K will help
                ratio = clkIn*1.001/clkAv;

                *ppm1K=1;
        }

        if(*ppm1K)
                tempClkIn = clkIn*1.001;
        else
                tempClkIn = clkIn;

        if(lessThan5PPM(clkAv, tempClkIn))
        {
                *postDiv2=(ENABLE_DIV2|DIV2_BY_1);
                div2=1;
        }
        else if(lessThan5PPM((1.5*clkAv), tempClkIn))
        {
                *postDiv2=(ENABLE_DIV2|DIV2_BY_1P5);
                div2=1.5;
        }
        else if( lessThan5PPM((2*clkAv), tempClkIn))
        {
                *postDiv2=(ENABLE_DIV2|DIV2_BY_2);
                div2=2;
        }
        else if( lessThan5PPM((3*clkAv), tempClkIn))
        {
                *postDiv2=(ENABLE_DIV2|DIV2_BY_3);
                div2=3;
        }
        else if(lessThan5PPM((7.5*clkAv), tempClkIn))
        {
                *postDiv2=(ENABLE_DIV2|DIV2_BY_7P5);
                div2=7.5;
        }
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
        else if(lessThan5PPM((12.5*clkAv), tempClkIn))
        {
                *postDiv2=(ENABLE_DIV2|DIV2_BY_12P5);
                div2=12.5;
        }
#endif
        else if(lessThan5PPM((15*clkAv), tempClkIn))
        {
                *postDiv2=(ENABLE_DIV2|DIV2_BY_15);
                div2=15;
        }
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
        else if(lessThan5PPM((25*clkAv), tempClkIn))
        {
                *postDiv2=(ENABLE_DIV2|DIV2_BY_25);
                div2=25;
        }
#endif
        else
        {
                iRatio = (int)(ratio+0.5);

                //postDiv only takes even number
                if((iRatio & 0x1) || (iRatio<4 || iRatio>511))
                {
                        //dbg_printf(PRN_RES, "Failed to find DIV_XX: Ratio=%f, iRatio=%d\n", ratio, iRatio);
                        return 1;
                }

                *postDiv = iRatio;
        }

        if(*postDiv2)
                finalClkAv = tempClkIn/div2;
        else
                finalClkAv = tempClkIn/iRatio;

        //dbg_printf(PRN_RES, "final clkAv=%3.8f\n", finalClkAv);

        if(!lessThan5PPM(finalClkAv, clkAv))
        {
                //dbg_printf(PRN_RES, "Failed to find right frequency\n");
                return 1;
        }

        return 0;
}

//this function will calculate ppmOffset, postDiv and postDiv2 values to achiev clkAv frequencyn
//given the clkIn reference frequency. It will not use ppm1K but rely on ppmOffset only
int calcAVPLL(double clkIn, double clkAv, int *ppmOffset, int *postDiv, int *postDiv2)
{
        double ratio=clkIn/clkAv;
        int       iRatio;
        double clkOut;
        double coeff;

        //first see if DIV2 is needed
        iRatio = (int)(ratio*2+0.5);

        *postDiv2=0;
        *postDiv=0;
        *ppmOffset=0;

        if(iRatio==2)
        {
                *postDiv2=(ENABLE_DIV2|DIV2_BY_1);

        }
        else if(iRatio==3)
        {
                *postDiv2=(ENABLE_DIV2|DIV2_BY_1P5);
        }
        else if( iRatio==4)
        {
                *postDiv2=(ENABLE_DIV2|DIV2_BY_2);
        }
        else if( iRatio==6)
        {
                *postDiv2=(ENABLE_DIV2|DIV2_BY_3);
        }
        else if(iRatio==15)
        {
                *postDiv2=(ENABLE_DIV2|DIV2_BY_7P5);
        }
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
        else if(iRatio==25)
        {
                *postDiv2=(ENABLE_DIV2|DIV2_BY_12P5);
        }
#endif
        else if(iRatio==30)
        {
                *postDiv2=(ENABLE_DIV2|DIV2_BY_15);
        }
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
        else if(iRatio==50)
        {
                *postDiv2=(ENABLE_DIV2|DIV2_BY_25);
        }
#endif

        if(*postDiv2)
        {
                //for computing freq offset
                clkOut = clkAv*iRatio/2;

                if(iRatio==ratio*2)
                {
                        //divided evenly, no need for offset
                        return 0;
                }
        }
        else
        {
                //round down first
                iRatio=(int)(ratio);

                //postDiv only takes even number
                if(iRatio & 0x1) iRatio++;

                if(iRatio<4 || iRatio>511)
                {
                        //dbg_printf(PRN_RES, "Failed to find DIV_XX: Ratio=%f, iRatio=%d\n", ratio, iRatio);
                        return 1;
                }

                clkOut = clkAv*iRatio;

                *postDiv = iRatio;

                if(iRatio==ratio)
                {

                       //divided evenly, no need for offset
                        return 0;
                }
        }
        //compute freq offset
        coeff = -(clkOut - clkIn)*128/clkOut;

        if(coeff>0)
                *ppmOffset = (int)(coeff*4096+0.5);
        else
                *ppmOffset = (int)(coeff*4096-0.5);

        //ppmOffset can have 15 bits max
        if(*ppmOffset==0 || *ppmOffset>32767 || *ppmOffset<-32767)
        {
                //dbg_printf(PRN_RES, "Failed to find ppmOffset: Ratio=%f, iRatio=%d, coeff=%f\n", ratio, iRatio, coeff);
                return 1;
        }

        return 0;
}

void AVPLL_EnableChannel(int avpll, int id, int on)
{
        T32Gbl_avPllCtl12 regData12;

	    //Power up all 7 channels
    	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), &regData12.u32);
        if (avpll == 0){ // AVPLL-A
            if (on)
                regData12.uavPllCtl_avPllPU_A |= (1<<id);
            else
                regData12.uavPllCtl_avPllPU_A &= ~(1<<id);
        } else {
            if (on)
                regData12.uavPllCtl_avPllPU_B |= (1<<id);
            else
                regData12.uavPllCtl_avPllPU_B &= ~(1<<id);
        }
    	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), regData12.u32);
}

//this function will enable AVPLL A and B and turn on caliberation
void AVPLL_Enable(void)
{
        T32Gbl_avPllCtl12 regData12;
        T32Gbl_avPllCtl1 regData1;
        T32Gbl_avPllCtl regData;
        T32Gbl_clkEnable  regClkEnable;
        T32Gbl_PadSelect  regPadSelect;
        volatile int i;
		unsigned int	uiBootflow_Flag ;

		uiBootflow_Flag = reg(CHIP_CTRL_SW_GENERIC0_REG) ;
		printf("%s:	CHIP_CTRL_SW_GENERIC0_REG=0x%x, uiBootflow_Flag=0x%x\n",__FUNCTION__, CHIP_CTRL_SW_GENERIC0_REG, uiBootflow_Flag) ;
		//	if boot flow set AVPLL,	this function does not set AVPLL again

		if(uiBootflow_Flag != PRE_DEFINED_AVPLL_SET_FLAG){

			printf("%s:	AVPLL has not been set by boot flow, uiBootflow_Flag=0x%x\n",__FUNCTION__, uiBootflow_Flag) ;

			// Set default value for Reference, C0 start reference is wrong, otherwise calibration wrong
			changeRefFreq(AVPLL_VCO_FREQ_2_225G, 0);
			changeRefFreq(AVPLL_VCO_FREQ_1_620G, 1);

	        //toggle PU and PU_PLL_B
    	    BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), &regData.u32);
        	regData.uavPllCtl_avPllPu=0;
	        regData.uavPllCtl_avPllPu_Pll_B=0;
    	    BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), regData.u32);

        	//turn off caliberation
	        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl1), &regData1.u32);
    	    regData1.uavPllCtl_avPllCalStart=0;
        	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl1), regData1.u32);
	        for(i=0; i<1000; i++);

    	    //Power up PU and PU_PLL_B
        	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), &regData.u32);
	        regData.uavPllCtl_avPllPu=1;
    	    regData.uavPllCtl_avPllPu_Pll_B=1;
        	BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), regData.u32);

	        //Power up all 7 channels
    	    BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), &regData12.u32);
        	regData12.uavPllCtl_avPllPU_A = 0x7f;
	        regData12.uavPllCtl_avPllPU_B = 0x7f;
    	    BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), regData12.u32);

        	for(i=0; i<1000; i++);

	        resetAvPll_A();
    	    resetAvPll_B();

	        for(i=0; i<10000; i++);
    	    //enable caliberation
        	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl1), &regData1.u32);
	        regData1.uavPllCtl_avPllCalStart=1;
    	    BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl1), regData1.u32);

        	//dbg_printf(PRN_RES, "Enable AvPLL A and B Channels\n");

		}else{
			printf("%s:	AVPLL has been set by boot flow\n", __FUNCTION__) ;
			//	init global value
			pll_A_VCO_Setting = AVPLL_VCO_FREQ_2_225G ;
			pll_B_VCO_Setting = AVPLL_VCO_FREQ_1_620G ;
		}

   	    //dbg_printf(PRN_RES, "Enable DVIO Pad\n");
       	BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_PadSelect), &regPadSelect.u32);
        regPadSelect.uPadSelect_DVIO_OEN = 1;
   	    BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_PadSelect), regPadSelect.u32);

 		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkEnable), &regClkEnable.u32);
		regClkEnable.uclkEnable_audio0ClkEn = 1;
		regClkEnable.uclkEnable_audio1ClkEn = 1;
		// because of HW bug, we had to enable MIC AIO and DHUB first then turn on MIC MCLK.
//        regClkEnable.uclkEnable_audio2ClkEn = 1;
  	    regClkEnable.uclkEnable_audio3ClkEn = 1;
#if (BERLIN_CHIP_VERSION >= BERLIN_B_0)
		regClkEnable.uclkEnable_audioHdClkEn = 1;
#endif
        regClkEnable.uclkEnable_video0ClkEn = 1;
   	    regClkEnable.uclkEnable_video1ClkEn = 1;
       	regClkEnable.uclkEnable_video2ClkEn = 1;
        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkEnable), regClkEnable.u32);


#if (BERLIN_CHIP_VERSION >= BERLIN_B_0)
		// Set charge pump current
		BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), &regData.u32);
#if (BERLIN_CHIP_VERSION >= BERLIN_C_0)
		regData.uavPllCtl_avPllIcp_A = 0;
#else
		regData.uavPllCtl_avPllIcp_A = 0x02;
#endif
		BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl), regData.u32);
#endif // (BERLIN_CHIP_VERSION >= BERLIN_B_0)

}

void AVPLL_EnableMicClk(int en)
{
        T32Gbl_clkEnable  regClkEnable;
        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkEnable), &regClkEnable.u32);
		if (en) {
			en = 1;
		}
        regClkEnable.uclkEnable_audio2ClkEn = en;
        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_clkEnable), regClkEnable.u32);
}

void set1KPPM_A(int chId, int enable)
{
        T32Gbl_avPllCtl2 reg;

        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl2), &reg.u32);
        reg.uavPllCtl_avPll1000PPM_En_A &= ~(1<<(7-chId));
        reg.uavPllCtl_avPll1000PPM_En_A |= (enable<<(7-chId));
        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl2), reg.u32);

        //toggle the offset_rdy bit
        {
                volatile int i;

                T32Gbl_avPllCtl11 regData;
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), &regData.u32);
                regData.uavPllCtl_avPllFreq_Offset_Rdy_A = (1<<(7-chId));
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), regData.u32);
                //add some delay because need to be asserted by 30ns
                for(i=0; i<1000; i++);

                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), &regData.u32);
                regData.uavPllCtl_avPllFreq_Offset_Rdy_A = 0;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), regData.u32);
        }

}

void set1KPPM_B(int chId, int enable)
{
        T32Gbl_avPllCtl3 reg;
        T32Gbl_avPllCtl11 regData;

        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl3), &reg.u32);
        reg.uavPllCtl_avPll1000PPM_En_B &= ~(1 << (7-chId));
        reg.uavPllCtl_avPll1000PPM_En_B |= (enable << (7-chId));
        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl3), reg.u32);

        {
                volatile int i;

                //toggle the offset_rdy bit
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), &regData.u32);
                regData.uavPllCtl_avPllFreq_Offset_Rdy_B = (1 << (7-chId));
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), regData.u32);

                //add some delay because need to be asserted by 30ns
                for(i = 0; i < 1000; i++);

                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), &regData.u32);
                regData.uavPllCtl_avPllFreq_Offset_Rdy_B = 0;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), regData.u32);
        }
}

#define DIV2_ENABLE_MASK (1<<3)

int disableAvPll_A_Div2(int chId)
{
        T32Gbl_avPllCtl16 regData16;
        T32Gbl_avPllCtl17 regData17;

        if (chId==0)
        {
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl16), &regData16.u32);
                regData16.uavPllCtl_avPllDiv2_A1 &= ~DIV2_ENABLE_MASK;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl16), regData16.u32);

                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), &regData17.u32);
                regData17.uavPllCtl_avPllDiv2_A2 &= ~DIV2_ENABLE_MASK;
                regData17.uavPllCtl_avPllDiv2_A3 &= ~DIV2_ENABLE_MASK;
                regData17.uavPllCtl_avPllDiv2_A4 &= ~DIV2_ENABLE_MASK;
                regData17.uavPllCtl_avPllDiv2_A5 &= ~DIV2_ENABLE_MASK;
                regData17.uavPllCtl_avPllDiv2_A6 &= ~DIV2_ENABLE_MASK;
                regData17.uavPllCtl_avPllDiv2_A7 &= ~DIV2_ENABLE_MASK;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), regData17.u32);

        }
        else if (chId==1)
        {
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl16), &regData16.u32);
                regData16.uavPllCtl_avPllDiv2_A1 &= ~DIV2_ENABLE_MASK;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl16), regData16.u32);
        }
        else
        {
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), &regData17.u32);
                if(chId==2)
                        regData17.uavPllCtl_avPllDiv2_A2 &= ~DIV2_ENABLE_MASK;
                else if(chId==3)
                        regData17.uavPllCtl_avPllDiv2_A3 &= ~DIV2_ENABLE_MASK;
                else if(chId==4)
                        regData17.uavPllCtl_avPllDiv2_A4 &= ~DIV2_ENABLE_MASK;
                else if(chId==5)
                        regData17.uavPllCtl_avPllDiv2_A5 &= ~DIV2_ENABLE_MASK;
                else if(chId==6)
                        regData17.uavPllCtl_avPllDiv2_A6 &= ~DIV2_ENABLE_MASK;
                else if(chId==7)
                        regData17.uavPllCtl_avPllDiv2_A7 &= ~DIV2_ENABLE_MASK;
                else
                {
                        //dbg_printf(PRN_RES, "Bad Channel ID %d\n", chId);
                        return 1;
                }
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), regData17.u32);
        }

        return 0;
}

//in div2, the div2 enable bit should already be set
int setAvPll_A_Div2(int div2, int chId)
{
        T32Gbl_avPllCtl16 regData16;
        T32Gbl_avPllCtl17 regData17;

        if (chId==0)
        {
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl16), &regData16.u32);
                regData16.uavPllCtl_avPllDiv2_A1 = div2;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl16), regData16.u32);

                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), &regData17.u32);
                regData17.uavPllCtl_avPllDiv2_A2 = div2;
                regData17.uavPllCtl_avPllDiv2_A3 = div2;
                regData17.uavPllCtl_avPllDiv2_A4 = div2;
                regData17.uavPllCtl_avPllDiv2_A5 = div2;
                regData17.uavPllCtl_avPllDiv2_A6 = div2;
                regData17.uavPllCtl_avPllDiv2_A7 = div2;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), regData17.u32);

        }
        else if (chId==1)
        {
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl16), &regData16.u32);
                regData16.uavPllCtl_avPllDiv2_A1 = div2;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl16), regData16.u32);
        }
        else
        {
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), &regData17.u32);
                if(chId==2)
                        regData17.uavPllCtl_avPllDiv2_A2 = div2;
                else if(chId==3)
                        regData17.uavPllCtl_avPllDiv2_A3 = div2;
                else if(chId==4)
                        regData17.uavPllCtl_avPllDiv2_A4 = div2;
                else if(chId==5)
                        regData17.uavPllCtl_avPllDiv2_A5 = div2;
                else if(chId==6)
                        regData17.uavPllCtl_avPllDiv2_A6 = div2;
                else if(chId==7)
                        regData17.uavPllCtl_avPllDiv2_A7 = div2;
                else
                {
                        //dbg_printf(PRN_RES, "Bad Channel ID %d\n", chId);
                        return 1;
                }
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), regData17.u32);
        }

        return 0;
}

int disableAvPll_B_Div2(int chId)
{
        T32Gbl_avPllCtl17 regData17;
        T32Gbl_avPllCtl18 regData18;

        if (chId==0)
        {
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), &regData17.u32);
                regData17.uavPllCtl_avPllDiv2_B1 &= ~DIV2_ENABLE_MASK;
                regData17.uavPllCtl_avPllDiv2_B2 &= ~DIV2_ENABLE_MASK;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), regData17.u32);

                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl18), &regData18.u32);
                regData18.uavPllCtl_avPllDiv2_B3 &= ~DIV2_ENABLE_MASK;
                regData18.uavPllCtl_avPllDiv2_B4 &= ~DIV2_ENABLE_MASK;
                regData18.uavPllCtl_avPllDiv2_B5 &= ~DIV2_ENABLE_MASK;
                regData18.uavPllCtl_avPllDiv2_B6 &= ~DIV2_ENABLE_MASK;
                regData18.uavPllCtl_avPllDiv2_B7 &= ~DIV2_ENABLE_MASK;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl18), regData18.u32);

        }
        else if (chId==1)
        {
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), &regData17.u32);
                regData17.uavPllCtl_avPllDiv2_B1 &= ~DIV2_ENABLE_MASK;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), regData17.u32);
        }
        else if (chId==2)
        {
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), &regData17.u32);
                regData17.uavPllCtl_avPllDiv2_B2 &= ~DIV2_ENABLE_MASK;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), regData17.u32);
        }
        else
        {
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl18), &regData18.u32);
                if(chId==3)
                        regData18.uavPllCtl_avPllDiv2_B3 &= ~DIV2_ENABLE_MASK;
                else if(chId==4)
                        regData18.uavPllCtl_avPllDiv2_B4 &= ~DIV2_ENABLE_MASK;
                else if(chId==5)
                        regData18.uavPllCtl_avPllDiv2_B5 &= ~DIV2_ENABLE_MASK;
                else if(chId==6)
                        regData18.uavPllCtl_avPllDiv2_B6 &= ~DIV2_ENABLE_MASK;
                else if(chId==7)
                        regData18.uavPllCtl_avPllDiv2_B7 &= ~DIV2_ENABLE_MASK;
                else
                {
                        //dbg_printf(PRN_RES, "Bad Channel ID %d\n", chId);
                        return 1;
                }
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl18), regData18.u32);
        }

        return 0;
}

//in div2, the div2 enable bit should be set
int setAvPll_B_Div2(int div2, int chId)
{
        T32Gbl_avPllCtl17 regData17;
        T32Gbl_avPllCtl18 regData18;

        if (chId==0)
        {
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), &regData17.u32);
                regData17.uavPllCtl_avPllDiv2_B1 = div2;
                regData17.uavPllCtl_avPllDiv2_B2 = div2;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), regData17.u32);

                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl18), &regData18.u32);
                regData18.uavPllCtl_avPllDiv2_B3 = div2;
                regData18.uavPllCtl_avPllDiv2_B4 = div2;
                regData18.uavPllCtl_avPllDiv2_B5 = div2;
                regData18.uavPllCtl_avPllDiv2_B6 = div2;
                regData18.uavPllCtl_avPllDiv2_B7 = div2;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl18), regData18.u32);

        }
        else if (chId==1)
        {
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), &regData17.u32);
                regData17.uavPllCtl_avPllDiv2_B1 = div2;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), regData17.u32);
        }
        else if (chId==2)
        {
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), &regData17.u32);
                regData17.uavPllCtl_avPllDiv2_B2 = div2;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl17), regData17.u32);
        }
        else
        {
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl18), &regData18.u32);
                if(chId==3)
                        regData18.uavPllCtl_avPllDiv2_B3 = div2;
                else if(chId==4)
                        regData18.uavPllCtl_avPllDiv2_B4 = div2;
                else if(chId==5)
                        regData18.uavPllCtl_avPllDiv2_B5 = div2;
                else if(chId==6)
                        regData18.uavPllCtl_avPllDiv2_B6 = div2;
                else if(chId==7)
                        regData18.uavPllCtl_avPllDiv2_B7 = div2;
                else
                {
                        //dbg_printf(PRN_RES, "Bad Channel ID %d\n", chId);
                        return 1;
                }
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl18), regData18.u32);
        }

        return 0;
}

int setAvPll_A_Div(int div, int chId)
{

        T32Gbl_avPllCtl12 regData12;
        T32Gbl_avPllCtl13 regData13;
        T32Gbl_avPllCtl14 regData14;

        if(disableAvPll_A_Div2(chId))
                return 1;

        switch (chId)
        {
                case 0:
                {
                        //dbg_printf(PRN_RES, " setting value of div_a for all channels to 0x%x\n", div);
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), &regData12.u32);
                        regData12.uavPllCtl_avPllDiv_A1 = div;
                        regData12.uavPllCtl_avPllDiv_A2 = div;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), regData12.u32);

                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl13), &regData13.u32);
                        regData13.uavPllCtl_avPllDiv_A3 = div;
                        regData13.uavPllCtl_avPllDiv_A4 = div;
                        regData13.uavPllCtl_avPllDiv_A5 = div;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl13), regData13.u32);

                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl14), &regData14.u32);
                        regData14.uavPllCtl_avPllDiv_A6 = div;
                        regData14.uavPllCtl_avPllDiv_A7 = div;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl14), regData14.u32);
                        break;
                };
                case 1:
                case 2:
                {
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), &regData12.u32);
                        if(chId==1)
                                regData12.uavPllCtl_avPllDiv_A1 = div;
                        else
                                regData12.uavPllCtl_avPllDiv_A2 = div;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl12), regData12.u32);
                        break;
                }
                case 3:
                case 4:
                case 5:
                {
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl13), &regData13.u32);
                        if(chId==3)
                                regData13.uavPllCtl_avPllDiv_A3 = div;
                        else if(chId==4)
                                regData13.uavPllCtl_avPllDiv_A4 = div;
                        else
                                regData13.uavPllCtl_avPllDiv_A5 = div;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl13), regData13.u32);
                        break;
                }
                case 6:
                case 7:
                {
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl14), &regData14.u32);
                        if(chId==6)
                                regData14.uavPllCtl_avPllDiv_A6 = div;
                        else
                                regData14.uavPllCtl_avPllDiv_A7 = div;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl14), regData14.u32);
                        break;
                }
                default:
                {
                        //dbg_printf(PRN_RES, "bad chan ID\n");
                        return 1;
                }
        }

        return 0;
}

int setAvPll_B_Div(int div, int chId)
{
        T32Gbl_avPllCtl14 regData14;
        T32Gbl_avPllCtl15 regData15;
        T32Gbl_avPllCtl16 regData16;

        if(disableAvPll_B_Div2(chId))
                return 1;

        switch (chId)
        {
                case 0:
                {
                        //dbg_printf(PRN_RES, " setting value of div_b for all channels to 0x%x\n", div);
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl14), &regData14.u32);
                        regData14.uavPllCtl_avPllDiv_B1 = div;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl14), regData14.u32);

                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl15), &regData15.u32);
                        regData15.uavPllCtl_avPllDiv_B2 = div;
                        regData15.uavPllCtl_avPllDiv_B3 = div;
                        regData15.uavPllCtl_avPllDiv_B4 = div;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl15), regData15.u32);

                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl16), &regData16.u32);
                        regData16.uavPllCtl_avPllDiv_B5 = div;
                        regData16.uavPllCtl_avPllDiv_B6 = div;
                        regData16.uavPllCtl_avPllDiv_B7 = div;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl16), regData16.u32);
                        break;
                };
                case 1:
                {
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl14), &regData14.u32);
                        regData14.uavPllCtl_avPllDiv_B1 = div;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl14), regData14.u32);
                        break;
                }
                case 2:
                case 3:
                case 4:
                {
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl15), &regData15.u32);
                        if(chId==2)
                                regData15.uavPllCtl_avPllDiv_B2 = div;
                        else if(chId==3)
                                regData15.uavPllCtl_avPllDiv_B3 = div;
                        else if(chId==4)
                                regData15.uavPllCtl_avPllDiv_B4 = div;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl15), regData15.u32);
                        break;
                }
                case 5:
                case 6:
                case 7:
                {
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl16), &regData16.u32);
                        if(chId==5)
                                regData16.uavPllCtl_avPllDiv_B5 = div;
                        else if(chId==6)
                                regData16.uavPllCtl_avPllDiv_B6 = div;
                        else if(chId==7)
                                regData16.uavPllCtl_avPllDiv_B7 = div;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl16), regData16.u32);
                        break;
                }
                default:
                {
                        //dbg_printf(PRN_RES, "bad chan ID\n");
                        return 1;
                }
        }

        return 0;
}

int setAvPll_A_offset (int offset, int chId)
{
        unsigned int reg_offset;

        if(offset<0)
        {
                reg_offset = (1<<15) | (-offset) ;
        }
        else
        {
                reg_offset = offset;
        }

        //dbg_printf(PRN_RES, "set A%d offset to 0x%x\n", chId, reg_offset);

        switch (chId)
        {
                case 1:
                {
                        T32Gbl_avPllCtl3 regData3;
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl3), &regData3.u32);
                        regData3.uavPllCtl_avPllFreq_Offset_A1 = reg_offset;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl3), regData3.u32);
                        break;
                }
                case 2:
                {
                        T32Gbl_avPllCtl4 regData4;
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl4), &regData4.u32);
                        regData4.uavPllCtl_avPllFreq_Offset_A2 = reg_offset;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl4), regData4.u32);
                        break;
                }
                case 3:
                {
                        T32Gbl_avPllCtl4 regData4;
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl4), &regData4.u32);
                        regData4.uavPllCtl_avPllFreq_Offset_A3 = reg_offset;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl4), regData4.u32);
                        break;
                }
                case 4:
                {
                        T32Gbl_avPllCtl5 regData5;
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl5), &regData5.u32);
                        regData5.uavPllCtl_avPllFreq_Offset_A4 = reg_offset;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl5), regData5.u32);
                        break;
                }
                case 5:
                {
                        T32Gbl_avPllCtl5 regData5;
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl5), &regData5.u32);
                        regData5.uavPllCtl_avPllFreq_Offset_A5 = reg_offset;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl5), regData5.u32);
                        break;
                }
                case 6:
                {
                        T32Gbl_avPllCtl6 regData6;
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl6), &regData6.u32);
                        regData6.uavPllCtl_avPllFreq_Offset_A6 = reg_offset;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl6), regData6.u32);
                        break;
                }
                case 7:
                {
                        T32Gbl_avPllCtl6 regData6;
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl6), &regData6.u32);
                        regData6.uavPllCtl_avPllFreq_Offset_A7 = reg_offset;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl6), regData6.u32);
                        break;
                }
                case 8:
                {
                        T32Gbl_avPllCtl7 regData7;
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl7), &regData7.u32);
                        regData7.uavPllCtl_avPllFreq_Offset_A8 = reg_offset;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl7), regData7.u32);
                        break;
                }
                default:
                {
                        //dbg_printf(PRN_RES, "bad Channel ID %d\n", chId);
                        return 1;
                }
        }
        //toggle the offset_rdy bit
        {
                volatile int i;

                T32Gbl_avPllCtl11 regData;
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), &regData.u32);
                regData.uavPllCtl_avPllFreq_Offset_Rdy_A = (1<<(7-chId));
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), regData.u32);

                for(i=0; i<10000; i++);

                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), &regData.u32);
                regData.uavPllCtl_avPllFreq_Offset_Rdy_A = 0;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), regData.u32);
        }

        return 0;

}

int setAvPll_B_offset (int offset, int chId)
{
        unsigned int reg_offset;
        volatile int i;

        if (offset < 0)
                reg_offset = (1 << 15) | (-offset);
        else
                reg_offset = offset;

        //dbg_printf(PRN_RES, "set B%d offset to 0x%x\n", chId, reg_offset);
        switch (chId)
        {
                case 1:
                {
                        T32Gbl_avPllCtl7 regData7;
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl7), &regData7.u32);
                        regData7.uavPllCtl_avPllFreq_Offset_B1 = reg_offset;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl7), regData7.u32);
                        break;
                }
                case 2:
                {
                        T32Gbl_avPllCtl8 regData8;
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl8), &regData8.u32);
                        regData8.uavPllCtl_avPllFreq_Offset_B2 = reg_offset;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl8), regData8.u32);
                        break;
                }
                case 3:
                {
                        T32Gbl_avPllCtl8 regData8;
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl8), &regData8.u32);
                        regData8.uavPllCtl_avPllFreq_Offset_B3 = reg_offset;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl8), regData8.u32);
                        break;
                }
                case 4:
                {
                        T32Gbl_avPllCtl9 regData9;
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), &regData9.u32);
                        regData9.uavPllCtl_avPllFreq_Offset_B4 = reg_offset;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), regData9.u32);
                        break;
                }
                case 5:
                {
                        T32Gbl_avPllCtl9 regData9;
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), &regData9.u32);
                        regData9.uavPllCtl_avPllFreq_Offset_B5 = reg_offset;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl9), regData9.u32);
                        break;
                }
                case 6:
                {
                        T32Gbl_avPllCtl10 regData10;
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl10), &regData10.u32);
                        regData10.uavPllCtl_avPllFreq_Offset_B6 = reg_offset;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl10), regData10.u32);
                        break;
                }
                case 7:
                {
                        T32Gbl_avPllCtl10 regData10;
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl10), &regData10.u32);
                        regData10.uavPllCtl_avPllFreq_Offset_B7 = reg_offset;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl10), regData10.u32);
                        break;
                }
                case 8:
                {
                        T32Gbl_avPllCtl11 regData11;
                        BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), &regData11.u32);
                        regData11.uavPllCtl_avPllFreq_Offset_B8 = reg_offset;
                        BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), regData11.u32);
                        break;
                }
                default:
                {
                        //dbg_printf(PRN_RES, "bad Channel ID %d\n", chId);
                        return 1;
                }
        }

        //toggle the offset_rdy bit
        {
                T32Gbl_avPllCtl11 regData;
                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), &regData.u32);
                regData.uavPllCtl_avPllFreq_Offset_Rdy_B = (1<<(7-chId));;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), regData.u32);

                //add some delay because need to be asserted by 30ns
                for (i = 0; i < 1000; i++);

                BFM_HOST_Bus_Read32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), &regData.u32);
                regData.uavPllCtl_avPllFreq_Offset_Rdy_B = 0;
                BFM_HOST_Bus_Write32((MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_avPllCtl11), regData.u32);
        }

        return 0;
}

int AVPLL_Set(int groupId, int chanId, int refFreqIndex, float avFreq)
{
        int ppmOffset;
        int postDiv;
        int postDiv2;
        int iResult;
        float refFreq = refFreqs[refFreqIndex];

        if ((groupId != 0) && (groupId != 1))
            return 0;

        if (((groupId == 0) && (pll_A_VCO_Setting != refFreqIndex))
           || ((groupId == 1) && (pll_B_VCO_Setting != refFreqIndex))) {

            changeRefFreq(refFreqIndex, groupId);
        }

        iResult=calcAVPLL(refFreq, avFreq, &ppmOffset, &postDiv, &postDiv2);

        //printf("%s: InputClk=%f, OutputClk=%f, ppmOffset=%d, div=%d, div2=%d\n", __FUNCTION__, refFreq, avFreq, ppmOffset, postDiv, postDiv2-8);


        if(iResult)
        {
                return 1;
        }

        //set the registers
        if(postDiv2 & DIV2_ENABLE_MASK)
        {
                //using Div2
                if(groupId)
                        iResult=setAvPll_B_Div2(postDiv2,chanId);
                else
                        iResult=setAvPll_A_Div2(postDiv2,chanId);
        }
        else
        {
                if(groupId)
                        iResult=setAvPll_B_Div(postDiv,chanId);
                else
                        iResult=setAvPll_A_Div(postDiv,chanId);
        }

        //dbg_printf(PRN_RES, "no offset for now\n");

        if(iResult) return iResult;

        //set the offset register
        if(groupId)
        {
                iResult=setAvPll_B_offset (ppmOffset,chanId);
        }
        else
        {
                iResult=setAvPll_A_offset (ppmOffset,chanId);
        }
        return iResult;
}

int AVPLL_SetWithPPM1K(int groupId, int chanId, int refFreqIndex, float avFreq, int ppm1k_en)
{
        int ppm1K;
        int postDiv;
        int postDiv2;
        int iResult;
        float refFreq = refFreqs[refFreqIndex];

        if ((groupId != 0) && (groupId != 1))
            return 0;

        if (((groupId == 0) && (pll_A_VCO_Setting != refFreqIndex))
           || ((groupId == 1) && (pll_B_VCO_Setting != refFreqIndex))) {

            changeRefFreq(refFreqIndex, groupId);
        }

        iResult=calcPPM1kAVPLL(refFreq, avFreq, ppm1k_en, &ppm1K, &postDiv, &postDiv2);

        //printf("%s: InputClk=%f, OutputClk=%f, ppm1K=%d, div=%d, div2=%d\n", __FUNCTION__, refFreq, avFreq, ppm1K, postDiv, postDiv2-8);

        if(iResult)
        {
                return 1;
        }

        //set the registers
        if(postDiv2 & DIV2_ENABLE_MASK)
        {
                //using Div2
                if(groupId)
                        iResult=setAvPll_B_Div2(postDiv2,chanId);
                else
                        iResult=setAvPll_A_Div2(postDiv2,chanId);
        }
        else
        {
                if(groupId)
                        iResult=setAvPll_B_Div(postDiv,chanId);
                else
                        iResult=setAvPll_A_Div(postDiv,chanId);
        }

        if(iResult) return iResult;

        //set the ppm1K register
        if(groupId)
        {
                set1KPPM_B(chanId, ppm1K);
        }
        else
        {
                set1KPPM_A(chanId, ppm1K);
        }
        return iResult;
}


#endif // <- #if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
#endif // <- #if defined(BERLIN)

