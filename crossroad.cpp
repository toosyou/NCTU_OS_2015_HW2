#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <errno.h>

#include <pthread.h>

using namespace std;

struct car{
	string name;
    int rest;
    int clock;
    int first_lock;
    int second_lock;
    int through;
    int index_now;
    pthread_mutex_t clock_lock;

	car(){
		rest = 0;
		clock = 1;
		first_lock = 0;
		second_lock = 0;
		through = 0;
		index_now = 0;
	}
};

pthread_mutex_t road_lock[4];

car N,E,S,W;
int clock_time = 1;
bool need_to_end = false;

void* check_time(void*){
	while(1){
		if(N.rest == 0 && E.rest == 0 && S.rest == 0 && W.rest == 0){
			need_to_end = true;
			break;
		}
		if(N.clock > clock_time && E.clock > clock_time && S.clock > clock_time && W.clock > clock_time){
			clock_time++;
			//check for deadlock
			if(N.through == 1 && E.through == 1 && S.through == 1 && W.through ==1){
				//deadlock
				cout << "A DEADLOCK HAPPENS at " << clock_time << endl;
				//make N through
				W.through = 0;
				N.through = 2;
			}
			pthread_mutex_unlock(&N.clock_lock);
			pthread_mutex_unlock(&E.clock_lock);
			pthread_mutex_unlock(&S.clock_lock);
			pthread_mutex_unlock(&W.clock_lock);
		}
	}
}

void* try_to_cross(void* arg){
	car* this_car = (car*)arg;
	while(1){
		if(need_to_end)
			break;
		if(pthread_mutex_trylock(&this_car->clock_lock) == EBUSY)
			continue;
		//get the lock
		this_car->clock++;
		if(this_car->rest == 0){
			continue;
		}
		if(this_car->through == 0){
			if(pthread_mutex_trylock(&road_lock[this_car->first_lock]) != EBUSY ){
				//get the lock
				this_car->through++;
			}
		}
		else if(this_car->through == 1){
			if(pthread_mutex_trylock(&road_lock[this_car->second_lock]) != EBUSY ){
				//get the lock
				this_car->through++;
			}
		}
		else if(this_car->through == 2){
			//crose the road
			cout << this_car->name << " " << this_car->index_now << " leaves at " << clock_time <<endl;
			this_car->through = 3;
			
		}
		else if(this_car->through == 3){
			//release
			this_car->index_now++;
			this_car->rest--;
			pthread_mutex_unlock(&road_lock[this_car->second_lock]);
			pthread_mutex_unlock(&road_lock[this_car->first_lock]);
			this_car->through = 0;
		}
	}
}

int main(int argc,char* argv[]){
    if(argc < 5)
        return -1;
    int number_cars_N = atoi(argv[1]);
    int number_cars_E = atoi(argv[2]);
    int number_cars_S = atoi(argv[3]);
    int number_cars_W = atoi(argv[4]);
    N.rest = number_cars_N;
    E.rest = number_cars_E;
    S.rest = number_cars_S;
    W.rest = number_cars_W;
    N.name = string("N");
    E.name = string("E");
    S.name = string("S");
    W.name = string("W");
    N.first_lock = 0; N.second_lock = 2;
    E.first_lock = 1; E.second_lock = 0;
    W.first_lock = 2; W.second_lock = 3;
    S.first_lock = 3; S.second_lock = 1;

    pthread_t threads[5]; // one for timing
    pthread_create(&threads[0],NULL,try_to_cross,&N);
    pthread_create(&threads[1],NULL,try_to_cross,&E);
    pthread_create(&threads[2],NULL,try_to_cross,&W);
    pthread_create(&threads[3],NULL,try_to_cross,&S);
    pthread_create(&threads[4],NULL,check_time,NULL);

    for(int i=0;i<5;++i){
    	pthread_join(threads[i],NULL);
    }

    return 0;
}