#include <unistd.h>
#include <arpa/inet.h>  /* inet(3) functions */
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#include <sys/socket.h> /* basic socket definitions */
#include <sys/types.h>  /* basic system data types */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <limits.h>
#include <poll.h> /* poll function */

#define MAXLINE 10240

#ifndef OPEN_MAX
#define OPEN_MAX 40960
#endif
void handle(struct pollfd *clients, int maxClient, int readyClient);

int main(int argc, char **argv) {
  int servPort = 88960;
  int listenq = 1024;
  int listenfd, connfd;
  struct pollfd clients[OPEN_MAX];
  int maxi;
  socklen_t socklen = sizeof(struct sockaddr_in);
  struct sockaddr_in cliaddr, servaddr;
  char buf[MAXLINE];
  int nready;

  bzero(&servaddr, socklen);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(servPort);

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0) {
    perror("socket error");
  }

  int opt = 1;
  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt error");
  }

  if (bind(listenfd, (struct sockaddr *)&servaddr, socklen) == -1) {
    perror("bind error");
    exit(-1);
  }
  if (listen(listenfd, listenq) < 0) {
    perror("listen error");
  }

  clients[0].fd = listenfd;
  clients[0].events = POLLIN;
  int i;
  for (i = 1; i < OPEN_MAX; i++)
    clients[i].fd = -1;
  maxi = listenfd + 1;

  printf("pollechoserver startup, listen on port:%d\n", servPort);
  printf("max connection is %d\n", OPEN_MAX);

  for (;;) {
    nready = poll(clients, maxi + 1, -1);
    // printf("nready is %d,%d\n", nready, maxi);
    if (nready == -1) {
      perror("poll error");
    }
    if (clients[0].revents & POLLIN) {
      connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &socklen);
      sprintf(buf, "accept form %s:%d\n", inet_ntoa(cliaddr.sin_addr),
              cliaddr.sin_port);
      printf(buf, "");

      for (i = 0; i < OPEN_MAX; i++) {
        if (clients[i].fd == -1) {
          clients[i].fd = connfd;
          clients[i].events = POLLIN;
          break;
        }
      }

      if (i == OPEN_MAX) {
        fprintf(stderr, "too many connection, more than %d\n", OPEN_MAX);
        close(connfd);
        continue;
      }

      if (i > maxi)
        maxi = i;

      /* --nready; */
    }

    handle(clients, maxi, nready);
  }
}

void handle(struct pollfd *clients, int maxClient, int nready) {
  int connfd;
  int i, nread;
  char buf[MAXLINE];

  if (nready == 0)
    return;

  for (i = 1; i < maxClient; i++) {
    connfd = clients[i].fd;
    if (connfd == -1)
      continue;
    if (clients[i].revents & (POLLIN | POLLERR)) {
      nread = read(connfd, buf, MAXLINE); //读取客户端socket流
      if (nread < 0) {
        perror("read error");
        close(connfd);
        clients[i].fd = -1;
        continue;
      }
      if (nread == 0) {
        printf("client close the connection");
        close(connfd);
        clients[i].fd = -1;
        continue;
      }
      printf("read data '%s'", buf); // peterlin

      // write(connfd, buf, nread);//响应客户端
      write(connfd, "server res", nread); //响应客户端
      if (--nready <= 0) //没有连接需要处理，退出循环
        break;
    }
  }
}