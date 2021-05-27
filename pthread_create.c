#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

struct retv {
	int val;
	char a[256];
};

void sys_err(const char *str) {
	perror(str);
	exit(1);
}
void *tfn(void *arg) {
	printf("pid = %d, tid = %lu\n",getpid(),pthread_self());
	struct retv *ret = (struct retv*)malloc(sizeof(struct retv));
	ret->val = 100;
	strcpy(ret->a, "i am back");
	return (void*)ret;
}
	

int main() {
	pthread_t tid;
	int res;
	res = pthread_create(&tid,NULL,tfn,NULL);
	if (res != 0)
		sys_err("pthread_create error");
	struct retv *ret;
	pthread_join(tid,(void**)&ret);
	
	printf("ret->val = %d, ret->a = %s\n",ret->val,ret->a);

	pthread_exit(0);
}
