/*
 * sci.h
 *
 *  Created on: Mar 15, 2016
 *      Author: цок╤
 */

#ifndef SCI_H_
#define SCI_H_

unsigned char SendHexHigh(unsigned char hex);
unsigned char SendHexLow(unsigned char hex);
unsigned char SendInt1(int currentdata);
unsigned char SendInt2(int currentdata);
unsigned char SendInt3(int currentdata);
unsigned char SendInt4(int currentdata);

#endif /* SCI_H_ */
