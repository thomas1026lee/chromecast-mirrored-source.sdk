#!/bin/bash
if [ $# != "2" ];then
	echo "Usage: $0 in_file out_file"
	exit 1
fi
INFILE=$1
OUTFILE=$2
if [ -z $CROSS_COMPILE ]; then
CROSS_COMPILE=arm-marvell-eabi-
fi
READELF=${CROSS_COMPILE}readelf
OBJCOPY=${CROSS_COMPILE}objcopy
ELF_DUMP=.temp_elf_dump
BIN_FILE=.temp_bin
HEADER_FILE=.temp_heder

if [ ! -f $INFILE ];then
	echo "Input file $INFILE does not exist"
	exit 1
fi
cp $INFILE $BIN_FILE -f

ISELF=`xxd -l4 $INFILE | grep "\.ELF"`
if [ -n "$ISELF" ];then
	echo "Input file is elf type, extracting binary..."
	$READELF -S $INFILE > $ELF_DUMP
	EXE_ADDR=0x`grep '\.text' $ELF_DUMP | sed 's/\[[ ]*[(0-9)]*\]//' | awk {'print $3'}`
	$OBJCOPY -O binary $INFILE $BIN_FILE
	rm $ELF_DUMP -f
fi

FILE_OFF=32
OFF_HEX=`printf %08x $FILE_OFF`
OFF1=`echo $OFF_HEX|cut -b 7-8`
OFF2=`echo $OFF_HEX|cut -b 5-6`
OFF3=`echo $OFF_HEX|cut -b 3-4`
OFF4=`echo $OFF_HEX|cut -b 1-2`

FILE_LEN=`stat -c %s $BIN_FILE`
LEN_HEX=`printf %08x $FILE_LEN`
LEN1=`echo $LEN_HEX|cut -b 7-8`
LEN2=`echo $LEN_HEX|cut -b 5-6`
LEN3=`echo $LEN_HEX|cut -b 3-4`
LEN4=`echo $LEN_HEX|cut -b 1-2`

EXE_ADDR_HEX=`printf %08x $EXE_ADDR`
EXE1=`echo $EXE_ADDR_HEX|cut -b 7-8`
EXE2=`echo $EXE_ADDR_HEX|cut -b 5-6`
EXE3=`echo $EXE_ADDR_HEX|cut -b 3-4`
EXE4=`echo $EXE_ADDR_HEX|cut -b 1-2`

echo "F1A3ADD2" "$OFF1$OFF2$OFF3$OFF4" "$LEN1$LEN2$LEN3$LEN4" "$EXE1$EXE2$EXE3$EXE4" \
	"00000000" "00000000" "00000000" "00000000" | xxd -r -p > $HEADER_FILE
cat $HEADER_FILE $BIN_FILE> $OUTFILE
rm $BIN_FILE $HEADER_FILE -f
