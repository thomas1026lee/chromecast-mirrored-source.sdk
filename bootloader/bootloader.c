/*
 * (C) Copyright Marvell Semiconductors, Inc 2008 All rightes reserved
 */
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

#include "drmdmx.h"
#include "basic_inc.h"

#if BOOTLOADER_FASTLOGO
#include "thinvpp_api.h"
#include "avpll.h"
#include "api_avio_dhub.h"
#endif

#include "nand_block0_layout_A0.h"
#include "image2_header_A0.h"

#include "image3_header.h"
#include "version_table.h"
#if ANDROID_BOOT
#include "android/bootimg.h"
#include "android/flash_ts.h"
#include "board_config.h"
#include "usb_cust_key.h"
#endif

#include "nflash_drv.h"

#include "linux_param.h"

#include "bootloader_customize.h"

#ifdef CONFIG_USB
#include "usb_storage.h"
#endif

//#define CONFIG_FORCE_ENCRYPTION

#define BOOT_SRC_NAND     1
#define BOOT_SRC_USB      2

/* on a USB storage device, leave first 4KB for MBR/part table
 * and start boot image at a 4KB offset.
 */
#define USB_BOOTIMG_START      (4 * 1024)
#define USB_BLOCK_SIZ          512

/* Key offsets in CPU image */
#define CPU_IMG_OFFS_PKID        0
#define CPU_IMG_OFFS_SKID        64
#define CPU_IMG_OFFS_STYPE       68
#define CPU_IMG_OFFS_IMGSIZ      124
#define CPU_IMG_OFFS_IMGSTA      128

#define KEY_IDX_RKEK          0x1
#define KEY_IDX_SIGNK         0x2
#define KEY_IDX_AESK          0x4
#define KEY_IDX_ROM_BEG       0x10
#define KEY_IDX_ROM_END       0x4F
#define KEY_IDX_CUSTK         0x80
#define KEY_IDX_CUSTK_2       0x82 /* 2nd CUSTK loaded from bootloader */
#define KEY_IDX_INVALID       0xFF

#define MAX_ANDROID_IMG_SIZE (32 * 1024 * 1024)
#define MIN_ANDROID_IMG_SIZE (1 * 1024 * 1024)
#define MAX_LOGO_FRM_SIZE (4 * 1024 * 1024)
#define ANDROID_IMG_BUF (BOARD_MEM_SIZE - MAX_ANDROID_IMG_SIZE)
#define LOGO_FRM_BUF (ANDROID_IMG_BUF - MAX_LOGO_FRM_SIZE)

#if (BERLIN_CHIP_VERSION >= BERLIN_BG2_A0)
#define	MV88DE3100_CHIPID_REG1	(0xF7CC07C8)
#define	MV88DE3100_CHIPID_REG2	(0xF7CC07CC)
#else
#define	MV88DE3100_CHIPID_REG1	(0xF7CC07D8)
#define	MV88DE3100_CHIPID_REG2	(0xF7CC07DC)
#endif

#define	PRODUCT_CHIP_EXT_ID		(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_ProductId_ext)
#define MV88DE3XXX_OTP_STS_REG 0xF7CC08C8
#define MV88DE3XXX_OTP_LKG_ID 0XF7CC07DD
#define MV88DE3XXX_SOC_REV_REG 0XF7CC07D4
#define MV88DE3XXX_SOC_REV_MASK 0x3
#define MV88DE3XXX_SOC_REV_SHIFT 29

#ifdef EMMC_BOOT
#define	SDIO_DATA_BUFFER		(0x4000000)			//	64M as SDIO data buffer start
#define	SDIO_BLK_SIZE			(512)
#endif

#define MAX_RAMDISK_IMPLICT_SIZE	(0x2000000)

enum sm_ldo_ctrl_vout_sel_e {

    SM_LDO_CTRL_VOUT_0_900V = 0,
    SM_LDO_CTRL_VOUT_0_789V,
    SM_LDO_CTRL_VOUT_0_826V,
    SM_LDO_CTRL_VOUT_0_863V,
    SM_LDO_CTRL_VOUT_0_937V,
    SM_LDO_CTRL_VOUT_0_974V,
    SM_LDO_CTRL_VOUT_1_011V,
    SM_LDO_CTRL_VOUT_1_048V,
    SM_LDO_CTRL_VOUT_MAX
};

#if ANDROID_BOOT
#define FTS_NAME        "fts"
#define KERNEL_NAME     "kernel"
#define RECOVERY_NAME   "recovery"

#define FTS_KEY_MACADDR			"macaddr"
#define FTS_WIFI_MFG_MODE "wifi_mfg_mode"
#define FTS_DEVICE_CFGED  "device_configured"

#define BOOTLOADER_COMMAND_KEY		"bootloader.command"
#define ANDROID_CRASH_COUNTER_KEY	"crashcounter.android"
#define MAX_CRASH_COUNT                 5
#define BOOTMODE_NORMAL                 0
#define BOOTMODE_RECOVERY               1
#define BOOTMODE_BOOTUSB                2
#define BOOTMODE_BACKUPSYS              3
const char kBootcmdRecovery[] = "recovery";
const char kFtsBootcmdRecovery[] = "boot-recovery";
const char kFtsBootcmdBackupsys[] = "boot-backupsys";

#endif /* #if ANDROID_BOOT */

#define STR(x) #x
#define XSTR(x) STR(x)

#if defined(DEBUG)
#define debug_printf(a...) lgpl_printf(a)
#else
#define debug_printf(a...)
#endif

#ifdef	NAND_BOOT
extern struct mv_nand_data nand_data ;
block0_t block0;
#endif

#ifdef	EMMC_BOOT
struct mv_nand_data nand_data ;
#endif

ver_table_entry_t vt_img3;
ver_table_entry_t vt_sysconf;
ver_table_entry_t vt_flashless;
#if ANDROID_BOOT
static const ver_table_entry_t vt_fts = {
	.part1_start_blkind = FTS_BLK_START,
	.part1_blks = FTS_BLK_CNT,
};
static const ver_table_entry_t vt_recovery = {
	.part1_start_blkind = RECOVERY_BLK_START,
	.part1_blks = RECOVERY_BLK_CNT,
};
static const ver_table_entry_t vt_kernel = {
	.part1_start_blkind = KERNEL_BLK_START,
	.part1_blks = KERNEL_BLK_CNT,
};
static const ver_table_entry_t vt_backupsys = {
	.part1_start_blkind = BACKUPSYS_BLK_START,
	.part1_blks = BACKUPSYS_BLK_CNT,
};

static unsigned int fts_dev_id = 0;

static boot_img_hdr Mkbootimg_hdr;
void (*TheKernel)(int zero, int arch, unsigned int params);
static int Bootmode = BOOTMODE_NORMAL;
#endif /* #if ANDROID_BOOT */

Image3_Attr img3_hdr;
sm_image_attr *sm_hdr;
cpu0_image_attr *cpu0_hdr;
cpu1_image_attr *cpu1_hdr;
recovery_ou_attr *recou_hdr;
Encryption_Header en_hdr;
gen_header_t sysconf_hdr;

Mem_Layout *sMem_Layout ;

#ifdef CONFIG_SM
#define REBOOT_MSG_EXTRA_BUF_ADDR	SM_MSG_EXTRA_BUF_ADDR
#define REBOOT_MSG_EXTRA_BUF_SIZE	SM_MSG_EXTRA_BUF_SIZE
#else // !CONFIG_SM
#define REBOOT_MSG_EXTRA_BUF_ADDR	0xa00000	// TODO: need a buffer address
#define REBOOT_MSG_EXTRA_BUF_SIZE	512
#endif // CONFIG_SM

char reset_msg_extra_buf[REBOOT_MSG_EXTRA_BUF_SIZE];
int reset_msg_extra_size = 0;

unsigned int uiBoot = 0;
unsigned int uiWarmDown_2_Linux_Addr = 0;

void setup_linux_bootparam(u32 mem_start, u32 mem_size, char *cmdline,
						   u8 chip_rev, u8 board_rev);
extern void Jump(unsigned int, unsigned int, unsigned int, unsigned int);
extern void Copy_Code_To_Figo_Sram();
extern int MV_SM_Send_Msg(int, void *, int);
extern void APB_UART_init(unsigned int , unsigned int , unsigned int ) ;

extern char gp_cust_figo_image[];
extern int MV_DRMLIB_Load_Customer_Key( char *, int);
extern int lgpl_printf(const char *format, ...);


HRESULT MV_SM_Dev_Init(UCHAR * pucData_Buffer,UINT32 uiData_Size);

static void make_mac_from_chipid(long long chipid, char *buf);

void abort(){printf("Abort\n"); while(1);}

int IS_Z1() { return 0; return (readl(PRODUCT_CHIP_EXT_ID) & 0xFF) == 0; }
int IS_Z2() { return 0; return (readl(PRODUCT_CHIP_EXT_ID) & 0xFF) == 2; }
int IS_B0() { return 1; return (readl(PRODUCT_CHIP_EXT_ID) & 0xFF) >= 0xB0; }
#ifdef ENABLE_CONNECT_BTN
extern int is_EnterRecovery;
extern void Check_connectbtnGPIO();
#endif
void pad_init(void)
{
#if (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
	int val;
	T32Gbl_PadSelect reg;
	GA_REG_WORD32_READ(MEMMAP_CHIP_CTRL_REG_BASE+ RA_Gbl_PadSelect, &(reg.u32));
	reg.uPadSelect_DVIO_OEN = Gbl_PadSelect_DVIO_OEN_Enable;
#if 0 //def CYPRESS_EMI	// default always disable
	reg.uPadSelect_DVIO0_V18EN = 1;     //BG_FIX for EMI issue
#else
	reg.uPadSelect_DVIO0_V18EN = 0;
#endif
	reg.uPadSelect_CLK0_ZN = 7;
	reg.uPadSelect_CLK0_ZP = 7;
	reg.uPadSelect_DVIO0_ZN = 7;
#if (BERLIN_CHIP_VERSION < BERLIN_BG2)
	reg.uPadSelect_DVIO0_ZP = 7;
#endif
	GA_REG_WORD32_WRITE(MEMMAP_CHIP_CTRL_REG_BASE+ RA_Gbl_PadSelect, (reg.u32));

#if (BERLIN_CHIP_VERSION >= BERLIN_BG2)
	T32Gbl_PadSelect1 reg1;
	GA_REG_WORD32_READ(MEMMAP_CHIP_CTRL_REG_BASE+ RA_Gbl_PadSelect1, &(reg1.u32));
	reg1.uPadSelect_DVIO0_ZP = 7;
	GA_REG_WORD32_WRITE(MEMMAP_CHIP_CTRL_REG_BASE+ RA_Gbl_PadSelect1, (reg1.u32));
#endif

#ifdef CONFIG_SM
	/*
	 * SM_PAD_CTRL
	 * URT0 RXD internal pull up
	 * bit 28: PU_EN_URT0_RXD
	 */
	GA_REG_WORD32_READ(SOC_SM_SYS_CTRL_REG_BASE + 0x48, &val);
	/*debug_printf("<<<<<<<< SM_PAD_CTRL0: 0x%08X\n", val);*/
	val |= 0x10000000;
	GA_REG_WORD32_WRITE(SOC_SM_SYS_CTRL_REG_BASE + 0x48, val);
	GA_REG_WORD32_READ(SOC_SM_SYS_CTRL_REG_BASE + 0x48, &val);
	/*debug_printf(">>>>>>>> SM_PAD_CTRL0: 0x%08X\n", val);*/
#endif // CONFIG_SM
#endif
}

/* Initiailize GPIOs used by bootloader.
 */
static int board_gpio_init(void)
{
	int ret = 0;
#if CFG_BOARD_NAME == BOARD_EUREKA_B1
	ret = GPIO_PortSetInOut(BUTTON_GPIO, 1);
	if (ret){
		lgpl_printf("ERROR: failed to set button GPIO: %d \n", BUTTON_GPIO);
		return -1;
	}
	ret = GPIO_PortSetInOut(LED_WHITE_GPIO, 0);
	if (ret){
		lgpl_printf("ERROR: failed to set white LED GPIO: %d \n", LED_WHITE_GPIO);
		return -1;
	}
	ret = GPIO_PortSetInOut(LED_RED_GPIO, 0);
	if (ret){
		lgpl_printf("ERROR: failed to set red LED GPIO: %d \n", LED_RED_GPIO);
		return -1;
	}
	return 0;
#endif
	return 1;
}

/* get button status
 * !0 - pressed
 *	0 - not pressed
 */
static int get_button_status(void)
{
	int ret = 0;
	int button = 1;
#ifdef BUTTON_GPIO
	ret = GPIO_PortRead(BUTTON_GPIO, &button);
	if (ret){
		lgpl_printf("ERROR: failed to read button status, ret %d\n", ret);
		return 0;
	}
#endif
	return (button == 0);
}

/* Get SOC revision information */
static unsigned char get_soc_rev(void)
{
	/* Bits 30:29 are marked by Marvell to flag SoC revision.
	 * 0x0 - A0
	 * 0x1 - A1 (LVT + Metal fix)
	 * 0x2 - A2 (Metal fix only)
	 */
	unsigned int rev = readl(MV88DE3XXX_SOC_REV_REG);
	rev &= (MV88DE3XXX_SOC_REV_MASK << MV88DE3XXX_SOC_REV_SHIFT);
	rev >>= MV88DE3XXX_SOC_REV_SHIFT;
	return (unsigned char)rev;
}

