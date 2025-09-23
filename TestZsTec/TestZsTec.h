// TestZsTec.h : main header file for the TESTZSTEC application
//

#if !defined(AFX_TESTZSTEC_H__83B8C15B_9390_42E2_B72C_D49C1C018D0A__INCLUDED_)
#define AFX_TESTZSTEC_H__83B8C15B_9390_42E2_B72C_D49C1C018D0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestZsTecApp:
// See TestZsTec.cpp for the implementation of this class
//

class CTestZsTecApp : public CWinApp
{
public:
	CTestZsTecApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestZsTecApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestZsTecApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTZSTEC_H__83B8C15B_9390_42E2_B72C_D49C1C018D0A__INCLUDED_)
