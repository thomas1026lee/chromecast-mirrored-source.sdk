/*
 * drivers/mtd/nand/mv_nand.c
 *
 *  Copyright (C) 2008 hzchen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <common.h>
#if defined(__UBOOT__)
#include <malloc.h>
#include "galois_io.h"
#else
#include "io.h"
#include "lgpl_printf.h"

#endif /* #if defined(__UBOOT__) */

#include "global.h"
#include "memmap.h"
#include "nand_priv.h"
#include "mv_nand.h"
#include "galois_speed.h"

#define SUPPORT_RANDOMIZATION

#define debug_printf(a)
#define NAND_TIME_OUT	10000000
char mvnand_page_buffer[MV_NAND_MAX_PAGE_SIZE + MV_NAND_MAX_OOB_SIZE];

#define RAND_DATA_POS   0xB00000

#if defined(__UBOOT__)
static struct mv_nand_flash_dev nand_flash_ids[] = {
	/*
	{"NAND 1MiB 5V 8-bit",		0x6e, 256, 1, 0x1000, 0},
	{"NAND 2MiB 5V 8-bit",		0x64, 256, 2, 0x1000, 0},
	{"NAND 2MiB 5V 8-bit",		0x64, 256, 2, 0x1000, 0},
	{"NAND 1MiB 3,3V 8-bit",	0xe8, 256, 1, 0x1000, 0},
	{"NAND 1MiB 3,3V 8-bit",	0xec, 256, 1, 0x1000, 0},
	{"NAND 2MiB 3,3V 8-bit",	0xea, 256, 2, 0x1000, 0},
	{"NAND 4MiB 3,3V 8-bit",	0xd5, 512, 4, 0x2000, 0},
	{"NAND 4MiB 3,3V 8-bit",	0xe3, 512, 4, 0x2000, 0},
	{"NAND 4MiB 3,3V 8-bit",	0xe5, 512, 4, 0x2000, 0},
	{"NAND 8MiB 3,3V 8-bit",	0xd6, 512, 8, 0x2000, 0},

	{"NAND 8MiB 1,8V 8-bit",	0x39, 512, 8, 0x2000, 0},
	{"NAND 8MiB 3,3V 8-bit",	0xe6, 512, 8, 0x2000, 0},
	{"NAND 8MiB 1,8V 16-bit",	0x49, 512, 8, 0x2000, NAND_BUSWIDTH_16},
	{"NAND 8MiB 3,3V 16-bit",	0x59, 512, 8, 0x2000, NAND_BUSWIDTH_16},
	
	{"NAND 16MiB 1,8V 8-bit",	0x33, 512, 16, 0x4000, 0},
	{"NAND 16MiB 3,3V 8-bit",	0x73, 512, 16, 0x4000, 0},
	{"NAND 16MiB 1,8V 16-bit",	0x43, 512, 16, 0x4000, NAND_BUSWIDTH_16},
	{"NAND 16MiB 3,3V 16-bit",	0x53, 512, 16, 0x4000, NAND_BUSWIDTH_16},
	*/
	{"NAND 32MiB 1,8V 8-bit",	0x35, 512, 32, 0x4000, 0},
	{"NAND 32MiB 3,3V 8-bit",	0x75, 512, 32, 0x4000, 0},
	{"NAND 32MiB 1,8V 16-bit",	0x45, 512, 32, 0x4000, NAND_BUSWIDTH_16},
	{"NAND 32MiB 3,3V 16-bit",	0x55, 512, 32, 0x4000, NAND_BUSWIDTH_16},
	
	{"NAND 64MiB 1,8V 8-bit",	0x36, 512, 64, 0x4000, 0},
	{"NAND 64MiB 3,3V 8-bit",	0x76, 512, 64, 0x4000, 0},
	{"NAND 64MiB 1,8V 16-bit",	0x46, 512, 64, 0x4000, NAND_BUSWIDTH_16},
	{"NAND 64MiB 3,3V 16-bit",	0x56, 512, 64, 0x4000, NAND_BUSWIDTH_16},
	
	{"NAND 128MiB 1,8V 8-bit",	0x78, 512, 128, 0x4000, 0},
	{"NAND 128MiB 1,8V 8-bit",	0x39, 512, 128, 0x4000, 0},
	{"NAND 128MiB 3,3V 8-bit",	0x79, 512, 128, 0x4000, 0},
	{"NAND 128MiB 1,8V 16-bit",	0x72, 512, 128, 0x4000, NAND_BUSWIDTH_16},
	{"NAND 128MiB 1,8V 16-bit",	0x49, 512, 128, 0x4000, NAND_BUSWIDTH_16},
	{"NAND 128MiB 3,3V 16-bit",	0x74, 512, 128, 0x4000, NAND_BUSWIDTH_16},
	{"NAND 128MiB 3,3V 16-bit",	0x59, 512, 128, 0x4000, NAND_BUSWIDTH_16},
	
	{"NAND 256MiB 3,3V 8-bit",	0x71, 512, 256, 0x4000, 0},

#define LP_OPTIONS (NAND_SAMSUNG_LP_OPTIONS | NAND_NO_READRDY | NAND_NO_AUTOINCR)
#define LP_OPTIONS16 (LP_OPTIONS | NAND_BUSWIDTH_16)

	/*512 Megabit */
	{"NAND 64MiB 1,8V 8-bit",	0xA2, 0,  64, 0, LP_OPTIONS},
	{"NAND 64MiB 3,3V 8-bit",	0xF2, 0,  64, 0, LP_OPTIONS},
	{"NAND 64MiB 1,8V 16-bit",	0xB2, 0,  64, 0, LP_OPTIONS16},
	{"NAND 64MiB 3,3V 16-bit",	0xC2, 0,  64, 0, LP_OPTIONS16},

	/* 1 Gigabit */
	{"NAND 128MiB 1,8V 8-bit",	0xA1, 0, 128, 0, LP_OPTIONS},
	{"NAND 128MiB 3,3V 8-bit",	0xF1, 0, 128, 0, LP_OPTIONS},
	{"NAND 128MiB 1,8V 16-bit",	0xB1, 0, 128, 0, LP_OPTIONS16},
	{"NAND 128MiB 3,3V 16-bit",	0xC1, 0, 128, 0, LP_OPTIONS16},

	/* 2 Gigabit */
	{"NAND 256MiB 1,8V 8-bit",	0xAA, 0, 256, 0, LP_OPTIONS},
	{"NAND 256MiB 3,3V 8-bit",	0xDA, 0, 256, 0, LP_OPTIONS},
	{"NAND 256MiB 1,8V 16-bit",	0xBA, 0, 256, 0, LP_OPTIONS16},
	{"NAND 256MiB 3,3V 16-bit",	0xCA, 0, 256, 0, LP_OPTIONS16},

	/* 4 Gigabit */
	{"NAND 512MiB 1,8V 8-bit",	0xAC, 0, 512, 0, LP_OPTIONS},
	{"NAND 512MiB 3,3V 8-bit",	0xDC, 0, 512, 0, LP_OPTIONS},
	{"NAND 512MiB 1,8V 16-bit",	0xBC, 0, 512, 0, LP_OPTIONS16},
	{"NAND 512MiB 3,3V 16-bit",	0xCC, 0, 512, 0, LP_OPTIONS16},

	/* 8 Gigabit */
	{"NAND 1GiB 1,8V 8-bit",	0xA3, 0, 1024, 0, LP_OPTIONS},
	{"NAND 1GiB 3,3V 8-bit",	0xD3, 0, 1024, 0, LP_OPTIONS},
	{"NAND 1GiB 1,8V 16-bit",	0xB3, 0, 1024, 0, LP_OPTIONS16},
	{"NAND 1GiB 3,3V 16-bit",	0xC3, 0, 1024, 0, LP_OPTIONS16},

	/* 16 Gigabit */
	{"NAND 2GiB 1,8V 8-bit",	0xA5, 0, 2048, 0, LP_OPTIONS},
	{"NAND 2GiB 3,3V 8-bit",	0xD5, 0, 2048, 0, LP_OPTIONS},
	{"NAND 2GiB 1,8V 16-bit",	0xB5, 0, 2048, 0, LP_OPTIONS16},
	{"NAND 2GiB 3,3V 16-bit",	0xC5, 0, 2048, 0, LP_OPTIONS16},
	{"NAND 2GiB 3,3V 16-bit",	0x48, 0, 2048, 0, LP_OPTIONS16},

	/* 32 Gigabit */
	{"NAND 4GiB 1,8V 8-bit",	0xD7, 0, 4096, 0, LP_OPTIONS},
	{"NAND 4GiB 1,8V 8-bit",	0x68, 0, 4096, 0, LP_OPTIONS},

	{"AND 128MiB 3,3V 8-bit",	0x01, 2048, 128, 0x4000,
	NAND_IS_AND | NAND_NO_AUTOINCR |NAND_NO_READRDY | NAND_4PAGE_ARRAY |
	BBT_AUTO_REFRESH},
	{0,}
};

static struct nand_manufacturers nand_manuf_ids[] = {
	{NAND_MFR_TOSHIBA, "Toshiba"},
	{NAND_MFR_SAMSUNG, "Samsung"},
	{NAND_MFR_FUJITSU, "Fujitsu"},
	{NAND_MFR_NATIONAL, "National"},
	{NAND_MFR_RENESAS, "Renesas"},
	{NAND_MFR_STMICRO, "ST Micro"},
	{NAND_MFR_HYNIX, "Hynix"},
	{NAND_MFR_MICRON, "Micron"},
	{0x0, "Unknown"}
};

