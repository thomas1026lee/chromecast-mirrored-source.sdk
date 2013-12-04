#
# sub image Rules
#
# $(module_name) variable should be defined in each module's Makefile
#

sub_image_suffix := .subimg .subimg.info
module_target := \
	$(foreach suffix, $(sub_image_suffix), $(temp_dir)/$(module_name)$(suffix))

$(temp_dir)/$(module_name).subimg:

#
# we need to know the size of subimage, so subimg.info depends on subimage
#
$(temp_dir)/$(module_name).subimg.info: PRIVATE_MODULE_NAME := $(module_name)
$(temp_dir)/$(module_name).subimg.info: PRIVATE_RES_BLKS := $(reserved_blocks)

ifeq ("$(BOOT_TYPE)", "EMMC_BOOT")
$(temp_dir)/$(module_name).subimg.info: \
$(temp_dir)/subimglayout \
$(temp_dir)/$(module_name).subimg \
$(temp_dir)/gen_subimg_info
	@echo "* generating $@"
	START_BLK_IND=`egrep -w $(PRIVATE_MODULE_NAME) $(temp_dir)/subimglayout | awk '{print $$2}'` && \
	NUM_BLKS=`egrep -w $(PRIVATE_MODULE_NAME) $(temp_dir)/subimglayout | awk '{print $$3}'` && \
	gen_subimg_info --name $(PRIVATE_MODULE_NAME) \
			--major $(MAJOR_VERSION) --minor $(MINOR_VERSION) \
			--reserved_blocks $(PRIVATE_RES_BLKS) \
			--start_blkind $$START_BLK_IND \
			--num_blocks $$NUM_BLKS \
			--output $@ \
			$(temp_dir)/$(PRIVATE_MODULE_NAME).subimg
else
$(temp_dir)/$(module_name).subimg.info: \
$(temp_dir)/subimglayout \
$(temp_dir)/$(module_name).subimg \
$(temp_dir)/gen_subimg_info
	@echo "* generating $@"
	START_BLK_IND=`egrep -w $(PRIVATE_MODULE_NAME) $(temp_dir)/subimglayout | awk '{print $$2}'` && \
	NUM_BLKS=`egrep -w $(PRIVATE_MODULE_NAME) $(temp_dir)/subimglayout | awk '{print $$3}'` && \
	DATA_TYPE=`egrep -w $(PRIVATE_MODULE_NAME) $(temp_dir)/subimglayout | awk '{print $$4}'` && \
	gen_subimg_info --name $(PRIVATE_MODULE_NAME) \
			--major $(MAJOR_VERSION) --minor $(MINOR_VERSION) \
			--reserved_blocks $(PRIVATE_RES_BLKS) \
			--start_blkind $$START_BLK_IND \
			--num_blocks $$NUM_BLKS \
			--data_type $$DATA_TYPE \
			--output $@ \
			$(temp_dir)/$(PRIVATE_MODULE_NAME).subimg
endif
include $(BUILD_COMMON)
