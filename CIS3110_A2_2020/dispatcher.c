/*
dispatcher.c

Student Name : Danyal Mahmood
Student ID # : 0956989

Dispatch Algorithm : ....
*/

#include <string.h>
#include <stddef.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 100

/*
Any required standard libraries and your header files here
*/
#include "PriorityQueueAPI.h"
#include <stdbool.h>

typedef struct process{
    int pid;
    int start_time;
    int readystart;
    int readytotal;
    int blockedstart;
    int blockedtotal;
    int blockedCount;
    int run_time;
    int finishedCount;
    int runningtime;
} Process;

typedef struct cpu {
    int time;
    Process* process;
} CPU;

typedef struct hdd {
    int time;
    Process* process;
} HDD;


void printNode(void* data){
    printf("%d %d %d\n", ((Process*)data)->pid, ((Process*)data)->start_time, ((Process*)data)->run_time);
}

void deleteListNode(void *data){
}

int checkBlockedTimes(int pid, int count, int ids[], int times[], int blockedCount) {
    if (pid == 0)
    {
        return -1;
    }

    int blockcount = 0;

    for (int i = 0; i < count; ++i) {
        if (ids[i] == pid) {
            if (blockcount == blockedCount) {
                return times[i];
            } else {
                blockcount++;
            }
        }
    }

    return -1;
}

