#include <stdio.h>
#include <stdlib.h>



//make queue
//initialize threadds
//add thread to queue
//extract thread for every job
//return the thread after it finishes execution

void initializeThreads(pthread_t * tids, int threadCount);
void thread(void * arg);
void Pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

int main(int argc, char ** argv) {

        if(argc != 2) {
                printf("Usage: <thread count> \n");
                return 0;
        }

        pthread_t * tids = malloc(sizeof(pthread_t) * atoi(argv[1])); 
        initializeThreads(tids, atoi(argv[1]));

        printf("%s", argv[1]);
        return 0;
}

void initializeThreads(pthread_t * tids, int threadCount) {
        int * ptr;
        for(int i = 0; i < threadCount; i++) {
                ptr = malloc(sizeof(int));
                *ptr = i;
                Pthread_create(tids[*ptr], NULL, thread, ptr);
        }
}

void Pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg) {
        if(pthread_create(thread, attr, start_routine, arg) != 0) {
                perror("error creating thread ");
        }
}

void thread(void * arg) {

}

