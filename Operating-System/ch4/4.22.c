/**
 * Figure 3.33
 */
//complie gcc -o 3.22 3.22.c -lrt

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

int main()
{
    int num [7]= {90,81,78,95,79,72,85};
    pid_t pid;

	/* fork a child process */
	pid = fork();

	if (pid < 0) { /* error occurred */ 
		fprintf(stderr, "Fork Failed"); 
		return 1;
	}
	else if (pid == 0) { /* child process */
        // printf("2333%d \n",*len);
        pid_t pid_2;
        pid_2 = fork();
        if(pid_2<0)
        {
            fprintf(stderr, "Fork Failed"); 
		return 1;
        }
        else if(pid_2 == 0)
        {
            int j ,sum=0;
            for(j = 0;j<7;j++)
            {
                sum += num[j];
            }
            printf("The average value is %d\n",sum/7);
        }
        else
        {
             int j ,min=9999;
            for(j = 0;j<7;j++)
            {
                if(num[j]<min)
                    min = num[j];
            }            
            wait(NULL);
            printf("The minimum value is %d\n",min);
        }
        
	}
	else { /* parent process */
		/* parent will wait for the child to complete */
        int j ,max=-9999;
            for(j = 0;j<7;j++)
            {
                if(num[j]>max)
                    max = num[j];
            }            
            wait(NULL);
            printf("The minimum value is %d\n",max);
		// wait(NULL);
	}

	return 0;
}
