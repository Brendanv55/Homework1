
#include "header.hpp"

//Semaphore

data* data_ptr = new data;
sem_t* smphre;
    
key_t key = 200;
size_t size = sizeof(data);
int shmflag = 0;
int shmID = shmget(key, size, shmflag);

//produce method
void* produce(void *ptr);

int main() {
    std:: cout << "start consumer\n";

    smphre = sem_open("/sembrv", O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 1);

    if (smphre == SEM_FAILED) {
        std::cout << "Sem failed to open: " << errno << "\n";
    }

    for (int i = 0; i < 2; i++) {
        consumerArgs* ptr = new consumerArgs;
        ptr->sem_ptr = smphre;

        pthread_t threadId;
        int success = pthread_create(&threadId, NULL, produce, (void*) ptr); // Create thread
        pthread_join(threadId, NULL); // Blocking wait

        std::cout << "Consumed: " << ptr->return_val << "\n";
    }

    return 0;
}

void* produce(void *arg_struct) {
    consumerArgs* ptr;
    ptr = (consumerArgs*) arg_struct; // Typecast from void to my argument structure

    // std::cout << "Hello\n"
    sem_wait(ptr->sem_ptr); // wait to enter critical section
    std::cout << "Hello \n";
    key_t key = 200;
    size_t size = sizeof(data);
    int shmflag = 0;
    int shmID = shmget(key, size, shmflag);
        std::cout << "Hello godbye \n";


    void* void_ptr = shmat(shmID, NULL, 0);
    data* shared_data = (data *) void_ptr;

    while (shared_data->in == shared_data->out) {};//Do nothing if empty

    ptr->return_val = shared_data->buffer[shared_data->out];
    shared_data->out = (shared_data->out + 1) % 2;

    pthread_exit(0);
}