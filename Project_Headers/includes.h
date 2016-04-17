/*
 * includes.h
 *
 *  Created on: Mar 12, 2016
 *      Author: p
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

#ifndef _TYPEDEF_BYTE_
#define _TYPEDEF_BYTE_
typedef unsigned char BYTE;
#endif

#ifndef _TYPEDEF_SBYTE_
#define _TYPEDEF_SBYTE_
typedef signed char SBYTE;
#endif

#ifndef _TYPEDEF_WORD_
#define _TYPEDEF_WORD_
typedef unsigned short WORD;
#endif

#ifndef _TYPEDEF_SWORD_
#define _TYPEDEF_SWORD_
typedef signed short SWORD;
#endif

#ifndef _TYPEDEF_DWORD_
#define _TYPEDEF_DWORD_
//typedef unsigned int DWORD;
typedef unsigned long int DWORD;
#endif

#include "init.h"
#include "MPC5604B.h"
#include "IntcInterrupts.h"
#include "pit.h"
#include "capture.h"
#include "delay.h"
#include "speed.h"
#include "steer.h"
#include "analysis.h"
#include "debug.h"
#include "sci.h"
#include "oled.h"

#define ABS(x)              ((x) > 0 ? (x) : 0-(x))

/***************keys*****************/
#define S6 SIU.GPDI[13].R
#define S5 SIU.GPDI[14].R
#define S4 SIU.GPDI[39].R	//E0
#define S3 SIU.GPDI[15].R

/***************steer*****************/
#define TargetSteer EMIOS_0.CH[4].CBDR.R // ¶æ»úÖµ
#define CENTER 2430//3720

#endif /* INCLUDES_H_ */
