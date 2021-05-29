#include "wrap.h"
#include <poll.h>
#include <strings.h>
#include <ctype.h>

#define SRV_PORT 7777
#define OPEN_MAX 1024
int main () {
	int lfd = Socket(AF_INET,SOCK_STREAM,0);
	int opt = 1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));
	char buf[BUFSIZ];

	struct sockaddr_in srv_addr, clt_addr;
	socklen_t clt_addr_len;
	bzero(&srv_addr,sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(lfd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));

	Listen(lfd,128);

	struct pollfd client[OPEN_MAX];
	client[0].fd = lfd;
	client[0].events = POLLIN;
	for (int i = 1; i < OPEN_MAX; ++i) 
		client[i].fd = -1;

	int maxi = 0;

	while (1) {
		int nready = poll(client,maxi+1,-1);
		if (nready == -1)
			sys_err("poll error\n");

		if (nready == 0)
			continue;

		if (client[0].revents & POLLIN) {
			clt_addr_len = sizeof(clt_addr);
			int cfd = Accept(lfd,(struct sockaddr*)&clt_addr,&clt_addr_len);
			int i;
			for (i = 1; i < OPEN_MAX; ++i) {
				if (client[i].fd == -1) {
					client[i].fd = cfd;
					client[i].events = POLLIN;
					if (maxi < i)
						maxi = i;
					break;
				}
			}

			if (i == OPEN_MAX)
				sys_err("connect too much\n");
			if (--nready == 0)
				continue;

		} else {
			for (int i = 1; i < maxi+1; ++i) {
				if (client[i].fd == -1)
					continue;
				if (client[i].revents & POLLIN) {
					int ret = Read(client[i].fd,buf,sizeof(buf));
					if (ret == 0) {
						close(client[i].fd);
						client[i].fd = -1;
						continue;
					} else if (ret == -1) {
						if (errno == ECONNRESET) {
							close(client[i].fd);
							client[i].fd = -1;
						}else
							sys_err("read error\n");
					} else {
						for (int j = 0; j < ret; ++j)
							buf[j] = toupper(buf[j]);
						Write(client[i].fd,buf,ret);
						Write(STDOUT_FILENO,buf,ret);
					}
				}
				if (--nready == 0)
					break;
			}
		}
	}
	close(lfd);
	return 0;
}
