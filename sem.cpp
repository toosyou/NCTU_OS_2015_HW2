#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <semaphore.h> 
#include <pthread.h>

using namespace std;

sem_t sem_counter;

class Counter
{

    int value;

public:
    Counter(){
        this->value = 0;
    }
    ~Counter(){
        //do nothing
    }

    void increment(void){
        this->value++;
    }
    void print(void){
        cout << value <<endl;
    }
    /* data */
};

Counter x;

void* thread_runner(void*){
    for(int i=0;i<100000000;++i){
        sem_wait(&sem_counter);
        x.increment();
        sem_post(&sem_counter);
    }
}

int main(void){
    clock_t start_clock = clock();

    pthread_t tid[3];
    sem_init(&sem_counter,0,1);
    for(int i=0;i<3;++i){
        pthread_create(&tid[i],NULL,thread_runner,0);
    }
    for(int i=0;i<3;++i){
        pthread_join(tid[i],NULL);
    }
    x.print();
    sem_destroy(&sem_counter);

    clock_t used_clock = clock() - start_clock;
    cout << "time used: " << (float) used_clock / CLOCKS_PER_SEC << endl;
    return 0;
}
