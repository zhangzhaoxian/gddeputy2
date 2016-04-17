/*
 * debug.c
 *
 *  Created on: Mar 15, 2016
 *      Author: ��˶
 */

#include "includes.h"

unsigned char *send;
unsigned char putstring[]="Image";
int Left[128];
int Right[128];
unsigned int Ts=0;
unsigned int Tc=0;
int CurrentSteer=0;

unsigned char S3_last=1;
unsigned char S4_last=1;
unsigned char S5_last=1;
unsigned char S6_last=1;
unsigned char keymode=0;


//********************************************************************************************************
//****************************************���뿪�غ���*****************************************************
//********************************************************************************************************
//void ChooseMode(void)
//{
//	mode=15-(switch1*8+switch2*4+switch3*2+switch4);
//}
//********************************************************************************************************
//****************************************�������ͺ���*****************************************************
//********************************************************************************************************

/*************************�������ͺ���*********************/
void BlueTx(void)                             //����������
{
	send = putstring;
	LINFlex_TX(*send++);
}
/*************************�����ӿں���*********************/
void LINFlex_TX(unsigned char data)
{
	LINFLEX_0.UARTSR.B.DTF=1;
	LINFLEX_0.BDRL.B.DATA0=data;       //�������
}
/*************************�����жϺ���*********************/
void LINFlex_TX_Interrupt(void)
{
	unsigned char aa='L';
	unsigned char bb='R';
	unsigned char steer='X';
	unsigned char speed='Y';
	unsigned char data='Z';
//		if(*send!=0x00&&Ts==0)
//			LINFlex_TX(*send++);
//		else
//		{
//			send = putstring;
//		}
	switch(Ts){
	case 0: if(*send!=0x00){
				LINFlex_TX(*send++);
				break;}
			else{
				Ts=1;
				break;}
	case 1:
		LINFlex_TX(aa);
		ImageCopy(Left,PixelLeft);
		Ts=2;
		break;
	case 2: 
		LINFlex_TX(SendInt2(Left[Tc]));        //������CCDͼ��
		Ts=3;
		break;
	case 3: 
		LINFlex_TX(SendInt3(Left[Tc]));
		Ts=4;
		break;
	case 4: 
		LINFlex_TX(SendInt4(Left[Tc]));
		if(Tc<127){
			Tc++;
			Ts=2;}
		else{
			Tc=0;
			Ts=5;}
		break;
	case 5: 
		LINFlex_TX(bb);
		ImageCopy(Right,PixelRight);
		Ts=6;
		break;
	case 6: 
		LINFlex_TX(SendInt2(Right[Tc]));        //������CCDͼ��
		Ts=7;
		break;
	case 7: 
		LINFlex_TX(SendInt3(Right[Tc]));
		Ts=8;
		break;
	case 8:
		LINFlex_TX(SendInt4(Right[Tc]));
		if(Tc<127){
			Tc++;
			Ts=6;}
		else{
			Tc=0;
			Ts=9;
		}
		break;
	case 9: 
		CurrentSteer=TargetSteer;
		LINFlex_TX(steer);
		Ts=10;
		break;
	case 10: 
		LINFlex_TX(SendInt1(CurrentSteer));        //���Ͷ��ֵ
		Ts=11;
		break;
	case 11: 
		LINFlex_TX(SendInt2(CurrentSteer));
		Ts=12;
		break;
	case 12: 
		LINFlex_TX(SendInt3(CurrentSteer));
		Ts=13;
		break;
	case 13:
		LINFlex_TX(SendInt4(CurrentSteer));
		Ts=23;
		break;
	case 14: 
		LINFlex_TX(speed);
		Ts=15;
		break;
	case 15: 
		//LINFlex_TX(SendInt1(csl));
		//LINFlex_TX(SendInt2(b_value));        //����B�����ر�׼
		LINFlex_TX(SendInt1(tsl));
		Ts=16;
		break;
	case 16: 
		//LINFlex_TX(SendInt2(csl));
		//LINFlex_TX(SendInt3(b_value));
		LINFlex_TX(SendInt2(tsl));
		Ts=17;
		break;
	case 17: 
		//LINFlex_TX(SendInt3(csl));
		//LINFlex_TX(SendInt4(b_value));
		LINFlex_TX(SendInt3(tsl));
		Ts=18;
		break;
	case 18:
		//LINFlex_TX(SendInt4(csl));
		//LINFlex_TX(SendInt3(b_start));        //����B��ʼλ
		LINFlex_TX(SendInt4(tsl));
		Ts=19;
		break;
	case 19: 
		//LINFlex_TX(SendInt1(csr));
		//LINFlex_TX(SendInt4(b_start));
		LINFlex_TX(SendInt1(tsr));
		Ts=20;
		break;
	case 20: 
		//LINFlex_TX(SendInt2(csr));
		//LINFlex_TX(SendInt2(b_T));           //����B��ֵ
		LINFlex_TX(SendInt2(tsr));
		Ts=21;
		break;
	case 21: 
		//LINFlex_TX(SendInt3(csr));
		//LINFlex_TX(SendInt3(b_T));
		LINFlex_TX(SendInt3(tsr));
		Ts=22;
		break;
	case 22:
		//LINFlex_TX(SendInt4(csr));
		//LINFlex_TX(SendInt4(b_T));
		LINFlex_TX(SendInt4(tsr));
		Ts=23;
		break;
	case 23: 
		LINFlex_TX(data);
		Ts=24;
		break;
	case 24: 
		LINFlex_TX(SendInt4(al_flag));        //����B_flag
		Ts=25;
		break;
	case 25: 
		LINFlex_TX(SendInt4(br_flag));
		Ts=26;
		break;
	case 26: 
		LINFlex_TX(SendInt1(error));        //����error
		Ts=27;
		break;
	case 27:
		LINFlex_TX(SendInt2(error));
		Ts=28;
		break;
	case 28: 
		LINFlex_TX(SendInt3(error));        
		Ts=29;
		break;
	case 29:
		LINFlex_TX(SendInt4(error));
		Ts=30;
		break;
	case 30:
		send = putstring;
		Ts=0;
//		LINFLEX_0.LINCR1.B.INIT=1;
//		LINFLEX_0.LINIER.B.DRIE=0;
//		LINFLEX_0.LINCR1.B.INIT=0; //�˳���ʼ��ģʽ
		break;	
	}
}

//********************************************************************************************************
//****************************************�������Ժ���*****************************************************
//********************************************************************************************************

void KeyJudge(void)
{
	if(S3==0&&S3_last==1){   //����S3����
		keymode=1;
		TargetSteer+=100;
		//tsr-=10;
		}
	if(S4==0&&S4_last==1){   //����S4����
	    keymode=2;
	    TargetSteer-=100;
	    //tsr+=10;
	    }
	if(S5==0&&S5_last==1){   //����S5����
		keymode=3;
		TargetSteer+=10;
		//tsl-=10;
		}
	if(S6==0&&S6_last==1){   //����S6����
		keymode=4; 
		TargetSteer-=10;
		//tsl+=10;
		}
	S3_last=S3;             //���水����״̬
	S4_last=S4;
	S5_last=S5;
	S6_last=S6;
}
