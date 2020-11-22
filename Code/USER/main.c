#include "led.h"
#include "delay.h"
#include "sys.h"

#include "usart.h"
#include "hc05.h"
#include "usart3.h"			 	 
#include "string.h"	   
#include "usmart.h"	
#include "timer.h"
#include "control.h"
 /************************************************
	Bluetooth£º  
			PA4-->KEY
			PB10-->RXD
			PB11-->TXD
			GND-->GND
			5V-->VCC 
	Direction control£º
			PC0-->FRONT_LEFT-IN1
			PC1-->FRONT_LEFT-IN2
			PC2-->FRONT_RIGHT-IN1
			PC3-->FRONT_RIGHT-IN2
			PC4-->BACK_LEFT-IN1
			PC5-->BACK_LEFT-IN2
			PC6-->BACK_RIGHT-IN1
			PC7-->BACK_RIGHT-IN2
	Speed control£º
			PB6-->FRONT_LEFT
			PB7-->FRONT_RIGHT
			PB8-->BACK_LEFT
			PB9-->BACK_RIGHT
************************************************/


	
u32 temp1=0,temp2=0,temp3=0,temp4=0;
extern u16 time[2];
extern u8 biti; 
	




 int main(void)
 {	 
	
		  
	u8 reclen=0;
	
	delay_init();	    	 //Delay function initialization	 
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//Set NVIC interruption 2:2 to seize priority£¬2-digit response priority
	uart_init(115200);	 	//The initial serial port is 115200
	TIM3_PWM_Int_Init(99,7199);
	LED_Init();				//Initialize the hardware interface connected to the LED
 			
	usmart_dev.init(72); 	//Initialize USMART		
	delay_ms(1000);			//Waiting for the bluetooth module become stable 
	
	while(HC05_Init()) 		//Initialize ATK-HC05 module  
	{
		u3_printf("error\r\n");
	}
	USART3_RX_STA=0;
	HC05_Set_Cmd("AT+ROLE=0");
	HC05_Set_Cmd("AT+RESET"); 
 	while(1) 
	{			  
		if(USART3_RX_STA&0X8000)			//Have already received one group of data
		{
				
 			reclen=USART3_RX_STA&0X7FFF;	// Get the length of data 
		  	USART3_RX_BUF[reclen]=0;	 	// Adding end character
			
			//Speed setting
			if(reclen==1)
			{
				if(strcmp((const char*)USART3_RX_BUF,"0")==0){time[0]=90;time[1]=9;}
				else if(strcmp((const char*)USART3_RX_BUF,"1")==0){time[0]=80;time[1]=19;}
				else if(strcmp((const char*)USART3_RX_BUF,"2")==0){time[0]=70;time[1]=29;}
				else if(strcmp((const char*)USART3_RX_BUF,"3")==0){time[0]=60;time[1]=39;}
				else if(strcmp((const char*)USART3_RX_BUF,"4")==0){time[0]=50;time[1]=49;}
				else if(strcmp((const char*)USART3_RX_BUF,"5")==0){time[0]=40;time[1]=59;}
				else if(strcmp((const char*)USART3_RX_BUF,"6")==0){time[0]=30;time[1]=69;}
				else if(strcmp((const char*)USART3_RX_BUF,"7")==0){time[0]=20;time[1]=79;}
				else if(strcmp((const char*)USART3_RX_BUF,"8")==0){time[0]=10;time[1]=89;}
				else if(strcmp((const char*)USART3_RX_BUF,"9")==0){time[0]=1;time[1]=98;}
				u3_printf("speed=%s\r\n",USART3_RX_BUF);
			}
			
			// Set direction
			 else if(reclen==4||reclen==5)
			{
				if(strcmp((const char*)USART3_RX_BUF,"none")==0)
					
				{
					PCout(0)=0;PCout(1)=0;PCout(2)=0;PCout(3)=0;PCout(4)=0;PCout(5)=0;PCout(6)=0;PCout(7)=0;
				}
					
				else if(strcmp((const char*)USART3_RX_BUF,"head")==0)
					
				{
					PCout(0)=0;PCout(1)=1;PCout(2)=0;PCout(3)=1;PCout(4)=0;PCout(5)=1;PCout(6)=0;PCout(7)=1;
				}
				else if(strcmp((const char*)USART3_RX_BUF,"back")==0)car_dir_mood=2;
				else if(strcmp((const char*)USART3_RX_BUF,"left")==0)car_dir_mood=3;
				else if(strcmp((const char*)USART3_RX_BUF,"right")==0)car_dir_mood=4;
				else if(strcmp((const char*)USART3_RX_BUF,"clock")==0)car_dir_mood=5;
				else if(strcmp((const char*)USART3_RX_BUF,"anti")==0)car_dir_mood=6;
				else if(strcmp((const char*)USART3_RX_BUF,"NtoW")==0)car_dir_mood=7;
				else if(strcmp((const char*)USART3_RX_BUF,"NtoE")==0)car_dir_mood=8;
				else if(strcmp((const char*)USART3_RX_BUF,"StoW")==0)car_dir_mood=9;
				else if(strcmp((const char*)USART3_RX_BUF,"StoE")==0)car_dir_mood=10;
				else if(strcmp((const char*)USART3_RX_BUF,"Stop")==0)car_dir_mood=11;
//				wheel_control(car_dir_mood);
			}
 			
 			USART3_RX_STA=0;	 
		}	
	}
}
