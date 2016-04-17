/*
 * init.c
 *
 *  Created on: Mar 12, 2016
 *      Author: p
 */
#include "includes.h"

void initALL(void)
{
	disableWatchdog(); //关闭看门狗
	initModesAndClock();
	enableIrq();
	initEMIOS_0MotorAndSteer();
	initEMIOS_0ModulusCounter();
//	initEMIOS_0Image();
	initLINFlex_0_UART();
	initAD();
//	initKeys_Switchs_Infrared();
	initTestIO();
	OLED_Init();
}

/*********************************************************************************************/
/*********************************  关闭看门狗 **********************************************/
/*********************************************************************************************/
void disableWatchdog(void)
{
	SWT.SR.R = 0x0000c520; /* Write keys to clear soft lock bit */
	SWT.SR.R = 0x0000d928;
	SWT.CR.R = 0x8000010A; /* Clear watchdog enable (WEN) */
}

//*****************************************************************************************************************
//*			 *************************时钟初始化 *******************************************************    	  *
//*****************************************************************************************************************
void initModesAndClock(void) 
{
	ME.MER.R = 0x0000001D;          /* Enable DRUN, RUN0, SAFE, RESET modes */
	//Initialize PLL before turning it on                    
	//设置sysclk
	//CGM.FMPLL_CR.R = 0x02400100;    /* 8 MHz xtal: Set PLL0 to 64 MHz */  
	CGM.FMPLL_CR.R = 0x01280000;      /* 8 MHz xtal: Set PLL0 to 80 MHz */
	//CGM.FMPLL_CR.R = 0x013C0000;    /* 8 MHz xtal: Set PLL0 to 120 MHz */   
	ME.RUN[0].R = 0x001F0064;       /* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL     sysclk选择锁相环时钟*/
	ME.RUNPC[0].R = 0x00000010;    /* Peri. Cfg. 1 settings: only run in RUN0 mode      选择RUN0模式*/
	/*************************PCTL[?] 选择需要时钟模块****************************/
	ME.PCTL[32].R = 0x00; 			/* MPC56xxB/P/S ADC 0: select ME.RUNPC[0] */
	ME.PCTL[48].R = 0x00;           /* MPC56xxB/P/S LINFlex 0: select ME.RUNPC[0] */
	ME.PCTL[68].R = 0x00;           /* MPC56xxB/S SIUL:  select ME.RUNPC[0] */ 
	ME.PCTL[72].R = 0x00;           /* MPC56xxB/S EMIOS 0:  select ME.RUNPC[0] */ 
	//Mode Transition to enter RUN0 mode      
	ME.MCTL.R = 0x40005AF0;         /* Enter RUN0 Mode & Key */
	ME.MCTL.R = 0x4000A50F;         /* Enter RUN0 Mode & Inverted Key */ 
	    
	while (ME.GS.B.S_MTRANS) {}     // Wait for mode transition to complete 等待模式转换完成    
	/********************************** Note: could wait here using timer and/or I_TC IRQ*/                          
	while(ME.GS.B.S_CURRENTMODE != 4) {} // Verify RUN0 is the current mode 等待选择RUN0模式
	//开peri0、1、2
	CGM.SC_DC[0].R = 0x80;//LIN
	CGM.SC_DC[1].R = 0x80;//FLEXCAN,DSPI
	CGM.SC_DC[2].R = 0x80;//eMIOS,CTU,ADC
}

