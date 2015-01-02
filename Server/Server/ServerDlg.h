
// ServerDlg.h : 头文件
//
#pragma once
#include "Resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "ServerSocket.h"
#include "ClientSocket.h"

#define EXIT			255
#define SERVERCLOSE		254
#define REPEATNAME		253
#define NEWCLIENT		1
#define ONLINECLIENT	2
#define MESSAGE_ALL		5
#define MESSAGE_ONE		6

const int MAX_BUFSIZE=1024;
// CServerDlg 对话框
class CServerDlg : public CDialogEx
{
// 构造
public:
	CServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnStartStop();
private:

	short m_listenPort;
	CIPAddressCtrl m_ctrlServerIP;
	CString m_srvrName;			//服务器的主机名
	CString m_srvrStatus;		//服务器的状态，可以为停止或正在监听
	CComboBox m_ctrlMaxCnt;	
public:
	CServerSocket m_serverSocket;
	UINT m_clientCnt;
	UINT m_maxClient;
	CListBox m_ctrlChatRoomInfo;
	afx_msg void OnBnClickedBtnClear();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();

	void ProcessData(CClientSocket *clientSocket);
	
	afx_msg void OnBnClickedBtnExport();
};
