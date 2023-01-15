#ifndef __ADC_H
#define __ADC_H	

#define ADC_DEV_NAME        "adc1"      /* ADC 设备名称 */
#define ADC_DEV_CHANNEL     6           /* ADC 通道 */
#define REFER_VOLTAGE       330         /* 参考电压 3.3V,数据精度乘以100保留2位小数*/
#define CONVERT_BITS        (1 << 12)   /* 转换位数为12位 */

extern rt_mailbox_t vol_mb;
extern char vol_mb_pool[4];

int adc_read(void);


#endif
