#include <stdio.h>
#include "schedulers.h"
#include "task.h"
#include "list.h"
struct node *head = NULL;
struct node *tail= NULL;
void add(char *name, int priority, int burst)
{
    Task *new_node = (Task*)malloc(sizeof(Task));
    new_node->burst = burst;
    new_node->name = name;
    new_node->priority = priority;
    tail_add(&tail,&head,new_node);
}
void schedule()
{
    traverse(head);
}