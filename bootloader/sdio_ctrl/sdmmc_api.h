// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES OR INDEMNITIES.
//
//
// (C) Copyright 2006 Marvell International Ltd.
// All Rights Reserved

#ifndef __SDMMC_API_H__
#define __SDMMC_API_H__

//#include "typedef.h"
//#include "flash.h"
//#include "xllp_defs.h"
#include "sdioController.h"

#define NO_OF_SDIO_CARDS 3

// Globals
#define ENABLE_PARTITION_SUPPORT	0
#define TRUE                		1
#define FALSE               		0
#define OSCR_OFFSET         		0x10
#define BOOL                		int
#define OCR_ARG             		0x80FF8000
#define SD_OCR_ARG          		0x40FF8000
#define OCR_ACCESS_MODE_MASK		0x60000000

#define RESPONSE_LENGTH     		8
#define R2_BYTE_LENGTH				16
#define MMC_BUSY_BIT        		0x80000000
#define SD_NO_CMD1          		0x300
#define LOW_ARG             		0x0000ffff
#define NO_FLAGS            		0
#define BLK_CNT             		1
#define BLK_LEN             		512
#define	SDVHS_2_7_TO_3_6	 		0x1
#define	SDVHS_LOW_VOLT		 		0x2
#define	SDVHSARGSHIFT				8
#define SDVHSCHECKPATTERN			0x33
#define HARD512BLOCKLENGTH			512
#define PARTITIONMASK				0xF
#define MMC_SD_BOOT_PARTITION		1
#define MMC_SD_BOOT_PARTITION2		2
#define MMC_SD_USER_PARTITION		0
#define SCRSD1BITMODE 				0x1
#define SCRSD4BITMODE 				0x5
#define SD_CMD6_4BITMODE 			2
#define R1_LOCKEDCARDMASK			0xFDFFFFBF
#define R1_NOMASK					0xFFFFFFFF


// MMC Controller selections
typedef enum
{
	MMCNOTENABLED		 =  0,
	MMCLEGACYCONTROLLER1 =	1,
	MMCLEGACYCONTROLLER2 =	2,
	MM4CONTROLLER0 		 =	3,
	MM4CONTROLLER1 		 =	4,
	MM4CONTROLLER2 		 =	5,
}CONTROLLER_TYPE;

enum 
{
	XLLP_MMC = 0, 
	XLLP_SD =1, 
	XLLP_eSD = 2
};

