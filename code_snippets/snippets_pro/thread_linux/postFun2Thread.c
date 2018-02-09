/*
  测试，线程用作串行队列，用来处理投递给子线程的任务请求
*/
#include <deque>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
typedef void (*FUN)(int);
static std::deque<FUN> MsgDeq; //共享消息队列
std::vector<int> vec_thread_datas;
static bool post_event_finished = false;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

class c_mutex {
public:
  c_mutex() { pthread_mutex_lock(&mutex); }
  ~c_mutex() { pthread_mutex_unlock(&mutex); }
};
// loop线程
void *fun2(void *arg) {
  std::deque<FUN>::iterator it;
  int input_cb_data = 0;
  while (true) {
    pthread_mutex_lock(&mutex);
    // 条件变量使用例子，
    // 条件变量的使用目的也是避免消费者平凡去尝试加锁查看是否又新数据到来
    while (MsgDeq.empty()) {
      pthread_cond_wait(&cond, &mutex);
    }

    it = MsgDeq.begin();
    (*(*it))(input_cb_data++);
    MsgDeq.pop_front();
    pthread_mutex_unlock(&mutex);

    if (post_event_finished == true && true == MsgDeq.empty()) {
      break;
    }
  }
  return (void *)0;
}

void *fun1(void *arg) {
  std::deque<FUN>::iterator it;
  int input_cb_data = 0;
  while (true) {
    if (false == MsgDeq.empty()) {
      c_mutex t;
      it = MsgDeq.begin();
      (*(*it))(input_cb_data++);
      MsgDeq.pop_front();
    } else {
      // printf("thread empty loop\n");
    }
    if (post_event_finished == true && true == MsgDeq.empty()) {
      break;
    }
  }
  return (void *)0;
}

void show_callback(int i) { vec_thread_datas.push_back(i); }
void post() { MsgDeq.push_back(&show_callback); }

int main() {
  pthread_t t1, t2;
  void *res;
  int s;

  s = pthread_create(&t1, NULL, fun2, NULL);
  // s = pthread_create(&t2, NULL, fun2, NULL);

  // 在主线程投递请求
  for (int i = 0; i < 1000000; i++) {
    c_mutex t;
    pthread_cond_signal(&cond);
    post();
  }
  post_event_finished = true;

  s = pthread_join(t1, &res);
  // s = pthread_join(t2, &res);

  // 打印线程处理完的归总数据
  std::vector<int>::iterator it = vec_thread_datas.begin();
  printf("vector size:%d\n", vec_thread_datas.size());
  for (int i = 0; it != vec_thread_datas.end(); it++, i++) {
    if (i != *it) {
      std::cout << "pos:" << i << " data" << *it << std::endl;
    }
  }

  return 0;
}