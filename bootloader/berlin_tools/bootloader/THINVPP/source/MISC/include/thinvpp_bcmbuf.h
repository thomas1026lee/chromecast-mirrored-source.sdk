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

#ifndef _BCMBUF_H_
#define _BCMBUF_H_

#ifdef __cplusplus
extern "C" {
#endif

#if BOOTLOADER_SHOWLOGO
typedef struct DHUB_CFGQ_T {
    int base_addr;
    int *addr;
    int len;
} DHUB_CFGQ;
#endif

/* structure of a buffer descriptor */
/* a buffer descriptor contains the pointers of the entire buffer and sub-buffers */
typedef struct BCMBUF_T {
    int addr;
    unsigned int *head;          // head of total BCM buffer
    unsigned int *dv1_head;      // head of BCM sub-buffer used for CPCB0
    unsigned int *dv3_head;      // head of BCM sub-buffer used for CPCB2
    unsigned int *tail;          // tail of the buffer, used for checking wrap around
    unsigned int *writer;        // write pointer of queue, update with shadow_tail with commit
    int size;                    // size of total BCM buffer
    int subID;                   // sub-buffer ID currently in use
} BCMBUF;

/******* register programming buffer APIs **************/
/***************************************************************
 * FUNCTION: allocate register programming buffer
 * PARAMS: *buf - pointer to a register programming buffer
 *       : size - size of the buffer to allocate
 *       :      - (should be a multiple of 4)
 * RETURN:  1 - succeed
 *          0 - failed to initialize a BCM buffer
 ****************************************************************/
int THINVPP_BCMBUF_Create(BCMBUF *pbcmbuf,int size);

/***************************************************************
 * FUNCTION: free register programming buffer
 * PARAMS: *buf - pointer to a register programming buffer
 * RETURN:  1 - succeed
 *          0 - failed to initialize a BCM buffer
 ****************************************************************/
int THINVPP_BCMBUF_Destroy(BCMBUF *pbcmbuf);

/***************************************************************
 * FUNCTION: reset a register programming buffer
 * PARAMS: *buf - pointer to a register programming buffer
 * RETURN:  1 - succeed
 *          0 - failed to initialize a BCM buffer
 ****************************************************************/
int THINVPP_BCMBUF_Reset(BCMBUF *pbcmbuf);

/*********************************************************
 * FUNCTION: selest BCM sub-buffer to use
 * PARAMS: *buf - pointer to the buffer descriptor
 *         subID - DV_1, DV_2, DV_3
 ********************************************************/
void THINVPP_BCMBUF_Select(BCMBUF *pbcmbuf, int subID);

/*********************************************************
 * FUNCTION: write register address (4bytes) and value (4bytes) to the buffer
 * PARAMS: *buf - pointer to the buffer descriptor
 *               address - address of the register to be set
 *               value - the value to be written into the register
 * RETURN: 1 - succeed
 *               0 - register programming buffer is full
 ********************************************************/
int THINVPP_BCMBUF_Write(BCMBUF *pbcmbuf, unsigned int address, unsigned int value);

/*********************************************************************
 * FUNCTION: do the hardware transmission
 * PARAMS: block - 0: return without waiting for transaction finishing
 *                 1: return after waiting for transaction finishing
 ********************************************************************/
void THINVPP_BCMBUF_HardwareTrans(BCMBUF *pbcmbuf, int block);


#if BOOTLOADER_SHOWLOGO
int THINVPP_CFGQ_Create(DHUB_CFGQ *cfgQ, int size);
int THINVPP_CFGQ_Destroy(DHUB_CFGQ *cfgQ);

/*******************************************************************************
 * FUNCTION: commit cfgQ which contains BCM DHUB programming info to interrupt service routine
 * PARAMS: *cfgQ - cfgQ
 *         cpcbID - cpcb ID which this cmdQ belongs to
 *         intrType - interrupt type which this cmdQ belongs to: 0 - VBI, 1 - VDE
 * NOTE: this API is only called from VBI/VDE ISR.
 *******************************************************************************/
int THINVPP_BCMDHUB_CFGQ_Commit(DHUB_CFGQ *cfgQ, int cpcbID);

/*********************************************************************
 * FUNCTION: send a BCM BUF info to a BCM cfgQ
 * PARAMS: *pbcmbuf - pointer to the BCMBUF
 *         *cfgQ - target BCM cfgQ
 * NOTE: this API is only called from VBI/VDE ISR.
 ********************************************************************/
int THINVPP_BCMBUF_To_CFGQ(BCMBUF *pbcmbuf, DHUB_CFGQ *cfgQ);

/*********************************************************************
 * FUNCTION: send a BCM cfgQ info to a BCM cfgQ
 * PARAMS: src_cfgQ - pointer to the source BCM cfgQ
 *         *cfgQ - target BCM cfgQ
 * NOTE: this API is only called from VBI/VDE ISR.
 ********************************************************************/
void THINVPP_CFGQ_To_CFGQ(DHUB_CFGQ *src_cfgQ, DHUB_CFGQ *cfgQ);
#endif

#ifdef __cplusplus
}
#endif

#endif
