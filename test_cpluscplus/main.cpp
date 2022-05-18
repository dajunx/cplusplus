#include<iostream>
#include "node_server_mgr.h"
#include "ui_server_mgr.h"
#include "common.h"

int main()
{
	context ctx;
	ctx.m_p_ui = new ui_server_mgr();
	ctx.m_p_ui->init(&ctx);
	ctx.m_p_node = new node_server_mgr();
	ctx.m_p_node->init(&ctx);

	return 0;
}