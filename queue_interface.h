/*
 * queue_interface.h
 *
 *  Created on: May 22, 2022
 *      Author: Mega Store
 */

#ifndef QUEUE_INTERFACE_H_
#define QUEUE_INTERFACE_H_

typedef struct q {
	PCB* arr[PROCESSES_MAX];
	int front;
	int rear;
} Queue;

void QueueInit(Queue* q);
void QueueEnqueue(Queue* q, PCB* data);
PCB* QueueDequeue(Queue* q);
bool QueueIsEmpty(Queue* q);

#endif /* QUEUE_INTERFACE_H_ */
