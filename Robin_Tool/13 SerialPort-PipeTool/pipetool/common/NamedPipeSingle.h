#pragma once

#include <wtypes.h>

#define NAMED_PIPE_SINGLE_NAME  _T("\\\\.\\pipe\\senypipedll")

enum _TYPE_DATA
{
	PIPE_LOGON,//��¼
	PIPE_EXIT//��ȡ�ͻ��˽���
};

enum _CONTENT_DATA
{
	_CONTENT_SUSPEND,	// ����
	_CONTENT_HIBER,		// ����
	_CONTENT_CONTINUE,	// ��������
	_CONTENT_BREAK		// ��ֹ����
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
// ʵ�ַ���˲�ͣ��ѭ���ṩ���񣬿ͻ���ÿͨ��һ�ξͶϿ���
DWORD WINAPI ServerPipeProc(LPVOID lParam);
BOOL ClientPipeProc(_CONTENT_DATA content);
BOOL SendPipeText(char* buf, WORD nLen);

void ServerDoPipeRecv(char* buf, WORD nLen);
BOOL ClientDoPipeRecv(char* buf, WORD nLen);

void CloseClientPipeProc(void);
void CloseServerPipeProc(void);
