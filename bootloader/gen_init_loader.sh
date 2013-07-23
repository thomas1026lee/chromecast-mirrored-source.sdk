#!/bin/bash

[ "$FLASH_TYPE" = "" -o "$CUST_SELFKEY_IMAGE" = "" ] && {
	source ../../Customization_Data/config
	if [ -z $CPUPLL ];then
		CPUPLL=1000
	fi
	if [ -z $MEMPLL ];then
		MEMPLL=1600
	fi
	if [ -z $SYSPLL ];then
		SYSPLL=800
	fi
	export PLATFORM
	export CPUPLL
	export PV_COMP
	export NFCECCCLK
	export LINUX_ON_CPU1
	export CPU_TYPE
	export CPU_PLL
	export MEMPLL
	export SYSPLL
	export SYSTEM_CONFIGURATION_PATH
	export PLATFORM_INFO
	export BUILD_GOOGLETV
	export BOOT_TYPE
	export RANDOMIZER
	export SM_IMAGE_ACTIVE
	export SM_IMAGE_PATH
	export SM_IMAGE_FORMAT
	export CPU0_START_ADDR
	export CONFIG_GPT
	export DISABLE_CRASH_COUNTER
	export BERLIN_CHIP
	top_data_dir=../../Customization_Data/
	export top_data_dir  
}
pagesize=`echo $FLASH_TYPE | cut -d _ -f1`
page_per_block=`echo $FLASH_TYPE | cut -d _ -f3`
block_size=`expr $pagesize \* $page_per_block`
block_size_kB=`expr $block_size / 1024`
echo "pagesize=$pagesize , page_per_block=$page_per_block"
echo "block_size_kB=$block_size_kB"
WORKING_DIR=`pwd`
ADDHEADER_TOOL="../Scripts/addheader.sh"
LOCAL_CUST_SELFKEY_PATH=$WORKING_DIR/../../Customization_Data/${CUST_SELFKEY_IMAGE}
SYSTEM_INIT_FILE_LOCAL=SI_part1.bin
SI_CUST_FIGO_IMAGE_LOCAL=SI_cust_figo.bin
SM_PATH=$WORKING_DIR/../../Customization_Data/$SM_IMAGE_PATH
if [ $BOOT_TYPE = "NAND_BOOT" ]; then
	BOOT_DEV=nand
else
	BOOT_DEV=emmc
fi

# copy the linux parameters (bootargs)
if [ ! -f ../.temp/linux_param.h ]; then
	echo run \"make linux_param\" under MV88DE3100_Tools/ first
	exit 1
fi
cp -f ../.temp/linux_param.h .

# check platform info
if [ \( ! -f $top_data_dir/$PLATFORM_INFO \) -a \( ! -L $top_data_dir/$PLATFORM_INFO \) ]; then
	echo "Error: Pin setting file $top_data_dir/$PLATFORM_INFO does not exist."
	exit 1
fi
cp -f $top_data_dir/$PLATFORM_INFO pin_settings.h ||exit 1

# copy SM
if [ "$SM_IMAGE_FORMAT" = "binary" ]; then
	echo "SM image is a binary file"
else
	echo "SM image is a $SM_IMAGE_FORMAT file" do not support now
	exit 1
fi
cp -f $SM_PATH sm.bin

if [ $BERLIN_CHIP = "BG2CD" ]; then
	SYS_INIT_DIR=../sys_init_bg2cd
else
if [ $CPU_TYPE = "Z1" -o $CPU_TYPE = "Z2" ];then
	SYS_INIT_DIR=../sys_init_Zx
else
	SYS_INIT_DIR=../sys_init
fi
fi

SYS_INIT_BIN_FILE=sys_init_${BOOT_DEV}_secure_CPU"$CPUPLL"_MEM"$MEMPLL"_SYS"$SYSPLL"_$CPU_TYPE.bin

