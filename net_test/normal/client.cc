#include  <unistd.h>
#include  <sys/types.h>       /* basic system data types */
#include  <sys/socket.h>      /* basic socket definitions */
#include  <netinet/in.h>      /* sockaddr_in{} and other Internet defns */
#include  <arpa/inet.h>       /* inet(3) functions */
#include <netdb.h> /*gethostbyname function */

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <iostream>

#define MAXLINE 1024

void handle(int connfd);

int main(int argc, char **argv)
{
    char * servInetAddr = (char*)malloc(10);
    sprintf(servInetAddr,"%s","127.0.0.1");
    int servPort = 6888;
    char buf[MAXLINE];
    int connfd;
    struct sockaddr_in servaddr;

    connfd = socket(AF_INET, SOCK_STREAM, 0);

    //bzero(&servaddr, sizeof(servaddr));
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(servPort);
    inet_pton(AF_INET, servInetAddr, &servaddr.sin_addr);

    if (connect(connfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("connect error");
        return -1;
    }
    printf("welcome to echoclient\n");

    //向服务器循环写入数据
    std::string strSrcData;
    int ret_send = 0;
    while (true) {
      std::string strSend;
      if (strSrcData.compare("----------") == 0) {
        strSrcData.clear();
      }
      strSend = strSrcData + "\n";
      ret_send = write(connfd, strSend.c_str(), strSend.size());
      if (ret_send == 0) {
        std::cout<<"server child err:"<<errno<<std::endl;
        break;
      }
      strSrcData.append("-");
      usleep(200000);
    }
    
    close(connfd);
    return 0;
}
