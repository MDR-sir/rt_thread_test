#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>  
#include "led.h"       //对应的头文件

int led_init(void)
{
    /* 设定 LED 引脚为推挽输出模式 */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
    return 0;
}

int led_on(void)
{
    /* 调用 API 输出低电平 */
    rt_pin_write(LED0_PIN, PIN_LOW);
    return 0;
}

int led_off(void)
{
    /* 调用 API 输出高电平 */
    rt_pin_write(LED0_PIN, PIN_HIGH);
    return 0;
}

int led_toggle(void)
{
    /* 调用 API 读出当前电平 然后输出相反电平 */
    rt_pin_write(LED0_PIN, !rt_pin_read(LED0_PIN));
    return 0;
}


/******************************************************************/
#define Thread_Stack_Size 512
#define Thread_Priority 25
#define Thread_Timeslice 5

static rt_thread_t led_thread = RT_NULL;
static rt_sem_t led_sem = RT_NULL;
static rt_sem_t led_sem_on = RT_NULL;
static rt_sem_t led_sem_off = RT_NULL;
static void led_thread_entry(void* parameter)
{
		led_init();
    while (1)
    {
				rt_sem_take(led_sem,RT_WAITING_FOREVER);
				led_toggle();
				rt_sem_release(led_sem);
        rt_thread_mdelay(500);
    }
}	


int led_test()
{
		led_thread = rt_thread_create("led",
																			led_thread_entry,RT_NULL,
																			Thread_Stack_Size,
																			Thread_Priority,
																			Thread_Timeslice);
		led_sem = rt_sem_create("led",1,RT_IPC_FLAG_FIFO);
		led_sem_on = rt_sem_create("led on",0,RT_IPC_FLAG_FIFO);
		led_sem_off = rt_sem_create("led off",1,RT_IPC_FLAG_FIFO);
		
		if(led_thread != RT_NULL && led_sem != RT_NULL && led_sem_on != RT_NULL && led_sem_off != RT_NULL)
				rt_thread_startup(led_thread);
		else
				return -1;
		
		return 0;
}


MSH_CMD_EXPORT(led_test,a test of led)



static void LED_ON()
{
		rt_err_t res;
		res=rt_sem_trytake(led_sem_on);
		if(res == RT_EOK)
		{
			rt_sem_release(led_sem);
			rt_sem_release(led_sem_off);
		}
}

MSH_CMD_EXPORT(LED_ON,start led test)


static void LED_OFF()
{
		rt_err_t res;
		res=rt_sem_trytake(led_sem_off);
		if(res == RT_EOK)
		{
			rt_sem_take(led_sem,RT_WAITING_FOREVER);
			rt_sem_release(led_sem_on);
		}
}

MSH_CMD_EXPORT(LED_OFF,close led test)



static void idlehook_indicator_lamp()
{
	static int idle_cnt=0;
	if(idle_cnt>=500000)
	{
		rt_pin_write(indicator_lamp_PIN, !rt_pin_read(indicator_lamp_PIN));
//		rt_kprintf("idle\n");
		idle_cnt=0;
	}
	idle_cnt++;
}

void indicator_lamp_init()
{
	rt_pin_mode(indicator_lamp_PIN, PIN_MODE_OUTPUT);
	rt_thread_idle_sethook(idlehook_indicator_lamp);
}



