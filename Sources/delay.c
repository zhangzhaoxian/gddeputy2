/*
 * delay.c
 *
 *  Created on: Mar 14, 2016
 *      Author: 刘裕
 */
#include "includes.h"

void udelay(int us)
{
   int udelay_i;
   for(udelay_i=0;udelay_i<us; udelay_i++)
   {
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop); //_asm nop 1个时钟周期     1us
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   	       __asm(nop); __asm(nop); __asm(nop); __asm(nop);
   }
}

void mdelay(int ms)
{
	int mdelay_i;
	for(mdelay_i=0;mdelay_i<ms;mdelay_i++)
		udelay(1000);
}


