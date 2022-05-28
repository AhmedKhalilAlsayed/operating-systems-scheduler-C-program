#include <stdio.h>

#include "process_interface.h"

#include "array_interface.h"

void ArrayInit(Array* ptr)
{
    if (ptr != NULL)
    {
        ptr->arr[0] = NULL;
        ptr->size = 0;
    }
}

void ArrayAdd(Array* ptr, PCB* pP)
{
    if (ptr != NULL && pP != NULL)
    {
        ptr->arr[ptr->size] = pP;
        ptr->size++;
    }
}

int ArrayFindRetIdx(Array* ptr, int arivalTime, int arivalBlockedTime)
{

    int index = NULL_NUM;

    if (ptr != NULL)
    {

        if (arivalTime != NULL_NUM)
        {

            for (int i = 0; i < ptr->size; i++)
            {
                if (ptr->arr[i]->arivalTime == arivalTime)
                {
                    index = i;
                    break;
                }
            }

        }
        else if (arivalBlockedTime != NULL_NUM)
        {

            for (int i = 0; i < ptr->size; i++)
            {
                if (ptr->arr[i]->arivalTimeFromBlockedQ == arivalBlockedTime)
                {
                    index = i;
                    break;
                }
            }

        }
        else
        {

            //
        }

    }

    return index;
}

void ArrayDelete(Array* ptr, int index)
{
    if (ptr != NULL && (index >= 0 && index < ptr->size))
    {

        PCB* temp = NULL;
        for (int i = index; i < ptr->size; i++)
        {
            temp = ptr->arr[i];
            ptr->arr[i] = ptr->arr[i + 1];
            ptr->arr[i + 1] = temp;
        }

        //ptr->arr[ptr->size - 1] = NULL;
        ptr->size--;

    }
}
