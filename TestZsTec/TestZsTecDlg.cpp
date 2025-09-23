// TestZsTecDlg.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "TestZsTec.h"
#include "TestZsTecDlg.h"
#include "DialogSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestZsTecDlg dialog

CTestZsTecDlg::CTestZsTecDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestZsTecDlg::IDD, pParent)
	, m_getLimitMax(0)
	, m_setLimitMax(0)
	, m_getLimitMin(0)
	, m_setLimitMin(0)
{
	//{{AFX_DATA_INIT(CTestZsTecDlg)
	m_getDestTemp = 0.0;
	m_getMaxTemp = 0.0;
	m_getMaxOutput = 0.0;
	m_setDestTemp = 20.0;
	m_setMaxOutput = 10.0;
	m_setMaxTemp = 25.0;
	m_setMode = 0;
	m_isOpen = FALSE;
	m_chTotal = 0;
	m_getMaxCurrent = 0.0;
	m_setMaxCurrent = 7.0;

	m_getLimitMax = 0.0;
	m_getLimitMin = 0.0;
	m_setLimitMax = 25.0;
	m_setLimitMin = 10.0;

	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestZsTecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestZsTecDlg)
	DDX_Control(pDX, IDC_COMBO_NO, m_comboNo);
	DDX_Control(pDX, IDC_LIST_TEMP, m_tempList);
	DDX_Text(pDX, IDC_EDIT_GET_DEST_TEMP, m_getDestTemp);
	DDX_Text(pDX, IDC_EDIT_GET_MAX_TEMP, m_getMaxTemp);
	DDX_Text(pDX, IDC_EDIT_GET_MAX_OUTPUT, m_getMaxOutput);
	DDX_Text(pDX, IDC_EDIT_SET_DEST_TEMP, m_setDestTemp);
	DDV_MinMaxDouble(pDX, m_setDestTemp, 10., 30.);
	DDX_Text(pDX, IDC_EDIT_SET_MAX_OUTPUT, m_setMaxOutput);
	DDV_MinMaxDouble(pDX, m_setMaxOutput, 1., 20.);
	DDX_Text(pDX, IDC_EDIT_SET_MAX_TEMP, m_setMaxTemp);
	DDV_MinMaxDouble(pDX, m_setMaxTemp, 10., 40.);
	DDX_CBIndex(pDX, IDC_COMBO_SET_MODE, m_setMode);
	DDX_Text(pDX, IDC_EDIT_GET_MAX_CURRENT, m_getMaxCurrent);
	DDX_Text(pDX, IDC_EDIT_SET_MAX_CURRENT, m_setMaxCurrent);
	DDV_MinMaxDouble(pDX, m_setMaxCurrent, 1., 15.);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_GET__LIMIT_MAX, m_getLimitMax);
	DDX_Text(pDX, IDC_EDIT_SET__LIMIT_MAX, m_setLimitMax);
	DDX_Text(pDX, IDC_EDIT_GET__LIMIT_MIN, m_getLimitMin);
	DDX_Text(pDX, IDC_EDIT_SET__LIMIT_MIN, m_setLimitMin);
}

BEGIN_MESSAGE_MAP(CTestZsTecDlg, CDialog)
	//{{AFX_MSG_MAP(CTestZsTecDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SETTING, OnBtnSetting)
	ON_BN_CLICKED(IDC_BTN_OPEN, OnBtnOpen)
	ON_BN_CLICKED(IDC_BTN_GET, OnBtnGet)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestZsTecDlg message handlers

