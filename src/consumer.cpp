
// Brendan Valleau
// 10/17/2025
// KSU CS-OS
// Homework1

#include "header.hpp"

//Semaphore

data* data_ptr = new data;
sem_t* smphre;
    
key_t key = 200;
size_t size = sizeof(data);
int shmflag = 0;
int shmID = shmget(key, size, IPC_CREAT | IPC_EXCL | 0777);

//consume method
void* consume(void *ptr);

int main() {
    std:: cout << "start consumer\n";

    smphre = sem_open("/sembrv", O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 1);

    if (smphre == SEM_FAILED) {
        std::cout << "Sem failed to open: " << errno << "\n";
    }

    for (int i = 0; i < 10; i++) {
        consumerArgs* ptr = new consumerArgs;
        ptr->sem_ptr = smphre;

        pthread_t threadId;
        int success = pthread_create(&threadId, NULL, consume, (void*) ptr); // Create thread, pass my thread argument struct ptr
        pthread_join(threadId, NULL); // Blocking wait

        std::cout << "Consumed: " << ptr->return_val << "\n";

        delete ptr;
    }
    if (smphre != (void*)-1) sem_wait(smphre); // Allow main thread to cleanup shared memory
    return 0;
}

void* consume(void *arg_ptr) {
    consumerArgs* ptr;
    ptr = (consumerArgs*) arg_ptr; // Typecast from void to my argument structure

    sem_wait(ptr->sem_ptr); // wait to enter critical section

    key_t key = 200;
    size_t size = sizeof(data);
    int shmflag = 0;

    int shmID = shmget(key, size, 0666);
    if (shmID < 0) std::cerr << " Bad get " << errno << "\n";

    data* dataptr = (data*) shmat(shmID, NULL, 0);

    if (dataptr == (void*)-1) std::cerr << "Bad mat " << errno << "\n";

    while (dataptr->in == dataptr->out) {}; //Do nothing if empty

    int val = dataptr->buffer[dataptr->out];
    ptr->return_val = val;
    dataptr->out = (dataptr->out + 1) % 2;

    int detach = shmdt(dataptr);

    pthread_exit(0);
}