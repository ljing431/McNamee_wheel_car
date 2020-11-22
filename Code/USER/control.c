#include "control.h"
#include "sys.h"
#include "usart3.h"
PID Vel_PID[4];
int Pulse_width[4] = {1999, 1999, 1999, 1999};
int Rpm_Vel[4] = {0,0,0,0};
u16 wheel_speed=0,car_dir_mood=0;

// Initialize 4 wheels' I/O ports
void init_wheel_control()
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;				 //LED0-->PB.5 port configuration
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //Push-pull output
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // The speed of I/O port is 50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 // Set GPIOB.5 using the set parameters
 GPIO_ResetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);						 //PB.5 Êä³ö¸ß

 
}

//Wheel direction control function
//PC0£¬PC1:left front wheel(in1,in2)
//PC2£¬PC3:right front wheel(in1,in2)
//PC4£¬PC5:left rear wheel(in1,in2)
//PC6£¬PC7:right rear wheel(in1,in2)
void wheel_control(u8 mood)
{
	u8 a=mood;
	switch(a)
	{
		case 0:PCout(0)=0;PCout(1)=0;PCout(2)=0;PCout(3)=0;PCout(4)=0;PCout(5)=0;PCout(6)=0;PCout(7)=0;break;//stop
		case 1:PCout(0)=0;PCout(1)=1;PCout(2)=0;PCout(3)=1;PCout(4)=0;PCout(5)=1;PCout(6)=0;PCout(7)=1;break;//forward
		case 2:PCout(0)=1;PCout(1)=0;PCout(2)=1;PCout(3)=0;PCout(4)=1;PCout(5)=0;PCout(6)=1;PCout(7)=0;break;//backward
		case 3:PCout(0)=1;PCout(1)=0;PCout(2)=0;PCout(3)=1;PCout(4)=0;PCout(5)=1;PCout(6)=1;PCout(7)=0;break;//left translation
		case 4:PCout(0)=0;PCout(1)=1;PCout(2)=1;PCout(3)=0;PCout(4)=1;PCout(5)=0;PCout(6)=0;PCout(7)=1;break;//right translation
		case 5:PCout(0)=0;PCout(1)=1;PCout(2)=1;PCout(3)=0;PCout(4)=0;PCout(5)=1;PCout(6)=1;PCout(7)=0;break;//clockwise
		case 6:PCout(0)=1;PCout(1)=0;PCout(2)=0;PCout(3)=1;PCout(4)=1;PCout(5)=0;PCout(6)=0;PCout(7)=1;break;//counterclockwise
		case 7:PCout(0)=0;PCout(1)=0;PCout(2)=0;PCout(3)=1;PCout(4)=0;PCout(5)=1;PCout(6)=0;PCout(7)=0;break;//left forward
		case 8:PCout(0)=0;PCout(1)=1;PCout(2)=0;PCout(3)=0;PCout(4)=0;PCout(5)=0;PCout(6)=0;PCout(7)=1;break;//right forward
		case 9:PCout(0)=1;PCout(1)=0;PCout(2)=0;PCout(3)=0;PCout(4)=0;PCout(5)=0;PCout(6)=1;PCout(7)=0;break;//left rare
		case 10:PCout(0)=0;PCout(1)=0;PCout(2)=1;PCout(3)=0;PCout(4)=1;PCout(5)=0;PCout(6)=0;PCout(7)=0;break;//right rare
		case 11:PCout(0)=1;PCout(1)=1;PCout(2)=1;PCout(3)=1;PCout(4)=1;PCout(5)=1;PCout(6)=1;PCout(7)=1;break;//brake
	}
	u3_printf("mood=%d",a);
}

// Initialize some data's input of PID, we haven't used this function yet.
void PID_Init(PID *PID,float kp,float ki,float kd,float max,float min,float Summax,float Summin)
{
	PID->Kp = kp;
	PID->Ki = ki;
	PID->Kd = kd;
	PID->Err = 0;
	PID->ErrSum = 0;
	PID->LastErr = 0;
	PID->Max = max;
	PID->Min = min ;
	PID->Summax = Summax;
	PID->Summin = Summin;
}
// The calculation of PID total deviation, the result will be restored in PID->Pid_out
void PID_Cal(PID *PID,float goal,float now_val)
{
	static float errsum = 0;
	float pout = 0;
	float iout = 0;
	float dout = 0;
	float pidout = 0;
	float delta_err = 0;
	
	PID->Err = (int)(goal - now_val);
	delta_err = PID->Err - PID->LastErr;
	errsum += PID->Err;
	PID->ErrSum = PID_LIMIT(errsum,PID->Summax,PID->Summin);
	PID->LastErr = PID->Err;
	
	pout = PID->Kp * PID->Err;
	iout = PID->Ki * PID->ErrSum;	
	dout = PID->Kd * delta_err;
	
	pidout = pout + iout + dout;
	PID->Pid_out = PID_LIMIT(pidout,PID->Max,PID->Min);
}
//convey the revised data (revised by PID) to trolley to correct it.
void Vel_PidCal(void)
{
	PID_Cal(&Vel_PID[0], (float)wheel_speed, Rpm_Vel[0]);
	PID_Cal(&Vel_PID[1], (float)wheel_speed, Rpm_Vel[1]);
	PID_Cal(&Vel_PID[2], (float)wheel_speed, Rpm_Vel[2]);
	PID_Cal(&Vel_PID[3], (float)wheel_speed, Rpm_Vel[3]);
	
	Pulse_width[0] += (int)Vel_PID[0].Pid_out;
	Pulse_width[1] += (int)Vel_PID[1].Pid_out;
	Pulse_width[2] += (int)Vel_PID[2].Pid_out;
	Pulse_width[3] += (int)Vel_PID[3].Pid_out;
}




