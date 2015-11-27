#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <pthread.h>

using namespace std;

int homemade_spinlock = 0;

void homemade_spin_lock(int *spinlock_addr) {
    asm(
        "spin_lock: \n\t"
        "xorl %%ecx, %%ecx \n\t"
        "incl %%ecx \n\t"
        "spin_lock_retry: \n\t"
        "xorl %%eax, %%eax \n\t"
        "lock; cmpxchgl %%ecx, (%0) \n\t" "jnz spin_lock_retry \n\t"
        : : "r" (spinlock_addr) : "ecx", "eax" );
}
void homemade_spin_unlock(int *spinlock_addr) {
    asm(
        "spin_unlock: \n\t" "movl $0, (%0) \n\t"
        : : "r" (spinlock_addr) : );
}

class Counter
{

    int value;

public:
    Counter() {
        this->value = 0;
    }
    ~Counter() {
        //do nothing
    }

    void increment(void) {
        this->value++;
    }
    void print(void) {
        cout << value <<endl;
    }
    /* data */
};

Counter x;

void* thread_runner(void*) {
    for (int i = 0; i < 100000000; ++i) {
        homemade_spin_lock(&homemade_spinlock);
        x.increment();
        homemade_spin_unlock(&homemade_spinlock);
    }
}

int main(void) {
    clock_t start_clock = clock();

    pthread_t tid[3];
    homemade_spinlock = 0; //init
    for (int i = 0; i < 3; ++i) {
        pthread_create(&tid[i], NULL, thread_runner, 0);
    }
    for (int i = 0; i < 3; ++i) {
        pthread_join(tid[i], NULL);
    }
    x.print();

    clock_t used_clock = clock() - start_clock;
    cout << "time used: " << (float) used_clock / CLOCKS_PER_SEC << endl;
    return 0;
}