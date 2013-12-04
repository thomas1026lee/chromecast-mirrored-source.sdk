#include "Diag.h"
#include "util.h"
#include "global.h"
//#include "pcieDiag.h"
#include "memmap.h"
#include "apbRegBase.h"
#include "sdioController.h"
#include "sdmmc_api.h"
#include "gicDiag.h"
#include "util.h"
//#include "SD.h"

#include "pic.h"
#include "SysMgr.h"


extern void  IRQ_Handler(void);

#ifdef	EMMC_DEBUG
#define printf lgpl_printf
#else
#define printf
#define	dbg_printf
#endif

#define	SDIO_INTERNAL_DIV	SDCLK_SEL_DIV_2

//	Added by Xiufeng, SDIO has 3 slots
#define	SD3_SLOT1_BASE_OFFSET		0x0
#define	SD3_SLOT2_BASE_OFFSET		0x800
//	EMMC
#define	SD3_SLOT3_BASE_OFFSET		0x1000

#define	LOG_REG			(MEMMAP_CHIP_CTRL_REG_BASE+RA_Gbl_sw_generic3)

#define SDIO3_SLOT1_REG_BASE	(MEMMAP_SDIO3_REG_BASE+SD3_SLOT1_BASE_OFFSET)
#define SDIO3_SLOT2_REG_BASE	(MEMMAP_SDIO3_REG_BASE+SD3_SLOT2_BASE_OFFSET)
#define SDIO3_SLOT3_REG_BASE	(MEMMAP_SDIO3_REG_BASE+SD3_SLOT3_BASE_OFFSET)
#define	GLOBAL_SDIO_DLLMST_STATUS	(MEMMAP_CHIP_CTRL_REG_BASE+RA_Gbl_sdioDllMstStatus)
#define	GLOBAL_SDIO_DLLMST_REF		(MEMMAP_CHIP_CTRL_REG_BASE+RA_Gbl_sdioDllMstRef)
#define	GLOBAL_SDIO_DLLMST_CTRL		(MEMMAP_CHIP_CTRL_REG_BASE+RA_Gbl_sdioDllMstCtrl)

#define SDIO_ADMA_RXDESC_BASE 0xA80000
#define SDIO_ADMA_TXDESC_BASE 0xA80400
#define	SDIO_BOOT_BUF	0xF8080000

#define SDIO_CARD_PRESENT_BIT 0x40000
#define SD_CE_ATA_2_OFFSET 		0x010E
#define SD_CE_ATA_2_REG  		(MEMMAP_SDIO3_REG_BASE + SD_CE_ATA_2_OFFSET)

/**
 * General error code definitions			0x0 - 0x1F
 **/
#define NoError					0x0
#define NotFoundError					0x1
#define GeneralError					0x2
#define WriteError					0x3
#define ReadError							0x4
#define NotSupportedError					0x5
#define InvalidPlatformConfigError			0x6
#define PlatformBusy						0x7
#define PlatformReady						0x8
#define InvalidSizeError					0x9

// Flash Related Errors					0x20 - 0x3F
#define EraseError							0x20
#define ProgramError						0x21
#define InvalidBootTypeError				0x22
#define ProtectionRegProgramError			0x23
#define NoOTPFound							0x24
#define BBTReadError						0x25
#define MDOCInitFailed						0x26
#define OneNandInitFailed                   0x27
#define MDOCFormatFailed                    0x28
#define BBTExhaustedError                   0x29
#define FlashDriverInitError                0x30
#define FlashFuncNotDefined					0x31
#define OTPError							0x32
#define InvalidAddressRangeError			0x33
#define FlashLockError						0x34
#define ReadDisturbError					0x35

#define TimeOutError						0x96


// SDMMC Errors							0xD0-0xE2
#define SDMMC_SWITCH_ERROR					0xD0
#define SDMMC_ERASE_RESET_ERROR				0xD1
#define SDMMC_CIDCSD_OVERWRITE_ERROR		0xD2
#define SDMMC_OVERRUN_ERROR				0xD3
#define SDMMC_UNDERUN_ERROR				0xD4
#define SDMMC_GENERAL_ERROR				0xD5
#define SDMMC_CC_ERROR						0xD6
#define SDMMC_ECC_ERROR					0xD7
#define SDMMC_ILL_CMD_ERROR				0xD8
#define SDMMC_COM_CRC_ERROR					0xD9
#define SDMMC_LOCK_ULOCK_ERRROR				0xDA
#define SDMMC_LOCK_ERROR					0xDB
#define SDMMC_WP_ERROR						0xDC
#define SDMMC_ERASE_PARAM_ERROR				0xDD
#define SDMMC_ERASE_SEQ_ERROR				0xDE
#define SDMMC_BLK_LEN_ERROR				0xDF
#define SDMMC_ADDR_MISALIGN_ERROR			0xE0
#define SDMMC_ADDR_RANGE_ERROR				0xE1
#define SDMMCDeviceNotReadyError			0xE2
#define SDMMCInitializationError			0xE3
#define SDMMCDeviceVoltageNotSupported		0xE4

typedef struct SDIO_SD
{
    unsigned char   dataBusWidth;
    unsigned char   securedMode;
    unsigned short  sdCardType;
    unsigned int    sizeProtectedArea;
    unsigned char   speedClass;
    unsigned char   performanceMove;
    unsigned short  auSize;

} SDIO_SD;
typedef struct SDIO_SCR
{
    unsigned char   structure;
    unsigned char   specVersion;
    unsigned char   dataStatus;
    unsigned char   security;
    unsigned char   busWidth;
    unsigned short  reserved;
    unsigned long   manufatureReserved;

} SDIO_SCR;

// CID: card identification(128 bits = 16 bytes)
typedef struct      // check whether there is any word alligment issue.
{
    unsigned char   manufactureID;
    char            OEM[3];
    char            name[7];            // SD: 5 byte string, MMC: 6 byte string
    unsigned char   revision;
    unsigned long   serialNumber;
    unsigned short  manufactureDate;    // SD: 12 bit code, MMC: 8 bit code.

} SDIO_CID;

// CSD
typedef struct
{
    unsigned char   structure;					// [127:126] for eMMC and SD
    unsigned char   specificationVersion;
    unsigned char   dataReadAccessTime2;
    unsigned char   dataReadAccessTime1;
    unsigned char   maxDataTransferRate;
    unsigned short  cardCommandClass;
    unsigned char   readBlockLength;
    unsigned char   partialReadFlag;
    unsigned char   writeBlockMisalign;
    unsigned char   readBlockMisalign;
    unsigned char   DSR_implemented;
    unsigned short  deviceSize;
    unsigned char   maxreadCurrent_VddMin;
    unsigned char   maxReadCurrent_VddMax;
    unsigned char   maxWriteCurrent_VddMin;
    unsigned char   maxWriteCurrent_VddMax;
    unsigned char   deviceSizeMultiplier;
    unsigned char   eraseBlockEnable;           //SD only
    unsigned char   eraseSectorSize;            //SD:7 bits, MMC: 5 bits
    unsigned char   eraseGroupSize;             //MMC: only
    unsigned char   WP_GroupSize;               //SD:7 bits, MMC: 5 bits
    unsigned char   WP_GroupEnable;
    unsigned char   writeSpeedFactor;
    unsigned char   writeBlockLength;
    unsigned char   partialWriteFlag;
    unsigned char   fileFormatGroup;
    unsigned char   copyFlag;
    unsigned char   WP_perm;
    unsigned char   WP_temp;
    unsigned char   fileFormat;

} SDIO_CSD;

SDMMC_Properties_T      SDMMC_Prop;
MM4_SDMMC_CONTEXT_T	MM4_Context;
int	emmc_inited = 0 ;
SDIO_CID m_cid;

static int mmc4_switch(P_SDMMC_Properties_T pSDMMCP, unsigned char set, unsigned char index, unsigned char value);
static int mmc4_set_hs(P_SDMMC_Properties_T pSDMMCP);
static int mmc4_wait_card_ready(P_SDMMC_Properties_T pSDMMCP);

UINT_T MM4_CheckCardStatus(P_SDMMC_Properties_T pSDMMCP, UINT_T R1_Resp_Match, UINT_T Mask);
#if 1
/***********************************************************
*   MM4_ReadFifo
*      Reads the contents of the read fifo (512 words)
*   Input:
*      P_SDMMC_Properties_T pSDMMCP - pointer to the SDMMC context structure
*   Output:
*      buffer will contain the contents of the read fifo
*   Returns:
*      none
*************************************************************/
void MM4_ReadFifo(P_SDMMC_Properties_T pSDMMCP)
{
    int i, t = 0;
    UINT_T Buffer =0x0;
	P_MM4_SDMMC_CONTEXT_T pContext;
	volatile UINT_T *pMMC_RX_Fifo;

	// Assign our context value
	pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;

	pMMC_RX_Fifo = (volatile UINT_T *)&(pContext->pMMC4Reg->mm4_dp);
	t = pSDMMCP->Trans.WordIndex;

	// Ignore Pre Bytes
	for (i=0; (i < MM4FIFOWORDSIZE) && (t < pSDMMCP->Trans.StartDiscardWords); i++, t++){
		Buffer = *pMMC_RX_Fifo;
	}

	// Read Requested Data
	for (; ((i < MM4FIFOWORDSIZE) && (t < (pSDMMCP->Trans.TransWordSize-pSDMMCP->Trans.EndDiscardWords))); i++, t++){
		((UINT_T*) pSDMMCP->Trans.LocalAddr)[t] = *pMMC_RX_Fifo;
	}

	// Ignore Trailing Bytes
	for (; (i < MM4FIFOWORDSIZE) && (t < pSDMMCP->Trans.TransWordSize); i++, t++){
		Buffer = *pMMC_RX_Fifo;
	}

	pSDMMCP->Trans.WordIndex = t;
}

/***********************************************************
*   SDMMCWriteFifo
*      Writes 2048 bytes (512 words) to the FIFO
*   Input:
*      P_SDMMC_Properties_T pSDMMCP - pointer to the SDMMC context structure
*   Output:
*      none
*   Returns:
*      none
*************************************************************/
void MM4_WriteFifo(P_SDMMC_Properties_T pSDMMCP)
{
    int i, t = 0;
    UINT_T Buffer =0x0;
	P_MM4_SDMMC_CONTEXT_T pContext;
	volatile UINT_T *pMMC_TX_Fifo;

	// Assign our context value
	pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;

	pMMC_TX_Fifo = (volatile UINT_T *)&(pContext->pMMC4Reg->mm4_dp);
	t = pSDMMCP->Trans.WordIndex;

	// Ignore Pre Bytes
	for (i=0; (i < MM4FIFOWORDSIZE) && (t < pSDMMCP->Trans.StartDiscardWords); i++, t++)
		*pMMC_TX_Fifo = Buffer;

	// Write Requested Data
	for (; ((i < MM4FIFOWORDSIZE) && (t < (pSDMMCP->Trans.TransWordSize-pSDMMCP->Trans.EndDiscardWords))); i++, t++)
		*pMMC_TX_Fifo = ((UINT_T*) pSDMMCP->Trans.LocalAddr)[t];

	// Ignore Trailing Bytes
	for (; (i < MM4FIFOWORDSIZE) && (t < pSDMMCP->Trans.TransWordSize); i++, t++)
		*pMMC_TX_Fifo = Buffer;

	pSDMMCP->Trans.WordIndex = t;

}
#endif

/****************************************************************
*   MM4_Read_Response
*	Reads the response from the Controller Buffer Registers to the Local Buffer.
*		According to the last command and response type it does the correct interpretation.
*		There is also a timeout as the routine waits for the ISR to signal last command completion.
*   Input:
*		P_SDMMC_Properties_T pSDMMCP - Generic SD/MMC driver properties structure
*		ResponseType - See SD/MMC specifications
*		ResponseTimeOut - A time out value in millisec's
*   Output:
*		none
*   Returns:
*		TimeOutError or NoError
*****************************************************************/

UINT_T MM4_Interpret_Response(P_SDMMC_Properties_T pSDMMCP, UINT_T ResponseType, UINT_T ResponseTimeOut)
{
	UINT_T temp, temp2, temp3;	// ElapsedMSecs;
	UINT_T Retval = NoError;
	P_MM4_SDMMC_CONTEXT_T pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;		// Assign our context value
	P_MM4_CMD_XFRMD_UNION pMM4_XFRMD;
	volatile unsigned int i ;

	// Setup OS Timer register (Register 4)
	//pContext->pOSTReg->omcr4 = 0x42;				// Increment evrey msec
	// Clear the value;
	//pContext->pOSTReg->oscr4 = 0;
	// Overlap XFRMD register contents using uniun
	pMM4_XFRMD = (P_MM4_CMD_XFRMD_UNION) &pContext->pMMC4Reg->mm4_cmd_xfrmd;
	i=0;

	while (!pSDMMCP->CardReponse.CommandComplete)
	{
		i++;
		if (i>=0x200000) break;
	}

	if (i==0x200000)
	{

		printf(" pSDMMCP->CardReponse.CommandComplete = %d Time out \n", pSDMMCP->CardReponse.CommandComplete);
		Retval = TimeOutError;
		return Retval;
	}


	// Read in the Buffers
	switch (ResponseType)
	{
		case MMC_RESPONSE_NONE:
			break;
		case MMC_RESPONSE_R1:
		case MMC_RESPONSE_R1B:
			pSDMMCP->CardReponse.R1_RESP = pSDMMCP->CardReponse.pBuffer[0];
			printf("MMC_RESPONSE_R1B resp: 0x%x\n", pSDMMCP->CardReponse.R1_RESP) ;
			break;
		case MMC_RESPONSE_R2:			// This is for CID or CSD register
		{
			if (pMM4_XFRMD->mm4_cmd_xfrmd_bits.cmd_idx == XLLP_MMC_CMD9) //CSD
			{
				// Copy the CSD values from the buffer
				pSDMMCP->CardReg.CSD.CSD_VALUE[0] = (pSDMMCP->CardReponse.pBuffer[3] << 8) | (pSDMMCP->CardReponse.pBuffer[2] >> 24);
				pSDMMCP->CardReg.CSD.CSD_VALUE[1] = (pSDMMCP->CardReponse.pBuffer[2] << 8) | (pSDMMCP->CardReponse.pBuffer[1] >> 24);
				pSDMMCP->CardReg.CSD.CSD_VALUE[2] = (pSDMMCP->CardReponse.pBuffer[1] << 8) | (pSDMMCP->CardReponse.pBuffer[0] >> 24);
				pSDMMCP->CardReg.CSD.CSD_VALUE[3] = (pSDMMCP->CardReponse.pBuffer[0] << 8);
				   // Optionally we could record maximum block lengths from the CSD.
				// But some devices cheat and put incorrect values in this field.
				// Save off read Block Size, play it safe, for now hard code to 512 Bytes
				pSDMMCP->ReadBlockSize = HARD512BLOCKLENGTH;
				// Save off Write Block Size
				pSDMMCP->WriteBlockSize = HARD512BLOCKLENGTH;

				// Capture Erase Granularity.
				{
					pSDMMCP->EraseSize = ((pSDMMCP->CardReg.CSD.CSD_VALUE[2] >> 5) & 0x1F) + 1; 		// Get ERASE_GRP_MULT
					pSDMMCP->EraseSize *= (((pSDMMCP->CardReg.CSD.CSD_VALUE[2] >> 10) & 0x1F) + 1);	// Get ERASE_GRP_SIZE
					pSDMMCP->EraseSize *= pSDMMCP->WriteBlockSize;
					lgpl_printf("pSDMMCP->EraseSize = 0x%x.\n", pSDMMCP->EraseSize);
				}

				// Now calculate the capacity of this card
				temp = ((pSDMMCP->CardReg.CSD.CSD_VALUE[2] >> 16) & 0xF);		// Get READ_BL_LEN
				temp = 1 << temp;										// Now we have Max Block Length
				temp2 = ((pSDMMCP->CardReg.CSD.CSD_VALUE[1] >> 15) & 0x7) + 2;	// Get C_SIZE_MULT
				temp2 = 1 << temp2;
				temp3 = ((pSDMMCP->CardReg.CSD.CSD_VALUE[1] >> 30) & 0x3);		// Get C_SIZE
				temp3 |= ((pSDMMCP->CardReg.CSD.CSD_VALUE[2] & 0x3FF) << 2);	// Get C_SIZE
				temp3++;
				pSDMMCP->CardCapacity = temp3 * temp2 * temp;		// Total Size of the card in Bytes
			}
			else // Assume CID
			{
				// Copy the CSD values from the buffer
				for (i=0; i<4; i++)
					pSDMMCP->CardReg.CID.CID_VALUE[i] = pSDMMCP->CardReponse.pBuffer[i];

				// Now capture the serial number from the CID - 32 bit number
				if (pSDMMCP->SD == XLLP_MMC)
				{
					pSDMMCP->CardReg.CID.SerialNum = (pSDMMCP->CardReg.CID.CID_VALUE[0] >> 16) & (0xFF);
					pSDMMCP->CardReg.CID.SerialNum |= (pSDMMCP->CardReg.CID.CID_VALUE[1] << 16);
				}
				else
				{
					pSDMMCP->CardReg.CID.SerialNum = (pSDMMCP->CardReg.CID.CID_VALUE[0] >> 24) & (0xFF); //KT
					pSDMMCP->CardReg.CID.SerialNum |= (pSDMMCP->CardReg.CID.CID_VALUE[1] << 8);
				}
			}
			break;
		}
	case MMC_RESPONSE_R3:
	{
		pSDMMCP->CardReg.OCR = pSDMMCP->CardReponse.pBuffer[0];
		break;
	}
	case MMC_RESPONSE_R4:				// These modes are not supported by the driver
	case MMC_RESPONSE_R5:
	case MMC_RESPONSE_R5B:
		break;
	case MMC_RESPONSE_R6:				// Publishes RCA for SD cards
	{
		pSDMMCP->CardReg.RCA = pSDMMCP->CardReponse.pBuffer[0];
		break;
	}
	case MMC_RESPONSE_R7:
	{
		pSDMMCP->SD_VHS = pSDMMCP->CardReponse.pBuffer[0];
		break;
	}
 }

 return Retval;
}