static char rand_nand_id_lst[][8] = {
        {0xEC, 0xD7, 0x94, 0x7A, 0x54, 0x43, 0x0, 0x0},
        {0},
};

#endif /* #if defined(__UBOOT__) */

static int mv_nand_cmd_request(void);

static unsigned int real_device_oobsize;

struct mv_nand_data nand_data;
loff_t chip_size=0;

#define NAKED_CMD(cs, cmd) \
		do{ \
			int ctrl; \
			mv_nand_cmd_request(); \
			ctrl = NDCB0_CS(cs) | NDCB0_CMD_NAKED_CMD | NDCB0_NC \
				| NDCB0_ADDRC(5) | NDCB0_CMD2(0x0) | NDCB0_CMD1(cmd); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ctrl); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0); \
			if( wait_status_bit_set(cs ? MV_NAND_NDSR_CS1_CMDD : MV_NAND_NDSR_CS0_CMDD) ) \
				return -1; \
		}while(0);

#define NAKED_CMD_SENSE(cs, cmd) \
		do{ \
			int ctrl; \
			mv_nand_cmd_request(); \
			ctrl = NDCB0_CS(cs) | NDCB0_CMD_RESET | NDCB0_NC | NDCB0_CMD1(cmd); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ctrl); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0); \
			if(wait_status_bit_set(cs ? MV_NAND_NDSR_RDY1 : MV_NAND_NDSR_RDY0)) \
				return -1; \
		}while(0);

#define READ_ID(cs, ovlen, addr, cmd) \
		do { \
			int ctrl; \
			mv_nand_cmd_request(); \
			ctrl = NDCB0_CS(cs) | NDCB0_LENOVRD | NDCB0_CMD_READID \
				| NDCB0_ADDRC(1) | NDCB0_NC | NDCB0_CMD1(cmd); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ctrl); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, addr); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET,  \
				NDCB3_NDLENCNT(ovlen)); \
		} while(0);

#define NAKED_ADDR(cs, ac, ndcr1, ndcr2, ndcr3) \
		do{\
			int ctrl; \
			mv_nand_cmd_request(); \
			ctrl = NDCB0_CS(cs) | NDCB0_CMD_NAKED_ADDR | NDCB0_NC \
				| NDCB0_ADDRC(ac) | NDCB0_CMD2(0x0) | NDCB0_CMD1(0x0); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ctrl); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ndcr1); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ndcr2); \
			if(ac >= 6) \
				MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ndcr3); \
			if( wait_status_bit_set(cs ? MV_NAND_NDSR_CS1_CMDD : MV_NAND_NDSR_CS0_CMDD) ) \
				return -1; \
		}while(0);

#define NAKED_WR_DATA(cs, ovlen) \
		do { \
			int ctrl; \
			mv_nand_cmd_request(); \
			ctrl = NDCB0_CMDX_NWRITE | NDCB0_LENOVRD | NDCB0_CS(cs) \
				| NDCB0_CMD_PROGRAM | NDCB0_NC \
				| NDCB0_ADDRC(5) | NDCB0_CMD2(0x0) | NDCB0_CMD1(0x0); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ctrl); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET,  \
				NDCB3_NDLENCNT(ovlen)); \
			if( wait_status_bit_set(MV_NAND_NDSR_WRDREQ) ) \
				return -1; \
		}while(0);

#define NAKED_RD_DATA(cs, ovlen) \
		do { \
			int ctrl; \
			mv_nand_cmd_request(); \
			ctrl = NDCB0_CMDX_NREAD |NDCB0_LENOVRD | NDCB0_CS(cs) \
				| NDCB0_CMD_READ | NDCB0_NC \
				| NDCB0_ADDRC(0) | NDCB0_CMD2(0x0) | NDCB0_CMD1(0x0); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ctrl); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0); \
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, \
				NDCB3_NDLENCNT(ovlen)); \
		}while(0);

unsigned int mv_nand_page_size(void){
	return (nand_data.szofpg);
}
unsigned int mv_nand_block_size(void){
	return (nand_data.szofblk);
}
unsigned int mv_nand_oob_size(void){
	return real_device_oobsize;
}
loff_t mv_nand_chip_size(void){
	return chip_size;
}

static inline int wait_status_bit_set(unsigned int bit)
{
	unsigned int read, i=NAND_TIME_OUT;
	
	//by pass ready check
	do {
		MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDSR_OFFSET , &read);	
		read &= bit;
	} while (!read && --i);

	if(!i) {
		printf("wait bit %08X time out!\n", bit);
		return -1;
	}
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDSR_OFFSET, read);
	return 0;
}

/*
 * clean all status bit, set NDCR.ND_RUN bit
 */
static void mv_nand_start(void)
{
	unsigned int read;
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE ,MV_NAND_NDSR_OFFSET , MV_NAND_NDSR_VAL);
	MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCR_OFFSET , &read);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCR_OFFSET,
				(read | MV_NAND_NDCR_ND_RUN));
}

/*
 * clean all status bit, clean NDCR.ND_RUN bit
 */
static void mv_nand_disable(void)
{
	unsigned int read;
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDSR_OFFSET , MV_NAND_NDSR_VAL);
	MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCR_OFFSET , &read);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCR_OFFSET,
				(read &(~ MV_NAND_NDCR_ND_RUN)));
}

static inline int mv_nand_cmd_request(void)
{
	return wait_status_bit_set(MV_NAND_NDSR_WRCMDREQ);
}

#if defined(__UBOOT__)
static void dump_registers(void)
{
	unsigned int read;

	// Read back for sanity check
	MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDTR0CS0_OFFSET, &read);				
	printf("NDTR0CS0 reads 0x%X\n", read);
	
	MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDTR1CS0_OFFSET, &read);				
	printf("NDTR1CS0 reads 0x%X\n", read);
	
	MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDREDEL_OFFSET, &read);				
	printf("NDREDEL reads 0x%X\n", read);
	
	MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDSR_OFFSET, &read);					
	printf("NDSR reads 0x%X\n", read);
	
	MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDCR_OFFSET, &read);					
	printf("NDCR reads 0x%X\n", read);

	MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDECCCTRL_OFFSET, &read);
	printf("NDECCCTRL reads 0x%X\n", read);

	
}

int process_inital_bad_block(void)
{
	return 0;
}
#endif /* #if defined(__UBOOT__) */
#ifdef SUPPORT_RANDOMIZATION
/****************************************************************************
 *
 * For Randomizer Start
 *
 ****************************************************************************/
#include "nand_randomizer.h"

#define TRACE_LOG(...)	//printf

#define DUMP_8_BYTES(name, b)                                               \
    if (b)                                                                  \
    {                                                                       \
        TRACE_LOG("%s(0x%08X): %02X %02X %02X %02X %02X %02X %02X %02X\n",  \
            name, (int)(b),                                                 \
            (b)[0], (b)[1], (b)[2], (b)[3], (b)[4], (b)[5], (b)[6], (b)[7]);\
    }

#define NAND_ID_SIZE			(4)	// FIXME: 4 is a hack. we need to fix it.
// most of the chip should be 6

#define RANDOMIZER_BUFFER_SIZE_MAX	(MV_NAND_RANDOMIZER_BUFFER_SIZE_MAX)
#define RANDOMIZED_BUFFER_SIZE_MAX	(MV_NAND_MAX_PAGE_SIZE + MV_NAND_MAX_OOB_SIZE)

//unsigned char g_randomizer_buffer[RANDOMIZER_BUFFER_SIZE_MAX];
static unsigned char g_randomized_buffer[RANDOMIZED_BUFFER_SIZE_MAX];


static void nand_randomizer_init_by_flash_type (void)
{
	struct mv_nand_data *board = &nand_data;
	unsigned int block_size = board->szofblk;
	unsigned int page_size = board->szofpg;
	unsigned int oob_size = 32; // should be mtd->oobsize, we fix 32 to make sure it can work on marvell NFC
	unsigned char id_data[8];
	int randomized = 0;
	static unsigned char g_randomizer_buffer[RANDOMIZER_BUFFER_SIZE_MAX];

	// Get randomizer type by chip_id
	if (0 != mv_nand_read_id(id_data))
	{
		return;
	}

	randomized = mv_nand_randomizer_init(
		id_data, 
		NAND_ID_SIZE,
		block_size,
		page_size, 
		oob_size,
		g_randomizer_buffer,
		RANDOMIZER_BUFFER_SIZE_MAX);
	printf("nand_randomizer_init_by_flash_type(chip_id = 0x%02X%02X%02X%02X%02X%02X): !!! %s !!!\n",
		id_data[0], id_data[1], id_data[2], id_data[3], id_data[4], id_data[5],
		randomized ? "RANDOMIZED" : "UNRANDOMIZED");
}

static void nand_randomizer_release (void)
{
	return;
}

static void dump_block_randomization_info(unsigned int page_addr, int randomized)
{
	static int s_pre_block_index = -1;
	int block_index = page_addr >> 7;
	int page_index = page_addr & 7;
	if (s_pre_block_index != block_index)
	{
		TRACE_LOG("block %d: !!! %s !!! (page=%d)\n", block_index, 
			randomized ? "RANDOMIZED" : "UNRANDOMIZED", page_index);
		s_pre_block_index = block_index;
	}
}

