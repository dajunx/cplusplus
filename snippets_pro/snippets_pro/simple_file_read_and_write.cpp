//简单的文件读写

#include <iostream>
#include <string>
#include <fstream>

void write()
{
  std::fstream file("file.txt",
    std::fstream::in | std::fstream::out | std::fstream::app);
  file<<"lin";
  file.close();
}

void read()
{
  std::string str;
  std::fstream file("file.txt", std::fstream::in);
  file>>str;
  std::cout<<str;
  file.close();
}

int main()
{
  write();
  read();
  return 0;
}