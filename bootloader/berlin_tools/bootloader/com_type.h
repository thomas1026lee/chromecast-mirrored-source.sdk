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
//! \file com_type.h
//! \brief type define for default type
//!	
//! Purpose:
//!	Defines an alias name for default type		
//!
//! Note:
//!	Version, Date and Author :
//!		04/15/2007,	Fang Bao 
//!				Add CYGPKG_KERNEL_THREADS_DESTRUCTORS macro to avoid error if no eCos lib supported
//!		06/24/2008, Fang Bao
//!				Modify MV_CC_(X)SEC_PER_SEC_64 using singed value, add MV_CC_NSEC_PER_SEC_U64 using unsiged value
//!
////////////////////////////////////////////////////////////////////////////////

#ifndef __COMMON_TYPE_H__
#define __COMMON_TYPE_H__

#include "ctypes.h"

typedef unsigned char       UCHAR;
typedef char                CHAR;

typedef UCHAR               BOOL;
typedef UCHAR               BOOLEAN;
typedef short               SHORT;
typedef unsigned short      USHORT;
typedef int                 INT;
typedef unsigned int        UINT; 
typedef long                LONG;
typedef unsigned long       ULONG;
typedef long long			LONGLONG;
typedef unsigned long long	ULONGLONG;
typedef void                VOID;
typedef void*               PTR;
typedef void**              PHANDLE;
typedef void*               HANDLE;
typedef void*               PVOID;

typedef UCHAR               BYTE;

typedef CHAR                INT8;
typedef UCHAR               UINT8;
typedef short               INT16;
typedef unsigned short      UINT16;
typedef int                 INT32;
typedef unsigned int        UINT32; 
typedef long long           INT64;
typedef unsigned long long  UINT64; 
typedef unsigned int        SIZE_T;
typedef unsigned int        size_t;

typedef signed int			HRESULT;

typedef	unsigned int        MV_U32;
typedef	signed int          MV_32;
typedef	unsigned long long  MV_U64;
typedef	signed long long    MV_64;

typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;


#ifndef TRUE
#define TRUE							(1)
#endif
#ifndef FALSE
#define FALSE							(0)
#endif

#ifndef true
#define true							(1)
#endif
#ifndef false
#define false							(0)
#endif

#ifndef True
#define True							(1)
#endif
#ifndef False
#define False							(0)
#endif

#ifndef NULL
#define NULL			((void *)0)
#endif


#define	S_OK			(0x0)

#define uswap_16(x) \
	((((x) & 0xff00) >> 8) | \
	 (((x) & 0x00ff) << 8))
#define uswap_32(x) \
	((((x) & 0xff000000) >> 24) | \
	 (((x) & 0x00ff0000) >>  8) | \
	 (((x) & 0x0000ff00) <<  8) | \
	 (((x) & 0x000000ff) << 24))
#define _uswap_64(x, sfx) \
	((((x) & 0xff00000000000000##sfx) >> 56) | \
	 (((x) & 0x00ff000000000000##sfx) >> 40) | \
	 (((x) & 0x0000ff0000000000##sfx) >> 24) | \
	 (((x) & 0x000000ff00000000##sfx) >>  8) | \
	 (((x) & 0x00000000ff000000##sfx) <<  8) | \
	 (((x) & 0x0000000000ff0000##sfx) << 24) | \
	 (((x) & 0x000000000000ff00##sfx) << 40) | \
	 (((x) & 0x00000000000000ff##sfx) << 56))
#if defined(__GNUC__)
# define uswap_64(x) _uswap_64(x, ull)
#else
# define uswap_64(x) _uswap_64(x, )
#endif

#if __BYTE_ORDER == __LITTLE_ENDIAN
# define cpu_to_le16(x)		(x)
# define cpu_to_le32(x)		(x)
# define cpu_to_le64(x)		(x)
# define le16_to_cpu(x)		(x)
# define le32_to_cpu(x)		(x)
# define le64_to_cpu(x)		(x)
# define cpu_to_be16(x)		uswap_16(x)
# define cpu_to_be32(x)		uswap_32(x)
# define cpu_to_be64(x)		uswap_64(x)
# define be16_to_cpu(x)		uswap_16(x)
# define be32_to_cpu(x)		uswap_32(x)
# define be64_to_cpu(x)		uswap_64(x)
#else
# define cpu_to_le16(x)		uswap_16(x)
# define cpu_to_le32(x)		uswap_32(x)
# define cpu_to_le64(x)		uswap_64(x)
# define le16_to_cpu(x)		uswap_16(x)
# define le32_to_cpu(x)		uswap_32(x)
# define le64_to_cpu(x)		uswap_64(x)
# define cpu_to_be16(x)		(x)
# define cpu_to_be32(x)		(x)
# define cpu_to_be64(x)		(x)
# define be16_to_cpu(x)		(x)
# define be32_to_cpu(x)		(x)
# define be64_to_cpu(x)		(x)
#endif

#endif	// #ifndef __COMMON_TYPE__
