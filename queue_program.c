#include <stdio.h>

#include "process_interface.h"

#include "queue_interface.h"

void QueueInit(Queue* q)
{
    if (q != NULL)
    {
        q->front = -1;
        q->rear = -1;
    }
}

void QueueEnqueue(Queue* q, PCB* data)
{

    if (q->front == -1 && q->rear == -1)
    {
        q->rear++;
        q->front++;
        q->arr[q->front] = data;
    }
    else
    {

        //q->rear = (q->rear + 1) % MAX;

        if (((q->rear + 1) % PROCESSES_MAX) != q->front)
        {

            q->rear = (q->rear + 1) % PROCESSES_MAX;
            q->arr[q->rear] = data;

        }
        else
        {
            //full
        }
    }

}

PCB* QueueDequeue(Queue* q)
{

    PCB* retData = NULL;

    if (q->front == -1 && q->rear == -1)
    {

        //empty

    }
    else
    {

        retData = q->arr[q->front];

        if (q->front == q->rear)
        {

            q->front = -1;
            q->rear = -1;

            //retData = -111111;

        }
        else
        {

            q->front = (q->front + 1) % PROCESSES_MAX;

        }

    }

    return retData;
}

bool QueueIsEmpty(Queue* q)
{
    if (q != NULL)
    {

        if (q->front != -1 && q->rear != -1)
        {
            return false;
        }

    }

    return true;
}
