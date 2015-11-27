#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#define number_thread 4

using namespace std;

int index = 0;

pthread_mutex_t mutex_in,mutex_out;

struct pi_calculator{
    int points_in;
    int points_out;
    pi_calculator(){
        points_in = 0;
        points_out = 0;
    }
};

pi_calculator tmp;
int total_number_of_points;

void* cal(void* arg){
    pi_calculator* this_cal = (pi_calculator*)arg;
    for(int i=0;i<total_number_of_points;++i){
        double x = (double)rand() / (double)RAND_MAX;
        double y = (double)rand() / (double)RAND_MAX;
        if(x*x + y*y <= 1){
            pthread_mutex_lock(&mutex_in);
            this_cal->points_in++;
            pthread_mutex_unlock(&mutex_in);
        }
        else{
            pthread_mutex_lock(&mutex_out);
            this_cal->points_out++;
            pthread_mutex_unlock(&mutex_out);
        }
    }
}

int main(int argc,char *argv[]){
    clock_t start_clock = clock();

    //get the argument
    char *number_string = argv[1];
    total_number_of_points = atoi(number_string);
    //init

    srand(time(NULL));
    pthread_t tid[number_thread];
    for(int i=0;i<number_thread;++i){
        pthread_create(&tid[i],NULL,cal,(void*)&tmp);
    }
    for(int i=0;i<number_thread;++i){
        pthread_join(tid[i],NULL);
    }
    int number_points_in = tmp.points_in;
    int number_points_total = tmp.points_in + tmp.points_out;
    double pi = 4.0*(double)number_points_in/(double)number_points_total;
    cout << pi <<endl;

    clock_t used_clock = clock() - start_clock;
    cout << "time used: " << (float) used_clock / CLOCKS_PER_SEC << endl;
    return 0;
}