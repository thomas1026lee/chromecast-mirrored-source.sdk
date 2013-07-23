/*******************************************************************************
 * Copyright (C) Marvell International Ltd. and its affiliates
 *******************************************************************************
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
 *******************************************************************************/
/*
 * bootloader memory layout:
 *  0x00600000 - 0x0067C000  496kB vt, drm_img, custkey, bootloader preloaded buffer
 *  0x0067C000 - 0x00680000   16kB 16kB for level 1 translation table base
 *  0x00680000 - 0x009C0000 3328kB code, rodata, data, bss, heap
 *  0x009C0000 - 0x00A00000  256kB stack
 *
 *******************************************************************************/

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(BootLoaderEntry)
SECTIONS
{
	. = 0x00600000;
	__preload_buf_start = .;

	. = 0x00680000 - 0x4000;
	. = ALIGN(0x4000);
	__ttb_base_start = .;

	. = 0x00680000;
	. = ALIGN(4);
	.text :
	{
		*(.text)
	}

	.rodata :
	{
		*(.rodata)
	}

        . = ALIGN(4);
	.data :
	{
		*(.data)
	}
	__exidx_start = .;
	.ARM.exidx   : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) }
	__exidx_end = .;

	. = ALIGN(4);
        .bss :
	{
		*(.bss)
	}

	. = ALIGN(8);
	__heap_start = .;

	. = 0x00A00000 - 0x00040000;
	__heap_end = .;

	. = 0x00A00000;
	. = ALIGN(8);
	__stack_start = .;

}
