/// TODo 待整理代码
//主要功能是 生成文件 + 维护一定量的文件数目(即删除老的文件)

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/thread.hpp>

bool test1(std::string &str_file_name) {
  std::string content;
  // 准备目录;
  const std::string str_splash = "\\/";
  namespace fs = boost::filesystem;
  fs::path cur_path(fs::initial_path());
  fs::path file_path = fs::system_complete(fs::path(str_file_name));
  std::string str_tag_path = file_path.normalize().generic_string();
  std::string::size_type pos = str_tag_path.find_last_of(str_splash);
  if (pos == std::string::npos) {
    return false;
  }
  str_tag_path = str_tag_path.substr(0, pos);
  fs::create_directories(fs::path(str_tag_path));

  boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1),
                                 boost::posix_time::hours(8));

  std::stringstream str_date;
  boost::posix_time::ptime t(boost::posix_time::second_clock::local_time());
  std::cout << "second: " << (t - epoch).total_seconds() << std::endl;
  str_date << str_file_name << ".";
  str_date << (t - epoch).total_seconds();

  content.append(str_date.str().c_str());
  //写入xml文件;
  std::ofstream of_config(str_file_name.c_str(), std::ios::trunc),
      of_config_tmp(str_date.str().c_str(), std::ios::trunc);
  if (of_config.is_open()) {
    of_config << content << std::endl;
    of_config.close();

    of_config_tmp << content << std::endl;
    of_config_tmp.close();
  }

  //删除过期文件
  int number_of_files = 0;
  std::map<std::string, boost::filesystem::path> file_name_map;
  boost::filesystem::directory_iterator end_itr;
  for (boost::filesystem::directory_iterator itr(str_tag_path); itr != end_itr;
       ++itr) {
    if (boost::filesystem::is_directory(itr->status())) {
      continue;
    }

    if (memcmp(itr->path().leaf().normalize().generic_string().c_str(),
               str_file_name.c_str(), str_file_name.length()) == 0) {
      file_name_map.insert(std::pair<std::string, boost::filesystem::path>(
          itr->path().leaf().normalize().generic_string(), itr->path().leaf()));
      number_of_files++;
    }
  }
  std::cout << "file count: " << number_of_files << std::endl;

  int max_count = 5;
  std::map<std::string, boost::filesystem::path>::iterator it_begin =
      file_name_map.begin();
  int del_file_count = file_name_map.size() - max_count;
  it_begin++;
  for (; it_begin != file_name_map.end() && del_file_count > 0; it_begin++) {
    boost::filesystem::remove(it_begin->second);
    del_file_count--;
  }

  return false;
}

int main() {
  std::string file_name("file_name_test.log");
  test1(file_name);

  for (int i = 0; i < 100; ++i) {
    std::string file_name("file_name_test.log");
    test1(file_name);
    boost::this_thread::sleep(boost::posix_time::seconds(1));
  }

  int i = 0;
  return 0;
}