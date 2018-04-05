#ifndef H_TERMINAL_H
#define H_TERMINAL_H

///TODO(linjianjun) 2018.04.05 终端如何抽象，后续再来思考，手机终端和ATM终端先各自实现
//class terminal {
//public:
//  terminal() {}
//  ~terminal() {}
//};

class ATM{
public:
  ATM() {}
  ~ATM() {}

  //提现
  int borrow_money();

  //还款
  int giveBack_money();

  //查询ATM操作详情记录
  int scan_ATM_detail();
};

//手机终端暂时不支持：1、信用卡借钱；2、还信用卡借款； 先简化终端功能，做出来再思考扩展；
class phone
{
public:
  phone() {}
  ~phone() {}

  //消费
  int spend_money();

  //转账
  int exchange_money();

  //查询手机终端操作流水
  int scan_phone_detail();
};

#endif
