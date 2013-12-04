#ifndef __SDIO_CONTROLLER_H
#define __SDIO_CONTROLLER_H

// DMA or PIO mode 
#define SDIO_DMA	1
#define	MULTI_SDIO	1
#define __SDIO_INT_MODE___

typedef volatile unsigned long 	VUINT32_T;
typedef unsigned long 		   	UINT32_T, ULONG;
typedef volatile unsigned int  	VUINT_T;
typedef unsigned int 		   	UINT_T;
typedef int			 		   	INT_T;

//typedef unsigned short 		   	UINT16_T, USHORT;
//typedef volatile unsigned short VUINT16_T;
//typedef unsigned char 			UINT8_T;
//typedef char		 			INT8_T;

#define TRUE	1
#define FALSE   0

// MMC controller registers definition
typedef struct 
{
  VUINT32_T mm4_sysaddr;                  		// DMA system address				 		0x0
  VUINT32_T mm4_blk_cntl;                       // Block size control register		 		0x4
  VUINT32_T mm4_arg;                      		// Command argument					 		0x8
  VUINT32_T mm4_cmd_xfrmd;                      // Command and transfer mode		 		0xC
  VUINT32_T mm4_resp0;                          // cmd response 0					 		0x10
  VUINT32_T mm4_resp1;                          // cmd response 1					 		0x14
  VUINT32_T mm4_resp2;                          // cmd response 2					 		0x18
  VUINT32_T mm4_resp3;                          // cmd response 3					 		0x1C
  VUINT32_T mm4_dp;                             // buffer data port					 		0x20
  VUINT32_T mm4_state;                          // mm4 state						 		0x24
  VUINT32_T mm4_cntl1;                          // host control 1					 		0x28
  VUINT32_T mm4_cntl2;                          // host control 2					 		0x2C
  VUINT32_T mm4_i_stat;                         // status of current command 		 		0x30
  VUINT32_T mm4_i_stat_en;                      // interrupt status enable			 		0x34
  VUINT32_T mm4_i_sig_en;                       // interrupt signal enable			 		0x38
  VUINT32_T mm4_acmd12_er;                      // auto cmd12 error status			 		0x3C
  VUINT32_T mm4_cap1_2;                         // capabilities 1,2					 		0x40
  VUINT32_T mm4_cap3_4;                         // capabilities 3,4							0x44
  VUINT32_T mm4_sd_max_current1_2;              // max current capabilities					0x48
  VUINT32_T mm4_sd_max_current3_4;              // max current capabilities					0x4C
  VUINT32_T mm4_force_event;              		// force event for AutoCMD12 Error Status	0x50
  VUINT32_T mm4_adma_err_stat;                  // ADMA Error Status						0x54
  VUINT32_T mm4_adma_system_address[2];      	// ADMA Address 63:0						0x58
  VUINT32_T mm4_preset_value_for_init_ds; 		//											0x60
  VUINT32_T mm4_preset_value_for_hs_sdr12;		//											0x64  
  VUINT32_T mm4_preset_value_for_hs_sdr25_50;	//											0x68  
  VUINT32_T mm4_preset_value_for_hs_sdr104_50;	//											0x6C  
  VUINT32_T reserved_X1[28];                    // reserved fields							0x70
  VUINT32_T mm4_shared_bus_control;        		// Shared Bus Control Register				0xE0
  VUINT32_T reserved_X2[6];                   	// reserved									0xE4
  VUINT32_T mm4_sd_slot_int_stat_ctrl_ver;      // SD Interrupt Line and Version Support	0xFC
  VUINT32_T mm4_sd_cfg_fifo_param;				// SD Extra Parameters register				0x100
  VUINT32_T mm4_sd_fifo_param;					// SD_FIFO_PARAM register					0x104
  VUINT32_T mm4_sd_spi_mode_clk_brst_size;		// SD_SIP_MODE register						0x108
  VUINT32_T mm4_sd_ce_ata_1_2;					// SD_CE_ATA_1 and SD_CE_ATA_2				0x10C
  VUINT32_T mm4_sd_pad_io_setup;				// MM4_SD_PAD_IO_SETUP						0x110  	
  VUINT32_T mm4_rx_cfg ;						// RX_CFG_REG								0x114
  VUINT32_T mm4_tx_cfg ;						// TX_CFG_REG								0x118
} MM4_SDMMC_T, *P_MM4_SDMMC_T;

