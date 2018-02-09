// c++ time 相关 初步体验

#include <Windows.h> //Sleep()
#include <ctime>     //包含 #include <time.h>
#include <iostream>
#include <string>

int main() {
  //系统的当前日历时间，自
  //1970年1月1日以来经过的秒数(时间戳)。如果系统没有时间，则返回 .1。 time_t
  // time(time_t *time);
  time_t tt, tt1;
  tt = time(NULL);
  tt1 = time(NULL);

  //该函数返回一个指向表示本地时间的 tm 结构的指针。
  // struct tm *localtime(const time_t *time);
  struct tm *p_tm1;
  p_tm1 = localtime(&tt);

  //该函数返回一个指向 time 的指针，time 为 tm
  //结构，用协调世界时（UTC）也被称为格林尼治标准时间（GMT）表示。 struct tm
  // *gmtime(const time_t *time);
  memset(p_tm1, 0, sizeof(struct tm));
  p_tm1 = gmtime(&tt);

  //该返回一个表示当地时间的字符串指针，字符串形式 day month year
  //hours:minutes:seconds year\n\0。 char *ctime(const time_t *time);
  char *time_sz = (char *)malloc(sizeof(char) * 31);
  time_sz = ctime(&tt);

  //该函数返回一个指向字符串的指针，字符串包含了 time 所指向结构中存储的信息，
  //  返回形式为：day month date hours:minutes:seconds year\n\0。
  // char * asctime ( const struct tm * time );
  memset(time_sz, 0, sizeof(char) * 31);
  time_sz = asctime(p_tm1);

  //该函数返回日历时间，相当于 time 所指向结构中存储的时间
  // time_t mktime(struct tm *time);
  memset(&tt, 0, sizeof(time_t));
  tt = mktime(p_tm1);

  //该函数返回程序执行起（一般为程序的开头），处理器时钟所使用的时间。如果时间不可用，则返回
  //.1 clock_t clock(void);
  clock_t cbe = clock();
  Sleep(3);
  clock_t ced = clock();
  int clock_leg = ced - cbe;

  //该函数返回 time1 和 time2 之间相差的秒数。
  // double difftime ( time_t time2, time_t time1 );
  double time_leng = difftime(tt1, tt);

  //该函数可用于格式化日期和时间为指定的格式。
  // size_t strftime();
  char *strptr = (char *)malloc(sizeof(char) * 32);
  size_t st = strftime(strptr, sizeof(char) * 31, "%A", p_tm1);

  int i = 0;
  return 0;
}

/*
asctime：将 日期(struct tm)转换成字符串
ctime：将 秒数转换成字符串
gettimeofday：根据给定的与1970年1月1日相减得秒数，得到时间和日期
gmtime：根据给定的与1970年1月1日相减得秒数，取得时间和日期
localtime：根据给定的与1970年1月1日相减得秒数，取得当地的时区的时间和日期
mktime：将时间结构数据转换成经过的秒数
settimeofday：　设置系统时间，只有ｒｏｏｔ用户能运行
*/