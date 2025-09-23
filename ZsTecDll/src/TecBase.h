#ifndef _TEC_BASE_H_
#define _TEC_BASE_H_

#include "..\\libs\\RsCommDll\\RsCommDll.h"
#include "..\\libs\\ZsFdb\\ZsFdb.h"
#include "..\\libs\\ProTecLed\\ProTecLed.h"
#include "..\\libs\\ProTecLed\\ProTecLed.config"
#include <string>

enum TEC_PARA
{
	CUR_TEMP,
	DEST_TEMP,
	MAX_TEMP,
	MAX_OUTPUT,
	MAX_CURRENT,
	LIMIT_MAX_TEMP,
	LIMIT_MIN_TEMP,
	INIT_TSC_NUM,
	CTRL_MODE,
	TEC_OPEN,
	TEC_ALL_OPEN,
	TEC_IS_OUTPUT
};

struct STLimitTemp
{
	double dLimitMaxTemp;
	double dLimitMinTemp;
	STLimitTemp()
	{
		dLimitMaxTemp = 0.0;
		dLimitMinTemp = 0.0;
	}
};


enum ProtocolType
{
	PROTOCOL_OLD,
	PROTOCOL_NEW,
};


class CTecBase
{
public:
	CTecBase();
	virtual ~CTecBase();
public:
	virtual bool Exit();
	const char* GetErrDesc();
	void CheckDebugFlag();
public:
	//base 1
	virtual bool SetCtrlPara(int no, double destTemp, double maxTemp, 
			double maxOutput, double maxCurrent, double maxLimtTemp, double minLimtTemp, int mode)=0;
	virtual bool GetCtrlPara(int no, double& destTemp, double& maxTemp, 
			double& maxOutput, double& maxCurrent, double &maxLimtTemp, double &minLimtTemp, int& mode)=0;
	virtual bool GetCurTemp(int no, double& temp)=0;
	virtual bool IsTecOpen(int no, int& isOpen)=0;
	virtual bool OpenTec(int no, int en)=0;
	virtual bool IsAllTecOpen(int& isOpen) = 0;
	virtual bool OpenAllTec(int& isOpen) = 0;
	virtual bool InitLimitTemp(double maxLimtTemp, double minLimtTemp, int& retFlag) = 0;
	virtual bool InitTscNum(int no, double tscNum) = 0;
public:
	virtual int GetActTemp(double* temp, int total)=0;
	virtual int GetActTempTec(double* temp, int total) = 0;
	virtual int GetActTempWater(double& temp, double& temp2) = 0;
	virtual bool CleanAlarmMsg() = 0;
	virtual std::string ExceptionStrMsg() = 0;
	virtual bool JudgeTecValid(int nNo) =0;
	// 纪录错误日志
	virtual void WriteLogMsg(std::string strMsg) = 0;
public:
	STLimitTemp m_stLimitTemp[20];
	ProtocolType  m_Protocolmode;	// 协议类型
	int  m_nTecValid;		// 记录有效的tec设备
protected:
	char m_errDesc[255];
	bool m_isDebug;
	
	
};

class CMyThreadLock
{
public:
	CMyThreadLock();
	~CMyThreadLock();
};

#endif//_TEC_BASE_H_