#if 1
/***********************************************************
*    SDMMCReadOneBlock()
*       Reads the given block off of the SD/MMC card and
*       into LocalAddr or empty buffer
*    input:
*		none
*    output:
*       LocalAddr will contain the contents of the block
*    returns:
*       none
************************************************************/

UINT_T MM4_ReadOneBlock(P_SDMMC_Properties_T pSDMMCP)
{
    UINT_T argument;
	UINT_T Retval = NoError;
	P_MM4_SDMMC_CONTEXT_T pContext;
	P_MM4_BLK_CNTL pMM4_BLK_CNTL;

	// Assign our context value
	pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;

	// Must set MMC NUMBLK
	pMM4_BLK_CNTL = (P_MM4_BLK_CNTL) &pContext->pMMC4Reg->mm4_blk_cntl;
	pMM4_BLK_CNTL->blk_cnt = pSDMMCP->Trans.NumBlocks;

	// Set up State
	pSDMMCP->State = READ;

	// Do a CMD 17 Read Single Block
	if (pSDMMCP->SDMA_Mode)
	{
		pMM4_BLK_CNTL->dma_bufsz = MM4_512_HOST_DMA_BDRY;
	    pContext->pMMC4Reg->mm4_sysaddr = pSDMMCP->Trans.LocalAddr;
	}

    argument = pSDMMCP->Trans.CardAddress;


	printf( "\n Send CMD17, data address argument is 0x%x\n ", argument);

	MM4_SendDataCommand(pSDMMCP, XLLP_MMC_CMD17, argument, MM4_SINGLE_BLOCK_TRAN, MM4_CARD_TO_HOST_DATA, MM4_RT_R1 | MM4_48_RES);

	// Wait for the Read to Complete
	while ((pSDMMCP->State != FAULT) && (pSDMMCP->State != READY));		// TBD add timeout Let the ISR run, we'll either get a fault or finish


	// Get the Card Response
	Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1, 0x100);

	if ((Retval != NoError) || ((pSDMMCP->CardReponse.R1_RESP&0xF00) != 0x900))
	{
		Retval = ReadError;
		pSDMMCP->State = FAULT;
		// Send a stop command
		MM4_SendStopCommand(pSDMMCP);
	}
	else
	{
		pSDMMCP->State = READY;
	}

	printf( "\n Send CMD17 done , read one block data done!\n ", argument);

	return Retval;
}

/***********************************************************
*    SDMMCReadBlocks()
*       Reads the given block off of the SD/MMC card and
*       into LocalAddr or empty buffer
*    input:
*		none
*    output:
*       LocalAddr will contain the contents of the block
*    returns:
*       none
************************************************************/

UINT_T MM4_ReadBlocks(P_SDMMC_Properties_T pSDMMCP)
{
    UINT_T argument;
	UINT_T Retval = NoError;
	P_MM4_SDMMC_CONTEXT_T pContext;
	P_MM4_BLK_CNTL pMM4_BLK_CNTL;

	// Assign our context value
	pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;

	// Must set MMC NUMBLK
	pMM4_BLK_CNTL = (P_MM4_BLK_CNTL) &pContext->pMMC4Reg->mm4_blk_cntl;
	pMM4_BLK_CNTL->blk_cnt = pSDMMCP->Trans.NumBlocks;

	// Do a CMD 18 Read Multiple Block
	if (pSDMMCP->SDMA_Mode)
	{
		pMM4_BLK_CNTL->dma_bufsz = MM4_512_HOST_DMA_BDRY;
	    pContext->pMMC4Reg->mm4_sysaddr = pSDMMCP->Trans.LocalAddr;
	}

	argument = pSDMMCP->Trans.CardAddress;



	MM4_SendDataCommand(pSDMMCP, XLLP_MMC_CMD18, argument, MM4_MULTI_BLOCK_TRAN, MM4_CARD_TO_HOST_DATA, MM4_48_RES);

	// Wait for the Read to Complete
	while ((pSDMMCP->State != FAULT) && (pSDMMCP->State != READY));		// TBD add timeout Let the ISR run, we'll either get a fault or finish

	mmc4_wait_card_ready(pSDMMCP);

	// Get the Card Response
	Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1, 0x100);
	// enable cmd complete interrupt
	if ((Retval != NoError) || ((pSDMMCP->CardReponse.R1_RESP&0xF00) != 0x900))
	{
		lgpl_printf("MM4_ReadBlocks fail. Retval = 0x%x, pSDMMCP->CardReponse.R1_RESP = 0x%x.\n", Retval, pSDMMCP->CardReponse.R1_RESP);
		Retval = ReadError;
		pSDMMCP->State = FAULT;
		// Send a stop command
		MM4_SendStopCommand(pSDMMCP);
	}
	else
	{
		pSDMMCP->State = READY;
	}
	//MMC4DmaSelect(pContext,0x00);  // set DMA select back to SDMA
	return Retval;
}


UINT_T SDIOReadOneBlk (P_SDMMC_Properties_T pSDMMCP, UINT_T FlashOffset, UINT_T LocalBuffer, UINT_T Size)
{

	UINT_T Retval = NoError;
    //UINT_T flags = NO_FLAGS;
	// Make sure State is correct
	if (pSDMMCP->State != READY)
		return SDMMCDeviceNotReadyError;

#ifndef MULTI_SDIO
	// Check if Start Address and Size are word aligned
	//if ( ((Size % 4) !=0) || ((FlashOffset % 4) !=0) || ((LocalBuffer % 4) !=0))
	if ( ((Size % 4) !=0) || ((LocalBuffer % 4) !=0))
		return SDMMC_ADDR_MISALIGN_ERROR;
#endif

	// Set up State
	pSDMMCP->State = READ;

	// Does the start/end addresses align on Block Boundries? Probably not, record discard bytes
	pSDMMCP->Trans.CardAddress = FlashOffset;

	if (pSDMMCP->AccessMode == SECTOR_ACCESS) {
		pSDMMCP->Trans.StartDiscardWords = 0;

		if ((Size % pSDMMCP->ReadBlockSize) == 0)
			pSDMMCP->Trans.EndDiscardWords = 0;
		else
			pSDMMCP->Trans.EndDiscardWords = pSDMMCP->ReadBlockSize - (Size % pSDMMCP->ReadBlockSize);
	} else {	// BYTE_ACCESS
		pSDMMCP->Trans.StartDiscardWords = FlashOffset % pSDMMCP->ReadBlockSize;

		if (((FlashOffset + Size) % pSDMMCP->ReadBlockSize) == 0)
			pSDMMCP->Trans.EndDiscardWords = 0;
		else
			pSDMMCP->Trans.EndDiscardWords = pSDMMCP->ReadBlockSize - ((FlashOffset + Size) % pSDMMCP->ReadBlockSize);
	}
	pSDMMCP->Trans.NumBlocks = (pSDMMCP->Trans.EndDiscardWords + pSDMMCP->Trans.StartDiscardWords + Size) / pSDMMCP->ReadBlockSize;
	pSDMMCP->Trans.TransWordSize = pSDMMCP->Trans.NumBlocks * pSDMMCP->ReadBlockSize / 4;		// Total Transfer Size including pre and post bytes

	// Convert to # of words
	pSDMMCP->Trans.StartDiscardWords /=4;
	pSDMMCP->Trans.EndDiscardWords /= 4;

	pSDMMCP->Trans.LocalAddr = LocalBuffer;
	pSDMMCP->Trans.WordIndex = 0;									// Stores Index of Current write position

	// Kick off the Read
	//Retval = pSDMMCP->Funcs.Read_F();
	if(pSDMMCP->Trans.NumBlocks == 1)
			Retval = MM4_ReadOneBlock(pSDMMCP);
	else // multiple blocks
			Retval = MM4_ReadBlocks(pSDMMCP);

//	if (pSDMMCP->State == FAULT)
//		Retval = (SDIOGetCardErrorState(pSDMMCP));			// TBD s/w reset?

	pSDMMCP->State = READY;

	printf( "\n SDIO Read done");

	// read data from FIFO
	//MM4_ReadFifo(pSDMMCP);

	return Retval;
}


/***********************************************************
*    MM4_WriteOneBlock()
*		Writes the required number of blocks to CardAddress
*    input:
*		none
*    output:
*       Address starting with CardAddress will contain content from LocalAddress
*    returns:
*       none
************************************************************/

UINT_T MM4_WriteOneBlock(P_SDMMC_Properties_T pSDMMCP)
{
	UINT_T argument;
	UINT_T Retval = NoError;
	P_MM4_SDMMC_CONTEXT_T pContext;
	P_MM4_BLK_CNTL pMM4_BLK_CNTL;

	// Assign our context value
	pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;

	// Must set MMC NUMBLK
	pMM4_BLK_CNTL = (P_MM4_BLK_CNTL) &pContext->pMMC4Reg->mm4_blk_cntl;
	pMM4_BLK_CNTL->blk_cnt = pSDMMCP->Trans.NumBlocks;

	// Do a CMD 24 Write single Block
	if (pSDMMCP->SDMA_Mode)
	{
		pMM4_BLK_CNTL->dma_bufsz = MM4_512_HOST_DMA_BDRY;
	    pContext->pMMC4Reg->mm4_sysaddr = pSDMMCP->Trans.LocalAddr;
	}

	argument = pSDMMCP->Trans.CardAddress;



	printf( "\n Send CMD24, data address argument is 0x%x\n ", argument);

	MM4_SendDataCommand(pSDMMCP, XLLP_MMC_CMD24, argument, MM4_SINGLE_BLOCK_TRAN, MM4_HOST_TO_CARD_DATA, MM4_RT_R1 | MM4_48_RES);
	//MM4_SendDataCommand(pSDMMCP, XLLP_MMC_CMD24, argument, MM4_MULTI_BLOCK_TRAN, MM4_HOST_TO_CARD_DATA, MM4_48_RES);

	// Wait for the Write to Complete
	while ((pSDMMCP->State != FAULT) && (pSDMMCP->State != READY));		// TBD add timeout Let the ISR run, we'll either get a fault or finish

	// Get the Card Response
	Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1, 0x100);

	if ((Retval != NoError) || (pSDMMCP->State == FAULT) || ((pSDMMCP->CardReponse.R1_RESP&0xF00) != 0x900))
	{
		Retval = WriteError;
		pSDMMCP->State = FAULT;
		// Send a stop command
		MM4_SendStopCommand(pSDMMCP);

		lgpl_printf( "\n CMD24 complete, write one block data error !\n ");
	}
	else
	{
		pSDMMCP->State = READY;
	}

	printf( "\n CMD24 complete, write one block data done !\n ");

	return Retval;
}

/***********************************************************
*    MM4_WriteBlocks()
*		Writes the required number of blocks to CardAddress
*    input:
*		none
*    output:
*       Address starting with CardAddress will contain content from LocalAddress
*    returns:
*       none
************************************************************/

UINT_T MM4_WriteBlocks(P_SDMMC_Properties_T pSDMMCP)
{
	UINT_T argument;
	UINT_T Retval = NoError;
	P_MM4_SDMMC_CONTEXT_T pContext;
	P_MM4_BLK_CNTL pMM4_BLK_CNTL;

	// Assign our context value
	pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;

	// Must set MMC NUMBLK
	pMM4_BLK_CNTL = (P_MM4_BLK_CNTL) &pContext->pMMC4Reg->mm4_blk_cntl;
	pMM4_BLK_CNTL->blk_cnt = pSDMMCP->Trans.NumBlocks;

	// Do a CMD 25 Write Multiple Blocks
	if (pSDMMCP->SDMA_Mode)
	{

		printf( "\n MM4_DMA mode\n ");
		printf(" pSDMMCP->Trans.LocalAddr = 0x%x\n", pSDMMCP->Trans.LocalAddr) ;
		pMM4_BLK_CNTL->dma_bufsz = MM4_512_HOST_DMA_BDRY;
	    pContext->pMMC4Reg->mm4_sysaddr = pSDMMCP->Trans.LocalAddr;
	}

    argument = pSDMMCP->Trans.CardAddress;


	lgpl_printf( "MM4_WriteBlocks, write 0x%x blocks to 0x%x\n ", pMM4_BLK_CNTL->blk_cnt, argument);


    // only need xfer interrupt -KT
#ifndef MULTI_SDIO
//	SDIOEnableCmdInterrupt(0);
#else
//	SDIOEnableCmdInterrupt(uchCardId ,0);
#endif

	//MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD23, MM4_CMD_TYPE_NORMAL, pMM4_BLK_CNTL->blk_cnt, MM4_RT_R1 | MM4_48_RES);
	//Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1, 0x100);

	//pSDMMCP->State = WRITE;
	MM4_SendDataCommand(pSDMMCP, XLLP_MMC_CMD25, argument, MM4_MULTI_BLOCK_TRAN, MM4_HOST_TO_CARD_DATA, MM4_RT_R1 | MM4_48_RES);

	// Wait for the Write to Complete
	while ((pSDMMCP->State != FAULT) && (pSDMMCP->State != READY));		// TBD add timeout Let the ISR run, we'll either get a fault or finish
	//SDIOGetCardErrorState(pSDMMCP) ;
	// Get the Card Response
	Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1, 0x100);
	mmc4_wait_card_ready(pSDMMCP);
	// Retval |= MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1, 0x100);
	Retval |= MM4_CheckCardStatus(pSDMMCP, 0x900, R1_LOCKEDCARDMASK);
	if ((Retval != NoError) || (pSDMMCP->State == FAULT) || ((pSDMMCP->CardReponse.R1_RESP &0xF00) != 0x900))
	{
		lgpl_printf("%s(), line %d: Retval = 0x%x, state = 0x%x, R1_RESP = 0x%x.\n", 
			__FUNCTION__, __LINE__, Retval, pSDMMCP->State, pSDMMCP->CardReponse.R1_RESP);
		Retval = WriteError;
		pSDMMCP->State = FAULT;
		// Send a stop command
		MM4_SendStopCommand(pSDMMCP);
	}
	else
	{
		pSDMMCP->State = READY;
	}
	return Retval;
}



