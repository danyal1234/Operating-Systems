/*
functions.h
Student Name : Danyal Mahmood
Student ID # : 0956989
*/

#include <stdio.h>

// entries insde the TLB struct
typedef struct tlbentry {
    int page;
    int frame;
} TLBentry;

// print stats function
void printStatistics(int total, int totalPageFaults, int tlbHit);
