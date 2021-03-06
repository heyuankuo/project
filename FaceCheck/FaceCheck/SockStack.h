#pragma once
#include "winsock2.h"
class CSockStack
{
public:
	CSockStack() :m_uindex(0)
	{
		memset(&m_events, 0, sizeof m_events);
		memset(&m_socks, 0, sizeof m_socks);
	}
	~CSockStack();

public:
	/**
	 * 添加套接字-事件
	 * @param		sock		[in]		套接字描述
	 * @param		event		[in]		事件句柄
	 */
	int AddRecord( SOCKET sock, WSAEVENT event);

	/**
	 * 删除套接字-事件
	 * @param		index		[in]		要删除的项目的索引
	 */
	int DeleteRecord( unsigned index);

public:

	SOCKET		m_sock_server;
	SOCKET      m_socks[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT    m_events[WSA_MAXIMUM_WAIT_EVENTS];
	
	ULONG		m_uindex; // 栈顶
};

