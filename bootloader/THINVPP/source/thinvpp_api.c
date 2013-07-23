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

#define _THINVPP_API_C_

#include "memmap.h"
#include "galois_io.h"
#include "global.h"
#include "thinvpp_module.h"
#include "thinvpp_apifuncs.h"
#include "thinvpp_cfg.h"
#include "thinvpp_scl.h"
#include "thinvpp_cpcb.h"
#include "thinvpp_be.h"
#include "thinvpp_isr.h"
#if BOOTLOADER_FASTLOGO
#include "avpll.h"
#endif

#if BOOTLOADER_SHOWLOGO
#include "vpp_be_hdmitx.h"
#ifdef __LINUX_KERNEL__
#include <linux/mm.h>
#include <asm/page.h>
#include <linux/delay.h>
#endif
#include "maddr.h"
#endif
#include "vpp.h"

#define bTST(x, b) (((x) >> (b)) & 1)

#if BOOTLOADER_FASTLOGO
void delay_10ns(int delay)
{
    int i;

    for (i=0; i<delay; i++) {
    __asm__ __volatile__ ("MOV r0, r0"	"\n\t" :::"r0");
    __asm__ __volatile__ ("MOV r0, r0"	"\n\t" :::"r0");
    __asm__ __volatile__ ("MOV r0, r0"	"\n\t" :::"r0");
    __asm__ __volatile__ ("MOV r0, r0"	"\n\t" :::"r0");
    __asm__ __volatile__ ("MOV r0, r0"	"\n\t" :::"r0");
    __asm__ __volatile__ ("MOV r0, r0"	"\n\t" :::"r0");
    __asm__ __volatile__ ("MOV r0, r0"	"\n\t" :::"r0");
    __asm__ __volatile__ ("MOV r0, r0"	"\n\t" :::"r0");
    __asm__ __volatile__ ("MOV r0, r0"	"\n\t" :::"r0");
    __asm__ __volatile__ ("MOV r0, r0"	"\n\t" :::"r0");
   }
}

static int GetVCOFreq(int resID, int bit_depth)
{
    if (resID == RES_RESET)
        return (-1);

    int avpll_freq_index, deep_color_index;

    if (m_resinfo_table[resID].freq <= 25200) {
        avpll_freq_index = 0;
    } else if ((m_resinfo_table[resID].freq == 27000) || (m_resinfo_table[resID].freq == 27027)) {
        avpll_freq_index = 1;
    } else if ((m_resinfo_table[resID].freq == 74250) || (m_resinfo_table[resID].freq == 74176)) {
        avpll_freq_index = 3;
    } else if ((m_resinfo_table[resID].freq == 148500) || (m_resinfo_table[resID].freq == 148352)) {
        avpll_freq_index = 5;
    } else
        return (-1);

    if (bit_depth == OUTPUT_BIT_DEPTH_12BIT)
        deep_color_index = 2;
    else if (bit_depth == OUTPUT_BIT_DEPTH_10BIT)
        deep_color_index = 1;
    else if (bit_depth == OUTPUT_BIT_DEPTH_8BIT)
        deep_color_index = 0;
    else
        return (-1);

    return (diag_getVCOFreq(deep_color_index, avpll_freq_index));
}

static int NeedAVPLL_PPM1K(int resID)
{
    int frame_rate;
    int ret;

    frame_rate = m_resinfo_table[resID].frame_rate;
    if (m_resinfo_table[resID].type == TYPE_SD) {
        /* for SD resolution */
        if ((frame_rate == FRAME_RATE_59P94) || (frame_rate == FRAME_RATE_50))
            ret = 0;
        else
            ret = 1;
    } else {
        /* for HD resolution */
        if ((frame_rate == FRAME_RATE_59P94) || (frame_rate == FRAME_RATE_29P97) || (frame_rate == FRAME_RATE_23P98) || (frame_rate == FRAME_RATE_119P88) || (frame_rate == FRAME_RATE_47P96))
            ret = 0;
        else
            ret = 1;;
    }

    return ret;
}
#endif

#if BOOTLOADER_SHOWLOGO
static void VPP_dhub_sem_clear(void)
{
    int instat;

    HDL_semaphore *pSemHandle = thinvpp_obj->pSemHandle;

    instat = semaphore_chk_full(pSemHandle, -1);

    if (bTST(instat, avioDhubSemMap_vpp_vppCPCB0_intr)){
        semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppCPCB0_intr, 1);
        semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppCPCB0_intr);
    }else if (bTST(instat, avioDhubSemMap_vpp_vppCPCB2_intr)){
        semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppCPCB2_intr, 1);
        semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppCPCB2_intr);
    }

    return;
}
#endif //BOOTLOADER_SHOWLOGO


/***********************************************
 * FUNCTION: create a VPP object
 * PARAMS: base_addr - VPP object base address
 *         *handle - pointer to object handle
 * RETURN: MV_THINVPP_OK - succeed
 *         MV_THINVPP_EUNCONFIG - not initialized
 ***********************************************/
