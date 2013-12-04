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

#define _VPP_ISR_C_

#include "thinvpp_module.h"
#include "thinvpp_scl.h"
#include "thinvpp_cpcb.h"
#include "vpp_cpcb_int.h"
#include "thinvpp_apifuncs.h"
#include "thinvpp_be.h"
#include "vpp_be_enc.h"
#include "thinvpp_cfg.h"
#include "thinvpp_isr.h"

#include "vpp.h"
#include "maddr.h"

#include "vpp_fe_deint.h"
#include "vpp_fe_dlr.h"
#include "vpp_frc_scl.h"
#include "api_avio_dhub.h"
#include "avioDhub.h"

#if BOOTLOADER_SHOWLOGO
                    /////////////////////////////////////////////////////
                    //                                                 //
                    //  Interrupt Service Routines for Berlin          //
                    //                                                 //
                    /////////////////////////////////////////////////////

#define DAC_REPLUG_DELAY    10
#define DAC_POWERDOWN_DELAY 100
#ifndef __LINUX_KERNEL__
#define virt_to_phys(x) (x)
#endif

static void updateVPSize(THINVPP_OBJ *vpp_obj, PLANE *plane)
{
    VPP_VP_CLIENT_SIZE vp_size;

    vp_size.ihRes = plane->actv_win.width;
    vp_size.ivRes = plane->actv_win.height;
    vp_size.IsProgressive = 1;

    FE_DEINT_SetVpDeintClientSize(vpp_obj, &vp_size, 0/* no training required */);
}

/********************************************************************************
 * FUNCTION: start all active plane data loader of a VPP FE channel
 * PARAMS: *vpp_obj - pointer to VPP object
 *         chanID - channel
 * RETURN: MV_THINVPP_OK - succeed
 *         MV_THINVPP_EBADPARAM - invalid parameter
 *         MV_THINVPP_EUNCONFIG - plane not activated yet
 *******************************************************************************/
