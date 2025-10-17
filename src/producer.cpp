
// Brendan Valleau
// 10/17/2025
// KSU CS-OS
// Homework1

#include "header.hpp"

// data* data_ptr = new data;
sem_t* smphre;

//produce method
void* produce(void *ptr);

int main() {

    smphre = sem_open("/sembrv", O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 0);

    if (smphre == SEM_FAILED) {
        std::cout << "Sem failed to open: " << errno << "\n";
    }

    int val = sem_init(smphre, 1, 1);

    key_t key = 200;
    size_t size = sizeof(data);
    int shmID = shmget(key, size, IPC_CREAT | IPC_EXCL | 0777);

    if (shmID == -1) std::cerr << "Shared memory failed to retrieve";

    data* dataAddr = (data*) shmat(shmID, NULL, 0);
    if (dataAddr == (void*)-1) { std::cerr << "shmat failed, error no:  "<< errno << "\n";}

    int detach = shmdt(dataAddr);

    // std::cout << "bow\n";

    for (int i = 0; i < 10; i++) {
        threadArgs* ptr = new threadArgs;
        ptr->sem_ptr = smphre;
        pthread_t threadId;
        int success = pthread_create(&threadId, NULL, produce, (void*) ptr); // Create thread
        pthread_join(threadId, NULL); // Blocking wait
        delete ptr;
    }

    sem_post(smphre); // Allow consumer to process data before closing segment
    sem_close(smphre);
    shmctl(shmID, IPC_RMID, 0);
    return 0;
}

void* produce(void *arg_struct) {
    threadArgs* ptr;
    ptr = (threadArgs*) arg_struct;

    key_t key = 200;
    size_t size = sizeof(data);
    int shmflag = 0;
    int shmID = shmget(key, size, 0666);

    if (shmID < 0) std::cerr << " Bad get " << errno << "\n";

    data* addr = (data*) shmat(shmID, NULL, 0); // Acquire shared memory

    if (addr == (void*)-1) std::cerr << "Bad mat " << errno << "\n";

    while ((addr->in + 1) % 2 == addr->out) {};

    addr->buffer[addr->in] = 1;  // Message 
    addr->in = (addr->in + 1) % 2; // Circular queuei
    
    int detach = shmdt(addr);

    std::cout << "Producer produced an item\n";

    sem_post(ptr->sem_ptr); // Only posting in producer
    pthread_exit(0);
}