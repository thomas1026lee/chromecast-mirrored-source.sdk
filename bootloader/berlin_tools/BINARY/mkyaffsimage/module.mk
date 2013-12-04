LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

module_name := mkyaffsimage

include $(BUILD_BINARY)

#
# override rules inside binary.mk
#
.PHONY: $(module_target)
$(module_target):
	@$(MAKE) -C $(top_tool_dir)/BINARY/mkyaffsimage/utils
	@cp -v $(top_tool_dir)/BINARY/mkyaffsimage/utils/mkyaffs2image $@

clean-$(module_name)::
	@$(MAKE) -C $(top_tool_dir)/BINARY/mkyaffsimage/utils clean
