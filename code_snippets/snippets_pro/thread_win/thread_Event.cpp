/*
  使用Event（事件） 实现父进程与子进程交互
*/
#include <Windows.h>
#include <iostream>
volatile long nCount;
#define THREAD_NUM 50
HANDLE g_hThreadEvent;
CRITICAL_SECTION g_csThreadCode;

DWORD WINAPI ThreadProc(LPVOID lpParameter) {
  int ThreadId = *(int *)lpParameter;
  SetEvent(g_hThreadEvent);

  EnterCriticalSection(&g_csThreadCode);
  nCount++;
  std::cout << "线程ID：" << ThreadId << ", 共享资源值：" << nCount
            << std::endl;
  LeaveCriticalSection(&g_csThreadCode);

  return 0L;
}

int main() {
  InitializeCriticalSection(&g_csThreadCode);
  g_hThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

  HANDLE thread[THREAD_NUM];
  for (int i = 0; i < THREAD_NUM;) {
    /*
      g_csThreadParameter 达不到效果，参见
          http://blog.csdn.net/morewindows/article/details/7442639 解释
    */
    thread[i] = CreateThread(NULL, 0, ThreadProc, &i, 0, NULL);
    // Event 解决线程ID 共享问题，临界区无法解决
    WaitForSingleObject(g_hThreadEvent, INFINITE);
    i++;
  }

  WaitForMultipleObjects(THREAD_NUM, thread, true, INFINITE);

  CloseHandle(g_hThreadEvent);
  DeleteCriticalSection(&g_csThreadCode);

  return 0;
}