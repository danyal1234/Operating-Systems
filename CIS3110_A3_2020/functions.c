/*
functions.c
Student Name : Danyal Mahmood
Student ID # : 0956989
*/
#include <stdio.h>

void printStatistics(int total, int totalPageFaults, int tlbHit) {
	// print statistics
	printf("Number of Translated Addresses = %d\n", total);
	printf("Page Faults = %d\n", totalPageFaults);
	printf("Page Fault Rate = %.3f\n", (double)totalPageFaults/(double)total);
	printf("TLB Hits = %d\n", tlbHit);
	printf("TLB Hit Rate = %.3f\n", (double)tlbHit/(double)total);
}