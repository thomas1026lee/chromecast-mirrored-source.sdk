/*
 * (C) Copyright Marvell Semiconductors,Inc 2006 All rightes reserved
 */
 
#ifndef	__UTIL_H__
#define	__UTIL_H__

void *	UtilMemSet(void *s, int c, int n);
void *	UtilMemCpy(void *s1, const void *s2, int n);
int	UtilMemCmp(const void *s1, const void *s2, int n);
void *	UtilMemAlloc(int size);
void *	UtilMemAllocZ(int size);
void    UtilMemReset(void);

//used for bitmaps that could be more than 32 bits wide
#define BITMASK(b)		(1 << ((b) % 8))
#define BITSLOT(b)		((b) / 8)
#define BITSET(a, b)	((a)[BITSLOT(b)] |= BITMASK(b))
#define BITCLEAR(a, b)	((a)[BITSLOT(b)] &= ~BITMASK(b))
#define BITTEST(a, b)	((a)[BITSLOT(b)] & BITMASK(b))
#define BITNSLOTS(nb)	((nb + 8 - 1) / 8)

#define BFM_HOST_Bus_Write32(offset, val)	((*(UNSG32*)(offset))=val)
#define BFM_HOST_Bus_Read32(offset, holder)	((*(UNSG32*)(holder))=(*(UNSG32*)(offset)))
#define BFM_HOST_Bus_Write16(offset, val)	((*(unsigned short*)(offset))=val)
#define BFM_HOST_Bus_Read16(offset, holder)	((*(unsigned short*)(holder))=(*(unsigned short*)(offset)))
#define BFM_HOST_Bus_Write8(offset, val)	((*(unsigned char*)(offset))=val)
#define BFM_HOST_Bus_Read8(offset, holder)	((*(unsigned char*)(holder))=(*(unsigned char*)(offset)))


#define CMD_BUF_SIZE	2048
#define	CMD_LINE_BUFFER_SIZE		CMD_BUF_SIZE

#define PROMPT "BerlinDebug>"

#define	malloc UtilMemAlloc


#if 0
void *	UtilMemMove(void *dest, const void *src, int n);
#endif

#endif	/* __UTIL_H__	*/
