// TestZsTecDlg.h : header file
//

#if !defined(AFX_TESTZSTECDLG_H__9ABBB649_98D9_403E_864B_2A5C860C897D__INCLUDED_)
#define AFX_TESTZSTECDLG_H__9ABBB649_98D9_403E_864B_2A5C860C897D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "module\\gui\\reportctrl\\reportctrl.h"
/////////////////////////////////////////////////////////////////////////////
// CTestZsTecDlg dialog


#include "module\libs\ZsTecDll\zsTecDll.h"
#pragma comment(lib, ".\\Debug\\ZsTecDll.lib")

class CTestZsTecDlg : public CDialog
{
// Construction
public:
	CTestZsTecDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestZsTecDlg)
	enum { IDD = IDD_TESTZSTEC_DIALOG };
	CComboBox	m_comboNo;
	CReportCtrl	m_tempList;
	double	m_getDestTemp;
	double	m_getMaxTemp;
	double	m_getMaxOutput;
	double	m_setDestTemp;
	double	m_setMaxOutput;
	double	m_setMaxTemp;
	int		m_setMode;
	double	m_getMaxCurrent;
	double	m_setMaxCurrent;

	// ³¬ÎÂ¸æ¾¯
	double m_getLimitMax;
	double m_setLimitMax;
	double m_getLimitMin;
	double m_setLimitMin;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestZsTecDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestZsTecDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnSetting();
	afx_msg void OnBtnOpen();
	afx_msg void OnBtnGet();
	afx_msg void OnBtnSet();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnStart();
	afx_msg void OnBtnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void UpdateHmiNoTotal();
	void RefreshCurTemp();
public:
	BOOL m_isOpen;
	int m_chTotal;
//	double m_maxTemp, m_minTemp;
	double m_actTemp[14];
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTZSTECDLG_H__9ABBB649_98D9_403E_864B_2A5C860C897D__INCLUDED_)
