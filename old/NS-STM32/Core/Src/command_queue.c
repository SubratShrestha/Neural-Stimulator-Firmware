#include "command_queue.h"
#include "main.h"

void InitQueue(PQueue queue){
	queue->Front = queue->Rear = (PNODE)malloc(sizeof(NODE));
	if (queue->Front != NULL) {
		queue->Front->Next = NULL;
	}else{
		//should reboot here
		HAL_NVIC_SystemReset();
	}
}

uint8_t IsEmptyQueue(PQueue queue) {
    if (queue->Front == queue->Rear) {
        return 1;
    }
    else {
        return 0;
    }
}

void EnQueue(PQueue queue, uint8_t *commands,uint8_t commands_len) {
    PNODE P = (PNODE)calloc(1,sizeof(NODE));
    if (P != NULL) {
    	memcpy(P->command,commands,commands_len);
		P->Next = NULL;
		queue->Rear->Next = P;
		queue->Rear = P;
    }
}

void DeQueue(PQueue queue,uint8_t *commands) {
    if (IsEmptyQueue(queue)!=1) {
    	PNODE  P= queue->Front->Next;
    	memcpy(commands,P->command,5);
		queue->Front->Next = P->Next;
		if (queue->Rear==P)
			queue->Rear = queue->Front;
		free(P);
		P = NULL;
    }
}

void DestroyQueue(PQueue queue) {
    while (queue->Front != NULL) {
        queue->Rear = queue->Front->Next;
        free(queue->Front);
        queue->Front = queue->Rear;
    }
}