static int startChannelDataLoader(THINVPP_OBJ *vpp_obj, int chanID)
{
    int cpcbID;
    CHAN *chan;

    cpcbID = CPCB_OF_CHAN(vpp_obj, chanID);
    chan = &vpp_obj->chan[chanID];

    if (chanID == CHAN_MAIN)
    { /* none AUX channel */
        VBUF_INFO * pinfo;
        unsigned active_left, active_width, frame_addr;
        unsigned dlr_id = VPP_FE_DLR_CHANNEL_MAIN;
        unsigned scl_id = VPP_FRC_SCL_MAIN;
        PLANE *plane = &vpp_obj->plane[PLANE_MAIN];
        VPP_FE_DLR_PLANE_DATA_FMT plane_fmt;
        VPP_SCL_CTRL scl_ctrl;
        VPP_FRC_RES frc_res;
        VPP_SCL_RES scl_res;
        VPP_FE_DLR_INPUT_RES res;
        int CropWpl;

        pinfo = plane->pinfo;
        frame_addr = (unsigned)pinfo->m_pbuf_start + (unsigned) pinfo->m_disp_offset;
        active_left = (pinfo->m_active_left) & ~1; //has to be even num
        active_width = ((pinfo->m_active_left + pinfo->m_active_width) & ~1) - active_left; //has to be even num

        if (plane->status == STATUS_ACTIVE) {

            chan->disp_win_attr.bgcolor = pinfo->bgcolor;
            chan->disp_win_attr.alpha = pinfo->alpha;

            plane->actv_win.x = active_left;
            plane->actv_win.y = pinfo->m_active_top;
            plane->actv_win.width  = active_width;
            plane->actv_win.height = pinfo->m_active_height;

            THINVPP_CPCB_SetPlaneAttribute(vpp_obj, cpcbID, chan->dvlayerID, chan->disp_win_attr.alpha, chan->disp_win_attr.bgcolor);

            plane_fmt.SrcFmt = SRCFMT_YUV422;
            plane_fmt.FmtOrder = ORDER_UYVY;
            FE_DLR_SetPlaneDataFmt(vpp_obj, dlr_id, &plane_fmt);

            FRC_SCL_ChopCtrl(vpp_obj, scl_id, 0);

            /*calculate crop WPL*/
            CropWpl = (((plane->actv_win.width + PIXEL_PER_BEAT_YUV_422 - 1) / PIXEL_PER_BEAT_YUV_422) << 16);
            plane->wpl = CropWpl >> 16;

            res.HRes = plane->actv_win.width;
            res.VRes = plane->actv_win.height;
            FE_DLR_SetPlaneSize(vpp_obj, dlr_id, &res, CropWpl);
            FE_DLR_SetVPDMX(vpp_obj, &res, 1, 0);
            FE_DLR_SetDummyTG(vpp_obj, &res, 1, 0);
            updateVPSize(vpp_obj, plane);

            scl_res.IHRes = plane->ref_win.width; /* resolution before scaling */
            scl_res.IVRes = plane->ref_win.height;
            scl_res.OHRes = chan->disp_win.width; /* resolution after scaling */
            scl_res.OVRes = chan->disp_win.height;
            scl_ctrl.HScalePos = VPP_SCL_HSCALE_AUTO;
            scl_ctrl.InputClr = 0; /* YUV input for none-OSD plane, ignore CSC in scaler */
            scl_ctrl.OsdInput = 0; /* non-OSD input (without alpha) */
            scl_ctrl.NLEn = 0; /* linear horizontal scaling mode */
            scl_ctrl.CenterFrac = 0; /* center value */
            scl_ctrl.BitMode = chan->scl_in_out_mode; /* input & output mode, depending on configuration */
            scl_ctrl.I565 = 0;
            scl_ctrl.HTapNum = 12;
            scl_ctrl.VTapNum = 8;
            scl_ctrl.DynamicLoad = 1; /* dynamically update coefficient table */
            scl_ctrl.ForceSel = 0;
            scl_ctrl.HSclMode = -1;
            scl_ctrl.VSclMode = -1;
            frc_res.HRes = scl_res.IHRes;
            frc_res.VRes = scl_res.IVRes;
            /* update scaling mode according to down-scaling ratio */
            plane->mode = MODE_INLINE;
            FRC_SCL_SetDeLrstDelay(vpp_obj, scl_id, 80);
            FRC_SCL_SetWorkMode(vpp_obj, scl_id, plane->mode);
            FRC_SCL_SetSclCtrlParams(vpp_obj, scl_id, &scl_res, &scl_ctrl);
            FRC_SCL_SetFrcParams(vpp_obj, scl_id, &frc_res);

            /*program for detail scaler*/
            FRC_SCL_SetDeLrstDelay(vpp_obj, VPP_FRC_SCL_DETAIL, 80);
            FRC_SCL_SetWorkMode(vpp_obj, VPP_FRC_SCL_DETAIL, plane->mode);
            FRC_SCL_SetSclCtrlParams(vpp_obj, VPP_FRC_SCL_DETAIL, &scl_res, &scl_ctrl);
            FRC_SCL_SetFrcParams(vpp_obj, VPP_FRC_SCL_DETAIL, &frc_res);

            THINVPP_CPCB_SetPlaneSourceWindow(vpp_obj, cpcbID, chan->dvlayerID,
                plane->actv_win.x, plane->actv_win.y, plane->actv_win.width, plane->actv_win.height);
            THINVPP_CPCB_SetPlaneBGWindow(vpp_obj, cpcbID, chan->dvlayerID,
                plane->ref_win.x, plane->ref_win.y, plane->ref_win.width, plane->ref_win.height);

            /* start data loader DMA to load display content */
            vpp_obj->dv[CPCB_1].curr_cpcb_vbi_dma_cfgQ->len += START_2DDMA(plane->dmaRdhubID, plane->dmaRID,
                (unsigned int)frame_addr, pinfo->m_buf_stride, plane->wpl*8, plane->actv_win.height,
                (unsigned int (*)[2])(vpp_obj->dv[CPCB_1].curr_cpcb_vbi_dma_cfgQ->addr+vpp_obj->dv[CPCB_1].curr_cpcb_vbi_dma_cfgQ->len*2));

            /* start read-back data loader */
            FE_DLR_EnableChannel(vpp_obj, dlr_id, 1);

            plane->status = STATUS_DISP;

        } else if (plane->status == STATUS_DISP) {
#if BOOTLOADER_ANIMLOGO
            if (chan->disp_win_attr.bgcolor != pinfo->bgcolor || chan->disp_win_attr.alpha != pinfo->alpha)
            {
                // update bgcolor and alpha
                chan->disp_win_attr.bgcolor = pinfo->bgcolor;
                chan->disp_win_attr.alpha = pinfo->alpha;
                THINVPP_CPCB_SetPlaneAttribute(vpp_obj, cpcbID, chan->dvlayerID, chan->disp_win_attr.alpha, chan->disp_win_attr.bgcolor);
            }

            if (plane->actv_win.x != active_left ||
                plane->actv_win.y != pinfo->m_active_top ||
                plane->actv_win.width  != active_width ||
                plane->actv_win.height != pinfo->m_active_height)
            {
                // update active window location and size
                plane->actv_win.x = active_left;
                plane->actv_win.y = pinfo->m_active_top;
                plane->actv_win.width  = active_width;
                plane->actv_win.height = pinfo->m_active_height;

                CropWpl = (((plane->actv_win.width + PIXEL_PER_BEAT_YUV_422 - 1) / PIXEL_PER_BEAT_YUV_422) << 16);
                plane->wpl = CropWpl >> 16;
                res.HRes = plane->actv_win.width;
                res.VRes = plane->actv_win.height;
                FE_DLR_SetPlaneSize(vpp_obj, dlr_id, &res, CropWpl);
                FE_DLR_SetVPDMX(vpp_obj, &res, 1, 0);
                FE_DLR_SetDummyTG(vpp_obj, &res, 1, 0);
                updateVPSize(vpp_obj, plane);

                THINVPP_CPCB_SetPlaneSourceWindow(vpp_obj, cpcbID, chan->dvlayerID,
                    plane->actv_win.x, plane->actv_win.y, plane->actv_win.width, plane->actv_win.height);
            }
#endif

            /* start data loader DMA to load display content */
            vpp_obj->dv[CPCB_1].curr_cpcb_vbi_dma_cfgQ->len += START_2DDMA(plane->dmaRdhubID, plane->dmaRID,
                (unsigned int)frame_addr, pinfo->m_buf_stride, plane->wpl*8, plane->actv_win.height,
                (unsigned int (*)[2])(vpp_obj->dv[CPCB_1].curr_cpcb_vbi_dma_cfgQ->addr+vpp_obj->dv[CPCB_1].curr_cpcb_vbi_dma_cfgQ->len*2));

            /* start read-back data loader */
            FE_DLR_EnableChannel(vpp_obj, dlr_id, 1);

        } else if (plane->status == STATUS_STOP) {

            THINVPP_CPCB_SetPlaneSourceWindow(vpp_obj, cpcbID, chan->dvlayerID, 0, 0, 0, 0);
            THINVPP_CPCB_SetPlaneBGWindow(vpp_obj, cpcbID, chan->dvlayerID, 0, 0, 0, 0);
            plane->status = STATUS_INACTIVE;

        }
    }

    return (MV_THINVPP_OK);
}