/* Set white, red LEDs */
static int set_led(int white_on, int red_on)
{
	int ret = 0;
#ifdef LED_WHITE_GPIO
	ret = GPIO_PortWrite(LED_WHITE_GPIO, white_on);
	if (ret){
		lgpl_printf("ERROR: failed to set white LED, ret %d\n", ret);
		ret = 1;
	}
#endif
#ifdef LED_RED_GPIO
	ret = GPIO_PortWrite(LED_RED_GPIO, red_on);
	if (ret){
		lgpl_printf("ERROR: failed to set red LED, ret %d\n", ret);
		ret = 1;
	}
#endif
	return ret;
}

/* Verify CPU image header flags, enforce security checks */
static int verify_boot_img_hdr(char *img, int boot_src)
{
	int sign_kidx, par_kidx;

	sign_kidx = *((int *)(img + CPU_IMG_OFFS_SKID));
	par_kidx	= *((int *)(img + CPU_IMG_OFFS_PKID));
#if CFG_BOARD_NAME == BOARD_EUREKA_B3
	/* Insist on only booting CUSTK signed images */
	if ((boot_src == BOOT_SRC_NAND) && (sign_kidx != KEY_IDX_SIGNK)){
		printf("ERROR: NAND image signed with wrong key, index %d\n",
               sign_kidx);
		return -1;
	}
	if ((boot_src == BOOT_SRC_USB) && (sign_kidx != KEY_IDX_CUSTK_2)){
		printf("ERROR: USB image signed with wrong key, index %d\n",
               sign_kidx);
		return -1;
	}
	/* TODO(kolla): enforce encryption flags
	 * par_kidx == KEY_IDX_CUSTK
	 */
#elif CFG_BOARD_NAME == BOARD_EUREKA_B2
	/* Insist on only booting CUSTK signed images */
	if ((boot_src == BOOT_SRC_NAND) && (sign_kidx != KEY_IDX_SIGNK)){
		printf("ERROR: NAND image signed with wrong key, index %d\n",
               sign_kidx);
		return -1;
	}
#elif CFG_BOARD_NAME == BOARD_EUREKA_B1
#endif //CFG_BOARD_NAME

	return 0;
}

#ifdef CONFIG_SM
void set_sm_ldo_ctrl_vout(int vout_sel)
{
    unsigned int read;

    vout_sel &= (1 << bsmSysCtl_SM_LDO_CTRL_VOUT_SEL) - 1;

    BFM_HOST_Bus_Read32((SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_LDO_CTRL), &read);

    read = (read & 0xFFFFFFC0)| (vout_sel << LSb32smSysCtl_SM_LDO_CTRL_VOUT_SEL);

    BFM_HOST_Bus_Write32((SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_LDO_CTRL), read);

}
#endif // CONFIG_SM

#if ANDROID_BOOT
//typedef unsigned int u32;
void setup_linux_bootparam_with_ramdisk(u32 mem_start, u32 mem_size, char *cmdline,
										u32 initrd_start, u32 initrd_size,
										u8 chip_rev, u8 board_rev);
static long long nand_read_generic(unsigned int start, unsigned int end, char* data_buff,
					unsigned int data_size);
static int get_mac_addr(char* macaddr);

inline unsigned get_aligned(unsigned address, unsigned page_size) {
	return (address + page_size - 1) / page_size * page_size;
}

int bootimg_print_image_hdr (boot_img_hdr *hdr)
{
	int i;
	debug_printf ("   Image magic:   %s\n", hdr->magic);

	debug_printf ("   kernel_size:   0x%x\n", hdr->kernel_size);
	debug_printf ("   kernel_addr:   0x%x\n", hdr->kernel_addr);

	debug_printf ("   rdisk_size:   0x%x\n", hdr->ramdisk_size);
	debug_printf ("   rdisk_addr:   0x%x\n", hdr->ramdisk_addr);

	debug_printf ("   second_size:   0x%x\n", hdr->second_size);
	debug_printf ("   second_addr:   0x%x\n", hdr->second_addr);

	debug_printf ("   tags_addr:   0x%x\n", hdr->tags_addr);
	debug_printf ("   page_size:   0x%x\n", hdr->page_size);

	debug_printf ("   name:      %s\n", hdr->name);
	debug_printf ("   cmdline:   %s\n", hdr->cmdline);

	for (i=0;i<8;i++)
		debug_printf ("   id[%d]:   0x%x\n", i, hdr->id[i]);
}

static int write_android_bootloader_message(const char *command,
					const char *status,
					const char *recovery)
{
	int res = flash_ts_set("bootloader.command", command);
	if (!res)
		res = flash_ts_set("bootloader.status", status);
	if (!res)
		res = flash_ts_set("bootloader.recovery", recovery);
	else
		debug_printf("Failed to set bootloader command\n");

	return res;
}

void set_android_crash_counter(int cnt)
{
	char value[2];
	value[0] = '0' + cnt;
	value[1] = '\0';
	flash_ts_set(ANDROID_CRASH_COUNTER_KEY, value);
}

int increase_android_crash_counter(void)
{
	int crash_counter = flash_ts_get_int(ANDROID_CRASH_COUNTER_KEY, 0);
	set_android_crash_counter(crash_counter + 1);
}

int check_android_recovery_mode(void)
{
	char boot_command[256] = {0};
	int crash_counter;
	int button_status;

	crash_counter = flash_ts_get_int(ANDROID_CRASH_COUNTER_KEY, 0);
	debug_printf ("fts: %s: %d\n", ANDROID_CRASH_COUNTER_KEY, crash_counter);

	flash_ts_get(BOOTLOADER_COMMAND_KEY, boot_command, sizeof(boot_command));
	debug_printf ("fts: %s: %s\n", BOOTLOADER_COMMAND_KEY, boot_command);

	if (reset_msg_extra_size != 0) {
		debug_printf ("reboot parameter: %s\n", reset_msg_extra_buf);

		if (strncmp(reset_msg_extra_buf, kBootcmdRecovery, sizeof(kBootcmdRecovery)) == 0) {
			return BOOTMODE_RECOVERY;
		}
	}

	if (strncmp(boot_command, kFtsBootcmdRecovery, sizeof(kFtsBootcmdRecovery)) == 0) {
		return BOOTMODE_RECOVERY;
	}
	if (strncmp(boot_command, kFtsBootcmdBackupsys,
				sizeof(kFtsBootcmdBackupsys)) == 0) {
		return BOOTMODE_BACKUPSYS;
	}
	/* Boot in to USB on button press only if no other
	 * conditions for booting in to recovery are being met. Since
	 * button press is also used for factory reset, request to
	 * boot in to recovery in fts overrides button press.
	 */
	button_status = get_button_status();
	if (button_status){
		lgpl_printf("Detected button press -- booting from USB\n");
		return BOOTMODE_BOOTUSB;
	}
#ifndef DISABLE_CRASH_COUNTER
	if (crash_counter > MAX_CRASH_COUNT) {
		char dev_cfged[64] = {0,};
		flash_ts_get(FTS_DEVICE_CFGED, dev_cfged, sizeof(dev_cfged));
		/* Attempt to boot in to recovery after consecutive crashes
		 * only if the device is configured at least once.
		 */
		if (!flash_ts_get_int("crashcounter-disable", 0) &&
			!strcmp("true", dev_cfged)) {
			/* TODO(kolla): determine if we need to send a special message
			 * to recovery to recover from crash.
			 */
			return BOOTMODE_RECOVERY;
		}
	}
#endif // DISABLE_CRASH_COUNTER

	// if SM asked specifically to go to recovery mode, do it.
	if ( MV_SoC_STATE_BOOT_RECOVERY_KERNEL == uiBoot ) 
	{
			lgpl_printf("forcing to recovery mode\n");		
			return BOOTMODE_RECOVERY;
	}
	
	return BOOTMODE_NORMAL;
}


#include "sha1.h"

static unsigned int swap32(unsigned int x)
{
	unsigned y;
	y = ((x << 8) & 0xFF00FF00) | ((x >> 8) & 0x00FF00FF);
	y = (y >> 16) | (y << 16);
	return y;
}

int verify_android_image(const unsigned char *img_start, unsigned int len)
{
	int i;
	SHA1Context sha;		/* SHA-1 context                 */
	const boot_img_hdr *hdr = (const boot_img_hdr *)img_start;
	const unsigned char *kernel_data = img_start + hdr->page_size;
	const unsigned char *ramdisk_data = kernel_data + get_aligned(hdr->kernel_size, hdr->page_size);
	const unsigned char *second_data = ramdisk_data + get_aligned(hdr->ramdisk_size, hdr->page_size);

	SHA1Reset(&sha);
	SHA1Input(&sha, kernel_data, hdr->kernel_size);
	SHA1Input(&sha, &hdr->kernel_size, sizeof(hdr->kernel_size));
	SHA1Input(&sha, ramdisk_data, hdr->ramdisk_size);
	SHA1Input(&sha, &hdr->ramdisk_size, sizeof(hdr->ramdisk_size));
	SHA1Input(&sha, second_data, hdr->second_size);
	SHA1Input(&sha, &hdr->second_size, sizeof(hdr->second_size));

	if (!SHA1Result(&sha))
		printf("bootimg SHA: could not compute message digest\n");
	else {
		/* convert it into mkbootimg header's byte sequence */
		for (i = 0; i < 5; i++)
			sha.Message_Digest[i] = swap32(sha.Message_Digest[i]);

		for (i = 0; i < 5; i++)
			if (sha.Message_Digest[i] != hdr->id[i]) {
				printf("bootimg SHA1 verify fail: start 0x%08x, size %d\n"
						"\tOrig %08X %08X %08X %08X %08X\n"
						"\tCalc %08X %08X %08X %08X %08X\n",
						(unsigned int)img_start, len,
						hdr->id[0],
						hdr->id[1],
						hdr->id[2],
						hdr->id[3],
						hdr->id[4],
						sha.Message_Digest[0],
						sha.Message_Digest[1],
						sha.Message_Digest[2],
						sha.Message_Digest[3],
						sha.Message_Digest[4]);

				return -1;
			}
		return 0;
	}

	return -1;
}


