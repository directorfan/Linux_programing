#include "wrap.h"

#include <sys/epoll.h>

#define maxline 10

int main() {
	int pfd[2];
	char buf[maxline];
	struct epoll_event tmp, epf[10];

	int ret = pipe(pfd);
	if (ret == -1)
		sys_err("pipe error\n");

	 pid_t pid = fork();
	 if (pid == -1)
		 sys_err("fork error\n");
	 else if (pid == 0) {
		 close(pfd[0]);
		 char ch = 'a';
		 while (1) {
			 int i;
			for (i = 0; i < maxline/2; ++i)
				buf[i] = ch;
			buf[i-1] = '\n';
			++ch;
			for (i = maxline/2; i < maxline;++i)
				buf[i] = ch;
			buf[i-1] = '\n';
			Write(pfd[1],buf,10);
			++ch;
			sleep(3);
		 }
	 } else {
		close(pfd[1]);
		int epfd = epoll_create(1);
		if (epfd == -1)
			sys_err("epoll_create error\n");

		//tmp.events = EPOLLIN | EPOLLET; 
		tmp.events = EPOLLIN; 
		tmp.data.fd = pfd[0];
		ret = epoll_ctl(epfd,EPOLL_CTL_ADD,pfd[0],&tmp);
		if (ret == -1)
			sys_err("epoll_ctl error\n");
		while (1) {
			char buf[10];
			int epret = epoll_wait(epfd,epf,10,-1);
			if (epret == -1)
				sys_err("epoll_wait error\n");

			int num = Read(pfd[0],buf,maxline/2);
			Write(STDOUT_FILENO,buf,num);
		}
	 }
	 close(pfd[0]);
	 return 0;
}
