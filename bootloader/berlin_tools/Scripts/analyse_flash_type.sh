#!/bin/bash
#

[ $# -ne 2 ] && exit 1

FLASH_TYPE=$1
OUT=$2

error() {
	echo $1
	exit 1
}

is_all_digits() {
	[ "$1" = "" ] && return 0
	echo $1 | egrep "[^0-9]" > /dev/null
	if [ $? -eq 0 ]; then
		return 0
	else
		return 1
	fi
}

PAGE_DATA_SIZE=`echo $FLASH_TYPE | cut -d '_' -f 1`
is_all_digits $PAGE_DATA_SIZE && {
	error "FLASH_TYPE variable doesn't contain page size"
}
PAGE_OOB_SIZE=`echo $FLASH_TYPE | cut -d '_' -f 2`
is_all_digits $PAGE_OOB_SIZE && {
	error "FLASH_TYPE variable doesn't contain oob size"
}
PAGENUM_PER_BLOCK=`echo $FLASH_TYPE | cut -d '_' -f 3`
is_all_digits $PAGENUM_PER_BLOCK && {
	error "FLASH_TYPE variable doesn't contain page number in one block"
}
CHIP_SIZE=`echo $FLASH_TYPE | cut -d '_' -f 4`
is_all_digits $CHIP_SIZE && {
	error "FLASH_TYPE variable doesn't contain chip size"
}
BLOCK_DATA_SIZE=$(($PAGE_DATA_SIZE*$PAGENUM_PER_BLOCK))

echo $FLASH_TYPE > $OUT
echo $PAGE_DATA_SIZE >> $OUT
echo $PAGE_OOB_SIZE >> $OUT
echo $PAGENUM_PER_BLOCK >> $OUT
echo $CHIP_SIZE >> $OUT
echo $BLOCK_DATA_SIZE >> $OUT

exit 0
