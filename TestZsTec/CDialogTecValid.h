#pragma once


// CDialogTecValid 对话框

class CDialogTecValid : public CDialog
{

public:
	CDialogTecValid(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogTecValid();

// 对话框数据
	enum { IDD = IDD_DIALOG_VALID };

public:
	void SetTecNumAndValid(int nTecNum, int nTecValid);
	int GetTecValidValue() { return m_nTecValid; }
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
private:
	void SetCtrlStatus();


private:
	int m_nTecNum;
	int m_nTecValid;

public:
	virtual BOOL OnInitDialog();

};
