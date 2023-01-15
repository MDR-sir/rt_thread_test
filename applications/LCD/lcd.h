#ifndef __LCD_H
#define __LCD_H	


//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)




void LCD_Fill(rt_uint16_t x_start, rt_uint16_t y_start, rt_uint16_t x_end, rt_uint16_t y_end, rt_uint16_t color);
void LCD_Fill_Colors(rt_uint16_t x_start, rt_uint16_t y_start, rt_uint16_t x_end, rt_uint16_t y_end, rt_uint16_t *colors);
void LCD_DrawPoint(rt_uint16_t x, rt_uint16_t y,rt_uint32_t color);
void LCD_DrawLine(rt_uint16_t x1, rt_uint16_t y1, rt_uint16_t x2, rt_uint16_t y2, rt_uint16_t color);
void LCD_DrawRectangle(rt_uint16_t x1, rt_uint16_t y1, rt_uint16_t x2, rt_uint16_t y2, rt_uint16_t color);
void LCD_Draw_Circle(rt_uint16_t x0,rt_uint16_t y0,rt_uint8_t r, rt_uint16_t color);
void LCD_ShowChar(rt_uint16_t x,rt_uint16_t y,rt_uint8_t num,rt_uint8_t size,rt_uint8_t mode, rt_uint16_t point_color, rt_uint16_t back_color);
void LCD_ShowNum(rt_uint16_t x, rt_uint16_t y, rt_uint32_t num, rt_uint8_t len, rt_uint8_t size, rt_uint16_t point_color, rt_uint16_t back_color);
void LCD_ShowxNum(rt_uint16_t x,rt_uint16_t y,rt_uint32_t num,rt_uint8_t len,rt_uint8_t size,rt_uint8_t mode, rt_uint16_t point_color, rt_uint16_t back_color);
void LCD_ShowString(rt_uint16_t x, rt_uint16_t y, rt_uint16_t width, rt_uint16_t height, rt_uint8_t size, rt_uint8_t *p, rt_uint16_t point_color, rt_uint16_t back_color);

int lcd_test(void);


#endif
