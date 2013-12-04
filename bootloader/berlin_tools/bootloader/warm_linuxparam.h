/* 
  * This file is generated automatically. Don't edit it! 
  * the following macro is system defined. 
  */

#include "sys_memmap.h"

#define CPU0_MEM_START			(GALOIS_CPU0_CACHE_START)
 #define CPU0_MEM_SIZE			(GALOIS_CPU0_CACHE_SIZE)
 #define CPU0_LINUX_START		(CPU0_MEM_START + 0x8000)
 #define LINUX_BOOTPARAM_ADDR	(CPU0_MEM_START+0x100)

#define LINUX_BOOTARGS "sata_reset_done console=ttyS0,115200 root=/dev/mtdblock1 rootfstype=yaffs2 ro init=/bin/srvd mtdparts=mv_nand:67108864(bootloader),268435456(rootfs),83886080(app-A),83886080(app-B),33554432(localstorage),1564475392(BDlocalstorage),8388608(factory_setting),16777216(flashless_loader),20971520(sysconf)" 

