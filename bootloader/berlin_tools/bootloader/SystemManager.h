#ifndef _SYSTEMMANAGER_H_
#define _SYSTEMMANAGER_H_

//#include "SysMgr.h"

#define CPUINDEX				1

#define SM_ITCM_BASE				0x00000000
#define SM_ITCM_SIZE				0x00008000	// 32k
#define SM_ITCM_END				0x00008000
#define SM_ITCM_ALIAS_SIZE			0x00020000	// 128k

#define SM_DTCM_BASE				0x04000000
#define SM_DTCM_SIZE				0x00004000	// 16k
#define SM_DTCM_END				0x04004000
#define SM_DTCM_ALIAS_SIZE			0x00020000	// 128k

#define SM_APBC_BASE				0x10000000
#define SM_APBC_SIZE				0x00010000	// 64k
#define SM_APBC_END				0x10010000
#define SM_APBC_ALIAS_SIZE			0x00010000	// 64k

#define SM_SECM_BASE				0x40000000
#define SM_SECM_SIZE				0x00010000	//64k
#define SM_SECM_END				0x40010000
#define SM_SECM_ALIAS_SIZE			0x00010000	// 64k

#define SOC_ITCM_BASE				0xf7f80000
#define SOC_DTCM_BASE				0xf7fa0000
#define SOC_APBC_BASE				0xf7fc0000
#define SOC_SECM_BASE				0xf7fd0000

#if ( CPUINDEX == 2 )
//#define SM_APBC( reg )			( (INT32)(reg) + SM_APBC_BASE )
#define SM_APBC( reg )				( (INT32)(reg) )
#else
//#define SM_APBC( reg )			( (INT32)(reg) + SOC_APBC_BASE )
#define SM_APBC( reg )				( (INT32)(reg) - SM_APBC_BASE + SOC_APBC_BASE )
#endif

#define SOC_ITCM( addr )			( (INT)addr - SM_ITCM_BASE + SOC_ITCM_BASE )
#define SOC_DTCM( addr )			( (INT)addr - SM_DTCM_BASE + SOC_DTCM_BASE )
#define SOC_APBC( addr )			( (INT)addr - SM_APBC_BASE + SOC_APBC_BASE )
#define SOC_SECM( addr )			( (INT)addr - SM_SECM_BASE + SOC_SECM_BASE )

#define SM_SM_APB_ICTL_BASE			(SM_APBC_BASE)
#define SM_SM_APB_WDT0_BASE			(SM_APBC_BASE + 0x1000)
#define SM_SM_APB_WDT1_BASE			(SM_APBC_BASE + 0x2000)
#define SM_SM_APB_WDT2_BASE			(SM_APBC_BASE + 0x3000)
#define SM_SM_APB_TIMERS_BASE			(SM_APBC_BASE + 0x4000)
#define SM_SM_APB_COUNTER_BASE			(SM_APBC_BASE + 0x5000)
#define SM_SM_APB_SPI_BASE			(SM_APBC_BASE + 0x6000)
#define SM_SM_APB_I2C0_BASE			(SM_APBC_BASE + 0x7000)
#define SM_SM_APB_I2C1_BASE			(SM_APBC_BASE + 0x8000)
#define SM_SM_APB_UART0_BASE			(SM_APBC_BASE + 0x9000)
#define SM_SM_APB_UART1_BASE			(SM_APBC_BASE + 0xA000)
#define SM_SM_APB_UART2_BASE			(SM_APBC_BASE + 0xB000)
#define SM_SM_APB_GPIO_BASE			(SM_APBC_BASE + 0xC000)
#define SM_SM_SYS_CTRL_REG_BASE			(SM_APBC_BASE + 0xD000)

#define LSb32SS_SM_ICTL_IRQ_SM_WDT0		0
#define LSb32SS_SM_ICTL_IRQ_SM_WDT1		1
#define LSb32SS_SM_ICTL_IRQ_SM_WDT2		2
#define LSb32SS_SM_ICTL_IRQ_SM_TIMERS		3
#define LSb32SS_SM_ICTL_IRQ_RTC		 	4
#define LSb32SS_SM_ICTL_IRQ_SM_SPI		5
#define LSb32SS_SM_ICTL_IRQ_SM_I2C0		6
#define LSb32SS_SM_ICTL_IRQ_SM_I2C1		7
#define LSb32SS_SM_ICTL_IRQ_SM_UART0		8
#define LSb32SS_SM_ICTL_IRQ_SM_UART1		9
#define LSb32SS_SM_ICTL_IRQ_SM_UART2		10
#define LSb32SS_SM_ICTL_IRQ_SM_GPIO_A		11
#define LSb32SS_SM_ICTL_IRQ_ADC		 	12

