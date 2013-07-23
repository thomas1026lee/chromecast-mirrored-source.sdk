/* Board configuration information.
 *
 * Copyright (C) 2012 Google, Inc.
 */
#ifndef BOARD_CONFIG_H__
#define BOARD_CONFIG_H__

#define BOARD_BG2PROTO  1
#define BOARD_EUREKA_B1 2
#define BOARD_EUREKA_B2 3
#define BOARD_EUREKA_B3 4

/* bg2proto uses Samsung K9GBG08U0A MLC
 * 4GB, 1M block, 8K page
 */
#if CFG_BOARD_NAME == BOARD_BG2PROTO
#define BOARD_NAME            "bg2proto"
#define FTS_BLK_START          9
#define FTS_BLK_CNT            7
#define KERNEL_BLK_START      16
#define KERNEL_BLK_CNT        16
#define RECOVERY_BLK_START    32
#define RECOVERY_BLK_CNT      60
#define BOARD_MEM_SIZE        (512 * 1024 * 1024)
#define FORCE_CORE_VOLT       950
#elif CFG_BOARD_NAME == BOARD_EUREKA_B1
#define BOARD_NAME            "eureka-b1"
#define BOARD_REV              1
#define FTS_BLK_START          9
#define FTS_BLK_CNT            7
#define KERNEL_BLK_START      16
#define KERNEL_BLK_CNT        16
#define RECOVERY_BLK_START    32
#define RECOVERY_BLK_CNT      60
#define BACKUPSYS_BLK_START   (RECOVERY_BLK_START + RECOVERY_BLK_CNT)
#define BACKUPSYS_BLK_CNT     80
#define BOARD_MEM_SIZE        (512 * 1024 * 1024)
#define BUTTON_GPIO           6
#define LED_WHITE_GPIO        17
#define LED_RED_GPIO          18
#define FORCE_CORE_VOLT       0
#elif CFG_BOARD_NAME == BOARD_EUREKA_B2
#define BOARD_NAME            "eureka-b2"
#define BOARD_REV              2
#define FTS_BLK_START         2016
#define FTS_BLK_CNT            8
#define KERNEL_BLK_START       9
#define KERNEL_BLK_CNT        16
#define RECOVERY_BLK_START    1872
#define RECOVERY_BLK_CNT      48
#define BACKUPSYS_BLK_START   (RECOVERY_BLK_START + RECOVERY_BLK_CNT)
#define BACKUPSYS_BLK_CNT     96
#define BOARD_MEM_SIZE        (512 * 1024 * 1024)
#define BUTTON_GPIO           6
#define LED_WHITE_GPIO        17
#define LED_RED_GPIO          18
#define FORCE_CORE_VOLT       0
#elif CFG_BOARD_NAME == BOARD_EUREKA_B3
#define BOARD_NAME            "eureka-b3"
#define BOARD_REV              3
#define FTS_BLK_START         2016
#define FTS_BLK_CNT            8
#define KERNEL_BLK_START       9
#define KERNEL_BLK_CNT        16
#define RECOVERY_BLK_START    1872
#define RECOVERY_BLK_CNT      48
#define BACKUPSYS_BLK_START   (RECOVERY_BLK_START + RECOVERY_BLK_CNT)
#define BACKUPSYS_BLK_CNT     96
#define BOARD_MEM_SIZE        (512 * 1024 * 1024)
#define BUTTON_GPIO           6
#define LED_WHITE_GPIO        17
#define LED_RED_GPIO          18
#define FORCE_CORE_VOLT       0
#else
#error "CFG_BOARD_NAME must be set"
#endif //CFG_BOARD_NAME

#endif //BOARD_CONFIG_H__
