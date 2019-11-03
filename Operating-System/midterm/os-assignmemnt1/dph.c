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
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum {THIKING, HUNGRY, EATING} state[5];

typedef struct ShareInt
{
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int ident;
}Int;
Int condi[5];
void pickup(int i,Int *param);
void putdown(int i,Int *param);
void test(int i,Int *param);

int sum; /* this data is shared by the thread(s) */

int sudo[9][9];

typedef struct 
{
    /* data */
    int row_start;
    int col_start;
    int flags;
}parameters;

// void *runner(parameters *param); /* the thread */
void *runner(Int *param); /* the thread */
void *runner2(Int *param); /* the thread */
void *runner3(Int *param); /* the thread */
void *runner4(Int *param); /* the thread */
void *runner5(Int *param); /* the thread */
void state_printf()
{
    char *s[] = {"THIKING", "HUNGRY", "EATING"};
    printf("%s %s %s %s %s\n",s[state[0]],s[state[1]],s[state[2]],s[state[3]],s[state[4]]);
}
int main(int argc, char *argv[])
{
    memset((void *)sudo,0,sizeof(sudo));
    pthread_t tid; /* the thread identifier */
    pthread_t tid2; /* the thread identifier */
    pthread_t tid3; /* the thread identifier */
    pthread_t tid4; /* the thread identifier */
    pthread_t tid5; /* the thread identifier */
    pthread_attr_t attr; /* set of attributes for the thread */
    pthread_attr_t attr2; /* set of attributes for the thread */
    pthread_attr_t attr3; /* set of attributes for the thread */
    pthread_attr_t attr4; /* set of attributes for the thread */
    pthread_attr_t attr5; /* set of attributes for the thread */

    condi[0].ident = 0;
    condi[1].ident = 1;
    condi[2].ident = 2;
    condi[3].ident = 3;
    condi[4].ident = 4;

    /* parameters *data1 = (parameters *)malloc(sizeof(parameters));
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
    data3->row_start = 0;*/



    /* get the default attributes */
    pthread_attr_init(&attr);
    pthread_attr_init(&attr2);
    pthread_attr_init(&attr3);
    pthread_attr_init(&attr4);
    pthread_attr_init(&attr5);
    // sudo[0][0] =1;
    // sudo[1][2]=1;
    /* create the thread */
    pthread_create(&tid,&attr,(void *)runner,&condi[0]);
    pthread_create(&tid2,&attr2,(void *)runner,&condi[1]);
    pthread_create(&tid3,&attr3,(void *)runner,&condi[2]);
    pthread_create(&tid4,&attr4,(void *)runner,&condi[3]);
    pthread_create(&tid5,&attr5,(void *)runner,&condi[4]);

    /* now wait for the thread to exit */
    pthread_join(tid3,NULL);

    printf("sum = %d\n",sum);
}

void *runner(Int *param)
{
    while(1)
    {
        int a = rand() % 2000000 + 1000000;
        // printf("%d\n",a);
        usleep(a);
        pickup(param->ident,param);
        a = rand() % 2000000 + 1000000;
        usleep(a);
        putdown(param->ident,param);
    }
}

// void *runner2(Int *param)
// {
//     while(1)
//     {
//         int a = rand() % 2000000 + 1000000;
//         // printf("%d\n",a);
//         usleep(a);
//         pickup(param->ident,param);
//         a = rand() % 2000000 + 1000000;
//         usleep(a);
//         putdown(param->ident,param);
//     }
// }

// void *runner3(Int *param)
// {
//     while(1)
//     {
//         int a = rand() % 2000000 + 1000000;
//         // printf("%d\n",a);
//         usleep(a);
//         pickup(param->ident,param);
//         a = rand() % 2000000 + 1000000;
//         usleep(a);
//         putdown(param->ident,param);
//     }
// }

// void *runner4(Int *param)
// {
//     while(1)
//     {
//         int a = rand() % 2000000 + 1000000;
//         // printf("%d\n",a);
//         usleep(a);
//         pickup(param->ident,param);
//         a = rand() % 2000000 + 1000000;
//         usleep(a);
//         putdown(param->ident,param);
//     }
// }

