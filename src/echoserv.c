#include "unpbase.h"
#include "sockutil.h"

int main(int argc, const char * argv[]) {
    int port = 1234;
    if (argc > 1) {
        port = atoi(argv[1]);
    }

    int sfd = open_listen(port);
    
    for (;;) {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        char ip[16];
        int fd = accept(sfd, (SA *)&peer, &len);
        if (fd < 0) {
            fatal_sys("accept:");
        } else {
            printf("New connection: %s:%d\n", inet_ntop(AF_INET, (void *)&peer.sin_addr, ip, sizeof(ip)), ntohs(peer.sin_port));
        }

        char buff[MAXLINE + 1];
        int  n;
        for (;;) {
            n = read(fd, buff, MAXLINE);
            if (n > 0) {
                buff[n] = '\0';
            } else {
                close(fd);
                printf("Close connection: %s:%d\n", inet_ntop(AF_INET, (void *)&peer.sin_addr, ip, sizeof(ip)), ntohs(peer.sin_port));
                break;
            }

            printf("Recvd %s", buff);

            if (write(fd, buff, strlen(buff)) != strlen(buff)) {
                error("write short");
            }
        }
    }

    return 0;
}