/*************************** Register Masks ********************************/
/**************************************************************************/
// ******************** MM4_BLK_CNTL **********************************
typedef struct
{
 unsigned int xfr_blksz       : 12;       // Transfer Block Size
 unsigned int dma_bufsz       : 4;        // Host DMA buffer size
 unsigned int blk_cnt         : 16;       // Block Count for Current Transfer
} MM4_BLK_CNTL, *P_MM4_BLK_CNTL;

#define MM4_512_HOST_DMA_BDRY  0x7


// ********************* MM4_CMD_XFRMD ********************************
//	3/8: Xiufeng update according sdh_register.xls
typedef struct
{

unsigned int dma_en 		 : 1;		 // DMA enable						   0
unsigned int blkcbten		 : 1;		 // Block Count Enable				   1
unsigned int autocmd12		 : 2;		 // AutoCMD12						   2
//	xiufeng	From APSE's code there is one reserved1 bit here because 
//	autocmd12 is 1 bit
//	unsigned int reserved1		 : 1;		 // 								   3
unsigned int dxfrdir		 : 1;		 // Data Transfer Direction Select	   4
unsigned int ms_blksel		 : 1;		 // Multi Block Select				   5
unsigned int reserved2		 : 10;		  //								   6
unsigned int res_type		 : 2;		 // Response Type					   16
unsigned int reserved3		 : 1;		 // 								   18
unsigned int crcchken		 : 1;		 // CRC check enable				   19
unsigned int idxchken		 : 1;		 // Command Index Check Enable		   20
unsigned int dpsel			 : 1;		 // Data present   select			   	   21
unsigned int cmd_type		 : 2;		 // Cmd Type						   22
unsigned int cmd_idx		 : 6;		 // Cmd Index						   24
unsigned int reserved4		 : 2;		 // 								   30

} MM4_CMD_XFRMD, *P_MM4_CMD_XFRMD;

typedef union
{
	MM4_CMD_XFRMD mm4_cmd_xfrmd_bits;
	unsigned int		  mm4_cmd_xfrmd_value;
} MM4_CMD_XFRMD_UNION, *P_MM4_CMD_XFRMD_UNION; 

#define NO_ARGUMENT					0xffffffff
#define MM4_CMD_TYPE_NORMAL 		0
#define MM4_CMD_TYPE_SUSPEND  		1
#define MM4_CMD_TYPE_RESUME 		2
#define MM4_CMD_TYPE_ABORT 			3
#define MM4_CMD_DATA 				1
#define MM4_CMD_NODATA				0
#define MM4_NO_RES					0
#define MM4_136_RES					1
#define MM4_48_RES					2
#define MM4_48_RES_WITH_BUSY		3
#define MM4_MULTI_BLOCK_TRAN		1
#define MM4_SINGLE_BLOCK_TRAN		0
#define MM4_HOST_TO_CARD_DATA		0
#define MM4_CARD_TO_HOST_DATA		1


//	Add by xiufeng

// this information will be included in the response type argument of relevant apis.
// it will occupy bits 15:8 of the RespType parameter.
#define MM4_RT_MASK					0x7f00
#define MM4_RT_NONE					0x0000
#define	MM4_RT_R1					0x0100
#define	MM4_RT_R2					0x0200
#define	MM4_RT_R3					0x0300
#define	MM4_RT_R4					0x0400
#define	MM4_RT_R5					0x0500
#define	MM4_RT_R6					0x0600
#define	MM4_RT_R7					0x0700		// sd card interface condition

