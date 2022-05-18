#pragma once

class node_server_event
{
public:
	virtual ~node_server_event() {}

	virtual void on_debug(const char* p_msg) = 0;
};

class i_node_server
{
public:
	static i_node_server* create_node_server();
	static void destory_node_server(i_node_server* p_node_server);

	virtual bool init(node_server_event* p_event) = 0;
};