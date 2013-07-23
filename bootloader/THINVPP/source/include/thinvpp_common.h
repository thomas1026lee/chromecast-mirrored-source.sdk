/*******************************************************************************
*                Copyright 2007, MARVELL SEMICONDUCTOR, LTD.                   *
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL.                      *
* NO RIGHTS ARE GRANTED HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT  *
* OF MARVELL OR ANY THIRD PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE        *
* DISCRETION TO REQUEST THAT THIS CODE BE IMMEDIATELY RETURNED TO MARVELL.     *
* THIS CODE IS PROVIDED "AS IS". MARVELL MAKES NO WARRANTIES, EXPRESSED,       *
* IMPLIED OR OTHERWISE, REGARDING ITS ACCURACY, COMPLETENESS OR PERFORMANCE.   *
*                                                                              *
* MARVELL COMPRISES MARVELL TECHNOLOGY GROUP LTD. (MTGL) AND ITS SUBSIDIARIES, *
* MARVELL INTERNATIONAL LTD. (MIL), MARVELL TECHNOLOGY, INC. (MTI), MARVELL    *
* SEMICONDUCTOR, INC. (MSI), MARVELL ASIA PTE LTD. (MAPL), MARVELL JAPAN K.K.  *
* (MJKK), MARVELL ISRAEL LTD. (MSIL).                                          *
*******************************************************************************/

#ifndef _THINVPP_COMMON_H_
#define _THINVPP_COMMON_H_

#include "com_type.h"

/* maximum number of VPP objects */
#ifdef MAX_NUM_OBJECTS
#error "duplicated MACRO definition of MAX_NUM_OBJECTS."
#else
#define MAX_NUM_OBJECTS  1
#endif

/* maximum number of integer parameters of callback functions */
#ifdef MAX_NUM_PARAMS
#error "duplicated MACRO definition of MAX_NUM_PARAMS."
#else
#define MAX_NUM_PARAMS    10
#endif

/* maximum number of frames in frame buffer queue */
#ifdef MAX_NUM_FRAMES
#error "duplicated MACRO definition of MAX_NUM_FRAMES"
#else
#define MAX_NUM_FRAMES    20
#endif

/* maximum number of commands in command queue */
#ifdef MAX_NUM_CMDS
#error "duplicated MACRO definition of MAX_NUM_CMDS"
#else
#define MAX_NUM_CMDS      100
#endif

/* maximum number of events */
#ifdef MAX_NUM_EVENTS
#error "duplicated MACRO definition of MAX_NUM_EVENTS"
#else
#define MAX_NUM_EVENTS    32
#endif

/* BCM buffer size */
#ifdef BCM_BUFFER_SIZE
#error "duplicated MACRO definition of BCM_BUFFER_SIZE"
#else
/* DV1_BCM_BUFFER_SIZE + DV2_BCM_BUFFER_SIZE + DV3_BCM_BUFFER_SIZE = BCM_BUFFER_SIZE */
#if (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
#define BCM_BUFFER_SIZE   0xA000//20480
#else // (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
#define BCM_BUFFER_SIZE   0x17700//96000
#define DV1_BCM_BUFFER_SIZE   0x7d00//32000
#define DV2_BCM_BUFFER_SIZE   0x7d00//32000
#define DV3_BCM_BUFFER_SIZE   0x7d00//32000
#define VDE_BCM_BUFFER_SIZE   0x800
#endif // (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
#endif

/* 3D De-interlacer field buffer size */
#ifdef DEINT_BUFFER_SIZE
#error "duplicated MACRO definition of DEINT_BUFFER_SIZE"
#else
//#define DEINT_BUFFER_SIZE   (1920*1082*26*3/16) // 3 1080I field (26-bit per pixel)
//#define DEINT_BUFFER_SIZE   (1920*1081*26*2/8) // 2 1080P frame (26-bit per pixel)
#define DEINT_BUFFER_SIZE   (1920*1081*26*3/8) // 6 1080I frame (26-bit per pixel)
#endif

/* offline scaling threshold ratio */
#ifdef OFFLINE_SCL_THRESH_VRATIO
#error "duplicated MACRO definition of OFFLINE_SCL_THRESH_VRATIO"
#else
#if (defined(BERLIN_A0_HACK) && (BERLIN_CHIP_VERSION < BERLIN_B_0))
#define OFFLINE_SCL_THRESH_VRATIO 1
#else
#define OFFLINE_SCL_THRESH_VRATIO 2
#endif
#endif

/* offline frame buffer size */
#ifdef OFFLINE_BUFFER_SIZE
#error "duplicated MACRO definition of OFFLINE_BUFFER_SIZE"
#else
#define OFFLINE_BUFFER_SIZE   (1920*1080*2/(OFFLINE_SCL_THRESH_VRATIO))  // 2 offline frames
//#define OFFLINE_BUFFER_SIZE   (1920*1080*2)  // 2 offline frames
#define AUX_OFFLINE_BUFFER_SIZE   (720*576*2)  // 2 aux offline frames
#endif

/* CC data queue size */
#ifdef CC_DATA_QUEUE_SIZE
#error "duplicated MACRO definition of CC_DATA_QUEUE_SIZE"
#else
#define CC_DATA_QUEUE_SIZE   (100)
#endif

/* WSS data queue size */
#ifdef WSS_DATA_QUEUE_SIZE
#error "duplicated MACRO definition of WSS_DATA_QUEUE_SIZE"
#else
#define WSS_DATA_QUEUE_SIZE   (10)
#endif

/* CGMS data queue size */
#ifdef CGMS_DATA_QUEUE_SIZE
#error "duplicated MACRO definition of CGMS_DATA_QUEUE_SIZE"
#else
#define CGMS_DATA_QUEUE_SIZE   (10)
#endif

/* TT data length per frame */
#ifdef TT_DATA_LEN_PER_FRAME
#error "duplicated MACRO definition of TT_DATA_LEN_PER_FRAME"
#else
#define TT_DATA_LEN_PER_FRAME   (40 * 4)
#endif

/* TT data queue size */
#ifdef TT_DATA_QUEUE_SIZE
#error "duplicated MACRO definition of TT_DATA_QUEUE_SIZE"
#else
#define TT_DATA_QUEUE_SIZE   (TT_DATA_LEN_PER_FRAME * 10)
#endif

/* DMA command buffer size */
#ifdef DMA_CMD_BUFFER_SIZE
#error "duplicated MACRO definition of DMA_CMD_BUFFER_SIZE"
#else
#if (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
#define DMA_CMD_BUFFER_SIZE   (20 * 8)
#else // (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
#define DMA_CMD_BUFFER_SIZE   (100 * 8)
#endif // (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
#endif

/* Hardware interface constants */

/* Register width in Berlin is 32 bits */
typedef unsigned int REG_WIDTH;
#define SIZE_OF_REG     sizeof(REG_WIDTH)

// Packed structure and packed array attributes
#ifndef PACKED_STRUCT
    #ifdef __LINUX__
        #define PACKED_STRUCT __attribute__((packed))
        #define PACKED_ARRAY  __attribute__((aligned(1)))
    #else
        #define PACKED_STRUCT
        #define PACKED_ARRAY
    #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* callback function type definition */
typedef int (*FUNCPTR)(void *, void *);
#ifdef __cplusplus
}
#endif

#endif
