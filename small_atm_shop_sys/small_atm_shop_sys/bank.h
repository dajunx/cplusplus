#ifndef H_BANK_H
#define H_BANK_H

class bank{
public:
  bank() {}
  ~bank() {}

  //消费
  int decrease_money();

  //取现
  int borrow_money_to_users();

  //收到还款
  int get_giveback();

  //查询用户余额
  int scan_user_leave_money();

  //查询用户提现数目
  int scan_user_borrow_totals();

  //用户间转账
  int exchange_money();

  //添加用户
  int add_user_account();

  //管理用户信用额度
  int manage_user_credit_card_max();

  //冻结用户账号
  int freeze_user_account();
};

#endif
