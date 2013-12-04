#ifndef	__TIMER__
#define	__TIMER__

#define ASIC_PLATFORM 1
#define BERLIN_PLATFORM 1

#define GALOIS_CPU0_TIMER 0
#define GALOIS_UBOOT_TIMER	(GALOIS_CPU0_TIMER)
#define TIMER_LOAD_VAL 0xffffffff

#if 0
#define GALOIS_TIMER_CLOCK (ASIC_PLATFORM == 1? \
		(BERLIN_PLATFORM == 1? 75000000 : 100000000) : \
		(BERLIN_PLATFORM == 1? 4500000 : 1500000))
#endif

#define GALOIS_TIMER_CLOCK 100000000
#ifndef CONFIG_SYS_HZ
#define CONFIG_SYS_HZ			(GALOIS_TIMER_CLOCK)
#endif

extern unsigned int timer_clk_freq;

unsigned long get_timer (unsigned long base);
void reset_timer (void);
void set_timer (unsigned long t);
void udelay(unsigned long usec);


#endif

