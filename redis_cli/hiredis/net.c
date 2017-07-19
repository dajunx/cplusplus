/* Extracted from anet.c to work properly with Hiredis error reporting.
 *
 * Copyright (c) 2006-2011, Salvatore Sanfilippo <antirez at gmail dot com>
 * Copyright (c) 2010-2011, Pieter Noordhuis <pcnoordhuis at gmail dot com>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "fmacros.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#ifdef WIN32
#include <stddef.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <poll.h>
#include <limits.h>
#endif
#include "net.h"
#include "sds.h"

#ifdef WIN32
#pragma warning(disable: 4996)
#else
#endif

#ifdef WIN32
static void win_strerror(int err, char* buf, size_t len) {
	LPSTR s = NULL;

	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
		NULL, err,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&s, 0, NULL);
	strncpy(buf, s, len);
	LocalFree(s);
}
#endif

/* Defined in hiredis.c */
void __redisSetError(redisContext *c, int type, const char *str);

static void __redisSetErrorFromErrno(redisContext *c, int type, const char *prefix) {
#ifdef WIN32
#define os_snprintf _snprintf
#else
#define os_snprintf snprintf
#endif

    char buf[128] = { 0 };
    size_t len = 0;

    if (prefix != NULL)
        len = os_snprintf(buf,sizeof(buf),"%s: ",prefix);
    
#ifdef WIN32
	win_strerror(errno, buf+len,sizeof(buf)-len);
#else
	strerror_r(errno,buf+len,sizeof(buf)-len);
#endif
    __redisSetError(c,type,buf);
}

static int redisSetReuseAddr(redisContext *c, int fd) {
#ifdef WIN32
	#define os_type char
#else
	#define os_type void
#endif
    int on = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (os_type *)&on, sizeof(on)) == -1) {
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
#ifdef WIN32
        closesocket(fd);
#else
		close(fd);
#endif
        return REDIS_ERR;
    }
    return REDIS_OK;
}

static int redisCreateSocket(redisContext *c, int type) {
    int s;
    if ((s = socket(type, SOCK_STREAM, 0)) == -1) {
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
        return REDIS_ERR;
    }
    if (type == AF_INET) {
        if (redisSetReuseAddr(c,s) == REDIS_ERR) {
            return REDIS_ERR;
        }
    }
    return s;
}

int redisSetBlocking(redisContext *c, int fd, int blocking) {
#ifdef WIN32
	u_long sockopt = blocking ? 0 : 1;
	if (ioctlsocket(fd, FIONBIO, &sockopt) != 0) {
		c->err = WSAGetLastError();
		__redisSetErrorFromErrno(c,REDIS_ERR_IO,"ioctlsocket(FIONBIO)");
		closesocket(fd);
		return REDIS_ERR;
	}
#else
    int flags;

    /* Set the socket nonblocking.
     * Note that fcntl(2) for F_GETFL and F_SETFL can't be
     * interrupted by a signal. */
    if ((flags = fcntl(fd, F_GETFL)) == -1) {
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,"fcntl(F_GETFL)");
        close(fd);
        return REDIS_ERR;
    }

    if (blocking)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flags) == -1) {
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,"fcntl(F_SETFL)");
        close(fd);
        return REDIS_ERR;
    }
#endif
    return REDIS_OK;
}

int redisKeepAlive(redisContext *c, int interval) {
#ifdef WIN32
#define os_type char
#else
#define os_type void
#endif

    int val = 1;
    int fd = c->fd;
#ifdef WIN32
	struct tcp_keepalive alive_in; 
	struct tcp_keepalive alive_out;
	unsigned long ulBytesReturn = 0;
	int nRet = 0;
#endif

    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (os_type*)&val, sizeof(val)) == -1){
        __redisSetError(c,REDIS_ERR_OTHER,strerror(errno));
        return REDIS_ERR;
    }

#ifdef _OSX
    val = interval;
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPALIVE, &val, sizeof(val)) < 0) {
        __redisSetError(c,REDIS_ERR_OTHER,strerror(errno));
        return REDIS_ERR;
    }
#elif WIN32
	alive_in.keepalivetime	= 10000;
	alive_in.keepaliveinterval  = interval;
	alive_in.onoff	= 1;

	nRet = WSAIoctl(fd, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in),  
		&alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL);  
	if (nRet == SOCKET_ERROR)  {   
		__redisSetError(c,REDIS_ERR_OTHER,"can't set socket keepalive");
		return REDIS_ERR; 
	}
