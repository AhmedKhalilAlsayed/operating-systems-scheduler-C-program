#ifndef PROCESS_INTERFACE_H_
#define PROCESS_INTERFACE_H_


#define PROCESSES_MAX 10

#define NULL_NUM -1

#define true  1
#define false 0
typedef char bool;

/// the state of process
enum ProcessStates
{
    runningState, readyState, blockedState, finishedState, newState
};

/// the state of program if now CPU exe or IO request
enum ProgramStates
{
    cpuState, ioState
};

/// Process Control Block (PCB)
typedef struct P
{
    int ID;
    int CPUTime1, CPUTime2, IOTime;
    int arivalTime, finishTime;
    int processState /*, programState*/;
    int arivalTimeFromBlockedQ;
} PCB;


#endif
