
// ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "afxdialogex.h"
#include "ServerDlg.h"
#include "ClientSocket.h"
#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

vector<CClientSocket*> clientList;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CServerDlg 对话框



CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerDlg::IDD, pParent)
	, m_clientCnt(0)
	, m_listenPort(2537)
	, m_maxClient(50)
	, m_srvrName(_T(""))
	, m_srvrStatus(_T("已停止"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_serverSocket.m_pSerDlg=this;
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CLIENTCNT, m_clientCnt);
	DDX_Text(pDX, IDC_LISTENPORT, m_listenPort);
	//  DDX_CBIndex(pDX, IDC_MAXCLIENT, m_maxClient);
	DDX_Control(pDX, IDC_SERVERIP, m_ctrlServerIP);
	DDX_Text(pDX, IDC_SERVERNAME, m_srvrName);
	DDX_Text(pDX, IDC_STATUS, m_srvrStatus);
	DDX_Control(pDX, IDC_MAXCLIENT, m_ctrlMaxCnt);
	DDX_Control(pDX, IDC_CHATROOMINFO, m_ctrlChatRoomInfo);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CServerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CServerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_START_STOP, &CServerDlg::OnBnClickedBtnStartStop)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CServerDlg::OnBnClickedBtnClear)
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_EXPORT, &CServerDlg::OnBnClickedBtnExport)
END_MESSAGE_MAP()


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if (!AfxSocketInit())
	{
	   AfxMessageBox(_T("Failed to Initialize Sockets"), MB_OK | MB_ICONSTOP);
	   exit(1);
	}
	char hostname[256];
	PHOSTENT hostinfo;
	CString IP;
	gethostname(hostname,sizeof(hostname));		//获取主机名
	hostinfo=gethostbyname(hostname);			//根据主机名获取主机IP

	m_srvrName=hostname;	//设置服务器名称
	IP=inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
	m_ctrlServerIP.SetWindowTextW(IP);	//设置服务器IP
	
	m_ctrlMaxCnt.AddString(_T("50"));
	m_ctrlMaxCnt.AddString(_T("40"));
	m_ctrlMaxCnt.AddString(_T("30"));
	m_ctrlMaxCnt.AddString(_T("20"));
	m_ctrlMaxCnt.AddString(_T("10"));
	m_ctrlMaxCnt.AddString(_T("5"));
	m_ctrlMaxCnt.SetCurSel(0);

	m_serverSocket.m_pSerDlg=this;
	UpdateData(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rect;
		GetClientRect(&rect);
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);   
		CBitmap bmpBackground;   

       bmpBackground.LoadBitmap(IDB_BACKGROUND); 

       BITMAP bitmap;   
       bmpBackground.GetBitmap(&bitmap);   
       CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);   

       dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 

//		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
//	CDialogEx::OnOK();
}


void CServerDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if(IDYES==MessageBox(_T("确认关闭服务器？"),_T("提示"),MB_ICONQUESTION|MB_YESNO))
	{
		CDialogEx::OnCancel();
	}
}


void CServerDlg::OnBnClickedBtnStartStop()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_srvrStatus=="已停止")	//状态为已停止，则要开启服务器监听端口
	{
		UpdateData();
		WCHAR str[3];
		m_ctrlMaxCnt.GetWindowTextW(str,3);
		m_maxClient=_ttoi(str);
		if (m_listenPort<1024)
		{
			MessageBox(_T("请选择大于1024的端口！"),_T("提示"),MB_ICONWARNING);
			return;
		}
		if (!m_serverSocket.Create(m_listenPort))
		{
			MessageBox(_T("创建套接字出错！"),_T("错误"),MB_ICONERROR);
			return;
		}
		if (!m_serverSocket.Listen())
		{
			MessageBox(_T("监听失败！"),_T("错误"),MB_ICONERROR);
			return;
		}
		GetDlgItem(IDC_BTN_START_STOP)->SetWindowTextW(_T("停止"));
		GetDlgItem(IDC_LISTENPORT)->EnableWindow(FALSE);
	}
	else		//状态为正在监听，要停止监听
	{
		GetDlgItem(IDC_BTN_START_STOP)->SetWindowTextW(_T("启动"));
		GetDlgItem(IDC_LISTENPORT)->EnableWindow(TRUE);
		m_clientCnt = 0;
		vector<CClientSocket *>::iterator iter;
		for (iter = clientList.begin(); iter != clientList.end(); )
		{
			unsigned char buf[2]={0};
			buf[0]=SERVERCLOSE;
			(*iter)->Send(buf,1);
			(*iter)->Close();
			delete (*iter);
			iter=clientList.erase(iter);
		}
		clientList.clear();
		m_serverSocket.Close();
	}

	m_srvrStatus=m_srvrStatus=="正在监听……"?"已停止":"正在监听……";
	GetDlgItem(IDC_STATUS)->SetWindowTextW(m_srvrStatus);
	UpdateData(FALSE);
}



