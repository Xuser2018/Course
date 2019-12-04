#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 1024 
#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define FRAME_NUM 256
#define TLB_SIZE 16
int find_table(int num,unsigned int *page)
{
    int i;
    int position = -1;
    for(i=0; i < PAGE_SIZE; i++)
    {
        if(page[i] == num)
        {
            position = i;
            break;
        }
    }
    return position;
}
int find_TLB(int num,unsigned int *tlb)
{
    int i;
    int position = -1;
    for(i=0; i < TLB_SIZE; i++)
    {
        if(tlb[i] == num)
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
    int page_table[PAGE_SIZE];
    int i,j;
    for(i=0;i<PAGE_SIZE;i++) page_table[i] = -1;
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
    int TLB[2][TLB_SIZE];
    for(j=0;j<2;j++) for(i=0;i<TLB_SIZE;i++) TLB[j][i] = -1;
    while(fgets(buf,MAX_LINE,add) != NULL)
    {
        // printf("%s",buf);
        virtual_address = atoi(buf);
        // printf("%d\n",virtual_address);
        page_pos = (virtual_address & 0xFF00) >> 8;
        page_offset = virtual_address & 255;
        frame_pos = find_TLB(page_pos,TLB[0]);
        if (frame_pos == -1)
        {
            // printf("not find in TLB\n");
            TLB[0][TLB_pos] = page_pos;
            frame_pos = find_table(page_pos,page_table);
            if(frame_pos == -1)
            {
                page_table[pos] = page_pos;
                fseek(value,page_pos*256,SEEK_SET);
                fread(mem[pos],FRAME_SIZE,1,value);
                // printf("new frame\n");
                frame_pos = pos;
                pos += 1;
            }
            TLB[1][TLB_pos] = frame_pos;
            TLB_pos = (TLB_pos+1)%16;
        }
        else 
        {
            Hit_count += 1;
            frame_pos = TLB[1][frame_pos];
        }       
        // printf("%d %d\n",page_pos,page_table[1]);        
        physical_address = frame_pos * 256 + page_offset;
        // printf("%d %d\n",physical_address,mem[frame_pos][page_offset]);
        printf("%d\n",mem[frame_pos][page_offset]);
        count += 1;
        // if(count >106) break;
    }
    printf("Page Faults = %d\n",pos);
    printf("TLB Hits = %d\n",Hit_count);
    return 0;
}