#include "wrap.h"

#include <sys/socket.h>
#include <sys/select.h>
#include <strings.h>

#define SRV_PORT 6666

int main() {
	int lfd, maxfd;
	char buf[BUFSIZ];

	lfd = Socket(AF_INET,SOCK_STREAM,0);
	maxfd = lfd;

	int opt = 1;
	setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,(void*)&opt,sizeof(opt));

	struct sockaddr_in srv_addr, clt_addr;
	socklen_t clt_addr_len = sizeof(clt_addr);

	bzero(&srv_addr,sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(lfd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));

	Listen(lfd,128);

	fd_set rset, allset;
	FD_ZERO(&allset);
	FD_SET(lfd,&allset);


	while (1) {
		rset = allset;
		int nready = select(maxfd+1,&rset,NULL,NULL,NULL);

		if (nready == -1)
			sys_err("select error\n");
		if (nready == 0)
			continue;

		if (FD_ISSET(lfd,&rset)) {
			int cfd = Accept(lfd,(struct sockaddr*)&clt_addr,&clt_addr_len);
			if (cfd > maxfd)
				maxfd = cfd;
			FD_SET(cfd,&allset);

			if (--nready == 0)
				continue;
		}

		for (int i = lfd+1; i < maxfd+1; ++i) {
			if (FD_ISSET(i,&rset)) {
				int ret = Read(i,buf,sizeof(buf));
				if (ret == 0) {
					close(i);
					FD_CLR(i,&allset);
				} else {

					for (int j = 0; j < ret; ++j) {
						buf[j] = toupper(buf[j]);
					}

					Write(STDOUT_FILENO,buf,ret);
					Write(i,buf,ret);
				}
			}
		}
	}
	close(lfd);
	return 0;
}

