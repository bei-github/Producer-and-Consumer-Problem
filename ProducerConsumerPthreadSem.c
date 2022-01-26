#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

int mem[15];
int count = 0;

pthread_mutex_t mutex_id;
sem_t semFull, semEmpty;

void* produce() {
	int data = rand() % 1000;
	while (1) {
		pthread_mutex_lock(&mutex_id);
		sem_wait(&semEmpty);
		mem[count] = data;
		count++;
		pthread_mutex_unlock(&mutex_id);
		sem_post(&semFull);
	}
}

void* consume() {
	while (1) {
		pthread_mutex_lock(&mutex_id);
		sem_wait(&semFull);
		printf("data is %d \n", mem[count - 1]);
		count--;
		pthread_mutex_unlock(&mutex_id);
		sem_post(&semEmpty);
	}
}


int main(int argc, char* argv[]) {
	pthread_t thread_id[10];
	
	pthread_mutex_init(&mutex_id, NULL);
	sem_init(&semEmpty, 0, 15);
	sem_init(&semFull, 0, 0);

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
	sem_destroy(&semFull);
	sem_destroy(&semEmpty);

	return 0;
}