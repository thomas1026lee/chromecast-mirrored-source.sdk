#ifndef _DIAG_
#define _DIAG_

#ifdef GA_TC1_SV0
#include "memmap.h"
#endif

#ifdef GA_LITE_TC1_SV0
#include "memMapGaLiteTc1.h"
#endif

#define CPU_TO_AHB_CLOCK_RATIO 4

//DWORD	WINAPI ThreadMemDiagHost( LPVOID lpParam ) ;

#define GET_VM_ADDR(addr)	((unsigned char*)(addr))

//
//#define TC1_SV          0
#define BERLIN_BG2_DIAG		1
#define AS_2ND_CPU		0
#if AS_2ND_CPU
 #define USE_CMD_LINE    0
 #define USE_UART		0
#else
 #define USE_CMD_LINE    1
 #define USE_UART		1
#endif
#define EN_UART0_FC		0
#define EN_SELF_TEST	0
#define APB_RST			0
#define CHK_INT			0
#define EN_CPU_INT      1
#define UART_BAUD_SETTING_DELAY 1
#define INVALID_REG_ACCESS 1
////////////////////////////////////
#define UART_DLL_5M_9600		0x20
#define UART_DLH_5M_9600		0x00

#define UART_DLL_4M_19200   	0x0D
#define UART_DLL_5M_19200   	0x10

#define UART_DLL_6M_19200   	0x14
#define UART_DLH_6M_19200   	0

#define UART_DLL_1P5M_19200   	0x05
#define UART_DLH_1P5M_19200     0x00

#define UART_DLL_9M_115200		0x05
#define UART_DLH_9M_115200		0x00

#define UART_DLL_9M_19200		0x1E
#define UART_DLH_9M_19200		0x00

#define UART_DLL_7P2M_19200		0x18
#define UART_DLH_7P2M_19200		0x00

#define UART_DLL_2M_9600		0x0D
#define UART_DLL_1P2M_19200		0x04

#define UART_DLL_100M_19200 	0x44
#define UART_DLH_100M_19200 	0x01

#define UART_DLL_100M_9600 		0x88
#define UART_DLH_100M_9600 		0x02

#define UART_DLL_100M_115200 	0x36
#define UART_DLH_100M_115200 	0x00

#define UART_DLL_20M_115200 	0x0B
#define UART_DLH_20M_115200 	0x00

#define UART_DLL_20M_19200		0x41
#define UART_DLH_20M_19200		0x00

#define UART_DLL_20M_9600		0x85
#define UART_DLH_20M_9600		0x00

#define UART_DLL_25M_115200 	0x0E
#define UART_DLH_25M_115200 	0x00

#define UART_DLL_24P576M_115200 0x0D        // 13.51; 0xD or 0xE
#define UART_DLH_24P576M_115200 0x00

#define UART_DLL_14P318M_115200 0x08        // for 14.31818MHz
#define UART_DLH_14P318M_115200 0x00
#define UART_DLL_14P318M_9600   0x95        // for 14.31818MHz
#define UART_DLH_14P318M_9600   0x00
//#define UART_DLL_24M_19200	0x52		// for 24.576MHz
#define UART_DLL_24M_19200	 	0x51		// for 24MHz
#define UART_DLH_24M_19200 		0x00

#define UART_DLL_36M_115200 	0x14
#define UART_DLH_36M_115200 	0x00

#define UART_DLL_48M_115200 	0x1A
#define UART_DLH_48M_115200 	0x00

#define UART_DLL_18M_19200	 	0x3C
#define UART_DLH_18M_19200 		0x00

#define UART_DLL_30M_115200 	0x10
#define UART_DLH_30M_115200 	0x00

#define UART_DLL_15M_19200	 	0x32
#define UART_DLH_15M_19200 		0x00

#define UART_DLL_50M_38400	 	0x53
#define UART_DLH_50M_38400 		0x00

#define UART_DLL_66M_115200 	0x24

#ifdef BG2_DV_BOARD
#define UART_DLL				UART_DLL_25M_115200
#define UART_DLH				UART_DLH_25M_115200
#define UART_DLL_9600			UART_DLL_20M_9600
#define UART_DLH_9600			UART_DLH_20M_9600
#else
#define UART_DLL				UART_DLL_48M_115200
#define UART_DLH				UART_DLH_48M_115200
#define UART_DLL_9600			UART_DLL_5M_9600
#define UART_DLH_9600			UART_DLH_5M_9600
#endif
//#define INSERT_BUG			// insert bug for verifing test logic

#define MAX_ERR_CNT		1


#endif
