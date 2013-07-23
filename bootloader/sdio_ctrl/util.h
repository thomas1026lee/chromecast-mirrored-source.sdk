/*
 * (C) Copyright Marvell Semiconductors,Inc 2006 All rightes reserved
 */
 
#ifndef	__UTIL_H__
#define	__UTIL_H__

//macro for memory read/write
#define BFM_HOST_Bus_Write32(offset, val) ((*(unsigned int*)(offset))=val)
#define BFM_HOST_Bus_Read32(offset, holder)	((*(unsigned int*)(holder))=(*(unsigned int*)(offset)))
#define BFM_HOST_Bus_Write16(offset, val) ((*(unsigned short*)(offset))=val)
#define BFM_HOST_Bus_Read16(offset, holder)	((*(unsigned short*)(holder))=(*(unsigned short*)(offset)))
#define BFM_HOST_Bus_Write8(offset, val) ((*(unsigned char*)(offset))=val)
#define BFM_HOST_Bus_Read8(offset, holder)	((*(unsigned char*)(holder))=(*(unsigned char*)(offset)))

enum PRINT_LEVEL
{
        PRN_NONE=0,
        PRN_ERR,
        PRN_RES,
        PRN_INFO,
        PRN_DBG

};


#endif	/* __UTIL_H__	*/
