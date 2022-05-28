#include <stdio.h>
#include <stdlib.h>

#include "process_interface.h"

#include "array_interface.h"

#include "queue_interface.h"

#include "priority_queue_interface.h"

#define FCFS_MODE   0
#define RR_MODE     1

/// declarations

void readFromFile();

void initScheduler();

void inputAndBlockedToReadyQ();

bool FCFS_execute();

bool RR_execute();

void printState(int state);

void printProcessStates();

bool isAllProcessFinished();

/// end

/// global vars
// all processes of system saved here
Array arr_systemProcesses;
// all processes but will be enter as we go (depend on current time)
Array arr_inputProcesses;
// blocked processes
Array arr_blockedProcesses;
// ready processes
Queue q_readyProcesses;
// current process (in processor)
PCB* curntProcess;
// current simulation time
int curntTime = 0;

FILE* resultFile;

int q;
int qRunning;

/// end

int main(void)
{

/// init arrays and vars to start

    float cpuUtil = 0;

    int mode = NULL_NUM;

    resultFile = NULL;

    initScheduler();


    printf("\tScheduler program :)\n\n");


    /// read from file to arr_systemProcesses[]
    readFromFile();

    printf(">which scheduler: type %d for FCFS, type %d for RR , others to exit : \n",FCFS_MODE, RR_MODE);
    scanf("%d", &mode);

    switch(mode)
    {
    case FCFS_MODE:
        resultFile = fopen("FCFS_result.txt","w");
        break;

    case RR_MODE:
        resultFile = fopen("RR_result.txt","w");

        printf(">enter 'q' time of Round Robin = ");

        while(1)
        {

            scanf("%d", &q);

            if(q <= 0)
                printf("invalid 'q' time, should be greater than 0, 'q' = ");
            else
            {
                qRunning = q;
                break;
            }
        }


        fprintf(resultFile, "\tq = %d\n",q);

        break;
    default :
        exit(1);

    }


    /// from arr_systemProcesses[] to arr_inputProcesses[]

    for (int i = 0; i < arr_systemProcesses.size; i++)
    {
        // from system to input
        ArrayAdd(&arr_inputProcesses, arr_systemProcesses.arr[i]);
    }


    /// <FOR LOOP> FOR SIMULATION TIME

    int isExtCall = false;

    curntTime=0;
    for (; curntTime < 1000; curntTime++)
    {

        isExtCall = false;

        printf("%d-> ",curntTime);
        fprintf(resultFile,"%d-> ",curntTime);

        inputAndBlockedToReadyQ(curntTime);

        do
        {

            dispatcher();

            if(mode == FCFS_MODE)
            {
                isExtCall = FCFS_execute();
            }
            else
            {
                isExtCall = RR_execute();
            }

            // isExtCall == false
            if(isExtCall == false)
            {
                /// print the diagram
                printProcessStates(mode);

                if(curntProcess!= NULL && curntProcess->processState == runningState)
                    cpuUtil++;
            }

        }
        while (isExtCall);


        /// to detect the end the simulation
        /// to stop the outer for loop of simulation

        if(isAllProcessFinished() == true)
            break;


    }

    /// The summary of system

    printf("Finishing Time  : %d\n",curntTime-1);
    fprintf(resultFile,"Finishing Time  : %d\n",curntTime-1);
    printf("CPU Utilization : %.3f\n",cpuUtil/curntTime);
    fprintf(resultFile,"CPU Utilization : %.3f\n",cpuUtil/curntTime);

    for(int i=0; i<arr_systemProcesses.size; i++)
    {

        printf("Turnaround time of Process %d: ",arr_systemProcesses.arr[i]->ID);
        fprintf(resultFile,"Turnaround time of Process %d: ",arr_systemProcesses.arr[i]->ID);
        printf("%d\n",arr_systemProcesses.arr[i]->finishTime - arr_systemProcesses.arr[i]->arivalTime + 1);
        fprintf(resultFile,"%d\n",arr_systemProcesses.arr[i]->finishTime - arr_systemProcesses.arr[i]->arivalTime + 1);

    }



    if(resultFile != NULL)
        fclose(resultFile);

    return 0;
}



