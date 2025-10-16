
#include <iostream>
#include "pthread.h"
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cstring>
#include <stdexcept>
// #include <cstdio>
// #include <cstdlib>
// #include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct {
        int in = 0;
        int out = 0;
        int buffer[2] = {};
} data;


typedef struct {
    sem_t* sem_ptr;
    data* data_ptr;
} threadArgs;