int MV_THINVPP_Create(int base_addr, unsigned options)
{
    if (!(thinvpp_obj = (THINVPP_OBJ *)THINVPP_MALLOC(sizeof(THINVPP_OBJ)))){
        return (MV_THINVPP_ENOMEM);
    }

    THINVPP_MEMSET(thinvpp_obj, 0, sizeof(THINVPP_OBJ));

    thinvpp_obj->options = options;
    thinvpp_obj->base_addr = base_addr;

    /* create VBI BCM buffer */
    if (THINVPP_BCMBUF_Create(&(thinvpp_obj->vbi_bcm_buf[0]), BCM_BUFFER_SIZE) != MV_THINVPP_OK){
        THINVPP_FREE(thinvpp_obj);
        return (MV_THINVPP_ENOMEM);
    }

#if BOOTLOADER_SHOWLOGO
    if (thinvpp_obj->options & THINVPP_OPTION_SHOWLOGO)
    {
        if (THINVPP_BCMBUF_Create(&(thinvpp_obj->vbi_bcm_buf[1]), BCM_BUFFER_SIZE) != MV_THINVPP_OK){
            THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[0]));
            THINVPP_FREE(thinvpp_obj);
            return (MV_THINVPP_ENOMEM);
        }


        if (THINVPP_CFGQ_Create(&(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[0]), DMA_CMD_BUFFER_SIZE) != MV_THINVPP_OK) {
            THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[0]));
            THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[1]));
            THINVPP_FREE(thinvpp_obj);
            return (MV_THINVPP_ENOMEM);
        }
        if (THINVPP_CFGQ_Create(&(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[1]), DMA_CMD_BUFFER_SIZE) != MV_THINVPP_OK) {
            THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[0]));
            THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[1]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[0]));
            THINVPP_FREE(thinvpp_obj);
            return (MV_THINVPP_ENOMEM);
        }

        if (THINVPP_CFGQ_Create(&(thinvpp_obj->dv[CPCB_3].vbi_dma_cfgQ[0]), DMA_CMD_BUFFER_SIZE) != MV_THINVPP_OK) {
            THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[0]));
            THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[1]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[0]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[1]));
            THINVPP_FREE(thinvpp_obj);
            return (MV_THINVPP_ENOMEM);
        }
        if (THINVPP_CFGQ_Create(&(thinvpp_obj->dv[CPCB_3].vbi_dma_cfgQ[1]), DMA_CMD_BUFFER_SIZE) != MV_THINVPP_OK) {
            THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[0]));
            THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[1]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[0]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[1]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_3].vbi_dma_cfgQ[0]));
            THINVPP_FREE(thinvpp_obj);
            return (MV_THINVPP_ENOMEM);
        }

        if (THINVPP_CFGQ_Create(&(thinvpp_obj->dv[CPCB_1].vbi_bcm_cfgQ[0]), DMA_CMD_BUFFER_SIZE) != MV_THINVPP_OK) {
            THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[0]));
            THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[1]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[0]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[1]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_3].vbi_dma_cfgQ[0]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_3].vbi_dma_cfgQ[1]));
            THINVPP_FREE(thinvpp_obj);
            return (MV_THINVPP_ENOMEM);
        }
        if (THINVPP_CFGQ_Create(&(thinvpp_obj->dv[CPCB_1].vbi_bcm_cfgQ[1]), DMA_CMD_BUFFER_SIZE) != MV_THINVPP_OK) {
            THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[0]));
            THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[1]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[0]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[1]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_3].vbi_dma_cfgQ[0]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_3].vbi_dma_cfgQ[1]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_bcm_cfgQ[0]));
            THINVPP_FREE(thinvpp_obj);
            return (MV_THINVPP_ENOMEM);
        }

        if (THINVPP_CFGQ_Create(&(thinvpp_obj->dv[CPCB_3].vbi_bcm_cfgQ[0]), DMA_CMD_BUFFER_SIZE) != MV_THINVPP_OK) {
            THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[0]));
            THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[1]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[0]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[1]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_3].vbi_dma_cfgQ[0]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_3].vbi_dma_cfgQ[1]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_bcm_cfgQ[0]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_bcm_cfgQ[1]));
            THINVPP_FREE(thinvpp_obj);
            return (MV_THINVPP_ENOMEM);
        }
        if (THINVPP_CFGQ_Create(&(thinvpp_obj->dv[CPCB_3].vbi_bcm_cfgQ[1]), DMA_CMD_BUFFER_SIZE) != MV_THINVPP_OK) {
            THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[0]));
            THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[1]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[0]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[1]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_3].vbi_dma_cfgQ[0]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_3].vbi_dma_cfgQ[1]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_bcm_cfgQ[0]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_bcm_cfgQ[1]));
            THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_3].vbi_bcm_cfgQ[0]));
            THINVPP_FREE(thinvpp_obj);
            return (MV_THINVPP_ENOMEM);
        }
    }
#endif //BOOTLOADER_SHOWLOGO

    return (MV_THINVPP_OK);
}

/***********************************************
 * FUNCTION: destroy a VPP object
 * PARAMS: handle - VPP object handle
 * RETURN: MV_THINVPP_OK - succeed
 *         MV_THINVPP_EUNCONFIG - not initialized
 *         MV_THINVPP_ENODEV - no device
 *         MV_THINVPP_ENOMEM - no memory
 ***********************************************/
