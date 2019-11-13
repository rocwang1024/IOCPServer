//定义控制台应用程序的入口点。
#include "stdafx.h"
#include "IocpBase.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS

class CMyServer : public CIocpBase
{
public:
	// 新连接
	void OnConnectionAccepted(SocketContext* sockContext)
	{
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		printf("%4d-%02d-%02d %02d:%02d:%02d.%03d："
			"A connection is accepted，Current connections：%d\n",
			sys.wYear, sys.wMonth, sys.wDay,
			sys.wHour, sys.wMinute, sys.wSecond,
			sys.wMilliseconds, GetConnectCount());
	}

	// 连接关闭
	void OnConnectionClosed(SocketContext* sockContext)
	{
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		printf("%4d-%02d-%02d %02d:%02d:%02d.%03d："
			"A connection is closed，Current connections：%d\n",
			sys.wYear, sys.wMonth, sys.wDay,
			sys.wHour, sys.wMinute, sys.wSecond,
			sys.wMilliseconds, GetConnectCount());
	}

	// 连接上发生错误
	void OnConnectionError(SocketContext* sockContext, int error)
	{
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		printf("%4d-%02d-%02d %02d:%02d:%02d.%03d："
			"A connection have an error：%d，Current connections：%d\n",
			sys.wYear, sys.wMonth, sys.wDay,
			sys.wHour, sys.wMinute, sys.wSecond,
			sys.wMilliseconds, error, GetConnectCount());
	}

	// 读操作完成
	void OnRecvCompleted(SocketContext* sockContext, IoContext* ioContext)
	{
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		printf("%4d-%02d-%02d %02d:%02d:%02d.%03d：Recv data：%s \n",
			sys.wYear, sys.wMonth, sys.wDay,
			sys.wHour, sys.wMinute, sys.wSecond,
			sys.wMilliseconds, ioContext->wsaBuf.buf);
	}

	// 写操作完成
	void OnSendCompleted(SocketContext* sockContext, IoContext* ioContext)
	{
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		printf("%4d-%02d-%02d %02d:%02d:%02d.%03d：Send data successful！\n",
			sys.wYear, sys.wMonth, sys.wDay,
			sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
	}
};

int main()
{
	CMyServer* pServer = new CMyServer;

	// 开启服务
	if (pServer->Start())
	{
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		printf("%4d-%02d-%02d %02d:%02d:%02d.%03d："
			"Server start successful on port:%d... \n",
			sys.wYear, sys.wMonth, sys.wDay,
			sys.wHour, sys.wMinute, sys.wSecond,
			sys.wMilliseconds, pServer->GetPort());
	}
	else
	{
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		printf("%4d-%02d-%02d %02d:%02d:%02d.%03d：Server start failed！\n",
			sys.wYear, sys.wMonth, sys.wDay, sys.wHour,
			sys.wMinute, sys.wSecond, sys.wMilliseconds);
		return 0;
	}

	// 创建事件对象，让ServerShutdown程序能够关闭自己
	HANDLE hEvent = ::CreateEvent(NULL, FALSE, FALSE, L"ShutdownEvent");
	if (hEvent != NULL)
	{
		::WaitForSingleObject(hEvent, INFINITE);
		::CloseHandle(hEvent);
	}
	// 关闭服务
	pServer->Stop();
	delete pServer;

	SYSTEMTIME sys;
	GetLocalTime(&sys);
	printf("%4d-%02d-%02d %02d:%02d:%02d.%03d：Server is closed \n",
		sys.wYear, sys.wMonth, sys.wDay, sys.wHour,
		sys.wMinute, sys.wSecond, sys.wMilliseconds);
	return 0;
}

