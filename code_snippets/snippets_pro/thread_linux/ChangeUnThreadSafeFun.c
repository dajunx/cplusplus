/*
  使用 线程特有数据技术实现函数线程安全
*/
#define _GNU_SOURCE /* Get '_sys_nerr' and '_sys_errlist'                      \
                       declarations from <stdio.h> */
#include <pthread.h>
#include <stdio.h>
#include <string.h> /* Get declaration of strerror() */

#define MAX_ERROR_LEN                                                          \
  256 /* Maximum length of string                                              \
         returned by strerror() */

static char buf[MAX_ERROR_LEN]; /* Statically allocated return buffer */
static char *str[3] = {"111", "222", "NULL"};
static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t strerrorKey;

static void destructor(void *buf) { free(buf); }

static void createKey(void) {
  int s;

  /* Allocate a unique thread-specific data key and save the address
     of the destructor for thread-specific data buffers */

  s = pthread_key_create(&strerrorKey, destructor);
  if (s != 0)
    printf("pthread_key_create \n");
}
char *strerror_safe_version(int err) {
  int s;
  char *buf;

  /* Make first caller allocate key for thread-specific data */

  s = pthread_once(&once, createKey);
  if (s != 0)
    printf("pthread_once\n");

  buf = pthread_getspecific(strerrorKey);
  if (buf == NULL) { /* If first call from this thread, allocate
                        buffer for thread, and save its location */
    buf = malloc(MAX_ERROR_LEN);
    if (buf == NULL)
      printf("malloc\n");

    s = pthread_setspecific(strerrorKey, buf);
    if (s != 0)
      printf("pthread_setspecific\n");
  }

  if (err == 0) {
    strncpy(buf, str[err], 3);
    printf("strerror %d, str:%s\n.", err, str[err]);
    buf[4] = '\0';
  } else if (err == 1) {
    strncpy(buf, str[err], 3);
    printf("strerror %d, str:%s\n.", err, str[err]);
    buf[4] = '\0';
  } else {
    strncpy(buf, str[err], 4);
    printf("strerror %d, str:%s\n.", err, str[err]);
    buf[5] = '\0';
  }

  return buf;
}

char *strerror_unsafe_version(int err) {
  if (err == 0) {
    strncpy(buf, str[err], 3);
    printf("strerror %d, str:%s\n.", err, str[err]);
    buf[4] = '\0';
  } else if (err == 1) {
    strncpy(buf, str[err], 3);
    printf("strerror %d, str:%s\n.", err, str[err]);
    buf[4] = '\0';
  } else {
    strncpy(buf, str[err], 4);
    printf("strerror %d, str:%s\n.", err, str[err]);
    buf[5] = '\0';
  }

  return buf;
}

static void *threadFunc(void *arg) {
  char *str;

  printf("Other thread about to call strerror()\n");
  str = strerror_safe_version(1);
  printf("Other thread: str (%p) = %s\n", str, str);

  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t t;
  int s;
  char *str;

  str = strerror_safe_version(0);
  printf("Main thread has called strerror()\n");

  s = pthread_create(&t, NULL, threadFunc, NULL);
  if (s != 0)
    printf("pthread_create\n");

  s = pthread_join(t, NULL);
  if (s != 0)
    printf("pthread_join\n");

  /* If strerror() is not thread-safe, then the output of this printf() be
     the same as that produced by the analogous printf() in threadFunc() */

  printf("Main thread:  str (%p) = %s\n", str, str);

  return 0;
}
