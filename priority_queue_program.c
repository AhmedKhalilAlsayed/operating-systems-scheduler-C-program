#include <stdio.h>
#include <stdlib.h>

#include "process_interface.h"

#include "priority_queue_interface.h"

void PriQueueEnqueue(PriQueue** q, PCB* data, int pri)
{

    PriQueue* new = (PriQueue*) malloc(sizeof(PriQueue));

    new->data = data;
    new->pri = pri;
    new->next = NULL;

    if (*q == NULL)
    {
        // for 1st node
        *q = new;

    }
    else
    {

        PriQueue* ptr = *q;

        //for 1st node
        if ((ptr->pri) > pri)
        {
            new->next = ptr;
            *q = new;

        }
        else
        {

            //for any node
            while ((ptr->next != NULL))
            {

                if ((ptr->next->pri) > pri)
                {
                    break;
                }
                else
                {
                    ptr = ptr->next;
                }

            }

            if (ptr->next != NULL)
            {

                new->next = ptr->next;
                ptr->next = new;

            }
            else
            {

                ptr->next = new;

            }

        }

    }
}

PCB* PriQueueDequeue(PriQueue** q, int* pri)
{

    PCB* p = NULL;

    if (*q != NULL)
    {

        p = (*q)->data;

        if (pri != NULL)
        {
            *pri = (*q)->pri;
        }

        PriQueue* delete = *q;
        *q = (*q)->next;
        free(delete);

    }
    else
    {
        //
    }

    return p;

}