/***********************************************************
*    SDIOWriteOneBlk()
*    input:
*	 Description:
*		This function will write as many bytes as specified from LocalBuffer to Flash
*		Offset. The flash driver must have been previously initialized using
*		InitializeSDMMCDriver. The relavent addresses must have been previously erased.
*		If the function encounters an error it will not
*		re-attempt the operation.
*    Inputs:
*	   FlashOffset - The source address on the flash where data will be written to
*			specified as a byte value. Must be 32 bits aligned
*		LocalBuffer - Local address where data will be copied from.
*		Size - Specifies number of bytes to write - 32 bits aligned
*		FlashBootType - Normal or Save State flash
*    output:
*       Desired Values are written to flash
*    returns:
*       WTPTP recognized errors
************************************************************/


UINT_T SDIOWriteOneBlk (P_SDMMC_Properties_T pSDMMCP, UINT_T FlashOffset, UINT_T LocalBuffer, UINT_T Size)
{
	UINT_T Retval = NoError;
	    //UINT_T flags = NO_FLAGS;
	// Initialize Flash Properties

	// Make sure State is correct
	if (pSDMMCP->State != READY)
		return SDMMCDeviceNotReadyError;

#ifndef MULTI_SDIO
	// Check if Start Address and Size are word aligned
	if ( ((Size % 4) !=0) || ((FlashOffset % 4) !=0) || ((LocalBuffer % 4) !=0))
		return SDMMC_ADDR_MISALIGN_ERROR;
#endif


	// Set up State
	pSDMMCP->State = WRITE;

	// Does the start/end addresses align on Block Boundries? Probably not, record discard bytes
	pSDMMCP->Trans.CardAddress = FlashOffset;
	if (pSDMMCP->AccessMode == SECTOR_ACCESS) {
		pSDMMCP->Trans.StartDiscardWords = 0;
		//pSDMMCP->Trans.EndDiscardWords = Size / pSDMMCP->WriteBlockSize * pSDMMCP->WriteBlockSize - Size;
		if ((Size % pSDMMCP->WriteBlockSize) == 0)
			pSDMMCP->Trans.EndDiscardWords = 0;
		else
			pSDMMCP->Trans.EndDiscardWords = pSDMMCP->WriteBlockSize - (Size % pSDMMCP->WriteBlockSize);
	} else {	// BYTE_ACCESS
		pSDMMCP->Trans.StartDiscardWords = FlashOffset % pSDMMCP->WriteBlockSize;

		if (((FlashOffset + Size) % pSDMMCP->WriteBlockSize) == 0)
			pSDMMCP->Trans.EndDiscardWords = 0;
		else
			pSDMMCP->Trans.EndDiscardWords = pSDMMCP->WriteBlockSize - ((FlashOffset + Size) % pSDMMCP->WriteBlockSize);
	}
	pSDMMCP->Trans.NumBlocks = (pSDMMCP->Trans.EndDiscardWords + pSDMMCP->Trans.StartDiscardWords + Size) / pSDMMCP->WriteBlockSize;
	pSDMMCP->Trans.TransWordSize = pSDMMCP->Trans.NumBlocks * pSDMMCP->WriteBlockSize / 4; 		// Total Transfer Size including pre and post, in words

	// Convert to # of words
	pSDMMCP->Trans.StartDiscardWords /= 4;
	pSDMMCP->Trans.EndDiscardWords /= 4;
	pSDMMCP->Trans.LocalAddr = LocalBuffer;
	pSDMMCP->Trans.WordIndex = 0;									// Stores Index of Current write position

	if(pSDMMCP->Trans.NumBlocks == 1)
		Retval = MM4_WriteOneBlock(pSDMMCP);
	else
		Retval = MM4_WriteBlocks(pSDMMCP);


	pSDMMCP->State = READY;

	printf( "\n SDIO Write done.");

	return Retval;
}

int SDIOErase(P_SDMMC_Properties_T pSDMMCP, unsigned int start, unsigned int end)
{
	UINT_T Retval = NoError;
	UINT_T Argument = 0;

	// Make sure State is correct
	if (pSDMMCP->State != READY)
		return SDMMCDeviceNotReadyError;

	// Does the start/end addresses align on Block Boundries? Probably not, record discard bytes
	pSDMMCP->Trans.CardAddress = start;

	Argument = pSDMMCP->Trans.CardAddress;

	MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD35, MM4_CMD_TYPE_NORMAL, Argument, MM4_RT_R1 | MM4_48_RES);
	Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1, 0x100);
	if (Retval != NoError) {
		pSDMMCP->State = FAULT;
		lgpl_printf("Fail to send XLLP_MMC_CMD35.state = 0x%x.\n", (pSDMMCP->CardReponse.R1_RESP));
		return EraseError;
	}

	Argument = end;

	MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD36, MM4_CMD_TYPE_NORMAL, Argument, MM4_RT_R1 | MM4_48_RES);
	Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1, 0x100);
	if (Retval != NoError) {
		pSDMMCP->State = FAULT;
		lgpl_printf("Fail to send XLLP_MMC_CMD36.state = 0x%x.\n", (pSDMMCP->CardReponse.R1_RESP));
		return EraseError;
	}

	// Set up State
	pSDMMCP->State = ERASE;

	Argument = 0;

	MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD38, MM4_CMD_TYPE_NORMAL, Argument, MM4_RT_R1 | MM4_RT_BUSY | MM4_48_RES_WITH_BUSY);
	Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1, 0x100);
	if (Retval != NoError) {
		pSDMMCP->State = FAULT;
		lgpl_printf("Fail to send XLLP_MMC_CMD38.state = 0x%x.\n", (pSDMMCP->CardReponse.R1_RESP));
		return EraseError;
	}

	Retval = mmc4_wait_card_ready(pSDMMCP);
	Retval |= MM4_CheckCardStatus(pSDMMCP, 0x900, R1_LOCKEDCARDMASK);
	if ((Retval != NoError)) {
		lgpl_printf("%s(), line %d: Send XLLP_MMC_CMD38 timeout.\n", __func__, __LINE__);
		return Retval;
	}

	return Retval;
}

int SDIOWrite(P_SDMMC_Properties_T pSDMMCP, unsigned int start, unsigned int blk, unsigned char *puchWriteBuff)
{
	int rtn;
	int uiCount = 0;
	rtn = SDIOWriteOneBlk(pSDMMCP , start, puchWriteBuff, 0x200 * blk);
	return rtn ;
}


#endif


#if 1
/******************************************************************************
  Description:
    Set up the registers of the controller to start the transaction to
    communicate to the card for setup related commands.
    The commands are clearly defined in the MMC specification.
  Input Parameters:
	   P_SDMMC_Properties_T pSDMMCP - Generic SD/MMC driver properties structure
	Cmd
		Command Index - See MMC or SD specification
    argument
	the argument of  the command. MSW is for ARGH and LSW is for ARGL
	ResType
		Expected response type
  Output Parameters:
    None
  Returns:
    None
*******************************************************************************/
////////////////////////////////////////////////////////////////////
//
// [127:8] --> REG[119:0]
//
////////////////////////////////////////////////////////////////////
void MM4_Get_CID(P_SDMMC_Properties_T pSDMMCP)
{
    unsigned int   m_cmdResponse[4];
    m_cmdResponse[0]=pSDMMCP->CardReponse.pBuffer[0];
    m_cmdResponse[1]=pSDMMCP->CardReponse.pBuffer[1];
    m_cmdResponse[2]=pSDMMCP->CardReponse.pBuffer[2];
    m_cmdResponse[3]=pSDMMCP->CardReponse.pBuffer[3];


	////////////////////////////////////////////////////////
    // decode m_cid fields: high 8 bits ignored
    // Note: card information is not used by this driver. If
    //       OEM asks for this, add an API.
	////////////////////////////////////////////////////////
    m_cid.manufactureID = (unsigned char)(m_cmdResponse[3]>>16&0x00ff);
    m_cid.OEM[0] = (m_cmdResponse[3]>>8 &0xff);
    m_cid.OEM[1] = (m_cmdResponse[3]&0xff);
    m_cid.OEM[2] = 0;
    m_cid.name[0] = (m_cmdResponse[2]>>24 &0xff);
    m_cid.name[1] = (m_cmdResponse[2]>>16 &0xff);
    m_cid.name[2] = (m_cmdResponse[2]>>8 &0xff);
    m_cid.name[3] = (m_cmdResponse[2] &0xff);
    m_cid.name[4] = (m_cmdResponse[1]>>24 &0xff);;


    m_cid.name[5] = 0;
    m_cid.revision = (m_cmdResponse[1]>>16 &0xff);;
    m_cid.serialNumber =(m_cmdResponse[1] &0xFFFF);
    m_cid.serialNumber <<= 16;
    m_cid.serialNumber |= (m_cmdResponse[0]>>16 &0xFFFF);
    m_cid.manufactureDate = (m_cmdResponse[0] &0xFFF);;


#if 1	// debug
	printf("\n CID:");
	printf("\n %08x %08x %08x %08x:",m_cmdResponse[0],m_cmdResponse[1],m_cmdResponse[2],m_cmdResponse[3]);
	//printf("\n %04x %04x %04x %04x:",SDIO_Response[0],SDIO_Response[1],SDIO_Response[2],SDIO_Response[3]);
	//printf("\n %04x %04x %04x %04x:",SDIO_Response[4],SDIO_Response[5],SDIO_Response[6],SDIO_Response[7]);

	lgpl_printf("Manufacture ID:%d\n",m_cid.manufactureID);
	lgpl_printf("Product name:%s\n",m_cid.name);
	printf("\n Serial Number:%x",m_cid.serialNumber);
    {
	unsigned short month,year;
	month = m_cid.manufactureDate&0xF;
	year = ((m_cid.manufactureDate&0x0FF0)>>4)+2000;
	printf("\n month=%d, year=%d\n",month,year);
	}
#endif
}



void MM4_Get_SCR(P_SDMMC_Properties_T pSDMMCP)
{
    SDIO_SCR m_scr;
	m_scr.structure=pSDMMCP->CardReg.SCR.SCR_VALUE[0] >>4 &0x0F;
    m_scr.specVersion=pSDMMCP->CardReg.SCR.SCR_VALUE[0] &0x0F;
    m_scr.dataStatus=pSDMMCP->CardReg.SCR.SCR_VALUE[0]>>15 & 0x1;
    m_scr.security=pSDMMCP->CardReg.SCR.SCR_VALUE[0]>>12 & 0x07;
    m_scr.busWidth=pSDMMCP->CardReg.SCR.SCR_VALUE[0]>>8 &0x0F ;
    m_scr.reserved=pSDMMCP->CardReg.SCR.SCR_VALUE[0]>>16 &0xFFFF;
    m_scr.manufatureReserved=pSDMMCP->CardReg.SCR.SCR_VALUE[1];

    printf("\n SCR :");
    printf("\n structure= %x specVersion= %x dataStatus= %x",m_scr.structure,m_scr.specVersion,m_scr.dataStatus);
    printf("\n security = %x busWidth= %x manufatureRes= 0x%x",m_scr.security,m_scr.busWidth,m_scr.manufatureReserved);

}

#endif

int SDIORead(P_SDMMC_Properties_T pSDMMCP, unsigned int start, unsigned int blk, unsigned char *puchReadBuff)
{
    int rtn;

	int uiCount = 0;
	rtn = SDIOReadOneBlk(pSDMMCP, start, puchReadBuff, 0x200 * blk);
#if 0
	printf(PRN_DBG, "\n The Read Data is:\n");

	for(uiCount=0;uiCount <0x200 * blk; uiCount++){
		printf( "0x%02x\t ",puchReadBuff[uiCount]);
		if(uiCount%8 == 7)printf(PRN_DBG, "\n",puchReadBuff[uiCount]);
	}
#endif
    return rtn;
}



/****************************************************************
*    MM4_SetControllerVoltage
*		Inspects the Capabilities Register for supported voltage types by the
*		controller. Then programs the CNTL1 register with the desired range.
*		Enables bus power
*    Input:
*		P_MM4_SDMMC_CONTEXT_T pContext
*    Output:
*		none
*    Returns:
*       none
*****************************************************************/
UINT_T MM4_SetControllerVoltage (P_MM4_SDMMC_CONTEXT_T pContext)
{
	 UINT_T controllervoltage;
	 P_MM4_CNTL1 pMM4_CNTL1 = (P_MM4_CNTL1) &pContext->pMMC4Reg->mm4_cntl1;
	 MM4_CNTL1_UNION MM4_cntl1;

	 // Capture the Value
	 MM4_cntl1.mm4_cntl1_value = *(VUINT_T*) pMM4_CNTL1;

	 // Note that this really doesn't control voltage, it just needs to match
	 // one of the supported values in the capabilities 2 register.
	 controllervoltage = MM4_VLTGSEL_3_3;

	 // Set the voltage to controller
	 MM4_cntl1.mm4_cntl1_bits.vltgsel = controllervoltage;

	 // Enable Bus Power
	 MM4_cntl1.mm4_cntl1_bits.buspwr = 1;

	 // Write back out.
	 *(VUINT_T*) pMM4_CNTL1 = MM4_cntl1.mm4_cntl1_value;

	 return controllervoltage;
}
/****************************************************************
*   SDMMCGetMatchWaitCardStatus
*	Gets the status of the card by issuing CMD 13. The rerturn from the routine is based
*		on a check against the expected value which is passed in
*   Input:
*	pSDMMCP - pointer to the current context
*	MaxWaitMSec - Maximum wait time in millisec
*		R1_Resp_Match - Desired Value to be matched
*   Output:
*	none
*   Returns:
*	none
*****************************************************************/
UINT_T MM4_CheckCardStatus(P_SDMMC_Properties_T pSDMMCP, UINT_T R1_Resp_Match, UINT_T Mask)
{
    UINT_T flags, argument, cardStatus, retval;

	//send CMD13 to check the status of the card
	argument = pSDMMCP->CardReg.RCA;
	MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD13, MM4_CMD_TYPE_NORMAL, argument, MM4_RT_R1 | MM4_48_RES);
	retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1, 0x100);

	// Mask out undesired check bits
	cardStatus = (pSDMMCP->CardReponse.R1_RESP) & Mask;

	if ((cardStatus == R1_Resp_Match) && (retval == NoError))
		return NoError;
	else{
		lgpl_printf( "\n check state error, state is suppose to be 0x%x \n", R1_Resp_Match);
		lgpl_printf( "\n Current state is 0x%x.\n", cardStatus);
		return TimeOutError ;
	}
}

