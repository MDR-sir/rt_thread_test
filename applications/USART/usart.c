#include "usart.h"
#include <rtthread.h>
#include <rtdevice.h>


static rt_device_t  usart_dev = RT_NULL;
static struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; 

static rt_thread_t  usart_workdata_thread = RT_NULL;
static rt_mailbox_t msg_size_mb = RT_NULL;

rt_mq_t  msg_mq;
static rt_uint8_t msg_pool[BSP_UART3_RX_BUFSIZE];

static rt_err_t rx_complete_callback(rt_device_t dev, rt_size_t size)
{
    rt_mb_send(msg_size_mb,size);
    return RT_EOK;
}

static void usart_workdata_thread_entry(void* parameter)
{
    char str[] = "open usart3 successfully!!!\r\n";
		rt_uint32_t size;
		static char rx_buffer[BSP_UART3_RX_BUFSIZE ];
	
    rt_device_write(usart_dev, 0, str, sizeof(str));
		
    while(1)
    {
				rt_mb_recv (msg_size_mb, &size, RT_WAITING_FOREVER);
        rt_device_read(usart_dev, 0, rx_buffer, size);
        rt_device_write(usart_dev, 0, rx_buffer, size);
			
				rx_buffer[size]='\0';
				rt_mq_send(msg_mq, rx_buffer, size+1);
    }
    
}

int usart_init()
{
		/* 初始化一个 mailbox */
    msg_size_mb = rt_mb_create("usart_msg_size",1,RT_IPC_FLAG_FIFO); 
	
		msg_mq = rt_mq_create("usart_msg", BSP_UART3_RX_BUFSIZE,
           1, RT_IPC_FLAG_FIFO);
	
    usart_dev = rt_device_find(USART_NAME);
    if(usart_dev == RT_NULL)
        return -1;

    config.rx_bufsz = 128; 
    rt_device_control(usart_dev, RT_DEVICE_CTRL_CONFIG, &config);
    rt_device_set_rx_indicate(usart_dev, rx_complete_callback);
    rt_device_open(usart_dev, RT_DEVICE_FLAG_RX_NON_BLOCKING | RT_DEVICE_FLAG_TX_BLOCKING); // 串口设备使用模式为 (发送阻塞 接收非阻塞) 模式
    
    usart_workdata_thread = rt_thread_create("usart",
                                            usart_workdata_thread_entry,RT_NULL,
                                            Thread_Stack_Size,
                                            Thread_Priority,
                                            Thread_Timeslice);
    if( usart_workdata_thread != RT_NULL && msg_size_mb != RT_NULL )
        rt_thread_startup(usart_workdata_thread);
    else
        return -2;
    
    return 0;
}
MSH_CMD_EXPORT(usart_init,usart test)

