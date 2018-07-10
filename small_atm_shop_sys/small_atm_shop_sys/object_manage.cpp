#include "object_manage.h"
#include "bank.h"
#include "shop.h"
#include "terminal.h"
#include "users.h"
#include "db_manage.h"
#include "tools.h"

object_manage* object_manage::p_object_manage_ = NULL;
bool object_manage::init()
{
  bool bSucc = false;
  p_shop_ = new shop();
  p_bank_ = new bank();
  p_terminal_ = new terminal();
  p_user_manage_ = new user_manage();
  p_db_ = new db();
  p_records_ = new records();

  if (p_shop_ != NULL 
    && p_bank_ != NULL 
    && p_terminal_ != NULL 
    && p_user_manage_ != NULL
    && p_db_ != NULL
    && p_records_ != NULL)
  {
    bSucc = true;
  }

  return bSucc;
}

void object_manage::add_template_users()
{
  set_users_.insert(new user(xiaohong_id, normal_user));
  set_users_.insert(new user(xiaoli_id, normal_user));
  set_users_.insert(new user(xiaowang_admin_id, admin));

  std::set<user*>::iterator it = set_users_.begin();
  for(; p_user_manage_ && it!= set_users_.end(); ++it)
  {
    p_user_manage_->add_user(*it, (*it)->utype);
  }
}

user* object_manage::get_user(int uid)
{
  ///TODO 效率低，后续优化 ljj;
  // 若找不到用户，生成一个默认用户，用来给其他业务流程进行判断;
  user* p_target_user = new user(0, normal_user);
  std::set<user*>::iterator it = set_users_.begin();
  for (; it != set_users_.end(); ++it)
  {
    if ((*it)->uid == uid) {
      p_target_user = *it;
      break;
    }
  }

  return p_target_user;
}