#if 1
/****************************************************************
*    MM4_CheckVoltageCompatibility
*		Checks to make sure that the OCR register of the device supports the
*		voltage range that was selected for the controller
*    Input:
*		P_MM4_SDMMC_CONTEXT_T pContext, UINT_T ControllerVoltage
*    Output:
*		none
*    Returns:
*       none
*****************************************************************/
UINT_T MM4_CheckVoltageCompatibility(P_SDMMC_Properties_T pSDMMCP, UINT_T ControllerVoltage)
{
	// Check SD vs MMC
	if (pSDMMCP->SD == XLLP_SD)
	{
		switch (ControllerVoltage)
		{
			case MM4_VLTGSEL_3_3:
				if (pSDMMCP->CardReg.OCR & SD_OCR_VOLTAGE_3_3_TO_3_6)
					return NoError;

			case MM4_VLTGSEL_3_0:
				if (pSDMMCP->CardReg.OCR & SD_OCR_VOLTAGE_1_8_TO_3_3)
					return NoError;

			case MM4_VLTGSEL_1_8:
				if (pSDMMCP->CardReg.OCR & SD_OCR_VOLTAGE_1_8)
					return NoError;
		}
	}
	else
	{
		if ((pSDMMCP->CardReg.OCR & MMC_OCR_VOLTAGE_ALL) == MMC_OCR_VOLTAGE_ALL)
			return NoError;
	}
	return SDMMCDeviceVoltageNotSupported;
}

#endif	//	end of function MM4_CheckVoltageCompatibility


#if 1
/***************************************************************
*   MM4SetBusWidth()
*	Sets the Bus width highest bus width supported.
*   Input:
*   Output:
*   Returns: NoError, ReadError or NotSupportedError
*
*****************************************************************/
UINT_T MM4SetBusWidth(P_SDMMC_Properties_T pSDMMCP)
{
	P_MM4_SDMMC_CONTEXT_T pContext;
	P_MM4_CNTL1 pMM4_CNTL1;
	UINT_T Retval = NoError;

	pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;

	// Assign our context value
	pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;
	pMM4_CNTL1 = (P_MM4_CNTL1) &pContext->pMMC4Reg->mm4_cntl1;

	Retval = mmc4_switch(pSDMMCP, 0, BUS_WIDTH_MMC_EXT_CSD_OFFSET, 2);
	if (Retval != NoError) {
		lgpl_printf("Send XLLP_MMC_CMD6 to set bus width fail.\n");
		return Retval;
	}

	// Now change the controller to boost bus width

	pMM4_CNTL1->ex_data_width = 1;		// Move to 8-bit mode.

	return NoError;
}

#endif		//	end of MM4SetBusWidth


#if 1
/**********************************************************
*   MM4_IDCard
*      Identifies which type of card was inserted
*   Input:
*	   none
*   Output:
*      none
*   Returns:
*      WTP recoginized Success/Fail return code
***********************************************************/
unsigned int MM4_IDCard(P_SDMMC_Properties_T pSDMMCP, UINT_T *pControllerVoltage)
{
	// Local Variables
	UINT_T startTime, endTime, elapsedSec;
	P_MM4_SD_CE_ATA_1_2 pMM4_SD_CE_ATA_1_2;
	UINT_T AnyError = 0;
	UINT_T argument = 0;
	int CardFound = FALSE;
	UINT_T HighCapacity = 0;
	UINT_T attempts = 0;
	volatile int stop_flag = 1;
	// Assign our context value
	P_MM4_SDMMC_CONTEXT_T pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;

	// Assign some context
	pMM4_SD_CE_ATA_1_2 = (P_MM4_SD_CE_ATA_1_2)&pContext->pMMC4Reg->mm4_sd_ce_ata_1_2;

	// Enable power,	pControllerVoltage will be returned to up-level function for volatage checking.
	*pControllerVoltage = MM4_SetControllerVoltage(pContext);
	pContext->pMMC4Reg->mm4_blk_cntl = 0;

	// Check for High Capacity Cards First
	// Send CMD0 (GO_IDLE_STATE) to get card into idle state
	pMM4_SD_CE_ATA_1_2->mmc_card = 1 ;
	printf("\n send CMD0 (GO_IDLE_STATE) to get card into idle state\n");
	MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD0, MM4_CMD_TYPE_NORMAL, argument, MM4_RT_NONE | MM4_NO_RES);
	AnyError = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_NONE, 0x10);
	if (AnyError != NoError) {
		lgpl_printf("Send CMD0 (GO_IDLE_STATE) Fail.\n");
	}

	pSDMMCP->SD = XLLP_MMC;

	// First time, pass NULL argument to get back values card is compatible with
	// Send appropriate CMD Sequence to Identify the type of card inserted
	argument = 0;
	//argument = 0x40000000;    // set HCS bit for v2.0 SDHC -KT
	pSDMMCP->CardReg.OCR = 0;			// Make sure to clear out OCR.

	do
	{
		printf( "Send XLLP_MMC_CMD1 to MMC card\n");
		pMM4_SD_CE_ATA_1_2->mmc_card = 1;
		MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD1, MM4_CMD_TYPE_NORMAL, argument, MM4_48_RES);
		AnyError = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R3, 0x10);
		if (AnyError != NoError) {
			lgpl_printf("Send XLLP_MMC_CMD1 Fail.\n");
		}

		if (pSDMMCP->CardReg.OCR == 0)
		{
			CardFound = FALSE;
		}
		else
		{
			CardFound = TRUE;
		}
		//if (cnt<20) argument = pSDMMCP->CardReg.OCR | 0x40000000;   // set HCS bit
		//else argument = pSDMMCP->CardReg.OCR;
		argument = pSDMMCP->CardReg.OCR | 0x40000000;
		printf( "CMD1 OCR=0x%08x\n", argument);

	} while (((pSDMMCP->CardReg.OCR & 0x80000000) != 0x80000000));

	HighCapacity = ((pSDMMCP->CardReg.OCR &0x40000000) !=0);   // check OCR bit 30 (CCS )

	// Assign Access Mode.
	if (pSDMMCP->CardReg.OCR & OCR_ACCESS_MODE_MASK)
		pSDMMCP->AccessMode = SECTOR_ACCESS;
	else
		pSDMMCP->AccessMode = BYTE_ACCESS;

	printf("\n\n CMD1 Card OCR=0x%08x SDHC=%d\n",pSDMMCP->CardReg.OCR, HighCapacity);
	return NoError;
}

////////////////////////////////////////////////////////////////////////
//
//  Get card CSD
////////////////////////////////////////////////////////////////////////
void MM4_Get_CSD(P_SDMMC_Properties_T pSDMMCP)
{
	//unsigned short SDIO_Response[8];
	unsigned int   m_cmdResponse[4];
    SDIO_CSD m_csd;
    m_cmdResponse[0] = pSDMMCP->CardReponse.pBuffer[0];
    m_cmdResponse[1] = pSDMMCP->CardReponse.pBuffer[1];
    m_cmdResponse[2] = pSDMMCP->CardReponse.pBuffer[2];
    m_cmdResponse[3] = pSDMMCP->CardReponse.pBuffer[3];

	////////////////////////////////////////////////////////
    // decode m_csd data
	////////////////////////////////////////////////////////
    m_csd.structure = (m_cmdResponse[3]>> 22) & 0x03;

    m_csd.specificationVersion = (m_cmdResponse[3]>> 16) & 0x3F;

    m_csd.dataReadAccessTime1 = (m_cmdResponse[3] >> 8) & 0xFF ;
    m_csd.dataReadAccessTime2 = (m_cmdResponse[3]) & 0xFF ;

    /////////////////////////////////////////////////////////

    m_csd.maxDataTransferRate = (m_cmdResponse[2] >> 24) & 0xFF;
    m_csd.cardCommandClass =(m_cmdResponse[2] >> 12) & 0xFFF;
    //m_csd.cardCommandClass <<= 4;
    //m_csd.cardCommandClass |= ((SDIO_Response[3] >> 12) & 0x000f);

    m_csd.readBlockLength = (m_cmdResponse[2] >> 8) & 0x0F;
    m_csd.partialReadFlag = (m_cmdResponse[2] >> 7) & 0x01;;
    m_csd.writeBlockMisalign = (m_cmdResponse[2] >> 6) & 0x01;
    m_csd.readBlockMisalign = ((m_cmdResponse[2] >> 5)& 0x0001);
    m_csd.DSR_implemented = ((m_cmdResponse[2] >> 4)& 0x0001);
    // 2 bit reserved
    m_csd.deviceSize = (m_cmdResponse[2]) & 0x03;
    m_csd.deviceSize <<= 10;
    m_csd.deviceSize |= (m_cmdResponse[1] >> 22) & 0x3FF;

    ///////////////////////////////////////////////////////////
    m_csd.maxreadCurrent_VddMin = (m_cmdResponse[1] >> 19) & 0x07;
    m_csd.maxReadCurrent_VddMax = (m_cmdResponse[1] >> 16) & 0x07;
    m_csd.maxWriteCurrent_VddMin= (m_cmdResponse[1] >> 13) & 0x07;
    m_csd.maxWriteCurrent_VddMax= (m_cmdResponse[1] >> 10) & 0x07;
    m_csd.deviceSizeMultiplier = (m_cmdResponse[1] >> 7) & 0x07;
    m_csd.eraseBlockEnable =  (m_cmdResponse[1] >> 6) & 0x01;             // 1 bit
    m_csd.eraseSectorSize = (m_cmdResponse[1]) & 0x3F;;                 // 7 bits:6+1
    m_csd.eraseSectorSize <<= 6;
    m_csd.eraseSectorSize |= (m_cmdResponse[0] >> 31) & 0x01;

    /////////////////////////////////////////////////////////////
    m_csd.WP_GroupSize =   (m_cmdResponse[0] >> 24) & 0x7F;                 // 7 bits
    m_csd.WP_GroupEnable = (m_cmdResponse[0] >> 23) & 0x01;
    // 2 bit reserved
    m_csd.writeSpeedFactor = (m_cmdResponse[0] >> 18) & 0x07;              // 3 bits
    m_csd.writeBlockLength = (m_cmdResponse[0] >> 14) & 0x0F;              // 4 bits:

    m_csd.partialWriteFlag = (m_cmdResponse[0] >> 13) & 0x01;             // 1 bit
    // 5 bits reserved
    m_csd.fileFormatGroup = (m_cmdResponse[0]>> 7) & 0x01;
    m_csd.copyFlag = (m_cmdResponse[0] >> 6) & 0x01;
    m_csd.WP_perm =  (m_cmdResponse[0] >> 5) & 0x01;
    m_csd.WP_temp = (m_cmdResponse[0] >> 4) & 0x01;
    m_csd.fileFormat = (m_cmdResponse[0] >> 2) & 0x03;;
                                         // CRC is ignored

#if 1	// debug
	{
	dbg_printf(PRN_INFO,"\n\n CSD: %d",pSDMMCP->CardCapacity);
	dbg_printf(PRN_INFO,"\n %08x %08x %08x %08x:",m_cmdResponse[0],m_cmdResponse[1],m_cmdResponse[2],m_cmdResponse[3]);
	//dbg_printf(PRN_RES,"\n %04x %04x %04x %04x:",SDIO_Response[0],SDIO_Response[1],SDIO_Response[2],SDIO_Response[3]);
	//dbg_printf(PRN_RES,"\n %04x %04x %04x %04x:",SDIO_Response[4],SDIO_Response[5],SDIO_Response[6],SDIO_Response[7]);
	if(m_csd.structure==0)
	{
	unsigned int mult = 1<<(m_csd.deviceSizeMultiplier+2);
	unsigned int capacity= (m_csd.deviceSize+1) * mult * (1<<m_csd.readBlockLength);
	dbg_printf(PRN_INFO,"\n Device Size: %d = c_size(%d) x mult(%d) x blk_len(%d)",capacity,(m_csd.deviceSize+1),mult,(1<<m_csd.readBlockLength));

	}
	else
	{
		//v2.0
		m_csd.deviceSize = (m_cmdResponse[1])>>8 &0x03FFFFF;
		unsigned int capacity= (m_csd.deviceSize+1) *512;
		dbg_printf(PRN_INFO,"\n detected CSD v2.0 SDHC csd_structure=%x",m_csd.structure);
		dbg_printf(PRN_INFO,"\n Device Size: %d KB",capacity);

	}
	//dbg_printf(PRN_INFO,"\n Device Size: %d = c_size(%d) x mult(%d) x blk_len(%d)",capacity,(m_csd.deviceSize+1),mult,(1<<m_csd.readBlockLength));

	dbg_printf(PRN_INFO,"\n command class:%x",m_csd.cardCommandClass);
	dbg_printf(PRN_INFO,"\n Access time(TAAC):unit=%d,value=%d",m_csd.dataReadAccessTime1&0x07,(m_csd.dataReadAccessTime1&0x78)>>3);
	dbg_printf(PRN_INFO,"\n Access time(NSAC):%d",m_csd.dataReadAccessTime2);
	dbg_printf(PRN_INFO,"\n Max transfer rate:%x",m_csd.maxDataTransferRate);
//	dbg_printf(PRN_RES,"\n Max transfer rate:unit=%d,value=%d",m_csd.maxDataTransferRate&0x07,(m_csd.maxDataTransferRate&0x78)>>3);
	dbg_printf(PRN_INFO,"\n Read block len:%d",1<<m_csd.readBlockLength);
	dbg_printf(PRN_INFO,"\n Write block len:%d",1<<m_csd.writeBlockLength);
	dbg_printf(PRN_INFO,"\n Erase sector size:%d",m_csd.eraseSectorSize+1);
	dbg_printf(PRN_INFO,"\n File format:%d",m_csd.fileFormat);
	dbg_printf(PRN_INFO,"\n DSR implemented:%d",m_csd.DSR_implemented);
	}
#endif


}

static int mmc4_wait_card_ready(P_SDMMC_Properties_T pSDMMCP)
{
	int timeout = 0x800000;

    UINT_T argument, cardStatus, retval;

	while (timeout) {
		//send CMD13 to check the status of the card
		argument = pSDMMCP->CardReg.RCA;
		MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD13, MM4_CMD_TYPE_NORMAL, argument, MM4_RT_R1 | MM4_48_RES);
		retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1, 0x100);

		// Mask out undesired check bits
		cardStatus = (pSDMMCP->CardReponse.R1_RESP);

		if ((cardStatus & 0x100) && (retval == NoError))
			return NoError;

		timeout--;
	}

	return TimeOutError;
}

