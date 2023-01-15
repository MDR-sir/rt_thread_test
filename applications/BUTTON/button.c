#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>  
#include "button.h"       //对应的头文件

static BUTTONS Buttons;
static BUTTON button_1;
static void button_callback(BUTTON *button)
{
    switch(button->event){
        case BUTTON_EVENT_CLICK_DOWN:
            rt_kprintf("BUTTON_EVENT_CLICK_DOWN\n");
            break;
				case BUTTON_EVENT_CLICK_UP:
            rt_kprintf("BUTTON_EVENT_CLICK_UP\n");
            break;
				case BUTTON_EVENT_HOLD_DOWN:
            rt_kprintf("BUTTON_EVENT_HOLD_DOWN\n");
            break;
        case BUTTON_EVENT_HOLD_UP:
            rt_kprintf("BUTTON_EVENT_HOLD_UP\n");
            break;
        default:
            ;
    }
}

static void button_register(BUTTON *button,rt_base_t Pin)
{
    button->pin = Pin;
		rt_pin_mode(Pin, PIN_MODE_INPUT_PULLUP);
    button->event = BUTTON_EVENT_NONE;
    button->time = 0;
    button->cnt = 0;
    button->callback_func = (button_callback_func)button_callback;
    Buttons.buttons_list[Buttons.num++] = button;
}

static void button_scan(void *parameter)
{
    int i;
    rt_uint16_t time;
    int pin_state;
    for(i=0;i<Buttons.num;i++)
    {
        pin_state = rt_pin_read(Buttons.buttons_list[i]->pin);
        if(pin_state==Press_State)
        {
            Buttons.buttons_list[i]->time++;
            time=Buttons.buttons_list[i]->time;
            if(time == Click_Time)
						{
                Buttons.buttons_list[i]->event = BUTTON_EVENT_CLICK_DOWN;
								button_callback(Buttons.buttons_list[i]);
						}
            else if(time == Hold_Time)
						{
                Buttons.buttons_list[i]->event = BUTTON_EVENT_HOLD_DOWN;
								button_callback(Buttons.buttons_list[i]);
						}
        }
        else
        {
            time = Buttons.buttons_list[i]->time;
            if(time >= Click_Time && time < Hold_Time)
                Buttons.buttons_list[i]->event = BUTTON_EVENT_CLICK_UP;
            else if(time >= Hold_Time)
                Buttons.buttons_list[i]->event = BUTTON_EVENT_HOLD_UP;
            else
                Buttons.buttons_list[i]->event = BUTTON_EVENT_NONE;
            Buttons.buttons_list[i]->time = 0;
						button_callback(Buttons.buttons_list[i]);
        }
        
    }
}

static rt_timer_t button_timer = RT_NULL;

void Buttons_init()
{
    
		Buttons.num=0;

    button_register(&button_1,Button_PIN);
	/*****************软件定时器**********/
		button_timer = rt_timer_create("timer_button",
																		button_scan,
																		NULL,
																		Timer_outtime_ms,
																		RT_TIMER_FLAG_PERIODIC);
		if(button_timer != RT_NULL)
				rt_timer_start(button_timer);
	/*****************硬件定时器**********/
//		rt_device_t timer_dev = RT_NULL;
//		rt_hwtimer_mode_t mode = HWTIMER_MODE_PERIOD;                 
//		rt_hwtimerval_t timeout; 

//    timer_dev = rt_device_find(Timer_Device);
//    rt_device_open(timer_dev, RT_DEVICE_OFLAG_RDWR);
//    rt_device_set_rx_indicate(timer_dev, button_scan);
//    rt_device_control(timer_dev, HWTIMER_CTRL_MODE_SET, &mode);
//    timeout.sec = 0;      /* 秒 */
//    timeout.usec = Timer_outtime_ms*1000;     /* 微秒 */
//    rt_device_write(timer_dev, 0, &timeout, sizeof(timeout));

}