int MV_THINVPP_Destroy(void)
{
    if (thinvpp_obj == NULL)
        return (MV_THINVPP_ENODEV);

    /* free BCM buffer memory */
    THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[0]));
#if BOOTLOADER_SHOWLOGO
    if (thinvpp_obj->options & THINVPP_OPTION_SHOWLOGO)
    {
        THINVPP_BCMBUF_Destroy(&(thinvpp_obj->vbi_bcm_buf[1]));

        THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[0]));
        THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[1]));
        THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_3].vbi_dma_cfgQ[0]));
        THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_3].vbi_dma_cfgQ[1]));
        THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_bcm_cfgQ[0]));
        THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_1].vbi_bcm_cfgQ[1]));
        THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_3].vbi_bcm_cfgQ[0]));
        THINVPP_CFGQ_Destroy(&(thinvpp_obj->dv[CPCB_3].vbi_bcm_cfgQ[1]));
    }
#endif //BOOTLOADER_SHOWLOGO

    /* free vpp object memory */
    THINVPP_FREE(thinvpp_obj);
    thinvpp_obj = NULL;

    return (MV_THINVPP_OK);
}

/***************************************
 * FUNCTION: VPP reset
 * INPUT: NONE
 * RETURN: NONE
 **************************************/
int MV_THINVPP_Reset(void)
{
    int i;

    if (!thinvpp_obj)
        return (MV_THINVPP_ENODEV);

    // reset VPP object variable
    thinvpp_obj->status = STATUS_INACTIVE;

    /* reset planes */
    for (i=FIRST_PLANE; i<MAX_NUM_PLANES; i++){
        thinvpp_obj->plane[i].status = STATUS_INACTIVE;
        thinvpp_obj->plane[i].mode = -1; // invalid
        thinvpp_obj->plane[i].srcfmt = -1; // invalid
        thinvpp_obj->plane[i].order = -1; // invalid

        thinvpp_obj->plane[i].actv_win.x = 0;
        thinvpp_obj->plane[i].actv_win.y = 0;
        thinvpp_obj->plane[i].actv_win.width = 0;
        thinvpp_obj->plane[i].actv_win.height = 0;

        thinvpp_obj->plane[i].ref_win = thinvpp_obj->plane[i].actv_win;
    }

    /* reset channels */
    for (i=FIRST_CHAN; i<MAX_NUM_CHANS; i++) {
        thinvpp_obj->chan[i].status = STATUS_INACTIVE;
        thinvpp_obj->chan[i].dvID = -1; // invalid
        thinvpp_obj->chan[i].dvlayerID = -1; // invalid
        thinvpp_obj->chan[i].zorder = -1; // invalid

        thinvpp_obj->chan[i].disp_win.x = 0;
        thinvpp_obj->chan[i].disp_win.y = 0;
        thinvpp_obj->chan[i].disp_win.width = 0;
        thinvpp_obj->chan[i].disp_win.height = 0;
    }

    /* reset DVs */
    for (i=FIRST_CPCB; i<MAX_NUM_CPCBS; i++) {
        thinvpp_obj->dv[i].status = STATUS_INACTIVE;
        thinvpp_obj->dv[i].output_res = RES_INVALID; // invalid
        thinvpp_obj->dv[i].num_of_vouts = 0;
        thinvpp_obj->dv[i].vbi_num = 0;
    }

    /* reset VOUTs */
    for (i=FIRST_VOUT; i<MAX_NUM_VOUTS; i++) {
        thinvpp_obj->vout[i].status = STATUS_INACTIVE;
        thinvpp_obj->vout[i].dvID = -1; // invalid
    }

    /* reset VBI BCM buffer */
    THINVPP_BCMBUF_Reset(&thinvpp_obj->vbi_bcm_buf[0]);
    thinvpp_obj->pVbiBcmBuf = &(thinvpp_obj->vbi_bcm_buf[0]);
#if BOOTLOADER_SHOWLOGO
    if (thinvpp_obj->options & THINVPP_OPTION_SHOWLOGO)
    {
        THINVPP_BCMBUF_Reset(&thinvpp_obj->vbi_bcm_buf[1]);
        thinvpp_obj->pVbiBcmBufCpcb[CPCB_1] = &(thinvpp_obj->vbi_bcm_buf[0]);
        thinvpp_obj->pVbiBcmBufCpcb[CPCB_3] = &(thinvpp_obj->vbi_bcm_buf[0]);

        thinvpp_obj->dv[CPCB_1].curr_cpcb_vbi_dma_cfgQ = &(thinvpp_obj->dv[CPCB_1].vbi_dma_cfgQ[0]);
        thinvpp_obj->dv[CPCB_3].curr_cpcb_vbi_dma_cfgQ = &(thinvpp_obj->dv[CPCB_3].vbi_dma_cfgQ[0]);
        thinvpp_obj->dv[CPCB_1].curr_cpcb_vbi_bcm_cfgQ = &(thinvpp_obj->dv[CPCB_1].vbi_bcm_cfgQ[0]);
        thinvpp_obj->dv[CPCB_3].curr_cpcb_vbi_bcm_cfgQ = &(thinvpp_obj->dv[CPCB_3].vbi_bcm_cfgQ[0]);
    }
#endif //BOOTLOADER_SHOWLOGO

    /* reset dHub cmdQ */
#if (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
    for (i = 0; i < avioDhubChMap_vpp_TT_R; i++)
#else // (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
    for (i = 0; i < avioDhubChMap_vpp_SPDIF_W; i++)
#endif // (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
        thinvpp_obj->dhub_cmdQ[i] = 0;

    /* select BCM sub-buffer to dump register settings */
    THINVPP_BCMBUF_Select(thinvpp_obj->pVbiBcmBuf, -1);

    THINVPP_SCL_Reset(thinvpp_obj);
    THINVPP_CPCB_Reset(thinvpp_obj);
#if BOOTLOADER_FASTLOGO
    THINVPP_BE_Reset(thinvpp_obj);

    /* start BCM engine to program VPP registers */
    THINVPP_BCMBUF_HardwareTrans(thinvpp_obj->pVbiBcmBuf, 1/*block*/);
#endif

    return (MV_THINVPP_OK);
}

