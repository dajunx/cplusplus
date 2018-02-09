/*
  简单的windows平台创建线程和关闭线程以及传递参数
*/
#include <Windows.h>
#include <iostream>
#include <process.h> // _beginthreadex head file
HANDLE g_hMutex1 = NULL;
static int threadFinished = 0;

DWORD WINAPI ThreadProc(LPVOID lpParameter) {
  int *p_data = (int *)lpParameter;
  int tmp = 0;
  for (int i = 0; i < 100; ++i) {
    WaitForSingleObject(g_hMutex1, INFINITE);
    tmp = *p_data;
    tmp++;
    *p_data = tmp;
    ReleaseMutex(g_hMutex1);
  }

  threadFinished++;
  std::cout << "child thread finished." << std::endl;
  return 0L;
}

unsigned int __stdcall ThreadFun(PVOID pm) {
  std::cout << "_beginthreadex call." << std::endl;
  return 0;
}

int main() {
  int ThreadInputData = 0;
  g_hMutex1 = CreateMutex(NULL, false, NULL);
  HANDLE thread1 = CreateThread(NULL, 0, ThreadProc, &ThreadInputData, 0, NULL);
  HANDLE thread2 = CreateThread(NULL, 0, ThreadProc, &ThreadInputData, 0, NULL);

  WaitForSingleObject(thread1, INFINITE);
  WaitForSingleObject(thread2, INFINITE);
  /*
    //等待线程结束的 低效率办法
  while(threadFinished != 2) {
    Sleep(1000);
  }
  */
  CloseHandle(thread1);
  CloseHandle(thread2);

  // 使用 _beginthreadex
  // 函数创建线程，避免线程函数调用c函数产生的共享数据安全问题；
  HANDLE thread3 = (HANDLE)_beginthreadex(NULL, 0, ThreadFun, NULL, 0, NULL);
  WaitForSingleObject(thread3, INFINITE);
  CloseHandle(thread3);

  return 0;
}