#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

 
main()  
{
  int pid;
  struct timeval start,end;
  int i;
  long forktime;
  double avgtime;
  int iters = 250;
  int status;

  gettimeofday(&start,NULL);
  
  for (i = 0; i < iters; i++)  /* create iters processes */
    {
      if ((pid = fork()) == 0) /* child process */
        {  
	  null_proc(); 
	  exit(1); 
	}
      else if (pid == -1)     /* error */
        { 
	  printf("error %d\n",errno); 
	  exit(-1); 
	}
      else                   /* parent process */
        continue;
    }
  
  /* only parent process reaches this point */
  gettimeofday(&end,NULL);
  for ( i = 0; i < iters; i++)  /* have to do a wait for each child */
  wait(&status);
  forktime = (end.tv_sec - start.tv_sec)*1000000 + 
		(end.tv_usec - start.tv_usec);
  avgtime = (double)forktime/(double)iters;
  printf("Avg fork time =%f microsec\n", avgtime);
  
}
 
 
null_proc()
{
}
 

