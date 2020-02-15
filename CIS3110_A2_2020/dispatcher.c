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
    int exchangeTimes[20];
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

int checkBlockedTimes(int pid, int count, int ids[], int times[]) {
    if (pid == 0)
    {
        return -1;
    }

    for (int i = 0; i < count; ++i) {
        if (ids[i] == pid) {
            ids[i] = 0;
            return times[i];
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

       printf("Process %3d wants to start at %6dms and run for %6dms and has %3d hard drive exchanges\n",  process_id, start_time, run_time, num_exchanges);  
       queueInsert(newQueue, &processes[processCounter], processes[processCounter].start_time);
       processCounter++;  
    }

    while(1) {
        while(1) {
            if (queueisEmpty(newQueue) == 0) {
                break;
            }

            Process* peakProcess = (Process*)queuePeak(newQueue);
            if (peakProcess && peakProcess->start_time == cpu.time) {
                queuePop(newQueue);

                int remainingtime = peakProcess->run_time - peakProcess->runningtime;

                if (remainingtime > peakProcess->run_time) {
                    cpu.process->readystart = cpu.time;
                    queueInsert(readyQueue, cpu.process, remainingtime);
                    cpu.process = peakProcess;
                } else {
                    peakProcess->readystart = cpu.time;
                    queueInsert(readyQueue, peakProcess, peakProcess->run_time);
                }
            } else {
                break;
            }
        }


        if (cpu.process->pid == 0 && queueisEmpty(readyQueue) != 0) {
            cpu.process = (Process*)queuePeak(readyQueue);
            cpu.process->readytotal += cpu.time - cpu.process->readystart;
            queuePop(readyQueue);
        } else if (cpu.process->pid != 0 && cpu.process->run_time == cpu.process->runningtime) {
            finishedRunning++;
            cpu.process->finishedCount = finishedRunning;
            if (queueisEmpty(readyQueue) == 0)
            {
                cpu.process = &zeroProcess;
            } else {
                cpu.process = (Process*)queuePeak(readyQueue);
                cpu.process->readytotal += cpu.time - cpu.process->readystart;
                queuePop(readyQueue);
            }
        }

        int blockedCheck = checkBlockedTimes(cpu.process->pid, totalblocktimes, pidblockid, pidblocktime);
        if (blockedCheck != -1 && blockedCheck <= cpu.time) {
            cpu.process->blockedstart = cpu.time;
            queueInsert(blockedQueue, cpu.process, blockedCount);
            blockedCount++;
            cpu.process->blockedCount++;

            if (queueisEmpty(readyQueue) != 0) {
                cpu.process = (Process*)queuePeak(readyQueue);
                cpu.process->readytotal += cpu.time - cpu.process->readystart;
                queuePop(readyQueue);
            } else {
                cpu.process = &zeroProcess;
            }
        }

        if (hddrunning) {
            hdd.time--;
            if (hdd.time == 0) {
                if (cpu.process->pid == 0)
                {
                    cpu.process = hdd.process;
                } else {
                    int remainingtime = hdd.process->run_time - hdd.process->runningtime;
                    int currentrunningtime = cpu.process->run_time - cpu.process->runningtime;
                    if (remainingtime < currentrunningtime) {
                        queueInsert(readyQueue, cpu.process, currentrunningtime);
                        cpu.process = hdd.process;
                    } else {
                        queueInsert(readyQueue, hdd.process, remainingtime);
                    }
                }

                hdd.time = harddrive;

                if (queueisEmpty(blockedQueue) == 0) {
                    hddrunning = false;
                } else {
                    hdd.process = (Process*)queuePeak(blockedQueue);
                    hdd.process->blockedtotal += cpu.time - hdd.process->blockedstart;
                    queuePop(blockedQueue);
                }
            }
        } else {
            if (queueisEmpty(blockedQueue) != 0) {
                hdd.process = (Process*)queuePeak(blockedQueue);
                hdd.process->blockedtotal += cpu.time - hdd.process->blockedstart;
                hddrunning = true;
                queuePop(blockedQueue);
            }
        }

        printQueue(blockedQueue);
        if (queueisEmpty(newQueue) == 0 && queueisEmpty(blockedQueue) == 0 && queueisEmpty(readyQueue) == 0 && cpu.process->pid == 0 && !hddrunning) {
            break;
        }

        cpu.process->runningtime += 100;
        cpu.time +=  100;
    }

    printf("0 %d\n", zeroProcess.runningtime);

    for (int j = 0; j < processCounter; ++j) {
        for (int i = 0; i < processCounter; ++i) {
            if (processes[i].finishedCount == j+1) {
                printf("%d %d %d %d\n", processes[i].pid, processes[i].runningtime, processes[i].readytotal, processes[i].blockedtotal);
            }
        }
    }

}
