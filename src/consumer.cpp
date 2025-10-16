
#include <iostream>
#include "pthread.h"
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>

typedef struct {
        int in = 0;
        int out = 0;
        int buffer[2] = {};
} data;

//Semaphore
const char* name = "brv_smph";
sem_t* smphre = sem_open(name, 1);

//Shared memory init
data* data_ptr = new data;

key_t key = 200;
size_t size = sizeof(&data_ptr);
int shmflag = 1;
int shmID = shmget(key, size, shmflag);

//produce method
void *consume(void *ptr);

int consumedItem;

int main() {
    for (int i = 0; i < 5; i++) {
        // pthread_t threadId;
        // int success = pthread_create(&threadId, NULL, produce, NULL); // Create thread
        // pthread_join(threadId, NULL); // Blocking wait
    }
}

int item;
void *consume(void *ptr) {
    sem_wait(smphre); // wait to enter critical section

    void* smaddr;
    void* addr = shmat(shmID, smaddr, shmflag); // Acquire shared memory

    data* shared_data = (data *) addr;

    while (shared_data->out == shared_data->in) {};


    item = shared_data->buffer[shared_data->out]; // Message 
    shared_data->out = (shared_data->out + 1) % 2; // Increase out
    
    int detach = shmdt(addr);

    sem_post(smphre); // release
    pthread_exit(0);
}