//*****************************************************************************************************************
//*************************eMIOS初始化:电机正反转E3、E4、E5、E6；舵机输出A4  **********************************************
//*****************************************************************************************************************
/*************************PWM初始化***********************/
void initEMIOS_0MotorAndSteer(void)
{
  //eMIOS0初始化80MHz分为1MHz
	EMIOS_0.MCR.B.GPRE= 31;   //GPRE+1=分频系数；/* Divide 80 MHz sysclk by 31+1 = 32 for 2.5MHz(0.4us) eMIOS clk*/
	EMIOS_0.MCR.B.GPREN = 1;	/* Enable eMIOS clock */
	EMIOS_0.MCR.B.GTBE = 1;   /* Enable global time base */
	EMIOS_0.MCR.B.FRZ = 1;    /* Enable stopping channels when in debug mode */	
  /**********电机PWM 5kHZ E3、E4、E5、E6*********************************************************************************/ 
	//eMIOS0D通道16设置/* EMIOS 0 CH 16: Modulus Up Counter */
    EMIOS_0.CH[16].CCR.B.UCPRE=0;	    /* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[16].CCR.B.UCPEN = 1;   /* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[16].CCR.B.FREN = 1; 	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[16].CADR.R = 500;	/********设置周期200us  5kHZ********/
	EMIOS_0.CH[16].CCR.B.MODE = 0x50; /* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[16].CCR.B.BSL = 0x3;	/* Use internal counter */
	/*E3(EMIOS 0 CH 19)左前进输出: Output Pulse Width Modulation */
	EMIOS_0.CH[19].CCR.B.BSL = 0x1;//use counter bus D
	EMIOS_0.CH[19].CCR.B.MODE = 0x60;//Mode is OPWM Buffered
	EMIOS_0.CH[19].CCR.B.EDPOL = 1;//Polarity-leading edge sets output/trailing clears
	EMIOS_0.CH[19].CADR.R = 0;//Leading edge when channel counter bus=250
	EMIOS_0.CH[19].CBDR.R = 0;//Trailing edge when channel counter bus=500
	SIU.PCR[67].R = 0x0600;//Assign EMIOS_0 CH19 to Pad
	/*E4(EMIOS 0 CH 20)左后退输出: Output Pulse Width Modulation */
	EMIOS_0.CH[20].CCR.B.BSL = 0x1;//use counter bus D
	EMIOS_0.CH[20].CCR.B.MODE = 0x60;//Mode is OPWM Buffered
	EMIOS_0.CH[20].CCR.B.EDPOL = 1;//Polarity-leading edge sets output/trailing clears
	EMIOS_0.CH[20].CADR.R = 0;//Leading edge when channel counter bus=250
	EMIOS_0.CH[20].CBDR.R = 0;//Trailing edge when channel counter bus=500
	SIU.PCR[68].R = 0x0600;//Assign EMIOS_0 CH20 to Pad
	 /* E5(EMIOS 0 CH 21)右前进输出: Output Pulse Width Modulation */
	EMIOS_0.CH[21].CCR.B.BSL = 0x1;	/* Use counter bus D (default) */
	EMIOS_0.CH[21].CCR.B.MODE = 0x60; /* Mode is OPWM Buffered */
    EMIOS_0.CH[21].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[21].CADR.R = 0;     /* Leading edge when channel counter bus=250*/
	EMIOS_0.CH[21].CBDR.R = 0;      /* Trailing edge when channel counter bus=500*/
	SIU.PCR[69].R = 0x0600;    //[11:10]选择AFx 此处AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
	/* E6（EMIOS 0 CH 22）右后退输出: Output Pulse Width Modulation */
	EMIOS_0.CH[22].CCR.B.BSL = 0x1;	/* Use counter bus D (default) */
	EMIOS_0.CH[22].CCR.B.MODE = 0x60; /* Mode is OPWM Buffered */
    EMIOS_0.CH[22].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[22].CADR.R = 0;     /* Leading edge when channel counter bus=250*/
	EMIOS_0.CH[22].CBDR.R = 0;     /* Trailing edge when channel counter bus=500*/
	SIU.PCR[70].R = 0x0600;   //[11:10]选择AFx 此处AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
	
	/**********舵机PWM 50HZ A4口输出50000*7.5%=3750中位**********/
	//eMIOS0 A通道0设置/* EMIOS 0 CH 0: Modulus Counter */
	EMIOS_0.CH[0].CCR.B.UCPRE=0;	    /* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[0].CCR.B.UCPEN = 1;   /* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[0].CCR.B.FREN = 1; 	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[0].CADR.R = 50000;/********设置周期20ms  50HZ*******/
	EMIOS_0.CH[0].CCR.B.MODE = 0x50; /* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[0].CCR.B.BSL = 0x3;	/* Use internal counter */
		
	/* EMIOS 0 CH 4: Output Pulse Width Modulation */
	EMIOS_0.CH[4].CCR.B.BSL = 1;	/* Use counter bus B (default) */
	EMIOS_0.CH[4].CCR.B.MODE = 0x60; /* Mode is OPWM Buffered */  
	EMIOS_0.CH[4].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[4].CADR.R = 0;//半占空比/* Leading edge when channel counter bus=250*/
	EMIOS_0.CH[4].CBDR.R = CENTER;            /* Trailing edge when channel counter bus=500*/
	SIU.PCR[4].R = 0x0600;    //[11:10]选择AFx 此处AF1   A4口舵机输出
}
/*************************光编初始化***********************/
void initEMIOS_0ModulusCounter(void) //D12,A8模数计数器入口，上升沿，D11,A6光编正反转
{
	//D12
	//EMIOS_0.CH[24].CCR.B.MODE = 0x51; // Mode is MCB, 
	EMIOS_0.CH[24].CCR.B.MODE = 0x13; // Mode is MCB, 
	EMIOS_0.CH[24].CCR.B.BSL = 0x3; // Use internal counter
	EMIOS_0.CH[24].CCR.B.UCPRE=0; // Set channel prescaler to divide by 1
	EMIOS_0.CH[24].CCR.B.UCPEN = 1; // Enable prescaler; uses default divide by 1 
	//EMIOS_0.CH[24].CCR.B.FREN = 0;// Freeze channel counting when in debug mode 
	EMIOS_0.CH[24].CCR.B.EDPOL=1; //Edge Select rising edge
	EMIOS_0.CH[24].CADR.R=0xffff;
	SIU.PCR[60].R = 0x0102;  // Initialize pad for eMIOS channel Initialize pad for input
	SIU.PCR[41].R = 0x0102;  //C9左轮光编正反转 ，高电平正转，低电平反转
	 
	 //A8 
	 //	EMIOS_0.CH[8].CCR.B.MODE = 0x51; // Mode is MCB, 
	EMIOS_0.CH[8].CCR.B.MODE = 0x13; // Mode is MCB, 
	EMIOS_0.CH[8].CCR.B.BSL = 0x3; // Use internal counter
	EMIOS_0.CH[8].CCR.B.UCPRE=0; // Set channel prescaler to divide by 1
	EMIOS_0.CH[8].CCR.B.UCPEN = 1; // Enable prescaler; uses default divide by 1 
	//EMIOS_0.CH[8].CCR.B.FREN = 0;// Freeze channel counting when in debug mode 
	EMIOS_0.CH[8].CCR.B.EDPOL=1; //Edge Select rising edge
	EMIOS_0.CH[8].CADR.R=0xffff;
	SIU.PCR[8].R = 0x0102;  // Initialize pad for eMIOS channel Initialize pad for input
	SIU.PCR[47].R=0x0102;  //C15右轮光编正反转，高电平反转，低电平正转
}

