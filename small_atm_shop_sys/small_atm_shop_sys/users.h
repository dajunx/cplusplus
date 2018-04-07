#ifndef H_USER_h
#define H_USER_h

#include <vector>

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
//private:
  int uid;
  em_user_type utype;
};

class user_manage {
public:
  user_manage() { }
  ~user_manage() { }

  // 登录(普通用户登录，管理员登录)
  int login(user* pUser);

  //添加用户 （诸如：添加用户、用户额度设置、冻结用户、提现费率调整）
  int add_user(user* pUser, em_user_type type)
  {
    if (NULL == pUser) {
      return -1; //待添加用户指针不能为空
    }

    switch(type) {
    case normal_user:
      {
        vec_normal_users_.push_back(pUser);
      }
      break;
    case admin:
      {
        pAdmin_ = pUser;
      }
      break;
    default:
      break;
    }

    return 0;
  }

  // 冻结用户
  int freeze_user(user* pUser);

  // 校验用户
  bool exist_pointed_user(user* pUser)
  {
    if (NULL == pUser) {
      return false; //待添加用户指针不能为空
    }

    bool exist_pointed_user = false;
    std::vector<user*>::iterator it = vec_normal_users_.begin();
    for (; it!= vec_normal_users_.end(); ++it)
    {
      if (pUser == *it) {
        exist_pointed_user = true;
        break;
      }
    }

    return exist_pointed_user;
  }

//private:
  std::vector<user*> vec_normal_users_;
  user* pAdmin_;
};

#endif
