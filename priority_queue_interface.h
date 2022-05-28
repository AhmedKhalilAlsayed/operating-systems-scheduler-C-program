/*
 * priority_queue_interface.h
 *
 *  Created on: May 22, 2022
 *      Author: Mega Store
 */

#ifndef PRIORITY_QUEUE_INTERFACE_H_
#define PRIORITY_QUEUE_INTERFACE_H_

typedef struct Q {
	int pri;
	PCB* data;
	struct Q* next;
} PriQueue;

void PriQueueEnqueue(PriQueue** q, PCB* data, int pri);

PCB* PriQueueDequeue(PriQueue** q, int* pri);

#endif /* PRIORITY_QUEUE_INTERFACE_H_ */
