#include "unpbase.h"
#include "sockutil.h"

void sigchld_handler(int sig) {
    int saved_errno = errno;
    while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {
        ;
    }
    errno = saved_errno;
}

void zombie_clean() {
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = sigchld_handler;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, NULL) < 0) {
        fatal_sys("sigaction");
    }
}

void doit(int fd, struct sockaddr_in peer, socklen_t len) {
    char buff[MAXLINE];
    int  n;
    char ip[16];
    for (;;) {
        n = readline(fd, buff, MAXLINE);
        if (n <= 0) {
            printf("Close connection: %s:%d\n", inet_ntop(AF_INET, (void *)&peer.sin_addr, ip, sizeof(ip)), ntohs(peer.sin_port));
            return;
        }

        printf("Recvd %s", buff);

        if (writen(fd, buff, strlen(buff)) < strlen(buff)) {
            error("write short");
        }
    }
}

int main(int argc, const char * argv[]) {
    int port = 1234;
    char * portEnv;
    if (argc > 1) {
        port = atoi(argv[1]);
    } else if ( (portEnv = getenv("LISTENP")) != NULL) {
        port = atoi(portEnv);
    }

    zombie_clean();

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


        pid_t child = fork();
        if (child == 0) {
            close(sfd);
            doit(fd, peer, len);
            if (close(fd) < 0) {
                error_sys("close fd %d", fd);
            }
            exit(0);
            // zombie if not waited
        }
        if (close(fd) < 0) {
            error_sys("close fd %d", fd);
        }
    }

    return 0;
}
