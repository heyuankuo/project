#include <iostream>
#include <vector>
#include <WS2tcpip.h>
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")

using std::cout;
using std::endl;
void trace(char *s)
{
	cout << s << endl;
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
	HANDLE hEventObject = CreateEvent(NULL, TRUE, FALSE, NULL);
	int err = WSAEventSelect(sock_server, hEventObject, FD_READ | FD_WRITE);
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

	err = connect(sock_server, (sockaddr *)&addr_server, sizeof addr_server);
	if (SOCKET_ERROR == err)
	{
		err = GetLastError();
		trace( "连接失败");
	}

	return 0;
}