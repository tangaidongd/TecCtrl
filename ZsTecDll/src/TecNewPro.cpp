#include "stdafx.h"
#include "TecNewPro.h"
#include "log.h"
#include <string>
#include <io.h>
#include <direct.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>



// 构造时自动加锁
CAutoCriticalSection::CAutoCriticalSection(CCriticalSection* p_pCS)
{
	m_pCS = p_pCS;
	if (m_pCS != NULL)
		m_pCS->Lock();
}

// 析构时自动解锁
CAutoCriticalSection::~CAutoCriticalSection()
{
	if (m_pCS != NULL)
		m_pCS->Unlock();
}




CTecNewPro g_tecNewProObj;

ST_ErrorValue g_strErrorFlagMsg[] =
{
	200,	"DMD和TEC热电偶断开",
	200.1,	"DMD热电偶断开",
	200.2,	"TEC热电偶断开",
	200.3,	"冷端温度采集不到",
	200.4,	"冷端温度超范围",
	200.5,	"AD采集异常",
	200.6,	"AD采集出错",
	201,	"DMD和TEC超高温",
	201.1,	"DMD超高温",
	201.2,	"TEC超高温",
	202,	"DMD和TEC超低温",
	202.1,	"DMD超低温",
	202.2,	"TEC超低温",
	202.3,	"TEC铜块结露",
	202.4,	"冷水块结露",
	203,	"DMD和TEC通道自检错误",
	203.1,	"DMD通道自检错误",
	203.2,	"TEC通道自检错误",
	204.1,	"DMD和TEC温差过大",
	204.2,	"TEC持续高电流输出",
	205.1,	"驱动板掉电",
	205.2,	"驱动板输出开路",
	206,	"TEC输出过流",
	207.1,	"TEC电流关闭无效",
	207.2,	"TEC电流输出无效",
	211.2,	"水温热电偶断开",
	211.3,	"水温冷端温度采集不到",
	211.4,	"水温冷端温度超范围",
	211.5,	"水温AD采集异常",
	211.6,	"水温AD采集出错",
	212.1,	"冷水机温度过低",
	212.2,	"冷水机温度过高",
	213,	"结露检测连接异常",
	215,	"电源箱内板连接异常",
	216,	"电源风扇异常",
	217,	"电源箱过温",
	220,	"TEC电源自检未完成",
};

CTecNewPro::CTecNewPro()
{

}

CTecNewPro::~CTecNewPro()
{

}

int CTecNewPro::Init(int com, int total, int nLogFlag, int nLogRefreshCycle, bool bIsPowerDouble)
{
	Exit();
	CheckDebugFlag();

	m_nLogFlag = nLogFlag;
	m_nLogRefreshCycle = nLogRefreshCycle;
	m_bIsPowerDouble = bIsPowerDouble;
	// 打开窗口获取数目等 
	char zComValue[50] = {0};
	sprintf(zComValue, "com%d", com);
	int rt = GetProObj().OpenCom(zComValue);
	if (!rt)
	{
		strcpy(m_errDesc, "open com failed");
		return 0;
	}

	// 连接成功之后，清除一次告警信息
	GetProObj().CommonControlClearExceptionFlag(0); 
	if (m_bIsPowerDouble)
		GetProObj().CommonControlClearExceptionFlag(1);

	if (m_bIsPowerDouble)
		GetProObj().SetPowerDouble(m_bIsPowerDouble);

	// 设备总数没有传，就从设备中自己获取
	if (total == 0)
	{
		int nTempValue = 0;
		if (GetProObj().CommonReadVersion(nTempValue, total, nTempValue, nTempValue, nTempValue))
		{
			strcpy(m_errDesc, "CommonReadVersion failed");
			return 0;
		}	
	}

	m_total = total;
	memset(m_curDmdTemp, 0, sizeof(m_curDmdTemp));
	//确认下该端口通信成功

	int zDmdTemp[25] = { 0 };
	int nTecNum = 0;
	bool bRet = GetProObj().CommonReadDmdTemperature(zDmdTemp, nTecNum);
	if (!bRet)
	{
		GetProObj().CloseCom();
		strcpy(m_errDesc, "get Tec Temp failed");
		return 0;
	}
	
	//建立查询线程
	if (!AfxBeginThread(ThreadReqCurTempNewPro, this, THREAD_PRIORITY_LOWEST))
	{
		strcpy(m_errDesc, "create thread failed");
		Exit();
		return 0;
	}

	int nRet = 0;
	for (int i =0; i < m_total; ++i)
	{
		if (!JudgeTecValid(i+1))
			continue;
		nRet |= (0x01 << i);
	}
	return nRet;
}

bool CTecNewPro::Exit()
{
	//关闭查询线程
	// 这段代码在异常退出时候，会卡死
	m_isThreadRun = false;
	while (m_threadRunState)
	{
		Sleep(200);
	}

	//m_comObj[0].Close();
	GetProObj().CloseCom();
	return false;
}

