/*******************************************************************************
*                Copyright 2007, MARVELL SEMICONDUCTOR, LTD.                   *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL.                      *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
*                                                                              *
* MARVELL COMPRISES MARVELL TECHNOLOGY GROUP LTD. (MTGL) AND ITS SUBSIDIARIES, *
* MARVELL INTERNATIONAL LTD. (MIL), MARVELL TECHNOLOGY, INC. (MTI), MARVELL    *
* SEMICONDUCTOR, INC. (MSI), MARVELL ASIA PTE LTD. (MAPL), MARVELL JAPAN K.K.  *
* (MJKK), MARVELL ISRAEL LTD. (MSIL).                                          *
*******************************************************************************/


////////////////////////////////////////////////////////////////////////////////
//! \file sys_memmap.h
//! \brief system level memory map, define memory block address for different CPUs.
//!	
//!	
//!	
//! Purpose:
//!	   define memory address for CPUs
//!
//!	Version, Date and Author :
//!		V 1.00,	7/16/2007,	Fenglei Wang
//!
//! Note:
////////////////////////////////////////////////////////////////////////////////

#ifndef _SYS_MEMMAP_H_
#define _SYS_MEMMAP_H_

#include "memmap.h"

#define GALOIS_CPU1_CACHE_START		(MEMMAP_DRAM_CACHE_BASE)
#if defined(BERLIN)
#define GALOIS_CPU1_CACHE_SIZE		0x16000000
#else
#define GALOIS_CPU1_CACHE_SIZE		0x0e000000
#endif

#define GALOIS_CPU0_CACHE_START		( GALOIS_CPU1_CACHE_START + GALOIS_CPU1_CACHE_SIZE )
#if defined(BERLIN)
#define GALOIS_CPU0_CACHE_SIZE		0x06000000
#else
#define GALOIS_CPU0_CACHE_SIZE		0x0e000000
#endif

#define GALOIS_CACHE_ADDR_END		( GALOIS_CPU0_CACHE_START + GALOIS_CPU0_CACHE_SIZE )

#if GALOIS_CACHE_ADDR_END > ( MEMMAP_DRAM_CACHE_BASE + MEMMAP_DRAM_CACHE_SIZE )
#error Error: cached memory address out of range!!!
#endif

#define GALOIS_NON_CACHE_START		( MEMMAP_DRAM_UNCACHE_BASE | (GALOIS_CACHE_ADDR_END))
#define GALOIS_NON_CACHE_SIZE		0x04000000

#define CACHE_ADDR_MASK				(MEMMAP_DRAM_UNCACHE_BASE -1)
#define NON_CACHE_ADDR( x )			( (void*)( ( (unsigned int)(x) & CACHE_ADDR_MASK) | MEMMAP_DRAM_UNCACHE_BASE) )

#define CACHE_ADDR( x )				( (void*)( (unsigned int)(x) & CACHE_ADDR_MASK) )

#endif	// #ifndef _SYS_MEMMAP_H_
