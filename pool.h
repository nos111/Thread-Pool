#ifndef POOL_H
#define POOL_H
#include <pthread.h>
#include <semaphore.h>

#define MAXTHREADS 200

void initialize(int threadCount);
void addJob(int (*funcPtr) (int x));

#endif