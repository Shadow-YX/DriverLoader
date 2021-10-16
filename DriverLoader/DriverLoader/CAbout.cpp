// CAbout.cpp: 实现文件
//

#include "pch.h"
#include "DriverLoader.h"
#include "CAbout.h"
#include "afxdialogex.h"


// CAbout 对话框

IMPLEMENT_DYNAMIC(CAbout, CDialogEx)

CAbout::CAbout(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ABOUTBOX, pParent)
{

}

CAbout::~CAbout()
{
}

void CAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAbout, CDialogEx)
END_MESSAGE_MAP()


// CAbout 消息处理程序
