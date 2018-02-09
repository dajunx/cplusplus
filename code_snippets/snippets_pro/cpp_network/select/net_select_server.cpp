#include <unistd.h>
#include <arpa/inet.h>  /* inet(3) functions */
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#include <sys/select.h> /* select function*/
#include <sys/socket.h> /* basic socket definitions */
#include <sys/types.h>  /* basic system data types */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 10240

void handle(int *clientSockFds, int maxFds, fd_set *pRset, fd_set *pAllset,
            char *quit);

int main(int argc, char **argv) {
  int servPort = 6888;
  int listenq = 1024;

  int listenfd, connfd;
  struct sockaddr_in cliaddr, servaddr;
  socklen_t socklen = sizeof(struct sockaddr_in);
  int nready, nread;
  char buf[MAXLINE];
  int clientSockFds[FD_SETSIZE];
  fd_set allset, rset;
  int maxfd;

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0) {
    perror("socket error");
    return -1;
  }

  int opt = 1;
  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt error");
  }

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(servPort);

  if (bind(listenfd, (struct sockaddr *)&servaddr, socklen) == -1) {
    perror("bind error");
    exit(-1);
  }

  if (listen(listenfd, listenq) < 0) {
    perror("listen error");
    return -1;
  }

  int i = 0;
  for (i = 0; i < FD_SETSIZE; i++)
    clientSockFds[i] = -1;
  FD_ZERO(&allset);
  FD_SET(listenfd, &allset);
  maxfd = listenfd;

  printf("echo server use select startup, listen on port %d\n", servPort);
  printf("max connection: %d\n", FD_SETSIZE);

  char *quit = "quit";
  for (;;) {
    rset = allset;
    nready = select(maxfd + 1, &rset, NULL, NULL, NULL);
    if (nready < 0) {
      perror("select error");
      continue;
    }
    if (FD_ISSET(listenfd, &rset)) {
      connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &socklen);
      if (connfd < 0) {
        perror("accept error");
        continue;
      }

      sprintf(buf, "accept form %s:%d\n", inet_ntoa(cliaddr.sin_addr),
              cliaddr.sin_port);
      printf(buf, "");

      for (i = 0; i < FD_SETSIZE; i++) {
        if (clientSockFds[i] == -1) {
          clientSockFds[i] = connfd;
          break;
        }
      }
      if (i == FD_SETSIZE) {
        fprintf(stderr, "too many connection, more than %d\n", FD_SETSIZE);
        close(connfd);
        continue;
      }
      if (connfd > maxfd)
        maxfd = connfd;

      FD_SET(connfd, &allset);
      /* if (--nready <= 0) */
      /*     continue; */
    }

    handle(clientSockFds, maxfd, &rset, &allset, quit);
  }
}

void handle(int *clientSockFds, int maxFds, fd_set *pRset, fd_set *pAllset,
            char *quit) {
  int nread;
  int i;
  char buf[MAXLINE];
  for (i = 0; i < maxFds; i++) {
    if (clientSockFds[i] != -1) {
      if (FD_ISSET(clientSockFds[i], pRset)) {
        nread = read(clientSockFds[i], buf, MAXLINE); //读取客户端socket流
        if (nread < 0) {
          perror("read error");
          close(clientSockFds[i]);
          FD_CLR(clientSockFds[i], pAllset);
          clientSockFds[i] = -1;
          continue;
        }
        if (nread == 0) { // kill 可以引发这个case
          printf("client close the connection\n");
          close(clientSockFds[i]);
          FD_CLR(clientSockFds[i], pAllset);
          clientSockFds[i] = -1;
          continue;
        }
        if (strncmp(buf, quit, 4) ==
            0) { // socket传递过来的数据，除了输入的本身，还额外包含回车和换行2个字符
          printf("client close the connection 1\n");
          close(clientSockFds[i]);
          FD_CLR(clientSockFds[i], pAllset);
          clientSockFds[i] = -1;
          continue;
        }
        write(clientSockFds[i], buf, nread); //响应客户端  有可能失败，暂不处理
      }
    }
  }
}