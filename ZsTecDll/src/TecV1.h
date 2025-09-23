#ifndef _TEC_V1_H_
#define _TEC_V1_H_

#include "TecBase.h"

class CTecV1: public CTecBase
{
public:
	CTecV1();
	~CTecV1();
public:
	int Init(int com1, int com2, int com3);//按bit显示是否init成功
	virtual bool Exit();
public:
	//base 1
	virtual bool SetCtrlPara(int no, double destTemp, double maxTemp, 
			double maxOutput, double maxCurrent, double maxLimtTemp, double minLimtTemp, int mode);
	virtual bool GetCtrlPara(int no, double& destTemp, double& maxTemp, 
			double& maxOutput, double& maxCurrent, double &maxLimtTemp, double &minLimtTemp, int& mode);
	virtual bool GetCurTemp(int no, double& temp);
	virtual bool IsTecOpen(int no, int& isOpen);
	virtual bool OpenTec(int no, int en);
	virtual bool IsAllTecOpen(int& isOpen);
	virtual bool OpenAllTec(int& isOpen);
	virtual bool InitLimitTemp(double maxLimtTemp, double minLimtTemp, int& retFlag);
	virtual bool InitTscNum(int no, double tscNum) { return true; };
public:
	virtual int GetActTemp(double* temp, int total);
	virtual bool JudgeTecValid(int nNo);

	virtual int GetActTempTec(double* temp, int total) { return 0; };
	virtual int GetActTempWater(double& temp, double& temp2) { return 0; };
	virtual bool CleanAlarmMsg() { return false; };
	virtual std::string ExceptionStrMsg() { return ""; };
	virtual void WriteLogMsg(std::string strMsg);




protected:
	bool SetMaxOutput(int no, double maxOutput);
	bool SetMaxCurrent(int no, double maxCurrent);
	bool SetMode(int no, int mode);
	bool GetMaxOutput(int no, double& maxOutput);
	bool GetMaxCurrent(int no, double& maxCurrent);
	bool GetMode(int no, int& mode);
	bool GetLimitTemp(int no, double& maxLimit, double& minLimit);
	bool SetLimitTemp(int no, double maxLimit, double minLimit);

	virtual CSynPComm* GetComAndCh(int no, int& chNo, int& addr);
protected:
	const char* GetParaName(int paraType);
	//将要操作的参数指令放在cmd中
	bool ExecGetCmd(CSynPComm* pComObj, int addr, const char* cmd, double& val);
	bool ExecSetCmd(CSynPComm* pComObj, int addr, const char* cmd);
public:
	double m_curTemp[20];		//数据改大，可以支持更多tec
	CSynPComm m_comObj[3];
	ZsFdb m_fdbObj;
	bool m_isThreadRun;
	bool m_threadRunState;
	int  m_total;
};

UINT ThreadReqCurTemp(LPVOID pParam);

extern CTecV1 g_tecV1Obj;

#endif//_TEC_V1_H_