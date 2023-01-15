#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "led.h" 
#include "button.h"
#include "lcd.h"
#include "usart.h"

int main(void)
{	
		indicator_lamp_init();
		Buttons_init();
	  adc_read();
		usart_init();
		lcd_test();
    return RT_EOK;
}



