#include <sys/time.h>
#include <stdio.h>
#include <pthread.h> 
#include <errno.h>

 
 
main()  
{
  struct timeval start,end;
  long forktime;
  double avgtime;
  pthread_t last_thread; 
  int i;
  int iters = 250;
  void *null_proc();
 
  gettimeofday(&start,NULL);

  
  for (i = 0; i < iters - 1; i++)  /* create (iters-1) threads */ 
    pthread_create(&last_thread,NULL,null_proc,NULL);
      
  pthread_create(&last_thread,NULL,null_proc,NULL); /* create last thread */
  
 gettimeofday(&end,NULL);

  pthread_join(last_thread, NULL);  /* wait for last thread */
  forktime = (end.tv_sec - start.tv_sec)*1000000 + 
		(end.tv_usec - start.tv_usec);
  avgtime = (double)forktime/(double)iters;
  printf("Avg thr_create time = %f microsec\n", avgtime);

}
 
 
void *null_proc()
{
}