/***************************************
 * FUNCTION: VPP profile configuration
 * INPUT: NONE
 * RETURN: NONE
 **************************************/
int MV_THINVPP_Config(void)
{
    int i;

    if (!thinvpp_obj)
        return (MV_THINVPP_ENODEV);

    /* VPP module has been configured */
    if (thinvpp_obj->status == STATUS_ACTIVE)
        return (MV_THINVPP_OK);

    thinvpp_obj->pSemHandle = dhub_semaphore(&VPP_dhubHandle.dhub);

    /* config planes */
    for (i=FIRST_PLANE; i<MAX_NUM_PLANES; i++){

        /* set plane's DMA channel ID */
        if (i == PLANE_MAIN){
            thinvpp_obj->plane[i].dmaRID = avioDhubChMap_vpp_MV_R; // inline read DMA
            thinvpp_obj->plane[i].dmaRdhubID = (int)(&VPP_dhubHandle);
            thinvpp_obj->plane[i].offline_dmaWID = avioDhubChMap_vpp_MV_FRC_W; // offline write-back DMA
            thinvpp_obj->plane[i].offline_dmaWdhubID = (int)(&VPP_dhubHandle);
            thinvpp_obj->plane[i].offline_dmaRID = avioDhubChMap_vpp_MV_FRC_R; // offline readd-back DMA
            thinvpp_obj->plane[i].offline_dmaRdhubID = (int)(&VPP_dhubHandle);
#if (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
        } else if (i == PLANE_PIP){
            thinvpp_obj->plane[i].dmaRID = avioDhubChMap_vpp_PIP_R; // inline read DMA
            thinvpp_obj->plane[i].dmaRdhubID = (int)(&VPP_dhubHandle);
            thinvpp_obj->plane[i].offline_dmaWID = avioDhubChMap_vpp_PIP_FRC_W; // offline write-back DMA
            thinvpp_obj->plane[i].offline_dmaWdhubID = (int)(&VPP_dhubHandle);
            thinvpp_obj->plane[i].offline_dmaRID = avioDhubChMap_vpp_PIP_FRC_R; // offline readd-back DMA
            thinvpp_obj->plane[i].offline_dmaRdhubID = (int)(&VPP_dhubHandle);
        } else if (i == PLANE_AUX){
            thinvpp_obj->plane[i].offline_dmaWID = avioDhubChMap_vpp_AUX_FRC_W; // AUXoffline write-back DMA
            thinvpp_obj->plane[i].offline_dmaWdhubID = (int)(&VPP_dhubHandle);
            thinvpp_obj->plane[i].offline_dmaRID = avioDhubChMap_vpp_AUX_FRC_R; // AUX offline read-back DMA
            thinvpp_obj->plane[i].offline_dmaRdhubID = (int)(&VPP_dhubHandle);
#endif // (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
        }

    } // <- config FE planes

    /* config channels */
#if LOGO_ENABLE_MAIN
    thinvpp_obj->chan[CHAN_MAIN].dvID = CPCB_1; //#if LOGO_ENABLE_MAIN
#endif
#if LOGO_ENABLE_PIP
    thinvpp_obj->chan[CHAN_PIP].dvID = CPCB_1;
#endif
    thinvpp_obj->chan[CHAN_AUX].dvID = CPCB_3;

#if LOGO_ENABLE_MAIN
    thinvpp_obj->chan[CHAN_MAIN].zorder = CPCB_ZORDER_7; //#if LOGO_ENABLE_MAIN
#endif
#if LOGO_ENABLE_PIP
    thinvpp_obj->chan[CHAN_PIP].zorder = CPCB_ZORDER_2;
#endif
    thinvpp_obj->chan[CHAN_AUX].zorder = CPCB_ZORDER_1;

    // for B0, channel connection with CPCB is fixed
#if LOGO_ENABLE_MAIN
    thinvpp_obj->chan[CHAN_MAIN].dvlayerID = CPCB1_PLANE_1; //#if LOGO_ENABLE_MAIN
#endif
#if LOGO_ENABLE_PIP
    thinvpp_obj->chan[CHAN_PIP].dvlayerID = CPCB1_PLANE_2;
#endif
    thinvpp_obj->chan[CHAN_AUX].dvlayerID = CPCB1_PLANE_1; // PLANE-1 of CPCB-2

#if LOGO_ENABLE_MAIN
  #if LOGO_ENABLE_PIP || (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
     thinvpp_obj->dv[CPCB_1].num_of_chans = 2;
     thinvpp_obj->dv[CPCB_1].chanID[0] = CHAN_MAIN;
     thinvpp_obj->dv[CPCB_1].chanID[1] = CHAN_PIP;
  #else
     thinvpp_obj->dv[CPCB_1].num_of_chans = 1;
     thinvpp_obj->dv[CPCB_1].chanID[0] = CHAN_MAIN;
  #endif
#else
  #if LOGO_ENABLE_PIP
     thinvpp_obj->dv[CPCB_1].num_of_chans = 1;
     thinvpp_obj->dv[CPCB_1].chanID[0] = CHAN_PIP;
  #else
     thinvpp_obj->dv[CPCB_1].num_of_chans = 0;
  #endif
#endif
     thinvpp_obj->dv[CPCB_1].num_of_vouts = 2;
     thinvpp_obj->dv[CPCB_1].voutID[0] = VOUT_HDMI;
     thinvpp_obj->dv[CPCB_1].voutID[1] = VOUT_HD;

#if (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
     thinvpp_obj->dv[CPCB_3].num_of_chans = 1;
     thinvpp_obj->dv[CPCB_3].chanID[0] = CHAN_AUX;
     thinvpp_obj->dv[CPCB_3].num_of_vouts = 1;
     thinvpp_obj->dv[CPCB_3].voutID[0] = VOUT_SD;
#else // (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
     thinvpp_obj->dv[CPCB_3].num_of_chans = 0;
#endif // (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)

    /* config VOUTs */
    thinvpp_obj->vout[VOUT_HDMI].dvID = CPCB_1;
    thinvpp_obj->vout[VOUT_HD].dvID = CPCB_1;
    thinvpp_obj->vout[VOUT_SD].dvID = CPCB_3;

    /* select BCM sub-buffer to dump register settings */
    THINVPP_BCMBUF_Select(thinvpp_obj->pVbiBcmBuf, -1);

    THINVPP_SCL_Config(thinvpp_obj);
    THINVPP_CPCB_Config(thinvpp_obj);
#if BOOTLOADER_FASTLOGO
    THINVPP_BE_Config(thinvpp_obj);

    /* start BCM engine to program VPP registers */
    THINVPP_BCMBUF_HardwareTrans(thinvpp_obj->pVbiBcmBuf, 1/*block*/);
#endif

#if BOOTLOADER_SHOWLOGO
    if (thinvpp_obj->options & THINVPP_OPTION_SHOWLOGO)
    {
        BCM_SCHED_SetMux(BCM_SCHED_Q0, 0); /* CPCB0 VBI -> Q0 */
        BCM_SCHED_SetMux(BCM_SCHED_Q1, 1); /* CPCB1 VBI -> Q1 */
        BCM_SCHED_SetMux(BCM_SCHED_Q2, 2); /* CPCB2 VBI -> Q2 */
        BCM_SCHED_SetMux(BCM_SCHED_Q3, 3); /* CPCB0 VDE -> Q3 */
        BCM_SCHED_SetMux(BCM_SCHED_Q4, 4); /* CPCB1 VDE -> Q4 */
        BCM_SCHED_SetMux(BCM_SCHED_Q5, 5); /* CPCB2 VDE -> Q5 */
    }
#endif //BOOTLOADER_SHOWLOGO

    /* set VPP module to be configured */
    thinvpp_obj->status = STATUS_ACTIVE;
    return (MV_THINVPP_OK);
}

