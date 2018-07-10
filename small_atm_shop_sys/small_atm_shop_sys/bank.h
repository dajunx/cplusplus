#ifndef H_BANK_H
#define H_BANK_H

#include <sstream>

#include "object_manage.h"
#include "db_manage.h"
#include "users.h"
#include "tools.h"

class user;
class db;
class object_manage;

class bank{
public:
  bank() {}
  ~bank() {}

  //消费 or 提现
  int reduce_money(user* pUser, float money)
  {
    db& db_temp = object_manage::get_instance()->get_db();
    if (NULL == pUser) {
      return -1;
    }

    return db_temp.reduce_money(pUser->uid, money);
  }

  //取现
  int borrow_money_to_users();

  //收到还款
  int get_giveback();

  //查询用户余额
  int scan_user_leave_money(user* pUser, float& left_money)
  {
    db& db_temp = object_manage::get_instance()->get_db();
    if (NULL == pUser) {
      return -1; //待添加用户指针不能为空
    }

    return db_temp.scan_money(pUser->uid, left_money);
  }

  //查询用户提现数目
  int scan_user_borrow_totals();

  //用户间转账
  int exchange_money();

  //存钱
  int increase_money(user* pUser, float money)
  {
    db& db_temp = object_manage::get_instance()->get_db();
    if (NULL == pUser) {
      return -1; //待添加用户指针不能为空
    }

    db_temp.increase_money(pUser->uid, money);

    return 0;
  }

  //初始化用户帐户
  int init_user_account(user* pUser, float money)
  {
    db& db_temp = object_manage::get_instance()->get_db();
    if (NULL == pUser) {
      return -1; //待添加用户指针不能为空
    }

    db_temp.increase_money(pUser->uid, money);

    records& record_temp = object_manage::get_instance()->get_records();
    std::stringstream strLog;
    strLog << "[初始化用户]:" << pUser->uid << " 信用卡帐户" << std::endl;
    record_temp.saveContentToFile("ATM.log", strLog.str());
    return 0;
  }

  //管理用户信用额度
  int manage_user_credit_card_max();

  //冻结用户账号
  int freeze_user_account();
};

#endif
