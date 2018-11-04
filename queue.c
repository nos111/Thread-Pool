#include <stdio.h>
#include "queue.h"

void add(struct Queue * q, funcPtr fp) {
        int index = q->rear + 1;
        if(index >= MAXTHREADS) {
                index = 0;
        }
        if(index == q->front) {
                printf("Queue is full");
                return;
        }
        q->fp[index] = fp;
}

funcPtr extract(struct Queue * q) {
        int index = q->front + 1;
        if(index >= MAXTHREADS) {
                index = 0;
        }
        if(index == q->rear) {
                printf("Queue is empty");
                return NULL;
        }
        return q->fp[index];
}

int isEmpty(struct Queue * q) {
        return q->front == q->rear ? 1 : 0;
}