/*******************************************************************
 * FUNCTION: set CPCB or DV output resolution
 * INPUT: cpcbID - CPCB(for Berlin) or DV(for Galois) id
 *        resID - id of output resolution
 *        bit_depth - HDMI deep color bit depth
 * RETURN: MV_THINVPP_OK - SUCCEED
 *         MV_EBADPARAM - invalid parameters
 *         MV_EUNCONFIG - VPP not configured or plane not active
 *         MV_EFRAMEQFULL - frame queue is full
 * Note: this function has to be called before enabling a plane
 *       which belongs to that CPCB or DV.
 *******************************************************************/
int MV_THINVPP_SetCPCBOutputResolution(int cpcbID, int resID, int bit_depth)
{
    int params[3];
#if BOOTLOADER_FASTLOGO
    int vco_freq;
    int ppm1k_en;
    int     avpll_freq_index;
    int     deep_color_index;
    float     ovsmp_index;
#endif

    if (!thinvpp_obj)
        return (MV_THINVPP_ENODEV);

    if (resID<FIRST_RES || resID>=MAX_NUM_RESS)
        return (MV_THINVPP_EBADPARAM);

    if (thinvpp_obj->status == STATUS_INACTIVE){
        /* VPP module is not configured */
        return (MV_THINVPP_EUNCONFIG);
    }

    if (cpcbID<FIRST_CPCB || cpcbID>=MAX_NUM_CPCBS)
        return (MV_THINVPP_EBADPARAM);

#if BOOTLOADER_FASTLOGO
    vco_freq = GetVCOFreq(resID, bit_depth);
#endif

    if (resID != RES_RESET) {

#if BOOTLOADER_FASTLOGO
        /* set CPCB new resolution */
        if (m_resinfo_table[resID].freq <= 25200) {
            avpll_freq_index = 0;
            ovsmp_index = 4.0;
        } else if ((m_resinfo_table[resID].freq == 27000) || (m_resinfo_table[resID].freq == 27027)) {
            avpll_freq_index = 1;
            ovsmp_index = 4.0;
        } else if ((m_resinfo_table[resID].freq == 74250) || (m_resinfo_table[resID].freq == 74176)) {
            avpll_freq_index = 3;
            ovsmp_index = 2.0;
        } else if ((m_resinfo_table[resID].freq == 148500) || (m_resinfo_table[resID].freq == 148352)) {
            avpll_freq_index = 5;
            ovsmp_index = 1.0;
        } else {
            return (MV_THINVPP_EBADPARAM);
        }

        if (bit_depth == OUTPUT_BIT_DEPTH_12BIT)
            deep_color_index = 2;
        else if (bit_depth == OUTPUT_BIT_DEPTH_10BIT)
            deep_color_index = 1;
        else if (bit_depth == OUTPUT_BIT_DEPTH_8BIT)
            deep_color_index = 0;
        else
            return (MV_THINVPP_EBADPARAM);

        ppm1k_en = NeedAVPLL_PPM1K(resID);

        if (cpcbID == CPCB_1)
#if (BERLIN_CHIP_VERSION >= BERLIN_BG2_A0)
            diag_videoFreq_A(avpll_freq_index, deep_color_index, ppm1k_en, ovsmp_index, 6);
#else
            diag_videoFreq_A(avpll_freq_index, deep_color_index, ppm1k_en, ovsmp_index, 1);
#endif
        else if (cpcbID == CPCB_2)
#if (BERLIN_CHIP_VERSION >= BERLIN_BG2_A0)
            diag_videoFreq_A(avpll_freq_index, deep_color_index, ppm1k_en, ovsmp_index, 5);
#else
            diag_videoFreq_A(avpll_freq_index, deep_color_index, ppm1k_en, ovsmp_index, 2);
#endif
        else if (cpcbID == CPCB_3)
#if (BERLIN_CHIP_VERSION >= BERLIN_BG2_A0)
            diag_videoFreq_B(avpll_freq_index, 2 /* always set to 12-bit for AUX */, ppm1k_en, ovsmp_index, 6);
#else
            diag_videoFreq_B(avpll_freq_index, 2 /* always set to 12-bit for AUX */, ppm1k_en, ovsmp_index, 1);
#endif

        delay_10ns(2000000);
#endif

        /* select BCM sub-buffer to dump register settings */
        THINVPP_BCMBUF_Select(thinvpp_obj->pVbiBcmBuf, cpcbID);

        /* set CPCB resolution */
        params[0] = cpcbID;
        params[1] = resID;
        VPP_SetCPCBOutputResolution(thinvpp_obj, params);

#if BOOTLOADER_FASTLOGO
        /* start BCM engine to program VPP registers */
        THINVPP_BCMBUF_HardwareTrans(thinvpp_obj->pVbiBcmBuf, 1/*block*/);

        if (cpcbID == CPCB_1)
            MV_THINVPP_SetHdmiVideoFmt(OUTPUT_COLOR_FMT_RGB888, bit_depth, 1);
#endif

        /* set DV status to active */
        thinvpp_obj->dv[cpcbID].status = STATUS_ACTIVE;
    }

    return (MV_THINVPP_OK);
}

