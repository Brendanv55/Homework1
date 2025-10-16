
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
        int success = pthread_create(&threadId, NULL, consume, (void*) smphre); // Create thread
        pthread_join(threadId, NULL); // Blocking wait

        std::cout << "Consumed: " << ptr->return_val << "\n";
    }

    return 0;
}

void* consume(void *sem) {
    sem_t* ptr;
    ptr = (sem_t*) sem;
    // ptr = (consumerArgs*) arg_struct; // Typecast from void to my argument structure

    // std::cout << "Hello\n"
    sem_wait(ptr); // wait to enter critical section


    std::cout << "Hello \n";
    key_t key = 200;
    size_t size = sizeof(data);
    int shmflag = 0;
    int shmID = shmget(key, size, shmflag);
        std::cout << "Hello godbye \n";


    void* void_ptr = shmat(shmID, NULL, 0);
    // data* shared_data = (data *) void_ptr;
    struct data* shared_data = mmap(NULL, sizeof(struct data), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_data == MAP_FAILED) {
        std::cerr << "Memory map failed :(";
    }

    std::cout << "after ptr conversion\n";
    std::cout << "Let's check if object exitsts: " << shared_data->in << std::endl;

    while (shared_data->in == shared_data->out) {};//Do nothing if empty


    ptr->return_val = shared_data->buffer[shared_data->out];
    shared_data->out = (shared_data->out + 1) % 2;

    
    if (munmap (shared_data, sizeof (struct data)) == -1) std::cerr<< "munmap";

    pthread_exit(0);
}