#else
    val = interval;
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &val, sizeof(val)) < 0) {
        __redisSetError(c,REDIS_ERR_OTHER,strerror(errno));
        return REDIS_ERR;
    }

    val = interval/3;
    if (val == 0) val = 1;
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &val, sizeof(val)) < 0) {
        __redisSetError(c,REDIS_ERR_OTHER,strerror(errno));
        return REDIS_ERR;
    }

    val = 3;
    if (setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &val, sizeof(val)) < 0) {
        __redisSetError(c,REDIS_ERR_OTHER,strerror(errno));
        return REDIS_ERR;
    }
#endif
    return REDIS_OK;
}

int redisSetTcpNoDelay(redisContext *c, int fd) {
#ifdef WIN32
	#define os_type char
#else
	#define os_type void
#endif
    int yes = 1;
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (os_type *)&yes, sizeof(yes)) == -1) {
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,"setsockopt(TCP_NODELAY)");
#ifdef WIN32
        closesocket(fd);
#else
		close(fd);
#endif
        return REDIS_ERR;
    }
    return REDIS_OK;
}

#define __MAX_MSEC (((LONG_MAX) - 999) / 1000)

int redisContextWaitReady(redisContext *c, int fd, const struct timeval *timeout) {
#ifdef WIN32
    struct timeval to;
    struct timeval *toptr = NULL;
    fd_set wfd;
#else
    struct pollfd   wfd[1];
    long msec;

    msec          = -1;
    wfd[0].fd     = fd;
    wfd[0].events = POLLOUT;
#endif

    /* Only use timeout when not NULL. */
    if (timeout != NULL) {
        if (timeout->tv_usec > 1000000 || timeout->tv_sec > __MAX_MSEC) {
			__redisSetErrorFromErrno(c, REDIS_ERR_IO, NULL);
#ifdef WIN32
            closesocket(fd);
#else
            close(fd);
#endif
            return REDIS_ERR;
        }

#ifdef WIN32
        to = *timeout;
        toptr = &to;
#else
        msec = (timeout->tv_sec * 1000) + ((timeout->tv_usec + 999) / 1000);

        if (msec < 0 || msec > INT_MAX) {
            msec = INT_MAX;
        }
#endif
    }

#ifdef WIN32
    {
        FD_ZERO(&wfd);
        FD_SET(fd, &wfd);

        if (select(FD_SETSIZE, NULL, &wfd, NULL, toptr) == -1) {
            __redisSetErrorFromErrno(c,REDIS_ERR_IO,"select(2)");
            closesocket(fd);
            return REDIS_ERR;
		}

        if (!FD_ISSET(fd, &wfd)) {
            errno = WSAETIMEDOUT;
#else
    if (errno == EINPROGRESS) {

        int res;

        if ((res = poll(wfd, 1, msec)) == -1) {
            __redisSetErrorFromErrno(c, REDIS_ERR_IO, "poll(2)");
            close(fd);
            return REDIS_ERR;
        } else if (res == 0) {
            errno = ETIMEDOUT;
#endif
            __redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
#ifdef WIN32
        	closesocket(fd);
#else
			close(fd);
#endif
            return REDIS_ERR;
        }

        if (redisCheckSocketError(c, fd) != REDIS_OK)
            return REDIS_ERR;

        return REDIS_OK;
    }

    __redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
#ifdef WIN32
	closesocket(fd);
#else
	close(fd);
#endif
    return REDIS_ERR;
}

int redisCheckSocketError(redisContext *c, int fd) {
#ifdef WIN32
	#define os_type char
	#define os_len int
#else
	#define os_type void
	#define os_len socklen_t
#endif

	int err = 0;	
	os_len errlen = sizeof(err);
	errno = 0;
    if (getsockopt(fd, SOL_SOCKET, SO_ERROR, (os_type *)&err, &errlen) == -1) {
#ifdef WIN32
		errno = WSAGetLastError();
#endif
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,"getsockopt(SO_ERROR)");
#ifdef WIN32
        closesocket(fd);
#else
		close(fd);
#endif
        return REDIS_ERR;
    }

    if (err) {
        errno = err;
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
#ifdef WIN32
        closesocket(fd);
#else
		close(fd);
#endif
        return REDIS_ERR;
    }

    return REDIS_OK;
}

int redisContextSetTimeout(redisContext *c, const struct timeval tv) {
#ifdef WIN32
	#define os_type char	
#else
	#define os_type void
#endif

    if (setsockopt(c->fd,SOL_SOCKET,SO_RCVTIMEO,(os_type *)&tv,sizeof(tv)) == -1) {
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,"setsockopt(SO_RCVTIMEO)");
        return REDIS_ERR;
    }
    if (setsockopt(c->fd,SOL_SOCKET,SO_SNDTIMEO,(os_type *)&tv,sizeof(tv)) == -1) {
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,"setsockopt(SO_SNDTIMEO)");
        return REDIS_ERR;
    }
    return REDIS_OK;
}

