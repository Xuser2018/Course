/**
 * Figure 3.33
 */

#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<sys/mman.h>
int  collatz(int n,int *start, int *num )
{
    if(n==1 || n==0)
    {   
		*start += 1;
	    *num = n; 
		return n;
	}
    // printf("%d,",n);
	*start += 1;
	*num = n;
    if(n%2 == 0)
        collatz(n/2,start,num+1);
    else
        collatz(3*n +1,start,num+1) ; 
}

int main()
{
pid_t pid;
const char *name = "OS";
const int SIZE = 4096;
int shm_fd;
void *ptr;
// shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
shm_fd = shm_open(name, O_RDONLY, 0666);
ftruncate(shm_fd,SIZE);
// ptr = mmap(0,SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
int *test = (int*)ptr;
// *test= 1000;
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
		int shm_fd2 = shm_open(name, O_CREAT | O_RDWR, 0666);
		// ptr = mmap(0,SIZE, PROT_READ , MAP_SHARED, shm_fd2, 0);
		ptr = mmap(0,SIZE, PROT_WRITE , MAP_SHARED, shm_fd2, 0);
		int *len = (int *)ptr;
		int *num = len+1;
		*len = 0;
		collatz(n,len,num);
        // printf("2333%d \n",*len);
	}
	else { /* parent process */
		/* parent will wait for the child to complete */
		wait(NULL);
		int i;
		int *num =test+1;
		for (i=0;i<*test-1;i++)
		{
			printf(" %d ",*num);
			num+=1;
		}
		printf(" %d",*num);
		printf("Child Complete%d",*test);
	}

	return 0;
}
