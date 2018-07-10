#include <iostream>

//user defined headers
#include "object_manage.h"

#include "bank.h"
#include "shop.h"
#include "terminal.h"
#include "users.h"
#include "db_manage.h"

int main() 
{
  object_manage* p_object_manage = object_manage::get_instance();
  //生成测试用户
  p_object_manage->add_template_users();

  user* p_test_user1 = p_object_manage->get_user(xiaohong_id);
  user* p_test_user2 = p_object_manage->get_user(xiaoli_id);
  user* p_test_admin = p_object_manage->get_user(xiaowang_admin_id);

  //初始化信用卡额度（增加15000)
  terminal& ter = p_object_manage->get_terminal();
  ter.init_credit_card(p_test_user1,  15000);
  ter.init_credit_card(p_test_user2,  15000);

  //商店购买流程
  shop& sh = p_object_manage->get_shop();
  sh.doShopping(p_test_user1, 15000);
  sh.doShopping(p_test_user2, 100);

  //用户1银行存款不够，向用户2借钱 100
  if ( 0 == ter.borrow_money_from_user(p_test_user2, p_test_user1, 100)) {
    //借到了钱，继续消费
    sh.doShopping(p_test_user1);
  }

  //还钱
  ter.return_money(p_test_user1, 15000);

  //提现
  ter.borrow_money_from_bank(p_test_user1, 100);

  /*
  ** 管理用户
  */
  //冻结用户
  user_manage& ugr = p_object_manage->get_user_manage();
  ugr.freeze_user(p_test_user1, p_test_admin->utype);

  return 0;
}