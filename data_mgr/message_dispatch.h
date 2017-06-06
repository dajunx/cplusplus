#ifndef MESSAGE_DISPATCH
#define MESSAGE_DISPATCH
//#include "common_def.h"
#include "data_mgr.h"

struct msg_parameter
{
  msg_parameter() : key_(NULL), value_(NULL) {}
  msg_parameter(const std::string& key, const std::string& value)
    : key_(key), value_(value) {}
  ~msg_parameter() {}

  std::string key_;
  std::string value_;
};

typedef boost::function<void(boost::shared_ptr<msg_parameter>)> fun_;

class message_mgr : public boost::enable_shared_from_this<message_mgr>
{
public:
  message_mgr()
  {
    ptr_data_mgr_ = boost::make_shared<data_mgr>();
  }
  ~message_mgr() {}

  void init()
  {
    //消息回调绑定
    fun_map_["get"] = boost::bind(&message_mgr::funget, shared_from_this(), boost::lambda::_1);
    fun_map_["set"] = boost::bind(&message_mgr::funset, shared_from_this(), boost::lambda::_1);
    fun_map_["save"] = boost::bind(&message_mgr::funsave, shared_from_this(), boost::lambda::_1);
    fun_map_["quit"] = boost::bind(&message_mgr::funquit, shared_from_this(), boost::lambda::_1);
  }

  void funset(boost::shared_ptr<msg_parameter> ptr_parameter)
  {
    std::cout<<"fun :"<<__FUNCTION__<<std::endl;
    if (ptr_parameter && ptr_data_mgr_->add_data(ptr_parameter->key_, ptr_parameter->value_)) {
      std::cout<<"fun :"<<__FUNCTION__<<", OK"<<std::endl;
    }
  }

  void funget(boost::shared_ptr<msg_parameter> ptr_parameter)
  {
    std::cout<<"fun :"<<__FUNCTION__<<std::endl;
    if (ptr_parameter && ptr_data_mgr_->get_data(ptr_parameter->key_, ptr_parameter->value_)) {
      std::cout<<"fun :"<<__FUNCTION__<<", OK"<<std::endl;
    }
  }

  void funsave(boost::shared_ptr<msg_parameter> ptr_parameter)
  {
    std::cout<<"fun :"<<__FUNCTION__<<std::endl;
  }

  void funquit(boost::shared_ptr<msg_parameter> ptr_parameter)
  {
    std::cout<<"fun :"<<__FUNCTION__<<std::endl;
  }

  void dispatch_msg(const std::string& msg, boost::shared_ptr<msg_parameter> ptr_parameter)
  {
    fun_map_[msg](ptr_parameter);
  }

  void registe_msg_mgr()
  {
  }

  bool check_point_msg(const std::string& msg)
  {
    BOOST_AUTO(it, fun_map_.find(msg));
    if (it != fun_map_.end()) {
      return true;
    }

    return false;
  }

public:
  std::map<std::string, fun_> fun_map_;
  boost::shared_ptr<data_mgr> ptr_data_mgr_;
};

#endif