#ifndef COMMON_FUN
#define COMMON_FUN

//#include "data_mgr.h"
#include "common_def.h"

// 解析命令，返还一对命令/值 对;
// set foo lin; get foo; shutdown;
///TODO 函数变成 非inline之后 链接错误，提示 main中与 network_mgr 中标识符冲突
inline void parse_cmd_and_content(char* content, std::vector<std::string>& vec_data)
{
  if (!content) return;
  if (vec_data.size() != 3) return;

  char* p_ch = content;
  int data_pos = 0;
  std::string single_data;
  while (p_ch != NULL) {
    if (*p_ch != ' ' && *p_ch != '\0') {
      single_data.push_back(*p_ch);
    } else if (*p_ch == ' ' || *p_ch == '\0') {
      vec_data[data_pos++] = single_data;
      single_data.clear();
    }

    if (*p_ch == '\0') break;
    p_ch++;
  }
}

inline void file_read()
{
  std::string str;
  std::fstream file("file.txt", std::fstream::in);
  file>>str;
  std::cout<<str;
  file.close();
}

inline void file_write()
{
  std::fstream file("file.txt",
    std::fstream::in | std::fstream::out | std::fstream::app);
  file<<"lin";
  file.close();
}

#endif