#pragma once

#include <wtypes.h>


#define WM_PIPE_ERROR	(WM_USER+1)
#define WM_PIPE_RECV	(WM_USER+2)
#define WM_PIPE_SEND	(WM_USER+3)
#define WM_PIPE_EXIT	(WM_USER+4)

#define CONNECTING_STATE 0 
#define READING_STATE 1 
#define WRITING_STATE 2 
#define INSTANCES 4 
#define PIPE_TIMEOUT 5000
#define BUFSIZE 4096

typedef struct
{
	OVERLAPPED oOverlap;
	char chRequest[BUFSIZE];
	DWORD cbRead;
	DWORD dwState;
#define PIPE_STATE_NONE 0
#define PIPE_STATE_PENDING 1
	DWORD dwOperator;
#define IO_PIPE_CONNECT	0  // 服务端接收客户端连接
#define IO_PIPE_READ	1  // 接收数据
#define IO_PIPE_WRITE	2  // 发送数据
} PIPE_OBJECT;

class CNamedPipe
{
public:
	CNamedPipe(HWND hWnd);
	virtual ~CNamedPipe();

	bool OpenPipe(LPCTSTR lpszPipename);
	bool CreatePipe(LPCTSTR lpszPipename);
	
	bool SendText(char* buff, int size);

	bool IsExist(LPCTSTR lpszPipename);

	void Shutdown();

	bool IsOpen();
private:
	void ClosePipe();
	void ConnectToNewClient();
	bool ConnectToServer();
	void DisconnectAndReconnect();
	bool PostRead(PIPE_OBJECT* pObject);
	bool PostWrite(PIPE_OBJECT* pObject);
	bool PostConnect(PIPE_OBJECT* pObject);
private:
	static DWORD WINAPI PipeClientProc(LPVOID lParam);
	static DWORD WINAPI PipeServerProc(LPVOID lParam);
	void DoneServer();
	void DoneClient();
private:
	HANDLE m_hPipe;
	HANDLE m_hEvent[3];
	HANDLE m_hThread;
	PIPE_OBJECT m_pObjectConnect;
	PIPE_OBJECT m_pObjectRecv;
	PIPE_OBJECT m_pObjectSend;
	bool m_bShutdown;
	HWND m_hWnd;
	wchar_t m_szPipeName[MAX_PATH];
};

