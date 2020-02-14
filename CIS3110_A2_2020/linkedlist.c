#include <stdio.h>
#include <stdlib.h>
#include "LinkedListAPI.h"

int compare(const void *first, const void *second) {
    int i1 = ((Node *) first)->priority;
    int i2 = ((Node *) second)->priority;
    return i1 - i2;
}

List *initializeList(void (*printFunction)(void *toBePrinted),void (*deleteFunction)(void *toBeDeleted),int (*compareFunction)(const void *first,const void *second)) {
    
    List *a = malloc(sizeof(List));

    a->head = NULL;
    a->tail = NULL;
    a->deleteNode = deleteFunction;
    a->compare = compareFunction;
    a->printNode = printFunction;

    return a;
}

Node *initializeNode(void *data, int priority) {

    Node *a = malloc(sizeof(Node));

	a->data = data;
    a->next = NULL;
    a->previous = NULL;
    a->priority = priority;

    return a;
}

void insertFront(List *list, Node* a) {

    if (list == NULL) {
        return;
    }

    a->next = list->head;
    if (list->head != NULL) {
        list->head->previous = a;
    }

    list->head = a;

    if (list->tail == NULL) {
        list->tail = list->head;
    }
}

void insertBack(List *list, Node* a) {

    if (list == NULL) {
        return;
    }

    a->previous = list->tail;
    if (list->tail != NULL) {
        list->tail->next = a;
    }

    list->tail = a;

    if (list->head == NULL) {
        list->head = list->tail;
    }
}


void deleteList(List *list) {

    if(list == NULL) {
        return;
    }
    if(list->head == NULL) {
        return;
    }

    Node *a = NULL;
    a = malloc(sizeof(Node));

    while(list->head != NULL){
        a->next = list->head;
        list->head = list->head->next;
        list->deleteNode(a->next->data);
        free(a->next);
    }
	free(a);
    list->head = NULL;
    list->tail = NULL;
}

void insertSorted(List *list, void *toBeAdded, int priority) {

    if(list == NULL) {
        return;
    }

    Node *a;
    Node *b;
    a = malloc(sizeof(Node));
    b = initializeNode(toBeAdded, priority);

    if(list->head == NULL) {
        list->head = b;
        list->tail = b;
        b->next = NULL;
        b->previous = NULL;
		free(a);
        return;
    }

    a->next = list->head;
    
    if(list->compare(list->head,b) > 0) {
        insertFront(list, b);
        free(a);
        return;
    }

    while(a->next != NULL) {
        if(list->compare(a->next,b) > 0) {
            b->next = a->next;
            b->previous = a->next->previous;
            a->next->previous->next = b;
            a->next->previous = b;
            free(a);
            return;
        }

        if(a->next->next == NULL) {
            insertBack(list, b);
            free(a);
            return;
        }
        a->next = a->next->next;
    }

    free(a);
    return;
}

int deleteNodeFromList(List *list, void *toBeDeleted) {

	if(list == NULL) {
        return EXIT_FAILURE;
    }

     if(toBeDeleted == NULL) {
        return EXIT_FAILURE;
    }

    if(list->head == NULL) {
        return EXIT_FAILURE;
    }

    Node *a;
    a = malloc(sizeof(Node));
    a->next = list->head;

    while(a->next != NULL) {
        if(list->compare(toBeDeleted,a->next->data) == 0) {
            if(a->next->next == NULL && a->next->previous == NULL) {
                list->head = NULL;
                list->tail = NULL;
            }
            
            else if(a->next->previous == NULL) {
                list->head = a->next->next;
                a->next->next = NULL;
                list->head->previous = NULL;
            }
     
            else if(a->next->next == NULL) {
                list->tail = a->next->previous;
                a->next->previous = NULL;
                list->tail->next = NULL;
            }
     
            else {
                a->next->previous->next = a->next->next;
                a->next->next->previous = a->next->previous;
                a->next->next = NULL;
                a->next->previous = NULL;
            }
        }
        
        a->next = a->next->next;
        if(a->next == NULL){
            return -1;
        }

        }
    
	       return EXIT_SUCCESS;

}

void *getFromFront(List *list) {

    if(list == NULL){
        return NULL;
    }

    if(list->head == NULL){
        return NULL;
    } 

    return list->head->data;
}

void *getFromBack(List *list) {

    if(list == NULL){
        return NULL;
    }

    if(list->tail == NULL){
        return NULL;
    } 
    
    return list->tail->data; 
}

void printForward(List *list) {

    if (list == NULL) {
        return;
    }

    Node *a = list->head;
    while (a != NULL) {
        list->printNode(a->data);
        a = a->next;
    }
}

void printBackwards(List *list) {

    if (list == NULL) {
        return;
    }

    Node *a = list->tail;
    while (a != NULL) {
        list->printNode(a->data);
        a = a->previous;
    }
}
