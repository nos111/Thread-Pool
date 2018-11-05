#ifndef POOL_H
#define POOL_H

#define MAXTHREADS 200

int initialize(int threadCount);
int addJob(int (*funcPtr) (int x));

#endif