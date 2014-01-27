#include "drmdmx.h"

#include "memmap.h"
#include "customer_keystore_spec.h"
#include "basic_inc.h"
#include "nand_priv.h"
#include "common.h"

#define MEMPOLL_SIZE            (64 * 1024) /* 64kB. In fact, the real size should be less than 16kB */
#define MEMPOOL_ALIGNED_SIZE    (MEMPOLL_SIZE + 128)    /* make it (64 + 64) header/end aligned. */
#define CUSTK_IMAGE_MAX_SIZE 4096
#define PRELOAD_BUF_SIZE    (512 * 1024)
extern int __preload_buf_start;
#define PRELOAD_BUF_ADDR    ((unsigned int)(&__preload_buf_start))
#define VT_BUF_ADDR     (PRELOAD_BUF_ADDR)
#define FIGO_BUF_ADDR       (VT_BUF_ADDR + 4096)
#define IMG2_PART1_SIZE     52<<10
#define VT_OFFSET       (IMG2_PART1_SIZE)
#define VT_SIZE         4096
#define FIGO_OFFSET     ((VT_OFFSET) + (VT_SIZE))
#define FIGO_SIZE       4096
#define KEY_SIZE        4096

static UNSG8 mempool[MEMPOOL_ALIGNED_SIZE];
static UNSG8 dtcmpool[MEMPOOL_ALIGNED_SIZE];
static UNSG8 gp_cust_figo_image[CUSTK_IMAGE_MAX_SIZE] ={0};
static unsigned char *gp_figo_buf  = FIGO_BUF_ADDR;

extern struct mv_nand_data nand_data;

static void Wait_FIGO()
{
  UNSG32 nRet = 0;
  T32SECSTATUS_CFG cfgSecStatus;
  //Waiting for FIGO Ready
  while(1) {
    cfgSecStatus.u32 = MV_FIGODRV_IO_RD32(DRMFIGOREG_SECSTAT);
    if ( cfgSecStatus.uCFG_flag  == SECSTATUS_CFG_flag_DISABLED )
    {
      continue;
    }
    if ( cfgSecStatus.uCFG_flag  == SECSTATUS_CFG_flag_FAILED )
    {
      nRet    = MV_FIGODRV_ERR;
      break;
    }
    if ( cfgSecStatus.uCFG_flag  == SECSTATUS_CFG_flag_ENABLED )
    {
      nRet    = MV_FIGODRV_OK;
      break;
    }
  }
  return;
}

static int parse_figo(char * figobuff)
{
  unsigned int figo_len, figo_off;
  unsigned int flags;

  flags = ((unsigned int *)gp_figo_buf)[0];
  figo_off = ((unsigned int *)gp_figo_buf)[1];
  figo_len = ((unsigned int *)gp_figo_buf)[2];
  printf("figo magic = %x, figo_off = %d, figo_len = %d\n", flags, figo_off, figo_len);

  if(figo_off > FIGO_SIZE || figo_len > FIGO_SIZE || (figo_off + figo_len) > FIGO_SIZE )
    return -1;

  UtilMemCpy(figobuff, &gp_figo_buf[figo_off], figo_len);

  return 0;
}

static int get_figo_img(char * figobuff)
{
  int i;
  for(i=1; i<9;i++) {
    if(mv_nand_block_bad(i*nand_data.szofblk, 0))
      continue;
    if( mv_nand_read_block( (loff_t)(i*nand_data.szofblk + FIGO_OFFSET),
                            gp_figo_buf, FIGO_SIZE + KEY_SIZE) < 0 )
      continue;

    if (0 == parse_figo(figobuff))
      break;
  }
  if(i==9) {
    //      lgpl_printf("Read FIGO image and customer key error!\n");
    return -1;
  }
  return 0;
}

