#if !defined(AFX_DIALOGPASSWORD_H__DDAD4C3C_45DF_427E_8D90_F26EC1D128FA__INCLUDED_)
#define AFX_DIALOGPASSWORD_H__DDAD4C3C_45DF_427E_8D90_F26EC1D128FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogPassword.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPassword dialog

class CDialogPassword : public CDialog
{
// Construction
public:
	CDialogPassword(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogPassword)
	enum { IDD = IDD_DIALOG_PASSWORD };
	CString	m_password;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogPassword)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogPassword)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPASSWORD_H__DDAD4C3C_45DF_427E_8D90_F26EC1D128FA__INCLUDED_)
