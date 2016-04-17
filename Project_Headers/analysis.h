/*
 * analysis.h
 *
 *  Created on: Mar 15, 2016
 *      Author: 孟硕
 */

#ifndef ANALYSIS_H_
#define ANALYSIS_H_

extern int A[128];
extern int B[128];
extern int al_end,ar_end,bl_end,br_end;
extern int a_start,b_start;
extern int a_value,b_value;                          //判断跳变沿的差值标准
extern int a_T,b_T;                                  //黑白阈值
extern int al_count,ar_count,bl_count,br_count;  //白点计数
extern int al_flag,ar_flag,bl_flag,br_flag,allflag;//0,1,2,3,4;黑，白，白-黑，黑-白，错误
extern int wrong_flag;
extern int al_edge,ar_edge,bl_edge,br_edge;//跳变沿
extern int error;

extern void DataSet(void);
extern void PixelScan(void);
extern void ErrorCalculate(void);

#endif /* ANALYSIS_H_ */
