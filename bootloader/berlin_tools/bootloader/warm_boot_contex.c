extern unsigned int FIGO_Sram_Code[] ; 
extern unsigned int Figo_Sram_Code_Size ;
void 	Copy_Code_To_Figo_Sram(){
	
	unsigned int * pFigo_SRAM_Base ;
	int i ; 
	unsigned int * pCode ; 
	pCode = FIGO_Sram_Code ;

	pFigo_SRAM_Base = (unsigned int *)0xF7CC1000 ;
	// copy code to FIGO SRAM
	for( i = 0 ; i < Figo_Sram_Code_Size ; i ++ )
        {
                *pFigo_SRAM_Base ++ = *pCode ++;
        }
//	while(1) ; 
	//jumpt to figo SRAM
	__reset_cpu(0xF7CC1000, 0, 0) ;
}