typedef	enum
{
    XLLP_MMC_CMD0               = 0x0,	// go idle state
    XLLP_MMC_CMD1               = 0x1,	// send op command
    XLLP_MMC_CMD2               = 0x2,	// all send cid
    XLLP_MMC_CMD3               = 0x3,	// set relative addr
    XLLP_MMC_CMD4               = 0x4,	// set dsr
    XLLP_MMC_CMD7               = 0x7,	 // select/deselect card
    XLLP_SD_CMD6				= 0x6, 	 // SD Switch Function Command
    XLLP_MMC_CMD6				= 0x6, 	// MMC Switch Function Command
	XLLP_SD_ACMD6				= 0x6,	// SD ACMD Command for SET_BUS_WIDTH
    XLLP_SD_CMD8               	= 0x8,	 // SD Card Interface Condition
    XLLP_MMC_CMD8				= 0x8,	// MMC request to read EXT CSD
    XLLP_MMC_CMD9             	= 0x9,	// send csd
    XLLP_MMC_CMD10            	= 0xa,	// send cid
    XLLP_MMC_CMD11              = 0xb,	//  read data until stop
    XLLP_MMC_CMD12              = 0xc,	 // stop transmission
    XLLP_MMC_CMD13              = 0xd,	 // send status
    XLLP_MMC_CMD15              = 0xf,	// go inactive state
    XLLP_MMC_CMD16              = 0x10,	//  set block length
    XLLP_MMC_CMD17              = 0x11,	// read single block
    XLLP_MMC_CMD18              = 0x12,	//  read multiple block
    XLLP_MMC_CMD20              = 0x14,	 // write data until stop
	XLLP_MMC_CMD23            	= 0x17,	 // Set Block Count
    XLLP_MMC_CMD24            	= 0x18,	 // write block
    XLLP_MMC_CMD25              = 0x19,	 // write multiple block
    XLLP_MMC_CMD26              = 0x1a,	// program CID
    XLLP_MMC_CMD27            	= 0x1b,	// program CSD
    XLLP_MMC_CMD28            	= 0x1c,	// set write prot
    XLLP_MMC_CMD29            	= 0x1d,  // clr write prot
    XLLP_MMC_CMD30            	= 0x1e,	// send write prot
    XLLP_SD_CMD32            	= 0x20,	 // tag sector start
    XLLP_SD_CMD33            	= 0x21,	// tag sector end
    XLLP_SD_CMD38            	= 0x26,	 // erase
    XLLP_MMC_CMD34            	= 0x22,	// untag sector
    XLLP_MMC_CMD35            	= 0x23,	// tag erase group start
    XLLP_MMC_CMD36            	= 0x24,	//  tag erase group end
    XLLP_MMC_CMD37            	= 0x25,	 // untag erase group
	XLLP_eSD_CMD37				= 0x25,  // SD PartitionManagement group Command 
    XLLP_eSD_CMD57				= 0x39,
    XLLP_MMC_CMD38            	= 0x26,	 // erase
    XLLP_MMC_CMD39              = 0x27,	//fast IO
    XLLP_MMC_CMD40              = 0x28,	// go irq state
    XLLP_MMC_CMD42            	= 0x2a,	// lock-unlock
    XLLP_eSD_CMD43				= 0x2b, // Select Partition
    XLLP_SD_CMD55              	= 0x37,	// app cmd
    XLLP_MMC_CMD56            	= 0x38,	//  gen cmd
    XLLP_SPI_CMD58              = 0x3a,	 // read ocr
    XLLP_SPI_CMD59              = 0x3b,	 // crc on-off
    XLLP_SD_ACMD41	            = 0x29,
    XLLP_SD_ACMD13      	    = 0x0d,	  // Read SD Status
 #ifndef IO_CARD_SUPPORT 
    XLLP_SD_ACMD51      	    = 0x33	  // Read SD Configuration Register (SCR)
#else
    XLLP_SD_ACMD51      	    = 0x33,	  // Read SD Configuration Register (SCR)
    XLLP_SDIO_CMD5				= 0x05,	//Satya
    XLLP_SDIO_CMD52				= 0x34,
    XLLP_SDIO_CMD53				= 0x35
#endif
} XLLP_MMC_CMD;

typedef struct
{
	UINT_T		CID_VALUE[4];
	UINT_T		SerialNum;	
} CID_LAYOUT;

typedef struct
{
	UINT_T		CSD_VALUE[4];
} CSD_LAYOUT;
typedef struct
{
	UINT_T		SD_VALUE[4];
} SD_LAYOUT;

typedef struct
{
	UINT_T		SCR_VALUE[2];
} SCR_LAYOUT;

// SD CMD 6 Related
typedef struct
{
	unsigned int   	FunctionGroup		:24;
	unsigned int	Reserved 			:7;
	unsigned int	Mode 				:1;
} CMD6_LAYOUT;

typedef union
{
	UINT_T			CMD6_Bits;
	CMD6_LAYOUT		CMD6_Layout;
} CMD6_OVERLAY;

// SD CMD 6 Related
typedef struct
{
	unsigned int   	FunctionGroup		:24;
	unsigned int	Reserved 			:7;
	unsigned int	Mode 				:1;
} SD_CMD6_LAYOUT;


typedef union
{
	UINT_T				SD_CMD6_Bits;
	SD_CMD6_LAYOUT		SD_CMD6_Layout;
} SD_CMD6_OVERLAY;


// MMC CMD 6 Related
typedef struct
{
	unsigned int	CmdSet 				:3;
	unsigned int	Reserved0 			:5;
	unsigned int   	Value				:8;
	unsigned int   	Index				:8;
	unsigned int   	Access				:2;
	unsigned int	Reserved1 			:5;
} MMC_CMD6_LAYOUT;