static int is_chunk_blank(const unsigned char *buf, int is_last_chunk)
{
	const int error_pos = is_last_chunk ? 0x7cd : 0x7c8;
	int i;
	for (i = 0; i < CHUNK_SIZE; i++) {
		if (*buf++ != 0xff && i != error_pos)
			return 0;
	}
	return 1;
}

static int is_page_blank(const unsigned char *buf, const unsigned char *oob, int page_size)
{
	const int chunk_count = page_size / CHUNK_SIZE;
	const unsigned char *chunk_start = buf;
	int i;
	for (i = 0; i < chunk_count - 1; i++) {
		if (!is_chunk_blank(chunk_start, 0))
			return 0;
		chunk_start += CHUNK_SIZE;
	}
	// last chunk
	return is_chunk_blank(chunk_start, 1);
}

static int nand_read_page_post_process (unsigned int page_addr, 
					const unsigned char *data_src, const unsigned char *oob_src,
					unsigned char *data_dst, unsigned char *oob_dst)
{
	struct mv_nand_data *board = &nand_data;
	unsigned int block_size = board->szofblk;
	unsigned int page_size = board->szofpg;
	unsigned int oob_size = 32; // mtd->oobsize;

	unsigned int randomized_length = 0;
	randomized_length = mv_nand_randomizer_randomize_page(page_addr, 
		data_src, oob_src, data_dst, oob_dst);

	dump_block_randomization_info(page_addr, randomized_length);
	return randomized_length > 0 ? 1 : 0;
}

static int nand_write_page_pre_process (unsigned int page_addr, 
					const unsigned char *data_src, const unsigned char *oob_src,
					unsigned char *data_dst, unsigned char *oob_dst)
{
	unsigned int randomized_length = 0;

	randomized_length = mv_nand_randomizer_randomize_page(page_addr, 
		data_src, oob_src, data_dst, oob_dst) ? 1 : 0;

	dump_block_randomization_info(page_addr, randomized_length);
	return randomized_length > 0 ? 1 : 0;
}

static void mv_nand_decode_address (loff_t addr, unsigned int *p_block_index, 
				    unsigned int *p_page_index, unsigned int *p_pos_in_page)
{
	struct mv_nand_data *board = &nand_data;
	unsigned int block_size = board->szofblk;
	unsigned int page_size = board->szofpg;
	unsigned int page_shift = board->page_shift;
	unsigned int block_shift = page_shift;	// start from page_shift to calculate it

	while ((1 << block_shift) != block_size)
	{
		block_shift++;
	}

	if (p_block_index)
	{
		*p_block_index = (unsigned int)(addr >> block_shift);
	}

	if (p_page_index)
	{
		*p_page_index = ((unsigned int)addr & (block_size - 1)) >> page_shift;
	}

	if (p_pos_in_page)
	{
		*p_pos_in_page = (unsigned int)addr & (page_size - 1);
	}
}
/****************************************************************************
 *
 * For Randomizer End
 *
 ****************************************************************************/
#endif // SUPPORT_RANDOMIZATION

static int mv_nand_page_bad(unsigned int block_index, unsigned int page_index)
{
	struct mv_nand_data *board = (struct mv_nand_data *)&nand_data;
	unsigned int block_size = board->szofblk;
	unsigned int page_size = board->szofpg;
	loff_t ofs = (loff_t)block_size * block_index + page_size * page_index;
	unsigned int page_addr = (unsigned int)(ofs >> board->page_shift);
	unsigned char bad_marker, bad_marker_expect;
	int randomized;

   	mv_nand_read_page(ofs, mvnand_page_buffer);
	bad_marker = mvnand_page_buffer[page_size];
	if((bad_marker & 0xFF)!= 0xff){
		return 1;
	}

    return 0;
}

/**
 * mv_nand_block_bad - [DEFAULT] Read bad block marker from the chip
 * @ofs:	offset from device start
 * Check, if the block is bad.
 */
int mv_nand_block_bad(loff_t ofs, int getchip)
{
	struct mv_nand_data *board = (struct mv_nand_data *)&nand_data;
#ifdef SUPPORT_RANDOMIZATION
	unsigned int block_size = board->szofblk;
	unsigned int page_size = board->szofpg;
	unsigned int block_index, page_index;

	mv_nand_decode_address (ofs, &block_index, NULL, NULL);
#endif // SUPPORT_RANDOMIZATION

	if (mv_nand_page_bad(block_index, 0))
	{
		return 1;
	}
	if (mv_nand_page_bad(block_index, 1))
	{
		return 1;
	}
	// last page
	if (mv_nand_page_bad(block_index, block_size/page_size - 1))
	{
		return 1;
	}

	return 0;
	
}

int mv_nand_mark_badblock(loff_t ofs, int getchip)
{
	int res = 0;
	int res1 = 0;
	struct mv_nand_data *board = &nand_data;
	unsigned int block_size = board->szofblk;
	unsigned int page_size = board->szofpg;

	ofs = ofs & (~((loff_t)block_size - 1));
	mvnand_page_buffer[page_size] = 0x55;
	res = mv_nand_write_page(ofs, mvnand_page_buffer);
	ofs = ofs + page_size;
	res1 = mv_nand_write_page(ofs, mvnand_page_buffer);
	return res && res1;
}

#if defined(__UBOOT__)
/*
 * configure controller using default para 
 *
 */
static void mv_nand_chip_init(void )
{
	int i,j;
 
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCR_OFFSET, 0);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDTR0CS0_OFFSET,  MV_NAND_NDTR0CS0_VAL);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDTR1CS0_OFFSET, MV_NAND_NDTR1CS0_VAL);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDREDEL_OFFSET, MV_NAND_NDREDEL_VAL);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDSR_OFFSET, MV_NAND_NDSR_VAL);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDECCCTRL_OFFSET, MV_NAND_NDECCCTRL_VAL);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCR_OFFSET, MV_NAND_NDCR_VAL);

	for (i = 0; i < 2176; i++)
		MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDDB_OFFSET, &j);
}
#endif /* #if defined(__UBOOT__) */

static int mv_nand_chip_reinit(void)
{
	struct mv_nand_data *board = (struct mv_nand_data *)&nand_data;
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDTR0CS0_OFFSET,  board->ndtr0cs0);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDTR1CS0_OFFSET, board->ndtr1cs0);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDREDEL_OFFSET, board->ndredel);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDECCCTRL_OFFSET, board->ndeccctrl);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCR_OFFSET, board->ndcr);
	return 0;
}

/*
 * disable ECC and BCH
 */
static void mv_nand_preinit_exrw(void)
{
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDECCCTRL_OFFSET, MV_NAND_NDECCCTRL_VAL);
/*	int ctrl;
	struct mv_nand_data *board = (struct mv_nand_data *)&nand_data;
	if(board->conbch & 0x7F) {
		MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDECCCTRL_OFFSET ,&ctrl);
		ctrl &= ~1;
		MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDECCCTRL_OFFSET, ctrl);
	}
	if(board->conecc){
		MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCR_OFFSET, &ctrl);
		ctrl &= ~MV_NAND_NDCR_ECC_EN;
		MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCR_OFFSET, ctrl);
	}
	if(board->conspare) {
		MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCR_OFFSET ,&ctrl);
		ctrl &= ~MV_NAND_NDCR_SPARE_EN;
		MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCR_OFFSET, ctrl);
	}
*/
}

/*
 * enable ECC and BCH as nessasary
 */
static void mv_nand_preinit_rw(void)
{
	struct mv_nand_data *board = (struct mv_nand_data *)&nand_data;
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDECCCTRL_OFFSET, board->ndeccctrl);
/*	int ctrl;
	struct mv_nand_data *board = (struct mv_nand_data *)&nand_data;
	if(board->conecc){
		MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCR_OFFSET, &ctrl);
		ctrl |= MV_NAND_NDCR_ECC_EN;
		MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCR_OFFSET, ctrl);
	}
	if(board->conspare) {
		MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCR_OFFSET ,&ctrl);
		ctrl |= MV_NAND_NDCR_SPARE_EN;
		MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCR_OFFSET, ctrl);
	}
	if(board->conbch & 0x7F) {
		MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDECCCTRL_OFFSET ,&ctrl);
		ctrl |= (board->conbch & 0x7F);
		MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDECCCTRL_OFFSET, ctrl);
	}
*/
}

int mv_nand_reset_chip(int getchip)
{
	int ctrl;

	mv_nand_preinit_exrw();
	mv_nand_disable();
	mv_nand_start();
	
	if (mv_nand_cmd_request())
		return -1;
	ctrl = 0x00A000FF | (getchip ? MV_NAND_NDCB_CSEL : 0);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ctrl);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0);

	if(wait_status_bit_set(getchip ? MV_NAND_NDSR_RDY1 : MV_NAND_NDSR_RDY0))
		return -1;

	if(wait_status_bit_set(getchip ? MV_NAND_NDSR_CS1_CMDD : MV_NAND_NDSR_CS0_CMDD))
		return -1;

	mv_nand_disable();
	return 0;
}


static inline void mv_nand_read_buf(char *buf ,int len)
{
	unsigned int *buf_tsf = (unsigned int *)buf;
	int i;
	for (i = 0; i < len / sizeof(int); i++){
		MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDDB_OFFSET, &buf_tsf[i]);
	}
}

