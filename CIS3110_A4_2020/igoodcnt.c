#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

// global variables
int NITER;
int cnt = 0;
sem_t mutex;

void * Count(void * a)
{
    int i, tmp;
    for(i = 0; i < NITER; i++)
    {
    	// wait for access to resource cnt by obtainting semaphore
		sem_wait (&mutex);
        tmp = cnt;      /* copy the global cnt locally */
        tmp = tmp+1;    /* increment the local copy */
        cnt = tmp;      /* store the local value into the global cnt */ 
		sem_post (&mutex); 
		// releast semaphore when done with resource cnt
    }
}

int main(int argc, char * argv[])
{
	pthread_t tid1, tid2;

	// validate arguments
	// implementation

	// Parsing the arguments passed to your C program
	// Including the number of times that each thread increments
	// the shared count cnt
	// For example, NITER = 20000;

	// error check for bad arguments
	if (argv[1] == NULL) {
		fprintf(stderr, "usage: %s nonumberofiterationsprovided\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if (argc != 2) {
		fprintf(stderr, "usage: %s wrongnumberofarguments\n", argv[0]);
 		exit(EXIT_FAILURE);
	}

	// create semaphore
	sem_init(&mutex, 0, 1);
	// determine user entered number of iterations
	NITER = atoi(argv[1]);

	// check if negative number of counters
	if (NITER < 0) {
		fprintf(stderr, "usage: %s negativenumberofiterations\n", argv[0]);
 		exit(EXIT_FAILURE);
	}

	// Display the number of times (entered by the user) that each thread
	// increments the shared count cnt
// PLEASE DO NOT remove or modify the following code 
	printf("2*NITER is [%d]\n", 2*NITER);
// End of code section 

	// creating Thread 1
	if(pthread_create(&tid1, NULL, Count, NULL))
	{
		fprintf(stderr, "usage: %s failurecreatethread1\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// creating Thread 2
	if(pthread_create(&tid2, NULL, Count, NULL))
	{
		fprintf(stderr, "usage: %s failurecreatethread2\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if(pthread_join(tid1, NULL))	/* wait for the thread 1 to finish */
	{
		fprintf(stderr, "usage: %s failurejointhread1\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if(pthread_join(tid2, NULL))        /* wait for the thread 2 to finish */
	{
		fprintf(stderr, "usage: %s failurejointhread2\n", argv[0]);
		exit(EXIT_FAILURE);
	}


        // Display the value of count cnt
// PLEASE DO NOT remove or modify the following code
	printf("\nCounter is [%d]\n", cnt);
	if (cnt == 2 * NITER) 
		printf("\nPASS\n");
	else
		printf("\nFAIL\n");
// End of code section

	// free semaphore
	sem_destroy(&mutex);

	pthread_exit(NULL);
 	exit(EXIT_SUCCESS);
}

