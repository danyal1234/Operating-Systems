#ifndef _PRIORITY_QUEUE_API_
 #define _PRIORITY_QUEUE_API_

#include "LinkedListAPI.h"

// Code taken from work done in previous course CIS2520 F17

/**
 * The basis of the queue 
 * Uses a backing linkedlist to create queue aswell as a member that holds queue length
 */
typedef struct queueContainer {
    List *list;
    int length;
} Queue;

/**
 * Function to create a new queue backed by a linked list
 * @param printFunction function pointer to print a single node of queue
 * @param deleteFunction function pointer to delete a single node of queue
 * @return pointer to created empty queue
 */
Queue *queueCreate(void (*printFunction)(void *toBePrinted), void (*deleteFunction)(void *toBeDeleted));

/**
 * Function to destory and free a previously created queue
 * @pre queue must be a created queue
 * @param priority queue
 * @post queue is destoryed and all associated memory is freed
 */
void queueDestroy(Queue *queue);

/**
 * Adds data sorted based on priority value
 * @param priority queue
 * @param data being inserted into queue
 * @param priority of data being inserted
 * @pre queue must be a created queue
 */
void queueInsert(Queue *queue, void *data, int priority);

/**
 * Removes a piece of data from the front of the queue
 * @pre queue must be a created queue
 * @param priority queue
 * @post the data from the front of the queue is removed
 */
void queuePop(Queue *queue);

/**
 * Gets the data from the front of the queue without removing it
 * @pre queue must be a created queue
 * @return the data from the front of the queue
 */
void *queuePeak(Queue *queue);

/**
 * Checks if queue is empty
 * @pre queue must be a created queue
 * @param priority queue
 * @return length of the queue, 0 if queue empty
 */
int queueisEmpty(Queue *queue);

void printQueue(Queue *queue);

#endif
