#ifndef POOL_H
#define POOL_H
#include <pthread.h>

#define MAXTHREADS 20

void initializeThreads(pthread_t * tids, int threadCount);
void * thread(void * arg);
void Pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
void killThreads(pthread_t * tids, int threadCount);

#endif