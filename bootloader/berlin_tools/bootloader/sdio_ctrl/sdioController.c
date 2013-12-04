
/******************************************************************************
**
**  FILENAME:       mm4_mmc_controller.c
**
**  PURPOSE: MMC and SD specific low level controller routines for the MM4 controller
**           
**            
**                  
**
******************************************************************************/
#include "sdioController.h"
#include "util.h"
//#include "misc.h"
#ifdef	EMMC_DEBUG
#define	printf lgpl_printf
#else
#define	printf 
#endif
void MMC4HighSpeedSelect(P_MM4_SDMMC_CONTEXT_T pContext, UINT_T hs_mode)
{
  P_MM4_CNTL1 pMM4_CNTL1;
  MM4_CNTL1_UNION MM4_cntl1; 

  pMM4_CNTL1 = (P_MM4_CNTL1)((VUINT32_T) &pContext->pMMC4Reg->mm4_cntl1);
  MM4_cntl1.mm4_cntl1_value = *(VUINT_T *)pMM4_CNTL1;
  MM4_cntl1.mm4_cntl1_bits.hispeed = hs_mode;

  *(VUINT_T *)pMM4_CNTL1 = MM4_cntl1.mm4_cntl1_value;
  return;
}

/******************************************************************************
  Description:
    Start MMC/SD Internal bus clock.  MUST be done to start MM4CLK!
    Only after starting bus clock, communication between 
    controller and card is possible
  Input Parameters:
    pContext--Pointer to MMC context structure
  Output Parameters:
    None
  Returns:
    None  
*******************************************************************************/
void MMC4StartInternalBusClock(P_MM4_SDMMC_CONTEXT_T pContext)
{
  P_MM4_CNTL2 pMM4_CNTL2;
  MM4_CNTL2_UNION MM4_cntl2; 

  pMM4_CNTL2 = (P_MM4_CNTL2)((VUINT32_T) &pContext->pMMC4Reg->mm4_cntl2);
  MM4_cntl2.mm4_cntl2_value = *(VUINT_T *)pMM4_CNTL2;
  MM4_cntl2.mm4_cntl2_bits.inter_clk_en = 1;
  *(VUINT_T *)pMM4_CNTL2 = MM4_cntl2.mm4_cntl2_value;

  // Wait for clock to become stable. * TBD * Add timeout
  do 
  {
  	MM4_cntl2.mm4_cntl2_value = *(VUINT_T *)pMM4_CNTL2;
  } while (!MM4_cntl2.mm4_cntl2_bits.inter_clk_stable);

  return;
}




/******************************************************************************
  Description:
    Start MMC bus clock. Only after starting bus clock, communication between 
    controller and card is possible
  Input Parameters:
    pContext--Pointer to MMC context structure
  Output Parameters:
    None
  Returns:
    None  
*******************************************************************************/
void MMC4StartBusClock(P_MM4_SDMMC_CONTEXT_T pContext)
{
  P_MM4_CNTL2 pMM4_CNTL2;
  MM4_CNTL2_UNION MM4_cntl2; 

  pMM4_CNTL2 = (P_MM4_CNTL2)((VUINT32_T) &pContext->pMMC4Reg->mm4_cntl2);
  MM4_cntl2.mm4_cntl2_value = *(VUINT_T *)pMM4_CNTL2;
     
  MM4_cntl2.mm4_cntl2_bits.mm4clken = 1;

  *(VUINT_T *)pMM4_CNTL2 = MM4_cntl2.mm4_cntl2_value;
  return;
}

/******************************************************************************
  Description:
    Stops MMC bus clock.
  Input Parameters:
    pContext--Pointer to MMC context structure
  Output Parameters:
    None
  Returns:
    None  
*******************************************************************************/
void MMC4StopBusClock (P_MM4_SDMMC_CONTEXT_T pContext)
{
  //UINT32_T retry_count = 0xff;
  P_MM4_CNTL2 pMM4_CNTL2; 
  MM4_CNTL2_UNION MM4_cntl2;     

  // Request bus clock stop 
  pMM4_CNTL2 = (P_MM4_CNTL2)((VUINT32_T) &pContext->pMMC4Reg->mm4_cntl2);
  MM4_cntl2.mm4_cntl2_value = *(VUINT_T *)pMM4_CNTL2;
  MM4_cntl2.mm4_cntl2_bits.mm4clken = 0;
  *(VUINT_T *)pMM4_CNTL2 = MM4_cntl2.mm4_cntl2_value;
  
  return;
}


