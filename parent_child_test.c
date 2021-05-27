#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
int main() {
	int fd[2];
	int res = pipe(fd);
	if (res == -1) {
		perror("pipe error");
		exit(1);
	}
	int pid = fork();
	if (pid == -1) {
		perror("fork error");
		exit(1);
	} else	if (pid > 0) {
		close(fd[0]);
		dup2(fd[1],STDOUT_FILENO);
		execlp("ls","ls",NULL);
		wait(NULL);
	} else {
		close(fd[1]);
		dup2(fd[0],STDIN_FILENO);
		execlp("wc","wc","-l",NULL);
	}
}

