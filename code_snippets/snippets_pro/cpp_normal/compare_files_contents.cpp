#include <iostream>
#include <memory>
#include <vector>

#include <fstream>
#include <sstream>
#include <string>

void readFileContents(const std::string &filename,
  std::vector<std::string> &vec_contents) {
    std::ifstream fin(filename.c_str());
    std::string s;
    int nCount = 0;
    while (getline(fin, s)) {
      nCount++;
      vec_contents.push_back(s);
    }
    std::cout << "nCount:" << nCount << std::endl;
}

int main() {
  std::string log_rootPath("C:\\Users\\Administrator\\Desktop\\109_log");

  std::stringstream sourceFileName;
  sourceFileName
    << log_rootPath
    << "\\2018-02-09\\58.215.165.216_1.text_reocord.log.2018-02-09-";
  std::stringstream DesFileName;
  DesFileName
    << log_rootPath
    << "\\2018-02-09\\58.215.165.216_1.text_reocord.log.2018-02-09-00~11";

  std::vector<std::string> vecSourceFilesContent, vecDesFileContent;
  std::stringstream filename_tmp;
  for (int i = 0; i < 12; i++) {
    filename_tmp << sourceFileName.str();
    if (i < 10) {
      filename_tmp << "0" << i;
    } else {
      filename_tmp << i;
    }
    readFileContents(filename_tmp.str(), vecSourceFilesContent);
    filename_tmp.str("");
  }

  readFileContents(DesFileName.str(), vecDesFileContent);

  size_t size = vecSourceFilesContent.size();
  std::vector<std::string>::iterator it1 = vecSourceFilesContent.begin(),
    it2 = vecDesFileContent.begin();

  for (int i = 0; i < size; ++i) {
    if ((*it1++).compare(*it2++) != 0) {
      std::cout << "err";
    }
  }

  return 0;
}