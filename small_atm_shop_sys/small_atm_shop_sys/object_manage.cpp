#include "object_manage.h"
#include "bank.h"
#include "shop.h"
#include "terminal.h"
#include "users.h"
#include "db_manage.h"

object_manage* object_manage::p_object_manage_ = NULL;
bool object_manage::init()
{
  bool bSucc = false;
  p_shop_ = new shop();
  p_bank_ = new bank();
  p_terminal_ = new terminal();
  p_user_manage_ = new user_manage();
  p_db_ = new db();

  if (p_shop_ != NULL 
    && p_bank_ != NULL 
    && p_terminal_ != NULL 
    && p_user_manage_ != NULL
    && p_db_ != NULL)
  {
    bSucc = true;
  }

  return bSucc;
}

void object_manage::add_template_users()
{
  set_users_.insert(new user(661, normal_user));
  set_users_.insert(new user(662, normal_user));
  set_users_.insert(new user(991, admin));

  std::set<user*>::iterator it = set_users_.begin();
  for(; p_user_manage_ && it!= set_users_.end(); ++it)
  {
    //p_user_manage_->add_user(*it, it->utype);
  }
}

user& object_manage::get_user(int uid)
{
  ///TODO 如下 nFind 需要修改
  user uFind(uid, normal_user);
  user* p_user = NULL;
  std::set<user*>::iterator it = set_users_.find(&uFind);
  if (it != set_users_.end()) {
    p_user = *it;
  } else {
    // 找不到用户，生成一个默认用户，用来给其他业务流程进行判断
    p_user = new user(0, normal_user);
  }

  return *p_user;
}