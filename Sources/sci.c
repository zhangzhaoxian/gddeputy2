/*
 * sci.c
 *
 *  Created on: Mar 15, 2016
 *      Author: цок╤
 */

#include "includes.h"

unsigned char SendHexHigh(unsigned char hex) {
	unsigned char temp=0;
	unsigned char high;
	high = hex >> 4;
	if(high < 10) {
		temp=high + '0';
	} else {
		temp=(high - 10) + 'A';
	}
	return temp;
}

unsigned char SendHexLow(unsigned char hex) {
	unsigned char temp=0;
	unsigned char low;
	low = hex & 0x0F;
	if(low < 10) {
		temp=low + '0';
	} else {
		temp=(low - 10) + 'A';
	}
	return temp;
}

unsigned char SendInt1(int currentdata)
{
  char temp;
  temp=currentdata>>8;
  return SendHexHigh(temp);
}

unsigned char SendInt2(int currentdata)
{
  char temp;
  temp=currentdata>>8;
  return SendHexLow(temp);
}

unsigned char SendInt3(int currentdata)
{
  char temp;
  temp=currentdata & 0x00FF;
  return SendHexHigh(temp);
}

unsigned char SendInt4(int currentdata)
{
  char temp;
  temp=currentdata & 0x00FF;
  return SendHexLow(temp);
}

