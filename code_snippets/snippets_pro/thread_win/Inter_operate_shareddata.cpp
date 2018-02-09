/*
  使用原子操作函数来操作 线程共享数据
*/
#include <Windows.h>
#include <iostream>
volatile long nCount;

DWORD WINAPI ThreadProc(LPVOID lpParameter) {
  for (int i = 0; i < 100000; ++i) {
    InterlockedIncrement((LPLONG)&nCount);
  }

  return 0L;
}

int main() {
  HANDLE thread[2];
  for (int i = 0; i < 2; i++) {
    thread[i] = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
  }

  WaitForMultipleObjects(2, thread, true, INFINITE);

  return 0;
}