/// to notify the system all processes is done
bool isAllProcessFinished()
{

    int countFinishedP = 0;
    for(int i = 0; i<arr_systemProcesses.size; i++)
    {
        if(arr_systemProcesses.arr[i]->processState == finishedState)
        {
            countFinishedP++;
        }
    }

    if(countFinishedP == arr_systemProcesses.size)
    {
        return true;

    }
    else
    {
        return false;

    }

}

/// should be called at the start of program
void initScheduler()
{
    curntProcess = NULL;
    QueueInit(&q_readyProcesses);
    ArrayInit(&arr_systemProcesses);
    ArrayInit(&arr_inputProcesses);
    ArrayInit(&arr_blockedProcesses);
}

/// read file
void readFromFile()
{
// "/home/ubuntu/Desktop/3/Test.txt"
// "/home/ubuntu/Desktop/3/Test-file-1(1).txt"
// "/home/ubuntu/Desktop/3/Test-file-2(1).txt"
// "/home/ubuntu/Desktop/3/Test-file-3(1).txt"

    char path[100];

    printf(">please Enter the path of your file: ");

    scanf("%s", path);

    FILE *f = fopen(path,"r");

    if(f == NULL)
    {

        printf("File not found !\n\n");
        exit(1);
    }

    PCB tempPCB;

    tempPCB.arivalTimeFromBlockedQ = NULL_NUM;
    tempPCB.finishTime = NULL_NUM;
    tempPCB.processState = newState;

    while(fscanf(f, "%*c%d %d %d %d\n",&tempPCB.ID, &tempPCB.CPUTime1, &tempPCB.IOTime, &tempPCB.arivalTime) != EOF)
    {
        //printf("asd ");
        tempPCB.CPUTime2 = tempPCB.CPUTime1;

        PCB* ptrPCB = (PCB*)malloc(sizeof(PCB));

        if(ptrPCB == NULL)
        {
            printf("Cannot find Space!\n");
            exit(1);
        }

        *ptrPCB = tempPCB;

        ArrayAdd(&arr_systemProcesses, ptrPCB);

    }

    if(f != NULL)
    {
        fclose(f);
    }


}

/// target:
/// check input and blocked process to ready queue
void inputAndBlockedToReadyQ()
{

/// exp:
// scan the input queue with curntTime
// if there greater than enter at the same time
// sort them with lowest ID
// we will use priority queue with ID

// in some cases we will need priority queue, so we will use buffer to save all process has the same
// arrival time with priority its process ID

    PriQueue* buffer = NULL;

    // the index in arr_systemProcesses[]
    int index = NULL_NUM;


    /// send from input processes to buffer

    while (1)
    {

        index = ArrayFindRetIdx(&arr_inputProcesses, curntTime, NULL_NUM);

        if (index == NULL_NUM)
            break;

        arr_inputProcesses.arr[index]->processState = readyState;

        PriQueueEnqueue(&buffer, arr_inputProcesses.arr[index],
                        arr_inputProcesses.arr[index]->ID);

        ArrayDelete(&arr_inputProcesses, index);

    }

    /// send from blocked Q to buffer

    while (1)
    {

        index = ArrayFindRetIdx(&arr_blockedProcesses, NULL_NUM, curntTime);

        if (index == NULL_NUM)
            break;


        arr_blockedProcesses.arr[index]->processState = readyState;

        PriQueueEnqueue(&buffer, arr_blockedProcesses.arr[index],
                        arr_blockedProcesses.arr[index]->ID);

        ArrayDelete(&arr_blockedProcesses, index);

    }

    /// send buffer to ready Q

    while (buffer != NULL)
    {

        QueueEnqueue(&q_readyProcesses, PriQueueDequeue(&buffer, NULL));

    }


}

/// the dispatcher will choose which process will enter to processor
void dispatcher()
{
    if (curntProcess == NULL)
    {

        if (!QueueIsEmpty(&q_readyProcesses))
        {

            curntProcess = QueueDequeue(&q_readyProcesses);

            curntProcess->processState = runningState;

            qRunning = q;
        }

    }
    else
    {
        // other process is running now
    }
}

