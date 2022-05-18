#include <iostream>
#include "node_server.h"

i_node_server* i_node_server::create_node_server()
{
	return new node_server();
}

void i_node_server::destory_node_server(i_node_server* p_node_server)
{
	if (p_node_server)
	{
		delete p_node_server;
		p_node_server = nullptr;
	}
}

node_server::node_server()
{

}

bool node_server::init(node_server_event* p_event)
{
	return true;
}