#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <pthread.h>

using namespace std;

pthread_mutex_t mutex_counter;

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
        pthread_mutex_lock(&mutex_counter);
        x.increment();
        pthread_mutex_unlock(&mutex_counter);
    }
}

int main(void){
    pthread_t tid[3];
    for(int i=0;i<3;++i){
        pthread_create(&tid[i],NULL,thread_runner,0);
    }
    for(int i=0;i<3;++i){
        pthread_join(tid[i],NULL);
    }
    x.print();
    return 0;
}