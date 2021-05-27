#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_rwlock_t rwlock;
int var = 100;

void *rdfn (void* arg) {
	while (1) {
		pthread_rwlock_rdlock(&rwlock);
		usleep(1000);
		printf("i am read thread -- var = %d\n", var);
		pthread_rwlock_unlock(&rwlock);
		usleep(1000);
	}
}

void *wrfn(void *arg) {
	while (1) {
		pthread_rwlock_wrlock(&rwlock);
		usleep(1000);
		++var;
		printf("write thread var = %d\n",var);
		pthread_rwlock_unlock(&rwlock);
		usleep(1000);
	}
}

	int main() {
		pthread_rwlock_init(&rwlock,NULL);

		pthread_t tid[8];
		for (int i = 0; i < 5; ++i) {
			pthread_create(&tid[i],NULL,rdfn,NULL);
		}
		for (int i = 0; i < 3; ++i) {
			pthread_create(&tid[i+5],NULL,wrfn,NULL);
		}

		for (int i = 0; i < 8; ++i) {
			pthread_join(tid[i],NULL);
		}
		pthread_rwlock_destroy(&rwlock);
	}