/* interrupt service routine for CPCB TG interrupt */
void THINVPP_CPCB_ISR_service(THINVPP_OBJ *vpp_obj, int cpcbID)
{
    DV *pDV;
    int params[2];

    if (!vpp_obj) return;

    pDV = &(vpp_obj->dv[cpcbID]);
    pDV->vbi_num ++; // VBI counter

    if(pDV->status == STATUS_ACTIVE){
        if (cpcbID == CPCB_1) {
            vpp_obj->dv[CPCB_1].curr_cpcb_vbi_bcm_cfgQ->len = 0;
            vpp_obj->dv[CPCB_1].curr_cpcb_vbi_dma_cfgQ->len = 0;
            vpp_obj->pVbiBcmBuf = vpp_obj->pVbiBcmBufCpcb[CPCB_1];
            THINVPP_BCMBUF_Select(vpp_obj->pVbiBcmBuf, CPCB_1);

            startChannelDataLoader(vpp_obj, CHAN_MAIN);

            THINVPP_CFGQ_To_CFGQ(vpp_obj->dv[CPCB_1].curr_cpcb_vbi_dma_cfgQ, vpp_obj->dv[CPCB_1].curr_cpcb_vbi_bcm_cfgQ);
            THINVPP_BCMBUF_To_CFGQ(vpp_obj->pVbiBcmBuf, vpp_obj->dv[CPCB_1].curr_cpcb_vbi_bcm_cfgQ);
            THINVPP_BCMDHUB_CFGQ_Commit(vpp_obj->dv[CPCB_1].curr_cpcb_vbi_bcm_cfgQ, CPCB_1);

            if (vpp_obj->pVbiBcmBufCpcb[CPCB_1] == &vpp_obj->vbi_bcm_buf[0])
                vpp_obj->pVbiBcmBufCpcb[CPCB_1] = &vpp_obj->vbi_bcm_buf[1];
            else
                vpp_obj->pVbiBcmBufCpcb[CPCB_1] = &vpp_obj->vbi_bcm_buf[0];

            if (vpp_obj->dv[CPCB_1].curr_cpcb_vbi_dma_cfgQ == &vpp_obj->dv[CPCB_1].vbi_dma_cfgQ[0])
                vpp_obj->dv[CPCB_1].curr_cpcb_vbi_dma_cfgQ = &(vpp_obj->dv[CPCB_1].vbi_dma_cfgQ[1]);
            else
                vpp_obj->dv[CPCB_1].curr_cpcb_vbi_dma_cfgQ = &(vpp_obj->dv[CPCB_1].vbi_dma_cfgQ[0]);

            if (vpp_obj->dv[CPCB_1].curr_cpcb_vbi_bcm_cfgQ == &vpp_obj->dv[CPCB_1].vbi_bcm_cfgQ[0])
                vpp_obj->dv[CPCB_1].curr_cpcb_vbi_bcm_cfgQ = &(vpp_obj->dv[CPCB_1].vbi_bcm_cfgQ[1]);
            else
                vpp_obj->dv[CPCB_1].curr_cpcb_vbi_bcm_cfgQ = &(vpp_obj->dv[CPCB_1].vbi_bcm_cfgQ[0]);
        }
    } else if (pDV->status == STATUS_STOP) {
        if (cpcbID == CPCB_1) {
            vpp_obj->dv[CPCB_1].curr_cpcb_vbi_bcm_cfgQ->len = 0;
            vpp_obj->dv[CPCB_1].curr_cpcb_vbi_dma_cfgQ->len = 0;
            vpp_obj->pVbiBcmBuf = vpp_obj->pVbiBcmBufCpcb[CPCB_1];
            THINVPP_BCMBUF_Select(vpp_obj->pVbiBcmBuf, CPCB_1);

            params[0] = CPCB_1;
            params[1] = RES_RESET;
            VPP_SetCPCBOutputResolution(vpp_obj, params);

            THINVPP_CFGQ_To_CFGQ(vpp_obj->dv[CPCB_1].curr_cpcb_vbi_dma_cfgQ, vpp_obj->dv[CPCB_1].curr_cpcb_vbi_bcm_cfgQ);
            THINVPP_BCMBUF_To_CFGQ(vpp_obj->pVbiBcmBuf, vpp_obj->dv[CPCB_1].curr_cpcb_vbi_bcm_cfgQ);
            THINVPP_BCMDHUB_CFGQ_Commit(vpp_obj->dv[CPCB_1].curr_cpcb_vbi_bcm_cfgQ, CPCB_1);

            if (vpp_obj->pVbiBcmBufCpcb[CPCB_1] == &vpp_obj->vbi_bcm_buf[0])
                vpp_obj->pVbiBcmBufCpcb[CPCB_1] = &vpp_obj->vbi_bcm_buf[1];
            else
                vpp_obj->pVbiBcmBufCpcb[CPCB_1] = &vpp_obj->vbi_bcm_buf[0];

            if (vpp_obj->dv[CPCB_1].curr_cpcb_vbi_dma_cfgQ == &vpp_obj->dv[CPCB_1].vbi_dma_cfgQ[0])
                vpp_obj->dv[CPCB_1].curr_cpcb_vbi_dma_cfgQ = &(vpp_obj->dv[CPCB_1].vbi_dma_cfgQ[1]);
            else
                vpp_obj->dv[CPCB_1].curr_cpcb_vbi_dma_cfgQ = &(vpp_obj->dv[CPCB_1].vbi_dma_cfgQ[0]);

            if (vpp_obj->dv[CPCB_1].curr_cpcb_vbi_bcm_cfgQ == &vpp_obj->dv[CPCB_1].vbi_bcm_cfgQ[0])
                vpp_obj->dv[CPCB_1].curr_cpcb_vbi_bcm_cfgQ = &(vpp_obj->dv[CPCB_1].vbi_bcm_cfgQ[1]);
            else
                vpp_obj->dv[CPCB_1].curr_cpcb_vbi_bcm_cfgQ = &(vpp_obj->dv[CPCB_1].vbi_bcm_cfgQ[0]);
        }
        return;
    }

    return;
}

/*************************************************************
 * FUNCTION: register VPP interrupt service routine
 * PARAMS: cpcbID - CPCB ID
 * RETURN: none
 *************************************************************/
void THINVPP_Enable_ISR_Interrupt(THINVPP_OBJ *vpp_obj, int cpcbID, int flag)
{
    if (cpcbID == CPCB_1){
        flag = flag? 1: 0;
        /* configure and enable CPCB0 interrupt */
        semaphore_cfg(vpp_obj->pSemHandle, avioDhubSemMap_vpp_vppCPCB0_intr, 1, 0);
        semaphore_pop(vpp_obj->pSemHandle, avioDhubSemMap_vpp_vppCPCB0_intr, 1);
        semaphore_clr_full(vpp_obj->pSemHandle, avioDhubSemMap_vpp_vppCPCB0_intr);
        semaphore_intr_enable(vpp_obj->pSemHandle, avioDhubSemMap_vpp_vppCPCB0_intr, 0/*empty*/, flag/*full*/, 0/*almost empty*/, 0/*almost full*/, 0/*cpu id*/);
    }

    return;
}

#endif //BOOTLOADER_SHOWLOGO
