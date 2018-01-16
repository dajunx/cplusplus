#include <iostream>
#include <vector>
#include <memory>

#include <string>
#include <fstream>
#include <sstream>

void readFileContents(const std::string& filename, std::vector<std::string>& vec_contents)
{
  std::ifstream fin(filename);  
  std::string s;
  int nCount = 0;
  while(getline(fin,s))
  {
    nCount++;
    vec_contents.push_back(s);
  }
  std::cout<<"nCount:"<<nCount<<std::endl;
}

int main()
{
  //   std::auto_ptr<int> p(new int);
  //   *p.get() = 10;
  std::string filename1("C:\\Users\\Administrator\\Desktop\\109_2018_01_09\\119.120.92.166_2.text_reocord.log.2018-01-09-");
  std::string filename2("C:\\Users\\Administrator\\Desktop\\109_2018_01_09\\119.120.92.166_2.text_reocord.log.2018-01-09-12~23");
  std::vector<std::string> vec_content1, vec_content2;
  std::stringstream filename_tmp;
  for (int i=12; i < 24; i++)
  {
    filename_tmp << filename1;
    if (i < 10) {
      filename_tmp << "0" << i;
    } else {
      filename_tmp << i;
    }
    readFileContents(filename_tmp.str(), vec_content1);
    filename_tmp.str("");
  }

  readFileContents(filename2, vec_content2);

  size_t size = vec_content1.size();
  std::vector<std::string>::iterator it1 = vec_content1.begin(), it2 = vec_content2.begin();
  for (int i=0; i<size ; ++i)
  {
    if ((*it1++).compare(*it2++) != 0) {
      std::cout<<"err";
    }
  }

  return 0;
}