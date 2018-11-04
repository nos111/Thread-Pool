#ifndef QUEUE_H
#define QUEUE_H

#include "pool.h"

struct Queue {
        int queueArray[MAXTHREADS];
        int front,rear;
};

bool add(struct Queue * q);
bool extract(struct Queue * q);


#endif
