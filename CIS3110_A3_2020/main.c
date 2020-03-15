#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "LinkedListAPI.h"

typedef struct tlbentry {
    int page;
    int frame;
} TLBentry;

void printNode(void* data){
}

void deleteListNode(void *data){
	free((TLBentry*)data);
}

int compareNode(const void *first,const void *second) {
	return 0;
}
 
int main(int argc, char **argv) {
	int physicalMemCounter = 0;
	int pageTable[255];
	bool pageSet[255];
	int physicalMem[255*255];
	int address = 0;
	int total = 0;
	int tlbHit = 0;
	int tblCounter = 0;
	int totalPageFaults = 0;
	char fromMem[256] = "";
	bool tblFound = false;

	int offset;
	int first16bit;
	int pagenumb;

	int correlatedFrame;

	FILE* inFile = fopen(argv[1], "r");
	FILE* backingStore = fopen("BACKING_STORE.bin", "rb");
	List* TBL = initializeList(&printNode, &deleteListNode, &compareNode);

	if (!inFile) {
		fprintf(stderr, "usage: %s filenameoflogicaladdresses\n", argv[0]);
 		exit(EXIT_FAILURE);
	} else if (!backingStore) {
		fprintf(stderr, "usage: %s filenameofbackingstore\n", argv[0]);
 		exit(EXIT_FAILURE);
	}

	while(fscanf(inFile,"%d", &address)>0) {
		total++;

		offset = address & 0xFF;
		first16bit = address & 0xFF00;
		pagenumb = first16bit>>8;
		tblFound = false;

		if (pageSet[pagenumb] == true) {
			Node* currentNode = TBL->head;
			while(currentNode != NULL) {
				if (((TLBentry*)(currentNode->data))->page == pagenumb) {
					correlatedFrame = ((TLBentry*)(currentNode->data))->frame;
					printf("Virtual address: %d Physical address: %d Value: %d\n", address, correlatedFrame*256+offset, physicalMem[correlatedFrame*256+offset]);
					tblFound = true;
					tlbHit++;
					break;
				}
				currentNode = currentNode->next;
			}
		}

		if (tblFound == true) {
			continue;
		}

		if (pageSet[pagenumb] == false) {
			pageSet[pagenumb] = true;
			totalPageFaults++;
			pageTable[pagenumb] = physicalMemCounter;
			fseek(backingStore, pagenumb*256, SEEK_SET);
			fread(&fromMem, sizeof(fromMem), 1, backingStore);
			for (int i = 0; i < 256; ++i) {
				physicalMem[physicalMemCounter*256 + i] = fromMem[i];
			}
			physicalMemCounter++;
		}

		if (tblCounter == 16) {
			deleteNodeFromList(TBL, getFromFront(TBL));
			tblCounter--;
		}

		TLBentry* insertEntry = malloc(sizeof(TLBentry));
		insertEntry->page = pagenumb;
		insertEntry->frame = pageTable[pagenumb];
		insertSorted(TBL, insertEntry, 0);
		tblCounter++;

		printf("Virtual address: %d Physical address: %d Value: %d\n", address, pageTable[pagenumb]*256+offset, physicalMem[pageTable[pagenumb]*256+offset]);
	}

	printf("Number of Translated Addresses = %d\n", total);
	printf("Page Faults = %d\n", totalPageFaults);
	printf("Page Fault Rate = %.3f\n", (double)totalPageFaults/(double)total);
	printf("TLB Hits = %d\n", tlbHit);
	printf("TLB Hit Rate = %.3f\n", (double)tlbHit/(double)total);

	fclose(inFile);
	fclose(backingStore);
	deleteList(TBL);
}
