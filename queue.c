#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void enqueue(struct Queue * q, funcPtr fp) {
        printf("adding to queue \n");
        int index = q->rear + 1;
        if(index >= MAXTHREADS) {
                index = 0;
        }
        if(index == q->front) {
                printf("Queue is full \n");
                exit(0);
        }
        q->rear = q->rear + 1;
        q->fp[index] = fp;
}

funcPtr dequeue(struct Queue * q) {
        int index = q->front + 1;
        if(index >= MAXTHREADS) {
                index = 0;
        }
        if(index == q->rear + 1) {
                printf("Queue is empty \n");
                exit(0);
        }
        q->front = index;
        return q->fp[index];
}

int isEmpty(struct Queue * q) {
        return q->front == q->rear ? 1 : 0;
}