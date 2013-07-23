#ifndef __DEBUG_H
#define	__DEBUG_H

#include "com_type.h"

enum PRINT_LEVEL
{
	PRN_NONE=0,
	PRN_ERR,
	PRN_RES,
	PRN_INFO,
	PRN_DBG

};

#define	DEBUG_LEVEL		PRN_DBG


#if 0
//#define	dbg_printf(level, fmt, ...) if(level<=DEBUG_LEVEL)lgpl_printf(fmt, ...)
#define	dbg_printf(lel, ...)	\
do { \
	if (lel <= DEBUG_LEVEL) { \
		lgpl_printf(__VA_ARGS__); \
		} \
	} \
} while(0)
#endif

#endif
