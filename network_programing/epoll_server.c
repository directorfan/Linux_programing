#include "wrap.h"
#include <string.h>
#include <sys/epoll.h>
#include <ctype.h>

#define SRV_PORT 8888
#define OPEN_MAX 8192
int main() {
	int lfd;
	lfd = Socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in srv_addr, clt_addr;
	socklen_t clt_addr_len;

	char buf[BUFSIZ];

	bzero(&srv_addr,sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(lfd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));

	Listen(lfd,128);

	int epfd = epoll_create(OPEN_MAX);
	if (epfd == -1)
		sys_err("epoll_create error\n");

	struct epoll_event tfd, ep[OPEN_MAX];
	tfd.events = EPOLLIN;
	tfd.data.fd = lfd;

	int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&tfd);
	if (ret == -1)
		sys_err("epoll_ctl error\n");

	while (1) {
		int times = epoll_wait(epfd,ep,OPEN_MAX,-1);
		if (ret == -1)
			sys_err("epoll_wait error\n");

		for (int i = 0; i < times; ++i) {
			int sockfd;
			if ((sockfd = ep[i].data.fd) == lfd) {
				clt_addr_len = sizeof(clt_addr_len);
				int cfd = Accept(lfd,(struct sockaddr*)&clt_addr,&clt_addr_len);
				tfd.data.fd = cfd;
				tfd.events = EPOLLIN;
				ret = epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&tfd);
			} else {
				int num = Read(sockfd,buf,sizeof(buf));
				if (num == -1) {
					if (errno == ECONNRESET) {
						ret = epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,NULL);
						if (ret == -1)
							sys_err("epoll_ctl error\n");
						close(sockfd);
					} else
						sys_err("read eroro\n");
				} else if (num == 0) {
					ret = epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,NULL);
					if (ret == -1)
						sys_err("epoll_ctl error\n");
					close(sockfd);
				} else {
					for (int j = 0; j < num; ++j)
						buf[j] = toupper(buf[j]);
					Write(STDOUT_FILENO,buf,num);
					Write(sockfd,buf,num);
				}
			}
		}
	}
	close(lfd);
	close(epfd);
	return 0;
}
