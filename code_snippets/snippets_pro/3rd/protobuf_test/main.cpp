#include <iostream>
#include <string>

#include "lm.helloworld.pb.h"
using namespace std;

int main()
{
  lm::helloworld msg;
  msg.set_id(5);
  lm::userinfo* ui = msg.mutable_uinfo();
  ui->set_uid(555);
  ui->set_pwd(999);
  ui->set_name("lin");
  ui->set_some("hello!");
  std::cout << "message: "<<msg.Utf8DebugString() << std::endl;

  std::string ss;
  ui->SerializeToString(&ss);
  std::cout << "serialized msg: "<< ss << std::endl;

	return 0;
}