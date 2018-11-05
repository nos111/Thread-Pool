#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include "pool.h"
#include "queue.h"

typedef void handler_t(int);
#define MALLOCERROR 200

int initializeThreads(pthread_t * tids, int threadCount);
int thread(void * arg);
int killThreads(pthread_t * tids, int threadCount);
handler_t *Signal(int signum, handler_t *handler);
void sigint_handler(int sig);


static pthread_mutex_t queueLock;                         
static sem_t threadRunLock;
static struct Queue * q;
static pthread_t * tids;
static int loopThreadBool = 1;                         
static int threadCount = MAXTHREADS;


int initialize(int tCount) {
        int errorCode = 0;                             
        Signal(SIGINT,  sigint_handler);   /* ctrl-c */
        Signal(SIGTSTP, sigint_handler);  /* ctrl-z */
        q = malloc(sizeof(struct Queue));
        if(q == NULL) return MALLOCERROR;
        q->front = q->rear = 0;

        errorCode = pthread_mutex_init(&queueLock, NULL);
        if(errorCode != 0) return errorCode;
        errorCode = sem_init(&threadRunLock, 0, 0);
        if(errorCode != 0) return errorCode;

        tids = malloc(sizeof(pthread_t) * tCount); 
        if(tids == NULL) return MALLOCERROR;
        threadCount = tCount;
        errorCode = initializeThreads(tids, threadCount);
        if(errorCode != 0) return errorCode;
        loopThreadBool = 1;  
        return errorCode;
}


int initializeThreads(pthread_t * tids, int threadCount) {
        int * ptr;
        int returnValue = 0;
        for(int i = 0; i < threadCount; i++) {
                ptr = (int*)malloc(sizeof(int));
                if(ptr == NULL) return MALLOCERROR;
                *ptr = i;
                returnValue = pthread_create(&tids[*ptr], NULL, &thread, ptr);
                if(returnValue != 0) return returnValue;
        }
        return returnValue;
}

int killThreads(pthread_t * tids, int threadCount) {
        int returnValue = 0;
        loopThreadBool = 0;
        for(int i = 0; i < threadCount; i++) {
                if((returnValue = pthread_cancel(tids[i])) != 0) {
                        return returnValue;
                }
        }
        return returnValue;
}

int thread(void * arg) {
        funcPtr ptr;
        int returnValue = 0;
        int myId = *((int*)(arg));
        free(arg);
        printf("hello from thread %d \n", myId);
        while(loopThreadBool) {
                if((returnValue = sem_wait(&threadRunLock)) != 0) {
                        return returnValue;
                }
                printf("Thread %d acquired control \n", myId);
                if((returnValue = pthread_mutex_lock(&queueLock)) != 0) {
                        return returnValue;
                }
                printf("Thread %d gained access to queue \n", myId);
                if((ptr = dequeue(q)) == NULL) {
                        return QUEUE_EMPTY;
                }
                if((returnValue = pthread_mutex_unlock(&queueLock)) != 0) {
                        return returnValue;
                }
                printf("calc result from thread %d is %d \n", myId, ptr(myId));
        }
        return returnValue;

}


/**************** Jobs Routines ****************/

int addJob(funcPtr fp) {
        int returnValue = 0;
        if((returnValue = pthread_mutex_lock(&queueLock)) != 0) {
                return returnValue;
        }
        if((returnValue = enqueue(q, fp)) != 0) {
                return returnValue;
        }
        if((returnValue = pthread_mutex_unlock(&queueLock)) != 0) {
                return returnValue;
        }
        if((returnValue =sem_post(&threadRunLock)) != 0) {
                return returnValue;
        }
        return returnValue;
}


/**************** Signals handling for a clean termination  ****************/

handler_t *Signal(int signum, handler_t *handler) 
{
        struct sigaction action, old_action;

        action.sa_handler = handler;  
        sigemptyset(&action.sa_mask); /* block sigs of type being handled */
        action.sa_flags = SA_RESTART; /* restart syscalls if possible */
        int errorCode;
        if ((errorCode = sigaction(signum, &action, &old_action) < 0))
                perror("Signal error");
        return (old_action.sa_handler);
}

void sigint_handler(int sig) {
        killThreads(tids, threadCount);
        free(q);
        free(tids);
        exit(0);
}