static int load_android_image(int bootmode, int boot_src)
{
	ver_table_entry_t *vt_android;
	int iPageSize = 0;
	int iBlockSize	;

	unsigned int android_start, android_end;
	int ret;
	unsigned mkbootimg_page;
	unsigned char* k_buff;
	unsigned int k_buff_size;
	unsigned int k_buff_offset;
	unsigned int cpu_img_siz = 0;
	unsigned int sign_kidx, par_kidx;
	int usb_stor_cnt;
	static char cpu_img_hdr[USB_BLOCK_SIZ];

	/* Use memory near the end of DDR for loading
	 * Android image for verification. Bootloader has
	 * configured MMU for flat memory map, i.e. VA == PA.
	 */
	k_buff = (char *)ANDROID_IMG_BUF;

	if (boot_src == BOOT_SRC_USB){
#ifdef CONFIG_USB
		android_start = USB_BOOTIMG_START/USB_BLOCK_SIZ;
		ret = usb_init();
		if (ret < 0){
			lgpl_printf("ERROR: Failed to usb_init ret %d\n", ret);
			return -1;
		}
		/* recognize storage devices */
		usb_stor_cnt = usb_stor_scan();
		if (!usb_stor_cnt){
			lgpl_printf("ERROR: No USB storage device detected\n");
			return -1;
		}
		/* Read from first USB device */
		if(usb_stor_read(0, android_start, 1, cpu_img_hdr) != 1){
			lgpl_printf("ERROR: Failed to read CPU image header from USB \n");
			return -1;
		}
#else
		debug_printf("ERROR: USB boot is not supported\n");
		return -1;
#endif //CONFIG_USB
	}else{
		if (bootmode == BOOTMODE_RECOVERY)
			vt_android = &vt_recovery;
		else if (bootmode == BOOTMODE_BACKUPSYS)
			vt_android = &vt_backupsys;
		else
			vt_android = &vt_kernel;

		iPageSize = nand_data.szofpg ;
		iBlockSize = nand_data.szofblk;

		android_start = vt_android->part1_start_blkind;
		android_end = android_start + vt_android->part1_blks;
		android_start *= iBlockSize;
		android_end *= iBlockSize;

		/* Read CPU image header to determing length of the image */
		ret = nand_read_generic(android_start,
								android_end,
								cpu_img_hdr, sizeof(cpu_img_hdr));
		if (ret < 0){
			debug_printf("NAND: Failed to read CPU image header!\n");
			return -1;
		}
	}

	cpu_img_siz = *((unsigned int *)(cpu_img_hdr + CPU_IMG_OFFS_IMGSIZ));
	cpu_img_siz = get_aligned(cpu_img_siz, 16);
	cpu_img_siz += CPU_IMG_OFFS_IMGSTA;
	/* Add min size check to be safe */
	if (cpu_img_siz > MAX_ANDROID_IMG_SIZE || cpu_img_siz < MIN_ANDROID_IMG_SIZE) {
		debug_printf("ERROR: aligned CPU img_siz %u is invalid\n",
					 cpu_img_siz);
		return -1;
	}

	/* Wipe the scratch memory first to make sure nothing */
	/* is present from the last boot attempt. */
	memset(k_buff, 0, MAX_ANDROID_IMG_SIZE);

	if (boot_src == BOOT_SRC_USB){
#ifdef CONFIG_USB
		int blk_cnt;
		int ret;
		/* Load the customer key needed for verifying USB
		 * bootable images in to FIGO.
		 */
#if CFG_BOARD_NAME != BOARD_EUREKA_B2
		/* Some eureka-b2 devices have incorrect OTP causing loading of custk to fail.
		 * Disable loading of customer key.
		 */
		ret = MV_DRMLIB_Load_Customer_Key(usb_custk_store, sizeof(usb_custk_store));
		if (MV_FIGODRV_OK != ret) {
		  lgpl_printf("ERROR: Failed to load USB customer key \n");
		  return -1;
		}
#endif //CFG_BOARD_NAME

		/* Avoid re-reading the first block to prevent */
		/* security hacks */
		memcpy(k_buff, cpu_img_hdr, sizeof(cpu_img_hdr));

		blk_cnt = (cpu_img_siz + USB_BLOCK_SIZ - 1)/USB_BLOCK_SIZ;
		/* Always read from first USB storage device */
		ret = usb_stor_read(0,
							(android_start + 1),
							(blk_cnt - 1),
							(k_buff + USB_BLOCK_SIZ));
		if (ret != (blk_cnt - 1)){
			lgpl_printf("ERROR: Failed to read CPU image blk_cnt %d ret %d\n",
						(blk_cnt - 1), ret);
			return -1;
		}

		/* There can be trailing bytes beyond the end of the image */
		/* due to rounding up to USB_BLOCK_SIZ (and which would */
		/* otherwise be unverified).  Might as well zero all remaining */
		/* memory to be safer. */
		int trailing_bytes = MAX_ANDROID_IMG_SIZE - cpu_img_siz;
		if (trailing_bytes > 0) {
			lgpl_printf("NOTE: %x trailing bytes zeroed\n", trailing_bytes);
			memset(k_buff + cpu_img_siz, 0, trailing_bytes);
		}

#endif //CONFIG_USB
	}else{
		ret = nand_read_generic(android_start,
								android_end,
								k_buff,
								cpu_img_siz);
		if (ret < 0){
			lgpl_printf("ERROR: Failed to read CPU image ret %d\n",
						ret);
			return -1;
		}
	}
	if (verify_boot_img_hdr(k_buff, boot_src) != 0) {
		return -1;
	}
	/* VerifyImage verifies image signature on FIGO and decrypts
	 * image if encrypted. Output image without CPU image header and
	 * signature block is written out, in this case output image is
	 * bootimg.
	 */
#if CFG_BOARD_NAME == BOARD_EUREKA_B2
	/* Some eureka-b2 devices have incorrect OTP causing signing check
	 * to fail for USB boot. Disable USB boot check.
	 */
	if (boot_src == BOOT_SRC_USB) {
	  int rsa_sig_siz = 128;
	  /* Verify image strips the CPU image header from input
	   * image. Do that manually when VerifyImage is not used. CPU image
	   * header consists of 128 byte RSA sign as well.
	   */
	  k_buff += CPU_IMG_OFFS_IMGSTA + rsa_sig_siz;
	  ret = 0;
	} else {
	  ret = VerifyImage((unsigned int)k_buff,
						cpu_img_siz,
						(unsigned int)k_buff);
	}
#else
	ret = VerifyImage((unsigned int)k_buff,
					  cpu_img_siz,
					  (unsigned int)k_buff);
#endif //CFG_BOARD_NAME
	if(ret) {
		printf("ERROR: Verify image failed! ret=%d\n", ret);
		return -1;
	}

	memcpy(&Mkbootimg_hdr, k_buff, sizeof(Mkbootimg_hdr));

	if (strncmp(Mkbootimg_hdr.magic, BOOT_MAGIC, BOOT_MAGIC_SIZE)) {
		printf("ERROR: Invalid mkbootimg header!\n");
		return -1;
	}

	/* force to use predefined kernel addr (CPU0_START_ADDR) in config.
	 * We don't use the kernel_addr from kernel partition for scurity
	 * consideration.
	 */
	Mkbootimg_hdr.kernel_addr = CPU0_START_ADDR;

	TheKernel = (void (*)(int, int, int))(Mkbootimg_hdr.kernel_addr);

	mkbootimg_page = Mkbootimg_hdr.page_size;
	// align by page_size and add one page for skipping header.
	k_buff_size = mkbootimg_page +
		get_aligned(Mkbootimg_hdr.kernel_size, mkbootimg_page) +
		get_aligned(Mkbootimg_hdr.ramdisk_size, mkbootimg_page) +
		get_aligned(Mkbootimg_hdr.second_size, mkbootimg_page);

	/* Check bootloader has enough memory to be able to
	 * read the firmware image for verification.
	 */
	if (k_buff_size > MAX_ANDROID_IMG_SIZE){
		debug_printf("BOOTLOADER: Firmware image is too big %d for "
					 "verification\n",
					 k_buff_size);
		return -1;
	}

	UtilMemCpy(Mkbootimg_hdr.kernel_addr, k_buff + mkbootimg_page, Mkbootimg_hdr.kernel_size);
	if (Mkbootimg_hdr.ramdisk_size) {
		k_buff_offset = get_aligned(Mkbootimg_hdr.kernel_size + mkbootimg_page, mkbootimg_page);
		UtilMemCpy(Mkbootimg_hdr.ramdisk_addr, k_buff + k_buff_offset, Mkbootimg_hdr.ramdisk_size);
	}

	return 0;
}

void make_android_macaddr(char *mac_addr_buf)
{
	char mac_addr[32] = "macaddr=";

#if (CFG_BOARD_NAME <= BOARD_EUREKA_B1)
    /* Newer hardware revs can make MAC addr from chip ID. */
	flash_ts_get(FTS_KEY_MACADDR, &mac_addr[8], sizeof(mac_addr) - 8);
	printf("get macaddr from fts: %s\n", &mac_addr[8]);
#endif //CFG_BOARD_NAME

	if (strlen(&mac_addr[8]) <= 0) {
		// get from u-boot.env
		if (0 != get_mac_addr(mac_addr)) {
			// if failed, try to generate from chip id
			long long chipid;
			chipid = readl(MV88DE3100_CHIPID_REG1) ;
			chipid += (long long)(readl(MV88DE3100_CHIPID_REG2)) << 32;
			if(chipid){
				make_mac_from_chipid(chipid, mac_addr);
			} else {
				// use default macaddr if fail to generate from chip id
				strcpy(mac_addr, "macaddr=00:11:22:33:44:55");
			}
		}
	}
	if (mac_addr_buf)
		strcpy(mac_addr_buf, mac_addr);
}

void setup_android_kernel_param(int boot_mode)
{
	char kernel_param[1024];
	char mac_addr[32];
	char tmp_buf[256];

	make_android_macaddr(mac_addr);

	//add flash_ts dev_id info
#if defined(NAND_BOOT)
	lgpl_sprintf(tmp_buf, " flash_ts.dev_id=%d flash_ts.size=%d flash_ts.erasesize=%d flash_ts.writesize=%d",
			fts_dev_id, vt_fts.part1_blks * nand_data.szofblk, nand_data.szofblk, nand_data.szofpg);
#elif defined(EMMC_BOOT)
	lgpl_sprintf(tmp_buf, " emmc_ts.dev_id=%d emmc_ts.size=%d emmc_ts.erasesize=%d emmc_ts.writesize=%d",
			fts_dev_id, vt_fts.part1_blks * nand_data.szofblk, nand_data.szofblk, 512);
#endif
#if defined(DEBUG) || (CFG_BOARD_NAME <= BOARD_EUREKA_B1)
	lgpl_printf("mkbootimg bootargs: %s\n", Mkbootimg_hdr.cmdline);
#endif //DEBUG

	strcpy(kernel_param, Mkbootimg_hdr.cmdline);
	/* Append mac address and fts command line params to kernel
	 * command line.
	 */
	strcat(kernel_param, " ");
	strcat(kernel_param, mac_addr);
	strcat(kernel_param, tmp_buf);

	if (reset_msg_extra_size != 0) {
		strcat(kernel_param, " ");
		strcat(kernel_param, reset_msg_extra_buf);
	}

	if (Mkbootimg_hdr.ramdisk_size) {
	  memset(tmp_buf, 0, sizeof(tmp_buf));
	  strcat(kernel_param, tmp_buf);
	}
#if defined(DEBUG) || (CFG_BOARD_NAME <= BOARD_EUREKA_B1)
	lgpl_printf("Generated bootargs: %s\n", kernel_param);
#endif //DEBUG

	/* Setup androidboot.hardware and androidboot.bootmode
	 * params needed by kernel. Allow boot from USB to gain all
	 * permissions a recovery image would have.
	 */
	strcat(kernel_param, " androidboot.hardware="XSTR(BOARD_NAME));
	if (boot_mode == BOOTMODE_NORMAL)
		strcat(kernel_param, " androidboot.mode=normal");
	else if ((boot_mode == BOOTMODE_RECOVERY) ||
			(boot_mode == BOOTMODE_BOOTUSB))
		strcat(kernel_param, " androidboot.mode=recovery");
	else if (boot_mode == BOOTMODE_BACKUPSYS)
		strcat(kernel_param, " androidboot.mode=backupsys");
	else
		lgpl_printf("ERROR: unknown boot mode %d \n", boot_mode);

	/* Pass bootloader version info to kernel */
	strcat(kernel_param, " androidboot.bootloader="BOOTLOADER_VERSION);

	/* If the fts param for wifi mfg mode is set, pass module
	 * params to enable wifi mfg mode.
	 */
#if (CFG_BOARD_NAME <= BOARD_EUREKA_B1)
	flash_ts_get(FTS_WIFI_MFG_MODE, tmp_buf, sizeof(tmp_buf));
	if ((boot_mode == BOOTMODE_NORMAL) && !strcmp("1", tmp_buf)) {
		strcat(kernel_param, " sd8787.mfg_mode=1 sd8787.drv_mode=1");
		strcat(kernel_param, " sd8787.fw_name=mrvl/sd8787_mfg.bin");
	}
#endif //CFG_BOARD_NAME

	setup_linux_bootparam_with_ramdisk(Mkbootimg_hdr.kernel_addr - 0x8000,
									   MEMORY_SIZE - Mkbootimg_hdr.kernel_addr,
									   kernel_param,
									   Mkbootimg_hdr.ramdisk_addr,
									   Mkbootimg_hdr.ramdisk_size,
									   get_soc_rev(), BOARD_REV);
}

void start_android_kernel(void)
{
	// TODO: Set appropriate arch parameter.
	unsigned arch = 0;
	unsigned params = Mkbootimg_hdr.kernel_addr - 0x8000 + 0x100;

	if (Bootmode == BOOTMODE_NORMAL) {
		lgpl_printf("Boot normal GTV image\n");
		// crash counter will be cleared after completing GTV boot (by CrashCounter app)
		increase_android_crash_counter();
	} else {
		lgpl_printf("Boot recovery image\n");
		set_android_crash_counter(0);
	}
#if defined(DEBUG) || (CFG_BOARD_NAME <= BOARD_EUREKA_B1)
	lgpl_printf("Start kernel at 0x%08x\n", Mkbootimg_hdr.kernel_addr);
#endif //DEBUG
	__reset_cpu((int)TheKernel , 1330, params) ;
	TheKernel (0, arch, params);
}

#endif /* #if ANDROID_BOOT */

#if 0
// the pucNAND_Addr should be page aligned . pucDDRAddr should have one page size memory at least
int NAND_ReadData(unsigned char* pucNAND_Addr, unsigned char* pucDDRAddr,
			unsigned int uiData_Length)
{
	int i, resd, cplen, ret=0;
	int iPageSize = nand_data.szofpg;
	int iPage_Number;
	int page_buff[8448/4];

	iPage_Number = (uiData_Length + iPageSize - 1)/iPageSize; // page number we need read

	for (i = 0; i < iPage_Number; i++) {
		resd = uiData_Length - i*iPageSize;
		cplen = resd < iPageSize ? resd : iPageSize;
		if ((ret = NFlash_PageRead((loff_t)pucNAND_Addr, page_buff)) == 0) {
			UtilMemCpy(pucDDRAddr, page_buff, cplen);
			pucNAND_Addr += iPageSize;
			pucDDRAddr += iPageSize;
		} else {
			return ret;
		}
	}

	return 0;
}

int ReadBlock(unsigned char* pucBlockAddr, unsigned char* pucDDRAddr)
{
	return NAND_ReadData(pucBlockAddr, pucDDRAddr, nand_data.szofblk);
}
#endif

#ifdef NAND_BOOT
/*
 * return: the block address after the last block of the image
 */
static long long nand_read_generic(unsigned int start, unsigned int end, char* data_buff,
					unsigned int data_size)
{
	unsigned int i,j,k;
	unsigned int block_size, page_size;
	int page_buff[8448/4];
	unsigned int residual, br;

	block_size = nand_data.szofblk;
	page_size = nand_data.szofpg;

	if((start & (block_size - 1)) || (end & (block_size - 1)))
		return -1;
	if( (end < start) || (end - start < data_size))
		return -1;
	j=0;
	for(i=start;i<end;i+=block_size) {
		residual = data_size - j;
		br = residual < block_size ? residual : block_size;
		if(j >= data_size)
			break;
		if(is_block_bad((loff_t)i)) {
			debug_printf("Bad block found @0x%08x.\n", i);
			continue;
		}
		for(k=0;k<br;k+=page_size) {
			unsigned int resd = br - k;
			unsigned int cp_size = resd < page_size ? resd : page_size;
			if(NFlash_PageRead((loff_t)i + k, page_buff)) {
				lgpl_printf("Read failed @ 0x%08x\n", i + k);
				return -1;
			}
			UtilMemCpy(data_buff + j + k, page_buff, cp_size);
		}
		j+=block_size;
	}
	if(j < data_size)
		return -1;

	return i;
}

/*
 * ECC strength parameter is for A0
 * fix addr_cycle to 5
 */
