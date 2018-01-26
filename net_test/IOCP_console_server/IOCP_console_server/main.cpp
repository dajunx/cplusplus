#include <iostream>
#include "IOCPModel.h"

CIOCPModel io;

DWORD WINAPI startNet(LPVOID lpParameter)
{
  io.InitServer();

  return 0L;
}

DWORD WINAPI stopNet(LPVOID lpParameter)
{
  while(true)
  {
    Sleep(1000);
    //io.Stop();
  }
  io.UnloadSocketLib();

  return 0L;
}

int main()
{
  io.LoadSocketLib();

  HANDLE thread[2];
  thread[0] = CreateThread(NULL, 0, startNet, NULL, 0, NULL);
  thread[1] = CreateThread(NULL, 0, stopNet, NULL, 0, NULL);

  WaitForMultipleObjects(2, thread, true, INFINITE);
  return 0;
}