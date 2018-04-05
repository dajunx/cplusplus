#ifndef H_USER_h
#define H_USER_h

enum em_user_type{normal_user, admin};
enum em_card_type{bank_card, credit_card};

class user {
public:
  user() {}
  user(int input_udi, em_user_type type)
    :uid(input_udi),
     utype(type)
  {}
  ~user() {}

private:
  int uid;
  em_user_type utype;
};

class user_manage {
public:
  user_manage() { }
  ~user_manage() { }

  // 购买商品 (买东西暂时做成固定扣款，每次扣一百吧！)
  static int buy_something_from_shop(user* pUser);

  // 转账给其他用户
  static int exchange_money(user* pUserSrc, user* pUserDes);

  // 提现
  static int borrow_money(user* pUser);

  // 还款
  static int giveBack_money(user* pUser);

  // 存钱
  static int stone_money(user* pUser, int money);

  // 余额查询
  static int scan_personal_money(user* pUser, em_card_type type);

  // 查看个人银行流水
  static int scan_personal_bank_detail(user* pUser);

  // 登录(普通用户登录，管理员登录)
  static int login(user* pUser);

  //用户管理 （诸如：添加用户、用户额度设置、冻结用户、提现费率调整）
  static int add_user(user* pUser, em_user_type type){

  }

  static int set_user_credit_card_max(user* pUser);
  static int freeze_user(user* pUser);
};

#endif
