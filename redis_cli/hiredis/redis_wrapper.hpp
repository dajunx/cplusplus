#ifndef REDIS_WRAPPER_HPP
#define REDIS_WRAPPER_HPP

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

struct event_base;
struct redisAsyncContext;
struct redisReply;

class redis_wrapper : boost::noncopyable
{
public:
	redis_wrapper(struct event_base* base);

	~redis_wrapper();

	void set_connect_callback(const boost::function<void(int)>& cb);

	void set_disconnect_callback(const boost::function<void(int)>& cb);

	void set_reply_callback(const boost::function<void(void*, void*)>& cb);

	int async_connect(const char *ip, int port);

	int async_command_argv(int argc, const char **argv, const size_t *argvlen, 
		const boost::function<void(void*)>& cb);

	int async_command_argv(int argc, const char **argv, const size_t *argvlen, 
		void* data);

	void disconnect();
private:
	static void handle_connect(const redisAsyncContext *c, int status);
	static void handle_disconnect(const redisAsyncContext *c, int status);
	static void handle_reply_v1(redisAsyncContext *c, void *r, void* data);
	static void handle_reply_v2(redisAsyncContext *c, void *r, void* data);

	struct event_base* base_;
	struct redisAsyncContext* ctx_;
	boost::function<void(int)> con_cb_;
	boost::function<void(int)> dis_con_cb_;
	boost::function<void(void*, void*)> rep_cb_;
};

#endif