bool CTecNewPro::SetCtrlPara(int no, double destTemp, double maxTemp, double maxOutput, double maxCurrent, double maxLimtTemp, double minLimtTemp, int mode)
{
	strcpy(m_errDesc, "function invalid");
	return false;
}

bool CTecNewPro::GetCtrlPara(int no, double& destTemp, double& maxTemp, double& maxOutput, double& maxCurrent, double& maxLimtTemp, double& minLimtTemp, int& mode)
{
	strcpy(m_errDesc, "function invalid");
	return false;
}

bool CTecNewPro::GetCurTemp(int no, double& temp)
{
	int nTecNo = no - 1;
	if (nTecNo > m_total)
		return false;

	temp = m_curDmdTemp[nTecNo];
	return true;
}

bool CTecNewPro::IsTecOpen(int no, int& isOpen)
{
	int nTecBitValue;
	int nTotalNum = 0;
	int nTecNo = no - 1;
	if (GetProObj().TecReadTempControlSwitch(nTecBitValue, nTotalNum))
	{
		if (nTecNo > nTotalNum)
			return false;

		isOpen = 0;
		if (nTecBitValue & (1 << nTecNo))
		{
			isOpen = 1;
		}
		return true;
	}
	return false;
}

bool CTecNewPro::OpenTec(int no, int en)
{
	//EM_TEC_WRITE_TEMP_CONTROL_SWITCH,			// 写入TEC温控开关
	//	EM_TEC_READ_TEMP_CONTROL_SWITCH,			// 读取TEC温控开关
	int nTecBitValue;
	int nTotalNum = 0;
	int nTecNo = no - 1;
	if (GetProObj().TecReadTempControlSwitch(nTecBitValue, nTotalNum))
	{
		if (nTecNo > nTotalNum)
			return false;

		if (en)
		{
			nTecBitValue |= (1 << nTecNo);
		}
		else
		{
			nTecBitValue &= ~(1 << nTecNo);
		}
		if (GetProObj().TecWriteTempControlSwitch(nTecBitValue, nTotalNum))
		{
			return true;
		}
	}

	strcpy(m_errDesc, "OpenTec Failed");
	return false;
}

bool CTecNewPro::IsAllTecOpen(int& isOpen)
{
	int nBitValue = 0;
	int nTecNum = 0;
	bool bRet = GetProObj().TecReadTempControlSwitch(nBitValue, nTecNum);
	if (bRet)
	{
		isOpen = nBitValue;
	}

	return bRet;
}

bool CTecNewPro::OpenAllTec(int& isOpen)
{
	/*int nTecBitValue = 0;
	for (int i = 0; i < m_total; ++i)
	{
		nTecBitValue |= (1 << i);
	}*/

	if (GetProObj().TecWriteTempControlSwitch(isOpen, m_total))
	{
		return true;
	}


	return false;
}

bool CTecNewPro::InitLimitTemp(double maxLimtTemp, double minLimtTemp, int& retFlag)
{
	for (int i = 0; i < m_total; ++i)
	{
		m_stLimitTemp[i].dLimitMaxTemp = maxLimtTemp;
		m_stLimitTemp[i].dLimitMinTemp = minLimtTemp;
	}
	return true;
}

int CTecNewPro::GetActTemp(double* temp, int total)
{
	if (total > m_total)
		total = m_total;

	int rt = 0;
	if (!GetProObj().ComIsOpen())
	{
		for (int i = 0; i < total; i++)
		{
			temp[i] = 0;
		}
		return 0;
	}
	for (int i = 0; i < total; i++)
	{
		temp[i] = m_curDmdTemp[i];
		rt += 1;
	}
	return rt;
}

int CTecNewPro::GetActTempTec(double* temp, int total)
{
	if (total > m_total)
		total = m_total;

	int rt = 0;
	if (!GetProObj().ComIsOpen())
	{
		for (int i = 0; i < total; i++)
		{
			temp[i] = 0;
		}
		return 0;
	}
	for (int i = 0; i < total; i++)
	{
		temp[i] = m_curTecTemp[i];
		rt += 1;
	}
	return rt;
}

int CTecNewPro::GetActTempWater(double& temp, double& temp2)
{
	int rt = 0;
	if (!GetProObj().ComIsOpen())
	{
		temp = 0;
		return rt;
	}

	temp = m_curColdWaterTemp[0];
	rt += 1;

	temp2 = 0;
	if(m_bIsPowerDouble)
	{
		temp2 = m_curColdWaterTemp[1];
		rt += 1;
	}
		

	return rt;
}

