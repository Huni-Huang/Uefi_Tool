// ClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "..\common\NamedPipe.h"
#include "resource.h"
// CClientDlg �Ի���
class CClientDlg : public CDialog
{
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	CNamedPipe* m_pNamedPipe;
	LRESULT OnPipeRecv(WPARAM wParam, LPARAM lParam);
	LRESULT OnPipeError(WPARAM wParam, LPARAM lParam);
	LRESULT OnPipeSend(WPARAM wParam, LPARAM lParam);
	LRESULT OnPipeExit(WPARAM wParam, LPARAM lParam);

	void ChangeHex(CString& strText);
public:
	afx_msg void OnBnClickedOk();
	int DoRecvData(DWORD nDist, char* buf, WORD nLen);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonCreate();
	afx_msg void OnBnClickedButtonOpen();
	CEdit m_edit_recv;
	CString m_edit_send;
	CString m_edit_pipename;
	BOOL m_check_show;
	BOOL m_check_circle;
	UINT m_edit_inter_time;
	
	afx_msg void OnBnClickedCheck1();
	CString m_edit_recv_str;
	afx_msg void OnBnClickedCheck2();
	BOOL m_check_show_recv;
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedButtonClearRecv();
	afx_msg void OnBnClickedButtonRecvSave();
};
