#ifndef BUTTON_H
#define BUTTON_H

#include <rtthread.h>
#include <rtdevice.h>


#define Timer_Device "timer11"
#define Timer_outtime_ms  10
#define Click_Time    50/Timer_outtime_ms
#define Hold_Time     2000/Timer_outtime_ms
#define Press_State		PIN_LOW

#define Button_PIN    GET_PIN(E, 4)

typedef void (*button_callback_func)(void*);

typedef enum button_event{
    BUTTON_EVENT_NONE,        /* 无按键事件 */
    BUTTON_EVENT_CLICK_DOWN, /* 按键单击按下事件 */
    BUTTON_EVENT_CLICK_UP,   /* 按键单击结束事件 */
    BUTTON_EVENT_HOLD_DOWN,       /* 按键长按开始事件 */
    BUTTON_EVENT_HOLD_UP    /* 按键长按结束事件 */
}button_event;

typedef struct button_message{
    rt_base_t pin;
    button_event event;
    rt_uint16_t time;
    rt_uint16_t cnt;
    button_callback_func callback_func;
}BUTTON;

#define Buttons_List_Max 20

typedef struct buttons_messge{
    rt_uint8_t num;
    rt_timer_t timer;   
    BUTTON *buttons_list[Buttons_List_Max];
}BUTTONS;


void Buttons_init(void);


#endif
