#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 1024 
#define PAGE_NUM 256
#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define FRAME_NUM 256
#define TLB_SIZE 16

int find_TLB(int num,int (*tlb)[2])
{
    int i;
    int position = -1;
    for(i=0; i < TLB_SIZE; i++)
    {
        if(tlb[i][0] == num)
        {
            position = i;
            break;
        }
    }
    return position;
}
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
    int count = 0;
    int virtual_address;
    int physical_address;
    int pos = 0;
    int page_pos;
    int page_offset;
    int frame_pos;
    int frame_offset;
    int Hit_count = 0;
    int TLB_pos = 0;
    int TLB[TLB_SIZE][2];
    int empty = FRAME_SIZE;
    int full = 0;
    for(j=0;j<2;j++) for(i=0;i<TLB_SIZE;i++) TLB[i][j] = -1;
    while(fgets(buf,MAX_LINE,add) != NULL)
    {
        // printf("%s",buf);
        virtual_address = atoi(buf);
        // printf("%d\n",virtual_address);
        page_pos = (virtual_address & 0xFF00) >> 8;
        page_offset = virtual_address & 255;
        frame_pos = find_TLB(page_pos,TLB);
        if (frame_pos == -1)
        {
            // printf("not find in TLB\n");
            TLB[TLB_pos][0] = page_pos;
            if(frame_table[page_pos] == -1)
            {
                if(empty > 0)
                {
                    frame_pos = frame_table[page_pos] = full;
                    page_table[full] = page_pos;
                    fseek(value,page_pos*256,SEEK_SET);
                    fread(mem[full],FRAME_SIZE,1,value);
                    full += 1;
                    empty -= 1;
                }
            }
            else
            {
                frame_pos = frame_table[page_pos];
            }
            TLB[TLB_pos][1] = frame_pos;
            TLB_pos = (TLB_pos+1)%16;
        }
        else 
        {
            Hit_count += 1;
            frame_pos = TLB[frame_pos][1];
        }       
        // printf("%d %d\n",page_pos,page_table[1]);        
        physical_address = frame_pos * 256 + page_offset;
        // printf("%d %d\n",physical_address,mem[frame_pos][page_offset]);
        printf("%d\n",mem[frame_pos][page_offset]);
        count += 1;
        // if(count > 10) break;
    }
    printf("Page Faults = %d\n",full);
    printf("TLB Hits = %d\n",Hit_count);
    return 0;
}