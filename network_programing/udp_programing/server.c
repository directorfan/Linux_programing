#include <strings.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#define SRV_PORT 9725

int main() {
	int sockfd;
	sockfd = socket(AF_INET,SOCK_DGRAM,0);

	struct sockaddr_in srv_addr, clt_addr;
	socklen_t clt_addr_len;

	bzero(&srv_addr,sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(sockfd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
	char buf[BUFSIZ];
	char str[INET_ADDRSTRLEN];
	while (1) {
		clt_addr_len = sizeof(clt_addr);
		int num = recvfrom(sockfd,buf,BUFSIZ,0,(struct sockaddr*)&clt_addr,&clt_addr_len);
		if (num == -1)
			perror("recvfrom error\n");
		printf("the client addr is %s, port is %d\n",
				inet_ntop(AF_INET,&clt_addr.sin_addr,str,sizeof(str)),
					ntohs(clt_addr.sin_port));
		
		for (int i = 0; i < num; ++i) 
			buf[i] = toupper(buf[i]);

		int n = sendto(sockfd,buf,num,0,(struct sockaddr*)&clt_addr,clt_addr_len);
		if (n == -1)
			perror("send to error\n");
	}
	close(sockfd);
	return 0;
}