#define MM4_RT_BUSYMASK				0x8000
#define MM4_RT_BUSY					0x8000


// ********************* MM4_STATE ********************************
typedef struct
{

	unsigned int ccmdinhbt	   : 1;
	unsigned int dcmdinhbt	   : 1;
	unsigned int datactv	   : 1;
	unsigned int retuning_req  : 1;
	unsigned int reserved0	   : 4;
	unsigned int wractv 	   : 1;
	unsigned int rdactv 	   : 1;
	unsigned int bufwren	   : 1;
	unsigned int bufrden	   : 1;
	unsigned int reserved1	   : 4;
	unsigned int cdinstd	   : 1;
	unsigned int cdstbl 	   : 1;
	unsigned int cddetlvl	   : 1;
	unsigned int wpswlvl	   : 1;
	unsigned int lwrdatlvl	   : 4;
	unsigned int cmdlvl 	   : 1;
	//	3/12: Xiufeng
	//	From APSE's code, there is one param uprdatlvl which is 4 bits. our controller
	// 	does not have this parameter and reservers the following 7 bits
//	unsigned int uprdatlvl	   : 4;
	unsigned int reserved2	   : 7;

} MM4_STATE, *P_MM4_STATE;

typedef union
{
	MM4_STATE     mm4_state_bits;
	unsigned int  mm4_state_value;
} MM4_STATE_UNION, *P_MM4_STATE_UNION; 

// ********************* MM4_CNTL1 ********************************
typedef struct
{
	unsigned int ledcntl	   : 1; 	   // 0
	unsigned int datawidth	   : 1; 	   // 1
	unsigned int hispeed	   : 1; 	   // 2
	unsigned int dma_sel	   : 2; 	   // 3
	unsigned int ex_data_width : 1; 	   // 5
	unsigned int card_det_l    : 1; 	   // 6
	unsigned int card_det_s    : 1; 	   // 7
	unsigned int buspwr 	   : 1; 	   // 8
	unsigned int vltgsel	   : 3; 	   // 9
	unsigned int reserved2	   : 4; 	   // 12
	unsigned int bgreqstp	   : 1; 	   // 16
	unsigned int contreq	   : 1;		   // 17
	unsigned int rdwcntl	   : 1;		   // 18
	unsigned int bgirqen	   : 1;        // 19
	unsigned int reserved3	   : 4;        
	unsigned int card_int_wake : 1;        // 24
	unsigned int card_ins_wake : 1;		   // 25
	unsigned int card_rem_wake : 1;		   // 26
	unsigned int reserved4	   : 5;

} MM4_CNTL1, *P_MM4_CNTL1;

typedef union
{
	MM4_CNTL1     mm4_cntl1_bits;
	unsigned int		  mm4_cntl1_value;
} MM4_CNTL1_UNION, *P_MM4_CNTL1_UNION; 

#define MM4_VLTGSEL_1_8		0x5
#define MM4_VLTGSEL_3_0		0x6
#define MM4_VLTGSEL_3_3		0x7

// ********************* MM4_CNTL2 ********************************
typedef struct
{
	unsigned int inter_clk_en	   : 1;    // Internal Clock Enable
	unsigned int inter_clk_stable  : 1;    // Internal Clock Stable
	unsigned int mm4clken		   : 1;    // Clock Enable
	unsigned int reserved1		   : 2;    // bits 3,4
	unsigned int clk_gen_sel	   : 1;    // bit 5
	unsigned int sd_freq_sel_hi    : 2;    // 6 								
	unsigned int sd_freq_sel_lo    : 8;    // 8 								 
	unsigned int dtocntr		   : 4;    // bit 0 Data Timeout Counter Value
	unsigned int reserved2		   : 4;    //
	unsigned int mswrst 		   : 1;    // bit 8 Software Reset for All
	unsigned int cmdswrst		   : 1;    // bit 9 Software Reset for MM4CMD Line
	unsigned int datswrst		   : 1;    // bit 10 Software Reset for MM4DATx Lines
	unsigned int reserved3		   : 5;    // bits 11-15

} MM4_CNTL2, *P_MM4_CNTL2;