static int mmc4_switch(P_SDMMC_Properties_T pSDMMCP, unsigned char set, unsigned char index, unsigned char value)
{
	int err;
	unsigned int arg = 0;

	arg = (MMC_SWITCH_MODE_WRITE_BYTE << 24) | (index << 16) | (value << 8) | set;

	MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD6, MM4_CMD_TYPE_NORMAL, arg, MM4_RT_R1 | MM4_RT_BUSY | MM4_48_RES_WITH_BUSY);
	err = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1B, 0x10);
	if (err != NoError) {
		printf("%s(), line %d: Send XLLP_MMC_CMD6 error.\n", __func__, __LINE__);
		return err;
	}

	err = mmc4_wait_card_ready(pSDMMCP);
	if ((err != NoError)) {
		printf("%s(), line %d: Send XLLP_MMC_CMD6 timeout.\n", __func__, __LINE__);
		return err;
	}

	return NoError;
}

static int mmc4_set_hs(P_SDMMC_Properties_T pSDMMCP)
{
	int err = 0;

	err = mmc4_switch(pSDMMCP, EXT_CSD_CMD_SET_NORMAL, HS_TIMING_MMC_EXT_CSD_OFFSET, 1);

	return err;
}

/**********************************************************
*   MM4_CardInit
*      Initializes the inserted card
*   Input:
*	   none
*   Output:
*      none
*   Returns:
*      WTP recoginized Success/Fail return code
***********************************************************/


UINT_T MM4_CardInit(P_SDMMC_Properties_T pSDMMCP)
{
	UINT_T argument, Retval;
	UINT_T controllervoltage;

	P_MM4_SDMMC_CONTEXT_T pContext;
	P_MM4_BLK_CNTL pMM4_BLK_CNTL;


	// Assign our context value
	pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;

	//Must set MMC NUMBLK
	pMM4_BLK_CNTL = (P_MM4_BLK_CNTL) &pContext->pMMC4Reg->mm4_blk_cntl;
	pMM4_BLK_CNTL->blk_cnt = 1;
	pMM4_BLK_CNTL->xfr_blksz =512; //512 byte

#if SDIO_DMA
	pSDMMCP->SDMA_Mode = TRUE;
#else
	pSDMMCP->SDMA_Mode = FALSE;
#endif

	printf("\n MM4_CardInit: go to identifies card\n");
    Retval = MM4_IDCard(pSDMMCP, &controllervoltage);
	if (Retval != NoError){
		lgpl_printf(" MM4_IDCard: fail.\n");
		return Retval;
	}
	// Set up State
	pSDMMCP->State = INITIALIZE;

	// SD and MMC joint functionality again
	// At this point we should have our OCR contents. See if they match the voltage range we choose for the controller
	Retval = MM4_CheckVoltageCompatibility(pSDMMCP, controllervoltage);
	if (Retval != NoError) {
		lgpl_printf("MM4_CheckVoltageCompatibility fail.\n");
		return SDMMCInitializationError; // We couldn't find any cards
	}

	//send CMD2 to get the CID numbers
	argument = NO_ARGUMENT;
	MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD2, MM4_CMD_TYPE_NORMAL, argument, MM4_RT_R2| MM4_136_RES);
	Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R2, 0x100);
	if (Retval != NoError) {
		lgpl_printf("Send XLLP_MMC_CMD2 fail.\n");
		return SDMMCInitializationError;
	}

	//	3/16:	Xiufeng:	Do not touch this function
	//					Since there are some difference in the definition of device CID registers.
	//					This function need to double check for MMC realted settings.
    MM4_Get_CID(pSDMMCP);   //KT added ; print out CID

	// Next its CMD3 to assign an RCA to the cards
	pSDMMCP->CardReg.RCA = pSDMMCP->CardReg.CID.SerialNum;
	argument = pSDMMCP->CardReg.RCA;
	MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD3, MM4_CMD_TYPE_NORMAL, argument, MM4_RT_R1 | MM4_48_RES);
	Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1, 0x100);
	if (Retval != NoError) {
		lgpl_printf("Send XLLP_MMC_CMD3 fail.\n");
		return SDMMCInitializationError;
	}

	lgpl_printf("CID_SerialNum=%08x  RCA=%x.\n",pSDMMCP->CardReg.CID.SerialNum,pSDMMCP->CardReg.RCA);

	//send CMD13 to check the status of the card
	Retval = MM4_CheckCardStatus(pSDMMCP, 0x700, R1_LOCKEDCARDMASK);		// Make sure card is stdby mode
	if (Retval != NoError) {
		lgpl_printf("line %d:MM4_CheckCardStatus fail.\n", __LINE__);
		return SDMMCInitializationError;
	}

	// Send CMD 9 to retrieve the CSD
	argument = pSDMMCP->CardReg.RCA;
	MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD9, MM4_CMD_TYPE_NORMAL, argument, MM4_RT_R2 | MM4_136_RES);
	Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R2, 0x100);
	if (Retval != NoError) {
		lgpl_printf("line %d: Send XLLP_MMC_CMD9 fail.\n", __LINE__);
		return SDMMCInitializationError;
	}

	//	3/16:	Xiufeng:	Do not touch this function
	//					Since there are some difference in the definition of device CSD registers.
	//					This function need to double check for MMC realted settings.
	MM4_Get_CSD(pSDMMCP);   //KT added; print out CSD

	// Save off some information from the CSD
	//pSDMMCP->pFlashP->BlockSize = pSDMMCP->EraseSize;
	// There is no page size for SDMMC. However save some information so slot type functionality will work
	//pSDMMCP->pFlashP->PageSize = pSDMMCP->ReadBlockSize;

	//send CMD7 to get card into transfer state
	argument = pSDMMCP->CardReg.RCA;
	MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD7, MM4_CMD_TYPE_NORMAL, argument, MM4_48_RES_WITH_BUSY);
	Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1B, 0x100);
	if (Retval != NoError) {
		lgpl_printf("Send XLLP_MMC_CMD7 fail.\n");
		return Retval;
	}

	Retval = mmc4_wait_card_ready(pSDMMCP);
	if (Retval != NoError) {
		lgpl_printf("Send XLLP_MMC_CMD7 timeout.\n");
		return Retval;
	}

	//send CMD13 to check the status of the card
	Retval = MM4_CheckCardStatus(pSDMMCP, 0x900, R1_LOCKEDCARDMASK);		// Make sure card is stdby mode
	if (Retval != NoError) {
		lgpl_printf("line %d: Send MM4_CheckCardStatus fail.\n", __LINE__);
		return SDMMCInitializationError;
	}

	// CMD 16 Set Block Length
	argument = pSDMMCP->ReadBlockSize;
	MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD16, MM4_CMD_TYPE_NORMAL, argument, MM4_48_RES);
	Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1, 0x100);
	if (Retval != NoError) {
		lgpl_printf("line %d: Send XLLP_MMC_CMD16 fail.\n", __LINE__);
		return SDMMCInitializationError;
	}
	// Set the block length for the controller
	pContext->pMMC4Reg->mm4_blk_cntl = argument;

	// Attempt to Increase Bus width
	// 3/16:	the following code is from APSE
	Retval = MM4SetBusWidth(pSDMMCP);
	if (Retval != NoError) {
		lgpl_printf("MM4SetBusWidth fail.\n");
		return SDMMCInitializationError;
	}

	Retval = mmc4_set_hs(pSDMMCP);
	if (Retval != NoError) {
		lgpl_printf("mmc4_set_hs fail.\n");
		return SDMMCInitializationError;
	}

	//send CMD13 to check the status of the card
	Retval = MM4_CheckCardStatus(pSDMMCP, 0x900, R1_LOCKEDCARDMASK);		// Make sure card is transfer mode
	if (Retval != NoError) {
		lgpl_printf("line %d: Send MM4_CheckCardStatus fail.\n", __LINE__);
		return SDMMCInitializationError;
	}

	// Adjust Clock Speed.
	// For EMMC, the controll's PLL is NANDECCCLOCK clock/4. the output clock is SDIO3PLL/(2*rate)
	// if rate = 0, output clock is same with SDIO3PLL.
	// here set the divider to 2 for 100MHz
	MMC4SetBusRate(pContext, SDIO_INTERNAL_DIV);

	//send CMD13 to check the status of the card
	Retval = MM4_CheckCardStatus(pSDMMCP, 0x900, R1_LOCKEDCARDMASK);		// Make sure card is stdby mode
	if (Retval != NoError) {
		lgpl_printf("line %d: Send MM4_CheckCardStatus fail.\n", __LINE__);
		return SDMMCInitializationError;
	}

	// Set up State, Ready for Data transfers
	pSDMMCP->State = READY;


	return NoError;

}

#endif



/******************************************************************************
  Description:
    Set up the registers of the controller to start the transaction to
    communicate to the card for data related command.  The commands are clearly defined in the MMC
    specification.
  Input Parameters:
	   P_SDMMC_Properties_T pSDMMCP - Generic SD/MMC driver properties structure
  Cmd
		Command Index - See MMC or SD specification
  argument
	the argument of  the command. MSW is for ARGH and LSW is for ARGL
  BlockType
		Multiple or Single Block Type
  ResType
		Expected response type
  Output Parameters:
    None
  Returns:
    None
*******************************************************************************/

void MM4_SendDataCommand (P_SDMMC_Properties_T pSDMMCP,
                  UINT_T Cmd,
                  UINT_T  Argument,
                  UINT_T BlockType,
                  UINT_T DataDirection,
                  UINT_T ResType)
{
	 // Assign our context value
	 P_MM4_SDMMC_CONTEXT_T pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;

	 // no need to clear out any fault state that may be left over from a previously failed transaction.
	 // that's because the caller has set State to read or write before calling here.

	 // No Response to the command yet
	 pSDMMCP->CardReponse.CommandComplete = 0;
	 pSDMMCP->Trans.RespType = (ResType >> 8) & 0x000000ff;
	 pSDMMCP->Trans.Cmd = Cmd;	// Fixme: how to know when to set the ACMD flag?

	 printf("\n ready to send cmd %02d arg=0x%08x (resp_type=0x%02X) ",Cmd,Argument,ResType);

	 MMC4SendDataCommand(pContext,
	                  Cmd,
	                  Argument,
	                  BlockType,
	                  DataDirection,
	                  ResType & 0x000000ff);
}

/******************************************************************************
  Description:
    Set up the registers of the controller to start the transaction to
    communicate to the card for data related command.  The commands are clearly defined in the MMC
    specification.
  Input Parameters:
	   P_SDMMC_Properties_T pSDMMCP - Generic SD/MMC driver properties structure
  Cmd
		Command Index - See MMC or SD specification
  argument
	the argument of  the command. MSW is for ARGH and LSW is for ARGL
  BlockType
		Multiple or Single Block Type
  ResType
		Expected response type
  Output Parameters:
    None
  Returns:
    None
*******************************************************************************/

void MM4_SendDataCommandNoAuto12 (P_SDMMC_Properties_T pSDMMCP,
                  UINT_T Cmd,
                  UINT_T  Argument,
                  UINT_T BlockType,
                  UINT_T DataDirection,
                  UINT_T ResType)
{
	 // Assign our context value
	 P_MM4_SDMMC_CONTEXT_T pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;

	 // no need to clear out any fault state that may be left over from a previously failed transaction.
	 // that's because the caller has set State to read or write before calling here.

	 // No Response to the command yet
	 pSDMMCP->CardReponse.CommandComplete = 0;

	 // save the info for use by the isr:
	 pSDMMCP->Trans.RespType = (ResType >> 8) & 0x000000ff;
	 pSDMMCP->Trans.Cmd = Cmd;	// Fixme: how to know when to set the ACMD flag?

	 printf("\n ready to send cmd %02d arg=0x%08x (resp_type=0x%02X) ",Cmd,Argument,ResType);

	 MMC4SendDataCommandNoAuto12(pContext,
	                  Cmd,
	                  Argument,
	                  BlockType,
	                  DataDirection,
	                  ResType & 0x000000ff); // clear out any bits not for the SD_CMD.RES_TYPE field

}


/******************************************************************************
  Description:
    Set up the registers of the controller to start the transaction to
    communicate to the card for setup related commands.
    The commands are clearly defined in the MMC specification.
  Input Parameters:
	   P_SDMMC_Properties_T pSDMMCP - Generic SD/MMC driver properties structure
	Cmd
		Command Index - See MMC or SD specification
    argument
	the argument of  the command. MSW is for ARGH and LSW is for ARGL
	ResType
		Expected response type
  Output Parameters:
    None
  Returns:
    None
*******************************************************************************/

void MM4_SendSetupCommand(P_SDMMC_Properties_T pSDMMCP,
                  UINT_T Cmd,
                  UINT_T CmdType,
                  UINT_T Argument,
                  UINT_T ResType)
{


	 // Assign our context value
	 P_MM4_SDMMC_CONTEXT_T pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;

	 pSDMMCP->State = READY;

	 // No Response to the command yet
	 pSDMMCP->CardReponse.CommandComplete = 0;

	 pSDMMCP->Trans.RespType = (ResType >> 8) & 0x000000ff;

	 printf("\n ready to send cmd %02d arg=0x%08x (resp_type=0x%02X) ",Cmd,Argument,ResType);

	 MMC4SendSetupCommand(pContext,
	                  Cmd,
	                  CmdType,
	                  Argument,
	                  ResType);
}


/****************************************************************
*   MM4_SendStopCommand
*	Issues a stop command for open ended read and write block operations
*
*   Input:
*      P_SDMMC_Properties_T pSDMMCP - pointer to the SDMMC context structure
*   Output:
*      none
*   Returns:
*      none
*****************************************************************/
void MM4_SendStopCommand(P_SDMMC_Properties_T pSDMMCP)
{
	P_MM4_SDMMC_CONTEXT_T pContext;
	// Assign our context value
	pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;

	// Send a CMD 12 to stop transmissions.
	MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD12, MM4_CMD_TYPE_NORMAL, NULL, MM4_48_RES_WITH_BUSY);
	MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1, 0x100);
}

/****************************************************************
*   MM4_ISR
*	Interrupt Service Routine for SDMMC controller
*		Controls flow and catches faults asynchronously
*   Input:
*	   P_SDMMC_Properties_T pSDMMCP
*   Output:
*      none
*   Returns:
*      none
*****************************************************************/

