#!/bin/bash

source ../../berlin_config/System_Configuration/Device_Timing/Nand_Timing_Config &&\
source ../../berlin_config/config || exit 1

pagesize=`echo $FLASH_TYPE | cut -d _ -f1`
page_per_block=`echo $FLASH_TYPE | cut -d _ -f3`
block_size=`expr $pagesize \* $page_per_block`

echo "#ifndef	__NAND_PARAM_H" > nand_param.h
echo "#define	__NAND_PARAM_H" >> nand_param.h
echo "" >> nand_param.h
echo "" >> nand_param.h

echo "#define NDTR0CS0		$NDTR0CS0" >> nand_param.h
echo "#define NDTR1CS0		$NDTR1CS0" >> nand_param.h
echo "#define NDREDEL		$NDREDEL" >> nand_param.h
echo "" >> nand_param.h
echo "#define NAND_PAGE_SIZE				$pagesize" >> nand_param.h
echo "#define NAND_BLOCK_SIZE				$block_size" >> nand_param.h
echo "#define NAND_ADDRESS_CYCLE				5" >> nand_param.h
echo "#define NAND_SPARE_EN					1" >> nand_param.h
echo "#define NAND_ECC_EN					1" >> nand_param.h
echo "#define NAND_BCH_EN					1" >> nand_param.h
echo "#define NAND_ECC_STRENGTH				$ECC_STRENGTH" >> nand_param.h

echo "" >> nand_param.h
echo "" >> nand_param.h

echo "#endif" >> nand_param.h
