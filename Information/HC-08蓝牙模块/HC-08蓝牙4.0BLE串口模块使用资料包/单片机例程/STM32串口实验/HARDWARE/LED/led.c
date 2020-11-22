#include "led.h"


//初始化PA5和PA4为输出口.并使能这两个口的时钟		    , PB6测试使用，可屏蔽
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PA端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //LED0-->PA.4 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.4
 GPIO_ResetBits(GPIOA,GPIO_Pin_4);						 //PA.4 输出低
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //PA.5 端口配置, 推挽输出
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOA,GPIO_Pin_5); 						 //PA.5 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	    		 //PB.6 端口配置, 推挽输出
 GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_ResetBits(GPIOB,GPIO_Pin_6); 						 //PB.6 输出低,控制3V3
	
}
 