int init_nfc(int block_size, int page_size, int ecc_strength)
{
//	int ret=0;
	// configure device
	nand_data.szofpg = page_size;
	nand_data.szofblk = block_size;
	nand_data.t = ecc_strength;
	if((page_size != 512) && (page_size != 2048) &&
		(page_size != 4096) && (page_size != 8192)) {
		printf("[%s:%d] error nand parameters(block_size=%d, page_size=%d, ecc=%d)\n",
			__func__, __LINE__, block_size, page_size, ecc_strength);
		return -1;
	}

	debug_printf("NAND block size %d, page size %d, ecc_strength %d\n",
		nand_data.szofblk, nand_data.szofpg, nand_data.t);

	NFlash_Init();
#if 0
	if(NFlash_ResetDevice()){
		lgpl_printf("Reset NAND error!\n");;
//		while(1);
	}
	if( (ret = NAND_ReadData( (unsigned char *)0,
				(unsigned char *)(&block0), sizeof(block0_t) )) != 0 ) {
		lgpl_printf("Read NAND block0 error, ret = %u!\n", ret);;
//		while(1);
		return ret;
	}

	nand_data.ndtr0cs0 = block0.ndtr0cs0;
	nand_data.ndtr1cs0 = block0.ndtr1cs0;
	nand_data.ndredel = block0.ndredel;

	NFlash_Init();
	if(NFlash_ResetDevice()){
		lgpl_printf("reinit NAND error!\n");;
		while(1);
	}
#endif
	return 0;
}

#endif

void DisableInterrupt(void)
{
//	int my_cpsr ;

	__asm__ __volatile__(
			"MRS			r0, CPSR" 	"\n\t"		//get CRPR
			"BIC			r0, #0xC0"	"\n\t" 		// disable IRQ and FIQ
			"MSR			CPSR, r0"  "\n\t"
			:
			:
			:"r0"
		);
}

#ifdef CONFIG_SM
/* 
response_to_sm() 

If we're in a warmup state, bootloader could decide to boot either the normal or recovery mode. 
Beetle wants the user to be able to use the CONNECT button to navigate the recovery menu in case the remote is lost. Therefore
SM needs to know which linux is booted so that it can interpret the CONNECT button appropriately.
Therefore, in the warmup state response messages, include the bootmode for SM to use.
 */

static void response_to_sm()
{
	/*
	 * Moved from bootloader to here
	 * cold_up warm_up ... response
	 */
#ifdef	BOX_PRODUCT_RELEASE
	if( (MV_SoC_STATE_WARMUP_0 != uiBoot) && (MV_SoC_STATE_WARMUP_1 != uiBoot) && (MV_SoC_STATE_SYSRESET != uiBoot) )
#else
	/* send response if system is going to suspend(warmdown2) */
	if( (MV_SoC_STATE_WARMDOWN_1 == uiBoot) || (MV_SoC_STATE_WARMDOWN_2 == uiBoot)
		|| (MV_SoC_STATE_COLDBOOTUP == uiBoot))
#endif
	{
		lgpl_printf("Prepare to enter power saving mode.\n");

		// Save DDR parameters to SM
		//		Save_Warm_Boot_Contex() ;

		// Disable interrupt & Make sure code running in I-$
		DisableInterrupt();

#ifdef ENABLE_CONNECT_BTN
		/* it's a walk around for SM can't receive response from figo */
		/* if bettle prj enable suspend/resume funciton and the figo issue is still here,
		 * we also need add response for warmdown2
		 */
		if(MV_SoC_STATE_WARMDOWN_1 == uiBoot)
		{
			unsigned int puiMsg[8] ;

			lgpl_printf("Send warmup to warmdown response.\n");
			puiMsg[0] = MV_SM_POWER_WARMDOWN_1_RESP;
			puiMsg[1] = 0 ;
			puiMsg[2] = 0 ;
			puiMsg[3] = 0 ;
			puiMsg[4] = 0 ;
			puiMsg[5] = 0 ;
			puiMsg[6] = 0 ;
			puiMsg[7] = 0 ;
			MV_SM_Send_Msg(MV_SM_ID_POWER, puiMsg, 8) ;
		}
#endif
		// first we should copy cache lock down code into FIGO SRAM
		lgpl_printf("Copy code to FIGO SRAM and run from there.\n");
		Copy_Code_To_Figo_Sram() ;

	}
	//add by hjcai,System reset,SM codes will be loaded by CPU1 image, so should suspend SM
	else if(MV_SoC_STATE_SYSRESET==uiBoot)
	{
		lgpl_printf("Boot from system reset.\n");
		//		reg(SM_CPU_CTRL)=0x4;
	}
	else if(MV_SoC_STATE_WARMUP_0_RESP == uiBoot)
	{
		//	first send WARMUP_RESP_0 to SM
		lgpl_printf("Send WARMUP_0_RESP.\n");

		unsigned int puiMsg[8] ;
		puiMsg[0] = MV_SoC_STATE_WARMUP_0_RESP ;
		puiMsg[1] = 0 ;
		puiMsg[2] = 0 ;
		puiMsg[3] = 0 ;
		puiMsg[4] = 0 ;
		puiMsg[5] = 0 ;
		puiMsg[6] = 0 ;
		puiMsg[7] = 0 ;
		MV_SM_Send_Msg(MV_SM_ID_POWER, puiMsg, 8) ;
	}
	else if(MV_SoC_STATE_WARMUP_1_RESP == uiBoot || MV_SoC_STATE_WARMUP_FROM_DDR_OFF_1 == uiBoot)
	{
		//	first send WARMUP_RESP_1 to SM
		unsigned int puiMsg[8] ;

		lgpl_printf("Send WARMUP_1_RESP.\n");
		if (MV_SoC_STATE_WARMUP_FROM_DDR_OFF_1 == uiBoot)
			lgpl_printf("Answering WU_FROM_DDR_OFF1.\n");
		puiMsg[0] = MV_SoC_STATE_WARMUP_1_RESP ;
		puiMsg[1] = 0 ;
		puiMsg[2] = 0 ;
		puiMsg[3] = 0 ;
		puiMsg[4] = 0 ;
		puiMsg[5] = 0 ;
		puiMsg[6] = 0 ;
		puiMsg[7] = 0 ;
		MV_SM_Send_Msg(MV_SM_ID_POWER, puiMsg, 8) ;
	}
}

	// send special bootmode message. We might not otherwise send a message to SM, depending
	// on how we decided to choose recovery/normal, so make sure he knows. (This is so that Beetle
	// devices can handle the CONNECT button correctly for Recovery.)
	// all warmup states should pass this way.	
static void notify_SM_of_Bootmode(int Bootmode)
{
	
	unsigned int puiMsg[8] ;
	lgpl_printf("Send bootmode=%d to SM.\n", Bootmode);
	puiMsg[0] = MV_SM_POWER_BOOT_MODE ;
	puiMsg[1] = Bootmode;				// 0 = normal, 1 = recovery
	puiMsg[2] = 0 ;
	puiMsg[3] = 0 ;
	puiMsg[4] = 0 ;
	puiMsg[5] = 0 ;
	puiMsg[6] = 0 ;
	puiMsg[7] = 0 ;
	MV_SM_Send_Msg(MV_SM_ID_POWER, puiMsg, 8) ;
}


unsigned int get_temp_id()
{
	return (readl(0xF7CC07DC) & 0xFF);
}
/* this function notify SM the chip revision if it is equal or above B0
 * also, send temp ID to SM.
 */
static void notify_sm(void)
{
	unsigned int pMsgSend[3] = {0};
	unsigned int temp = get_temp_id();
	printf("OTP Temperature: %d celsius degree\n", temp);

	if (IS_B0()) {
		debug_printf("BOOT: CPU - B0\n");
		pMsgSend[0] = MV_SM_TEMP_CPU_TYPE;
		pMsgSend[1] = MV_SM_TEMP_CPU_TYPE_B0;
		pMsgSend[2] = temp;
		MV_SM_Send_Msg(MV_SM_ID_TEMP, pMsgSend, sizeof(pMsgSend));
	} else {
		debug_printf("BOOT: CPU - !B0\n");
	}
}
#endif // CONFIG_SM

inline int ctoi(char c)
{
	if((c>='0')&&(c<='9'))
		return c-'0';
	else if((c>='a')&&(c<='f'))
		return c-'a' + 10;
	else if((c>='A')&&(c<='F'))
		return c-'A' + 10;
	else
		return -1;
}

inline char itoc(char i)
{
	int j = i&0xf;
	if(j<10)
		return j+'0';
	else
		return j-10+'A';
}

#define SOC_WOL_BASE    0xf7fe0000
void set_wol_macaddr(char* mac)
{
	int i=0;
	int value[2];
	char *p=(char*)value;

	printf("WOL MAC address: %s\n", mac);
	for(i=0;i<6;i++){
		p[i]=(ctoi(mac[3*i])<<4) + ctoi(mac[3*i+1]);
	}
	writel(value[0], (SOC_WOL_BASE + RA_WOL_MAC_ADDR_LO));
	writel(value[1], (SOC_WOL_BASE + RA_WOL_MAC_ADDR_HI));
}

/*
 * Get MAC address from NAND
 */
static int get_mac_addr(char* macaddr)
{
	int i;
#ifdef EMMC_BOOT
	int uboot_env_start = 0x3C00000;
#else
#define NAND_UBOOT_ENV_BLOCK_START	(12)
	int uboot_env_start = NAND_UBOOT_ENV_BLOCK_START * nand_data.szofblk;
#endif
	int uboot_env_end = 0x4000000;
	int uboot_env_size = 0x1000;
	char *env_buff = UtilMemAlloc(uboot_env_size);
	int blk_size = nand_data.szofblk;
	char *tmp_buf;

#ifdef EMMC_BOOT
	if (do_emmc_switch_part(0)){
		lgpl_printf("Fail to switch user partition.\n");
	}

	if (0 != do_emmcread((unsigned long long )uboot_env_start, 0x1000 / 0x200, env_buff)) {

		UtilMemCpy(macaddr, "macaddr=00:11:22:33:44:55", 25);
		macaddr[25]='\0';
		return -1;
	}
#else
	for(i=0;i<4;i++) {
		if(nand_read_generic(uboot_env_start + i*blk_size, uboot_env_end, env_buff,
					uboot_env_size) >= 0) {
			break;
		}
	}

	if(i==4) {
		UtilMemCpy(macaddr, "macaddr=00:11:22:33:44:55", 25);
		macaddr[25]='\0';
		return -1;
	}

#endif
	i=0;
	while(i<uboot_env_size) {
		tmp_buf = strstr(env_buff, "ethaddr=");
		if(tmp_buf)
			break;
		i += strlen(env_buff);
		env_buff += strlen(env_buff);
		i++;
		env_buff++;
	}
//	lgpl_printf("uboot env: %s\n", env_buff);
	if(i>=uboot_env_size) {
		UtilMemCpy(macaddr, "macaddr=00:11:22:33:44:55", 25);
		macaddr[25]='\0';
		return -1;
	}
	strcpy(macaddr, "macaddr=");
	UtilMemCpy(macaddr+8, tmp_buf+8, 17);
	macaddr[25]='\0';
	return 0;
}

//buf size must be greater than 17+8
static void make_mac_from_chipid(long long chipid, char *buf)
{
	int i=0;
	int len;
	char *new = buf;

	debug_printf("cpu id:%08x %08x\n", (int)(chipid>>32), (int)(chipid));
	strcpy(new, "macaddr=");
	new+=sizeof("macaddr=")-1;
	*new++='0';
	*new++='0';
	for(i=0;i<5;i++) {
		char hex;
		hex = chipid >> (32-8*i);
		*new++=':';
		*new++=itoc(hex>>4);
		*new++=itoc(hex);
	}
	*new='\0';
}

