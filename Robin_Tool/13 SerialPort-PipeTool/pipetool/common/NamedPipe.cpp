#include "stdafx.h"
#include "NamedPipe.h"

#include <cassert>


CNamedPipe::CNamedPipe(HWND hWnd)
	: m_hPipe(INVALID_HANDLE_VALUE)
	, m_hThread(NULL)
	, m_bShutdown(false)
	, m_hWnd(hWnd)
{
	m_hEvent[0] = CreateEvent(
		NULL,    // default security attribute 
		TRUE,    // manual-reset event 
		TRUE,    // initial state = signaled 
		NULL);   // unnamed event object 
	m_hEvent[1] = CreateEvent(
		NULL,    // default security attribute 
		TRUE,    // manual-reset event 
		FALSE,    // initial state = signaled 
		NULL);   // unnamed event object 
	m_hEvent[2] = CreateEvent(
		NULL,    // default security attribute 
		TRUE,    // manual-reset event 
		FALSE,    // initial state = signaled 
		NULL);   // unnamed event object 
}


CNamedPipe::~CNamedPipe()
{
	Shutdown();
	CloseHandle(m_hEvent[0]);
	CloseHandle(m_hEvent[1]);
	CloseHandle(m_hEvent[2]);
}

bool CNamedPipe::IsOpen()
{
	return (INVALID_HANDLE_VALUE != m_hPipe);
}

bool CNamedPipe::OpenPipe(LPCTSTR lpszPipename)
{
	lstrcpy(m_szPipeName, lpszPipename);
	if (!ConnectToServer())
		return false;

	m_hThread = ::CreateThread(NULL, 0, PipeClientProc, this, 0, NULL);
	if (NULL == m_hThread)
	{
		if (NULL != m_hWnd)
			::SendMessage(m_hWnd, WM_PIPE_ERROR, (LONG)L"创建线程失败！", 0);
		return false;
	}

	return true;
}

