##
#
# Makefile
#
# GNU makefile for BootLoader object
##



#
# configure file
#
-include configure.am
BUILD_GOOGLETV = true
CFG_BOARD_NAME = bg2proto

CROSS_COMPILE=arm-unknown-linux-gnueabi-
ifeq ($(strip $(CROSS_COMPILE)),)
CROSS_COMPILE=arm-unknown-linux-gnueabi-
endif

#CFLAGS		= -march=armv5t -I$(PROJ_DIR) -Wall -Wstrict-prototypes -g -Os -gdwarf-2 -fno-strict-aliasing  -fno-common -ffixed-r8 -msoft-float -fno-builtin -ffreestanding -nostdinc -isystem $(GCCINCDIR) -pipe

#CFLAGS		= -march=armv5t -I$(PROJ_DIR) -W -Wall -Wstrict-prototypes -gdwarf-2 -O0 -fno-common -fno-strict-aliasing -nostdinc -msoft-float -pipe 
#CFLAGS          = -mcpu=arm9e -I$(PROJ_DIR) -W -Wall -Wstrict-prototypes -gdwarf-2 -O0 -nostdinc -msoft-float -pipe

#CFLAGS		= -W -Wall -g -gdwarf-2 -O2 -msoft-float -nostdinc -march=armv5t -I$(PROJ_DIR) -DDUAL_DDR_ENABLE -DENABLE_REG_LOG -DTIME_CALC
#SFLAGS		= -Wa, -g -gdwarf-2 -O2 -msoft-float -nostdinc -march=armv5t -I$(PROJ_DIR) -DDUAL_DDR_ENABLE -DENABLE_REG_LOG -DTIME_CALC

#CFLAGS		= -W -Wall -g -gdwarf-2 -O1 -msoft-float -nostdinc -march=armv5t -I$(PROJ_DIR) -DDUAL_DDR_ENABLE -DENABLE_REG_LOG 
#SFLAGS		= -Wa, -g -gdwarf-2 -O1 -msoft-float -nostdinc -march=armv5t -I$(PROJ_DIR) -DDUAL_DDR_ENABLE -DENABLE_REG_LOG 

ifeq ($(CFG_BOARD_NAME), bg2proto)
  CFLAGS = -W -Wall -g -gdwarf-2 -O0 -msoft-float -nostdlib -fno-builtin -march=armv5t -I$(PROJ_DIR) -DBOOTLOADER -DDEBUG
  SFLAGS = -g -gdwarf-2 -O0 -msoft-float -nostdlib -fno-builtin -march=armv5t -I$(PROJ_DIR) -DBOOTLOADER
else
  CFLAGS = -W -Wall -mcpu=cortex-a9 -mtune=cortex-a9 -O2 -msoft-float -nostdlib -fno-builtin -I$(PROJ_DIR) -DBOOTLOADER -DDEBUG
  SFLAGS = -mcpu=cortex-a9 -mtune=cortex-a9 -O2 -msoft-float -nostdlib -fno-builtin -I$(PROJ_DIR) -DBOOTLOADER
endif

HOST_CFLAGS = -W -Wall -g -gdwarf-2 -O0

CHIPVERSIONS = -DBERLIN \
		-DBERLIN_B_0=0x1000 -DBERLIN_C_0=0x2000 -DBERLIN_A_0=0x0000 -DBERLIN_A_1=0x0010 -DBERLIN_A_2=0x0020 \
		-DBERLIN_BG2=0x3000 -DBERLIN_BG2_Z1=0x3000 -DBERLIN_BG2_Z2=0x3010 -DBERLIN_BG2_A0=0x3020 -DBERLIN_BG2_A1=0x3030 \
		-DBERLIN_BG2CD_A0=0x3100
CFLAGS += $(CHIPVERSIONS)
SFLAGS += $(CHIPVERSIONS)
HOST_CFLAGS += $(CHIPVERSIONS)

LDFLAGS		= -T bootloader.lds -Map bootloader.map -Bstatic -O0 

# Determine last commit in src tree and status to add to
# bootloader version info. configure.am, sm.bin are modified
# by build scripts, so ignore them during index check.
GIT_LAST_COMMIT = $(shell git rev-parse --verify --short HEAD)
GIT_TREE_DIRTY = $(if $(shell git diff-index --name-only HEAD | \
	grep -E -v '(configure.am|sm.bin)'),-dirty)
CFLAGS += -DBOOTLOADER_VERSION=\"$(GIT_LAST_COMMIT)$(GIT_TREE_DIRTY)\"

#
# Source tree roots
#
PROJ_DIR = .

