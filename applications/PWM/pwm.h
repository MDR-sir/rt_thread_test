#ifndef PWM_H
#define PWM_H

#include <rtthread.h>

#define PWM_DEVICE  "pwm2"
#define PWM_CHANNEL      4
#define PWM_PERIOD	500000

void pwm_on(void);
void pwm_off(void);
void pwm_set(rt_uint32_t pulse);


#endif