int redisContextConnectTcp(redisContext *c, const char *addr, int port, const struct timeval *timeout) {
#ifdef WIN32
    int s, rv;
	unsigned long inaddr;
	struct sockaddr_in sockaddr;
    int blocking = (c->flags & REDIS_BLOCK);

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;

	inaddr = inet_addr(addr);
	if (inaddr != INADDR_NONE)
		memcpy(&sockaddr.sin_addr, &inaddr, sizeof(inaddr));
	sockaddr.sin_port = htons(port);

    if ((s = socket(AF_INET,SOCK_STREAM,0)) == -1)
        goto error;

    if (redisSetBlocking(c,s,0) != REDIS_OK)
        goto error;

    if (connect(s, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1) {
		errno = WSAGetLastError();
        if (errno == WSAEINPROGRESS && !blocking) {
            /* This is ok. */
        } else {
			if (redisContextWaitReady(c,s,timeout) != REDIS_OK)
				goto error;
        }
    }

	if (blocking && redisSetBlocking(c,s,1) != REDIS_OK)
		goto error;

	if (redisSetTcpNoDelay(c,s) != REDIS_OK)
		goto error;

    c->fd = s;
    c->flags |= REDIS_CONNECTED;
    rv = REDIS_OK;
    goto end;

error:
    rv = REDIS_ERR;
end:
    return rv;  // Need to return REDIS_OK if alright
#else
    int s, rv;
    char _port[6];  /* strlen("65535"); */
    struct addrinfo hints, *servinfo, *p;
    int blocking = (c->flags & REDIS_BLOCK);

    snprintf(_port, 6, "%d", port);
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    /* Try with IPv6 if no IPv4 address was found. We do it in this order since
     * in a Redis client you can't afford to test if you have IPv6 connectivity
     * as this would add latency to every connect. Otherwise a more sensible
     * route could be: Use IPv6 if both addresses are available and there is IPv6
     * connectivity. */
    if ((rv = getaddrinfo(addr,_port,&hints,&servinfo)) != 0) {
         hints.ai_family = AF_INET6;
         if ((rv = getaddrinfo(addr,_port,&hints,&servinfo)) != 0) {
            __redisSetError(c,REDIS_ERR_OTHER,gai_strerror(rv));
            return REDIS_ERR;
        }
    }
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((s = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) == -1)
            continue;

        if (redisSetBlocking(c,s,0) != REDIS_OK)
            goto error;
        if (connect(s,p->ai_addr,p->ai_addrlen) == -1) {
            if (errno == EHOSTUNREACH) {
                close(s);
                continue;
            } else if (errno == EINPROGRESS && !blocking) {
                /* This is ok. */
            } else {
                if (redisContextWaitReady(c,s,timeout) != REDIS_OK)
                    goto error;
            }
        }
        if (blocking && redisSetBlocking(c,s,1) != REDIS_OK)
            goto error;
        if (redisSetTcpNoDelay(c,s) != REDIS_OK)
            goto error;

        c->fd = s;
        c->flags |= REDIS_CONNECTED;
        rv = REDIS_OK;
        goto end;
    }
    if (p == NULL) {
        char buf[128];
        snprintf(buf,sizeof(buf),"Can't create socket: %s",strerror(errno));
        __redisSetError(c,REDIS_ERR_OTHER,buf);
        goto error;
    }

error:
    rv = REDIS_ERR;
end:
    freeaddrinfo(servinfo);
    return rv;  // Need to return REDIS_OK if alright
#endif
}

#ifdef WIN32
#else
int redisContextConnectUnix(redisContext *c, const char *path, const struct timeval *timeout) {
    int s;
    int blocking = (c->flags & REDIS_BLOCK);
    struct sockaddr_un sa;

    if ((s = redisCreateSocket(c,AF_LOCAL)) < 0)
        return REDIS_ERR;
    if (redisSetBlocking(c,s,0) != REDIS_OK)
        return REDIS_ERR;

    sa.sun_family = AF_LOCAL;
    strncpy(sa.sun_path,path,sizeof(sa.sun_path)-1);
    if (connect(s, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
        if (errno == EINPROGRESS && !blocking) {
            /* This is ok. */
        } else {
            if (redisContextWaitReady(c,s,timeout) != REDIS_OK)
                return REDIS_ERR;
        }
    }

    /* Reset socket to be blocking after connect(2). */
    if (blocking && redisSetBlocking(c,s,1) != REDIS_OK)
        return REDIS_ERR;

    c->fd = s;
    c->flags |= REDIS_CONNECTED;
    return REDIS_OK;
}
#endif