bool CTecNewPro::CleanAlarmMsg()
{
	bool bRet = GetProObj().CommonControlClearExceptionFlag(0);
	if (bRet)
	{
		int nTecNum = 0;
		int zDmdTemp[MAX_TEC_DATA] = { 0 };
		memset(zDmdTemp, 0, MAX_TEC_DATA * sizeof(int));
		bRet = GetProObj().CommonReadDmdTemperature(zDmdTemp, nTecNum);
		if (bRet)
		{
			SetSomeValue(m_curDmdTemp, zDmdTemp, nTecNum, 0.1, bRet);
		}
		Sleep(110);
	}
	return bRet;
}

std::string CTecNewPro::ExceptionStrMsg()
{
	char zSprintfBuf[500] = { 0 };
	std::string strMsg = "";
	std::string strTecName = "";

	double dFirstError = 0;
	for (int i = 0; i < m_total; ++i)
	{
		// 检查是否有错误，错误信息都是200以上
		if (m_curDmdTemp[i] < 190)
			continue;

		// 210-218之间的错误，之返回错误信息， 其他的，只显示和第一个错误一致的通道
		if (m_curDmdTemp[i] < 218 && m_curDmdTemp[i] > 210)
		{
			strMsg.append(GetAllExceptionStrDetail(m_curDmdTemp[i]));
			break;
		}
		if (abs(dFirstError - 0) < 1e-4)
		{
			dFirstError = m_curDmdTemp[i];
			sprintf(zSprintfBuf, "ch%d", i + 1);
			strTecName.append(zSprintfBuf);
			strMsg.append(GetAllExceptionStrDetail(m_curDmdTemp[i]));
		}
		else if (dFirstError - m_curDmdTemp[i] < 1e-4)
		{
			sprintf(zSprintfBuf, ",ch%d", i + 1);
			strTecName.append(zSprintfBuf);
		}
	}

	if (!strTecName.empty())
	{
		strTecName.append("--");
		strMsg = strTecName + strMsg;
	}
		
	return strMsg;
}

std::string CTecNewPro::GetAllExceptionStrDetail(double dDmdTemp)
{
	std::string strDetail = "";
	int nErrorNum = sizeof(g_strErrorFlagMsg) / sizeof(ST_ErrorValue);
	for (int i = 0; i < nErrorNum; ++i)
	{
		if (abs(dDmdTemp - g_strErrorFlagMsg[i].dErrorFlag) < 1e-4)
		{
			strDetail.append(g_strErrorFlagMsg[i].zErrorMsg);
			break;
		}
	}
	return strDetail;
}

bool CTecNewPro::JudgeTecValid(int nNo)
{
	if (nNo < 1 || nNo > 24)			// 限制序列号返回1-24
	{
		strcpy(m_errDesc, "number invalid");
		return false;
	}

	if ((m_nTecValid >> (nNo - 1) & 0x00000001) == 1)
		return true;
	else
		return false;
}

void CTecNewPro::SetSomeValue(double* pCurTemp, int* pGetTemp, int nTecNum, double dUnit, bool bIsGetSuc /*= true*/)
{
	CAutoCriticalSection AutoLock(&s_CriticalSection11);
	memset(pCurTemp, 0, MAX_TEC_DATA * sizeof(double));
	if (bIsGetSuc)
	{
		for (int i = 0; i < nTecNum; ++i)
		{
			pCurTemp[i] = pGetTemp[i] * dUnit;
		}
		return;
	}

	for (int i = 0; i < nTecNum; ++i)
	{
		pCurTemp[i] = 100;
	}
}



