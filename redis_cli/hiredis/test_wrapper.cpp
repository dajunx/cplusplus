#include "redis_wrapper.hpp"
#include "redis_client.hpp"
#include <event2/event.h>
#include <hiredis.h>

#ifdef WIN32
#include <winsock2.h>
#endif

#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/lambda.hpp>

const char* largv_auth[] = {"auth", "foobared"};
size_t largvlen_auth[] = {4, 8};

const char* largv[] = {"GET", "key"};
size_t largvlen[] = {3, 3};

struct foo 
{
	foo()
	{

	}

	int start()
	{
		base = event_base_new();

		client.reset(new redis_wrapper(base));
		client->set_connect_callback(boost::bind(&foo::handle_connect, this, boost::lambda::_1));
		client->set_disconnect_callback(boost::bind(&foo::handle_disconnect, this, boost::lambda::_1));
		client->set_reply_callback(boost::bind(&foo::handle_reply, this, boost::lambda::_1, boost::lambda::_2));

		int ret = client->async_connect("172.20.24.232", 6379);
		if (ret != 0)
		{
			return ret;
		}

		return 0;
	}

	void handle_connect(int err)
	{
		if (err != REDIS_OK) 
		{
			printf("Connect error\n");
			return;
		}

		printf("Connected...\n");

		::Sleep(5000);

		client->async_command_argv(2, largv_auth, largvlen_auth, "hello");
	}

	void handle_disconnect(int err)
	{
		if (err != REDIS_OK) 
		{
			printf("Disconnect error\n");
			return;
		}

		printf("Disconnected...\n");
	}

	void handle_reply(void* r, void* data)
	{
		redisReply *reply = (redisReply *)r;
		if (reply == NULL) 
			return;

		printf("argv[%s]: %s\n", (char*)data, reply->str);
	}

	int run()
	{
		return event_base_dispatch(base);
	}

	struct event_base *base;
	boost::scoped_ptr<redis_wrapper> client;	
};

struct foo2 
{
	foo2()
	{

	}

	int start()
	{
		base = event_base_new();

		client.reset(new redis_client(base));
		client->set_reply_callback(boost::bind(&foo2::handle_reply, this, boost::lambda::_1, boost::lambda::_2));

		client->connect("172.20.24.232", 6379);

		return 0;
	}

	void handle_reply(void* r, void* data)
	{
		redisReply *reply = (redisReply *)r;
		if (reply == NULL) 
			return;

		printf("argv[%s]: %s\n", (char*)data, reply->str);
	}

	int run()
	{
		return event_base_dispatch(base);
	}

	struct event_base *base;
	boost::scoped_ptr<redis_client> client;	
};

int main (int argc, char **argv) 
{
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 1), &wsaData);
#endif

	foo2 f;

	f.start();
	f.run();

	return 0;
}