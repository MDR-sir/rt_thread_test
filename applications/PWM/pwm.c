#include <rtthread.h>
#include <rtdevice.h>
#include "pwm.h"
#include "finsh.h"

struct rt_device_pwm     *pwm_device = RT_NULL; /* PWM设备句柄 */

static int pwm_init(void)
{
		/* 查找设备 */
    pwm_device = (struct rt_device_pwm *)rt_device_find(PWM_DEVICE);
    if (pwm_device == RT_NULL)
    {
        rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEVICE);
        return RT_ERROR;
    }
		    /* 设置PWM周期和脉冲宽度默认值 */
    rt_pwm_set(pwm_device, PWM_CHANNEL, PWM_PERIOD, 0);
    /* 使能设备 */
    rt_pwm_enable(pwm_device, PWM_CHANNEL);
		return 0;
}

void pwm_on(void)
{
    rt_pwm_enable(pwm_device, PWM_CHANNEL); //使能对应的 PWM 通道
}

void pwm_off(void)
{
    rt_pwm_disable(pwm_device, PWM_CHANNEL); //关闭对应的 PWM 通道
}

void pwm_set(rt_uint32_t pulse)
{
    rt_uint32_t period;

		period = 500000;    /* 周期为0.5ms，单位为纳秒ns */
    /* 利用 PWM API 设定 周期和占空比 */
    rt_pwm_set(pwm_device, PWM_CHANNEL, period, pulse);//channel,period,pulse

}     

void pwm_led_sample(void)
{
    rt_uint32_t period, pulse, dir, times;

		pwm_init();
	
    period = PWM_PERIOD;    /* 周期为0.5ms，单位为纳秒ns */
		pulse = 0;          /* PWM脉冲宽度值，单位为纳秒ns */
    dir = 1;            /* PWM脉冲宽度值的增减方向 */
		times = 0 ;

    while ((times++)<400)//2个周期
    {
        rt_thread_mdelay(50);
        if (dir)
        {
            pulse += 5000;      /* 从0值开始每次增加5000ns */
        }
        else
        {
            pulse -= 5000;      /* 从最大值开始每次减少5000ns */
        }
        if (pulse >= period)
        {
            dir = 0;
        }
        if (0 == pulse)
        {
            dir = 1;
        }

        /* 设置PWM周期和脉冲宽度 */
        rt_pwm_set(pwm_device, PWM_CHANNEL, period, pulse);
    }
		/* 关闭设备 */
    rt_pwm_disable(pwm_device, PWM_CHANNEL);
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(pwm_led_sample, pwm sample);
