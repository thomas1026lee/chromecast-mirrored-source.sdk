SHELL:=/bin/bash


# check for broken versions of make
ifeq (0,$(shell expr $$(echo $(MAKE_VERSION) | sed "s/[^0-9\.].*//") \>= 3.81))
$(warning ********************************************************************************)
$(warning *  You are using version $(MAKE_VERSION) of make.)
$(warning *  You must upgrade to version 3.81 or greater.)
$(warning ********************************************************************************)
$(error stopping)
endif

all_modules :=
all_module_targets :=
all_module_clean_targets :=

#
# This is the very first rule!!!
#
.PHONY: debu
DEFAULT_GAOL := debu
$(DEFAULT_GAOL) :

#
# include *module.mk
#
subdirs := SUB_IMAGE IMAGE BINARY
abs_subdirs := $(foreach dir, $(subdirs), $(top_tool_dir)/$(dir))
subdir_makefiles := $(shell find $(abs_subdirs) -name module.mk |\
   						egrep -v "\.svn")
#$(info )
#$(info ****** begin to include '$(subdir_makefiles)')
include $(subdir_makefiles)
$(info )
$(info ****** subdir_makefiles all included)
$(info )

#
# which module(s) to make
#

ifeq ($(work_dir), $(top_tool_dir))
debu: $(all_modules)
else
$(if $(wildcard module.mk),,$(error No 'module.mk'))
my_module := $(strip $(shell awk -F "\=" '/module_name/{print $$2}' module.mk | head -n 1))
$(info main.mk current module = $(my_module))
$(if $(my_module),,$(error need to define 'module_name' in module.mk))
debu: $(my_module)
	@echo "module \`$(my_module)' has been built"
endif

#
# common rules
#
.PHONY: flash_type subimglayout version_table mtdparts reco_rootfs linux_param
flash_type: $(temp_dir)/flash_type
version_table: $(temp_dir)/version_table
subimglayout: $(temp_dir)/subimglayout
mtdparts: $(temp_dir)/mtdparts
reco_rootfs: $(temp_dir)/reco_rootfs.cpio.gz
linux_param: $(temp_dir)/linux_param $(temp_dir)/linux_param.h

page_data_size = $(shell head -n 2 $(temp_dir)/flash_type 2>/dev/null | tail -n 1)
page_oob_size = $(shell head -n 3 $(temp_dir)/flash_type 2>/dev/null | tail -n 1)
pages_per_block = $(shell head -n 4 $(temp_dir)/flash_type 2>/dev/null | tail -n 1)
chip_size = $(shell head -n 5 $(temp_dir)/flash_type 2>/dev/null | tail -n 1)
block_size = $(shell head -n 6 $(temp_dir)/flash_type 2>/dev/null | tail -n 1)
aft_script = $(top_tool_dir)/Scripts/analyse_flash_type.sh

$(temp_dir)/flash_type: $(config_file) $(aft_script)
	@[ "$(FLASH_TYPE)" = "" ] && {echo missing FLASH_TYPE in $(config_file) && exit 1} || exit 0
	@if [ -f $(temp_dir)/flash_type -a $@ -ot $(aft_script) ]; then \
		$(aft_script) $(FLASH_TYPE) $@; \
	else \
		old_flash_type=`[ -f $@ ] && head -n 1 $@`; \
		if [ "$(FLASH_TYPE)" != "$$old_flash_type" ]; then \
			$(aft_script) $(FLASH_TYPE) $@; \
		else \
			exit 0; \
		fi; \
	fi

$(temp_dir)/subimglayout $(temp_dir)/version_table $(temp_dir)/mtdparts: \
$(config_file) $(top_data_dir)/$(PARTITION_TABLE_PATH) $(temp_dir)/flash_type $(temp_dir)/parse_pt $(temp_dir)/crc
	if [ "$(BOOT_TYPE)" = "EMMC_BOOT" ]; then \
		parse_pt $(MAJOR_VERSION) $(MINOR_VERSION) \
		$(block_size) $(chip_size) \
		$(top_data_dir)/$(PARTITION_TABLE_PATH) \
		$(temp_dir)/mtdparts \
		$(temp_dir)/version_table \
		$(temp_dir)/subimglayout \
		$(temp_dir)/emmc_part_table \
		$(temp_dir)/emmc_part_list \
		$(temp_dir)/emmc_image_list; \
	else \
		parse_pt $(MAJOR_VERSION) $(MINOR_VERSION) \
		$(block_size) $(chip_size) \
		$(top_data_dir)/$(PARTITION_TABLE_PATH) \
		$(temp_dir)/mtdparts \
		$(temp_dir)/version_table \
		$(temp_dir)/subimglayout;\
	fi; \
	crc -a $(temp_dir)/version_table

