
// DriverLoaderDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "DriverLoader.h"
#include "DriverLoaderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDriverLoaderDlg 对话框



CDriverLoaderDlg::CDriverLoaderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRIVERLOADER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDriverLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LST_INF, m_lstInf);
}

BEGIN_MESSAGE_MAP(CDriverLoaderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(BTN_LOAD, &CDriverLoaderDlg::OnBnClickedLoad)
	ON_BN_CLICKED(BTN_Run, &CDriverLoaderDlg::OnBnClickedRun)
	ON_BN_CLICKED(BTN_Stop, &CDriverLoaderDlg::OnBnClickedStop)
	ON_BN_CLICKED(BTN_UnLoad, &CDriverLoaderDlg::OnBnClickedUnload)
	ON_BN_CLICKED(BTN_ABOUT, &CDriverLoaderDlg::OnBnClickedAbout)
END_MESSAGE_MAP()


// CDriverLoaderDlg 消息处理程序

BOOL CDriverLoaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_lstInf.InsertColumn(0, _T("Driver"), LVCFMT_LEFT, 100);
	m_lstInf.InsertColumn(1, _T("Operation"), LVCFMT_LEFT, 120);
	m_lstInf.InsertColumn(2, _T("Status"), LVCFMT_LEFT,120);
	m_lstInf.InsertColumn(3, _T("Last Error"), LVCFMT_LEFT, 210);

	//设置风格
	m_lstInf.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_GRIDLINES);

	//解决Win10下的拖放文件问题
	DragAcceptFiles(TRUE);
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDriverLoaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDriverLoaderDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}


//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDriverLoaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//文件拖放
void CDriverLoaderDlg::OnDropFiles(HDROP hDropInfo) {
	TCHAR szFilePath[MAX_PATH];
	DragQueryFile(hDropInfo, 0, szFilePath, sizeof(szFilePath)); //获取文件路径
	SetDlgItemText(EDT_PATH, szFilePath);
	DragFinish(hDropInfo);                                    //拖放结束后,释放内存 
	CDialogEx::OnDropFiles(hDropInfo);
}

void CDriverLoaderDlg::OnBnClickedLoad()
{

	GetDlgItemText(EDT_PATH, m_csFilePath);

	int index = m_csFilePath.ReverseFind('\\');
	m_csFileName= m_csFilePath.Mid(index + 1);
	index = m_csFileName.Find('.');
	m_csDiverName = m_csFileName.Left(index);//获取驱动名
	m_csFileType = m_csFileName.Mid(index); //获取文件类型


	m_csOperation = "Register";

	if (m_csFilePath.GetLength() == 0)
	{
		AfxMessageBox((CString)"Please Select  File");
		return;
	}

	if (m_csFileType != (CString)".sys")
	{
		AfxMessageBox((CString)"Please Select sys File !");
		return;
	}

	m_scHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (!m_scHandle)
	{
		AfxMessageBox((CString)"打开服务控制器失败失败，请打开管理员权限运行");
		CloseServiceHandle(m_scHandle);
		return;
	}

	//加载驱动
	m_hServiceDDK = CreateService(
		m_scHandle,
		m_csFileName.GetBuffer(),//驱动服务名称
		m_csFileName.GetBuffer(),//驱动服务显示名称
		SERVICE_ALL_ACCESS,
		SERVICE_KERNEL_DRIVER,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_IGNORE,
		m_csFilePath.GetBuffer(),//路径
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);

	if (!m_hServiceDDK)
	{
		//判断驱动是否存在
		if (GetLastError() == ERROR_SERVICE_EXISTS)
		{
			MsgInf(0, (CString)"Service Exist");
			if (!m_hServiceDDK)
			{
				m_hServiceDDK = OpenService(m_scHandle, m_csFileName.GetBuffer(), SERVICE_ALL_ACCESS);
			}
		}
		else {
			CString csErrorCode;
			CString csMsg;
			csErrorCode.Format((CString)"%u", GetLastError());
			csMsg = (CString)"Error Code: ";
			MsgInf(0, csMsg + csErrorCode);
		}

	}
	else {
		//加载成功
		MsgInf(1, (CString)"Operation Success");
	}

	if (m_scHandle != NULL)
	{
		CloseServiceHandle(m_scHandle);
	}
	if (m_hServiceDDK != NULL)
	{
		CloseServiceHandle(m_hServiceDDK);
	}

}


