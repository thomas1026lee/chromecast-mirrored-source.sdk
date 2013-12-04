#ifndef	_IMAGE_2_IO_H
#define	_IMAGE_2_IO_H

#define	__chk_io_ptr(a)		(void)0	

#define __raw_writeb(v,a)   (__chk_io_ptr(a), *(volatile unsigned char *)(a) = (v))
#define __raw_writew(v,a)   (__chk_io_ptr(a), *(volatile unsigned short *)(a) = (v))
#define __raw_writel(v,a)   (__chk_io_ptr(a), *(volatile unsigned int *)(a) = (v))

#define __raw_readb(a)      (__chk_io_ptr(a), *(volatile unsigned char *)(a))
#define __raw_readw(a)      (__chk_io_ptr(a), *(volatile unsigned short *)(a))
#define __raw_readl(a)      (*(volatile unsigned int *)(a))


#define writeb	__raw_writeb
#define writew	__raw_writew
#define writel	__raw_writel

#define readb	__raw_readb
#define	readw	__raw_readw
#define	readl	__raw_readl

#ifndef BFM_HOST_Bus_Write32
#define BFM_HOST_Bus_Write32(a, v) (writel(v, a))
#endif

#ifndef BFM_HOST_Bus_Read32
#define BFM_HOST_Bus_Read32(a, h) (*h = readl(a))
#endif

#ifndef GA_REG_WORD32_WRITE
#define GA_REG_WORD32_WRITE(a, v) (writel(v, a))
#endif

#ifndef GA_REG_WORD32_READ
#define GA_REG_WORD32_READ(a, h) (*h = readl(a))
#endif

#endif
