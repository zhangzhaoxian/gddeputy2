/*
 * init.h
 *
 *  Created on: Mar 12, 2016
 *      Author: p
 */

#ifndef INIT_H_
#define INIT_H_
void initALL(void);
void disableWatchdog(void);				//关看门狗
void initModesAndClock(void);			//时钟初始化80M
void initEMIOS_0MotorAndSteer(void);    //PWM初始化:电机正反转E3、E4、E5、E6；舵机输出A11
void initEMIOS_0ModulusCounter(void);   //光编计数，测正反转
void enableIrq(void);
void initLINFlex_0_UART(void);
void initAD(void);
void initTestIO(void);


#endif /* INIT_H_ */