void CDriverLoaderDlg::OnBnClickedRun()
{
	GetDlgItemText(EDT_PATH, m_csFilePath);

	int index = m_csFilePath.ReverseFind('\\');
	m_csFileName = m_csFilePath.Mid(index + 1);
	index = m_csFileName.Find('.');
	m_csDiverName = m_csFileName.Left(index);//获取驱动名
	m_csFileType = m_csFileName.Mid(index); //获取文件类型

	m_csOperation = "Run";

	if (m_csFilePath.GetLength() == 0)
	{
		AfxMessageBox((CString)"Please Select  File");
		return;
	}

	if (m_csFileType != (CString)".sys")
	{
		AfxMessageBox((CString)"Please Select sys File !");
		return;
	}

	//判断驱动是否注册
	if (!m_hServiceDDK)
	{
		AfxMessageBox((CString)"Please Register");
		return;
	}
	
	m_scHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (!m_scHandle)
	{
		AfxMessageBox((CString)"打开服务控制器失败失败，请打开管理员权限运行");
		CloseServiceHandle(m_scHandle);
		return;
	}

	m_hServiceDDK = OpenService(m_scHandle, m_csFileName.GetBuffer(), SERVICE_START);

	if (!StartService(m_hServiceDDK, NULL, NULL))
	{
		switch (GetLastError())
		{
		case ERROR_SERVICE_ALREADY_RUNNING:
			MsgInf(0, (CString)"Already Runnning");
			break;
		case ERROR_SERVICE_NOT_FOUND:
			MsgInf(0, (CString)"No Found");
			break;
		default:
			CString csErrorCode;
			CString csMsg;
			csErrorCode.Format((CString)"%u", GetLastError());
			csMsg=(CString)"Error Code: ";
			MsgInf(0, csMsg+csErrorCode);
			break;
		}
	}
	else {
		//启动成功
		MsgInf(1, (CString)"Operation Success");
	}

	if (m_scHandle != NULL)
	{
		CloseServiceHandle(m_scHandle);
	}
	if (m_hServiceDDK != NULL)
	{
		CloseServiceHandle(m_hServiceDDK);
	}

}


void CDriverLoaderDlg::OnBnClickedStop()
{
	GetDlgItemText(EDT_PATH, m_csFilePath);

	int index = m_csFilePath.ReverseFind('\\');
	m_csFileName = m_csFilePath.Mid(index + 1);
	index = m_csFileName.Find('.');
	m_csDiverName = m_csFileName.Left(index);//获取驱动名
	m_csFileType = m_csFileName.Mid(index); //获取文件类型

	m_csOperation = "Stop";

	if (m_csFilePath.GetLength() == 0)
	{
		AfxMessageBox((CString)"Please Select  File");
		return;
	}

	if (m_csFileType != (CString)".sys")
	{
		AfxMessageBox((CString)"Please Select sys File !");
		return;
	}

	m_scHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!m_scHandle)
	{
		AfxMessageBox((CString)"打开服务控制器失败失败，请打开管理员权限运行");
		CloseServiceHandle(m_scHandle);
		return;
	}

	m_hServiceDDK = OpenService(m_scHandle, m_csFileName.GetBuffer(), SC_MANAGER_ALL_ACCESS);

	SERVICE_STATUS svsta = { 0 };
	//关闭驱动
	if (!ControlService(m_hServiceDDK, SERVICE_CONTROL_STOP, &svsta))
	{
		switch (GetLastError())	
		{
		case ERROR_SERVICE_NOT_ACTIVE:
		case ERROR_SERVICE_NEVER_STARTED:
			MsgInf(0, (CString)"No Run");
			break;
		case ERROR_SERVICE_NOT_FOUND:
			MsgInf(0, (CString)"No Found");
		default:
			CString csErrorCode;
			CString csMsg;
			csErrorCode.Format((CString)"%u", GetLastError());
			csMsg = (CString)"Error Code: ";
			MsgInf(0, csMsg + csErrorCode);
			break;
		}
	}
	else
	{
		//卸载成功
		MsgInf(1, (CString)"Operation Success");
	}

	if (m_scHandle != NULL)
	{
		CloseServiceHandle(m_scHandle);
	}
	if (m_hServiceDDK != NULL)
	{
		CloseServiceHandle(m_hServiceDDK);
	}

}


