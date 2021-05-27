#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t mutex;

void *tfn(void *arg) {
	while (1) {
		pthread_mutex_lock(&mutex);
		printf("hello");
		sleep(rand()%3);
		printf(" world\n");
		pthread_mutex_unlock(&mutex);
		sleep(rand()%3);
	}
}

int main() {
	pthread_mutex_init(&mutex,NULL);
	int ret;
	pthread_t tid;
	pthread_create(&tid,NULL,tfn,NULL);

	srand(time(NULL));

	while (1) {
		pthread_mutex_lock(&mutex);
		printf("Hello");
		sleep(rand()%3);
		printf(" World\n");
		pthread_mutex_unlock(&mutex);
		sleep(rand()%3);
	}

	pthread_join(tid,NULL);
	pthread_mutex_destroy(&mutex);	
}