static inline void mv_nand_write_buf(char *buf ,int len)
{
	unsigned int *buf_tsf = (unsigned int *)buf;
	int i;
	for (i = 0; i < len/sizeof(int); i++)
		MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDDB_OFFSET, buf_tsf[i]);
}

static int mv_nand_read_status(void)
{
	int ctrl;
	int cmd[2];
	struct mv_nand_data *board = &nand_data;
	mv_nand_preinit_exrw();
	mv_nand_disable();
	mv_nand_start();
	mv_nand_cmd_request();
	ctrl = 0x00800070 | (board->ncs ? MV_NAND_NDCB_CSEL : 0);	
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDCB0_OFFSET, ctrl);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDCB0_OFFSET, 0);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDCB0_OFFSET, 0);
	if( wait_status_bit_set(MV_NAND_NDSR_RDDREQ) )
		return -1;
	mv_nand_read_buf((char *)&cmd[0] , 8);
	if( wait_status_bit_set(board->ncs ? (MV_NAND_NDSR_CS1_CMDD) : (MV_NAND_NDSR_CS0_CMDD )) )
		return -1;

	return cmd[0] & 0xff;
}

/*
 * oob: 1, buffer contain oob. 0, buffer does not contain oob
 */
int mv_nand_write_large_page(loff_t srcx, char *buf, int oob)
{
	int ctrl, i;
	unsigned int row_addr;
	unsigned int dst_4321, dst_xxx5;
	struct mv_nand_data *board = &nand_data;
	int page_size = board->szofpg;
	int oob_per_chunk;
	int page_layout = board->conspare;
	const unsigned char default_oob_data[32] = {
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	char* oob_buf = oob ? (buf + page_size) : default_oob_data;

	unsigned int page_shift = board->page_shift;

#ifdef SUPPORT_RANDOMIZATION
	unsigned int page_addr = (unsigned int)(srcx >> page_shift);
	int randomized = 0;

	DUMP_8_BYTES("WriteBefore:", buf);
	randomized = nand_write_page_pre_process(page_addr,
		buf,
		oob ? (buf + page_size) : default_oob_data,
		g_randomized_buffer,
		g_randomized_buffer + page_size);

	if (randomized)
	{
		buf = g_randomized_buffer;
		oob_buf = g_randomized_buffer + page_size;
	}
	TRACE_LOG("Write Page(0x%08X): %s\n", (int)srcx, randomized ? "RANDOMIZED" : "UNRANDOMIZED");
	DUMP_8_BYTES("WriteReal:", buf);
#endif // SUPPORT_RANDOMIZATION

	mv_nand_preinit_rw();
	mv_nand_disable();
	mv_nand_start();
	
	mv_nand_cmd_request();
	ctrl = NDCB0_CS(board->ncs) | NDCB0_CMD_NAKED_CMD | NDCB0_NC 
		| NDCB0_ADDRC(board->addr_cycle) | NDCB0_CMD2(0xFF) | NDCB0_CMD1(0x80);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ctrl);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0);
	if( wait_status_bit_set(board->ncs ? MV_NAND_NDSR_CS1_CMDD : MV_NAND_NDSR_CS0_CMDD) )
		return -1;
	
	mv_nand_cmd_request();
	ctrl = NDCB0_CS(board->ncs) | NDCB0_CMD_NAKED_ADDR | NDCB0_NC 
		| NDCB0_ADDRC(board->addr_cycle) | NDCB0_CMD2(0xFF) | NDCB0_CMD1(0xFF);

	row_addr = (srcx >> page_shift);
	dst_4321 = (row_addr << 16) & 0xFFFF0000;
	dst_xxx5 = (row_addr >> 16) & 0xFF;
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ctrl);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, dst_4321);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, dst_xxx5);
	if( wait_status_bit_set(board->ncs ? MV_NAND_NDSR_CS1_CMDD : MV_NAND_NDSR_CS0_CMDD) )
		return -1;

	i = (page_size - 1 + board->chunk_size)/board->chunk_size;
	if(page_layout==MODE_MULTI_SPARE)
		oob_per_chunk = board->oobsize/i;
	else
		oob_per_chunk = board->oobsize;
	while(i-->0){
		mv_nand_cmd_request();
		ctrl = NDCB0_CMDX_NWRITE | NDCB0_LENOVRD | NDCB0_CS(board->ncs)
			| NDCB0_CMD_PROGRAM | NDCB0_NC 
			| NDCB0_ADDRC(board->addr_cycle) | NDCB0_CMD2(0x0) | NDCB0_CMD1(0x00);
		MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ctrl);
		MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0);
		MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0);
		if((i==0) || (page_layout==MODE_MULTI_SPARE))
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 
					NDCB3_NDLENCNT(board->chunk_size + oob_per_chunk));
		else
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 
					NDCB3_NDLENCNT(board->chunk_size));

		if( wait_status_bit_set(MV_NAND_NDSR_WRDREQ) )
			return -1;

		/* write data */
		mv_nand_write_buf(buf, board->chunk_size);
		buf += board->chunk_size;
		/* write oob */
		if((i==0) || (page_layout==MODE_MULTI_SPARE)) {
			mv_nand_write_buf(oob_buf, oob_per_chunk);
			if (oob && (page_layout==MODE_MULTI_SPARE)) {
				oob_buf += oob_per_chunk;
			}
		}
	}

	/* commit write */
	mv_nand_cmd_request();
	ctrl = NDCB0_CMDX_NWRITE | NDCB0_CS(board->ncs) | NDCB0_CMD_NAKED_CMD | NDCB0_NC 
		| NDCB0_ADDRC(board->addr_cycle) | NDCB0_CMD2(0xFF) | NDCB0_CMD1(0x10);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ctrl);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0);

	if( wait_status_bit_set(board->ncs ? MV_NAND_NDSR_RDY1 : MV_NAND_NDSR_RDY0) )
		return -1;
	
	if( wait_status_bit_set(board->ncs ? (MV_NAND_NDSR_CS1_CMDD| MV_NAND_NDSR_CS1_PAGED):
		(MV_NAND_NDSR_CS0_CMDD| MV_NAND_NDSR_CS0_PAGED)) )
		return -1;

	mv_nand_disable();
	/* we just add a patch here, would be remove in the future.
	 * The interface of nand should be: (all the other function should call this interfaces)
	 * int mv_nand_read_page(uint64_t addr, uint8_t *data, uint8_t *oob);
	 * int mv_nand_write_page(uint64_t addr, const uint8_t *data, const uint8_t *oob);
	 */
	if (1) {
		int ret = 0;
		do {
			ret = mv_nand_read_status();
		}while(!(ret & 0x40));
		return ret & 0x01;
	}
	return 0;
}

#if defined(__UBOOT__)
//buffer should not contain oob
//page_size must be greater than 2048
int mv_nand_write_multi_plane(loff_t pos, char *buf, long size)
{
	int i,j, k;
	int cs=0;
	struct mv_nand_data *board = &nand_data;
	int blk_size, page_size, chk_size;
	int page_shift;
	int row_addr;
	int repeat;
	int oob_per_chunk = 32;
	char oob_buf[32];
	int page_layout;
	int planes;
	unsigned check_sum_w, check_sum_r;
	int start, end;
	char* curr_buf;
	loff_t curr_pos;
	int pages_per_blk;
	int dst_4321, dst_xxx5;
	char *tmp_buf;

	check_sum_w = crc32(0, (unsigned char*)buf, size);

	start = get_timer(0);

	mv_nand_preinit_rw();
	mv_nand_disable();
	mv_nand_start();

	blk_size = board->szofblk;
	page_size = board->szofpg;
	chk_size = board->chunk_size;
	page_shift = board->page_shift;
	page_layout = board->conspare;
	planes=2;

	if(page_size < 2048) {
		printf("multi_plane write for page_size %d is not supported.\n", page_size);
		return -1;
	}

//	printf("pos=%Lx, size=%x.\n", pos, size);
	if(( pos % blk_size ) || (size % (2*blk_size))) {
		printf("pos=%Lx, size=%x, alignment error.\n", pos, (int)size);
		return -1;
	}
	
	repeat= size/(planes*blk_size);
	pages_per_blk =blk_size/page_size;

	memset(oob_buf, 0xff, sizeof(oob_buf));
	curr_buf = buf;
	curr_pos = pos; //firsr plane
	for(k=0;k<repeat;k++) {
		curr_pos += blk_size; //make plane bit high
		for(i=0;i<pages_per_blk;i++)
		{

			//		printf("%u\n", i);
			tmp_buf = curr_buf;

			//first plane
			//		printf("1\n");
			NAKED_CMD(cs, 0x80);
			//		printf("2\n");
			NAKED_ADDR(cs, 5, 0, 0, 0);
			//		printf("3\n");
			j = page_size/chk_size;
			while(j-->0) {
				if((j==0) || (page_layout==MODE_MULTI_SPARE)) {
					NAKED_WR_DATA(cs, chk_size + oob_per_chunk);
				} else {
					NAKED_WR_DATA(cs, chk_size);
				}
				/* write data */
				mv_nand_write_buf(tmp_buf, chk_size);
				tmp_buf += chk_size; 
				/* write oob */
				if((j==0) || (page_layout==MODE_MULTI_SPARE))
					mv_nand_write_buf(oob_buf, oob_per_chunk);
			}
			//		printf("4\n");
			NAKED_CMD(cs, 0x11);

			tmp_buf = curr_buf + blk_size;
//			printf("curr_pos=0x%Lx, A20=%Lx\n", curr_pos, curr_pos & (1<<19));
			row_addr = (curr_pos >> page_shift);
			dst_4321 = (row_addr << 16) & 0xFFFF0000;
			dst_xxx5 = (row_addr >> 16) & 0xFF;

			if( wait_status_bit_set(cs ? MV_NAND_NDSR_RDY1 : MV_NAND_NDSR_RDY0) )
				return -1;

			//second plane
			//		printf("5\n");
			NAKED_CMD(cs, 0x81);
			//		printf("6\n");
			NAKED_ADDR(cs, 5, dst_4321, dst_xxx5, 0);
			j = page_size/chk_size;
			while(j-->0) {
				if((j==0) || (page_layout==MODE_MULTI_SPARE)) {
					NAKED_WR_DATA(cs, chk_size + oob_per_chunk);
				} else {
					NAKED_WR_DATA(cs, chk_size);
				}
				/* write data */
				mv_nand_write_buf(tmp_buf, chk_size);
				tmp_buf += chk_size; 
				/* write oob */
				if((j==0) || (page_layout==MODE_MULTI_SPARE))
					mv_nand_write_buf(oob_buf, oob_per_chunk);
			}
			//		printf("7\n");
			NAKED_CMD(cs, 0x10);
			curr_pos += page_size;
			curr_buf += page_size;

			//		printf("8\n");
			if( wait_status_bit_set(cs ? MV_NAND_NDSR_RDY1 : MV_NAND_NDSR_RDY0) )
				return -1;
		}
		curr_buf += blk_size;
	}
	mv_nand_disable();
	
	end = get_timer(start);
	printf("time used: %u ms.\n", end);

	for(i=0;i<size/page_size;i++) {
		curr_buf = buf + i*page_size;
		curr_pos = pos + i*page_size; //firsr plane
		mv_nand_read_page(curr_pos, curr_buf);
	}

	check_sum_r = crc32(0, (unsigned char*)buf, size);
	printf("check_sum_w=%08x, check_sum_r=%08x\n", check_sum_w, check_sum_r);
	if(check_sum_w != check_sum_r)
		return -1;
	return 0;
}
#endif /* #if defined(__UBOOT__) */

