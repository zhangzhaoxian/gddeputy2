/*
 * capture.c
 *
 *  Created on: Mar 14, 2016
 *      Author: 刘裕
 */

#include "includes.h"

int PixelLeft[128]={0};
int PixelRight[128]={0};
unsigned int speedcounter1=0,speedcounter2=0,speedcounter3=0,speedcounter4=0;

/*************************CCD采集函数***********************/

void ImageCapture(int L[],int R[]) 
{
  //initAD();
  int Result0, Result1;
  int i;
  CCDL_SI=1;       // SI = 1	
  CCDR_SI=1;
  SamplingDelay();     //600ns
  CCDL_CLK=1;      //CLK =1;
  CCDR_CLK=1;
  SamplingDelay();
  CCDL_SI=0;       // SI=0;
  CCDR_SI=0;
  udelay(1);           //延时1μs
  for(i=0;i<20;i++)
  {
     Cpu_Delay1us();
  }
  
  while (ADC.CDR[34].B.VALID != 1) {}; /* 等待最后一次扫描完成*/
  Result1= ADC.CDR[32].B.CDATA; /* 读取ANS0的转换结果数据*/
  Result0= ADC.CDR[34].B.CDATA; /* 读取ANS2的转换结果数据*/

  L[0]=Result0;
  R[0]=Result1;
  CCDL_CLK=0;   //CLK=0
  CCDR_CLK=0;
  
  for(i=0; i<127; i++) 
  {
     udelay(2);           //延时2μs
     udelay(2);
     CCDL_CLK = 1;    // CLK = 1 
     CCDR_CLK = 1;
     udelay(2);
     udelay(2);
        
     //Sampling Pixel 2~128
     while (ADC.CDR[34].B.VALID != 1) {}; /* 等待最后一次扫描完成*/
     Result1= ADC.CDR[32].B.CDATA; /* 读取ANS0的转换结果数据*/
     Result0= ADC.CDR[34].B.CDATA; /* 读取ANS2的转换结果数据*/
     L[i+1]=Result0;
     R[i+1]=Result1;
     CCDL_CLK = 0;    // CLK = 0 
     CCDR_CLK = 0;
  }
  udelay(2);
  udelay(2);
  CCDL_CLK = 1;        // CLK = 1
  CCDR_CLK = 1; 
  udelay(2);
  udelay(2);
  CCDL_CLK = 0;        // CLK = 0
  CCDR_CLK = 0;        // CLK = 0
  udelay(2);
  udelay(2); 
}

/*************************图像复制函数***********************/
void ImageCopy(int L[],int PL[])
{
	int i;
	for(i=0;i<128;i++)
	{
		L[i]=PL[i];
	}
}

/*************************光编计数函数***********************/
void SpeedCount(void)
{
	speedcounter1=EMIOS_0.CH[24].CCNTR.R;              //左D12
	if(speedcounter1<speedcounter2)
	{
		csl=speedcounter1+65536-speedcounter2;         //current speed left
	}
	else 
		csl=speedcounter1-speedcounter2;
//	if(forewardleft)
//		csl=csl;
//	else 
//		csl=-csl;
	speedcounter2=speedcounter1;
	
	speedcounter3=EMIOS_0.CH[8].CCNTR.R;               //右A8
	if(speedcounter3<speedcounter4)
	{
		csr=speedcounter3+65536-speedcounter4;         //current speed right
	}
	else 
		csr=speedcounter3-speedcounter4;	
//	if(backwardright) 
//		csr=-csr;
//	else 
//		csr=csr;
	speedcounter4=speedcounter3;
}


