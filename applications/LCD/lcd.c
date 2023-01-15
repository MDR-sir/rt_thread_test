#include <rtthread.h>
#include "drv_lcd.h"
#include "font.h" 
#include "lcd.h"
#include "stdio.h"
#include "adc.h" 
#include "usart.h"

#define LCD_BASE ((uint32_t)(0x6C000000 | 0x0000007E))
#define LCD ((LCD_CONTROLLER_TypeDef *)LCD_BASE)


//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Fill(rt_uint16_t x_start, rt_uint16_t y_start, rt_uint16_t x_end, rt_uint16_t y_end, rt_uint16_t color)
{          
	rt_uint16_t i,j;
	rt_uint16_t x_len=0; 
	x_len=x_end-x_start+1;	 
	for(i=y_start;i<=y_end;i++)
	{
		LCD_SetCursor(x_start,i);      				//设置光标位置 
		LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
          for (j = 0; j < x_len; j++)
          {
               LCD->RAM=color;     //设置光标位置
          }
     }
}
void LCD_Fill_Colors(rt_uint16_t x_start, rt_uint16_t y_start, rt_uint16_t x_end, rt_uint16_t y_end, rt_uint16_t *colors)
{
    
     rt_uint16_t i,j;
     rt_uint16_t height,width ;
     width=x_end-x_start+1; 			//得到填充的宽度
     height=y_end-y_start+1;			//高度
     for(i=0;i<height;i++ )
     {
          LCD_SetCursor(x_start, y_start+i);
          LCD_WriteRAM_Prepare();
          for(j = 0; j < width; j++)
          {
               LCD->RAM = colors[i * width + j];
          }
          
     }
}

void LCD_DrawPoint(rt_uint16_t x, rt_uint16_t y,rt_uint32_t color)
{	   
 if (lcddev.id == 0X5510)
    {
        LCD_WR_REG(lcddev.setxcmd);
        LCD_WR_DATA(x >> 8);
        LCD_WR_REG(lcddev.setxcmd + 1);
        LCD_WR_DATA(x & 0XFF);
        LCD_WR_REG(lcddev.setycmd);
        LCD_WR_DATA(y >> 8);
        LCD_WR_REG(lcddev.setycmd + 1);
        LCD_WR_DATA(y & 0XFF);
    }
    else if (lcddev.id == 0X1963)
    {
        if (lcddev.dir == 0)x = lcddev.width - 1 - x;

        LCD_WR_REG(lcddev.setxcmd);
        LCD_WR_DATA(x >> 8);
        LCD_WR_DATA(x & 0XFF);
        LCD_WR_DATA(x >> 8);
        LCD_WR_DATA(x & 0XFF);
        LCD_WR_REG(lcddev.setycmd);
        LCD_WR_DATA(y >> 8);
        LCD_WR_DATA(y & 0XFF);
        LCD_WR_DATA(y >> 8);
        LCD_WR_DATA(y & 0XFF);
    }
    else     //9341/5310/7789等设置坐标
    {
        LCD_WR_REG(lcddev.setxcmd);
        LCD_WR_DATA(x >> 8);
        LCD_WR_DATA(x & 0XFF);
        LCD_WR_REG(lcddev.setycmd);
        LCD_WR_DATA(y >> 8);
        LCD_WR_DATA(y & 0XFF);
    }

    LCD->REG = lcddev.wramcmd; 
    LCD->RAM = color; 
}



//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(rt_uint16_t x1, rt_uint16_t y1, rt_uint16_t x2, rt_uint16_t y2, rt_uint16_t color)
{
		    rt_uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;

    if (delta_x > 0)
        incx = 1; //设置单步方向
    else if (delta_x == 0)
        incx = 0; //垂直线
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; //水平线
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)
        distance = delta_x; //选取基本增量坐标轴
    else
        distance = delta_y;

    for (t = 0; t <= distance + 1; t++) //画线输出
    {
        LCD_DrawPoint(uRow, uCol, color); //画点
        xerr += delta_x;
        yerr += delta_y;

        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}	

//画矩形
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(rt_uint16_t x1, rt_uint16_t y1, rt_uint16_t x2, rt_uint16_t y2, rt_uint16_t color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}



//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_Draw_Circle(rt_uint16_t x0,rt_uint16_t y0,rt_uint8_t r, rt_uint16_t color)
{
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);       //判断下个点位置的标志

    while (a <= b)
    {
        LCD_DrawPoint(x0 + a, y0 - b, color);        //5
        LCD_DrawPoint(x0 + b, y0 - a, color);        //0
        LCD_DrawPoint(x0 + b, y0 + a, color);        //4
        LCD_DrawPoint(x0 + a, y0 + b, color);        //6
        LCD_DrawPoint(x0 - a, y0 + b, color);        //1
        LCD_DrawPoint(x0 - b, y0 + a, color);
        LCD_DrawPoint(x0 - a, y0 - b, color);        //2
        LCD_DrawPoint(x0 - b, y0 - a, color);        //7
        a++;

        //使用Bresenham算法画圆
        if (di < 0)di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}



