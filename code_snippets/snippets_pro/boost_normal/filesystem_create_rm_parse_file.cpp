// filesystem 创建移除目录、文件，解析加载
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <string>
#include <vector>
using namespace boost;

struct data {
  std::string name;
  int value;
};

void create_dir() {
  try {
    filesystem::path path_dir("e:\\temp\\test");
    if (create_directory(path_dir)) {
      rename(path_dir, "e:\\temp\\test2");
      boost::filesystem::remove("e:\\temp\\test2");
    }
  } catch (filesystem::filesystem_error &e) {
    std::cerr << e.what() << '\n';
  }
}

void create_file(filesystem::path &path_temp) {
  try {
    if (filesystem::exists(path_temp) &&
        !filesystem::is_regular_file(path_temp)) {
      filesystem::fstream fs;
      fs.open(path_temp);
      fs.close();
    }
  } catch (filesystem::filesystem_error &e) {
    std::cerr << e.what() << '\n';
  }
}

void remove_directory_file(filesystem::path &path_temp) {
  try {
    if (filesystem::exists(path_temp) &&
        filesystem::is_regular_file(path_temp)) {
      filesystem::remove_all(path_temp);
    }
  } catch (filesystem::filesystem_error &e) {
    std::cerr << e.what() << '\n';
  }
}

void parse(filesystem::fstream &fs,
           std::map<std::string, std::vector<data>> &dd) {
  char content[50];
  std::string title;
  std::vector<data> data_tmp_vec;
  bool bonePairData = false;
  while (fs.getline(content, 50)) {
    std::string temp(content);
    if (temp.find_first_of("[") != std::string::npos &&
        temp.find_first_of("]") != std::string::npos) {
      if (bonePairData) {
        // 存入一组记录
        dd[title] = data_tmp_vec;
        data_tmp_vec.clear();
      }
      title = temp.substr(1, temp.find_first_of("]") - 1);
      bonePairData = !bonePairData;
    } else if (temp.find_first_of("=") != std::string::npos) {
      std::size_t pos = temp.find_first_of("=");
      data data_tmp;
      data_tmp.name = temp.substr(0, pos);
      temp.erase(0, pos + 1);
      data_tmp.value = atoi(temp.c_str());
      data_tmp_vec.push_back(data_tmp);
    }
  }
  // 存入一组记录
  dd[title] = data_tmp_vec;
  data_tmp_vec.clear();
}

void load_content_from_file(filesystem::path &path_file,
                            std::map<std::string, std::vector<data>> &dd) {
  try {
    if (filesystem::exists(path_file) &&
        filesystem::is_regular_file(path_file)) {
      filesystem::fstream fs;
      fs.open("e:\\temp\\test.txt", std::fstream::in);
      parse(fs, dd);

      fs.close();
    }
  } catch (filesystem::filesystem_error &e) {
    std::cerr << e.what() << '\n';
  }
}

void save_content() {
  try {
    std::string content = "linjianjun";
    filesystem::fstream fs;
    fs.open("e:\\temp\\test1.txt",
            std::fstream::in | std::fstream::out | std::fstream::app);
    fs << "[" << content << "]" << std::endl;
    fs << content << "=" << content << std::endl;
    fs << "[" << content << "]" << std::endl;
    fs << content << "=" << content << std::endl;

    fs.close();
  } catch (filesystem::filesystem_error &e) {
    std::cerr << e.what() << '\n';
  }
}

int main(int argc, char *argv[]) {
  filesystem::path path_dir("e:\\temp");
  filesystem::path path_dir_file("e:\\temp\\test.txt");

  // 	std::map<std::string, std::vector<data> > dd;
  // 	load_content_from_file(path_dir_file, dd);
  save_content();
  return 0;
}