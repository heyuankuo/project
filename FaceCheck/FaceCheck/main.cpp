//#include <windows.h>
#include <iostream>
#include <vector>
#include <process.h>
#include "winsock2.h"
#include "Accepter.h"
#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;
void trace(char *s)
{
	cout << s << endl;
}

void facecheck_recv(void *pclient)
{
	while (1)
	{

	}

}

void facecheck_send(void *pclient)
{

	while (1)
	{

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

	WSAEVENT    eventArray[WSA_MAXIMUM_WAIT_EVENTS];
	SOCKET      *sockArray[WSA_MAXIMUM_WAIT_EVENTS];
	int nEventTotal = 0;

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
	WSAEVENT event = ::WSACreateEvent();
	err = WSAEventSelect(sock_server, event, FD_ACCEPT | FD_CLOSE);
	if (SOCKET_ERROR == err)
	{
		trace("�첽����ʧ��");
		closesocket(sock_server);
		WSACleanup();
		return -1;
	}

	// ��ӵ�����  
	eventArray[nEventTotal] = event;
	sockArray[nEventTotal] = &sock_server;
	nEventTotal++;

	//���ܿͻ�������  
	std::vector<CAccepter> vclients(5);	// ָ���ͻ�����
	CAccepter *pclient = &vclients[0];
	while (1)
	{
		// �ж��Ƿ��п�������
		if (NULL == pclient)
		{
			trace("��������");
			continue;
		}

		// ��������
		int nIndex = ::WSAWaitForMultipleEvents(nEventTotal, eventArray, FALSE, WSA_INFINITE, FALSE);
		nIndex = nIndex - WSA_WAIT_EVENT_0;

		for (int i = nIndex; i < nEventTotal; ++i)
		{
			int ret;
			ret = ::WSAWaitForMultipleEvents(1, &eventArray[i], TRUE, 1000, FALSE);
			if (ret == WSA_WAIT_FAILED || ret == WSA_WAIT_TIMEOUT)
			{
				continue;
			}
			else
			{
				// ��ȡ������֪ͨ��Ϣ��WSAEnumNetworkEvents�������Զ����������¼�  
				WSANETWORKEVENTS event;
				::WSAEnumNetworkEvents(*sockArray[i], eventArray[i], &event);
				if (event.lNetworkEvents & FD_ACCEPT)                // ����FD_ACCEPT֪ͨ��Ϣ  
				{
					if (event.iErrorCode[FD_ACCEPT_BIT] == 0)
					{
						if (nEventTotal > WSA_MAXIMUM_WAIT_EVENTS)
						{
							trace("̫������!");
							continue;
						}
						pclient->m_sock = accept(sock_server, (sockaddr FAR*)&(pclient->m_addr), NULL);
						WSAEVENT event = ::WSACreateEvent();
						::WSAEventSelect(pclient->m_sock, event, FD_READ | FD_CLOSE | FD_WRITE);
						// ��ӵ�����  
						eventArray[nEventTotal] = event;
						sockArray[nEventTotal] = &pclient->m_sock;
						nEventTotal++;
						pclient = NULL;

						// ������д�߳�
						_beginthread(	facecheck_recv, NULL, (void *)pclient);
						_beginthread(	facecheck_send, NULL, (void *)pclient);
					}
				}
				else if (event.lNetworkEvents & FD_CLOSE)        // ����FD_CLOSE֪ͨ��Ϣ  
				{
					if (event.iErrorCode[FD_CLOSE_BIT] == 0)
					{
						::closesocket(*sockArray[i]);
						*sockArray[i] = NULL;
						for (int j = i; j < nEventTotal - 1; j++)
						{
							sockArray[j] = sockArray[j + 1];
							sockArray[j] = sockArray[j + 1];
						}
						nEventTotal--;
					}
				}
			}
				
		}
		
		// ��ѯ���е�����
		if (NULL == pclient)
		{
			for (int i = 0; i < 5; ++i)
			{
				if (NULL == vclients[i].m_sock)
				{
					pclient = &vclients[i];
					break;
				}
			}

		}
		
	}

	// 
	return 0;
}