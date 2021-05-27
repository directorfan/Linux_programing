#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#define NUM 5

sem_t product, blank;
int queue[NUM];

void *producer(void *arg) {
	int i = 0;
	while (1) {
		sem_wait(&blank);
		queue[i] = rand()%1000 + 1;
		printf("producer queue[i] = %d\n",queue[i]);
		sem_post(&product);
		i = (i+1)%NUM;
		sleep(rand()%3);
	}
}

void *consumer(void *arg) {
	int i = 0;
	while (1) {
		sem_wait(&product);
		printf("-----consumer queue[i] = %d\n",queue[i]);
		queue[i] = 0;
		sem_post(&blank);
		i = (i+1)%NUM;
		sleep(rand()%5);
	}
}

int main () {
	sem_init(&product,0,0);
	sem_init(&blank,0,5);

	srand(time(NULL));
	pthread_t pid,cid;

	pthread_create(&pid,NULL,producer,NULL);
	pthread_create(&cid,NULL,consumer,NULL);

	pthread_join(pid,NULL);
	pthread_join(cid,NULL);

	sem_destroy(&product);
	sem_destroy(&blank);
}

