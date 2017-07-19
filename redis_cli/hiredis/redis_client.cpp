#include "redis_client.hpp"
#include "redis_wrapper.hpp"

#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

#include <event2/event.h>
#include <hiredis.h>

namespace {

timeval to_timeval(boost::uint32_t interval)
{
	timeval val;
	val.tv_sec = interval / 1000;
	val.tv_usec = (interval % 1000 ) * 1000;
	return val;
}

const boost::uint32_t DEFAULT_CONNECT_TIMEOUT = 4000;
const boost::uint32_t DEFAULT_RECONNECT_INTERVAL = 2000;

}

//namespace is {
//
//namespace cache {

redis_client::redis_client(struct event_base* base)
	: stat_(disconnected)
	, base_(base)
	, timer_(0)
	, conn_timeout_(DEFAULT_CONNECT_TIMEOUT)
	, reconn_interval_(DEFAULT_RECONNECT_INTERVAL)
	, port_(0)
{
}

void redis_client::connect(const char *ip, int port)
{
	ip_ = ip;
	port_ = port;

	connect_helper();
}

void redis_client::set_reply_callback(const boost::function<void(void*, void*)>& cb)
{
	rep_cb_ = cb;
}

int redis_client::command_argv(int argc, const char **argv, const size_t *argvlen, void* data)
{
	return core_->async_command_argv(argc, argv, argvlen, data);
}

void redis_client::handle_connect(int status)
{
	if (status == REDIS_OK)
	{
		printf("Connected...\n");

		stat_ = connected;
		evtimer_del(timer_);
	}
	else
	{
		printf("Connect error\n");

		stat_ = disconnected;
		timeval val = to_timeval(reconn_interval_);
		evtimer_add(timer_, &val);
	}
}

void redis_client::handle_disconnect(int)
{
	printf("Disconnected...\n");

	stat_ = disconnected;
	timeval val = to_timeval(reconn_interval_);
	evtimer_add(timer_, &val);
}

void redis_client::invoke_timer(evutil_socket_t fd, short events, void *ctx)
{
	(static_cast<redis_client*>(ctx))->handle_timeout(fd, events);
}

void redis_client::handle_timeout(evutil_socket_t fd, short events)
{
	printf("Timeout...\n");

	switch (stat_)
	{
	case disconnected:
		{
			connect_helper();
		}
	break;
	case connecting:
		{
			core_->disconnect();
		}
	break;
	default:
	break;
	}
}

void redis_client::set_connect_timeout(boost::uint32_t interval)
{
	conn_timeout_ = interval;
}

void redis_client::set_reconnect_interval(boost::uint32_t value)
{
	reconn_interval_ = value;
}

void redis_client::connect_helper()
{
	core_.reset(new redis_wrapper(base_));
	core_->set_connect_callback(boost::bind(&redis_client::handle_connect, this, boost::lambda::_1));
	core_->set_disconnect_callback(boost::bind(&redis_client::handle_disconnect, this, boost::lambda::_1));
	core_->set_reply_callback(rep_cb_);

	int ret = core_->async_connect(ip_.c_str(), port_);
	if (ret == 0)
	{
		stat_ = connecting;
	}

	if (!timer_)
	{
		timer_ = evtimer_new(base_, &redis_client::invoke_timer, this);
	}

	timeval val = to_timeval(conn_timeout_);
	evtimer_add(timer_, &val);
}

redis_client::~redis_client()
{
	if (timer_)
	{
		evtimer_del(timer_);
	}
}

//}
//
//}