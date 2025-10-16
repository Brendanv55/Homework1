
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
    std:: cout << "start\n";
    // const char* name = "/brv";
    // mode_t mode = 0644;
    // unsigned value = 0;
    // int val = sem_init(smphre, 1, 1);

    smphre = sem_open("/sembrv", O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP, 1);

     if (smphre == SEM_FAILED) {
        std::cout << "You're a flop " << errno << "\n";
    } else std::cout << "Succeseed wowww\n";

    int val = sem_init(smphre, 1, 1);

    std::cout << "dow\n";

    void* addr = shmat(shmID, data_ptr, shmflag);
    // smaddr
    // Copy my struct into the shared memory segmenttry
    std::cout << "meow";
    addr = data_ptr; // Copy data into addr?
    std::cout << "dow";

    int detach = shmdt(addr);

    std::cout << "bow\n";

    // for (int i = 0; i < 5; i++) {
        threadArgs* ptr = new threadArgs;
        ptr->sem_ptr = smphre;
        ptr->data_ptr = data_ptr;
        pthread_t threadId;
        int success = pthread_create(&threadId, NULL, produce, (void*) ptr); // Create thread
        pthread_join(threadId, NULL); // Blocking wait
        // delete &ptr;
    // }

    // delete &data_ptr;
    //TODO:" Cleanup shmat & Sem
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

    // int* val;
    // sem_getvalue(smphre, val);
    // std::cout << &val << "\n";
    // sem_trywait(arg_smphre); // wait to enter critical section
    std::cout << "Meow\n";
    // void* shmaddr;
    void* addr = shmat(shmID, ptr->data_ptr, shmflag); // Acquire shared memory

    // data* shared_data = (data *) shmaddr;

    // std::cout << (ptr->data_ptr)->out << " " << (ptr->data_ptr)->in << '\n';

    // data obj = *(ptr->data_ptr);

    while (((ptr->data_ptr)->in + 1) % 2 == (ptr->data_ptr)->out) {};// s//td::cout << "Waiting " << obj. in << ' ' << obj.out << '\n';

    // std::cout << "Dow\n";

    (ptr->data_ptr)->buffer[(ptr->data_ptr)->in] = 1; // Message 
    (ptr->data_ptr)->in = ((ptr->data_ptr)->in + 1) % 2; // Circular queuei++
    
    int detach = shmdt(addr);
        // std::cout << "Grr\n";

    std::cout << "Thread values: " << (ptr->data_ptr)->out << ' ' << (ptr->data_ptr)->in << '\n';

    sem_post(ptr->sem_ptr); // Only posting in producer
    pthread_exit(0);
}