static void init_mac_address(long long chipid, const char* old_param, char* new_param)
{
	int i=0;
	int len;
	char *new = new_param;

	debug_printf("cpu id:%08x %08x\n", (int)(chipid>>32), (int)(chipid));
	len = strlen(old_param);
	UtilMemCpy(new, old_param, len);
	new+=len;
	*new = ' ';
	new++;
	make_mac_from_chipid(chipid, new);
	set_wol_macaddr(new+8);
/*
	UtilMemCpy(new, " macaddr=",sizeof(" macaddr=")-1);
	new+=sizeof(" macaddr=")-1;
	*new++='0';
	*new++='0';
	for(i=0;i<5;i++) {
		char hex;
		hex = chipid >> (32-8*i);
		*new++=':';
		*new++=itoc(hex>>4);
		*new++=itoc(hex);
	}
	*new='\0';
*/
}
#if 0
void load_flashless_data()
{
	unsigned int i;

#ifdef	NAND_BOOT
	int iBlockSize = nand_data.szofblk;
#endif

#ifdef	NAND_BOOT


#define	SATA_DATA_ENTRY_ADDR		(sMem_Layout->uizsp_mem_size+sMem_Layout->uicpu1_mem_size+sMem_Layout->uicpu0_mem_size+0x200000+(20<<20))
#define SATA_VAR_BLOCKS			16
#define SATA_IMAGE_BLOCKS		16
//	unsigned char * pucSata_Config_Buffer = (unsigned char*)0x5000000 ;
	unsigned char * pucSata_Config_Buffer = UtilMemAlloc(iBlockSize);

	unsigned int flash_less_start, flash_less_end;
	unsigned char * flash_less_buff = (unsigned char*)SATA_DATA_ENTRY_ADDR;

	if(!pucSata_Config_Buffer) {
		lgpl_printf("Malloc failed.\n");
		return;
	}
	if(UtilMemCmp(FLASHLESS_NAME, vt_flashless.name, sizeof(FLASHLESS_NAME)) != 0)
		return;

	debug_printf("Loading flashless data ...\n");
	if((vt_flashless.part1_version.major_version > vt_flashless.part2_version.major_version)
		|| ((vt_flashless.part1_version.major_version == vt_flashless.part2_version.major_version)
		 &&(vt_flashless.part1_version.minor_version > vt_flashless.part2_version.minor_version))) {
		flash_less_start = vt_flashless.part1_start_blkind;
		flash_less_end = flash_less_start + vt_flashless.part1_blks;
//		flash_less_start *= iBlockSize;
//		flash_less_end *= iBlockSize;
	} else {
		flash_less_start = vt_flashless.part2_start_blkind;
		flash_less_end = flash_less_start + vt_flashless.part2_blks;
//		flash_less_start *= iBlockSize;
//		flash_less_end *= iBlockSize;
	}
	debug_printf("flashless start:%d, end: %d\n", flash_less_start, flash_less_end);
	if(flash_less_start + SATA_VAR_BLOCKS+SATA_IMAGE_BLOCKS > flash_less_end)
		lgpl_printf("warning: sata data start %d, end %d.\n", flash_less_start, flash_less_end);

	for(i=flash_less_start ; i<flash_less_start + SATA_VAR_BLOCKS; i++){
		if(ReadBlock((unsigned char*)(i*iBlockSize), pucSata_Config_Buffer) >= 0){
			UtilMemCpy( flash_less_buff, pucSata_Config_Buffer, iBlockSize);
			if ((i - flash_less_start < 8) || (i - flash_less_start == 8)){ // blk 0~7 is for 8 variables(its size < 1blk), blk 8~16 is for one image
				*(unsigned int *)(flash_less_buff+16) = iBlockSize;//for Linux kernel's convenience
			}
		}else{
			// fill 0 in magic field to make sure application will regard this block as bad block.
			*(unsigned int *)flash_less_buff= 0;
			*(unsigned int *)(flash_less_buff+4) = 0;
			*(unsigned int *)(flash_less_buff+8) = 0;
			*(unsigned int *)(flash_less_buff+12) = 0;
			*(unsigned int *)(flash_less_buff+16) = iBlockSize;//for Linux kernel's convenience
		}
//		pucSata_Config_Buffer += iBlockSize ;
		flash_less_buff += iBlockSize ;
	}
	/* copy images */
	for(i=flash_less_start + SATA_VAR_BLOCKS; i<(flash_less_start + SATA_VAR_BLOCKS+SATA_IMAGE_BLOCKS); i++){
		if(ReadBlock((unsigned char*)(i*iBlockSize), pucSata_Config_Buffer) >= 0){
			//	succeed to read sata data
			//	in the production, may add filter here to just pick up flashvar block and break.
			UtilMemCpy( flash_less_buff, pucSata_Config_Buffer, iBlockSize) ;
//			pucSata_Config_Buffer += iBlockSize ;
			flash_less_buff += iBlockSize ;
		}
	}
#endif		//	end of NAND_BOOT

	debug_printf("Done.\n");
}

int load_sysconf()
{
	unsigned int sysconf_start, sysconf_end;
	int iBlockSize  ;
	char *sysconf_buff;

#ifdef	NAND_BOOT
	if(UtilMemCmp(SYSCONF_NAME, vt_sysconf.name, sizeof(SYSCONF_NAME)) != 0)
		return 1;

	iBlockSize = nand_data.szofblk;
	sysconf_buff = (char *)(sMem_Layout->uizsp_mem_size+sMem_Layout->uicpu1_mem_size
			+ sMem_Layout->uicpu0_mem_size+0x200000) ;
	debug_printf("Loading sysconfig data ...\n");

	//try to loader main part
	sysconf_start = vt_sysconf.part1_start_blkind + vt_sysconf.part1_blks/2;
	sysconf_end = sysconf_start + vt_sysconf.part1_blks/2;
	sysconf_start *= iBlockSize;
	sysconf_end *= iBlockSize;

	debug_printf("main sysconfig start:%u, end: %u\n", sysconf_start, sysconf_end);
	if(nand_read_generic(sysconf_start, sysconf_end,
		(char*)&sysconf_hdr, sizeof(sysconf_hdr)) < 0) {
		lgpl_printf("Read sysconf header error!\n");
		goto load_backup;
	}
	debug_printf("Image size %d\n", sysconf_hdr.length);
	if(sysconf_hdr.magic != MAGIC_NUMBER) {
		lgpl_printf("MAGIC_NUMBER does not match.\n");
		goto load_backup;
	}
	if(nand_read_generic(sysconf_start, sysconf_end, sysconf_buff-sysconf_hdr.offset,
				sysconf_hdr.length + sysconf_hdr.offset) < 0) {
		lgpl_printf("Read sysconf data error!\n");
		goto load_backup;
	}
	debug_printf("Done.\n");
	return 0;
load_backup:
	//try to loader backup part
	sysconf_start = vt_sysconf.part1_start_blkind;
	sysconf_end = sysconf_start + vt_sysconf.part1_blks/2;
	sysconf_start *= iBlockSize;
	sysconf_end *= iBlockSize;

	debug_printf("backup sysconfig start:%u, end: %u\n", sysconf_start, sysconf_end);
	if(nand_read_generic(sysconf_start, sysconf_end,
		(char*)&sysconf_hdr, sizeof(sysconf_hdr)) < 0) {
		lgpl_printf("Read sysconf header error!\n");
		return -1;
	}
	debug_printf("Image size %d\n", sysconf_hdr.length);
	if(sysconf_hdr.magic != MAGIC_NUMBER) {
		lgpl_printf("MAGIC_NUMBER does not match.\n");
		return -1;
	}
	if(nand_read_generic(sysconf_start, sysconf_end, sysconf_buff-sysconf_hdr.offset,
				sysconf_hdr.length + sysconf_hdr.offset) < 0) {
		lgpl_printf("Read sysconf data error!\n");
		return -1;
	}
#endif		//	end of NAND boot

	debug_printf("Done.\n");
	return 0;
}
#endif

#ifdef	NAND_BOOT
static long long get_next_img_addr(unsigned int start, unsigned int end, unsigned int data_size)
{
	unsigned int i,j;
	unsigned int block_size;

	block_size = nand_data.szofblk;
	j = (data_size + block_size - 1) & ~(block_size - 1);
	for(i=start;i<end;i+=block_size) {
		if(j==0)
			break;
		if(is_block_bad((loff_t)i)) {
			debug_printf("Bad block found @0x%08x.\n", i);
			continue;
		}
		j-=block_size;
	}
	if(i >= end)
		return -1;

	return i;
}
#endif
#ifdef	EMMC_BOOT
static long long get_next_img_addr(unsigned int start, unsigned int end, unsigned int data_size)
{
	unsigned int i,j;
	unsigned int block_size;

	block_size = nand_data.szofblk;
	j = (data_size + block_size - 1) & ~(block_size - 1);
	return (start + j);
}
#endif

void get_reset_msg(void)
{
	/*
	 * clear reset_msg_extra_size to an initial value
	 */
	reset_msg_extra_size = 0;

	if (MV_SoC_STATE_SYSRESET == uiBoot) {
		char * extra_buf = REBOOT_MSG_EXTRA_BUF_ADDR;
		reset_msg_extra_size = *(int *)extra_buf;
		lgpl_printf("%s,%d: sysreset, need to get reset_msg_extra_size=%d\n",
				__func__, __LINE__, reset_msg_extra_size);
		if (reset_msg_extra_size > 0) {
			lgpl_printf("Copying extra message(len=%d) for sysreset...\n",
					reset_msg_extra_size);
			UtilMemCpy(reset_msg_extra_buf, extra_buf+4, reset_msg_extra_size);
		}
	}
}



#if ANDROID_BOOT
/*
 * For Android not need to load SM from bootimgs.
 * Currently, we use this solution. Because it can save the space of flash
 * memory of bootimgs & bootimgs-B. And currently, there is a bug in recoery
 * scripts, it doesn't update bootimgs-B.
 */

