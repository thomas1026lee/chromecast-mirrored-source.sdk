LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

module_name := parse_pt

source_files := parse_pt.c
CFLAGS += -g

include $(BUILD_BINARY)
