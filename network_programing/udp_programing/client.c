#include <sys/socket.h>
#include <stdio.h>
#include <strings.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define SRV_PORT 9725

int main() {
	int sockfd;
	sockfd = socket(AF_INET,SOCK_DGRAM,0);
	if (sockfd == -1)
		perror("socket error\n");

	struct sockaddr_in srv_addr;
	bzero(&srv_addr,sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
	inet_pton(AF_INET,"127.0.0.1",&srv_addr.sin_addr);

	char buf[BUFSIZ];
	while(read(STDIN_FILENO,buf,BUFSIZ) != -1) {
		int n = sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&srv_addr,sizeof(srv_addr));
		if (n == -1)
			perror("sendto error\n");

		int num = recvfrom(sockfd,buf,sizeof(buf),0,NULL,0);
		if (num == -1)
			perror("recvfrom error\n");

		write(STDOUT_FILENO,buf,num);
	}
	close(sockfd);
	return 0;
}