#define MM4CLKEN_EN_CLOCK    1

#define MM4CLOCK200KHZRATE	0x1FF	// Set also additional SD_FREQ_HI bit.	So SD_FREQ_SEL = 0x1FF = 511 * 2 = 1022 (clock divider)
#define MM4CLOCK50MHZRATE	2
#define MM4CLOCK25MHZRATE	4
#define MM4CLOCK13MHZRATE	8
#define MM4CLOCK6MHZRATE	16

#define MM4CLOCK187KHZRATE	0x80	
#define MM4CLOCK48MHZRATE	0x0
#define CLOCK_27_MULT		0xE

#define	SDCLK_SEL_DIV_1024	0x200		
#define	SDCLK_SEL_DIV_512	0x100		
#define SDCLK_SEL_DIV_256	0x80
#define SDCLK_SEL_DIV_128	0x40
#define SDCLK_SEL_DIV_64	0x20
#define SDCLK_SEL_DIV_32	0x10
#define SDCLK_SEL_DIV_16	0x08
#define SDCLK_SEL_DIV_8		0x04
#define SDCLK_SEL_DIV_4		0x02
#define SDCLK_SEL_DIV_2		0x01
#define SDCLK_SEL_DIV_1		0x00

#define SDBUS_4BIT			0x01
typedef union
{
	MM4_CNTL2     mm4_cntl2_bits;
	unsigned int		  mm4_cntl2_value;
} MM4_CNTL2_UNION, *P_MM4_CNTL2_UNION; 

// ********************* MM4_I_STAT, MM4_I_STAT_EN, MM4_I_SIGN_EN ************
typedef struct
{
	unsigned int cmdcomp	   : 1;   //0
	unsigned int xfrcomp	   : 1;   //1
	unsigned int bgevnt 	   : 1;   //2
	unsigned int dmaint 	   : 1;   //3
	unsigned int bufwrrdy	   : 1;   //4
	unsigned int bufrdrdy	   : 1;   //5
	unsigned int cdins		   : 1;   //6
	unsigned int cdrem		   : 1;   //7
	unsigned int cdint		   : 1;   //8
	unsigned int int_a		   : 1;   //9
	unsigned int int_b		   : 1;   //10
	unsigned int int_c		   : 1;   //11
	unsigned int retuninig_int : 1;   //12
	unsigned int reserved0	   : 2;   //13
	unsigned int errint 	   : 1;   //15
	unsigned int ctoerr 	   : 1;   //16
	unsigned int ccrcerr	   : 1;   //17
	unsigned int cenderr	   : 1;   //18
	unsigned int cidxerr	   : 1;   //19
	unsigned int dtoerr 	   : 1;   //20
	unsigned int dcrcerr	   : 1;   //21
	unsigned int denderr	   : 1;   //22
	unsigned int ilmterr	   : 1;   //23
	unsigned int ac12err	   : 1;   //24
	unsigned int admaerr	   : 1;   //25
	unsigned int tune_err	   : 1;   //26
	unsigned int reserved1	   : 1;   //27
	unsigned int spierr 	   : 1;   //28
	unsigned int axi_resp_err  : 1;   //29
	unsigned int cpl_tout_err  : 1;   //30
	unsigned int crc_stat_err  : 1;   //31

} MM4_I_STAT, *P_MM4_I_STAT, MM4_I_STAT_EN, *P_MM4_I_STAT_EN, MM4_I_SIGN_EN, *P_MM4_I_SIGN_EN;

#define DISABLE_INTS 	0
#define ENABLE_INTS		1

typedef union
{
	MM4_I_STAT 	  mm4_i_stat_bits;
	unsigned int		  mm4_i_stat_value;
} MM4_I_STAT_UNION, *P_MM4_I_STAT_UNION; 