ifeq ($(BERLIN_CHIP), BG2CD)
SUPPORT_FASTLOGO = 1
INCLUDE_DIR = include/Firmware_Berlin_BG2CD_A0
THINVPP_INCLUDE_DIR = THINVPP/source/include/include_BG2CD_A0
CFLAGS += -DBERLIN_CHIP_VERSION=0x3100 -DBG2CD=1
SFLAGS += -DBERLIN_CHIP_VERSION=0x3100 -DBG2CD=1
HOST_CFLAGS += -DBERLIN_CHIP_VERSION=0x3100

else

ifeq ($(CPU_TYPE), Z1)
INCLUDE_DIR = include/Firmware_Include_Z1
THINVPP_INCLUDE_DIR = THINVPP/source/include/include_BG2
CFLAGS += -DBERLIN_CHIP_VERSION=0x3000
SFLAGS += -DBERLIN_CHIP_VERSION=0x3000
HOST_CFLAGS += -DBERLIN_CHIP_VERSION=0x3000
else

ifeq ($(CPU_TYPE), Z2)
INCLUDE_DIR = include/Firmware_Include_Z1
THINVPP_INCLUDE_DIR = THINVPP/source/include/include_BG2_Z2
CFLAGS += -DBERLIN_CHIP_VERSION=0x3010
SFLAGS += -DBERLIN_CHIP_VERSION=0x3010
HOST_CFLAGS += -DBERLIN_CHIP_VERSION=0x3010
else
INCLUDE_DIR = include/Firmware_Include_A0
THINVPP_INCLUDE_DIR = THINVPP/source/include/include_BG2_A0
CFLAGS += -DBERLIN_CHIP_VERSION=0x3030
SFLAGS += -DBERLIN_CHIP_VERSION=0x3030
HOST_CFLAGS += -DBERLIN_CHIP_VERSION=0x3030
endif
endif

endif

CFLAGS += -DBERLIN_C_SERIES -I$(INCLUDE_DIR) -I$(THINVPP_INCLUDE_DIR) -Inand_ctrl/include/ -I../Common/randomizer/ -DCPU0_START_ADDR=$(CPU0_START_ADDR)
SFLAGS += -DBERLIN_C_SERIES -I$(INCLUDE_DIR)
HOST_CFLAGS += -I$(INCLUDE_DIR)

ifeq ($(SUPPORT_FASTLOGO),1)
SUPPORT_SHOWLOGO = 1
CFLAGS += -DBOOTLOADER_FASTLOGO=1 -DLOGO_ENABLE_MAIN=1 -DLOGO_ENABLE_PIP=1 -DLOGO_ENABLE_AUX=1
ifeq ($(SUPPORT_SHOWLOGO),1)
CFLAGS += -DBOOTLOADER_SHOWLOGO=1
#CFLAGS += -DBOOTLOADER_SHOWLOGO=1 -DBOOTLOADER_ANIMLOGO=1
endif
CFLAGS += -ITHINVPP/include
CFLAGS += -ITHINVPP/source/include
CFLAGS += -ITHINVPP/source/FE/include
CFLAGS += -ITHINVPP/source/SCL/include
CFLAGS += -ITHINVPP/source/CPCB/include
CFLAGS += -ITHINVPP/source/BE/include
CFLAGS += -ITHINVPP/source/MISC/include
CFLAGS += -IAVPLL/include
CFLAGS += -IDHUB/include
else
SUPPORT_SHOWLOGO = 0
endif

ifeq ($(BOX_PRODUCT_RELEASE), y)
CFLAGS += -DBOX_PRODUCT_RELEASE
SFLAGS += -DBOX_PRODUCT_RELEASE
endif

CFLAGS += -Iinclude/
SFLAGS += -Iinclude/
HOST_CFLAGS += -Iinclude/

CFLAGS += -Iclock/include/
SFLAGS += -Iclock/include/
HOST_CFLAGS += -Iclock/include/
#
# Common source
#
Source :=	$(PROJ_DIR)/init.S $(PROJ_DIR)/uish.S $(PROJ_DIR)/figo_sram_bin.c \
		$(PROJ_DIR)/initmmu.S\
		$(PROJ_DIR)/cache.S\
		$(PROJ_DIR)/util.c $(PROJ_DIR)/warm_linuxparam.c \
		$(PROJ_DIR)/figo.S \
		$(PROJ_DIR)/apb_uart_driver.c $(PROJ_DIR)/lgpl_printf.c string.c \
		$(PROJ_DIR)/warm_boot_contex.c \
		$(PROJ_DIR)/clock/galois_speed.c \
		$(PROJ_DIR)/pin_init.c \
		$(PROJ_DIR)/gpio.c \
		$(PROJ_DIR)/code_data.S \
		$(PROJ_DIR)/crc32.c

