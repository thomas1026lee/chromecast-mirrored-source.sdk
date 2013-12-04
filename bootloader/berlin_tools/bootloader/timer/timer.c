/*
 * (C) Copyright 2010
 * Marvell Semiconductor <www.marvell.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <galois_speed.h>
#include "apb_timer.h"
#include "timer.h"

unsigned int timer_clk_freq;
static inline unsigned long read_timer(void)
{
	return TIMERx_GetCurrentValue(GALOIS_UBOOT_TIMER) \
	      / (timer_clk_freq/ 1000);
}

static unsigned long timestamp;
static unsigned long lastdec;

void reset_timer_masked (void)
{
	/* reset time */
	lastdec = read_timer();  /* capure current decrementer value time */
	timestamp = 0;	       /* start "advancing" time stamp from 0 */
}

unsigned long get_timer_masked (void)
{
	unsigned long now = read_timer();

	if (lastdec >= now) {
		/* normal mode */
		timestamp += lastdec - now;
	} else {
		/* we have an overflow ... */
		timestamp += lastdec +
			(TIMER_LOAD_VAL / (timer_clk_freq/ 1000)) - now;
	}
	lastdec = now;

	return timestamp;
}

/*
 * timer without interrupts
 */

void reset_timer (void)
{
	reset_timer_masked ();
}

unsigned long get_timer (unsigned long base)
{
	return get_timer_masked () - base;
}

void set_timer (unsigned long t)
{
	timestamp = t;
}

static inline unsigned long uboot_cntr_val(void)
{
	return TIMERx_GetCurrentValue(GALOIS_UBOOT_TIMER);
}

void __udelay(unsigned long usec)
{
	unsigned int current;
	unsigned long delayticks;

	current = uboot_cntr_val();
	delayticks = (usec * (timer_clk_freq/ 1000000));

	if (current < delayticks) {
		delayticks -= current;
		while (uboot_cntr_val() < current)
			;
		while ((TIMER_LOAD_VAL - delayticks) < uboot_cntr_val())
			;
	} else {
		while (uboot_cntr_val() > (current - delayticks))
			;
	}
}
#ifndef CONFIG_WD_PERIOD
# define CONFIG_WD_PERIOD	(10 * 1000 * 1000)	/* 10 seconds default*/
#endif
void udelay(unsigned long usec)
{
	unsigned long kv;

	do {

		kv = usec > CONFIG_WD_PERIOD ? CONFIG_WD_PERIOD : usec;
		__udelay (kv);
		usec -= kv;
	} while(usec);
}

#define INT_UNMASK_BIT		0
#define INT_MASK_BIT		1
#define INT_ENABLE_BIT		1
#define INT_DISABLE_BIT		0

int timer_init (void)
{
	/* call low level code to init timer, TODO: don't include low level code like this */
	TIMERx_SetControlReg(GALOIS_UBOOT_TIMER, INT_MASK_BIT, INT_DISABLE_BIT, TIMER_USR_DEFINED_MODE);
	TIMERx_SetLoadCount(GALOIS_UBOOT_TIMER, TIMER_LOAD_VAL);
	/* INT_ENABLE_BIT is mis-name of ENABLE_BIT */
	TIMERx_SetIntEn(GALOIS_UBOOT_TIMER, INT_ENABLE_BIT);

	/* init the timestamp and lastdec value */
	reset_timer_masked();

#if (ASIC_PLATFORM)
	timer_clk_freq = 1000000*GaloisGetFrequency(SOC_FREQ_CFG);
	printf("timer_clk_freq = 0x%x \n",timer_clk_freq);
#else
	timer_clk_freq = CONFIG_SYS_HZ;
#endif

	return 0;
}
int timer_stop(void)
{
	TIMERx_SetIntEn(GALOIS_UBOOT_TIMER, INT_DISABLE_BIT);
	return 0;
}
