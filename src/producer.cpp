
#include "header.hpp"

//Semaphore

data* data_ptr = new data;
sem_t* smphre;
    
key_t key = 200;
size_t size = sizeof(data);
// int shmflag = 0;
int shmID = shmget(key, size, IPC_CREAT);

//produce method
void* produce(void *ptr);

int main() {
    std:: cout << "start producer\n";

    smphre = sem_open("/sembrv", O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 0);

    if (smphre == SEM_FAILED) {
        std::cout << "Sem failed to open: " << errno << "\n";
    }

    int val = sem_init(smphre, 1, 1);

    std::cout << "dow\n";

    void* dataAddr = shmat(shmID, (void*)0, 0);
    if (dataAddr == (void*)-1) { std::cerr << "Bad shmat use?\n";}

    std::cout << shmID << " " << sizeof(data) << '\n';

    // if (ftruncate(shmID, sizeof(data)) == -1) {
    //     std::cerr << "Memory resize failed " << errno;
    // }

    // data* shared_data;
     
    // shared_data = (data*) mmap(NULL, sizeof(data), PROT_READ | PROT_WRITE, MAP_SHARED, shmID, 0);
    
    // if (shared_data == MAP_FAILED) std::cerr << "Memory map failed :(" << errno;

    // //Unmap shared mem from proccess
    // if (munmap (shared_data, sizeof (data)) == -1) std::cerr<< "munmap " << errno;    

    int detach = shmdt(dataAddr);

    std::cout << "bow\n";

    for (int i = 0; i < 2; i++) {
        threadArgs* ptr = new threadArgs;
        ptr->sem_ptr = smphre;
        ptr->data_ptr = data_ptr;
        pthread_t threadId;
        int success = pthread_create(&threadId, NULL, produce, (void*) ptr); // Create thread
        pthread_join(threadId, NULL); // Blocking wait
        // delete &ptr;
    }

    std::cout << "Main thread values: " << (data_ptr)->out << " " << (data_ptr)->in << '\n';

    return 0;
}

void* produce(void *arg_struct) {
    threadArgs* ptr;
    ptr = (threadArgs*) arg_struct;
    // sem_t* arg_smphre;
    // arg_smphre = (sem_t*) ptr;

    key_t key = 200;
    size_t size = sizeof(data);
    int shmflag = 0;
    int shmID = shmget(key, size, shmflag);

    void* addr = shmat(shmID, ptr->data_ptr, shmflag); // Acquire shared memory

    while (((ptr->data_ptr)->in + 1) % 2 == (ptr->data_ptr)->out) {};// s//td::cout << "Waiting " << obj. in << ' ' << obj.out << '\n';

    (ptr->data_ptr)->buffer[(ptr->data_ptr)->in] = 1; // Message 
    (ptr->data_ptr)->in = ((ptr->data_ptr)->in + 1) % 2; // Circular queuei++
    
    int detach = shmdt(addr);
        // std::cout << "Grr\n";
    std::cout << "Producer produced an item\n";

    sem_post(ptr->sem_ptr); // Only posting in producer
    pthread_exit(0);
}