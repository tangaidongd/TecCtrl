// CDialogTecValid.cpp: 实现文件
//
#include "stdafx.h"
#include "TestZsTec.h"
#include "CDialogTecValid.h"
#include "afxdialogex.h"


// CDialogTecValid 对话框

CDialogTecValid::CDialogTecValid(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_VALID, pParent)
{

}

CDialogTecValid::~CDialogTecValid()
{
}

void CDialogTecValid::SetTecNumAndValid(int nTecNum, int nTecValid)
{
	m_nTecNum = nTecNum;
	m_nTecValid = nTecValid;
	if (m_nTecNum > 16)
		m_nTecNum = 16;
}

void CDialogTecValid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogTecValid, CDialog)
END_MESSAGE_MAP()


// CDialogTecValid 消息处理程序


void CDialogTecValid::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_nTecValid = 0;
	for (int i = 0; i < m_nTecNum; ++i)
	{
		CButton* pIoStatus = (CButton*)GetDlgItem(IDC_CHECK1 + i);
		pIoStatus->ShowWindow(SW_SHOW);
		int nStatus = pIoStatus->GetCheck();
		m_nTecValid = m_nTecValid | (nStatus<<i);
	}

	CDialog::OnOK();
}


void CDialogTecValid::SetCtrlStatus()
{
	int nStatus = 0;
	for (int i = 0; i < m_nTecNum; ++i)
	{
		CButton* pIoStatus = (CButton*)GetDlgItem(IDC_CHECK1 + i);
		pIoStatus->ShowWindow(SW_SHOW);
		nStatus = m_nTecValid >> i & 0x01;
		pIoStatus->SetCheck(nStatus);
	}

	for (int i = m_nTecNum; i < 16; ++i)
	{
		CButton* pIoStatus = (CButton*)GetDlgItem(IDC_CHECK1 + i);
		pIoStatus->ShowWindow(SW_HIDE);
	}
}

BOOL CDialogTecValid::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetCtrlStatus();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