//*****************************************************************************************************************
//****************************************ADC初始化******************************************************    	  *
//*****************************************************************************************************************
void initAD(void)
{
  ADC.MCR.R = 0x20000000;       /*未读转换数据不能被覆盖；右对齐；连续转换序列模式；引起当前链转换结束并终止操作；对通道注入的外触发器使不能；模拟时钟频率为40MHz；CTU触发转换使不能；自动时钟关闭使不能；正常模式*/
  ADC.NCMR[1].R = 0x00000005;   /*使能CH32和CH34通道（标准通道）的位正常采样*/
  ADC.CTR[1].R = 0x00008606;    /*转换时间寄存器 与标准通道相关联*/
  ADC.MCR.B.NSTART=1;         /* Trigger normal conversions for ADC0 */
  
  SIU.PCR[26].R = 0x2100;//CCDR AO  B10
  SIU.PCR[11].R = 0x0200;//CCDR CLK A11
  SIU.PCR[10].R = 0x0200;//CCDR SI  A10
  SIU.PCR[24].R = 0x2100;//CCDL AO  B8
  SIU.PCR[6].R = 0x0200;//CCDL CLK  A6
  SIU.PCR[66].R = 0x0200;//CCDL SI  E2
}

//*****************************************************************************************************************
//****************************************中断初始化******************************************************    	  *
//*****************************************************************************************************************
void enableIrq(void) 
{
  INTC.CPR.B.PRI = 0;          /* Single Core: Lower INTC's current priority */
  asm(" wrteei 1");	    	   /* Enable external interrupts */
}

//*****************************************************************************************************************
//************************************************调试模块************************************************    	  *
//*****************************************************************************************************************
/*************************蓝牙初始化***********************/
void initLINFlex_0_UART(void) 
{
	LINFLEX_0.LINCR1.B.INIT=1;  //进入初始化模式
	LINFLEX_0.LINCR1.R=0x00000015; 
	LINFLEX_0.LINIER.B.DTIE=1; //允许发送中断
	/*波特率算法baud=Fperiph_clk/(16*LFDIV)
	DIV_M=LFDIV整数部分
	DIV_F=LFDIV小数部分*16  */
      //38400:64M-104+3
	LINFLEX_0.LINIBRR.B.DIV_M =86;  	//波特率设置38400:80M-130+3 57600:80M-86+13 115200:80M-43+6 
	LINFLEX_0.LINFBRR.B.DIV_F =13;	//38400:64M-104+3
    LINFLEX_0.UARTCR.B.UART=1;
	LINFLEX_0.UARTCR.R=0x00000033;//8-bit data UART mode
	LINFLEX_0.LINCR1.B.INIT=0; //退出初始化模式
	
	SIU.PCR[18].R = 0x0400;    /* MPC56xxB: Configure port B2 as LIN0TX */
    SIU.PCR[19].R = 0x0103;    /* MPC56xxB: Configure port B3 as LIN0RX */
  	INTC_InstallINTCInterruptHandler(LINFlex_TX_Interrupt,80,2); 
}

