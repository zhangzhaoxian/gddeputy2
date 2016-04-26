/*
 * analysis.c
 *
 *  Created on: Mar 15, 2016
 *      Author: ��˶
 */

#include "includes.h"

int A[128]={0};
int B[128]={0};
int al_end=10,ar_end=0,bl_end=0,br_end=120;            //����CCD������ɨ����յ�
int a_start=75,b_start=55;                          //����CCDɨ����ʼ��
int a_mid=0,b_mid=0;                                //����CCD�⵽�������м�ֵ
int a_value=10,b_value=10;                          //�ж������صĲ�ֵ��׼
int a_T=375,b_T=375;                                  //�ڰ���ֵ
int al_count=0,ar_count=0,bl_count=0,br_count=0;  //�׵����
int a_PixelNumber=65,b_PixelNumber=65;
int a_allwhite=62,a_allblack=2,b_allwhite=62,b_allblack=2;                  //ȫ��,ȫ���жϱ�׼
int a_scan=5,a_halfscan=3,b_scan=5,b_halfscan=3;
int al_flag=4,ar_flag=4,bl_flag=4,br_flag=4,allflag=4444;//0,1,2,3,4;�ڣ��ף���-�ڣ���-�ף�����
int wrong_flag=0;
int al_edge=0,ar_edge=0,bl_edge=0,br_edge=0;//������
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
				al_edge=i-a_halfscan;  //��CCD���������
				break;
			}
//			if(PixelRight[i]-PixelRight[i-b_scan]>bv[k]&&PixelRight[i-1]-PixelRight[i-b_scan-1]>bv[k])
//			{
// 			    bl_edge=i-b_halfscan;  //��CCD���������
//				break;
//			}
		}
		for(i=70;i<127;i++) 
		{
//			if(PixelLeft[i]-PixelLeft[i+a_scan]>av[k]&&PixelLeft[i+1]-PixelLeft[i+a_scan+1]>av[k])
//			{
//				ar_edge=i+a_halfscan;  //��CCD�ұ�������
//				break;
//			}
			if(PixelRight[i]-PixelRight[i+b_scan]>bv[k]&&PixelRight[i+1]-PixelRight[i+b_scan+1]>bv[k])
			{
				br_edge=i+b_halfscan;  //��CCD�ұ�������
				break;
			}
		}
		as[k]=(al_edge+ar_edge)/2;
		bs[k]=(bl_edge+br_edge)/2;
		ascnt+=as[k];
		bscnt+=bs[k];
	}
	a_value=avcnt/10;     //��CCD�ж������صĲ�ֵ��׼
	b_value=bvcnt/10;     //��CCD�ж������صĲ�ֵ��׼
	//b_value=bv[0];
	a_mid=ascnt/10;
	b_mid=bscnt/10;       //�����м�ֵ
	al_end=a_start-44;    //��CCDͼ������
	ar_end=a_start+44;    //��CCDͼ���Ҽ���
	bl_end=b_start-44;    //��CCDͼ������
	br_end=b_start+44;    //��CCDͼ���Ҽ���
	a_T=atcnt/10;
	b_T=btcnt/10;         //�ڰ���ֵ
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
	for(i=b_start;i<br_end;i++) //_start��ȷ�� ������������������
	{
		if(B[i]>b_T) //�ҵ�����ɨ��������
			br_count++;
		if((B[i]-B[i+b_scan])/b_scan>b_value&&(B[i-1]-B[i+b_scan-1])/b_scan>b_value)  //��Ϊ��б���ж�
		{
			br_flag=2; //�׵���
			br_edge=i+b_halfscan; //��Ϊ�Ӻ�
			break;
		}
		if((B[i+b_scan]-B[i])/b_scan>b_value&&(B[i+b_scan-1]-B[i-1])/b_scan>b_value)
		{
			br_flag=3; //�ڵ���
			br_edge=i+b_halfscan; //��Ϊ�Ӻ�
			break;
		}
		if(br_count>b_allwhite)
			br_flag=1;
		else if(br_count<b_allblack)
			br_flag=0;
		else
			br_flag=4;
		
	    }
	
    for(j=a_start;j>al_end;j--)  //����������ͷ��ɨһ�� r��l��
	{
	    if(A[j]>a_T) //�������ɨ��������
		    al_count++;
	    if((A[j]-A[j-a_scan])/a_scan>a_value&&(A[j-1]-A[j-a_scan-1])/a_scan>a_value)
	    {
		    al_flag=2; //��-��
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
	if(al_flag==2&&br_flag==2)                              //22ֱ��
	{
		error=(al_edge-a_start+br_edge-b_start)*0.8;   //a_start a_end b_start b_end�ɻ�Ϊ��������
		return;
	}
	if(al_flag==1&&br_flag==1)                              //11ʮ��
	{
		wrong_flag=3;
		return;
	}
	if(al_flag==1&&br_flag==2)                              //12��תС
	{
		error=(br_edge-br_end)*1.3;
		//����һ����ʱ����
		return;
	}
	if(al_flag==1&&br_flag==0)                              //10��ת��
	{
		error=b_start-br_end;
		return;
	}
	if(al_flag==3&&br_flag==0)                              //30��ת��
	{
		error=al_edge-br_end;
		return;	
	}
	if(al_flag==2&&br_flag==1)                              //21��תС
	{
		error=(al_edge-al_end)*1.3;
		return;
	}
	if(al_flag==0&&br_flag==1)                              //01��ת��
	{
		error=a_start-al_end;
		return;
	}
	if(al_flag==0&&br_flag==3)                              //03��ת��
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

