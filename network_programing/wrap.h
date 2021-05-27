#ifndef _WRAP_H_
#define _WRAP_H_


#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

void sys_err(const char*str);
int Socket(int domain, int type, int protocol);
int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Accept(int socket, struct sockaddr *restrict address,socklen_t *restrict address_len);
int Connect(int socket, const struct sockaddr *address,socklen_t address_len);
ssize_t Read(int fd, void *buf, size_t count);
size_t Write(int fd, const void *buf, size_t count);

#endif
