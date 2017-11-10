1、在编译好 pthreads静态库之后，在编译 RedisServer工程的时候，得设置 Runtime Library 为(/MT)
2、直接提供 win端下 pthread 头文件和静态库 省的编译配置麻烦
3、github： https://github.com/MicrosoftArchive/redis/tree/2.4.6_win32