#	@parse_pt --major $(MAJOR_VERSION) --minor $(MINOR_VERSION) \
#		--block_size $(block_size) --chip_size $(chip_size) \
#		$(top_data_dir)/$(PARTITION_TABLE_PATH) \
#		--mtdparts $(temp_dir)/mtdparts \
#		--vt $(temp_dir)/version_table \
#		--subimglayout $(temp_dir)/subimglayout

# linux parameter common part is moved to Customization_Data/config
#LINUX_PARAM_COMMON_PART := sata_reset_done nfc_shared console=ttyS0,115200 root=/dev/mtdblock5 rootfstype=yaffs2 ro init=/bin/srvd mtdparts=mv_nand:
#for android it should be
#LINUX_PARAM_COMMON_PART := sata_reset_done nfc_shared console=ttyS0,115200 root=/dev/mtdblock5 rootfstype=yaffs2 ro init=/init androidboot.console=ttyS0 mtdparts=mv_nand:

$(temp_dir)/linux_param: $(temp_dir)/linux_param_commonpart $(temp_dir)/mtdparts
	@echo "* generate linux parameters: $@"
	if [ "$(BOOT_TYPE)" != "EMMC_BOOT" ]; then \
		cat $^ > $@; \
	else \
		cat $< > $@; \
	fi

$(temp_dir)/linux_param_commonpart: $(config_file)
		\echo -n $(LINUX_PARAM_COMMON_PART) > $@;

$(temp_dir)/linux_param.h: $(temp_dir)/linux_param
	echo "* generate linux parameters header: $@"
	. $(scripts_dir)/gen_linux_param.sh

#
# recovery rootfs inside bootimgs
#
reco_rootfs_dir := $(temp_dir)/reco_rootfs.tmp

.PHONY: $(temp_dir)/reco_rootfs.cpio.gz
$(temp_dir)/reco_rootfs.cpio.gz:
	@echo "generating $@"
	@rm -rf $(reco_rootfs_dir)
	@cp -rpf $(top_tool_dir)/indep_rootfs $(reco_rootfs_dir)
	@find $(reco_rootfs_dir) -name "\.svn" | xargs rm -rf
	@mv $(reco_rootfs_dir)/init_recovery.sh $(reco_rootfs_dir)/ou.sh
	@rm $(reco_rootfs_dir)/init_ou.sh
	@$(ELF_STRIP_CMD) -s $(reco_rootfs_dir)/OU_tools/OU_GUI
	-@rm -rf $(reco_rootfs_dir)/OU_tools/OU_Burner
	@cd $(reco_rootfs_dir) && \
		find . -print | cpio -o -H newc | gzip -c > $(temp_dir)/reco_rootfs.cpio.gz
	@echo "GEN $@"

.PHONY: clean
clean: $(all_module_clean_targets)
	-@rm -rf $(temp_dir)/reco_rootfs.cpio.gz $(temp_dir)/subimglayout \
		$(temp_dir)/linux_param_commonpart $(temp_dir)/linux_param $(temp_dir)/linux_param.h $(temp_dir)/mtdparts \
		$(temp_dir)/version_table $(temp_dir)/flash_type \
		$(reco_rootfs_dir)
	-@rm -rf $(top_tool_dir)/enc_tool

help: help_main help_local

help_local::

help_main:
	@echo
	@echo "Make commands:"
	@echo "  1. Generate all targets:"
	@echo "    [Variable setting list ] make"
	@echo "  2. Generate specific targets:"
	@echo "    [Variable setting list ] make [target_name1 target_name2 ...]"
	@echo "  3. Clean all targets:"
	@echo "    [Variable setting list ] make clean"
	@echo "  4. Clean specific target:"
	@echo "    [Variable setting list ] make [clean-target_name1 clean-target_name2 ...]"
	@echo
	@echo "  Variable setting list is to overide the default value"
	@echo "  Available Target names: "
	@echo "      [$(all_modules) ]"
	@echo