static int mv_nand_write512_ecc(int srcx, char *buf)
{
	int ctrl ;
	unsigned int dst, dst_4321, dst_xxx5;
	struct mv_nand_data *board = &nand_data;
	mv_nand_preinit_rw();
	mv_nand_disable();
	mv_nand_start();
	
	mv_nand_cmd_request();
	ctrl = 0x022c1080 | (board->ncs ? MV_NAND_NDCB_CSEL : 0);
	dst = srcx;
	dst_4321 = ((dst & 0xFF) | (dst >> (board->page_shift - 8)));
	dst_xxx5 = 0;
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ctrl);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, dst_4321);	
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, dst_xxx5);	
	if( wait_status_bit_set(MV_NAND_NDSR_WRDREQ) )
		return -1;
	
	mv_nand_write_buf(buf, board->szofpg);
	mv_nand_write_buf(buf + board->szofpg, board->oobsize/2);
	if( wait_status_bit_set(board->ncs? MV_NAND_NDSR_RDY1 : MV_NAND_NDSR_RDY0) )
		return -1;
	
	if( wait_status_bit_set(board->ncs ? ( MV_NAND_NDSR_CS1_PAGED) : (MV_NAND_NDSR_CS0_PAGED)) )
		return -1;
	mv_nand_disable();

	return 0;
}

/*
 * return - 0: success, others: fail
 */
int mv_nand_write_page(loff_t ofs, char *buf)
{
	struct mv_nand_data *board = &nand_data;
	int ret = 0;

	if(board->szofpg == 512)
		ret = mv_nand_write512_ecc(ofs, buf);
	else 
		ret = mv_nand_write_large_page(ofs, buf, 1);

	if(ret)
		return ret;

	do {
		ret = mv_nand_read_status();
	}while(!(ret & 0x40));
	
	return ret & 0x01;	
}

/*
 * return - 0, success; 1, fail
 */
int mv_nand_erase(loff_t srcx)
{
	int ctrl;
	unsigned int dst_4321, dst;
	struct mv_nand_data *board = &nand_data;

	mv_nand_preinit_exrw();
	mv_nand_disable();
	mv_nand_start();
	mv_nand_cmd_request();

	dst = (int)(srcx >> board->page_shift);
	ctrl= 0x005BD060 | (board->ncs ? MV_NAND_NDCB_CSEL : 0);

	dst_4321 = (dst & ~((board->szofblk / board->szofpg) - 1));
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDCB0_OFFSET, ctrl);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDCB0_OFFSET, dst_4321);	
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDCB0_OFFSET, 0);
	if( wait_status_bit_set(board->ncs ? MV_NAND_NDSR_CS1_CMDD : MV_NAND_NDSR_CS0_CMDD) )
		return -1;
	ctrl = 0;

	do {
		ctrl = mv_nand_read_status();
	}while(!(ctrl & 0x40));
	return ctrl & 0x01;

}

void mv_nand_select_chip(int i)
{
	return;
}

/*
 * oob, 1 buffer contain oob, 0 buffer doesn't contain oob
 * return:
 *      0: sucess
 *      1: uncorrectable error
 *      -1: fail
 */
int mv_nand_read_large_page(loff_t srcx , char *buf, int oob)
{
	int ctrl , i, j, uncerr=0;
	unsigned int row_addr, page_shift;
	unsigned int dst_4321, dst_xxx5;
	struct mv_nand_data *board = &nand_data;
	int page_size = board->szofpg;
	int bch_enabled = board->conbch;
	int oob_per_chunk;
	int page_layout = board->conspare;
	char* oob_buf;
	char tmp_oob_buf[128];

	char* out_buf = buf;
	unsigned int page_addr = (unsigned int)(srcx >> board->page_shift);
#ifdef SUPPORT_RANDOMIZATION
	int randomized = 0;
#endif // SUPPORT_RANDOMIZATION

//	printf("read @0x%08X\n", srcx);
//	printf("%s: 0\n", __FUNCTION__);
	page_shift = board->page_shift;

	mv_nand_preinit_rw();
	mv_nand_disable();
	mv_nand_start();
	
	mv_nand_cmd_request();
	ctrl = NDCB0_CS(board->ncs) | NDCB0_CMD_NAKED_CMD | NDCB0_NC 
		| NDCB0_ADDRC(board->addr_cycle) | NDCB0_CMD2(0xFF) | NDCB0_CMD1(0);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ctrl);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0);
	
	mv_nand_cmd_request();
	ctrl = NDCB0_CS(board->ncs) | NDCB0_CMD_NAKED_ADDR | NDCB0_NC 
		| NDCB0_ADDRC(board->addr_cycle) | NDCB0_CMD2(0xFF) | NDCB0_CMD1(0xFF);

	row_addr = (srcx >> page_shift);
	dst_4321 = (row_addr << 16) & 0xFFFF0000;
	dst_xxx5 = (row_addr >> 16) & 0xFF;

	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ctrl);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, dst_4321);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, dst_xxx5);
	
	mv_nand_cmd_request();
	ctrl = NDCB0_CS(board->ncs) | NDCB0_CMD_NAKED_CMD | NDCB0_NC 
		| NDCB0_ADDRC(board->addr_cycle) | NDCB0_CMD2(0xFF) | NDCB0_CMD1(0x30);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ctrl);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0);

	i = (page_size - 1 + board->chunk_size)/board->chunk_size;
	if(oob)
		oob_buf = buf + page_size;
	else
		oob_buf = tmp_oob_buf;
	if(page_layout==MODE_MULTI_SPARE)
		oob_per_chunk = board->oobsize/i;
	else
		oob_per_chunk = board->oobsize;

//	printf("%s: 3\n", __FUNCTION__);
	while(i-->0){
		mv_nand_cmd_request();
		ctrl = NDCB0_CMDX_NREAD |NDCB0_LENOVRD | NDCB0_CS(board->ncs) | NDCB0_NC 
			| NDCB0_ADDRC(board->addr_cycle) | NDCB0_CMD2(0xFF) | NDCB0_CMD1(0x30);
		MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ctrl);
		MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0);
		MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, 0);
		if((i==0) || (page_layout==MODE_MULTI_SPARE))
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET,
				NDCB3_NDLENCNT(board->chunk_size + oob_per_chunk));
		else
			MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET,
				NDCB3_NDLENCNT(board->chunk_size));

		/*read data*/
		if(bch_enabled) {
			for(j = 0 ;j < board->chunk_size/32;j++) {
				if( wait_status_bit_set(MV_NAND_NDSR_RDDREQ) )
					return -1;
				mv_nand_read_buf(buf, 32);
				buf += 32;
			}
		} else {
			if( wait_status_bit_set(MV_NAND_NDSR_RDDREQ) )
				return -1;
			mv_nand_read_buf(buf, board->chunk_size);
			buf += board->chunk_size;
		}
		/* read oob */
		if((i==0) || (page_layout==MODE_MULTI_SPARE)) {
			if( bch_enabled && wait_status_bit_set(MV_NAND_NDSR_RDDREQ) )
				return -1;
			mv_nand_read_buf(oob_buf, oob_per_chunk);
			oob_buf += oob_per_chunk;
		}
		/* read state */	
		MV_NAND_REG_READ_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDSR_OFFSET ,&ctrl);
		ctrl &= MV_NAND_NDSR_UNCERR;
		MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE,MV_NAND_NDSR_OFFSET, ctrl);
		uncerr |= ctrl;
	}
