#include <Galois_memmap.h>
#include <global.h>
#include "io.h"
#include "pwm_driver.h"
#include "galois_speed.h"

/* set_led_blacklight: input channel: 0, 1
		       input duty:0-100
*/
void set_led_blacklight(int channel, int duty)
{
    int pwm_freq;
    PWM_SETTINGS pwm_setting ;

    pwm_setting.En = 1;
    pwm_setting.PreScale = 1 ;
    pwm_setting.Polarity = 0 ;

    pwm_freq = GaloisGetFrequency(SOC_FREQ_CFG);
    pwm_setting.Duty = pwm_freq*duty/100;
    pwm_setting.Tcnt= pwm_freq;

    pwm_write_settings(channel, &pwm_setting) ;
    pwm_on_off(channel, &pwm_setting, 1) ;

    return;
}
