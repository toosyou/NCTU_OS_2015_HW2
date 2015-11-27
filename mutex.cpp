#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <pthread.h>

#define NUMBER_THREAD 16

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
        cout << value << endl;
    }
    /* data */
};

Counter x;

int number2thread = 100000000;

void* thread_runner(void*){
    for(int i=0;i<number2thread;++i){
        pthread_mutex_lock(&mutex_counter);
        x.increment();
        pthread_mutex_unlock(&mutex_counter);
    }
}

int main(void){
    clock_t start_clock = clock();

    pthread_t tid[NUMBER_THREAD];
    for(int i=0;i<NUMBER_THREAD;++i){
        pthread_create(&tid[i],NULL,thread_runner,0);
    }
    for(int i=0;i<NUMBER_THREAD;++i){
        pthread_join(tid[i],NULL);
    }
    x.print();

    clock_t used_clock = clock() - start_clock;
    cout << "time used: " << (float) used_clock / CLOCKS_PER_SEC << endl;
    return 0;
}