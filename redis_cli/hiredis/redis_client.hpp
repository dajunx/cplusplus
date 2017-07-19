#ifndef CACHE_REDIS_CLIENT_HPP
#define CACHE_REDIS_CLIENT_HPP

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <event2/util.h>

struct event_base;
struct event;
class redis_wrapper;

//namespace is {
//
//namespace cache {

class redis_client  : boost::noncopyable
{
public:
	enum state 
	{
		disconnected,
		connecting,
		connected
	};

	redis_client(struct event_base* base);

	~redis_client();

	void set_reply_callback(const boost::function<void(void*, void*)>& cb);

	void set_connect_timeout(boost::uint32_t interval);

	void set_reconnect_interval(boost::uint32_t value);

	void connect(const char *ip, int port);
	
	int command_argv(int argc, const char **argv, const size_t *argvlen, 
		void* data);

	state get_status() const
	{
		return stat_;
	}
private:
	void handle_connect(int status);
	void handle_disconnect(int status);
	static void invoke_timer(evutil_socket_t fd, short events, void *ctx);
	void handle_timeout(evutil_socket_t fd, short events);
	void connect_helper();

	state stat_;
	struct event_base* base_;
	struct event* timer_;
	boost::scoped_ptr<redis_wrapper> core_;
	boost::function<void(void*, void*)> rep_cb_;
	boost::uint32_t conn_timeout_;
	boost::uint32_t reconn_interval_;
	std::string ip_;
	boost::uint16_t port_;
};

//}
//
//}

#endif