//	printf("%s: 5\n", __FUNCTION__);
	if( wait_status_bit_set(board->ncs ? (MV_NAND_NDSR_CS1_PAGED):(MV_NAND_NDSR_CS0_PAGED )) )
		return -1;
	if( wait_status_bit_set(board->ncs ? (MV_NAND_NDSR_CS1_CMDD) : (MV_NAND_NDSR_CS0_CMDD )) )
		return -1;

	mv_nand_disable();

	if(uncerr) {
		// TODO:
		// reading a blank page would cause uncorrectable error, and some bytes in
		// perticular postions are wrong. This function will check these errors.
		// But this function assume that all the data readbadk are 0xff, if some
		// bytes are not 0xff, we are not sure what we will get at present. Need to
		// clarify it.
		if (is_page_blank(out_buf, out_buf + page_size, page_size))
		{
			TRACE_LOG("Read blank buffer\n");
			memset(out_buf, 0xFF, page_size);
			if (oob)
				memset(out_buf + page_size, 0xFF, 32);
			return 0;
		}
		return -1;
	}

#ifdef SUPPORT_RANDOMIZATION
	DUMP_8_BYTES("ReadBefore:", out_buf);
	randomized = nand_read_page_post_process(page_addr, 
		out_buf, 
		oob ? (out_buf + page_size) : NULL, 
		out_buf,  
		oob ? (out_buf + page_size) : NULL);
	TRACE_LOG("Read Page(0x%08X): %s\n", (int)srcx, randomized ? "RANDOMIZED" : "UNRANDOMIZED");
	DUMP_8_BYTES("ReadReal:", out_buf);
#endif // SUPPORT_RANDOMIZATION

	return 0;
}

static int mv_nand_read512_ecc(int srcx, char *buf)
{
	int ctrl;
	unsigned int addr, dst_4321, dst_xxx5;
	struct mv_nand_data *board = &nand_data;
	mv_nand_preinit_rw();
	mv_nand_disable();
	mv_nand_start();
	
	mv_nand_cmd_request();
	ctrl = 0x001c3000 | (board->ncs ? MV_NAND_NDCB_CSEL : 0);
	addr = srcx;
	dst_4321 = ((addr & 0xFF) | (addr >> (board->page_shift - 8)));
	dst_xxx5 = 0;
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, ctrl);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, dst_4321);	
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDCB0_OFFSET, dst_xxx5);
	if( wait_status_bit_set(MV_NAND_NDSR_RDDREQ) )
		return -1;
	
	mv_nand_read_buf(buf, board->szofpg);
	mv_nand_read_buf(buf + board->szofpg, board->oobsize/2);
	
	if( wait_status_bit_set(board->ncs ? (MV_NAND_NDSR_CS1_PAGED) : (MV_NAND_NDSR_CS0_PAGED)) )
		return -1;
	
	if( wait_status_bit_set(board->ncs ? (MV_NAND_NDSR_CS1_CMDD) : (MV_NAND_NDSR_CS0_CMDD )) )
		return -1;
	
	mv_nand_disable();
	return 0;
}

int mv_nand_read_page(loff_t ofs,char *buf)
{
	struct mv_nand_data *board = &nand_data;
	int ret;

	if(board->szofpg == 512)
		ret = mv_nand_read512_ecc(ofs, buf);
	else 
		ret = mv_nand_read_large_page(ofs, buf, 1);

	return ret;
}

#if defined(__UBOOT__)
static int mv_nand_read_param_page(char *buf)
{
	mv_nand_preinit_exrw();
	mv_nand_disable();
	mv_nand_start();

	NAKED_CMD(0, 0xEC);
	NAKED_ADDR(0, 1, 0x00, 0, 0);
	udelay(100);
	NAKED_RD_DATA(0, 256);
	if( wait_status_bit_set(MV_NAND_NDSR_RDDREQ) )
		return -1;

	mv_nand_read_buf(buf,8);
	mv_nand_disable();
	return 0;
}

static int mv_nand_read_onfi_id(char *buf)
{
	mv_nand_preinit_exrw();
	mv_nand_disable();
	mv_nand_start();

	READ_ID(0, 6, 0x00, 0x90);

/*	NAKED_CMD(0, 0x90);
	NAKED_ADDR(0, 1, 0x00, 0, 0);
	udelay(10);
	NAKED_RD_DATA(0, 6);
*/	if( wait_status_bit_set(MV_NAND_NDSR_RDDREQ) )
		return -1;

	mv_nand_read_buf(buf,8);
	mv_nand_disable();
	return 0;
}
#endif /* #if defined(__UBOOT__) */

int mv_nand_read_id(char *buf)
{
	int ctrl ;
	struct mv_nand_data *board = &nand_data;
	mv_nand_preinit_exrw();
	mv_nand_disable();
	mv_nand_start();
	
	mv_nand_cmd_request();
	ctrl = 0x00610090 | (board->ncs? MV_NAND_NDCB_CSEL : 0);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE ,MV_NAND_NDCB0_OFFSET , ctrl);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE ,MV_NAND_NDCB0_OFFSET , 0);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE ,MV_NAND_NDCB0_OFFSET , 0);
	if( wait_status_bit_set(MV_NAND_NDSR_RDDREQ) )
		return -1;

	/* read data */
	mv_nand_read_buf(buf,8);
	mv_nand_disable();
	return 0;
}

#if defined(__UBOOT__)
static void init_timing(struct mv_nand_data * pndata)
{
	int nfc_frequency;
	struct nand_timing * timing = pndata->chip->timing;
	struct ndtr0cs0 reg0;
	struct ndtr1cs0 reg1;
	struct ndredel reg2;
	int max;
	unsigned tmp;

#if 1
	//never try to caculate timing!!!
	pndata->ndtr0cs0 = MV_NAND_NDTR0CS0_VAL;
	pndata->ndtr1cs0 = MV_NAND_NDTR1CS0_VAL;
	pndata->ndredel = MV_NAND_NDREDEL_VAL;
	return;
#endif

	nfc_frequency = GaloisGetFrequency(SOC_FREQ_NFCECC)/2; //MHz
	debug_printf(("NFC clock frequency %d\n", nfc_frequency));

	reg0.u32_ndtr0cs0 = 0;
	reg1.u32_ndtr1cs0 = 0;
	reg2.u32_ndredel = 0;
	//NDTR0CS0
	reg0.SelCntr = 1;
	tmp = nfc_frequency * (timing->tREA - timing->tRP)/1000+ 1;
	if(tmp > ((1<<4) - 1))
		tmp = (1<<4) - 1;
	reg0.Rd_Cnt_Del = tmp;
	debug_printf(("Rd_Cnt_Del= %x\n", reg0.Rd_Cnt_Del));

	tmp = nfc_frequency * timing->tCH/1000;
	if(tmp > ((1<<3) - 1))
		tmp = (1<<3) - 1;
	reg0.tCH = tmp;
	debug_printf(("tCH= %x\n", reg0.tCH));

	tmp = nfc_frequency * timing->tCS/1000;
	if(tmp > ((1<<3) - 1))
		tmp = (1<<3) - 1;
	reg0.tCS = tmp;
	debug_printf(("tCS= %x\n", reg0.tCS));

	tmp = nfc_frequency * timing->tWH/1000;
	if(tmp > ((1<<3) - 1))
		tmp = (1<<3) - 1;
	reg0.tWH = tmp;
	debug_printf(("tWH= %x\n", reg0.tWH));

	tmp = nfc_frequency * timing->tWP/1000;
	if(tmp > ((1<<3) - 1))
		tmp = (1<<3) - 1;
	reg0.tWP = tmp;
	if(reg0.tWP + reg0.tWH < 2)
		reg0.tWP = 2 - reg0.tWH;
	debug_printf(("tWP= %x\n", reg0.tWP));

	reg0.sel_nRE_edge = 0;

	tmp = nfc_frequency * timing->tRH/1000;
	if(tmp > ((1<<3) - 1))
		tmp = (1<<3) - 1;
	reg0.tRH = tmp;
	debug_printf(("tRH= %x\n", reg0.tRH));

	tmp = nfc_frequency * timing->tRP/1000;
	if(tmp > ((1<<4) - 1))
		tmp = (1<<4) - 1;
	reg0.etRP = tmp>>3;
	debug_printf(("etRP= %x\n", reg0.etRP));
	reg0.tRP = tmp&7;
	debug_printf(("tRP= %x\n", reg0.tRP));

	tmp = nfc_frequency * timing->tADL/1000+ reg0.tWP + 4; 
	if(tmp > ((1<<5) - 1))
		tmp = (1<<5) - 1;
	reg0.tADL = tmp;
	debug_printf(("tADL= %x\n", reg0.tADL));

	//NDTR1CS0
	tmp = nfc_frequency * timing->tR/1000- reg0.tCH - 2;
	if(tmp > ((1<<16) - 1))
		tmp = (1<<16) - 1;
	reg1.tR = tmp;
	debug_printf(("tR= %x\n", reg1.tR));

	reg1.wait_mode = 1;
	reg1.PRESCALE = 0;

	tmp = nfc_frequency * timing->tRHW/(1000*16) + 1;
	if(tmp > ((1<<2) - 1))
		tmp = (1<<2) - 1;
	reg1.tRHW = tmp;
	debug_printf(("tRHW= %x\n", reg1.tRHW));

	max = reg0.tWH > reg0.tCH ? reg0.tWH : reg0.tCH; 
	tmp = nfc_frequency * timing->tWHR/1000+ 1 + max;
	if(tmp > ((1<<4) - 1))
		tmp = (1<<4) - 1;
	reg1.tWHR = tmp;
	debug_printf(("tWHR= %x\n", reg1.tWHR));

	tmp = nfc_frequency * timing->tAR/1000+ 1;
	if(tmp > ((1<<4) - 1))
		tmp = (1<<4) - 1;
	reg1.tAR = tmp;
	debug_printf(("tAR= %x\n", reg1.tAR));

	pndata->ndtr0cs0 = reg0.u32_ndtr0cs0;
	pndata->ndtr1cs0 = reg1.u32_ndtr1cs0;
	pndata->ndredel = reg2.u32_ndredel;
	debug_printf(("ndtr0cs0 %08x, ndtr1cs0 %08x, ndredel %08x\n",
				pndata->ndtr0cs0, pndata->ndtr1cs0, pndata->ndredel));
#if 1
	unsigned int i; 
	char temp[8];
	mv_nand_read_id(temp);
	debug_printf(("NAND id "));
	for(i=0;i<8;i++)
		debug_printf(("%02X", temp[i]));
	debug_printf(("\n"));
#endif
}
#endif /* #if defined(__UBOOT__) */

