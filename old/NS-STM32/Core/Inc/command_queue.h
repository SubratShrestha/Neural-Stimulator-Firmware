#ifndef command_queue_h
#define command_queue_h
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

typedef struct Node {
    char command[64];
    struct Node * Next;
}NODE, *PNODE;


typedef struct QNode {
    PNODE Front, Rear;
} Queue, *PQueue;

void InitQueue(PQueue queue);
uint8_t IsEmptyQueue(PQueue queue);
void EnQueue(PQueue queue, uint8_t *commands,uint8_t commands_len);
void DeQueue(PQueue queue,uint8_t *commands);
void DestroyQueue(PQueue queue);


#endif
