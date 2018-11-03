#include <stdio.h>
#include <stdlib.h>



//make queue
//initialize threadds
//add thread to queue
//extract thread for every job
//return the thread after it finishes execution
void initializeThreads(pthread_t * tids, int threadCount);

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

}