int init_nand_data(struct mv_nand_data * board)
{
	unsigned int ndcr;
	int pages_per_block;
	unsigned int i;

	board->chunk_size = 2048;
	if(board->szofpg== 512) {
		board->addr_cycle = 4;
	} else {
		board->addr_cycle = 5;
	}
	for(i=0;i<16;i++){
		if((unsigned int)(1<<i) == board->szofpg)
			break;
	}
	board->page_shift = i;

	board->ncs = 0;
#ifdef NAND_MODE_MULTI_SPARE
    	board->conspare = MODE_MULTI_SPARE;
#else
	    board->conspare = MODE_SINGLE_SPARE;
#endif

	//oob size avaliable for user
	if(board->szofpg== 512)
		board->oobsize = 16;
	else if(board->conspare == MODE_NONE_SPARE)
		board->oobsize = 0;
	else if(board->conspare == MODE_SINGLE_SPARE)
		board->oobsize = 32;
	else
		board->oobsize = board->szofpg/board->chunk_size * 32;

	board->conecc = 1;
	if(board->szofpg== 512)
		board->conbch = 0;
	else
		board->conbch = 1;
	board->constp = 0;

	pages_per_block = board->szofblk/board->szofpg;
	ndcr = MV_NAND_NDCR_VAL;
	ndcr &= ~(((1 << MV_NAND_NDCR_PG_PER_BLK_BITS) - 1) << MV_NAND_NDCR_PG_PER_BLK_LSB);
	if (pages_per_block == 32)
		ndcr|= (PAGES_PER_BLOCK_32 << MV_NAND_NDCR_PG_PER_BLK_LSB);
	else if (pages_per_block == 64)
		ndcr|= (PAGES_PER_BLOCK_64 << MV_NAND_NDCR_PG_PER_BLK_LSB);
	else if (pages_per_block == 128)
		ndcr|= (PAGES_PER_BLOCK_128 << MV_NAND_NDCR_PG_PER_BLK_LSB);
	else if (pages_per_block == 256)
		ndcr|= (PAGES_PER_BLOCK_256 << MV_NAND_NDCR_PG_PER_BLK_LSB);
	else {
		return -1;
	}
	ndcr &= ~(((1 << MV_NAND_NDCR_PG_SIZE_BITS) - 1) << MV_NAND_NDCR_PG_SIZE_LSB);
	if(board->szofpg != 512)
		ndcr |= 1 << MV_NAND_NDCR_PG_SIZE_LSB;

//	ndcr &= ~(((1 << MV_NAND_NDCR_ID_BYTE_BITS) - 1) << MV_NAND_NDCR_ID_BYTE_LSB);
//	ndcr |= 6 << MV_NAND_NDCR_ID_BYTE_LSB;
	if (board->conecc & 1)
		ndcr |= MV_NAND_NDCR_ECC_EN;
	else
		ndcr &= ~MV_NAND_NDCR_ECC_EN;

	if (board->constp & 1)
		ndcr |= MV_NAND_NDCR_STOP_ON_UNCOR;
	else
		ndcr &= ~MV_NAND_NDCR_STOP_ON_UNCOR;

	board->ndeccctrl = (board->conbch & 0x7F) | (board->t << 24);
	board->ndcr = ndcr;

#if 1	// It's a quick fix here. To use the new timing setting in bootloader
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDTR0CS0_OFFSET, MV_NAND_NDTR0CS0_VAL);
	MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDTR1CS0_OFFSET, MV_NAND_NDTR1CS0_VAL);
	// MV_NAND_REG_WRITE_DWORD(MV_NAND_DFC_REG_BASE, MV_NAND_NDREDEL_OFFSET, MV_NAND_NDREDEL_VAL);
#endif

#ifdef SUPPORT_RANDOMIZATION
	nand_randomizer_init_by_flash_type();
#endif // SUPPORT_RANDOMIZATION
	return 0;
}

#if defined(__UBOOT__)
static int mv_nand_get_flash_type(int busw, int *maf_id,struct mv_nand_flash_dev **rtype)
{
	unsigned int tmp[2];
	int i, dev_id, maf_idx;
	struct mv_nand_flash_dev *type = NULL;
	struct mv_nand_data *board = &nand_data;
	struct nand_param **p_nparam = buildin_nand_list;
	mv_nand_select_chip(0);
	
	mv_nand_read_id((char *)&tmp[0]);
	printf("NAND chip id ");
	for(i=0;i<8;i++)
		printf("%02X", ((char *)tmp)[i]);
	printf("\n");
	
	//new detect method
	while(*p_nparam) {
		if( (strncmp((char*)tmp, &((*p_nparam)->maker), 5) == 0)
			|| (strncmp((char*)tmp, &((*p_nparam)->maker), 6) == 0)) {
			board->chip = *p_nparam;
			board->szofpg = board->chip->page_size;
			board->szofblk = board->chip->block_size;
			board->t = board->chip->t;
			init_nand_data(board);
			init_timing(board);
			real_device_oobsize = board->oobsize;
			chip_size = (long long)board->chip->chip_size * 1024*1024;
			return 0;
		}
		p_nparam++;
	}
	
	printf("NAND is not in the buildin list. Try best to get its parameter.\n");
	//if detect failed, use the old method.
	*maf_id = tmp[0] & 0xff;
	dev_id = (tmp[0] >> 8) & 0xff;
	for (i = 0; nand_flash_ids[i].name != NULL; i++) {
		if ((dev_id == nand_flash_ids[i].id)) {
			type =  &nand_flash_ids[i];
		}
	}
	if (!type)
		return -1;
	chip_size = (long long)(type->chipsize) << 20;

	/* Newer devices have all the information in additional id bytes */
 	if (!type->pagesize) {
		int extid;

		/* it is SAMGSUNG chip and support cache program */
		if((NAND_MFR_SAMSUNG == *maf_id) && ((tmp[0] >> 16)&0x80)) {
			/* The 3rd id byte holds MLC / multichip data */
			/* The 4th id byte is the important one */
			extid = (tmp[0]>>24)&0xff;
			/* Calc pagesize */
			board->szofpg = 2048 << (extid & 0x3);
			extid >>= 4;
			
			/* Calc blocksize. Blocksize is multiples of 64KiB */
			board->szofblk = (128 * 1024) << (extid & 0x03);
			extid >>= 2;
			busw = 1;
		} else if ((NAND_MFR_HYNIX == *maf_id) && ((tmp[0]>>16)&0x80)) {
			/* The 3rd id byte holds MLC / multichip data */
			/* The 4th id byte is the important one */
			extid = (tmp[0]>>24)&0xff;
			/* Calc pagesize */
			board->szofpg = 2048 << (extid & 0x3);
			switch (extid & 0xB0) {
				case 0:
					board->szofblk = 128 * 1024;
					break;
				case 0x10:
					board->szofblk = 2 * 128 * 1024;
					break;
				case 0x20:
					board->szofblk = 4 * 128 * 1024;
					break;
				case 0x30:
					board->szofblk = 6 * 128 * 1024;
					break;
				case 0x80:
					board->szofblk = 8 * 128 * 1024;
					break;
				case 0x90:
					board->szofblk = 2 * 1024 * 1024;
					break;
			}
			real_device_oobsize = 128;
			busw = 1;
		} else if (NAND_MFR_MICRON == *maf_id) {
			board->szofpg = 4096;
			board->szofblk = 4096*256;
		} else {
			/* The 3rd id byte holds MLC / multichip data */
			/* The 4th id byte is the important one */
			extid = (tmp[0]>>24)&0xff;
			/* Calc pagesize */
			board->szofpg = 1024 << (extid & 0x3);
			extid >>= 2;
			/* Calc oobsize */
			extid >>= 2;
			/* Calc blocksize. Blocksize is multiples of 64KiB */
			board->szofblk = (64 * 1024) << (extid & 0x03);
			extid >>= 2;
			/* Get buswidth information */
			busw = (extid & 0x01) ? NAND_BUSWIDTH_16 : 0;
			if((NAND_MFR_SAMSUNG == *maf_id) && (dev_id == 0xd5))
				chip_size = (long long)((64 << ((tmp[1] >> 4) & 0x7)) << 17) * ( 1 << ((tmp[1] >> 2) & 0x3));
		}
	} else {
		/*
		 * Old devices have chip data hardcoded in the device id table
		 */
		if ((NAND_MFR_SAMSUNG == *maf_id) && (dev_id == 0xd5)) {
			int extid;	
			/* The 3rd id byte holds MLC / multichip data */
			/* The 4th id byte is the important one */
			extid = (tmp[0]>>24)&0xff;
			/* Calc pagesize */
			board->szofpg = 1024 << (extid & 0x3);
			extid >>= 2;
			/* Calc oobsize */
			extid >>= 2;
			/* Calc blocksize. Blocksize is multiples of 64KiB */
			board->szofblk = (64 * 1024) << (extid & 0x03);
			extid >>= 2;
			/* Get buswidth information */
			busw = (extid & 0x01) ? NAND_BUSWIDTH_16 : 0;
			chip_size = (long long)((64 << ((tmp[1] >> 4) & 0x7)) << 17) * ( 1 << ((tmp[1] >> 2) & 0x3));
		} else {
			board->szofblk = type->erasesize;
			board->szofpg = type->pagesize;
			if(board->conbch && (board->szofpg == 512)) {
				board->conbch = 0;
			}
		}
	}
	for (maf_idx = 0; nand_manuf_ids[maf_idx].id != 0x0; maf_idx++)
		if ((nand_manuf_ids[maf_idx].id == *maf_id))
			break;

	printf("%s NAND flash.\n",  nand_manuf_ids[maf_idx].name );

    *rtype = type;

#if (defined(BERLIN_SOC_BG2) && !defined(BERLIN_SOC_BG2_Z1))
       board->t = 48;
#else
       board->t = 16;
#endif
	init_nand_data(board);
	real_device_oobsize = board->oobsize;

	board->chip = buildin_nand_list[0];
	init_timing(board);

	return 0;
}

