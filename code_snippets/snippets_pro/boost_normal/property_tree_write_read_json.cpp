// boost property_tree read/write json

#include <iostream>
#include <string>
#include <vector>

#include <boost/cstdint.hpp>
#include <boost/foreach.hpp>
#include <boost/locale.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

void write_local_file(const std::string &file, const std::string &content) {
  std::ofstream of_config(file.c_str(), std::ios::trunc);
  if (of_config.is_open()) {
    of_config << content << std::endl;
    of_config.close();
  }
}

void fun1(const std::string &db) {
  try {
    boost::property_tree::ptree ptree;
    boost::property_tree::read_json(db, ptree);
    BOOST_FOREACH(boost::property_tree::ptree::value_type &value,
                  ptree.get_child("keywords")) {
      boost::property_tree::ptree &child = value.second;
      boost::property_tree::write_json("filter_new1.txt", child);

      const std::string str = child.get<std::string>("keyword");
      const std::wstring wstr = boost::locale::conv::utf_to_utf<wchar_t>(str);
      boost::uint32_t replact =
          child.get<boost::uint32_t>("replace") ? true : false;
    }

    BOOST_FOREACH(boost::property_tree::ptree::value_type &value,
                  ptree.get_child("keywords5")) {
      boost::property_tree::ptree &child = value.second;
      boost::property_tree::write_json("filter_new2.txt", child);

      const std::string str = child.get<std::string>("keyword");
      const std::wstring wstr = boost::locale::conv::utf_to_utf<wchar_t>(str);
      boost::uint32_t replact =
          child.get<boost::uint32_t>("replace") ? true : false;
    }
  } catch (const std::exception &) {
  }
}

void printTree(const std::string &db) {
  std::vector<int> vec_partner_ids;
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(db, pt);
  std::string file_name;

  int i = 0;
  std::stringstream tmp;
  try {
    for (boost::property_tree::ptree::iterator pos = pt.begin();
         pos != pt.end(); ++pos) {
      tmp << ++i;
      file_name.append(db, 0, db.size() - 4); //剔除 ".txt" 片段
      file_name.append(tmp.str());
      file_name.append(".txt");

      boost::property_tree::ptree empty_child;
      boost::property_tree::ptree child;
      child.add_child(tmp.str(), pos->second);
      vec_partner_ids.push_back(atoi(pos->first.c_str()));

      BOOST_FOREACH(boost::property_tree::ptree::value_type &value,
                    child.get_child(tmp.str(), empty_child)) {
        boost::property_tree::ptree &child_tmp = value.second;
        boost::property_tree::write_json(file_name, child_tmp);
      }

      tmp.str("");
      file_name.clear();
    }
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}

void printTree_signal(const std::string &db, boost::uint32_t partner_id) {
  boost::property_tree::ptree pt;
  boost::property_tree::read_json(db, pt);
  std::string file_name;

  try {
    int i = 0;
    std::stringstream tmp;
    tmp << partner_id;
    file_name.append(db, 0, db.size() - 4); //剔除 ".txt" 片段
    file_name.append(tmp.str());
    file_name.append(".txt");

    boost::property_tree::ptree empty_child;
    boost::property_tree::ptree child;
    child.add_child(tmp.str(), pt.begin()->second);
    BOOST_FOREACH(boost::property_tree::ptree::value_type &value,
                  child.get_child(tmp.str(), empty_child)) {
      boost::property_tree::ptree &child_tmp = value.second;
      boost::property_tree::write_json(file_name, child_tmp);
    }
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}

void fun2(const std::string &db) {
  try {
    // 读取 指定json子节点，然后写入新文件中
    boost::property_tree::ptree ptree;
    boost::property_tree::read_json(db, ptree);

    BOOST_FOREACH(boost::property_tree::ptree::value_type &value,
                  ptree.get_child("0")) {
      boost::property_tree::ptree &child = value.second;
      boost::property_tree::write_json("filter_new1.txt", child);
    }

    BOOST_FOREACH(boost::property_tree::ptree::value_type &value,
                  ptree.get_child("1")) {
      boost::property_tree::ptree &child = value.second;
      boost::property_tree::write_json("filter_new2.txt", child);
    }

    BOOST_FOREACH(boost::property_tree::ptree::value_type &value,
                  ptree.get_child("2")) {
      boost::property_tree::ptree &child = value.second;
      boost::property_tree::write_json("filter_new3.txt", child);
    }
  } catch (const std::exception &) {
  }
}

void fun3(const std::string &db) {
  try {
    // 读取 指定json子节点(按照数字从大到小排序)
    boost::property_tree::ptree ptree;
    boost::property_tree::read_json(db, ptree);
    int size = ptree.size();

    std::stringstream tmp;
    int i = 0;
    tmp << i;
    std::string child_name, save_file_name;
    child_name.append(tmp.str());
    int nCount = 0;

    for (int i = 0; i < static_cast<boost::uint32_t>(-1) && nCount < size;
         ++i) {
      save_file_name.append(db, 0, db.size() - 4); //剔除 ".txt" 片段
      save_file_name.append(tmp.str());
      save_file_name.append(".txt");

      bool bFinded = false;
      boost::property_tree::ptree empty_tree;
      BOOST_FOREACH(boost::property_tree::ptree::value_type &value,
                    ptree.get_child(child_name, empty_tree)) {
        bFinded = true;
        boost::property_tree::ptree &child = value.second;
        boost::property_tree::write_json(save_file_name, child);
      }

      if (bFinded)
        nCount++;

      tmp.str("");
      c tmp << ++i;
      child_name.clear();
      save_file_name.clear();
      child_name.append(tmp.str());
    }

  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}

int main() {
  // fun3("filter_new.txt");
  // printTree("filter_new.txt");
  printTree_signal("filter_new.txt", 1);
  // write_local_file("test111.txt", "linjianjun");

  int i = 0;

  return 0;
}