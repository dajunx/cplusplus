/*
A regular pipe can only connect two related processes. It is created by a process and will vanish when the last process closes it.
A named pipe, also called a FIFO for its behavior, can be used to connect two unrelated processes and exists independently of the processes; meaning it can exist even if no one is using it. A FIFO is created using the mkfifo() library function.
*/
//writer.c

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
  int fd;
  char * myfifo = "/tmp/myfifo";

  /* create the FIFO (named pipe) */
  mkfifo(myfifo, 0666);

  /* write "Hi" to the FIFO */
  fd = open(myfifo, O_WRONLY);
  write(fd, "Hi", sizeof("Hi"));
  close(fd);

  /* remove the FIFO */
  unlink(myfifo);

  return 0;
}

//---------------------------------------------------------

  //reader.c
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_BUF 1024

  int main()
{
  int fd;
  char * myfifo = "/tmp/myfifo";
  char buf[MAX_BUF];

  /* open, read, and display the message from the FIFO */
  fd = open(myfifo, O_RDONLY);
  read(fd, buf, MAX_BUF);
  printf("Received: %s\n", buf);
  close(fd);

  return 0;
}