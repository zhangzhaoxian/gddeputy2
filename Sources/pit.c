/*
 * pit.c
 *
 *  Created on: Mar 14, 2016
 *      Author: 刘裕
 */
#include "includes.h"

unsigned int IOtest=0;
unsigned int pitcount0=0,pitcount1=0,pitcount2=0,pitcount3=0,pitcount4=0,pitcount5=0;

void initPIT(void) 
{                                   //1ms一个控制周期// NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 
  PIT.PITMCR.R = 0x00000001;       // Enable PIT and configure timers to stop in debug mode 
  PIT.CH[1].LDVAL.R = 80000;      // PIT1 timeout = 80000 sysclks x 1sec/80M sysclks = 1 msec 
  PIT.CH[1].TCTRL.R = 0x000000003; // Enable PIT1 interrupt and make PIT active to count 
  INTC_InstallINTCInterruptHandler(PitISR,60,4); 
}

void PitISR(void)//1ms一个控制周期
{
	pitcount0++;                                  //5+2+0.035ms一次清零
	if(pitcount0==2)
	{
		pitcount2++;
		if(pitcount2>=2)                          //5ms一次,需2ms,共12ms一次
		{
			//time1=TIME;
			pitcount2=0;
		    ImageCapture(PixelLeft,PixelRight);
			PixelScan();                          //扫描跳变沿
			ErrorCalculate();                     //计算偏差值
//			Steer_PDSet();
//			SteerControl();
			//time2=TIME;
			//time3=TimeMesure();
		}
	}
	if(pitcount0==3)
	{
		pitcount3++;
		if(pitcount3>=3)                          //15ms一次,35us
		{
			//time1=TIME;
			pitcount3=0;
//			SpeedCount();
			//Speed_control(); 
			//time2=TIME;
			//time3=TimeMesure();
		}
	}
	if(pitcount0==5)
	{
		pitcount0=0;
		pitcount5++;
		if(pitcount5>=100)                         //1s一次
		{
			pitcount5=0;
			OLED_SetPointer(2,20);
			OLED_Str("ts: ");
			OLED_Num(CurrentSteer);
		}
	}
	PIT.CH[1].TFLG.B.TIF = 1;//write 1 to clear PIT1 清除标志位
}

