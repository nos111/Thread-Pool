#ifndef QUEUE_H
#define QUEUE_H

#include "pool.h"

typedef int (*funcPtr) (int x);

struct Queue {
        funcPtr fp[MAXTHREADS];
        int front,rear;
};

void enqueue(struct Queue * q, funcPtr fp);
funcPtr dequeue(struct Queue * q);
int isEmpty(struct Queue * q);



#endif
