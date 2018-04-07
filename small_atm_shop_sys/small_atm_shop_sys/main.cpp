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

  bank bk;
  
  // -.- 存钱(哈哈)
  phone ph;
  ph.stone_money(ugr, &lin001, bk, 10);
  ph.stone_money(ugr, &lin002, bk, 1000);

  //存完钱当然是看看存好了没！！！
  ph.scan_personal_money(&lin001, bank_card, bk);
  ph.scan_personal_money(&lin002, bank_card, bk);

  //接下来就可以买买买了！！！
  shop sh;
  sh.doShopping(&lin001, bk);
  sh.doShopping(&lin001, bk);

  //用户1银行存款不够，向用户2借钱 100
  if ( 0 == ph.borrow_money(&lin002, &lin001, bk, 100)) {
    //借到了钱，继续消费
    sh.doShopping(&lin001, bk);
  }

  return 0;
}