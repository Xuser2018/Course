/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the producer process that writes to the shared memory region.
 *
 * Figure 3.16
 * 
 * To compile, enter
 * 	gcc shm-posix-producer.c -lrt
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/syscall.h>  
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <semaphore.h>
typedef struct param
{
	int *ptr;
	int a;
	int count;
	int current;
	sem_t *sem;
	sem_t *empty;
	sem_t *full;
	pthread_mutex_t lock;
    pthread_cond_t cond;
}param;
void *runner(param *param);


int main(int argc, char **argv)
{
	sem_unlink("SEM");
	sem_unlink("empty");
	sem_unlink("full");
	sem_t *sem;
	sem_t *empty;
	sem_t *full;
	sem = sem_open("SEM", O_CREAT, 0666, 20);
	empty = sem_open("empty", O_CREAT, 0666, 20);
	full = sem_open("full", O_CREAT, 0666, 0);
	// char *a=empty;
	// int *b = empty;
	// printf("%d %p\n",*empty,empty);
	// printf("%d %p\n",*b,b);
	// printf("%d %d %d %d %p\n",*a,*(a+1),*(a+2),*(a+3),a);
	// printf("%d %d %d %d %p\n",*(a+4),*(a+5),*(a+6),*(a+7),a);
	// return 0;
	const int SIZE = sizeof(int) * 20;
	const char *name = "OS";
	// const char *message0= "Studying ";
	// const char *message1= "Operating Systems ";
	// const char *message2= "Is Fun!";

	int shm_fd;
	void *ptr;

	/* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd,SIZE);

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}
	param *par = (param*)malloc(sizeof(param));
	par->a = atoi(argv[1]);
	par->ptr = (int*) ptr;
	par->sem = sem;
	par->empty = empty;
	par->full = full;
	par->current = 0;
	par->count = 0;

	pthread_t tid; /* the thread identifier */
	pthread_t tid2; /* the thread identifier */
	pthread_t tid3; /* the thread identifier */

	pthread_attr_t attr; /* set of attributes for the thread */
	pthread_attr_t attr2; /* set of attributes for the thread */
	pthread_attr_t attr3; /* set of attributes for the thread */

	pthread_attr_init(&attr);
	pthread_attr_init(&attr2);
	pthread_attr_init(&attr3);

	pthread_create(&tid,&attr,(void *)runner,par);
	pthread_create(&tid2,&attr2,(void *)runner,par);
	pthread_create(&tid3,&attr3,(void *)runner,par);
	pthread_join(tid,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);
	printf("%d, %d, %d\n",*(par->ptr),*(par->ptr+1),*(par->ptr+2));
	/**
	 * Now write to the shared memory region.
 	 *
	 * Note we must increment the value of ptr after each write.
	 */
	// sprintf(ptr,"%s",message0);
	// ptr += strlen(message0);
	// sprintf(ptr,"%s",message1);
	// ptr += strlen(message1);
	// sprintf(ptr,"%s",message2);
	// ptr += strlen(message2);

	return 0;
}

void *runner(param *param)
{
	pid_t pid1 = (int)syscall(__NR_gettid);
	pid_t pid = getpid();
	double a;
	// int s_time;
	while(1)
	{
		do{
			a = ((double)rand()/RAND_MAX);
		}while(0);
		double b = param->a;
		usleep(1000000*(log(a/b)/-b));
		// usleep(1000000*(-param->a*log(a)));
		// a = (rand()%10)/10.0;
		// s_time = param->a * exp(param->a * a * -1) * 1000000;
		// printf("%d %d %lf %d\n",a,s_time,exp(param->a * a * -1),param->a * a * (-1)/10);
		// usleep(s_time);
		pthread_mutex_lock(&param->lock);
		sem_wait(param->empty);
		param->ptr[param->current] = rand();	
		param->count += 1;
		printf("[pid: %d] [tid: %d] [value: %15d] %d\n",pid,pid1,param->ptr[param->current],param->count);
		param->current += 1;
		if(param->current == 20)
			param->current = 0;
		sem_post(param->full);
		pthread_mutex_unlock(&param->lock);
	}
}