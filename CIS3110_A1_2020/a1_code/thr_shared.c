#include <sys/time.h>
#include <stdio.h>
#include <pthread.h> 
#include <unistd.h>
#include <stdlib.h>


void *proc();

int shared_number;


main()  
{
  int i;
  pthread_t new_thread;
  int sleep_time;
  int seed;
  
  shared_number = 1;
  
  printf("Enter a positive integer for seed: ");
  scanf("%d",&seed);
  srand48(seed);             /* initialize seed of random number stream */
  
  /*  thr_create(NULL,0,proc,NULL,0,&new_thread);*/
  pthread_create(&new_thread,NULL,proc,NULL);
  /* create new thread */
  
  for (i = 0; i < 50; i++)
    { 
      printf("MAIN THREAD: i = %d, shared_number = %d\n",i,shared_number);
      sleep_time = 100000.0*drand48();        /* generate random sleep time */
      printf("sleep time = %d microseconds\n",sleep_time);
      usleep(sleep_time);
      shared_number = shared_number + 2;
    }
  
  pthread_join(new_thread,NULL);
  
  printf("MAIN THREAD: DONE\n");
}
 
 
void *proc()
{
  int i = 0;
  int DONE;
  
  DONE = 0;
  while (!DONE)
    {
      i++;
      if (i%10000 == 0)
	printf("CHILD THREAD: i = %d,shared_number = %d\n",i,shared_number);
      if (i > 5000000)
        DONE = 1;
    }
  printf("CHILD THREAD: DONE\n");
  
}


