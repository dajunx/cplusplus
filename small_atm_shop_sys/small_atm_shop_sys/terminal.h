#ifndef H_TERMINAL_H
#define H_TERMINAL_H

#include "object_manage.h"

class user;
class user_manage;

class terminal{
public:
  terminal() {}
  ~terminal() {}

  int init_credit_card(user* pUser, float money)
  {
    bank& bk = object_manage::get_instance()->get_bank();
    if (NULL == pUser || money < 0) {
      return -1; //待添加用户指针不能为空
    }

    return bk.init_user_account(pUser, money);
  }

  // 转账给其他用户
  int borrow_money_from_user(user* pUserSrc, user* pUserDes, float money)
  {
    bank& bk = object_manage::get_instance()->get_bank();
    //参数校验
    if(NULL == pUserSrc || NULL == pUserDes || money < 0) {
      std::cout<<"parameter is not allow!"<<std::endl;
    }

    //查看被借钱的人钱够不够
    float left_money = 0;
    bk.scan_user_leave_money(pUserSrc, left_money);
    if (left_money < money) {
      std::cout<<"user has not enough to borrow to others."<<std::endl;
    }

    //借钱
    bk.reduce_money(pUserSrc, money);
    bk.increase_money(pUserDes, money);

    records& record_temp = object_manage::get_instance()->get_records();
    std::stringstream strLog;
    strLog << "[转账] 借钱人:" << pUserDes->uid << "， 被借钱人:" << pUserSrc->uid << std::endl;
    record_temp.saveContentToFile("ATM.log", strLog.str());

    return 0;
  }

  // 还掉信用卡钱
  int return_money(user* pUser, float money)
  {
    user_manage& mgr = object_manage::get_instance()->get_user_manage();
    bank& bk = object_manage::get_instance()->get_bank();
    if (NULL == pUser) {
      return -1; //待添加用户指针不能为空
    }

    if (!mgr.active_user(pUser)) {
      return -1; // 要存钱的用户不存在，请对该用户开户
    }

    records& record_temp = object_manage::get_instance()->get_records();
    std::stringstream strLog;
    strLog << "[还钱]:" << pUser->uid <<", 还掉的金额:" << money << std::endl;
    record_temp.saveContentToFile("ATM.log", strLog.str());

    return bk.increase_money(pUser, money);
  }

  // 余额查询
  int scan_personal_money(user* pUser)
  {
    bank& bk = object_manage::get_instance()->get_bank();
    if (NULL == pUser) {
      return -1; //待添加用户指针不能为空
    }

    ///TODO. 这个地方有点蹩脚，再改改
    float left_money = 0;
    return bk.scan_user_leave_money(pUser, left_money);
  }

  int borrow_money_from_bank(user* pUser, float money)
  {
    bank& bk = object_manage::get_instance()->get_bank();
    if (NULL == pUser) {
      return -1;
    }

    records& record_temp = object_manage::get_instance()->get_records();
    std::stringstream strLog;
    strLog << "[提现]:" << pUser->uid <<", 提现金额:" << money << std::endl;
    record_temp.saveContentToFile("ATM.log", strLog.str());

    //每次消费暂时定位 10 个单位
    return bk.reduce_money(pUser, money * 1.05);
  }

  // 查看个人银行流水
  static int scan_personal_bank_detail(user* pUser);
};

#endif