void MM4_ISR(unsigned char uchCardId)
{
	UINT_T i;
	VUINT_T *pControllerBuffer;
	UINT_T i_stat_copy;
	UINT_T i_err_stat;				// Keep a copy of i stat register
	UINT_T i_acmd12_err_stat;
	P_MM4_I_STAT  p_i_stat_copy;			// Pointer to the copy.
	P_MM4_I_STAT_UNION pMM4_I_STAT_U;
//	printf( "\n Interrupt is received from SD %d card", uchCardId);

	P_SDMMC_Properties_T pSDMMCP = &SDMMC_Prop ;
	P_MM4_SDMMC_CONTEXT_T pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;		// Assign our context value
	P_MM4_CMD_XFRMD_UNION pCmdXfer= (P_MM4_CMD_XFRMD_UNION)&pContext->pMMC4Reg->mm4_cmd_xfrmd;
	P_MM4_STATE_UNION pState = (P_MM4_STATE_UNION)&pContext->pMMC4Reg->mm4_state;
	p_i_stat_copy = (P_MM4_I_STAT)&i_stat_copy;


	// Mask all interrupts KT added
	//MMC4EnableDisableIntSources(pSDMMCP->pContext, DISABLE_INTS);

	// Save off the interrupt source to the copy
	pMM4_I_STAT_U = (P_MM4_I_STAT_UNION) &pContext->pMMC4Reg->mm4_i_stat;
	i_stat_copy = pMM4_I_STAT_U->mm4_i_stat_value;

	printf("\n enter MM4_ISR i_stat_copy = 0x%x \n", i_stat_copy);

	// Check for any error
	if (p_i_stat_copy->errint)
	{
	    i_err_stat = i_stat_copy >> 16 ;
		// TBD pSDMMCP->CardReponse.CommandError = 1;
		// Reset MM4CMD pin in case it's hung waiting on a response
		//pContext->pMMC4Reg->mm4_cntl2 |= 0x02000000;
		MMC4CMDSWReset(pContext);	// this cleas the command inhibit flag in sd_present_state_1.
		MMC4DataSWReset(pContext);	// this clears the data inhibit flag and stops mclk

		if ((pSDMMCP->State == WRITE) || (pSDMMCP->State == READ) || (pSDMMCP->State == DATATRAN))
		{
		    printf(PRN_ERR,"\n errint:i_stat=0x%08x\n",i_stat_copy);
			pSDMMCP->State = FAULT;

			pContext->pMMC4Reg->mm4_i_stat &= 0x0000FFFF;	//Satya
			pSDMMCP->State = READY;		//Satya
		}

		if( i_err_stat & SD_ERROR_INT_STATUS_AUTO_CMD12_ERR )
		{
			i_acmd12_err_stat = pContext->pMMC4Reg->mm4_acmd12_er;
			printf(PRN_ERR,"\n acmd12 errin stat= 0x%08x \n",pContext->pMMC4Reg->mm4_acmd12_er);
			// clear the acmd12 error bits.
			pContext->pMMC4Reg->mm4_acmd12_er = i_acmd12_err_stat;
		}

	}

	// is this an sdma interrupt event?
	if(pContext->pMMC4Reg->mm4_i_stat & (1u<<3))	// bit 3 is dmaint
	{
		// the transfer halted because the boundary specified in ... was reached.
		// rewriting the sysaddr with the next address allows the transfer to resume.
		// fortunately the sysaddr register itself contains the next address.
		// so, just re-write the sysaddr register with its own current contents.
		pContext->pMMC4Reg->mm4_sysaddr = pContext->pMMC4Reg->mm4_sysaddr;	// sysaddr points to next addr to write.

		pContext->pMMC4Reg->mm4_i_stat = (1u<<3);	// bit 3 is dmaint
		i = pContext->pMMC4Reg->mm4_i_stat;	// read-back to ensure write completes

	}

	// Has the Command completed? If so read the response register
	if (p_i_stat_copy->cmdcomp)
	{
		// Indicate that the response has been read
		// TBD pSDMMCP->CardReponse.CommandError = 0;
		pSDMMCP->CardReponse.CommandComplete = 1;

		pControllerBuffer = (VUINT_T *) &pContext->pMMC4Reg->mm4_resp0;
		for (i = 0; i < 4; i++)
			pSDMMCP->CardReponse.pBuffer[i] = pControllerBuffer[i];
	}
	//printf(" ISR: i_stat = 0x%08x \n",i_stat_copy);

	// Are we SDMA mode enabled?
	if (pSDMMCP->SDMA_Mode)
	{
		if (p_i_stat_copy->xfrcomp){

			printf("MM4 ISR: transfer complete, blk_cntl = 0x%x\n", pContext->pMMC4Reg->mm4_blk_cntl);
			pSDMMCP->State = READY;
		}
	}


	// Clear the interrupts
	pMM4_I_STAT_U->mm4_i_stat_value = i_stat_copy;	// Clear the interrupt source.
	i = pMM4_I_STAT_U->mm4_i_stat_value;			// Must make a dummy read to allow interrupts to clear.

#if 1

	if (pSDMMCP->SDMA_Mode == FALSE)
	{
		// Handle State based interrupts XFRCOMP, BUFRDRDY, BUFWRRDY
		switch (pSDMMCP->State)
		{
			case WRITE:
			{
				if (p_i_stat_copy->bufwrrdy){
					printf("MM4 ISR: write fifo, blk_cntl = 0x%x\n", pContext->pMMC4Reg->mm4_blk_cntl);
					MM4_WriteFifo(pSDMMCP);
				}

				// Are we done sending all of data?
				if (pSDMMCP->Trans.TransWordSize == pSDMMCP->Trans.WordIndex)
					pSDMMCP->State = DATATRAN;

				break;
			}
			case READ:
			{
				if (p_i_stat_copy->bufrdrdy){
					printf("MM4 ISR: read fifo, blk_cntl = 0x%x\n", pContext->pMMC4Reg->mm4_blk_cntl);
					MM4_ReadFifo(pSDMMCP);
				}

				// Are we done sending all of data?
				if (pSDMMCP->Trans.TransWordSize == pSDMMCP->Trans.WordIndex)
					pSDMMCP->State = DATATRAN;

				break;
			}
			case DATATRAN:
			{
				// Wait for Transfer Complete Signal
				printf("MM4 ISR: transfer complete, blk_cntl = 0x%x\n", pContext->pMMC4Reg->mm4_blk_cntl);
				//MM4_SendStopCommand(pSDMMCP);
				if (p_i_stat_copy->xfrcomp)
					pSDMMCP->State = READY;

				break;
			}
			default:
				break;
		}
	}


	printf("MM4 ISR end, mm4_i_stat_value = 0x%x  \n",pMM4_I_STAT_U->mm4_i_stat_value);
#endif

	// un-Mask all interrupts KT added
	//MMC4EnableDisableIntSources(pSDMMCP->pContext, ENABLE_INTS);
	return;
}


#if 1
int SdioStart(P_SDMMC_Properties_T pSDMMCP)
{
	return MM4_CardInit(pSDMMCP);
}

#endif


/***************************************************************
*   MM4SwitchPartitionForAlternateBootMode
*		eMMC Alternate Boot Mode Setup
*   Input:
*		PartitionNumber - Contains the partition Number to switch to and enable bits for the boot partitions.
*   Output:
*   Returns: NoError, ReadError or NotSupportedError
*
*****************************************************************/

UINT_T MM4SwitchPartitionForAlternateBootMode(UINT_T partition)
{
	 UINT_T argument, Cmd;
	 UINT_T Retval = NoError;
	 UINT_T flags = NO_FLAGS;
	 P_MM4_SDMMC_CONTEXT_T pContext;
	 MMC_CMD6_OVERLAY Cmd6;
	 PARTITION_CONFIG_EXT_CSD MMCPartConfig;
	 P_MM4_SD_CE_ATA_1_2 pMM4_SD_CE_ATA_1_2;
	 // Assign our context value
	 P_SDMMC_Properties_T pSDMMCP = (P_SDMMC_Properties_T)(&SDMMC_Prop) ;
	 pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;

	// Assign some context
	pMM4_SD_CE_ATA_1_2 = (P_MM4_SD_CE_ATA_1_2)&pContext->pMMC4Reg->mm4_sd_ce_ata_1_2;

	// Send CMD0 (GO_IDLE_STATE) to get card into idle state
	pMM4_SD_CE_ATA_1_2->mmc_card = 1 ;
//	printf( "\n send CMD0 (GO_IDLE_STATE) to get card into idle state\n");
//	MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD0, MM4_CMD_TYPE_NORMAL, argument, MM4_RT_NONE | MM4_NO_RES);
//	Retval += MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_NONE, 0x10);
//	printf( "\n CMD0 (GO_IDLE_STATE) done\n");


	 // Issue CMD 6 write byte 179 (0xB3)
	 Cmd6.MMC_CMD6_Layout.Access = EXT_CSD_ACCESS_WRITE_BYTE;				// Write byte
	 Cmd6.MMC_CMD6_Layout.CmdSet = 0;				// Don't Care
	 Cmd6.MMC_CMD6_Layout.Index = PARTITION_CONFIG_MMC_EXT_CSD_OFFSET;       // Choose Boot Config
	 Cmd6.MMC_CMD6_Layout.Reserved0 = 0;
	 Cmd6.MMC_CMD6_Layout.Reserved1 = 0;
	 //Cmd6.MMC_CMD6_Layout.Value = 0x49;	 // 0x49					// Boot from Partition 1, no send boot acknowledge
	 Cmd6.MMC_CMD6_Layout.Value = 0 ;
	 Cmd6.MMC_CMD6_Layout.Value |= 0x40 ;					// send boot acknowledge
	 Cmd6.MMC_CMD6_Layout.Value |= (partition << 3) ;		// set boot partition
	 Cmd6.MMC_CMD6_Layout.Value |= (partition) ;			// set access permission to boot partition.
	 MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD6, MM4_CMD_TYPE_NORMAL, Cmd6.MMC_CMD6_Bits, MM4_48_RES_WITH_BUSY);
	 Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1B, 0x100);

	 //Retval = MM4_CheckCardStatus(pSDMMCP, 0x0900, R1_LOCKEDCARDMASK);


	 // Issue CMD 6 to write byte 177 (0xB1)
	 Cmd6.MMC_CMD6_Layout.Access = EXT_CSD_ACCESS_WRITE_BYTE;				// Clear bits
	 Cmd6.MMC_CMD6_Layout.CmdSet = 0;						// Don't Care
	 Cmd6.MMC_CMD6_Layout.Index = BOOT_BUS_WIDTH_MMC_EXT_CSD_OFFSET;       // Choose Boot Config
	 Cmd6.MMC_CMD6_Layout.Reserved0 = 0;
	 Cmd6.MMC_CMD6_Layout.Reserved1 = 0;
	 //	Set to 8 bit mode
	 Cmd6.MMC_CMD6_Layout.Value = 0x2 ; //0x2; //0xA; //  Use 2 for Samsung 4.3+ parts, they don't allow setting fast timing bits 0x2;
	 MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD6, MM4_CMD_TYPE_NORMAL, Cmd6.MMC_CMD6_Bits, MM4_48_RES_WITH_BUSY);
	 Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1B, 0x100);

	return NoError;
}
/***********************************************************
*    MM4_MMCAlternateBootModeReadBlocks()
*       Reads the given block off of the SD/MMC card and
*       into LocalAddr or empty buffer
*    input:
*		none
*    output:
*       LocalAddr will contain the contents of the block
*    returns:
*       none
************************************************************/
UINT_T MM4_MMCAlternateBootModeReadBlocks(P_SDMMC_Properties_T pSDMMCP)
{
	UINT_T flags, argument;
	UINT_T Retval = NoError;
	P_MM4_SDMMC_CONTEXT_T pContext;
	P_MM4_BLK_CNTL pMM4_BLK_CNTL;
	UINT_T i ;

	printf( "\n MM4_MMCAlternateBootModeReadBlocks enter \n");

	// Assign our context value
	pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;
	// Must set MMC NUMBLK
	pMM4_BLK_CNTL = (P_MM4_BLK_CNTL) &pContext->pMMC4Reg->mm4_blk_cntl;
	pMM4_BLK_CNTL->blk_cnt = 128;

	// Set up State
	pSDMMCP->State = READ;

	// Are we SDMA mode enabled?
	if (pSDMMCP->SDMA_Mode)
	{
		pContext->pMMC4Reg->mm4_blk_cntl |= MM4_512_HOST_DMA_BDRY << 12;	// Set SDMA buffer size.
		pContext->pMMC4Reg->mm4_sysaddr = SDIO_BOOT_BUF;
	}
	// Do a CMD 0 to start Alternate Boot Mode and read
	argument = MMC_ALTERNATE_BOOT_ARGUMENT;
	MM4_SendDataCommandNoAuto12(pSDMMCP, XLLP_MMC_CMD0, argument, MM4_MULTI_BLOCK_TRAN, MM4_CARD_TO_HOST_DATA, MM4_RT_NONE | MM4_NO_RES);
	//MM4_SendDataCommand(pSDMMCP, XLLP_MMC_CMD0, argument, MM4_MULTI_BLOCK_TRAN, MM4_CARD_TO_HOST_DATA, MM4_RT_NONE | MM4_NO_RES);

	while( (pSDMMCP->State != FAULT) && (pSDMMCP->State != READY) ){
		;
	}

	// Get the Card Response
	if (Retval != NoError)
		return Retval;

	// Issue CMD 0 again to stop sending data and terminate alternate boot mode.
	MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD0, MM4_CMD_TYPE_NORMAL, 0x0, MM4_NO_RES);
	Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_NONE, 0x10);

#if 0
	for(i=0; i<512*4; i++){
		printf( "0x%02x\t", *((unsigned char *)(SDIO_BOOT_BUF+i)));
		if(i%8 == 7)
			printf( "\n") ;
	}
#endif
	return Retval;
}