#if BOOTLOADER_SHOWLOGO
int MV_THINVPP_IsCPCBActive(int cpcbID)
{
    int vtotal;

    if (cpcbID == CPCB_1) {
        vtotal = (GLB_REG_READ32(MEMMAP_VPP_REG_BASE+(CPCB0_VT_H << 2)) & 0x0ff);
        vtotal <<= 8;
        vtotal |= (GLB_REG_READ32(MEMMAP_VPP_REG_BASE+(CPCB0_VT_L << 2)) & 0x0ff);
    } else if (cpcbID == CPCB_3) {
        vtotal = (GLB_REG_READ32(MEMMAP_VPP_REG_BASE+(CPCB2_VT_H << 2)) & 0x0ff);
        vtotal <<= 8;
        vtotal |= (GLB_REG_READ32(MEMMAP_VPP_REG_BASE+(CPCB2_VT_L << 2)) & 0x0ff);
    } else
        vtotal = 0;

    return (vtotal);
}




int MV_THINVPP_SetMainRefWindow(const VPP_WIN *win)
{
    if (!thinvpp_obj)
        return (MV_THINVPP_ENODEV);

    if (!win)
        return (MV_THINVPP_EBADPARAM);

#if LOGO_ENABLE_MAIN
    thinvpp_obj->plane[PLANE_MAIN].ref_win = *win;
#else
    thinvpp_obj->plane[PLANE_PIP].ref_win = *win;
#endif
    return (MV_THINVPP_OK);
}

