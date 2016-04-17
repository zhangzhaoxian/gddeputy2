/*
 * debug.h
 *
 *  Created on: Mar 15, 2016
 *      Author: цок╤
 */

#ifndef DEBUG_H_
#define DEBUG_H_

extern int Left[128];
extern int Right[128];
extern unsigned char *send;
extern unsigned char putstring[];
extern unsigned int Ts;
extern unsigned int Tc;
extern int CurrentSteer;

extern unsigned char S3_last;
extern unsigned char S4_last;
extern unsigned char S5_last;
extern unsigned char S6_last;
extern unsigned char keymode;

void BlueTx(void); 
void LINFlex_TX(unsigned char data);
void LINFlex_TX_Interrupt(void);
extern void KeyJudge(void);

#endif /* DEBUG_H_ */
