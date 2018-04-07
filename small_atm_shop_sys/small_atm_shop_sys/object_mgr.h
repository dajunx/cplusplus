#ifndef H_SHOP_H
#define H_SHOP_H

#include <vector>

#include "users.h"
#include "terminal.h"
#include "bank.h"
#include "db_manage.h"
#include "shop.h"

class object_manager {
public:
  object_manager() {}
  ~object_manager() {}

  static phone* get_phone() { return &phone_; }
  static bank* get_bank() { return &bank_; }
  static db* get_db() { return &db_; }

private:
  static std::vector<user> vec_users_;
  static phone phone_;
  static bank bank_;
  static db db_;
};

#endif