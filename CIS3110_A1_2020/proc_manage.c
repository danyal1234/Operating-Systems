#include <stdio.h>
#include <unistd.h> 

int main()
{
    int pid;
    int i;
    printf("hello\n");
    fprintf(stderr, "This is the process with the process ID of %d\n", getpid());

    for(i=0;i<2;i++) {
    	pid = fork();

	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed");
		return 1;
	}

    	if(pid == 0) { /* child process */
		printf("hello\n");
		fprintf(stderr, "This is the process with the process ID of %d\n", getpid());
   	} else { /* parent process */
		printf("hello\n");
		fprintf(stderr, "This is the process with the process ID of %d\n", getpid());
	}
    }
    printf("hello\n");
    fprintf(stderr, "This is the process with the process ID of %d\n", getpid());

    return 0;
}
