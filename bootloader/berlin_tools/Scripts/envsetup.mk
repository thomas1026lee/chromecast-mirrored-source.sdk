top_tool_dir = $(shell \
	TOPFILE="Scripts/common.mk"; \
	if [ -f $$TOPFILE ]; then \
		pwd; \
	else \
            HERE=$$PWD; \
            T=; \
            while [ \( ! \( -f $$TOPFILE \) \) -a \( $$PWD != "/" \) ]; do \
                cd .. > /dev/null; \
                T=`PWD= /bin/pwd`; \
            done; \
            cd $$HERE > /dev/null; \
            if [ -f "$$T/$$TOPFILE" ]; then \
                echo $$T; \
            fi; \
	fi)

work_dir = $(shell pwd)

scripts_dir := $(top_tool_dir)/Scripts
CLEAR_VARS := $(scripts_dir)/clear_vars.mk
define my-dir
$(strip \
	  $(eval md_file_ := $$(lastword $$(MAKEFILE_LIST))) \
	    $(if $(filter $(CLEAR_VARS),$(md_file_)), \
	       $(error LOCAL_PATH must be set before including $$(CLEAR_VARS)), \
	       $(patsubst %/,%,$(dir $(md_file_))) \
      ) \
  )
endef

MAJOR_VERSION = $(shell date +%Y%m%d)
MINOR_VERSION = $(shell date +%H%M)

top_data_dir := $(shell \
	cd $(top_tool_dir)/../berlin_config > /dev/null;\
	T=`PWD= /bin/pwd`; echo $$T)
top_fs_dir := $(top_data_dir)/File_Systems
config_file := $(top_data_dir)/config

temp_dir = $(top_tool_dir)/.temp
$(if $(wildcard $(temp_dir)),,$(shell mkdir $(temp_dir)))
PATH := $(PATH):$(temp_dir)

include $(config_file)
include $(top_tool_dir)/Scripts/supportedsettings.cfg
$(if $(LINUX_PARAM_COMMON_PART),,$(error LINUX_PARAM_COMMON_PART not defined inside $(config_file)))

CPU0CLK = $(shell echo $(CPU0CLKS)| cut -d '-' -f $(SYS_CLOCK_SETTING))
CPU1CLK = $(shell echo $(CPU1CLKS) | cut -d '-' -f $(SYS_CLOCK_SETTING))
DDRCLK = $(shell echo $(DDRCLKS) | cut -d '-' -f $(SYS_CLOCK_SETTING))
VSCOPECLK = $(shell echo $(VSCOPECLKS) | cut -d '-' -f $(SYS_CLOCK_SETTING))

export CONFIG_GPT
export top_tool_dir
export top_data_dir
export top_fs_dir
export temp_dir
export work_dir
export MAJOR_VERSION
export MINOR_VERSION
export SYS_CLOCK_SETTING
export CPU0CLK
export CPU1CLK
export DDRCLK
export VSCOPECLK
DDR_CHANNEL?=DDR_DUAL_CHANNEL
export DDR_CHANNEL
DDR_GENERATION_TYPE?=DDR3
export DDR_GENERATION_TYPE
export FLASH_TYPE
export CUST_KEY_IMAGE
export CUST_SELFKEY_IMAGE
export KEY_INJECTION_FLOW_ENABLE
export BOX_PRODUCT_RELEASE
export PRODUCTION_CHIP
export ROM_KEY_DISABLE
export OTP_RSA_KEY_FILE_NAME
export USE_CUST_KEY_ENCRYPT
export CPU_IMAGE_ENCRYPT
export MEMORY_MAP_OPTION
CPU_TYPE?=Z1
export CPU_TYPE
export PV_COMP_METHOD_PWM
export PV_COMP_METHOD_PG8x7
export LINUX_ON_CPU1
export OU_SCRIPT_PATH
export GEN_OUIMG_SCRIPT_PATH
export NAND_DEBUG
#export CPU0_LINUX_PARAM_ADDR
export CPU0_START_ADDR
export SYSTEM_CONFIGURATION_PATH
export PLATFORM
export PLATFORM_INFO
export BUILD_GOOGLETV
export BOOT_TYPE
export PV_COMP
export NFCECCCLK
export RANDOMIZER
export EXTERNAL_EMMC
export CONNECT_BUTTON_ENABLE
export LINUX_PARAM_NORMAL_BOOT_PART
export LINUX_PARAM_RECOVERY_BOOT_PART
export SM_IMAGE_ACTIVE
export SM_IMAGE_PATH
export SM_IMAGE_FORMAT
export ROOTFS_DIR
export DISABLE_CRASH_COUNTER
export BERLIN_CHIP
export CONFIG_USB
export CONFIG_USB0_STORAGE
export CONFIG_USB1_STORAGE

#
# setup enc_tool
#
ifeq ($(CPU_TYPE), Z1)
ENC_TOOL=$(top_tool_dir)/enc_tool_z1
ifeq ($(shell cp -f $(top_tool_dir)/enc_tool_z1 $(top_tool_dir)/enc_tool), 0)
$(warning copy enc_tool_z1 to enc_tool failed.)
$(error stopping)
endif
else
ENC_TOOL=$(top_tool_dir)/enc_tool_z2a0
ifeq ($(shell cp -f $(top_tool_dir)/enc_tool_z2a0 $(top_tool_dir)/enc_tool), 0)
$(warning copy enc_tool_z2a0 to enc_tool failed.)
$(error stopping)
endif
endif
export ENC_TOOL

#
# set pll frequency
#
SYSPLL?=800
CPUPLL?=1000
MEMPLL?=1600

export CPUPLL
export MEMPLL
export SYSPLL

BUILD_BINARY := $(scripts_dir)/binary.mk
BUILD_COMMON := $(scripts_dir)/common.mk
BUILD_SUB_IMAGE := $(scripts_dir)/sub_image.mk
BUILD_YAFFS_IMAGE := $(scripts_dir)/yaffs_image.mk
BUILD_EXT2FS_IMAGE := $(scripts_dir)/ext2fs_image.mk
BUILD_EXT4FS_IMAGE := $(scripts_dir)/ext4fs_image.mk

MAKE_EXT4FS := $(top_tool_dir)/Common/tools/make_ext4fs
export MAKE_EXT4FS

ifeq ($(strip $(CROSS_COMPILE)),)
CROSS_COMPILE=arm-marvell-linux-gnueabi-
endif
export CROSS_COMPILE

ifeq ($(strip $(APP_CROSS_COMPILE)),)
APP_CROSS_COMPILE=arm-marvell-linux-gnueabi-
endif
export APP_CROSS_COMPILE

ifeq ($(strip $(DISABLE_ELF_STRIP)),1)
ELF_STRIP_CMD := /bin/true
else
ELF_STRIP_CMD := $(CROSS_COMPILE)strip
endif
export ELF_STRIP_CMD
