#ifndef _USB_STORAGE_H_
#define _USB_STORAGE_H_
/** USB DRIVER API.
 *
 * Follow the steps to use it.
 * step 1, call usb_start() to initialize USB.
 * step 2, call usb_stor_scan() to scan usb storage devices.
 * step 3, loop calling usb_stor_read() to get image from scaned devices
 *		 and check validity of image.
 *
 * Take booting up daig.bin as an example.
 *
 * static int do_binary_check(unsigned char *usb_buf)
 * {
 * 	 return 0;
 * }
 * static int usb_stor_read_example()
 * {
 * 	 int usb_stor_cnt=0;
 * 	 int ret, i;
 * 	 unsigned int start_blk, size_blk;
 * 	 unsigned char *usb_buf = (unsigned char*)0x20000000;
 *
 * 	 start_blk = 1; 		 //start from the second block
 * 	 size_blk = (7<<20)/512; //size of daig.bin
 *
 * 	 ret = usb_init();
 * 	 // try to recognize storage devices immediately
 * 	 if (ret >= 0)
 * 		 usb_stor_cnt = usb_stor_scan();
 *
 * 	 for(i = 0;i < usb_stor_cnt; i++) //try to read binary from all scaned device.
 * 	 {
 * 		 memset(usb_buf,0,size_blk*512);
 *
 * 		 if(usb_stor_read(i, start_blk, size_blk, usb_buf) == size_blk)
 * 		 {
 * 			 printf("usb_read: end startblk %d, blccnt %d buffer 0x%x\n",
 * 			 start_blk, size_blk, usb_buf);
 * 			 if (do_binary_check(usb_buf) == 0)//passed check
 * 				 break;
 * 		 }
 * 	 }
 * 	 if (i == usb_stor_cnt)
 * 	 {
 * 		 printf("Can not get daig image.\n");
 * 		 if (usb_stor_cnt == 0)
 * 			 printf("No usb stor dev available.\n");
 * 		 while(1);
 * 	 }
 * __reset_cpu((unsigned int)usb_buf, 1330, 0);
 *  }
 */


/* Stop usb devices.
 *
 * Stops the LowLevel Part and deregisters USB devices.
 */
int usb_stop(void);

/* Init usb devices.
 *
 * usb init and scan usb device.
 *
 * @retval =0 		has found usb devices.
 * @retval = -1 		has not found usb devices.
 */
int usb_init();

/* Scan usb storage devices.
 *
 * scan the usb and reports device info.
 *
 * @retval 			the number of usb storage devices.
 */
int usb_stor_scan();

/** Usb storage read
 *
 *  @param device     	usb storage device index.
 *  @param blknr 	 	start address in block.
 *  @param blkcnt 	size in block.
 *  @param buffer        objective data buffer.
 *
 *  @retval: >0 		count of read blocks.
 *  @retval: =0 		read errror.
 */
unsigned long usb_stor_read(int device, unsigned long blknr,
			    unsigned long blkcnt, void *buffer);

#endif
