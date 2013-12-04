#include <memmap.h>
#include <global.h>
#include "io.h"
#include "pwm_driver.h"
#include "galois_speed.h"

#define PWM_CHANNEL     2

struct duty_volt {
    int duty;
    int volt;
};

static struct duty_volt dv_table[] =
{
    {25, 1025},
    {50, 1000},
    {63, 975},
    {80, 950},
    {-1, -1},
};


int pwm(int volt)
{
    int pwm_freq;
    struct duty_volt *p;
    PWM_SETTINGS pwm_setting ;

    pwm_setting.En = 1;
    pwm_setting.PreScale = 1 ;
    pwm_setting.Polarity = 0 ;
    p = dv_table;
    while(p->duty != -1) {
        if(p->volt == volt)
            break;
        p++;
    }
    if(p->duty == -1)
        return -1;
    pwm_freq = GaloisGetFrequency(SOC_FREQ_CFG);
    pwm_setting.Duty = pwm_freq*p->duty/100;
    pwm_setting.Tcnt= pwm_freq;

    pwm_write_settings(PWM_CHANNEL, &pwm_setting) ;
    pwm_on_off(PWM_CHANNEL, &pwm_setting, 1) ;

    return 0;
}
