#include "common.h"
#include "mv_nand.h"

int lgpl_printf(const char *format, ...);

#define DO_TRACE_LOG				(0)

#if DO_TRACE_LOG
#define TRACE_LOG				lgpl_printf
#else
#define TRACE_LOG(fmt...)
#endif


extern struct mv_nand_data nand_data ;

int is_blank(const void *buf, size_t size)
{
	size_t i;
	const unsigned int *data = (const unsigned int *)buf;
	size /= sizeof(data[0]);

	for (i = 0; i < size; i++)
		if (data[i] != 0xffffffff)
			return 0;
	return 1;
}

int NFlash_Init()
{
	TRACE_LOG("NFlash_Init() -> init_nand_data()\n");
	init_nand_data(&nand_data);

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
	mv_nand_reset_chip(0);
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
	return mv_nand_block_bad(addr, 0);
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
	TRACE_LOG("NFlash_PageRead(addr=0x%08X, pbuf=0x%08X)\n", (int)addr, (int)pbuf);
	return mv_nand_read_large_page(addr, pbuf, 0);
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
	TRACE_LOG("NFlash_PageWrite(addr=0x%08X, pbuf=0x%08X)\n", (int)ofs, (int)buf);
	return mv_nand_write_large_page(ofs, buf, 0);
}

/********************************************************
 * FUNCTION: erase a block
 * PARAM: ofs - flash page memory address
 * RETURN: 0 - succeed
 *         -1 - fail
 *******************************************************/
inline int NFlash_Erase(loff_t ofs)
{
	return mv_nand_erase(ofs);
}

/********************************************************
 * FUNCTION: mark one block as bad
 * PARAM: ofs - flash page memory address
 * RETURN: 0 - succeed
 *         -1 - fail
 *******************************************************/
inline int NFlash_Markbad(loff_t ofs)
{
	return mv_nand_mark_badblock(ofs, 0);
}
