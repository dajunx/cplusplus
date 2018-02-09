//简单的文件读写

#include <fstream>
#include <iostream>
#include <string>

void write() {
  std::fstream file("file.txt",
                    std::fstream::in | std::fstream::out | std::fstream::app);
  file << "lin";
  file.close();
}

void read() {
  std::string str;
  std::fstream file("file.txt", std::fstream::in);
  file >> str;
  std::cout << str;
  file.close();
}

//////////////////////////////////////////////////////////////////////////
// 按照行、单词方式读取文件到内存中
// url:http://www.cnblogs.com/JCSU/articles/1190685.html
//////////////////////////////////////////////////////////////////////////

//读取方式: 逐词读取, 词之间用空格区分
// read data from the file, Word By Word
// when used in this manner, we'll get space-delimited bits of text from the
// file but all of the whitespace that separated words (including newlines) was
// lost.
void ReadFileDataFromFileWBW() {
  std::ifstream file("file.txt");
  std::string s;
  while (file >> s) {
    std::cout << "ch : " << s << std::endl;
  }
}

//读取方式: 逐行读取, 将行读入字符数组, 行之间用回车换行区分
// If we were interested in preserving whitespace,
// we could read the file in Line-By-Line using the I/O getline() function.
void ReadDataFromFileLBLIntoCharArray() {
  std::ifstream fin("file.txt");
  const int LINE_LENGTH = 100;
  char str[LINE_LENGTH];
  while (fin.getline(str, LINE_LENGTH)) {
    std::cout << "Read from file into char array: " << str << std::endl;
  }
}

//读取方式: 逐行读取, 将行读入字符串, 行之间用回车换行区分
// If you want to avoid reading into character arrays,
// you can use the C++ string getline() function to read lines into strings
void ReadDataFromFileLBLIntoString() {
  std::ifstream fin("file.txt");
  std::string s;
  while (getline(fin, s)) {
    std::cout << "Read from file into string: " << s << std::endl;
  }
}

//带错误检测的读取方式
// Simply evaluating an I/O object in a boolean context will return false
// if any errors have occurred
void ReadDataWithErrChecking() {
  std::string filename = "dataFUNNY.txt";
  std::ifstream fin(filename.c_str());
  if (!fin) {
    std::cout << "Error opening " << filename << " for input" << std::endl;
  }
}

int main() {
  ReadFileDataFromFileWBW();
  ReadDataFromFileLBLIntoCharArray();
  ReadDataFromFileLBLIntoString();
  ReadDataWithErrChecking();

  write();
  read();

  return 0;
}