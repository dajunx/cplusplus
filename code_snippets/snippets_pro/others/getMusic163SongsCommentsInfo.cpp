

// lib curlpp
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

// lib rapidjson
#include "rapidjson/document.h"
#include "rapidjson/filestream.h"
#include "rapidjson/reader.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>

#include <fstream>
#include <sstream>

size_t WriteCallback(char* ptr, size_t size, size_t nmemb, void *f)
{
  FILE *file = (FILE *)f;
  return fwrite(ptr, size, nmemb, file);
};

/*
  url相关
  http://music.163.com/api/v1/resource/comments/R_SO_4_28793052?limit=20&offset=3
  ?limit=20&offset=3 没有这两个参数，可以拉去热门评论
  limit 显示的普通评论条数；  offset 评论偏移条数
*/
void save_url_json_contents(std::string& filename, std::string& url)
{
  try
  {
    curlpp::Cleanup cleaner;
    curlpp::Easy request;

    curlpp::options::WriteFunctionCurlFunction
      myFunction(WriteCallback);

    FILE *file = stdout;
    file = fopen(filename.c_str(), "wb");
    if(file == NULL)
    {
      fprintf(stderr, "%s/n", strerror(errno));
      return ;
    } 

    curlpp::OptionTrait<void *, CURLOPT_WRITEDATA> 
      myData(file);

    request.setOpt(myFunction);
    request.setOpt(myData);

    request.setOpt(new curlpp::options::Url(url.c_str()));
    request.setOpt(new curlpp::options::Verbose(true));
    request.perform();
  }

  catch (curlpp::LogicError & e)
  {
    std::cout << e.what() << std::endl;
  }

  catch (curlpp::RuntimeError & e)
  {
    std::cout << e.what() << std::endl;
  }
}

void parse_json(const std::string& filename, std::string& output_str)
{
  try {
    FILE* pFileRead = NULL;
    pFileRead = fopen(filename.c_str(), "r");
    if (!pFileRead) {
      return;
    }
    using namespace rapidjson;

    Document document_src;
    FileStream inputStream(pFileRead);
    document_src.ParseStream<0>(inputStream);
    std::stringstream output;

    const Value& hotComments = document_src["hotComments"];
    if(hotComments.IsArray())
    {
      for (int i=0; i< hotComments.Size(); ++i)
      {
        const Value& user = hotComments[i]["user"];
        if(user.IsObject())
        {
          std::cout<<"user name:"<<user["nickname"].GetString()<<std::endl;
          output << "user name:" << user["nickname"].GetString() << std::endl;
        }

        const Value& content = hotComments[i]["content"];
        if (content.IsString())
        {
          std::cout<<"content value:"<<content.GetString()<<std::endl;
          output << "content:" << content.GetString() << std::endl;
        }
      }
    }

    output_str.swap(output.str());
  }
  catch (curlpp::LogicError & e)
  {
    std::cout << e.what() << std::endl;
  }
  catch (curlpp::RuntimeError & e)
  {
    std::cout << e.what() << std::endl;
  }
}

void write(const std::string& filename, std::string& input_str) {
  std::fstream file(filename,
    std::fstream::in | std::fstream::out | std::fstream::app);
  file << input_str;
  file.close();
}

int main()
{
  //file 文件名
  std::string filename("music163_comment.txt"), filename_out("music163_comment_out.txt");
  
  //json 源
  std::string music_id("28793052");
  std::string url("http://music.163.com/api/v1/resource/comments/R_SO_4_");
  url.append(music_id);

  std::string content;

  save_url_json_contents(filename, url);
  parse_json(filename, content);
  write(filename_out, content);

  return 0;
}