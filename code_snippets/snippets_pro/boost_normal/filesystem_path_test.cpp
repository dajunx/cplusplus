#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>

class test1 {
public:
  void fun1() {
    std::string cache_dir_temp("c:");
    std::string cache_dir_;
    boost::filesystem::path udp_path(cache_dir_temp);

    std::string file_name("UDP_info.ini");
    boost::system::error_code ec;
    boost::filesystem::exists(udp_path, ec);
    if (ec) {
      cache_dir_.assign(file_name);
    } else {
      udp_path = udp_path / file_name;
      cache_dir_.assign(udp_path.string());
    }
    std::cout << "file path:" << cache_dir_.c_str() << std::endl;
  }

  void fun2() {
    boost::filesystem::path udp_path("C:\\Users\\Administrator");
    boost::filesystem::path root_path = udp_path.root_path();
    boost::filesystem::path root_name = udp_path.root_name();

    std::string fielname("udp_ini.txt");
    if (boost::filesystem::exists(udp_path)) {
      udp_path = udp_path / fielname;
      std::cout << "file path:" << udp_path.c_str() << std::endl;
      int i = 0;
    }
  }

  void fun3() {
    boost::filesystem::path p(__FILE__);
    boost::filesystem::path parent_path(p.parent_path().string());

    parent_path /= "UDP_info.ini";
    std::cout << parent_path.string() << '\n';
    parent_path.remove_filename();
    std::cout << parent_path.string() << '\n';
  }

  void set_cache_dir(std::string const &cache_dir_temp) {
    udp_disk_cache_path_ = cache_dir_temp;
    if (!cache_dir_temp.empty() &&
        cache_dir_temp[cache_dir_temp.size() - 1] != '/' &&
        cache_dir_temp[cache_dir_temp.size() - 1] != '\\') {
      udp_disk_cache_path_ += '/';
    }
    udp_disk_cache_path_ += "UDP_info.ini";
  }

  void save_to_disk() {
    boost::property_tree::ptree pt;
    std::string first = "test1", second = "1";

    try {
      pt.add(first + ".name1", second);

      boost::property_tree::ini_parser::write_ini(udp_disk_cache_path_, pt);
    } catch (std::exception &e) {
      std::cout << "err:" << e.what() << std::endl;
    }
  }
  std::string udp_disk_cache_path_;
};

int main() {
  test1 t1;
  t1.fun1();
  t1.fun2();
  t1.fun3();
  std::string input_path("c:");
  t1.set_cache_dir(input_path);
  t1.save_to_disk();
  return 0;
}