/*********************测试IO初始化***********************/
void initTestIO(void)
{
//	SIU.PCR[24].R = 0x0200;//CCDL AO  B8
//	SIU.PCR[27].R = 0x0200;//CCDL CLK B11
//	SIU.PCR[61].R = 0x0200;//CCDL SI  D13
//	SIU.PCR[26].R = 0x0200;//CCDR AO  B10
//	SIU.PCR[63].R = 0x0200;//CCDR CLK D15
//	SIU.PCR[62].R = 0x0200;//CCDR SI  D14
//	SIU.PCR[25].R = 0x0200;//CCDM AO  B9
//	SIU.PCR[46].R = 0x0200;//CCDM CLK C14
//	SIU.PCR[2].R = 0x0200; //CCDM SI  A2
//	SIU.PCR[59].R = 0x0200;//COUNTER1 D11
//	SIU.PCR[60].R = 0x0200;//COUNTER1 D12
//	SIU.PCR[6].R = 0x0200; //COUNTER2 A6
//	SIU.PCR[8].R = 0x0200; //COUNTER1 A8
	SIU.PCR[72].R = 0x0200;//OLED     E8
	SIU.PCR[74].R = 0x0200;//OLED     E10
	SIU.PCR[75].R = 0x0200;//OLED     E11
	SIU.PCR[42].R = 0x0200;//OLED     C10
	SIU.PCR[17].R = 0x0200;//OLED     B1
	SIU.PCR[73].R = 0x0200; //BEE      E9
//	SIU.PCR[9].R = 0x0200; //SUPER1   A9
//	SIU.PCR[5].R = 0x0200; //SUPER1   A5
//	SIU.PCR[66].R = 0x0200;//SUPER1   E2
	SIU.PCR[16].R = 0x0100;//SWITCH1  B0
	SIU.PCR[38].R = 0x0100;//SWITCH2  C6
	SIU.PCR[43].R = 0x0100;//SWITCH3  C11
	SIU.PCR[65].R = 0x0100;//SWITCH4  E1
	SIU.PCR[1].R = 0x0100;//SWITCH5   A1
	SIU.PCR[13].R = 0x0103;//KEY S6   A13
	SIU.PCR[14].R = 0x0103;//KEY S5   A14
	SIU.PCR[39].R = 0x0103;//KEY S4   C7
	SIU.PCR[15].R = 0x0103;//KEY S3   A15
//	
//	SIU.GPDO[24].R = 1;//CCDL AO  B8
//	SIU.GPDO[27].R = 0;//CCDL CLK B11
//	SIU.GPDO[61].R = 0;//CCDL SI  D13
//	SIU.GPDO[26].R = 1;//CCDR AO  B10
//	SIU.GPDO[63].R = 0;//CCDR CLK D15
//	SIU.GPDO[62].R = 0;//CCDR SI  D14
//	SIU.GPDO[25].R = 1;//CCDM AO  B9
//	SIU.GPDO[46].R = 0;//CCDM CLK C14
//	SIU.GPDO[2].R = 0; //CCDM SI  A2
//	SIU.GPDO[59].R = 1;//COUNTER1 D11
//	SIU.GPDO[60].R = 0;//COUNTER1 D12
//	SIU.GPDO[6].R = 0; //COUNTER2 A6
//	SIU.GPDO[8].R = 1; //COUNTER1 A8
//	SIU.GPDO[72].R = 0;//OLED     E8
//	SIU.GPDO[74].R = 0;//OLED     E10
//	SIU.GPDO[75].R = 0;//OLED     E11
//	SIU.GPDO[42].R = 0;//OLED     C10
//	SIU.GPDO[17].R = 1;//OLED     B1
	SIU.GPDO[0].R = 0; //BEE      A0
//	SIU.GPDO[9].R = 1; //SUPER1   A9
//	SIU.GPDO[5].R = 0; //SUPER1   A5
//	SIU.GPDO[66].R = 0;//SUPER1   E2
//	SIU.GPDO[18].R = 1;//UART     B2
//	SIU.GPDO[19].R = 0;//UART     B3
}


