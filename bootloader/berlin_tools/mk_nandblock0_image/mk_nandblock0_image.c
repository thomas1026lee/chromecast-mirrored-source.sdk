#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "../Common/include/nand_block0_layout.h"
#include "nand_timing_reg.h"
#include "../Common/include/Firmware_Include_Z1/MctrlDual.h" 
#include "ddr_timing.h"
#include "nand_param.h"

#define	MAGIC_NUMBER		0xD2ADA3F1

#define	Nand_Attr		{\
	unsigned int 	page_size		:	8;\
	unsigned int 	address_cycle	:	8;\
	unsigned int 	block_size		:	16;\
}

typedef	union T32NAND_Attr_Config{
	unsigned int	u32 ; 
	struct	Nand_Attr ; 
}T32NAND_Attr_Config ; 

#define	Nand_ECC_Attr	{\
	unsigned int	ecc_en			:	8;\
	unsigned int	bch_en			:	8;\
	unsigned int	spare_en		:	8;\
	unsigned int	reserve			:	8;\
}

typedef	union	T32NAND_ECC_Config{
	unsigned int	u32	; 
	struct	Nand_ECC_Attr ; 
}T32NAND_ECC_Config ;

int main(int argc, char * argv[])
{
	unsigned char * image_name = argv[1] ;
	int page_size = NAND_PAGE_SIZE ; 
	int block_size = NAND_BLOCK_SIZE ; 
	int address_cycles = NAND_ADDRESS_CYCLE ; 
	int nand_spare_enable = NAND_SPARE_EN ; 
	int nand_ecc_enable = NAND_ECC_EN ; 
	int nand_bch_enable = NAND_BCH_EN ; 
	block0_t block0;
	bootparam_t bootp;
	int *buff = (int*)(&bootp);
	int i ; 
	FILE	* file ; 
	T32Gbl_NAND_Timing_Reg0*		nand_timing0 = (T32Gbl_NAND_Timing_Reg0*)(&block0.ndtr0cs0); 
	T32Gbl_NAND_Timing_Reg1*		nand_timing1 = (T32Gbl_NAND_Timing_Reg1*)(&block0.ndtr1cs0); 
	TMctrlDual_Timing1*			ddr_timing1 = (TMctrlDual_Timing1*)(&block0.ddr_timing1) ; 
	TMctrlDual_Timing2*			ddr_timing2 = (TMctrlDual_Timing2*)(&block0.ddr_timing2) ; 
	TMctrlDual_Timing3*			ddr_timing3	= (TMctrlDual_Timing3*)(&block0.ddr_timing3) ;


//	T32NAND_Attr_Config nand_flash_attr ; 
//	T32NAND_ECC_Config nand_ecc_attr ; 

	unsigned int * block0_buff = NULL ; 

	memset(&block0, 0, sizeof(block0));

	block0_buff = (unsigned int *)malloc(block_size*sizeof(unsigned char)) ; 
	if(NULL == block0_buff){
		printf("malloc block0_buff error\n") ; 
		return 1 ; 
	}
	bootp.magic = MAGIC_NUMBER;
	if(page_size == 512){
		bootp.page_size = 0 ; 
	}else if(page_size == 2048){
		bootp.page_size = 1 ; 
	}else if(page_size == 4096){
		bootp.page_size = 2 ; 
	}else if(page_size == 8192){
		bootp.page_size = 3 ; 
	}else{
		printf("Invalid page size\n") ; 
		return -1 ; 
	}

	bootp.address_cycle = address_cycles ; 
	if(block_size%1024){
		printf("Invalid block size\n") ;
		return -1 ; 
	}
	bootp.block_size = block_size/1024 ; 

	bootp.ecc_en = nand_ecc_enable ; 
	bootp.bch_en = nand_bch_enable ; 
	bootp.spare_en = nand_spare_enable ; 
	bootp.reserve = 0 ; 

	nand_timing0->u32 = NDTR0CS0;
	nand_timing1->u32 = NDTR1CS0;
	printf("nand_timing0 value = 0x%x\n", nand_timing0->u32) ; 
	printf("nand_timing1 value = 0x%x\n", nand_timing1->u32) ; 
	
	block0.ddr_CSL = MCTRLDUAL_COMMCONFIG_CL_SEL ; 
	printf("block0.ddr_CSL = %d\n" , block0.ddr_CSL) ; 
	ddr_timing1->uTiming1_init_tras = MCTRLDUAL_TIMING_REG1_INIT_TRAS; 
	ddr_timing1->uTiming1_init_trfc = MCTRLDUAL_TIMING_REG1_INIT_TRFC; 
	ddr_timing1->uTiming1_init_trc = MCTRLDUAL_TIMING_REG1_INIT_TRC; 
	ddr_timing1->uTiming1_init_refc = MCTRLDUAL_TIMING_REG1_INIT_REFC; 
	printf("ddr_timing1 reg = 0x%x\n" , ddr_timing1->u32[0]) ; 

	ddr_timing2->uTiming2_init_trp = MCTRLDUAL_TIMING_REG2_INIT_TRP; 
	ddr_timing2->uTiming2_init_trrd = MCTRLDUAL_TIMING_REG2_INIT_TRRD ; 
	ddr_timing2->uTiming2_init_trcd = MCTRLDUAL_TIMING_REG2_INIT_TRCD ; 
	ddr_timing2->uTiming2_init_twr = MCTRLDUAL_TIMING_REG2_INIT_TWR ; 
	ddr_timing2->uTiming2_init_trtp = MCTRLDUAL_TIMING_REG2_INIT_TRTP ; 
	ddr_timing2->uTiming2_init_twtr = MCTRLDUAL_TIMING_REG2_INIT_TWTR ; 
	ddr_timing2->uTiming2_init_tmrd = MCTRLDUAL_TIMING_REG2_INIT_TMRD ; 
	printf("ddr_timing2 reg = 0x%x\n", ddr_timing2->u32[0]) ; 

	ddr_timing3->uTiming3_init_txsnr = MCTRLDUAL_TIMING_REG3_INIT_TXSNR ; 
	ddr_timing3->uTiming3_init_tick = MCTRLDUAL_TIMING_REG3_INIT_TICK ; 
	ddr_timing3->uTiming3_init_tfaw = MCTRLDUAL_TIMING_REG3_INIT_TFAW ; 
	ddr_timing3->uTiming3_init_trfcmax = MCTRLDUAL_TIMING_REG3_INIT_TRFCMAX ;
	printf("ddr_timing3 reg = 0x%x\n", ddr_timing3->u32[0]) ;

	block0.first_burn_flag = 1;
	block0.ddr_type = DDR_TYPE;
	block0.ddr_channel = DDR_CHANNEL;
	strncpy(&block0.cpu_type, CPU_TYPE, 2);
	printf("ddr type: %u, ddr channel: %u, cpu type: %c%c\n",
		block0.ddr_type, block0.ddr_channel, block0.cpu_type[0], block0.cpu_type[1]);

	memset(block0_buff, 0, block_size) ; 
	for(i=0; i<33; i++){
		memcpy(&block0.bootparam[i], &bootp, sizeof(bootp));
	}
	printf("sizeof bootp= %d, magic = 0x%08x, flash attribute= 0x%08x, ecc atribute = 0x%08x\n", \
			sizeof(bootp), buff[0], buff[1], buff[2]) ; 

	file = fopen(argv[1], "w") ; 
	if(file == NULL){
		printf("open file %s error\n", argv[1]) ; 
		return -2 ; 
	}
	fwrite(&block0, sizeof(block0) , 1, file) ; 
	fseek(file, block_size-1, SEEK_SET);
	i=0;
	fwrite(&i, sizeof(char) , 1, file) ; 
	fclose(file) ; 
	return 0;
}
