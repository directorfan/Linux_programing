#include<unistd.h>
#include<sys/types.h>
#include<stdio.h>
#include<sys/wait.h>
int main() {
	int i = 0;
	for (;i < 5; ++i) {
		if (fork() == 0) {
			sleep(i);
			printf("i am the %dth child\n",i+1);
			break;
		}
	}
	if (i == 5) {
		sleep(5);
		printf("....\n");
		while (waitpid(-1,NULL,0) != -1) {
			printf("child process deal\n");
		}
	}
}
