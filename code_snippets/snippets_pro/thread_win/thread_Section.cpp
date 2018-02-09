/*
  使用共享段（临界区） 实现多个线程共享数据
*/
#include <Windows.h>
#include <iostream>
volatile long nCount;
#define THREAD_NUM 50
CRITICAL_SECTION g_csThreadParameter, g_csThreadCode;

DWORD WINAPI ThreadProc(LPVOID lpParameter) {
  int ThreadId = *(int *)lpParameter;
  LeaveCriticalSection(&g_csThreadParameter);

  EnterCriticalSection(&g_csThreadCode);
  nCount++;
  std::cout << "线程ID：" << ThreadId << ", 共享资源值：" << nCount
            << std::endl;
  LeaveCriticalSection(&g_csThreadCode);

  return 0L;
}

int main() {
  InitializeCriticalSection(&g_csThreadParameter);
  InitializeCriticalSection(&g_csThreadCode);

  HANDLE thread[THREAD_NUM];
  for (int i = 0; i < THREAD_NUM;) {
    /*
      g_csThreadParameter 达不到效果，参见
          http://blog.csdn.net/morewindows/article/details/7442639 解释
    */
    EnterCriticalSection(&g_csThreadParameter);
    thread[i] = CreateThread(NULL, 0, ThreadProc, &i, 0, NULL);
    i++;
  }

  WaitForMultipleObjects(THREAD_NUM, thread, true, INFINITE);

  return 0;
}