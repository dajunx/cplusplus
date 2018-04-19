
/*
  编译环境：由于依赖库jsoncpp使用的是c++11，所以需要 vs2015才可编译
  目的：解析json文件中的文本到本地文件中
*/
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <set>

#include "json/json.h"

void read_filter_word_content_to_simple_txt(std::string& filename, std::set<std::string>& set_contents)
{
  std::ifstream ifs;
  ifs.open("filter_words.txt");
  assert(ifs.is_open());

  Json::Reader reader;
  Json::Value root;
  if (!reader.parse(ifs, root, false))
  {
    return;
  }

  Json::Value child_node = root["keywords"];
  int keywords_words_count = child_node.size();
  for (int i = 0; i < keywords_words_count; ++i)
  {
    //std::cout << "words:" << child_node[i]["keyword"].asString() << std::endl;
    set_contents.insert(child_node[i]["keyword"].asString());
  }
}

void saveContentToFile(std::string& filename, std::set<std::string>& setContents)
{
  std::fstream file(filename, std::fstream::in | std::fstream::out | std::fstream::app);

  std::set<std::string>::iterator it = setContents.begin();
  for (; it != setContents.end(); ++it)
  {
    file << *it << std::endl;
  }
  file.close();
}

int main()
{
  std::string filename("filter_words.txt"), filename_out("filter_words_out.txt");
  std::set<std::string> setAllContents;
  read_filter_word_content_to_simple_txt(filename, setAllContents);
  saveContentToFile(filename_out, setAllContents);

  return 0;
}