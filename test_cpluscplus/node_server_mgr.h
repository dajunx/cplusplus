#include "i_node_server.h"
#include "common.h"

class node_server_mgr
{
public:
	node_server_mgr();
	~node_server_mgr();

	bool init(context* p_ctx);

	i_node_server* get_node_server();

	i_node_server* m_p_node;
	context* m_p_ctx;
};