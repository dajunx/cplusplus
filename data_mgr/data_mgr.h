#ifndef DATA_MGR
#define DATA_MGR
#include "common_def.h"

class data_mgr
{
public:
  data_mgr() {}
  ~data_mgr() {}

  bool get_data(const std::string& key, std::string& out_value)
  {
    BOOST_AUTO(it, string_map_data_.find(key));
    if (it != string_map_data_.end()) {
      out_value = it->second;
    }

    return true;
  }

  // mode 添加数据模式 0 替换模式 1 排重模式
  bool add_data(const std::string& key, const std::string& value, boost::uint32_t mode = 0)
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

    return ret;
  }
public:
  std::map<std::string, std::string> string_map_data_;
};

#endif
