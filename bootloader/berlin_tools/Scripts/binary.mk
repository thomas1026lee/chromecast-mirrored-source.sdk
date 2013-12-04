
module_target := $(temp_dir)/$(module_name)

common_dep := Makefile $(scripts_dir)/binary.mk $(scripts_dir)/common.mk
module_objs := $(addprefix $(LOCAL_PATH)/, $(source_files:.c=.o))

#$(info )
#$(info binary.mk included)
#$(info LOCAL_PATH='$(LOCAL_PATH)')
#$(info module_name='$(module_name)')
#$(info module_objs='$(module_objs)')
#$(info module_target='$(module_target)')

CFLAGS += -I$(top_tool_dir)/Common/include -I.

ifeq ($(CONFIG_GPT), y)
CFLAGS += -DCONFIG_GPT
endif
ifneq ($(strip $(module_objs)),)
$(module_objs): PRIVATE_ARCH := $(ARCH)
$(module_objs): PRIVATE_CFLAGS := $(CFLAGS)
$(module_objs): %.o: %.c $(common_dep)
	$(PRIVATE_ARCH)gcc $(PRIVATE_CFLAGS) -c -MD -o $@ $(filter %.c, $^)
#$(info include $(module_objs:.o=.d))
-include $(module_objs:.o=.d)
endif

#$(info binary.mk adds a rule)
#$(info $(module_target) - $(module_objs) $(common_dep))

$(module_target): PRIVATE_ARCH := $(ARCH)
$(module_target): PRIVATE_LDFLAGS := $(LDFLAGS)
$(module_target): $(module_objs) $(common_dep)
	@echo linking $@ with arch=$(PRIVATE_ARCH) ldflags=$(PRIVATE_LDFLAGS)
	$(PRIVATE_ARCH)gcc $(PRIVATE_LDFLAGS) -o $@ $(filter %.o, $^)

.PHONY: clean-$(module_name)
clean-$(module_name): PRIVATE_MODULE_NAME := $(module_name)
clean-$(module_name): PRIVATE_CLEAN_FILES := $(LOCAL_PATH)/*.o $(LOCAL_PATH)/*.d
clean-$(module_name)::
	-@rm -rf $(PRIVATE_CLEAN_FILES)

include $(BUILD_COMMON)