void dispatcher(FILE *fd, int harddrive){
    /*
        Your code here.
        You may edit the following code
    */
    char line_buffer[MAX_LINE_LENGTH];
    int start_time, run_time, process_id, num_exchanges, exchange_time = 0;
    char *token;

    Queue* readyQueue = queueCreate(&printNode, &deleteListNode);
    Queue* newQueue = queueCreate(&printNode, &deleteListNode);
    Queue* blockedQueue = queueCreate(&printNode, &deleteListNode);

    Process* processes = malloc(sizeof(Process)*1000);
    int processCounter = 0;
    int blockedCount = 0;
    int finishedRunning = 0;
    bool hddrunning = false;

    int pidblockid[100];
    int pidblocktime[100];
    int totalblocktimes = 0;

    Process zeroProcess;
    zeroProcess.pid = 0;
    zeroProcess.runningtime = 0;
    CPU cpu;
    cpu.process = &zeroProcess;
    cpu.time = 0;
    HDD hdd;
    hdd.time = harddrive;
    
    //Process simulation input line by line
    while (fgets(line_buffer, MAX_LINE_LENGTH, fd) != NULL && line_buffer[0] != '\n') {
        token = strtok(line_buffer, " ");
        sscanf(token, "%d",&start_time);
        processes[processCounter].start_time = start_time;
       
        token = strtok(NULL, " ");
        sscanf(token, "%d",&process_id);
        processes[processCounter].pid = process_id;
        
        token = strtok(NULL, " ");
        sscanf(token, "%d",&run_time);
        processes[processCounter].run_time = run_time;
        
        num_exchanges = 0;
        token = strtok(NULL, " ");
        while ( token != NULL ) {
            num_exchanges += sscanf(token, "%d",&exchange_time);
            pidblockid[totalblocktimes] = process_id;
            pidblocktime[totalblocktimes] = exchange_time;
            totalblocktimes++;
            token = strtok(NULL, " ");
        }

       queueInsert(newQueue, &processes[processCounter], processes[processCounter].start_time);
       processCounter++;  
    }

    while(1) {
        // check to see if any processes are requesting hdd exchange
        int blockedCheck = checkBlockedTimes(cpu.process->pid, totalblocktimes, pidblockid, pidblocktime, cpu.process->blockedCount);
        if (blockedCheck != -1 && blockedCheck == cpu.process->runningtime) {
            cpu.process->blockedCount++;
            cpu.process->blockedstart = cpu.time;
            queueInsert(blockedQueue, cpu.process, blockedCount);
            blockedCount++;

            if (queueisEmpty(readyQueue) != 0) {
                cpu.process = (Process*)queuePeak(readyQueue);
                cpu.process->readytotal += cpu.time - cpu.process->readystart;
                queuePop(readyQueue);
            } else {
                cpu.process = &zeroProcess;
            }
        }

        // check to see if any new proccesses have arrived
        while(1) {
            if (queueisEmpty(newQueue) == 0) {
                break;
            }

            Process* peakProcess = (Process*)queuePeak(newQueue);
            if (peakProcess && peakProcess->start_time == cpu.time) {
                queuePop(newQueue);

                int currentRemainingTime = cpu.process->run_time - cpu.process->runningtime;
                if (cpu.process->pid == 0 || peakProcess->run_time < currentRemainingTime) {
                    if (cpu.process->pid != 0) {
                        cpu.process->readystart = cpu.time;
                        queueInsert(readyQueue, cpu.process, currentRemainingTime);
                    }
                    cpu.process = peakProcess;    
                } else {
                    peakProcess->readystart = cpu.time;
                    queueInsert(readyQueue, peakProcess, peakProcess->run_time);
                }
            } else {
                break;
            }
        }

        // check to see if process finished running, or any new processes to run if 0 process running
        if (cpu.process->pid == 0 && queueisEmpty(readyQueue) != 0) {
            cpu.process = (Process*)queuePeak(readyQueue);
            cpu.process->readytotal += cpu.time - cpu.process->readystart;
            queuePop(readyQueue);
        } else if (cpu.process->pid != 0 && cpu.process->run_time == cpu.process->runningtime) {
            if (checkBlockedTimes(cpu.process->pid, totalblocktimes, pidblockid, pidblocktime, cpu.process->blockedCount) != cpu.process->run_time) {
                finishedRunning++;
                cpu.process->finishedCount = finishedRunning;
            } else {
                cpu.process->blockedCount++;
                cpu.process->blockedstart = cpu.time;
                queueInsert(blockedQueue, cpu.process, blockedCount);
                blockedCount++;
            }

            if (queueisEmpty(readyQueue) == 0) {
                cpu.process = &zeroProcess;
            } else {
                cpu.process = (Process*)queuePeak(readyQueue);
                cpu.process->readytotal += cpu.time - cpu.process->readystart;
                queuePop(readyQueue);
            }
        }

        // hard drive process
        if (hddrunning) {
            hdd.time--;

            // if hard drive done running, assign new task to task if blocked and update cpu process
            if (hdd.time == 0) {
                if (hdd.process->run_time == hdd.process->runningtime) {
                    finishedRunning++;
                    hdd.process->finishedCount = finishedRunning;
                } else if (cpu.process->pid == 0) {
                    cpu.process = hdd.process;
                } else {
                    int remainingtime = hdd.process->run_time - hdd.process->runningtime;
                    int currentrunningtime = cpu.process->run_time - cpu.process->runningtime;
                    if (remainingtime < currentrunningtime) {
                        cpu.process->readystart = cpu.time;
                        queueInsert(readyQueue, cpu.process, currentrunningtime);
                        cpu.process = hdd.process;
                    } else {
                        hdd.process->readystart = cpu.time;
                        queueInsert(readyQueue, hdd.process, remainingtime);
                    }
                }

                hdd.time = harddrive;

                if (queueisEmpty(blockedQueue) == 0) {
                    hddrunning = false;
                } else {
                    hdd.process = (Process*)queuePeak(blockedQueue);
                    hdd.process->blockedtotal += cpu.time - hdd.process->blockedstart + harddrive;
                    queuePop(blockedQueue);
                }
            }
        } else {
            if (queueisEmpty(blockedQueue) != 0) {
                hdd.process = (Process*)queuePeak(blockedQueue);
                hdd.process->blockedtotal += cpu.time - hdd.process->blockedstart + harddrive;
                hddrunning = true;
                queuePop(blockedQueue);
            }
        }

        // finished processing
        if (queueisEmpty(newQueue) == 0 && queueisEmpty(blockedQueue) == 0 && queueisEmpty(readyQueue) == 0 && cpu.process->pid == 0 && !hddrunning) {
            break;
        }

        cpu.process->runningtime++;
        cpu.time++;
    }

    // print results
    printf("0 %d\n", zeroProcess.runningtime);

    for (int j = 0; j < processCounter; ++j) {
        for (int i = 0; i < processCounter; ++i) {
            if (processes[i].finishedCount == j+1) {
                printf("%d %d %d %d\n", processes[i].pid, processes[i].runningtime, processes[i].readytotal, processes[i].blockedtotal);
            }
        }
    }

    free(processes);
    queueDestroy(readyQueue);
    queueDestroy(blockedQueue);
    queueDestroy(newQueue);
}
