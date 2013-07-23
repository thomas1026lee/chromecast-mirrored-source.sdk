#ifndef	__COMMON_H__
#define	__COMMON_H__
#include "io.h"
#include "string.h"
#include "lgpl_printf.h"
#include <util.h>

typedef long long loff_t;

#define TRUE		1
#define FALSE		0

#define	MAGIC_NUMBER			(0xD2ADA3F1)

#define COMMAND_SIZE	5
#define _RESERVED_		0

#define IMAGE3_DDR_ADDR		IMAGE3_DDR_ADDRESS

#define reg(addr)	(*((volatile int*)(addr)))

#undef  DEBUG
#ifdef	DEBUG
#define debug(fmt,args...)	printf (fmt ,##args)
#define debugX(level,fmt,args...) if (DEBUG>=level) printf(fmt,##args);
#else
#define debug(fmt,args...)
#define debugX(level,fmt,args...)
#endif	/* DEBUG */


#endif	/* __COMMON_H__	*/
