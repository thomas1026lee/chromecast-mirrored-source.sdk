
read LINUX_PARAM_COMMON_PART_FINAL < $temp_dir/linux_param
echo LINUX_PARAM_COMMON_PART_FINAL: $LINUX_PARAM_COMMON_PART_FINAL
echo \#define LINUX_PARAM_COMMON_PART        \"$LINUX_PARAM_COMMON_PART_FINAL\" > $temp_dir/linux_param.h

quot=\"
if [ `expr substr "$LINUX_PARAM_NORMAL_BOOT_PART" 1 1` = \" ]; then
	quot=
fi
echo \#define LINUX_PARAM_NORMAL_BOOT_PART  $quot$LINUX_PARAM_NORMAL_BOOT_PART$quot >> $temp_dir/linux_param.h

quot=\"
if [ `expr substr "$LINUX_PARAM_RECOVERY_BOOT_PART" 1 1` = \" ]; then
	quot=
fi
echo \#define LINUX_PARAM_RECOVERY_BOOT_PART $quot$LINUX_PARAM_RECOVERY_BOOT_PART$quot >> $temp_dir/linux_param.h

echo \#define MEMORY_SIZE \(1024 \* 1024 \* 1024\) >> $temp_dir/linux_param.h
