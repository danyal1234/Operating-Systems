#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// entries insde the TLB struct
typedef struct tlbentry {
    int page;
    int frame;
} TLBentry;
 
int main(int argc, char **argv) {
	int physicalMemCounter = 0;
	int pageTable[255];
	bool pageSet[255];
	int physicalMem[255*255];
	int address = 0;
	int total = 0;
	int tlbHit = 0;
	int tlbCounter = 0;
	int totalPageFaults = 0;
	char fromMem[256] = "";
	bool tlbFound = false;

	// create TLB
	TLBentry tlbEntries[16];

	// variables for bitmasking
	int offset;
	int first16bit;
	int pagenumb;

	int correlatedFrame;

	// open necessary files
	FILE* inFile = fopen(argv[1], "r");
	FILE* backingStore = fopen("BACKING_STORE.bin", "rb");

	// error check for bad arguments
	if (!inFile) {
		fprintf(stderr, "usage: %s filenameoflogicaladdresses\n", argv[0]);
 		exit(EXIT_FAILURE);
	} else if () {

	} else if (!backingStore) {
		fprintf(stderr, "usage: %s noinstanceofbackingstore\n", argv[0]);
 		exit(EXIT_FAILURE);
	}

	while(fscanf(inFile,"%d", &address)>0) {
		total++;

		// take first 8 bits of address
		offset = address & 0xFF;
		// take first 8 bits of the 16 bits of adress
		first16bit = address & 0xFF00;
		// shift right to remove 8 0's
		pagenumb = first16bit>>8;
		tlbFound = false;

		// check if page number exists in TLB
		if (pageSet[pagenumb] == true) {
			for (int i = 0; i < tlbCounter; ++i) {
				if (tlbEntries[i].page == pagenumb) {
					correlatedFrame = tlbEntries[i].frame;
					printf("Virtual address: %d Physical address: %d Value: %d\n", address, correlatedFrame*256+offset, physicalMem[correlatedFrame*256+offset]);
					tlbFound = true;
					tlbHit++;
					break;
				}
			}
		}

		// if TLB hit continue to next address
		if (tlbFound == true) {
			continue;
		}

		//if page fault then acccess backing store for 256 byte values
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

		// if TLB length is 16 remove first item in list
		if (tlbCounter == 16) {
			for (int i = 1; i < tlbCounter; ++i) {
				tlbEntries[i-1].page = tlbEntries[i].page;
				tlbEntries[i-1].frame = tlbEntries[i].frame;
			}
			tlbCounter--;
		}

		// insert recently accessed page number into TLB
		tlbEntries[tlbCounter].page = pagenumb;
		tlbEntries[tlbCounter].frame = pageTable[pagenumb];
		tlbCounter++;

		// if TLB hit use correlated frame and print value
		printf("Virtual address: %d Physical address: %d Value: %d\n", address, pageTable[pagenumb]*256+offset, physicalMem[pageTable[pagenumb]*256+offset]);
	}

	// print statistics
	printf("Number of Translated Addresses = %d\n", total);
	printf("Page Faults = %d\n", totalPageFaults);
	printf("Page Fault Rate = %.3f\n", (double)totalPageFaults/(double)total);
	printf("TLB Hits = %d\n", tlbHit);
	printf("TLB Hit Rate = %.3f\n", (double)tlbHit/(double)total);

	fclose(inFile);
	fclose(backingStore);
 	exit(EXIT_SUCCESS);
}
