LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

module_name := gen_uniimg

source_files := main.c
CFLAGS += -g

include $(BUILD_BINARY)
