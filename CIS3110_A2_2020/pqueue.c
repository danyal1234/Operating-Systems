#include <stdio.h>
#include <stdlib.h>
#include "PriorityQueueAPI.h"
#include "LinkedListAPI.h"

Queue *queueCreate(void (*printFunction)(void *toBePrinted), void (*deleteFunction)(void *toBeDeleted))
{
    Queue *queue = malloc(sizeof(Queue));
    queue->list = initializeList(printFunction, deleteFunction, &compare);
    queue->length = 0;
    return queue;
}

void printQueue(Queue *queue) {
    printForward(queue->list);
}

void queueDestroy(Queue *queue)
{
    if(queue->list == NULL){
        return;
    }

    deleteList(queue->list);
    free(queue);
}

void queueInsert(Queue *queue, void *data, int priority)
{
    if(queue->list == NULL){
        return;
    }

    insertSorted(queue->list, data, priority);
    queue->length++;
}

void queuePop(Queue *queue)
{
    if (queue->length == 0) {
        return;
    }

    int i = deleteNodeFromList(queue->list, queuePeak(queue));
    
    if(i == -1){
		queue->length--;
	}
}

void *queuePeak(Queue *queue)
{
    if (queue->length == 0) {
        return NULL;
    } 
    else {
        return getFromFront(queue->list);
    }
}

int queueisEmpty(Queue *queue)
{
    if (queue->length == 0) {
        return 0;
    }
    else {
        return queue->length;
    }
}
