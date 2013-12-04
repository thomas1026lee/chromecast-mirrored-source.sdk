#!/bin/bash

source ../Customization_Data/config
pagesize=`echo $FLASH_TYPE | cut -d _ -f1`
page_per_block=`echo $FLASH_TYPE | cut -d _ -f3`
nand_capacity=`echo $FLASH_TYPE | cut -d _ -f4`
block_size=`expr $pagesize \* $page_per_block`
#block_size_kB=`expr $block_size / 1024`
#echo "pagesize=$pagesize , page_per_block=$page_per_block"
#echo "block_size_kB=$block_size_kB"

#	Generate block 0
make -C SUB_IMAGE/block0/

#	Generate bootloader
make -C SUB_IMAGE/bootloader/

#	Generate boot images
make -C SUB_IMAGE/bootimgs/

make -C BINARY/gen_uniimg/
.temp/gen_uniimg -o /tftpboot/tzhang/uNAND.img -d .temp/ -p $pagesize -b $block_size -c $nand_capacity -j 1234 -n 1234 block0 bootloader bootimgs
