#ifndef H_TERMINAL_H
#define H_TERMINAL_H

///TODO(linjianjun) 2018.04.05 终端如何抽象，后续再来思考，手机终端和ATM终端先各自实现
//class terminal {
//public:
//  terminal() {}
//  ~terminal() {}
//};

#include "users.h"
#include "bank.h"

class user;
class user_manage;

class ATM{
public:
  ATM() {}
  ~ATM() {}

  // 提现
  int borrow_money(user* pUser);

  // 还款
  int giveBack_money(user* pUser);

  // 用户信用额度设置
  static int set_user_credit_card_max(user* pUser);
};

//手机终端暂时不支持：1、信用卡借钱；2、还信用卡借款； 先简化终端功能，做出来再思考扩展；
class phone
{
public:
  phone() {}
  ~phone() {}

  // 购买商品 (买东西暂时做成固定扣款，每次扣一百吧！)
  static int buy_something_from_shop(user* pUser);

  // 转账给其他用户
  static int exchange_money(user* pUserSrc, user* pUserDes);

  // 存钱
  int stone_money(user_manage& mgr, user* pUser, int money)
  {
    if (NULL == pUser) {
      return -1; //待添加用户指针不能为空
    }

    if (!mgr.exist_pointed_user(pUser)) {
      return -1; // 要存钱的用户不存在，请对该用户开户
    }

    return bank_.stone_money(pUser, money);
  }

  // 余额查询
  int scan_personal_money(user* pUser, em_card_type type)
  {
    if (NULL == pUser) {
      return -1; //待添加用户指针不能为空
    }

    bank_.scan_user_leave_money(pUser);
  }

  // 查看个人银行流水
  static int scan_personal_bank_detail(user* pUser);

private:
  bank bank_;
};

#endif