ifeq ($(SUPPORT_FASTLOGO),1)
Source += $(wildcard THINVPP/source/*.c)
Source += $(wildcard THINVPP/source/FE/source/*.c)
Source += $(wildcard THINVPP/source/SCL/source/*.c)
Source += $(wildcard THINVPP/source/CPCB/source/*.c)
Source += $(wildcard THINVPP/source/BE/source/*.c)
Source += $(wildcard THINVPP/source/MISC/source/*.c)
Source += $(wildcard AVPLL/source/*.c)
Source += $(wildcard DHUB/source/*.c)
ifeq ($(SUPPORT_SHOWLOGO),1)
Source += $(PROJ_DIR)/showlogo.c
endif
endif

ifeq ($(SM_IMAGE_ACTIVE), 1)
Source += $(PROJ_DIR)/SM_Comm.c $(PROJ_DIR)/SM_Driver.c
CFLAGS += -DCONFIG_SM
SFLAGS += -DCONFIG_SM
endif

ifeq ($(BUILD_GOOGLETV), true)
Source += $(PROJ_DIR)/bootloader.c $(PROJ_DIR)/android/ctype.c\
		$(PROJ_DIR)/android/strtol.c $(PROJ_DIR)/android/flash_ts.c \
		$(PROJ_DIR)/sha/sha1.c
CFLAGS += -DANDROID_BOOT=1 -DCFG_BOARD_NAME=$(CFG_BOARD_NAME) -Isha/
else
Source += $(PROJ_DIR)/bootloader.c $(PROJ_DIR)/android/ctype.c
endif

CustomizeFile := $(PROJ_DIR)/customization/$(PLATFORM)/bootloader_customize.c
ifeq ($(CustomizeFile), $(wildcard $(CustomizeFile)))
	Source += $(CustomizeFile)
else
	Source += $(PROJ_DIR)/customization/default/bootloader_customize.c
endif

Source += load_cust_key.c

ifneq ($(PV_COMP), )
CFLAGS += -DPV_COMP
Source += pv_comp/pv_compensation.c
ifeq ($(PV_COMP), PWM)
CFLAGS += -DPWM
Source += pv_comp/pwm.c pv_comp/pwm_driver.c
endif
ifeq ($(PV_COMP), ONEWIRE)
CFLAGS += -DONEWIRE
Source += pv_comp/one_wire.c
endif
endif

ifeq ($(CPUPLL), 1200)
CFLAGS += -DCPUPLL_1200
Source += clock/initClocks.c
endif

ifeq ($(BOOT_TYPE), NAND_BOOT)
Source += nand_ctrl/mv_nand.c $(PROJ_DIR)/nflash_drv.c ../Common/randomizer/nand_randomizer.c ../Common/randomizer/prbs15.c
CFLAGS += -DNAND_BOOT
SFLAGS += -DNAND_BOOT
ifeq ($(RANDOMIZER), y)
CFLAGS += -DENABLE_RANDOMIZER
SFLAGS += -DENABLE_RANDOMIZER
endif
ifeq ($(NFCECCCLK), 400)
CFLAGS += -DNFCECCCLK_400
SFLAGS += -DNFCECCCLK_400
endif
endif

ifeq ($(BOOT_TYPE), EMMC_BOOT)
Source += sdio_ctrl/irqHandler.S sdio_ctrl/sdioDiag.c \
		sdio_ctrl/sdioController.c sdio_ctrl/gicDiag.c \
		sdio_ctrl/sdio_printf.c emmc_drv.c
CFLAGS += -DEMMC_BOOT
SFLAGS += -DEMMC_BOOT

ifeq ($(EXTERNAL_EMMC), y)
CFLAGS += -DEXTERNAL_EMMC
SFLAGS += -DEXTERNAL_EMMC
endif
endif

ifeq ($(SUPPORT_SHOWLOGO),1)
Source += sdio_ctrl/irqHandler.S sdio_ctrl/gicDiag.c
CFLAGS += -I$(PROJ_DIR)/sdio_ctrl
endif

ifeq ($(CONFIG_USB), y)
Source += usb_ctrl/usb.c usb_ctrl/ehci-hcd.c \
	  usb_ctrl/ehci-berlin.c usb_ctrl/usb_storage.c \
	  timer/timer.c timer/apb_timer.c
CFLAGS += -Itimer -Iusb_ctrl
SFLAGS += -Itimer -Iusb_ctrl
HOST_CFLAGS += -Itimer -Iusb_ctrl
CFLAGS += -DCONFIG_USB
SFLAGS += -DCONFIG_USB

ifeq ($(CONFIG_USB0_STORAGE), y)
CFLAGS += -DCONFIG_USB0_STORAGE
SFLAGS += -DCONFIG_USB0_STORAGE
endif

ifeq ($(CONFIG_USB1_STORAGE), y)
CFLAGS += -DCONFIG_USB1_STORAGE
SFLAGS += -DCONFIG_USB1_STORAGE
endif

endif

ifeq ($(CONNECT_BUTTON_ENABLE), y)
CFLAGS += -DENABLE_CONNECT_BTN
SFLAGS += -DENABLE_CONNECT_BTN
endif

ifeq ($(CONFIG_GPT), y)
CFLAGS += -DCONFIG_GPT
SFLAGS += -DCONFIG_GPT
endif

ifeq ($(DISABLE_CRASH_COUNTER), y)
CFLAGS += -DDISABLE_CRASH_COUNTER
SFLAGS += -DDISABLE_CRASH_COUNTER
endif

FIGO_source := 	$(PROJ_DIR)/figo_sram.S
#
# Common objects
#
Object :=	$(patsubst %.S,%.o,$(Source))
Object :=	$(patsubst %.c,%.o,$(Object))

FIGO_Object :=	$(patsubst %.S,%.o,$(FIGO_source))
#
# Include the make variables (CC, etc...)
#
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CROSS_COMPILE)g++
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump
GCCINCDIR	= $(shell $(CC) -print-file-name=include)
PLATFORM_LIBS	= -L $(shell dirname `$(CC) $(CFLAGS) -print-libgcc-file-name`) -lgcc


#
# Common rule
#
.c.o:
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c -o $@ $(PROJ_DIR)/$<
.S.o:
	$(CC) $(SFLAGS) $(EXTRA_CFLAGS) -c -o $@ $(PROJ_DIR)/$<

#
# Common Make targets
#
all:	bootloader
#	@./addheader.sh $<.elf $<.img
#	@./encryption.sh $<.img $<.img.en

bootloader:	header $(Object)
	$(LD) $(LDFLAGS) $(EXTRA_LDFLAGS) -o $@.elf $(Object) $(PLATFORM_LIBS)
	$(OBJCOPY) -O binary $@.elf $@.bin

.PHONY: gen_uboot_scr
gen_uboot_scr: gen_uboot_scr.c pin_settings.h
	gcc $(HOST_CFLAGS) $< -o $@
	./$@
#############################################################################
#	configuration
#############################################################################
configure: $(FIGO_Object)
	gcc code.c -o code
	$(LD)  -T figo_sram.lds -o figo_sram.elf $(FIGO_Object)
	$(OBJCOPY)      -O binary figo_sram.elf figo_sram.bin
	./code
	./berlin_config.sh 

#.PHONY: header
#header:	$(INCLUDE_DIR)/Galois_memmap.h $(INCLUDE_DIR)/global.h $(INCLUDE_DIR)/MctrlDual.h $(INCLUDE_DIR)/drm_sw_spec.h
#	@./header.sh $(INCLUDE_DIR)/Galois_memmap.h $(PROJ_DIR)/memmap.h MEMMAP_
#	@./header.sh $(INCLUDE_DIR)/global.h $(PROJ_DIR)/ra_gbl.h RA_Gbl_
#	@./header.sh $(INCLUDE_DIR)/MctrlDual.h $(PROJ_DIR)/mem_ctrl.h RA_MctrlDual_
#	@./header.sh $(INCLUDE_DIR)/drmdmx.h $(PROJ_DIR)/figo_drm.h Figo_Drm_
#	@./header.sh $(INCLUDE_DIR)/drm_sw_spec.h $(PROJ_DIR)/drm.h DRM_

.PHONY: clean
clean:
	@rm -rf $(Object) $(FIGO_Object) bootloader.elf bootloader.map bootloader.bin configure.ame
	@rm -rf memmap.h ra_gbl.h mem_ctrl.h figo_drm.h drm.h
	@rm -rf flash.bin encrypt
	@rm -rf debug.log
	@rm -rf figo_sram.bin figo_sram.elf figo_sram_bin.c
	@rm -rf code echobin flash_en.bin
	@rm -rf cold_cmd_table.bin warm_cmd_table.bin
	@rm -rf	loader.bin loader_en.bin sm.en.bin 
	@rm -rf global.h Galois_memmap.h
	@rm -rf gen_uboot_scr

.PHONY: regen
regen: clean all
