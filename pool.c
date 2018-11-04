#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include "pool.h"
#include "queue.h"

typedef void handler_t(int);
void initializeThreads(pthread_t * tids, int threadCount);
void thread(void * arg);
void Pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
void killThreads(pthread_t * tids, int threadCount);
void Sem_init(sem_t * sem, int pshared, unsigned int value);
void V(sem_t * s);
void P(sem_t * s);
handler_t *Signal(int signum, handler_t *handler);
void sigint_handler(int sig);

/* 
        - Add job to queue
        - Each thread will try to extract a job once the semaphore is increased
        - Only the thread that locks the mutex can access the jobs queue
        - Loop while you can
*/

/**************** Global variables ****************/
sem_t mutex;
sem_t semaphore;
struct Queue * q;
pthread_t * tids;
int work = 1;
int threadCount = MAXTHREADS;

void initialize(int tCount) {
        work = 1;
        Signal(SIGINT,  sigint_handler);   /* ctrl-c */
        Signal(SIGTSTP, sigint_handler);  /* ctrl-z */
        q = (struct Queue *)malloc(sizeof(struct Queue));
        q->front = q->rear = 0;
        Sem_init(&mutex, 0, 1);
        Sem_init(&semaphore, 0, 0);
        tids = (pthread_t *)malloc(sizeof(pthread_t) * tCount); 
        threadCount = tCount;
        initializeThreads(tids, threadCount);
}

/**************** Threads Routines ****************/

void initializeThreads(pthread_t * tids, int threadCount) {
        int * ptr;
        for(int i = 0; i < threadCount; i++) {
                ptr = (int*)malloc(sizeof(int));
                *ptr = i;
                Pthread_create(&tids[*ptr], NULL, &thread, ptr);
        }
}

void killThreads(pthread_t * tids, int threadCount) {
        work = 0;
        for(int i = 0; i < threadCount; i++) {
                pthread_join(tids[i], NULL);
        }
}

void Pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg) {
        if(pthread_create(thread, attr, start_routine, arg) != 0) {
                perror("error creating thread ");
        }
}

void thread(void * arg) {
        int myId = *((int*)(arg));
        free(arg);
        printf("hello from thread %d \n", myId);
        while(work) {
                P(&semaphore);
                printf("Thread %d acquired control \n", myId);
                P(&mutex);
                printf("Thread %d gained access to queue \n", myId);
                funcPtr ptr = dequeue(q);
                V(&mutex);
                printf("calc result from thread %d is %d \n", myId, ptr(myId));

        }

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


/**************** Jobs Routines ****************/

void addJob(funcPtr fp) {
        P(&mutex);
        enqueue(q, fp);
        V(&mutex);
        V(&semaphore);
}


/**************** Signals handling for a clean termination  ****************/

handler_t *Signal(int signum, handler_t *handler) 
{
    struct sigaction action, old_action;

    action.sa_handler = handler;  
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
	perror("Signal error");
    return (old_action.sa_handler);
}

void sigint_handler(int sig) {
        killThreads(tids, threadCount);
        free(q);
        free(tids);
        exit(0);
}