void CTecNewPro::WriteValue2Log()
{
	if (m_total > MAX_TEC_DATA)
		return;

	if (!m_nLogFlag)
		return;

	// 写日志之前判断一次目录，没有就创建
	if (_access(g_pLogDir, 0) == -1)
	{
		_mkdir(g_pLogDir);
	}

	// 向csv文档中写入数据
	bool bFileExsit = true;
	SYSTEMTIME systime;
	GetLocalTime(&systime);//GetSystemTime得到的是格林威治标准时间
	char zFilePath[MAX_PATH] = { 0 };
	sprintf_s(zFilePath, "%s\\%04d%02d%02d.csv", g_pLogDir, systime.wYear, systime.wMonth, systime.wDay);
	if (_access(zFilePath, 0) == -1)
		bFileExsit = false;

	std::ofstream dataFile;
	dataFile.open(zFilePath, std::ios::app);
	if (!dataFile.is_open())
		return;

	if (!bFileExsit)
	{
		dataFile << "时间" << ','
			<< "设备名" << ','
		    << "DMD温度" << ','
			<< "Tec温度" << ','
			<< "电流值" << ','
			<< "冷水块温度";
		dataFile << std::endl;
	}

	double dDmdValue = 0.0;
	double dTecValue = 0.0;
	double deElecValue = 0.0;
	double dColdValue = 0.0;
	std::string strVale;
	char zLogValue[128] = { 0 };

	dColdValue = m_curColdWaterTemp[0];
	for (int i = 0; i < m_total; ++i)
	{
		strVale = "";
		sprintf(zLogValue, "%d %d-%d %d:%d:%d %d", systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds);
		dataFile << zLogValue << ',';
		sprintf(zLogValue, "Tec%d  ", i+1);
		dataFile << zLogValue << ',';
		//strVale.append(zLogValue);

		dDmdValue = m_curDmdTemp[i];
		dTecValue = m_curTecTemp[i];
		deElecValue = m_curElectricTemp[i];
		memset(zLogValue, 0, 128);
		if (m_nLogFlag & EM_FLAG_DMD_TEMP)
		{
			sprintf(zLogValue, "%.2f  ", dDmdValue);
			//strVale.append(zLogValue);
		}
		dataFile << zLogValue << ',';
		memset(zLogValue, 0, 128);
		if (m_nLogFlag & EM_FLAG_TEC_TEMP)
		{
			sprintf(zLogValue, "%.2f  ", dTecValue);
			//strVale.append(zLogValue);
		}
		dataFile << zLogValue << ',';
		memset(zLogValue, 0, 128);
		if (m_nLogFlag & EM_FLAG_ELECTRIC)
		{
			sprintf(zLogValue, "%.2f  ", deElecValue);
			//strVale.append(zLogValue);
		}
		dataFile << zLogValue << ',';
		memset(zLogValue, 0, 128);
		if (m_nLogFlag & EM_FLAG_WATER_TEMP)
		{
			sprintf(zLogValue, "%.2f  ", dColdValue);
			//strVale.append(zLogValue);
		}
		dataFile << zLogValue;
		dataFile << std::endl;
		//DEBUG_LOG(EM_LOG_INFO, "%s", strVale.c_str());	
	}
	dataFile.close();
}


UINT ThreadReqCurTempNewPro(LPVOID pParam)
{
	CTecNewPro* pTec = (CTecNewPro*)pParam;
	pTec->m_isThreadRun = true;
	pTec->m_threadRunState = true;
	int total = pTec->m_total;
	int nLogFlag = pTec->m_nLogFlag;
	int zDmdTemp[MAX_TEC_DATA] = { 0 };
	int zTecValue[MAX_TEC_DATA] = { 0 };
	int zWaterValue[MAX_TEC_DATA] = { 0 };
	int cnt = 0;
	int nSleepTime = 110;
	DWORD tickStart = GetTickCount();
	while (pTec->m_isThreadRun)
	{
		int nTecNum = 0;
		memset(zDmdTemp, 0, MAX_TEC_DATA*sizeof(int));
		bool bRet = GetProObj().CommonReadDmdTemperature(zDmdTemp, nTecNum);
		pTec->SetSomeValue(pTec->m_curDmdTemp, zDmdTemp, nTecNum, 0.01, bRet);
		Sleep(nSleepTime);

		if (nLogFlag & EM_FLAG_ELECTRIC)
		{
			// 电流数据
			int TecValue[MAX_TEC_DATA] = { 0 };
			int LedValue[MAX_TEC_DATA] = { 0 };
			int nTecNum = 0;
			int nLedNum = 0;
			bRet = GetProObj().CommonReadChannalElectric(LedValue, nLedNum, TecValue, nTecNum);
			pTec->SetSomeValue(pTec->m_curElectricTemp, TecValue, nTecNum, 0.01, bRet);
			Sleep(nSleepTime);
		}

		// 电流数据  EM_COMMON_READ_TEC_TEMPERATURE,				// Tec 温度	11	
		memset(zTecValue, 0, MAX_TEC_DATA * sizeof(int));
		nTecNum = 0;
		bRet = GetProObj().CommonReadTecTemperature(zTecValue, nTecNum);
		pTec->SetSomeValue(pTec->m_curTecTemp, zTecValue, nTecNum, 0.1, bRet);
		Sleep(nSleepTime);

		// EM_COMMON_READ_TEC_COLD_WATER_TEMPERATURE,	// TEC冷水块温度		111

		memset(zWaterValue, 0, MAX_TEC_DATA * sizeof(int));
		nTecNum = 0;
		bRet = GetProObj().CommonReadTecColdWaterTemperature(zWaterValue, nTecNum);
		pTec->SetSomeValue(pTec->m_curColdWaterTemp, zWaterValue, nTecNum, 0.1, bRet);
		Sleep(nSleepTime);
	
		if (nLogFlag != 0)
		{
			// 指定周期刷新日志
			DWORD tickEnd = GetTickCount();
			if (tickEnd - tickStart > pTec->m_nLogRefreshCycle)
			{
				pTec->WriteValue2Log();
				tickStart = tickEnd;
			}
		}

	}
	pTec->m_threadRunState = false;
	return 0;
}


