/*
  使用信号量 解决父子线程的共享数据问题
*/
#include <Windows.h>
#include <iostream>
volatile long nCount;
#define THREAD_NUM 50
HANDLE g_hThreadSemaphore;
CRITICAL_SECTION g_csThreadCode;

DWORD WINAPI ThreadProc(LPVOID lpParameter) {
  int ThreadId = *(int *)lpParameter;
  // 信号量加1
  ReleaseSemaphore(g_hThreadSemaphore, 1, NULL);

  EnterCriticalSection(&g_csThreadCode);
  nCount++;
  std::cout << "线程ID：" << ThreadId << ", 共享资源值：" << nCount
            << std::endl;
  LeaveCriticalSection(&g_csThreadCode);

  return 0L;
}

int main() {
  InitializeCriticalSection(&g_csThreadCode);
  // 当前0个资源，最大允许1个同时访问
  g_hThreadSemaphore = CreateSemaphore(NULL, 0, 1, NULL);

  HANDLE thread[THREAD_NUM];
  for (int i = 0; i < THREAD_NUM;) {
    /*
      g_csThreadParameter 达不到效果，参见
          http://blog.csdn.net/morewindows/article/details/7442639 解释
    */
    thread[i] = CreateThread(NULL, 0, ThreadProc, &i, 0, NULL);
    // 信号量 解决线程ID 在父子线程中的共享问题，临界区无法解决
    WaitForSingleObject(g_hThreadSemaphore, INFINITE);
    i++;
  }

  WaitForMultipleObjects(THREAD_NUM, thread, true, INFINITE);

  CloseHandle(g_hThreadSemaphore);
  DeleteCriticalSection(&g_csThreadCode);

  return 0;
}