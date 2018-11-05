#ifndef QUEUE_H
#define QUEUE_H
#include "pool.h"

#define QUEUE_EMPTY 20
#define QUEUE_FULL 30
typedef int (*funcPtr) (int x);

struct Queue {
        funcPtr fp[MAXTHREADS];
        int front,rear;
};

int enqueue(struct Queue * q, funcPtr fp);
funcPtr dequeue(struct Queue * q);
int isEmpty(struct Queue * q);



#endif
