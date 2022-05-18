#include "node_server_mgr.h"

node_server_mgr::node_server_mgr()
{
	m_p_node = i_node_server::create_node_server();
}

node_server_mgr::~node_server_mgr()
{
	if (m_p_node)
	{
		i_node_server::destory_node_server(m_p_node);
		m_p_node = nullptr;
	}
}

bool node_server_mgr::init(context* p_ctx)
{
	m_p_ctx = p_ctx;
	return true;
}

i_node_server* node_server_mgr::get_node_server()
{
	return m_p_node;
}
