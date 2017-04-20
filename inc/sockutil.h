#ifndef UNP_INCLUDE_SOCKUTIL_H
#define UNP_INCLUDE_SOCKUTIL_H

#include<sys/socket.h>
#include <arpa/inet.h>

#include "unpbase.h"

typedef struct sockaddr SA;

int open_listen(int port);
ssize_t writen(int fd, const void *vptr, size_t n);
ssize_t readn(int fd, void *vptr, size_t n);
ssize_t readline(int fd, void *vptr, size_t maxlen);
ssize_t readlinebuf(void **vptrptr);

#endif
