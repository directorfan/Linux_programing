#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include<stdlib.h>

void my_catch(int signo) {
	int status;
	pid_t wpid;
	while ((wpid = waitpid(0,&status,WNOHANG)) != -1 && wpid != 0) {
		if (WIFEXITED(status))
			printf("----catch child %d,status %d\n",wpid,WEXITSTATUS(status));
	}
}
void sys_err(const char *str) {
	perror(str);
	exit(1);
}
int main() {
	int i;
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set,SIGCHLD);
	sigprocmask(SIG_BLOCK,&set,NULL);
	for (i = 0; i < 5; ++i) 
		if (fork() == 0) 
			break;
	if (i == 5) {
		struct sigaction act;
		act.sa_handler = my_catch;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		int ret = sigaction(SIGCHLD,&act,NULL);


		sigset_t set;
		sigemptyset(&set);
		sigaddset(&set,SIGCHLD);
		sigprocmask(SIG_UNBLOCK,&set,NULL);

		if (ret == -1)
			sys_err("sigaction error\n");
		while (1);
	} else {
		printf("i am child %d\n",getpid());
		return 1;
	}
}