bool CNamedPipe::ConnectToServer()
{
	if (!IsExist(m_szPipeName))
	{
		if (NULL != m_hWnd)
			::SendMessage(m_hWnd, WM_PIPE_ERROR, (LONG)L"当前有名管道不存在！", 0);
		return false;
	}

	m_hPipe = CreateFile(m_szPipeName, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (INVALID_HANDLE_VALUE == m_hPipe)
	{
		if (NULL != m_hWnd)
			::SendMessage(m_hWnd, WM_PIPE_ERROR, (LONG)L"连接有名管道失败！", 0);
		return false;
	}
	SetEvent(m_hEvent[0]);
	return true;
}

bool CNamedPipe::CreatePipe(LPCTSTR lpszPipename)
{
	if (IsExist(lpszPipename))
	{
		if (NULL != m_hWnd)
			::SendMessage(m_hWnd, WM_PIPE_ERROR, (LONG)L"当前有名管道已经存在！", 0);
		return false;
	}
	m_hPipe = CreateNamedPipe(lpszPipename, 
		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
		PIPE_WAIT, 
		1, 
		BUFSIZE, 
		BUFSIZE, 
		PIPE_TIMEOUT, 
		NULL);
	if (INVALID_HANDLE_VALUE == m_hPipe)
	{
		if (NULL != m_hWnd)
			::SendMessage(m_hWnd, WM_PIPE_ERROR, (LONG)L"创建有名管道失败！", 0);
		return false;
	}

	m_pObjectConnect.oOverlap.hEvent = m_hEvent[0];

	ConnectToNewClient();

	m_hThread = ::CreateThread(NULL, 0, PipeServerProc, this, 0, NULL);
	if (NULL == m_hThread)
	{
		if (NULL != m_hWnd)
			::SendMessage(m_hWnd, WM_PIPE_ERROR, (LONG)L"创建线程失败！", 0);
		return false;
	}

	return true;
}


void CNamedPipe::ConnectToNewClient()
{
	BOOL fConnected, fPendingIO = FALSE;

	// Start an overlapped connection for this pipe instance. 
	fConnected = ConnectNamedPipe(m_hPipe, &m_pObjectConnect.oOverlap);

	// Overlapped ConnectNamedPipe should return zero. 
	if (fConnected)
	{
		if (NULL != m_hWnd)
			::SendMessage(m_hWnd, WM_PIPE_ERROR, (LONG)L"ConnectNamedPipe 失败！", 0);
		return;
	}

	switch (GetLastError())
	{
		// The overlapped connection in progress. 
	case ERROR_IO_PENDING:
		break;
		// Client is already connected, so signal an event. 

	case ERROR_PIPE_CONNECTED:
		if (SetEvent(m_pObjectConnect.oOverlap.hEvent))
			break;

		// If an error occurs during the connect operation... 
	default:
		WCHAR szError[100];
		wsprintfW(szError, L"ConnectNamedPipe error %d code", GetLastError());
		if (NULL != m_hWnd)
			::SendMessage(m_hWnd, WM_PIPE_ERROR, (LONG)szError, 0);
		return;
	}
}

void CNamedPipe::DisconnectAndReconnect()
{
	// Disconnect the pipe instance. 
	if (!DisconnectNamedPipe(m_hPipe))
	{
		if (NULL != m_hWnd)
			::SendMessage(m_hWnd, WM_PIPE_ERROR, (LONG)L"DisconnectNamedPipe 错误！", 0);
	}

	// Call a subroutine to connect to the new client. 
	ConnectToNewClient();
}


void CNamedPipe::Shutdown()
{
	if (!m_bShutdown)
	{
		m_bShutdown = true;
		SetEvent(m_hEvent[0]);
		ClosePipe();
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
}

void CNamedPipe::ClosePipe()
{
	if (INVALID_HANDLE_VALUE == m_hPipe)
	{
		DisconnectNamedPipe(m_hPipe);
		CloseHandle(m_hPipe);
		m_hPipe = INVALID_HANDLE_VALUE;
	}
}

bool CNamedPipe::SendText(char* buff, int size)
{
	assert(INVALID_HANDLE_VALUE != m_hPipe);
	if (size > BUFSIZE)
	{
		if (NULL != m_hWnd)
			::SendMessage(m_hWnd, WM_PIPE_ERROR, (LONG)L"发送数据长度失败！", 0);
		return false;
	}
	if (0 == size)
		return true;
	
	m_pObjectSend.cbRead = size;
	memcpy_s(m_pObjectSend.chRequest, BUFSIZE, buff, size);
	
	return PostWrite(&m_pObjectSend);
}

bool CNamedPipe::IsExist(LPCTSTR lpszPipename)
{
	return (TRUE == WaitNamedPipe(lpszPipename, 2000));
}

bool CNamedPipe::PostRead(PIPE_OBJECT* pObject)
{
	pObject->oOverlap.hEvent = m_hEvent[1];
	pObject->oOverlap.Offset = 0;
	pObject->oOverlap.OffsetHigh = 0;
	pObject->dwOperator = IO_PIPE_READ;
	pObject->dwState = PIPE_STATE_NONE;
	pObject->cbRead = BUFSIZE;

	DWORD dwRet;
	if (!ReadFile(m_hPipe, pObject->chRequest, pObject->cbRead, &dwRet, &pObject->oOverlap))
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			WCHAR szError[100];
			wsprintfW(szError, L"ReadFile error %d code", GetLastError());
			if (NULL != m_hWnd)
				::SendMessage(m_hWnd, WM_PIPE_ERROR, (LONG)szError, 0);
			return false;
		}
		pObject->dwState = PIPE_STATE_PENDING;
	}
	else
	{
		SetEvent(pObject->oOverlap.hEvent);
	}
	return true;
}

bool CNamedPipe::PostWrite(PIPE_OBJECT* pObject)
{
	pObject->oOverlap.hEvent = m_hEvent[2];
	pObject->oOverlap.Offset = 0;
	pObject->oOverlap.OffsetHigh = 0;
	pObject->dwOperator = IO_PIPE_WRITE;
	pObject->dwState = PIPE_STATE_NONE;
	DWORD dwRet;
	if (!WriteFile(m_hPipe, pObject->chRequest, pObject->cbRead, &dwRet, &pObject->oOverlap))
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			WCHAR szError[100];
			wsprintfW(szError, L"WriteFile error %d code", GetLastError());
			if (NULL != m_hWnd)
				::SendMessage(m_hWnd, WM_PIPE_ERROR, (LONG)szError, 0);
			return false;
		}
		pObject->dwState = PIPE_STATE_PENDING;
	}
	else
	{
		SetEvent(pObject->oOverlap.hEvent);
	}
	return true;
}

bool CNamedPipe::PostConnect(PIPE_OBJECT* pObject)
{
	return true;
}

