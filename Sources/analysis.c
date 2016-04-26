/*
 * analysis.c
 *
 *  Created on: Mar 15, 2016
 *      Author: 孟硕
 */

#include "includes.h"

int A[128]={0};
int B[128]={0};
int al_end=10,ar_end=0,bl_end=0,br_end=120;            //左右CCD向左右扫描的终点
int a_start=75,b_start=55;                          //左右CCD扫描起始点
int a_mid=0,b_mid=0;                                //左右CCD测到的赛道中间值
int a_value=10,b_value=10;                          //判断跳变沿的差值标准
int a_T=375,b_T=375;                                  //黑白阈值
int al_count=0,ar_count=0,bl_count=0,br_count=0;  //白点计数
int a_PixelNumber=65,b_PixelNumber=65;
int a_allwhite=62,a_allblack=2,b_allwhite=62,b_allblack=2;                  //全白,全黑判断标准
int a_scan=5,a_halfscan=3,b_scan=5,b_halfscan=3;
int al_flag=4,ar_flag=4,bl_flag=4,br_flag=4,allflag=4444;//0,1,2,3,4;黑，白，白-黑，黑-白，错误
int wrong_flag=0;
int al_edge=0,ar_edge=0,bl_edge=0,br_edge=0;//跳变沿
int error=0;
int i=0,j=0;

void DataSet(void)
{
	int k;
	int as[10],ascnt=0,bs[10],bscnt=0;
	int av[10],avcnt=0,bv[10],bvcnt=0;
	int at[10],atcnt=0,bt[10],btcnt=0;
	for(k=0;k<10;k++)
	{
		mdelay(10);
		ImageCapture(PixelLeft,PixelRight); 
		av[k]=(PixelLeft[70]+PixelLeft[71]-PixelLeft[20]-PixelLeft[21]/5);
		avcnt+=av[k];
		bv[k]=(PixelRight[70]+PixelRight[110]-PixelRight[20]-PixelRight[111])/5;
		bvcnt+=bv[k];
		at[k]=(PixelLeft[70]+PixelLeft[71]+PixelLeft[20]+PixelLeft[21]/5);
		atcnt+=at[k];
		bt[k]=(PixelRight[70]+PixelRight[71]+PixelRight[110]+PixelRight[111])/5;
		btcnt+=bt[k];
		for(i=70;i>0;i--)   //
		{
			if(PixelLeft[i]-PixelLeft[i-a_scan]>av[k]&&PixelLeft[i-1]-PixelLeft[i-a_scan-1]>av[k])
			{
				al_edge=i-a_halfscan;  //左CCD左边跳变沿
				break;
			}
//			if(PixelRight[i]-PixelRight[i-b_scan]>bv[k]&&PixelRight[i-1]-PixelRight[i-b_scan-1]>bv[k])
//			{
// 			    bl_edge=i-b_halfscan;  //右CCD左边跳变沿
//				break;
//			}
		}
		for(i=70;i<127;i++) 
		{
//			if(PixelLeft[i]-PixelLeft[i+a_scan]>av[k]&&PixelLeft[i+1]-PixelLeft[i+a_scan+1]>av[k])
//			{
//				ar_edge=i+a_halfscan;  //左CCD右边跳变沿
//				break;
//			}
			if(PixelRight[i]-PixelRight[i+b_scan]>bv[k]&&PixelRight[i+1]-PixelRight[i+b_scan+1]>bv[k])
			{
				br_edge=i+b_halfscan;  //右CCD右边跳变沿
				break;
			}
		}
		as[k]=(al_edge+ar_edge)/2;
		bs[k]=(bl_edge+br_edge)/2;
		ascnt+=as[k];
		bscnt+=bs[k];
	}
	a_value=avcnt/10;     //左CCD判断条边沿的差值标准
	b_value=bvcnt/10;     //右CCD判断跳变沿的差值标准
	//b_value=bv[0];
	a_mid=ascnt/10;
	b_mid=bscnt/10;       //赛道中间值
	al_end=a_start-44;    //左CCD图像左极限
	ar_end=a_start+44;    //左CCD图像右极限
	bl_end=b_start-44;    //右CCD图像左极限
	br_end=b_start+44;    //右CCD图像右极限
	a_T=atcnt/10;
	b_T=btcnt/10;         //黑白阈值
	//b_T=bt[0];
}