// void *runner5(Int *param)
// {
//     while(1)
//     {
//         int a = rand() % 2000000 + 1000000;
//         // printf("%d\n",a);
//         usleep(a);
//         pickup(param->ident,param);
//         a = rand() % 2000000 + 1000000;
//         usleep(a);
//         putdown(param->ident,param);
//     }
// }

void pickup(int i,Int *param)
{
    state[i] = HUNGRY;
    state_printf();
    test(i,param);
    state[i] = EATING;
    state_printf();
}

void putdown(int i,Int *param)
{
    state[i] = THIKING;
    state_printf();
    // pthread_cond_signal(&condi[(i+4)%5].cond);
    // pthread_cond_signal(&condi[(i+4)%5].cond);
    pthread_cond_signal(&condi[i].cond);
}

void test(int i,Int *param)
{
    // if((state[(i+4)%5] != EATING) && (state[i] == HUNGRY) && state[(i+1)%5] != EATING)
    // {
    //     state[i] = EATING;
    // }
    while(state[(i+4)%5] == EATING || state[(i+1)%5] == EATING)
    {
        int rv = pthread_mutex_lock(&condi[(i+4)%5].lock);
        if(rv != 0)
            continue;
        if(state[(i+4)%5] == EATING)
        {
            pthread_cond_wait(&condi[(i+4)%5].cond,&condi[(i+4)%5].lock);
        }
        pthread_mutex_unlock(&condi[(i+4)%5].lock);
        int rv2 = pthread_mutex_lock(&condi[(i+1)%5].lock);
        if(rv2 != 0)
            continue;
        if(state[(i+1)%5] == EATING)
        {
            pthread_cond_wait(&condi[(i+1)%5].cond,&condi[(i+1)%5].lock);
        }
        pthread_mutex_unlock(&condi[(i+1)%5].lock);
    }
}



/**
 * The thread will begin control in this function
 */
// void *runner(parameters *param) 
// {
// // int i, upper = atoi(param);
// // sum = 0;

// // 	if (upper > 0) {
// // 		for (i = 1; i <= upper; i++)
// // 			sum += i;
// // 	}

// // 	pthread_exit(0);
//     int flag[9];
//     if(param->flags == 0)
//     {
//         while(1)
//         {
//             int m,n;
//             for(m=0;m<9;m++)
//             {
//                 memset(flag,0,sizeof(flag));
//                 for(n=0;n<9;n++)
//                 {
//                     if(sudo[m][n]!=0)
//                     {
//                         if(flag[sudo[m][n]]==1)
//                             printf("row %d col %d is invalid\n",m,n);
//                         flag[sudo[m][n]] =1;
//                     }
//                 }
//             }
//         }
//     }

//     else if(param->flags == 1)
//     {
//         while(1)
//         {
//             int m,n;
//             for(m=0;m<9;m++)
//             {
//                 memset(flag,0,sizeof(flag));
//                 for(n=0;n<9;n++)
//                 {
//                     if(sudo[n][m]!=0)
//                     {
//                         if(flag[sudo[n][m]]==1)
//                             printf("row %d col %d is invalid\n",n,m);
//                         flag[sudo[n][m]] =1;
//                     }
//                 }
//             }
//         }
//     }

//     else
//     {
//         // printf("%d %d\n",param->row_start,param->col_start);
//         /* code */
//         while(1)
//         {
//             int m,n;
//             // int flag[9];
//             memset(flag,0,sizeof(flag));
//             // printf("%d %d\n",param->row_start,param->col_start);
//             for(m=param->row_start;m<param->row_start+3;m++)
//             {
//                 for(n=param->col_start;n<param->col_start+3;n++)
//                 {
//                     if(sudo[m][n]!=0)
//                     {
//                         if(flag[sudo[m][n]]==1)
//                             printf("row %d col %d is invalid\n",m,n);
//                         flag[sudo[m][n]] =1;
//                     }
//                 }

//             }
//         }
//     }
//     pthread_exit(0);
// }
