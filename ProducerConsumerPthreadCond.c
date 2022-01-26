#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

int mem[10];
int wPtr = 0, rPtr = 0;
bool empty = true, full = false;

pthread_mutex_t mutex_id;
pthread_cond_t cond_id1, cond_id2;

void* produce() {
	int data = rand() % 1000;
	while (1) {
		pthread_mutex_lock(&mutex_id);
		while (full) {
			pthread_cond_wait(&cond_id1, &mutex_id);
		}
		mem[wPtr] = data;
		wPtr = (wPtr + 1) % 10;
		empty = false;
		full = (wPtr == rPtr);
		pthread_mutex_unlock(&mutex_id);
		pthread_cond_broadcast(&cond_id2);
	}
}

void* consume() {
	while (1) {
		pthread_mutex_lock(&mutex_id);
		while (empty) {
			pthread_cond_wait(&cond_id2, &mutex_id);
		}
		printf("data is %d \n", mem[rPtr]);
		rPtr = (rPtr + 1) % 10;
		full = false;
		empty = (rPtr == wptr);
		pthread_mutex_unlock(&mutex_id);
		pthread_cond_broadcast(&cond_id1);
	}
}


int main(int argc, char* argv[]) {
	pthread_t thread_id[10];
	
	pthread_mutex_init(&mutex_id, NULL);
	pthread_cond_init(&cond_id1, NULL);
	pthread_cond_init(&cond_id2, NULL);

	for (int idx = 0; idx < 10; idx++) {
		if (idx & 0 == 0) {
			if (pthread_create(&thread_id[idx], NULL, &produce, NULL) != 0) {
				perror(" Can't create producer thread");
			}
		}
		else {
			if (pthread_create(&thread_id[idx], NULL, &consume, NULL) != 0) {
				perror(" Can't create consumer thread");
			}
		}
	}
	

	for (int idx = 0; idx < 10; idx++) {
		if (pthread_join(thread_id[idx], NULL) != 0) {
			perror(" Can't create join thread");
		}
	}

	pthread_mutex_destroy(&mutex_id);
	pthread_cond_destroy(&cond_id1);
	pthread_cond_destroy(&cond_id2);

	return 0;
}