[ $# = 1 ] && VT_PATH=$1 || VT_PATH=../.temp/version_table

#if [ "$ROM_KEY_DISABLE" = "0" ]; then
#	cp diag_jtag_switch_figo_romkey.bin figo_image.bin
#else
#	cp diag_jtag_switch_figo_distkey.bin figo_image.bin
#fi

if [ "$PRODUCTION_CHIP" = "0" ]; then
	echo "Generating bootloader for engineer chip"
	if [ ! -f ../DRM_Image/customer_keystore_figo.bin_ROM_Key ]; then
		echo "../DRM_Image/customer_keystore_figo.bin_ROM_Key not found"
		exit 1
	fi
	DRM_IMAGE_FILE=../DRM_Image/customer_keystore_figo.bin_ROM_Key
	if [ "is$PLATFORM" != "is" -a -d $SYS_INIT_DIR/$PLATFORM ]; then
		SYS_INIT_BIN_DIR=$SYS_INIT_DIR/$PLATFORM/${BOOT_DEV}_secure_romkey
	else
		SYS_INIT_BIN_DIR=$SYS_INIT_DIR/default/${BOOT_DEV}_secure_romkey
	fi
else
	echo "Generating bootloader for production chip"
	if [ ! -f ../DRM_Image/customer_keystore_figo.bin_Dist_Key ]; then
		echo "../DRM_Image/customer_keystore_figo.bin_Dist_Key not found"
		exit 1
	fi
	DRM_IMAGE_FILE=../DRM_Image/customer_keystore_figo.bin_Dist_Key
	if [ "is$PLATFORM" != "is" -a -d $SYS_INIT_DIR/$PLATFORM ]; then
		SYS_INIT_BIN_DIR=$SYS_INIT_DIR/$PLATFORM/${BOOT_DEV}_secure_distkey
	else
		SYS_INIT_BIN_DIR=$SYS_INIT_DIR/default/${BOOT_DEV}_secure_distkey
	fi
fi

# From eureka-b2 onwards, new securestore format is used for better
# security. Pick correct FIGO firmware and default CUSTK files in
# new format.
# eureka-b2 => CFG_BOARD_NAME = 3
# eureka-b3 => CFG_BOARD_NAME = 4
if (( $CFG_BOARD_NAME > 2 ))
then
  DRM_IMAGE_FILE=../DRM_Image/customer_keystore_figo.bin_Dist_Key-eureka
  SYS_INIT_BIN_DIR=$SYS_INIT_DIR/$PLATFORM/${BOOT_DEV}_secure_distkey
  CUST_KEY_IMAGE=${CUST_KEY_IMAGE}-eureka
fi

#	Copy system init part1 image and cust figo image to local
cp $DRM_IMAGE_FILE $SI_CUST_FIGO_IMAGE_LOCAL

echo "sys_init binary file: $SYS_INIT_BIN_DIR/$SYS_INIT_BIN_FILE"
cp $SYS_INIT_BIN_DIR/$SYS_INIT_BIN_FILE $SYSTEM_INIT_FILE_LOCAL || exit 1

if [ ! -f $VT_PATH ];then
	echo "please generate $VT_PATH first."
	exit 1
fi

make clean &&\
make configure CPU_TYPE=$CPU_TYPE CPUPLL=$CPUPLL BOX_PRODUCT_RELEASE=$BOX_PRODUCT_RELEASE BOOT_TYPE=$BOOT_TYPE RANDOMIZER=$RANDOMIZER &&\
make >/dev/null && \
rm -f pin_settings.h || exit 1


echo cust_self_key=$CUST_SELFKEY_IMAGE
ls -l $WORKING_DIR/../../Customization_Data/$CUST_SELFKEY_IMAGE

$ADDHEADER_TOOL bootloader.elf bootloader.img && \
cp -f ../enc_tool . && \
cp -f ../Common/tools/encryption.sh ./ 
cp -f ../Common/tools/encrypt ./
if [ "$CPU_IMAGE_ENCRYPT" = "y" ];then
	cp -f $LOCAL_CUST_SELFKEY_PATH ./ 
fi
./encryption.sh bootloader.img bootloader_en.img || {
	echo fail to generate bootloader_en.img
	exit 1
}

if [ "$ROM_KEY_DISABLE" = "" ];then
    echo "ROM_KEY_DISABLE is not set in config file, please update your config file"
    exit 1
fi

if [ "$ROM_KEY_DISABLE" = "1" ]; then
	echo " ****************** ROM key has been disabled ************"

	if [ "$OTP_RSA_KEY_FILE_NAME" = "" ]; then
		echo "ROM KEY has been disabled, but OTP_RSA_KEY_FILE_NAME is not defined"
		exit 1 
	fi
	
	if [ ! -f ../../Customization_Data/${OTP_RSA_KEY_FILE_NAME} ];then
		echo " Can not find User RSA key file: ../../Customization_Data/${OTP_RSA_KEY_FILE_NAME} "
		exit 1
	fi
	cp ../../Customization_Data/${OTP_RSA_KEY_FILE_NAME} ./
	#cp ../SPI_Secure_Uboot_Releases_Prod_Chip/ChangeSign ./
	cp ../kms_release_bg2_32/seclab/bin/ChangeSign ./
	if [ ! -f ./ChangeSign ];then
		echo "ChangeSign doesn't exist"
		exit 1
	fi

    echo "	resign system init..."
    ./ChangeSign $SYSTEM_INIT_FILE_LOCAL 0
	echo "  resign cust figo image..."
	./ChangeSign $SI_CUST_FIGO_IMAGE_LOCAL 0
fi

cp $SYSTEM_INIT_FILE_LOCAL ../IMAGE/Key_Injection/SI_part1
cp $SI_CUST_FIGO_IMAGE_LOCAL ../IMAGE/Key_Injection/SI_figo_image
cp $WORKING_DIR/../../Customization_Data/${CUST_KEY_IMAGE} ../IMAGE/Key_Injection/SI_cust_key
cp bootloader_en.img ../IMAGE/Key_Injection/SI_part2

cp -vf $SYSTEM_INIT_FILE_LOCAL image2_init.bin &&\
$ADDHEADER_TOOL $SI_CUST_FIGO_IMAGE_LOCAL figo.img &&\
$ADDHEADER_TOOL $WORKING_DIR/../../Customization_Data/${CUST_KEY_IMAGE} key.img || exit 1

echo --------------------
echo     begin padding
echo --------------------
dd if=$VT_PATH of=version conv=notrunc && \
SIZE=`stat -c %s version` && \
dd if=/dev/urandom of=version bs=1 seek=$SIZE count=`expr 4096 - $SIZE` conv=notrunc && \
SIZE=`stat -c %s figo.img` && \
dd if=/dev/urandom of=figo.img bs=1 seek=$SIZE count=`expr 4096 - $SIZE` conv=notrunc && \
SIZE=`stat -c %s key.img` && \
dd if=/dev/urandom of=key.img bs=1 seek=$SIZE count=`expr 4096 - $SIZE` conv=notrunc && \
cat image2_init.bin version figo.img key.img bootloader_en.img> init_loader.img || exit 1
cp init_loader.img bootloader_nopadding.img
if [ $BOOT_TYPE = "EMMC_BOOT" ]; then
	INIT_LOADER_FINAL_SIZE=`stat -c %s init_loader.img`
    if [ $INIT_LOADER_FINAL_SIZE -ge 524288 ]; then
        echo "Error: bootloader partition image size is greater than 512K."
        exit 1;
    fi
	dd if=/dev/zero of=init_loader.img bs=1 count=1 seek=524287 conv=notrunc || exit 1
else
	INIT_LOADER_FINAL_SIZE=`stat -c %s init_loader.img`
    if [ $INIT_LOADER_FINAL_SIZE -ge $block_size ]; then
        echo "Error: bootloader partition image size is greater than block size."
        exit 1;
    fi
    SIZE=`stat -c %s init_loader.img` && \
	dd if=/dev/urandom of=init_loader.img bs=1 seek=$SIZE count=`expr $block_size - $SIZE` conv=notrunc || exit 1
fi