typedef union
{
	UINT_T				MMC_CMD6_Bits;
	MMC_CMD6_LAYOUT		MMC_CMD6_Layout;
} MMC_CMD6_OVERLAY;


#define MMC_SWITCH_MODE_CMD_SET		0x00 /* Change the command set */
#define MMC_SWITCH_MODE_SET_BITS	0x01 /* Set bits in EXT_CSD byte
						addressed by index which are
						1 in value field */
#define MMC_SWITCH_MODE_CLEAR_BITS	0x02 /* Clear bits in EXT_CSD byte
						addressed by index, which are
						1 in value field */
#define MMC_SWITCH_MODE_WRITE_BYTE	0x03 /* Set target byte to value */

// EXT_CSD
#define BOOT_BUS_WIDTH_MMC_EXT_CSD_OFFSET	177
#define PARTITION_CONFIG_MMC_EXT_CSD_OFFSET	179
#define BUS_WIDTH_MMC_EXT_CSD_OFFSET		183
#define HS_TIMING_MMC_EXT_CSD_OFFSET		185
#define EXT_CSD_ACCESS_CMD_SET				0
#define EXT_CSD_ACCESS_SET_BITS				1
#define EXT_CSD_ACCESS_CLEAR_BITS			2
#define EXT_CSD_ACCESS_WRITE_BYTE			3
#define PARTITION_ACCESS_BITS				0x7

#define EXT_CSD_CMD_SET_NORMAL		(1<<0)
#define EXT_CSD_CMD_SET_SECURE		(1<<1)
#define EXT_CSD_CMD_SET_CPSECURE	(1<<2)


#define PARTITION_MANAGEMENT_CMD_SET 	0xFFFF2F
#define DEFAULT_CMD_SET					0xFFFFFF
#define CHECK_FUNCTION_MODE 			0
#define SWITCH_FUNCTION_MODE			1
#define PARTITION_MANAGEMENT_FUNCTION 	2
#define MMC_ALTERNATE_BOOT_ARGUMENT 	0xFFFFFFFA
#define MMC_CMD0_PRE_IDLE_ARGUMENT		0xF0F0F0F0


typedef enum
{
	UNINITIALIZED,			// Controller and Card are uninitialized
	INITIALIZE,				// Controller and Card are being ninitialized
	READ,					// Multiple Block Read State
	ERASE,					// Erase State
	WRITE,					// Multiple Block Write State
	READY,					// The Card is ready for Data Transfer 
	DATATRAN,				// The controller has finished data transfer but card may be busy
	FAULT					// Fault
} SDMMC_IO_TRANSFERS;

// Vital information used in Data Transfers by the ISR and driver routines.
typedef struct
{
	UINT_T					CardAddress;		// Starting Card Address
	UINT_T					TransWordSize; 		// Total Number of Bytes involved in this transaction
	UINT_T					NumBlocks; 			// Total Number of Blocks involved in this transaction
	UINT_T					LocalAddr;			// Destination Address Pointer for reads, and source addr for writes
	UINT_T 					StartDiscardWords;	// Words - from the first block that caller doesn't want
	UINT_T					EndDiscardWords;	// Words - from the last block that caller doesn't want
	UINT_T					WordIndex;			// Words - Word index to the progress in this transfer request
	UINT_T					Cmd;				// The command that started the transaction
	UINT_T					ACmdFlag;			// Indicates if the executing command is standard vs. application specific (ACMD)
	UINT_T					AutoCMD12Mode;		// When "true", the controller is operating in auto command 12 mode.
	UINT_T					RespType;			// Info about the response type and potential for busy state.
} SDMMC_TRANSFER;

typedef struct
{
	UINT_T					RCA;			// RCA of the card	
  	UINT_T					OCR;			// OCR Register Contents
	CID_LAYOUT				CID;			// CID Register
  	CSD_LAYOUT			  	CSD;			// CSD Register Contents
  	SCR_LAYOUT				SCR;			// SCR Register Contents
  	SD_LAYOUT               SD;             // SD status	
}	SDMMC_CARD_REGISTERS; 
#define SD_SPEC_MASK	0x0F000000
#define SD_SPEC_OFFSET	24


