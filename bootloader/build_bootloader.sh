#!/bin/bash
set -o errtrace
trap 'echo Fatal error: script $0 aborting at line $LINENO, command \"$BASH_COMMAND\" returned $?; exit 1' ERR

if (( $# < 1 ))
then
  echo "Usage: $0 <board_name>"
  exit 2
fi

# Mapping of board name and BOARD_ macros
# defined in board_config.h
declare -A board_cfgmacro_map
board_cfgmacro_map=([bg2proto]=1\
                    [eureka-b1]=2\
                    [eureka-b2]=3\
                    [eureka-b3]=4)

# Fixed config location used by Marvell build tools
CONFIG_PATH=$(readlink -f berlin_config/config)
CONFIGS_DIR=$(readlink -f berlin_config/configs)
BLOCK0_DIR=$(readlink -f berlin_tools/SUB_IMAGE/block0)
BOOTLD_DIR=$(readlink -f berlin_tools/SUB_IMAGE/bootloader)
BINARY_DIR=$(readlink -f berlin_tools/BINARY)

cpu_num=$(grep -c processor /proc/cpuinfo)

board_name=$1
board_cfg=$(readlink -f ${CONFIGS_DIR}/${board_name}.cfg)
curr_dir=$(readlink -f $(dirname $0))

out_dir=bootloader.build.${board_name}
files_to_clean=( \
  berlin_tools/.temp \
  ${out_dir}
)

# Some Makefiles do refer to variables defined in board_cfg
source ${board_cfg}

# cleanup
for f in ${files_to_clean[@]}
do
  rm -fr $f
done

# set config
rm -f ${CONFIG_PATH}
cp ${board_cfg} ${CONFIG_PATH}

# build block0
cd ${BLOCK0_DIR}
make CPU_TYPE=${CPU_TYPE} -j ${cpu_name}
cd ${curr_dir}

# build bootloader
cd ${BOOTLD_DIR}
make CFG_BOARD_NAME=${board_cfgmacro_map[$board_name]} -j ${cpu_name}
cd ${curr_dir}

# Gather all the relevant artifacts
bootl_unpadded=bootloader.${board_name}
mkdir ${out_dir} || true

# subimages used for building uNAND image
cp berlin_tools/.temp/bootloader.subimg ${out_dir}
cp berlin_tools/.temp/bootloader.subimg.info ${out_dir}
cp berlin_tools/.temp/block0.subimg ${out_dir}
cp berlin_tools/.temp/block0.subimg.info ${out_dir}

# unpadded bootloader for OTA
cp berlin_tools/bootloader/bootloader_nopadding.img ${out_dir}/${bootl_unpadded}

# build image tools in case they were updated
for tool in gen_subimg_info gen_uniimg mkyaffsimage; do
  cd ${BINARY_DIR}/${tool}
  make
done
