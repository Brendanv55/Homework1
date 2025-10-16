
#include "header.hpp"

//Semaphore

data* data_ptr = new data;
sem_t* smphre;
    
key_t key = 200;
size_t size = sizeof(data);
int shmflag = 0;
int shmID = shmget(key, size, shmflag);

//consume method
void* consume(void *ptr);

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
        int success = pthread_create(&threadId, NULL, consume, (void*) ptr); // Create thread
        pthread_join(threadId, NULL); // Blocking wait

        std::cout << "Consumed: " << ptr->return_val << "\n";
    }

    return 0;
}

void* consume(void *arg_ptr) {
    consumerArgs* ptr;
    // ptr = (sem_t*) sem;
    ptr = (consumerArgs*) arg_ptr; // Typecast from void to my argument structure

    // std::cout << "Hello\n"
    sem_wait(ptr->sem_ptr); // wait to enter critical section


    std::cout << "Hello \n";
    key_t key = 200;
    size_t size = sizeof(data);
    int shmflag = 0;
    int shmID = shmget(key, size, shmflag);
        std::cout << "Hello godbye \n";


    void* void_ptr = shmat(shmID, NULL, 0);
    // data* shared_data = (data *) void_ptr;
    data* dataptr;
    // dataptr = (data*) void_ptr;

    void* result= mmap(NULL, sizeof(data), PROT_READ | PROT_WRITE, MAP_SHARED, shmID, 0);
    if (result == MAP_FAILED) {
        std::cerr << "Memory map failed :(";
    } else dataptr = (data*) result;

    std::cout << "after ptr conversion\n";
    std::cout << "Let's check if object exitsts: " << dataptr->in << std::endl;

    while (dataptr->in == dataptr->out) {};//Do nothing if empty


    ptr->return_val = dataptr->buffer[dataptr->out];
    dataptr->out = (dataptr->out + 1) % 2;

    
    if (munmap (dataptr, sizeof (data)) == -1) std::cerr<< "munmap";

    pthread_exit(0);
}