/// this function is the processor operation
bool FCFS_execute()
{

    // if external call happened like I/O request
    bool isExtCall = false;

    if (curntProcess != NULL)
    {

        if (curntProcess->CPUTime1 >= 0)
        {

            if (curntProcess->CPUTime1 == 0)
            {
                // IO request
                // change state to blocking
                if(curntProcess->IOTime>0)
                {

                    curntProcess->processState = blockedState;


                    curntProcess->arivalTimeFromBlockedQ = curntProcess->IOTime
                                                           + curntTime;

                    curntProcess->IOTime = NULL_NUM;


                    ArrayAdd(&arr_blockedProcesses, curntProcess);

                    curntProcess = NULL;

                    isExtCall = true;
                    return isExtCall;
                }
                else
                {
                    //
                }



            }
            else
            {
                curntProcess->CPUTime1--;

                return isExtCall;
            }


        }



        if (curntProcess->CPUTime2 >= 0)
        {

            if (curntProcess->CPUTime2 == 0)
            {

                curntProcess->processState = finishedState;

                curntProcess->finishTime = curntTime-1;

                curntProcess = NULL;

                isExtCall = true;
                return isExtCall;

            }
            else
            {
                curntProcess->CPUTime2--;

                isExtCall = false;
                return isExtCall;
            }

        }

    }

    return false;
}

/// this function is the processor operation
bool RR_execute()
{

    // if external call happened like I/O request
    bool isExtCall = false;

    if (curntProcess != NULL)
    {

        if (curntProcess->CPUTime1 >= 0)
        {

            if (curntProcess->CPUTime1 == 0)
            {
                // IO request
                // change state to blocking
                if(curntProcess->IOTime>0)
                {

                    curntProcess->processState = blockedState;


                    curntProcess->arivalTimeFromBlockedQ = curntProcess->IOTime
                                                           + curntTime;

                    curntProcess->IOTime = NULL_NUM;


                    ArrayAdd(&arr_blockedProcesses, curntProcess);

                    curntProcess = NULL;

                    isExtCall = true;
                    return isExtCall;
                }
                else
                {
                    //
                }



            }
            else
            {
                if(qRunning > 0)
                {

                    curntProcess->CPUTime1--;
                    qRunning--;

                }
                else
                {

                    curntProcess->processState = readyState;
                    QueueEnqueue(&q_readyProcesses, curntProcess);
                    curntProcess = NULL;

                    return true;


                }


                return isExtCall;
            }


        }



        if (curntProcess->CPUTime2 >= 0)
        {

            if (curntProcess->CPUTime2 == 0)
            {

                curntProcess->processState = finishedState;

                curntProcess->finishTime = curntTime-1;

                curntProcess = NULL;

                isExtCall = true;
                return isExtCall;

            }
            else
            {
                if(qRunning > 0)
                {

                    curntProcess->CPUTime2--;
                    qRunning--;

                }
                else
                {

                    curntProcess->processState = readyState;
                    QueueEnqueue(&q_readyProcesses, curntProcess);
                    curntProcess = NULL;

                    return true;


                }

                isExtCall = false;
                return isExtCall;
            }

        }

    }

    return false;

}

/// covert the state number with its String
void printState(int state)
{

    switch(state)
    {

    case runningState:
        if(resultFile != NULL)
            fprintf(resultFile,"running");
        printf("running");
        break;

    case readyState:
        if(resultFile != NULL)
            fprintf(resultFile,"ready");
        printf("ready");
        break;

    case blockedState:
        if(resultFile != NULL)
            fprintf(resultFile,"blocked");
        printf("blocked");
        break;
    }

}

/// print the diagram
void printProcessStates()
{


    for(int i=0; i<arr_systemProcesses.size; i++)
    {
        if((arr_systemProcesses.arr[i]->processState != finishedState) && (arr_systemProcesses.arr[i]->processState != newState))
        {
            printf("\t%d: ",arr_systemProcesses.arr[i]->ID);
            fprintf(resultFile,"\t%d: ",arr_systemProcesses.arr[i]->ID);

            printState(arr_systemProcesses.arr[i]->processState);

            //if(arr_systemProcesses.arr[i]->processState == runningState)
            //  cpuUtil++;

        }



    }

    printf("\n");
    fprintf(resultFile,"\n");




}