/**********************************************************
*   MM4_MMCAlternateBootMode
*      This mode uses Alternate Boot Method mode for eMMC Partition 1 to stream in
*		It assumes 8 bit mode Operation
*		High Speed Operations
*		The address where images are streamed are defined in ISRAM_IMAGE_LOAD_BASE
*		128 KB only will be loaded.
*		When this function is called.
*   Input:
*	   none
*   Output:
*      none
*   Returns:
*      WTP recoginized Success/Fail return code
***********************************************************/
UINT_T MM4_MMCAlternateBootTest(P_SDMMC_Properties_T pSDMMCP)
{
	UINT_T argument, cardStatus, resp, cmd55_resp;
	UINT_T attempts, controllervoltage;
	P_MM4_SDMMC_CONTEXT_T pContext;
	P_MM4_CNTL1 pMM4_CNTL1;
	P_MM4_SD_CFG_FIFO_PARAM pMM4_SD_CFG_FIFO_PARAM;
	P_MM4_SD_FIFO_PARAM_UNION pMM4_SD_FIFO_PARAM ;
	P_MM4_SD_CE_ATA_1_2 pMM4_SD_CE_ATA_1_2;
	UINT_T startTime, endTime;
	UINT_T retval = NoError;
	P_MM4_BLK_CNTL pMM4_BLK_CNTL;
	volatile int state ;
	UINT_T iResult ;
	int i ;


	printf( "\n MM4_MMCAlternateBootTest entered\n");

	pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;
	pMM4_CNTL1 = (P_MM4_CNTL1) &pContext->pMMC4Reg->mm4_cntl1;
	pMM4_SD_CFG_FIFO_PARAM = (P_MM4_SD_CFG_FIFO_PARAM)&pContext->pMMC4Reg->mm4_sd_cfg_fifo_param;
	pMM4_SD_FIFO_PARAM = (P_MM4_SD_FIFO_PARAM_UNION)&pContext->pMMC4Reg->mm4_sd_fifo_param;
	pMM4_SD_CE_ATA_1_2 = (P_MM4_SD_CE_ATA_1_2)&pContext->pMMC4Reg->mm4_sd_ce_ata_1_2;
	pContext->pMMC4Reg->mm4_blk_cntl = 0 ;
	pMM4_SD_CE_ATA_1_2->mmc_card = 1;
#if 1

#if SDIO_DMA
	pSDMMCP->SDMA_Mode = TRUE;
#else
	pSDMMCP->SDMA_Mode = FALSE;
#endif

	pMM4_SD_FIFO_PARAM->MM4_SD_FIFO_PARAM_bits.ovrrd_clk_oen = 1 ;
	pMM4_SD_FIFO_PARAM->MM4_SD_FIFO_PARAM_bits.force_clk_on = 1 ;

	// Save off read Block Size, play it safe, for now hard code to 512 Bytes
	pSDMMCP->ReadBlockSize = HARD512BLOCKLENGTH;
	pSDMMCP->WriteBlockSize = HARD512BLOCKLENGTH;
	pSDMMCP->EraseSize = HARD512BLOCKLENGTH;

	// Set the block length for the controller
	pContext->pMMC4Reg->mm4_blk_cntl = pSDMMCP->ReadBlockSize;

	// Switch to x8 mode.
	pMM4_CNTL1->ex_data_width = 1;		// Move to 8-bit mode.
//	pMM4_CNTL1->hispeed = 1 ;		// enabe high speed mode
	pMM4_SD_CE_ATA_1_2->mmc_card = 1;

	// Enable BOOT ACK through the controller.
	pMM4_SD_CFG_FIFO_PARAM->boot_ack = 1;

	//	issue CMD0 to go to pre-idle state
	argument = MMC_CMD0_PRE_IDLE_ARGUMENT ;
	MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD0, MM4_CMD_TYPE_NORMAL, argument, MM4_RT_NONE| MM4_NO_RES);
	retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_NONE, 0x10);	 // No Response to this cmd.


	// Enable capturing status for sending out 74 clock cycles.

	pMM4_SD_CE_ATA_1_2->misc_int_en = 1;
	// Enable and start 74 clock cycles
	pMM4_SD_CFG_FIFO_PARAM->gen_pad_clk_cnt = 74;		// 74 clock cycles.
	pMM4_SD_CFG_FIFO_PARAM->gen_pad_clk_on = 1;			// Start generating the clocks.

	do
	{
		if (pMM4_SD_CE_ATA_1_2->misc_int)
			break;		  // 74 clock cycles have been generated.
	}
	while(1);

	for(i=0 ; i<0x400; i++){
		state=i;
	}

	// Set the State to READY
	pSDMMCP->State = READY;

	// Set up State
	// pSDMMCP->State = READ;
	retval = MM4_MMCAlternateBootModeReadBlocks(pSDMMCP) ;
	if(retval != NoError){
		printf( "\n MM4_MMCAlternateBootModeReadBlocks error, retval = 0x%x\n", retval);
		return retval ;
	}else
		printf( "\n MM4_MMCAlternateBootModeReadBlocks done\n") ;


	// Issue go pre-idle Add this back for V4.4 support.
	// MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD0, MM4_CMD_TYPE_NORMAL, MMC_CMD0_PRE_IDLE_ARGUMENT, MM4_NO_RES);
	//	retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_NONE, 0x10);	 // No Response to this cmd.
#endif		//endif of #if 0

	return retval;
}




/***************************************************************
*   MM4SwitchPartition
*		If the Card supports partitioning (eSD) this routine will switch to the appropriate
*		partition by using extended partition command set CMD37.
*   Input:
*		PartitionNumber - Contains the partition Number to switch to and enable bits for the boot partitions.
*   Output:
*   Returns: NoError, ReadError or NotSupportedError
*
*****************************************************************/
UINT_T MM4SwitchPartition(P_SDMMC_Properties_T pSDMMCP, UINT_T PartitionNumber)
{
	 UINT_T argument, Cmd;
	 UINT_T Retval = NoError;
	 UINT_T flags = NO_FLAGS;
	 P_MM4_SDMMC_CONTEXT_T pContext;
	 MMC_CMD6_OVERLAY Cmd6;
	 PARTITION_CONFIG_EXT_CSD MMCPartConfig;
	 int i;
	 volatile int delay,ii;

	// Assign our context value
	pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;

	printf("MM4SwitchPartition: switch to part %d\n:", PartitionNumber);

	//send CMD13 to check the status of the card
	Retval = MM4_CheckCardStatus(pSDMMCP, 0x900, R1_LOCKEDCARDMASK);		// Make sure card is transfer mode
	if (Retval != NoError)
		return SDMMCInitializationError;

	{
	    // Issue CMD 6 to clear PARTITION_ACCESS bits in EXT_CSD register byte 179
		Cmd6.MMC_CMD6_Layout.Access = EXT_CSD_ACCESS_CLEAR_BITS;				// Clear bits
	    Cmd6.MMC_CMD6_Layout.CmdSet = 0;						// Don't Care
	    Cmd6.MMC_CMD6_Layout.Index = PARTITION_CONFIG_MMC_EXT_CSD_OFFSET;       // Choose Boot Config
	    Cmd6.MMC_CMD6_Layout.Reserved0 = 0;
	    Cmd6.MMC_CMD6_Layout.Reserved1 = 0;
	    Cmd6.MMC_CMD6_Layout.Value = PARTITION_ACCESS_BITS;						// Clear out Partition Access bits

		for (i = 0; i < 10; i++) {
		    MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD6, MM4_CMD_TYPE_NORMAL, Cmd6.MMC_CMD6_Bits, MM4_RT_R1 | MM4_RT_BUSY | MM4_48_RES_WITH_BUSY);
			Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1B, 0x10);
			if (Retval == NoError)
				break;
			for(delay=0; delay<=0x10000; delay++)
				ii=delay;
		}
		// Now issue CMD 6 again to set the right bits.
		Cmd6.MMC_CMD6_Layout.Access = EXT_CSD_ACCESS_SET_BITS;				// Clear bits
	    Cmd6.MMC_CMD6_Layout.CmdSet = 0;	// Don't Care
	    Cmd6.MMC_CMD6_Layout.Index = PARTITION_CONFIG_MMC_EXT_CSD_OFFSET;   // Choose Boot Config
	    Cmd6.MMC_CMD6_Layout.Reserved0 = 0;
	    Cmd6.MMC_CMD6_Layout.Reserved1 = 0;

#define HYNIX 0x90

		if (m_cid.manufactureID == HYNIX) {
			if(PartitionNumber == 2){										//enable boot partition 2 for boot
				Cmd6.MMC_CMD6_Layout.Value = (2 << 3);
			}else
				Cmd6.MMC_CMD6_Layout.Value = (1 << 3);                      //enable boot partition 1 for boot avoid EXT_CSD[179] was distorted
		    Cmd6.MMC_CMD6_Layout.Value |= PartitionNumber | (1 << 6);	    // Set the correct partition and enable Boot acknowledge sent during boot operation.
		}else {
			Cmd6.MMC_CMD6_Layout.Value = PartitionNumber;
		}

		for (i = 0; i < 10; i++) {
		    MM4_SendSetupCommand(pSDMMCP, XLLP_MMC_CMD6, MM4_CMD_TYPE_NORMAL, Cmd6.MMC_CMD6_Bits, MM4_RT_R1 | MM4_RT_BUSY | MM4_48_RES_WITH_BUSY);
			Retval = MM4_Interpret_Response(pSDMMCP, MMC_RESPONSE_R1B, 0x10);
			//lgpl_printf("index = %d\n", i);
			if (Retval == NoError)
				break;
		}
	 }

	Retval |= mmc4_wait_card_ready(pSDMMCP);
	Retval |= MM4_CheckCardStatus(pSDMMCP, 0x900, R1_LOCKEDCARDMASK);
	if (Retval != NoError)
	{
		lgpl_printf("Switch Partition Error. Retval = 0x%x.\n", Retval);
		pSDMMCP->State = READY;
		return SDMMC_SWITCH_ERROR;
	}
	 return NoError;
}



//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
//-------------------------------------------------------------------------------
void InstallHighHandler(unsigned int handlerAddr, unsigned int vector)
{
	//load PC with content at current PC + 0x38,
	//since current PC is vector+0x8, the handler should be at vector+0x40
	BFM_HOST_Bus_Write32(vector, 0xE59FF038);
	BFM_HOST_Bus_Write32(vector+0x40, handlerAddr);
}

void EnableIRQ(void)
{
//  int my_cpsr ;

    __asm__ __volatile__(
            "MRS            r0, CPSR"   "\n\t"      //get CRPR
            "BIC            r0, r0, #0x80"  "\n\t"      // disable IRQ and FIQ
            "MSR            CPSR, r0"  "\n\t"
            :
            :
            :"r0"
        );
}

UNSG32 getMPid(void)
{
	__asm__ __volatile__(
		"MRC		p15,0,r0,c0,c0,5"	"\n\t"
		"ANDS		r0, r0, #0xf"		"\n\t"
		:
		:
		:"r0"
	) ;
}

static void mmc4_init_dll(P_MM4_SDMMC_CONTEXT_T pSDCXT)
{
	int i;
	unsigned int tx_delay, val;
	TGbl_sdioDllMstStatus	sdio_dll_Mst_st;
	TGbl_sdioDllMstRef	sdio_dll_Mst_ref;
	TGbl_sdioDllMstCtrl sdio_dll_Mst_ctrl;
	MM4_TX_CFG_UNION	mm4_tx_cfg;

	//	Program the sdio3DllMstRefClk to be 100 MHz
	//	(MEMMAP_CHIP_REG_BASE + FA_Gbl_sdioDllMstCtrl (0x250))

	//	by default, ref clock is came from SYSPLL, so set the divider to 4;
	BFM_HOST_Bus_Read32(GLOBAL_SDIO_DLLMST_REF, &sdio_dll_Mst_ref.u32[0]);
	sdio_dll_Mst_ref.usdioDllMstRef_ClkSel = 4;
	BFM_HOST_Bus_Write32(GLOBAL_SDIO_DLLMST_REF, sdio_dll_Mst_ref.u32[0]);

	//	Program the DLL Master Reset ( bit 22)	using the Global register
	//	using the following register addr
	//	(MEMMAP_CHIP_REG_BASE + FA_Gbl_sdioDllMstCtrl (0x254) ) to a
	//	value of 1 ( active high reset )
	BFM_HOST_Bus_Read32(GLOBAL_SDIO_DLLMST_CTRL, &sdio_dll_Mst_ctrl.u32[0]);
	sdio_dll_Mst_ctrl.usdioDllMstCtrl_RESET = 1 ;
	BFM_HOST_Bus_Write32(GLOBAL_SDIO_DLLMST_CTRL, sdio_dll_Mst_ctrl.u32[0]);

	//	Wait for atleast 10ns. Better to wait for 5 clks of
	//	sdio3MstRefClk to be conservative
	//	CPU CLK is 400MHz, one clcyes is 2.5ns, so 10ns = 4 cycles
	//	sdio3MstRefClk = 100Mhz, one clk = 10ns, 5 clks = 50ns = 25 cpu cycles
	for(i=0; i<16; i++)
		BFM_HOST_Bus_Write32(LOG_REG, i);

	//	rogram the PH_SEL4 to a value 0x1f
	//	((MEMMAP_CHIP_REG_BASE + FA_Gbl_sdioDllMstCtrl (0x254)
	//	bits [19:15] of the register
	BFM_HOST_Bus_Read32(GLOBAL_SDIO_DLLMST_CTRL, &sdio_dll_Mst_ctrl.u32[0]);
	sdio_dll_Mst_ctrl.usdioDllMstCtrl_PH_SEL4 = 0x1F ;
	BFM_HOST_Bus_Write32(GLOBAL_SDIO_DLLMST_CTRL, sdio_dll_Mst_ctrl.u32[0]);

	//	Program the DLL Master Reset ( bit 22)	using the Global
	//	register using the following register addr
	//	(MEMMAP_CHIP_REG_BASE + FA_Gbl_sdioDllMstCtrl (0x254) )
	//	to a value of 0 ( deassert reset )
	BFM_HOST_Bus_Read32(GLOBAL_SDIO_DLLMST_CTRL, &sdio_dll_Mst_ctrl.u32[0]);
	sdio_dll_Mst_ctrl.usdioDllMstCtrl_RESET = 0 ;
	BFM_HOST_Bus_Write32(GLOBAL_SDIO_DLLMST_CTRL, sdio_dll_Mst_ctrl.u32[0]);

	//	Wait for atleast 10ns. Better to wait for 5 clks of
	//	sdio3MstRefClk (100MHz) to be conservative
	//	Wait for 4096 clocks of sdio3MstRefClk
	//	( 100MHz as programmed in step1.) after step 6.
	//	Just to be conservative, wait extra 10 clocks(100MHz Clock).
	//	( this is for DLL lock )
#if 0
	p_sdio_dll_Mst_st = (TGbl_sdioDllMstStatus *)(GLOBAL_SDIO_DLLMST_STATUS);
	do
	{
		if (p_sdio_dll_Mst_st->usdioDllMstStatus_DLL_LOCK)
			break;
	}
	while(1);
#else
	//	for FPGA, use SW delay to wait for DLL LOCK
	//	4096 clocks of sdio3MstRefClk = 4096 * 4 of cpu cycles
	//	11 cycles every loop	11*0x800 = 5632x4
	for(i=1; i<0x800; i++)
		BFM_HOST_Bus_Write32(LOG_REG, i);

#endif
	val=0;
	while(val != 1)
	{
		BFM_HOST_Bus_Read32(GLOBAL_SDIO_DLLMST_STATUS, &sdio_dll_Mst_st.u32[0]);
		val = sdio_dll_Mst_st.usdioDllMstStatus_DLL_LOCK;
	}
	//	Read the status using the sdioDllMstStatus register.
	//	This is the delay value that needs to be programmed into
	//	the delay element of MMC controller
	//	Read MEMMAP_CHIP_REG_BASE + RA_Gbl_sdioDllMstStatus (0x25c)
	//	and use the read data to program for delay element. As we programmed PH_SEL4,
	//	the status has to be read for DELAY_CTRL4 ( bits 26:18]
	BFM_HOST_Bus_Read32(GLOBAL_SDIO_DLLMST_STATUS, &sdio_dll_Mst_st.u32[0]);
	tx_delay = sdio_dll_Mst_st.usdioDllMstStatus_DELAY_CTRL4 ;

	//	Write the tx_delay to reg log
	//	Program the 9-bit  delay value in the MMC Device
	//	DLL Register in the MMC controller.
	//	this will make sure the hold time is more than 3ns for 25MHz clock

	//	the read_data is less than value of 0xab ( value obtained in simulation ),
	//	program a value of 0xab in the registe

	// tx_delay = 0xab ;
	// printf("tx_delay = 0x%x.\n", tx_delay);
	BFM_HOST_Bus_Read32( &pSDCXT->pMMC4Reg->mm4_tx_cfg, &mm4_tx_cfg.MM4_TX_CFG_value);
	mm4_tx_cfg.MM4_TX_CFG_bits.tx_hold_delay0 = tx_delay ;
	BFM_HOST_Bus_Write32(&pSDCXT->pMMC4Reg->mm4_tx_cfg, mm4_tx_cfg.MM4_TX_CFG_value);
	BFM_HOST_Bus_Write32(&pSDCXT->pMMC4Reg->mm4_rx_cfg, 0x0);

}


