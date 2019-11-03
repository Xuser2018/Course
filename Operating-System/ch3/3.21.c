/**
 * Figure 3.33
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int  collatz(int n )
{
    if(n==1 || n==0)
        return n;
    printf("%d,",n);
    if(n%2 == 0)
        collatz(n/2);
    else
        collatz(3*n +1) ; 
}

int main()
{
pid_t pid;
	/* fork a child process */
    int n;
    printf("please input an interger:\n");
    scanf("%d",&n);
	pid = fork();

	if (pid < 0) { /* error occurred */ 
		fprintf(stderr, "Fork Failed"); 
		return 1;
	}
	else if (pid == 0) { /* child process */
		// printf("start\n");
		// int a =23333;
		// printf("%d",a);
		// execlp("/bin/ls","ls",NULL);
		// printf("LINE J\n");
        printf("%d",collatz(n));
        n=100;

	}
	else { /* parent process */
		/* parent will wait for the child to complete */
		wait(NULL);
		printf("Child Complete",n);
	}

	return 0;
}