typedef struct 
{
  UINT_T pBuffer[4];							// Buffer to read values in to
  volatile UINT_T CommandComplete;			
// TBD  UINT_T CommandError;	
  UINT_T SendStopCommand;					// If some read/write transactions require stop command	
  UINT_T R1_RESP;							// Capture the R1 Response of the Card for most commands
} SDMMC_RESPONSE;

typedef enum
{
	BYTE_ACCESS = 0, 
	SECTOR_ACCESS = 1
}ACCESS_MODE;

// This Properties structure is shared between MM4 and legacy type drivers. The pContext field
// is specific to the variant. 

typedef struct 
{
	CONTROLLER_TYPE			ControllerType;		// See CONTROLLER_TYPE platformconfig.h
	P_MM4_SDMMC_CONTEXT_T	 	*pContext; 			// Pointer to MMC control registers
	UINT_T					SD; 				// Indicates if the card is SD, eSD or MMC
  	UINT_T					Slot; 				// Indicates which slot used
	SDMMC_CARD_REGISTERS	CardReg;			// Card Registers 
	SDMMC_RESPONSE			CardReponse;		// Card Response Related 
	UINT_T 					SD_VHS; 			// SD Voltage Acceptance Return Value (SD only)
	volatile SDMMC_IO_TRANSFERS	State;			// Indicate State of the card
	unsigned int 			pad1;
	ACCESS_MODE				AccessMode;			// High Density Card
	unsigned int			pad2;
	SDMMC_TRANSFER			Trans;				// Transfer State of the Card
	UINT_T					ReadBlockSize;		// Bytes - Block Size Used for Reads
	UINT_T					WriteBlockSize;		// Bytes - Block Size Used for Writes
	UINT_T					EraseSize;			// Bytes - Minimum Size of an erasable unit
	UINT_T					CardCapacity; 		// Bytes - Maximum Capacity of the card	
//	P_FlashProperties_T 	pFlashP; 			// A pointer to generic FlashProperties structure
//	SDMMC_FUNCTIONS			Funcs;				// Pointer to some internal driver functions
	UINT_T					SDMA_Mode; 			// Enable SDMA Mode or Not.
	UINT_T					StrictErrorCheck;	// Relax error checking during card init. Not all cards are strictly compliant.
} 
SDMMC_Properties_T, *P_SDMMC_Properties_T;


#ifdef IO_CARD_SUPPORT
//Satya
typedef struct SDIO_Properties_s
{
	UINT8_T		uchCardReadyAfterInit:1;
	UINT8_T 	uchNoOfIoFunc:3;
	UINT8_T		uchMemPresent:1;
	UINT8_T		uchStuffBits:3;
//	UINT32_T	uchIoOcer:24;
} __attribute__((packed)) SDIO_Properties_T;



//CMD52 structure
#if 0
typedef struct SDIO_CMD52_Arg_s
{
	UINT_T 	uchRwFlag:1;
	UINT_T	uchFuncNum:3;
	UINT_T 	uchRawFlag:1;
	UINT_T	uchStuffBit:1;
	UINT_T	uiRegAddr:17;
	UINT_T	uchStuffBit1:1;
	UINT_T	uchWriteData:8;
} __attribute__((packed)) SDIO_CMD52_Arg_T;
#endif

typedef struct SDIO_CMD52_Arg_s
{
	UINT_T	uchWriteData:8;
	UINT_T	uchStuffBit1:1;
	UINT_T	uiRegAddr:17;
	UINT_T	uchStuffBit:1;
	UINT_T 	uchRawFlag:1;
	UINT_T	uchFuncNum:3;
	UINT_T 	uchRwFlag:1;
} __attribute__((packed)) SDIO_CMD52_Arg_T;

typedef union SDIO_CMD52_Arg_Union
{
	SDIO_CMD52_Arg_T stCmd52Arg;
	UINT32_T uiCmd52Arg;
}SDIO_CMD52_Arg_Union_T;