int MV_THINVPP_SetMainDisplayFrame(VBUF_INFO *pinfo)
{
    PLANE *plane;

    if (!thinvpp_obj)
        return (MV_THINVPP_ENODEV);

    if (!pinfo)
        return (MV_THINVPP_EBADPARAM);

#if LOGO_ENABLE_MAIN
    plane = &(thinvpp_obj->plane[PLANE_MAIN]);
#else
    plane = &(thinvpp_obj->plane[PLANE_PIP]);
#endif
    plane->pinfo = pinfo;

    plane->actv_win.x = pinfo->m_active_left;
    plane->actv_win.y = pinfo->m_active_top;
    plane->actv_win.width  = pinfo->m_active_width;
    plane->actv_win.height = pinfo->m_active_height;

    return (MV_THINVPP_OK);
}

/******************************************************************************
 * FUNCTION: open a window of a video/graphics plane for display.
 *           the window is defined in end display resolution
 * INPUT: planeID - id of a video/grahpics plane
 *        *win - pointer to a vpp window struct
 *        *attr - pointer to a vpp window attribute struct
 * RETURN: MV_THINVPP_OK - SUCCEED
 *         MV_EBADPARAM - invalid parameters
 *         MV_EUNCONFIG - VPP not configured
 *         MV_EUNSUPPORT - plane not connected in configuration
 *         MV_ECMDQFULL - command queue is full
 ******************************************************************************/
int MV_THINVPP_OpenDispWindow(int planeID, const VPP_WIN *win, const VPP_WIN_ATTR *attr)
{
    int chanID;
    int cpcbID;
    PLANE *plane;
    CHAN *chan;

    if (!thinvpp_obj)
        return (MV_THINVPP_ENODEV);

    if (planeID<FIRST_PLANE || planeID>=MAX_NUM_PLANES)
        return (MV_THINVPP_EBADPARAM);

    if (!win)
        return (MV_THINVPP_EBADPARAM);

    if ((win->width<=0) || (win->height<=0))
        return (MV_THINVPP_EBADPARAM);

    if (thinvpp_obj->status == STATUS_INACTIVE){
        /* VPP module is not configured */
        return (MV_THINVPP_EUNCONFIG);
    }

    plane = &(thinvpp_obj->plane[planeID]);
    chanID = planeID;
    chan = &(thinvpp_obj->chan[chanID]);
    cpcbID = chan->dvID;

    /* update video/graphics channel display window */
    chan->disp_win.x = win->x;
    chan->disp_win.y = win->y;
    chan->disp_win.width = win->width;
    chan->disp_win.height = win->height;

    if (plane->ref_win.width == 0)
        plane->ref_win.width = chan->disp_win.width;
    if (plane->ref_win.height == 0)
        plane->ref_win.height = chan->disp_win.height;

    if (attr){
        chan->disp_win_attr.bgcolor = attr->bgcolor;
        chan->disp_win_attr.alpha = attr->alpha;
    } else {
        chan->disp_win_attr.bgcolor = DEFAULT_BGCOLOR;
        chan->disp_win_attr.alpha = DEFAULT_ALPHA;
    }
    /* set video/graphics plane & channel in active status */
    plane->status = STATUS_ACTIVE;
    chan->status = STATUS_ACTIVE;

    THINVPP_Enable_ISR_Interrupt(thinvpp_obj, cpcbID, 1);
    VPP_dhub_sem_clear();

    return (MV_THINVPP_OK);
}

int MV_THINVPP_CloseDispWindow(void)
{
    if (!thinvpp_obj)
        return (MV_THINVPP_ENODEV);

    /* wait for CPCB TG reset done */
    thinvpp_obj->plane[PLANE_MAIN].status = STATUS_STOP;
    thinvpp_obj->plane[PLANE_PIP].status = STATUS_STOP;
    thinvpp_obj->plane[PLANE_AUX].status = STATUS_STOP;

#if LOGO_ENABLE_MAIN
    while(thinvpp_obj->plane[PLANE_MAIN].status != STATUS_INACTIVE);
#else
    while(thinvpp_obj->plane[PLANE_PIP].status != STATUS_INACTIVE);
#endif
#if (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
    while(thinvpp_obj->plane[PLANE_AUX].status != STATUS_INACTIVE);
#endif

    return (MV_THINVPP_OK);
}

int MV_THINVPP_Stop(void)
{
    if (!thinvpp_obj)
        return (MV_THINVPP_ENODEV);

    /* wait for CPCB TG reset done */
#if 1
    BE_HDMIPHY_SetDAMP(thinvpp_obj, 0);
    BE_HDMIPHY_SetEAMP(thinvpp_obj, 0);

    thinvpp_obj->dv[CPCB_1].status = STATUS_STOP;
    thinvpp_obj->dv[CPCB_3].status = STATUS_STOP;
//    if (thinvpp_obj->plane[PLANE_MAIN].status != STATUS_INACTIVE)
    while(thinvpp_obj->dv[CPCB_1].status != STATUS_INACTIVE);
    while(thinvpp_obj->dv[CPCB_1].vbi_num < 1);

#if (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
    while(thinvpp_obj->dv[CPCB_3].status != STATUS_INACTIVE);
    while(thinvpp_obj->dv[CPCB_3].vbi_num < 1);
#endif
#else
    thinvpp_obj->dv[CPCB_1].status = STATUS_INACTIVE;
    thinvpp_obj->dv[CPCB_3].status = STATUS_INACTIVE;
#endif

    // disable CPCB VBI interrupts
    THINVPP_Enable_ISR_Interrupt(thinvpp_obj, CPCB_1, 0);
    THINVPP_Enable_ISR_Interrupt(thinvpp_obj, CPCB_3, 0);
    VPP_dhub_sem_clear();

    return (MV_THINVPP_OK);
}
#endif //BOOTLOADER_SHOWLOGO

