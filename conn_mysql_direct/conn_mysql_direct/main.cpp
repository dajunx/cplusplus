#include <iostream>
#include <string>

#include "tools.h"

int main() {
  worker w;
  w.chooseDatabase();
  w.execute_sql();

  return 0;
}