/******************************************************************************
  Description:
    Set a new MMC bus clock rate. This function stops and resumes bus clock.
  Input Parameters:
    pContext
      Pointer to MMC context structure
    rate
      bus clock speed
  Output Parameters:
    None
  Returns:
    None  
*******************************************************************************/
void MMC4SetBusRate(P_MM4_SDMMC_CONTEXT_T pContext, UINT_T rate) 
{
  P_MM4_CNTL2 pMM4_CNTL2; 
  MM4_CNTL2_UNION MM4_cntl2; 

  // Request bus clock stop, set rate, start clock. 
  MMC4StopBusClock (pContext);
  pMM4_CNTL2 = (P_MM4_CNTL2)((VUINT32_T) &pContext->pMMC4Reg->mm4_cntl2);
  MM4_cntl2.mm4_cntl2_value = *(VUINT_T *)pMM4_CNTL2;    

  // Update the rate and start the clock.
  MM4_cntl2.mm4_cntl2_bits.sd_freq_sel_lo = (rate & 0xFF);
  MM4_cntl2.mm4_cntl2_bits.sd_freq_sel_hi = ((rate >> 8) & 3);
  MM4_cntl2.mm4_cntl2_bits.mm4clken = 1;
  *(VUINT_T *)pMM4_CNTL2 = MM4_cntl2.mm4_cntl2_value;
  
  MMC4StartInternalBusClock(pContext);

  return;
}


/******************************************************************************
  Description:
    This routine unmasks and enables or masks and disables required interrupts 
    needed by the driver
    Input Parameters:
    	pContext
      		Pointer to MMC context structure
			Desire - Enable or Disable the interrupts
  Output Parameters:
    None
  Returns:
    None  
*******************************************************************************/
void MMC4EnableDisableIntSources(P_MM4_SDMMC_CONTEXT_T pContext, UINT_T Desire)
{
  P_MM4_I_STAT pMM4_I_STAT; 
  MM4_I_STAT_UNION MM4_i_stat;

  // Capture existing Value
  pMM4_I_STAT = (P_MM4_I_STAT)((VUINT32_T) &pContext->pMMC4Reg->mm4_i_sig_en);

  MM4_i_stat.mm4_i_stat_value = *(VUINT_T*)pMM4_I_STAT; 
  // Route the interrupt signal enable register 
  MM4_i_stat.mm4_i_stat_bits.cmdcomp = Desire;
  MM4_i_stat.mm4_i_stat_bits.xfrcomp = Desire;
  MM4_i_stat.mm4_i_stat_bits.bufwrrdy = Desire; 	
  MM4_i_stat.mm4_i_stat_bits.bufrdrdy = Desire;
  // KT added for card detection
  MM4_i_stat.mm4_i_stat_bits.cdins = Desire;
  MM4_i_stat.mm4_i_stat_bits.cdrem = Desire;
  //
  MM4_i_stat.mm4_i_stat_bits.cdint = Desire;
  MM4_i_stat.mm4_i_stat_bits.errint = Desire;
  MM4_i_stat.mm4_i_stat_bits.ctoerr = Desire;   // KT added
  MM4_i_stat.mm4_i_stat_bits.ccrcerr= Desire;   // KT added
  MM4_i_stat.mm4_i_stat_bits.cenderr = Desire;
  MM4_i_stat.mm4_i_stat_bits.cidxerr = Desire;  //
  MM4_i_stat.mm4_i_stat_bits.dtoerr = Desire;
  MM4_i_stat.mm4_i_stat_bits.dcrcerr = Desire;
  MM4_i_stat.mm4_i_stat_bits.denderr = Desire;
#if SDIO_DMA
  MM4_i_stat.mm4_i_stat_bits.dmaint = Desire;    //KT added
  MM4_i_stat.mm4_i_stat_bits.ac12err = Desire;   //KT added auto CMD12 error
#endif

  // Write it out
  *(VUINT_T*)pMM4_I_STAT = MM4_i_stat.mm4_i_stat_value;

  // Now remove the masks	
  pMM4_I_STAT = (P_MM4_I_STAT)((VUINT32_T) &pContext->pMMC4Reg->mm4_i_stat_en);
  MM4_i_stat.mm4_i_stat_value = *(VUINT_T*)pMM4_I_STAT; 
  MM4_i_stat.mm4_i_stat_bits.cmdcomp = Desire;
  MM4_i_stat.mm4_i_stat_bits.xfrcomp = Desire;
  MM4_i_stat.mm4_i_stat_bits.bufwrrdy = Desire; 	
  MM4_i_stat.mm4_i_stat_bits.bufrdrdy = Desire;
  // KT added for card detection
  MM4_i_stat.mm4_i_stat_bits.cdins = Desire;
  MM4_i_stat.mm4_i_stat_bits.cdrem = Desire;
  //
//  MM4_i_stat.mm4_i_stat_bits.cdint = Desire;	--> Satya
  MM4_i_stat.mm4_i_stat_bits.errint = Desire;
  MM4_i_stat.mm4_i_stat_bits.ctoerr = Desire;
  MM4_i_stat.mm4_i_stat_bits.ccrcerr= Desire;   // KT added
  MM4_i_stat.mm4_i_stat_bits.cenderr = Desire;  //
  MM4_i_stat.mm4_i_stat_bits.cidxerr = Desire;  //
  MM4_i_stat.mm4_i_stat_bits.dtoerr = Desire;
  MM4_i_stat.mm4_i_stat_bits.dcrcerr = Desire;
  MM4_i_stat.mm4_i_stat_bits.denderr = Desire;
  
#if SDIO_DMA
  MM4_i_stat.mm4_i_stat_bits.dmaint = Desire;    //KT added
  MM4_i_stat.mm4_i_stat_bits.ac12err = Desire;   //KT added auto CMD12 error
#endif

  // Write it out
  *(VUINT_T*)pMM4_I_STAT = MM4_i_stat.mm4_i_stat_value;

  printf("\n MM4_i_stat.mm4_i_stat_value is 0x%x", MM4_i_stat.mm4_i_stat_value);


  return;
}

