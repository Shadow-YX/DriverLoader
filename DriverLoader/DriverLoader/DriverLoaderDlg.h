
// DriverLoaderDlg.h: 头文件
//

#pragma once
#include<Winsvc.h>
#include "CAbout.h"

// CDriverLoaderDlg 对话框
class CDriverLoaderDlg : public CDialogEx
{
// 构造
public:
	CDriverLoaderDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRIVERLOADER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:

	//驱动状态信息列表
	CString m_csFileType;
	CString m_csFilePath;
	CString m_csFileName;
	CString m_csDiverName;
	CString m_csOperation;
	UINT nItemIdx;

	SC_HANDLE m_scHandle;
	SC_HANDLE m_hServiceDDK;
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
	afx_msg void OnBnClickedLoad();
	void MsgInf(int nState, CString csLastError);
	afx_msg void OnDropFiles(HDROP hDropInfo);

	CListCtrl m_lstInf;
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedUnload();
	afx_msg void OnBnClickedAbout();
};
