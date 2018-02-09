/*
  模拟业务服务多线程模型：
    主线程产生数据，供子线程使用
*/
#include <pthread.h>
#include <stdio.h>
#include <time.h>

typedef struct DATA {
  int index;
  int data;
} DATA_;

void *fun1(void *arg) {
  DATA_ *data = (DATA_ *)arg;
  for (;;) {
    printf("fun1 recive data[%d]: %d\n", data->index, data->data);
    sleep(1);
  }

  printf("fun1 print data finished\n");
  return 1;
}

int main() {
  pthread_t t1, t2, t3;
  void *res;
  int s;
  DATA_ data[3] = {{0, 0}, {1, 0}, {2, 0}};

  s = pthread_create(&t1, NULL, fun1, &data[0]);
  s = pthread_create(&t2, NULL, fun1, &data[1]);
  // s = pthread_create(&t3, NULL, fun1, &data);

  // 每秒钟修改data数据，供子线程读取使用
  // 模拟服务器收到客户端请求后的分发到工作线程处理
  for (int i = 0; i < 6; i++) {
    if (i / 2 == 0) {
      data[1].data++;
    } else {
      data[0].data++;
    }
    sleep(1);
  }

  // 结束线程
  s = pthread_cancel(t1);
  s = pthread_cancel(t2);

  s = pthread_join(t1, &res);
  s = pthread_join(t2, &res);
  printf("fun1 thread return succ, res:%ld\n", (long)res);

  return 0;
}