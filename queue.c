#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int enqueue(struct Queue * q, funcPtr fp) {
        int index = q->rear + 1;
        if(index >= MAXTHREADS) {
                index = 0;
        }
        if(index == q->front) {
                return QUEUE_FULL;
        }
        q->rear = q->rear + 1;
        q->fp[index] = fp;
        return 0;
}

funcPtr dequeue(struct Queue * q) {
        int index = q->front + 1;
        if(index >= MAXTHREADS) {
                index = 0;
        }
        if(index == q->rear + 1) {
                return NULL;
        }
        q->front = index;
        return q->fp[index];
}

int isEmpty(struct Queue * q) {
        return q->front == q->rear ? 1 : 0;
}