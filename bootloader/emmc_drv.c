#include "common.h"
#include "nand_priv.h"

int lgpl_printf(const char *format, ...);

#define DO_TRACE_LOG				(0)

#if DO_TRACE_LOG
#define TRACE_LOG				lgpl_printf
#else
#define TRACE_LOG(fmt...)
#endif

int do_emmcread(unsigned long long start, unsigned int blks, unsigned char * buffer);
int do_emmcwrite(unsigned long long offset, unsigned int blks, unsigned char * emmc_buffer);
int do_emmcerase(unsigned long long offset, unsigned int blks);

extern struct mv_nand_data nand_data ;

int is_blank(const void *buf, size_t size)
{
	size_t i;
	const unsigned int *data = (const unsigned int *)buf;
	size = size / sizeof(*data);

	if (data[0] != 0x00000000 && data[0] != 0xffffffff)
		return 0;

	for (i = 0; i < size; i++)
		if (data[i] != data[0])
			return 0;
	return 1;
}


int NFlash_Init()
{
	TRACE_LOG("NFlash_Init() -> init_nand_data()\n");
	// nand_data.szofpg = 512;
	// nand_data.szofblk = 524288;

    return 0;
}

/********************************************************
 * FUNCTION: reset device
 * PARAM: none
 * RETURN: 0 - succeed
 *        -2 - fail
 *******************************************************/
int NFlash_ResetDevice(void)
{
	TRACE_LOG("NFlash_ResetDevice() -> mv_nand_reset_chip(0)\n");
	// mv_nand_reset_chip(0);
	return 0;
}

/********************************************************
 * FUNCTION: return the block status, good or bad
 * PARAM: addr - flash page memory address
 * RETURN: 0 - good block
 *        1 - bad block
 *******************************************************/
int is_block_bad(loff_t addr)
{
	TRACE_LOG("is_block_bad(addr=0x%08X)\n", (int)addr);
	return 0;
}

/********************************************************
 * FUNCTION: read a page 
 * PARAM: addr - flash page memory address
 *        pbuf - buffer to hold data
 * RETURN: 0 - succeed
 *         -1 - fail
 *******************************************************/
inline int NFlash_PageRead(loff_t addr, int *pbuf)
{
	unsigned int blk = nand_data.szofpg / 512;
	TRACE_LOG("NFlash_PageRead(addr=0x%08X, pbuf=0x%08X)\n", (int)addr, (int)pbuf);
	
	return do_emmcread(addr, blk, (unsigned char *)pbuf);
}

/********************************************************
 * FUNCTION: write a page 
 * PARAM: off - flash page memory address
 *        buf - buffer to hold data
 * RETURN: 0 - succeed
 *         -1 - fail
 *******************************************************/
inline int NFlash_PageWrite(loff_t ofs, int *buf)
{
	unsigned int blk = nand_data.szofpg / 512;
	TRACE_LOG("NFlash_PageWrite(addr=0x%08X, pbuf=0x%08X)\n", (int)ofs, (int)buf);
	return do_emmcwrite(ofs, blk, buf);
}

/********************************************************
 * FUNCTION: erase a block
 * PARAM: ofs - flash page memory address
 * RETURN: 0 - succeed
 *         -1 - fail
 *******************************************************/
inline int NFlash_Erase(loff_t ofs)
{
	unsigned int blks = nand_data.szofblk / 512;

	return do_emmcerase(ofs, blks);
}

/********************************************************
 * FUNCTION: mark one block as bad
 * PARAM: ofs - flash page memory address
 * RETURN: 0 - succeed
 *         -1 - fail
 *******************************************************/
inline int NFlash_Markbad(loff_t ofs)
{
	return 0;
}
