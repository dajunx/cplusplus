/* c-sytle io test
  getInputDataFromBuf:
  c风格io测试，如下是 测试刷新缓冲区测试
  1.若没有 fflush(stdin)，结果如下
    输入 123abc回车
    显示 a = 123, c = a
  2.若有 fflush(stdin)，结果如下
    输入 123abc回车
        xyz回车
    输出 a = 123, c = x

  ConnBufWithStream
  setbuf(stdout, outbuf) 把缓冲区与流相连.
  在 fflush调度后才会在屏幕打印出两行 "This is a test of buffered output." 字符

  相关连接：
  1、http://c.biancheng.net/cpp/html/2506.html
  2、http://c.biancheng.net/cpp/html/265.html
*/
#include <stdio.h>
#include <stdlib.h>
char outbuf[BUFSIZ];

void getInputDataFromBuf() {
  int a;
  char c;

  scanf("%d", &a);
  fflush(stdin); // 刷新缓冲区
  c = getchar();
  printf("a = %d, c = %c \n", a, c);
}

void ConnBufWithStream() {
  setbuf(stdout, outbuf); // 把缓冲区与流相连
  puts("This is a test of buffered output.\n");
  puts(outbuf);
  fflush(stdout); // 刷新
  puts(outbuf);   // 输出
}

int main() {
  getInputDataFromBuf();
  ConnBufWithStream();
  return 0;
}