#define CLEAR_INTS_MASK		0xFFFF7EFF


// ********************* MM4_ACMD12_ER *******************************************
typedef struct
{
	unsigned int ac12nexe		   : 1;   // 0
	unsigned int ac12toer		   : 1;   // 1
	unsigned int ac12crcer		   : 1;   // 2
	unsigned int ac12ender		   : 1;   // 3
	unsigned int ac12idxer		   : 1;   // 4
	unsigned int reserved0		   : 2;   // 5
	unsigned int cmdnisud		   : 1;   // 7
	unsigned int reserved1		   : 8;   // 8
	unsigned int uhs_mode_sel	   : 3;   // 16    
	unsigned int sgh_v18_en 	   : 1;   // 19
	unsigned int drv_strength_sel  : 2;   // 20 
	unsigned int exe_tuning 	   : 1;   // 22
	unsigned int sampling_clk_sel  : 1;   // 23
	unsigned int reserved2		   : 6;   // 24
	unsigned int async_int_en	   : 1;   // 30
	unsigned int pre_val_en 	   : 1;   // 31    

} MM4_ACMD12_ER, *P_MM4_ACMD12_ER;

// ********************* MM4_CAP0 *******************************************
typedef struct
{
 unsigned int toclkfreq				: 6;
 unsigned int reserved0				: 1;
 unsigned int toclkunit				: 1;
 unsigned int bsclkfreq				: 8;
 //	3/12 Xiufeng
 //	the following is not same with ASPE's code
 unsigned int max_blk_len			: 2;
 unsigned int ex_data_width_support	: 1; 
 //	3/12 Xiufeng
 //	no reserved bit here
 //unsigned int reserved1				: 1;
 unsigned int adma2_support			: 1;
 unsigned int adma1_support			: 1;
 unsigned int hi_speed_support		: 1;
 unsigned int sdma_support			: 1;
 unsigned int sus_res_support		: 1;
 unsigned int vlg_33_support		: 1;
 unsigned int vlg_30_support		: 1;
 unsigned int vlg_18_support		: 1;
 unsigned int reserved2				: 1;
 unsigned int sys_bus_64_support	: 1;
 unsigned int async_int_support		: 1;
 //	3/12 Xiufeng
 //	cfg_slot_type is 2 bits now
 unsigned int cfg_slot_type			: 1;
} MM4_CAP1_2, *P_MM4_CAP1_2;

typedef union
{
	MM4_CAP1_2 	  mm4_cap1_2_bits;
	UINT_T		  mm4_cap1_2_value;
} MM4_CAP1_2_UNION, *P_MM4_CAP1_2_UNION; 



// ********************* MM4_SD_MAX_CURRENT1_2 *******************************************
typedef struct
{
 unsigned int v3_3vmaxi		: 8;
 unsigned int v3_0vmaxi		: 8;
 unsigned int v1_8vmaxi		: 8;
 unsigned int reserved0 	: 8;
} MM4_SD_MAX_CURRENT1_2, *P_MM4_SD_MAX_CURRENT1_2;

typedef union
{
	MM4_SD_MAX_CURRENT1_2 	  	mm4_sd_max_current1_2_bits;
	UINT_T		  				mm4_sd_max_current1_2_value;
} MM4_SD_MAX_CURRENT1_2_UNION, *P_MM4_SD_MAX_CURRENT1_2_UNION; 

// ********************* MM4_SD_CFG_FIFO_PARAM *******************************************

typedef struct
{
 unsigned int reserved0				: 3;	// Bits 0-2
 unsigned int boot_ack				: 1; 	// Bit 3
 unsigned int squ_empty_check		: 1;	// Bit 4 
 unsigned int squ_full_check		: 1;	// Bit 5
 unsigned int gen_pad_clk_on		: 1;	// Bit 6
 unsigned int reserved2				: 17;	// Bits 7-23
 unsigned int gen_pad_clk_cnt		: 8;	// Bits	24-31
} MM4_SD_CFG_FIFO_PARAM, *P_MM4_SD_CFG_FIFO_PARAM;

