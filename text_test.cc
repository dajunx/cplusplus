#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

int main()
{
  ifstream ru_file("peterlin_RU.txt");
  if(!ru_file)
    cout<<"peterlin_RU.txt is not exist."<<endl;

  string buffer;
  while(getline(ru_file, buffer))
  {
    string::size_type pos = 0, pos_next = -1;
    string str_temp[5];

    for(int i = 0;i<5;++i)
    {
      pos = ++pos_next;
      pos_next = buffer.find(",", pos_next);
      str_temp[i] = buffer.substr(pos, pos_next - pos);
    }

    cout<<"userid:"<<str_temp[0]<<","
        <<"reg_tm:"<<str_temp[1]<<","
        <<"channelid:"<<str_temp[2]<<","
        <<"zone_id:"<<str_temp[3]<<","
        <<"name:"<<str_temp[4]<<endl;
  }

  ru_file.close();
  return 0;
}