/******************************************************************************
  Description:
    Set the data response timeout value.
  Input Parameters:
    pContext
      Pointer to MMC context structure
    CounterValue  
      the value which will be written into DTOCNTR
  Output Parameters:
    None
  Returns:
    None  
*******************************************************************************/
void MMC4SetDataTimeout(P_MM4_SDMMC_CONTEXT_T pContext, UINT_T CounterValue)
{
 P_MM4_CNTL2 pMM4_CNTL2; 
 MM4_CNTL2_UNION MM4_cntl2; 

 // Set the register
 pMM4_CNTL2 = (P_MM4_CNTL2)((VUINT32_T) &pContext->pMMC4Reg->mm4_cntl2);
 MM4_cntl2.mm4_cntl2_value = *(VUINT_T *)pMM4_CNTL2;    
 MM4_cntl2.mm4_cntl2_bits.dtocntr = CounterValue; 	

 // Write Back
 *(VUINT_T *)pMM4_CNTL2 = MM4_cntl2.mm4_cntl2_value;    
 return;
}

/******************************************************************************
  Description:
    This function will induce a software reset of all MMC4 data lines
  Input Parameters:
    pContext
      Pointer to MMC context structure
  Output Parameters:
    None
  Returns:
    None  
*******************************************************************************/
void MMC4DataSWReset(P_MM4_SDMMC_CONTEXT_T pContext)
{
 P_MM4_CNTL2 pMM4_CNTL2; 
 MM4_CNTL2_UNION MM4_cntl2; 

 // Set the register
 pMM4_CNTL2 = (P_MM4_CNTL2)((VUINT32_T) &pContext->pMMC4Reg->mm4_cntl2);
 MM4_cntl2.mm4_cntl2_value = *(VUINT_T *)pMM4_CNTL2;    
 MM4_cntl2.mm4_cntl2_bits.datswrst = 1; 	

 // Write Back
 *(VUINT_T *)pMM4_CNTL2 = MM4_cntl2.mm4_cntl2_value;    
 return;
}

/******************************************************************************
  Description:
    This function will induce a full software reset of all MMC4 components except
	MM4_CAPX
  Input Parameters:
    pContext
      Pointer to MMC context structure
  Output Parameters:
    None
  Returns:
    None  
*******************************************************************************/
void MMC4FullSWReset(P_MM4_SDMMC_CONTEXT_T pContext)
{
 P_MM4_CNTL2 pMM4_CNTL2; 
 MM4_CNTL2_UNION MM4_cntl2; 

 // Set the register
 pMM4_CNTL2 = (P_MM4_CNTL2)((VUINT32_T) &pContext->pMMC4Reg->mm4_cntl2);
 MM4_cntl2.mm4_cntl2_value = *(VUINT_T *)pMM4_CNTL2;    
 MM4_cntl2.mm4_cntl2_bits.mswrst = 1; 	

 // Write Back
 *(VUINT_T *)pMM4_CNTL2 = MM4_cntl2.mm4_cntl2_value;    
 return;
}

