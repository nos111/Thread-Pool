#ifndef POOL_H
#define POOL_H
#include <pthread.h>
#include <semaphore.h>

#define MAXTHREADS 200

void initialize(char ** argv);
void initializeThreads(pthread_t * tids, int threadCount);
void thread(void * arg);
void Pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
void killThreads(pthread_t * tids, int threadCount);
void Sem_init(sem_t * sem, int pshared, unsigned int value);
void V(sem_t * s);
void P(sem_t * s);
void addJob(int (*funcPtr) (int x));
int done();

#endif