typedef union
{
	MM4_SD_CFG_FIFO_PARAM 	MM4_SD_CFG_FIFO_PARAM_bits;
	UINT_T		  		MM4_SD_CFG_FIFO_PARAM_value;
} MM4_SD_CFG_FIFO_PARAM_UNION, *P_MM4_SD_CFG_FIFO_PARAM_UNION; 


// ********************* MM4_SD_FIFO_PARAM *******************************************

typedef struct
{
 unsigned int rtc 					: 2;	// Bits 1:0
 unsigned int wtc					: 2; 	// Bit  3:2
 unsigned int fifo_clock			: 1;	// Bit 4 
 unsigned int fifo_cs				: 1;	// Bit 5
 unsigned int power_down			: 1;	// Bit 6
 unsigned int reserver0				: 1; 	// Bit 7
 unsigned int clk_gate_ctl			: 1;	// bit 8
 unsigned int clk_gate_on			: 1;	// bit 9
 unsigned int reserver1				: 1;	// bit 10
 unsigned int ovrrd_clk_oen			: 1; 	// bit 11
 unsigned int force_clk_on			: 1;	// bit 12
 unsigned int reserved2				: 19;	// Bits 13:31
} MM4_SD_FIFO_PARAM, *P_MM4_SD_FIFO_PARAM;

typedef union
{
	MM4_SD_FIFO_PARAM 	MM4_SD_FIFO_PARAM_bits;
	UINT_T		  		MM4_SD_FIFO_PARAM_value;
} MM4_SD_FIFO_PARAM_UNION, *P_MM4_SD_FIFO_PARAM_UNION; 


// ********************* MM4_SD_CE_ATA_1_2 *******************************************
typedef struct
{
 unsigned int cpl_timeout			: 14;	// Bits 0-13
 unsigned int reserved0				: 2;	// Bit 14,15
 unsigned int misc_int_int_en	 	: 1;	// Bit 0
 unsigned int misc_int_en	 		: 1;	// Bit 1
 unsigned int misc_int		 		: 1;	// Bit 2
 unsigned int reserved1				: 1;	// Bit 3
 unsigned int cpl_complete_int_en	: 1;	// Bit 4
 unsigned int cpl_complete_en		: 1;	// Bit 5
 unsigned int cpl_complete			: 1;	// Bit 6
 //	3/12 Xiufeng
 //	for BG2 SDIO controller, reserved 4 bits here and the left
 //	one bits is used as mms_resetn.the mmc_resetn controls the value
 // of the pin MMC_RESETN going to eMMC device.
 //unsigned int reserved2		 		: 5;	// Bit 7
 unsigned int reserved2		 		: 4;	// Bit 7
 unsigned int mmc_resetn			: 1;		// Bit 11
 unsigned int mmc_card				: 1;	// Bit 12
 unsigned int ceata_card			: 1;	// Bit 13
 unsigned int snd_cpl				: 1;	// Bit 14
 unsigned int chk_cpl				: 1;	// Bit 15
} MM4_SD_CE_ATA_1_2, *P_MM4_SD_CE_ATA_1_2;

typedef union
{
	MM4_SD_CE_ATA_1_2 	MM4_SD_CE_ATA_1_2_bits;
	UINT_T		  		MM4_SD_CE_ATA_1_2_value;
} MM4_SD_CE_ATA_1_2_UNION, *PMM4_SD_CE_ATA_1_2_UNION; 


typedef	struct
{
 unsigned int tx_hold_delay0		: 9;	// bit 8:0
 unsigned int reserved1				: 7;	// bit 15:9 
 unsigned int tx_hold_delay1		: 9;	// bit 24:16
 unsigned int reserved2				: 7;	// bit 31:25
}MM4_TX_CFG, *P_MM4_TX_CFG ;