static SIGN32 MV_DRMLIB_PreProcess_Customer_KeyStore(UNSG8* pbCustKeyStore,
                                                     UNSG32 uKeyStoreSize,
                                                     UNSG32* uKeyNum)
{
  UNSG32 uAESKeyNum = 0;
  UNSG32 uRSAKeyNum = 0;
  UNSG32 uKeyId = 0;
  UNSG32 uSize = 0;
  UNSG32 nRet = 0;
  UNSG32 i = 0;
  UNSG32 uAESKeyCnt = 0;
  UNSG32 uRSAPubKeyCnt = 0;
  UNSG32 uRSAPrvKeyCnt = 0;

  // Parse the keystore header
  uAESKeyNum = *((UNSG32*)pbCustKeyStore);
  uRSAKeyNum = *((UNSG32*)(pbCustKeyStore + 4));

  if(uAESKeyNum == 0x00 && uRSAKeyNum == 0x00)
  {
    //lgpl_printf("Invalid Customer Key Store: AES Key Num = 0x%x, RSA Key Num = 0x%x\n", uAESKeyNum,uRSAKeyNum);
    return -1;
  }
  else
  {
    //lgpl_printf("Valid Customer Key Store: AES Key Num = 0x%x, RSA Key Num = 0x%x\n", uAESKeyNum,uRSAKeyNum);
  }
  pbCustKeyStore = pbCustKeyStore + 8; // 8 is lenght of header

  for(i =0; i < uAESKeyNum + uRSAKeyNum; i ++)
  {
    uKeyId = *((UNSG32*)pbCustKeyStore);
    uSize = *((UNSG32*)(pbCustKeyStore + 4));
    if(uKeyId ==0x80 || uKeyId == 0x81 )
    {
      pbCustKeyStore = pbCustKeyStore + 8;

      //lgpl_printf("No.%d AES Customer Key Store Useful Info:\n", uAESKeyCnt + 1);
      //lgpl_printf("Enc_Root_ID = [0x%x], Sign_Key_ID = [0x%x] ", *(pbCustKeyStore + 0x0), *(pbCustKeyStore + 0x48));
      //lgpl_printf("Sign_Type = [0x%x], Sign_Len = [0x%x]\n", *(pbCustKeyStore + 0x4c), *(pbCustKeyStore + 0x4d));
      uAESKeyCnt++;
    }
    else if(uKeyId == 0x82)
    {
      pbCustKeyStore = pbCustKeyStore + 8;

      //lgpl_printf("No.%d RSA Public Customer Key Store Useful Info:\n", uRSAPubKeyCnt + 1);
      //lgpl_printf("Enc_Root_ID = [0x%x], Sign_Key_ID = [0x%x] ", *(pbCustKeyStore + 0x0), *(pbCustKeyStore + 0x48));
      //lgpl_printf("Sign_Type = [0x%x], Sign_Len = [0x%x]\n", *(pbCustKeyStore + 0x4c), *(pbCustKeyStore + 0x4d));
      uRSAPubKeyCnt++;
    }
    else if(uKeyId == 0x8e)
    {
      pbCustKeyStore = pbCustKeyStore + 8;

      //lgpl_printf("No.%d RSA Private Customer Key Store Useful Info:\n", uRSAPrvKeyCnt + 1);
      //lgpl_printf("Enc_Root_ID = [0x%x], Sign_Key_ID = [0x%x] ", *(pbCustKeyStore + 0x0), *(pbCustKeyStore + 0x48));
      //lgpl_printf("Sign_Type = [0x%x], Sign_Len = [0x%x]\n", *(pbCustKeyStore + 0x4c), *(pbCustKeyStore + 0x4d));
      uRSAPrvKeyCnt++;
    }
    else
    {
      //lgpl_printf("Contian Invalid Customer Key ID:0x%x, Pls check the key store file\n", uKeyId);
      return -1;
    }

    pbCustKeyStore = pbCustKeyStore + uSize;

    if(pbCustKeyStore >= (pbCustKeyStore + uKeyStoreSize))
      break;
  }

  *uKeyNum = uAESKeyNum + uRSAKeyNum;

  return 0;
}

