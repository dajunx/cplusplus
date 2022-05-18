#include <iostream>

class node_server_mgr;
class ui_server_mgr;

struct context
{
	node_server_mgr* m_p_node;
	ui_server_mgr* m_p_ui;
};