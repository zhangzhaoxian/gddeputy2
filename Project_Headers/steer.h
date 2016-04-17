/*
 * steer.h
 *
 *  Created on: Mar 15, 2016
 *      Author: цок╤
 */

#ifndef STEER_H_
#define STEER_H_

extern int target_offset,last_offset;

void SET_steer(unsigned int steer);
extern void Steer_PDSet(void);
extern void SteerControl(void);

#endif /* STEER_H_ */
