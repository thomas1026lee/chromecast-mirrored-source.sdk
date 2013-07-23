#ifndef __NAND_CHIP_H
#define __NAND_CHIP_H

struct ndtr0cs0 {
	union{
	struct{
	unsigned int tRP		: 3;
	unsigned int tRH		: 3;
	unsigned int etRP	: 1;
	unsigned int sel_nRE_edge : 1;
	unsigned int tWP		: 3;
	unsigned int tWH		: 3;
	unsigned int reserved	: 2;
	unsigned int tCS		: 3;
	unsigned int tCH		: 3;
	unsigned int Rd_Cnt_Del	: 4;
	unsigned int SelCntr	: 1;
	unsigned int tADL	: 5;
	};
	unsigned int u32_ndtr0cs0;
	};
};

struct ndtr1cs0{
	union{
	struct{
	unsigned int tAR		: 4;
	unsigned int tWHR	: 4;
	unsigned int tRHW	: 2;
	unsigned int reserved	: 4;
	unsigned int PRESCALE	: 1;
	unsigned int wait_mode	: 1;
	unsigned int tR		: 16;
	};
	unsigned int u32_ndtr1cs0;
	};
};

struct ndredel{
	union{
	struct{
	unsigned int reserved	: 4;
	unsigned int nd_re_en	: 4;
	unsigned int nd_re_sel	: 4;
	unsigned int nd_re_del7	: 4;
	unsigned int nd_re_del6	: 4;
	unsigned int nd_re_del5	: 4;
	unsigned int nd_re_del4	: 4;
	unsigned int nd_re_del3	: 4;
	unsigned int nd_re_del2	: 4;
	unsigned int nd_re_del1	: 4;
	unsigned int nd_re_del0	: 4;
	};
	unsigned int u32_ndredel;
	};
};
//unit: ns
struct nand_timing{
	int tADL; //Address to Data loding time
	int tREA; //nRE access time
	int tCH; //Enable signal hold time
	int tCS; //enable signal setup time
	int tWH; //nWE high duration time
	int tWP; //nWE pulse width
	int tRH; //or tREH, nRE high duration time
	int tRP; //nRE pulse width
	int tR; //nWE high to nRE low for read,
	int tRHW; //Read to write delay
	int tWHR; //nWE high to nRE low for read status
	int tAR; //ALE low to nRE low delay
};

struct nand_param {
	const char maker;
	const char dev_id;
	const char id[6];
	const int page_size;
	const int block_size;
	const int chip_size; //MB
	int t;//ECC strenght
	struct nand_timing * timing;
};

struct nand_timing timing_ONFI_mode0 = {//ONFI mode 0
	.tADL = 120,
	.tREA = 40,
	.tCH = 20,
	.tCS = 40,
	.tWH = 30,
	.tWP = 40,
	.tRH = 30,
	.tRP = 50,
	.tR = 200000,
	.tRHW = 200,
	.tWHR = 100,
	.tAR = 25,
};

struct nand_timing timing_ONFI_mode5 = {//ONFI mode 5
	.tADL = 70,
	.tREA = 16,
	.tCH = 5,
	.tCS = 15,
	.tWH = 7,
	.tWP = 10,
	.tRH = 7,
	.tRP = 10,
	.tR = 60000,
	.tRHW = 100,
	.tWHR = 60,
	.tAR = 10,
};

//Samsung
struct nand_timing timing_K9G8G08x0M = {
	.tADL = 100,
	.tREA = 20,
	.tCH = 5,
	.tCS = 15, //20
	.tWH = 10,
	.tWP = 15,
	.tRH = 10,
	.tRP = 15,
	.tR = 60000,
	.tRHW = 100,
	.tWHR = 60,
	.tAR = 10,
};

struct nand_timing timing_K9GAG08x0M = {
	.tADL = 100,
	.tREA = 20,
	.tCH = 5,
	.tCS = 12, //20
	.tWH = 10,
	.tWP = 12,
	.tRH = 10,
	.tRP = 12,
	.tR = 60000,
	.tRHW = 100,
	.tWHR = 60,
	.tAR = 10,
};

struct nand_timing timing_MT29F= {
	.tADL = 70,
	.tREA = 16,
	.tCH = 5,
	.tCS = 15, //20
	.tWH = 7,
	.tWP = 10,
	.tRH = 10,
	.tRP = 10,
	.tR = 25000,
	.tRHW = 100,
	.tWHR = 60,
	.tAR = 10,
};

//Hynix
struct nand_timing timing_HY27UFxx2G2B= {
	.tADL = 100,
	.tREA = 20,
	.tCH = 5,
	.tCS = 12, //20
	.tWH = 10,
	.tWP = 12,
	.tRH = 10,
	.tRP = 12,
	.tR = 60000,
	.tRHW = 100,
	.tWHR = 60,
	.tAR = 10,
};

struct nand_param chip_default = {
	.maker = 0,
	.dev_id = 0,
	.t = 16,
	.timing = &timing_K9G8G08x0M,
};

/*
 * Samsung
 */
//2K page
struct nand_param SAMSUNG_K9G8G08x0M = {
	.maker = 0xEC,
	.dev_id = 0xD3,
	.id = {0x51, 0x95, 0x58, 0xEC, 0x00, 0x00},
	.t = 16,
	.page_size = 2048,
	.block_size = 256*1024,
	.chip_size = 1024,
	.timing = &timing_K9G8G08x0M,
};