#endif
/* Bit Position Macros */
#define XLLP_BIT_0    ( 1u << 0 )
#define XLLP_BIT_1    ( 1u << 1 )
#define XLLP_BIT_2    ( 1u << 2 )
#define XLLP_BIT_3    ( 1u << 3 )
#define XLLP_BIT_4    ( 1u << 4 )
#define XLLP_BIT_5    ( 1u << 5 )
#define XLLP_BIT_6    ( 1u << 6 )
#define XLLP_BIT_7    ( 1u << 7 )
#define XLLP_BIT_8    ( 1u << 8 )
#define XLLP_BIT_9    ( 1u << 9 )
#define XLLP_BIT_10   ( 1u << 10 )
#define XLLP_BIT_11   ( 1u << 11 )
#define XLLP_BIT_12   ( 1u << 12 )
#define XLLP_BIT_13   ( 1u << 13 )
#define XLLP_BIT_14   ( 1u << 14 )
#define XLLP_BIT_15   ( 1u << 15 )
#define XLLP_BIT_16   ( 1u << 16 )
#define XLLP_BIT_17   ( 1u << 17 )
#define XLLP_BIT_18   ( 1u << 18 )
#define XLLP_BIT_19   ( 1u << 19 )
#define XLLP_BIT_20   ( 1u << 20 )
#define XLLP_BIT_21   ( 1u << 21 )
#define XLLP_BIT_22   ( 1u << 22 )
#define XLLP_BIT_23   ( 1u << 23 )
#define XLLP_BIT_24   ( 1u << 24 )
#define XLLP_BIT_25   ( 1u << 25 )
#define XLLP_BIT_26   ( 1u << 26 )
#define XLLP_BIT_27   ( 1u << 27 )
#define XLLP_BIT_28   ( 1u << 28 )
#define XLLP_BIT_29   ( 1u << 29 )
#define XLLP_BIT_30   ( 1u << 30 )
#define XLLP_BIT_31   ( 1u << 31 )


/*	SD_ERROR_INT_STATUS				0x0032	Error Interrupt Status Register */
/* CRC Status Error */
#define	SD_ERROR_INT_STATUS_CRC_STATUS_ERR			XLLP_BIT_15			
/* Command Completion Signal Timeout Error */
#define	SD_ERROR_INT_STATUS_CPL_TIMEOUT_ERR			XLLP_BIT_14			
/* AXI Bus Response Error */
#define	SD_ERROR_INT_STATUS_AXI_RESP_ERR			XLLP_BIT_13			
/* SPI Mode Error */
#define	SD_ERROR_INT_STATUS_SPI_ERR					XLLP_BIT_12			
/*		Bit(s) SD_ERROR_INT_STATUS_RSRV_11_10 reserved */
#define	SD_ERROR_INT_STATUS_ADMA_ERR				XLLP_BIT_9			/* ADMA Error */
/* Auto CMD12 Error */
#define	SD_ERROR_INT_STATUS_AUTO_CMD12_ERR			XLLP_BIT_8			
/* Current Limit Error */
#define	SD_ERROR_INT_STATUS_CUR_LIMIT_ERR			XLLP_BIT_7			
/* ReadDataEnd Bit Error */
#define	SD_ERROR_INT_STATUS_RD_DATA_END_BIT_ERR		XLLP_BIT_6			
/* Read Data CRC Error */
#define	SD_ERROR_INT_STATUS_RD_DATA_CRC_ERR			XLLP_BIT_5			
/* Data Timeout Error */
#define	SD_ERROR_INT_STATUS_DATA_TIMEOUT_ERR		XLLP_BIT_4			
/* Command Index Error */
#define	SD_ERROR_INT_STATUS_CMD_INDEX_ERR			XLLP_BIT_3			
/* Command End Bit Error */
#define	SD_ERROR_INT_STATUS_CMD_END_BIT_ERR			XLLP_BIT_2			
/* Command CRC Error */
#define	SD_ERROR_INT_STATUS_CMD_CRC_ERR				XLLP_BIT_1			
/* Command Timeout Error */
#define	SD_ERROR_INT_STATUS_CMD_TIMEOUT_ERR			XLLP_BIT_0	





