//__FILE__ 全路径、相对路径问题
/*
__FILE__
宏在debug模式下，h和cpp文件都是全路径；在release模式下，h是全路径、cpp是相对路径
*/

#include "common.h"
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char *argv[]) {
  show();
  std::string path = __FILE__;
  cout << "__FILE__ => " << path << endl;
  return 0;
}