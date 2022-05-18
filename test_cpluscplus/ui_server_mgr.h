#include <iostream>
#include "common.h"

class ui_server_mgr
{
public:
	ui_server_mgr();
	~ui_server_mgr();

	bool init(context* p_ctx);

	context* m_p_ctx;
};