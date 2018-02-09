// va_start、va_end、va_list的使用 列子

#include <stdarg.h>
#include <stdio.h>

void simple_va_fun(int start, ...) {
  va_list arg_ptr;
  int nArgValue = start;
  int nArgCount = 0;
  va_start(arg_ptr, start);
  do {
    ++nArgCount;
    printf("the %d the arg:%d\n", nArgCount, nArgValue);
    nArgValue = va_arg(arg_ptr, int);
  } while (nArgValue != -1);
  return;
}

int writeMultiString(void *c, ...) {
  va_list ap;
  const char *str;

  va_start(ap, c);
  str = va_arg(ap, const char *);

  while (str != NULL) {
    fputs(str, (FILE *)c);
    str = va_arg(ap, const char *);
  }

  va_end(ap);
  return 0;
}

int main() {
  simple_va_fun(100, -1);
  simple_va_fun(100, 200, -1);

  return 0;
}