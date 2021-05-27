#include "wrap.h"

#define SERV_PORT 9527


int main() {
	int cfd;
	char buf[BUFSIZ];
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET,"127.0.0.1",(void *)&serv_addr.sin_addr);

	cfd = Socket(AF_INET,SOCK_STREAM,0);

	int ret = Connect(cfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	while (1) {
		ret = Write(cfd,"hello\n",6);
		ret = Read(cfd,buf,sizeof(buf));
		ret = Write(STDOUT_FILENO,buf,ret);

		sleep(1);
	}
	close(cfd);
}