void PixelScan(void)
{
	al_count=0,ar_count=0,bl_count=0,br_count=0;
	al_flag=4,ar_flag=4,bl_flag=4,br_flag=4,allflag=4444;
	al_edge=0,ar_edge=0,bl_edge=0,br_edge=0;
	error=0;
	wrong_flag=0;
	ImageCopy(A,PixelLeft);
	ImageCopy(B,PixelRight);
	for(i=b_start;i<br_end;i++) //_start待确定 ？？？？？？？？？
	{
		if(B[i]>b_T) //右灯向右扫描跳变沿
			br_count++;
		if((B[i]-B[i+b_scan])/b_scan>b_value&&(B[i-1]-B[i+b_scan-1])/b_scan>b_value)  //改为用斜率判断
		{
			br_flag=2; //白到黑
			br_edge=i+b_halfscan; //改为加号
			break;
		}
		if((B[i+b_scan]-B[i])/b_scan>b_value&&(B[i+b_scan-1]-B[i-1])/b_scan>b_value)
		{
			br_flag=3; //黑到白
			br_edge=i+b_halfscan; //改为加号
			break;
		}
		if(br_count>b_allwhite)
			br_flag=1;
		else if(br_count<b_allblack)
			br_flag=0;
		else
			br_flag=4;
		
	    }
	
    for(j=a_start;j>al_end;j--)  //尝试两摄像头各扫一侧 r与l反
	{
	    if(A[j]>a_T) //左灯向左扫描跳变沿
		    al_count++;
	    if((A[j]-A[j-a_scan])/a_scan>a_value&&(A[j-1]-A[j-a_scan-1])/a_scan>a_value)
	    {
		    al_flag=2; //白-黑
		    al_edge=j-a_halfscan;
		    break;
	    }
	    if((A[j-a_scan]-A[j])/a_scan>a_value&&(A[j-a_scan-1]-A[j-1])/a_scan>a_value)
	    {
		    al_flag=3;
		    al_edge=j-a_halfscan; 
		    break;
	    }
	    if(al_count>a_allwhite)
	        al_flag=1;
	    else if(al_count<a_allblack)
		    al_flag=0;
	    else
		    al_flag=4;
	}
}

void ErrorCalculate(void)
{
	if(al_flag==2&&br_flag==2)                              //22直道
	{
		error=(al_edge-a_start+br_edge-b_start)*0.8;   //a_start a_end b_start b_end可换为其他参数
		return;
	}
	if(al_flag==1&&br_flag==1)                              //11十字
	{
		wrong_flag=3;
		return;
	}
	if(al_flag==1&&br_flag==2)                              //12左转小
	{
		error=(br_edge-br_end)*1.3;
		//加入一个延时函数
		return;
	}
	if(al_flag==1&&br_flag==0)                              //10左转中
	{
		error=b_start-br_end;
		return;
	}
	if(al_flag==3&&br_flag==0)                              //30左转大
	{
		error=al_edge-br_end;
		return;	
	}
	if(al_flag==2&&br_flag==1)                              //21右转小
	{
		error=(al_edge-al_end)*1.3;
		return;
	}
	if(al_flag==0&&br_flag==1)                              //01右转中
	{
		error=a_start-al_end;
		return;
	}
	if(al_flag==0&&br_flag==3)                              //03右转大
	{
		error=br_edge-al_end;
		return;
	}
	if(al_flag==0&&br_flag==0)                              
	{
		wrong_flag=2;
	}
	if(al_flag==4||br_flag==4)
	{
		wrong_flag=1;
	}
	wrong_flag=1;
}

//ImageCopy(AA,PixelLeft);
//ImageCopy(BB,PixelRight);

