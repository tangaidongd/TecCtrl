// DialogSetting.cpp : implementation file
//

#include "stdafx.h"
#include <io.h>
#include "TestZsTec.h"
#include "DialogSetting.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSetting dialog
CDialogSetting::CDialogSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetting::IDD, pParent)
	, m_com4(0)
	, m_com5(0)
	, m_com6(0)
	, m_com7(0)
{
	//{{AFX_DATA_INIT(CDialogSetting)
	m_com = 0;
	m_com1 = 0;
	m_com2 = 0;
	m_com3 = 0;
	m_maxTemp = 0.0;
	m_mode = -1;
	m_isDebug = FALSE;
	m_minTemp = 0.0;
	m_nTecValid = 0x3F;

	m_mapTotalAndValid.clear();
	//}}AFX_DATA_INIT
}


void CDialogSetting::InitTecValid()
{
	m_mapTotalAndValid.clear();
	STTecTotalAndValid stTecTotalAndValid;

	stTecTotalAndValid.nTecValid = 0x3F;
	stTecTotalAndValid.nTecTotal = 6;
	m_mapTotalAndValid[TEC_3CARD_2CH] = stTecTotalAndValid;
	
	stTecTotalAndValid.nTecValid = 0x3F;
	stTecTotalAndValid.nTecTotal = 6;
	m_mapTotalAndValid[TEC_6CARD_1CH] = stTecTotalAndValid;

	stTecTotalAndValid.nTecValid = 0xFFF;
	stTecTotalAndValid.nTecTotal = 12;
	m_mapTotalAndValid[TEC_12CARD_1CH] = stTecTotalAndValid;

	stTecTotalAndValid.nTecValid = 0x3F;
	stTecTotalAndValid.nTecTotal = 6;
	m_mapTotalAndValid[TEC_6CARD_1CH_NEW] = stTecTotalAndValid;

	stTecTotalAndValid.nTecValid = 0xFFF;
	stTecTotalAndValid.nTecTotal = 12;
	m_mapTotalAndValid[TEC_6CARD_2CH_NEW] = stTecTotalAndValid;

	stTecTotalAndValid.nTecValid = 0x3FFF;
	stTecTotalAndValid.nTecTotal = 14;
	m_mapTotalAndValid[TEC_7CARD_2CH_NEW] = stTecTotalAndValid;
}

void CDialogSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSetting)
	DDX_Text(pDX, IDC_EDIT_COM, m_com);
	DDX_Text(pDX, IDC_EDIT_COM1, m_com1);
	DDX_Text(pDX, IDC_EDIT_COM2, m_com2);
	DDX_Text(pDX, IDC_EDIT_COM3, m_com3);
	DDX_Text(pDX, IDC_EDIT_MAX_TEMP, m_maxTemp);
	DDV_MinMaxDouble(pDX, m_maxTemp, 10., 100.);
	DDX_Radio(pDX, IDC_RADIO_MODE, m_mode);
	DDX_Check(pDX, IDC_CHECK_DEBUG, m_isDebug);
	DDX_Text(pDX, IDC_EDIT_MIN_TEMP, m_minTemp);
	DDV_MinMaxDouble(pDX, m_minTemp, 1., 90.);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_COM4, m_com4);
	DDX_Text(pDX, IDC_EDIT_COM5, m_com5);
	DDX_Text(pDX, IDC_EDIT_COM6, m_com6);
	DDX_Text(pDX, IDC_EDIT_COM7, m_com7);
}


BEGIN_MESSAGE_MAP(CDialogSetting, CDialog)
	//{{AFX_MSG_MAP(CDialogSetting)
	ON_BN_CLICKED(IDC_RADIO_MODE, OnRadioMode)
	ON_BN_CLICKED(IDC_RADIO_MODE2, OnRadioMode2)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_MODE3, &CDialogSetting::OnBnClickedRadioMode3)
	ON_BN_CLICKED(IDC_RADIO_MODE4, &CDialogSetting::OnBnClickedRadioMode4)
	ON_BN_CLICKED(IDC_RADIO_MODE5, &CDialogSetting::OnBnClickedRadioMode5)
	ON_BN_CLICKED(IDC_BTN_VALID, &CDialogSetting::OnBnClickedBtnValid)
	ON_BN_CLICKED(IDC_RADIO_MODE6, &CDialogSetting::OnBnClickedRadioMode6)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSetting message handlers

