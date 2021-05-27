#include "wrap.h"

#define SERV_PORT 9527

int main() {
	int lfd, cfd;
	int ret;

	char buf[BUFSIZ], client_IP[1024];

	struct sockaddr_in serv_addr, clit_addr;
	socklen_t clit_addr_len = sizeof(clit_addr);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(9527);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	lfd = Socket(AF_INET,SOCK_STREAM,0);

	ret = Bind(lfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

	ret = Listen(lfd,128);

	cfd = Accept(lfd,(struct sockaddr*)&clit_addr,&clit_addr_len);

	inet_ntop(AF_INET,&clit_addr.sin_addr.s_addr,client_IP,sizeof(client_IP));
	if (!client_IP)
		sys_err("null client_IP\n");
	printf("IP:%s port:%d\n",
			client_IP
			,ntohs(clit_addr.sin_port));


	while (1) {
		int len = Read(cfd,buf,sizeof(buf));
		if (len == -1)
			sys_err("read error\n");

		Write(STDOUT_FILENO,buf,len);

		for (int i = 0; i < len; ++i) 
			buf[i] = toupper(buf[i]);

			Write(cfd,buf,len);
	}
		close(lfd);
		close(cfd);
		return 0;
}