void CNamedPipe::DoneServer()
{
	while (1)
	{
		DWORD dwWait = WaitForMultipleObjects(
			3,
			m_hEvent,
			FALSE,
			5000);
		if (m_bShutdown || dwWait == WAIT_FAILED)
		{
			break;
		}

		if (dwWait == WAIT_TIMEOUT)
		{

		}
		else
		{
			DWORD dwRet;
			if (0 == dwWait - WAIT_OBJECT_0)
			{
				BOOL bSuccess = GetOverlappedResult(m_hPipe,
					&m_pObjectConnect.oOverlap,
					&dwRet,
					TRUE);
				ResetEvent(m_hEvent[dwWait - WAIT_OBJECT_0]);
				if (!bSuccess)
				{
					DisconnectAndReconnect();
					continue;
				}
				
				if (!PostRead(&m_pObjectRecv))
				{
					DisconnectAndReconnect();
					continue;
				}
			}
			else if (1 == dwWait - WAIT_OBJECT_0)
			{
				BOOL bSuccess = GetOverlappedResult(m_hPipe,
					&m_pObjectRecv.oOverlap,
					&dwRet,
					TRUE);
				ResetEvent(m_hEvent[dwWait - WAIT_OBJECT_0]);
				if (!bSuccess || dwRet == 0)
				{
					DisconnectAndReconnect();
					continue;
				}
				m_pObjectRecv.chRequest[dwRet] = 0;
				m_pObjectRecv.chRequest[dwRet+1] = 0;
				if (NULL != m_hWnd)
					::SendMessage(m_hWnd, WM_PIPE_RECV, (LONG)m_pObjectRecv.chRequest, dwRet);
				if (!PostRead(&m_pObjectRecv))
				{
					DisconnectAndReconnect();
					continue;
				}
			}
			else if (2 == dwWait - WAIT_OBJECT_0)
			{
				BOOL bSuccess = GetOverlappedResult(m_hPipe,
					&m_pObjectSend.oOverlap,
					&dwRet,
					TRUE);
				ResetEvent(m_hEvent[dwWait - WAIT_OBJECT_0]);
				if (!bSuccess || dwRet != m_pObjectSend.cbRead)
				{
					DisconnectAndReconnect();
					continue;
				}
				if (NULL != m_hWnd)
					::SendMessage(m_hWnd, WM_PIPE_SEND, 0, 0);
			}
		}
	}
}

DWORD WINAPI CNamedPipe::PipeServerProc(LPVOID lParam)
{
	CNamedPipe* pNamedPipe = (CNamedPipe*)lParam;

	pNamedPipe->DoneServer();


	return 0;
}

void CNamedPipe::DoneClient()
{
	while (1)
	{
		DWORD dwWait = WaitForMultipleObjects(
			3,
			m_hEvent,
			FALSE,
			5000);
		if (m_bShutdown || dwWait == WAIT_FAILED)
		{
			break;
		}

		if (dwWait == WAIT_TIMEOUT)
		{

		}
		else
		{
			DWORD dwRet;
			if (0 == dwWait - WAIT_OBJECT_0)
			{
				if (!PostRead(&m_pObjectRecv))
				{
					if (!ConnectToServer())
						break;
					continue;
				}
				ResetEvent(m_hEvent[dwWait - WAIT_OBJECT_0]);
			}
			else if (1 == dwWait - WAIT_OBJECT_0)
			{
				BOOL bSuccess = GetOverlappedResult(m_hPipe,
					&m_pObjectRecv.oOverlap,
					&dwRet,
					TRUE);
				ResetEvent(m_hEvent[dwWait - WAIT_OBJECT_0]);
				if (!bSuccess || dwRet == 0)
				{
					if (!ConnectToServer())
						break;
					continue;
				}
				m_pObjectRecv.chRequest[dwRet] = 0;
				m_pObjectRecv.chRequest[dwRet + 1] = 0;
				if (NULL != m_hWnd)
					::SendMessage(m_hWnd, WM_PIPE_RECV, (LONG)m_pObjectRecv.chRequest, dwRet);
				if (!PostRead(&m_pObjectRecv))
				{
					DisconnectAndReconnect();
					continue;
				}
			}
			else if (2 == dwWait - WAIT_OBJECT_0)
			{
				BOOL bSuccess = GetOverlappedResult(m_hPipe,
					&m_pObjectSend.oOverlap,
					&dwRet,
					TRUE);
				ResetEvent(m_hEvent[dwWait - WAIT_OBJECT_0]);
				if (!bSuccess || dwRet != m_pObjectSend.cbRead)
				{
					if (!ConnectToServer())
						break;
					continue;
				}
				if (NULL != m_hWnd)
					::SendMessage(m_hWnd, WM_PIPE_SEND, 0, 0);
			}
		}
	}
}

DWORD WINAPI CNamedPipe::PipeClientProc(LPVOID lParam)
{
	CNamedPipe* pNamedPipe = (CNamedPipe*)lParam;

	pNamedPipe->DoneClient();

	return 0;
}