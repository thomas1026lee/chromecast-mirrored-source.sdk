#ifndef _BLOCK0_H_
#define _BLOCK0_H_

typedef struct _bootparam_t_ {
	unsigned int magic;

	union{
		unsigned int	nand_param;
	struct{
		unsigned int 	page_size		:	2;
		unsigned int 	address_cycle		:	2;
		unsigned int 	block_size		:	4;

		unsigned int	ecc_en			:	1;
		unsigned int	bch_en			:	1;
		unsigned int	spare_en		:	1;
		unsigned int	chunk_size		:	2;
		unsigned int	ecc_strength		:	5;
		unsigned int	reserve			:	14;
		};
	} ;
} bootparam_t;

#define DDR2	2
#define DDR3	3
#define DDR_DUAL_CHANNEL	2
#define DDR_SINGLE_CHANNEL	1

typedef struct _block0_t_ {
	union{
		bootparam_t bootparam[256];
		char pad[2048];
	} ;

	unsigned int 	ndtr0cs0; 
	unsigned int	ndtr1cs0; 
	unsigned int	ndredel; 
	unsigned int	ddr_CSL; 
	unsigned int 	ddr_timing1; 
	unsigned int	ddr_timing2; 
	unsigned int 	ddr_timing3;
	unsigned int	first_burn_flag;
	unsigned char	ddr_type	: 4;
	unsigned char	ddr_channel	: 4;
	unsigned char	cpu_type[2];
} block0_t;

#endif
