/* c-sytle io test
  c风格io测试，如下是 测试刷新缓冲区测试
  1.若没有 fflush(stdin)，结果如下
    输入 123abc回车
    显示 a = 123, c = a
  2.若有 fflush(stdin)，结果如下
    输入 123abc回车
        xyz回车
    输出 a = 123, c = x

*/
#include <stdio.h>
#include <stdlib.h>
int main(){
  int a;
  char c;

  scanf("%d", &a);
  fflush(stdin); // 刷新缓冲区
  c = getchar();
  printf("a = %d, c = %c \n", a, c);
  return 0;
}