void CDriverLoaderDlg::OnBnClickedUnload()
{
	GetDlgItemText(EDT_PATH, m_csFilePath);

	int index = m_csFilePath.ReverseFind('\\');
	m_csFileName = m_csFilePath.Mid(index + 1);
	index = m_csFileName.Find('.');
	m_csDiverName = m_csFileName.Left(index);//获取驱动名
	m_csFileType = m_csFileName.Mid(index); //获取文件类型

	m_csOperation = "UnRegister";

	if (m_csFilePath.GetLength() == 0)
	{
		AfxMessageBox((CString)"Please Select  File");
		return;
	}

	if (m_csFileType != (CString)".sys")
	{
		AfxMessageBox((CString)"Please Select sys File !");
		return;
	}

	m_scHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!m_scHandle)
	{
		AfxMessageBox((CString)"打开服务控制器失败失败，请打开管理员权限运行");
		CloseServiceHandle(m_scHandle);
		return;
	}

	m_hServiceDDK = OpenService(m_scHandle, m_csFileName.GetBuffer(), SC_MANAGER_ALL_ACCESS);

	//卸载驱动
	if (!DeleteService(m_hServiceDDK))
	{
		switch (GetLastError ())
		{
		case ERROR_SERVICE_DOES_NOT_EXIST:
		case ERROR_SERVICE_NOT_FOUND:
			MsgInf(0, (CString)"Service Not Found");
			break;
		default:
			CString csErrorCode;
			CString csMsg;
			csErrorCode.Format((CString)"%u", GetLastError());
			csMsg = (CString)"Error Code: ";
			MsgInf(0, csMsg + csErrorCode);
			break;
		}
	}
	else
	{
		//卸载成功
		MsgInf(1, (CString)"Operation Success");
	}

	if (m_scHandle != NULL)
	{
		CloseServiceHandle(m_scHandle);
	}
	if (m_hServiceDDK != NULL)
	{
		CloseServiceHandle(m_hServiceDDK);
	}

}



void CDriverLoaderDlg::MsgInf(int nState, CString csLastError)
{
	UINT nColIdx = 0;
	CString csState;
	if (1 == nState)
	{
		csState = "Sucess";
	}
	else
	{
		csState = "Failed";
	}

	if (m_csDiverName.GetLength() == 0)
	{
		m_lstInf.InsertItem(nItemIdx, (CString)"unKnow");
	}
	else
	{
		m_lstInf.InsertItem(nItemIdx, m_csDiverName);
	}

	m_lstInf.SetItemText(nItemIdx, ++nColIdx, m_csOperation);
	m_lstInf.SetItemText(nItemIdx, ++nColIdx, csState);
	m_lstInf.SetItemText(nItemIdx++, ++nColIdx, csLastError);

}


void CDriverLoaderDlg::OnBnClickedAbout()
{
	CAbout dlg;
	dlg.DoModal(); //以模态方式运行
}
