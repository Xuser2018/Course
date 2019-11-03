/**
 * A pthread program illustrating how to
 * create a simple thread and some of the pthread API
 * This program implements the summation function where
 * the summation operation is run as a separate thread.
 *
 * Most Unix/Linux/OS X users
 * gcc thrd.c -lpthread
 *
 * Figure 4.11
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sum; /* this data is shared by the thread(s) */

int sudo[9][9];

typedef struct 
{
    /* data */
    int row_start;
    int col_start;
    int flags;
}parameters;

void *runner(parameters *param); /* the thread */

int main(int argc, char *argv[])
{
memset(sudo,0,sizeof(sudo));

pthread_t tid; /* the thread identifier */
pthread_t tid2; /* the thread identifier */
pthread_t tid3; /* the thread identifier */
pthread_attr_t attr; /* set of attributes for the thread */
pthread_attr_t attr2; /* set of attributes for the thread */
pthread_attr_t attr3; /* set of attributes for the thread */

// if (argc != 2) {
// 	fprintf(stderr,"usage: a.out <integer value>\n");
// 	/*exit(1);*/
// 	return -1;
// }

// if (atoi(argv[1]) < 0) {
// 	fprintf(stderr,"Argument %d must be non-negative\n",atoi(argv[1]));
// 	/*exit(1);*/
// 	return -1;
// }
parameters *data1 = (parameters *)malloc(sizeof(parameters));
data1->flags = 0;
data1->col_start = 0;
data1->row_start = 0;

parameters *data2 = (parameters *)malloc(sizeof(parameters));
data2->flags = 1;
data2->col_start = 0;
data2->row_start = 0;

parameters *data3 = (parameters *)malloc(sizeof(parameters));
data3->flags = 2;
data3->col_start = 0;
data3->row_start = 0;

parameters *data4 = (parameters *)malloc(sizeof(parameters));
data4->flags = 2;
data4->col_start = 3;
data4->row_start = 0;

parameters *data5 = (parameters *)malloc(sizeof(parameters));
data5->flags = 2;
data5->col_start = 6;
data5->row_start = 0;

parameters *data6 = (parameters *)malloc(sizeof(parameters));
data6->flags = 2;
data6->col_start = 0;
data6->row_start = 3;

parameters *data7 = (parameters *)malloc(sizeof(parameters));
data7->flags = 2;
data7->col_start = 3;
data7->row_start = 3;

parameters *data8 = (parameters *)malloc(sizeof(parameters));
data8->flags = 2;
data8->col_start = 6;
data8->row_start = 3;

/* get the default attributes */
pthread_attr_init(&attr);
pthread_attr_init(&attr2);
pthread_attr_init(&attr3);
sudo[0][0] =1;
// sudo[1][2]=1;
/* create the thread */
pthread_create(&tid,&attr,runner,data1);
pthread_create(&tid2,&attr2,runner,data2);
pthread_create(&tid3,&attr3,runner,data3);

/* now wait for the thread to exit */
pthread_join(tid3,NULL);

printf("sum = %d\n",sum);
}

/**
 * The thread will begin control in this function
 */
void *runner(parameters *param) 
{
// int i, upper = atoi(param);
// sum = 0;

// 	if (upper > 0) {
// 		for (i = 1; i <= upper; i++)
// 			sum += i;
// 	}

// 	pthread_exit(0);
    int flag[9];
    if(param->flags == 0)
    {
        while(1)
        {
            int m,n;
            for(m=0;m<9;m++)
            {
                memset(flag,0,sizeof(flag));
                for(n=0;n<9;n++)
                {
                    if(sudo[m][n]!=0)
                    {
                        if(flag[sudo[m][n]]==1)
                            printf("row %d col %d is invalid\n",m,n);
                        flag[sudo[m][n]] =1;
                    }
                }
            }
        }
    }

    else if(param->flags == 1)
    {
        while(1)
        {
            int m,n;
            for(m=0;m<9;m++)
            {
                memset(flag,0,sizeof(flag));
                for(n=0;n<9;n++)
                {
                    if(sudo[n][m]!=0)
                    {
                        if(flag[sudo[n][m]]==1)
                            printf("row %d col %d is invalid\n",n,m);
                        flag[sudo[n][m]] =1;
                    }
                }
            }
        }
    }

    else
    {
        // printf("%d %d\n",param->row_start,param->col_start);
        /* code */
        while(1)
        {
            int m,n;
            // int flag[9];
            memset(flag,0,sizeof(flag));
            // printf("%d %d\n",param->row_start,param->col_start);
            for(m=param->row_start;m<param->row_start+3;m++)
            {
                for(n=param->col_start;n<param->col_start+3;n++)
                {
                    if(sudo[m][n]!=0)
                    {
                        if(flag[sudo[m][n]]==1)
                            printf("row %d col %d is invalid\n",m,n);
                        flag[sudo[m][n]] =1;
                    }
                }

            }
        }
    }
    pthread_exit(0);
}