static int mv_nand_scan_ident(int maxchips)
{
	struct mv_nand_flash_dev *type;
	int ret,i, busw = 0, nand_maf_id;
	unsigned int tmp[2];
	ret = mv_nand_get_flash_type(busw, &nand_maf_id ,&type);
	if(ret)
		return ret;
	for (i = 1; i < maxchips; i++) {
		mv_nand_select_chip(i);
		mv_nand_read_id((char *)&tmp[0]);
		if (nand_maf_id != (tmp[0]&0xff) ||
				type->id != ((tmp[0]>>8)&0xff))
			break;
	}

	printf("Total size %LuB, block size: %uB, page size: %uB\n",
		      mv_nand_chip_size(), mv_nand_block_size(), mv_nand_page_size());
	printf("oob size: %uB\n", mv_nand_oob_size());

	return 0;
}

static int mv_nand_scan(int maxchips)
{
	int ret;
	ret = mv_nand_scan_ident(maxchips);
	return ret;
}


/*
 * Probe for the NAND device.
 */ 
int mv_nand_probe(void)
{
	int res = 0;

	mv_nand_chip_init();
	//printf("reset nand flash before proble\n") ;
	if(mv_nand_reset_chip(0)) {
		res = -1;
		goto out;
	}
	//printf("reset nand flash before proble\n") ;
	if (mv_nand_scan(1)) {
		res = -1;
		goto out;
	}
	if(mv_nand_chip_reinit()) {
		res = -1;
		goto out;
	}
out:
	return res;
}
#endif /* #if defined(__UBOOT__) */

/*
 * 100% finished.
 * Read within one block, data is copied only once.
 * nand_start: no alignment requirement
 * data_size: no alignement requirement, but data_size + nand_start
 *              must not exceed the block boundary
 * return value: 
 *   -1  when error
 *   actual data has been read, maybe little than data_size
 */
int mv_nand_read_block(long long nand_start, char* data_buf, int data_size)
{
	long long pos;
	int page_size, block_size;
	char *buf;
	long long mask;
	int oddment;
	int size;
    int ret;

	page_size = mv_nand_page_size();
	block_size = mv_nand_block_size();
	mask = (~(page_size- 1));

	//check whether exceed block end
	if((nand_start & (block_size-1)) + data_size > block_size)
		data_size = block_size - (nand_start & (block_size - 1));
	//
	size = data_size;
	buf = data_buf;
	pos = nand_start & mask;
	oddment = nand_start & ~mask;
	if(oddment) {
		oddment = page_size - (nand_start & ~mask);
		oddment = oddment < data_size ? oddment : data_size;
		ret = mv_nand_read_large_page(pos, mvnand_page_buffer, 0);
        if(ret == 1)
            printf("Uncorrectable error @ 0x%08x\n", (int)pos);
		if(ret)
			return -1;
		memcpy(buf, &mvnand_page_buffer[nand_start & ~mask], oddment);
		pos += page_size;
		buf += oddment;
	}
	data_size -= oddment;
	if(!data_size)
		return size;
	while(data_size >= page_size) {
		ret = mv_nand_read_large_page(pos, buf, 0);
        if(ret == 1)
            printf("Uncorrectable error @ 0x%08x\n", (int)pos);
		if(ret)
			return -1;
		pos += page_size;
		data_size -= page_size;
		buf += page_size;
	}

	if(data_size) {
		ret = mv_nand_read_large_page(pos, mvnand_page_buffer, 0);
        if(ret == 1)
            printf("Uncorrectable error @ 0x%08x\n", (int)pos);
		if(ret)
			return -1;
		memcpy(buf, mvnand_page_buffer, data_size);
	}

	return size;
}

/*
 * 70% finished.
 * nand_start: must be page_aligned
 * data_size: no alignement requirement, but data_size + nand_start
 * must not exceed the block boundary
 * return:
 *      0: sucess
 *      1: fail
 *      
 */
int mv_nand_write_block(loff_t nand_start, char* data_buf, int data_size, int verify)
{
	int page_size, block_size;
	loff_t mask, block_mask;
//	unsigned check_sum_w, check_sum_r;
	int size = data_size;
	char *buf = data_buf;
	loff_t pos = nand_start;

//	check_sum_w = crc32(0, (unsigned char*)data_buf, data_size);
//	printf("nand_start: 0x%09Lx\n", nand_start);
	page_size = mv_nand_page_size();
	block_size = mv_nand_block_size();
	mask = (~(page_size- 1));
	block_mask = (~(block_size - 1));
	if(pos & (page_size - 1))
		return -1;
	if( (pos & block_mask) !=
		((pos + size -1) & block_mask) )
		return -1;
	while(size >= page_size) {
		if(mv_nand_write_large_page(pos, buf, 0))
			return -1;
		pos += page_size;
		size -= page_size;
		buf += page_size;
	}

	if(size) {
		memcpy(mvnand_page_buffer, buf, size);
		memset(&mvnand_page_buffer[size], 0xff, page_size-size);
		if(mv_nand_write_large_page(pos, mvnand_page_buffer, 0))
			return -1;
	}
/*
	mv_nand_read_block(nand_start, (char*)0x1000000, data_size);

	compare_buff(data_buf, 0x1000000, data_size);
	check_sum_r = crc32(0, (unsigned char*)0x1000000, data_size);
	if(check_sum_w != check_sum_r) {
		printf("check_sum_w=%08x, check_sum_r=%08x\n", check_sum_w, check_sum_r);
		return -1;
	}
*/
	return 0;
}

/*
 * 80% finished. start needs to be aligned, dual plane is not supported.
 * bad block will be skipped automatically
 * nand_start: must be block aligned
 * data_size: no requirement
 *  return:
 *       -1: fail
 *       other: current read position.
 */
loff_t mv_nand_read_skip_bad(loff_t nand_start, char* data_buf, int data_size)
{
	int block_size;
	loff_t pos, size, block_mask;
	char *buf;
	int rdsz=0;

    if(!data_size)
        return nand_start;

	block_size = mv_nand_block_size();
	block_mask = ~((loff_t)block_size - 1);

	//force block aligned
	if(nand_start & (block_size-1))
		return -1;

	pos = nand_start;
	buf = data_buf;
	size = data_size;

	while(size > 0) {
		if(mv_nand_block_bad(pos, 0)) {
			pos += block_size;
			continue;
		}
		rdsz = mv_nand_read_block(pos, buf, size);
		if(rdsz < 0)
			return -1;
		size -= rdsz;
		buf += rdsz;
		pos += block_size;
	}

	return pos-block_size+rdsz;
}

/*
 * 100% finished.
 * suppose image length is [data_size], it is stored in the partition starts from
 * [start] ends at [end]. This function is to find out the next block bihind this image.
 */
loff_t get_next_addr(loff_t start, loff_t end, unsigned int data_size)
{
    loff_t i;
    unsigned int j;
    unsigned int block_size;

    block_size = nand_data.szofblk;
    j = (data_size + block_size - 1) & ~(block_size - 1);
    for(i=start;i<end;i+=block_size) {
        if(j==0)
            break;
        if(mv_nand_block_bad(i, 0)) {
//            printf("Bad block found @0x%09Lx.\n", i);
            continue;
        }
        j-=block_size;
    }
    if(i >= end)
        return -1;

    return i;
}
