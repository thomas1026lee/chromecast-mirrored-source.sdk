#ifndef _NFLASH_DRV_H_
#define _NFLASH_DRV_H_

#ifdef __cplusplus__
extern "C" {
#endif

int is_blank(const void *buf, size_t size);

/********************************************************
 * FUNCTION: initialize NFC with device configuration
 * PARAM: pconfig - device specific configuration
 * RETURN: none
 *******************************************************/
int NFlash_Init();

/********************************************************
 * FUNCTION: reset device
 * PARAM: none
 * RETURN: 0 - succeed
 *        -1 - fail
 *******************************************************/
int NFlash_ResetDevice(void);

/********************************************************
 * FUNCTION: return the block status, good or bad
 * PARAM: addr - flash page memory address
 * RETURN: 0 - good block
 *        1 - bad block
 *******************************************************/
int is_block_bad(loff_t addr);

/********************************************************
 * FUNCTION: read a page 
 * PARAM: addr - flash page memory address
 *        pbuf - buffer to hold data
 * RETURN: 0 - succeed
 *         -1 - fail
 *******************************************************/
int NFlash_PageRead(loff_t addr, int *pbuf);

/********************************************************
 * FUNCTION: write a page 
 * PARAM: off - flash page memory address
 *        buf - buffer to hold data
 * RETURN: 0 - succeed
 *         -1 - fail
 *******************************************************/
int NFlash_PageWrite(loff_t ofs, int *buf);

/********************************************************
 * FUNCTION: erase a block
 * PARAM: ofs - flash page memory address
 * RETURN: 0 - succeed
 *         -1 - fail
 *******************************************************/
int NFlash_Erase(loff_t ofs);

/********************************************************
 * FUNCTION: mark one block as bad
 * PARAM: ofs - flash page memory address
 * RETURN: 0 - succeed
 *         -1 - fail
 *******************************************************/
int NFlash_Markbad(loff_t ofs);

#ifdef __cplusplus__
}
#endif

#endif
