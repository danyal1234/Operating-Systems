#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "PriorityQueueAPI.h"
 
int main(int argc, char **argv) {

	if (argc != 2) {
		printf("File not specified. Exiting..\n");
		return -1;
	}

	int physicalMemCounter = 0;
	int pageTable[255];
	bool pageSet[255];
	int physicalMem[255*255];
	int address = 0;
	int total = 0;
	int totalPageFaults = 0;
	char fromMem[256] = "";

	FILE* inFile = fopen(argv[1], "r");
	FILE* backingStore = fopen("BACKING_STORE.bin", "rb");

	if (!inFile) {
		printf("Address file not found.\n");
		return -1;
	} else if (!backingStore) {
		printf("Backing store not found.\n");
		return -1;
	}

	while(fscanf(inFile,"%d", &address)>0) {
		total++;

		int offset = address & 0xFF;
		int first16bit = address & 0xFF00;
		int pagenumb = first16bit>>8;

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

		printf("Virtual address: %d Physical address: %d Value: %d\n", address, pageTable[pagenumb]*256+offset, physicalMem[pageTable[pagenumb]*256+offset]);
	}

	fclose(inFile);
	fclose(backingStore);

	printf("Number of Translated Addresses = %d\n", total);
	printf("Page Faults = %d\n", totalPageFaults);
	printf("Page Fault Rate = %.3f\n", (double)totalPageFaults/(double)total);
}
