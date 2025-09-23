#pragma once
#include <afxmt.h>
#include "TecBase.h"

#define  MAX_TEC_DATA  20

enum LOG_FLAG
{
	EM_FLAG_DMD_TEMP	= 0x00000001,
	EM_FLAG_ELECTRIC	= 0x00000002,
	EM_FLAG_TEC_TEMP	= 0x00000004,
	EM_FLAG_WATER_TEMP	= 0x00000008
};

struct ST_ErrorValue
{
	double dErrorFlag;
	char   zErrorMsg[50];
};


class CTecNewPro : public CTecBase
{
public:
	CTecNewPro();
	~CTecNewPro();
public:
	int Init(int com, int total, int nLogFlag, int nLogRefreshCycle, bool bIsPowerDouble);//按bit显示是否init成功
	virtual bool Exit();
public:
	//base 1
	virtual bool SetCtrlPara(int no, double destTemp, double maxTemp,
		double maxOutput, double maxCurrent, double maxLimtTemp, double minLimtTemp, int mode);
	virtual bool GetCtrlPara(int no, double& destTemp, double& maxTemp,
		double& maxOutput, double& maxCurrent, double& maxLimtTemp, double& minLimtTemp, int& mode);
	virtual bool GetCurTemp(int no, double& temp);
	virtual bool IsTecOpen(int no, int& isOpen);
	virtual bool OpenTec(int no, int en);
	virtual bool IsAllTecOpen(int& isOpen);
	virtual bool OpenAllTec(int& isOpen);
	virtual bool InitLimitTemp(double maxLimtTemp, double minLimtTemp, int& retFlag);
	virtual bool InitTscNum(int no, double tscNum) { return true; };
public:
	int GetTecNum() { return m_total; }

	virtual int GetActTemp(double* temp, int total);
	int GetActTempTec(double* temp, int total);	//Tec温度 
	int GetActTempWater(double& temp, double& temp2);			//水温 
	bool CleanAlarmMsg();						// 清除tec报警信息
	virtual std::string ExceptionStrMsg();
	std::string GetAllExceptionStrDetail(double dDmdTemp);
	virtual bool JudgeTecValid(int nNo);
	void SetSomeValue(double* pCurTemp, int* pGetTemp, int nTecNum, double dUnit = 1, bool bIsGetSuc = true);
	void WriteValue2Log();
	virtual void WriteLogMsg(std::string strMsg) {};

public:
	double m_curDmdTemp[MAX_TEC_DATA];			// dmd温度	//数据改大，可以支持更多tec
	double m_curTecTemp[MAX_TEC_DATA];			// Tec温度
	double m_curElectricTemp[MAX_TEC_DATA];		// 电流
	double m_curColdWaterTemp[MAX_TEC_DATA];	// 冷水块

	CSynPComm m_comObj[3];
	ZsFdb m_fdbObj;
	bool m_isThreadRun;
	bool m_threadRunState;
	int  m_total;
	int  m_nLogFlag;
	int  m_nLogRefreshCycle;
	bool m_bIsPowerDouble;
};

UINT ThreadReqCurTempNewPro(LPVOID pParam);

extern CTecNewPro g_tecNewProObj;


static CCriticalSection     s_CriticalSection11;          // 临界区
class CAutoCriticalSection
{
public:
	CAutoCriticalSection(CCriticalSection* p_pCS);
	~CAutoCriticalSection();

private:
	CCriticalSection* m_pCS;
};


