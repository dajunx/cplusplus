#include <iostream>
#include <string>

#include "tools.h"

int main() {
  worker w;
  w.chooseDatabase();
  w.run_thread();

  return 0;
}