void CServerDlg::OnBnClickedBtnClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ctrlChatRoomInfo.ResetContent();
}


HBRUSH CServerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	int CtrlID=pWnd->GetDlgCtrlID();
	switch (CtrlID)
	{
	case IDC_SERVERNAME:
	case IDC_STATUS:
	case IDC_CLIENTCNT:
		pDC->SetBkColor(RGB(255, 255, 255));
		return ::CreateSolidBrush(RGB(255, 255, 255));
	case IDC_STATIC:
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));  //字体颜色
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	default:
		break;
	}
	return hbr;
}


void CServerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_serverSocket.Close();
	CDialogEx::OnClose();
}

void CServerDlg::ProcessData(CClientSocket *clientSocket)
{
	unsigned char buf[MAX_BUFSIZE]={0};
	wchar_t wbuf[MAX_BUFSIZE]={0};
	wchar_t srcName[16]={0},dstName[16]={0};
	int len = clientSocket->Receive(buf,MAX_BUFSIZE);
	vector<CClientSocket *>::iterator iter; 
	CString log,usrName;
	CTime tm;
	switch(buf[0])
	{
	case NEWCLIENT:		//收到的是新用户上线消息，转发给所有人，并给新用户发送全部在线用户信息
		memcpy(wbuf,buf+1,len - 1);		//获取用户名
		usrName = wbuf;
		clientSocket->m_userName = usrName;		//设置用户名
		for (iter= clientList.begin(); iter < clientList.end()-1; iter++)
		{
			if((*iter) ->m_userName == usrName)
			{
				buf[0]=REPEATNAME;
				clientSocket->Send(buf,1);		//用户名重复
				clientSocket->Close();
				delete clientSocket;
				clientList.pop_back();
				m_clientCnt--;
				return;
			}
		}
		for (iter= clientList.begin(); iter < clientList.end(); iter++)
		{
			if(*iter != NULL)
			{
				(*iter)->Send(buf,len);		//转发到每个客户端
			}
		}
		Sleep(100);
		for (iter= clientList.begin(); iter < clientList.end() - 1; iter++)
		{
			if(*iter != NULL)
			{
				buf[0] = ONLINECLIENT;
				memcpy(wbuf,(*iter)->m_userName.GetBuffer(0),(*iter)->m_userName.GetLength()*2);
				memcpy(buf+1,wbuf,(*iter)->m_userName.GetLength()*2);
				clientSocket->Send(buf,(*iter)->m_userName.GetLength()*2+1); //发送在线用户的信息给新上线的用户
				Sleep(100);
			}
		}
		break;

	case MESSAGE_ALL:	//收到的是发送给所有人的聊天消息
		for (iter= clientList.begin(); iter < clientList.end(); iter++)
		{
			if(*iter != NULL)
			{
				(*iter)->Send(buf,len);		//转发到每个客户端
			}
		}
		break;
	case MESSAGE_ONE:	//收到的是发送给某个人的聊天消息
		memcpy(dstName,buf+1,16);
		memcpy(srcName,buf+17,16);
		for (iter= clientList.begin(); iter < clientList.end(); iter++)
		{
			if((*iter) -> m_userName == dstName)	//发给目的用户
			{
				(*iter)->Send(buf,len);		//转发
				break;
			}
		}
		break;
	case EXIT:		//断开连接的代码

		tm=CTime::GetCurrentTime();
		log.Format(_T("【%s : %u】与服务器断开。"),clientSocket->m_userIP,clientSocket->m_userPort);
		log=tm.Format("%Y-%m-%d %X : ")+log;
		m_ctrlChatRoomInfo.AddString(log);
		m_clientCnt--;
		UpdateData(FALSE);
		clientSocket->Close();
		for (iter = clientList.begin(); iter != clientList.end();)
		{
			if (*iter == clientSocket)		//连接断开，删除套接字
			{
				iter=clientList.erase(iter);
			}
			else		//给其他客户端转发下线的消息
			{
				(*iter)->Send(buf,len);		
				iter++;
			}
		}
		delete clientSocket;
		break;
	default:
		break;
	}
}

void CServerDlg::OnBnClickedBtnExport()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(FALSE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Text Files(*.txt)|*.txt||"),AfxGetMainWnd());
	CString strPath,strLog;
	if (dlg.DoModal() == IDOK)
	{
		strPath=dlg.GetPathName();
		if (strPath.Right(4)!=".txt")
			strPath+=".txt";
	}
	CFile file(strPath,CFile::modeCreate|CFile::modeWrite);
	for (int i = 0; i < m_ctrlChatRoomInfo.GetCount(); i++)
	{
		CString str;
		m_ctrlChatRoomInfo.GetText(i,str);
		strLog+=str;
		strLog+=_T("\r\n");
	}
	file.Write(strLog,strLog.GetLength()*2);
	file.Close();
	MessageBox(_T("导出成功！"),_T("提示"),MB_ICONINFORMATION);
}
