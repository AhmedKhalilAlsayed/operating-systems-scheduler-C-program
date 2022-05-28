/*
 * array_interface.h
 *
 *  Created on: May 24, 2022
 *      Author: Mega Store
 */

#ifndef ARRAY_INTERFACE_H_
#define ARRAY_INTERFACE_H_

typedef struct a {
	PCB* arr[PROCESSES_MAX];
	int size;
} Array;

void ArrayInit(Array* ptr);
void ArrayAdd(Array* ptr, PCB* pP);
int ArrayFindRetIdx(Array* ptr, int arivalTime, int arivalBlockedTime);
void ArrayDelete(Array* ptr, int index);

#endif /* ARRAY_INTERFACE_H_ */