BOOL CDialogSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	// TODO: Add extra initialization here
	int mode, com1, com2, com3, chTotal;
	double maxTemp, minTemp;
	GetTecObj().GetPara(mode, com1, com2, com3, maxTemp, minTemp, chTotal);
	m_mode = mode;
	m_com1 = com1;
	m_com2 = com2;
	m_com3 = com3;
	m_com = com1;
	m_com4 = com1;
	m_com5 = com1;
	m_com6 = com1;
	m_com7 = com1;
	m_maxTemp = maxTemp;
	m_minTemp = minTemp;
	UpdateUi();

	TCHAR exeFullPath[MAX_PATH];
	GetModuleFileName(NULL, exeFullPath, MAX_PATH);
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	_splitpath(exeFullPath, drive, dir, NULL, NULL);
	sprintf(m_debugFile, "%s%s\\tec.debug", drive, dir);
	if(_access(m_debugFile, 0)==-1)
		m_isDebug = FALSE;
	else
		m_isDebug = TRUE;

	InitTecValid();
	if (m_mapTotalAndValid.count(m_mode))
	{
		m_mapTotalAndValid[m_mode].nTecValid = GetTecObj().GetTecExistStatus();
		//m_mapTotalAndValid[m_mode].nTecTotal = chTotal;
		m_DialogTecValid.SetTecNumAndValid(m_mapTotalAndValid[m_mode].nTecTotal, m_mapTotalAndValid[m_mode].nTecValid);
	}

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogSetting::OnOK() 
{
	// TODO: Add extra validation here
	if(!UpdateData(TRUE))
		return;
	if (m_mapTotalAndValid.count(m_mode) < 1)
	{
		MessageBox("有效位未查找到此模式");
		return;
	}

	int mode = m_mode;
	switch(mode)
	{
	case TEC_3CARD_2CH:
		GetTecObj().SetPara(m_com1, m_com2, m_com3, TEC_3CARD_2CH);
		break;
	case TEC_6CARD_1CH:
		GetTecObj().SetPara(m_com, TEC_6CARD_1CH);
		break;
	case TEC_12CARD_1CH:
		GetTecObj().SetPara(m_com7, TEC_12CARD_1CH);
		break;
	case TEC_6CARD_2CH_NEW:
		GetTecObj().SetPara(m_com4, TEC_6CARD_2CH_NEW);
		break;
	case TEC_7CARD_2CH_NEW:
		GetTecObj().SetPara(m_com5, TEC_7CARD_2CH_NEW);
		break;
	case TEC_6CARD_1CH_NEW:
		GetTecObj().SetPara(m_com6, TEC_6CARD_1CH_NEW);
		break;
	default:
		ASSERT(FALSE);
		return;
	}

	// 保留设置的有效位置
	GetTecObj().SetTecExistStatus(m_mapTotalAndValid[mode].nTecValid);
	if(m_maxTemp < m_minTemp)
	{
		double temp = m_maxTemp;
		m_maxTemp = m_minTemp;
		m_minTemp = temp;
	}
	GetTecObj().SetLmtTemp(m_maxTemp, m_minTemp);
	GetTecObj().Save();

	if(m_isDebug)
	{
		if(_access(m_debugFile, 0)==-1)
		{
			FILE* fp = fopen(m_debugFile, "w");
			if(fp)
			{
				fclose(fp);
			}
		}
	}
	else
	{
		if(_access(m_debugFile, 0)!=-1)
		{
			DeleteFile(m_debugFile);
		}
	}

	CDialog::OnOK();
}

void CDialogSetting::UpdateUi()
{
	if(m_mode== TEC_3CARD_2CH)
	{
		GetDlgItem(IDC_EDIT_COM7)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COM2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COM3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAX_TEMP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MIN_TEMP)->EnableWindow(TRUE);
	}
	if (m_mode == TEC_6CARD_1CH)
	{
		GetDlgItem(IDC_EDIT_COM7)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COM4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAX_TEMP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MIN_TEMP)->EnableWindow(TRUE);
	}
	if (m_mode == TEC_6CARD_2CH_NEW)
	{
		GetDlgItem(IDC_EDIT_COM7)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COM)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAX_TEMP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MIN_TEMP)->EnableWindow(FALSE);
	}
	if (m_mode == TEC_7CARD_2CH_NEW)
	{
		GetDlgItem(IDC_EDIT_COM6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM5)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COM4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAX_TEMP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MIN_TEMP)->EnableWindow(FALSE);
	}
	if (m_mode == TEC_6CARD_1CH_NEW)
	{
		GetDlgItem(IDC_EDIT_COM7)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM6)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COM5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAX_TEMP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MIN_TEMP)->EnableWindow(FALSE);
	}
	if (m_mode == TEC_12CARD_1CH)
	{
		GetDlgItem(IDC_EDIT_COM7)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COM6)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM5)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM4)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COM3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAX_TEMP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MIN_TEMP)->EnableWindow(TRUE);
	}
}

void CDialogSetting::OnRadioMode() 
{
	// TODO: Add your control notification handler code here
	m_mode = TEC_3CARD_2CH;
	UpdateUi();
}

void CDialogSetting::OnRadioMode2() 
{
	// TODO: Add your control notification handler code here
	m_mode = TEC_6CARD_1CH;
	UpdateUi();
}


void CDialogSetting::OnBnClickedRadioMode3()
{
	m_mode = TEC_6CARD_2CH_NEW;
	UpdateUi();
}


void CDialogSetting::OnBnClickedRadioMode4()
{
	m_mode = TEC_7CARD_2CH_NEW;
	UpdateUi();
}


void CDialogSetting::OnBnClickedRadioMode5()
{
	m_mode = TEC_6CARD_1CH_NEW;
	UpdateUi();
}


void CDialogSetting::OnBnClickedRadioMode6()
{
	m_mode = TEC_12CARD_1CH;
	UpdateUi();
}



void CDialogSetting::OnBnClickedBtnValid()
{
	if (m_mapTotalAndValid.count(m_mode) < 1)
	{
		MessageBox("没有此模式");
		return;
	}

	m_DialogTecValid.SetTecNumAndValid(m_mapTotalAndValid[m_mode].nTecTotal, m_mapTotalAndValid[m_mode].nTecValid);
	if (m_DialogTecValid.DoModal() == IDOK)
	{
		int nTecValid = m_DialogTecValid.GetTecValidValue();
		m_mapTotalAndValid[m_mode].nTecValid = nTecValid;
	}
}

