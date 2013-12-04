#
# Clear out values of all variables used by templates.
#

module_name :=
module_type :=
module_dep :=
fs_source_dir :=
precopy_script :=
postcopy_script :=
reserved_blocks := 0

ARCH :=
LDFLAGS :=
CFLAGS :=
source_files :=

# Trim MAKEFILE_LIST so that $(call my-dir) doesn't need to
# # iterate over thousands of entries every time.
# # Leave the current makefile to make sure we don't break anything
# # that expects to be able to find the name of the current makefile.
MAKEFILE_LIST := $(lastword $(MAKEFILE_LIST))