/******************************************************************************
  Description:
    This function will induce a software reset of all MMC4 data lines
  Input Parameters:
    pContext
      Pointer to MMC context structure
  Output Parameters:
    None
  Returns:
    None
*******************************************************************************/
void MMC4CMDSWReset(P_MM4_SDMMC_CONTEXT_T pContext)
{
 P_MM4_CNTL2 pMM4_CNTL2;
 MM4_CNTL2_UNION MM4_cntl2;

 // Set the register
 pMM4_CNTL2 = (P_MM4_CNTL2)((VUINT32_T) &pContext->pMMC4Reg->mm4_cntl2);
 MM4_cntl2.mm4_cntl2_value = *(VUINT_T *)pMM4_CNTL2;
 MM4_cntl2.mm4_cntl2_bits.cmdswrst = 1;

 // Write Back
 *(VUINT_T *)pMM4_CNTL2 = MM4_cntl2.mm4_cntl2_value;
 return;
}



/******************************************************************************
  Description:
    Set up the registers of the controller to start the transaction to 
    communicate to the card for data related command.  The commands are clearly defined in the MMC 
    specification.
  Input Parameters:
    pContext
      	Pointer to MMC context structure
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
void MMC4SendDataCommand(P_MM4_SDMMC_CONTEXT_T pContext, 
                  UINT_T Cmd,
                  UINT_T  Argument, 
                  UINT_T BlockType,
                  UINT_T DataDirection,
                  UINT_T ResType)
{
 MM4_CMD_XFRMD_UNION xfrmd;
 P_MM4_STATE pMM4_STATE;

 // Make sure the controller is ready to accept the next command
 pMM4_STATE = (P_MM4_STATE) &pContext->pMMC4Reg->mm4_state;
 while (pMM4_STATE->dcmdinhbt) 
 	{;}	// Wait. 
 
 // Set the Argument Field
 pContext->pMMC4Reg->mm4_arg = Argument;	
#if 0
 // Set the Data Transfer Command fields.	
 xfrmd.mm4_cmd_xfrmd_value = 0;
 xfrmd.mm4_cmd_xfrmd_bits.cmd_idx = Cmd;
 xfrmd.mm4_cmd_xfrmd_bits.cmd_type = MM4_CMD_TYPE_NORMAL;
 xfrmd.mm4_cmd_xfrmd_bits.dpsel = MM4_CMD_DATA;
 xfrmd.mm4_cmd_xfrmd_bits.idxchken = TRUE;
 xfrmd.mm4_cmd_xfrmd_bits.crcchken = TRUE;
 xfrmd.mm4_cmd_xfrmd_bits.res_type = ResType;
 xfrmd.mm4_cmd_xfrmd_bits.ms_blksel = BlockType;
 xfrmd.mm4_cmd_xfrmd_bits.dxfrdir = DataDirection;
 //xfrmd.mm4_cmd_xfrmd_bits.autocmd12 = TRUE;
 //xfrmd.mm4_cmd_xfrmd_bits.blkcbten = TRUE;
 xfrmd.mm4_cmd_xfrmd_bits.autocmd12 = FALSE;
 xfrmd.mm4_cmd_xfrmd_bits.blkcbten  = FALSE;
#endif

	 // Set the Data Transfer Command fields.
	 xfrmd.mm4_cmd_xfrmd_value = 0;
	 xfrmd.mm4_cmd_xfrmd_bits.cmd_idx = Cmd;
	 xfrmd.mm4_cmd_xfrmd_bits.cmd_type = MM4_CMD_TYPE_NORMAL;
	 xfrmd.mm4_cmd_xfrmd_bits.dpsel = MM4_CMD_DATA;
	 xfrmd.mm4_cmd_xfrmd_bits.idxchken = TRUE;
	 xfrmd.mm4_cmd_xfrmd_bits.crcchken = TRUE;
	 xfrmd.mm4_cmd_xfrmd_bits.res_type = ResType;
	 xfrmd.mm4_cmd_xfrmd_bits.ms_blksel = BlockType;
	 xfrmd.mm4_cmd_xfrmd_bits.dxfrdir = DataDirection;
	 xfrmd.mm4_cmd_xfrmd_bits.autocmd12 = FALSE;
	 xfrmd.mm4_cmd_xfrmd_bits.blkcbten = TRUE;

#if SDIO_DMA
	 // enable DMA
	 xfrmd.mm4_cmd_xfrmd_bits.dma_en = 1;
#if 1
	 if (Cmd==25 || Cmd==18 )
	 {  // multiple blocks with Auto STOP command
	 	xfrmd.mm4_cmd_xfrmd_bits.autocmd12 = TRUE;
	// 	xfrmd.mm4_cmd_xfrmd_bits.blkcbten = TRUE;
	 	
	 	// Stop at Block Gap request; set bit 0 of 2Ah
	 	//MMC4StopAtBlockGap(pContext);
	 }
#endif	 

#endif
	 // Kick off the command
	 pContext->pMMC4Reg->mm4_cmd_xfrmd = xfrmd.mm4_cmd_xfrmd_value;
	 return;
}

/******************************************************************************
  Description:
    Set up the registers of the controller to start the transaction to
    communicate to the card for data related command.  The commands are clearly defined in the MMC
    specification.
  Input Parameters:
    pContext
      	Pointer to MMC context structure
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
void MMC4SendDataCommandNoAuto12(P_MM4_SDMMC_CONTEXT_T pContext,
                  UINT_T Cmd,
                  UINT_T  Argument,
                  UINT_T BlockType,
                  UINT_T DataDirection,
                  UINT_T ResType)
{
	 MM4_CMD_XFRMD_UNION xfrmd;
	 P_MM4_STATE pMM4_STATE;

	 // Make sure the controller is ready to accept the next command
	 pMM4_STATE = (P_MM4_STATE) &pContext->pMMC4Reg->mm4_state;
	 while (pMM4_STATE->dcmdinhbt)
	 	{;}	// Wait.

	 // Set the Argument Field
	 pContext->pMMC4Reg->mm4_arg = Argument;

	 // Set the Data Transfer Command fields.
	 xfrmd.mm4_cmd_xfrmd_value = 0;
	 xfrmd.mm4_cmd_xfrmd_bits.cmd_idx = Cmd;
	 xfrmd.mm4_cmd_xfrmd_bits.cmd_type = MM4_CMD_TYPE_NORMAL;
	 xfrmd.mm4_cmd_xfrmd_bits.dpsel = MM4_CMD_DATA;
	 // xfrmd.mm4_cmd_xfrmd_bits.idxchken = TRUE;
	 xfrmd.mm4_cmd_xfrmd_bits.crcchken = TRUE;
	 xfrmd.mm4_cmd_xfrmd_bits.res_type = ResType;
	 xfrmd.mm4_cmd_xfrmd_bits.ms_blksel = BlockType;
	 xfrmd.mm4_cmd_xfrmd_bits.dxfrdir = DataDirection;
	 xfrmd.mm4_cmd_xfrmd_bits.autocmd12 = FALSE;
	 xfrmd.mm4_cmd_xfrmd_bits.blkcbten = TRUE;

#if SDIO_DMA
		xfrmd.mm4_cmd_xfrmd_bits.dma_en = TRUE;
#endif

	 printf("\n Sending command: xfrmd = 0x%x ",xfrmd.mm4_cmd_xfrmd_value);

	 // Kick off the command
	 pContext->pMMC4Reg->mm4_cmd_xfrmd = xfrmd.mm4_cmd_xfrmd_value;
 return;
}


/******************************************************************************
  Description:
    Set up the registers of the controller to start the transaction to 
    communicate to the card for setup related commands.  
    The commands are clearly defined in the MMC specification.
  Input Parameters:
    pContext
      	Pointer to MMC context structure
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
void MMC4SendSetupCommand(P_MM4_SDMMC_CONTEXT_T pContext,
                  UINT_T Cmd,
                  UINT_T CmdType,
                  UINT_T Argument,
                  UINT_T ResType)
{
	 MM4_CMD_XFRMD_UNION xfrmd;
	 P_MM4_STATE pMM4_STATE;

	 // Make sure the controller is ready to accept the next command
	 pMM4_STATE = (P_MM4_STATE) &pContext->pMMC4Reg->mm4_state;
	 while (pMM4_STATE->ccmdinhbt) 
	 	{;}	// Wait. 

	 // Set the Argument Field
	 pContext->pMMC4Reg->mm4_arg = Argument;	

	 // Set the Data Transfer Command fields.	
	 xfrmd.mm4_cmd_xfrmd_value = 0;
	 xfrmd.mm4_cmd_xfrmd_bits.cmd_idx = Cmd;
	 xfrmd.mm4_cmd_xfrmd_bits.cmd_type = CmdType;
	 xfrmd.mm4_cmd_xfrmd_bits.idxchken = TRUE;
	 xfrmd.mm4_cmd_xfrmd_bits.crcchken = TRUE;
	 xfrmd.mm4_cmd_xfrmd_bits.res_type = ResType;

	 printf("\n Sending command: xfrmd = 0x%x ",xfrmd.mm4_cmd_xfrmd_value);

	 // Kick off the command
	 pContext->pMMC4Reg->mm4_cmd_xfrmd = xfrmd.mm4_cmd_xfrmd_value;
	 return;
	 
}
