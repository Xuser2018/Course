#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 1024 
#define PAGE_NUM 256
#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define FRAME_NUM 128
#define TLB_SIZE 16

typedef struct Double_Link_List
{
    int table[2];
    struct Double_Link_List *next;
    struct Double_Link_List *front;
}DL_List;

DL_List* DL_List_Init(int size);

DL_List* DL_List_find(int num,DL_List *head);

DL_List* DL_List_Update(DL_List *DL_List_head,DL_List *DL_List_pos);

DL_List* DL_List_Delete(DL_List *DL_List_head,DL_List *DL_List_pos);

void Frame_Init(DL_List *head);

int main()
{
    char buf[MAX_LINE];
    char mem[FRAME_NUM][FRAME_SIZE];
    int page_table[FRAME_NUM];
    int frame_table[PAGE_NUM];
    int empty_frame[FRAME_NUM];
    int i,j;
    for(i=0;i<FRAME_NUM;i++) page_table[i] = -1;
    for(i=0;i<FRAME_NUM;i++) empty_frame[i] = i;
    for(i=0;i<PAGE_NUM;i++) frame_table[i] = -1;
    FILE *add;
    FILE *value;
    int len;
    if((add = fopen("addresses.txt","r")) == NULL)
    {
        perror("fail to read addresses.txt");
        exit(1);
    }
    if((value = fopen("BACKING_STORE.bin","rb")) == NULL)
    {
        perror("fail to read BACKING_STORE.bin");
        exit(1);
    }
    DL_List *TLB_head = DL_List_Init(TLB_SIZE);
    DL_List *Frame_head = DL_List_Init(FRAME_NUM);
    Frame_Init(Frame_head);
    Frame_head = Frame_head->front;
    DL_List *TLB_pos,*Frame_List_pos;
    int count = 0;
    int virtual_address;
    int physical_address;
    int pos = 0;
    int page_pos;
    int page_offset;
    int frame_pos = 0;
    int frame_offset;
    int Hit_count = 0;
    int TLB[TLB_SIZE][2];
    int empty = FRAME_SIZE;
    int full = 0;
    for(j=0;j<2;j++) for(i=0;i<TLB_SIZE;i++) TLB[i][j] = -1;
    while(fgets(buf,MAX_LINE,add) != NULL)
    {
        virtual_address = atoi(buf);
        page_pos = (virtual_address & 0xFF00) >> 8;
        page_offset = virtual_address & 255;
        TLB_pos = DL_List_find(page_pos,TLB_head);
        if (TLB_pos == NULL)
        {
            TLB_head = TLB_head->next;
            TLB_head ->table[0] = page_pos;
            Frame_List_pos = DL_List_find(page_pos,Frame_head);
            if(Frame_List_pos == NULL)
            {
                Frame_head = Frame_head->next;
                frame_pos = Frame_head ->table[1];
                Frame_head ->table[0] = page_pos;
                Frame_head ->table[1] = frame_pos;
                fseek(value,page_pos*256,SEEK_SET);
                fread(mem[frame_pos],FRAME_SIZE,1,value);
                full += 1;
            }
            else
            {
                frame_pos = Frame_List_pos-> table[1];
                Frame_head =  DL_List_Update(Frame_head,Frame_List_pos);
            }
            TLB_head -> table[1] = frame_pos;
        }
        else 
        {
            Hit_count += 1;
            frame_pos = TLB_pos->table[1];
            TLB_head =  DL_List_Update(TLB_head,TLB_pos);
        } 
        // printf("%d\n",frame_pos);            
        physical_address = frame_pos * 256 + page_offset;
        printf("%d\n",mem[frame_pos][page_offset]);
        count += 1;
        // if(count > 288) break;
    }

    printf("Page Faults = %d\n",full);
    printf("TLB Hits = %d\n",Hit_count);
    return 0;
}

DL_List* DL_List_Init(int size)
{
    int i;
    DL_List *new_node,*cur_node, *head;
    new_node = (DL_List *)malloc(sizeof(DL_List));
    new_node ->table[0] = -4;
    new_node ->table[1] = -4;
    new_node ->front = new_node;
    new_node ->next = new_node;
    head = cur_node = new_node;
    for(i = 0;i < size -1;i++)
    {
        new_node = (DL_List *)malloc(sizeof(DL_List));
        new_node -> table[0] = -1;
        new_node -> table[1] = -1;
        cur_node -> next = new_node;
        new_node->front = cur_node;
        new_node->next = head;
        head ->front = new_node;
        cur_node = new_node;
    }
    return head;
}

DL_List* DL_List_find(int num,DL_List *head)
{
    DL_List *tail = head;
    while(head->next != tail)
    {
        if(head->table[0] == num)
            return head;
        head = head->next;
    }
    if(head->table[0] == num)
        return head;
    return NULL;
}

void Frame_Init(DL_List *head)
{
    DL_List *tail = head;
    int temp = 0;
    while(head->next != tail)
    {
        head->table[1] = temp;
        temp += 1;
        head = head->next;
    }
    head->table[1] == temp;
}

DL_List* DL_List_Update(DL_List *DL_List_head,DL_List *DL_List_pos)
{
    if(DL_List_pos!=DL_List_head)
    {
        DL_List_pos->front->next = DL_List_pos->next;
        DL_List_pos->next->front = DL_List_pos->front;
        DL_List_pos->next = DL_List_head->next;
        DL_List_pos->front = DL_List_head;
        DL_List_head->next->front = DL_List_pos;
        DL_List_head->next = DL_List_pos;
        DL_List_head = DL_List_pos;   
    }
    return DL_List_head;
}

DL_List* DL_List_Delete(DL_List *DL_List_head,DL_List *DL_List_pos)
{
    if(DL_List_pos!=DL_List_head)
    {
        DL_List_pos->table[0] = -1;
        DL_List_pos->front->next = DL_List_pos->next;
        DL_List_pos->next->front = DL_List_pos->front;
        DL_List_pos->next = DL_List_head->next;
        DL_List_pos->front = DL_List_head;
        DL_List_head->next->front = DL_List_pos;
        DL_List_head->next = DL_List_pos; 
    }
    else
    {
        DL_List_head->table[0] = -1;
        DL_List_head = DL_List_head->front;
    }
    return DL_List_head;
}