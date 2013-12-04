/********************************************************************************
*		Copyright 2007, MARVELL SEMICONDUCTOR, LTD.			*
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL.			*
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT	*
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE		*
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.	*
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,	*
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.	*
*										*
* MARVELL COMPRISES MARVELL TECHNOLOGY GROUP LTD. (MTGL) AND ITS SUBSIDIARIES,	*
* MARVELL INTERNATIONAL LTD. (MIL), MARVELL TECHNOLOGY, INC. (MTI), MARVELL	*
* SEMICONDUCTOR, INC. (MSI), MARVELL ASIA PTE LTD. (MAPL), MARVELL JAPAN K.K.	*
* (MJKK), MARVELL ISRAEL LTD. (MSIL).						*
*********************************************************************************/

///////////////////////////////////////////////////////////////////////////////
//! \file config.h
//! \brief some configuration of system, modified by berlin_config
//!
//! Puopose:
//!	save configuration files
///////////////////////////////////////////////////////////////////////////////
#ifndef	__CONFIG_H__
#define	__CONFIG_H__


#define	COLD_BOOT_CMD_ADDRESS		__cold_boot_cmd_address_am
#define BAD_BLOCK_TABLE_ADDRESS		__bad_block_table_address_am
#define IMAGE3_START_BLOCK_INDEX_NAND	__image3_start_block_index_nand_am
#define IMAGE3_START_BLOCK_INDEX_SPI	__image3_start_block_index_spi_am
#define IMAGE3_DDR_ADDRESS		0x2800000	
#define IMAGE2_3_TEXT_BASE		0x300000

#define CONFIG_CMD_USB

/*
 * USB/EHCI
 */
#ifdef CONFIG_CMD_USB
#define CONFIG_EHCI_DCACHE
#define CONFIG_USB_EHCI
#define CONFIG_EHCI_IS_TDI
#define CONFIG_USB_EHCI_BERLIN
#define CONFIG_USB_STORAGE
#define CONFIG_DOS_PARTITION
#endif

#define	__BYTE_ORDER	__LITTLE_ENDIAN

#endif //!< __CONFIG_H__
