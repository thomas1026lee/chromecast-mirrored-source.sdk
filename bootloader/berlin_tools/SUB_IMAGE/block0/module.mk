LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

module_name := block0

include $(BUILD_SUB_IMAGE)

BLOCK0_IMG_SRCPATH := $(top_tool_dir)/mk_nandblock0_image/block0.img

.PHONY: $(temp_dir)/$(module_name).subimg
$(temp_dir)/$(module_name).subimg:
	@$(MAKE) -C $(top_tool_dir)/mk_nandblock0_image
	@diff $(BLOCK0_IMG_SRCPATH) $@ && exit 0 || cp -v $(BLOCK0_IMG_SRCPATH) $@

clean-$(module_name)::
	@$(MAKE) -C $(top_tool_dir)/mk_nandblock0_image clean