#ifdef CONFIG_SM
int load_and_start_sm(void)
{
	extern unsigned int sm_start, sm_end;
	/****************************************************************
	 * Load and start SM
	 ****************************************************************/
	//		if not warm down or warm up, always load system manage image
	//		the cases are:	cold boot up, warm up after onlineUpgrade
	if(( MV_SoC_STATE_SYSRESET == uiBoot ) ||
	   (MV_SoC_STATE_COLDBOOTUP == uiBoot ) ||
	   ( ( MV_SoC_STATE_SYSRESET != uiBoot ) && ( MV_SoC_STATE_WARMUP_0 != uiBoot ) &&
	     ( MV_SoC_STATE_WARMDOWN_1 != uiBoot ) && (MV_SoC_STATE_WARMUP_1 != uiBoot) &&
	     ( MV_SoC_STATE_WARMDOWN_2 != uiBoot ) && (MV_SoC_STATE_WARMUP_2 != uiBoot)
		  && (MV_SoC_STATE_WARMUP_FROM_DDR_OFF_1 != uiBoot)	&& (MV_SoC_STATE_BOOT_RECOVERY_KERNEL != uiBoot)
	   ))
		  // make Beetle "DDR_Off" case like the other warmups: there's no need to reload
		  // SM. Even though DDR is gone, it doesn't affect SM.
	{
		//	when box product release is defined, SM will shut down SoC after power on.
		//	this can be done by write the cold boot flag to SM's register
#ifdef	BOX_PRODUCT_RELEASE
		if(MV_SoC_STATE_SYSRESET != uiBoot) {
#else
		if(MV_SoC_STATE_COLDBOOTUP == uiBoot) {
#endif
			debug_printf("Set cold boot flag: %08x@%08x\n", MV_SM_RSTFLAG_COLD, SM_BOOT_FLAG_REG);
			writel(MV_SM_RSTFLAG_COLD, SM_BOOT_FLAG_REG);
		}

		debug_printf("start to kick off SM CPU: start 0x%08x, size %d\n", sm_start, sm_end - sm_start);
		//	download SM image from DDR to ITCM and kick off SM
		MV_SM_Dev_Init((unsigned char*)sm_start, sm_end - sm_start);
		debug_printf("SM CPU is kicked off.\n");

	}
	return 0;
}
#endif //CONFIG_SM

int Image_Load_And_Start(void)
{
	int ret= 0;
#ifdef ENABLE_CONNECT_BTN
		/* the second time to check connect btn */
		Check_connectbtnGPIO();
#endif

	get_reset_msg();

#ifdef CONFIG_SM
	load_and_start_sm();
#endif //CONFIG_SM

#ifdef ENABLE_CONNECT_BTN
		/* the third time to check connect btn */
		Check_connectbtnGPIO();
#endif
	/****************************************************************
	 * Read and setup kernel Image
	 ****************************************************************/
	Bootmode = check_android_recovery_mode();
	if (Bootmode == BOOTMODE_BOOTUSB)
		ret = load_android_image(Bootmode, BOOT_SRC_USB);
	else
		ret = load_android_image(Bootmode, BOOT_SRC_NAND);
	if (ret != 0 && Bootmode == BOOTMODE_NORMAL) {
		lgpl_printf("Invalid normal boot image! Boot recovery image...\n");
		write_android_bootloader_message("boot-recovery", "", "recovery\n--show_text\n");
		Bootmode = BOOTMODE_RECOVERY;
		ret = load_android_image(Bootmode, BOOT_SRC_NAND);
	} else if (ret != 0 && Bootmode == BOOTMODE_RECOVERY) {
		lgpl_printf("Invalid recovery boot image! Boot normal image...\n");
		/* Note: we don't clear the recovery flags, we just let device
		 *       to enter recovery mode if recovery mode is avaliable.
		 * write_android_bootloader_message("", "", "");
		 */

		Bootmode = BOOTMODE_NORMAL;
		ret = load_android_image(BOOTMODE_NORMAL, BOOT_SRC_NAND);
	}
	if (ret != 0) {
		/* Booting from NAND failed, try boot from USB */
		lgpl_printf("Booting from NAND failed, booting from USB....!\n");
		Bootmode = BOOTMODE_BOOTUSB;
		ret = load_android_image(Bootmode, BOOT_SRC_USB);
	}
	if (ret != 0){
		lgpl_printf("FATAL ERROR! There is no bootable image on this machine!\n");
		return -1;
	}
#ifdef CONFIG_SM
	notify_sm();
#endif //CONFIG_SM

	/* Turn on White LED indicating successful
	 * image validation.
	 */
	set_led(1, 0);
	/*
	 * setup linux boot parameter
	 */
	setup_android_kernel_param(Bootmode);

#ifdef CONFIG_SM
	notify_SM_of_Bootmode(Bootmode); 	// SM behaves differently for recovery vs normal
#endif //CONFIG_SM

	return 0;
}

#else /* !#if ANDROID_BOOT */
/* For
 * - Linux
 * - Android needs to load SM from bootimgs
 */
int Image_Load_And_Start()
{
	int iPageSize = 0;
	int iBlockSize  ;

	int ret= 0;

	unsigned int img3_start, img3_end;
	unsigned int en_size;

	char *sm_buff, *cpu0_buff, *cpu1_buff, *en_buff;
	int sm_addr, cpu0_addr, cpu1_addr, recou_addr, en_addr;

	long long chipid;

	iPageSize = nand_data.szofpg ;
	iBlockSize = nand_data.szofblk;

#ifdef ENABLE_CONNECT_BTN
		/* the second time to check connect btn */
		Check_connectbtnGPIO();
#endif

#ifdef	NAND_BOOT
	if((vt_img3.part1_version.major_version > vt_img3.part2_version.major_version)
		|| ((vt_img3.part1_version.major_version == vt_img3.part2_version.major_version)
		 &&(vt_img3.part1_version.minor_version > vt_img3.part2_version.minor_version)))
	{
		img3_start = vt_img3.part1_start_blkind;
		img3_end = img3_start + vt_img3.part1_blks;
		img3_start *= iBlockSize;
		img3_end *= iBlockSize;
	}
	else
	{
		img3_start = vt_img3.part2_start_blkind;
		img3_end = img3_start + vt_img3.part2_blks;
		img3_start *= iBlockSize;
		img3_end *= iBlockSize;
	}
#endif

#ifdef	EMMC_BOOT
	iBlockSize = nand_data.szofblk;
	if((vt_img3.part1_version.major_version > vt_img3.part2_version.major_version)
		|| ((vt_img3.part1_version.major_version == vt_img3.part2_version.major_version)
		 &&(vt_img3.part1_version.minor_version > vt_img3.part2_version.minor_version)))

	{
		img3_start = vt_img3.part1_start_blkind;
		img3_end = img3_start + vt_img3.part1_blks;
		img3_start *= iBlockSize;
		img3_end *= iBlockSize;
	}
	else
	{
		img3_start = vt_img3.part2_start_blkind;
		img3_end = img3_start + vt_img3.part2_blks;
		img3_start *= iBlockSize;
		img3_end *= iBlockSize;
	}

#endif

	/****************************************************************
	 * Read Image 3 Header
	 ****************************************************************/
	debug_printf("Image3 start:%08x, end: %08x\n", img3_start, img3_end);

#ifdef	NAND_BOOT
	debug_printf("Reading image3 attribute ...\n");

	sm_addr = nand_read_generic(img3_start, img3_end, (char *)(&img3_hdr), sizeof(Image3_Attr));
	if(sm_addr < 0)
	{
		lgpl_printf("Read image3 header error!\n");
		return -1;
	}
#endif
#ifdef	EMMC_BOOT
	//	Here we read out all the IM3, from img3_start to img3_end
	//	This can be optimized after we add more paramters in img3
	//	to indicate the real size of im3
	ret = do_emmc_switch_part(0);
	if (ret) {
		lgpl_printf("EMMC: Switch to user partition error!\n");
	}
	debug_printf("Read all the IM3...");

	int blks = (img3_end - img3_start) / SDIO_BLK_SIZE + 1;

#if 1
	int cur, offset = 0;
	do {
		/*
		 * The 65535 constraint comes from some hardware has
		 * only 16 bit width block number counter
		 */
		cur = (blks > 65535) ? 65535 : blks;

		ret = do_emmcread((unsigned long long )img3_start + offset * SDIO_BLK_SIZE, cur, SDIO_DATA_BUFFER + offset * SDIO_BLK_SIZE);
		if (ret != 0){
			lgpl_printf("EMMC: read image3 fail.\n");
			return ret;
		}

		blks -= cur;
		offset += cur;
	} while (blks > 0);
#else
	ret = do_emmcread(img3_start, blks, SDIO_DATA_BUFFER);
	if (ret) {
		lgpl_printf("EMMC: read image3 fail.\n");
		return ret;
	}
#endif

	debug_printf("Done\n");
	UtilMemCpy(&img3_hdr, SDIO_DATA_BUFFER, sizeof(Image3_Attr)) ;
	sm_addr = SDIO_DATA_BUFFER + nand_data.szofblk ;
#endif
	strcpy(img3_hdr.linux_bootargs, LINUX_PARAM_COMMON_PART);
//	debug_printf("Image3 atrribute:\n");
//	for(k=0;k<sizeof(Image3_Attr)/4;k++)
//		debug_printf("0x%08x.\n", ((int *)&img3_hdr)[k]);

	sm_hdr = &img3_hdr.sm_param;
	cpu0_hdr = &img3_hdr.cpu0_param;
	cpu1_hdr = &img3_hdr.cpu1_param;
	recou_hdr = &img3_hdr.recou_param;
	sMem_Layout = &img3_hdr.mem_layout;

#ifdef CONFIG_FORCE_ENCRYPTION
	sm_hdr->bsm_image_encrypt = 1;
	cpu0_hdr->bcpu0_image_encrypt = 1;
	cpu1_hdr->bcpu1_image_encrypt = 1;
#endif /* CONFIG_FORCE_ENCRYPTION */

	cpu0_addr = get_next_img_addr(sm_addr, img3_end, sm_hdr->uism_image_final_size);
	cpu1_addr = get_next_img_addr(cpu0_addr, img3_end, cpu0_hdr->uicpu0_image_final_size);
	recou_addr = get_next_img_addr(cpu1_addr, img3_end, cpu1_hdr->uicpu1_image_final_size);
	en_addr = get_next_img_addr(recou_addr, img3_end, recou_hdr->size);

	debug_printf("sm_addr: %08x, cpu0_addr: %08x, cpu1_addr: %08x, recou_addr: %08x, en_addr: %08x\n",
			sm_addr, cpu0_addr, cpu1_addr, recou_addr, en_addr);
	debug_printf("Memory map: CPU0 %uM, CPU1 %uM, ZSP: %uM, SHR %uM\n",
			sMem_Layout->uicpu0_mem_size>>20, sMem_Layout->uicpu1_mem_size>>20,
			sMem_Layout->uizsp_mem_size>>20, sMem_Layout->uishm_mem_size>>20);
	if((cpu0_addr < 0) || (cpu1_addr < 0) || (recou_addr < 0) || (en_addr < 0))
	{
		lgpl_printf("Scan image3 error.\n");
		lgpl_printf("sm_addr: %08x, cpu0_addr: %08x, cpu1_addr: %08x, recou_addr: %08x, en_addr: %08x\n",
			sm_addr, cpu0_addr, cpu1_addr, recou_addr, en_addr);
		return -1;
	}

#ifdef	NAND_BOOT
	if(nand_read_generic(en_addr, img3_end, (char*)&en_hdr, sizeof(en_hdr)) < 0)
	{
		lgpl_printf("Read image3 header error!\n");
		return -1;
	}
#endif

#ifdef	EMMC_BOOT
	UtilMemCpy(&en_hdr, en_addr, sizeof(en_hdr)) ;
#endif

//	debug_printf("Encryption atrribute:\n");
//	for(k=0;k<sizeof(en_hdr)/4;k++) debug_printf("0x%08x.\n", ((int *)&en_hdr)[k]);
	// currently, we don't support CPU1 image
	en_size = en_hdr.uiCPU0_Encrypt_Image_Header_Offset + en_hdr.uiCPU0_Encrypt_Image_Header_Size;
	debug_printf("Encryption image size= %d\n", en_size);
	en_buff = UtilMemAlloc(en_size);
	if(!en_buff) {
		lgpl_printf("Malloc failed.\n");
		return -1;
	}
#ifdef	NAND_BOOT
	if(nand_read_generic(en_addr, img3_end, en_buff, en_size) < 0)
	{
		lgpl_printf("Read image3 header error!\n");
		return -1;
	}
#endif

#ifdef	EMMC_BOOT
	UtilMemCpy(en_buff, en_addr, en_size) ;
#endif
	/*
	 * clear reset_msg_extra_size to an initial value
	 */
	get_reset_msg();

	debug_printf("encryption header copied\n");
#ifdef CONFIG_SM
	/****************************************************************
	 * Load and start SM
	 ****************************************************************/
	//		if not warm down or warm up, always load system manage image
	//		the cases are:	cold boot up, warm up after onlineUpgrade
	if(( MV_SoC_STATE_SYSRESET == uiBoot ) ||
	   (MV_SoC_STATE_COLDBOOTUP == uiBoot ) ||
	   ( ( MV_SoC_STATE_SYSRESET != uiBoot ) && ( MV_SoC_STATE_WARMUP_0 != uiBoot ) &&
	     ( MV_SoC_STATE_WARMDOWN_1 != uiBoot ) && (MV_SoC_STATE_WARMUP_1 != uiBoot) &&
	     ( MV_SoC_STATE_WARMDOWN_2 != uiBoot ) && (MV_SoC_STATE_WARMUP_2 != uiBoot)
		  && (MV_SoC_STATE_WARMUP_FROM_DDR_OFF_1 != uiBoot)	&& (MV_SoC_STATE_BOOT_RECOVERY_KERNEL != uiBoot)
		  // make Beetle "DDR_Off" case like the other warmups: there's no need to reload 
		  // SM. Even though DDR is gone, it doesn't affect SM.

				/*  ( MV_SoC_STATE_SYSRESET != uiBoot ) */ ))
	{
		//	for cold boot up, Image-2 load SM's image and kick off Sm


		//	when box product release is defined, SM will shut down SoC after power on.
		//	this can be done by write the cold boot flag to SM's register
#ifdef	BOX_PRODUCT_RELEASE
		if(MV_SoC_STATE_SYSRESET != uiBoot) {
#else
		if(MV_SoC_STATE_COLDBOOTUP == uiBoot) {
#endif
			debug_printf("Set cold boot flag: %08x@%08x\n", MV_SM_RSTFLAG_COLD, SM_BOOT_FLAG_REG);
			writel(MV_SM_RSTFLAG_COLD, SM_BOOT_FLAG_REG);
		}

		// copy SM image from NAND
		debug_printf("Loading SM image... final size %u\n", sm_hdr->uism_image_final_size);
		if(!sm_hdr->bsm_image_encrypt)
			en_hdr.uiSM_Encrypt_Image_Header_Size = 0;

		sm_buff = UtilMemAlloc(sm_hdr->uism_image_final_size + en_hdr.uiSM_Encrypt_Image_Header_Size);
		if(!sm_buff) {
			lgpl_printf("Malloc failed.\n");
			return -1;
		}

		UtilMemCpy(sm_buff, en_buff + en_hdr.uiSM_Encrypt_Image_Header_Offset, en_hdr.uiSM_Encrypt_Image_Header_Size);

#ifdef	NAND_BOOT
		if(nand_read_generic(sm_addr, cpu0_addr, (char *)(sm_buff+en_hdr.uiSM_Encrypt_Image_Header_Size), sm_hdr->uism_image_final_size) < 0)
		{
			lgpl_printf("Read sm image error!\n");
			return -1;
		}
#endif

#ifdef	EMMC_BOOT
		UtilMemCpy((char *)(sm_buff+en_hdr.uiSM_Encrypt_Image_Header_Size), sm_addr, sm_hdr->uism_image_final_size) ;
#endif
		if(sm_hdr->bsm_image_encrypt) {
			debug_printf("Encryption key index: %u\n", sm_buff[0]);
			ret = VerifyImage((unsigned int)sm_buff, sm_hdr->uism_encrypt_image_size, (unsigned int)sm_buff);
			if(ret) {
				lgpl_printf("Verify SM image failed! ret=%d\n", ret);
				return -1;
			}
			debug_printf("verify passed.\n");
		}

		if(sm_hdr->bsm_image_bss_init)
			UtilMemSet(sm_buff + sm_hdr->uism_bss_start_addr - sm_hdr->uism_image_load_addr, 0, sm_hdr->uism_bss_length);

#ifdef ENABLE_CONNECT_BTN
		/* the third time to check connect btn */
		Check_connectbtnGPIO();
#endif

		debug_printf("Done.\n");
		//	download SM image from DDR to ITCM and kick off SM
		MV_SM_Dev_Init((unsigned char*)sm_buff, sm_hdr->uism_image_ori_size ) ;
		debug_printf("SM CPU is kicked off.\n");

		//	send msg to SM to power off SoC
		//	FIGO_SRAM_ENTRY() ;
	}
#endif // CONFIG_SM
#if 0
	if(uiBoot == MV_SoC_STATE_WARMDOWN_2){
			stop_flag = 1 ;
			while(1) ;
	}
#endif

//	load_flashless_data();

//	load_sysconf();

	/****************************************************************
	 * Read and setup kernel Image
	 ****************************************************************/

	cpu0_buff = (char *)cpu0_hdr->uicpu0_image_load_addr - sizeof(linux_hdr_t);
	if((uiBoot != MV_SoC_STATE_WARMDOWN_2)){

		debug_printf("Loading CPU0 image... load address: 0x%x, final size %u\n", (int)cpu0_buff,cpu0_hdr->uicpu0_image_final_size);
		if(cpu0_hdr->bcpu0_image_encrypt)
			UtilMemCpy(cpu0_buff, en_buff + en_hdr.uiCPU0_Encrypt_Image_Header_Offset, en_hdr.uiCPU0_Encrypt_Image_Header_Size);
		else
			en_hdr.uiCPU0_Encrypt_Image_Header_Size = 0;
#ifdef NAND_BOOT
		if(nand_read_generic(cpu0_addr, cpu1_addr, (char *)(cpu0_buff+en_hdr.uiCPU0_Encrypt_Image_Header_Size), cpu0_hdr->uicpu0_image_final_size) < 0)
		{
			lgpl_printf("Read CPU0 image error!\n");
			return -1;
		}
#endif

#ifdef	EMMC_BOOT
		UtilMemCpy((char *)(cpu0_buff+en_hdr.uiCPU0_Encrypt_Image_Header_Size), cpu0_addr, cpu0_hdr->uicpu0_image_final_size) ;
#endif

		if(cpu0_hdr->bcpu0_image_encrypt) {
			debug_printf("Encryption key index: %u\n", cpu0_buff[0]);
			ret = VerifyImage((unsigned int)cpu0_buff, cpu0_hdr->uicpu0_encrypt_image_size, (unsigned int)cpu0_buff);
			if(ret) {
				lgpl_printf("Verify cpu0 image failed!ret=%d\n", ret);
				return -1;
			}
			debug_printf("verify passed.\n");
		}

	//	Xiufeng:	do not use ramdisk for BG2
#if 1
	{
		linux_hdr_t *lhdr = cpu0_buff;
		int len;

		debug_printf("kernel_size=%d, ramdisk_size=%d, ramdisk_addr=%x\n",
				lhdr->kernel_size, lhdr->ramdisk_size, lhdr->ramdisk_addr);
		debug_printf("reloading ramdisk...");
#ifdef NAND_BOOT
		UtilMemCpy(lhdr->ramdisk_addr,
				cpu0_hdr->uicpu0_image_load_addr+lhdr->kernel_size,
				lhdr->ramdisk_size);
#elif defined(EMMC_BOOT)
		UtilMemCpy((unsigned char *)0x6000000,
			cpu0_hdr->uicpu0_image_load_addr+lhdr->kernel_size,
			lhdr->ramdisk_size);

		UtilMemCpy(lhdr->ramdisk_addr,(unsigned char *)0x6000000, lhdr->ramdisk_size);
#endif
		debug_printf("DONE!\n");

		debug_printf("append initrd to linux bootargs\n");
		len = strlen((char *)img3_hdr.linux_bootargs);
		lgpl_sprintf(img3_hdr.linux_bootargs + len, " initrd=0x%x,0x%x",
				lhdr->ramdisk_addr, lhdr->ramdisk_size);
		debug_printf("%s\n", img3_hdr.linux_bootargs);
	}
#endif
	if(cpu0_hdr->bcpu0_image_bss_init)
		UtilMemSet((void*)cpu0_hdr->uicpu0_bss_start_addr, 0, cpu0_hdr->uicpu0_bss_length);

	}//endof if(uiBoot != MV_SoC_STATE_WARMDOWN_2)
	debug_printf("Done.\n");

#ifdef CONFIG_SM
	notify_sm();
#endif // CONFIG_SM
		/*
		 * setup linux boot parameter
		 */
		//	if MV88DE3010, try to use CPUID as MAC address
		chipid = readl(MV88DE3100_CHIPID_REG1) ;
		chipid += (long long)(readl(MV88DE3100_CHIPID_REG2)) << 32;
#ifdef	NAND_BOOT
		if(chipid && block0.first_burn_flag){
#endif
#ifdef	EMMC_BOOT
		//	 a hack
		if(chipid){
#endif
			char tmp_param[1024];
			char tmp_buf[64];
			init_mac_address(chipid, (char *)(&img3_hdr.linux_bootargs), tmp_param);
			if (reset_msg_extra_size) {
				int orig_len = strlen(tmp_param);
				tmp_param[orig_len] = ' ';
				UtilMemCpy(tmp_param+orig_len+1, reset_msg_extra_buf, reset_msg_extra_size);
				tmp_param[orig_len+1+reset_msg_extra_size] = 0;
			}

			debug_printf("Real bootargs: %s\n", tmp_param);
			setup_linux_bootparam(cpu0_hdr->uicpu0_image_load_addr-0x8000, sMem_Layout->uicpu0_mem_size, tmp_param);
		} else {
			char tmp_param1[1024];
			char mac_addr[32];

			get_mac_addr(mac_addr);
			set_wol_macaddr(&mac_addr[8]);

			strncpy(tmp_param1, img3_hdr.linux_bootargs, sizeof(tmp_param1));
			strcat(tmp_param1, " ");
			strncat(tmp_param1, mac_addr, sizeof(mac_addr));
			debug_printf("%s,%d: reset_msg_extra_size=%d\n",
					__func__, __LINE__, reset_msg_extra_size);
			if (reset_msg_extra_size == 0) {
				char tmp_buf[64];
				//	Xiufeng:	Fix me
				//	The No.2 parameter uicpu0_mem_size, is not used by Linux.

				debug_printf("REal bootargs: %s\n", tmp_param1);
				setup_linux_bootparam(cpu0_hdr->uicpu0_image_load_addr-0x8000,
					sMem_Layout->uicpu0_mem_size, tmp_param1);

			} else {
				char tmp_param[1024];
				char tmp_buf[64];
				int orig_len = strlen(tmp_param1);

				UtilMemCpy(tmp_param, tmp_param1, orig_len);
				tmp_param[orig_len] = ' ';
				UtilMemCpy(tmp_param+orig_len+1, reset_msg_extra_buf, reset_msg_extra_size);
				tmp_param[orig_len+1+reset_msg_extra_size] = 0;

				debug_printf("REAl bootargs: %s\n", tmp_param);
				setup_linux_bootparam(cpu0_hdr->uicpu0_image_load_addr-0x8000,
					sMem_Layout->uicpu0_mem_size, tmp_param);
			}
			debug_printf("%d: Done!\n", __LINE__);
		}
#if	0
			setup_linux_bootparam(cpu0_hdr->uicpu0_image_load_addr-0x8000,
					sMem_Layout->uicpu0_mem_size, LINUX_BOOTARGS);

#endif
		//	Xiufeng: Here we set CHIP_CTRL_SW_GENERIC3_REG to 1 to keep consistent with Berlin
		//		 In ideal case, BG2 Linux does not need NFC sync, but bootflow does not
		//		 rely on this
		reg(CHIP_CTRL_SW_GENERIC3_REG) = 1;//allow Linux to use NFC

	return 0;
}
#endif /* #if ANDROID_BOOT */

// if here, Image-3 is copied into DDR
int Image_3_Verify_Jump(void)
{
	char * tmp_buff;
	int ret;

	volatile int stop_flag = 0;

	//	write 0xBF0A0001 to CHIP_CTRL_SW_GENERIC0_REG
	//	This is the flag to eCos passed by Bootflow. This value means the AVPLL
	//	has been set by bootflow, eCos shall not reset AVPLL
	GA_REG_WORD32_WRITE(CHIP_CTRL_SW_GENERIC0_REG, 0xBF0A0001) ;

	// any state reach here should start/resume kernel
#if ANDROID_BOOT
		if( (uiWarmDown_2_Linux_Addr != 0xFFFFFFFF) && (uiBoot == MV_SoC_STATE_WARMUP_1)){
			// resume
			lgpl_printf("Resume Linux at address: 0x%08x\n", uiWarmDown_2_Linux_Addr);
			/* we don't care the parameters when resume */
			__reset_cpu(uiWarmDown_2_Linux_Addr, 1330, 0) ;
		}else{
			start_android_kernel();
		}
#else /* #if ANDROID_BOOT */

		reg(CHIP_CTRL_SW_GENERIC3_REG) = 0x1 ;
		if( (uiWarmDown_2_Linux_Addr != 0xFFFFFFFF) && (uiBoot == MV_SoC_STATE_WARMUP_1)){
			// resume
			reg(CHIP_CTRL_SW_GENERIC1_REG) = uiWarmDown_2_Linux_Addr ;
			lgpl_printf("JUMP to kernel entrance 0x%08x\n", uiWarmDown_2_Linux_Addr);
			__reset_cpu(uiWarmDown_2_Linux_Addr, 1330, cpu0_hdr->uicpu0_image_load_addr-0x8000) ;
		}else{
			lgpl_printf("JUMp to kernel entrance 0x%08x\n", cpu0_hdr->uicpu0_image_load_addr);
			//parameter0: Linux load address
			//parameter1: 1330
			//parameter2: Linux Command line address
			__reset_cpu(cpu0_hdr->uicpu0_image_load_addr, 1330, cpu0_hdr->uicpu0_image_load_addr-0x8000) ;
		}
#endif /* #if ANDROID_BOOT */

	return 0;
}
#ifdef EMMC_BOOT
/* increase emmc dev id.
 */
unsigned int emmc_dev_id_inc(unsigned int dev_id)
{
	dev_id++;

#ifndef CONFIG_GPT
	/* if we reach EBR area, need add 1 to skip it */
	if (dev_id == 4)
		dev_id++;
#endif
	return dev_id;
}
#endif

void dump_version_entry(ver_table_entry_t * vte)
{
	printf("%s: part1(start=%d, blks=%d, version=%08u%04u), part2(start=%d, blks=%d, version=%08u%04u)\n", vte->name,
			vte->part1_start_blkind, vte->part1_blks, vte->part1_version.major_version, vte->part1_version.minor_version,
			vte->part2_start_blkind, vte->part2_blks, vte->part2_version.major_version, vte->part2_version.minor_version);
}

unsigned int crc32(unsigned int crc, unsigned char *buf, unsigned int len);
#define CRC32_SIZE		(4)
int get_version_table()
{
	unsigned int i,j,ret=0;
	int img3_found;
#if ANDROID_BOOT
	int android_kernel_found=0;
	int android_recovery_found=0;
#endif
	char temp_buff[4096 + 32];
	version_table_t *vt = (version_table_t *)temp_buff;
	ver_table_entry_t * vt_entry;
	unsigned vt_size = 0;
	unsigned int dev_id = 0;

#ifdef	NAND_BOOT
	for(i=1; i<9;i++) {
		if(is_block_bad((loff_t)i*nand_data.szofblk)) {
			debug_printf("Bad block found @0x%08x.\n", i*nand_data.szofblk);
			continue;
		}
		ret = mv_nand_read_block( (long long)i*nand_data.szofblk + VT_OFFSET,
			(unsigned char *)temp_buff, sizeof(temp_buff) );
		if( ret != sizeof(temp_buff) ) {
			debug_printf("Read version table failed, i=%u, ret=%u.\n", i, ret);
			continue;
		}
		vt_size = sizeof(version_table_t) + vt->num_entries * sizeof(ver_table_entry_t);
		/* verify the version table data */
		if(vt->magic != MAGIC_NUMBER || 0xffffffff != crc32(0, temp_buff, vt_size + CRC32_SIZE))
			continue;

		dev_id = 0;
		img3_found = 0;
		for(j=0;j<vt->num_entries;j++) {
			vt_entry = &vt->table[j];
			printf("[%02d,d%02d] ", j, dev_id);
			dump_version_entry(vt_entry);
			if(UtilMemCmp(IMG3_NAME, vt_entry->name, sizeof(IMG3_NAME)) == 0) {
				UtilMemCpy(&vt_img3, vt_entry, sizeof(ver_table_entry_t));
				img3_found = 1;
			}
			if(UtilMemCmp(SYSCONF_NAME, vt_entry->name, sizeof(SYSCONF_NAME)) == 0) {
				UtilMemCpy(&vt_sysconf, vt_entry, sizeof(ver_table_entry_t));
			}
			if(UtilMemCmp(FLASHLESS_NAME, vt_entry->name, sizeof(FLASHLESS_NAME)) == 0) {
				UtilMemCpy(&vt_flashless, vt_entry, sizeof(ver_table_entry_t));
			}
#if ANDROID_BOOT
			if(UtilMemCmp(FTS_NAME, vt_entry->name, sizeof(FTS_NAME)) == 0) {
				fts_dev_id = dev_id;
				UtilMemCpy(&vt_fts, vt_entry, sizeof(ver_table_entry_t));
			}
			if(UtilMemCmp(KERNEL_NAME, vt_entry->name, sizeof(KERNEL_NAME)) == 0) {
				UtilMemCpy(&vt_kernel, vt_entry, sizeof(ver_table_entry_t));
				android_kernel_found = 1;
			}
			if(UtilMemCmp(RECOVERY_NAME, vt_entry->name, sizeof(RECOVERY_NAME)) == 0) {
				UtilMemCpy(&vt_recovery, vt_entry, sizeof(ver_table_entry_t));
				android_recovery_found = 1;
			}
#endif
			if (vt_entry->part1_start_blkind != vt_entry->part2_start_blkind)
				/* double copy of the partition */
				dev_id += 2;
			else
				dev_id += 1;
		}
#if ANDROID_BOOT
		if(img3_found && android_kernel_found && android_recovery_found)
			break;
#else
		if(img3_found)
			break;
#endif
	}

	if(i==9) {
		lgpl_printf("Scan version table error!\n");
		return -1;
	}
#endif

#ifdef	EMMC_BOOT
	char *real_buff = (char *)(((unsigned int)temp_buff + 31) & (~0x1f));
	vt = (version_table_t *)(real_buff);

	for (i = 1; i <= 2; i++) {
		debug_printf("EMMC: Load version table from boot partition %d.\n", i);
		ret = do_emmc_switch_part(i);
		if(ret){
			debug_printf("EMMC: switch to boot partion %d error.\n", i);
			continue;
		}

		ret = do_emmcread(0xd000ULL, 8, real_buff);
		if(ret){
			debug_printf("EMMC: read version table error\n");
			continue;
		}

		vt_size = sizeof(version_table_t) + vt->num_entries * sizeof(ver_table_entry_t);

		if(vt->magic != MAGIC_NUMBER || 0xffffffff != crc32(0, real_buff, vt_size + CRC32_SIZE)) {
			debug_printf("EMMC: MAGIC NUMBER or CRC error.\n");
			continue;
		}

		dev_id = 0;
		img3_found = 0;
		for(j=0;j<vt->num_entries;j++) {
			vt_entry = &vt->table[j];
			printf("[%02d,sd%02d] ", j, dev_id);
			dump_version_entry(vt_entry);
			if(UtilMemCmp(IMG3_NAME, vt_entry->name, sizeof(IMG3_NAME)) == 0) {
				UtilMemCpy(&vt_img3, vt_entry, sizeof(ver_table_entry_t));
				img3_found = 1;
			}
			if(UtilMemCmp(SYSCONF_NAME, vt_entry->name, sizeof(SYSCONF_NAME)) == 0) {
				UtilMemCpy(&vt_sysconf, vt_entry, sizeof(ver_table_entry_t));
			}
			if(UtilMemCmp(FLASHLESS_NAME, vt_entry->name, sizeof(FLASHLESS_NAME)) == 0) {
				UtilMemCpy(&vt_flashless, vt_entry, sizeof(ver_table_entry_t));
			}

#if ANDROID_BOOT
			if(UtilMemCmp(FTS_NAME, vt_entry->name, sizeof(FTS_NAME)) == 0) {
				fts_dev_id = dev_id;
				UtilMemCpy(&vt_fts, vt_entry, sizeof(ver_table_entry_t));
			}
			if(UtilMemCmp(KERNEL_NAME, vt_entry->name, sizeof(KERNEL_NAME)) == 0) {
				UtilMemCpy(&vt_kernel, vt_entry, sizeof(ver_table_entry_t));
				android_kernel_found = 1;
			}
			if(UtilMemCmp(RECOVERY_NAME, vt_entry->name, sizeof(RECOVERY_NAME)) == 0) {
				UtilMemCpy(&vt_recovery, vt_entry, sizeof(ver_table_entry_t));
				android_recovery_found = 1;
			}
#endif

			dev_id = emmc_dev_id_inc(dev_id);
			if (vt_entry->part1_start_blkind != vt_entry->part2_start_blkind) {
				/* double copy of the partition */
				dev_id = emmc_dev_id_inc(dev_id);
			}
		}

#if ANDROID_BOOT
		if(img3_found && android_kernel_found && android_recovery_found)
			break;
#else
		if(img3_found)
			break;
#endif
	}

	if(i==3) {
		lgpl_printf("Scan version table error!\n");
		return -1;
	}

#endif

	return 0;
}

void dump_mem(unsigned int addr, int count)
{
	int i;
	for (i = 0; i < count; i++, addr += 4) {
		if (i % 4 == 0) {
			if (i)
				printf("\n");
			printf("0x%08x: ", addr);
		}
		printf("%08x ", readl(addr));
	}
	printf("\n");
}

void check_stop_watchdog(unsigned int base)
{
#define WDT_CTRL		(0x00)
#define WDT_CRR			(0x0C)
#define WDT_STAT		(0x10)
#define WDT_EOI			(0x14)
#define WDT_CTRL_EN		(1 << 0)
#define WDT_STAT_INTR		(1 << 0)

	unsigned int wdt_en   = readl(base + WDT_CTRL) & WDT_CTRL_EN;
	unsigned int wdt_intr = readl(base + WDT_STAT) & WDT_STAT_INTR;
	if ((MV_SoC_STATE_COLDBOOTUP == uiBoot) ||
		(MV_SoC_STATE_SYSRESET == uiBoot) ||
		(wdt_en && wdt_intr)) {
		/* once WDT is enabled, we can't disable it.
		 * so we just restart it (write magic number 0x76 to CRR).
		 */
		writel(0x76, base + WDT_CRR);
		/* clear the pending interrupt */
		readl(base + WDT_EOI);

		/* set bootflag to SYSRESET or CLLDSYSRESET */
		if((MV_SoC_STATE_COLDBOOTUP == uiBoot) ||
			(MV_SoC_STATE_WARMDOWN_1 == uiBoot) ||
			(MV_SoC_STATE_WARMDOWN_2 == uiBoot))
			uiBoot = MV_SoC_STATE_COLDBOOTUP;
		else
			uiBoot = MV_SoC_STATE_SYSRESET;
	}
}

static unsigned int read_otp_status(void)
{
	return readl(MV88DE3XXX_OTP_STS_REG);
}

static unsigned int read_otp_lkg_curr(void)
{
	return readb(MV88DE3XXX_OTP_LKG_ID) * 4;
}

void StartBootLoader(int block_size, int page_size, int addr_cycle)
{
	int ret = 0;
#ifndef CONFIG_SM
	/* if no SM, always in SYSPOWERON state */
	uiBoot = MV_SoC_STATE_SYSPOWERON;
	uiWarmDown_2_Linux_Addr = 0;
#else // CONFIG_SM
	uiBoot =  readl( SM_BOOT_FLAG_REG );
	uiWarmDown_2_Linux_Addr = readl((SM_WARMDOWN_2_LINUX_ADDR)) ;
	/* check and stop sm watchdog if there is pending wdt interrupt */
	if (uiBoot != MV_SoC_STATE_SYSPOWERON) {
		check_stop_watchdog(SM_APB_WDT0_BASE);
		check_stop_watchdog(SM_APB_WDT1_BASE);
		check_stop_watchdog(SM_APB_WDT2_BASE);
	}
#endif // CONFIG_SM
	pad_init();
	pin_init();

	board_gpio_init();
	/* Turn on red LED */
	set_led(0, 1);
	init_dbg_port(SM_APB_UART0_BASE) ;

	lgpl_printf("\n%s %s [%s %s] ver:%s\n",
				BOARD_NAME, CHIP_VERSION, __DATE__, __TIME__,
				BOOTLOADER_VERSION);
	lgpl_printf("OTP status=0x%08X lkg curr=%d mA\n",
				read_otp_status(), read_otp_lkg_curr());
#if defined(DEBUG)
	if ((uiBoot != MV_SoC_STATE_WARMUP_1) || (uiWarmDown_2_Linux_Addr == 0xFFFFFFFF)) {
		lgpl_printf("Pinmux configuration:\n");

#ifdef CONFIG_SM
		lgpl_printf("\nGSM 0x%08x 0x%08x;\n", (SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL),
				readl((SM_SYS_CTRL_REG_BASE + RA_smSysCtl_SM_GSM_SEL)));
#endif // CONFIG_SM
		lgpl_printf("GSOC 0x%08x 0x%08x;\n", (MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux),
				readl(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux));
		lgpl_printf("GSOC1 0x%08x 0x%08x;\n", (MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1),
				readl(MEMMAP_CHIP_CTRL_REG_BASE + RA_Gbl_pinMux1));
		lgpl_printf("\n");
	}
#endif

#if (defined(PV_COMP) && (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0))
	pv_comp();
#endif

#if (defined(CPUPLL_1200) && (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0))
	initCpuPll_1200();
#endif

#ifdef CONFIG_SM
	set_sm_ldo_ctrl_vout(SM_LDO_CTRL_VOUT_1_011V);
#endif // CONFIG_SM

	bootloader_customize_initialize_postprocess(uiBoot);

	/* if it's resume, then we just start kernel after do some initializations */
	if ((uiBoot == MV_SoC_STATE_WARMUP_1) && (uiWarmDown_2_Linux_Addr != 0xFFFFFFFF))
		goto START;
#if defined(DEBUG) || (CFG_BOARD_NAME <= BOARD_EUREKA_B1)
	dump_mem(0xF7EA0014, 4);	// SYSPLL Registers
	dump_mem(0xF7EA0028, 4);	// MEMPLL Registers
	dump_mem(0xF7EA003C, 4);	// CPUPLL Registers
	lgpl_printf("Clock configuration:\n");
	show_speed();
#endif //DEBUG || CFG_BOARD_NAME


#ifdef	NAND_BOOT
	// initialize nand flash
	init_nfc(block_size, page_size, addr_cycle);
#endif

#ifdef	EMMC_BOOT
	nand_data.szofblk = block_size;
	nand_data.szofpg = 8192;

	ret = do_emmcinit() ;
	if (ret) {
		printf("emmc init fail.\n");
		while(1);
	}
	//reconfigure NFC
#endif

	UtilMemReset();

#if BOOTLOADER_FASTLOGO
        switch (uiBoot) {
        case MV_SoC_STATE_WARMUP_1:
        case MV_SoC_STATE_WARMUP_2:
                if (uiWarmDown_2_Linux_Addr != 0xFFFFFFFF)
                        break;
        case 0:
        case MV_SoC_STATE_SYSRESET:
        case MV_SoC_STATE_COLDBOOTUP:
        case MV_SoC_STATE_WARMUP_0:
        case MV_SoC_STATE_WARMUP_FROM_DDR_OFF_1:
                printf("[FASTLOGO] init.");

                // enable SOC AVPLL
                //printf("[FASTLOGO] init AVPLL.\n");
                AVPLL_Enable();

                //printf("[FASTLOGO] init DHubs.\n");
                DhubInitialization(0, VPP_DHUB_BASE, VPP_HBO_SRAM_BASE,
                        &VPP_dhubHandle, VPP_config, VPP_NUM_OF_CHANNELS);
                DhubInitialization(0, AG_DHUB_BASE, AG_HBO_SRAM_BASE,
                        &AG_dhubHandle, AG_config, AG_NUM_OF_CHANNELS);

                //printf("[FASTLOGO] create THINVPP.\n");
                MV_THINVPP_Create(MEMMAP_VPP_REG_BASE, THINVPP_OPTION_SHOWLOGO);
                //printf("\n[FASTLOGO] reset.");
                MV_THINVPP_Reset();
                //printf("\n[FASTLOGO] config.");
                MV_THINVPP_Config();
                printf("\n[FASTLOGO] Set CPCB1 output reso %d.", RES_525P5994);
                MV_THINVPP_SetCPCBOutputResolution(CPCB_1, RES_525P5994, OUTPUT_BIT_DEPTH_8BIT);
#if (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
                printf("\n[FASTLOGO] Set CPCB3 output reso %d.", RES_NTSC_M);
                MV_THINVPP_SetCPCBOutputResolution(CPCB_3, RES_NTSC_M, OUTPUT_BIT_DEPTH_8BIT);
#endif // (BERLIN_CHIP_VERSION != BERLIN_BG2CD_A0)
#if BOOTLOADER_SHOWLOGO
                showlogo_start((void *)LOGO_FRM_BUF);
#endif //BOOTLOADER_SHOWLOGO
                printf("\n[FASTLOGO] done.\n");
                break;
        default:
                break;
        }
#endif //BOOTLOADER_FASTLOGO

#ifndef ANDROID_BOOT
	//always get version table for all the boot state
	if(get_version_table()) {
		lgpl_printf("get version table error!\n");
		while(1);
	}
#endif //! ANDROID_BOOT
#if ANDROID_BOOT

#ifdef EMMC_BOOT
	if (do_emmc_switch_part(0)){
		lgpl_printf("Fail to switch user partition.\n");
	}
#endif
	if( (MV_SoC_STATE_COLDBOOTUP == uiBoot) ||
			( (uiBoot != MV_SoC_STATE_WARMUP_0) &&
			  (uiBoot != MV_SoC_STATE_WARMUP_1) &&
			  (uiBoot != MV_SoC_STATE_WARMUP_2) &&
			  (uiBoot != MV_SoC_STATE_WARMDOWN_2) ) )
	{
#ifdef ENABLE_CONNECT_BTN
		/* the first time to check connect btn */
		Check_connectbtnGPIO();
#endif

	}
	ret = flash_ts_init(vt_fts.part1_start_blkind, vt_fts.part1_blks);
	if (ret){
		lgpl_printf("flash_ts_init() init fail.\n");
		while(1);
	}
#endif

	if( (MV_SoC_STATE_COLDBOOTUP == uiBoot) || (MV_SoC_STATE_WARMUP_FROM_DDR_OFF_1 == uiBoot) ||
	    ( (uiBoot != MV_SoC_STATE_WARMUP_0) &&
	      (uiBoot != MV_SoC_STATE_WARMUP_1) &&
	      (uiBoot != MV_SoC_STATE_WARMUP_2) &&
	      (uiBoot != MV_SoC_STATE_WARMDOWN_2) ) )
	{
		// note: in the case of STATE_WARMUP_0/1/2, we expect RAM is OK. In the case of 
		// Beetle DDR_OFF, it's not. So make sure Image_Load_And_Start is called for the Beetle case.
		// note: MV_SoC_STATE_BOOT_RECOVERY_KERNEL takes this branch.

		bootloader_customize_load_image_preprocess(uiBoot);
#ifdef ENABLE_CONNECT_BTN
		Check_connectbtnGPIO();
#endif
		debug_printf("Not warm up.\n");

		if(Image_Load_And_Start())
		{
			lgpl_printf("tbdzz---- Img_Ld_And_Start error! Spinning now!\n");
			while(1);
		}

		bootloader_customize_load_image_postprocess(uiBoot);
	}

START:
#ifdef CONFIG_SM
	debug_printf("Responding to SM...\n");
	bootloader_customize_response_to_sm_preprocess(uiBoot);
	response_to_sm();
	debug_printf("Warm up.\n");
#endif // CONFIG_SM

#ifdef EMMC_BOOT
#ifdef EXTERNAL_EMMC
	/* code below is for the factory tool to switch from external EMMC to on-board EMMC */
#define EMMC_SWITCH_GPIO 4
#define GPIO_VALUE_FOR_ON_BOARD_EMMC 1

	GPIO_PortWrite(EMMC_SWITCH_GPIO, GPIO_VALUE_FOR_ON_BOARD_EMMC);
#endif
#endif

	bootloader_customize_start_kernel_preprocess(uiBoot);
#if BOOTLOADER_SHOWLOGO
	showlogo_stop();
#endif //BOOTLOADER_SHOWLOGO
	Image_3_Verify_Jump() ;
	while(1);
}

/* TODO(kolla): This seem to be a result of statically linking libgcc.a,
 * one of the u-boot forums recommended locally defining this function.
 * Find cleaner fix.
 */
void __aeabi_unwind_cpp_pr0(void)
{
}
