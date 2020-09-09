/*
 * Macros.h
 * This file for macros and #defines  
 *
 * Created: 2/5/2020 1:05:37 AM
 *  Author: Khaled Osama Eraky
 */ 


#ifndef MACROS_H_
#define MACROS_H_

#define SETBIT(R,B)			((R) |= (1<<(B)))
#define CLEARBIT(R,B)		((R) &=~(1<<(B)))
#define TOGGLEBIT(R,B)		((R) ^= (1<<(B)))
#define READBIT(R,B)		(((R) & (1<<(B)))>>(B))

#endif /* MACROS_H_ */