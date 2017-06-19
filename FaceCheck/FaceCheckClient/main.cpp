#include <iostream>
#include <vector>
#include <WS2tcpip.h>
#include <process.h>
#include "winsock2.h"
#include "Accepter.h"
#include "FaceMessage.h"
#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;
void trace(char *s)
{
	cout << s << endl;
}

/**
 * 发送信息
 * 此函数确保所有信息发送完成
 */
int SafeSend( SOCKET sock, const char *buffer, unsigned len)
{
	unsigned tmplen = 0;
	while (tmplen < len )
	{
		int err = 0;
		err = send(sock, buffer + tmplen, len - tmplen, 0);
		if (SOCKET_ERROR == err)
		{
			if (10035 == err)
			{
				Sleep(10);
				continue;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			tmplen += err;
		}
		
	}

	return 0;
}

/**
 * 客户端发送程序
 */
unsigned _stdcall send_proc(void *accp)
{
	CAccepter *paccp = (CAccepter *)accp;
	Sleep(2000);
	while (1)
	{
		int nIndex = ::WSAWaitForMultipleEvents(1, paccp->m_pevent, TRUE, WSA_INFINITE, FALSE);
		if (nIndex == WSA_WAIT_FAILED)
		{
			// 异常消息处理
			trace("客户连接异常");
		}

		// 提取事件
		WSANETWORKEVENTS event = { 0 };
		::WSAEnumNetworkEvents(*(paccp->m_psock), *(paccp->m_pevent), &event);

		if (event.lNetworkEvents & FD_WRITE)		// 读 事件
		{
			if (event.iErrorCode[FD_WRITE_BIT] == 0)
			{

				// 创建XML报文
				CFaceMessage fm;

				fm.FaceLoadImage();

				// 发送报文
				int a = SafeSend( *(paccp->m_psock), fm.message_all, 32 + strlen(fm.str_face_xml) + strlen(fm.imgdata_base64));
				cout << "结果为：" << a << endl;
			}
		}
	}
	
}

int main( int argc, char *argv[])
{

	// 套接字初始化
	WSADATA			wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		trace("WSAStartup failed!");
		return 1;
	}

	// 创建套接字
	SOCKET sock_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sock_server)
	{
		trace("socket failed!");
		WSACleanup();//释放套接字资源;  
		return  -1;
	}

	// 指定工作方式
	WSAEVENT event_server = ::WSACreateEvent();
	int err = WSAEventSelect(sock_server, event_server, FD_READ | FD_WRITE);
	if (SOCKET_ERROR == err)
	{
		trace("异步配置失败");
		closesocket(sock_server);
		WSACleanup();
		return -1;
	}

	// 服务器套接字地址配置
	SOCKADDR_IN		addr_server;
	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(6000);
	addr_server.sin_addr.s_addr = inet_addr("192.168.1.168");

	CAccepter accp;
	accp.m_psock = &sock_server;
	accp.m_pevent = &event_server;

	err = connect(sock_server, (sockaddr *)&addr_server, sizeof addr_server);
	if (SOCKET_ERROR == err)
	{
		err = GetLastError();
		if (10035 == err)
		{

		}
		else
		{
			trace("连接失败");
		}
		
	}

	HANDLE h_sendThread = (HANDLE)_beginthreadex(NULL, NULL, send_proc, (void *)&accp, 0, NULL);

	::WaitForSingleObject(h_sendThread, INFINITE);
	CloseHandle(h_sendThread );
	return 0;
}