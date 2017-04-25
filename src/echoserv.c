#include "unpbase.h"
#include "sockutil.h"

#define MAXEVENTS 10

int doit(int fd) {
    char buf[MAXLINE];

    int n = read(fd, buf, MAXLINE);
    if (n <= 0) {
        return -1;
    }
    if (writen(fd, buf, n) != n) {
        error_sys("writen");
        return -1;
    }

    return 0;
}

int main(int argc, const char * argv[]) {
    int port = 1234;
    char *portEnv;
    if (argc > 1) {
        port = atoi(argv[1]);
    } else if ( (portEnv = getenv("LISTENP")) != NULL) {
        port = atoi(portEnv);
    }

    int serv_fd = open_listen(port);

    int epoll_fd = epoll_create(0xCAFE); // the param is ignored
    if (epoll_fd < 0 ) {
        fatal_sys("epoll_create");
    }

    struct epoll_event ev, events[MAXEVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = serv_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, serv_fd, &ev) < 0) {
        fatal_sys("epoll_ctl add serv");
    }

    for (;;) {
        int nfds = epoll_wait(epoll_fd, events, MAXEVENTS, -1);
        if (nfds < 0) {
            if (errno == EINTR) {
                continue;
            }
            fatal_sys("epoll_wait");
        }

        int i = 0;
        for (; i < nfds; i++) {
            if (events[i].data.fd == serv_fd) {
                int conn_fd = accept(serv_fd, NULL, NULL);
                if (conn_fd < 0) {
                    fatal_sys("accept");
                }
                ev.events = EPOLLIN;
                ev.data.fd = conn_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev) < 0) {
                    fatal_sys("epoll_ctl add conn");
                }
            } else {
                if (doit(events[i].data.fd) < 0) {
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL) < 0) {
                        fatal_sys("epoll_ctl del conn");
                    }
                    close(events[i].data.fd);
                }
            }
        }
    }

    return 0;
}
