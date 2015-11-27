#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#define number_thread 4

using namespace std;

int index = 0;

struct pi_calculator{
    int points_in;
    int points_out;
    pi_calculator(){
        points_in = 0;
        points_out = 0;
    }
};

pi_calculator tmp[number_thread];
int number_points2thread = 0;


void* cal(void* arg){
    pi_calculator* this_cal = (pi_calculator*)arg;
    unsigned seed = time(NULL);
    for(int i=0;i<number_points2thread;++i){
        double x = (double)rand_r(&seed) / (double)RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&seed) / (double)RAND_MAX * 2.0 - 1.0;
        if(x*x + y*y <= 1.0){
            this_cal->points_in++;
        }
        else{
            this_cal->points_out++;
        }
    }
}

int main(int argc,char *argv[]){
    clock_t start_clock = clock();

    //get the argument
    char *number_string = argv[1];
    int total_number_of_points = atoi(number_string);
    number_points2thread = total_number_of_points / number_thread;
    //init
    int number_points_in = 0;
    int number_points_total = 0;
    //srand(time(NULL));

    pthread_t tid[number_thread];
    for(int i=0;i<number_thread;++i){
        pthread_create(&tid[i],NULL,cal,(void*)&tmp[i]);
    }
    for(int i=0;i<number_thread;++i){
        pthread_join(tid[i],NULL);
    }
    for(int i=0;i<number_thread;++i){
        cout << "in: " << tmp[i].points_in << "\tout: " << tmp[i].points_out <<endl;
        number_points_in += tmp[i].points_in;
        number_points_total += tmp[i].points_in + tmp[i].points_out;
    }
    double pi = (double)4.0*(double)number_points_in/(double)number_points_total;
    cout << pi <<endl;

    clock_t used_clock = clock() - start_clock;
    cout << "time used: " << (float) used_clock / CLOCKS_PER_SEC << endl;
    return 0;
}