typedef union
{
	MM4_TX_CFG		MM4_TX_CFG_bits ; 
	UINT_T			MM4_TX_CFG_value ; 
}MM4_TX_CFG_UNION, *PMM4_TX_CFG_UNION ;

/*********** End MM4 Register Def's **************************************/
// response types
enum {
  MMC_RESPONSE_NONE = 1L<<8,
  MMC_RESPONSE_R1 = 2L<<8,
  MMC_RESPONSE_R1B = 3L<<8,
  MMC_RESPONSE_R2 = 4L<<8,
  MMC_RESPONSE_R3 = 5L<<8,
  MMC_RESPONSE_R4 = 6L<<8,
  MMC_RESPONSE_R5 = 7L<<8,
  MMC_RESPONSE_R5B = 8L<<8,
  MMC_RESPONSE_R6 = 9L<<8,
  MMC_RESPONSE_R7 = 0xAL<<8,
  MMC_RESPONSE_MASK = 0x0000FF00
};

#define SD_OCR_VOLTAGE_3_3_TO_3_6 	0xE00000
#define SD_OCR_VOLTAGE_1_8_TO_3_3 	0x1F8000
#define SD_OCR_VOLTAGE_1_8		 	0x80
#define MMC_OCR_VOLTAGE_ALL			0xFF8000
#define	MM4FIFOWORDSIZE				0x80

// device context for MMC API, containing anything needed to operate on 
// this API. It is always first parameter for all APIs.
typedef struct 
{
  P_MM4_SDMMC_T            	pMMC4Reg;				// MMC1 register base
} MM4_SDMMC_CONTEXT_T, *P_MM4_SDMMC_CONTEXT_T;




#ifdef IO_CARD_SUPPORT

#define SDIO_OCR_VOLTAGE_2_0_TO_2_1		0x000100
#define SDIO_OCR_VOLTAGE_2_1_TO_2_2		0x000200
#define SDIO_OCR_VOLTAGE_2_2_TO_2_3		0x000400
#define SDIO_OCR_VOLTAGE_2_3_TO_2_4		0x000800
#define SDIO_OCR_VOLTAGE_2_4_TO_2_5		0x001000
#define SDIO_OCR_VOLTAGE_2_5_TO_2_6		0x002000
#define SDIO_OCR_VOLTAGE_2_6_TO_2_7		0x004000
#define SDIO_OCR_VOLTAGE_2_7_TO_2_8		0x008000
#define SDIO_OCR_VOLTAGE_2_8_TO_2_9		0x010000
#define SDIO_OCR_VOLTAGE_2_9_TO_3_0		0x020000
#define SDIO_OCR_VOLTAGE_3_0_TO_3_1		0x040000
#define SDIO_OCR_VOLTAGE_3_1_TO_3_2		0x080000
#define SDIO_OCR_VOLTAGE_3_2_TO_3_3		0x100000
#define SDIO_OCR_VOLTAGE_3_3_TO_3_4		0x200000
#define SDIO_OCR_VOLTAGE_3_4_TO_3_5		0x400000
#define SDIO_OCR_VOLTAGE_3_5_TO_3_6		0x800000
#define SDIO_OCR_ALL_VOLTAGES			0xFFFFFF

#endif

#define SD_OCR_VOLTAGE_3_3_TO_3_6 	0xE00000
#define SD_OCR_VOLTAGE_1_8_TO_3_3 	0x1F8000
#define SD_OCR_VOLTAGE_1_8		 	0x80
#define MMC_OCR_VOLTAGE_ALL			0xFF8000
#define	MM4FIFOWORDSIZE				0x80


// ADMA descriptor
typedef struct sdio_adma_desc
{
	unsigned int valid 		:1;
	unsigned int end		:1;
	unsigned int interrupt	:1;
	unsigned int zero		:1;
	unsigned int act1		:1;
	unsigned int act2		:1;
	unsigned int resvered	:10;
	unsigned int length		:16;
	unsigned int addr;
} SDIO_ADMA_DESC, *SDIO_ADMA_DESC_PTR;
#endif
