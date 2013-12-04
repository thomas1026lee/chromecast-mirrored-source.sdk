LOCAL_PATH := $(call my-dir)

include $(scripts_dir)/clear_vars.mk

module_name:=gen_subimg_info

source_files := main.c crc32.c

CFLAGS += -g

include $(scripts_dir)/binary.mk
