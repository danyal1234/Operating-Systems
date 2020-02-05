#include <sys/time.h>
#include <stdio.h>
#include <pthread.h> 
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>


void *proc();

int shared_number;

 
main()  
{
  int i;
  pthread_t new_thread;
  int sleep_time;
  int pid;
  int status;
  int seed;
  
  shared_number = 1;
  
  printf("Enter a positive integer for seed: ");
  scanf("%d",&seed);
  
  
  srand48(seed);                /* initialize random number stream */
  
  if ((pid = fork()) == 0)  
    {                                   /* child process */
      proc(); 
      exit(0);
    }
  else if (pid == -1)                   /* error        */
    { 
      printf("error %d\n",errno); 
      exit(-1); 
    }
  else 
    { /* parent process */
      
      for (i = 0; i < 50; i++)
	{ 
	  printf("MAIN PROCESS: i = %d, shared_number = %d\n",i,shared_number);
	  sleep_time = 100000.0*drand48(); /* generate random sleep time */
	  printf("sleep time = %d microseconds\n",sleep_time);
	  usleep(sleep_time);
	  shared_number = shared_number + 2;
	}
      
      wait(&status);           /* wait for child process */
      
      printf("MAIN PROCESS: DONE\n");
    }
}
 
void *proc()
{
  int i;
  int DONE;
  
  DONE = 0;
  i = 0;
  while (!DONE)
    {
      i++; 
      if (i%10000 == 0)
	printf("CHILD PROCESS: i = %d,shared_number = %d\n",i,shared_number);
      if (i > 5000000)
	DONE = 1;
    }
  printf("CHILD PROCESS: DONE\n");
  
}

