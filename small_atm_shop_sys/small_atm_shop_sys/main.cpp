#include <iostream>

//user defined headers
#include "users.h"
#include "terminal.h"
#include "shop.h"

int main() 
{
  //初始状态，构造出用户、管理员 实体
  user lin001(661, normal_user), lin002(662, normal_user), lin_admin(991, admin);
  
  //添加到银行系统中
  user_manage ugr;
  ugr.add_user(&lin001, normal_user);
  ugr.add_user(&lin002, normal_user);
  ugr.add_user(&lin_admin, admin);
  
  // -.- 存钱(哈哈)
  phone ph;
  ph.stone_money(ugr, &lin001, 1000);
  ph.stone_money(ugr, &lin002, 1000);

  //存完钱当然是看看存好了没！！！
  ph.scan_personal_money(&lin001, bank_card);
  ph.scan_personal_money(&lin002, bank_card);

  //接下来就可以买买买了！！！
  //phone::buy_something_from_shop(lin001);
  //phone::scan_personal_money(lin001, bank_card); //剁手后的余额查看


  return 0;
}