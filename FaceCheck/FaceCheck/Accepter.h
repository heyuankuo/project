#pragma once
#include "winsock2.h"
#define RECV_BUFFER_SIZE		1024 * 16
class CAccepter
{
public:
	CAccepter();
	~CAccepter();

public: 
	SOCKET m_sock;				// �ͻ��׽���
	SOCKADDR_IN m_addr;			//	�ͻ�����ַ
	HANDLE	m_hthread_recv;		// ���վ��
	HANDLE	m_hthread_send;		// ���;��
	char	m_buffer_recv[RECV_BUFFER_SIZE];
	char	m_send_recv[RECV_BUFFER_SIZE];

};
