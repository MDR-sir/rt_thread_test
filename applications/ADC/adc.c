#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>  
#include "adc.h"       //对应的头文件



#define Thread_Stack_Size 512
#define Thread_Priority 25
#define Thread_Timeslice 5


rt_mailbox_t vol_mb =RT_NULL;
static rt_thread_t adc_thread = RT_NULL;


static void adc_thread_entry(void *parameter)
{
    rt_adc_device_t adc_dev = RT_NULL;
    rt_uint32_t value,vol;

    adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
    

    while(1)
    {
        /* 使能设备 */
        rt_adc_enable(adc_dev, 6);
            /* 读取采样值 */
        value = rt_adc_read(adc_dev, 6);

        /* 转换为对应电压值 */
        vol = value * REFER_VOLTAGE / CONVERT_BITS;
        /* 关闭通道 */
        rt_adc_disable(adc_dev, ADC_DEV_CHANNEL);
        rt_mb_send(vol_mb,vol);
//        rt_kprintf("the voltage is :%d.%02d \n", vol / 100, vol % 100);
        rt_thread_mdelay(100);
    }
    
}



int adc_read()
{
		/* 初始化一个 mailbox */
    vol_mb = rt_mb_create("voltage",1,RT_IPC_FLAG_FIFO); 
	
    adc_thread = rt_thread_create("adc",
                                adc_thread_entry,RT_NULL,
                                Thread_Stack_Size,
                                Thread_Priority,
                                Thread_Timeslice);
    if(adc_thread != RT_NULL && vol_mb != RT_NULL)
            rt_thread_startup(adc_thread);
    else
            return -1;
    
    return 0;
}

//MSH_CMD_EXPORT(adc_read,a test of adc)
