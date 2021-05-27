#include "wrap.h"
#include <strings.h>
#include <stdio.h>
#include <pthread.h>

#define srv_port 8000

struct s_info {
	struct sockaddr_in clt_addr;
	int cfd;
};

void *do_work (void *arg) {
	int ret;
	char str[INET_ADDRSTRLEN];
	char buf[BUFSIZ];
	while (1) {
		struct s_info *info = (struct s_info*)arg;
		struct sockaddr_in clt_addr = info->clt_addr;
		int cfd = info->cfd;
		
		printf("client IP:%s port:%d\n",
				inet_ntop(AF_INET,&clt_addr.sin_addr,str,sizeof(str)),
				ntohs(clt_addr.sin_port));
		
	ret = Read(cfd,buf,sizeof(buf));
	if (ret == 0) {
		perror("client close\n");
		pthread_exit(NULL);
	}

	for (int i = 0; i < ret;++i)
		buf[i] = toupper(buf[i]);

	write(cfd,buf,ret);
	write(STDOUT_FILENO,buf,ret);
	}
}


int main() {
	int lfd, cfd;
	struct sockaddr_in srv_addr, clt_addr;
	pthread_t tid[128];
	int i = 0;

	socklen_t clt_addr_len = sizeof(clt_addr);

	bzero(&clt_addr,sizeof(clt_addr));

	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(srv_port);
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	

	lfd = Socket(AF_INET,SOCK_STREAM,0);

	Bind(lfd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));

	Listen(lfd,128);

	while (1) {
		cfd = Accept(lfd,(struct sockaddr*)&clt_addr,&clt_addr_len);
		struct s_info info;
		info.clt_addr = clt_addr;
		info.cfd = cfd;

		pthread_create(&tid[i],NULL,do_work,(void*)&info);
		pthread_detach(tid[i]);
		++i;
	}
}