//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24/32
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(rt_uint16_t x,rt_uint16_t y,rt_uint8_t num,rt_uint8_t size,rt_uint8_t mode, rt_uint16_t point_color, rt_uint16_t back_color)
{  							  
    rt_uint8_t temp,t1,t;
	rt_uint16_t y0=y;
	rt_uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else if(size==32)temp=asc2_3216[num][t];	//调用3216字体
		else return;								//没有的字库
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_DrawPoint(x,y,point_color);
			else if(mode==0)LCD_DrawPoint(x,y,back_color);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//超区域了
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//超区域了
				break;
			}
		}
	}
}

//m^n函数
//返回值:m^n次方.
static rt_uint32_t LCD_Pow(rt_uint8_t m,rt_uint8_t n)
{
	rt_uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}

//显示数字,高位为0,则不显示
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);
void LCD_ShowNum(rt_uint16_t x, rt_uint16_t y, rt_uint32_t num, rt_uint8_t len, rt_uint8_t size, rt_uint16_t point_color, rt_uint16_t back_color)
{
    rt_uint8_t t, temp;
    rt_uint8_t enshow = 0;

    for (t = 0; t < len; t++)
    {
        temp = (num / LCD_Pow(10, len - t - 1)) % 10;

        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                LCD_ShowChar(x + (size / 2)*t, y, ' ', size, 0, point_color, back_color);
                continue;
            }
            else enshow = 1;

        }

        LCD_ShowChar(x + (size / 2)*t, y, temp + '0', size, 0, point_color, back_color);
    }
}



//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);	 
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(rt_uint16_t x,rt_uint16_t y,rt_uint32_t num,rt_uint8_t len,rt_uint8_t size,rt_uint8_t mode, rt_uint16_t point_color, rt_uint16_t back_color)
{  
	rt_uint8_t t,temp;
	rt_uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01, point_color, back_color);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01, point_color, back_color);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01, point_color, back_color); 
	}
}


//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址
void LCD_ShowString(rt_uint16_t x, rt_uint16_t y, rt_uint16_t width, rt_uint16_t height, rt_uint8_t size, rt_uint8_t *p, rt_uint16_t point_color, rt_uint16_t back_color)
{
    rt_uint8_t x0 = x;
    width += x;
    height += y;

    while ((*p <= '~') && (*p >= ' '))   //判断是不是非法字符!
    {
        if (x >= width)
        {
            x = x0;
            y += size;
        }

        if (y >= height)break; //退出

        LCD_ShowChar(x, y, *p, size, 0, point_color, back_color);
        x += size / 2;
        p++;
    }  
}



/******************************************************************/
#define Thread_Stack_Size 512
#define Thread_Priority 25
#define Thread_Timeslice 5


static rt_thread_t lcd_thread = RT_NULL;

static void lcd_thread_entry(void* parameter)
{
    rt_device_t lcd = RT_NULL;
	
		rt_uint8_t lcd_id[12],str[50],msg_str[100];	
		rt_uint32_t vol;
		rt_err_t res;
	
    lcd = rt_device_find("lcd");
    rt_device_init(lcd);
	
		sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);
    LCD_ShowString(30,40,210,24,24,"RT_Thread_Test",RED,WHITE);	
    LCD_ShowString(30,70,200,16,16,"MY TEST",BLUE,WHITE);
    LCD_ShowString(30,90,200,16,16,"M.sir",GBLUE,WHITE);
    LCD_ShowString(30,110,200,16,16,lcd_id,YELLOW,WHITE);		//LCD ID	 
		
		while(1)
		{
			rt_mb_recv(vol_mb, &vol, 5);
			res = rt_mq_recv (msg_mq, msg_str,100, 5);
			sprintf((char*)str,"the voltage is :%d.%02d \n", vol / 100, vol % 100);
			LCD_ShowString(80,5,200,16,16,str,RED,WHITE);
			if(res == RT_EOK)
					LCD_ShowString(30,180,100,200,16,msg_str,BLUE,WHITE);
			
		}
}


int lcd_test()
{
		lcd_thread = rt_thread_create("lcd",
                                    lcd_thread_entry,RT_NULL,
                                    Thread_Stack_Size,
                                    Thread_Priority,
                                    Thread_Timeslice);
		
		
		if(lcd_thread != RT_NULL )
				rt_thread_startup(lcd_thread);
		else
				return -1;
		
		return 0;
}

MSH_CMD_EXPORT(lcd_test,a test of lcd)








