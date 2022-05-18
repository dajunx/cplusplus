#include "ui_server_mgr.h"

ui_server_mgr::ui_server_mgr()
{
	m_p_ctx = nullptr;
}

ui_server_mgr::~ui_server_mgr()
{
}

bool ui_server_mgr::init(context* p_ctx)
{
	m_p_ctx = p_ctx;
	return true;
}
