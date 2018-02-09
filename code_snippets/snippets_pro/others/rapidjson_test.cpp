
//腾讯 json 开源库测试，使用版本 v0.1 https://github.com/Tencent/rapidjson
// 源码见 3rd

#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include "rapidjson/reader.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <cstdio>
#include <iostream>

using namespace rapidjson;

void load_json(std::string file_name, Document &d) {
  FILE *pFile = fopen(file_name.c_str(), "r");
  if (pFile) {
    //读取文件进行解析成json
    rapidjson::FileStream inputStream(pFile);
    d.ParseStream<0>(inputStream);
    fclose(pFile);
  }
}

void parse_json_to_multifiles(std::string file_name, Document &d) {
  const rapidjson::Value &value1 = d;
  for (rapidjson::Value::ConstMemberIterator iter = value1.MemberBegin();
       iter != value1.MemberEnd(); ++iter) {
    const rapidjson::Value &name_json = iter->name; // 这个必须是字符串
    const rapidjson::Value &value_json = iter->value; // 这个可以为对象、数组等
    // printf("%s\n", name_json.GetString());
    std::cout << "name_json: " << name_json.GetString() << std::endl;
    std::cout << name_json.GetType() << std::endl;

    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType &allocator = document.GetAllocator();
    std::string partner_id_data;
    rapidjson::Value data(rapidjson::kObjectType);
    // data.Swap(const_cast<rapidjson::Value&>(value1[name_json.GetString()]));
    // //version > 0.11 新版本节点拷贝方式
    data = const_cast<rapidjson::Value &>(value1[name_json.GetString()]);
    document.AddMember("keywords", data, allocator);

    std::string sub_file_name;
    std::string str_partner_id(name_json.GetString());
    sub_file_name.append(file_name, 0, file_name.size() - 4); //剔除 ".txt" 片段
    sub_file_name.append(str_partner_id.c_str());
    sub_file_name.append(".txt");
    // std::cout<<"filename: "<<sub_file_name<<std::endl;

    //写入磁盘文件
    FILE *pFile1 = fopen(sub_file_name.c_str(), "w");
    if (pFile1) {
      //读取文件进行解析成json
      rapidjson::FileStream inputStream(pFile1);
      // StringBuffer buffer;
      Writer<rapidjson::FileStream> writer(inputStream);
      document.Accept(writer);
      fclose(pFile1);
    }

    sub_file_name.clear();
  }
}

int main() {
  std::string file_name("filter_words.txt");
  Document d;
  load_json(file_name, d);
  parse_json_to_multifiles(file_name, d);

  return 0;
}