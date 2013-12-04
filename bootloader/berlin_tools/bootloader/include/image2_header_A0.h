/*******************************************************************************
* Copyright (C) Marvell International Ltd. and its affiliates
*
* Marvell GPL License Option
*
* If you received this File from Marvell, you may opt to use, redistribute and/or
* modify this File in accordance with the terms and conditions of the General
* Public License Version 2, June 1991 (the "GPL License"), a copy of which is
* available along with the File in the license.txt file or by writing to the Free
* Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 or
* on the worldwide web at http://www.gnu.org/licenses/gpl.txt.
*
* THE FILE IS DISTRIBUTED AS-IS, WITHOUT WARRANTY OF ANY KIND, AND THE IMPLIED
* WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY
* DISCLAIMED.  The GPL License provides additional details about this warranty
* disclaimer.
********************************************************************************/

#ifndef	_IMAGE2_HEADER_H_
#define	_IMAGE2_HEADER_H_

#include "version_table.h"

/*
 * bootloader block layout:
 * 0       : Im2-part1
 * 52K     : VERSION TABLE
 * 56K     : cust figo
 * 60K     : cust key
 * 64K     : Im2-part2
 */
#define IMG2_PART1_SIZE		52<<10
#define VT_OFFSET		(IMG2_PART1_SIZE)
#define VT_SIZE			4096
#define FIGO_OFFSET		((VT_OFFSET) + (VT_SIZE))
#define FIGO_SIZE		4096
#define KEY_OFFSET		((FIGO_OFFSET) + (FIGO_SIZE))
#define KEY_SIZE		4096
#define IMG2_PART2_OFFSET	((KEY_OFFSET) + (KEY_SIZE))

typedef struct _gen_header_t_ {
	unsigned int magic;
	unsigned int offset;
	unsigned int length;
	unsigned int exe_addr;
	unsigned int crc;
	unsigned int flag;
} gen_header_t;

typedef struct _image2_header_t {
	char part1[IMG2_PART1_SIZE];
	union{
		version_table_t vt;
		char vt_pad[VT_SIZE];
	};
	union{
		gen_header_t figo;
		char figo_pad[FIGO_SIZE];
	};
	union{
		gen_header_t cust_key;
		char cust_key_pad[KEY_SIZE];
	};
	char part2[];
} image2_header_t;

#endif