static void MV_DRMLIB_Check_ReturnError(UNSG32 uErrorCode)
{
  switch(uErrorCode){
    case DRMDiag_CUSTOMER_KEYSTORE_CTX_uErrorCode_RETURN_OK:
      //lgpl_printf("Customer Key has been set successfully\n");
      break;
    case DRMDiag_CUSTOMER_KEYSTORE_CTX_uErrorCode_INVALID_ENC_KEYID:
      //lgpl_printf("Invalid encryption key id\n");
      break;
    case DRMDiag_CUSTOMER_KEYSTORE_CTX_uErrorCode_INVALID_SIGN_KEYID:
      //lgpl_printf("Invalid signed key id\n");
      break;
    case DRMDiag_CUSTOMER_KEYSTORE_CTX_uErrorCode_AESUNWRAP_FAILED:
      //lgpl_printf("SubKey AES Unwrap Failed\n");
      break;
    case DRMDiag_CUSTOMER_KEYSTORE_CTX_uErrorCode_CMAC_VERIFY_FAILED:
      //lgpl_printf("CMAC valud is not valid\n");
      break;
    default:
      //lgpl_printf("Invalid Return Error Code, Check the Image again, ErrorCode = [0x%x]\n", uErrorCode);
      break;
  }
}

SIGN32 MV_DRMLIB_Load_Customer_Key(UNSG8* pbCustKeyStore, UNSG32 uKeyStoreSize)
{
  FIGODRV_HANDLE  hFigoDrv = NULL;
  UNSG32 i = 0;
  UNSG32 uEncImgSize = MEMPOLL_SIZE;
  SIGN32 nRet = 0;
  UNSG32 uErrCode = 0;
  UNSG32 uCheckSize = 1024;
  UNSG8* pbSrcDDR = NULL;
  UNSG8* pbDstImg = NULL;
  SIE_DRMROM_CMD  drmCmd;
  SIE_DRMROM_CMD  drmRsp;
  T32SECSTATUS_CFG cfgSecStatus;
  void *pDiagDtcm = NULL;
  UNSG32 uReady = 0;
  UNSG32 uRspFlag = 0;
  UNSG32 uCmdFlag = 0;
  UNSG32 uErrorCode = 0;
  UNSG32 uKeyNum = 0;
  UNSG32 uKeyId = 0;
  UNSG32 uSize = 0;
  UNSG8 * pbTemp = NULL;

  get_figo_img(gp_cust_figo_image);
  flush_all_dcache();
  nRet = MV_DRMLIB_PreProcess_Customer_KeyStore(pbCustKeyStore, uKeyStoreSize, &uKeyNum);
  if(0 != nRet) {
    lgpl_printf("Warning: Failed to preprocess keystore \n");
    return -1;
  }
  pbCustKeyStore = pbCustKeyStore + 8;    //Skip the key store header: 16 bytes
  uKeyStoreSize  = uKeyStoreSize - 8;

  //!Wait FIGO Ready
  Wait_FIGO();
  //!Prepare the Image
  {
    UNSG8* pbAlloc = NULL;
    UNSG32  uVSize = MEMPOLL_SIZE;
    unsigned int i = 0;
    pbAlloc = (UNSG8*)mempool;
    pbSrcDDR = (UNSG8*)(ALIGN32(pbAlloc) + 32);
    pbTemp = pbSrcDDR;
    for (i = 0; i< uVSize - 64; i = i + 4)
    {
      pbTemp = pbSrcDDR + i;
      *(UNSG32 *)(pbTemp) = 0;
    }
  }
  pbDstImg = pbSrcDDR;
  {
    unsigned int i = 0;
    for (i = 0; i < CUSTK_IMAGE_MAX_SIZE; i++)
      *(pbDstImg + i) = *((UNSG8*)gp_cust_figo_image + i);

  }
  //! Init DTCM
  {
    int i = 0;
    void* pDiagDtcm = NULL;
    UNSG32 uSize = 0;
    pDiagDtcm = (void *)(ALIGN32(dtcmpool) + 32);
    for(i = 0 ; i< (9688/4); i = i+1 )
      *(((UNSG32*)pDiagDtcm) + i) = 0;

    uSize = 9688 - RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_cmdFlag;
    //lgpl_printf("Before Init the DTCM, the Size = [0x%x]\n", uSize);
    for ( i = 0; i < uSize/4; i +=1 )
    {
      MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_cmdFlag + i*4, 0);
    }
  }
  flush_all_dcache();
  //Load FIGO Image
  {
    int i = 0;
    for (i = 0; i < sizeof(drmCmd)/8; i = i + 8)
      *((UNSG64 *)&drmCmd + i) = 0;
    cfgSecStatus.u32 = MV_FIGODRV_IO_RD32(DRMFIGOREG_SECSTAT);
    if ( cfgSecStatus.uCFG_flag  != SECSTATUS_CFG_flag_ENABLED )
    {
      return MV_FIGODRV_ERR_INVLAID_ARG;
    }
    //! Check figo command status
    drmCmd.u32DRMROM_CMD_STAT = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_STAT);
    if ( drmCmd.uSTAT_en )
    {
      return MV_FIGODRV_ERR_INVLAID_STATUS;
    }

    drmCmd.uCMD_CFG_tag     = DRMROM_CMD_TYPE_LD_FIGOIMG;
    drmCmd.uCMD_CFG_nonce   = (UNSG32)0xffffffff;

    drmCmd.uCMD_DAT0_crcCmd32   = 0;
    drmCmd.uCMD_DAT1_imgSz      = 0;
    drmCmd.uCMD_DAT2_imgSrcAddr = (UNSG32)pbDstImg;
    drmCmd.uSTAT_en = 1;
    //! Issue command
    MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_CMD_CFG,  drmCmd.u32DRMROM_CMD_CMD_CFG);
    MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_CMD_DAT0, drmCmd.u32DRMROM_CMD_CMD_DAT0);
    MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_CMD_DAT1, drmCmd.u32DRMROM_CMD_CMD_DAT1);
    MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_CMD_DAT2, drmCmd.u32DRMROM_CMD_CMD_DAT2);
    MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_CMD_DAT3, drmCmd.u32DRMROM_CMD_CMD_DAT3);

    MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_STAT,     drmCmd.u32DRMROM_CMD_STAT);
    //! Check figo command status
    while(1)
    {
      drmRsp.u32DRMROM_CMD_STAT = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_STAT);
      if ( drmRsp.uSTAT_en )
      {
        continue;
      }
      break;
    }
    //! Load Reponse
    drmRsp.u32DRMROM_CMD_RSP_CFG = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_RSP_CFG);
    drmRsp.u32DRMROM_CMD_RSP_DAT0 = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_RSP_DAT0);
    drmRsp.u32DRMROM_CMD_RSP_DAT1 = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_RSP_DAT1);

    if ( drmRsp.uRSP_CFG_tag != DRMROM_CMD_TYPE_LD_FIGOIMG)
    {
      return MV_FIGODRV_ERR;
    }
    //!Todo verify crc32 value with nonce
    uErrorCode = drmRsp.uRSP_DAT1_error;
    if (0 != uErrorCode)
      return MV_FIGODRV_ERR;

  }
  flush_all_dcache();
  // Load the customer keystore into DTCM
  {
    UNSG32 uAESKeyCnt = 0;
    UNSG32 uRSAPubKeyCnt = 0;
    UNSG32 uRSAPrvKeyCnt = 0;
    UNSG32 j = 0;
    for(i =0; i< uKeyNum; i++)
    {
      uKeyId = *(UNSG32*)pbCustKeyStore;      //! First Word is Key ID
      uSize = *((UNSG32*)(pbCustKeyStore + 4));   //! Second word is size
      //printf("%s, %d, key id == 0x%x\n", __FUNCTION__, __LINE__, uKeyId);
      if((uKeyId == 0x80 || uKeyId == 0x81)&& uSize == 0x90)
      {
        uAESKeyCnt++;
#if 0
        pbCustKeyStore = pbCustKeyStore + 8 + uSize;
        continue;
#endif
        lgpl_printf("Load No.%d AES Key into User Key DTCM Area\n", uAESKeyCnt);
      }
      else if(uKeyId == 0x82&& uSize == 0x140)
      {
        uRSAPubKeyCnt++;
        lgpl_printf("Load No.%d RSA Public Key into User Key DTCM Area\n", uRSAPubKeyCnt);
      }
      else if(uKeyId == 0x8e && uSize == 0x180)
      {
        uRSAPrvKeyCnt++;
        lgpl_printf("Load No.%d RSA Private Key into User Key DTCM Area\n", uRSAPrvKeyCnt);
      }
      else
      {
        lgpl_printf("Warning: Invalid key store!Key Id is:0x%x, keystore size is: 0x%x\n", uKeyId, uSize);
        uErrorCode = 0xff;
        break;
      }

      pbCustKeyStore = pbCustKeyStore + 8;
      for(j = 0; j < uSize/4; j++)
      {
        MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_custKeystore + j*4,  *(UNSG32*)(pbCustKeyStore + j*4));
      }
      lgpl_printf("Loading Secure Customer Key Store is finished\n");

      do{
        uRspFlag = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_rspFlag);
        uRspFlag = uRspFlag & 0xffff;
        MV_FIGODRV_SLEEP(1000);
      } while (uRspFlag != 0x3010);
      // Send Command to FIOG image
      {
        MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_uKeyId, uKeyId);
        uCmdFlag = 0xa5;
        MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_cmdFlag, uCmdFlag);
      }

      // Wait the Command is executed
      do{
        uCmdFlag = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_cmdFlag);
        uCmdFlag = uCmdFlag & 0xff;
        MV_FIGODRV_SLEEP(1000);
      } while (uCmdFlag != 0x30);
      uErrorCode = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + BA_DRMDiag_CUSTOMER_KEYSTORE_CTX_uErrorCode);
      uErrorCode = uErrorCode&0xff;
      MV_DRMLIB_Check_ReturnError(uErrorCode);

      if(uErrorCode != 0)
      {
        break;
      }
      //lgpl_printf("******* Load Customer Key store successfully*********\n");
      pbCustKeyStore = pbCustKeyStore + uSize;
    }
  }
  uCmdFlag = 0x5a;
  MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_cmdFlag, uCmdFlag);

  do{
    uRspFlag = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_rspFlag);
    uRspFlag = uRspFlag & 0xffff;
    MV_FIGODRV_SLEEP(1000);
  } while (uRspFlag != 0x88de);

  //  uCmdFlag = 0xf3;
  //  MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_cmdFlag, uCmdFlag);
  lgpl_printf("Finish to load Customer Key store\n");
  uErrorCode = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + BA_DRMDiag_CUSTOMER_KEYSTORE_CTX_uErrorCode);
  uErrorCode = uErrorCode&0xff;

  MV_DRMLIB_Check_ReturnError(uErrorCode);

  if(uErrorCode != 0)
  {
    lgpl_printf("uErrorCode = 0x%x\n", uErrorCode);
    //return -1;
  }

  uCmdFlag = 0xde;
  MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_cmdFlag, uCmdFlag);
  // Wait Figo ROM code ready
  Wait_FIGO();
  lgpl_printf("FIGO is running at FIGO ROM Code agian\n");

  if(uErrorCode != 0)
  {
    return -1;
  }

  return MV_FIGODRV_OK;
}

