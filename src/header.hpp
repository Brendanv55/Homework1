
#include <iostream>
#include "pthread.h"
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <stdexcept>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>

typedef struct {
        int in = 0;
        int out = 0;
        int buffer[2] = {};
} data;


typedef struct {
    sem_t* sem_ptr;
    data* data_ptr;
} threadArgs;

typedef struct {
    sem_t* sem_ptr;
    int return_val;
} consumerArgs;
