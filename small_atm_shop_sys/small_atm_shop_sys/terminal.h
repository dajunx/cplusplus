#ifndef H_TERMINAL_H
#define H_TERMINAL_H

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
  int borrow_money(user* pUserSrc, user* pUserDes, bank& bk, int money)
  {
    //参数校验
    if(NULL == pUserSrc || NULL == pUserDes || money < 0) {
      std::cout<<"parameter is not allow!"<<std::endl;
    }

    //查看被借钱的人钱够不够
    int left_money = 0;
    bk.scan_user_leave_money(pUserSrc, left_money);
    if (left_money < money) {
      std::cout<<"user has not enough to borrow to others."<<std::endl;
    }

    //借钱
    bk.reduce_money(pUserSrc, money);
    bk.stone_money(pUserDes, money);
    std::cout<<"user: "<<pUserSrc->uid
            <<"borrow "<<money
            <<" money to user: "<<pUserDes->uid
            <<" success."<<std::endl;

    return 0;
  }

  // 存钱
  int stone_money(user_manage& mgr, user* pUser, bank& bk, int money)
  {
    if (NULL == pUser) {
      return -1; //待添加用户指针不能为空
    }

    if (!mgr.exist_pointed_user(pUser)) {
      return -1; // 要存钱的用户不存在，请对该用户开户
    }

    return bk.stone_money(pUser, money);
  }

  // 余额查询
  int scan_personal_money(user* pUser, em_card_type type, bank& bk)
  {
    if (NULL == pUser) {
      return -1; //待添加用户指针不能为空
    }

    ///TODO. 这个地方有点蹩脚，再改改
    int left_money = 0;
    return bk.scan_user_leave_money(pUser, left_money);
  }

  // 查看个人银行流水
  static int scan_personal_bank_detail(user* pUser);
};

#endif