#if 1
#define MV_SM_READ_REG32( reg )			( *( (UINT32*)SM_APBC( reg ) ) )
#define MV_SM_READ_REG8( reg )			( *( (UCHAR*)SM_APBC( reg ) ) )
#define MV_SM_WRITE_REG32( reg, value )		( *( (UINT32*)SM_APBC( reg ) ) = (UINT32)( value ) )
#define MV_SM_WRITE_REG8( reg, value )		( *( (UCHAR*)SM_APBC( reg ) ) = (UCHAR)( value ) )
#else
#define MV_SM_READ_REG32( reg )			( *( (UINT32*)SM_APBC( reg ) ) )
#define MV_SM_READ_REG8( reg )			( *( (UCHAR*)SM_APBC( reg ) ) )
#define MV_SM_WRITE_REG32( reg, value )		{ ( *( (UINT32*)SM_APBC( reg ) ) = (UINT32)( value ) ); printf( "reg 0x%8.8x = 0x%8.8x\n", SM_APBC(reg), value ); }
#define MV_SM_WRITE_REG8( reg, value )		{ ( *( (UCHAR*)SM_APBC( reg ) ) = (UCHAR)( value ) ); printf( "reg 0x%8.8x = 0x%2.2x\n", SM_APBC(reg), value ); }
#endif


// define module ID for each module
#define MV_SM_ID_SYS				1
#define MV_SM_ID_COMM				2
#define MV_SM_ID_IR				3
#define MV_SM_ID_KEY				4
#define MV_SM_ID_POWER				5
#define MV_SM_ID_WD				6
#define MV_SM_ID_TEMP				7
#define MV_SM_ID_VFD				8
#define MV_SM_ID_SPI				9
#define MV_SM_ID_I2C				10
#define MV_SM_ID_UART				11


//	define SM state
#define MV_SM_STATE_COLDBOOTUP			1
#define MV_SM_STATE_WAIT_WARMUP_0_RESP		2
#define MV_SM_STATE_WARMUP			3
#define MV_SM_STATE_WAIT_WARMDOWN_RESP		4
#define MV_SM_STATE_WARMDOWN			5
#define MV_SM_STATE_WAIT_WARMUP_RESP		6

// following assignment is so we can assign Beetle-specific value that is different from
// a normal STATE_WARMUP_1...

#define reserved_MV_SM_STATE			7
#define MV_SM_POWER_BOOT_MODE			8 /* also used as msg type to SM */

#define MV_SM_STATE_WAIT_WARMDOWN_2_RESP	(0xA0)
#define MV_SM_STATE_WARMDOWN_2			(0xA1)
#define MV_SM_STATE_WAIT_WARMUP_2_RESP		(0xA2)
#define MV_SM_STATE_WARMUP_2			(0xA3)


//	define	 message  content
#define MV_SM_POWER_WARMUP_0_RESP		(MV_SM_STATE_WAIT_WARMUP_0_RESP)
#define MV_SM_POWER_WARMDOWN_1_RESP		(MV_SM_STATE_WAIT_WARMDOWN_RESP)
#define MV_SM_POWER_WARMUP_1_RESP		(MV_SM_STATE_WAIT_WARMUP_RESP)
#define MV_SM_POWER_WARMDOWN_2_RESP		(MV_SM_STATE_WAIT_WARMDOWN_2_RESP)
#define MV_SM_POWER_WARMUP_2_RESP		(MV_SM_STATE_WAIT_WARMUP_2_RESP)
#define MV_SoC_STATE_COLDBOOT			0x21

//	define	SoC state
//
#define MV_SoC_STATE_SYSPOWERON			0
#define MV_SoC_STATE_SYSRESET			0xFF
#define MV_SoC_STATE_COLDBOOTUP			(MV_SM_STATE_COLDBOOTUP)
#define MV_SoC_STATE_WARMUP_0_RESP		(MV_SM_POWER_WARMUP_0_RESP)
#define MV_SoC_STATE_WARMUP_0			(MV_SM_STATE_WAIT_WARMUP_0_RESP)

#define MV_SoC_STATE_WARMDOWN_1_RESP		(MV_SM_POWER_WARMDOWN_1_RESP)
#define MV_SoC_STATE_WARMDOWN_1			(MV_SM_STATE_WAIT_WARMDOWN_RESP)

#define MV_SoC_STATE_WARMUP_1_RESP		(MV_SM_POWER_WARMUP_1_RESP)
#define MV_SoC_STATE_WARMUP_1			(MV_SM_STATE_WAIT_WARMUP_RESP)
#define MV_SoC_STATE_WARMUP_2			(MV_SM_STATE_WAIT_WARMUP_2_RESP)
#define MV_SoC_STATE_WARMDOWN_2			(MV_SM_STATE_WAIT_WARMDOWN_2_RESP)

// Beetle specific variation on STATE_WARMUP_1. When bootloader sees it, he should skip 
// loading SM with sm.bin as he'd normally do, and then respond to it with the same message 
// he would use with MV_SoC_STATE_WARMUP_1.
#define	MV_SoC_STATE_WARMUP_FROM_DDR_OFF_1 (reserved_MV_SM_STATE)
#define MV_SoC_STATE_BOOT_RECOVERY_KERNEL  (MV_SM_POWER_BOOT_MODE) /* state requesting recovery mode*/ 

#define MV_SM_RSTFLAG_COLD			0xFFFF
#define MV_SM_RSTFLAG_WARM			0xFF00
#define MV_SM_TEMP_CPU_TYPE			0xCC
#define MV_SM_TEMP_CPU_TYPE_B0			0xB0

//#define DYNAMIC_MODULE

#endif // #ifndef _SYSTEMMANAGER_H_
