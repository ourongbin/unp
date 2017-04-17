#ifndef UNP_INCLUDE_SOCKUTIL_H
#define UNP_INCLUDE_SOCKUTIL_H

#include<sys/socket.h>
#include <arpa/inet.h>

#include "unpbase.h"

typedef struct sockaddr SA;

int open_listen(int port);

#endif
