#include  <unistd.h>
#include  <sys/types.h>       /* basic system data types */
#include  <sys/socket.h>      /* basic socket definitions */
#include  <netinet/in.h>      /* sockaddr_in{} and other Internet defns */
#include  <arpa/inet.h>       /* inet(3) functions */

#include <pthread.h>

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE 1024

static void *HandleClientMsg(void *arg) {
  int* pConnfd = (int*)arg;

  size_t  n;
  char  buf[MAXLINE];

  while(1) {
    n = read(*pConnfd, buf, MAXLINE);
    if (n < 0) {
      if(errno != EINTR) {
        perror("read error");
        break;
      }
    } else if (n == 0) {
      //connfd is closed by client
      close(*pConnfd);
      printf("client exit\n");
      break;
    }

    //client exit
    if (strncmp("exit", buf, 4) == 0) {
      close(*pConnfd);
      printf("client exit\n");
      break;
    }

    buf[n] = 0x00;
    printf("%s \n", buf);
    //write(connfd, buf, n); //write maybe fail,here don't process failed error
  }

  close(*pConnfd);    /* close listening socket */
  return 0;
}

int  main(int argc, char **argv)
{
    int     listenfd, connfd;
    int  serverPort = 6888;
    int listenq = 1024;
    pid_t   childpid;
    char buf[MAXLINE];
    socklen_t socklen;
    pthread_t thread1;

    struct sockaddr_in cliaddr, servaddr;
    socklen = sizeof(cliaddr);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(serverPort);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("socket error");
        return -1;
    }
    if (bind(listenfd, (struct sockaddr *) &servaddr, socklen) < 0) {
        perror("bind error");
        return -1;
    }
    if (listen(listenfd, listenq) < 0) {
        perror("listen error");
        return -1;
    }
    printf("echo server startup,listen on port:%d\n", serverPort);
    while(1) {
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &socklen);
        if (connfd < 0) {
            perror("accept error");
            continue;
        }

        //使用线程代替进程负责数据接收
        int s = pthread_create(&thread1, NULL, HandleClientMsg, &connfd);
        if (s != 0) {
          printf("pthread_create err.\n");
        }
    }

    return 0;
}