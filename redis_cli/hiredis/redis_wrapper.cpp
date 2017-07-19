#include "redis_wrapper.hpp"

#include <hiredis.h>
#include <async.h>
#include <adapters/libevent.h>

#include <boost/scoped_ptr.hpp>

redis_wrapper::redis_wrapper(struct event_base* base)
	: base_(base)
	, ctx_(0)
{
}

redis_wrapper::~redis_wrapper()
{
	//if (ctx_)
	//{
	//	redisAsyncFree(ctx_);
	//}
}

void redis_wrapper::set_connect_callback(const boost::function<void(int)>& cb)
{
	con_cb_ = cb;
}

void redis_wrapper::set_disconnect_callback(const boost::function<void(int)>& cb)
{
	dis_con_cb_ = cb;
}

void redis_wrapper::set_reply_callback(const boost::function<void(void*, void*)>& cb)
{
	rep_cb_ = cb;
}

int redis_wrapper::async_connect(const char *ip, int port)
{
	BOOST_ASSERT(!ctx_);

	redisAsyncContext* ctx = redisAsyncConnectV2(ip, port);
	if (ctx->err) 
	{
		return ctx->err;
	}

	ctx_ = ctx;
	ctx_->data = this;
	redisLibeventAttach(ctx_, base_);
	redisAsyncSetConnectCallback(ctx_, &redis_wrapper::handle_connect);
	redisAsyncSetDisconnectCallback(ctx_, &redis_wrapper::handle_disconnect);

	return 0;
}

int redis_wrapper::async_command_argv(int argc, const char **argv, const size_t *argvlen, 
	const boost::function<void(void*)>& cb)
{
	BOOST_ASSERT(ctx_);

	return redisAsyncCommandArgv(ctx_, &redis_wrapper::handle_reply_v1, new boost::function<void(void*)>(cb), argc, argv, argvlen);
}

int redis_wrapper::async_command_argv(int argc, const char **argv, const size_t *argvlen, void* data)
{
	BOOST_ASSERT(ctx_);

	return redisAsyncCommandArgv(ctx_, &redis_wrapper::handle_reply_v2, data, argc, argv, argvlen);
}

void redis_wrapper::handle_connect(const redisAsyncContext* c, int status)
{
	redis_wrapper* self = reinterpret_cast<redis_wrapper*>(c->data);
	if (self->con_cb_)
	{
		self->con_cb_(status);
	}
}

void redis_wrapper::handle_disconnect(const redisAsyncContext *c, int status)
{
	redis_wrapper* self = reinterpret_cast<redis_wrapper*>(c->data);
	if (self->dis_con_cb_)
	{
		self->dis_con_cb_(status);
	}
}

void redis_wrapper::handle_reply_v1(redisAsyncContext *c, void *r, void* data)
{
	boost::scoped_ptr< boost::function<void(void*)> > fun(reinterpret_cast< boost::function<void(void*)> * >(r));
	(*fun)(data);
}

void redis_wrapper::handle_reply_v2(redisAsyncContext *c, void *r, void* data)
{
	redis_wrapper* self = reinterpret_cast<redis_wrapper*>(c->data);
	if (self->rep_cb_)
	{
		self->rep_cb_(r, data);
	}
}

void redis_wrapper::disconnect()
{
	BOOST_ASSERT(ctx_);
	redisAsyncDisconnect(ctx_);
}



