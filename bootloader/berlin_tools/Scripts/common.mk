#SHELL=/bin/bash

#
# define module target
#

ifeq ($(module_name),)
$(error empty module name)
endif

all_modules += $(module_name)
all_module_targets += $(module_target)
all_module_clean_targets += clean-$(module_name)

#
# rule for each module
#
#$(info common.mk adds a rule)
#$(info "$(module_name): $(module_target)")
#$(info)

.PHONY: $(module_name)
$(module_name): $(module_target)
	@echo GEN $@
	@echo

clean-$(module_name): PRIVATE_MODULE_NAME := $(module_name)
clean-$(module_name): PRIVATE_MODULE_TARGET := $(module_target)
clean-$(module_name)::
	@echo "Clean: $(PRIVATE_MODULE_NAME)"
	-@rm -f $(PRIVATE_MODULE_TARGET)
