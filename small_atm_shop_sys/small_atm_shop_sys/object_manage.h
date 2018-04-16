#ifndef H_OBJECT_MANAGE_H
#define H_OBJECT_MANAGE_H

#include <set>

class bank;
class shop;
class terminal;
class user;
class user_manage;
class db_manage;
class db;

class object_manage
{
public:
  object_manage()
    :p_shop_(NULL), p_bank_(NULL), p_terminal_(NULL), p_user_manage_(NULL), p_db_(NULL)
  {}
  ~object_manage() {}

  static object_manage* get_instance()
  {
    if (p_object_manage_ == NULL)
    {
      p_object_manage_ = new object_manage();
      p_object_manage_->init();
    }

    return p_object_manage_;
  }
  
  bool init();
  void add_template_users();
  user& get_user(int uid);

  shop& get_shop() { return *p_shop_; }
  bank& get_bank() { return *p_bank_; }
  terminal& get_terminal() { return *p_terminal_; }
  user_manage& get_user_manage() { return *p_user_manage_; }
  db& get_db() { return *p_db_; }

private:
  shop* p_shop_;
  bank* p_bank_;
  terminal* p_terminal_;
  user_manage* p_user_manage_;
  std::set<user*> set_users_;
  db* p_db_;
  static object_manage* p_object_manage_;
};

//object_manage* object_manage::p_object_manage_ = NULL;

#endif 