#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <pthread.h>

using namespace std;

pthread_spinlock_t counter_lock;

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
        cout << value;
    }
    /* data */
};

Counter x;

void* thread_runner(void*){
    for(int i=0;i<100000000;++i){
        pthread_spin_lock(&counter_lock);
        x.increment();
        pthread_spin_unlock(&counter_lock);
    }
}

int main(void){
    pthread_t tid[3];
    pthread_spin_init(&counter_lock,0);
    for(int i=0;i<3;++i){
        pthread_create(&tid[i],NULL,thread_runner,0);
    }
    for(int i=0;i<3;++i){
        pthread_join(tid[i],NULL);
    }
    x.print();
    return 0;
}