#if !defined(AFX_DIALOGSETTING_H__14FA2CE1_B8E1_47C4_A797_438B574D4A44__INCLUDED_)
#define AFX_DIALOGSETTING_H__14FA2CE1_B8E1_47C4_A797_438B574D4A44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSetting.h : header file
//
#include <map>


typedef struct STTecTotalAndValid
{
	int	nTecValid;
	int nTecTotal;
};


/////////////////////////////////////////////////////////////////////////////
// CDialogSetting dialog
#include "CDialogTecValid.h"

class CDialogSetting : public CDialog
{
// Construction
public:
	CDialogSetting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogSetting)
	enum { IDD = IDD_DIALOG_SETTING };
	int		m_com;
	int		m_com1; 
	int		m_com2;
	int		m_com3;
	int		m_com4;
	int		m_com5;
	int		m_com6;
	int		m_com7;
	double	m_maxTemp;
	int		m_mode;
	BOOL	m_isDebug;
	double	m_minTemp;
	int		m_nTecValid;		// Tec有效位
	int		m_nTecTotal;		// Tec总数
	//}}AFX_DATA
	std::map<int, STTecTotalAndValid> m_mapTotalAndValid;

private:
	CDialogTecValid m_DialogTecValid;

private:
	void InitTecValid();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSetting)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioMode();
	afx_msg void OnRadioMode2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
	void UpdateUi();
	char m_debugFile[255];
	
	afx_msg void OnBnClickedRadioMode3();
	afx_msg void OnBnClickedRadioMode4();
	
	afx_msg void OnBnClickedRadioMode5();
	
	afx_msg void OnBnClickedBtnValid();
	
	
	afx_msg void OnBnClickedRadioMode6();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSETTING_H__14FA2CE1_B8E1_47C4_A797_438B574D4A44__INCLUDED_)
