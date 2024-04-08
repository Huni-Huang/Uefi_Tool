#include "StdAfx.h"
#include "NamedPipeSingle.h"
#include <tchar.h>

BOOL g_bNamedPipeConnect = TRUE;
HANDLE g_hPipe = INVALID_HANDLE_VALUE;

void ServerDoPipeRecv(char* buf, WORD nLen)
{
	tagComm* pComm = (tagComm*)buf;
	switch (pComm->type)
	{
	case PIPE_LOGON:
		{
			tagContent* pContent = (tagContent*)buf;
			TCHAR szBuf[MAX_PATH];
			if (pContent->content == _CONTENT_SUSPEND)
			{
				_stprintf_s(szBuf, _T("是否继续待机？"));
			}
			else 
			{
				_stprintf_s(szBuf, _T("是否继续休眠？"));
			}
			if (::MessageBox(NULL, szBuf, _T("信息提示"), MB_OKCANCEL|MB_ICONINFORMATION) == IDOK)
			{
				pContent->content = _CONTENT_CONTINUE;
			}
			else
			{
				pContent->content = _CONTENT_BREAK;
			}
			pContent->type = PIPE_EXIT;

			if (!SendPipeText((char*)pContent, sizeof(tagContent)))
			{
				CloseHandle(g_hPipe);
				g_hPipe = INVALID_HANDLE_VALUE;
				return;
			}
		}
		break;
	case PIPE_EXIT:
		{
			CloseServerPipeProc();
		}
		break;
	default:
		break;
	}
}

BOOL ClientDoPipeRecv(char* buf, WORD nLen)
{
	BOOL bRet = TRUE;
	tagComm* pComm = (tagComm*)buf;
	switch (pComm->type)
	{
	case PIPE_EXIT:
		{
			tagContent* pContent = (tagContent*)buf;
			if (pContent->content == _CONTENT_BREAK)
			{
				bRet = FALSE;
			}
			CloseClientPipeProc();
		}
		break;
	default:
		break;
	}

	return bRet;
}


BOOL ClientPipeProc(_CONTENT_DATA content)
{
	BOOL bRet = TRUE;
	g_bNamedPipeConnect = TRUE;
	if(!WaitNamedPipe(NAMED_PIPE_SINGLE_NAME, NMPWAIT_WAIT_FOREVER))
	{
		return TRUE;
	}
	g_hPipe = CreateFile(NAMED_PIPE_SINGLE_NAME, GENERIC_READ | GENERIC_WRITE,
		0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(INVALID_HANDLE_VALUE == g_hPipe)
	{
		return TRUE;
	}

	tagContent hContent;
	hContent.type = PIPE_LOGON;
	hContent.content = content;
	if (!SendPipeText((char*)&hContent, sizeof(tagContent)))
	{
		CloseClientPipeProc();
		return TRUE;
	}
	char buf[4096];
	memset(buf, 0, sizeof(buf));
	DWORD	nRecvLen = 0;
	while (g_bNamedPipeConnect)
	{
		DWORD dwRead = 0;
		if(!ReadFile(g_hPipe, buf+nRecvLen, sizeof(buf)-nRecvLen,&dwRead,NULL))
		{
			break;
		}
		nRecvLen += dwRead;
		if (nRecvLen > 2)
		{
			WORD dwRecv = *(WORD*)buf;
			dwRecv += sizeof(WORD);

			while (dwRecv <= nRecvLen)
			{
				if (!ClientDoPipeRecv(buf+sizeof(WORD), dwRecv-2))
					bRet = FALSE;
				nRecvLen -= dwRecv;
				memcpy(buf, buf+dwRecv, nRecvLen);
				if (nRecvLen > 2)
				{
					dwRecv = *(WORD*)buf;
					dwRecv += sizeof(WORD);
				}
				else
					break;
			}
		}
	}
	CloseClientPipeProc();

	return bRet;
}

DWORD WINAPI ServerPipeProc(LPVOID lParam)
{
	CWnd* pWnd = (CWnd*)lParam;

	g_bNamedPipeConnect = TRUE;
	do
	{
		g_hPipe = CreateNamedPipe(NAMED_PIPE_SINGLE_NAME, PIPE_ACCESS_DUPLEX /*| FILE_FLAG_OVERLAPPED*/ ,
			0, 1, 1024, 1024, 0, NULL);
		if(INVALID_HANDLE_VALUE == g_hPipe)
		{
			Sleep(2000);
			continue;
		}

		if(!ConnectNamedPipe(g_hPipe, NULL))
		{
			CloseHandle(g_hPipe);
			g_hPipe = INVALID_HANDLE_VALUE;
			Sleep(2000);
			break;
		}
	
		char buf[4096];
		memset(buf, 0, sizeof(buf));
		DWORD	nRecvLen = 0;
		while (g_bNamedPipeConnect)
		{
			DWORD dwRead = 0;
			if(!ReadFile(g_hPipe, buf+nRecvLen, sizeof(buf)-nRecvLen,&dwRead,NULL))
			{
				break;
			}
			nRecvLen += dwRead;
			if (nRecvLen > 2)
			{
				WORD dwRecv = *(WORD*)buf;
				dwRecv += sizeof(WORD);

				while (dwRecv <= nRecvLen)
				{
					ServerDoPipeRecv(buf+sizeof(WORD), dwRecv-2);
					nRecvLen -= dwRecv;
					memcpy(buf, buf+dwRecv, nRecvLen);
					if (nRecvLen > 2)
					{
						dwRecv = *(WORD*)buf;
						dwRecv += sizeof(WORD);
					}
					else
						break;
				}
			}
		}
		if (g_hPipe != INVALID_HANDLE_VALUE)
		{
			DisconnectNamedPipe(g_hPipe);
			CloseHandle(g_hPipe);
			g_hPipe = INVALID_HANDLE_VALUE;
		}
	}while (g_bNamedPipeConnect);
	return 0;
}

void CloseClientPipeProc(void)
{
	g_bNamedPipeConnect = FALSE;
	if (g_hPipe != INVALID_HANDLE_VALUE)
	{
		CloseHandle(g_hPipe);
		g_hPipe = INVALID_HANDLE_VALUE;
	}
}

void CloseServerPipeProc(void)
{
	g_bNamedPipeConnect = FALSE;
	if (g_hPipe != INVALID_HANDLE_VALUE)
	{
		DisconnectNamedPipe(g_hPipe);
		CloseHandle(g_hPipe);
		g_hPipe = INVALID_HANDLE_VALUE;
	}
}

BOOL SendPipeText(char* buf, WORD nLen)
{
	char* pBuf = new char[nLen+sizeof(WORD)];
	*(WORD*)pBuf = nLen;
	memcpy(pBuf+sizeof(WORD), buf, nLen);
	if (g_hPipe != INVALID_HANDLE_VALUE)
	{
		DWORD dwRet = 0;
		DWORD dwSend = nLen + sizeof(WORD);
		DWORD dwSended = 0;
		do 
		{
			if (!WriteFile(g_hPipe, pBuf+dwSended, dwSend, &dwRet, NULL))
				break;
			if (dwSend <= dwRet)
			{
				return TRUE;
			}
			dwSend -= dwRet;
			dwSended += dwRet;
		}while(TRUE);
	}
	return FALSE;
}