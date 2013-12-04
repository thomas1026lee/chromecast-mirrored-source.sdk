#ifndef	__NAND_TIMING_REG_H
#define	__NAND_TIMING_REG_H

#include "../Common/include/Firmware_Berlin_BG2CD_A0/ctypes.h" 

#define	w32NDTR0CS0_Config				{\
	UNSG32	ucTiming_tRP				:	3;\
	UNSG32	ucTiming_tRH				:	3;\
	UNSG32	ucTiming_etRP				:	1;\
	UNSG32	ucTiming_Reserved1			:	1;\
	UNSG32	ucTiming_tWP				:	3;\
	UNSG32	ucTiming_tWH				:	3;\
	UNSG32	ucTiming_Reserved2			:	2;\
	UNSG32	ucTiming_tCS				:	3;\
	UNSG32	ucTiming_tCH				:	3;\
	UNSG32	ucTiming_Rd_Cnt_Del			:	4;\
	UNSG32	ucTiming_SelCntr			:	1;\
	UNSG32	ucTiming_tADL 				: 	5;\
}

typedef	union	T32Gbl_NAND_Timing_Reg0{	
	UNSG32	u32 ; 
	struct	w32NDTR0CS0_Config ; 
}T32Gbl_NAND_Timing_Reg0 ;

#define	w32NDTR1CS0_Config				{\
	UNSG32	ucTiming_tAR				:	4;\
	UNSG32	ucTiming_tWHR				:	4;\
	UNSG32	ucTiming_Reserved1			:	6;\
	UNSG32	ucTiming_Prescale			:	1;\
	UNSG32	ucTiming_WaitMode			:	1;\
	UNSG32	ucTiming_tR					:	16;\
}

typedef	union	T32Gbl_NAND_Timing_Reg1{	
	UNSG32	u32 ; 
	struct	w32NDTR1CS0_Config ; 
}T32Gbl_NAND_Timing_Reg1 ;


#endif