static void mmc4_init_irq(void)
{
	//InstallHighHandler((unsigned int)DAbort_Handler, MEMMAP_HIGH_EXCEPT_DATA_REG);

	printf("%s, InstallHighHandler\n", __FUNCTION__) ;
	//	Init IRQ handler
	InstallHighHandler((unsigned int)IRQ_Handler, MEMMAP_HIGH_EXCEPT_IRQ_REG);
	//GIC distribution interface is only needed to be initialized once
	//but the CPU interface needs to be initalized by both CPU cores
	//CPU interface registers are banked (same address)
	//So we will have both first and second CPU to call the same funtion

	printf("%s, initMPGIC\n", __FUNCTION__) ;
	initMPGIC();

	printf("%s, EnableIRQ\n", __FUNCTION__) ;
	// enable IRQ
	EnableIRQ();

	//Enable_IRQ_PIC(getMPid(), IRQ_emmc_int);
	diag_GICSetInt(getMPid(), MP_BERLIN_INTR_ID(IRQ_emmc_int), INT_ENABLE);
}
static int emmc_hardware_reset(void)
{
	int i;
	unsigned short reg_value;
	//
	//set MMC Resetn to 0
	BFM_HOST_Bus_Read16(SD_CE_ATA_2_REG, &reg_value);
	reg_value &= ~(1 << 11);
	BFM_HOST_Bus_Write16(SD_CE_ATA_2_REG, reg_value);
	//need at least 1us
	//11 cycles every loop
	// 1 cycle = 1ns for CPU 1200Hz
	//100*11=1100 ns
	for(i=0; i<100; i++)
		BFM_HOST_Bus_Write32(LOG_REG, i);
	//set MMC Resetn to 1
	BFM_HOST_Bus_Read16(SD_CE_ATA_2_REG, &reg_value);
	reg_value |= (1 << 11);
	BFM_HOST_Bus_Write16(SD_CE_ATA_2_REG, reg_value);
	//200us
	//set MMC Resetn to 0
	//BFM_HOST_Bus_Read16(SD_CE_ATA_2_REG, &reg_value);
	//reg_value |= (1 << 11);
	//BFM_HOST_Bus_Write16(SD_CE_ATA_2_REG, reg_value);
}

int do_emmcinit(void)
{
	UINT_T argument, cardStatus, resp, cmd55_resp;
	UINT_T attempts, controllervoltage;

	P_MM4_SDMMC_CONTEXT_T pSDCXT ;
	P_MM4_CNTL1 pMM4_CNTL1;
	UINT_T iResult ;
	UINT_T retval = NoError;
	unsigned int i ;
	unsigned int * IM2_buffer = NULL ;
	unsigned int * IM2_Temp_Buff = NULL ;

	P_SDMMC_Properties_T pSDMMCP = (P_SDMMC_Properties_T)(&SDMMC_Prop) ;
	pSDMMCP->pContext = &MM4_Context ;

	pSDCXT = (P_MM4_SDMMC_CONTEXT_T)pSDMMCP->pContext ;
	pSDCXT->pMMC4Reg = (P_MM4_SDMMC_T)SDIO3_SLOT3_REG_BASE ;

	//	Init SDMMC_Prop
	SDMMC_Prop.ControllerType=0;	// See CONTROLLER_TYPE platformconfig.h
	SDMMC_Prop.State=UNINITIALIZED;				// Indicate State of the card

	emmc_hardware_reset();
	mmc4_init_irq();

	mmc4_init_dll(pSDCXT);

	printf("%s, MMC4FullSWReset\n", __FUNCTION__) ;
	MMC4FullSWReset(pSDCXT);

	printf("%s, MMC4StartInternalBusClock\n", __FUNCTION__) ;
	// Start and Wait for primary internal clock to start.
	MMC4StartInternalBusClock(pSDCXT);

	// eMMC clock share the same source with NANDECC clock, after reset, the default
	// clock is 200MHz, here we set divider to 8 to make eMMC output clock to 25MHz.
	//MMC4SetBusRate(pSDCXT, SDCLK_SEL_DIV_16);
	//MMC4SetBusRate(pSDCXT, SDCLK_SEL_DIV_8);
	// MMC4SetBusRate(pSDCXT, SDIO_INTERNAL_DIV);	// for 100MHz, set output 25MHz

	// Set Read Response Timeout
	MMC4SetDataTimeout(pSDCXT, CLOCK_27_MULT);


	printf("%s, MM4_SetControllerVoltage\n", __FUNCTION__) ;
	// Enable power, after this , the clock output will be enabled.
	MM4_SetControllerVoltage(pSDCXT);


	printf("%s, MMC4EnableDisableIntSources\n", __FUNCTION__) ;
	// Unmask and Enable interrupts
	MMC4EnableDisableIntSources(pSDCXT, ENABLE_INTS);

	// To identify
	//      100M/512 = 200K
	MMC4SetBusRate(pSDCXT, SDCLK_SEL_DIV_512);
	//	for FPGA platform, this is 12MHz/32 < 400KHz
	//	MMC4SetBusRate(pSDCXT, SDCLK_SEL_DIV_32);

	iResult = SdioStart(pSDMMCP) ;
	if(iResult != NoError){
		lgpl_printf("\n eMMC indentify error.\n");
		return 0x800276 ;
	}
	printf("%s, Sdio Indentify done\n", __FUNCTION__) ;
	emmc_inited = 1 ;

	return 0;
#if 0

	printf("%s, begin to write data to emmc device\n", __FUNCTION__) ;

	IM2_buffer = (unsigned int *)0x200000 ;
	for(i=0; i<0x100000; i++)
		*(IM2_buffer + i) = i ;
	iResult = SDIOWrite(pSDMMCP, 0 , 64, IM2_buffer) ;
	if(NoError != iResult){
		printf("\n SDIO write error\n") ;
	}

	printf("%s,  write data done\n") ;

	IM2_Temp_Buff = (unsigned int *)0x800000 ;
	for(i=0; i<0x100000; i++)
		*(IM2_Temp_Buff+i) = i+1 ;
	printf("%s, read data from eMMC device\n", __FUNCTION__)  ;
	iResult = SDIORead(pSDMMCP, 0, 64, IM2_Temp_Buff) ;
	if(NoError != iResult){
		printf("\n SDIO read error\n") ;
		return -1 ;
	}

	printf(" compare read and write buffer\n") ;
	for(i=0; i<0x100000; i++)
	{
		if( *(IM2_Temp_Buff+i) != *(IM2_buffer+i) ){
			printf("verify error, i=0x%x, data is 0x%x and 0x%x\n",
					i, *(IM2_buffer+i), *(IM2_Temp_Buff+i)) ;
			break ;
		}
	}

	for(i=0; i<10; i++)
		printf("Read data %x\n", *(IM2_Temp_Buff+i) ) ;
	printf(" verify passed\n") ;

#endif
}

int do_emmc_switch_part(UINT_T PartitionNumber)
{
	int ret = 0;
	P_SDMMC_Properties_T pSDMMCP = (P_SDMMC_Properties_T)(&SDMMC_Prop) ;

	ret = MM4SwitchPartition(pSDMMCP, PartitionNumber);

	return ret;
}

//int do_emmcread(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
int do_emmcread(unsigned long long start, unsigned int blks, unsigned char * buffer)
{
	//	TODO
	int res ;
	unsigned int read_start;
	//	local buffer to store data
	P_SDMMC_Properties_T pSDMMCP = (P_SDMMC_Properties_T)(&SDMMC_Prop) ;

	flush_all_dcache();
	if(emmc_inited != 1){
		lgpl_printf("%s: need to run emmcinit first\n", __FUNCTION__) ;
		return -1 ;
	}

	//printf("read data from 0x%x, size is 0x%x\n", start, blks*0x200) ;
   if (pSDMMCP->AccessMode == SECTOR_ACCESS)
		read_start = (unsigned int)(start/HARD512BLOCKLENGTH);
   else
		read_start = (unsigned int)start;
	res = SDIORead(pSDMMCP, read_start, blks, buffer) ;
	if(NoError != res){
		lgpl_printf("\n SDIO read error\n") ;
		return -1 ;
	}

#if 0
	//	dump one block
	for(i=0 ; i<512; i++){
		if(i%8 == 0)
			printf("\n0x%x:\t", start + i) ;
		printf("%2x  ", *(buffer+i)) ;
	}
#endif

	printf("\n*******************  emmcread done   *******************\n") ;
	return 0;
}

//int do_emmcpart(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
int do_emmcpart(int part)
{
	//int part;
	int ret ;
	P_SDMMC_Properties_T pSDMMCP = (P_SDMMC_Properties_T)(&SDMMC_Prop) ;
	//part = simple_strtoul(argv[1], NULL, 10);
	printf("%s: set partition to partition %d\n", __FUNCTION__, part);
	ret = MM4SwitchPartition(pSDMMCP, part) ;
	if(ret != NoError){
		printf("MM4SwitchPartition error\n");
		return ret;
	}else{
		printf("MM4SwitchPartition done\n");
		return 0;
	}
}

int do_emmcerase(unsigned long long offset, unsigned int blks)
{
	int error = 0;
	int partition = 0; 		//	by default, read from user area
	unsigned int end;
	int per_group, groups;
	int c ;
	int res ;
	P_SDMMC_Properties_T pSDMMCP = (P_SDMMC_Properties_T)(&SDMMC_Prop);

	lgpl_printf("%s(offset=0x%08x, blks=%d)\n", __func__, (unsigned)offset, blks);

	if(emmc_inited != 1){
		lgpl_printf("%s: need to run emmcinit first\n", __FUNCTION__);
		return -1 ;
	}

	//	swtich partition
	// printf("%s: switch to partition %d\n", __FUNCTION__, partition);

	res = MM4SwitchPartition(pSDMMCP, partition);
	if(NoError != res){
		lgpl_printf( "\n MM4SwitchPartition error\n");
		return -1 ;
	}

	per_group = pSDMMCP->EraseSize / HARD512BLOCKLENGTH;
	if (blks % per_group) {
		blks = (blks + per_group - 1) & (~(per_group - 1));
	}

	if (pSDMMCP->AccessMode == SECTOR_ACCESS){
		offset /= HARD512BLOCKLENGTH; 	// In sector mode addressing; all addresses need to be specified as block offsets.
		end = offset + blks - 1;
	} else {
		end = offset + (blks - 1) * HARD512BLOCKLENGTH;
	}

	lgpl_printf("Erase from start block 0x%x blks = %d, erase unit = 0x%x(%d blks).\n", 
		(unsigned)offset, (unsigned)blks,  (unsigned)pSDMMCP->EraseSize, (unsigned)per_group);

	res = SDIOErase(pSDMMCP, (unsigned int)offset, end);
	if(NoError != res){
		lgpl_printf("\n SDIO erase error. res = %d.\n", res);
		return -1 ;
	}
	printf("Done.\n");
	return 0 ;
}

int do_emmcwrite(unsigned long long offset, unsigned int blks, unsigned char * emmc_buffer)
{

	//	TODO
#if 1
	int partition = 0; 		//	by default, read from user area
	int res ;
	P_SDMMC_Properties_T pSDMMCP = (P_SDMMC_Properties_T)(&SDMMC_Prop) ;

	if(emmc_inited != 1){
		lgpl_printf("%s: need to run emmcinit first\n", __FUNCTION__) ;
		return -1 ;
	}
#if 0
	//	swtich partition
	res = MM4SwitchPartition(pSDMMCP, partition) ;
	if(NoError != res){

		lgpl_printf( "\n MM4SwitchPartition error\n") ;
		return -1 ;
	}
#endif
	flush_all_dcache();
	if (pSDMMCP->AccessMode == SECTOR_ACCESS)
		offset /= HARD512BLOCKLENGTH;	// In sector mode addressing; all addresses need to be specified as block offsets.
	res = SDIOWrite(pSDMMCP, (unsigned int)offset, blks, emmc_buffer) ;

	if(NoError != res){
		lgpl_printf("\n SDIO write error\n") ;
		return -1 ;
	}
 #endif

	return 0 ;
}

/***************************************************************
*   MM4_MMCReadEXTCSD
* 	Reads in 512 bytes of Extended CSD
*   Input: Pointer to 512 byte buffer
*   Output:
*   Returns: NoError, ReadError or NotSupportedError
*
*****************************************************************/
UINT_T MM4_MMCReadEXTCSD (UINT_T *pBuffer)
{
	UINT_T Cmd;
	UINT_T Retval = NoError;
	P_MM4_SDMMC_CONTEXT_T pContext;
	P_MM4_CNTL1 pMM4_CNTL1;
	P_MM4_BLK_CNTL pMM4_BLK_CNTL;

	// Initialize Flash Properties
	P_SDMMC_Properties_T pSDMMCP = (P_SDMMC_Properties_T)(&SDMMC_Prop) ;

	if (pSDMMCP->SD == XLLP_SD)
		return NoError;			// This is an MMC command only!

	// Assign our context value
	pContext = (P_MM4_SDMMC_CONTEXT_T) pSDMMCP->pContext;
	pMM4_CNTL1 = (P_MM4_CNTL1) &pContext->pMMC4Reg->mm4_cntl1;

	// Set up State
	pSDMMCP->State = READ;

	// This requires a transfer over the data lines.
	pMM4_BLK_CNTL = (P_MM4_BLK_CNTL) &pContext->pMMC4Reg->mm4_blk_cntl;
	pMM4_BLK_CNTL->blk_cnt = 0;
	if (pSDMMCP->SDMA_Mode)
	{
		pMM4_BLK_CNTL->dma_bufsz = MM4_512_HOST_DMA_BDRY;
	    pContext->pMMC4Reg->mm4_sysaddr = (UINT_T) pBuffer;
	} //MMC_SDMA_MODE
	pSDMMCP->Trans.StartDiscardWords = 0;
	pSDMMCP->Trans.EndDiscardWords = 0;								// We'll take all 512 bytes
	pSDMMCP->Trans.TransWordSize = pSDMMCP->ReadBlockSize / 4;		// Total Transfer Size including pre and post bytes
	pSDMMCP->Trans.LocalAddr = (UINT_T) pBuffer;
	pSDMMCP->Trans.WordIndex = 0;									// Stores Index of Current write position

	MM4_SendDataCommandNoAuto12(pSDMMCP, XLLP_MMC_CMD8, NO_ARGUMENT, MM4_SINGLE_BLOCK_TRAN, MM4_CARD_TO_HOST_DATA, MM4_RT_R1 | MM4_48_RES);

	// Wait for the Read to Complete
	while( (pSDMMCP->State != FAULT) && (pSDMMCP->State != READY) ){
		;
	}

	//send CMD13 to check the status of the card
	Retval |= MM4_CheckCardStatus(pSDMMCP, 0x900, R1_LOCKEDCARDMASK);		// Make sure card is transfer mode

	pSDMMCP->State = READY;

	if (Retval != NoError)
		return SDMMC_SWITCH_ERROR;

	return NoError;
}

//int do_emmcexcsd(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
int do_emmcexcsd(unsigned char * buffer)
{
	//	TODO
	//unsigned char * buffer = NULL;
	unsigned int ret,i ;

	printf("*******************  do_emmcexcsd  *******************\n") ;

	//buffer = simple_strtoul(argv[1], NULL, 10) ;
	printf(" read excsd to 0x%x\n", buffer);

	ret = MM4_MMCReadEXTCSD(buffer);
	if(ret != NoError){
		printf("MM4_MMCReadEXTCSD error, ret = 0x%x\n", ret) ;
		return -1 ;
	}

	//	dump one block
	for(i=0 ; i<512; i++){
		if(i%8 == 0)
			printf("\n0x%08x:\t", i) ;
		printf("%02x  ", *(buffer+i)) ;
	}

	printf("do_emmcexcsd done\n");
	return 0 ;
}

