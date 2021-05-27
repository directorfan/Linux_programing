#include "wrap.h"
#include <ctype.h>
#include <strings.h>
#include <signal.h>
#include <sys/wait.h>
#define srv_port 9999

void catch_child (int signum) {
	while (waitpid(0,NULL,WNOHANG) > 0);
}

int main() {
	int lfd, cfd;
	int pid;
	char buf[BUFSIZ];

	struct sockaddr_in srv_addr, clt_addr;
	bzero(&srv_addr,sizeof(srv_addr));

	socklen_t clt_addr_len = sizeof(clt_addr);
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(srv_port);
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	lfd = Socket(AF_INET,SOCK_STREAM,0);

	Bind(lfd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));

	Listen(lfd,128);

	while (1) {
		cfd  = Accept(lfd,(struct sockaddr*)&clt_addr,&clt_addr_len);

		pid = fork();
		if (pid == -1) {
			sys_err("fork error\n");
		} else if (pid == 0) {
			close(lfd);
			break;
		} else {
			struct sigaction act;
			act.sa_handler = catch_child;
			sigemptyset(&act.sa_mask);
			act.sa_flags = 0;

			sigaction(SIGCHLD,&act,NULL);
			close(cfd);
		}
	}

	if (pid == 0) {
		while (1) {
			int ret = Read(cfd,buf,sizeof(buf));
			if (ret == 0) {
				close(cfd);
				perror("client close\n");
				exit(1);
			}
			for (int i = 0; i < ret; ++i) {
				buf[i] = toupper(buf[i]);
			}
			write(STDOUT_FILENO,buf,ret);
			Write(cfd,buf,ret);
		}
	}
}
