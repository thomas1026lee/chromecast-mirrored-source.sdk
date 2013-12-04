#ifndef __MV__NAND_H
#define __MV__NAND_H

#if !defined(__UBOOT__)
#endif /* #if defined(__UBOOT__) */
/* Chip can not auto increment pages */
#define NAND_NO_AUTOINCR	0x00000001
/* Buswitdh is 16 bit */
#define NAND_BUSWIDTH_16	0x00000002
/* Device supports partial programming without padding */
#define NAND_NO_PADDING		0x00000004
/* Chip has cache program function */
#define NAND_CACHEPRG		0x00000008
/* Chip has copy back function */
#define NAND_COPYBACK		0x00000010
/* AND Chip which has 4 banks and a confusing page / block
* assignment. See Renesas datasheet for further information */
#define NAND_IS_AND		0x00000020
/* Chip has a array of 4 pages which can be read without
 * additional ready /busy waits */
#define NAND_4PAGE_ARRAY	0x00000040
/* Chip requires that BBT is periodically rewritten to prevent
 * bits from adjacent blocks from 'leaking' in altering data.
 * This happens with the Renesas AG-AND chips, possibly others.  */
#define BBT_AUTO_REFRESH	0x00000080
/* Chip does not require ready check on read. True
 * * for all large page devices, as they do not support
 * * autoincrement.*/
#define NAND_NO_READRDY		0x00000100
/* Chip does not allow subpage writes */
#define NAND_NO_SUBPAGE_WRITE	0x00000200

/* Options valid for Samsung large page devices */
#define NAND_SAMSUNG_LP_OPTIONS \
	(NAND_NO_PADDING | NAND_CACHEPRG | NAND_COPYBACK)
#define NAND_SMALL_BADBLOCK_POS		5
#define NAND_LARGE_BADBLOCK_POS		0

#define NAND_MFR_AMD		0x01
#define NAND_MFR_TOSHIBA	0x98
#define NAND_MFR_SAMSUNG	0xec
#define NAND_MFR_HYNIX		0xad
#define NAND_MFR_STMICRO	0x20
#define NAND_MFR_NATIONAL	0x8f
#define NAND_MFR_FUJITSU	0x04
#define NAND_MFR_RENESAS	0x07
#define NAND_MFR_MICRON         0x2C

struct mv_nand_flash_dev {
	char *name;
	int id;
	unsigned long pagesize;		/* Bytes */
	unsigned long chipsize;		/* MB */
	unsigned long erasesize;	/* Bytes */
	unsigned long options;
};

struct nand_manufacturers {
	int id;
	char *name;
};


int mv_nand_probe(void);
int mv_nand_read_page(loff_t ofs, char *buf);
int mv_nand_read_large_page(loff_t srcx , char *buf, int oob);
int mv_nand_write_large_page(loff_t srcx, char *buf, int oob);
int mv_nand_block_bad(loff_t ofs, int getchip);
int mv_nand_erase(loff_t srcx);
int mv_nand_write_page(loff_t ofs, char *buf);
int mv_nand_read_id(char *buf);
int mv_nand_mark_badblock(loff_t ofs, int getchip);

unsigned int mv_nand_page_size(void);
unsigned int mv_nand_block_size(void);
unsigned int mv_nand_oob_size(void);
loff_t mv_nand_chip_size(void);
int mv_nand_reset_chip(int getchip);
#endif
