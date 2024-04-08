// ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "..\common\NamedPipeSingle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CClientDlg �Ի���




CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
	, m_edit_send(_T(""))
	, m_edit_pipename(_T(""))
	, m_check_show(FALSE)
	, m_check_circle(FALSE)
	, m_edit_inter_time(0)
	, m_edit_recv_str(_T(""))
	, m_check_show_recv(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_edit_send);
	DDX_Control(pDX, IDC_EDIT4, m_edit_recv);
	DDX_Text(pDX, IDC_EDIT2, m_edit_pipename);
	DDX_Check(pDX, IDC_CHECK1, m_check_show);
	DDX_Check(pDX, IDC_CHECK2, m_check_circle);
	DDX_Text(pDX, IDC_EDIT3, m_edit_inter_time);
	DDV_MinMaxUInt(pDX, m_edit_inter_time, 100, 100000000);
	DDX_Text(pDX, IDC_EDIT4, m_edit_recv_str);
	DDX_Check(pDX, IDC_CHECK3, m_check_show_recv);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CClientDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CClientDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CClientDlg::OnBnClickedButtonCreate)
	ON_MESSAGE(WM_PIPE_RECV, &CClientDlg::OnPipeRecv)
	ON_MESSAGE(WM_PIPE_ERROR, &CClientDlg::OnPipeError)
	ON_MESSAGE(WM_PIPE_EXIT, &CClientDlg::OnPipeExit)
	ON_MESSAGE(WM_PIPE_SEND, &CClientDlg::OnPipeSend)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CClientDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_CHECK1, &CClientDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CClientDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CClientDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_RECV, &CClientDlg::OnBnClickedButtonClearRecv)
	ON_BN_CLICKED(IDC_BUTTON_RECV_SAVE, &CClientDlg::OnBnClickedButtonRecvSave)
END_MESSAGE_MAP()


// CClientDlg ��Ϣ�������

BOOL CClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_pNamedPipe = new CNamedPipe(m_hWnd);
	if (nullptr == m_pNamedPipe)
	{
		MessageBox(L"���������ܵ�����ʧ��");
		EndDialog(IDCANCEL);
	}
	m_edit_pipename = L"\\\\.\\pipe\\com_1";
#ifdef _DEBUG
	m_edit_pipename += L"pipe\\testNamedPipe";
#endif
	m_edit_inter_time = 1000;

	UpdateData(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);
	switch (nIDEvent)
	{
	case 1:
		{
		char buff[4096];
		int size = 0;
		if (m_check_show)
		{
			wchar_t wch[10] = { 0 };
			for (int i = 0; i < m_edit_send.GetLength(); i += 3)
			{
				if (i < m_edit_send.GetLength()-3 && m_edit_send.Mid(i).GetAt(2) != L' ')
				{
					MessageBox(L"�������ݸ�ʽ����ȷ��");
					return;
				}
				_stscanf_s(m_edit_send.Mid(i).GetBuffer(), L"%02X", &buff[size++]);
			}
		}
		else
		{
			size = WideCharToMultiByte(CP_ACP, 0, m_edit_send, -1, buff, 4096, nullptr, nullptr);
		}
		
		m_pNamedPipe->SendText(buff, size);
		}
		break;
	case 2:
		{
			
		}
		break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CClientDlg::OnBnClickedOk()
{
}

void CClientDlg::OnBnClickedCancel()
{
	if (nullptr != m_pNamedPipe)
		m_pNamedPipe->Shutdown();
	CDialog::OnCancel();
}

void CClientDlg::OnBnClickedButtonOpen()
{
	UpdateData(TRUE);
	if (!m_pNamedPipe->OpenPipe(m_edit_pipename))
		return;
	GetDlgItem(IDC_BUTTON_CREATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(FALSE);
}

void CClientDlg::OnBnClickedButtonCreate()
{
	UpdateData(TRUE);

	if (!m_pNamedPipe->CreatePipe(m_edit_pipename))
			return;
	GetDlgItem(IDC_BUTTON_CREATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(FALSE);
}

void CClientDlg::OnBnClickedButtonSend()
{
	UpdateData(TRUE);
	if (m_pNamedPipe->IsOpen())
	{
		GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
		SetTimer(1, 10, 0);
	}
	else
	{
		MessageBox(L"�ܵ�δ�򿪣�");
	}
}

LRESULT CClientDlg::OnPipeRecv(WPARAM wParam, LPARAM lParam)
{
	char* buff = (char*)wParam;
	int size = (int)lParam;

	CString strText;
	if (m_check_show_recv)
	{
		CString strTmp;
		for (int i = 0; i < size; i++)
		{
			strTmp.Format(L"%02X ", (BYTE)buff[i]);
			strText += strTmp;
		}
	}
	else
	{
		strText = (char*)wParam;
	}
	int nLength = m_edit_recv.SendMessage(WM_GETTEXTLENGTH);
	m_edit_recv.SetSel(nLength, nLength);
	m_edit_recv.ReplaceSel(strText);
	return 0;
}

LRESULT CClientDlg::OnPipeError(WPARAM wParam, LPARAM lParam)
{
	MessageBox((WCHAR*)wParam);
	return 0;
}

LRESULT CClientDlg::OnPipeSend(WPARAM wParam, LPARAM lParam)
{
	if (m_check_circle)
	{
		SetTimer(1, 1000, 0);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);
	}
	return 0;
}

LRESULT CClientDlg::OnPipeExit(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDC_BUTTON_CREATE)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(TRUE);
	return 0;
}


void CClientDlg::OnBnClickedCheck1()
{
	UpdateData(TRUE);
	ChangeHex(m_edit_send);
	UpdateData(FALSE);
	
}

void CClientDlg::ChangeHex(CString& strText)
{
	CString tmp = strText;
	strText = L"";
	if (m_check_show)
	{
		CString strTmp;
		for (int i = 0; i < tmp.GetLength(); i++)
		{
			strTmp.Format(L"%02X ", tmp.GetAt(i));
			strText += strTmp;
		}
	}
	else
	{
		wchar_t wch[5] = { 0 };
		for (int i = 0; i < tmp.GetLength(); i += 3)
		{
			if (tmp.Mid(i).GetLength() >= 3 && tmp.Mid(i).GetAt(2) != L' ')
			{
				MessageBox(L"�������ݸ�ʽ����ȷ��");
				return;
			}
			_stscanf_s(tmp.Mid(i), L"%02X", wch);
			strText += wch;
		}
	}
}



void CClientDlg::OnBnClickedCheck2()
{
	UpdateData(TRUE);
	if (m_check_circle)
	{
		GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT3)->EnableWindow(TRUE);
	}
}


void CClientDlg::OnBnClickedCheck3()
{
	UpdateData(TRUE);
}


void CClientDlg::OnBnClickedButtonClearRecv()
{
	m_edit_recv_str = L"";
	UpdateData(FALSE);
}


void CClientDlg::OnBnClickedButtonRecvSave()
{
	UpdateData(TRUE);
	CString strPath = CTime::GetCurrentTime().Format(L"%Y%m%d%H%M%S.txt");
	CStdioFile hFile;
	if (!hFile.Open(strPath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite))
	{
		strPath.Format(L"%d", GetLastError());
		MessageBox(strPath + L" �򿪻򴴽��ļ�ʧ��");
		return;
	}
	hFile.SeekToEnd();
	hFile.WriteString(m_edit_recv_str);
	hFile.Close();
}
