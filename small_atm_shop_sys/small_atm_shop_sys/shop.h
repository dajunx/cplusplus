#ifndef H_SHOP_H
#define H_SHOP_H

#include "users.h"
#include "bank.h"

class shop{
public:
  shop() {}
  ~shop() {}

  //购物 (购物只实现扣款，不对购买实物做处理);
  int doShopping(user* pUser)
  {
    if (NULL == pUser) {
      return -1;
    }


  }
};

#endif
