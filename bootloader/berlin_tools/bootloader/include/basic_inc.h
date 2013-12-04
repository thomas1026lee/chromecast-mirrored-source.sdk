#ifndef __BASIC_INC_H__
#define __BASIC_INC_H__
#include "memmap.h"

typedef UNSG32* FIGODRV_HANDLE;
#define MV_FIGODRV_IO_WR32(_a,_d)                  	\
{													\
	(*((volatile UNSG32 *)(_a)) = (UNSG32)(_d));	\
}
#define MV_FIGODRV_IO_RD32(_a)                     ((*((volatile UNSG32 *)(_a))))

//! Read an unsigned 32 bits value from the BIU address 
#define MV_FIGODRV_BIURD32(_hFigoDrv, _pHboAddr, _uVal)										\
{																							\
	(_uVal) = MV_FIGODRV_IO_RD32((UNSG32)(_pHboAddr) + (MEMMAP_TSI_REG_BASE));		\
}

//! Write an unsigned 32 bits value into the BIU address
#define MV_FIGODRV_BIUWR32(_hFigoDrv, _pHboAddr, _uVal)												\
{																						\
	MV_FIGODRV_IO_WR32((UNSG32)(_pHboAddr) + (MEMMAP_TSI_REG_BASE), (_uVal));					\
}
//! Align a 32 bits integer to 32 bytes aligned value
#define ALIGN32(_Size)						((((UNSG32)(_Size)) + 0x1f) & (~0x1f))
#define MV_FIGODRV_SLEEP(_ns)                      do {} while (0)
typedef enum
{
	//! Drive error code, Success
	MV_FIGODRV_OK = 0,

	//! Drive error code, Unknown error
	MV_FIGODRV_ERR = -1,

	//! Drive error code, The given arguments are invalid
	MV_FIGODRV_ERR_INVLAID_ARG = -3,

	//! Drive error code, The given arguments are invalid
	MV_FIGODRV_ERR_INVLAID_STATUS = -4,

} MV_FIGODRV_ERROR_CODE;

enum
{
	//! Security status register address 
	DRMFIGOREG_SECSTAT	 = (MEMMAP_TSI_REG_BASE + RA_DRMDMX_SECSAT + RA_SECSTATUS_CFG),
};
enum
{
	//! Security command status register address 
	DRMFIGOREG_CMDSTAT	 = (MEMMAP_TSI_REG_BASE + RA_DRMDMX_DTCM),
};

#endif
