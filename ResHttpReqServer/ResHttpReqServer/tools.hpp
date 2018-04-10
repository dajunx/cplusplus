
#ifndef HTTP_MIME_TYPES_HPP
#define HTTP_MIME_TYPES_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

/*
内存中 http 请求展现的头数据，如下：
GET /ReadMe.txt HTTP/1.1
Host: 127.0.0.1:9999
Connection: keep-alive
Cache-Control: max-age=0
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.186 Safari/537.36

目标是 /ReadMe.txt 文本,使用方法 getTargetContent 获取
*/

class IO {
public:
  IO() {}
  ~IO() {}

  void wrapSave(const std::string &fileName, const std::string &data) {
    saveContentToFile(fileName, data);
  }

  void readFileContent(const std::string fileName) {
    return;
    /// TODO 没想好这点要做什么
    std::ifstream file(fileName);
    std::string s;
    while (file >> s) {
      std::cout << "ch : " << s << std::endl;
    }
  }

  void saveContentToFile(const std::string &fileName,
    const std::string &inputData) {
      std::fstream file(fileName,
        std::fstream::in | std::fstream::out | std::fstream::app);
      file << inputData;
      file << "================================================" << std::endl;
      file.close();
  }
};

class Http {
public:
  Http() {}
  ~Http() {}

  void getRequestContent(char* receiveContent, std::string& targetContent) {
    std::string content(receiveContent);
    std::string::size_type target_begin = content.find_first_of(" ");
    std::string::size_type target_end = content.find(" ", target_begin+1);
    targetContent = content.substr(target_begin+2, target_end - target_begin - 1);

    io.wrapSave("http.log", "begin:\n" + targetContent + "\n");
  }

  void produceResponse(const std::string& targetContent, std::string& reply) {
    if (false == reply.empty()) {
      return;
    }

    std::string filePath("F:\\github_git\\ResHttpReqServer\\ResHttpReqServer\\");
    filePath += targetContent;
    std::string fileContent;
    std::ifstream file(filePath, std::fstream::in);
    std::string s;
    while (getline(file, s)) {
      s.append("\r\n");
      fileContent += s;
    }

    // 计算Content-Length，若此值不对则回包会有问题，浏览器解析失败
    std::ifstream is;
    is.open(filePath.c_str(), std::ios::binary);
    is.seekg(0, std::ios::end);
    int fileContentLength = is.tellg();

    //
    std::stringstream ssRes;
    ssRes << "HTTP/1.0 200 OK\r\n"
          << "Content-Length: " << fileContentLength << "\r\n"
          << "Content-Type: text/plain\r\n\r\n"
          << fileContent;
    reply = ssRes.str();

    io.wrapSave("http.log", "end:\n" + reply);
    file.close();
  }

private:
  IO io;
};


#endif // HTTP_MIME_TYPES_HPP