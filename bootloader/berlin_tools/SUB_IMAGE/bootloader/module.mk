LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

module_name := bootloader

include $(BUILD_SUB_IMAGE)

.PHONY: $(temp_dir)/$(module_name).subimg
$(temp_dir)/$(module_name).subimg: $(temp_dir)/version_table $(temp_dir)/linux_param.h
	# sys_init is in secure domain, don't genereate it in real-time
	# @cd $(top_tool_dir)/sys_init/source && ./gen_sys_init.sh
	@cd $(top_tool_dir)/bootloader && ./gen_init_loader.sh $(temp_dir)/version_table
	cp -v $(top_tool_dir)/bootloader/init_loader.img $@

clean-$(module_name)::
	-@$(MAKE) -C $(top_tool_dir)/bootloader clean
