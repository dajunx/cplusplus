#ifndef H_SHOP_H
#define H_SHOP_H

#include "object_manage.h"

class shop{
public:
  shop() {}
  ~shop() {}

  //购物 (购物只实现扣款，不对购买实物做处理);
  int doShopping(user* pUser, float money = 100)
  {
    bank& bk = object_manage::get_instance()->get_bank();
    if (NULL == pUser) {
      return -1;
    }

    //每次消费暂时定位 10 个单位
    return bk.reduce_money(pUser, money);
  }
};

#endif