/*********************************************************************
*	SDMMC R1 Response Bits
**********************************************************************/
typedef enum
{
	R1_SWITCH_ERROR	 			= XLLP_BIT_0,
	R1_ERASE_RESET_ERROR	 	= XLLP_BIT_13,
	R1_CIDCSD_OVERWRITE_ERROR	= XLLP_BIT_16,
	R1_OVERRUN_ERROR 			= XLLP_BIT_17,
	R1_UNDERUN_ERROR 			= XLLP_BIT_18,
	R1_GENERAL_ERROR 			= XLLP_BIT_19,
	R1_CC_ERROR 				= XLLP_BIT_20,
	R1_ECC_ERROR 				= XLLP_BIT_21,
	R1_ILL_CMD_ERROR 			= XLLP_BIT_22,
	R1_COM_CRC_ERROR			= XLLP_BIT_23,
	R1_LOCK_ULOCK_ERRROR		= XLLP_BIT_24,
	R1_LOCK_ERROR 				= XLLP_BIT_25,
	R1_WP_ERROR 				= XLLP_BIT_26,
	R1_ERASE_PARAM_ERROR		= XLLP_BIT_27,
	R1_ERASE_SEQ_ERROR			= XLLP_BIT_28,
	R1_BLK_LEN_ERROR 			= XLLP_BIT_29,
	R1_ADDR_MISALIGN_ERROR 		= XLLP_BIT_30,
	R1_ADDR_RANGE_ERROR 		= XLLP_BIT_31
} R1_SDMMC_RESPONSE_CODES;

typedef struct
{
	unsigned int Reserved0	 		: 1;
	unsigned int BootAck			: 1;
	unsigned int BootPartitionEn	: 3;
	unsigned int PartitionAccess	: 3;
} PARTITION_CONFIG_EXT_CSD;


void SDMMC_ISR(void);
UINT_T SDMMCGetCardErrorState(P_SDMMC_Properties_T pSDMMCP);

//Function Prototypes
void MMC4StopInternalBusClock(P_MM4_SDMMC_CONTEXT_T pContext);
void MMC4StartInternalBusClock(P_MM4_SDMMC_CONTEXT_T pContext);
void MMC4StartBusClock(P_MM4_SDMMC_CONTEXT_T pContext);
void MMC4StopBusClock (P_MM4_SDMMC_CONTEXT_T pContext);
void MMC4SetBusRate(P_MM4_SDMMC_CONTEXT_T pContext, unsigned int rate);
void MMC4EnableDisableIntSources(P_MM4_SDMMC_CONTEXT_T pContext, unsigned int Desire);
void MMC4SetDataTimeout(P_MM4_SDMMC_CONTEXT_T pContext, unsigned int CounterValue);
void MMC4DataSWReset(P_MM4_SDMMC_CONTEXT_T pContext);
void MMC4FullSWReset(P_MM4_SDMMC_CONTEXT_T pContext);

void MMC4SendDataCommand(P_MM4_SDMMC_CONTEXT_T pContext, 
                  UINT_T Cmd,
                  UINT_T  Argument, 
                  UINT_T BlockType,
                  UINT_T DataDirection,
                  UINT_T ResType);


void MMC4SendSetupCommand(P_MM4_SDMMC_CONTEXT_T pContext,
                  UINT_T Cmd,
                  UINT_T CmdType,
                  UINT_T Argument,
                  UINT_T ResType);

P_SDMMC_Properties_T GetSDMMCProperties(unsigned char);

extern UINT_T MM4_HCInit(P_SDMMC_Properties_T pSDMMCP);
extern int SDIOWaitForCardDetect(P_SDMMC_Properties_T pSDMMCP) ;
extern UINT_T MM4_CardInit(P_SDMMC_Properties_T pSDMMCP);
extern int SioStart(P_SDMMC_Properties_T pSDMMCP) ;

#endif		// __SDMMC_API_H__