//4K page
struct nand_param SAMSUNG_K9GAG08x0M = {
	.maker = 0xEC,
	.dev_id = 0xD5,
	.id = {0x14, 0xB6, 0x74, 0xEC, 0x00, 0x00},
	.t = 16,
	.page_size = 4096,
	.block_size = 512*1024,
	.chip_size = 2048,
	.timing = &timing_K9GAG08x0M,
};

struct nand_param SAMSUNG_K9GAG08U0D = {
	.maker = 0xEC,
	.dev_id = 0xD5,
	.id = {0x94, 0x29, 0x34, 0x41, 0x00, 0x00},
	.t = 48,
	.page_size = 4096,
	.block_size = 512*1024,
	.chip_size = 2048,
	.timing = &timing_K9GAG08x0M,
};

//8K page
struct nand_param SAMSUNG_K9G8G08U0C = {
	.maker = 0xEC,
	.dev_id = 0xD3,
	.id = {0x84, 0x72, 0x50, 0x42, 0x00, 0x00},
	.t = 48,
	.page_size = 8192,
	.block_size = 512*1024,
	.chip_size = 1024,
	.timing = &timing_K9GAG08x0M,
};

struct nand_param SAMSUNG_K9GBG08U0A = {
	.maker = 0xEC,
	.dev_id = 0xD7,
	.id = {0x94, 0x7A, 0x54, 0x43, 0x00, 0x00},
	.t = 48,
	.page_size = 8192,
	.block_size = 1024*1024,
	.chip_size = 4096,
	.timing = &timing_K9GAG08x0M,
};

struct nand_param SAMSUNG_K9GAG08U0E = {
	.maker = 0xEC,
	.dev_id = 0xD5,
	.id = {0x84, 0x72, 0x50, 0x42, 0x00, 0x00},
	.t = 48,
	.page_size = 8192,
	.block_size = 1024*1024,
	.chip_size = 2048,
	.timing = &timing_K9GAG08x0M,
};

struct nand_param SAMSUNG_K9LBG08U0E = {
	.maker = 0xEC,
	.dev_id = 0xD7,
	.id = {0xC5, 0x72, 0x54, 0x42, 0x00, 0x00},
	.t = 48,
	.page_size = 8192,
	.block_size = 1024*1024,
	.chip_size = 4096,
	.timing = &timing_K9GAG08x0M,
};

/*
 * Hynix
 */
//2K
struct nand_param HINIX_HY27UF082G2B= {
	.maker = 0xAD,
	.dev_id = 0xDA,
	.id = {0x10, 0x95, 0x44, 0xAD, 0x00, 0x00},
	.t = 16,
	.page_size = 2048,
	.block_size = 128*1024,
	.chip_size = 256,
	.timing = &timing_HY27UFxx2G2B,
};

struct nand_param HINIX_HY27UF084G2B= {
	.maker = 0xAD,
	.dev_id = 0xDC,
	.id = {0x10, 0x95, 0x54, 0xAD, 0x00, 0x00},
	.t = 16,
	.page_size = 2048,
	.block_size = 128*1024,
	.chip_size = 512,
	.timing = &timing_HY27UFxx2G2B,
};
//8K
struct nand_param HINIX_HY27UBG8T2A= {
	.maker = 0xAD,
	.dev_id = 0xD7,
	.id = {0x94, 0x9A, 0x74, 0x42, 0x00, 0x00},
	.t = 48,
	.page_size = 8192,
	.block_size = 2*1024*1024,
	.chip_size = 4096,
	.timing = &timing_HY27UFxx2G2B,
};

/*
 * Micron
 */
//4K page
struct nand_param MICRON_MT298G08A = {
	.maker = 0x2C,
	.dev_id = 0xD3,
	.t = 16,
	.timing = &timing_ONFI_mode5,
};

struct nand_param MICRON_MT29F32G08CBACA= {
	.maker = 0x2C,
	.dev_id = 0x68,
	.id = {0x04, 0x46, 0x89, 0x00, 0x00, 0x00},
	.t = 48,
	.page_size = 4096,
	.block_size = 1024*1024,
	.chip_size = 4096,
	.timing = &timing_K9G8G08x0M,
};

struct nand_param MICRON_MT29F16G08ABABA= {
	.maker = 0x2C,
	.dev_id = 0x48,
	.id = {0x00, 0x26, 0x89, 0x00, 0x00, 0x00},
	.t = 16,
	.page_size = 4096,
	.block_size = 512*1024,
	.chip_size = 2048,
	.timing = &timing_K9G8G08x0M,
};

struct nand_param MICRON_MT29F4G08ABADA = {
	.maker = 0x2C,
	.dev_id = 0xDC,
	.id = {0x90, 0x95, 0x56, 0x00, 0x00, 0x00},
	.t = 16,
	.page_size = 2048,
	.block_size = 128*1024,
	.chip_size = 512,
	.timing = &timing_K9G8G08x0M,
};

struct nand_param *buildin_nand_list []= {
	&chip_default,
	&SAMSUNG_K9GBG08U0A,
	&SAMSUNG_K9GAG08x0M,
	&SAMSUNG_K9G8G08x0M, 
	&SAMSUNG_K9GAG08U0D, 
	&SAMSUNG_K9G8G08U0C, 
	&SAMSUNG_K9GAG08U0E, 
	&SAMSUNG_K9LBG08U0E, 
	&HINIX_HY27UF082G2B,
	&HINIX_HY27UF084G2B,
	&HINIX_HY27UBG8T2A,
	&MICRON_MT29F4G08ABADA, 
	&MICRON_MT298G08A,
	&MICRON_MT29F32G08CBACA,
	&MICRON_MT29F16G08ABABA,
	&MICRON_MT29F4G08ABADA, 
	NULL
};
#endif
