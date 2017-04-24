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
    addr.sin_addr.s_addr= htonl(INADDR_ANY);
    if (bind(sfd, (SA *)&addr, sizeof(addr)) < 0) {
        fatal_sys("bind %d", port);
    }

    if (listen(sfd, 1024) < 0) {
        fatal_sys("listen");
    }

    return sfd;
}

ssize_t writen(int fd, const void *vptr, size_t n) {
	size_t		nleft;
	ssize_t		nwritten;
	const char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;		/* and call write() again */
			else
				return -1;			/* error */
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return n;
}

ssize_t readn(int fd, void *vptr, size_t n) {
	size_t	nleft;
	ssize_t	nread;
	char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nread = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR)
				nread = 0;		/* and call read() again */
			else
				return -1;
		} else if (nread == 0)
			break;				/* EOF */

		nleft -= nread;
		ptr   += nread;
	}
	return n - nleft;		/* return >= 0 */
}