HRESULT MV_Gen_Random_Server(UINT8* pbOutDat, UINT32*uOutSize )
{
    HRESULT lRes = S_OK;

	UNSG32 i = 0;
	UNSG32 uEncImgSize = MEMPOLL_SIZE;
	SIGN32 nRet = 0;
	UNSG32 uErrCode = 0;
	UNSG32 uCheckSize = 1024;
	UNSG8* pbSrcDDR = NULL;
	UNSG8* pbDstImg = NULL;
	SIE_DRMROM_CMD	drmCmd;
	SIE_DRMROM_CMD	drmRsp;
	T32SECSTATUS_CFG cfgSecStatus;
	SIE_DRMDiag_CUSTOMER_KEYSTORE_CTX *pDiagDtcm = NULL;
	UNSG32 uReady = 0;
	UNSG32 uRspFlag = 0;
	UNSG32 uCmdFlag = 0;
	UNSG32 uErrorCode = 0;
	UNSG32 uKeyNum = 0;
	UNSG32 uKeyId = 0;
	UNSG32 uSize = 0;
	UNSG32 uOut_data = 0;
	UNSG8 * pbTemp = NULL;

        UINT8* pbBufAligned32 = NULL;
        UINT32 uBufSizeAligned32 = (32 + 31)&(~(32-1));

    if((pbOutDat ==NULL) && (uOutSize == NULL) && (*uOutSize == 0))
    {
        lRes = MV_FIGODRV_ERR_INVLAID_ARG;
        printf("invalid lenght of randon number(0)\n");
        return lRes;
    }
    uBufSizeAligned32 = (*uOutSize + 15)&(~(16-1));
	#if 1
	{
		get_figo_img(gp_cust_figo_image);
	}
	#endif
	flush_all_dcache();

	//!Wait FIGO Ready
	Wait_FIGO();
	//!Prepare the Image
	{
		UNSG8* pbAlloc = NULL;
		UNSG32	uVSize = MEMPOLL_SIZE;
		unsigned int i = 0;
		pbAlloc = (UNSG8*)mempool;
		pbSrcDDR = (UNSG8*)(ALIGN32(pbAlloc) + 32);
		pbTemp = pbSrcDDR;
		for (i = 0; i< uVSize - 64; i = i + 4)
		{
			pbTemp = pbSrcDDR + i;
			*(UNSG32 *)(pbTemp) = 0;
		}
	}
	pbDstImg = pbSrcDDR;
	{
		unsigned int i = 0;
		for (i = 0; i < CUSTK_IMAGE_MAX_SIZE; i++)
			*(pbDstImg + i) = *((UNSG8*)gp_cust_figo_image + i);

	}
	//! Init DTCM
	{
		int i = 0;
		SIE_DRMDiag_CUSTOMER_KEYSTORE_CTX* pDiagDtcm = NULL;
		UNSG32 uSize = 0;
		pDiagDtcm = (SIE_DRMDiag_CUSTOMER_KEYSTORE_CTX *)(ALIGN32(dtcmpool) + 32);
		for(i = 0 ; i< sizeof(SIE_DRMDiag_CUSTOMER_KEYSTORE_CTX)/4; i = i+1 )
			*(((UNSG32*)pDiagDtcm) + i) = 0;

		uSize = sizeof(SIE_DRMDiag_CUSTOMER_KEYSTORE_CTX) - RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_cmdFlag;
		//lgpl_printf("Before Init the DTCM, the Size = [0x%x]\n", uSize);
		for ( i = 0; i < uSize/4; i +=1 )
		{
			MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_cmdFlag + i*4, 0);
		}
	}
	flush_all_dcache();
	//Load FIGO Image
	{
		int i = 0;
		for (i = 0; i < sizeof(drmCmd)/8; i = i + 8)
			*((UNSG64 *)&drmCmd + i) = 0;
		cfgSecStatus.u32 = MV_FIGODRV_IO_RD32(DRMFIGOREG_SECSTAT);
		if ( cfgSecStatus.uCFG_flag  != SECSTATUS_CFG_flag_ENABLED )
		{
			return MV_FIGODRV_ERR_INVLAID_ARG;
		}
		//! Check figo command status
		drmCmd.u32DRMROM_CMD_STAT = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_STAT);
		if ( drmCmd.uSTAT_en )
		{
			return MV_FIGODRV_ERR_INVLAID_STATUS;
		}

		drmCmd.uCMD_CFG_tag		= DRMROM_CMD_TYPE_LD_FIGOIMG;
		drmCmd.uCMD_CFG_nonce	= (UNSG32)0xffffffff;

		drmCmd.uCMD_DAT0_crcCmd32	= 0;
		drmCmd.uCMD_DAT1_imgSz		= 0;
		drmCmd.uCMD_DAT2_imgSrcAddr = (UNSG32)pbDstImg;
		drmCmd.uSTAT_en = 1;
		//! Issue command
		MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_CMD_CFG,	drmCmd.u32DRMROM_CMD_CMD_CFG);
		MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_CMD_DAT0, drmCmd.u32DRMROM_CMD_CMD_DAT0);
		MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_CMD_DAT1, drmCmd.u32DRMROM_CMD_CMD_DAT1);
		MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_CMD_DAT2, drmCmd.u32DRMROM_CMD_CMD_DAT2);
		MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_CMD_DAT3, drmCmd.u32DRMROM_CMD_CMD_DAT3);

		MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_STAT,		drmCmd.u32DRMROM_CMD_STAT);
		//! Check figo command status
		while(1)
		{
			drmRsp.u32DRMROM_CMD_STAT = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_STAT);
			if ( drmRsp.uSTAT_en )
			{
				continue;
			}
			break;
		}
		//! Load Reponse
		drmRsp.u32DRMROM_CMD_RSP_CFG = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_RSP_CFG);
		drmRsp.u32DRMROM_CMD_RSP_DAT0 = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_RSP_DAT0);
		drmRsp.u32DRMROM_CMD_RSP_DAT1 = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + RA_DRMROM_CMD_RSP_DAT1);

		if ( drmRsp.uRSP_CFG_tag != DRMROM_CMD_TYPE_LD_FIGOIMG)
		{
			return MV_FIGODRV_ERR;
		}
		//!Todo verify crc32 value with nonce
		uErrorCode = drmRsp.uRSP_DAT1_error;
		if (0 != uErrorCode)
			return MV_FIGODRV_ERR;

	}
	flush_all_dcache();
	// Load the customer keystore into DTCM
	{
		UNSG32 uAESKeyCnt = 0;
		UNSG32 uRSAPubKeyCnt = 0;
		UNSG32 uRSAPrvKeyCnt = 0;
		UNSG32 j = 0;
		//for(i =0; i< uKeyNum; i++)
		{
			uKeyId = uBufSizeAligned32;		//! First Word is Key ID

			do{
				uRspFlag = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_rspFlag);
				uRspFlag = uRspFlag & 0xffff;
				MV_FIGODRV_SLEEP(1000);
			} while (uRspFlag != 0x3010);
			// Send Command to FIOG image
			{
				MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_uKeyId, uKeyId);
				uCmdFlag = 0xbf;
				MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_cmdFlag, uCmdFlag);
			}
		}
	}
	do{
		uRspFlag = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_rspFlag);
		uRspFlag = uRspFlag & 0xffff;
		MV_FIGODRV_SLEEP(1000);
	} while (uRspFlag != 0x88de);
	for ( i = 0; i < uBufSizeAligned32/4; i +=1 )
	{
		uOut_data = MV_FIGODRV_IO_RD32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_custKeystore + i*4);
		memcpy(((UINT8 *)pDiagDtcm->ie_custKeystore + i*4), &uOut_data, sizeof(uOut_data));
	}
	memcpy(pbOutDat, pDiagDtcm->ie_custKeystore, *uOutSize);
//	uCmdFlag = 0xf3;
//	MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_cmdFlag, uCmdFlag);

	uCmdFlag = 0xde;
	MV_FIGODRV_IO_WR32(DRMFIGOREG_CMDSTAT + RA_DRMDiag_CUSTOMER_KEYSTORE_CTX_cmdFlag, uCmdFlag);
	// Wait Figo ROM code ready
	Wait_FIGO();

	return MV_FIGODRV_OK;
}
