// log4cplus 使用
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>
// #include <boost/chrono/chrono.hpp>
// #include <boost/asio.hpp>
// #include <boost/make_shared.hpp>
// #include <boost/shared_ptr.hpp>
// #include <boost/bind.hpp>
// #include <boost/enable_shared_from_this.hpp>
// #include <boost/thread/mutex.hpp>
// #include <boost/ref.hpp>
#include <string>

#include <conio.h>
#include <iostream>
#include <log4cplus/configurator.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/helpers/sleep.h>
#include <log4cplus/layout.h>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

using namespace log4cplus;
using namespace log4cplus::helpers;
// using namespace boost;

int main() {
  log4cplus::PropertyConfigurator::doConfigure("mylog.cfg");

  // 定义一个控制台的Appender
  SharedAppenderPtr pConsoleAppender(new ConsoleAppender());

  // 定义Logger
  Logger pTestLogger = Logger::getInstance("filter_text");

  // 将需要关联Logger的Appender添加到Logger上
  pTestLogger.addAppender(pConsoleAppender);

  // 输出日志信息
  while (1) {
    boost::posix_time::ptime start_time =
        boost::posix_time::microsec_clock::local_time();
    ;
    std::string str_time;
    str_time.swap(boost::posix_time::to_simple_string(start_time));
    LOG4CPLUS_WARN(pTestLogger, str_time);
    boost::this_thread::sleep(boost::posix_time::milliseconds(10));
  }

  boost::this_thread::sleep(boost::posix_time::seconds(1000));

  return 0;
}

/*
----------------------------------------------------------------------------------------------------------------
  mylog.cfg file:


#RootLogger
log4cplus.rootLogger=ALL, ALL_MSG
  log4cplus.logger.filter_text=ALL, ALL_FILTER_TEXT,RemoteServer
#log4cplus.logger.filter_text=ALL, ALL_FILTER_TEXT

  log4cplus.activity.filter_text=false

#####################################################################################################
# root logger...
# Appenders
  log4cplus.appender.ALL_MSG=log4cplus::AsyncAppender
  log4cplus.appender.ALL_MSG=log4cplus::RollingFileAppender
  log4cplus.appender.ALL_MSG.File=log.log
  log4cplus.appender.ALL_MSG.Schedule=DAILY
  log4cplus.appender.ALL_MSG.MaxBackupIndex=100
  log4cplus.appender.ALL_MSG.layout=log4cplus::PatternLayout
  log4cplus.appender.ALL_MSG.layout.ConversionPattern=%D{%m-%d %H:%M:%S,%q}
[%-5t][%-5p] -- %-8l%n %m%n log4cplus.appender.ALL_MSG.MaxFileSize=500MB

#####################################################################################################

# text filter...
# Appenders
  log4cplus.appender.ALL_FILTER_TEXT=log4cplus::AsyncAppender
  log4cplus.appender.ALL_FILTER_TEXT.Appender=log4cplus::DailyRollingFileAppender
  log4cplus.appender.ALL_FILTER_TEXT.Appender.File=filter_text.log
  log4cplus.appender.ALL_FILTER_TEXT.Appender.Schedule=HOURLY
  log4cplus.appender.ALL_FILTER_TEXT.Appender.MaxBackupIndex=240
  log4cplus.appender.ALL_FILTER_TEXT.Appender.layout=log4cplus::PatternLayout
  log4cplus.appender.ALL_FILTER_TEXT.Appender.layout.ConversionPattern=%m%n
  log4cplus.appender.RemoteServer=log4cplus::AsyncAppender
  log4cplus.appender.RemoteServer.QueueLimit=429496729
  log4cplus.appender.RemoteServer.Appender=log4cplus::SocketAppender
  log4cplus.appender.RemoteServer.Appender.host=127.0.0.1
  log4cplus.appender.RemoteServer.Appender.port=22289
  */