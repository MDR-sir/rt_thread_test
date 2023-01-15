#ifndef __USART_H
#define __USART_H

#include <rtthread.h>
#include <rtdevice.h>

#define USART_NAME  "uart3"
#define Thread_Stack_Size 1024
#define Thread_Priority 25
#define Thread_Timeslice 5

extern rt_mq_t msg_mq;

int usart_init(void);
#endif
