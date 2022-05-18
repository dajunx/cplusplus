#pragma once
#include "i_node_server.h"
#include <iostream>

class node_server : public i_node_server
{
public:
	node_server();

	virtual bool init(node_server_event* p_event);
};