#pragma once

#include <wtypes.h>

#define NAMED_PIPE_SINGLE_NAME  _T("\\\\.\\pipe\\senypipedll")

enum _TYPE_DATA
{
	PIPE_LOGON,//登录
	PIPE_EXIT//获取客户端进程
};

enum _CONTENT_DATA
{
	_CONTENT_SUSPEND,	// 待机
	_CONTENT_HIBER,		// 休眠
	_CONTENT_CONTINUE,	// 继续操作
	_CONTENT_BREAK		// 终止操作
};
typedef struct _tagComm
{
	_TYPE_DATA type;
}tagComm;

typedef struct _tagContent : public _tagComm
{
	_CONTENT_DATA content;
}tagContent;

extern BOOL g_bNamedPipeConnect;

// 2013-5-29
// 实现服务端不停的循环提供服务，客户端每通信一次就断开。
DWORD WINAPI ServerPipeProc(LPVOID lParam);
BOOL ClientPipeProc(_CONTENT_DATA content);
BOOL SendPipeText(char* buf, WORD nLen);

void ServerDoPipeRecv(char* buf, WORD nLen);
BOOL ClientDoPipeRecv(char* buf, WORD nLen);

void CloseClientPipeProc(void);
void CloseServerPipeProc(void);
