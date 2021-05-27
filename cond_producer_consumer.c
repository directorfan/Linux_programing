#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void sys_err (int ret, const char* str) {
	fprintf(stderr,"%s:%s\n",str,strerror(ret));
}

struct msg {
	int num;
	struct msg *next;
};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct msg *head;


void *producer (void *arg) {
	while (1) {
		struct msg* tp =(struct msg*) malloc(sizeof(struct msg));
		tp->num = rand() % 1000 + 1;
		printf("-------producer num = %d\n",tp->num);

		int	ret = pthread_mutex_lock(&mutex);
		if (ret != 0)
			sys_err(ret,"pthread_mutex_lock error");
		tp->next = head;
		head = tp;
		ret = pthread_mutex_unlock(&mutex);
		if (ret != 0)
			sys_err(ret,"pthread_mutex_unlock error");
		pthread_cond_signal(&cond);

		sleep(rand()%3);
	}
	return NULL;
}

void *consumer (void *arg) {
	while (1) {
		struct msg* tp;

		int ret = pthread_mutex_lock(&mutex);
		if (ret != 0)
			sys_err(ret,"pthread_mutex_lock error");
		while (!head) {
			ret = pthread_cond_wait(&cond,&mutex);
			if (ret != 0)
				sys_err(ret,"pthread_cond_wait error");
		}
		tp = head;
		head = head->next;
		ret = pthread_mutex_unlock(&mutex);
		if (ret != 0)
			sys_err(ret,"pthread_mutex_unlock error");

		printf("consumer %lu num = %d\n",pthread_self(),tp->num);
		free(tp);
		sleep(rand()%3);
	}
	return NULL;
}

int main() {

	srand(time(NULL));
	pthread_t pid, cid[4];
	pthread_create(&pid,NULL,producer,NULL);

	pthread_create(&(cid[1]),NULL,consumer,NULL);
	pthread_create(&(cid[2]),NULL,consumer,NULL);
	pthread_create(&(cid[3]),NULL,consumer,NULL);
	pthread_create(&(cid[4]),NULL,consumer,NULL);

	pthread_join(pid,NULL);
	pthread_join(cid[1],NULL);
	pthread_join(cid[2],NULL);
	pthread_join(cid[2],NULL);
	pthread_join(cid[2],NULL);

	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);	

	return 0;
}

