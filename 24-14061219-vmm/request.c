#include <stdio.h>
#include <stdlib.h>
#include "vmm.h"
#define MANUAL

/* 访存请求 */
Ptr_MemoryAccessRequest ptr_memAccReq;
/* 产生访存请求 */
int main()
{

        /*产生请求地址 */
        FILE *fifo2;
        /* 访存请求 */
        Ptr_MemoryAccessRequest ptr_memAccReq;
#ifdef MANUAL
        unsigned long virAddr;
        int pid;
        char c;
        char man=' ';
    while(TRUE){
        printf("请输入进程号：\n");
        scanf("%d",&pid);
        ptr_memAccReq->pid=pid;
        printf("请输入请求地址: \n");
        scanf("%lu",&virAddr);
	ptr_memAccReq->virAddr = virAddr% VIRTUAL_MEMORY_SIZE;
        printf("请输入请求类型r？w？e？: \n");
        scanf(" %c",&man);
        getchar();
        switch (man)
	{
		case 'r': //读请求
		{
			ptr_memAccReq->reqType = REQUEST_READ;
			printf("产生请求：\n地址：%u\t类型：读取\n", ptr_memAccReq->virAddr);
			break;
		}
		case 'w': //写请求
		{
			ptr_memAccReq->reqType = REQUEST_WRITE;
			/* 产生待写入的值 */
                        printf("请输入写入的值\n");
                        scanf("%c",&ptr_memAccReq->value);
                        getchar();
			printf("产生请求：\n地址：%u\t类型：写入\t值：%02X\n", ptr_memAccReq->virAddr, ptr_memAccReq->value);
			break;
		}
		case 'e':
		{
			ptr_memAccReq->reqType = REQUEST_EXECUTE;
			printf("产生请求：\n地址：%u\t类型：执行\n", ptr_memAccReq->virAddr);
			break;
		}
		default:
			break;
	}
#else
	ptr_memAccReq->virAddr = random() % VIRTUAL_MEMORY_SIZE;
        
	/* 随机产生请求类型 */
	switch (random() % 3)
	{
		case 0: //读请求
		{
			ptr_memAccReq->reqType = REQUEST_READ;
			printf("产生请求：\n地址：%u\t类型：读取\n", ptr_memAccReq->virAddr);
			break;
		}
		case 1: //写请求
		{
			ptr_memAccReq->reqType = REQUEST_WRITE;
			/* 随机产生待写入的值 */
			ptr_memAccReq->value = random() % 0xFFu;
			printf("产生请求：\n地址：%u\t类型：写入\t值：%02X\n", ptr_memAccReq->virAddr, ptr_memAccReq->value);
			break;
		}
		case 2:
		{
			ptr_memAccReq->reqType = REQUEST_EXECUTE;
			printf("产生请求：\n地址：%u\t类型：执行\n", ptr_memAccReq->virAddr);
			break;
		}
		default:
			break;
	}
#endif
        fifo2=fopen(FIFO,"a+");
        if(fifo2<0)
	        printf("open fifo2 failed");
	if(fwrite(ptr_memAccReq,sizeof(MemoryAccessRequest),1,fifo2)<0)
                printf("do_request write failed");
        fclose(fifo2);
        printf("按X退出程序，按其他键继续...\n");
                        c = getchar();
		        if (c == 'x' || c == 'X')
			    break;
		        while (c != '\n')
			    c = getchar();
    }
}
