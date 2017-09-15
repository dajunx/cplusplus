#ifndef DATA_MGR
#define DATA_MGR
#include "common_def.h"

class data_mgr
{
public:
  data_mgr() {
    init_data();
  }
  ~data_mgr() {}

  bool get_data(std::string& key, std::string& value)
  {
    BOOST_AUTO(it, string_map_data_.find(key));
    if (it != string_map_data_.end()) {
      key = it->first;
      value = it->second;
    }

    return true;
  }

  // mode 添加数据模式 0 替换模式 1 排重模式
  bool add_data(std::string& key, std::string& value, boost::uint32_t mode = 0)
  {
    bool ret = true;

    do 
    {
      if ( 0 == mode) {
        string_map_data_.insert(std::pair<std::string, std::string>(key, value));
      } else if ( 1 == mode) {
        BOOST_AUTO(it, string_map_data_.find(key));
        if (it == string_map_data_.end()) {
          ret = false;
          break;
        }
        string_map_data_.insert(std::pair<std::string, std::string>(key, value));
      }
    } while (0);

    key.clear();
    value.clear();
    return ret;
  }

  void save_data()
  {
    std::fstream file("file.txt",
      std::fstream::in | std::fstream::out | std::fstream::app);
    std::map<std::string, std::string>::iterator it = string_map_data_.begin();
    for (; it != string_map_data_.end(); ++it)
    {
      file<<it->first<<","<<it->second<<std::endl;
    }
    file.close();
  }

  void init_data()
  {
    std::fstream file("file.txt", std::fstream::in);
    std::map<std::string, std::string>::iterator it = string_map_data_.begin();

    char str[10];
    std::string key, value;
    while(file.getline(str, 10))
    {
      bool is_key = true;
      std::cout<<"data: "<<str<<std::endl;
      char* p_ch = str;
      while (*p_ch != '\0') {
        if (*p_ch == ',') {
          is_key = false;
          p_ch++;
          continue;
        }
        is_key ? key.append(1, *p_ch) : value.append(1, *p_ch);
        p_ch++;
      }

      string_map_data_.insert(std::pair<std::string, std::string>(key, value));
      key.clear();
      value.clear();
    }
    file.close();
  }

public:
  std::map<std::string, std::string> string_map_data_;
};

#endif
