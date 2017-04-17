#include "unpbase.h"
#include "sockutil.h"

int open_listen(int port) {
    int optval = 1;
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) {
        fatal_sys("socket");
    }
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int)) < 0) {
        fatal_sys("socket");
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr= INADDR_ANY;
    if (bind(sfd, (SA *)&addr, sizeof(addr)) < 0) {
        fatal_sys("bind %d", port);
    }

    if (listen(sfd, 2) < 0) {
        fatal_sys("listen");
    }

    return sfd;
}
