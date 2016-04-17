/*
 * capture.h
 *
 *  Created on: Mar 14, 2016
 *      Author: ��ԣ
 */

#ifndef CAPTURE_H_
#define CAPTURE_H_

#define CCDR_SI  SIU.GPDO[10].R   //���������Դ������Ķ˿� SI  A10
#define CCDR_CLK SIU.GPDO[11].R   //���������Դ������Ķ˿� CLK A11

#define CCDL_SI  SIU.GPDO[66].R   //���������Դ������Ķ˿� SI  E2
#define CCDL_CLK SIU.GPDO[6].R    //���������Դ������Ķ˿� CLK A6

extern void ImageCapture(int L[],int R[]);
extern void ImageCopy(int L[],int PL[]);
extern void SpeedCount(void);
extern int PixelLeft[128];
extern int PixelRight[128];
extern unsigned int speedcounter1,speedcounter2,speedcounter3,speedcounter4;

#endif /* CAPTURE_H_ */
