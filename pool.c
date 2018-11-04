#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "pool.h"
#include "queue.h"

/* 
        - Add job to queue
        - Each thread will try to extract a job once the semaphore is increased
        - Only the thread that locks the mutex can access the jobs queue
        - Loop while you can
*/

sem_t mutex;
sem_t semaphore;

int sum(int x) {
        printf("%d \n", x + 2);
        return x + 2;
}

int minus(int x) {
        printf("%d \n", x - 2);
        return x - 2;
}



int main(int argc, char ** argv) {

        if(argc != 2) {
                printf("Usage: <thread count> \n");
                return 0;
        }
        struct Queue * q = (struct Queue *)malloc(sizeof(struct Queue));
        q->front = q->rear = 0;
        Sem_init(&mutex, 0, 1);
        Sem_init(&semaphore, 0, 0);

        enqueue(q, sum);
        enqueue(q, minus);
        funcPtr mySum = dequeue(q);
        printf("using funcPtr %d \n", mySum(10));
        mySum = dequeue(q);
        printf("using funcPtr %d \n", mySum(10));
        pthread_t * tids = (pthread_t *)malloc(sizeof(pthread_t) * atoi(argv[1])); 
        initializeThreads(tids, atoi(argv[1]));
        killThreads(tids, atoi(argv[1]));

        printf("%s", argv[1]);
        return 0;
}

/**************** Threads Routines ****************/

void initializeThreads(pthread_t * tids, int threadCount) {
        int * ptr;
        for(int i = 0; i < threadCount; i++) {
                ptr = (int*)malloc(sizeof(int));
                *ptr = i;
                Pthread_create(&tids[*ptr], NULL, thread, ptr);
        }
}

void killThreads(pthread_t * tids, int threadCount) {
        for(int i = 0; i < threadCount; i++) {
                pthread_join(tids[i], NULL);
        }
}

void Pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg) {
        if(pthread_create(thread, attr, start_routine, arg) != 0) {
                printf("created thread");
                perror("error creating thread ");
        }
}

void * thread(void * arg) {
        int myId = *((int*)(arg));
        printf("hello from thread %d \n", myId);
        //while(1) {

        //}

}

/**************** Semaphores Routines ****************/


//if s is non zero, s is decremented and we return
//if s is zero, suspend thread until is is non zero
void P(sem_t * s) {
        if(sem_wait(s) != 0) {
                perror("error P ");
        }
}

//increase s by one and use sem post to notifiy any thread that is waiting for the semaphore to change
void V(sem_t * s) {
        if(sem_post(s) != 0) {
                perror("error V ");
        }
}

void Sem_init(sem_t * sem, int pshared, unsigned int value) {
        if(sem_init(sem, pshared, value) != 0) {
                perror("error sem_wait ");
        }
}