BOOL CTestZsTecDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	UpdateHmiNoTotal();
	SetTimer(999, 1000, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestZsTecDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestZsTecDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestZsTecDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTestZsTecDlg::OnBtnSetting() 
{
	// TODO: Add your control notification handler code here
	CDialogSetting dlg;
	if(dlg.DoModal()==IDOK)
	{
		UpdateHmiNoTotal();
	}
}

void CTestZsTecDlg::OnBtnOpen() 
{
	// TODO: Add your control notification handler code here
	if(m_isOpen)
	{
		GetTecObj().Exit();
		GetDlgItem(IDC_BTN_SETTING)->EnableWindow(TRUE);
		m_isOpen = FALSE;
		GetDlgItem(IDC_BTN_OPEN)->SetWindowText("打开");
	}
	else
	{
		int rt = GetTecObj().Init();
		if(!rt)
		{
			AfxMessageBox(GetTecObj().GetErrDesc());
			return;
		}

		int flagInit = 0;
		flagInit = GetTecObj().GetTecExistStatus();
		//if (rt != 0x3F && rt != 0xFFF && rt != 0x3FFF)
		if(rt != flagInit)
		{
			AfxMessageBox(GetTecObj().GetErrDesc());
		}
		GetDlgItem(IDC_BTN_SETTING)->EnableWindow(FALSE);
		m_isOpen = TRUE;
		GetDlgItem(IDC_BTN_OPEN)->SetWindowText("关闭");
	}
}

void CTestZsTecDlg::OnBtnGet() 
{
	// TODO: Add your control notification handler code here
	if(!m_isOpen)
		return;
	if(!UpdateData(TRUE))
		return;
	int no = m_comboNo.GetCurSel()+1;
	int getMode = 0;
	if(!GetTecObj().GetCtrlPara(no, m_getDestTemp, m_getMaxTemp, m_getMaxOutput,
		m_getMaxCurrent, m_getLimitMax, m_getLimitMin, getMode))
	{
		AfxMessageBox(GetTecObj().GetErrDesc());
		return;
	}
	int isOpen = 0;
	if(!GetTecObj().IsTecOpen(no, isOpen))
	{
		AfxMessageBox(GetTecObj().GetErrDesc());
		return;
	}
	if(getMode==0)
	{
		GetDlgItem(IDC_EDIT_GET_MODE)->SetWindowText("制冷");
	}
	else if(getMode==1)
	{
		GetDlgItem(IDC_EDIT_GET_MODE)->SetWindowText("加热");
	}
	else
	{
		GetDlgItem(IDC_EDIT_GET_MODE)->SetWindowText("双向");
	}
	if(isOpen)
	{
		GetDlgItem(IDC_EDIT_IS_RUN)->SetWindowText("启动");
	}
	else
	{
		GetDlgItem(IDC_EDIT_IS_RUN)->SetWindowText("停止");
	}
	UpdateData(FALSE);
}

void CTestZsTecDlg::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
	if(!m_isOpen)
		return;
	if(!UpdateData(TRUE))
		return;
	int no = m_comboNo.GetCurSel()+1;
	if(!GetTecObj().SetCtrlPara(no, m_setDestTemp, m_setMaxTemp, m_setMaxOutput, 
		m_setMaxCurrent, m_setLimitMax, m_setLimitMin, m_setMode))
	{
		AfxMessageBox(GetTecObj().GetErrDesc());
	}
}

void CTestZsTecDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_isOpen)
	{
		if(AfxMessageBox("是否退出", MB_YESNO)!=IDYES)
			return;
		GetTecObj().Exit();
	}	
	CDialog::OnOK();
}

void CTestZsTecDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(AfxMessageBox("是否退出", MB_YESNO)!=IDYES)
		return;
	GetTecObj().Exit();
	CDialog::OnCancel();
}

void CTestZsTecDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==999)
	{
		if(m_isOpen)
		{
			RefreshCurTemp();
		}
	}
	CDialog::OnTimer(nIDEvent);
}
void CTestZsTecDlg::RefreshCurTemp()
{
	int rt = GetTecObj().GetActTemp(m_actTemp, m_chTotal);
	if(rt<1)
		return;
	CString str;
	int warnFlag = GetTecObj().IsTempExceed();
	for(int i=0; i<m_chTotal; i++)
	{
		/*if(fabs(m_actTemp[i])<0.001)
		{
			str = "";
		}
		else
		{
			str.Format("%3.2f", m_actTemp[i]);
		}*/
		str.Format("%3.2f", m_actTemp[i]);	// 将0值也显示出来
		m_tempList.SetItemText(i, 1, str);
		if(warnFlag&(0x01<<i))
		{
			m_tempList.SetItemTextColor(i, 1, RGB(255, 0, 0));
		}
		else
		{
			m_tempList.SetItemTextColor(i, 1, RGB(0, 0, 0));
		}
	}
}
void CTestZsTecDlg::UpdateHmiNoTotal()
{
	int mode, com1, com2, com3;
	double maxTemp, minTemp;
	GetTecObj().GetPara(mode, com1, com2, com3, maxTemp, minTemp, m_chTotal);
	m_comboNo.ResetContent();
	for(int i=0; i<m_chTotal; i++)
	{
		CString str;
		str.Format("%d", i+1);
		m_comboNo.AddString(str);
	}
	m_comboNo.SetCurSel(0);

	m_tempList.DeleteAllItems();
	m_tempList.SetColumnHeader(_T("#,30;当前温度(℃),115;"));
	char strno[30];
	for(int i=0; i<m_chTotal; i++)
	{
		sprintf(strno, "%d", i+1);
		m_tempList.InsertItemEx(i, strno, "");
	}
	m_tempList.SetGridLines();
	m_tempList.SetSortable(FALSE);

	m_setLimitMin = minTemp;
	m_setLimitMax = maxTemp;
	UpdateData(false);
}

void CTestZsTecDlg::OnBtnStart() 
{
	// TODO: Add your control notification handler code here
	int no = m_comboNo.GetCurSel()+1;
	if(!GetTecObj().OpenTec(no, 1))
	{
		AfxMessageBox(GetTecObj().GetErrDesc());
	}
}

void CTestZsTecDlg::OnBtnStop() 
{
	// TODO: Add your control notification handler code here
	int no = m_comboNo.GetCurSel()+1;
	if(!GetTecObj().OpenTec(no, 0))
	{
		AfxMessageBox(GetTecObj().GetErrDesc());
	}
}
