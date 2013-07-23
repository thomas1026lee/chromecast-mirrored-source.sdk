
#include "galois_io.h"
#include "thinvpp_api.h"
#include "thinvpp_isr.h"
#include "api_dhub.h"
#include "api_avio_dhub.h"
#include "memmap.h"
#include "gicDiag.h"
#include "pic.h"

#include "logo_data.h"

#ifndef bTST
#define bTST(x, b) (((x) >> (b)) & 1)
#endif


#ifndef BFM_HOST_Bus_Write32
#define BFM_HOST_Bus_Write32 GA_REG_WORD32_WRITE
#endif

volatile unsigned logo_isr_count;

#ifndef  EMMC_BOOT
extern void  IRQ_Handler(void);
static void InstallHighHandler(unsigned int handlerAddr, unsigned int vector)
{
	//load PC with content at current PC + 0x38,
	//since current PC is vector+0x8, the handler should be at vector+0x40
	BFM_HOST_Bus_Write32(vector, 0xE59FF038);
	BFM_HOST_Bus_Write32(vector+0x40, handlerAddr);
}

static void EnableIRQ(void)
{
    __asm__
    __volatile__(
        "MRS        r0, CPSR"       "\n\t"
        "BIC        r0, r0, #0x80"  "\n\t"
        "MSR        CPSR, r0"       "\n\t"
        :
        :
        :"r0"
    );
}
#endif

static UNSG32 getMPid(void)
{
	__asm__
    __volatile__(
		"MRC		p15,0,r0,c0,c0,5"	"\n\t"
		"ANDS		r0, r0, #0xf"		"\n\t"
		:
		:
		:"r0"
	) ;
}


static void showlogo_init_irq(void)
{
    int i;
    unsigned cpu_id=0;
	//printf("\n%s\n", __FUNCTION__) ;

    cpu_id = getMPid();

#ifdef  EMMC_BOOT
    //bootloader.c should have called do_emmcinit() to setup the irq
#else
	//printf("%s, InstallHighHandler\n", __FUNCTION__) ;
	InstallHighHandler((unsigned int)IRQ_Handler, MEMMAP_HIGH_EXCEPT_IRQ_REG);

	//GIC distribution interface is only needed to be initialized once
	//but the CPU interface needs to be initalized by both CPU cores
	//CPU interface registers are banked (same address)
	//So we will have both first and second CPU to call the same funtion
	//printf("%s, initMPGIC\n", __FUNCTION__) ;
	initMPGIC();

	//printf("%s, EnableIRQ\n", __FUNCTION__) ;
	EnableIRQ();
#endif

	printf("%s, Enable IRQ_dHubIntrAvio0(0x%x) for cpu %u\n", __FUNCTION__, MP_BERLIN_INTR_ID(IRQ_dHubIntrAvio0), cpu_id) ;
	diag_GICSetInt(cpu_id, MP_BERLIN_INTR_ID(IRQ_dHubIntrAvio0), INT_ENABLE);
}


static VBUF_INFO vbuf;
int showlogo_start(void *buf)
{
    VPP_WIN_ATTR showlogo_attr;
    VPP_WIN showlogo_win;
    void *start;
    unsigned size;

	printf("[SHOWLOGO] start\n");
    logo_isr_count = 0;
    showlogo_init_irq();

	//printf("%s, SetMainDisplay\n", __FUNCTION__) ;

    vbuf.m_pbuf_start    = buf; // base address of the frame buffer;
    vbuf.m_disp_offset   = 0;
    vbuf.m_active_left   = (720 - yuv_logo_width)/2; // at center
    vbuf.m_active_top    = (480 - yuv_logo_height)/2; // at center

    // adjust the following three fields to reflect the actual logo dimensions
    vbuf.m_buf_stride    = yuv_logo_stride;
    vbuf.m_active_width  = yuv_logo_width;
    vbuf.m_active_height = yuv_logo_height;

    vbuf.alpha   = 255;
    // Format of bgcolor: YUYV
    // This matches the background color of our boot logo.
    vbuf.bgcolor = 0x00800080;

    start = vbuf.m_pbuf_start;
    size = vbuf.m_buf_stride * vbuf.m_active_height;
    UtilMemCpy(start, yuv_logo, size);
    flush_dcache_range(start, (unsigned int *)(((int)(start))+(size)));

    MV_THINVPP_SetMainDisplayFrame(&vbuf);

    showlogo_win.x = 0;
    showlogo_win.y = 0;
    showlogo_win.width  = 720;
    showlogo_win.height = 480;
    showlogo_attr.bgcolor = vbuf.bgcolor;
    showlogo_attr.alpha   = vbuf.alpha;
	MV_THINVPP_OpenDispWindow(PLANE_MAIN, &showlogo_win, &showlogo_attr);

	return 0;
}

