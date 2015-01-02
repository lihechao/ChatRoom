
// ClientDlg.h : 头文件
//

#pragma once
#include "Resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "ClientSocket.h"

const int MAX_BUFSIZE = 1024;
#define EXIT			255
#define SERVERCLOSE		254
#define REPEATNAME		253
#define NEWCLIENT		1
#define ONLINECLIENT	2
#define MESSAGE_ALL		5
#define MESSAGE_ONE		6

#define MAX_ENTERSTYLE  5
#define MAX_LEAVESTYLE  5
const static wchar_t enterStyle[][20]={_T("一个筋斗云翻进"),
										_T("偷偷摸摸地溜进"),
										_T("大摇大摆地走进"),
										_T("活蹦乱跳地进入"),
										_T("闲庭信步地走进")};
const static wchar_t leaveStyle[][20]={_T("恋恋不舍地离开了"),
										_T("潇洒地离开了"),
										_T("落寞地转身离开了"),
										_T("脚步轻轻地离开了"),
										_T("头也不回地离开了")};
// CClientDlg 对话框
class CClientDlg : public CDialogEx
{
// 构造
public:
	CClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CLIENT_DIALOG };

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
	afx_msg void OnBnClickedSendmsg();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	CString m_usrName;
	CString m_serverIP;
	CIPAddressCtrl m_ctrlServerIP;
	UINT m_serverPort;
	CClientSocket m_clientSocket;
	CString m_sendMsg;
	CFont m_editFont;
	CEdit m_ctrlSendMsg;
public:
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnClose();

	void ProcessRecvData();
	CComboBox m_ctrlMsgStyle;
	CComboBox m_ctrlPeopleToTalk;
	CString m_publicChannel;
	CString m_privateChannel;
	CListBox m_ctrlUserOnline;
	afx_msg void OnDestroy();
	CString m_strPeopleToTalk;
};
