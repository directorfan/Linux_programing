#include "wrap.h"

void sys_err(const char*str) {
	perror(str);
	exit(1);
}

int Socket(int domain, int type, int protocol) {
	int n = socket(domain,type,protocol);
	if (n == -1)
		sys_err("socket error\n");
	return n;
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	int n = bind(sockfd,addr,addrlen);
	if (n == -1)
		sys_err("bind error\n");
	return n;
}

int Listen(int sockfd, int backlog) {
	int n = listen(sockfd,backlog);
	if (n == -1)
		sys_err("liste error\n");
	return n;
}

int Accept(int socket, struct sockaddr *restrict address,socklen_t *restrict address_len) {
again:;
	int n = accept(socket,address,address_len);
	if (n == -1) {
		if (errno ==  EINTR || errno == ECONNABORTED)
			goto again;
		else
			sys_err("accept error\n");
	}
	return n;
}

int Connect(int socket, const struct sockaddr *address,socklen_t address_len) {
again:;
	int n = connect(socket,address,address_len);
	if (n == -1) {
		if (errno == EINTR)
			goto again;
		else
			sys_err("connect error\n");
	}
}

ssize_t Read(int fd, void *buf, size_t count) {
again:;
	int n = read(fd,buf,count);
	if (n == -1) {
		if (errno == EINTR || errno == EAGAIN)
			goto again;
		else
			sys_err("read error\n");
	}
	return n;
}

size_t Write(int fd, const void *buf, size_t count) {
	int n = write(fd,buf,count);
	if (n == -1)
		sys_err("write error\n");
	return n;
}