int showlogo_stop()
{
	//printf("%s enter, isr_count=%u\n", __FUNCTION__, logo_isr_count) ;

#if BOOTLOADER_ANIMLOGO
    {
        // force to show logo
        int count = ((logo_isr_count/100)+1)*100;
        while (logo_isr_count < 1000)
        {
            if (count < logo_isr_count)
            {
                count += 100;
                printf("%s, isr_count=%u\n", __FUNCTION__, logo_isr_count) ;
            }
        }
    }
#endif
	MV_THINVPP_CloseDispWindow();
	MV_THINVPP_Stop();
	MV_THINVPP_Destroy();

    MV_THINVPP_Create(MEMMAP_VPP_REG_BASE, THINVPP_OPTION_DEFAULT);
    MV_THINVPP_Reset();
    MV_THINVPP_Config();
    MV_THINVPP_SetCPCBOutputResolution(CPCB_1, RES_525P5994, OUTPUT_BIT_DEPTH_8BIT);

	printf("[SHOWLOGO] stopped\n");
    return 0;
}

void showlogo_isr(void)
{
    int instat;
    HDL_semaphore *pSemHandle;

    ++logo_isr_count;

    /* VPP interrupt handling  */
    pSemHandle = dhub_semaphore(&VPP_dhubHandle.dhub);
    instat = semaphore_chk_full(pSemHandle, -1);

    if (bTST(instat, avioDhubSemMap_vpp_vppCPCB0_intr))
    {
#if BOOTLOADER_ANIMLOGO
        static int anim, anim_count;

        if (logo_isr_count == 0)
            anim = anim_count = 0;

        switch (anim) {
        default:
                anim = 0;
        case 0:
            // fade out animation
            if (vbuf.alpha > 1)
                vbuf.alpha -= 2;
            else
            {
                vbuf.alpha = 0;
                ++anim;
                printf("%s.%d anim %d\n", __FUNCTION__, logo_isr_count, anim) ;
            }

            // bgcolor animation
            vbuf.bgcolor = 0x00800080 | ((((255-vbuf.alpha)<<16) + (255-vbuf.alpha))<<8);
            break;

        case 1:
            // fade in animation
            if (vbuf.alpha<255)
                vbuf.alpha += 2;
            else
            {
                vbuf.alpha = 255;
                ++anim;
                printf("%s.%d anim %d\n", __FUNCTION__, logo_isr_count, anim) ;
            }

            // bgcolor animation
            vbuf.bgcolor = 0x00800080 | ((((255-vbuf.alpha)<<16) + (255-vbuf.alpha))<<8);
            break;

        case 2:
            // active window locatoin animation
            // move to upper left
            if (vbuf.m_active_left > 48)
            {
                vbuf.m_active_left -= 2;
                vbuf.m_active_top = vbuf.m_active_left*2/3;
            }
            else
            {
                vbuf.m_active_left = 48;
                vbuf.m_active_top = 32;
                ++anim;
                printf("%s.%d anim %d\n", __FUNCTION__, logo_isr_count, anim) ;
            }
            break;

        case 3:
            // active window locatoin animation
            // move to center
            if (vbuf.m_active_left < 216)
            {
                vbuf.m_active_left += 2;
                vbuf.m_active_top = vbuf.m_active_left*2/3;
            }
            else
            {
                vbuf.m_active_left = 216;
                vbuf.m_active_top = 144;
                ++anim;
                printf("%s.%d anim %d\n", __FUNCTION__, logo_isr_count, anim) ;
            }
            break;

        }
#endif

        semaphore_pop(pSemHandle, avioDhubSemMap_vpp_vppCPCB0_intr, 1);
        semaphore_clr_full(pSemHandle, avioDhubSemMap_vpp_vppCPCB0_intr);
        THINVPP_CPCB_ISR_service(thinvpp_obj, CPCB_1);
    }
}
