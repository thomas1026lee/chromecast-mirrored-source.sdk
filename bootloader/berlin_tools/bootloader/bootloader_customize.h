#include "common.h"
#include "chipctrl_const.h"
#ifdef CONFIG_SM
#include "sm_const.h"
#include "SysMgr.h"
#endif // CONFIG_SM
#include "figo.h"
#include "config.h"
#include "nand_priv.h"
#include "util.h"
#include "cold_linuxparam.h"
#include "SystemManager.h"
#include "system_config.h"
#include "global.h"
#include "galois_io.h"
#include "apb_perf_base.h"
#include "apb_gpio.h"
#include "galois_speed.h"
#include "pin_settings.h"
#include "wol.h"

#include "image3_header.h"
#include "version_table.h"
#if ANDROID_BOOT
#include "android/bootimg.h"
#include "android/flash_ts.h"
#endif

#include "nflash_drv.h"

#if defined(DEBUG)
#define debug_printf(a...) lgpl_printf(a)
#else
#define debug_printf(a...)
#endif

extern unsigned int uiBoot;
extern unsigned int uiWarmDown_2_Linux_Addr;

void bootloader_customize_initialize_postprocess(unsigned int boot_state);
void bootloader_customize_load_image_preprocess(unsigned int boot_state);
void bootloader_customize_load_image_postprocess(unsigned int boot_state);
void bootloader_customize_response_to_sm_preprocess(unsigned int boot_state);
void bootloader_customize_start_kernel_preprocess(unsigned int boot_state);
