#ifndef ANET_H
#define ANET_H

#define ANET_OK 0
#define ANET_ERR -1
#define ANET_ERR_LEN 256

int anetTcpConnect(char *err, char *addr, int port); // no use
int anetResolve(char *err, char *host, char *ipbuf); // no use
int anetWrite(int fd, void *buf, int count); // no use
int anetTcpKeepAlive(char *err, int fd); // no use
int anetRead(int fd, void *buf, int count); // no use

int anetTcpServer(char *err, int port, char *bindaddr); // 1
int anetAccept(char *err, int serversock, char *ip, int *port); // 2
int anetNonBlock(char *err, int fd);
int anetTcpNoDelay(char *err, int fd);


#endif
