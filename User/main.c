#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "MY_time_Conf.h"
#include "lcd.h"
#include "adc.h"
#include "string.h"
#include "LDC1000.h"
/**************************
模板配置信息：
选用芯片是 stm32f103vc
启动信息：stm32f10x_hd.s
固件库：stm32固件库v3.5
***************************/

#define led_PB12   PBout(12)	   //位绑定，位定义PB12口作为LED输出。这样速度最快
//#define led_PB1   PBout(1)	   //位绑定，位定义PB1口作为LED输出。这样速度最快
//#define PA9   PAout(10)
//#define CLK1   PDout(0)
//#define CWW1   PDout(1)
//#define CLK2   PDout(2)
//#define CWW2   PDout(3)


//uint8_t i = 0;    //测试用

float a[3],w[3],angle[3],T;
extern unsigned char Re_buf[11],temp_buf[11],counter;
extern unsigned char sign;

u8 cishu=0,cishu1=0;






/*************************************************
函数: int main(void)
功能: main主函数
参数: 无
返回: 无
**************************************************/

int main(void)
{
		unsigned char Temp[11];
		char xianshi[7];
		
		//u8 i;

		RCC_Configuration();      //初始化时钟
		delay_init(72);            //初始化延时函数
		ADC_Configuration();
		Timer2_Init(1000,7200);
	  TIM3_PWM_Init(7200,1);
	 	TIM_SetCompare1(TIM3,3600);
		TIM_SetCompare2(TIM3,3600);
		TIM_SetCompare3(TIM3,3600);
		TIM_SetCompare4(TIM3,3600);
		TIM4_Configuration();
		TIM1_Configuration();
		TIM5_PWM_Init(9,1);
		LDC_SPI_Init();
		LDC1000_init();
		USART1_Config();   //初始化 配置USART1
		USART2_Config();   //初始化 配置USART2 （用于接收MPU6050数据）
		LCD_Init();				//lcd显示初始化
		POINT_COLOR=RED;	//字体设置为红色
		delay_ms(200);
    LCD_ShowString(30,40,210,24,24, "anjian start ^_^");//显示测试  
   while (1)
   {
      if(sign)
      {  
         memcpy(Temp,Re_buf,11);
         sign=0;
         if(Re_buf[0]==0x55)       //检查帧头
         {  
            switch(Re_buf[1])
            {
               case 0x51: //标识这个包是加速度包
                  a[0] = ((short)(Temp[3]<<8 | Temp[2]))/32768.0*16;      //X轴加速度
                  a[1] = ((short)(Temp[5]<<8 | Temp[4]))/32768.0*16;      //Y轴加速度
                  a[2] = ((short)(Temp[7]<<8 | Temp[6]))/32768.0*16;      //Z轴加速度
                  T    = ((short)(Temp[9]<<8 | Temp[8]))/340.0+36.25;      //温度
                  break;
               case 0x52: //标识这个包是角速度包
                  w[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*2000;      //X轴角速度
                  w[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*2000;      //Y轴角速度
                  w[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*2000;      //Z轴角速度
                  T    = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;      //温度
                  break;
               case 0x53: //标识这个包是角度包
                  angle[0] = ((short)(Temp[3]<<8| Temp[2]))/32768.0*180;   //X轴滚转角（x 轴）
                  angle[1] = ((short)(Temp[5]<<8| Temp[4]))/32768.0*180;   //Y轴俯仰角（y 轴）
                  angle[2] = ((short)(Temp[7]<<8| Temp[6]))/32768.0*180;   //Z轴偏航角（z 轴）
                  T        = ((short)(Temp[9]<<8| Temp[8]))/340.0+36.25;   //温度

                  //printf("X轴角度：%.2f   Y轴角度：%.2f   Z轴角度：%.2f\r\n",angle[0],angle[1],angle[2]);
                  break;
               default:  break;
            }
						cishu=TIM4->CNT;
						cishu1=TIM1->CNT;
						sprintf(xianshi,"%06.2f",angle[2]);//z轴 显示输出
						LCD_ShowString(30,80,210,24,24,xianshi);
						
					
         }
         
      }
      //delay_ms(50);
   }
}




