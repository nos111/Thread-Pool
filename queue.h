#ifndef QUEUE_H
#define QUEUE_H

#include "pool.h"

typedef int (*funcPtr) (int x);

struct Queue {
        funcPtr fp[MAXTHREADS];
        int front,rear;
};

void add(struct Queue * q, funcPtr fp);
funcPtr extract(struct Queue * q);
int isEmpty(struct Queue * q);



#endif
