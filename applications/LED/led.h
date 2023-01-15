#ifndef LED_H
#define LED_H

#define LED0_PIN     GET_PIN(F, 9)
#define LED1_PIN     GET_PIN(F, 10)
#define indicator_lamp_PIN	LED1_PIN

int led_init(void);          //LED 灯初始化
int led_on(void);            //LED 灯亮
int led_off(void);           //LED 灯灭
int led_toggle(void);        //LED 灯亮灭状态翻转

int led_test(void);
void indicator_lamp_init(void);
#endif
