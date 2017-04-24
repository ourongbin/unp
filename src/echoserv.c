#include "unpbase.h"
#include "sockutil.h"

void * doit(void *arg) {
    if (pthread_detach(pthread_self()) < 0) {
        error("pthread_detach");
    }
    int fd = (int)arg;
    char buff[MAXLINE];
    int  n;
    for (;;) {
        n = read(fd, buff, MAXLINE);
        if (n < 0 && errno == EINTR) {
            continue;
        } else if (n <= 0) {
            break;
        } 
        //printf("Recvd %s", buff);

        if (writen(fd, buff, strlen(buff)) < strlen(buff)) {
            error("write short");
        }
    }
    printf("Close connection\n");
    if (close(fd) < 0) {
        error_sys("close fd %d", fd);
    }
    return NULL;
}

int main(int argc, const char * argv[]) {
    int port = 1234;
    char * portEnv;
    if (argc > 1) {
        port = atoi(argv[1]);
    } else if ( (portEnv = getenv("LISTENP")) != NULL) {
        port = atoi(portEnv);
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

        pthread_t th;
        if (pthread_create(&th, NULL, doit, (void *)fd) < 0) {
            error("pthread_create");
        }
    }

    return 0;
}
