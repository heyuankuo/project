#pragma once
#include "winsock2.h"
#define RECV_BUFFER_SIZE		1024 * 16
class CAccepter
{
public:
	CAccepter();
	~CAccepter();

public: 
	SOCKET m_sock;				// 客户套接字
	SOCKADDR_IN m_addr;			//	客户机地址
	HANDLE	m_hthread_recv;		// 接收句柄
	HANDLE	m_hthread_send;		// 发送句柄
	char	m_buffer_recv[RECV_BUFFER_SIZE];
	char	m_send_recv[RECV_BUFFER_SIZE];

};

