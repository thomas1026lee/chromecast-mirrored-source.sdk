/* 
  * This file is generated automatically. Don't edit it! 
  * the following macro is system defined. 
  */
 
#ifndef	__COLD_LINUXPARAM_H
#define	__COLD_LINUXPARAM_H

#if 0
#include "sys_memmap.h"

 #define CPU0_MEM_START		(GALOIS_CPU0_CACHE_START)
 #define CPU0_MEM_SIZE			(GALOIS_CPU0_CACHE_SIZE)
 #define CPU0_LINUX_START		(CPU0_MEM_TART + 0x8000)
 #define LINUX_BOOTPARAM_ADDR	(CPU0_MEM_START+0x100)
#endif

#define LINUX_BOOTARGS "sata_reset_done nfc_shared macaddr=00:83:11:11:00:48 console=ttyS0,115200 root=/dev/mtdblock1 rootfstype=yaffs2 ro init=/bin/srvd has_video_logo mtdparts=mv_nand:64M(bootloader),200M(rootfs),80M(app-A),80M(app-B),32M(localstorage),-(BDlocalstorage)" 

#endif