#if BOOTLOADER_FASTLOGO
/********************************************************************************
 * FUNCTION: Set Hdmi Video format
 * INPUT: color_fmt - color format (RGB, YCbCr 444, 422)
 *      : bit_depth - 8/10/12 bit color
 *      : pixel_rept - 1/2/4 repetitions of pixel
 * RETURN: MV_THINVPP_OK - SUCCEED
 *         MV_EBADPARAM - invalid parameters
 *         MV_EUNCONFIG - VPP not configured
 *         MV_THINVPP_ENODEV - no device
 *         MV_EUNSUPPORT - channel not connected in configuration
 *         MV_THINVPP_EBADCALL - channel not connected to DV1
 *         MV_ECMDQFULL - command queue is full
 ********************************************************************************/
int MV_THINVPP_SetHdmiVideoFmt(int color_fmt, int bit_depth, int pixel_rept)
{
    int     cpcbID;
    int     resID;
    int     retVal;
    int     instat;
#if !defined(FPGA)
#if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
    int     avpll_freq_index;
    int     deep_color_index;
    float   freq_factor;
#else
    float   freq_factor;
    float   avpll_freq;
#endif
    int ppm1k_en;
#endif

    if (!thinvpp_obj)
        return (MV_THINVPP_ENODEV);

    if ((cpcbID = CPCB_OF_VOUT(thinvpp_obj, VOUT_HDMI)) == CPCB_INVALID) {
          /* Output is not connected to any DV in configuration */
        return (MV_THINVPP_EUNSUPPORT);
    }

    /* Configure AVPLL */
    resID = thinvpp_obj->dv[cpcbID].output_res;
    if (resID == RES_INVALID)
        return (MV_THINVPP_EBADPARAM);

    if ((m_resinfo_table[resID].freq == 25200) || (m_resinfo_table[resID].freq == 25175)) {
        avpll_freq_index = 0;
    } else if ((m_resinfo_table[resID].freq == 27000) || (m_resinfo_table[resID].freq == 27027)) {
        avpll_freq_index = 1;
        if (pixel_rept == 2) {
            if (m_resinfo_table[resID].scan != SCAN_INTERLACED)
                avpll_freq_index = 2;
        } else if (pixel_rept == 4) {
            if (m_resinfo_table[resID].scan != SCAN_INTERLACED)
                avpll_freq_index = 4;
            else
                avpll_freq_index = 2;
        }
    } else if ((m_resinfo_table[resID].freq == 74250) || (m_resinfo_table[resID].freq == 74176)) {
        avpll_freq_index = 3;
    } else if ((m_resinfo_table[resID].freq == 148500) || (m_resinfo_table[resID].freq == 148352)) {
        if (resID == RES_1080I60 || resID == RES_1080I5994 || resID == RES_1080I50)
            avpll_freq_index = 3;
        else
            avpll_freq_index = 5;
    } else {
        return (MV_THINVPP_EBADPARAM);
    }

    if (bit_depth == OUTPUT_BIT_DEPTH_12BIT) {
        deep_color_index = 2;
        freq_factor = 15.0;
    } else if (bit_depth == OUTPUT_BIT_DEPTH_10BIT) {
        deep_color_index = 1;
        freq_factor = 12.5;
    } else if (bit_depth == OUTPUT_BIT_DEPTH_8BIT) {
        deep_color_index = 0;
        freq_factor = 10.0;
    } else {
        return (MV_THINVPP_EBADPARAM);
    }

    ppm1k_en = NeedAVPLL_PPM1K(resID);

    diag_videoFreq_A(avpll_freq_index, deep_color_index, ppm1k_en, freq_factor, 7);

    delay_10ns(2000000);

    /* Config video format */
    if (MV_THINVPP_OK != (retVal = THINVPP_BE_ConfigHdmiVideoFmt (thinvpp_obj, color_fmt, bit_depth, pixel_rept)))
        return retVal;

    semaphore_pop(thinvpp_obj->pSemHandle, avioDhubSemMap_vpp_vppCPCB0_intr, 1);
    semaphore_clr_full(thinvpp_obj->pSemHandle, avioDhubSemMap_vpp_vppCPCB0_intr);
    do {
        instat = semaphore_chk_full(thinvpp_obj->pSemHandle, -1);
    } while (!(bTST(instat, avioDhubSemMap_vpp_vppCPCB0_intr)));


    THINVPP_BCMBUF_Select(thinvpp_obj->pVbiBcmBuf, cpcbID);

    THINVPP_BE_SetHdmiVideoFmt(thinvpp_obj);

    /* start BCM engine to program VPP registers */
    THINVPP_BCMBUF_HardwareTrans(thinvpp_obj->pVbiBcmBuf, 1/*block*/);

    return (MV_THINVPP_OK);
}
#endif


