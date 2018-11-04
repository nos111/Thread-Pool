#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "pool.h"
#include "queue.h"

/**************** Example routines ****************/
int sum(int x) {
        //printf("%d \n", x + 2);
        sleep(2);
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
       
        
        initialize(argv);
        for(int i = 0; i < 20; i++) {
                addJob(sum);
        }
        while(1) {
                
        }
        
        return 0;
}



