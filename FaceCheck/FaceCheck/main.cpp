//#include <windows.h>
#include <iostream>
#include <vector>
#include <process.h>
#include "winsock2.h"
#include "Accepter.h"
#include "SockStack.h"
#include "FaceMessage.h"
#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;
void trace(char *s)
{
	cout << s << endl;
}

/**
 * ���տ���
 * �������գ�����մ��
 */
int SafeRecv( SOCKET sock, char *buffer, unsigned sum  )
{
	int recvsum = 0;
	while ( 1 )
	{
		int err = recv(sock, buffer + recvsum, sum, 0);
		if (SOCKET_ERROR == err)
		{
			err = GetLastError();
			if (10035 == err)
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
		else if (err > 0)
		{
			// trace(buffer + recvsum);
			recvsum += err;			
			continue;
		}
	}
}

/**
 * 
 */
unsigned  __stdcall recv_client(void *accp)
{
	CAccepter *paccp = (CAccepter *)accp;
	Sleep(2000);
	while (1)
	{
		int nIndex = ::WSAWaitForMultipleEvents(1, paccp->m_pevent, TRUE, WSA_INFINITE, FALSE);
		if (nIndex == WSA_WAIT_FAILED)
		{
			// �쳣��Ϣ����
			trace("�ͻ������쳣");
		}

		// ��ȡ�¼�
		WSANETWORKEVENTS event = { 0 };
		::WSAEnumNetworkEvents(*(paccp->m_psock), *(paccp->m_pevent), &event);

		if (event.lNetworkEvents & FD_READ )		// �� �¼�
		{
			if (event.iErrorCode[FD_READ_BIT] == 0)
			{
				CFaceMessage facemsg;
				memset( facemsg.message_all, 0, sizeof facemsg.message_all);
				SafeRecv(*(paccp->m_psock), facemsg.message_all, 1024);

				// ����
				facemsg.GetMsgHead();
				facemsg.ParseClientXml();
			}
			else
			{
				trace("�����쳣");
			}
		}
		else if (event.lNetworkEvents & FD_CLOSE)		// �ر� �¼�
		{
			if (0 == event.iErrorCode[FD_CLOSE_BIT] || 
				10053 == event.iErrorCode[FD_CLOSE_BIT] )
			{
				trace("�ͻ��˶Ͽ�����");
				CoUninitialize();
				return 0;
			}
		}

		continue;
	}

	return 0;

}

void send_client(void *pclient)
{

	while (1)
	{

	}
}

/**
 * �ͻ����ӹ���
 */
void MangerClient(void *pclient)
{
	CSockStack *client = (CSockStack *)pclient;

	// �����ͻ�
	SOCKET sock_client = accept(client->m_sock_server, NULL, NULL);
	WSAEVENT event_client = ::WSACreateEvent();
	::WSAEventSelect(sock_client, event_client, FD_CLOSE | FD_READ);
	client->AddRecord(sock_client, event_client);		// ��ӵ�����

	CAccepter accp;
	accp.m_psock = &sock_client;
	accp.m_pevent = &event_client;

	// ������д�߳�
	HANDLE h_recv = (HANDLE)_beginthreadex(NULL, NULL, recv_client, (void *)&accp, NULL, NULL);
	// _beginthread(	client_send, NULL, (void *)pclient);

	::WaitForMultipleObjects(1, &h_recv, TRUE, INFINITE);
	CloseHandle(h_recv);

	// �����жϣ�������Դ
	closesocket(sock_client);
	for (unsigned i = 0; i < client->m_uindex; ++i)
	{
		if (client->m_socks[i] == sock_client)
		{
			client->DeleteRecord(i);
			break;
		}
	}
}

int main( int argc, char *argv[])
{
	// �׽��ֳ�ʼ��
	WSADATA			wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		trace("WSAStartup failed!");
		return 1;
	}

	// �����׽���
	SOCKET sock_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sock_server)
	{
		trace("socket failed!" );
		WSACleanup();//�ͷ��׽�����Դ;  
		return  -1;
	}

	// �������׽��ֵ�ַ����
	SOCKADDR_IN		addr_server;
	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(6000);
	addr_server.sin_addr.s_addr = INADDR_ANY;

	// ���׽���
	int err = bind(sock_server, (LPSOCKADDR)&addr_server, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == err)
	{
		trace("bind failed!");
		closesocket(sock_server);
		WSACleanup();
		return -1;
	}

	// ���� 
	err = listen(sock_server, 5);
	if (SOCKET_ERROR == err)
	{
		trace( "listen failed!" );
		closesocket(sock_server);
		WSACleanup();   
		return -1;
	}

	// ָ��������ʽ
	WSAEVENT event_server = ::WSACreateEvent();
	err = WSAEventSelect(sock_server, event_server, FD_ACCEPT | FD_CLOSE);
	if (SOCKET_ERROR == err)
	{
		trace("�첽����ʧ��");
		closesocket(sock_server);
		WSACleanup();
		return -1;
	}

	CSockStack clients; // �����׽��ֹ���ջ
	clients.m_sock_server = sock_server;

	// ��������ѭ��
	while (1)
	{
		// �ȴ��������׽����¼�
		int nIndex = ::WSAWaitForMultipleEvents( 1, &event_server, TRUE, WSA_INFINITE, FALSE);
		if (nIndex == WSA_WAIT_FAILED )
		{
			// �쳣��Ϣ����
			continue;
		}

		// ��ȡ�¼�
		WSANETWORKEVENTS event;
		::WSAEnumNetworkEvents(sock_server, event_server, &event);

		if (event.lNetworkEvents & FD_ACCEPT)		// accept �¼�
		{
			if (event.iErrorCode[FD_ACCEPT_BIT] == 0)
			{
				if (clients.m_uindex > WSA_MAXIMUM_WAIT_EVENTS - 1)
				{
					trace("̫������!");
					continue;
				}

				// �����ͻ������߳�
				trace("�ͻ��˽�������");
				_beginthread(MangerClient, NULL, (void *)&clients);
			}
		}
		
	}

	return 0;
}